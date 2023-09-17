/*-----------------------------------------------------------------------------
 * EcHotConnect.h           file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Willig, Andreas
 * Description              description of file
 * Date                     2008/11/19::10:01
 *---------------------------------------------------------------------------*/

#ifndef INC_ECHOTCONNECT
#define INC_ECHOTCONNECT  1

/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ECTIMER
#include "EcTimer.h"
#endif

/*-DEFINES-------------------------------------------------------------------*/

#define EC_HC_DFLT_HCMODE           echm_automatic

#define PORT_CLOSE_FLAG             0x80
#define PORT_OPEN_FLAG              0x40
#define PORT_MODIFY_MASK            0xC0

#define PORT_MODE_AUTO              ECM_DLCTRL_LOOP_PORT_VAL_AUTO
#define PORT_MODE_AUTOCL            ECM_DLCTRL_LOOP_PORT_VAL_AUTOCLOSE
#define PORT_MODE_OPEN              ECM_DLCTRL_LOOP_PORT_VAL_ALWAYSOPEN
#define PORT_MODE_CLOSE             ECM_DLCTRL_LOOP_PORT_VAL_ALWAYSCLOSED
#define PORT_MODE_MASK              ECM_DLCTRL_LOOP_PORTS_MASK

#define PORT0_BITOFFSET             ECM_DLCTRL_LOOP_PORTX_SHIFT(ESC_PORT_A)
#define PORT1_BITOFFSET             ECM_DLCTRL_LOOP_PORTX_SHIFT(ESC_PORT_B)
#define PORT2_BITOFFSET             ECM_DLCTRL_LOOP_PORTX_SHIFT(ESC_PORT_C)
#define PORT3_BITOFFSET             ECM_DLCTRL_LOOP_PORTX_SHIFT(ESC_PORT_D)

#define PORTCHG_SOURCE_MANUAL              0x01
#define PORTCHG_SOURCE_BORDERCLOSE         0x02
#define PORTCHG_SOURCE_BLOCKNODE           0x03

/*-TYPEDEFS------------------------------------------------------------------*/
class  CEcMaster;
class  CEcDevice;
class  CEcSlave;
struct _EC_T_HOTCONNECT_GROUP;

typedef enum _EC_T_HCGROUPTYPE
{
    hcgrouptype_mandatory   = 0,        /* mandatory */
    hcgrouptype_optional    = 1,        /* optional free connect */

    hcgrouptype_BCppDummy   = 0xffffffff
} EC_T_HCGROUPTYPE;

/* this is required as all cyclic commands have to be adjusted appropriately (WKC)
 * See EtherCAT ESC Hardware Datasheet Section I - Technology: Working Counter Incremental
 */
typedef struct _EC_T_CYC_CMD_WKC_DESC_ENTRY
{
    struct TEcCycCmdConfigDesc* pCycCmdCfgDesc; /* Cyclic command descriptor (WKC will be adjusted depending on HC groups detected) */
    EC_T_WORD                   wWkcIncrement;  /* expected WKC amount (only the WKC part which is related to the HC group) */
    EC_T_BOOL                   bOptional;      /* Influence is optional (changeable) */
    EC_T_BOOL                   bActive;        /* Influence is active, opt. Node Present, no affect if bOpt = FALSE */
} EC_T_CYC_CMD_WKC_DESC_ENTRY;
typedef struct _EC_T_HOTCONNECT_GROUP
{
    EC_T_DWORD                      dwGrpMembers;       /* Amount of Group Members */
    CEcSlave**                      ppGroupMembers;     /* Array Holding member slave nodes of this HC Group (CEcSlave objects) */

    EC_T_DWORD                      dwIdentCmdsCount;   /* Number of ident cmds in paIdentCmds */
    EcInitCmdDesc**                 paIdentCmds;        /* Array of ident cmds pointer to identify the group */
    EC_T_DWORD                      dwIdentCmdCur;      /* Current evaluate ident cmd */

    EC_T_HCGROUPTYPE                oHCGroupType;       /* Type of HC Group */

    EC_T_CYC_CMD_WKC_DESC_ENTRY*    aCycCmdWkcList;     /* array which is at longest as long as existing cyclic datagrams */
    EC_T_DWORD                      dwMaxNumCycCmdWkcList; /* Maximum number of elements in aCycCmdWkcList array */

    EC_T_DWORD                      dwNumPreviousPorts; /* total number of PreviousPort Entries */
    EC_T_SLAVE_PORT_DESC*           apoPreviousPort;

    EC_T_BOOL                       bGroupComplete;
    EC_T_BOOL                       bGroupPresent;      /* Group is connected */
    EC_T_BOOL                       bGroupTargStateSet; /* Group reached target state (SAFEOP, OP etc.) */
    EC_T_BOOL                       bGroupActive;
    EC_T_BOOL                       bNotifyPresence;
    struct _EC_T_HOTCONNECT_GROUP*  pPrevGroup;         /* Previous group found on bus */

} EC_T_HOTCONNECT_GROUP;

