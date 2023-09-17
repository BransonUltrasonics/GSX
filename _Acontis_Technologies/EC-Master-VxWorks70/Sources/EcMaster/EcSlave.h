/*-----------------------------------------------------------------------------
 * EcSlave.h                header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              
 *---------------------------------------------------------------------------*/


#ifndef INC_ECSLAVE
#define INC_ECSLAVE

/*-INCLUDES------------------------------------------------------------------*/

/*-DEFINES/MACROS------------------------------------------------------------*/
#define REGULAR_MAILBOX               0
#define BOOT_MAILBOX                  1

#define IGNORE_PREV_PORT_FLAG       ((EC_T_WORD)(0x8000))

#ifndef EC_MBX_RETRYACCESS_COUNT
#define EC_MBX_RETRYACCESS_COUNT    100
#endif
#define EC_MBX_RETRYACCESS_PERIOD    25     /* Period to retry mailbox access e.g. frameloss wkc error ... */

/*-TYPEDEFS------------------------------------------------------------------*/
typedef enum _iecs_t_eMboxRequestState
{
    iecs_mbx_idle   = 0x0,
    iecs_mbx_poll   = 0x1,
    iecs_mbx_read   = 0x2,
        
    iecs_BCppDummy  = 0xffffffff    
} IECS_T_EMBXREQUESTSTATE;

EC_T_CHAR*  GetStateChangeName(EC_T_WORD state);
class CEcMaster;

#ifndef INC_ECBUSTOPOLOGY
#include <EcBusTopology.h>
#endif

/********************************************************************************/
/** \brief EtherCAT Slave class. 
 */
class CEcSlave;
typedef CList<class CEcSlave*, class CEcSlave*> CEcSlaveList;
typedef CEcSlaveList::CNode* CEcSlaveListNode;
class CEcSlave
{
public: 
    CEcSlave(CEcMaster* pMaster, EC_T_ENI_SLAVE* pEniSlave, CEcDevice* poEcDevice);
#if (defined INSTRUMENT_MASTER)
    CEcSlave() {}
#endif
    virtual ~CEcSlave();    

    /********************************************************************************/
    /*                  InitCmds                                                    */
    virtual EC_T_DWORD  AddCoeInitCmds(EC_T_ADD_COE_INITCMD_DESC_ENTRY* pbCoeInitCmds, EC_T_WORD wCount);
    virtual EC_T_BOOL   StartInitCmds(EC_T_WORD transition, EC_T_BOOL bRetry);
    virtual EC_T_BOOL   AreInitCmdsActive(EC_T_VOID)
                        { return (m_wInitCmdsCurrent != INITCMD_INACTIVE) || (m_wMasterInitCmdsCurrent != INITCMD_INACTIVE); }
    virtual EC_T_BOOL   AreAllInitCmdsProcessed(EC_T_VOID)
                        { return (m_wInitCmdsCurrent >= m_wInitCmdsCount) && (m_wMasterInitCmdsCurrent >= m_pMaster->m_nInitCmdsCount); }
            EC_T_BOOL   AreInitCmdsPending(EC_T_VOID)
                        { return (AreInitCmdsActive() && !AreAllInitCmdsProcessed());}
    EcInitCmdDesc*      GetInitCmdsCurrent(EC_T_VOID)
                        {
                            if (m_wMasterInitCmdsCurrent < m_pMaster->m_nInitCmdsCount)
                            {
                                return m_pMaster->m_ppInitCmds[m_wMasterInitCmdsCurrent];
                            }
                            if (m_wInitCmdsCurrent < m_wInitCmdsCount)
                            {
                                return m_ppInitCmds[m_wInitCmdsCurrent];
                            }
                            OsDbgAssert(EC_FALSE);
                            return EC_NULL;
                        }

    virtual EC_T_VOID   StopInitCmds(EC_T_WORD wTransition, EC_T_WORD wCurrTransition);

    EC_T_BOOL           ConvertMasterInitCmdToSlaveInitCmd(EcInitCmdDesc* pInitCmdDesc);

	/********************************************************************************/
	/** \brief This method is called cyclically by eUsrJob_MasterTimer
	* \param cycle Current cycle tick
	*/
	virtual EC_T_VOID   OnTimer(EC_T_DWORD dwCurMsecCounter) 
                        { EC_UNREFPARM(dwCurMsecCounter); }
    
    /********************************************************************************/
    /*                  Slave State Machine                                         */
    /** \brief Request slave state machine to change to the specified state */
    EC_T_VOID           RequestSmState(EC_T_STATE state);
    virtual EC_T_DWORD  SlaveStateMachine();
    virtual EC_T_VOID   ResetSlaveStateMachine(EC_T_VOID);
    virtual EC_T_VOID   StabilizeSlaveStateMachine(EC_T_VOID);
    virtual EC_T_VOID   CancelSlaveStateMachine(EC_T_VOID);
    EC_T_BOOL           TargetStateNotReachable(EC_T_VOID) 
                        {return m_bInitCmdFailedError;}
    EC_T_DWORD          SetDeviceState(EC_T_WORD wDeviceState);

    /********************************************************************************/
    /*                  Slave Presence / Identification / Hot Connect               */
    virtual EC_T_VOID   SetSlavePresence(EC_T_BOOL bPresent);
    EC_T_VOID           InvalidatePresence(EC_T_VOID) 
                        { m_bIsPresenceValid = EC_FALSE; }
    EC_T_BOOL           IsPresenceValid(EC_T_VOID) 
                        { return m_bIsPresenceValid; }
    EC_T_VOID           NotifyPresence(EC_T_VOID);
    EC_T_BOOL           IsPresentOnBus(EC_T_VOID)
                        { return (ePRESENT == m_ePresenceOnBus); }
    EC_T_BOOL           IsOptional(EC_T_VOID)
                        { return m_bIsOptional; }
    EC_T_BOOL           IsHCGroupHead(EC_T_VOID)
                        { return m_bIsGHCGroupHead; }
    EC_T_BOOL           IsHCGroupTail(EC_T_VOID)
                        { 
                            EC_T_HOTCONNECT_GROUP* pGroup = m_pMaster->m_oHotConnect.GetGroup(m_dwHCGroupId);
                            return ((EC_NULL != pGroup)
                                 && (EC_NULL != pGroup->ppGroupMembers)
                                 && (this    == pGroup->ppGroupMembers[pGroup->dwGrpMembers-1]));
                        }
    EC_T_DWORD          GetHCGroupId(EC_T_VOID)
                        { return m_dwHCGroupId; }
    EC_T_HOTCONNECT_GROUP* GetHCGroup(EC_T_VOID)
                        { return m_pMaster->m_oHotConnect.GetGroup(m_dwHCGroupId); }
    EC_T_BOOL           HasIdentificationCheck(EC_T_VOID) 
                        { return GetIdentification(EC_NULL, EC_NULL); }
    EC_T_BOOL           GetIdentification(EC_T_WORD* pwAdo, EC_T_WORD* pwValue);
    EC_T_VOID           SetIgnoreAbsence(EC_T_BOOL bIgnoreAbsence)
                        {
                            m_bIgnoreAbsence = bIgnoreAbsence;
                        }
    EC_T_BOOL           IgnoreAbsence(EC_T_VOID)
                        { return m_bIgnoreAbsence; }
    EC_T_STATE          GetEcatState(EC_T_VOID) {return (EC_T_STATE)((EC_NULL != m_pBusSlave)?(m_pBusSlave->GetAlStatus() & DEVICE_STATE_MASK):(eEcatState_UNKNOWN));}
    EC_T_STATE          GetSmReqEcatState(EC_T_VOID) { return (EC_T_STATE)((DEVICE_STATE_UNKNOWN == m_wReqStateMachDevState)?(eEcatState_UNKNOWN):(m_wReqStateMachDevState & DEVICE_STATE_MASK)); }
    EC_T_DWORD          SetMaxState(EC_T_STATE eMaxState)
                        {
                            m_eMaxState = eMaxState;
                            if (m_eMaxState < GetEcatState())
                            {
                                return SetDeviceState((EC_T_WORD)m_eMaxState);
                            }
                            else
                            {
                                return EC_E_NOERROR;
                            }
                        }
    EC_T_STATE          GetMaxState(EC_T_VOID)
                        { return m_eMaxState; }

