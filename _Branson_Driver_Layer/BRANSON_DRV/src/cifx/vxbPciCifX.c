/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: vxbPciCifX.c 7069 2015-07-21 12:23:45Z LuisContreras $:

  Description:
    VxWorks PCI component of the CIFX device driver

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2017-10-12  FL, fixed build error
    2015-06-08  initial version

**************************************************************************************/

/*****************************************************************************/
/*! \file vxbPciCifX.c
*    PCI handling functions for CIFX device driver                           */
/*****************************************************************************/

/* includes */
#include <vxWorks.h>
#include <intLib.h>
#include <errnoLib.h>
#include <errno.h>
#include <sioLib.h>
#include <ioLib.h>
#include <stdio.h>
#include <string.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbPciLib.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/timer/vxbTimerLib.h>

/* Hilscher specific includes */
#include "cifXVxWorks_internal.h"

/* External definitions */
extern uint32_t g_ulDeviceCount;                /*!< Number of devices handled by toolkit */

IMPORT STATUS createCifXDevice( char * name, VXB_DEV_ID pDev, VXW_CIFXDRV_DEVICEENTRY_T * ptDeviceInfo);

/* Local functions forward declaration */
LOCAL STATUS pciCifXProbe     (VXB_DEV_ID pDev);
LOCAL STATUS pciCifXAttach    (VXB_DEV_ID pDev);
LOCAL STATUS pciCifXShutdown  (VXB_DEV_ID pDev);
LOCAL STATUS pciCifXDetach    (VXB_DEV_ID pDev);

LOCAL VXB_DRV_METHOD pciCifxMethodList[] =
{
  /* DEVICE API */
  { VXB_DEVMETHOD_CALL(vxbDevProbe),    pciCifXProbe },
  { VXB_DEVMETHOD_CALL(vxbDevAttach),   pciCifXAttach },
  { VXB_DEVMETHOD_CALL(vxbDevShutdown), pciCifXShutdown },
  { VXB_DEVMETHOD_CALL(vxbDevDetach),   pciCifXDetach },
  { 0, NULL }
};

/* CIFX PCI driver */
VXB_DRV vxbPciCifxDrv =
{
  { NULL } ,
  "CIFX",                       /* Name */
  "CIFX PCI driver",            /* Description */
  VXB_BUSID_PCI,                /* Class */
  0,                            /* Flags */
  0,                            /* Reference count */
  pciCifxMethodList             /* Method table */
};

typedef struct CIFX_PCI_SCAN_INFOtag
{
  char              szName[CIFx_MAX_INFO_NAME_LENTH]; /*!< Our device name */
  PCI_HEADER_DEVICE tPciHeader;                       /*!< PCI device header information */
} CIFX_PCI_SCAN_INFO;

LOCAL CIFX_PCI_SCAN_INFO tcifXPciInfo = {{0}};

/*****************************************************************************/
/*! List of PCI devices to search for                                        */
/*****************************************************************************/
static struct PCI_DEVICE_SCANLIST_Ttag
{
  int iVendorID;  /*!< Vendor ID for scanning */
  int iDeviceID;  /*!< Device ID for scanning */
  int iSubVendor; /*!< SubSystem ID for scanning */
  int iSubSystem; /*!< SubSystem ID for scanning */
  int iDPMBar;    /*!< BAR that contains the DPM */
} s_atPciScanList[] =
{
  /* CIFX cards */
  { HILSCHER_PCI_VENDOR_ID,           /* Vendor ID */
    CIFX50_PCI_DEVICE_ID,             /* Device ID */
    0,                                /* Subvendor ID */
    CIFX50_PCI_SUBYSTEM_ID,           /* Subsystem ID */
  },
  /* NETPLC100C-FB and NETPLC100C-RE cards */
  { HILSCHER_PCI_VENDOR_ID,           /* Vendor ID */
    NETPLC100C_PCI_DEVICE_ID,         /* Device ID */
    0,                                /* Subvendor ID */
    NETPLC100C_PCI_SUBYSTEM_ID_RAM,   /* Subsystem ID */
  },
  /* NETPLC100C-FB and NETPLC100C-RE cards */
  { HILSCHER_PCI_VENDOR_ID,           /* Vendor ID */
    NETPLC100C_PCI_DEVICE_ID,         /* Device ID */
    0,                                /* Subvendor ID */
    NETPLC100C_PCI_SUBYSTEM_ID_FLASH, /* Subsystem ID */
  },
  /* netJACK100 cards */
  { HILSCHER_PCI_VENDOR_ID,           /* Vendor ID */
    NETJACK100_PCI_DEVICE_ID,         /* Device ID */
    0,                                /* Subvendor ID */
    NETJACK100_PCI_SUBYSTEM_ID_RAM,   /* Subsystem ID */
  },
  /* netJACK100 cards */
  { HILSCHER_PCI_VENDOR_ID,           /* Vendor ID */
    NETJACK100_PCI_DEVICE_ID,         /* Device ID */
    0,                                /* Subvendor ID */
     NETJACK100_PCI_SUBYSTEM_ID_FLASH,/* Subsystem ID */
  },
  /* NXPCA-PCI Boards */
  { PLX_PCI_VENDOR_ID,                /* Vendor ID */
    PCI9030_DEVICE_ID,                /* Device ID */
    PLX_PCI_VENDOR_ID,                /* Subvendor ID */
    NXPCAPCI_REV2_SUBSYS_ID,          /* Subsystem ID */
  },
  { PLX_PCI_VENDOR_ID,                /* Vendor ID */
    PCI9030_DEVICE_ID,                /* Device ID */
    PLX_PCI_VENDOR_ID,                /* Subvendor ID */
    NXPCAPCI_REV3_SUBSYS_ID,          /* Subsystem ID */
  }
};

