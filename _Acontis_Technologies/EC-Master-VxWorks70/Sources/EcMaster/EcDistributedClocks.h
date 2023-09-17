/*-----------------------------------------------------------------------------
 * EcDistributedClocks.h    Header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Andreas Willig
 * Description              Interface to DCS class
 * Date                     [8/28/2006]
 *---------------------------------------------------------------------------*/

#ifndef __EcDistributedClocks_H__
#define __EcDistributedClocks_H__   1

/*-INCLUDES------------------------------------------------------------------*/
#include "EcDcBusShift.h"

/*-DEFINES-------------------------------------------------------------------*/
#define DC_PROPAGDELAYMEAS_CYCLES               8
#define DC_CONTINUOUSMEASURING_PERIOD       30000   /* 30sec */

#define DC_ARMW_BURSTCYCLES_DCINIT          10000   /* default value during startup */
#define DC_ARMW_BURSTSPP                       12

/* 0x0930/0x0931: DC SPEED COUNTER START (ECM_DCS_SPEEDCOUNT_START) */
#define DC_SPEEDCOUNT_START_STD_VALUE           ((EC_T_WORD)0x1000)
#define DC_SPEEDCOUNT_START_BUSSHIFT_VALUE      ((EC_T_WORD)0x0800)

/* 0x0934/0x0935: System Time Filter Depth and Speed Counter Filter Depth (ECM_DCS_SYSTIMEDIFF_FILTERDEPTH) */
#define DC_FILTER_DEPTH_BUSSHIFT_REFCLOCK_VALUE ((EC_T_WORD)0x0000)
#define DC_FILTER_DEPTH_STD_VALUE               ((EC_T_WORD)0x0C00)

#define DC_START_CYC_SAFETY_DEFAULT     (EC_MAKEQWORD(0,50000000))

/*-TYPEDEFS------------------------------------------------------------------*/
typedef struct _EC_T_DC_CALCULATEPROPAGDELAYS_CONTEXT 
{
    CEcBusSlave* pBusSlaveRef;    
    CEcBusSlave* pBusSlaveCur;
    EC_T_WORD    wPortInCur;
    CEcBusSlave* pBusSlaveLast;
    EC_T_WORD    wPortOutLast;
    EC_T_BOOL    bBehindBusSlaveRef;
    EC_T_DWORD   dwPropagDelay;
    EC_T_DWORD   dwPropagDelayExtrapolated;
} EC_T_DC_CALCULATEPROPAGDELAYS_CONTEXT;

/*-CLASS---------------------------------------------------------------------*/

class CEcMaster;
class CEcDevice;
class CEcDistributedClocks;

class CEcDistributedClocks
{
public:
    CEcDistributedClocks(                           CEcMaster*  pMaster                 );
    ~CEcDistributedClocks(                          EC_T_VOID                           );

    /* generic management */
    EC_T_VOID   ProcessResult(                      EC_T_DWORD result,
                                                    EC_T_DWORD invokeId,
                                                    PETYPE_EC_CMD_HEADER pEcCmdHeader   );
    EC_T_DWORD  Configure(                          EC_T_DC_CONFIGURE* pDcConfig        );
    EC_T_DWORD  GetConfig(                          EC_T_DC_CONFIGURE* pDcConfig        );

    EC_T_DWORD  StartInitDC(                        EC_T_VOID                           );
    EC_T_DWORD  StartTopologyChanged(               EC_T_VOID                           );
    EC_T_DWORD  WaitForStateMachine(                EC_T_VOID                           );
    EC_T_DWORD  CancelRequest(                      EC_T_VOID                           );

    /* reference slaves */
    EC_T_DWORD  SetRefClockAutoIncAddr(             EC_T_WORD   wAutoIncAddr            );
    EC_T_DWORD  SetRefClockFixedAddr(               EC_T_WORD   wFixedAddr              );

    EC_T_VOID   RemoveBusSlave(                     CEcBusSlave* poBusSlave             );

    EC_T_VOID   ResetRefBusSlaves(                  EC_T_VOID                           );
    EC_T_VOID   LookForRefBusSlaves(                CEcBusSlave* pBusSlave              );

