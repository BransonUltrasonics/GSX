/*-----------------------------------------------------------------------------
 * EcDcBusShift.h           Header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Holger Oelhaf
 * Description              Master synchronization (DCM)
 *---------------------------------------------------------------------------*/

#ifndef INC_ECDCBUSSHIFT
#define INC_ECDCBUSSHIFT   1

/*-INCLUDES------------------------------------------------------------------*/
#include "AtEthercat.h"
#include "EcTimer.h"

/*-DEFINES-------------------------------------------------------------------*/

#define CONTROLLER_JOB_RESET                    1    /* reset controller */
#define CONTROLLER_JOB_CTL                      2    /* default controller loop */
#define CONTROLLER_JOB_NEW_SETVAL               3    /* new set value set by application */

#define DRIFT_CALC_STEP_WIDTH                  10

#define DCM_MAX_TIMER_DRIFT                  1000    /* max. drift between local timer and DC reference clock */

#if defined EC_VERSION_WINDOWS
#define P_GAIN                                  1    /* proportional gain in ppt (part per thousand)*/
#define DRIFT_ERROR_GAIN                        3    /* multiplier for drift error */
#define ERROR_INPUT_VALIDATION_MAX_NSEC      20000   /* error inputs above this value are considered invalid.
* if error input prediction is valid then the difference between the error input and the expected value
* is taken.
*/
#else
#define P_GAIN                                  2    /* proportional gain in ppt (part per thousand)*/
#define DRIFT_ERROR_GAIN                        3    /* multiplier for drift error */
#define ERROR_INPUT_VALIDATION_MAX_NSEC      5000
#endif

#define DCM_MAX_CONTROLLER_SET_PMIL           100     /* max. controller output */

/*-TYPEDEFS------------------------------------------------------------------*/

typedef EC_T_UINT   EC_T_UCALCVAL;
typedef EC_T_INT    EC_T_CALCVAL;

typedef enum _E_CTL_STATUS
{
    eDcBsCtlNotInitialized                   =  0,   /* controller not yet initialized */
//  eDcBsCtlSyncNotRunning                   =  1,   /* synchronization not yet running */
//  eDcBsCtlMasterStateLowerThanPreop        =  2,   /* Master state below PREOP (INIT) */
//  eDcBsCtlNoNewTimestampValue              =  3,   /* Controller was called, but no new timestamp value is available */
//  eDcBsCtlTimestampBusy                    =  4,   /* Controller was called while timestamping was busy */
//  eDcBsCtlSlavesNotInSync                  =  5,   /* Slaves are not in SYNC */
    eDcBsCtlBusTimeIsZero                    =  6,   /* EtherCAT Bus Time is 0 */
//  eDcBsCtlGridOffsetIsZero                 =  7,   /* Grid Offset is 0 */
//  eDcBsCtlBusTimeBelowGridOffset           =  8,   /* EtherCAT Bus Time is still below Grid Offset */
    eDcBsCtlBusTimeCountsBackwards           =  9,   /* EtherCAT Bus Time counts backwards (current time is below last time) */
    eDcBsCtlBusTimeDiffTooLow                = 10,   /* Change of EtherCAT Bus Time less than 50% of DCM controller cycle */
    eDcBsCtlBusTimeDiffTooHigh               = 11,   /* Change of EtherCAT Bus Time higher than 150% of DCM controller cycle */
//  eDcBsCtlSendTimeStampDiffTooHigh         = 12,   /* Timestamp difference of frame send time is too high (>50% of average) */
//  eDcBsCtlSyncPeriodLengthIsZero           = 13,   /* DC Sync Period length is 0 */
    eDcBsCtlControllerFailure                = 14,   /* DCM PI Controller failure */
    eDcBsCtlControllerOk                     = 15,   /* DCM PI Controller successful call */
//  eDcBsCtlModifyFailure                    = 16,   /* DCM timer/bus modification failure */
//  eDcBsCtlModifyOk                         = 17,   /* DCM timer/bus modification successful call */
    eDcBsCtlMaxCtlErrorExceeded              = 18,   /* DCM maximum controller error exceeded */
    eDcBsCtlOk                               = 19,   /* DCM job call success */
//  eDcBsCtlControllerNotEnabled             = 20,   /* DCM controller not enabled */
    eDcBsCtlResetRequest                     = 21,   /* DCM controller reset request */
    eDcBsCtlMissingCtlErrorLimit             = 22,   /* DCM controller error limit is missing */
    eDcBsCtlDriftTooHigh                     = 23,   /* DCM drift between host timer and reference clock is too high */
    eDcBsCtlBusCycleTimeIsZero               = 24    /* Master config parameter dwBusCycleTimeUsec is 0 */
#define NUM_CTL_STATUS                         25
} E_DCBS_CTL_STATUS;


