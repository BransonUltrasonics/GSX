#ifndef __CIFXERRORSSTR_H__
#define __CIFXERRORSSTR_H__

/*******************************************************************************
* CIF Device Driver Errors
*******************************************************************************/
struct {
  char *   str;             /* String */
  long     id;              /* Message ID */
} CIFXErrorStr[] = {

{"No Error", CIFX_NO_ERROR},

/*******************************************************************************
* Generic Errors
*******************************************************************************/
/* MessageId: CIFX_INVALID_POINTER*/
/* MessageText:*/
/*  Invalid pointer (NULL) passed to driver*/
{"Invalid pointer (NULL) passed to driver",  CIFX_INVALID_POINTER },

/* MessageId: CIFX_INVALID_BOARD*/
/* MessageText:*/
/*  No board with the given name / index available*/
{"No board with the given name / index available", CIFX_INVALID_BOARD},

/* MessageId: CIFX_INVALID_CHANNEL*/
/* MessageText:*/
/*  No channel with the given index available*/
{"No channel with the given index available", CIFX_INVALID_CHANNEL},

/* MessageId: CIFX_INVALID_HANDLE*/
/* MessageText:*/
/*  Invalid handle passed to driver*/
{"Invalid handle passed to driver", CIFX_INVALID_HANDLE},

/* MessageId: CIFX_INVALID_PARAMETER*/
/* MessageText:*/
/*  Invalid parameter*/
{"Invalid parameter", CIFX_INVALID_PARAMETER},

/* MessageId: CIFX_INVALID_COMMAND*/
/* MessageText:*/
/*  Invalid command*/
{"Invalid command", CIFX_INVALID_COMMAND},

/* MessageId: CIFX_INVALID_BUFFERSIZE*/
/* MessageText:*/
/*  Invalid buffer size*/
{"Invalid buffer size", CIFX_INVALID_BUFFERSIZE},

/* MessageId: CIFX_INVALID_ACCESS_SIZE*/
/* MessageText:*/
/*  Invalid access size*/
{"Invalid access size", CIFX_INVALID_ACCESS_SIZE},

/* MessageId: CIFX_FUNCTION_FAILED*/
/* MessageText:*/
/*  Function failed*/
{"Function failed", CIFX_FUNCTION_FAILED},

/* MessageId: CIFX_FILE_OPEN_FAILED*/
/* MessageText:*/
/*  File could not be opened*/
{"File could not be opened", CIFX_FILE_OPEN_FAILED},

/* MessageId: CIFX_FILE_SIZE_ZERO*/
/* MessageText:*/
/*  File size is zero*/
{"File size is zero", CIFX_FILE_SIZE_ZERO},

/* MessageId: CIFX_FILE_LOAD_INSUFF_MEM*/
/* MessageText:*/
/*  Insufficient memory to load file*/
{"Insufficient memory to load file", CIFX_FILE_LOAD_INSUFF_MEM},

/* MessageId: CIFX_FILE_CHECKSUM_ERROR*/
/* MessageText:*/
/*  File checksum compare failed*/
{"File checksum compare failed", CIFX_FILE_CHECKSUM_ERROR},

/* MessageId: CIFX_FILE_READ_ERROR*/
/* MessageText:*/
/*  Error reading from file*/
{"Error reading from file", CIFX_FILE_READ_ERROR},

/* MessageId: CIFX_FILE_TYPE_INVALID*/
/* MessageText:*/
/*  Invalid file type*/
{"Invalid file type", CIFX_FILE_TYPE_INVALID},

/* MessageId: CIFX_FILE_NAME_INVALID*/
/* MessageText:*/
/*  Invalid file name*/
{"Invalid file name", CIFX_FILE_NAME_INVALID},

/* MessageId: CIFX_FUNCTION_NOT_AVAILABLE*/
/* MessageText:*/
/*  Driver function not available*/
{"Driver function not available", CIFX_FUNCTION_NOT_AVAILABLE},

/* MessageId: CIFX_BUFFER_TOO_SHORT*/
/* MessageText:*/
/*  Given buffer is too short*/
{"Given buffer is too short", CIFX_BUFFER_TOO_SHORT},

/* MessageId: CIFX_MEMORY_MAPPING_FAILED*/
/* MessageText:*/
/*  Failed to map the memory*/
{"Failed to map the memory", CIFX_MEMORY_MAPPING_FAILED},

/* MessageId: CIFX_NO_MORE_ENTRIES*/
/* MessageText:*/
/*  No more entries available*/
{"No more entries available", CIFX_NO_MORE_ENTRIES},

/* MessageId: CIFX_CALLBACK_MODE_UNKNOWN*/
/* MessageText:*/
/*  Unknown callback handling mode*/
{"Unknown callback handling mode", CIFX_CALLBACK_MODE_UNKNOWN},

/* MessageId: CIFX_CALLBACK_CREATE_EVENT_FAILED*/
/* MessageText:*/
/*  Failed to create callback events*/
{"Failed to create callback events", CIFX_CALLBACK_CREATE_EVENT_FAILED},

/* MessageId: CIFX_CALLBACK_CREATE_RECV_BUFFER*/
/* MessageText:*/
/*  Failed to create callback receive buffer*/
{"Failed to create callback receive buffer", CIFX_CALLBACK_CREATE_RECV_BUFFER},

/* MessageId: CIFX_CALLBACK_ALREADY_USED*/
/* MessageText:*/
/*  Callback already used*/
{"Callback already used", CIFX_CALLBACK_ALREADY_USED},

/* MessageId: CIFX_CALLBACK_NOT_REGISTERED*/
/* MessageText:*/
/*  Callback was not registerd before*/
{"Callback was not registerd before", CIFX_CALLBACK_NOT_REGISTERED},

/* MessageId: CIFX_INTERRUPT_DISABLED*/
/* MessageText:*/
/*  Interrupt is disabled*/
{"Interrupt is disabled", CIFX_INTERRUPT_DISABLED},

/********************************************************************************/
/* Generic Driver Errors */
/********************************************************************************/
/* MessageId: CIFX_DRV_NOT_INITIALIZED*/
/* MessageText:*/
/*  Driver not initialized*/
{"Driver not initialized", CIFX_DRV_NOT_INITIALIZED},

/* MessageId: CIFX_DRV_INIT_STATE_ERROR*/
/* MessageText:*/
/*  Driver init state error*/
{"Driver init state error", CIFX_DRV_INIT_STATE_ERROR},

/* MessageId: CIFX_DRV_READ_STATE_ERROR*/
/* MessageText:*/
/*  Driver read state error*/
{"Driver read state error", CIFX_DRV_READ_STATE_ERROR},

/* MessageId: CIFX_DRV_CMD_ACTIVE*/
/* MessageText:*/
/*  Command is active on device*/
{"Command is active on device", CIFX_DRV_CMD_ACTIVE},

/* MessageId: CIFX_DRV_DOWNLOAD_FAILED*/
/* MessageText:*/
/*  General error during download*/
{"General error during download", CIFX_DRV_DOWNLOAD_FAILED},

/* MessageId: CIFX_DRV_WRONG_DRIVER_VERSION*/
/* MessageText:*/
/*  Wrong driver version*/
{"Wrong driver version", CIFX_DRV_WRONG_DRIVER_VERSION},

/* MessageId: CIFX_DRV_DRIVER_NOT_LOADED*/
/* MessageText:*/
/*  CIFx driver is not running*/
{"CIFx driver is not running", CIFX_DRV_DRIVER_NOT_LOADED},

/* MessageId: CIFX_DRV_INIT_ERROR*/
/* MessageText:*/
/*  Failed to initialize the device*/
{"Failed to initialize the device", CIFX_DRV_INIT_ERROR},

/* MessageId: CIFX_DRV_CHANNEL_NOT_INITIALIZED*/
/* MessageText:*/
/*  Channel not initialized (xOpenChannel not called)*/
{"Channel not initialized (xOpenChannel not called)", CIFX_DRV_CHANNEL_NOT_INITIALIZED},

/* MessageId: CIFX_DRV_IO_CONTROL_FAILED*/
/* MessageText:*/
/*  IOControl call failed*/
{"IOControl call failed", CIFX_DRV_IO_CONTROL_FAILED},

/* MessageId: CIFX_DRV_NOT_OPENED*/
/* MessageText:*/
/*  Driver was not opened*/
{"Driver was not opened", CIFX_DRV_NOT_OPENED},

/* MessageId: CIFX_DRV_DOWNLOAD_STORAGE_UNKNOWN*/
/* MessageText:*/
/*  Unknown download storage type (RAM/FLASH based) found*/
{"Unknown download storage type (RAM/FLASH based) found", CIFX_DRV_DOWNLOAD_STORAGE_UNKNOWN},

/* MessageId: CIFX_DRV_DOWNLOAD_FW_WRONG_CHANNEL*/
/* MessageText:*/
/*  Channel number for a firmware download not supported*/
{"Channel number for a firmware download not supported", CIFX_DRV_DOWNLOAD_FW_WRONG_CHANNEL},

/* MessageId: CIFX_DRV_DOWNLOAD_MODULE_NO_BASEOS*/
/* MessageText:*/
/*  Modules are not allowed without a Base OS firmware*/
{"Modules are not allowed without a Base OS firmware", CIFX_DRV_DOWNLOAD_MODULE_NO_BASEOS},

/*******************************************************************************/
/* Generic Device Errors */
/*******************************************************************************/
/* MessageId: CIFX_DEV_DPM_ACCESS_ERROR*/
/* MessageText:*/
/*  Dual port memory not accessable (board not found)*/
{"Dual port memory not accessable (board not found)", CIFX_DEV_DPM_ACCESS_ERROR},

/* MessageId: CIFX_DEV_NOT_READY*/
/* MessageText:*/
/*  Device not ready (ready flag failed)*/
{"Device not ready (ready flag failed)", CIFX_DEV_NOT_READY},

/* MessageId: CIFX_DEV_NOT_RUNNING*/
/* MessageText:*/
/*  Device not running (running flag failed)*/
{"Device not running (running flag failed)", CIFX_DEV_NOT_RUNNING},

/* MessageId: CIFX_DEV_WATCHDOG_FAILED*/
/* MessageText:*/
/*  Watchdog test failed*/
{"Watchdog test failed", CIFX_DEV_WATCHDOG_FAILED},

/* MessageId: CIFX_DEV_SYSERR*/
/* MessageText:*/
/*  Error in handshake flags*/
{"Error in handshake flags", CIFX_DEV_SYSERR},

/* MessageId: CIFX_DEV_MAILBOX_FULL*/
/* MessageText:*/
/*  Send mailbox is full*/
{"Send mailbox is full", CIFX_DEV_MAILBOX_FULL},

/* MessageId: CIFX_DEV_PUT_TIMEOUT*/
/* MessageText:*/
/*  Send packet timeout*/
{"Send packet timeout", CIFX_DEV_PUT_TIMEOUT},

/* MessageId: CIFX_DEV_GET_TIMEOUT*/
/* MessageText:*/
/*  Receive packet timeout*/
{"Receive packet timeout", CIFX_DEV_GET_TIMEOUT},

/* MessageId: CIFX_DEV_GET_NO_PACKET*/
/* MessageText:*/
/*  No packet available*/
{"No packet available", CIFX_DEV_GET_NO_PACKET},

/* MessageId: CIFX_DEV_MAILBOX_TOO_SHORT*/
/* MessageText:*/
/*  Mailbox too short*/
{"Mailbox too short", CIFX_DEV_MAILBOX_TOO_SHORT},

/* MessageId: CIFX_DEV_RESET_TIMEOUT*/
/* MessageText:*/
/*  Reset command timeout*/
{"Reset command timeout", CIFX_DEV_RESET_TIMEOUT},

/* MessageId: CIFX_DEV_NO_COM_FLAG*/
/* MessageText:*/
/*  COM-flag not set*/
{"COM-flag not set", CIFX_DEV_NO_COM_FLAG},

/* MessageId: CIFX_DEV_EXCHANGE_FAILED*/
/* MessageText:*/
/*  I/O data exchange failed*/
{"I/O data exchange failed", CIFX_DEV_EXCHANGE_FAILED},

/* MessageId: CIFX_DEV_EXCHANGE_TIMEOUT*/
/* MessageText:*/
/*  I/O data exchange timeout*/
{"I/O data exchange timeout", CIFX_DEV_EXCHANGE_TIMEOUT},

/* MessageId: CIFX_DEV_COM_MODE_UNKNOWN*/
/* MessageText:*/
/*  Unknown I/O exchange mode */
{"Unknown I/O exchange mode", CIFX_DEV_COM_MODE_UNKNOWN},

/* MessageId: CIFX_DEV_FUNCTION_FAILED*/
/* MessageText:*/
/*  Device function failed*/
{"Device function failed", CIFX_DEV_FUNCTION_FAILED},

/* MessageId: CIFX_DEV_DPMSIZE_MISMATCH*/
/* MessageText:*/
/*  DPM size differs from configuration*/
{"DPM size differs from configuration", CIFX_DEV_DPMSIZE_MISMATCH},

/* MessageId: CIFX_DEV_STATE_MODE_UNKNOWN*/
/* MessageText:*/
/*  Unknown state mode*/
{"Unknown state mode", CIFX_DEV_STATE_MODE_UNKNOWN},

/* MessageId: CIFX_DEV_HW_PORT_IS_USED*/
/* MessageText:*/
/*  Output port already in use*/
{"Output port already in use", CIFX_DEV_HW_PORT_IS_USED},

/* MessageId: CIFX_DEV_CONFIG_LOCK_TIMEOUT*/
/* MessageText:*/
/*  Configuration locking timeout*/
{"Configuration locking timeout", CIFX_DEV_CONFIG_LOCK_TIMEOUT},

/* MessageId: CIFX_DEV_CONFIG_UNLOCK_TIMEOUT*/
/* MessageText:*/
/*  Configuration unlocking timeout*/
{"Configuration unlocking timeout", CIFX_DEV_CONFIG_UNLOCK_TIMEOUT},

/* MessageId: CIFX_DEV_HOST_STATE_SET_TIMEOUT*/
/* MessageText:*/
/*  Set HOST state timeout*/
{"Set HOST state timeout", CIFX_DEV_HOST_STATE_SET_TIMEOUT},

/* MessageId: CIFX_DEV_HOST_STATE_CLEAR_TIMEOUT*/
/* MessageText:*/
/*  Clear HOST state timeout*/
{"Clear HOST state timeout", CIFX_DEV_HOST_STATE_CLEAR_TIMEOUT},

/* MessageId: CIFX_DEV_INITIALIZATION_TIMEOUT*/
/* MessageText:*/
/*  Timeout during channel initialization*/
{"Timeout during channel initialization", CIFX_DEV_INITIALIZATION_TIMEOUT},

/* MessageId: CIFX_DEV_BUS_STATE_ON_TIMEOUT*/
/* MessageText:*/
/*  Set Bus ON Timeout*/
{"Set Bus ON Timeout", CIFX_DEV_BUS_STATE_ON_TIMEOUT},

/* MessageId: CIFX_DEV_BUS_STATE_OFF_TIMEOUT*/
/* MessageText:*/
/*  Set Bus OFF Timeout*/
{"Set Bus OFF Timeout", CIFX_DEV_BUS_STATE_OFF_TIMEOUT},

/* MessageId: CIFX_DEV_MODULE_ALREADY_RUNNING*/
/* MessageText:*/
/*  Module already running*/
{"Module already running", CIFX_DEV_MODULE_ALREADY_RUNNING},

/* MessageId: CIFX_DEV_MODULE_ALREADY_EXISTS*/
/* MessageText:*/
/*  Module already exists*/
{"Module already exists", CIFX_DEV_MODULE_ALREADY_EXISTS},

/* MessageId: CIFX_DEV_DMA_INSUFF_BUFFER_COUNT*/
/* MessageText:*/
/*  Number of configured DMA buffers insufficient*/
{"Number of configured DMA buffers insufficient", CIFX_DEV_DMA_INSUFF_BUFFER_COUNT},

/* MessageId: CIFX_DEV_DMA_BUFFER_TOO_SMALL*/
/* MessageText:*/
/*  DMA buffers size too small (min size 256Byte)*/
{"DMA buffers size too small (min size 256Byte)", CIFX_DEV_DMA_BUFFER_TOO_SMALL},

/* MessageId: CIFX_DEV_DMA_BUFFER_TOO_BIG*/
/* MessageText:*/
/*  DMA buffers size too big (max size 63,75KByte)*/
{"DMA buffers size too big (max size 63,75KByte)", CIFX_DEV_DMA_BUFFER_TOO_BIG},

/* MessageId: CIFX_DEV_DMA_BUFFER_NOT_ALIGNED*/
/* MessageText:*/
/*  DMA buffer alignment failed (must be 256Byte)*/
{"DMA buffer alignment failed (must be 256Byte)", CIFX_DEV_DMA_BUFFER_NOT_ALIGNED},

/* MessageId: CIFX_DEV_DMA_HANSHAKEMODE_NOT_SUPPORTED*/
/* MessageText:*/
/*  I/O data uncontrolled handshake mode not supported*/
{"I/O data uncontrolled handshake mode not supported", CIFX_DEV_DMA_HANSHAKEMODE_NOT_SUPPORTED},

/* MessageId: CIFX_DEV_DMA_IO_AREA_NOT_SUPPORTED*/
/* MessageText:*/
/*  I/O area in DMA mode not supported (only area 0 possible)*/
{"I/O area in DMA mode not supported (only area 0 possible)", CIFX_DEV_DMA_IO_AREA_NOT_SUPPORTED},

/* MessageId: CIFX_DEV_DMA_STATE_ON_TIMEOUT*/
/* MessageText:*/
/*  Set DMA ON Timeout*/
{"Set DMA ON Timeout", CIFX_DEV_DMA_STATE_ON_TIMEOUT},

/* MessageId: CIFX_DEV_DMA_STATE_OFF_TIMEOUT*/
/* MessageText:*/
/*  Set DMA OFF Timeout*/
{"Set DMA OFF Timeout", CIFX_DEV_DMA_STATE_OFF_TIMEOUT},

/* MessageId: CIFX_DEV_SYNC_STATE_INVALID_MODE*/
/* MessageText:*/
/*  Device is in invalid mode for this operation*/
{"Device is in invalid mode for this operation", CIFX_DEV_SYNC_STATE_INVALID_MODE},

/* MessageId: CIFX_DEV_SYNC_STATE_TIMEOUT*/
/* MessageText:*/
/*  Waiting for synchronization event bits timed out*/
{"Waiting for synchronization event bits timed out", CIFX_DEV_SYNC_STATE_TIMEOUT},

{NULL, -1}
};
/*******************************************************************************/

#endif  /*__CIFXERRORS_H__ */
