/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: cifXFunctions_Internal.c 6954 2015-07-10 13:10:34Z Robert $:

  Description:
    CIFX API internal function implementation

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2015-06-12  created

**************************************************************************************/

/*****************************************************************************/
/*! \file cifXFunctions_internal.c
*    CIFX API internal function implementation                               */
/*****************************************************************************/

#include "OS_Includes.h"

#include "cifXUser.h"
#include "cifXErrors.h"
#include "cifXDrv_ioctl.h"
#include "cifXFunctions_Internal.h"
#include <taskLib.h>

/* taskspawn() default parameters */
#define CALLBACK_STACKSIZE 0x2000

/*! Gets the current priority of calling task and returns a higher priority
*   used for callback tasks */
int Callback_Priority(void)
{
  int lPriority;
  taskPriorityGet(taskIdSelf(), &lPriority);
  return lPriority - 1;
}

CIFX_EVENT_NAMES CIFXNotifyEventNames[] = {
{NULL}, /* CIFX API notifications start at 1 */
{"MBXFUL"},/* CIFX_NOTIFY_RX_MBX_FULL */
{"MBXEMP"},/* CIFX_NOTIFY_TX_MBX_EMPTY */
{"PD0_IN"},/* CIFX_NOTIFY_PD0_IN */
{"PD1_IN"},/* CIFX_NOTIFY_PD1_IN */
{"PD0_OU"},/* CIFX_NOTIFY_PD0_OUT */
{"PD1_OU"},/* CIFX_NOTIFY_PD1_OUT */
{"NTSYNC"},/* CIFX_NOTIFY_SYNC */
{"NTCOMS"},/* CIFX_NOTIFY_COM_STATE */
};

CIFX_EVENT_NAMES CIFXCallbackEventNames[] = {
{NULL},   /* Semaphore IDs start at 1 */
{"RECVIN"}, /* Semaphore for callback receive IN */
{"RECVOU"}, /* Semaphore for callback receive OUT */
{"PROGIN"}, /* Semaphore for callback progress IN */
{"PROGOU"}  /* Semaphore for callback progress OUT */
};

/*****************************************************************************/
/*! Create a name (16 Bytes) used to create named events
*   \param ptEventNames   List of possible names
*   \param ulEventID    Event type to select the name
*   \param fd       Device for which the event name should be created
*   \param ulNameBufferSize Size of the name buffer
*   \param szNameBuffer     Pointer to the name buffer                       */
/*****************************************************************************/
void CreateEventName(CIFX_EVENT_NAMES* ptEventNames, uint32_t ulEventID, int fd, uint32_t ulNameBufferSize, char* szNameBuffer)
{
  if ((NULL != szNameBuffer) && (ulNameBufferSize >= NOTIFICATION_STRING_LENGTH))
  {
  /* Take the event name from the event list and add the device handle */
  snprintf(szNameBuffer,
       (size_t)NOTIFICATION_STRING_LENGTH,
      "/%s%08x", ptEventNames[ ulEventID].str, fd);
  }
}

