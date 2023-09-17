/*-----------------------------------------------------------------------------
 * LinkOsPlatform.h         header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Andreas Willig
 * Description              os platform layer for EtherCAT link layer
 *---------------------------------------------------------------------------*/

#ifndef INC_LINK_OS_PLATFORM
#define INC_LINK_OS_PLATFORM

/*-INCLUDE-------------------------------------------------------------------*/
#include <vxWorks.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysLib.h>
#include <taskLib.h>
#include <EcType.h>
#include <intLib.h>
#include <cacheLib.h>

/*-DEFINES-------------------------------------------------------------------*/

#if (defined _VX_CPU_FAMILY) && (_VX_CPU_FAMILY==_VX_PPC)
#define EC_BIG_ENDIAN 1
#endif

#define ECLINKOS_INLINEATTRIBUTE

/* Use OS provided PCI search function */
#define ECLINKOS_USE_OS_PCISCANFUNC

#if (defined TOOLN) && (TOOLN==1) && ((defined _WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6))

/* ## needs a param before issuing string so "generate" one */
#define ECLINKOS_INLINEINT(dummy, functiondecl)     \
    __inline dummy ##functiondecl __attribute__((always_inline)); \
    __inline dummy ##functiondecl 

#else

/* ## needs a param before issuing string so "generate" one */
#define ECLINKOS_INLINEINT(dummy, functiondecl)     \
    __inline dummy ##functiondecl 
#endif

#define ECLINKOS_INLINE(funcdecl) ECLINKOS_INLINEINT(,funcdecl)

#define	MAX_DEVICE_WINDOWS				6

/* virtual/physical access to DMA area */
#define VIRT_TO_PHYS(pAdapter,dwAddr) (dwAddr)
#define PHYS_TO_VIRT(pAdapter,dwAddr) (dwAddr)

#define LinkOsPlatformInit() 				/* NOOP */
#define LinkOsClose()
#define LinkOsMalloc(nSize)                 malloc((size_t)(nSize))
#define LinkOsRealloc(pMem,nSize)           realloc((void*)(pMem),(size_t)(nSize))
#define LinkOsFree(pMem)                    free((void*)(pMem))
#define LinkOsMemset(pDest,nVal,nSize)      memset((void*)pDest,(int)nVal,(size_t)(nSize))
#define LinkOsMemcpy(pDest,pSrc,nSize)      memcpy((void*)(pDest),(const void*)(pSrc),(size_t)(nSize))
#define LinkOsMemcmp(pBuf1,pBuf2,nSize)     memcmp((void*)(pBuf1),(const void*)(pBuf2),(size_t)(nSize))
#define LinkOsMemmove(pDest,pSrc,nSize)     memmove((void*)(pDest),(const void*)(pSrc),(size_t)(nSize))
#define LinkOsStrlen(szString)              strlen((const char*)(szString))
#define LinkOsStrcmp(szStr1,szStr2)         strcmp((const char*)szStr1,(const char*)szStr2)

#define LinkOsSetEvent(semBEvent)           semGive((SEM_ID)(semBEvent))
#define LinkOsResetEvent(semBEvent)         semTake((SEM_ID)(semBEvent),NO_WAIT)
#define LinkOsSleep(dwMsec)                 taskDelay(  EC_MAX( ((sysClkRateGet() * (UINT32)(dwMsec) + 500)/1000) , 1 )  )
#ifdef DEBUG
#ifdef ASSERT_SUSPEND
#define LinkOsDbgAssert(bCond)              LinkOsDbgAssertFunc((bCond),__FILE__,__LINE__)
EC_T_VOID LinkOsDbgAssertFunc(EC_T_BOOL bAssertCondition, EC_T_CHAR* szFile, EC_T_DWORD dwLine);
#else
#define LinkOsDbgAssert                             assert
#endif
#endif
#ifndef LinkOsDbgAssert
#define LinkOsDbgAssert(x)
#endif

#if ((defined _WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6))
#define LinkOsSnprintf                              snprintf
#endif

#define LinkOsIntLock                       intLock
#define LinkOsIntUnlock                     intUnlock

#if ((defined _WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6))
#  define LinkOsMemoryBarrier VX_MEM_BARRIER_RW
#else
#  define LinkOsMemoryBarrier()
#endif /* ((defined _WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6)) */

#define LinkOsCacheFlushPipe()  CACHE_PIPE_FLUSH() /* From cacheLib */

/*-TYPEDEFS/ENUMS------------------------------------------------------------*/
typedef EC_T_DWORD PHYSICAL_ADDRESS;

