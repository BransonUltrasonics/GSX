/*-----------------------------------------------------------------------------
 * AtEthercatPrivate.h      header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              private header for AT-EM interface layer
 *---------------------------------------------------------------------------*/

#ifndef INC_ATETHERCATPRIVATE
#define INC_ATETHERCATPRIVATE

/*-INCLUDE-------------------------------------------------------------------*/

#ifndef INC_ECCONFIG
#include <EcConfig.h>
#endif

/*-DEFINES/MACROS------------------------------------------------------------*/
#define EC_NUMOF_MCSM_ORDERS_API            20     /*< maximum number of elements in API McSM order queue */
#define EC_NUMOF_MCSM_ORDERS_INTERNAL        8     /*< maximum number of elements in internal McSM order queue */
#define EC_NUMOF_MCSM_ORDERS_AUTO_RELEASE    4     /*< maximum number of elements in auto release McSM order queue */
#define EC_NUMOF_MCSM_ORDERS                EC_NUMOF_MCSM_ORDERS_API + EC_NUMOF_MCSM_ORDERS_INTERNAL + EC_NUMOF_MCSM_ORDERS_AUTO_RELEASE
#define EC_MAX_CLIENTS                      0xFE   /* maximum number of clients (must not exceed 0xFE, see INVOKE_ID_CLNT_TFER_Q_RAW_CMD and INVOKE_ID_TFER_Q_RAW_CMD) */

/*-FORWARD DECLARATIONS------------------------------------------------------*/
struct _EC_T_CLNTDESC;
class CEcDeviceFactory;
class CEcConfigData;
class CEcSlave;
class CEcMbSlave;

/*-TYPEDEFS/ENUMS------------------------------------------------------------*/
typedef struct _CYCLIC_ENTRY_MGMT_DESC CYCLIC_ENTRY_MGMT_DESC;

/* events for master control state machine */
typedef enum _EC_T_MCSM_ORDER_TYPE
{
    eMcSmOrderType_UNKNOWN                          ,   /*< unknown */
    eMcSmOrderType_REQ_MASTER_STATE                 ,   /*< request for a new master state */
    eMcSmOrderType_REQ_SB                           ,   /*< request new bus scan */
    eMcSmOrderType_REQ_SB_FOR_GENERATE_ENI          ,   /*< request new bus scan */
    eMcSmOrderType_TOPOLOGY_CHANGE                  ,   /*< process detected topology change */
    eMcSmOrderType_HC_ACCEPT_TOPOLOGY_CHANGE        ,   /*< request HC Accept Topology Change */
    eMcSmOrderType_PORTOPERATION                    ,   /*< request HC Port Operation */
    eMcSmOrderType_EEP_OPERATION                    ,   /*< request EEPROM Operation */
    eMcSmOrderType_REQ_DLSTATUS_IST                 ,   /*< DL Status IST */
    eMcSmOrderType_REQ_ALSTATUS_REFRESH             ,   /*< AL Status refresh */
    eMcSmOrderType_SB_ACCEPT_TOPOLOGY_CHANGE        ,   /*< accept topology change (manual mode) */
    eMcSmOrderType_REQ_EXTENDED_CONFIG              ,   /*< request extend configuration */

    /* Borland C++ datatype alignment correction */
    eMcSmOrderType_BCppDummy   = 0xFFFFFFFF
} EC_T_MCSM_ORDER_TYPE;                                       

/* states of the master control state machine */
/* sequence of enum MUST NOT be changed ... McSm counts on values ! */
typedef enum _EC_T_MCSM_STATE
{
    eEcatMcSmState_UNKNOWN                          ,   /*< unknown */
    eEcatMcSmState_START                            ,   /*< start/idle state */

    eEcatMcSmState_STOP_REQ                         ,   /*< request stop state */
    eEcatMcSmState_WAIT_STOP                        ,   /*< wait for stop state */
    eEcatMcSmState_STOP_DONE                        ,   /*< stop state reached */

    eEcatMcSmState_START_SB                         ,   /*< start ScanBus */        
    eEcatMcSmState_WAIT_SB                          ,   /*< wait for ScanBus to finish */
    eEcatMcSmState_SB_DONE                          ,   /*< ScanBus done */

    eEcatMcSmState_TRANSITION                       ,   /*< determine next state transition */
    
    eEcatMcSmState_REQ_MASTER_INIT                  ,   /*< request master init state */
    eEcatMcSmState_WAIT_MASTER_INIT                 ,   /*< wait for master init state */
    eEcatMcSmState_MASTER_INIT_DONE                 ,   /*< master init state entered */
    
    eEcatMcSmState_REQ_MASTER_PREOP                 ,   /*< request master preop state */
    eEcatMcSmState_WAIT_MASTER_PREOP                ,   /*< wait for master preop state */
    eEcatMcSmState_MASTER_PREOP_DONE                ,   /*< master preop state entered */

    eEcatMcSmState_START_DC_INIT                    ,   /*< start DC init */        
    eEcatMcSmState_WAIT_DC_INIT                     ,   /*< wait for DC init */
    eEcatMcSmState_DC_INIT_DONE                     ,   /*< DC init done */

    eEcatMcSmState_REQ_MASTER_SAFEOP                ,   /*< request master safeop state */
    eEcatMcSmState_WAIT_MASTER_SAFEOP               ,   /*< wait for master safeop state */
    eEcatMcSmState_MASTER_SAFEOP_DONE               ,   /*< master safeop state entered */
    
    eEcatMcSmState_REQ_MASTER_OP                    ,   /*< request master op state */
    eEcatMcSmState_WAIT_MASTER_OP                   ,   /*< wait for master op state */
    eEcatMcSmState_MASTER_OP_DONE                   ,   /*< master op state entered */

    eEcatMcSmState_START_EEPROMOP                   ,   /*< start for EEPROM Operation */
    eEcatMcSmState_WAIT_EEPROMOP                    ,   /*< wait for EEPROM Operation */
    eEcatMcSmState_EEPROMOP_DONE                    ,   /*< EEPROM Operation done */

    eEcatMcSmState_START_TOPOCHANGE                 ,   /*< start topology change detection */
    eEcatMcSmState_WAIT_TOPOCHANGE                  ,   /*< wait for topology change detection */
    eEcatMcSmState_TOPOCHANGE_DONE                  ,   /*< topology change detection finished */

    eEcatMcSmState_START_ACCCEPTTOPOCHANGE          ,   /*< continue topology change detection */
    eEcatMcSmState_WAIT_ACCEPTTOPOCHANGE            ,   /*< wait for topology change detection (cntd) */
    eEcatMcSmState_ACCEPTTOPOCHANGE_DONE            ,   /*< topology change detection finished (cntd) */

    eEcatMcSmState_START_PORTOPERATION              ,   /*< start HC Port Operation */
    eEcatMcSmState_WAIT_PORTOPERATION               ,   /*< wait HC Port Operation */
    eEcatMcSmState_PORTOPERATION_DONE               ,   /*< Port Operation finished */

    eEcatMcSmState_START_DLSTATUS_IST               ,   /*< start DL Status Int Operation */
    eEcatMcSmState_WAIT_DLSTATUS_IST                ,   /*< wait DL Status Int Operation */
    eEcatMcSmState_DLSTATUS_IST_DONE                ,   /*< DL Status Int Operation finished */

    eEcatMcSmState_START_ALSTATUS_REFRESH           ,   /*< start AL status refresh operation */
    eEcatMcSmState_WAIT_ALSTATUS_REFRESH            ,   /*< wait AL status refresh operation */
    eEcatMcSmState_ALSTATUS_REFRESH_DONE            ,   /*< AL status refresh operation finished */

    eEcatMcSmState_START_EXTENDED_CONFIG            ,   /*< start extend configuration */
    eEcatMcSmState_WAIT_EXTENDED_CONFIG             ,   /*< wait extend configuration */
    eEcatMcSmState_EXTENDED_CONFIG_DONE             ,   /*< extended configuration finished */

    /* Borland C++ datatype alignment correction */
    eEcatMcSmState_BCppDummy                        = 0xFFFFFFFF
} EC_T_MCSM_STATE;                                       


typedef enum
{
    eEcatOrderState_Idle=0,                 /*< free: can be used by app. (head) */
    eEcatOrderState_Pending,                /*< pending: new/valid order         */
    eEcatOrderState_Running,                /*< in process by McSM               */
    eEcatOrderState_Interrupted,            /*< order is interrupted             */
    eEcatOrderState_Canceling,              /*< order is canceling               */
    eEcatOrderState_Finished,               /*< order is finished                */
    /* Borland C++ datatype alignment correction */
    eEcatOrderState_BCppDummy   = 0xFFFFFFFF
} EC_T_MCSM_ORDER_STATE;

