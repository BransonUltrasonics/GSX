/*-----------------------------------------------------------------------------
 * EcOsPlatform.h           
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              AT-EM OS-Layer header file for VxWorks platform
 *---------------------------------------------------------------------------*/

#ifndef INC_ECOSPLATFORM
#define INC_ECOSPLATFORM

/*-SUPPORT-SELECTION----------------------------------------------------------*/
#define EC_SOCKET_IP_SUPPORTED

/*-INCLUDES------------------------------------------------------------------*/
#include <vxWorks.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysLib.h>
#include <taskLib.h>

#if defined(_WRS_VX_SMP) && defined(CPU_FAMILY) && (CPU_FAMILY==I80X86)
#  define USE_SPINLOCKS /* Spinlock implementation is X86 specific */
#endif /* if defined(_WRS_VX_SMP) && defined(CPU_FAMILY) && (CPU_FAMILY==I80X86) */

#ifdef USE_SPINLOCKS
#  include <spinLockLib.h>
#endif

#ifdef _WRS_VX_SMP
#  include <cpusetCommon.h>
#  if !(VX_CPU_FAMILY==ppc)
#  	  include <arch/i86/vxI86Lib.h>
#  endif
#  ifdef __cplusplus
#    include <new.h>
#  endif
#endif /* ifdef _WRS_VX_SMP */

#ifdef EC_SOCKET_IP_SUPPORTED
#  include <ioLib.h>
#  include <sockLib.h>
#  include <inetLib.h>
#  include <selectLib.h>
#  include <netinet/tcp.h>
#endif /* #ifdef EC_SOCKET_IP_SUPPORTED */

#include <EcType.h>

/*-OS-VERSION-IDENTIFICATION--------------------------------------------------*/
#ifndef EC_VERSION_VXWORKS
#define EC_VERSION_VXWORKS ((_WRS_VXWORKS_MAJOR*100) + (_WRS_VXWORKS_MINOR*10))
#endif

#ifndef ATECAT_PLATFORMSTR
#if   EC_VERSION_VXWORKS == 550
#define ATECAT_PLATFORMSTR "VxWorks55"
#elif EC_VERSION_VXWORKS == 680
#define ATECAT_PLATFORMSTR "VxWorks68"
#elif EC_VERSION_VXWORKS == 690
#define ATECAT_PLATFORMSTR "VxWorks69"
#elif EC_VERSION_VXWORKS == 700
#define ATECAT_PLATFORMSTR "VxWorks70"
#else
#define ATECAT_PLATFORMSTR "VxWorks"
#endif
#endif

/*-DEFINES-------------------------------------------------------------------*/

/* Structure pack */
#define EC_PACKED_INCLUDESTART(Bytes)   <EcOsPlatform.h>
#define EC_PACKED_INCLUDESTOP           <EcOsPlatform.h>
#define EC_PACKED(Bytes)                __attribute__((aligned(Bytes),packed))

#define ATECAT_API
#if (defined _VX_CPU_FAMILY) && (_VX_CPU_FAMILY==_VX_ARM)
#  define WITHALIGNMENT
#elif (defined CPU) && (CPU==ARMARCH5)
#  define WITHALIGNMENT
#else
#  undef WITHALIGNMENT
#endif

#if (defined _VX_CPU_FAMILY) && (_VX_CPU_FAMILY==_VX_PPC)
#  define EC_BIG_ENDIAN 1
#endif

/* using the DEBUG definition: ASSERTions are activated */
/*#define DEBUG*/
/* using the ASSERT_SUSPEND definition: the asserting task will be suspended */
#define ASSERT_SUSPEND


#ifdef _WRS_VX_SMP
#  define EC_CPUSET_DEFINED
   typedef cpuset_t    EC_T_CPUSET;        /* CPU-set for SMP systems */
#  define EC_CPUSET_ZERO(CpuSet)          CPUSET_ZERO((CpuSet))
#  define EC_CPUSET_SET(CpuSet,nCpuIndex) CPUSET_SET((CpuSet),(nCpuIndex))
#  define EC_CPUSET_SETALL(CpuSet)        CPUSET_SETALL((CpuSet))       
#endif

/*-MACROS--------------------------------------------------------------------*/
/* define here if tracing shall be enabled */
/*#define DEBUGTRACE*/

#define EC_INLINEATTRIBUTE
#if (defined __cplusplus)
  #define EC_INLINEKEYWORD __inline
#else /* __cplusplus */
  #define EC_INLINEKEYWORD __inline__
#endif /* __cplusplus */

#define EC_INLINEINT(dummy, functiondecl)     \
    EC_INLINEKEYWORD dummy ##functiondecl EC_INLINEATTRIBUTE; \
    static EC_INLINEKEYWORD dummy ##functiondecl 

#define EC_INLINE(funcdecl) EC_INLINEINT(,funcdecl)


