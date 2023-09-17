/*-----------------------------------------------------------------------------
 * EcOs.cpp                 Implementation file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT master OS Layer for VxWorks
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <EcOsPlatform.h>
#include <EcOs.h>

#include <EcError.h>

#include <vxWorks.h>
#include <stdlib.h>
#include <errnoLib.h>
#include <stdio.h>
#include <string.h>
#include <sysLib.h>
#include <tickLib.h>
#include <semLib.h>
#include <logLib.h>
#include <symLib.h>
#include <sysSymTbl.h>
#include <intLib.h>
#include <time.h>

#if (defined CPU) && ((CPU==PENTIUM)||(CPU==PENTIUM4)) && (defined VXWIN)
#  include "arch/i86/pentiumLib.h"
#  include "vxWinLib.h"
#endif

/* vxTimeBaseGet() is also supported in VxWorks 5.4, so we use the old CPU_ macros */
#if defined(CPU_FAMILY) && (CPU_FAMILY==PPC) 
#  include <vxLib.h> /* vxTimeBaseGet */
#endif /* if defined(CPU_FAMILY) && (CPU_FAMILY==PPC) */

#if !(defined SYM_IS_TEXT)
#  define SYM_IS_TEXT(symType) \
     ((EC_T_BOOL)((((SYM_TYPE)(symType))&SYM_TEXT)==SYM_TEXT))
#endif

#if !(defined SYM_IS_GLOBAL)
#  define SYM_IS_GLOBAL(symType) \
     ((EC_T_BOOL)((((SYM_TYPE)(symType))&SYM_GLOBAL)==SYM_GLOBAL))
#endif

/*-DEFINES-------------------------------------------------------------------*/
#define TRACE_FILENAME "ecatTrace.log"

/*
 * If USE_CCNT is defined, use the ARM CCNT as performance counter.
 * It is supported for the ARMv7 architecture (Cortex-A(8,9,15,..) and Cortex-R).
 * See also the ARM Architecture Reference Manual ARMv7-A and ARMv7-R edition.
 *
 * Note: CCNT must first be enabled in privileged mode or the CPU will generate
 * an undefined instruction exception.
 */
#if defined(CPU_FAMILY) && (CPU_FAMILY==ARM)
#  define USE_CCNT
#endif

/*-TYPEDEFS------------------------------------------------------------------*/

typedef struct _LLREGISTER_FINDREGFUNC_PARAMS
{
    EC_T_CHAR* strSymbolName;
    EC_T_CHAR* pFuncAddr;
    SYM_TYPE   oSymType;
    
} LLREGISTER_FINDREGFUNC_PARAMS;

/*-LOCALS--------------------------------------------------------------------*/
static EC_PF_SYSTIME    S_pfSystemTimeGet       = EC_NULL;
#ifdef DEBUGTRACE
static FILE* S_pfTraceFile = EC_NULL;
#endif

#if (defined INCLUDE_OSPERF)
static EC_T_UINT64      S_qwLastPerfTime    = 0;
#endif
static EC_PF_OSDBGMSGHK S_pfOsDbgMsgHook        = EC_NULL;
static EC_T_BOOL        S_bSpinLockActive       = EC_FALSE;         /* flag: if set, no OS calls are allowed */

#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) ) || (_WRS_VXWORKS_MAJOR > 6)
static TASK_ID          S_nSpinLockTaskId       = 0;                /* thread which owns the spin lock */
#else
static EC_T_INT         S_nSpinLockTaskId       = 0;                /* thread which owns the spin lock */
#endif
#if (defined DEBUG)
static EC_T_INT         S_nJobTaskTaskId        = 0;                /* job thread */
#endif

EC_T_DWORD              G_dwOsLockCounter       = 0;
EC_T_DWORD              G_dwOsUnLockCounter     = 0;

#ifdef USE_CCNT
static EC_T_BOOL        S_bCccntEnabled;

#ifndef _DIAB_TOOL
static void CPUEnableCCNT()
{
    /* Make CCNT accessible from usermode */
    __asm__ volatile ("mcr p15, 0, %0, c9, c14, 0" :: "r"(1));
    /* Disable counter flow interrupt */
    __asm__ volatile ("mcr p15, 0, %0, c9, c14, 2" :: "r"(0x8000000f));
    /* Initialize CCNT */
    __asm__ volatile ("mcr p15, 0, %0, c9, c12, 0" :: "r"(5));
    /* Start CCNT */
    __asm__ volatile ("mcr p15, 0, %0, c9, c12, 1" :: "r"(0x80000000));
}
static UINT32 ReadCCNT()
{
    UINT32 tmp;
    __asm__ volatile ("mrc p15, 0, %0, c9, c13, 0" : "=r" (tmp));
    return tmp;
}
#else
__asm volatile void CPUEnableCCNT()
{
! "r2"                      /* r2 clobbered */
    /* Make CCNT accessible from usermode */
    mov r2, #1
    MCR p15, 0, r2, c9, c14, 0
    /* Disable counter flow interrupt */
    mov r2, #0x8000000f
    MCR p15, 0, r2, c9, c14, 2 
    /* Initialize CCNT */
    mov r2, #5
    MCR p15, 0, r2, c9, c12, 0
    /* Start CCNT */
    mov r2, #0x80000000
    MCR p15, 0, r2, c9, c12, 1
}
__asm volatile UINT32 ReadCCNT()
{
! "r0"                      /* r0 clobbered */
    MRC p15, 0, r0, c9, c13, 0
    /* r0 is the return value */
}
#endif /* _DIAB_TOOL */