typedef enum _EC_T_EHCSTATEMACHINE
{
    ehcsm_unknown       ,

	/* probe present groups, check if currently present groups are still there (don't look for any other possible HC groups) */
    ehcsm_ppg_init      ,   /* start probe present groups (ppg) */
    ehcsm_ppg_grp_det   ,   /* send ident commands (ppg) */
    ehcsm_ppg_done      ,   /* probe present groups (ppg) done  */

    ehcsm_opcl_ports    ,   /* opening or closing ports in progress */
    
    /* Borland C++ datatype alignment correction */
    ehcsm_BCppDummy     = 0xFFFFFFFF
} EC_T_EHCSTATEMACHINE;

typedef enum _ECHC_T_ESTATE
{
    echc_eIdle          ,   /* Idle, nothing to do */
    echc_eWaitStart     ,   /* Wait for start Probe HC */
    echc_eSBEnhance     ,   /* Do SB Enhancement Scan */
    echc_eScanGrps      ,   /* Scan for new HC Groups */
    echc_eInitStateWait ,   /* wait for New found groups reach init state */
    echc_eReqStateWait  ,   /* wait for New found groups reach target state */
        
    echc_BCppDummy      = 0xFFFFFFFF
} ECHC_T_ESTATE;

typedef struct _EC_T_SLAVE_PORT_DESC_EX
{
    EC_T_DWORD          m_dwDLControlValue;
    EC_T_WORD           m_wSlaveAddress;
    EC_T_WORD           m_wFlags[4];          /* Port Operation Flags */
    EC_T_WORD           m_wReserved;
#if (defined INCLUDE_RED_DEVICE)
    EC_T_WORD           m_wFixedAddrBehind[4];
    EC_T_WORD           m_wPortBehind[4];
#endif /* INCLUDE_RED_DEVICE */
} EC_T_SLAVE_PORT_DESC_EX, *EC_PT_SLAVE_PORT_DESC_EX;

typedef struct _EC_T_HCPORTLISTPORTOP
{
    EC_T_DWORD                  m_dwPortEntries;    /* Open Port Entries */
    EC_T_DWORD                  m_dwMaxPortEntries; /* Possible Open Port Entries */
    EC_T_SLAVE_PORT_DESC_EX*    m_poPorts;          /* List of open Ports on bus */
        
    EC_T_DWORD                  CreatePortList( EC_T_DWORD  dwMaxPortEntries, struct _EC_T_MEMORY_LOGGER* pMLog);
    EC_T_VOID                   DeletePortList( struct _EC_T_MEMORY_LOGGER* pMLog       );
    EC_T_VOID                   ResetPortList(  EC_T_VOID                       );

    EC_T_DWORD                  AddPort(        EC_T_WORD   wSlaveAddr
                                               ,EC_T_WORD   wPort              
                                               ,EC_T_WORD   wFlags              );
    EC_T_DWORD                  AddPort(        EC_T_SLAVE_PORT_DESC_EX* pEntrySrc
                                               ,EC_T_WORD wPort                 );

    EC_T_DWORD                  RemovePort(     EC_T_WORD   wSlaveAddr          
                                               ,EC_T_WORD   wPort = ((EC_T_WORD)-1)
                                                                                );

    EC_T_SLAVE_PORT_DESC_EX*    FindEntry(      EC_T_SLAVE_PORT_DESC_EX* salt
                                               ,EC_T_WORD   wSlaveAddr          );
    EC_T_BOOL                   IsPortInList(   EC_T_WORD   wSlaveAddr
                                               ,EC_T_WORD   wPort               );

} EC_T_HCPORTLISTPORTOP, *EC_PT_HCPORTLISTPORTOP;
     