    /* bus time */
    EC_T_VOID   SetBusTime(                         EC_T_UINT64 qwBusTime,
                                                    EC_T_BOOL   bBusTimeAvailable = EC_TRUE);
    EC_T_UINT64 GetBusTime(                         EC_T_VOID                           )
                    { return m_qwBusTime; }
    EC_T_UINT64 GetNextEstimatedBusTime(            EC_T_VOID                           )
                    { return (0 == m_nDisableEstimationCnt)?((2*m_qwBusTime) - m_qwBusTimeLast):0; }
    EC_T_VOID   DistributeSystemTimeSend(           EC_T_LINK_FRAMEDESC* poLinkFrameDesc,
                                                    EC_T_BOOL            bCycFramesAreProcessed,
                                                    EC_T_BOOL            bStampJob,
                                                    EcCycCmdConfigDesc*  pCmdDesc,
                                                    ETYPE_EC_CMD_HEADER* pCurCmd        );
    EC_T_VOID   DistributeSystemTimeProcess(        EcCycCmdConfigDesc*  pCmdDesc,
                                                    ETYPE_EC_CMD_HEADER* pCurCmd        );
    EC_T_VOID   ShiftSystemTimeSend(                EcCycCmdConfigDesc*  pCmdDesc,
                                                    ETYPE_EC_CMD_HEADER* pCurCmd        );
    EC_T_VOID   ReadSystemTimeDifferenceSend(       EcCycCmdConfigDesc*  pCmdDesc,
                                                    ETYPE_EC_CMD_HEADER* pCurCmd        );

    /* monitoring */
    EC_T_BOOL   IsInSync(                           EC_T_VOID                           )
                    { return m_bInSync; }
    EC_T_VOID   SetInSync(EC_T_BOOL bInSync)
                    { m_bInSync = bInSync; }
    EC_T_VOID   SetSyncWindowMonitoringConfigured(  EC_T_VOID                           )
                    { m_bSyncWindowMonitoringConfigured = EC_TRUE; }
    EC_T_VOID   ActivateSyncWindowMonitoring(       EC_T_VOID                           )
                    { m_bSyncWindowMonitoringActive = m_bSyncWindowMonitoringConfigured; }
    EC_T_VOID   SetDeviationLimit(                  EC_T_DWORD  dwDevLimit              );
    EC_T_DWORD  GetDeviationLimit(                  EC_T_VOID                           );
    EC_T_DC_SYNC_NTFY_DESC* 
                GetSlaveSyncNotification(           EC_T_VOID                           )
                    { return &m_oSlaveSyncNotification; }
    EC_T_WORD   GetSyncWindowMonitoringFixedAddr(   EC_T_VOID                           );
    EC_T_VOID   RefreshSyncWindowMonitoring(        EC_T_WORD  wFixedAddress,
                                                    EC_T_DWORD dwSystemTimeDifference   );
    EC_T_VOID   EnableContinuousMeasuring(          EC_T_VOID                           )
                    { m_bContinuousMeasuringEnabled = EC_TRUE; }
    EC_T_VOID   DisableContinuousMeasuring(         EC_T_VOID                           )
                    { m_bContinuousMeasuringEnabled = EC_FALSE; }
    EC_T_VOID   HandleContinuousMeasuring(          EC_T_VOID                           );

    /* DC master sync */
    EC_T_VOID   DcmReset(                           EC_T_VOID                           )
                    { m_poDcm->Reset(); }
    EC_T_VOID   DcmNotifyMasterStateChange(EC_MASTER_STATE eMasterState)
                    { m_poDcm->NotifyMasterStateChange(eMasterState); }

    EC_T_DWORD  DcmSetBusShiftConfigured(           EC_T_VOID                           );
    EC_T_BOOL   DcmGetBusShiftConfigured(           EC_T_VOID                           )
                    { return m_bBusShiftConfiguredInEni; }
    EC_T_DCM_MODE DcmGetMode(                       EC_T_VOID                           )
                    { return m_eDcmMode; }