    /********************************************************************************/
    /** \brief Processes the response of an EtherCAT Cmd.
    *
    * \param result     Result of pending QueueEcatCmdReq
    * \param invokeId   InvokeId of the request
    * \param pHead      Actual response data    
    *
    * \return
    */
    virtual EC_T_VOID   ProcessCmdResult(EC_T_DWORD result, EC_T_DWORD invokeId, PETYPE_EC_CMD_HEADER pEcCmdHeader);

    /********************************************************************************/
    /** \brief Determines if the EtherCAT slave has an mailbox.
    *
    * \return TRUE if the slave has a mailbox otherwise EC_FALSE.
    */
    virtual EC_T_BOOL   HasMailBox()
                        { return EC_FALSE; }

    virtual EC_T_DWORD  GetThisSize() const { return sizeof(CEcSlave); }
    
    /********************************************************************************/
    /** \brief Gets the name of the EtherCAT slave.
    *
    * \return The name of the EtherCAT slave.
    */
    EC_INSTRUMENT_MOCKABLE_FUNC const EC_T_CHAR* GetName( EC_T_VOID );

    /********************************************************************************/
    /** \brief Gets the auto-increment address of the EtherCAT slave.
    *
    * \return The auto-increment address of the EtherCAT slave.
    */
    EC_T_WORD           GetAutoIncAddress( EC_T_VOID ) 
                        { return m_wAutoIncAddr; }
    
    /********************************************************************************/
    /** \brief Gets the configured auto-increment address of the EtherCAT slave.
    *
    * \return The configured auto-increment address of the EtherCAT slave.
    */
    EC_T_WORD           GetCfgAutoIncAddress(EC_T_VOID) 
                        { return m_pEniSlave->wAutoIncAddr; }

    /********************************************************************************/
    /** \brief Gets the physics of the EtherCAT slave.
    *
    * \return The Physics of the EtherCAT slave.
    */
    const EC_T_CHAR*    GetPhysics(EC_T_VOID) 
                        { return m_pEniSlave->szPhysics; }
    EC_T_BYTE           GetPhysicsDesc(EC_T_VOID);
                                                              
    /********************************************************************************/
    /** \brief Gets the physical address of the EtherCAT slave.
    *
    * \return Physical address of the EtherCAT slave.
    */
    EC_T_WORD           GetFixedAddr() 
                        { return m_wFixedAddr; }

    /********************************************************************************/
    /** \brief Sets the physical address of the EtherCAT slave.
    *
    */
    EC_T_VOID           SetFixedAddr(EC_T_WORD wAddr)
                        { m_wFixedAddr = wAddr; }

    /********************************************************************************/
    /** \brief Gets the configured physical address of the EtherCAT slave.
    *
    * \return Physical address of the EtherCAT slave.
    */
    EC_INSTRUMENT_MOCKABLE_FUNC EC_T_WORD GetCfgFixedAddr(EC_T_VOID) 
                        { return m_pEniSlave->wPhysAddr; }

    EC_T_VOID           GetSlaveProp(EC_T_SLAVE_PROP* pSlaveProp );
    EC_T_DWORD          GetSlaveId(EC_T_VOID)
                        { return m_dwSlaveId; }
    EC_T_VOID           SetSlaveId(EC_T_DWORD dwSlaveId)
                        { m_dwSlaveId = dwSlaveId; }

    EC_T_DWORD          EcTransferRawCmd( EC_T_DWORD dwUniqueClntId, EC_T_WORD wInvokeId, EC_T_BYTE byCmd, EC_T_DWORD dwMemoryAddress, 
                                         EC_T_VOID* pvData, EC_T_WORD wLen, EC_T_DWORD dwTimeout );
    EC_T_BOOL           SlaveStateMachIsStable( EC_T_VOID ) { return m_bStateMachineStable; }
    EC_T_DWORD          GetVendorId(EC_T_VOID){ return m_pEniSlave->dwVendorId; }
    EC_T_DWORD          GetProductCode(EC_T_VOID){ return m_pEniSlave->dwProductCode; }
    EC_T_DWORD          GetRevisionNumber(EC_T_VOID){ return m_pEniSlave->dwRevisionNumber; }
    EC_T_DWORD          GetSerialNumber(EC_T_VOID){ return m_pEniSlave->dwSerialNumber; }
    EC_T_DWORD          GetConfigValue(EC_T_WORD wAddr, EC_T_DWORD& dwVal);
    EC_T_DWORD          GetSlaveInfo(EC_T_eINFOENTRY eEntry, EC_T_BYTE* pbyData, EC_T_DWORD* pdwLen);
    EC_T_DWORD          GetSlaveInfoAll(EC_T_GET_SLAVE_INFO* pSlaveInfo);
    EC_T_VOID           GetCfgSlaveInfo(EC_T_CFG_SLAVE_INFO* pSlaveInfo);

    EC_T_VOID           LinkMessages(EC_T_BOOL bEnable) { m_bEnableLinkMessages = bEnable; }
    EC_T_BOOL           GetLinkMessagesEnabled(EC_T_VOID) { return m_bEnableLinkMessages; }

#if (defined INCLUDE_DC_SUPPORT)
    EC_T_DWORD          GetDcRegisterCycleTime0(EC_T_VOID){  return m_pEniSlave->dwDcRegisterCycleTime0; }
    EC_T_DWORD          GetDcRegisterCycleTime1(EC_T_VOID){  return m_pEniSlave->dwDcRegisterCycleTime1; }
    EC_T_BOOL           GetDcSendSyncActivation(EC_T_VOID){ return m_bDcSendSyncActivation; }
    EC_T_BOOL           GetDclSendConfiguration(EC_T_VOID){ return m_bDclSendConfiguration; }
    EC_T_BOOL           GetDcInitializationRequired(EC_T_VOID) { return m_bDcInitializationRequired; }
    EC_T_VOID           SetDcInitializationRequired(EC_T_BOOL bDcInitializationRequired) { m_bDcInitializationRequired = bDcInitializationRequired; }
    EC_T_BOOL           IsDcInitializationRequired(EC_T_VOID) { return GetDcSendSyncActivation() || GetDclSendConfiguration() || GetDcInitializationRequired(); }
    EC_T_BOOL           IsDcPotentialRefClock(EC_T_VOID) { return m_pEniSlave->bDcPotentialRefClock; }
#endif /* INCLUDE_DC_SUPPORT */

#if (defined INCLUDE_SLAVE_STATISTICS)
    EC_T_DWORD          GetStatisticCounters(EC_T_SLVSTATISTICS_DESC* pStatistics);
    EC_T_VOID           ClearStatisticCounters(EC_T_VOID);
#endif