typedef enum _E_CONTROLLER_STATE
{
    eCtlStateReset,             /* reset state */
    eCtlStateNewOutput          /* determine new controller output value for timer adjustment */
} E_CONTROLLER_STATE;


typedef struct
{
    EC_T_INT        nJob;                           /* [in]  what to do */
    EC_T_DWORD      dwBusCycleTimeNsec;             /* [in]  bus cycle time in nsec */
    EC_T_INT        nGain;                          /* [in]  proportional gain in ppt (part per thousand) */
    EC_T_INT        nDriftErrorGain;                /* [in]  multiplier for drift error */
    EC_T_INT        nMaxValidVal;                   /* [in]  error inputs above this value are considered invalid */

    EC_T_INT        nCtlOutputTotal;                /* [out] total controller output in ppm */
    EC_T_INT        nCtlOutputSumComp;              /* [out] summary controller output */
    EC_T_INT        nCtlErrorIntegralNsec;          /* [out] controller error integral in nanoseconds */

    EC_T_INT        nCtlErrorNsec;                  /* [out] controller error in nanoseconds */
    EC_T_INT        nCtlErrorFilt;                  /* [out] filtered error value */
    EC_T_BOOL       bInitFilter;                    
    EC_T_INT        nCurrDrift;                     /* [out] current drift */
    EC_T_INT        nErrCorrDrift;
    EC_T_INT        nSystemDrift;                   /* [out] system drift without controller */

    EC_T_INT        nCtlErrorPmil;                  /* [out] controller error [1/10 pmil] */
    E_CONTROLLER_STATE  ECtlState;                  /* controller state */

    EC_T_DWORD      dwSkipCounter;                  /* number of errors which are skipped */

    EC_T_INT        naCtlErrFlt[DRIFT_CALC_STEP_WIDTH+1];   /* filtered contr. error storage for drift calc */
    EC_T_INT        nCtlErrIdx;
    EC_T_INT        nCtlErrFltIdx;
    EC_T_DWORD      dwDriftCounter;

    EC_T_BOOL       bCalcDrift;                     /* enable drift calculation. needed for bus shift */
    EC_T_BOOL       bIncrementOutput;               /* enable increment controller outputs. needed for bus shift */
    EC_T_BOOL       bBuildTotalCtlErr;              /* enable total controller error calculation. needed for master shift */

    EC_T_INT        nTimerAdjustWaitTickCounter;
    EC_T_INT        nTimerAdjust;

    EC_T_BOOL       bReqAdjustment;                 /* [out] Request Dcm adjustment */
} DCBS_CTL_PARAM;


/*-CLASS---------------------------------------------------------------------*/

class CEcMaster;
class CEcDistributedClocks;
class CEcDcmBase;
class CEcDcx;

class CEcDcmInSync
{
public:
    CEcDcmInSync(CEcMaster* poMaster);
    virtual ~CEcDcmInSync() { }

    EC_T_BOOL   IsInSync() { return m_bInSync; }
    EC_T_DWORD  Configure(EC_T_DWORD dwInSyncLimit, EC_T_DWORD dwInSyncSettleTime, EC_T_DWORD dwMonitoringStartDelayCycles = 0)
                {
                    m_dwMonitoringStartDelayCycles = dwMonitoringStartDelayCycles;
                    if (0 != dwInSyncLimit) { m_dwInSyncLimit = dwInSyncLimit; }
                    if (0 != dwInSyncSettleTime) { m_dwInSyncSettleTime = dwInSyncSettleTime; }
                    return EC_E_NOERROR; 
                }
    EC_T_DWORD  GetInSyncLimit(EC_T_VOID) { return m_dwInSyncLimit; }
    EC_T_DWORD  GetInSyncSettleTime(EC_T_VOID) { return m_dwInSyncSettleTime; }

    EC_T_VOID   Reset(EC_T_VOID);
    
    EC_T_DWORD  NotifyCtlError(EC_T_INT nCtlErrorNsec);
    EC_T_VOID   ResetCtlErrorStatus(EC_T_VOID)
                { m_nCtlErrorNsecAvg = 0; m_nCtlErrorNsecMax = 0; }
    EC_T_INT    GetCtlErrorAvg(EC_T_VOID) { return m_nCtlErrorNsecAvg; }
    EC_T_INT    GetCtlErrorMax(EC_T_VOID) { return m_nCtlErrorNsecMax; }

    virtual EC_T_VOID   NotifyInSync(EC_T_BOOL bInSyncNotified, EC_T_INT nCtlErrorNsec, EC_T_DWORD dwErrorCode);

private:
    /* InSync monitoring */
    EC_T_DWORD              m_dwInSyncLimit;
    CEcTimer                m_oInSyncSettleTime;
    EC_T_DWORD              m_dwInSyncSettleTime;
    EC_T_DWORD              m_dwCycleCnt;
    EC_T_DWORD              m_dwMonitoringStartDelayCycles;

protected:
    CEcMaster*              m_poMaster;
    EC_T_INT                m_nCtlErrorNsecAvg;         /* average controller error */
    EC_T_INT                m_nCtlErrorNsecMax;         /* maximum controller error */

