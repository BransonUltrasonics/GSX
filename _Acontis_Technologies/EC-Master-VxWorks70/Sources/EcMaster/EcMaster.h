/*-----------------------------------------------------------------------------
 * EcMaster.h               header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              
 *---------------------------------------------------------------------------*/


#ifndef INC_ECMASTER
#define INC_ECMASTER


/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ATETHERCAT
#include "AtEthercat.h"
#endif
#ifndef INC_ECCOMMONPRIVATE
#include "EcCommonPrivate.h"
#endif
#ifndef	INC_ECINTERFACE
#include "EcInterface.h"
#endif
#ifndef INC_ECMASTERECOMMON
#include "EcMasterCommon.h"
#endif
#ifndef INC_ECETHSWITCH
#include "EcEthSwitch.h"
#endif
#ifndef INC_ECSERVICES
#include "EcServices.h"
#endif
#ifndef INC_ECIOIMAGE
#include "EcIoImage.h"
#endif
#ifndef	INC_FIFO
#include "EcFiFo.h"
#endif
#ifndef INC_ECDEVICE
#include "EcDevice.h"
#endif
#ifndef INC_ECTIMER
#include "EcTimer.h"
#endif
#ifndef INC_ECBUSTOPOLOGY
#include "EcBusTopology.h"
#endif
#ifndef INC_ECHOTCONNECT
#include "EcHotConnect.h"
#endif

#if (defined EC_SOCKET_IP_SUPPORTED)
#include "EcSocket.h"
#endif

#ifndef INC_ATETHERCATPRIVATE
#include "AtEthercatPrivate.h"
#endif

/*-DEFINES-------------------------------------------------------------------*/

/** Default Timeout for DC = 12sec */
#define EC_DC_DEFAULTTIMEOUT        12000

/** Default Timeout for SB = 10sec */
#define EC_SB_DEFAULTTIMEOUT        10000

/** Default Number of supported slave elements in BT Scan */
#define EC_SB_DEFAULTSLAVEAMOUNT    256

#if (defined INCLUDE_FORCE_PROCESSDATA)
#define EC_NUMOF_FORCE_PROCESS_DATA_ENTRIES  100
#endif

/** Default Timeout for HC Group Detection = 15sec */
#define EC_HC_DETECTIONTIMEOUT      15000

#define EC_MASTER_STATECHANGE_DEFAULTTIMEOUT        20000   /* default timeout until the master is expected to enter the newly requested state */
#define ECAT_MASTER_ORDER_TIMEOUT_DEFAULT           15000   /* default timeout until an order shall be finished */
#define ECAT_MCSM_STABLE_TIMEOUT_DEFAULT            30000   /* default timeout until McSm stable and order queue empty */

/* See NotificationCodeToNotificationMask */ 
#define NOTIFICATION_MASK_DEFAULT ( \
    (0x00000001 & 0)    /* EC_NOTIFY_SLAVE_STATECHANGED */          | \
    (0x00000002 & 0)    /* EC_NOTIFY_SLAVES_STATECHANGED */         | \
    (0x00000004)        /* EC_NOTIFY_SLAVE_UNEXPECTED_STATE */      | \
    (0x00000008 & 0)    /* EC_NOTIFY_SLAVES_UNEXPECTED_STATE */     | \
    (0x00000010)        /* EC_NOTIFY_SLAVE_PRESENCE */              | \
    (0x00000020 & 0)    /* EC_NOTIFY_SLAVES_PRESENCE */             | \
    (0x00000040)        /* EC_NOTIFY_SLAVE_ERROR_STATUS_INFO */     | \
    (0x00000080 & 0)    /* EC_NOTIFY_SLAVES_ERROR_STATUS */         | \
    (0x00000100)        /* EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL */ | \
    (0x00000200)        /* EC_NOTIFY_CYCCMD_WKC_ERROR */            | \
    (0x00000400)        /* EC_NOTIFY_SB_MISMATCH */                 | \
    (0x00000800)        /* EC_NOTIFY_SB_STATUS */                   | \
    (0x00001000)        /* EC_NOTIFY_STATUS_SLAVE_ERROR */          | \
    (0x00002000)        /* EC_NOTIFY_FRAME_RESPONSE_ERROR */        | \
    (0x00004000)        /* EC_NOTIFY_HC_TOPOCHGDONE */              | \
    (0x00008000)        /* EC_NOTIFY_STATECHANGED */                | \
    (0x00010000 & 0)    /* EC_NOTIFY_COE_INIT_CMD */                | \
     0x00020000         /* EC_NOTIFY_JUNCTION_RED_CHANGE */         | \
    (0x00040000)        /* EC_NOTIFY_ALL_DEVICES_OPERATIONAL */     | \
    (0x00080000)        /* EC_NOTIFY_DC_STATUS */                   | \
    (0x00100000)        /* EC_NOTIFY_DC_SLV_SYNC */                 | \
    (0x00200000)        /* EC_NOTIFY_DCM_SYNC */                    | \
    (0x00400000)        /* EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR */| \
    (0x00800000 & 0)    /* EC_NOTIFY_REFCLOCK_PRESENCE */           | \
    (0x01000000)        /* EC_NOTIFY_DCX_SYNC */                    | \
    (0x02000000)        /* EC_NOTIFY_HC_DETECTADDGROUPS */          | \
    (0x04000000)        /* EC_NOTIFY_FRAMELOSS_AFTER_SLAVE */       \
)

/** Aging variable to set the AL Status WKC to 0 (must be more than m_oUnprocTimeout + 2 cycles) */
#define ALSTATUS_AGING             10

/** Amount of provided Notification buffers */
#define EC_NOTIFICATION_BUFFERS     4

#define MAX_QUEUED_COE_CMDS        20           /*< maximum number of queued CoE commands */
#define MAX_QUEUED_FOE_CMDS         4           /*< maximum number of queued FoE commands */
#define MAX_QUEUED_SOE_CMDS         4           /*< maximum number of queued SoE commands */
#define MAX_QUEUED_VOE_CMDS         4           /*< maximum number of queued VoE commands */
#define MAX_QUEUED_AOE_CMDS        20           /*< maximum number of queued AoE commands */
#define MAX_QUEUED_RAWMBX_CMDS      4           /*< maximum number of queued raw mailbox commands */

#define EC_MASTER_SLAVESTATISTICS_PERIOD   0    /* Polling period (0 = disabled) */

#define EXTEVT_DC_EVENT_0       0
#define EXTEVT_DC_LATCHEVT      EXTEVT_DC_EVENT_0
#define EXTEVT_RESERVED0        1
#define EXTEVT_DLSTATUS_CHG     2
#define EXTEVT_ALSTATUS_CHG     3

#define EXTEVT_SYNMAN_CH0_EVT   4
#define EXTEVT_SYNMAN_CH1_EVT   5
#define EXTEVT_SYNMAN_CH2_EVT   6
#define EXTEVT_SYNMAN_CH3_EVT   7

#define EXTEVT_SYNMAN_CH4_EVT   8
#define EXTEVT_SYNMAN_CH5_EVT   9
#define EXTEVT_SYNMAN_CH6_EVT  10
#define EXTEVT_SYNMAN_CH7_EVT  11

#define EXTEVT_RESERVED1       12
#define EXTEVT_RESERVED2       13
#define EXTEVT_RESERVED3       14
#define EXTEVT_RESERVED4       15

#define REMOVE_MBX_READ_RETRIES
                                 
#define INITCMD_INACTIVE        (0xffff)

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
#define PTS_RECEIVE_BUFFER      (ETHERNET_MAX_FRAMEBUF_LEN) 
#endif

/* IDs of notifications that can be enabled/disabled. See also NOTIFICATION_MASK_DEFAULT . */
#define NotificationCodeToNotificationMask(dwCode) \
    ((EC_NOTIFY_SLAVE_STATECHANGED == dwCode)?              0x00000001:   \
    ((EC_NOTIFY_SLAVES_STATECHANGED == dwCode)?             0x00000002:   \
    ((EC_NOTIFY_SLAVE_UNEXPECTED_STATE == dwCode)?          0x00000004:   \
    ((EC_NOTIFY_SLAVES_UNEXPECTED_STATE == dwCode)?         0x00000008:   \
    ((EC_NOTIFY_SLAVE_PRESENCE == dwCode)?                  0x00000010:   \
    ((EC_NOTIFY_SLAVES_PRESENCE == dwCode)?                 0x00000020:   \
    ((EC_NOTIFY_SLAVE_ERROR_STATUS_INFO == dwCode)?         0x00000040:   \
    ((EC_NOTIFY_SLAVES_ERROR_STATUS == dwCode)?             0x00000080:   \
    ((EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL == dwCode)?     0x00000100:   \
    ((EC_NOTIFY_CYCCMD_WKC_ERROR == dwCode)?                0x00000200:   \
    ((EC_NOTIFY_SB_MISMATCH == dwCode)?                     0x00000400:   \
    ((EC_NOTIFY_SB_STATUS == dwCode)?                       0x00000800:   \
    ((EC_NOTIFY_STATUS_SLAVE_ERROR == dwCode)?              0x00001000:   \
    ((EC_NOTIFY_FRAME_RESPONSE_ERROR == dwCode)?            0x00002000:   \
    ((EC_NOTIFY_HC_TOPOCHGDONE == dwCode)?                  0x00004000:   \
    ((EC_NOTIFY_STATECHANGED == dwCode)?                    0x00008000:   \
    ((EC_NOTIFY_COE_INIT_CMD == dwCode)?                    0x00010000:   \
    ((EC_NOTIFY_JUNCTION_RED_CHANGE == dwCode)?             0x00020000:   \
    ((EC_NOTIFY_ALL_DEVICES_OPERATIONAL == dwCode)?         0x00040000:   \
    ((EC_NOTIFY_DC_STATUS == dwCode)?                       0x00080000:   \
    ((EC_NOTIFY_DC_SLV_SYNC == dwCode)?                     0x00100000:   \
    ((EC_NOTIFY_DCM_SYNC == dwCode)?                        0x00200000:   \
    ((EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR == dwCode)?    0x00400000:   \
    ((EC_NOTIFY_REFCLOCK_PRESENCE == dwCode)?               0x00800000:   \
    ((EC_NOTIFY_DCX_SYNC == dwCode)?                        0x01000000:   \
    ((EC_NOTIFY_HC_DETECTADDGROUPS == dwCode)?              0x02000000:   \
    ((EC_NOTIFY_FRAMELOSS_AFTER_SLAVE == dwCode)?           0x04000000:   \
    0)))))))))))))))))))))))))))

/* trace/error strings */
#define StrMbxTypeText(var) \
    ((0 == var)?"ERR": /* ETHERCAT_MBOX_TYPE_ERROR */ \
    ((1 == var)?"AOE": /* ETHERCAT_MBOX_TYPE_ADS */ \
    ((2 == var)?"EOE": /* ETHERCAT_MBOX_TYPE_ETHERNET */ \
    ((3 == var)?"COE": /* ETHERCAT_MBOX_TYPE_CANOPEN */ \
    ((4 == var)?"FOE": /* ETHERCAT_MBOX_TYPE_FILEACCESS */ \
    ((5 == var)?"SOE": /* ETHERCAT_MBOX_TYPE_SOE */ \
    ((15 == var)?"VOE": /* ETHERCAT_MBOX_TYPE_VOE */ \
    "UNK")))))))

/* master related invoke ids */
/* low values are reserved for transition code invoke ids, see ECAT_INITCMD_I_P, ECAT_INITCMD_BEFORE, ... */
/* 0x0001xxxx are reserved for slave related invoke ids */
/* 0x0002xxxx are reserved for master related invoke ids */

/* Invoke ID's for DCS are located in EcDistributedClocks.h */

/*---------------------------------------------------------------------------*/
class CEcSlave;
class CEcMbSlave;
class CEcMaster;
class CEcDistributedClocks;
class CEcBusSlave;
class CEcSdoServ;
class CEcConfigData;


/*---------------------------------------------------------------------------*/
typedef struct _EC_T_INTFEATUREFIELD_DESC
{
#if (defined INCLUDE_OEM)
    EC_T_UINT64 qwOemKey;
#endif /* INCLUDE_OEM */
    EC_T_DWORD  dwSBTimeout;
    EC_T_DWORD  dwStateChangeDefaultTimeout;
    EC_T_DWORD  dwHCDetectionTimeout;
    EC_T_BOOL   bSlaveAliasAddressing;
#if (defined VLAN_FRAME_SUPPORT)
    EC_T_BOOL   bVLANEnabled;
    EC_T_WORD   wVLANId;
    EC_T_BYTE   byVLANPrio;
#endif
    EC_T_BOOL   bLocksDisabled;
    EC_T_BOOL   bTopologyChangeAutoMode;
    EC_T_DWORD  dwTopologyChangeDelay;
#if (defined INCLUDE_HOTCONNECT)
    EC_T_EHOTCONNECTMODE oHCMode;
#endif
    EC_T_BOOL   bNotifyUnexpectedBusSlaves;
    EC_T_DWORD  dwInitCmdRetryTimeout;
    EC_T_DWORD  dwMbxCmdTimeout;
    EC_T_DWORD  dwDefaultMbxPolling;
    EC_T_BOOL   bPdOffsetCompMode;

    EC_T_BOOL               m_abReadEEPROMEntry[READEEPROMENTRY_COUNT];
    EC_T_BOOL               m_abCheckEEPROMSetByIoctl[CHECKEEPROMENTRY_COUNT];
    EC_T_CHECKEEPROMENTRY   m_aeCheckEEPROMEntry[CHECKEEPROMENTRY_COUNT];

#if (defined INCLUDE_DC_SUPPORT)
    EC_T_BOOL   bDcDefault;
    EC_T_BOOL   bAddBrdSyncWindowMonitoring;
    EC_T_DC_CONFIGURE DcConfig;
    EC_T_DCM_CONFIG   DcmConfig;
    EC_T_DWORD  dwDcmInSyncTimeout;
    EC_T_BOOL   bFirstDcSlaveAsRefClock;
#endif

#if (defined INCLUDE_TRACE_DATA)
    EC_T_WORD   wTraceDataSize;
#endif /* INCLUDE_TRACE_DATA */

    EC_T_DWORD  dwSupportedFeatures;
    EC_T_BOOL   bAllSlavesMustReachState;
    EC_T_DWORD  dwNotificationMask;
    EC_T_DWORD  dwCycErrorNotifyMask;
    EC_T_UINT64 qwLk;

#if (defined INCLUDE_MASTER_OBD)
	EC_T_DWORD  dwVendorID;                     /* Index 0x1018, Subindex 001 */
    EC_T_DWORD  dwProductcode;                  /* Index 0x1018, Subindex 002 */
    EC_T_DWORD  dwRevision;                     /* Index 0x1018, Subindex 003 */
    EC_T_DWORD  dwSerialnumber;                 /* Index 0x1018, Subindex 004 */
    EC_T_CHAR*  szDevicename;                   /* Index 0x1008, Subindex 000 */
    EC_T_CHAR*  szHardwareversion;              /* Index 0x1009, Subindex 000 */
    EC_T_CHAR*  szSoftwareversion;              /* Index 0x100A, Subindex 000 */
    EC_T_WORD   wDevicenameSize;                /* size of allocated memory for szDevicename in bytes */
    EC_T_WORD   wHardwareversionSize;           /* size of allocated memory for szHardwareversion in bytes */
    EC_T_WORD   wSoftwareversionSize;           /* size of allocated memory for szSoftwareversion in bytes */
#endif

    EC_T_CYCFRAME_LAYOUT eCycFrameLayout;
    EC_T_BOOL   bCopyInfoInSendCycFrames;
    EC_T_BOOL   bIgnoreInputsOnWkcError;
    EC_T_BOOL   bZeroInputsOnWkcZero;
    EC_T_BOOL   bZeroInputsOnWkcError;

#if (defined INCLUDE_VARREAD)
    EC_T_BOOL   bAddVarsForSpecificDataTypes;
#endif
#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_BOOL   bRedEnhancedLineCrossedDetectionEnabled;
#endif
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_BOOL   bErrorOnLineCrossed;
    EC_T_BOOL   bNotifyNotConnectedPortA;
    EC_T_BOOL   bNotifyUnexpectedConnectedPort;
#endif
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    EC_T_BOOL   bJunctionRedundancyEnabled;
#endif
    EC_T_BOOL   bAutoAckAlStatusError;
    EC_T_BOOL   bAutoAdjustCycCmdWkc;
    EC_T_BOOL   bAdjustCycFramesAfterslavesStateChange;

    EC_T_BOOL   bGenEniAssignEepromBackToEcat;
} EC_T_INTFEATUREFIELD_DESC, *EC_PT_INTFEATUREFIELD_DESC;