typedef union
{
    struct
    {
        /* API -> MCSM request */
        EC_T_BOOL       bMcSmRequestBusScan;
        EC_T_BOOL       bMcSmRequestApplyConfig;
        EC_T_BOOL       bMcSmRequestResetConfig;

        /* MCSM -> API request */
        EC_T_BOOL       bApiRequestResetConfig;
        EC_T_BOOL       bApiRequestRecreateCfgTopology;

        CEcBusSlave*    poBusSlavePool;
        EC_T_BYTE*      pbyBusSlavesMem;
        EC_T_DWORD      dwBusSlavesMemSize;

        CEcSlave**      apoSlaves;
        EC_T_WORD       wSlaveCnt;
        CEcMbSlave**    apoMbSlaves;
        EC_T_WORD       wMbSlaveCnt;
        EC_T_WORD       wMaxNumSlaves;
    } ConfigExtend;
} EC_T_MCSM_ORDER_CONTEXT;

typedef struct _EC_T_MCSM_ORDER
{
    EC_T_LISTENTRY                      ListEntry;
    CFiFoListDyn<_EC_T_MCSM_ORDER*>*    pFreeMcSmOrderFifo;

    EC_T_MCSM_ORDER_STATE    eOrderState;
    EC_T_MCSM_ORDER_TYPE     eMcSmOrderType;
    EC_T_STATE               eReqState;
    EC_T_MCSM_STATE          eMcSmReqState;
    EC_T_DWORD               dwOrderId;
    EC_T_DWORD               dwRetryCnt;
    EC_T_BOOL                bAutoRelease;
    EC_T_BOOL                bCancel;

    EC_T_MCSM_STATE          EMcSmErrorState;        /**< state when the error was detected */
    EC_T_MCSM_STATE          EMcSmReqState;          /**< requested final state */
    EC_T_DWORD               dwMcSmLastErrorStatus;  /**< status value of last error */
    
    EC_T_MCSM_ORDER_CONTEXT* pContext;

    CEcTimer                 oTimeout;
} EC_T_MCSM_ORDER;

#define EcStateToEcMcSmEvent(EcatState)                                 \
    ((EcatState)==eEcatState_INIT?eMcSmOrderType_MASTER_INIT:          \
     ((EcatState)==eEcatState_PREOP?eMcSmOrderType_MASTER_PREOP:       \
      ((EcatState)==eEcatState_SAFEOP?eMcSmOrderType_MASTER_SAFEOP:    \
       ((EcatState)==eEcatState_OP?eMcSmOrderType_MASTER_OP:           \
        eMcSmOrderType_UNKNOWN                                         \
    ))))
#define EcStateToEcMcSmState(EcatState)                                     \
    ((EcatState)==eEcatState_INIT?eEcatMcSmState_MASTER_INIT_DONE:         \
     ((EcatState)==eEcatState_PREOP?eEcatMcSmState_MASTER_PREOP_DONE:      \
      ((EcatState)==eEcatState_SAFEOP?eEcatMcSmState_MASTER_SAFEOP_DONE:   \
       ((EcatState)==eEcatState_OP?eEcatMcSmState_MASTER_OP_DONE:          \
        eEcatMcSmState_UNKNOWN                                             \
    ))))

#ifdef INCLUDE_LOG_MESSAGES
#define GetEcMcSmStateString(McSmState) \
    ((McSmState)==eEcatMcSmState_UNKNOWN?"UNKNOWN": \
    ((McSmState)==eEcatMcSmState_START?"START": \
    ((McSmState)==eEcatMcSmState_STOP_REQ?"STOP_REQ": \
    ((McSmState)==eEcatMcSmState_WAIT_STOP?"WAIT_STOP": \
    ((McSmState)==eEcatMcSmState_STOP_DONE?"STOP_DONE": \
    ((McSmState)==eEcatMcSmState_START_SB?"START_SB": \
    ((McSmState)==eEcatMcSmState_WAIT_SB?"WAIT_SB": \
    ((McSmState)==eEcatMcSmState_SB_DONE?"SB_DONE": \
    ((McSmState)==eEcatMcSmState_TRANSITION?"TRANSITION": \
    ((McSmState)==eEcatMcSmState_REQ_MASTER_INIT?"REQ_MASTER_INIT": \
    ((McSmState)==eEcatMcSmState_WAIT_MASTER_INIT?"WAIT_MASTER_INIT": \
    ((McSmState)==eEcatMcSmState_MASTER_INIT_DONE?"MASTER_INIT_DONE": \
    ((McSmState)==eEcatMcSmState_REQ_MASTER_PREOP?"REQ_MASTER_PREOP": \
    ((McSmState)==eEcatMcSmState_WAIT_MASTER_PREOP?"WAIT_MASTER_PREOP": \
    ((McSmState)==eEcatMcSmState_MASTER_PREOP_DONE?"MASTER_PREOP_DONE": \
    ((McSmState)==eEcatMcSmState_START_DC_INIT?"START_DC_INIT": \
    ((McSmState)==eEcatMcSmState_WAIT_DC_INIT?"WAIT_DC_INIT": \
    ((McSmState)==eEcatMcSmState_DC_INIT_DONE?"DC_INIT_DONE": \
    ((McSmState)==eEcatMcSmState_REQ_MASTER_SAFEOP?"REQ_MASTER_SAFEOP": \
    ((McSmState)==eEcatMcSmState_WAIT_MASTER_SAFEOP?"WAIT_MASTER_SAFEOP": \
    ((McSmState)==eEcatMcSmState_MASTER_SAFEOP_DONE?"MASTER_SAFEOP_DONE": \
    ((McSmState)==eEcatMcSmState_REQ_MASTER_OP?"REQ_MASTER_OP": \
    ((McSmState)==eEcatMcSmState_WAIT_MASTER_OP?"WAIT_MASTER_OP": \
    ((McSmState)==eEcatMcSmState_MASTER_OP_DONE?"MASTER_OP_DONE": \
    ((McSmState)==eEcatMcSmState_START_EEPROMOP?"START_EEPROMOP": \
    ((McSmState)==eEcatMcSmState_WAIT_EEPROMOP?"WAIT_EEPROMOP": \
    ((McSmState)==eEcatMcSmState_EEPROMOP_DONE?"EEPROMOP_DONE": \
    ((McSmState)==eEcatMcSmState_START_TOPOCHANGE?"START_TOPOCHANGE": \
    ((McSmState)==eEcatMcSmState_WAIT_TOPOCHANGE?"WAIT_TOPOCHANGE": \
    ((McSmState)==eEcatMcSmState_TOPOCHANGE_DONE?"TOPOCHANGE_DONE": \
    ((McSmState)==eEcatMcSmState_START_ACCCEPTTOPOCHANGE?"START_ACCCEPTTOPOCHANGE": \
    ((McSmState)==eEcatMcSmState_WAIT_ACCEPTTOPOCHANGE?"WAIT_ACCEPTTOPOCHANGE": \
    ((McSmState)==eEcatMcSmState_ACCEPTTOPOCHANGE_DONE?"ACCEPTTOPOCHANGE_DONE": \
    ((McSmState)==eEcatMcSmState_START_PORTOPERATION?"START_PORTOPERATION": \
    ((McSmState)==eEcatMcSmState_WAIT_PORTOPERATION?"WAIT_PORTOPERATION": \
    ((McSmState)==eEcatMcSmState_PORTOPERATION_DONE?"PORTOPERATION_DONE": \
    ((McSmState)==eEcatMcSmState_START_DLSTATUS_IST?"START_DLSTATUS_IST": \
    ((McSmState)==eEcatMcSmState_WAIT_DLSTATUS_IST?"WAIT_DLSTATUS_IST": \
    ((McSmState)==eEcatMcSmState_DLSTATUS_IST_DONE?"DLSTATUS_IST_DONE": \
    ((McSmState)==eEcatMcSmState_START_ALSTATUS_REFRESH?"REQ_ALSTATUS_REFRESH": \
    ((McSmState)==eEcatMcSmState_WAIT_ALSTATUS_REFRESH?"WAIT_ALSTATUS_REFRESH": \
    ((McSmState)==eEcatMcSmState_ALSTATUS_REFRESH_DONE?"ALSTATUS_REFRESH_DONE": \
    ((McSmState)==eEcatMcSmState_START_EXTENDED_CONFIG?"REQ_EXTENDED_CONFIG": \
    ((McSmState)==eEcatMcSmState_WAIT_EXTENDED_CONFIG?"WAIT_EXTENDED_CONFIG": \
    ((McSmState)==eEcatMcSmState_EXTENDED_CONFIG_DONE?"EXTENDED_CONFIG_DONE": \
    "XXX - MCSM STATE INVALID" \
)))))))))))))))))))))))))))))))))))))))))))))


