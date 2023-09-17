/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: cifXFunctions.c 7056 2015-07-17 12:15:35Z LuisContreras $:

  Description:
    cifX API function implementation

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2015-06-12  created

**************************************************************************************/

/*****************************************************************************/
/*! \file cifX API function implementation
*    cifX API function implementation                                        */
/*****************************************************************************/

#include "OS_Includes.h"

#include "cifXErrors.h"
#include "cifXerrorsstr.h"
#include "cifXUser.h"
#include "cifXDrv_ioctl.h"        /* structure definitions for IOctls */
#include "cifXFunctions_Internal.h"   /* helper functions for callbacks (notify, recv, progress) */

extern CIFX_EVENT_NAMES CIFXNotifyEventNames[];
extern CIFX_EVENT_NAMES CIFXCallbackEventNames[];

/*! **************************************************************************
* Generic IO Control, evaluating error info which
* is expected to be the first int32_t in the input data
*   \param hDevice      Device handle
*   \param dwCode       IOControl code
*   \param pvInput      Input buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t xDeviceIoControl(CIFXHANDLE hDevice,
                         int        ulCode,
                         void*      pvInput)
{
  int32_t       lRet        = CIFX_DRV_IO_CONTROL_FAILED;
  DEVxIO_BASE*  ptBaseCtrl  = (DEVxIO_BASE*)pvInput;

  ptBaseCtrl->plError = (int32_t*)&lRet;

  if(NULL == hDevice)
  {
    return CIFX_DRV_IO_CONTROL_FAILED;
  }

  if(OK != ioctl( *((int *)&hDevice), ulCode, (_Vx_ioctl_arg_t)pvInput))
  {
    lRet = CIFX_DRV_IO_CONTROL_FAILED;
  }

  return lRet;
}

/*****************************************************************************/
/*! Open a connection to the driver
*   \param phDriver     Returned handle to the driver
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xDriverOpen(CIFXHANDLE* phDriver)
{
  int32_t lRet = CIFX_NO_ERROR;
  long    fd   = 0;

  if(NULL == phDriver)
    return CIFX_INVALID_POINTER;

  /* File descriptor will be cifX handle */
  /* Open driver via regular open() function, returns file descriptor */
  fd = (long)open( "/cifX0", 0, 0);

  if( ERROR == fd)
  {
    lRet = CIFX_DRV_DRIVER_NOT_LOADED;
    *phDriver = NULL;
  }else
  {
    *phDriver = (CIFXHANDLE)fd;
  }

  return lRet;
}

/*****************************************************************************/
/*! Close a connection to the driver
*   \param hDriver     Handle to connection, that is being closed
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xDriverClose(CIFXHANDLE hDriver)
{
  int32_t lRet = CIFX_NO_ERROR;
  int     fd   = 0;

  if(NULL == hDriver)
    return CIFX_INVALID_HANDLE;

  fd = *((int *)&hDriver);

  if( ERROR != fd)
   (void)close( fd);
  else
   lRet = CIFX_INVALID_BOARD;

  return lRet;
}

/*! **************************************************************************
* Query information about the running device driver
*   \param hDriver      Driver handle, if NULL the driver will be temporarily opened by the API
*   \param ulSize       Buffer size
*   \param ptDriverInfo Returned Driver information
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xDriverGetInformation(CIFXHANDLE hDriver, uint32_t ulSize, void* pvDriverInfo)
{
  int32_t lRet         = CIFX_NO_ERROR;
  BOOL    fCloseDriver = FALSE;

  if(NULL == pvDriverInfo)
    return CIFX_INVALID_POINTER;

  if(NULL == hDriver)
  {
    lRet = xDriverOpen(&hDriver);
    fCloseDriver = TRUE;
  }

  if(CIFX_NO_ERROR == lRet)
  {
    DEVxIO_DRIVER_INFO tDriverInfo = {0};

    tDriverInfo.ptDriverInfo = (DRIVER_INFORMATION*)pvDriverInfo;
    tDriverInfo.ulSize       = ulSize;

    lRet = xDeviceIoControl(hDriver,
                            CIFx_IOCTL_DRV_INFO,
                            &tDriverInfo);

    if(fCloseDriver)
      xDriverClose(hDriver);
  }

  return lRet;
}

/*! **************************************************************************
* Enumerate over all available Boards
*   \param hDriver      Driver handle, if NULL the driver will be temporarily opened by the API
*   \param ulBoard      Should be counted from 0 upwards on enumeration
*   \param ulSize       Size of the user buffer
*   \param pvBoardInfo  Returned board information
*   \return CIFX_NO_ERROR on success, DRV_NO_MORE_BOARDS when ulBoard is bigger than found board count
******************************************************************************/
int32_t APIENTRY xDriverEnumBoards(CIFXHANDLE hDriver, uint32_t ulBoard, uint32_t ulSize, void* pvBoardInfo)
{
  if(NULL == pvBoardInfo)
    return CIFX_INVALID_POINTER;

  int32_t lRet = CIFX_NO_ERROR;
  BOOL fCloseDriver = FALSE;

  if(NULL == hDriver)
  {
    lRet = xDriverOpen(&hDriver);
    fCloseDriver = TRUE;
  }

  if(CIFX_NO_ERROR == lRet)
  {
    DEVxIO_DRIVER_ENUMBOARD tEnumBoard = {0};

    tEnumBoard.ulSize         = ulSize;
    tEnumBoard.ulBoardNumber  = ulBoard;
    tEnumBoard.ptBoardInfo    = (BOARD_INFORMATION*)pvBoardInfo;

    lRet = xDeviceIoControl(hDriver,
                            CIFx_IOCTL_DRV_ENUMBOARD,
                            &tEnumBoard);

    if(fCloseDriver)
      xDriverClose(hDriver);
  }

  return lRet;
}

/*! **************************************************************************
* Enumerate over all available Channels on a board
*   \param hDriver        Driver handle, if NULL the driver will be temporarily opened by the API
*   \param ulBoard        The board number during board enumeration
*   \param ulChannel      Should be counted from 0 upwards on enumeration
*   \param ulSize         Size of the user buffer
*   \param pvChannelInfo  Returned channel information
*   \return CIFX_NO_ERROR on success, DRV_NO_MORE_CHANNELS when ulChannel is bigger than found Channel count
******************************************************************************/
int32_t APIENTRY xDriverEnumChannels(CIFXHANDLE hDriver, uint32_t ulBoard, uint32_t ulChannel, uint32_t ulSize, void* pvChannelInfo)
{
  if(NULL == pvChannelInfo)
    return CIFX_INVALID_POINTER;

  int32_t lRet = CIFX_NO_ERROR;
  BOOL fCloseDriver = FALSE;

  if(NULL == hDriver)
  {
    lRet = xDriverOpen(&hDriver);
    fCloseDriver = TRUE;
  }

  if(CIFX_NO_ERROR == lRet)
  {
    DEVxIO_DRIVER_ENUMCHANNEL tEnumChannels = {0};

    tEnumChannels.ulSize          = ulSize;
    tEnumChannels.ulBoardNumber   = ulBoard;
    tEnumChannels.ulChannelNumber = ulChannel;
    tEnumChannels.ptChannelInfo   = (CHANNEL_INFORMATION*)pvChannelInfo;

    lRet = xDeviceIoControl(hDriver,
                            CIFx_IOCTL_DRV_ENUMCHANNEL,
                            &tEnumChannels);
    if(fCloseDriver)
      xDriverClose(hDriver);
  }

  return lRet;
}

