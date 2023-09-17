/*-----------------------------------------------------------------------------
 * EcDcBusShift.cpp         cpp file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Holger Oelhaf
 * Description              Master synchronization (DCM)
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "EcCommon.h"
#include "EcCommonPrivate.h"

#if (defined INCLUDE_DC_SUPPORT)

#include <EcMaster.h>
#include <EcSlave.h>
#if (defined INCLUDE_MASTER_OBD)
#include <EcSdoServ.h>
#endif
#include <EcDistributedClocks.h>

/*-COMPILER SETTINGS---------------------------------------------------------*/
/*-DEFINES-------------------------------------------------------------------*/
#define ETHTYPE1        ((EC_T_BYTE)0xFF)
#define ETHTYPE0        ((EC_T_BYTE)0x10)
#define STATE_CHNG      ((EC_T_BYTE)0x00)
#define DCAPROVAL       ((EC_T_BYTE)0x01)
#define DCM_IN_SYNC_START_DELAY_CYCLES  1500
#define DCM_IN_SYNC_SETTLE_TIME         1500
#define DCM_IN_SYNC_LIMIT              25000

/*-MACROS--------------------------------------------------------------------*/
#undef  EcLinkErrorMsg
#define EcLinkErrorMsg m_poMaster->m_poEcDevice->LinkErrorMsg
#undef  EcLinkDbgMsg
#define EcLinkDbgMsg   m_poMaster->m_poEcDevice->LinkDbgMsg

#define SET_DCM_CTL_STATUS(EStatus)      \
    m_ECtlStatus = EStatus;              \
    m_adwStatusCnt[EStatus]++;           

#if (defined INCLUDE_DCX)
#define DCX_EXTSYNCSTATUS_PDO_IDX                       0x10F4
#define DCX_EXTSYNCSTATUS_PDO_SUBIDX_TIME_STAMP_UPD     15      /* Sub index SYNC Inputs.Timestamp update toggle */
#define DCX_EXTSYNCSTATUS_PDO_SUBIDX_NO_EXT_DEV         16      /* Sub index SYNC Inputs.External device not connected */
#define DCX_EXTSYNCSTATUS_PDO_SUBIDX_INT_TIME_STAMP     17      /* Sub index SYNC SYNC Inputs.Internal time stamp */  
#define DCX_EXTSYNCSTATUS_PDO_SUBIDX_EXT_TIME_STAMP     18      /* Sub index SYNC SYNC Inputs.Internal time stamp*/  
#define DCX_EXTSYNCSTATUS_PDO_PROC_VAR_CNT              4
#define DCX_TIME_STAMP_UPDATE_WATCHDOG                  3
#define DCX_EL6692_PRODUCT_CODE                         0x1A243052
#endif

/*-TYPEDEFS------------------------------------------------------------------*/

/*-CLASS---------------------------------------------------------------------*/

/*-FUNCTION DECLARATION------------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-GLOBAL VARIABLES----------------------------------------------------------*/

/*-FUNCTIONS-----------------------------------------------------------------*/

/*-CEcDcmInSync--------------------------------------------------------------*/
CEcDcmInSync::CEcDcmInSync(CEcMaster* poMaster)
{
    m_poMaster = poMaster;

    m_bInSync = EC_FALSE;
    m_dwInSyncSettleTime = DCM_IN_SYNC_SETTLE_TIME;
    m_dwInSyncLimit = DCM_IN_SYNC_LIMIT;
    m_dwCycleCnt = 0;
    m_dwMonitoringStartDelayCycles = 0;

    m_nCtlErrorNsecAvg = 0;
    m_nCtlErrorNsecMax = 0;
}

EC_T_VOID CEcDcmInSync::Reset(EC_T_VOID)
{ 
    m_bInSync = EC_FALSE; 
    m_dwCycleCnt = 0;
    m_oInSyncSettleTime.Stop(); 
    this->ResetCtlErrorStatus();
}

/***************************************************************************************************/
/**
\brief  Acquires a new error value and monitors settle time

\return EC_E_NOERROR
*/
EC_T_DWORD CEcDcmInSync::NotifyCtlError(EC_T_INT nCtlErrorNsec)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_BOOL  bNotify  = EC_FALSE;
    EC_T_BOOL  bInSync  = EC_FALSE;

    /* Start InSync monitoring after x bus cycles */
    if (m_dwCycleCnt++ < m_dwMonitoringStartDelayCycles)
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    /* calculate average and maximum controller error */
    m_nCtlErrorNsecAvg = (m_nCtlErrorNsecAvg * 4095 + nCtlErrorNsec) / 4096;
    if (abs(m_nCtlErrorNsecMax) < abs(nCtlErrorNsec))
    {
        m_nCtlErrorNsecMax = nCtlErrorNsec;
    }

    /* handle settling */
    if (0 != m_dwInSyncLimit)
    {
        if ((EC_T_INT)m_dwInSyncLimit < abs(nCtlErrorNsec))
        {
            /* stop settle time */
            m_oInSyncSettleTime.Stop();

            if (IsInSync())
            {
                bInSync  = EC_FALSE;
                bNotify  = EC_TRUE;
                dwRetVal = DCM_E_MAX_CTL_ERROR_EXCEED;
            }
        }
        else
        {
            if (!IsInSync())
            {
                /* after we are below the limit we have to wait the settle time */
                if (!m_oInSyncSettleTime.IsStarted())
                {
                    m_oInSyncSettleTime.Start(m_dwInSyncSettleTime, m_poMaster->GetMsecCounterPtr());
                }
                /* wait until settle time expires */
                if (m_oInSyncSettleTime.IsElapsed())
                {
                    m_oInSyncSettleTime.Stop();
                    bNotify  = EC_TRUE;
                    bInSync  = EC_TRUE;
                    dwRetVal = EC_E_NOERROR;
                }
                else
                {
                    dwRetVal = EC_E_BUSY;
                }
            }
        }
    }
    else
    {
        /* monitoring off, assume "in sync" */
        if (!IsInSync())
        {
            bNotify  = EC_TRUE;
            bInSync  = EC_TRUE;
            dwRetVal = EC_E_NOERROR;
        }
    }
    /* handle notification */
    if (bNotify)
    {
        this->NotifyInSync(bInSync, nCtlErrorNsec, dwRetVal);
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  DCM InSync notification wrapper

\return
*/
EC_T_VOID CEcDcmInSync::NotifyInSync(EC_T_BOOL bInSyncNotified, EC_T_INT nCtlErrorNsec, EC_T_DWORD dwErrorCode)
{
    EC_UNREFPARM(dwErrorCode);
    m_bInSync = bInSyncNotified;
    m_poMaster->NotifyDcmSync(bInSyncNotified, nCtlErrorNsec, m_nCtlErrorNsecAvg, m_nCtlErrorNsecMax);
}

#if (defined INCLUDE_DCX)
/*-CEcDcxInSync--------------------------------------------------------------*/
/***************************************************************************************************/
/**
\brief  DCX InSync notification wrapper

\return
*/
EC_T_VOID CEcDcxInSync::NotifyInSync(EC_T_BOOL bInSyncNotified, EC_T_INT nCtlErrorNsec, EC_T_DWORD dwErrorCode)
{
    EC_T_DCX_SYNC_NTFY_DESC oDcxInSyncNotify;
    oDcxInSyncNotify.IsInSync         = bInSyncNotified;
    oDcxInSyncNotify.nCtlErrorNsecCur = nCtlErrorNsec;
    oDcxInSyncNotify.nCtlErrorNsecAvg = m_nCtlErrorNsecAvg;
    oDcxInSyncNotify.nCtlErrorNsecMax = m_nCtlErrorNsecMax;
    oDcxInSyncNotify.nTimeStampDiff   = m_poDcx->GetTimeStampDiff();
    oDcxInSyncNotify.dwErrorCode      = dwErrorCode;

    m_bInSync = bInSyncNotified;
    m_poMaster->NotifyDcxSync(&oDcxInSyncNotify);
}
#endif /*INCLUDE_DCX*/

/*-CEcDcmController----------------------------------------------------------*/
CEcDcmController::CEcDcmController(CEcMaster* poMaster)
{
    m_poMaster            = poMaster;
    m_dwStatusCode        = EC_E_NOTREADY;
    m_pCtlparam           = EC_NULL;
    {
        EC_T_DWORD dwRes = EC_E_ERROR;
        EC_T_DWORD dwTmp = 0;
        EC_T_INT   nTmp  = 0;

        /* GetInputFrequency() can do some run once initialization */
        dwRes = OsHwTimerGetInputFrequency(&dwTmp);
        if (EC_E_NOERROR == dwRes)
        {
            dwRes = OsHwTimerGetCurrentCount(&nTmp);
            m_bSyncToTimerIrq = (EC_E_NOERROR == dwRes);
        }
        else
        {
            m_bSyncToTimerIrq = EC_FALSE;
        }
        EC_UNREFPARM(dwTmp);
        EC_UNREFPARM(nTmp);
    }
    m_nSetValNsec         = 0;
    m_qwDcStartTimeOffset = 0;
    m_nActValNsec         = 0;
    m_bNewSetVal          = EC_FALSE;
    m_bConfigured         = EC_FALSE;

    m_ECtlStatus = eDcBsCtlNotInitialized;
    OsMemset(&m_adwStatusCnt, 0, sizeof(m_adwStatusCnt));

    m_bResetRequest       = EC_TRUE;
    m_qwBusTimeFirst      = 0;
    m_qwBusTimeLast       = 0;
    m_dwBusCycleCount     = 0;
    m_bCheckBusCycleTime  = EC_TRUE;
}

/***************************************************************************************************/
/**
\brief  DCM controller initialization

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmController::Init(DCBS_CTL_PARAM* pCtlparam)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    m_pCtlparam = pCtlparam;
    m_pCtlparam->nGain = P_GAIN;
    m_pCtlparam->nDriftErrorGain = DRIFT_ERROR_GAIN;
    m_pCtlparam->nMaxValidVal = ERROR_INPUT_VALIDATION_MAX_NSEC;
    
    /* get bus cycle time (nsec) */
    m_pCtlparam->dwBusCycleTimeNsec = m_poMaster->GetBusCycleTimeUsec() * 1000;

    /* no errors */
    dwRetVal = EC_E_NOERROR;

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Configure DC master synchonization in bus shift mode

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmController::Configure(EC_T_DCM_CONFIG_BUSSHIFT* pDcmConfig)
{
    if (EC_NULL == m_pCtlparam)
    {
        return EC_E_INVALIDSTATE;
    }
    if (0 != pDcmConfig->nCtlGain)
    {
        m_pCtlparam->nGain = pDcmConfig->nCtlGain;
    }
    if (0 != pDcmConfig->nCtlDriftErrorGain)
    {
        m_pCtlparam->nDriftErrorGain = pDcmConfig->nCtlDriftErrorGain;
    }
    if (0 != pDcmConfig->nMaxValidVal)
    {
        m_pCtlparam->nMaxValidVal = pDcmConfig->nMaxValidVal;
    }
    if (((EC_T_INT)m_qwDcStartTimeOffset - m_nSetValNsec) != (EC_T_INT)pDcmConfig->nCtlSetVal)
    {
        if (0 == m_qwDcStartTimeOffset)
        {
            m_qwDcStartTimeOffset = (EC_T_UINT64)pDcmConfig->nCtlSetVal;
            m_nSetValNsec = 0;
        }
        else
        {
            m_nSetValNsec = (EC_T_INT)m_qwDcStartTimeOffset - pDcmConfig->nCtlSetVal;
        }
        m_bNewSetVal = EC_TRUE;
    }
    m_bConfigured = EC_TRUE;
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Get controller configuration

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmController::GetConfig(EC_T_DCM_CONFIG_BUSSHIFT* pDcmConfig)
{
    pDcmConfig->nCtlGain           = m_pCtlparam->nGain;
    pDcmConfig->nCtlDriftErrorGain = m_pCtlparam->nDriftErrorGain;
    pDcmConfig->nMaxValidVal       = m_pCtlparam->nMaxValidVal;
    pDcmConfig->nCtlSetVal         = this->GetCtlSetVal();

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Reset the controller

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmController::Reset(EC_T_VOID)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_poMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "DcmCtlReset()\n"));

    m_bResetRequest = EC_TRUE;
    m_dwStatusCode   = EC_E_NOTREADY;

    return EC_E_NOERROR;
}

/********************************************************************************************/
/**
\brief  Controller status string conversion
*
* \return status string
*/
EC_T_CHAR* CEcDcmController::CtlStatusString(
    E_DCBS_CTL_STATUS ECtlStatus
    )
{
    switch (ECtlStatus)
    {
    case eDcBsCtlNotInitialized:            return (EC_T_CHAR*)"controller not yet initialized";
        //  case eDcBsCtlSyncNotRunning:            return (EC_T_CHAR*)"synchronization not yet running";
        //  case eDcBsCtlMasterStateLowerThanPreop: return (EC_T_CHAR*)"Master state below PREOP (INIT)";
        //  case eDcBsCtlNoNewTimestampValue:       return (EC_T_CHAR*)"Controller was called, but no new timestamp value is available";
        //  case eDcBsCtlTimestampBusy:             return (EC_T_CHAR*)"Controller was called while timestamping was busy";
        //  case eDcBsCtlSlavesNotInSync:           return (EC_T_CHAR*)"Slaves are not in SYNC";
    case eDcBsCtlBusTimeIsZero:             return (EC_T_CHAR*)"EtherCAT Bus Time is 0";
        //  case eDcBsCtlGridOffsetIsZero:          return (EC_T_CHAR*)"Grid Offset is 0";
        //  case eDcBsCtlBusTimeBelowGridOffset:    return (EC_T_CHAR*)"EtherCAT Bus Time is still below Grid Offset";
    case eDcBsCtlBusTimeCountsBackwards:    return (EC_T_CHAR*)"EtherCAT Bus Time counts backwards (current time is below last time)";
    case eDcBsCtlBusTimeDiffTooLow:         return (EC_T_CHAR*)"Change of EtherCAT Bus Time less than 50% of DCM controller cycle";
    case eDcBsCtlBusTimeDiffTooHigh:        return (EC_T_CHAR*)"Change of EtherCAT Bus Time higher than 150% of DCM controller cycle";
        //  case eDcBsCtlSendTimeStampDiffTooHigh:  return (EC_T_CHAR*)"Timestamp difference of frame send time is too high (>50% of average)";
        //  case eDcBsCtlSyncPeriodLengthIsZero:    return (EC_T_CHAR*)"DC Sync Period length is 0";
    case eDcBsCtlControllerFailure:         return (EC_T_CHAR*)"DCM PI Controller failure";
    case eDcBsCtlControllerOk:              return (EC_T_CHAR*)"DCM PI Controller successful call";
        //  case eDcBsCtlModifyFailure:             return (EC_T_CHAR*)"DCM timer/bus modification failure";
        //  case eDcBsCtlModifyOk:                  return (EC_T_CHAR*)"DCM timer/bus modification successful call";
    case eDcBsCtlMaxCtlErrorExceeded:       return (EC_T_CHAR*)"DCM maximum controller error exceeded";
    case eDcBsCtlOk:                        return (EC_T_CHAR*)"DCM job call success";
        //  case eDcBsCtlControllerNotEnabled:      return (EC_T_CHAR*)"DCM controller not enabled";
    case eDcBsCtlResetRequest:              return (EC_T_CHAR*)"DCM controller reset request";
    case eDcBsCtlMissingCtlErrorLimit:      return (EC_T_CHAR*)"DCM controller error limit is missing";
    case eDcBsCtlDriftTooHigh:              return (EC_T_CHAR*)"DCM drift between host timer and reference clock is too high";
    case eDcBsCtlBusCycleTimeIsZero:        return (EC_T_CHAR*)"Master config parameter dwBusCycleTimeUsec is 0";
    default:                                return (EC_T_CHAR*)"XXX - unknown DCM Controller Status";
    }
}