/*-CLASS---------------------------------------------------------------------*/

class CEcHotConnect
{
public:
    CEcHotConnect();
    ~CEcHotConnect();

    EC_T_VOID               SetMaster(CEcMaster* pMaster, CEcDevice* poEcDevice)
                                        { m_pMaster = pMaster; m_poEcDevice = poEcDevice; }
    EC_T_DWORD              CreateHCGroups( EC_T_DWORD dwGroupCount);
    EC_T_HOTCONNECT_GROUP*  GetGroup( EC_T_DWORD dwIndex );
                                        
    EC_T_DWORD              GetGroupAmount(EC_T_VOID)
                                        { return m_dwHotConGroupListLen; }
    EC_T_DWORD              RecalculateWKC(EC_T_BOOL bAuto);

    EC_T_VOID               CreateCycCmd(EC_T_DWORD dwNumCmds);
    EC_T_VOID               AddCycCmd(EC_T_VOID* pvCycCmdCfgDesc);

    EC_T_DWORD              CreatePortList(EC_T_DWORD dwSlaves);
    
    EC_T_VOID               ProcessResult(EC_T_DWORD result, EC_T_DWORD invokeId, PETYPE_EC_CMD_HEADER pEcCmdHeader);

    EC_T_VOID               RefreshGroupActive(EC_T_HOTCONNECT_GROUP* pHCGroup);
    EC_T_VOID               SetGroupPresence(EC_T_HOTCONNECT_GROUP* pHCGroup, EC_T_BOOL bPresent);
    EC_T_VOID               RefreshCfgSlavePresence(CEcSlave* pCfgSlave);

    EC_T_DWORD              GetNumGroupMembers( EC_T_DWORD dwGroupIndex );
    EC_T_DWORD              GetSlaveIdsOfGroup(EC_T_DWORD dwGroupIndex, EC_T_DWORD* adwSlaveId, EC_T_DWORD dwMaxNumSlaveIds);

    EC_T_DWORD              StartTopologyChange(EC_T_VOID);
    EC_T_DWORD              StartManualContinue(EC_T_VOID);
    EC_T_DWORD              WaitForHCStateMachine(EC_T_VOID);
    EC_T_DWORD              ReleaseHCStateMachine(EC_T_VOID);

    EC_T_BOOL               TopologyChangeInProgress(EC_T_VOID)
                                        { return (m_eCurState != m_eReqState); }
    EC_T_BOOL               WaitingForManualContinue(EC_T_VOID)
                                        { return m_bWaitingForManualContinue; }

    EC_T_DWORD              SetMode(EC_T_EHOTCONNECTMODE    oMode);
    EC_T_EHOTCONNECTMODE    GetMode(EC_T_VOID)
                                        { return m_oHCMode; }

    static EC_T_CHAR*       EcHlSmStateString( ECHC_T_ESTATE EHcState );
    static EC_T_CHAR*       MasterStateString( EC_T_WORD  wMasterState );

#if (defined INCLUDE_PORT_OPERATION)
    EC_T_WORD               CreatePortChangeFlags(EC_T_BOOL bClose, EC_T_BOOL bForce, EC_T_BYTE bySource);
    EC_T_DWORD              QueueChangePortRequest(EC_T_WORD wSlaveAddr, EC_T_WORD wPort, EC_T_WORD wFlags);
    EC_T_DWORD              StartChangePort(EC_T_DWORD dwTimeout);
    EC_T_DWORD              PollChangePort(EC_T_VOID);
    EC_T_DWORD              QueuePortsBySource(EC_T_BYTE bySource);
    EC_T_DWORD              OpenPortsBySource(EC_T_BYTE bySource, EC_T_DWORD dwTimeout);
    EC_T_DWORD              CountClosedPortsBySource(EC_T_BYTE bySource);
    EC_T_BOOL               IsBorderCloseActive(EC_T_VOID)
                                        { return ((echm_borderclose == (m_oHCMode&echm_borderclose))&&(1<m_dwHotConGroupListLen)); }
    EC_T_BOOL               IsPortClosed(EC_T_WORD wSlaveAddr, EC_T_WORD wPort);