    EC_T_WORD           GetAlStatus(EC_T_VOID)      {return (EC_T_WORD)((EC_NULL != m_pBusSlave)?(m_pBusSlave->GetAlStatus()):(DEVICE_STATE_UNKNOWN));}
    EC_T_WORD           GetAlStatusCode(EC_T_VOID)  {return (EC_T_WORD)((EC_NULL != m_pBusSlave)?(m_pBusSlave->GetAlStatusCode()):(0));}
    EC_T_WORD           GetDeviceState(EC_T_VOID)   {return (EC_T_WORD)((EC_NULL != m_pBusSlave)?(m_pBusSlave->GetAlStatus() & DEVICE_STATE_MASK):(DEVICE_STATE_UNKNOWN));}
    EC_T_WORD           GetSmDeviceState(EC_T_VOID)    { return m_wCurrStateMachDevState; }
    EC_T_WORD           GetSmReqDeviceState(EC_T_VOID) { return m_wReqStateMachDevState; }
    
    EC_T_DWORD          GetNumPreviousPorts(EC_T_VOID) { return m_pEniSlave->wNumPreviousPorts; }
    EC_T_CHECKEEPROMENTRY GetCheckEEPromEntryCriterion(EC_T_DWORD dwEEPromEntryIdx) { return m_pEniSlave->aeCheckEEPROMEntry[dwEEPromEntryIdx]; }

    struct _EC_T_MEMORY_LOGGER* GetMemLog() { return ((EC_NULL != m_pMaster) ? m_pMaster->GetMemLog() : EC_NULL); }

    virtual EC_T_BOOL   HasPendingRequests(EC_T_VOID) { return AreInitCmdsPending() || m_bRawTferPending; }
#if (defined INCLUDE_CONFIG_EXTEND)
    EC_T_VOID           SetRemoveOnTimer(EC_T_BOOL bRemove) { m_bRemoveOnTimer = bRemove; }
    EC_T_BOOL           ShallRemoveOnTimer(EC_T_VOID) { return m_bRemoveOnTimer; }
#endif

private:
    EC_T_VOID           SetSmDeviceState(EC_T_WORD wCurrStateMachDevState);

public:
    CEcMaster*                  m_pMaster;
    CEcDevice*                  m_poEcDevice;

    CEcBusSlave*                m_pBusSlave;
    CEcBusSlave*                m_pBusSlaveCandidate;

    EC_T_VOID*                  m_poSlaveLock;              /* mutex object for slave object synchronization */

    EC_T_ENI_SLAVE*             m_pEniSlave;                /* eni slave */
    
    EC_T_DWORD                  m_dwSlaveId;
    EC_T_WORD                   m_wFixedAddr;               /* fixed station address */
    
    EC_T_WORD                   m_wAutoIncAddr;             /* auto increment address */

    EC_T_BOOL                   m_bDeviceEmulation;         /* is Device Emulation slave (information from bus) */

    EC_T_WORD                   m_wAlStatusNotified;                /* last notified AL_STATUS */
    EC_T_WORD                   m_wUnexpStateCurAlStatusNotified;   /* last unexpected AL_STATUS notified (current AlStatus) */
    EC_T_WORD                   m_wUnexpStateExpAlStatusNotified;   /* last unexpected AL_STATUS notified (requested AlStatus) */

protected:
    EC_T_WORD                   m_wCurrStateMachDevState;   /* current state machine slave device state */
    EC_T_WORD                   m_wReqStateMachDevState;    /* requested state machine slave device state */
    
public:
    PEcInitCmdDesc*             m_ppInitCmds;
    EC_T_WORD                   m_wInitCmdsCount;
    EC_T_WORD                   m_wMaxNumInitCmds;

    CEcTimer                    m_oInitCmdsTimeout;
    CEcTimer                    m_oInitCmdRetryTimeout;

    EC_T_WORD                   m_wInitCmdsRetries;
    EC_T_WORD                   m_wInitCmdsCurrent;
    EC_T_WORD                   m_wMasterInitCmdsCurrent;

    EC_T_WORD                   m_wActTransition;

    EC_T_BOOL                   m_bStateMachineStable;      /* request for initcmd send */
    EC_T_BOOL                   m_bStandardInitCmdsFirst;   /* if slave state is below PREOP the standard initcmds are before the mbx initcmds */
    EC_T_BOOL                   m_bStandardInitCmdsDone;

    EC_T_BOOL                   m_bInitCmdFailedError;      /* EC_TRUE if init Cmd failed, requested state could not be reached */

    EC_T_DWORD                  m_bRawTferPending;  /* EC_TRUE if a raw transfer is pending */
    EC_T_DWORD                  m_dwRawTferStatus;  /* raw transfer status */
    
    EC_T_WORD                   m_wRawTferDataLen;  /* data length of a raw transfer (valid only for read requests) */

    EC_T_BYTE*                  m_pbyRawTferData;   /* raw transfer result data buffer */

    EC_T_CYC_CMD_WKC_DESC_ENTRY* m_aCycCmdWkcList;  /* array which is at longest as long as existing cyclic datagrams */
    EC_T_DWORD                  m_dwNumCycCmdWkc;
    EC_T_BOOL                   m_bCycErrorNotifyMasked;

#if (defined INCLUDE_WKCSTATE)
    EC_T_WORD                   m_wWkcStateInEntries;
    EC_T_WORD                   m_awWkcStateDiagOffsIn[EC_CFG_SLAVE_PD_SECTIONS];
    EC_T_WORD                   m_wWkcStateOutEntries;
    EC_T_WORD                   m_awWkcStateDiagOffsOut[EC_CFG_SLAVE_PD_SECTIONS];
#endif /* INCLUDE_WKCSTATE */
    EC_T_BOOL                   m_bEnableLinkMessages;

    EC_T_SLAVE_PORT_DESC*       m_pPreviousPort;

    enum
    {        
        eUNKNOWN = 0,
        eABSENT  = 1,
        ePRESENT = 2
    }                           m_ePresenceOnBus,
                                m_ePresenceNotified;
    EC_T_BOOL                   m_bIsPresenceValid;
    EC_T_BOOL                   m_bIgnoreAbsence;
    EC_T_STATE                  m_eMaxState;

    EC_T_BOOL                   m_bIsOptional;
    EC_T_BOOL                   m_bIsGHCGroupHead;
    EC_T_DWORD                  m_dwHCGroupId;              /* 0: mandatory */

    /* topology validation */
    CEcSlaveList                m_aoPossiblePortChildrenList[ESC_PORT_COUNT];
    CEcSlaveList                m_oPossibleBusChildrenList;                   /* needed for Superset-ENI */

protected:
    EC_T_WORD                   m_wSyncManagerEnabled;
    EC_T_BOOL                   m_bCancelStateMachine;

private:
    EC_T_DWORD QueueEcatInitCmd(EC_T_DWORD dwInvokeId, EC_T_BYTE byCmd, EC_T_WORD wAdp, EC_T_WORD wAdo, EC_T_WORD wLen, EC_T_BYTE* abyData, EC_T_WORD wCurrTransition);
    EC_T_DWORD ALStatusInterruptEnable(EC_T_BOOL bEnable);

#if (defined INCLUDE_DC_SUPPORT)
    EC_T_BOOL                   m_bDcSendSyncActivation;
    EC_T_BOOL                   m_bDclSendConfiguration;
    EC_T_BOOL                   m_bDcInitializationRequired; 
#endif /* INCLUDE_DC_SUPPORT */

#if (defined INCLUDE_SLAVE_STATISTICS)
    EC_T_WORD                   m_wRxErrCounter[4];     /* 0x300:0x307 */
    EC_T_BYTE                   m_byFwdRxErrCounter[4]; /* 0x308:0x30B */
    EC_T_BYTE                   m_byProcUnitErrCounter; /* 0x30C */
    EC_T_BYTE                   m_byPDIErrCounter;      /* 0x30D */
    EC_T_BYTE                   m_byLostLinkCounter[4]; /* 0x30E:0x313 */ 
#endif

#if (defined INCLUDE_CONFIG_EXTEND)
    EC_T_BOOL                   m_bRemoveOnTimer;
#endif
};