/********************************************************************************************/
/**
\brief  Controller status show routine
*
* \return N/A
*/
EC_T_DWORD CEcDcmController::StatusShow(
    EC_T_INT bnResetCounters
    )
{
    EC_DBGMSG("**********************************************************************\n");
    EC_DBGMSG("DCM Bus cycle time: %d nsec\n", m_pCtlparam->dwBusCycleTimeNsec);
    EC_DBGMSG("DCM Sync signal delay set value: %d nsec\n", m_nSetValNsec);
    EC_DBGMSG("DCM Last error: 0x%x\n", m_dwStatusCode);
    EC_DBGMSG("DCM controller error cur: %d nsec\n", m_pCtlparam->nCtlErrorNsec);
    EC_DBGMSG("**********************************************************************\n");
    EC_DBGMSG("Current DCM Controller status: %s\n\n", CtlStatusString(m_ECtlStatus));
    EC_DBGMSG("Status Counters\n");
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlNotInitialized], CtlStatusString(eDcBsCtlNotInitialized));
    //  EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlSyncNotRunning],            CtlStatusString(eDcBsCtlSyncNotRunning) );
    //  EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlMasterStateLowerThanPreop], CtlStatusString(eDcBsCtlMasterStateLowerThanPreop) );
    //  EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlNoNewTimestampValue],       CtlStatusString(eDcBsCtlNoNewTimestampValue) );
    //  EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlTimestampBusy],             CtlStatusString(eDcBsCtlTimestampBusy) );
    //  EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlSlavesNotInSync],           CtlStatusString(eDcBsCtlSlavesNotInSync) );
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlBusTimeIsZero], CtlStatusString(eDcBsCtlBusTimeIsZero));
    //  EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlGridOffsetIsZero],          CtlStatusString(eDcBsCtlGridOffsetIsZero) );
    //  EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlBusTimeBelowGridOffset],    CtlStatusString(eDcBsCtlBusTimeBelowGridOffset) );
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlBusTimeCountsBackwards], CtlStatusString(eDcBsCtlBusTimeCountsBackwards));
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlBusTimeDiffTooLow], CtlStatusString(eDcBsCtlBusTimeDiffTooLow));
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlBusTimeDiffTooHigh], CtlStatusString(eDcBsCtlBusTimeDiffTooHigh));
    //  EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlSendTimeStampDiffTooHigh],  CtlStatusString(eDcBsCtlSendTimeStampDiffTooHigh) );    EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlSyncPeriodLengthIsZero],    CtlStatusString(eDcBsCtlSyncPeriodLengthIsZero) );
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlControllerFailure], CtlStatusString(eDcBsCtlControllerFailure));
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlControllerOk], CtlStatusString(eDcBsCtlControllerOk));
    //  EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlModifyFailure],             CtlStatusString(eDcBsCtlModifyFailure) );
    //  EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlModifyOk],                  CtlStatusString(eDcBsCtlModifyOk) );
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlMaxCtlErrorExceeded], CtlStatusString(eDcBsCtlMaxCtlErrorExceeded));
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlOk], CtlStatusString(eDcBsCtlOk));
    //  EC_DBGMSG( "%d: %s\n", m_adwStatusCnt[eDcBsCtlControllerNotEnabled],      CtlStatusString(eDcBsCtlControllerNotEnabled) );
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlResetRequest], CtlStatusString(eDcBsCtlResetRequest));
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlMissingCtlErrorLimit], CtlStatusString(eDcBsCtlMissingCtlErrorLimit));
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlDriftTooHigh], CtlStatusString(eDcBsCtlDriftTooHigh));
    EC_DBGMSG("%d: %s\n", m_adwStatusCnt[eDcBsCtlBusCycleTimeIsZero], CtlStatusString(eDcBsCtlBusCycleTimeIsZero));
    if (bnResetCounters)
    {
        OsMemset(m_adwStatusCnt, 0, NUM_CTL_STATUS*sizeof(EC_T_DWORD));
    }
    return EC_E_NOERROR;
}
/********************************************************************************************/
/**
\brief  DCM Controller.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmController::Controller()
{
    EC_T_DWORD      dwRetVal = EC_E_NOERROR;
    EC_T_INT        nCtlErrorNsec;                /* effective controller error in timer increments */
    EC_T_INT        qnCtlOutputPpmP;                    /* controller proportional output in ppm */
    EC_T_BOOL       bStateMachActive = EC_TRUE;
    EC_T_INT        nCurrCtlErrorNsec = 0;
    EC_T_INT        nDrift = 0;
    EC_T_INT        nCycleTime100us = EC_MAX(1, m_pCtlparam->dwBusCycleTimeNsec / 100000);

    if (m_pCtlparam->nJob == CONTROLLER_JOB_RESET)
    {
        m_pCtlparam->ECtlState = eCtlStateReset;
    }
    else if (m_pCtlparam->nJob == CONTROLLER_JOB_CTL)
    {
        if (m_pCtlparam->ECtlState == eCtlStateReset)
        {
            /* leave reset state and enter filter state */
            m_pCtlparam->ECtlState = eCtlStateNewOutput;
        }
    }
    else if (m_pCtlparam->nJob == CONTROLLER_JOB_NEW_SETVAL)
    {
        m_pCtlparam->ECtlState = eCtlStateReset;
    }

    /* reset controller output */
    m_pCtlparam->nCtlOutputTotal = 0;

    while (bStateMachActive)
    {
        bStateMachActive = EC_FALSE;    /* by default: leave SM */

        switch (m_pCtlparam->ECtlState)
        {
            /*****************************************************************/
            /*****************************************************************/
        case eCtlStateReset:
            /* controller deadzone: no adjustment */
            m_pCtlparam->nCtlOutputTotal = 0;
            /* reset drift filter */
            m_pCtlparam->nCtlErrorFilt = 0;
            m_pCtlparam->bInitFilter = EC_TRUE;
            /* initialize controller output information */
            OsMemset(m_pCtlparam->naCtlErrFlt, 0, (DRIFT_CALC_STEP_WIDTH + 1)*sizeof(EC_T_INT));
            m_pCtlparam->dwDriftCounter = 0;
            m_pCtlparam->nCurrDrift = 0;
            m_pCtlparam->nCtlErrorIntegralNsec = 0;
            break;


            /*****************************************************************/
            /*****************************************************************/
        case eCtlStateNewOutput:

            nCurrCtlErrorNsec = m_pCtlparam->nCtlErrorNsec;

            /* skip x values if they are above the limit */
            if ((labs(m_pCtlparam->nCtlErrorFilt - nCurrCtlErrorNsec) > m_pCtlparam->nMaxValidVal) && (m_pCtlparam->dwSkipCounter < 2))
            {
                m_pCtlparam->dwSkipCounter++;
            }
            else
            {
                m_pCtlparam->dwSkipCounter = 0;

                /* exponential smoothing with alpha = 1/8 */
                if (m_pCtlparam->bInitFilter)
                {
                    m_pCtlparam->bInitFilter = EC_FALSE;
                    m_pCtlparam->nCtlErrorFilt = nCurrCtlErrorNsec;
                }
                else
                {
                    m_pCtlparam->nCtlErrorFilt = (m_pCtlparam->nCtlErrorFilt * 7 + nCurrCtlErrorNsec) / 8;
                }
            }

            /* calculate drift */
            /* --------------- */
            if (m_pCtlparam->bCalcDrift)//m_eMode == eDcmMode_BusShift)
            {
                m_pCtlparam->naCtlErrFlt[m_pCtlparam->nCtlErrFltIdx] = m_pCtlparam->nCtlErrorFilt;
                {
                    EC_T_INT    nPrevValue = m_pCtlparam->naCtlErrFlt[(m_pCtlparam->nCtlErrFltIdx + 1) % (DRIFT_CALC_STEP_WIDTH + 1)];

                    if (nPrevValue != 0)
                    {
                        nDrift = m_pCtlparam->naCtlErrFlt[m_pCtlparam->nCtlErrFltIdx] - nPrevValue;

                        /* drift based on msec */
                        nDrift = (nDrift * 1000) / (EC_T_INT)(m_pCtlparam->dwBusCycleTimeNsec / 1000);

                        if (m_pCtlparam->dwDriftCounter == 0)
                        {
                            m_pCtlparam->nCurrDrift = nDrift;
                        }
                        else
                        {
                            /* exponential smoothing with alpha = 1/8 */
                            m_pCtlparam->nCurrDrift = ((m_pCtlparam->nCurrDrift * 7) + nDrift) / 8;
                        }
                        m_pCtlparam->dwDriftCounter++;
                    }
                }
                m_pCtlparam->nCtlErrFltIdx++;
                if (m_pCtlparam->nCtlErrFltIdx == (DRIFT_CALC_STEP_WIDTH + 1)) m_pCtlparam->nCtlErrFltIdx = 0;

                /* save system drift 100 cycles after enabling the controller */
                if (m_pCtlparam->dwDriftCounter == 100)
                {
                    m_pCtlparam->nSystemDrift = m_pCtlparam->nCurrDrift / DRIFT_CALC_STEP_WIDTH;
#if (defined DEBUG_DCM_OSDBGMSG)
                    EC_DBGMSG("DCM Inital system drift =%d\n", m_pCtlparam->nSystemDrift);
#endif
                }

                /* check if drift is to high for controller */
                if ((m_pCtlparam->dwDriftCounter > 100) && labs(m_pCtlparam->nSystemDrift) > DCM_MAX_TIMER_DRIFT)
                {
                    dwRetVal = DCM_E_DRIFT_TO_HIGH;
                }

                /* calculate drift based error value */
                m_pCtlparam->nErrCorrDrift = m_pCtlparam->nCurrDrift / DRIFT_CALC_STEP_WIDTH;

                /* limit value */
                if (m_pCtlparam->nErrCorrDrift > 0)
                {
                    m_pCtlparam->nErrCorrDrift = EC_MIN(m_pCtlparam->nErrCorrDrift, DCM_MAX_TIMER_DRIFT);
                }
                else
                {
                    m_pCtlparam->nErrCorrDrift = EC_MAX(m_pCtlparam->nErrCorrDrift, -DCM_MAX_TIMER_DRIFT);
                }

                m_pCtlparam->nErrCorrDrift = (m_pCtlparam->nErrCorrDrift * m_pCtlparam->nErrCorrDrift * m_pCtlparam->nDriftErrorGain * nCycleTime100us) / 10;

                if (m_pCtlparam->nCurrDrift < 0)
                {
                    m_pCtlparam->nErrCorrDrift = -m_pCtlparam->nErrCorrDrift;
                }
            }

            /* build total controller error */
            /*****************************************************************/
            if (m_pCtlparam->bBuildTotalCtlErr)//m_eMode == eDcmMode_MasterShift)
            {
                m_pCtlparam->nGain = 1;
                //nCtlErrorNsec = (m_pCtlparam->nGain * m_pCtlparam->nCtlErrorFilt) + m_pCtlparam->nCtlErrorIntegralNsec;          // todo which gain?
                nCtlErrorNsec = (m_pCtlparam->nCtlErrorFilt / 2) + m_pCtlparam->nCtlErrorIntegralNsec;

                m_pCtlparam->nCtlErrorIntegralNsec = m_pCtlparam->nCtlErrorIntegralNsec + m_pCtlparam->nCtlErrorFilt / 128;
                if (m_pCtlparam->nCtlErrorIntegralNsec > 0)
                {
                    /* maximum is 10% of cycle time */
                    m_pCtlparam->nCtlErrorIntegralNsec = EC_MIN(m_pCtlparam->nCtlErrorIntegralNsec, (EC_T_INT)(m_pCtlparam->dwBusCycleTimeNsec / 10));
                }
                else
                {
                    m_pCtlparam->nCtlErrorIntegralNsec = EC_MAX(m_pCtlparam->nCtlErrorIntegralNsec, -(EC_T_INT)(m_pCtlparam->dwBusCycleTimeNsec / 10));
                }
                m_pCtlparam->nCtlOutputSumComp = m_pCtlparam->nCtlErrorIntegralNsec;       /* just for logging purpose */
            }
            else
            {
                nCtlErrorNsec = (m_pCtlparam->nGain * m_pCtlparam->nCtlErrorFilt) + m_pCtlparam->nErrCorrDrift + m_pCtlparam->nCtlOutputSumComp;
            }

            m_pCtlparam->nCtlErrorPmil = (10 * nCtlErrorNsec) / (EC_T_INT)(m_pCtlparam->dwBusCycleTimeNsec / 1000);

            qnCtlOutputPpmP = (EC_T_CALCVAL)nCtlErrorNsec / (EC_T_INT)(m_pCtlparam->dwBusCycleTimeNsec / 1000);

            if (m_pCtlparam->bIncrementOutput)//m_eMode == eDcmMode_BusShift)
            {
                /* increment outputs */
                m_pCtlparam->nCtlOutputSumComp = (m_pCtlparam->nCtlOutputSumComp * 9) / 10;
                if (qnCtlOutputPpmP > 0)
                {
                    m_pCtlparam->nCtlOutputSumComp = EC_MAX(-3000, m_pCtlparam->nCtlOutputSumComp - 300);
                }
                else if (qnCtlOutputPpmP < 0)
                {
                    m_pCtlparam->nCtlOutputSumComp = EC_MIN(+3000, m_pCtlparam->nCtlOutputSumComp + 300);
                }
                else
                {
                    if (m_pCtlparam->nCtlOutputSumComp > 0)
                    {
                        m_pCtlparam->nCtlOutputSumComp = EC_MAX(0, m_pCtlparam->nCtlOutputSumComp - 300);
                    }
                    else
                    {
                        m_pCtlparam->nCtlOutputSumComp = EC_MIN(0, m_pCtlparam->nCtlOutputSumComp + 300);
                    }
                }
            }
            /* set timer adjustment */
            if (qnCtlOutputPpmP != 0)
            {
                if (qnCtlOutputPpmP > 0)
                {
                    qnCtlOutputPpmP = EC_MIN(qnCtlOutputPpmP, DCM_MAX_CONTROLLER_SET_PMIL);
                }
                else
                {
                    qnCtlOutputPpmP = EC_MAX(qnCtlOutputPpmP, -DCM_MAX_CONTROLLER_SET_PMIL);
                }
                m_pCtlparam->nCtlOutputTotal = qnCtlOutputPpmP;
            }

            break;

        default:
            break;
        }
    } /* while( bStateMachActive ) */

    return dwRetVal;
}