    EC_T_BOOL               m_bInSync;
};

#if (defined INCLUDE_DCX)
class CEcDcxInSync : public CEcDcmInSync
{
public:
    CEcDcxInSync(CEcMaster* poMaster, CEcDcx* poDcx) : CEcDcmInSync(poMaster) { m_poDcx = poDcx; }
    virtual ~CEcDcxInSync() { }

protected:
    EC_T_VOID NotifyInSync(EC_T_BOOL bInSyncNotified, EC_T_INT nCtlErrorNsec, EC_T_DWORD dwErrorCode);

private:
    CEcDcx* m_poDcx;
};
#endif

class CEcDcmController
{
public:
    CEcDcmController(CEcMaster* poMaster);
    ~CEcDcmController() {}

    EC_T_BOOL               GetSyncToTimerIrq(EC_T_VOID) { return m_bSyncToTimerIrq; }
    EC_T_INT                GetCtlSetVal(EC_T_VOID) { return  (EC_T_INT)m_qwDcStartTimeOffset - m_nSetValNsec; }
    EC_T_INT                GetCtlActVal(EC_T_VOID) { return  m_nActValNsec; }
    EC_T_DWORD              GetTimerAdjust(EC_T_INT* pnAdjustPermil);

    EC_T_DWORD              Init(DCBS_CTL_PARAM* pCtlParam);
    EC_T_DWORD              Configure(EC_T_DCM_CONFIG_BUSSHIFT* pDcmConfig);
    EC_T_DWORD              GetConfig(EC_T_DCM_CONFIG_BUSSHIFT* pDcmConfig);
    EC_T_DWORD              DistributeBusTimeProcess(EC_T_UINT64 qwBusTime);
    EC_T_DWORD              ProcessCtlError(EC_T_INT nCtlErrorNsec);
    EC_T_DWORD              Reset(EC_T_VOID);
    EC_T_VOID               SetStatusCode(EC_T_DWORD dwStatus) { m_dwStatusCode = dwStatus; }
    EC_T_DWORD              GetStatusCode(EC_T_VOID) { return m_dwStatusCode; }
    EC_T_DWORD              StatusShow(EC_T_INT bnResetCounters);
    EC_T_UINT64             GetDcStartTime(EC_T_UINT64 qwBusTime);

    EC_T_DWORD              UpdateLog(EC_T_DCM_LOG* pDcmLog, EC_T_CHAR** ppszLog, EC_T_DWORD dwMaxLogSize);
private:
    EC_T_DWORD              Controller();
    EC_T_CHAR*              CtlStatusString(E_DCBS_CTL_STATUS ECtlStatus);

private:
    CEcMaster*              m_poMaster;

    EC_T_DWORD              m_dwStatusCode;             /* actual status/error code */
    EC_T_BOOL               m_bResetRequest;

    /* controller parameters */
    DCBS_CTL_PARAM*         m_pCtlparam;                /* controller parameter (in and out) */
    EC_T_BOOL               m_bConfigured;

    EC_T_BOOL               m_bSyncToTimerIrq;
    EC_T_INT                m_nSetValNsec;              /* controller set value */
    EC_T_BOOL               m_bNewSetVal;               /* flag: new set value set */
    EC_T_INT                m_nActValNsec;              /* controller actual value */

    E_DCBS_CTL_STATUS       m_ECtlStatus;               /* current controller status */
    EC_T_DWORD              m_adwStatusCnt[NUM_CTL_STATUS];  /* controller status history counter */
EC_INSTRUMENT_MOCKABLE_VAR
    EC_T_UINT64             m_qwBusTimeFirst;           /* first bus cycle saw by the controller */
    EC_T_UINT64             m_qwBusTimeLast;            /* last bus cycle saw by the controller */
    EC_T_UINT64             m_qwDcStartTimeOffset;
    EC_T_DWORD              m_dwBusCycleCount;
    EC_T_BOOL               m_bCheckBusCycleTime;
};

class CEcDcmBase
{
public:
    CEcDcmBase(CEcMaster* poMaster);
    virtual ~CEcDcmBase();

    virtual EC_T_VOID       SetStatus(EC_T_DWORD dwStatus) { EC_UNREFPARM(dwStatus); }
    virtual EC_T_DWORD      GetStatus(EC_T_DWORD* pdwErrorCode, EC_T_INT* pnDiffCur, EC_T_INT* pnDiffAvg, EC_T_INT* pnDiffMax)
                            {
                                EC_UNREFPARM(pdwErrorCode); EC_UNREFPARM(pnDiffAvg); EC_UNREFPARM(pnDiffCur); EC_UNREFPARM(pnDiffMax);
                                return EC_E_NOTSUPPORTED;
                            }
    virtual EC_T_DWORD      ResetStatus(EC_T_VOID) { return EC_E_NOERROR; }
    virtual EC_T_DWORD      ShowStatus(EC_T_VOID) { return EC_E_NOTSUPPORTED; };
    