    EC_T_DWORD  DcmConfigure(                       EC_T_DCM_CONFIG* pDcmConfig);
    EC_T_DWORD  DcmGetConfig(                       EC_T_DCM_CONFIG* pDcmConfig         )
                    { return m_poDcm->GetConfig(pDcmConfig); }
    EC_T_BOOL   DcmIsInSync(                        EC_T_VOID                           )
                    { return m_poDcm->IsInSync(); }
    EC_T_VOID   DcmSetStatus(                       EC_T_DWORD  dwStatus                )
                    { m_poDcm->SetStatus(dwStatus); }
    EC_T_DWORD  DcmGetStatus(                       EC_T_DWORD* pdwStatus, 
                                                    EC_T_INT*   pnDiffCur,
                                                    EC_T_INT*   pnDiffAvg,
                                                    EC_T_INT*   pnDiffMax               )
                    { return ((EC_NULL == m_poDcm)?(EC_NULL):(m_poDcm->GetStatus(pdwStatus, pnDiffCur, pnDiffAvg,pnDiffMax))); }
#if (defined INCLUDE_DCX)
    EC_T_DWORD  DcxGetStatus(                       EC_T_DWORD* pdwErrorCode,   
                                                    EC_T_INT*   pnDiffCur, 
                                                    EC_T_INT*   pnDiffAvg, 
                                                    EC_T_INT*   pnDiffMax,                 
                                                    EC_T_INT64* pnTimeStampDiff         );
#endif
    EC_T_DWORD  DcmResetStatus(                     EC_T_VOID                           )
                    { return m_poDcm->ResetStatus(); }
    EC_T_DWORD  DcmGetLog(                          EC_T_CHAR**   pszLog                )
                    { return ((EC_NULL == m_poDcm)?(EC_NULL):(m_poDcm->GetLog(pszLog))); }
    EC_T_DWORD  DcmGetLog(                          EC_T_DCM_LOG* pDcmLog               )
                    { return ((EC_NULL == m_poDcm)?(EC_NULL):(m_poDcm->GetLog(pDcmLog))); }
    EC_T_DWORD  DcmShowStatus(                      EC_T_VOID                           )
                    { return m_poDcm->ShowStatus(); }
    EC_T_DWORD  DcmRegisterDcStartTimeCallback(     EC_T_DC_SYNCSO_REGDESC* pParms      );
    EC_T_DWORD  DcmRegisterTimeStampCallback(       EC_T_REGISTER_TSPARMS* pParms,
                                                    EC_T_REGISTER_TSRESULTS* pRes       );
    EC_T_DWORD  DcmUnregisterTimeStampCallback(     EC_T_VOID                           )
                    { return m_poDcm->UnregisterTimeStampCallback(); }
    EC_T_BOOL   DcmCheckForStamp(                   EC_T_VOID                           )
                    { return m_bDCInitialized ? m_poDcm->CheckForStamp() : EC_FALSE; }
    EC_T_DWORD* DcmLastTsResultPtr(                 EC_T_VOID                           )
                    {
                        EC_T_DWORD* pdwTimeStampLast = EC_NULL;
                        m_poDcm->GetTimeStampCallback(EC_NULL, EC_NULL, &pdwTimeStampLast);
                        return pdwTimeStampLast; 
                    }
    static EC_T_DWORD DcmHostTimeStamp(             EC_T_PVOID  pCallerData,
                                                    EC_T_DWORD* pdwHostTime             );
#if (defined INCLUDE_DCX)
    EC_T_DWORD   DcxSetExtClock(                    EC_T_BOOL   bFixedAddress,
                                                    EC_T_WORD   wSlaveAddress           );
#endif
    EC_T_VOID    DcShiftSystemTime(                 EC_T_DC_SHIFTSYSTIME_DESC* pShiftSysTime)
                    { OsMemcpy(&m_oDcShiftConfig, pShiftSysTime, sizeof(EC_T_DC_SHIFTSYSTIME_DESC)); }