/********************************************************************************************/
/**
\brief  returns timer adjustment when not CtlOff

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmController::GetTimerAdjust(EC_T_INT* pnAdjustPermil)
{
    if (EC_NULL == pnAdjustPermil)
    {
        return EC_E_INVALIDPARM;
    }

    *pnAdjustPermil = m_pCtlparam->nTimerAdjust;

    return EC_E_NOERROR;
}

EC_T_UINT64 CEcDcmController::GetDcStartTime(EC_T_UINT64 qwBusTime)
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_poMaster->m_MasterConfig.dwStateChangeDebug >> 4) & 3);
#endif
    EC_T_UINT64 qwDcStartTime     = 0;
    EC_T_DWORD  dwDcStartTimeGrid = m_poMaster->m_pDistributedClks->GetDcStartTimeGrid();

    /* align the DC start time to master time grid */
    OsDbgAssert(0 != m_qwBusTimeFirst);
    qwDcStartTime = qwBusTime - (qwBusTime % m_pCtlparam->dwBusCycleTimeNsec) + (m_qwBusTimeFirst % m_pCtlparam->dwBusCycleTimeNsec);

    if (!m_bConfigured)
    {
        /* set default start time offset */
        m_qwDcStartTimeOffset = (m_poMaster->GetBusCycleTimeUsec() * 2000) / 3;
    }
    /* add the DC start time offset */
    qwDcStartTime += m_qwDcStartTimeOffset;

    if (0 != dwDcStartTimeGrid)
    {
        qwDcStartTime -= qwDcStartTime % dwDcStartTimeGrid;
        qwDcStartTime += dwDcStartTimeGrid;
    }
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DcmCtlDcStartTime\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "qwBusTime             = 0x%08X%08X\n", EC_HIDWORD(qwBusTime), EC_LODWORD(qwBusTime)));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "m_qwBusTimeFirst      = 0x%08X%08X\n", EC_HIDWORD(m_qwBusTimeFirst), EC_LODWORD(m_qwBusTimeFirst)));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "dwBusCycleTimeNsec    =         0x%08X\n", m_pCtlparam->dwBusCycleTimeNsec));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "m_qwDcStartTimeOffset = 0x%08X%08X\n", EC_HIDWORD(m_qwDcStartTimeOffset), EC_LODWORD(m_qwDcStartTimeOffset)));

    return  qwDcStartTime;
}

/***************************************************************************************************/
/**
\brief  Execute the controller main loop. The function has to be called in each cycle.

\return N/A.
*/
EC_T_DWORD CEcDcmController::ProcessCtlError(EC_T_INT nCtlErrorNsec)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    m_pCtlparam->nCtlErrorNsec = nCtlErrorNsec;

    /* process reset */
    if (m_bResetRequest)
    {
        m_bResetRequest = EC_FALSE;
        m_dwStatusCode = EC_E_BUSY;
        dwRetVal = EC_E_BUSY;

        m_pCtlparam->nJob = CONTROLLER_JOB_RESET;
        this->Controller();
    }
    else
    {
        /* execute controller */
        m_pCtlparam->nJob = CONTROLLER_JOB_CTL;
        if (m_bNewSetVal)
        {
            m_pCtlparam->nJob = CONTROLLER_JOB_NEW_SETVAL;
            m_bNewSetVal = EC_FALSE;
        }

        dwRes = this->Controller();

        if (dwRes == EC_E_NOERROR)
        {
            SET_DCM_CTL_STATUS(eDcBsCtlControllerOk);
        }
        else if (dwRes == DCM_E_DRIFT_TO_HIGH)
        {
            SET_DCM_CTL_STATUS(eDcBsCtlDriftTooHigh);
        }
        else
        {
            SET_DCM_CTL_STATUS(eDcBsCtlControllerFailure);
        }
        /* don't override previous status */
        if ((EC_E_BUSY == m_dwStatusCode) || (EC_E_NOERROR == m_dwStatusCode))
        {
            m_dwStatusCode = dwRes;
        }

        if ((EC_E_BUSY == m_dwStatusCode) || (EC_E_NOERROR == m_dwStatusCode) || (DCM_E_DRIFT_TO_HIGH == m_dwStatusCode) || (DCM_E_MAX_CTL_ERROR_EXCEED == m_dwStatusCode))
        {
            m_pCtlparam->bReqAdjustment = EC_TRUE;
            dwRetVal = EC_E_NOERROR;
        }
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Process bus time, calculate error and execute the controller main loop. The function has to be called in each cycle.

\return N/A.
*/
EC_T_DWORD CEcDcmController::DistributeBusTimeProcess(EC_T_UINT64 qwBusTime)
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_poMaster->m_MasterConfig.dwStateChangeDebug >> 4) & 3);
#endif
    EC_T_INT  nCtlErrorNsec = 0;

    /* bus shift needs bus cycle time information parameter */
    if (0 == m_pCtlparam->dwBusCycleTimeNsec)
    {
        SET_DCM_CTL_STATUS(eDcBsCtlBusCycleTimeIsZero);
        m_bResetRequest = EC_TRUE;
        goto Exit;
    }
    /* bus shift needs DC slaves */
    if (0 == m_poMaster->m_pBusTopology->GetDCSlaveCount())
    {
        m_qwBusTimeFirst = 0;
        m_bResetRequest = EC_TRUE;
        m_dwStatusCode = EC_E_NOERROR;
        goto Exit;
    }
    /* handle reset request */
    if (m_bResetRequest)
    {
        EC_T_DWORD dwDcStartTimeGrid = m_poMaster->m_pDistributedClks->GetDcStartTimeGrid();

        SET_DCM_CTL_STATUS(eDcBsCtlResetRequest);

        /* align first bus time to DcStartTimeGrid to compensate DCM initial error */
        if (0 != dwDcStartTimeGrid)
        {
            m_pCtlparam->nMaxValidVal = dwDcStartTimeGrid / 2;
            m_qwBusTimeFirst = qwBusTime - (qwBusTime % dwDcStartTimeGrid);
            if ((qwBusTime - m_qwBusTimeFirst) > (dwDcStartTimeGrid / 2))
            {
                m_qwBusTimeFirst += dwDcStartTimeGrid;
            }
            m_qwBusTimeFirst -= GetCtlSetVal();
        }
        else
        {
            m_qwBusTimeFirst = qwBusTime;
        }
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL),
            "DCM BusTime at startup = 0x%08X%08X aligned to 0x%08X%08X\n",
            EC_HIDWORD(qwBusTime), EC_LODWORD(qwBusTime),
            EC_HIDWORD(m_qwBusTimeFirst), EC_LODWORD(m_qwBusTimeFirst)));

        m_dwBusCycleCount = 0;

        this->ProcessCtlError(0);
    }
    else
    {
        /* check if bus time is valid */
        if (qwBusTime == 0)
        {
            m_bResetRequest = EC_TRUE;
            if (qwBusTime == 0)
            {
                SET_DCM_CTL_STATUS(eDcBsCtlBusTimeIsZero);
            }
            goto Exit;
        }
        else if (qwBusTime < m_qwBusTimeLast)
        {
            /* invalid ! */
            m_qwBusTimeLast = qwBusTime;
            m_bResetRequest = EC_TRUE;
            SET_DCM_CTL_STATUS(eDcBsCtlBusTimeCountsBackwards);
            goto Exit;
        }
        m_dwBusCycleCount++;
        m_qwBusTimeLast = qwBusTime;

        /* check if real bus cycle time is matching the value in dwBusCycleTimeNsec */
        if (m_bCheckBusCycleTime)
        {
            if (m_dwBusCycleCount == 100)
            {
                EC_T_UINT64 qwTimeDiff = qwBusTime - m_qwBusTimeFirst;
                EC_T_INT    nRealCycles = 0;

                m_bCheckBusCycleTime = EC_FALSE;
                nRealCycles = EC_LODWORD(qwTimeDiff) / m_pCtlparam->dwBusCycleTimeNsec;
                if (nRealCycles <= 90)
                {
                    SET_DCM_CTL_STATUS(eDcBsCtlBusTimeDiffTooLow);
                    m_dwStatusCode = DCM_E_BUS_CYCLE_WRONG;
                    goto Exit;
                }
                else if (nRealCycles >= 110)
                {
                    SET_DCM_CTL_STATUS(eDcBsCtlBusTimeDiffTooHigh);
                    m_dwStatusCode = DCM_E_BUS_CYCLE_WRONG;
                    goto Exit;
                }
            }
        }
        /* calculate controller error */
        m_nActValNsec = (EC_T_INT)(EC_LODWORD((qwBusTime - m_qwBusTimeFirst) % m_pCtlparam->dwBusCycleTimeNsec));
        {
            EC_T_INT nCycleNsec = (EC_T_INT)m_pCtlparam->dwBusCycleTimeNsec;

            nCtlErrorNsec = m_nSetValNsec - m_nActValNsec;

            /* considering that the process value can only be between 0 and BusCycleTimeNsec, */
            /* the error can only be between -dwBusCycleTimeNsec/2 and dwBusCycleTimeNsec/2   */
            if (nCtlErrorNsec < -(nCycleNsec / 2))
            {
                nCtlErrorNsec += nCycleNsec;
            }
            else if (nCtlErrorNsec >(nCycleNsec / 2))
            {
                nCtlErrorNsec -= nCycleNsec;
            }
        }
        /* execute controller */
        this->ProcessCtlError(nCtlErrorNsec);
    }
    SET_DCM_CTL_STATUS(eDcBsCtlOk);

Exit:
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Update DCM controller logging.

\return EC_E_NOERROR on success, otherwise error code.
*/
EC_T_DWORD CEcDcmController::UpdateLog(EC_T_DCM_LOG* pDcmLog, EC_T_CHAR** ppszLog, EC_T_DWORD dwMaxLogSize)
{
    EC_T_DWORD   dwRetVal = EC_E_ERROR;
    EC_T_INT     nSystemTimeDifference = 0;
    EC_T_UINT64  qwBusTime = m_poMaster->m_pDistributedClks->GetBusTime();
    EC_T_UINT64  qwDcStartTime = m_poMaster->m_pDistributedClks->GetDcStartTime();
    EC_T_WORD    wRefClock = (EC_T_WORD)((EC_NULL != m_poMaster->m_pDistributedClks->m_pBusSlaveRefClock) ? m_poMaster->m_pDistributedClks->m_pBusSlaveRefClock->GetFixedAddress() : 0xFFFF);

    /* calculate system time difference */
    if (EC_NULL != m_poMaster->m_pDistributedClks->m_pBusSlaveSyncWindowMonitoringLast)
    {
        EC_T_DWORD dwSystemTimeDifference = m_poMaster->m_pDistributedClks->m_pBusSlaveSyncWindowMonitoringLast->m_dwSystemTimeDifference;

        nSystemTimeDifference = (EC_T_INT)(dwSystemTimeDifference & ~0x80000000);
        if (0 != (dwSystemTimeDifference & 0x80000000))
        {
            nSystemTimeDifference = -1 * nSystemTimeDifference;
        }
    }
    /* refresh current logging data set */
    pDcmLog->dwMsecCounter = m_poMaster->m_dwMsecCounter;
    pDcmLog->nCtlSetVal = (EC_T_INT)this->GetCtlSetVal();
    pDcmLog->qwBusTime = qwBusTime;
    pDcmLog->nCtlErrorNsec = m_pCtlparam->nCtlErrorNsec;
    pDcmLog->nDrift = m_pCtlparam->nCurrDrift / DRIFT_CALC_STEP_WIDTH;
    pDcmLog->dwErrorCode = m_dwStatusCode;
    pDcmLog->bDcmInSync = m_poMaster->m_pDistributedClks->DcmIsInSync();
    pDcmLog->bDcInSync = m_poMaster->m_pDistributedClks->IsInSync();
    pDcmLog->qwDcStartTime = qwDcStartTime;
    pDcmLog->nSystemTimeDifference = nSystemTimeDifference;

    /* print logging string */
    if (1 < dwMaxLogSize)
    {
        /* Time [ms];SetVal [ns];BusTime [ns];ActVal [ns];CtlAdj; CtlErr [ns];CtlErrFilt; Drift; CtlErr [1/10 pmil]; CtlErrI; CtlOutTot[pmil]; DC StartTime;DCM ErrorCode;DCM InSync;DC InSync;SystemTimeDiff [ns]*/
        *ppszLog += OsSnprintf(*ppszLog, dwMaxLogSize, "%lu; %ld; %lu; %lu; %ld; %d; %d;  %d; %d; %ld; %ld; %lu; %lu; %ld; %ld; %ld; %d",
            m_poMaster->m_dwMsecCounter,                                    /* Time [ms]            */
            (EC_T_INT)this->GetCtlSetVal(),                                 /* SetVal [ns]          */
            EC_LODWORD(qwBusTime),                                          /* BusTime [ns]         */
            this->GetCtlActVal(),                                           /* ActVal [ns]          */
            m_pCtlparam->nTimerAdjust,                                      /* CtlAdj               */
            m_pCtlparam->nCtlErrorNsec,                                     /* CtlErr [nsec]        */
            m_pCtlparam->nCtlErrorFilt,                                     /* CtlErrFilt           */
            m_pCtlparam->nCurrDrift / DRIFT_CALC_STEP_WIDTH,                /* Drift [ppm]          */
            m_pCtlparam->nCtlErrorPmil,                                     /* CtlErr [1/10 pmil]   */
            m_pCtlparam->nCtlOutputSumComp,                                 /* CtlOutSum            */
            (EC_T_INT)m_pCtlparam->nCtlOutputTotal,                         /* CtlOutTot [pmil]     */
            EC_LODWORD(qwDcStartTime),                                      /* DC StartTime         */
            m_dwStatusCode,                                                 /* DCM ErrorCode        */
            m_poMaster->m_pDistributedClks->DcmIsInSync(),                  /* DCM InSync           */
            m_poMaster->m_pDistributedClks->IsInSync(),                     /* DC InSync            */
            nSystemTimeDifference,                                          /* SystemTimeDiff [ns]  */
            wRefClock                                                       /* RefClock FixedAddr   */
            );
        dwRetVal = EC_E_NOERROR;
    }
    else
    {
        dwRetVal = EC_E_INVALIDSIZE;
    }
    
    return dwRetVal;
}