#define GetEcMcSmOrderString(McSmEvent) \
    ((McSmEvent)==eMcSmOrderType_UNKNOWN?"UNKNOWN": \
    ((McSmEvent)==eMcSmOrderType_REQ_MASTER_STATE?"REQ_MASTER_STATE": \
    ((McSmEvent)==eMcSmOrderType_REQ_SB?"REQ_SB": \
    ((McSmEvent)==eMcSmOrderType_REQ_SB_FOR_GENERATE_ENI?"REQ_SB_FOR_GENERATE_ENI": \
    ((McSmEvent)==eMcSmOrderType_TOPOLOGY_CHANGE?"TOPOLOGY_CHANGE": \
    ((McSmEvent)==eMcSmOrderType_HC_ACCEPT_TOPOLOGY_CHANGE?"HC_ACCEPT_TOPOLOGY_CHANGE": \
    ((McSmEvent)==eMcSmOrderType_PORTOPERATION?"PORTOPERATION": \
    ((McSmEvent)==eMcSmOrderType_EEP_OPERATION?"EEP_OPERATION": \
    ((McSmEvent)==eMcSmOrderType_REQ_DLSTATUS_IST?"REQ_DLSTATUS_IST": \
    ((McSmEvent)==eMcSmOrderType_REQ_ALSTATUS_REFRESH?"REQ_ALSTATUS_REFRESH": \
    ((McSmEvent)==eMcSmOrderType_SB_ACCEPT_TOPOLOGY_CHANGE?"SB_ACCEPT_TOPOLOGY_CHANGE": \
    ((McSmEvent)==eMcSmOrderType_REQ_EXTENDED_CONFIG?"REQ_EXTENDED_CONFIG": \
    "XXX - MCSM ORDER TYPE INVALID" \
))))))))))))
#endif

/* link debug Ethernet type values */
#define ETHTYP0_GENERIC     0xFF    /* generic */

#if (defined INCLUDE_COE_PDO_SUPPORT)
#define ETHTYP1_RXPDO       0x50
#endif
#define ETHTYP1_USRCTL      0xE0
#define ETHTYP0_USRCTL      0xFF    /* generic */

#define ETHTYP1_MCSM        0xFE
#define ETHTYP0_MCSM        0xFF    /* generic */
#define ETHTYP0_MCSM_ORADD  0xF0    /* order queue add marker */
#define ETHTYP0_MCSM_ORNEW  0xF1    /* new order marker */
#define ETHTYP0_MCSM_ORSTRT 0xF2    /* start order marker */
#define ETHTYP0_MCSM_ORINT  0xF3    /* interrupt order marker */
#define ETHTYP0_MCSM_ORREST 0xF4    /* restart order marker */
#define ETHTYP0_MCSM_ORFIN  0xF5    /* finish order marker */
#define ETHTYP0_MCSM_EEPOP  0xF6    /* EEPROM operation marker */
#define ETHTYP0_MCSM_STCNG  0xF8    /* state change marker */
#define ETHTYP0_MCSM_TOPCH  0xFC    /* topology change marker */
#define ETHTYP0_MCSM_SLERR  0xFD    /* slave error marker */
#define ETHTYP0_MCSM_PORTOP 0xFE    /* port operation marker */

#define ETHTYP1_HCSM_HL     0xFC    /* high level hot connect statemachine */
#define ETHTYP1_HCSM_LL     0xFB    /* low level hot connect statemachine */
#define ETHTYP1_HCSM_GO     0xFA    /* hot connect Group operations */
#define ETHTYP1_HCSM_SC     0xF9    /* hot connect Group state machine state change */
#define ETHTYP0_HCSM        0xFF    /* generic hot connect statemachine */

#define ETHTYP1_EEPSM_SC    0xF8    /* EEPROM Operation state machine state change */
#define ETHTYP0_EEPSM       0xFD    /* EEPROM Operation statemachine */

#define ETHTYP1_ECATAPI     0xF7    /* Tracing EC-Master API calls */
#define ETHTYP0_ECATAPI_SET_MASTER_STATE    0x01    /* ecatSetMasterState */
#define ETHTYP0_ECATAPI_SET_SLAVE_STATE     0x02    /* ecatSetSlaveState */
#define ETHTYP0_ECATAPI_GET_SLAVE_INFO      0x03    /* ecatGetSlaveInfo */

/* events for master control state machine */
typedef enum _EC_T_ENABLE_STATUS
{
    eEnableStatus_UNKNOWN = 0,
    eEnableStatus_DISABLED,
    eEnableStatus_ENABLED,

    /* Borland C++ datatype alignment correction */
    eEnableStatus_BCppDummy   = 0xFFFFFFFF
} EC_T_ENABLE_STATUS;

/* private EtherCAT mailbox transfer object */
typedef struct _EC_T_MBXTFERP
{
    EC_T_MBXTFER    MbxTfer;    /*< the global mailbox transfer object (accessible by the user) */
    EC_T_DWORD      dwSlaveId;  /*< slave id of the current pending transfer */
    EC_T_BOOL       bAbort;     /*< pending transfer abort request */

    EcMailboxCmd    oEcMbxCmd;  /*< the mailbox command descriptor */
} EC_T_MBXTFERP;

/* EtherCAT client descriptor */                    
typedef struct _EC_T_CLNTDESC                       
{                                                   
    struct _EC_T_CLNTDESC* pNext;                   /*< out pointer to next client */
    EC_T_DWORD      dwId;                           /*< out client ID, used by the application */
    EC_T_BYTE       byUniqueClntId;                 /*  internally used unique ID, e.g. for usage as part of an invoke ID */
    EC_T_DWORD      dwNotificationMask;             /*  internally used Notification Mask */
    EC_T_VOID*      pCallerData;                    /*< in  caller data */
    EC_PF_NOTIFY    S_pfnNotify;                    /*< in  notify callback function pointer */
    EC_T_BOOL       bInRes1;                        /*< in  reserved */
    EC_T_BOOL       bInRes2;                        /*< in  reserved */
} EC_T_CLNTDESC;

/* Structure that indicates a setProcessData/getProcessData request */ 
typedef struct _EC_T_SET_GET_PROCESS_DATA_DESC
{
    EC_T_BOOL   bOutputData;        /*< EC_TRUE:Output data, EC_FALSE: Input data */
    EC_T_DWORD  dwBitOffset;        /*< Bit offset in PD Image */
    EC_T_BYTE*  pbyData;            /*< Data container */
    EC_T_DWORD  dwBitLength;        /*< Bits to read */
    EC_T_BOOL   bRequestStarted;    /*< Timeout value */
}EC_T_SET_GET_PROCESS_DATA_DESC, *EC_PT_SET_GET_PROCESS_DATA_DESC;

/* Structure that indicates a force/release request */ 
typedef struct _EC_T_FORCE_RELEASE_PROCESS_DATA_REQ
{
    EC_T_DWORD  dwClientId;         /*< ClientId of connection/client */
    EC_T_BOOL   bOutput;            /*< EC_TRUE:Output data, EC_FALSE: Input data */
    EC_T_BOOL   bRelease;           /*< EC_TRUE:Release, EC_FALSE: Force*/
    EC_T_BOOL   bReleaseAll;        /*< EC_TRUE:Release all */
    EC_T_DWORD  dwBitOffset;        /*< Bit offset in PD Image */
    EC_T_WORD   wBitLength;         /*< Bits to read */
    EC_T_BYTE	abyData[8];           /*< Data container */
    EC_T_BOOL   bRequestStarted;    /*< Timeout value */
} EC_T_FORCE_RELEASE_PROCESS_DATA_REQ;

class CEcMaster;
class CEcDevice;
class CEcRedDevice;

