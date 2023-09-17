/*-----------------------------------------------------------------------------
 * EcInterface.h            header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              
 *---------------------------------------------------------------------------*/

/*-DEFINES/MACROS------------------------------------------------------------*/
#ifndef	INC_ECINTERFACE
#define	INC_ECINTERFACE

/*-INCLUDES------------------------------------------------------------------*/
#include "EcTimer.h"

/*-DEFINES/MACROS------------------------------------------------------------*/
#ifndef VG_IN
#define VG_IN                               0
#define VG_OUT                              1
#endif

#define IOSTATE_READY                       0
#define IOSTATE_BUSY                        1

#define MAILBOXCMD_ECAT_AOE_BEGIN           0xF100
#define MAILBOXCMD_ECAT_AOE_READ            0xF101
#define MAILBOXCMD_ECAT_AOE_WRITE           0xF102
#define MAILBOXCMD_ECAT_AOE_READWRITE       0xF103
#define MAILBOXCMD_ECAT_AOE_WRITECONTROL    0xF104
#define MAILBOXCMD_ECAT_AOE_END             0xF1FF

#define MAILBOXCMD_CANOPEN_BEGIN            0xF300      /* begin of reserved CANopen area */
#define MAILBOXCMD_CANOPEN_SDO              0xF302
#define MAILBOXCMD_CANOPEN_RX_PDO           0xF310
#define MAILBOXCMD_CANOPEN_TX_PDO           0xF311
#define MAILBOXCMD_CANOPEN_SDO_INFO_LIST    0xF3FC      /* listType = index */
#define MAILBOXCMD_CANOPEN_SDO_INFO_OBJ     0xF3FD
#define MAILBOXCMD_CANOPEN_SDO_INFO_ENTRY   0xF3FE
#define MAILBOXCMD_CANOPEN_END              0xF3FF      /* end of reserved CANopen area */

#define MAILBOXCMD_ECAT_FOE_BEGIN           0xF400      /* File Access over EtherCAT */
#define MAILBOXCMD_ECAT_FOE_FOPENREAD       0xF401
#define MAILBOXCMD_ECAT_FOE_FOPENWRITE      0xF402
#define MAILBOXCMD_ECAT_FOE_FCLOSE          0xF403
#define MAILBOXCMD_ECAT_FOE_FREAD           0xF404
#define MAILBOXCMD_ECAT_FOE_FWRITE          0xF405
#define MAILBOXCMD_ECAT_FOE_PROGRESSINFO    0xF406
#define MAILBOXCMD_ECAT_FOE_END             0xF41F      /* File Access over EtherCAT */

#ifdef INCLUDE_SOE_SUPPORT
#define MAILBOXCMD_ECAT_SOE_BEGIN           0xF500      /* begin SoE Servo over EtherCAT */
#define MAILBOXCMD_ECAT_SOE_READREQUEST     0xF501
//#define MAILBOXCMD_ECAT_SOE_READRESPONSE    0xF502
#define MAILBOXCMD_ECAT_SOE_WRITEREQUEST    0xF503
//#define MAILBOXCMD_ECAT_SOE_WRITERESPONSE   0xF504
#define MAILBOXCMD_ECAT_SOE_ABORT_COMMAND   0xF505
#define MAILBOXCMD_ECAT_SOE_END             0xF51F      /* end SoE Servo over EtherCAT */
#endif

#ifdef INCLUDE_VOE_SUPPORT
#define MAILBOXCMD_ECAT_VOE_READ            0xF600      /* Vendor over EtherCAT */
#define MAILBOXCMD_ECAT_VOE_WRITE           0xF601
#endif

#if (defined INCLUDE_RAWMBX_SUPPORT)
#define MAILBOXCMD_ECAT_RAWMBX              0xF700
#endif

/*-FORWARD DECLARATIONS------------------------------------------------------*/
struct _EC_T_MBXTFERP;

/*-TYPEDEFS/ENUMS------------------------------------------------------------*/
typedef enum EC_MAILBOX_CMD
{	
    EC_MAILBOX_CMD_UPLOAD       = 1,    
    EC_MAILBOX_CMD_DOWNLOAD     = 2,
    EC_MAILBOX_CMD_READWRITE    = 3,

    /* Borland C++ datatype alignment correction */
    EC_MAIBLOX_CMD_BCppDummy   = 0xFFFFFFFF
}EC_MAILBOX_CMD;