/*
 * Enable CCNT, so that subsequent (usermode) tasks can 
 * access the CCNT (doesn't apply to VxWorks kernel mode!).
 * The enable must be done in privileged CPU mode.
 * Access permission to CCNT is set for each CPU.
 */
static void EnableCCNT()
{
    cpuset_t curMask;
    cpuset_t origMask;
    cpuset_t enMask = vxCpuEnabledGet();
    
    taskCpuAffinityGet(taskIdSelf(), &origMask); /* Save CPU affinity to restore it later */

    for (unsigned i = 0; i < VX_MAX_SMP_CPUS; ++i)
    {
        if (!CPUSET_ISSET(enMask, i)) continue;
        
        CPUSET_ZERO(curMask);
        CPUSET_SET(curMask, i);
        taskCpuAffinitySet (taskIdSelf(), curMask);
        CPUEnableCCNT();
    }
    
    taskCpuAffinitySet(taskIdSelf(), origMask); /* Restore CPU affinity */
    S_bCccntEnabled = EC_TRUE;
}
#endif /* USE_CCNT */

/********************************************************************************/
/** \brief OS Layer initialization.
*
* \return status code
*/
EC_T_DWORD OsInit(EC_T_OS_PARMS* pInitDesc)
{
    if(pInitDesc != EC_NULL)
    {
        if ((EC_NULL == S_pfSystemTimeGet) && (EC_NULL != pInitDesc->pfSystemTimeGet))
        {
            S_pfSystemTimeGet = pInitDesc->pfSystemTimeGet;
        }
    }
    return EC_E_NOERROR;
}

/********************************************************************************/
/** \brief OS Layer de-initialization.
*
* \return status code
*/
EC_T_DWORD OsDeinit(EC_T_VOID)
{
#ifdef DEBUGTRACE
    if( S_pfTraceFile != EC_NULL ) fclose( S_pfTraceFile );
    S_pfTraceFile = EC_NULL;
#endif
    return EC_E_NOERROR;
}

/********************************************************************************/
/** \brief Add OS Layer Debug Message hook
*
* \return N/A
*/
EC_T_VOID OsAddDbgMsgHook(EC_PF_OSDBGMSGHK pfOsDbgMsgHook)
{
    S_pfOsDbgMsgHook = pfOsDbgMsgHook;
}


/********************************************************************************/
/** \brief 
*
* \return 
*/
EC_T_DWORD OsQueryMsecCount(EC_T_VOID)
{
    EC_T_INT64 llTicks;
    
    llTicks = (EC_T_INT64)tickGet();
    llTicks = llTicks * (EC_T_INT64)1000;
    llTicks = llTicks / (EC_T_INT64)sysClkRateGet();
    return (EC_T_DWORD)llTicks;
}

/********************************************************************************/
/** \brief Open configuration file
*
* \return handle to configuration file or NULL in case of error
*/
EC_T_VOID* OsCfgFileOpen(const EC_T_CHAR* szCfgFileName)
{
    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    return fopen(szCfgFileName, "r");
}

/********************************************************************************/
/** \brief Close configuration file
*
* \return if no error has occurred, OsCfgFileClose returns 0. Otherwise, it returns a nonzero value.
*/
EC_T_INT OsCfgFileClose(EC_T_VOID* pvCfgFile)
{
   EC_T_INT nRet = 0;
    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    if( EC_NULL != pvCfgFile )
    {
        nRet = fclose( (FILE*)pvCfgFile );
    }

    return nRet;
}

/********************************************************************************/
/** \brief Read next chunk of the configuration file
*
* \return number of bytes read
*/
EC_T_INT   OsCfgFileRead(EC_T_VOID* pvCfgFile, EC_T_VOID* pvDst, EC_T_INT nLen)
{
   EC_T_INT nRet = 0;
    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    if( EC_NULL != pvCfgFile )
    {
        nRet = fread(pvDst, 1, nLen, (FILE*)pvCfgFile);
    }

    return nRet;
}

/********************************************************************************/
/** \brief Determine if last OsCfgFileRead operation did cause an error
*
* \return if no error has occurred, OsCfgFileError returns 0. Otherwise, it returns a nonzero value.
*/
EC_T_INT   OsCfgFileError(EC_T_VOID* pvCfgFile)
{
    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    return ferror((FILE*)pvCfgFile);
}

/********************************************************************************/
/** \brief Determine if the end of the configuration file is reached
*
* \return Returns 0 if the current position has not reached the end of the configuration file.
*/
EC_T_INT   OsCfgFileEof(EC_T_VOID* pvCfgFile)
{
    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    return feof((FILE*)pvCfgFile);
}