typedef struct _EC_T_ATECATDESC
{
    EC_T_BOOL                       bInitialized;           /**< Initialization flag */
    EC_T_DWORD                      dwInstanceID;           /**< Instance ID */
    EC_T_VOID*                      poClntDescLock;         /* protect client desc list */
    EC_T_DWORD                      dwClntDescInUse;        /* flag client desc list in use */
    EC_T_BYTE                       byNextUniqueClntId;     /*  next free unique client ID */
    EC_T_BYTE                       byNumRegisteredClnts;   /* number of registered clients */
    EC_T_VOID*                      poSetProcessDataDescLock;       /*< Protect the oSetProcessDataDesc */
    EC_T_VOID*                      poGetProcessDataDescLock;       /*< Data description struct for the GetProcessData calls  */
#if (defined INCLUDE_FORCE_PROCESSDATA)
    EC_T_VOID*                      poForceRelProcessDataDescLock;  /*< Data description struct for the emForceProcessDataBits/emReleaseProcessDataBits calls  */
#endif

                                    
    EC_T_STATE                      eMasterState;           /**< Current state of the master stack */

    /* interface state machine */
    EC_T_MCSM_STATE                 EMcSmCurrState;             /**< master control state machine current state */
    EC_T_MCSM_STATE                 EMcSmReqState;              /**< master control state machine requested (final) SM state */
    EC_T_STATE                      EMcSmReqMasterOpState;      /**< master control state machine requested master operational state */

#if (defined INCLUDE_DC_SUPPORT)
    EC_T_BOOL                       bMcSmDcInitReq;             /**< EC_TRUE if DC INIT should be execute */
#endif
    EC_T_DWORD                      dwStateChangeDebug;         /**< state change debug level */

    CEcDevice*                      poEcDevice;             /**< Pointer to the etherCAT device object */
    CEcRedDevice*                   poEcRedDevice;          /**< Pointer to the redundant etherCAT device object */
    CEcMaster*                      poMaster;               /**< Pointer to the etherCAT master object */
    
    EC_T_CLNTDESC*                  pClntDescHead;          /**< Head of client descriptor linked list */
                                    
    EC_T_DWORD                      dwOnMasterTimerCnt;     /**< Counter for OnMasterTimer() calls */
    EC_T_ENABLE_STATUS              EMasterOpStateCyclicErrorNotificationsStatus;  
    EC_T_WORD                       wNumSendCycFrames;     /**< number of frames send by last eUsrJob_SendAllCyc... call */
    EC_T_WORD                       wNumSendAcycFrames;    /**< number of frames send by last eUsrJob_SendAcycFrames... call */

    EC_T_INIT_MASTER_PARMS*         pInitMasterParms;       /**< Original unmodified parameter set, set by ecatInitMaster used by ecatConfigureMaster */
    
    /* callback function to inform application that all cyclic frame(s) are received */
    EC_T_CYCFRAME_RX_CBDESC         oCycRxDesc;

    CEcTimer*                       poMcSlaveErrAckRetryTimeout; /**< slave error acknowledge retry timeout helper */
    EC_T_BOOL                       bJobsActive;                /**< its allowed to execute the jobs   */
    EC_T_BOOL                       bReqJobsInactive;           /**< request to jobs to get inactive   */
    
    /* MC state machine order management */
    EC_T_MCSM_ORDER                 aMcSmOrders[EC_NUMOF_MCSM_ORDERS]; /**< MC state machine order storage */
    CFiFoListDyn<EC_T_MCSM_ORDER*>* pFreeMcSmOrderFifoApi;             /**< Idle orders (used by API) */
    CFiFoListDyn<EC_T_MCSM_ORDER*>* pFreeMcSmOrderFifoInternal;        /**< Idle orders (used by Core) */
    CFiFoListDyn<EC_T_MCSM_ORDER*>* pFreeMcSmOrderFifoAutoRelease;     /**< Idle auto release orders (used by API) */
    CFiFoListDyn<EC_T_MCSM_ORDER*>* pNewPendingMcSmOrderFifo;          /**< New pending orders (used by API and jobs) */
    EC_T_LISTENTRY                  oPendingMcSmOrderList;             /**< Pending orders (used by jobs only) */
    EC_T_DWORD                      dwOrderId;                         /**< store last order id (to determine next unique one) */
    EC_T_DWORD McSmOrderQueueAdd(EC_T_MCSM_ORDER_TYPE       eMcSmOrderType,
                                 EC_T_MCSM_ORDER**          ppoMcSmOrder,
                                 EC_T_BOOL                  bAutoRelease,
                                 EC_T_DWORD                 dwTimeout,
                                 EC_T_STATE                 eReqState = eEcatState_UNKNOWN,
                                 EC_T_MCSM_STATE            eMcSmReqState = eEcatMcSmState_UNKNOWN,
                                 EC_T_MCSM_ORDER_CONTEXT*   pOrderContext = EC_NULL
                                 );
    EC_T_VOID  McSmOrderFinish(  EC_T_MCSM_ORDER*       poMcSmOrder);
    EC_T_VOID  McSmOrderRelease( EC_T_MCSM_ORDER*       poMcSmOrder);
    EC_T_VOID  McSmCancelAllOrders(EC_T_VOID);

    EC_T_SET_GET_PROCESS_DATA_DESC  oSetProcessDataDesc;        /**< Data description struct for the SetProcessData calls */
    EC_T_SET_GET_PROCESS_DATA_DESC  oGetProcessDataDesc;        /**< Data description struct for the GetProcessData calls*/
#if (defined INCLUDE_FORCE_PROCESSDATA)
    EC_T_FORCE_RELEASE_PROCESS_DATA_REQ oForceProcessDataDesc;
#endif
    EC_T_MCSM_ORDER*                pMcSmOrderBusScan;          /**< Pointer to running/pending bus scan order for McSM */
    EC_T_DWORD                      dwOrderTimeout;             /**< Order Timeout */
    EC_T_LINKMODE                   eLinkMode;
    EC_T_DWORD                      dwPrevRefreshLinkStatusCnt;
    EC_T_DWORD                      dwRefreshLinkStatusCnt;
} EC_T_ATECATDESC;

class CAtEmInterface;
typedef struct _EC_T_ATECATMULTIMASTER
{
    CAtEmInterface*     pInst;                      /**<  instance pointer to master instance */
    EC_T_BOOL           bExtClkShutdownReq;         /**< Request to shutdown (disable) ClkTickAnnounce() */
    EC_T_BOOL           bExtClkShutdownAck;         /**< Acknowledge to shutdown (disable) ClkTickAnnounce() */
} EC_T_ATECATMULTIMASTER;

typedef struct _EC_T_LINK_DEV_RESTORE_PARAM
{
    EC_T_CHAR             szDriverIdent[MAX_DRIVER_IDENT_LEN];
    EC_T_LINK_DRV_DESC    oLinkDrv;
} EC_T_LINK_DEV_RESTORE_PARAM;

/* static master configuration */
typedef struct _EC_T_MASTER_CONFIG
{
    EC_T_INT    dwMaxSlavesProcessedPerCycle;   /*<  */
    EC_T_DWORD  dwEcatCmdTimeout;               /*< timeout to send pending ethercat command frames */
    EC_T_DWORD  dwEcatCmdMaxRetries;            /*< maximum retries to send pending ethercat command frames */
    EC_T_DWORD  dwEoeTimeout;                   /*< timeout sending EoE frames */
    EC_T_DWORD  dwMaxQueuedEthFrames;           /*< maximum number of queued Ethernet frames */
    EC_T_DWORD  dwAdditionalEoEEndpoints;
    EC_T_DWORD  dwMaxSlaveCmdPerFrame;          /*< maximum number of slave commands per frame */
    EC_T_DWORD  dwBusCycleTimeUsec;             /*< [usec] bus cycle time in microseconds */
    EC_T_DWORD  dwLogLevel;                     /*< log level. See EC_LOG_LEVEL_... */
    EC_T_DWORD  dwStateChangeDebug;             /*< Slave state change debugging level: Bit 0 = OsDbgMsg Bit 1 = Link-Msg */
    EC_T_DWORD  dwErrorMsgToLinkLayer;          /*< if EC_TRUE error messages are sent to the link layer */
    EC_T_DWORD  dwMaxSentQueuedFramesPerCyc;    /*< max number of queued frames sent with eUsrJob_SendAcycFrames within one cycle */
} EC_T_MASTER_CONFIG;
typedef struct _EC_T_MASTER_CONFIG_EX
{
    EC_T_DWORD  dwInstanceID;
    EC_T_BOOL   bOnlyProcDataInImage;           /* copy only proc. data into input image */
    EC_T_BOOL   bUseRedundancy;
    EC_T_WORD   wMaxBusSlaves;                  /* the maximum number of bus slaves  */
} EC_T_MASTER_CONFIG_EX;

