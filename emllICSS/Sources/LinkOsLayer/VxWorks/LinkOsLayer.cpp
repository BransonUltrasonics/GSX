/*-----------------------------------------------------------------------------
 * LinkOsLayer.cpp          cpp file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "LinkOsLayer.h"
#include "EcLink.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vxWorks.h>
#include <version.h>
#include <cacheLib.h>
#include <errnoLib.h>
#include <sysLib.h>
#include <tickLib.h>
#include <semLib.h>
#include <logLib.h>
#include <symLib.h>
#include <sysSymTbl.h>
#include <intLib.h>
#include <iv.h>

#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) && (_WRS_CONFIG_LP64) ) || (_WRS_VXWORKS_MAJOR > 6)
extern "C"
{
#include <vmLib.h>
#include <vxBus.h>
#include <vxbPciLib.h>
}
#define EXCLUDE_SYS_LOSAL
#ifndef VXB_RES_MEMORY /* TODO: better detection */
#define SUPPORT_LEGACY_VXBUS
#endif
#else
#ifndef NO_PCI_SUPPORT
# include <drv/pci/pciConfigLib.h>
# include <drv/pci/pciIntLib.h>
#endif /* ifndef NO_PCI_SUPPORT */
#endif

#if defined(_VX_CPU_FAMILY) && (_VX_CPU_FAMILY==_VX_PPC || _VX_CPU_FAMILY==_VX_ARM)
#  define EXCLUDE_SYS_LOSAL
#endif /* if defined(_VX_CPU_FAMILY) && (_VX_CPU_FAMILY==_VX_PPC || _VX_CPU_FAMILY==_VX_ARM) */

#ifndef EXCLUDE_SYS_LOSAL
#  define SLOSAL_NOT_INBSP 1
//#  include "sysLoSalAdd.c"
#endif /* ifndef EXCLUDE_SYS_LOSAL */


#ifdef LINKLAYER_INCLUDE_OEM
#  include LINKLAYER_INCLUDE_OEM
#endif /* ifdef LINKLAYER_INCLUDE_OEM */

#if ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 8)) || (_WRS_VXWORKS_MAJOR > 6)
#  define WRHV_SUPPORTED /* WindRiver Hypervisor */
#endif /* if ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 8)) || (_WRS_VXWORKS_MAJOR > 6) */

#if defined(_WRS_VX_SMP) && defined(CPU_FAMILY) && (CPU_FAMILY==I80X86)
#  define USE_SPINLOCKS /* Spinlock implementation is X86 specific */
#endif /* if defined(_WRS_VX_SMP) && defined(CPU_FAMILY) && (CPU_FAMILY==I80X86) */

#if (defined __cplusplus)
extern "C" {
#endif

#ifdef WRHV_SUPPORTED 

typedef uint64_t        vbiPhysAddr_t;
typedef void *          vbiGuestPhysAddr_t;
typedef int32_t			vbiStatus_t;

/* 
 * from vbi.h
 * extern vbiStatus_t vbiGuestDmaAddrGet 
 *   (
 *   vbiGuestPhysAddr_t gaddr,	// guest physical address to translate
 *   vbiPhysAddr_t     *paddr	   // translated physical address
 *   );
 */
#define VBI_DMA_ADDR_GET_NAME "vbiGuestDmaAddrGet"
typedef vbiStatus_t (*FP_VBI_DMA_ADDR_GET)(vbiGuestPhysAddr_t gaddr, vbiPhysAddr_t *paddr);

#endif /* ifdef WRHV_SUPPORTED */


#if defined(_VX_CPU_FAMILY) && (_VX_CPU_FAMILY==_VX_PPC || _VX_CPU_FAMILY==_VX_ARM)

void sysUsDelay(int delay); /* may be defined in BSP's sysLib.c */

/* This function is not provided by PPC/ARM BSP's */
STATUS	sysIntDisablePIC(int intLevel)
{
	return (STATUS) intDisable(intLevel);
}

/* This function is not provided by PPC/ARM BSP's */
STATUS	sysIntEnablePIC(int intLevel)
{
	return (STATUS) intEnable(intLevel); 
}

#endif /* if defined(_VX_CPU_FAMILY) && (_VX_CPU_FAMILY==_VX_PPC || _VX_CPU_FAMILY==_VX_ARMARCH7) */

#if (defined __cplusplus)
};
#endif

#ifndef EXCLUDE_SYS_LOSAL
/* SysLoSal connection */
typedef STATUS (*EC_PF_SYSLOSALINTCONNECT)(int iLevel, VOIDFUNCPTR pfIsr, int parameter);
typedef STATUS (*EC_PF_SYSLOSALINTDISCONNECT)(int iLevel, VOIDFUNCPTR pfIsr, int parameter);
typedef BOOL   (*EC_PF_SYSLOSALPCIFINDDEV)(UINT32 dwVendorId, UINT32 dwDeviceId, UINT32 dwIndex, UINT32* pdwBus, UINT32* pdwDevice, UINT32* pdwFunction);
static EC_PF_SYSLOSALINTCONNECT    S_pfnSysLoSalIntConnect    = EC_NULL;
static EC_PF_SYSLOSALINTDISCONNECT S_pfnSysLoSalIntDisconnect = EC_NULL;
static EC_PF_SYSLOSALPCIFINDDEV    S_pfnSysLoSalPCIFindDev    = EC_NULL;
static UINT32* S_pdwSysLoSalMapCnt = EC_NULL;
#endif /* ifndef EXCLUDE_SYS_LOSAL */

static EC_PF_LINKOSDBGMSGHK S_pfLinkOsDbgMsgHook    = EC_NULL;

#ifdef DEBUG
static EC_T_BOOL            S_bSpinLockActive       = EC_FALSE; /* flag: if set, no OS calls are allowed */
static int                  S_nSpinLockTaskId       = 0;        /* thread which owns the spin lock */
#endif /* ifdef DEBUG */

static EC_T_VOID* FindSym(const EC_T_CHAR* szFuncName)
{
EC_T_VOID* pvRetVal = EC_NULL;

#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9)) || (_WRS_VXWORKS_MAJOR > 6)
SYMBOL_DESC SymDesc;

        memset(&SymDesc, 0, sizeof(SYMBOL_DESC));
        SymDesc.mask = SYM_FIND_BY_NAME;
        SymDesc.name = (char*)szFuncName;
        SymDesc.nameLen = strlen(SymDesc.name);
        symFind(sysSymTbl, &SymDesc);
        if (NULL != SymDesc.value)
        {
            pvRetVal = SymDesc.value;
        }
#else
SYM_TYPE SymType;
    symFindByName(sysSymTbl, (char*)szFuncName, (char**)&pvRetVal, &SymType);
#endif
    return pvRetVal;
}

/*******************************************************************************
*
* sysDelay - a small delay
*
*/

EC_T_VOID LinkOsSysDelay(EC_T_DWORD dwTimeUsec)
{
#if defined(_VX_CPU_FAMILY) && (_VX_CPU_FAMILY==_VX_PPC)
   sysUsDelay(dwTimeUsec);
#else
   EC_T_DWORD dwCnt;
   EC_T_DWORD dwDelay = 0;

   dwDelay = (dwTimeUsec * sysClkRateGet())/1000000;
   if( dwDelay > 0 )
   {
      taskDelay(dwDelay);
   }
   else
   {
      for( dwCnt = 0; dwCnt<dwTimeUsec; dwCnt++ )
      {
         sysDelay();
      }
   }
#endif /* defined(_VX_CPU_FAMILY) && (_VX_CPU_FAMILY==_VX_PPC) */
   return;
}


/*******************************************************************************
*
* SysAllocDmaBuffer - allocate DMA buffer
*
*/
EC_T_VOID LinkOsAllocDmaBuffer(
    EC_T_VOID*  pvDmaObject
   ,EC_T_BYTE** ppbyVirtAddrCached
   ,EC_T_BYTE** ppbyVirtAddrUncached
   ,EC_T_BYTE** ppbyPhysAddr
   ,EC_T_DWORD  dwSize
   )
{
ECLINKOS_UNREFPARM(pvDmaObject);

#ifdef WRHV_SUPPORTED
    FP_VBI_DMA_ADDR_GET   fpDmaAddrGet;    
    vbiPhysAddr_t         physAddr;    
#endif
    
#if defined(LL_PLATFORM_ALLOCDMABUFFER)
    LL_PLATFORM_ALLOCDMABUFFER(pvDmaObject, ppbyVirtAddrCached, ppbyVirtAddrUncached,
                               ppbyPhysAddr, dwSize );
#else
    
    *ppbyVirtAddrCached = (EC_T_BYTE*)cacheDmaMalloc(dwSize);
    *ppbyPhysAddr = (EC_T_BYTE*)CACHE_DMA_VIRT_TO_PHYS(*ppbyVirtAddrCached);

#ifdef WRHV_SUPPORTED
    fpDmaAddrGet = (FP_VBI_DMA_ADDR_GET)FindSym(VBI_DMA_ADDR_GET_NAME);
    if (NULL != fpDmaAddrGet)
    {
      LinkOsDbgMsg("WindRiver HyperVisor mode enabled\n");

      /* Translate Guest-PhysicalAddress to Hardware-PhysicalAddress for DMA transfer */
      if ((*fpDmaAddrGet)((void *)*ppbyVirtAddrCached, &physAddr) != OK)
      {
         LinkOsDbgMsg("LinkOsAllocDmaBuffer: FAILED\n");
      }
      if ((physAddr >> 32) != 0)
      {
         LinkOsDbgMsg("LinkOsAllocDmaBuffer: LinkOsAllocDmaBuffer phys addr is gt 32bit\n");
      }
      *ppbyPhysAddr = (EC_T_BYTE*)0 + physAddr;
    }
#endif /* ifdef WRHV_SUPPORTED */

    *ppbyVirtAddrUncached  = *ppbyVirtAddrCached;
    
/*  printf("@@@ DMA alloc phys 0x%08x, virt 0x%08x\n", *ppbyPhysAddr, *ppbyVirtAddrCached); */
    
#endif /* if defined(LL_PLATFORM_ALLOCDMABUFFER) */
    
   return;
}


