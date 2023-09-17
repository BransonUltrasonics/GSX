/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: vxbCifXVxWorks.c 7069 2015-07-21 12:23:45Z LuisContreras $:

  Description:
    VxWorks PCI driver registration functions

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2015-06-08  created

**************************************************************************************/

/*****************************************************************************/
/*! \file vxbCifXVxWorks.c
*    VxWorks PCI driver registration functions                               */
/*****************************************************************************/
/*
DESCRIPTION
The logical layers of a typical system using a CIFX appear:

/cs
         +--------------------+
         | Application module |
         +--------------------+ <-- read(), write(), ioctl(), close()
                   |
         +--------------------+
         | VxWorks I/O System |
         +--------------------+ <-- IOS layer iosRead, iosWrite, ioctl...(vxbCifXVxWorksDrv.c)
                   |
+--------------------------------------+
| Diff interface CIFX driver module    |
+--------------------------------------+

/cs

This driver allows the I/O system to access a CIIFX directly as an I/O device.

vxbCifXVxWorksDrv is used to simply provide a high-level method
of accessing a CIFX device through I/O calls.

USER-CALLABLE ROUTINES
Most of the routines in this driver are accessible only through the
I/O system. However, the following two routines can be called directly:
cifXDriverOpen() to initialize the driver, and cifXDriverClose("name") to delete
devices.
*/

/* includes */

#include <vxWorks.h>
#include <semLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <logLib.h>
#include <lstLib.h>
#include <iosLib.h>
#include <hwif/vxBus.h>

#include "cifXVxWorks_internal.h"
#include "cifXToolkit/cifXToolkit.h"
#include "cifXToolkit/cifXErrors.h"

STATUS cifXAddPCIDevice ( VXW_CIFXDRV_DEVICEENTRY_T* ptDevEntry, char* pszName, PVXW_CIFXDRV_DEVINSTANCE_T * ptDevInstance);

IMPORT VXB_DRV vxbPciCifxDrv;

/* defines */
typedef struct
    {
    DEV_HDR                     devHdr;                   /* device header */
    VXB_DEV_ID                  devID;                    /* device id */
    char                        name[MAX_DRV_NAME_LEN];   /* device name */
    VXW_CIFXDRV_DEVINSTANCE_T * ptDevInstance;            /* Hilscher device instance data */
    } CIFXDRV_DEV;

/* typedefs */
LOCAL int   g_iDriverNumber  = 0;                         /* driver number assigned to this driver */
LOCAL BOOL  g_fVxBusOK       = FALSE;                     /* VxBus initialization state */


/* Forward declarations */
STATUS registerCIFXDrv (BOOL                        fSingleDir,         /*!< !=0 will use a single firmware directory */
                        unsigned long               ulTraceLevel,       /*!< Trace level at start-up */
                        unsigned long               ulPollInterval,     /*!< Interval to poll for COS Flag changes */
                        char*                       szDriverBaseDir);   /*!< Base device directory to search for settings/configuration/firmware files */

STATUS createCIFXDev   (char *                      name,               /*!< Name of the iso device to create */
                        VXB_DEV_ID                  pDev,               /*!< Trace level at startup */
                        VXW_CIFXDRV_DEVICEENTRY_T * pDevData);          /*!< Device data data */

/* I/O control functions */
LOCAL VXW_CIFXDRV_DEVINSTANCE_T * cifXDrvOpen ( CIFXDRV_DEV *  pDevice,
                                                char *         name,
                                                int            flags);

LOCAL STATUS                      cifXDrvClose( VXW_CIFXDRV_DEVINSTANCE_T *   pDevice);   /* Our device handle */
LOCAL STATUS                      cifXDrvIoctl( VXW_CIFXDRV_DEVINSTANCE_T *   hDevice,    /* Our device handle */
                                                int                           cmd,        /* Requested command */
                                                _Vx_ioctl_arg_t               arg);       /* Command argument */


/* Information from the cifX Toolkit */
extern TKIT_DRIVER_INFORMATION  g_tDriverInfo;                /*!< Global driver information            */

extern uint32_t                 g_ulDeviceCount;              /*!< Number of devices handled by toolkit */
extern PDEVICEINSTANCE*         g_pptDevices;                 /*!< Array of device informations         */
extern void*                    g_pvTkitLock;                 /*!< Toolkit lock */
extern char*                    g_szDriverBaseDir;            /*!< Device base directory */
extern BOOL                     g_fSingleDir;                 /*!< Device base directory */

/*****************************************************************************/
/*! registerCIFXDrv - install a CIFX driver
 *  This routine initializes the CIFX driver. It is called automatically
 *  when VxWorks is configured with the INCLUDE_CIFXDRV component.
 *
 *  It must be called exactly once, before any other routine in the library.
 *
 *  \param fSingleDir      Use a single firmware directory
 *  \param ulTraceLevel    Trace level at startup
 *  \param ulPollInterval  Intervall to poll for COS Flag changes
 *  \param szDriverBaseDir Base device directory to search for settings/configuration/firmware files
 *
 *  \return OK, or ERROR if the I/O system cannot install the driver.         */
 /*****************************************************************************/
STATUS registerCIFXDrv (BOOL           fSingleDir,
                        unsigned long  ulTraceLevel,
                        unsigned long  ulPollInterval,
                        char*          szDriverBaseDir)
{
  STATUS iRet = OK;
  long   lRet = CIFX_NO_ERROR;

  /* Check if driver already installed */
  if (g_iDriverNumber > 0)
  {
    return iRet;
  }

  /* Wait until file system is available */
  taskDelay( 300);

  /* Initialize the CIFX driver */
  if( CIFX_NO_ERROR != (lRet = cifXInitializeDriver ( fSingleDir,           /* !=0 will use a single firmware directory */
                                                      ulTraceLevel,         /* Trace level at start-up */
                                                      ulPollInterval,       /* Interval to poll for COS Flag changes */
                                                      szDriverBaseDir)))    /* Base device directory to search for settings/configuration/firmware files */
  {
    iRet = ERROR;

  /* Offer the IOS interface */
  } else if( 0 >= (g_iDriverNumber = iosDrvInstall ((DRV_CREATE_PTR) cifXDrvOpen,
                                                    (DRV_REMOVE_PTR) NULL,
                                                    (DRV_OPEN_PTR)   cifXDrvOpen,
                                                    (DRV_CLOSE_PTR)  cifXDrvClose,
                                                    (DRV_READ_PTR)   NULL,
                                                    (DRV_WRITE_PTR)  NULL,
                                                    (DRV_IOCTL_PTR)  cifXDrvIoctl) ))
  {
    /* Failed to initialize our driver interface */
    iRet = ERROR;

    /* Add our driver as a VxBus PCI driver to the system */
  } else
  {
    /* Create our PCI driver */
    if ( OK != (iRet = vxbDrvAdd( &vxbPciCifxDrv)))
    {
      iRet = ERROR;
    } else
    {
      /* We have a PCI device driver  added */
      g_fVxBusOK = TRUE;
    }
  }

  if (OK != iRet)
  {
    /* Un-install the driver interface */
    if ( g_iDriverNumber > 0)
    {
      g_iDriverNumber = 0;
    }

    if ( TRUE == g_fVxBusOK)
      vxbDrvRemove( &vxbPciCifxDrv);

    /* De-initialize the driver */
    cifXDeinitializeDriver();
  }

  return iRet;
}

/*****************************************************************************/
/*! createCIFXDev - create a ios CIFX device
 *  This routine is called by the PCI component to create a ios device
 *
 *  \param name     Device name to create
 *  \param pDev     VxWorks device identification
 *  \param pDevData   Hilscher device data
 *
 *  \return OK, or ERROR if device cannot be created                          */
 /*****************************************************************************/
STATUS createCifXDevice(char *                      name,
                        VXB_DEV_ID                  pDev,
                        VXW_CIFXDRV_DEVICEENTRY_T * pDevData)
{
  STATUS                      sRet = OK;
  CIFXDRV_DEV *               pCifxDrv = NULL;
  VXW_CIFXDRV_DEVINSTANCE_T * ptDevInstance = NULL;

  /* Try to add the new device to the hilscher toolkit */
  if ( OK == (sRet = cifXAddPCIDevice ( pDevData, name, &ptDevInstance)))
  {
    /* Create a CIFX ios device */
    pCifxDrv = malloc (sizeof (CIFXDRV_DEV));
    VXB_ASSERT (pCifxDrv != NULL, ERROR);

    bzero ((char *) pCifxDrv, sizeof (CIFXDRV_DEV));
    sprintf (pCifxDrv->name, "/%s", name);

    pCifxDrv->ptDevInstance = ptDevInstance;

    /* Setup an ios device to handle our PCI devices */
    if (iosDevAdd ((DEV_HDR *) & (pCifxDrv->devHdr), pCifxDrv->name, g_iDriverNumber) != OK)
    {
      free (pCifxDrv);
      sRet = ERROR;
    }
  }

  return sRet;
}

