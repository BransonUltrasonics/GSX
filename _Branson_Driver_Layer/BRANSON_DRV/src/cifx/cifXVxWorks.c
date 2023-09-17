/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: cifXVxWorks.c 7069 2015-07-21 12:23:45Z LuisContreras $:

  Description:
    VxWorks driver initialization and handling functions

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2015-06-08  created

**************************************************************************************/

/*****************************************************************************/
/*! \file cifXVxWorks.c
*    VxWorks OS Abstraction Layer implementation                             */
/*****************************************************************************/

#include "cifXVxWorks_internal.h"
#include "OS_Includes.h"

extern uint32_t           g_ulDeviceCount;              /*!< Number of devices handled by toolkit */
extern PDEVICEINSTANCE*   g_pptDevices;                 /*!< Array of device informations         */
extern void*              g_pvTkitLock;                 /*!< Toolkit lock */
extern char*              g_szDriverBaseDir;            /*!< Device base directory */
extern BOOL               g_fSingleDir;                 /*!< Device base directory */

static TASK_ID            s_iCOSThread        = 0;
static int                s_fCOSThreadRunning = 0;
static uint32_t           s_ulPollinterval = CIFX_DRIVER_DEFAULT_POLL_INTERVAL;

/*****************************************************************************/
/*! Thread for cycle Toolkit timer (used for checking COS bits) on
 *  polling cards
 *   \return 0                                                               */
/*****************************************************************************/
LOCAL int cifXCOSCheck(void)
{
  while(s_fCOSThreadRunning)
  {
    cifXTKitCyclicTimer();
    OS_Sleep(s_ulPollinterval);
  }

  return 0;
}

/*****************************************************************************/
/*! cifXToolkit callback function to adjust timings at different stages
 *   \param pvDeviceInstance Deviceinstance pointer for the device
 *   \param eEvent           Occured event                                   */
/*****************************************************************************/
LOCAL void cifXToolkitNotifyCallback(void* pvDeviceInstance, CIFX_TOOLKIT_NOTIFY_E eEvent)
{
  DEVICEINSTANCE* ptDevInst = (DEVICEINSTANCE*)pvDeviceInstance;

  switch(eEvent)
  {
  case eCIFX_TOOLKIT_EVENT_PRERESET:
    break;

  case eCIFX_TOOLKIT_EVENT_POSTRESET:
    break;

  case eCIFX_TOOLKIT_EVENT_PRE_BOOTLOADER:
    if(eCHIP_TYPE_NETX51 == ptDevInst->eChipType)
    {
      /* netX51 has invalid timing configuration, so we need to adjust it here, to be able to access
         memory starting at offset 0x100 */
      NETX51_DPM_CONFIG_AREA_T* ptDpmConfig = (NETX51_DPM_CONFIG_AREA_T*)ptDevInst->pbDPM;

      ptDpmConfig->ulDpmRdyCfg    = MSK_NX56_dpm_rdy_cfg_rdy_pol |           /* DPM is ready when external RDY-signal is high. */
                                   (1 << SRT_NX56_dpm_rdy_cfg_rdy_drv_mode); /* Push-Pull Mode */
      ptDpmConfig->ulDpmTimingCfg = 0; /* Disable setup times and filter. */

      if(g_ulTraceLevel & TRACE_LEVEL_DEBUG)
      {
        USER_Trace(pvDeviceInstance, TRACE_LEVEL_DEBUG, "Adjusting netX51/52 DPM Timings before starting 2nd Stage Loader");
      }
    }
    break;

  case eCIFX_TOOLKIT_EVENT_POST_BOOTLOADER:
    if(eCHIP_TYPE_NETX51 == ptDevInst->eChipType)
    {
      PVXW_CIFXDRV_DEVINSTANCE_T ptDevExt = (PVXW_CIFXDRV_DEVINSTANCE_T)ptDevInst->pvOSDependent;

      /* Some NXHX51/52 Boards don't have the WIF pin connected correctly an always startup in 16 Bit mode.
         We will try to detect it here, and update the DPM configuration accordingly */
      switch(ptDevExt->tPciInfo.ulPLXMode)
      {
        case PLX_MODE_8BIT:
          {
            /* PLX requests 8 Bit, so we need to check if we are able to access the DPM correctly, if not, we will update the DPM configuration */
            NETX51_DPM_CONFIG_AREA_T* ptDpmConfig  = (NETX51_DPM_CONFIG_AREA_T*)(ptDevInst->pbDPM + ptDevInst->ulDPMSize - sizeof(*ptDpmConfig));
            uint32_t                  ulCookie;

            OS_Memcpy(&ulCookie, ptDevInst->pbDPM, sizeof(ulCookie));

            if(ulCookie != CIFX_DPMSIGNATURE_BSL_VAL)
            {
              if(g_ulTraceLevel & TRACE_LEVEL_INFO)
              {
                USER_Trace(pvDeviceInstance,
                           TRACE_LEVEL_INFO,
                           "Unable to read cookie from netX51/52. Setting DPM to 8 bit. This might be caused by a NXHX Board that does not have the SIRQ/WIF pin connected correctly (Read: 0x%08X)",
                           ulCookie);
              }

              ptDpmConfig->ulDpmCfg0x0 = 0;
            }
          }
          break;

        case PLX_MODE_16BIT:
        case PLX_MODE_32BIT:
        case PLX_MODE_INVALID:
        default:
          break;
      }
    }
    break;

  default:
    break;
  }
}