/* master descriptor used for "Fast mode" */
typedef struct _EC_T_FAST_MASTER_DESC
{
    EC_T_BYTE*              pPDOut;
    EC_T_BYTE*              pPDIn;

    CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc;   /* Pointer to the singular cyclic command */

    EC_T_LINK_DRV_DESC*     poLinkDrvDesc;

    EC_T_WORD               wNumSendCycFrames;      /**< number of frames send by last emFastSendAllCycFrames call 
                                                      can be 0 or 1, becuase only one frame supported*/
} EC_T_FAST_MASTER_DESC;


/*-CLASS---------------------------------------------------------------------*/
class CAtEmInterface
{
public:
    CAtEmInterface();
    virtual ~CAtEmInterface();
    
    EC_T_ATECATDESC m_oAtEcatDesc;

    EC_T_DWORD  InitMaster(   EC_T_DWORD                          dwInstanceID,
                              EC_T_BOOL                           bFirstInitialization,
                              EC_T_INIT_MASTER_PARMS*             pParms,
                              struct _EC_T_INTFEATUREFIELD_DESC*  pFeatures,
                              EC_T_LINK_DEV_RESTORE_PARAM*        pRestoreParam,
                              EC_T_LINK_DEV_RESTORE_PARAM*        pRedRestoreParam,
                              EC_T_CYCFRAME_RX_CBDESC*            pCycRxDesc,
                              EC_T_CNF_TYPE                       eCnfType,
                              EC_T_PBYTE                          pbyCnfData,
                              EC_T_DWORD                          dwCnfDataLen    );
                                          
    EC_T_DWORD  DeinitMaster(             EC_T_VOID                               );
                                          
    EC_T_DWORD  GetMasterParms(           EC_T_INIT_MASTER_PARMS* pParms,
                                          EC_T_DWORD              dwParmsBufSize  );
                                          
    EC_T_DWORD  ChangeMasterParms(        EC_T_INIT_MASTER_PARMS* pParms          );

#if (defined INCLUDE_OEM)
    EC_T_VOID   SetOemKey(                EC_T_UINT64             qwOemKey        );
    EC_T_DWORD  CheckOemKey(              EC_T_UINT64             qwOemKey        );
#endif /* INCLUDE_OEM */

    EC_T_DWORD  IoControl(                EC_T_DWORD              dwCode, 
                                          EC_T_IOCTLPARMS*        pParms          );
                                          
    EC_T_DWORD  ScanBus(                  EC_T_DWORD              dwTimeout       );
                                          
#if (defined INCLUDE_RESCUE_SCAN)
    EC_T_DWORD  RescueScan(               EC_T_DWORD              dwTimeout       );
#endif /* INCLUDE_RESCUE_SCAN */

    EC_T_DWORD  GetMasterInfo(            EC_T_MASTER_INFO*       pMasterInfo     );

    EC_T_DWORD  ConfigureMaster(          EC_T_CNF_TYPE           eCnfType,
                                          EC_T_PBYTE              pbyCnfData, 
                                          EC_T_DWORD              dwCnfDataLen    );
                                          
    EC_T_DWORD  ConfigLoad(               EC_T_CNF_TYPE           eCnfType,
                                          EC_T_PBYTE              pbyCnfData,
                                          EC_T_DWORD              dwCnfDataLen    );
                                          
    EC_T_DWORD  ConfigExcludeSlave(       EC_T_WORD               wPhysAddress    );
                                          
    EC_T_DWORD  ConfigIncludeSlave(       EC_T_WORD               wPhysAddress    );
                                          
    EC_T_DWORD  ConfigSetPreviousPort(    EC_T_WORD               wPhysAddress,
                                          EC_T_WORD               wPhysAddressPrev,
                                          EC_T_WORD               wPortPrev       );
                                          
    EC_T_DWORD  ConfigApply(              EC_T_VOID                               );
                                          
    EC_T_DWORD  ConfigExtend(             EC_T_BOOL               bResetConfig,
                                          EC_T_DWORD              dwTimeout       );

    EC_T_DWORD  SetMasterInitParmBusCycleTimeUsec(EC_T_DWORD      dwBusCycleTimeUsec);
    EC_T_VOID   CalculateDefaultMcSmOrderTimeout(EC_T_DWORD       dwBusCycleTimeUsec);
                                          
    EC_T_DWORD  SetMasterState(           EC_T_DWORD              dwTimeout,
                                          EC_T_STATE              eReqState       );
    EC_T_STATE  GetMasterState(           EC_T_VOID                               );
                                          
    EC_T_DWORD  Start(                    EC_T_DWORD              dwTimeout       );
    EC_T_DWORD  Stop(                     EC_T_DWORD              dwTimeout       );
                                          
    EC_T_DWORD  RefreshProcessData(       EC_T_DWORD              dwTimeout       );
                                          
    EC_T_DWORD  GetSlaveId(               EC_T_WORD               wStationAddress );
    EC_T_DWORD  GetSlaveIdAtPosition(     EC_T_WORD               wAutoIncAddress );
                                          