/* descriptor for a single ethercat telegram (slave command) */
typedef struct TECAT_SLAVECMD_DESC
{
    CEcSlave*   pSlave;
    EC_T_DWORD  invokeId;
#ifdef REMOVE_MBX_READ_RETRIES
    PETYPE_EC_CMD_HEADER pEcCmdHeader;  /* pointer to corresponding ecat telegram in the frame, EC_NULL if none is there */
#endif
    EC_T_BYTE   cmd;
    EC_T_BYTE   byIdx;
    EC_T_WORD   ado; 
    EC_T_WORD   len; 
} ECAT_SLAVECMD_DESC, *PECAT_SLAVECMD_DESC;

#if (defined INCLUDE_MAILBOX_STATISTICS)
#define EC_MAILBOX_STATISTICS_IDX_AOE 0
#define EC_MAILBOX_STATISTICS_IDX_COE 1
#define EC_MAILBOX_STATISTICS_IDX_EOE 2
#define EC_MAILBOX_STATISTICS_IDX_FOE 3
#define EC_MAILBOX_STATISTICS_IDX_SOE 4
#define EC_MAILBOX_STATISTICS_IDX_VOE 5
#define EC_MAILBOX_STATISTICS_IDX_RAWMBX 6
#define EC_MAILBOX_STATISTICS_IDX_RES 7
#define EC_MAILBOX_STATISTICS_CNT 8
typedef struct
{
    EC_T_DWORD dwCnt;
    EC_T_DWORD dwBytes;
} EC_T_STATISTIC_TRANSFER_CUR;
typedef struct
{
    EC_T_STATISTIC_TRANSFER_CUR Read;
    EC_T_STATISTIC_TRANSFER_CUR Write;
} EC_T_STATISTIC_TRANSFER_DUPLEX_CUR;
#endif

/* helper class for a Ethernet frame containing multiple ethercat telegrams (slave commands) */
struct _ECAT_SLAVEFRAME_DESC;
class CEcSlaveFrame
{
public:
    CEcSlaveFrame();
    EC_T_VOID InitInstance(EC_T_DWORD nMaxSlaveCmdsPerFrame, struct _EC_T_MEMORY_LOGGER* pMLog);
    EC_T_VOID FreeInstance(EC_T_DWORD nMaxSlaveCmdsPerFrame, struct _EC_T_MEMORY_LOGGER* pMLog);
    ~CEcSlaveFrame();
    struct _ECAT_SLAVEFRAME_DESC*   m_pSlaveFrameDesc;  /* corresponding frame descriptor */
    ETHERNET_88A4_MAX_FRAME         m_EthernetFrame;    /* storage for the Ethernet frame */
    PETYPE_88A4_HEADER              m_p88A4Header;      /* EC Header */
    PETYPE_EC_CMD_HEADER            m_pLastEcCmdHeader; /* pointer to last ecat telegram in the frame, EC_NULL if none is there */
    PECAT_SLAVECMD_DESC             m_aEcCmdDesc;       /* descriptor of the corresponding ecat telegrams within the Ethernet frame */
    EC_T_DWORD                      m_nNumCmdsInFrame;  /* number of ecat telegrams within this frame */
    EC_T_DWORD                      m_nSpace;
    CEcTimer                        m_timeout;
    EC_T_WORD                       m_retry;
    EC_T_BYTE                       m_byEcCmdHeaderIDX; /* IDX of all ecat telegrams within this frame. This value is internally used to
                                                         * distinguish between cyclic and acyclic frames.
                                                         * Range for acyclic frames: 
                                                         * min value: EC_HEADER_IDX_SLAVECMD 
                                                         * max value: EC_HEADER_IDX_SLAVECMD + pMasterConfig->dwMaxQueuedEthFrames - 1
                                                         */
};

typedef struct _ECAT_SLAVEFRAME_DESC
{
    EC_T_LISTENTRY      ListEntry; 
    CEcSlaveFrame*      poEcSlaveFrame;
    EC_T_BOOL           bIgnoreFrameInAcycLimits;
    EC_T_DWORD*         pdwTimeStamp;       /*< data store to store timestamp result to */
    EC_T_PVOID          pvTimeStampCtxt;    /*< context for pfnTimeStamp */
    EC_PF_TIMESTAMP     pfnTimeStamp;       /*< function if not EC_NULL called to do timestamping */
    EC_T_DWORD*         pdwLastTSResult;    /*< result code store of last time stamp call */

    EC_T_WORD           wTimestampOffset;   /*< Place in the frame where the timestamp has to be placed */
    EC_T_WORD           wTimestampSize;     /*< Size in byte of the timestamp */
    EC_T_DWORD          dwRepeatCnt;
} ECAT_SLAVEFRAME_DESC, *PECAT_SLAVEFRAME_DESC;

/*---------------------------------------------------------------------------*/
/* logical master states */
typedef enum EEC_MASTER_STATE
{
    EC_MASTER_STATE_NONE,

    EC_MASTER_STATE_UNKNOWN,

    EC_MASTER_STATE_WAIT_SLAVE,

    EC_MASTER_STATE_WAIT_SLAVE_I,
    EC_MASTER_STATE_INIT,

    EC_MASTER_STATE_WAIT_MASTER_I_P,
    EC_MASTER_STATE_WAIT_SLAVE_I_P,

    EC_MASTER_STATE_PREOP,

    EC_MASTER_STATE_WAIT_MASTER_P_I,
    EC_MASTER_STATE_WAIT_SLAVE_P_I,

    EC_MASTER_STATE_WAIT_MASTER_P_S,
    EC_MASTER_STATE_WAIT_SLAVE_P_S,

    EC_MASTER_STATE_SAFEOP,

    EC_MASTER_STATE_WAIT_MASTER_S_I,
    EC_MASTER_STATE_WAIT_SLAVE_S_I,
    EC_MASTER_STATE_WAIT_MASTER_S_P,
    EC_MASTER_STATE_WAIT_SLAVE_S_P,
    EC_MASTER_STATE_WAIT_MASTER_S_O,
    EC_MASTER_STATE_WAIT_SLAVE_S_O,    

    EC_MASTER_STATE_OP,

    EC_MASTER_STATE_WAIT_MASTER_O_I,
    EC_MASTER_STATE_WAIT_SLAVE_O_I,
    EC_MASTER_STATE_WAIT_MASTER_O_P,
    EC_MASTER_STATE_WAIT_SLAVE_O_P,
    EC_MASTER_STATE_WAIT_MASTER_O_S,
    EC_MASTER_STATE_WAIT_SLAVE_O_S,

    EC_MASTER_STATE_DISABLED,

    /* Borland C++ datatype alignment correction */
    EC_MASTER_STATE_BCppDummy   = 0xFFFFFFFF
} EC_MASTER_STATE;

typedef struct _EC_T_SLAVE_STATE_UPDATE_NTFY_DESC
{
    EC_T_DWORD	dwResult;
} EC_T_SLAVE_STATE_UPDATE_NTFY_DESC;

typedef union _EC_T_NOTIFICATION_DATA
{
    EC_T_NOTIFICATION_DESC          Notification;
    EC_T_ERROR_NOTIFICATION_DESC    ErrorNotification;
    EC_T_MBXTFER                    MbxTfer;
} EC_T_NOTIFICATION_DATA;
#define SIZEOF_EC_T_NOTIFICATION_DATA sizeof(EC_T_NOTIFICATION_DATA)

typedef struct _EC_T_NOTIFICATION_INTDESC
{
    EC_T_DWORD                          dwIdx;
    EC_T_BOOL                           bUsed;
    EC_T_DWORD                          dwClientID;
    EC_T_NOTIFICATION_DATA              uNotification;
} EC_T_NOTIFICATION_INTDESC;

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
typedef struct _PTS_CONTROL_DESC
{
    EC_T_IPADDR             oIpAddr;
    EC_T_WORD               wPort;
    EC_T_WORD               wRsvd;
    EC_T_DWORD              dwPtsThreadPriority;
    EC_T_VOID*              pvPtsThreadHandle; 

    EC_T_LINK_DRV_DESC*     poLinkLayerDrvDesc;
    EC_T_VOID*              pvLinkLayerInstance;

    EC_PTS_STATE            eReqPtsState;
    EC_PTS_STATE            eCurPtsState;
    EC_T_DWORD              dwPtsError;
    struct _EC_T_MEMORY_LOGGER*     pMLog;
} PTS_CONTROL_DESC;
#endif /* INCLUDE_PASS_THROUGH_SERVER && EC_SOCKET_IP_SUPPORTED */

/*---------------------------------------------------------------------------*/

/** maximum number of cyclic frames  */
#define EC_MAX_NUM_CYC_FRAMES       64

#if (defined INCLUDE_VARREAD)
typedef struct _EC_T_CYC_SWAP_INFO
{
    EC_T_BOOL  bIsInputData;
    EC_T_WORD  wBitOffs; 
    EC_T_WORD  wBitSize;
} EC_T_CYC_SWAP_INFO;
#endif /* INCLUDE_VARREAD */

/*---------------------------------------------------------------------------*/
/* descriptor for a single cyclic command configuration inside a cyclic Ethernet frame */
typedef struct TEcCycCmdConfigDesc
{
    ETYPE_EC_CMD_HEADER             EcCmdHeader;
    EC_T_BOOL                       bActive;
    EC_T_BOOL                       bCheckWkc;
    EC_T_WORD                       wConfiguredWkc;
    EC_T_WORD                       wExpectedWkc;
#if (defined INCLUDE_WKCSTATE)
    EC_T_WORD                       wWkcStateDiagOffs;
#endif
    EC_T_WORD                       cmdSize;
    EC_T_BOOL                       bCopyInputs;
    EC_T_BOOL                       bCopyOutputs;
    EC_T_BOOL                       bIsLogicalMBoxStateCmd; /* EC_TRUE if this is a logical mailbox state command */
    EC_T_WORD                       wConfOpStatesMask;      /* mask with all states set where this command shall be sent */
    EC_T_WORD                       wEcCmdWkcOffset;        /* working counter offset in ecat command */
    EC_T_WORD                       wCmdTypeOffset;         /* offset in frame of the command type (used to NOP) */
    EC_T_WORD                       wDataLen;               /* len of payload in bytes */
    EC_T_BOOL                       bReadAlStatus;          /* EC_TRUE if this command reads ALSTATUS */

    EC_T_BOOL                       bDcDistributeSystemTime;  /* EC_TRUE if this command reads/write System Time */
    EC_T_BOOL                       bDcReadSystemTimeDiff;    /* EC_TRUE if this command reads the wire or'ed SystemTime Difference */   
    EC_T_BOOL                       bDcShiftSystemTime;       /* EC_TRUE if this command is the APWR after ARMW, which can be replaced by DC Master Sync */
    EC_T_BOOL                       bDcLatchReceiveTimes;     /* EC_TRUE if this command access to Receive Time Port 0, which can be replaced by DC Master Sync */

    EC_T_DWORD                      dwOutImageSize;            /* process data image size */
    EC_T_DWORD                      dwOutOffset;               /* offset in data image (in bytes) */
    EC_T_DWORD                      dwInImageSize;            /* process data image size */
    EC_T_DWORD                      dwInOffset;               /* offset in data image (in bytes) */

    EC_T_DWORD                      dwTimeStampLo;          /* synchronization */
    EC_T_DWORD                      dwTimeStampPostLo;      /* synchronization */
    EC_T_BOOL                       bThisTimeStamped;       /* if set to TRUE timestamping was done during this cycle */
    
    EC_T_BOOL                       bIsHcSlaveInfluence;    /* EC_TRUE: WKC is influenced by non mandatory HC group, EC_FALSE: only mandatory slaves
                                                             * have WKC influence */
    EC_T_WORD                       wLastWkc;               /* Last received working counter value */
    EC_T_WORD                       wALStatusAging;         /* aging to reset m_wALStatusWkc if frameloss */
} EcCycCmdConfigDesc;

/*---------------------------------------------------------------------------*/
/* descriptor for a single cyclic frame, such a frame contains multiple ecat telegrams */
#define MAX_NUM_CMD_PER_FRAME       100
typedef struct TEcCycFrameDesc
{
    ETHERNET_88A4_FRAME             e88A4Frame;
    ETYPE_VLAN_HEADER               vlanHeader;
    EC_T_BYTE                       byCmdHdrIdx;
    EC_T_BYTE                       byRes;
    EC_T_WORD                       wCmdCnt;
    EcCycCmdConfigDesc*             apCycCmd[MAX_NUM_CMD_PER_FRAME];
    EC_T_BOOL                       bIsPending;

    EC_T_VOID*                      pvContext;                      /* context returned e.g. by AllocsendFrame */
    EC_T_BYTE*                      pbyFrame;                       /* containing frame */
    EC_T_WORD                       wSize;
} EcCycFrameDesc;

typedef struct TEcCycCmdConfig
{
    ETHERNET_ADDRESS        macTarget;
    EC_T_WORD               reserved1;
    ETYPE_VLAN_HEADER       vlanHeader;
    EC_T_WORD               wTotalNumCmds;
    EC_T_WORD               wNumFrames;
    EcCycFrameDesc*         apCycFrameDesc[EC_MAX_NUM_CYC_FRAMES];
    EC_T_DWORD              dwCycleTime;                                    /* cycle time for current cyclic entry */
    EC_T_DWORD              dwPriority;                                     /* priority of the current cyclic entry */
    EC_T_DWORD              dwTaskId;                                       /* task id for current cyclic entry */
#if (defined INCLUDE_VARREAD)
    EC_T_WORD               wNumOfSwapInfosAllocated;                       /* number of allocated swap info entries */
    EC_T_WORD               wNumOfSwapInfos;                                /* number of swap info entries */
    EC_T_CYC_SWAP_INFO*     pSwapInfo;                                      /* copy info */
#endif
    EC_T_WORD               wNumOfCopyInfosAllocated;                       /* number of allocated slave-to-slave copy info entries */
    EC_T_WORD               wNumOfCopyInfos;                                /* number of slave-to-slave copy info entries */
    EC_T_CYC_COPY_INFO*     pCopyInfo;                                      /* copy info */
} EcCycCmdConfig;