#ifdef CIFX_TOOLKIT_DMA
/*****************************************************************************/
/*! Allocate cache-safe buffer for DMA transfers
 *   \param ptDevInstance  Pointer to device instance
 *   \param ptDMABuffer    Pointer to DMA Buffer Structure
 *   \param ulCount        Number of DMA Buffer
 *   \param ulBufferSize   Number of bytes to allocate
 *   \return TRUE on success                                                 */
/*****************************************************************************/
LOCAL BOOL cifXAllocDMABuffer(PVXW_CIFXDRV_DEVINSTANCE_T ptDevInstance, PCIFX_DMABUFFER_T ptDMABuffer, uint32_t ulCount, uint32_t ulBufferSize)
{
  int32_t  fRet      = TRUE;
  void*    pvBuf     = NULL;
  void*    pVirtAddr = NULL;

  if (NULL == ptDMABuffer)
    return FALSE;

  if ( (!CACHE_DMA_IS_WRITE_COHERENT ()) ||
       (!CACHE_DMA_IS_READ_COHERENT ())     )
  {
    if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
    {
      USER_Trace(NULL,
                 TRACE_LEVEL_ERROR,
                 "DMA transfers requires cache coherent memory!");
    }
    return FALSE;
  }

  pvBuf = vxbDmaBufMemAlloc(ptDevInstance->tDeviceEntry.pDev,
                            ptDevInstance->ptVxbDmaTagId,
                            &pVirtAddr,
                            VXB_DMABUF_MEMALLOC_CLEAR_BUF,
                            &(ptDevInstance->ptVxbDmaMapId[ulCount]));

  if (NULL == pvBuf)
  {
    if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
    {
      USER_Trace(NULL,
                 TRACE_LEVEL_ERROR,
                 "Failed allocating cache-safe buffer for DMA transfers!");
    }
    fRet = FALSE;

  } else
  {
    if(ERROR == vxbDmaBufMapLoad( ptDevInstance->tDeviceEntry.pDev,
                                  ptDevInstance->ptVxbDmaTagId,
                                  ptDevInstance->ptVxbDmaMapId[ulCount],
                                  pvBuf,
                                  ulBufferSize,
                                  0))
    {

      if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
      {
        USER_Trace(NULL, TRACE_LEVEL_ERROR, "Failed to load DMA map.");
      }
      fRet = FALSE;

    } else
    {

      /* Get 256 byte aligned mem pointer */
      uint32_t* pulPhys;
      void* pvAligned = (void*)(((uint64_t)pvBuf + (CIFX_DMA_MODULO_SIZE-1)) & (~(CIFX_DMA_MODULO_SIZE-1)));

      ptDMABuffer->pvBuffer          = pvAligned;
      pulPhys = (uint32_t*)(CACHE_DMA_VIRT_TO_PHYS (pvAligned));
      ptDMABuffer->ulPhysicalAddress = *((uint32_t*)&pulPhys);
      ptDMABuffer->ulSize            = ulBufferSize;
      /* Store original mem pointer for cifXFreeDMABuffer() routine */
      ptDMABuffer->pvUser            = pvBuf;
    }
  }

  return fRet;
}

/*****************************************************************************/
/*! Free the buffer acquired with cifXAllocDMABuffer()
 *   \param ptDevInstance  Pointer to device instance
 *   \param ulCount        DMA Buffer number
 *   \param ptDMABuffer    Pointer to DMA Buffer Structure                   */