    static EC_T_DWORD       s_dwClosePortReserved;
    static EC_T_DWORD       ClosePortAtMainLine(EC_T_PVOID pCallerData, EC_T_DWORD* pdwReserved);
	static EC_T_DWORD       OpenPortAtMainLine(EC_T_PVOID pCallerData, EC_T_DWORD* pdwReserved);
#if (defined INCLUDE_RED_DEVICE)
    static EC_T_DWORD       ClosePortAtRedLine(EC_T_PVOID pCallerData, EC_T_DWORD* pdwReserved);
	static EC_T_DWORD       OpenPortAtRedLine(EC_T_PVOID pCallerData, EC_T_DWORD* pdwReserved);
#endif
#endif

    struct _EC_T_MEMORY_LOGGER*     GetMemLog();

protected:
	
private:
    EC_T_BOOL               TopologyChangedWhileHC(EC_T_VOID);

    EC_T_DWORD              m_dwHotConGroupListLen; /* Length of m_pHotConnectGroupList */
    EC_T_HOTCONNECT_GROUP*  m_pHotConnectGroupList; /* ArrayList Holding HotConnect Groups */

    EC_T_DWORD              m_dwCmdDescListLen;     /* Length of m_apCmdDescList */
    EC_T_DWORD              m_dwCmdsInList;         /* Cmds in List */
    EC_T_VOID**             m_apCmdDescList;        /* ArrayList Holding Cyclic Datagrams */

    CEcMaster*              m_pMaster;
    CEcDevice*              m_poEcDevice;

    EC_T_DWORD              m_dwOutStandingPOCalls; /* count for outstanding cmds for port-op */

    EC_T_DWORD              m_dwHCCurGroupIdx;      /* Storage for current group Index */
    
    /* list of ports, the list contains only nodes currently present on the bus */
    EC_T_HCPORTLISTPORTOP   m_oClosedPorts;              /* List of all manually closed ports */
    EC_T_HCPORTLISTPORTOP   m_oPortsToChange;            /* List of all currently changing ports */
    EC_T_DWORD              m_bPortOperationsFailed;     /* EC_TRUE: at least on port operations has failed */
    EC_T_DWORD              m_dwNumSucceedPortOperations; /* number successful executed port operations */

    /* topology changed while HC management */
    EC_T_DWORD              m_dwSlavesAtStart;
    EC_T_BOOL               m_bLinkConnectedAtStart;

    EC_T_EHOTCONNECTMODE    m_oHCMode;              /* Mode of HC Operation */
    EC_T_BOOL               m_bWaitingForManualContinue; /* EC_TRUE if waiting for manual continue */

    EC_T_BOOL               m_bSMRestart;           /* Restart desired, persistent */
    EC_T_BOOL               m_bSMTimeout;           /* Timeout occured */
    EC_T_DWORD              m_dwHCSmResult;         /* result value of hot connect the state machine */

    /* state machine states */
public:
    typedef enum _ECHCSMC_T_CMDSTATES
    {
        echcsmc_idle                ,
        echcsmc_pending             ,
        echcsmc_retry               ,
        echcsmc_error               ,
            
        echcsmc_BCppDummy           = 0xFFFFFFFF
    } ECHCSMC_T_CMDSTATES;

    EC_T_VOID HCStateMachine(EC_T_VOID);

    typedef enum _ECHCSM_T_ESTATEMACHINE
    {
        echcsm_unknown              ,
        echcsm_idle                 ,
        echcsm_start                ,
        echcsm_restart              ,
            
        echcsm_BusScan              ,
        echcsm_BusScan_wait         ,
        echcsm_BusScan_done         ,

        echcsm_GroupInitState       ,
        echcsm_GroupInitState_wait  ,
        echcsm_GroupInitState_done  ,

        echcsm_GroupPreopState      ,
        echcsm_GroupPreopState_wait ,
        echcsm_GroupPreopState_done ,

        echcsm_DCActivation         ,
        echcsm_DCActivation_wait    ,
        echcsm_DCActivation_done    ,

        echcsm_GroupAdded           ,

        echcsm_GroupMasterState     ,
        echcsm_GroupMasterState_wait,
        echcsm_GroupMasterState_done,

        echcsm_TopoChangeManual     ,
        echcsm_TopoChangeManualContinue,
        echcsm_TopoChangeAutomatic  ,

        echcsm_reqto_wait           ,
        echcsm_stuck                ,
            
        echcsm_BCppDummy            = 0xFFFFFFFF
    } ECHCSM_T_ESTATEMACHINE;
    EC_T_CHAR* EHCSTATESText(ECHCSM_T_ESTATEMACHINE eState);
#if (defined INCLUDE_HOTCONNECT)
    EC_T_VOID  HCSmHandleGroupAddedNotification(EC_T_VOID);
#endif
    typedef struct _ECHCSM_REQ
    {
        ECHCSM_T_ESTATEMACHINE  eState;
        EC_T_DWORD              dwResult;
        EC_T_BOOL               bUsed;
    } ECHCSM_REQ;