/* this structure describes a base/length pair */
typedef struct _DEVICEWINDOW_tag 
{
	EC_T_DWORD dwBase;					/* window base address */
	EC_T_DWORD dwLen;					/* window size */
} DEVICEWINDOW, *PDEVICEWINDOW;

/* this structure contains address window information */
typedef struct _DDKWINDOWINFO_tag 
{
	EC_T_DWORD	    dwNumIoWindows;                 /* number of I/O windows */
	DEVICEWINDOW    ioWindows[MAX_DEVICE_WINDOWS];
	EC_T_DWORD	    dwNumMemWindows;                /* number of memory windows */
	DEVICEWINDOW    memWindows[MAX_DEVICE_WINDOWS];
} DDKWINDOWINFO, *PDDKWINDOWINFO;

/* this structure contains ISR information */
typedef struct _DDKISRINFO_tag 
{
	EC_T_DWORD	dwSysintr;      /* System interrupt ID */
} DDKISRINFO, *PDDKISRINFO;

#ifdef _WRS_VX_SMP
#define EC_LINKCPUSET_DEFINED
typedef cpuset_t    EC_T_LINKCPUSET;        /* CPU-set for SMP systems */
#define EC_LINKCPUSET_ZERO(CpuSet)          CPUSET_ZERO((CpuSet))
#define EC_LINKCPUSET_SET(CpuSet,nCpuIndex) CPUSET_SET((CpuSet),(nCpuIndex))
#define EC_LINKCPUSET_SETALL(CpuSet)        CPUSET_SETALL((CpuSet))       
#endif

/*ID word indices*/
#define	PCIID_CLASS							0
#define	PCIID_SUBCLASS						1
#define	PCIID_PROGIF						2
#define	PCIID_VENDORID						3
#define	PCIID_DEVICEID						4
#define	PCIID_REVISIONID					5
#define	PCIID_SUBSYSTEMVENDORID				6
#define	PCIID_SUBSYSTEMID					7
#define	PCIID_MAXNUMIDS						(PCIID_SUBSYSTEMID + 1)

/*ID word masks*/
#define	PCIIDM_CLASS						(1 << PCIID_CLASS)
#define	PCIIDM_SUBCLASS						(1 << PCIID_SUBCLASS)
#define	PCIIDM_PROGIF						(1 << PCIID_PROGIF)
#define	PCIIDM_VENDORID						(1 << PCIID_VENDORID)
#define	PCIIDM_DEVICEID						(1 << PCIID_DEVICEID)
#define	PCIIDM_REVISIONID					(1 << PCIID_REVISIONID)
#define	PCIIDM_SUBSYSTEMVENDORID			(1 << PCIID_SUBSYSTEMVENDORID)
#define	PCIIDM_SUBSYSTEMID					(1 << PCIID_SUBSYSTEMID)

/*this structure keeps track of standard PCI device instance information*/
typedef struct _DDKPCIINFO_tag 
{
    EC_T_DWORD  cbSize;					    /*size of this structure*/

	/* instancing information */
	EC_T_DWORD  dwBusNumber;
	EC_T_DWORD  dwDeviceNumber;
	EC_T_DWORD  dwFunctionNumber;
	EC_T_DWORD  dwInstanceIndex;

	/* device ID information */
	EC_T_DWORD  dwWhichIds;				    /*idVals[PCIID_XXX] present if (dwWhichIds & (1 << PCIID_XXX)) != 0*/
    EC_T_DWORD  idVals[PCIID_MAXNUMIDS];    /*class, subclass, etc, populated as described in dwWhichIds*/
} DDKPCIINFO, *PDDKPCIINFO;

#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) ) || (_WRS_VXWORKS_MAJOR > 6)
struct _EC_T_LINKOS_PCIDEV_DESC;
#endif
typedef struct _T_PCIINFO
{
    DDKPCIINFO      Pci;
    DDKWINDOWINFO   Window;
    DDKISRINFO      Isr;
    EC_T_INT        nCardHWId;
#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) ) || (_WRS_VXWORKS_MAJOR > 6)
    struct _EC_T_LINKOS_PCIDEV_DESC* pDev;
#endif
} T_PCIINFO;

typedef va_list             EC_T_LINKVALIST;

/*-MACROS/INLINES------------------------------------------------------------*/

extern EC_T_DWORD G_dwLinkOsLockCounter;
extern EC_T_DWORD G_dwLinkOsUnLockCounter;

#endif /* INC_LINK_OS_PLATFORM */


/*-END OF SOURCE FILE--------------------------------------------------------*/