    virtual EC_T_DWORD      DistributeSystemTimeProcess(EC_T_BOOL bStamped, EC_T_DWORD  dwLocalTime, EC_T_DWORD dwLocalTime2)
                            {
                                EC_UNREFPARM(bStamped); EC_UNREFPARM(dwLocalTime); EC_UNREFPARM(dwLocalTime2);
                                return EC_E_NOERROR;
                            }

    virtual EC_T_VOID       Reset(EC_T_VOID) {};

    virtual EC_T_DWORD      Configure(EC_T_DCM_CONFIG* pDcmConfig) 
                            { 
                                EC_UNREFPARM(pDcmConfig); 
#if (defined INCLUDE_MASTER_OBD)
                                /* set DCM in sync for eDcmMode_Off */
                                UpdateSdoService(); 
#endif
                                return EC_E_NOERROR; 
                            }
    virtual EC_T_DWORD      GetConfig(EC_T_DCM_CONFIG* pDcmConfig) { EC_UNREFPARM(pDcmConfig); return EC_E_NOERROR; }
    virtual EC_T_INT        GetCtlSetVal(EC_T_VOID) { return 0; }

    virtual EC_T_BOOL       IsInSync(EC_T_VOID) { return EC_TRUE; }
    virtual EC_T_DWORD      GetLog(EC_T_CHAR** pszLog);
    virtual EC_T_DWORD      GetLog(EC_T_DCM_LOG* pDcmLog);

    virtual EC_T_UINT64     DcStartTime(EC_T_UINT64 qwBusTime, EC_T_UINT64 qwCycleTime);

    virtual EC_T_BOOL       CheckForStamp(EC_T_VOID) { return EC_FALSE; }
    virtual EC_T_DWORD      RegisterStartTimeCallback(EC_T_DC_SYNCSO_REGDESC* pParms) { EC_UNREFPARM(pParms); return EC_E_INVALIDSTATE; }
    virtual EC_T_DWORD      RegisterTimeStampCallback(EC_T_REGISTER_TSPARMS* pParms, EC_T_REGISTER_TSRESULTS* pRes)
                            {
                                EC_UNREFPARM(pParms); EC_UNREFPARM(pRes);
                                return EC_E_INVALIDSTATE;
                            }
    virtual EC_T_DWORD      UnregisterTimeStampCallback(EC_T_VOID) { return EC_E_INVALIDSTATE; }
    virtual EC_T_DWORD      GetTimeStampCallback(EC_PF_TIMESTAMP* ppfnTimeStamp, 
                                                 EC_T_PVOID* ppvTimeStampContext, 
                                                 EC_T_DWORD** ppdwTimeStampLast);

    virtual EC_T_INT        GetAdjustWaitCycles(EC_T_VOID) { return 1; }
    virtual EC_T_DWORD      GetAdjust(EC_T_INT* pnAdjustPermil) { EC_UNREFPARM(pnAdjustPermil); return EC_E_INVALIDSTATE; }

    virtual CEcBusSlave*    GetBusSlaveBusTimeRef(EC_T_VOID);
    virtual EC_T_VOID       SetBusSlaveBusTimeRef(CEcBusSlave* poBusTimeRef) { EC_UNREFPARM(poBusTimeRef); }

    CEcSlave*               GetCfgSlaveSyncRef() { return m_poCfgSlaveSyncRef; }
    CEcSlave*               SetCfgSlaveSyncRef(CEcSlave* poCfgSlv) { return m_poCfgSlaveSyncRef = poCfgSlv; }

    virtual size_t          GetThisSize() { return sizeof(CEcDcmBase); }

    virtual EC_T_VOID       NotifyMasterStateChange(EC_MASTER_STATE eMasterState)
                            {
                                if (EC_MASTER_STATE_INIT == eMasterState)
                                    Reset();
                            }
    virtual EC_T_BOOL       UseDcLoopControlStdValues(EC_T_VOID) { return EC_TRUE; }

#if (defined INCLUDE_MASTER_OBD)
    EC_T_VOID               UpdateSdoService(EC_T_VOID);
#endif /* INCLUDE_MASTER_OBD */

public:
    EC_T_DWORD              m_dwRefPulseLength;
protected:
    static EC_T_DWORD       TimeStampCallback(EC_T_PVOID pvContext, EC_T_DWORD* pdwLocalTime);
           EC_T_DWORD       TimeStampCallback(EC_T_DWORD* pdwLocalTime);
    CEcMaster*              m_poMaster;                 /* Master instance used */
    CEcDcmInSync*           m_poInSync;                 /* InSync monitoring */