/*******************************************************************************
*
* SysFreeDmaBuffer - free DMA buffer
*
*/
EC_T_VOID LinkOsFreeDmaBuffer(
     EC_T_VOID*  pvDmaObject
    ,EC_T_BYTE*  pbyPhysAddr
	  ,EC_T_BYTE*  pbyVirtAddrCached
    ,EC_T_BYTE*  pbyVirtAddrUncached
    ,EC_T_DWORD  dwSize         
                                )
{
    ECLINKOS_UNREFPARM(pvDmaObject);
    ECLINKOS_UNREFPARM(pbyPhysAddr);
    ECLINKOS_UNREFPARM(dwSize);
	ECLINKOS_UNREFPARM(pbyVirtAddrUncached);
    
#if defined(LL_PLATFORM_FREEDMABUFFER)
    LL_PLATFORM_FREEDMABUFFER(pvDmaObject, pbyPhysAddr, pbyVirtAddrCached,
                              pbyVirtAddrUncached, dwSize );
#else
    cacheDmaFree(((EC_T_VOID*)pbyVirtAddrCached));
#endif /* if defined(LL_PLATFORM_FREEDMABUFFER) */
    return;
}


#ifndef EXCLUDE_SYS_LOSAL
/***************************************************************************************************/
/**
\brief  Iterator function to attach to the SysLoSal symbols

\return TRUE to continue, FALSE to stop iteration
*/
static BOOL AttachToSysLOSAL(
	char*      strCurrSymbolName,  /* symbol/entry name           */
	int        nVal,    		   /* symbol/entry value          */
	SYM_TYPE   oSymType,           /* symbol/entry type           */
	VOID* 	   poParams,    	   /* arbitrary user-supplied arg */
	UINT16     wGroup              /* symbol/entry group number   */
)
{
BOOL       bRetVal = EC_TRUE;
EC_T_VOID* pvSym   = EC_NULL;

    /* get symbol address */
    pvSym = FindSym(strCurrSymbolName);

    /* look for SysLoSal symbols */
    if      (NULL != strstr(strCurrSymbolName, "SysLoSalIntConnect"))
    {
        S_pfnSysLoSalIntConnect = (EC_PF_SYSLOSALINTCONNECT)pvSym;
    }
    else if (NULL != strstr(strCurrSymbolName, "SysLoSalIntDisconnect"))
    {
        S_pfnSysLoSalIntDisconnect = (EC_PF_SYSLOSALINTDISCONNECT)pvSym;    
    }
    else if (NULL != strstr(strCurrSymbolName, "SysLoSalPCIFindDev"))
    {
        S_pfnSysLoSalPCIFindDev = (EC_PF_SYSLOSALPCIFINDDEV)pvSym;
    }
    else if (NULL != strstr(strCurrSymbolName, "G_dwPciBusMapCnt"))
    {
        S_pdwSysLoSalMapCnt = (EC_T_DWORD*)pvSym;
    }
    else
    {
    	goto Exit;
    }

 Exit:
    /* evaluate all the symbols until all SysLoSal symbols found */
    if ((EC_NULL != S_pfnSysLoSalIntConnect) && (EC_NULL != S_pfnSysLoSalIntDisconnect) && (EC_NULL != S_pfnSysLoSalPCIFindDev) && (EC_NULL == S_pdwSysLoSalMapCnt))
    {
        bRetVal = FALSE;
    }
    return bRetVal;
}
#endif /* ifndef EXCLUDE_SYS_LOSAL */

#if ( (_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) && (_WRS_CONFIG_LP64)) || (_WRS_VXWORKS_MAJOR > 6)
#ifndef NO_PCI_SUPPORT
typedef struct _T_KNOWNCARD
{
    const EC_T_CHAR*       sIdentStr;
    LOSAL_T_CARD_IDENTIFY* aIdentify;
} T_KNOWNCARD;
       LOSAL_T_CARD_IDENTIFY G_oKnownGeiCards[]     = LOSAL_KNOWN_GEI_CARDS; /* This symbol is global because of emllInstallDeviceI8254x()! */
static LOSAL_T_CARD_IDENTIFY S_oKnownFeiCards[]     = LOSAL_KNOWN_FEI_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oKnownRTL8139Cards[] = LOSAL_KNOWN_RTL_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oKnownRTL8169Cards[] = LOSAL_KNOWN_RTL8169_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oKnownR6040Cards[]   = LOSAL_KNOWN_R6040_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oKnownCCATCards[]    = LOSAL_KNOWN_CCAT_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oKnownEG20TCards[]   = LOSAL_KNOWN_EG20T_CARDS;
static T_KNOWNCARD S_aKnownCards[] =
{
    { EC_LINK_PARMS_IDENT_I8254X,  G_oKnownGeiCards },
    { EC_LINK_PARMS_IDENT_I8255X,  S_oKnownFeiCards },
    { EC_LINK_PARMS_IDENT_RTL8139, S_oKnownRTL8139Cards },
    { EC_LINK_PARMS_IDENT_RTL8169, S_oKnownRTL8169Cards },
    { EC_LINK_PARMS_IDENT_R6040,   S_oKnownR6040Cards },
    { EC_LINK_PARMS_IDENT_CCAT,    S_oKnownCCATCards },
    { EC_LINK_PARMS_IDENT_EG20T,   S_oKnownEG20TCards },
};
#define KNOWNCARD_CNT (sizeof(S_aKnownCards) / sizeof(S_aKnownCards[0]))

#define MAX_LINKOS_PCIDEV_DESC 64
static EC_T_INT S_nLinkOsPciDevCnt = 0;
typedef struct _EC_T_LINKOS_PCIDEV_DESC
{
    struct vxbDev*       pVxbDev; /* direct access only allowed by legacy VxBus! */
    struct vxbPciDevice* pVxbPciDev;
} EC_T_LINKOS_PCIDEV_DESC;
static EC_T_LINKOS_PCIDEV_DESC S_aLinkOsPciDevDesc[MAX_LINKOS_PCIDEV_DESC];

#ifndef VXB_RES_MEMORY
/* defined in vxbResourceLib.h */
#define VXB_RES_MEMORY 1
typedef UINT64 VXB_KEY;
typedef struct vxbDev* VXB_DEV_ID;
typedef struct vxbResource
{
    SL_NODE node;     /* to make a resource list */
    UINT32 id;        /* IDX and type */
    void * pRes;      /* resource description */
    VXB_DEV_ID owner; /* device using this resource */
    VXB_KEY vxbSerial;/* Serial number */
} VXB_RESOURCE;
/* defined in vxbAccess.h */
typedef struct vxbResourceAdr
{
	UINT32 start;    /* resource range start */
    VIRT_ADDR virtAddr; /* virtual address, for memory mapped resources */
    size_t    size;     /* resource range size */
    void * pHandle;  /* phandle to access the device */
} VXB_RESOURCE_ADR;
#endif

typedef struct vxbResourceAdr64
{
    UINT64 start;    /* resource range start */
    VIRT_ADDR virtAddr; /* virtual address, for memory mapped resources */
    size_t    size;     /* resource range size */
    void * pHandle;  /* phandle to access the device */
} VXB_RESOURCE_ADR_64;

typedef void* (*FP_VXB_DEV_GET)(struct vxbDev*);
typedef struct vxbDev* (*FP_VXB_DEV_CHILD_GET)(struct vxbDev*);
typedef STATUS (*FP_VXB_DEV_ITERATE)(FUNCPTR func, void* pArg, UINT32 flags);
typedef struct vxbDev* (*FP_VXB_DEV_CHILD_GET)(struct vxbDev*);
typedef VXB_RESOURCE* (*FP_VXB_RESOURCE_ALLOC)(struct vxbDev*, UINT16 type, UINT16 idx);
struct vxbDevList;
typedef struct vxbDevList* (*FP_VXB_RESOURCE_LIST_GET)(struct vxbDev*);
typedef void vxbResourceTravelFunc (void * pArg, VXB_RESOURCE * pRes);
typedef STATUS (*FP_VXB_RESOURCE_ITERATE)(struct vxbDevList*, vxbResourceTravelFunc ,void *);
typedef int (*FP_VM_MAX_PHYS_BITS_GET)(void); 