/* descriptor for management of a single cyclic entry (Cyclic entry in the XML configuration file) */
typedef struct _CYCLIC_ENTRY_MGMT_DESC
{
    EC_T_DWORD              dwTaskId;                               /* related task id (value from XML file) */
    EC_T_DWORD              dwPriority;                             /* related priority (value from XML file) */
    EC_T_DWORD              dwCycleTime;                            /* related cycle time (value from XML file) */
    
    EC_T_WORD               wTotalNumCmds;                          /* total number of commands in frame within this cyclic entry */
    EC_T_WORD               wNumFrames;                             /* number of frames within this cyclic entry  */    
    EcCycFrameDesc**        apEcCycFrameDesc;                       /* array containing cyclic frame descriptors  */
    EcCycCmdConfigDesc**    apEcAllCycCmdConfigDesc;                /* array containing all cyclic command configuration */

    EC_T_BYTE               byFrameIdxFirst;                        /* Command header IDX of first frame for this cyclic entry */
    EC_T_BYTE               byFrameIdxLast;                         /* last ecat telegram IDX value of all frames for this state */

    /* cyclic frames monitoring */
    EC_T_BYTE               byFramesMonitoringCycleIdx;             /* monitored cycle index */
    EC_T_WORD               wFramesMonitoringSndCnt;                /* number of send frames monitored this cycle */
    EC_T_WORD               wFramesMonitoringRcvCnt;                /* number of received frames monitored this cycle */
#if (defined INCLUDE_VARREAD)
    EC_T_WORD               wNumOfSwapInfos;                        /* number of swap info entries */
    EC_T_CYC_SWAP_INFO*     pSwapInfo;                              /* swap info */
#endif
    /* Copy information for Slave-to-Slave communication */
    EC_T_WORD               wNumOfCopyInfos;                        /* number of copy info entries */
    EC_T_CYC_COPY_INFO*     pCopyInfo;                              /* copy info */
} CYCLIC_ENTRY_MGMT_DESC;


typedef struct _EC_T_PD_VAR_INFO_INTERN
{
    EC_T_CHAR                   szName[MAX_PROCESS_VAR_NAME_LEN_EX];/* Name of the found process variable */
    EC_T_CHAR*                  szSpecificDataType;                 /* Name of the unknown specific data type, EC_T_NULL if DataType is known */
    EC_T_WORD                   wDataType;                          /* Data type of the found process variable */
    EC_T_WORD                   wFixedAddr;                         /* Station address of the slave that is owner of this variable */
    EC_T_INT                    nBitSize;                           /* Size in bit of the found process variable */
    EC_T_INT                    nBitOffs;                           /* Bit offset in the process data image */
    EC_T_BOOL                   bIsInputData;                       /* Determines whether the found process variable is an input variable or an output variable */
    EC_T_WORD                   wIndex;                             /* Object Index */
    EC_T_WORD                   wSubIndex;                          /* Object SubIndex */
    EC_T_WORD                   wPdoIndex;                          /* PDO Index */
} EC_T_PD_VAR_INFO_INTERN;

/* definitions for EC_T_FORCE_RELEASE_PROCESS_DATA_INFO.wStatus */
#define EC_FORCE_RELEASE_STATUS_USED   1                    /* Bit 0: Entry valid and used */
#define EC_FORCE_RELEASE_STATUS_OUTPUT 2                    /* Bit 1: If set output data   */

typedef struct _EC_T_FORCE_RELEASE_PROCESS_DATA_INFO
{
    EC_T_DWORD  dwClientId;
    EC_T_WORD   wStatus;            /*< Status bits EC_FORCE_RELEASE_STATUS_ */
    EC_T_WORD   wBitLength;         /*< Bits length */
    EC_T_DWORD  dwBitOffset;        /*< Bit offset in PD Image */
    EC_T_BYTE	abyData[8];         /*< Data container */
} EC_T_FORCE_RELEASE_PROCESS_DATA_INFO;


/*-brief EtherCAT master class-----------------------------------------------*/

class CEcEEPRom;
class CEcDevice;
struct _EC_T_INIT_MASTER_PARMS;

class CEcMaster
{
public:     
    /********************************************************************************/
    /** \brief Constructor
    *
    *\param ipDev Pointer to EtherCAT device
    *\param pDesc Description of the EtherCAT master
    *\param pMbCallback Pointer to the mailbox callback interface   
    * 
    * \return 
    */
#if (defined INSTRUMENT_MASTER)
    CEcMaster() {};
    EC_T_BOOL m_bUnused;
#endif
    CEcMaster(  CEcDevice*                  pEcDevice
               ,PEcMasterDesc               pDesc
               ,MBX_CALLBACK                pfMbCallback
               ,NOTIFY_CALLBACK             pfNotifyCallback
               ,EC_T_MASTER_CONFIG*     pMasterConfig
               ,EC_T_MASTER_CONFIG_EX*  pMasterConfigEx
               ,EC_T_INTFEATUREFIELD_DESC*  pFeatures
               ,EC_T_DWORD                  dwVersion           );
    
    /********************************************************************************/
    /** \brief destructor
    *
    * \return 
    */
    virtual ~CEcMaster();

    EC_T_DWORD GetInstanceId(EC_T_VOID) { return m_MasterConfigEx.dwInstanceID; }

#if (defined INCLUDE_OEM)
    EC_T_VOID  SetOemKey(EC_T_UINT64 qwOemKey)
        { m_qwOemKey = qwOemKey; }
    
    EC_T_UINT64 GetOemKey(EC_T_VOID)
        { return m_qwOemKey; }

    EC_T_DWORD CheckOemKey(EC_T_UINT64 qwOemKey);
#endif /* INCLUDE_OEM */

    EC_T_VOID   SetLicenseKey(EC_T_UINT64 qwLicenseKey) 
        { m_qwLk = qwLicenseKey; }

    EC_T_UINT64 GetLicenseKey(EC_T_VOID) 
        { return m_qwLk; }

#if (defined INCLUDE_SLAVE_STATISTICS)
    EC_T_DWORD  GetSlaveStatisticsPeriod(EC_T_VOID) { return m_dwSlaveStatisticsPeriod; }
    EC_T_DWORD  SetSlaveStatisticsPeriod(EC_T_DWORD dwPeriod);
    EC_T_VOID   ClearSlaveStatistics(EC_T_VOID);
    EC_T_VOID   RequestSlaveStatistics(EC_T_VOID) { m_bSlaveStatisticsRequested = EC_TRUE; };
    EC_T_BOOL   ReadSlaveStatisticsSliced(EC_T_VOID);
#endif

    /* Log */
    EC_T_VOID   LogInitCmd(EcInitCmdDesc* pInitCmdDesc, EC_T_WORD wTransition);

    /* Notifications */
    EC_T_VOID   SetNotificationMask(EC_T_DWORD dwNotificationMask) {m_dwNotificationMask = dwNotificationMask;}
    EC_T_DWORD  GetNotificationMask() {return m_dwNotificationMask;}

    EC_T_VOID   NotifyStateChanged(EC_T_STATE eOldState, EC_T_STATE eNewState);

    EC_T_VOID   NotifySlaveStateChanged(CEcSlave* pCfgSlave);
    EC_T_VOID   NotifySlavesStateChanged(EC_T_VOID);

    EC_T_VOID   NotifySlaveUnexpectedState(CEcSlave* pCfgSlave, EC_T_STATE eSlaveDeviceStateCheck);
    EC_T_VOID   NotifySlavesUnexpectedState(EC_T_VOID);

    EC_T_VOID   NotifySlavesPresence(EC_T_VOID);
    EC_T_VOID   NotifySlavePresence(EC_T_WORD wStationAddress, EC_T_BOOL bPresent);

    EC_T_VOID   NotifySlavesError(EC_T_VOID);
    EC_T_VOID   NotifySlaveError(CEcSlave* pCfgSlave, EC_T_WORD wAlStatus, EC_T_WORD wAlStatusCode);

#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    EC_T_VOID   NotifyJunctionRedChange(CEcBusSlave* pBusSlave);
#endif
    EC_T_VOID   NotifyTopologyChangeDone(EC_T_DWORD dwStatusCode);
    EC_T_VOID   NotifyScanBusStatus(EC_T_DWORD dwResultCode, EC_T_DWORD dwSlaveCount);
    EC_T_VOID   NotifyScanBusMismatch(CEcBusSlave* poBusSlave, CEcBusSlave* poBusSlavePrev, EC_T_WORD wPortPrev, CEcSlave* poCfgSlave = EC_NULL);
#if (defined INCLUDE_HOTCONNECT)
    EC_T_VOID   NotifyScanBusDuplicateHcNode(CEcBusSlave* poBusSlave, CEcBusSlave* poBusSlavePrev, EC_T_WORD wPortPrev, CEcSlave* poCfgSlave);
#endif

    EC_T_VOID   NotifyFrameResponseError(EC_T_CHAR chErrInf0, EC_T_CHAR chErrInf1, EC_T_BOOL bIsCyclicFrame, EC_T_FRAME_RSPERR_TYPE eErrorType, EC_T_BYTE byIdxSet, EC_T_BYTE byIdxAct);
    EC_T_VOID   NotifyFramelossAfterSlave(CEcBusSlave* pBusSlave, EC_T_WORD wPortIdx);

    EC_T_VOID   NotifySlaveCoEInitCmd(CEcSlave* pCfgSlave, EC_T_WORD wCurrTransition, EcMailboxCmdDesc* pMbxInitCmd, EC_T_DWORD dwResult, EC_T_BYTE* pbyData, EC_T_WORD wDataLen);
    EC_T_VOID   NotifySlaveInitCmdResponseError(CEcSlave* pCfgSlave, EC_T_WORD wCurrTransition, EC_T_INITCMD_ERR_TYPE EErrorType, EC_T_CHAR* achComment, EC_T_DWORD dwCommentLen);

#if (defined INCLUDE_FOE_SUPPORT)
    EC_T_VOID   NotifyMbxTferProgress(EC_T_MBXTFERP* pMbxTferPriv);
    EC_T_VOID   NotifyMbxRcv(EC_T_MBXTFERP* pMbxTferPriv);
#endif

    EC_T_VOID   NotifyRefClockPresence(CEcBusSlave* pSlave);
    EC_T_VOID   NotifyDcStatus(EC_T_DWORD dwStatusCode);
    EC_T_VOID   NotifyDcSlvStatus(EC_T_BOOL bInSync, EC_T_BOOL bDeviationIsNegative, EC_T_DWORD dwDeviation, CEcBusSlave* pBusSlave);
    EC_T_VOID   NotifyDcmSync(EC_T_BOOL bInSync, EC_T_INT nCtlErrorNsecCur, EC_T_INT nCtlErrorNsecAvg, EC_T_INT nCtlErrorNsecMax);

#if (defined INCLUDE_DCX)
    EC_T_VOID   NotifyDcxSync(EC_T_DCX_SYNC_NTFY_DESC* pDcxInSyncNotify);
#endif
    EC_T_VOID   NotifyAllDevicesOperational(EC_T_BOOL bAllDevicesOperational);

    /********************************************************************************/
    /** \brief This method is called cyclically by eUsrJob_MasterTimer
    *
    * \param cycle Current cycle tick
    *
    * \return Result of the call.   
    */
    EC_T_DWORD OnMasterTimer(EC_T_VOID);
    EC_T_VOID  SetMasterTimerEnabled(EC_T_BOOL bEnabled) { m_bMasterTimerEnabled = bEnabled; }
    
    /********************************************************************************/
    /** \brief Executes the state machine of the EtherCAT master.
    *
    * \param bOnTimer Specifies if this method is called by OnTimer.
    *
    * \return 
    */
    EC_T_VOID  MasterStateMachine(EC_T_VOID);
    EC_T_VOID  ResetMasterStateMachine(EC_T_VOID);
    EC_T_VOID  StabilizeMasterStateMachine(EC_T_DWORD dwResult, EC_T_WORD wDeviceState);
    EC_T_VOID  CancelMasterStateMachine(EC_T_VOID);
    EC_T_VOID  RequestSlaveSmState(CEcSlave* pCfgSlave, EC_T_WORD wRequestedDeviceState);

    EC_T_DWORD IsAnyCmdExceedingImage();
    EC_T_BOOL  AreAllCycFramesProcessed();
    EC_T_DWORD SendCycFramesByTaskId(EC_T_DWORD dwTaskId
#if (defined INCLUDE_DC_SUPPORT)
        , EC_T_BOOL bStampJob
#endif
        , EC_T_WORD* pwNumSendFrames);
    EC_T_DWORD SendCycFramesByEntry(EC_T_DWORD dwCycEntryIndex
#if (defined INCLUDE_DC_SUPPORT)
        , EC_T_BOOL bStampJob
#endif
        );
    EC_T_DWORD SendCycFramesInDma(EC_T_WORD* pwNumSendFrames);
    EC_T_DWORD SendAcycFrames(EC_T_WORD* pwNumSendAcycFrames);
#ifdef REMOVE_MBX_READ_RETRIES
    EC_T_VOID  CancelMbxCmds(CEcSlaveFrame* poEcSlaveFrame, EC_T_DWORD dwCmdResult);
#endif
    EC_T_VOID  CancelAcycFrame(CEcSlaveFrame* poEcSlaveFrame, EC_T_DWORD dwCmdResult);
    EC_T_DWORD CheckFrame(PETHERNET_FRAME pFrame, PETHERNET_FRAME pRedFrame);

    EC_T_DWORD SetSrcMacAddress(ETHERNET_ADDRESS macSrc)
        { 
            m_pMasterDesc->oMacSrc = macSrc;

            /* adjust mac source address in acyclic frames */
            for (EC_T_DWORD i = 0; i < m_MasterConfig.dwMaxQueuedEthFrames; i++)
            {
                m_aoEcSlaveFrame[i].m_EthernetFrame.EthernetFrame.Source = m_pMasterDesc->oMacSrc;
            }
            return EC_E_NOERROR;
        }
    EC_T_DWORD GetSrcMacAddress(ETHERNET_ADDRESS* pMacSrc) { if (pMacSrc != EC_NULL) *pMacSrc = m_pMasterDesc->oMacSrc; return EC_E_NOERROR; }
    EC_T_DWORD CheckDestMacAddress(ETHERNET_ADDRESS mac)  { return mac == m_pMasterDesc->oMacDest; }