/*****************************************************************************/
LOCAL void cifXFreeDMABuffer(PVXW_CIFXDRV_DEVINSTANCE_T ptDevInstance, uint32_t ulCount, PCIFX_DMABUFFER_T ptDMABuffer)
{
  STATUS sRet = OK;
  if (NULL == ptDMABuffer)
    return;

  sRet = vxbDmaBufMemFree( ptDevInstance->ptVxbDmaTagId,
                          ptDMABuffer->pvUser,
                          ptDevInstance->ptVxbDmaMapId[ulCount]);
  if(OK != sRet)
  {
    if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
    {
      USER_Trace(&ptDevInstance->tDeviceInst, TRACE_LEVEL_ERROR, "Error on freeing DMA buffer.");
    }
  }

  ptDMABuffer->pvBuffer          = NULL;
  ptDMABuffer->ulPhysicalAddress = 0;
  ptDMABuffer->ulSize            = 0;
  ptDMABuffer->pvUser            = NULL;
}
#endif /* CIFX_TOOLKIT_DMA */

/*****************************************************************************/
/*! Add a PCI device to toolkit control
 *  \param ptDevEntry     Device to add
 *  \param pszName        Device name created by the PCI functions
 *  \param ptDevInstance  Driver device instance
 *  \return OK on success                                                    */