    /* DC start time */
    EC_T_VOID   SetDcStartTimeSafetyNsec(           EC_T_UINT64 qwNsec                  )
                    { m_qwDcStartTimeSafetyNsec = qwNsec; }
    EC_T_VOID   CalculateDcStartTime(               EC_T_VOID                           );
    EC_T_UINT64 GetDcStartTime(                     EC_T_VOID                           )
                    { return m_qwDcStartTime; }     /* first start time after DC (re)initialization */
    EC_T_UINT64 GetDcStartTimeNext(                 EC_T_VOID                           )
                    { return m_qwDcStartTimeNext; } /* start time for the next sync signal activation */
    EC_T_DWORD  GetAdjust(                          EC_T_INT*   pnAdjustPermil          )
                    { return m_poDcm->GetAdjust(pnAdjustPermil); }
    EC_T_VOID   SetFirstDcSlaveAsRefClock(          EC_T_BOOL   bFirstDcSlaveAsRefClock )
                    { m_bFirstDcSlaveAsRefClock = bFirstDcSlaveAsRefClock; }
    EC_T_DWORD  GetDcStartTimeGrid(                 EC_T_VOID                           )
                    { return m_dwDcStartTimeGrid; }

    struct _EC_T_MEMORY_LOGGER* GetMemLog() { return m_pMaster->GetMemLog(); }

private:
EC_INSTRUMENT_MOCKABLE_VAR
    EC_T_VOID   CalculatePortDelaysPrepare(         EC_T_VOID                           );
    EC_T_VOID   CalculatePortDelays(                CEcBusSlave*      pBusSlave,        
                                                    EC_T_INT          nMeasCnt = 1, 
                                                    EC_T_INT          nMeasCycles = 1,  
                                                    EC_T_BOOL         bCumulateDelay = EC_FALSE);

    EC_T_VOID   CalculatePropagDelaysPrepare(       EC_T_VOID                           );
    EC_T_VOID   CalculatePropagDelays(              EC_T_DC_CALCULATEPROPAGDELAYS_CONTEXT* pContext);
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_VOID   PrintPortDelays(                    CEcBusSlave*      pBusSlaveRoot     );
    EC_T_VOID   PrintPropagDelays(                  CEcBusSlave*      pBusSlaveRoot     );
#endif
    EC_T_VOID   CalculateSystemTimeOffset(          CEcBusSlave*      pBusSlave         );
#if (defined INCLUDE_RED_DEVICE)
    static EC_T_DWORD RedClosePortStamp(            EC_T_PVOID        pCallerData,
                                                    EC_T_DWORD*       pdwHostTimeLo     );
#endif

public:
    /* initialization flag */
    EC_T_BOOL               m_bDCInitialized;               /* EC_TRUE if DC is initialized */

private:
    /* generic management */
    CEcMaster*              m_pMaster;                      /* Master Instance used */
    CEcDevice*              m_poEthDevice;                  /* Ethernet device interface */

EC_INSTRUMENT_MOCKABLE_VAR
    /* propagation delays */
    EC_T_INT                m_nPortDelaysMeasCycles;        /* counter to manage the port delays measurement */
    EC_T_INT                m_nPortDelaysMeasCnt;           /* counts until m_nPortDelaysMeasCycles */
    EC_T_BOOL               m_bContinuousMeasuringEnabled;  /* continuous run-time measuring (0x900) enabled see emDcContDelayCompEnable() */
    EC_T_BOOL               m_bContinuousMeasuringActive;   /* continuous run-time measuring active */
    CEcTimer                m_oContinuousMeasuringTime;     /* continuous run-time measuring period */
#if (defined INCLUDE_RED_DEVICE)
    EC_T_BOOL               m_bLineBreakAtStart;            /* EC_TRUE if line break was detected at start */
    EC_T_DWORD              m_dwRedClosePortStamp;          /* dummy for callback */
public:
    CEcBusSlave*            m_pRedBusSlaveRefClock;         /* bus slave used as reference clock on the redundancy line */
    EC_T_SDWORD             m_bRedSynchronizeBusTime;       /* EC_TRUE if the system time (from main line) should be synchronized on the redundancy line
                                                               EC_FALSE if the system time from red line should be distribute on red line */
    EC_T_SDWORD             m_nRedPropagDelay;				/* propagation delay between the redundancy port and the slave used as reference clock on the redundancy line */
#endif /*INCLUDE_RED_DEVICE*/