    CEcSlave*               m_poCfgSlaveSyncRef;
 
    EC_T_DWORD              m_dwTimeStampLastResult; /* Default variable to prevent null pointer in link layer */

    struct _EC_T_MEMORY_LOGGER* GetMemLog() { return m_poMaster->GetMemLog(); }

EC_INSTRUMENT_MOCKABLE_VAR
    /* logging */
    EC_T_DCM_LOG            m_oLog;
#define DCM_LOG_BUFFER_SIZE 512
    EC_T_CHAR               m_szLog[DCM_LOG_BUFFER_SIZE];
    EC_T_CHAR*              m_pszLogNext;
#if (defined INSTRUMENT_MASTER)
    EC_T_DWORD              m_dwLogGuardian;
#endif
};

class CEcDcmMasterShift : public CEcDcmBase
{
public:
    CEcDcmMasterShift(CEcMaster* poMaster);
    virtual ~CEcDcmMasterShift() {}

    virtual EC_T_VOID       SetStatus(EC_T_DWORD dwStatus) { m_oController.SetStatusCode(dwStatus); }
    virtual EC_T_DWORD      GetStatus(EC_T_DWORD* pdwErrorCode, EC_T_INT* pnDiffCur, EC_T_INT* pnDiffAvg, EC_T_INT* pnDiffMax);
    virtual EC_T_DWORD      ResetStatus(EC_T_VOID) { m_poInSync->ResetCtlErrorStatus(); return EC_E_NOERROR; }
    virtual EC_T_DWORD      ShowStatus(EC_T_VOID) { return m_oController.StatusShow(EC_FALSE); }

    virtual EC_T_BOOL       IsInSync(EC_T_VOID) { return m_poInSync->IsInSync(); }

    virtual EC_T_DWORD      Configure(EC_T_DCM_CONFIG* pDcmConfig);
    virtual EC_T_DWORD      GetConfig(EC_T_DCM_CONFIG* pDcmConfig);
    virtual EC_T_INT        GetCtlSetVal(EC_T_VOID) { return m_oController.GetCtlSetVal(); }

    virtual EC_T_DWORD      DistributeSystemTimeProcess(EC_T_BOOL bStamped, EC_T_DWORD  dwLocalTime, EC_T_DWORD dwLocalTime2);
    virtual EC_T_VOID       Reset(EC_T_VOID) { m_poInSync->Reset(); m_oController.Reset(); }
    
    virtual EC_T_INT        GetAdjustWaitCycles(EC_T_VOID) { return 4; }
    virtual EC_T_DWORD      GetAdjust(EC_T_INT* pnAdjustPermil) 
                            { return (m_oDcmConfig.bCtlOff) ? m_oController.GetTimerAdjust(pnAdjustPermil) : EC_E_INVALIDSTATE; }
    
    virtual EC_T_UINT64     DcStartTime(EC_T_UINT64 qwBusTime, EC_T_UINT64 qwCycleTime)
                            {
                                EC_UNREFPARM(qwCycleTime);
                                return m_oController.GetDcStartTime(qwBusTime);
                            }

    virtual EC_T_BOOL       CheckForStamp(EC_T_VOID) { return m_oController.GetSyncToTimerIrq(); }

    virtual size_t          GetThisSize() { return sizeof(CEcDcmMasterShift); }

protected:            
            EC_T_DWORD      AdjustTimer(EC_T_VOID);

    EC_T_DCM_CONFIG_MASTERSHIFT m_oDcmConfig;
    DCBS_CTL_PARAM              m_oCtlparam;                /* controller parameter (in and out) */
EC_INSTRUMENT_MOCKABLE_VAR
    CEcDcmController            m_oController;
};

class CEcDcmBusShift : public CEcDcmBase
{
public:
    CEcDcmBusShift(CEcMaster* poMaster);
    virtual ~CEcDcmBusShift() {}

    virtual EC_T_VOID       SetStatus(EC_T_DWORD dwStatus) { m_oController.SetStatusCode(dwStatus); }
    virtual EC_T_DWORD      GetStatus(EC_T_DWORD* pdwErrorCode, EC_T_INT* pnDiffCur, EC_T_INT* pnDiffAvg, EC_T_INT* pnDiffMax);
    virtual EC_T_DWORD      ResetStatus(EC_T_VOID) { m_poInSync->ResetCtlErrorStatus(); return EC_E_NOERROR; }
    virtual EC_T_DWORD      ShowStatus(EC_T_VOID) { return m_oController.StatusShow(EC_FALSE); }
    
    virtual EC_T_BOOL       IsInSync(EC_T_VOID) { return m_poInSync->IsInSync(); }

    virtual EC_T_DWORD      Configure(EC_T_DCM_CONFIG* pDcmConfig);
    virtual EC_T_DWORD      GetConfig(EC_T_DCM_CONFIG* pDcmConfig);
    virtual EC_T_INT        GetCtlSetVal(EC_T_VOID) { return m_oController.GetCtlSetVal(); }
    virtual EC_T_DWORD      Initialize(EC_T_VOID);