/********************************************************************************/
/** \brief 
*
* \return
*/
EC_T_VOID OsDbgMsg(const EC_T_CHAR* szFormat, ...)
{
    EC_T_VALIST vaList;

    /* check if spin lock is active */
#ifdef NON_REENTRANT_OSDBGMSG
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));
#else
    if( !S_bSpinLockActive || (S_nSpinLockTaskId != taskIdSelf()) )
#endif
    {
        EC_VASTART(vaList, szFormat);
        if( S_pfOsDbgMsgHook != EC_NULL )
        {
            (*S_pfOsDbgMsgHook)(szFormat, vaList);
        }
        EC_VAEND(vaList);
    }
}

/***************************************************************************************************/
/**
\brief  Get Curretn DBG Message Hook.
\return Dbg Message Hook.
*/
EC_PF_OSDBGMSGHK OsGetDbgMsgHook()
{
    return S_pfOsDbgMsgHook;
}

#ifdef DEBUGTRACE
/********************************************************************************/
/** \brief OS Layer initialization.
*
* \return status code
*/
static void OsOpenTraceFile(void)
{
    if( S_pfTraceFile == EC_NULL )
    {
        S_pfTraceFile = fopen( TRACE_FILENAME, "wb" );
        if( S_pfTraceFile == EC_NULL )
        {
            OsDbgMsg( "OsOpenTraceFile(): cannot open trace file %s\n", TRACE_FILENAME );
        }
        else
        {
            OsDbgMsg( "************************************\n" );
            OsDbgMsg( "TRACE ON: trace data stored in file: %s\n", TRACE_FILENAME );
            OsDbgMsg( "************************************\n" );
        }
    }
}
#endif

/********************************************************************************/
/** \brief Print a trace message
*
* \return N/A
*/
#ifdef OsTrcMsg
#undef OsTrcMsg
#endif
EC_T_VOID OsTrcMsg(const EC_T_CHAR* szFormat, ...)
{
va_list vaArgs;

    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    va_start(vaArgs, szFormat);
#ifdef DEBUGTRACE
    if( S_pfTraceFile == EC_NULL )  OsOpenTraceFile();
    if( S_pfTraceFile != EC_NULL )
    {
        vfprintf( S_pfTraceFile, szFormat, vaArgs);
        fflush( S_pfTraceFile );
    }
    else
    {
        vprintf(szFormat, vaArgs);
    }
#else
    vprintf(szFormat, vaArgs);
#endif
    va_end(vaArgs);
}

#ifdef ASSERT_SUSPEND
/********************************************************************************/
/** \brief 
*
* \return
*/
#ifdef __cplusplus
extern "C"
{
#endif
EC_T_VOID OsDbgAssertFunc(EC_T_BOOL bAssertCondition, EC_T_CHAR* szFile, EC_T_DWORD dwLine)
{
    if( !bAssertCondition )
    {
        printf("ASSERTION in file %s, line %d\n", szFile, dwLine);
        taskSuspend(0);
    }
}
#ifdef __cplusplus
}
#endif
#endif




/********************************************************************************/
/** \brief Create a synchronization mutual exclusion object
*
* \return handle to the mutex object.
*/
EC_T_VOID* OsCreateLock(EC_T_VOID)
{
    return OsCreateLockTyped(eLockType_DEFAULT);
}



/********************************************************************************/
/** \brief Create a synchronization mutual exclusion object
*
* \return handle to the mutex object.
*/
EC_T_VOID* OsCreateLockTyped(EC_T_OS_LOCK_TYPE   eLockType)
{
OS_LOCK_DESC* pvLockDesc = (OS_LOCK_DESC*)OsMalloc(sizeof(OS_LOCK_DESC));

    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    if( pvLockDesc != EC_NULL )
    {
        pvLockDesc->nLockCnt = 0;
        pvLockDesc->eLockType = eLockType; 

        switch(pvLockDesc->eLockType)
        {
        default:
            OsDbgAssert( EC_FALSE );
        case eLockType_DEFAULT:     /*< Default mutex           */
        case eLockType_INTERFACE:   /*< only interface */
            pvLockDesc->semMId = semMCreate(SEM_Q_PRIORITY|SEM_DELETE_SAFE|SEM_INVERSION_SAFE);
            break;

        /*< only jobs --> spin lock */
        case eLockType_SPIN:
#ifdef USE_SPINLOCKS
            pvLockDesc->semMId = 0;
            SPIN_LOCK_ISR_INIT( &pvLockDesc->SpinLock, 0 );
#else
            pvLockDesc->semMId = semMCreate(SEM_Q_PRIORITY|SEM_DELETE_SAFE|SEM_INVERSION_SAFE);
#endif
            break;
        }
    }

    OsDbgAssert(EC_NULL != pvLockDesc);
    
    return pvLockDesc;
}