    EC_T_VOID  SetAllSlavesMustReachState(EC_T_BOOL bAllSlavesMustReachState) { m_bAllSlavesMustReachState = bAllSlavesMustReachState; }
    EC_T_BOOL  GetAllSlavesMustReachState(EC_T_VOID) { return m_bAllSlavesMustReachState; }

#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_VOID  SetRedEnhancedLineCrossedDetectionEnabled(EC_T_BOOL bRedEnhancedLineCrossedDetectionEnabled) { m_pBusTopology->SetRedEnhancedLineCrossedDetectionEnabled(bRedEnhancedLineCrossedDetectionEnabled); }
#endif
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_VOID  SetNotifyNotConnectedPortA(EC_T_BOOL bNotifyNotConnectedPortA) { m_pBusTopology->SetNotifyNotConnectedPortA(bNotifyNotConnectedPortA); }
    EC_T_VOID  SetNotifyUnexpectedConnectedPort(EC_T_BOOL bNotifyUnexpectedConnectedPort) { m_pBusTopology->SetNotifyUnexpectedConnectedPort(bNotifyUnexpectedConnectedPort); }
#endif
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    EC_T_VOID  SetJunctionRedundancyEnabled(EC_T_BOOL bJunctionRedundancyEnabled) { m_pBusTopology->SetJunctionRedundancyEnabled(bJunctionRedundancyEnabled); }
#endif
    EC_T_VOID  SetAutoAckAlStatusError(EC_T_BOOL bAutoAckAlStatusError) { m_bAutoAckAlStatusError = bAutoAckAlStatusError; }
    EC_T_BOOL  GetAutoAckAlStatusError(EC_T_VOID) { return m_bAutoAckAlStatusError; }

    EC_T_VOID  SetAutoAdjustCycCmdWkc(EC_T_BOOL bAutoAdjustCycCmdWkc) { m_bAutoAdjustCycCmdWkc = bAutoAdjustCycCmdWkc; }
    EC_T_BOOL  GetAutoAdjustCycCmdWkc(EC_T_VOID) { return m_bAutoAdjustCycCmdWkc; }

    EC_T_VOID  SetAdjustCycFramesAfterslavesStateChange(EC_T_BOOL bAdjustCycFramesAfterslavesStateChange) { m_bAdjustCycFramesAfterslavesStateChange = bAdjustCycFramesAfterslavesStateChange; }
    EC_T_BOOL  GetAdjustCycFramesAfterslavesStateChange(EC_T_VOID) { return m_bAdjustCycFramesAfterslavesStateChange; }

    EC_T_BOOL  IsConfigLoaded(EC_T_VOID) { return m_bConfigLoaded; }
    EC_T_VOID  SetConfigLoaded(EC_T_BOOL bConfigLoaded) { m_bConfigLoaded = bConfigLoaded; }

    EC_T_BOOL  IsConfigured(EC_T_VOID) { return m_bConfigured; }
    EC_T_VOID  SetConfigured(EC_T_BOOL bConfigured) { m_bConfigured = bConfigured; }

    EC_T_DWORD SetCycFrameLayout(EC_T_CYCFRAME_LAYOUT eCycFrameLayout)
    {
#if (defined INCLUDE_TRACE_DATA)
        if ((eCycFrameLayout_FIXED == eCycFrameLayout) && (m_wTraceDataSize > 0))
        {
            return EC_E_NOTSUPPORTED;
        }
#endif /* INCLUDE_TRACE_DATA */
#if (defined INCLUDE_DC_SUPPORT)
        if ((eCycFrameLayout_FIXED == eCycFrameLayout) && m_bAddBrdSyncWindowMonitoring)
        {
            return EC_E_NOTSUPPORTED;
        }
#endif /* INCLUDE_DC_SUPPORT */

        m_eCycFrameLayout = eCycFrameLayout;
        return EC_E_NOERROR;
    }
    EC_T_CYCFRAME_LAYOUT GetCycFrameLayout(EC_T_VOID) { return m_eCycFrameLayout; }

    EC_T_VOID  CalculateEcatCmdTimeout(EC_T_DWORD dwBusCycleTimeUsec);
    EC_T_DWORD GetEcatCmdTimeout(EC_T_VOID) { return m_dwEcatCmdTimeout; }
    EC_T_DWORD SetDefaultTimeouts(EC_T_MASTERDEFAULTTIMEOUTS_DESC* pDefaultTimeoutsDesc);

    EC_T_VOID SetCopyInfoInSendCycFrames(EC_T_BOOL bCopyInfoInSendCycFrames) { m_bCopyInfoInSendCycFrames = bCopyInfoInSendCycFrames; }
    
    EC_T_VOID SetIgnoreInputsOnWkcError(EC_T_BOOL bIgnoreInputsOnWkcError) 
    { 
        m_bIgnoreInputsOnWkcError = bIgnoreInputsOnWkcError; 
        if (m_bIgnoreInputsOnWkcError)
            m_bZeroInputsOnWkcZero = EC_FALSE;
    }

    EC_T_VOID SetZeroInputsOnWkcZero(EC_T_BOOL bZeroInputsOnWkcZero)
    {
        m_bZeroInputsOnWkcZero = bZeroInputsOnWkcZero;
        if (m_bZeroInputsOnWkcZero)
            m_bIgnoreInputsOnWkcError = EC_FALSE;
    }
    EC_T_VOID SetZeroInputsOnWkcError(EC_T_BOOL bZeroInputsOnWkcError)
    {
        m_bZeroInputsOnWkcError = bZeroInputsOnWkcError;
    }
    
    EC_T_VOID SetGenEniAssignEepromBackToEcat(EC_T_BOOL bGenEniAssignEepromBackToEcat) { m_bGenEniAssignEepromBackToEcat = bGenEniAssignEepromBackToEcat; }

    EC_T_DWORD GetBusCycleTimeUsec(EC_T_VOID) { return m_MasterConfig.dwBusCycleTimeUsec; }

    /*-IEcMailbox----------------------------------------------------------------*/

    /********************************************************************************/
    /** \brief Mailbox response to a previously sent mailbox(see SendMailboxCmd) command.
    *
    * \param pCmd Mailbox command
    *
    * \return 
    */
    EC_T_DWORD MailboxResponse(  PEcMailboxCmd       pCmd
                                ,EC_T_BYTE*          pbyMbxData
                                ,EC_T_DWORD          dwMbxDataLength
                                ,EC_T_DWORD          dwMbxResult     );
    
    /********************************************************************************/
    /** \brief Sends a mailbox command to the specified slave.
    *
    * \param nSlave Number of the slave
    *
    * \return Result of the call.
    */

    EC_T_DWORD SendMailboxCmd(EC_T_DWORD nSlave, PEcMailboxCmd pCmd);   

#ifdef INCLUDE_FOE_SUPPORT
    EC_T_DWORD FoeSegmentedUploadStart(EC_T_MBXTFERP* pMbxTferPriv, EC_T_CHAR* szFileName, EC_T_DWORD dwFileNameLen, EC_T_DWORD dwFileSize, 
                                         EC_T_DWORD dwPassword);
    EC_T_DWORD FoeSegmentedUploadContinue(EC_T_MBXTFERP* pMbxTferPriv);
    EC_T_DWORD FoeSegmentedDownloadStart(EC_T_MBXTFERP* pMbxTferPriv, EC_T_CHAR* szFileName, EC_T_DWORD dwFileNameLen, EC_T_DWORD dwFileSize, 
                                         EC_T_DWORD dwPassword);
    EC_T_DWORD FoeSegmentedDownloadContinue(EC_T_MBXTFERP* pMbxTferPriv);
#endif /* INCLUDE_FOE_SUPPORT */

    /** \brief Set CoE emergency mailbox command block.
    *
    * \return 
    */
    EC_T_VOID SetCoeEmergencyMbxCmd( PEcMailboxCmd pCmd)
        { m_pEcMbxCoeEmergencyCmd = pCmd; }

    /** \brief Get CoE emergency mailbox command block.
    *
    * \return 
    */
    PEcMailboxCmd GetCoeEmergencyMbxCmd(EC_T_VOID)
        {return m_pEcMbxCoeEmergencyCmd;}

#ifdef INCLUDE_SOE_SUPPORT
    /** \brief Set SoE emergency mailbox command block.
    *
    * \return 
    */
    EC_T_VOID SetSoeEmergencyMbxCmd( PEcMailboxCmd pCmd)
        { m_pEcMbxSoeEmergencyCmd = pCmd; }

    /** \brief Get SoE emergency mailbox command block.
    *
    * \return 
    */
    PEcMailboxCmd GetSoeEmergencyMbxCmd(EC_T_VOID)
        {return m_pEcMbxSoeEmergencyCmd;}

    /** \brief Set SoE notification mailbox command block.
    *
    * \return 
    */
    EC_T_VOID SetSoeNotificationMbxCmd( PEcMailboxCmd pCmd)
        { m_pEcMbxSoeNotificationCmd = pCmd; }

    /** \brief Get SoE notification mailbox command block.
    *
    * \return 
    */
    PEcMailboxCmd GetSoeNotificationMbxCmd(EC_T_VOID)
        {return m_pEcMbxSoeNotificationCmd;}
#endif

    /********************************************************************************/
    /** \brief Request EtherCAT master to change to the specified state.
    *
    * \state The requested EtherCAT state.
    *
    * \return 
    */
    EC_T_VOID RequestMasterSmState(EC_T_WORD state);
    EC_T_VOID RefreshMasterStateMachIsStable(EC_T_VOID)
        {
            if ((m_wCurMasterDeviceState == m_wReqMasterDeviceState) && 
                   ((m_eCurMasterLogicalState == EC_MASTER_STATE_UNKNOWN) ||
                    (m_eCurMasterLogicalState == EC_MASTER_STATE_INIT)    ||
                    (m_eCurMasterLogicalState == EC_MASTER_STATE_PREOP)   ||
                    (m_eCurMasterLogicalState == EC_MASTER_STATE_SAFEOP)  ||
                    (m_eCurMasterLogicalState == EC_MASTER_STATE_OP)
                   )
               )
            {
                if (EC_E_BUSY == m_dwMasterStateMachineResult)
                {
                    m_dwMasterStateMachineResult = EC_E_NOERROR;
                }
                m_bStateMachIsStable = EC_TRUE;
            }
            else
            {
                m_bStateMachIsStable = EC_FALSE;
            }
        }
    EC_T_BOOL MasterStateMachIsStable(EC_T_VOID) {return m_bStateMachIsStable;}

    /* Scan Bus */
    EC_T_VOID  SBCreateCfgTopology(EC_T_VOID);
    EC_T_VOID  SBDeleteCfgTopology(EC_T_VOID);
    EC_T_DWORD SBFreeRanges(EC_T_VOID);
    EC_T_VOID  SBSetFreeRange(EC_T_DWORD dwIdx, EC_T_WORD wBegin, EC_T_WORD wEnd);

    EC_T_DWORD GetScanBusDefaultTimeout(EC_T_VOID) { return m_dwSBTimeout; };
    EC_T_DWORD StartBTBusScan(EC_T_DWORD dwTimeout);
    EC_T_DWORD GetBusScanResult(EC_T_VOID) { return m_dwScanBusResult; };
    EC_T_DWORD GetScanBusSlaveCount(EC_T_VOID);

    EC_T_DWORD StartBTTopologyChange(EC_T_VOID);
    EC_T_DWORD StartBTAcceptTopologyChange(EC_T_VOID);
    EC_T_DWORD StartBTDlStatusInt(EC_T_VOID);
    EC_T_DWORD StartBTAlStatusRefresh(EC_T_VOID);
    EC_T_DWORD WaitForBTStateMachine(EC_T_VOID);
    EC_T_DWORD ReleaseBTStateMachine(EC_T_DWORD dwResult);

    EC_T_VOID  SetSBVerificationEnable(EC_T_BOOL bEnable);

    EC_T_DWORD SbGetSlaveInfo(EC_T_DWORD dwAutoIncAddr, EC_T_PVOID pvInfo);
    EC_T_DWORD SbGetSlaveInfoEep(EC_T_SB_SLAVEINFO_EEP_REQ_DESC* pReq, EC_T_SB_SLAVEINFO_EEP_RES_DESC* pRes);
    EC_T_DWORD SbGetSlaveInfoEx(EC_T_SB_SLAVEINFO_REQ_DESC* pReq, EC_T_SB_SLAVEINFO_RES_DESC* pRes);

#if (defined INCLUDE_DC_SUPPORT)
    /* distributed clocks */
    EC_T_DWORD SetDcSupportEnabled(EC_T_BOOL bEnable, EC_T_DWORD dwTimeout);
    EC_T_BOOL  GetDcSupportEnabled(EC_T_VOID){ return m_bDCSupportEnabled;};
    EC_T_VOID  SetAddBrdSyncWindowMonitoring(EC_T_BOOL bVal){ m_bAddBrdSyncWindowMonitoring = bVal;};
    EC_T_BOOL  GetAddBrdSyncWindowMonitoring(EC_T_VOID){ return m_bAddBrdSyncWindowMonitoring;};
    EC_T_DWORD GetDcmInSyncTimeout(EC_T_VOID){ return m_dwDcmInSyncTimeout; };
#endif /* INCLUDE_DC_SUPPORT */

    /***************************************************************************************************/
    /**
    \brief  Enable ScanBus.
    
    \return EC_E_NOERROR on success, error code otherwise.
    */
    EC_T_DWORD SetSBEnabled(
                                   EC_T_DWORD dwTimeout     /**< [in]	Timeout value in msec if not set default 
                                                                        value is used*/
                                   )
    {
        if( 0 != dwTimeout )
        {
            m_dwSBTimeout = dwTimeout;
        }

        return EC_E_NOERROR;
    }

    EC_T_DWORD EnableAliasAddressing(EC_T_BOOL bEnable);

    EC_T_VOID IncNotifySBStatusMaskedCnt(EC_T_VOID) { m_dwNotifySBStatusMaskedCnt++; }
    EC_T_VOID DecNotifySBStatusMaskedCnt(EC_T_VOID)
    {
        OsDbgAssert(0 < m_dwNotifySBStatusMaskedCnt);
        if (0 < m_dwNotifySBStatusMaskedCnt)
            m_dwNotifySBStatusMaskedCnt--;
    }
    EC_T_VOID IncNotifySBMismatchMaskedCnt(EC_T_VOID) { m_dwNotifySBMismatchMaskedCnt++; }
    EC_T_VOID DecNotifySBMismatchMaskedCnt(EC_T_VOID)
    {
        OsDbgAssert(0 < m_dwNotifySBMismatchMaskedCnt);
        if (0 < m_dwNotifySBMismatchMaskedCnt)
            m_dwNotifySBMismatchMaskedCnt--;
    }
    EC_T_VOID IncNotifyTopoChangeDoneMaskedCnt(EC_T_VOID) { m_dwNotifyTopoChangeDoneMaskedCnt++; }
    EC_T_VOID DecNotifyTopoChangeDoneMaskedCnt(EC_T_VOID)
    {
        OsDbgAssert(0 < m_dwNotifyTopoChangeDoneMaskedCnt);
        if (0 < m_dwNotifyTopoChangeDoneMaskedCnt)
            m_dwNotifyTopoChangeDoneMaskedCnt--;
    }

