/*-----------------------------------------------------------------------------
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Cyril Eyssautier
 * Description              Implementation of Distributed clocks
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>

/*-COMPILER SETTINGS---------------------------------------------------------*/
#undef DEBUG_DC_SYSTIMEVALUES

#if (defined INCLUDE_DC_SUPPORT)

#include <EcInvokeId.h>
#include <EcMaster.h>
#include <EcSlave.h>
#include <EcBusTopology.h>
#include <EcDistributedClocks.h>
#if (defined INCLUDE_MASTER_OBD)
#include <EcSdoServ.h>
#endif

/*-DEFINES-------------------------------------------------------------------*/
#define ETHTYPE1        ((EC_T_BYTE)0xFF)
#define ETHTYPE0        ((EC_T_BYTE)0x10)
#define STATE_CHNG      ((EC_T_BYTE)0x00)
#define DCAPROVAL       ((EC_T_BYTE)0x01)

#define LINE_DELAY_MII_PORT_TYPICAL     610     /* typical line delay of a MII port */

/*-MACROS--------------------------------------------------------------------*/
#undef  EcLinkErrorMsg
#define EcLinkErrorMsg m_poEthDevice->LinkErrorMsg
#undef  EcLinkDbgMsg
#define EcLinkDbgMsg   m_poEthDevice->LinkDbgMsg

/*-FUNCTIONS-----------------------------------------------------------------*/

/*-CEcDistributedClocks------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Default Constructor.                                                 
Creates a static instance of Distributed Clocks management class.
*/
CEcDistributedClocks::CEcDistributedClocks(
    CEcMaster*  pMaster
   )
{
    /* generic management */
    m_pMaster                   = pMaster;
    m_poEthDevice               = pMaster->EthDevice();
    m_dwTimeout                 = EC_DC_DEFAULTTIMEOUT;

    /* initialization flag */
    m_bDCInitialized            = EC_FALSE;

    /* propagation delay */
    m_nPortDelaysMeasCycles     = 0;
    m_nPortDelaysMeasCnt        = 0;
    m_bContinuousMeasuringEnabled = EC_FALSE;
    m_bContinuousMeasuringActive  = EC_FALSE;
    m_pDCReqContinuousMeasuring = EC_NULL;
#if (defined INCLUDE_RED_DEVICE)
    m_bLineBreakAtStart         = EC_FALSE;
    m_dwRedClosePortStamp       = 0;
    m_nRedPropagDelay			= 0;
    m_pRedBusSlaveRefClock      = EC_NULL;
    m_bRedSynchronizeBusTime    = EC_FALSE;
#endif /* INCLUDE_RED_DEVICE */
    /* reference clock */
    m_pCfgSlaveRefClock         = EC_NULL;
    m_pBusSlaveRefClock         = EC_NULL;
    m_bBusSlaveRefClockFound    = EC_FALSE;
    m_bBusSlaveRefClockChanged  = EC_FALSE;
    m_bNoDCSlaveWasConnected    = EC_FALSE;
    m_bFirstDcSlaveAsRefClock   = EC_FALSE;

    /* DC bus time */
    m_qwBusTimeLast             = 0;
    m_qwBusTime                 = 0;
    m_dwBusTimeMsecCounter      = 0;
    m_nDisableEstimationCnt     = 0;

    /* system time offset */
    m_pBusSlaveSystemTimeOffsetRef = EC_NULL;
    m_qwReferenceTimeHost       = 0;
    m_qwReferenceTimeBus        = 0;

    /* distribution control */
    m_bAcycDistributionEnabled  = EC_TRUE;
    m_bCycDistributionActive    = EC_FALSE;
    m_bAcycDistributionActive   = EC_FALSE;

    /* ARMW burst handling */
    m_dwBurstTotalLength        = DC_ARMW_BURSTCYCLES_DCINIT;
    m_dwBurstBulk               = DC_ARMW_BURSTSPP;
    m_bBulkInLinkLayer          = EC_FALSE;
    m_dwBurstCycles             = 0;

    /* DC start time */
    m_dwDcStartTimeGrid         = 0;
    m_qwDcStartTimeSafetyNsec   = DC_START_CYC_SAFETY_DEFAULT;
    m_qwDcStartTime             = 0;
    m_qwDcStartTimeNext         = 0;

    /* DC monitoring */
    m_bSyncWindowMonitoringConfigured   = EC_FALSE;
    m_bSyncWindowMonitoringActive       = EC_FALSE;
    m_pBusSlaveSyncWindowMonitoring     = EC_NULL;
    m_pBusSlaveSyncWindowMonitoringLast = EC_NULL;
    m_dwSystemTimeDifference    = 0;
    m_bInSync                   = EC_FALSE;
    m_bInSyncNotifyActive       = EC_TRUE;
    m_dwDeviationLimit          = 0x3ff;        /* Default 2^10 - 1 = 1023 nanosec */
    m_dwInSyncSettleTime        = 1000;         /* default settle time : 1 second (1000msec) */
    m_bDeviationIsNegative      = EC_FALSE;
    m_dwDeviation               = 0;
    OsMemset(&m_oSlaveSyncNotification, 0, sizeof(EC_T_DC_SYNC_NTFY_DESC));

    /* DCM */
    m_eDcmMode                  = eDcmMode_Off;
    m_bBusShiftConfiguredInEni  = EC_FALSE;
    m_bDcmConfiguredByApp       = EC_FALSE;
    m_poDcm                     = EC_NEW(CEcDcmBase(m_pMaster));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::m_poDcm", m_poDcm, sizeof(CEcDcmBase));
    OsMemset(&m_oDcShiftConfig, 0, sizeof(EC_T_DC_SHIFTSYSTIME_DESC));

#if (defined INCLUDE_DCX)
    m_wDcxExtClockFixedAddr     = 0;
#endif

    /* state machine management */
    OsMemset(&m_oRequest, 0, sizeof(m_oRequest));
    m_pRequest                  = EC_NULL;
    m_pCurRequest               = EC_NULL;
    m_eReqState                 = ecdcsm_idle;
    m_aCurStateList             = EC_NULL;
    m_dwCurStateIdx             = 0;
    m_eCurState                 = ecdcsm_idle;
    m_pDCReqExternal            = EC_NULL;
    m_dwEcatCmdPending          = 0;
    m_dwEcatCmdSent             = 0;
    m_dwEcatCmdProcessed        = 0;
    m_pBusSlaveCur              = EC_NULL;
    OsMemset(&m_oCalculatePropagDelaysMainContext, 0, sizeof(EC_T_DC_CALCULATEPROPAGDELAYS_CONTEXT));
#if (defined INCLUDE_RED_DEVICE)
    OsMemset(&m_oCalculatePropagDelaysRedContext,  0, sizeof(EC_T_DC_CALCULATEPROPAGDELAYS_CONTEXT));
#endif
    /* initialization for completeness */
    m_dwDCResult          = EC_E_ERROR;
    m_dwEcatCmdPending    = 0;
    m_dwEcatCmdPendingMax = 0;
}

/***************************************************************************************************/
/**
\brief  Destructor

Destroy static instance of Distributed Clocks management class.
*/
CEcDistributedClocks::~CEcDistributedClocks()
{
    if (EC_NULL != m_poDcm)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::~m_poDcm", m_poDcm, m_poDcm->GetThisSize());
        SafeDelete(m_poDcm);
    }
}

/***************************************************************************************************/
/**
\brief  Register Start System Time Offset Callback.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDistributedClocks::DcmRegisterDcStartTimeCallback(
    EC_T_DC_SYNCSO_REGDESC*     pParms  /**< [in]   Registration Parameter set */
                                                        )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes = EC_E_NOERROR;

    /* check parameters */
    if ((EC_NULL == pParms) || (EC_NULL == pParms->pCallbackParm) || (EC_NULL == pParms->pfnCallback))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwRes = m_poDcm->RegisterStartTimeCallback(pParms);
    if (EC_E_INVALIDSTATE == dwRes)
    {
	    /* instance CEcMasterShiftByApp object if necessary */
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::~m_poDcm", m_poDcm, sizeof(CEcDcmBase));
        SafeDelete(m_poDcm);

        m_poDcm = EC_NEW(CEcMasterShiftByApp(m_pMaster));
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::m_poDcm", m_poDcm, sizeof(CEcMasterShiftByApp));
        if (EC_NULL == m_poDcm) { dwRetVal = EC_E_NOMEMORY; goto Exit; }
        
        dwRes = m_poDcm->RegisterStartTimeCallback(pParms);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

        m_eDcmMode = eDcmMode_MasterShiftByApp;
        m_bDcmConfiguredByApp = EC_TRUE;
    }
    dwRetVal = dwRes;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Register Callback.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDistributedClocks::DcmRegisterTimeStampCallback(
    EC_T_REGISTER_TSPARMS*      pParms      /**< [in]   Register parameters */
   ,EC_T_REGISTER_TSRESULTS*    pRes        /**< [in]   Register results */
   )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes = EC_E_NOERROR;

    /* check parameters */
    if ((EC_NULL == pParms) || (EC_NULL == pParms->pfTimeStamp) || (EC_NULL == pRes))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    dwRes = m_poDcm->RegisterTimeStampCallback(pParms, pRes);
    if (EC_E_INVALIDSTATE == dwRes)
    {
	    /* instance CEcMasterShiftByApp object if necessary */
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::~m_poDcm", m_poDcm, sizeof(CEcDcmBase));
        SafeDelete(m_poDcm);

        m_poDcm = EC_NEW(CEcMasterShiftByApp(m_pMaster));
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::m_poDcm", m_poDcm, sizeof(CEcMasterShiftByApp));
        if (EC_NULL == m_poDcm) { dwRetVal = EC_E_NOMEMORY; goto Exit; }

        dwRes = m_poDcm->RegisterTimeStampCallback(pParms, pRes);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

        m_eDcmMode = eDcmMode_MasterShiftByApp;
        m_bDcmConfiguredByApp = EC_TRUE;
    }
    dwRetVal = dwRes;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Host time stamp

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDistributedClocks::DcmHostTimeStamp(EC_T_PVOID pCallerData, EC_T_DWORD* pdwHostTimeLo)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    CEcMaster* poMaster = (CEcMaster*)pCallerData;

    OsDbgAssert(EC_NULL != pdwHostTimeLo);

    if ((EC_NULL != poMaster) && (eDcmMode_LinkLayerRefClock == poMaster->m_pDistributedClks->DcmGetMode()))
    {
        EC_T_LINK_IOCTLPARMS oIoCtlParms;
        OsMemset(&oIoCtlParms, 0, sizeof(EC_T_LINK_IOCTLPARMS));

        oIoCtlParms.pbyOutBuf = (EC_T_BYTE*)pdwHostTimeLo;
        oIoCtlParms.dwOutBufSize = sizeof(EC_T_UINT64);
        dwRetVal = poMaster->m_poEcDevice->LinkIoctl(EC_LINKIOCTL_GETTIME, &oIoCtlParms);
    }
    else
    {
        OsSystemTimeGet((EC_T_UINT64*)pdwHostTimeLo);
    }

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Set bus shift mode

\return EC_E_NOERROR on success, error code otherwise.
*/
 EC_T_DWORD CEcDistributedClocks::DcmSetBusShiftConfigured(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    /* set flags */
    m_bBusShiftConfiguredInEni = EC_TRUE;

#if (defined INCLUDE_DCX)
    if (0 != m_wDcxExtClockFixedAddr)
    {
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }
#endif

    if (!m_bDcmConfiguredByApp)
    {
        CEcDcmBusShift* pDcmBusShift = EC_NULL;
        /* delete DCM placeholder */
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::~m_poDcm", m_poDcm, sizeof(CEcDcmBase));
        SafeDelete(m_poDcm);

        pDcmBusShift = EC_NEW(CEcDcmBusShift(m_pMaster));
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::m_poDcm", pDcmBusShift, sizeof(CEcDcmBusShift));
        if (EC_NULL == pDcmBusShift) { dwRetVal = EC_E_NOMEMORY; goto Exit; }

        /* initialize bus shift controller with default configuration */
        pDcmBusShift->Initialize();

        m_poDcm = pDcmBusShift;
        m_eDcmMode = eDcmMode_BusShift;
    }
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
 }

#if (defined INCLUDE_DCX)
 /***************************************************************************************************/
 /**
 \brief  Set external clock slave for Dcx mode

 \return EC_E_NOERROR on success, error code otherwise.
 */
EC_T_DWORD CEcDistributedClocks::DcxSetExtClock(
     EC_T_BOOL               bFixedAddress,              /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
     EC_T_WORD               wSlaveAddress               /**< [in]   Slave address */
     )
 {
    EC_T_DWORD dwRes = EC_E_ERROR;
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    CEcDcmBase* poDcmTmp = m_poDcm;

    if (!bFixedAddress)
    {
        CEcSlave* poCfgSlave = m_pMaster->GetSlaveByCfgAutoIncAddr(wSlaveAddress);
        if (EC_NULL == poCfgSlave) { dwRetVal = EC_E_NOTFOUND; goto Exit; }

        m_wDcxExtClockFixedAddr = poCfgSlave->GetCfgFixedAddr();
    }
    else
    {
        m_wDcxExtClockFixedAddr = wSlaveAddress;
    }

    if (!m_bDcmConfiguredByApp)
    {
        CEcDcx* poDcx = EC_NEW(CEcDcx(m_pMaster));
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::m_poDcm", poDcx, sizeof(CEcDcx));
        if (EC_NULL == poDcx) { dwRetVal = EC_E_NOMEMORY; goto Exit; }

        m_poDcm = poDcx;
             
        dwRes = poDcx->SetExtClock(m_wDcxExtClockFixedAddr);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

        m_eDcmMode = eDcmMode_Dcx;
    }
    
    dwRetVal = EC_E_NOERROR;
Exit:
    /* clean up */
    if (m_poDcm != poDcmTmp)
    {
        /* in case of an error restore Dcm object */
        if (EC_E_NOERROR != dwRetVal)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::~m_poDcm", m_poDcm, sizeof(CEcDcmBase));
            SafeDelete(m_poDcm);
            m_poDcm = poDcmTmp;
        }
        else
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::~m_poDcm", poDcmTmp, sizeof(CEcDcmBase));
            SafeDelete(poDcmTmp);
        }
    }

    return dwRetVal;
}
#endif

/***************************************************************************************************/
/**
\brief  Configure DCM synchonization

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDistributedClocks::DcmConfigure(EC_T_DCM_CONFIG* pDcmConfig)
{
    EC_T_DWORD    dwRetVal = EC_E_ERROR;
    EC_T_DWORD    dwRes    = EC_E_ERROR;
    CEcDcmBase*   poDcmTmp = m_poDcm;

    /* dcmmodes busshift and masterrefclock require a prepared eni */
    if (m_pMaster->IsConfigLoaded())
    {
        /* notice dcm configuration by application after ecatConfigureMaster() */
        m_bDcmConfiguredByApp = EC_TRUE;

        if (((pDcmConfig->eMode == eDcmMode_BusShift) || (pDcmConfig->eMode == eDcmMode_MasterRefClock) || (pDcmConfig->eMode == eDcmMode_Dcx))
            && !m_bBusShiftConfiguredInEni)
        {
            dwRetVal = EC_E_FEATURE_DISABLED;
            goto Exit;
        }
    }

    if (m_eDcmMode != pDcmConfig->eMode)
    {
        /* create a new DCM instance */
        switch (pDcmConfig->eMode)
        {
            case eDcmMode_Off:
            {
                m_poDcm = EC_NEW(CEcDcmBase(m_pMaster));
                EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::m_poDcm", m_poDcm, sizeof(CEcDcmBase));
                if (EC_NULL == m_poDcm) { dwRetVal = EC_E_NOMEMORY; goto Exit; }
            } break;
            case eDcmMode_BusShift:
            {
                CEcDcmBusShift* pDcmBusShift = EC_NEW(CEcDcmBusShift(m_pMaster));
                EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::m_poDcm", pDcmBusShift, sizeof(CEcDcmBusShift));
                if (EC_NULL == pDcmBusShift) { dwRetVal = EC_E_NOMEMORY; goto Exit; }

                m_poDcm = pDcmBusShift;

                /* initialize bus shift controller with default configuration */
                dwRes = pDcmBusShift->Initialize(); 
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            } break;
            case eDcmMode_MasterShift:
            {
                m_poDcm = EC_NEW(CEcDcmMasterShift(m_pMaster));
                EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::m_poDcm", m_poDcm, sizeof(CEcDcmMasterShift));
                if (EC_NULL == m_poDcm) { dwRetVal = EC_E_NOMEMORY; goto Exit; }
            } break;
            case eDcmMode_LinkLayerRefClock:
            {
                m_poDcm = EC_NEW(CEcDcmLinkLayerRefClock(m_pMaster));
                EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::m_poDcm", m_poDcm, sizeof(CEcDcmLinkLayerRefClock));
                if (EC_NULL == m_poDcm) { dwRetVal = EC_E_NOMEMORY; goto Exit; }
            } break;
            case eDcmMode_MasterRefClock:
            {
                m_poDcm = EC_NEW(CEcDcmMasterRefClock(m_pMaster));
                EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::m_poDcm", m_poDcm, sizeof(CEcDcmMasterRefClock));
                if (EC_NULL == m_poDcm) { dwRetVal = EC_E_NOMEMORY; goto Exit; }
            } break;
#if (defined INCLUDE_DCX)
            case eDcmMode_Dcx:
            {
                CEcDcx* poDcx = EC_NEW(CEcDcx(m_pMaster));
                EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::m_poDcm", poDcx, sizeof(CEcDcx));
                if (EC_NULL == poDcx) { dwRetVal = EC_E_NOMEMORY; goto Exit; }
                
                m_poDcm = poDcx;

                /* set external clock slave address if configured */
                if (0 != pDcmConfig->u.Dcx.wExtClockFixedAddr)
                {
                    m_wDcxExtClockFixedAddr = pDcmConfig->u.Dcx.wExtClockFixedAddr;
                }

                dwRes = poDcx->SetExtClock(m_wDcxExtClockFixedAddr);
                if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
            } break;
#endif
            default:
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            } /* no break */
        }
        /* set flag again in case of multible calls to ecatConfigureMaster */
        m_bDcmConfiguredByApp = EC_TRUE; 
    }
    /* apply configuration */
    dwRes = m_poDcm->Configure(pDcmConfig);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    /* no error */
    m_eDcmMode = pDcmConfig->eMode;
    dwRetVal = EC_E_NOERROR;

Exit:
    /* clean up */
    if (m_poDcm != poDcmTmp)
    {
        /* in case of an error restore Dcm object */
        if (EC_E_NOERROR != dwRetVal)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::~m_poDcm", m_poDcm, sizeof(CEcDcmBase));
            SafeDelete(m_poDcm);
            m_poDcm = poDcmTmp;
        }
        else
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcDistributedClocks::~m_poDcm", poDcmTmp, sizeof(CEcDcmBase));
            SafeDelete(poDcmTmp);
        }
    }

    return dwRetVal;
}