/********************************************************************************/
typedef struct _MBXPROTOCOL_DESC
{
    CFiFoListDyn<PEcMailboxCmd>*    pCmdFifo;
    struct _cur
    {
        EC_T_BOOL                   bIsValid;
        EC_T_BOOL                   bSendEnabled;
        EC_T_BOOL                   bRetry;
        EC_T_WORD                   wRetryCounter;
        PEcMailboxCmd               pCmd;
        EC_T_DWORD                  dwOffset;
        EC_T_DWORD                  dwRetData;
    } cur;
} MBXPROTOCOL_DESC;

/********************************************************************************/
typedef struct TECMBSLAVE_COE_CMD_INFO
{
    PEcMailboxCmd   pCmd;
    PEcMailboxCmd   pRet;
    EC_T_WORD           timeout;
} ECMBSLAVE_COE_CMD_INFO, *PECMBSLAVE_COE_CMD_INFO;

typedef struct TECMBSLAVE_COE_DESC
{
    CFiFoListDyn<PEcMailboxCmd>*    pPendMailboxCmdFifo;    /* FIFO containing pending mailbox Cmds */

    struct _CoeMbxDesc
    {
        EC_T_BOOL                   bIsValid;
        EC_T_BOOL                   bSendEnabled;           /* flag: send this Cmd */
        EC_T_BOOL                   bIsInitCmd;
        EC_T_DWORD                  dwSdoAbortErrorCode;    /* error code that initiated abort */
        EC_T_DWORD                  dwRetryCounter;
        EC_SDO_HDR                  EcSdoHeader;
        ETHERCAT_MBOX_HEADER        EcMbxHeader;            /* currently pending mbx header */
        ETHERCAT_CANOPEN_HEADER     EcCanHeader;            /* currently pending can header */   
        PEcMailboxCmd               pCurrPendQueuedMbxCmd;  /* currently pending mailbox Cmd (queued into acyclic queue) */
        EC_T_DWORD                  nRetData;
        EC_T_DWORD                  nOffset;
        EC_T_BOOL                   bFramePending;          /* EC_TRUE if Frame was queued instead of sending. */
        EC_T_BOOL                   bFramePendingData;      /* EC_TRUE if Frame was queued instead of sending and segmented data available. */
        EC_T_BOOL                   bCoeWkcRetry;           /* flag: if set, Coe mbx protocol is in WKC retry status */
    } CurrPendCoeMbxCmdDesc;
} ECMBSLAVE_COE_DESC, *PECMBSLAVE_COE_DESC;

#if (defined INCLUDE_EOE_SUPPORT)
/********************************************************************************/
typedef struct TECMAILBOX_SWITCHPORT
{
    struct
    {
        EC_T_BYTE*      pData;
        EC_T_DWORD      nData;
        EC_T_DWORD      nOffs;
    }   pendSend;
    struct
    {
        EC_T_BYTE*      pData;
        EC_T_DWORD      nData;
        EC_T_DWORD      nMax;
    }   pendRecv;
    EC_T_DWORD          nSendFrameNo;
    EC_T_DWORD          nMaxSendFragment;
    CEthernetPort*      pPort;
} ECMAILBOX_SWITCHPORT, *PECMAILBOX_SWITCHPORT;

typedef struct TECMBSLAVE_EOE_DESC
{
    struct _T_pendSend
    {
        EC_T_BYTE*  pData;
        EC_T_WORD   nData;
        EC_T_WORD   nOffs;
        EC_T_WORD   nSend;
    }   pendSend;
    struct _T_pendRecv
    {
        EC_T_BYTE*  pData;
        EC_T_WORD   nData;
        EC_T_WORD   nMax;
    }   pendRecv;
    EC_T_WORD       nSendFrameNo;
    EC_T_WORD       nSendFragNo;
    EC_T_WORD       nMaxSendFragment;
    EC_T_WORD       nLastFragNo;
    EC_T_WORD       retry;
    EC_T_BOOL       bIsInitCmd;
    EC_T_BOOL       bRetrySendMbx;
    EC_T_BOOL       bSendInitCmdReq;
    CEthernetPort*  pPort;
    CEcTimer  oRetryTimeout;
} ECMBSLAVE_EOE_DESC, *PECMBSLAVE_EOE_DESC;
#endif /* INCLUDE_EOE_SUPPORT */

#if (defined INCLUDE_FOE_SUPPORT)
typedef struct TECMBSLAVE_FOE_DESC
{
    CFiFoListDyn<PEcMailboxCmd>*    pPendMailboxCmdFifo;    /* FIFO containing pending mailbox Cmds */
    struct _FoeMbxDesc
    {
        EC_T_DWORD                  bIsValid            : 1;        /* flag: handling Cmd from Fifo */
        EC_T_DWORD                  bNewCmd             : 1;
        EC_T_DWORD                  bRead               : 1;
        EC_T_DWORD                  bDataPend           : 1;        /* flag: req Cmd inserted into frame, awaiting response */
        EC_T_DWORD                  bBusy               : 1;
        EC_T_DWORD                  bLastRead           : 1;
        EC_T_DWORD                  bSendEnabled        : 1;        /* flag: send this Cmd */
        EC_FOE_HDR                  EcFoeHeader;                    /* currently pending foe header */
        ETHERCAT_MBOX_HEADER        EcMbxHeader;                    /* currently pending mbx header */
        PEcMailboxCmd               pCurrPendQueuedMbxCmd;          /* currently pending mailbox Cmd (queued into acyclic queue) */
        EC_T_DWORD                  nRetData;
        EC_T_DWORD                  nOffset;
        EC_T_DWORD                  dwLastPacketNumber;             /* last packet number which was received */
        EC_T_DWORD                  dwLastProcessedPacketNumber;    /* last packet number which was processed */
        EC_T_DWORD                  dwRetryCounter;
        EC_T_BOOL                   bFramePending;                  /* EC_TRUE if Frame was queued instead of sending. */
        EC_T_BOOL                   bFoeWkcRetry;                   /* flag: if set, Foe mbx protocol is in WKC retry status */
    } CurrPendFoeMbxCmdDesc;
} ECMBSLAVE_FOE_DESC, *PECMBSLAVE_FOE_DESC;
#endif /* INCLUDE_FOE_SUPPORT */

#if (defined INCLUDE_SOE_SUPPORT)
/********************************************************************************/
typedef struct TECMBSLAVE_SOE_CMD_INFO
{
    PEcMailboxCmd   pCmd;
    PEcMailboxCmd   pRet;
    EC_T_WORD           timeout;
} ECMBSLAVE_SOE_CMD_INFO, *PECMBSLAVE_SOE_CMD_INFO;