/*! **************************************************************************
* Get/Return a memory pointer to the boards dual-port memory
*   \param hDriver      Driver handle, if NULL the driver will be temporarily opened by the API
*   \param ulBoard      The board number
*   \param ulCmd        Function command
*   \param pvMemoryInfo Memory information structure
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xDriverMemoryPointer(CIFXHANDLE hDriver, uint32_t ulBoardNumber, uint32_t ulCmd, void* pvMemoryInfo)
{
  if(NULL == pvMemoryInfo)
    return CIFX_INVALID_POINTER;

  int32_t lRet         = CIFX_NO_ERROR;
  BOOL    fCloseDriver = FALSE;

  if(NULL == hDriver)
  {
    lRet = xDriverOpen(&hDriver);
    fCloseDriver = TRUE;
  }

  if(CIFX_NO_ERROR == lRet)
  {
    DEVxIO_DRIVER_MEMPTR tMemoryInfo = {0};

    tMemoryInfo.ulBoardNumber = ulBoardNumber;
    tMemoryInfo.ulCmd         = ulCmd;
    tMemoryInfo.ptMemory      = (MEMORY_INFORMATION*)pvMemoryInfo;

    lRet = xDeviceIoControl(hDriver,
                            CIFx_IOCTL_DRV_MEMPTR,
                            &tMemoryInfo);

    if(fCloseDriver)
      xDriverClose(hDriver);
  }

  return lRet;
}

/*! **************************************************************************
* Restart a device
*   \param hDriver      Driver handle, if NULL the driver will be temporarily opened by the API
*   \param tBoardInfo   Board information
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xDriverRestartDevice(CIFXHANDLE hDriver, char* szBoardName, void* pvData)
{
  int32_t lRet      = CIFX_NO_ERROR;
  BOOL fCloseDriver = FALSE;

  if(NULL == hDriver)
  {
    lRet = xDriverOpen(&hDriver);
    fCloseDriver = TRUE;
  }

  if(CIFX_NO_ERROR == lRet)
  {
    DEVxIO_DRIVER_RESTARTDEVICE tRestart = {0};

    tRestart.szBoardName = szBoardName;
    tRestart.pvData      = pvData;

    lRet = xDeviceIoControl(hDriver,
                            CIFx_IOCTL_DRV_RESTARTDEVICE,
                            &tRestart);

    if(fCloseDriver)
      xDriverClose(hDriver);
  }

  return lRet;
}

/*! **************************************************************************
* Gets the error description in english from error number
*   \param lError         Error number returned by driver function
*   \param szBuffer       Return buffer for error description
*   \param ulBufferLen    Length of the return buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xDriverGetErrorDescription(int32_t lError, char* szBuffer, uint32_t ulBufferLen)
{
  int32_t lRet = CIFX_INVALID_PARAMETER;
  int     iIdx = 0;

  if(NULL == szBuffer)
  {
    return CIFX_INVALID_POINTER;
  }

  for(iIdx = 0; NULL != CIFXErrorStr[iIdx].str; iIdx++)
  {
    if(CIFXErrorStr[iIdx].id == lError)
    {
      int iStrLen = 0;
      strncpy(szBuffer, CIFXErrorStr[iIdx].str, ulBufferLen);

      while((iStrLen = (int)strlen(szBuffer)) > 0)
      {
        if( (szBuffer[iStrLen - 1] == '\r') ||
            (szBuffer[iStrLen - 1] == '\n'))
        {
          szBuffer[iStrLen - 1] = '\0';
        } else
        {
          break;
        }
      }
      lRet = CIFX_NO_ERROR;
      break;
    }
  }

  return lRet;
}

/***************************************************************************
*
*
*   CHANNEL FUNCTIONS....................
*
*
*
*
*
******************************************************************************/
/*! **************************************************************************
* Opens a channel by name (Name can be obtained when enumerating Channels)
*   \param szBoard    Name of the board
*   \param ulChannel  Channel number
*   \param phChannel  Returned handle to the channel (Needed for all channel specific operations)
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelOpen( CIFXHANDLE hDriver, char* szBoard, uint32_t ulChannel, CIFXHANDLE* phChannel)
{
  int32_t           lRet      = CIFX_NO_ERROR;
  PCHANNEL_HANDLE_T ptChannel = NULL;
  char              szDeviceName[CIFX_MAX_FILE_NAME_LENGTH] = {0};
  long              fd        = ERROR;

  if( (NULL == szBoard) ||
      (NULL == phChannel) )
    return CIFX_INVALID_POINTER;

  /* Create device name from board name and open device file handle */
  sprintf( szDeviceName, "/%s", szBoard);
  fd = (long)open( szDeviceName, 0, 0);

  if( ERROR == fd)
  {
  /* Failed to open the device */
    lRet = CIFX_INVALID_CHANNEL;

  /* Create a channel handle structure to store also the channel number */
  }else if( (NULL == (ptChannel = (PCHANNEL_HANDLE_T) OS_Memalloc( sizeof(CHANNEL_HANDLE_T)))) )
  {
    lRet = CIFX_INVALID_POINTER;

  }else
  {
    DEVxIO_CHANNEL_OPEN tOpenChannel = {0};

  ptChannel->hDevice = (CIFXHANDLE)fd;

    tOpenChannel.plError   = &lRet;
    tOpenChannel.ulChannel = ulChannel;

    lRet = xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_OPEN,
                &tOpenChannel);

    if(CIFX_NO_ERROR == lRet)
    {
    /* Fill channel management structure */
    ptChannel->pvNotificationLock = OS_CreateLock();
    ptChannel->ulChannel          = ulChannel;

    /* Initialize the notification list */
    lstInit(&ptChannel->lstNotifications);

    *phChannel = (CIFXHANDLE)ptChannel;
    }
  }

  if( CIFX_NO_ERROR != lRet)
  {
    if( ERROR != fd)
    /* Close the file handle */
    (void)close( (int)fd);

    if( NULL != ptChannel)
    OS_Memfree(ptChannel);

    *phChannel = NULL;
  }

  return lRet;
}

/*! **************************************************************************
* Closes an open channel
*   \param hChannel Channel handle acquired by xChannelOpen
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelClose(CIFXHANDLE hChannel)
{
  int32_t lRet = CIFX_NO_ERROR;
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(hChannel == NULL)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_CLOSE tCloseChannel = {0};

  OS_EnterLock(ptChannel->pvNotificationLock);

  /* Remove all notifications */
  while( 0 != lstCount(&ptChannel->lstNotifications))
  {
    PLST_NODE_T pNode = (PLST_NODE_T)lstFirst(&ptChannel->lstNotifications);

    /* UnregisterNotification will also remove this entry from
       lstNotifications, so we MUST not erase it here */
    xChannelUnregisterNotification(hChannel,
                                   pNode->ptCallback->ulNotification);
  }

  OS_LeaveLock(ptChannel->pvNotificationLock);
  lstFree(&ptChannel->lstNotifications);
  OS_DeleteLock(ptChannel->pvNotificationLock);

  /* Add the channel number to the IO control data structure */
  tCloseChannel.ulChannel = ptChannel->ulChannel;

  lRet = xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_CLOSE,
                          &tCloseChannel);

  if(lRet == CIFX_NO_ERROR)
  {
  int fd = *((int*)&ptChannel->hDevice);
    (void) close( fd);

    OS_Memfree(ptChannel);
  }

  return lRet;
}