/* VXB_DRV_DEF(vxbPciCifxDrv) */

/******************************************************************************
*
* pciCifXProbe - probe the PCI bus about compatible PCI device
*
* This routine probe the pciBus for compatible PCI device by checking the vendor
* and device ID.
*
* RETURNS: OK if probe passes and assumed a valid CIFX
* (or compatible) device.  ERROR otherwise.
*
*/
LOCAL STATUS pciCifXProbe
    (
    VXB_DEV_ID  pDev
    )
{
  STATUS    sRet = ERROR;
  int       iIdx = 0;
  uint32_t  vendorId = 0;
  uint32_t  deviceId = 0;
  uint32_t  subVendorId = 0;
  uint32_t  subSystemId = 0;

  /* Read the PCI info structure */
  VXB_PCI_BUS_CFG_READ (pDev, PCI_CFG_VENDOR_ID,      2, vendorId);
  VXB_PCI_BUS_CFG_READ (pDev, PCI_CFG_DEVICE_ID,      2, deviceId);
  VXB_PCI_BUS_CFG_READ (pDev, PCI_CFG_SUB_VENDER_ID,  2, subVendorId);
  VXB_PCI_BUS_CFG_READ (pDev, PCI_CFG_SUB_SYSTEM_ID,  2, subSystemId);

  /* Scan for al defined devices */
  for ( iIdx = 0; iIdx < sizeof(s_atPciScanList) / sizeof(s_atPciScanList[0]); ++iIdx)
  {
    if( (s_atPciScanList[iIdx].iVendorID  == vendorId)    &&
        (s_atPciScanList[iIdx].iDeviceID  == deviceId)    &&
        (s_atPciScanList[iIdx].iSubVendor == subVendorId) &&
        (s_atPciScanList[iIdx].iSubSystem == subSystemId) )
    {
      UINT32 ulOffset   = 0;
      char * pbData   = (char*)&tcifXPciInfo.tPciHeader;

      /* We found one of our devices */
      sRet = OK;

      /* Read the complete PCI_HEADER_DEVICE structure */
      for ( ulOffset = 0; ulOffset < sizeof (PCI_HEADER_DEVICE); ulOffset += (UINT32)sizeof(uint16_t))
      {
        VXB_PCI_BUS_CFG_READ (pDev, ulOffset,  sizeof(uint16_t), pbData[ulOffset]);
      }

      /* Create a accessible VxWorks device */
      (void) snprintf (tcifXPciInfo.szName, CIFx_MAX_INFO_NAME_LENTH, "%s%d", "cifX", g_ulDeviceCount);
      vxbDevNameSet( pDev, tcifXPciInfo.szName, FALSE);

      vxbDevDrvDataSet(pDev, (void*)&tcifXPciInfo);
      break;
    }
  }

  return sRet;
}