/*****************************************************************************/
/*! Notify callback wrapper thread for system notifications.
*   \param pvParam  Pointer to callback information (PNOTIFY_CALLBACK_WRAPPER_T)
*   \return 0                                                                */
/*****************************************************************************/
static uint32_t NotifyCallbackThread(void* pvParam)
{
  PNOTIFY_CALLBACK_DATA_T ptCallback = (PNOTIFY_CALLBACK_DATA_T)pvParam;

  ptCallback->fCallbackRunning = TRUE;

  do
  {
  /* Wait on driver notification */
    if ( (CIFX_EVENT_SIGNALLED == OS_WaitEvent(ptCallback->hEvent, WAIT_FOREVER)) &&
         (TRUE                 == ptCallback->fCallbackRunning)                      )
    {
      /* Notification callback event signaled by driver*/
      /* Check if we have to do a data fetch via a separate callback */
      if(ptCallback->pfnNotify)
      {
        void*                       pvData      = NULL;
        uint32_t                    ulDataLen   = 0;
        DEVxIO_CHANNEL_NOTIFY_DATA  tNotifyData = {0};

        switch(ptCallback->ulNotification)
        {
        case CIFX_NOTIFY_RX_MBX_FULL:
        case CIFX_NOTIFY_TX_MBX_EMPTY:
        case CIFX_NOTIFY_COM_STATE:

          tNotifyData.ulChannel      = ptCallback->ulChannel;
          tNotifyData.ulNotification = ptCallback->ulNotification;

          /* Process the data fetch */
          if(CIFX_NO_ERROR == (xDeviceIoControl(ptCallback->hDevice,
                                                CIFx_IOCTL_CH_NOTIFY_DATA,
                                                &tNotifyData)))
          {
            pvData    = tNotifyData.abData;
            ulDataLen = tNotifyData.ulDataLen;
          }
          break;

        default:
          break;
        }

        /* Call the user notification function */
        ptCallback->pfnNotify( ptCallback->ulNotification,
                               ulDataLen,
                               pvData,
                               ptCallback->pvUser);
      }
    }

  } while(ptCallback->fCallbackRunning);

  return 0;
}

/*****************************************************************************/
/*! Create all needed resources for functions that provide a callback for
*   unexpected packets and a progress callback
*   \param pfnNotifyCallback Notify function
*   \param ptIoctl           IOCTL Data
*   \param pvUser            Pointer passed on user callback
*   \return != NULL on success                                               */
/*****************************************************************************/
int32_t CreateNotifyCallback( CIFXHANDLE                   hDevice,
                              DEVxIO_CHANNEL_NOTIFICATION* ptIoctl,
                              PFN_NOTIFY_CALLBACK          pfnCallback,
                              void*                        pvUser,
                              PNOTIFY_CALLBACK_DATA_T*  pptCallbackData)
{
  int32_t lRet = CIFX_NO_ERROR;

  PNOTIFY_CALLBACK_DATA_T ptCallbackDataInternal = (PNOTIFY_CALLBACK_DATA_T)OS_Memalloc(sizeof(NOTIFY_CALLBACK_DATA_T));

  if(NULL == ptCallbackDataInternal)
  {
    lRet = CIFX_FUNCTION_FAILED;
  } else
  {
    OS_Memset(ptCallbackDataInternal, 0, sizeof(NOTIFY_CALLBACK_DATA_T));

    /* Insert CIFX device specific information */
    ptCallbackDataInternal->hDevice        = hDevice;
    ptCallbackDataInternal->ulChannel      = ptIoctl->ulChannel;
    ptCallbackDataInternal->ulNotification = ptIoctl->ulNotification;
    ptCallbackDataInternal->fCallbackRunning = FALSE;

    /* Store local thread information */
    ptCallbackDataInternal->pfnNotify      = pfnCallback;
    ptCallbackDataInternal->pvUser         = pvUser;

    /* Create the event handle */
    if( NULL == (ptCallbackDataInternal->hEvent = OS_CreateNamedEvent(ptIoctl->szEventName)))
    {
      /* Needed events could not be created */
      lRet = CIFX_CALLBACK_CREATE_EVENT_FAILED;

    /* Create the local notification callback task */
    } else if (NULL == (ptCallbackDataInternal->hThread = taskSpawn(NULL,             /* Task notify callback */
                                  Callback_Priority(),
                                  0,
                                  CALLBACK_STACKSIZE,
                                  (FUNCPTR)NotifyCallbackThread,
                                  (_Vx_usr_arg_t)ptCallbackDataInternal,0,0,0,0,0,0,0,0,0)))
    {
      /* Callback thread could not be created */
      lRet = CIFX_CALLBACK_CREATE_EVENT_FAILED;

    } else
    {
      /* Use higher priority if working with sync notifications */
      if( CIFX_NOTIFY_SYNC == ptCallbackDataInternal->ulNotification)
        taskPrioritySet(ptCallbackDataInternal->hThread, Callback_Priority()-1);

      /* Wait until thread is running */
      while( FALSE == ptCallbackDataInternal->fCallbackRunning)
      {
        OS_Sleep(10);
      }
    }
  }

  /* Deallocate all resources on error */
  if(CIFX_NO_ERROR != lRet)
  {
    /* Remove all notify data including the event */
    if(NULL != ptCallbackDataInternal->hThread)
      RemoveNotifyCallback(ptCallbackDataInternal);

    *pptCallbackData = NULL;

  } else
  {
    /* Return the created notify data information */
    *pptCallbackData = ptCallbackDataInternal;
  }

  return lRet;
}