/*-CEcDcmBase----------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Default constructor.

*/
CEcDcmBase::CEcDcmBase(CEcMaster* poMaster)
{
    m_poMaster           = poMaster;
    m_poCfgSlaveSyncRef  = EC_NULL;

    OsMemset(&m_oLog, 0, sizeof(EC_T_DCM_LOG));
    m_szLog[0] = '\0';
    m_pszLogNext = m_szLog;

    m_dwRefPulseLength   = 0;

    m_poInSync = EC_NEW(CEcDcmInSync(poMaster));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDcmBase::m_poInSync", m_poInSync, sizeof(CEcDcmInSync));

    m_dwTimeStampLastResult = 0;
}

CEcDcmBase::~CEcDcmBase()
{
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "~CEcDcmBase::m_poInSync", m_poInSync, sizeof(CEcDcmInSync));
    SafeDelete(m_poInSync);
}

/***************************************************************************************************/
/**
\brief  Get current DC master synchronization logging string

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmBase::GetLog(
    EC_T_CHAR** pszLog  /**< [out]   pointer to the current log string */
    )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (m_pszLogNext != m_szLog)
    {
        *pszLog = &m_szLog[0];
        m_pszLogNext = m_szLog;
        dwRetVal = EC_E_NOERROR;
    }
    
    return dwRetVal;
}
/***************************************************************************************************/
/**
\brief  Get current DC master synchronization logging struct

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmBase::GetLog(EC_T_DCM_LOG* pDcmLog)
{
    OsMemcpy(pDcmLog, &m_oLog, sizeof(EC_T_DCM_LOG));
    return EC_E_NOERROR;
}

EC_T_DWORD CEcDcmBase::GetTimeStampCallback(
    EC_PF_TIMESTAMP*    ppfnTimeStamp,
    EC_T_PVOID*         ppvTimeStampContext,
    EC_T_DWORD**        ppdwTimeStampLast
    )
{
    if (EC_NULL != ppfnTimeStamp)
        *ppfnTimeStamp = TimeStampCallback;
    if (EC_NULL != ppvTimeStampContext)
        *ppvTimeStampContext = this;
    if (EC_NULL != ppdwTimeStampLast)
        *ppdwTimeStampLast = &m_dwTimeStampLastResult;

    return EC_E_INVALIDSTATE;
}

EC_T_UINT64 CEcDcmBase::DcStartTime(EC_T_UINT64 qwBusTime, EC_T_UINT64 qwCycleTime)
{
    EC_T_DWORD dwDcStartTimeGrid = m_poMaster->m_pDistributedClks->GetDcStartTimeGrid();

    EC_UNREFPARM(qwCycleTime);

    if (0 != dwDcStartTimeGrid)
    {
        qwBusTime += dwDcStartTimeGrid - (qwBusTime % dwDcStartTimeGrid);
    }

    return qwBusTime;
}

/***************************************************************************************************/
/**
\brief  Get bus time reference slave

/return BusTimeRefSlave
*/
CEcBusSlave* CEcDcmBase::GetBusSlaveBusTimeRef(EC_T_VOID)
{ 
    return m_poMaster->m_pDistributedClks->m_pBusSlaveRefClock;
}

#if (defined INCLUDE_MASTER_OBD)
EC_T_VOID CEcDcmBase::UpdateSdoService(EC_T_VOID)
{
    EC_T_OBJ2102* pSdoDcmObject = &m_poMaster->m_pSDOService->m_oDcmBusShiftObject;
    EC_T_DWORD    dwErrorCode   = EC_E_ERROR;
    EC_T_INT      nCtlErrorFilt = 0;
    EC_T_INT      nCtlErrorAvg  = 0;
    EC_T_INT      nCtlErrorMax  = 0;
    this->GetStatus(&dwErrorCode, &nCtlErrorFilt, &nCtlErrorAvg, &nCtlErrorMax);

    pSdoDcmObject->bDcInSync     = m_poMaster->m_pDistributedClks->IsInSync();
    pSdoDcmObject->bDcmInSync    = this->IsInSync();
    pSdoDcmObject->nCtlSetVal    = this->GetCtlSetVal();
    pSdoDcmObject->dwErrorCode   = dwErrorCode;
    pSdoDcmObject->nCtlErrorFilt = nCtlErrorFilt;
    pSdoDcmObject->nCtlErrorAvg  = nCtlErrorAvg;
    pSdoDcmObject->nCtlErrorMax  = nCtlErrorMax;

    m_poMaster->m_pSDOService->EntrySet_SetDCMInSync(IsInSync());
}
#endif /* INCLUDE_MASTER_OBD */

EC_T_DWORD CEcDcmBase::TimeStampCallback(EC_T_PVOID pvContext, EC_T_DWORD* pdwLocalTime)
{
    return ((CEcDcmBase*)pvContext)->TimeStampCallback(pdwLocalTime);
}

EC_T_DWORD CEcDcmBase::TimeStampCallback(EC_T_DWORD* pdwLocalTime)
{
EC_T_INT nCountPermil = 0;

    OsHwTimerGetCurrentCount(&nCountPermil); EC_UNREFPARM(nCountPermil);

    *pdwLocalTime = (EC_T_DWORD)(nCountPermil * m_poMaster->GetBusCycleTimeUsec());

    return EC_E_NOERROR;
}

/*-CEcDcmMasterShift---------------------------------------------------------*/
CEcDcmMasterShift::CEcDcmMasterShift(CEcMaster* poMaster) : CEcDcmBase(poMaster), m_oController(poMaster)
{
    OsMemset(&m_oDcmConfig, 0, sizeof(EC_T_DCM_CONFIG_MASTERSHIFT));
    OsMemset(&m_oCtlparam, 0, sizeof(DCBS_CTL_PARAM));
    m_oCtlparam.bBuildTotalCtlErr = EC_TRUE;
    m_oCtlparam.bCalcDrift = EC_FALSE;
    m_oCtlparam.bIncrementOutput = EC_FALSE;

    m_oController.Init(&m_oCtlparam);

    OsSnprintf(m_szLog, DCM_LOG_BUFFER_SIZE, "Time [ms];SetVal [ns];BusTime [ns];ActVal [ns];CtlAdj;CtlErr [ns];CtlErrFilt; Drift [ppm]; CtlErr [1/10 pmil]; CtlOutSum; CtlOutTot [pmil];DC StartTime;DCM ErrorCode;DCM InSync;DC InSync;SystemTimeDiff [ns];RefClock FixedAddr");
    m_pszLogNext = m_szLog + OsStrlen(m_szLog);
}

/***************************************************************************************************/
/**
\brief  Get status of DCM Mastershift controller

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmMasterShift::GetStatus(
    EC_T_DWORD* pdwErrorCode,
    EC_T_INT*   pnDiffCur,
    EC_T_INT*   pnDiffAvg,
    EC_T_INT*   pnDiffMax
    )
{
    *pdwErrorCode = m_oController.GetStatusCode();
    *pnDiffCur = m_oCtlparam.nCtlErrorNsec;
    *pnDiffAvg = m_poInSync->GetCtlErrorAvg();
    *pnDiffMax = m_poInSync->GetCtlErrorMax();


    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Configure Mastershift DCM controller and insync monitoring

\return EC_E_NOERROR on success, otherwise error code.
*/
EC_T_DWORD CEcDcmMasterShift::Configure(EC_T_DCM_CONFIG* pDcmConfig)
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_poMaster->m_MasterConfig.dwStateChangeDebug >> 4) & 3);
#endif
    EC_T_DWORD dwRes = EC_E_NOERROR;
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    OsMemcpy(&m_oDcmConfig, &pDcmConfig->u.MasterShift, sizeof(EC_T_DCM_CONFIG_MASTERSHIFT));
    
    if (!m_oDcmConfig.bCtlOff)
    {
        EC_T_DWORD dwTmp = 0;
        dwRes = OsHwTimerGetInputFrequency(&dwTmp); EC_UNREFPARM(dwTmp);
        if (dwRes != EC_E_NOERROR) { dwRetVal = dwRes; goto Exit; }
    }

    /* configure controller */
    {
        EC_T_DCM_CONFIG_BUSSHIFT oBusShiftConfig;
        OsMemset(&oBusShiftConfig, 0, sizeof(EC_T_DCM_CONFIG_BUSSHIFT));

        oBusShiftConfig.nCtlSetVal = m_oDcmConfig.nCtlSetVal;
        oBusShiftConfig.nCtlGain = m_oDcmConfig.nCtlGain;
        oBusShiftConfig.nCtlDriftErrorGain = m_oDcmConfig.nCtlDriftErrorGain;
        oBusShiftConfig.nMaxValidVal = m_oDcmConfig.nMaxValidVal;
        oBusShiftConfig.bCtlOff = m_oDcmConfig.bCtlOff;

        dwRes = m_oController.Init(&m_oCtlparam);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

        dwRes = m_oController.Configure(&oBusShiftConfig);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }

    /* configure InSync monitoring */
    {
        /* 5 % limit in NSEC for InSync monitoring */
        EC_T_DWORD dwInSyncLimit = m_poMaster->GetBusCycleTimeUsec() * (1000 / 20);

        if (0 != pDcmConfig->u.MasterShift.dwInSyncLimit)
            dwInSyncLimit = pDcmConfig->u.MasterShift.dwInSyncLimit;

        m_poInSync->Configure(dwInSyncLimit, pDcmConfig->u.MasterShift.dwInSyncSettleTime, DCM_IN_SYNC_START_DELAY_CYCLES);
    }
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DcmConfigure()\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCM Mode           = eDcmMode_MasterShift\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "InSync limit       = %d\n", m_poInSync->GetInSyncLimit()));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "InSync settle time = %d\n", m_poInSync->GetInSyncSettleTime()));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Controller SetVal  = %d\n", m_oController.GetCtlSetVal()));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Controller gain    = %d\n", m_oCtlparam.nGain));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Drift error gain   = %d\n", m_oCtlparam.nDriftErrorGain));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Max valid value    = %d\n", m_oCtlparam.nMaxValidVal));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Controller off     = %d\n", m_oDcmConfig.bCtlOff));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Logging enabled    = %d\n", m_oDcmConfig.bLogEnabled));

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get DCM Mastershift configuration