#ifdef SUPPORT_LEGACY_VXBUS
static EC_T_BOOL                S_bVxbLegacy             = EC_TRUE;
#endif

static EC_T_BOOL                S_bPhysAddr64Bit         = EC_FALSE;
static struct vxbDev*           S_pVxbRoot               = EC_NULL;
static FP_VXB_DEV_GET           S_pfnVxbDevClassGet      = EC_NULL;
static FP_VXB_DEV_GET           S_pfnVxbDevIvarsGet      = EC_NULL;
static FP_VXB_DEV_ITERATE       S_pfnVxbDevIterate       = EC_NULL;
static FP_VXB_DEV_CHILD_GET     S_pfnVxbDevFirstChildGet = EC_NULL;
static FP_VXB_DEV_CHILD_GET     S_pfnVxbDevNextChildGet  = EC_NULL;
static FP_VXB_RESOURCE_ALLOC    S_pfnVxbResourceAlloc    = EC_NULL;
static FP_VXB_RESOURCE_LIST_GET S_pfnVxbResourceListGet  = EC_NULL;
static FP_VXB_RESOURCE_ITERATE  S_pfnVxbResourceIterate  = EC_NULL;

static void LinkOsGetPciInfoAddDev(struct vxbDev* pVxbDev, struct vxbPciDevice* pVxbPciDev)
{
    if (S_nLinkOsPciDevCnt < MAX_LINKOS_PCIDEV_DESC)
    {
        S_aLinkOsPciDevDesc[S_nLinkOsPciDevCnt].pVxbDev    = pVxbDev;
        S_aLinkOsPciDevDesc[S_nLinkOsPciDevCnt].pVxbPciDev = pVxbPciDev;
        S_nLinkOsPciDevCnt++;
/*      printf("@@@ Add 0x%04X:0x%04X\n", pVxbPciDev->pciVendId, pVxbPciDev->pciDevId); */
    }
}
static void LinkOsGetPciInfoIterate(struct vxbDev* pVxbDev, void* pArgs)
{
#ifdef SUPPORT_LEGACY_VXBUS
    if (S_bVxbLegacy)
    {
        switch (pVxbDev->busID)
        {
        case VXB_BUSID_PCI:
        case VXB_BUSID_PCIX:
        case VXB_BUSID_PCIEXPRESS:
            LinkOsGetPciInfoAddDev(pVxbDev, (struct vxbPciDevice*)pVxbDev->pBusSpecificDevInfo);
            break;
        default:
            break;
        }
    }
    else
#endif /* SUPPORT_LEGACY_VXBUS */
    {
        /* vxbDevClassGet returns normally VXB_BUSTYPE_ID */
        if (0 == strcmp ("vxbPciBus", (const char *)S_pfnVxbDevClassGet(pVxbDev)))
        {
            LinkOsGetPciInfoAddDev(pVxbDev, (struct vxbPciDevice*)S_pfnVxbDevIvarsGet(pVxbDev));
        }
    }
    return;
}
static EC_T_VOID LinkOsRecordPciDevs(EC_T_VOID)
{
#ifdef SUPPORT_LEGACY_VXBUS
    if (S_bVxbLegacy)
    {
        S_pfnVxbDevIterate((FUNCPTR)LinkOsGetPciInfoIterate, NULL, VXB_ITERATE_INSTANCES | VXB_ITERATE_ORPHANS);
    }
    else
#endif /* SUPPORT_LEGACY_VXBUS */
    {
        /* TODO: better looping */
        for (struct vxbDev* pChildDev = S_pfnVxbDevFirstChildGet(S_pVxbRoot); pChildDev != NULL; pChildDev = S_pfnVxbDevNextChildGet(pChildDev))
        {
            LinkOsGetPciInfoIterate(pChildDev, NULL);
            for (struct vxbDev* pChild2Dev = S_pfnVxbDevFirstChildGet(pChildDev); pChild2Dev != NULL; pChild2Dev = S_pfnVxbDevNextChildGet(pChild2Dev))
            {
                LinkOsGetPciInfoIterate(pChild2Dev, NULL);
                for (struct vxbDev* pChild3Dev = S_pfnVxbDevFirstChildGet(pChild2Dev); pChild3Dev != NULL; pChild3Dev = S_pfnVxbDevNextChildGet(pChild3Dev))
                {
                    LinkOsGetPciInfoIterate(pChild3Dev, NULL);
                    for (struct vxbDev* pChild4Dev = S_pfnVxbDevFirstChildGet(pChild3Dev); pChild4Dev != NULL; pChild4Dev = S_pfnVxbDevNextChildGet(pChild4Dev))
                    {
                        LinkOsGetPciInfoIterate(pChild4Dev, NULL);
                    }
                }
            }
        }
    }
    return;
}
static EC_T_BOOL LinkOsGetPciDevInstance(LOSAL_T_CARD_IDENTIFY* aKnownCardsIdentify, EC_T_DWORD dwUnit, EC_T_LINKOS_PCIDEV_DESC** ppLinkOsPciDev, LOSAL_T_CARD_IDENTIFY** ppCardIdentify)
{
EC_T_INT   nLinkOsPciDevIdx = 0;
EC_T_INT   nKnownCardIdx    = 0;
EC_T_DWORD dwUnitCur        = 0;

    for (nLinkOsPciDevIdx = 0; nLinkOsPciDevIdx < S_nLinkOsPciDevCnt; nLinkOsPciDevIdx++)
    {
    EC_T_LINKOS_PCIDEV_DESC* pLinkOsPciDev = &S_aLinkOsPciDevDesc[nLinkOsPciDevIdx];
    struct vxbPciDevice*     pVxbPciDev    = pLinkOsPciDev->pVxbPciDev;

        for (nKnownCardIdx = 0; 0 != aKnownCardsIdentify[nKnownCardIdx].wVendorId; nKnownCardIdx++)
        {
        LOSAL_T_CARD_IDENTIFY* pCardIdentify = &aKnownCardsIdentify[nKnownCardIdx];

/*          printf("@@@ Consider 0x%04X:0x%04X vs 0x%04X:0x%04X\n", pVxbPciDev->pciVendId, pVxbPciDev->pciDevId, pCardIdentify->wVendorId, pCardIdentify->wDeviceId); */

            if ((pVxbPciDev->pciVendId == pCardIdentify->wVendorId) && (pVxbPciDev->pciDevId == pCardIdentify->wDeviceId))
            {
                dwUnitCur++;
                if (dwUnitCur == dwUnit)
                {
                    *ppLinkOsPciDev = pLinkOsPciDev;
                    *ppCardIdentify = pCardIdentify;
/*                  vxbDevStructShow(pDev); */
                    return EC_TRUE;
                }
            }
        }
    }
    return EC_FALSE;
}
static EC_T_BOOL LinkOsGetPciInfoInternal(T_PCIINFO* pInfo, EC_T_DWORD dwUnit, LOSAL_T_CARD_IDENTIFY* aKnownCardsIdentify)
{
EC_T_BOOL bRetVal = EC_FALSE;
EC_T_LINKOS_PCIDEV_DESC* pLinkOsPciDev = EC_NULL;
LOSAL_T_CARD_IDENTIFY*   pCardIdentify = EC_NULL;

    /* record all PCI devices once */
    if (0 == S_nLinkOsPciDevCnt)
    {
        /* attach to non legacy VxBus */
        S_pVxbRoot               = (struct vxbDev*)FindSym("vxbRoot");
        S_pfnVxbDevClassGet      = (FP_VXB_DEV_GET)FindSym("vxbDevClassGet");
        S_pfnVxbDevIvarsGet      = (FP_VXB_DEV_GET)FindSym("vxbDevIvarsGet");
        S_pfnVxbDevIterate       = (FP_VXB_DEV_ITERATE)FindSym("vxbDevIterate");
        S_pfnVxbDevFirstChildGet = (FP_VXB_DEV_CHILD_GET)FindSym("vxbDevFirstChildGet");
        S_pfnVxbDevNextChildGet  = (FP_VXB_DEV_CHILD_GET)FindSym("vxbDevNextChildGet");
        S_pfnVxbResourceAlloc    = (FP_VXB_RESOURCE_ALLOC)FindSym("vxbResourceAlloc");
        S_pfnVxbResourceListGet  = (FP_VXB_RESOURCE_LIST_GET)FindSym("vxbResourceListGet");
        S_pfnVxbResourceIterate  = (FP_VXB_RESOURCE_ITERATE)FindSym("vxbResourceIterate");
#ifdef SUPPORT_LEGACY_VXBUS
        if (EC_NULL == S_pVxbRoot)
        {
/*          printf("@@@ assume vxbus_legacy-1.1.3.3\n"); */
            S_bVxbLegacy = EC_TRUE;        
        }
        else
        {
/*          printf("@@@ assume vxbus-1.0.3.2\n"); */
            S_bVxbLegacy = EC_FALSE;
        }
#endif /* SUPPORT_LEGACY_VXBUS */
        LinkOsRecordPciDevs();

        /* check if phys_addr is 64Bit */
        FP_VM_MAX_PHYS_BITS_GET S_pfnvmMaxPhysBitsGet = (FP_VM_MAX_PHYS_BITS_GET)FindSym("vmMaxPhysBitsGet");
        if (EC_NULL != S_pfnvmMaxPhysBitsGet)
        {
        	S_bPhysAddr64Bit = S_pfnvmMaxPhysBitsGet() > 32 ? 1 : 0 ;
        }
    }
    /* get required PCI device instance */
    if (!LinkOsGetPciDevInstance(aKnownCardsIdentify, dwUnit, &pLinkOsPciDev, &pCardIdentify))
    {
        goto Exit;
    }
    /* fill info structure */
    {
    struct vxbDev*        pVxbDev    = pLinkOsPciDev->pVxbDev;
    struct vxbPciDevice*  pVxbPciDev = pLinkOsPciDev->pVxbPciDev;
    EC_T_INT nBarIdx = 0;

/*      printf("@@@ Network adapter instance found at PCI %3d:%3d:%3d\n", pVxbPciDev->pciBus, pVxbPciDev->pciDev, pVxbPciDev->pciFunc); */

        LinkOsMemset((EC_T_VOID*)pInfo, 0, sizeof(T_PCIINFO));
        pInfo->Pci.cbSize           = (EC_T_DWORD)sizeof(DDKPCIINFO);   
        pInfo->Pci.dwInstanceIndex  = dwUnit;
        pInfo->nCardHWId            = pCardIdentify->wHWId;
        pInfo->Pci.dwWhichIds       = 0;
        pInfo->Pci.dwBusNumber      = pVxbPciDev->pciBus;
        pInfo->Pci.dwDeviceNumber   = pVxbPciDev->pciDev;
        pInfo->Pci.dwFunctionNumber = pVxbPciDev->pciFunc;
        pInfo->pDev = pLinkOsPciDev;
/*      pInfo->Isr.dwSysintr        = pVxbPciDev->pIntInfo-> */
        for (nBarIdx = 0; nBarIdx < VXB_MAXBARS; nBarIdx++)
        {
#ifdef SUPPORT_LEGACY_VXBUS
            if (S_bVxbLegacy)
            {
                switch (pVxbDev->regBaseFlags[nBarIdx])
                {
                case VXB_REG_IO:
                    pInfo->Window.ioWindows[pInfo->Window.dwNumIoWindows].dwLen  = (EC_T_DWORD)pVxbDev->regBaseSize[nBarIdx];
                    pInfo->Window.ioWindows[pInfo->Window.dwNumIoWindows].dwBase = (EC_T_DWORD)((EC_T_BYTE*)pVxbDev->pRegBasePhys[nBarIdx] - (EC_T_BYTE*)0);
                    pInfo->Window.dwNumIoWindows++;
                    break;
                case VXB_REG_MEM:
                    pInfo->Window.memWindows[pInfo->Window.dwNumMemWindows].dwLen  = (EC_T_DWORD)pVxbDev->regBaseSize[nBarIdx];
                    pInfo->Window.memWindows[pInfo->Window.dwNumMemWindows].dwBase = (EC_T_DWORD)((EC_T_BYTE*)pVxbDev->pRegBasePhys[nBarIdx] - (EC_T_BYTE*)0);
                    pInfo->Window.dwNumMemWindows++;
                    break;
                default:
                    continue;
                }
            }
            else
#endif /* SUPPORT_LEGACY_VXBUS */
            {
            VXB_RESOURCE* pVxbRsrc = S_pfnVxbResourceAlloc(pVxbDev, VXB_RES_MEMORY, (UINT16)nBarIdx);

                if (NULL != pVxbRsrc)
                {
                    if (NULL != pVxbRsrc->pRes)
                    {
/*
                        printf ("@@@ BAR[%d] %d bytes at 0x%04X%04X (0x%04X%04X)\n", nBarIdx, pVxbRsrcAdr->size,
                                    EC_HIDWORD(((EC_T_BYTE*)pVxbRsrcAdr->start    - (EC_T_BYTE*)0)),
                                    EC_LODWORD(((EC_T_BYTE*)pVxbRsrcAdr->start    - (EC_T_BYTE*)0)),
                                    EC_HIDWORD(((EC_T_BYTE*)pVxbRsrcAdr->virtAddr - (EC_T_BYTE*)0)),
                                    EC_LODWORD(((EC_T_BYTE*)pVxbRsrcAdr->virtAddr - (EC_T_BYTE*)0)));
*/                     
						/* return 32Bit address because of non 64Bit compatible LinkOsLayer, original 64Bit value will be extended back later in LinkOsMapMemory */
                    	if (S_bPhysAddr64Bit)
                    	{
                    	VXB_RESOURCE_ADR_64* pVxbRsrcAdr64 = (VXB_RESOURCE_ADR_64*)pVxbRsrc->pRes;

                    	    pInfo->Window.memWindows[pInfo->Window.dwNumMemWindows].dwBase = (EC_T_DWORD)((EC_T_BYTE*)pVxbRsrcAdr64->start - (EC_T_BYTE*)0);
                    	    pInfo->Window.memWindows[pInfo->Window.dwNumMemWindows].dwLen  = (EC_T_DWORD)pVxbRsrcAdr64->size;
                    	}
                    	else
                    	{
                    	VXB_RESOURCE_ADR* pVxbRsrcAdr = (VXB_RESOURCE_ADR*)pVxbRsrc->pRes;

							pInfo->Window.memWindows[pInfo->Window.dwNumMemWindows].dwBase = (EC_T_DWORD)((EC_T_BYTE*)pVxbRsrcAdr->start - (EC_T_BYTE*)0);
							pInfo->Window.memWindows[pInfo->Window.dwNumMemWindows].dwLen  = (EC_T_DWORD)pVxbRsrcAdr->size;		
                    	}
                    	pInfo->Window.dwNumMemWindows++;
                    }
                }
            }
        }
    }
    /* no errors */
    bRetVal = EC_TRUE;

Exit:
    return bRetVal;
}

