/*-----------------------------------------------------------------------------
 * LinkOsPci.c
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Kai Olbrich
 * Description              Generic (os independent) PCI access functions
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "LinkOsLayer.h"
#include "EcLink.h"

#if !((defined UNDER_CE) || ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9) && (_WRS_CONFIG_LP64)) || (_WRS_VXWORKS_MAJOR > 6))

/*-DEFINES-------------------------------------------------------------------*/

#define BUSMAP_CNT                    64 /* Entries in PCI map (max PCI devices) */
#define PCI_MAX_BUS                   255
#define PCI_MAX_DEVICES               32
#define PCI_MAX_FUNCTION              8

#define PCI_CMD_IO_ENABLE             0x0001
#define PCI_CMD_MEM_ENABLE            0x0002
#define PCI_CMD_MASTER_ENABLE         0x0004

#define PCI_CFG_VENDOR_ID             0x00
#define PCI_CFG_COMMAND               0x04
#define PCI_CFG_SUBCLASS              0x0a
#define PCI_CFG_HEADER_TYPE           0x0e
#define PCI_CFG_BASE_ADDRESS_0        0x10
#define PCI_CFG_BASE_ADDRESS_2        0x18
#define PCI_CFG_SECONDARY_BUS         0x19
#define PCI_CFG_BASE_ADDRESS_3        0x1c
#define PCI_CFG_DEV_INT_LINE          0x3c


#define PCI_IOBASE_MASK              (~0x3)
#define PCI_MEMBASE_MASK             (~0xf)

#define PCI_BAR_SPACE_MASK            0x01
#define PCI_BAR_SPACE_IO              0x01
#define PCI_BAR_SPACE_MEM             0x00

#define PCI_BAR_MEM_TYPE_MASK         0x06
#define PCI_BAR_MEM_ADDR32            0x00
#define PCI_BAR_MEM_BELOW_1MB         0x02
#define PCI_BAR_MEM_ADDR64            0x04
#define PCI_BAR_MEM_RESERVED          0x06

#define PCI_CLASS_BRIDGE_CTLR         0x06
#define PCI_SUBCLASS_P2P_BRIDGE       0x04
#define PCI_SUBCLASS_HOST_PCI_BRIDGE  0x00
#define PCI_SUBCLASS_CARDBUS_BRIDGE   0x07

#define PCI_HEADER_TYPE_MASK          0x7f
#define PCI_HEADER_PCI_CARDBUS        0x02
#define PCI_HEADER_PCI_PCI            0x01
#define PCI_HEADER_TYPE_BRIDGE        0x01
#define PCI_HEADER_TYPE0              0x00
#define PCI_HEADER_MULTI_FUNC         0x80

/*-GLOBALS-------------------------------------------------------------------*/

/*-LOCALS-------------------------------------------------------------------*/

/* TODO if all LinkLayers using the new PCI layer:
 * Kill macros in LinkOsLayer.h and move the definition to here.
 * Add extern decl. in LinkOsLayer.h.
 */
       LOSAL_T_CARD_IDENTIFY G_oKnownGeiCards[]     = LOSAL_KNOWN_GEI_CARDS; /* This symbol is global because of emllInstallDeviceI8254x()! */
static LOSAL_T_CARD_IDENTIFY S_oKnownFeiCards[]     = LOSAL_KNOWN_FEI_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oKnownRTL8139Cards[] = LOSAL_KNOWN_RTL_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oKnownRTL8169Cards[] = LOSAL_KNOWN_RTL8169_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oKnownR6040Cards[]   = LOSAL_KNOWN_R6040_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oKnownCCATCards[]    = LOSAL_KNOWN_CCAT_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oKnownEG20TCards[]   = LOSAL_KNOWN_EG20T_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oKnownAlteraTSECards[]   = LOSAL_KNOWN_ALTERATSE_CARDS;

typedef struct _T_PCIDESC
{
   EC_T_WORD wVendorId;
   EC_T_WORD wDeviceId;
   EC_T_BYTE bBusNo;
   EC_T_BYTE bDeviceNo;
   EC_T_BYTE bFunctionNo;
} T_PCIDESC;

