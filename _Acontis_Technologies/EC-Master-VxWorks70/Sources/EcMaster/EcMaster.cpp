/*-----------------------------------------------------------------------------
 * EcMaster.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description
 *---------------------------------------------------------------------------*/


/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>

#include <EcMasterCommon.h>
#include <EcLink.h>
#include <EcInterfaceCommon.h>
#include <EcInterface.h>
#include <EcEthSwitch.h>
#include <EcIoImage.h>
#include "EcFiFo.h"
#include <EcServices.h>
#include <EcDevice.h>
#include <EcMaster.h>

#include <EcBusTopology.h>
#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
#include <EcHotConnect.h>
#endif
#if (defined INCLUDE_DC_SUPPORT)
#include <EcDistributedClocks.h>
#endif

#if (defined INCLUDE_MASTER_OBD)
#include <EcSdoServ.h>
#endif

#include <EcEEPRom.h>

#include <EcSlave.h>

#include <EcInvokeId.h>

#include <EcDeviceFactory.h>

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
#include <EcPts.h>
#endif

#include "EcText.h"

/*-DEFINES-------------------------------------------------------------------*/
#define INCLUDE_FRAME_LOSS_RETRY

/* debug message type IDs for link layer debug messages */
#define ETHTYPE1        ((EC_T_BYTE)0xFF)
#define ETHTYPE0        ((EC_T_BYTE)0x00)
#define NOTIFICATION    ((EC_T_BYTE)0x00)
#define STATE_CHNG      ((EC_T_BYTE)0x01)

static inline EC_T_DWORD BYTE_ALIGN_DOWN(EC_T_DWORD val) { return val & ~7; }
static inline EC_T_DWORD BYTE_ALIGN_UP(EC_T_DWORD val) { return (val + 7) & ~7; }
static inline EC_T_DWORD WORD_ALIGN_UP(EC_T_DWORD val) { return (val + 15) & ~15; }

#define INIT_CMD_DEFAULT_TIMEOUT        5000        /* 5000 ms */

#if (defined INCLUDE_GEN_OP_ENI)
#define LOGICAL_MAILBOX_STATE_ADDR      0x09000000
#define LWR_START_ADDR                  0x10000000
#define LRD_START_ADDR                  0x10000800
#endif /*INCLUDE_GEN_OP_ENI*/

/*-TYPEDEFS------------------------------------------------------------------*/

/*-CEcSlaveFrame---------------------------------------------------------*/

/********************************************************************************/
/** \brief CEcSlaveFrame constructor
*
*/
CEcSlaveFrame::CEcSlaveFrame()
{
    m_pSlaveFrameDesc   = EC_NULL;
    OsMemset(&m_EthernetFrame, 0, sizeof(ETHERNET_88A4_FRAME));
    m_p88A4Header       = &(m_EthernetFrame.E88A4Header);
    m_pLastEcCmdHeader  = EC_NULL;
    m_aEcCmdDesc        = EC_NULL;
    m_nNumCmdsInFrame   = 0;
    m_nSpace            = 0;
    m_retry             = 0;
    m_byEcCmdHeaderIDX  = 0;
}

/********************************************************************************/
/** \brief CEcSlaveFrame initialization
*
*/
EC_T_VOID CEcSlaveFrame::InitInstance(EC_T_DWORD nMaxSlaveCmdsPerFrame, struct _EC_T_MEMORY_LOGGER* pMLog)
{
    m_aEcCmdDesc = (PECAT_SLAVECMD_DESC)OsMalloc(sizeof(ECAT_SLAVECMD_DESC) * nMaxSlaveCmdsPerFrame);
    OsMemset(m_aEcCmdDesc, 0, sizeof(ECAT_SLAVECMD_DESC) * nMaxSlaveCmdsPerFrame);
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "CEcSlaveFrame::m_aEcCmdDesc", m_aEcCmdDesc, 
        nMaxSlaveCmdsPerFrame * sizeof(ECAT_SLAVECMD_DESC));
}

EC_T_VOID CEcSlaveFrame::FreeInstance(EC_T_DWORD nMaxSlaveCmdsPerFrame, struct _EC_T_MEMORY_LOGGER* pMLog)
{
    if (m_aEcCmdDesc)
    {
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "CEcSlaveFrame::~m_aEcCmdDesc", m_aEcCmdDesc, 
            nMaxSlaveCmdsPerFrame * sizeof(ECAT_SLAVECMD_DESC));
        OsFree(m_aEcCmdDesc);
        m_aEcCmdDesc = EC_NULL;
    }
}

/********************************************************************************/
/** \brief CEcSlaveFrame destructor
*
*/
CEcSlaveFrame::~CEcSlaveFrame()
{
}

/*-CEcMaster-----------------------------------------------------------------*/

#define CECMASTERCONST_m_dwSBTimeout                    EC_SB_DEFAULTTIMEOUT
#define CECMASTERCONST_m_dwHCDetectionTimeout           EC_HC_DETECTIONTIMEOUT
#define CECMASTERCONST_m_bTopologyChangeAutoMode        ECBT_TOPOCHANGE_AUTOMODE
#define CECMASTERCONST_m_dwTopologyChangeDelay          ECBT_TOPOCHANGE_DELAY
#define CECMASTERCONST_m_oHCMODE                        EC_HC_DFLT_HCMODE
#define CECMASTERCONST_m_bNotifyUnexpectedBusSlaves     EC_TRUE
#define CECMASTERCONST_m_dwInitCmdRetryTimeout          EC_INITCMDRETRYTIMEOUT
#define CECMASTERCONST_m_dwMbxCmdTimeout                EC_MBX_DEFAULT_TIMEOUT
#define CECMASTERCONST_m_dwDefaultMbxPolling            DEFAULT_PHYSICAL_MBX_POLLING_TIME
#define CECMASTERCONST_m_dwDcmInSyncTimeout             EC_WAITINFINITE
#define CECMASTERCONST_m_bFirstDcSlaveAsRefClock        EC_FALSE
#define CECMASTERCONST_m_bAllSlavesMustReachState       EC_TRUE
#define CECMASTERCONST_m_eCycFrameLayout                eCycFrameLayout_STANDARD
#define CECMASTERCONST_m_dwNotificationMask             NOTIFICATION_MASK_DEFAULT
#define CECMASTERCONST_m_dwCycErrorNotifyMask           EC_CYC_ERR_MASK_ALL
#define CECMASTERCONST_m_dwStateChangeDefaultTimeout    EC_MASTER_STATECHANGE_DEFAULTTIMEOUT
#if (defined VLAN_FRAME_SUPPORT)
#define CECMASTERCONST_m_bVLANEnabled                   EC_FALSE
#define CECMASTERCONST_m_wVLANId                        0
#define CECMASTERCONST_m_byVLANPrio                     0
#endif
#define CECMASTERCONST_m_bCopyInfoInSendCycFrames       EC_FALSE
#define CECMASTERCONST_m_bIgnoreInputsOnWkcError        EC_FALSE
#define CECMASTERCONST_m_bZeroInputsOnWkcZero           EC_FALSE
#define CECMASTERCONST_m_bZeroInputsOnWkcError          EC_FALSE
#if (defined INCLUDE_VARREAD)
#define CECMASTERCONST_m_bAddVarsForSpecificDataTypes   EC_TRUE
#endif
#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
#define CECMASTERCONST_m_bRedEnhancedLineCrossedDetectionEnabled EC_FALSE
#endif
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
#define CECMASTERCONST_m_bErrorOnLineCrossed            EC_TRUE
#define CECMASTERCONST_m_bNotifyNotConnectedPortA       EC_TRUE
#define CECMASTERCONST_m_bNotifyUnexpectedConnectedPort EC_TRUE
#endif
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
#define CECMASTERCONST_m_bJunctionRedundancyEnabled     EC_FALSE
#endif
#define CECMASTERCONST_m_bGenEniAssignEepromBackToEcat  EC_TRUE
#define CECMASTERCONST_m_bAutoAckAlStatusError          EC_TRUE
#define CECMASTERCONST_m_bAutoAdjustCycCmdWkc           EC_FALSE
#define CECMASTERCONST_m_bAdjustCycFramesAfterslavesStateChange EC_FALSE

CEcMaster::CEcMaster
(
    CEcDevice*              poEcDevice,         /* [in]  EtherCAT device interface */
    PEcMasterDesc           pDesc,              /* [in]  master descriptor */
    MBX_CALLBACK            pfMbCallBack,       /* [in]  mailbox callback */
    NOTIFY_CALLBACK         pfNotifyCallback,   /* [in]  notification callback see CAtEmInterface::WrapperToNotifyAllClients() */
    EC_T_MASTER_CONFIG*     pMasterConfig,      /* [in]  parameters */
    EC_T_MASTER_CONFIG_EX*  pMasterConfigEx,    /* [in]  extended parameters */
    EC_T_INTFEATUREFIELD_DESC* pFeatures,       /* [in]  features to preconfigure */
    EC_T_DWORD              dwVersion           /* [in]  application asked version */
)
:
    m_MasterConfig(*pMasterConfig),
    m_poEcDevice(poEcDevice),

    m_pfMbCallBack(pfMbCallBack),
    m_pfNotifyCallback(pfNotifyCallback),
    m_pMasterDesc(pDesc),

    m_MasterConfigEx(*pMasterConfigEx),
    m_dwAppVersion(dwVersion)
{
    EC_T_DWORD i = 0;

#if (defined INCLUDE_OEM)
    m_qwOemKey = 0;
#endif /* INCLUDE_OEM */
    m_dwSupportedFeatures = 0;

#if (defined INCLUDE_DC_SUPPORT)
    /* initialize DC */
    m_pDistributedClks  = EC_NEW(CEcDistributedClocks(this));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_pDistributedClks", m_pDistributedClks, sizeof(CEcDistributedClocks));
    m_bDCSupportEnabled        = EC_FALSE;
#endif /* INCLUDE_DC_SUPPORT */

    /* initialize BT */
    m_pBusTopology      = EC_NEW(CEcBusTopology());
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_pBusTopology", m_pBusTopology, sizeof(CEcBusTopology));
    m_pBusTopology->InitInstance(this, m_poEcDevice, m_MasterConfigEx.wMaxBusSlaves);

    /* by default enable SB Support */
    m_dwSBTimeout       = CECMASTERCONST_m_dwSBTimeout;
    m_dwScanBusResult   = EC_E_NOTREADY;

    /* Hot Connect Detection TO */
    m_dwHCDetectionTimeout = CECMASTERCONST_m_dwHCDetectionTimeout;

    /* Determine the start of the acyclic command index  */
    m_byMinAcycCmdIdx = (EC_T_BYTE)(0xFF - pMasterConfig->dwMaxQueuedEthFrames);

#if (defined INCLUDE_MASTER_OBD)
    m_pSDOService       = EC_NULL;
    m_wStateChngReq     = 0xffff;
#endif

    m_pEcMbxCoeEmergencyCmd     = EC_NULL;
    m_pEcMbxSoeEmergencyCmd     = EC_NULL;
    m_pEcMbxSoeNotificationCmd  = EC_NULL;

    /* at the beginning assume no slave error detected */
    m_bSlaveErrorDetected       = EC_FALSE;

    m_bRawTferPending           = EC_FALSE;
    m_dwRawTferStatus           = EC_E_NOERROR;
    m_wRawTferDataLen           = 0;
    m_pbyRawTferData            = EC_NULL;

    /* milli seconds counter */
    m_dwCfgSlaveOnTimerIdx      = 0;
    m_dwMsecCounter             = OsQueryMsecCount();

    m_bLatchReceiveTimesRequest = EC_FALSE;

    m_wAlStatusReg              = 0;
    m_wALStatusWkc              = EC_AL_STATUS_INIT_VALUE;
    m_wALStatusExpectedWkc      = 0;

#if (defined VLAN_FRAME_SUPPORT)
    /* VLAN vars */
    m_bVLANEnabled              = CECMASTERCONST_m_bVLANEnabled;
    m_wVLANId                   = CECMASTERCONST_m_wVLANId;
    m_byVLANPrio                = CECMASTERCONST_m_byVLANPrio;
    if (EC_NULL != pFeatures)
    {
        m_bVLANEnabled          = pFeatures->bVLANEnabled;
        m_wVLANId               = pFeatures->wVLANId;
        m_byVLANPrio            = pFeatures->byVLANPrio;
    }
#endif

    /* clean memory provider struct */
    OsMemset(&m_oMemProvider, 0, sizeof(EC_T_MEMPROV_DESC));

    m_bPdOffsetCompMode     = EC_FALSE;

#if (defined INCLUDE_WKCSTATE)
    m_nDiagnosisImageSize   = 0;
    m_pbyDiagnosisImage     = EC_NULL;
#endif /* INCLUDE_WKCSTATE */

#if (defined INCLUDE_DC_SUPPORT)
    m_bAddBrdSyncWindowMonitoring = EC_FALSE;
    m_dwDcmInSyncTimeout    = CECMASTERCONST_m_dwDcmInSyncTimeout;
#endif /* INCLUDE_DC_SUPPORT */

    m_bConfigLoaded         = EC_FALSE;
    m_bConfigured           = EC_FALSE;

    m_nCfgSlaveCount        = 0;
    m_nEcMbSlave            = 0;
    m_wMaxNumSlaves         = pDesc->wMaxNumSlaves;
    m_ppEcSlave             = EC_NEW(CEcSlave*[m_wMaxNumSlaves]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_ppEcSlave", m_ppEcSlave, m_wMaxNumSlaves * sizeof(CEcSlave*));
    OsMemset(m_ppEcSlave, 0, m_wMaxNumSlaves*sizeof(CEcSlave*));

    m_ppEcMbSlave           = EC_NEW(CEcMbSlave*[m_wMaxNumSlaves]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_ppEcMbSlave", m_ppEcMbSlave, m_wMaxNumSlaves * sizeof(CEcMbSlave*));
    OsMemset(m_ppEcMbSlave, 0, m_wMaxNumSlaves*sizeof(CEcMbSlave*));

    m_pSlavesStateMachineFifo = EC_NEW(CFiFoListDyn<CEcSlave*>(m_MasterConfigEx.wMaxBusSlaves, EC_NULL, EC_NULL, EC_MEMTRACE_CORE_MASTER));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_pSlavesStateMachineFifo", m_pSlavesStateMachineFifo, sizeof(CFiFoListDyn<CEcSlave*>));

    m_wCurMasterDeviceState  = DEVICE_STATE_UNKNOWN;
    m_wReqMasterDeviceState  = DEVICE_STATE_UNKNOWN;
    m_eCurMasterLogicalState = EC_MASTER_STATE_UNKNOWN;
    m_wCycFramesDeviceState  = EC_MASTER_STATE_UNKNOWN;
    m_maxAsyncFrameSize = ETHERNET_MAX_FRAME_LEN;

    /* no lock required because add and remove is always in OnMasterTimer() */
    m_dwStateChangeDefaultTimeout = EC_MASTER_STATECHANGE_DEFAULTTIMEOUT;
    m_bMasterTimerEnabled = EC_FALSE;
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    m_dwNumCycConfigEntries = 0;
#endif
    m_byMaxCycleIdx = 0;
    m_byCurCycleIdx = 0;
    m_wAllCycEntriesNumFrames = 0;
    m_dwCycErrorNotifyMask = CECMASTERCONST_m_dwCycErrorNotifyMask;
    m_dwCycErrorNotifyMaskedCnt = 0;
    m_dwFrameResponseNotifyMaskedCnt = 0;

    m_bAllSlavesMustReachState = CECMASTERCONST_m_bAllSlavesMustReachState;
    m_dwNotificationMask = CECMASTERCONST_m_dwNotificationMask;
    OsMemset(&m_SlavesStateChangedNotifyDesc, 0, sizeof(EC_T_SLAVES_STATECHANGED_NTFY_DESC));
    OsMemset(&m_SlavesUnexpectedStateNotifyDesc, 0, sizeof(EC_T_ERROR_NOTIFICATION_DESC));
    OsMemset(&m_SlavesPresenceNotifyDesc, 0, sizeof(EC_T_SLAVES_PRESENCE_NTFY_DESC));
    OsMemset(&m_SlavesErrorNotifyDesc, 0, sizeof(EC_T_ERROR_NOTIFICATION_DESC));

    m_dwMasterStateMachineResult = EC_E_NOTREADY;
    m_bSlaveStateMachineError = EC_FALSE;
    m_bCancelStateMachine     = EC_FALSE;
    m_bStateMachIsStable      = EC_TRUE;

    m_poFreeSlaveFrameDescLock = OsCreateLockTyped(eLockType_SPIN);
    m_poEcTransferRawCmdLock   = OsCreateLockTyped(eLockType_INTERFACE);

    /* initialize Notification Store */
    m_pNotificationStore    = (EC_T_NOTIFICATION_INTDESC*)OsMalloc((sizeof(EC_T_NOTIFICATION_INTDESC)*EC_NOTIFICATION_BUFFERS));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster:m_pNotificationStore", m_pNotificationStore, sizeof(EC_T_NOTIFICATION_INTDESC)*EC_NOTIFICATION_BUFFERS);

    OsDbgAssert(EC_NULL != m_pNotificationStore);
    if (EC_NULL != m_pNotificationStore)
    {
        EC_T_DWORD dwIdx = 0;
        OsMemset(m_pNotificationStore, 0, sizeof(EC_T_NOTIFICATION_INTDESC) * EC_NOTIFICATION_BUFFERS);

        for (dwIdx = 0; dwIdx < EC_NOTIFICATION_BUFFERS; dwIdx++)
        {
            m_pNotificationStore[dwIdx].dwIdx = dwIdx;
        }
    }

    InitializeListHead(&m_FreeSlaveFrameDescList);
    InitializeListHead(&m_PendingSlaveFrameDescList);

    m_pCurrPendingSlaveFrameDesc    = EC_NULL;

    /* adjust parameters if necessary */
    OsDbgAssert( ETHERNET_MAX_FRAME_LEN >= (MAX_EC_CMD_PER_FRAME * ETYPE_EC_OVERHEAD) + 0x10);
    if (m_MasterConfig.dwMaxSlaveCmdPerFrame == 0)
    {
        m_MasterConfig.dwMaxSlaveCmdPerFrame = MAX_EC_CMD_PER_FRAME;
    }
    else if (m_MasterConfig.dwMaxSlaveCmdPerFrame > MAX_EC_CMD_PER_FRAME)
    {
        EC_ERRORMSG(("EC-Master configuration error: limit MasterConfig.dwMaxSlaveCmdPerFrame to %d!\n", MAX_EC_CMD_PER_FRAME));
        m_MasterConfig.dwMaxSlaveCmdPerFrame = MAX_EC_CMD_PER_FRAME;
    }
    /* allocate buffer for the pending EtherCAT Frames that are initialized inQueueEcatCmdReq */
    m_aoEcSlaveFrame = EC_NEW(CEcSlaveFrame[m_MasterConfig.dwMaxQueuedEthFrames]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_aoEcSlaveFrame", m_aoEcSlaveFrame, m_MasterConfig.dwMaxQueuedEthFrames * sizeof(CEcSlaveFrame));
    OsMemset(m_aoEcSlaveFrame, 0, m_MasterConfig.dwMaxQueuedEthFrames * sizeof(CEcSlaveFrame));

    m_aSlaveFrameDesc = EC_NEW(ECAT_SLAVEFRAME_DESC[m_MasterConfig.dwMaxQueuedEthFrames]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_aSlaveFrameDesc", m_aSlaveFrameDesc, m_MasterConfig.dwMaxQueuedEthFrames * sizeof(ECAT_SLAVEFRAME_DESC));

    OsMemset(m_aSlaveFrameDesc, 0, m_MasterConfig.dwMaxQueuedEthFrames * sizeof(ECAT_SLAVEFRAME_DESC));
    OsDbgAssert(m_aSlaveFrameDesc!=EC_NULL);
    for (i=0; i < m_MasterConfig.dwMaxQueuedEthFrames; i++)
    {
        m_aSlaveFrameDesc[i].ListEntry.Flink = EC_NULL;
        m_aSlaveFrameDesc[i].ListEntry.Blink = EC_NULL;
        m_aSlaveFrameDesc[i].poEcSlaveFrame  = &m_aoEcSlaveFrame[i];
        m_aoEcSlaveFrame[i].m_pSlaveFrameDesc                         = &m_aSlaveFrameDesc[i];
        m_aoEcSlaveFrame[i].InitInstance(m_MasterConfig.dwMaxSlaveCmdPerFrame, GetMemLog());
        m_aoEcSlaveFrame[i].m_byEcCmdHeaderIDX                        = (EC_T_BYTE)(m_byMinAcycCmdIdx + i);
        m_aoEcSlaveFrame[i].m_EthernetFrame.EthernetFrame.Destination = m_pMasterDesc->oMacDest;
        m_aoEcSlaveFrame[i].m_EthernetFrame.EthernetFrame.Source      = m_pMasterDesc->oMacSrc;
        EC_ETHFRM_SET_FRAMETYPE((&(m_aoEcSlaveFrame[i].m_EthernetFrame.EthernetFrame)), ETHERNET_FRAME_TYPE_BKHF);

        m_aoEcSlaveFrame[i].m_p88A4Header = &(m_aoEcSlaveFrame[i].m_EthernetFrame.E88A4Header);

#if (defined VLAN_FRAME_SUPPORT)
        if (m_bVLANEnabled)
        {
            EC_ETHFRM_SET_FRAMETYPE((EC_T_BYTE*)&(m_aoEcSlaveFrame[i].m_EthernetFrame.EthernetFrame), ETHERNET_FRAME_TYPE_VLAN);

            m_aoEcSlaveFrame[i].m_p88A4Header = &(((ETHERNET_88A4_MAX_VLAN_FRAME*)(&(m_aoEcSlaveFrame[i].m_EthernetFrame.EthernetFrame)))->E88A4Header);

            ETYPE_VLAN_HEADER* pVlanHrd = &((TETHERNET_VLAN_FRAME*)&(m_aoEcSlaveFrame[i].m_EthernetFrame.EthernetFrame))->VLan;

            EC_RESET_VLANHDR(pVlanHrd);
            EC_SET_VLANHDR_Type(pVlanHrd, ETHERNET_FRAME_TYPE_VLAN);
            EC_SET_VLANHDR_ID(pVlanHrd, m_wVLANId);
            EC_SET_VLANHDR_Prio(pVlanHrd, m_byVLANPrio);

            EC_ETHFRM_SET_FRAMETYPE((EC_T_BYTE*)&(m_aoEcSlaveFrame[i].m_EthernetFrame.EthernetFrame) + ETYPE_VLAN_HEADER_LEN, ETHERNET_FRAME_TYPE_BKHF);
        }
#endif

        EC_88A4HDR_RESET(m_aoEcSlaveFrame[i].m_p88A4Header);
        EC_88A4HDR_SET_E88A4HDRTYPE((m_aoEcSlaveFrame[i].m_p88A4Header), ETYPE_88A4_TYPE_ECAT);
        EC_88A4HDR_SET_E88A4FRAMELEN((m_aoEcSlaveFrame[i].m_p88A4Header), 0);

        OsDbgAssert((EC_T_BYTE)(m_byMinAcycCmdIdx + i) < (m_byMinAcycCmdIdx + m_MasterConfig.dwMaxQueuedEthFrames));
        OsDbgAssert((EC_T_BYTE)(m_byMinAcycCmdIdx + i) < 0xff);

        OsDbgAssert(m_aoEcSlaveFrame[i].m_byEcCmdHeaderIDX < (m_byMinAcycCmdIdx + m_MasterConfig.dwMaxQueuedEthFrames));
        OsDbgAssert( m_aoEcSlaveFrame[i].m_byEcCmdHeaderIDX < 0xFF); /* maximum allowed IDX value is 0xFE */

        InsertTailList(&m_FreeSlaveFrameDescList, &m_aSlaveFrameDesc[i].ListEntry);
    }
    m_ppEcPortMBoxState = EC_NULL;
    if (m_pMasterDesc->wSizeAddressMBoxStates > 0)
    {   /* master is configured to check the state of the mailbox */
        m_ppEcPortMBoxState = EC_NEW(CEcMbSlave*[m_pMasterDesc->wSizeAddressMBoxStates * 8]);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_ppEcPortMBoxState", m_ppEcPortMBoxState, 
            m_pMasterDesc->wSizeAddressMBoxStates * 8 * sizeof(CEcMbSlave*));
        OsMemset(m_ppEcPortMBoxState, 0, m_pMasterDesc->wSizeAddressMBoxStates * 8 * sizeof(CEcMbSlave*));
    }
    /* init commands */
    m_nInitCmdsCount   = 0;
    m_nInitCmdsCurrent = INITCMD_INACTIVE;
    m_wMaxNumInitCmds = pDesc->wNumEcatInitCmds;
    if (0 == m_wMaxNumInitCmds)
    {
        m_ppInitCmds = EC_NULL;
    }
    else
    {
        m_ppInitCmds = EC_NEW(PEcInitCmdDesc[m_wMaxNumInitCmds]);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_ppInitCmds", m_ppInitCmds, m_wMaxNumInitCmds * sizeof(PEcInitCmdDesc));
    }
    if (m_ppInitCmds)
    {
        EcInitCmdDesc*  pInitCmdDesc = EC_NULL;

        for (i = 0; i < pDesc->wNumEcatInitCmds; i++)
        {
            pInitCmdDesc = pDesc->apPkdEcatInitCmdDesc[i];

            if ((EC_ICMDHDR_GET_CMDIDX_CMD(&(pInitCmdDesc->EcCmdHeader)) == EC_CMD_TYPE_BWR) && (EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)) == ECREG_STATION_ADDRESS))
            {
                /* filter broadcast write of configured station address */
                /* because master setup all the fixed address already in the bus scan */
                /* SafeDeleteArray(pCmd); */
                /* continue; */

                /* reuse this master init command to disable PDI watchdog on transition to INIT */
                EC_ICMDHDR_SET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader), ECREG_WATCHDOG_TIME_PDI);
                EC_ECINITCMDDESC_SET_TRANSITION(pInitCmdDesc, (ECAT_INITCMD_P_I | ECAT_INITCMD_S_I | ECAT_INITCMD_O_I | ECAT_INITCMD_B_I | ECAT_INITCMD_BEFORE));
            }
            if ((EC_ICMDHDR_GET_CMDIDX_CMD(&(pInitCmdDesc->EcCmdHeader)) == EC_CMD_TYPE_BWR) && (EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)) == ECREG_SLV_ECATEVENTMASK))
            {
                /* filter broadcast write of ECAT event mask */
                /* because this is done in the bus scan */
                continue;
            }
            if ((EC_ICMDHDR_GET_CMDIDX_CMD(&(pInitCmdDesc->EcCmdHeader)) == EC_CMD_TYPE_BWR) && (EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)) == ECREG_FMMU_CONFIG))
            {
                /* allow clear FMMU only for IP transition */
                /* to protect for error state during transition to INIT */
                EC_ECINITCMDDESC_SET_TRANSITION(pInitCmdDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE);
            }
            /* place init command in slave command list */
            m_ppInitCmds[m_nInitCmdsCount++] = pInitCmdDesc;
        }
    }
    /* slave CoeInitCmds */
    m_pvSlaveCoeInitCmdCallbackParm = EC_NULL;
    m_pfnSlaveCoeInitCmdCallback = EC_NULL;

    /* cyclic communication timeout check */
    m_aCyclicEntryMgmtDesc = EC_NULL;

    /* process data write access management */
    m_bHavePdWriteAccess = EC_FALSE;
    m_pbyPDInBasePtr = EC_NULL;
#ifdef DEBUG
    m_dwPrintNotOpTime = 0;
    m_dwPrintErrStateTime = 0;
#endif
    m_bCyclicTimeoutEnabled = EC_FALSE;

#if (defined EVAL_VERSION)
    m_pLkMemory      = EC_NEW(EC_T_BYTE[512]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_pLkMemory", m_pLkMemory, 512);
#endif

#if (defined INCLUDE_MASTER_OBD)
    m_pSDOService       = EC_NEW(CEcSdoServ(this, m_poEcDevice, m_MasterConfigEx.wMaxBusSlaves));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_pSDOService", m_pSDOService, sizeof(CEcSdoServ));

    m_pdwLostFrameCrt   = EC_NULL;

    if (EC_NULL != m_pSDOService)
    {
        m_pdwLostFrameCrt   = m_pSDOService->GetLostFrameCrtPtr();

#if (defined INCLUDE_DC_SUPPORT)
        m_pSDOService->EntrySet_SetDCEnabled(m_bDCSupportEnabled);
#endif
    }
#endif

    m_pEEEPRom = EC_NEW(CEcEEPRom(this, m_poEcDevice));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_pEEEPRom", m_pEEEPRom, sizeof(CEcEEPRom));

#if (defined INCLUDE_SLAVE_STATISTICS)
    m_dwSlaveStatisticsPeriod  = EC_MASTER_SLAVESTATISTICS_PERIOD;
    if (0 != m_dwSlaveStatisticsPeriod)
    {
        m_oSlaveStatisticsTimer.Start(m_dwSlaveStatisticsPeriod, GetMsecCounterPtr());
    }
    m_dwSlaveStatisticsSlaveIdxCur = 0;
    m_bSlaveStatisticsRequested    = EC_FALSE;
#endif

#if (defined INCLUDE_RED_DEVICE)
    m_bRedFrameSplitEnabled = EC_FALSE;
    m_bRedNotify            = EC_FALSE;
    m_bRedNotifyActive      = EC_TRUE;
    m_bRedLineBreak         = EC_FALSE;
    m_bRedLineCrossed       = EC_FALSE;
    m_bRedLineChanging      = EC_FALSE;
    m_wRedNumSlavesOnMain   = 0;
    m_wRedNumSlavesOnRed    = 0;
    m_pbyRedFrame           = EC_NEW(EC_T_BYTE[sizeof(ETHERNET_88A4_MAX_FRAME)]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::CreateCyclicFrames", m_pbyRedFrame, sizeof(ETHERNET_88A4_MAX_FRAME));
#endif
    m_bNotAllDevOpNotified            = EC_FALSE;
    m_bNotAllDevOpNotificationEnabled = EC_TRUE;
    m_dwMinCycOutOffset     = 0;
    m_dwMinRecv             = 0;
    m_dwMinCycInOffset      = 0;
    m_dwMinSend             = 0;
    m_dwCopyOffset          = 0;
    m_dwDuplicateCheckCount = 0;
    m_bOnlyProcDataInImage  = m_MasterConfigEx.bOnlyProcDataInImage;

    m_pvCycFrameRxCallBack      = EC_NULL;
    m_pvCycFrameRxCallBackParam = EC_NULL;
#if (defined INCLUDE_FRAME_LOGGING)
    m_pvLogFrameCallBack        = EC_NULL;
    m_pvLogFrameCallBackContext = EC_NULL;
#endif

    m_oHotConnect.SetMaster(this, m_poEcDevice);

    m_dwBytesPerSecondCnt = 0;      /* bytes/per second */
    m_dwBytesPerSecondAct = 0;      /* TX bytes/second actual value */
    m_dwBytesPerSecondMin = 0;      /* TX bytes/second min. value   */
    m_dwBytesPerSecondMax = 0;      /* TX bytes/second max. value   */

    m_dwBytesPerCycleCnt  = 0;       /* bytes/per cycle */
    m_dwBytesPerCycleAct  = 0;       /* TX bytes/cycle actual value */
    m_dwBytesPerCycleMin  = 0;       /* TX bytes/cycle min. value   */
    m_dwBytesPerCycleMax  = 0;       /* TX bytes/cycle max. value   */

    m_bDLStatusIRQMasked  = EC_FALSE;
    m_bDLStatusIRQ        = EC_FALSE;
    m_bALStatusIRQ        = EC_FALSE;

#if (defined INCLUDE_VARREAD)
    /* We initialize the number of INPUT/OUTPUT entries to 0 but we increment it each time we add an entry.
       also see AddProcessVarInfoEntry() */
    m_dwProcVarInfoNumEntriesOutp = 0;
    m_dwProcVarInfoNumEntriesInp  = 0;

    /* add two more input variables for the devices state bus time variable eventually created if DC is activated */
    pDesc->dwProcessVarInfoNumEntriesInp += 2;
#endif

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
    memset(&m_oPtsControlDesc, 0, sizeof(m_oPtsControlDesc));
    m_oPtsControlDesc.eCurPtsState = ePtsStateNotRunning;
    m_oPtsControlDesc.eReqPtsState = ePtsStateNone;
#endif

#if (defined INCLUDE_AOE_SUPPORT)
    /* default AoE Net ID */
    m_oNetId.aby[0] = 1;
    m_oNetId.aby[1] = 1;
    m_oNetId.aby[2] = 1;
    m_oNetId.aby[3] = 1;
    m_oNetId.aby[4] = 1;
    m_oNetId.aby[5] = 1;
#endif

    m_eCycFrameLayout = CECMASTERCONST_m_eCycFrameLayout;

    /* master settings (properties) */
    m_dwMasterPropNumEntries = 0;
    m_dwMasterPropArraySize  = 0;
    m_aMasterPropEntries     = EC_NULL;

#if (defined INCLUDE_TRACE_DATA)
    m_dwTraceDataOffset = 0;
    m_wTraceDataSize = 0; /* will be set in SetFeatureField() */
#endif /* INCLUDE_TRACE_DATA */

#if (defined INCLUDE_MAILBOX_STATISTICS)
    OsMemset(&m_MailboxStatistics, 0, EC_MAILBOX_STATISTICS_CNT * sizeof(EC_T_STATISTIC_TRANSFER_DUPLEX));
    OsMemset(&m_MailboxStatisticsCur, 0, EC_MAILBOX_STATISTICS_CNT * sizeof(EC_T_STATISTIC_TRANSFER_DUPLEX_CUR));
#endif /* INCLUDE_MAILBOX_STATISTICS */

    CalculateEcatCmdTimeout(pMasterConfig->dwBusCycleTimeUsec);

    /* set feature field */
    if (EC_NULL != pFeatures)
    {
        SetFeatureField(pFeatures);
    }

    /* allocate memory for the process variable info entry pointers */
#if (defined INCLUDE_VARREAD)
    m_apProcVarInfoEntriesInp = EC_NULL;
    m_dwMaxProcVarInfoNumEntriesInp = pDesc->dwProcessVarInfoNumEntriesInp;
    if (m_dwMaxProcVarInfoNumEntriesInp > 0)
    {
        m_apProcVarInfoEntriesInp = (EC_T_PD_VAR_INFO_INTERN**)OsMalloc(sizeof(EC_T_PD_VAR_INFO_INTERN*) * m_dwMaxProcVarInfoNumEntriesInp);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_pProcVarInfoEntriesInp", m_apProcVarInfoEntriesInp, sizeof(EC_T_PD_VAR_INFO_INTERN*) * m_dwMaxProcVarInfoNumEntriesInp);
        OsDbgAssert(!(m_apProcVarInfoEntriesInp == EC_NULL));
    }

#if (defined INCLUDE_TRACE_DATA_VARINFO)
    if (m_wTraceDataSize > 0)
    {
        pDesc->dwProcessVarInfoNumEntriesOutp++;
    }
#endif
    m_apProcVarInfoEntriesOutp = EC_NULL;
    m_dwMaxProcVarInfoNumEntriesOutp = pDesc->dwProcessVarInfoNumEntriesOutp;
    if (m_dwMaxProcVarInfoNumEntriesOutp > 0)
    {
        m_apProcVarInfoEntriesOutp = (EC_T_PD_VAR_INFO_INTERN**)OsMalloc(sizeof(EC_T_PD_VAR_INFO_INTERN*) * m_dwMaxProcVarInfoNumEntriesOutp);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_pProcVarInfoEntriesOutp", m_apProcVarInfoEntriesOutp, sizeof(EC_T_PD_VAR_INFO_INTERN*) * m_dwMaxProcVarInfoNumEntriesOutp);
        OsDbgAssert(!(m_apProcVarInfoEntriesOutp == EC_NULL));
    }
#endif /* INCLUDE_VARREAD */

#if (defined INCLUDE_FORCE_PROCESSDATA)
    m_wForceProcessDataMaxEntries = EC_NUMOF_FORCE_PROCESS_DATA_ENTRIES;
    m_wForceProcessDataEntries    = 0;

    m_pForceProcessDataList        = (EC_T_FORCE_RELEASE_PROCESS_DATA_INFO*)OsMalloc(sizeof(EC_T_FORCE_RELEASE_PROCESS_DATA_INFO) * m_wForceProcessDataMaxEntries);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_pForceProcessDataList", m_pForceProcessDataList, 
        sizeof(EC_T_FORCE_RELEASE_PROCESS_DATA_INFO) * m_wForceProcessDataMaxEntries);
    OsMemset(m_pForceProcessDataList, 0, sizeof(EC_T_FORCE_RELEASE_PROCESS_DATA_INFO) * m_wForceProcessDataMaxEntries);
    OsDbgAssert(!(m_pForceProcessDataList == EC_NULL));
#endif
    m_dwNotifySBMismatchMaskedCnt     = 0;
    m_dwNotifySBStatusMaskedCnt       = 0;
    m_dwNotifyTopoChangeDoneMaskedCnt = 0;
}

/********************************************************************************/
/** \brief destructor
*
*/

CEcMaster::~CEcMaster()
{
    EC_T_UINT           i                       = 0,
                        j                       = 0;
    CYCLIC_ENTRY_MGMT_DESC*    pEcCycCmdDesc           = EC_NULL;
    
    EC_TRACEMSG(EC_TRACE_CORE, ("CEcMaster::~CEcMaster()\n"));

    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_pEEEPRom", m_pEEEPRom, sizeof(CEcEEPRom));
    SafeDelete(m_pEEEPRom);

#if (defined INCLUDE_MASTER_OBD)
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_pSDOService", m_pSDOService, sizeof(CEcSdoServ));
    SafeDelete(m_pSDOService);
#endif

    if (m_aCyclicEntryMgmtDesc != EC_NULL)
    {
        EC_T_INT nNumCycEntries = 1;
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
        for (i = 0; i < (EC_T_UINT)m_dwNumCycConfigEntries; i++)
#else
        i = 0;
#endif
        {
            pEcCycCmdDesc = &m_aCyclicEntryMgmtDesc[i];
            for (j=0; j < pEcCycCmdDesc->wNumFrames; j++ )
            {
                if (m_eCycFrameLayout == eCycFrameLayout_STANDARD || m_eCycFrameLayout == eCycFrameLayout_DYNAMIC)
                {
                    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~CEcMaster", pEcCycCmdDesc->apEcCycFrameDesc[j]->pbyFrame, ETHERNET_MAX_FRAMEBUF_LEN);
                    SafeOsFree(pEcCycCmdDesc->apEcCycFrameDesc[j]->pbyFrame);
                }
                EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~CEcMaster", pEcCycCmdDesc->apEcCycFrameDesc[j], sizeof(EcCycFrameDesc));
                SafeOsFree(pEcCycCmdDesc->apEcCycFrameDesc[j]);
            }
            EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~CEcMaster", pEcCycCmdDesc->apEcCycFrameDesc, 
                pEcCycCmdDesc->wNumFrames * sizeof(EcCycFrameDesc*));
            SafeDeleteArray(pEcCycCmdDesc->apEcCycFrameDesc);

            EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~CEcMaster", m_aCyclicEntryMgmtDesc[i].apEcAllCycCmdConfigDesc,
                pEcCycCmdDesc->wTotalNumCmds * sizeof(EcCycCmdConfigDesc*));
            SafeDeleteArray(m_aCyclicEntryMgmtDesc[i].apEcAllCycCmdConfigDesc);
        }
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
        nNumCycEntries = m_dwNumCycConfigEntries;
#endif
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~CEcMaster", m_aCyclicEntryMgmtDesc, nNumCycEntries*sizeof(CYCLIC_ENTRY_MGMT_DESC));
        SafeDeleteArray(m_aCyclicEntryMgmtDesc);
    }
#if (defined INCLUDE_RED_DEVICE)
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~CEcMaster", m_pbyRedFrame, sizeof(ETHERNET_88A4_MAX_FRAME));
    SafeDeleteArray(m_pbyRedFrame);
#endif

    if (EC_NULL != m_ppInitCmds)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_ppInitCmds", m_ppInitCmds, m_wMaxNumInitCmds * sizeof(PEcInitCmdDesc));
        SafeDeleteArray(m_ppInitCmds);
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_aSlaveFrameDesc", m_aSlaveFrameDesc,
        m_MasterConfig.dwMaxQueuedEthFrames * sizeof(ECAT_SLAVEFRAME_DESC));
    SafeDeleteArray(m_aSlaveFrameDesc);
    for (i = 0; i < m_MasterConfig.dwMaxQueuedEthFrames; i++)
    {
        m_aoEcSlaveFrame[i].FreeInstance(m_MasterConfig.dwMaxQueuedEthFrames, GetMemLog());
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_aoEcSlaveFrame", m_aoEcSlaveFrame,
        m_MasterConfig.dwMaxQueuedEthFrames * sizeof(CEcSlaveFrame));
    SafeDeleteArray(m_aoEcSlaveFrame);

#if (defined INCLUDE_DC_SUPPORT)
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_pDistributedClks", m_pDistributedClks, sizeof(CEcDistributedClocks));
    SafeDelete(m_pDistributedClks);
#endif /* INCLUDE_DC_SUPPORT */

    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_pBusTopology", m_pBusTopology, sizeof(CEcBusTopology));
    SafeDelete(m_pBusTopology);

    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_pSlavesStateMachineFifo", m_pSlavesStateMachineFifo, sizeof(CFiFoListDyn<CEcSlave*>));
    SafeDelete(m_pSlavesStateMachineFifo);

    for (i = 0; i < m_nCfgSlaveCount; i++)
    {
        if (EC_NULL != m_ppEcSlave[i])
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~CEcMaster", m_ppEcSlave[i], m_ppEcSlave[i]->GetThisSize());
            SafeDelete(m_ppEcSlave[i]);
        }
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_ppEcMbSlave", m_ppEcMbSlave, m_wMaxNumSlaves * sizeof(CEcMbSlave*));
    SafeDeleteArray(m_ppEcMbSlave);
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_ppEcSlave", m_ppEcSlave, m_wMaxNumSlaves * sizeof(CEcSlave*));
    SafeDeleteArray(m_ppEcSlave);

    if (m_pMasterDesc && m_ppEcPortMBoxState)
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_ppEcPortMBoxState", m_ppEcPortMBoxState, 
            m_pMasterDesc->wSizeAddressMBoxStates * 8 * sizeof(CEcMbSlave*));
    SafeDeleteArray(m_ppEcPortMBoxState);

    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_pNotificationStore", m_pNotificationStore, sizeof(EC_T_NOTIFICATION_INTDESC)*EC_NOTIFICATION_BUFFERS);
    OsFree(m_pNotificationStore);
    m_pNotificationStore = EC_NULL;

#if (defined EVAL_VERSION)
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_pLkMemory", m_pLkMemory, 512);
    OsFree(m_pLkMemory);
    m_pLkMemory = EC_NULL;
#endif

#if (defined INCLUDE_VARREAD)
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_apProcVarInfoEntriesInp", m_apProcVarInfoEntriesInp,
        sizeof(EC_T_PD_VAR_INFO_INTERN*) * m_dwMaxProcVarInfoNumEntriesInp);
    SafeOsFree(m_apProcVarInfoEntriesInp);
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_apProcVarInfoEntriesOutp", m_apProcVarInfoEntriesOutp, 
        sizeof(EC_T_PD_VAR_INFO_INTERN*) * m_dwMaxProcVarInfoNumEntriesOutp);
    SafeOsFree(m_apProcVarInfoEntriesOutp);
    m_dwProcVarInfoNumEntriesOutp = 0;
    m_dwProcVarInfoNumEntriesInp  = 0;
#endif

#if (defined INCLUDE_WKCSTATE)
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_pbyDiagnosisImage", m_pbyDiagnosisImage, m_nDiagnosisImageSize);
    SafeOsFree(m_pbyDiagnosisImage);
    m_nDiagnosisImageSize = 0;
#endif /* INCLUDE_WKCSTATE */

#if (defined INCLUDE_FORCE_PROCESSDATA)
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_pForceProcessData", m_pForceProcessDataList, 
        sizeof(EC_T_FORCE_RELEASE_PROCESS_DATA_INFO) * m_wForceProcessDataMaxEntries);
    SafeOsFree(m_pForceProcessDataList);
#endif

    /* master settings (properties) */
    m_dwMasterPropNumEntries = 0;
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::~m_aMasterProps", m_aMasterPropEntries, 
        sizeof(EC_T_MASTER_PROP_DESC) * m_dwMasterPropArraySize);
    SafeOsFree(m_aMasterPropEntries);

    OsDeleteLock(m_poFreeSlaveFrameDescLock);
    OsDeleteLock(m_poEcTransferRawCmdLock);
}

#if (defined INCLUDE_OEM)
/********************************************************************************/
/** \brief Checks if OEM Key from parameter matches the OEM Key from emSetOemKey
*
* \return EC_E_NOERROR or EMRAS_E_OEM_KEY_MISMATCH if Master OEM Key not set or differs from Client
*/
EC_T_DWORD CEcMaster::CheckOemKey(EC_T_UINT64 qwOemKey)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (m_qwOemKey != qwOemKey)
    {
        if (0 == m_qwOemKey)
        {
            dwRetVal = EC_E_OEM_KEY_MISSING;
            goto Exit;
        }

        OsSleep(1000);
        dwRetVal = EC_E_OEM_KEY_MISMATCH;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}
#endif /* INCLUDE_OEM */

/********************************************************************************/
/** \brief Set EtherCAT master default timeout values
*
* \return EC_E_NOERROR
*/
EC_T_DWORD CEcMaster::SetDefaultTimeouts(EC_T_MASTERDEFAULTTIMEOUTS_DESC* pDefaultTimeoutsDesc)
{
    if (EC_NULL == pDefaultTimeoutsDesc)
    {
        m_dwStateChangeDefaultTimeout = 0;
        m_dwInitCmdRetryTimeout = 0;
        m_dwMbxCmdTimeout = 0;
        m_dwDefaultMbxPolling = 0;
#if (defined INCLUDE_DC_SUPPORT)
        m_dwDcmInSyncTimeout = 0;
#endif
    }
    else
    {
        m_dwStateChangeDefaultTimeout = pDefaultTimeoutsDesc->dwMasterStateChange;
        m_dwInitCmdRetryTimeout = pDefaultTimeoutsDesc->dwInitCmdRetry;
        m_dwMbxCmdTimeout = pDefaultTimeoutsDesc->dwMbxCmd;
        m_dwDefaultMbxPolling = pDefaultTimeoutsDesc->dwMbxPolling;
#if (defined INCLUDE_DC_SUPPORT)
        m_dwDcmInSyncTimeout = pDefaultTimeoutsDesc->dwDcmInSync;
#endif
    }

    if (0 == m_dwStateChangeDefaultTimeout)
    {
        m_dwStateChangeDefaultTimeout = CECMASTERCONST_m_dwStateChangeDefaultTimeout;
    }
    if (0 == m_dwInitCmdRetryTimeout)
    {
        m_dwInitCmdRetryTimeout = CECMASTERCONST_m_dwInitCmdRetryTimeout;
    }
    if (0 == m_dwMbxCmdTimeout)
    {
        m_dwMbxCmdTimeout = CECMASTERCONST_m_dwMbxCmdTimeout;
    }
    if (0 == m_dwDefaultMbxPolling)
    {
        m_dwDefaultMbxPolling = CECMASTERCONST_m_dwDefaultMbxPolling;
    }
#if (defined INCLUDE_DC_SUPPORT)
    if (0 == m_dwDcmInSyncTimeout)
    {
        m_dwDcmInSyncTimeout = CECMASTERCONST_m_dwDcmInSyncTimeout;
    }
#endif
    return EC_E_NOERROR;
}

EC_T_VOID CEcMaster::CalculateEcatCmdTimeout(EC_T_DWORD dwBusCycleTimeUsec)
{
    if (0 == m_MasterConfig.dwEcatCmdTimeout)
    {
        m_dwEcatCmdTimeout = EC_MAX(1, EC_MIN(20, (dwBusCycleTimeUsec * 20) / 1000));
    }
    else
    {
        m_dwEcatCmdTimeout = m_MasterConfig.dwEcatCmdTimeout;
    }
}

#if (defined INCLUDE_SLAVE_STATISTICS)
/***************************************************************************************************/
/**
\brief  Clear all error registers in all slaves
*/
EC_T_VOID CEcMaster::ClearSlaveStatistics(EC_T_VOID)
{
EC_T_DWORD dwSlaveIdx = 0;

    for (dwSlaveIdx = 0; dwSlaveIdx < m_nCfgSlaveCount; dwSlaveIdx++)
    {
    CEcSlave* pCfgSlave = m_ppEcSlave[dwSlaveIdx];
    
        pCfgSlave->ClearStatisticCounters();
    }
    QueueRegisterCmdReq(
        EC_NULL, INVOKE_ID_GET_SLAVE_STATISTICS, EC_CMD_TYPE_BWR, 0,
        ECREG_SLV_RXERRCOUNTER, 0x14, EC_NULL
        );
}
/***************************************************************************************************/
/**
\brief  Sends datagrams to collect slave statistics counters.
\return EC_TRUE if done.
*/
EC_T_BOOL CEcMaster::ReadSlaveStatisticsSliced(EC_T_VOID)
{
    if (m_dwSlaveStatisticsSlaveIdxCur < m_nCfgSlaveCount)
    {
    CEcSlave* pCfgSlave = m_ppEcSlave[m_dwSlaveStatisticsSlaveIdxCur];

        QueueRegisterCmdReq(
            pCfgSlave, INVOKE_ID_GET_SLAVE_STATISTICS, EC_CMD_TYPE_FPRD, pCfgSlave->GetFixedAddr(),
            ECREG_SLV_RXERRCOUNTER, 0x14, EC_NULL
            );
        m_dwSlaveStatisticsSlaveIdxCur++;
    }
    if (m_dwSlaveStatisticsSlaveIdxCur == m_nCfgSlaveCount)
    {
        m_dwSlaveStatisticsSlaveIdxCur = 0;
        return EC_TRUE;
    }
    return EC_FALSE;
}
/***************************************************************************************************/
/**
\brief  Update Slave Statistics collection period.

This function implicitly forces an immediate collection of slave statistics if performed successful.
A Period of 0 disables automatic slave statistics collection.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::SetSlaveStatisticsPeriod(
    EC_T_DWORD  dwPeriod    /**< [in]   slave statistics collection period to set */
                                            )
{
    m_dwSlaveStatisticsPeriod = dwPeriod;
    if (0 == m_dwSlaveStatisticsPeriod)
    {
        m_oSlaveStatisticsTimer.Stop();
    }
    else
    {
        m_oSlaveStatisticsTimer.Start(m_dwSlaveStatisticsPeriod, GetMsecCounterPtr());
    }
    m_bSlaveStatisticsRequested = EC_TRUE;
    return EC_E_NOERROR;
}
#endif /* INCLUDE_SLAVE_STATISTICS */

/***************************************************************************************************/
/**
\brief  Log InitCmd sending as formatted string
*/
EC_T_VOID CEcMaster::LogInitCmd(EcInitCmdDesc* pInitCmdDesc, EC_T_WORD wTransition)
{
    if (m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE)
    {
        OsDbgMsg("InitCmd: %s, %s, %s, %d, 0x%04x, \"%s\", WKC %d\n",
            GetStateChangeNameShort(wTransition),
            (EC_ECINITCMDDESC_GET_MASTERINITCMD(pInitCmdDesc) ? "Master" : "Slave"),
            EcatCmdShortText(EC_AL_ICMDHDR_GET_CMDIDX_CMD(&pInitCmdDesc->EcCmdHeader)),
            (EC_T_INT)EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader)),
            (EC_T_INT)EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
            ((EC_ECINITCMDDESC_GET_CMTLEN(pInitCmdDesc) > 0) ? EcInitCmdDescComment(pInitCmdDesc) : ""),
            ((EC_T_INT)EC_ECINITCMDDESC_GET_CNT(pInitCmdDesc)));
    }
}

#if 0
/***************************************************************************************************/
EC_T_VOID CEcMaster::LogRescueScanGetNextPortToOpen(EC_T_WORD wAutoIncAddr, EC_T_WORD m_wFixedAddress, EC_T_WORD wPortIdx)
{
    if (m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE)
    {
        OsDbgMsg("RescueScanGetNextPortToOpen: %d, %d, %d\n", wAutoIncAddr, m_wFixedAddress, wPortIdx);
    }
}
#endif

/***************************************************************************************************/
/**
\brief  Notify all slave state changes since last NotifySlavesStateChanged
*/
EC_T_VOID CEcMaster::NotifySlavesStateChanged(EC_T_VOID)
{
    OsDbgAssert(0 != NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_STATECHANGED));
    /* prevent disabled or empty notification */
    if ((0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_STATECHANGED)))
        || (0 == m_SlavesStateChangedNotifyDesc.wCount))
        return;

    Notify(EC_NOTIFY_SLAVES_STATECHANGED, 0, (EC_T_NOTIFICATION_DATA*)&m_SlavesStateChangedNotifyDesc,
        SIZEOF_EC_T_SLAVES_STATECHANGED_NTFY_DESC(m_SlavesStateChangedNotifyDesc.wCount));

    /* reset notification data */
    OsMemset(&m_SlavesStateChangedNotifyDesc, 0, sizeof(EC_T_SLAVES_STATECHANGED_NTFY_DESC));
}


/***************************************************************************************************/
/**
\brief  Notify Master's state changed (EC_NOTIFY_STATECHANGED)
*/
EC_T_VOID CEcMaster::NotifyStateChanged(EC_T_STATE eOldState, EC_T_STATE eNewState)
{
    if (0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_STATECHANGED)))
    {
        EC_T_NOTIFICATION_INTDESC* pNotification = AllocNotification();
        if (EC_NULL != pNotification)
        {
            pNotification->uNotification.Notification.desc.StatusChngNtfyDesc.oldState = eOldState;
            pNotification->uNotification.Notification.desc.StatusChngNtfyDesc.newState = eNewState;
            NotifyAndFree(EC_NOTIFY_STATECHANGED, pNotification, sizeof(EC_T_STATECHANGE));
        }
    }
}

/***************************************************************************************************/
/**
\brief  Notify slave's state changed indirectly (EC_NOTIFY_SLAVES_STATECHANGED)
        or directly (EC_NOTIFY_SLAVE_STATECHANGED)
*/
EC_T_VOID CEcMaster::NotifySlaveStateChanged(CEcSlave* pCfgSlave)
{
    /* reset UnexpectedState information */
    pCfgSlave->m_wUnexpStateCurAlStatusNotified = DEVICE_STATE_UNKNOWN;
    pCfgSlave->m_wUnexpStateExpAlStatusNotified = DEVICE_STATE_UNKNOWN;

    if ((pCfgSlave->GetSmDeviceState() != DEVICE_STATE_UNKNOWN) && (pCfgSlave->m_wAlStatusNotified != pCfgSlave->GetAlStatus()))
    {
        /* EC_NOTIFY_SLAVE_STATECHANGED */
        if (0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVE_STATECHANGED)))
        {
            /* notify directly */
            EC_T_NOTIFICATION_INTDESC* pNotification = AllocNotification();
            if (EC_NULL != pNotification)
            {
                pNotification->uNotification.Notification.desc.SlaveStateChangedDesc.newState = pCfgSlave->GetEcatState();
                pCfgSlave->GetSlaveProp(&pNotification->uNotification.Notification.desc.SlaveStateChangedDesc.SlaveProp);
                NotifyAndFree(EC_NOTIFY_SLAVE_STATECHANGED, pNotification, sizeof(EC_T_SLAVE_STATECHANGED_NTFY_DESC));
            }
        }
        /* EC_NOTIFY_SLAVES_STATECHANGED */
        if (0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_STATECHANGED)))
        {
            /* set notification data */
            m_SlavesStateChangedNotifyDesc.SlaveStates[m_SlavesStateChangedNotifyDesc.wCount].wStationAddress = pCfgSlave->GetCfgFixedAddr();
            m_SlavesStateChangedNotifyDesc.SlaveStates[m_SlavesStateChangedNotifyDesc.wCount].byState = (EC_T_BYTE)pCfgSlave->GetEcatState();

            /* mark entry used in SlavesStateChangedNotifyDesc */
            m_SlavesStateChangedNotifyDesc.wCount++;

            /* Notify slave's state changed directly if notification descriptor full or waiting for next flush would likely take too long.
               Slaves' state changes will be notified anyway indirectly when Master State Machine gets stable. */
            if ((m_SlavesStateChangedNotifyDesc.wCount >= MAX_SLAVES_STATECHANGED_NTFY_ENTRIES)
                || (MasterStateMachIsStable() && (DEVICE_STATE_UNKNOWN != m_wCurMasterDeviceState)))
            {
                NotifySlavesStateChanged();
            }
        }
        pCfgSlave->m_wAlStatusNotified = pCfgSlave->GetAlStatus();
    }
}

/***************************************************************************************************/
/**
\brief  Notify all slaves in unexpected state since last NotifySlavesUnexpectedState
*/
EC_T_VOID CEcMaster::NotifySlavesUnexpectedState(EC_T_VOID)
{
    OsDbgAssert(0 != NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_UNEXPECTED_STATE));
    /* prevent disabled or empty notification */
    if ((0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_UNEXPECTED_STATE)))
        || (0 == m_SlavesUnexpectedStateNotifyDesc.desc.SlavesUnexpectedStateDesc.wCount))
        return;

    Notify(EC_NOTIFY_SLAVES_UNEXPECTED_STATE, 0, (EC_T_NOTIFICATION_DATA*)&m_SlavesUnexpectedStateNotifyDesc,
        SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVES_UNEXPECTED_STATE_DESC(m_SlavesUnexpectedStateNotifyDesc.desc.SlavesUnexpectedStateDesc.wCount));

    /* reset notification data */
    OsMemset(&m_SlavesUnexpectedStateNotifyDesc, 0, sizeof(EC_T_ERROR_NOTIFICATION_DESC));
}

/***************************************************************************************************/
/**
\brief  Notify slave's in unexpected state indirectly (EC_T_SLAVES_UNEXPECTED_STATE_DESC)
        or directly (EC_T_SLAVE_UNEXPECTED_STATE_DESC)
*/
EC_T_VOID CEcMaster::NotifySlaveUnexpectedState(CEcSlave* pCfgSlave, EC_T_STATE expState)
{
#ifdef INCLUDE_LOG_MESSAGES
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
    EC_T_STATE curState = pCfgSlave->GetEcatState();

    EC_T_WORD wCurState = (eEcatState_UNKNOWN == curState)?DEVICE_STATE_UNKNOWN:((EC_T_WORD)curState);
    EC_T_WORD wExpState = (eEcatState_UNKNOWN == expState)?DEVICE_STATE_UNKNOWN:((EC_T_WORD)expState);

    /* prevent notifying UNKNOWN expected state */
    if (eEcatState_UNKNOWN == expState)
    {
        return;
    }
    /* prevent notifying previous unexpected state again */
    if (  (pCfgSlave->m_wUnexpStateCurAlStatusNotified == wCurState)
       && (pCfgSlave->m_wUnexpStateExpAlStatusNotified == wExpState))
    {
        return;
    }
    /* prevent notifying same state */
    if (wCurState == wExpState)
    {
        return;
    }
    /* EC_T_SLAVE_UNEXPECTED_STATE_DESC */
    if (0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVE_UNEXPECTED_STATE)))
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+NOTIFICATION), "Notify EC_NOTIFY_SLAVE_UNEXPECTED_STATE (%d, %s, %s)\n", pCfgSlave->GetFixedAddr(), SlaveDevStateText(curState), SlaveDevStateText(expState)));

        /* notify directly */
        EC_T_NOTIFICATION_INTDESC* pNotification = AllocNotification();
        if (EC_NULL != pNotification)
        {
            pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_SLAVE_UNEXPECTED_STATE;

            pCfgSlave->GetSlaveProp(&pNotification->uNotification.ErrorNotification.desc.SlaveUnexpectedStateDesc.SlaveProp);
            pNotification->uNotification.ErrorNotification.desc.SlaveUnexpectedStateDesc.curState = curState;
            pNotification->uNotification.ErrorNotification.desc.SlaveUnexpectedStateDesc.expState = expState;

            NotifyAndFree(EC_NOTIFY_SLAVE_UNEXPECTED_STATE, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_UNEXPECTED_STATE);
        }
    }

    /* EC_T_SLAVES_UNEXPECTED_STATE_DESC */
    if (0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_UNEXPECTED_STATE)))
    {
        EC_T_SLAVES_UNEXPECTED_STATE_DESC* pNotifyDesc = EC_NULL;
        pNotifyDesc = &m_SlavesUnexpectedStateNotifyDesc.desc.SlavesUnexpectedStateDesc;

        /* set notification data */
        m_SlavesUnexpectedStateNotifyDesc.dwNotifyErrorCode = EC_NOTIFY_SLAVES_UNEXPECTED_STATE;
        pNotifyDesc->SlaveStates[pNotifyDesc->wCount].wStationAddress = pCfgSlave->GetCfgFixedAddr();
        pNotifyDesc->SlaveStates[pNotifyDesc->wCount].curState = curState;
        pNotifyDesc->SlaveStates[pNotifyDesc->wCount].expState = pCfgSlave->GetSmReqEcatState();

        /* mark entry used in SlavesUnexpectedStateNotifyDesc */
        pNotifyDesc->wCount++;

        /* Notify unexpected state directly if notification descriptor full or waiting for next flush would likely take too long.
           Unexpected states will be notified anyway indirectly when Master State Machine gets stable. */
        if ((pNotifyDesc->wCount >= MAX_SLAVES_UNEXPECTED_STATE_NTFY_ENTRIES)
            || (MasterStateMachIsStable() && (DEVICE_STATE_UNKNOWN != m_wCurMasterDeviceState)))
        {
            NotifySlavesUnexpectedState();
        }
    }
    /* prevent notifying this unexpected state again */
    pCfgSlave->m_wUnexpStateCurAlStatusNotified = wCurState;
    pCfgSlave->m_wUnexpStateExpAlStatusNotified = wExpState;

    /* assure next slave state notification */
    pCfgSlave->m_wAlStatusNotified = pCfgSlave->GetAlStatus();
}

/***************************************************************************************************/
/**
\brief  Notify slave (dis-)appeard indirectly (EC_T_SLAVES_PRESENCE_NTFY_DESC)
        or directly (EC_T_SLAVE_PRESENCE_NTFY_DESC)
        Replaces EC_NOTIFY_SLAVE_APPEARS, EC_NOTIFY_SLAVE_DISAPPEARS.
*/
EC_T_VOID CEcMaster::NotifySlavePresence(EC_T_WORD wStationAddress, EC_T_BOOL bPresent)
{
#ifdef INCLUDE_LOG_MESSAGES
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
    /* EC_NOTIFY_SLAVE_PRESENCE */
    if (0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVE_PRESENCE)))
    {
        /* notify directly */
        EC_T_NOTIFICATION_INTDESC* pNotification = AllocNotification();
        if (EC_NULL == pNotification)
            return;

        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+NOTIFICATION), "Notify EC_NOTIFY_SLAVE_PRESENCE (0x%04X, 0%d)\n", wStationAddress, bPresent));

        pNotification->uNotification.Notification.desc.SlavePresenceDesc.wStationAddress = wStationAddress;
        pNotification->uNotification.Notification.desc.SlavePresenceDesc.bPresent = EcBoolToByte(bPresent);
        NotifyAndFree(EC_NOTIFY_SLAVE_PRESENCE, pNotification, sizeof(EC_T_SLAVE_PRESENCE_NTFY_DESC));
    }

    /* EC_NOTIFY_SLAVES_PRESENCE */
    if (0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_PRESENCE)))
    {
        /* set notification data */
        m_SlavesPresenceNotifyDesc.SlavePresence[m_SlavesPresenceNotifyDesc.wCount].wStationAddress = wStationAddress;
        m_SlavesPresenceNotifyDesc.SlavePresence[m_SlavesPresenceNotifyDesc.wCount].bPresent = EcBoolToByte(bPresent);

        /* mark entry used in SlavesStateChangedNotifyDesc */
        m_SlavesPresenceNotifyDesc.wCount++;

        /* Notify slave's presence directly if notification descriptor full or waiting for next flush would likely take too long.
           Slaves' presence will be notified anyway indirectly when Master State Machine gets stable. */
        if ((m_SlavesPresenceNotifyDesc.wCount >= MAX_SLAVES_PRESENCE_NTFY_ENTRIES)
            || (MasterStateMachIsStable() && (DEVICE_STATE_UNKNOWN != m_wCurMasterDeviceState)))
        {
            NotifySlavesPresence();
        }
    }
}

/***************************************************************************************************/
/**
\brief  Notify all slaves (dis-)appeared since last NotifySlavesPresence
*/
EC_T_VOID CEcMaster::NotifySlavesPresence(EC_T_VOID)
{
    OsDbgAssert(0 != NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_PRESENCE));
    /* prevent disabled or empty notification */
    if ((0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_PRESENCE)))
        || (0 == m_SlavesPresenceNotifyDesc.wCount))
        return;

    Notify(EC_NOTIFY_SLAVES_PRESENCE, 0, (EC_T_NOTIFICATION_DATA*)&m_SlavesPresenceNotifyDesc,
        SIZEOF_EC_T_SLAVES_PRESENCE_NTFY_DESC(m_SlavesPresenceNotifyDesc.wCount));

    /* reset notification data */
    OsMemset(&m_SlavesPresenceNotifyDesc, 0, sizeof(EC_T_SLAVES_PRESENCE_NTFY_DESC));
}

/***************************************************************************************************/
/**
\brief  Notify slave error indirectly (EC_T_SLAVES_ERROR_DESC)
        or directly (EC_T_SLAVE_ERROR_INFO_DESC)
*/
EC_T_VOID CEcMaster::NotifySlaveError(CEcSlave* pCfgSlave, EC_T_WORD wAlStatus, EC_T_WORD wAlStatusCode)
{
    /* EC_NOTIFY_SLAVE_ERROR_STATUS_INFO */
    if (0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVE_ERROR_STATUS_INFO)))
    {
        /* notify directly */
        EC_T_NOTIFICATION_INTDESC* pNotification = AllocNotification();
        if (EC_NULL == pNotification)
            return;

        pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_SLAVE_ERROR_STATUS_INFO;
        pCfgSlave->GetSlaveProp(&pNotification->uNotification.ErrorNotification.desc.SlaveErrInfoDesc.SlaveProp);
        pNotification->uNotification.ErrorNotification.desc.SlaveErrInfoDesc.wStatus     = wAlStatus;
        pNotification->uNotification.ErrorNotification.desc.SlaveErrInfoDesc.wStatusCode = wAlStatusCode;
        NotifyAndFree(EC_NOTIFY_SLAVE_ERROR_STATUS_INFO, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_ERROR_INFO);
    }

    /* EC_NOTIFY_SLAVES_ERROR_STATUS */
    if (0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_ERROR_STATUS)))
    {
        /* set notification data */
        m_SlavesErrorNotifyDesc.desc.SlavesErrDesc.SlaveError[m_SlavesErrorNotifyDesc.desc.SlavesErrDesc.wCount].wStationAddress = pCfgSlave->GetFixedAddr();
        m_SlavesErrorNotifyDesc.desc.SlavesErrDesc.SlaveError[m_SlavesErrorNotifyDesc.desc.SlavesErrDesc.wCount].wStatus = wAlStatus;
        m_SlavesErrorNotifyDesc.desc.SlavesErrDesc.SlaveError[m_SlavesErrorNotifyDesc.desc.SlavesErrDesc.wCount].wStatusCode = wAlStatusCode;

        /* mark entry used in SlavesErrDesc */
        m_SlavesErrorNotifyDesc.desc.SlavesErrDesc.wCount++;

        /* Notify slave's error directly if notification descriptor full or waiting for next flush would likely take too long.
           Slaves' error will be notified anyway indirectly when Master State Machine gets stable. */
        if ((m_SlavesPresenceNotifyDesc.wCount >= MAX_SLAVES_ERROR_NTFY_ENTRIES)
            || (MasterStateMachIsStable() && (DEVICE_STATE_UNKNOWN != m_wCurMasterDeviceState)))
        {
            NotifySlavesError();
        }
    }
}

/***************************************************************************************************/
/**
\brief  Notify all slave errors since last NotifySlavesError
*/
EC_T_VOID CEcMaster::NotifySlavesError(EC_T_VOID)
{
    OsDbgAssert(0 != NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_ERROR_STATUS));
    /* prevent disabled or empty notification */
    if ((0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVES_ERROR_STATUS)))
        || (0 == m_SlavesErrorNotifyDesc.desc.SlavesErrDesc.wCount))
        return;

    m_SlavesErrorNotifyDesc.dwNotifyErrorCode = EC_NOTIFY_SLAVES_ERROR_STATUS;
    Notify(EC_NOTIFY_SLAVES_ERROR_STATUS, 0, (EC_T_NOTIFICATION_DATA*)&m_SlavesErrorNotifyDesc,
        SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVES_ERROR_DESC(m_SlavesErrorNotifyDesc.desc.SlavesErrDesc.wCount));

    /* reset notification data */
    OsMemset(&m_SlavesErrorNotifyDesc, 0, sizeof(EC_T_ERROR_NOTIFICATION_DESC));
}

#if (defined INCLUDE_JUNCTION_REDUNDANCY)
/***************************************************************************************************/
/**
\brief  Notify junction redundancy line break change
*/
EC_T_VOID CEcMaster::NotifyJunctionRedChange(CEcBusSlave* pBusSlave)
{
EC_T_NOTIFICATION_INTDESC*     pNotification          = EC_NULL;
EC_T_JUNCTION_RED_CHANGE_DESC* pJunctionRedChangeDesc = EC_NULL;

    if (0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_JUNCTION_RED_CHANGE)))
        return;

    pNotification = AllocNotification();
    if (EC_NULL != pNotification)
    {
        pJunctionRedChangeDesc = &(pNotification->uNotification.ErrorNotification.desc.JunctionRedChangeDesc);
        pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_JUNCTION_RED_CHANGE;

        pBusSlave->GetSlaveProp(&pJunctionRedChangeDesc->SlaveProp);
        pJunctionRedChangeDesc->bLineBreak = !pBusSlave->IsSlaveConPortX(ESC_PORT_A);
        NotifyAndFree(EC_NOTIFY_JUNCTION_RED_CHANGE, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_JUNCTION_RED_CHANGE);
    }
}
#endif /* INCLUDE_JUNCTION_REDUNDANCY */

EC_T_VOID CEcMaster::NotifyTopologyChangeDone(EC_T_DWORD dwStatusCode)
{
#ifdef INCLUDE_LOG_MESSAGES
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
    if ((0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_HC_TOPOCHGDONE))) && (0 == m_dwNotifyTopoChangeDoneMaskedCnt))
    {
        EC_T_NOTIFICATION_INTDESC* pNotification = AllocNotification();
        if (EC_NULL == pNotification)
            return;

        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+NOTIFICATION), "Notify EC_NOTIFY_HC_TOPOCHGDONE (0x%08X)\n", dwStatusCode));

        pNotification->uNotification.Notification.desc.StatusCode = dwStatusCode;
        NotifyAndFree(EC_NOTIFY_HC_TOPOCHGDONE, pNotification, sizeof(EC_T_DWORD));
    }
}

EC_T_VOID CEcMaster::NotifyScanBusStatus(EC_T_DWORD dwResultCode, EC_T_DWORD dwSlaveCount)
{
#ifdef INCLUDE_LOG_MESSAGES
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_MasterConfig.dwStateChangeDebug >> 2) &3);
#endif
    if ((0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SB_STATUS))) && (0 == m_dwNotifySBStatusMaskedCnt))
    {
        EC_T_NOTIFICATION_INTDESC* pNotification = AllocNotification();
        if (EC_NULL == pNotification)
            return;

        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+NOTIFICATION), "Notify EC_NOTIFY_SB_STATUS (0x%08X, %d)\n", dwResultCode, dwSlaveCount));

        pNotification->uNotification.Notification.desc.ScanBusNtfyDesc.dwResultCode = dwResultCode;
        pNotification->uNotification.Notification.desc.ScanBusNtfyDesc.dwSlaveCount = dwSlaveCount;
        NotifyAndFree(EC_NOTIFY_SB_STATUS, pNotification, sizeof(EC_T_SB_STATUS_NTFY_DESC));
    }
}

static EC_INLINESTART EC_T_VOID FillMismatchNotification(EC_T_SB_MISMATCH_DESC* pMisMatch, CEcBusSlave* poBusSlave, CEcBusSlave* poBusSlavePrev, EC_T_WORD wPortPrev, CEcSlave* poCfgSlave)
{
    OsMemset(pMisMatch, 0, sizeof(EC_T_SB_MISMATCH_DESC));

    if (EC_NULL != poBusSlave)
    {
        pMisMatch->wBusAIncAddress = poBusSlave->GetAutoIncAddress();
        pMisMatch->dwBusVendorId = poBusSlave->GetVendorId();
        pMisMatch->dwBusProdCode = poBusSlave->GetProductCode();
        pMisMatch->dwBusRevisionNo = poBusSlave->GetRevisionNo();
        pMisMatch->dwBusSerialNo = poBusSlave->GetSerialNo();
        pMisMatch->wBusFixedAddress = poBusSlave->GetFixedAddress();
#if (defined INCLUDE_HOTCONNECT)
        pMisMatch->wIdentificationVal = poBusSlave->GetIdentifyData();
#endif
        /* fill information about mismatching identification */
        if (EC_NULL != poBusSlave->m_pCfgSlaveIdentMisMatch)
        {
            EC_T_WORD wIdentValue = 0;
            ETYPE_EC_CMD_HEADER* pCmdHdr = (ETYPE_EC_CMD_HEADER*)&pMisMatch->oIdentCmdHdr;

            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, poBusSlave->m_wIdentificationAdo);
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, poBusSlave->GetFixedAddress());
            pMisMatch->dwCmdData = poBusSlave->m_wIdentificationValue;
            pMisMatch->dwCmdVMask = 0xFFFF;
            poBusSlave->m_pCfgSlaveIdentMisMatch->GetIdentification(EC_NULL, &wIdentValue);
            pMisMatch->dwCmdVData = wIdentValue;
            pMisMatch->bIdentValidationError = EC_TRUE;
        }
    }
    else
    {
        pMisMatch->wBusAIncAddress = INVALID_AUTO_INC_ADDR;
        pMisMatch->wBusFixedAddress = INVALID_FIXED_ADDR;
    }

    if (EC_NULL != poBusSlavePrev)
    {
        CEcSlave* poCfgSlavePrev = poBusSlavePrev->GetCfgSlave();
        /* returns the configured fixed address, because temporary address is a confusing information */
        if (EC_NULL != poCfgSlavePrev)
        {
            pMisMatch->wPrevFixedAddress = poCfgSlavePrev->GetCfgFixedAddr();
        }
        else
        {
            pMisMatch->wPrevFixedAddress = poBusSlavePrev->GetFixedAddress();
        }
        pMisMatch->wPrevPort = wPortPrev;
        pMisMatch->wPrevAIncAddress = poBusSlavePrev->GetAutoIncAddress();
    }
    else
    {
        pMisMatch->wPrevAIncAddress = INVALID_AUTO_INC_ADDR;
        pMisMatch->wPrevFixedAddress = INVALID_FIXED_ADDR;
        pMisMatch->wPrevPort = ESC_PORT_INVALID;
    }

    if (EC_NULL != poCfgSlave)
    {
        pMisMatch->dwCfgVendorId = poCfgSlave->GetVendorId();
        pMisMatch->dwCfgProdCode = poCfgSlave->GetProductCode();
        pMisMatch->dwCfgRevisionNo = poCfgSlave->GetRevisionNumber();
        pMisMatch->dwCfgSerialNo = poCfgSlave->GetSerialNumber();
        pMisMatch->wCfgFixedAddress = poCfgSlave->GetFixedAddr();
        pMisMatch->wCfgAIncAddress = poCfgSlave->GetCfgAutoIncAddress();
    }
    else
    {
        pMisMatch->wCfgAIncAddress = INVALID_AUTO_INC_ADDR;
        pMisMatch->wCfgFixedAddress = INVALID_FIXED_ADDR;
    }

} EC_INLINESTOP

EC_T_VOID CEcMaster::NotifyScanBusMismatch(CEcBusSlave* poBusSlave, CEcBusSlave* poBusSlavePrev, EC_T_WORD wPortPrev, CEcSlave* poCfgSlave)
{
#ifdef INCLUDE_LOG_MESSAGES
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_MasterConfig.dwStateChangeDebug >> 2) & 3);
#endif
    if ((0 != (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SB_MISMATCH))) && (0 == m_dwNotifySBMismatchMaskedCnt))
    {
        EC_T_SB_MISMATCH_DESC* pMisMatch = EC_NULL;
        EC_T_NOTIFICATION_INTDESC* pNotification = AllocNotification();
        if (EC_NULL == pNotification)
            return;

        pMisMatch = &(pNotification->uNotification.Notification.desc.ScanBusMismatch);
        FillMismatchNotification(pMisMatch, poBusSlave, poBusSlavePrev, wPortPrev, poCfgSlave);

        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + NOTIFICATION), "Notify EC_NOTIFY_SB_MISMATCH(%d, %d, %d, %d)\n", 
            pMisMatch->wBusFixedAddress, pMisMatch->wPrevFixedAddress, wPortPrev, pMisMatch->wCfgFixedAddress));

        NotifyAndFree(EC_NOTIFY_SB_MISMATCH, pNotification, sizeof(EC_T_SB_MISMATCH_DESC));
    }
}
#if (defined INCLUDE_HOTCONNECT)
EC_T_VOID CEcMaster::NotifyScanBusDuplicateHcNode(CEcBusSlave* poBusSlave, CEcBusSlave* poBusSlavePrev, EC_T_WORD wPortPrev, CEcSlave* poCfgSlave)
{
#ifdef INCLUDE_LOG_MESSAGES
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_MasterConfig.dwStateChangeDebug >> 2) & 3);
#endif
    EC_T_SB_MISMATCH_DESC* pMisMatch = EC_NULL;
    EC_T_NOTIFICATION_INTDESC* pNotification = AllocNotification();
    if (EC_NULL == pNotification)
        return;

    pMisMatch = &(pNotification->uNotification.Notification.desc.ScanBusMismatch);
    FillMismatchNotification(pMisMatch, poBusSlave, poBusSlavePrev, wPortPrev, poCfgSlave);

    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + NOTIFICATION), "Notify EC_NOTIFY_SB_DUPLICATE_HC_NODE(%d, %d, %d, %d)\n",
        pMisMatch->wBusFixedAddress, pMisMatch->wPrevFixedAddress, wPortPrev, pMisMatch->wCfgFixedAddress));

    NotifyAndFree(EC_NOTIFY_SB_DUPLICATE_HC_NODE, pNotification, sizeof(EC_T_SB_MISMATCH_DESC));
}
#endif /* INCLUDE_HOTCONNECT */

EC_T_VOID CEcMaster::NotifyFrameResponseError(EC_T_CHAR chErrInf0, EC_T_CHAR chErrInf1, EC_T_BOOL bIsCyclicFrame, EC_T_FRAME_RSPERR_TYPE eErrorType, EC_T_BYTE byIdxSet, EC_T_BYTE byIdxAct)
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;

    if (m_dwFrameResponseNotifyMaskedCnt != 0)
    {
        return;
    }

    if (bIsCyclicFrame)
    {
        /* mask cyclic eRspErr_UNEXPECTED */
        if (eRspErr_UNEXPECTED == eErrorType)
        {
            if (0 == (m_dwCycErrorNotifyMask & EC_CYC_ERR_MASK_UNEXPECTED_FRAME_RESPONSE))
                return;
        }
        /* mask and log cyclic eRspErr_NO_RESPONSE */
        if (eRspErr_NO_RESPONSE == eErrorType)
        {
            if (0 == (m_dwCycErrorNotifyMask & EC_CYC_ERR_MASK_NO_FRAME_RESPONSE_ERROR))
                return;
#ifdef DEBUG
            {
                static EC_T_DWORD m_dwCycErrPrintTime = 0;
                if (OsQueryMsecCount() > m_dwCycErrPrintTime + 2000)
                {
                    EC_ERRORMSGC(("ERROR: Missing cyclic command response!\n"));
                    m_dwCycErrPrintTime = OsQueryMsecCount();
                }
            }
#endif
            EC_ERRORMSGL(("ERROR: Missing cyclic command response!\n"));
        }
    }
    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
    if (EC_NULL == pNotification)
        return;

    pNotification->uNotification.ErrorNotification.dwNotifyErrorCode                        = EC_NOTIFY_FRAME_RESPONSE_ERROR;
    pNotification->uNotification.ErrorNotification.achErrorInfo[0]                          = chErrInf0;
    pNotification->uNotification.ErrorNotification.achErrorInfo[1]                          = chErrInf1;
    pNotification->uNotification.ErrorNotification.desc.FrameRspErrDesc.bIsCyclicFrame      = bIsCyclicFrame;
    pNotification->uNotification.ErrorNotification.desc.FrameRspErrDesc.EErrorType          = eErrorType;
    pNotification->uNotification.ErrorNotification.desc.FrameRspErrDesc.byEcCmdHeaderIdxSet = byIdxSet;
    pNotification->uNotification.ErrorNotification.desc.FrameRspErrDesc.byEcCmdHeaderIdxAct = byIdxAct;
    NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_FRAME_RSPERR);
}

EC_T_VOID CEcMaster::NotifyFramelossAfterSlave(CEcBusSlave* pBusSlave, EC_T_WORD wPortIdx)
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;

    if (0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_FRAMELOSS_AFTER_SLAVE)))
        return;

    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
    if (EC_NULL == pNotification)
        return;

    if (EC_NULL != pBusSlave)
    {
        pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_FRAMELOSS_AFTER_SLAVE;
        pBusSlave->GetSlaveProp(&pNotification->uNotification.ErrorNotification.desc.FramelossAfterSlaveDesc.SlaveProp);
        pNotification->uNotification.ErrorNotification.desc.FramelossAfterSlaveDesc.wPort = wPortIdx;

        if (m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_ERROR)
        {
            OsDbgMsg("Notify EC_NOTIFY_FRAMELOSS_AFTER_SLAVE(%d, %d, %d)\n", 
                pNotification->uNotification.ErrorNotification.desc.FramelossAfterSlaveDesc.SlaveProp.wAutoIncAddr, 
                pNotification->uNotification.ErrorNotification.desc.FramelossAfterSlaveDesc.SlaveProp.wStationAddress, 
                pNotification->uNotification.ErrorNotification.desc.FramelossAfterSlaveDesc.wPort);
        }

        NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_FRAMELOSS_AFTER_SLAVE);
    }
}

EC_T_VOID CEcMaster::NotifySlaveCoEInitCmd(CEcSlave* pCfgSlave, EC_T_WORD wCurrTransition, EcMailboxCmdDesc* pMbxInitCmd, EC_T_DWORD dwResult, EC_T_BYTE* pbyData, EC_T_WORD wDataLen)
{
EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;

    if (0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_COE_INIT_CMD)))
        return;

    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
    if (EC_NULL == pNotification)
        return;

    pCfgSlave->GetSlaveProp(&pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.SlaveProp);
    pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.dwHandle = EC_ECMBXCMDDESC_GET_HANDLE(pMbxInitCmd);
    pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.wTransition = wCurrTransition;
    SAFE_STRCPY(pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.szComment, EcMailboxCmdDescComment(pMbxInitCmd), EC_MIN(EC_ECMBXCMDDESC_GET_CMTLEN(pMbxInitCmd) + 1, MAX_STD_STRLEN - 1));
    pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.dwErrorCode = dwResult;

    pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.bFixed = (0 != EC_ECMBXCMDDESC_GET_FIXED(pMbxInitCmd));
    pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.byCcs = pMbxInitCmd->uMbxHdr.coe.EcSdoHeader.uHdr.Abt.Ccs;
    pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.bCompleteAccess = pMbxInitCmd->uMbxHdr.coe.EcSdoHeader.uHdr.Iuq.Complete;
    pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.wIndex = EC_ECSDOHDR_GET_INDEX(&pMbxInitCmd->uMbxHdr.coe.EcSdoHeader);
    pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.bySubIndex = pMbxInitCmd->uMbxHdr.coe.EcSdoHeader.SubIndex;
    pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.dwDataLen = wDataLen;
    pNotification->uNotification.MbxTfer.MbxData.CoE_InitCmd.pbyData = pbyData;

    NotifyAndFree(EC_NOTIFY_COE_INIT_CMD, pNotification, sizeof(EC_T_MBXTFER));
}

EC_T_VOID CEcMaster::NotifySlaveInitCmdResponseError(CEcSlave* pCfgSlave, EC_T_WORD wCurrTransition, EC_T_INITCMD_ERR_TYPE EErrorType, EC_T_CHAR* achComment, EC_T_DWORD dwCommentLen)
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;

    if (0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR)))
        return;

    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
    if (EC_NULL == pNotification)
        return;

    pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR;
    pNotification->uNotification.ErrorNotification.achErrorInfo[0]   = 0;
    pNotification->uNotification.ErrorNotification.achErrorInfo[1]   = 0;

    OsMemset(&pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc, 0, sizeof(EC_T_INITCMD_ERR_DESC));
    pCfgSlave->GetSlaveProp(&pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.SlaveProp);

    SAFE_STRCPY(pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.achStateChangeName, GetStateChangeName(wCurrTransition), MAX_SHORT_STRLEN - 1);

    pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.EErrorType = EErrorType;

    SAFE_STRCPY(pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.szComment, achComment, EC_MIN(dwCommentLen + 1, MAX_STD_STRLEN - 1));
    pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.szComment[EC_MIN(dwCommentLen, MAX_STD_STRLEN - 1)] = '\0';

    NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_INITCMD_RESPONSE_ERROR);
}

EC_T_VOID CEcMaster::NotifyRefClockPresence(CEcBusSlave* pSlave)
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;

    if (0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_REFCLOCK_PRESENCE)))
        return;

    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
    if (EC_NULL == pNotification)
        return;

    OsMemset(&pNotification->uNotification.Notification.desc.RefClockPresenceNtfyDesc, 0, sizeof(EC_T_REFCLOCK_PRESENCE_NTFY_DESC));
    if (EC_NULL != pSlave)
    {
        pNotification->uNotification.Notification.desc.RefClockPresenceNtfyDesc.bPresent = EC_TRUE;
        pSlave->GetSlaveProp(&pNotification->uNotification.Notification.desc.RefClockPresenceNtfyDesc.SlaveProp);
    }

    NotifyAndFree(EC_NOTIFY_REFCLOCK_PRESENCE, pNotification, sizeof(EC_T_REFCLOCK_PRESENCE_NTFY_DESC));
}
EC_T_VOID CEcMaster::NotifyDcStatus(EC_T_DWORD dwStatusCode)
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;

    if (0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_DC_STATUS)))
        return;

    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
    if (EC_NULL == pNotification)
        return;

    pNotification->uNotification.Notification.desc.StatusCode = dwStatusCode;
    NotifyAndFree(EC_NOTIFY_DC_STATUS, pNotification, sizeof(EC_T_DWORD));
}
EC_T_VOID CEcMaster::NotifyDcSlvStatus(EC_T_BOOL bInSync, EC_T_BOOL bDeviationIsNegative, EC_T_DWORD dwDeviation, CEcBusSlave* pBusSlave)
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;
    EC_T_DC_SYNC_NTFY_DESC*    pSyncNtfyDesc = EC_NULL;

    if (0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_DC_SLV_SYNC)))
        return;

    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
    if (EC_NULL == pNotification)
        return;

    pSyncNtfyDesc = &(pNotification->uNotification.Notification.desc.SyncNtfyDesc);

    pSyncNtfyDesc->IsInSync                  = bInSync;
    pSyncNtfyDesc->IsNegative                = bDeviationIsNegative;
    pSyncNtfyDesc->dwDeviation               = dwDeviation;

    pSyncNtfyDesc->SlaveProp.achName[0]      = '\0';
    pSyncNtfyDesc->SlaveProp.wAutoIncAddr    = 0;
    pSyncNtfyDesc->SlaveProp.wStationAddress = 0;

    if (EC_NULL != pBusSlave)
    {
        pBusSlave->GetSlaveProp(&pSyncNtfyDesc->SlaveProp);
    }

    NotifyAndFree(EC_NOTIFY_DC_SLV_SYNC, pNotification, sizeof(EC_T_DC_SYNC_NTFY_DESC));
}
EC_T_VOID CEcMaster::NotifyDcmSync(EC_T_BOOL bInSync, EC_T_INT nCtlErrorNsecCur, EC_T_INT nCtlErrorNsecAvg, EC_T_INT nCtlErrorNsecMax)
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;
    EC_T_DCM_SYNC_NTFY_DESC*   pDcmInSyncNotify = EC_NULL;

    if (0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_DCM_SYNC)))
        return;

    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
    if (EC_NULL == pNotification)
        return;

    pDcmInSyncNotify = &(pNotification->uNotification.Notification.desc.DcmInSyncDesc);

    pDcmInSyncNotify->IsInSync         = bInSync;
    pDcmInSyncNotify->nCtlErrorNsecCur = nCtlErrorNsecCur;
    pDcmInSyncNotify->nCtlErrorNsecAvg = nCtlErrorNsecAvg;
    pDcmInSyncNotify->nCtlErrorNsecMax = nCtlErrorNsecMax;
    NotifyAndFree(EC_NOTIFY_DCM_SYNC, pNotification, sizeof(EC_T_DCM_SYNC_NTFY_DESC));
}

#if (defined INCLUDE_DCX)
EC_T_VOID CEcMaster::NotifyDcxSync(EC_T_DCX_SYNC_NTFY_DESC* pDcxInSyncNotify)
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;

    if (0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_DCX_SYNC)))
        return;

    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
    if (EC_NULL == pNotification)
        return;

    OsMemcpy(&pNotification->uNotification.Notification.desc.DcxInSyncDesc, pDcxInSyncNotify, sizeof(EC_T_DCX_SYNC_NTFY_DESC));

    NotifyAndFree(EC_NOTIFY_DCX_SYNC, pNotification, sizeof(EC_T_DCX_SYNC_NTFY_DESC));
}
#endif /*INCLUDE_DCX*/

EC_T_VOID CEcMaster::NotifyAllDevicesOperational(EC_T_BOOL bAllDevicesOperational)
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_MasterConfig.dwStateChangeDebug >> 0) & 3);
#endif
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;
    EC_T_DWORD dwNotifyCode = bAllDevicesOperational ? EC_NOTIFY_ALL_DEVICES_OPERATIONAL : EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL;

    /* filter notification */
    if (bAllDevicesOperational)
    {
        if (0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_ALL_DEVICES_OPERATIONAL)))
            goto Exit;

        /* EC_NOTIFY_ALL_DEVICES_OPERATIONAL only notified after EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL */
        if (!m_bNotAllDevOpNotified)
        {
            goto Exit;
        }
    }
    else
    {
        if (0 == (m_dwNotificationMask & NotificationCodeToNotificationMask(EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL)))
            goto Exit;

        /* notified EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL only once */
        if (m_bNotAllDevOpNotified)
        {
            goto Exit;
        }
#if (defined INCLUDE_HOTCONNECT)
        /* EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL can be masked by HotConnect */
        if (!m_bNotAllDevOpNotificationEnabled)
        {
            goto Exit;
        }
#endif
    }
    pNotification = AllocNotification();
    if (EC_NULL != pNotification)
    {
        pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = dwNotifyCode;
        NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, sizeof(EC_T_DWORD));
    }
#if (defined INCLUDE_LOG_MESSAGES)
    if (!bAllDevicesOperational)
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + NOTIFICATION), "Notify EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL\n"));
    }
    else
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + NOTIFICATION), "Notify EC_NOTIFY_ALL_DEVICES_OPERATIONAL\n"));
    }
#endif
    /* notice "not all devices operational" notified */
    m_bNotAllDevOpNotified = !bAllDevicesOperational;
Exit:
    return;
}

#if (defined INCLUDE_FOE_SUPPORT)
EC_T_VOID CEcMaster::NotifyMbxTferProgress(EC_T_MBXTFERP* pMbxTferPriv)
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;

    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
    if ((EC_NULL == pNotification) || (0 == pMbxTferPriv->MbxTfer.dwClntId))
    {
        if (pNotification != NULL)
        {
           FreeNotification(pNotification);
        }
        return;
    }

    OsMemcpy(&pNotification->uNotification.MbxTfer, &pMbxTferPriv->MbxTfer, sizeof(EC_T_MBXTFER));
    pNotification->dwClientID = pMbxTferPriv->MbxTfer.dwClntId;

    if (eMbxTferType_FOE_SEG_DOWNLOAD == pMbxTferPriv->MbxTfer.eMbxTferType)
    {
        pNotification->uNotification.MbxTfer.eMbxTferType = eMbxTferType_FOE_FILE_DOWNLOAD;
        pNotification->uNotification.MbxTfer.dwDataLen = pNotification->uNotification.MbxTfer.MbxData.FoE.dwFileSize;
    }
    if (eMbxTferType_FOE_SEG_UPLOAD == pMbxTferPriv->MbxTfer.eMbxTferType)
    {
        pNotification->uNotification.MbxTfer.eMbxTferType = eMbxTferType_FOE_FILE_UPLOAD;
        pNotification->uNotification.MbxTfer.dwDataLen = pNotification->uNotification.MbxTfer.MbxData.FoE.dwFileSize;
    }

    NotifyAndFree(EC_NOTIFY_MBOXRCV, pNotification, sizeof(EC_T_MBXTFER));
}

EC_T_VOID CEcMaster::NotifyMbxRcv(EC_T_MBXTFERP* pMbxTferPriv)
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;

    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
    if (EC_NULL == pNotification)
        return;

    OsMemcpy(&pNotification->uNotification.MbxTfer, &pMbxTferPriv->MbxTfer, sizeof(EC_T_MBXTFER));
    pNotification->dwClientID = pMbxTferPriv->MbxTfer.dwClntId;

    NotifyAndFree(EC_NOTIFY_MBOXRCV, pNotification, sizeof(EC_T_MBXTFER));
}
#endif /* INCLUDE_FOE_SUPPORT */

/********************************************************************************/
/** \brief This method is called cyclically by eUsrJob_MasterTimer
*
* \return
*/
EC_T_DWORD  CEcMaster::OnMasterTimer(EC_T_VOID)
{
    EC_T_INT                        nSlaveSliceCount        = 0;
    CEcSlave*                       pCfgSlave               = EC_NULL;
    ECAT_SLAVEFRAME_DESC*           pSlvFrmDesc             = EC_NULL;
    CEcSlaveFrame*                  poEcSlaveFrame          = EC_NULL;

    /* execute master state machine */
    MasterStateMachine();

    if (m_bMasterTimerEnabled)
    {
        /* slave OnTimer */ 
        if ((m_nCfgSlaveCount > 0) && (DEVICE_STATE_UNKNOWN != m_wCurMasterDeviceState))
        {
        CEcSlave* pCfgSlaveFirstOnTimer = EC_NULL;
        
            for (nSlaveSliceCount = GetMaxSlaveProcessedPerCycle();
                 nSlaveSliceCount > 0;
                 nSlaveSliceCount--, m_dwCfgSlaveOnTimerIdx = (m_dwCfgSlaveOnTimerIdx + 1) % m_nCfgSlaveCount)
            {
                pCfgSlave = m_ppEcSlave[m_dwCfgSlaveOnTimerIdx];

                /* don't call slave state machines twice */
                if (pCfgSlaveFirstOnTimer == EC_NULL)
                {
                    pCfgSlaveFirstOnTimer = pCfgSlave;
                }
                else
                {
                    /* don't call OnTimer twice */
                    if (pCfgSlaveFirstOnTimer == pCfgSlave)
                    {
                        break;
                    }
                }
#if (defined INCLUDE_CONFIG_EXTEND)
                if (pCfgSlave->ShallRemoveOnTimer() && !pCfgSlave->HasPendingRequests())
                {
                    RemoveSlaveFromList(pCfgSlave);
                    if (EC_NULL != pCfgSlave->m_pBusSlave)
                    {
                        pCfgSlave->m_pBusSlave->SetCfgSlave(EC_NULL);
                    }
                    pCfgSlave->SetRemoveOnTimer(EC_FALSE);
                }
                else
#endif
                {
                    pCfgSlave->OnTimer(m_dwMsecCounter);
                }
            }
        }
        /* timeout handling for pending frames */
        while (!IsListEmpty(&m_PendingSlaveFrameDescList))
        {
            /* get frame descriptors */
            pSlvFrmDesc    = (ECAT_SLAVEFRAME_DESC*)GetListHeadEntry(&m_PendingSlaveFrameDescList);
            poEcSlaveFrame = pSlvFrmDesc->poEcSlaveFrame;
            OsDbgAssert(EC_NULL != poEcSlaveFrame);

            /* check if time out elapsed */
            if (!poEcSlaveFrame->m_timeout.IsElapsed())
            {
                /* break on first non timed out frame to protect against endless loop */
                break;
            }
            /* this has to be an acyclic command (sent using QueueEcatCmdReq) */
            OsDbgAssert((poEcSlaveFrame->m_byEcCmdHeaderIDX >= m_byMinAcycCmdIdx) && (poEcSlaveFrame->m_byEcCmdHeaderIDX < (m_byMinAcycCmdIdx + m_MasterConfig.dwMaxQueuedEthFrames)));

            /* timeout handling */
            poEcSlaveFrame->m_timeout.Stop();

            /* remove frame from pending list m_PendingSlaveFrameDescList */
            RemoveEntryList(&poEcSlaveFrame->m_pSlaveFrameDesc->ListEntry);
#if (defined INCLUDE_MASTER_OBD)
            EC_SETDWORD(m_pdwLostFrameCrt, (EC_GETDWORD(m_pdwLostFrameCrt)+1));
#endif
            /* don't retry if link disconnected or frame is not monitored */
            if (m_poEcDevice->IsSendEnabled() && (poEcSlaveFrame->m_retry != (EC_T_WORD)0xFFFF))
            {
                if (poEcSlaveFrame->m_retry > 0)
                {
                    EC_ERRORMSGL(("CEcMaster::OnTimer() Timeout elapsed for frame IDX=0x%x, retry count = %d\n", poEcSlaveFrame->m_byEcCmdHeaderIDX, poEcSlaveFrame->m_retry));
                    /* decrement retry counter */
                    poEcSlaveFrame->m_retry--;
                    poEcSlaveFrame->m_pSlaveFrameDesc->dwRepeatCnt = 0;

#ifdef REMOVE_MBX_READ_RETRIES
                    CancelMbxCmds(poEcSlaveFrame, EC_E_FRAME_LOST);
#endif
                    /* increment MAC address to mark frame as retry frame */
                    {
                    ETHERNET_ADDRESS addr = m_poEcDevice->GetMacAddress();

                        EC_ETHFRM_SET_RETRYMACIDX(&(poEcSlaveFrame->m_EthernetFrame), addr.b,
                            EC_LOBYTE(((EC_T_WORD)m_MasterConfig.dwEcatCmdMaxRetries) - (poEcSlaveFrame->m_retry)));
                    }
                    /* requeue frame */
                    if (m_poEcDevice->DeviceQueueFrame(pSlvFrmDesc))
                    {
                        /* */
                        InsertTailList(&m_PendingSlaveFrameDescList, &poEcSlaveFrame->m_pSlaveFrameDesc->ListEntry);
                        poEcSlaveFrame = EC_NULL;
                    }
                }
                else
                {
                    NotifyFrameResponseError('T', 0, EC_FALSE, eRspErr_RETRY_FAIL, poEcSlaveFrame->m_byEcCmdHeaderIDX, poEcSlaveFrame->m_byEcCmdHeaderIDX);
                    EC_ERRORMSGL(("CEcMaster::OnTimer() Timeout elapsed for frame IDX=0x%x, no retries\n", poEcSlaveFrame->m_byEcCmdHeaderIDX));
                }
            }
            if (EC_NULL != poEcSlaveFrame)
            {
                /* cancel current frame */
                CancelAcycFrame(poEcSlaveFrame, EC_E_TIMEOUT);
                poEcSlaveFrame = EC_NULL;
            }
        }
    } /* m_bMasterTimerEnabled */

    if (EC_NULL != m_pBusTopology)
    {
        m_pBusTopology->BTStateMachine();
    }
#if (defined INCLUDE_DC_SUPPORT)
    /* trigger DC State machine */
    if (m_bDCSupportEnabled)
    {
        m_pDistributedClks->DCStateMachine();
    }
#endif

    m_oHotConnect.HCStateMachine();

#if (defined INCLUDE_SLAVE_STATISTICS)
    /* check timeout helper whether to trigger a slave counter update */
    if (m_bSlaveStatisticsRequested || m_oSlaveStatisticsTimer.IsElapsed())
    {
        if (ReadSlaveStatisticsSliced())
        {
            m_bSlaveStatisticsRequested = EC_FALSE;

            if (0 == m_dwSlaveStatisticsPeriod)
            {
                m_oSlaveStatisticsTimer.Stop();
            }
            else
            {
                m_oSlaveStatisticsTimer.Start(m_dwSlaveStatisticsPeriod, GetMsecCounterPtr());
            }
        }
    }
#endif

    /* monitor all devices are / are not operational */
    if (MasterStateMachIsStable() && (m_wReqMasterDeviceState == DEVICE_STATE_OP) && (GetScanBusSlaveCount() > 0) && GetAllSlavesMustReachState())
    {
#if (defined INCLUDE_HOTCONNECT)
        EC_T_DWORD dwHCGroupAmount = m_oHotConnect.GetGroupAmount();
#endif /* INCLUDE_HOTCONNECT */

        /* Block Not all devices are operational notification if topology change is in process or received broadcast read AL-status WKC was unequal expected one */
        if (m_pBusTopology->TopologyIsChanging() || TopologyChangedDetected() || m_oHotConnect.TopologyChangeInProgress() || m_oHotConnect.WaitingForManualContinue())
        {
            m_bNotAllDevOpNotificationEnabled = EC_FALSE;
        }
        else
        {
            /* We set m_bNotAllDevOpNotificationEnabled == EC_TRUE in McSM state eEcatMcSmState_TOPOCHANGE_DONE
            * m_bNotAllDevOpNotificationEnabled = EC_TRUE; */
        }
        /* AL status or WKC does not match? */
        /* notify application immediately if configuration does not contain optional hot connect groups */
        if ((m_wAlStatusReg != DEVICE_STATE_OP) || ((m_wALStatusExpectedWkc != m_wALStatusWkc)
#if (defined INCLUDE_HOTCONNECT)
            && (dwHCGroupAmount == 1)
#endif /* INCLUDE_HOTCONNECT */
            ))
        {
            NotifyAllDevicesOperational(EC_FALSE);
        }
        else
        {
            NotifyAllDevicesOperational(EC_TRUE);
        }
    }
    /* Update Master Info */
    if (!m_MasterInfoTimer.IsStarted())
    {
        m_MasterInfoTimer.Start(1000, GetMsecCounterPtr());
    }
    else if (m_MasterInfoTimer.IsElapsed())
    {
        m_MasterInfoTimer.Start(1000, GetMsecCounterPtr());
        MasterInfoSecondComplete();
    }

    return EC_E_NOERROR;
}

/********************************************************************************/
/** \brief Create string out of master state value.
*
* \return string representation of master state
*/
EC_T_CHAR*   GetMasterStateName(EC_MASTER_STATE eMasterState)
{
#ifdef INCLUDE_LOG_MESSAGES
    switch (eMasterState)
    {
    case EC_MASTER_STATE_NONE:              return (EC_T_CHAR*)"MASTER_STATE_NONE";
    case EC_MASTER_STATE_INIT:              return (EC_T_CHAR*)"MASTER_STATE_INIT";
    case EC_MASTER_STATE_WAIT_SLAVE:        return (EC_T_CHAR*)"EC_MASTER_STATE_WAIT_SLAVE";
    case EC_MASTER_STATE_WAIT_SLAVE_I:      return (EC_T_CHAR*)"EC_MASTER_STATE_WAIT_SLAVE_I";
    case EC_MASTER_STATE_WAIT_MASTER_I_P:   return (EC_T_CHAR*)"MASTER_STATE_WAIT_MASTER_I_P";
    case EC_MASTER_STATE_WAIT_SLAVE_I_P:    return (EC_T_CHAR*)"MASTER_STATE_WAIT_SLAVE_I_P";
    case EC_MASTER_STATE_PREOP:             return (EC_T_CHAR*)"MASTER_STATE_PREOP";
    case EC_MASTER_STATE_WAIT_MASTER_P_I:   return (EC_T_CHAR*)"MASTER_STATE_WAIT_MASTER_P_I";
    case EC_MASTER_STATE_WAIT_SLAVE_P_I:    return (EC_T_CHAR*)"MASTER_STATE_WAIT_SLAVE_P_I";
    case EC_MASTER_STATE_WAIT_MASTER_P_S:   return (EC_T_CHAR*)"MASTER_STATE_WAIT_MASTER_P_S";
    case EC_MASTER_STATE_WAIT_SLAVE_P_S:    return (EC_T_CHAR*)"MASTER_STATE_WAIT_SLAVE_P_S";
    case EC_MASTER_STATE_SAFEOP:            return (EC_T_CHAR*)"MASTER_STATE_SAFEOP";
    case EC_MASTER_STATE_WAIT_MASTER_S_I:   return (EC_T_CHAR*)"MASTER_STATE_WAIT_MASTER_S_I";
    case EC_MASTER_STATE_WAIT_SLAVE_S_I:    return (EC_T_CHAR*)"MASTER_STATE_WAIT_SLAVE_S_I";
    case EC_MASTER_STATE_WAIT_MASTER_S_P:   return (EC_T_CHAR*)"MASTER_STATE_WAIT_MASTER_S_P";
    case EC_MASTER_STATE_WAIT_SLAVE_S_P:    return (EC_T_CHAR*)"MASTER_STATE_WAIT_SLAVE_S_P";
    case EC_MASTER_STATE_WAIT_MASTER_S_O:   return (EC_T_CHAR*)"MASTER_STATE_WAIT_MASTER_S_O";
    case EC_MASTER_STATE_WAIT_SLAVE_S_O:    return (EC_T_CHAR*)"MASTER_STATE_WAIT_SLAVE_S_O";
    case EC_MASTER_STATE_OP:                return (EC_T_CHAR*)"MASTER_STATE_OP";
    case EC_MASTER_STATE_WAIT_MASTER_O_I:   return (EC_T_CHAR*)"MASTER_STATE_WAIT_MASTER_O_I";
    case EC_MASTER_STATE_WAIT_SLAVE_O_I:    return (EC_T_CHAR*)"MASTER_STATE_WAIT_SLAVE_O_I";
    case EC_MASTER_STATE_WAIT_MASTER_O_P:   return (EC_T_CHAR*)"MASTER_STATE_WAIT_MASTER_O_P";
    case EC_MASTER_STATE_WAIT_SLAVE_O_P:    return (EC_T_CHAR*)"MASTER_STATE_WAIT_SLAVE_O_P";
    case EC_MASTER_STATE_WAIT_MASTER_O_S:   return (EC_T_CHAR*)"MASTER_STATE_WAIT_MASTER_O_S";
    case EC_MASTER_STATE_WAIT_SLAVE_O_S:    return (EC_T_CHAR*)"MASTER_STATE_WAIT_SLAVE_O_S";
    case EC_MASTER_STATE_DISABLED:          return (EC_T_CHAR*)"EC_MASTER_STATE_DISABLED";
    case EC_MASTER_STATE_UNKNOWN:           return (EC_T_CHAR*)"EC_MASTER_STATE_UNKNOWN";
    default:                                OsDbgAssert(EC_FALSE); break;
    }
    return (EC_T_CHAR*)"MASTER_STATE_UNKNOWN";
#else
    EC_UNREFPARM(eMasterState);
    return (EC_T_CHAR*)"MASTER_STATE_?";
#endif
}

/********************************************************************************/
/** \brief Reset EtherCAT master state machine.
*
* \return
*/
EC_T_VOID CEcMaster::ResetMasterStateMachine(EC_T_VOID)
{
    for (EC_T_UINT i = 0; i < m_nCfgSlaveCount; i++)
    {
        m_ppEcSlave[i]->ResetSlaveStateMachine();
    }
    StabilizeMasterStateMachine(EC_E_NOTREADY, DEVICE_STATE_UNKNOWN);
}

/********************************************************************************/
/** \brief Stabilize the state machine of the EtherCAT master.
*
* \return
*/
EC_T_VOID CEcMaster::StabilizeMasterStateMachine(EC_T_DWORD dwResult, EC_T_WORD wDeviceState)
{
    m_wReqMasterDeviceState = wDeviceState;
    m_wCurMasterDeviceState = wDeviceState;
    switch (wDeviceState)
    {
    case DEVICE_STATE_UNKNOWN:        m_eCurMasterLogicalState = EC_MASTER_STATE_UNKNOWN; break;
    case DEVICE_STATE_INIT:           m_eCurMasterLogicalState = EC_MASTER_STATE_INIT;    break;
    case DEVICE_STATE_PREOP:          m_eCurMasterLogicalState = EC_MASTER_STATE_PREOP;   break;
    case DEVICE_STATE_SAFEOP:         m_eCurMasterLogicalState = EC_MASTER_STATE_SAFEOP;  break;
    case DEVICE_STATE_OP:             m_eCurMasterLogicalState = EC_MASTER_STATE_OP;      break;
    default: OsDbgAssert(EC_FALSE);   m_eCurMasterLogicalState = EC_MASTER_STATE_UNKNOWN; break;
    }
#if (defined INCLUDE_MASTER_RED)
    m_wLastMasterDeviceStateInactive = wDeviceState;
#endif
    if (EC_E_NOTREADY == dwResult)
    {
        m_dwMasterStateMachineResult = EC_E_NOTREADY;
    }
    else
    {
        if (EC_E_BUSY == m_dwMasterStateMachineResult)
        {
            m_dwMasterStateMachineResult = dwResult;
        }
    }
    m_bCancelStateMachine = EC_FALSE;
    m_bStateMachIsStable  = EC_TRUE;
    m_nInitCmdsCurrent    = INITCMD_INACTIVE;
}

/********************************************************************************/
/** \brief Request EtherCAT master to change to the specified state.
*
* \return
*/
EC_T_VOID CEcMaster::RequestMasterSmState(EC_T_WORD wReqMasterDeviceState)
{
    OsDbgAssert( m_bMasterTimerEnabled );
    OsDbgAssert((DEVICE_STATE_INIT == wReqMasterDeviceState) || (DEVICE_STATE_PREOP == wReqMasterDeviceState)|| (DEVICE_STATE_SAFEOP == wReqMasterDeviceState)|| (DEVICE_STATE_OP == wReqMasterDeviceState));
    EC_TRACEMSG(EC_TRACE_CORE, ("CEcMaster::RequestMasterState() %s (0x%x)\n", SlaveDevStateText(wReqMasterDeviceState), wReqMasterDeviceState));

    if (MasterStateMachIsStable() && (DEVICE_STATE_UNKNOWN != m_wCurMasterDeviceState))
    {
        /* check if slaves should be set to master state. This is needed if        */
        /* SetSlaveState was called or previous SetMasterState was not successfull */
        if (wReqMasterDeviceState >= m_wCurMasterDeviceState)
        {
            for (EC_T_DWORD dwSlaveIdx = 0; dwSlaveIdx < m_nCfgSlaveCount; dwSlaveIdx++)
            {
                if (m_wCurMasterDeviceState != m_ppEcSlave[dwSlaveIdx]->GetDeviceState())
                {
                    RequestSlaveSmState(m_ppEcSlave[dwSlaveIdx], m_wCurMasterDeviceState);
                    m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE;
                }
            }
        }
    }
    m_wReqMasterDeviceState = wReqMasterDeviceState;
    m_dwMasterStateMachineResult = EC_E_BUSY;
    m_bSlaveStateMachineError = EC_FALSE;
}

/********************************************************************************/
/** \brief Cancel the state machine of the EtherCAT master.
*
* \return
*/
EC_T_VOID CEcMaster::CancelMasterStateMachine(EC_T_VOID)
{
EC_T_DWORD dwSlaveIdx = 0;

    for (dwSlaveIdx = 0; dwSlaveIdx < m_nCfgSlaveCount; dwSlaveIdx++)
    {
        m_ppEcSlave[dwSlaveIdx]->CancelSlaveStateMachine();
    }
    if (m_nInitCmdsCurrent != INITCMD_INACTIVE)
    {
        m_bCancelStateMachine = EC_TRUE;
    }
    else
    {
        StabilizeMasterStateMachine(EC_E_CANCEL, GetMasterLowestDeviceState());
    }
}

/********************************************************************************/
/** \brief Request specific slave to change to the specified state
*
* \return
*/
EC_T_VOID CEcMaster::RequestSlaveSmState(CEcSlave* pCfgSlave, EC_T_WORD wRequestedDeviceState)
{
#if (defined INCLUDE_HOTCONNECT)
    if (!pCfgSlave->IsOptional() || ((GetHCMode() & echm_automan_mask) != echm_fullmanual))
#endif
    {
        pCfgSlave->RequestSmState((EC_T_STATE)wRequestedDeviceState);
    }
}

/********************************************************************************/
/** \brief Executes the state machine of the EtherCAT master.
*
* \return
*/
EC_T_VOID CEcMaster::MasterStateMachine(EC_T_VOID)
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_MasterConfig.dwStateChangeDebug >> 0) & 3);
    EC_MASTER_STATE eOldState = m_eCurMasterLogicalState;
#endif
    EC_T_INT        nSlaveSliceCount        = 0;
    EC_T_DWORD      dwSlaveIdx              = 0;

    /* Shall all slaves reach master state for this transition? */
    EC_T_BOOL       bAllSlavesMustReachState = m_bAllSlavesMustReachState;
    EC_T_WORD       wStartMasterInitCmds    = 0;
    EC_MASTER_STATE eNextMasterLogicalState = EC_MASTER_STATE_NONE;
    CEcSlave*       pCfgSlave               = EC_NULL;
#if (defined INCLUDE_DC_SUPPORT)
    EC_T_BOOL       bWaitForDcmInSync       = EC_FALSE;
#endif
    /* if master timer disabled, nothing to do */
    if (!m_bMasterTimerEnabled)
    {
        goto Exit;
    }
    /* refresh state machine is stable flag */
    RefreshMasterStateMachIsStable();

    /* execute slave state machines */
    {
    CEcSlave* pCfgSlaveFirstAdded = EC_NULL;

        /* apply slave slicing */
        for (nSlaveSliceCount = GetMaxSlaveProcessedPerCycle();
             nSlaveSliceCount > 0;
             nSlaveSliceCount--)
        {
            /* get config slave */
            if (!m_pSlavesStateMachineFifo->RemoveNoLock(pCfgSlave))
            {
                break;
            }
            /* don't call slave state machines twice */
            if (pCfgSlaveFirstAdded == pCfgSlave)
            {
                m_pSlavesStateMachineFifo->Add(pCfgSlave);
                break;
            }
            /* execute slave state machines */
            switch (pCfgSlave->SlaveStateMachine())
            {
            case eStateMachRes_Pending:
            {
                m_pSlavesStateMachineFifo->Add(pCfgSlave);

                /* store first added slave to not call slave state machines twice */
                if (EC_NULL == pCfgSlaveFirstAdded)
                {
                    pCfgSlaveFirstAdded = pCfgSlave;
                }
            } break;
            case eStateMachRes_Done:
            {
                /* nothing to do */
            } break;
            case eStateMachRes_Error:
            {
                if (bAllSlavesMustReachState && pCfgSlave->IsPresentOnBus())
                {
                    m_bSlaveStateMachineError = EC_TRUE;
                }
            } break;
            default:
            {
                OsDbgAssert(EC_FALSE);
                break;
            }
            }
        }
    }
    if (0 != m_pSlavesStateMachineFifo->GetCount())
    {
        /* wait for slave state machines */
        goto Exit;
    }

    /* check if all slaves must reach master state */
    switch( m_eCurMasterLogicalState )
    {
    case EC_MASTER_STATE_WAIT_SLAVE_O_I:
    case EC_MASTER_STATE_WAIT_SLAVE_O_P:
    case EC_MASTER_STATE_WAIT_SLAVE_O_S:
    case EC_MASTER_STATE_WAIT_SLAVE_S_I:
    case EC_MASTER_STATE_WAIT_SLAVE_S_P:
    case EC_MASTER_STATE_WAIT_SLAVE_P_I:
        /* transition to a lower state shall be always possible */
        bAllSlavesMustReachState = EC_FALSE;
        break;
    case EC_MASTER_STATE_UNKNOWN:
        if (m_wReqMasterDeviceState == DEVICE_STATE_INIT)
        {
            /* transition to INIT shall be always possible */
            bAllSlavesMustReachState = EC_FALSE;
        }
        break;
    default:
        break;
    }

    /* execute master state machine */
    switch (m_eCurMasterLogicalState)
    {
    case EC_MASTER_STATE_UNKNOWN:
        if (DEVICE_STATE_UNKNOWN != m_wReqMasterDeviceState)
        {
            /* master state is currently unknown, wait until all slaves are in INIT state */
            for (dwSlaveIdx = 0; dwSlaveIdx < m_nCfgSlaveCount; dwSlaveIdx++)
            {
                m_ppEcSlave[dwSlaveIdx]->ResetSlaveStateMachine();
                RequestSlaveSmState(m_ppEcSlave[dwSlaveIdx], eEcatState_INIT);
            }
            m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_I;
        }
        break;

    case EC_MASTER_STATE_WAIT_SLAVE_I:
        {
            m_eCurMasterLogicalState = EC_MASTER_STATE_INIT;
            if (m_MasterConfig.dwStateChangeDebug&3)
            {
                m_poEcDevice->LinkStateChngMsg( m_MasterConfig.dwStateChangeDebug, "Change Master State from UNKNOWN to INIT\n" );
            }
        }
        break;

        /* TODO Add II-InitCmd support */
    case EC_MASTER_STATE_INIT:
    case EC_MASTER_STATE_WAIT_MASTER_I_P:
        switch (m_wReqMasterDeviceState)
        {
        case DEVICE_STATE_UNKNOWN:
        case DEVICE_STATE_INIT:
            m_wCurMasterDeviceState = DEVICE_STATE_INIT;
            break;
        case DEVICE_STATE_PREOP:
        case DEVICE_STATE_SAFEOP:
        case DEVICE_STATE_OP:
            wStartMasterInitCmds    = ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE;
            eNextMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_I_P;
            break;
        default:
            OsDbgAssert(EC_FALSE);
            break;
        }
        break;
    case EC_MASTER_STATE_WAIT_SLAVE_I_P:
        wStartMasterInitCmds    = ECAT_INITCMD_I_P;
        eNextMasterLogicalState = EC_MASTER_STATE_PREOP;
        break;

    case EC_MASTER_STATE_PREOP:
    case EC_MASTER_STATE_WAIT_MASTER_P_I:
    case EC_MASTER_STATE_WAIT_MASTER_P_S:
        switch (m_wReqMasterDeviceState)
        {
        case DEVICE_STATE_INIT:
#if (defined INCLUDE_DC_SUPPORT)
            m_pDistributedClks->m_bCycDistributionActive = EC_FALSE;
#endif
            wStartMasterInitCmds    = ECAT_INITCMD_P_I | ECAT_INITCMD_BEFORE;
            eNextMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_P_I;
            break;
        case DEVICE_STATE_PREOP:
            m_wCurMasterDeviceState = DEVICE_STATE_PREOP;
            break;
        case DEVICE_STATE_SAFEOP:
        case DEVICE_STATE_OP:
#if (defined INCLUDE_DC_SUPPORT)
            /* at the beginning of the SAFEOP transition, wait for DCM InSync to protect for some slave PLL error due to Busshift acceleration */
            if ((eDcmMode_BusShift == m_pDistributedClks->DcmGetMode()) && (EC_NULL != m_pDistributedClks->m_pBusSlaveRefClock) && !GetAdjustCycFramesAfterslavesStateChange())
            {
                if (m_pDistributedClks->DcmIsInSync())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: DCM InSync\n"));
                    m_oDcmInSyncTimeout.Stop();
                }
                else
                {
                    bWaitForDcmInSync = EC_TRUE;

                    if (m_dwDcmInSyncTimeout != EC_WAITINFINITE)
                    {
                        if (!m_oDcmInSyncTimeout.IsStarted())
                        {
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: wait for DCM InSync %dms\n", m_dwDcmInSyncTimeout));
                            m_oDcmInSyncTimeout.Start(m_dwDcmInSyncTimeout, GetMsecCounterPtr());
                        }
                        if (m_oDcmInSyncTimeout.IsElapsed())
                        {
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: Timeout waiting for DCM InSync\n"));
                            m_oDcmInSyncTimeout.Stop();
                            bWaitForDcmInSync = EC_FALSE;
                            m_pDistributedClks->DcmSetStatus(DCM_E_MAX_CTL_ERROR_EXCEED);
                        }
                    }
                }
            }
            else
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: don't wait for DCM InSync\n"));
            }
            if (!bWaitForDcmInSync)
#endif /* INCLUDE_DC_SUPPORT */
            {
                wStartMasterInitCmds    = ECAT_INITCMD_P_S | ECAT_INITCMD_BEFORE;
                eNextMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_P_S;
            }
            break;
        default:
            OsDbgAssert(EC_FALSE);
            break;
        }
        break;
    case EC_MASTER_STATE_WAIT_SLAVE_P_I:
        wStartMasterInitCmds    = ECAT_INITCMD_P_I;
        eNextMasterLogicalState = EC_MASTER_STATE_INIT;
        break;
    case EC_MASTER_STATE_WAIT_SLAVE_P_S:
        wStartMasterInitCmds    = ECAT_INITCMD_P_S;
        eNextMasterLogicalState = EC_MASTER_STATE_SAFEOP;
        break;

    case EC_MASTER_STATE_SAFEOP:
    case EC_MASTER_STATE_WAIT_MASTER_S_I:
    case EC_MASTER_STATE_WAIT_MASTER_S_P:
    case EC_MASTER_STATE_WAIT_MASTER_S_O:
        switch (m_wReqMasterDeviceState)
        {
        case DEVICE_STATE_INIT:
#if (defined INCLUDE_DC_SUPPORT)
            m_pDistributedClks->m_bCycDistributionActive = EC_FALSE;
#endif
            wStartMasterInitCmds    = ECAT_INITCMD_S_I | ECAT_INITCMD_BEFORE;
            eNextMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_S_I;
            break;
        case DEVICE_STATE_PREOP:
            wStartMasterInitCmds    = ECAT_INITCMD_S_P | ECAT_INITCMD_BEFORE;
            eNextMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_S_P;
            break;
        case DEVICE_STATE_SAFEOP:
            m_wCurMasterDeviceState = DEVICE_STATE_SAFEOP;
            break;
        case DEVICE_STATE_OP:
#if (defined INCLUDE_DC_SUPPORT)
            /* at the beginning of the OP transition, wait for DCM InSync if cyclic frame adjustment was done after slave state change */
            if ((eDcmMode_Off != m_pDistributedClks->DcmGetMode()) && (EC_NULL != m_pDistributedClks->m_pBusSlaveRefClock) && GetAdjustCycFramesAfterslavesStateChange())
			{
                if (m_pDistributedClks->DcmIsInSync())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: DCM InSync\n"));
                    m_pDistributedClks->ActivateSyncWindowMonitoring();
                    if (m_pDistributedClks->IsInSync())
                    {
                        m_oDcmInSyncTimeout.Stop();
                    }
                    else
                    {
                        bWaitForDcmInSync = EC_TRUE;
                    }
				}
                else
                {
                    bWaitForDcmInSync = EC_TRUE;

                    if (m_dwDcmInSyncTimeout != EC_WAITINFINITE)
                    {
                        if (!m_oDcmInSyncTimeout.IsStarted())
                        {
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: wait for DCM InSync %dms\n", m_dwDcmInSyncTimeout));
                            m_oDcmInSyncTimeout.Start(m_dwDcmInSyncTimeout, GetMsecCounterPtr());
                        }
                        if (m_oDcmInSyncTimeout.IsElapsed())
                        {
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: Timeout waiting for DCM InSync\n"));
                            m_oDcmInSyncTimeout.Stop();
                            bWaitForDcmInSync = EC_FALSE;
                            m_pDistributedClks->DcmSetStatus(DCM_E_MAX_CTL_ERROR_EXCEED);
                            m_pDistributedClks->ActivateSyncWindowMonitoring();
                        }
                    }
                }
            }
            else
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: don't wait for DCM InSync\n"));
            }
            if (!bWaitForDcmInSync)
#endif /* INCLUDE_DC_SUPPORT */
            {
                wStartMasterInitCmds    = ECAT_INITCMD_S_O | ECAT_INITCMD_BEFORE;
                eNextMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_S_O;
            }
            break;
        default:
            OsDbgAssert(EC_FALSE);
            break;
        }
        break;
    case EC_MASTER_STATE_WAIT_SLAVE_S_I:
        wStartMasterInitCmds    = ECAT_INITCMD_S_I;
        eNextMasterLogicalState = EC_MASTER_STATE_INIT;
        break;
    case EC_MASTER_STATE_WAIT_SLAVE_S_P:
        wStartMasterInitCmds    = ECAT_INITCMD_S_P;
        eNextMasterLogicalState = EC_MASTER_STATE_PREOP;
        break;
    case EC_MASTER_STATE_WAIT_SLAVE_S_O:
        wStartMasterInitCmds    = ECAT_INITCMD_S_O;
        eNextMasterLogicalState = EC_MASTER_STATE_OP;
        break;

    case EC_MASTER_STATE_OP:
    case EC_MASTER_STATE_WAIT_MASTER_O_I:
    case EC_MASTER_STATE_WAIT_MASTER_O_P:
    case EC_MASTER_STATE_WAIT_MASTER_O_S:
        switch (m_wReqMasterDeviceState)
        {
        case DEVICE_STATE_INIT:
#if (defined INCLUDE_DC_SUPPORT)
            m_pDistributedClks->m_bCycDistributionActive = EC_FALSE;
#endif
            wStartMasterInitCmds    = ECAT_INITCMD_O_I | ECAT_INITCMD_BEFORE;
            eNextMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_O_I;
            break;
        case DEVICE_STATE_PREOP:
            wStartMasterInitCmds    = ECAT_INITCMD_O_P | ECAT_INITCMD_BEFORE;
            eNextMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_O_P;
            break;
        case DEVICE_STATE_SAFEOP:
            wStartMasterInitCmds    = ECAT_INITCMD_O_S | ECAT_INITCMD_BEFORE;
            eNextMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_O_S;
            break;
        case DEVICE_STATE_OP:
            m_wCurMasterDeviceState = DEVICE_STATE_OP;
            break;
        default:
            OsDbgAssert(EC_FALSE);
            break;
        }
        break;
    case EC_MASTER_STATE_WAIT_SLAVE_O_I:
        wStartMasterInitCmds    = ECAT_INITCMD_O_I;
        eNextMasterLogicalState = EC_MASTER_STATE_INIT;
        break;
    case EC_MASTER_STATE_WAIT_SLAVE_O_P:
        wStartMasterInitCmds    = ECAT_INITCMD_O_P;
        eNextMasterLogicalState = EC_MASTER_STATE_PREOP;
        break;
    case EC_MASTER_STATE_WAIT_SLAVE_O_S:
        wStartMasterInitCmds    = ECAT_INITCMD_O_S;
        eNextMasterLogicalState = EC_MASTER_STATE_SAFEOP;
        break;
    case EC_MASTER_STATE_WAIT_SLAVE:
        switch (m_wCurMasterDeviceState)
        {
        case DEVICE_STATE_UNKNOWN:        m_eCurMasterLogicalState = EC_MASTER_STATE_UNKNOWN; break;
        case DEVICE_STATE_INIT:           m_eCurMasterLogicalState = EC_MASTER_STATE_INIT;    break;
        case DEVICE_STATE_PREOP:          m_eCurMasterLogicalState = EC_MASTER_STATE_PREOP;   break;
        case DEVICE_STATE_SAFEOP:         m_eCurMasterLogicalState = EC_MASTER_STATE_SAFEOP;  break;
        case DEVICE_STATE_OP:             m_eCurMasterLogicalState = EC_MASTER_STATE_OP;      break;
        default: OsDbgAssert(EC_FALSE);   m_eCurMasterLogicalState = EC_MASTER_STATE_UNKNOWN; break;
        }
        break;
    default:
        OsDbgAssert(EC_FALSE);
        break;
    } /* switch (m_eCurMasterLogicalState) */

    /* handle master init commands */
    if ((0 != wStartMasterInitCmds) && (INITCMD_INACTIVE == m_nInitCmdsCurrent))
    {
        /* start init commands only if send is enabled */
        if (m_poEcDevice->IsSendEnabled())
        {
            /* check if an init command is defined for the transition specified in wStartMasterInitCmds */
            for (m_nInitCmdsCurrent = 0; m_nInitCmdsCurrent < m_nInitCmdsCount; m_nInitCmdsCurrent++)
            {
                /* check if Init command exists for this transition and if ECAT_INITCMD_BEFORE is set */
                /* in m_ppInitCmds[m_nInitCmdsCurrent]->transition as well as in wStartMasterInitCmds */
                if ((EC_ECINITCMDDESC_GET_TRANSITION(m_ppInitCmds[m_nInitCmdsCurrent]) & (wStartMasterInitCmds|ECAT_INITCMD_BEFORE)) == wStartMasterInitCmds)
                {
                    /* init command found --> process the first init command for this transition */
                    break;
                }
            }
        }
        else
        {
            m_nInitCmdsCurrent = m_nInitCmdsCount;
        }
        if (m_nInitCmdsCurrent < m_nInitCmdsCount)
        {
        PEcInitCmdDesc pInitCmdDesc = EC_NULL;

            /* determine the next master logical state */
            switch (eNextMasterLogicalState)
            {
            case EC_MASTER_STATE_WAIT_SLAVE_P_I: m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_MASTER_P_I; break;
            case EC_MASTER_STATE_WAIT_SLAVE_S_I: m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_MASTER_S_I; break;
            case EC_MASTER_STATE_WAIT_SLAVE_O_I: m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_MASTER_O_I; break;
            case EC_MASTER_STATE_WAIT_SLAVE_I_P: m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_MASTER_I_P; break;
            case EC_MASTER_STATE_WAIT_SLAVE_S_P: m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_MASTER_S_P; break;
            case EC_MASTER_STATE_WAIT_SLAVE_O_P: m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_MASTER_O_P; break;
            case EC_MASTER_STATE_WAIT_SLAVE_P_S: m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_MASTER_P_S; break;
            case EC_MASTER_STATE_WAIT_SLAVE_O_S: m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_MASTER_O_S; break;
            case EC_MASTER_STATE_WAIT_SLAVE_S_O: m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_MASTER_S_O; break;
            default: OsDbgAssert(EC_FALSE); break;
            }
            /* get init command descriptor */
            pInitCmdDesc = m_ppInitCmds[m_nInitCmdsCurrent];

            /* start timeout for init commands */
            {
                EC_T_WORD wInitCmdTimeout = 0;

                wInitCmdTimeout = EC_ECINITCMDDESC_GET_INITCMDTIMEOUT(pInitCmdDesc);
                if (0 == wInitCmdTimeout)
                {
                    wInitCmdTimeout = INIT_CMD_DEFAULT_TIMEOUT;
                }
                m_oInitCmdsTimeout.Start(wInitCmdTimeout, GetMsecCounterPtr());
            }
            m_oInitCmdRetryTimeout.Start(m_dwInitCmdRetryTimeout, GetMsecCounterPtr());

            EC_TRACEMSG(EC_TRACE_INITCMDS, ("-----\n"));
            EC_TRACEMSG(EC_TRACE_INITCMDS,
               ("Master InitCmd QuCmd[%d]: %s: %s,adp=%d,ado=0x%x\n",
                m_nInitCmdsCurrent,
                GetStateChangeNameShort((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)),
                EcatCmdShortText(pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd),
                (int)EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                (int)EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader))
               )
                       );
            EC_TRACEMSG(EC_TRACE_INITCMDS, ("-----\n"));

            /* send first init command defined for this transition */
            QueueRegisterCmdReq(
                EC_NULL,
                wStartMasterInitCmds,
                pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd,
                EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader)),
                EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                EC_ICMDHDR_GET_LEN_LEN(&(pInitCmdDesc->EcCmdHeader)),
                EcInitCmdDescData(pInitCmdDesc)
                           );
            /* the other init commands defined for this transition will be sent in EcMaster::ProcessCmdResult after */
            /* receiving the response to this frame */
        }
        else
        {
        EC_T_WORD wSlaveDeviceStateReq   = 0;
        EC_T_WORD wSlaveDeviceStateCheck = 0;

            /* don't send more master init commands now */
            m_nInitCmdsCurrent = INITCMD_INACTIVE;

            /* determine next master state to change to and the eventually slave state to request */
            switch (eNextMasterLogicalState)
            {
            case EC_MASTER_STATE_WAIT_SLAVE_P_I:
            case EC_MASTER_STATE_WAIT_SLAVE_S_I:
            case EC_MASTER_STATE_WAIT_SLAVE_O_I: wSlaveDeviceStateReq   = DEVICE_STATE_INIT;   break;
            case EC_MASTER_STATE_INIT          : wSlaveDeviceStateCheck = DEVICE_STATE_INIT;   break;
            case EC_MASTER_STATE_WAIT_SLAVE_I_P:
            case EC_MASTER_STATE_WAIT_SLAVE_S_P:
            case EC_MASTER_STATE_WAIT_SLAVE_O_P: wSlaveDeviceStateReq   = DEVICE_STATE_PREOP;  break;
            case EC_MASTER_STATE_PREOP         : wSlaveDeviceStateCheck = DEVICE_STATE_PREOP;  break;
            case EC_MASTER_STATE_WAIT_SLAVE_P_S:
            case EC_MASTER_STATE_WAIT_SLAVE_O_S: wSlaveDeviceStateReq   = DEVICE_STATE_SAFEOP; break;
            case EC_MASTER_STATE_SAFEOP        : wSlaveDeviceStateCheck = DEVICE_STATE_SAFEOP; break;
            case EC_MASTER_STATE_WAIT_SLAVE_S_O: wSlaveDeviceStateReq   = DEVICE_STATE_OP;     break;
            case EC_MASTER_STATE_OP            : wSlaveDeviceStateCheck = DEVICE_STATE_OP;     break;
            default: break;
            }
            /* slave state should be requested or check */
            if (0 != wSlaveDeviceStateReq)
            {
                /* request slaves state disregarding slave slicing */
                for (dwSlaveIdx = 0; dwSlaveIdx < m_nCfgSlaveCount; dwSlaveIdx++)
                {
                    RequestSlaveSmState(m_ppEcSlave[dwSlaveIdx], wSlaveDeviceStateReq);
                }
                /* master state reached */
                m_eCurMasterLogicalState = eNextMasterLogicalState;
            }
            else
            {
                /* determine current state of the master state machine */
                if (!m_bSlaveStateMachineError || (0 == m_pBusTopology->GetConnectedSlaveCount()) || !bAllSlavesMustReachState)
                {
                    /* notify slave in unexpected state for EC_IOCTL_ALL_SLAVES_MUST_REACH_MASTER_STATE = EC_FALSE */
                    if ((0 != m_pBusTopology->GetConnectedSlaveCount()) && m_bSlaveStateMachineError)
                    {
                        /* look for slaves in unexpected state disregarding slave slicing */
                        for (dwSlaveIdx = 0; dwSlaveIdx < m_nCfgSlaveCount; dwSlaveIdx++)
                        {
                            /* get config slave */
                            pCfgSlave = m_ppEcSlave[dwSlaveIdx];

                            if ((pCfgSlave->GetDeviceState() != wSlaveDeviceStateCheck) && (pCfgSlave->IsPresentOnBus()))
                            {
                                NotifySlaveUnexpectedState(pCfgSlave, (EC_T_STATE)wSlaveDeviceStateCheck);
                            }
                        }
                    }
                    /* no slave is present or slaves reached requested state or all slaves must not reach master state */
#if (defined INCLUDE_DC_SUPPORT)
                    /* at the end of the SAFEOP transition, wait for DCM and DC InSync if DC slaves are present on bus */
                    if ((eDcmMode_Off != m_pDistributedClks->DcmGetMode()) && (0 != m_pBusTopology->GetDCSlaveCount()) && (EC_MASTER_STATE_WAIT_SLAVE_P_S == m_eCurMasterLogicalState) && !GetAdjustCycFramesAfterslavesStateChange())
                    {
                        if (m_pDistributedClks->DcmIsInSync())
                        {
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: DCM InSync\n"));
                            m_pDistributedClks->ActivateSyncWindowMonitoring();
                            if (m_pDistributedClks->IsInSync())
                            {
                                m_oDcmInSyncTimeout.Stop();
                            }
                            else                            
                            {
                                bWaitForDcmInSync = EC_TRUE;
                            }
                        }
                        else
                        {
                            bWaitForDcmInSync = EC_TRUE;
                        }
                        if (bWaitForDcmInSync)
                        {
                            if (m_dwDcmInSyncTimeout != EC_WAITINFINITE)
                            {
                                if (!m_oDcmInSyncTimeout.IsStarted())
                                {
                                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: wait for DCM InSync %dms\n", m_dwDcmInSyncTimeout));
                                    m_oDcmInSyncTimeout.Start(m_dwDcmInSyncTimeout, GetMsecCounterPtr());
                                }
                                if (m_oDcmInSyncTimeout.IsElapsed())
                                {
                                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: Timeout waiting for DCM InSync\n"));
                                    m_oDcmInSyncTimeout.Stop();
                                    bWaitForDcmInSync = EC_FALSE;
                                    m_pDistributedClks->DcmSetStatus(DCM_E_MAX_CTL_ERROR_EXCEED);
                                    m_pDistributedClks->ActivateSyncWindowMonitoring();
                                }
                            }
                        }
                    }
                    else
                    {
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "MasterSm: don't wait for DCM InSync\n"));
                    }
                    if (!bWaitForDcmInSync)
                    {
                        m_pDistributedClks->DcmNotifyMasterStateChange(eNextMasterLogicalState);
#else
                    {
#endif /* INCLUDE_DC_SUPPORT */
                        /* master state reached */
                        m_eCurMasterLogicalState = eNextMasterLogicalState;
                    }
                }
                else
                {
                    /* last slave state change failed, stabilize state machine */
                    StabilizeMasterStateMachine(EC_E_SLAVE_ERROR, GetMasterLowestDeviceState());
                }
            }
        } /* else if ( m_nInitCmdsCurrent < m_nInitCmdsCount ) */
    } /* if ((0 != wStartMasterInitCmds) && (INITCMD_INACTIVE == m_nInitCmdsCurrent)) */

    /* refresh state machine is stable flag */
    RefreshMasterStateMachIsStable();

Exit:
#if (defined INCLUDE_LOG_MESSAGES)
    if ((m_eCurMasterLogicalState != eOldState) && byDbgLvl)
    {
        EC_DBGMSGL(( 
            byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
            "MasterSm: %s -> %s\n", GetMasterStateName(eOldState), GetMasterStateName(m_eCurMasterLogicalState)));
    }
#endif
    return;
}

EC_T_DWORD CEcMaster::IsAnyCmdExceedingImage()
{
EC_T_DWORD dwNumCycConfigEntries;
EC_T_DWORD dwCycEntryIndex;
EC_T_WORD  wFrameIdx;

    if (EC_NULL == m_aCyclicEntryMgmtDesc)
    {
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
        OsDbgAssert(0 == m_dwNumCycConfigEntries);
#endif
        return EC_E_NOERROR;
    }
    if ((EC_NULL == m_oMemProvider.pbyPDOutData) && (EC_NULL == m_oMemProvider.pbyPDInData))
    {
        /* no memory provider */
        return EC_E_NOERROR;
    }
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    dwNumCycConfigEntries = m_dwNumCycConfigEntries;
#else
    dwNumCycConfigEntries = 1;
#endif
    for (dwCycEntryIndex = 0; dwCycEntryIndex < dwNumCycConfigEntries; dwCycEntryIndex++)
    {
        CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc = &m_aCyclicEntryMgmtDesc[dwCycEntryIndex];

        for (wFrameIdx = 0; wFrameIdx < pCyclicEntryMgmtDesc->wNumFrames; wFrameIdx++)
        {
            EcCycFrameDesc* pFrameDesc = pCyclicEntryMgmtDesc->apEcCycFrameDesc[wFrameIdx];
            for (EC_T_WORD wCmdIdx = 0; wCmdIdx < pFrameDesc->wCmdCnt; wCmdIdx++)
            {
                EcCycCmdConfigDesc* pCmdDesc = pFrameDesc->apCycCmd[wCmdIdx];
                if (pCmdDesc->bCopyInputs && (pCmdDesc->dwInOffset + pCmdDesc->wDataLen > m_oMemProvider.dwPDInDataLength))
                {
                    EC_ERRORMSGC(("Cmd offset + length exceeds IN image!\n"));
                    return EC_E_XML_INVALID_INP_OFF;
                }
                if (pCmdDesc->bCopyOutputs && (pCmdDesc->dwOutOffset + pCmdDesc->wDataLen > m_oMemProvider.dwPDOutDataLength))
                {
                    EC_ERRORMSGC(("Cmd offset + length exceeds OUT image!\n"));
                    return EC_E_XML_INVALID_OUT_OFF;
                }
            }
        }
    }
    return EC_E_NOERROR;
}

/********************************************************************************/
/** \brief Determine if all previously sent cyclic frames are received and processed
*
* \return
*/
EC_T_BOOL CEcMaster::AreAllCycFramesProcessed()
{
    EC_T_BOOL  bCycFramesAreProcessed = EC_TRUE;
    EC_T_DWORD dwNumCycConfigEntries;
    EC_T_DWORD dwCycEntryIndex;

    /* check if cyclic frames are present */
    if (EC_NULL == m_aCyclicEntryMgmtDesc)
    {
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
        OsDbgAssert(0 == m_dwNumCycConfigEntries);
#endif
        bCycFramesAreProcessed = EC_TRUE;
        goto Exit;
    }
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    dwNumCycConfigEntries = m_dwNumCycConfigEntries;
#else
    dwNumCycConfigEntries = 1;
#endif
    for (dwCycEntryIndex = 0; dwCycEntryIndex < dwNumCycConfigEntries; dwCycEntryIndex++)
    {
        CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc = &m_aCyclicEntryMgmtDesc[dwCycEntryIndex];

        /* check communication timeout */
        /* sent/received frames count, and last frame index must match */
        if (pCyclicEntryMgmtDesc->wFramesMonitoringSndCnt > pCyclicEntryMgmtDesc->wFramesMonitoringRcvCnt)
        {
            /* if no link do not set this error */
            if (!m_poEcDevice->IsLinkConnected())
            {
                bCycFramesAreProcessed = EC_TRUE;
                goto Exit;
            }

            bCycFramesAreProcessed = EC_FALSE;
#if (defined INCLUDE_WKCSTATE)
            for (EC_T_WORD wFrameIdx = 0; wFrameIdx < pCyclicEntryMgmtDesc->wNumFrames; wFrameIdx++)
            {
                EcCycFrameDesc* pFrameDesc = pCyclicEntryMgmtDesc->apEcCycFrameDesc[wFrameIdx];

                if (pFrameDesc->bIsPending)
                {
                    pFrameDesc->bIsPending = EC_FALSE;

                    /* set wkc state errors */
                    for (EC_T_WORD wCmdIdx = 0; wCmdIdx < pFrameDesc->wCmdCnt; wCmdIdx++)
                    {
                        EcCycCmdConfigDesc* pCmdDesc = pFrameDesc->apCycCmd[wCmdIdx];

                        if (pCmdDesc->bActive && pCmdDesc->bCheckWkc)
                        {
                            EC_SETBIT(GetDiagnosisImagePtr(), pCmdDesc->wWkcStateDiagOffs);
                        }
                    }
                }
            }
#else
            goto Exit;
#endif
        }
    }

Exit:
    return bCycFramesAreProcessed;
}

/********************************************************************************/
/** \brief Send cyclic frames for one specific task id
*
* All cyclic frames for one specific task id specified within the XML configuration file in Config/Cyclic/Frame are sent here.
*
* This method is called by CEcDevice::StartIo()
*
* \return N/A
*/
EC_T_DWORD CEcMaster::SendCycFramesByTaskId(
    EC_T_DWORD dwTaskId                 /* [in]  send all cyclic frames of the corresponding task, 0xFFFFFFFF = send all frames */
#if (defined INCLUDE_DC_SUPPORT)
   ,EC_T_BOOL  bStampJob                /* [in]  job requests stampped cyc io update */
#endif
   ,EC_T_WORD* pwNumSendFrames
                                           )
{
    EC_T_DWORD              dwRetVal             = EC_E_ERROR;
    EC_T_DWORD              dwRes                = EC_E_ERROR;
    CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc = EC_NULL;
    EC_T_DWORD              dwCycEntryIndex      = 0;
    EC_T_BOOL               bTaskIdFound         = EC_FALSE;
    EC_T_WORD               wNumSendFrames       = 0;

#if !(defined INCLUDE_MULTIPLE_CYC_ENTRIES)
    EC_UNREFPARM(dwTaskId);
#endif

    /* start sending cyclic frames */
    m_poEcDevice->SetFramesType(EC_TRUE);
    
    /* increment current cycle idx for cyclic frames monitoring */
    m_byCurCycleIdx = (EC_T_BYTE)((m_byCurCycleIdx + 1) % m_byMaxCycleIdx);

    pCyclicEntryMgmtDesc = m_aCyclicEntryMgmtDesc;
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    for (dwCycEntryIndex=0; dwCycEntryIndex < m_dwNumCycConfigEntries; dwCycEntryIndex++, pCyclicEntryMgmtDesc++)
    {
        if ((dwTaskId == 0xFFFFFFFF) || (pCyclicEntryMgmtDesc->dwTaskId == dwTaskId) )
#endif
        {
            bTaskIdFound = EC_TRUE;
            dwRes = SendCycFramesByEntry(dwCycEntryIndex
#if (defined INCLUDE_DC_SUPPORT)
                , bStampJob
#endif
                );
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        }
        wNumSendFrames = (EC_T_WORD)(wNumSendFrames + pCyclicEntryMgmtDesc->wFramesMonitoringSndCnt);
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    }
#endif
    if (!bTaskIdFound)
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (0 != wNumSendFrames)
    {
        m_poEcDevice->FlushFrames();
    }
    *pwNumSendFrames = wNumSendFrames;

    return dwRetVal;
}

/********************************************************************************/
/** \brief Send cyclic frames
*
* All cyclic frames of a single cyclic entry specified within the XML configuration file in Config/Cyclic/Frame are sent here.
*
* \return N/A
*/
EC_T_DWORD CEcMaster::SendCycFramesByEntry(
    EC_T_DWORD              dwCycEntryIndex         /* [in]  send all cyclic frames of the corresponding cyclic entry */
#if (defined INCLUDE_DC_SUPPORT)
    ,EC_T_BOOL              bStampJob               /* [in]  job requests stampped cyc io update */
#endif
                                          )
{
    EC_T_DWORD              dwRetVal                = EC_E_NOERROR;
    EC_T_DWORD              dwRes                   = EC_E_NOERROR;
    CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc    = EC_NULL;
    EC_T_BOOL               bCycFramesAreProcessed  = EC_FALSE;
#if (defined INCLUDE_MASTER_OBD) || (defined INCLUDE_DC_SUPPORT)
    EC_T_BOOL               bCycFramesAreLost       = EC_FALSE;
#endif
    volatile EC_T_PBYTE     pbyPDOutBasePtr         = EC_NULL;
#if (defined INCLUDE_VARREAD)
    EC_T_BOOL               bSwapped                = EC_FALSE;
#endif
    EC_T_WORD               wFrameIdx               = 0;
    EC_T_DEVICE_FRAMEDESC   oCycDeviceFrameDesc;
    EC_T_LINK_FRAMEDESC*    poLinkFrameDesc         = &(oCycDeviceFrameDesc.oLinkFrameDesc);
    ETYPE_EC_CMD_HEADER*    pFirstCmd               = EC_NULL;
    EC_T_BYTE*              pPtr                    = EC_NULL;
    ETYPE_EC_CMD_HEADER*    pCurCmd                 = EC_NULL;

    EC_TRACEMSG( EC_TRACE_CORE_SEQUENCE, ("[%06d] CEcMaster::SendCycFramesByEntry(): GetDeviceState()\n", OsQueryMsecCount()));

#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    if (dwCycEntryIndex >= m_dwNumCycConfigEntries )
    {
        OsDbgAssert( EC_FALSE );
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
#endif
    /* select management descriptor */
    pCyclicEntryMgmtDesc = &m_aCyclicEntryMgmtDesc[dwCycEntryIndex];

    /* check if send is enabled */
    if (!m_poEcDevice->IsSendEnabled())
    {
        /* refresh ALStatusWkc */
        if (EC_AL_STATUS_INIT_VALUE != m_wALStatusWkc)
        {
            m_wALStatusWkc    = 0;
        }
#if (defined INCLUDE_RED_DEVICE)
        m_wRedNumSlavesOnMain = 0;
        m_wRedNumSlavesOnRed  = 0;
#if (defined INCLUDE_MASTER_OBD)
        RedUpdateOd();
#endif
#endif /* INCLUDE_RED_DEVICE */
        dwRetVal = EC_E_LINK_DISCONNECTED;
        goto Exit;
    }
    /* cyclic frames monitoring */
    if (0 != pCyclicEntryMgmtDesc->wFramesMonitoringSndCnt)
    {
        if (pCyclicEntryMgmtDesc->wFramesMonitoringRcvCnt < pCyclicEntryMgmtDesc->wFramesMonitoringSndCnt)
        {
#if (defined INCLUDE_MASTER_OBD) || (defined INCLUDE_DC_SUPPORT)
            bCycFramesAreLost = EC_TRUE;
#endif
            NotifyFrameResponseError('\0', '\0', EC_TRUE, eRspErr_NO_RESPONSE, 0, 0);
        }
        else
        {
            bCycFramesAreProcessed = EC_TRUE;
        }
    }
    if (pCyclicEntryMgmtDesc->wFramesMonitoringRcvCnt > pCyclicEntryMgmtDesc->wFramesMonitoringSndCnt)
    {
        NotifyFrameResponseError('\0', '\0', EC_TRUE, eRspErr_UNEXPECTED, 0, 0);
    }
    pCyclicEntryMgmtDesc->wFramesMonitoringSndCnt = 0;
    pCyclicEntryMgmtDesc->wFramesMonitoringRcvCnt = 0;

#if (defined INCLUDE_MASTER_OBD)
    if (bCycFramesAreLost)
    {
        EC_SETDWORD(m_pdwLostFrameCrt, (EC_GETDWORD(m_pdwLostFrameCrt)+1));
    }
#endif
    /* detect cyclic frames change */
    {
    EC_T_WORD wCycFramesDeviceState;

        if (GetAdjustCycFramesAfterslavesStateChange())
        {
            wCycFramesDeviceState = GetMasterDeviceState();
        }
        else
        {
            wCycFramesDeviceState = GetMasterHighestDeviceState();
        }
        if (m_wCycFramesDeviceState != wCycFramesDeviceState)
        {
            m_wCycFramesDeviceState  = wCycFramesDeviceState;

            /* adjust cyclic frames according to master state */
            switch (m_wCycFramesDeviceState)
            {
            case DEVICE_STATE_UNKNOWN:
                dwRetVal = EC_E_INVALIDSTATE;
                goto Exit;
                /* no break */
            case DEVICE_STATE_INIT:         AdjustCycFramesToState(DEVICE_STATE_INIT);
                break;
            case DEVICE_STATE_PREOP:        AdjustCycFramesToState(DEVICE_STATE_PREOP);
                break;
            case DEVICE_STATE_SAFEOP:       AdjustCycFramesToState(DEVICE_STATE_SAFEOP);
                break;
            case DEVICE_STATE_OP:           AdjustCycFramesToState(DEVICE_STATE_OP);
                break;
            default:
                break;
            }
        }
    }
#ifdef INCLUDE_MEMORY_PROVIDER
    pbyPDOutBasePtr = GetPDOutBasePointer();
#else
    pbyPDOutBasePtr = m_oMemProvider.pbyPDOutData;
#endif
#if (defined INCLUDE_VARREAD)
    if(pCyclicEntryMgmtDesc->wNumOfSwapInfos != 0)
    {
        SwapData(pCyclicEntryMgmtDesc, EC_FALSE, pbyPDOutBasePtr);
        bSwapped = EC_TRUE;
    }
#endif
    /* Copy information for Slave-to-Slave communication */
    if ((0 != pCyclicEntryMgmtDesc->wNumOfCopyInfos) && m_bCopyInfoInSendCycFrames)
    {
    EC_T_BYTE* pbyPDInBasePtr = GetPDInBasePointer();

        if (EC_NULL != pbyPDOutBasePtr)
        {
            S2SCopyData(pCyclicEntryMgmtDesc, pbyPDInBasePtr, pbyPDOutBasePtr);
            ReturnPDInBasePointer();
        }
    }

#if (defined INCLUDE_FORCE_PROCESSDATA)
     if((EC_NULL != pbyPDOutBasePtr) && (m_wForceProcessDataEntries != 0))
     {
         ForceProcessDataOutputs(pbyPDOutBasePtr);
     }
#endif

    /* create frame descriptors */
    OsMemset(poLinkFrameDesc, 0, sizeof(EC_T_LINK_FRAMEDESC));
    for (wFrameIdx = 0; wFrameIdx < pCyclicEntryMgmtDesc->wNumFrames; wFrameIdx++)
    {
        EcCycFrameDesc* pFrameDesc = pCyclicEntryMgmtDesc->apEcCycFrameDesc[wFrameIdx];
        ETYPE_88A4_HEADER* p88A4Hdr;
        EC_T_WORD  wCmdsLen = 0;
        EC_T_WORD  wHeaderLen = ETHERNET_88A4_FRAME_LEN;
        EC_T_DWORD actSize=ETHERNET_88A4_FRAME_LEN + EC_AL_88A4HDR_GET_E88A4FRAMELEN(&(pFrameDesc->e88A4Frame.E88A4Header));
#if (defined VLAN_FRAME_SUPPORT)
        if (m_bVLANEnabled)
        {
            wHeaderLen += ETYPE_VLAN_HEADER_LEN;
            actSize += ETYPE_VLAN_HEADER_LEN;
        }
#endif
        /* frame should be sent */
        /* request frame buffer from link layer, avoid allocation by master */
        dwRes = m_poEcDevice->DeviceFrameDescCreate(&oCycDeviceFrameDesc, EC_FALSE, pFrameDesc->pbyFrame,
#if (defined INCLUDE_RED_DEVICE)
            m_pbyRedFrame,
#endif
            actSize);
        if (dwRes != EC_E_NOERROR)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
        poLinkFrameDesc->pfnTimeStamp = EC_NULL;

        switch (m_eCycFrameLayout)
        {
        case eCycFrameLayout_STANDARD:
        case eCycFrameLayout_DYNAMIC:
            /* fill frame header */
            OsMemcpy(&poLinkFrameDesc->pbyFrame[0], &pFrameDesc->e88A4Frame, ETHERNET_88A4_FRAME_LEN);
            break;
        case eCycFrameLayout_FIXED:
            OsMemcpy(&poLinkFrameDesc->pbyFrame[0], pFrameDesc->pbyFrame, pFrameDesc->wSize);
            break;
        case eCycFrameLayout_IN_DMA:
            break;
        default:
            OsDbgAssert(EC_FALSE);
            break;
        }

#if (defined VLAN_FRAME_SUPPORT)
        if (m_bVLANEnabled)
        {
            ETYPE_VLAN_HEADER* pVlanHrd = &((ETHERNET_VLAN_FRAME*)(poLinkFrameDesc->pbyFrame))->VLan;

            EC_RESET_VLANHDR(pVlanHrd);
            EC_SET_VLANHDR_Type(pVlanHrd, ETHERNET_FRAME_TYPE_VLAN);
            EC_SET_VLANHDR_ID(pVlanHrd, m_wVLANId);
            EC_SET_VLANHDR_Prio(pVlanHrd, m_byVLANPrio);

            EC_ETHFRM_SET_FRAMETYPE(poLinkFrameDesc->pbyFrame + ETYPE_VLAN_HEADER_LEN, ETHERNET_FRAME_TYPE_BKHF);
            p88A4Hdr  = (ETYPE_88A4_HEADER*)  (poLinkFrameDesc->pbyFrame + ETHERNET_FRAME_LEN      + ETYPE_VLAN_HEADER_LEN);
            pFirstCmd = (ETYPE_EC_CMD_HEADER*)(poLinkFrameDesc->pbyFrame + ETHERNET_88A4_FRAME_LEN + ETYPE_VLAN_HEADER_LEN);
        }
        else
#endif
        {
            p88A4Hdr  = (ETYPE_88A4_HEADER*)&poLinkFrameDesc->pbyFrame[ETHERNET_FRAME_LEN];
            pFirstCmd = &(((PETHERNET_88A4_HEADER)&poLinkFrameDesc->pbyFrame[ETHERNET_FRAME_LEN])->sETECAT.FirstEcCmdHeader);
        }

        /* iteration of commands: update output process image stored in the frame */
        oCycDeviceFrameDesc.dwNumCmdsInFrame = 0;
        pPtr = (EC_T_BYTE*)pFirstCmd;

        for (EC_T_WORD wCmdIdx = 0; wCmdIdx < pFrameDesc->wCmdCnt; wCmdIdx++)
        {
            EcCycCmdConfigDesc* pCmdDesc = pFrameDesc->apCycCmd[wCmdIdx];

            pCurCmd = ((ETYPE_EC_CMD_HEADER*)pPtr);

            /* command header and payload */
            switch (m_eCycFrameLayout)
            {
            case eCycFrameLayout_STANDARD:
            case eCycFrameLayout_DYNAMIC:
                /* skip all inactive commands */
                if (!pCmdDesc->bActive)
                {
                    continue;
                }
                /* copy cmd header */
                OsMemcpy(pCurCmd, &pCmdDesc->EcCmdHeader, sizeof(ETYPE_EC_CMD_HEADER));

                /* move pointer to payload */
                pPtr += ETYPE_EC_CMD_HEADER_LEN;

                /* copy payload */
                if (pCmdDesc->bCopyOutputs && (EC_NULL != pbyPDOutBasePtr) )
                {
                    /* copy data from output process image to the cyclic cmd */
                    OsMemcpyPdOut(pPtr, &pbyPDOutBasePtr[pCmdDesc->dwOutOffset], pCmdDesc->wDataLen);
                }
                else
                {
                    OsMemset(pPtr, 0, pCmdDesc->wDataLen);
                }
                break;
            case eCycFrameLayout_FIXED:
            case eCycFrameLayout_IN_DMA:
                /* move pointer to payload */
                pPtr += ETYPE_EC_CMD_HEADER_LEN;
                break;
            default:
                OsDbgAssert(EC_FALSE);
                break;
            }
            /* handle ALStatusWkc aging */
            if (pCmdDesc->bReadAlStatus)
            {
                if (m_bLatchReceiveTimesRequest && (0 == dwCycEntryIndex))
                {
                    /* activate command to execute the latch receive times request. Latching the   */
                    /* receive times required an empty bus, because a previous frame could trigger */
                    /* the port receive time stamping.                                             */
                    /* This place is not absolutely safe, but should be enough in most of the case */
                    pCurCmd->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
                    EC_ICMDHDR_SET_ADDR_ADO(pCurCmd, ECM_DCS_REC_TIMEPORT0);
                }
                else
                {
                    pCurCmd->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BRD;
                    EC_ICMDHDR_SET_ADDR_ADO(pCurCmd, ECREG_AL_STATUS);

                    if (pCmdDesc->wALStatusAging != 0)
                    {
                        pCmdDesc->wALStatusAging--;
                        if (pCmdDesc->wALStatusAging == 0)
                        {
                            m_wALStatusWkc = 0;
                        }
                    }
                }
            }
#if (defined INCLUDE_DC_SUPPORT)
            /* command to latch port receive times */
            else if (pCmdDesc->bDcLatchReceiveTimes)
            {
                /* deactivate command */
                pCurCmd->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_NOP;
            }
            /* command to distribute system time over the bus */
            else if (pCmdDesc->bDcDistributeSystemTime)
            {
                m_pDistributedClks->DistributeSystemTimeSend(poLinkFrameDesc, bCycFramesAreProcessed, bStampJob, pCmdDesc, pCurCmd);
            }
            /* command to shift the system time */
            else if (pCmdDesc->bDcShiftSystemTime )
            {
                m_pDistributedClks->ShiftSystemTimeSend(pCmdDesc, pCurCmd);
            }
            /* command to monitor the system time difference */
            else if (pCmdDesc->bDcReadSystemTimeDiff)
            {
                m_pDistributedClks->ReadSystemTimeDifferenceSend(pCmdDesc, pCurCmd);
            }
#else
EC_UNREFPARM(pCurCmd);
EC_UNREFPARM(bCycFramesAreProcessed);
#endif /* INCLUDE_DC_SUPPORT */
            oCycDeviceFrameDesc.dwNumCmdsInFrame++;
            pPtr += pCmdDesc->wDataLen;      /* payload */
            /* WKC */
            EC_SET_FRM_WORD(pPtr, 0);
            pPtr += sizeof(EC_T_WORD);
            wCmdsLen = (EC_T_WORD)(wCmdsLen + sizeof(ETYPE_EC_CMD_HEADER) + pCmdDesc->wDataLen + sizeof(EC_T_WORD));
        } /* for wCmdIdx */

        if (oCycDeviceFrameDesc.dwNumCmdsInFrame != 0)
        {
            /* cyclic frames monitoring */
            pCyclicEntryMgmtDesc->byFramesMonitoringCycleIdx = m_byCurCycleIdx;
            pCyclicEntryMgmtDesc->wFramesMonitoringSndCnt++;
            pFrameDesc->bIsPending = EC_TRUE;

            /* fill EtherCAT header */
            EC_88A4HDR_RESET(p88A4Hdr);
            EC_88A4HDR_SET_E88A4HDRTYPE(p88A4Hdr, ETYPE_88A4_TYPE_ECAT);
            EC_88A4HDR_SET_E88A4FRAMELEN(p88A4Hdr, wCmdsLen);
            pFirstCmd->uCmdIdx.swCmdIdx.byIdx = (EC_T_BYTE)(pFirstCmd->uCmdIdx.swCmdIdx.byIdx + pCyclicEntryMgmtDesc->byFramesMonitoringCycleIdx * m_wAllCycEntriesNumFrames);

            /* send */
            oCycDeviceFrameDesc.oLinkFrameDesc.dwSize = EC_MAX(wCmdsLen + wHeaderLen, ETHERNET_MIN_FRAME_LEN);
            dwRes = m_poEcDevice->DeviceFrameSendAndFree(&oCycDeviceFrameDesc);
            if (EC_E_NOERROR != dwRes)
            {
                if ((dwRes == EC_E_DUPLICATE) && (m_poEcDevice->GetCheckCounter() > 333))
                {
                    dwRes = EC_E_EVAL_EXPIRED;
                }
                dwRetVal = dwRes;
                goto Exit;
            }
        }
        else
        {
            m_poEcDevice->DeviceFrameFree(&oCycDeviceFrameDesc);
        }
    } /*for (wFrameIdx=0; wFrameIdx < pCyclicEntryMgmtDesc->wNumFrames; wFrameIdx++ )*/

Exit:
#if (defined INCLUDE_VARREAD)
    if (bSwapped)
    {
        SwapData(pCyclicEntryMgmtDesc, EC_FALSE, pbyPDOutBasePtr);
    }
#endif
#ifdef INCLUDE_MEMORY_PROVIDER
    if (EC_NULL != pbyPDOutBasePtr)
    {
        ReturnPDOutBasePointer();
    }
#endif
    return dwRetVal;
}


/********************************************************************************/
/** \brief Send cyclic frames with IN_DMA layout
*
*          Only 1 frame sent currently
*
* \return N/A
*/
EC_T_DWORD CEcMaster::SendCycFramesInDma(EC_T_WORD* pwNumSendFrames)
{
EC_T_DWORD              dwRetVal             = EC_E_NOERROR;
EC_T_DWORD              dwRes                = EC_E_NOERROR;
CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc = &m_aCyclicEntryMgmtDesc[0]; /* TODO */
EcCycFrameDesc*         pFrameDesc           = pCyclicEntryMgmtDesc->apEcCycFrameDesc[0]; /* TODO */
EC_T_WORD               wCmdIdx              = 0;
EC_T_BOOL               bSendingFramesStarted = EC_FALSE;

    /* check if send is enabled */
    if (!m_poEcDevice->IsSendEnabled())
    {
        /* refresh ALStatusWkc */
        if (EC_AL_STATUS_INIT_VALUE != m_wALStatusWkc)
        {
            m_wALStatusWkc    = 0;
        }
        dwRetVal = EC_E_LINK_DISCONNECTED;
        goto Exit;
    }
    /* reset communication timeout checking */
    pCyclicEntryMgmtDesc->wFramesMonitoringSndCnt = 0;
    pCyclicEntryMgmtDesc->wFramesMonitoringRcvCnt = 0;

    /* detect cyclic frames change */
    {
    EC_T_WORD wCycFramesDeviceState = GetMasterHighestDeviceState();

        if (m_wCycFramesDeviceState != wCycFramesDeviceState)
        {
            m_wCycFramesDeviceState  = wCycFramesDeviceState;

            /* adjust cyclic frames according to master state */
            switch (m_wCycFramesDeviceState)
            {
            case DEVICE_STATE_UNKNOWN:
                dwRetVal = EC_E_INVALIDSTATE;
                goto Exit;
                /* no break */
            case DEVICE_STATE_INIT:   AdjustCycFramesToState(DEVICE_STATE_INIT);
                break;
            case DEVICE_STATE_PREOP:  AdjustCycFramesToState(DEVICE_STATE_PREOP);
                break;
            case DEVICE_STATE_SAFEOP: AdjustCycFramesToState(DEVICE_STATE_SAFEOP);
                break;
            case DEVICE_STATE_OP:     AdjustCycFramesToState(DEVICE_STATE_OP);
                break;
            default:
                break;
            }
        }
    }
    /* monitor read AlStatus command */
    for (wCmdIdx = 0; wCmdIdx < pFrameDesc->wCmdCnt; wCmdIdx++)
    {
        EcCycCmdConfigDesc* pCmdDesc = pFrameDesc->apCycCmd[wCmdIdx];

        /* handle ALStatusWkc aging */
        if (pCmdDesc->bReadAlStatus)
        {
            if (m_bLatchReceiveTimesRequest)
            {
                EC_T_BYTE* pbyFrame = GetPDOutPtr();

                /* activate command to execute the latch receive times request. Latching the   */
                /* receive times required an empty bus, because a previous frame could trigger */
                /* the port receive time stamping.                                             */
                /* This place is not absolutely safe, but should be enough in most of the case */
                pbyFrame[pCmdDesc->wCmdTypeOffset] = EC_CMD_TYPE_BWR;
                EC_SET_FRM_WORD((&pbyFrame[pCmdDesc->wCmdTypeOffset] + 4), ECM_DCS_REC_TIMEPORT0);
            }
            if (pCmdDesc->wALStatusAging != 0)
            {
                pCmdDesc->wALStatusAging--;
                if (pCmdDesc->wALStatusAging == 0)
                {
                    m_wALStatusWkc = 0;
                }
            }
        }
    }
    /* send cyclic frames */
    if (pFrameDesc->wSize != 0)
    {
    EC_T_LINK_FRAMEDESC oLinkFrameDesc;

        /* start sending cyclic frames */
        bSendingFramesStarted = EC_TRUE;
        m_poEcDevice->SetFramesType(EC_TRUE);

        /* cyclic frames monitoring */
        pCyclicEntryMgmtDesc->wFramesMonitoringSndCnt++;
        pFrameDesc->bIsPending = EC_TRUE;

        OsMemset(&oLinkFrameDesc, 0, sizeof(EC_T_LINK_FRAMEDESC));
        oLinkFrameDesc.dwSize       = pFrameDesc->wSize;
        oLinkFrameDesc.pbyFrame     = pFrameDesc->pbyFrame;

        /* copy data should be DWORD aligned */
        EC_T_WORD wCopySize = pFrameDesc->wSize;
        if ((wCopySize % sizeof(EC_T_DWORD)) != 0)
        {
            wCopySize = (EC_T_WORD)((pFrameDesc->wSize / sizeof(EC_T_DWORD) + 1) * sizeof(EC_T_DWORD));
        }

        OsMemcpy(pFrameDesc->pbyFrame, GetPDOutPtr(), wCopySize);

        dwRes = m_poEcDevice->DeviceFrameSend(&oLinkFrameDesc);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
        *pwNumSendFrames = 1;
    }

Exit:
    /* flush frames */
    if (bSendingFramesStarted)
    {
        m_poEcDevice->FlushFrames();
    }

    return dwRetVal;
}


/********************************************************************************/
/** \brief Send acyclic frames
*
* All pending acyclic frames are sent here.
*
* \return N/A
*/
EC_T_DWORD CEcMaster::SendAcycFrames(EC_T_WORD* pwNumSendAcycFrames)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_DWORD dwRes    = EC_E_ERROR;
EC_T_BOOL  bSendingFramesStarted = EC_FALSE;

    /* reset counter */
    *pwNumSendAcycFrames = 0;

    /* skip acyclic communication if receive time latching is requested to   */
    /* protect against disturbance (some IP core ESC cannot latch correctly) */
    if (m_bLatchReceiveTimesRequest)
    {
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }
    /* flush current pending frame */
    EcatFlushCurrPendingSlaveFrame();

    if (!m_poEcDevice->IsSendEnabled())
    {
        /* cancel queue frames */
        m_poEcDevice->CancelQueuedFrames(EC_E_LINK_DISCONNECTED);
    }
    else
    {
        /* start sending acyclic frames */
        bSendingFramesStarted = EC_TRUE;
        m_poEcDevice->SetFramesType(EC_FALSE);

        /* send queued frames */
        dwRes = m_poEcDevice->SendQueuedFrames(pwNumSendAcycFrames);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
#if (defined INCLUDE_DC_SUPPORT) && (defined INCLUDE_DC_ADD_ACYC_DISTRIBUTION)
        /* use acyclic free bandwidth to improve DC behavior on network */
        if ((0 == *pwNumSendAcycFrames) && m_pDistributedClks->m_bAcycDistributionActive)
        {
            if (EC_NULL != m_pDistributedClks->m_pBusSlaveRefClock)
            {
            EC_T_DWORD dwFrameIdx = 0;

                for (dwFrameIdx = 0; dwFrameIdx < EC_MAX(m_MasterConfig.dwMaxSentQueuedFramesPerCyc, 1); dwFrameIdx++)
                {
                    /* queue EtherCAT command */
                    dwRes = QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_DC_ACYCDISTRIBUTION, EC_CMD_TYPE_FRMW, m_pDistributedClks->m_pBusSlaveRefClock->GetFixedAddress(),
                        ECM_DCS_SYSTEMTIME, 8, EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        dwRetVal = dwRes;
                        goto Exit;
                    }
                    /* flush the frame because only one command can be processed at once */
                    EcatFlushCurrPendingSlaveFrame();
                }
            }
            dwRes = m_poEcDevice->SendQueuedFrames(pwNumSendAcycFrames);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        }
#endif /* INCLUDE_DC_SUPPORT */
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    /* flush frames */
    if (bSendingFramesStarted)
    {
        m_poEcDevice->FlushFrames();
    }

    OsDbgAssert(EC_E_ERROR != dwRetVal);

    return dwRetVal;
}

#ifdef REMOVE_MBX_READ_RETRIES
/********************************************************************************/
/** \brief This method is called when an acyclic lost frame is retried
*
* \return
*/
EC_T_VOID CEcMaster::CancelMbxCmds(CEcSlaveFrame* poEcSlaveFrame, EC_T_DWORD dwCmdResult)
{
PETYPE_EC_CMD_HEADER pEcCmdHeader = EC_NULL;

    OsDbgAssert(EC_NULL != poEcSlaveFrame);
    OsDbgAssert(0 != poEcSlaveFrame->m_nNumCmdsInFrame);
    for (EC_T_UINT i=0; i < poEcSlaveFrame->m_nNumCmdsInFrame; i++)
    {
        if (poEcSlaveFrame->m_aEcCmdDesc[i].pSlave)
        {
            /* frame originated from a CEcSlave object */
            if ( (poEcSlaveFrame->m_aEcCmdDesc[i].invokeId == eMbxInvokeId_RECV_FROM_SLAVE)
               ||(poEcSlaveFrame->m_aEcCmdDesc[i].invokeId == eMbxInvokeId_INITCMD_RECV_FROM_SLAVE)
               ||(poEcSlaveFrame->m_aEcCmdDesc[i].invokeId == eMbxInvokeId_TOGGLE_REPEAT_RES)
               ||(poEcSlaveFrame->m_aEcCmdDesc[i].invokeId == eMbxInvokeId_INITCMD_TOGGLE_REPEAT_RES)
              )
            {
                pEcCmdHeader = poEcSlaveFrame->m_aEcCmdDesc[i].pEcCmdHeader;
                OsDbgAssert( pEcCmdHeader != EC_NULL );
                if (pEcCmdHeader != EC_NULL )
                {
                    OsDbgAssert( pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd == EC_CMD_TYPE_FPRD);
                    if (pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd == EC_CMD_TYPE_FPRD)
                    {
                        poEcSlaveFrame->m_aEcCmdDesc[i].pSlave->ProcessCmdResult(dwCmdResult, poEcSlaveFrame->m_aEcCmdDesc[i].invokeId, pEcCmdHeader);
                        /* invalidate mailbox command */
                        pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_NOP;
#ifdef DEBUG
                        /* change compare info */
                        poEcSlaveFrame->m_aEcCmdDesc[i].cmd = EC_CMD_TYPE_NOP;
#endif
                        poEcSlaveFrame->m_aEcCmdDesc[i].invokeId = eMbxInvokeId_INVALIDATED_RECV_FROM_SLAVE;
                    }
                }
            }
        }
    }
}
#endif

/********************************************************************************/
/** \brief Cancel acyclic frames
*
* All queued acyclic frames will be canceled here.
*
* \return N/A
*/
EC_T_VOID CEcMaster::CancelAcycFrame(CEcSlaveFrame* poEcSlaveFrame, EC_T_DWORD dwCmdResult)
{
EC_T_DWORD dwCmdsIdx = 0;

    /* processed monitored frames */
    if (poEcSlaveFrame->m_retry != 0xFFFF)
    {
        for (dwCmdsIdx = 0; dwCmdsIdx < poEcSlaveFrame->m_nNumCmdsInFrame; dwCmdsIdx++)
        {
            if (poEcSlaveFrame->m_aEcCmdDesc[dwCmdsIdx].pSlave)
            {
                poEcSlaveFrame->m_aEcCmdDesc[dwCmdsIdx].pSlave->ProcessCmdResult(dwCmdResult, poEcSlaveFrame->m_aEcCmdDesc[dwCmdsIdx].invokeId, poEcSlaveFrame->m_aEcCmdDesc[dwCmdsIdx].pEcCmdHeader);
            }
            else
            {
                ProcessCmdResult(dwCmdResult, &poEcSlaveFrame->m_aEcCmdDesc[dwCmdsIdx], poEcSlaveFrame->m_aEcCmdDesc[dwCmdsIdx].pEcCmdHeader);
            }
        }
    }
    FreeSlaveFrameDesc(poEcSlaveFrame->m_pSlaveFrameDesc);
}

/********************************************************************************/
/** \brief Checks received ecat frame.
*
* This function is called by CEcDevice::ProcessFrame() after checking that it is a ecat frame.
*
* \return error code.
*/
EC_T_DWORD  CEcMaster::CheckFrame
(   PETHERNET_FRAME     pFrame,
    PETHERNET_FRAME     pRedFrame
)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
    EC_T_DWORD                  dwRetVal = EC_E_ERROR;
    EC_T_WORD*                  pwFrameType;
    PETHERNET_88A4_HEADER       pE88A4;
    EC_T_WORD                   wE88A4Len;
    EC_T_BYTE                   byFrameIdxFrm;
    PETYPE_EC_CMD_HEADER        pEcCmdHeader;
    PETYPE_EC_CMD_HEADER        pEcOrgCmdHeader;
    EC_T_NOTIFICATION_INTDESC*  pNotification;
#if (defined WITHALIGNMENT)
    static EC_T_WORD            s_awDatagramBuffer[ETHERNET_MAX_FRAME_LEN / sizeof(EC_T_WORD)] = {0};
#endif


#if (defined INCLUDE_RED_DEVICE)
    if (m_poEcDevice->m_bRedDeviceEnabled)
    {
        /* handle received frame(s) and detect line break */
        if ((EC_NULL != pRedFrame) && (EC_NULL != pFrame))
        {
#if 0       /* safe handling but doesn't work with HILSCHER slaves (they don't mark frame as processed) */
            if      (((pFrame->Source.b[0] & 0x0A) == 0x08) && ((pRedFrame->Source.b[0] & 0x0A) == 0x02))
            {
                /* main line received unprocessed red frame and red line received processed main frame -> no line break */
                /* if frame splitting is enabled don't disable it here but wait for AL status */
            }
            else if (((pFrame->Source.b[0] & 0x0A) == 0x02) && ((pRedFrame->Source.b[0] & 0x0A) == 0x0A))
            {
                /* main line received processed main frame and red line received processed red frame -> line break detected -> activate frame splitting */
                m_bRedFrameSplitEnabled = EC_TRUE;
            }
            else if (((pFrame->Source.b[0] & 0x0A) == 0x0A) && ((pRedFrame->Source.b[0] & 0x0A) == 0x02))
            {
                /* main line received processed red frame and red line received processed main frame -> IN/OUT crossed lines -> activate frame splitting */
                m_bRedFrameSplitEnabled = EC_TRUE;
            }
            else
            {
                /* can only happen if link are connected to a switch */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_NOERROR;
                goto Exit;
            }
#else
            /* TODO: m_bRedLineCrossed */
            if (((pFrame->Source.b[0] & 0x02) == 0x02) || ((pFrame->Source.b[0] & 0x08) == 0x00) || ((pRedFrame->Source.b[0] & 0x08) == 0x08))
            {
                /* main line receive processed frames (crossed) or main/red line receive main/red frames (line break) */
                /* -> activate frame splitting */
                m_bRedFrameSplitEnabled = EC_TRUE;
            }
#endif
        }
        else
        {
            /* one frame is missing -> line break detected -> activate frame splitting */
            m_bRedFrameSplitEnabled = EC_TRUE;
        }
        /* merge/check frame(s) if frame splitting is enabled */
        if (m_bRedFrameSplitEnabled)
        {
            RedBreakFrameMerge(pFrame, pRedFrame);

            if (EC_NULL == pFrame)
            {
                pFrame = pRedFrame;
            }
        }
        else
        {
            pFrame = pRedFrame;
            RedGetNumSlaves(pFrame);
        }
        /* at this point pFrame should point to the merged/full frame */
    }
#else
    EC_UNREFPARM(pRedFrame);
#endif

    pwFrameType = FRAMETYPE_PTR(pFrame);
    pE88A4      = (PETHERNET_88A4_HEADER)EC_ENDOF(pwFrameType);

    /* alignment safe if LinkLayer delivers frames aligned!! */
    wE88A4Len       = EC_AL_88A4HDR_GET_E88A4FRAMELEN(&(pE88A4->E88A4Header));
    pEcCmdHeader    = &pE88A4->sETECAT.FirstEcCmdHeader;
    pEcOrgCmdHeader = pEcCmdHeader;

#if (defined WITHALIGNMENT)
    /* not WORD aligned ? */
    if ((EC_T_DWORD)pEcOrgCmdHeader & 1)
    {
        OsDbgAssert(EC_FALSE);
        /* per-se not alignment safe !! */
        OsMemcpy(s_awDatagramBuffer, pEcOrgCmdHeader, EC_MIN((EC_ICMDHDR_GET_LEN_LEN(pEcOrgCmdHeader) + ETYPE_EC_OVERHEAD), sizeof(s_awDatagramBuffer)));
        pEcCmdHeader = (PETYPE_EC_CMD_HEADER)s_awDatagramBuffer;
    }

#endif

    byFrameIdxFrm = EC_AL_ICMDHDR_GET_CMDIDX_IDX(pEcCmdHeader);

    EC_TRACEMSG(
        EC_TRACE_CORE_RCV_CYCFRAMES|EC_TRACE_CORE_RCV_QUEFRAMES,
        ("RCV CmdIdx 0x%x\n", EC_AL_ICMDHDR_GET_CMDIDX(pEcCmdHeader))
               );

    /* acyclic frame response processing */
    if ((byFrameIdxFrm >= m_byMinAcycCmdIdx) && (byFrameIdxFrm < (m_byMinAcycCmdIdx + m_MasterConfig.dwMaxQueuedEthFrames)))
    {
        ECAT_SLAVEFRAME_DESC*       pSlaveFrameDesc = EC_NULL;
        CEcSlaveFrame*              poEcSlaveFrame  = EC_NULL;
        EC_T_UINT                   nLoopIndex                  = 0;

        PerfMeasStart(&G_TscMeasDesc, EC_MSMT_ProcessAcycFrames);

        /* the IDX values range is between m_byMinAcycCmdIdx and (m_byMinAcycCmdIdx + m_MasterConfig.dwMaxQueuedEthFrames) */
        EC_TRACEMSG( EC_TRACE_CORE_QUECMD_SEQ | EC_TRACE_CORE_SEQUENCE, ("====>[%06d] CEcMaster::CheckFrame()\n", OsQueryMsecCount()));

        /* look for matching pending frame */
        if (!IsListEmpty(&m_PendingSlaveFrameDescList))
        {
        EC_T_LISTENTRY* pListIterator = EC_NULL;

            for (pListIterator = GetListHeadEntry(&m_PendingSlaveFrameDescList);
                 pListIterator != &m_PendingSlaveFrameDescList;
                 pListIterator = GetListNextEntry(pListIterator))
            {
                pSlaveFrameDesc = (PECAT_SLAVEFRAME_DESC)pListIterator;
                poEcSlaveFrame = pSlaveFrameDesc->poEcSlaveFrame;

                if (poEcSlaveFrame->m_byEcCmdHeaderIDX == byFrameIdxFrm)
                {
                    break;
                }
            }
            if (pListIterator == &m_PendingSlaveFrameDescList)
            {
                /* no pending frame found */
                poEcSlaveFrame = EC_NULL;
            }
        }
        if ((EC_NULL == poEcSlaveFrame) || EC_ETHFRM_GET_RETRYINDEX(&(poEcSlaveFrame->m_EthernetFrame)) != EC_ETHFRM_GET_RETRYINDEX(pFrame))
        {
            /* no pending frame found or timed out response (retry frame sent) */
            NotifyFrameResponseError(0, 0, EC_FALSE, eRspErr_UNEXPECTED, 0, byFrameIdxFrm);
            dwRetVal = EC_E_INVALIDINDEX;
            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_ProcessAcycFrames);
            goto Exit;
        }
        /* process all commands */
        PerfMeasStart(&G_TscMeasDesc, EC_MSMT_ProcessAcycFramesCore);
        OsDbgAssert(poEcSlaveFrame->m_nNumCmdsInFrame <= m_MasterConfig.dwMaxSlaveCmdPerFrame);
        OsDbgAssert(0 != poEcSlaveFrame->m_nNumCmdsInFrame);
        for (nLoopIndex = 0; nLoopIndex < poEcSlaveFrame->m_nNumCmdsInFrame; nLoopIndex++)
        {
        PECAT_SLAVECMD_DESC pCmdDesc = &poEcSlaveFrame->m_aEcCmdDesc[nLoopIndex];
        EC_T_DWORD dwCurCmdResultCode = EC_E_NOERROR;
        EC_T_WORD  wEcCmdLen = 0;

            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_ProcessAcycFramesSingleCmd);

            /* preprocess command */
            if (EC_NULL != pEcCmdHeader)
            {
                 /* process interrupt field */
#if (defined WITHALIGNMENT)
                OsDbgAssert((((EC_T_DWORD)pEcCmdHeader)&1)==0);
#endif
                /* cmdHdr shall always be aligned here ! */
                wEcCmdLen = EC_AL_CMDHDRLEN_GET_LEN(&(pEcCmdHeader->uLen));
                if (EC_AL_ICMDHDR_GET_IRQ(pEcCmdHeader) != 0)
                {
                    ProcessIrq(EC_AL_ICMDHDR_GET_IRQ(pEcCmdHeader));
                }
                /* check if received command match pending one */
                if (
                      (pCmdDesc->cmd   != EC_AL_ICMDHDR_GET_CMDIDX_CMD(pEcCmdHeader))
                    ||(pCmdDesc->byIdx != EC_AL_ICMDHDR_GET_CMDIDX_IDX(pEcCmdHeader))
                    ||(pCmdDesc->ado   != EC_AL_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader))
                    ||(pCmdDesc->len   != EC_AL_ICMDHDR_GET_LEN_LEN(pEcCmdHeader))
                    )
                {
                    dwCurCmdResultCode = EC_E_FRAME_LOST;
                }
            }
            else
            {
                dwCurCmdResultCode = EC_E_CMD_MISSING;
            }
            /* notify initiator */
            if (pCmdDesc->pSlave)
            {
                if (  (pCmdDesc->invokeId == eMbxInvokeId_INVALIDATED_RECV_FROM_SLAVE)
                   || (pCmdDesc->invokeId == eMbxInvokeId_INVALIDATED_SEND_TO_SLAVE)
                   )
                {
                    /* skip canceled mailbox commands */
                    OsDbgAssert((pCmdDesc->cmd == EC_CMD_TYPE_NOP) || (pCmdDesc->cmd == EC_CMD_TYPE_FPRD));
                }
                else
                {
                    EC_TRACEMSG(EC_TRACE_CORE, ("CEcMaster::CheckFrame() Forward QueueEcatCmdReq to slave %s\n", pCmdDesc->pSlave->GetName()));
                    EC_TRACEMSG(EC_TRACE_CORE_QUECMD_SEQ, ("---->[%06d] CEcMaster::CheckFrame(): %s - invokeId=0x%x, cmd=%d, adp=0x%x, ado=0x%x\n",
                        OsQueryMsecCount(), pCmdDesc->pSlave->GetName(), pCmdDesc->invokeId,
                        (EC_T_DWORD)pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd,
                        (EC_T_DWORD)EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader),
                        (EC_T_DWORD)EC_AL_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader)
                        ));
                    PerfMeasStart(&G_TscMeasDesc, EC_MSMT_ProcessSlaveCmd);
                    pCmdDesc->pSlave->ProcessCmdResult(dwCurCmdResultCode, pCmdDesc->invokeId, pEcCmdHeader);
                    PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_ProcessSlaveCmd);
                }
            }
            else
            {
                EC_TRACEMSG(EC_TRACE_CORE, ("CEcMaster::CheckFrame() Master frame\n"));
                EC_TRACEMSG(EC_TRACE_CORE_QUECMD_SEQ, ("---->[%06d] CEcMaster::CheckFrame(): MasterFrame - invokeId=0x%x, cmd=%d, adp=0x%x, ado=0x%x\n",
                    OsQueryMsecCount(), pCmdDesc->invokeId, (EC_T_DWORD)pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd,
                    (EC_T_DWORD)(EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader)),
                    (EC_T_DWORD)(EC_AL_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader))
                    ));
                PerfMeasStart(&G_TscMeasDesc, EC_MSMT_ProcessMasterCmd);
                ProcessCmdResult(dwCurCmdResultCode, pCmdDesc, pEcCmdHeader);
                PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_ProcessMasterCmd);
            }
            /* move to next command in received frame */
            if ((EC_NULL != pEcCmdHeader) && (EC_AL_CMDHDRLEN_GET_NEXT(&(pEcCmdHeader->uLen))))
            {
                pEcCmdHeader = pEcOrgCmdHeader = (PETYPE_EC_CMD_HEADER)(((EC_T_BYTE*)pEcOrgCmdHeader) + wEcCmdLen + ETYPE_EC_OVERHEAD);
#if (defined WITHALIGNMENT)
                /* not WORD aligned ? */
                if ((EC_T_DWORD)pEcOrgCmdHeader & 1)
                {
                    OsMemcpy(s_awDatagramBuffer, pEcOrgCmdHeader, EC_MIN((EC_ICMDHDR_GET_LEN_LEN(pEcOrgCmdHeader) + ETYPE_EC_OVERHEAD), sizeof(s_awDatagramBuffer)));
                    pEcCmdHeader = (PETYPE_EC_CMD_HEADER)s_awDatagramBuffer;
                }
#endif
            }
            else
            {
                pEcCmdHeader = EC_NULL;
            }
        }
        /* wait repeat count reach zero */
        if (1 >= pSlaveFrameDesc->dwRepeatCnt)
        {
            FreeSlaveFrameDesc(poEcSlaveFrame->m_pSlaveFrameDesc);
            poEcSlaveFrame = EC_NULL;
        }
        else
        {
            pSlaveFrameDesc->dwRepeatCnt--;
        }
        PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_ProcessAcycFramesCore);

        PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_ProcessAcycFrames);
    } /* acyclic frame response processing */

    /* cyclic frame response processing */
    else if (byFrameIdxFrm <= m_byMaxCycCmdIdx)
    {
        PerfMeasStart(&G_TscMeasDesc, EC_MSMT_ProcessCycFrames);
        if (0 != m_wAllCycEntriesNumFrames)
        {
        EC_T_BYTE byCurCycleIdx = (EC_T_BYTE)(byFrameIdxFrm / m_wAllCycEntriesNumFrames);
        EC_T_BYTE byFrameIdx = (EC_T_BYTE)(byFrameIdxFrm % m_wAllCycEntriesNumFrames);
        EC_T_BOOL bFrameIsValid = EC_FALSE;
        CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc;
        EcCycFrameDesc* pEcCycFrameDesc = EC_NULL;

            /* determine the corresponding cyclic entry index and frame index */
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
            EC_T_DWORD              dwCycEntryIndex;
            for (dwCycEntryIndex = 0, pCyclicEntryMgmtDesc = m_aCyclicEntryMgmtDesc; dwCycEntryIndex < m_dwNumCycConfigEntries; dwCycEntryIndex++, pCyclicEntryMgmtDesc++)
#else
            pCyclicEntryMgmtDesc = m_aCyclicEntryMgmtDesc;
#endif
            {
                if ((byFrameIdx >= pCyclicEntryMgmtDesc->byFrameIdxFirst) && (byFrameIdx <= pCyclicEntryMgmtDesc->byFrameIdxLast))
                {
                    for (EC_T_DWORD dwCycFrmIndex = 0; dwCycFrmIndex < pCyclicEntryMgmtDesc->wNumFrames; dwCycFrmIndex++)
                    {
                        pEcCycFrameDesc = pCyclicEntryMgmtDesc->apEcCycFrameDesc[dwCycFrmIndex];
                        if (byFrameIdx == pEcCycFrameDesc->byCmdHdrIdx)
                        {
                            bFrameIsValid = EC_TRUE;
                            break;
                        }
                    }
                    if (byCurCycleIdx != pCyclicEntryMgmtDesc->byFramesMonitoringCycleIdx)
                    {
                        bFrameIsValid = EC_FALSE;
                    }
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
                    break;
#endif
                }
            }
            if (bFrameIsValid)
            {
            EC_T_BYTE byCycCmdIdx = 0;

                pCyclicEntryMgmtDesc->wFramesMonitoringRcvCnt++;
                pEcCycFrameDesc->bIsPending = EC_FALSE;

                /* move this to a different place after interrupt mode (SyncSm) refactoring */
                if (!m_bHavePdWriteAccess)
                    GetPDInBasePointer();   /* Fix: #00913 */

                /* process all cyclic commands in the frame */
                while (EC_NULL != pEcCmdHeader)
                {
                EcCycCmdConfigDesc* pCmdDesc = EC_NULL;
                EC_T_WORD wEcCmdLen = EC_AL_CMDHDRLEN_GET_LEN(&(pEcCmdHeader->uLen));

                    /* look for each EtherCAT sub telegram */
                    if (wE88A4Len < ETYPE_EC_OVERHEAD + wEcCmdLen)
                    {
                        EC_ERRORMSG(("CEcMaster::CheckFrame(): corrupt frame!\n"));
                        break;
                    }
                    if (0 != EC_AL_ICMDHDR_GET_IRQ(pEcCmdHeader))
                    {
                        ProcessIrq(EC_AL_ICMDHDR_GET_IRQ(pEcCmdHeader));
                    }
                    /* get command descriptor */
                    switch (m_eCycFrameLayout)
                    {
                    case eCycFrameLayout_STANDARD:
                    case eCycFrameLayout_DYNAMIC:
                        /* skip inactive command because there are not sent on the network */
                        for (pCmdDesc = pEcCycFrameDesc->apCycCmd[byCycCmdIdx];
                            (byCycCmdIdx < pEcCycFrameDesc->wCmdCnt) && (EC_NULL != pCmdDesc);
                             byCycCmdIdx++, pCmdDesc = pEcCycFrameDesc->apCycCmd[byCycCmdIdx])
                        {
                            if (pCmdDesc->bActive)
                            {
                                break;
                            }
                        }
                        /* sanity check */
                        if (byCycCmdIdx >= pEcCycFrameDesc->wCmdCnt)
                        {
                            /* can happen if masters are started concurrently at same bus
                               concurrent access can be detected by checking MAC address */
                            OsDbgAssert(EC_FALSE);
                            pCmdDesc = EC_NULL;
                        }
                        break;
                    case eCycFrameLayout_FIXED:
                    case eCycFrameLayout_IN_DMA:
                        pCmdDesc = pEcCycFrameDesc->apCycCmd[byCycCmdIdx];
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        break;
                    }
                    if (EC_NULL != pCmdDesc)
                    {
                        if (pCmdDesc->bActive)
                        {
                        EC_T_BYTE byWkcState = 0; /* 0 = data valid */

                            /* check WKC */
                            pCmdDesc->wLastWkc = EC_GET_FRM_WORD((((EC_T_BYTE*)pEcCmdHeader) + pCmdDesc->wEcCmdWkcOffset));
                            if (pCmdDesc->bCheckWkc)
                            {
                                if ((pCmdDesc->wLastWkc != pCmdDesc->wExpectedWkc) || (0 == pCmdDesc->wLastWkc))
                                {
                                    byWkcState = 1; /* 1 = data invalid */
                                }
#if (defined INCLUDE_WKCSTATE)
                                if(byWkcState == 0)  
                                    EC_CLRBIT(m_pbyDiagnosisImage, pCmdDesc->wWkcStateDiagOffs);
                                else 
                                    EC_SETBIT(m_pbyDiagnosisImage, pCmdDesc->wWkcStateDiagOffs);
#endif /* INCLUDE_WKCSTATE */
                            }
                            if (pCmdDesc->bCopyInputs && (EC_NULL != m_pbyPDInBasePtr))
                            {
                                EC_T_BOOL bCopyInputs = EC_FALSE;
                                EC_T_BOOL bSetZeroInputs = EC_FALSE;

                                if (0 == byWkcState)
                                {
                                    bCopyInputs = EC_TRUE;
                                }
                                else
                                {
                                    if (m_bZeroInputsOnWkcError)
                                    {
                                        bSetZeroInputs = EC_TRUE;
                                    }
                                    else
                                    {
                                        if (!m_bIgnoreInputsOnWkcError)
                                        {
                                            /* legacy behavior: input data are copied if WKC is not zero and below expected value */
                                            if ((0 != pCmdDesc->wLastWkc) && (pCmdDesc->wLastWkc <= pCmdDesc->wConfiguredWkc))
                                            {
                                                bCopyInputs = EC_TRUE;
                                            }
                                        }

                                        if (m_bZeroInputsOnWkcZero && (0 == pCmdDesc->wLastWkc))
                                        {
                                            bSetZeroInputs = EC_TRUE;
                                        }
                                    }
                                }
                                if (bCopyInputs)
                                {
                                    OsMemcpyPdIn(((EC_T_PBYTE)(m_pbyPDInBasePtr+(pCmdDesc->dwInOffset))), (EC_T_BYTE*)EC_ENDOF(pEcCmdHeader), wEcCmdLen);
                                }
                                else if (bSetZeroInputs)
                                {
                                    OsMemsetPdIn(((EC_T_PBYTE)(m_pbyPDInBasePtr + (pCmdDesc->dwInOffset))), 0, wEcCmdLen);
                                }
                            }
                            if (pCmdDesc->bReadAlStatus)
                            {
                                if (  (EC_CMD_TYPE_BWR       == pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd)
                                   && (ECM_DCS_REC_TIMEPORT0 == EC_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader)))
                                {
                                    m_bLatchReceiveTimesRequest = EC_FALSE;
                                    if (eCycFrameLayout_IN_DMA == GetCycFrameLayout())
                                    {
                                    EC_T_BYTE* pbyFrame = GetPDOutPtr();

                                        pbyFrame[pCmdDesc->wCmdTypeOffset] = EC_CMD_TYPE_BRD;
                                        EC_SET_FRM_WORD((&pbyFrame[pCmdDesc->wCmdTypeOffset] + 4), ECREG_AL_STATUS);
                                    }
                                }
                                else
                                {
                                    /* this command reads the AL STATUS, checks if all devices are still in OP state or the ERROR flag is set */
                                    /* pCmdDesc->pbyCycInputs points to image offset where AL_STATUS is located
                                    * offset is set in XML file at Config/CyclicFrame/Cmd/InputOffs of the BRD AL STATUS command */
                                    m_wAlStatusReg   = (EC_T_WORD)(EC_GET_FRM_WORD(EC_ENDOF(pEcCmdHeader)) & ~DEVICE_STATE_IDREQUEST);
                                    m_wALStatusWkc   = ETYPE_EC_CMD_GETWKC(pEcCmdHeader);
                                    m_wALStatusExpectedWkc = pCmdDesc->wExpectedWkc;
                                    pCmdDesc->wALStatusAging = ALSTATUS_AGING;
#if (defined INCLUDE_MASTER_OBD)
                                    m_pSDOService->m_bIsAlStatusWkcOk  = (m_wALStatusWkc == m_wALStatusExpectedWkc) ? EC_TRUE : EC_FALSE;
                                    m_pSDOService->m_wAlStatusBrdValue = m_wAlStatusReg;
#endif
                                    /* check if the error bit is set, only if master is not in INIT state */
                                    if ((m_wAlStatusReg & DEVICE_STATE_ERROR) && (GetMasterLowestDeviceState() != DEVICE_STATE_INIT))
                                    {
                                        /* store Error flag, it is processed in next Timer Tick by master state machine */
                                        SetSlaveErrorDetected(EC_TRUE);

                                        if (m_dwCycErrorNotifyMask & EC_CYC_ERR_MASK_STATUS_SLAVE_ERROR)
                                        {
                                            EC_ERRORMSGL( ("CEcMaster::CheckFrame() at least one EtherCAT device is in error state!\n") );
                                            pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
                                            if (EC_NULL != pNotification)
                                            {
                                                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode  = EC_NOTIFY_STATUS_SLAVE_ERROR;
                                                NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, sizeof(EC_T_DWORD));
                                            }
#ifdef DEBUG
                                            {
                                                if (OsQueryMsecCount() > (m_dwPrintErrStateTime + 2000))
                                                {
                                                    EC_ERRORMSGC_L( ("CEcMaster::CheckFrame() at least one EtherCAT device is in error state!\n") );
                                                    m_dwPrintErrStateTime = OsQueryMsecCount();
                                                }
                                                else if (OsQueryMsecCount() < m_dwPrintErrStateTime)
                                                {
                                                    /* time wrapped, initialize */
                                                    m_dwPrintErrStateTime = OsQueryMsecCount();
                                                }
                                            }
#endif
                                        }
                                    }
                                }
                            }
#if (defined INCLUDE_DC_SUPPORT)
                            /* command to latch port receive times */
                            else if (pCmdDesc->bDcLatchReceiveTimes)
                            {
                                /* nothing to do */
                            }
                            /* command to distribute system time over the bus */
                            else if (pCmdDesc->bDcDistributeSystemTime)
                            {
                                m_pDistributedClks->DistributeSystemTimeProcess(pCmdDesc, pEcCmdHeader);
                            }
                            /* command to monitor the system time difference */
                            else if (pCmdDesc->bDcReadSystemTimeDiff)
                            {
                                m_pDistributedClks->RefreshSyncWindowMonitoring(EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader), EC_GET_FRM_DWORD(EC_ENDOF(pEcCmdHeader)));
                            }
#endif /* INCLUDE_DC_SUPPORT */
                            else if (pCmdDesc->bIsLogicalMBoxStateCmd)
                            {
                                /* command is responsible for reading out the state of the mailbox -> check if the mailbox of a slave has to be read out */
                                EC_TRACEMSG(EC_TRACE_CORE, ("CEcMaster::CheckFrame() Read state of mailbox\n"));

                                CheckLogicalStateMBox(pEcCmdHeader, wEcCmdLen);
                            } /* bIsLogicalMBoxStateCmd */

                            /* notify application if process data are invalid */
                            if ((1 == byWkcState) && (0 == m_dwCycErrorNotifyMaskedCnt) && (0 != (m_dwCycErrorNotifyMask & EC_CYC_ERR_MASK_CYCCMD_WKC_ERROR)))
                            {
                            EC_T_BOOL bDoNotifyCycCmdWkcError = EC_TRUE;

                                if (pCmdDesc->bReadAlStatus)
                                {
                                    bDoNotifyCycCmdWkcError = EC_FALSE;
                                }
                                if (!m_bStateMachIsStable)
                                {
                                    bDoNotifyCycCmdWkcError = EC_FALSE;
                                }
                                if (m_wCycFramesDeviceState != m_wCurMasterDeviceState)
                                {
                                    bDoNotifyCycCmdWkcError = EC_FALSE;
                                }
                                /* do not notify an WKC error in case of an LWR command in SAFEOP state */
                                if ((pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd == EC_CMD_TYPE_LWR) && (GetMasterLowestDeviceState() != DEVICE_STATE_OP))
                                {
                                    bDoNotifyCycCmdWkcError = EC_FALSE;
                                }
#if (defined INCLUDE_HOTCONNECT)
                                if (pCmdDesc->bIsHcSlaveInfluence)
                                {
                                    if (m_pBusTopology->TopologyIsChanging() || TopologyChangedDetected() || m_oHotConnect.TopologyChangeInProgress() || m_oHotConnect.WaitingForManualContinue())
                                    {
                                        bDoNotifyCycCmdWkcError = EC_FALSE;
                                    }
                                }
#endif
                                if (0 == pCmdDesc->wExpectedWkc)
                                {
                                    bDoNotifyCycCmdWkcError = EC_FALSE;
                                }

                                if (bDoNotifyCycCmdWkcError)
                                {
                                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + NOTIFICATION), "Notify EC_NOTIFY_CYCCMD_WKC_ERROR (%d, 0x%x, %d, %d)\n", 
                                        pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd, EC_ICMDHDR_GET_ADDR(&(pCmdDesc->EcCmdHeader)), pCmdDesc->wLastWkc, pCmdDesc->wExpectedWkc));

                                    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
                                    if (EC_NULL != pNotification)
                                    {
                                        pNotification->uNotification.ErrorNotification.dwNotifyErrorCode  = EC_NOTIFY_CYCCMD_WKC_ERROR;
                                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.byCmd   = pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd;
                                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.dwAddr  = EC_ICMDHDR_GET_ADDR(&(pCmdDesc->EcCmdHeader));
                                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcAct = pCmdDesc->wLastWkc;
                                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcSet = pCmdDesc->wExpectedWkc;
                                        NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_WKCERR);
                                    }
                                }
                            }
                        } /* if (bActive) */
                    } /* if (pCmdDesc != EC_NULL) */
                    else if (m_dwCycErrorNotifyMask & EC_CYC_ERR_MASK_UNEXPECTED_FRAME_RESPONSE)
                    {
                        NotifyFrameResponseError(0, 0, EC_TRUE, eRspErr_UNEXPECTED, 0, byFrameIdxFrm);
                    }

                    /* iterate to next ecat command */
                    wE88A4Len = (EC_T_WORD)(wE88A4Len - (ETYPE_EC_OVERHEAD + wEcCmdLen));

                    if (EC_AL_CMDHDRLEN_GET_NEXT(&(pEcCmdHeader->uLen)))
                    {
                        /* get next sub command */
                        pEcCmdHeader = pEcOrgCmdHeader = (PETYPE_EC_CMD_HEADER)((EC_T_BYTE*)pEcOrgCmdHeader + wEcCmdLen + ETYPE_EC_OVERHEAD);
#if (defined WITHALIGNMENT)
                        /* not WORD aligned ? */
                        if ((EC_T_DWORD)pEcOrgCmdHeader & 1)
                        {
                            OsMemcpy(s_awDatagramBuffer, pEcOrgCmdHeader, EC_MIN((EC_ICMDHDR_GET_LEN_LEN(pEcOrgCmdHeader) + ETYPE_EC_OVERHEAD), sizeof(s_awDatagramBuffer)));
                            pEcCmdHeader = (PETYPE_EC_CMD_HEADER)s_awDatagramBuffer;
                        }
#endif
                        /* move to next command in list */
                        byCycCmdIdx++;
                    }
                    else
                    {
                        pEcCmdHeader = EC_NULL;
                    }
                } /* while ( pEcCmdHeader ) */

                /* check if last frame in this cyclic entry is received */
                if (pCyclicEntryMgmtDesc->wFramesMonitoringRcvCnt == pCyclicEntryMgmtDesc->wFramesMonitoringSndCnt)
                {
#if (defined INCLUDE_VARREAD)
                    SwapData(pCyclicEntryMgmtDesc, EC_TRUE, m_pbyPDInBasePtr);
#endif
                    /* Copy information for Slave-to-Slave communication */
                    if ((0 != pCyclicEntryMgmtDesc->wNumOfCopyInfos) && !m_bCopyInfoInSendCycFrames)
                    {
                    EC_T_BYTE* pbyPDOutBasePtr = GetPDOutBasePointer();

                        if (EC_NULL != pbyPDOutBasePtr)
                        {
                            S2SCopyData(pCyclicEntryMgmtDesc, m_pbyPDInBasePtr, pbyPDOutBasePtr);
#ifdef INCLUDE_MEMORY_PROVIDER
                            ReturnPDOutBasePointer();
#endif
                        }
                    }

#if (defined INCLUDE_FORCE_PROCESSDATA)
                    if(m_wForceProcessDataEntries != 0)
                       ForceProcessDataInputs(m_pbyPDInBasePtr);
#endif

                    /* move this to a different place after interrupt mode (SyncSm) refactoring */
                    ReturnPDInBasePointer();

                    /* inform application that all cyclic frames are received ? */
                    CycFrameReceivedCallback();

                    /* bus load measurement */
#if (defined INCLUDE_MASTER_OBD)
                    BusLoadMeaCycleComplete();
#endif
                }
            } /* if (bFrameIsValid) */
            else
            {
                NotifyFrameResponseError(0, 0, EC_TRUE, eRspErr_UNEXPECTED, 0, byFrameIdxFrm);
            }
        }
        PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_ProcessCycFrames);
    } /* cyclic frame response processing */

    /* unexpected frame */
    else
    {
        NotifyFrameResponseError(0, 0, EC_TRUE, eRspErr_WRONG_IDX, 0, byFrameIdxFrm);
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
#if (defined INCLUDE_RED_DEVICE)
    if (m_bRedNotify)
    {
        pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
        if (EC_NULL != pNotification)
        {
            if (m_bRedLineBreak)
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+NOTIFICATION), "Notify EC_NOTIFY_RED_LINEBRK (%d, %d)\n", m_wRedNumSlavesOnMain, m_wRedNumSlavesOnRed));
                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_RED_LINEBRK;
            }
            else
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+NOTIFICATION), "Notify EC_NOTIFY_RED_LINEFIXED\n"));
                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_RED_LINEFIXED;
            }
            pNotification->uNotification.ErrorNotification.desc.RedChangeDesc.wNumOfSlavesMain = m_wRedNumSlavesOnMain;
            pNotification->uNotification.ErrorNotification.desc.RedChangeDesc.wNumOfSlavesRed  = m_wRedNumSlavesOnRed;
            NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_RED_CHANGE);
        }
        m_bRedNotify = EC_FALSE;
    }
#endif
    return dwRetVal;
}


#if (defined INCLUDE_RED_DEVICE)
/********************************************************************************/
/** \brief Redundancy: In case of line break, merge frames
*
* Note:
* Auto Inc Addresses
* 1. Slave -->  0x0000 -->  0 dez.
* 2. Slave -->  0xFFFF --> -1 dez.
* 3. Slave -->  0xFFFE --> -2 dez.

* Without break
* 0x0000(0)    0xFFFF(-1)    0xFFFE(-2)    0xFFFD(-3)    0xFFFC(-4)    0xFFFB(-5)    0xFFFA(-6)
* Break between Slave 3(-2) and 4(-3)
* number of slave on main: 3
* 0x0000(0)    0xFFFF(-1)    0xFFFE(-2)    0x0000( 0)    0xFFFF(-1)    0xFFFE(-2)    0xFFFD(-3)
*
* \return
*/
EC_T_VOID CEcMaster::RedBreakFrameMerge
(   PETHERNET_FRAME     pFrame,
    PETHERNET_FRAME     pRedFrame
)
{
    PETHERNET_88A4_HEADER pMain88A4        = EC_NULL;
    PETHERNET_88A4_HEADER pRed88A4         = EC_NULL;
    PETYPE_EC_CMD_HEADER  pCmdHeader       = EC_NULL;
    PETYPE_EC_CMD_HEADER  pMainCmdHeader   = EC_NULL;
    PETYPE_EC_CMD_HEADER  pRedCmdHeader    = EC_NULL;
    EC_T_BOOL             bFramesReceivedOnSameLine = EC_TRUE;
    EC_T_WORD             wEcCmdLen        = 0;
    EC_T_WORD             wMainWkc         = 0;
    EC_T_WORD             wRedWkc          = 0;
    EC_T_WORD             wNumSlavesOnMain = 0;
    EC_T_WORD             wNumSlavesOnRed  = 0;
    EC_T_WORD             wAdp             = 0;
    EC_T_BYTE*            pbyData          = EC_NULL;
    EC_T_BYTE*            pbyRedData       = EC_NULL;
    EC_T_INT              nCnt             = 0;

    /* process frames */
    if (EC_NULL != pRedFrame)
    {
        if ((pRedFrame->Source.b[0] & 0x08) != 0x08)
        {
            bFramesReceivedOnSameLine = EC_FALSE;
        }
        pRed88A4       = (PETHERNET_88A4_HEADER)EC_ENDOF(FRAMETYPE_PTR(pRedFrame));
        pRedCmdHeader  = &pRed88A4->sETECAT.FirstEcCmdHeader;
    }
    if (EC_NULL != pFrame)
    {
        if ((pFrame->Source.b[0] & 0x08) == 0x08)
        {
            bFramesReceivedOnSameLine = EC_FALSE;
        }
        pMain88A4      = (PETHERNET_88A4_HEADER)EC_ENDOF(FRAMETYPE_PTR(pFrame));
        pMainCmdHeader = &pMain88A4->sETECAT.FirstEcCmdHeader;
    }
    if ((EC_NULL == pMainCmdHeader) || (EC_NULL == pRedCmdHeader))
    {
        /* Both frame pointers can't be NULL */
        OsDbgAssert((EC_NULL != pMainCmdHeader) || (EC_NULL != pRedCmdHeader));

        /* early line break/fixed detection, line break flag and counters will be set with the next AL_STATUS BRD command */
        if (!m_bRedLineBreak)
        {
            m_bRedLineChanging = EC_TRUE;
        }
        /* one of the frame pointer is missing -> check frame */
        if (EC_NULL != pMainCmdHeader)
        {
            pCmdHeader = pMainCmdHeader;
        }
        else
        {
            pCmdHeader = pRedCmdHeader;
        }
        /* check commands */
        while (EC_NULL != pCmdHeader)
        {
            wEcCmdLen = EC_CMDHDRLEN_GET_LEN(&(pCmdHeader->uLen));

            switch (pCmdHeader->uCmdIdx.swCmdIdx.byCmd)
            {
            case EC_CMD_TYPE_BRD:
                if(ECREG_AL_STATUS == EC_ICMDHDR_GET_ADDR_ADO(pCmdHeader))
                {
                    if (bFramesReceivedOnSameLine)
                    {
                        if (EC_NULL != pMainCmdHeader)
                        {
                            wMainWkc = ETYPE_EC_CMD_GETWKC(pCmdHeader);
                            wRedWkc  = 0;
                        }
                        else
                        {
                            wMainWkc = 0;
                            wRedWkc  = ETYPE_EC_CMD_GETWKC(pCmdHeader);
                        }
                        /* monitor line break changes */
                        if (m_bRedLineChanging)
                        {
                            m_bRedLineChanging = EC_FALSE;
                            m_bRedNotify       = m_bRedNotifyActive;
                            m_bRedLineBreak    = EC_TRUE;

                            /* don't wait for DL status IRQ */
                            if (m_bRedNotifyActive)
                            {
                                m_pBusTopology->SetTopologyChanged();
                            }
                        }
                        /* determine number of slaves reachable on the main/red lines */
                        wNumSlavesOnMain = wMainWkc;
                        wNumSlavesOnRed  = wRedWkc;

                        /* monitor line break moves */
                        if (m_bRedLineBreak && ((m_wRedNumSlavesOnMain != wNumSlavesOnMain) || (m_wRedNumSlavesOnRed != wNumSlavesOnRed)))
                        {
                            m_wRedNumSlavesOnMain = wNumSlavesOnMain;
                            m_wRedNumSlavesOnRed  = wNumSlavesOnRed;
                            m_bRedNotify          = m_bRedNotifyActive;
#if (defined INCLUDE_MASTER_OBD)
                            RedUpdateOd();
#endif
                        }
                    }
                }
                break;
            default:
                break;
            }
            /* get next sub command */
            if (EC_CMDHDRLEN_GET_NEXT(&(pCmdHeader->uLen)))
            {
                pCmdHeader = (PETYPE_EC_CMD_HEADER)((EC_T_BYTE*)pCmdHeader + wEcCmdLen + ETYPE_EC_OVERHEAD);
            }
            else
            {
                pCmdHeader = EC_NULL;
            }
        }
    }
    else /* ((EC_NULL == pMain88A4) || (EC_NULL == pRed88A4)) */
    {
        /* TODO: use bFramesReceivedOnSameLine? */
        /* early line break/fixed detection, line break flag and counters will be set with the next AL_STATUS BRD command */
        if (  ((pFrame->Source.b[0]    & 0x02) == 0x02) && ((pFrame->Source.b[0]    & 0x08) == 0x00)
           && ((pRedFrame->Source.b[0] & 0x02) == 0x02) && ((pRedFrame->Source.b[0] & 0x08) == 0x08)
           )
        {
            if (!m_bRedLineBreak)
            {
                m_bRedLineChanging = EC_TRUE;
            }
        }
        else
        {
            if (m_bRedLineBreak)
            {
                m_bRedLineChanging = EC_TRUE;
            }
#if (defined INCLUDE_DC_SUPPORT)
            /* reset redundancy reference clock as soon as possible to protect for wrong handling by the next line break */
            m_pDistributedClks->m_pRedBusSlaveRefClock = EC_NULL;
#endif
        }
        /* merge frames and check commands */
        while ((EC_NULL != pMainCmdHeader) && (EC_NULL != pRedCmdHeader))
        {
            wEcCmdLen  = EC_CMDHDRLEN_GET_LEN(&(pMainCmdHeader->uLen));
            wMainWkc   = ETYPE_EC_CMD_GETWKC(pMainCmdHeader);
            wRedWkc    = ETYPE_EC_CMD_GETWKC(pRedCmdHeader);
            pbyRedData = (EC_T_BYTE*)EC_ENDOF(pRedCmdHeader);
            pbyData    = (EC_T_BYTE*)EC_ENDOF(pMainCmdHeader);

            /* TODO: check if MAIN header match RED (consider NOP) */

            /* merge IRQ field */
            EC_ICMDHDR_SET_IRQ(pMainCmdHeader, ((EC_T_WORD)((EC_ICMDHDR_GET_IRQ(pMainCmdHeader) | EC_ICMDHDR_GET_IRQ(pRedCmdHeader)))));

            switch( pMainCmdHeader->uCmdIdx.swCmdIdx.byCmd )
            {
            case EC_CMD_TYPE_NOP:
                switch( pRedCmdHeader->uCmdIdx.swCmdIdx.byCmd )
                {
                case EC_CMD_TYPE_APRD:
                case EC_CMD_TYPE_APWR:
                case EC_CMD_TYPE_APRW:
                case EC_CMD_TYPE_ARMW:
                    /* restore command */
                    pMainCmdHeader->uCmdIdx.swCmdIdx.byCmd = pRedCmdHeader->uCmdIdx.swCmdIdx.byCmd;

                    /* set ADP */
                    wAdp = EC_ICMDHDR_GET_ADDR_ADP(pRedCmdHeader);
                    if (!bFramesReceivedOnSameLine)
                    {
                        wAdp = (EC_T_WORD)(wAdp + m_wRedNumSlavesOnRed);
                    }
                    EC_ICMDHDR_SET_ADDR_ADP(pMainCmdHeader, wAdp);
                    /* copy data */
                    for (nCnt=0; nCnt < wEcCmdLen; nCnt++ )
                    {
                        *pbyData++ = *pbyRedData++;
                    }
                    /* set WKC */
                    ETYPE_EC_CMD_SETWKC(pMainCmdHeader, wRedWkc);
                    break;

                default:
                    break;
                }
                break; /* EC_CMD_TYPE_NOP */

            case EC_CMD_TYPE_APRD:
            case EC_CMD_TYPE_APWR:
            case EC_CMD_TYPE_APRW:
            case EC_CMD_TYPE_ARMW:
                /* correct ADP */
                wAdp = EC_ICMDHDR_GET_ADDR_ADP(pMainCmdHeader);
                if (bFramesReceivedOnSameLine)
                {
                    wAdp = (EC_T_WORD)(wAdp + m_wRedNumSlavesOnRed);
                }
                EC_ICMDHDR_SET_ADDR_ADP(pMainCmdHeader, wAdp);
                break;

            case EC_CMD_TYPE_FPRD:
                /* combine data */
                if ((wRedWkc != 0) && (wMainWkc != 0))
                {
                    for (nCnt=0; nCnt < wEcCmdLen; nCnt++ )
                    {
                        *pbyData++ |= *pbyRedData++;
                    }
                }
                else if (wRedWkc != 0)
                {
                    for (nCnt=0; nCnt < wEcCmdLen; nCnt++ )
                    {
                        *pbyData++ = *pbyRedData++;
                    }
                }
                /* correct WKC */
                ETYPE_EC_CMD_SETWKC(pMainCmdHeader, (EC_T_WORD)(wMainWkc + wRedWkc));
                break;

            case EC_CMD_TYPE_BRD:
                if (ECREG_AL_STATUS == EC_ICMDHDR_GET_ADDR_ADO(pMainCmdHeader))
                {
                    /* line crossed detection */
                    /* in case of line fixed, frames sent on Red shouldn't have been */
                    /* processed. Don't use processed flag because of Hilscher's ESC */
                    if (((pFrame->Source.b[0] & 0x08) == 0x08) && (0 != wMainWkc))
                    {
                        m_bRedLineCrossed = EC_TRUE;
                    }
                    else
                    {
                        m_bRedLineCrossed = EC_FALSE;
                    }
                    /* monitor line break changes */
                    if (m_bRedLineChanging)
                    {
                        m_bRedLineChanging = EC_FALSE;
                        m_bRedNotify       = m_bRedNotifyActive;

                        if (!m_bRedLineBreak)
                        {
                            m_bRedLineBreak = EC_TRUE;

                            /* don't wait for DL status IRQ */
                            if (m_bRedNotifyActive)
                            {
                                m_pBusTopology->SetTopologyChanged();
                            }
                        }
                        else
                        {
                            m_bRedLineBreak = EC_FALSE;
                            m_bRedFrameSplitEnabled = EC_FALSE;
                        }
                    }
                    /* determine number of slaves reachable on the main/red lines and detect crossed lines */
                    if (m_bRedLineBreak)
                    {
                        /* standard line break */
                        /* frames sent on main are received on main */
                        wNumSlavesOnMain = wMainWkc;
                        wNumSlavesOnRed  = wRedWkc;
                    }
                    else
                    {
                        
                        /* line fixed (or crossed) */
                        /* frames sent on main are received on red */
                        wNumSlavesOnMain = wRedWkc;
                        wNumSlavesOnRed  = wMainWkc;
                    }
                    /* monitor line break moves */
                    if (m_bRedLineBreak && ((m_wRedNumSlavesOnMain != wNumSlavesOnMain) || (m_wRedNumSlavesOnRed != wNumSlavesOnRed)))
                    {
                        m_bRedNotify = m_bRedNotifyActive;
                    }
                    /* refresh num slaves */
                    m_wRedNumSlavesOnMain = wNumSlavesOnMain;
                    m_wRedNumSlavesOnRed  = wNumSlavesOnRed;
#if (defined INCLUDE_MASTER_OBD)
                    RedUpdateOd();
#endif
                }
                /* merge data */
                for (nCnt=0; nCnt < wEcCmdLen; nCnt++ )
                {
                    *pbyData++ |= *pbyRedData++;
                }
                /* correct WKC */
                ETYPE_EC_CMD_SETWKC(pMainCmdHeader, (EC_T_WORD)(wMainWkc + wRedWkc));
                break;

            case EC_CMD_TYPE_FPWR:
            case EC_CMD_TYPE_BWR:
            case EC_CMD_TYPE_LWR:
            case EC_CMD_TYPE_FRMW:
                /* correct WKC */
                ETYPE_EC_CMD_SETWKC(pMainCmdHeader, (EC_T_WORD)(wMainWkc + wRedWkc));
                break;

            case EC_CMD_TYPE_LRD:
                /* merge data */
                for (nCnt=0; nCnt < wEcCmdLen; nCnt++ )
                {
                    *pbyData++ |= *pbyRedData++;
                }
                /* correct WKC */
                ETYPE_EC_CMD_SETWKC(pMainCmdHeader, (EC_T_WORD)(wMainWkc + wRedWkc));
                break;

            case EC_CMD_TYPE_LRW:
                if (wMainWkc != 0)
                {
                    /* nothing to do */
                }
                else if (wRedWkc != 0)
                {
                    for (nCnt=0; nCnt < wEcCmdLen; nCnt++ )
                    {
                        *pbyData++ = *pbyRedData++;
                    }
                }
                /* correct WKC */
                ETYPE_EC_CMD_SETWKC(pMainCmdHeader, (EC_T_WORD)(wMainWkc + wRedWkc));
                break;

            case EC_CMD_TYPE_FPRW:
                if (wMainWkc != 0)
                {
                    /* data are already stored */
                }
                else if (wRedWkc != 0)
                {
                    for (nCnt=0; nCnt < wEcCmdLen; nCnt++ )
                    {
                        *pbyData++ = *pbyRedData++;
                    }
                }
                /* correct WKC */
                ETYPE_EC_CMD_SETWKC(pMainCmdHeader, (EC_T_WORD)(wMainWkc + wRedWkc));
                break;

            case EC_CMD_TYPE_BRW:
                EC_ERRORMSGC(("CEcMaster::RedBreakFrameMerge() invalid command BRW in redundancy mode.\n"));
                OsDbgAssert(EC_FALSE);
                break;

            default:
                break;
            } /* switch( pMainCmdHeader->uCmdIdx.swCmdIdx.byCmd ) */

            /* get next sub command */
            if (EC_CMDHDRLEN_GET_NEXT(&(pMainCmdHeader->uLen)))
            {
                pMainCmdHeader = (PETYPE_EC_CMD_HEADER)((EC_T_BYTE*)pMainCmdHeader + wEcCmdLen + ETYPE_EC_OVERHEAD);
                pRedCmdHeader  = (PETYPE_EC_CMD_HEADER)((EC_T_BYTE*)pRedCmdHeader  + wEcCmdLen + ETYPE_EC_OVERHEAD);
            }
            else
            {
                pMainCmdHeader = EC_NULL;
            }
        }
    }
}

/********************************************************************************/
/** \brief Redundancy: Get Num Slaves on Main
*
* Note:
*
* \return
*/
EC_T_VOID CEcMaster::RedGetNumSlaves
(   PETHERNET_FRAME     pFrame
)
{
    PETHERNET_88A4_HEADER pMain88A4      = EC_NULL;
    PETYPE_EC_CMD_HEADER  pCmdHeader     = EC_NULL;
    EC_T_WORD             wEcCmdLen      = 0;
    EC_T_BOOL             bFound = EC_FALSE;

    if (EC_NULL != pFrame)
    {
        pMain88A4  = (PETHERNET_88A4_HEADER)EC_ENDOF(FRAMETYPE_PTR(pFrame));
        pCmdHeader = &pMain88A4->sETECAT.FirstEcCmdHeader;
    }

    while( (EC_NULL != pCmdHeader) && !bFound)
    {
        wEcCmdLen = EC_CMDHDRLEN_GET_LEN(&(pCmdHeader->uLen));

        switch (pCmdHeader->uCmdIdx.swCmdIdx.byCmd)
        {
        case EC_CMD_TYPE_BRD:
            if (ECREG_AL_STATUS == EC_ICMDHDR_GET_ADDR_ADO(pCmdHeader))
            {
                m_wRedNumSlavesOnMain = ETYPE_EC_CMD_GETWKC(pCmdHeader);
                m_wRedNumSlavesOnRed  = 0;
#if (defined INCLUDE_MASTER_OBD)
                RedUpdateOd();
#endif
                bFound = EC_TRUE;
            }
            break;
        default:
            break;
        }

        /* get next sub command */
        if (EC_CMDHDRLEN_GET_NEXT(&(pCmdHeader->uLen)))
        {
            pCmdHeader = (PETYPE_EC_CMD_HEADER)((EC_T_BYTE*)pCmdHeader + wEcCmdLen + ETYPE_EC_OVERHEAD);
        }
        else
        {
            pCmdHeader = EC_NULL;
        }
    }
}

/********************************************************************************/
/** \brief Redundancy: Update OD
*
* Note:
*
* \return
*/
#if (defined INCLUDE_MASTER_OBD)
EC_T_VOID CEcMaster::RedUpdateOd()
{
  if (EC_NULL != m_pSDOService )
  {
    m_pSDOService->UpdateRedundancy(
      (m_poEcDevice != EC_NULL && m_poEcDevice->m_bRedDeviceEnabled),
      m_wRedNumSlavesOnMain,
      m_wRedNumSlavesOnRed,
      (EC_T_BYTE)(m_bRedLineBreak?1:0));
  }
}
#endif /* INCLUDE_MASTER_OBD */
#endif /* INCLUDE_RED_DEVICE */

/*-IEcMailbox----------------------------------------------------------------*/

/********************************************************************************/
/** \brief Mailbox response to a previously sent mailbox(see SendMailboxCmd) command.
*
* \return
*/
EC_T_DWORD CEcMaster::MailboxResponse(
    PEcMailboxCmd     pCmd
   ,EC_T_BYTE*        pbyMbxData
   ,EC_T_DWORD        dwMbxDataLength
   ,EC_T_DWORD        dwMbxResult)
{
#if (defined INCLUDE_MAILBOX_STATISTICS)
    EC_T_MBXTFERP* pMbxTferPriv = (EC_T_MBXTFERP*)(pCmd->pvContext);
    if(pMbxTferPriv->dwSlaveId != MASTER_SLAVE_ID)
    {
        switch (pMbxTferPriv->MbxTfer.eMbxTferType)
        {
#if (defined INCLUDE_AOE_SUPPORT)
        case eMbxTferType_AOE_READ:             MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_AOE, EC_TRUE,  dwMbxDataLength); break;
        case eMbxTferType_AOE_WRITE:       
        case eMbxTferType_AOE_WRITECONTROL:     MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_AOE, EC_FALSE, dwMbxDataLength); break;
        case eMbxTferType_AOE_READWRITE:        MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_AOE, EC_FALSE, dwMbxDataLength);
                                                MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_AOE, EC_TRUE,  dwMbxDataLength); break;
#endif
#if (defined INCLUDE_COE_PDO_SUPPORT)
        case eMbxTferType_COE_RX_PDO:
#endif
        case eMbxTferType_COE_SDO_DOWNLOAD:     MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_COE, EC_FALSE, dwMbxDataLength); break;
        case eMbxTferType_COE_SDO_UPLOAD:
        case eMbxTferType_COE_GETODLIST:
        case eMbxTferType_COE_GETOBDESC:
        case eMbxTferType_COE_GETENTRYDESC:
        case eMbxTferType_COE_EMERGENCY:        MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_COE, EC_TRUE,  dwMbxDataLength); break;
        case eMbxTferType_FOE_SEG_UPLOAD:
        case eMbxTferType_FOE_FILE_UPLOAD:      MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_FOE, EC_TRUE,  dwMbxDataLength); break;
        case eMbxTferType_FOE_SEG_DOWNLOAD:
        case eMbxTferType_FOE_FILE_DOWNLOAD:    MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_FOE, EC_FALSE, (dwMbxResult == EC_E_BUSY) ? dwMbxDataLength : 0); break;
#if (defined INCLUDE_SOE_SUPPORT)
        case eMbxTferType_SOE_READREQUEST:
        case eMbxTferType_SOE_READRESPONSE:
        case eMbxTferType_SOE_NOTIFICATION:
        case eMbxTferType_SOE_EMERGENCY:        MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_SOE, EC_TRUE,  dwMbxDataLength); break;
        case eMbxTferType_SOE_WRITEREQUEST:
        case eMbxTferType_SOE_WRITERESPONSE:    MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_SOE, EC_FALSE, dwMbxDataLength); break;
#endif
#if (defined INCLUDE_VOE_SUPPORT)
        case eMbxTferType_VOE_MBX_READ:         MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_VOE, EC_TRUE,  dwMbxDataLength); break;
        case eMbxTferType_VOE_MBX_WRITE:        MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_VOE, EC_FALSE, dwMbxDataLength); break;
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
        case eMbxTferType_RAWMBX:               MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_RAWMBX, EC_FALSE, dwMbxDataLength);
                                                MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_RAWMBX, EC_TRUE,  dwMbxDataLength); break;
#endif
        default: break;
        }
    }
#endif /* INCLUDE_MAILBOX_STATISTICS */

    if (m_pfMbCallBack)
    {
        /* see CAtEmInterface::MboxNotify */
        return m_pfMbCallBack(m_MasterConfigEx.dwInstanceID, pCmd, pbyMbxData, dwMbxDataLength, dwMbxResult);
    }
    else
    {
        return EC_E_NOERROR;
    }
}

/********************************************************************************/
/** \brief Sends a mailbox command to the specified slave.
*
* \return
*/
EC_T_DWORD CEcMaster::SendMailboxCmd(EC_T_DWORD nSlave, PEcMailboxCmd pCmd)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes = EC_E_ERROR;

    if (nSlave < m_nCfgSlaveCount)
    {
        CEcSlave* pSlave = m_ppEcSlave[nSlave];

        if (!pSlave->HasMailBox())
        {
            dwRetVal = EC_E_NO_MBX_SUPPORT;
            goto Exit;
        }

        if (!pSlave->IsPresentOnBus())
        {
            dwRetVal = EC_E_SLAVE_NOT_PRESENT;
            goto Exit;
        }

        dwRes = ((CEcMbSlave*)pSlave)->SendMailboxCmd(pCmd);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    else if (nSlave == MASTER_SLAVE_ID)
    {
#if (defined INCLUDE_MASTER_OBD)
        dwRes = MasterLocalMailboxCmd(pCmd);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
#else
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
#endif
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

#ifdef INCLUDE_FOE_SUPPORT
EC_T_DWORD CEcMaster::FoeSegmentedUploadStart(EC_T_MBXTFERP* pMbxTferPriv, EC_T_CHAR* szDstFileName, EC_T_DWORD dwFileNameLen, EC_T_DWORD dwFileSize, EC_T_DWORD dwPassword)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes = EC_E_ERROR;

    CEcSlave* pSlave = GetCfgSlaveBySlaveId(pMbxTferPriv->dwSlaveId);
    if (EC_NULL == pSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    if (!pSlave->HasMailBox())
    {
        dwRetVal = EC_E_NO_MBX_SUPPORT;
        goto Exit;
    }
    if (!pSlave->IsPresentOnBus())
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    dwRes = ((CEcMbSlave*)pSlave)->FoeSegmentedUploadStart(pMbxTferPriv, szDstFileName, dwFileNameLen, dwFileSize, dwPassword);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_DWORD CEcMaster::FoeSegmentedUploadContinue(EC_T_MBXTFERP* pMbxTferPriv)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    CEcSlave* pSlave = GetCfgSlaveBySlaveId(pMbxTferPriv->dwSlaveId);
    if (EC_NULL == pSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    if (!pSlave->HasMailBox())
    {
        dwRetVal = EC_E_NO_MBX_SUPPORT;
        goto Exit;
    }
    if (!pSlave->IsPresentOnBus())
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    ((CEcMbSlave*)pSlave)->FoeSegmentedUploadContinue(pMbxTferPriv);
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_DWORD CEcMaster::FoeSegmentedDownloadStart(EC_T_MBXTFERP* pMbxTferPriv, EC_T_CHAR* szDstFileName, EC_T_DWORD dwFileNameLen, EC_T_DWORD dwFileSize, EC_T_DWORD dwPassword)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes = EC_E_ERROR;

    CEcSlave* pSlave = GetCfgSlaveBySlaveId(pMbxTferPriv->dwSlaveId);
    if (EC_NULL == pSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    if (!pSlave->HasMailBox())
    {
        dwRetVal = EC_E_NO_MBX_SUPPORT;
        goto Exit;
    }
    if (!pSlave->IsPresentOnBus())
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    dwRes = ((CEcMbSlave*)pSlave)->FoeSegmentedDownloadStart(pMbxTferPriv, szDstFileName, dwFileNameLen, dwFileSize, dwPassword);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_DWORD CEcMaster::FoeSegmentedDownloadContinue(EC_T_MBXTFERP* pMbxTferPriv)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    CEcSlave* pSlave = GetCfgSlaveBySlaveId(pMbxTferPriv->dwSlaveId);
    if (EC_NULL == pSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    if (!pSlave->HasMailBox())
    {
        dwRetVal = EC_E_NO_MBX_SUPPORT;
        goto Exit;
    }
    if (!pSlave->IsPresentOnBus())
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    ((CEcMbSlave*)pSlave)->FoeSegmentedDownloadContinue(pMbxTferPriv);
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}
#endif /* INCLUDE_FOE_SUPPORT */

#if (defined INCLUDE_MASTER_OBD)

/***************************************************************************************************/
/**
\brief  Work Mailbox cmd sent to Master himself.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::MasterLocalMailboxCmd(
    PEcMailboxCmd pCmd      /**< [in]   Mbx cmd to execute */
                                    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    EC_TRACEMSG(EC_TRACE_CORE, ("CEcMaster::MasterLocalMailboxCmd()\n"));

    /* don't queue like in slave objects, execute immediate ! */
    if (pCmd->cmdId == MAILBOXCMD_CANOPEN_SDO)
    {
        /* SDO Transfer */
        if (pCmd->wMbxCmdType == EC_MAILBOX_CMD_UPLOAD)
        {
            /* SDO Upload */
            if (EC_NULL == m_pSDOService)
            {
                dwRetVal = EC_E_INVALIDSTATE;
                goto Exit;
            }

            dwRetVal = m_pSDOService->SDOUpload(pCmd);
        }
        else if (pCmd->wMbxCmdType == EC_MAILBOX_CMD_DOWNLOAD)
        {
            /* SDO Download */
            if (EC_NULL == m_pSDOService)
            {
                dwRetVal = EC_E_INVALIDSTATE;
                goto Exit;
            }

            dwRetVal = m_pSDOService->SDODownload(pCmd);
        }
        else
        {
            /* ?invalid access request? */
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_ACCESSDENIED;
            goto Exit;
        }
    }
    else if ((pCmd->cmdId == MAILBOXCMD_CANOPEN_SDO_INFO_LIST) && (pCmd->wMbxCmdType == EC_MAILBOX_CMD_UPLOAD))
    {
        /* info list upload */
        if (EC_NULL == m_pSDOService)
        {
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }

        dwRetVal = m_pSDOService->GetObjDictList(pCmd);
    }
    else if ((pCmd->cmdId == MAILBOXCMD_CANOPEN_SDO_INFO_OBJ) && (pCmd->wMbxCmdType == EC_MAILBOX_CMD_UPLOAD))
    {
        /* info object upload */
        if (EC_NULL == m_pSDOService)
        {
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }

        dwRetVal = m_pSDOService->GetObjDescription(pCmd);
    }
    else if ((pCmd->cmdId == MAILBOXCMD_CANOPEN_SDO_INFO_ENTRY) && (pCmd->wMbxCmdType == EC_MAILBOX_CMD_UPLOAD))
    {
        /* info entry upload */
        if (EC_NULL == m_pSDOService)
        {
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }

        dwRetVal = m_pSDOService->GetEntryDescription(pCmd);
    }
    else if ((pCmd->cmdId >= MAILBOXCMD_ECAT_FOE_BEGIN) && (pCmd->cmdId <= MAILBOXCMD_ECAT_FOE_END))
    {
        dwRetVal = EC_E_NO_FOE_SUPPORT;
        goto Exit;
    }
    else if (pCmd->cmdId == MAILBOXCMD_ECAT_RAWMBX)
    {
        ETHERCAT_MBOX_HEADER*       pMBox = (ETHERCAT_MBOX_HEADER*)(pCmd->pvContext->MbxTfer.pbyMbxTferData);
        ETHERCAT_CANOPEN_HEADER*    pCanHdr = (ETHERCAT_CANOPEN_HEADER*)(&pCmd->pvContext->MbxTfer.pbyMbxTferData[ETHERCAT_MBOX_HEADER_LEN]);
        EC_SDO_HDR*                 pEcSdoHeader = (EC_SDO_HDR*)(&pCmd->pvContext->MbxTfer.pbyMbxTferData[ETHERCAT_MBOX_HEADER_LEN+ETHERCAT_CANOPEN_HEADER_LEN]);
        EC_T_WORD                   wOffsetData = ETHERCAT_MBOX_HEADER_LEN + ETHERCAT_CANOPEN_HEADER_LEN + EC_ECSDOHDR_OFFS_SDODATA;

        pCmd->sIndex.index = EC_ECSDOHDR_GET_INDEX(pEcSdoHeader);
        pCmd->sIndex.subIndex = pEcSdoHeader->SubIndex;

        if(pEcSdoHeader->uHdr.Iuq.Complete)
        {
            pCmd->dwMailboxFlags = EC_MAILBOX_FLAG_SDO_COMPLETE;
        }

        /* SDO Transfer */
        if (pCmd->wMbxCmdType == EC_MAILBOX_CMD_UPLOAD)
        {
            /* SDO Upload */
            if (EC_NULL == m_pSDOService)
            {
                dwRetVal = EC_E_INVALIDSTATE;
                goto Exit;
            }

            pCmd->pbyData = pCmd->pbyData + wOffsetData;    /* modify data pointer to keep headers */

            dwRetVal = m_pSDOService->SDOUpload(pCmd);      /* get data, copied to pCmd->pbyData */

            if(dwRetVal == EC_E_NOERROR)
            {
                if((pCmd->length <= EC_ECSDOHDR_OFFS_SDODATA) && (pCmd->length > 0))    /* Expedited ? */
                {
                    pEcSdoHeader->uHdr.Ius.Expedited = EC_TRUE;
                    pEcSdoHeader->uHdr.Ius.SizeInd = EC_TRUE;
                    pEcSdoHeader->uHdr.Ius.Size = 4 - pCmd->length;
                }
                else
                {
                    /* normal transfer */
                    pEcSdoHeader->uHdr.Ius.Expedited = EC_FALSE;
                    pEcSdoHeader->uHdr.Ius.SizeInd = EC_TRUE;
                    pEcSdoHeader->uHdr.Ius.Size = 0;

                    /* set length in front of data */
                    OsMemmove(pCmd->pbyData+4, pCmd->pbyData, pCmd->length);
                    EC_SETDWORD(pCmd->pbyData, pCmd->length);
                }

                pCmd->length = pCmd->length + wOffsetData;                  /* add header length */
                pCmd->length = EC_MAX(pCmd->length, 10+ETHERCAT_MBOX_HEADER_LEN);

                pCmd->pbyData = pCmd->pvContext->MbxTfer.pbyMbxTferData;    /* data pointer to mailbox header */
                pMBox->wLength = (EC_T_WORD)(pCmd->length - ETHERCAT_MBOX_HEADER_LEN);

                pEcSdoHeader->uHdr.Ius.Scs = SDO_SCS_INITIATE_UPLOAD;
            }
            else
            {
                EC_T_DWORD dwCoeError = EcConvertEcErrorToCoeError(dwRetVal);

                EC_SETDWORD(pCmd->pbyData, dwCoeError);

                pCmd->pbyData = pCmd->pvContext->MbxTfer.pbyMbxTferData;    /* data pointer to mailbox header */
                pMBox->wLength = 10;
                pEcSdoHeader->uHdr.Abt.Ccs = SDO_SCS_ABORT_TRANSFER;
            }
            
            /* set response codes */
            EC_ECCOEHDR_SET_COETYPE(pCanHdr, ETHERCAT_CANOPEN_TYPE_SDORES);
        }
    }
    else
    {
        EC_ERRORMSG(("CEcMaster::MasterLocalMailboxCmd(): unknown mailbox protocol command ID\n"));
        dwRetVal = EC_E_UNKNOWN_MBX_PROTOCOL;
        goto Exit;
    }

    /* simulate we did receive a response :) */
    MailboxResponse(pCmd, pCmd->pbyData, pCmd->length, dwRetVal);
Exit:
    return dwRetVal;
}

#endif

/***************************************************************************************************/
/**
\brief  Store ProcessData Offsets.

Used for compensation of configurator specialties.
*/
EC_T_VOID CEcMaster::SetConfiguratorOffsets(
    EC_T_DWORD  dwMinRecv       /**< [in]   Min Receive offset */
   ,EC_T_DWORD  dwMinCycOut     /**< [in]   Min Cyclic Out offset */
   ,EC_T_DWORD  dwMinSend       /**< [in]   Min Send offset */
   ,EC_T_DWORD  dwMinCycIn      /**< [in]   Min Cyclic In offset*/
                                           )
{
    /* sanity corrections */
    if (dwMinCycIn == dwMinCycOut)
    {
        dwMinRecv = dwMinSend = EC_MIN(dwMinRecv, dwMinSend);
    }

    m_dwMinRecv = dwMinRecv;
    m_dwMinCycOutOffset = dwMinCycOut;
    m_dwMinSend = dwMinSend;
    m_dwMinCycInOffset = dwMinCycIn;
}
/*-IEcMaster-----------------------------------------------------------------*/


/********************************************************************************/
/** \brief Start EtherCAT master Scan Bus.
*
* Note: this function returns immediately.
*
* \return EC_E_NOERROR if Scan Bus request was queued successfully.
*/
EC_T_DWORD CEcMaster::StartBTBusScan(EC_T_DWORD dwTimeout)
{
    m_dwScanBusResult = EC_E_BUSY;

    return m_pBusTopology->StartBusScan(dwTimeout);
}

/********************************************************************************/
/** \brief Start BT state machine for topology change
*
* Note: this function returns immediately.
*
* \return EC_E_NOERROR if Scan Bus request was queued successfully.
*/
EC_T_DWORD CEcMaster::StartBTTopologyChange(EC_T_VOID)
{
    m_dwScanBusResult = EC_E_BUSY;

    OsDbgAssert(EC_NULL != m_pBusTopology);

    return m_pBusTopology->StartTopologyChange(m_dwSBTimeout);
}

/********************************************************************************/
/** \brief Start BT state machine to accept topology change (for manual mode)
*
* Note: this function returns immediately.
*
* \return EC_E_NOERROR if Scan Bus request was queued successfully.
*/
EC_T_DWORD CEcMaster::StartBTAcceptTopologyChange(EC_T_VOID)
{
    m_dwScanBusResult = EC_E_BUSY;

    return m_pBusTopology->StartAcceptTopologyChange(m_dwSBTimeout);
}

/********************************************************************************/
/** \brief Start BT state machine for DL status interrupt
*
* Note: this function returns immediately.
*
* \return EC_E_NOERROR if Scan Bus request was queued successfully.
*/
EC_T_DWORD CEcMaster::StartBTDlStatusInt(EC_T_VOID)
{
    return m_pBusTopology->StartDlStatusInt(m_dwSBTimeout);
}

/********************************************************************************/
/** \brief Start BT state machine for AL status refresh
*
* Note: this function returns immediately.
*
* \return EC_E_NOERROR if Scan Bus request was queued successfully.
*/
EC_T_DWORD CEcMaster::StartBTAlStatusRefresh(EC_T_VOID)
{
    return m_pBusTopology->StartAlStatusRefresh(m_dwSBTimeout);
}

/********************************************************************************/
/** \brief Wait for BT state machine
*
* Note: this function returns immediately.
*
* \return EC_E_NOERROR if Scan Bus request was queued successfully.
*/
EC_T_DWORD CEcMaster::WaitForBTStateMachine(EC_T_VOID)
{
EC_T_DWORD dwRes = m_pBusTopology->WaitForBTStateMachine();

    if ((EC_E_BUSY == dwRes) || (EC_E_NOTREADY == dwRes))
    {
        dwRes = EC_E_BUSY;
    }
    else
    {
        if ((EC_E_BUSY == m_dwScanBusResult) || (EC_E_NOERROR != dwRes))
        {
            m_dwScanBusResult = dwRes;
        }
    }
    return dwRes;
}

/********************************************************************************/
/** \brief Release BT state machine
*
* Note: this function returns immediately.
*
* \return EC_E_NOERROR if Scan Bus request was queued successfully.
*/
EC_T_DWORD CEcMaster::ReleaseBTStateMachine(EC_T_DWORD dwResult)
{
EC_T_DWORD dwRes = m_pBusTopology->ReleaseBTStateMachine();

    if (EC_E_BUSY == dwRes)
    {
        dwRes = dwResult;
    }
    if ((EC_E_BUSY == m_dwScanBusResult) || (EC_E_NOERROR != dwRes))
    {
        m_dwScanBusResult = dwRes;
    }

    return dwRes;
}

/***************************************************************************************************/
/**
\brief  Create configured topology information
*/
EC_T_VOID CEcMaster::SBCreateCfgTopology(EC_T_VOID)
{
    m_pBusTopology->CreateCfgTopology();
    return;
}

/***************************************************************************************************/
/**
\brief  Delete configured topology information
*/
EC_T_VOID CEcMaster::SBDeleteCfgTopology(EC_T_VOID)
{
    m_pBusTopology->DeleteCfgTopology();
    return;
}


/***************************************************************************************************/
/**
\brief  Get Amount of Free Ranges within BT.

\return Amount of ranges.
*/
EC_T_DWORD CEcMaster::SBFreeRanges(EC_T_VOID)
{
    return ((EC_NULL == m_pBusTopology)?0:m_pBusTopology->FreeRanges());
}

/***************************************************************************************************/
/**
\brief  Get Amount of Free Ranges within BT.

\return Amount of ranges.
*/
EC_T_VOID CEcMaster::SBSetFreeRange(
    EC_T_DWORD  dwIdx   /**< [in]   Range index */
   ,EC_T_WORD   wBegin  /**< [in]   Range begin */
   ,EC_T_WORD   wEnd    /**< [in]   Range end */
                                    )
{
    m_pBusTopology->FreeRangeSet(dwIdx, wBegin, wEnd);
}

/***************************************************************************************************/
/**
\brief  Set ScanBus Verification Enable.
*/
EC_T_VOID CEcMaster::SetSBVerificationEnable(
                                             EC_T_BOOL bEnable  /**< [in]   Enable ScanBus if true */
                                            )
{
    m_pBusTopology->SetVerificationActive(bEnable);
}

/***************************************************************************************************/
/**
\brief  Get Slave Information by Slave Index.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::SbGetSlaveInfo(
                                     EC_T_DWORD dwAutoIncAddr,
                                     EC_T_PVOID pvInfo
                                    )
{
EC_T_DWORD   dwRetVal  = EC_E_ERROR;
EC_T_DWORD   dwRes     = EC_E_ERROR;
CEcBusSlave* pBusSlave = EC_NULL;
EC_PT_SB_SLAVEINFO_DESC pInfo = (EC_PT_SB_SLAVEINFO_DESC)pvInfo;

    /* lock the bus slave list */
    m_pBusTopology->InterLockList(EC_TRUE);

    /* bus scan shall be complete */
    dwRes = GetBusScanResult();
    if ((EC_E_BUSY == dwRes) || (EC_E_NOTREADY == dwRes))
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    /* get bus slave */
    pBusSlave = m_pBusTopology->GetBusSlaveByAutoIncAddress(EC_LOWORD(dwAutoIncAddr));
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* handle request */
    pBusSlave->GetSlaveInfo(pInfo);

    /* no error */
    dwRetVal = EC_E_NOERROR;

Exit:
    /* release bus slave list */
    m_pBusTopology->InterLockList(EC_FALSE);

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get Information about Slave EEPROM Contents.

\return EC_E_NOERROR on success, error code otherwise .
*/
EC_T_DWORD CEcMaster::SbGetSlaveInfoEep(
                                        EC_T_SB_SLAVEINFO_EEP_REQ_DESC* pReq,   /**< [in]   request parameters */
                                        EC_T_SB_SLAVEINFO_EEP_RES_DESC* pRes    /**< [out]  response */
                                       )
{
EC_T_DWORD   dwRetVal  = EC_E_ERROR;
EC_T_DWORD   dwRes     = EC_E_ERROR;
CEcBusSlave* pBusSlave = EC_NULL;
CEcSlave*    pCfgSlave = EC_NULL;

    /* lock the bus slave list */
    m_pBusTopology->InterLockList(EC_TRUE);

    /* bus scan shall be complete */
    dwRes = GetBusScanResult();
    if ((EC_E_BUSY == dwRes) || (EC_E_NOTREADY == dwRes))
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    /* get bus slave */
    pBusSlave = m_pBusTopology->GetBusSlaveByAutoIncAddress(EC_LOWORD(pReq->wAutoIncAddress));
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* handle request */
    switch (pReq->eSbSlaveInfoType)
    {
    case sbsit_bustopology:
        {
            switch (pReq->eEEPROMEntry)
            {
            case eEEP_VendorId:       pRes->dwEEPROMValue = pBusSlave->GetVendorId();     break;
            case eEEP_ProductCode:    pRes->dwEEPROMValue = pBusSlave->GetProductCode();  break;
            case eEEP_RevisionNumber: pRes->dwEEPROMValue = pBusSlave->GetRevisionNo();   break;
            case eEEP_SerialNumber:   pRes->dwEEPROMValue = pBusSlave->GetSerialNo();     break;
            case eEEP_BootRcvMbx:     pRes->dwEEPROMValue = pBusSlave->GetBootRcvMbx();   break;
            case eEEP_BootSndMbx:     pRes->dwEEPROMValue = pBusSlave->GetBootSndMbx();   break;
            case eEEP_StdRcvMbx:      pRes->dwEEPROMValue = pBusSlave->GetStdRcvMbx();    break;
            case eEEP_StdSndMbx:      pRes->dwEEPROMValue = pBusSlave->GetStdSndMbx();    break;
            case eEEP_MbxProtocol:    pRes->dwEEPROMValue = pBusSlave->GetMbxProtocols(); break;
            case eEEP_AliasAddress:   pRes->dwEEPROMValue = pBusSlave->GetAliasAddress(); break;
            case eEEP_BCppDummy:
            default:
                break;
            }
            pRes->eEEPROMEntry    = pReq->eEEPROMEntry;
            pRes->dwScanBusStatus = pBusSlave->GetScanBusStatus();

        }break;
    case sbsit_configuration:
        {
        EC_T_DWORD dwEEPROMValue = 0;

            /* get config slave */
            pCfgSlave = pBusSlave->m_pCfgSlave;
            if (EC_NULL == pCfgSlave)
            {
                dwRetVal = EC_E_NOTFOUND;
                goto Exit;
            }
            dwRes = pCfgSlave->GetConfigValue((EC_T_WORD)pReq->eEEPROMEntry, dwEEPROMValue);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            pRes->dwEEPROMValue = dwEEPROMValue;
        }break;
    case sbsit_unknown:
    default:
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        } /* no break */
    }
    /* no error */
    dwRetVal = EC_E_NOERROR;

Exit:
    /* release bus slave list */
    m_pBusTopology->InterLockList(EC_FALSE);

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get Extended Information about Slave.

\return EC_E_NOERROR on success, error code otherwise .
*/
EC_T_DWORD CEcMaster::SbGetSlaveInfoEx(
                                        EC_T_SB_SLAVEINFO_REQ_DESC* pReq,   /**< [in]   request parameters */
                                        EC_T_SB_SLAVEINFO_RES_DESC* pRes    /**< [out]  response */
                                       )
{
EC_T_DWORD   dwRetVal  = EC_E_ERROR;
EC_T_DWORD   dwRes     = EC_E_ERROR;
CEcBusSlave* pBusSlave = EC_NULL;
CEcSlave*    pCfgSlave = EC_NULL;

    /* lock the bus slave list */
    m_pBusTopology->InterLockList(EC_TRUE);

    /* bus scan shall be complete */
    dwRes = GetBusScanResult();
    if ((EC_E_BUSY == dwRes) || (EC_E_NOTREADY == dwRes))
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    /* get bus slave */
    pBusSlave = m_pBusTopology->GetBusSlaveByAutoIncAddress(EC_LOWORD(pReq->wAutoIncAddress));
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* handle request */
    if ((eie_portstate     == pReq->eInfoEntry)
    ||  (eie_dcsupport     == pReq->eInfoEntry)
    ||  (eie_dc64support   == pReq->eInfoEntry)
    ||  (eie_alias_address == pReq->eInfoEntry)
    ||  (eie_phys_address  == pReq->eInfoEntry)
    ||  (eie_esctype       == pReq->eInfoEntry)
      )
    {
        switch( pReq->eInfoEntry )
        {
        case eie_portstate:
            {
            EC_T_WORD wPortState = 0;

                if (sizeof(EC_T_WORD) > pRes->dwInfoLength)
                {
                    dwRetVal = EC_E_INVALIDSIZE;
                    goto Exit;
                }
                wPortState = pBusSlave->GetPortState();

                pRes->eInfoEntry   = pReq->eInfoEntry;
                pRes->dwInfoLength = sizeof(EC_T_WORD);
                EC_SETWORD((pRes->pbyInfo), wPortState);
            } break;
        case eie_dcsupport:
            {
                if (sizeof(EC_T_BOOL) > pRes->dwInfoLength)
                {
                    dwRetVal = EC_E_INVALIDSIZE;
                    goto Exit;
                }
                pRes->eInfoEntry   = pReq->eInfoEntry;
                pRes->dwInfoLength = sizeof(EC_T_BOOL);
                EC_SETDWORD((pRes->pbyInfo), pBusSlave->IsDCSupport());
            } break;
        case eie_dc64support:
            {
                if (sizeof(EC_T_BOOL) > pRes->dwInfoLength)
                {
                    dwRetVal = EC_E_INVALIDSIZE;
                    goto Exit;
                }
                pRes->eInfoEntry   = pReq->eInfoEntry;
                pRes->dwInfoLength = sizeof(EC_T_BOOL);
                EC_SETDWORD((pRes->pbyInfo), pBusSlave->IsDC64Support());
            } break;
        case eie_alias_address:
            {
                if (sizeof(EC_T_WORD) > pRes->dwInfoLength)
                {
                    dwRetVal = EC_E_INVALIDSIZE;
                    goto Exit;
                }
                pRes->eInfoEntry   = pReq->eInfoEntry;
                pRes->dwInfoLength = sizeof(EC_T_WORD);
                EC_SETWORD((pRes->pbyInfo), pBusSlave->GetAliasAddress());
            } break;
        case eie_phys_address:
            {
                if (sizeof(EC_T_WORD) > pRes->dwInfoLength)
                {
                    dwRetVal = EC_E_INVALIDSIZE;
                    goto Exit;
                }
                pRes->eInfoEntry   = pReq->eInfoEntry;
                pRes->dwInfoLength = sizeof(EC_T_WORD);
                EC_SETWORD((pRes->pbyInfo), pBusSlave->GetFixedAddress());
            } break;
        case eie_esctype:
            {
                if (sizeof(EC_T_BYTE) > pRes->dwInfoLength)
                {
                    dwRetVal = EC_E_INVALIDSIZE;
                    goto Exit;
                }
                pRes->eInfoEntry   = pReq->eInfoEntry;
                pRes->dwInfoLength = sizeof(EC_T_BYTE);
                (*(pRes->pbyInfo)) = pBusSlave->GetESCType();
            } break;
        default:
            {
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
        }
    }
    else
    {
        /* get config slave */
        pCfgSlave = pBusSlave->m_pCfgSlave;
        if (EC_NULL == pCfgSlave)
        {
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }
        pRes->eInfoEntry   = pReq->eInfoEntry;
        pRes->dwInfoLength = EC_MAX(pRes->dwInfoLength, sizeof(EC_T_DWORD));
        dwRes = pCfgSlave->GetSlaveInfo(pRes->eInfoEntry, pRes->pbyInfo, (EC_T_DWORD*)&(pRes->dwInfoLength));
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    /* no error */
    dwRetVal = EC_E_NOERROR;

Exit:
    /* release bus slave list */
    m_pBusTopology->InterLockList(EC_FALSE);

    return dwRetVal;
}

#if (defined INCLUDE_DC_SUPPORT)
/***************************************************************************************************/
/**
\brief  Enable DC Support.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::SetDcSupportEnabled(
    EC_T_BOOL   bEnable,            /**< [in]   DC Support on / off [EC_TRUE/EC_FALSE]*/
    EC_T_DWORD  dwTimeout           /**< [in]   Timeout value in msec if not set default
                                                value is used*/
                                                 )
{
    EC_UNREFPARM(dwTimeout);
    m_bDCSupportEnabled = bEnable;
#if (defined INCLUDE_MASTER_OBD)
    if (EC_NULL != m_pSDOService)
    {
        m_pSDOService->EntrySet_SetDCEnabled(bEnable);
    }
#endif
    return EC_E_NOERROR;
}
#endif /* INCLUDE_DC_SUPPORT */

/***************************************************************************************************/
/**
\brief  Enable slave alias addressing.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::EnableAliasAddressing(
    EC_T_BOOL bEnable
                                           )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    m_pBusTopology->EnableAliasAddressing(bEnable);

    dwRetVal = EC_E_NOERROR;
    return dwRetVal;
}

/********************************************************************************/
/** \brief Gets a pointer to the EtherCAT slave with the specified address.
*
* \return Slave interface
*/
CEcSlave* CEcMaster::GetSlaveByAddr(EC_T_WORD addr)
{
CEcSlave* pSlave = EC_NULL;
EC_T_DWORD  nPos = 0;

    for (nPos = 0; nPos < m_nCfgSlaveCount; nPos++)
    {
        pSlave = m_ppEcSlave[nPos];
        if (pSlave->m_wFixedAddr == addr)
        {
            return pSlave;
        }
    }
    return EC_NULL;
}

/********************************************************************************/
/** \brief Gets a pointer to the EtherCAT slave with the specified configured address.
*
* \return Slave interface
*/
CEcSlave* CEcMaster::GetSlaveByCfgFixedAddr(EC_T_WORD addr)
{
CEcSlave* pSlave = EC_NULL;
EC_T_DWORD  nPos = 0;

    for (nPos = 0; nPos < m_nCfgSlaveCount; nPos++)
    {
        pSlave = m_ppEcSlave[nPos];
        if (pSlave->GetCfgFixedAddr() == addr)
        {
            return pSlave;
        }
    }
    return EC_NULL;
}

/***************************************************************************************************/
/**
\brief  Find slave by AutoInc Addr.

  (Only Slaves which are connected to Bus have got valid AINC Addresses)

\return Slave interface.
*/
CEcSlave* CEcMaster::GetSlaveByAutoIncAddr(
    EC_T_WORD   wAutoIncAddr           /**< [in]   Auto Increment Address */
                                       )
{
    CEcSlave*   pRetVal     = EC_NULL;
    EC_T_DWORD  dwSlaveCnt  = GetCfgSlaveCount();
    EC_T_DWORD  dwSlaveIdx  = 0;
    CEcSlave*   pCfgSlave   = EC_NULL;

    for (dwSlaveIdx = 0; dwSlaveIdx < dwSlaveCnt; dwSlaveIdx++)
    {
        pCfgSlave = GetSlaveByIndex(dwSlaveIdx);
        if (EC_NULL == pCfgSlave)
        {
            continue;
        }
        if ((pCfgSlave->m_wAutoIncAddr == wAutoIncAddr) && pCfgSlave->IsPresentOnBus())
        {
            pRetVal = pCfgSlave;
            break;
        }
    }
    return pRetVal;
}

/***************************************************************************************************/
/**
\brief  Find slave by configured AutoInc Addr.

\return Pointer to matching Cfgslave
*/
CEcSlave* CEcMaster::GetSlaveByCfgAutoIncAddr(
    EC_T_WORD   wCfgAutoIncAddr           /**< [in]   Auto Increment Address */
                                       )
{
    CEcSlave*   pRetVal     = EC_NULL;
    EC_T_DWORD  dwSlaveCnt  = GetCfgSlaveCount();
    EC_T_DWORD  dwSlaveIdx  = 0;
    CEcSlave*   pCfgSlave   = EC_NULL;

    for (dwSlaveIdx = 0; dwSlaveIdx < dwSlaveCnt; dwSlaveIdx++)
    {
        pCfgSlave = GetSlaveByIndex(dwSlaveIdx);
        if (EC_NULL == pCfgSlave)
        {
            continue;
        }
        if (pCfgSlave->GetCfgAutoIncAddress() == wCfgAutoIncAddr)
        {
            pRetVal = pCfgSlave;
            break;
        }
    }
    return pRetVal;
}

/***************************************************************************************************/
/**
\brief  Find bus slave by slave ID

\return Pointer to matching BusSlave
*/
CEcBusSlave* CEcMaster::GetBusSlaveBySlaveId(EC_T_DWORD dwSlaveId)
{
CEcBusSlave* pBusSlaveRetVal = EC_NULL;
    
    /* slave ids above cfg slave count are used to address unexpected bus slaves */
    if (dwSlaveId >= m_nCfgSlaveCount)
    {
        pBusSlaveRetVal = m_pBusTopology->GetBusSlaveByIndex(dwSlaveId - m_nCfgSlaveCount);
        /* only return a unexpected bus slave */
        if ((EC_NULL != pBusSlaveRetVal) && (EC_NULL != pBusSlaveRetVal->GetCfgSlave()))
        {
            pBusSlaveRetVal = EC_NULL;
        }
    }
    else
    {
    CEcSlave* pCfgSlave = GetCfgSlaveBySlaveId(dwSlaveId);

        if (EC_NULL != pCfgSlave)
        {
            pBusSlaveRetVal = pCfgSlave->m_pBusSlave;
        }
    }
    return pBusSlaveRetVal;
}

/***************************************************************************************************/
/**
\brief  Return matching slave ID

        If master is configured, return the index of matching config slave or a "virtual" slave ID
        of mismatching bus slave (based on its index)
        If master is not configured return bus slave index.

\return Slave ID
*/
/* if master is configured, return a "virtual" slave ID outside the config slave index domain, return bus slave index */

EC_T_DWORD CEcMaster::GetSlaveIdByFixedAddress(EC_T_WORD wFixedAddress)
{
    if (0 == wFixedAddress)
    {
        return MASTER_SLAVE_ID;
    }
    {
    CEcSlave* pCfgSlave = GetSlaveByCfgFixedAddr(wFixedAddress);

        if (EC_NULL != pCfgSlave)
        {
            return pCfgSlave->GetSlaveId();
        }
    }
    {
    CEcBusSlave* pBusSlave = m_pBusTopology->GetBusSlaveByFixedAddress(wFixedAddress);

        if (EC_NULL != pBusSlave)
        {
            return pBusSlave->GetBusIndex() + GetCfgSlaveCount();
        }
    }
    return INVALID_SLAVE_ID;
}

/********************************************************************************/
/** \brief Creates an EtherCAT Slave.
*
* \return Slave object
*/
CEcSlave* CEcMaster::CreateSlave(EC_T_ENI_SLAVE* pEniSlave)
{
    CEcSlave*   pSlave = EC_NULL;
    CEcMbSlave* pMbSlave = EC_NULL;

    if (pEniSlave->sParm.bMailbox)
    {
        pMbSlave = EC_NEW(CEcMbSlave(this, pEniSlave, m_poEcDevice, m_dwMsecCounter));
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::CreateSlave", pMbSlave, sizeof(CEcMbSlave));
        pSlave = pMbSlave;
    }
    else
    {
        pSlave = EC_NEW(CEcSlave(this, pEniSlave, m_poEcDevice));
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::CreateSlave", pSlave, sizeof(CEcSlave));
    }
    if (EC_NULL == pSlave)
    {
        goto Exit;
    }

Exit:
    return pSlave;
}

EC_T_DWORD CEcMaster::AddSlaveToList(CEcSlave* pSlave)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (m_pMasterDesc->wMaxNumSlaves <= m_nCfgSlaveCount)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    pSlave->SetSlaveId(m_nCfgSlaveCount);

#if (defined INCLUDE_MASTER_OBD)
    /* Add configured slave to Master OD */
    SDOAddConfiguredSlave(m_nCfgSlaveCount, pSlave);
#endif

    m_ppEcSlave[m_nCfgSlaveCount++] = pSlave;
    if (pSlave->HasMailBox())
    {
        m_ppEcMbSlave[m_nEcMbSlave++] = (CEcMbSlave*)pSlave;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_VOID CEcMaster::RemoveSlaveFromList(CEcSlave* pSlave)
{
    EC_T_DWORD dwIdx = 0;
    EC_T_BOOL bSlaveFound = EC_FALSE;
    EC_T_BOOL bMbSlaveFound = EC_FALSE;

    /* remove slave from list and refresh slave id */
    for (dwIdx = 0; dwIdx < m_nCfgSlaveCount; dwIdx++)
    {
        if ((m_ppEcSlave[dwIdx] == pSlave) || bSlaveFound)
        {
            bSlaveFound = EC_TRUE;
            if (dwIdx < m_nCfgSlaveCount - 1)
            {
                m_ppEcSlave[dwIdx] = m_ppEcSlave[dwIdx + 1];
                m_ppEcSlave[dwIdx]->SetSlaveId(dwIdx);
            }
            else
            {
                m_ppEcSlave[dwIdx] = EC_NULL;
            }
        }
        if ((m_ppEcMbSlave[dwIdx] == pSlave) || bMbSlaveFound)
        {
            bMbSlaveFound = EC_TRUE;
            if (dwIdx < m_nCfgSlaveCount - 1)
            {
                m_ppEcMbSlave[dwIdx] = m_ppEcMbSlave[dwIdx + 1];
            }
            else
            {
                m_ppEcMbSlave[dwIdx] = EC_NULL;
            }
        }
    }
    /* decrease counters */
    if (bSlaveFound)
        m_nCfgSlaveCount--;
    if (bMbSlaveFound)
        m_nEcMbSlave--;
}
/********************************************************************************/
/** \brief Creates the EtherCAT commands that should be sent cyclically based on XML configuration
*
* \return
*/
EC_T_DWORD CEcMaster::CreateCyclicCmds
    (EC_T_INT        nNumCycEntries     /* [in]  total number of cyclic entries in the XML file */
    ,EC_T_INT        nCycEntryIndex     /* [in]  index of current cyclic entry */
    ,EcCycCmdConfig* pCycCmdConfig      /* [in]  cyclic command configuration data */
#if (defined INCLUDE_WKCSTATE)
    ,EC_T_WORD*      pwWkcStateDiagOffs /* [in/out] current bit offeset in diagnosis image for WkcState */
#endif
)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_DWORD dwRes = EC_E_ERROR;
EcCycFrameDesc* pCurrEcCycFrameDesc = EC_NULL;
EcCycCmdConfigDesc* pCmdDesc;
EC_T_WORD wCmdsLen = 0;
EC_T_WORD wHeaderLen;
EC_T_WORD wFrameIdx = 0;
CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc = EC_NULL;

    wHeaderLen = ETHERNET_88A4_FRAME_LEN;
#if (defined VLAN_FRAME_SUPPORT)
    if (m_bVLANEnabled)
    {
        wHeaderLen += ETYPE_VLAN_HEADER_LEN;
    }
#endif

    /* initialize management for all cyclic entries, execute on entry 0 */
    if (nCycEntryIndex == 0 )
    {
        OsDbgAssert(m_aCyclicEntryMgmtDesc==EC_NULL);
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
        m_dwNumCycConfigEntries = (EC_T_DWORD)nNumCycEntries;
#endif
        m_aCyclicEntryMgmtDesc = EC_NEW(CYCLIC_ENTRY_MGMT_DESC[nNumCycEntries]);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::m_aCyclicEntryMgmtDesc", m_aCyclicEntryMgmtDesc, nNumCycEntries * sizeof(CYCLIC_ENTRY_MGMT_DESC));
        if (m_aCyclicEntryMgmtDesc == EC_NULL )
        {
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        OsMemset(m_aCyclicEntryMgmtDesc,0,nNumCycEntries*sizeof(CYCLIC_ENTRY_MGMT_DESC));

        m_wAllCycEntriesNumFrames  = 0;

        /* initialize AlStatusRead flag array */
        m_bFoundAlStatusRead = EC_FALSE;

#if (defined EVAL_VERSION)
        if (m_pLkMemory != EC_NULL)
        {
            for (EC_T_INT nOffset = 0; nOffset < 512; nOffset += 8)
            {
                OsMemcpy(&m_pLkMemory[nOffset], &m_qwLk, 8);
            }
        }
#endif
    }
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    OsDbgAssert(m_dwNumCycConfigEntries>0);
#endif
    OsDbgAssert(m_aCyclicEntryMgmtDesc!=EC_NULL);

    pCyclicEntryMgmtDesc = &m_aCyclicEntryMgmtDesc[nCycEntryIndex];
    pCyclicEntryMgmtDesc->dwTaskId      = pCycCmdConfig->dwTaskId;
    pCyclicEntryMgmtDesc->dwPriority    = pCycCmdConfig->dwPriority;
    pCyclicEntryMgmtDesc->dwCycleTime   = pCycCmdConfig->dwCycleTime;
    pCyclicEntryMgmtDesc->wTotalNumCmds = 0; /* total number of commands will be determined later */
    pCyclicEntryMgmtDesc->byFrameIdxFirst = 0xff;
    pCyclicEntryMgmtDesc->byFrameIdxLast  = 0xff;

    /* alloc frame desc managment memory */
    pCyclicEntryMgmtDesc->apEcCycFrameDesc    = EC_NEW(EcCycFrameDesc*[pCycCmdConfig->wNumFrames]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::CreateCyclicCmds", pCyclicEntryMgmtDesc->apEcCycFrameDesc, pCycCmdConfig->wNumFrames * sizeof(EcCycFrameDesc*));
    OsMemset(pCyclicEntryMgmtDesc->apEcCycFrameDesc, 0, pCycCmdConfig->wNumFrames*sizeof(EcCycFrameDesc*));

    /* number of required frames will be recalculated later */
    pCyclicEntryMgmtDesc->wNumFrames = 0;

#if (defined INCLUDE_VARREAD)
    /* swap information */
    if (0 != pCycCmdConfig->wNumOfSwapInfos)
    {
        if (m_eCycFrameLayout == eCycFrameLayout_IN_DMA)
        {
            /* if IN_DMA the process data should stay swapped until MAC completely sent the frame (race condition is currently not handled) */
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_NOTSUPPORTED;
            goto Exit;
        }
        pCyclicEntryMgmtDesc->wNumOfSwapInfos = pCycCmdConfig->wNumOfSwapInfos;
        OsDbgAssert(EC_NULL == pCyclicEntryMgmtDesc->pSwapInfo);
        pCyclicEntryMgmtDesc->pSwapInfo = pCycCmdConfig->pSwapInfo;
    }
#endif /* INCLUDE_VARREAD */

    /* Copy information for Slave-to-Slave communication */
    if (0 != pCycCmdConfig->wNumOfCopyInfos)
    {
        pCyclicEntryMgmtDesc->wNumOfCopyInfos = pCycCmdConfig->wNumOfCopyInfos;
        OsDbgAssert(EC_NULL == pCyclicEntryMgmtDesc->pCopyInfo);
        pCyclicEntryMgmtDesc->pCopyInfo = pCycCmdConfig->pCopyInfo;
    }
    /* optimize copy information */
    if (0 != pCyclicEntryMgmtDesc->wNumOfCopyInfos)
    {
        S2SOptimizeCopyInfos(pCyclicEntryMgmtDesc);
    }

    /* allocate cyclic command config desc table */
    pCyclicEntryMgmtDesc->apEcAllCycCmdConfigDesc = EC_NEW(EcCycCmdConfigDesc*[pCycCmdConfig->wTotalNumCmds]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::apEcAllCycCmdConfigDesc", pCyclicEntryMgmtDesc->apEcAllCycCmdConfigDesc, 
        pCycCmdConfig->wTotalNumCmds * sizeof(EcCycCmdConfigDesc*));

    /* process all commands and insert frames in managment description */
    for (wFrameIdx = 0; wFrameIdx < pCycCmdConfig->wNumFrames; wFrameIdx++)
    {
        EC_T_WORD wCmdIdx = 0;
        pCurrEcCycFrameDesc = pCycCmdConfig->apCycFrameDesc[wFrameIdx];

        for (wCmdIdx = 0; wCmdIdx < pCurrEcCycFrameDesc->wCmdCnt; wCmdIdx++)
        {
            pCmdDesc = pCurrEcCycFrameDesc->apCycCmd[wCmdIdx];

            if ((ETHERNET_MAX_FRAME_LEN - wHeaderLen) < ETYPE_EC_CMD_GETLEN(&pCmdDesc->EcCmdHeader))
            {
                /* single command is larger than a frame => Fatal error */
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }

            pCmdDesc->bActive = EC_FALSE;
            pCmdDesc->dwInOffset += m_dwCopyOffset;
            pCmdDesc->dwOutOffset += m_dwCopyOffset;
            pCmdDesc->wDataLen = EC_ICMDHDR_GET_LEN_LEN(&(pCmdDesc->EcCmdHeader));
            
            if (0 == m_oMemProvider.dwPDInDataLength)
            {
                pCmdDesc->bCopyInputs = EC_FALSE;
            }
            if (0 == m_oMemProvider.dwPDOutDataLength)
            {
                pCmdDesc->bCopyOutputs = EC_FALSE;
            }
            pCmdDesc->wEcCmdWkcOffset = (EC_T_WORD)ETYPE_EC_CMD_GETWKCOFF(&pCmdDesc->EcCmdHeader);
            pCmdDesc->bThisTimeStamped = EC_FALSE;

            /* copy only proc. data () into input image ? */
            if (m_bOnlyProcDataInImage)
            {
                switch (pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                {
                case EC_CMD_TYPE_LRD:
                case EC_CMD_TYPE_LWR:
                case EC_CMD_TYPE_LRW:
                    if (!pCmdDesc->bIsLogicalMBoxStateCmd) break;
                    /* no break */
                default:
                    pCmdDesc->bCopyInputs = EC_FALSE;
                    pCmdDesc->bCopyOutputs = EC_FALSE;
                    break;
                }
            }
            /* check if this command is the broadcast AL status read */
            if ((EC_CMD_TYPE_BRD == pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                && (ECREG_AL_STATUS == EC_ICMDHDR_GET_ADDR_ADO(&(pCmdDesc->EcCmdHeader))))
            {
                pCmdDesc->bReadAlStatus = EC_TRUE;
                m_bFoundAlStatusRead = EC_TRUE;
            }
            else
            {
                pCmdDesc->bReadAlStatus = EC_FALSE;
            }
            /* datagram to shift the reference clock ? */
            if ((EC_CMD_TYPE_APWR == pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                && (ECM_DCS_SYSTEMTIME == EC_ICMDHDR_GET_ADDR_ADO(&(pCmdDesc->EcCmdHeader))))
            {
                pCmdDesc->bDcShiftSystemTime = EC_TRUE;
            }
            else
            {
                pCmdDesc->bDcShiftSystemTime = EC_FALSE;
            }
            /* check if this command is the ARMW on System Time */
            if ((EC_CMD_TYPE_ARMW == pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                && (ECREG_SYSTEMTIME == EC_ICMDHDR_GET_ADDR_ADO(&(pCmdDesc->EcCmdHeader))))
            {
                pCmdDesc->bDcDistributeSystemTime = EC_TRUE;
            }
            else
            {
                pCmdDesc->bDcDistributeSystemTime = EC_FALSE;
            }
            /* check if this command is the broadcast System Time difference Read */
            if ((EC_CMD_TYPE_BRD == pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                && (ECREG_SYSTIME_DIFF == EC_ICMDHDR_GET_ADDR_ADO(&(pCmdDesc->EcCmdHeader))))
            {
#if (defined INCLUDE_DC_SUPPORT)
                m_pDistributedClks->SetSyncWindowMonitoringConfigured();
#endif
                pCmdDesc->bDcReadSystemTimeDiff = EC_TRUE;
            }
            else
            {
                pCmdDesc->bDcReadSystemTimeDiff = EC_FALSE;
            }
            /* datagram accessing Receive Time Port 0, to latch port receive times */
            if ((EC_CMD_TYPE_NOP == pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                && (ECM_DCS_REC_TIMEPORT0 == EC_ICMDHDR_GET_ADDR_ADO(&(pCmdDesc->EcCmdHeader))))
            {
                pCmdDesc->bDcLatchReceiveTimes = EC_TRUE;
            }
            else
            {
                pCmdDesc->bDcLatchReceiveTimes = EC_FALSE;
            }
#if (defined INCLUDE_RED_DEVICE)
            /* check if we have invalid commands in redundant mode */
            if (m_poEcDevice->m_bRedConfigured)
            {
                if (EC_CMD_TYPE_BRW == pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                {
                    OsDbgAssert(EC_FALSE);
                    dwRetVal = EC_E_XML_INVALID_CMD_WITH_RED;
                    goto Exit;
                }
                else if ((EC_CMD_TYPE_LRW == pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd) && (pCmdDesc->wExpectedWkc > 3))
                {
                    OsDbgAssert(EC_FALSE);
                    dwRetVal = EC_E_XML_INVALID_CMD_WITH_RED;
                    goto Exit;
                }
            }
#endif
#if (defined INCLUDE_WKCSTATE)
            if (!pCmdDesc->bIsLogicalMBoxStateCmd)
            {
                pCmdDesc->wWkcStateDiagOffs = *pwWkcStateDiagOffs;
                (*pwWkcStateDiagOffs)++;
            }
#endif
            /* add cyclic command entry to apEcAllCycCmdConfigDesc */
            pCyclicEntryMgmtDesc->apEcAllCycCmdConfigDesc[pCyclicEntryMgmtDesc->wTotalNumCmds++] = pCmdDesc;

            /* adjust frame length */
            wCmdsLen = (EC_T_WORD)(wCmdsLen + ETYPE_EC_CMD_GETLEN(&pCmdDesc->EcCmdHeader));

            /* In dynamic mode we create new frames and store as much as possible cmds in it */
            if (m_eCycFrameLayout == eCycFrameLayout_DYNAMIC)
            {
                EcCycFrameDesc* pFrameDesc = EC_NULL;
                /* if this is the first frame or the frame would be too large with this command we need a new one */
                if ((pCyclicEntryMgmtDesc->wNumFrames == 0) || (wCmdsLen > (ETHERNET_MAX_FRAME_LEN - wHeaderLen)))
                {
                    pFrameDesc = (EcCycFrameDesc*)OsMalloc(sizeof(EcCycFrameDesc));
                    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::pFrameDesc", pFrameDesc, sizeof(EcCycFrameDesc));
                    if (pFrameDesc != EC_NULL)
                    {
                        OsMemset(pFrameDesc, 0, sizeof(EcCycFrameDesc));
                    }
                    else
                    {
                        dwRetVal = EC_E_NOMEMORY;
                        goto Exit;
                    }
                    pCyclicEntryMgmtDesc->apEcCycFrameDesc[pCyclicEntryMgmtDesc->wNumFrames++] = pFrameDesc;
                    wCmdsLen = ETYPE_EC_CMD_GETLEN(&pCmdDesc->EcCmdHeader);
                }
                /* get current frame desc from managment desc */
                pFrameDesc = pCyclicEntryMgmtDesc->apEcCycFrameDesc[pCyclicEntryMgmtDesc->wNumFrames - 1];
                /* insert cyclic command in frame and update frame length */
                pFrameDesc->apCycCmd[pFrameDesc->wCmdCnt++] = pCmdDesc;
                EC_88A4HDR_SET_E88A4FRAMELEN(&(pFrameDesc->e88A4Frame.E88A4Header), wCmdsLen);
            }
        }
        /* use information from ENI file */
        if (m_eCycFrameLayout != eCycFrameLayout_DYNAMIC && pCurrEcCycFrameDesc->wCmdCnt > 0)
        {
            /* copy frame in managment description */
            EcCycFrameDesc* pFrameDesc = (EcCycFrameDesc*)OsMalloc(sizeof(EcCycFrameDesc));
            EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::pFrameDesc", pFrameDesc, sizeof(EcCycFrameDesc));
            if (EC_NULL == pFrameDesc)
            {
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }

            OsMemcpy(pFrameDesc, pCurrEcCycFrameDesc, sizeof(EcCycFrameDesc));
            pCyclicEntryMgmtDesc->apEcCycFrameDesc[pCyclicEntryMgmtDesc->wNumFrames++] = pFrameDesc;

            EC_88A4HDR_SET_E88A4FRAMELEN(&(pFrameDesc->e88A4Frame.E88A4Header), wCmdsLen);
            wCmdsLen = 0;
        }

        if (pCyclicEntryMgmtDesc->wNumFrames >= EC_MAX_NUM_CYC_FRAMES)
        {
            EC_ERRORMSG(("CEcMaster::CreateCyclicCmds(): too many cyclic frames, master only supports %d frames)!\n", EC_MAX_NUM_CYC_FRAMES));
        }
    }


    /* process frames in managment description */
    for (wFrameIdx = 0; wFrameIdx < pCyclicEntryMgmtDesc->wNumFrames; wFrameIdx++)
    {
        pCurrEcCycFrameDesc = pCyclicEntryMgmtDesc->apEcCycFrameDesc[wFrameIdx];

        /* normally this is the broadcast Ethernet address. */
        pCurrEcCycFrameDesc->e88A4Frame.EthernetFrame.Destination = pCycCmdConfig->macTarget;
        pCurrEcCycFrameDesc->e88A4Frame.EthernetFrame.Source = m_pMasterDesc->oMacSrc;
        EC_ETHFRM_SET_FRAMETYPE(&(pCurrEcCycFrameDesc->e88A4Frame.EthernetFrame), ETHERNET_FRAME_TYPE_BKHF);
        EC_88A4HDR_SET_E88A4HDRTYPE(&(pCurrEcCycFrameDesc->e88A4Frame.E88A4Header), ETYPE_88A4_TYPE_ECAT);

        pCurrEcCycFrameDesc->vlanHeader = pCycCmdConfig->vlanHeader;
        /* determine command header IDX for cyclic frame */
        pCurrEcCycFrameDesc->byCmdHdrIdx = (EC_T_BYTE)m_wAllCycEntriesNumFrames;
        pCyclicEntryMgmtDesc->byFrameIdxLast = pCurrEcCycFrameDesc->byCmdHdrIdx;
        if (pCyclicEntryMgmtDesc->byFrameIdxFirst == 0xff)
        {
            pCyclicEntryMgmtDesc->byFrameIdxFirst = pCurrEcCycFrameDesc->byCmdHdrIdx;
        }

        if ((m_wAllCycEntriesNumFrames + 1) >= (EC_T_WORD)(m_byMinAcycCmdIdx))
        {
            EC_ERRORMSG(("CEcMaster::CreateCyclicCmds(): too many cyclic commands in XML configuration file (max = %d)\n", m_byMinAcycCmdIdx));
            dwRetVal = EC_E_CYC_CMDS_OVERFLOW;
            goto Exit;
        }

        switch (m_eCycFrameLayout)
        {
        case eCycFrameLayout_STANDARD:
        case eCycFrameLayout_DYNAMIC:
            /* always allocate maximum because frames may be filled with more/other cmds in case */
            pCurrEcCycFrameDesc->pbyFrame = (EC_T_BYTE*)OsMalloc(ETHERNET_MAX_FRAMEBUF_LEN);
            EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster::CreateCyclicFrames", pCurrEcCycFrameDesc->pbyFrame, ETHERNET_MAX_FRAMEBUF_LEN);
            if (pCurrEcCycFrameDesc->pbyFrame == EC_NULL)
            {
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }
            break;
        case eCycFrameLayout_FIXED:
            /* use process image memory */
            if (m_oMemProvider.dwPDOutDataLength >= (EC_T_DWORD)((m_wAllCycEntriesNumFrames + 1) * ETHERNET_MAX_FRAMEBUF_LEN))
            {
                pCurrEcCycFrameDesc->pbyFrame = m_oMemProvider.pbyPDOutData + (m_wAllCycEntriesNumFrames * ETHERNET_MAX_FRAMEBUF_LEN);
            }
            else
            {
                EC_ERRORMSG(("CEcMaster::CreateCyclicCmds(): Process image too small for fixed layout!\n"));
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }
            break;
        case eCycFrameLayout_IN_DMA:
            /* allocate frame buffer in DMA */
        {
            EC_T_LINK_FRAMEDESC oLinkFrameDesc;

            OsMemset(&oLinkFrameDesc, 0, sizeof(EC_T_LINK_FRAMEDESC));
            dwRes = m_poEcDevice->FrameAlloc(&oLinkFrameDesc, ETHERNET_MAX_FRAMEBUF_LEN);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            if (EC_NULL == oLinkFrameDesc.pbyFrame)
            {
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_NOTSUPPORTED;
                goto Exit;
            }
#if 1
            pCurrEcCycFrameDesc->pvContext = oLinkFrameDesc.pvContext;
            pCurrEcCycFrameDesc->pbyFrame = oLinkFrameDesc.pbyFrame;
            pCurrEcCycFrameDesc->wSize = 0;
#else
            m_oMemProvider.pbyPDOutData = oLinkFrameDesc.pbyFrame;
            m_oMemProvider.dwPDOutDataLength = m_oMemProvider.dwPDOutDataLength; /* TODO m_oMemProvider.dwPDOutDataLength currently set by pEcDevice->CreateImage() */
            pCurrEcCycFrameDesc->pbyFrame = oLinkFrameDesc.pbyFrame;
            pCurrEcCycFrameDesc->wSize = 0;

            m_poEcDevice->DeviceFlushRecvFrames();
            OsMemset(&oLinkFrameDesc, 0, sizeof(EC_T_LINK_FRAMEDESC));
            dwRes = m_poEcDevice->RecvFrame(&oLinkFrameDesc);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            if (EC_NULL == oLinkFrameDesc.pbyFrame)
            {
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_NOTSUPPORTED;
                goto Exit;
            }
            m_oMemProvider.pbyPDInData = oLinkFrameDesc.pbyFrame;
            m_oMemProvider.dwPDInDataLength = m_oMemProvider.dwPDInDataLength; /* TODO m_oMemProvider.dwPDInDataLength currently set by pEcDevice->CreateImage() */
#endif
        }
            break;
        default:
            OsDbgAssert(EC_FALSE);
            break;
        }

        /* count this  frame  */
        m_wAllCycEntriesNumFrames++;
    }
    OsDbgAssert (pCyclicEntryMgmtDesc->wNumFrames > 0);              /* at least one frame should have been created */

    switch (m_eCycFrameLayout)
    {
    case eCycFrameLayout_STANDARD:
    case eCycFrameLayout_DYNAMIC:
        /* nothing to do */
        break;
    case eCycFrameLayout_FIXED:
    case eCycFrameLayout_IN_DMA:
        dwRes = CreateCycFrameTemplates (pCyclicEntryMgmtDesc);
        if (dwRes != EC_E_NOERROR)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
        break;
    default:
        OsDbgAssert(EC_FALSE);
        break;
    }

    /* prepare cyclic frames monitoring */
    m_byMaxCycleIdx = (EC_T_BYTE)((m_byMinAcycCmdIdx - 1) / m_wAllCycEntriesNumFrames);
    m_byMaxCycCmdIdx = (EC_T_BYTE)(m_byMaxCycleIdx * m_wAllCycEntriesNumFrames);

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}


/********************************************************************************/
/** \brief Remove duplicate copy info entries and correct bit offs for comp mode
*
* \return EC_E_NOERROR or error code
*/
EC_T_DWORD CEcMaster::S2SOptimizeCopyInfos(CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc)
{
    EC_T_DWORD          dwRetVal = EC_E_NOERROR;
    EC_T_WORD           wOptNumOf = 0;
    EC_T_WORD           wSrcIdx = 0;
    EC_T_WORD           wOptIdx = 0;
    EC_T_CYC_COPY_INFO* pSrcInfo = EC_NULL;
    EC_T_CYC_COPY_INFO* pOptInfo = EC_NULL;
    EC_T_BOOL           bFound = EC_FALSE;

    /* list of unique copy info entries (no duplicates) */
    EC_T_CYC_COPY_INFO* paOptCopyInfo = EC_NULL;

    paOptCopyInfo = (EC_T_CYC_COPY_INFO*)OsMalloc(sizeof(EC_T_CYC_COPY_INFO) * pCyclicEntryMgmtDesc->wNumOfCopyInfos);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster:paOptCopyInfo", paOptCopyInfo,
        sizeof(EC_T_CYC_COPY_INFO) * pCyclicEntryMgmtDesc->wNumOfCopyInfos);
    if (EC_NULL == paOptCopyInfo)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(paOptCopyInfo, 0, sizeof(EC_T_CYC_COPY_INFO) * pCyclicEntryMgmtDesc->wNumOfCopyInfos);

    /* create list of unique copy info entries (no duplicates) */
    for (wSrcIdx = 0, pSrcInfo = pCyclicEntryMgmtDesc->pCopyInfo; wSrcIdx < pCyclicEntryMgmtDesc->wNumOfCopyInfos; wSrcIdx++, pSrcInfo++)
    {
        /* search duplicate */
        bFound = EC_FALSE;
        for (wOptIdx = 0, pOptInfo = paOptCopyInfo; (wOptIdx < wOptNumOf) && !bFound; wOptIdx++, pOptInfo++)
        {
            if (pSrcInfo->wTaskId == pOptInfo->wTaskId)
            {
                if ((pSrcInfo->wSrcBitOffs == pOptInfo->wSrcBitOffs) &&
                    (pSrcInfo->wDstBitOffs == pOptInfo->wDstBitOffs) &&
                     (pSrcInfo->wBitSize == pOptInfo->wBitSize))
                {
                    bFound = EC_TRUE;
                }
            }
        }

        /* store unique entry */
        if (!bFound)
        {
            pOptInfo = paOptCopyInfo + wOptNumOf;
            if( ((pSrcInfo->wSrcBitOffs % 8) == 0) && ((pSrcInfo->wDstBitOffs % 8) == 0) && ((pSrcInfo->wBitSize % 8) == 0))
            {
                pSrcInfo->wFlags |= CYC_COPY_INFO_FLAG_BYTE_COPY;    
            }
            OsMemcpy(pOptInfo, pSrcInfo, sizeof(EC_T_CYC_COPY_INFO));
            wOptNumOf++;
        }
    }

    /* replace original list with unique copy info entries */
    pCyclicEntryMgmtDesc->wNumOfCopyInfos = wOptNumOf;
    OsMemcpy(pCyclicEntryMgmtDesc->pCopyInfo, paOptCopyInfo, sizeof(EC_T_CYC_COPY_INFO) * wOptNumOf);

    /* correct bit offets for compatibility mode (before V2.1) */
    if (m_bPdOffsetCompMode)
    {
        for (wSrcIdx = 0, pSrcInfo = pCyclicEntryMgmtDesc->pCopyInfo; wSrcIdx < pCyclicEntryMgmtDesc->wNumOfCopyInfos; wSrcIdx++, pSrcInfo++)
        {
            pSrcInfo->wSrcBitOffs = (EC_T_WORD)(pSrcInfo->wSrcBitOffs + (m_dwMinCycInOffset*8)  - m_dwMinRecv);
            pSrcInfo->wDstBitOffs = (EC_T_WORD)(pSrcInfo->wDstBitOffs + (m_dwMinCycOutOffset*8) - m_dwMinSend);
        }
    }

Exit:
    if (EC_NULL != paOptCopyInfo)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster:paOptCopyInfo", paOptCopyInfo,
            sizeof(EC_T_CYC_COPY_INFO) * pCyclicEntryMgmtDesc->wNumOfCopyInfos);
        OsFree(paOptCopyInfo);
    }
    return dwRetVal;
}

#if (defined INCLUDE_VARREAD)
/********************************************************************************/
/** \brief SwapData
*
* \return status value
*/
EC_T_VOID CEcMaster::SwapData
(CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc
,EC_T_BOOL               bInputData
,EC_T_BYTE*              pbyPDBase
)
{
EC_T_DWORD dwIdx = 0;

    /* swap information */
    for (dwIdx = 0; dwIdx < pCyclicEntryMgmtDesc->wNumOfSwapInfos; dwIdx++)
    {
    EC_T_CYC_SWAP_INFO* pSwapInfo = &pCyclicEntryMgmtDesc->pSwapInfo[dwIdx];

        if (pSwapInfo->bIsInputData != bInputData)
        {
            continue;
        }
        OsDbgAssert((m_eCycFrameLayout != eCycFrameLayout_IN_DMA));
        {
        EC_T_BYTE* pbyOffset = pbyPDBase + pSwapInfo->wBitOffs/8;

            switch (pSwapInfo->wBitSize)
            {
            case 16: { EC_T_WORD    wTmp = EC_GETWORD(pbyOffset);   wTmp = EC_WORDSWAP(wTmp);   EC_SETWORD(pbyOffset,   wTmp); } break;
            case 32: { EC_T_DWORD  dwTmp = EC_GETDWORD(pbyOffset); dwTmp = EC_DWORDSWAP(dwTmp); EC_SETDWORD(pbyOffset, dwTmp); } break;
            case 64: { EC_T_UINT64 qwTmp = EC_GETQWORD(pbyOffset); qwTmp = EC_QWORDSWAP(qwTmp); EC_SETQWORD(pbyOffset, qwTmp); } break;
            default: OsDbgAssert(EC_FALSE);
            }
        }
    }
}
#endif /* INCLUDE_VARREAD */

/********************************************************************************/
/** \brief Slave-to-Slave communication: copy data
*
* \return status value
*/
EC_T_DWORD CEcMaster::S2SCopyData
(CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc
,EC_T_BYTE*              pbyPDInpBase
,EC_T_BYTE*              pbyPDOutBase
)
{
    EC_T_DWORD              dwRetVal = EC_E_NOERROR;
    EC_T_WORD               wSrcIdx = 0;
    EC_T_CYC_COPY_INFO*     pSrcInfo = EC_NULL;

    /* go through array */
    for (wSrcIdx = 0, pSrcInfo = pCyclicEntryMgmtDesc->pCopyInfo; wSrcIdx < pCyclicEntryMgmtDesc->wNumOfCopyInfos; wSrcIdx++, pSrcInfo++)
    {
        if(pSrcInfo->wFlags & CYC_COPY_INFO_FLAG_BYTE_COPY)
        {
            OsMemcpyPdOut(&pbyPDOutBase[pSrcInfo->wDstBitOffs/8], &pbyPDInpBase[pSrcInfo->wSrcBitOffs/8], pSrcInfo->wBitSize/8);
        }
        else
        {
            OsCopyBitsPdOut(pbyPDOutBase, pSrcInfo->wDstBitOffs, pbyPDInpBase, pSrcInfo->wSrcBitOffs, pSrcInfo->wBitSize);
        }
    }

    return dwRetVal;
}


#if (defined INCLUDE_FORCE_PROCESSDATA)
/********************************************************************************/
/** \brief Force process data outputs
*
* \return status value
*/
EC_T_DWORD CEcMaster::ForceProcessDataOutputs
   (EC_T_BYTE*              pbyPDOutBase
)
{
   EC_T_DWORD              dwRetVal = EC_E_NOERROR;
   EC_T_WORD               wIdx = 0;
   EC_T_FORCE_RELEASE_PROCESS_DATA_INFO*   pEntry;

    /* go through array */
    for (wIdx = 0, pEntry = m_pForceProcessDataList; wIdx < m_wForceProcessDataEntries; wIdx++, pEntry++)
    {
        if((pEntry->wStatus & EC_FORCE_RELEASE_STATUS_USED) && (pEntry->wStatus & EC_FORCE_RELEASE_STATUS_OUTPUT))
        {
           OsCopyBitsPdOut(pbyPDOutBase, pEntry->dwBitOffset, pEntry->abyData, 0, pEntry->wBitLength);
        }
    }

    return dwRetVal;
}


/********************************************************************************/
/** \brief Force process data inputs
*
* \return status value
*/
EC_T_DWORD CEcMaster::ForceProcessDataInputs
   (EC_T_BYTE*              pbyPDInpBase
)
{
   EC_T_DWORD              dwRetVal = EC_E_NOERROR;
   EC_T_WORD               wIdx = 0;
   EC_T_FORCE_RELEASE_PROCESS_DATA_INFO*   pEntry;

    /* go through array */
    for (wIdx = 0, pEntry = m_pForceProcessDataList; wIdx < m_wForceProcessDataEntries; wIdx++, pEntry++)
    {
        if((pEntry->wStatus & EC_FORCE_RELEASE_STATUS_USED) && !(pEntry->wStatus & EC_FORCE_RELEASE_STATUS_OUTPUT))
        {
           OsCopyBitsPdOut(pbyPDInpBase, pEntry->dwBitOffset, pEntry->abyData, 0, pEntry->wBitLength);
        }
    }

    return dwRetVal;
}


/********************************************************************************/
/** \brief Manage Force request
*
* \return status value
*/

EC_T_DWORD CEcMaster::ManageForceData
   (EC_T_FORCE_RELEASE_PROCESS_DATA_REQ*              poRequest
)
{
    EC_T_DWORD              dwRetVal = EC_E_NOERROR;
    EC_T_WORD               wIdx = 0;
    EC_T_FORCE_RELEASE_PROCESS_DATA_INFO*   pEntry = EC_NULL;
    EC_T_BOOL               bFoundEntry = EC_FALSE;

    if(poRequest->bReleaseAll)
    {
        for (wIdx = 0, pEntry = m_pForceProcessDataList; wIdx < m_wForceProcessDataEntries; wIdx++, pEntry++)
        {
            if(pEntry->wStatus & EC_FORCE_RELEASE_STATUS_USED)
            {
                /* search for entry in array and just check clientId */
                if(pEntry->dwClientId == poRequest->dwClientId)
                {
                    /* remove entry */
                    pEntry->wStatus = pEntry->wBitLength = 0;
                    pEntry->dwBitOffset = pEntry->dwClientId  = 0;
                }
            }
        }
    }
    else
    {
        /* check if already a force entry for this variable exists */
        for (wIdx = 0, pEntry = m_pForceProcessDataList; wIdx < m_wForceProcessDataEntries; wIdx++, pEntry++)
        {
            if(pEntry->wStatus & EC_FORCE_RELEASE_STATUS_USED)
            {
                /* search for entry in array */
                if((pEntry->dwBitOffset == poRequest->dwBitOffset) && (pEntry->wBitLength == poRequest->wBitLength) &&
                    (pEntry->dwClientId == poRequest->dwClientId))
                {
                    if((pEntry->wStatus & EC_FORCE_RELEASE_STATUS_OUTPUT) && (poRequest->bOutput))
                    {
                        if(poRequest->bRelease)
                        {
                            /* remove output force entry */
                            pEntry->wStatus = pEntry->wBitLength = 0;
                            pEntry->dwBitOffset = pEntry->dwClientId  = 0;
                        }
                        else
                        {
                            bFoundEntry = EC_TRUE;
                        }
                        break;
                    }
                    else if(!(pEntry->wStatus & EC_FORCE_RELEASE_STATUS_OUTPUT) && (!poRequest->bOutput))
                    {
                        if(poRequest->bRelease)
                        {
                            /* remove input force entry */
                            pEntry->wStatus = pEntry->wBitLength = 0;
                            pEntry->dwBitOffset = pEntry->dwClientId  = 0;
                        }
                        else
                        {
                            bFoundEntry = EC_TRUE;
                        }
                        break;
                    }
                }
            }
        }

        /* do we have to force ? */
        if(!poRequest->bRelease)
        {
            /* search for empty entry ? */
            if(!bFoundEntry)
            {
                for (wIdx = 0, pEntry = m_pForceProcessDataList; wIdx < m_wForceProcessDataMaxEntries; wIdx++, pEntry++)
                {
                    /* search for free entry */
                    if(!(pEntry->wStatus & EC_FORCE_RELEASE_STATUS_USED))
                    {
                        bFoundEntry = EC_TRUE;
                        break;
                    }
                }
            }

            if(bFoundEntry)     /* if we found one */
            {
                pEntry->wStatus = EC_FORCE_RELEASE_STATUS_USED;
                m_wForceProcessDataEntries = EC_MAX(m_wForceProcessDataEntries, (EC_T_WORD)(wIdx + 1));
                if(poRequest->bOutput)  pEntry->wStatus |= EC_FORCE_RELEASE_STATUS_OUTPUT;
                pEntry->dwClientId = poRequest->dwClientId;
                pEntry->wBitLength = poRequest->wBitLength;
                pEntry->dwBitOffset = poRequest->dwBitOffset;
                OsMemcpy(pEntry->abyData, poRequest->abyData, sizeof(pEntry->abyData));     /* store force data */
            }
            else
            {
                dwRetVal = EC_E_NOMEMORY;
            }
        }
    }

    return dwRetVal;
}
#endif

/********************************************************************************/
/** \brief Returns cyclic entry by index
*
* \return descriptor pointer
*/
CYCLIC_ENTRY_MGMT_DESC* CEcMaster::GetCycEntryDesc(EC_T_INT nCycEntryIndex)
{
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    OsDbgAssert(nCycEntryIndex < (EC_T_INT)m_dwNumCycConfigEntries);
#else
    OsDbgAssert(nCycEntryIndex == 0);
#endif
    if (EC_NULL == m_aCyclicEntryMgmtDesc)
        return EC_NULL;

    return &m_aCyclicEntryMgmtDesc[nCycEntryIndex];
}

/********************************************************************************/
/** \brief Returns sum of cyclic Entries commands number
*
* \return num commands
*/
EC_T_WORD CEcMaster::GetCycConfigEntriesNumCmds()
{
    EC_T_WORD wNumCmds = 0;
    EC_T_DWORD dwCycEntryIndex = 0;
    
    /* iterate through all cyclic entries */
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    for (dwCycEntryIndex = 0; dwCycEntryIndex < this->GetNumCycConfigEntries(); dwCycEntryIndex++)
#endif
    {
        CYCLIC_ENTRY_MGMT_DESC* pCycEntry = this->GetCycEntryDesc((EC_T_INT)dwCycEntryIndex);
        wNumCmds = (EC_T_WORD)(wNumCmds + pCycEntry->wTotalNumCmds);
    }
    return wNumCmds;
}

/********************************************************************************/
/** \brief Determine cyclic configuration
*
* \return status value
*/
EC_T_DWORD CEcMaster::GetMasterCycConfig
(EC_T_DWORD            dwCycEntryIndex
,EC_T_CYC_CONFIG_DESC* pCycConfigDesc
)
{
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    pCycConfigDesc->dwNumCycEntries = m_dwNumCycConfigEntries;
#else
    pCycConfigDesc->dwNumCycEntries = IsConfigured() ? 1 : 0;
#endif

    if (dwCycEntryIndex < pCycConfigDesc->dwNumCycEntries )
    {
        pCycConfigDesc->dwTaskId    = m_aCyclicEntryMgmtDesc[dwCycEntryIndex].dwTaskId;
        pCycConfigDesc->dwPriority  = m_aCyclicEntryMgmtDesc[dwCycEntryIndex].dwPriority;
        pCycConfigDesc->dwCycleTime = m_aCyclicEntryMgmtDesc[dwCycEntryIndex].dwCycleTime;
        return EC_E_NOERROR;
    }
    return EC_E_INVALIDINDEX;
}


/********************************************************************************/
/** \brief Determine cyclic configuration
*
* \return status value
*/
EC_T_BOOL CEcMaster::IsSlaveToSlaveCommConfigured(EC_T_VOID)
{
EC_T_DWORD dwCycEntryIdx;

#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    for (dwCycEntryIdx = 0; dwCycEntryIdx < m_dwNumCycConfigEntries; dwCycEntryIdx++)
#else
    dwCycEntryIdx = 0;
#endif
    {
        if (0 != m_aCyclicEntryMgmtDesc[dwCycEntryIdx].wNumOfCopyInfos)
        {
            return EC_TRUE;
        }
    }
    return EC_FALSE;
}


/********************************************************************************/
/** \brief Creates the EtherCAT frames that should be sent cyclically
*
* No VLAN support for FIXED layout
* \return
*/
EC_T_DWORD CEcMaster::CreateCycFrameTemplates
    (CYCLIC_ENTRY_MGMT_DESC* pCyclicEntryMgmtDesc
)
{
PETHERNET_88A4_HEADER pE88A = EC_NULL;
EC_T_BYTE* pbyFrame;
EC_T_WORD wFrameIdx;
EC_T_DWORD dwRes = EC_E_NOERROR;

    OsDbgAssert((m_eCycFrameLayout == eCycFrameLayout_FIXED) || (m_eCycFrameLayout == eCycFrameLayout_IN_DMA));
#if (defined VLAN_FRAME_SUPPORT)
    OsDbgAssert(!m_bVLANEnabled);
#endif
    /* iteration of cyclic frames */
    for (wFrameIdx = 0; (wFrameIdx < pCyclicEntryMgmtDesc->wNumFrames) && (dwRes == EC_E_NOERROR); wFrameIdx++ )
    {
        EcCycFrameDesc* pFrameDesc = pCyclicEntryMgmtDesc->apEcCycFrameDesc[wFrameIdx];

        if (m_eCycFrameLayout == eCycFrameLayout_IN_DMA)
        {
            pbyFrame = GetPDOutPtr();
        }
        else
        {
            pbyFrame = pFrameDesc->pbyFrame;
        }
        if (pbyFrame == EC_NULL)
        {
            OsDbgAssert(EC_FALSE);
            dwRes = EC_E_NOMEMORY;
            break;
        }

        OsMemcpy(&pbyFrame[0], &pFrameDesc->e88A4Frame, ETHERNET_88A4_FRAME_LEN);
        pE88A = (PETHERNET_88A4_HEADER)&pbyFrame[ETHERNET_FRAME_LEN];

        /* get pointer of First EtherCAT Command */
        EC_T_BYTE* pPtr = (EC_T_BYTE*)&(pE88A->sETECAT.FirstEcCmdHeader);
        PETYPE_EC_CMD_HEADER pLastEcCmdHeader = EC_NULL;

        /* iteration of commands */
        for (EC_T_WORD wCmdIdx=0; wCmdIdx < pFrameDesc->wCmdCnt; wCmdIdx++ )
        {
            EcCycCmdConfigDesc* pCmdDesc = pFrameDesc->apCycCmd[wCmdIdx];
            PETYPE_EC_CMD_HEADER pEcCmdHeader  = (PETYPE_EC_CMD_HEADER)pPtr;

            pLastEcCmdHeader = pEcCmdHeader;

            /* refresh command descriptor */
            EC_ICMDHDR_SET_LEN_NEXT(&pCmdDesc->EcCmdHeader, EC_TRUE);
            if (wCmdIdx == 0)
            {
                /* set frame index in the first command */
                pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byIdx = pFrameDesc->byCmdHdrIdx;
            }

            /* copy cmd header */
            *pEcCmdHeader = pCmdDesc->EcCmdHeader;

            /* save offset of command type */
            pCmdDesc->wCmdTypeOffset = (EC_T_WORD)((EC_T_BYTE*)pEcCmdHeader - pbyFrame);

            /* commands are initially disabled */
            pbyFrame[pCmdDesc->wCmdTypeOffset] = EC_CMD_TYPE_NOP;
            pCmdDesc->bActive = EC_FALSE;

            pPtr += ETYPE_EC_CMD_HEADER_LEN;

            /* clear payload area */
            OsMemset(pPtr, 0, pCmdDesc->wDataLen);
            pPtr += pCmdDesc->wDataLen;

            /* WKC */
            EC_SET_FRM_WORD(pPtr, 0);
            pPtr += sizeof(EC_T_WORD);
        }
        /* terminate current frame */
        if (0 != pFrameDesc->wCmdCnt)
        {
            EC_ICMDHDR_SET_LEN_NEXT(&(pFrameDesc->apCycCmd[pFrameDesc->wCmdCnt-1]->EcCmdHeader), EC_FALSE);
            EC_ICMDHDR_SET_LEN_NEXT(pLastEcCmdHeader, EC_FALSE);
        }
        pFrameDesc->wSize = (EC_T_WORD)(pPtr - pbyFrame);

    } /* for (wFrameIdx=0; wFrameIdx < pCyclicEntryMgmtDesc->wNumFrames; wFrameIdx++ ) */

    return dwRes;
}


/********************************************************************************/
/** \brief Processes the response of an EtherCAT command.
*
* \return
*/
EC_T_VOID CEcMaster::ProcessCmdResult(EC_T_DWORD result, PECAT_SLAVECMD_DESC pSlaveCmd, PETYPE_EC_CMD_HEADER pEcCmdHeader)
{
    EC_T_NOTIFICATION_INTDESC*  pNotification   = EC_NULL;
    EC_T_WORD                   wEcCmdLen       = 0;

    EC_T_DWORD                  dwInvokeId      = pSlaveCmd->invokeId;

    if (EC_NULL != pEcCmdHeader )
    {
        wEcCmdLen = EC_CMDHDRLEN_GET_LEN(&(pEcCmdHeader->uLen));
    }
    else
    {
        wEcCmdLen = 0;
    }

    /* low values are reserved for transition code invoke ids, see ECAT_INITCMD_I_P, ECAT_INITCMD_BEFORE, ... */
    if (dwInvokeId <= 0xFFFF)
    {
        if ( m_nInitCmdsCurrent != INITCMD_INACTIVE && m_nInitCmdsCurrent < m_nInitCmdsCount )
        {
            EC_TRACEMSG(EC_TRACE_CORE, ("CEcMaster::ProcessCmdResult() invokeId = %s (0x%lx)\n", GetStateChangeNameShort((EC_T_WORD)(dwInvokeId&0x7ff)), (EC_T_INT)dwInvokeId));
            if ( result == EC_E_NOERROR )
            {
                /* check working counter if necessary ( cnt != 0xFFFF (ECAT_WCOUNT_DONT_CHECK) ) */
                if ( EC_ECINITCMDDESC_GET_CNT(m_ppInitCmds[m_nInitCmdsCurrent]) != 0xFFFF && ETYPE_EC_CMD_GETWKC(pEcCmdHeader) != EC_ECINITCMDDESC_GET_CNT(m_ppInitCmds[m_nInitCmdsCurrent]) )
                {
                    /* invalid working counter */
                    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
                    if (EC_NULL != pNotification)
                    {
                        pNotification->uNotification.ErrorNotification.dwNotifyErrorCode  = EC_NOTIFY_MASTER_INITCMD_WKC_ERROR;
                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.byCmd   = pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd;
                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.dwAddr  = EC_ICMDHDR_GET_ADDR(pEcCmdHeader);
                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcAct = ETYPE_EC_CMD_GETWKC(pEcCmdHeader);
                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcSet = EC_ECINITCMDDESC_GET_CNT(m_ppInitCmds[m_nInitCmdsCurrent]);
                        NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_WKCERR);
                    }

                    if (EC_ECINITCMDDESC_GET_RETRIES(m_ppInitCmds[m_nInitCmdsCurrent]) > 0 && !m_bCancelStateMachine)
                    {   /* retry */
                        PEcInitCmdDesc pInitCmdDesc = m_ppInitCmds[m_nInitCmdsCurrent];

                        EC_TRACEMSG(
                            EC_TRACE_INITCMDS,
                            ("CEcMaster::ProcessCmdResult() Retry send master InitCmd[%d]: %s: %s,adp=%d,ado=0x%x\n",
                             m_nInitCmdsCurrent,
                             GetStateChangeNameShort((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)),
                             EcatCmdShortText(pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd),
                             (int)EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                             (int)EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader))
                            )      );

                        /* send EtherCAT command again */
                        QueueRegisterCmdReq(
                            EC_NULL,
                            dwInvokeId,
                            pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd,
                            EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader)),
                            EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                            EC_ICMDHDR_GET_LEN_LEN(&(pInitCmdDesc->EcCmdHeader)),
                            EcInitCmdDescData(pInitCmdDesc)
                                       );
                        if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                        {
                            m_oInitCmdRetryTimeout.Start(m_dwInitCmdRetryTimeout, GetMsecCounterPtr());
                            EC_ECINITCMDDESC_SET_RETRIES(m_ppInitCmds[m_nInitCmdsCurrent], (EC_T_WORD)(EC_ECINITCMDDESC_GET_RETRIES(m_ppInitCmds[m_nInitCmdsCurrent])-1));
                        }
                    }
                    else
                    {
                        EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult() '%s' failed! Error: '%s'. \nWorking Counter '%d' read and '%d' expected.\n",
                            GetStateChangeName((EC_T_WORD)(dwInvokeId&(~ECAT_INITCMD_BEFORE))), EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]),
                            ETYPE_EC_CMD_GETWKC(pEcCmdHeader), EC_ECINITCMDDESC_GET_CNT(m_ppInitCmds[m_nInitCmdsCurrent])));

                        /* last master init command failed or state machine has been canceled, and stabilize state machine */
                        StabilizeMasterStateMachine((m_bCancelStateMachine?EC_E_CANCEL:EC_E_TIMEOUT), GetMasterLowestDeviceState());
                    }
                    goto Exit;
                }

                EC_T_BOOL bNext = EC_TRUE;
                if ( EC_ECINITCMDDESC_GET_VALIDATE(m_ppInitCmds[m_nInitCmdsCurrent]) && wEcCmdLen == EC_ICMDHDR_GET_LEN_LEN(&(m_ppInitCmds[m_nInitCmdsCurrent]->EcCmdHeader)) )
                {   /* initcmd with validation */
                    EC_T_BYTE* pData = (EC_T_BYTE*)EC_ENDOF(pEcCmdHeader);
                    EC_T_DWORD nmData[2] = {0, 0};
                    if ( EC_ECINITCMDDESC_GET_VALIDATEMASK(m_ppInitCmds[m_nInitCmdsCurrent]) )
                    {
                        if ( wEcCmdLen <= sizeof(nmData) )
                        {
                            OsMemcpy(nmData, pData, wEcCmdLen);
                        }

                        EC_T_BYTE* pMask = (EC_T_BYTE*)EcInitCmdDescVMData(m_ppInitCmds[m_nInitCmdsCurrent]);
                        for (EC_T_INT i=0; i < wEcCmdLen; i++ )
                        {
                            pData[i] &= pMask[i];
                        }
                    }
                    if ( OsMemcmp(pData, EcInitCmdDescVData(m_ppInitCmds[m_nInitCmdsCurrent]), EC_ICMDHDR_GET_LEN_LEN(pEcCmdHeader) != 0) )
                    {   /* validate failed -> retry */
                        if (m_oInitCmdsTimeout.IsElapsed() || m_bCancelStateMachine)
                        {
                            pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
                            if (EC_NULL != pNotification)
                            {
                                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_MASTER_INITCMD_RESPONSE_ERROR;
                                SAFE_STRCPY( pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.achStateChangeName, GetStateChangeName((EC_T_WORD)(dwInvokeId&(~ECAT_INITCMD_BEFORE))), MAX_SHORT_STRLEN - 1);
                                pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.EErrorType = eInitCmdErr_VALIDATION_ERR;
                                SAFE_STRCPY(pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.szComment, EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]), EC_MIN(EC_ECINITCMDDESC_GET_CMTLEN(m_ppInitCmds[m_nInitCmdsCurrent]) + 1, MAX_STD_STRLEN - 1));
                                NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_INITCMD_RESPONSE_ERROR);
                            }

                            if ( EC_ICMDHDR_GET_LEN_LEN(pEcCmdHeader) == 2*sizeof(EC_T_DWORD) )
                            {
                                if ( EC_ECINITCMDDESC_GET_VALIDATEMASK(m_ppInitCmds[m_nInitCmdsCurrent]) )
                                {
                                    EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult() '%s' failed! Error: '%s'. \nValue '0x%08x %08x' read and '0x%08x %08x' with mask '0x%08x %08x' expected.\n",
                                    GetStateChangeName((EC_T_WORD)dwInvokeId), EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]),
                                    nmData[1], nmData[0],
                                    ((EC_T_DWORD*)EcInitCmdDescVData(m_ppInitCmds[m_nInitCmdsCurrent]))[1], ((EC_T_DWORD*)EcInitCmdDescVData(m_ppInitCmds[m_nInitCmdsCurrent]))[0],
                                    ((EC_T_DWORD*)EcInitCmdDescVMData(m_ppInitCmds[m_nInitCmdsCurrent]))[1], ((EC_T_DWORD*)EcInitCmdDescVMData(m_ppInitCmds[m_nInitCmdsCurrent]))[0]));
                                }
                                else
                                {
                                    EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult() '%s' failed! Error: '%s'. \nValue '0x%08x %08x' read and '0x%08x %08x' expected.\n",
                                    GetStateChangeName((EC_T_WORD)dwInvokeId), EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]),
                                    ((EC_T_DWORD*)pData)[1], ((EC_T_DWORD*)pData)[0],
                                    ((EC_T_DWORD*)EcInitCmdDescVData(m_ppInitCmds[m_nInitCmdsCurrent]))[1], ((EC_T_DWORD*)EcInitCmdDescVData(m_ppInitCmds[m_nInitCmdsCurrent]))[0]));
                                }
                            }
                            else if ( wEcCmdLen == sizeof(EC_T_DWORD) )
                            {
                                if ( EC_ECINITCMDDESC_GET_VALIDATEMASK(m_ppInitCmds[m_nInitCmdsCurrent]) )
                                {
                                    EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult() '%s' failed! Error: '%s'. \nValue '0x%08x' read and '0x%08x' with mask '0x%08x' expected.\n",
                                    GetStateChangeName((EC_T_WORD)dwInvokeId), EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]),
                                    nmData[0], *(EC_T_DWORD*)EcInitCmdDescVData(m_ppInitCmds[m_nInitCmdsCurrent]),
                                    *(EC_T_DWORD*)EcInitCmdDescVMData(m_ppInitCmds[m_nInitCmdsCurrent])));
                                }
                                else
                                {
                                    EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult() '%s' failed! Error: '%s'. \nValue '0x%08x' read and '0x%08x' expected.\n",
                                    GetStateChangeName((EC_T_WORD)dwInvokeId), EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]),
                                    *(EC_T_DWORD*)pData, *(EC_T_DWORD*)EcInitCmdDescVData(m_ppInitCmds[m_nInitCmdsCurrent])));
                                }
                            }
                            else if ( EC_ICMDHDR_GET_LEN_LEN(pEcCmdHeader) == sizeof(EC_T_WORD) )
                            {
                                if ( EC_ECINITCMDDESC_GET_VALIDATEMASK(m_ppInitCmds[m_nInitCmdsCurrent]) )
                                {
                                    EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult() '%s' failed! Error: '%s'. \nValue '0x%04x' read and '0x%04x' with mask '0x%04x' expected.\n",
                                    GetStateChangeName((EC_T_WORD)dwInvokeId), EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]),
                                    (EC_T_WORD)(nmData[0]), *(EC_T_WORD*)EcInitCmdDescVData(m_ppInitCmds[m_nInitCmdsCurrent]),
                                    *(EC_T_WORD*)EcInitCmdDescVMData(m_ppInitCmds[m_nInitCmdsCurrent])));
                                }
                                else
                                {
                                    EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult() '%s' failed! Error: '%s'. \nValue '0x%04x' read and '0x%04x' expected.\n",
                                    GetStateChangeName((EC_T_WORD)dwInvokeId), EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]),
                                    *(EC_T_WORD*)pData, *(EC_T_WORD*)EcInitCmdDescVData(m_ppInitCmds[m_nInitCmdsCurrent])));
                                }
                            }
                            else if ( EC_ICMDHDR_GET_LEN_LEN(pEcCmdHeader) == sizeof(EC_T_BYTE) )
                            {
                                if ( EC_ECINITCMDDESC_GET_VALIDATEMASK(m_ppInitCmds[m_nInitCmdsCurrent]) )
                                {
                                    EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult() '%s' failed! Error: '%s'. \nValue '0x%02x' read and '0x%02x' with mask '0x%02x' expected.\n",
                                    GetStateChangeName((EC_T_WORD)dwInvokeId), EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]),
                                    (EC_T_BYTE)(nmData[0]), *(EC_T_BYTE*)EcInitCmdDescVData(m_ppInitCmds[m_nInitCmdsCurrent]),
                                    *(EC_T_BYTE*)EcInitCmdDescVMData(m_ppInitCmds[m_nInitCmdsCurrent])));
                                }
                                else
                                {
                                    EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult() '%s' failed! Error: '%s'. \nValue '0x%02x' read and '0x%02x' expected.\n",
                                    GetStateChangeName((EC_T_WORD)dwInvokeId), EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]),
                                    *(EC_T_BYTE*)pData, *(EC_T_BYTE*)EcInitCmdDescVData(m_ppInitCmds[m_nInitCmdsCurrent])));
                                }
                            }
                            else
                            {
                                EC_ERRORMSG(("CEcMaster::ProcessCmdResult() '%s' failed! Error: '%s'.\n",
                                GetStateChangeName((EC_T_WORD)dwInvokeId), EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent])));
                            }
                            /* last master init command failed or state machine has been canceled, stabilize state machine */
                            StabilizeMasterStateMachine((m_bCancelStateMachine?EC_E_CANCEL:EC_E_TIMEOUT), GetMasterLowestDeviceState());
                        }
                        else
                        {
                            PEcInitCmdDesc pInitCmdDesc = m_ppInitCmds[m_nInitCmdsCurrent];
                            EC_ERRORMSGC_L(
                                ("CEcMaster::ProcessCmdResult() Invalid response, retry send master InitCmd[%d]: %s: %s,adp=%d,ado=0x%x\n",
                                 m_nInitCmdsCurrent,
                                 GetStateChangeNameShort((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)),
                                 EcatCmdShortText(pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd),
                                 (int)EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                                 (int)EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader))
                                )         );
                            QueueRegisterCmdReq(
                                EC_NULL,
                                dwInvokeId,
                                pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd,
                                EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader)),
                                EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                                EC_ICMDHDR_GET_LEN_LEN(&(pInitCmdDesc->EcCmdHeader)),
                                EcInitCmdDescData(pInitCmdDesc)
                                           );
                        }
                        bNext = EC_FALSE;
                    }
                }
                if (m_bCancelStateMachine)
                {
                    m_nInitCmdsCurrent = INITCMD_INACTIVE;
                    m_bCancelStateMachine = EC_FALSE;
                    bNext = EC_FALSE;
                }
                if ( bNext )
                {
                    m_nInitCmdsCurrent++;
                    while ( m_nInitCmdsCurrent < m_nInitCmdsCount )
                    {
                        if ( (EC_ECINITCMDDESC_GET_TRANSITION(m_ppInitCmds[m_nInitCmdsCurrent]) & (dwInvokeId|ECAT_INITCMD_BEFORE)) == dwInvokeId )
                            break;
                        m_nInitCmdsCurrent++;
                    }
                    if ( m_nInitCmdsCurrent < m_nInitCmdsCount )
                    {
                        PEcInitCmdDesc pInitCmdDesc = m_ppInitCmds[m_nInitCmdsCurrent];

                        EC_TRACEMSG(
                            EC_TRACE_INITCMDS,
                            ("CEcMaster::ProcessCmdResult() Next InitCmd QuCmd[%d]: %s: %s,adp=%d,ado=0x%x\n",
                             m_nInitCmdsCurrent,
                             GetStateChangeNameShort((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)),
                             EcatCmdShortText(pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd),
                             (int)EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                             (int)EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader))
                            )      );
                        EC_TRACEMSG(EC_TRACE_INITCMDS, ("-----\n"));

                        {
                            EC_T_WORD wInitCmdTimeout = 0;

                            wInitCmdTimeout = EC_ECINITCMDDESC_GET_INITCMDTIMEOUT(pInitCmdDesc);
                            if (0 == wInitCmdTimeout)
                            {
                                wInitCmdTimeout = INIT_CMD_DEFAULT_TIMEOUT;
                            }
                            m_oInitCmdsTimeout.Start(wInitCmdTimeout, GetMsecCounterPtr());
                        }

                        m_oInitCmdRetryTimeout.Start(m_dwInitCmdRetryTimeout, GetMsecCounterPtr());

                        QueueRegisterCmdReq(
                            EC_NULL,
                            dwInvokeId,
                            pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd,
                            EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader)),
                            EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                            EC_ICMDHDR_GET_LEN_LEN(&(pInitCmdDesc->EcCmdHeader)),
                            EcInitCmdDescData(pInitCmdDesc)
                                       );
                    } /* if ( m_nInitCmdsCurrent < m_nInitCmdsCount ) */
                    else
                    {
                        EC_TRACEMSG(EC_TRACE_CORE, ("CEcMaster::ProcessCmdResult() All init commands sent for transition %s\n", GetStateChangeNameShort((EC_T_WORD)(dwInvokeId&0x7ff))));

                        /* don't send more master init commands now */
                        m_nInitCmdsCurrent = INITCMD_INACTIVE;

                        EC_TRACEMSG(EC_TRACE_CORE, ("CEcMaster::ProcessCmdResult() State Change ready invokeId 0x%x, name 'MASTER'\n", dwInvokeId));
                        EC_T_WORD wSlaveDeviceStateReq = 0;
                        switch ( dwInvokeId )
                        {
                        case ECAT_INITCMD_I_P|ECAT_INITCMD_BEFORE:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_I_P;   wSlaveDeviceStateReq = DEVICE_STATE_PREOP; break;

                        case ECAT_INITCMD_I_P:
                        case ECAT_INITCMD_S_P:
                        case ECAT_INITCMD_O_P:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_PREOP; break;

                        case ECAT_INITCMD_P_I|ECAT_INITCMD_BEFORE:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_P_I;   wSlaveDeviceStateReq = DEVICE_STATE_INIT; break;
                        case ECAT_INITCMD_P_S|ECAT_INITCMD_BEFORE:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_P_S;   wSlaveDeviceStateReq = DEVICE_STATE_SAFEOP; break;

                        case ECAT_INITCMD_P_S:
                        case ECAT_INITCMD_O_S:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_SAFEOP; break;

                        case ECAT_INITCMD_S_I|ECAT_INITCMD_BEFORE:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_S_I;   wSlaveDeviceStateReq = DEVICE_STATE_INIT; break;
                        case ECAT_INITCMD_S_P|ECAT_INITCMD_BEFORE:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_S_P;   wSlaveDeviceStateReq = DEVICE_STATE_PREOP; break;
                        case ECAT_INITCMD_S_O|ECAT_INITCMD_BEFORE:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_S_O;   wSlaveDeviceStateReq = DEVICE_STATE_OP; break;

                        case ECAT_INITCMD_S_O:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_OP; break;

                        case ECAT_INITCMD_O_I|ECAT_INITCMD_BEFORE:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_O_I;   wSlaveDeviceStateReq = DEVICE_STATE_INIT; break;
                        case ECAT_INITCMD_O_P|ECAT_INITCMD_BEFORE:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_O_P;   wSlaveDeviceStateReq = DEVICE_STATE_PREOP; break;
                        case ECAT_INITCMD_O_S|ECAT_INITCMD_BEFORE:
                            m_eCurMasterLogicalState = EC_MASTER_STATE_WAIT_SLAVE_O_S;   wSlaveDeviceStateReq = DEVICE_STATE_SAFEOP; break;
                        }
                        EC_TRACEMSG(EC_TRACE_CORE, ("CEcMaster::ProcessCmdResult() m_eCurMasterLogicalState = %s\n", GetMasterStateName(m_eCurMasterLogicalState)));
                        if (m_MasterConfig.dwStateChangeDebug&3 )
                        {
                            m_poEcDevice->LinkStateChngMsg( m_MasterConfig.dwStateChangeDebug, "Master State = %s\n", GetMasterStateName(m_eCurMasterLogicalState) );
                        }
                        if (0 != wSlaveDeviceStateReq)
                        {
                            for (EC_T_UINT i = 0; i < m_nCfgSlaveCount; i++)
                            {
                                RequestSlaveSmState(m_ppEcSlave[i], wSlaveDeviceStateReq);
                            }
                        }
                    } /* else if ( m_nInitCmdsCurrent < m_nInitCmdsCount ) */
                }
            } /* if ( result == EC_E_NOERROR ) */
            else
            {
                if (EC_ECINITCMDDESC_GET_RETRIES(m_ppInitCmds[m_nInitCmdsCurrent]) > 0 && !m_bCancelStateMachine)
                {
                    /* retry */
                    PEcInitCmdDesc pInitCmdDesc = m_ppInitCmds[m_nInitCmdsCurrent];
                    EC_ERRORMSGC_L(
                        ("CEcMaster::ProcessCmdResult() Error, retry send master InitCmd[%d]: %s: %s,adp=%d,ado=0x%x\n",
                         m_nInitCmdsCurrent,
                         GetStateChangeNameShort((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)),
                         EcatCmdShortText(pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd),
                         (int)EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                         (int)EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader))
                        )         );

                    QueueRegisterCmdReq(
                        EC_NULL,
                        dwInvokeId,
                        pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd,
                        EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader)),
                        EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                        EC_ICMDHDR_GET_LEN_LEN(&(pInitCmdDesc->EcCmdHeader)),
                        EcInitCmdDescData(pInitCmdDesc)
                                   );

                    if (m_oInitCmdRetryTimeout.IsElapsed()|| !m_oInitCmdRetryTimeout.IsStarted())
                    {
                        m_oInitCmdRetryTimeout.Start(m_dwInitCmdRetryTimeout, GetMsecCounterPtr());

                        EC_ECINITCMDDESC_SET_RETRIES(m_ppInitCmds[m_nInitCmdsCurrent],
                            (EC_T_WORD)(EC_ECINITCMDDESC_GET_RETRIES(m_ppInitCmds[m_nInitCmdsCurrent])-1)
                            );
                    }
                }
                else
                {
                    pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
                    if (EC_NULL != pNotification)
                    {
                        pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_MASTER_INITCMD_RESPONSE_ERROR;
                        SAFE_STRCPY( pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.achStateChangeName, GetStateChangeName((EC_T_WORD)(dwInvokeId&(~ECAT_INITCMD_BEFORE))), MAX_SHORT_STRLEN - 1);
                        pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.EErrorType = eInitCmdErr_NO_RESPONSE;
                        SAFE_STRCPY(pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.szComment, EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]), EC_MIN(EC_ECINITCMDDESC_GET_CMTLEN(m_ppInitCmds[m_nInitCmdsCurrent]) + 1, MAX_STD_STRLEN - 1));
                        NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_INITCMD_RESPONSE_ERROR);
                    }

                    EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult() '%s' failed! Error: '%s'. \nCommunication Error '0x%x'.\n",
                        GetStateChangeName((EC_T_WORD)(dwInvokeId&(~ECAT_INITCMD_BEFORE))), EcInitCmdDescComment(m_ppInitCmds[m_nInitCmdsCurrent]), result));

                    /* last master init command failed or state machine has been canceled, stabilize state machine */
                    StabilizeMasterStateMachine((m_bCancelStateMachine?EC_E_CANCEL:EC_E_TIMEOUT), GetMasterLowestDeviceState());
                }
            }
        } /* if ( m_nInitCmdsCurrent != INITCMD_INACTIVE && m_nInitCmdsCurrent < m_nInitCmdsCount ) */
        else
        {
            /* We did not expect this invoke-id now as all init commands are already processed.
             * We may get here in case of previous retries
             */
            EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult() Unexpected invokeId = 0x%lx, m_nInitCmdsCurrent=%d, m_nInitCmdsCount=%d\n", (EC_T_INT)dwInvokeId, (int)m_nInitCmdsCurrent, (int)m_nInitCmdsCount));
        }
    }
    /* Scanbus (EcBusTopology) */
    else if (dwInvokeId >= INVOKE_ID_SB_COUNT && dwInvokeId <= INVOKE_ID_SB_MAX)
    {
        m_pBusTopology->ProcessResult(result, pSlaveCmd, pEcCmdHeader);
    }
    /* Port operation */
    else if (dwInvokeId >= INVOKE_ID_PORTOP_CHANGEPORT && dwInvokeId <= INVOKE_ID_PORTOP_MAX)
    {
        m_oHotConnect.ProcessResult(result, dwInvokeId, pEcCmdHeader);
    }
#if (defined INCLUDE_DC_SUPPORT)
    /* Distributed Clocks (EcDistributedClocks) */
    else if (dwInvokeId >= INVOKE_ID_DC_CLEARSYSTIMEVAL && dwInvokeId <= INVOKE_ID_DC_MAX)
    {
        m_pDistributedClks->ProcessResult(result, dwInvokeId, pEcCmdHeader);
    }
#endif /* INCLUDE_DC_SUPPORT */
    /* EEPROM Access */
    else if (dwInvokeId >= INVOKE_ID_EEPA_START && dwInvokeId <= INVOKE_ID_EEPA_MAX)
    {
        m_pEEEPRom->ProcessResult(result, dwInvokeId, pEcCmdHeader);
    }
    else if (dwInvokeId == INVOKE_ID_WRITEEVENTMASK)
    {

    }
#if (defined INCLUDE_SLAVE_STATISTICS)
    else if (dwInvokeId == INVOKE_ID_GET_SLAVE_STATISTICS)
    {

    }
#endif
    else if (dwInvokeId == INVOKE_ID_TFER_RAW_CMD)
    {
        if (m_dwRawTferStatus == EC_E_BUSY)
        {
            OsDbgAssert( m_bRawTferPending );
            if ( result == EC_E_NOERROR )
            {
                if (ETYPE_EC_CMD_GETWKC(pEcCmdHeader) >= 1 )
                {
                    /* slave did respond */
                    if (m_wRawTferDataLen > 0 )
                    {
                        /* copy data into user area */
                        EC_T_BYTE* pbyTferData;
                        pbyTferData = (EC_T_BYTE*)EC_ENDOF(pEcCmdHeader);
                        OsDbgAssert( m_wRawTferDataLen <= MAX_EC_DATA_LEN );
                        OsDbgAssert( m_pbyRawTferData != EC_NULL );
                        OsMemcpy( m_pbyRawTferData, pbyTferData, m_wRawTferDataLen );
                    }
                    m_dwRawTferStatus = EC_E_NOERROR;
                }
                else
                {
                    EC_ERRORMSGC_L(("CEcMaster::ProcessCmdResult(): invalid working counter when receiving raw command result (Master)\n"));
                    m_dwRawTferStatus = EC_E_NOTREADY;
                }
            }
            else
            {
                /* communication error */
                EC_ERRORMSGC_L( ("CEcMaster::ProcessCmdResult() 'Master' : Communication Error '0x%x'.\n", result) );
                m_dwRawTferStatus = result;
            }
            OsDbgAssert( m_dwRawTferStatus != EC_E_BUSY);
        }
    }
    else if (dwInvokeId >= INVOKE_ID_CLNT_TFER_Q_RAW_CMD )
    {
        EC_T_WORD   wInvokeId   = EC_LOWORD(dwInvokeId);
        EC_T_BYTE*  pbyTferData = EC_NULL;

        pNotification = (EC_T_NOTIFICATION_INTDESC*)AllocNotification();
        if (EC_NULL != pNotification)
        {
           pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwInvokeId = EC_MAKEDWORD(0,wInvokeId);
           pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwResult   = result;
           if (EC_NULL != pEcCmdHeader )
           {
               pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwWkc      = ETYPE_EC_CMD_GETWKC(pEcCmdHeader);
               pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwCmdIdx   = EC_MAKEDWORD(0, EC_ICMDHDR_GET_CMDIDX(pEcCmdHeader));
               pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwAddr     = EC_ICMDHDR_GET_ADDR(pEcCmdHeader);
               pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwLength   = EC_MAKEDWORD(0, wEcCmdLen);
   
               pbyTferData = (EC_T_BYTE*)EC_ENDOF(pEcCmdHeader);
               pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.pbyData = pbyTferData;
           }
           if ((dwInvokeId & INVOKE_ID_TFER_Q_RAW_CMD) == INVOKE_ID_TFER_Q_RAW_CMD )
           {
               /* broadcast raw cmd */
               pNotification->dwClientID = 0;
               NotifyAndFree(EC_NOTIFY_RAWCMD_DONE, pNotification, sizeof(EC_T_RAWCMDRESPONSE_NTFY_DESC));
           }
           else
           {
               EC_T_BYTE byClntIdent;
               /* client specific raw cmd */
               byClntIdent = (EC_T_BYTE)((dwInvokeId >> 16) & 0xFF);
   
               pNotification->dwClientID = IS_CLIENT_RAW_CMD | (EC_T_DWORD)byClntIdent;
               NotifyAndFree(EC_NOTIFY_RAWCMD_DONE, pNotification, sizeof(EC_T_RAWCMDRESPONSE_NTFY_DESC));
           }
        }
    }
    else
    {
        EC_ERRORMSG(("CEcMaster::ProcessCmdResult() Invalid invokeId = 0x%lx\n", (EC_T_INT)dwInvokeId));
    }

Exit:
    return;
}

/********************************************************************************/
/** \brief Is called when a new message in a slave mailbox has been indicated.
*
* \return
*/
EC_T_VOID CEcMaster::CheckLogicalStateMBox
(   PETYPE_EC_CMD_HEADER    pEcCmdHeader
   ,EC_T_WORD               wEcCmdLen
)
{
    EC_T_BYTE   byData;
    EC_T_BYTE*  pbyData = EC_NULL;

    /* find responsible mailbox slave */
    if ((EC_ICMDHDR_GET_ADDR(pEcCmdHeader) == m_pMasterDesc->dwLogAddressMBoxStates))
    {
        pbyData = (EC_T_BYTE*)EC_ENDOF(pEcCmdHeader);

        for (EC_T_INT i=0; i < wEcCmdLen; i+=sizeof(EC_T_BYTE) )
        {
            byData = pbyData[i];

            if (0 != byData)
            {
                for (EC_T_INT b=0; b < 8; b++ )
                {
                    if (0 != (byData & (1 << b)))
                    {
                        EC_T_INT idx = i*8 + b;
                        if ((idx < m_pMasterDesc->wSizeAddressMBoxStates) && (m_ppEcPortMBoxState[idx]))
                        {
                            /* read mailbox from slave */
                            m_ppEcPortMBoxState[idx]->AddMailboxActionRequestRead();
                        }
                    }
                }
            }
        }
    }
    return;
}

/********************************************************************************/
/** \brief Register or unregister mailbox polling for the specified EtherCAT slave.
*
* \return
*/
EC_T_BOOL CEcMaster::RegisterLogicalStateMBoxPolling(CEcMbSlave* pSlave, EC_T_WORD slaveAddressMBoxState, EC_T_BOOL bUnregister)
{
    if (slaveAddressMBoxState < m_pMasterDesc->wSizeAddressMBoxStates * 8)
    {
        OsDbgAssert(m_ppEcPortMBoxState != EC_NULL);
        if ( bUnregister )
        {
            if ( m_ppEcPortMBoxState[slaveAddressMBoxState] != EC_NULL )
            {
                m_ppEcPortMBoxState[slaveAddressMBoxState] = EC_NULL;
                return EC_TRUE;
            }
        }
        else
        {
            if ( m_ppEcPortMBoxState[slaveAddressMBoxState] == EC_NULL )
            {
                m_ppEcPortMBoxState[slaveAddressMBoxState] = pSlave;
                return EC_TRUE;
            }
        }
    }
    return EC_FALSE;
}

/********************************************************************************/
/** \brief Flush current pending EtherCAT frame stored in m_pCurrPendingSlaveFrameDesc
*
* \return status code
*/
EC_T_DWORD CEcMaster::EcatFlushCurrPendingSlaveFrameStamp(
    EC_PF_TIMESTAMP pfnTimeStamp    /**< [in]   timestamp function, if EC_NULL, no timestamping */
   ,EC_T_PVOID      pvTimeStampCtxt /**< [in]   timestamp context */
   ,EC_T_DWORD*     pdwTimeStamp    /**< [in]   Datastore for Pres-Send Timestamp (if bStampedFrame)*/
   ,EC_T_WORD       wTimestampOffset
   ,EC_T_WORD       wTimestampSize
   ,EC_T_DWORD      dwRepeatCnt
   ,EC_T_WORD       wRetryCnt
   )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (EC_NULL == m_pCurrPendingSlaveFrameDesc)
    {
        return EC_E_NOERROR; /* didn't lock! */
    }

    /* frame to flush available */
    OsLock(m_poFreeSlaveFrameDescLock);
    /* Mark frame as retry frame using MAC Address */
    ETHERNET_ADDRESS addr = m_poEcDevice->GetMacAddress();
    EC_ETHFRM_SET_RETRYMACIDX(&m_pCurrPendingSlaveFrameDesc->poEcSlaveFrame->m_EthernetFrame, addr.b, 0);
    m_pCurrPendingSlaveFrameDesc->wTimestampOffset = wTimestampOffset;
    m_pCurrPendingSlaveFrameDesc->wTimestampSize   = wTimestampSize;
    m_pCurrPendingSlaveFrameDesc->dwRepeatCnt = dwRepeatCnt;
    m_pCurrPendingSlaveFrameDesc->poEcSlaveFrame->m_retry = (EC_T_WORD)((0 == wRetryCnt)?m_MasterConfig.dwEcatCmdMaxRetries:wRetryCnt);

    /* check for registered DCM functions */
    if (EC_NULL != pfnTimeStamp )
    {
        m_pCurrPendingSlaveFrameDesc->pfnTimeStamp        = pfnTimeStamp;
        m_pCurrPendingSlaveFrameDesc->pdwTimeStamp        = pdwTimeStamp;
        m_pCurrPendingSlaveFrameDesc->pvTimeStampCtxt     = pvTimeStampCtxt;
#if (defined INCLUDE_DC_SUPPORT)
        m_pCurrPendingSlaveFrameDesc->pdwLastTSResult     = m_pDistributedClks->DcmLastTsResultPtr();
#else
        m_pCurrPendingSlaveFrameDesc->pdwLastTSResult     = 0;
#endif
    }
    /* queue and insert frame */
    if (!m_poEcDevice->DeviceQueueFrame(m_pCurrPendingSlaveFrameDesc))
    {
        EC_ERRORMSGC(("EcatFlushCurrPendingSlaveFrame(): cannot queue ethernet frame!\nConfiguration Parameter MasterConfig.dwMaxQueuedEthFrames too small?\n"));
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }
    InsertTailList(&m_PendingSlaveFrameDescList, &m_pCurrPendingSlaveFrameDesc->ListEntry);

    /* next time a new slave frame descriptor has to be taken from the free slave frame descriptor list (m_FreeSlaveFrameDescList) */
    m_pCurrPendingSlaveFrameDesc = EC_NULL;

    dwRetVal = EC_E_NOERROR;
Exit:
    OsUnlock(m_poFreeSlaveFrameDescLock);
    OsDbgAssert(dwRetVal != EC_E_ERROR);
    return dwRetVal;
}

/********************************************************************************/
/** \brief Queue a new EtherCAT command request.
*
* The ecat command is inserted into the current pending slave frame descriptor m_pCurrPendingSlaveFrameDesc.
* As long as the ecat command fits into the currently allocated Ethernet frame within this descriptor
* and m_MasterConfig.dwMaxSlaveCmdPerFrame is not reached the new command is appended after the last
* queued command.
* The pending frame (and thus all commands within this frame) is finally flushed into CEcDevice::m_PendingSlaveFrameDescList
* in EcatFlushCurrPendingSlaveFrame().
* Queued frames will be sent by CEcDevice::SendQueuedFrames()
*
* \return
*/
EC_T_DWORD CEcMaster::QueueEcatCmdReq(
     EC_T_BOOL                  bAcycCountIgnored
    ,CEcSlave*                  pSlave
    ,EC_T_DWORD                 invokeId
    ,EC_T_BYTE                  cmd
    ,EC_T_WORD                  adp
    ,EC_T_WORD                  ado
    ,EC_T_WORD                  len
    ,EC_T_VOID*                 pData
    ,EC_T_WORD                  cnt
    ,PETHERCAT_MBOX_HEADER      pMbox
    ,PETHERCAT_ETHERNET_HEADER  pEthernet
    ,PETHERCAT_CANOPEN_HEADER   pCANopen
    ,PEC_SDO_HDR       pSDO
    ,PEC_FOE_HDR       pFoE
#ifdef INCLUDE_SOE_SUPPORT
    ,PEC_SOE_HDR       pSoE
#endif
#ifdef INCLUDE_AOE_SUPPORT
    ,PEC_AOE_HDR       pAoE
    ,PEC_AOE_CMD_HDR   pAoeCmd
#endif
    )
{
    PETYPE_EC_CMD_HEADER    pEcCmdHeader        = EC_NULL;
    EC_T_BOOL               bLocked             = EC_FALSE;
    EC_T_DWORD              dwRes               = EC_E_ERROR;
    EC_T_DWORD              dwRetVal            = EC_E_ERROR;
    PECAT_SLAVEFRAME_DESC*  ppSlvFrmDesc        = EC_NULL;
    EC_T_DWORD              dwIdx = 0;
    PECAT_SLAVECMD_DESC     pSlaveDesc          = EC_NULL;
    PETYPE_EC_CMD_HEADER    pFirstEcCmdHdr      = EC_NULL;
    PETYPE_EC_CMD_HEADER    pHeader             = EC_NULL;
    ETHERNET_88A4_FRAME     *p88A4Frame         = EC_NULL;

#if !(defined INCLUDE_FOE_SUPPORT)
    EC_UNREFPARM(pFoE);
#endif

    PerfMeasStart(&G_TscMeasDesc, EC_MSMT_QueueEcatCmdReq);

    if ((pSlave != EC_NULL) && !pSlave->IsPresentOnBus())
    {
        return EC_E_SLAVE_NOT_PRESENT;
    }

    if (len > MAX_EC_DATA_LEN)
    {
        EC_ERRORMSG(("CEcMaster::QueueEcatCmdReq() - ecat command is too long!\n"));
        dwRetVal = EC_E_INVALIDSIZE;
        goto Exit;
    }

    ppSlvFrmDesc = &m_pCurrPendingSlaveFrameDesc;

    if ((invokeId == INVOKE_ID_TFER_RAW_CMD) || (invokeId >= INVOKE_ID_CLNT_TFER_Q_RAW_CMD))
    {
        /* RAW_CMD needs master state machine and slave state machine stable */
        if (!MasterStateMachIsStable() || ((pSlave != EC_NULL) && !pSlave->SlaveStateMachIsStable()))
        {
            dwRetVal = EC_E_BUSY;
            goto Exit;
        }
    }

#if (defined DEBUG)
    else
    {
        switch (invokeId)
        {
        case ECAT_INITCMD_I_P:
        case ECAT_INITCMD_I_P|ECAT_INITCMD_BEFORE:
        case ECAT_INITCMD_P_S:
        case ECAT_INITCMD_P_S|ECAT_INITCMD_BEFORE:
        case ECAT_INITCMD_P_I:
        case ECAT_INITCMD_P_I|ECAT_INITCMD_BEFORE:
        case ECAT_INITCMD_S_P:
        case ECAT_INITCMD_S_P|ECAT_INITCMD_BEFORE:
        case ECAT_INITCMD_S_O:
        case ECAT_INITCMD_S_O|ECAT_INITCMD_BEFORE:
        case ECAT_INITCMD_S_I:
        case ECAT_INITCMD_S_I|ECAT_INITCMD_BEFORE:
        case ECAT_INITCMD_O_S:
        case ECAT_INITCMD_O_S|ECAT_INITCMD_BEFORE:
        case ECAT_INITCMD_O_P:
        case ECAT_INITCMD_O_P|ECAT_INITCMD_BEFORE:
        case ECAT_INITCMD_O_I:
        case ECAT_INITCMD_O_I|ECAT_INITCMD_BEFORE:
        case ECAT_INITCMD_I_B:
        case ECAT_INITCMD_B_I:
        case eMbxInvokeId_INITCMD_RECV_FROM_SLAVE:
        case eMbxInvokeId_INITCMD_POLL_RECV_FROM_SLAVE:
        case eMbxInvokeId_INITCMD_SEND_TO_SLAVE:
            /* init commands are sent when changing master/slave states */
            break;

        case eMbxInvokeId_SEND_TO_SLAVE:
        case eMbxInvokeId_SEND_FILL_TO_SLAVE:
        case eMbxInvokeId_RECV_FROM_SLAVE:
        case eMbxInvokeId_POLL_RECV_FROM_SLAVE:
        case eMbxInvokeId_TOGGLE_REPEAT_REQ:
        case eMbxInvokeId_INITCMD_TOGGLE_REPEAT_REQ:
        case eMbxInvokeId_TOGGLE_REPEAT_RES:
        case eMbxInvokeId_INITCMD_TOGGLE_REPEAT_RES:
            break;

        case INVOKE_ID_DC_CLEARSYSTIMEVAL:
        case INVOKE_ID_DC_DISABLESYNCSIGNALS:
        case INVOKE_ID_DC_LATCHRECVTIMES:
        case INVOKE_ID_DC_READRECVTIMES:
        case INVOKE_ID_DC_REDOPENCLOSEPORT:
        case INVOKE_ID_DC_REDHANDLING:
        case INVOKE_ID_DC_DBGSYSTIMEVAL:
        case INVOKE_ID_DC_UPDATESLAVESTO:
        case INVOKE_ID_DC_READSYNCPULSEWDT:
        case INVOKE_ID_DC_ARMWBURST:
        case INVOKE_ID_DC_SETDCSTARTTIME:
        case INVOKE_ID_DC_SETDCACTIVATION:
        case INVOKE_ID_DC_SETDCCYCLETIME:
        case INVOKE_ID_DC_SETDCLATCHCONFIG:
        case INVOKE_ID_DC_CFGDCCONTROLLERS:
        case INVOKE_ID_DC_WRITESYSTIMEVAL:
        case INVOKE_ID_DC_READREFTIME:
        case INVOKE_ID_DC_WAITFORINSYNC:
        case INVOKE_ID_DC_ACYCDISTRIBUTION:
        case INVOKE_ID_DC_MAX:
            /* normal commands must not be sent when changing master/slave states */
            break;

        case INVOKE_ID_SB_COUNT:
        case INVOKE_ID_SB_GETBUSSLAVES:
        case INVOKE_ID_SB_PORTSTATUS:
        case INVOKE_ID_SB_EEPPREBUSY:
        case INVOKE_ID_SB_EEPADDR:
        case INVOKE_ID_SB_EEPBUSY:
        case INVOKE_ID_SB_EEPREAD:
        case INVOKE_ID_SB_EEPACCACQUIRE:
        case INVOKE_ID_WRITEEVENTMASK:
#if (defined INCLUDE_SLAVE_STATISTICS)
        case INVOKE_ID_GET_SLAVE_STATISTICS:
#endif
        case INVOKE_ID_SB_ACTIVATEALIAS:
        case INVOKE_ID_SB_IDENTIFYSLAVE:
        case INVOKE_ID_SB_PDICONTROL:
        case INVOKE_ID_SB_WRTTMPSLVADDR:
        case INVOKE_ID_SB_WRTSLVFIXADDR:
        case INVOKE_ID_SB_CHECKFIXEDADDRESS:
        case INVOKE_ID_SB_READALSTATUS:
        case INVOKE_ID_SB_CHECKTMPADDRESS:
        case INVOKE_ID_SB_CHECKDUPLICATE:
        case INVOKE_ID_SB_RDPORTCONTROL:
        case INVOKE_ID_SB_WRTPORTCONTROL:
        case INVOKE_ID_SB_READEVENTREGISTER:
        case INVOKE_ID_SB_READEVENTMASK:
        case INVOKE_ID_SB_WRITEEVENTMASK:
        case INVOKE_ID_SB_OPENCLOSEDPORTA:
        case INVOKE_ID_SB_EEPCHECKSUMERROR:
#if (defined INCLUDE_INCLUDE_RED_DEVICE)
        case INVOKE_ID_SB_REDHANDLING:
#endif
        case INVOKE_ID_SB_LATCHRECVTIMES:
        case INVOKE_ID_SB_READRECVTIMES:        
        case INVOKE_ID_EEPA_WRELOADCMD:
        case INVOKE_ID_EEPA_PRELOADCMPL:
        case INVOKE_ID_EEPA_RESET_R:
        case INVOKE_ID_EEPA_RESET_E:
        case INVOKE_ID_EEPA_RESET_S:
        case INVOKE_ID_EEPA_CMD:
        case INVOKE_ID_EEPA_PREADCMPL:
        case INVOKE_ID_EEPA_READVALUE:
        case INVOKE_ID_EEPA_WWRITECMDDAT:
        case INVOKE_ID_EEPA_PWRITECMPL:
        case INVOKE_ID_EEPA_ASRD:
        case INVOKE_ID_EEPA_ASWR:
        case INVOKE_ID_EEPA_ASCHECK:
        case INVOKE_ID_EEPA_ACRD:
        case INVOKE_ID_PORTOP_CHANGEPORT:
        case INVOKE_ID_PORTOP_RDDLCTRLCL:
        case INVOKE_ID_SB_CHKCLEAN:
        case INVOKE_ID_SB_ACKSLVERROR:
        case INVOKE_ID_SB_ACYCALSTATUSBRD:
            {
                /* scan bus commands and getting the slave state can be requested every time */
            } break;
        default:
            {
               OsDbgAssert( EC_FALSE );
            } break;
        }
    }
#endif /* DEBUG */

    /* QueueEcatCmdReq can be either be called by CEcMaster::OnTimer or CEcDevice::GetIoState -> CEcDevice::CheckFrame -> CEcMaster::CheckFrame */
    /* ->CEcMaster::ProcessCmdResult or CEcSlave::ProcessCmdResult*/

    EC_TRACEMSG( EC_TRACE_CORE_QUECMD_SEQ | EC_TRACE_CORE_SEQUENCE, ("<----[%06d] CEcMaster::QueueEcatCmdReq(): %s - invokeId=0x%x, cmd=%d, adp=0x%x, ado=0x%x\n",
                 m_dwMsecCounter, pSlave?pSlave->GetName():"Master", invokeId, (EC_T_DWORD)cmd, (EC_T_DWORD)adp, (EC_T_DWORD)ado));

    OsLock(m_poFreeSlaveFrameDescLock);
    bLocked = EC_TRUE;

    if (EC_NULL != *ppSlvFrmDesc)
    {   /* pending frame exists, that has not been sent yet */
        OsDbgAssert( (*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame <= m_MasterConfig.dwMaxSlaveCmdPerFrame );
        if (   ((*ppSlvFrmDesc)->poEcSlaveFrame->m_nSpace < (len + ETYPE_EC_OVERHEAD))
            || ((*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame == m_MasterConfig.dwMaxSlaveCmdPerFrame)
           )
        {
            /* either the sub telegram does not fit into the currently built pending frame or the maximum
             * number of telegrams that are allowed to be stored in the frame is reached
             * -> store current pending frame desc into pending list before
             * getting a new one from the free list
             *
             * flush pending frame into CEcDevice::m_PendingSlaveFrameDescList
             * Queued frames will be sent by CEcDevice::SendQueuedFrames
             */
            dwRes = EcatFlushCurrPendingSlaveFrame();
            m_dwDuplicateCheckCount = 0;
            if (dwRes != EC_E_NOERROR)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
#ifdef DEBUG
            if (EC_NULL != *ppSlvFrmDesc)
            {
                OsDbgAssert((*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame == 0);
            }
#endif
        }

        /* check for duplicate addresses in current frame */
        for (dwIdx = 0; dwIdx < m_dwDuplicateCheckCount; dwIdx++)
        {
            if (m_adwDuplicateCheckAddress[dwIdx] == EC_MAKEDWORD(adp, ado))
            {
                dwRes = EcatFlushCurrPendingSlaveFrame();
                m_dwDuplicateCheckCount = 0;
                break;
            }

            /* check for mbx read and mbx write in current frame */
            if (   (adp == EC_HIWORD(m_adwDuplicateCheckAddress[dwIdx]))
                && (ado >= 0x1000)
                && (EC_LOWORD(m_adwDuplicateCheckAddress[dwIdx]) >= 0x1000)
                && (m_adwDuplicateCheckCmd[dwIdx]                != cmd)         /* do not allow read and write in the same frame */
              )
            {
                dwRes = EcatFlushCurrPendingSlaveFrame();
                m_dwDuplicateCheckCount = 0;
                break;
            }
        }
    }

    if (EC_NULL == *ppSlvFrmDesc)
    {
        (*ppSlvFrmDesc) = AllocSlaveFrameDesc();

        if (EC_NULL == *ppSlvFrmDesc)
        {
            EC_ERRORMSGC(("CEcMaster::QueueEcatCmdReq() - resource error: cannot queue ecat command,\nParameter MasterConfig.dwMaxQueuedEthFrames too small!\n"));
            dwRetVal = EC_E_ACYC_FRM_FREEQ_EMPTY;
            goto Exit;
        }
        {
            ETHERNET_88A4_MAX_FRAME     *pEthFrame                      = &(*ppSlvFrmDesc)->poEcSlaveFrame->m_EthernetFrame;

            p88A4Frame                                                  = ((ETHERNET_88A4_FRAME*)(&(*ppSlvFrmDesc)->poEcSlaveFrame->m_EthernetFrame));
            p88A4Frame->EthernetFrame.Source                            = m_pMasterDesc->oMacSrc;
            p88A4Frame->EthernetFrame.Destination                       = m_pMasterDesc->oMacDest;

            pFirstEcCmdHdr                                              = &(pEthFrame->sETECAT.FirstEcCmdHeader);
            m_dwDuplicateCheckCount = 0;        /* clear num of datagrams */

#if (defined VLAN_FRAME_SUPPORT)
            if (m_bVLANEnabled)
            {
                ETYPE_VLAN_HEADER* pVlanHrd = &((TETHERNET_VLAN_FRAME*)&(pEthFrame->EthernetFrame))->VLan;

                EC_RESET_VLANHDR(pVlanHrd);
                EC_SET_VLANHDR_Type(pVlanHrd, ETHERNET_FRAME_TYPE_VLAN);
                EC_SET_VLANHDR_ID(pVlanHrd, m_wVLANId);
                EC_SET_VLANHDR_Prio(pVlanHrd, m_byVLANPrio);

                pFirstEcCmdHdr  = &(((ETHERNET_88A4_MAX_VLAN_FRAME*)pEthFrame)->sETECAT.FirstEcCmdHeader);
            }
#endif
            EC_88A4HDR_SET_E88A4FRAMELEN((*ppSlvFrmDesc)->poEcSlaveFrame->m_p88A4Header, 0);
        }

        OsDbgAssert(0 != EC_AL_88A4HDR_GET_E88A4HDRTYPE((*ppSlvFrmDesc)->poEcSlaveFrame->m_p88A4Header));

        (*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame                         = 0;

        /* calculate number of Bytes this frame has left for the EtherCAT commands */
        /* m_maxAsynFrameSize = ETHERNET_MAX_FRAME_LEN = 1514 */
        /* nSpace = 1514 - 14  - 2 = 1498 Bytes */
#if (defined VLAN_FRAME_SUPPORT)
        if (m_bVLANEnabled)
        {
            (*ppSlvFrmDesc)->poEcSlaveFrame->m_nSpace       = m_maxAsyncFrameSize - ETHERNET_VLAN_FRAME_LEN - ETYPE_88A4_HEADER_LEN;
        }
        else
#endif
        {
            (*ppSlvFrmDesc)->poEcSlaveFrame->m_nSpace           = m_maxAsyncFrameSize - ETHERNET_FRAME_LEN - ETYPE_88A4_HEADER_LEN;
        }
        (*ppSlvFrmDesc)->poEcSlaveFrame->m_pLastEcCmdHeader = EC_NULL;
    }

    OsDbgAssert((EC_NULL != ppSlvFrmDesc) && (EC_NULL !=(*ppSlvFrmDesc)));

    (*ppSlvFrmDesc)->bIgnoreFrameInAcycLimits |= bAcycCountIgnored;

    pHeader = (*ppSlvFrmDesc)->poEcSlaveFrame->m_pLastEcCmdHeader;
    if (pHeader)
    {   /* another EtherCAT cmd is added to the current command */
        EC_ICMDHDR_SET_LEN_NEXT(pHeader, EC_TRUE);
        pHeader = (*ppSlvFrmDesc)->poEcSlaveFrame->m_pLastEcCmdHeader = NEXT_EcCmdHeader(pHeader);
        OsMemset(pHeader, 0, sizeof(ETYPE_EC_CMD_HEADER));
        pHeader->uCmdIdx.swCmdIdx.byIdx  = 0;
    }
    else
    {   /*new pending frame -> set pLastHead to to the first EtherCAT-Header(directly after 88A4-Header)*/
        pHeader = (*ppSlvFrmDesc)->poEcSlaveFrame->m_pLastEcCmdHeader = pFirstEcCmdHdr;
        /*each entry ECAT_SLAVEFRAME_DESC in m_FreeSlaveFrameDescList has a different idx number(see constructor)*/
        OsMemset( pHeader, 0, sizeof(ETYPE_EC_CMD_HEADER) );
        pHeader->uCmdIdx.swCmdIdx.byIdx = (*ppSlvFrmDesc)->poEcSlaveFrame->m_byEcCmdHeaderIDX;
    }
    OsDbgAssert((*ppSlvFrmDesc)->poEcSlaveFrame->m_pLastEcCmdHeader->uCmdIdx.swCmdIdx.byIdx < m_byMinAcycCmdIdx + m_MasterConfig.dwMaxQueuedEthFrames);

    /* now populate the ecat command inside the Ethernet frame */
    pHeader->uCmdIdx.swCmdIdx.byCmd = cmd;
    EC_ICMDHDR_SET_ADDR_ADP(pHeader, adp);
    EC_ICMDHDR_SET_ADDR_ADO(pHeader, ado);
    EC_ICMDHDR_SET_LEN_LEN(pHeader, len); /* res and next = 0; */

    /* save address info for duplicate check */
    m_adwDuplicateCheckAddress[m_dwDuplicateCheckCount] = EC_MAKEDWORD(adp, ado);
    m_adwDuplicateCheckCmd[m_dwDuplicateCheckCount] = cmd;
    m_dwDuplicateCheckCount++;

    pEcCmdHeader = pHeader;
    EC_TRACEMSG( EC_TRACE_CREATE_FRAMES,
                ("CEcMaster::QueueEcatCmdReq(): %s - Insert CmdIdx 0x%x (adr 0x%x, len 0x%x) into frame\n",
                 pSlave?pSlave->GetName():"Master",
                 EC_ICMDHDR_GET_CMDIDX(pEcCmdHeader),
                 EC_ICMDHDR_GET_ADDR(pEcCmdHeader),
                 EC_ICMDHDR_GET_LEN(pEcCmdHeader)
                )
               );

    if (len > 0)
    {
        if (pMbox)
        {
            EC_T_BYTE* pByte    = (EC_T_BYTE*)EC_ENDOF(pHeader);
            EC_T_UINT bytesLeft = len;
            EC_T_UINT bytesData = EC_ECMBOXHDR_GET_LENGTH(pMbox);

            /* copy Mailbox header to the end of the EtherCAT command */
            OsMemcpy(pByte, pMbox, ETHERCAT_MBOX_HEADER_LEN);
            pByte     += ETHERCAT_MBOX_HEADER_LEN;
            bytesLeft -= ETHERCAT_MBOX_HEADER_LEN;
            if (pEthernet)
            {
                /* copy EoE header to the end of the EtherCAT command */
                OsMemcpy(pByte, pEthernet, ETHERCAT_ETHERNET_HEADER_LEN);
                pByte     += ETHERCAT_ETHERNET_HEADER_LEN;
                bytesLeft -= ETHERCAT_ETHERNET_HEADER_LEN;
                bytesData -= ETHERCAT_ETHERNET_HEADER_LEN;
            }
            else if (pCANopen)
            {
                /* copy CoE header to the end of the EtherCAT command */
                OsMemcpy(pByte, pCANopen, ETHERCAT_CANOPEN_HEADER_LEN);
                pByte       += ETHERCAT_CANOPEN_HEADER_LEN;
                bytesLeft   -= ETHERCAT_CANOPEN_HEADER_LEN;
                bytesData   -= ETHERCAT_CANOPEN_HEADER_LEN;
                if (pSDO)
                {
                    /* copy Sdo header to the end of the EtherCAT command */
                    OsMemcpy(pByte, pSDO, EC_SDO_HDR_LEN);
                    pByte       += EC_SDO_HDR_LEN;
                    bytesLeft   -= EC_SDO_HDR_LEN;
                    bytesData   -= EC_SDO_HDR_LEN;
                }
            }
#ifdef INCLUDE_FOE_SUPPORT
            else if (pFoE)
            {
                /* copy FoE header to the end of the EtherCAT command */
                OsMemcpy(pByte, pFoE, EC_FOE_HDR_LEN);
                pByte     += EC_FOE_HDR_LEN;
                bytesLeft -= EC_FOE_HDR_LEN;
                bytesData -= EC_FOE_HDR_LEN;
            }
#endif
#ifdef INCLUDE_SOE_SUPPORT
            else if (pSoE)
            {
                /* copy SoE header to the end of the EtherCAT command */
                OsMemcpy(pByte, pSoE, EC_SOE_HDR_LEN);
                pByte     += EC_SOE_HDR_LEN;
                bytesLeft -= EC_SOE_HDR_LEN;
                bytesData -= EC_SOE_HDR_LEN;
            }
#endif
#ifdef INCLUDE_AOE_SUPPORT
            else if (pAoE)
            {
                /* copy AoE header to the end of the EtherCAT command */
                OsMemcpy(pByte, pAoE, EC_AOE_HDR_LEN);
                pByte     += EC_AOE_HDR_LEN;
                bytesLeft -= EC_AOE_HDR_LEN;
                bytesData -= EC_AOE_HDR_LEN;
                if (pAoeCmd)
                {
                    /* copy Aoe command to the end of the AoE header */
                    OsMemcpy(pByte, pAoeCmd, pAoeCmd->dwUsedLength);
                    pByte       += pAoeCmd->dwUsedLength;
                    bytesLeft   -= pAoeCmd->dwUsedLength;
                    bytesData   -= pAoeCmd->dwUsedLength;
                }
            }
#endif
            /* copy data to the end of the EtherCAT command */
            if (pData && (bytesData > 0) && (bytesLeft > 0))
            {
                if (bytesData > bytesLeft)
                {
                    bytesData = bytesLeft;
                }
                OsMemcpy(pByte, pData, bytesData);
                pByte     += bytesData;
                bytesLeft -= bytesData;
            }
            if (bytesLeft > 0)
            {
                OsMemset(pByte, 0, bytesLeft);
            }
        }
        else
        {
            /* copy data to the end of the EtherCAT command */
            if (pData)
            {
                OsMemcpy(EC_ENDOF(pHeader), pData, len);
            }
            else
            {
                OsMemset(EC_ENDOF(pHeader), 0, len);
            }
        }
    }
    /*set working counter of command (normally cnt = 0 )*/

    pSlaveDesc = &(*ppSlvFrmDesc)->poEcSlaveFrame->m_aEcCmdDesc[(*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame];

    ETYPE_EC_CMD_SETWKC(pHeader, cnt);
    pSlaveDesc->pSlave   = pSlave;
    pSlaveDesc->invokeId = invokeId;
#ifdef REMOVE_MBX_READ_RETRIES
    pSlaveDesc->pEcCmdHeader = pEcCmdHeader;
#endif
    /* store additional frame info to be compared later! */
    pSlaveDesc->cmd = cmd;
    pSlaveDesc->byIdx = pHeader->uCmdIdx.swCmdIdx.byIdx ;
    pSlaveDesc->ado = ado;
    pSlaveDesc->len = len;

    (*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame++;
    OsDbgAssert( (*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame <= m_MasterConfig.dwMaxSlaveCmdPerFrame );
    (*ppSlvFrmDesc)->poEcSlaveFrame->m_nSpace -= len + ETYPE_EC_OVERHEAD;

    {
        EC_T_WORD   wLen    = 0;

        wLen    = (EC_T_WORD)EC_88A4HDR_GET_E88A4FRAMELEN((*ppSlvFrmDesc)->poEcSlaveFrame->m_p88A4Header);
        wLen    = (EC_T_WORD)(wLen + len );
        wLen    = (EC_T_WORD)(wLen + ETYPE_EC_OVERHEAD );

        EC_88A4HDR_SET_E88A4FRAMELEN((*ppSlvFrmDesc)->poEcSlaveFrame->m_p88A4Header, wLen);
    }

    EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMaster::QueueEcatCmdReq(): CmdIdx = 0x%x, invokeId = 0x%x\n", (EC_T_DWORD)EC_ICMDHDR_GET_CMDIDX((*ppSlvFrmDesc)->poEcSlaveFrame->m_pLastEcCmdHeader),invokeId));

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (bLocked)
    {
        OsUnlock(m_poFreeSlaveFrameDescLock);
        bLocked = EC_FALSE;
    }

    if (EC_NULL != pEcCmdHeader)
    {
        OsDbgAssert(ETYPE_EC_CMD_GETWKC(pEcCmdHeader) == 0);
    }

    PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_QueueEcatCmdReq);

    return dwRetVal;
}

/********************************************************************************/
/** \brief Queue a new EtherCAT command request to read/write registers (no mailbox)
*
* \return
*/
EC_T_DWORD CEcMaster::QueueRegisterCmdReq(
     CEcSlave*                  pSlave
    ,EC_T_DWORD                 invokeId
    ,EC_T_BYTE                  cmd
    ,EC_T_WORD                  adp
    ,EC_T_WORD                  ado
    ,EC_T_WORD                  len
    ,EC_T_VOID*                 pData
    )
{
    PETYPE_EC_CMD_HEADER    pEcCmdHeader        = EC_NULL;
    EC_T_BOOL               bLocked             = EC_FALSE;
    EC_T_DWORD              dwRes               = EC_E_ERROR;
    EC_T_DWORD              dwRetVal            = EC_E_ERROR;
    PECAT_SLAVEFRAME_DESC*  ppSlvFrmDesc        = EC_NULL;
    PECAT_SLAVECMD_DESC     pSlaveDesc          = EC_NULL;
    PETYPE_EC_CMD_HEADER    pFirstEcCmdHdr      = EC_NULL;
    PETYPE_EC_CMD_HEADER    pHeader             = EC_NULL;
    ETHERNET_88A4_FRAME     *p88A4Frame         = EC_NULL;

    PerfMeasStart(&G_TscMeasDesc, EC_MSMT_QueueRegisterCmdReq);

    if ((pSlave != EC_NULL) && !pSlave->IsPresentOnBus())
    {
        return EC_E_SLAVE_NOT_PRESENT;
    }

    if (len > MAX_EC_DATA_LEN)
    {
        EC_ERRORMSG(("CEcMaster::QueueEcatCmdReq() - ecat command is too long!\n"));
        dwRetVal = EC_E_INVALIDSIZE;
        goto Exit;
    }

    ppSlvFrmDesc = &m_pCurrPendingSlaveFrameDesc;

    OsLock(m_poFreeSlaveFrameDescLock);
    bLocked = EC_TRUE;

    if ( EC_NULL != (*ppSlvFrmDesc) )
    {   /* pending frame exists, that has not been sent yet */
        OsDbgAssert( (*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame <= m_MasterConfig.dwMaxSlaveCmdPerFrame );
        if (   ((*ppSlvFrmDesc)->poEcSlaveFrame->m_nSpace < (len + ETYPE_EC_OVERHEAD))
            || ((*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame == m_MasterConfig.dwMaxSlaveCmdPerFrame)
           )
        {
            /* either the sub telegram does not fit into the currently built pending frame or the maximum
             * number of telegrams that are allowed to be stored in the frame is reached
             * -> store current pending frame desc into pending list before
             * getting a new one from the free list
             *
             * flush pending frame into CEcDevice::m_PendingSlaveFrameDescList
             * Queued frames will be sent by CEcDevice::SendQueuedFrames
             */
            dwRes = EcatFlushCurrPendingSlaveFrame();
            m_dwDuplicateCheckCount = 0;
            if (dwRes != EC_E_NOERROR)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
#ifdef DEBUG
            if (EC_NULL != *ppSlvFrmDesc)
            {
                OsDbgAssert((*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame == 0);
            }
#endif
        }
    }

    if (EC_NULL == *ppSlvFrmDesc)
    {
        /* get new current frame descriptor from free list. */
        (*ppSlvFrmDesc) = AllocSlaveFrameDesc();

        if (EC_NULL == *ppSlvFrmDesc)
        {
            EC_ERRORMSGC(("CEcMaster::QueueEcatCmdReq() - resource error: cannot queue ecat command,\nParameter MasterConfig.dwMaxQueuedEthFrames too small!\n"));
            dwRetVal = EC_E_ACYC_FRM_FREEQ_EMPTY;
            goto Exit;
        }
        {
            ETHERNET_88A4_MAX_FRAME     *pEthFrame                      = &(*ppSlvFrmDesc)->poEcSlaveFrame->m_EthernetFrame;

            p88A4Frame                                                  = ((ETHERNET_88A4_FRAME*)(&(*ppSlvFrmDesc)->poEcSlaveFrame->m_EthernetFrame));
            p88A4Frame->EthernetFrame.Source                            = m_pMasterDesc->oMacSrc;
            p88A4Frame->EthernetFrame.Destination                       = m_pMasterDesc->oMacDest;

            pFirstEcCmdHdr                                              = &(pEthFrame->sETECAT.FirstEcCmdHeader);
            m_dwDuplicateCheckCount = 0;        /* clear num of datagrams */

#if (defined VLAN_FRAME_SUPPORT)
            if (m_bVLANEnabled)
            {
                ETYPE_VLAN_HEADER* pVlanHrd = &((TETHERNET_VLAN_FRAME*)&(pEthFrame->EthernetFrame))->VLan;

                EC_RESET_VLANHDR(pVlanHrd);
                EC_SET_VLANHDR_Type(pVlanHrd, ETHERNET_FRAME_TYPE_VLAN);
                EC_SET_VLANHDR_ID(pVlanHrd, m_wVLANId);
                EC_SET_VLANHDR_Prio(pVlanHrd, m_byVLANPrio);

                pFirstEcCmdHdr  = &(((ETHERNET_88A4_MAX_VLAN_FRAME*)pEthFrame)->sETECAT.FirstEcCmdHeader);
            }
#endif
            EC_88A4HDR_SET_E88A4FRAMELEN((*ppSlvFrmDesc)->poEcSlaveFrame->m_p88A4Header, 0);
        }

        OsDbgAssert(0 != EC_AL_88A4HDR_GET_E88A4HDRTYPE((*ppSlvFrmDesc)->poEcSlaveFrame->m_p88A4Header));

        (*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame                         = 0;

        /* calculate number of Bytes this frame has left for the EtherCAT commands */
        /* m_maxAsynFrameSize = ETHERNET_MAX_FRAME_LEN = 1514 */
        /* nSpace = 1514 - 14  - 2 = 1498 Bytes */
#if (defined VLAN_FRAME_SUPPORT)
        if (m_bVLANEnabled)
        {
            (*ppSlvFrmDesc)->poEcSlaveFrame->m_nSpace       = m_maxAsyncFrameSize - ETHERNET_VLAN_FRAME_LEN - ETYPE_88A4_HEADER_LEN;
        }
        else
#endif
        {
            (*ppSlvFrmDesc)->poEcSlaveFrame->m_nSpace           = m_maxAsyncFrameSize - ETHERNET_FRAME_LEN - ETYPE_88A4_HEADER_LEN;
        }
        (*ppSlvFrmDesc)->poEcSlaveFrame->m_pLastEcCmdHeader = EC_NULL;
    }

    OsDbgAssert((EC_NULL != ppSlvFrmDesc) && (EC_NULL !=(*ppSlvFrmDesc)));

    pHeader = (*ppSlvFrmDesc)->poEcSlaveFrame->m_pLastEcCmdHeader;
    if (pHeader)
    {   /* another EtherCAT cmd is added to the current command */
        EC_ICMDHDR_SET_LEN_NEXT(pHeader, EC_TRUE);
        pHeader = (*ppSlvFrmDesc)->poEcSlaveFrame->m_pLastEcCmdHeader = NEXT_EcCmdHeader(pHeader);
        OsMemset(pHeader, 0, sizeof(ETYPE_EC_CMD_HEADER));
        pHeader->uCmdIdx.swCmdIdx.byIdx  = 0;
    }
    else
    {   /*new pending frame -> set pLastHead to to the first EtherCAT-Header(directly after 88A4-Header)*/
        pHeader = (*ppSlvFrmDesc)->poEcSlaveFrame->m_pLastEcCmdHeader = pFirstEcCmdHdr;
        /*each entry ECAT_SLAVEFRAME_DESC in m_FreeSlaveFrameDescList has a different idx number(see constructor)*/
        OsMemset( pHeader, 0, sizeof(ETYPE_EC_CMD_HEADER) );
        pHeader->uCmdIdx.swCmdIdx.byIdx = (*ppSlvFrmDesc)->poEcSlaveFrame->m_byEcCmdHeaderIDX;
    }
    OsDbgAssert((*ppSlvFrmDesc)->poEcSlaveFrame->m_pLastEcCmdHeader->uCmdIdx.swCmdIdx.byIdx < m_byMinAcycCmdIdx + m_MasterConfig.dwMaxQueuedEthFrames);

    /* now populate the ecat command inside the Ethernet frame */
    pHeader->uCmdIdx.swCmdIdx.byCmd = cmd;
    EC_ICMDHDR_SET_ADDR_ADP(pHeader, adp);
    EC_ICMDHDR_SET_ADDR_ADO(pHeader, ado);
    EC_ICMDHDR_SET_LEN_LEN(pHeader, len); /* res and next = 0; */

    pEcCmdHeader = pHeader;
    if (len > 0)
    {
        /* copy data to the end of the EtherCAT command */
        if (pData)
        {
            OsMemcpy(EC_ENDOF(pHeader), pData, len);
        }
        else
        {
            OsMemset(EC_ENDOF(pHeader), 0, len);
        }
    }
    pSlaveDesc = &(*ppSlvFrmDesc)->poEcSlaveFrame->m_aEcCmdDesc[(*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame];

    ETYPE_EC_CMD_SETWKC(pHeader, 0);
    pSlaveDesc->pSlave   = pSlave;
    pSlaveDesc->invokeId = invokeId;
#ifdef REMOVE_MBX_READ_RETRIES
    pSlaveDesc->pEcCmdHeader = pEcCmdHeader;
#endif
    /* store additional frame info to be compared later! */
    pSlaveDesc->cmd = cmd;
    pSlaveDesc->byIdx = pHeader->uCmdIdx.swCmdIdx.byIdx ;
    pSlaveDesc->ado = ado;
    pSlaveDesc->len = len;

    (*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame++;
    OsDbgAssert((*ppSlvFrmDesc)->poEcSlaveFrame->m_nNumCmdsInFrame <= m_MasterConfig.dwMaxSlaveCmdPerFrame);
    (*ppSlvFrmDesc)->poEcSlaveFrame->m_nSpace -= len + ETYPE_EC_OVERHEAD;

    {
        EC_T_WORD wLen = 0;
        wLen = (EC_T_WORD)EC_88A4HDR_GET_E88A4FRAMELEN((*ppSlvFrmDesc)->poEcSlaveFrame->m_p88A4Header);
        wLen = (EC_T_WORD)(wLen + len);
        wLen = (EC_T_WORD)(wLen + ETYPE_EC_OVERHEAD);

        EC_88A4HDR_SET_E88A4FRAMELEN((*ppSlvFrmDesc)->poEcSlaveFrame->m_p88A4Header, wLen);
    }

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (bLocked)
    {
        OsUnlock(m_poFreeSlaveFrameDescLock);
        bLocked = EC_FALSE;
    }

    if (EC_NULL != pEcCmdHeader )
    {
        OsDbgAssert(ETYPE_EC_CMD_GETWKC(pEcCmdHeader) == 0);
    }

    PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_QueueRegisterCmdReq);
    return dwRetVal;
}


/********************************************************************************/
/** \brief Check if enough free space inside the frame is available
*
* If not enough space is available, begin new frame
*
* \return
*/
EC_T_DWORD CEcMaster::PrepareFreeFrameSpace(EC_T_WORD wMinFrameSpace)
{
    EC_T_BOOL               bLocked             = EC_FALSE;
    EC_T_DWORD              dwRes               = EC_E_ERROR;
    EC_T_DWORD              dwRetVal            = EC_E_ERROR;

    if (wMinFrameSpace > MAX_EC_DATA_LEN)
    {
        EC_ERRORMSG(("CEcMaster::PrepareFreeFrameSpace() - ecat command is too long!\n"));
        dwRetVal = EC_E_INVALIDSIZE;
        goto Exit;
    }

    OsLock(m_poFreeSlaveFrameDescLock);
    bLocked = EC_TRUE;

    if (EC_NULL != m_pCurrPendingSlaveFrameDesc)
    {   /* pending frame exists, that has not been sent yet */
        OsDbgAssert( m_pCurrPendingSlaveFrameDesc->poEcSlaveFrame->m_nNumCmdsInFrame <= m_MasterConfig.dwMaxSlaveCmdPerFrame );
        if (   (m_pCurrPendingSlaveFrameDesc->poEcSlaveFrame->m_nSpace < wMinFrameSpace)
            || (m_pCurrPendingSlaveFrameDesc->poEcSlaveFrame->m_nNumCmdsInFrame == m_MasterConfig.dwMaxSlaveCmdPerFrame)
           )
        {
            /* either the sub telegram does not fit into the currently built pending frame or the maximum
             * number of telegrams that are allowed to be stored in the frame is reached
             * -> store current pending frame desc into pending list before
             * getting a new one from the free list
             *
             * flush pending frame into CEcDevice::m_PendingSlaveFrameDescList
             * Queued frames will be sent by CEcDevice::SendQueuedFrames
             */
            dwRes = EcatFlushCurrPendingSlaveFrame();
            m_dwDuplicateCheckCount = 0;
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if (bLocked)
    {
        OsUnlock(m_poFreeSlaveFrameDescLock);
        bLocked = EC_FALSE;
    }

    return dwRetVal;
}



/*-------------------------------------------------------------------------------*/

EC_T_DWORD CEcMaster::Notify(EC_T_DWORD dwNotificationCode,
                             EC_T_DWORD dwClientId,
                             EC_T_NOTIFICATION_DATA* pNotificationData,
                             EC_T_DWORD dwNotificationSize /* = SIZEOF_EC_T_NOTIFICATION_DATA */
                             )
{
    EC_T_DWORD          dwNumNotify = 0;
    EC_T_NOTIFYPARMS    NotifyParms;

    PerfMeasStart(&G_TscMeasDesc, EC_MSMT_NotifyAndFree);
    OsMemset(&NotifyParms, 0, sizeof(EC_T_NOTIFYPARMS));

#if (defined INCLUDE_MASTER_OBD)
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->AddHistoryNotification(dwNotificationCode, pNotificationData);
    }
#endif

    if (m_pfNotifyCallback != EC_NULL )
    {
        /* populate notify structure */
        NotifyParms.pbyInBuf       = (EC_T_BYTE*)pNotificationData;
        NotifyParms.dwInBufSize   = dwNotificationSize;
        NotifyParms.pbyOutBuf     = EC_NULL;
        NotifyParms.dwOutBufSize  = 0;
        NotifyParms.pdwNumOutData = EC_NULL;
        PerfMeasStart(&G_TscMeasDesc, EC_MSMT_NotifyCallback);
        dwNumNotify = m_pfNotifyCallback(m_MasterConfigEx.dwInstanceID, dwClientId, dwNotificationCode, &NotifyParms);
        PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_NotifyCallback);
    }

    return dwNumNotify;
}

/***************************************************************************************************/
/**
\brief  Notify Application.

This call notifies the Application and releases the previous allocated Notification Descriptor.
*/
EC_T_DWORD CEcMaster::NotifyAndFree
(   EC_T_DWORD  dwNotificationCode,
    EC_T_NOTIFICATION_INTDESC*  pNotificationDesc,
    EC_T_DWORD dwNotificationSize
)
{
    EC_T_DWORD dwNumNotify = 0;

    dwNumNotify = Notify(dwNotificationCode, pNotificationDesc->dwClientID, &pNotificationDesc->uNotification, dwNotificationSize);

    /* free alloc'ed notification descriptor */
    OsDbgAssert(m_pNotificationStore[pNotificationDesc->dwIdx].bUsed == EC_TRUE);
    FreeNotification(pNotificationDesc);
    PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_NotifyAndFree);
    return dwNumNotify;
}

/***************************************************************************************************/
/**
\brief  Acquire a notification descriptor.

\return Pointer to Notification descriptor, EC_NULL in case of error.
*/
EC_T_NOTIFICATION_INTDESC* CEcMaster::AllocNotification(EC_T_VOID)
{
    EC_T_NOTIFICATION_INTDESC*  pRet    = EC_NULL;
    EC_T_DWORD                  dwIdx   = 0;

    if (EC_NULL == m_pNotificationStore)
    {
        pRet = EC_NULL;
        goto Exit;
    }

    for (dwIdx = 0; dwIdx < EC_NOTIFICATION_BUFFERS; dwIdx++)
    {
        if (!m_pNotificationStore[dwIdx].bUsed)
        {
            pRet = &(m_pNotificationStore[dwIdx]);
            pRet->bUsed = EC_TRUE;
            pRet->dwClientID = 0;
            OsMemset(&(pRet->uNotification), 0, SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER);
            break;
        }
    }

Exit:
    return pRet;
}

/***************************************************************************************************/
/**
\brief  Get Serialized Configuration Entries.

This function returns serialized feature entries for re-spawn of master with equal entries
(features set by IOCTL calls between ecatInitMaster and ecatConfigureMaster).
Result returned is input for new Instance SetFeatureField method.

\return EC_E_NOERROR on success or error code otherwise.
*/
EC_T_INTFEATUREFIELD_DESC* CEcMaster::GetFeatureField(EC_T_VOID)
{
    EC_T_INTFEATUREFIELD_DESC*  pRetVal     = EC_NULL;
    EC_T_INTFEATUREFIELD_DESC*  pFeatures   = EC_NULL;
    EC_T_DWORD                  dwRes       = EC_E_ERROR;
#if (defined INCLUDE_MASTER_OBD)
    EC_T_WORD                   wBitLength  = 0;
#endif

    pFeatures = (EC_PT_INTFEATUREFIELD_DESC)OsMalloc(sizeof(EC_T_INTFEATUREFIELD_DESC));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcMaster:GetFeatureField", pFeatures, sizeof(EC_T_INTFEATUREFIELD_DESC));
    if (EC_NULL == pFeatures)
    {
        dwRes = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pFeatures, 0, sizeof(EC_T_INTFEATUREFIELD_DESC));
#if (defined INCLUDE_OEM)
    pFeatures->qwOemKey                     = m_qwOemKey;
#endif /* INCLUDE_OEM */
    pFeatures->dwSupportedFeatures          = m_dwSupportedFeatures;
    pFeatures->dwHCDetectionTimeout         = m_dwHCDetectionTimeout;
    pFeatures->dwSBTimeout                  = m_dwSBTimeout;
    pFeatures->dwStateChangeDefaultTimeout  = m_dwStateChangeDefaultTimeout;
    pFeatures->bSlaveAliasAddressing        = m_pBusTopology->IsAliasAddressingEnabled();
    OsMemcpy(pFeatures->m_abReadEEPROMEntry, m_pBusTopology->m_abReadEEPROMEntry, READEEPROMENTRY_COUNT*sizeof(EC_T_BOOL));
    OsMemcpy(pFeatures->m_abCheckEEPROMSetByIoctl, m_pBusTopology->m_abCheckEEPROMSetByIoctl, CHECKEEPROMENTRY_COUNT*sizeof(EC_T_BOOL));
    OsMemcpy(pFeatures->m_aeCheckEEPROMEntry, m_pBusTopology->m_aeCheckEEPROMEntry, CHECKEEPROMENTRY_COUNT*sizeof(EC_T_CHECKEEPROMENTRY));
#if (defined VLAN_FRAME_SUPPORT)
    pFeatures->bVLANEnabled                 = m_bVLANEnabled;
    pFeatures->wVLANId                      = m_wVLANId;
    pFeatures->byVLANPrio                   = m_byVLANPrio;
#endif
    pFeatures->bTopologyChangeAutoMode      = m_pBusTopology->GetTopologyChangeAutoMode();
    pFeatures->dwTopologyChangeDelay        = m_pBusTopology->GetTopologyChangeDelay();
#if (defined INCLUDE_HOTCONNECT)
    pFeatures->oHCMode                      = GetHCMode();
#endif
    pFeatures->bNotifyUnexpectedBusSlaves   = m_pBusTopology->GetNotifyUnexpectedBusSlaves();
    pFeatures->dwInitCmdRetryTimeout        = m_dwInitCmdRetryTimeout;
    pFeatures->dwMbxCmdTimeout              = m_dwMbxCmdTimeout;
    pFeatures->dwDefaultMbxPolling          = m_dwDefaultMbxPolling;
    pFeatures->bPdOffsetCompMode            = m_bPdOffsetCompMode;
#if (defined INCLUDE_DC_SUPPORT)
    pFeatures->bAddBrdSyncWindowMonitoring  = m_bAddBrdSyncWindowMonitoring;
    pFeatures->bDcDefault                   = EC_FALSE;
    m_pDistributedClks->GetConfig(&pFeatures->DcConfig);
    m_pDistributedClks->DcmGetConfig(&pFeatures->DcmConfig);
    pFeatures->dwDcmInSyncTimeout           = m_dwDcmInSyncTimeout;
    pFeatures->bFirstDcSlaveAsRefClock      = m_pDistributedClks->m_bFirstDcSlaveAsRefClock;
#endif
    pFeatures->bAllSlavesMustReachState     = m_bAllSlavesMustReachState;
    pFeatures->dwNotificationMask           = m_dwNotificationMask;
    pFeatures->dwCycErrorNotifyMask         = m_dwCycErrorNotifyMask;
    pFeatures->qwLk                         = m_qwLk;

#if (defined INCLUDE_MASTER_OBD)
    dwRes = m_pSDOService->Read(COEOBJID_IDENTITY_OBJECT, 1, sizeof(EC_T_DWORD), EC_NULL, (EC_T_WORD*)(EC_T_VOID*)&pFeatures->dwVendorID,     EC_FALSE);
    OsDbgAssert(EC_E_NOERROR == dwRes);
    dwRes = m_pSDOService->Read(COEOBJID_IDENTITY_OBJECT, 2, sizeof(EC_T_DWORD), EC_NULL, (EC_T_WORD*)(EC_T_VOID*)&pFeatures->dwProductcode,  EC_FALSE);
    OsDbgAssert(EC_E_NOERROR == dwRes);
    dwRes = m_pSDOService->Read(COEOBJID_IDENTITY_OBJECT, 3, sizeof(EC_T_DWORD), EC_NULL, (EC_T_WORD*)(EC_T_VOID*)&pFeatures->dwRevision,     EC_FALSE);
    OsDbgAssert(EC_E_NOERROR == dwRes);
    dwRes = m_pSDOService->Read(COEOBJID_IDENTITY_OBJECT, 4, sizeof(EC_T_DWORD), EC_NULL, (EC_T_WORD*)(EC_T_VOID*)&pFeatures->dwSerialnumber, EC_FALSE);
    OsDbgAssert(EC_E_NOERROR == dwRes);
    dwRes = m_pSDOService->GetEntryBitLength(COEOBJID_MANF_DEVICE_NAME, 0, &wBitLength);
    OsDbgAssert(EC_E_NOERROR == dwRes);
    if (wBitLength > 0)
    {
        pFeatures->wDevicenameSize = (EC_T_WORD)((wBitLength + 7) / 8 + 1);
        pFeatures->szDevicename = (EC_T_CHAR*)OsMalloc(pFeatures->wDevicenameSize);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "pFeatures->szDevicename", pFeatures->szDevicename, pFeatures->wDevicenameSize);
        if (EC_NULL == pFeatures->szDevicename)
        {
            dwRes = EC_E_NOMEMORY;
            goto Exit;
        }
        dwRes = m_pSDOService->Read(COEOBJID_MANF_DEVICE_NAME, 0, pFeatures->wDevicenameSize, EC_NULL, (EC_T_WORD*)(EC_T_VOID*)pFeatures->szDevicename, EC_FALSE);
        OsDbgAssert(EC_E_NOERROR == dwRes);
        pFeatures->szDevicename[pFeatures->wDevicenameSize - 1] =  '\0';
    }
    dwRes = m_pSDOService->GetEntryBitLength(COEOBJID_MANF_HW_VERSION, 0, &wBitLength);
    OsDbgAssert(EC_E_NOERROR == dwRes);
    if (wBitLength > 0)
    {
        pFeatures->wHardwareversionSize = (EC_T_WORD)((wBitLength + 7) / 8 + 1);
        pFeatures->szHardwareversion = (EC_T_CHAR*)OsMalloc(pFeatures->wHardwareversionSize);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "pFeatures->szHardwareversion", pFeatures->szHardwareversion, pFeatures->wHardwareversionSize);
        if (EC_NULL == pFeatures->szHardwareversion)
        {
            dwRes = EC_E_NOMEMORY;
            goto Exit;
        }
        dwRes = m_pSDOService->Read(COEOBJID_MANF_HW_VERSION, 0, pFeatures->wHardwareversionSize, EC_NULL, (EC_T_WORD*)(EC_T_VOID*)pFeatures->szHardwareversion, EC_FALSE);
        OsDbgAssert(EC_E_NOERROR == dwRes);
        pFeatures->szHardwareversion[pFeatures->wHardwareversionSize - 1] = '\0';
    }
    dwRes = m_pSDOService->GetEntryBitLength(COEOBJID_MANF_SW_VERSION, 0, &wBitLength);
    OsDbgAssert(EC_E_NOERROR == dwRes);
    if (wBitLength > 0)
    {
        pFeatures->wSoftwareversionSize = (EC_T_WORD)((wBitLength + 7) / 8 + 1);
        pFeatures->szSoftwareversion = (EC_T_CHAR*)OsMalloc(pFeatures->wSoftwareversionSize);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "pFeatures->szSoftwareversion", pFeatures->szSoftwareversion, pFeatures->wSoftwareversionSize);
        if (EC_NULL == pFeatures->szSoftwareversion)
        {
            dwRes = EC_E_NOMEMORY;
            goto Exit;
        }
        dwRes = m_pSDOService->Read(COEOBJID_MANF_SW_VERSION, 0, pFeatures->wSoftwareversionSize, EC_NULL, (EC_T_WORD*)(EC_T_VOID*)pFeatures->szSoftwareversion, EC_FALSE);
        OsDbgAssert(EC_E_NOERROR == dwRes);
        pFeatures->szSoftwareversion[pFeatures->wSoftwareversionSize - 1] = '\0';
    }
#endif /* INCLUDE_MASTER_OBD */

    pFeatures->eCycFrameLayout = m_eCycFrameLayout;
    pFeatures->bCopyInfoInSendCycFrames = m_bCopyInfoInSendCycFrames;

#if (defined INCLUDE_TRACE_DATA)
    pFeatures->wTraceDataSize = m_wTraceDataSize;
#endif
#if (defined INCLUDE_VARREAD)
    pFeatures->bAddVarsForSpecificDataTypes = m_bAddVarsForSpecificDataTypes;
#endif
#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
    pFeatures->bRedEnhancedLineCrossedDetectionEnabled = m_pBusTopology->GetRedEnhancedLineCrossedDetectionEnabled();
#endif
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    pFeatures->bErrorOnLineCrossed = m_pBusTopology->GetErrorOnLineCrossed();
    pFeatures->bNotifyNotConnectedPortA = m_pBusTopology->GetNotifyNotConnectedPortA();
    pFeatures->bNotifyUnexpectedConnectedPort = m_pBusTopology->GetNotifyUnexpectedConnectedPort();
#endif
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    pFeatures->bJunctionRedundancyEnabled = m_pBusTopology->GetJunctionRedundancyEnabled();
#endif

    pFeatures->bGenEniAssignEepromBackToEcat = m_bGenEniAssignEepromBackToEcat;
    pFeatures->bAutoAckAlStatusError = m_bAutoAckAlStatusError;
    pFeatures->bAutoAdjustCycCmdWkc = m_bAutoAdjustCycCmdWkc;
    pFeatures->bAdjustCycFramesAfterslavesStateChange = m_bAdjustCycFramesAfterslavesStateChange;

    /* no errors */
    pRetVal = pFeatures;

Exit:
    if (EC_E_NOMEMORY == dwRes)
    {
        CEcMaster::FreeFeatureField(pFeatures, GetMemLog());
    }

    return pRetVal;
}

EC_T_VOID CEcMaster::FreeFeatureField(EC_T_INTFEATUREFIELD_DESC* pFeatures, struct _EC_T_MEMORY_LOGGER* pMLog)
{
    if (pFeatures)
    {
#if (defined INCLUDE_MASTER_OBD)
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "FreeFeatureField:pFeatures->szDevicename", pFeatures->szDevicename, pFeatures->wDevicenameSize);
        SafeOsFree(pFeatures->szDevicename);
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "FreeFeatureField:pFeatures->szHardwareversion", pFeatures->szHardwareversion, pFeatures->wHardwareversionSize);
        SafeOsFree(pFeatures->szHardwareversion);
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "FreeFeatureField:pFeatures->szSoftwareversion", pFeatures->szSoftwareversion, pFeatures->wSoftwareversionSize);
        SafeOsFree(pFeatures->szSoftwareversion);
#endif
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_INTERFACE, pMLog, "InitMasterEx:pFeatures", pFeatures, sizeof(EC_T_INTFEATUREFIELD_DESC));
        SafeOsFree(pFeatures);
    }
}

/***************************************************************************************************/
/**
\brief  Create Empty Feature Field.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_INTFEATUREFIELD_DESC* CEcMaster::CreateFeatureField(
    EC_T_DWORD      dwSupportedFeatures /**< [out] Supported features */
  , struct _EC_T_MEMORY_LOGGER* pMLog                                      )
{
    EC_T_INTFEATUREFIELD_DESC*  pRetVal     = EC_NULL;
    EC_T_INTFEATUREFIELD_DESC*  pFeatures   = EC_NULL;
    EC_T_DWORD dwRes = EC_E_ERROR;

    pFeatures = (EC_PT_INTFEATUREFIELD_DESC)OsMalloc(sizeof(EC_T_INTFEATUREFIELD_DESC));
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "CEcMaster:CreateFeatureField", pFeatures, sizeof(EC_T_INTFEATUREFIELD_DESC));
    if (EC_NULL == pFeatures)
    {
        dwRes = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pFeatures, 0, sizeof(EC_T_INTFEATUREFIELD_DESC));

    pFeatures->dwSupportedFeatures          = dwSupportedFeatures;

    pFeatures->dwHCDetectionTimeout         = CECMASTERCONST_m_dwHCDetectionTimeout;
    pFeatures->dwSBTimeout                  = CECMASTERCONST_m_dwSBTimeout;
    pFeatures->dwStateChangeDefaultTimeout  = CECMASTERCONST_m_dwStateChangeDefaultTimeout;
    pFeatures->bSlaveAliasAddressing        = EC_FALSE;
    OsMemset(pFeatures->m_abReadEEPROMEntry, 0, READEEPROMENTRY_COUNT*sizeof(EC_T_BOOL));
    pFeatures->m_abReadEEPROMEntry[READEEPROMENTRY_IDX_VENDORID]    = BT_CHECK_EEPENTRY_VENDORID;
    pFeatures->m_abReadEEPROMEntry[READEEPROMENTRY_IDX_PRODUCTCODE] = BT_CHECK_EEPENTRY_PRODUCTCODE;
    pFeatures->m_abReadEEPROMEntry[READEEPROMENTRY_IDX_REVISIONNO]  = BT_CHECK_EEPENTRY_REVISIONNO;
    pFeatures->m_abReadEEPROMEntry[READEEPROMENTRY_IDX_SERIALNO]    = BT_CHECK_EEPENTRY_SERIALNO;
#if (defined VLAN_FRAME_SUPPORT)
    pFeatures->bVLANEnabled                 = CECMASTERCONST_m_bVLANEnabled;
    pFeatures->wVLANId                      = CECMASTERCONST_m_wVLANId;
    pFeatures->byVLANPrio                   = CECMASTERCONST_m_byVLANPrio;
#endif
    pFeatures->bLocksDisabled               = EC_FALSE;
    pFeatures->bTopologyChangeAutoMode      = CECMASTERCONST_m_bTopologyChangeAutoMode;
    pFeatures->dwTopologyChangeDelay        = CECMASTERCONST_m_dwTopologyChangeDelay;
#if (defined INCLUDE_HOTCONNECT)
    pFeatures->oHCMode                      = CECMASTERCONST_m_oHCMODE;
#endif
    pFeatures->bNotifyUnexpectedBusSlaves   = CECMASTERCONST_m_bNotifyUnexpectedBusSlaves;
    pFeatures->dwInitCmdRetryTimeout        = CECMASTERCONST_m_dwInitCmdRetryTimeout;
    pFeatures->dwMbxCmdTimeout              = CECMASTERCONST_m_dwMbxCmdTimeout;
    pFeatures->dwDefaultMbxPolling          = CECMASTERCONST_m_dwDefaultMbxPolling;
#if (defined INCLUDE_DC_SUPPORT)
    pFeatures->bDcDefault                   = EC_TRUE;
    OsMemset(&pFeatures->DcConfig,  0, sizeof(EC_T_DC_CONFIGURE));
    OsMemset(&pFeatures->DcmConfig, 0, sizeof(EC_T_DCM_CONFIG));
    pFeatures->dwDcmInSyncTimeout           = CECMASTERCONST_m_dwDcmInSyncTimeout;
    pFeatures->bFirstDcSlaveAsRefClock      = CECMASTERCONST_m_bFirstDcSlaveAsRefClock;
#endif
    pFeatures->bAllSlavesMustReachState     = CECMASTERCONST_m_bAllSlavesMustReachState;
    pFeatures->dwNotificationMask           = CECMASTERCONST_m_dwNotificationMask;
    pFeatures->dwCycErrorNotifyMask         = CECMASTERCONST_m_dwCycErrorNotifyMask;

#if (defined INCLUDE_MASTER_OBD)
    pFeatures->dwVendorID                   = EC_MASTER_VENDOR_ID;
    pFeatures->dwProductcode                = EC_MASTER_PRODUCT_CODE;
    pFeatures->dwRevision                   = ATECAT_VERSION;
    pFeatures->dwSerialnumber               = 0;
    pFeatures->wDevicenameSize              = (EC_T_WORD)(OsStrlen((EC_T_CHAR*)EC_MASTER_DEVICE_NAME) + 1);
    pFeatures->szDevicename                 = (EC_T_CHAR*)OsMalloc(pFeatures->wDevicenameSize);
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "pFeatures->szDevicename", pFeatures->szDevicename, pFeatures->wDevicenameSize);
    
    pFeatures->wHardwareversionSize         = (EC_T_WORD)(OsStrlen((EC_T_CHAR*)EC_MASTER_HW_VERSION) + 1);
    pFeatures->szHardwareversion            = (EC_T_CHAR*)OsMalloc(pFeatures->wHardwareversionSize);
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "pFeatures->szHardwareversion", pFeatures->szHardwareversion, pFeatures->wHardwareversionSize);

    pFeatures->wSoftwareversionSize        = (EC_T_WORD)(OsStrlen((EC_T_CHAR*)EC_MASTER_SW_VERSION) + 1);
    pFeatures->szSoftwareversion            = (EC_T_CHAR*)OsMalloc(pFeatures->wSoftwareversionSize);
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "pFeatures->szSoftwareversion", pFeatures->szSoftwareversion, pFeatures->wSoftwareversionSize);
    if ((EC_NULL == pFeatures->szDevicename) || (EC_NULL == pFeatures->szHardwareversion) || (EC_NULL == pFeatures->szSoftwareversion))
    {
        dwRes = EC_E_NOMEMORY;
        goto Exit;
    }
    OsStrncpy(pFeatures->szDevicename, EC_MASTER_DEVICE_NAME, pFeatures->wDevicenameSize);
    OsSnprintf(pFeatures->szHardwareversion, (EC_T_INT)pFeatures->wHardwareversionSize, "V%d.%d.%d.%02d", ATECAT_VERS_MAJ, ATECAT_VERS_MIN, ATECAT_VERS_SERVICEPACK, ATECAT_VERS_BUILD);
    OsSnprintf(pFeatures->szSoftwareversion, (EC_T_INT)pFeatures->wSoftwareversionSize, "V%d.%d.%d.%02d", ATECAT_VERS_MAJ, ATECAT_VERS_MIN, ATECAT_VERS_SERVICEPACK, ATECAT_VERS_BUILD);
#endif

    pFeatures->eCycFrameLayout              = CECMASTERCONST_m_eCycFrameLayout;
    pFeatures->bCopyInfoInSendCycFrames     = CECMASTERCONST_m_bCopyInfoInSendCycFrames;
    pFeatures->bIgnoreInputsOnWkcError      = CECMASTERCONST_m_bIgnoreInputsOnWkcError;
    pFeatures->bZeroInputsOnWkcZero         = CECMASTERCONST_m_bZeroInputsOnWkcZero;
    pFeatures->bZeroInputsOnWkcError        = CECMASTERCONST_m_bZeroInputsOnWkcError;

#if (defined INCLUDE_VARREAD)
    pFeatures->bAddVarsForSpecificDataTypes  = CECMASTERCONST_m_bAddVarsForSpecificDataTypes;
#endif
#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
    pFeatures->bRedEnhancedLineCrossedDetectionEnabled = CECMASTERCONST_m_bRedEnhancedLineCrossedDetectionEnabled;
#endif
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    pFeatures->bErrorOnLineCrossed = CECMASTERCONST_m_bErrorOnLineCrossed;
    pFeatures->bNotifyNotConnectedPortA = CECMASTERCONST_m_bNotifyNotConnectedPortA;
    pFeatures->bNotifyUnexpectedConnectedPort = CECMASTERCONST_m_bNotifyUnexpectedConnectedPort;
#endif
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    pFeatures->bJunctionRedundancyEnabled = CECMASTERCONST_m_bJunctionRedundancyEnabled;
#endif

    pFeatures->bGenEniAssignEepromBackToEcat = CECMASTERCONST_m_bGenEniAssignEepromBackToEcat;
    pFeatures->bAutoAckAlStatusError = CECMASTERCONST_m_bAutoAckAlStatusError;
    pFeatures->bAutoAdjustCycCmdWkc = CECMASTERCONST_m_bAutoAdjustCycCmdWkc;
    pFeatures->bAdjustCycFramesAfterslavesStateChange = CECMASTERCONST_m_bAdjustCycFramesAfterslavesStateChange;

    /* no errors */
    pRetVal = pFeatures;

Exit:
    if (EC_E_NOMEMORY == dwRes)
    {
        CEcMaster::FreeFeatureField(pFeatures, pMLog);
    }

    return pRetVal;
}

/***************************************************************************************************/
/**
\brief  Set Serialized Configuration Entries.

This function configures serialized feature entries for re-spawn of master with equal entries.
Input is result from old Instance GetFeatureField method.
\return EC_E_NOERROR on success or error code otherwise.
*/
EC_T_DWORD CEcMaster::SetFeatureField(
    EC_T_INTFEATUREFIELD_DESC*  pFeatures /**< [in] Feature Field*/
                                     )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
#if (defined INCLUDE_MASTER_OBD)
    EC_T_DWORD                  dwRes       = EC_E_ERROR;
#endif

    if (EC_NULL == pFeatures)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

#if (defined INCLUDE_OEM)
    SetOemKey(pFeatures->qwOemKey);
#endif /* INCLUDE_OEM */
    m_dwSupportedFeatures           = pFeatures->dwSupportedFeatures;
    m_dwHCDetectionTimeout          = pFeatures->dwHCDetectionTimeout;
    m_dwSBTimeout                   = ((0 == pFeatures->dwSBTimeout)?m_dwSBTimeout:pFeatures->dwSBTimeout);
    m_dwStateChangeDefaultTimeout   = pFeatures->dwStateChangeDefaultTimeout;
#if (defined INCLUDE_DC_SUPPORT)
    m_bAddBrdSyncWindowMonitoring   = pFeatures->bAddBrdSyncWindowMonitoring;
#endif /* INCLUDE_DC_SUPPORT */
    if (EC_NULL != m_pBusTopology)
    {
        m_pBusTopology->EnableAliasAddressing(pFeatures->bSlaveAliasAddressing);
        OsMemcpy(m_pBusTopology->m_abReadEEPROMEntry, pFeatures->m_abReadEEPROMEntry, READEEPROMENTRY_COUNT*sizeof(EC_T_BOOL));
        OsMemcpy(m_pBusTopology->m_abCheckEEPROMSetByIoctl, pFeatures->m_abCheckEEPROMSetByIoctl, CHECKEEPROMENTRY_COUNT*sizeof(EC_T_BOOL));
        OsMemcpy(m_pBusTopology->m_aeCheckEEPROMEntry, pFeatures->m_aeCheckEEPROMEntry, CHECKEEPROMENTRY_COUNT*sizeof(EC_T_CHECKEEPROMENTRY));
    }
#if (defined INCLUDE_MASTER_OBD)
#if (defined INCLUDE_DC_SUPPORT)
    if (EC_NULL != m_pSDOService)
    {
        m_pSDOService->EntrySet_SetDCEnabled(m_bDCSupportEnabled);
    }
#endif
#endif
#if (defined VLAN_FRAME_SUPPORT)
    m_bVLANEnabled = pFeatures->bVLANEnabled;
    m_wVLANId      = pFeatures->wVLANId;
    m_byVLANPrio   = pFeatures->byVLANPrio;
#endif
    m_pBusTopology->SetTopologyChangeAutoMode(pFeatures->bTopologyChangeAutoMode);
    m_pBusTopology->SetTopologyChangeDelay(pFeatures->dwTopologyChangeDelay);
#if (defined INCLUDE_HOTCONNECT)
    SetHCMode(pFeatures->oHCMode);
#endif
    m_pBusTopology->SetNotifyUnexpectedBusSlaves(pFeatures->bNotifyUnexpectedBusSlaves);
    m_dwInitCmdRetryTimeout = pFeatures->dwInitCmdRetryTimeout;
    m_dwMbxCmdTimeout = pFeatures->dwMbxCmdTimeout;
    m_dwDefaultMbxPolling = pFeatures->dwDefaultMbxPolling;
    if (pFeatures->bPdOffsetCompMode)
    {
        SetProcessDataCompatMode();
    }
#if (defined INCLUDE_DC_SUPPORT)
    if (!pFeatures->bDcDefault)
    {
        m_pDistributedClks->Configure(&pFeatures->DcConfig);
        m_pDistributedClks->DcmConfigure(&pFeatures->DcmConfig);
    }
    m_dwDcmInSyncTimeout = pFeatures->dwDcmInSyncTimeout;
    m_pDistributedClks->SetFirstDcSlaveAsRefClock(pFeatures->bFirstDcSlaveAsRefClock);
#endif
    m_bAllSlavesMustReachState = pFeatures->bAllSlavesMustReachState;
    m_dwNotificationMask = pFeatures->dwNotificationMask;
    m_dwCycErrorNotifyMask = pFeatures->dwCycErrorNotifyMask;

    m_qwLk = pFeatures->qwLk;

#if (defined INCLUDE_MASTER_OBD)
    if (EC_NULL != m_pSDOService)
    {
        dwRes = m_pSDOService->Write(COEOBJID_IDENTITY_OBJECT, 1, sizeof(EC_T_DWORD), EC_NULL, (EC_T_WORD*)(EC_T_VOID*)&pFeatures->dwVendorID,     EC_FALSE, EC_TRUE);
        OsDbgAssert(EC_E_NOERROR == dwRes);
        dwRes = m_pSDOService->Write(COEOBJID_IDENTITY_OBJECT, 2, sizeof(EC_T_DWORD), EC_NULL, (EC_T_WORD*)(EC_T_VOID*)&pFeatures->dwProductcode,  EC_FALSE, EC_TRUE);
        OsDbgAssert(EC_E_NOERROR == dwRes);
        dwRes = m_pSDOService->Write(COEOBJID_IDENTITY_OBJECT, 3, sizeof(EC_T_DWORD), EC_NULL, (EC_T_WORD*)(EC_T_VOID*)&pFeatures->dwRevision,     EC_FALSE, EC_TRUE);
        OsDbgAssert(EC_E_NOERROR == dwRes);
        dwRes = m_pSDOService->Write(COEOBJID_IDENTITY_OBJECT, 4, sizeof(EC_T_DWORD), EC_NULL, (EC_T_WORD*)(EC_T_VOID*)&pFeatures->dwSerialnumber, EC_FALSE, EC_TRUE);
        OsDbgAssert(EC_E_NOERROR == dwRes);
    }
    if (EC_NULL != pFeatures->szDevicename)
    {
        dwRes = m_pSDOService->Write(COEOBJID_MANF_DEVICE_NAME, 0, (EC_T_DWORD)OsStrlen(pFeatures->szDevicename), EC_NULL,
            (EC_T_WORD*)(EC_T_VOID*)pFeatures->szDevicename, EC_FALSE, EC_TRUE);
        OsDbgAssert(EC_E_NOERROR == dwRes);
    }
    if (EC_NULL != pFeatures->szHardwareversion)
    {
        dwRes = m_pSDOService->Write(COEOBJID_MANF_HW_VERSION, 0, (EC_T_DWORD)OsStrlen(pFeatures->szHardwareversion), EC_NULL,
            (EC_T_WORD*)(EC_T_VOID*)pFeatures->szHardwareversion, EC_FALSE, EC_TRUE);
        OsDbgAssert(EC_E_NOERROR == dwRes);
    }
    if (EC_NULL != pFeatures->szSoftwareversion)
    {
        dwRes = m_pSDOService->Write(COEOBJID_MANF_SW_VERSION, 0, (EC_T_DWORD)OsStrlen(pFeatures->szSoftwareversion), EC_NULL,
            (EC_T_WORD*)(EC_T_VOID*)pFeatures->szSoftwareversion, EC_FALSE, EC_TRUE);
        OsDbgAssert(EC_E_NOERROR == dwRes);
    }
#endif
    m_eCycFrameLayout = pFeatures->eCycFrameLayout;
#if (defined INCLUDE_TRACE_DATA)
    m_wTraceDataSize = pFeatures->wTraceDataSize;
#endif /* INCLUDE_TRACE_DATA */
    m_bCopyInfoInSendCycFrames = pFeatures->bCopyInfoInSendCycFrames;
    SetIgnoreInputsOnWkcError (pFeatures->bIgnoreInputsOnWkcError);
    SetZeroInputsOnWkcZero (pFeatures->bZeroInputsOnWkcZero);
    SetZeroInputsOnWkcError (pFeatures->bZeroInputsOnWkcError);

#if (defined INCLUDE_VARREAD)
    m_bAddVarsForSpecificDataTypes = pFeatures->bAddVarsForSpecificDataTypes;
#endif
#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
    m_pBusTopology->SetRedEnhancedLineCrossedDetectionEnabled(pFeatures->bRedEnhancedLineCrossedDetectionEnabled);
#endif
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    m_pBusTopology->SetErrorOnLineCrossed(pFeatures->bErrorOnLineCrossed);
    m_pBusTopology->SetNotifyNotConnectedPortA(pFeatures->bNotifyNotConnectedPortA);
    m_pBusTopology->SetNotifyUnexpectedConnectedPort(pFeatures->bNotifyUnexpectedConnectedPort);
#endif
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    m_pBusTopology->SetJunctionRedundancyEnabled(pFeatures->bJunctionRedundancyEnabled);
#endif

    m_bGenEniAssignEepromBackToEcat = pFeatures->bGenEniAssignEepromBackToEcat;
    m_bAutoAckAlStatusError = pFeatures->bAutoAckAlStatusError;
    m_bAutoAdjustCycCmdWkc = pFeatures->bAutoAdjustCycCmdWkc;
    m_bAdjustCycFramesAfterslavesStateChange = pFeatures->bAdjustCycFramesAfterslavesStateChange;

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
#if !(defined DEBUG) && (defined INCLUDE_MASTER_OBD)
    EC_UNREFPARM(dwRes);
#endif
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Register external memory provider.

\.return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::SetPDMemProvider(EC_T_MEMPROV_DESC* pMemProv)
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;

    if (EC_NULL == pMemProv )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (m_eCycFrameLayout == eCycFrameLayout_IN_DMA)
    {
        OsMemcpy(pMemProv->pbyPDOutData, GetPDOutPtr(), GetPDOutSize());
    }
    OsMemcpy(&m_oMemProvider, pMemProv, sizeof(EC_T_MEMPROV_DESC));

    /* validate provided memory */
    dwRetVal = IsAnyCmdExceedingImage();

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Set process data image pointers to memory provider.

Usually used to store default ioimage parameters to memory provider structure.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::SetPDOutPointer(
                                      EC_T_PBYTE pbyBuffer,
                                      EC_T_DWORD dwBufSize
                                     )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    m_oMemProvider.pbyPDOutData         = pbyBuffer;
    m_oMemProvider.dwPDOutDataLength    = dwBufSize;

    dwRetVal = EC_E_NOERROR;

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Set process data image pointers to memory provider.

Usually used to store default ioimage parameters to memory provider structure.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::SetPDInPointer(
                                      EC_T_PBYTE pbyBuffer,
                                      EC_T_DWORD dwBufSize
                                    )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    m_oMemProvider.pbyPDInData         = pbyBuffer;
    m_oMemProvider.dwPDInDataLength    = dwBufSize;

    dwRetVal = EC_E_NOERROR;

    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Get Process data image out base pointer.

\return Ptr or EC_NULL in case of error.
*/
EC_T_PBYTE CEcMaster::GetPDOutBasePointer()
{
    EC_T_PBYTE pbyRet   = EC_NULL;

#ifdef INCLUDE_MEMORY_PROVIDER
    if (EC_NULL != m_oMemProvider.pfPDOutDataReadRequest)
    {
        m_oMemProvider.pfPDOutDataReadRequest( m_oMemProvider.pvContext, &pbyRet );
    }
#endif
    if (EC_NULL == pbyRet )
    {
        pbyRet = m_oMemProvider.pbyPDOutData;
    }

    return pbyRet;
}

EC_T_VOID CEcMaster::ReturnPDOutBasePointer()
{
#ifdef INCLUDE_MEMORY_PROVIDER
    if (EC_NULL != m_oMemProvider.pfPDOutDataReadRelease )
        m_oMemProvider.pfPDOutDataReadRelease(m_oMemProvider.pvContext);
#endif
}



/***************************************************************************************************/
/**
\brief  Get Process data image in base pointer.

\return Ptr or EC_NULL in case of error.
*/
EC_T_PBYTE CEcMaster::GetPDInBasePointer(EC_T_VOID)
{
    EC_T_PBYTE pbyRet   = EC_NULL;

    if (!m_bHavePdWriteAccess)
    {
        m_bHavePdWriteAccess = EC_TRUE;
#ifdef INCLUDE_MEMORY_PROVIDER
        if (EC_NULL != m_oMemProvider.pfPDInDataWriteRequest)
        {
            m_oMemProvider.pfPDInDataWriteRequest( m_oMemProvider.pvContext, &pbyRet );
        }
#endif

        if (EC_NULL == pbyRet )
        {
            pbyRet = m_oMemProvider.pbyPDInData;
        }
        m_pbyPDInBasePtr = pbyRet;
    }

    return m_pbyPDInBasePtr;
}


EC_T_VOID CEcMaster::ReturnPDInBasePointer(EC_T_VOID)
{
    if (m_bHavePdWriteAccess)
    {
        m_bHavePdWriteAccess = EC_FALSE;
#ifdef INCLUDE_MEMORY_PROVIDER
        if (EC_NULL != m_oMemProvider.pfPDInDataWriteRelease )
        {
            m_oMemProvider.pfPDInDataWriteRelease(m_oMemProvider.pvContext);
        }
#endif
    }

    m_pbyPDInBasePtr = EC_NULL;
}

#if (defined INCLUDE_WKCSTATE)
EC_T_DWORD CEcMaster::CreateDiagnosisImage(EC_T_INT nNumCyclicCmds)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;

    m_nDiagnosisImageSize = (nNumCyclicCmds+7)/8;
    m_pbyDiagnosisImage = (EC_T_BYTE*)OsMalloc(m_nDiagnosisImageSize);
    if (EC_NULL == m_pbyDiagnosisImage)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CreateDiagnosisImage() m_pbyDiagnosisImage", m_pbyDiagnosisImage, m_nDiagnosisImageSize);
    /* initialize all Wkc states as error, except bit 0 */
    OsMemset(m_pbyDiagnosisImage, 0xFF, m_nDiagnosisImageSize);
    EC_CLRBIT(m_pbyDiagnosisImage, 0);

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}
#endif /* INCLUDE_WKCSTATE */


#if (defined INCLUDE_MASTER_OBD)
/***************************************************************************************************/
/**
\brief  Store Link Layer Parameters to ODL.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::SDOSetLinkLayerParms(
    EC_T_LINK_PARMS* pLinkParms  /**< [in]   LinkLayer Parameter set */
                                       )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    if (EC_NULL == m_pSDOService )
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRetVal = m_pSDOService->SetLinkLayerParms(pLinkParms);
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Store if Link Layer does support Polling or not.
*/
EC_T_VOID CEcMaster::SDOSetLinkLayerPolling(
    EC_T_BOOL bActive  /**< [in]   Alloc support on / off */
                                             )
{
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->SetLinkLayerPolling(bActive);
    }
}

/***************************************************************************************************/
/**
\brief  Store if Link Layer does support Frame Alloc or not.
*/
EC_T_VOID CEcMaster::SDOSetLinkLayerAllocSupport(
    EC_T_BOOL bActive  /**< [in]   Alloc support on / off */
                                             )
{
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->SetLinkLayerAllocSupport(bActive);
    }
}

/***************************************************************************************************/
/**
\brief  Store Hardware Addresses.
*/
EC_T_VOID CEcMaster::SDOSetMacAddress(
    EC_T_BOOL bRedIf,   /**< [in]   EC_TRUE: redundant HW */
    EC_T_BYTE byMac[6]  /**< [in]   HW Address */
                                     )
{
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->SetMacAddress(bRedIf, byMac);
    }
}

/***************************************************************************************************/
/**
\brief  Store Config Addresses.
*/
EC_T_VOID CEcMaster::SDOSetCfgMacAddress(EC_T_VOID)
{
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->SetCfgMacAddress(EC_TRUE,  m_pMasterDesc->oMacSrc.b);
        m_pSDOService->SetCfgMacAddress(EC_FALSE, m_pMasterDesc->oMacDest.b);
    }
}

/***************************************************************************************************/
/**
\brief  Clear OBD Slave list.
*/
EC_T_VOID CEcMaster::SDOClearSlaves(EC_T_WORD wNumOfSlavesToClean)
{
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->CleanSlaves(wNumOfSlavesToClean);
    }
}

/***************************************************************************************************/
/**
\brief  Store additional slave to OD.

*/
EC_T_VOID CEcMaster::SDOAddSlave(
    EC_T_DWORD  dwSDOSlaveIdx       /**< [in]   Master OD 0x3000 Slave Array Index */
   ,EC_T_DWORD  dwVendorId          /**< [in]   Vendor Code */
   ,EC_T_DWORD  dwProdCode          /**< [in]   Product Code */
   ,EC_T_DWORD  dwRevisionNo        /**< [in]   Revision No */
   ,EC_T_DWORD  dwSerialNo          /**< [in]   Serial No*/
   ,EC_T_WORD   wMbxSupportedProtocols /**< [in]   Supported Mailbox Protokolls */
   ,EC_T_WORD   wAutoIncAddr        /**< [in]   Auto Increment Address*/
   ,EC_T_WORD   wFixedAddr          /**< [in]   Slave Programmed fixed Address */
   ,EC_T_WORD   wAliasAddr          /**< [in]   Slave Programmed Alias Address */
   ,EC_T_WORD   wPortState          /**< [in]   Slave Port state */
   ,EC_T_BOOL   bDCSupport          /**< [in]   DC Support */
   ,EC_T_BOOL   bDC64Support        /**< [in]   DC 64Bit Support */
   ,EC_T_DWORD  dwSbErrorCode       /**< [in]   Slave scanbus check status */
   ,EC_T_BYTE   byFmmuEntitites     /**< [in]   Number of FMMU entities */
   ,EC_T_BYTE   bySyncManEntities   /**< [in]   Number of Sync Manager entities */
   ,EC_T_BYTE   byRAMSizeKb         /**< [in]   Size of Slave RAM in kByte */
   ,EC_T_BYTE   byPortDescriptor    /**< [in]   Port Descriptor */
   ,EC_T_BYTE   byESCType           /**< [in]   ESC type */
   ,CEcSlave*   pCfgSlave           /**< [in]   Hooked Slave configuration Instance if existent */
                                )
{
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->AddSlave(
            dwSDOSlaveIdx, dwVendorId, dwProdCode, dwRevisionNo, dwSerialNo, wMbxSupportedProtocols,
            wAutoIncAddr, wFixedAddr, wAliasAddr, wPortState,
            bDCSupport, bDC64Support, dwSbErrorCode, byFmmuEntitites,
            bySyncManEntities, byRAMSizeKb, byPortDescriptor, byESCType, pCfgSlave
                               );
    }
}

/***************************************************************************************************/
/**
\brief  Store additional slave to OD.

*/
EC_T_VOID CEcMaster::SDOAddConfiguredSlave(
    EC_T_DWORD  dwSlaveIdx          /**< [in]   Slave Array Index */
   ,CEcSlave*   pCfgSlave           /**< [in]   Slave configuration Instance */
                                )
{
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->AddConfiguredSlave(dwSlaveIdx, pCfgSlave);
    }
}

/***************************************************************************************************/
/**
\brief  Store additional slave to OD.

*/
EC_T_VOID CEcMaster::SDOAddConnectedSlave(
   CEcBusSlave* pCfgBusSlave           /**< [in]   Slave configuration Instance */
                                )
{
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->AddConnectedSlave(pCfgBusSlave);
    }
}

/***************************************************************************************************/
/**
\brief  Store error flag to ODL.

*/
EC_T_VOID CEcMaster::SDOSetSlaveError(
    EC_T_DWORD  dwSDOSlaveIdx,  /**< [in]   Master OD 0x3000 Slave Array Index */
    EC_T_DWORD  dwALStatus,     /**< [in]   AL Status register */
    EC_T_DWORD  dwStatusCode    /**< [in]   Status code */
                                     )
{
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->SetSlaveError(dwSDOSlaveIdx, dwALStatus, dwStatusCode);
    }
}

/***************************************************************************************************/
/**
\brief  Set Slave counters.
*/
EC_T_VOID CEcMaster::SDOSetSlaveAmount(
    EC_T_DWORD  dwSlaves,       /**< [in]   SB Slaves */
    EC_T_DWORD  dwDCSlaves      /**< [in]   SB DC Slaves */
                                      )
{
    EC_T_DWORD dwCfgSlaves = 0;
    EC_T_DWORD dwMbSlaves = 0;

    dwCfgSlaves = m_nCfgSlaveCount;
    dwMbSlaves  = m_nEcMbSlave;

    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->SetSlaveAmount(dwSlaves, dwDCSlaves, dwCfgSlaves, dwMbSlaves);
    }
}

EC_T_DWORD*     CEcMaster::SDOGetTXFrameCrtPtr(EC_T_VOID)
{
    if (EC_NULL != m_pSDOService)
    {
        return m_pSDOService->GetTXFrameCrtPtr();
    }

    return EC_NULL;
}

EC_T_DWORD*     CEcMaster::SDOGetRXFrameCrtPtr(EC_T_VOID)
{
    if (EC_NULL != m_pSDOService)
    {
        return m_pSDOService->GetRXFrameCrtPtr();
    }

    return EC_NULL;
}

EC_T_DWORD*     CEcMaster::SDOGetCycFrameCrtPtr(EC_T_VOID)
{
    if (EC_NULL != m_pSDOService)
    {
        return m_pSDOService->GetCycFrameCrtPtr();
    }

    return EC_NULL;
}

EC_T_DWORD*     CEcMaster::SDOGetCycDgramCrtPtr(EC_T_VOID)
{
    if (EC_NULL != m_pSDOService)
    {
        return m_pSDOService->GetCycDgramCrtPtr();
    }

    return EC_NULL;
}

EC_T_DWORD*     CEcMaster::SDOGetAcycFrameCrtPtr(EC_T_VOID)
{
    if (EC_NULL != m_pSDOService)
    {
        return m_pSDOService->GetAcycFrameCrtPtr();
    }

    return EC_NULL;
}

EC_T_DWORD*     CEcMaster::SDOGetAcycDgramCrtPtr(EC_T_VOID)
{
    if (EC_NULL != m_pSDOService)
    {
        return m_pSDOService->GetAcycDgramCrtPtr();
    }

    return EC_NULL;
}

/***************************************************************************************************/
/**
\brief  Store config file CRC32 to OD.
*/
EC_T_VOID CEcMaster::SDOSetConfigCRC32ChkSum(
    EC_T_DWORD dwCRC32Sum       /**< [in]   CRC32 Checksum */
                                            )
{
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->SetConfigCRC32ChkSum(dwCRC32Sum);
    }
}

/***************************************************************************************************/
/**
\brief  Set DC Busy State to OD.
*/
EC_T_VOID CEcMaster::SDOEntrySet_SetDCBusy(
    EC_T_BOOL   bDCBusy     /**< [in]   DC Busy */
                                          )
{
    if (EC_NULL != m_pSDOService )
    {
        m_pSDOService->EntrySet_SetDCBusy(bDCBusy);
    }
}

#endif

EC_T_VOID CEcMaster::RecalculateCycCmdWkc(CEcSlave* pCfgSlave)
{
    EC_T_DWORD dwCycCmdWkcIdx;
    EC_T_WORD wSlaveDevState = pCfgSlave->GetDeviceState();

    if (GetAutoAdjustCycCmdWkc())
    {
        for (dwCycCmdWkcIdx = 0; dwCycCmdWkcIdx < pCfgSlave->m_dwNumCycCmdWkc; dwCycCmdWkcIdx++)
        {
            EC_T_CYC_CMD_WKC_DESC_ENTRY* pCycCmdWkc = &pCfgSlave->m_aCycCmdWkcList[dwCycCmdWkcIdx];

            /* slave is present and its state matches CycCmd's OpState */
            if (pCfgSlave->IsPresentOnBus() && (0 != (pCycCmdWkc->pCycCmdCfgDesc->wConfOpStatesMask & wSlaveDevState)))
            {
                if (!pCycCmdWkc->bActive)
                {
                    pCycCmdWkc->pCycCmdCfgDesc->wExpectedWkc = (EC_T_WORD)(pCycCmdWkc->pCycCmdCfgDesc->wExpectedWkc + pCycCmdWkc->wWkcIncrement);
                    pCycCmdWkc->bActive = EC_TRUE;
                }
            }
            else
            {
                if (pCycCmdWkc->bActive)
                {
                    pCycCmdWkc->pCycCmdCfgDesc->wExpectedWkc = (EC_T_WORD)(pCycCmdWkc->pCycCmdCfgDesc->wExpectedWkc - pCycCmdWkc->wWkcIncrement);
                    pCycCmdWkc->bActive = EC_FALSE;
                }
            }
        }
    }
}

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)

/***************************************************************************************************/
/**
\brief  Calculate expected working counters, based on HC Group entries.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::RecalculateHCWKC(EC_T_BOOL bAuto)
{
    return m_oHotConnect.RecalculateWKC(bAuto);
}

/***************************************************************************************************/
/**
\brief  Prepare storage list for cyclic datagrams in HC Instance.
*/
EC_T_VOID CEcMaster::CreateCycCmdHC(
    EC_T_DWORD dwNumCmds    /**< [in]   Total amount of cyclic datagrams */
                                   )
{
    m_oHotConnect.CreateCycCmd(dwNumCmds);
}

/***************************************************************************************************/
/**
\brief  Add Cyclic Command Descriptor to list of cyclic commands within HC Instance.
*/
EC_T_VOID CEcMaster::AddCycCmdHC(
    EcCycCmdConfigDesc* pEcCmdCfgDesc   /**< [in]   cyclic command to add */
                                )
{
    m_oHotConnect.AddCycCmd(pEcCmdCfgDesc);
}

/***************************************************************************************************/
/**
\brief  Configure HotConnect Mode of operation.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::SetHCMode(
    EC_T_EHOTCONNECTMODE    oMode   /**< [in]   HC Mode of Operation to configure */
                               )
{
    return m_oHotConnect.SetMode(oMode);
}

/***************************************************************************************************/
/**
\brief  Configure HotConnect Mode of operation.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_EHOTCONNECTMODE CEcMaster::GetHCMode(EC_T_VOID)
{
    return m_oHotConnect.GetMode();
}

/***************************************************************************************************/
/**
\brief  Configure HotConnect Timeout Value fields.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::HCConfigureTimeouts(
    EC_T_HC_CONFIGURETIMEOUTS_DESC*     pConfig /**< [in]   Timeout Configuration */
                                         )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (EC_NULL == pConfig )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    m_dwHCDetectionTimeout = pConfig->dwDetectionTimeout;

    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Continue Operation in Manual mode.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::StartHCManualContinue(EC_T_VOID)
{
    return m_oHotConnect.StartManualContinue();
}

#endif /* INCLUDE_HOTCONNECT || INCLUDE_WKCSTATE */

/***************************************************************************************************/
/**
\brief  Create HotConnect Port List.
*/
EC_T_VOID CEcMaster::CreatePortList(EC_T_VOID)
{
    m_oHotConnect.CreatePortList(m_nCfgSlaveCount);
}

/***************************************************************************************************/
/**
\brief  Start Topology Change.

Place Topo Change order to HC State Machine
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::StartHCTopologyChange(EC_T_VOID)
{
    return m_oHotConnect.StartTopologyChange();
}

/***************************************************************************************************/
/**
\brief  Get Status of Topology Change.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::WaitForHCStateMachine()
{
    return m_oHotConnect.WaitForHCStateMachine();
}

/***************************************************************************************************/
/**
\brief  Release HC state machine

\return Result
*/
EC_T_DWORD CEcMaster::ReleaseHCStateMachine(EC_T_DWORD dwResult)
{
    EC_UNREFPARM(dwResult);
    return m_oHotConnect.ReleaseHCStateMachine();
}

/***************************************************************************************************/
/**
\brief  Process CMD Irq field.
*/
EC_T_VOID CEcMaster::ProcessIrq(EC_T_WORD wIrq)
{
    if (TESTBIT(EXTEVT_DLSTATUS_CHG, wIrq) && !m_bDLStatusIRQMasked)
    {
        m_bDLStatusIRQ = EC_TRUE;
    }
    if (TESTBIT(EXTEVT_ALSTATUS_CHG, wIrq))
    {
        m_bALStatusIRQ = EC_TRUE;
    }
    return;
}

/***************************************************************************************************/
/**
\brief  Reset a specific slave by its Id.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::ResetSlaveById(
    EC_T_DWORD dwSlaveId    /**< [in]   Index of Slave to reset */
                                            )
{
    EC_T_DWORD dwRetVal  = EC_E_ERROR;
    CEcSlave*  pCfgSlave = EC_NULL;

    if (!m_bConfigured)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    pCfgSlave = GetCfgSlaveBySlaveId(dwSlaveId);
    if (EC_NULL == pCfgSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    pCfgSlave->ResetSlaveStateMachine();

    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}


/********************************************************************************/
/** \brief Transfer a raw EtherCAT command.
*
* \return  status value.
*/
EC_T_DWORD CEcMaster::EcTransferRawCmd
(   EC_T_DWORD  dwUniqueClntId,     /* [in]     client id, 0 if broadcast */
    EC_T_WORD   wInvokeId,          /* [in]     invoke id to be used for deferred transfers (timeout EC_NOWAIT) */
    EC_T_BYTE   byCmd,              /* [in]     EtherCAT command */
    EC_T_DWORD  dwMemoryAddress,    /* [in]     address inside the slave */
    EC_T_VOID*  pvData,             /* [in/out] data */
    EC_T_WORD   wLen,               /* [in]     size of data */
    EC_T_DWORD  dwTimeout           /* [in]     Timeout value. If set to EC_NOWAIT the transfer is queued and the application will get a notification */
)
{
    CEcTimer  oTimeout;
    EC_T_DWORD      dwRetVal                        = EC_E_NOERROR;
    EC_T_WORD       wAdp                            = 0,
                    wAdo                            = 0;
    EC_T_BOOL       bReadData                       = EC_TRUE;
    EC_T_BOOL       bInvalidateSingleRawTferRequest = EC_FALSE;
    EC_T_BOOL       bLockTaken                      = EC_FALSE;


    if (wLen > MAX_EC_DATA_LEN )
    {
        dwRetVal = EC_E_INVALIDSIZE;
        goto Exit;
    }
    switch( byCmd )
    {
    case EC_CMD_TYPE_LRD:
    case EC_CMD_TYPE_LWR:
    case EC_CMD_TYPE_LRW:
        wAdp = EC_LOWORD(dwMemoryAddress);
        wAdo = EC_HIWORD(dwMemoryAddress);
        break;
    case EC_CMD_TYPE_FPRD:
    case EC_CMD_TYPE_FPRW:
    case EC_CMD_TYPE_FPWR:
    case EC_CMD_TYPE_APRD:
    case EC_CMD_TYPE_APWR:
    case EC_CMD_TYPE_APRW:
    case EC_CMD_TYPE_ARMW:
    case EC_CMD_TYPE_FRMW:
        wAdp = EC_HIWORD(dwMemoryAddress);
        wAdo = EC_LOWORD(dwMemoryAddress);
        break;
    case EC_CMD_TYPE_BRD:
    case EC_CMD_TYPE_BWR:
    case EC_CMD_TYPE_BRW:
        wAdp = 0;
        wAdo = EC_LOWORD(dwMemoryAddress);
        break;
    default:
        /* command not supported */
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (   (byCmd == EC_CMD_TYPE_BWR)
        || (byCmd == EC_CMD_TYPE_LWR)
      )
    {
        bReadData = EC_FALSE;
    }

    if (dwTimeout == EC_NOWAIT )
    {
        EC_T_DWORD dwInvokeId;
        EC_T_DWORD dwClntIdent;
        if (dwUniqueClntId == 0 )
        {
            /* broadcast */
            dwInvokeId = (EC_T_DWORD)( INVOKE_ID_TFER_Q_RAW_CMD | wInvokeId);
        }
        else
        {
            /* client specific cmd */
            OsDbgAssert( (INVOKE_ID_TFER_Q_RAW_CMD - (dwUniqueClntId&(~IS_CLIENT_RAW_CMD))) >= INVOKE_ID_CLNT_TFER_Q_RAW_CMD );
            OsDbgAssert( (INVOKE_ID_TFER_Q_RAW_CMD - INVOKE_ID_CLNT_TFER_Q_RAW_CMD) == 0xFF0000 );
            OsDbgAssert( dwUniqueClntId & IS_CLIENT_RAW_CMD );
            dwClntIdent = (EC_T_DWORD)(((dwUniqueClntId & (~IS_CLIENT_RAW_CMD)) & 0xFF) << 16);
            dwInvokeId = (EC_T_DWORD)(INVOKE_ID_CLNT_TFER_Q_RAW_CMD | dwClntIdent | wInvokeId);
        }
        dwRetVal = QueueEcatCmdReq( EC_FALSE, EC_NULL, dwInvokeId, byCmd, wAdp, wAdo, wLen, pvData );
    }
    else
    {
        OsDbgAssert( !(dwUniqueClntId & IS_CLIENT_RAW_CMD) );
        OsLock(m_poEcTransferRawCmdLock); bLockTaken = EC_TRUE;
        if ((m_dwRawTferStatus == EC_E_BUSY) || m_bRawTferPending )
        {
            /* another request is pending (e.g. issued by another thread) */
            dwRetVal = EC_E_BUSY;
            goto Exit;
        }

        /* issue a new raw transfer */
        m_dwRawTferStatus = EC_E_BUSY;
        m_bRawTferPending = EC_TRUE;
        bInvalidateSingleRawTferRequest = EC_TRUE;
        if (bReadData )
        {
            m_wRawTferDataLen = wLen;
            m_pbyRawTferData = (EC_T_BYTE*)pvData;
        }
        else
        {
            m_wRawTferDataLen = 0;
            m_pbyRawTferData = EC_NULL;
        }
        OsDbgAssert(bLockTaken); OsUnlock(m_poEcTransferRawCmdLock); bLockTaken = EC_FALSE;
        dwRetVal = QueueEcatCmdReq( EC_FALSE, EC_NULL, INVOKE_ID_TFER_RAW_CMD, byCmd, wAdp, wAdo, wLen, pvData );
        if (dwRetVal != EC_E_NOERROR )
        {
            goto Exit;
        }
        /* wait for response */
        oTimeout.Start(dwTimeout, GetMsecCounterPtr());
        while( (m_dwRawTferStatus == EC_E_BUSY) && !oTimeout.IsElapsed() )
        {
            OsSleep(1);
        }
        dwRetVal = m_dwRawTferStatus;
        if (dwRetVal != EC_E_BUSY)
        {
            /* request has terminated */
            if ((dwRetVal == EC_E_NOERROR) && bReadData )
            {
                OsDbgAssert(wLen == m_wRawTferDataLen);
                OsDbgAssert(m_pbyRawTferData == (EC_T_BYTE*)pvData);
                if (pvData != m_pbyRawTferData)
                {
                    OsMemcpy(pvData, m_pbyRawTferData, wLen);
                }
            }
        }
        else if (oTimeout.IsElapsed() )
        {
            /* a timeout occurred */
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }
        else
            OsDbgAssert(EC_FALSE);

        if (!m_poEcDevice->IsLinkConnected() )
        {
            /* link cable disconnected */
            dwRetVal = EC_E_LINK_DISCONNECTED;
            goto Exit;
        }
    } /* else if (dwTimeout == EC_NOWAIT ) */

Exit:
    if (bInvalidateSingleRawTferRequest )
    {
        /* invalidate request */
        if (!bLockTaken)
        {
            OsLock(m_poEcTransferRawCmdLock);
            bLockTaken=EC_TRUE;
        }
        if (dwRetVal == EC_E_BUSY)
        {
            /* do not leave status in BUSY state!! */
            m_dwRawTferStatus = EC_E_NOERROR;
        }
        else
        {
            m_dwRawTferStatus = dwRetVal;
        }
        m_wRawTferDataLen = 0;
        m_pbyRawTferData = EC_NULL;
        m_bRawTferPending = EC_FALSE;
    }

    if (bLockTaken)
    {
        OsUnlock(m_poEcTransferRawCmdLock);
        bLockTaken = EC_FALSE;
    }

    return dwRetVal;
}

/********************************************************************************/
/** \brief Reset the cyclic frame manager.
 *  Called in case of link recovery.
 */
EC_T_VOID CEcMaster::ResetCyclicFrmMgr()
{
    m_wALStatusWkc = EC_AL_STATUS_INIT_VALUE;

    if (m_aCyclicEntryMgmtDesc != EC_NULL)
    {
    EC_T_DWORD dwCycEntryIdx = 0;

#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
        for (dwCycEntryIdx=0; dwCycEntryIdx<(EC_T_UINT)m_dwNumCycConfigEntries; dwCycEntryIdx++)
#else
        dwCycEntryIdx = 0;
#endif
        {
        CYCLIC_ENTRY_MGMT_DESC* pEcCycMgmtDesc = &m_aCyclicEntryMgmtDesc[dwCycEntryIdx];

            pEcCycMgmtDesc->wFramesMonitoringSndCnt = 0;
            pEcCycMgmtDesc->wFramesMonitoringRcvCnt = 0;

            for (EC_T_DWORD dwFrameIdx = 0; dwFrameIdx < pEcCycMgmtDesc->wNumFrames; dwFrameIdx++)
            {
                pEcCycMgmtDesc->apEcCycFrameDesc[dwFrameIdx]->bIsPending = EC_FALSE;
            }
        }
    }
}

/***************************************************************************************************/
/**
\brief  Start EEPROM Read from slave.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::PrepareEEPRomRead(
    EC_T_BOOL       bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
   ,EC_T_WORD       wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_WORD       wEEPRomStartOffset      /**< [in]   Address to start EEPRom Read from.*/
   ,EC_T_WORD*      pwReadData              /**< [in]   Pointer to WORD array to carry the read data */
   ,EC_T_DWORD      dwReadLen               /**< [in]   Sizeof Read Data WORD array (in WORDS) */
   ,EC_T_DWORD*     pdwNumOutData           /**< [out]  Pointer to DWORD carrying actually read data after completion */
   ,EC_T_DWORD      dwTimeout               /**< [in]   Operation Timeout */
                                       )
{
    return m_pEEEPRom->PrepareUpload(bFixedAddressing, wSlaveAddress, wEEPRomStartOffset, pwReadData, dwReadLen, pdwNumOutData, dwTimeout);
}

/***************************************************************************************************/
/**
\brief  Start EEPROM Write to slave.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::PrepareEEPRomWrite(
    EC_T_BOOL       bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
   ,EC_T_WORD       wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_WORD       wEEPRomStartOffset      /**< [in]   Address to start EEPRom Write from */
   ,EC_T_WORD*      pwWriteData             /**< [in]   Pointer to WORD array carrying the write data */
   ,EC_T_DWORD      dwWriteLen              /**< [in]   Sizeof Write Data WORD array (in WORDS) */
   ,EC_T_DWORD      dwTimeout               /**< [in]   Operation Timeout */
                                        )
{
    return m_pEEEPRom->PrepareDownload(bFixedAddressing, wSlaveAddress, wEEPRomStartOffset, pwWriteData, dwWriteLen, dwTimeout);
}

/***************************************************************************************************/
/**
\brief  Start EEPROM Reload.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::PrepareEEPRomReload(
    EC_T_BOOL       bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
   ,EC_T_WORD       wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_DWORD      dwTimeout               /**< [in]   Operation Timeout */
                                         )
{
    return m_pEEEPRom->PrepareReload(bFixedAddressing, wSlaveAddress, dwTimeout);
}

/***************************************************************************************************/
/**
\brief  Start Slave Reset.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::PrepareSlaveReset(
    EC_T_BOOL       bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
   ,EC_T_WORD       wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_DWORD      dwTimeout               /**< [in]   Operation Timeout */
                                       )
{
    return m_pEEEPRom->PrepareReset(bFixedAddressing, wSlaveAddress, dwTimeout);
}

/***************************************************************************************************/
/**
\brief  Start Assign Slave EEPRom to Slave PDI Application.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::PrepareEEPRomAssign(
    EC_T_BOOL       bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
   ,EC_T_WORD       wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_BOOL       bSlavePDIAccessEnable   /**< [in]   EC_TRUE: assign EEPRom to slave PDI application, EC_FALSE: assign EEPRom to ECat Master */
   ,EC_T_BOOL       bForceAssign            /**< [in]   EC_TRUE: force Assignment, only available for ECat Master Assignment, EC_FALSE: don't force */
   ,EC_T_DWORD      dwTimeout               /**< [in]   Operation Timeout */
                                          )
{
    return m_pEEEPRom->PrepareAssign( bFixedAddressing, wSlaveAddress, bSlavePDIAccessEnable, bForceAssign, dwTimeout);
}

/***************************************************************************************************/
/**
\brief  Start check whether Slave PDI application marks EEPRom Access active.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::PrepareEEPRomActive(
    EC_T_BOOL       bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
   ,EC_T_WORD       wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_BOOL*      pbSlavePDIAccessActive  /**< [out]  Pointer holding Boolean buffer, EC_TRUE: Slave PDI application EEPRom access active, EC_FALSE: not active */
   ,EC_T_DWORD      dwTimeout               /**< [in]   Operation Timeout */
                                         )
{
    return m_pEEEPRom->PrepareActive( bFixedAddressing, wSlaveAddress, pbSlavePDIAccessActive, dwTimeout);
}

/***************************************************************************************************/
/**
\brief  Start EEPROM Operation.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::StartEEPROMOperation(EC_T_VOID)
{
    return m_pEEEPRom->StartEEPROMOP();
}

/***************************************************************************************************/
/**
\brief  Poll progress of EEPROM Operation.

\return EC_E_ERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::PollEEPROMOperation()
{
    return m_pEEEPRom->PollEEPROMOp();
}

/***************************************************************************************************/
/**
\brief  Get all slave information

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD  CEcMaster::GetAllSlaveInfo(
    EC_T_BOOL               bFixedAddress   /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
   ,EC_T_WORD               wSlaveAddress   /**< [in]   Slave address */
   ,EC_T_GET_SLAVE_INFO*    pSlaveInfo      /**< [out]  Structure include all information of the slave. */
                                        )
{
    EC_T_DWORD   dwRetVal    = EC_E_ERROR;
    EC_T_DWORD   dwRes       = EC_E_ERROR;
    EC_T_BOOL    bListLocked = EC_FALSE;
    CEcBusSlave* pBusSlave   = EC_NULL;
    CEcSlave*    pCfgSlave   = EC_NULL;
    EC_T_DWORD   dwPortIdx   = 0;

     /* bus scan shall be complete */
    dwRes = GetBusScanResult();
    if (dwRes == EC_E_BUSY)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    /* lock the bus slave list */
    m_pBusTopology->InterLockList(EC_TRUE);
    bListLocked = EC_TRUE;

    /* get bus and config slave pointers */
    if (bFixedAddress)
    {
        /* bus slave linked to matching config slave has highest priority */
        pBusSlave = m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
        if (EC_NULL == pBusSlave)
        {
            /* matching config slave have next priority */
            pCfgSlave = GetSlaveByAddr(wSlaveAddress);

            /* don't return and eventually matching bus slave to protect API for changing results */
        }
        else
        {
            /* get config slave linked to bus slave */
            pCfgSlave = pBusSlave->GetCfgSlave();
        }
    }
    else
    {
        /* matching bus slave has highest priority */
        pBusSlave = m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
        if (EC_NULL == pBusSlave)
        {
            /* matching config slave have next priority */
            pCfgSlave = GetSlaveByCfgAutoIncAddr(wSlaveAddress);
        }
        else
        {
            /* get config slave linked to bus slave */
            pCfgSlave = pBusSlave->GetCfgSlave();
        }
    }
    /* at least one slave pointer is needed */
    if ((EC_NULL == pBusSlave) && (EC_NULL == pCfgSlave))
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* reset structure */
    OsMemset(pSlaveInfo, 0, sizeof(EC_T_GET_SLAVE_INFO));

    /* get information from bus slave */
    if (EC_NULL != pBusSlave)
    {
        pSlaveInfo->dwScanBusStatus  = pBusSlave->GetScanBusStatus();
        pSlaveInfo->dwVendorId       = pBusSlave->GetVendorId();
        pSlaveInfo->dwProductCode    = pBusSlave->GetProductCode();
        pSlaveInfo->dwRevisionNumber = pBusSlave->GetRevisionNo();
        pSlaveInfo->dwSerialNumber   = pBusSlave->GetSerialNo();

        pSlaveInfo->byESCType        = pBusSlave->GetESCType();
        pSlaveInfo->bDcSupport       = pBusSlave->IsDCSupport();
        pSlaveInfo->bDc64Support     = pBusSlave->IsDC64Support();

        pSlaveInfo->wAliasAddress    = pBusSlave->GetAliasAddress();
        pSlaveInfo->wPhysAddress     = pBusSlave->GetFixedAddress();

        pSlaveInfo->byPortDescriptor = pBusSlave->GetPortDescriptor();

        pSlaveInfo->wPortState       = pBusSlave->GetPortState();

        for (dwPortIdx = 0; dwPortIdx < ESC_PORT_COUNT; dwPortIdx++)
        {
            if ((EC_NULL == pBusSlave->m_apBusSlaveChildren[dwPortIdx])
             || (EC_NULL == pBusSlave->m_apBusSlaveChildren[dwPortIdx]->m_pCfgSlave))
            {
                pSlaveInfo->aPortSlaveIds[dwPortIdx] = INVALID_SLAVE_ID;
            }
            else
            {
                pSlaveInfo->aPortSlaveIds[dwPortIdx] = pBusSlave->m_apBusSlaveChildren[dwPortIdx]->m_pCfgSlave->GetSlaveId();
            }
        }
#if (defined INCLUDE_DC_SUPPORT)
        pSlaveInfo->dwSystemTimeDifference = pBusSlave->m_dwSystemTimeDifference;
#else
        pSlaveInfo->dwSystemTimeDifference = 0;
#endif
    }
    /* get information from config slave */
    if (EC_NULL != pCfgSlave)
    {
        dwRes = pCfgSlave->GetSlaveInfoAll(pSlaveInfo);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    /* get scan bus error code */
    pSlaveInfo->dwSBErrorCode = m_dwScanBusResult;

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (bListLocked)
    {
        bListLocked = EC_FALSE;
        m_pBusTopology->InterLockList(EC_FALSE);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get config slave information

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD  CEcMaster::GetCfgSlaveInfo(
    EC_T_BOOL               bStationAddress /**< [in]   EC_TRUE: Use station address, EC_FALSE: use auto increment address */
   ,EC_T_WORD               wSlaveAddress   /**< [in]   Slave address */
   ,EC_T_CFG_SLAVE_INFO*    pSlaveInfo      /**< [out]  Slave information */
    )
{
EC_T_DWORD   dwRetVal    = EC_E_ERROR;
EC_T_BOOL    bListLocked = EC_FALSE;
CEcSlave*    pCfgSlave   = EC_NULL;

    /* lock the bus slave list */
    m_pBusTopology->InterLockList(EC_TRUE);
    bListLocked = EC_TRUE;

    /* get config slave pointer */
    if (bStationAddress)
    {
        pCfgSlave = GetSlaveByAddr(wSlaveAddress);
    }
    else
    {
        pCfgSlave = GetSlaveByCfgAutoIncAddr(wSlaveAddress);
    }
    if (EC_NULL == pCfgSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    pCfgSlave->GetCfgSlaveInfo(pSlaveInfo);

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (bListLocked)
    {
        bListLocked = EC_FALSE;
        m_pBusTopology->InterLockList(EC_FALSE);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get bus slave information

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD  CEcMaster::GetBusSlaveInfo(
    EC_T_BOOL               bStationAddress /**< [in]   EC_TRUE: Use station address, EC_FALSE: use auto increment address */
   ,EC_T_WORD               wSlaveAddress   /**< [in]   Slave address */
   ,EC_T_BUS_SLAVE_INFO*    pSlaveInfo      /**< [out]  Slave information */
    )
{
EC_T_DWORD   dwRetVal    = EC_E_ERROR;
EC_T_DWORD   dwPortIdx   = 0;
EC_T_BOOL    bListLocked = EC_FALSE;
CEcBusSlave* pBusSlave   = EC_NULL;

    /* lock the bus slave list */
    m_pBusTopology->InterLockList(EC_TRUE);
    bListLocked = EC_TRUE;

    /* get bus slave pointer */
    if (bStationAddress)
    {
        pBusSlave = m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
    }
    else
    {
        pBusSlave = m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
    }
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* fill bus slave info structure */
    OsMemset(pSlaveInfo, 0, sizeof(EC_T_BUS_SLAVE_INFO));
    pSlaveInfo->dwSlaveId = GetSlaveIdByFixedAddress(pBusSlave->GetFixedAddress());
    for (dwPortIdx = 0; dwPortIdx < ESC_PORT_COUNT; dwPortIdx++)
    {
        if (pBusSlave->IsSlaveConPortX((EC_T_WORD)dwPortIdx))
        {
            if (EC_NULL != pBusSlave->m_apBusSlaveChildren[dwPortIdx])
            {
                pSlaveInfo->adwPortSlaveIds[dwPortIdx] = GetSlaveIdByFixedAddress(pBusSlave->m_apBusSlaveChildren[dwPortIdx]->GetFixedAddress());
            }
            else if ((pBusSlave == m_pBusTopology->GetBusSlaveMainPort()) && (m_pBusTopology->GetMainPort() == dwPortIdx))
            {
                pSlaveInfo->adwPortSlaveIds[dwPortIdx] = MASTER_SLAVE_ID;
            }
#if (defined INCLUDE_RED_DEVICE)
            else if ((pBusSlave == m_pBusTopology->GetBusSlaveRedPort()) && (m_pBusTopology->GetRedPort() == dwPortIdx))
            {
                pSlaveInfo->adwPortSlaveIds[dwPortIdx] = MASTER_RED_SLAVE_ID;
            }
#endif
            else if (pBusSlave->IsEL9010ConnectedX((EC_T_WORD)dwPortIdx))
            {
                pSlaveInfo->adwPortSlaveIds[dwPortIdx] = EL9010_SLAVE_ID;
            }
            else
            {
                pSlaveInfo->adwPortSlaveIds[dwPortIdx] = INVALID_SLAVE_ID;
            }
        }
        else if (pBusSlave->IsFramelossAfterSlave((EC_T_WORD)dwPortIdx))
        {
            pSlaveInfo->adwPortSlaveIds[dwPortIdx] = FRAMELOSS_SLAVE_ID;
        }
        else
        {
            pSlaveInfo->adwPortSlaveIds[dwPortIdx] = INVALID_SLAVE_ID;
        }
    }
    pSlaveInfo->wPortState              = pBusSlave->GetPortState();
    pSlaveInfo->wAutoIncAddress         = pBusSlave->GetAutoIncAddress();
#if( defined INCLUDE_DC_SUPPORT )
    pSlaveInfo->bDcSupport              = pBusSlave->IsDcUnitSupported();
    pSlaveInfo->bDc64Support            = pBusSlave->IsDcUnitSupported() && pBusSlave->IsDC64Support();
#endif
    pSlaveInfo->dwVendorId              = pBusSlave->GetVendorId();
    pSlaveInfo->dwProductCode           = pBusSlave->GetProductCode();
    pSlaveInfo->dwRevisionNumber        = pBusSlave->GetRevisionNo();
    pSlaveInfo->dwSerialNumber          = pBusSlave->GetSerialNo();
    pSlaveInfo->wMbxSupportedProtocols  = pBusSlave->GetMbxProtocols();
    pSlaveInfo->byESCType               = pBusSlave->GetESCType();
    pSlaveInfo->byESCRevision           = pBusSlave->GetESCRevision();
    pSlaveInfo->wESCBuild               = pBusSlave->GetESCBuild();
    pSlaveInfo->byPortDescriptor        = pBusSlave->GetPortDescriptor();
    pSlaveInfo->wFeaturesSupported      = pBusSlave->GetFeaturesSupported();
    pSlaveInfo->wStationAddress         = pBusSlave->GetFixedAddress();
    pSlaveInfo->wAliasAddress           = pBusSlave->GetAliasAddress();
    pSlaveInfo->wAlStatus               = pBusSlave->GetAlStatus();
    pSlaveInfo->wAlStatusCode           = pBusSlave->GetAlStatusCode();
#if (defined INCLUDE_DC_SUPPORT)
    pSlaveInfo->dwSystemTimeDifference  = pBusSlave->m_dwSystemTimeDifference;
#endif
    pSlaveInfo->wDlStatus               = pBusSlave->GetDlStatus();
    pSlaveInfo->wPrevPort               = pBusSlave->GetPortAtParent();
#if (defined INCLUDE_HOTCONNECT)
    pSlaveInfo->wIdentifyData           = pBusSlave->GetIdentifyData();
#endif
    pSlaveInfo->bLineCrossed            = pBusSlave->IsLineCrossedNotified();
#if( defined INCLUDE_DC_SUPPORT )
    pSlaveInfo->dwSlaveDelay            = pBusSlave->GetSlaveDelay();
    pSlaveInfo->dwPropagDelay           = pBusSlave->GetPropagDelay();
    pSlaveInfo->bIsRefClock             = ((EC_NULL != m_pDistributedClks) && (pBusSlave == m_pDistributedClks->m_pBusSlaveRefClock))?EC_TRUE:EC_FALSE;
#endif
    pSlaveInfo->bIsDeviceEmulation      = pBusSlave->IsDeviceEmulation();
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (bListLocked)
    {
        bListLocked = EC_FALSE;
        m_pBusTopology->InterLockList(EC_FALSE);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get port state of a slave

\return -
*/
EC_T_DWORD CEcMaster::GetPortState(
    EC_T_BOOL  bFixedAddress,
    EC_T_WORD  wSlaveAddress,
    EC_T_WORD* pwPortState
)
{
    EC_T_DWORD   dwRetVal    = EC_E_ERROR;
    EC_T_DWORD   dwRes       = EC_E_ERROR;
    EC_T_BOOL    bListLocked = EC_FALSE;
    EC_T_WORD    wPortState  = 0;
    CEcBusSlave* pBusSlave   = EC_NULL;
    CEcSlave*    pCfgSlave   = EC_NULL;

    /* bus scan shall be complete */
    dwRes = GetBusScanResult();
    if (dwRes == EC_E_BUSY)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    /* lock the bus slave list */
    m_pBusTopology->InterLockList(EC_TRUE);
    bListLocked = EC_TRUE;

    /* get bus and config slave pointers */
    if (bFixedAddress)
    {
        /* bus slave linked to matching config slave has highest priority */
        pBusSlave = m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
        if (EC_NULL == pBusSlave)
        {
            /* matching config slave have next priority */
            pCfgSlave = GetSlaveByAddr(wSlaveAddress);

            /* don't return and eventually matching bus slave to protect API for changing results */
        }
        else
        {
            /* get config slave linked to bus slave */
            pCfgSlave = pBusSlave->GetCfgSlave();
        }
    }
    else
    {
        /* matching bus slave has highest priority */
        pBusSlave = m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
        if (EC_NULL == pBusSlave)
        {
            /* matching config slave have next priority */
            pCfgSlave = GetSlaveByCfgAutoIncAddr(wSlaveAddress);
        }
        else
        {
            /* get config slave linked to bus slave */
            pCfgSlave = pBusSlave->GetCfgSlave();
        }
    }
    /* at least one slave pointer is needed */
    if ((EC_NULL == pBusSlave) && (EC_NULL == pCfgSlave))
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* get information from bus slave */
    if (EC_NULL != pBusSlave)
    {
        wPortState = pBusSlave->GetPortState();
    }
    /* no errors */
    *pwPortState = wPortState;
    dwRetVal     = EC_E_NOERROR;

Exit:
    if (bListLocked)
    {
        bListLocked = EC_FALSE;
        m_pBusTopology->InterLockList(EC_FALSE);
    }
    return dwRetVal;
}

#if (defined INCLUDE_MASTER_OBD)
/***************************************************************************************************/
/**
\brief  Bus load measurement cycle is complete

\return -
*/
EC_T_VOID  CEcMaster::BusLoadMeaCycleComplete(
    EC_T_VOID)
{

    /* skip values during startup etc. */
    if (m_eCurMasterLogicalState <= EC_MASTER_STATE_INIT)
    {
        m_dwBytesPerCycleCnt = 0;
        return;
    }

    /* save value of this cycle */
    m_dwBytesPerCycleAct = m_dwBytesPerCycleCnt;
    m_dwBytesPerCycleCnt = 0;

    /* calc min/max values */
    m_dwBytesPerCycleMin = EC_MIN(m_dwBytesPerCycleMin, m_dwBytesPerCycleAct);
    m_dwBytesPerCycleMax = EC_MAX(m_dwBytesPerCycleMax, m_dwBytesPerCycleAct);

    /* store it in object dictionary */
    EC_SETDWORD(&m_pSDOService->m_oBusLoadBaseObject.dwBytesPerCycleAct, m_dwBytesPerCycleAct);
    EC_SETDWORD(&m_pSDOService->m_oBusLoadBaseObject.dwBytesPerCycleMin, m_dwBytesPerCycleMin);
    EC_SETDWORD(&m_pSDOService->m_oBusLoadBaseObject.dwBytesPerCycleMax, m_dwBytesPerCycleMax);
}
#endif /* INCLUDE_MASTER_OBD */

/***************************************************************************************************/
/**
\brief  Bus load measurement second is complete
*/
EC_T_VOID  CEcMaster::MasterInfoSecondComplete(EC_T_VOID)
{
    /* skip values during startup etc. */
    if (m_eCurMasterLogicalState <= EC_MASTER_STATE_INIT)
    {
        m_dwBytesPerSecondCnt = 0;
        return;
    }

#if (defined INCLUDE_MAILBOX_STATISTICS)
    /* latch Last MailboxStatistics */
    {
        for (EC_T_DWORD dwMbxIdx = 0; dwMbxIdx < EC_MAILBOX_STATISTICS_CNT; dwMbxIdx++)
        {
            m_MailboxStatistics[dwMbxIdx].Read.Cnt.dwLast = m_MailboxStatisticsCur[dwMbxIdx].Read.dwCnt; /* Read Transfer Count Last Second */
            m_MailboxStatistics[dwMbxIdx].Read.Bytes.dwLast = m_MailboxStatisticsCur[dwMbxIdx].Read.dwBytes; /* Bytes Read Last Second */
            m_MailboxStatistics[dwMbxIdx].Write.Cnt.dwLast = m_MailboxStatisticsCur[dwMbxIdx].Write.dwCnt; /* Write Transfer Count Last Second */
            m_MailboxStatistics[dwMbxIdx].Write.Bytes.dwLast = m_MailboxStatisticsCur[dwMbxIdx].Write.dwBytes; /* Bytes Write Last Second */
        }
    }

    /* reset */
    OsMemset(&m_MailboxStatisticsCur, 0, sizeof(m_MailboxStatisticsCur));
#endif /* INCLUDE_MAILBOX_STATISTICS */

    /* bus load measurement */
#if (defined INCLUDE_MASTER_OBD)
    /* refactored from BusLoadMeaSecondComplete */
    /* save value of this cycle */
    m_dwBytesPerSecondAct = m_dwBytesPerSecondCnt;
    m_dwBytesPerSecondCnt = 0;

    /* calc min/max values */
    m_dwBytesPerSecondMin = EC_MIN(m_dwBytesPerSecondMin, m_dwBytesPerSecondAct);
    m_dwBytesPerSecondMax = EC_MAX(m_dwBytesPerSecondMax, m_dwBytesPerSecondAct);

    /* store it in object dictionary */
    EC_SETDWORD(&m_pSDOService->m_oBusLoadBaseObject.dwBytesPerSecondAct, m_dwBytesPerSecondAct);
    EC_SETDWORD(&m_pSDOService->m_oBusLoadBaseObject.dwBytesPerSecondMin, m_dwBytesPerSecondMin);
    EC_SETDWORD(&m_pSDOService->m_oBusLoadBaseObject.dwBytesPerSecondMax, m_dwBytesPerSecondMax);
#endif /* INCLUDE_MASTER_OBD */
}

#if (defined INCLUDE_VARREAD)
/********************************************************************************/
/** \brief Add a process variable information entry pointer to the the master
*
* \return EC_E_NOERROR on success
*/
EC_T_DWORD CEcMaster::AddProcessVarInfoEntry(
    EC_T_PD_VAR_INFO_INTERN*  pProcessVarInfoEntry, /**< [in]  Process variable information entry pointer to add. This entry must be located/stored in CEcConfigData */
    EC_T_BOOL                 bIsInputVar           /**< [in]  EC_TRUE: Add to the process var input entries; EC_FALSE: Add to the process var output entries */
    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    /* check parameter */
    if (EC_NULL == pProcessVarInfoEntry)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (bIsInputVar)
    {
        m_apProcVarInfoEntriesInp[m_dwProcVarInfoNumEntriesInp++] = pProcessVarInfoEntry;
    }
    else
    {
        m_apProcVarInfoEntriesOutp[m_dwProcVarInfoNumEntriesOutp++] = pProcessVarInfoEntry;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Gets the number of process variables of a specific slave.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::GetSlaveProcVarInfoNumOf(EC_T_BOOL   bFixedAddress,           /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
                                               EC_T_WORD   wSlaveAddress,           /**< [in]   Slave address */
                                               EC_T_WORD*  pwSlaveVarInfoNumOf,     /**< [out]  Number of found process variables */
                                               EC_T_WORD*  pwIndexFirst,
                                               EC_T_WORD*  pwIndexLast,
                                               EC_T_BOOL   bInputProcessVariables   /**< [in]   EC_TRUE: Number of input variables; EC_FALSE: Number of output variables */
                                              )
{
    EC_T_DWORD dwRetVal                 = EC_E_NOERROR;
    CEcSlave*  pSlave                   = EC_NULL;
    EC_T_DWORD dwProcVarInfoEntryIdx    = 0;
    EC_T_DWORD dwFixedAddr              = 0;
    EC_T_BOOL  bFound                   = EC_FALSE;

    /* check parameter */
    if (EC_NULL == pwSlaveVarInfoNumOf)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    *pwSlaveVarInfoNumOf = 0;

    if (bFixedAddress)
    {
        dwFixedAddr = wSlaveAddress;
        if(IsConfigured())      /* required because this function is already called in CEcDeviceFactory() */
        {
            /* validate fixed address */
            if(GetSlaveIdByFixedAddress(wSlaveAddress) == INVALID_SLAVE_ID)
            {
                dwRetVal = EC_E_NOTFOUND;
                goto Exit;
            }
        }
    }
    else
    {
        pSlave = GetSlaveByAutoIncAddr(wSlaveAddress);

        if (EC_NULL == pSlave)
        {
            dwRetVal = EC_E_NOTFOUND;
            goto Exit;
        }

        dwFixedAddr = pSlave->GetFixedAddr();
    }

    if (bInputProcessVariables)
    {
        for (dwProcVarInfoEntryIdx = 0; dwProcVarInfoEntryIdx < m_dwProcVarInfoNumEntriesInp; dwProcVarInfoEntryIdx++)
        {
            if (m_apProcVarInfoEntriesInp[dwProcVarInfoEntryIdx]->wFixedAddr == dwFixedAddr)
            {
                if (!bFound)
                {
                    bFound = EC_TRUE;
                    *pwIndexFirst = (EC_T_WORD)dwProcVarInfoEntryIdx;
                }
                *pwIndexLast = (EC_T_WORD)dwProcVarInfoEntryIdx;

                (*pwSlaveVarInfoNumOf)++;
            }
        }
    }
    else
    {
        for (dwProcVarInfoEntryIdx = 0; dwProcVarInfoEntryIdx < m_dwProcVarInfoNumEntriesOutp; dwProcVarInfoEntryIdx++)
        {
            if (m_apProcVarInfoEntriesOutp[dwProcVarInfoEntryIdx]->wFixedAddr == dwFixedAddr)
            {
                if (!bFound)
                {
                    bFound = EC_TRUE;
                    *pwIndexFirst = (EC_T_WORD)dwProcVarInfoEntryIdx;
                }
                *pwIndexLast = (EC_T_WORD)dwProcVarInfoEntryIdx;

                (*pwSlaveVarInfoNumOf)++;
            }
        }
    }

Exit:
    return dwRetVal;
}



/***************************************************************************************************/
/**
\brief  Gets all process variable info entries of a specific slave.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD  CEcMaster::GetSlaveProcVarInfoEntries(
    EC_T_BOOL   bFixedAddress,                   /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD   wSlaveAddress,                   /**< [in]   Slave address */
    EC_T_PROCESS_VAR_INFO* pSlaveVarInfoEntries, /**< [out]  All read process var info entries */
    EC_T_PROCESS_VAR_INFO_EX* pSlaveVarInfoEntriesEx,
    EC_T_WORD  dwMaxEntriesToRead,               /**< [in]   Max. process var info entries to read */
    EC_T_WORD* pdwReadEntries,                   /**< [in]   Read entries */
    EC_T_BOOL   bInputProcessVariables)          /**< [in]   EC_TRUE: Read input var info entries; EC_FALSE: Read output var info entries */
{
    EC_T_DWORD dwRetVal                 = EC_E_NOERROR;
    CEcSlave*  pSlave                   = EC_NULL;
    EC_T_DWORD dwIdx                    = 0;
    EC_T_DWORD dwFixedAddr              = 0;
    EC_T_PD_VAR_INFO_INTERN** apProcVarInfoEntries;
    EC_T_DWORD              dwProcVarInfoNumEntries;

    if (pdwReadEntries == EC_NULL)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    (*pdwReadEntries) = 0;

    if (bFixedAddress )
    {
        dwFixedAddr = wSlaveAddress;
    }
    else
    {
        pSlave = GetSlaveByAutoIncAddr(wSlaveAddress);
        if (EC_NULL == pSlave)
        {
            dwRetVal = EC_E_NOTFOUND;
            goto Exit;
        }
        dwFixedAddr = pSlave->GetFixedAddr();
    }

    apProcVarInfoEntries = (bInputProcessVariables) ? m_apProcVarInfoEntriesInp : m_apProcVarInfoEntriesOutp;
    dwProcVarInfoNumEntries = (bInputProcessVariables) ? m_dwProcVarInfoNumEntriesInp : m_dwProcVarInfoNumEntriesOutp;

    for (dwIdx = 0; dwIdx < dwProcVarInfoNumEntries; dwIdx++)
    {
        if (apProcVarInfoEntries[dwIdx]->wFixedAddr == dwFixedAddr)
        {
            if (pSlaveVarInfoEntries != EC_NULL)
            {
                OsMemcpy(pSlaveVarInfoEntries->szName, apProcVarInfoEntries[dwIdx]->szName, MAX_PROCESS_VAR_NAME_LEN);
                pSlaveVarInfoEntries->wDataType    = apProcVarInfoEntries[dwIdx]->wDataType;
                pSlaveVarInfoEntries->wFixedAddr   = apProcVarInfoEntries[dwIdx]->wFixedAddr;
                pSlaveVarInfoEntries->nBitSize     = apProcVarInfoEntries[dwIdx]->nBitSize;
                pSlaveVarInfoEntries->nBitOffs     = apProcVarInfoEntries[dwIdx]->nBitOffs;
                pSlaveVarInfoEntries->bIsInputData = apProcVarInfoEntries[dwIdx]->bIsInputData;
                pSlaveVarInfoEntries++;
            }
            if (pSlaveVarInfoEntriesEx != EC_NULL)
            {
                OsMemcpy(pSlaveVarInfoEntriesEx->szName, apProcVarInfoEntries[dwIdx]->szName, MAX_PROCESS_VAR_NAME_LEN_EX);
                pSlaveVarInfoEntriesEx->wDataType    = apProcVarInfoEntries[dwIdx]->wDataType;
                pSlaveVarInfoEntriesEx->wFixedAddr   = apProcVarInfoEntries[dwIdx]->wFixedAddr;
                pSlaveVarInfoEntriesEx->nBitSize     = apProcVarInfoEntries[dwIdx]->nBitSize;
                pSlaveVarInfoEntriesEx->nBitOffs     = apProcVarInfoEntries[dwIdx]->nBitOffs;
                pSlaveVarInfoEntriesEx->bIsInputData = apProcVarInfoEntries[dwIdx]->bIsInputData;
                pSlaveVarInfoEntriesEx->wIndex       = apProcVarInfoEntries[dwIdx]->wIndex;
                pSlaveVarInfoEntriesEx->wSubIndex    = apProcVarInfoEntries[dwIdx]->wSubIndex;
                pSlaveVarInfoEntriesEx->wPdoIndex    = apProcVarInfoEntries[dwIdx]->wPdoIndex;
                pSlaveVarInfoEntriesEx++;
            }
            (*pdwReadEntries)++;
        }

        if ((*pdwReadEntries) >= dwMaxEntriesToRead)
        {
            break;
        }
    }

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Finds a process variable information entry by the variable name

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD  CEcMaster::FindProcVarByName(
    EC_T_CHAR*                  szVariableName,         /**< [in]   Variable name */
    EC_T_PROCESS_VAR_INFO*      pProcessVarInfo,        /**< [out]  Process variable information entry */
    EC_T_PROCESS_VAR_INFO_EX*   pProcessVarInfoEx,      /**< [out]  Process variable information entry */
    EC_T_BOOL                   bInputProcessVariables  /**< [in]   EC_TRUE: Read INPUT var info entries; EC_FALSE: Read OUTPUT var info entries */)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_DWORD dwIdx = 0;
    EC_T_PD_VAR_INFO_INTERN** apProcVarInfoEntries;
    EC_T_DWORD dwProcVarInfoNumEntries;

    /* check parameter */
    if (EC_NULL == szVariableName)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    apProcVarInfoEntries = bInputProcessVariables ? m_apProcVarInfoEntriesInp : m_apProcVarInfoEntriesOutp;
    dwProcVarInfoNumEntries = bInputProcessVariables ? m_dwProcVarInfoNumEntriesInp : m_dwProcVarInfoNumEntriesOutp;

    for (dwIdx = 0; dwIdx < dwProcVarInfoNumEntries; dwIdx++)
    {
        if (OsStrcmp(apProcVarInfoEntries[dwIdx]->szName, szVariableName) == 0)
        {
            if (EC_NULL != pProcessVarInfo)
            {
                OsMemcpy(pProcessVarInfo->szName, apProcVarInfoEntries[dwIdx]->szName, MAX_PROCESS_VAR_NAME_LEN);
                pProcessVarInfo->wDataType    = apProcVarInfoEntries[dwIdx]->wDataType;
                pProcessVarInfo->wFixedAddr   = apProcVarInfoEntries[dwIdx]->wFixedAddr;
                pProcessVarInfo->nBitSize     = apProcVarInfoEntries[dwIdx]->nBitSize;
                pProcessVarInfo->nBitOffs     = apProcVarInfoEntries[dwIdx]->nBitOffs;
                pProcessVarInfo->bIsInputData = apProcVarInfoEntries[dwIdx]->bIsInputData;
            }

            if (EC_NULL != pProcessVarInfoEx)
            {
                OsMemcpy(pProcessVarInfoEx->szName, apProcVarInfoEntries[dwIdx]->szName, MAX_PROCESS_VAR_NAME_LEN_EX);
                pProcessVarInfoEx->wDataType    = apProcVarInfoEntries[dwIdx]->wDataType;
                pProcessVarInfoEx->wFixedAddr   = apProcVarInfoEntries[dwIdx]->wFixedAddr;
                pProcessVarInfoEx->nBitSize     = apProcVarInfoEntries[dwIdx]->nBitSize;
                pProcessVarInfoEx->nBitOffs     = apProcVarInfoEntries[dwIdx]->nBitOffs;
                pProcessVarInfoEx->bIsInputData = apProcVarInfoEntries[dwIdx]->bIsInputData;
                pProcessVarInfoEx->wIndex       = apProcVarInfoEntries[dwIdx]->wIndex;
                pProcessVarInfoEx->wSubIndex    = apProcVarInfoEntries[dwIdx]->wSubIndex;
                pProcessVarInfoEx->wPdoIndex    = apProcVarInfoEntries[dwIdx]->wPdoIndex;
            }

            break;
        }
    }
    if (dwIdx == dwProcVarInfoNumEntries)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }

Exit:
    return dwRetVal;
}

#endif

/***************************************************************************************************/
/**
\brief Gets the number of slaves determined in the last scan bus.

\return Number of bus slaves
*/
EC_T_DWORD CEcMaster::GetScanBusSlaveCount(EC_T_VOID)
{
    return m_pBusTopology->GetConnectedSlaveCount();
}


#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
/***************************************************************************************************/
/**
\brief  Gets the status of the Pass-through server.

\return Pass-through server status
*/
EC_PTS_STATE CEcMaster::PassThroughSrvGetStatus()
{
    return m_oPtsControlDesc.eCurPtsState;
}


/***************************************************************************************************/
/**
\brief Enables the Pass-through server.

\return EC_E_NOERRROR or error code.
*/
EC_T_DWORD CEcMaster::PassThroughSrvEnable(EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    CEcTimer oPtsEnableTimeout;

    /* parameter check */
    if (dwTimeout == EC_NOWAIT)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* check if the pass through server is running */
    if (m_oPtsControlDesc.eCurPtsState == ePtsStateNotRunning)
    {
        dwRetVal = EC_E_PTS_IS_NOT_RUNNING;
        goto Exit;
    }

    /* request pts enable */
    m_oPtsControlDesc.eReqPtsState = ePtsStateRunningEnabled;

    /* Wait until the pass through server is enabled */
    oPtsEnableTimeout.Start(dwTimeout);
    while( m_oPtsControlDesc.eCurPtsState != ePtsStateRunningEnabled && !oPtsEnableTimeout.IsElapsed())
    {
        OsSleep(10);
    }

    /* check if timeout has elapsed */
    if (oPtsEnableTimeout.IsElapsed())
    {
        dwRetVal = EC_E_TIMEOUT;
    }
Exit:
    return dwRetVal;
}



/***************************************************************************************************/
/**
\brief Disables the Pass-through server.

\return EC_E_NOERRROR or error code.
*/
EC_T_DWORD CEcMaster::PassThroughSrvDisable(EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    CEcTimer oPtsEnableTimeout;

    /* parameter check */
    if (dwTimeout == EC_NOWAIT)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* check if the pass through server is running */
    if (m_oPtsControlDesc.eCurPtsState == ePtsStateNotRunning)
    {
        dwRetVal = EC_E_PTS_IS_NOT_RUNNING;
        goto Exit;
    }

    /* request pts disable */
    m_oPtsControlDesc.eReqPtsState = ePtsStateRunningDisabled;

    /* Wait until the pass through is disabled */
    oPtsEnableTimeout.Start(dwTimeout);
    while( m_oPtsControlDesc.eCurPtsState != ePtsStateRunningDisabled && !oPtsEnableTimeout.IsElapsed())
    {
        OsSleep(10);
    }

    /* check if timeout has elapsed */
    if (oPtsEnableTimeout.IsElapsed())
    {
        dwRetVal = EC_E_TIMEOUT;
    }
Exit:
    return dwRetVal;
}



/***************************************************************************************************/
/**
\brief Starts the Pass Through Server

\return EC_E_NOERRROR or error code.
*/
EC_T_DWORD CEcMaster::PassThroughSrvStart(
    EC_T_PTS_SRV_START_PARMS* poPtsStartParams,
    EC_T_DWORD dwTimeout)
{
    CEcTimer oTimeout;
    EC_T_DWORD dwRetVal      = EC_E_NOERROR;
    EC_T_DWORD dwThreadPrio  = poPtsStartParams->dwPtsThreadPriority;

    /* parameter check */
    if ( poPtsStartParams == EC_NULL
      || dwTimeout == EC_NOWAIT)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* check if the pts is running */
    if (   m_oPtsControlDesc.eCurPtsState      == ePtsStateRunningDisabled
        || m_oPtsControlDesc.eCurPtsState      == ePtsStateRunningEnabled)
    {
        dwRetVal = EC_E_PTS_IS_RUNNING;
        goto Exit;
    }

    m_oPtsControlDesc.dwPtsError   = EC_E_NOERROR;

    /* reset pts control parameter */
    m_oPtsControlDesc.eCurPtsState = ePtsStateNotRunning;
    m_oPtsControlDesc.eReqPtsState = ePtsStateNone;

    /* prepare PTS start parameter  */
    m_oPtsControlDesc.poLinkLayerDrvDesc  = &m_poEcDevice->m_LinkDrvDesc;
    m_oPtsControlDesc.pvLinkLayerInstance = m_poEcDevice->m_LinkDrvDesc.pvLinkInstance;
    m_oPtsControlDesc.wPort               = poPtsStartParams->wPort;
    m_oPtsControlDesc.pMLog               = GetMemLog();  
    m_oPtsControlDesc.dwPtsThreadPriority = poPtsStartParams->dwPtsThreadPriority;
    OsMemcpy(&m_oPtsControlDesc.oIpAddr, &poPtsStartParams->oIpAddr, sizeof(m_oPtsControlDesc.oIpAddr));

    /* start the pass through server */
    m_oPtsControlDesc.pvPtsThreadHandle = OsCreateThread( (EC_T_CHAR*)"tPassThroughServer", CEcMaster::tPassThroughServer, dwThreadPrio, 0x4000, (EC_T_VOID*)&m_oPtsControlDesc);
    if (m_oPtsControlDesc.pvPtsThreadHandle == EC_NULL)
    {
        dwRetVal = EC_E_PTS_THREAD_CREATE_FAILED;
        goto Exit;
    }

    /* Wait until the pass through server has started */
    oTimeout.Start(dwTimeout);
    while(m_oPtsControlDesc.eCurPtsState == ePtsStateNotRunning
       && m_oPtsControlDesc.dwPtsError   == EC_E_NOERROR
       && !oTimeout.IsElapsed())
    {
        OsSleep(10);
    }

    /* Error while starting the pass through server */
    if (oTimeout.IsElapsed())
    {
        dwRetVal = EC_E_TIMEOUT;
        goto Exit;
    }

    dwRetVal = m_oPtsControlDesc.dwPtsError;

Exit:
    /* if the pass through server main thread could not be created
       and the pass through server was not already running, we call the stop routine to
       release all currently used resources.*/
    if (dwRetVal != EC_E_PTS_IS_RUNNING && dwRetVal != EC_E_NOERROR)
    {
        PassThroughSrvStop(dwTimeout);
    }

    return dwRetVal;
}



/***************************************************************************************************/
/**
\brief Stops the Pass Through Server

\return EC_E_NOERRROR or error code.
*/
EC_T_DWORD CEcMaster::PassThroughSrvStop(EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    CEcTimer oPtsStopTimeout;

    /* parameter check */
    if (dwTimeout == EC_NOWAIT)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* check if the pass through server is running */
    if (m_oPtsControlDesc.eCurPtsState == ePtsStateNotRunning)
    {
        dwRetVal = EC_E_PTS_IS_NOT_RUNNING;
        goto Exit;
    }

    /* stop the pass through server */
    m_oPtsControlDesc.eReqPtsState = ePtsStateNotRunning;

    /* Wait until the pass through server has stopped */
    oPtsStopTimeout.Start(dwTimeout);
    while( m_oPtsControlDesc.eCurPtsState != ePtsStateNotRunning  && !oPtsStopTimeout.IsElapsed())
    {
        OsSleep(10);
    }

    /* Check if timeout has elapsed */
    if (oPtsStopTimeout.IsElapsed())
    {
        dwRetVal = EC_E_TIMEOUT;
        goto Exit;
    }

    /* Remove the thread handle */
    OsDeleteThreadHandle(m_oPtsControlDesc.pvPtsThreadHandle);
    m_oPtsControlDesc.pvPtsThreadHandle = EC_NULL;

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief Pass through server thread

\return none
*/
EC_T_VOID CEcMaster::tPassThroughServer(EC_T_VOID* pvThreadParam)
{
#if PTS_KEEP_ALIVE
    CEcTimer oPtsKeepAliveTimeout;
#endif
    EC_T_LINK_FRAMEDESC oRxEtherCATFrame = {0};
    EC_T_PTS_ADMIN_MSG oAdminMsg         = {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, 0, 0, 0};
    EC_T_SOCKADDR oSrcAddr               = {0};
    EC_T_DWORD dwRes                     = EC_E_NOERROR;
    EC_T_LINK_DRV_DESC* pLinkDrvDesc     = EC_NULL;
    EC_T_BYTE* pbyEtherCATFrameBuffer    = EC_NULL;
    CEcSocketIp* poPtsSocket             = EC_NULL;
    EC_T_BOOL bAllocSupport              = EC_TRUE;
    EC_T_BOOL bIsFlushFramesRequired     = EC_FALSE;
    EC_T_LINK_IOCTLPARMS oLinkIoCtlParms = {0};

    PTS_CONTROL_DESC* poPtsControlDesc   = (PTS_CONTROL_DESC*)pvThreadParam;

    /* Parameter check */
    if (poPtsControlDesc == EC_NULL)
    {
        OsDbgAssert(EC_FALSE);
        poPtsControlDesc->dwPtsError = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* Parameter check */
    if ((poPtsControlDesc->pvLinkLayerInstance == EC_NULL) ||
        (poPtsControlDesc->poLinkLayerDrvDesc  == EC_NULL) ||
        (poPtsControlDesc->wPort               == 0))
    {
        poPtsControlDesc->dwPtsError = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* get the LinkLayer */
    pLinkDrvDesc = poPtsControlDesc->poLinkLayerDrvDesc;

    /* the link layer must be started in polling mode */
    if (pLinkDrvDesc->pfEcLinkGetMode(pLinkDrvDesc->pvLinkInstance) != EcLinkMode_POLLING)
    {
        poPtsControlDesc->dwPtsError = EC_E_PTS_LL_MODE_NOT_SUPPORTED;
        goto Exit;
    }

    if (EC_NULL != pLinkDrvDesc->pfEcLinkIoctl)
    {
        OsMemset(&oLinkIoCtlParms, 0, sizeof(EC_T_LINK_IOCTLPARMS));
        oLinkIoCtlParms.dwOutBufSize = sizeof(EC_T_BOOL);
        oLinkIoCtlParms.pbyInBuf    = (EC_T_PBYTE)&bIsFlushFramesRequired;
        pLinkDrvDesc->pfEcLinkIoctl(pLinkDrvDesc->pvLinkInstance, EC_LINKIOCTL_IS_FLUSHFRAMES_REQUIRED, &oLinkIoCtlParms);
    }

    /* Allocate memory for send and receive */
    pbyEtherCATFrameBuffer = (EC_T_BYTE*)OsMalloc(PTS_RECEIVE_BUFFER);
    if (pbyEtherCATFrameBuffer == EC_NULL)
    {
        poPtsControlDesc->dwPtsError = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_CORE_MASTER, poPtsControlDesc->pMLog, "tPassThroughServer:pbyEtherCATFrameBuffer", pbyEtherCATFrameBuffer, PTS_RECEIVE_BUFFER);

    /* Open a socket */
    poPtsSocket = EC_NEW(CEcSocketIp(emrassocktype_udp));
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_CORE_MASTER, poPtsControlDesc->pMLog, "CEcMaster::poPtsSocket", poPtsSocket, sizeof(CEcSocketIp));
    if (poPtsSocket == EC_NULL)
    {
        poPtsControlDesc->dwPtsError = EC_E_NOMEMORY;
        goto Exit;
    }

    /* Initialize and bind the socket */
    if (poPtsSocket->Bind(&poPtsControlDesc->oIpAddr, poPtsControlDesc->wPort) != EC_E_NOERROR)
    {
        poPtsControlDesc->dwPtsError = EC_E_PTS_SOCK_BIND_FAILED;
        goto Exit;
    }

    /* Set pass through server in running state */
    poPtsControlDesc->dwPtsError        = EC_E_NOERROR;
    poPtsControlDesc->eCurPtsState      = ePtsStateRunningDisabled;


#if PTS_KEEP_ALIVE
    oPtsKeepAliveTimeout.Start(100);
#endif

    while (poPtsControlDesc->eReqPtsState != ePtsStateNotRunning)
    {
        EC_T_DWORD dwRecvDataSize             = 0;
        EC_T_DWORD dwSrcAddrLen               = 0;
        ETHERNET_FRAME* poEthernetFrameHeader = (ETHERNET_FRAME*)pbyEtherCATFrameBuffer;

        /* pts enable state was requested */
        if (poPtsControlDesc->eReqPtsState == ePtsStateRunningEnabled)
        {
            poPtsControlDesc->dwPtsError   = EC_E_NOERROR;
            poPtsControlDesc->eReqPtsState = ePtsStateNone;
            poPtsControlDesc->eCurPtsState = ePtsStateRunningEnabled;
            continue;
        }

        /* pts disable state was requested */
        if (poPtsControlDesc->eReqPtsState == ePtsStateRunningDisabled)
        {
            poPtsControlDesc->dwPtsError   = EC_E_NOERROR;
            poPtsControlDesc->eReqPtsState = ePtsStateNone;
            poPtsControlDesc->eCurPtsState = ePtsStateRunningDisabled;
            continue;
        }

#if PTS_KEEP_ALIVE
        /* if the keep alive timeout is elapsed we send a administrative message to the source */
        if (oPtsKeepAliveTimeout.IsElapsed())
        {
            /* */
            oAdminMsg.wEtherType = PTS_BIG_ENDIAN_ADMIN_MSG_ETHERTYPE;
            oAdminMsg.wDataLength = sizeof(EC_T_PTS_ERROR_MSG);
            oAdminMsg.wMsgType = PTS_ADM_MSG_KEEP_ALIVE;

            poPtsSocket->SendData((EC_T_BYTE*)&oAdminMsg, sizeof(oAdminMsg), &oSrcAddr, sizeof(oSrcAddr));
        }
#endif

        /* Read the Ethernet frame header and the EtherCAT frame header */
        dwRecvDataSize = PTS_RECEIVE_BUFFER;
        dwSrcAddrLen   = sizeof(sockaddr);
        dwRes = poPtsSocket->RecvData((EC_T_BYTE*)poEthernetFrameHeader, &dwRecvDataSize, 1, &oSrcAddr, &dwSrcAddrLen);
        if (dwRes == EC_E_NOERROR)
        {
            /* Check the ether type of the current frame */
            if (poEthernetFrameHeader->__FrameType != PTS_ETHERCAT_ETHERTYPE &&
               poEthernetFrameHeader->__FrameType != PTS_ADMIN_MSG_ETHERTYPE )
            {
                /* No EtherCAT frame or pts admin frame (ether type != 0x88a4 && ether type != 0x88a5). We discard it */
                OsSleep(1);
                continue;
            }

            /* check if the pts is in state running disabled */
            if (poPtsControlDesc->eCurPtsState == ePtsStateRunningDisabled)
            {
#if PTS_KEEP_ALIVE
                oPtsKeepAliveTimeout.Restart();
#endif

                /* if we are in state running disabled we don't pass the received EtherCAT frame
                   through the EtherCAT bus. Instead we send a admin message to the source (ET9000) */
                oAdminMsg.wEtherType = PTS_ADMIN_MSG_ETHERTYPE;
                oAdminMsg.wDataLength = sizeof(EC_T_PTS_ERROR_MSG);
                oAdminMsg.wMsgType = PTS_ADM_MSG_ERROR;
                oAdminMsg.oAdminMsgs.oInfoMsg.dwErrorCode = EC_E_PTS_NOT_ENABLED;
                OsStrncpy(oAdminMsg.oAdminMsgs.oInfoMsg.szAdditionalMsg, EC_SZTXT_EC_E_PTS_NOT_ENABLED, PTS_MAX_MSG_LEN);

                poPtsSocket->SendData((EC_T_BYTE*)&oAdminMsg, sizeof(oAdminMsg), &oSrcAddr, sizeof(oSrcAddr));
                OsSleep(1);
                continue;
            }

            /* We are in state running enabled and have received a EtherCAT frame over UDP... */

            /* if the link layer supports frame allocation, we get the frame from the link layer. Otherwise we take the alocated buffer*/
            if (bAllocSupport)
            {
                dwRes = pLinkDrvDesc->pfEcLinkAllocSendFrame(pLinkDrvDesc->pvLinkInstance, &oRxEtherCATFrame, EC_MAX(dwRecvDataSize, ETHERNET_MIN_FRAME_LEN));
                if (EC_E_NOTSUPPORTED == dwRes)
                {
                    bAllocSupport = EC_FALSE;
                }
            }

            /* copy or assign the send data */
            if (bAllocSupport)
            {
                OsMemcpy(oRxEtherCATFrame.pbyFrame, pbyEtherCATFrameBuffer, dwRecvDataSize);
            }
            else
            {
                oRxEtherCATFrame.dwSize   = dwRecvDataSize;
                oRxEtherCATFrame.pbyFrame = (EC_T_BYTE*)pbyEtherCATFrameBuffer;
            }

            /* ...now send it through the EtherCAT bus */
            if (bAllocSupport )
            {
                dwRes = pLinkDrvDesc->pfEcLinkSendAndFreeFrame(pLinkDrvDesc->pvLinkInstance, &oRxEtherCATFrame);
            }
            else
            {
                dwRes = pLinkDrvDesc->pfEcLinkSendFrame( pLinkDrvDesc->pvLinkInstance, &oRxEtherCATFrame);
            }

            if (bIsFlushFramesRequired)
            {
                pLinkDrvDesc->pfEcLinkIoctl(pLinkDrvDesc->pvLinkInstance, EC_LINKIOCTL_FLUSHFRAMES, &oLinkIoCtlParms);
            }

            /* check if the frame send routing has returned an error. */
            if (EC_E_NOERROR == dwRes )
            {
                /* all is fine, now wait for an answer */
            }
            else
            {
#if PTS_KEEP_ALIVE
                oPtsKeepAliveTimeout.Restart();
#endif

                /* Sending of the EtherCAT frame failed now inform the source (ET9000) with a administrative message */
                oAdminMsg.wEtherType = PTS_ADMIN_MSG_ETHERTYPE;
                oAdminMsg.wDataLength = sizeof(EC_T_PTS_ERROR_MSG);
                oAdminMsg.wMsgType = PTS_ADM_MSG_ERROR;
                oAdminMsg.oAdminMsgs.oInfoMsg.dwErrorCode = EC_E_SENDFAILED;
                OsStrncpy(oAdminMsg.oAdminMsgs.oInfoMsg.szAdditionalMsg, "Pass Through Server could not send the EtherCAT frame!", PTS_MAX_MSG_LEN);

                poPtsSocket->SendData((EC_T_BYTE*)&oAdminMsg, sizeof(oAdminMsg), &oSrcAddr, sizeof(oSrcAddr));
            }
        }

        /* if the PTS is disabled we must not call the link layer received function. So we wait for the next cycle */
        if (poPtsControlDesc->eCurPtsState == ePtsStateRunningDisabled)
        {
            OsSleep(1);
            continue;
        }

        do
        {
            oRxEtherCATFrame.dwSize = 0;
            oRxEtherCATFrame.pbyFrame = EC_NULL;

            /* Receive poll the link layer to get the returning EtherCAT frame */
            dwRes = pLinkDrvDesc->pfEcLinkRecvFrame(pLinkDrvDesc->pvLinkInstance, &oRxEtherCATFrame);

            if ((EC_NULL != oRxEtherCATFrame.pbyFrame) && (0 != oRxEtherCATFrame.dwSize))
            {
#if PTS_KEEP_ALIVE
                oPtsKeepAliveTimeout.Restart();
#endif

                /* We have received the returning EtherCAT frame, now send the answer to source (e.g. ET9000) */
                poPtsSocket->SendData(oRxEtherCATFrame.pbyFrame, oRxEtherCATFrame.dwSize, &oSrcAddr, sizeof(oSrcAddr));
            }

            /* Free the received EtherCAT frame */
            if ((EC_NULL != oRxEtherCATFrame.pbyFrame) && (0 != oRxEtherCATFrame.dwSize))
            {
                pLinkDrvDesc->pfEcLinkFreeRecvFrame(pLinkDrvDesc->pvLinkInstance, &oRxEtherCATFrame);
            }
        } while ((EC_NULL != oRxEtherCATFrame.pbyFrame) && (0 != oRxEtherCATFrame.dwSize));
    }

Exit:

    /* Unbind and delete the socket */
    if (poPtsSocket != EC_NULL)
    {
        poPtsSocket->Disconnect(EC_FALSE);
        poPtsSocket->DeInitSocketLayer();
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_MASTER, poPtsControlDesc->pMLog, "tPassThroughServer:~poPtsSocket", poPtsSocket, sizeof(CEcSocketIp));
        delete poPtsSocket;
        poPtsSocket = EC_NULL;
    }

    /* Free the frame buffer  */
    if (pbyEtherCATFrameBuffer != EC_NULL)
    {
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_MASTER, poPtsControlDesc->pMLog, "tPassThroughServer:~pbyEtherCATFrameBuffer", pbyEtherCATFrameBuffer, PTS_RECEIVE_BUFFER);
        SafeOsFree(pbyEtherCATFrameBuffer);
    }

    /* temporary store the return value */
    dwRes = poPtsControlDesc->dwPtsError;

    /* Reset the PTS control structure */
    OsMemset(poPtsControlDesc,0,sizeof(PTS_CONTROL_DESC));

    /* back up the stored return value */
    poPtsControlDesc->dwPtsError = dwRes;

    /* set the pts into the state not running */
    poPtsControlDesc->eCurPtsState = ePtsStateNotRunning;
    poPtsControlDesc->eReqPtsState = ePtsStateNone;

    return;
}

#endif /* INCLUDE_PASS_THROUGH_SERVER && EC_SOCKET_IP_SUPPORTED */

#if (defined INCLUDE_VOE_SUPPORT)
/***************************************************************************************************/
/**
\brief Gets the VoE mailbox transfer object of a specific slave by the slave id

\return none
*/
EC_T_DWORD CEcMaster::GetVoeMbxTferObjBySlaveId(
    EC_T_DWORD dwSlaveID,
    EC_T_VOID** ppvVoeMbxTferObj
    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    CEcMbSlave* pVoeSlave = EC_NULL;

    *ppvVoeMbxTferObj = EC_NULL;

    /* parameter check */
    if (dwSlaveID >= m_nCfgSlaveCount)
    {
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }

    /* check if this slave is a mailbox slave to get the mailbox object */
    if (m_ppEcSlave[dwSlaveID] != EC_NULL && m_ppEcSlave[dwSlaveID]->HasMailBox())
    {
        pVoeSlave = (CEcMbSlave*)m_ppEcSlave[dwSlaveID];
        *ppvVoeMbxTferObj = pVoeSlave->GetVoeMbxTferObj();
        if (*ppvVoeMbxTferObj == EC_NULL)
        {
            dwRetVal = EC_E_NO_VOE_SUPPORT;
            goto Exit;
        }
    }
    else
    {
        dwRetVal = EC_E_NO_MBX_SUPPORT;
    }

Exit:
    return dwRetVal;
}

#endif



#if (defined INCLUDE_AOE_SUPPORT)

/***************************************************************************************************/
/**
\brief  Retrieves the NetID of a specific EtherCAT device
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMaster::GetSlaveNetId(
    EC_T_DWORD              dwSlaveId,      /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID*         poAoeNetId      /**< [out]  AoE NetID of the corresponding slave */
    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    if (poAoeNetId == EC_NULL)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (dwSlaveId >=  m_nCfgSlaveCount)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }

    if (m_ppEcSlave[dwSlaveId]->HasMailBox())
    {
        CEcMbSlave* poAoeSlave = (CEcMbSlave*)m_ppEcSlave[dwSlaveId];
        dwRetVal = poAoeSlave->GetNetId(poAoeNetId);
    }
    else
    {
        dwRetVal = EC_E_NO_MBX_SUPPORT;
        goto Exit;
    }
Exit:
    return dwRetVal;
}

#endif


/***************************************************************************************************/
/**
\brief  Generate ENI file to get into PREOP state based on scanned slaves

Mailbox slave with input and outputs:
SM 0 (mailbox out), SM 1 (mailbox in), SM 2 (outputs), SM 3 (inputs), FMMU 0 (outputs), FMMU 1 (inputs), FMMU 2 (mbx state)

Mailbox slave with outputs:
SM 0 (mailbox out), SM 1 (mailbox in), SM 2 (outputs), FMMU 0 (outputs), FMMU 1 (mbx state)

Mailbox slave with inputs:
SM 0 (mailbox out), SM 1 (mailbox in), SM 2 (inputs), FMMU 0 (inputs), FMMU 1 (mbx state)

Slave with input and outputs:
SM 0 (outputs), SM 1 (inputs), FMMU 0 (outputs), FMMU 1 (inputs)

Slave with outputs:
SM 0 (outputs), FMMU 0 (outputs)

Slave with input:
SM 0 (inputs), FMMU 0 (inputs)

\return EC_E_NOERROR on success, error code otherwise.
*/
#if (defined INCLUDE_GEN_OP_ENI)
#define NUMOFELEMENTS(table) (sizeof(table)/sizeof(table[0]))

static EC_INLINESTART EC_T_VOID InitializeEcatCmdDesc(EC_T_ECAT_INIT_CMDS_DESC* pCmdDesc, const EC_T_CHAR* szComment)
{
    OsMemset(pCmdDesc, 0, sizeof(EC_T_ECAT_INIT_CMDS_DESC));
    if (EC_NULL != szComment)
    {
        OsStrncpy(pCmdDesc->chCommentData, szComment, MAX_INITCMD_COMMENT_LEN);
        pCmdDesc->dwCommentLen = (EC_T_DWORD)OsStrlen(pCmdDesc->chCommentData);
    }
} EC_INLINESTOP

EC_T_DWORD CEcMaster::ConfigGenerateMaster(CEcConfigData* poConfigData, CEcBusSlave* poBusSlaveList)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwNumOfEoESlaves = 0;
    EC_T_WORD wLogicalMBoxStateCnt = 0;
    CEcBusSlave* pBusSlaveLoop = EC_NULL;
    EcMasterDesc* pCfgMaster = poConfigData->GetCfgMasterDesc();
    EC_T_ECAT_INIT_CMDS_DESC oEcatInitCmdDesc;

    /* determine number of EoE slaves and other values */
    for (pBusSlaveLoop = poBusSlaveList; EC_NULL != pBusSlaveLoop; pBusSlaveLoop = pBusSlaveLoop->GetNext())
    {
        EC_T_WORD wSupportedMbxProtocols = 0;
        CEcBusSlaveGenEni* pBusSlaveCur = pBusSlaveLoop->GetBusSlaveGenEni();

        if (EC_NULL == pBusSlaveCur)
        {
            OsDbgAssert(EC_FALSE);
            break;
        }

        OsDbgAssert(EC_NULL == pBusSlaveCur->GetCfgSlave());

        wSupportedMbxProtocols = pBusSlaveCur->GetMbxProtocols();
        if (wSupportedMbxProtocols & EC_MBX_PROTOCOL_EOE)
        {
            dwNumOfEoESlaves++;
        }
        /* count number of mailbox slave with FMMU for status bit */
        if (wSupportedMbxProtocols != 0)
        {
            if (0 != pBusSlaveCur->m_byLogicalMBoxState)
            {
                wLogicalMBoxStateCnt++;
            }
        }
#if (defined INCLUDE_DC_SUPPORT)
        if (0 != pBusSlaveCur->m_wDcActivation)
        {
            poConfigData->m_bDcEnabled = EC_TRUE;
        }
#endif
    }


    /* master info */
    OsMemset(pCfgMaster->oMacDest.b, 0xFF, 6);
    OsMemset(pCfgMaster->oMacSrc.b, 0xFF, 6);

    EC_SET_FRM_WORD(pCfgMaster->abyEthType, ETHERNET_FRAME_TYPE_BKHF);

    /* master mailbox states */
    if (wLogicalMBoxStateCnt != 0)
    {
        pCfgMaster->dwLogAddressMBoxStates = LOGICAL_MAILBOX_STATE_ADDR;
        pCfgMaster->wSizeAddressMBoxStates = wLogicalMBoxStateCnt;

        wLogicalMBoxStateCnt = (EC_T_WORD)WORD_ALIGN_UP(wLogicalMBoxStateCnt);
    }
    /* EoE virtual switch */
    if (dwNumOfEoESlaves != 0)
    {
        pCfgMaster->bEoE = EC_TRUE;
        pCfgMaster->dwMaxPorts = dwNumOfEoESlaves + 1;
        pCfgMaster->dwMaxFrames = 100 + (dwNumOfEoESlaves * 20);
        pCfgMaster->dwMaxMACs = 100;
    }

    /* Master init commands */
    {
        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
        EC_T_BYTE abyData[] = { 0, 0 };
        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "read slave count");
        oEcatInitCmdDesc.pbyData = abyData;
        oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
        oEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;

        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE);
        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 0);

        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BRD;
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_STATUS);

        dwRes = poConfigData->CreateECatInitCmd(&pCfgMaster->apPkdEcatInitCmdDesc, &pCfgMaster->wNumEcatInitCmds, &oEcatInitCmdDesc);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }
    {
        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
        EC_T_BYTE abyData[] = { 0, 0 };
        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "read slave count");
        oEcatInitCmdDesc.pbyData = abyData;
        oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
        oEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;

        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE);
        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 0);

        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BRD;
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_STATUS);

        dwRes = poConfigData->CreateECatInitCmd(&pCfgMaster->apPkdEcatInitCmdDesc, &pCfgMaster->wNumEcatInitCmds, &oEcatInitCmdDesc);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }
    {
        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
        EC_T_BYTE abyData[] = { 0x04, 0x00 };
        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "enable ECAT IRQ");
        oEcatInitCmdDesc.pbyData = abyData;
        oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
        oEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;

        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE);
        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_SLV_ECATEVENTMASK);

        dwRes = poConfigData->CreateECatInitCmd(&pCfgMaster->apPkdEcatInitCmdDesc, &pCfgMaster->wNumEcatInitCmds, &oEcatInitCmdDesc);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }
    {
        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
        EC_T_BYTE abyData[] = { 0, 0 };
        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear configured addresses");
        oEcatInitCmdDesc.pbyData = abyData;
        oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
        oEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;

        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE);
        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_STATION_ADDRESS);

        dwRes = poConfigData->CreateECatInitCmd(&pCfgMaster->apPkdEcatInitCmdDesc, &pCfgMaster->wNumEcatInitCmds, &oEcatInitCmdDesc);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }
    {
        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
        EC_T_BYTE abyData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear crc register");
        oEcatInitCmdDesc.pbyData = abyData;
        oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
        oEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;

        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE);
        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_SLV_RXERRCOUNTER);

        dwRes = poConfigData->CreateECatInitCmd(&pCfgMaster->apPkdEcatInitCmdDesc, &pCfgMaster->wNumEcatInitCmds, &oEcatInitCmdDesc);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }
    {
        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear fmmu");
        oEcatInitCmdDesc.wDataLen = 256;
        oEcatInitCmdDesc.bOnlyDataLen = EC_TRUE;
        oEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;

        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_IP_PI_BI_SI_OI | ECAT_INITCMD_BEFORE);
        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_FMMU_CONFIG);

        dwRes = poConfigData->CreateECatInitCmd(&pCfgMaster->apPkdEcatInitCmdDesc, &pCfgMaster->wNumEcatInitCmds, &oEcatInitCmdDesc);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }
    {
        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear sm");
        oEcatInitCmdDesc.wDataLen = 256;
        oEcatInitCmdDesc.bOnlyDataLen = EC_TRUE;
        oEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;

        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE);
        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_SYNCMANAGER_CONFIG);

        dwRes = poConfigData->CreateECatInitCmd(&pCfgMaster->apPkdEcatInitCmdDesc, &pCfgMaster->wNumEcatInitCmds, &oEcatInitCmdDesc);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }

#if (defined INCLUDE_DC_SUPPORT)
    {
        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear dc system time");
        oEcatInitCmdDesc.wDataLen = 32;
        oEcatInitCmdDesc.bOnlyDataLen = EC_TRUE;
        oEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;
    
        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE);
        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);
    
        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_SYSTEMTIME);
    
        dwRes = poConfigData->CreateECatInitCmd(&pCfgMaster->apPkdEcatInitCmdDesc, &pCfgMaster->wNumEcatInitCmds, &oEcatInitCmdDesc);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }
    {
        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
        EC_T_BYTE abyData[] = { 0 };
        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear dc cycle cfg");
        oEcatInitCmdDesc.pbyData = abyData;
        oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
        oEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;
    
        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE);
        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);
    
        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_DC_ACTIVATION_REGISTER);
    
        dwRes = poConfigData->CreateECatInitCmd(&pCfgMaster->apPkdEcatInitCmdDesc, &pCfgMaster->wNumEcatInitCmds, &oEcatInitCmdDesc);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }
    {
        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
        EC_T_BYTE abyData[] = { 0x00, 0x10 };
        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "reset dc speed");
        oEcatInitCmdDesc.pbyData = abyData;
        oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
        oEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;
    
        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE);
        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);
    
        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_SPEEDCOUNT_START);
    
        dwRes = poConfigData->CreateECatInitCmd(&pCfgMaster->apPkdEcatInitCmdDesc, &pCfgMaster->wNumEcatInitCmds, &oEcatInitCmdDesc);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }
    {
        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
        EC_T_BYTE abyData[] = { 0x00, 0x0C };
        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "configure dc filter");
        oEcatInitCmdDesc.pbyData = abyData;
        oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
        oEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;
    
        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_BEFORE);
        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);
    
        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_SYSTIMEDIFF_FILTERDEPTH);
    
        dwRes = poConfigData->CreateECatInitCmd(&pCfgMaster->apPkdEcatInitCmdDesc, &pCfgMaster->wNumEcatInitCmds, &oEcatInitCmdDesc);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }
#endif

    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

EC_T_DWORD CEcMaster::ConfigGenerateCyclic(CEcConfigData* poConfigData, EC_T_STATE eMaxEcatState)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EcCycFrameDesc* pCurCyclicFrameDesc = EC_NULL;
    EC_T_CYCLIC_ENTRY_CFG_DESC* poCyclicEntry = EC_NULL;
    EC_T_WORD wInputOutputOffs = 16;
    EC_T_WORD wDataLength = 0;

    dwRes = poConfigData->CreateCyclicEntry(&poCyclicEntry);
    if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

    dwRes = poConfigData->CreateCyclicFrame(poCyclicEntry, &pCurCyclicFrameDesc);
    if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

    poCyclicEntry->dwCycleTime = GetBusCycleTimeUsec();

    /* BRD Read AL status */
    wDataLength = 2;
    dwRes = poConfigData->GenerateCyclicFrameCmd(pCurCyclicFrameDesc,
        DEVICE_STATE_PREOP | DEVICE_STATE_SAFEOP | DEVICE_STATE_OP,
        EC_CMD_TYPE_BRD,
        0, ECREG_AL_STATUS, 0,
        wDataLength,
        (EC_T_WORD)poConfigData->GetNumSlaves(),
        wInputOutputOffs, wInputOutputOffs);
    if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    wInputOutputOffs = (EC_T_WORD)(wInputOutputOffs + wDataLength + ETYPE_EC_OVERHEAD);

    if (eEcatState_OP == eMaxEcatState)
    {
        EC_T_WORD wLogicalMBoxStateCnt = (EC_T_WORD)WORD_ALIGN_UP(poConfigData->GetCfgMasterDesc()->wSizeAddressMBoxStates);

#if (defined INCLUDE_DC_SUPPORT)
        if (poConfigData->m_bDcEnabled)
        {
            /* <Comment><![CDATA[DC; NOP; Receive Time Port 1 Register]]></Comment> */
            wDataLength = 4;
            dwRes = poConfigData->GenerateCyclicFrameCmd(pCurCyclicFrameDesc,
                DEVICE_STATE_PREOP | DEVICE_STATE_SAFEOP | DEVICE_STATE_OP,
                EC_CMD_TYPE_NOP,
                0, ECM_DCS_REC_TIMEPORT0, 0,
                wDataLength,
                0,
                wInputOutputOffs, wInputOutputOffs);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            wInputOutputOffs = (EC_T_WORD)(wInputOutputOffs + wDataLength + ETYPE_EC_OVERHEAD);

            /* <Comment><![CDATA[DC; ARMW; System Time Register]]></Comment> */
            wDataLength = 4;
            dwRes = poConfigData->GenerateCyclicFrameCmd(pCurCyclicFrameDesc,
                DEVICE_STATE_PREOP | DEVICE_STATE_SAFEOP | DEVICE_STATE_OP,
                EC_CMD_TYPE_ARMW,
                0, ECM_DCS_SYSTEMTIME, 0,
                wDataLength,
                0,
                wInputOutputOffs, wInputOutputOffs);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            wInputOutputOffs = (EC_T_WORD)(wInputOutputOffs + wDataLength + ETYPE_EC_OVERHEAD);

            /* <Comment><![CDATA[DC; APWR; System Time Register (Bus-Shift)]]></Comment> */
            wDataLength = 4;
            dwRes = poConfigData->GenerateCyclicFrameCmd(pCurCyclicFrameDesc,
                DEVICE_STATE_PREOP | DEVICE_STATE_SAFEOP | DEVICE_STATE_OP,
                EC_CMD_TYPE_APWR,
                0, ECM_DCS_SYSTEMTIME, 0,
                wDataLength,
                0,
                wInputOutputOffs, wInputOutputOffs);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            wInputOutputOffs = (EC_T_WORD)(wInputOutputOffs + wDataLength + ETYPE_EC_OVERHEAD);
        }
#endif
        if (0 != wLogicalMBoxStateCnt)
        {
            // cyclic cmd (Poll Mailbox State)
            wDataLength = (EC_T_WORD)(wLogicalMBoxStateCnt / 8);     /* convert bitlength into bytes: WORD aligned already done above */
            dwRes = poConfigData->GenerateCyclicFrameCmd(pCurCyclicFrameDesc,
                DEVICE_STATE_PREOP | DEVICE_STATE_SAFEOP | DEVICE_STATE_OP,
                EC_CMD_TYPE_LRD,
                0, 0, LOGICAL_MAILBOX_STATE_ADDR,
                wDataLength,
                0,
                wInputOutputOffs, wInputOutputOffs);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            wInputOutputOffs = (EC_T_WORD)(wInputOutputOffs + wDataLength + ETYPE_EC_OVERHEAD);
        }

        // Processdata - one LRD command
        if (poConfigData->m_dwHighestSlaveRecv > poConfigData->m_dwLowestSlaveRecv)
        {
            EC_T_DWORD dwLogicalInpLen = poConfigData->m_dwHighestSlaveRecv - poConfigData->m_dwLowestSlaveRecv;
            dwLogicalInpLen = WORD_ALIGN_UP(dwLogicalInpLen) / 8;
            dwRes = poConfigData->GenerateCyclicFrameCmd(pCurCyclicFrameDesc,
                DEVICE_STATE_SAFEOP | DEVICE_STATE_OP,
                EC_CMD_TYPE_LRD,
                0, 0, LRD_START_ADDR,
                (EC_T_WORD)dwLogicalInpLen,
                poConfigData->m_wLogicalInpWkc,
                wInputOutputOffs, wInputOutputOffs);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            wInputOutputOffs = (EC_T_WORD)(wInputOutputOffs + dwLogicalInpLen + ETYPE_EC_OVERHEAD);
        }

        // Processdata - one LWR command
        if (poConfigData->m_dwHighestSlaveSend > poConfigData->m_dwLowestSlaveSend)
        {
            EC_T_DWORD dwLogicalOutLen = poConfigData->m_dwHighestSlaveSend - poConfigData->m_dwLowestSlaveSend;
            dwLogicalOutLen = WORD_ALIGN_UP(dwLogicalOutLen) / 8;
            dwRes = poConfigData->GenerateCyclicFrameCmd(pCurCyclicFrameDesc,
                DEVICE_STATE_SAFEOP | DEVICE_STATE_OP,
                EC_CMD_TYPE_LWR,
                0, 0, LWR_START_ADDR,
                (EC_T_WORD)dwLogicalOutLen,
                poConfigData->m_wLogicalOutpWkc,
                wInputOutputOffs, wInputOutputOffs);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            wInputOutputOffs = (EC_T_WORD)(wInputOutputOffs + dwLogicalOutLen + ETYPE_EC_OVERHEAD);

        }

        /* check if more than one cyclic frame is required, if yes --> currently not supported! */
        if (wInputOutputOffs > ETHERNET_MAX_FRAME_LEN)
        {
            dwRetVal = EC_E_ENI_NO_SAFEOP_OP_SUPPORT;
            EC_ERRORMSGL(("CEcMaster::ConfigGenerate() Process data size too big for one cyclic frame (Max/Required=%d/%d)!\n",
                ETHERNET_MAX_FRAMEBUF_LEN, wInputOutputOffs));
            goto Exit;
        }
    }

    poConfigData->m_dwInputByteSize = wInputOutputOffs;
    poConfigData->m_dwOutputByteSize = wInputOutputOffs;

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_DWORD CEcMaster::ConfigGenerateProcessImage(CEcConfigData* poConfigData, CEcBusSlave* poBusSlaveList, EC_T_STATE eMaxEcatState)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_NOERROR;
    EC_T_DWORD dwSlvIdx = 0;
    CEcBusSlave* pBusSlaveLoop = EC_NULL;

    for (pBusSlaveLoop = poBusSlaveList, dwSlvIdx = 0; EC_NULL != pBusSlaveLoop; pBusSlaveLoop = pBusSlaveLoop->GetNext(), dwSlvIdx++)
    {
        EC_T_INT nIdx = 0;
        EC_T_DWORD dwBitOffs = 0;
        ECBT_T_PDOLIST* poPdoList = EC_NULL;
        CEcBusSlaveGenEni* pBusSlaveCur = pBusSlaveLoop->GetBusSlaveGenEni();

        if (EC_NULL == pBusSlaveCur)
        {
            OsDbgAssert(EC_FALSE);
            break;
        }

        OsDbgAssert(EC_NULL == pBusSlaveCur->GetCfgSlave());

        /* calculate bit offsets */
        /* inputs */
        dwBitOffs = poConfigData->GetEniSlave(dwSlvIdx)->adwPDInOffset[0];
        for (poPdoList = pBusSlaveCur->m_poTxPdoList; EC_NULL != poPdoList; poPdoList = poPdoList->pNext)
        {
            if (0xFF != poPdoList->bySmIdx)
            {
                for (nIdx = 0; nIdx < poPdoList->byEntries; nIdx++)
                {
                    ECBT_T_PDOENTRY* poPdoEntry = &poPdoList->poEntries[nIdx];

                    if (poPdoEntry->wIndex != 0)
                    {
                        poPdoEntry->wBitOffs = (EC_T_WORD)dwBitOffs;
                    }
                    dwBitOffs += poPdoEntry->byBitLen;
                }
            }
        }
        /* outputs */
        dwBitOffs = poConfigData->GetEniSlave(dwSlvIdx)->adwPDOutOffset[0];
        for (poPdoList = pBusSlaveCur->m_poRxPdoList; EC_NULL != poPdoList; poPdoList = poPdoList->pNext)
        {
            if (0xFF != poPdoList->bySmIdx)
            {
                for (nIdx = 0; nIdx < poPdoList->byEntries; nIdx++)
                {
                    ECBT_T_PDOENTRY* poPdoEntry = &poPdoList->poEntries[nIdx];

                    if (poPdoEntry->wIndex != 0)
                    {
                        poPdoEntry->wBitOffs = (EC_T_WORD)dwBitOffs;
                    }
                    dwBitOffs += poPdoEntry->byBitLen;
                }
            }
        }
#if (defined INCLUDE_VARREAD)
        /* create process var info input entries */
        for (poPdoList = pBusSlaveCur->m_poTxPdoList; EC_NULL != poPdoList; poPdoList = poPdoList->pNext)
        {
            EC_T_INT nPdInputBoundary = 0;
            EC_T_ENI_SLAVE* pEniSlave = poConfigData->GetEniSlave(dwSlvIdx);

            if (pEniSlave->wPDInEntries == 1)
            {
                /* determine upper boundary of process data */
                nPdInputBoundary = pEniSlave->adwPDInOffset[0] + pEniSlave->adwPDInSize[0];
            }

            if (0xFF != poPdoList->bySmIdx)
            {
                /* iterate throw pdo list and create process var info entries */
                for (nIdx = 0; nIdx < poPdoList->byEntries; nIdx++)
                {
                    ECBT_T_PDOENTRY* poPdoEntry = &poPdoList->poEntries[nIdx];

                    if (poPdoEntry->wIndex != 0)
                    {
                        EC_T_PD_VAR_INFO_INTERN oProcessVarInfo;
                        OsMemset(&oProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                        EcSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "Slave_%03d.0x%04X.0x%04X:%d",
                            pBusSlaveCur->GetFixedAddress(), poPdoList->wIndex, poPdoEntry->wIndex, poPdoEntry->bySubIndex);

                        oProcessVarInfo.wDataType = DEFTYPE_NULL;
                        oProcessVarInfo.nBitSize = poPdoEntry->byBitLen;
                        oProcessVarInfo.bIsInputData = EC_TRUE;

                        if (eEcatState_OP == eMaxEcatState)
                        {
                            oProcessVarInfo.nBitOffs = poPdoEntry->wBitOffs;
                            /* set addr, index etc. only if the entry is inside of slaves input process data */
                            if ((poPdoEntry->wBitOffs + poPdoEntry->byBitLen) <= nPdInputBoundary)
                            {
                                oProcessVarInfo.wFixedAddr = pBusSlaveCur->GetFixedAddress();
                                oProcessVarInfo.wIndex = poPdoEntry->wIndex;
                                oProcessVarInfo.wSubIndex = poPdoEntry->bySubIndex;
                                oProcessVarInfo.wPdoIndex = poPdoList->wIndex;
                            }
                        }
                        dwRes = poConfigData->CreateProcessVarInfoEntry(&oProcessVarInfo, EC_TRUE, EC_NULL);
                        if (EC_E_NOERROR != dwRes) { dwRes = dwRetVal; goto Exit; }
                    }
                }
            }
        }
        /* create process var info output entries */
        for (poPdoList = pBusSlaveCur->m_poRxPdoList; EC_NULL != poPdoList; poPdoList = poPdoList->pNext)
        {
            EC_T_INT nPdOutputBoundary = 0;
            EC_T_ENI_SLAVE* pEniSlave = poConfigData->GetEniSlave(dwSlvIdx);

            if (pEniSlave->wPDOutEntries == 1)
            {
                /* determine upper boundary of process data */
                nPdOutputBoundary = pEniSlave->adwPDOutOffset[0] + pEniSlave->adwPDOutSize[0];
            }

            if (0xFF != poPdoList->bySmIdx)
            {
                /* iterate throw pdo list and create process var info entries */
                for (nIdx = 0; nIdx < poPdoList->byEntries; nIdx++)
                {
                    ECBT_T_PDOENTRY* poPdoEntry = &poPdoList->poEntries[nIdx];

                    if (poPdoEntry->wIndex != 0)
                    {
                        EC_T_PD_VAR_INFO_INTERN oProcessVarInfo;
                        OsMemset(&oProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));

                        EcSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "Slave_%03d.0x%04X.0x%04X:%d",
                            pBusSlaveCur->GetFixedAddress(), poPdoList->wIndex, poPdoEntry->wIndex, poPdoEntry->bySubIndex);

                        oProcessVarInfo.wDataType = DEFTYPE_NULL;
                        oProcessVarInfo.nBitSize = poPdoEntry->byBitLen;
                        oProcessVarInfo.bIsInputData = EC_FALSE;

                        if (eEcatState_OP == eMaxEcatState)
                        {
                            oProcessVarInfo.nBitOffs = poPdoEntry->wBitOffs;
                            /* set addr, index etc. only if the entry is inside of slaves output process data */
                            if ((poPdoEntry->wBitOffs + poPdoEntry->byBitLen) <= nPdOutputBoundary)
                            {
                                oProcessVarInfo.wFixedAddr = pBusSlaveCur->GetFixedAddress();
                                oProcessVarInfo.wIndex = poPdoEntry->wIndex;
                                oProcessVarInfo.wSubIndex = poPdoEntry->bySubIndex;
                                oProcessVarInfo.wPdoIndex = poPdoList->wIndex;
                            }
                        }
                        dwRes = poConfigData->CreateProcessVarInfoEntry(&oProcessVarInfo, EC_FALSE, EC_NULL);
                        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
                    }
                }
            }
        }
#endif /* INCLUDE_VARREAD */
    }
#if (defined INCLUDE_VARREAD)
    dwRetVal = EC_E_NOERROR;
Exit:
#else
    EC_UNREFPARM(eMaxEcatState);
    EC_UNREFPARM(dwRes);
    dwRetVal = EC_E_NOERROR;
#endif /* INCLUDE_VARREAD */
    return dwRetVal;
}

EC_T_DWORD CEcMaster::ConfigGenerateEniSlave(CEcConfigData* poConfigData, CEcBusSlave* poBusSlaveList, EC_T_STATE eMaxEcatState)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_INT nIdx = 0;
    CEcBusSlave* pBusSlaveLoop = EC_NULL;
    EC_T_ECAT_INIT_CMDS_DESC oEcatInitCmdDesc;
    /* process data image */
    EC_T_WORD  wPdImageDataOutOffset = 0;
    EC_T_WORD  wPdImageDataInpOffset = 0;
    EC_T_WORD  wDataLength = 0;
    /* mailbox */
    EC_T_WORD  wLogicalMBoxStateBitAddr = 0;
    EC_T_WORD  wLogicalMBoxStateCnt = (EC_T_WORD)WORD_ALIGN_UP(poConfigData->GetCfgMasterDesc()->wSizeAddressMBoxStates);
    EC_T_DWORD dwLogicalOutAddr = (EC_T_DWORD)LWR_START_ADDR * 8; /* start address for LWR as bit offset value */
    EC_T_DWORD dwLogicalInpAddr = (EC_T_DWORD)LRD_START_ADDR * 8; /* start address for LRD as bit offset value */
#if (defined INCLUDE_DC_SUPPORT)
    EC_T_BOOL  bSetReferenceClock = (0 == poConfigData->GetNumSlaves()) ? EC_TRUE : EC_FALSE;
#endif

    OsDbgAssert(!((eEcatState_OP == eMaxEcatState) && (0 != poConfigData->GetNumSlaves())));

    /* datagram order:
    1. BRD Read AL status
    2. DC; NOP; Receive Time Port 1 Register (conditional)
    3. DC; ARMW; System Time Register (conditional)
    4. DC; APWR; System Time Register Bus-Shift (conditional)
    5. LRD Poll Mailbox State (conditional and always WORD aligned)
    6. LRD command (conditional and always WORD aligned)
    7. LWR command (conditional and always WORD aligned)
    */


    if (eEcatState_OP == eMaxEcatState)
    {
        wPdImageDataOutOffset = 16;

        /* BRD AL status: Len is always 2 bytes */
        wPdImageDataOutOffset = (EC_T_WORD)(wPdImageDataOutOffset + 2 + ETYPE_EC_OVERHEAD);

#if (defined INCLUDE_DC_SUPPORT)
        if (poConfigData->m_bDcEnabled)
        {
            //2. DC; NOP; Receive Time Port 1 Register (conditional)
            wPdImageDataOutOffset = (EC_T_WORD)(wPdImageDataOutOffset + 4 + ETYPE_EC_OVERHEAD);
            //3. DC; ARMW; System Time Register (conditional)
            wPdImageDataOutOffset = (EC_T_WORD)(wPdImageDataOutOffset + 4 + ETYPE_EC_OVERHEAD);
            //4. DC; APWR; System Time Register Bus-Shift (conditional)
            wPdImageDataOutOffset = (EC_T_WORD)(wPdImageDataOutOffset + 4 + ETYPE_EC_OVERHEAD);
        }
#endif

        /* 5. LRD Poll Mailbox State (conditional and always WORD aligned) */
        if (wLogicalMBoxStateCnt != 0)
        {
            wDataLength = (EC_T_WORD)(wLogicalMBoxStateCnt / 8);     /* convert bitlength into bytes: WORD aligned already done above */
            wPdImageDataOutOffset = (EC_T_WORD)(wPdImageDataOutOffset + wDataLength + ETYPE_EC_OVERHEAD);
        }

        wPdImageDataInpOffset = wPdImageDataOutOffset;

        /* 6. LRD command (always WORD aligned) --> calculate total size of inputs */
        wDataLength = 0;        /* in bits */
        for (pBusSlaveLoop = poBusSlaveList; EC_NULL != pBusSlaveLoop; pBusSlaveLoop = pBusSlaveLoop->GetNext())
        {
            CEcBusSlaveGenEni* pBusSlaveCur = pBusSlaveLoop->GetBusSlaveGenEni();
            EC_T_WORD          wPdInpSize = 0; // total size of process data for the whole slave

            if (EC_NULL == pBusSlaveCur)
            {
                OsDbgAssert(EC_FALSE);
                break;
            }

            // process data master inputs (Slave -> Master)
            if (pBusSlaveCur->m_bySmInpEntries != 0)
            {
                EC_T_WORD   wPdInpOffset = wDataLength;
                for (nIdx = 0; nIdx < pBusSlaveCur->m_bySmInpEntries; nIdx++)
                {
                    wPdInpSize = (EC_T_WORD)(wPdInpSize + pBusSlaveCur->m_aSmInpEntries[nIdx].wBitLen);

                    /* take care of alignment */
                    if (pBusSlaveCur->m_aSmInpEntries[nIdx].wMaxVarSize > 1)
                    {
                        wPdInpOffset = (EC_T_WORD)BYTE_ALIGN_UP(wPdInpOffset);
                    }
                }
                if (wPdInpSize > 0)
                {
                    wDataLength = (EC_T_WORD)(wPdInpOffset + wPdInpSize);
                }
            }
        }
        wDataLength = (EC_T_WORD)(WORD_ALIGN_UP(wDataLength) / 8);
        if (wDataLength != 0)
        {
            wPdImageDataOutOffset = (EC_T_WORD)(wPdImageDataOutOffset + wDataLength + ETYPE_EC_OVERHEAD);
        }

        /* convert to bits and add the 10 bytes offset */
        wPdImageDataInpOffset = (EC_T_WORD)((wPdImageDataInpOffset + 10) * 8);
        wPdImageDataOutOffset = (EC_T_WORD)((wPdImageDataOutOffset + 10) * 8);
    }

    /* begin slaves */
    for (pBusSlaveLoop = poBusSlaveList; EC_NULL != pBusSlaveLoop; pBusSlaveLoop = pBusSlaveLoop->GetNext())
    {
        CEcBusSlaveGenEni* pBusSlaveCur = pBusSlaveLoop->GetBusSlaveGenEni();
        EC_T_ENI_SLAVE*    pEniSlaveCur = EC_NULL;

        /* skip configured slaves */
        if ((EC_NULL == pBusSlaveCur) || (EC_NULL != pBusSlaveCur->GetCfgSlave()))
        {
            continue;
        }

        dwRes = poConfigData->CreateEniSlave(&pEniSlaveCur);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

        EC_T_WORD   wAutoInc = 0;
        EC_T_WORD   wStationAddr = 0;
        EC_T_WORD   wSupportedMbxProtocols = 0;
        EC_T_BOOL   bMbxStd = EC_FALSE;
        EC_T_BOOL   bMbxBoot = EC_FALSE;
        EC_T_BOOL   bDevEmul = EC_FALSE;
        EC_T_WORD   wPdOutSize = 0;                              // total size of process data for the whole slave
        EC_T_WORD   wPdInpSize = 0;                              // total size of process data for the whole slave
#if (defined INCLUDE_DC_SUPPORT)
        EC_T_UINT64 qwCycleTime0 = 0;
        EC_T_UINT64 qwCycleTime1 = 0;
        EC_T_UINT64 qwShiftTime = 0;
#endif
        if (EC_NULL == pBusSlaveCur)
        {
            OsDbgAssert(EC_FALSE);
            break;
        }
        wAutoInc = pBusSlaveCur->GetAutoIncAddress();
        bDevEmul = pBusSlaveCur->IsDeviceEmulation();
        wStationAddr = pBusSlaveCur->GetFixedAddress();
        wSupportedMbxProtocols = pBusSlaveCur->GetMbxProtocols();

        if (wSupportedMbxProtocols != 0) bMbxStd = EC_TRUE;

        EcSnprintf(pEniSlaveCur->szName, ECAT_DEVICE_NAMESIZE, "Slave_%03d", wStationAddr);
        pEniSlaveCur->wPhysAddr = wStationAddr;

        /* Ports 0, 1, 2, 3: Y=MII: K=E-Bus: Blank=Port not used */
        /* Port configuration: 00: Not implemented 01: Not configured (ESI EEPROM) 10: EBUS 11: MII / RMII (0x0007)*/
        pEniSlaveCur->wAutoIncAddr = wAutoInc;
        {
            EC_T_BYTE byPortDescriptor = pBusSlaveCur->GetPortDescriptor();
            EC_T_CHAR* pchPhysics = pEniSlaveCur->szPhysics;
            for (EC_T_INT nPortIdx = 0; nPortIdx < 4; nPortIdx++)
            {
                switch (byPortDescriptor & 3)
                {
                case 0: break;
                case 1: break;
                case 2: *(pchPhysics++) = 'K'; break;
                case 3: *(pchPhysics++) = 'Y'; break; /* 'H' */
                }
                byPortDescriptor = (EC_T_BYTE)(byPortDescriptor >> 2);
            }
        }
        pEniSlaveCur->dwVendorId = pBusSlaveCur->GetVendorId();
        pEniSlaveCur->dwProductCode = pBusSlaveCur->GetProductCode();
        pEniSlaveCur->dwRevisionNumber = pBusSlaveCur->GetRevisionNo();
        pEniSlaveCur->dwSerialNumber = pBusSlaveCur->GetSerialNo();

        if (eEcatState_OP == eMaxEcatState)
        {
            EC_T_WORD wPdInpOffset = wPdImageDataInpOffset;
            EC_T_WORD wPdOutOffset = wPdImageDataOutOffset;
            /* Send (master outputs, master -> slave) */
            if (pBusSlaveCur->m_bySmOutEntries != 0)
            {
                for (nIdx = 0; nIdx < pBusSlaveCur->m_bySmOutEntries; nIdx++)
                {
                    wPdOutSize = (EC_T_WORD)(wPdOutSize + pBusSlaveCur->m_aSmOutEntries[nIdx].wBitLen);
                    /* take care of alignment */
                    if (pBusSlaveCur->m_aSmOutEntries[nIdx].wMaxVarSize > 1)
                    {
                        wPdOutOffset = (EC_T_WORD)BYTE_ALIGN_UP(wPdOutOffset);
                        dwLogicalOutAddr = BYTE_ALIGN_UP(dwLogicalOutAddr);
                    }
                }
                if (wPdOutSize > 0)
                {
                    /* we only generate one process data <Send> entry */
                    wPdImageDataOutOffset = (EC_T_WORD)(wPdOutOffset + wPdOutSize);
                    poConfigData->m_dwLowestSlaveSend = EC_MIN(poConfigData->m_dwLowestSlaveSend, wPdOutOffset);
                    poConfigData->m_dwHighestSlaveSend = EC_MAX(poConfigData->m_dwHighestSlaveSend, (EC_T_DWORD)(wPdOutOffset + wPdOutSize));
                    pEniSlaveCur->adwPDOutOffset[pEniSlaveCur->wPDOutEntries] = wPdOutOffset;
                    pEniSlaveCur->adwPDOutSize[pEniSlaveCur->wPDOutEntries] = wPdOutSize;
                    pEniSlaveCur->wPDOutEntries++;
                }
            }
            /* Recv (master inputs, slave -> master) */
            if (pBusSlaveCur->m_bySmInpEntries != 0)
            {
                for (nIdx = 0; nIdx < pBusSlaveCur->m_bySmInpEntries; nIdx++)
                {
                    wPdInpSize = (EC_T_WORD)(wPdInpSize + pBusSlaveCur->m_aSmInpEntries[nIdx].wBitLen);
                    /* take care of alignment */
                    if (pBusSlaveCur->m_aSmInpEntries[nIdx].wMaxVarSize > 1)
                    {
                        wPdInpOffset = (EC_T_WORD)BYTE_ALIGN_UP(wPdInpOffset);
                        dwLogicalInpAddr = BYTE_ALIGN_UP(dwLogicalInpAddr);
                    }
                }
                if (wPdInpSize > 0)
                {
                    /* we only generate one process data <Recv> entry */
                    wPdImageDataInpOffset = (EC_T_WORD)(wPdInpOffset + wPdInpSize);
                    poConfigData->m_dwLowestSlaveRecv = EC_MIN(poConfigData->m_dwLowestSlaveRecv, wPdInpOffset);
                    poConfigData->m_dwHighestSlaveRecv = EC_MAX(poConfigData->m_dwHighestSlaveRecv, (EC_T_DWORD)(wPdInpOffset + wPdInpSize));
                    pEniSlaveCur->adwPDInOffset[pEniSlaveCur->wPDInEntries] = wPdInpOffset;
                    pEniSlaveCur->adwPDInSize[pEniSlaveCur->wPDInEntries] = wPdInpSize;
                    pEniSlaveCur->wPDInEntries++;
                }
            }
            /* <Sm0...> entries will be ignored in EcConfigXpat */
        }
        /* slave with mailbox ? */
        if (bMbxStd)
        {
            pEniSlaveCur->sParm.bMailbox = EC_TRUE;
            pEniSlaveCur->wMboxOutStart = pBusSlaveCur->GetStdRcvMbxOffset();
            pEniSlaveCur->wMboxOutLen = pBusSlaveCur->GetStdRcvMbxSize();
            pEniSlaveCur->wMboxInStart = pBusSlaveCur->GetStdSndMbxOffset();
            pEniSlaveCur->wMboxInLen = pBusSlaveCur->GetStdSndMbxSize();
            pEniSlaveCur->wCyclePhysicalMBoxPollingTime = 10;
            pEniSlaveCur->sParm.bCyclicPhysicalMBoxPolling = EC_TRUE;
            if ((eEcatState_OP == eMaxEcatState) && (0 != pBusSlaveCur->m_byLogicalMBoxState))
            {
                pEniSlaveCur->wSlaveLogicalAddressMBoxState = wLogicalMBoxStateBitAddr;
                pEniSlaveCur->sParm.bLogicalStateMBoxPolling = EC_TRUE;
            }


            if ((pBusSlaveCur->GetBootRcvMbx() != 0) || (pBusSlaveCur->GetBootSndMbx() != 0))
            {
                bMbxBoot = EC_TRUE;
                pEniSlaveCur->sParm.bBootStrapSupport = EC_TRUE;
#if (defined INCLUDE_FOE_SUPPORT)
                pEniSlaveCur->sParm.bCyclicPhysicalMBoxBootPolling = EC_TRUE;
#endif
                pEniSlaveCur->wMbxBootOutStart = pBusSlaveCur->GetBootRcvMbxOffset();
                pEniSlaveCur->wMbxBootOutLen = pBusSlaveCur->GetBootRcvMbxSize();
                pEniSlaveCur->wMbxBootInStart = pBusSlaveCur->GetBootSndMbxOffset();
                pEniSlaveCur->wMbxBootInLen = pBusSlaveCur->GetBootSndMbxSize();
            }
            pEniSlaveCur->sParm.bAoeSupport = (wSupportedMbxProtocols & EC_MBX_PROTOCOL_AOE) ? EC_TRUE : EC_FALSE;
            pEniSlaveCur->sParm.bEoeSupport = (wSupportedMbxProtocols & EC_MBX_PROTOCOL_EOE) ? EC_TRUE : EC_FALSE;
            pEniSlaveCur->sParm.bCoeSupport = (wSupportedMbxProtocols & EC_MBX_PROTOCOL_COE) ? EC_TRUE : EC_FALSE;
            pEniSlaveCur->sParm.bFoeSupport = (wSupportedMbxProtocols & EC_MBX_PROTOCOL_FOE) ? EC_TRUE : EC_FALSE;
            pEniSlaveCur->sParm.bSoeSupport = (wSupportedMbxProtocols & EC_MBX_PROTOCOL_SOE) ? EC_TRUE : EC_FALSE;
            pEniSlaveCur->sParm.bVoeSupport = (wSupportedMbxProtocols & EC_MBX_PROTOCOL_VOE) ? EC_TRUE : EC_FALSE;
        }

        /* begin initcmds */
        /* <Comment><![CDATA[set device state to INIT]]></Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0x00, 0x00 };
            abyData[0] = (EC_T_BYTE)(bDevEmul ? 0x01 : 0x11);
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set device state to INIT");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_I | ECAT_INITCMD_B_I | ECAT_INITCMD_S_I | ECAT_INITCMD_O_I);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_APWR;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wAutoInc);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_CONTROL);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }

#if (defined INCLUDE_DC_SUPPORT)
        if ((eEcatState_OP == eMaxEcatState) && (0 != pBusSlaveCur->m_wDcActivation))
        {
            /* <Comment><![CDATA[clear DC activation]]></Comment> */
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0x00, 0x00 };
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear DC activation");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_S_I | ECAT_INITCMD_O_I);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_APWR;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wAutoInc);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_DC_UNIT_CONTROL);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }
#endif
        /* <Comment><![CDATA[check device state for INIT]]></Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0x00, 0x00 };
            EC_T_BYTE abyValidateData[] = { 0x01, 0x00 };
            EC_T_BYTE abyValidateDataMask[] = { 0x0f, 0x00 };
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "check device state for INIT");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
            oEcatInitCmdDesc.pbyValidateData = abyValidateData;
            oEcatInitCmdDesc.wValidateDataLen = NUMOFELEMENTS(abyValidateData);
            oEcatInitCmdDesc.pbyValidateDataMask = abyValidateDataMask;
            oEcatInitCmdDesc.wValidateDataMaskLen = NUMOFELEMENTS(abyValidateDataMask);
            oEcatInitCmdDesc.wValidateTimeout = 10000;

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_I | ECAT_INITCMD_S_I | ECAT_INITCMD_O_I);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_APRD;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wAutoInc);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_STATUS);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }

        /* <Comment><![CDATA[check device state for INIT]]></Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0x00, 0x00 };
            EC_T_BYTE abyValidateData[] = { 0x01, 0x00 };
            EC_T_BYTE abyValidateDataMask[] = { 0x0f, 0x00 };
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "check device state for INIT");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
            oEcatInitCmdDesc.pbyValidateData = abyValidateData;
            oEcatInitCmdDesc.wValidateDataLen = NUMOFELEMENTS(abyValidateData);
            oEcatInitCmdDesc.pbyValidateDataMask = abyValidateDataMask;
            oEcatInitCmdDesc.wValidateDataMaskLen = NUMOFELEMENTS(abyValidateDataMask);
            oEcatInitCmdDesc.wValidateTimeout = 10000;

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_B_I);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_APRD;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wAutoInc);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_STATUS);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }

        /* <Comment><![CDATA[set device state to INIT]]></Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0x00, 0x00 };
            abyData[0] = (EC_T_BYTE)(bDevEmul ? 0x01 : 0x11);
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set device state to INIT");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_I_B);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_APWR;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wAutoInc);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_CONTROL);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }

        /* <Comment><![CDATA[check device state for INIT]]></Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0x00, 0x00 };
            EC_T_BYTE abyValidateData[] = { 0x01, 0x00 };
            EC_T_BYTE abyValidateDataMask[] = { 0x0f, 0x00 };
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "check device state for INIT");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
            oEcatInitCmdDesc.pbyValidateData = abyValidateData;
            oEcatInitCmdDesc.wValidateDataLen = NUMOFELEMENTS(abyValidateData);
            oEcatInitCmdDesc.pbyValidateDataMask = abyValidateDataMask;
            oEcatInitCmdDesc.wValidateDataMaskLen = NUMOFELEMENTS(abyValidateDataMask);
            oEcatInitCmdDesc.wValidateTimeout = 10000;

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_I_B);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_APRD;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wAutoInc);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_STATUS);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }

        if (bMbxStd)
        {
            /* <Comment><![CDATA[clear sm 0/1 (mailbox out/in)]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear sm 0/1 (mailbox out/in)");
                oEcatInitCmdDesc.wDataLen = 16;
                oEcatInitCmdDesc.bOnlyDataLen = EC_TRUE;

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc,
                    ECAT_INITCMD_I_P | ECAT_INITCMD_I_B | ECAT_INITCMD_P_I | ECAT_INITCMD_S_I | ECAT_INITCMD_O_I);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_SYNCMANAGER_CONFIG);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
            /* <Comment><![CDATA[clear sm 0/1 (mailbox out/in)]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear sm 0 / 1 (mailbox out / in)");
                oEcatInitCmdDesc.wDataLen = 16;
                oEcatInitCmdDesc.bOnlyDataLen = EC_TRUE;

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_B_I);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_APWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wAutoInc);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_SYNCMANAGER_CONFIG);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
            /* <Comment><![CDATA[set sm 0 (mailbox out)]]></Comment> */
            /* Byte 0/1: Physical start address, Byte 2/3: Length, Byte 4: Control, Byte 5: Status */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x01, 0x00 };
                EC_T_WORD wSmStart  = pBusSlaveCur->GetStdRcvMbxOffset();
                EC_T_WORD wSmLength = pBusSlaveCur->GetStdRcvMbxSize();
                abyData[0] = EC_LOBYTE(wSmStart);  abyData[1] = EC_HIBYTE(wSmStart);
                abyData[2] = EC_LOBYTE(wSmLength); abyData[3] = EC_HIBYTE(wSmLength);
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set sm 0 (mailbox out)");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_SYNCMANAGER_MBX_OUT);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
            /* <Comment><![CDATA[set sm 1 (mailbox in)]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x01, 0x00 };
                EC_T_WORD wSmStart  = pBusSlaveCur->GetStdSndMbxOffset();
                EC_T_WORD wSmLength = pBusSlaveCur->GetStdSndMbxSize();
                abyData[0] = EC_LOBYTE(wSmStart);  abyData[1] = EC_HIBYTE(wSmStart);
                abyData[2] = EC_LOBYTE(wSmLength); abyData[3] = EC_HIBYTE(wSmLength);
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set sm 1 (mailbox in)");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_SYNCMANAGER_MBX_IN);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
        }

        if (bMbxBoot)
        {
            /* <Comment><![CDATA[set sm 0 (bootstrap out)]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x01, 0x00 };
                EC_T_WORD wSmStart  = pBusSlaveCur->GetBootRcvMbxOffset();
                EC_T_WORD wSmLength = pBusSlaveCur->GetBootRcvMbxSize();
                abyData[0] = EC_LOBYTE(wSmStart);  abyData[1] = EC_HIBYTE(wSmStart);
                abyData[2] = EC_LOBYTE(wSmLength); abyData[3] = EC_HIBYTE(wSmLength);
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set sm 0 (bootstrap out)");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_B);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_SYNCMANAGER_MBX_OUT);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
            /* <Comment><![CDATA[set sm 1 (bootstrap in)]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x01, 0x00 };
                EC_T_WORD wSmStart  = pBusSlaveCur->GetBootSndMbxOffset();
                EC_T_WORD wSmLength = pBusSlaveCur->GetBootSndMbxSize();
                abyData[0] = EC_LOBYTE(wSmStart);  abyData[1] = EC_HIBYTE(wSmStart);
                abyData[2] = EC_LOBYTE(wSmLength); abyData[3] = EC_HIBYTE(wSmLength);
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set sm 1 (bootstrap in)");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_B);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_SYNCMANAGER_MBX_IN);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
        }
#if (defined INCLUDE_DC_SUPPORT)
        if ((eEcatState_OP == eMaxEcatState) && (0 != pBusSlaveCur->m_wDcActivation))
        {
            EC_T_UINT64 qwTmp;
            EC_T_WORD   wTmp;
            EC_T_DWORD  dwBusCycleTimeUsec = GetBusCycleTimeUsec();

            qwCycleTime0 = pBusSlaveCur->m_dwDcSync0CycleTime;
            if (0 == qwCycleTime0)
            {
                if (pBusSlaveCur->m_aDcSync[0].swCycleFactor >= 0)
                {
                    qwCycleTime0 = (EC_T_UINT64)(dwBusCycleTimeUsec * pBusSlaveCur->m_aDcSync[0].swCycleFactor);
                }
                else
                {
                    qwCycleTime0 = (EC_T_UINT64)(dwBusCycleTimeUsec / (-1 * pBusSlaveCur->m_aDcSync[0].swCycleFactor));
                }
            }
            if (pBusSlaveCur->m_aDcSync[1].swCycleFactor >= 0)
            {
                qwCycleTime1 = (EC_T_UINT64)(dwBusCycleTimeUsec * pBusSlaveCur->m_aDcSync[1].swCycleFactor);
            }
            else
            {
                qwCycleTime1 = (EC_T_UINT64)(dwBusCycleTimeUsec / (-1 * pBusSlaveCur->m_aDcSync[1].swCycleFactor));
            }
            /* <Comment><![CDATA[set DC cycle time]]></Comment> */
            qwTmp = EC_QWORDSWAP(qwCycleTime0);
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
                abyData[0] = EC_BYTE7(qwTmp); abyData[1] = EC_BYTE6(qwTmp);
                abyData[2] = EC_BYTE5(qwTmp); abyData[3] = EC_BYTE4(qwTmp);
                abyData[4] = EC_BYTE3(qwTmp); abyData[5] = EC_BYTE2(qwTmp);
                abyData[6] = EC_BYTE1(qwTmp); abyData[7] = EC_BYTE0(qwTmp);
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set DC cycle time");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_S);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_DC_CYCLETIME0);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
            /* <Comment><![CDATA[set DC start time]]></Comment> */
            qwShiftTime = (EC_T_UINT64)pBusSlaveCur->m_aDcSync[0].dwShiftTime;
            qwTmp = EC_QWORDSWAP(qwShiftTime);
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
                abyData[0] = EC_BYTE7(qwTmp); abyData[1] = EC_BYTE6(qwTmp);
                abyData[2] = EC_BYTE5(qwTmp); abyData[3] = EC_BYTE4(qwTmp);
                abyData[4] = EC_BYTE3(qwTmp); abyData[5] = EC_BYTE2(qwTmp);
                abyData[6] = EC_BYTE1(qwTmp); abyData[7] = EC_BYTE0(qwTmp);
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set DC start time");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_S);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_DC_WR_STARTCYCOP);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }

            /* <Comment><![CDATA[set DC activation]]></Comment> */
            wTmp = EC_WORDSWAP(pBusSlaveCur->m_wDcActivation);
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0x00, 0x00 };
                abyData[0] = EC_HIBYTE(wTmp); abyData[1] = EC_LOBYTE(wTmp);
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set DC activation");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_S);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_DC_UNIT_CONTROL);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
            /* <Comment><![CDATA[set DC latch cfg]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0, 0 };
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set DC latch cfg");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_S);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_DCL_CTRL_LATCH);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
            /* <Comment><![CDATA[clear DC activation]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0, 0 };
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear DC activation");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_DC_UNIT_CONTROL);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
        }
#endif
        /* <Comment><![CDATA[set device state to PREOP]]></Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0x02, 0x00 };
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set device state to PREOP");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_S_P | ECAT_INITCMD_O_P);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 300);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_CONTROL);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }

        /* <Comment><![CDATA[clear sms]]></Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_WORD wSmAddr = bMbxStd ? ECREG_SYNCMANAGER2 : ECREG_SYNCMANAGER0; /* for a mailbox slave use SM2 otherwise start with SM0 */
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear sms");
            oEcatInitCmdDesc.wDataLen = 16;
            oEcatInitCmdDesc.bOnlyDataLen = EC_TRUE;

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc,
                ECAT_INITCMD_I_P | ECAT_INITCMD_S_P | ECAT_INITCMD_S_I | ECAT_INITCMD_O_P | ECAT_INITCMD_O_I);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, wSmAddr);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }

        if (eEcatState_OP == eMaxEcatState)
        {
            EC_T_WORD wFmmuIdx = 0;
            EC_T_WORD wFmmuAddr = 0;
            EC_T_WORD awFmmuClear[8];
            EC_T_WORD wMbxFmmuAddr = 0;
            EC_T_WORD wSmInpIdx = 0;
            EC_T_WORD wSmOutIdx = 0;
            EC_T_WORD wNumOfOutFmmu;
            EC_T_WORD wNumOfInpFmmu;
            EC_T_WORD wSmAddr = bMbxStd ? ECREG_SYNCMANAGER2 : ECREG_SYNCMANAGER0;    /* for a mailbox slave use SM2 otherwise start with SM0 */
            
            /* <Comment><![CDATA[set sm (outputs)]]></Comment> */
            /* Byte 0/1: Physical start address, Byte 2/3: Length, Byte 4: Control, Byte 5: Status, Byte 6: Activate, Byte 7: PDI Control */
            for (nIdx = 0; nIdx < pBusSlaveCur->m_bySmOutEntries; nIdx++)
            {
                EC_T_BYTE byControl = pBusSlaveCur->m_aSmOutEntries[nIdx].byControl;
                EC_T_BYTE bySmEnabled = (EC_T_BYTE)((0 != (pBusSlaveCur->m_aSmOutEntries[nIdx].byEnable & ESC_SII_CAT_SYNCM_ENABLE_ENABLED)) ? 1 : 0);

                if (pBusSlaveCur->m_aSmOutEntries[nIdx].wBitLen == 0) bySmEnabled = 0;

                if (!(pBusSlaveCur->m_aSmOutEntries[nIdx].byEnable & ESC_SII_CAT_SYNCM_ENABLE_VIRTUAL))
                {
                    ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                    EC_T_WORD wSmStart = pBusSlaveCur->m_aSmOutEntries[nIdx].wAddr;
                    EC_T_WORD wSmLength = (EC_T_WORD)(BYTE_ALIGN_UP(pBusSlaveCur->m_aSmOutEntries[nIdx].wBitLen) / 8);
                    EC_T_BYTE abyData[] = { 0x00, 0x00, 0x00, 0x00, byControl, 0x00, bySmEnabled, 0x00 };

                    abyData[0] = EC_LOBYTE(wSmStart);  abyData[1] = EC_HIBYTE(wSmStart);
                    abyData[2] = EC_LOBYTE(wSmLength); abyData[3] = EC_HIBYTE(wSmLength);
                    InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set sm (outputs)");
                    oEcatInitCmdDesc.pbyData = abyData;
                    oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                    EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_S);
                    EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                    EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                    pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                    pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                    EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                    EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, wSmAddr);

                    dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                    if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
                }
                wSmAddr += 8;   // jump to next SM
            }

            /* <Comment><![CDATA[set sm (inputs)]]></Comment> */
            /* Byte 0/1: Physical start address, Byte 2/3: Length, Byte 4: Control, Byte 5: Status, Byte 6: Activate, Byte 7: PDI Control */
            for (nIdx = 0; nIdx < pBusSlaveCur->m_bySmInpEntries; nIdx++)
            {
                EC_T_BYTE byControl = pBusSlaveCur->m_aSmInpEntries[nIdx].byControl;
                EC_T_BYTE bySmEnabled = (EC_T_BYTE)((0 != (pBusSlaveCur->m_aSmInpEntries[nIdx].byEnable & ESC_SII_CAT_SYNCM_ENABLE_ENABLED)) ? 1 : 0);

                if (pBusSlaveCur->m_aSmInpEntries[nIdx].wBitLen == 0) bySmEnabled = 0;

                if (!(pBusSlaveCur->m_aSmInpEntries[nIdx].byEnable & ESC_SII_CAT_SYNCM_ENABLE_VIRTUAL))
                {
                    ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                    EC_T_WORD wSmStart = pBusSlaveCur->m_aSmInpEntries[nIdx].wAddr;
                    EC_T_WORD wSmLength = (EC_T_WORD)(BYTE_ALIGN_UP(pBusSlaveCur->m_aSmInpEntries[nIdx].wBitLen) / 8);
                    EC_T_BYTE abyData[] = { 0x00, 0x00, 0x00, 0x00, byControl, 0x00, bySmEnabled, 0x00 };

                    abyData[0] = EC_LOBYTE(wSmStart);  abyData[1] = EC_HIBYTE(wSmStart);
                    abyData[2] = EC_LOBYTE(wSmLength); abyData[3] = EC_HIBYTE(wSmLength);
                    InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set sm (inputs)");
                    oEcatInitCmdDesc.pbyData = abyData;
                    oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                    EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_S);
                    EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                    EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                    pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                    pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                    EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                    EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, wSmAddr);

                    dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                    if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
                }
                wSmAddr += 8;   // jump to next SM
            }

            OsMemset(awFmmuClear, 0, sizeof(awFmmuClear));
            wMbxFmmuAddr = 0;

            /* always start with FMMU 0 */
            wSmInpIdx = wSmOutIdx = 0;

            /* check if we have more than one FMMU in one direction */
            wNumOfOutFmmu = 0;
            wNumOfInpFmmu = 0;
            for (wFmmuIdx = 0, wFmmuAddr = ECREG_FMMU_CONFIG; wFmmuIdx < EC_CFG_SLAVE_PD_SECTIONS; wFmmuIdx++, wFmmuAddr += 16)
            {
                if (pBusSlaveCur->m_abyFmmu[wFmmuIdx] == ESC_SII_CAT_FMMU_OUTPUT)
                    wNumOfOutFmmu++;
                else if (pBusSlaveCur->m_abyFmmu[wFmmuIdx] == ESC_SII_CAT_FMMU_INPUT)
                    wNumOfInpFmmu++;
            }

            for (wFmmuIdx = 0, wFmmuAddr = ECREG_FMMU_CONFIG; wFmmuIdx < EC_CFG_SLAVE_PD_SECTIONS; wFmmuIdx++, wFmmuAddr += 16)
            {
                EC_T_WORD wPhyStart = 0;
                EC_T_WORD wPhyLength = 0;

                switch (pBusSlaveCur->m_abyFmmu[wFmmuIdx])
                {
                case 0:     /* not used */
                case 0xFF:
                    break;
                case ESC_SII_CAT_FMMU_OUTPUT:
                    if (wPdOutSize != 0)
                    {
                        EC_T_WORD wLogicalStartbit = 0;
                        EC_T_WORD wLogicalEndbit = 0;
                        EC_T_WORD wBitLen = 0;
                        EC_T_DWORD dwEndBit = 0;

                        if (wNumOfOutFmmu == 1)
                            wBitLen = wPdOutSize;
                        else
                            wBitLen = pBusSlaveCur->m_aSmOutEntries[wSmOutIdx].wBitLen;

                        dwEndBit = dwLogicalOutAddr + wBitLen - 1;
                        wLogicalStartbit = (EC_T_WORD)(dwLogicalOutAddr % 8);
                        wLogicalEndbit = (EC_T_WORD)(dwEndBit % 8);

                        wPhyStart = pBusSlaveCur->m_aSmOutEntries[wSmOutIdx].wAddr;
                        if (wBitLen != 0)
                            wPhyLength = (EC_T_WORD)(((wLogicalStartbit + wBitLen - 1) / 8) - (wLogicalStartbit / 8) + 1);
                        else
                            wPhyLength = 0;

                        EC_T_DWORD lAddr = dwLogicalOutAddr / 8;
                        /* <Comment><![CDATA[set fmmu (outputs)]]></Comment> */
                        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                        EC_T_BYTE abyData[] = {
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
                        };
                        /* logical start address (4 Byte) */
                        abyData[0] = (EC_T_BYTE)lAddr;            abyData[1] = (EC_T_BYTE)(lAddr >> 8);
                        abyData[2] = (EC_T_BYTE)(lAddr >> 16);    abyData[3] = (EC_T_BYTE)(lAddr >> 24);
                        /* Length in bytes (2 Byte) */
                        abyData[4] = (EC_T_BYTE)wPhyLength;       abyData[5] = (EC_T_BYTE)(wPhyLength >> 8);
                        /* Logical start bit, Logical end bit */
                        abyData[6] = (EC_T_BYTE)wLogicalStartbit; abyData[7] = (EC_T_BYTE)wLogicalEndbit;
                        /* Physical Address of first SyncMgr for this direction (2 Byte) */
                        abyData[8] = (EC_T_BYTE)wPhyStart;        abyData[9] = (EC_T_BYTE)(wPhyStart >> 8);
                        abyData[12] = (EC_T_BYTE)((wBitLen != 0) ? 0x01 : 0x00);
                        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set fmmu (outputs)");
                        oEcatInitCmdDesc.pbyData = abyData;
                        oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_S);
                        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, wFmmuAddr);

                        dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

                        awFmmuClear[wFmmuIdx] = wFmmuAddr;
                        dwLogicalOutAddr += wBitLen;
                        if (wSmOutIdx == 0) { poConfigData->m_wLogicalOutpWkc++; }          // for expected WKC
                        wSmOutIdx++;
                    }
                    break;

                case ESC_SII_CAT_FMMU_INPUT:
                    if (wPdInpSize != 0)
                    {
                        EC_T_WORD wLogicalStartbit = 0;
                        EC_T_WORD wLogicalEndbit = 0;
                        EC_T_WORD wBitLen;
                        EC_T_DWORD dwEndBit;

                        if (wNumOfInpFmmu == 1)
                            wBitLen = wPdInpSize;
                        else
                            wBitLen = pBusSlaveCur->m_aSmInpEntries[wSmInpIdx].wBitLen;

                        dwEndBit = dwLogicalInpAddr + wBitLen - 1;
                        wLogicalStartbit = (EC_T_WORD)(dwLogicalInpAddr % 8);
                        wLogicalEndbit = (EC_T_WORD)(dwEndBit % 8);

                        wPhyStart = pBusSlaveCur->m_aSmInpEntries[wSmInpIdx].wAddr;
                        if (wBitLen != 0)
                            wPhyLength = (EC_T_WORD)(((wLogicalStartbit + wBitLen - 1) / 8) - (wLogicalStartbit / 8) + 1);
                        else
                            wPhyLength = 0;

                        EC_T_DWORD lAddr = dwLogicalInpAddr / 8;
                        /* <Comment><![CDATA[set fmmu (inputs)]]></Comment> */
                        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                        EC_T_BYTE abyData[] = {
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
                        };
                        /* logical start address (4 Byte) */
                        abyData[0] = (EC_T_BYTE)lAddr;            abyData[1] = (EC_T_BYTE)(lAddr >> 8);
                        abyData[2] = (EC_T_BYTE)(lAddr >> 16);    abyData[3] = (EC_T_BYTE)(lAddr >> 24);
                        /* Length in bytes (2 Byte) */
                        abyData[4] = (EC_T_BYTE)wPhyLength;       abyData[5] = (EC_T_BYTE)(wPhyLength >> 8);
                        /* Logical start bit, Logical end bit */
                        abyData[6] = (EC_T_BYTE)wLogicalStartbit; abyData[7] = (EC_T_BYTE)wLogicalEndbit;
                        /* Physical Address of first SyncMgr for this direction (2 Byte) */
                        abyData[8] = (EC_T_BYTE)wPhyStart;        abyData[9] = (EC_T_BYTE)(wPhyStart >> 8);
                        abyData[12] = (EC_T_BYTE)((wBitLen != 0) ? 0x01 : 0x00);
                        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set fmmu (inputs)");
                        oEcatInitCmdDesc.pbyData = abyData;
                        oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_S);
                        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, wFmmuAddr);

                        dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

                        awFmmuClear[wFmmuIdx] = wFmmuAddr;
                        dwLogicalInpAddr += wBitLen;
                        if (wSmInpIdx == 0) { poConfigData->m_wLogicalInpWkc++; }
                        wSmInpIdx++;
                    }
                    break;

                case ESC_SII_CAT_FMMU_MAILBOX:
                    /* for a mailbox slave setup the FMMU for the status bit */
                    if (bMbxStd)
                    {
                        /* <Comment><![CDATA[set fmmu 2 (mailbox state)]]></Comment> */
                        EC_T_DWORD absBitoffs = LOGICAL_MAILBOX_STATE_ADDR * 8 + wLogicalMBoxStateBitAddr;
                        EC_T_DWORD endBit = absBitoffs + 1 /* Bitlen: 1 */ - 1;
                        EC_T_DWORD logicalStartByte = BYTE_ALIGN_DOWN(absBitoffs) / 8;
                        EC_T_DWORD logicalEndByte = BYTE_ALIGN_DOWN(endBit) / 8;
                        EC_T_DWORD byteLength = logicalEndByte - logicalStartByte + 1;
                        EC_T_DWORD logicalStartbit = (EC_T_DWORD)(absBitoffs % 8);
                        EC_T_DWORD logicalEndbit = (EC_T_DWORD)(endBit % 8);

                        ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                        EC_T_BYTE abyData[] = {
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x0D, 0x08, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00
                        };
                        /* logical start address (4 Byte) */
                        abyData[0] = (EC_T_BYTE)logicalStartByte;         abyData[1] = (EC_T_BYTE)(logicalStartByte >> 8);
                        abyData[2] = (EC_T_BYTE)(logicalStartByte >> 16); abyData[3] = (EC_T_BYTE)(logicalStartByte >> 24);
                        /* Length in bytes (2 Byte) */
                        abyData[4] = (EC_T_BYTE)byteLength;               abyData[5] = (EC_T_BYTE)(byteLength >> 8);
                        /* Logical start bit, Logical end bit */
                        abyData[6] = (EC_T_BYTE)logicalStartbit;          abyData[7] = (EC_T_BYTE)logicalEndbit;
                        InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set fmmu 2 (mailbox state)");
                        oEcatInitCmdDesc.pbyData = abyData;
                        oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                        EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_I_B);
                        EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                        EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                        pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, wFmmuAddr);

                        dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

                        wMbxFmmuAddr = wFmmuAddr;
                        wLogicalMBoxStateBitAddr++;
                    }
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                    break;
                }
            }
            /* <Comment><![CDATA[set device state to SAFEOP]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0x04, 0x00 };
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set device state to SAFEOP");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_O_S);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_CONTROL);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }

            for (wFmmuIdx = 0; wFmmuIdx < 8; wFmmuIdx++)
            {
                if (awFmmuClear[wFmmuIdx] != 0)
                {
                    /* <Comment><![CDATA[clear fmmu X]]></Comment> */
                    ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                    InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear fmmu X");
                    oEcatInitCmdDesc.wDataLen = 16;
                    oEcatInitCmdDesc.bOnlyDataLen = EC_TRUE;

                    EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc,
                        ECAT_INITCMD_S_P | ECAT_INITCMD_S_I | ECAT_INITCMD_O_P | ECAT_INITCMD_O_I);
                    EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
                    EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                    pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                    pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                    EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                    EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, awFmmuClear[wFmmuIdx]);

                    dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                    if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
                }
            }

            if (wMbxFmmuAddr != 0)
            {
                /* <Comment><![CDATA[clear fmmu X]]></Comment> */
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear fmmu X");
                oEcatInitCmdDesc.wDataLen = 16;
                oEcatInitCmdDesc.bOnlyDataLen = EC_TRUE;

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc,
                    ECAT_INITCMD_P_I | ECAT_INITCMD_B_I | ECAT_INITCMD_S_I | ECAT_INITCMD_O_I);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, wMbxFmmuAddr);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
#if (defined INCLUDE_DC_SUPPORT)
            if (0 != pBusSlaveCur->m_wDcActivation)
            {
                /* <Comment><![CDATA[clear DC activation]]></Comment> */
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0, 0 };
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "clear DC activation");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_S_P | ECAT_INITCMD_O_P);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_DC_UNIT_CONTROL);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
#endif
            /* <Comment><![CDATA[check device state for PREOP]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0, 0, 0, 0, 0, 0 };
                EC_T_BYTE abyValidateData[] = { 0x02, 0, 0, 0, 0, 0 };
                EC_T_BYTE abyValidateDataMask[] = { 0x0f, 0, 0, 0, 0, 0 };
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "check device state for PREOP");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
                oEcatInitCmdDesc.pbyValidateData = abyValidateData;
                oEcatInitCmdDesc.wValidateDataLen = NUMOFELEMENTS(abyValidateData);
                oEcatInitCmdDesc.pbyValidateDataMask = abyValidateDataMask;
                oEcatInitCmdDesc.wValidateDataMaskLen = NUMOFELEMENTS(abyValidateDataMask);
                oEcatInitCmdDesc.wValidateTimeout = 5000;

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_S_P | ECAT_INITCMD_O_P);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_STATUS);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
        }
        /* <Comment><![CDATA[assign EEPROM to PDI]]</Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0x01 };
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "assign EEPROM to PDI");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_I_B);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_SB_EEP_SLV_PDIACCSTATE);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }
        /* <Comment><![CDATA[set device state to PREOP]]></Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0x00, 0x00 };
            abyData[0] = (EC_T_BYTE)(bDevEmul ? 0x02 : 0x12);
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set device state to PREOP");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 300);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_CONTROL);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }
        /* <Comment><![CDATA[check device state for PREOP]]></Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0, 0, 0, 0, 0, 0 };
            EC_T_BYTE abyValidateData[] = { 0x02, 0, 0, 0, 0, 0 };
            EC_T_BYTE abyValidateDataMask[] = { 0x1f, 0, 0, 0, 0, 0 };
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "check device state for PREOP");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
            oEcatInitCmdDesc.pbyValidateData = abyValidateData;
            oEcatInitCmdDesc.wValidateDataLen = NUMOFELEMENTS(abyValidateData);
            oEcatInitCmdDesc.pbyValidateDataMask = abyValidateDataMask;
            oEcatInitCmdDesc.wValidateDataMaskLen = NUMOFELEMENTS(abyValidateDataMask);
            oEcatInitCmdDesc.wValidateTimeout = 10000;

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_STATUS);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }
        /* <Comment><![CDATA[assign EEPROM back to ECAT]]></Comment> */
        if (m_bGenEniAssignEepromBackToEcat)
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0 };
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "assign EEPROM back to ECAT");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_P | ECAT_INITCMD_B_I | ECAT_INITCMD_I_I);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_APWR;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wAutoInc);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_SB_EEP_SLV_PDIACCSTATE);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }
        /* <Comment><![CDATA[set device state to BOOT]]></Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0x13, 0x00 };
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set device state to BOOT");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_B);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_CONTROL);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }
        /* <Comment><![CDATA[check device state for BOOT]]></Comment> */
        {
            ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
            EC_T_BYTE abyData[] = { 0, 0, 0, 0, 0, 0 };
            EC_T_BYTE abyValidateData[] = { 0x03, 0, 0, 0, 0, 0 };
            EC_T_BYTE abyValidateDataMask[] = { 0x1f, 0, 0, 0, 0, 0 };
            InitializeEcatCmdDesc(&oEcatInitCmdDesc, "check device state for BOOT");
            oEcatInitCmdDesc.pbyData = abyData;
            oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
            oEcatInitCmdDesc.pbyValidateData = abyValidateData;
            oEcatInitCmdDesc.wValidateDataLen = NUMOFELEMENTS(abyValidateData);
            oEcatInitCmdDesc.pbyValidateDataMask = abyValidateDataMask;
            oEcatInitCmdDesc.wValidateDataMaskLen = NUMOFELEMENTS(abyValidateDataMask);
            oEcatInitCmdDesc.wValidateTimeout = 30000;

            EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_I_B);
            EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
            EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

            pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_STATUS);

            dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }

        if (eEcatState_OP == eMaxEcatState)
        {
            /* <Comment><![CDATA[set device state to SAFEOP]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0x04, 0x00 };
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set device state to SAFEOP");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_S);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_CONTROL);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
            /* <Comment><![CDATA[check device state for SAFEOP]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0, 0, 0, 0, 0, 0 };
                EC_T_BYTE abyValidateData[] = { 0x04, 0, 0, 0, 0, 0 };
                EC_T_BYTE abyValidateDataMask[] = { 0x1f, 0, 0, 0, 0, 0 };
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "check device state for SAFEOP");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
                oEcatInitCmdDesc.pbyValidateData = abyValidateData;
                oEcatInitCmdDesc.wValidateDataLen = NUMOFELEMENTS(abyValidateData);
                oEcatInitCmdDesc.pbyValidateDataMask = abyValidateDataMask;
                oEcatInitCmdDesc.wValidateDataMaskLen = NUMOFELEMENTS(abyValidateDataMask);
                oEcatInitCmdDesc.wValidateTimeout = 10000;

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_P_S);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_STATUS);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
            /* <Comment><![CDATA[check device state for SAFEOP]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0, 0, 0, 0, 0, 0 };
                EC_T_BYTE abyValidateData[] = { 0x04, 0, 0, 0, 0, 0 };
                EC_T_BYTE abyValidateDataMask[] = { 0x0f, 0, 0, 0, 0, 0 };
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "check device state for SAFEOP");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
                oEcatInitCmdDesc.pbyValidateData = abyValidateData;
                oEcatInitCmdDesc.wValidateDataLen = NUMOFELEMENTS(abyValidateData);
                oEcatInitCmdDesc.pbyValidateDataMask = abyValidateDataMask;
                oEcatInitCmdDesc.wValidateDataMaskLen = NUMOFELEMENTS(abyValidateDataMask);
                oEcatInitCmdDesc.wValidateTimeout = 200;

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_O_S);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_STATUS);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
            /* <Comment><![CDATA[set device state to OP]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0x08, 0x00 };
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "set device state to OP");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_S_O);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_CONTROL);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
            /* <Comment><![CDATA[check device state for OP]]></Comment> */
            {
                ETYPE_EC_CMD_HEADER* pCmdHdr = EC_NULL;
                EC_T_BYTE abyData[] = { 0, 0, 0, 0, 0, 0 };
                EC_T_BYTE abyValidateData[] = { 0x08, 0, 0, 0, 0, 0 };
                EC_T_BYTE abyValidateDataMask[] = { 0x1f, 0, 0, 0, 0, 0 };
                InitializeEcatCmdDesc(&oEcatInitCmdDesc, "check device state for OP");
                oEcatInitCmdDesc.pbyData = abyData;
                oEcatInitCmdDesc.wDataLen = NUMOFELEMENTS(abyData);
                oEcatInitCmdDesc.pbyValidateData = abyValidateData;
                oEcatInitCmdDesc.wValidateDataLen = NUMOFELEMENTS(abyValidateData);
                oEcatInitCmdDesc.pbyValidateDataMask = abyValidateDataMask;
                oEcatInitCmdDesc.wValidateDataMaskLen = NUMOFELEMENTS(abyValidateDataMask);
                oEcatInitCmdDesc.wValidateTimeout = 10000;

                EC_ECINITCMDDESC_SET_TRANSITION(&oEcatInitCmdDesc.EcInitCmdsDesc, ECAT_INITCMD_S_O);
                EC_ECINITCMDDESC_SET_CNT(&oEcatInitCmdDesc.EcInitCmdsDesc, 1);
                EC_ECINITCMDDESC_SET_RETRIES(&oEcatInitCmdDesc.EcInitCmdsDesc, 3);

                pCmdHdr = &oEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wStationAddr);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECREG_AL_STATUS);

                dwRes = poConfigData->CreateECatInitCmd(&pEniSlaveCur->apPkdEcatInitCmdDesc, &pEniSlaveCur->wNumEcatInitCmds, &oEcatInitCmdDesc);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            }
        }
        /* end initcmds */

        /* previous port entry */
        if (pBusSlaveCur != m_pBusTopology->GetBusSlaveList())
        {
            CEcBusSlave* pBusSlavePrev = EC_NULL;
            EC_T_WORD    wPrevPort = ESC_PORT_INVALID;

#if (defined INCLUDE_RED_DEVICE)
            if (m_bRedLineBreak && (m_wRedNumSlavesOnMain == pBusSlaveCur->GetBusIndex()))
            {
                /* slave is at line break attach it virtualy to previous one */
                pBusSlavePrev = pBusSlaveCur->GetPrev();
                wPrevPort = ESC_PORT_B;
            }
            else
#endif /* INCLUDE_RED_DEVICE */
                if (EC_NULL != pBusSlaveCur->GetParentSlave())
                {
                    pBusSlavePrev = pBusSlaveCur->GetParentSlave();
                    wPrevPort = pBusSlaveCur->GetPortAtParent();
                }
                else
                {
                    EC_ERRORMSGC(("Unsupported topology!"));
                }
            if (EC_NULL != pBusSlavePrev)
            {
                if (ESC_PORT_INVALID == wPrevPort)
                {
                    EC_ERRORMSGC(("Unsupported topology!"));
                    wPrevPort = ESC_PORT_B;
                }
                pEniSlaveCur->wNumPreviousPorts = 1;
                pEniSlaveCur->aoPreviousPort[pEniSlaveCur->wNumPreviousPorts - 1].wPortNumber = wPrevPort;
                pEniSlaveCur->aoPreviousPort[pEniSlaveCur->wNumPreviousPorts - 1].wSlaveAddress = pBusSlavePrev->GetFixedAddress();
            }
        }
#if (defined INCLUDE_DC_SUPPORT)
        if ((eEcatState_OP == eMaxEcatState) && (0 != pBusSlaveCur->m_wDcActivation))
        {
            pEniSlaveCur->sParm.bDc = EC_TRUE;
            pEniSlaveCur->dwDcRegisterCycleTime0 = EC_LODWORD(qwCycleTime0);
            pEniSlaveCur->dwDcRegisterCycleTime1 = EC_LODWORD(qwCycleTime1);
            /* shift time ignored in EcConfigXpat */
            pEniSlaveCur->bDcReferenceClock = bSetReferenceClock; /* first DC slave as ref clock */
            bSetReferenceClock = EC_FALSE;
        }
#endif
        /* end slave */
        if (0 != pEniSlaveCur->wPhysAddr)
        {
            /* reconfigure array of free address ranges for BT Temporary address generation */
            poConfigData->RemoveAddressFromRange(pEniSlaveCur->wPhysAddr);
        }
    } /* end slave loop */

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_DWORD CEcMaster::ConfigGenerate(
    CEcConfigData*  poConfigData,
    CEcBusSlave*    poBusSlaveList,
    EC_T_STATE      eMaxEcatState,
    EC_T_BOOL       bExtendConfig
    )
{
    EC_T_DWORD  dwRetVal = EC_E_NOERROR;
    
    if ((EC_NULL == poConfigData) || (bExtendConfig && (eEcatState_OP == eMaxEcatState)))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    if (bExtendConfig)
    {
        dwRetVal = ConfigGenerateEniSlave(poConfigData, poBusSlaveList, eMaxEcatState);
        if (EC_E_NOERROR != dwRetVal) { goto Exit; }
    }
    else
    {
        poConfigData->Initialize();

        dwRetVal = ConfigGenerateMaster(poConfigData, poBusSlaveList);
        if (EC_E_NOERROR != dwRetVal) { goto Exit; }

        dwRetVal = ConfigGenerateEniSlave(poConfigData, poBusSlaveList, eMaxEcatState);
        if (EC_E_NOERROR != dwRetVal) { goto Exit; }

        /* cyclic */
        dwRetVal = ConfigGenerateCyclic(poConfigData, eMaxEcatState);
        if (EC_E_NOERROR != dwRetVal) { goto Exit; }

        /* process image */
        dwRetVal = ConfigGenerateProcessImage(poConfigData, poBusSlaveList, eMaxEcatState);
        if (EC_E_NOERROR != dwRetVal) { goto Exit; }
    }

Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief Apply extended configuration to the master
*
*   Swaps preallocated config/mailbox slave storage with the old ones in the master.
*
*/
EC_T_VOID CEcMaster::ConfigExtendApply(
    CEcSlave***   papoSlaves,       /**<[in/out] Pointer to (new/old) slave storage */
    EC_T_WORD     wSlaveCnt,        /**<[in] Number of config slaves in slave storage */
    CEcMbSlave*** papoMbSlaves,     /**<[in/out] Pointer to (new/old) mailbox slave storage */
    EC_T_WORD     wMbSlaveCnt,      /**<[in] Number of mailbox slaves in slave storage */
    EC_T_WORD*    pwMaxNumSlaves    /**<[in/out] Pointer to Maximum number of slaves in slave storage */
    )
{
    /* backup slave storage pointers */
    CEcSlave**   ppEcSlaveOld     = m_ppEcSlave;
    CEcMbSlave** ppEcMbSlaveOld   = m_ppEcMbSlave;
    EC_T_WORD    wMaxNumSlavesOld = (EC_T_WORD)m_wMaxNumSlaves;

    OsDbgAssert(0 == m_pSlavesStateMachineFifo->GetCount());
    
    /* swap config slave storages */
    m_ppEcSlave      = *papoSlaves;
    m_nCfgSlaveCount = wSlaveCnt;
    m_ppEcMbSlave    = *papoMbSlaves;
    m_nEcMbSlave     = wMbSlaveCnt;

    m_wMaxNumSlaves  = *pwMaxNumSlaves;

    /* return old slave storages */
    *papoSlaves     = ppEcSlaveOld;
    *papoMbSlaves   = ppEcMbSlaveOld;
    *pwMaxNumSlaves = wMaxNumSlavesOld;
}   

#endif /* INCLUDE_GEN_OP_ENI */

/********************************************************************************/
/** \brief Adjust Cyclic Frames according to master state
*
*  For eCycFrameLayout_STANDARD and eCycFrameLayout_DYNAMIC, the cyclic commands should be activated according
*  to the state and the frames should be prepared with the FrameIdx in the first
*  command and the correct NEXTCMD flag should be set
*
*  For eCycFrameLayout_FIXED and eCycFrameLayout_IN_DMA, the commands should be only activated according to
*  the state
*
* \return N/A
*/
EC_T_VOID  CEcMaster::AdjustCycFramesToState(EC_T_WORD wState)
{
#ifdef INCLUDE_LOG_MESSAGES
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
    CYCLIC_ENTRY_MGMT_DESC* pMgmtDesc;
    EcCycFrameDesc*         pCycFrameDesc;
    EC_T_BYTE*              pbyFrame;
    EcCycCmdConfigDesc*     pCmdDesc;
    EC_T_WORD               wFrmIdx;
    EC_T_WORD               wCmdIdx;
    EC_T_DWORD              dwCycEntryIndex;

    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+NOTIFICATION), "AdjustCycFramesToState(%s)\n", ecatStateToStr((DEVICE_STATE_UNKNOWN == wState)?eEcatState_UNKNOWN:(EC_T_STATE)wState)));

    /* walk through all cyclic entries */
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    for (dwCycEntryIndex = 0; dwCycEntryIndex < m_dwNumCycConfigEntries; dwCycEntryIndex++)
#else
    dwCycEntryIndex = 0;
#endif
    {
        /* get cyclic entry */
        pMgmtDesc = &m_aCyclicEntryMgmtDesc[dwCycEntryIndex];

        /* walk through all frames */
        for (wFrmIdx = 0; wFrmIdx < pMgmtDesc->wNumFrames; wFrmIdx++)
        {
        EcCycCmdConfigDesc* pCmdDescFirst = EC_NULL;
        EcCycCmdConfigDesc* pCmdDescLast  = EC_NULL;

            /* get frame descriptor */
            pCycFrameDesc = pMgmtDesc->apEcCycFrameDesc[wFrmIdx];

            if (m_eCycFrameLayout == eCycFrameLayout_IN_DMA)
            {
                pbyFrame = GetPDOutPtr();
            }
            else
            {
                pbyFrame = pCycFrameDesc->pbyFrame;
            }

            /* walk through all commands */
            for (wCmdIdx = 0; wCmdIdx < pCycFrameDesc->wCmdCnt; wCmdIdx++)
            {
                /* get command descriptor */
                pCmdDesc = pCycFrameDesc->apCycCmd[wCmdIdx];

                /* command should be activated according the state */
                pCmdDesc->bActive = (0 != (pCmdDesc->wConfOpStatesMask & wState));

#if (defined INCLUDE_HOTCONNECT)
                if (eCycFrameLayout_DYNAMIC == m_eCycFrameLayout)
                {
                    /* disable command if recalculated WKC is zero (no slave is present)  */
                    /* except BRD AL status to assure correct master stack functionality */
                    if (pCmdDesc->bCheckWkc && (0 == pCmdDesc->wExpectedWkc) && !pCmdDesc->bReadAlStatus)
                    {
                        pCmdDesc->bActive = EC_FALSE;
                    }
                }
#endif /* INCLUDE_HOTCONNECT */

                switch (m_eCycFrameLayout)
                {
                case eCycFrameLayout_DYNAMIC:
                case eCycFrameLayout_STANDARD:
                    /* skip inactive command */
                    if (!pCmdDesc->bActive)
                    {
                        continue;
                    }
                    /* reset index field and set NEXT field */
                    pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byIdx = 0;
                    EC_ICMDHDR_SET_LEN_NEXT(&pCmdDesc->EcCmdHeader, EC_TRUE);

                    /* track first command */
                    if (EC_NULL == pCmdDescFirst)
                    {
                        pCmdDescFirst = pCmdDesc;
                    }
                    /* track last command */
                    pCmdDescLast = pCmdDesc;
                    break;
                case eCycFrameLayout_FIXED:
                case eCycFrameLayout_IN_DMA:
                    /* activate/deactivate command in the template frame */
                    if (pCmdDesc->wCmdTypeOffset != 0)
                    {
                        if (pCmdDesc->bActive)
                        {
                            pbyFrame[pCmdDesc->wCmdTypeOffset] = pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd;
                        }
                        else
                        {
                            pbyFrame[pCmdDesc->wCmdTypeOffset] = EC_CMD_TYPE_NOP;
                        }
                    }
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                    break;
                }
            }
            /* set frame index into first command and reset NEXT field of last command (eCycFrameLayout_STANDARD / eCycFrameLayout_DYNAMIC) */
            if (EC_NULL != pCmdDescFirst)
            {
                pCmdDescFirst->EcCmdHeader.uCmdIdx.swCmdIdx.byIdx = pCycFrameDesc->byCmdHdrIdx;
                if (EC_NULL != pCmdDescLast)
                {
                    EC_ICMDHDR_SET_LEN_NEXT(&pCmdDescLast->EcCmdHeader, EC_FALSE);
                }
            }
        }
    }
}

#if (defined INCLUDE_MAILBOX_STATISTICS)
EC_T_VOID CEcMaster::ClearMailboxStatisticsCounters(EC_T_UINT64 qwClear)
{
    EC_T_DWORD dwClearIdx = 0;
    for (dwClearIdx = 0; dwClearIdx < EC_MAILBOX_STATISTICS_CNT; dwClearIdx++, qwClear = (qwClear >> 8))
    {
        if ((qwClear & 0x01) == 0x01) /* Total Read Transfer Count */
        {
            m_MailboxStatistics[dwClearIdx].Read.Cnt.dwTotal = 0;
        }
        if ((qwClear & 0x02) == 0x02) /* Read Transfer Count Last Second */
        {
            m_MailboxStatistics[dwClearIdx].Read.Cnt.dwLast = 0;
        }
        if ((qwClear & 0x04) == 0x04) /* Total Bytes Read */
        {
            m_MailboxStatistics[dwClearIdx].Read.Bytes.dwTotal = 0;
        }
        if ((qwClear & 0x08) == 0x08) /* Bytes Read Last Second */
        {
            m_MailboxStatistics[dwClearIdx].Read.Bytes.dwLast = 0;
        }
        if ((qwClear & 0x10) == 0x10) /* Total Write Transfer Count */
        {
            m_MailboxStatistics[dwClearIdx].Write.Cnt.dwTotal = 0;
        }
        if ((qwClear & 0x20) == 0x20) /* Write Transfer Count Last Second */
        {
            m_MailboxStatistics[dwClearIdx].Write.Cnt.dwLast = 0;
        }
        if ((qwClear & 0x40) == 0x40) /* Total Bytes Write */
        {
            m_MailboxStatistics[dwClearIdx].Write.Bytes.dwTotal = 0;
        }
        if ((qwClear & 0x80) == 0x80) /* Bytes Write Last Second */
        {
            m_MailboxStatistics[dwClearIdx].Write.Bytes.dwLast = 0;
        }
    }
}
#endif /* INCLUDE_MAILBOX_STATISTICS */

/*-END OF SOURCE FILE--------------------------------------------------------*/