/*****************************************************************************/
/*! cifXDrvOpen - open CIFX driver
 *  This routine is called to open a CIFX device.
 *  It returns a pointer to the device.  This routine is normally reached only via the I/O system.
 *
 *  \param pDevice    CIFX device handle
 *  \param name       String pointer holding the device name
 *  \param flags      Flags for the device handle (not used)
 *
 *  \return OK, or ERROR if the I/O system cannot install the driver.         */
 /*****************************************************************************/
LOCAL VXW_CIFXDRV_DEVINSTANCE_T * cifXDrvOpen ( CIFXDRV_DEV * pCifXDevice,
                                                char *        name,
                                                int           flags)
{
  PVXW_CIFXDRV_DEVINSTANCE_T ptDrvDevInst = (PVXW_CIFXDRV_DEVINSTANCE_T)pCifXDevice->ptDevInstance;

  /* Make sure this is a valid device */
  if ( NULL == pCifXDevice)
    return ((VXW_CIFXDRV_DEVINSTANCE_T *) ERROR);

  /* Non-null file name */
  if (EOS != name[0])
    return ((VXW_CIFXDRV_DEVINSTANCE_T *) ERROR);

  if(CIFX_NO_ERROR == xDriverOpen(&ptDrvDevInst->hDriver))
  {
    ++ptDrvDevInst->ulOpenCount;

    if(g_ulTraceLevel & TRACE_LEVEL_INFO)
  {
    USER_Trace(&ptDrvDevInst->tDeviceInst,
             TRACE_LEVEL_INFO,
         "cifXDrvOpen function reached, count = %d", ptDrvDevInst->ulOpenCount);
    }

    return pCifXDevice->ptDevInstance;
  }

  return NULL;
}

/*****************************************************************************/
/*! cifXDrvClose - open CIFX driver
 *  This routine is called to close a CIFX device.
 *  This routine is normally reached only via the I/O system.
 *  \param pDevice    CIFX device handle
 *  \return OK on success                                                     */
 /*****************************************************************************/
LOCAL STATUS cifXDrvClose( VXW_CIFXDRV_DEVINSTANCE_T * pCifXDrvDevice)
{
  /* Make sure this is a valid device */
  if ( NULL == pCifXDrvDevice)
    return (ERROR);

  if( CIFX_NO_ERROR == xDriverClose(pCifXDrvDevice->hDriver))
  {
    --pCifXDrvDevice->ulOpenCount;
  if(0 == pCifXDrvDevice->ulOpenCount)
    pCifXDrvDevice->hDriver = NULL;
  }

  if(g_ulTraceLevel & TRACE_LEVEL_INFO)
  {
    USER_Trace(&pCifXDrvDevice->tDeviceInst,
             TRACE_LEVEL_INFO,
         "cifXDrvClose function reached, count = %d", pCifXDrvDevice->ulOpenCount);
  }

  return (OK);
}

/*****************************************************************************/
/*! Progress callback wrapper
*   \param  ulStep
*   \param  ulMaxStep
*   \param  pvUser
*   \param  bFinished
*   \param  lError                                                           */
/*****************************************************************************/
static void ProgressCallback(uint32_t ulStep, uint32_t ulMaxStep, void* pvUser, int8_t bFinished, int32_t lError)
{
  PCALLBACK_DATA_T ptCallbackData = (PCALLBACK_DATA_T)pvUser;

  /* Check for previous errors */
  if(ptCallbackData->fSuppressProgress)
  {
    if(g_ulTraceLevel & TRACE_LEVEL_DEBUG)
    {
      USER_Trace(&ptCallbackData->ptDevExt->tDeviceInst,
                TRACE_LEVEL_DEBUG,
                "Progress callback suppressed, due to previous error!");
    }
  } else
  {
    /* Callback application */
    *(ptCallbackData->tProgressCallback.pbFinished) = bFinished;
    *(ptCallbackData->tProgressCallback.plError)    = lError;
    *(ptCallbackData->tProgressCallback.pulMaxStep) = ulMaxStep;
    *(ptCallbackData->tProgressCallback.pulStep)    = ulStep;

    OS_SetEvent(ptCallbackData->hProgressOut);

    /* Wait for application to signal end of callback via event */
    if(CIFX_EVENT_SIGNALLED != OS_WaitEvent(ptCallbackData->hProgressIn, 1000))
    {
      ptCallbackData->fSuppressProgress = 1;
    }
  }
}

/*****************************************************************************/
/*! Receive packet callback wrapper
*   \param  ptRecvPacket    Receive Packet
*   \param  pvUser          Pointer to callback management                   */
/*****************************************************************************/
static void ReceiveCallback(CIFX_PACKET* ptRecvPkt, void* pvUser)
{
  PCALLBACK_DATA_T ptCallbackData = (PCALLBACK_DATA_T)pvUser;

  /* Check for previous errors */
  if(ptCallbackData->fSuppressRecv)
  {
    if(g_ulTraceLevel & TRACE_LEVEL_DEBUG)
    {
      USER_Trace(&ptCallbackData->ptDevExt->tDeviceInst,
                 TRACE_LEVEL_DEBUG,
                 "Receive callback suppressed, due to previous error!");
    }
  } else
  {
    /* Callback application */
    OS_Memcpy(ptCallbackData->tRecvCallback.pvBuffer,
              ptRecvPkt,
              min(ptRecvPkt->tHeader.ulLen + sizeof(ptRecvPkt->tHeader),
              ptCallbackData->tRecvCallback.ulBufferLen));

    OS_SetEvent(ptCallbackData->hRecvOut);

    /* Wait for application to signal end of callback via event */
    if(CIFX_EVENT_SIGNALLED != OS_WaitEvent(ptCallbackData->hRecvIn, 1000))
    {
      ptCallbackData->fSuppressRecv = 1;
    }
  }
}

/*****************************************************************************/
/*! Prepare receive packet callback
*   \param  ptCmdClbkIN     Receive data command structure
*   \param  ptCallbackData  Management structure for this callback
*   \return NULL if no callback is requested / can be done                   */
/*****************************************************************************/
static PFN_RECV_PKT_CALLBACK PrepareRecvCallbackData( IOCTL_RECV_CALLBACK_T* ptCmdClbkIN, CALLBACK_DATA_T* ptCallbackData)
{
  PFN_RECV_PKT_CALLBACK pfnRecv = NULL;

  if( NULL != ptCmdClbkIN)
  {
    if( 0 != OS_Strlen( ptCmdClbkIN->szEventIN))
    {
      /* Create named event */
      ptCallbackData->hRecvOut = semOpen(ptCmdClbkIN->szEventIN,
                                         SEM_TYPE_BINARY,
                                         SEM_EMPTY,
                                         SEM_Q_PRIORITY,
                                         0,
                                         NULL);
    }

    if( 0 != OS_Strlen( ptCmdClbkIN->szEventIN))
    {
      /* Create named event */
      ptCallbackData->hRecvIn = semOpen(ptCmdClbkIN->szEventOUT,
                                        SEM_TYPE_BINARY,
                                        SEM_EMPTY,
                                        SEM_Q_PRIORITY,
                                        0,
                                        NULL);
    }

    if( (SEM_ID_NULL != ptCallbackData->hRecvIn) &&
        (SEM_ID_NULL != ptCallbackData->hRecvOut)  )
    {
      /* Got the global semaphore */
      ptCallbackData->tRecvCallback.pvBuffer    = ptCmdClbkIN->pvBuffer;
      ptCallbackData->tRecvCallback.ulBufferLen = ptCmdClbkIN->ulBufferLen;

      pfnRecv  = ReceiveCallback;
    }
  }

  return pfnRecv;
}