/* frame type enums */
typedef enum _EC_T_FRAME_TYPE
{
    eFrameType_UNKNOWN  = 0,                            /* unknown */
    eFrameType_CYCLIC   = 1,                            /* cyclic frame */                    
    eFrameType_ACYCLIC  = 2,                            /* acyclic frame */
    eFrameType_DEBUG    = 3,                            /* debug frame */

    /* Borland C++ datatype alignment correction */
    eFrameType_BCppDummy   = 0xFFFFFFFF
} EC_T_FRAME_TYPE;

typedef struct _EC_T_DEVICE_FRAMEDESC
{
    EC_T_LINK_FRAMEDESC     oLinkFrameDesc;             /* main link layer frame descriptor */
    EC_T_LINK_FRAMEDESC     oRedLinkFrameDesc;          /* redundant link layer frame descriptor */
    EC_T_DWORD              dwNumCmdsInFrame;           /* number of datagrams per frame */
    EC_T_BOOL               bIgnoreFrameInAcycLimits;   /* This frame is not counted for ACYC Limit (used for Slave Reset) */
} EC_T_DEVICE_FRAMEDESC;

typedef struct TEcMailboxCmd
{
    EC_T_DWORD  dwInvokeId;   
    EC_T_DWORD  dwMailboxFlags;
    EC_T_DWORD  length;                                 /* number of bytes to read or to write */
    EC_T_DWORD  cmdId;
    EC_T_WORD   wMbxCmdType;                            /* e.g. EC_MAILBOX_CMD_DOWNLOAD */
    EC_T_WORD   wMbxType;                               /* e.g. ETHERCAT_MBOX_TYPE_FILEACCESS, ETHERCAT_MBOX_TYPE_CANOPEN */
    EC_T_WORD   wMbxSubType;                            /* e.g. ETHERCAT_CANOPEN_TYPE_SDOREQ */
    union
    {
        EC_T_DWORD indexOffset;
        EC_T_DWORD dwNumber;
#ifdef INCLUDE_FOE_SUPPORT
        struct
        {
            EC_T_CHAR  achFileName[MAX_FILE_NAME_SIZE]; /**< FoE filename for FoE Up/Download, not zero-terminated! */
            EC_T_DWORD dwFileNameLength;                /**< FoE length of achFileName in bytes */
            EC_T_DWORD dwPassword;                      /**< FoE password for FoE up- and download */
        } FoE;
#endif /* INCLUDE_FOE_SUPPORT */
        struct
        {
            EC_T_WORD   index;
            EC_T_BYTE   subIndex;
            EC_T_BYTE   valueInfo;
        } sIndex;
    };
    struct _EC_T_MBXTFERP* pvContext;                   /* context used by interface */
    EC_T_BOOL   bInUseByInterface;                      /* EC_TRUE if in use by interface, must not be destroyed unless not in use */
#ifdef INCLUDE_SOE_SUPPORT
    EC_T_BYTE   byDriveNo;                              /**< SoE number of drive in slave */
    EC_T_BYTE*  pbyElementFlags;                        /**< SoE which element of object is written */
    EC_T_WORD   wIDN;                                   /**< SoE IDN of the object */
#endif /* INCLUDE_SOE_SUPPORT */
#ifdef INCLUDE_AOE_SUPPORT
    EC_T_WORD   wAoeCmdId;
    EC_T_DWORD  dwIndexGroup;
    EC_T_DWORD  dwIndexOffset;
    EC_T_DWORD  dwAoeInvokeId;
    EC_T_BYTE   oTargetNetId[6];
    EC_T_WORD   wTargetPort;
    EC_T_DWORD  dwAoeErrorCode;
    EC_T_DWORD  dwAoeCmdResult;
    EC_T_DWORD  dwAoeReadLength;                        /* Read length for ReadWrite */
    EC_T_WORD   wAoEState;                              /* used by WriteControl */
    EC_T_WORD   wDeviceState;                           /* used by WriteControl */
#endif /* INCLUDE_AOE_SUPPORT */
    CEcTimer    oTimeout;                               /* Application timeout */
    CEcTimer    oRetryTimeout;                          /* Timeout for resending current pending mailbox */
    EC_T_BOOL   bRetrySendMbx;                          /* True if retry send mailbox is requested */

    EC_T_BYTE*  pbyData;
} EcMailboxCmd, *PEcMailboxCmd;

typedef struct TEcMailboxCmdRW : public TEcMailboxCmd
{
    EC_T_DWORD writeLength;
} EcMailboxCmdRW, *PEcMailboxCmdRW;

#endif /* INC_ECINTERFACE */


/*-END OF SOURCE FILE--------------------------------------------------------*/