/*! **************************************************************************
* Remove notify callback thread and resources
*   \param ptThreadInfo  Callback thread information
******************************************************************************/
void RemoveNotifyCallback(PNOTIFY_CALLBACK_DATA_T ptThreadInfo)
{
  /* Terminate callback thread, if it was created */
  if( NULL != ptThreadInfo)
  {
    if(NULL != ptThreadInfo->hThread)
    {
      ptThreadInfo->fCallbackRunning = FALSE;

      OS_SetEvent(ptThreadInfo->hEvent);
      OS_Sleep(20);
      taskWait(ptThreadInfo->hThread, WAIT_FOREVER);
      ptThreadInfo->hThread = NULL;
    }

    /* Delete Event */
    if(NULL != ptThreadInfo->hEvent)
    {
      OS_DeleteNamedEvent(ptThreadInfo->hEvent);
      ptThreadInfo->hEvent = NULL;
    }

    /* Free allocated resource */
    OS_Memfree(ptThreadInfo);
  }
}

/*! **************************************************************************
*   Receive callback wrapper thread for unexpected packets (e.g. FindFirst/FindNext)
*   \param pvParam  Pointer to callback information (PCALLBACK_WRAPPER_T)
*   \return 0
******************************************************************************/
static uint32_t ReceiveCallbackThread(void* pvParam)
{
  PPROGRESS_RECV_CALLBACK_DATA_T ptCallback = (PPROGRESS_RECV_CALLBACK_DATA_T)pvParam;

  ptCallback->fRecvClbkRunning = TRUE;

  do
  {
    if ( (CIFX_EVENT_SIGNALLED == OS_WaitEvent( ptCallback->pvRecvEventIN, WAIT_FOREVER)) &&
         (TRUE                 == ptCallback->fRecvClbkRunning)                      )
    {
      /* Receive callback event signaled by driver*/
      if(ptCallback->pfnRecvClbk)
      {
        /* Data can be consumed by user */
          ptCallback->pfnRecvClbk((CIFX_PACKET*)ptCallback->tRecvData.pvBuffer,
                            ptCallback->pvRecvUser);
      }

      /* Tell the driver we are done with the callback */
      OS_SetEvent(ptCallback->pvRecvEventOUT);
    }

  } while(ptCallback->fRecvClbkRunning);

  return 0;
}

/*! **************************************************************************
*   Progress and Receive callback wrapper thread for
*   unexpected packets and download progress (e.g. Download/Upload)
*   \param pvParam  Pointer to callback information (PCALLBACK_WRAPPER_T)
*   \return 0
******************************************************************************/
static uint32_t ProgressCallbackThread(void* pvParam)
{
  PPROGRESS_RECV_CALLBACK_DATA_T ptCallback = (PPROGRESS_RECV_CALLBACK_DATA_T)pvParam;

  ptCallback->fProgressClbkRunning = TRUE;

  do
  {
    if ( (CIFX_EVENT_SIGNALLED == OS_WaitEvent( ptCallback->pvProgressEventIN, WAIT_FOREVER)) &&
         (TRUE                 == ptCallback->fProgressClbkRunning)                      )
    {
      /* Progress callback event signaled by driver*/
      if(ptCallback->pfnProgressClbk)
      {
        ptCallback->pfnProgressClbk(ptCallback->tProgressData.ulStep,
                                    ptCallback->tProgressData.ulMaxStep,
                                    ptCallback->pvProgressUser,
                                    ptCallback->tProgressData.bFinished,
                                    ptCallback->tProgressData.lError);
      }

      /* Tell the driver we are done with the callback */
      OS_SetEvent(ptCallback->pvProgressEventOUT);
    }

  } while(ptCallback->fProgressClbkRunning);

  return 0;
}