/*! **************************************************************************
* Reset the channel
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param usResetMode  Cold/Warmstart selection
*   \param ulTimeout    Time in ms to wait for the reset
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelReset(CIFXHANDLE hChannel, uint32_t ulResetMode, uint32_t ulTimeout)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_RESET tReset = {0};

  tReset.ulChannel  = ptChannel->ulChannel;
  tReset.ulMode     = ulResetMode;
  tReset.ulTimeout  = ulTimeout;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_RESET,
                          &tReset);
}

/*! **************************************************************************
* Returns Channel specific information (same information as from Channel enumeration)
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulSize         Size of the user buffer
*   \param pvChannelInfo  Returned channel specific information
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelInfo(CIFXHANDLE hChannel, uint32_t ulSize, void* pvChannelInfo)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_INFO tChannelInfo = {0};

  tChannelInfo.ulChannel     = ptChannel->ulChannel;
  tChannelInfo.ulSize        = ulSize;
  tChannelInfo.ptChannelInfo = (CHANNEL_INFORMATION*)pvChannelInfo;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_INFO,
                          &tChannelInfo);
}

/*! **************************************************************************
* Returns the Mailbox state from a specific channel
*   \param hChannel         Channel handle acquired by xChannelOpen
*   \param pusRecvPktCount  Number of Messages waiting in receive mailbox
*   \param pusSendMbxState  State of the Send Mailbox
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelGetMBXState(CIFXHANDLE hChannel, uint32_t* pulRecvPktCount, uint32_t* pulSendMbxState)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_MBXSTATE tMbxState;
  tMbxState.ulChannel       = ptChannel->ulChannel;
  tMbxState.pulReceiveCount = pulRecvPktCount;
  tMbxState.pulSendState    = pulSendMbxState;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_MBXSTATE,
                          &tMbxState);
}

/*! **************************************************************************
* Inserts a packet into a channel mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ptSendPkt  Packet to send to channel
*   \param ulTimeout  Time in ms to wait for card to accept the packet
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPutPacket(CIFXHANDLE hChannel, CIFX_PACKET* ptSendPkt, uint32_t ulTimeout)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_PACKET tPacket = {0};

  tPacket.ulChannel     = ptChannel->ulChannel;
  tPacket.pvPacket      = (void*)ptSendPkt;
  tPacket.ulBufferSize  = ptSendPkt->tHeader.ulLen + (uint32_t)sizeof(ptSendPkt->tHeader);
  tPacket.ulTimeout = ulTimeout;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_PUTPACKET,
                          &tPacket);
}

/*! **************************************************************************
* Gets a packet from a channel mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ptRecvPkt  Returned packet
*   \param ulTimeout  Time in ms to wait for available packets
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelGetPacket(CIFXHANDLE hChannel, uint32_t ulSize, CIFX_PACKET* ptRecvPkt, uint32_t ulTimeout)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_PACKET tPacket = {0};

  tPacket.ulChannel     = ptChannel->ulChannel;
  tPacket.ulBufferSize  = ulSize;
  tPacket.pvPacket      = (void*)ptRecvPkt;
  tPacket.ulTimeout     = ulTimeout;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_GETPACKET,
                          &tPacket);
}

/*! **************************************************************************
* Read back a packet from a channel send mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ptRecvPkt  Returned packet
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelGetSendPacket(CIFXHANDLE hChannel, uint32_t ulSize, CIFX_PACKET* ptRecvPkt)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_PACKET tPacket = {0};

  tPacket.ulChannel     = ptChannel->ulChannel;
  tPacket.ulBufferSize  = ulSize;
  tPacket.pvPacket      = (void*)ptRecvPkt;
  tPacket.ulTimeout     = 0;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_READSENDPACKET,
                          &tPacket);
}

/*! **************************************************************************
* Trigger the channel watchdog
*   \param hChannel    Channel handle acquired by xChannelOpen
*   \param ulCmd       Watchdog command
*   \param pulTrigger  Actual trigger value from hardware
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelWatchdog(CIFXHANDLE hChannel, uint32_t ulCmd, uint32_t* pulTrigger)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_WATCHDOG tWatchdog = {0};

  tWatchdog.ulChannel   = ptChannel->ulChannel;
  tWatchdog.ulCmd       = ulCmd;
  tWatchdog.pulWatchdog = pulTrigger;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_WATCHDOG,
                          &tWatchdog);
}

/*! **************************************************************************
* Set/Get Host state
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulCmd      Function command
*   \param pulState   Actual state returned by the get command
*   \param ulTimeout  Time in ms to wait for the defined state
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelHostState(CIFXHANDLE hChannel, uint32_t ulCmd, uint32_t* pulState, uint32_t ulTimeout)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_HOSTSTATE tHostState  = {0};

  tHostState.ulChannel   = ptChannel->ulChannel;
  tHostState.ulCmd       = ulCmd;
  tHostState.pulState    = pulState;
  tHostState.ulTimeout   = ulTimeout;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_HOSTSTATE,
                          &tHostState);
}

/*! **************************************************************************
* Set/Get Configuration Lock state
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulCmd      Function command
*   \param pulState   Actual state returned by the get command
*   \param ulTimeout  Time in ms to wait for the defined state
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelConfigLock(CIFXHANDLE hChannel, uint32_t ulCmd, uint32_t* pulState, uint32_t ulTimeout)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_HOSTSTATE tConfigLock = {0};

  tConfigLock.ulChannel   = ptChannel->ulChannel;
  tConfigLock.ulCmd       = ulCmd;
  tConfigLock.pulState    = pulState;
  tConfigLock.ulTimeout   = ulTimeout;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_CONFIGLOCK,
                          &tConfigLock);
}

/*! **************************************************************************
* Set/Get Bus state
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulCmd      Function command
*   \param pulState   Actual state returned by the get command
*   \param ulTimeout  Time in ms to wait for the defined state
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelBusState(CIFXHANDLE hChannel, uint32_t ulCmd, uint32_t* pulState, uint32_t ulTimeout)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_BUSSTATE tBusState = {0};

  tBusState.ulChannel   = ptChannel->ulChannel;
  tBusState.ulCmd       = ulCmd;
  tBusState.pulState    = pulState;
  tBusState.ulTimeout   = ulTimeout;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_BUSSTATE,
                          &tBusState);
}

/*! **************************************************************************
* Set/Get DMA state
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulCmd      Function command
*   \param pulState   Actual state returned by the get command
*   \param ulTimeout  Time in ms to wait for the defined state
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelDMAState(CIFXHANDLE hChannel, uint32_t ulCmd, uint32_t* pulState)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_DMASTATE tDMAState = {0};

  tDMAState.ulChannel   = ptChannel->ulChannel;
  tDMAState.ulCmd       = ulCmd;
  tDMAState.pulState    = pulState;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_DMASTATE,
                          &tDMAState);
}

/*! **************************************************************************
* Get area information about an I/O Block
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulCmd        CIFX_IO_INPUT_AREA/CIFX_IO_OUTPUT_AREA
*   \param ulAreaNumber I/O area number to get data from
*   \param ulSize       Size of passed pointer
*   \param pvData       Pointer to CHANNEL_IO_INFORMATION structure
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelIOInfo(CIFXHANDLE  hChannel,
                                uint32_t    ulCmd,
                                uint32_t    ulAreaNumber,
                                uint32_t    ulSize,
                                void*       pvData)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_IOINFO tIoInfo = {0};

  tIoInfo.ulChannel       = ptChannel->ulChannel;
  tIoInfo.ulAreaNumber    = ulAreaNumber;
  tIoInfo.ulCmd           = ulCmd;
  tIoInfo.ulSize          = ulSize;
  tIoInfo.ptChannelIOInfo = (CHANNEL_IO_INFORMATION*)pvData;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_IOINFO,
                          &tIoInfo);
}

/*! **************************************************************************
* Reads the Input data from the channel
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulAreaNumber I/O input area number
*   \param ulOffset     Data offset in Input area
*   \param ulDataLen    Length of data to read
*   \param pvData       Buffer to place returned data
*   \param ulTimeout    Time in ms to wait for the function to finish
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelIORead(CIFXHANDLE  hChannel,
                                uint32_t    ulAreaNumber,
                                uint32_t    ulOffset,
                                uint32_t    ulDataLen,
                                void*       pvData,
                                uint32_t    ulTimeout)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_IOREAD tIoRead = {0};

  tIoRead.ulChannel    = ptChannel->ulChannel;
  tIoRead.ulAreaNumber = ulAreaNumber;
  tIoRead.ulOffset     = ulOffset;
  tIoRead.ulSize       = ulDataLen;
  tIoRead.pvBuffer     = pvData;
  tIoRead.ulTimeout    = ulTimeout;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_IOREAD,
                          &tIoRead);
}

/*! **************************************************************************
* Writes the Output data to the channel
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulAreaNumber I/O output area number
*   \param ulOffset     Data offset in Output area
*   \param ulDataLen    Length of data to send
*   \param pabWriteData Buffer containing send data
*   \param ulTimeout    Time in ms to wait for the function to finish
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelIOWrite( CIFXHANDLE  hChannel,
                                  uint32_t    ulAreaNumber,
                                  uint32_t    ulOffset,
                                  uint32_t    ulDataLen,
                                  void*       pvWriteData,
                                  uint32_t    ulTimeout)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_IOWRITE tIoWrite = {0};

  tIoWrite.ulChannel    = ptChannel->ulChannel;
  tIoWrite.ulAreaNumber = ulAreaNumber;
  tIoWrite.ulOffset     = ulOffset;
  tIoWrite.ulSize       = ulDataLen;
  tIoWrite.pvBuffer     = pvWriteData;
  tIoWrite.ulTimeout    = ulTimeout;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_IOWRITE,
                          &tIoWrite);
}

/*! **************************************************************************
* Read back Send Data Area from channel
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulAreaNumber I/O output area number
*   \param ulOffset     Data start offset
*   \param ulDataLen    Data length to read
*   \param pvData       Data buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelIOReadSendData(CIFXHANDLE  hChannel,
                                        uint32_t    ulAreaNumber,
                                        uint32_t    ulOffset,
                                        uint32_t    ulDataLen,
                                        void*       pvData)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_IOREAD tIoRead = {0};

  tIoRead.ulChannel     = ptChannel->ulChannel;
  tIoRead.ulAreaNumber  = ulAreaNumber;
  tIoRead.ulOffset      = ulOffset;
  tIoRead.ulSize        = ulDataLen;
  tIoRead.pvBuffer      = pvData;
  tIoRead.ulTimeout     = 0;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_IOREADSEND,
                          &tIoRead);
}

/*! **************************************************************************
* Access the channels control block
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulCmd        Command
*   \param ulOffset     Data start offset
*   \param ulDataLen    Data length
*   \param pvData       Data buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelControlBlock(CIFXHANDLE  hChannel,
                                      uint32_t    ulCmd,
                                      uint32_t    ulOffset,
                                      uint32_t    ulDataLen,
                                      void*       pvData)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_CTRL tCtrlBlock = {0};

  tCtrlBlock.ulChannel    = ptChannel->ulChannel;
  tCtrlBlock.ulCmd        = ulCmd;
  tCtrlBlock.ulOffset     = ulOffset;
  tCtrlBlock.ulSize       = ulDataLen;
  tCtrlBlock.pvBuffer     = pvData;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_CTRL,
                          &tCtrlBlock);
}

/*! **************************************************************************
* Acces the channels common status block
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulCmd        Command
*   \param ulOffset     Data start offset
*   \param ulDataLen    Data length
*   \param pvData       Data buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelCommonStatusBlock( CIFXHANDLE  hChannel,
                                            uint32_t    ulCmd,
                                            uint32_t    ulOffset,
                                            uint32_t    ulDataLen,
                                            void*       pvData)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_STATE tStateBlock = {0};

  tStateBlock.ulChannel    = ptChannel->ulChannel;
  tStateBlock.ulCmd        = ulCmd;
  tStateBlock.ulOffset     = ulOffset;
  tStateBlock.ulSize       = ulDataLen;
  tStateBlock.pvBuffer     = pvData;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_COMMON_STATE,
                          &tStateBlock);
}

/*! **************************************************************************
* Access the channels "user" block
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulAreaNumber Data area number
*   \param ulCmd        Command
*   \param ulOffset     Data start offset
*   \param ulDataLen    Data length
*   \param pvData       Data buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelUserBlock( CIFXHANDLE  hChannel,
                                    uint32_t    ulAreaNumber,
                                    uint32_t    ulCmd,
                                    uint32_t    ulOffset,
                                    uint32_t    ulDataLen,
                                    void*       pvData)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_USER tUserBlock = {0};

  tUserBlock.ulChannel    = ptChannel->ulChannel;
  tUserBlock.ulAreaNumber = ulAreaNumber;
  tUserBlock.ulCmd        = ulCmd;
  tUserBlock.ulOffset     = ulOffset;
  tUserBlock.ulSize       = ulDataLen;
  tUserBlock.pvBuffer     = pvData;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_USER,
                          &tUserBlock);
}

/*! **************************************************************************
* Access the channels extended status block
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulCmd        Command
*   \param ulOffset     Data start offset
*   \param ulDataLen    Data length
*   \param pvData       Data buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelExtendedStatusBlock( CIFXHANDLE  hChannel,
                                              uint32_t    ulCmd,
                                              uint32_t    ulOffset,
                                              uint32_t    ulDataLen,
                                              void*       pvData)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_STATE tStateBlock = {0};

  tStateBlock.ulChannel    = ptChannel->ulChannel;
  tStateBlock.ulCmd        = ulCmd;
  tStateBlock.ulOffset     = ulOffset;
  tStateBlock.ulSize       = ulDataLen;
  tStateBlock.pvBuffer     = pvData;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_EXTENDED_STATE,
                          &tStateBlock);
}

/*! **************************************************************************
* Get a Pointer to the I/O Area
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulCmd        Command
*   \param pvMemoryInfo Memory information
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPLCMemoryPtr(CIFXHANDLE hChannel, uint32_t ulCmd, void* pvMemoryInfo)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  if(NULL == pvMemoryInfo)
    return CIFX_INVALID_POINTER;

  DEVxIO_CHANNEL_IOMEMPTR tMemoryInfo = {0};

  tMemoryInfo.ulChannel     = ptChannel->ulChannel;
  tMemoryInfo.ulCmd         = ulCmd;
  tMemoryInfo.ptMemory      = (PLC_MEMORY_INFORMATION*)pvMemoryInfo;

  return xDeviceIoControl(ptChannel->hDevice,
                            CIFx_IOCTL_CH_IOMEMPTR,
                            &tMemoryInfo);
}

/*! **************************************************************************
* Checks if input data area has been updated and ready for reading by application
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   I/O input area number
*   \param pusReadState   State of the input area
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPLCIsReadReady(CIFXHANDLE hChannel, uint32_t ulAreaNumber, uint32_t* pulReadState)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_IOREADRDY tIoReady = {0};

  tIoReady.ulChannel    = ptChannel->ulChannel;
  tIoReady.ulAreaNumber = ulAreaNumber;
  tIoReady.pulState     = pulReadState;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_IOREADRDY,
                          &tIoReady);
}

/*! **************************************************************************
* Checks if output data area has been read by the firmware
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   I/O input area number
*   \param pusWriteState  State of the output area
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPLCIsWriteReady(CIFXHANDLE hChannel, uint32_t ulAreaNumber, uint32_t* pulWriteState)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_IOREADRDY tIoReady = {0};

  tIoReady.ulChannel    = ptChannel->ulChannel;
  tIoReady.ulAreaNumber = ulAreaNumber;
  tIoReady.pulState     = pulWriteState;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_IOWRITERDY,
                          &tIoReady);
}

/*! **************************************************************************
* Requests the firmware to read the data from input data area
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   I/O output area number
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPLCActivateWrite(CIFXHANDLE hChannel, uint32_t ulAreaNumber)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_IODORW tDoWrite = {0};

  tDoWrite.ulChannel    = ptChannel->ulChannel;
  tDoWrite.ulAreaNumber = ulAreaNumber;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_IODOWRITE,
                          &tDoWrite);
}

/*! **************************************************************************
* Requests Firmware to update input data area
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   I/O input area number
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPLCActivateRead(CIFXHANDLE hChannel, uint32_t ulAreaNumber)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_IODORW tDoRead = {0};

  tDoRead.ulChannel     = ptChannel->ulChannel;
  tDoRead.ulAreaNumber  = ulAreaNumber;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_IODOREAD,
                          &tDoRead);
}

/*! **************************************************************************
* Download a file (Firmware, Configuration, etc) to the device
*   \param hChannel           Handle to the channel
*   \param ulMode             Download mode (DOWNLOAD_MODE_FIRMWARE, etc)
*   \param szFileName         Name of the file
*   \param pabFileData        Pointer to the file data
*   \param ulFileSize         Length of the file data
*   \param pfnCallback        Callback for progress indication (NULL for no callback)
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelDownload(CIFXHANDLE hChannel, uint32_t ulMode,
                                  char* szFileName, uint8_t* pabFileData, uint32_t ulFileSize,
                                  PFN_PROGRESS_CALLBACK pfnCallback, PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;
  int32_t lRet = CIFX_NO_ERROR;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_DOWNLOAD            tDownload     = {0};

  PROGRESS_RECV_CALLBACK_DATA_T  tCallbackData;
  OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

  tDownload.ulChannel   = ptChannel->ulChannel;
  tDownload.ulMode      = ulMode;
  tDownload.pszFileName = szFileName;
  tDownload.ulFileSize  = ulFileSize;
  tDownload.pvFileData  = pabFileData;

  if( NULL != pfnRecvPktCallback)
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_IN,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_OUT,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventOUT);

    if( CIFX_NO_ERROR == (lRet = CreateReceiveCallback( pfnRecvPktCallback, &tCallbackData, pvUser)))
      tDownload.ptRecvData = &tCallbackData.tRecvData;
  }

  if( (CIFX_NO_ERROR == lRet) &&
      (NULL != pfnCallback)    )
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_PROGRESS_IN,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tProgressData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_PROGRESS_OUT,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tProgressData.szEventOUT);

    if( CIFX_NO_ERROR == (lRet = CreateProgressCallback( pfnCallback, &tCallbackData, pvUser)))
      tDownload.ptProgressData = &tCallbackData.tProgressData;
  }

  if( CIFX_NO_ERROR == lRet)
  {
    lRet = xDeviceIoControl(ptChannel->hDevice,
                            CIFx_IOCTL_CH_DOWNLOAD,
                            &tDownload);

    RemoveProgressCallback(&tCallbackData);
    RemoveReceiveCallback(&tCallbackData);
  }

  return lRet;
}

/*****************************************************************************/
/*! Starts directory enumeration on the given channel
*   \param hChannel           Handle to the channel
*   \param ptDirectoryInfo    Pointer to enumeration result.
*                             (Will be initialized inside function)
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xChannelFindFirstFile(CIFXHANDLE hChannel, CIFX_DIRECTORYENTRY* ptDirectoryInfo,
                                       PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;
  int32_t lRet = CIFX_NO_ERROR;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_FIND_FILE              tFind         = {0};

  PROGRESS_RECV_CALLBACK_DATA_T  tCallbackData;
  OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

  tFind.ulChannel       = ptChannel->ulChannel;
  tFind.ptDirectoryInfo = ptDirectoryInfo;

  if ( NULL != pfnRecvPktCallback)
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_IN,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_OUT,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventOUT);

    if ( CIFX_NO_ERROR == (lRet = CreateReceiveCallback( pfnRecvPktCallback,
                               &tCallbackData,
                               pvUser)))
    tFind.ptRecvData = &tCallbackData.tRecvData;
  }


  if ( CIFX_NO_ERROR == lRet)
  {
  tFind.ptRecvData = &tCallbackData.tRecvData;
    lRet = xDeviceIoControl(ptChannel->hDevice,
                            CIFx_IOCTL_CH_FINDFIRST,
                            &tFind);

    RemoveReceiveCallback(&tCallbackData);
  }

  return lRet;
}

/*****************************************************************************/
/*! Enumerate next entry in directory on the given channel
*   \param hChannel           Handle to the channel
*   \param ptDirectoryInfo    Pointer to enumeration result.
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xChannelFindNextFile(CIFXHANDLE hChannel, CIFX_DIRECTORYENTRY* ptDirectoryInfo,
                                      PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;
  int32_t lRet = CIFX_NO_ERROR;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_FIND_FILE              tFind         = {0};

  PROGRESS_RECV_CALLBACK_DATA_T  tCallbackData;
  OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

  tFind.ulChannel       = ptChannel->ulChannel;
  tFind.ptDirectoryInfo = ptDirectoryInfo;

  if( NULL != pfnRecvPktCallback)
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_IN,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_OUT,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventOUT);

    if ( CIFX_NO_ERROR == (lRet = CreateReceiveCallback( pfnRecvPktCallback,
                               &tCallbackData,
                               pvUser)))
      tFind.ptRecvData = &tCallbackData.tRecvData;
  }

  if ( CIFX_NO_ERROR == lRet)
  {
    lRet = xDeviceIoControl(ptChannel->hDevice,
                            CIFx_IOCTL_CH_FINDNEXT,
                            &tFind);

    RemoveReceiveCallback(&tCallbackData);
  }

  return lRet;
}

/*****************************************************************************/
/*! Uploads a file via Communication channel
*   \param hChannel           Handle to the Channel
*   \param ulMode             Transfer Mode
*   \param szFileName         Filename to upload
*   \param pabFileData        Pointer to buffer receiving upload
*   \param pulFileSize        [in]Length of buffer, [out] Bytes copied to buffer
*   \param pfnCallback        Callback pointer for progress
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xChannelUpload(CIFXHANDLE  hChannel, uint32_t ulMode, char* szFileName, uint8_t* pabFileData, uint32_t* pulFileSize,
                                PFN_PROGRESS_CALLBACK pfnCallback, PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;
  int32_t lRet = CIFX_NO_ERROR;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  DEVxIO_UPLOAD                 tUpload       = {0};

  PROGRESS_RECV_CALLBACK_DATA_T  tCallbackData;
  OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

  tUpload.ulChannel   = ptChannel->ulChannel;
  tUpload.ulMode      = ulMode;
  tUpload.pszFileName = szFileName;
  tUpload.pvFileData  = pabFileData;
  tUpload.pulFileSize = pulFileSize;

  if( NULL != pfnRecvPktCallback)
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_IN,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_OUT,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventOUT);

    if( CIFX_NO_ERROR == CreateReceiveCallback( pfnRecvPktCallback, &tCallbackData, pvUser))
      tUpload.ptRecvData = &tCallbackData.tRecvData;
  }

  if( (CIFX_NO_ERROR == lRet) &&
      (NULL != pfnCallback)    )
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_PROGRESS_IN,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tProgressData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_PROGRESS_OUT,
            *((int *)&(ptChannel->hDevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tProgressData.szEventOUT);

    if( CIFX_NO_ERROR == (lRet = CreateProgressCallback( pfnCallback, &tCallbackData, pvUser)))
      tUpload.ptProgressData = &tCallbackData.tProgressData;
  }

  if( CIFX_NO_ERROR == lRet)
  {
    lRet = xDeviceIoControl(ptChannel->hDevice,
                            CIFx_IOCTL_CH_UPLOAD,
                            &tUpload);

    RemoveReceiveCallback(&tCallbackData);
    RemoveProgressCallback(&tCallbackData);
  }

  return lRet;
}

/*****************************************************************************/
/*! Register notification
*   \param hChannel           Handle to the Channel
*   \param ulNotifyType       Notification type
*   \param pfnCallback        Callback pointer for notification handling
*   \param pvUser             User parameter for callback.
*   \param phSync             Sync handle
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xChannelRegisterNotification(CIFXHANDLE          hChannel,
                                              uint32_t            ulNotification,
                                              PFN_NOTIFY_CALLBACK pfnCallback,
                                              void*               pvUser)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;
  int32_t lRet = CIFX_NO_ERROR;

  DEVxIO_CHANNEL_NOTIFICATION tNotify = {0};

  PNOTIFY_CALLBACK_DATA_T  ptCallback = NULL;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  if(NULL == pfnCallback)
    return CIFX_INVALID_POINTER;

  tNotify.ulChannel      = ptChannel->ulChannel;
  tNotify.ulNotification = ulNotification;

  CreateEventName(CIFXNotifyEventNames,
          ulNotification,
          *((int *)&(ptChannel->hDevice)),
          NOTIFICATION_STRING_LENGTH,
          tNotify.szEventName);

  if(CIFX_NO_ERROR == (lRet = CreateNotifyCallback( ptChannel->hDevice, &tNotify, pfnCallback, pvUser, &ptCallback)))
  {
    lRet = xDeviceIoControl(ptChannel->hDevice,
                            CIFx_IOCTL_CH_NOTIFY_REG,
                            &tNotify);

    if(CIFX_NO_ERROR != lRet)
    {
      RemoveNotifyCallback(ptCallback);
    } else
    {
      /* Add the notification information the notify list */
      PLST_NODE_T ptlistNode = (PLST_NODE_T) OS_Memalloc (sizeof (LST_NODE_T));
      ptlistNode->ptCallback = ptCallback;

      OS_EnterLock(ptChannel->pvNotificationLock);
      lstAdd(&ptChannel->lstNotifications, (NODE*)ptlistNode);
      OS_LeaveLock(ptChannel->pvNotificationLock);
    }
  }

  return lRet;
}