typedef struct TECMBSLAVE_SOE_DESC
{
    CFiFoListDyn<PEcMailboxCmd>*    pPendMailboxCmdFifo;    /* FIFO containing pending mailbox Cmds */
    struct _SoeMbxDesc
    {
        EC_T_BOOL                   bIsValid;
        EC_SOE_HDR         EcSoeHeader;
        PEcMailboxCmd               pCurrPendQueuedMbxCmd;  /* currently pending mailbox Cmd (queued into acyclic queue) */
        EC_T_DWORD                  nRetData;
        EC_T_DWORD                  nOffset;
        EC_T_WORD                   retry;
        EC_T_WORD                   wFragmentsLeft;
        EC_T_BOOL                   bSendEnabled;
        EC_T_BOOL                   bSoeWkcRetry;         /* flag: if set, Soe mbx protocol is in WKC retry status */
    } CurrPendSoeMbxCmdDesc;
} ECMBSLAVE_SOE_DESC, *PECMBSLAVE_SOE_DESC;
#endif /* INCLUDE_SOE_SUPPORT */

#if (defined INCLUDE_VOE_SUPPORT)
/********************************************************************************/
typedef struct TECMBSLAVE_VOE_CMD_INFO
{
    PEcMailboxCmd   pCmd;
    PEcMailboxCmd   pRet;
    EC_T_WORD           timeout;
} ECMBSLAVE_VOE_CMD_INFO, *PECMBSLAVE_VOE_CMD_INFO;

typedef struct TECMBSLAVE_VOE_DESC
{
    CFiFoListDyn<PEcMailboxCmd>*    pPendMailboxCmdFifo;    /* FIFO containing pending mailbox Cmds */
    struct _VoeMbxDesc
    {
        EC_T_BOOL                   bIsValid;
        PEcMailboxCmd               pCurrPendQueuedMbxCmd;  /* currently pending mailbox Cmd (queued into acyclic queue) */
        EC_T_DWORD                  nRetData;
        EC_T_WORD                   dwRetryCounter;
        EC_T_BOOL                   bSendEnabled;
        EC_T_BOOL                   bVoeWkcRetry;         /* flag: if set, Voe mbx protocol is in WKC retry status */
    } CurrPendVoeMbxCmdDesc;
} ECMBSLAVE_VOE_DESC, *PECMBSLAVE_VOE_DESC;
#endif /* INCLUDE_VOE_SUPPORT */

#if (defined INCLUDE_AOE_SUPPORT)
/********************************************************************************/
typedef struct TECMBSLAVE_AOE_DESC
{
    CFiFoListDyn<PEcMailboxCmd>*    pPendMailboxCmdFifo;     /* FIFO containing pending mailbox Cmds */
    // CFiFoListDyn<PEcMailboxCmd>*    pCurPendMailboxCmdFifo;  /* FIFO containing currently pending mailbox Cmds */
    struct _AoeMbxDesc
    {
        EC_T_BOOL                   bIsValid;
        EC_AOE_HDR                  oEcAoeHeader;
        EC_AOE_CMD_HDR              oEcAoeCmdHeader;
        PEcMailboxCmd               pCurrPendQueuedMbxCmd;  /* currently pending mailbox Cmd (queued into acyclic queue) */
        EC_T_DWORD                  nRetData;
        EC_T_DWORD                  nOffset;
        EC_T_WORD                   wRetryCounter;
        EC_T_WORD                   wFragmentsLeft;
        EC_T_BOOL                   bSendEnabled;
        EC_T_BOOL                   bAoeWkcRetry;         /* flag: if set, Aoe mbx protocol is in WKC retry status */
    } CurrPendAoeMbxCmdDesc;
    EC_T_AOE_NETID                  oAoeNetId;
    EC_T_DWORD                      dwAoeInvokeId;
} ECMBSLAVE_AOE_DESC, *PECMBSLAVE_AOE_DESC;
#endif /* INCLUDE_AOE_SUPPORT */

#if (defined INCLUDE_RAWMBX_SUPPORT)
#define RAWMBX_FIXEDADDRESS (EC_T_WORD)63459
/********************************************************************************/
typedef struct TECMBSLAVE_RAWMBX_DESC
{
    MBXPROTOCOL_DESC    mbxDesc;

    /* simple implementation for simple instance mailbox gateway */
    /* multi instance would need ClientId table                  */
    EC_T_DWORD dwClientId;
} ECMBSLAVE_RAWMBX_DESC;
#endif

/********************************************************************************/
/** \brief EtherCAT mailbox slave class
*
* \return
*/
class CEcMbSlave : public CEcSlave
{
public:
    CEcMbSlave(     CEcMaster*              pMaster
                   ,EC_T_ENI_SLAVE*         pEniSlave
                   ,CEcDevice*              poEcDevice
                   ,EC_T_DWORD              dwMsecCounter       );
    virtual ~CEcMbSlave();

    EC_T_DWORD  SendMailboxCmd(PEcMailboxCmd pCmd);
    EC_T_DWORD  FoeSegmentedUploadStart(EC_T_MBXTFERP* pMbxTferPriv, EC_T_CHAR* szFileName, EC_T_DWORD dwFileNameLen, EC_T_DWORD dwFileSize, 
                                          EC_T_DWORD dwPassword);
    EC_T_VOID   FoeSegmentedUploadSetSegmentDone(EC_T_MBXTFERP* pMbxTferPriv);
    EC_T_VOID   FoeSegmentedUploadContinue(EC_T_MBXTFERP* pMbxTferPriv);
    EC_T_DWORD  FoeSegmentedDownloadStart(EC_T_MBXTFERP* pMbxTferPriv, EC_T_CHAR* szFileName, EC_T_DWORD dwFileNameLen, EC_T_DWORD dwFileSize, 
                                          EC_T_DWORD dwPassword);
    EC_T_VOID   FoeSegmentedDownloadSetSegmentDone(EC_T_MBXTFERP* pMbxTferPriv);
    EC_T_VOID   FoeSegmentedDownloadContinue(EC_T_MBXTFERP* pMbxTferPriv);

    virtual EC_T_DWORD  SlaveStateMachine();
    virtual EC_T_VOID   ResetSlaveStateMachine(EC_T_VOID);
    virtual EC_T_VOID   StabilizeSlaveStateMachine(EC_T_VOID);
    virtual EC_T_VOID   CancelSlaveStateMachine(EC_T_VOID);

    virtual EC_T_DWORD  AddCoeInitCmds(EC_T_ADD_COE_INITCMD_DESC_ENTRY* pbCoeInitCmds, EC_T_WORD wCount);

    virtual EC_T_BOOL   StartInitCmds(EC_T_WORD wTransition, EC_T_BOOL bRetry);
            EC_T_BOOL   AreMbxInitCmdsActive(EC_T_VOID)
                        { return (m_wMbxInitCmdsCurrent != INITCMD_INACTIVE) || (m_wCoeInitCmdsCurrent != INITCMD_INACTIVE);}
    virtual EC_T_BOOL   AreInitCmdsActive(EC_T_VOID)
                        { return CEcSlave::AreInitCmdsActive() || AreMbxInitCmdsActive(); }
            EC_T_BOOL   AreAllMbxInitCmdsProcessed(EC_T_VOID)
                        { return (m_wMbxInitCmdsCurrent >= m_wMbxInitCmdsCount) && (m_wCoeInitCmdsCurrent >= m_wCoeInitCmdsCount);}
            EC_T_BOOL   AreMbxInitCmdsPending(EC_T_VOID)
                        { return (AreMbxInitCmdsActive() && !AreAllMbxInitCmdsProcessed());}
    virtual EC_T_BOOL   AreAllInitCmdsProcessed(EC_T_VOID)
                        { return CEcSlave::AreAllInitCmdsProcessed() && AreAllMbxInitCmdsProcessed(); }
            PEcMailboxCmdDesc GetMbxInitCmdsCurrent(EC_T_VOID)
                        {
                          if (m_wMbxInitCmdsCurrent < m_wMbxInitCmdsCount)
                          {
                              return m_ppMbxInitCmds[m_wMbxInitCmdsCurrent];
                          }
                          if (m_wCoeInitCmdsCurrent < m_wCoeInitCmdsCount)
                          {
                              return m_ppCoeInitCmds[m_wCoeInitCmdsCurrent];
                          }
                          OsDbgAssert(EC_FALSE);
                          return EC_NULL;
                        }
    virtual EC_T_VOID   StopInitCmds(EC_T_WORD wTransition, EC_T_WORD wCurrTransition);