/*! **************************************************************************
*   Create all needed resources for functions that only
*   provide a callback for unexpected packets
*   \param pfnRecvPktCallback  Function to call
*   \param ptRecvData          IOCTL Data
*   \param ptThreadInfo        Callback thread information
*   \param pvUser              Pointer passed on user callback
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t CreateReceiveCallback(PFN_RECV_PKT_CALLBACK         pfnRecvPktCallback,
                              PROGRESS_RECV_CALLBACK_DATA_T*    ptThreadInfo,
                              void*                           pvUser)
{
  int32_t lRet = CIFX_NO_ERROR;

  /* Only create resources, if callback is requested */
  if(NULL == pfnRecvPktCallback)
    /* User don't want a callback */
    return CIFX_NO_ERROR;

  ptThreadInfo->pvRecvUser  = pvUser;
  ptThreadInfo->pfnRecvClbk = pfnRecvPktCallback;

  /* NOTE: This limits the packet to 1600 bytes. If the mailbox
           is larger in the future, it might be needed to check
           the mailbox size */

  ptThreadInfo->tRecvData.ulBufferLen = (uint32_t)sizeof(CIFX_PACKET);
  if(NULL == (ptThreadInfo->tRecvData.pvBuffer = OS_Memalloc( ptThreadInfo->tRecvData.ulBufferLen)) )
  {
    /* Out of memory. Buffer for packet creation error */
    lRet = CIFX_CALLBACK_CREATE_RECV_BUFFER;

  } else if( (NULL == (ptThreadInfo->pvRecvEventIN  = OS_CreateNamedEvent(ptThreadInfo->tRecvData.szEventIN))) ||
             (NULL == (ptThreadInfo->pvRecvEventOUT = OS_CreateNamedEvent(ptThreadInfo->tRecvData.szEventOUT)))  )
  {
    /* Needed events could not be created */
    lRet = CIFX_CALLBACK_CREATE_EVENT_FAILED;

  } else if (NULL == (ptThreadInfo->hRecvThread = taskSpawn(NULL, /* Task receive callback */
                                                            Callback_Priority(),
                                                            0,
                                                            CALLBACK_STACKSIZE,
                                                            (FUNCPTR)ReceiveCallbackThread,
                                                            (_Vx_usr_arg_t)ptThreadInfo,0,0,0,0,0,0,0,0,0)))
  {
    /* Callback wrapper thread could not be created */
    lRet = CIFX_CALLBACK_CREATE_EVENT_FAILED;
  }else
  {
    /* Wait until thread is running */
    while( FALSE == ptThreadInfo->fRecvClbkRunning)
    {
      OS_Sleep(10);
    }
  }

  /* Deallocate all resources on error */
  if(CIFX_NO_ERROR != lRet)
  {
    RemoveReceiveCallback(ptThreadInfo);
  }

  return lRet;
}

/*! **************************************************************************
*   Create all needed resources for functions using a progress callback
*   \param pfnProgessCallback  Function to call
*   \param ptProgressData      IOCTL Data
*   \param ptThreadInfo        Callback thread information
*   \param pvUser              Pointer passed on user callback
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t CreateProgressCallback         ( PFN_PROGRESS_CALLBACK        pfnProgressCallback,
                                         PROGRESS_RECV_CALLBACK_DATA_T* ptThreadInfo,
                                         void*                        pvProgressUser)
{
  int32_t lRet = CIFX_NO_ERROR;

  /* Only create resources, if callback is requested */
  if( NULL == pfnProgressCallback)
    /* User don't want a callback */
    return CIFX_NO_ERROR;

  ptThreadInfo->pvProgressUser  = pvProgressUser;
  ptThreadInfo->pfnProgressClbk = pfnProgressCallback;

  if( (NULL == (ptThreadInfo->pvProgressEventIN  = OS_CreateNamedEvent(ptThreadInfo->tProgressData.szEventIN) )) ||
      (NULL == (ptThreadInfo->pvProgressEventOUT = OS_CreateNamedEvent(ptThreadInfo->tProgressData.szEventOUT) ))  )
  {
    /* Needed events could not be created */
    lRet = CIFX_CALLBACK_CREATE_EVENT_FAILED;

  } else if (NULL == (ptThreadInfo->hProgressThread = taskSpawn(NULL,
                                                                Callback_Priority(),
                                                                0,
                                                                CALLBACK_STACKSIZE,
                                                                (FUNCPTR)ProgressCallbackThread,
                                                                (_Vx_usr_arg_t)ptThreadInfo,0,0,0,0,0,0,0,0,0)))
  {
    /* Callback wrapper thread could not be created */
    lRet = CIFX_CALLBACK_CREATE_EVENT_FAILED;
  }else
  {
    /* Wait until thread is running */
    while( FALSE == ptThreadInfo->fProgressClbkRunning)
    {
      OS_Sleep(10);
    }
  }

  /* Deallocate all resources on error */
  if(CIFX_NO_ERROR != lRet)
  {
    RemoveProgressCallback(ptThreadInfo);
  }

  return lRet;
}