\return EC_E_NOERROR on success, otherwise error code.
*/
EC_T_DWORD CEcDcmMasterShift::GetConfig(EC_T_DCM_CONFIG* pDcmConfig)
{
    EC_T_DCM_CONFIG_BUSSHIFT oCtlConfig;
    OsMemset(&oCtlConfig, 0, sizeof(EC_T_DCM_CONFIG_BUSSHIFT));

    pDcmConfig->eMode = eDcmMode_MasterShift;
    OsMemcpy(&pDcmConfig->u.MasterShift, &m_oDcmConfig, sizeof(EC_T_DCM_CONFIG_MASTERSHIFT));

    /* get configuration from controller */
    m_oController.GetConfig(&oCtlConfig);
    pDcmConfig->u.MasterShift.nCtlGain = oCtlConfig.nCtlGain;
    pDcmConfig->u.MasterShift.nCtlDriftErrorGain = oCtlConfig.nCtlDriftErrorGain;
    pDcmConfig->u.MasterShift.nMaxValidVal = oCtlConfig.nMaxValidVal;
    pDcmConfig->u.MasterShift.nCtlSetVal = oCtlConfig.nCtlSetVal;

    /* get InSync configuration */
    pDcmConfig->u.MasterShift.dwInSyncLimit = m_poInSync->GetInSyncLimit();
    pDcmConfig->u.MasterShift.dwInSyncSettleTime = m_poInSync->GetInSyncSettleTime();

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Adjust HW timer

\return EC_E_NOERROR on success, otherwise error code.
*/
EC_T_DWORD CEcDcmMasterShift::AdjustTimer(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    m_oCtlparam.nTimerAdjustWaitTickCounter--;
    if ((0 != m_oCtlparam.nCtlOutputTotal) && (m_oCtlparam.nTimerAdjustWaitTickCounter <= 0))
    {
        m_oCtlparam.nTimerAdjustWaitTickCounter = GetAdjustWaitCycles();
        m_oCtlparam.nTimerAdjust = m_oCtlparam.nCtlOutputTotal;
    }
    else
    {
        m_oCtlparam.nTimerAdjust = 0;
    }
    if (!m_oDcmConfig.bCtlOff)
    {
        /* call kernel for adjustment of timer */
        dwRetVal = OsHwTimerModifyInitialCount(m_oCtlparam.nTimerAdjust);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Called each cycle marked as measurement cycle, when processing ARMW response.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmMasterShift::DistributeSystemTimeProcess(
    EC_T_BOOL   bStamped,
    EC_T_DWORD  dwLocalTime,
    EC_T_DWORD  dwLocalTime2
    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_DWORD dwRes = EC_E_NOERROR;
    CEcDistributedClocks* poDC = m_poMaster->m_pDistributedClks;
    EC_T_UINT64 qwBusTime = poDC->GetBusTime();
    
    EC_UNREFPARM(dwLocalTime2);
    if (bStamped)
    {
        qwBusTime = qwBusTime - dwLocalTime;
    }
    if ((EC_NULL != poDC->m_pBusSlaveRefClock) && poDC->m_bDCInitialized)
    {
        dwRes = m_oController.DistributeBusTimeProcess(qwBusTime);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

        if (m_oCtlparam.bReqAdjustment)
        {
            this->AdjustTimer();
            m_oCtlparam.bReqAdjustment = EC_FALSE;
        }

        /* InSync monitoring */
        dwRes = m_poInSync->NotifyCtlError(m_oCtlparam.nCtlErrorNsec);
        /* Update controller status */
        if (EC_E_BUSY != dwRes)
            m_oController.SetStatusCode(dwRes);

        /* handle logging */
        if (m_oDcmConfig.bLogEnabled)
        {
            EC_T_DWORD dwMaxLogSize = (EC_T_DWORD)(DCM_LOG_BUFFER_SIZE - (m_pszLogNext - m_szLog));

            dwRes = m_oController.UpdateLog(&m_oLog, &m_pszLogNext, dwMaxLogSize);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }

#if (defined INCLUDE_MASTER_OBD)
        UpdateSdoService();
#endif
    }
    
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/*-CEcDcmBusShift------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Default constructor.

*/
CEcDcmBusShift::CEcDcmBusShift(CEcMaster* poMaster) : CEcDcmBase(poMaster), m_oController(poMaster)
{
    OsMemset(&m_oDcmConfig, 0, sizeof(EC_T_DCM_CONFIG_BUSSHIFT));
    OsMemset(&m_oCtlparam, 0, sizeof(DCBS_CTL_PARAM));
    m_oCtlparam.bBuildTotalCtlErr = EC_FALSE;
    m_oCtlparam.bCalcDrift = EC_TRUE;
    m_oCtlparam.bIncrementOutput = EC_TRUE;

    m_oController.Init(&m_oCtlparam);

    OsMemset(&m_oDcShiftDesc, 0, sizeof(EC_T_DC_SHIFTSYSTIME_DESC));

    OsSnprintf(m_szLog, DCM_LOG_BUFFER_SIZE, "Time [ms];SetVal [ns];BusTime [ns];ActVal [ns];CtlAdj;CtlErr [ns];CtlErrFilt; Drift [ppm]; CtlErr [1/10 pmil]; CtlOutSum; CtlOutTot [pmil];DC StartTime;DCM ErrorCode;DCM InSync;DC InSync;SystemTimeDiff [ns];RefClock FixedAddr");
    m_pszLogNext = m_szLog + OsStrlen(m_szLog);
}

/***************************************************************************************************/
/**
\brief  Get status of DCM Busshift controller

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmBusShift::GetStatus(
    EC_T_DWORD* pdwErrorCode,
    EC_T_INT*   pnDiffCur,
    EC_T_INT*   pnDiffAvg,
    EC_T_INT*   pnDiffMax
    )
{
    *pdwErrorCode = m_oController.GetStatusCode();
    *pnDiffCur = m_oCtlparam.nCtlErrorNsec;
    *pnDiffAvg = m_poInSync->GetCtlErrorAvg();
    *pnDiffMax = m_poInSync->GetCtlErrorMax();

    return EC_E_NOERROR;
}


/***************************************************************************************************/
/**
\brief  Initialize DCM Busshift controller

\return EC_E_NOERROR on success, otherwise error code.
*/
EC_T_DWORD CEcDcmBusShift::Initialize(EC_T_VOID)
{
    EC_T_DWORD dwRes = EC_E_ERROR;
    
    dwRes = m_oController.Init(&m_oCtlparam);
    if (EC_E_NOERROR != dwRes) { goto Exit; }

    /* 5 % limit in nsec for InSync monitoring */
    {
        EC_T_DWORD dwInSyncLimit = m_poMaster->GetBusCycleTimeUsec() * (1000 / 20);
        m_poInSync->Configure(dwInSyncLimit, 0, DCM_IN_SYNC_START_DELAY_CYCLES);
    }

    dwRes = EC_E_NOERROR;
Exit:
    return dwRes;
}

/***************************************************************************************************/
/**
\brief  Configure DCM Busshift controller and insync monitoring

\return EC_E_NOERROR on success, otherwise error code.
*/
EC_T_DWORD CEcDcmBusShift::Configure(EC_T_DCM_CONFIG* pDcmConfig)
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_poMaster->m_MasterConfig.dwStateChangeDebug >> 4) & 3);
#endif
    EC_T_DWORD dwRes = EC_E_ERROR;

    if (m_poMaster->IsConfigLoaded() && !m_poMaster->m_pDistributedClks->DcmGetBusShiftConfigured())
    {
        dwRes = EC_E_FEATURE_DISABLED;
        goto Exit;
    }

    OsMemcpy(&m_oDcmConfig, &pDcmConfig->u.BusShift, sizeof(EC_T_DCM_CONFIG_BUSSHIFT));

    /* configure controller */
    dwRes = m_oController.Init(&m_oCtlparam);
    if (EC_E_NOERROR != dwRes) { goto Exit; }
    
    dwRes = m_oController.Configure(&m_oDcmConfig);
    if (EC_E_NOERROR != dwRes) { goto Exit; }

    /* configure InSync monitoring */
    m_poInSync->Configure(pDcmConfig->u.BusShift.dwInSyncLimit, pDcmConfig->u.BusShift.dwInSyncSettleTime, DCM_IN_SYNC_START_DELAY_CYCLES);

    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DcmConfigure()\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCM Mode           = eDcmMode_Busshift\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "InSync limit       = %d\n", m_poInSync->GetInSyncLimit()));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "InSync settle time = %d\n", m_poInSync->GetInSyncSettleTime()));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Controller SetVal  = %d\n", m_oController.GetCtlSetVal()));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Controller gain    = %d\n", m_oCtlparam.nGain));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Drift error gain   = %d\n", m_oCtlparam.nDriftErrorGain));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Max valid value    = %d\n", m_oCtlparam.nMaxValidVal));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Controller off     = %d\n", m_oDcmConfig.bCtlOff));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Logging enabled    = %d\n", m_oDcmConfig.bLogEnabled));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DC loop std values = %d\n", m_oDcmConfig.bUseDcLoopCtlStdValues));

    dwRes = EC_E_NOERROR;
Exit:
    return dwRes;
}

/***************************************************************************************************/
/**
\brief  Get DCM Busshift configuration

\return EC_E_NOERROR
*/
EC_T_DWORD CEcDcmBusShift::GetConfig(EC_T_DCM_CONFIG* pDcmConfig)
{
    pDcmConfig->eMode = eDcmMode_BusShift;
    OsMemcpy(&pDcmConfig->u.BusShift, &m_oDcmConfig, sizeof(EC_T_DCM_CONFIG_BUSSHIFT));
    /* get controller configuration */
    m_oController.GetConfig(&pDcmConfig->u.BusShift);
    /* InSync configuration */
    pDcmConfig->u.BusShift.dwInSyncLimit = m_poInSync->GetInSyncLimit();
    pDcmConfig->u.BusShift.dwInSyncSettleTime = m_poInSync->GetInSyncSettleTime();

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Adjust distributed clocks shift time

\return EC_E_NOERROR
*/
EC_T_DWORD CEcDcmBusShift::AdjustDcShiftTime(EC_T_VOID)
{
    if (0 != m_oCtlparam.nCtlOutputTotal)
    {               
        /* clear DC shift descriptor */
        OsMemset(&m_oDcShiftDesc, 0, sizeof(m_oDcShiftDesc));

        /* get the values of the controller */
        m_oDcShiftDesc.dwCyclesToShift = 1;
        m_oDcShiftDesc.nShiftTime = m_oCtlparam.nCtlOutputTotal;

        /* shift DC clock master */
        if (m_oDcShiftDesc.nShiftTime > 0)
        {
            m_oDcShiftDesc.nShiftTime = (EC_T_INT)(m_oCtlparam.dwBusCycleTimeNsec * 2);
        }
        else if (m_oDcShiftDesc.nShiftTime < 0)
        {
            m_oDcShiftDesc.nShiftTime = (EC_T_INT)(m_oCtlparam.dwBusCycleTimeNsec / 2);
        }
        if (m_oCtlparam.nTimerAdjustWaitTickCounter <= 0)
        {
            m_oCtlparam.nTimerAdjustWaitTickCounter = GetAdjustWaitCycles();
            if (!m_oDcmConfig.bCtlOff)
            {
                m_poMaster->m_pDistributedClks->DcShiftSystemTime(&m_oDcShiftDesc);
            }
        }
        else
        {
            m_oCtlparam.nTimerAdjustWaitTickCounter--;
            m_oDcShiftDesc.nShiftTime = 0;
        }
        m_oCtlparam.nTimerAdjust = m_oDcShiftDesc.nShiftTime;
    }
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Called each cycle marked as measurement cycle, when processing ARMW response.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmBusShift::DistributeSystemTimeProcess(
    EC_T_BOOL   bStamped,
    EC_T_DWORD  dwLocalTime,
    EC_T_DWORD  dwLocalTime2
    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_DWORD dwRes = EC_E_NOERROR;
    CEcDistributedClocks* poDC = m_poMaster->m_pDistributedClks;
    EC_T_UINT64 qwBusTime = poDC->GetBusTime();

    EC_UNREFPARM(dwLocalTime2);
    if (bStamped)
    {
        qwBusTime = qwBusTime - dwLocalTime;
    }
    if ((EC_NULL != poDC->m_pBusSlaveRefClock) && poDC->m_bDCInitialized)
    {
        dwRes = m_oController.DistributeBusTimeProcess(qwBusTime);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

        /* handle output */
        if (m_oCtlparam.bReqAdjustment)
        {
            this->AdjustDcShiftTime();
            m_oCtlparam.bReqAdjustment = EC_FALSE;
        }

        /* InSync monitoring */
        dwRes = m_poInSync->NotifyCtlError(m_oCtlparam.nCtlErrorNsec);
        /* Update controller status */
        if (EC_E_BUSY != dwRes)
            m_oController.SetStatusCode(dwRes);

        /* handle logging */
        if (m_oDcmConfig.bLogEnabled)
        {
            EC_T_DWORD dwMaxLogSize = (EC_T_DWORD)(DCM_LOG_BUFFER_SIZE - (m_pszLogNext - m_szLog));

            dwRes = m_oController.UpdateLog(&m_oLog, &m_pszLogNext, dwMaxLogSize);
            if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
        }

#if (defined INCLUDE_MASTER_OBD)
        UpdateSdoService();
#endif
    }
    
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}
/*-CEcDcmMasterRefClock------------------------------------------------------*/
CEcDcmMasterRefClock::CEcDcmMasterRefClock(CEcMaster* poMaster) : CEcDcmBase(poMaster)
{
    OsMemset(&m_oDcmConfig, 0, sizeof(EC_T_DCM_CONFIG_MASTERREFCLOCK));
    m_poBusSlaveBusTimeRef = EC_NULL;

    m_qwMasterTime  = 0;
    m_nCtlErrorNsec = 0;
    m_dwStatusCode 	= EC_E_NOTREADY;
    
    /* prepare DCM log */
    OsSnprintf(m_szLog, DCM_LOG_BUFFER_SIZE, "Time [ms];SetVal [ns];BusTime [ns];MasterTime [ns];CtlErrBefFil [ns];DCM ErrorCode;DCM InSync;DC StartTime;DC InSync;SystemTimeDifference [ns]");
    m_pszLogNext = m_szLog + OsStrlen(m_szLog);
}

EC_T_DWORD CEcDcmMasterRefClock::GetStatus(EC_T_DWORD* pdwErrorCode, EC_T_INT* pnDiffCur, EC_T_INT* pnDiffAvg, EC_T_INT* pnDiffMax)
{
    EC_UNREFPARM(pdwErrorCode); 
    *pnDiffCur = m_nCtlErrorNsec;
    *pnDiffAvg = m_poInSync->GetCtlErrorAvg(); 
    *pnDiffMax = m_poInSync->GetCtlErrorMax();
    return m_dwStatusCode;
}

/***************************************************************************************************/
/**
\brief  Configure DCM Master is reference clock and insync monitoring

\return EC_E_NOERROR
*/
EC_T_DWORD CEcDcmMasterRefClock::Configure(EC_T_DCM_CONFIG* pDcmConfig)
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_poMaster->m_MasterConfig.dwStateChangeDebug >> 4) & 3);
#endif
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* MasterRefClock needs bus shift configuration in Eni */
    if (m_poMaster->IsConfigLoaded() && !m_poMaster->m_pDistributedClks->DcmGetBusShiftConfigured())
    {
        dwRetVal = EC_E_FEATURE_DISABLED;
        goto Exit;
    }

    OsMemcpy(&m_oDcmConfig, &pDcmConfig->u.MasterRefClock, sizeof(EC_T_DCM_CONFIG_MASTERREFCLOCK));

    /* configure InSync */
    {
        EC_T_DWORD dwInSyncLimit = 10000; 
        EC_T_DWORD dwInSyncSettleTime = 2000;

        if (0 != pDcmConfig->u.MasterRefClock.dwInSyncLimit)
            dwInSyncLimit = pDcmConfig->u.MasterRefClock.dwInSyncLimit;

        if (0 != pDcmConfig->u.MasterRefClock.dwInSyncSettleTime)
            dwInSyncSettleTime = pDcmConfig->u.MasterRefClock.dwInSyncSettleTime;
        /* Start InSync monitoring immediately */
        m_poInSync->Configure(dwInSyncLimit, dwInSyncSettleTime);
    }
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DcmConfigure()\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCM Mode           = eDcmMode_MasterRefClock\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "InSync limit       = %d\n", m_poInSync->GetInSyncLimit()));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "InSync settle time = %d\n", m_poInSync->GetInSyncSettleTime()));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Controller SetVal  = %d\n", m_oDcmConfig.nCtlSetVal));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Logging enabled    = %d\n", m_oDcmConfig.bLogEnabled));

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_DWORD CEcDcmMasterRefClock::GetConfig(EC_T_DCM_CONFIG* pDcmConfig)
{   
    pDcmConfig->eMode = eDcmMode_MasterRefClock;    
    OsMemcpy(&pDcmConfig->u.MasterRefClock, &m_oDcmConfig, sizeof(EC_T_DCM_CONFIG_MASTERREFCLOCK));
    pDcmConfig->u.MasterRefClock.dwInSyncLimit = m_poInSync->GetInSyncLimit();
    pDcmConfig->u.MasterRefClock.dwInSyncSettleTime = m_poInSync->GetInSyncSettleTime();
     
    return EC_E_NOERROR;
}
/***************************************************************************************************/
/**
\brief  Called each cycle marked as measurement cycle, when processing ARMW response.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcmMasterRefClock::DistributeSystemTimeProcess(
    EC_T_BOOL   bStamped,
    EC_T_DWORD  dwLocalTime,
    EC_T_DWORD  dwLocalTime2
    )
{
    CEcDistributedClocks* poDC = m_poMaster->m_pDistributedClks;

    EC_UNREFPARM(bStamped); EC_UNREFPARM(dwLocalTime); EC_UNREFPARM(dwLocalTime2);
    
    /* execute DCM monitoring */
    if (poDC->m_bDCInitialized)
    {
        m_nCtlErrorNsec = (EC_T_INT)(poDC->GetBusTime() - m_qwMasterTime);

        /* InSync */
        m_dwStatusCode = m_poInSync->NotifyCtlError(m_nCtlErrorNsec);

        /* logging */
        if (m_oDcmConfig.bLogEnabled)
        {
            EC_T_INT nLogMax = DCM_LOG_BUFFER_SIZE - (EC_T_INT)(m_pszLogNext - m_szLog);

            if (1 < nLogMax)
            {
                EC_T_INT nSystemTimeDifference = 0;

                if (EC_NULL != poDC->m_pBusSlaveSyncWindowMonitoringLast)
                {
                    EC_T_DWORD dwSystemTimeDifference = poDC->m_pBusSlaveSyncWindowMonitoringLast->m_dwSystemTimeDifference;

                    nSystemTimeDifference = (EC_T_INT)(dwSystemTimeDifference & ~0x80000000);
                    if (0 != (dwSystemTimeDifference & 0x80000000))
                    {
                        nSystemTimeDifference = -1 * nSystemTimeDifference;
                    }
                }
                m_pszLogNext += OsSnprintf(m_pszLogNext, nLogMax, "%u; %ld; %lu; %lu; %ld; 0x%08X; %d; %lu; %d; %d",
                    m_poMaster->m_dwMsecCounter,            /* Time [ms]                */
                    m_oDcmConfig.nCtlSetVal,                /* SetVal [ns]              */
                    EC_LODWORD(poDC->GetBusTime()),         /* BusTime [ns]             */
                    EC_LODWORD(m_qwMasterTime),             /* MasterTime [ns]          */
                    m_nCtlErrorNsec,                        /* CtlErr [ns]              */
                    m_dwStatusCode,                         /* DCM ErrorCode            */
                    m_poInSync->IsInSync(),                 /* DCM InSync               */
                    EC_LODWORD(poDC->GetDcStartTime()),     /* DC StartTime             */
                    poDC->IsInSync(),                       /* DC InSync                */
                    nSystemTimeDifference);                 /* SystemTimeDifference [ns]*/
            }
        }
#if (defined INCLUDE_MASTER_OBD)
        UpdateSdoService();
#endif
    }
    return EC_E_NOERROR;
}