#if (defined INCLUDE_DCX)
/***************************************************************************************************/
/**
\brief  Get DC master external synchonization status

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDistributedClocks::DcxGetStatus(
    EC_T_DWORD* pdwErrorCode, 
    EC_T_INT*   pnDiffCur, 
    EC_T_INT*   pnDiffAvg, 
    EC_T_INT*   pnDiffMax, 
    EC_T_INT64* pnTimeStampDiff
    )
{
    CEcDcx* poDcx = EC_NULL;

    if (eDcmMode_Dcx != m_eDcmMode)
        return EC_E_INVALIDSTATE;

    poDcx = (CEcDcx*)m_poDcm;
    
    return poDcx->GetDcxStatus(pdwErrorCode, pnDiffCur, pnDiffAvg, pnDiffMax, pnTimeStampDiff);
}
#endif

/***************************************************************************************************/
/**
\brief  Configure distributed clocks
*/
EC_T_DWORD CEcDistributedClocks::Configure(
    EC_T_DC_CONFIGURE* pDcConfig
   )
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (pDcConfig->bBulkInLinkLayer)
    {
        if (!m_pMaster->m_poEcDevice->IsBulkInLinkLayerSupported())
        {
            dwRetVal = EC_E_NOTSUPPORTED;
            goto Exit;
        }
    }
    m_bBulkInLinkLayer = pDcConfig->bBulkInLinkLayer;
    m_bAcycDistributionEnabled = !pDcConfig->bAcycDistributionDisabled;

    if (pDcConfig->dwTimeout != 0)
    {
        m_dwTimeout = pDcConfig->dwTimeout;
    }
    SetDeviationLimit(pDcConfig->dwDevLimit);

    if (pDcConfig->dwSettleTime != 0)
    {
        m_dwInSyncSettleTime = pDcConfig->dwSettleTime;
    }
    if (pDcConfig->dwTotalBurstLength != 0)
    {
        m_dwBurstTotalLength = pDcConfig->dwTotalBurstLength;
    }
    if (pDcConfig->dwBurstBulk != 0)
    {
        m_dwBurstBulk = pDcConfig->dwBurstBulk;
    }
    if (pDcConfig->dwDcStartTimeGrid != 0)
    {
        m_dwDcStartTimeGrid = pDcConfig->dwDcStartTimeGrid;
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "DcConfigure()\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "Burst length    = %d\n", m_dwBurstTotalLength));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "Burst bulk      = %d\n", m_dwBurstBulk));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "InSync settle   = %d\n", m_dwInSyncSettleTime));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "Timeout         = %d\n", m_dwTimeout));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "DcStartTimeGrid = %d\n", m_dwDcStartTimeGrid));

Exit:
    return dwRetVal;
}
EC_T_DWORD CEcDistributedClocks::GetConfig(
    EC_T_DC_CONFIGURE* pDcConfig
   )
{
    pDcConfig->dwDevLimit         = GetDeviationLimit();
    pDcConfig->dwTotalBurstLength = m_dwBurstTotalLength;
    pDcConfig->dwBurstBulk        = m_dwBurstBulk;
    pDcConfig->dwSettleTime       = m_dwInSyncSettleTime;
    pDcConfig->dwTimeout          = m_dwTimeout;
    pDcConfig->dwDcStartTimeGrid  = m_dwDcStartTimeGrid;

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Start Init DC.

Start Initialization / Synchronization of Distributed Clocks.
\return EC_E_NOERROR in case of success, ErrorCode otherwise.
*/
EC_T_DWORD CEcDistributedClocks::StartInitDC(EC_T_VOID)
{
    return RequestState(ecdcsm_dcinit, &m_pDCReqExternal);
}

/***************************************************************************************************/
/**
\brief  Handle topology changes

\return EC_E_NOERROR in case of success, ErrorCode otherwise.
*/
EC_T_DWORD CEcDistributedClocks::StartTopologyChanged(EC_T_VOID)
{
    if (m_bNoDCSlaveWasConnected)
    {
        return RequestState(ecdcsm_dcinit, &m_pDCReqExternal);
    }
    else
    {
        return RequestState(ecdcsm_dctopologychanged, &m_pDCReqExternal);
    }
}

/***************************************************************************************************/
/**
\brief  Wait for state machine
*
\return EC_E_NOERROR on successful completion of scan, error code otherwise.
*/
EC_T_DWORD CEcDistributedClocks::WaitForStateMachine(EC_T_VOID)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (EC_NULL != m_pDCReqExternal)
    {
        /* if request is finished, release request handle */
        if ((m_eCurState == m_eReqState) && (EC_NULL == m_pCurRequest))
        {
            /* make state machine stable and idle */
            m_eCurState = m_eReqState = ecdcsm_idle;
            ReleaseReq(m_pDCReqExternal);
            m_pDCReqExternal = EC_NULL;
            dwRetVal = m_dwDCResult;
        }
        else
        {
            dwRetVal = EC_E_BUSY;
        }
    }
    else
    {
        dwRetVal = EC_E_NOTREADY;
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Cancel a pending / busy request.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDistributedClocks::CancelRequest( EC_T_VOID )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    if (EC_NULL == m_pDCReqExternal)
    {
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }

    dwRetVal = PollState(m_pDCReqExternal);
    
    switch( dwRetVal )
    {
        case EC_E_BUSY:
        {
            /* request active, issue cancel */
            m_pDCReqExternal->bCancelRequest = EC_TRUE;
        } break;
        case EC_E_NOTREADY:
        {
            /* request not started jet, prevent fetch */
            m_pDCReqExternal->bCancelRequest = EC_TRUE;
            m_pDCReqExternal->dwResult       = EC_E_TIMEOUT;

            m_pRequest = EC_NULL;
            dwRetVal = EC_E_NOERROR;
        } break;
        case EC_E_NOERROR:
        {
            /* request finished already, do nothing */
        } break;
        default:
        {
            /* unexpected state, cancel request */
            OsDbgAssert(EC_FALSE);
            m_pDCReqExternal->bCancelRequest = EC_TRUE;
            dwRetVal = EC_E_BUSY;
        } break;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
*/
EC_T_VOID CEcDistributedClocks::CalculateDcStartTime( EC_T_VOID )
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
EC_T_UINT64 qwDcStartTimeNext = 0;
EC_T_UINT64 qwCycleTime       = 0;
EC_T_UINT64 qwCycleAlignment  = 0;

    if (m_bDCInitialized)
    {
        /* determine cycle time */    
        if (0 != m_pMaster->GetBusCycleTimeUsec())
        {
            qwCycleTime = m_pMaster->GetBusCycleTimeUsec() * 1000;
        }
        else if (EC_NULL != m_poDcm->GetCfgSlaveSyncRef()) /* fallback if master doesn't know cycle time */
        {
            CEcSlave* poCfgSlaveSyncRef = m_poDcm->GetCfgSlaveSyncRef();
            qwCycleTime = EC_MAX(poCfgSlaveSyncRef->GetDcRegisterCycleTime0(), poCfgSlaveSyncRef->GetDcRegisterCycleTime1());
        }

        if (0 != qwCycleTime)
        {
            /* calculate safety cycles */
            qwDcStartTimeNext = ((m_qwDcStartTimeSafetyNsec / qwCycleTime) + 1) * qwCycleTime;

            /* calculate first DC start time */
            if (0 == m_qwDcStartTime)
            {
                EC_T_UINT64 qwDcmStartTime = m_poDcm->DcStartTime(m_qwBusTime, qwCycleTime);

                qwDcStartTimeNext += (qwDcmStartTime > 0) ? qwDcmStartTime : m_qwBusTime;

                /* store first DC start time */
                m_qwDcStartTime = qwDcStartTimeNext;
            }
            else
            {
                if (m_qwBusTime > m_qwDcStartTime)
                {
                    qwCycleAlignment   = ( m_qwBusTime - m_qwDcStartTime ) % qwCycleTime;
                    qwDcStartTimeNext += m_qwBusTime - qwCycleAlignment;
                }
                else
                {
                    qwDcStartTimeNext +=  m_qwDcStartTime;
                }
            }
            /* store next DC start time */
            m_qwDcStartTimeNext = qwDcStartTimeNext;
        }
        else
        {
            OsDbgAssert( EC_FALSE );
        }
    }
    {
        EC_T_UINT64 qwTmp = 0;

        if (eDcmMode_LinkLayerRefClock == m_eDcmMode)
        {
            EC_T_LINK_IOCTLPARMS oIoCtlParms;

            oIoCtlParms.pbyInBuf = EC_NULL;
            oIoCtlParms.dwInBufSize = 0;
            oIoCtlParms.pbyOutBuf = (EC_T_BYTE*)&qwTmp;
            oIoCtlParms.dwOutBufSize = sizeof(EC_T_UINT64);
            oIoCtlParms.pdwNumOutData = EC_NULL;
            m_pMaster->m_poEcDevice->LinkIoctl(EC_LINKIOCTL_GETTIME, &oIoCtlParms);
        }
        else
        {
            OsSystemTimeGet( &qwTmp);
        }

        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "CalculateDcStartTime()\n"));
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "DcStartTime     = 0x%08X%08X\n", EC_HIDWORD( m_qwDcStartTime ),     EC_LODWORD( m_qwDcStartTime )));
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "BusTime         = 0x%08X%08X\n", EC_HIDWORD( m_qwBusTime ),         EC_LODWORD( m_qwBusTime )));
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "HostTime        = 0x%08X%08X\n", EC_HIDWORD( qwTmp ),               EC_LODWORD( qwTmp )));
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), " Diff           = %18d\n", ( qwTmp - m_qwBusTime )));
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "DcStartTimeNext = 0x%08X%08X\n", EC_HIDWORD( m_qwDcStartTimeNext ), EC_LODWORD( m_qwDcStartTimeNext )));
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), " Diff           = %18d\n", ( m_qwDcStartTimeNext - m_qwBusTime )));
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "CycleTime       = %18d\n\n",     EC_LODWORD( qwCycleTime )));
    }
    return;
}

/***************************************************************************************************/
/**
*/
EC_T_VOID CEcDistributedClocks::SetBusTime(
    EC_T_UINT64 qwBusTime,              /**< [in]   Bus Time from Master Clk Slave */
    EC_T_BOOL   bBusTimeAvailable
                                          )
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif

    /* handle bus time is not available */
    if (!bBusTimeAvailable)
    {
        /* ensure m_qwBusTimeLast and m_qwBusTime consistency */
        m_nDisableEstimationCnt = 3;
        goto Exit;
    }
    /* handle first call */
    if (0 == m_qwBusTime)
    {
        goto Exit;
    }
    /* handle 64bit calls */
    if (0 != EC_HIDWORD(qwBusTime))
    {
        goto Exit;
    }
    /* handle 32bit calls */
    {
    EC_T_DWORD dwOverrunCount = 0;

        /* handle multiple overruns
         * This may occur if the ARMW was not send for a long time. One cycle of 32Bit DC is approximatly 4295 msecs
         * and if new 32Bit value less than old
         */
        dwOverrunCount = (m_pMaster->m_dwMsecCounter - m_dwBusTimeMsecCounter) / 4295;
        if (0 != dwOverrunCount)
        {
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "SetBusTime()\n"));
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "OverrunCount = %d\n", dwOverrunCount));
        }
        if (EC_LODWORD(m_qwBusTime) > EC_LODWORD(qwBusTime))
        {
            dwOverrunCount++;
        }
        /* correct bus time */
        qwBusTime = EC_MAKEQWORD((EC_HIDWORD(m_qwBusTime)+dwOverrunCount), EC_LODWORD(qwBusTime));
    }
Exit:
    /* refresh bus time  */
    if (bBusTimeAvailable)
    {
        m_qwBusTimeLast = m_qwBusTime;
        m_qwBusTime     = qwBusTime;
        m_dwBusTimeMsecCounter = m_pMaster->m_dwMsecCounter;
        if (0 != m_nDisableEstimationCnt)
        {
            m_nDisableEstimationCnt--;
        }
    }
    return;
}

/***************************************************************************************************/
/**
\brief
*/
EC_T_VOID CEcDistributedClocks::DistributeSystemTimeSend(
    EC_T_LINK_FRAMEDESC* poLinkFrameDesc,
    EC_T_BOOL            bCycFramesAreProcessed,
    EC_T_BOOL            bStampJob,
    EcCycCmdConfigDesc*  pCmdDesc,
    ETYPE_EC_CMD_HEADER* pCmdHdr
    )
{
EC_T_BOOL bCmdActive = EC_FALSE;

    if (m_bCycDistributionActive)
	{
        switch (m_eDcmMode)
        {
        case eDcmMode_LinkLayerRefClock:
        {
            /* request timestamping */
            poLinkFrameDesc->wTimestampOffset = (EC_T_WORD)(((EC_T_BYTE*)EC_ENDOF(pCmdHdr)) - poLinkFrameDesc->pbyFrame);
            poLinkFrameDesc->wTimestampSize   = pCmdDesc->wDataLen;

            /* activate command */
            bCmdActive = EC_TRUE;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_SYSTEMTIME);
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
        } break;
        case eDcmMode_MasterRefClock:
        {
            CEcDcmMasterRefClock* pDcmMasterRefClk = (CEcDcmMasterRefClock*)m_poDcm;
            CEcBusSlave* pBusSlaveBusTimeRef = pDcmMasterRefClk->GetBusSlaveBusTimeRef();

            if (EC_NULL != pBusSlaveBusTimeRef)
            {
                EC_T_WORD wFixedAddress = pBusSlaveBusTimeRef->GetFixedAddress();

                /* activate command */
                bCmdActive = EC_TRUE;
                EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wFixedAddress);
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_SYSTEMTIME);
                pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD;
            }
            /* update master time */
            if (0 != pDcmMasterRefClk->GetMasterTime())
            {
                pDcmMasterRefClk->SetMasterTime(pDcmMasterRefClk->GetMasterTime() + (EC_T_UINT64)m_pMaster->GetBusCycleTimeUsec() * 1000);
            }
        } break;
        case eDcmMode_BusShift:
        case eDcmMode_MasterShift:
        case eDcmMode_MasterShiftByApp:
        case eDcmMode_Dcx:
        default:
        {
            if (EC_NULL != m_pBusSlaveRefClock)
            {
            EC_T_UINT64 qwNextEstimatedBusTime = GetNextEstimatedBusTime();

                /* handle DCM */
		        if (bCycFramesAreProcessed && (DcmCheckForStamp() || bStampJob))
		        {
			        pCmdDesc->bThisTimeStamped = EC_TRUE;

			        poLinkFrameDesc->pdwTimeStampLo       = &(pCmdDesc->dwTimeStampLo);
			        poLinkFrameDesc->pdwTimeStampPostLo   = &(pCmdDesc->dwTimeStampPostLo);
                    m_poDcm->GetTimeStampCallback(&poLinkFrameDesc->pfnTimeStamp,
                                                  &poLinkFrameDesc->pvTimeStampCtxt,
                                                  &poLinkFrameDesc->pdwLastTSResult);
		        }
		        else
		        {
			        pCmdDesc->bThisTimeStamped = EC_FALSE;
		        }
                /* activate command */
                bCmdActive = EC_TRUE;
		        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, m_pBusSlaveRefClock->GetFixedAddress());
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_SYSTEMTIME);

                /* insert the estimated bus time to protect for huge system time differences in case of the reference clock suddendly disappears */
                if (0 != qwNextEstimatedBusTime)
                {
	                if (sizeof(EC_T_UINT64) == pCmdDesc->wDataLen)
	                {
		                EC_SET_FRM_QWORD((EC_T_PBYTE)EC_ENDOF(pCmdHdr), qwNextEstimatedBusTime);
	                }
	                else
	                {
		                EC_SET_FRM_DWORD((EC_T_PBYTE)EC_ENDOF(pCmdHdr), EC_LODWORD(qwNextEstimatedBusTime));
	                }
		            if (m_pMaster->m_pBusTopology->TopologyIsChanging())
		            {
                        /* don't distribute bus time if topology is changing, only read the bus time for DCM */
			            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD;
		            }
                    else
                    {
                        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FRMW;
                    }
                }
                else
                {
                    pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD;
                }
	        }
        } break;
        }
    }
    /* deactivate command if needed */
    if (!bCmdActive)
    {
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_SYSTEMTIME);
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_NOP;
    }
}

/***************************************************************************************************/
/**
\brief
*/
EC_T_VOID CEcDistributedClocks::DistributeSystemTimeProcess(
    EcCycCmdConfigDesc*  pCmdDesc,
    ETYPE_EC_CMD_HEADER* pCmdHdr
    )
{
    if (pCmdDesc->wLastWkc == 0)
    {
        SetBusTime(0, EC_FALSE);
    }
    else
    {
    EC_T_UINT64 qwBusTime = 0;

        if (sizeof(EC_T_UINT64) == pCmdDesc->wDataLen)
        {
            qwBusTime = EC_GET_FRM_QWORD(EC_ENDOF(pCmdHdr));
        }
        else
        {
            qwBusTime = EC_MAKEQWORD(0,EC_GET_FRM_DWORD(EC_ENDOF(pCmdHdr)));
        }
        SetBusTime(qwBusTime, EC_TRUE);
        m_poDcm->DistributeSystemTimeProcess(pCmdDesc->bThisTimeStamped, pCmdDesc->dwTimeStampLo, pCmdDesc->dwTimeStampPostLo);
    }
}

/***************************************************************************************************/
/**
\brief
*/
EC_T_VOID CEcDistributedClocks::ShiftSystemTimeSend(
    EcCycCmdConfigDesc*  pCmdDesc,
    ETYPE_EC_CMD_HEADER* pCmdHdr
    )
{
    EC_T_BOOL bCmdActive = EC_FALSE;

    switch (m_eDcmMode)
    {
    case eDcmMode_MasterRefClock: /* busshift */
    {
    EC_T_UINT64 qwShiftTime = ((CEcDcmMasterRefClock*)m_poDcm)->GetMasterTime();

        /* Distribute Master time to the slaves */
        if (0 != qwShiftTime)
        {
            /* activate command */
            bCmdActive = EC_TRUE;
            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
            EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_SYSTEMTIME);
            if (sizeof(EC_T_UINT64) == pCmdDesc->wDataLen)
            {
                EC_SET_FRM_QWORD((EC_T_PBYTE)EC_ENDOF(pCmdHdr), qwShiftTime);
            }
            else
            {
                EC_SET_FRM_DWORD((EC_T_PBYTE)EC_ENDOF(pCmdHdr), EC_LODWORD(qwShiftTime));
            }
            pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BWR;
        }
    } break;
    case eDcmMode_BusShift:
    case eDcmMode_MasterShift:
    case eDcmMode_MasterShiftByApp:
    case eDcmMode_Dcx:
    case eDcmMode_Off:    
    {
        if (EC_NULL != m_pBusSlaveRefClock)
        {
            /* Shift system time of the reference clock according DCM controller */
		    if( m_oDcShiftConfig.dwCyclesToShift != 0 )
		    {
            EC_T_UINT64 qwSystemTimeOffset = 0ul;
            EC_T_BYTE*  pbyCurData = (EC_T_BYTE*)pCmdHdr + ETYPE_EC_CMD_HEADER_LEN;

                m_oDcShiftConfig.dwCyclesToShift--;

                /* activate command */
                bCmdActive = EC_TRUE;
	            EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, m_pBusSlaveRefClock->GetFixedAddress());
                EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_SYSTEMTIME);
		        qwSystemTimeOffset = m_oDcShiftConfig.nShiftTime + GetBusTime();
                if (sizeof(EC_T_UINT64) == pCmdDesc->wDataLen)
		        {
			        EC_SET_FRM_QWORD(pbyCurData, qwSystemTimeOffset);
		        }
		        else
		        {
			        EC_SET_FRM_DWORD(pbyCurData, EC_LODWORD(qwSystemTimeOffset));
		        }
		        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
            }
		    else
		    {
                /* Reset shift time delayed, to give a chance to the acyclic distribution */
		        OsMemset(&m_oDcShiftConfig, 0, sizeof(EC_T_DC_SHIFTSYSTIME_DESC));
		    }
		}
    } break;
    default:
    {
        /* nothing to do */
        break;
    }
    }
    /* deactivate command if needed */
    if (!bCmdActive)
    {
        EC_ICMDHDR_SET_ADDR_ADP(pCmdHdr, 0);
        EC_ICMDHDR_SET_ADDR_ADO(pCmdHdr, ECM_DCS_SYSTEMTIME);
	    pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_NOP;
    }
}

/***************************************************************************************************/
/**
\brief
*/
EC_T_VOID CEcDistributedClocks::ReadSystemTimeDifferenceSend(
    EcCycCmdConfigDesc*  pCmdDesc,
    ETYPE_EC_CMD_HEADER* pCmdHdr
    )
{
EC_T_WORD wSyncWindowMonitoringFixedAddr = GetSyncWindowMonitoringFixedAddr();

    EC_UNREFPARM(pCmdDesc);

    if (INVALID_FIXED_ADDR == wSyncWindowMonitoringFixedAddr)
    {
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_NOP;
    }
    else
    {
        pCmdHdr->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD;
    }
    EC_AL_ICMDHDR_SET_ADDR_ADP(pCmdHdr, wSyncWindowMonitoringFixedAddr);
}