/********************************************************************************/
/** \brief Delete a mutex object
*
* \return N/A
*/
EC_T_VOID  OsDeleteLock(EC_T_VOID* pvLockHandle)
{
OS_LOCK_DESC* pvLockDesc = (OS_LOCK_DESC*)pvLockHandle;

    /* check if spin lock is active */
    OsDbgAssert( !(S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())) );

    if (pvLockDesc != EC_NULL)
    {
        OsDbgAssert( pvLockDesc->nLockCnt == 0 );

        switch(pvLockDesc->eLockType)
        {
        
        default:
            OsDbgAssert( EC_FALSE );
        case eLockType_DEFAULT:     /*< Default mutex           */
        case eLockType_INTERFACE:   /*< only interface */
            OsDbgAssert( pvLockDesc->semMId != 0 );
            semDelete((SEM_ID)pvLockDesc->semMId);
            break;

        /*< only jobs --> spin lock */
        case eLockType_SPIN:
#ifdef USE_SPINLOCKS
            /* nothing to do */
#else
            OsDbgAssert( pvLockDesc->semMId != 0 );
            semDelete((SEM_ID)pvLockDesc->semMId);
#endif
            break;
        }

        OsFree( pvLockDesc );
    }
}


/********************************************************************************/
EC_T_VOID  OsLock(EC_T_VOID* pvLockHandle)
{
OS_LOCK_DESC* pvLockDesc = (OS_LOCK_DESC*)pvLockHandle;
#ifdef DEBUG
int nTid;
    nTid = taskIdSelf();
#endif

    OsDbgAssert( pvLockDesc != EC_NULL );
    if (pvLockDesc != NULL)
    {
        switch(pvLockDesc->eLockType)
        {
        
        default:
            OsDbgAssert( EC_FALSE );
        case eLockType_DEFAULT:     /*< Default mutex           */
        case eLockType_INTERFACE:   /*< only interface */
            OsDbgAssert( pvLockDesc->semMId != 0 );
            /* it is not allowed to call oslock inside the job task with a lock type not equal to spin */
            OsDbgAssert(nTid != S_nJobTaskTaskId);
            semTake((SEM_ID)pvLockDesc->semMId, WAIT_FOREVER);
            break;
        
        /*< only jobs --> spin lock */
        case eLockType_SPIN:
#ifdef USE_SPINLOCKS
            if( spinLockIsrHeld( &pvLockDesc->SpinLock ) )
            {
            }
            else
            {
                SPIN_LOCK_ISR_TAKE( &pvLockDesc->SpinLock );
                OsDbgAssert( pvLockDesc->nLockCnt == 0 );
            }
#else /* #ifdef USE_SPINLOCKS */
            OsDbgAssert( pvLockDesc->semMId != 0 );
            semTake((SEM_ID)pvLockDesc->semMId, WAIT_FOREVER);
#endif /* #else USE_SPINLOCKS */
#ifdef DEBUG
            S_bSpinLockActive = EC_TRUE;
            S_nSpinLockTaskId = nTid;
#endif
            break;
        }
        pvLockDesc->nLockCnt++;
        G_dwOsLockCounter++;

#ifdef DEBUG
        pvLockDesc->nTaskId = nTid;
        if( pvLockDesc->nLockCnt >= 10 )
        {
            /* that much nesting levels? */
            OsDbgMsg( "FATAL Error in OsLock(): pvLockDesc = 0x%x, nLockCnt = %s, tid = 0x%x\n", pvLockDesc, pvLockDesc->nLockCnt, pvLockDesc->nTaskId );
            OsDbgAssert( EC_FALSE );
        }
#endif
    }
}


/********************************************************************************/
EC_T_VOID  OsUnlock(EC_T_VOID* pvLockHandle)
{
OS_LOCK_DESC* pvLockDesc = (OS_LOCK_DESC*)pvLockHandle;

    OsDbgAssert( pvLockDesc != EC_NULL );
    if (pvLockDesc != NULL)
    {
        OsDbgAssert( pvLockDesc->nLockCnt > 0 );
        G_dwOsUnLockCounter++;
        pvLockDesc->nLockCnt--;
        if( pvLockDesc->nLockCnt == 0 )
        {
#ifdef DEBUG
            S_bSpinLockActive = EC_FALSE;
            S_nSpinLockTaskId = 0;
            pvLockDesc->nTaskId = -1;
#endif
        }

        switch(pvLockDesc->eLockType)
        {
        
        default:
            OsDbgAssert( EC_FALSE );
        case eLockType_DEFAULT:     /*< Default mutex           */
        case eLockType_INTERFACE:   /*< only interface */
            OsDbgAssert( pvLockDesc->semMId != 0 );
            semGive((SEM_ID)pvLockDesc->semMId);
            break;

        /*< only jobs --> spin lock */
        case eLockType_SPIN:
#ifdef USE_SPINLOCKS
            if( spinLockIsrHeld( &pvLockDesc->SpinLock ) )
            {
                OsDbgAssert( 0 == (vxEflagsGet() & 0x200) );
                if( 0 == pvLockDesc->nLockCnt )
                {
                    SPIN_LOCK_ISR_GIVE( &pvLockDesc->SpinLock );
                }
            }
            else
            {
                OsDbgAssert(FALSE);
            }
#else
            OsDbgAssert( pvLockDesc->semMId != 0 );
            semGive((SEM_ID)pvLockDesc->semMId);
#endif
            break;

        }
    }
}