    virtual EC_T_DWORD      DistributeSystemTimeProcess(EC_T_BOOL bStamped, EC_T_DWORD  dwLocalTime, EC_T_DWORD dwLocalTime2);
    virtual EC_T_VOID       Reset(EC_T_VOID) { m_poInSync->Reset(); m_oController.Reset(); }
    
    virtual EC_T_DWORD      GetAdjust(EC_T_INT* pnAdjustPermil) 
                            { return (m_oDcmConfig.bCtlOff) ? m_oController.GetTimerAdjust(pnAdjustPermil) : EC_E_INVALIDSTATE; }

    virtual EC_T_UINT64     DcStartTime(EC_T_UINT64 qwBusTime, EC_T_UINT64 qwCycleTime)
                            {
                                EC_UNREFPARM(qwCycleTime);
                                return m_oController.GetDcStartTime(qwBusTime);
                            }

    virtual EC_T_BOOL       CheckForStamp(EC_T_VOID) { return m_oController.GetSyncToTimerIrq(); }

    virtual size_t          GetThisSize() { return sizeof(CEcDcmBusShift); }

    virtual EC_T_BOOL       UseDcLoopControlStdValues(EC_T_VOID) { return m_oDcmConfig.bUseDcLoopCtlStdValues; }

protected:
            EC_T_DWORD      AdjustDcShiftTime(EC_T_VOID);

    EC_T_DCM_CONFIG_BUSSHIFT    m_oDcmConfig;
    DCBS_CTL_PARAM              m_oCtlparam;                /* controller parameter (in and out) */
    EC_T_DC_SHIFTSYSTIME_DESC   m_oDcShiftDesc;
EC_INSTRUMENT_MOCKABLE_VAR
    CEcDcmController            m_oController;
};

class CEcDcmMasterRefClock : public CEcDcmBase
{
public:
    CEcDcmMasterRefClock(CEcMaster* poMaster);
    virtual ~CEcDcmMasterRefClock() {}

    virtual EC_T_VOID       SetStatus(EC_T_DWORD dwStatus) { m_dwStatusCode = dwStatus; }
    virtual EC_T_DWORD      GetStatus(EC_T_DWORD* pdwErrorCode, EC_T_INT* pnDiffCur, EC_T_INT* pnDiffAvg, EC_T_INT* pnDiffMax);
    virtual EC_T_DWORD      ShowStatus(EC_T_VOID) { return EC_E_NOTSUPPORTED; };
    virtual EC_T_DWORD      ResetStatus(EC_T_VOID) { return m_dwStatusCode = EC_E_NOERROR; }

    virtual EC_T_VOID       SetMasterTime(EC_T_UINT64 qwMasterTime) { m_qwMasterTime = qwMasterTime; }
    virtual EC_T_UINT64     GetMasterTime(EC_T_VOID) { return m_qwMasterTime; }

    virtual EC_T_BOOL       IsInSync(EC_T_VOID) { return m_poInSync->IsInSync(); }
	
    virtual EC_T_DWORD      Configure(EC_T_DCM_CONFIG* pDcmConfig);
    virtual EC_T_DWORD      GetConfig(EC_T_DCM_CONFIG* pDcmConfig);
    virtual EC_T_INT        GetCtlSetVal(EC_T_VOID) { return m_oDcmConfig.nCtlSetVal; }

    virtual EC_T_UINT64     DcStartTime(EC_T_UINT64 qwBusTime, EC_T_UINT64 qwCycleTime)
                            {
                                EC_UNREFPARM(qwCycleTime);
                                return qwBusTime + GetCtlSetVal();
                            }

    virtual EC_T_DWORD      DistributeSystemTimeProcess(EC_T_BOOL bStamped, EC_T_DWORD  dwLocalTime, EC_T_DWORD dwLocalTime2);
    virtual EC_T_VOID       Reset(EC_T_VOID) { m_poInSync->Reset(); }

    virtual CEcBusSlave*    GetBusSlaveBusTimeRef(EC_T_VOID) { return m_poBusSlaveBusTimeRef; }
    virtual EC_T_VOID       SetBusSlaveBusTimeRef(CEcBusSlave* poBusTimeRef) { m_poBusSlaveBusTimeRef = poBusTimeRef; }

    virtual size_t          GetThisSize() { return sizeof(CEcDcmMasterRefClock); }

private:
    EC_T_DCM_CONFIG_MASTERREFCLOCK  m_oDcmConfig;

    CEcBusSlave*                    m_poBusSlaveBusTimeRef;     /* bus slave used as reference for the bus time */

    EC_T_UINT64                     m_qwMasterTime;             /* master time distributed to the bus */
    EC_T_INT                        m_nCtlErrorNsec;
    EC_T_DWORD                      m_dwStatusCode;
};