/*-CEcDcmLinkLayerRefClock---------------------------------------------------*/
CEcDcmLinkLayerRefClock::CEcDcmLinkLayerRefClock(CEcMaster* poMaster) : CEcDcmBase(poMaster)
{ 
    OsMemset(&m_oDcmConfig, 0, sizeof(EC_T_DCM_CONFIG_LINKLAYERREFCLOCK));
    m_poBusSlaveBusTimeRef = EC_NULL;
    m_bInSync     = EC_FALSE;

    /* prepare DCM log */
    OsSnprintf(m_szLog, DCM_LOG_BUFFER_SIZE, "Time [ms];SetVal [ns];BusTime [ns];DC StartTime;DC InSync;SystemTimeDifference [ns]");
    m_pszLogNext = m_szLog + OsStrlen(m_szLog);
}

EC_T_DWORD CEcDcmLinkLayerRefClock::Configure(EC_T_DCM_CONFIG* pDcmConfig)
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_poMaster->m_MasterConfig.dwStateChangeDebug >> 4) & 3);
#endif
    EC_T_DWORD dwRes = EC_E_NOERROR;
    EC_T_UINT64 qwTime = 0;
    EC_T_LINK_IOCTLPARMS oIoCtlParms;
    OsMemset(&oIoCtlParms, 0, sizeof(EC_T_LINK_IOCTLPARMS));

    /* test the linklayer compatibility */
    oIoCtlParms.pbyOutBuf = (EC_T_BYTE*)&qwTime;
    oIoCtlParms.dwOutBufSize = sizeof(EC_T_UINT64);
    dwRes = m_poMaster->m_poEcDevice->LinkIoctl(EC_LINKIOCTL_GETTIME, &oIoCtlParms);
    if (EC_E_NOERROR != dwRes)
    {
        return EC_E_NOTSUPPORTED;
    }

    OsMemcpy(&m_oDcmConfig, &pDcmConfig->u.LinkLayerRefClock, sizeof(EC_T_DCM_CONFIG_LINKLAYERREFCLOCK));

    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DcmConfigure()\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCM Mode           = eDcmMode_LinkLayerRefClock\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Controller SetVal  = %d\n", m_oDcmConfig.nCtlSetVal));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Logging enabled    = %d\n", m_oDcmConfig.bLogEnabled));

    return EC_E_NOERROR;
}

EC_T_DWORD CEcDcmLinkLayerRefClock::GetConfig(EC_T_DCM_CONFIG* pDcmConfig)
{
    pDcmConfig->eMode = eDcmMode_LinkLayerRefClock;
    OsMemcpy(&pDcmConfig->u.LinkLayerRefClock, &m_oDcmConfig, sizeof(EC_T_DCM_CONFIG_LINKLAYERREFCLOCK));
    
    return EC_E_NOERROR;
}

EC_T_DWORD CEcDcmLinkLayerRefClock::DistributeSystemTimeProcess(EC_T_BOOL bStamped, EC_T_DWORD dwLocalTime, EC_T_DWORD dwLocalTime2)
{
    CEcDistributedClocks* poDC = m_poMaster->m_pDistributedClks;

    EC_UNREFPARM(bStamped); EC_UNREFPARM(dwLocalTime); EC_UNREFPARM(dwLocalTime2);

    if (poDC->m_bDCInitialized)
    {
        if (!m_bInSync)
        {
            m_bInSync = EC_TRUE;
            m_poMaster->NotifyDcmSync(m_bInSync, 0, 0, 0);
        }

        if (m_oDcmConfig.bLogEnabled)
        {
            EC_T_INT nLogMax = DCM_LOG_BUFFER_SIZE - (EC_T_INT)(m_pszLogNext - m_szLog);
            EC_T_INT nSystemTimeDifference = 0;
            EC_T_UINT64 qwBusTime = poDC->GetBusTime();

            if (EC_NULL != poDC->m_pBusSlaveSyncWindowMonitoringLast)
            {
                EC_T_DWORD dwSystemTimeDifference = poDC->m_pBusSlaveSyncWindowMonitoringLast->m_dwSystemTimeDifference;

                nSystemTimeDifference = (EC_T_INT)(dwSystemTimeDifference & ~0x80000000);
                if (0 != (dwSystemTimeDifference & 0x80000000))
                {
                    nSystemTimeDifference = -1 * nSystemTimeDifference;
                }
            }
            /* add a line break if current log isn´t empty */
            if ((m_pszLogNext != m_szLog) && (1 < nLogMax))
            {
                *m_pszLogNext = '\n';
                m_pszLogNext++;
                nLogMax--;
            }

            m_pszLogNext += OsSnprintf(m_pszLogNext, nLogMax, "%u; %ld; 0x%08X%08X; %lu; %d; %d",
                m_poMaster->m_dwMsecCounter,                   /* Time [ms]                */
                m_oDcmConfig.nCtlSetVal,                       /* SetVal [ns]              */
                EC_HIDWORD(qwBusTime), EC_LODWORD(qwBusTime),  /* BusTime [ns]             */
                EC_LODWORD(poDC->GetDcStartTime()),            /* DC StartTime             */
                poDC->IsInSync(),                              /* DC InSync                */
                nSystemTimeDifference);                        /* SystemTimeDifference [ns]*/
        }
    }
    return EC_E_NOERROR;
}

EC_T_UINT64 CEcDcmLinkLayerRefClock::DcStartTime(EC_T_UINT64 qwBusTime, EC_T_UINT64 qwCycleTime)
{
    EC_UNREFPARM(qwCycleTime);
    EC_T_UINT64 qwDcStartTime     = qwBusTime + GetCtlSetVal();
    EC_T_DWORD  dwDcStartTimeGrid = m_poMaster->m_pDistributedClks->GetDcStartTimeGrid();
    if (0 != dwDcStartTimeGrid)
    {
        qwDcStartTime -= qwDcStartTime % dwDcStartTimeGrid;
        qwDcStartTime += dwDcStartTimeGrid;
    }
    return qwDcStartTime;
}

/*-CEcDcmAppCallback---------------------------------------------------------*/
CEcMasterShiftByApp::CEcMasterShiftByApp(CEcMaster* poMaster) : CEcDcmBase(poMaster) 
{
    m_bInSync                = EC_FALSE;

    m_pfnDcStartTime         = EC_NULL;
    m_poDcStartTimeParm      = EC_NULL;

    m_pfnTimeStamp           = EC_NULL;
    m_pvTimeStampContext     = EC_NULL;
    m_dwTimeStampLastResult  = 0;
    m_dwCheckForStampReload  = 0;
    m_dwCheckForStampCounter = 0;

    OsMemset(&m_oActualValues, 0, sizeof(EC_T_ACTUALVAL));
}