/********************************************************************************/
/** \brief Create a binary semaphore
*
* \return event object to be referenced in further calls or EC_NULL in case of errors.
*/
EC_T_VOID* OsCreateEvent(EC_T_VOID)
{
    SEM_ID     semBId;
    EC_T_VOID* pvRetVal = EC_NULL;
    
    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    /* create event */
    semBId = semBCreate( SEM_Q_FIFO, SEM_EMPTY );
    if (semBId == NULL)
    {
        goto Exit;
    }
    /* no errors */
    pvRetVal = (EC_T_VOID*)semBId;
    
Exit:
    
    return pvRetVal;
}

/********************************************************************************/
/** \brief  Delete an event object
*
* \return N/A.
*/
EC_T_VOID OsDeleteEvent(EC_T_VOID* pvEvent)
{
    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    /* delete event */
    semDelete((SEM_ID)pvEvent);
}

/********************************************************************************/
/** \brief  Wait for an event object
*
* \return EC_E_NOERROR if event was set for the timeout, or error code in case of errors.
*/
EC_T_DWORD OsWaitForEvent(EC_T_VOID* pvEvent, EC_T_DWORD dwTimeout)
{
    STATUS Status;
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    int   nTicks;
    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    /* convert timeout */
    switch (dwTimeout)
    {
    case EC_NOWAIT:       nTicks = NO_WAIT;         break;
    case EC_WAITINFINITE: nTicks = WAIT_FOREVER;    break;
    default:
       {
          nTicks = sysClkRateGet() * dwTimeout / 1000;
          if (nTicks < 2) nTicks = 2;
          break;
       }
    }
    /* wait for event */
    Status = semTake((SEM_ID)pvEvent, nTicks);
    
    /* convert return value */
    if( Status == ERROR )
    {
        switch (errnoGet())
        {
        case S_objLib_OBJ_UNAVAILABLE:  dwRetVal = EC_E_TIMEOUT; break;
        case S_objLib_OBJ_TIMEOUT:      dwRetVal = EC_E_TIMEOUT; break;
        default:                        dwRetVal = EC_E_ERROR;   break;
        }
    }
    else
    {
        dwRetVal = EC_E_NOERROR;
    }
    
    return dwRetVal;
}



/********************************************************************************/
/** \brief Create thread.
*
* \return thread object or EC_NULL in case of an error.
*/
EC_T_VOID*  OsCreateThread(
                                       EC_T_CHAR* szThreadName, 
                                       EC_PF_THREADENTRY pfThreadEntry, 
                                       EC_T_DWORD dwPrio, 
                                       EC_T_DWORD dwStackSize, 
                                       EC_T_VOID* pvParams 
                                       )
{
    EC_T_VOID* pvThreadObject;
    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) ) || (_WRS_VXWORKS_MAJOR > 6)
    pvThreadObject = (EC_T_VOID*)taskSpawn( szThreadName, (int)dwPrio, VX_FP_TASK, (int)dwStackSize, (FUNCPTR)pfThreadEntry, (_Vx_usr_arg_t)pvParams, 2,3,4,5,6,7,8,9,10 );
#else
    pvThreadObject = (EC_T_VOID*)taskSpawn( szThreadName, (int)dwPrio, VX_FP_TASK, (int)dwStackSize, (FUNCPTR)pfThreadEntry, (int)pvParams, 2,3,4,5,6,7,8,9,10 );
#endif
    if (pvThreadObject == (EC_T_VOID*)ERROR)
    {
        pvThreadObject = EC_NULL;
        goto Exit;
    }

#if (defined DEBUG)
    /* is this the job task ? */
    if( OsStrcmp(szThreadName, "tEcJobTask") == 0)
    {
        S_nJobTaskTaskId = (EC_T_INT)pvThreadObject;
    }
#endif
    
Exit:
    return pvThreadObject;
}

/***************************************************************************************************/
/**
\brief  Delete a thread Handle return by OsCreateThread.
*/
EC_T_VOID   OsDeleteThreadHandle(
   EC_T_VOID* pvThreadObject      /**< [in]   Previously allocated Thread Handle */
                                 )
{
    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

  EC_UNREFPARM(pvThreadObject);
}

/********************************************************************************/
/** \brief Set thread priority.
*
* \return thread object or EC_NULL in case of an error.
*/
EC_T_VOID   OsSetThreadPriority( 
                                            EC_T_VOID* pvThreadObject, 
                                            EC_T_DWORD dwPrio 
                                            )
{
    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) ) || (_WRS_VXWORKS_MAJOR > 6)
    taskPrioritySet((TASK_ID)pvThreadObject, (int)dwPrio);
#else
    taskPrioritySet((int)pvThreadObject, (int)dwPrio);
#endif
}