#if !(defined RTAI)
static EC_T_DWORD S_dwPciBusMapCnt;
static T_PCIDESC  S_oPciBusMap[BUSMAP_CNT];
#endif

typedef enum E_PCI_SCAN_ALGO
{
   ePCI_SCANALGO_BUSORDER,
   ePCI_SCANALGO_LISTORDER,
   ePCI_SCANALGO_OS
} E_PCI_SCAN_ALGO;

typedef struct _T_CARDMAPPING
{
   const EC_T_CHAR*       sIdentStr;
   LOSAL_T_CARD_IDENTIFY* pIdentAry;

} T_CARDMAPPING;

static T_CARDMAPPING S_oCardMappig[] =
{
   { EC_LINK_PARMS_IDENT_I8254X,  G_oKnownGeiCards },
   { EC_LINK_PARMS_IDENT_I8255X,  S_oKnownFeiCards },
   { EC_LINK_PARMS_IDENT_RTL8139, S_oKnownRTL8139Cards },
   { EC_LINK_PARMS_IDENT_RTL8169, S_oKnownRTL8169Cards },
   { EC_LINK_PARMS_IDENT_R6040,   S_oKnownR6040Cards },
   { EC_LINK_PARMS_IDENT_CCAT,    S_oKnownCCATCards },
   { EC_LINK_PARMS_IDENT_EG20T,   S_oKnownEG20TCards },
   { EC_LINK_PARMS_IDENT_ALTERATSE, S_oKnownAlteraTSECards },
};

#define MAPPING_CNT (sizeof(S_oCardMappig) / sizeof(S_oCardMappig[0]))

/*-LOCAL FUNCTIONS-----------------------------------------------------------*/

#if (defined RTAI)

#ifdef __cplusplus
extern "C" {
#endif
EC_T_BOOL LinkOsPciScanForCard_OSOrder(
	T_PCIINFO*             pInfo,    	/**< [out]  Scanned information for PCI card */
    EC_T_DWORD             dwUnit,      /**< [in]   Unit number 1=first, 2=second ... card */
    LOSAL_T_CARD_IDENTIFY* paCardList   /**< [in]   PCI card list */
                        );
#ifdef __cplusplus
}
#endif