class CEcDcmLinkLayerRefClock : public CEcDcmBase
{
public:
    CEcDcmLinkLayerRefClock(CEcMaster* poMaster);
    virtual ~CEcDcmLinkLayerRefClock() {}

    virtual EC_T_BOOL       IsInSync(EC_T_VOID) { return m_bInSync; }
    
    virtual EC_T_DWORD      Configure(EC_T_DCM_CONFIG* pDcmConfig);
    virtual EC_T_DWORD      GetConfig(EC_T_DCM_CONFIG* pDcmConfig);
    virtual EC_T_INT        GetCtlSetVal(EC_T_VOID) { return m_oDcmConfig.nCtlSetVal; }

    virtual EC_T_UINT64     DcStartTime(EC_T_UINT64 qwBusTime, EC_T_UINT64 qwCycleTime);
                            
    virtual EC_T_DWORD      DistributeSystemTimeProcess(EC_T_BOOL bStamped, EC_T_DWORD  dwLocalTime, EC_T_DWORD dwLocalTime2);
    virtual EC_T_VOID       Reset(EC_T_VOID) { m_bInSync = EC_FALSE; }

    virtual CEcBusSlave*    GetBusSlaveBusTimeRef(EC_T_VOID) { return m_poBusSlaveBusTimeRef; }
    virtual EC_T_VOID       SetBusSlaveBusTimeRef(CEcBusSlave* poBusTimeRef) { m_poBusSlaveBusTimeRef = poBusTimeRef; }

    virtual size_t          GetThisSize() { return sizeof(CEcDcmLinkLayerRefClock); }

private:
    EC_T_DCM_CONFIG_LINKLAYERREFCLOCK   m_oDcmConfig;

    CEcBusSlave*                        m_poBusSlaveBusTimeRef;     /* bus slave used as reference for the bus time */
    EC_T_BOOL                           m_bInSync;
};

class CEcMasterShiftByApp : public CEcDcmBase
{
public:
    CEcMasterShiftByApp(CEcMaster* poMaster);
    virtual ~CEcMasterShiftByApp() {}

    virtual EC_T_DWORD      RegisterStartTimeCallback(EC_T_DC_SYNCSO_REGDESC* pParms);
    virtual EC_T_DWORD      RegisterTimeStampCallback(EC_T_REGISTER_TSPARMS* pParms, EC_T_REGISTER_TSRESULTS* pRes);
    virtual EC_T_DWORD      UnregisterTimeStampCallback(EC_T_VOID);

    virtual EC_T_DWORD      GetTimeStampCallback(EC_PF_TIMESTAMP* ppfnTimeStamp,
                                         EC_T_PVOID*      ppvTimeStampContext, 
                                         EC_T_DWORD**     ppdwTimeStampLast);

    virtual EC_T_BOOL       IsInSync() { return m_oActualValues.bDcmCtlInSync; }

    virtual EC_T_DWORD      GetConfig(EC_T_DCM_CONFIG* pDcmConfig) { pDcmConfig->eMode = eDcmMode_MasterShiftByApp; return EC_E_NOERROR; }

    virtual EC_T_UINT64     DcStartTime(EC_T_UINT64 qwBusTime, EC_T_UINT64 qwCycleTime);
    virtual EC_T_BOOL       CheckForStamp(EC_T_VOID);

    virtual EC_T_DWORD      DistributeSystemTimeProcess(EC_T_BOOL bStamped, EC_T_DWORD  dwLocalTime, EC_T_DWORD dwLocalTime2);
    virtual EC_T_VOID       Reset(EC_T_VOID) { m_oActualValues.bResetRequest = EC_TRUE; }

    virtual size_t          GetThisSize() { return sizeof(CEcMasterShiftByApp); }

private:
    EC_T_BOOL               m_bInSync;

    /* DC start time callback management */
    EC_T_PFSYNCSO_CB        m_pfnDcStartTime;
    EC_T_DC_SYNCSO_CB_PARM* m_poDcStartTimeParm;

    /* timestamp callback management */
    EC_PF_TIMESTAMP         m_pfnTimeStamp;
    EC_T_PVOID              m_pvTimeStampContext;
    EC_T_DWORD              m_dwCheckForStampReload;
    EC_T_DWORD              m_dwCheckForStampCounter;

    EC_T_ACTUALVAL          m_oActualValues;
};

#if (defined INCLUDE_DCX)
class CEcDcx : public CEcDcmBusShift
{
public:
    CEcDcx(CEcMaster* poMaster);
    virtual ~CEcDcx() {}