#ifdef _WRS_VX_SMP
/********************************************************************************/
/** \brief Set thread affinity.
*
* \return EC_TRUE if successful, EC_FALSE otherwise.
*/
EC_T_BOOL   OsSetThreadAffinity(
    EC_T_VOID* pvThreadObject,
    EC_T_CPUSET CpuSet
)
{
EC_T_CPUSET CpuSetResult;
STATUS Status = OK;

#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) ) || (_WRS_VXWORKS_MAJOR > 6)
    TASK_ID nTaskId = (TASK_ID)pvThreadObject;
#else
    int nTaskId = (int)pvThreadObject;
#endif

    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    Status = taskCpuAffinitySet( nTaskId, CpuSet );
    if( Status == OK )
    {
        Status = taskCpuAffinityGet( nTaskId, &CpuSetResult );
        if( Status == OK )
        {
            if( OsMemcmp(&CpuSet,&CpuSetResult,sizeof(EC_T_CPUSET))!=0 )
            {
                OsDbgMsg( "OsSetThreadAffinity failed, set value != act value!!!!\n" );   
                OsDbgMsg( "OsSetThreadAffinity failed, set value != act value!!!!\n" );   
                OsDbgMsg( "OsSetThreadAffinity failed, set value != act value!!!!\n" );   
                OsDbgMsg( "OsSetThreadAffinity failed, set value != act value!!!!\n" );   
                OsDbgMsg( "OsSetThreadAffinity failed, set value != act value!!!!\n" );   
                OsDbgMsg( "OsSetThreadAffinity failed, set value != act value!!!!\n" );
                Status = ERROR;
            }
        }
    }

    return (Status == OK);
}


/********************************************************************************/
/** \brief Get thread affinity.
*
* \return EC_TRUE if successful, EC_FALSE otherwise.
*/
EC_T_BOOL   OsGetThreadAffinity(
    EC_T_VOID* pvThreadObject,
    EC_T_CPUSET* pCpuSet
)
{
STATUS Status = OK;

#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) ) || (_WRS_VXWORKS_MAJOR > 6)
    TASK_ID nTaskId = (TASK_ID)pvThreadObject;
#else
    int nTaskId = (int)pvThreadObject;
#endif

    /* check if spin lock is active */
    OsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    Status = taskCpuAffinityGet( nTaskId, pCpuSet );

    return (Status == OK);
}
#else
/********************************************************************************/
/** \brief Set thread affinity.
*
* \return EC_TRUE if successful, EC_FALSE otherwise.
*/
EC_T_BOOL   OsSetThreadAffinity(
    EC_T_VOID* pvThreadObject,
    EC_T_CPUSET CpuSet
)
{
EC_UNREFPARM(pvThreadObject);
EC_UNREFPARM(CpuSet);
EC_T_BOOL bOk = EC_TRUE;

   if( (int)CpuSet != 1 )
   {
       OsDbgMsg( "OsSetThreadAffinity failed (OS-Layer with no SMP Support), CpuSet is not 1!!!!\n" );   
       OsDbgMsg( "OsSetThreadAffinity failed (OS-Layer with no SMP Support), CpuSet is not 1!!!!\n" );   
       OsDbgMsg( "OsSetThreadAffinity failed (OS-Layer with no SMP Support), CpuSet is not 1!!!!\n" );   
       OsDbgMsg( "OsSetThreadAffinity failed (OS-Layer with no SMP Support), CpuSet is not 1!!!!\n" );   
       OsDbgMsg( "OsSetThreadAffinity failed (OS-Layer with no SMP Support), CpuSet is not 1!!!!\n" );   
       OsDbgMsg( "OsSetThreadAffinity failed (OS-Layer with no SMP Support), CpuSet is not 1!!!!\n" );   
       OsDbgMsg( "OsSetThreadAffinity failed (OS-Layer with no SMP Support), CpuSet is not 1!!!!\n" );   
       bOk = EC_FALSE;
   }
    return bOk;
}


/********************************************************************************/
/** \brief Get thread affinity.
*
* \return EC_TRUE if successful, EC_FALSE otherwise.
*/
EC_T_BOOL   OsGetThreadAffinity( 
    EC_T_VOID*  pvThreadObject, 
    EC_T_CPUSET* pCpuSet
)
{
EC_UNREFPARM(pvThreadObject);
EC_UNREFPARM(pCpuSet);
    return EC_FALSE;
}
#endif

/***************************************************************************************************/
/**
\brief  Call a routine to examine each entry in a symbol table 

\return TRUE if symEach( ) is to continue calling it for each entry, or FALSE if it is done and symEach( ) can exit. 
*/
EC_T_BOOL FindSymbol(
   char *                       strCurrSymbolName,  /* symbol/entry name           */
   int                          nVal,             /* symbol/entry value          */
   SYM_TYPE                     oSymType,         /* symbol/entry type           */
   LLREGISTER_FINDREGFUNC_PARAMS*    poFindParams,       /* arbitrary user-supplied arg */
   UINT16                       wGroup            /* symbol/entry group number   */
)
{
BOOL    bRetVal = FALSE;
STATUS    oStatus = ERROR;

    /* Check if the current symbol name (eventually mangled) contains matches the 
     * search symbol name */
    if( NULL == strstr (strCurrSymbolName, poFindParams->strSymbolName) )
    {
       bRetVal = TRUE; /* continue search */
       goto Exit;
    }

    /* The current symbol name matches. Now get the symbol address and symbol type */

#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) ) || (_WRS_VXWORKS_MAJOR > 6)
    SYMBOL_DESC SymDesc;
    memset(&SymDesc, 0, sizeof(SYMBOL_DESC));
    SymDesc.mask = SYM_FIND_BY_NAME;
    SymDesc.name = strCurrSymbolName;
    SymDesc.nameLen = strlen(strCurrSymbolName);
    oStatus = symFind( sysSymTbl, &SymDesc);
    poFindParams->pFuncAddr = SymDesc.value;
    poFindParams->oSymType = SymDesc.type;   
#else
    oStatus = symFindByName( sysSymTbl, strCurrSymbolName, &poFindParams->pFuncAddr, &poFindParams->oSymType);
#endif
    if (OK == oStatus)
    {
       bRetVal = FALSE; /* stop search symbol */
    }
    else 
    {
       bRetVal = TRUE;  /* continue search */
    }    
 Exit:
    
    /* TRUE: if symEach( ) is to continue calling it for each entry 
     * FALSE: if it is done and symEach( ) can exit. */
    return (bRetVal);
}