#else /* !RTAI */
static EC_T_BOOL LinkOsPciScanBus(EC_T_DWORD dwBus)
{
   EC_T_BOOL  bRet = EC_FALSE;
   EC_T_DWORD dwDevice;
   EC_T_DWORD dwFunction;
   EC_T_DWORD dwVendWord;
   EC_T_WORD  wClass;
   EC_T_BYTE  byTmp;

   /* printf("LinkOsPciScanBus(%u) +\n", dwBus); */

   /* walk through the devices on the bus */
   for (dwDevice = 0; dwDevice < PCI_MAX_DEVICES; dwDevice++)
   {
      /* walk through the functions of the device */
      for (dwFunction = 0; dwFunction < PCI_MAX_FUNCTION; dwFunction++)
      {
         /* get PCI common header */
         if (LinkOsPciRead32(dwBus, dwDevice, dwFunction, PCI_CFG_VENDOR_ID, &dwVendWord) == EC_FALSE)
         {
            goto Exit;
         }

         /* check for a valid vendor number */
         if ( (0xFFFF == EC_LOWORD(dwVendWord)) || (0x0000 == EC_LOWORD(dwVendWord)) )
         {
            if (dwFunction == 0)
            {
               break; /* non-existent device, go to next device */
            }
            else
            {
               continue; /* function empty, try the next function */
            }
         }

         /* get PCI sub class */
         if (LinkOsPciRead16(dwBus, dwDevice, dwFunction, PCI_CFG_SUBCLASS, &wClass) == EC_FALSE)
         {
            goto Exit;
         }

         /* handle non P2P bridges */
         if ( ((PCI_CLASS_BRIDGE_CTLR << 8) | PCI_SUBCLASS_HOST_PCI_BRIDGE) != wClass )
         {
            LinkOsDbgAssert(S_dwPciBusMapCnt < BUSMAP_CNT);

            S_oPciBusMap[S_dwPciBusMapCnt].bBusNo      = dwBus;
            S_oPciBusMap[S_dwPciBusMapCnt].bDeviceNo   = dwDevice;
            S_oPciBusMap[S_dwPciBusMapCnt].bFunctionNo = dwFunction;
            S_oPciBusMap[S_dwPciBusMapCnt].wVendorId   = EC_LOWORD(dwVendWord);
            S_oPciBusMap[S_dwPciBusMapCnt].wDeviceId   = EC_HIWORD(dwVendWord);

            /*
            printf("%3u: Bus %2u Dev %2u Fun %2u: VendId 0x%04x, DevId 0x%04x\n",
                     S_dwPciBusMapCnt,
                     dwBus, dwDevice, dwFunction, EC_LOWORD(dwVendWord), EC_HIWORD(dwVendWord));
            */
            
            S_dwPciBusMapCnt++;
         }

         /* if P2P bridge, explore that bus recursively */
         if (   (((PCI_CLASS_BRIDGE_CTLR << 8) + PCI_SUBCLASS_P2P_BRIDGE) == wClass)
             || (((PCI_CLASS_BRIDGE_CTLR << 8) + PCI_SUBCLASS_CARDBUS_BRIDGE) == wClass)
            )
         {
            if (LinkOsPciRead8(dwBus, dwDevice, dwFunction, PCI_CFG_SECONDARY_BUS, &byTmp) == EC_FALSE)
            {
               goto Exit;
            }
            if (0 < byTmp)
            {
               if (! LinkOsPciScanBus(byTmp))
               {
                  goto Exit;
               }
            }
         }
         /* proceed to next device if single function device */
         if (0 == dwFunction)
         {
            if (LinkOsPciRead8(dwBus, dwDevice, dwFunction, PCI_CFG_HEADER_TYPE, &byTmp) == EC_FALSE)
            {
               goto Exit;
            }

            if (0 == (byTmp & PCI_HEADER_MULTI_FUNC))
            {
               break; /* no more functions - proceed to next PCI device */
            }
         }
      } /* Functions */
   } /* Devices */

   bRet = EC_TRUE;

Exit:

   /* printf("LinkOsPciScanBus(%u) - %u\n", dwBus, bRet); */

   return bRet;
}


/***************************************************************************************************/
/**
\brief  Scan PCI bus for a card with vendor and device ID given in paCardList.
        Scan algorithm: Use PCI bus sequence

\return EC_TRUE if card is found, EC_FALSE otherwise.
*/
static EC_T_BOOL LinkOsPciScanForCard_BusOrder(
    EC_T_DWORD*            pdwBus,       /**< [out]  PCI Bus number */
    EC_T_DWORD*            pdwDevice,    /**< [out]  PCI Device number */
    EC_T_DWORD*            pdwFunction,  /**< [out]  PCI Function number */
    EC_T_INT*              pnHwId,       /**< [out]  Hardware Ident */
    EC_T_DWORD             dwUnit,       /**< [in]   Unit number 1=first, 2=second ... card */
    LOSAL_T_CARD_IDENTIFY* paCardList    /**< [in]   PCI card list */
                        )
{
    EC_T_INT           nKnownCardIdx   = 0;
    EC_T_DWORD         dwUnitCur       = 0;
    EC_T_BOOL          bFound          = EC_FALSE;
    EC_T_DWORD         i;
    T_PCIDESC*         pPci;

    /* check parameters */
    if ( NULL == pdwBus || NULL == pdwDevice || NULL == pdwFunction || NULL == pnHwId )
    {
        goto Exit;
    }

    for (i = 0; (i < S_dwPciBusMapCnt) && (! bFound); ++i)
    {
      pPci = &S_oPciBusMap[i];

      /* check if current card is known */
      for (nKnownCardIdx = 0; ; nKnownCardIdx++)
      {
         if (paCardList[nKnownCardIdx].wVendorId == 0)
         {
            /* end of the known cards table */
            break;
         }

         if (   (paCardList[nKnownCardIdx].wVendorId == pPci->wVendorId)
             && (paCardList[nKnownCardIdx].wDeviceId == pPci->wDeviceId)
            )
         {
            /* check for right instance */
            dwUnitCur++;
            if (dwUnitCur == dwUnit)
            {
               *pnHwId      = paCardList[nKnownCardIdx].wHWId;
               *pdwBus      = pPci->bBusNo;
               *pdwDevice   = pPci->bDeviceNo;
               *pdwFunction = pPci->bFunctionNo;
               bFound       = EC_TRUE;
               goto Exit;
            }
         }
      }
   }

Exit:
   return bFound;
}