/*****************************************************************************/
STATUS cifXAddPCIDevice ( VXW_CIFXDRV_DEVICEENTRY_T* ptDevEntry, char* pszName, PVXW_CIFXDRV_DEVINSTANCE_T * ptDevInstance)
{
  PVXW_CIFXDRV_DEVINSTANCE_T ptDevInst  = (PVXW_CIFXDRV_DEVINSTANCE_T)OS_Memalloc(sizeof(VXW_CIFXDRV_DEVINSTANCE_T));
  STATUS                     sRet       = OK;

  if((NULL == ptDevEntry) || (NULL == ptDevInst))
    return ERROR;

  /* Initialize the Device Instance structure */
  OS_Memset(ptDevInst, 0, sizeof(*ptDevInst));

  /* Store our information for later */
  OS_Memcpy( &ptDevInst->tDeviceEntry, ptDevEntry, sizeof(*ptDevEntry));
  ptDevInst->tPciInfo.fPLXBridge           = FALSE;
  ptDevInst->tDeviceInst.ulPhysicalAddress = (uint32_t)ptDevEntry->ulPhysicalAddress;
  ptDevInst->tDeviceInst.pbDPM             = (uint8_t*)ptDevEntry->pvDPMAddress;
  ptDevInst->tDeviceInst.ulDPMSize         = (uint32_t)ptDevEntry->ulDPMSize;
  ptDevInst->tDeviceInst.bIrqNumber        = ptDevEntry->bIrqNumber;
  ptDevInst->tDeviceInst.fPCICard          = TRUE;
  ptDevInst->tDeviceInst.eDeviceType       = eCIFX_DEVICE_AUTODETECT;

  ptDevInst->tDeviceInst.pvOSDependent     = (void*)ptDevInst;

  /* Add a toolkit notify function callback */
  ptDevInst->tDeviceInst.pfnNotify= cifXToolkitNotifyCallback;


  /* Proceed with device specific settings */
  if (HILSCHER_PCI_VENDOR_ID == (unsigned short)ptDevEntry->vendorId)
  {
    if ( ((NETPLC100C_PCI_DEVICE_ID == (unsigned short)ptDevEntry->deviceId) && (NETPLC100C_PCI_SUBYSTEM_ID_FLASH == (unsigned short)ptDevEntry->subSystemId)) ||
         ((NETJACK100_PCI_DEVICE_ID == (unsigned short)ptDevEntry->deviceId) && (NETJACK100_PCI_SUBYSTEM_ID_FLASH == (unsigned short)ptDevEntry->subSystemId)) )
    {
      /* This is a flash based netPLC/netJACK, so set device type to flash based */
      ptDevInst->tDeviceInst.eDeviceType = eCIFX_DEVICE_FLASH_BASED;
    }
  }

#ifdef CIFX_TOOLKIT_DMA
  vxbDmaBufInit();

  /* Create a DMA Buf Tag needed for DMA allocation */
  ptDevInst->ptVxbDmaTagId = vxbDmaBufTagCreate(ptDevEntry->pDev,               /* VxWorks device instance */
                                                 NULL,                          /* parent */
                                                 CIFX_DMA_MODULO_SIZE,          /* alignment */
                                                 0,                             /* boundary */
                                                 0XFFFFFFFF,                    /* lowaddr */
                                                 0XFFFFFFFF,                    /* highaddr */
                                                 NULL,                          /* filter - unused */
                                                 NULL,                          /* filterarg - unused */
                                                 /* Memory address must be aligned to 256 byte (CIFX_DMA_MODULO_SIZE).
                                                  * In worst case we have to allocate additional 256 bytes            */
                                                 CIFX_DEFAULT_DMA_BUFFER_SIZE + CIFX_DMA_MODULO_SIZE, /* maxsize */
                                                 1,                             /* nSegments */
                                                 CIFX_DEFAULT_DMA_BUFFER_SIZE,  /* max seg size */
                                                 VXB_DMABUF_ALLOCNOW | VXB_DMABUF_NOCACHE,/* flags */
                                                 NULL,                          /* lockfunc - unused */
                                                 NULL,                          /* lockarg - unused */
                                                 NULL                           /* ppDmaTag - unused */
                                                 );

  if(NULL == ptDevInst->ptVxbDmaTagId)
  {
    if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
    {
      USER_Trace(&ptDevInst->tDeviceInst, TRACE_LEVEL_ERROR, "Error creating DMA tag.");
    }

  } else
  {
    int iIdx = 0;
    ptDevInst->tDeviceInst.ulDMABufferCount  = 0;

    /* Allocate cache-safe memory for DMA transfers */
    for(iIdx = 0; iIdx < CIFX_DMA_BUFFER_COUNT; iIdx++)
    {
      if (TRUE == cifXAllocDMABuffer(ptDevInst, &ptDevInst->tDeviceInst.atDmaBuffers[iIdx], iIdx, CIFX_DEFAULT_DMA_BUFFER_SIZE))
      {
        ptDevInst->tDeviceInst.ulDMABufferCount++;
      } else
      {
        if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
        {
          USER_Trace(&ptDevInst->tDeviceInst, TRACE_LEVEL_ERROR, "Failed allocating cache-safe buffer for DMA transfers!");
        }
        break;
      }
    }
  }
#endif /* CIFX_TOOLKIT_DMA */

  /* Create a device name for the toolkit ("CIFXn") */
  (void) snprintf (&ptDevInst->tDeviceInst.szName[0], CIFx_MAX_INFO_NAME_LENTH, "%s", pszName);

  if(CIFX_NO_ERROR != cifXTKitAddDevice(&ptDevInst->tDeviceInst))
  {
    /* Error adding device */
#ifdef CIFX_TOOLKIT_DMA
    uint32_t ulIdx = 0;
    for(ulIdx=0; ulIdx<ptDevInst->tDeviceInst.ulDMABufferCount; ulIdx++)
    {
      cifXFreeDMABuffer(ptDevInst, (uint32_t)ulIdx, &ptDevInst->tDeviceInst.atDmaBuffers[ulIdx]);
    }

    if(ERROR == vxbDmaBufTagDestroy(ptDevInst->ptVxbDmaTagId))
    {
      if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
      {
        USER_Trace(&ptDevInst->tDeviceInst, TRACE_LEVEL_ERROR, "Error on cleaning up DMA Tag.");
      }
    }
#endif /* CIFX_TOOLKIT_DMA */

    if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
    {
      USER_Trace(&ptDevInst->tDeviceInst, TRACE_LEVEL_ERROR, "Error adding PCI cifX device @ Phys. Addr 0x%lX.", ptDevInst->tDeviceInst.ulPhysicalAddress);
    }

    OS_Memfree(ptDevInst);
    ptDevInst = NULL;

    *ptDevInstance = NULL;

    sRet = ERROR;

  } else
  {
    /* Start thread for COS handling if the first device is added to the toolkit */
    if ( 1 == g_ulDeviceCount)
    {
      s_fCOSThreadRunning = 1;
      s_iCOSThread = taskSpawn("cifXCOSCheck",     /* name of new task */
                               100,                /* priority of new task */
                               0,                  /* task option word */
                               0x1000,             /* size (bytes) of stack needed plus name */
                               cifXCOSCheck,       /* entry point of new task */
                               0, 0, 0,  0,  0,  0, 0, 0, 0, 0);  /* 10 req'd args to pass to entryPt */
    }

    /* Return the pointer to the new device instance */
    *ptDevInstance = ptDevInst;
  }

  return sRet;
}