    /* cyclic timeout warning, cyclic error notification mask */
    EC_T_VOID  SetCycErrorNotifyMask(EC_T_DWORD dwCycErrorNotifyMask) { m_dwCycErrorNotifyMask = dwCycErrorNotifyMask; }
    EC_T_DWORD GetCycErrorNotifyMask(EC_T_VOID) { return m_dwCycErrorNotifyMask; }
    EC_T_VOID  IncCycErrorNotifyMaskedCnt(EC_T_VOID)
    {
        m_dwCycErrorNotifyMaskedCnt++;
    }
    EC_T_VOID  DecCycErrorNotifyMaskedCnt(EC_T_VOID)
    {
        if (0 == m_dwCycErrorNotifyMaskedCnt)
        {
            OsDbgAssert(EC_FALSE);
        }
        else
        {
            m_dwCycErrorNotifyMaskedCnt--;
        }
    }
    /* frame response error mask */
    EC_T_VOID  IncFrameResponseNotifyMaskedCnt(EC_T_VOID)
    {
        m_dwFrameResponseNotifyMaskedCnt++;
    }
    EC_T_VOID  DecFrameResponseNotifyMaskedCnt(EC_T_VOID)
    {
        if (0 == m_dwFrameResponseNotifyMaskedCnt)
        {
            OsDbgAssert(EC_FALSE);
        }
        else
        {
            m_dwFrameResponseNotifyMaskedCnt--;
        }
    }

    /* cyclic master timer settings */
    EC_T_INT GetDefaultMbxPollingTime()  { return m_dwDefaultMbxPolling; }

    EC_T_WORD GetMaxBusSlaves()  { return m_MasterConfigEx.wMaxBusSlaves; }

    /********************************************************************************/
    /** \brief Returns the current state of the EtherCAT master state machine
    *
    * \return Current state of the EtherCAT master.
    */
    EC_T_DWORD GetStateChangeDefaultTimeout( EC_T_VOID )  { return m_dwStateChangeDefaultTimeout; };
    EC_MASTER_STATE GetCurMasterState() {if (m_bMasterTimerEnabled) { return m_eCurMasterLogicalState; }
                                         else { return EC_MASTER_STATE_DISABLED;}};

    EC_T_WORD GetMasterDeviceState( EC_T_VOID ) { return m_wCurMasterDeviceState; };
    EC_T_WORD GetMasterReqDeviceState( EC_T_VOID )  { return m_wReqMasterDeviceState; };
    EC_T_WORD GetMasterLowestDeviceState( EC_T_VOID )  { return (DEVICE_STATE_UNKNOWN == m_wCurMasterDeviceState)?DEVICE_STATE_UNKNOWN:EC_MIN(m_wCurMasterDeviceState, m_wReqMasterDeviceState); };
    EC_T_WORD GetMasterHighestDeviceState( EC_T_VOID ) { return (DEVICE_STATE_UNKNOWN == m_wCurMasterDeviceState)?m_wReqMasterDeviceState:EC_MAX(m_wCurMasterDeviceState, m_wReqMasterDeviceState); };
    EC_T_BOOL GetMasterStateMachineResult(EC_T_VOID) { return m_dwMasterStateMachineResult; }

    CEcSlave* GetSlaveByAddr(EC_T_WORD wFixedAddr);
    CEcSlave* GetSlaveByCfgFixedAddr(EC_T_WORD wFixedAddr);
    CEcSlave* GetSlaveByIndex(EC_T_DWORD dwIdx) { return (dwIdx < m_nCfgSlaveCount)?m_ppEcSlave[dwIdx]:EC_NULL; }
    CEcSlave* GetSlaveByAutoIncAddr(EC_T_WORD wAutoIncAddr);
    CEcSlave* GetSlaveByCfgAutoIncAddr(EC_T_WORD wCfgAutoIncAddr);
    CEcSlave*    GetCfgSlaveBySlaveId(EC_T_DWORD dwSlaveId) { return GetSlaveByIndex(dwSlaveId); }
    CEcBusSlave* GetBusSlaveBySlaveId(EC_T_DWORD dwSlaveId);
    
    EC_T_DWORD GetSlaveIdByFixedAddress(EC_T_WORD wFixedAddress);

    /********************************************************************************/
    /** \brief Gets the number of EtherCAT slaves.
    *
    * \return Number of EtherCAT slaves.
    */
    EC_T_VOID  SetCfgSlaveCount(EC_T_DWORD dwSlaveCount){ m_nCfgSlaveCount = dwSlaveCount; } /* only used by eCnfType_GenPreopENI */
    EC_T_DWORD GetCfgSlaveCount() { return m_nCfgSlaveCount; }
    EC_T_DWORD GetMbSlaveCount() { return m_nEcMbSlave; }

    EC_T_VOID  SetTimeLimit(EC_T_DWORD dwTimeLimit) { m_pMasterDesc->dwTimeLimit = dwTimeLimit; }
    EC_T_DWORD GetTimeLimit() { return m_pMasterDesc->dwTimeLimit; }

    /*---------------------------------------------------------------------------*/
    
    /********************************************************************************/
    /** \brief Creates an EtherCAT Slave.
    *
    * \param pEniSlave Pointer to a EC_T_ENI_SLAVE structure describing the Slave to create.
    *
    * \return 
    */
    CEcSlave*  CreateSlave(EC_T_ENI_SLAVE* pEniSlave);
    /********************************************************************************/
    /** \brief Adds an EtherCAT Slave to the Master.
    *
    * \param pSlave Pointer to a Slave object.
    *
    * \return
    */
    EC_T_DWORD AddSlaveToList(CEcSlave* pSlave);
    EC_T_VOID  RemoveSlaveFromList(CEcSlave* pSlave);
    
    /********************************************************************************/
    /** \brief Create the EtherCAT commands that should be sent cyclically.
    *
    * \param pDesc Description of the cyclic EtherCAT commands.
    *
    * \return Result of the call.
    */
    EC_T_DWORD CreateCyclicCmds(  EC_T_INT nNumCycEntries
                                , EC_T_INT nCycEntryIndex
                                , EcCycCmdConfig* pDesc
#if (defined INCLUDE_WKCSTATE)
                                , EC_T_WORD* pwWkcStateDiagOffs
#endif
                                );
    EC_T_DWORD CreateCycFrameTemplates(CYCLIC_ENTRY_MGMT_DESC* pEcCycCmdDesc);

#if (defined INCLUDE_VARREAD)
    EC_T_VOID  SwapData(CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc, EC_T_BOOL bOutputDataOnly, EC_T_BYTE* pbyPDBase);
#endif
    /* Slave-to-Slave communication */
    EC_T_DWORD S2SOptimizeCopyInfos(CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc);
    EC_T_DWORD S2SCopyData(CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc, EC_T_BYTE* pbyPDInpBase, EC_T_BYTE* pbyPDOutBase);

#if (defined INCLUDE_MULTIPLE_CYC_ENTRIES)
    EC_T_DWORD GetNumCycConfigEntries(EC_T_VOID) { return m_dwNumCycConfigEntries; }
#endif
    CYCLIC_ENTRY_MGMT_DESC* GetCycEntryDesc(EC_T_INT nCycEntryIndex);
    EC_T_WORD GetCycConfigEntriesNumCmds();

    /********************************************************************************/
    /** \brief Returns the start address of the FMMU, that is configured to the status "written" bits of the of the mailbox out sync managers
    *
    * \return The start address of the FMMU, that is configured to the status "written" bits of the of the mailbox out sync managers
    */
    EC_T_DWORD  GetLogAddressMBoxStates()
                            { return m_pMasterDesc->dwLogAddressMBoxStates; }

    /********************************************************************************/
    /** \brief Returns the size of the FMMU, that is configured to the status "written" bits of the of the mailbox out sync managers    
    *
    * \return Size of the FMMU, that is configured to the status "written" bits of the of the mailbox out sync managers
    */
    EC_T_WORD GetSizeOfAddressMBoxStates()
                            { return m_pMasterDesc->wSizeAddressMBoxStates; }

    EC_T_DWORD Notify(EC_T_DWORD dwNotificationCode, EC_T_DWORD dwClientId, 
        EC_T_NOTIFICATION_DATA* pNotificationData, EC_T_DWORD dwNotificationSize);

    EC_T_NOTIFICATION_INTDESC* AllocNotification(EC_T_VOID);

    EC_T_VOID   FreeNotification(EC_T_NOTIFICATION_INTDESC* pNotificationDesc) 
    { m_pNotificationStore[pNotificationDesc->dwIdx].bUsed = EC_FALSE; }

    EC_T_DWORD  NotifyAndFree(EC_T_DWORD dwNotificationCode, 
        EC_T_NOTIFICATION_INTDESC* pNotificationDesc, EC_T_DWORD dwNotificationSize);

    EC_T_DWORD  GetMasterCycConfig(EC_T_DWORD dwCycEntryIndex, EC_T_CYC_CONFIG_DESC* pCycConfigDesc);
    EC_T_BOOL   IsSlaveToSlaveCommConfigured(EC_T_VOID);

    static EC_T_INTFEATUREFIELD_DESC* CreateFeatureField(EC_T_DWORD dwSupportedFeatures, struct _EC_T_MEMORY_LOGGER* pMLog);
    static EC_T_VOID FreeFeatureField(EC_T_INTFEATUREFIELD_DESC* pFeature, struct _EC_T_MEMORY_LOGGER* pMLog);
#if (defined VLAN_FRAME_SUPPORT)
    static EC_T_VOID  InsertVlanInFeatureField(EC_T_INTFEATUREFIELD_DESC* pFeatures, EC_T_BOOL bVLANEnabled, EC_T_WORD wVLANId, EC_T_BYTE byVLANPrio)
    {
        pFeatures->bVLANEnabled = bVLANEnabled;
        pFeatures->wVLANId      = wVLANId;
        pFeatures->byVLANPrio   = byVLANPrio;
    }
#endif
    EC_T_INTFEATUREFIELD_DESC* GetFeatureField(EC_T_VOID);
    EC_T_DWORD  SetFeatureField(EC_T_INTFEATUREFIELD_DESC* pFeatures);

    EC_T_BYTE*  GetPDOutPtr()   { return m_oMemProvider.pbyPDOutData; }
    EC_T_DWORD  GetPDOutSize()  { return m_oMemProvider.dwPDOutDataLength; }
    EC_T_BYTE*  GetPDInPtr()    { return m_oMemProvider.pbyPDInData; }
    EC_T_DWORD  GetPDInSize()   { return m_oMemProvider.dwPDInDataLength; }

    EC_T_DWORD  GetPDIn(EC_T_BYTE** ppbyPDIn, EC_T_DWORD* pdwPDInSize)
    {
        (*ppbyPDIn)  = m_oMemProvider.pbyPDInData;  (*pdwPDInSize)  = m_oMemProvider.dwPDInDataLength;
        return EC_E_NOERROR;
    }

    EC_T_DWORD  GetPDOut(EC_T_BYTE** ppbyPDOut, EC_T_DWORD* pdwPDOutSize)
    {
        (*ppbyPDOut) = m_oMemProvider.pbyPDOutData; (*pdwPDOutSize) = m_oMemProvider.dwPDOutDataLength;
        return EC_E_NOERROR;
    }
#if (defined INCLUDE_WKCSTATE)
    EC_T_BYTE*  GetDiagnosisImagePtr() { return m_pbyDiagnosisImage; }
#endif
    EC_T_DWORD SetPDMemProvider( EC_T_MEMPROV_DESC* pMemProv);
    EC_T_VOID  SetCycFrameReceivedCallback(EC_T_CYCFRAME_RX_CBDESC *pCycRxDesc)
        { m_pvCycFrameRxCallBack      = pCycRxDesc->pfnCallback; 
          m_pvCycFrameRxCallBackParam = pCycRxDesc->pCallbackParm; }

    EC_T_VOID  CycFrameReceivedCallback(EC_T_VOID)
    { if(m_pvCycFrameRxCallBack != EC_NULL) {m_pvCycFrameRxCallBack(m_pvCycFrameRxCallBackParam);}; }


    EC_T_VOID  SetProcessDataCompatMode(EC_T_VOID)
        { m_bPdOffsetCompMode = EC_TRUE;
          m_dwCopyOffset = 0; }

    EC_T_DWORD SetPDOutPointer(EC_T_PBYTE pbyBuffer, EC_T_DWORD dwBufSize);
    EC_T_DWORD SetPDInPointer( EC_T_PBYTE pbyBuffer, EC_T_DWORD dwBufSize);

    EC_T_PBYTE GetPDOutBasePointer();
    EC_T_PBYTE GetPDInBasePointer();

    EC_T_VOID ReturnPDOutBasePointer(EC_T_VOID);
    EC_T_VOID ReturnPDInBasePointer(EC_T_VOID);

#if (defined INCLUDE_WKCSTATE)
    EC_T_DWORD CreateDiagnosisImage(EC_T_INT nNumCyclicCmds);
#endif

    struct _EC_T_INIT_MASTER_PARMS* GetInitMasterParms(EC_T_VOID);
    EC_T_DWORD GetMaxSlaveProcessedPerCycle()           {return m_MasterConfig.dwMaxSlavesProcessedPerCycle;} 

    EC_T_DWORD GetMbxCmdTimeout(EC_T_VOID)              { return m_dwMbxCmdTimeout; }

#if (defined INCLUDE_RED_DEVICE)
#if (defined INSTRUMENT_MASTER)
    virtual EC_T_BOOL IsRedConfigured(EC_T_VOID)        { return m_poEcDevice->m_bRedConfigured; }
#else
    EC_T_BOOL IsRedConfigured(EC_T_VOID)                { return m_poEcDevice->m_bRedConfigured; }
#endif
#endif /* INCLUDE_RED_DEVICE */
    CEcDevice*      EthDevice(EC_T_VOID)                { return m_poEcDevice; }

    EC_T_VOID       ResetCyclicFrmMgr();

#if (defined INCLUDE_MASTER_OBD)
    EC_T_DWORD      SDOSetLinkLayerParms(EC_T_LINK_PARMS *pLinkParms);
    EC_T_VOID       SDOSetLinkLayerPolling(EC_T_BOOL bActive);
    EC_T_VOID       SDOSetLinkLayerAllocSupport(EC_T_BOOL bActive);

    EC_T_VOID       SDOSetMacAddress(EC_T_BOOL bRedIf, EC_T_BYTE byMac[6]);
    EC_T_VOID       SDOSetCfgMacAddress(EC_T_VOID);
    EC_T_VOID       SDOClearSlaves(EC_T_WORD wNumOfSlavesToClean);
    EC_T_VOID       SDOAddSlave(
                                            EC_T_DWORD dwSlaveId,
                                            
                                            EC_T_DWORD dwVendorId,
                                            EC_T_DWORD dwProdCode,
                                            EC_T_DWORD dwRevisionNo,
                                            EC_T_DWORD dwSerialNo,
                                            EC_T_WORD  wMbxSupportedProtocols,
                                            
                                            EC_T_WORD  wAutoIncAddr,
                                            EC_T_WORD  wFixedAddr,
                                            EC_T_WORD  wAliasAddr,

                                            EC_T_WORD  wPortState,
                                            EC_T_BOOL  bDCSupport,
                                            EC_T_BOOL  bDC64Support,

                                            EC_T_DWORD dwSbErrorCode,
                                            EC_T_BYTE  byFmmuEntitites,
                                            EC_T_BYTE  bySyncManEntities,
                                            EC_T_BYTE  byRAMSizeKb,
                                            EC_T_BYTE  byPortDescriptor,
                                            EC_T_BYTE  byESCType,
                                            CEcSlave*  pCfgSlave
                                       );
    EC_T_VOID       SDOAddConfiguredSlave(
                                            EC_T_DWORD dwSlaveId,
                                            CEcSlave*  pCfgSlave
                                       );
    EC_T_VOID       SDOAddConnectedSlave(
                                            CEcBusSlave* pCfgBus
                                       );