/***************************************************************************************************/
/**
\brief  Processes the response of an EtherCAT DCS command.

DCS Invoke ID's are redirected to here.
*/
EC_T_VOID CEcDistributedClocks::ProcessResult(
    EC_T_DWORD dwResult,                /**< [in] result */
    EC_T_DWORD dwInvokeId,              /**< [in] invokeId */
    PETYPE_EC_CMD_HEADER pEcCmdHeader   /**< [in] frame header */
                                              )
{
EC_T_PBYTE   pbyData       = EC_NULL;
EC_T_WORD    wWorkingCnt   = 0;
EC_T_WORD    wFixedAddress = 0;
CEcBusSlave* pBusSlave     = EC_NULL;

    /* handle error case */
    if (dwResult != EC_E_NOERROR)
    {
        goto Exit;
    }
    /* get command information */
    if (EC_NULL != pEcCmdHeader)
    {
        wWorkingCnt = ETYPE_EC_CMD_GETWKC(pEcCmdHeader);
        pbyData     = (EC_T_PBYTE)EC_ENDOF(pEcCmdHeader);
    }
    else
    {
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }
    /* dispatch the frame */
    switch (dwInvokeId)
    {
    /* clear system time values (offset and delay) */
    case INVOKE_ID_DC_CLEARSYSTIMEVAL:
        {
            if (m_eCurState == ecdcsm_clearsystimevalues_wait)
            {
                /* mark command as processed */
                m_dwEcatCmdProcessed++;
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwDCResult);
            }
        } break;
#if (defined INCLUDE_RED_DEVICE)
    /* close/open red port to allow propagation delay measurement */
    case INVOKE_ID_DC_REDOPENCLOSEPORT:
        {
            /* nothing to do */
        } break;
    /* handle redundancy */
    case INVOKE_ID_DC_REDHANDLING:
        {
            if ((m_eCurState == ecdcsm_reddisableecatevent_wait) || (m_eCurState == ecdcsm_redrefreshdlstatus_wait) || (m_eCurState == ecdcsm_redopenport_wait) || (m_eCurState == ecdcsm_redenableecatevent_wait))
            {
                if ( 1 == wWorkingCnt )
                {
                    /* get bus slave */
                    wFixedAddress = (EC_T_WORD)(EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader));
                    pBusSlave     = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
                        /* store values in bus slave */
                        switch (EC_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader))
                        {
                        case ECREG_DL_CONTROL:
                        {
                            pBusSlave->SetDlControl(EC_GET_FRM_DWORD(pbyData));

                            /* force redundancy link status to protect for frameloss */
                            if ((m_eCurState == ecdcsm_redopenport_wait))
                            {
                                m_pMaster->m_poEcDevice->m_pRedDevice->ForceLinkStatus(eLinkStatus_OK);
                                m_pMaster->m_poEcDevice->m_pRedDevice->RefreshLinkStatus();
                                m_pMaster->m_poEcDevice->m_pRedDevice->ForceLinkStatus(eLinkStatus_UNDEFINED);
                            }
                            /* mark command as processed */
                            m_dwEcatCmdProcessed++;
                        } break;
                        case ECREG_SLV_ECATEVENTMASK:
                        {
                            pBusSlave->SetEcatEventMask(EC_GET_FRM_WORD(pbyData));

                            /* mark command as processed */
                            m_dwEcatCmdProcessed++;
                        } break;
                        case ECREG_DL_STATUS:
                        {
                            pBusSlave->SetPortSetting(EC_GET_FRM_WORD(pbyData));

                            /* mark command as processed */
                            m_dwEcatCmdProcessed++;
                        } break;
                        default:
                        {
                            OsDbgAssert(EC_FALSE);
                        } break;
                        }
                    }
                }
            }
            else
            {
                OsDbgAssert((EC_E_TIMEOUT == m_dwDCResult) || (EC_E_NOMEMORY == m_dwDCResult));
            }
        } break;
#endif /* INCLUDE_RED_DEVICE */
    /* latch receive times (broadcast on Receive Time Port0) */
    case INVOKE_ID_DC_LATCHRECVTIMES:
        {
            if (m_eCurState == ecdcsm_latchrecvtimes_wait)
            {
                /* mark command as processed */
                m_dwEcatCmdProcessed++;
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwDCResult);
            }
        } break;
    /* read receive times */
    case INVOKE_ID_DC_READRECVTIMES:
        {
            if (m_eCurState == ecdcsm_readrecvtimes_wait)
            {
                /* get bus slave */
                wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                pBusSlave     = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wFixedAddress);
                if (EC_NULL != pBusSlave)
                {
                    if (0 == wWorkingCnt)
                    {
                        pBusSlave->SetRecvTimesSupported(EC_FALSE);

                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                    else if (1 == wWorkingCnt)
                    {
                        pBusSlave->SetRecvTimesSupported(EC_TRUE);
                        pBusSlave->SetRecvTimes(pbyData, m_pMaster);

                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwDCResult);
            }
        } break;
    /* write system time values (system time offset and propagation delay) */
    case INVOKE_ID_DC_WRITESYSTIMEVAL:
        {
            if (m_eCurState == ecdcsm_writesystimevalues_wait)
            {
                if (1 == wWorkingCnt)
                {
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwDCResult);
            }
        } break;
    /* write propagation delays */
    case INVOKE_ID_DC_WRITEPROPAGDELAYS:
        {
            if (m_eCurState == ecdcsm_writepropagdelays_wait)
            {
                if (1 == wWorkingCnt)
                {
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwDCResult);
            }
        } break;
#if (defined DEBUG_DC_SYSTIMEVALUES)
    /* for developpment purpose only */
    case INVOKE_ID_DC_DBGSYSTIMEVAL:
        {
            if (m_eCurState == ecdcsm_debugsystimevalues_wait)
            {
                if (1 == wWorkingCnt)
                {
                    EC_T_UINT64*                    pqwTimeOffsetRead   = EC_NULL;
                    EC_T_DWORD*                     pdwPropagDelay      = EC_NULL;

                    pqwTimeOffsetRead = (EC_T_UINT64*)pbyData;

                    pdwPropagDelay    = (EC_T_DWORD*)(pbyData+8);

                    /* get bus slave */
                    wFixedAddress     = (EC_T_WORD)(EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader));
                    pBusSlave         = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
                        /* place linklayer debug messages, which can be compared with the values from the 
                         * slave controllers, to have a possibility to approve written values, using wireshark */

                        if (byDbgLvl)
                        {
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL),
                                "Approve System Time Offset from slave 0x%lx : 0x%lx\n",pBusSlave->GetBusIndex(),EC_GET_FRM_QWORD(pqwTimeOffsetRead)));
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL),
                                "Approve Propagation Delay from slave 0x%lx : 0x%lx\n",pBusSlave->GetBusIndex(),EC_GET_FRM_DWORD(pdwPropagDelay)));
                        }
                    }
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwDCResult);
            }
        } break;
#endif /* DEBUG_DC_SYSTIMEVALUES */
    /* read reference clock sync pulse length DCM */
    case INVOKE_ID_DC_READSYNCPULSEWDT:
        {
            if (m_eCurState == ecdcsm_dcmreadpulselength_wait)
            {
                if (1 == wWorkingCnt)
                {
                    /* store grid value */
                    m_poDcm->m_dwRefPulseLength = EC_GET_FRM_WORD(pbyData);

                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwDCResult);
            }
        } break;
    /* ARMW Burst (FRMW) to saturize DC controllers */
    case INVOKE_ID_DC_ARMWBURST:
        {
            if (m_eCurState == ecdcsm_armwburst_wait)
            {
                /* mark command as processed */
                m_dwEcatCmdProcessed++;
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwDCResult);
            }
        } break;
    /* read reference time (stamped command) */
    case INVOKE_ID_DC_READREFTIME:
        {
            if ((m_eCurState == ecdcsm_readreferencetime_wait) || (m_eCurState == ecdcsm_startbustime64emul_wait))
            {
                if (1 == wWorkingCnt)
                {
                    m_qwReferenceTimeBus = EC_GET_FRM_QWORD(pbyData);

                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwDCResult);
            }
        } break;
    /* configure DC controllers */
    case INVOKE_ID_DC_CFGDCCONTROLLERS:
        {
            if ((m_eCurState == ecdcsm_configdccontrollers1_wait) || (m_eCurState == ecdcsm_configdccontrollers2_wait))
            {
                if (1 == wWorkingCnt)
                {
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwDCResult);
            }
        } break;
    /* wait for DC slaves InSync */
    case INVOKE_ID_DC_WAITFORINSYNC:
        {
            if (1 == wWorkingCnt)
            {
                RefreshSyncWindowMonitoring(EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader), EC_GET_FRM_DWORD(EC_ENDOF(pEcCmdHeader)));
            }
        } break;
    /* acyclic distribution */
    case INVOKE_ID_DC_ACYCDISTRIBUTION:
        {
            /* nothing to do */
        } break;

    /* default value, if unknown invoke id is received */
    default:
        {
            OsDbgAssert( EC_FALSE );
        } break;
    }

Exit:
    switch (dwInvokeId)
    {
        /* these commands are not monitored */
#if (defined INCLUDE_RED_DEVICE)
        case INVOKE_ID_DC_REDOPENCLOSEPORT:
#endif
        case INVOKE_ID_DC_ACYCDISTRIBUTION:
            break;
        case INVOKE_ID_DC_ARMWBURST:
            if (m_bBulkInLinkLayer && (EC_E_NOERROR != dwResult))
            {
                m_dwEcatCmdPending = 0;
                break;
            }
            /* no break */
        default:
        {
            /* decrement pending commands counter */
            OsDbgAssert(m_dwEcatCmdProcessed <= m_dwEcatCmdSent);
            OsDbgAssert(0 != m_dwEcatCmdPending);
            if (0 != m_dwEcatCmdPending)
            {
                m_dwEcatCmdPending--;
            }
        }
        break;
    }
    return;
}

/***************************************************************************************************/
/**
\brief  Set deviation limit

This function set the deviation limit.
*/
EC_T_VOID CEcDistributedClocks::SetDeviationLimit(
    EC_T_DWORD  dwDevLimit
   )
{
    m_dwDeviationLimit = (EC_T_DWORD)((1<<dwDevLimit)-1);

#if (defined INCLUDE_MASTER_OBD)
    m_pMaster->m_pSDOService->SetDCSlaveDevLimit((EC_T_DWORD)((1<<dwDevLimit)-1));
#endif
}

/***************************************************************************************************/
/**
\brief  Get deviation limit

This function get the deviation limit.
*/
EC_T_DWORD CEcDistributedClocks::GetDeviationLimit(EC_T_VOID)
{
EC_T_DWORD dwDeviationLimit = 0;
EC_T_BYTE  byCount          = 0;

    if (0 != m_dwDeviationLimit)
    {
        for (byCount = 0; byCount < 32; byCount++)
        {
            if (0 == (m_dwDeviationLimit&(1<<byCount)))
            {
                dwDeviationLimit = byCount;
            }
        }
    }
    return dwDeviationLimit;
}

/***************************************************************************************************/
/**
\brief Returns the fixed address of the next slave for the sync window monitoring
       All DC slaves after reference clock are returned. In case of redundancy line break the bus 
       slave at line break is not monitored.

This function updates the current Deviation value.
*/
EC_T_WORD CEcDistributedClocks::GetSyncWindowMonitoringFixedAddr(EC_T_VOID)
{
EC_T_WORD wFixedAddr    = INVALID_FIXED_ADDR;
EC_T_BOOL bRefClockOnly = EC_FALSE;

    /* select first slave to monitor */
    if (EC_NULL == m_pBusSlaveSyncWindowMonitoring)
    {
        if ((eDcmMode_LinkLayerRefClock == m_eDcmMode) || (eDcmMode_MasterRefClock == m_eDcmMode))
        {
            /* start with first slave */
            m_pBusSlaveSyncWindowMonitoring = m_pMaster->m_pBusTopology->GetBusSlaveList();
        }
        else
        {
            /* don't monitor system time difference if reference clock is unknown or not initialized */    
            if (!m_bSyncWindowMonitoringActive || (EC_NULL == m_pBusSlaveRefClock))
            {
                m_pBusSlaveSyncWindowMonitoringLast = EC_NULL;
                m_pBusSlaveSyncWindowMonitoring = EC_NULL;
                goto Exit;
            }
            /* start with the slave after the reference clock */
            m_pBusSlaveSyncWindowMonitoring = m_pBusSlaveRefClock->GetNext();
        }
        /* reset the system time difference over all DC slaves */
        m_dwSystemTimeDifference = 0;

        /* assume the reference clock is currently the only DC slave on bus */
        bRefClockOnly = EC_TRUE;
    }
    for (; (m_pBusSlaveSyncWindowMonitoring != EC_NULL); m_pBusSlaveSyncWindowMonitoring = m_pBusSlaveSyncWindowMonitoring->GetNext())
    {
        /* look for the next DC slave disregarding the redundancy line reference clock */
        if ( m_pBusSlaveSyncWindowMonitoring->IsDcInitialized()
#if (defined INCLUDE_RED_DEVICE)
         && (m_pBusSlaveSyncWindowMonitoring != m_pRedBusSlaveRefClock)
#endif      
           )
        {
            if (wFixedAddr == INVALID_FIXED_ADDR)
            {
                /* the reference clock is not the only DC slave on bus */
	            bRefClockOnly = EC_FALSE;

                /* get fixed address of the slave to monitor */
                wFixedAddr = m_pBusSlaveSyncWindowMonitoring->GetFixedAddress();
            }
            else
            {
                /* m_pBusSlaveSyncWindowMonitoring must point to next slave to monitor or to EC_NULL */
                break;
            }
        }
    }
    /* handle InSync notification if reference clock is the only DC slave on bus */
    if (bRefClockOnly)
    {
    EC_T_BOOL bNotify = !m_bInSync && m_bInSyncNotifyActive;

        /* refresh slave sync information */
        m_bInSync              = EC_TRUE;
        m_bDeviationIsNegative = EC_FALSE;
        m_dwDeviation          = 0;

        /* refresh slave sync notification descriptor */
        m_oSlaveSyncNotification.IsInSync    = m_bInSync;
        m_oSlaveSyncNotification.IsNegative  = m_bDeviationIsNegative;
        m_oSlaveSyncNotification.dwDeviation = m_dwDeviation;

        /* refresh master object dictionary */
#if (defined INCLUDE_MASTER_OBD)
        m_pMaster->m_pSDOService->EntrySet_SetDCInSync(m_bInSync);
#endif
        /* handle notification */
        if (bNotify)
        {
            m_pMaster->NotifyDcSlvStatus(m_bInSync, m_bDeviationIsNegative, m_dwDeviation, EC_NULL);
        }
    }
Exit:
    return wFixedAddr;
}

/***************************************************************************************************/
/**
\brief  Refresh sync window monitoring

This function updates the current deviation value and requests a deviation notification if necessary.
*/
EC_T_VOID CEcDistributedClocks::RefreshSyncWindowMonitoring(EC_T_WORD wFixedAddress, EC_T_DWORD dwSystemTimeDifference)
{
EC_T_BOOL    bInSyncDetected    = EC_FALSE;
EC_T_BOOL    bOutOfSyncDetected = EC_FALSE;
CEcBusSlave* pBusSlave          = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wFixedAddress);

    /* check parameters */
    if (EC_NULL == pBusSlave)
    {
        goto Exit;
    }
    /* save system time difference in the bus slave */
    pBusSlave->m_dwSystemTimeDifference = dwSystemTimeDifference;
    m_pBusSlaveSyncWindowMonitoringLast = pBusSlave;

    /* compute the system time difference over all DC slaves */
    if ((eDcmMode_LinkLayerRefClock == m_eDcmMode) || (eDcmMode_MasterRefClock == m_eDcmMode) || (EC_NULL != m_pBusSlaveRefClock))
    {
        m_dwSystemTimeDifference |= dwSystemTimeDifference;
    }
    /* handle "not in sync" notification for the current slave */
    if (0 != m_dwDeviationLimit)
    {
        if ((dwSystemTimeDifference & ~0x80000000) > m_dwDeviationLimit)
        {
            if (m_bInSync)
            {
                bOutOfSyncDetected = EC_TRUE;
            }
        }
    }
    /* by last monitored slave, refresh deviation value and handle settling and "in sync" notification */
    if (EC_NULL == m_pBusSlaveSyncWindowMonitoring)
    {
        /* refresh slave sync information */
        m_bDeviationIsNegative = (m_dwSystemTimeDifference &  0x80000000) != 0;
        m_dwDeviation          =  m_dwSystemTimeDifference & ~0x80000000;

        /* handle settling */
        if (0 != m_dwDeviationLimit)
        {
            if (m_dwDeviation > m_dwDeviationLimit)
            {
                /* stop settle time */
                m_oInSyncSettleTime.Stop();

                if (m_bInSync)
                {
                    /* refresh slave sync information */
                    m_bInSync = EC_FALSE;

                    /* "not in sync" notification already generated or triggered */
                }
            }
            else
            {
                if (!m_bInSync)
                {
                    /* after we are below the limit we have to wait the settle time */
                    if (!m_oInSyncSettleTime.IsStarted())
                    {
                        m_oInSyncSettleTime.Start(m_dwInSyncSettleTime, m_pMaster->GetMsecCounterPtr());
                    }
                    /* wait until settle time expires */
                    if (m_oInSyncSettleTime.IsElapsed())
                    {
                        m_oInSyncSettleTime.Stop();
                        m_bInSync = EC_TRUE;
                        bInSyncDetected = EC_TRUE;
                    }
                }
            }
        }
        else
        {
            /* monitoring off, assume "in sync" */
            if (!m_bInSync)
            {
                bInSyncDetected = EC_TRUE;

                /* refresh slave sync information */
                m_bInSync = EC_TRUE;
            }
        }
        /* refresh slave sync notification descriptor */
        m_oSlaveSyncNotification.IsInSync    = m_bInSync;
        m_oSlaveSyncNotification.IsNegative  = m_bDeviationIsNegative;
        m_oSlaveSyncNotification.dwDeviation = m_dwDeviation;
    }
    /* handle detected change */
    if (bInSyncDetected || bOutOfSyncDetected)
    {
        /* refresh slave sync notification descriptor */
        m_oSlaveSyncNotification.IsInSync    = m_bInSync;
        m_oSlaveSyncNotification.IsNegative  = m_bDeviationIsNegative;
        m_oSlaveSyncNotification.dwDeviation = m_dwDeviation;

        /* refresh master object dictionary */
#if (defined INCLUDE_MASTER_OBD)
        m_pMaster->m_pSDOService->EntrySet_SetDCInSync(m_bInSync);
#endif

        /* handle notification */
        if (bInSyncDetected)
        {
            m_pMaster->NotifyDcSlvStatus(EC_TRUE, m_bDeviationIsNegative, m_dwDeviation, EC_NULL);
        }
        else
        {
            m_pMaster->NotifyDcSlvStatus(EC_FALSE, (dwSystemTimeDifference &  0x80000000) != 0, dwSystemTimeDifference & ~0x80000000, pBusSlave);
        }
    }
Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Handle continuous run-time measuring