    EC_T_DWORD              SetExtClock(EC_T_WORD wFixedAddress);
    EC_T_DWORD              GetDcxStatus(EC_T_DWORD* pdwErrorCode,
                             EC_T_INT*   pnDiffCur, EC_T_INT* pnDiffAvg, EC_T_INT* pnDiffMax, 
                             EC_T_INT64* pnTimeStampDiff);
    EC_T_INT64              GetTimeStampDiff(EC_T_VOID) { return m_nTimeStampDiff; }
    EC_T_VOID               DcxReset(EC_T_VOID);

    virtual EC_T_VOID       SetStatus(EC_T_DWORD dwStatus);
    virtual EC_T_DWORD      GetStatus(EC_T_DWORD* pdwErrorCode,
                                      EC_T_INT*   pnDiffCur, EC_T_INT* pnDiffAvg, EC_T_INT* pnDiffMax)
                            {
                                return m_oDcmMasterShift.GetStatus(pdwErrorCode, pnDiffCur, pnDiffAvg, pnDiffMax); /* DCM status */
                            }
    virtual EC_T_DWORD      ResetStatus(EC_T_VOID);
    virtual EC_T_DWORD      ShowStatus(EC_T_VOID);

    virtual EC_T_BOOL       IsInSync(EC_T_VOID) { return m_oDcmMasterShift.IsInSync() && m_poInSync->IsInSync(); } /* DCM/DCX InSync */

    virtual EC_T_DWORD      Configure(EC_T_DCM_CONFIG* pDcmConfig);
    virtual EC_T_DWORD      GetConfig(EC_T_DCM_CONFIG* pDcmConfig);
    virtual EC_T_INT        GetCtlSetVal(EC_T_VOID) { return m_oDcmMasterShift.GetCtlSetVal(); }

    virtual EC_T_DWORD      DistributeSystemTimeProcess(EC_T_BOOL bStamped, EC_T_DWORD  dwLocalTime, EC_T_DWORD dwLocalTime2);
    virtual EC_T_VOID       Reset(EC_T_VOID);

    virtual EC_T_INT        GetAdjustWaitCycles(EC_T_VOID) { return m_nAdjustWaitCycles; }
    virtual EC_T_DWORD      GetAdjust(EC_T_INT* pnAdjustPermil);

    virtual EC_T_UINT64     DcStartTime(EC_T_UINT64 qwBusTime, EC_T_UINT64 qwCycleTime)
                            {
                                return m_oDcmMasterShift.DcStartTime(qwBusTime, qwCycleTime);
                            }

    virtual size_t          GetThisSize() { return sizeof(CEcDcx); }

    virtual EC_T_VOID       NotifyMasterStateChange(EC_MASTER_STATE eMasterState)
                            {
                                CEcDcmBase::NotifyMasterStateChange(eMasterState);
                                if (EC_MASTER_STATE_PREOP == eMasterState)
                                    DcxReset();
                            }
    virtual EC_T_BOOL       UseDcLoopControlStdValues(EC_T_VOID) { return EC_TRUE; }

private:
    EC_T_DWORD              UpdateLog(EC_T_VOID);

    EC_T_DWORD              m_dwExtClockTimeout;            /* configured external synchronization timeout */

EC_INSTRUMENT_MOCKABLE_VAR
    CEcDcmMasterShift       m_oDcmMasterShift;              /* Internal DCM Mastershift */
    CEcSlave*               m_poExtClockSlave;              /* external clock slave */

    EC_T_BOOL               m_bSystemTimeDistributionDetected;
    EC_T_UINT64             m_qwIntTimeStamp;
    EC_T_UINT64             m_qwExtTimeStamp;
    EC_T_INT64              m_nTimeStampDiff;               /* current difference between external and internal time stamp */
    EC_T_INT64              m_nTimeStampDiffFirst;          /* difference between external and internal time stamp at startup */
    EC_T_BOOL               m_bTimeStampDiffFirstMissing;   /* if EC_TRUE m_nTimeStampDiffFirst must be calculated */

    EC_T_BOOL               m_bNoExtClock;                  /* no external synchronization */
    CEcTimer                m_oNoExtClockTimer;

    EC_T_BOOL               m_bTimeStampUpdateToggleOld;
    EC_T_DWORD              m_dwTimeStampUpdateWatchdog;

    EC_T_PD_VAR_INFO_INTERN*  m_pExtSyncStatusPdoTimeStampUpdate;  /* process variable "Time Stamp Update Toggle" */
    EC_T_PD_VAR_INFO_INTERN*  m_pExtSyncStatusPdoNoExtDevice;      /* process variable "External device not connected" */
    EC_T_PD_VAR_INFO_INTERN*  m_pExtSyncStatusPdoIntTimeStamp;     /* process variable "Internal Time Stamp" */
    EC_T_PD_VAR_INFO_INTERN*  m_pExtSyncStatusPdoExtTimeStamp;     /* process variable "External Time Stamp" */

    EC_T_INT                m_nAdjustWaitCycles;
};
#endif
#endif /* INC_ECDCBUSSHIFT */

/*-END OF SOURCE FILE--------------------------------------------------------*/