    EC_T_VOID       SDOSetSlaveError(EC_T_DWORD dwSDOSlaveIdx, EC_T_DWORD dwALStatus, EC_T_DWORD dwStatusCode);
    EC_T_VOID       SDOSetSlaveAmount(EC_T_DWORD dwSlaves, EC_T_DWORD dwDCSlaves);
    EC_T_DWORD*     SDOGetTXFrameCrtPtr(EC_T_VOID);
    EC_T_DWORD*     SDOGetRXFrameCrtPtr(EC_T_VOID);
    EC_T_DWORD*     SDOGetCycFrameCrtPtr(EC_T_VOID);
    EC_T_DWORD*     SDOGetCycDgramCrtPtr(EC_T_VOID);
    EC_T_DWORD*     SDOGetAcycFrameCrtPtr(EC_T_VOID);
    EC_T_DWORD*     SDOGetAcycDgramCrtPtr(EC_T_VOID);

    EC_T_DWORD*     SDOGetBytesSecondCrtPtr(EC_T_VOID)  { return &m_dwBytesPerSecondCnt;}
    
    EC_T_DWORD*     SDOGetBytesCycleCrtPtr(EC_T_VOID)   { return &m_dwBytesPerCycleCnt;}
    EC_T_VOID       BusLoadMeaCycleComplete(EC_T_VOID);
    
    EC_T_VOID       SDOSetConfigCRC32ChkSum(EC_T_DWORD dwCRC32Sum);
    EC_T_VOID       SDOEntrySet_SetDCBusy(EC_T_BOOL bDCBusy);

    EC_T_WORD       GetMasterStChngRequest(EC_T_VOID)
                        { return m_wStateChngReq; }
    EC_T_VOID       SetMasterStChngRequest(EC_T_WORD wValue)
                        { m_wStateChngReq = wValue; }

    CEcSdoServ*     m_pSDOService;
#endif /* INCLUDE_MASTER_OBD */

    EC_T_VOID       MasterInfoSecondComplete(EC_T_VOID);
#if (defined INCLUDE_MAILBOX_STATISTICS)
    EC_T_VOID       MailboxStatisticsInc(EC_T_DWORD dwMbxIdx, EC_T_BOOL bRead, EC_T_DWORD dwBytes)
                        {
                            EC_T_STATISTIC_TRANSFER* pStats = (bRead ? &m_MailboxStatistics[dwMbxIdx].Read : &m_MailboxStatistics[dwMbxIdx].Write);
                            EC_T_STATISTIC_TRANSFER_CUR* pStatsCur = (bRead ? &m_MailboxStatisticsCur[dwMbxIdx].Read : &m_MailboxStatisticsCur[dwMbxIdx].Write);

                            /* Total */
                            pStats->Cnt.dwTotal++;
                            pStats->Bytes.dwTotal = pStats->Bytes.dwTotal + dwBytes;

                            /* Last Second */
                            pStatsCur->dwCnt++;
                            pStatsCur->dwBytes = pStatsCur->dwBytes + dwBytes;
                        }
    EC_T_VOID       ClearMailboxStatisticsCounters(EC_T_UINT64 qwCountersMask);
#endif /* INCLUDE_MAILBOX_STATISTICS */

    EC_T_VOID       RecalculateCycCmdWkc(CEcSlave* pCfgSlave);

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
    EC_T_DWORD      HCCreateGroups(EC_T_DWORD dwHCGroupAmount)
                        { return m_oHotConnect.CreateHCGroups(dwHCGroupAmount); }
    EC_T_HOTCONNECT_GROUP*
                    HCGetGroup(EC_T_DWORD dwGrpIndex)
                        { return m_oHotConnect.GetGroup(dwGrpIndex);}
    EC_T_DWORD      RecalculateHCWKC(EC_T_BOOL bAuto);

    EC_T_VOID       CreateCycCmdHC(EC_T_DWORD dwNumCmds);
    EC_T_VOID       AddCycCmdHC(EcCycCmdConfigDesc* pEcCmdCfgDesc);

    EC_T_DWORD      SetHCMode(EC_T_EHOTCONNECTMODE oMode);
    EC_T_EHOTCONNECTMODE 
                    GetHCMode(EC_T_VOID);
    
    EC_T_DWORD      HCGetNumGroupMembers(EC_T_DWORD dwGroupIndex)
                        { return m_oHotConnect.GetNumGroupMembers( dwGroupIndex ); }
    EC_T_DWORD      HCGetSlaveIdsOfGroup(EC_T_DWORD dwGroupIndex, EC_T_DWORD* adwSlaveId, EC_T_DWORD dwMaxNumSlaveIds)
                        { return m_oHotConnect.GetSlaveIdsOfGroup( dwGroupIndex, adwSlaveId, dwMaxNumSlaveIds ); }

    EC_T_DWORD      HCGetGroupAmount(EC_T_VOID)
                        { return m_oHotConnect.GetGroupAmount(); }
    EC_T_DWORD      HCTopoChangeTimeoutValue(EC_T_VOID)
                        { return m_dwHCDetectionTimeout; }
    EC_T_DWORD      HCConfigureTimeouts(EC_T_HC_CONFIGURETIMEOUTS_DESC* pConfig);

    EC_T_DWORD      StartHCManualContinue(EC_T_VOID);
#endif
    EC_T_DWORD      StartHCTopologyChange(EC_T_VOID);
    EC_T_DWORD      WaitForHCStateMachine(EC_T_VOID);
    EC_T_DWORD      ReleaseHCStateMachine(EC_T_DWORD dwResult);

    EC_T_VOID       CreatePortList(EC_T_VOID);

    EC_T_BOOL       TopologyChangedDetected(EC_T_VOID) {return  m_pBusTopology->TopologyChangedDetected();};

    EC_T_BOOL       SetDLStatusInterruptMasked(EC_T_BOOL bMasked) {EC_T_BOOL bMaskedOld = m_bDLStatusIRQMasked; m_bDLStatusIRQMasked = bMasked; return bMaskedOld;};
    EC_T_BOOL       DLStatusInterruptDetected(EC_T_VOID) {return m_bDLStatusIRQ;};
    EC_T_VOID       DLStatusInterruptDone(EC_T_VOID){ m_bDLStatusIRQ = EC_FALSE;};

	/* AL Status Interrupt is present. */
    EC_T_BOOL       ALStatusInterruptDetected(EC_T_BOOL bAck)
                                {EC_T_BOOL bInt = m_bALStatusIRQ; if( bAck ) { m_bALStatusIRQ = EC_FALSE; } return bInt; };

    EC_T_VOID       SetMsecCounter(EC_T_DWORD dwMsecVal)
                                {m_dwMsecCounter = dwMsecVal;}
    EC_T_DWORD*     GetMsecCounterPtr(EC_T_VOID)
                                { return &m_dwMsecCounter; }
    
    EC_T_DWORD              m_dwCfgSlaveOnTimerIdx;       /* slave slicing for CEcSlave::OnTimer()    */
    EC_T_DWORD              m_dwMsecCounter;

    EC_T_DWORD              GetApplicationVersion(EC_T_VOID)
                                { return m_dwAppVersion; }

    EC_T_MASTER_CONFIG                          m_MasterConfig;                 /* static master configuration */

    EC_T_VOID       SetNotAllDevOpNotificationEnabled(EC_T_VOID) { m_bNotAllDevOpNotificationEnabled = EC_TRUE; }

#if (defined INCLUDE_FRAME_LOGGING)
    EC_T_VOID       SetLogFrameCallback(EC_T_PFLOGFRAME_CB pvLogFrameCallBack, EC_T_VOID* pvContext)
                        { m_pvLogFrameCallBackContext      = pvContext; m_pvLogFrameCallBack = pvLogFrameCallBack; }
    EC_T_PFLOGFRAME_CB  GetLogFrameCallback(EC_T_VOID)   { return m_pvLogFrameCallBack; }
    EC_T_VOID       ClrLogFrameCallback(EC_T_VOID)
                        { m_pvLogFrameCallBack = EC_NULL; m_pvLogFrameCallBackContext      = EC_NULL; }
#endif

#if (defined INCLUDE_FORCE_PROCESSDATA)
    EC_T_DWORD      ManageForceData(EC_T_FORCE_RELEASE_PROCESS_DATA_REQ*);
#endif

    friend class CEcSlave;
    friend class CEcMbSlave;

    friend class CEcBusTopology;
    friend class CEcDistributedClocks;
    friend class CEcDevice;

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
    friend class CEcConfigXpat;
#endif
    friend class CEcHotConnect;

    /********************************************************************************/
    /** \brief Is called when a new message in a slave mailbox has been indicated.
    *
    * \param pEcCmdHeader Header of the command.
    *
    * \return Success of the call.
    */
    EC_T_VOID CheckLogicalStateMBox(PETYPE_EC_CMD_HEADER pEcCmdHeader, EC_T_WORD wEcCmdLen);

    /********************************************************************************/
    /** \brief Register or unregister mailbox polling for the specified EtherCAT slave.
    *
    * If mailbox polling is registered the master checks the state of the mailbox cyclically.
    *
    *\param pSlave Pointer to the EtherCAT slave.
    *\param slaveAddressMBoxState
    *\param bUnregister If FALSE mailbox polling is registered for this slave otherwise mailbox polling is unregistered.
    *
    * \return Success of the call.
    */
    EC_T_BOOL RegisterLogicalStateMBoxPolling(CEcMbSlave* pSlave, EC_T_WORD slaveAddressMBoxState, EC_T_BOOL bUnregister);

#if (defined INCLUDE_EOE_SUPPORT)
    /********************************************************************************/
    /** \brief Gets an interface to the ethernet switch.
    *
    * \return Interface to the ethernet switch.
    */
    CEcEthSwitch* EthernetSwitch()
        { return m_poEcDevice ? m_poEcDevice->EthernetSwitch() : EC_NULL; }
#endif /* INCLUDE_EOE_SUPPORT */

    /********************************************************************************/
    /** \brief Queue a new EtherCAT command request.
    *
    * \param bAcycCountIgnored
    * \param pSlave             Pointer to slave that initiated the request.
    * \param invokeId           Invoke Id of the request.
    * \param cmd                EtherCAT command type.
    * \param adp                Address of the slave the request is intended for.
    * \param ado                Local offset.
    * \param len                Length of the EtherCAT data
    * \param pData              data to be copied to the end of the command
    * \param cnt                expected working counter
    * \param pMbox              pointer to Mailbox Header
    * \param pEthernet          pointer to Ethernet over EtherCAT header
    * \param pCanOpen           pointer to CANopen over EtherCAT header
    * \param pSDO               pointer to CANopen Sdo header
    *
    * \return Result of the call.
    */
    EC_T_DWORD  QueueEcatCmdReq(
                                   EC_T_BOOL    bAcycCountIgnored,
                                   CEcSlave*    pSlave,
                                   EC_T_DWORD   invokeId,
                                   EC_T_BYTE    cmd,
                                   EC_T_WORD    adp,
                                   EC_T_WORD    ado,
                                   EC_T_WORD    len,
                                   EC_T_VOID*   pData,
                                   EC_T_WORD    cnt=0,
                                   ETHERCAT_MBOX_HEADER*     pMbox     =EC_NULL,
                                   ETHERCAT_ETHERNET_HEADER* pEthernet =EC_NULL,
                                   ETHERCAT_CANOPEN_HEADER*  pCANopen  =EC_NULL,
                                   EC_SDO_HDR*               pSDO      =EC_NULL
                                  ,EC_FOE_HDR*               pFoE      =EC_NULL
#ifdef INCLUDE_SOE_SUPPORT
                                  ,EC_SOE_HDR*               pSoE      =EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                  ,EC_AOE_HDR*               pAoE      =EC_NULL
                                  ,EC_AOE_CMD_HDR*           pAoeCmd   =EC_NULL
#endif
                                  );

    EC_INSTRUMENT_MOCKABLE_FUNC EC_T_DWORD QueueRegisterCmdReq(
                                     CEcSlave*                  pSlave,
                                     EC_T_DWORD                 invokeId,
                                     EC_T_BYTE                  cmd,
                                     EC_T_WORD                  adp,
                                     EC_T_WORD                  ado,
                                     EC_T_WORD                  len,
                                     EC_T_VOID*                 pData
                                    );

    EC_T_DWORD  PrepareFreeFrameSpace(EC_T_WORD  wMinFrameSpace);

    EC_T_DWORD  EcatFlushCurrPendingSlaveFrameStamp( 
                                    EC_PF_TIMESTAMP     pfnTimeStamp     = EC_NULL
                                   ,EC_T_PVOID          pvTimeStampCtxt  = EC_NULL
                                   ,EC_T_DWORD*         pdwTimeStamp     = EC_NULL
                                   ,EC_T_WORD           wTimestampOffset = 0
                                   ,EC_T_WORD           wTimestampSize   = 0
                                   ,EC_T_DWORD          dwRepeatCnt      = 0
                                   ,EC_T_WORD           wRetryCnt        = 0
                                   );
    EC_T_DWORD  EcatFlushCurrPendingSlaveFrame(EC_T_VOID)
        { return EcatFlushCurrPendingSlaveFrameStamp(); }
    EC_T_DWORD  EcatFlushCurrPendingSlaveFrameNoMonitoring(EC_T_VOID)
        { return EcatFlushCurrPendingSlaveFrameStamp(EC_NULL, EC_NULL, EC_NULL, 0, 0, 0, 0xFFFF); }
    EC_T_DWORD  EcatFlushCurrPendingSlaveFrameRepeatInLinkLayer(EC_T_DWORD dwRepeatCnt)
        { return EcatFlushCurrPendingSlaveFrameStamp(EC_NULL, EC_NULL, EC_NULL, 0, 0, dwRepeatCnt); }

    EC_T_VOID   EcatLockCurrPendingSlaveFrame(EC_T_VOID)
          { OsLock(m_poFreeSlaveFrameDescLock); }
    EC_T_VOID   EcatUnlockCurrPendingSlaveFrame(EC_T_VOID)
          { OsUnlock(m_poFreeSlaveFrameDescLock); }

    /********************************************************************************/
    /** \brief Processes the response of an EtherCAT command.
    *
    * \param result
    * \param pSlaveCmd
    * \param pEcCmdHeader
    *
    * \return 
    */
    EC_T_VOID   ProcessCmdResult(EC_T_DWORD result, PECAT_SLAVECMD_DESC pSlaveCmd, PETYPE_EC_CMD_HEADER pEcCmdHeader); 
    
    EC_T_VOID   ProcessIrq(EC_T_WORD wIrq);

    EC_T_BOOL   GetSlaveErrorDetected(EC_T_VOID)
        { return m_bSlaveErrorDetected; }
    EC_T_VOID   SetSlaveErrorDetected(EC_T_BOOL bV)
        { m_bSlaveErrorDetected = bV; }

    EC_T_DWORD  ResetSlaveById(EC_T_DWORD dwSlaveId);

#if (defined INCLUDE_MASTER_OBD)
    EC_T_DWORD  MasterLocalMailboxCmd(PEcMailboxCmd pCmd);
#endif
     