/*****************************************************************************/
/*! Unregister notification
*   \param hChannel           Handle to the Channel
*   \param hSync              Handle to the sync object
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xChannelUnregisterNotification(CIFXHANDLE  hChannel,
                                                uint32_t    ulNotification)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;
  int32_t lRet = CIFX_NO_ERROR;
  int     lIdx = 0;

  if(NULL == ptChannel->hDevice)
    return CIFX_INVALID_HANDLE;

  PNOTIFY_CALLBACK_DATA_T  ptCallback  = NULL;
  PLST_NODE_T              ptlistNode  = NULL;

  OS_EnterLock(ptChannel->pvNotificationLock);

  for(lIdx = 1; lIdx <= lstCount(&ptChannel->lstNotifications); ++lIdx)
  {
    ptlistNode = (PLST_NODE_T)lstNth(&ptChannel->lstNotifications, lIdx);
    if( (NULL != ptlistNode) &&
        (ptlistNode->ptCallback->ulNotification == ulNotification) )
    {
      ptCallback = ptlistNode->ptCallback;
      break;
    }
  }

  DEVxIO_CHANNEL_NOTIFICATION tUnreg = {0};

  if(NULL == ptCallback)
  {
    lRet = CIFX_INVALID_HANDLE;
  }else
  {
  tUnreg.ulChannel      = ptChannel->ulChannel;
    tUnreg.ulNotification = ptCallback->ulNotification;

    if(CIFX_NO_ERROR == (lRet = xDeviceIoControl(ptChannel->hDevice,
                                                 CIFx_IOCTL_CH_NOTIFY_UNREG,
                                                 &tUnreg) ) )
    {
      /* Remove the callback thread */
      RemoveNotifyCallback(ptCallback);

      /* Remove entry from notification list */
      lstDelete(&ptChannel->lstNotifications, (NODE*)ptlistNode);
      OS_Memfree(ptlistNode);
    }
  }

  OS_LeaveLock(ptChannel->pvNotificationLock);

  return lRet;
}