    virtual EC_T_VOID   OnTimer(EC_T_DWORD dwCurMsecCounter);

    virtual EC_T_VOID   SetSlavePresence(EC_T_BOOL bPresent);
    virtual EC_T_VOID   ProcessCmdResult(EC_T_DWORD result, EC_T_DWORD invokeId, PETYPE_EC_CMD_HEADER pEcCmdHeader);
    
    virtual EC_T_BOOL   HasMailBox();
    EC_T_WORD   GetMbxSize(EC_T_INT inOut)
                        { return (inOut == VG_IN) ? m_mbxILen[m_sMboxFlags.mbxIdx] : m_mbxOLen[m_sMboxFlags.mbxIdx]; }

    virtual EC_T_DWORD  GetThisSize() const { return sizeof(CEcMbSlave); }

    virtual EC_T_BOOL   HasPendingRequests(EC_T_VOID) { return CEcSlave::HasPendingRequests() || m_bMbxCmdPending; }

    EC_T_WORD   GetMbxProtocols(EC_T_VOID)
                        { return m_wSupportedMbxProtocols; }
       
    EC_T_DWORD  GetMbSlaveInfo(EC_T_eINFOENTRY eEntry, EC_T_BYTE* pbyData, EC_T_DWORD* pdwLen);
    EC_T_DWORD  GetMbSlaveInfoAll( EC_T_GET_SLAVE_INFO* pSlaveInfo);
    EC_T_VOID   GetCfgSlaveInfo(EC_T_CFG_SLAVE_INFO* pSlaveInfo);

    EC_T_VOID   AddMailboxActionRequestPoll()  {if(IsPresentOnBus() && m_sMboxFlags.bCyclicPhysicalMBoxPolling &&
                                                   (m_wMailboxActionState & iecs_mbx_poll) != iecs_mbx_poll
                                                  ) 
                                                {
                                                    (m_wMailboxActionRequest |= iecs_mbx_poll);
                                                }};
    EC_T_VOID   AddMailboxActionRequestRead()  {if(IsPresentOnBus() &&
                                                   (m_wMailboxActionState & iecs_mbx_read) != iecs_mbx_read
                                                  )
                                                {
                                                    (m_wMailboxActionRequest |= iecs_mbx_read);
                                                }};

    EC_T_VOID   DelMailboxActionPoll()  {m_wMailboxActionState &= (EC_T_WORD)(~(iecs_mbx_poll));}
    EC_T_VOID   DelMailboxActionRead()  {m_wMailboxActionState &= (EC_T_WORD)(~(iecs_mbx_read));}
    

    EC_T_VOID   SerializeMbxTfers()   {m_bSerializeMbxProtocols = EC_TRUE;}
    EC_T_VOID   ParallelMbxTfers()    {m_bSerializeMbxProtocols = EC_FALSE;}