    /* reference clock */
public:
    CEcSlave*               m_pCfgSlaveRefClock;            /* config slave used as reference clock */
    CEcBusSlave*            m_pBusSlaveRefClock;            /* bus slave used as reference clock */    
    EC_T_BOOL               m_bFirstDcSlaveAsRefClock;      /* if EC_TRUE first slave with DC initialization will become reference clock */
private:
    EC_T_BOOL               m_bBusSlaveRefClockFound;       /* EC_TRUE if reference clock found  */
    EC_T_BOOL               m_bBusSlaveRefClockChanged;     /* EC_TRUE if reference clock changed  */
    EC_T_BOOL               m_bNoDCSlaveWasConnected;       /* EC_TRUE if no DC slave was connected during the last state machine request */

    /* bus time */
    EC_T_UINT64             m_qwBusTimeLast;                /* last bus time */
public:
    EC_T_UINT64             m_qwBusTime;                    /* current bus time */
private:
    EC_T_DWORD              m_dwBusTimeMsecCounter;         /* bus time msec counter for wrap calculation */
    EC_T_INT                m_nDisableEstimationCnt;        /* next bus slave time estimation disabled counter is non zero */

    /* system time offset */
    CEcBusSlave*            m_pBusSlaveSystemTimeOffsetRef;
    EC_T_UINT64             m_qwReferenceTimeHost;
    EC_T_UINT64             m_qwReferenceTimeBus;

    /* distribution control */
    EC_T_BOOL               m_bAcycDistributionEnabled;
public:
    EC_T_BOOL               m_bCycDistributionActive;
    EC_T_BOOL               m_bAcycDistributionActive;
private:
    /* ARMW burst handling */
    EC_T_DWORD              m_dwBurstTotalLength;
    EC_T_DWORD              m_dwBurstBulk;
    EC_T_BOOL               m_bBulkInLinkLayer;
    EC_T_DWORD              m_dwBurstCycles;

    /* monitoring */
    EC_T_BOOL               m_bSyncWindowMonitoringConfigured; /* sync window monitoring configured in ENI */
    EC_T_BOOL               m_bSyncWindowMonitoringActive;     /* sync window monitoring active */
public:
    CEcBusSlave*            m_pBusSlaveSyncWindowMonitoring;
    CEcBusSlave*            m_pBusSlaveSyncWindowMonitoringLast; /* used to monitor the last system time difference */
private:
    EC_T_DWORD              m_dwSystemTimeDifference;       /* system time difference over all slaves (BRD 0x92C) */
    EC_T_DWORD              m_dwDeviation;
    EC_T_BOOL               m_bDeviationIsNegative;
    CEcTimer                m_oInSyncSettleTime;            /* Settle Time before deviation check is active */
    EC_T_DWORD              m_dwInSyncSettleTime;
    EC_T_DWORD              m_dwDeviationLimit;
    EC_T_BOOL               m_bInSync;
public:
    EC_T_BOOL               m_bInSyncNotifyActive;
private:
    EC_T_DC_SYNC_NTFY_DESC  m_oSlaveSyncNotification;

EC_INSTRUMENT_MOCKABLE_VAR
    /* DC start time */
    EC_T_DWORD              m_dwDcStartTimeGrid;            /* time grid to align dc start time */
    EC_T_UINT64             m_qwDcStartTimeSafetyNsec;      /* safety cycles count for DC start time */
    EC_T_UINT64             m_qwDcStartTime;                /* first start time after DC (re)initialization */
    EC_T_UINT64             m_qwDcStartTimeNext;            /* start time for the next sync signal activation */

    /* DCM */
    EC_T_DCM_MODE           m_eDcmMode;
    EC_T_BOOL               m_bBusShiftConfiguredInEni;     /* DCM BusShift configured in ENI */
    EC_T_BOOL               m_bDcmConfiguredByApp;          /* DCM configuration applied by application e.g. ecatDcmConfigure() */
EC_INSTRUMENT_MOCKABLE_VAR
    CEcDcmBase*             m_poDcm;                        /* DCM base class */

#if (defined INCLUDE_DCX)
    EC_T_WORD               m_wDcxExtClockFixedAddr;        /* DCX fixed address of external clock slave */
#endif
public:
    EC_T_DC_SHIFTSYSTIME_DESC m_oDcShiftConfig;