This function start periodicaly a ecdcsm_dccontinuousmeasuring request and wait for it.
*/
EC_T_VOID CEcDistributedClocks::HandleContinuousMeasuring(EC_T_VOID)
{
    /* handle continuous measurement */
    if (!m_bContinuousMeasuringEnabled || !m_bContinuousMeasuringActive)
    {
        goto Exit;
    }
    if (EC_NULL == m_pDCReqContinuousMeasuring)
    {
        if (m_oContinuousMeasuringTime.IsStarted())
        {
            if (m_oContinuousMeasuringTime.IsElapsed())
            {
                m_oContinuousMeasuringTime.Stop();
                RequestState(ecdcsm_dccontinuousmeasuring, &m_pDCReqContinuousMeasuring);
            }
        }
        else
        {
            m_oContinuousMeasuringTime.Start(DC_CONTINUOUSMEASURING_PERIOD, m_pMaster->GetMsecCounterPtr());
        }
    }
    else
    {
        if ((m_eCurState == m_eReqState) && (EC_NULL == m_pCurRequest))
        {
            /* make state machine stable and idle */
            m_eCurState = m_eReqState = ecdcsm_idle;
            ReleaseReq(m_pDCReqContinuousMeasuring);
            m_pDCReqContinuousMeasuring = EC_NULL;
        }
    }
Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Enqueue new Request for DC Instance.

\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CEcDistributedClocks::RequestState(
    ECDCSM_T_DCSTATES   eState          /**< [in]   Desired state */
   ,ECDCSM_REQ**        pHandle         /**< [out]  Handle to Request if EC_E_NOERROR is returned */
                                       )
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
EC_T_DWORD dwRetVal = EC_E_ERROR;

    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "DcRequest(%d)\n", eState));

    /* check parameter */
    if (EC_NULL == pHandle)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* only valid target states are let through */
    switch(eState)
    {
    case ecdcsm_dcinit:
    case ecdcsm_dctopologychanged:
    case ecdcsm_dccontinuousmeasuring:
        break;
    default:
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* check if a storage place in order queue can be acquired */
    if ((m_oRequest[0].bUsed) && (m_oRequest[1].bUsed))
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    /* get request handle */
    OsDbgAssert( EC_NULL == m_pRequest );
    if (EC_NULL == m_pRequest)
    {
        if (m_oRequest[0].bUsed)
        {
            (*pHandle) = &(m_oRequest[1]);
        }
        else
        {
            (*pHandle) = &(m_oRequest[0]);
        }
        
        /* mark handle as used, and store requested DC state */
        (*pHandle)->eState          = eState;
        (*pHandle)->dwResult        = EC_E_BUSY;
        (*pHandle)->bCancelRequest  = EC_FALSE;
        (*pHandle)->bUsed           = EC_TRUE;
        m_pRequest = (*pHandle);
        dwRetVal = EC_E_NOERROR;
    }
    else
    {
        dwRetVal = EC_E_BUSY;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Poll success of requested state.

\return error code.
*/
EC_T_DWORD CEcDistributedClocks::PollState(
    ECDCSM_REQ*     pHandle     /**< [in]   desired request to wait for */
                                          )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    if (m_pRequest == pHandle)
    {
        /* request not fetched for execution by state machine */
        dwRetVal = EC_E_NOTREADY;
        goto Exit;
    }
    
    if (m_pCurRequest == pHandle)
    {
        /* request fetched, result is updated by state machine */
        dwRetVal = m_pCurRequest->dwResult;
    }
    else
    {
        /* already out of SM, waits for result read */
        dwRetVal = EC_E_NOERROR;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Release handle after reading result code.

\return error code.
*/
EC_T_DWORD CEcDistributedClocks::ReleaseReq(
    ECDCSM_REQ*     pHandle     /**< [in]   desired request to release */
                                           )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* check parameter for valid handle */
    if ((pHandle != &(m_oRequest[0])) && (pHandle != &(m_oRequest[1])))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    /* return handle */
    if (pHandle == m_pRequest)
    {
        m_pRequest = EC_NULL;
    }

    /* we expect here that the state machine is idle and there is no running request set */
    OsDbgAssert(m_eCurState == m_eReqState && m_pCurRequest == EC_NULL);

    /* request still pending */
    if (pHandle == m_pCurRequest)
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    /* mark request as unused */
    pHandle->bUsed = EC_FALSE;

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Set reference clock slave by auto increment address
*/
EC_T_DWORD  CEcDistributedClocks::SetRefClockAutoIncAddr(EC_T_WORD wAutoIncAddr)
{
    m_pCfgSlaveRefClock = m_pMaster->GetSlaveByCfgAutoIncAddr(wAutoIncAddr);
    if (EC_NULL == m_pCfgSlaveRefClock)
	{
        return EC_E_NOTFOUND;
    }
    m_pCfgSlaveRefClock->SetDcInitializationRequired(EC_TRUE);
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Set reference clock slave by fixed address
*/
EC_T_DWORD  CEcDistributedClocks::SetRefClockFixedAddr(EC_T_WORD wFixedAddr)
{
    m_pCfgSlaveRefClock = m_pMaster->GetSlaveByCfgFixedAddr(wFixedAddr);
    if (EC_NULL == m_pCfgSlaveRefClock)
	{
        return EC_E_NOTFOUND;
    }
    m_pCfgSlaveRefClock->SetDcInitializationRequired(EC_TRUE);
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Remove bus slave
*/
EC_T_VOID CEcDistributedClocks::RemoveBusSlave(CEcBusSlave* poBusSlave)
{
    /* refresh DC reference clock information if needed */
    if (poBusSlave == m_pBusSlaveRefClock)
    {
        m_pBusSlaveRefClock = EC_NULL;
        m_pMaster->NotifyRefClockPresence(EC_NULL);
    }
    if (poBusSlave == m_poDcm->GetBusSlaveBusTimeRef())
    {
        m_poDcm->SetBusSlaveBusTimeRef(EC_NULL);
        m_pMaster->NotifyRefClockPresence(EC_NULL);
    }
    if (poBusSlave == m_pBusSlaveSyncWindowMonitoring)
    {
        m_pBusSlaveSyncWindowMonitoring = EC_NULL;
    }
    if (poBusSlave == m_pBusSlaveSyncWindowMonitoringLast)
    {
        m_pBusSlaveSyncWindowMonitoringLast = EC_NULL;
    }
}

/***************************************************************************************************/
/**
\brief  Reset reference bus slaves
*/
EC_T_VOID CEcDistributedClocks::ResetRefBusSlaves(EC_T_VOID)
{
    m_bBusSlaveRefClockFound = EC_FALSE;
    m_bBusSlaveRefClockChanged = EC_FALSE;
    m_pBusSlaveSystemTimeOffsetRef = EC_NULL;
    m_poDcm->SetCfgSlaveSyncRef(EC_NULL);
#if (defined INCLUDE_RED_DEVICE)
    m_pRedBusSlaveRefClock = EC_NULL;
#endif
}

/***************************************************************************************************/
/**
\brief  Look for reference bus slaves
*/
EC_T_VOID CEcDistributedClocks::LookForRefBusSlaves(CEcBusSlave* pBusSlave)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
CEcSlave* pCfgSlave = pBusSlave->GetCfgSlave();

    /* look for DC reference clock */
    switch (m_eDcmMode)
    {
    case eDcmMode_LinkLayerRefClock:
    case eDcmMode_MasterRefClock:
    {
        /* look for a slave to use as reference for the bus time */
        if ((EC_NULL == m_poDcm->GetBusSlaveBusTimeRef()) && pBusSlave->IsDcEnabled())
        {
             m_poDcm->SetBusSlaveBusTimeRef(m_pBusSlaveCur);
        }
#if (defined INCLUDE_RED_DEVICE)
        /* look for a slave to use as reference clock on the redundancy line in case of line break and reference clock is on the main line */
        if (m_bLineBreakAtStart && (EC_NULL == m_pRedBusSlaveRefClock))
        {
            if ((pBusSlave->GetBusIndex() >= m_pMaster->m_wRedNumSlavesOnMain) && pBusSlave->IsDcEnabled())
            {
                m_pRedBusSlaveRefClock = pBusSlave;
                m_pRedBusSlaveRefClock->m_dwSystemTimeDifference = 0;
            }
        }
#endif /* INCLUDE_RED_DEVICE */
    } break;
    default:
    {
        if (!m_bBusSlaveRefClockFound)
        {
            if (m_bFirstDcSlaveAsRefClock)
            {
                if (pCfgSlave->IsDcInitializationRequired())
                {
                    m_bBusSlaveRefClockFound = EC_TRUE;
                }
            }
            else if ((EC_NULL != m_pCfgSlaveRefClock) && (m_pCfgSlaveRefClock->IsPresentOnBus()))
            {
                if (pCfgSlave == m_pCfgSlaveRefClock)
                {
                    m_bBusSlaveRefClockFound = EC_TRUE;
                }
            }
            else
            {
                if (pCfgSlave->IsDcPotentialRefClock())
                {
                    m_bBusSlaveRefClockFound = EC_TRUE;
                }
                else
                {
                    if (pCfgSlave->IsDcInitializationRequired() && (EC_NULL != m_pCfgSlaveRefClock) && m_pCfgSlaveRefClock->IsOptional())
                    {
                        m_bBusSlaveRefClockFound = EC_TRUE;
                    }
                }
            }
            if (m_bBusSlaveRefClockFound)
            {
                if (m_pBusSlaveRefClock != pBusSlave)
                {
                    m_bBusSlaveRefClockChanged = EC_TRUE;
                    m_pBusSlaveRefClock        = pBusSlave;
                    m_pMaster->NotifyRefClockPresence(m_pBusSlaveRefClock);

                    /* deactivate distribution */
                    m_bCycDistributionActive  = EC_FALSE;
                    m_bAcycDistributionActive = EC_FALSE;
                }
            }
            else
            {
                if (pCfgSlave->IsDcInitializationRequired())
                {
                    m_dwDCResult = EC_E_DC_SLAVES_BEFORE_REF_CLOCK;
                }
            }
        }
        if (m_bBusSlaveRefClockFound)
        {
            /* validate reference clock */
            if ((EC_NULL != m_pBusSlaveRefClock) && !m_pBusSlaveRefClock->IsDcUnitSupported())
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), 
                    "DC (time loop control) unit of reference clock (%s) disabled\n", m_pBusSlaveRefClock->m_pCfgSlave->GetName()));
                m_dwDCResult = EC_E_DC_REF_CLOCK_SYNC_OUT_UNIT_DISABLED;
            }
            /* look for DCM synchronization reference clock (can be reference clock) */
            if (EC_NULL == m_poDcm->GetCfgSlaveSyncRef())
            {
                if (pCfgSlave->GetDcSendSyncActivation())
                {
                    m_poDcm->SetCfgSlaveSyncRef(pCfgSlave);
                }
            }
#if (defined INCLUDE_RED_DEVICE)
            /* look for a slave to use as reference clock on the redundancy line in case of line break and reference clock is on the main line */
            if (m_bLineBreakAtStart && (m_pBusSlaveRefClock->GetBusIndex() < m_pMaster->m_wRedNumSlavesOnMain) && (EC_NULL == m_pRedBusSlaveRefClock))
            {
                if ((pBusSlave->GetBusIndex() >= m_pMaster->m_wRedNumSlavesOnMain) && pBusSlave->IsDcEnabled())
                {
                    m_pRedBusSlaveRefClock = pBusSlave;
                    m_pRedBusSlaveRefClock->m_dwSystemTimeDifference = 0;
                }
            }
#endif /* INCLUDE_RED_DEVICE */
        }
    } break;
    }
    /* look for the first initialized 64bit DC slave to be used as system time offset reference */
    if (EC_NULL == m_pBusSlaveSystemTimeOffsetRef)
    {
        if (pBusSlave->IsDC64Support() && pBusSlave->IsDcInitialized())
        {
            m_pBusSlaveSystemTimeOffsetRef = pBusSlave;
        }
    }
}

#if (defined INCLUDE_RED_DEVICE)
/***************************************************************************************************/
/**
\brief  Force red line link status to protect for frameloss
*/
EC_T_DWORD CEcDistributedClocks::RedClosePortStamp(EC_T_PVOID pCallerData, EC_T_DWORD* pdwHostTimeLo)
{
CEcMaster* poMaster = (CEcMaster*)pCallerData;

    EC_UNREFPARM(pdwHostTimeLo);

    poMaster->m_poEcDevice->m_pRedDevice->ForceLinkStatus(eLinkStatus_DISCONNECTED);
    poMaster->m_poEcDevice->m_pRedDevice->RefreshLinkStatus();

    return EC_E_NOERROR;
}
#endif /* INCLUDE_RED_DEVICE */