    EC_T_DWORD  GetSlaveFixedAddr(        EC_T_DWORD              dwSlaveId,           
                                          EC_T_WORD*              pwFixedAddr     );
    EC_T_BOOL   GetSlaveProp(             EC_T_DWORD              dwSlaveId, 
                                          EC_T_SLAVE_PROP*        pSlaveProp      );
    EC_T_DWORD  GetSlavePortState(        EC_T_DWORD              dwSlaveId, 
                                          EC_T_WORD*              pwPortState     );
    EC_T_DWORD  GetSlaveState(            EC_T_DWORD              dwSlaveId, 
                                          EC_T_WORD*              pwCurrDevState, 
                                          EC_T_WORD*              pwReqDevState   );
    EC_T_DWORD  SetSlaveState(            EC_T_DWORD              dwSlaveId, 
                                          EC_T_WORD               wNewReqDevState, 
                                          EC_T_DWORD              dwTimeout       );
    EC_T_DWORD  ReadSlaveRegister(        EC_T_BOOL               bFixedAddressing,
                                          EC_T_WORD               wSlaveAddress,
                                          EC_T_WORD               wRegisterOffset,
                                          EC_T_VOID*              pvData,
                                          EC_T_WORD               wLen,
                                          EC_T_DWORD              dwTimeout       );
    EC_T_DWORD  WriteSlaveRegister(       EC_T_BOOL               bFixedAddressing,
                                          EC_T_WORD               wSlaveAddress,
                                          EC_T_WORD               wRegisterOffset,
                                          EC_T_VOID*              pvData,
                                          EC_T_WORD               wLen,
                                          EC_T_DWORD              dwTimeout       );
    EC_T_DWORD  ReadSlaveIdentification(  EC_T_BOOL               bFixedAddressing,
                                          EC_T_WORD               wSlaveAddress,
                                          EC_T_WORD               wAdo,
                                          EC_T_WORD*              pwValue,
                                          EC_T_DWORD              dwTimeout);
    EC_T_DWORD  SetSlaveDisabled(         EC_T_BOOL               bFixedAddressing,
                                          EC_T_WORD               wSlaveAddress,
                                          EC_T_BOOL               bDisabled);
    EC_T_DWORD  SetSlaveDisconnected(     EC_T_BOOL               bFixedAddressing,
                                          EC_T_WORD               wSlaveAddress,
                                          EC_T_BOOL               bDisabled);
    EC_T_DWORD  TferSingleRawCmd(         EC_T_DWORD              dwSlaveId, 
                                          EC_T_BYTE               byCmd, 
                                          EC_T_DWORD              dwMemoryAddress, 
                                          EC_T_VOID*              pvData, 
                                          EC_T_WORD               wLen, 
                                          EC_T_DWORD              dwTimeout       );
    EC_T_DWORD  QueueRawCmd(              EC_T_DWORD              dwClntId,
                                          EC_T_DWORD              dwSlaveId,
                                          EC_T_WORD               wInvokeId,
                                          EC_T_BYTE               byCmd,
                                          EC_T_DWORD              dwMemoryAddress,
                                          EC_T_VOID*              pvData,
                                          EC_T_WORD               wLen            );
    EC_T_DWORD  GetNumConfiguredSlaves(   EC_T_VOID                               );
    static                                
    EC_T_MBXTFER* MbxTferCreate(          EC_T_MBXTFER_DESC*      pMbxTferDesc,
                                          struct _EC_T_MEMORY_LOGGER* pMLog    );
    static                                
    EC_T_DWORD    MbxTferAbort(           EC_T_MBXTFER*           pMbxTfer        );
    static                                
    EC_T_VOID     MbxTferDelete(          EC_T_MBXTFER*           pMbxTfer,
                                          struct _EC_T_MEMORY_LOGGER* pMLog );
                                          
#if (defined INCLUDE_RAWMBX_SUPPORT)      
    EC_T_DWORD  SendRawMbx(               EC_T_DWORD              dwClntId, 
                                          EC_T_BYTE*              pbyMbxCmd,
                                          EC_T_DWORD              dwMbxCmdLen,
                                          EC_T_DWORD              dwTimeout);
#endif                                    
    EC_T_DWORD  CoeSdoDownloadReq(        EC_T_MBXTFER*           pMbxTfer, 
                                          EC_T_DWORD              dwSlaveId, 
                                          EC_T_WORD               wObIndex, 
                                          EC_T_BYTE               byObSubIndex, 
                                          EC_T_DWORD              dwTimeout,
                                          EC_T_DWORD              dwFlags         );
    EC_T_DWORD  CoeSdoDownload(           EC_T_DWORD              dwSlaveId,
                                          EC_T_WORD               wObIndex,
                                          EC_T_BYTE               byObSubIndex,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD              dwDataLen,
                                          EC_T_DWORD              dwTimeout,
                                          EC_T_DWORD              dwFlags         );
    EC_T_DWORD  CoeSdoUploadReq(          EC_T_MBXTFER*           pMbxTfer, 
                                          EC_T_DWORD              dwSlaveId, 
                                          EC_T_WORD               wObIndex, 
                                          EC_T_BYTE               byObSubIndex, 
                                          EC_T_DWORD              dwTimeout,
                                          EC_T_DWORD              dwFlags         );
    EC_T_DWORD  CoeSdoUpload(             EC_T_DWORD              dwSlaveId,
                                          EC_T_WORD               wObIndex,
                                          EC_T_BYTE               byObSubIndex,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD              dwDataLen,
                                          EC_T_DWORD*             pdwOutDataLen,
                                          EC_T_DWORD              dwTimeout,
                                          EC_T_DWORD              dwFlags         );
    EC_T_DWORD  CoeGetODList(             EC_T_MBXTFER*           pMbxTfer, 
                                          EC_T_DWORD              dwSlaveId, 
                                          EC_T_COE_ODLIST_TYPE    eListType, 
                                          EC_T_DWORD              dwTimeout       );
    EC_T_DWORD  CoeGetObjectDesc(         EC_T_MBXTFER*           pMbxTfer, 
                                          EC_T_DWORD              dwSlaveId, 
                                          EC_T_WORD               wObIndex, 
                                          EC_T_DWORD              dwTimeout       );
    EC_T_DWORD  CoeGetEntryDesc(          EC_T_MBXTFER*           pMbxTfer, 
                                          EC_T_DWORD              dwSlaveId, 
                                          EC_T_WORD               wObIndex, 
                                          EC_T_BYTE               byObSubIndex, 
                                          EC_T_BYTE               byValueInfo,
                                          EC_T_DWORD              dwTimeout       );
#if (defined INCLUDE_COE_PDO_SUPPORT)     
    EC_T_DWORD  CoeRxPdoTfer(             EC_T_MBXTFER*           pMbxTfer,
                                          EC_T_DWORD              dwSlaveId,
                                          EC_T_DWORD              dwNumber,
                                          EC_T_DWORD              dwTimeout       );
#endif /* INCLUDE_COE_PDO_SUPPORT */      
#if (defined INCLUDE_SOE_SUPPORT)         
    EC_T_DWORD SoeWrite(                  EC_T_DWORD              dwSlaveId,
                                          EC_T_BYTE               byDriveNo,
                                          EC_T_BYTE*              pbyElementFlags,
                                          EC_T_WORD               wIDN,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD              dwDataLen,
                                          EC_T_DWORD              dwTimeout       );
    EC_T_DWORD SoeWriteReq(               EC_T_MBXTFER*           pMbxTfer,  
                                          EC_T_DWORD              dwSlaveId,
                                          EC_T_BYTE               byDriveNo,
                                          EC_T_BYTE*              pbyElementFlags,
                                          EC_T_WORD               wIDN,	
                                          EC_T_DWORD              dwTimeout       );
    EC_T_DWORD SoeRead(                   EC_T_DWORD              dwSlaveId,
                                          EC_T_BYTE               byDriveNo,
                                          EC_T_BYTE*              pbyElementFlags,
                                          EC_T_WORD               wIDN,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD              dwDataLen,
                                          EC_T_DWORD*             pdwOutDataLen,
                                          EC_T_DWORD              dwTimeout       );
    EC_T_DWORD SoeReadReq(                EC_T_MBXTFER*           pMbxTfer,  
                                          EC_T_DWORD              dwSlaveId,
                                          EC_T_BYTE               byDriveNo,
                                          EC_T_BYTE*              pbyElementFlags,
                                          EC_T_WORD               wIDN,
                                          EC_T_DWORD              dwTimeout       );
    EC_T_DWORD SoeAbortProcCmd(           EC_T_DWORD              dwSlaveId,
                                          EC_T_BYTE               byDriveNo,
                                          EC_T_BYTE*              pbyElementFlags,
                                          EC_T_WORD               wIDN,
                                          EC_T_DWORD              dwTimeout       );
#endif /* INCLUDE_SOE_SUPPORT */          
#if (defined INCLUDE_FOE_SUPPORT)         
    EC_T_DWORD  FoeFileUpload(            EC_T_DWORD              dwSlaveId,
                                          EC_T_CHAR*              szFileName,
                                          EC_T_DWORD              dwFileNameLen,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD              dwDataLen,
                                          EC_T_DWORD*             pdwOutDataLen,
                                          EC_T_DWORD              dwPassword,       
                                          EC_T_DWORD              dwTimeout       );  
                                          
                                          
    EC_T_DWORD  FoeUploadReq(             EC_T_MBXTFER*           pMbxTfer,     
                                          EC_T_DWORD              dwSlaveId,
                                          EC_T_CHAR*              szFileName,
                                          EC_T_DWORD              dwFileNameLen,
                                          EC_T_DWORD              dwPassword,       
                                          EC_T_DWORD              dwTimeout       );  
                                          
    EC_T_DWORD  FoeFileDownload(          EC_T_DWORD              dwSlaveId,
                                          EC_T_CHAR*              szFileName,
                                          EC_T_DWORD              dwFileNameLen,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD              dwDataLen,
                                          EC_T_DWORD              dwPassword,       
                                          EC_T_DWORD              dwTimeout       ); 
                                          
    EC_T_DWORD  FoeDownloadReq(           EC_T_MBXTFER*           pMbxTfer,     
                                          EC_T_DWORD              dwSlaveId,
                                          EC_T_CHAR*              szFileName,
                                          EC_T_DWORD              dwFileNameLen,
                                          EC_T_DWORD              dwPassword,       
                                          EC_T_DWORD              dwTimeout       ); 
    EC_T_DWORD  FoeSegmentedDownloadReq(  
                                          EC_T_MBXTFER*           pMbxTfer,     
                                          EC_T_DWORD              dwSlaveId,
                                          EC_T_CHAR*              szFileName,
                                          EC_T_DWORD              dwFileNameLen,
                                          EC_T_DWORD              dwFileSize,
                                          EC_T_DWORD              dwPassword,       
                                          EC_T_DWORD              dwTimeout       ); 
    EC_T_DWORD  FoeSegmentedUploadReq(    
                                          EC_T_MBXTFER*           pMbxTfer,     
                                          EC_T_DWORD              dwSlaveId,
                                          EC_T_CHAR*              szFileName,
                                          EC_T_DWORD              dwFileNameLen,
                                          EC_T_DWORD              dwFileSize,
                                          EC_T_DWORD              dwPassword,       
                                          EC_T_DWORD              dwTimeout       ); 
#endif /* INCLUDE_FOE_SUPPORT */
#if (defined INCLUDE_EOE_ENDPOINT) && (defined INCLUDE_EOE_SUPPORT)
    EC_T_DWORD  EoeRegisterEndpoint(      EC_T_CHAR*              szEoEDrvIdent,
                                          EC_T_LINK_DRV_DESC*     pLinkDrvDesc    ); 
                                          
    EC_T_DWORD  EoeUnregisterEndpoint(    EC_T_LINK_DRV_DESC*     pLinkDrvDesc    ); 
#endif /* INCLUDE_EOE_ENDPOINT && INCLUDE_EOE_SUPPORT */

#if (defined INCLUDE_VOE_SUPPORT)
    EC_T_DWORD VoeRead(                   EC_T_DWORD              dwSlaveId,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD              dwDataLen,
                                          EC_T_DWORD*             pdwOutDataLen,
                                          EC_T_DWORD              dwTimeout       );
                                          