    /* state machine management */
    typedef enum _ECDCSM_T_DCSTATES
    {
        ecdcsm_idle                     ,           /* idle state */

        ecdcsm_start                    ,           /* start state of state machine */
        ecdcsm_start_wait               ,
        ecdcsm_start_done               ,

        ecdcsm_clearsystimevalues       ,           /* clear system time values (offset and delay) */
        ecdcsm_clearsystimevalues_send  ,
        ecdcsm_clearsystimevalues_wait  ,
        ecdcsm_clearsystimevalues_done  ,

#if (defined INCLUDE_RED_DEVICE)
        ecdcsm_reddisableecatevent      ,           /* disable ecat event on slave where redundancy link is connected */
        ecdcsm_reddisableecatevent_wait ,
        ecdcsm_reddisableecatevent_done ,

        ecdcsm_redcloseport             ,           /* close redundancy port to allow propagation delay measurement */
        ecdcsm_redcloseport_wait        ,
        ecdcsm_redcloseport_done        ,

        ecdcsm_redrefreshdlstatus       ,           /* refresh dl status on slave where redundancy link is connected */
        ecdcsm_redrefreshdlstatus_wait  ,
        ecdcsm_redrefreshdlstatus_done  ,
#endif
        ecdcsm_latchrecvtimes           ,           /* latch receive times (broadcast on Receive Time Port0) */
        ecdcsm_latchrecvtimes_wait      ,
        ecdcsm_latchrecvtimes_done      ,

        ecdcsm_readrecvtimes            ,           /* read receive times */
        ecdcsm_readrecvtimes_send       ,
        ecdcsm_readrecvtimes_wait       ,
        ecdcsm_readrecvtimes_done       ,

#if (defined INCLUDE_RED_DEVICE)
        ecdcsm_redopenport              ,           /* (re)open redundancy port */
        ecdcsm_redopenport_wait         ,
        ecdcsm_redopenport_done         ,

        ecdcsm_redenableecatevent       ,           /* (re)enable ecat event on slave where redundancy link is connected */
        ecdcsm_redenableecatevent_wait  ,
        ecdcsm_redenableecatevent_done  ,
#endif
        ecdcsm_calculateportdelays      ,           /* calculate port delays */
        ecdcsm_calculateportdelays_wait ,
        ecdcsm_calculateportdelays_done ,

        ecdcsm_calculatepropagdelays    ,        	/* calculate propagation delays */
        ecdcsm_calculatepropagdelays_wait,
        ecdcsm_calculatepropagdelays_done,

        ecdcsm_readreferencetime        ,           /* read reference time (stamped command) */
        ecdcsm_readreferencetime_wait   ,
        ecdcsm_readreferencetime_done   ,

        ecdcsm_writesystimevalues       ,           /* write system time values (system time offset and propagation delay) */
        ecdcsm_writesystimevalues_send  ,
        ecdcsm_writesystimevalues_wait  ,
        ecdcsm_writesystimevalues_done  ,

        ecdcsm_writepropagdelays        ,           /* write propagation delay */
        ecdcsm_writepropagdelays_send   ,
        ecdcsm_writepropagdelays_wait   ,
        ecdcsm_writepropagdelays_done   ,

#if (defined DEBUG_DC_SYSTIMEVALUES)
        ecdcsm_debugsystimevalues       ,        	/* for developpment purpose only */
        ecdcsm_debugsystimevalues_send  ,
        ecdcsm_debugsystimevalues_wait  ,       
        ecdcsm_debugsystimevalues_done  ,       
#endif

        ecdcsm_configdccontrollers1     ,        	/* configure DC controllers, step 1 */
        ecdcsm_configdccontrollers1_wait,
        ecdcsm_configdccontrollers1_send,
        ecdcsm_configdccontrollers1_done,

        ecdcsm_configdccontrollers2     ,        	/* configure DC controllers, step 2 */
        ecdcsm_configdccontrollers2_wait,
        ecdcsm_configdccontrollers2_send,
        ecdcsm_configdccontrollers2_done,
                                                
        ecdcsm_armwburst                ,        	/* ARMW Burst (FRMW) to saturize DC controllers */
        ecdcsm_armwburst_send           ,        	  
        ecdcsm_armwburst_wait           ,       
        ecdcsm_armwburst_done           ,

        ecdcsm_startbustime64emul       ,           /* start the 64bit bus time emulation */
        ecdcsm_startbustime64emul_wait  ,
        ecdcsm_startbustime64emul_done  ,

        ecdcsm_dcmreadpulselength       ,           /* read reference clock sync pulse length DCM */
        ecdcsm_dcmreadpulselength_wait  ,       
        ecdcsm_dcmreadpulselength_done  ,       

        ecdcsm_waitforinsync,                       /* is slave deviation within desired target range ? */

        ecdcsm_dcinit                   ,        	/* target state, DC Init */
        ecdcsm_dctopologychanged         ,           /* target state, Synchronize newly connected slaves */
        ecdcsm_dccontinuousmeasuring    ,           /* target state, continuous run-time measuring */

        ecdcsm_BCppDummy                = 0xFFFFFFFF
    } ECDCSM_T_DCSTATES;
    EC_T_CHAR* ESTATESText(ECDCSM_T_DCSTATES eState);