/***************************************************************************************************/
/**
\brief  Scan PCI bus for a card with vendor and device ID given in paCardList.
        Scan algorithm: Use paCardList sequence

\return EC_TRUE if card is found, EC_FALSE otherwise.
*/
static EC_T_BOOL LinkOsPciScanForCard_ListOrder(
    EC_T_DWORD*            pdwBus,       /**< [out]  PCI Bus number */
    EC_T_DWORD*            pdwDevice,    /**< [out]  PCI Device number */
    EC_T_DWORD*            pdwFunction,  /**< [out]  PCI Function number */
    EC_T_INT*              pnHwId,       /**< [out]  Hardware Ident */
    EC_T_DWORD             dwUnit,       /**< [in]   Unit number 1=first, 2=second ... card */
    LOSAL_T_CARD_IDENTIFY* paCardList    /**< [in]   PCI card list */
                        )
{
    EC_T_INT           nKnownCardIdx;
    EC_T_DWORD         dwUnitCur       = 0;
    EC_T_BOOL          bFound          = EC_FALSE;
    EC_T_DWORD         i;
    T_PCIDESC*         pPci;

    /* check parameters */
    if ( NULL == pdwBus || NULL == pdwDevice || NULL == pdwFunction || NULL == pnHwId )
    {
        goto Exit;
    }

    for (nKnownCardIdx = 0; paCardList[nKnownCardIdx].wVendorId != 0; nKnownCardIdx++)
    {
       /* Scan bus for this card */
       for (i = 0; (i < S_dwPciBusMapCnt) && (! bFound); ++i)
       {
          pPci = &S_oPciBusMap[i];

          if (   (paCardList[nKnownCardIdx].wVendorId == pPci->wVendorId)
              && (paCardList[nKnownCardIdx].wDeviceId == pPci->wDeviceId)
             )
          {
             /* check for right instance */
             dwUnitCur++;
             if (dwUnitCur == dwUnit)
             {
                *pnHwId      = paCardList[nKnownCardIdx].wHWId;
                *pdwBus      = pPci->bBusNo;
                *pdwDevice   = pPci->bDeviceNo;
                *pdwFunction = pPci->bFunctionNo;
                bFound       = EC_TRUE;
                goto Exit;
             }
          }
       }
    }

Exit:
   return bFound;
}