/*! **************************************************************************
*   Remove PROGRESS callback thread resources
*   \param ptThreadInfo        Callback thread information
******************************************************************************/
void RemoveProgressCallback(PROGRESS_RECV_CALLBACK_DATA_T* ptThreadInfo)
{
  if( NULL != ptThreadInfo)
  {
  /*=========================================*/
    /* Terminate PROGRESS thread               */
  /* and free resource                       */
  /*=========================================*/
    if(NULL != ptThreadInfo->hProgressThread)
    {
      ptThreadInfo->fProgressClbkRunning = FALSE;
      OS_SetEvent(ptThreadInfo->pvProgressEventIN);
      OS_Sleep(20);
      taskWait(ptThreadInfo->hProgressThread, WAIT_FOREVER);
      ptThreadInfo->hProgressThread = NULL;
    }

    /* Delete Progress callback events */
    if(NULL != ptThreadInfo->pvProgressEventIN)
    {
    OS_DeleteNamedEvent(ptThreadInfo->pvProgressEventIN);
    ptThreadInfo->pvProgressEventIN = NULL;
    }

    if(NULL != ptThreadInfo->pvProgressEventOUT)
    {
    OS_DeleteNamedEvent(ptThreadInfo->pvProgressEventOUT);
    ptThreadInfo->pvProgressEventOUT = NULL;
    }
  }
}

/*! **************************************************************************
*   Remove PROGRESS callback thread resources
*   \param ptThreadInfo        Callback thread information
******************************************************************************/
void RemoveReceiveCallback(PROGRESS_RECV_CALLBACK_DATA_T* ptThreadInfo)
{
  if( NULL != ptThreadInfo)
  {
  /*=========================================*/
    /* Terminate RECEIVE thread                */
  /* and free resource                       */
  /*=========================================*/
    if(NULL != ptThreadInfo->hRecvThread)
    {
      ptThreadInfo->fRecvClbkRunning = FALSE;
      OS_SetEvent(ptThreadInfo->pvRecvEventIN);
      OS_Sleep(20);
      taskWait(ptThreadInfo->hRecvThread, WAIT_FOREVER);
      ptThreadInfo->hRecvThread = NULL;
    }

    /* Delete Receive callback events */
    if(NULL != ptThreadInfo->pvRecvEventIN)
    {
      OS_DeleteNamedEvent(ptThreadInfo->pvRecvEventIN);
      ptThreadInfo->pvRecvEventIN = NULL;
    }

    if(NULL != ptThreadInfo->pvRecvEventOUT)
    {
      OS_DeleteNamedEvent(ptThreadInfo->pvRecvEventOUT);
      ptThreadInfo->pvRecvEventOUT = NULL;
    }

    /* Delete Receive callback data buffer */
    if(NULL != ptThreadInfo->tRecvData.pvBuffer)
    {
      OS_Memfree(ptThreadInfo->tRecvData.pvBuffer);
      ptThreadInfo->tRecvData.pvBuffer = NULL;
    }
  }
}