/******************************************************************************
*
* pciCifXAttach - attach for device presence at specific address
*
* This is the pciCifX initialization routine.
*
* RETURNS: OK or ERROR when initialize failed
*
* ERRNO
*/
LOCAL STATUS pciCifXAttach
    (
    VXB_DEV_ID pDev /* Device information */
    )
{
  VXW_CIFXDRV_DEVICEENTRY_T * ptDeviceEntry = NULL;
  CIFX_PCI_SCAN_INFO *        ptCifXPciCfg  = NULL;
  STATUS                      sRet          = OK;

  ptCifXPciCfg = (CIFX_PCI_SCAN_INFO *)vxbDevDrvDataGet(pDev);

  /* Create a Device entry structure */
  if (NULL == (ptDeviceEntry = (VXW_CIFXDRV_DEVICEENTRY_T*)vxbMemAlloc(sizeof(VXW_CIFXDRV_DEVICEENTRY_T))))
  {
    sRet = ERROR;

  } else
  {
    /* Check for CIFX or PLX based device */
    if( PCI9030_DEVICE_ID == ptCifXPciCfg->tPciHeader.deviceId)
    {
      /* This is a PLX device, we need MEM BAR 2 which is the DPM */
      if( NULL == (ptDeviceEntry->tCifXPciData.pResDPM = vxbResourceAlloc(pDev, VXB_RES_MEMORY, PCI_DEV_BAR2_INDEX)))
        sRet = ERROR;

      /* We also need BAR 1 holding he memory mapped I/O PCI registers */
      if( NULL == (ptDeviceEntry->tCifXPciData.pResPLXRegDPM = vxbResourceAlloc(pDev, VXB_RES_MEMORY, PCI_DEV_BAR1_INDEX)))
        sRet = ERROR;

      /* Allocate the interrupt resource */
      if (NULL == (ptDeviceEntry->tCifXPciData.pResIRQ = vxbResourceAlloc(pDev, VXB_RES_IRQ, 0)))
        sRet = ERROR;

    } else
    {
      /* This is a standard CIFX, MEM BAR 0 is the DPM */
      if( NULL == (ptDeviceEntry->tCifXPciData.pResDPM = vxbResourceAlloc(pDev, VXB_RES_MEMORY, PCI_DEV_BAR0_INDEX)))
        sRet = ERROR;

        /* Allocate the interrupt resource */
      if (NULL == (ptDeviceEntry->tCifXPciData.pResIRQ = vxbResourceAlloc(pDev, VXB_RES_IRQ, 0)))
        sRet = ERROR;

      /* EXTMEM not always available, just try to allocate the resource */
      ptDeviceEntry->tCifXPciData.pResEXTMEM = vxbResourceAlloc(pDev, VXB_RES_MEMORY, PCI_DEV_BAR1_INDEX);
    }
  }

  if ( OK == sRet)
  {
    /* Insert Device information */
    ptDeviceEntry->pDev               = pDev;
    ptDeviceEntry->ulPhysicalAddress  = (uint64_t*)((VXB_RESOURCE_ADR *)(ptDeviceEntry->tCifXPciData.pResDPM->pRes))->start;                 /* Physical DPM address     */
    ptDeviceEntry->pvDPMAddress       = (void*)((VXB_RESOURCE_ADR *)(ptDeviceEntry->tCifXPciData.pResDPM->pRes))->virtAddr;       /* Virtual pointer to DPM   */
    ptDeviceEntry->ulDPMSize          = ((VXB_RESOURCE_ADR *)(ptDeviceEntry->tCifXPciData.pResDPM->pRes))->size;                  /* Size of the DPM in bytes */
    ptDeviceEntry->bIrqNumber         = (unsigned char)((VXB_RESOURCE_IRQ *)(ptDeviceEntry->tCifXPciData.pResIRQ->pRes))->lVec;   /* Interrupt number         */

    ptDeviceEntry->fPCICard           = TRUE;                                                           /* TRUE if device is a PCI Card */

    /* Create a new CIFX device */
    sRet = createCifXDevice(  ptCifXPciCfg->szName, pDev, ptDeviceEntry);
  }

  if ( OK != sRet)
  {
    /* Free all Resources */
    if(NULL != ptDeviceEntry->tCifXPciData.pResDPM )
      vxbResourceFree( pDev, ptDeviceEntry->tCifXPciData.pResDPM);

    if(NULL != ptDeviceEntry->tCifXPciData.pResIRQ )
      vxbResourceFree( pDev, ptDeviceEntry->tCifXPciData.pResIRQ);

    if( NULL != ptDeviceEntry->tCifXPciData.pResPLXRegDPM)
      vxbResourceFree( pDev, ptDeviceEntry->tCifXPciData.pResPLXRegDPM);

    if(NULL != ptDeviceEntry->tCifXPciData.pResEXTMEM )
      vxbResourceFree( pDev, ptDeviceEntry->tCifXPciData.pResEXTMEM);
  }

  if(NULL != ptDeviceEntry)
    vxbMemFree(ptDeviceEntry);

  return sRet;
}

/******************************************************************************
*
* pciCifxDetach - detach a CIFX device
*
* This routine detach the CIFX driver from the device
*
* RETURNS: OK or ERROR when detach failed
*
* ERRNO
*/
STATUS pciCifXDetach
    (
    struct vxbDev * pDev
    )
{
  CIFX_DEV_RESOURCE_DATA * ptCifXdata;

  ptCifXdata = (CIFX_DEV_RESOURCE_DATA *)vxbDevSoftcGet(pDev);

  if ( NULL != ptCifXdata )
  {
    /* Free all Resources */
    if(NULL != ptCifXdata->pResDPM )
      vxbResourceFree( pDev, ptCifXdata->pResDPM);

    if(NULL != ptCifXdata->pResIRQ )
      vxbResourceFree( pDev, ptCifXdata->pResIRQ);

    if( NULL != ptCifXdata->pResPLXRegDPM)
      vxbResourceFree( pDev, ptCifXdata->pResPLXRegDPM);

    if(NULL != ptCifXdata->pResEXTMEM )
      vxbResourceFree( pDev, ptCifXdata->pResEXTMEM);
  }

 return OK;
}

/******************************************************************************
*
* pciCifxShutdown - shutdown CIFX device
*
* This routine shuts down the CIFX device
*
* RETURNS: OK or ERROR when shutdown failed
*
* ERRNO
*/
STATUS pciCifXShutdown
    (
    struct vxbDev * pDev
    )
{
  return OK;
}