/***************************************************************************************************/
/**
\brief  Top Level DC State Machine.
*/
EC_T_VOID CEcDistributedClocks::DCStateMachine( EC_T_VOID )
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
    EC_T_DWORD        dwRes            = EC_E_ERROR;
    EC_T_INT          nSlaveSliceCount = 0;
    EC_T_BOOL         bContinueSM      = EC_FALSE;
    ECDCSM_T_DCSTATES eOldState        = ecdcsm_idle;
    CEcBusSlave*      pBusSlave        = EC_NULL;
    CEcSlave*         pCfgSlave        = EC_NULL;

    HandleContinuousMeasuring();

    /* check whether state machine is idle */
    if ((m_eCurState == m_eReqState) && (EC_NULL == m_pCurRequest))
    {
        /* stable, check for queued request */
        if ((EC_NULL != m_pRequest) && (EC_FALSE == m_pRequest->bCancelRequest))
        {
            /* Fetch new Request */
            m_pCurRequest   = m_pRequest;

            /* free Request queue */
            m_pRequest      = EC_NULL;

            /* store current request as target for sm */
            m_eReqState             = m_pCurRequest->eState;

            /* start state Machine */
            m_eCurState             = ecdcsm_start;
            bContinueSM             = EC_TRUE;
            m_oTimeout.Stop();
        }
        else
        {
            /* no new request leave SM */
            bContinueSM             = EC_FALSE;
        }
    }
    else
    {
        /* request still pending, go into SM */
        bContinueSM                 = EC_TRUE;
    }
    /* target state reached, handle not thrown out */
    if ((m_eCurState == m_eReqState) && (EC_NULL != m_pCurRequest))
    {
        m_pCurRequest->dwResult = EC_E_NOERROR;
        m_pCurRequest = EC_NULL;
        bContinueSM = EC_FALSE;
    }
    /* Performance mark */
    PerfMeasStart(&G_TscMeasDesc, EC_MSMT_DCSM);

    while( bContinueSM )
    {
        /* only do a single run of SM per default */
        bContinueSM = EC_FALSE;

        eOldState = m_eCurState;

        /* if Current Request is missing, and SM is not stable -> stuck */
        if ((EC_NULL == m_pCurRequest) && (m_eCurState != m_eReqState))
        {
            OsDbgAssert(EC_FALSE);
            m_dwDCResult = EC_E_ERROR;
            m_eCurState  = m_eReqState;
            continue;
        }
        /* execute state machine */
        switch( m_eCurState )
        {
        case ecdcsm_start:
            {
                /* prepare request */
                m_dwCurStateIdx = 0;
                m_dwDCResult    = EC_E_BUSY;
#if (defined INCLUDE_MASTER_OBD)
                m_pMaster->SDOEntrySet_SetDCBusy(EC_TRUE);
#endif
                switch (m_eReqState)
                {
                case ecdcsm_dcinit:

                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "ecdcsm_dcinit\n"));

                    /* prepare request */
                    m_aCurStateList = C_aStateList_dcinit;

                    /* calculate here the maximum pending commands count:          */
                    /* maximum 10% of the the slaves can be handle at one time,    */
                    /* the value is rounded up to maximum slave commands per frame */
                    m_dwEcatCmdPendingMax = (m_pMaster->m_pBusTopology->GetListSlaveCount()*10)/100;
                    m_dwEcatCmdPendingMax = (m_dwEcatCmdPendingMax/m_pMaster->m_MasterConfig.dwMaxSlaveCmdPerFrame+1)*m_pMaster->m_MasterConfig.dwMaxSlaveCmdPerFrame;

                    /* DC initialization in progress */
                    m_bDCInitialized = EC_FALSE;

                    /* reset DCM */
                    DcmReset();

                    /* reset 64bit bus time emulation */
                    m_qwBusTime = 0;

                    /* reset DC start time */
                    m_qwDcStartTime     = 0;
                    m_qwDcStartTimeNext = 0;

                    /* deactivate distribution */
                    m_bCycDistributionActive  = EC_FALSE;
                    m_bAcycDistributionActive = EC_FALSE;

                    /* reset sync window monitoring */
                    m_bInSync = EC_FALSE;
                    m_oInSyncSettleTime.Stop();
                    OsMemset(&m_oSlaveSyncNotification, 0, sizeof(EC_T_DC_SYNC_NTFY_DESC));

                    /* set ARMW burst cycles */
                    m_dwBurstCycles = m_dwBurstTotalLength;

                    /* set port delay measurement cycles */
                    m_nPortDelaysMeasCycles = DC_PROPAGDELAYMEAS_CYCLES;

                    /* deactivate sync window monitoring */
                    m_bSyncWindowMonitoringActive = EC_FALSE;

                    /* deactivate continuous measurement */
                    m_bContinuousMeasuringActive = EC_FALSE;

                    /* deactivate notification */
                    m_bInSyncNotifyActive = EC_FALSE;
                    break;

                case ecdcsm_dctopologychanged:

                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "ecdcsm_dctopologychanged\n"));

                    /* prepare request */
                    m_aCurStateList = C_aStateList_topologychanged;

                    /* calculate here the maximum pending commands count:          */
                    /* maximum 10% of the the slaves can be handle at one time,    */
                    /* the value is rounded up to maximum slave commands per frame */
                    m_dwEcatCmdPendingMax = (m_pMaster->m_pBusTopology->GetListSlaveCount()*10)/100;
                    m_dwEcatCmdPendingMax = (m_dwEcatCmdPendingMax/m_pMaster->m_MasterConfig.dwMaxSlaveCmdPerFrame+1)*m_pMaster->m_MasterConfig.dwMaxSlaveCmdPerFrame;

                    /* set ARMW burst cycles */
                    m_dwBurstCycles = m_dwBurstTotalLength;

                    /* set port delay measurement cycles */
                    m_nPortDelaysMeasCycles = 1;

                    /* deactivate sync window monitoring */
                    m_bSyncWindowMonitoringActive = EC_FALSE;

                    /* deactivate continuous measurement */
                    m_bContinuousMeasuringActive = EC_FALSE;

                    /* deactivate notification */
                    m_bInSyncNotifyActive = EC_FALSE;
                    break;

                case ecdcsm_dccontinuousmeasuring:

                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "ecdcsm_dccontinuousmeasuring\n"));

                    /* prepare request */
                    m_aCurStateList = C_aStateList_dccontinuousmeasuring;

                    /* calculate here the maximum pending commands count:          */
                    /* maximum 10% of the the slaves can be handle at one time,    */
                    /* the value is rounded up to maximum slave commands per frame */
                    m_dwEcatCmdPendingMax = (m_pMaster->m_pBusTopology->GetListSlaveCount()*10)/100;
                    m_dwEcatCmdPendingMax = (m_dwEcatCmdPendingMax/m_pMaster->m_MasterConfig.dwMaxSlaveCmdPerFrame+1)*m_pMaster->m_MasterConfig.dwMaxSlaveCmdPerFrame;

                    /* set port delay measurement cycles */
                    m_nPortDelaysMeasCycles = 1;
                    break;

                default:
                    OsDbgAssert(EC_FALSE);
                    m_dwDCResult = EC_E_ERROR;
                    m_eCurState  = m_eReqState = ecdcsm_idle;
                    break;
                }
                /* dcmmodes busshift and masterrefclock require a prepared eni */
                if (((eDcmMode_BusShift == m_eDcmMode) || (eDcmMode_MasterRefClock == m_eDcmMode) || (eDcmMode_Dcx == m_eDcmMode))
                    && !m_bBusShiftConfiguredInEni)
                {
                    m_dwDCResult = EC_E_FEATURE_DISABLED;
                }
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* determine next state */
                if (0 == m_pMaster->m_pBusTopology->GetDCSlaveCount())
                {
                    m_bNoDCSlaveWasConnected = EC_TRUE;

                    switch(m_eReqState)
                    {
                    case ecdcsm_dcinit:
                    case ecdcsm_dctopologychanged:
                        NextState(ecdcsm_waitforinsync);
                        break;
                    case ecdcsm_dccontinuousmeasuring:
                        NextState(ecdcsm_dccontinuousmeasuring);
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        NextState(m_eReqState);
                        break;
                    }
                }
                else
                {
                    m_bNoDCSlaveWasConnected = EC_FALSE;

                    /* reset port delays measurement counter */
                    m_nPortDelaysMeasCnt = 0;

                    /* reset reference bus slaves */
                    ResetRefBusSlaves();

#if (defined INCLUDE_RED_DEVICE)
                    /* reset redundancy information */
                    m_bLineBreakAtStart    = m_pMaster->m_bRedLineBreak;
#endif
                    /* prepare next state */
                    m_pBusSlaveCur = m_pMaster->m_pBusTopology->GetBusSlaveList(); /* handle all slaves */

                    /* determine next state */
                    m_eCurState = ecdcsm_start_wait;
                }
                bContinueSM = EC_TRUE;
            } break;
        case ecdcsm_start_wait:
            {
                /* prepare bus slaves and look for reference bus slaves */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL);
                     nSlaveSliceCount--,       m_pBusSlaveCur  = m_pBusSlaveCur->GetNext())
                {
                    /* if DC not yet initialized, mark bus slave as not initialized */
                    if (!m_bDCInitialized)
                    {
                        m_pBusSlaveCur->SetDcInitialized(EC_FALSE);
                    }
                    /* clear port times */
                    m_pBusSlaveCur->ResetRecvTimes();

                    /* reset processed flag used in CalculatePropagDelays() */
                    m_pBusSlaveCur->ResetDCSmProc();

                    /* get config slave */
                    pCfgSlave = m_pBusSlaveCur->m_pCfgSlave;
                    if (EC_NULL == pCfgSlave)
                    {
                        /* skip not configured bus slave */
                        continue;
                    }
                    /* look for reference bus slaves */
                    LookForRefBusSlaves(m_pBusSlaveCur);
                }
                /* determine next state */
                if (EC_NULL == m_pBusSlaveCur)
                {
                    m_eCurState = ecdcsm_start_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_start_done:
            {
                /* if no reference clock found, reset DCM */
                if ((eDcmMode_LinkLayerRefClock != m_eDcmMode) && (eDcmMode_MasterRefClock != m_eDcmMode) && (EC_NULL == m_pBusSlaveRefClock))
                {
                    DcmReset();
                }
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* if no system time offset reference was found, use bus time reference slave */
                if (EC_NULL == m_pBusSlaveSystemTimeOffsetRef)
                {
                    m_pBusSlaveSystemTimeOffsetRef = m_poDcm->GetBusSlaveBusTimeRef();
                }
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "ecdcsm_start_done\n"));
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "BusTime             = 0x%08X%08X\n", EC_HIDWORD(m_qwBusTime), EC_LODWORD(m_qwBusTime)));
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "DC RefClock         = %s\n", (EC_NULL == m_pBusSlaveRefClock   )?"NULL":(EC_NULL == m_pBusSlaveRefClock->m_pCfgSlave)?"Unknown":m_pBusSlaveRefClock->m_pCfgSlave->GetName()));
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "SystemTimeOffsetRef = %s\n", (EC_NULL == m_pBusSlaveSystemTimeOffsetRef)?"NULL":(EC_NULL == m_pBusSlaveSystemTimeOffsetRef->m_pCfgSlave)?"Unknown": m_pBusSlaveSystemTimeOffsetRef->m_pCfgSlave->GetName()));
#if (defined INCLUDE_RED_DEVICE)
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "Red RefClock        = %s\n", (EC_NULL == m_pRedBusSlaveRefClock)?"NULL":(EC_NULL == m_pRedBusSlaveRefClock->m_pCfgSlave)?"Unknown":m_pRedBusSlaveRefClock->m_pCfgSlave->GetName()));
#endif
                if ((eDcmMode_MasterShift == m_eDcmMode) || (eDcmMode_Dcx == m_eDcmMode) || (eDcmMode_MasterShiftByApp == m_eDcmMode))
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "DCM SyncRef         = %s\n", (EC_NULL == m_poDcm->GetCfgSlaveSyncRef())?"NULL":m_poDcm->GetCfgSlaveSyncRef()->GetName()));
                }
                if ((eDcmMode_LinkLayerRefClock == m_eDcmMode) || (eDcmMode_MasterRefClock == m_eDcmMode))
                {
                    CEcBusSlave* pBusSlaveBusTimeRef = m_poDcm->GetBusSlaveBusTimeRef();
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "BusTime Ref         = %s\n", (EC_NULL == pBusSlaveBusTimeRef)?"NULL":(EC_NULL == pBusSlaveBusTimeRef->m_pCfgSlave)?"Unknown":pBusSlaveBusTimeRef->m_pCfgSlave->GetName()));
                }
                /* determine next state */
                NextState();
                bContinueSM = EC_TRUE;
            } break;
        /* clear system time values (offset and delay) */
        case ecdcsm_clearsystimevalues:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pMaster->m_pBusTopology->GetBusSlaveList(); /* handle all slaves */

                /* determine next state */
                m_eCurState = ecdcsm_clearsystimevalues_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecdcsm_clearsystimevalues_send:
            {
            EC_T_BYTE abyData[12];

                OsMemset(abyData, 0, sizeof(abyData));

                /* on dcinit use a broadcast, otherwise access each slaves */
                if (ecdcsm_dcinit == m_eReqState)
                {
                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_DC_CLEARSYSTIMEVAL, EC_CMD_TYPE_BWR, 0,
                        ECM_DCS_SYSTIME_OFFSET, sizeof(abyData), (EC_T_PVOID)abyData
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;

                    /* don't walk through bus slave list */
                    m_pBusSlaveCur = EC_NULL;
                }
                else
                {
                    /* walk through bus slave list */
                    for (; (EC_NULL != m_pBusSlaveCur) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                    {
                        /* skip non DC slaves */
                        if (!m_pBusSlaveCur->IsDcEnabled())
                        {
                            continue;
                        }
                        /* skip if DC already initialized */
                        if (m_pBusSlaveCur->IsDcInitialized())
                        {
                            continue;
                        }
                        /* queue EtherCAT command */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_DC_CLEARSYSTIMEVAL, EC_CMD_TYPE_FPWR, m_pBusSlaveCur->GetFixedAddress(),
                            ECM_DCS_SYSTIME_OFFSET, sizeof(abyData), (EC_T_PVOID)abyData
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                }
                /* determine next state */
                if (0 < m_dwEcatCmdSent)
                {
                    m_eCurState = ecdcsm_clearsystimevalues_wait;
                }
                else
                {
                    m_eCurState = ecdcsm_clearsystimevalues_done;
                }
            } break;
        case ecdcsm_clearsystimevalues_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurState = ecdcsm_clearsystimevalues_send;
                    }
                    else
                    {
                        m_eCurState = ecdcsm_clearsystimevalues_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_clearsystimevalues_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
                bContinueSM = EC_TRUE;
            } break;        
#if (defined INCLUDE_RED_DEVICE)
        /* disable ecat event on slave where redundancy link is connected */
        case ecdcsm_reddisableecatevent:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                if (m_pMaster->IsRedConfigured())
                {
                CEcBusSlave* pBusSlaveRedPort = m_pMaster->m_pBusTopology->GetBusSlaveRedPort();

                    /* disable ecat event */
                    if (!m_bLineBreakAtStart && (EC_NULL != pBusSlaveRedPort))
                    {
                        /* deactivate redundancy notification */
                        m_pMaster->m_bRedNotifyActive = EC_FALSE;

                        /* disable ecat event, to prevent for topology change */
                        if (pBusSlaveRedPort->GetEcatEventSupported())
                        {
                        EC_T_WORD wEcatEventMask    = pBusSlaveRedPort->GetEcatEventMask();
                        EC_T_WORD wEcatEventMaskFrm = 0;

                            /* prepare interrupt configuration value */
                            wEcatEventMask &= ~(ECM_ECATEVENT_DLSTATUS);
                            EC_SET_FRM_WORD(&wEcatEventMaskFrm, wEcatEventMask);

                            /* queue EtherCAT command */
                            dwRes = m_pMaster->QueueRegisterCmdReq(
                                EC_NULL, INVOKE_ID_DC_REDHANDLING, EC_CMD_TYPE_FPWR, pBusSlaveRedPort->GetFixedAddress(),
                                ECREG_SLV_ECATEVENTMASK, sizeof(EC_T_WORD), &wEcatEventMaskFrm
                                );
                            if (EC_E_NOERROR != dwRes)
                            {
                                /* try again next cycle */
                                goto Exit;
                            }
                            m_dwEcatCmdPending++;
                            m_dwEcatCmdSent++;

                            /* flush the frame because this command should be processed before the next ones of the current state */
                            m_pMaster->EcatFlushCurrPendingSlaveFrame();
                        }           
                    }
                }
                /* determine next state */
                if (0 < m_dwEcatCmdSent)
                {
                    m_eCurState = ecdcsm_reddisableecatevent_wait;
                }
                else
                {
                    m_eCurState = ecdcsm_reddisableecatevent_done;
                }
            } break;
        case ecdcsm_reddisableecatevent_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurState = ecdcsm_reddisableecatevent_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_reddisableecatevent_done:
            {
                /* check for abort */
                if (AbortState())
                {
#if (defined INCLUDE_RED_DEVICE)
                    if (m_pMaster->IsRedConfigured())
                    {
                        if (!m_bLineBreakAtStart)
                        {                    
                            NextState(ecdcsm_redenableecatevent);
                        }
                    }
#endif
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
            } break;
        /* close redundancy port to allow propagation delay measurement */
        case ecdcsm_redcloseport:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                if (m_pMaster->IsRedConfigured())
                {
                CEcBusSlave* pBusSlaveRedPort = m_pMaster->m_pBusTopology->GetBusSlaveRedPort();
                EC_T_WORD    wRedPort         = m_pMaster->m_pBusTopology->GetRedPort();

                    /* close the port */
                    if (!m_bLineBreakAtStart && (EC_NULL != pBusSlaveRedPort))
                    {
                    EC_T_DWORD dwDLControl    = pBusSlaveRedPort->GetDlControl();
                    EC_T_DWORD dwDLControlFrm = 0;

                        /* prepare DL control values */
                        dwDLControl &= ~ECM_DLCTRL_LOOP_PORTX_MASK(wRedPort);
                        dwDLControl |= (ECM_DLCTRL_LOOP_PORT_VAL_ALWAYSCLOSED<<ECM_DLCTRL_LOOP_PORTX_SHIFT(wRedPort));
                        EC_SET_FRM_DWORD((EC_T_BYTE*)&dwDLControlFrm, dwDLControl);

                        /* queue EtherCAT command in a single frame */
                        m_pMaster->EcatFlushCurrPendingSlaveFrame();
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_DC_REDOPENCLOSEPORT, EC_CMD_TYPE_FPWR, pBusSlaveRedPort->GetFixedAddress(),
                            ECREG_DL_CONTROL, sizeof(EC_T_DWORD), (EC_T_VOID*)&dwDLControlFrm
                            );
                        OsDbgAssert(EC_E_NOERROR == dwRes);

                        /* flush the frame with timestamping to force red line link status */
                        m_pMaster->EcatFlushCurrPendingSlaveFrameStamp(RedClosePortStamp, m_pMaster, &m_dwRedClosePortStamp, 0, 0, 0, 0xFFFF);

                        /* send a NOP command in a single frame to protect for frameloss (force to drop the previous pending command) */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_DC_REDOPENCLOSEPORT, EC_CMD_TYPE_NOP, pBusSlaveRedPort->GetFixedAddress(),
                            0, 0, EC_NULL
                            );
                        OsDbgAssert(EC_E_NOERROR == dwRes);

                        /* this command is not monitored */
                        m_pMaster->EcatFlushCurrPendingSlaveFrameNoMonitoring();
                    }
                }
                /* determine next state */
                if (0 < m_dwEcatCmdSent)
                {
                    m_eCurState = ecdcsm_redcloseport_wait;
                }
                else
                {
                    m_eCurState = ecdcsm_redcloseport_done;
                }
            } break;
        case ecdcsm_redcloseport_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurState = ecdcsm_redcloseport_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_redcloseport_done:
            {
                /* check for abort */
                if (AbortState())
                {
#if (defined INCLUDE_RED_DEVICE)
                    if (m_pMaster->IsRedConfigured())
                    {
                        if (!m_bLineBreakAtStart)
                        {
                            NextState(ecdcsm_redopenport);
                        }
                    }
#endif
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
            } break;
        /* refresh dl status on slave where redundancy link is connected */
        case ecdcsm_redrefreshdlstatus:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                if (m_pMaster->IsRedConfigured())
                {
                CEcBusSlave* pBusSlaveRedPort = m_pMaster->m_pBusTopology->GetBusSlaveRedPort();

                    /* refresh port information */
                    if (!m_bLineBreakAtStart && (EC_NULL != pBusSlaveRedPort))
                    {
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_DC_REDHANDLING, EC_CMD_TYPE_FPRD, pBusSlaveRedPort->GetFixedAddress(), 
                            ECREG_DL_STATUS, sizeof(EC_T_WORD), EC_NULL
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                }
                /* determine next state */
                if (0 < m_dwEcatCmdSent)
                {
                    m_eCurState = ecdcsm_redrefreshdlstatus_wait;
                }
                else
                {
                    m_eCurState = ecdcsm_redrefreshdlstatus_done;
                }
            } break;
        case ecdcsm_redrefreshdlstatus_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurState = ecdcsm_redrefreshdlstatus_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_redrefreshdlstatus_done:
            {
                /* check for abort */
                if (AbortState())
                {
#if (defined INCLUDE_RED_DEVICE)
                    if (m_pMaster->IsRedConfigured())
                    {
                        if (!m_bLineBreakAtStart)
                        {
                            NextState(ecdcsm_redopenport);
                        }
                    }
#endif
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
            } break;
#endif /* INCLUDE_RED_DEVICE */
        /* latch receive times (broadcast on Receive Time Port0) */
        case ecdcsm_latchrecvtimes:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* increment port delays measurement counter */
                m_nPortDelaysMeasCnt++;

                /* request latch receive time */
                m_pMaster->m_bLatchReceiveTimesRequest = EC_TRUE;

                /* determine next state */
                m_eCurState = ecdcsm_latchrecvtimes_wait;
            } break;
        case ecdcsm_latchrecvtimes_wait:
            {
                if (!m_pMaster->m_bLatchReceiveTimesRequest)
                {
                    /* determine next state */
                    m_eCurState = ecdcsm_latchrecvtimes_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_latchrecvtimes_done:
            {
                /* check for abort */
                if (AbortState())
                {
#if (defined INCLUDE_RED_DEVICE)
                    if (m_pMaster->IsRedConfigured())
                    {
                        if (!m_bLineBreakAtStart)
                        {
                            NextState(ecdcsm_redopenport);
                        }
                    }
#endif
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
            } break;
        /* read receive times */
        case ecdcsm_readrecvtimes:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pMaster->m_pBusTopology->GetBusSlaveList(); /* handle all slaves */

                /* determine next state */
                m_eCurState = ecdcsm_readrecvtimes_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecdcsm_readrecvtimes_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* we assume here that every slave controller has the receive time registers enabled */

                    /* skip slaves without RecvTime registers */
                    if (!m_pBusSlaveCur->IsRecvTimesSupported())
                    {
                        continue;
                    }
                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_DC_READRECVTIMES, EC_CMD_TYPE_FPRD, m_pBusSlaveCur->GetFixedAddress(),
                        ECM_DCS_REC_TIMEPORT0, 32, EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurState = ecdcsm_readrecvtimes_wait;   

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_readrecvtimes_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurState = ecdcsm_readrecvtimes_send;
                    }
                    else
                    {
                        m_eCurState = ecdcsm_readrecvtimes_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_readrecvtimes_done:
            {
                /* check for abort */
                if (AbortState())
                {
#if (defined INCLUDE_RED_DEVICE)
                    if (m_pMaster->IsRedConfigured())
                    {
                        if (!m_bLineBreakAtStart)
                        {
                            NextState(ecdcsm_redopenport);
                        }
                    }
#endif
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
                bContinueSM = EC_FALSE; /* leave state machine to reduce CPU load */
            } break;
        /* calculate port delays */
        case ecdcsm_calculateportdelays:
            {
                CalculatePortDelaysPrepare();
                
                /* determine next state */
                m_eCurState = ecdcsm_calculateportdelays_wait;
                bContinueSM = EC_TRUE;
            } break;
        case ecdcsm_calculateportdelays_wait:
            {
                /* calculate port delays */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL);
                     nSlaveSliceCount--,       m_pBusSlaveCur  = m_pBusSlaveCur->GetNext())
                {
                    CalculatePortDelays(m_pBusSlaveCur, m_nPortDelaysMeasCnt, m_nPortDelaysMeasCycles, (ecdcsm_dccontinuousmeasuring == m_eReqState));
                }
                /* determine next state */
                if (EC_NULL == m_pBusSlaveCur)
                {
                    m_eCurState = ecdcsm_calculateportdelays_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_calculateportdelays_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* determine next state */
                if ((ecdcsm_dcinit == m_eReqState) && (m_nPortDelaysMeasCnt < m_nPortDelaysMeasCycles))
                {
                    /* on dcinit, port delays are measured several times, latch receive times again */
                    NextState(ecdcsm_latchrecvtimes);
                }
                else
                {
                    NextState();

#if (defined INCLUDE_LOG_MESSAGES)
                    if (m_pMaster->m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE)
                    {
                        PrintPortDelays(m_pMaster->m_pBusTopology->GetBusSlaveList());
                    }
#endif
                }
                bContinueSM = EC_TRUE;
            } break;
        /* calculate propagation delays */
        case ecdcsm_calculatepropagdelays:
            {
                CalculatePropagDelaysPrepare();
                
                /* determine next state */
                m_eCurState = ecdcsm_calculatepropagdelays_wait;
                bContinueSM = EC_TRUE;
            } break;
        case ecdcsm_calculatepropagdelays_wait:
            {
                /* calculate port delays */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (EC_NULL != m_oCalculatePropagDelaysMainContext.pBusSlaveCur);
                     nSlaveSliceCount--)
                {
                    CalculatePropagDelays(&m_oCalculatePropagDelaysMainContext);
                }
#if (defined INCLUDE_RED_DEVICE)
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (EC_NULL != m_oCalculatePropagDelaysRedContext.pBusSlaveCur);
                     nSlaveSliceCount--)
                {
                    CalculatePropagDelays(&m_oCalculatePropagDelaysRedContext);
                }
#endif /* INCLUDE_RED_DEVICE */
                /* determine next state */
                if (   (EC_NULL == m_oCalculatePropagDelaysMainContext.pBusSlaveCur)
#if (defined INCLUDE_RED_DEVICE)
                    && (EC_NULL == m_oCalculatePropagDelaysRedContext.pBusSlaveCur)
#endif
                    )
                {
                    m_eCurState = ecdcsm_calculatepropagdelays_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_calculatepropagdelays_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* determine next state */
                NextState();

#if (defined INCLUDE_LOG_MESSAGES)
                if (m_pMaster->m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE)
                {
                    PrintPropagDelays(m_pMaster->m_pBusTopology->GetBusSlaveList());
                }
#endif
                bContinueSM = EC_TRUE;
            } break;
#if (defined INCLUDE_RED_DEVICE)
        /* (re)open redundancy port */
        case ecdcsm_redopenport:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                if (m_pMaster->IsRedConfigured())
                {
                CEcBusSlave* pBusSlaveRedPort = m_pMaster->m_pBusTopology->GetBusSlaveRedPort();
                EC_T_WORD    wRedPort         = m_pMaster->m_pBusTopology->GetRedPort();

                    /* (re)open redundancy port */
                    if (!m_bLineBreakAtStart && (EC_NULL != pBusSlaveRedPort))
                    {
                    EC_T_DWORD dwDLControl    = pBusSlaveRedPort->GetDlControl();
                    EC_T_DWORD dwDLControlFrm = 0;

                        /* prepare DL control values */
                        if (pBusSlaveRedPort->GetAutoCloseSupported())
                        {
                            dwDLControl &= ~ECM_DLCTRL_LOOP_PORTX_MASK(wRedPort);
                            dwDLControl |= (ECM_DLCTRL_LOOP_PORT_VAL_AUTOCLOSE<<ECM_DLCTRL_LOOP_PORTX_SHIFT(wRedPort));
                        }
                        else
                        {
                            dwDLControl &= ~ECM_DLCTRL_LOOP_PORTX_MASK(wRedPort);
                            dwDLControl |= (ECM_DLCTRL_LOOP_PORT_VAL_AUTO<<ECM_DLCTRL_LOOP_PORTX_SHIFT(wRedPort));
                        }
                        EC_SET_FRM_DWORD((EC_T_BYTE*)&dwDLControlFrm, dwDLControl);

                        /* queue EtherCAT command in a single frame */
                        m_pMaster->EcatFlushCurrPendingSlaveFrame();
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_DC_REDHANDLING, EC_CMD_TYPE_FPWR, pBusSlaveRedPort->GetFixedAddress(),
                            ECREG_DL_CONTROL, sizeof(EC_T_DWORD), (EC_T_VOID*)&dwDLControlFrm
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                        m_pMaster->EcatFlushCurrPendingSlaveFrame();

                        /* send a NOP command in a single frame to protect for frameloss (force to drop the previous pending command) */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_DC_REDOPENCLOSEPORT, EC_CMD_TYPE_NOP, pBusSlaveRedPort->GetFixedAddress(),
                            0, 0, EC_NULL
                            );
                        OsDbgAssert(EC_E_NOERROR == dwRes);

                        /* this command is not monitored */
                        m_pMaster->EcatFlushCurrPendingSlaveFrameNoMonitoring();
                    }
                }
                /* determine next state */
                if (0 < m_dwEcatCmdSent)
                {
                    m_eCurState = ecdcsm_redopenport_wait;
                }
                else
                {
                    m_eCurState = ecdcsm_redopenport_done;
                }
            } break;
        case ecdcsm_redopenport_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurState = ecdcsm_redopenport_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_redopenport_done:
            {
                /* check for abort */
                if (AbortState())
                {
#if (defined INCLUDE_RED_DEVICE)
                    if (m_pMaster->IsRedConfigured())
                    {
                        if (!m_bLineBreakAtStart)
                        {
                            NextState(ecdcsm_redenableecatevent);
                        }
                    }
#endif
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
            } break;
        /* (re)enable ecat event on slave where redundancy link is connected */
        case ecdcsm_redenableecatevent:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                if (m_pMaster->IsRedConfigured())
                {
                CEcBusSlave* pBusSlaveRedPort = m_pMaster->m_pBusTopology->GetBusSlaveRedPort();

                    if (!m_bLineBreakAtStart && (EC_NULL != pBusSlaveRedPort))
                    {
                        /* refresh port information */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_DC_REDHANDLING, EC_CMD_TYPE_FPRD, pBusSlaveRedPort->GetFixedAddress(), 
                            ECREG_DL_STATUS, sizeof(EC_T_WORD), EC_NULL
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;

                        /* (re)enable Ecat event */
                        if (pBusSlaveRedPort->GetEcatEventSupported())
                        {
                        EC_T_WORD wEcatEventMask    = pBusSlaveRedPort->GetEcatEventMask();
                        EC_T_WORD wEcatEventMaskFrm = 0;

                            /* prepare interrupt configuration value */
                            wEcatEventMask |= ECM_ECATEVENT_DLSTATUS;
                            EC_SET_FRM_WORD(&wEcatEventMaskFrm, wEcatEventMask);

                            /* queue EtherCAT command */
                            dwRes = m_pMaster->QueueRegisterCmdReq(
                                EC_NULL, INVOKE_ID_DC_REDHANDLING, EC_CMD_TYPE_FPWR, pBusSlaveRedPort->GetFixedAddress(),
                                ECREG_SLV_ECATEVENTMASK, sizeof(EC_T_WORD), &wEcatEventMaskFrm
                                );
                            if (EC_E_NOERROR != dwRes)
                            {
                                /* try again next cycle */
                                goto Exit;
                            }
                            m_dwEcatCmdPending++;
                            m_dwEcatCmdSent++;
                        }
                    }
                }
                /* determine next state */
                if (0 < m_dwEcatCmdSent)
                {
                    m_eCurState = ecdcsm_redenableecatevent_wait;
                }
                else
                {
                    m_eCurState = ecdcsm_redenableecatevent_done;
                }
            } break;
        case ecdcsm_redenableecatevent_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurState = ecdcsm_redenableecatevent_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_redenableecatevent_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* reactivate redundancy notification */
                m_pMaster->m_bRedNotifyActive = EC_TRUE;

                /* determine next state */
                NextState();
            } break;