/***************************************************************************************************/
/**
\brief  Scan PCI bus for a card with vendor and device ID given in paCardList.
        Scan algorithm: Use scan function provided by the underlying OS.
                        Simulate the behavior of LinkOsPciScanForCard_ListOrder()
                        
\return EC_TRUE if card is found, EC_FALSE otherwise.
*/
static EC_T_BOOL LinkOsPciScanForCard_OSOrder(
	T_PCIINFO*             pInfo,        /**< [out]  PCI Info */
    EC_T_DWORD             dwUnit,       /**< [in]   Unit number 1=first, 2=second ... card */
    LOSAL_T_CARD_IDENTIFY* paCardList    /**< [in]   PCI card list */
                        )
{
   EC_T_INT           nKnownCardIdx;
   EC_T_BOOL          bFound         = EC_FALSE;
   EC_T_DWORD         dwCardFound;
   EC_T_DWORD         dwCnt          = 0;

   /* check parameters */
   if ( NULL == pInfo || dwUnit < 1)
   {
       goto Exit;
   }

   for (nKnownCardIdx = 0; paCardList[nKnownCardIdx].wVendorId != 0; nKnownCardIdx++)
   {
      dwCardFound = 1;
      for (;;)
      {
         /*
         printf("Scan: Ven %x, Dev %x, Inst %d, FoundCards %d\n",
               paCardList[nKnownCardIdx].wVendorId, paCardList[nKnownCardIdx].wDeviceId, dwCardFound, dwCnt);
         */

         /* Scan bus for this card */
#ifdef ECLINKOS_USE_OS_PCICONFIGURE
          if (LinkOsPciFindDevInfo(paCardList[nKnownCardIdx].wVendorId,
                                   paCardList[nKnownCardIdx].wDeviceId,
                                   dwCardFound,
                                   pInfo) == EC_TRUE)
#else
         if (LinkOsPciFindDev(paCardList[nKnownCardIdx].wVendorId, 
                           paCardList[nKnownCardIdx].wDeviceId,
                           dwCardFound,
						   &pInfo->Pci.dwBusNumber,
						   &pInfo->Pci.dwDeviceNumber,
						   &pInfo->Pci.dwFunctionNumber) == EC_TRUE)
#endif /* ECLINKOS_USE_OS_PCICONFIGURE*/
         {
            dwCnt++; /* global instance counter */
            dwCardFound++; /* instance counter for the current card (vendor, device) */

            /* select the way to identify the adapter */
            if ((dwUnit & 0xFF000000) == EC_LINKUNIT_PCILOCATION)
            {
                /* look for adapter using PCI bus device function information */
                if (((EC_T_DWORD)((dwUnit>>16) & 0x000000FF) == pInfo->Pci.dwBusNumber)
                 && ((EC_T_DWORD)((dwUnit>> 8) & 0x000000FF) == pInfo->Pci.dwDeviceNumber)
                 && ((EC_T_DWORD)((dwUnit>> 0) & 0x000000FF) == pInfo->Pci.dwFunctionNumber))
                {
                   /* Card found */
                   bFound = EC_TRUE;
                   goto Exit;
                }
            }
            else
            {
                /* look for adapter using instance number */
                if (dwCnt == dwUnit)
                {
                   /* Card found */
                   bFound = EC_TRUE;
                   goto Exit;
                }
            }
         }
         else
         {
            /* No further instances of the current card present (vendor, device) */
            break;
         }
      }
   }

Exit:
    if (bFound)
    {
        pInfo->nCardHWId = paCardList[nKnownCardIdx].wHWId;
    }
return bFound;
}
#endif /* !RTAI */