EC_T_BOOL LinkOsGetPciInfo(T_PCIINFO* pInfo, const EC_T_CHAR*  szDeviceName, EC_T_DWORD dwUnit)
{
EC_T_UINT uKnownCardIdx;

    for (uKnownCardIdx = 0; (uKnownCardIdx < KNOWNCARD_CNT); uKnownCardIdx++)
    {
        if (0 == LinkOsStrcmp(S_aKnownCards[uKnownCardIdx].sIdentStr, szDeviceName))
        {
            return LinkOsGetPciInfoInternal(pInfo, dwUnit, S_aKnownCards[uKnownCardIdx].aIdentify);
        }
    }
    return EC_FALSE;
}

typedef struct _EC_T_MAP_MEMORY_ITERATE_CONTEXT
{
    EC_T_BYTE* pbyPhysical;
    EC_T_DWORD dwLen;
    EC_T_BYTE* pbyVirtual;
} EC_T_MAP_MEMORY_ITERATE_CONTEXT;
static void LinkOsVxbResourceTravelFunc(void* pArg, VXB_RESOURCE* pRes)
{
EC_T_MAP_MEMORY_ITERATE_CONTEXT* pMapMemoryIterateContext = (EC_T_MAP_MEMORY_ITERATE_CONTEXT*)pArg;

    if (EC_NULL != pMapMemoryIterateContext->pbyVirtual)
	{
			/* nothing to do */
			return;
	}
	/* only compare the lowest 32Bit address because of non 64Bit compatible LinkOsLayer */
	if (S_bPhysAddr64Bit)
	{
	VXB_RESOURCE_ADR_64* pVxbRsrcAdr64 = (VXB_RESOURCE_ADR_64*)pRes->pRes;

		if (EC_LODWORD(((EC_T_BYTE*)pMapMemoryIterateContext->pbyPhysical - (EC_T_BYTE*)0)) == EC_LODWORD(((EC_T_BYTE*)pVxbRsrcAdr64->start - (EC_T_BYTE*)0)))
		{
			pMapMemoryIterateContext->pbyVirtual = (EC_T_BYTE*)pVxbRsrcAdr64->virtAddr;
		}
	}
	else
	{
	VXB_RESOURCE_ADR* pVxbRsrcAdr = (VXB_RESOURCE_ADR*)pRes->pRes;		

		if (EC_LODWORD(((EC_T_BYTE*)pMapMemoryIterateContext->pbyPhysical - (EC_T_BYTE*)0)) == EC_LODWORD(((EC_T_BYTE*)pVxbRsrcAdr->start - (EC_T_BYTE*)0)))
		{
			pMapMemoryIterateContext->pbyVirtual = (EC_T_BYTE*)pVxbRsrcAdr->virtAddr;
		}
	}
}
EC_T_BYTE* LinkOsMapMemory(EC_T_BYTE* pbyPhysical, EC_T_DWORD dwLen)
{
EC_T_INT nLinkOsPciDevIdx;

    for (nLinkOsPciDevIdx = 0; nLinkOsPciDevIdx < S_nLinkOsPciDevCnt; nLinkOsPciDevIdx++)
    {
    struct vxbDev* pVxbDev = S_aLinkOsPciDevDesc[nLinkOsPciDevIdx].pVxbDev;

#ifdef SUPPORT_LEGACY_VXBUS
        if (S_bVxbLegacy)
        {
        EC_T_INT   nBarIdx;

            for (nBarIdx = 0; nBarIdx < VXB_MAXBARS; nBarIdx++)
            {
                if (pbyPhysical == pVxbDev->pRegBasePhys[nBarIdx])
                {
                    return (EC_T_BYTE*)pVxbDev->pRegBase[nBarIdx];
                }
            }
        }
        else
#endif /* SUPPORT_LEGACY_VXBUS */
        {
        EC_T_MAP_MEMORY_ITERATE_CONTEXT MapMemoryIterateContext;
        struct vxbDevList* pVxbRsrcList = S_pfnVxbResourceListGet(pVxbDev);
        
            MapMemoryIterateContext.pbyPhysical = pbyPhysical;
            MapMemoryIterateContext.pbyVirtual  = 0;
            S_pfnVxbResourceIterate(pVxbRsrcList, LinkOsVxbResourceTravelFunc, &MapMemoryIterateContext);
            if (0 != MapMemoryIterateContext.pbyVirtual)
            {
                return MapMemoryIterateContext.pbyVirtual;
            }
        }
    }
    return EC_NULL;
}
#else /* NO_PCI_SUPPORT */
typedef STATUS (*FP_VM_PAGE_MAP)(VM_CONTEXT_ID context, VIRT_ADDR virtualAddr, PHYS_ADDR physicalAddr, size_t len, UINT stateMask, UINT state);
typedef STATUS (*FP_VM_TRANSLATE)(VM_CONTEXT_ID context, VIRT_ADDR virtualAddr, PHYS_ADDR *physicalAddr);