#endif /* INCLUDE_RED_DEVICE */
        /* read reference time (stamped command) */
        case ecdcsm_readreferencetime:
            {
            	CEcBusSlave* poBusSlaveBusTimeRef = m_poDcm->GetBusSlaveBusTimeRef();

                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* reference clock could be deleted by the topology state machine */
                if (EC_NULL != poBusSlaveBusTimeRef)
                {
                    m_qwReferenceTimeHost = 0;
                    m_qwReferenceTimeBus  = 0;

                    /* queue EtherCAT command */
                    m_pMaster->EcatFlushCurrPendingSlaveFrame();
                    dwRes = m_pMaster->QueueEcatCmdReq(
                        EC_FALSE, EC_NULL, INVOKE_ID_DC_READREFTIME, EC_CMD_TYPE_FPRD, poBusSlaveBusTimeRef->GetFixedAddress(),
                        ECM_DCS_SYSTEMTIME, 8, EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;

                    /* flush the frame with timestamping */
                    m_pMaster->EcatFlushCurrPendingSlaveFrameStamp(DcmHostTimeStamp, m_pMaster, (EC_T_DWORD*)((EC_T_VOID*)&m_qwReferenceTimeHost));
                }
                /* determine next state */
                if (0 < m_dwEcatCmdSent)
                {
                    m_eCurState = ecdcsm_readreferencetime_wait;
                }
                else
                {
                    m_eCurState = ecdcsm_readreferencetime_done;
                }
            } break;
        case ecdcsm_readreferencetime_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurState = ecdcsm_readreferencetime_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_readreferencetime_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "ecdcsm_readreferencetime_done\n"));
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "HostTime = 0x%08X%08X\n", EC_HIDWORD(m_qwReferenceTimeHost), EC_LODWORD(m_qwReferenceTimeHost)));
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "BusTime  = 0x%08X%08X\n", EC_HIDWORD(m_qwReferenceTimeBus), EC_LODWORD(m_qwReferenceTimeBus)));

                /* determine next state */
                NextState();
            } break;
        /* write propagation delays */
        case ecdcsm_writepropagdelays:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                m_pBusSlaveCur = m_poDcm->GetBusSlaveBusTimeRef(); /* handle only time reference and slaves behind */

                /* determine next state */
                m_eCurState = ecdcsm_writepropagdelays_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecdcsm_writepropagdelays_send:
            {
            EC_T_DWORD dwPropagDelay = 0;

                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* skip non DC slaves */
                    if (!m_pBusSlaveCur->IsDcEnabled())
                    {
                        continue;
                    }
                    /* skip if DC not already initialized (propagation delay is set in ecdcsm_writesystimevalues) */
                    if (!m_pBusSlaveCur->IsDcInitialized())
                    {
                        continue;
                    }
                    /* set propagation delay */
                    dwPropagDelay = m_pBusSlaveCur->m_dwPropagDelay;
#ifdef EC_BIG_ENDIAN
                    dwPropagDelay = EC_DWORDSWAP(dwPropagDelay);
#endif
                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_DC_WRITEPROPAGDELAYS, EC_CMD_TYPE_FPWR, m_pBusSlaveCur->GetFixedAddress(),
                        ECM_DCS_SYSTIME_DELAY, sizeof(EC_T_DWORD), &dwPropagDelay
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurState = ecdcsm_writepropagdelays_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;

        case ecdcsm_writepropagdelays_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurState = ecdcsm_writepropagdelays_send;
                    }
                    else
                    {
                        m_eCurState = ecdcsm_writepropagdelays_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;

        case ecdcsm_writepropagdelays_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
                bContinueSM = EC_TRUE;
            } break;
        /* write system time values (system time offset and propagation delay) */
        case ecdcsm_writesystimevalues:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                m_pBusSlaveCur = m_poDcm->GetBusSlaveBusTimeRef(); /* handle only time reference and slaves behind */

                /* determine next state */
                m_eCurState = ecdcsm_writesystimevalues_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecdcsm_writesystimevalues_send:
            {
            EC_T_UINT64 qwSystemTimeOffset = 0;
            EC_T_DWORD  dwSystemTimeOffset = 0;
            EC_T_DWORD  dwPropagDelay      = 0;
            EC_T_BYTE   abyConfigData[0xc] = {0};

                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* skip non DC slaves */
                    if (!m_pBusSlaveCur->IsDcEnabled())
                    {
                        continue;
                    }
                    /* skip if DC already initialized */
                    if (m_pBusSlaveCur->IsDcInitialized())
                    {
                        continue;
                    }
                    /* calculate system time offset */
                    CalculateSystemTimeOffset(m_pBusSlaveCur);

                    /* clear memory */
                    OsMemset(abyConfigData, 0, sizeof(abyConfigData));

                    /* set propagation delay  */
                    dwPropagDelay = m_pBusSlaveCur->m_dwPropagDelay;
#ifdef EC_BIG_ENDIAN
                    dwPropagDelay = EC_DWORDSWAP(dwPropagDelay);
#endif
                    EC_SETDWORD((EC_T_VOID*)&(abyConfigData[8]), dwPropagDelay);

                    /* set system time offset */
                    qwSystemTimeOffset = m_pBusSlaveCur->GetSystemTimeOffset();
                    if (m_pBusSlaveCur->IsDC64Support())
                    {
#ifdef EC_BIG_ENDIAN
                        qwSystemTimeOffset = EC_QWORDSWAP(qwSystemTimeOffset);
#endif
                        EC_SETQWORD((EC_T_VOID*)&(abyConfigData[0]), qwSystemTimeOffset);
                    }
                    else
                    {
                        qwSystemTimeOffset = EC_MAKEQWORD(0, EC_LODWORD(qwSystemTimeOffset));
                        dwSystemTimeOffset = EC_LODWORD(qwSystemTimeOffset);
#ifdef EC_BIG_ENDIAN
                        dwSystemTimeOffset = EC_DWORDSWAP(dwSystemTimeOffset);
#endif
                        EC_SETDWORD((EC_T_VOID*)&(abyConfigData[0]), dwSystemTimeOffset);
                    }
                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_DC_WRITESYSTIMEVAL, EC_CMD_TYPE_FPWR, m_pBusSlaveCur->GetFixedAddress(),
                        ECM_DCS_SYSTIME_OFFSET, sizeof(abyConfigData), abyConfigData
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurState = ecdcsm_writesystimevalues_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;

        case ecdcsm_writesystimevalues_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurState = ecdcsm_writesystimevalues_send;
                    }
                    else
                    {
                        m_eCurState = ecdcsm_writesystimevalues_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;

        case ecdcsm_writesystimevalues_done:
            {
                /* activate cyclic distribution */
                m_bCycDistributionActive = EC_TRUE;

                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();

                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "ecdcsm_writesystimevalues_done\n"));
                for (pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveList(); EC_NULL != pBusSlave; pBusSlave = pBusSlave->GetNext())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "Slave=%4d - Offset=0x%08X%08X Delay=0x%08X\n", pBusSlave->GetFixedAddress(),
                        EC_HIDWORD(pBusSlave->GetSystemTimeOffset()), EC_LODWORD(pBusSlave->GetSystemTimeOffset()), pBusSlave->m_dwPropagDelay));
                }
                bContinueSM = EC_TRUE;
            } break;
#if (defined DEBUG_DC_SYSTIMEVALUES)
        /* for developpment purpose only */
        case ecdcsm_debugsystimevalues:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                m_pBusSlaveCur = m_poDcm->GetBusSlaveBusTimeRef(); /* handle only time reference and slaves behind */

                /* determine next state */
                m_eCurState = ecdcsm_debugsystimevalues_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecdcsm_debugsystimevalues_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* skip non DC slaves */
                    if (!m_pBusSlaveCur->IsDcEnabled())
                    {
                        continue;
                    }
                    /* skip if DC already initialized */
                    if (m_pBusSlaveCur->IsDcInitialized())
                    {
                        continue;
                    }
                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_DC_DBGSYSTIMEVAL, EC_CMD_TYPE_FPRD, m_pBusSlaveCur->GetFixedAddress(),
                        ECM_DCS_SYSTIME_OFFSET, (8+sizeof(EC_T_DWORD)), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurState = ecdcsm_debugsystimevalues_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_debugsystimevalues_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurState = ecdcsm_debugsystimevalues_send;
                    }
                    else
                    {
                        m_eCurState = ecdcsm_debugsystimevalues_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_debugsystimevalues_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
                bContinueSM = EC_TRUE;
            } break;
#endif /* DEBUG_DC_SYSTIMEVALUES */
        /* configure DC controllers */
        case ecdcsm_configdccontrollers1:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                m_pBusSlaveCur = m_poDcm->GetBusSlaveBusTimeRef(); /* handle only time reference and slaves behind */

                /* determine next state */
                m_eCurState = ecdcsm_configdccontrollers1_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecdcsm_configdccontrollers1_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    EC_T_WORD wFilterDepth = DC_FILTER_DEPTH_STD_VALUE;

                    /* skip non DC slaves */
                    if (!m_pBusSlaveCur->IsDcEnabled())
                    {
                        continue;
                    }
                    /* skip if DC already initialized and reference clock didn't change */
                    if (m_pBusSlaveCur->IsDcInitialized() && !m_bBusSlaveRefClockChanged)
                    {
                        continue;
                    }
                    if (!m_poDcm->UseDcLoopControlStdValues() && !m_pBusSlaveCur->UseDcLoopControlStdValues())
                    {
                        if (m_pBusSlaveCur == m_pBusSlaveRefClock)
                        {
                            wFilterDepth = DC_FILTER_DEPTH_BUSSHIFT_REFCLOCK_VALUE;
                        }
                    }
#ifdef EC_BIG_ENDIAN
                    wFilterDepth = EC_WORDSWAP(wFilterDepth);
#endif
                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_DC_CFGDCCONTROLLERS, EC_CMD_TYPE_FPWR, m_pBusSlaveCur->GetFixedAddress(),
                        ECM_DCS_SYSTIMEDIFF_FILTERDEPTH, sizeof(wFilterDepth), &wFilterDepth
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurState = ecdcsm_configdccontrollers1_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_configdccontrollers1_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurState = ecdcsm_configdccontrollers1_send;
                    }
                    else
                    {
                        m_eCurState = ecdcsm_configdccontrollers1_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_configdccontrollers1_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
                bContinueSM = EC_TRUE;
            } break;
        case ecdcsm_configdccontrollers2:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                m_pBusSlaveCur = m_poDcm->GetBusSlaveBusTimeRef(); /* handle only time reference and slaves behind */

                /* determine next state */
                m_eCurState = ecdcsm_configdccontrollers2_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecdcsm_configdccontrollers2_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    EC_T_WORD wFilterSettings = DC_SPEEDCOUNT_START_STD_VALUE;

                    /* skip non DC slaves */
                    if (!m_pBusSlaveCur->IsDcEnabled())
                    {
                        continue;
                    }
                    /* skip if DC already initialized and reference clock didn't change */
                    if (m_pBusSlaveCur->IsDcInitialized() && !m_bBusSlaveRefClockChanged)
                    {
                        continue;
                    }
                    if (!m_poDcm->UseDcLoopControlStdValues() && !m_pBusSlaveCur->UseDcLoopControlStdValues())
                    {
                        wFilterSettings = DC_SPEEDCOUNT_START_BUSSHIFT_VALUE;
                    }
#ifdef EC_BIG_ENDIAN
                    wFilterSettings = EC_WORDSWAP(wFilterSettings);
#endif
                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_DC_CFGDCCONTROLLERS, EC_CMD_TYPE_FPWR, m_pBusSlaveCur->GetFixedAddress(),
                        ECM_DCS_SPEEDCOUNT_START, sizeof(EC_T_WORD), &wFilterSettings
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurState = ecdcsm_configdccontrollers2_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_configdccontrollers2_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurState = ecdcsm_configdccontrollers2_send;
                    }
                    else
                    {
                        m_eCurState = ecdcsm_configdccontrollers2_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_configdccontrollers2_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
                bContinueSM = EC_TRUE;
            } break;
        /* start the 64bit bus time emulation */
        case ecdcsm_startbustime64emul:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                if (eDcmMode_LinkLayerRefClock == m_eDcmMode)
                {
                EC_T_LINK_IOCTLPARMS oIoCtlParms;

                    oIoCtlParms.pbyInBuf      = EC_NULL;
                    oIoCtlParms.dwInBufSize   = 0;
                    oIoCtlParms.pbyOutBuf     = (EC_T_BYTE*)&m_qwReferenceTimeBus;
                    oIoCtlParms.dwOutBufSize  = sizeof(EC_T_UINT64);
                    oIoCtlParms.pdwNumOutData = EC_NULL;
                    dwRes = m_pMaster->m_poEcDevice->LinkIoctl(EC_LINKIOCTL_GETTIME, &oIoCtlParms);
                    OsDbgAssert(dwRes == EC_E_NOERROR);
                }
                else
                {
                    m_qwReferenceTimeBus = 0;

                    /* look for the first 64bit DC slave starting from the reference clock and read its system time */
                    for (pBusSlave = m_poDcm->GetBusSlaveBusTimeRef(); EC_NULL != pBusSlave; pBusSlave = pBusSlave->GetNext())
                    {
                        if (pBusSlave->IsDcEnabled() && pBusSlave->IsDC64Support())
                        {
                            break;
                        }
                    }
                    if (EC_NULL != pBusSlave)
                    {
                        /* queue EtherCAT command */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_DC_READREFTIME, EC_CMD_TYPE_FPRD, pBusSlave->GetFixedAddress(),
                            ECM_DCS_SYSTEMTIME, 8, EC_NULL
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                }
                /* determine next state */
                if (0 < m_dwEcatCmdSent)
                {
                    m_eCurState = ecdcsm_startbustime64emul_wait;
                }
                else
                {
                    m_eCurState = ecdcsm_startbustime64emul_done;
                }
            } break;
        case ecdcsm_startbustime64emul_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurState = ecdcsm_startbustime64emul_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_startbustime64emul_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* initialize 64bit bus time emulation */
                if (0 == m_qwReferenceTimeBus)
                {
                    EC_T_UINT64 qwHostTime = 0;
                    OsSystemTimeGet(&qwHostTime);

                    /* cyclic distribution must be active to make sure m_qwBusTime is valid */
                    OsDbgAssert(EC_TRUE == m_bCycDistributionActive);

                    /* set bus time high dword to host time high dword */
                    SetBusTime(EC_MAKEQWORD(EC_HIDWORD(qwHostTime), EC_LODWORD(m_qwBusTime)));
                }
                else
                {
                    /* set bus time to first 64bit time on bus */
                    SetBusTime(m_qwReferenceTimeBus);
                }
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "ecdcsm_startbustime64emul_done\n"));
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "ReferenceTimeBus = 0x%08X%08X\n", EC_HIDWORD(m_qwReferenceTimeBus), EC_LODWORD(m_qwReferenceTimeBus)));
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "BusTime          = 0x%08X%08X\n", EC_HIDWORD(m_qwBusTime), EC_LODWORD(m_qwBusTime)));

                /* determine next state */
                NextState();
            } break;
        /* read reference clock sync pulse length DCM */
        case ecdcsm_dcmreadpulselength:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                if (EC_NULL != m_poDcm->GetCfgSlaveSyncRef())
                {
                    /* get bus slave */
                    pBusSlave = m_poDcm->GetCfgSlaveSyncRef()->m_pBusSlave;
                    if (EC_NULL != pBusSlave)
                    {
                        /* queue EtherCAT command */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_DC_READSYNCPULSEWDT, EC_CMD_TYPE_FPRD, pBusSlave->GetFixedAddress(),
                            ECM_DCS_DC_SYNCPULSEWIDTH, 2, EC_NULL
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                }
                /* determine next state */
                if (0 < m_dwEcatCmdSent)
                {
                    m_eCurState = ecdcsm_dcmreadpulselength_wait;
                }                                           
                else
                {
                    m_eCurState = ecdcsm_dcmreadpulselength_done;
                }
            } break;
        case ecdcsm_dcmreadpulselength_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurState = ecdcsm_dcmreadpulselength_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_dcmreadpulselength_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* check for retry */
                if (RetryState())
                {
                    break;
                }
                /* determine next state */
                NextState();
                bContinueSM = EC_TRUE;
            } break;
        /* ARMW Burst (FRMW) to saturize DC controllers */
        case ecdcsm_armwburst:
            {
            EC_T_BOOL bNewSlaveInitialized = EC_FALSE;
            
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                
                /* walk through bus slave list */
                for (pBusSlave = m_poDcm->GetBusSlaveBusTimeRef();EC_NULL != pBusSlave; pBusSlave = pBusSlave->GetNext())
                {
                    /* skip non DC slaves */
                    if (!pBusSlave->IsDcEnabled())
                    {
                        continue;
                    }
                    /* skip if DC already initialized */
                    if (pBusSlave->IsDcInitialized())
                    {
                        continue;
                    }
                    bNewSlaveInitialized = EC_TRUE;
                    pBusSlave->SetDcInitialized(EC_TRUE);
                }
                if (eDcmMode_MasterRefClock == m_eDcmMode)
                {
                    /* minimize the error on start */
                    ((CEcDcmMasterRefClock*)m_poDcm)->SetMasterTime(m_qwBusTime);
                }
                /* determine next state */
                if (bNewSlaveInitialized)
                {
                    m_eCurState = ecdcsm_armwburst_send;
                }
                else
                {
                    m_eCurState = ecdcsm_armwburst_done;
                    bContinueSM = EC_TRUE;
                }                
            } break;
        case ecdcsm_armwburst_send:
            {
                if (EC_NULL != m_poDcm->GetBusSlaveBusTimeRef())
                {
                EC_T_DWORD dwBurstBulkCnt;
                EC_T_DWORD dwBurstBulkDec;

                    if (m_bBulkInLinkLayer)
                    {
                        /* only DC burst should be repeated, flush the current frame */
                        m_pMaster->EcatFlushCurrPendingSlaveFrame();

                        dwBurstBulkCnt = 1;
                        dwBurstBulkDec = m_dwBurstBulk;
                    }
                    else
                    {
                        if (eDcmMode_LinkLayerRefClock == m_eDcmMode)
                        {
                            /* flush the current frame to assure the time stamp offset fits to header length */
                            m_pMaster->EcatFlushCurrPendingSlaveFrame();
                        }

                        dwBurstBulkCnt = m_dwBurstBulk;
                        dwBurstBulkDec = 1;
                    }
                    for (; (0 < m_dwBurstCycles) && (0 < dwBurstBulkCnt); dwBurstBulkCnt--)
                    {
                        if (m_dwBurstCycles > dwBurstBulkDec)
                        {
                            m_dwBurstCycles = m_dwBurstCycles - dwBurstBulkDec;
                        }
                        else
                        {
                            m_dwBurstCycles = 0;
                        }
                        switch (m_eDcmMode)
                        {
                        case eDcmMode_LinkLayerRefClock:
                        {
                        EC_T_WORD wHeaderLen = ETHERNET_88A4_FRAME_LEN + ETYPE_EC_CMD_HEADER_LEN;

#if (defined VLAN_FRAME_SUPPORT)
                            if (m_pMaster->m_bVLANEnabled)
                            {
                                wHeaderLen += ETYPE_VLAN_HEADER_LEN;
                            }
#endif
                            /* queue EtherCAT command */
                            dwRes = m_pMaster->QueueEcatCmdReq(
                                EC_TRUE, EC_NULL, INVOKE_ID_DC_ARMWBURST, EC_CMD_TYPE_BWR, 0,
                                ECM_DCS_SYSTEMTIME, 8, EC_NULL
                                );
                            if (EC_E_NOERROR != dwRes)
                            {
                                /* try again next cycle */
                                goto Exit;
                            }
                            m_dwEcatCmdPending = (EC_T_DWORD)(m_dwEcatCmdPending + (m_bBulkInLinkLayer?m_dwBurstBulk:1));
                            m_dwEcatCmdSent    = (EC_T_DWORD)(m_dwEcatCmdSent    + (m_bBulkInLinkLayer?m_dwBurstBulk:1));

                            /* flush the frame because only one command can be processed at once */
                            /* 64bit timestamp requested in data field behind command header     */
                            m_pMaster->EcatFlushCurrPendingSlaveFrameStamp(EC_NULL, EC_NULL, EC_NULL, wHeaderLen, 8, m_bBulkInLinkLayer?m_dwBurstBulk:0);
                        } break;
                        case eDcmMode_MasterRefClock:
                        {
                            /* only monitor burst cycles, burst is done by cyclic frames */
                            m_dwEcatCmdSent    = (EC_T_DWORD)(m_dwEcatCmdSent    + (m_bBulkInLinkLayer?m_dwBurstBulk:1));
                        } break;
                        default:
                        {
                            /* queue EtherCAT command */
                            dwRes = m_pMaster->QueueEcatCmdReq(
                                EC_TRUE, EC_NULL, INVOKE_ID_DC_ARMWBURST, EC_CMD_TYPE_FRMW, m_poDcm->GetBusSlaveBusTimeRef()->GetFixedAddress(), 
                                ECM_DCS_SYSTEMTIME, 8, EC_NULL
                                );
                            if (EC_E_NOERROR != dwRes)
                            {
                                /* try again next cycle */
                                goto Exit;
                            }
                            m_dwEcatCmdPending = (EC_T_DWORD)(m_dwEcatCmdPending + (m_bBulkInLinkLayer?m_dwBurstBulk:1));
                            m_dwEcatCmdSent    = (EC_T_DWORD)(m_dwEcatCmdSent    + (m_bBulkInLinkLayer?m_dwBurstBulk:1));

                            /* flush the frame because only one command can be processed at once */
                            m_pMaster->EcatFlushCurrPendingSlaveFrameRepeatInLinkLayer(m_bBulkInLinkLayer?m_dwBurstBulk:0);
                        } break;
                        }
                    }
                }
                else
                {
                    /* abort burst cycles */
                    m_dwBurstCycles = 0;
                }
                /* determine next state */
                if (0 < m_dwEcatCmdSent)
                {
                    m_eCurState = ecdcsm_armwburst_wait;
                }
                else
                {
                    m_eCurState = ecdcsm_armwburst_done;
                }
            } break;
        case ecdcsm_armwburst_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* check for abort */
                    if (AbortState())
                    {
                        break;
                    }
                    /* determine next state */
                    if (0 < m_dwBurstCycles)
                    {
                        m_eCurState = ecdcsm_armwburst_send;
                    }
                    else
                    {
                        m_eCurState = ecdcsm_armwburst_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecdcsm_armwburst_done:
            {
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* don't retry DC burst */

                /* determine next state */
                NextState();
                bContinueSM = EC_TRUE;
            } break;
        /* wait for InSync */
        case ecdcsm_waitforinsync:
            {
            EC_T_BOOL bWait = EC_FALSE;

                /* activate sync window monitoring, if DCM used, it will be done at the end of the PS transition */
  				if ((eDcmMode_Off == m_eDcmMode) || (DEVICE_STATE_SAFEOP <= m_pMaster->GetMasterDeviceState()))
                {
                    m_bSyncWindowMonitoringActive = m_bSyncWindowMonitoringConfigured;
                }
                /* enable notification during initialization to get first InSync notification */
                if (ecdcsm_dcinit == m_eReqState)
                {
                    m_bInSyncNotifyActive = EC_TRUE;
                }
                /* check for abort */
                if (AbortState())
                {
                    break;
                }
                /* wait for deviation limit reached if needed */
                if (!m_bNoDCSlaveWasConnected && m_bSyncWindowMonitoringConfigured)
                {
                	if (m_bSyncWindowMonitoringActive)
                	{
						if (!m_oTimeout.IsStarted())
						{
							m_oTimeout.Start(m_dwTimeout, m_pMaster->GetMsecCounterPtr());
						}
						if (m_oTimeout.IsElapsed())
						{
							m_pMaster->NotifyDcSlvStatus(EC_FALSE, EC_FALSE, 0, EC_NULL);
						}
						else
						{
							/* wait for deviation limit reached if needed */
							if (!m_bInSync)
							{
								/* wait for InSync */
								bWait = EC_TRUE;
							}
						}
                	}
                }
                else
                {
                    /* monitoring off, assume "in sync" */
                    m_bInSync = EC_TRUE;
                }
                /* determine next state */
                if (!bWait && (0 == m_dwEcatCmdPending))
                {
                    /* refresh slave sync notification descriptor */
                    m_oSlaveSyncNotification.IsInSync    = m_bInSync;
                    m_oSlaveSyncNotification.IsNegative  = m_bDeviationIsNegative;
                    m_oSlaveSyncNotification.dwDeviation = m_dwDeviation;

                    /* refresh master object dictionary */
#if (defined INCLUDE_MASTER_OBD)
                    m_pMaster->m_pSDOService->EntrySet_SetDCInSync(m_bInSync);
#endif
                    m_oTimeout.Stop();
                    NextState();
                    bContinueSM = EC_TRUE;
                }
            } break;
        /* target state, DC Init */
        case ecdcsm_dcinit:
            {
                if (EC_E_BUSY == m_dwDCResult)
                {
                    /* DC initialization done */
                    m_bDCInitialized = EC_TRUE;

                    /* enable acyclic distribution to improve InSync behavior */
                    if (((eDcmMode_BusShift == m_eDcmMode) || (eDcmMode_Dcx == m_eDcmMode))
                        && m_bAcycDistributionEnabled 
                        && (500 <= m_pMaster->GetBusCycleTimeUsec()))
                    {
                        m_bAcycDistributionActive = EC_TRUE;
                    }
                    m_dwDCResult = EC_E_NOERROR;
                }
                if (m_bNoDCSlaveWasConnected)
                {
                    /* trigger DCM once */
                    m_poDcm->DistributeSystemTimeProcess(EC_FALSE, 0, 0);
                }
                /* activate continuous measurement */
                m_bContinuousMeasuringActive = EC_TRUE;

#if (defined INCLUDE_MASTER_OBD)
                m_pMaster->SDOEntrySet_SetDCBusy(EC_FALSE);
#endif
                m_pCurRequest->dwResult = m_dwDCResult;
                m_pCurRequest = EC_NULL;
            } break;
        /* target state, DC Sync newly connected slaves */
        case ecdcsm_dctopologychanged:
            {
                if (EC_E_BUSY == m_dwDCResult)
                {
                    m_dwDCResult = EC_E_NOERROR;
                }
                if (m_bNoDCSlaveWasConnected)
                {
                    /* trigger DCM once */
                    m_poDcm->DistributeSystemTimeProcess(EC_FALSE, 0, 0);
                }
                /* reactivate continuous measurement */
                m_bContinuousMeasuringActive = EC_TRUE;

                /* reactivate notification */
                m_bInSyncNotifyActive = EC_TRUE;

#if (defined INCLUDE_RED_DEVICE)
                if (m_pMaster->IsRedConfigured())
                {
                CEcBusSlave* pBusSlaveRedPort = m_pMaster->m_pBusTopology->GetBusSlaveRedPort();

                    /* calculate propagation delay for red side */
                    m_nRedPropagDelay = 0;

                    /* start from red port slave and go back to line break slave, following the topology */
                    if (EC_NULL != pBusSlaveRedPort)
                    {
                    EC_T_DWORD dwLastPropagDelay = 0;
                    EC_T_DWORD dwLastLineDelay   = 0;

                        for (pBusSlave = pBusSlaveRedPort; EC_NULL != pBusSlave; pBusSlave = pBusSlave->m_apBusSlaveChildren[ESC_PORT_A])
                        {
                            m_nRedPropagDelay += dwLastLineDelay;

                            dwLastLineDelay    = pBusSlave->m_adwLineDelay[ESC_PORT_A];
                            dwLastPropagDelay  = pBusSlave->m_dwPropagDelay;
                        }
                        m_nRedPropagDelay -= dwLastPropagDelay;
                    }
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "RedPropagDelay: %4d\n", m_nRedPropagDelay));
                }