/***************************************************************************************************/
/**
\brief  Get Link Layer Driver Registration function

This function returns the link layer registration function.
\return link layer registration function.
*/
EC_PF_LLREGISTER OsGetLinkLayerRegFunc( EC_T_CHAR* szDriverIdent )
{
    EC_T_CHAR           szRegisterFunction[80];
    EC_PF_LLREGISTER    pfLlRegister            = EC_NULL;
    LLREGISTER_FINDREGFUNC_PARAMS oFindSymParams = {0};

    strcpy( szRegisterFunction, "emllRegister");
    strcat( szRegisterFunction, szDriverIdent );
    
    oFindSymParams.strSymbolName  = szRegisterFunction;
    oFindSymParams.pFuncAddr     = EC_NULL;
    oFindSymParams.oSymType      = 0;
    
#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) ) || (_WRS_VXWORKS_MAJOR > 6)
    symEach( sysSymTbl, (FUNCPTR)&FindSymbol, (_Vx_usr_arg_t)&oFindSymParams );
#else
    symEach( sysSymTbl, (FUNCPTR)&FindSymbol, (int)&oFindSymParams );
#endif
    
    if( NULL == oFindSymParams.pFuncAddr)
    {
      OsDbgMsg( "OsGetLinkLayerRegFunc: cannot find function %s \n", szRegisterFunction );
      pfLlRegister = EC_NULL;
        goto Exit;
    }
    
    if(!(SYM_IS_TEXT(oFindSymParams.oSymType ) && SYM_IS_GLOBAL(oFindSymParams.oSymType )))
    {
      OsDbgMsg( "OsGetLinkLayerRegFunc: cannot find function %s \n", szRegisterFunction );
      pfLlRegister = EC_NULL;
        goto Exit;       
    }
    
    pfLlRegister = (EC_PF_LLREGISTER)oFindSymParams.pFuncAddr;
    
Exit:
    return pfLlRegister;
}

#if (defined INCLUDE_OSPERF)
/*******************************************************************************
 *
 * plTimestampIsr - performance logging timestamp ISR
 *
 */
void               /* Return: N/A */
plTimestampIsr(
    int nArg       /* argument given in sysTimestampConnect */
              )
{
    S_qwLastPerfTime = EC_MAKEQWORD(EC_HIDWORD(S_qwLastPerfTime)+1, EC_LODWORD(S_qwLastPerfTime));
}
#endif