#ifdef __cplusplus
extern "C"
{
#endif


/* use macros for the most important OS layer routines */
#define OsSetLastError(dwError)			dwError

#define OsSleep(dwMsec)                         taskDelay(  EC_MAX( ((sysClkRateGet() * (UINT32)(dwMsec) + 500)/1000) , 1 )  )

#ifdef DEBUG
#  ifdef ASSERT_SUSPEND
#    define OsDbgAssert(bCond)                      OsDbgAssertFunc((bCond),__FILE__,__LINE__)
     EC_T_VOID OsDbgAssertFunc(EC_T_BOOL bAssertCondition, EC_T_CHAR* szFile, EC_T_DWORD dwLine);
#  else
#    define OsDbgAssert                             assert
#  endif
#endif

#ifndef OsDbgAssert
#  define OsDbgAssert(x)
#endif

#define OsSetEvent(semBEvent)                   semGive((SEM_ID)(semBEvent))
#define OsResetEvent(semBEvent)                 semTake((SEM_ID)(semBEvent),NO_WAIT)
     
#ifdef _WRS_VX_SMP /* Use the old CPU_ macros, because we also want VxWork 5.4 support */
#  if defined(CPU_FAMILY) && (CPU_FAMILY==I80X86)
#    define OsMemoryBarrier()  _WRS_ASM("mfence");
#  elif defined(CPU_FAMILY) && (CPU_FAMILY==PPC)
#    define OsMemoryBarrier()  _WRS_ASM("sync");
#  elif defined(CPU_FAMILY) && (CPU_FAMILY==ARM)
#    define OsMemoryBarrier()  VX_SYNC_BARRIER()
#  else
#    error Porting required
#  endif
#else
#  define OsMemoryBarrier()
#endif /* ifdef _WRS_VX_SMP */

#define OsStricmp  OsStrcmp

#ifdef EC_SOCKET_IP_SUPPORTED

   /* TODO: Check if the defines below really needed for the socket support */
#  if !(defined SOCKET)
     typedef int SOCKET;
#  endif

#  ifndef INVALID_SOCKET
#    define INVALID_SOCKET  ERROR
#  endif

#  ifndef IPPROTO_TCP
#    define IPPROTO_TCP PF_INET
#  endif

#  ifndef IPPROTO_UDP
#    define IPPROTO_UDP PF_INET
#  endif

#  ifndef SOCKET_ERROR
#    define SOCKET_ERROR ERROR
#  endif

#  ifndef TIMEVAL
#    define TIMEVAL timeval
#  endif

#  ifndef SHUT_RD
#    ifndef SD_RECEIVE
#      define SD_RECEIVE  ((EC_T_INT)0)
#    endif
#    define SHUTD_RD    ((EC_T_INT)0)
#  endif

#  ifndef SHUT_WR
#    ifndef SD_SEND
#      define SD_SEND     ((EC_T_INT)1)
#    endif
#    define SHUTD_WR    ((EC_T_INT)1)
#  endif

#  ifndef SHUT_RDWR
#    ifndef SD_BOTH
#      define SD_BOTH     ((EC_T_INT)2)
#    endif
#    define SHUTD_RDWR  ((EC_T_INT)2)
#  endif

#  ifndef SD_RECEIVE
#    define SD_RECEIVE  SHUT_RD
#  endif

#  ifndef SD_SEND
#    define SD_SEND     SHUT_WR
#  endif

#  ifndef SD_BOTH
#    define SD_BOTH     SHUT_RDWR
#  endif

#  ifndef OsSocketBind
     /*#error TODO: Implementaion of OsSocketBind in EcOs.cpp with -> m_oService.sin_len = sizeof(struct sockaddr_in);*/
     static EC_INLINE(EC_T_DWORD OsSocketBindVx(int s, struct sockaddr *name, int namelen ))
     {
        struct sockaddr_in* pSockA = (struct sockaddr_in*)name;
        namelen = (int)sizeof(struct sockaddr_in);
        pSockA->sin_len = (u_char)namelen;
        return bind(s, name, namelen);
     }
#    define OsSocketBind(hSockHandle, oSockAddr, nSockAddrLen) OsSocketBindVx(hSockHandle, oSockAddr, nSockAddrLen)
#  endif /* #ifndef OsSocketBind */

#  define OsSocketGetLastError()  0

#endif /* #if (defined EC_SOCKET_IP_SUPPORTED) */

     
/* optional: redirect trace messages into OS specific function, e.g. to store trace log into file 
 * default: print trace log as debug message
 */
#ifdef DEBUGTRACE
#  define OsTrcMsg OsTrcMsg
   ATECAT_API  EC_T_VOID   OsTrcMsg(const EC_T_CHAR* szFormat, ...);
#endif

#if (defined _WRS_VX_SMP) && (defined __cplusplus)
#  define EC_NEW(x) new (nothrow) x
#else
#  define EC_NEW(x) new x
#endif

#define EC_VASTART              va_start
#define EC_VAEND                va_end
#define EC_VAARG                va_arg

#ifdef USE_SPINLOCKS
  extern BOOL spinLockIsrHeld(spinlockIsr_t *pLock);
#endif

/*-TYPEDEFS------------------------------------------------------------------*/
typedef struct _OS_LOCK_DESC
{
    EC_T_OS_LOCK_TYPE   eLockType;            /* type of lock e. g. Default, SpinLock, Semaphore */
#ifdef USE_SPINLOCKS
    spinlockIsr_t SpinLock;
#endif
    SEM_ID    semMId;
    int       nLockCnt;
#ifdef DEBUG
    int       nTaskId;
#endif
} OS_LOCK_DESC;

typedef va_list             EC_T_VALIST;
typedef unsigned long long  EC_T_UINT64;
typedef signed long long    EC_T_INT64;

#ifdef __cplusplus
} /* extern "C"*/
#endif


#endif /* INC_ECOSPLATFORM */


/*-END OF SOURCE FILE--------------------------------------------------------*/