EC_T_BYTE* LinkOsMapMemory(EC_T_BYTE* pbyPhysical, EC_T_DWORD dwLen)
{
EC_T_BYTE* pbyRetVal = EC_NULL;
FP_VM_TRANSLATE pfnVmTranslate = (FP_VM_TRANSLATE)FindSym("vmTranslate");
FP_VM_PAGE_MAP pfnVmPageMap = (FP_VM_PAGE_MAP)FindSym("vmPageMap");


    if (NULL != pfnVmTranslate)
    {
    EC_T_BYTE* pvVirtual = NULL;

        if (OK == pfnVmTranslate((VM_CONTEXT_ID)NULL, (VIRT_ADDR)pbyPhysical, (PHYS_ADDR*)&pvVirtual))
        {
            printf("@@@ *LinkOsMapMemory() Already mapped!\n");
            pbyRetVal = pvVirtual;
            /* already mapped */
            goto Exit;
        }
    }
    if (NULL != pfnVmPageMap)
    {
    STATUS oStatus = ERROR;

        oStatus = pfnVmPageMap((VM_CONTEXT_ID)NULL, (VIRT_ADDR)pbyPhysical, (PHYS_ADDR)pbyPhysical, dwLen,
                    VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
                    VM_STATE_WRITABLE | VM_STATE_CACHEABLE_NOT | VM_STATE_MEM_COHERENCY | VM_STATE_GUARDED);
        if (OK != oStatus)
        {
            printf("@@@ *LinkOsMapMemory() pfnVmPageMap() returns %d\n", oStatus);
            goto Exit;    	    
        }
        pbyRetVal = pbyPhysical;
    }
Exit:
    return pbyRetVal;
}
#endif /* NO_PCI_SUPPORT */
#else /* (_WRS_VXWORKS_MAJOR >= 7) */
#ifndef NO_PCI_SUPPORT
static EC_T_BOOL S_bLinkOsPciInitialized = EC_FALSE;
EC_T_VOID LinkOsPciInit(EC_T_VOID)
{
    if (!S_bLinkOsPciInitialized)
    {
#ifndef EXCLUDE_SYS_LOSAL
       symEach(sysSymTbl, (FUNCPTR)AttachToSysLOSAL, EC_NULL);
#endif /* ifndef EXCLUDE_SYS_LOSAL */
       
       S_bLinkOsPciInitialized = EC_TRUE;
    }
}
EC_T_VOID LinkOsPciCleanup(EC_T_VOID)
{
}
EC_T_BOOL LinkOsPciRead8(EC_T_DWORD dwBusNum, EC_T_DWORD dwDevNum, EC_T_DWORD dwFunNum, EC_T_DWORD dwRegOffs, EC_T_VOID* pvBuf)
{
   return (OK == pciConfigInByte(dwBusNum, dwDevNum, dwFunNum, dwRegOffs, (UINT8 *) pvBuf));
}
EC_T_BOOL LinkOsPciRead16(EC_T_DWORD dwBusNum, EC_T_DWORD dwDevNum, EC_T_DWORD dwFunNum, EC_T_DWORD dwRegOffs, EC_T_VOID* pvBuf)
{
   return (OK == pciConfigInWord(dwBusNum, dwDevNum, dwFunNum, dwRegOffs, (UINT16 *) pvBuf));
}
EC_T_BOOL LinkOsPciRead32(EC_T_DWORD dwBusNum, EC_T_DWORD dwDevNum, EC_T_DWORD dwFunNum, EC_T_DWORD dwRegOffs, EC_T_VOID* pvBuf)
{
   return (OK == pciConfigInLong(dwBusNum, dwDevNum, dwFunNum, dwRegOffs, (UINT32 *) pvBuf));
}
EC_T_BOOL LinkOsPciWrite8(EC_T_DWORD dwBusNum, EC_T_DWORD dwDevNum, EC_T_DWORD dwFunNum, EC_T_DWORD dwRegOffs, const EC_T_VOID* pvBuf)
{
   return (OK == pciConfigOutByte(dwBusNum, dwDevNum, dwFunNum, dwRegOffs, *(UINT8 *) pvBuf));
}
EC_T_BOOL LinkOsPciWrite16(EC_T_DWORD dwBusNum, EC_T_DWORD dwDevNum, EC_T_DWORD dwFunNum, EC_T_DWORD dwRegOffs, const EC_T_VOID* pvBuf)
{
   return (OK == pciConfigOutWord(dwBusNum, dwDevNum, dwFunNum, dwRegOffs, *(UINT16 *) pvBuf));
}
EC_T_BOOL LinkOsPciWrite32(EC_T_DWORD dwBusNum, EC_T_DWORD dwDevNum, EC_T_DWORD dwFunNum, EC_T_DWORD dwRegOffs, const EC_T_VOID* pvBuf)
{
   return (OK == pciConfigOutLong(dwBusNum, dwDevNum, dwFunNum, dwRegOffs, *(UINT32 *) pvBuf));
}
EC_T_BOOL LinkOsPciFindDev(
    EC_T_DWORD         dwVendorID,
    EC_T_DWORD         dwDeviceID,
    EC_T_DWORD         dwIdx, /* 1: first device, 2: second device, ...*/
    EC_T_DWORD*        pdwBusNum,
    EC_T_DWORD*        pdwDevNum,
    EC_T_DWORD*        pdwFunNum
                            )
{
   LinkOsDbgAssert(dwIdx > 0);
   
#ifndef EXCLUDE_SYS_LOSAL
   if ((EC_NULL == S_pdwSysLoSalMapCnt) || (0 == *S_pdwSysLoSalMapCnt))
#endif /* ifndef EXCLUDE_SYS_LOSAL */
   {
       /* PCI device in memory cache is not filled.
        * This means that the mapping function SLOSAL_sysInit*() from sysLoSalAdd.c has not been called.
        * -> Use native VxWorks pciFindDevice() function. Probably the user didn't call SLOSAL_sysInit*()
        *    because VxBus is enabled. So pciFindDevice() should be fast anyway.
        */
       if ( pciFindDevice((int)dwVendorID, (int)dwDeviceID, (int)(dwIdx - 1), (int*)pdwBusNum, (int*)pdwDevNum, (int*)pdwFunNum) == OK )
       {
           return EC_TRUE;
       }
       else
       {
           return EC_FALSE;
       }
   }
#ifndef EXCLUDE_SYS_LOSAL
   else
   {
       /* Call pci scan function from sysLoSalAdd.c. This function does the search on
        * an in memory structure, so this search is fast. Note that the VxWorks 5.x (not for VxWorks 6.x) 
        * pciFindDevice() function accesses the PCI bus each time, so the native pciFindDevice() call is slow.
        */
       if (EC_NULL != S_pfnSysLoSalPCIFindDev)
       {
           return S_pfnSysLoSalPCIFindDev((UINT32)dwVendorID, (UINT32)dwDeviceID, (UINT32)dwIdx, (UINT32*)pdwBusNum, (UINT32*)pdwDevNum, (UINT32*)pdwFunNum);
       }
       else
       {
           return FALSE;
       }
   }
#endif /* ifndef EXCLUDE_SYS_LOSAL */
}
#endif /* NO_PCI_SUPPORT */

EC_T_BYTE* LinkOsMapMemory(EC_T_BYTE* pbyPhysical, EC_T_DWORD dwLen)
{
    EC_UNREFPARM(dwLen);
    return pbyPhysical;
}
#endif /* (_WRS_VXWORKS_MAJOR >= 7) */

EC_T_VOID LinkOsUnmapMemory(EC_T_BYTE* pbyVirtual, EC_T_DWORD dwLen)
{
    ECLINKOS_UNREFPARM( dwLen );
    ECLINKOS_UNREFPARM( pbyVirtual );
}