/***************************************************************************************************/
/**
\brief  Registers start time callback function and parameters from application

\return EC_E_NOERROR
*/
EC_T_DWORD CEcMasterShiftByApp::RegisterStartTimeCallback(EC_T_DC_SYNCSO_REGDESC* pParms)
{
    m_pfnDcStartTime = pParms->pfnCallback;
    m_poDcStartTimeParm = pParms->pCallbackParm;

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Registers time stamp callback function and parameters from application.

\return EC_E_NOERROR on success, otherwise error code.
*/
EC_T_DWORD CEcMasterShiftByApp::RegisterTimeStampCallback(EC_T_REGISTER_TSPARMS* pParms, EC_T_REGISTER_TSRESULTS* pRes)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    /* if a timestamp function is already registered, respond INVALID State to let application unregister
    * "old" function first */
    if (EC_NULL != m_pfnTimeStamp)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* store timestamping registration information */
    m_pfnTimeStamp          = pParms->pfTimeStamp;
    m_pvTimeStampContext    = pParms->pCallerData;
    m_dwCheckForStampReload = pParms->dwUpdateMultiplier;

    /* store parameter response for application, containing pointer to stamped dataset and
    * information about Hard Realtime possibilities */
    pRes->pActualValues = &m_oActualValues;
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Unregisters start time callback function and parameters.

\return EC_E_NOERROR
*/
EC_T_DWORD CEcMasterShiftByApp::UnregisterTimeStampCallback(EC_T_VOID)
{
    m_pfnTimeStamp           = EC_NULL;
    m_pvTimeStampContext     = EC_NULL;
    m_dwCheckForStampReload  = 0;
    m_dwCheckForStampCounter = 0;

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Get time stamp callback function and parameters.

\return EC_E_NOERROR
*/
EC_T_DWORD CEcMasterShiftByApp::GetTimeStampCallback(
    EC_PF_TIMESTAMP* ppfnTimeStamp, 
    EC_T_PVOID*      ppvTimeStampContext,
    EC_T_DWORD**     ppdwTimeStampLast
    )
{
    if (EC_NULL != ppfnTimeStamp)
        *ppfnTimeStamp = m_pfnTimeStamp;

    if (EC_NULL != ppvTimeStampContext)
        *ppvTimeStampContext = m_pvTimeStampContext;

    if (EC_NULL != ppdwTimeStampLast)
        *ppdwTimeStampLast = &m_dwTimeStampLastResult;

    return EC_E_NOERROR;
}

EC_T_UINT64 CEcMasterShiftByApp::DcStartTime(EC_T_UINT64 qwBusTime, EC_T_UINT64 qwCycleTime)
{

    if (EC_NULL == m_pfnDcStartTime)
    {
        return 0;
    }
    m_poDcStartTimeParm->dwTimeStamp        = m_oActualValues.dwBusSyncFrameSendTimeLo;
    m_poDcStartTimeParm->dwPostTimeStamp    = m_oActualValues.dwBusSyncFrameSendTimeLo;
    m_poDcStartTimeParm->dwBusTimeLo        = EC_LODWORD(qwBusTime);
    m_poDcStartTimeParm->dwBusTimeHi        = EC_HIDWORD(qwBusTime);
    m_poDcStartTimeParm->dwSyncPeriodLength = EC_LODWORD(qwCycleTime);
    /* get start time from callback */
    m_poDcStartTimeParm->dwTimeStampResult  = m_pfnDcStartTime(m_poDcStartTimeParm);

    return EC_MAKEQWORD((m_poDcStartTimeParm->dwStartSyncTimeHi), (m_poDcStartTimeParm->dwStartSyncTimeLo));
}

/***************************************************************************************************/
/**
\brief  Check whether cyclic timestamping is required.

This function decrements counter to see wheter stamping is required. If stamping is required
timer is reloaded.
\return EC_TRUE if required, EC_FALSE if not.
*/
EC_T_BOOL CEcMasterShiftByApp::CheckForStamp(EC_T_VOID)
{
    EC_T_BOOL bDoStamp = EC_FALSE;
    /* check if timestamp is registered and not disabled */
    if ((EC_NULL == m_pfnTimeStamp) || (0 == m_dwCheckForStampReload))
    {
        goto Exit;
    }
    /* handle check for stamp counter */
    if (0 == m_dwCheckForStampCounter)
    {
        m_dwCheckForStampCounter = m_dwCheckForStampReload;
        bDoStamp = EC_TRUE;
    }
    else
    {
        m_dwCheckForStampCounter--;
    }
Exit:
    return bDoStamp;
}
/***************************************************************************************************/
/**
\brief  Called each cycle marked as measurement cycle, when processing ARMW response.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMasterShiftByApp::DistributeSystemTimeProcess(
    EC_T_BOOL   bStamped, 
    EC_T_DWORD  dwLocalTime,   /**< [in]   System Stamp value from m_poDcm->pfnTimeStamp Send */ 
    EC_T_DWORD  dwLocalTime2   /**< [in]   System Stamp value from m_poDcm->pfnTimeStamp Recv */
    )
{
    if (bStamped)
    {
        EC_T_UINT64 qwCycleTime = 0;
        EC_T_BOOL   bNotify = EC_FALSE;

        /* determine cycle time */
        qwCycleTime = m_poMaster->GetBusCycleTimeUsec() * 1000;

        if (0 != qwCycleTime)
        {
            EC_T_UINT64 qwBusTime     = m_poMaster->m_pDistributedClks->GetBusTime();
            EC_T_UINT64 qwDcStartTime = m_poMaster->m_pDistributedClks->GetDcStartTime();
            /* begin update, synchronization by counter variable and memory barrier */
            m_oActualValues.dwBeginUpdateCnt++;
            OsMemoryBarrier();

            /* store actual values, required for DCM */
            m_oActualValues.bSlavesInSync = m_poMaster->m_pDistributedClks->IsInSync();

            m_oActualValues.dwBusTimeHi = EC_HIDWORD(qwBusTime);
            m_oActualValues.dwBusTimeLo = EC_LODWORD(qwBusTime);

            m_oActualValues.dwBusSyncFrameSendTimeLo = dwLocalTime;
            m_oActualValues.dwBusSyncFramePostSendTimeLo = dwLocalTime2;

            m_oActualValues.dwSyncPulseGridOffsetHi = EC_HIDWORD(qwDcStartTime);
            m_oActualValues.dwSyncPulseGridOffsetLo = EC_LODWORD(qwDcStartTime);
            m_oActualValues.dwSyncPeriodLength = EC_LODWORD(qwCycleTime);

            m_oActualValues.dwLastTimeStampResult = m_dwTimeStampLastResult;

            if (m_bInSync != m_oActualValues.bDcmCtlInSync)
            {
                m_bInSync = m_oActualValues.bDcmCtlInSync;
                bNotify = EC_TRUE;
            }
            /* end update, synchronization by counter variable and memory barrier */
            OsMemoryBarrier();
            m_oActualValues.dwEndUpdateCnt++;

#if (defined INCLUDE_MASTER_OBD)
            m_poMaster->m_pSDOService->EntrySet_SetDCMInSync(m_oActualValues.bDcmCtlInSync);
#endif
        }
        else
        {
            OsDbgAssert(EC_FALSE);
        }
        /* handle notification */
        if (bNotify)
        {
            m_poMaster->NotifyDcmSync(m_bInSync, 0, 0, 0);
        }
    }
    return EC_E_NOERROR;
}

#if (defined INCLUDE_DCX)

CEcDcx::CEcDcx(CEcMaster* poMaster) : CEcDcmBusShift(poMaster), m_oDcmMasterShift(poMaster)
{
    /* replace DcmInSync with DcxInSync */
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcDcx::~m_poInSync", m_poInSync, sizeof(CEcDcmInSync));
    SafeDelete(m_poInSync);
    
    m_poInSync = EC_NEW(CEcDcxInSync(poMaster, this));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcDcx::~m_poInSync", m_poInSync, sizeof(CEcDcxInSync));

    m_poExtClockSlave   = EC_NULL;
    m_dwExtClockTimeout = 1000;
    m_bNoExtClock = EC_TRUE;

    m_bSystemTimeDistributionDetected = EC_FALSE;
    m_qwIntTimeStamp = 0;
    m_qwExtTimeStamp = 0;
    m_nTimeStampDiff = 0;
    m_nTimeStampDiffFirst = 0;
    m_bTimeStampDiffFirstMissing = EC_TRUE;
    m_bTimeStampUpdateToggleOld  = EC_FALSE;
    m_dwTimeStampUpdateWatchdog  = DCX_TIME_STAMP_UPDATE_WATCHDOG;
    
    m_pExtSyncStatusPdoTimeStampUpdate = EC_NULL;
    m_pExtSyncStatusPdoNoExtDevice     = EC_NULL;
    m_pExtSyncStatusPdoIntTimeStamp    = EC_NULL;
    m_pExtSyncStatusPdoExtTimeStamp    = EC_NULL;

    m_nAdjustWaitCycles = EC_MAX(1, 1000 / poMaster->GetBusCycleTimeUsec());

    /* initialize logging */
    {
        EC_T_CHAR* pszMasterShiftLog = EC_NULL;

        /* get log header from mastershift */
        m_oDcmMasterShift.GetLog(&pszMasterShiftLog);

        /* append Dcx specific header */
        m_pszLogNext = m_szLog;
        m_pszLogNext += OsSnprintf(m_szLog, DCM_LOG_BUFFER_SIZE,
            "%s;IntTimeStamp[ns];ExtTimeStamp[ns]; TimeStampDiff[ns];SetVal(X) [ns];ActVal(X) [ns];CtlAdj(X);CtlErr(X) [ns];CtlErrFilt(X);Drift(X) [ppm];CtlErr(X) [1/10 pmil];CtlOutSum(X);CtlOutTot(X) [pmil];NoExtDev;"
            "DCX ErrorCode;DCX InSync"
            , pszMasterShiftLog
            );
    }
}

/***************************************************************************************************/
/**
\brief  Set external clock slave. 
        External sync status pdo is required in slave configuration.        

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcx::SetExtClock(
    EC_T_WORD wFixedAddress     /**<[in]  Fixed address of external clock slave */
    )
{
#if (defined INCLUDE_VARREAD)
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_WORD  wIdx = 0;
    EC_T_DWORD dwProcVarFoundCnt = 0;
    EC_T_PD_VAR_INFO_INTERN* pProcVarInfo = EC_NULL;

    if (0 == wFixedAddress)
    {
        dwRetVal = EC_E_FEATURE_DISABLED;
        goto Exit;
    }

    /* find external synchronization status pdo of the external clock slave */
    for (wIdx = 0; wIdx < m_poMaster->m_dwProcVarInfoNumEntriesInp; wIdx++)
    {
        pProcVarInfo = m_poMaster->m_apProcVarInfoEntriesInp[wIdx];
        /* skip pdo´s from other slaves */
        if (pProcVarInfo->wFixedAddr != wFixedAddress)
            continue;

        if (DCX_EXTSYNCSTATUS_PDO_IDX == pProcVarInfo->wIndex)
        {
            switch (pProcVarInfo->wSubIndex)
            {
            case DCX_EXTSYNCSTATUS_PDO_SUBIDX_TIME_STAMP_UPD:
                OsDbgAssert(1 == pProcVarInfo->nBitSize);
                m_pExtSyncStatusPdoTimeStampUpdate = pProcVarInfo;
                dwProcVarFoundCnt++;
                break;
            case DCX_EXTSYNCSTATUS_PDO_SUBIDX_NO_EXT_DEV:
                OsDbgAssert(1 == pProcVarInfo->nBitSize);
                m_pExtSyncStatusPdoNoExtDevice = pProcVarInfo;
                dwProcVarFoundCnt++;
                break;
            case DCX_EXTSYNCSTATUS_PDO_SUBIDX_INT_TIME_STAMP:
                OsDbgAssert(64 == pProcVarInfo->nBitSize);
                m_pExtSyncStatusPdoIntTimeStamp = pProcVarInfo;
                dwProcVarFoundCnt++;
                break;
            case DCX_EXTSYNCSTATUS_PDO_SUBIDX_EXT_TIME_STAMP:
                OsDbgAssert(64 == pProcVarInfo->nBitSize);
                m_pExtSyncStatusPdoExtTimeStamp = pProcVarInfo;
                dwProcVarFoundCnt++;
                break;
            default:
                break;
            }

            /* all process variables required for DCX found */
            if (DCX_EXTSYNCSTATUS_PDO_PROC_VAR_CNT == dwProcVarFoundCnt)
            {
                break;
            }
        }
    }
    if (DCX_EXTSYNCSTATUS_PDO_PROC_VAR_CNT != dwProcVarFoundCnt)
    {
        EC_ERRORMSG(("CEcDcx::SetExtClock(): External Sync Status PDO not found. Slave %d\n", wFixedAddress));
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    m_poExtClockSlave = m_poMaster->GetSlaveByCfgFixedAddr(wFixedAddress);
    if (EC_NULL == m_poExtClockSlave)
    {
        EC_ERRORMSG(("CEcDcx::SetExtClock(): External Sync Slave %d not found\n", wFixedAddress));
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* Execute DC initialization for bridge slaves even if there is no DC configuration for them in the ENI file */
    m_poExtClockSlave->SetDcInitializationRequired(EC_TRUE);
    
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(wFixedAddress);
    EC_ERRORMSG(("CEcDcx::SetExtClock(): DCX not supported, because INCLUDE_VARREAD not defined\n"));
    return EC_E_NOTSUPPORTED;
#endif /* INCLUDE_VARREAD */
}

/***************************************************************************************************/
/**
\brief  Distribute system time of DCM and DCX with InSync handling. Called each cycle marked as measurement cycle, when processing ARMW response.
        DCM: Mastershift controller 
        DCX: Busshift controller with external clock slave as time reference.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcx::DistributeSystemTimeProcess(EC_T_BOOL bStamped, EC_T_DWORD  dwLocalTime, EC_T_DWORD dwLocalTime2)
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_poMaster->m_MasterConfig.dwStateChangeDebug >> 4) & 3);
#endif
    EC_T_DWORD dwRes = EC_E_NOERROR;
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    CEcDistributedClocks* poDC = m_poMaster->m_pDistributedClks;

    /* Mastershift */
    m_oDcmMasterShift.DistributeSystemTimeProcess(bStamped, dwLocalTime, dwLocalTime2); 

    /* Busshift uses bridge device as external sync source */
    if ((EC_NULL != poDC->m_pBusSlaveRefClock) && (EC_NULL != m_poExtClockSlave) && poDC->m_bDCInitialized)
    {
        EC_T_BOOL   bNoExtClockFound  = EC_FALSE;
        EC_T_INT64  nTimeStampError   = 0;
        EC_T_BYTE*  pbyPDIn           = m_poMaster->GetPDInBasePointer();
        EC_T_UINT64 qwIntTimeStampOld = m_qwIntTimeStamp;
        EC_T_UINT64 qwExtTimeStampOld = m_qwExtTimeStamp;
        EC_T_BOOL   bNoExtDevConnected = EC_FALSE;
        EC_T_BOOL   bTimeStampUpdateToggle = EC_FALSE;

        /* DCX will only work in SAFEOP or OP */
        if (!m_bSystemTimeDistributionDetected && (DEVICE_STATE_SAFEOP <= m_poMaster->GetMasterReqDeviceState()))
        {
            m_bSystemTimeDistributionDetected = EC_TRUE;
            m_oNoExtClockTimer.Start(m_dwExtClockTimeout, m_poMaster->GetMsecCounterPtr());
        }

        /* read external sync status register from external clock */
        m_qwIntTimeStamp = EC_GET_FRM_QWORD(pbyPDIn + (m_pExtSyncStatusPdoIntTimeStamp->nBitOffs / 8));
        m_qwExtTimeStamp = EC_GET_FRM_QWORD(pbyPDIn + (m_pExtSyncStatusPdoExtTimeStamp->nBitOffs / 8));
        bNoExtDevConnected     = EC_TESTBIT(pbyPDIn, m_pExtSyncStatusPdoNoExtDevice->nBitOffs);
        bTimeStampUpdateToggle = EC_TESTBIT(pbyPDIn, m_pExtSyncStatusPdoTimeStampUpdate->nBitOffs);
        
        /* check for valid timestamps */
        if((0 == m_qwIntTimeStamp) && (0 == m_qwExtTimeStamp))
        {
            bNoExtClockFound = EC_TRUE;
            if (!m_bNoExtClock || m_oNoExtClockTimer.IsElapsed())
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCX Error: TimeStamps invalid\n"));
            }
        }
        /* check for changing timestamps */
        if ((qwIntTimeStampOld == m_qwIntTimeStamp) && (qwExtTimeStampOld == m_qwExtTimeStamp))
        {
            bNoExtClockFound = EC_TRUE;
            if (!m_bNoExtClock || m_oNoExtClockTimer.IsElapsed())
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCX Error: TimeStamps not refreshed\n"));
            }
        }
        /* check for external device connected */
        if (bNoExtDevConnected)
        {
            bNoExtClockFound = EC_TRUE;
            if (!m_bNoExtClock || m_oNoExtClockTimer.IsElapsed())
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCX Error: External device not connected\n"));
            }
        }
        /* check toggle bit (EL6692 toggles bit only in OP) */
        if ((m_bTimeStampUpdateToggleOld == bTimeStampUpdateToggle) &&
            ((DCX_EL6692_PRODUCT_CODE != m_poExtClockSlave->GetProductCode()) || DEVICE_STATE_OP == m_poExtClockSlave->GetDeviceState()))
        {
            if (0 == m_dwTimeStampUpdateWatchdog)
            {
                bNoExtClockFound = EC_TRUE;
                if (!m_bNoExtClock || m_oNoExtClockTimer.IsElapsed())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCX Error: TimeStamp update watchdog violation\n"));
                }
            }
            else
            {
                m_dwTimeStampUpdateWatchdog--;
            }
        }
        else
        {
            m_bTimeStampUpdateToggleOld = bTimeStampUpdateToggle;
            m_dwTimeStampUpdateWatchdog = DCX_TIME_STAMP_UPDATE_WATCHDOG;
        }
        /* detect external clock disconnection */
        if (bNoExtClockFound)
        {
            if (!m_bNoExtClock || m_oNoExtClockTimer.IsElapsed())
            {
                m_bNoExtClock = EC_TRUE;
                m_oNoExtClockTimer.Stop();

                if ((DEVICE_STATE_SAFEOP <= m_poMaster->GetMasterReqDeviceState()))
                {
                    m_poInSync->NotifyInSync(EC_FALSE, m_oCtlparam.nCtlErrorNsec, DCX_E_NO_EXT_CLOCK);
                }
            }
            /* skip controller if external clock not available */
            dwRetVal = DCX_E_NO_EXT_CLOCK;
            goto Exit;
        }
        /* At this point external clock device is working correctly */
        if (m_bNoExtClock)
        {
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCX: External device detected\n"));
        }
        m_bNoExtClock = EC_FALSE;
        m_oNoExtClockTimer.Stop();

        /* calculate timestamp difference */
        if ((EC_HIDWORD(m_qwExtTimeStamp) == 0) && (EC_HIDWORD(m_qwIntTimeStamp) == 0))
        {
            /* support 32Bit counters */
            m_nTimeStampDiff = (EC_T_INT64)((EC_T_INT)(EC_LODWORD(m_qwExtTimeStamp) - EC_LODWORD(m_qwIntTimeStamp)));
        }
        else
        {
            m_nTimeStampDiff = (EC_T_INT64)(m_qwExtTimeStamp - m_qwIntTimeStamp);
        }
        if (m_bTimeStampDiffFirstMissing) /* 0 is a valid TimeStampDiffFirst value */
        {
            EC_T_DWORD dwDcStartTimeGrid = m_poMaster->m_pDistributedClks->GetDcStartTimeGrid();

            if (0 != dwDcStartTimeGrid)
            {
                EC_T_UINT64 qwExtTimeStamp = m_qwExtTimeStamp - (m_qwExtTimeStamp % dwDcStartTimeGrid);
                EC_T_UINT64 qwIntTimeStamp = m_qwIntTimeStamp - (m_qwIntTimeStamp % dwDcStartTimeGrid);

                /* calculate timestamp difference */
                if ((EC_HIDWORD(qwExtTimeStamp) == 0) && (EC_HIDWORD(qwIntTimeStamp) == 0))
                {
                    /* support 32Bit counters */
                    m_nTimeStampDiffFirst = (EC_T_INT64)((EC_T_INT)(EC_LODWORD(qwExtTimeStamp) - EC_LODWORD(qwIntTimeStamp)));
                }
                else
                {
                    m_nTimeStampDiffFirst = (EC_T_INT64)(qwExtTimeStamp - qwIntTimeStamp);
                }
            }
            else
            {
                m_nTimeStampDiffFirst = m_nTimeStampDiff;
            }
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCX TimeStamp difference at startup = 0x%08X%08X\n",
                EC_HIDWORD(m_nTimeStampDiff), EC_LODWORD(m_nTimeStampDiff)));
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCX TimeStamp difference normalized = 0x%08X%08X\n",
                EC_HIDWORD(m_nTimeStampDiffFirst), EC_LODWORD(m_nTimeStampDiffFirst)));

            m_bTimeStampDiffFirstMissing = EC_FALSE;

            goto Exit;
        }
        /* calculate error between time stamps */
        nTimeStampError = m_nTimeStampDiff - m_nTimeStampDiffFirst;

        m_oController.ProcessCtlError((EC_T_INT)nTimeStampError);

        /* adjust reference clock */
        if (m_oCtlparam.bReqAdjustment)
        {
            AdjustDcShiftTime();
            m_oCtlparam.bReqAdjustment = EC_FALSE;
        }
        /* InSync monitoring */
        dwRes = m_poInSync->NotifyCtlError(m_oCtlparam.nCtlErrorNsec);
        /* Update controller status */
        if (EC_E_BUSY != dwRes)
            m_oController.SetStatusCode(dwRes);
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if (m_oDcmConfig.bLogEnabled && poDC->m_bDCInitialized)
    {
        this->UpdateLog();
    }
    return dwRetVal;
}
/***************************************************************************************************/
/**
\brief  Set status of DCM and DCX controller

\return 
*/
EC_T_VOID CEcDcx::SetStatus(EC_T_DWORD dwStatus)
{
    CEcDcmBusShift::SetStatus(dwStatus);
    m_oDcmMasterShift.SetStatus(dwStatus);
}