    EC_T_DWORD VoeWriteReq(               EC_T_MBXTFER*           pMbxTfer,
                                          EC_T_DWORD              dwSlaveId,
                                          EC_T_DWORD              dwTimeout       );
                                          
    EC_T_DWORD VoeWrite(                  EC_T_DWORD              dwSlaveId,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD              dwDataLen,
                                          EC_T_DWORD              dwTimeout       );
#endif /* INCLUDE_VOE_SUPPORT */          
#if (defined INCLUDE_AOE_SUPPORT)         
    EC_T_DWORD AoeGetSlaveNetId(          EC_T_DWORD              dwSlaveId, 
                                          EC_T_AOE_NETID*         poAoeNetId      );
                                          
    EC_T_DWORD AoeRead(                   EC_T_DWORD              dwSlaveId,
                                          EC_T_AOE_NETID*         poTargetNetId,           
                                          EC_T_WORD               wTargetPort,
                                          EC_T_DWORD              dwIndexGroup,
                                          EC_T_DWORD              dwIndexOffset,
                                          EC_T_DWORD              dwDataLength,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD*             pdwDataOutLen,
                                          EC_T_DWORD*             pdwErrorCode,
                                          EC_T_DWORD*             pdwCmdResult,
                                          EC_T_DWORD              dwTimeout       );
                                          
    EC_T_DWORD AoeReadReq(                EC_T_MBXTFER*           pMbxTfer,
                                          EC_T_DWORD              dwSlaveId,
                                          EC_T_AOE_NETID*         poTargetNetId,           
                                          EC_T_WORD               wTargetPort,
                                          EC_T_DWORD              dwIndexGroup,
                                          EC_T_DWORD              dwIndexOffset,
                                          EC_T_DWORD              dwTimeout       );
                                          
                                          
    EC_T_DWORD AoeWrite(                  EC_T_DWORD              dwSlaveId,
                                          EC_T_AOE_NETID*         poTargetNetId,           
                                          EC_T_WORD               wTargetPort,
                                          EC_T_DWORD              dwIndexGroup,
                                          EC_T_DWORD              dwIndexOffset,
                                          EC_T_DWORD              dwDataLength,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD*             pdwErrorCode,
                                          EC_T_DWORD*             pdwCmdResult,
                                          EC_T_DWORD              dwTimeout       );
                                          
    EC_T_DWORD AoeWriteReq(               EC_T_MBXTFER*           pMbxTfer,         
                                          EC_T_DWORD              dwSlaveId,
                                          EC_T_AOE_NETID*         poTargetNetId,           
                                          EC_T_WORD               wTargetPort,
                                          EC_T_DWORD              dwIndexGroup,
                                          EC_T_DWORD              dwIndexOffset,
                                          EC_T_DWORD              dwTimeout       );
                                          
    EC_T_DWORD AoeReadWrite(              EC_T_DWORD              dwSlaveId,
                                          EC_T_AOE_NETID*         poTargetNetId,           
                                          EC_T_WORD               wTargetPort,
                                          EC_T_DWORD              dwIndexGroup,
                                          EC_T_DWORD              dwIndexOffset,
                                          EC_T_DWORD              dwReadDataLen,
                                          EC_T_DWORD              dwWriteDataLength,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD*             pdwDataOutLen,
                                          EC_T_DWORD*             pdwErrorCode,
                                          EC_T_DWORD*             pdwCmdResult,
                                          EC_T_DWORD              dwTimeout       );
                                          
    EC_T_DWORD AoeWriteControl(           EC_T_DWORD              dwSlaveId,
                                          EC_T_AOE_NETID*         poTargetNetId,
                                          EC_T_WORD               wTargetPort,
                                          EC_T_WORD               wAoEState,
                                          EC_T_WORD               wDeviceState,
                                          EC_T_DWORD              dwDataLen,
                                          EC_T_BYTE*              pbyData,
                                          EC_T_DWORD*             pdwErrorCode,
                                          EC_T_DWORD*             pdwCmdResult,
                                          EC_T_DWORD              dwTimeout       );
#endif /* INCLUDE_AOE_SUPPORT */          
                                          
    EC_T_DWORD  GetNumConnectedSlaves(    EC_T_VOID                               );
#ifdef INCLUDE_RED_DEVICE
    EC_T_DWORD  GetNumConnectedSlavesMain(EC_T_VOID                             );
    EC_T_DWORD  GetNumConnectedSlavesRed(  EC_T_VOID                            );
#endif

    EC_T_DWORD  GetProcessData(         EC_T_BOOL               bOutputData,
                                        EC_T_DWORD              dwOffset,
                                        EC_T_BYTE*              pbyData,
                                        EC_T_DWORD              dwLength,
                                        EC_T_DWORD              dwTimeout       );
    
    EC_T_DWORD  SetProcessData(         EC_T_BOOL               bOutputData,
                                        EC_T_DWORD              dwOffset,
                                        EC_T_BYTE*              pbyData,
                                        EC_T_DWORD              dwLength,
                                        EC_T_DWORD              dwTimeout       );

    EC_T_DWORD GetProcessDataBits(      EC_T_BOOL               bOutputData,    
                                        EC_T_DWORD              dwBitOffsetPd,  
                                        EC_T_BYTE*              pbyDataDst,     
                                        EC_T_DWORD              dwBitLengthDst, 
                                        EC_T_DWORD              dwTimeout       );

    EC_T_DWORD SetProcessDataBits(      EC_T_BOOL               bOutputData,    
                                        EC_T_DWORD              dwBitOffsetPd,  
                                        EC_T_BYTE*              pbyDataSrc,     
                                        EC_T_DWORD              dwBitLengthSrc, 
                                        EC_T_DWORD              dwTimeout       );

#if (defined INCLUDE_FORCE_PROCESSDATA)
    EC_T_DWORD ForceProcessDataBits(    EC_T_DWORD				dwClientId,
										EC_T_BOOL               bOutputData,
                                        EC_T_DWORD              dwBitOffsetPd,  
                                        EC_T_WORD               wBitLength,    
                                        EC_T_BYTE*              pbyData,     
                                        EC_T_DWORD              dwTimeout);

    EC_T_DWORD ReleaseProcessDataBits(  EC_T_DWORD				dwClientId,
										EC_T_BOOL               bOutputData,
                                        EC_T_BOOL               bReleaseAll,
                                        EC_T_DWORD              dwBitOffsetPd,  
                                        EC_T_WORD               wBitLength,    
                                        EC_T_DWORD              dwTimeout);
#endif

    EC_T_DWORD  ReadSlaveEEPRom(        EC_T_BOOL               bFixedAddressing,
                                        EC_T_WORD               wSlaveAddress,
                                        EC_T_WORD               wEEPRomStartOffset,
                                        EC_T_WORD*              pwReadData,
                                        EC_T_DWORD              dwReadLen,
                                        EC_T_DWORD*             pdwNumOutData,
                                        EC_T_DWORD              dwTimeout       );

    EC_T_DWORD  WriteSlaveEEPRom(       EC_T_BOOL               bFixedAddressing,
                                        EC_T_WORD               wSlaveAddress,
                                        EC_T_WORD               wEEPRomStartOffset,
                                        EC_T_WORD*              pwWriteData,
                                        EC_T_DWORD              dwWriteLen,
                                        EC_T_DWORD              dwTimeout       );

    EC_T_DWORD  ReloadSlaveEEPRom(      EC_T_BOOL               bFixedAddressing, 
                                        EC_T_WORD               wSlaveAddress,    
                                        EC_T_DWORD              dwTimeout       );

    EC_T_DWORD  ResetSlaveController(   EC_T_BOOL               bFixedAddressing, 
                                        EC_T_WORD               wSlaveAddress,    
                                        EC_T_DWORD              dwTimeout       );
                                        
    EC_T_DWORD  AssignSlaveEEPRom(      EC_T_BOOL               bFixedAddressing,
                                        EC_T_WORD               wSlaveAddress,
                                        EC_T_BOOL               bSlavePDIAccessEnable,
                                        EC_T_BOOL               bForceAssign,
                                        EC_T_DWORD              dwTimeout       );

    EC_T_DWORD  ActiveSlaveEEProm(      EC_T_BOOL               bFixedAddressing,
                                        EC_T_WORD               wSlaveAddress,
                                        EC_T_BOOL*              pbSlavePDIAccessActive,
                                        EC_T_DWORD              dwTimeout       );
                                        