    EC_T_VOID   SetRetryAccessCount(EC_T_WORD wRetryAccessCount) {m_wRetryAccessCount   = wRetryAccessCount;};
    EC_T_VOID   SetRetryAccessPeriod(EC_T_WORD wRetryAccessPeriod) {m_wRetryAccessPeriod = wRetryAccessPeriod;};

#ifdef INCLUDE_VOE_SUPPORT
    EC_T_VOID*  GetVoeMbxTferObj(){return m_pvVoeMbxTferObj;}
#endif

#ifdef INCLUDE_AOE_SUPPORT
    EC_T_DWORD  GetNetId(EC_T_AOE_NETID* poAoeNetId);
#endif


protected:  
    EC_T_WORD   GetMBoxOutCmdLength(EC_T_WORD length);
    EC_T_DWORD  MailboxRes(PEcMailboxCmd pReq, EC_T_DWORD nResult, EC_T_DWORD cbLength=0, EC_T_VOID* pData=EC_NULL);
    EC_T_BOOL   CycleMBoxPolling();

#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_BOOL   IsAoECmd( PEcMailboxCmd pCmd )
                { return (pCmd->cmdId >= MAILBOXCMD_ECAT_AOE_BEGIN && pCmd->cmdId <= MAILBOXCMD_ECAT_AOE_END); }    
    EC_T_VOID   SendQueuedAoECmds();
    EC_T_VOID   ClearCurrPendAoeMbxCmdDesc() { if (EC_NULL != m_pAoEDesc) OsMemset(&m_pAoEDesc->CurrPendAoeMbxCmdDesc, 0, sizeof(m_pAoEDesc->CurrPendAoeMbxCmdDesc)); }
    EC_T_VOID   StopCurrAoEMbxRequest(EC_T_DWORD result, EC_T_DWORD nData=0, EC_T_VOID* pData=EC_NULL);
    EC_T_VOID   ProcessAoEReturningRequest(EC_T_BOOL bIsInitCmd,EC_T_WORD wWkc, PETYPE_EC_CMD_HEADER pEcCmdHeader, PETHERCAT_MBOX_HEADER pMBox);
    EC_T_VOID   ProcessAoEResponse(EC_T_BOOL bIsInitCmd, PETHERCAT_MBOX_HEADER pMBox);
#endif /* INCLUDE_AOE_SUPPORT */
#if (defined INCLUDE_EOE_SUPPORT)
    static  EC_T_DWORD  SendFrameToSlaveWrap(EC_T_BOOL bIsInitCmd, EC_T_VOID* pContext, PETHERNET_FRAME pData, EC_T_DWORD nData)
        {   EC_UNREFPARM(bIsInitCmd);
            EC_UNREFPARM(pContext);
            EC_UNREFPARM(pData);
            EC_UNREFPARM(nData);
            return EC_S_DEVICE_BUSY; } /* always return BUSY -> SendFrame called only within jobtask */
    EC_T_DWORD  SendFrameToSlave(EC_T_BOOL bIsInitCmd, PETHERNET_FRAME pData, EC_T_DWORD nData);
    EC_T_VOID   SendQueuedEoECmds();
    EC_T_VOID   ProcessEoEReturningRequest(EC_T_BOOL bIsInitCmd, EC_T_WORD wc, PETYPE_EC_CMD_HEADER pEcCmdHeader, PETHERCAT_MBOX_HEADER pMBox);
    EC_T_VOID   ProcessEoEResponse(EC_T_BOOL bIsInitCmd, PETHERCAT_MBOX_HEADER pMBox );
#endif /* INCLUDE_EOE_SUPPORT */
    EC_T_BOOL   IsCoECmd( PEcMailboxCmd pCmd )
                { return (pCmd->cmdId >= MAILBOXCMD_CANOPEN_BEGIN && pCmd->cmdId <= MAILBOXCMD_CANOPEN_END); }
    EC_T_VOID   SendQueuedCoECmds();
    EC_T_VOID   ClearCurrPendCoeMbxCmdDesc() { if (EC_NULL != m_pCoEDesc) OsMemset(&m_pCoEDesc->CurrPendCoeMbxCmdDesc, 0, sizeof(m_pCoEDesc->CurrPendCoeMbxCmdDesc)); }
    EC_T_VOID   StopCurrCoEMbxRequest(EC_T_BOOL bIsInitCmd, EC_T_DWORD result, EC_T_DWORD abortCode=0, EC_T_DWORD nData=0, EC_T_VOID* pData=EC_NULL);
    EC_T_VOID   ProcessCoEResponse(EC_T_BOOL bIsInitCmd, PETHERCAT_MBOX_HEADER pMBox );
    EC_T_VOID   ProcessCoEReturningRequest(EC_T_BOOL bIsInitCmd, EC_T_WORD wc, PETYPE_EC_CMD_HEADER pEcCmdHeader, PETHERCAT_MBOX_HEADER pMBox);
#if (defined INCLUDE_FOE_SUPPORT)
    EC_T_BOOL   IsFoECmd( PEcMailboxCmd pCmd )
                { return (pCmd->cmdId >= MAILBOXCMD_ECAT_FOE_BEGIN && pCmd->cmdId <= MAILBOXCMD_ECAT_FOE_END); }
    EC_T_VOID   SendQueuedFoECmds();
    EC_T_VOID   ClearCurrPendFoeMbxCmdDesc() { if (EC_NULL != m_pFoEDesc) OsMemset(&m_pFoEDesc->CurrPendFoeMbxCmdDesc, 0, sizeof(m_pFoEDesc->CurrPendFoeMbxCmdDesc)); }
    EC_T_VOID   ProcessFoEResponse(EC_T_BOOL bIsInitCmd, PETHERCAT_MBOX_HEADER pMBox);
    EC_T_VOID   ProcessFoEReadResponse(EC_T_BOOL bIsInitCmd, PETHERCAT_MBOX_HEADER pMBox);
    EC_T_VOID   SendFoEAckToSlave(EC_T_BOOL bIsInitCmd, EC_T_DWORD dwPacketNo);
    EC_T_VOID   ProcessFoEWriteResponse(EC_T_BOOL bIsInitCmd, PETHERCAT_MBOX_HEADER pMBox);
    EC_T_VOID   ProcessFoEReturningRequest(EC_T_BOOL bIsInitCmd, EC_T_WORD wc, PETYPE_EC_CMD_HEADER pEcCmdHeader, PETHERCAT_MBOX_HEADER pMBox);
    EC_T_VOID   StopCurrFoEMbxRequest(EC_T_BOOL bIsInitCmd, EC_T_DWORD result, EC_T_DWORD abortCode=0, EC_T_DWORD nData=0, EC_T_VOID* pData=EC_NULL);

public:
    /* Get Mbx data size for in or out according to Slave State, e.g. Bootstrap (see REGULAR_MAILBOX/BOOT_MAILBOX) */
    EC_T_WORD   GetFoEDataSize(EC_T_INT inOut)
                { return (EC_T_WORD)(GetMbxSize(inOut) - ETHERCAT_MBOX_HEADER_LEN - EC_FOE_HDR_LEN); }
    EC_T_WORD   GetFoEMaxFilenameLen()
                {
                    EC_T_WORD wRetVal = 0;
                    EC_T_WORD wMbxSize = GetMbxSize(VG_OUT);
                    if (wMbxSize < EC_FOE_MIN_MBX_LEN)
                    {
                        wRetVal = 0;
                        goto Exit;
                    }
                        
                    /* MaxFilenameLen can be calculated from MbxSize - overhead, 
                       but maximum is MAX_FILE_NAME_SIZE because it is stored in achFileName[MAX_FILE_NAME_SIZE] */
                    wRetVal = (EC_T_WORD)EC_MIN((EC_T_WORD)(wMbxSize - EC_FOE_MIN_MBX_LEN), (EC_T_WORD)MAX_FILE_NAME_SIZE);
                Exit:
                    return wRetVal;
                }
protected:
#endif /* INCLUDE_FOE_SUPPORT */
#if (defined INCLUDE_SOE_SUPPORT)
    EC_T_BOOL   IsSoECmd(PEcMailboxCmd pCmd)
                { return ((pCmd->cmdId >= MAILBOXCMD_ECAT_SOE_BEGIN) && (pCmd->cmdId <= MAILBOXCMD_ECAT_SOE_END)); }
    EC_T_VOID   SendQueuedSoECmds();
    EC_T_VOID   ClearCurrPendSoeMbxCmdDesc() { if (EC_NULL != m_pSoEDesc) OsMemset(&m_pSoEDesc->CurrPendSoeMbxCmdDesc, 0, sizeof(m_pSoEDesc->CurrPendSoeMbxCmdDesc)); }
    EC_T_VOID   ProcessSoEResponse(EC_T_BOOL bIsInitCmd, PETHERCAT_MBOX_HEADER pMBox );
    EC_T_VOID   ProcessSoEReturningRequest(EC_T_BOOL bIsInitCmd, EC_T_WORD wWkc, PETYPE_EC_CMD_HEADER pEcCmdHeader, PETHERCAT_MBOX_HEADER pMBox);
    EC_T_VOID   StopCurrSoEMbxRequest(EC_T_DWORD result, EC_T_DWORD nData=0, EC_T_VOID* pData=EC_NULL);

    EC_T_WORD   GetSoEDataSize(EC_T_INT inOut)
                { return (EC_T_WORD)(GetMbxSize(inOut) - ETHERCAT_MBOX_HEADER_LEN - EC_SOE_HDR_LEN); }
#endif /* INCLUDE_SOE_SUPPORT */
#if (defined INCLUDE_VOE_SUPPORT)
    EC_T_BOOL  IsVoECmd( PEcMailboxCmd pCmd )
                { return (pCmd->cmdId == MAILBOXCMD_ECAT_VOE_READ || pCmd->cmdId == MAILBOXCMD_ECAT_VOE_WRITE); }
    EC_T_VOID   SendQueuedVoECmds();
    EC_T_VOID   ClearCurrPendVoeMbxCmdDesc() { if (EC_NULL != m_pVoEDesc) OsMemset(&m_pVoEDesc->CurrPendVoeMbxCmdDesc, 0, sizeof(m_pVoEDesc->CurrPendVoeMbxCmdDesc)); }
    EC_T_VOID   StopCurrVoEMbxRequest(EC_T_DWORD result, EC_T_DWORD nData=0, EC_T_VOID* pData=EC_NULL);
    EC_T_VOID   ProcessVoEReturningRequest(EC_T_WORD wWkc, PETYPE_EC_CMD_HEADER pEcCmdHeader, PETHERCAT_MBOX_HEADER pMBox );
    EC_T_VOID   ProcessVoEResponse(PETHERCAT_MBOX_HEADER pMBox );
#endif /* INCLUDE_VOE_SUPPORT */
#if (defined INCLUDE_RAWMBX_SUPPORT)
    EC_T_BOOL   IsRawMbxCmd(EcMailboxCmd* pCmd)
                { return (pCmd->cmdId == MAILBOXCMD_ECAT_RAWMBX); }
    EC_T_VOID   SendQueuedRawMbxCmds();
    EC_T_VOID   ClearCurRawMbxDesc()
                { OsMemset(&m_pRawMbxDesc->mbxDesc.cur, 0, sizeof(m_pRawMbxDesc->mbxDesc.cur)); }
    EC_T_VOID   StopCurrRawMbxRequest(EC_T_BOOL bIsInitCmd, EC_T_DWORD dwResult, EC_T_DWORD dwData = 0, EC_T_VOID* pvData = EC_NULL);
    EC_T_VOID   ProcessRawMbxReturningRequest(EC_T_BOOL bIsInitCmd, EC_T_WORD wWkc, ETYPE_EC_CMD_HEADER* pEcCmdHeader, ETHERCAT_MBOX_HEADER* pMBox);
    EC_T_VOID   ProcessRawMbxResponse(ETHERCAT_MBOX_HEADER* pMBox);    
    MBXPROTOCOL_DESC* CreateMbxProtoDesc(EC_T_DWORD dwMaxQueuedCmds);
    EC_T_VOID   ClearMbxProtoDesc(MBXPROTOCOL_DESC* pMbxDesc);
    EC_T_VOID   DeleteMbxProtoDesc(MBXPROTOCOL_DESC* pMbxDesc);

#endif
  
