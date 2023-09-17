/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: cifXFunctions_Internal.h 6955 2015-07-10 13:22:25Z Robert $:

  Description:
    CIFX API internal function implementation

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2015-06-12  created

**************************************************************************************/

/*****************************************************************************/
/*! \addtogroup CIFX_DRIVER_API cifX Driver API implementation               */
/*! \{                                                                       */
/*****************************************************************************/

/* prevent multiple inclusion */
#ifndef __CIFX_INTERNAL__H
#define __CIFX_INTERNAL__H

/*---------------------------------------------*/
/* Definition for the notify callback handling */
/*---------------------------------------------*/
typedef struct PROGRESS_RECV_CALLBACK_DATA_Ttag
{
  TASK_ID                   hRecvThread;
  BOOL                      fRecvClbkRunning;
  PFN_RECV_PKT_CALLBACK     pfnRecvClbk;
  IOCTL_RECV_CALLBACK_T     tRecvData;
  void*                     pvRecvUser;
  void*                     pvRecvEventIN;
  void*                     pvRecvEventOUT;

  TASK_ID                   hProgressThread;
  BOOL                      fProgressClbkRunning;
  IOCTL_PROGRESS_CALLBACK_T tProgressData;
  PFN_PROGRESS_CALLBACK     pfnProgressClbk;
  void*                     pvProgressUser;
  void*                     pvProgressEventIN;
  void*                     pvProgressEventOUT;


} PROGRESS_RECV_CALLBACK_DATA_T, *PPROGRESS_RECV_CALLBACK_DATA_T;

int32_t CreateReceiveCallback     ( PFN_RECV_PKT_CALLBACK      pfnRecvPktCallback,
                                    PROGRESS_RECV_CALLBACK_DATA_T* ptThreadInfo,
                                    void*                      pvUser);

int32_t CreateProgressCallback    ( PFN_PROGRESS_CALLBACK        pfnProgressCallback,
                                    PROGRESS_RECV_CALLBACK_DATA_T* ptThreadInfo,
                                    void*                        pvProgressUser);

void RemoveProgressCallback       ( PROGRESS_RECV_CALLBACK_DATA_T* ptThreadInfo);
void RemoveReceiveCallback        ( PROGRESS_RECV_CALLBACK_DATA_T* ptThreadInfo);

/*---------------------------------------------*/
/* Definition for the notify callback handling */
/*---------------------------------------------*/
typedef struct NOTIFY_CALLBACK_DATA_Ttag
{
  TASK_ID                   hThread;            /* Notify thread */
  BOOL                      fCallbackRunning;   /* Notify thread stop indication */
  void*                     hEvent;             /* Our event handle */
  PFN_NOTIFY_CALLBACK       pfnNotify;          /* Callback function */
  void*                     pvUser;             /* Thread user data */
  /* Data necessary to read data via separate IOControl to the driver */
  CIFXHANDLE                hDevice;            /* CIFX device handle */
  uint32_t                  ulChannel;          /* CIFX device channel number */
  uint32_t                  ulNotification;

} NOTIFY_CALLBACK_DATA_T, *PNOTIFY_CALLBACK_DATA_T;

int32_t CreateNotifyCallback( CIFXHANDLE                    hDevice ,
                              DEVxIO_CHANNEL_NOTIFICATION*  ptIoctl ,
                              PFN_NOTIFY_CALLBACK           pfnCallback ,
                              void*                         pvUser,
                              PNOTIFY_CALLBACK_DATA_T*      pptCallbackData);

void RemoveNotifyCallback   ( NOTIFY_CALLBACK_DATA_T*    ptThreadInfo);

int32_t xDeviceIoControl(CIFXHANDLE hDevice, int dwCode, void* pvInput);

/* Node structure for list (notifications) */
typedef struct LST_NODE_Ttag
{
  struct LST_NODE_T*        ptNext;
  struct LST_NODE_T*        ptPrevious;
  PNOTIFY_CALLBACK_DATA_T ptCallback;

} LST_NODE_T, *PLST_NODE_T;

/* Channel Handle Structure */
typedef struct CHANNEL_HANDEL_Ttag
{
  CIFXHANDLE hDevice;
  uint32_t   ulChannel;

  void*      pvNotificationLock;
  LIST       lstNotifications;
} CHANNEL_HANDLE_T, *PCHANNEL_HANDLE_T;

typedef struct CIFX_EVENT_NAMESTag{
  char *   str;             /* String */
} CIFX_EVENT_NAMES;

#define CALLBACK_ID_RECEIVE_IN    1   /* Lookup value for named event Receive IN */
#define CALLBACK_ID_RECEIVE_OUT   2   /* Lookup value for named event Receive OUT */
#define CALLBACK_ID_PROGRESS_IN   3   /* Lookup value for named event Progress IN */
#define CALLBACK_ID_PROGRESS_OUT  4   /* Lookup value for named event Progress OUT */

void CreateEventName(CIFX_EVENT_NAMES* ptEventNames, uint32_t ulEventID, int fd, uint32_t ulNameBufferSize, char* szNameBuffer);

#endif