/*****************************************************************************/
/*! Signal synchronisation
*   \param hChannel     Handle to the Channel
*   \param ulCmd        Signal command
*   \param ulTimeout    Timeout in ms to wait
*   \param pulSyncCount Buffer pointer to receive the sync error counter
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xChannelSyncState(CIFXHANDLE hChannel, uint32_t ulCmd, uint32_t ulTimeout, uint32_t* pulSyncCount)
{
  PCHANNEL_HANDLE_T ptChannel = (PCHANNEL_HANDLE_T)hChannel;

  if(NULL == hChannel)
    return CIFX_INVALID_HANDLE;

  if(NULL == pulSyncCount)
    return CIFX_INVALID_POINTER;

  DEVxIO_CHANNEL_SYNCSTATE tState = {0};

  tState.ulChannel        = ptChannel->ulChannel;
  tState.ulCmd            = ulCmd;
  tState.ulTimeout        = ulTimeout;
  tState.pulSyncCount     = pulSyncCount;

  return xDeviceIoControl(ptChannel->hDevice,
                          CIFx_IOCTL_CH_SYNC_STATE,
                          &tState);
}

/***************************************************************************
*
*
*   SYSDEVICE FUNCTIONS....................
*
*
*
*
*
******************************************************************************/

/*! **************************************************************************
* Open the system device on the given board
*   \param hDriver      Driver handle obtained by xChannelOpen()
*   \param szBoard      Name of the board to open
*   \param phSysdevice  Returned handle to the System device area
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceOpen(CIFXHANDLE hDriver, char* szBoard, CIFXHANDLE* phSysdevice)
{
  int32_t lRet = CIFX_NO_ERROR;
  long    fd   = 0;
  char    szDeviceName[CIFX_MAX_FILE_NAME_LENGTH] = {0};

  if( (NULL == szBoard) ||
      (NULL == phSysdevice) )
    return CIFX_INVALID_POINTER;

  /* Create a device name and get file handle to the device */
  sprintf( szDeviceName, "/%s", szBoard);
  fd = (long)open( szDeviceName, 0, 0);

  if( ERROR == fd)
  {
    /* Failed to open the device */
    lRet = CIFX_INVALID_CHANNEL;

  }else
  {
    DEVxIO_CHANNEL_OPEN tOpenChannel = {0};

    lRet = xDeviceIoControl((CIFXHANDLE)fd,
                            CIFx_IOCTL_SYSDEVICE_OPEN,
                            &tOpenChannel);

    if( CIFX_NO_ERROR == lRet)
    {
      /* Return the new handle */
    *phSysdevice = (CIFXHANDLE)fd;
    }
  }

  if(lRet != CIFX_NO_ERROR)
  {
    if( ERROR != fd)
     (void)close( (int)fd);

    *phSysdevice = NULL;
  }

  return lRet;
}