/********************************************************************************/
/** \brief Create thread.
*
* \return thread object or EC_NULL in case of an error.
*/
EC_T_VOID*  LinkOsCreateThread(
    EC_T_CHAR*              szThreadName, 
    EC_PF_LINKTHREADENTRY   pfThreadEntry, 
    EC_T_DWORD              dwPrio, 
    EC_T_DWORD              dwStackSize, 
    EC_T_VOID*              pvParams 
                                          )
{
    EC_T_VOID* pvThreadObject;
#if ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9)) || (_WRS_VXWORKS_MAJOR > 6)
    pvThreadObject = (EC_T_VOID*)taskSpawn( szThreadName, (int)dwPrio, 0, (int)dwStackSize, (FUNCPTR)pfThreadEntry, (_Vx_usr_arg_t)pvParams, 2,3,4,5,6,7,8,9,10 );
#else
    pvThreadObject = (EC_T_VOID*)taskSpawn( szThreadName, (int)dwPrio, 0, (int)dwStackSize, (FUNCPTR)pfThreadEntry, (int)pvParams, 2,3,4,5,6,7,8,9,10 );    
#endif
    if (pvThreadObject == (EC_T_VOID*)ERROR)
    {
        pvThreadObject = EC_NULL;
        goto Exit;
    }
    
Exit:
    return pvThreadObject;
}

/***************************************************************************************************/
/**
\brief  Delete a thread Handle return by OsCreateThread.
*/
EC_T_VOID   LinkOsDeleteThreadHandle(
    EC_T_VOID* pvThreadObject		/**< [in]	Previously allocated Thread Handle */
                                                )
{
    ECLINKOS_UNREFPARM(pvThreadObject);
}

/********************************************************************************/
/** \brief Create a synchronization mutual exclusion object
*
* \return handle to the mutex object.
*/
EC_T_VOID* LinkOsCreateLock(EC_T_OS_LOCK_TYPE   eLockType)
{
OS_LOCK_DESC* pvLockDesc = (OS_LOCK_DESC*)OsMalloc(sizeof(OS_LOCK_DESC));

    /* check if spin lock is active */
    LinkOsDbgAssert( ! (S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())));

    if( pvLockDesc != EC_NULL )
    {
        pvLockDesc->nLockCnt = 0;
        pvLockDesc->eLockType = eLockType; 

        switch(pvLockDesc->eLockType)
        {
        default:
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
#endif /* ifdef USE_SPINLOCKS */
            break;
        }
    }

    LinkOsDbgAssert(EC_NULL != pvLockDesc);
    
    return pvLockDesc;
}


/********************************************************************************/
/** \brief Delete a mutex object
*
* \return N/A
*/
EC_T_VOID  LinkOsDeleteLock(EC_T_VOID* pvLockHandle)
{
OS_LOCK_DESC* pvLockDesc = (OS_LOCK_DESC*)pvLockHandle;

    /* check if spin lock is active */
    LinkOsDbgAssert( !(S_bSpinLockActive && (S_nSpinLockTaskId == taskIdSelf())) );

    if (pvLockDesc != EC_NULL)
    {
        LinkOsDbgAssert( pvLockDesc->nLockCnt == 0 );

        switch(pvLockDesc->eLockType)
        {
        
        default:
        case eLockType_DEFAULT:     /*< Default mutex           */
        case eLockType_INTERFACE:   /*< only interface */
            LinkOsDbgAssert( pvLockDesc->semMId != 0 );
            semDelete((SEM_ID)pvLockDesc->semMId);
            break;

        /*< only jobs --> spin lock */
        case eLockType_SPIN:
#ifdef USE_SPINLOCKS
            /* nothing to do */
#else
            LinkOsDbgAssert( pvLockDesc->semMId != 0 );
            semDelete((SEM_ID)pvLockDesc->semMId);
#endif /* ifdef USE_SPINLOCKS */
            break;
        }

        OsFree( pvLockDesc );
    }
}


/********************************************************************************/
EC_T_VOID  LinkOsLock(EC_T_VOID* pvLockHandle)
{
    OS_LOCK_DESC* pvLockDesc = (OS_LOCK_DESC*)pvLockHandle;
#ifdef DEBUG
    int nTid;
    nTid = taskIdSelf();
#endif /* ifdef DEBUG */
    
    LinkOsDbgAssert( pvLockDesc != EC_NULL );
    if (pvLockDesc != NULL)
    {
        switch(pvLockDesc->eLockType)
        {
            
        default:
        case eLockType_DEFAULT:     /*< Default mutex           */
        case eLockType_INTERFACE:   /*< only interface */
            LinkOsDbgAssert( pvLockDesc->semMId != 0 );
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
                LinkOsDbgAssert( pvLockDesc->nLockCnt == 0 );
            }
#else
            LinkOsDbgAssert( pvLockDesc->semMId != 0 );
            semTake((SEM_ID)pvLockDesc->semMId, WAIT_FOREVER);
#endif /* ifdef USE_SPINLOCKS */
#ifdef DEBUG
            S_bSpinLockActive = EC_TRUE;
            S_nSpinLockTaskId = nTid;
#endif /* ifdef DEBUG */
            break;
        }
        pvLockDesc->nLockCnt++;
        
#ifdef DEBUG
        pvLockDesc->nTaskId = nTid;
        if( pvLockDesc->nLockCnt >= 10 )
        {
            /* that much nesting levels? */
            LinkOsDbgMsg( "FATAL Error in LinkOsLock(): pvLockDesc = 0x%x, nLockCnt = %s, tid = 0x%x\n", pvLockDesc, pvLockDesc->nLockCnt, pvLockDesc->nTaskId );
            LinkOsDbgAssert( EC_FALSE );
        }
#endif /* ifdef DEBUG */
    }
}


/********************************************************************************/
EC_T_VOID  LinkOsUnlock(EC_T_VOID* pvLockHandle)
{
    OS_LOCK_DESC* pvLockDesc = (OS_LOCK_DESC*)pvLockHandle;
    
    LinkOsDbgAssert( pvLockDesc != EC_NULL );
    if (pvLockDesc != NULL)
    {
        LinkOsDbgAssert( pvLockDesc->nLockCnt > 0 );
        pvLockDesc->nLockCnt--;
        if( pvLockDesc->nLockCnt == 0 )
        {
#ifdef DEBUG
            S_bSpinLockActive = EC_FALSE;
            S_nSpinLockTaskId = 0;
            pvLockDesc->nTaskId = -1;
#endif /* ifdef DEBUG */
        }
        
        switch(pvLockDesc->eLockType)
        {
            
        default:
        case eLockType_DEFAULT:     /*< Default mutex           */
        case eLockType_INTERFACE:   /*< only interface */
            LinkOsDbgAssert( pvLockDesc->semMId != 0 );
            semGive((SEM_ID)pvLockDesc->semMId);
            break;
            
            /*< only jobs --> spin lock */
        case eLockType_SPIN:
#ifdef USE_SPINLOCKS
            if( spinLockIsrHeld( &pvLockDesc->SpinLock ) )
            {
                LinkOsDbgAssert( 0 == (vxEflagsGet() & 0x200) );
                if( 0 == pvLockDesc->nLockCnt )
                {
                    SPIN_LOCK_ISR_GIVE( &pvLockDesc->SpinLock );
                }
            }
            else
            {
                LinkOsDbgAssert(FALSE);
            }
#else
            LinkOsDbgAssert( pvLockDesc->semMId != 0 );
            semGive((SEM_ID)pvLockDesc->semMId);
#endif /* ifdef USE_SPINLOCKS */
            break;
            
        }
    }
}