/*****************************************************************************/
/*! Initialize/Start driver
 *   \param fSingleDir      Use a single firmware directory
 *   \param ulTraceLevel    Trace level at startup
 *   \param ulPollInterval  Intervall to poll for COS Flag changes
 *   \param szDriverBaseDir Base device directory to search for settings/configuration/firmware files
 *   \return CIFX_NO_ERROR on success                                        */
/*****************************************************************************/
int32_t cifXInitializeDriver ( BOOL fSingleDir, unsigned long ulTraceLevel, unsigned long ulPollInterval, char* pszDriverBaseDir)
{
  int32_t lRet = CIFX_NO_ERROR;

  g_ulTraceLevel    = (uint32_t)ulTraceLevel;
  g_fSingleDir      = fSingleDir;

  /* If ulPollInterval == 0 use default (500ms) */
  if (ulPollInterval > 0)
  {
    s_ulPollinterval  = (uint32_t)ulPollInterval;
  }

  if (pszDriverBaseDir)
  {
    uint32_t ulDriverBaseDirLen = OS_Strlen(pszDriverBaseDir) + 1;
    g_szDriverBaseDir = OS_Memalloc(ulDriverBaseDirLen);
    OS_Strncpy(g_szDriverBaseDir, pszDriverBaseDir, ulDriverBaseDirLen);
  } else
  {
    uint32_t ulDriverBaseDirLen = OS_Strlen(CIFX_DRIVER_DEFAULT_BASEDIR) + 1;
    g_szDriverBaseDir = OS_Memalloc(ulDriverBaseDirLen);
    OS_Strncpy(g_szDriverBaseDir, CIFX_DRIVER_DEFAULT_BASEDIR, ulDriverBaseDirLen);
  }

  if(CIFX_NO_ERROR != (lRet = cifXTKitInit()))
  {
    OS_Memfree(g_szDriverBaseDir);
    g_szDriverBaseDir = NULL;
    cifXTKitDeinit();
    if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
    {
      USER_Trace(NULL,
                 TRACE_LEVEL_ERROR,
                 "No cifX device available");
    }

    lRet = CIFX_DRV_INIT_ERROR;
  }

  return lRet;
}

/*****************************************************************************/
/*! De-initialize the CIFX driver
 *   \return CIFX_NO_ERROR on success                                        */
/*****************************************************************************/
int32_t cifXDeinitializeDriver(void)
{
  int32_t  lRet          = CIFX_NO_ERROR;

  /* Stop thread for COS handling */
  if(s_iCOSThread != 0)
  {
    s_fCOSThreadRunning = FALSE;

    do
    {
      OS_Sleep(s_ulPollinterval);
    } while (OK == taskIdVerify(s_iCOSThread));

    s_iCOSThread = 0;
  }

  OS_EnterLock(g_pvTkitLock);

  /* Delete all our device data which is stored in the toolkit */
  while (g_ulDeviceCount > 0)
  {
    PVXW_CIFXDRV_DEVINSTANCE_T ptDev = (PVXW_CIFXDRV_DEVINSTANCE_T)g_pptDevices[0]->pvOSDependent;

    if (CIFX_NO_ERROR == (lRet = cifXTKitRemoveDevice(ptDev->tDeviceInst.szName, 1)))
    {
#ifdef CIFX_TOOLKIT_DMA
      int iIdx = 0;
      for(iIdx = 0; iIdx < ptDev->tDeviceInst.ulDMABufferCount; iIdx++)
      {
        cifXFreeDMABuffer(ptDev, (uint32_t)iIdx, &ptDev->tDeviceInst.atDmaBuffers[iIdx]);
      }

      if(ERROR == vxbDmaBufTagDestroy(ptDev->ptVxbDmaTagId))
      {
        if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
        {
          USER_Trace(&ptDev->tDeviceInst, TRACE_LEVEL_ERROR, "Error on cleaning up DMA Tag.");
        }
      }
#endif /* CIFX_TOOLKIT_DMA */
      OS_Memfree(ptDev);

    } else
    {
      if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
      {
        USER_Trace( &ptDev->tDeviceInst, TRACE_LEVEL_DEBUG, "Failed to remove device from toolkit: %s", ptDev->tDeviceInst.szName);
      }
      break;
    }
  }

  OS_LeaveLock(g_pvTkitLock);

  if (CIFX_NO_ERROR == lRet)
  {
    OS_Memfree(g_szDriverBaseDir);
    g_szDriverBaseDir = NULL;

    cifXTKitDeinit();
  }

  return lRet;
}