/*! **************************************************************************
* Closed an open System device
*   \param hSysdevice  Handle to the System device to close
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceClose(CIFXHANDLE hSysdevice)
{
  int32_t lRet = CIFX_DRV_IO_CONTROL_FAILED;

  if(hSysdevice == NULL)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_CLOSE tCloseSysdevice = {0};

  lRet = xDeviceIoControl(hSysdevice,
                          CIFx_IOCTL_SYSDEVICE_CLOSE,
                          &tCloseSysdevice);

  if(lRet == CIFX_NO_ERROR)
  {
  int fd = *((int*)&hSysdevice);
    (void) close( fd);
  }

  return lRet;
}

/*! **************************************************************************
* Gets the information of a system device
*   \param hSysdevice   Handle to the system device
*   \param ulSize       Size of the passed structure
*   \param pvSystemInfo Pointer to the structure for returned data
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceInfo( CIFXHANDLE hSysdevice, uint32_t ulCmd, uint32_t ulSize, void* pvSystemInfo)
{
  if(NULL == hSysdevice)
    return CIFX_INVALID_HANDLE;

  if(NULL == pvSystemInfo)
    return CIFX_INVALID_POINTER;

  DEVxIO_SYSDEVICE_INFO tSysInfo = {0};

  tSysInfo.ulCmd  = ulCmd;
  tSysInfo.ulSize = ulSize;
  tSysInfo.pvInfo = pvSystemInfo;

  return xDeviceIoControl(hSysdevice,
                          CIFx_IOCTL_SYSDEVICE_INFO,
                          &tSysInfo);
}

/*! **************************************************************************
* Gets the Mailbox state of an open system device
*   \param hSysdevice      Handle to the System device
*   \param pulRecvPktCount Number of packets in receive mailbox
*   \param pulSendPktCount Number of packets the application is able to send at once
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceGetMBXState(CIFXHANDLE hSysdevice, uint32_t* pulRecvPktCount, uint32_t* pulSendPktCount)
{
  if(NULL == hSysdevice)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_MBXSTATE tMbxState = {0};

  tMbxState.pulReceiveCount = pulRecvPktCount;
  tMbxState.pulSendState    = pulSendPktCount;

  return xDeviceIoControl(hSysdevice,
                          CIFx_IOCTL_SYSDEVICE_MBXSTATE,
                          &tMbxState);
}

/*! **************************************************************************
* Inserts a packet into the System Mailbox
*   \param hSysdevice      Handle to the System device
*   \param ptSendPkt       Packet to send to device
*   \param ulTimeout       maximum time to wait for packet to be accepted by device (in ms)
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdevicePutPacket(CIFXHANDLE hSysdevice, CIFX_PACKET* ptSendPkt, uint32_t ulTimeout)
{
  if(NULL == hSysdevice)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_PACKET tPacket = {0};

  tPacket.pvPacket      = (void*)ptSendPkt;
  tPacket.ulBufferSize  = (uint32_t) (ptSendPkt->tHeader.ulLen + sizeof(ptSendPkt->tHeader));
  tPacket.ulTimeout     = ulTimeout;

  return xDeviceIoControl(hSysdevice,
                          CIFx_IOCTL_SYSDEVICE_PUTPACKET,
                          &tPacket);
}

/*! **************************************************************************
* Retrieves a packet from the System Mailbox
*   \param hSysdevice      Handle to the System device
*   \param ulSize          Size of the buffer to retrieve the packet
*   \param ptRecvPkt       Pointer to buffer for received packet
*   \param ulTimeout       maximum time to wait for packet to be delivered by device (in ms)
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceGetPacket(CIFXHANDLE hSysdevice, uint32_t ulSize, CIFX_PACKET* ptRecvPkt, uint32_t ulTimeout)
{
  if(NULL == hSysdevice)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_PACKET tPacket = {0};

  tPacket.ulBufferSize  = ulSize;
  tPacket.pvPacket      = (void*)ptRecvPkt;
  tPacket.ulTimeout     = ulTimeout;

  return xDeviceIoControl(hSysdevice,
                          CIFx_IOCTL_SYSDEVICE_GETPACKET,
                          &tPacket);
}

/*! **************************************************************************
* Reset the whole device (coldstart)
*   \param hSysdevice   Sysdevice handle acquired by xSysdeviceOpen
*   \param ulTimeout    Time in ms to wait for the reset
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceReset(CIFXHANDLE hSysdevice, uint32_t ulTimeout)
{
  if(NULL == hSysdevice)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_RESET tReset = {0};

  tReset.ulMode     = CIFX_SYSTEMSTART;
  tReset.ulTimeout  = ulTimeout;

  return xDeviceIoControl(hSysdevice,
                          CIFx_IOCTL_SYSDEVICE_RESET,
                          &tReset);
}

/*! **************************************************************************
* Bootstart start mode, this will stop the device in the bootloader
*   \param hSysdevice   Sysdevice handle acquired by xSysdeviceOpen
*   \param ulTimeout    Time in ms to wait for the reset
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceBootstart(CIFXHANDLE hSysdevice, uint32_t ulTimeout)
{
  if(NULL == hSysdevice)
    return CIFX_INVALID_HANDLE;

  DEVxIO_CHANNEL_RESET tReset = {0};

  tReset.ulMode     = CIFX_SYSTEMSTART;
  tReset.ulTimeout  = ulTimeout;

  return xDeviceIoControl(hSysdevice,
                          CIFx_IOCTL_SYSDEVICE_BOOTSTART,
                          &tReset);
}

/*! **************************************************************************
* Bootstart start mode, this will stop the device in the bootloader
*   \param hSysdevice     Sysdevice handle acquired by xSysdeviceOpen
*   \param ulCmd          Extended memory command
*   \param ptExtMemData   Memory information structure
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceExtendedMemory( CIFXHANDLE  hSysdevice, uint32_t ulCmd, CIFX_EXTENDED_MEMORY_INFORMATION* ptExtMemData)
{
  int32_t lRet = CIFX_NO_ERROR;

  if(NULL == hSysdevice)
    return CIFX_INVALID_HANDLE;

  if(NULL == ptExtMemData)
    return CIFX_INVALID_POINTER;

  INTERNAL_EXTENDED_MEMORY_INFORMATION  tInternalExtMem = {0};
  DEVxIO_SYSDEVICE_EXTMEMPTR            tExtMemPtr      = {0};
  uint64_t*                             pullMemoryId    = NULL;

  switch(ulCmd)
  {
    case CIFX_GET_EXTENDED_MEMORY_POINTER:
      if(NULL == (pullMemoryId = (uint64_t*)malloc( sizeof(*pullMemoryId))))
      {
        lRet = CIFX_MEMORY_MAPPING_FAILED;
      }
      break;

    case CIFX_FREE_EXTENDED_MEMORY_POINTER:
      pullMemoryId = (uint64_t*)ptExtMemData->pvMemoryID;
      tInternalExtMem.ullMemoryID = *pullMemoryId;
      break;

    case CIFX_GET_EXTENDED_MEMORY_INFO:
      break;

    default:
      lRet = CIFX_INVALID_COMMAND;
      break;
  }

  if( CIFX_NO_ERROR == lRet)
  {
    /* Fill in IOCtrl data */
    tExtMemPtr.ulCmd = ulCmd;

    /* Insert data into internal ExtMem structure */
    tInternalExtMem.pvMemoryPtr  = ptExtMemData->pvMemoryPtr;
    tInternalExtMem.ulMemorySize = ptExtMemData->ulMemorySize;
    tInternalExtMem.ulMemoryType = ptExtMemData->ulMemoryType;

    tExtMemPtr.ptMemory = &tInternalExtMem;

    lRet = xDeviceIoControl(hSysdevice,
                            CIFx_IOCTL_SYSDEVICE_EXTMEM,
                            &tExtMemPtr);

    if(CIFX_NO_ERROR == lRet)
    {
      switch(ulCmd)
      {
        case CIFX_GET_EXTENDED_MEMORY_INFO:
          {
            ptExtMemData->ulMemorySize = tInternalExtMem.ulMemorySize;
            ptExtMemData->ulMemoryType = tInternalExtMem.ulMemoryType;
          }
          break;

        case CIFX_GET_EXTENDED_MEMORY_POINTER:
          {
            *pullMemoryId = tInternalExtMem.ullMemoryID;
            ptExtMemData->pvMemoryID    = pullMemoryId;
            ptExtMemData->pvMemoryPtr   = tInternalExtMem.pvMemoryPtr;
            ptExtMemData->ulMemorySize  = tInternalExtMem.ulMemorySize;
            ptExtMemData->ulMemoryType  = tInternalExtMem.ulMemoryType;
          }
          break;

        case CIFX_FREE_EXTENDED_MEMORY_POINTER:
            free(pullMemoryId);
          break;

        default:
          /* Do nothing */
          break;
      }
    }
  }

  return lRet;
}