EC_T_DWORD OsSystemTimeGet(
                           EC_T_UINT64* pqwSystemTime
                           )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    /* check parameters */
    if( EC_NULL == pqwSystemTime )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get system time */
    if( EC_NULL != S_pfSystemTimeGet )
    {
        /* from configured function */
        dwRes = S_pfSystemTimeGet(pqwSystemTime);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    else
    {
    time_t ltime;

        time(&ltime);
        ltime = ltime - 946684800ul;
        EC_SETQWORD(pqwSystemTime, (((EC_T_UINT64)ltime)*1000000000ul));
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

#if (defined EC_SOCKET_IP_SUPPORTED)
ATECAT_API EC_T_DWORD OsSocketInit(EC_T_VOID)
{
    return EC_E_NOERROR;
}

ATECAT_API EC_T_DWORD OsSocketDeInit(EC_T_VOID)
{
    return EC_E_NOERROR;
}
#endif


static EC_T_BOOL   S_bCalibrated       = EC_FALSE;
static EC_T_UINT64 S_dwlHzFrequency    = 0;        // frequency in Hz
static EC_T_DWORD  S_dw100kHzFrequency = 0;        // frequency in 100 kHz units (e.g. 1MHz = 10)

#if defined(_DIAB_TOOL) && defined(_WRS_VXWORKS_MAJOR) && defined(_VX_CPU_FAMILY)
#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 8) ) || (_WRS_VXWORKS_MAJOR > 6) 
#if _VX_CPU_FAMILY==_VX_I80X86

#  define DIAB_TSC_SUPPORTED
/*
 * Read TSC (returns eax:edx)
 */
asm volatile EC_T_UINT64 __rdtsc()
{
   rdtsc
}
#endif /* if _VX_CPU_FAMILY==_VX_I80X86 */
#endif /* if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 8) ) || (_WRS_VXWORKS_MAJOR > 6) */
#endif /* if defined(_DIAB_TOOL) && defined(_WRS_VXWORKS_MAJOR) && defined(_VX_CPU_FAMILY) */
   
/***************************************************************************************************/
/**
@brief  OsMeasGetCounterTicks

@return the current TSC count [EC_T_UINT64]
*/
EC_T_UINT64 OsMeasGetCounterTicks( EC_T_VOID )
{
   EC_T_DWORD dwTimeStampLo = 0;
   EC_T_DWORD dwTimeStampHi = 0;
   
/* ++ X86
 * Read TSC
 */
#if defined(CPU_FAMILY) && (CPU_FAMILY==I80X86)
#  if defined(__GNUC__)
   __asm__ volatile (
                "rdtsc"
                : "=a" (dwTimeStampLo), "=d" (dwTimeStampHi)
                );
   return EC_MAKEQWORD(dwTimeStampHi, dwTimeStampLo);
#elif defined(DIAB_TSC_SUPPORTED)
   EC_UNREFPARM(dwTimeStampLo);
   EC_UNREFPARM(dwTimeStampHi);
   return __rdtsc();
#  else
#  warning "No timestamp counter support for this compiler!"
   return 0;
#  endif
/* -- X86 */

   
/* ++ PowerPC
 * Read PowerPC architecture's time base register.
 */
#elif defined(CPU_FAMILY) && (CPU_FAMILY==PPC) /* vxTimeBaseGet() is also supported in VxWorks 5.4, so we use the old CPU_ macros */
   /* vxTimeBaseGet should also work with the DIAB compiler. */
   vxTimeBaseGet((UINT32 *) &dwTimeStampHi, (UINT32 *) &dwTimeStampLo); /* check for wrap is already done in vxTimeBaseGet() */
   return EC_MAKEQWORD(dwTimeStampHi, dwTimeStampLo);
/* -- PowerPC */

/* ++ ARM Cortex
 * Read CCNT
 */   
#elif defined USE_CCNT
   EC_UNREFPARM(dwTimeStampHi);
   static EC_T_DWORD s_dwTimeStampHi = 0;
   static EC_T_DWORD s_dwTimeStampLoOld = 0;
   
   if (! S_bCccntEnabled) 
   {
      EnableCCNT();
   }
   dwTimeStampLo = ReadCCNT();
   
   if (dwTimeStampLo < s_dwTimeStampLoOld)
   {
      s_dwTimeStampHi = s_dwTimeStampHi + 1;
   }
   s_dwTimeStampLoOld = dwTimeStampLo;
   
   return EC_MAKEQWORD(s_dwTimeStampHi, dwTimeStampLo);
/* -- ARM Cortex */
   
/* +++ Porting required */
#else
#  warning "Timestamp counter on this platform is not supported!"
#endif   
/* --- Porting required */
}


/***************************************************************************************************/
/**
@brief  OsMeasCalibrate

Measure the TSC frequency for timing measurements.
This function must be called before using one of the following functions:
*/
EC_T_VOID OsMeasCalibrate(EC_T_UINT64 dwlFreqSet)
{
    EC_T_INT nThreadPriority = 0;

    if( !S_bCalibrated )
    {
        if( dwlFreqSet == 0 )
        {
            /* auto calibrate */
            OsDbgMsg( "Calibrate tsc measurement... " );

            taskPriorityGet(taskIdSelf(), &nThreadPriority);
            taskPrioritySet(taskIdSelf(), 0);
            {
                EC_T_UINT64 dwlTimeStamp1 = 0;
                EC_T_UINT64 dwlTimeStamp2 = 0;
                EC_T_UINT64 dwlTimeStamp3 = 0;

                OsSleep( 1 );
                dwlTimeStamp1  = OsMeasGetCounterTicks();
                OsSleep( 1000 );
                dwlTimeStamp2  = OsMeasGetCounterTicks();
                OsSleep( 2000 );
                dwlTimeStamp3  = OsMeasGetCounterTicks();

                S_dwlHzFrequency = ((dwlTimeStamp3 - dwlTimeStamp2)/2 + (dwlTimeStamp2 - dwlTimeStamp1))/2;
            }
            taskPrioritySet(taskIdSelf(), nThreadPriority);
            OsDbgMsg( "done: %d MHz\n", S_dwlHzFrequency/1000000 );
        }
        else
        {
            S_dwlHzFrequency = dwlFreqSet;
        }
        S_dw100kHzFrequency = (EC_T_DWORD)(S_dwlHzFrequency / 100000);
    }
    else if( dwlFreqSet != 0 )
    {
        /* two different frequencies should not happen */
        OsDbgAssert( dwlFreqSet == S_dwlHzFrequency );
    }
    S_bCalibrated = EC_TRUE;
}

/***************************************************************************************************/
/**
@brief  OsMeasGet100kHzFrequency

@return the frequency of the TSC in 100kHz multiples [EC_T_DWORD]
*/
EC_T_DWORD OsMeasGet100kHzFrequency(
                                        EC_T_VOID
                                        )
{
    EC_T_DWORD     dwRetVal     = 0;

    if( !S_bCalibrated )
    {
        OsDbgMsg("TscGet100kHzFrequency() Tsc not calibrated\n");
        goto Exit;
    }

    // no errors
    dwRetVal = S_dw100kHzFrequency;

Exit:
    return( dwRetVal );
}



/*-END OF SOURCE FILE--------------------------------------------------------*/