/********************************************************************************/
/** \brief Create a manual binary semaphore
*
* \return event object to be referenced in further calls or EC_NULL in case of errors.
*/
EC_T_VOID* LinkOsCreateEvent(EC_T_VOID)
{
    SEM_ID     semBId;
    EC_T_VOID* pvRetVal = EC_NULL;
    
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
EC_T_VOID LinkOsDeleteEvent(EC_T_VOID* pvEvent)
{
    /* delete event */
    semDelete((SEM_ID)pvEvent);
}

EC_T_DWORD G_dwLinkOsLockCounter   = 0;
EC_T_DWORD G_dwLinkOsUnLockCounter = 0;

/********************************************************************************/
/** \brief  Wait for an event object
*
* \return EC_E_NOERROR if event was set for the timeout, or error code in case of errors.
*/
EC_T_DWORD LinkOsWaitForEvent(EC_T_VOID* pvEvent, EC_T_DWORD dwTimeout)
{
    STATUS Status;
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    int   nTicks;
    
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
    G_dwLinkOsLockCounter++;
    
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

#ifdef ASSERT_SUSPEND
/********************************************************************************/
/** \brief 
*
* \return
*/
EC_T_VOID LinkOsDbgAssertFunc(EC_T_BOOL bAssertCondition, EC_T_CHAR* szFile, EC_T_DWORD dwLine)
{
    if( !bAssertCondition )
    {
        printf("ASSERTION in file %s, line %d\n", szFile, dwLine);
        taskSuspend(0);
    }
}
#endif

/********************************************************************************/
/** \brief Add OS Layer Debug Message hook
*
* \return N/A
*/
EC_T_VOID LinkOsAddDbgMsgHook(EC_PF_LINKOSDBGMSGHK pfOsDbgMsgHook)
{
    S_pfLinkOsDbgMsgHook = pfOsDbgMsgHook;
}

/********************************************************************************/
/** \brief 
*
* \return
*/
EC_T_VOID LinkOsDbgMsg(const EC_T_CHAR* szFormat, ...)
{
    EC_T_BOOL bPrintMsg = EC_FALSE;
    va_list vaArgs;
    
    va_start(vaArgs, szFormat);
    if( S_pfLinkOsDbgMsgHook != EC_NULL )
    {
        bPrintMsg = (*S_pfLinkOsDbgMsgHook)(szFormat, vaArgs);
    }
    if( bPrintMsg )
    {
        vprintf(szFormat, vaArgs);
    }
    va_end(vaArgs);
}

/*** Interrupts ***/

typedef struct _T_LOSAL_CEOSALIRQ
{
    EC_T_VOID*      pvIrqEvent;
    
    EC_T_VOID*      pvIstThread;
    EC_T_VOID*      pvStartEvent;
    EC_T_BOOL       bStopThread;

    EC_T_BOOL       bThreadIsRunning;

} T_LOSAL_CEOSALIRQ, *PT_LOSAL_CEOSALIRQ;

#define INITCEOSALIRQ(ptr) \
    if( EC_NULL == (ptr) ) { \
        (ptr) = (PT_LOSAL_CEOSALIRQ)LinkOsMalloc(sizeof(T_LOSAL_CEOSALIRQ)); \
        if( EC_NULL != (ptr) ) { \
            LinkOsMemset( (ptr), 0, sizeof(T_LOSAL_CEOSALIRQ)) ; \
        } }

#define DEINITCEOSALIRQ(ptr) \
    if( EC_NULL != (ptr) ) { \
        LinkOsFree((ptr)); \
        (ptr) = EC_NULL;  \
    }

#define CHECKCEOSALIRQ(ptr) \
    if( EC_NULL == (ptr) ) goto Exit;

/***************************************************************************************************/
/**
\brief  LinkOs IST Thread.
*/
static EC_T_VOID tLinkOsIst( 
    EC_T_VOID*  pvParms     /**< [in]   LOSAL IRQ Parms */
                           )
{
    EC_T_LINKOS_IRQ_PARM*   pInterruptParm  = (EC_T_LINKOS_IRQ_PARM*)pvParms;
    PT_LOSAL_CEOSALIRQ      pIrqLocal       = EC_NULL;
    EC_T_BOOL               bRunning        = EC_TRUE;
    
    if( EC_NULL == pInterruptParm )
    {
        goto Exit;
    }

    CHECKCEOSALIRQ(pInterruptParm->pvLOSALContext);

    pIrqLocal = (PT_LOSAL_CEOSALIRQ) (pInterruptParm->pvLOSALContext);

    pIrqLocal->bThreadIsRunning = EC_TRUE;
        
    /* Signal Thread Start */
    LinkOsSetEvent(pIrqLocal->pvStartEvent);

    while( bRunning )
    {
        /* wait until next loop */
        if( EC_E_NOERROR != LinkOsWaitForEvent(pIrqLocal->pvIrqEvent, EC_WAITINFINITE) )
        {
            /* error */
            bRunning = EC_FALSE;
            continue;
        }
        if( EC_TRUE == pIrqLocal->bStopThread )
        {
            bRunning = EC_FALSE;
            continue;
        }
        /* re-open interrupt closed in the ISR */
        if( EC_NULL != pInterruptParm->pfIrqOpen )
        {
            pInterruptParm->pfIrqOpen( pInterruptParm->pvAdapter );
        }
        /* call given IST Service Function */
        if( EC_NULL != pInterruptParm->pfIstFunction )
        {
            pInterruptParm->pfIstFunction(pInterruptParm->pvAdapter);
        }
    }

    pIrqLocal->bThreadIsRunning = EC_FALSE;

Exit:
    return;
}

#ifdef DEBUG
volatile unsigned osal_intcnt;
#endif /* ifdef DEBUG */

/*******************************************************************************
*
* LinkOsIsr - the ISR
*
*/
EC_T_VOID LinkOsIsr( EC_T_LINKOS_IRQ_PARM* pInterruptParm )
{
#ifdef DEBUG
    osal_intcnt++;
#endif /* ifdef DEBUG */
    
    PT_LOSAL_CEOSALIRQ pIrqLocal = (PT_LOSAL_CEOSALIRQ)pInterruptParm->pvLOSALContext;

    CHECKCEOSALIRQ( pIrqLocal );

    if( EC_NULL == pInterruptParm )
    {
        goto Exit;
    }
    
#if defined(DRV_DEBUG_MEASURE_RX_START)
    /* measure RX */
    DRV_DEBUG_MEASURE_RX_START();
#endif /* if defined(DRV_DEBUG_MEASURE_RX_START) */
    
    /* close IRQ if possible */
    if( EC_NULL != pInterruptParm->pfIrqClose )
    {
        pInterruptParm->pfIrqClose( pInterruptParm->pvAdapter );
    }
    /* acknowledge pending Card IRQ */
    if( EC_NULL != pInterruptParm->pfIrqAckAll )
    {
        pInterruptParm->pfIrqAckAll( pInterruptParm->pvAdapter );
    }
    /* call ISR callback */
    if( EC_NULL != pInterruptParm->pfIsrFunction )
    {
        pInterruptParm->pfIsrFunction(pInterruptParm->pvAdapter, pInterruptParm->pvIsrFuncCtxt);
    }
    /* trigger IST */
    LinkOsSetEvent(pIrqLocal->pvIrqEvent);
    
Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Initialize Interrupt and set up env.

\return EC_TRUE on success, EC_FALSE otherwise.
*/
EC_T_BOOL LinkOsInterruptInitialize( 
    EC_T_LINKOS_IRQ_PARM*   pInterruptParm  /**< [in]   Interrupt Parameter structure */
                                   )
{
EC_T_BOOL           bRetVal   = EC_FALSE;
PT_LOSAL_CEOSALIRQ  pIrqLocal = EC_NULL; /* set after INITCEOSALIRQ / CHECKCEOSALIRQ */

#if (((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9)) || (_WRS_VXWORKS_MAJOR > 6)) && !defined(NO_PCI_SUPPORT)
#ifdef SUPPORT_LEGACY_VXBUS
    vxbIntEnable(pInterruptParm->pPciInfo->pDev->pVxbDev, 0, (VOIDFUNCPTR)LinkOsIsr, (void*)pInterruptParm);
#else
    /* TODO */
#endif
#else
STATUS stat;

#ifdef DEBUG
    osal_intcnt = 0;
#endif /* ifdef DEBUG */
    
#if defined(LL_PLATFORM_INTINIT)
    LL_PLATFORM_INTINIT(pInterruptParm);
#endif /* if defined(LL_PLATFORM_INTINIT) */

    INITCEOSALIRQ( pInterruptParm->pvLOSALContext );
    
    CHECKCEOSALIRQ( pInterruptParm->pvLOSALContext );
    
    pIrqLocal = (PT_LOSAL_CEOSALIRQ)pInterruptParm->pvLOSALContext;

#ifndef EXCLUDE_SYS_LOSAL 
    /* check for SysLosal connection */    
    if (EC_NULL == S_pfnSysLoSalIntConnect)
    {
        LinkOsDbgMsg( "FATAL Error in LinkOsInterruptInitialize, SysLoSal not found!\n" );
        goto Exit;
    }
#endif /* ifndef EXCLUDE_SYS_LOSAL */   

    /* disable irq */
    sysIntDisablePIC( pInterruptParm->dwSysIrq );

    /* disable card IRQ */
    if( EC_NULL != pInterruptParm->pfIrqClose )
    {
        pInterruptParm->pfIrqClose( pInterruptParm->pvAdapter );
    }

    /* acknowledge pending Card IRQ */
    if( EC_NULL != pInterruptParm->pfIrqAckAll )
    {
        pInterruptParm->pfIrqAckAll( pInterruptParm->pvAdapter );
    }
    /* populate IRQ Local */
    pIrqLocal->pvIrqEvent = LinkOsCreateEvent();
    if( EC_NULL == pIrqLocal->pvIrqEvent )
    {
        goto Exit;
    }

    pIrqLocal->pvStartEvent = LinkOsCreateEvent();
    if( EC_NULL == pIrqLocal->pvStartEvent )
    {
        goto Exit;
    }

    pIrqLocal->bStopThread      = EC_FALSE;
    pIrqLocal->bThreadIsRunning = EC_FALSE;

    /* activate IRQ */

    if (pInterruptParm->eIntSource == INTSOURCE_PCI)
    {
#if defined(NO_PCI_SUPPORT)
       stat = ERROR;
#else
#  ifdef EXCLUDE_SYS_LOSAL
       /* We can call pciIntConnect() directly, because INUM_TO_IVEC translation is not neccesary */
       stat = pciIntConnect((VOIDFUNCPTR *)pInterruptParm->dwSysIrq, (VOIDFUNCPTR)LinkOsIsr, (EC_T_INT)pInterruptParm);
#  else
       /* Call BSP routine SysLoSalIntConnect() (sysLosalAdd.c) which in turn calls pciIntConnect() */
       stat = S_pfnSysLoSalIntConnect(pInterruptParm->dwSysIrq, (VOIDFUNCPTR)LinkOsIsr, (EC_T_INT)((EC_T_BYTE*)pInterruptParm - (EC_T_BYTE*)0));
#  endif /* ifdef EXCLUDE_SYS_LOSAL */
#endif /* if defined(NO_PCI_SUPPORT) */
    }
    else
    {
       /*
        * This path is mainly used for PPC SoC LinkLayer's (i.e. eTSEC), 
        * because pciIntConnect() doesn't fit for non PCI interrupts (pciIntDisconnect2() fails, pciIntConnect() not).
        * Note that the interrupt number (irq) is the identical to the Interrupt-vector on PPC,
        * so INUM_TO_IVEC translation isn't necessary. This can be different for other architectures (i.e. ARM).
        */
#if ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9)) || (_WRS_VXWORKS_MAJOR > 6)
    stat = intConnect((VOIDFUNCPTR*)((EC_T_BYTE*)0 + pInterruptParm->dwSysIrq), (VOIDFUNCPTR)LinkOsIsr, (_Vx_usr_arg_t)pInterruptParm);
#else
    stat = intConnect((VOIDFUNCPTR*)((EC_T_BYTE*)0 + pInterruptParm->dwSysIrq), (VOIDFUNCPTR)LinkOsIsr, (int)pInterruptParm);   
#endif
    }
    if (stat != OK)
    {
       LinkOsDbgMsg( "Error connecting interrupt handler (0x%08x)\n", errnoGet());
    }
    
    stat = sysIntEnablePIC(pInterruptParm->dwSysIrq);
    if (stat != OK)
    {
       LinkOsDbgMsg( "Error enabling interrupt (0x%08x)\n", errnoGet());
    }
#endif /* (_WRS_VXWORKS_MAJOR >= 7) */

    /* create IST thread */
    if( EC_NULL == pInterruptParm->pfIsrFunction )
    {
        pIrqLocal->pvIstThread = LinkOsCreateThread(
            (EC_T_CHAR*)"tLOsaL_IST", 
            (EC_PF_THREADENTRY)tLinkOsIst, 
            pInterruptParm->dwIstPriority, 
            pInterruptParm->dwIstStackSize, 
            (EC_T_PVOID)pInterruptParm
            );       
        if (EC_NULL == pIrqLocal->pvIstThread)
        {
            goto Exit;
        }        
        if (EC_E_TIMEOUT == LinkOsWaitForEvent(pIrqLocal->pvStartEvent, 3000))
        {
            SYS_ERRORMSGC(("I8254x-EcLinkOpen(): Cannot Initialize interrupt\n"));
            goto Exit;
        }
    } 
    /* enable card IRQ */
    if (EC_NULL != pInterruptParm->pfIrqOpen)
    {
        pInterruptParm->pfIrqOpen(pInterruptParm->pvAdapter);
    }
    bRetVal = EC_TRUE;
Exit:
    return bRetVal;
}

/***************************************************************************************************/
/**
\brief  Deprecated
*/
EC_T_VOID LinkOsInterruptDone(
    EC_T_LINKOS_IRQ_PARM*   pInterruptParm  /**< [in]   Interrupt parameter structure */
                             )
{
    EC_UNREFPARM(pInterruptParm);
}

/***************************************************************************************************/
/**
\brief  Disable IRQ.
*/
EC_T_VOID LinkOsInterruptDisable(
    EC_T_LINKOS_IRQ_PARM*   pInterruptParm  /**< [in]   Interrupt parameter structure */
                                )
{
PT_LOSAL_CEOSALIRQ pIrqLocal = EC_NULL;
EC_T_INT nTimeoutCnt = 0;

#if (((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9)) || (_WRS_VXWORKS_MAJOR > 6)) && !defined(NO_PCI_SUPPORT)
#ifdef SUPPORT_LEGACY_VXBUS
    vxbIntDisable(pInterruptParm->pPciInfo->pDev->pVxbDev, 0, (VOIDFUNCPTR)LinkOsIsr, (void*)pInterruptParm);
#else
    /* TODO */
#endif
#else
    STATUS stat;

    if( EC_NULL != (pInterruptParm->pfIrqClose) )
    {
        pInterruptParm->pfIrqClose( pInterruptParm->pvAdapter );
    }

    CHECKCEOSALIRQ( pInterruptParm->pvLOSALContext );
    
    pIrqLocal = (PT_LOSAL_CEOSALIRQ) (pInterruptParm->pvLOSALContext);
   
#ifndef EXCLUDE_SYS_LOSAL
    /* check for SysLosal connection */    
    if (EC_NULL == S_pfnSysLoSalIntDisconnect)
    {
        LinkOsDbgMsg( "FATAL Error in LinkOsInterruptDisable, SysLoSal not found!\n" );
        goto Exit;
    }
#endif /* ifndef EXCLUDE_SYS_LOSAL */
    
    stat = sysIntDisablePIC( pInterruptParm->dwSysIrq );
    if (stat != OK)
    {
       LinkOsDbgMsg( "Error disabling interrupt (0x%08x)\n", errnoGet());
    }

    if (pInterruptParm->eIntSource == INTSOURCE_PCI)
    {
#if defined(NO_PCI_SUPPORT)
       stat = ERROR;
#else
#  ifdef EXCLUDE_SYS_LOSAL 
       /* We can call pciIntDisconnect2() directly, because INUM_TO_IVEC translation is not neccesary */
       stat = pciIntDisconnect2((VOIDFUNCPTR *)pInterruptParm->dwSysIrq, (VOIDFUNCPTR)LinkOsIsr, (EC_T_INT)pInterruptParm);
#  else
       /* Call BSP routine SysLoSalIntDisconnect() (sysLosalAdd.c) which in turn calls pciIntDisconnect2() */
       stat = S_pfnSysLoSalIntDisconnect(pInterruptParm->dwSysIrq, (VOIDFUNCPTR)LinkOsIsr, (EC_T_INT)((EC_T_BYTE*)pInterruptParm - (EC_T_BYTE*)0));
#  endif /* ifdef EXCLUDE_SYS_LOSAL  */
#endif /* defined(NO_PCI_SUPPORT) */
    }
    else
    {
       /*
        * This path is mainly used for PPC SoC LinkLayer's (i.e. eTSEC), 
        * because pciIntDisconnect() fails for non PCI interrupts.
        * Note that the interrupt number (irq) is identical to the Interrupt-vector on PPC,
        * so INUM_TO_IVEC translation isn't necessary. This could be different for other architectures (i.e. ARM).
        */
#if ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9)) || (_WRS_VXWORKS_MAJOR > 6)
    	stat = intDisconnect((VOIDFUNCPTR*)((EC_T_BYTE*)0 + pInterruptParm->dwSysIrq), (VOIDFUNCPTR)LinkOsIsr, (_Vx_usr_arg_t)pInterruptParm);
#else
#if defined(VXWORKS54) || defined(VXWORKS55)
       /* There is no intDisconnect() in VxWorks <= 5.5. */
       stat = ERROR;
#else
       stat = intDisconnect((VOIDFUNCPTR*)((EC_T_BYTE*)0 + pInterruptParm->dwSysIrq), (VOIDFUNCPTR)LinkOsIsr, (int)pInterruptParm);
#endif
#endif
    }

    if (stat != OK)
	 {
       LinkOsDbgMsg( "Error disconnecting interrupt handler (0x%08x)\n", errnoGet());
	 }

#endif /* (_WRS_VXWORKS_MAJOR >= 7) */

    /* shut down IST thread if running */
    pIrqLocal->bStopThread = EC_TRUE;
    LinkOsSetEvent(pIrqLocal->pvIrqEvent);
    nTimeoutCnt = 0;
    while(pIrqLocal->bThreadIsRunning && (nTimeoutCnt < 500))
    {
        LinkOsSleep(10);
        nTimeoutCnt++;
    }

    /* clean up IRQ Local */
    LinkOsDeleteThreadHandle(pIrqLocal->pvIstThread);
    pIrqLocal->pvIstThread = EC_NULL;

    if( EC_NULL != pIrqLocal->pvIrqEvent )
        LinkOsDeleteEvent(pIrqLocal->pvIrqEvent);
    
    pIrqLocal->pvIrqEvent = EC_NULL;
    
    if( EC_NULL != pIrqLocal->pvStartEvent )
        LinkOsDeleteEvent(pIrqLocal->pvStartEvent);
    
    pIrqLocal->pvStartEvent     = EC_NULL;
    
    pIrqLocal->bStopThread      = EC_FALSE;
    pIrqLocal->bThreadIsRunning = EC_FALSE;

#if (((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9)) || (_WRS_VXWORKS_MAJOR > 6)) && !defined(NO_PCI_SUPPORT)
#else
Exit:
#endif
    DEINITCEOSALIRQ( pInterruptParm->pvLOSALContext )
    return;
}

/*-END OF SOURCE FILE--------------------------------------------------------*/