/*****************************************************************************/
/*! Prepare progress callback
*   \param  ptCmdClbkIN     Receive data command structure
*   \param  ptCallbackData  Management structure for this callback
*   \return NULL if no callback is requested / can be done                   */
/*****************************************************************************/
static PFN_PROGRESS_CALLBACK PrepareProgressCallbackData( IOCTL_PROGRESS_CALLBACK_T* ptCmdClbkIN, CALLBACK_DATA_T* ptCallbackData)
{
  PFN_PROGRESS_CALLBACK pfnProgress = NULL;

  if( NULL != ptCmdClbkIN)
  {
    if( 0 != OS_Strlen( ptCmdClbkIN->szEventIN))
    {
      /* Create named event */
      ptCallbackData->hProgressOut = semOpen(ptCmdClbkIN->szEventIN,
                                             SEM_TYPE_BINARY,
                                             SEM_EMPTY,
                                             SEM_Q_PRIORITY,
                                             0,
                                             NULL);
    }

    if( 0 != OS_Strlen( ptCmdClbkIN->szEventOUT))
    {
      /* Create named event */
      ptCallbackData->hProgressIn = semOpen(ptCmdClbkIN->szEventOUT,
                                            SEM_TYPE_BINARY,
                                            SEM_EMPTY,
                                            SEM_Q_PRIORITY,
                                            0,
                                            NULL);
    }

    if( (SEM_ID_NULL != ptCallbackData->hProgressIn) &&
        (SEM_ID_NULL != ptCallbackData->hProgressOut)  )
    {
      /* Got the global semaphore */
      ptCallbackData->tProgressCallback.plError    = &ptCmdClbkIN->lError;
      ptCallbackData->tProgressCallback.pbFinished = &ptCmdClbkIN->bFinished;
      ptCallbackData->tProgressCallback.pulMaxStep = &ptCmdClbkIN->ulMaxStep;
      ptCallbackData->tProgressCallback.pulStep    = &ptCmdClbkIN->ulStep;

      pfnProgress  = ProgressCallback;
    }
  }

  return pfnProgress;
}

/*****************************************************************************/
/*! Create an event handle from a given event name
*   \param  pszEventName  Event name of the global event
*   \return NULL if failed                                                   */
/*****************************************************************************/
static SEM_ID PrepareNotificationEvent(char* pszEventName)
{
  return semOpen( pszEventName,
                  SEM_TYPE_BINARY,
                  SEM_EMPTY,
                  SEM_Q_PRIORITY,
                  0,
                  NULL);
}

/*****************************************************************************/
/*! Clean up opened public event handles used for receive callback
*   \param  ptCallbackData  Management structure for this callback
*   \return 0 on success, else -1                                            */
/*****************************************************************************/
static void CleanRecvCallbackData(CALLBACK_DATA_T* ptCallbackData)
{
  if( NULL != ptCallbackData)
  {
    if( NULL != ptCallbackData->hRecvIn)
    {
      semClose(ptCallbackData->hRecvIn);
    }

    if( NULL != ptCallbackData->hRecvOut)
    {
      semClose(ptCallbackData->hRecvOut);
    }
  }
}

/*****************************************************************************/
/*! Clean up opened public event handles used for progress callback
*   \param  ptCallbackData  Management structure for this callback
*   \return 0 on success, else -1                                            */
/*****************************************************************************/
static void CleanProgressCallbackData(CALLBACK_DATA_T* ptCallbackData)
{
  if( NULL != ptCallbackData)
  {
    if( NULL != ptCallbackData->hProgressIn)
    {
      semClose(ptCallbackData->hProgressIn);
    }
    if( NULL != ptCallbackData->hProgressOut)
    {
      semClose(ptCallbackData->hProgressOut);
    }
  }
}

/*****************************************************************************/
/*! Create an event handle from a given event name
*   \param  pszEventName  Event name of the global event
*   \return 0 on success, else -1                                            */
/*****************************************************************************/
static void CleanNotificationEvent(SEM_ID hEvent)
{
  if( SEM_ID_NULL != hEvent)
  {
    semClose(hEvent);
  }
}

/*****************************************************************************/
/*! Check the state of the system device
*   \param  ptDevice    Pointer to the driver device instance
*   \param  plError     Pointer to error buffer
*   \return TRUE if state is OK                                              */
/*****************************************************************************/
static BOOL CheckSysdeviceState( PVXW_CIFXDRV_DEVINSTANCE_T ptDevice, int32_t* plError)
{
  BOOL fRet = FALSE;

  if(NULL == ptDevice->hSysdevice)
  {
    *plError = CIFX_DRV_CHANNEL_NOT_INITIALIZED;

  } else
  {
    fRet =TRUE;
  }

  return fRet;
}

/*****************************************************************************/
/*! Check the state of the communication channel
*   \param  ptDevice    Pointer to the driver device instance
*   \param  ulChannel   Communication channel number
*   \param  plError     Pointer to error buffer
*   \return TRUE if state is OK                                              */
/*****************************************************************************/
static BOOL CheckChannelState(PVXW_CIFXDRV_DEVINSTANCE_T ptDevice, uint32_t ulChannel, int32_t* plError)
{
  BOOL fRet = FALSE;

  if(ulChannel > (sizeof(ptDevice->ahChannels) / sizeof(ptDevice->ahChannels[0])) )
  {
    *plError = CIFX_INVALID_CHANNEL;

  } else if(NULL == ptDevice->ahChannels[ulChannel])
  {
    *plError = CIFX_DRV_CHANNEL_NOT_INITIALIZED;

  } else
  {
    fRet = TRUE;
  }

  return fRet;
}

/*****************************************************************************/
/*! Notify callback wrapper (taken from cifxCEDRV/CifxMain.cpp)
*   \param  ulNotification  Notification event
*   \param  ulDataLen       Length of provided data
*   \param  pvData          Notification data
*   \param  pvUser          Callback registration data                       */
/*****************************************************************************/
static void NotifyCallbackWrapper(uint32_t ulNotification, uint32_t ulDataLen, void* pvData, void* pvUser)
{
  CALLBACK_REGISTRATION_T* ptCallback = (CALLBACK_REGISTRATION_T*)pvUser;

  UNREFERENCED_PARAMETER(ulNotification);

  if(ptCallback->hUserEvent != NULL)
  {
    /* Copy Data do the user can get it via IOCTL call */
  if( ulDataLen <= DEVIOCTL_DATA_BUFFER_SIZE)
  {
      OS_Memcpy(ptCallback->abNotifyData, pvData, ulDataLen);
      ptCallback->ulNotifyDataLen = ulDataLen;
  }

    OS_SetEvent(ptCallback->hUserEvent);
  }
}

/*****************************************************************************/
/*! cifXDrvIoctl - do CIFX device control function
 *  This routine performs the CIFX API functions:
 *  If the ioctl (<function>) is unknown by the device driver, an error is
 *  returned.
 *  \param hDevice    Device handle
 *  \param cmd        I/O control function number
 *  \param arg        Argument structure holds the command specific data
 *
 *  \return OK, or ERROR if function failed or unknown function.              */
 /*****************************************************************************/