    EC_T_BYTE   IncCntMbxSend()	{ ++m_mbxLayer.byCntSend; if (m_mbxLayer.byCntSend==8) m_mbxLayer.byCntSend=1; return m_mbxLayer.byCntSend; }
    EC_T_BYTE   DecCntMbxSend()	{ --m_mbxLayer.byCntSend; if (m_mbxLayer.byCntSend==0) m_mbxLayer.byCntSend=7; return m_mbxLayer.byCntSend; }
    EC_T_BYTE   IncCntMbxRecv()	{ ++m_mbxLayer.byCntRecv; if (m_mbxLayer.byCntRecv==8) m_mbxLayer.byCntRecv=1; return m_mbxLayer.byCntRecv; }	

    EC_T_DWORD  EcatMbxSendCmdReq(EC_T_BOOL  bIsInitCmd
                                 ,EC_T_VOID* pData
                                 ,PETHERCAT_MBOX_HEADER     pMbox
                                 ,PETHERCAT_CANOPEN_HEADER  pCANopen
                                 ,PEC_SDO_HDR               pSDO
#ifdef INCLUDE_EOE_SUPPORT
                                 ,PETHERCAT_ETHERNET_HEADER pEthernet
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                 ,PEC_FOE_HDR               pFoE
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                 ,PEC_SOE_HDR               pSoE
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                 ,PEC_AOE_HDR               pAoE
                                 ,PEC_AOE_CMD_HDR           pAoeCmd
#endif
								 );

    EC_T_VOID   StopCurrMbxRequest(EC_T_BOOL bIsInitCmd, EC_T_DWORD dwResult, PETHERCAT_MBOX_HEADER pMBox);

    PECMBSLAVE_COE_DESC     m_pCoEDesc;
#if (defined INCLUDE_EOE_SUPPORT)
    PECMBSLAVE_EOE_DESC     m_pEoEDesc;
#endif
#if (defined INCLUDE_FOE_SUPPORT)
    PECMBSLAVE_FOE_DESC     m_pFoEDesc;
#endif
#if (defined INCLUDE_SOE_SUPPORT)
    PECMBSLAVE_SOE_DESC     m_pSoEDesc;
#endif
#if (defined INCLUDE_VOE_SUPPORT)
    PECMBSLAVE_VOE_DESC     m_pVoEDesc;
    EC_T_VOID*              m_pvVoeMbxTferObj;
#endif
#if (defined INCLUDE_AOE_SUPPORT)
    PECMBSLAVE_AOE_DESC     m_pAoEDesc;
    EC_T_DWORD              m_dwAoeInvokeId;
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
    ECMBSLAVE_RAWMBX_DESC*  m_pRawMbxDesc;
#endif
    EC_T_BYTE               m_byNumMbxProtocol;     /* Count the number of supported mailbox protocols */
    EC_T_WORD               m_wSupportedMbxProtocols;

    EC_T_WORD                   m_mbxOStart[2];
    EC_T_WORD                   m_mbxOLen[2];
    EC_T_WORD                   m_mbxIStart[2];
    EC_T_WORD                   m_mbxILen[2];
    
    struct
    {
        EC_T_WORD               bMbxOutShortSend                : 1;
        EC_T_WORD               mbxIdx                          : 1;
        EC_T_WORD               bCyclicPhysicalMBoxPolling      : 1;    /* set if per slave physical mailbox polling is active */
        EC_T_WORD               bLogicalStateMBoxPolling        : 1;    /* set if logical mailbox state polling is active */
        EC_T_WORD               bCyclicPhysicalMBoxBootPolling  : 1;    /* set if per slave physical mailbox Bootstrap polling is active */
        EC_T_WORD               bLogicalStateMBoxBootPolling    : 1;    /* set if logical mailbox state Bootstrap polling is active */
        EC_T_WORD               bBootStrapSupport               : 1;    /* set if FoE is configured in Bootstrap mode */
        EC_T_WORD               bFoeSupportNotBS                : 1;    /* set if FoE is configured in normal mode */
    } m_sMboxFlags;

    EC_T_WORD                   m_wMbxInitCmdsCurrent;
    EC_T_WORD                   m_wCoeInitCmdsCurrent;
    EC_T_WORD                   m_wMbxInitCmdsCount;
    EC_T_WORD                   m_wCoeInitCmdsCount;
    EC_T_WORD                   m_wMbxInitCmdsDataOffset;
    EC_T_DWORD                  m_dwCoeInitCmdsSize;                    /* size in bytes of m_pCoeInitCmds array */

    PEcMailboxCmdDesc*          m_ppMbxInitCmds;
    PEcMailboxCmdDesc           m_pCoeInitCmds;
    PEcMailboxCmdDesc*          m_ppCoeInitCmds;

    EC_T_WORD                   m_wSlaveLogicalAddressMBoxState;        /* bit offset in logical area for logical mailbox state polling */
    EC_T_WORD                   m_wCyclePhysicalMBoxPollingInterval;    /* interval for MBox polling (number of slave ticks before polling occurs) */
    
#if (defined INCLUDE_FOE_SUPPORT)
    EC_T_WORD                   m_wSlaveLogicalAddressMBoxBootState;    /* bit offset in logical area for logical mailbox state polling */
    EC_T_WORD                   m_wCyclePhysicalMBoxBootPollingInterval;/* interval for MBox polling (number of slave ticks before polling occurs) */
#endif
    EC_T_BOOL                   m_bMbxCmdPending;                           /* indicates that already a mailbox telegram is queued and not processed */

private:
    EC_T_VOID                   MBoxReadFromSlave();
    

    EC_T_WORD                   m_wMailboxActionRequest;
    EC_T_WORD                   m_wMailboxActionState;
    EC_T_BOOL                   m_bSerializeMbxProtocols;               /* flag: if set, no parallel mbx protocol allowed */

    /* MBX Polling */
    EC_T_DWORD                  m_dwLastOnTimerCall;                    /* used to evaluate time since last OnTimer call */
    EC_T_DWORD                  m_dwCurMbxPollingAge;                   /* Mbx Age Level, when over m_wCyclePhysicalMBoxPollingInterval, phys mbx polling is triggered */

    struct _mbxLayer
    {
        enum 
        {
            IDLE        = 0, 
            TOGGLED
        }	            eState;
        EC_T_BYTE       byCntSend;
        EC_T_BYTE       byCntRecv;
        EC_T_BOOL       bToggleBit;
        EC_T_BOOL       bRepeatRequested;
        CEcTimer  oTimeout;		
        EC_T_BYTE       byIDX;
    } m_mbxLayer;

    EC_T_BOOL                   m_bMbxReadStateIsBusy;              /* FALSE = New mailbox read allowed, TRUE = Waiting for return of last mailbox read */
    EC_T_WORD                   m_wRetryAccessCount;
    EC_T_WORD                   m_wRetryAccessPeriod;
    EC_T_WORD                   m_wEoeRetryAccessPeriod;
};

#endif /* INC_ECSLAVE */

/*-END OF SOURCE FILE--------------------------------------------------------*/