/***************************************************************************************************/
/**
\brief  Get status of DCX controller

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcx::GetDcxStatus(
    EC_T_DWORD* pdwErrorCode,
    EC_T_INT*   pnDiffCur,
    EC_T_INT*   pnDiffAvg,
    EC_T_INT*   pnDiffMax,
    EC_T_INT64* pnTimeStampDiff
    )
{
    if (m_bNoExtClock)
    {
        *pdwErrorCode = DCX_E_NO_EXT_CLOCK;
    }
    else
    {
        *pdwErrorCode = m_oController.GetStatusCode();
    }

    *pnDiffCur = m_oCtlparam.nCtlErrorNsec;
    *pnDiffAvg = m_poInSync->GetCtlErrorAvg();
    *pnDiffMax = m_poInSync->GetCtlErrorMax();
    *pnTimeStampDiff = m_nTimeStampDiff;

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Reset status of DCM and DCX controller

\return EC_E_NOERROR
*/
EC_T_DWORD CEcDcx::ResetStatus(EC_T_VOID)
{
    CEcDcmBusShift::ResetStatus();
    m_oDcmMasterShift.ResetStatus();
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Print status of DCM and DCX controller to console

\return EC_E_NOERROR
*/
EC_T_DWORD CEcDcx::ShowStatus(EC_T_VOID)
{
    EC_DBGMSG("**********************************************************************\n");
    EC_DBGMSG("DCM Busshift Controller:\n"); 
    CEcDcmBusShift::ShowStatus();
    EC_DBGMSG("**********************************************************************\n");
    EC_DBGMSG("DCM Mastershift Controller:\n");
    return m_oDcmMasterShift.ShowStatus();
};

/***************************************************************************************************/
/**
\brief  Configure DCM and DCX controller and InSync monitoring

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD  CEcDcx::Configure(EC_T_DCM_CONFIG* pDcmConfig)
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_poMaster->m_MasterConfig.dwStateChangeDebug >> 4) & 3);
#endif
    EC_T_DWORD dwRes = EC_E_NOERROR;
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    
    if (m_poMaster->IsConfigLoaded() && !m_poMaster->m_pDistributedClks->DcmGetBusShiftConfigured())
    {
        dwRetVal = EC_E_FEATURE_DISABLED;
        goto Exit;
    }

    m_oDcmConfig.nCtlSetVal = pDcmConfig->u.Dcx.nCtlSetVal;
    m_oDcmConfig.nCtlGain = pDcmConfig->u.Dcx.nCtlGain;
    m_oDcmConfig.nCtlDriftErrorGain = pDcmConfig->u.Dcx.nCtlDriftErrorGain;
    m_oDcmConfig.nMaxValidVal = pDcmConfig->u.Dcx.nMaxValidVal;
    m_oDcmConfig.bLogEnabled = pDcmConfig->u.Dcx.bLogEnabled;
    m_oDcmConfig.dwInSyncLimit = pDcmConfig->u.Dcx.dwInSyncLimit;
    m_oDcmConfig.dwInSyncSettleTime = pDcmConfig->u.Dcx.dwInSyncSettleTime;
    m_oDcmConfig.bCtlOff = pDcmConfig->u.Dcx.bCtlOff;
    m_dwExtClockTimeout = pDcmConfig->u.Dcx.dwExtClockTimeout;
    
    /* Mastershift */
    {
        EC_T_DCM_CONFIG oMasterShiftConfig;
        OsMemset(&oMasterShiftConfig, 0, sizeof(EC_T_DCM_CONFIG));
        OsMemcpy(&oMasterShiftConfig.u.MasterShift, &pDcmConfig->u.Dcx.MasterShift, sizeof(EC_T_DCM_CONFIG_MASTERSHIFT));
        oMasterShiftConfig.u.MasterShift.bLogEnabled = m_oDcmConfig.bLogEnabled;

        dwRes = m_oDcmMasterShift.Configure(&oMasterShiftConfig);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }
    }

    /* Busshift */
    {
        dwRes = m_oController.Init(&m_oCtlparam);
        if (EC_E_NOERROR != dwRes) { goto Exit; }

        dwRes = m_oController.Configure(&m_oDcmConfig);
        if (EC_E_NOERROR != dwRes) { goto Exit; }
    }
    /* configure Dcx InSync monitoring */
    {
        /* 5 % limit in NSEC for InSync monitoring as default */
        EC_T_DWORD dwInSyncLimit = m_poMaster->GetBusCycleTimeUsec() * (1000 / 20);
        if (0 != pDcmConfig->u.Dcx.dwInSyncLimit)
            dwInSyncLimit = pDcmConfig->u.Dcx.dwInSyncLimit;

        m_poInSync->Configure(dwInSyncLimit, pDcmConfig->u.Dcx.dwInSyncSettleTime, DCM_IN_SYNC_START_DELAY_CYCLES);
    }
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DcmConfigure()\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "DCM Mode                = eDcmMode_Dcx\n"));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Ext clock fixed address = %d\n", ((EC_NULL == m_poExtClockSlave) ? INVALID_FIXED_ADDR : m_poExtClockSlave->GetFixedAddr())));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Ext clock timeout       = %d\n", m_dwExtClockTimeout));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "InSync limit            = %d\n", m_poInSync->GetInSyncLimit()));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "InSync settle time      = %d\n", m_poInSync->GetInSyncSettleTime()));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Controller SetVal       = %d\n", m_oController.GetCtlSetVal()));
	EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Controller gain         = %d\n", m_oCtlparam.nGain));
	EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Drift error gain        = %d\n", m_oCtlparam.nDriftErrorGain));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Max valid value         = %d\n", m_oCtlparam.nMaxValidVal));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Controller off          = %d\n", m_oDcmConfig.bCtlOff));
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + DCAPROVAL), "Logging enabled         = %d\n", m_oDcmConfig.bLogEnabled));

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get configuration of DCM, DCX controller and InSync monitoring

\return EC_E_NOERROR
*/
EC_T_DWORD CEcDcx::GetConfig(EC_T_DCM_CONFIG* pDcmConfig)
{
    /* mastershift */
    EC_T_DCM_CONFIG oMasterShiftConfig;
    m_oDcmMasterShift.GetConfig(&oMasterShiftConfig);
    OsMemcpy(&pDcmConfig->u.Dcx.MasterShift, &oMasterShiftConfig.u.MasterShift, sizeof(EC_T_DCM_CONFIG_MASTERSHIFT));
    
    /* Dcx busshift */
    pDcmConfig->eMode = eDcmMode_Dcx;
    pDcmConfig->u.Dcx.nCtlSetVal         = m_oController.GetCtlSetVal();
    pDcmConfig->u.Dcx.nCtlGain           = m_oCtlparam.nGain;
    pDcmConfig->u.Dcx.nCtlDriftErrorGain = m_oCtlparam.nDriftErrorGain;
    pDcmConfig->u.Dcx.nMaxValidVal       = m_oCtlparam.nMaxValidVal;
    pDcmConfig->u.Dcx.bLogEnabled        = m_oDcmConfig.bLogEnabled;
    pDcmConfig->u.Dcx.dwInSyncLimit      = m_poInSync->GetInSyncLimit();
    pDcmConfig->u.Dcx.dwInSyncSettleTime = m_poInSync->GetInSyncSettleTime();
    pDcmConfig->u.Dcx.bCtlOff            = m_oDcmConfig.bCtlOff;
    pDcmConfig->u.Dcx.wExtClockFixedAddr = ((EC_NULL == m_poExtClockSlave) ? INVALID_FIXED_ADDR : m_poExtClockSlave->GetFixedAddr());
    pDcmConfig->u.Dcx.dwExtClockTimeout  = m_dwExtClockTimeout;

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Reset DCX controller and InSync monitoring

\return
*/
EC_T_VOID CEcDcx::DcxReset(EC_T_VOID)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_poMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "DcxReset()\n"));

    CEcDcmBusShift::Reset();
    m_bSystemTimeDistributionDetected = EC_FALSE;
    m_bTimeStampDiffFirstMissing = EC_TRUE;
    m_dwTimeStampUpdateWatchdog = DCX_TIME_STAMP_UPDATE_WATCHDOG;
    m_bNoExtClock = EC_TRUE;
}


/***************************************************************************************************/
/**
\brief  Reset DCM and DCX

\return
*/
EC_T_VOID CEcDcx::Reset(EC_T_VOID)
{
    this->DcxReset();
    m_oDcmMasterShift.Reset();
}

/***************************************************************************************************/
/**
\brief  Get timer adjust value of DCM Mastershift controller

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcx::GetAdjust(EC_T_INT* pnAdjustPermil)
{
    return m_oDcmMasterShift.GetAdjust(pnAdjustPermil); /* adjust value makes only sense in mastershift mode */
}

/***************************************************************************************************/
/**
\brief  Update DCM and DCX logging

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcDcx::UpdateLog(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwMaxLogSize = (EC_T_DWORD)(DCM_LOG_BUFFER_SIZE - (m_pszLogNext - m_szLog));
    
    /* copy/refresh current logging data set */
    m_oDcmMasterShift.GetLog(&m_oLog);

    /* print logging string */
    if (1 < dwMaxLogSize)
    {
        /* get log from Mastershift */
        EC_T_CHAR* szDcmMasterShiftLog = EC_NULL;
        dwRetVal = m_oDcmMasterShift.GetLog(&szDcmMasterShiftLog);
        if (EC_E_NOERROR != dwRetVal) { goto Exit; }
        /*
        ";IntTimeStamp[ns];ExtTimeStamp[ns];SetVal(X) [ns];ActVal(X) [ns];CtlAdj(X);CtlErr(X) [ns];CtlErrFilt(X);Drift(X) [ppm];CtlErr(X) [1/10 pmil];CtlOutSum(X);CtlOutTot(X) [pmil];NoExtDev;"
        "DCX ErrorCode;DCX InSync" 
        */
        m_pszLogNext += OsSnprintf(m_pszLogNext, dwMaxLogSize,
            "%s; 0x%08X%08X; 0x%08X%08X; %ld; %ld; %ld; %ld; %d; %d; %d; %d; %ld; %ld; %d; %lu; %ld",
            szDcmMasterShiftLog,                                            /* Mastershift Dcm log  */
            EC_HIDWORD(m_qwIntTimeStamp), EC_LODWORD(m_qwIntTimeStamp),     /* Internal time stamp  */
            EC_HIDWORD(m_qwExtTimeStamp), EC_LODWORD(m_qwExtTimeStamp),     /* External time stamp  */
            EC_LODWORD(m_nTimeStampDiff),                       /* Current difference between external and internal timestamp */
            m_oController.GetCtlSetVal(),                       /* SetVal [ns]          */
            m_oController.GetCtlActVal(),                       /* ActVal [ns]          */
			m_poMaster->m_pDistributedClks->m_oDcShiftConfig.nShiftTime, /* CtlAdj      */
            m_oCtlparam.nCtlErrorNsec,                          /* CtlErr [nsec]        */
            m_oCtlparam.nCtlErrorFilt,                          /* CtlErrFilt           */
            m_oCtlparam.nCurrDrift / DRIFT_CALC_STEP_WIDTH,     /* Drift [ppm]          */
            m_oCtlparam.nCtlErrorPmil,                          /* CtlErr [1/10 pmil]   */
            m_oCtlparam.nCtlOutputSumComp,                      /* CtlOutSum            */
            (EC_T_INT)m_oCtlparam.nCtlOutputTotal,              /* CtlOutTot [pmil]     */
            m_bNoExtClock,                                      /* External device not connected to bridge device */
            m_oController.GetStatusCode(),                      /* DCX ErrorCode        */
            m_poInSync->IsInSync()                              /* DCX InSync           */
            );
        dwRetVal = EC_E_NOERROR;
    }
    else
    {
        dwRetVal = EC_E_INVALIDSIZE;
    }

Exit:
    return dwRetVal;
}
#endif

#endif /* INCLUDE_DC_SUPPORT */

/*-END OF SOURCE FILE--------------------------------------------------------*/