    static 
    EC_T_DWORD  WrapperToMboxNotify(    EC_T_DWORD              dwInstanceID
                                       ,PEcMailboxCmd           pCmd
                                       ,EC_T_BYTE*              pbyMbxData
                                       ,EC_T_DWORD              dwMbxDataLength
                                       ,EC_T_DWORD              dwMbxResult     );

    static 
    EC_T_DWORD  WrapperToNotifyAllClients( 
                                        EC_T_DWORD              dwInstanceID, 
                                        EC_T_DWORD              dwClientID,
                                        EC_T_DWORD              dwCode, 
                                        EC_T_NOTIFYPARMS*       pParms          );

    EC_T_DWORD  HCAcceptTopoChange(     EC_T_VOID                               );
    EC_T_DWORD  HCGetNumGroupMembers(   EC_T_DWORD              dwGroupIndex    );
    EC_T_DWORD  HCGetSlaveIdsOfGroup(   EC_T_DWORD              dwGroupIndex,
                                        EC_T_DWORD*             adwSlaveId,
                                        EC_T_DWORD              dwMaxNumSlaveIds );
    EC_T_DWORD  SetSlavePortState(      EC_T_DWORD              dwSlaveId,
                                        EC_T_WORD               wPort,
                                        EC_T_BOOL               bClose,
                                        EC_T_BOOL               bForce,
                                        EC_T_DWORD              dwTimeout       );

    EC_T_DWORD SlaveSerializeMbxTfers(  EC_T_DWORD              dwSlaveId       );

    EC_T_DWORD SlaveParallelMbxTfers(   EC_T_DWORD              dwSlaveId       );

    EC_T_DWORD RegisterClient(          EC_T_REGISTERPARMS*     pParms, 
                                        EC_T_REGISTERRESULTS*   pResults        );
    
    EC_T_DWORD UnregisterClient(        EC_T_DWORD              dwClntId        );

    EC_T_DWORD BlockNode(               EC_T_SB_MISMATCH_DESC*  pMisMatch,
                                        EC_T_DWORD              dwTimeout       );

    EC_T_DWORD OpenBlockedPorts(        EC_T_DWORD              dwTimeout       );
    EC_T_DWORD ForceTopologyChange(     EC_T_VOID                               );
    EC_T_DWORD IsTopologyChangeDetected(EC_T_BOOL*              pbIsTopologyChangeDetected);

    EC_T_DWORD IsSlavePresent(          EC_T_DWORD              dwSlaveId,
                                        EC_T_BOOL*              pbPresence      );

    EC_T_VOID  EnableJobs(              EC_T_VOID                               );
    EC_T_VOID  DisableJobs(             EC_T_DWORD              dwTimeout       );
    EC_T_DWORD ExecJobPrivate(          EC_T_USER_JOB           EUserJob,
                                        EC_T_VOID*              pvParam,
                                        EC_T_BOOL               bForceJob       );
#if (defined INCLUDE_PASS_THROUGH_SERVER)

    EC_T_DWORD PassThroughSrvStart(     EC_T_PTS_SRV_START_PARMS* poPtsStartParams, 
                                        EC_T_DWORD dwTimeout                    );

    EC_T_DWORD PassThroughSrvStop(      EC_T_DWORD dwTimeout                    );

    EC_T_DWORD PassThroughSrvEnable(    EC_T_DWORD dwTimeout                    );

    EC_T_DWORD PassThroughSrvDisable(   EC_T_DWORD dwTimeout                    );

    EC_PTS_STATE PassThroughSrvGetStatus( EC_T_VOID                             );
#endif

#if (defined INCLUDE_ADS_ADAPTER)
    EC_T_DWORD AdsAdapterStart(     EC_T_ADS_ADAPTER_START_PARMS* poStartParams, 
                                        EC_T_DWORD dwTimeout                    );

    EC_T_DWORD AdsAdapterStop(      EC_T_DWORD dwTimeout                    );
#endif /* INCLUDE_ADS_ADAPTER */
    inline EC_T_BOOL AdsIsRunning();
    
    struct _EC_T_MEMORY_LOGGER* GetMemLog() { return &m_oMemLog; }

    EC_T_MEMORY_LOGGER      m_oMemLog;

private:

    EC_T_MBXTFERP*          m_pMbxCoeEmergencyTferPriv;
    EC_T_MBXTFERP*          m_pMbxSoeEmergencyTferPriv;
    EC_T_MBXTFERP*          m_pMbxSoeNotificationTferPriv;

    CEcDeviceFactory*       m_pFactory;
    CEcConfigData*          m_poEcConfigData;
#if (defined INCLUDE_CONFIG_EXTEND)
    CEcConfigData*          m_poEcConfigDataExtended;
#endif

    EC_T_MASTER_CONFIG_EX   m_oMasterConfigEx;
    EC_T_CONFIGPARM_DESC    m_oCfgParm;

#if (defined INCLUDE_ADS_ADAPTER) && (defined EC_SOCKET_IP_SUPPORTED)
    EC_T_PVOID              m_pAdsSrvHandle;
#endif /* INCLUDE_ADS_ADAPTER  && EC_SOCKET_IP_SUPPORTED */
    EC_T_DWORD              m_dwInternalIOControlCode;

    EC_T_DWORD  InitMasterEx(    EC_T_DWORD                      dwInstanceID,
                                 EC_T_BOOL                       bFirstInitialization,
                                 EC_T_INIT_MASTER_PARMS*         pParms,
                                 struct _EC_T_INTFEATUREFIELD_DESC* pFeatures,
                                 EC_T_LINK_DEV_RESTORE_PARAM*    pRestoreParam,
                                 EC_T_LINK_DEV_RESTORE_PARAM*    pRedRestoreParam,
                                 EC_T_CYCFRAME_RX_CBDESC*        pCycRxDesc,
                                 EC_T_CNF_TYPE                   eCnfType,
                                 EC_T_PBYTE                      pbyCnfData,
                                 EC_T_DWORD                      dwCnfDataLen    );
    EC_T_DWORD DeinitMasterEx(   EC_T_BOOL bDeinitForConfiguration,
                                 EC_T_BOOL bResetStates                  );

    EC_T_DWORD CheckMasterParms( EC_T_INIT_MASTER_PARMS* pParms          );

    EC_T_DWORD SetMasterStateEx( EC_T_DWORD dwTimeout, 
                                 EC_T_STATE eReqState);

    EC_T_VOID  McStateMachine(   EC_T_VOID );
    EC_T_DWORD McSmOrderAdd(     EC_T_MCSM_ORDER_TYPE       eMcSmOrderType,
                                 EC_T_MCSM_ORDER**          ppoMcSmOrder,
                                 EC_T_BOOL                  bAutoRelease,
                                 EC_T_DWORD                 dwTimeout,
                                 EC_T_STATE                 eReqState = eEcatState_UNKNOWN,
                                 EC_T_MCSM_STATE            eMcSmReqState = eEcatMcSmState_UNKNOWN,
                                 EC_T_MCSM_ORDER_CONTEXT*   pOrderContext = EC_NULL
                                                                         );
    EC_T_VOID  McSmOrderCancel(  EC_T_MCSM_ORDER*    poMcSmOrder         );
    EC_T_VOID  McSmOrderFinish(  EC_T_MCSM_ORDER*    poMcSmOrder         );

    EC_T_DWORD NotifyAllClients( EC_T_DWORD              dwCode, 
                                 EC_T_DWORD              dwClientID,
                                 EC_T_NOTIFYPARMS*       pParms          );

    EC_T_DWORD MboxNotify(       PEcMailboxCmd              pCmd
                                ,EC_T_BYTE*                 pbyMbxData
                                ,EC_T_DWORD                 dwMbxDataLength
                                ,EC_T_DWORD                 dwMbxResult
                                                                        );

    EC_T_DWORD MbxTfer(          EC_T_MBXTFERP*          pMbxTferPriv, 
                                 EC_T_DWORD              dwSlaveId, 
                                 EC_T_DWORD              dwTimeout,
                                 EC_T_BOOL               bWait           );
    EC_T_VOID UsrCtlClkHelper(   EC_T_VOID                               );

    EC_T_DWORD WaitUntilStableMcSm(EC_T_DWORD dwTimeout);

    EC_T_DWORD RestartMasterForPts();
};

#endif /* INC_ATETHERCATPRIVATE */

/*-END OF SOURCE FILE--------------------------------------------------------*/