/*! **************************************************************************
* Download a file (Firmware, Configuration etc.) to the device
*   \param hSysdevice         Handle to the system device
*   \param ulChannel          Channel number to load the file to
*   \param ulMode             Download mode (DOWNLOAD_MODE_FIRMWARE, etc)
*   \param szFileName         Name of the file
*   \param pabFileData        Pointer to the file data
*   \param ulFileSize         Length of the file data
*   \param pfnCallback        Callback for progress indication (NULL for no callback)
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceDownload(CIFXHANDLE hSysdevice, uint32_t ulChannel, uint32_t ulMode,
                                    char* szFileName,  uint8_t* pabFileData, uint32_t ulFileSize,
                                    PFN_PROGRESS_CALLBACK pfnCallback, PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser)
{
  int32_t lRet = CIFX_NO_ERROR;

  if(NULL == hSysdevice)
    return CIFX_INVALID_HANDLE;

  DEVxIO_DOWNLOAD               tDownload         = {0};

  PROGRESS_RECV_CALLBACK_DATA_T  tCallbackData;
  OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

  tDownload.ulMode      = ulMode;
  tDownload.ulChannel   = ulChannel;
  tDownload.pszFileName = szFileName;
  tDownload.ulFileSize  = ulFileSize;
  tDownload.pvFileData  = pabFileData;

  if( NULL != pfnRecvPktCallback)
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_IN,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_OUT,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventOUT);

    if( CIFX_NO_ERROR == (lRet = CreateReceiveCallback( pfnRecvPktCallback, &tCallbackData, pvUser)))
      tDownload.ptRecvData = &tCallbackData.tRecvData;

    tDownload.ptRecvData = &tCallbackData.tRecvData;
  }

  if( (CIFX_NO_ERROR == lRet) &&
      (NULL != pfnCallback)    )
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_PROGRESS_IN,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tProgressData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_PROGRESS_OUT,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tProgressData.szEventOUT);

    if( CIFX_NO_ERROR == (lRet = CreateProgressCallback( pfnCallback, &tCallbackData, pvUser)))
      tDownload.ptProgressData = &tCallbackData.tProgressData;
  }

  if( CIFX_NO_ERROR == lRet)
  {
    lRet = xDeviceIoControl(hSysdevice,
                            CIFx_IOCTL_SYSDEVICE_DOWNLOAD,
                            &tDownload);

    RemoveReceiveCallback(&tCallbackData);
    RemoveProgressCallback(&tCallbackData);
  }

  return lRet;
}

/*****************************************************************************/
/*! Starts directory enumeration on the given channel
*   \param hSysdevice         Handle to the channel
*   \param ulChannel          Channel number to get directory from
*   \param ptDirectoryInfo    Pointer to enumeration result.
*                             (Will be initialized inside function)
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xSysdeviceFindFirstFile(CIFXHANDLE hSysdevice, uint32_t ulChannel, CIFX_DIRECTORYENTRY* ptDirectoryInfo,
                                         PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser)
{
  int32_t lRet = CIFX_NO_ERROR;

  if(NULL == hSysdevice)
    return CIFX_INVALID_HANDLE;

  DEVxIO_FIND_FILE              tFind         = {0};

  PROGRESS_RECV_CALLBACK_DATA_T  tCallbackData;
  OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

  tFind.ptDirectoryInfo = ptDirectoryInfo;
  tFind.ulChannel       = ulChannel;

  if( NULL != pfnRecvPktCallback)
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_IN,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_OUT,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventOUT);

    if ( CIFX_NO_ERROR == (lRet = CreateReceiveCallback( pfnRecvPktCallback,
                               &tCallbackData,
                               pvUser)))
      tFind.ptRecvData = &tCallbackData.tRecvData;
  }

  if ( CIFX_NO_ERROR == lRet)
  {
    lRet = xDeviceIoControl(hSysdevice,
                            CIFx_IOCTL_SYSDEVICE_FINDFIRST,
                            &tFind);

    RemoveReceiveCallback(&tCallbackData);
  }
  return lRet;
}

/*****************************************************************************/
/*! Enumerate next entry in directory on the given channel
*   \param hChannel           Handle to the channel
*   \param ulChannel          Channel number to get directory from
*   \param ptDirectoryInfo    Pointer to enumeration result.
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xSysdeviceFindNextFile(CIFXHANDLE  hSysdevice, uint32_t ulChannel, CIFX_DIRECTORYENTRY* ptDirectoryInfo,
                                        PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser)
{
  int32_t lRet = CIFX_NO_ERROR;

  if(NULL == hSysdevice)
    return CIFX_INVALID_HANDLE;

  DEVxIO_FIND_FILE              tFind         = {0};

  PROGRESS_RECV_CALLBACK_DATA_T  tCallbackData;
  OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

  tFind.ptDirectoryInfo = ptDirectoryInfo;
  tFind.ulChannel       = ulChannel;

  if( NULL != pfnRecvPktCallback)
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_IN,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_OUT,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventOUT);

    if ( CIFX_NO_ERROR == (lRet = CreateReceiveCallback( pfnRecvPktCallback,
                               &tCallbackData,
                               pvUser)))
      tFind.ptRecvData = &tCallbackData.tRecvData;
  }

  if ( CIFX_NO_ERROR == lRet)
  {
    lRet = xDeviceIoControl(hSysdevice,
                            CIFx_IOCTL_SYSDEVICE_FINDNEXT,
                            &tFind);

    RemoveReceiveCallback(&tCallbackData);
  }

  return lRet;
}

/*****************************************************************************/
/*! Uploads a file via system channel
*   \param hSysdevice         Handle to the System device
*   \param ulChannel          Channel number to get directory from
*   \param ulMode             Transfer Mode
*   \param szFileName         Filename to upload
*   \param pabFileData        Pointer to buffer receiving upload
*   \param pulFileSize        [in]Length of buffer, [out] Bytes copied to buffer
*   \param pfnCallback        Callback pointer for progress
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xSysdeviceUpload(CIFXHANDLE hSysdevice, uint32_t ulChannel, uint32_t ulMode, char* szFileName, uint8_t* pabFileData, uint32_t* pulFileSize,
                                  PFN_PROGRESS_CALLBACK pfnCallback, PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser)
{
  int32_t lRet = CIFX_NO_ERROR;

  if(NULL == hSysdevice)
    return CIFX_INVALID_HANDLE;

  DEVxIO_UPLOAD                 tUpload       = {0};

  PROGRESS_RECV_CALLBACK_DATA_T  tCallbackData;
  OS_Memset( &tCallbackData, 0, sizeof(tCallbackData));

  tUpload.ulMode      = ulMode;
  tUpload.ulChannel   = ulChannel;
  tUpload.pszFileName = szFileName;
  tUpload.pvFileData  = pabFileData;
  tUpload.pulFileSize = pulFileSize;

  if( NULL != pfnRecvPktCallback)
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_IN,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_RECEIVE_OUT,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tRecvData.szEventOUT);

    if( CIFX_NO_ERROR == CreateReceiveCallback( pfnRecvPktCallback, &tCallbackData, pvUser))
      tUpload.ptRecvData = &tCallbackData.tRecvData;
  }

  if( (CIFX_NO_ERROR == lRet) &&
      (NULL != pfnCallback)    )
  {
    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_PROGRESS_IN,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tProgressData.szEventIN);

    CreateEventName(CIFXCallbackEventNames,
            CALLBACK_ID_PROGRESS_OUT,
            *((int *)&(hSysdevice)),
            NOTIFICATION_STRING_LENGTH,
            tCallbackData.tProgressData.szEventOUT);

    if( CIFX_NO_ERROR == CreateProgressCallback( pfnCallback, &tCallbackData, pvUser))
      tUpload.ptProgressData = &tCallbackData.tProgressData;
  }

  if ( CIFX_NO_ERROR == lRet)
  {
    lRet = xDeviceIoControl(hSysdevice,
                            CIFx_IOCTL_SYSDEVICE_UPLOAD,
                            &tUpload);

    RemoveReceiveCallback(&tCallbackData);
    RemoveProgressCallback(&tCallbackData);
  }

  return lRet;
}