    EC_T_VOID   SetConfiguratorOffsets(EC_T_DWORD dwMinRecv, EC_T_DWORD dwMinCycOut, EC_T_DWORD dwMinSend, EC_T_DWORD dwMinCycIn);
    EC_T_VOID   SetConfiguratorOffsets(EC_T_DWORD dwCopyOffset) 
        { if(!m_bPdOffsetCompMode) m_dwCopyOffset = dwCopyOffset; }
    
    /********************************************************************************/
    /** \brief Return e.g. ET9000-compatible copy offset 
     *  Needed to adjust PD offset according to cyclic cmd offset (ProcessImage/Inputs/Variable/BitOffs vs. Cyclic/Cmd/InputOffs).
     *  \return ETYPE_EC_CMD_HEADER_LEN for ET9000, 0 for EC-Engineer
     */
    EC_T_DWORD  GetCopyOffset(EC_T_VOID) 
        { return m_dwCopyOffset; }
    EC_T_BOOL   IsOnlyProcDataInImage(EC_T_VOID)
        { return m_bOnlyProcDataInImage; }

    EC_T_DWORD  GetAllSlaveInfo(EC_T_BOOL bFixedAddress,   EC_T_WORD wSlaveAddress, EC_T_GET_SLAVE_INFO* pSlaveInfo);
    EC_T_DWORD  GetCfgSlaveInfo(EC_T_BOOL bStationAddress, EC_T_WORD wSlaveAddress, EC_T_CFG_SLAVE_INFO* pSlaveInfo);
    EC_T_DWORD  GetBusSlaveInfo(EC_T_BOOL bStationAddress, EC_T_WORD wSlaveAddress, EC_T_BUS_SLAVE_INFO* pSlaveInfo);


    EC_T_DWORD  GetPortState(EC_T_BOOL bFixedAddress, EC_T_WORD wSlaveAddress, EC_T_WORD* pwPortState);

#if (defined INCLUDE_VARREAD)
    EC_T_VOID   SetAdditionalVariablesForSpecificDataTypesEnabled(EC_T_BOOL bAddVarsForSpecificDataTypes) {m_bAddVarsForSpecificDataTypes = bAddVarsForSpecificDataTypes;}
    EC_T_DWORD  AddProcessVarInfoEntry(EC_T_PD_VAR_INFO_INTERN* pProcessVarInfoEntry, EC_T_BOOL bIsInputVar);

    EC_T_DWORD  GetSlaveProcVarInfoNumOf  (EC_T_BOOL bFixedAddress, EC_T_WORD wSlaveAddress, 
        EC_T_WORD* pwSlaveVarInfoNumOf, EC_T_WORD*  pwIndexFirst, EC_T_WORD*  pwIndexLast, EC_T_BOOL bInputProcessVariables);
    EC_T_DWORD  GetSlaveProcVarInfoEntries( EC_T_BOOL bFixedAddress, EC_T_WORD wSlaveAddress, 
        EC_T_PROCESS_VAR_INFO* pSlaveVarInfoEntries, EC_T_PROCESS_VAR_INFO_EX* pSlaveVarInfoEntriesEx, 
        EC_T_WORD  wMaxEntriesToRead, EC_T_WORD* pwReadEntries, EC_T_BOOL bInputProcessVariables);
    EC_T_DWORD  FindProcVarByName(EC_T_CHAR*  szVariableName, EC_T_PROCESS_VAR_INFO* pProcessVarInfo, EC_T_PROCESS_VAR_INFO_EX* pProcessVarInfoEx, EC_T_BOOL bInputProcessVariables);
#endif

public:
    EC_T_DWORD EcTransferRawCmd( EC_T_DWORD dwUniqueClntId, EC_T_WORD wInvokeId, EC_T_BYTE byCmd, EC_T_DWORD dwMemoryAddress, 
                                         EC_T_VOID* pvData, EC_T_WORD wLen, EC_T_DWORD dwTimeout );

    EC_T_WORD GetBrdAlStatusWkc(    EC_T_VOID   ){ return  m_wALStatusWkc; }

    EC_T_WORD GetBrdAlStatusReg(    EC_T_VOID   ){ return m_wAlStatusReg; }

    EC_T_DWORD PrepareEEPRomRead(       EC_T_BOOL   bFixedAddressing
                                               ,EC_T_WORD   wSlaveAddress
                                               ,EC_T_WORD   wEEPRomStartOffset
                                               ,EC_T_WORD*  pwReadData
                                               ,EC_T_DWORD  dwReadLen
                                               ,EC_T_DWORD* pdwNumOutData
                                               ,EC_T_DWORD  dwTimeout               );

    EC_T_DWORD PrepareEEPRomWrite(      EC_T_BOOL   bFixedAddressing
                                               ,EC_T_WORD   wSlaveAddress
                                               ,EC_T_WORD   wEEPRomStartOffset
                                               ,EC_T_WORD*  pwWriteData
                                               ,EC_T_DWORD  dwWriteLen
                                               ,EC_T_DWORD  dwTimeout               );

    EC_T_DWORD PrepareEEPRomReload(     EC_T_BOOL   bFixedAddressing
                                               ,EC_T_WORD   wSlaveAddress
                                               ,EC_T_DWORD  dwTimeout               );

    EC_T_DWORD PrepareSlaveReset(       EC_T_BOOL   bFixedAddressing
                                               ,EC_T_WORD   wSlaveAddress
                                               ,EC_T_DWORD  dwTimeout               );

    EC_T_DWORD PrepareEEPRomAssign(     EC_T_BOOL   bFixedAddressing
                                               ,EC_T_WORD   wSlaveAddress
                                               ,EC_T_BOOL   bSlavePDIAccessEnable
                                               ,EC_T_BOOL   bForceAssign
                                               ,EC_T_DWORD  dwTimeout               );

    EC_T_DWORD PrepareEEPRomActive(     EC_T_BOOL   bFixedAddressing
                                               ,EC_T_WORD   wSlaveAddress
                                               ,EC_T_BOOL*  pbSlavePDIAccessActive
                                               ,EC_T_DWORD  dwTimeout               );

    EC_T_DWORD StartEEPROMOperation(    EC_T_VOID                                   );
    EC_T_DWORD PollEEPROMOperation(                                                 );

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
    static EC_T_VOID tPassThroughServer(EC_T_VOID* pvThreadParam);

    EC_PTS_STATE PassThroughSrvGetStatus(EC_T_VOID);

    EC_T_DWORD PassThroughSrvStart(     EC_T_PTS_SRV_START_PARMS* poPtsStartParams, 
                                        EC_T_DWORD dwTimeout);

    EC_T_DWORD PassThroughSrvStop(      EC_T_DWORD dwTimeout);

    EC_T_DWORD PassThroughSrvEnable(    EC_T_DWORD dwTimeout);

    EC_T_DWORD PassThroughSrvDisable(   EC_T_DWORD dwTimeout);
#endif

#if (defined INCLUDE_VOE_SUPPORT)
    EC_T_DWORD GetVoeMbxTferObjBySlaveId(EC_T_DWORD dwSlaveID, EC_T_VOID** ppvVoeMbxTferObj);
#endif

#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_DWORD GetSlaveNetId(           EC_T_DWORD dwSlaveId, EC_T_AOE_NETID* poAoeNetId);
#endif

    EC_T_VOID  TriggerCycFramesAdjustment(EC_T_VOID)
        { m_wCycFramesDeviceState = EC_MASTER_STATE_UNKNOWN; }
    EC_T_VOID  AdjustCycFramesToState(EC_T_WORD  wState);

    struct _EC_T_MEMORY_LOGGER* GetMemLog() { return m_poEcDevice ? m_poEcDevice->GetMemLog() : EC_NULL; }

    EC_T_VOID  ConfigExtendApply(CEcSlave*** papoSlaves, EC_T_WORD wSlaveCnt, CEcMbSlave*** papoMbSlaves, EC_T_WORD wMbSlaveCnt, EC_T_WORD* pwMaxNumSlaves);

    EC_T_DWORD ConfigGenerate(CEcConfigData* poConfigData, CEcBusSlave* poBusSlaveList, EC_T_STATE eMaxEcatState, EC_T_BOOL bExtendConfig = EC_FALSE);
    EC_T_DWORD ConfigGenerateMaster(CEcConfigData* poConfigData, CEcBusSlave* poBusSlaveList);
    EC_T_DWORD ConfigGenerateEniSlave(CEcConfigData* poConfigData, CEcBusSlave* poBusSlaveList, EC_T_STATE eReqState);
    EC_T_DWORD ConfigGenerateProcessImage(CEcConfigData* poConfigData, CEcBusSlave* poBusSlaveList, EC_T_STATE eReqState);
    EC_T_DWORD ConfigGenerateCyclic(CEcConfigData* poConfigData, EC_T_STATE eReqState);

private:  
#if (defined INCLUDE_RED_DEVICE)
    EC_T_VOID RedBreakFrameMerge(PETHERNET_FRAME pFrame, PETHERNET_FRAME pRedFrame);
    EC_T_VOID RedGetNumSlaves(PETHERNET_FRAME pFrame);
#if (defined INCLUDE_MASTER_OBD)
    EC_T_VOID RedUpdateOd();
#endif
#endif /* INCLUDE_RED_DEVICE */

#if (defined INCLUDE_FORCE_PROCESSDATA)
    EC_T_DWORD ForceProcessDataOutputs(EC_T_BYTE*             pbyPDOutBase);
    EC_T_DWORD ForceProcessDataInputs(EC_T_BYTE*              pbyPDInpBase);
#endif

#if (defined INCLUDE_OEM)
    EC_T_UINT64                                 m_qwOemKey;
#endif /* INCLUDE_OEM */

    PECAT_SLAVEFRAME_DESC AllocSlaveFrameDesc(EC_T_VOID)
        {
            PECAT_SLAVEFRAME_DESC pSlvFrmDesc = EC_NULL;

            /* get new current frame descriptor from free list. */
            if (!IsListEmpty(&m_FreeSlaveFrameDescList))
            {
                pSlvFrmDesc = (PECAT_SLAVEFRAME_DESC)RemoveHeadList(&m_FreeSlaveFrameDescList);

                /* initialize */
                pSlvFrmDesc->bIgnoreFrameInAcycLimits = EC_FALSE;
                pSlvFrmDesc->poEcSlaveFrame->m_timeout.Stop();
                pSlvFrmDesc->pfnTimeStamp = EC_NULL;
                pSlvFrmDesc->wTimestampOffset = 0;
                pSlvFrmDesc->wTimestampSize = 0;
                pSlvFrmDesc->dwRepeatCnt = 0;
                EC_AL_88A4HDR_SET_E88A4FRAMELEN(pSlvFrmDesc->poEcSlaveFrame->m_p88A4Header, 0);
                OsDbgAssert(0 != EC_AL_88A4HDR_GET_E88A4HDRTYPE(pSlvFrmDesc->poEcSlaveFrame->m_p88A4Header));
                pSlvFrmDesc->poEcSlaveFrame->m_pLastEcCmdHeader = EC_NULL;
                pSlvFrmDesc->poEcSlaveFrame->m_nNumCmdsInFrame = 0;
            }
            return pSlvFrmDesc;
        }
    EC_T_VOID  FreeSlaveFrameDesc(PECAT_SLAVEFRAME_DESC pSlvFrmDesc)
        {
            /* remove frame from pending list see m_PendingSlaveFrameDescList */
            RemoveEntryList(&pSlvFrmDesc->ListEntry);
            /* return frame to free list */
            InsertTailList(&m_FreeSlaveFrameDescList, &pSlvFrmDesc->ListEntry);
        }

public:
    EC_T_DWORD                                  m_dwSupportedFeatures;  /* supported features (for license check)*/
    CEcBusTopology*                             m_pBusTopology;         /* instance of Bus Topology Storage */
#if (defined INCLUDE_DC_SUPPORT)
    CEcDistributedClocks*                       m_pDistributedClks;     /* instance of Distributed Clocks handling class */    
#endif
    CEcDevice*                                  m_poEcDevice;           /* interface to EtherCAT device */

protected:
    MBX_CALLBACK                                m_pfMbCallBack;         /* mailbox callback to application interface */
    NOTIFY_CALLBACK                             m_pfNotifyCallback;     /* notification callback to application interface */
  
    PEcMasterDesc                               m_pMasterDesc;
    EC_T_WORD                                   m_wRes;                 /* ETHERNET_ADDRESS is not aligned */
    EC_T_BYTE*                                  m_pLkMemory;
    EC_T_WORD                                   m_wRes2;                /* ETHERNET_ADDRESS is not aligned */

    EC_T_BOOL                                   m_bConfigLoaded;        /* Configuration successfully applied */
    EC_T_BOOL                                   m_bConfigured;          /* Configuration successfully applied */
    
    /* slave infos */
    EC_T_DWORD                                  m_nCfgSlaveCount;
    EC_T_DWORD                                  m_nEcMbSlave;
    EC_T_WORD                                   m_wMaxNumSlaves;
    CEcSlave**                                  m_ppEcSlave;
    CEcMbSlave**                                m_ppEcMbSlave;
    CFiFoListDyn<CEcSlave*>*                    m_pSlavesStateMachineFifo;

    /* master state */
    EC_T_DWORD                                  m_dwStateChangeDefaultTimeout;
    EC_T_BOOL                                   m_bMasterTimerEnabled;      /* EC_TRUE if master timer is enabled */
    EC_T_WORD                                   m_wCurMasterDeviceState;
    EC_T_WORD                                   m_wReqMasterDeviceState;
    EC_T_WORD                                   m_wCycFramesDeviceState;
    EC_MASTER_STATE                             m_eCurMasterLogicalState;    
    EC_T_BOOL                                   m_bAllSlavesMustReachState; /* all the slaves must reached the master state if EC_TRUE */
    EC_T_DWORD                                  m_dwMasterStateMachineResult;
    EC_T_WORD                                   m_bSlaveStateMachineError;
    EC_T_BOOL                                   m_bCancelStateMachine;
    EC_T_BOOL                                   m_bStateMachIsStable;

    /* master init commands */
    CEcTimer                                    m_oInitCmdsTimeout;
    CEcTimer                                    m_oInitCmdRetryTimeout;
    EC_T_WORD                                   m_nInitCmdsCurrent;
public:
    EC_T_WORD                                   m_nInitCmdsCount;
    PEcInitCmdDesc*                             m_ppInitCmds;
    EC_T_WORD                                   m_wMaxNumInitCmds;

    /* slave CoeInitCmds */
    EC_T_VOID*                                  m_pvSlaveCoeInitCmdCallbackParm;
    EC_PF_COE_INITCMD_CALLBACK                  m_pfnSlaveCoeInitCmdCallback;
protected:
    EC_T_DWORD                                  m_maxAsyncFrameSize;

    EC_T_DWORD                                  m_dwInitCmdRetryTimeout;    /* Timeout to retry InitCmds */
    EC_T_DWORD                                  m_dwMbxCmdTimeout;
    EC_T_DWORD                                  m_dwDefaultMbxPolling;

    EC_T_DWORD                                  m_dwEcatCmdTimeout;         /*< timeout to send pending ethercat command frames */

    /* mailbox state evaluation */
    CEcMbSlave**                                m_ppEcPortMBoxState;