#endif
#if (defined INCLUDE_MASTER_OBD)
                m_pMaster->SDOEntrySet_SetDCBusy(EC_FALSE);
#endif
                m_pCurRequest->dwResult = m_dwDCResult;
                m_pCurRequest = EC_NULL;
            } break;
        /* target state, continuous run-time measuring */
        case ecdcsm_dccontinuousmeasuring:
            {
                if (EC_E_BUSY == m_dwDCResult)
                {
                    m_dwDCResult = EC_E_NOERROR;
                }
#if (defined INCLUDE_MASTER_OBD)
                m_pMaster->SDOEntrySet_SetDCBusy(EC_FALSE);
#endif
                m_pCurRequest->dwResult = m_dwDCResult;
                m_pCurRequest = EC_NULL;
            } break;
        default:
            {
                OsDbgAssert(EC_FALSE);
                m_pCurRequest = EC_NULL;
                m_dwDCResult = EC_E_ERROR;
                m_eCurState  = m_eReqState;
            } /* default */
        } /* switch m_eCurState */

#ifdef INCLUDE_LOG_MESSAGES
        if (((m_eCurState != eOldState) && byDbgLvl) && (ecdcsm_armwburst_wait != m_eCurState))
        {
            EC_DBGMSGL(( 
                byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
                "DcSM('%s'): %s -> %s\n", ESTATESText(m_eReqState), ESTATESText(eOldState), ESTATESText(m_eCurState)));
        }
#endif
    } /* while( bContinueSM)  */

	/* monitoring commands are only in SafeOP and OP cyclic commands       */
	/* EC_IOCTL_ADD_BRD_SYNC_WINDOW_MONITORING includes it in every states */
    if (m_bSyncWindowMonitoringActive && (DEVICE_STATE_SAFEOP > m_pMaster->GetMasterHighestDeviceState()) && !m_pMaster->GetAddBrdSyncWindowMonitoring() && (0 == m_dwEcatCmdPending))
	{
	EC_T_WORD wSyncWindowMonitoringFixedAddr = GetSyncWindowMonitoringFixedAddr();

		if (INVALID_FIXED_ADDR != wSyncWindowMonitoringFixedAddr)
		{
			/* queue EtherCAT command */
			dwRes = m_pMaster->QueueRegisterCmdReq(
				EC_NULL, INVOKE_ID_DC_WAITFORINSYNC, EC_CMD_TYPE_FPRD, wSyncWindowMonitoringFixedAddr,
				ECREG_SYSTIME_DIFF, 4, EC_NULL
				);
			if (EC_E_NOERROR != dwRes)
			{
				/* try again next cycle */
				goto Exit;
			}
			m_dwEcatCmdPending++;
		}
	}

Exit:
    /* performance measurement point */
    PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_DCSM);

    return;
}

EC_T_VOID CEcDistributedClocks::CalculatePortDelaysPrepare(EC_T_VOID)
{
    m_pBusSlaveCur = m_pMaster->m_pBusTopology->GetBusSlaveList(); /* handle all slaves */
    return;
}

EC_T_VOID CEcDistributedClocks::CalculatePortDelays(CEcBusSlave* pBusSlave, EC_T_INT nMeasCnt, EC_T_INT nMeasCycles, EC_T_BOOL bCumulateDelay)
{
    /* reset port, line and propagation delays */
    if (1 == nMeasCnt)
    {
        pBusSlave->ResetLineDelays();

        /* cumulate propagation delay (e.g. for continuous measuring) */
        if (!bCumulateDelay)
        {
            pBusSlave->ResetPortDelays();
            pBusSlave->m_dwPropagDelay = 0;
        }
    }
    /* calculate port delays */
    pBusSlave->CalculatePortDelays((nMeasCnt == nMeasCycles) ? nMeasCycles : 0);

    return;
}

EC_T_VOID CEcDistributedClocks::CalculatePropagDelaysPrepare(EC_T_VOID)
{
    OsMemset(&m_oCalculatePropagDelaysMainContext, 0, sizeof(EC_T_DC_CALCULATEPROPAGDELAYS_CONTEXT));

#if (defined INCLUDE_RED_DEVICE)
    OsMemset(&m_oCalculatePropagDelaysRedContext, 0, sizeof(EC_T_DC_CALCULATEPROPAGDELAYS_CONTEXT));
    if (0 != m_pMaster->m_wRedNumSlavesOnRed)
    {
        CEcBusSlave* pBusSlaveRedPort = m_pMaster->m_pBusTopology->GetBusSlaveRedPort();
        EC_T_WORD    wRedPort = m_pMaster->m_pBusTopology->GetRedPort();

        if (EC_NULL != pBusSlaveRedPort)
        {
            m_oCalculatePropagDelaysRedContext.pBusSlaveRef = (EC_NULL != m_pRedBusSlaveRefClock) ? m_pRedBusSlaveRefClock : m_poDcm->GetBusSlaveBusTimeRef();
            m_oCalculatePropagDelaysRedContext.pBusSlaveCur = pBusSlaveRedPort;
            m_oCalculatePropagDelaysRedContext.wPortInCur = wRedPort;
            m_oCalculatePropagDelaysRedContext.pBusSlaveCur->SetPortSmProc(ESC_PORT_A);
        }
    }
    if (!m_bLineBreakAtStart || (0 != m_pMaster->m_wRedNumSlavesOnMain))
#endif
    {
        if (EC_NULL != m_pMaster->m_pBusTopology->GetBusSlaveList())
        {
            m_oCalculatePropagDelaysMainContext.pBusSlaveRef = m_poDcm->GetBusSlaveBusTimeRef();
            m_oCalculatePropagDelaysMainContext.pBusSlaveCur = m_pMaster->m_pBusTopology->GetBusSlaveList();
            m_oCalculatePropagDelaysMainContext.wPortInCur = ESC_PORT_A;
            m_oCalculatePropagDelaysMainContext.pBusSlaveCur->SetPortSmProc(ESC_PORT_A);
        }
    }

    return;
}

/***************************************************************************************************/
/**
\brief  Calculate line delays between last and current DC slave and propagation delay related 
        to reference bus slave until current bus slave

        Slaves with incoherent times (eg. ESC20) are considered as cable between slaves with coherent times. They only get an extrapolated propagation delay.
        Last DC slave should always have coherent port receive times. This make possible to extrapolate propagation delay without error cumulation.
        
*/
EC_T_VOID CEcDistributedClocks::CalculatePropagDelays(EC_T_DC_CALCULATEPROPAGDELAYS_CONTEXT* pContext)
{
EC_T_DWORD   dwLineDelay   = 0;
EC_T_WORD    wPortOut      = ESC_PORT_INVALID;
EC_T_WORD    wPortInNext   = ESC_PORT_INVALID;
CEcBusSlave* pBusSlaveNext = EC_NULL;
EC_T_BOOL    bRecvTimesCoherent = pContext->pBusSlaveCur->AreRecvTimesCoherent();

    /* process next bus slave following frame processing order */
    m_pMaster->m_pBusTopology->GetNextBusSlaveByPropagation(pContext->pBusSlaveCur, pContext->wPortInCur, EC_FALSE, &wPortOut, &pBusSlaveNext, &wPortInNext);
    if (!pContext->pBusSlaveCur->IsRecvTimesSupported())
    {
        goto Exit;
    }
    /* line delays are calculated between last and current DC slave */
    if (EC_NULL != pContext->pBusSlaveLast)
    {
        /* calculate line delays */
        if (!pContext->pBusSlaveCur->IsDCSmProc())
        {
            /* forward direction */

            if (bRecvTimesCoherent)
            {
                /* divide the line delay symmetrical between last and current DC slave */
                dwLineDelay = (pContext->pBusSlaveLast->m_adwPortDelay[pContext->wPortOutLast] - pContext->pBusSlaveCur->m_dwSlaveDelay) / 2;

                /* store line delay for backward direction */
                pContext->pBusSlaveLast->m_adwLineDelay[pContext->wPortOutLast] = dwLineDelay;
                pContext->pBusSlaveCur->m_adwLineDelay[pContext->wPortInCur] = dwLineDelay;
            }
            else
            { 
                /* in case of incohrerent receive time use a typically line delay of a MII port */
                dwLineDelay = LINE_DELAY_MII_PORT_TYPICAL;
            }
        }
        else
        {
            /* backward direction */

            if (pContext->pBusSlaveCur == pContext->pBusSlaveLast)
            {
                /* no DC slave behind a DC slave port, consider port delay as line delay */
                pContext->pBusSlaveLast->m_adwLineDelay[pContext->wPortOutLast] = pContext->pBusSlaveLast->m_adwPortDelay[pContext->wPortOutLast];
            }
            dwLineDelay = pContext->pBusSlaveLast->m_adwLineDelay[pContext->wPortOutLast];
        }
        /* calculate propagation delay */
        if (pContext->bBehindBusSlaveRef)
        {
            /* select real propagation delay or extrapolated one based on receive times coherency */
            EC_T_DWORD*  pdwPropagDelay = bRecvTimesCoherent ? &pContext->dwPropagDelay : &pContext->dwPropagDelayExtrapolated;

            /* add line delay */
            *pdwPropagDelay += dwLineDelay;

            if (ESC_PORT_A == pContext->wPortInCur)
            {
                /* set propagation delay of current DC slave */
                pContext->pBusSlaveCur->m_dwPropagDelay = *pdwPropagDelay;

                /* add processing unit delay of current DC slave to propagation delay */
                *pdwPropagDelay += pContext->pBusSlaveCur->m_dwProcessingUnitDelay;
            }
            /* extrapolated propagation delay must always be up to date */
            pContext->dwPropagDelayExtrapolated = *pdwPropagDelay;
        }
    }
    /* monitor reference bus slave to start propagation delay calculation */
    if (!pContext->bBehindBusSlaveRef && (pContext->pBusSlaveCur == pContext->pBusSlaveRef))
    {
        pContext->bBehindBusSlaveRef = EC_TRUE;

        /* add processing unit delay of reference bus slave slave to propagation delay */
        pContext->dwPropagDelay = pContext->pBusSlaveCur->m_dwProcessingUnitDelay;
    }
    /* store last DC slave information (should always have coherent port receive times) */
    if (bRecvTimesCoherent)
    {
        pContext->pBusSlaveLast = pContext->pBusSlaveCur;
        pContext->wPortOutLast  = wPortOut;
    }

Exit:
    /* move to next bus slave */
    pContext->pBusSlaveCur->SetDCSmProc();
    pContext->pBusSlaveCur = pBusSlaveNext;
    pContext->wPortInCur   = wPortInNext;
    return;
}

#if (defined INCLUDE_LOG_MESSAGES)
EC_T_VOID CEcDistributedClocks::PrintPortDelays(CEcBusSlave* pBusSlaveRoot)
{
    CEcBusSlave* pBusSlave = EC_NULL;
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug >> 4) & 3);

    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "PortState (Slave connected / Link detected / Loop closed / Communication Established)\n"));
    for (pBusSlave = pBusSlaveRoot; EC_NULL != pBusSlave; pBusSlave = pBusSlave->GetNext())
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Slave=%4d - PA=%d%d%d%d PD=%d%d%d%d PB=%d%d%d%d PC=%d%d%d%d\n", pBusSlave->GetFixedAddress(),
            pBusSlave->IsSlaveConPortX(ESC_PORT_A), pBusSlave->IsLinkPortX(ESC_PORT_A), pBusSlave->IsLoopPortX(ESC_PORT_A), pBusSlave->IsSignalPortX(ESC_PORT_A),
            pBusSlave->IsSlaveConPortX(ESC_PORT_D), pBusSlave->IsLinkPortX(ESC_PORT_D), pBusSlave->IsLoopPortX(ESC_PORT_D), pBusSlave->IsSignalPortX(ESC_PORT_D),
            pBusSlave->IsSlaveConPortX(ESC_PORT_B), pBusSlave->IsLinkPortX(ESC_PORT_B), pBusSlave->IsLoopPortX(ESC_PORT_B), pBusSlave->IsSignalPortX(ESC_PORT_B),
            pBusSlave->IsSlaveConPortX(ESC_PORT_C), pBusSlave->IsLinkPortX(ESC_PORT_C), pBusSlave->IsLoopPortX(ESC_PORT_C), pBusSlave->IsSignalPortX(ESC_PORT_C)));
    }
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "PortRecvTimes\n"));
    for (pBusSlave = pBusSlaveRoot; EC_NULL != pBusSlave; pBusSlave = pBusSlave->GetNext())
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Slave=%4d - ProcUnit=0x%08X%08X PA=%10u PD=%10u PB=%10u PC=%10u\n", pBusSlave->GetFixedAddress(), 
            EC_HIDWORD(pBusSlave->GetRecvTimeProcessingUnit()), EC_LODWORD(pBusSlave->GetRecvTimeProcessingUnit()),
            pBusSlave->GetRecvTimeX(ESC_PORT_A), pBusSlave->GetRecvTimeX(ESC_PORT_D), pBusSlave->GetRecvTimeX(ESC_PORT_B), pBusSlave->GetRecvTimeX(ESC_PORT_C)));
    }
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "SlaveDelay / PortDelay\n"));
    for (pBusSlave = pBusSlaveRoot; EC_NULL != pBusSlave; pBusSlave = pBusSlave->GetNext())
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Slave=%4d - SDelay=%6u PA=%6u PD=%6u PB=%6u PC=%6u\n", pBusSlave->GetFixedAddress(), pBusSlave->m_dwSlaveDelay,
            pBusSlave->m_adwPortDelay[ESC_PORT_A], pBusSlave->m_adwPortDelay[ESC_PORT_D], pBusSlave->m_adwPortDelay[ESC_PORT_B], pBusSlave->m_adwPortDelay[ESC_PORT_C]));
    }
    return;
}