    static const ECDCSM_T_DCSTATES C_aStateList_dcinit[];
    static const ECDCSM_T_DCSTATES C_aStateList_topologychanged[];
    static const ECDCSM_T_DCSTATES C_aStateList_dccontinuousmeasuring[];

    typedef struct _ECDCSM_REQ
    {
        ECDCSM_T_DCSTATES   eState;
        EC_T_BOOL           bCancelRequest;
        EC_T_DWORD          dwResult;
        EC_T_BOOL           bUsed;
    } ECDCSM_REQ;
    
    EC_T_DWORD  RequestState(       ECDCSM_T_DCSTATES   eState          
                                   ,ECDCSM_REQ**        pHandle         );
    EC_T_DWORD  PollState(          ECDCSM_REQ*         pHandle         );
    EC_T_DWORD  ReleaseReq(         ECDCSM_REQ*         pHandle         );

    EC_T_VOID   DCStateMachine(     EC_T_VOID                           );

private:
    EC_T_BOOL   AbortState(         EC_T_VOID                           );
    EC_T_BOOL   NextState(          EC_T_VOID                           );
    EC_T_BOOL   NextState(          ECDCSM_T_DCSTATES   eState          );
    EC_T_BOOL   RetryState(         EC_T_VOID                           );

    /* Request buffers */
    ECDCSM_REQ              m_oRequest[2];
    /* Queued Request */
    ECDCSM_REQ*             m_pRequest;

    /* Current Running Request */
    ECDCSM_REQ*             m_pCurRequest;

    /* state machine */
    ECDCSM_T_DCSTATES       m_eReqState;
    const ECDCSM_T_DCSTATES* m_aCurStateList;
    EC_T_DWORD              m_dwCurStateIdx;
    ECDCSM_T_DCSTATES       m_eCurState;
EC_INSTRUMENT_MOCKABLE_VAR
    EC_T_DWORD              m_dwDCResult;

    /* handle to current requests */
    ECDCSM_REQ*             m_pDCReqExternal;
    ECDCSM_REQ*             m_pDCReqContinuousMeasuring;    /* continuous run-time measuring state machine request */

    /* generic state machine variables */
    CEcTimer                m_oTimeout;
    EC_T_DWORD              m_dwTimeout;
    EC_T_DWORD              m_dwEcatCmdPendingMax;          /* maximal number of pending ecat commands */
    EC_T_DWORD              m_dwEcatCmdPending;             /* number of pending ecat commands */
    EC_T_DWORD              m_dwEcatCmdSent;                /* number of pending ecat commands in a handling step */
    EC_T_DWORD              m_dwEcatCmdProcessed;           /* number of pending ecat commands in a handling step */
    CEcBusSlave*            m_pBusSlaveCur;                 /* currently evaluated bus slave */
    EC_T_DC_CALCULATEPROPAGDELAYS_CONTEXT m_oCalculatePropagDelaysMainContext;
#if (defined INCLUDE_RED_DEVICE)
    EC_T_DC_CALCULATEPROPAGDELAYS_CONTEXT m_oCalculatePropagDelaysRedContext;
#endif
};

/*-FUNCTION DECLARATION------------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FUNCTIONS-----------------------------------------------------------------*/

#endif /* __EcDistributedClocks_H__ */

/*-END OF SOURCE FILE--------------------------------------------------------*/