    EC_T_DWORD  RequestState(       ECHCSM_T_ESTATEMACHINE  eState
                                   ,ECHCSM_REQ**            pHandle         );
    EC_T_DWORD  ReleaseReq(         ECHCSM_REQ*             pHandle         );

#if (defined INCLUDE_PORT_OPERATION)
    EC_T_VOID HCPortOperationStateMachine(EC_T_VOID);

    typedef enum _ECHCSM_T_EPORTOPERATION
    {
        echcsmpo_unknown            ,
        echcsmpo_idle               ,
        echcsmpo_start              ,

        echcsmpo_readdlcontrol      ,
        echcsmpo_readdlcontrol_wait ,
        echcsmpo_readdlcontrol_done ,

        echcsmpo_writedlctrl        ,
        echcsmpo_writedlctrl_wait   ,
        echcsmpo_writedlctrl_done   ,

        echcsmpo_checkdlctrl        ,
        echcsmpo_checkdlctrl_wait   ,
        echcsmpo_checkdlctrl_done   ,

        echcsmpo_BTEnhanceScan      ,
        echcsmpo_BTEnhanceScan_wait ,
        echcsmpo_BTEnhanceScan_done ,

        echcsmpo_portoperation      ,

        echcsmpo_reqto              ,
        echcsmpo_reqto_wait         ,
        echcsmpo_stuck              ,

        echcsmpo_BCppDummy          = 0xFFFFFFFF
    } ECHCSM_T_EPORTOPERATION;
    EC_T_CHAR* EHCPOSTATESText(ECHCSM_T_EPORTOPERATION eState);

    typedef struct _ECHCSMPO_REQ
    {
        ECHCSM_T_EPORTOPERATION eState;
        CEcTimer                oTimeout;
        EC_T_DWORD              dwResult;
        EC_T_BOOL               bUsed;
    } ECHCSMPO_REQ;
    
    EC_T_DWORD  RequestPortOpState( ECHCSM_T_EPORTOPERATION eState          
                                   ,EC_T_DWORD              dwTimeout       
                                   ,ECHCSMPO_REQ**          pHandle         );
    EC_T_DWORD  PollPortOpState(    ECHCSMPO_REQ*           pHandle         );
    EC_T_DWORD  ReleasePortOpReq(   ECHCSMPO_REQ*           pHandle         );
#endif /* INCLUDE_PORT_OPERATION */

private:
    /* Request buffers */
    ECHCSM_REQ              m_oRequest[2];
    /* Queued Request */
    ECHCSM_REQ*             m_pRequest;
    /* Current Running Request */
    ECHCSM_REQ*             m_pCurRequest;
         
    /* Current State */
    ECHCSM_T_ESTATEMACHINE  m_eCurState;
    /* Requested State */
    ECHCSM_T_ESTATEMACHINE  m_eReqState;

    CEcTimer                m_oTimeout;

    /* Handle to current running HC operation */
    ECHCSM_REQ*             m_pCurrentHCReq;

#if (defined INCLUDE_PORT_OPERATION)
    /* Request buffers */
    ECHCSMPO_REQ            m_oPortOpRequest[2];
    /* Queued Request */
    ECHCSMPO_REQ*           m_pPortOpRequest;
    /* Current Running Request */
    ECHCSMPO_REQ*           m_pCurPortOpRequest;

    /* Current State */
    ECHCSM_T_EPORTOPERATION m_eCurPortOpState;
    /* Requested State */
    ECHCSM_T_EPORTOPERATION m_eReqPortOpState;
    
    /* Handle to current running Port operation */
    ECHCSMPO_REQ*           m_pCurrentHCPortOpReq;

    /* State of current command(s) */
    ECHCSMC_T_CMDSTATES     m_eCurPOCmdState;
#endif /* INCLUDE_PORT_OPERATION */
};

#endif /* INC_ECHOTCONNECT */ 