    EC_T_LISTENTRY                              m_FreeSlaveFrameDescList;       /* list containing unused ethercat frame descriptors */
    EC_T_LISTENTRY                              m_PendingSlaveFrameDescList;    /* list containing currently pending ethercat frame descriptors */
    PECAT_SLAVEFRAME_DESC                       m_aSlaveFrameDesc;              /* array with slave frame descriptors */
    CEcSlaveFrame*                              m_aoEcSlaveFrame;               /* array with ecat frame objects */
    PECAT_SLAVEFRAME_DESC                       m_pCurrPendingSlaveFrameDesc;   /* descriptor for current pending ethercat frame, 
                                                                                 * new ecat commands will be stored here using the QueueEcatCmdReq()
                                                                                 * method of the CEcMaster class.
                                                                                 */
    /* management of cyclic commands */
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    EC_T_DWORD                                  m_dwNumCycConfigEntries;        /* number of cyclic entries (configured within the XML configuration file) */
#endif
    EC_T_BYTE                                   m_byMaxCycleIdx;                /* maximum cycle index */
    EC_T_BYTE                                   m_byCurCycleIdx;                /* current cycle index */
    EC_T_WORD                                   m_wAllCycEntriesNumFrames;      /* total number of cyclic commands in all frames and all cyclic entries */
EC_INSTRUMENT_MOCKABLE_VAR
    CYCLIC_ENTRY_MGMT_DESC*                     m_aCyclicEntryMgmtDesc;         /* cyclic entry management */
    EC_T_BOOL                                   m_bCopyInfoInSendCycFrames;     /* if EC_TRUE, copy info processed in SendCycFrames in ProcessAllRxFrames otherwise */
    EC_T_BOOL                                   m_bIgnoreInputsOnWkcError;      /* if EC_TRUE, input process data are not copied on WKC error */
    EC_T_BOOL                                   m_bZeroInputsOnWkcZero;         /* if EC_TRUE, input process data are set to zero on WKC is zero */
    EC_T_BOOL                                   m_bZeroInputsOnWkcError;        /* if EC_TRUE, input process data are set to zero on WKC error */

    EC_T_BOOL                                   m_bHavePdWriteAccess;           /* EC_TRUE if the master has write access to the process data */
    EC_T_PBYTE                                  m_pbyPDInBasePtr;               /* current pointer to input process data */
    EC_T_BOOL                                   m_bFoundAlStatusRead;           /* flag if AlStatusRead was found, just to assure the XML configuration is valid! */

    /* cyclic error notification mask */
    EC_T_DWORD                                  m_dwCycErrorNotifyMask;         /* bit mask for generating cyclic error notifications */
    EC_T_DWORD                                  m_dwCycErrorNotifyMaskedCnt;    /* mask counter to masked notification */

    EC_T_DWORD                                  m_dwFrameResponseNotifyMaskedCnt;/* mask counter to masked notification */

#if (defined INCLUDE_RED_DEVICE)
    /* Redundancy */
    EC_T_BOOL                                   m_bRedFrameSplitEnabled;        /* enable frame splitting mechanism */
    EC_T_BOOL                                   m_bRedNotify;                   /* notify line status */
    EC_T_BOOL                                   m_bRedLineChanging;             /* line is status is changing */
public:
    EC_T_BOOL                                   m_bRedLineBreak;                /* line is broken */
    EC_T_BOOL                                   m_bRedLineCrossed;              /* line crossed detected */
    EC_T_BOOL                                   m_bRedNotifyActive;             /* notification active */
    EC_T_WORD                                   m_wRedNumSlavesOnMain;          /* number of slaves on main line, if line is o.k. then equal to total number of slaves */
    EC_T_WORD                                   m_wRedNumSlavesOnRed;           /* number of slaves on red line */

    EC_T_PBYTE                                  m_pbyRedFrame;
#endif /* INCLUDE_RED_DEVICE */

    EC_T_BOOL                                   m_bNotAllDevOpNotified;             /* trigger for all devices are operational notification */
    EC_T_BOOL                                   m_bNotAllDevOpNotificationEnabled;  /* check and notify if AL status or WKC does not match */

EC_INSTRUMENT_MOCKABLE_VAR
    EC_T_VOID*                                  m_poFreeSlaveFrameDescLock;
    EC_T_VOID*                                  m_poEcTransferRawCmdLock;
    
    PEcMailboxCmd                               m_pEcMbxCoeEmergencyCmd;

    PEcMailboxCmd                               m_pEcMbxSoeEmergencyCmd;
    PEcMailboxCmd                               m_pEcMbxSoeNotificationCmd;

    EC_T_MASTER_CONFIG_EX                       m_MasterConfigEx;               /* extended static master configuration */

    EC_T_DWORD                                  m_dwNotificationMask;
    
    EC_T_DWORD                                  m_dwNotifySBMismatchMaskedCnt;
    EC_T_DWORD                                  m_dwNotifySBStatusMaskedCnt;
    EC_T_DWORD                                  m_dwNotifyTopoChangeDoneMaskedCnt;

    EC_T_SLAVES_STATECHANGED_NTFY_DESC          m_SlavesStateChangedNotifyDesc;     /* slaves finished successfully state transition descriptor */
    EC_T_ERROR_NOTIFICATION_DESC                m_SlavesUnexpectedStateNotifyDesc;  /* slaves in unexpected state descriptor */
    EC_T_SLAVES_PRESENCE_NTFY_DESC              m_SlavesPresenceNotifyDesc;         /* slaves (dis-)appeared */
    EC_T_ERROR_NOTIFICATION_DESC                m_SlavesErrorNotifyDesc;            /* slaves error status */

#if (defined INCLUDE_DC_SUPPORT)
    EC_T_BOOL                                   m_bDCSupportEnabled;            /* DC Feature Support Enabled */
    EC_T_BOOL                                   m_bAddBrdSyncWindowMonitoring;  /* Add BRD to 0x92C if not in cyclic frame detected */
    EC_T_DWORD                                  m_dwDcmInSyncTimeout;
    CEcTimer                                    m_oDcmInSyncTimeout;
#endif /* INCLUDE_DC_SUPPORT */

    EC_T_DWORD                                  m_dwHCDetectionTimeout;         /* HotConnect Group Detection Timeout */

    EC_T_DWORD                                  m_dwSBTimeout;                  /* ScanBus Timeout */
    EC_T_DWORD                                  m_dwScanBusResult;              /* Last ScanBus result */
    
    EC_T_MEMPROV_DESC                           m_oMemProvider;                 /* Memory provider */

#if (defined INCLUDE_WKCSTATE)
    EC_T_INT                                    m_nDiagnosisImageSize;          /* sizeof the diagnosis image */
    EC_T_BYTE*                                  m_pbyDiagnosisImage;            /* diagnosis image returned by ecatGetDiagnosisImagePtr() */
#endif /* INCLUDE_WKCSTATE */

    EC_T_NOTIFICATION_INTDESC*                  m_pNotificationStore;           /* Notification buffer */    
    EC_T_BOOL                                   m_bSlaveErrorDetected;          /* Slave Error Detected */

    EC_T_DWORD                                  m_dwAppVersion;                 /* Application asked version */

#ifdef DEBUG
    EC_T_DWORD                                  m_dwPrintNotOpTime;
    EC_T_DWORD                                  m_dwPrintErrStateTime;
#endif
    EC_T_BOOL                                   m_bCyclicTimeoutEnabled;

#if (defined INCLUDE_MASTER_OBD)
    EC_T_DWORD*                                 m_pdwLostFrameCrt;
    EC_T_WORD                                   m_wStateChngReq;
#endif

#if (defined INCLUDE_SLAVE_STATISTICS)
    EC_T_DWORD                                  m_bSlaveStatisticsRequested;
    CEcTimer                                    m_oSlaveStatisticsTimer;
    EC_T_DWORD                                  m_dwSlaveStatisticsPeriod;
    EC_T_DWORD                                  m_dwSlaveStatisticsSlaveIdxCur; /* slicing control */
#endif

    EC_T_DWORD                                  m_bRawTferPending;  /* EC_TRUE if a raw transfer is pending */
    EC_T_DWORD                                  m_dwRawTferStatus;  /* raw transfer status */
    EC_T_WORD                                   m_wRawTferDataLen;  /* data length of a raw transfer (valid only for read requests) */

    EC_T_BYTE*                                  m_pbyRawTferData;   /* raw transfer result data buffer */

    EC_T_BOOL                                   m_bLatchReceiveTimesRequest;

    EC_T_WORD                                   m_wAlStatusReg;
    EC_T_WORD                                   m_wALStatusWkc;     /* current Working Counter value / Slave amount, returned by BRD AL Status read */
    EC_T_WORD                                   m_wALStatusExpectedWkc;/* last expected AL Status WKC */

    /* EEPRom Access */
    CEcEEPRom*                                  m_pEEEPRom;

public:
#if (defined VLAN_FRAME_SUPPORT)
    /* VLAN support */
    EC_T_BOOL                                   m_bVLANEnabled;     /* VLAN Tags are inserted / frames filtered if EC_TRUE */
    EC_T_WORD                                   m_wVLANId;          /* 12Bit VLAN Id */
    EC_T_BYTE                                   m_byVLANPrio;       /* 3Bit VLAN Prio */
#endif
    CEcHotConnect                               m_oHotConnect;      /* Hot Connect Instance */

private:
    /* Process Data offset calc */
    EC_T_BOOL                                   m_bPdOffsetCompMode;            /* Offset are in compatibility mode (before V2.1) */
    EC_T_DWORD                                  m_dwMinCycOutOffset;
    EC_T_DWORD                                  m_dwMinRecv;
    EC_T_DWORD                                  m_dwMinCycInOffset;
    EC_T_DWORD                                  m_dwMinSend;

    EC_T_DWORD                                  m_dwCopyOffset;                 /* default mode: copy offset for process data (in bytes) */
    EC_T_BOOL                                   m_bOnlyProcDataInImage;         /* copy only proc. data into input image */


    /* duplicate datagram in frame check */
    EC_T_DWORD                                  m_dwDuplicateCheckCount;        /* number of addresses in list */
    EC_T_DWORD                                  m_adwDuplicateCheckAddress[128];    /* addresses to check */
    EC_T_BYTE                                   m_adwDuplicateCheckCmd[128];        /* commands to check */

    /* callback function to inform application that all cyclic frame(s) are received */
    EC_PF_CYCFRAME_RECV                         m_pvCycFrameRxCallBack;     
    EC_T_VOID*                                  m_pvCycFrameRxCallBackParam;
    EC_T_UINT64                                 m_qwLk;


    /* bus load measurement */
    EC_T_DWORD                                  m_dwBytesPerSecondCnt;          /* bytes/per second */
    EC_T_DWORD                                  m_dwBytesPerSecondAct;          /* TX bytes/second actual value */
    EC_T_DWORD                                  m_dwBytesPerSecondMin;          /* TX bytes/second min. value   */
    EC_T_DWORD                                  m_dwBytesPerSecondMax;          /* TX bytes/second max. value   */

    EC_T_DWORD                                  m_dwBytesPerCycleCnt;           /* bytes/per cycle */
    EC_T_DWORD                                  m_dwBytesPerCycleAct;           /* TX bytes/cycle actual value */
    EC_T_DWORD                                  m_dwBytesPerCycleMin;           /* TX bytes/cycle min. value   */
    EC_T_DWORD                                  m_dwBytesPerCycleMax;           /* TX bytes/cycle max. value   */

    EC_T_BYTE                                   m_byMaxCycCmdIdx;               /* Max cyclic command index value */
    EC_T_BYTE                                   m_byMinAcycCmdIdx;              /* Min acyclic command index value */
    EC_T_BOOL                                   m_bDLStatusIRQMasked;           /* DL StatusIRQ masked */
    EC_T_BOOL                                   m_bDLStatusIRQ;                 /* DL StatusIRQ detected, cleared on read out */
	EC_T_BOOL                                   m_bALStatusIRQ;                 /* AL StatusIRQ detected, cleared on read out */

public:

#if (defined INCLUDE_VARREAD)
    EC_T_DWORD                                  m_dwProcVarInfoNumEntriesInp;   /* Number of INPUT process variable information entries stored in m_pProcVarInfoEntriesInp*/
    EC_T_DWORD                                  m_dwProcVarInfoNumEntriesOutp;  /* Number of OUTPUT process variable information entries stored in m_pProcVarInfoEntriesOutp*/
    
    EC_T_PD_VAR_INFO_INTERN**                   m_apProcVarInfoEntriesInp;       /* INPUT Process variable information entries */
    EC_T_PD_VAR_INFO_INTERN**                   m_apProcVarInfoEntriesOutp;      /* OUTPUT Process variable information entries */
    EC_T_DWORD                                  m_dwMaxProcVarInfoNumEntriesInp;
    EC_T_DWORD                                  m_dwMaxProcVarInfoNumEntriesOutp;

    EC_T_BOOL                                   m_bAddVarsForSpecificDataTypes; /* Create and add process image variables for specific complex known data types found in ENI */
#endif

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
    PTS_CONTROL_DESC                            m_oPtsControlDesc;              /* Structure to control the PTS */      
#endif

#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_AOE_NETID                              m_oNetId;                       /* Master AoE NetID */
#endif

#if (defined INCLUDE_FRAME_LOGGING)
    /* frame logging */
    EC_T_PFLOGFRAME_CB                          m_pvLogFrameCallBack;
    EC_T_VOID*                                  m_pvLogFrameCallBackContext;
#endif

    /* master settings (properties) */
    EC_T_DWORD                                  m_dwMasterPropNumEntries;                       /* number of list entries */
    EC_T_DWORD                                  m_dwMasterPropArraySize;                /* size of the m_aMasterPropEntries[] array */
    EC_T_MASTER_PROP_DESC*                      m_aMasterPropEntries;                           /* list entries */

    EC_T_CYCFRAME_LAYOUT                        m_eCycFrameLayout;

#if (defined INCLUDE_FORCE_PROCESSDATA)
    EC_T_WORD                                   m_wForceProcessDataMaxEntries;
    EC_T_WORD                                   m_wForceProcessDataEntries;  
    EC_T_FORCE_RELEASE_PROCESS_DATA_INFO*       m_pForceProcessDataList;
#endif

#if (defined INCLUDE_TRACE_DATA)
    EC_T_DWORD                                  m_dwTraceDataOffset;    /* in bytes */
    EC_T_WORD                                   m_wTraceDataSize;       /* in bytes */
#endif /* INCLUDE_TRACE_DATA */

    EC_T_BOOL                                   m_bGenEniAssignEepromBackToEcat;
    EC_T_BOOL                                   m_bAutoAckAlStatusError;
    EC_T_BOOL                                   m_bAutoAdjustCycCmdWkc;
    EC_T_BOOL                                   m_bAdjustCycFramesAfterslavesStateChange;

    CEcTimer                                    m_MasterInfoTimer;
#if (defined INCLUDE_MAILBOX_STATISTICS)
    EC_T_STATISTIC_TRANSFER_DUPLEX              m_MailboxStatistics[EC_MAILBOX_STATISTICS_CNT]; /* provided to Interface */
    EC_T_STATISTIC_TRANSFER_DUPLEX_CUR          m_MailboxStatisticsCur[EC_MAILBOX_STATISTICS_CNT]; /* cyclically latched and reset, not accessed by Interface */
#endif /* INCLUDE_MAILBOX_STATISTICS */
};
#endif /* ECMASTER_INC */
/*-END OF SOURCE FILE--------------------------------------------------------*/