STATUS cifXDrvIoctl( VXW_CIFXDRV_DEVINSTANCE_T * ptDrvDevInst,    /* Our device handle */
                     int                         IoControlCode,   /* Requested command */
                     _Vx_ioctl_arg_t             ioBuffer         /* Command argument */
                    )
{
  STATUS        retVal    = OK;
  DEVxIO_BASE*  ptBaseCmd = (DEVxIO_BASE*)ioBuffer;

  if (ptDrvDevInst == NULL)
    return ERROR;

  if( (NULL == ptBaseCmd)           ||
      (NULL == ptBaseCmd->plError))
    return ERROR;

  switch(IoControlCode)
  {

    /*=============================================================================================*/
    /* DRIVER RELATED FUNCTIONS                                                                    */
    /*=============================================================================================*/
    /*-------------------------------------------*/
    /* Driver: Read driver information */
    case CIFx_IOCTL_DRV_INFO:
      {
        /* Read driver driver information data */
        DEVxIO_DRIVER_INFO* ptCmd     = (DEVxIO_DRIVER_INFO*)ioBuffer;

        if( NULL != ptCmd->ptDriverInfo)
        {
          *(ptCmd->plError) = xDriverGetInformation(ptDrvDevInst->hDriver,
                           ptCmd->ulSize,
                           ptCmd->ptDriverInfo);

          /* Overwrite the toolkit version with the real driver version from cifXMan.cpp */
          OS_Strncpy(ptCmd->ptDriverInfo->abDriverVersion,
                     VXWCIFXDRV_VERSION,
                     sizeof(ptCmd->ptDriverInfo->abDriverVersion));
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Driver: Enumerate available boards */
    case CIFx_IOCTL_DRV_ENUMBOARD:
      {
        DEVxIO_DRIVER_ENUMBOARD* ptCmd  = (DEVxIO_DRIVER_ENUMBOARD*)ioBuffer;

        if ( NULL != ptCmd->ptBoardInfo)
        {
          /* Return the available boards */
          *(ptCmd->plError) = xDriverEnumBoards(ptDrvDevInst->hDriver,
                                                ptCmd->ulBoardNumber,
                                                ptCmd->ulSize,
                                                ptCmd->ptBoardInfo);
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Driver: Enumerate board channels */
    case CIFx_IOCTL_DRV_ENUMCHANNEL:
      {
        /* Enumerate the active channels */
        DEVxIO_DRIVER_ENUMCHANNEL* ptCmd  = (DEVxIO_DRIVER_ENUMCHANNEL*)ioBuffer;

        if( NULL != ptCmd->ptChannelInfo)
        {
          *(ptCmd->plError) = xDriverEnumChannels(ptDrvDevInst->hDriver,
                                                  ptCmd->ulBoardNumber,
                                                  ptCmd->ulChannelNumber,
                                                  ptCmd->ulSize,
                                                  ptCmd->ptChannelInfo);
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Driver: Get a DPM memory pointer */
    case CIFx_IOCTL_DRV_MEMPTR:
      {
        /* Enumerate the active channels */
        DEVxIO_DRIVER_MEMPTR* ptCmd  = (DEVxIO_DRIVER_MEMPTR*)ioBuffer;

        *(ptCmd->plError) = CIFX_INVALID_POINTER;

        if( (NULL != ptCmd->ptMemory)                &&
            (NULL != ptCmd->ptMemory->ppvMemoryPtr)  &&
            (NULL != ptCmd->ptMemory->pulMemorySize))
        {
          if(ptCmd->ptMemory->ulChannel != CIFX_NO_CHANNEL)
          {
            if( (NULL == ptCmd->ptMemory->pulChannelStartOffset) ||
                (NULL == ptCmd->ptMemory->pulChannelSize) )
            {
              *(ptCmd->plError) = CIFX_INVALID_POINTER;
              break;
            }
          }

          MEMORY_INFORMATION* ptMemory = ptCmd->ptMemory;

          *(ptCmd->plError) = xDriverMemoryPointer(ptDrvDevInst->hDriver,
                                                   ptCmd->ulBoardNumber,
                                                   ptCmd->ulCmd,
                                                   ptMemory);
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Driver: Download complete to store config files */
    case CIFx_IOCTL_DRV_DOWNLOADCOMPLETE:
      {
        DEVxIO_DOWNLOAD_COMPLETE* ptCmd  = (DEVxIO_DOWNLOAD_COMPLETE*)ioBuffer;

        *(ptCmd->plError) = CIFX_NO_ERROR;
      }
      break;

    /*-------------------------------------------*/
    /* Driver: Restart device */
    case CIFx_IOCTL_DRV_RESTARTDEVICE:
      {
        DEVxIO_DRIVER_RESTARTDEVICE* ptCmd  = (DEVxIO_DRIVER_RESTARTDEVICE*)ioBuffer;

        DEVICEINSTANCE*            ptDevice   = NULL;
        VXW_CIFXDRV_DEVINSTANCE_T* ptDrvInst  = NULL;

        if(NULL != ptCmd->szBoardName)
        {
          unsigned long ulIdx = 0;

          OS_EnterLock(g_pvTkitLock);

      /* Search the device with the given name */
      for (ulIdx = 0; ulIdx < g_ulDeviceCount; ulIdx++)
      {
      /* Compare the device name */
      ptDevice = (DEVICEINSTANCE*)(g_pptDevices[ulIdx]);

      uint32_t ulCompareLength = max(OS_Strlen(ptCmd->szBoardName), OS_Strlen(ptDevice->szName));
      if( 0 == OS_Strnicmp( ptDevice->szName, ptCmd->szBoardName, ulCompareLength))
      {
        if (g_ulTraceLevel & TRACE_LEVEL_INFO)
        {
        USER_Trace( ptDevice,
              TRACE_LEVEL_INFO,
              "RESTART DEVICE requested for device: %s",
              ptCmd->szBoardName);
        }

        ptDrvInst = (VXW_CIFXDRV_DEVINSTANCE_T*)ptDevice->pvOSDependent;
        break;
      }
      }

      OS_LeaveLock(g_pvTkitLock);

      if( NULL == ptDrvInst)
      {
        /* No device with the given name found */
            *(ptCmd->plError) = CIFX_INVALID_BOARD;

      }else
      {
      /* Remove a device */
      *(ptCmd->plError) = cifXTKitRemoveDevice(ptDevice->szName, 0);

      OS_Sleep(100);

      if ( CIFX_NO_ERROR == *(ptCmd->plError))
      {
        *(ptCmd->plError) = cifXTKitAddDevice(ptDevice);

        OS_Sleep(100);
      }

      if (g_ulTraceLevel & TRACE_LEVEL_INFO)
      {
        USER_Trace(ptDevice,
             TRACE_LEVEL_INFO,
             "RESTART DEVICE done, (Status: 0x%08X)\n",
             *(ptCmd->plError));
      }
      }

        } else
        {
      *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*=============================================================================================*/
    /* SYSTEM DEVICE RELATED FUNCTIONS                                                             */
    /*=============================================================================================*/
    /*-------------------------------------------*/
    /* Sysdevice: System device information */
    case CIFx_IOCTL_SYSDEVICE_INFO:
      {
        /* Open a channel, check if we have such a channel */
        DEVxIO_SYSDEVICE_INFO* ptCmd  = (DEVxIO_SYSDEVICE_INFO*)ioBuffer;

        if ( NULL != ptCmd->pvInfo)
        {
          if(CheckSysdeviceState(ptDrvDevInst, ptCmd->plError))
          {
            *(ptCmd->plError) = xSysdeviceInfo(ptDrvDevInst->hSysdevice,
                                               ptCmd->ulCmd,
                                               ptCmd->ulSize,
                                               ptCmd->pvInfo);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Open System device */
    case CIFx_IOCTL_SYSDEVICE_OPEN:
      {
        /* Open a channel, check if we have such a channel */
        DEVxIO_CHANNEL_OPEN* ptCmd  = (DEVxIO_CHANNEL_OPEN*)ioBuffer;

        *(ptCmd->plError) = xSysdeviceOpen(ptDrvDevInst->hDriver,
                                           ptDrvDevInst->tDeviceInst.szName,
                                           &ptDrvDevInst->hSysdevice);

        if(CIFX_NO_ERROR == (*ptCmd->plError))
        {
          ++ptDrvDevInst->ulSysdeviceOpenCnt;
        }

        if(g_ulTraceLevel & TRACE_LEVEL_INFO)
        {
        USER_Trace(&ptDrvDevInst->tDeviceInst,
                 TRACE_LEVEL_INFO,
             "xSysdeviceOpen called, count = %d", ptDrvDevInst->ulSysdeviceOpenCnt);
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Close system device */
    case CIFx_IOCTL_SYSDEVICE_CLOSE:
      {
        /* Close channel, check if we have such a channel */
        DEVxIO_CHANNEL_CLOSE* ptCmd  = (DEVxIO_CHANNEL_CLOSE*)ioBuffer;

        if(CheckSysdeviceState(ptDrvDevInst, ptCmd->plError))
        {
          *(ptCmd->plError) = xSysdeviceClose(ptDrvDevInst->hSysdevice);

          if( (CIFX_NO_ERROR == *(ptCmd->plError)) &&
              (0 == --ptDrvDevInst->ulSysdeviceOpenCnt) )
          {
            ptDrvDevInst->hSysdevice = NULL;
          }
        }

        if(g_ulTraceLevel & TRACE_LEVEL_INFO)
        {
        USER_Trace(&ptDrvDevInst->tDeviceInst,
                 TRACE_LEVEL_INFO,
             "xSysdeviceClose called, count = %d", ptDrvDevInst->ulSysdeviceOpenCnt);
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Send packet via system mailbox */
    case CIFx_IOCTL_SYSDEVICE_PUTPACKET:
      {
        /* Write a packet to the hardware */
        DEVxIO_CHANNEL_PACKET* ptCmd  = (DEVxIO_CHANNEL_PACKET*)ioBuffer;
        CIFX_PACKET* ptPacket = (CIFX_PACKET*)ptCmd->pvPacket;

        if ( NULL != ptPacket)
        {
          if(CheckSysdeviceState(ptDrvDevInst, ptCmd->plError))
          {
            *(ptCmd->plError) = xSysdevicePutPacket(ptDrvDevInst->hSysdevice,
                                                    ptPacket,
                                                    ptCmd->ulTimeout);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Receive packet via system mailbox */
    case CIFx_IOCTL_SYSDEVICE_GETPACKET:
      {
        /* Write a packet to the hardware */
        DEVxIO_CHANNEL_PACKET* ptCmd  = (DEVxIO_CHANNEL_PACKET*)ioBuffer;

        if ( NULL != ptCmd->pvPacket)
        {
          if(CheckSysdeviceState(ptDrvDevInst, ptCmd->plError))
          {
            *(ptCmd->plError) = xSysdeviceGetPacket(ptDrvDevInst->hSysdevice,
                                                    ptCmd->ulBufferSize,
                                                    (CIFX_PACKET*)ptCmd->pvPacket,
                                                    ptCmd->ulTimeout);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Read system device mailbox state */
    case CIFx_IOCTL_SYSDEVICE_MBXSTATE:
      {
        /* Write a packet to the hardware */
        DEVxIO_CHANNEL_MBXSTATE* ptCmd  = (DEVxIO_CHANNEL_MBXSTATE*)ioBuffer;

        if ( (NULL != ptCmd->pulReceiveCount) &&
             (NULL != ptCmd->pulSendState)  )
        {
          if(CheckSysdeviceState(ptDrvDevInst, ptCmd->plError))
          {
            *(ptCmd->plError) = xSysdeviceGetMBXState(ptDrvDevInst->hSysdevice,
                                                      ptCmd->pulReceiveCount,
                                                      ptCmd->pulSendState);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Perform card reset */
    case CIFx_IOCTL_SYSDEVICE_RESET:
      {
        DEVxIO_CHANNEL_RESET* ptCmd = (DEVxIO_CHANNEL_RESET*)ioBuffer;

        if(CheckSysdeviceState(ptDrvDevInst, ptCmd->plError))
        {
          *(ptCmd->plError) = xSysdeviceReset(ptDrvDevInst->hSysdevice, (ptCmd->ulTimeout));
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Perform card bootstart */
    case CIFx_IOCTL_SYSDEVICE_BOOTSTART:
      {
        DEVxIO_CHANNEL_RESET* ptCmd = (DEVxIO_CHANNEL_RESET*)ioBuffer;

        if(CheckSysdeviceState(ptDrvDevInst, ptCmd->plError))
        {
          *(ptCmd->plError) = xSysdeviceBootstart(ptDrvDevInst->hSysdevice, ptCmd->ulTimeout);
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Download */
    case CIFx_IOCTL_SYSDEVICE_DOWNLOAD:
      {
        /* Download to the device via the system channel */
        DEVxIO_DOWNLOAD*          ptCmd         = (DEVxIO_DOWNLOAD*)ioBuffer;

        PFN_PROGRESS_CALLBACK     pfnProgress   = NULL;
        PFN_RECV_PKT_CALLBACK     pfnRecv       = NULL;

        CALLBACK_DATA_T           tCallbackData;
        OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

        if( NULL != ptCmd->pvFileData)
        {
          tCallbackData.ptDevExt = ptDrvDevInst;

          /* Create the receive callback data if requested */
          pfnRecv = PrepareRecvCallbackData( ptCmd->ptRecvData, &tCallbackData);

          /* Create the progress callback data if requested */
          pfnProgress = PrepareProgressCallbackData( ptCmd->ptProgressData, &tCallbackData);

          if( CheckSysdeviceState(ptDrvDevInst, ptCmd->plError))
          {
            *(ptCmd->plError) = xSysdeviceDownload(ptDrvDevInst->hSysdevice,
                                                   ptCmd->ulChannel,
                                                   ptCmd->ulMode,
                                                   ptCmd->pszFileName,
                                                   (uint8_t*)ptCmd->pvFileData,
                                                   ptCmd->ulFileSize,
                                                   pfnProgress,                 /* Progress callback */
                                                   pfnRecv,                     /* Receive callback */
                                                   &tCallbackData);             /* pvUser */
          }

          CleanRecvCallbackData(&tCallbackData);
          CleanProgressCallbackData(&tCallbackData);
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Upload */
    case CIFx_IOCTL_SYSDEVICE_UPLOAD:
      {
        /* Download to the device via the system channel */
        DEVxIO_UPLOAD*            ptCmd         = (DEVxIO_UPLOAD*)ioBuffer;

        PFN_PROGRESS_CALLBACK     pfnProgress   = NULL;
        PFN_RECV_PKT_CALLBACK     pfnRecv       = NULL;

        CALLBACK_DATA_T           tCallbackData;
        OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

        if( NULL != ptCmd->pvFileData)
        {
          tCallbackData.ptDevExt = ptDrvDevInst;

          /* Create the receive callback data if requested */
          pfnRecv = PrepareRecvCallbackData( ptCmd->ptRecvData, &tCallbackData);

          /* Create the progress callback data if requested */
          pfnProgress = PrepareProgressCallbackData( ptCmd->ptProgressData, &tCallbackData);

          if (CheckSysdeviceState(ptDrvDevInst, ptCmd->plError))
          {
            *(ptCmd->plError) = xSysdeviceUpload(ptDrvDevInst->hSysdevice,
                                                 ptCmd->ulChannel,
                                                 ptCmd->ulMode,
                                                 ptCmd->pszFileName,
                                                 (uint8_t*)ptCmd->pvFileData,
                                                 ptCmd->pulFileSize,
                                                 pfnProgress,      /* Progress callback */
                                                 pfnRecv,          /* Receive callback */
                                                 &tCallbackData);  /* pvUser */
          }

          CleanRecvCallbackData(&tCallbackData);
          CleanProgressCallbackData(&tCallbackData);
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Find First File */
    case CIFx_IOCTL_SYSDEVICE_FINDFIRST:
      {
        /* Download to the device via the system channel */
        DEVxIO_FIND_FILE*      ptCmd    = (DEVxIO_FIND_FILE*)ioBuffer;

        PFN_RECV_PKT_CALLBACK  pfnRecv  = NULL;

        CALLBACK_DATA_T        tCallbackData;
        OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

        if( NULL != ptCmd->ptDirectoryInfo)
        {
          tCallbackData.ptDevExt = ptDrvDevInst;

          /* Create the receive callback data if requested */
          pfnRecv = PrepareRecvCallbackData( ptCmd->ptRecvData, &tCallbackData);

          if (CheckSysdeviceState(ptDrvDevInst, ptCmd->plError))
          {
            *(ptCmd->plError) = xSysdeviceFindFirstFile(ptDrvDevInst->hSysdevice,
                                                        ptCmd->ulChannel,
                                                        ptCmd->ptDirectoryInfo,
                                                        pfnRecv,
                                                        &tCallbackData);
          }

          CleanRecvCallbackData(&tCallbackData);
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Find Next File */
    case CIFx_IOCTL_SYSDEVICE_FINDNEXT:
      {
        /* Download to the device via the system channel */
        DEVxIO_FIND_FILE*     ptCmd    = (DEVxIO_FIND_FILE*)ioBuffer;

        PFN_RECV_PKT_CALLBACK pfnRecv  = NULL;

        CALLBACK_DATA_T       tCallbackData;
        OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

        if( NULL != ptCmd->ptDirectoryInfo )
        {
          tCallbackData.ptDevExt = ptDrvDevInst;

          /* Create the receive callback data if requested */
          pfnRecv = PrepareRecvCallbackData( ptCmd->ptRecvData, &tCallbackData);

          if (CheckSysdeviceState(ptDrvDevInst, ptCmd->plError))
          {
            *(ptCmd->plError) = xSysdeviceFindNextFile(ptDrvDevInst->hSysdevice,
                                                       ptCmd->ulChannel,
                                                       ptCmd->ptDirectoryInfo,
                                                       pfnRecv,
                                                       &tCallbackData);
          }

          CleanRecvCallbackData(&tCallbackData);
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Sysdevice: Extended Memory */
    case CIFx_IOCTL_SYSDEVICE_EXTMEM:
      {
        /* Download to the device via the system channel */
        DEVxIO_SYSDEVICE_EXTMEMPTR* ptCmd = (DEVxIO_SYSDEVICE_EXTMEMPTR*)ioBuffer;

        CIFX_EXTENDED_MEMORY_INFORMATION tExtMemInfo = {0};

        if( NULL != ptCmd->ptMemory)
        {
          /* convert to global structure */
          tExtMemInfo.pvMemoryID   = (void*)(uint32_t*)ptCmd->ptMemory->ullMemoryID;
          tExtMemInfo.pvMemoryPtr  = ptCmd->ptMemory->pvMemoryPtr;
          tExtMemInfo.ulMemorySize = ptCmd->ptMemory->ulMemorySize;
          tExtMemInfo.ulMemoryType = ptCmd->ptMemory->ulMemoryType;

          *(ptCmd->plError) = xSysdeviceExtendedMemory( ptDrvDevInst->hSysdevice,
                                                        ptCmd->ulCmd,
                                                        &tExtMemInfo);

          if( CIFX_NO_ERROR == *(ptCmd->plError))
          {
            if( CIFX_GET_EXTENDED_MEMORY_POINTER == ptCmd->ulCmd)
            {
              ptCmd->ptMemory->ullMemoryID   = (uint64_t)tExtMemInfo.pvMemoryID;
              ptCmd->ptMemory->pvMemoryPtr   = tExtMemInfo.pvMemoryPtr;
              ptCmd->ptMemory->ulMemorySize  = tExtMemInfo.ulMemorySize;
              ptCmd->ptMemory->ulMemoryType  = tExtMemInfo.ulMemoryType;
            }

            if( CIFX_GET_EXTENDED_MEMORY_INFO == ptCmd->ulCmd)
            {
              ptCmd->ptMemory->ulMemorySize = tExtMemInfo.ulMemorySize;
              ptCmd->ptMemory->ulMemoryType = tExtMemInfo.ulMemoryType;
            }
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*=============================================================================================*/
    /* CHANNEL RELATED FUNCTIONS                                                                   */
    /*=============================================================================================*/
    /*-------------------------------------------*/
    /* Channel: Read information */
    case CIFx_IOCTL_CH_INFO:
      {
        /* Enumerate the active channels */
        DEVxIO_CHANNEL_INFO* ptCmd  = (DEVxIO_CHANNEL_INFO*)ioBuffer;

        if( NULL != ptCmd->ptChannelInfo)
        {
          if(CheckChannelState(ptDrvDevInst,  ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelInfo(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                             ptCmd->ulSize,
                                             ptCmd->ptChannelInfo);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Open a channel */
    case CIFx_IOCTL_CH_OPEN:
      {
        /* Open a channel, check if we have such a channel */
        DEVxIO_CHANNEL_OPEN* ptCmd  = (DEVxIO_CHANNEL_OPEN*)ioBuffer;

        *(ptCmd->plError) = xChannelOpen(ptDrvDevInst->hDriver,
                                         ptDrvDevInst->tDeviceInst.szName,
                                         ptCmd->ulChannel,
                                         &ptDrvDevInst->ahChannels[ptCmd->ulChannel]);

        if(CIFX_NO_ERROR == *(ptCmd->plError))
        {
           ++ptDrvDevInst->aulChannelOpenCnt[ptCmd->ulChannel];
        }

        if(g_ulTraceLevel & TRACE_LEVEL_INFO)
        {
        USER_Trace(&ptDrvDevInst->tDeviceInst,
                 TRACE_LEVEL_INFO,
             "xChannelOpen called, count = %d", ptDrvDevInst->aulChannelOpenCnt[ptCmd->ulChannel]);
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Close a channel */
    case CIFx_IOCTL_CH_CLOSE:
      {
        /* Close channel, check if we have such a channel */
        DEVxIO_CHANNEL_CLOSE* ptCmd  = (DEVxIO_CHANNEL_CLOSE*)ioBuffer;

        if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
        {
          *(ptCmd->plError) = xChannelClose(ptDrvDevInst->ahChannels[ptCmd->ulChannel]);
        }

        if( (CIFX_NO_ERROR == *(ptCmd->plError)) &&
            (0 == --ptDrvDevInst->aulChannelOpenCnt[ptCmd->ulChannel]) )
        {
          ptDrvDevInst->ahChannels[ptCmd->ulChannel] = NULL;
        }

        if(g_ulTraceLevel & TRACE_LEVEL_INFO)
        {
        USER_Trace(&ptDrvDevInst->tDeviceInst,
                 TRACE_LEVEL_INFO,
             "xChannelClose called, count = %d", ptDrvDevInst->aulChannelOpenCnt[ptCmd->ulChannel]);
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Reset a channel */
    case CIFx_IOCTL_CH_RESET:
      {
        /* Reset channel, check if we have such a channel */
        DEVxIO_CHANNEL_RESET* ptCmd  = (DEVxIO_CHANNEL_RESET*)ioBuffer;

        if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
        {
          *(ptCmd->plError) = xChannelReset(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                            ptCmd->ulMode,
                                            ptCmd->ulTimeout);
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Process HOST state */
    case CIFx_IOCTL_CH_HOSTSTATE:
      {
        /* Read mailbox state */
        DEVxIO_CHANNEL_HOSTSTATE* ptCmd  = (DEVxIO_CHANNEL_HOSTSTATE*)ioBuffer;

        if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
        {
          *(ptCmd->plError) = xChannelHostState(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                ptCmd->ulCmd,
                                                ptCmd->pulState,
                                                ptCmd->ulTimeout);
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Process Configuration Lock */
    case CIFx_IOCTL_CH_CONFIGLOCK:
      {
        /* Read mailbox state */
        DEVxIO_CHANNEL_CONFIG_LOCK* ptCmd  = (DEVxIO_CHANNEL_CONFIG_LOCK*)ioBuffer;

        if( NULL != ptCmd->pulState)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelConfigLock(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                   ptCmd->ulCmd,
                                                   ptCmd->pulState,
                                                   ptCmd->ulTimeout);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Process BUS state */
    case CIFx_IOCTL_CH_BUSSTATE:
      {
        /* Read mailbox state */
        DEVxIO_CHANNEL_BUSSTATE* ptCmd  = (DEVxIO_CHANNEL_BUSSTATE*)ioBuffer;

        if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
        {
          *(ptCmd->plError) = xChannelBusState(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                               ptCmd->ulCmd,
                                               ptCmd->pulState,
                                               ptCmd->ulTimeout);
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: DMA state */
    case CIFx_IOCTL_CH_DMASTATE:
      {
        DEVxIO_CHANNEL_DMASTATE* ptCmd  = (DEVxIO_CHANNEL_DMASTATE*)ioBuffer;
#ifdef CIFX_TOOLKIT_DMA
        if(NULL != ptCmd->pulState)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelDMAState(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                 ptCmd->ulCmd,
                                                 ptCmd->pulState);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
#else
        *(ptCmd->plError) = CIFX_FUNCTION_NOT_AVAILABLE;
#endif
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Process watchdog */
    case CIFx_IOCTL_CH_WATCHDOG:
      {
        /* Write a packet to the hardware */
        DEVxIO_CHANNEL_WATCHDOG* ptCmd  = (DEVxIO_CHANNEL_WATCHDOG*)ioBuffer;

        if(NULL != ptCmd->pulWatchdog)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelWatchdog(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                 ptCmd->ulCmd,
                                                 ptCmd->pulWatchdog);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Send a packet via channel mailbox */
    case CIFx_IOCTL_CH_PUTPACKET:
      {
        /* Write a packet to the hardware */
        DEVxIO_CHANNEL_PACKET* ptCmd  = (DEVxIO_CHANNEL_PACKET*)ioBuffer;
        CIFX_PACKET* ptPacket = (CIFX_PACKET*)ptCmd->pvPacket;

        if( NULL != ptPacket)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelPutPacket(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                  (CIFX_PACKET*)ptCmd->pvPacket,
                                                  ptCmd->ulTimeout);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Receive a packet via channel mailbox */
    case CIFx_IOCTL_CH_GETPACKET:
      {
        /* Write a packet to the hardware */
        DEVxIO_CHANNEL_PACKET* ptCmd  = (DEVxIO_CHANNEL_PACKET*)ioBuffer;

        if( NULL != ptCmd->pvPacket)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelGetPacket(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                  ptCmd->ulBufferSize,
                                                  (CIFX_PACKET*)ptCmd->pvPacket,
                                                  ptCmd->ulTimeout);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Get the channel mailbox state */
    case CIFx_IOCTL_CH_MBXSTATE:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_MBXSTATE* ptCmd  = (DEVxIO_CHANNEL_MBXSTATE*)ioBuffer;

        if( (NULL != ptCmd->pulReceiveCount) &&
            (NULL != ptCmd->pulSendState))
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelGetMBXState(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                    ptCmd->pulReceiveCount,
                                                    ptCmd->pulSendState);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Read back the send packet mailbox */
    case CIFx_IOCTL_CH_READSENDPACKET:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_PACKET* ptCmd  = (DEVxIO_CHANNEL_PACKET*)ioBuffer;

        if( NULL != ptCmd->pvPacket)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelGetSendPacket(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                      ptCmd->ulBufferSize,
                                                      (CIFX_PACKET*)ptCmd->pvPacket);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Read channel information */
    case CIFx_IOCTL_CH_IOINFO:
      {
        DEVxIO_CHANNEL_IOINFO* ptCmd = (DEVxIO_CHANNEL_IOINFO*)ioBuffer;

        if( NULL != ptCmd->ptChannelIOInfo)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelIOInfo(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                               ptCmd->ulCmd,
                                               ptCmd->ulAreaNumber,
                                               ptCmd->ulSize,
                                               ptCmd->ptChannelIOInfo);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Read data from the input area */
    case CIFx_IOCTL_CH_IOREAD:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_IOREAD* ptCmd  = (DEVxIO_CHANNEL_IOREAD*)ioBuffer;

        if( NULL != ptCmd->pvBuffer)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelIORead(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                               ptCmd->ulAreaNumber,
                                               ptCmd->ulOffset,
                                               ptCmd->ulSize,
                                               ptCmd->pvBuffer,
                                               ptCmd->ulTimeout);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Write data to the output area */
    case CIFx_IOCTL_CH_IOWRITE:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_IOWRITE* ptCmd  = (DEVxIO_CHANNEL_IOWRITE*)ioBuffer;

        if( NULL != ptCmd->pvBuffer)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelIOWrite(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                ptCmd->ulAreaNumber,
                                                ptCmd->ulOffset,
                                                ptCmd->ulSize,
                                                ptCmd->pvBuffer,
                                                ptCmd->ulTimeout);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Read data back from the output area */
    case CIFx_IOCTL_CH_IOREADSEND:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_IOREAD* ptCmd  = (DEVxIO_CHANNEL_IOREAD*)ioBuffer;

        if( NULL != ptCmd->pvBuffer)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelIOReadSendData(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                       ptCmd->ulAreaNumber,
                                                       ptCmd->ulOffset,
                                                       ptCmd->ulSize,
                                                       ptCmd->pvBuffer);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: IO memory pointer */
    case CIFx_IOCTL_CH_IOMEMPTR:
      {
        /* Process the channels IO memptr function*/
        DEVxIO_CHANNEL_IOMEMPTR* ptCmd  = (DEVxIO_CHANNEL_IOMEMPTR*)ioBuffer;

        if( (NULL != ptCmd->ptMemory)                       &&
            (NULL != ptCmd->ptMemory->ppvMemoryPtr)         &&
            (NULL != ptCmd->ptMemory->pulIOAreaStartOffset) &&
            (NULL != ptCmd->ptMemory->pulIOAreaSize) )
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            PLC_MEMORY_INFORMATION* ptPLCMemoryInfo = ptCmd->ptMemory;

            /* xChannelPLCMemoryPtr will always map the whole DPM */
            *(ptCmd->plError) = xChannelPLCMemoryPtr(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                     ptCmd->ulCmd,
                                                     ptPLCMemoryInfo);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: PLC activate read */
    case CIFx_IOCTL_CH_IODOREAD:
      {
        /* Do I/O read function */
        DEVxIO_CHANNEL_IODORW* ptCmd  = (DEVxIO_CHANNEL_IODORW*)ioBuffer;

        if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
        {
          *(ptCmd->plError) = xChannelPLCActivateRead(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                      ptCmd->ulAreaNumber);
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: PLC activate write */
    case CIFx_IOCTL_CH_IODOWRITE:
      {
        /* Do I/O write function */
        DEVxIO_CHANNEL_IODORW* ptCmd  = (DEVxIO_CHANNEL_IODORW*)ioBuffer;

        if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
        {
          *(ptCmd->plError) = xChannelPLCActivateWrite(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                       ptCmd->ulAreaNumber);
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: PLC is read ready */
    case CIFx_IOCTL_CH_IOREADRDY:
      {
        /* Is I/O read ready */
        DEVxIO_CHANNEL_IOREADRDY* ptCmd  = (DEVxIO_CHANNEL_IOREADRDY*)ioBuffer;

        if( NULL != ptCmd->pulState)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelPLCIsReadReady(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                       ptCmd->ulAreaNumber,
                                                       ptCmd->pulState);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: PLC is write ready */
    case CIFx_IOCTL_CH_IOWRITERDY:
      {
        /* Do I/O write function */
        DEVxIO_CHANNEL_IOWRITERDY* ptCmd  = (DEVxIO_CHANNEL_IOWRITERDY*)ioBuffer;

        if( NULL != ptCmd->pulState)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelPLCIsWriteReady(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                        ptCmd->ulAreaNumber,
                                                        ptCmd->pulState);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Write data to the control area */
    case CIFx_IOCTL_CH_CTRL:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_CTRL* ptCmd  = (DEVxIO_CHANNEL_CTRL*)ioBuffer;

        if( NULL != ptCmd->pvBuffer)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelControlBlock(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                     ptCmd->ulCmd,
                                                     ptCmd->ulOffset,
                                                     ptCmd->ulSize,
                                                     ptCmd->pvBuffer);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Read data from the common state area*/
    case CIFx_IOCTL_CH_COMMON_STATE:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_STATE* ptCmd  = (DEVxIO_CHANNEL_STATE*)ioBuffer;

        if( NULL != ptCmd->pvBuffer)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelCommonStatusBlock(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                          ptCmd->ulCmd,
                                                          ptCmd->ulOffset,
                                                          ptCmd->ulSize,
                                                          ptCmd->pvBuffer);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Read data from the extended state area */
    case CIFx_IOCTL_CH_EXTENDED_STATE:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_STATE* ptCmd  = (DEVxIO_CHANNEL_STATE*)ioBuffer;

        if( NULL != ptCmd->pvBuffer)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelExtendedStatusBlock(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                            ptCmd->ulCmd,
                                                            ptCmd->ulOffset,
                                                            ptCmd->ulSize,
                                                            ptCmd->pvBuffer);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Read user area */
    case CIFx_IOCTL_CH_USER:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_USER* ptCmd  = (DEVxIO_CHANNEL_USER*)ioBuffer;

        if( NULL != ptCmd->pvBuffer)
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelUserBlock(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                  ptCmd->ulAreaNumber,
                                                  ptCmd->ulCmd,
                                                  ptCmd->ulOffset,
                                                  ptCmd->ulSize,
                                                  ptCmd->pvBuffer);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Download */
    case CIFx_IOCTL_CH_DOWNLOAD:
      {
        /* Download to the device via the system channel */
        DEVxIO_DOWNLOAD*          ptCmd         = (DEVxIO_DOWNLOAD*)ioBuffer;

        PFN_PROGRESS_CALLBACK     pfnProgress   = NULL;
        PFN_RECV_PKT_CALLBACK     pfnRecv       = NULL;

    CALLBACK_DATA_T           tCallbackData;
        OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

        if( NULL != ptCmd->pvFileData)
        {
          tCallbackData.ptDevExt = ptDrvDevInst;

          /* Create the receive callback data if requested */
          pfnRecv = PrepareRecvCallbackData( ptCmd->ptRecvData, &tCallbackData);

          /* Create the progress callback data if requested */
          pfnProgress = PrepareProgressCallbackData( ptCmd->ptProgressData, &tCallbackData);

          if( CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelDownload(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                 ptCmd->ulMode,
                                                 ptCmd->pszFileName,
                                                 (unsigned char*)ptCmd->pvFileData,
                                                 ptCmd->ulFileSize,
                                                 pfnProgress,      /* Progress callback */
                                                 pfnRecv,          /* Receive callback */
                                                 &tCallbackData);  /* pvUser */
          }

          CleanRecvCallbackData(&tCallbackData);
          CleanProgressCallbackData(&tCallbackData);
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Upload */
    case CIFx_IOCTL_CH_UPLOAD:
      {
        /* Download to the device via the system channel */
        DEVxIO_UPLOAD*            ptCmd         = (DEVxIO_UPLOAD*)ioBuffer;

        PFN_PROGRESS_CALLBACK     pfnProgress   = NULL;
        PFN_RECV_PKT_CALLBACK     pfnRecv       = NULL;

        CALLBACK_DATA_T           tCallbackData;
        OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

        if( NULL != ptCmd->pvFileData)
        {

          tCallbackData.ptDevExt = ptDrvDevInst;

          /* Create the receive callback data if requested */
          pfnRecv = PrepareRecvCallbackData( ptCmd->ptRecvData, &tCallbackData);

          /* Create the progress callback data if requested */
          pfnProgress = PrepareProgressCallbackData( ptCmd->ptProgressData, &tCallbackData);


          if (CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelUpload(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                               ptCmd->ulMode,
                                               ptCmd->pszFileName,
                                               (uint8_t*)ptCmd->pvFileData,
                                               ptCmd->pulFileSize,
                                               pfnProgress,      /* Progress callback */
                                               pfnRecv,          /* Receive callback */
                                               &tCallbackData);  /* pvUser */
          }

          CleanRecvCallbackData(&tCallbackData);
          CleanProgressCallbackData(&tCallbackData);
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Find First File */
    case CIFx_IOCTL_CH_FINDFIRST:
      {
        /* Download to the device via the system channel */
        DEVxIO_FIND_FILE*         ptCmd   = (DEVxIO_FIND_FILE*)ioBuffer;

        PFN_RECV_PKT_CALLBACK     pfnRecv = NULL;

        CALLBACK_DATA_T           tCallbackData;
        OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

        if( NULL != ptCmd->ptDirectoryInfo)
        {
          tCallbackData.ptDevExt = ptDrvDevInst;

          /* Create the receive callback data if requested */
          pfnRecv = PrepareRecvCallbackData( ptCmd->ptRecvData, &tCallbackData);

          if( CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelFindFirstFile(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                      ptCmd->ptDirectoryInfo,
                                                      pfnRecv,
                                                      &tCallbackData);
          }

          /* Close events */
          CleanRecvCallbackData(&tCallbackData);
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Find Next File */
    case CIFx_IOCTL_CH_FINDNEXT:
      {
        /* Download to the device via the system channel */
        DEVxIO_FIND_FILE*         ptCmd   = (DEVxIO_FIND_FILE*)ioBuffer;

        PFN_RECV_PKT_CALLBACK     pfnRecv = NULL;

        CALLBACK_DATA_T           tCallbackData;
        OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

        if( NULL != ptCmd->ptDirectoryInfo)
        {
          tCallbackData.ptDevExt = ptDrvDevInst;

          /* Create the receive callback data if requested */
          pfnRecv = PrepareRecvCallbackData( ptCmd->ptRecvData, &tCallbackData);

          if( CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelFindNextFile(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                     ptCmd->ptDirectoryInfo,
                                                     pfnRecv,
                                                     &tCallbackData);
          }

          /* Close events */
          CleanRecvCallbackData(&tCallbackData);
        }else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Register notification */
    case CIFx_IOCTL_CH_NOTIFY_REG:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_NOTIFICATION* ptCmd  = (DEVxIO_CHANNEL_NOTIFICATION*)ioBuffer;

        if( 0 != OS_Strlen(ptCmd->szEventName))
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            if(ptCmd->ulNotification >= CIFX_NOTIFY_MAX)
            {
              *(ptCmd->plError) = CIFX_INVALID_PARAMETER;
            } else
            {
              CALLBACK_REGISTRATION_T* ptRegistration = &ptDrvDevInst->aatNotifyCallbacks[ptCmd->ulChannel][ptCmd->ulNotification];

              if(NULL != ptRegistration->hUserEvent)
              {
                *(ptCmd->plError) = CIFX_CALLBACK_ALREADY_USED;
              } else if (NULL == (ptRegistration->hUserEvent = PrepareNotificationEvent(ptCmd->szEventName)))
              {
                *(ptCmd->plError) = CIFX_CALLBACK_CREATE_EVENT_FAILED;
              } else
              {
                *(ptCmd->plError) = xChannelRegisterNotification(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                                 ptCmd->ulNotification,
                                                                 NotifyCallbackWrapper,
                                                                 ptRegistration);
                if(CIFX_NO_ERROR != *(ptCmd->plError))
                {
                  ptRegistration->hUserEvent      = NULL;
                }
              }
            }
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Unregister notification */
    case CIFx_IOCTL_CH_NOTIFY_UNREG:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_NOTIFICATION* ptCmd  = (DEVxIO_CHANNEL_NOTIFICATION*)ioBuffer;

        if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
        {
          if(ptCmd->ulNotification >= CIFX_NOTIFY_MAX)
          {
            *(ptCmd->plError) = CIFX_INVALID_PARAMETER;
          } else
          {
            CALLBACK_REGISTRATION_T* ptRegistration = &ptDrvDevInst->aatNotifyCallbacks[ptCmd->ulChannel][ptCmd->ulNotification];

            if(NULL == ptRegistration->hUserEvent)
            {
              *(ptCmd->plError) = CIFX_CALLBACK_NOT_REGISTERED;
            } else
            {
              *(ptCmd->plError) = xChannelUnregisterNotification(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                                 ptCmd->ulNotification);

              if(CIFX_NO_ERROR == *ptCmd->plError)
              {
                ptRegistration->ulNotifyDataLen = 0;
                CleanNotificationEvent(ptRegistration->hUserEvent);
                ptRegistration->hUserEvent      = NULL;
              }
            }
          }
        }
      }
      break;

    case CIFx_IOCTL_CH_NOTIFY_DATA:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_NOTIFY_DATA* ptCmd  = (DEVxIO_CHANNEL_NOTIFY_DATA*)ioBuffer;

        if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
        {
          CALLBACK_REGISTRATION_T* ptCallback = &ptDrvDevInst->aatNotifyCallbacks[ptCmd->ulChannel][ptCmd->ulNotification];

          OS_Memcpy(ptCmd->abData,
                    ptCallback->abNotifyData,
                    (uint32_t)ptCallback->ulNotifyDataLen);

          ptCmd->ulDataLen  = (uint32_t)ptCallback->ulNotifyDataLen;
          *(ptCmd->plError) = CIFX_NO_ERROR;
        }
      }
      break;

    /*-------------------------------------------*/
    /* Channel: Signal sync state */
    case CIFx_IOCTL_CH_SYNC_STATE:
      {
        /* Get the command packet pointer */
        DEVxIO_CHANNEL_SYNCSTATE* ptCmd  = (DEVxIO_CHANNEL_SYNCSTATE*)ioBuffer;

        if( (NULL != ptCmd->pulSyncCount) &&
            (NULL != ptCmd->pulSyncCount))
        {
          if(CheckChannelState(ptDrvDevInst, ptCmd->ulChannel, ptCmd->plError))
          {
            *(ptCmd->plError) = xChannelSyncState(ptDrvDevInst->ahChannels[ptCmd->ulChannel],
                                                  ptCmd->ulCmd,
                                                  ptCmd->ulTimeout,
                                                  ptCmd->pulSyncCount);
          }
        } else
        {
          *(ptCmd->plError) = CIFX_INVALID_POINTER;
        }
      }
      break;

      /*-------------------------------------------*/
      /* Channel: NXSB DPM info */
      case CIFx_IOCTL_NXSB_DPMINFO:
        {
          DEVxIO_NXSB_MEMPTR* ptNXSB = (DEVxIO_NXSB_MEMPTR*)ioBuffer;

          if( NULL != ptNXSB->ppvDPMPointer)
          {
            ptNXSB->pvHandle        = NULL;
            *(ptNXSB->ppvDPMPointer)= (void*)*((int**)&ptDrvDevInst->tDeviceInst.ulDPMSize);
            ptNXSB->ulDataLength    = ptDrvDevInst->tDeviceInst.ulDPMSize;

            *(ptNXSB->plError) = CIFX_NO_ERROR;
          } else
          {
            *(ptNXSB->plError) = CIFX_INVALID_POINTER;
          }
        }
        break;

      /*-------------------------------------------*/
      case CIFx_IOCTL_NXSB_MAP:
        {
          DEVxIO_NXSB_MEMPTR* ptNXSB = (DEVxIO_NXSB_MEMPTR*)ioBuffer;

          if( NULL != ptNXSB->ppvDPMPointer)
          {
            ptNXSB->pvHandle = OS_MapUserPointer(ptDrvDevInst->tDeviceInst.pbDPM,
                                                 ptNXSB->ulDataLength,
                                                 ptNXSB->ppvDPMPointer,
                                                 (void*)ptDrvDevInst);
            if( NULL == ptNXSB->pvHandle)
              *(ptNXSB->plError) = CIFX_MEMORY_MAPPING_FAILED;

          } else
          {
            *(ptNXSB->plError) = CIFX_INVALID_POINTER;
          }
        }
        break;

      /*-------------------------------------------*/
      case CIFx_IOCTL_NXSB_UNMAP:
        {
          DEVxIO_NXSB_MEMPTR* ptNXSB = (DEVxIO_NXSB_MEMPTR*)ioBuffer;

          if( (NULL != ptNXSB->ppvDPMPointer) &&
              (NULL != ptNXSB->pvHandle))
          {
            if( !OS_UnmapUserPointer(ptNXSB->ppvDPMPointer, NULL))
            {
               *(ptNXSB->plError) = CIFX_INVALID_HANDLE;
            } else
            {
              ptNXSB->pvHandle                 = NULL;
              *(ptNXSB->ppvDPMPointer)         = NULL;
            }
          } else
          {
            *(ptNXSB->plError) = CIFX_INVALID_POINTER;
          }
        }
        break;

    /*-------------------------------------------*/
    default:
      /* Unrecognized IOCTL request */
      retVal = ERROR;
      break;
  }

  return (retVal);
}