EC_T_VOID CEcDistributedClocks::PrintPropagDelays(CEcBusSlave* pBusSlaveRoot)
{
    CEcBusSlave* pBusSlave = EC_NULL;
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug >> 4) & 3);

    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "PropagDelay / LineDelay\n"));
    for (pBusSlave = pBusSlaveRoot; EC_NULL != pBusSlave; pBusSlave = pBusSlave->GetNext())
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Slave=%4d - PDelay=%6u PA=%6u PD=%6u PB=%6u PC=%6u\n", pBusSlave->GetFixedAddress(), pBusSlave->m_dwPropagDelay,
            pBusSlave->m_adwLineDelay[ESC_PORT_A], pBusSlave->m_adwLineDelay[ESC_PORT_D], pBusSlave->m_adwLineDelay[ESC_PORT_B], pBusSlave->m_adwLineDelay[ESC_PORT_C]));
    }
    return;
}
#endif

/***************************************************************************************************/
/**
\brief  Calculate system time offset

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_VOID CEcDistributedClocks::CalculateSystemTimeOffset(CEcBusSlave* pBusSlave)
{
    EC_T_UINT64 qwSystemTimeOffset = 0;

    /* determine reference values */
    EC_T_UINT64 qwRefRecvTimeProcessingUnit = m_pBusSlaveSystemTimeOffsetRef->GetRecvTimeProcessingUnit();
    EC_T_UINT64 qwRefSystemTimeOffset       = m_pBusSlaveSystemTimeOffsetRef->GetSystemTimeOffset();

    if (ecdcsm_dcinit == m_eReqState)
    {
        /* ------------------------------------------------------------------------ */
        /* ET1100 datasheet: "The system time offset for the reference clock is the */
        /* host time minus receive time of the processing unit".                    */
        /* ------------------------------------------------------------------------ */
        /* After we clear the system time values in ecdcsm_clearsystimevalues, the  */
        /* reference bus time read in ecdcsm_readreferencetime is the local time of */
        /* the reference clock, similar to the receive time of the processing unit. */
        /* ------------------------------------------------------------------------ */
        qwSystemTimeOffset = m_qwReferenceTimeHost;
        if ((0 != m_dwDcStartTimeGrid) && (eDcmMode_Off != m_eDcmMode))
        {
            /* align system time to minimize DCM error on startup if grid is configured */
            qwSystemTimeOffset += m_dwDcStartTimeGrid - (qwSystemTimeOffset % m_dwDcStartTimeGrid);
            qwSystemTimeOffset -= m_poDcm->GetCtlSetVal();
        }
        qwSystemTimeOffset -= m_qwReferenceTimeBus;
        qwSystemTimeOffset += qwRefRecvTimeProcessingUnit - pBusSlave->GetRecvTimeProcessingUnit();
    }
    else
    {
        /* ------------------------------------------------------------------------ */
        /* During the first initialization (ecdcsm_dcinit), the system times on the */
        /* network match the host time.                                             */
        /* Later, the host time cannot be used anymore to initialize new DC slaves, */
        /* because host time and bus time could drift.                              */
        /* To have coherent system times on the network, use the first initialized  */
        /* 64bit DC slave to calculate the system time offset of the new DC slaves. */
        /* If no initialized 64bit slave is found, use the reference clock and      */
        /* compensate the original offset with current (emulated) bus time          */
        /* ------------------------------------------------------------------------ */
        CEcBusSlave* pBusSlaveBusTimeRef = m_poDcm->GetBusSlaveBusTimeRef();
        if (!pBusSlaveBusTimeRef->IsDC64Support())
        {
            qwRefRecvTimeProcessingUnit = EC_MAKEQWORD(EC_HIDWORD(m_qwBusTime - qwRefSystemTimeOffset), EC_LODWORD(qwRefRecvTimeProcessingUnit));
        }
	    qwSystemTimeOffset  = qwRefSystemTimeOffset;
	    qwSystemTimeOffset += qwRefRecvTimeProcessingUnit - pBusSlave->GetRecvTimeProcessingUnit();
    }
    /* set system time offset */
    pBusSlave->SetSystemTimeOffset(qwSystemTimeOffset);
}


/***************************************************************************************************/
/**
*/
EC_T_BOOL CEcDistributedClocks::AbortState( EC_T_VOID )
{
    /* detect state machine error */
    if (EC_E_BUSY != m_dwDCResult)
    {
        m_eCurState  = m_eReqState;
        return EC_TRUE;
    }
    /* handle cancel request */
    if (m_pCurRequest->bCancelRequest)
    {
        m_dwDCResult = EC_E_CANCEL;
        m_eCurState  = m_eReqState;
        return EC_TRUE;
    }
    return EC_FALSE;
}

/***************************************************************************************************/
/**
*/
EC_T_BOOL CEcDistributedClocks::NextState( EC_T_VOID )
{
    m_dwCurStateIdx++;
    m_eCurState = m_aCurStateList[m_dwCurStateIdx];

    return EC_TRUE;
}
EC_T_BOOL CEcDistributedClocks::NextState( ECDCSM_T_DCSTATES eState )
{
    /* look for next state */
    for (m_dwCurStateIdx = 0; (m_aCurStateList[m_dwCurStateIdx] != eState); m_dwCurStateIdx++)
    {
        /* nothing to do */
    }
    m_eCurState = m_aCurStateList[m_dwCurStateIdx];

    return EC_TRUE;
}

/***************************************************************************************************/
/**
*/
EC_T_BOOL CEcDistributedClocks::RetryState( EC_T_VOID )
{
    if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
    {
        m_eCurState = m_aCurStateList[m_dwCurStateIdx];

        return EC_TRUE;
    }
    return EC_FALSE;
}

/***************************************************************************************************/
/**
\brief  Generate State Text.

\return State Text.
*/
EC_T_CHAR* CEcDistributedClocks::ESTATESText(
    ECDCSM_T_DCSTATES   eState              /**< [in]   State to get text to */
                                            )
{
    EC_T_CHAR* szRet = EC_NULL;
    
    switch( eState )
    {
    case ecdcsm_idle:                       szRet = (EC_T_CHAR*)"ecdcsm_idle"; break;
    case ecdcsm_start:                      szRet = (EC_T_CHAR*)"ecdcsm_start"; break;
    case ecdcsm_clearsystimevalues:         szRet = (EC_T_CHAR*)"ecdcsm_clearsystimevalues"; break;
    case ecdcsm_clearsystimevalues_wait:    szRet = (EC_T_CHAR*)"ecdcsm_clearsystimevalues_wait"; break;
    case ecdcsm_clearsystimevalues_done:    szRet = (EC_T_CHAR*)"ecdcsm_clearsystimevalues_done"; break;
#if (defined INCLUDE_RED_DEVICE)
    case ecdcsm_reddisableecatevent:        szRet = (EC_T_CHAR*)"ecdcsm_reddisableecatevent"; break;
    case ecdcsm_reddisableecatevent_wait:   szRet = (EC_T_CHAR*)"ecdcsm_reddisableecatevent_wait"; break;
    case ecdcsm_reddisableecatevent_done:   szRet = (EC_T_CHAR*)"ecdcsm_reddisableecatevent_done"; break;
    case ecdcsm_redcloseport:               szRet = (EC_T_CHAR*)"ecdcsm_redcloseport"; break;
    case ecdcsm_redcloseport_wait:          szRet = (EC_T_CHAR*)"ecdcsm_redcloseport_wait"; break;
    case ecdcsm_redcloseport_done:          szRet = (EC_T_CHAR*)"ecdcsm_redcloseport_done"; break;
    case ecdcsm_redrefreshdlstatus:         szRet = (EC_T_CHAR*)"ecdcsm_redrefreshdlstatus"; break;
    case ecdcsm_redrefreshdlstatus_wait:    szRet = (EC_T_CHAR*)"ecdcsm_redrefreshdlstatus_wait"; break;
    case ecdcsm_redrefreshdlstatus_done:    szRet = (EC_T_CHAR*)"ecdcsm_redrefreshdlstatus_done"; break;
#endif /* INCLUDE_RED_DEVICE */
    case ecdcsm_latchrecvtimes:             szRet = (EC_T_CHAR*)"ecdcsm_latchrecvtimes"; break;
    case ecdcsm_latchrecvtimes_wait:        szRet = (EC_T_CHAR*)"ecdcsm_latchrecvtimes_wait"; break;
    case ecdcsm_latchrecvtimes_done:        szRet = (EC_T_CHAR*)"ecdcsm_latchrecvtimes_done"; break;
    case ecdcsm_readrecvtimes:              szRet = (EC_T_CHAR*)"ecdcsm_readrecvtimes"; break;
    case ecdcsm_readrecvtimes_send:         szRet = (EC_T_CHAR*)"ecdcsm_readrecvtimes_send"; break;
    case ecdcsm_readrecvtimes_wait:         szRet = (EC_T_CHAR*)"ecdcsm_readrecvtimes_wait"; break;
    case ecdcsm_readrecvtimes_done:         szRet = (EC_T_CHAR*)"ecdcsm_readrecvtimes_done"; break;
#if (defined INCLUDE_RED_DEVICE)
    case ecdcsm_redopenport:                szRet = (EC_T_CHAR*)"ecdcsm_redopenport"; break;
    case ecdcsm_redopenport_wait:           szRet = (EC_T_CHAR*)"ecdcsm_redopenport_wait"; break;
    case ecdcsm_redopenport_done:           szRet = (EC_T_CHAR*)"ecdcsm_redopenport_done"; break;
    case ecdcsm_redenableecatevent:         szRet = (EC_T_CHAR*)"ecdcsm_redenableecatevent"; break;
    case ecdcsm_redenableecatevent_wait:    szRet = (EC_T_CHAR*)"ecdcsm_redenableecatevent_wait"; break;
    case ecdcsm_redenableecatevent_done:    szRet = (EC_T_CHAR*)"ecdcsm_redenableecatevent_done"; break;
#endif /* INCLUDE_RED_DEVICE */
    case ecdcsm_calculateportdelays:        szRet = (EC_T_CHAR*)"ecdcsm_calculateportdelays"; break;
    case ecdcsm_calculateportdelays_wait:   szRet = (EC_T_CHAR*)"ecdcsm_calculateportdelays_wait"; break;
    case ecdcsm_calculateportdelays_done:   szRet = (EC_T_CHAR*)"ecdcsm_calculateportdelays_done"; break;
    case ecdcsm_calculatepropagdelays:      szRet = (EC_T_CHAR*)"ecdcsm_calculatepropagdelays"; break;
    case ecdcsm_calculatepropagdelays_wait: szRet = (EC_T_CHAR*)"ecdcsm_calculatepropagdelays_wait"; break;
    case ecdcsm_calculatepropagdelays_done: szRet = (EC_T_CHAR*)"ecdcsm_calculatepropagdelays_done"; break;
    case ecdcsm_readreferencetime:          szRet = (EC_T_CHAR*)"ecdcsm_readreferencetime"; break;
    case ecdcsm_readreferencetime_wait:     szRet = (EC_T_CHAR*)"ecdcsm_readreferencetime_wait"; break;
    case ecdcsm_readreferencetime_done:     szRet = (EC_T_CHAR*)"ecdcsm_readreferencetime_done"; break;
    case ecdcsm_writesystimevalues:         szRet = (EC_T_CHAR*)"ecdcsm_writesystimevalues"; break;
    case ecdcsm_writesystimevalues_wait:    szRet = (EC_T_CHAR*)"ecdcsm_writesystimevalues_wait"; break;
    case ecdcsm_writesystimevalues_done:    szRet = (EC_T_CHAR*)"ecdcsm_writesystimevalues_done"; break;
    case ecdcsm_writepropagdelays:          szRet = (EC_T_CHAR*)"ecdcsm_writepropagdelays"; break;
    case ecdcsm_writepropagdelays_wait:     szRet = (EC_T_CHAR*)"ecdcsm_writepropagdelays_wait"; break;
    case ecdcsm_writepropagdelays_done:     szRet = (EC_T_CHAR*)"ecdcsm_writepropagdelays_done"; break;
#if (defined DEBUG_DC_SYSTIMEVALUES)
    case ecdcsm_debugsystimevalues:         szRet = (EC_T_CHAR*)"ecdcsm_debugsystimevalues"; break;
    case ecdcsm_debugsystimevalues_send:    szRet = (EC_T_CHAR*)"ecdcsm_debugsystimevalues_send"; break;
    case ecdcsm_debugsystimevalues_wait:    szRet = (EC_T_CHAR*)"ecdcsm_debugsystimevalues_wait"; break;
    case ecdcsm_debugsystimevalues_done:    szRet = (EC_T_CHAR*)"ecdcsm_debugsystimevalues_done"; break;
#endif /* DEBUG_DC_SYSTIMEVALUES */
    case ecdcsm_configdccontrollers1:       szRet = (EC_T_CHAR*)"ecdcsm_configdccontrollers1"; break;
    case ecdcsm_configdccontrollers1_wait:  szRet = (EC_T_CHAR*)"ecdcsm_configdccontrollers1_wait"; break;
    case ecdcsm_configdccontrollers1_send:  szRet = (EC_T_CHAR*)"ecdcsm_configdccontrollers1_send"; break;
    case ecdcsm_configdccontrollers1_done:  szRet = (EC_T_CHAR*)"ecdcsm_configdccontrollers1_done"; break;
    case ecdcsm_configdccontrollers2:       szRet = (EC_T_CHAR*)"ecdcsm_configdccontrollers2"; break;
    case ecdcsm_configdccontrollers2_send:  szRet = (EC_T_CHAR*)"ecdcsm_configdccontrollers2_send"; break;
    case ecdcsm_configdccontrollers2_wait:  szRet = (EC_T_CHAR*)"ecdcsm_configdccontrollers2_wait"; break;
    case ecdcsm_configdccontrollers2_done:  szRet = (EC_T_CHAR*)"ecdcsm_configdccontrollers2_done"; break;
    case ecdcsm_armwburst:                  szRet = (EC_T_CHAR*)"ecdcsm_armwburst"; break;
    case ecdcsm_armwburst_send:             szRet = (EC_T_CHAR*)"ecdcsm_armwburst_send"; break;
    case ecdcsm_armwburst_wait:             szRet = (EC_T_CHAR*)"ecdcsm_armwburst_wait"; break;
    case ecdcsm_armwburst_done:             szRet = (EC_T_CHAR*)"ecdcsm_armwburst_done"; break;
    case ecdcsm_startbustime64emul:         szRet = (EC_T_CHAR*)"ecdcsm_startbustime64emul"; break;
    case ecdcsm_startbustime64emul_wait:    szRet = (EC_T_CHAR*)"ecdcsm_startbustime64emul_wait"; break;
    case ecdcsm_startbustime64emul_done:    szRet = (EC_T_CHAR*)"ecdcsm_startbustime64emul_done"; break;
    case ecdcsm_dcmreadpulselength:         szRet = (EC_T_CHAR*)"ecdcsm_dcmreadpulselength"; break;
    case ecdcsm_dcmreadpulselength_wait:    szRet = (EC_T_CHAR*)"ecdcsm_dcmreadpulselength_wait"; break;
    case ecdcsm_dcmreadpulselength_done:    szRet = (EC_T_CHAR*)"ecdcsm_dcmreadpulselength_done"; break;
    case ecdcsm_waitforinsync:              szRet = (EC_T_CHAR*)"ecdcsm_waitforinsync"; break;

    case ecdcsm_dcinit:                     szRet = (EC_T_CHAR*)"ecdcsm_dcinit"; break;
    case ecdcsm_dctopologychanged:          szRet = (EC_T_CHAR*)"ecdcsm_dctopologychanged"; break;
    case ecdcsm_dccontinuousmeasuring:      szRet = (EC_T_CHAR*)"ecdcsm_dccontinuousmeasuring"; break;    
    default:                                szRet = (EC_T_CHAR*)"Unknown"; break;
    }
    
    return szRet;  
}

/***************************************************************************************************/
/**
*/
const CEcDistributedClocks::ECDCSM_T_DCSTATES CEcDistributedClocks::C_aStateList_dcinit[] = {
        ecdcsm_start,
        ecdcsm_clearsystimevalues,
#if (defined INCLUDE_RED_DEVICE)
        ecdcsm_reddisableecatevent,
        ecdcsm_redcloseport,
        ecdcsm_redrefreshdlstatus,
#endif
        ecdcsm_latchrecvtimes,
        ecdcsm_readrecvtimes,
        ecdcsm_calculateportdelays,
        ecdcsm_calculatepropagdelays,
#if (defined INCLUDE_RED_DEVICE)
        ecdcsm_redopenport,
        ecdcsm_redenableecatevent,
#endif
        ecdcsm_readreferencetime,
        ecdcsm_writesystimevalues,
        ecdcsm_configdccontrollers1,
        ecdcsm_configdccontrollers2,
#if (defined DEBUG_DC_SYSTIMEVALUES)
        ecdcsm_debugsystimevalues,
#endif /* DEBUG_DC_SYSTIMEVALUES */                
        ecdcsm_startbustime64emul,
        ecdcsm_dcmreadpulselength,
        ecdcsm_armwburst,
        ecdcsm_waitforinsync,
        ecdcsm_dcinit
};
/***************************************************************************************************/
/**
*/
const CEcDistributedClocks::ECDCSM_T_DCSTATES CEcDistributedClocks::C_aStateList_topologychanged[] = {
        ecdcsm_start,
        ecdcsm_clearsystimevalues,
#if (defined INCLUDE_RED_DEVICE)
        ecdcsm_reddisableecatevent,
        ecdcsm_redcloseport,
        ecdcsm_redrefreshdlstatus,
#endif
        ecdcsm_latchrecvtimes,
        ecdcsm_readrecvtimes,
        ecdcsm_calculateportdelays,
        ecdcsm_calculatepropagdelays,
#if (defined INCLUDE_RED_DEVICE)
        ecdcsm_redopenport,
        ecdcsm_redenableecatevent,
#endif
        ecdcsm_writepropagdelays,
        ecdcsm_readreferencetime,
        ecdcsm_writesystimevalues,
        ecdcsm_configdccontrollers1,
        ecdcsm_configdccontrollers2,
#if (defined DEBUG_DC_SYSTIMEVALUES)
        ecdcsm_debugsystimevalues,
#endif /* DEBUG_DC_SYSTIMEVALUES */
        ecdcsm_armwburst,
        ecdcsm_waitforinsync,
        ecdcsm_dctopologychanged
};
/***************************************************************************************************/
/**
*/
const CEcDistributedClocks::ECDCSM_T_DCSTATES CEcDistributedClocks::C_aStateList_dccontinuousmeasuring[] = {
        ecdcsm_start,
#if (defined INCLUDE_RED_DEVICE)
        ecdcsm_reddisableecatevent,
        ecdcsm_redcloseport,
        ecdcsm_redrefreshdlstatus,
#endif
        ecdcsm_latchrecvtimes,
        ecdcsm_readrecvtimes,
        ecdcsm_calculateportdelays,
        ecdcsm_calculatepropagdelays,
#if (defined INCLUDE_RED_DEVICE)
        ecdcsm_redopenport,
        ecdcsm_redenableecatevent,
#endif
        ecdcsm_writepropagdelays,
#if (defined DEBUG_DC_SYSTIMEVALUES)
        ecdcsm_debugsystimevalues,
#endif /* DEBUG_DC_SYSTIMEVALUES */
        ecdcsm_dccontinuousmeasuring
};

#endif /* INCLUDE_DC_SUPPORT */

/*-END OF SOURCE FILE--------------------------------------------------------*/