/*******************************************************************************
*
* SysGetPciInfo - get PCI info
*
*/
static EC_T_BOOL LinkOsGetPciInfoInternal(
         T_PCIINFO*             pInfo,
         EC_T_DWORD             dwUnit,
         LOSAL_T_CARD_IDENTIFY* aKnownCards,
         E_PCI_SCAN_ALGO        eScanAlgo)
{
    EC_T_BOOL           bRetVal         = EC_FALSE;
    EC_T_BOOL           bRes            = EC_FALSE;
#ifndef ECLINKOS_USE_OS_PCICONFIGURE
    EC_T_INT            nBARIdx         = 0;
    EC_T_DWORD          dwReg           = 0;
    EC_T_DWORD          dwBaseAddress   = 0;
    EC_T_DWORD          dwSize          = 0;
    EC_T_DWORD          dwPciCmd        = 0;
    EC_T_DWORD          dwRegOffs       = 0;
    EC_T_BYTE           byHeaderType    = 0;
    EC_T_DWORD          dwBARCnt        = 0;
    EC_T_BOOL           bAddr64         = EC_FALSE;
    EC_T_BYTE           byIntNo         = 0;
#endif

    LinkOsPciInit();

    if (eScanAlgo == ePCI_SCANALGO_OS)
    {
       /* Use scan function provided by the underlying OS */
       bRes = LinkOsPciScanForCard_OSOrder(pInfo, dwUnit, aKnownCards);
#if !(defined RTAI)
    }
    else
    {
       /* Scan bus only once */    
       if (S_dwPciBusMapCnt == 0)
       {
          /* Scan PCI bus once and store scan results in memory (cache) */
          LinkOsPciScanBus(0);
       }
   
       /* look for the card (in PCI memory map) */
       if (eScanAlgo == ePCI_SCANALGO_BUSORDER)
       {
          bRes = LinkOsPciScanForCard_BusOrder(
                   &pInfo->Pci.dwBusNumber, &pInfo->Pci.dwDeviceNumber, &pInfo->Pci.dwFunctionNumber,
                   &pInfo->nCardHWId, dwUnit, aKnownCards);
       }
       else
       {
          bRes = LinkOsPciScanForCard_ListOrder(
                   &pInfo->Pci.dwBusNumber, &pInfo->Pci.dwDeviceNumber, &pInfo->Pci.dwFunctionNumber,
                   &pInfo->nCardHWId, dwUnit, aKnownCards);
       }
#endif
    }

    if (EC_FALSE == bRes)
    {
        goto Exit;
    }
    
    /* fill rest of the structure */
    pInfo->Pci.cbSize          = sizeof(DDKPCIINFO);
    pInfo->Pci.dwInstanceIndex = dwUnit;
    pInfo->Pci.dwWhichIds      = 0;

#ifdef ECLINKOS_USE_OS_PCICONFIGURE
    if (!LinkOsPciConfigure(pInfo))
    {
       goto Exit;
    }
#else
    /* enable mapped memory and IO addresses */
    dwPciCmd = PCI_CMD_IO_ENABLE | PCI_CMD_MEM_ENABLE | PCI_CMD_MASTER_ENABLE;
    LinkOsPciWrite32(pInfo->Pci.dwBusNumber, pInfo->Pci.dwDeviceNumber, pInfo->Pci.dwFunctionNumber, PCI_CFG_COMMAND, &dwPciCmd);

    /* get interrupt */
    LinkOsPciRead8(pInfo->Pci.dwBusNumber, pInfo->Pci.dwDeviceNumber, pInfo->Pci.dwFunctionNumber, PCI_CFG_DEV_INT_LINE, &byIntNo);
    pInfo->Isr.dwSysintr = byIntNo; /* Don't pass dwSysintr to LinkOsPciRead8() because we also support big endianness (PPC) */
    
    /* get number of BAR's */
    LinkOsPciRead8(pInfo->Pci.dwBusNumber, pInfo->Pci.dwDeviceNumber, pInfo->Pci.dwFunctionNumber, PCI_CFG_HEADER_TYPE, &byHeaderType);
    byHeaderType = byHeaderType & PCI_HEADER_TYPE_MASK;

    switch (byHeaderType)
    {
       case PCI_HEADER_TYPE0:       dwBARCnt = 6; break;
       case PCI_HEADER_PCI_PCI:     dwBARCnt = 2; break;
       case PCI_HEADER_PCI_CARDBUS: dwBARCnt = 1; break;
       default:                     dwBARCnt = 0; break;
    }

    /* get BARs */
    LinkOsMemset((EC_T_VOID*)&pInfo->Window, 0, sizeof(DDKWINDOWINFO));

    for (nBARIdx = 0; nBARIdx < dwBARCnt; nBARIdx++)
    {
       /* calculate BAR address */
       dwRegOffs = PCI_CFG_BASE_ADDRESS_0 + (sizeof(EC_T_DWORD) * nBARIdx);

       /* handle 64 bit BAR */
       if (bAddr64)
       {
           /* previous BAR was 64bit, skip current BAR */
           bAddr64 = EC_FALSE;
           continue;
       }

       /* read BAR */
       LinkOsPciRead32(pInfo->Pci.dwBusNumber, pInfo->Pci.dwDeviceNumber, pInfo->Pci.dwFunctionNumber, dwRegOffs, &dwReg);
       if ((0x00000000 == dwReg) || (0xFFFFFFFF == dwReg))
       {
          /* BAR is not implemented */
          continue;
       }

       /* read the length / address */
       dwPciCmd = 0xFFFFFFFF;
       LinkOsPciWrite32(pInfo->Pci.dwBusNumber, pInfo->Pci.dwDeviceNumber, pInfo->Pci.dwFunctionNumber, dwRegOffs, &dwPciCmd);
       LinkOsPciRead32(pInfo->Pci.dwBusNumber, pInfo->Pci.dwDeviceNumber, pInfo->Pci.dwFunctionNumber, dwRegOffs, &dwBaseAddress);
       LinkOsPciWrite32(pInfo->Pci.dwBusNumber, pInfo->Pci.dwDeviceNumber, pInfo->Pci.dwFunctionNumber, dwRegOffs, &dwReg);

       if (dwReg & 1)
       {
          /* IO space */
          /* Re-adjust dwBaseAddress if upper 16-bits are 0 (this is allowable in PCI 2.2 spec) */
          if (((dwBaseAddress & PCI_IOBASE_MASK) != 0) && ((dwBaseAddress & 0xFFFF0000) == 0))
          {
             dwBaseAddress |= 0xFFFF0000;
          }

          dwSize = ~(dwBaseAddress & PCI_IOBASE_MASK);

          if ((dwBaseAddress != 0) && (dwBaseAddress != 0xFFFFFFFF) && (((dwSize + 1) & dwSize) == 0))
          {
             /* BAR has valid format (consecutive high 1's and consecutive low 0's) */
             pInfo->Window.ioWindows[pInfo->Window.dwNumIoWindows].dwLen    = dwSize + 1;
             pInfo->Window.ioWindows[pInfo->Window.dwNumIoWindows++].dwBase = dwReg & PCI_IOBASE_MASK;
          }
          else
          {
             /* BAR invalid => skip to next one */
             continue;
          }

       }
       else
       {
          /* Memory space */
          if ((dwBaseAddress & 6) == 2)
          {
             /* PCI 2.2 spec no longer supports this type of memory addressing */
             continue;
          }

          /* process address attribute info */
          switch (dwReg & PCI_BAR_MEM_TYPE_MASK)
          {
             case PCI_BAR_MEM_ADDR32:
             case PCI_BAR_MEM_BELOW_1MB:
             {
                break; /* OK */
             }
             case PCI_BAR_MEM_ADDR64:
             {
               /* currently physical addresses above 4Gb are not supported,
                * so for right now we'll skip the next base address register
                * which belongs to the 64-bit pair of 32-bit base address
                * registers.
                */
                bAddr64 = EC_TRUE;
                break; /* OK */
             }
             case PCI_BAR_MEM_RESERVED:
             default:
             {
                continue;
             }
          }

          /* Re-adjust dwBaseAddress if upper 16-bits are 0 (this is allowable in PCI 2.2 spec) */
          if (((dwBaseAddress & PCI_MEMBASE_MASK) != 0) && ((dwBaseAddress & 0xFFFF0000) == 0))
          {
             dwBaseAddress |= 0xFFFF0000;
          }

          dwSize = ~(dwBaseAddress & PCI_MEMBASE_MASK);

          if ((dwBaseAddress != 0) && (dwBaseAddress != 0xFFFFFFFF) && (((dwSize + 1) & dwSize) == 0))
          {
             /* BAR has valid format (consecutive high 1's and consecutive low 0's) */
             pInfo->Window.memWindows[pInfo->Window.dwNumMemWindows].dwLen    = dwSize + 1;
             pInfo->Window.memWindows[pInfo->Window.dwNumMemWindows++].dwBase = dwReg & PCI_MEMBASE_MASK;
          }
          else
          {
             /* BAR invalid => skip to next one */
          }
       }
   }
#endif /* ifdef ECLINKOS_USE_OS_PCICONFIGURE */

   /* no errors */
   bRetVal = EC_TRUE;

Exit:

   LinkOsPciCleanup();

   return bRetVal;
}


EC_T_BOOL LinkOsGetPciInfo(T_PCIINFO* pInfo, const EC_T_CHAR*  szDeviceName, EC_T_DWORD dwUnit)
{
   EC_T_BOOL       bRes = EC_FALSE;
   EC_T_DWORD      i;
   E_PCI_SCAN_ALGO eScanAlgo;

#ifdef ECLINKOS_USE_OS_PCISCANFUNC
   eScanAlgo = ePCI_SCANALGO_OS;
#else
   eScanAlgo = ePCI_SCANALGO_BUSORDER;
#endif
   
   for (i = 0; (i < MAPPING_CNT) && !bRes; ++i)
   {
      if (LinkOsStrcmp(S_oCardMappig[i].sIdentStr, szDeviceName) == 0)
      {
         bRes = LinkOsGetPciInfoInternal(pInfo, dwUnit, S_oCardMappig[i].pIdentAry, eScanAlgo);
      }
   }

   return bRes;
}

#endif /* UNDER_CE */

/*-END OF SOURCE FILE--------------------------------------------------------*/
