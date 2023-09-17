/*-----------------------------------------------------------------------------
 * ATEMDemo.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT Master demo application
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <AtEthercat.h>
#include "SCInit.h"
#include "Logging.h"
#include "DynamicSystemCalls.h"


#ifdef ATEMRAS_SERVER 
#include <AtEmRasSrv.h>
#endif

#ifndef __SCUSRCONFIG__
#include "SCUsrConfig.h"
#endif

/* LOCAL DEFINES */
#define LogMsg      	S_poLog->LogMsg
#define LogError    	S_poLog->LogError

#define WARM_BOOT			0x00
#define BUS_SCAN_TIMEOUT 	5
#define FAILURE 			1


using namespace std;


/*-LOCAL VARIABLES-----------------------------------------------------------*/
static EC_T_CFG_SLAVE_INFO      S_aSlaveList[DEMO_MAX_NUM_OF_SLAVES];
static EC_T_CFG_SLAVE_INFO		S_dwSlavePc437xCfgInfo ;
static EC_T_CFG_SLAVE_INFO		S_dwSlaveAc437xCfginfo ;
static EC_T_DWORD				S_dwSlaveIdxTI437X	  = SLAVE_NOT_FOUND;
static EC_T_DWORD 				S_dwSlaveIdxPc437x	  = SLAVE_NOT_FOUND ;
static EC_T_DWORD 				S_dwSlaveIdxAc437x	  = SLAVE_NOT_FOUND ;
static EC_T_DWORD 			 	BusScanTimeOut = 0;


EC_T_DWORD	PCRXPDO_COUNTER;
EC_T_DWORD	ACRXPDO_COUNTER;


EC_T_DWORD          	   S_dwClntId        = 0;
//static EC_T_DWORD          	   S_dwClntId        = 0;
static CAtEmLogging*       S_poLog           = EC_NULL;
static T_DEMO_THREAD_PARAM S_DemoThreadParam = {0};
static EC_T_PVOID          S_pvtJobThread    = EC_NULL;
static EC_T_BOOL           S_shutdownflag    = EC_FALSE;

static          EC_T_WORD               connectedSlaveCount                     = 0;
/* Buffer containing offset information for each field of Weld Recipe Object */
static t_OD_OffsetMapping masterODList = {
				EL437x_INDEX_WELD_RECIPE, {EL437x_SUBINDEX_ENTRIES_OFFSET, EL437x_SUBINDEX_WELD_DELAY_OFFSET, EL437x_SUBINDEX_WELD_TIME_OFFSET, EL437x_SUBINDEX_INTENSITY_OFFSET, EL437x_SUBINDEX_CTRL_RECIPE_OVERRIDES_OFFSET, EL437x_SUBINDEX_INTENSITY_FACTOR_OFFSET, EL437x_SUBINDEX_PART_REFLECTIVITY_FACTOR_OFFSET, EL437X_SUBINDEX_TOOL_REFLECTIVITY_FACTOR_OFFSET}
                                    };

static EC_T_WORD          S_wUniqTransferId; /* Unique Transfer ID generator for SDO download / upload request message */


#ifdef ATEMRAS_SERVER 
static EC_T_BOOL           S_bRasSrvStarted  = EC_FALSE;
static EC_T_PVOID          S_pvRemoteApiSrvH = EC_NULL;
#endif
static EC_T_BOOL           S_bEnaPerfJobs    = EC_FALSE;
static EC_T_TSC_MEAS_DESC  S_TscMeasDesc;
static EC_T_CHAR*          S_aszMeasInfo[MAX_JOB_NUM] =
{
		(EC_T_CHAR*) "JOB_ProcessAllRxFrames",
		(EC_T_CHAR*) "JOB_SendAllCycFrames  ",
		(EC_T_CHAR*) "JOB_MasterTimer       ",
		(EC_T_CHAR*) "JOB_SendAcycFrames    ",
		(EC_T_CHAR*) "Cycle Time            ",
		(EC_T_CHAR*) "myAppWorkpd           "
};

///* Global object for PDO DATA*/
//TxPDO_PC PC_TxPdo,PrevPC_TxPdo,*PCTX;
//TxPDO_AC AC_TxPdo,PrevAC_TxPdo,*ACTX;
//
//RxPDO_PC PC_RxPdo,PrevPC_RxPdo,*PCRX;
//RxPDO_AC AC_RxPdo,PrevAC_RxPdo,*ACRX;


/*-FORWARD DECLARATIONS------------------------------------------------------*/
static EC_T_DWORD ecatNotifyCallback(EC_T_DWORD dwCode, EC_T_NOTIFYPARMS* pParms);
#if (defined ATEMRAS_SERVER)
static EC_T_DWORD RasNotifyWrapper(EC_T_DWORD dwCode, EC_T_NOTIFYPARMS* pParms);
#endif
static EC_T_VOID  tEcJobTask(EC_T_VOID* pvThreadParamDesc);

/*-MYAPP---------------------------------------------------------------------*/
/* Demo code: Remove/change this in your application */
static EC_T_DWORD myAppInit      (CAtEmLogging*           poLog, EC_T_INT nVerbose);
static EC_T_DWORD myAppPrepare   (CAtEmLogging*           poLog, EC_T_INT nVerbose);
static EC_T_DWORD myAppSetup     (CAtEmLogging*           poLog, EC_T_INT nVerbose, EC_T_DWORD dwClntId);
static EC_T_DWORD myAppWorkpd    (CAtEmLogging*           poLog, EC_T_INT nVerbose, EC_T_BYTE* pbyPDIn, EC_T_BYTE* pbyPDOut);
static EC_T_BYTE CoeSdoSubIndexToDataLength(EC_T_BYTE byValSubIndex) ;
static EC_T_DWORD displayWeldRecipe();
static EC_T_DWORD myAppNotify    (EC_T_DWORD 			 dwCode, EC_T_NOTIFYPARMS* pParms);
/* Demo code: End */

/*-FUNCTION DEFINITIONS------------------------------------------------------*/

/********************************************************************************/
/** \brief  EtherCAT Master demo Application.
*
* This is a EtherCAT Master demo application.
*
* \return  Status value.
*/

bool PcTxPdo_Diff()
{
	bool status;
	if((PrevPC_TxPdo.Amplitude 		!=	PC_TxPdo.Amplitude) 		|| 
	(PrevPC_TxPdo.Frequency			!= 	PC_TxPdo.Frequency) 		||
	(PrevPC_TxPdo.PCState			!= 	PC_TxPdo.PCState)			||
	(PrevPC_TxPdo.PC_StatusEvent	!=	PC_TxPdo.PC_StatusEvent)	||
	(PrevPC_TxPdo.Phase				!=	PC_TxPdo.Phase)				||
	(PrevPC_TxPdo.Power				!=	PC_TxPdo.Power))
	{
		PrevPC_TxPdo = PC_TxPdo; 
		//return true;
		status = true;
	}
	else
	{		
		//return false;
		status = false;
	}
	return status;
}

bool AcTxPdo_Diff()
{
	bool status;
	if((PrevAC_TxPdo.ACInputs 		!=	AC_TxPdo.ACInputs) 			|| 
	(PrevAC_TxPdo.ACState			!= 	AC_TxPdo.ACState) 			||
	(PrevAC_TxPdo.ACStatusEvent		!= 	AC_TxPdo.ACStatusEvent)		||
	(PrevAC_TxPdo.ActualDistance	!=	AC_TxPdo.ActualDistance)	||
	(PrevAC_TxPdo.ActualForce		!=	AC_TxPdo.ActualForce))
	{
		PrevAC_TxPdo = AC_TxPdo;
		//return true;
		status = true;
	}
	else
	{
		//return false;
		status = false;
	}
	return status;
}

bool PcRxPdo_Diff()
{
	bool status;
	if ((PrevPC_RxPdo.MasterState 	!= PC_RxPdo.MasterState) 		||
	(PrevPC_RxPdo.Amplitudechange 	!= PC_RxPdo.Amplitudechange)	||
	(PrevPC_RxPdo.MasterEvents 		!= PC_RxPdo.MasterEvents)		||
	(PrevPC_RxPdo.Frequency 		!= PC_RxPdo.Frequency))
	{
		PrevPC_RxPdo = PC_RxPdo;
		//return true;
		status = true;
	}
	else
	{
		//return false;
		status = false;
	}
	return status;
}

bool AcRxPdo_Diff()
{
	bool status;
	if ((PrevAC_RxPdo.SCState 		!= AC_RxPdo.SCState) 		||
	(PrevAC_RxPdo.ACControlEvent 	!= AC_RxPdo.ACControlEvent) ||
	(PrevAC_RxPdo.ACOutputs 		!= AC_RxPdo.ACOutputs))
	{
		PrevAC_RxPdo = AC_RxPdo;
		//return true;
		status = true;
	}
	else
	{
		//return false;
		status = false;
	}
	return status;
}

void GetSD_PDO()
{
	/*SD_ID RxSD_PC = sdOpen(RX_DATA_PC,0,0,0,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &PCRX);
	if(SD_ID_NULL != RxSD_PC)
	{
		cout<< endl << "READ : SD has been created successfully - PC_RX : " << RxSD_PC << endl;
	}

	SD_ID RxSD_AC = sdOpen(RX_DATA_AC,0,0,0,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &ACRX);
	if(SD_ID_NULL != RxSD_AC)
	{
		cout<< endl << "READ : SD has been created successfully - AC_RX : " << RxSD_AC << endl;
	}

	SD_ID TxSD_PC = sdOpen(TX_DATA_PC,0,0,0,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &PCTX);
	if(SD_ID_NULL != TxSD_PC)
	{
		cout<< endl << "READ : SD has been created successfully - PC_TX : " << TxSD_PC << endl;
	}

	SD_ID TxSD_AC = sdOpen(TX_DATA_AC,0,0,0,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &ACTX);
	if(SD_ID_NULL != TxSD_AC)
	{
		cout<< endl << "READ : SD has been created successfully - AC_TX : " << RxSD_AC << endl;
	}*/
	
	SD_ID RxSD_PC = sdOpen(RX_DATA_PC,0,OM_CREATE,SD_SIZE,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &PCRX);
	if(SD_ID_NULL != RxSD_PC)
	{
		cout << "MAIN : SD has been created successfully - PC : " << RxSD_PC << endl;
	}
	else
	{
		cout << "MAIN : Error on SD creation - PC : " << RxSD_PC << endl;
	}

	SD_ID RxSD_AC = sdOpen(RX_DATA_AC,0,OM_CREATE,SD_SIZE,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &ACRX);
	if(SD_ID_NULL != RxSD_AC)
	{
		cout << "MAIN : SD has been created successfully - AC : " << RxSD_AC << endl;
	}
	else
	{
		cout << "MAIN : Error on SD creation - AC : " << RxSD_AC << endl;
	}

	SD_ID TxSD_PC = sdOpen(TX_DATA_PC,0,OM_CREATE,SD_SIZE,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &PCTX);
	if(SD_ID_NULL != TxSD_PC)
	{
		cout << "MAIN : SD has been created successfully - PC_TX : " << TxSD_PC << endl;
	}
	else
	{
		cout << "MAIN : Error on SD creation - PC_TX : " << TxSD_PC << endl;
	}

	SD_ID TxSD_AC = sdOpen(TX_DATA_AC,0,OM_CREATE,SD_SIZE,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &ACTX);
	if(SD_ID_NULL != TxSD_AC)
	{
		cout << "MAIN : SD has been created successfully - AC_TX : " << TxSD_AC << endl;
	}
	else
	{
		cout << "MAIN : Error on SD creation - AC_TX : " << TxSD_AC << endl;
	}
}



/* Initialize EtherCat Master Stack */
EC_T_DWORD InitEcMasterStack(
	CEmNotification* 	pNotification
   ,EC_T_CNF_TYPE       eCnfType            /* [in] Enum type of configuration data provided */
   ,EC_T_PBYTE          pbyCnfData          /* [in] Configuration data */                      
   ,EC_T_DWORD          dwCnfDataLen        /* [in] Length of configuration data in byte */    
   ,EC_T_DWORD          dwBusCycleTimeUsec  /* [in]  bus cycle time in usec */
   ,EC_T_INT            nVerbose            /* [in]  verbosity level */
   ,EC_T_DWORD          dwDuration          /* [in]  test duration in msec (0 = forever) */
   ,EC_T_LINK_PARMS*    poLinkParms         /* [in]  pointer to link parameter */
   ,EC_T_VOID*          pvTimingEvent       /* [in]  Timing event handle */
   ,EC_T_DWORD          dwCpuIndex          /* [in]  SMP only: CPU index */
   ,EC_T_BOOL           bEnaPerfJobs        /* [in]  Performance measurement */
   ,EC_T_LINK_PARMS*    poLinkParmsRed      /* [in]  Redundancy Link Layer Parameter */
   ,CAtEmLogging*       poLog
)
{	
	EC_T_DWORD       dwRetVal 	= EC_E_NOERROR;
	EC_T_DWORD       dwRes    	= EC_E_NOERROR;
	EC_T_DWORD       BusScanRet = EC_E_NOERROR;
	CEcTimer         oTimeout;
	//EC_T_DWORD 		 BusScanTimeOut = 0;


/******************************/
/* Initialize EtherCAT master */
/******************************/
if (0 != nVerbose) LogMsg( "==========================" );
if (0 != nVerbose) LogMsg( "Initialize EtherCAT Master" );
if (0 != nVerbose) LogMsg( "==========================" );
{
	EC_T_INIT_MASTER_PARMS oInitParms;

	OsMemset(&oInitParms, 0, sizeof(EC_T_INIT_MASTER_PARMS));
	oInitParms.dwSignature                   = ATECAT_SIGNATURE;
	oInitParms.dwSize                        = sizeof(EC_T_INIT_MASTER_PARMS);
	oInitParms.pLinkParms                    = poLinkParms;
	oInitParms.pLinkParmsRed                 = poLinkParmsRed;
	oInitParms.dwBusCycleTimeUsec            = dwBusCycleTimeUsec;
	oInitParms.dwMaxBusSlaves                = MASTER_CFG_ECAT_MAX_BUS_SLAVES;
	oInitParms.dwMaxQueuedEthFrames          = MASTER_CFG_MAX_QUEUED_ETH_FRAMES;
	oInitParms.dwMaxSlaveCmdPerFrame         = MASTER_CFG_MAX_SLAVECMD_PER_FRAME;
	if (dwBusCycleTimeUsec < 1000)
	{
		oInitParms.dwMaxSentQueuedFramesPerCycle = 1;
	}
	else
	{
		oInitParms.dwMaxSentQueuedFramesPerCycle = MASTER_CFG_MAX_SENT_QUFRM_PER_CYC;
	}
	oInitParms.dwEcatCmdMaxRetries           = MASTER_CFG_ECAT_CMD_MAX_RETRIES;
	oInitParms.dwEoETimeout                  = MASTER_CFG_EOE_TIMEOUT;
	oInitParms.dwFoEBusyTimeout              = MASTER_CFG_FOE_BUSY_TIMEOUT;
	oInitParms.dwLogLevel                    = nVerbose;
	oInitParms.pfLogMsgCallBack              = CAtEmLogging::OsDbgMsgHookWrapper;
	dwRes = ecatInitMaster(&oInitParms);
	if (dwRes != EC_E_NOERROR)
	{
		dwRetVal = dwRes;
		LogError("Cannot initialize EtherCAT-Master! (Result = %s 0x%x)", ecatGetText(dwRes), dwRes);
		return FAILURE;
	}	
}

/* Create cyclic task to trigger master jobs */
/*********************************************/
S_DemoThreadParam.bJobThreadRunning  = EC_FALSE;
S_DemoThreadParam.bJobThreadShutdown = EC_FALSE;
S_DemoThreadParam.pLogInst           = S_poLog;
S_DemoThreadParam.pNotInst           = pNotification;
S_DemoThreadParam.dwCpuIndex         = dwCpuIndex;
S_DemoThreadParam.dwBusCycleTimeUsec = dwBusCycleTimeUsec;


S_pvtJobThread = OsCreateThread((EC_T_CHAR*)"tEcJobTask", tEcJobTask,
#if !(defined EC_VERSION_GO32)
		JOBS_THREAD_PRIO,
#else
		dwBusCycleTimeUsec,
#endif
		JOBS_THREAD_STACKSIZE, (EC_T_VOID*)&S_DemoThreadParam);
#ifdef RTAI
OsMakeThreadPeriodic(S_pvtJobThread, dwBusCycleTimeUsec);
#endif


/* wait until thread is running */
/* Delay is increased. Bus scan is long in the ring topology. */

oTimeout.Start(2000);		//2000 Milli seconds
while (!oTimeout.IsElapsed() && !S_DemoThreadParam.bJobThreadRunning)
{
	OsSleep(10);
}
if (!S_DemoThreadParam.bJobThreadRunning)
{
	dwRetVal = EC_E_TIMEOUT;
	LogError("Timeout starting JobTask");
	return FAILURE;
}
oTimeout.Stop();

/* Configure master */
dwRes = ecatConfigureMaster(eCnfType, pbyCnfData, dwCnfDataLen);
if (dwRes != EC_E_NOERROR)
{
	dwRetVal = dwRes;
	LogError("Cannot configure EtherCAT-Master! %s (Result = 0x%x)", ecatGetText(dwRes), dwRes);
	return FAILURE;
}

/* Register client */
{
	EC_T_REGISTERRESULTS oRegisterResults;

	OsMemset(&oRegisterResults, 0, sizeof(EC_T_REGISTERRESULTS));
	dwRes = ecatRegisterClient(ecatNotifyCallback, pNotification, &oRegisterResults);
	if (dwRes != EC_E_NOERROR)
	{
		dwRetVal = dwRes;
		LogError("Cannot register client! (Result = 0x%x)", dwRes);
		return FAILURE;
	}
	S_dwClntId = oRegisterResults.dwClntId;
	pNotification->SetClientID(S_dwClntId);
}

do
{
	/* Print found slaves */
	//if(nVerbose >= 2)
	if((nVerbose >= 2) && (BusScanTimeOut < BUS_SCAN_TIMEOUT))
	{
		dwRes = ecatScanBus(ETHERCAT_SCANBUS_TIMEOUT);
		BusScanRet = dwRes;
		switch (dwRes)
		{
		case EC_E_NOERROR:
		case EC_E_BUSCONFIG_MISMATCH:
		case EC_E_LINE_CROSSED:
			PrintSlaveInfos(INSTANCE_MASTER_DEFAULT, poLog);
			break;
		default:
			LogError("Cannot scan bus: %s (0x%lx)", ecatGetText(dwRes), dwRes);
			break;
		}
	}

		/*Checking number of Retries; If it exceeds, Stack will be rebooted*/
		if(BusScanTimeOut >= BUS_SCAN_TIMEOUT)
		{
			logMsg("BusScan Retried Count Exceeded; Rebooting the Stack\n",0,0,0,0,0,0);
			reboot(WARM_BOOT);
		}
	if(BusScanRet == EC_E_LINK_DISCONNECTED)
	{
		LogError("EC_E_LINK_DISCONNECTED\n");
		BusScanTimeOut++;
		BusScanRet = ReInitEcMasterStack(pNotification,eCnfType,pbyCnfData,dwCnfDataLen,dwBusCycleTimeUsec,nVerbose,dwDuration,poLinkParms,pvTimingEvent,dwCpuIndex,bEnaPerfJobs,poLinkParmsRed,poLog);
	}
	else if(BusScanRet == EC_E_BUSCONFIG_MISMATCH)
	{
		LogError("Bus Cinfiguration Mismatch Error\n");
		BusScanTimeOut++;
		OsSleep(10000);
		BusScanRet = ReInitEcMasterStack(pNotification,eCnfType,pbyCnfData,dwCnfDataLen,dwBusCycleTimeUsec,nVerbose,dwDuration,poLinkParms,pvTimingEvent,dwCpuIndex,bEnaPerfJobs,poLinkParmsRed,poLog);
	}
	else if((BusScanRet != EC_E_NOERROR) && (BusScanTimeOut <= BUS_SCAN_TIMEOUT))
	{             
		OsSleep(10000);
		BusScanTimeOut++;
		LogError("BusScanRet:%d....BusScanTimeOut%d \n", BusScanRet, BusScanTimeOut);
	}
	else
	{
		logMsg("BUS SCAN RETRIED COUNT : %d\n",BusScanTimeOut,0,0,0,0,0);
		BusScanTimeOut = 0;
		break;
	}

}while(BusScanRet != EC_E_NOERROR);

return BusScanRet;
}


/* Reinitialize EtherCat Master Stack */
EC_T_DWORD ReInitEcMasterStack(
	CEmNotification* 	pNotification
   ,EC_T_CNF_TYPE       eCnfType            /* [in] Enum type of configuration data provided */
   ,EC_T_PBYTE          pbyCnfData          /* [in] Configuration data */                      
   ,EC_T_DWORD          dwCnfDataLen        /* [in] Length of configuration data in byte */    
   ,EC_T_DWORD          dwBusCycleTimeUsec  /* [in]  bus cycle time in usec */
   ,EC_T_INT            nVerbose            /* [in]  verbosity level */
   ,EC_T_DWORD          dwDuration          /* [in]  test duration in msec (0 = forever) */
   ,EC_T_LINK_PARMS*    poLinkParms         /* [in]  pointer to link parameter */
   ,EC_T_VOID*          pvTimingEvent       /* [in]  Timing event handle */
   ,EC_T_DWORD          dwCpuIndex          /* [in]  SMP only: CPU index */
   ,EC_T_BOOL           bEnaPerfJobs        /* [in]  Performance measurement */
   ,EC_T_LINK_PARMS*    poLinkParmsRed      /* [in]  Redundancy Link Layer Parameter */
   ,CAtEmLogging*       poLog
)
{
	EC_T_DWORD       dwRetVal 	= EC_E_NOERROR;
	EC_T_DWORD       dwRes    	= EC_E_NOERROR;
	EC_T_DWORD       BusScanRet = EC_E_NOERROR;
	CEcTimer         oTimeout;

	/* Stop EtherCAT bus --> Set Master state to INIT */
	dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_INIT);
	if (EC_E_NOERROR != dwRes)
	{
		LogError("Cannot stop EtherCAT-Master! %s (0x%lx)", ecatGetText(dwRes), dwRes);
	}

	/* Unregister client */
	if (S_dwClntId != 0)
	{
		dwRes = ecatUnregisterClient(S_dwClntId);
		if (EC_E_NOERROR != dwRes)
		{
			LogError("Cannot unregister client! %s (0x%lx)", ecatGetText(dwRes), dwRes);
		}
		S_dwClntId = 0;
	}


#if (defined DEBUG) && (defined XENOMAI)
	/* Disable PRIMARY to SECONDARY MODE switch warning */
	dwRes = rt_task_set_mode(T_WARNSW, 0, NULL);
	if (dwRes != 0)
	{
		OsDbgMsg("OsCreateThread: rt_task_set_mode returned error %d\n", dwRes);
		OsDbgAssert(EC_FALSE);
	}
#endif /* XENOMAI */


	/* Shutdown tEcJobTask */
	S_DemoThreadParam.bJobThreadShutdown = EC_TRUE;
	oTimeout.Start(2000);		//2000 milli second
	while (S_DemoThreadParam.bJobThreadRunning && !oTimeout.IsElapsed())
	{
		OsSleep(10);
	}
	if (S_pvtJobThread != EC_NULL)
	{
		OsDeleteThreadHandle(S_pvtJobThread);
		S_pvtJobThread = EC_NULL;
	}

	/* Deinitialize master */
	dwRes = ecatDeinitMaster();
	if (EC_E_NOERROR != dwRes)
	{
		LogError("Cannot de-initialize EtherCAT-Master! %s (0x%lx)", ecatGetText(dwRes), dwRes);
	}
	
	/* Initialize Master stack */
	BusScanRet = InitEcMasterStack(pNotification,eCnfType,pbyCnfData,dwCnfDataLen,dwBusCycleTimeUsec,nVerbose,dwDuration,poLinkParms,pvTimingEvent,dwCpuIndex,bEnaPerfJobs,poLinkParmsRed,poLog);	
	return BusScanRet;
}


/* Dynamic system call for getting the bus scan retry count - called from RTP */
UINT32 EcatGetBusScanTimeoutSysCall()
{	
	return BusScanTimeOut;	
}


EC_T_DWORD SC_MAIN(
    CAtEmLogging*       poLog
   ,EC_T_CNF_TYPE       eCnfType            /* [in] Enum type of configuration data provided */
   ,EC_T_PBYTE          pbyCnfData          /* [in] Configuration data */                      
   ,EC_T_DWORD          dwCnfDataLen        /* [in] Length of configuration data in byte */    
   ,EC_T_DWORD          dwBusCycleTimeUsec  /* [in]  bus cycle time in usec */
   ,EC_T_INT            nVerbose            /* [in]  verbosity level */
   ,EC_T_DWORD          dwDuration          /* [in]  test duration in msec (0 = forever) */
   ,EC_T_LINK_PARMS*    poLinkParms         /* [in]  pointer to link parameter */
   ,EC_T_VOID*          pvTimingEvent       /* [in]  Timing event handle */
   ,EC_T_DWORD          dwCpuIndex          /* [in]  SMP only: CPU index */
   ,EC_T_BOOL           bEnaPerfJobs        /* [in]  Performance measurement */
#ifdef ATEMRAS_SERVER 
   ,EC_T_WORD           wServerPort         /* [in]   Remote API Server Port */
#endif
   ,EC_T_LINK_PARMS*    poLinkParmsRed      /* [in]  Redundancy Link Layer Parameter */
)
{	
	EC_T_DWORD       dwRetVal 	= EC_E_NOERROR;
	EC_T_DWORD       dwRes    	= EC_E_NOERROR;
	EC_T_DWORD       BusScanRet = EC_E_NOERROR;
	EC_T_BOOL        bRes     	= EC_FALSE;
	EC_T_DWORD  	 event		= 0;
	UINT8 			 TaskCreateFlag  = 0;
	char			 repeat 	= 'y';
	CEcTimer         oTimeout;
	EC_T_DWORD 		 BusScanTimeOut = 0;
	CEmNotification* pNotification  = EC_NULL;
	bIsExitKeyPressed			= false;
	Ctrl_TID 					= ERROR;
	
	/* Register the Custom system calls for SDO communication between SC(User spae) and Master stack(Kernel space) */
	if(SystemCallRegisty())
	{
		LogError("Error on registering the custom system calls for SDO communication between SC and Master stack\n");
	}
	
	/* Open the shared data region of PDO */
	GetSD_PDO();
	
	/* store parameters */
	S_poLog = poLog;
	S_bEnaPerfJobs = bEnaPerfJobs;
	S_DemoThreadParam.pvTimingEvent = pvTimingEvent;

	/* check if interrupt mode is selected */
	if (poLinkParms->eLinkMode != EcLinkMode_POLLING)
	{
		dwRetVal = EC_E_INVALIDPARM;
		LogError("Error: Link layer in 'interrupt' mode is not supported by EcMasterDemo. Please select 'polling' mode.");
		goto Exit;
	}
	/* set thread affinity */
	{
		EC_T_CPUSET CpuSet;

		EC_CPUSET_ZERO(CpuSet);
		EC_CPUSET_SET(CpuSet, dwCpuIndex);
		bRes = OsSetThreadAffinity(EC_NULL, CpuSet);
		if (!bRes)
		{
			dwRetVal = EC_E_INVALIDPARM;
			LogError("Error: Set thread affinity, invalid CPU index %d\n", dwCpuIndex);
			goto Exit;
		}
	}

	/* create notification context */
	pNotification = EC_NEW(CEmNotification(INSTANCE_MASTER_DEFAULT, poLog));
	if (EC_NULL == pNotification)
	{
		dwRetVal = EC_E_NOMEMORY;
		goto Exit;
	}
	pNotification->Verbose(nVerbose);

	if (S_bEnaPerfJobs)
	{
		PERF_MEASURE_JOBS_INIT(EC_NULL);
	}


	/* Demo code: Remove/change this in your application: Initialize application */
	/*****************************************************************************/
	dwRes = myAppInit(poLog, nVerbose);
	if (EC_E_NOERROR != dwRes)
	{
		LogError( (EC_T_CHAR*)"myAppInit failed, error code: 0x%x", dwRes );
		dwRetVal = dwRes;
		goto Exit;
	}
#ifdef ATEMRAS_SERVER
	/*******************************/
	/* Start RAS server if enabled */
	/*******************************/
	if (0xFFFF != wServerPort)
	{
		ATEMRAS_T_SRVPARMS oRemoteApiConfig;

		OsMemset(&oRemoteApiConfig, 0, sizeof(ATEMRAS_T_SRVPARMS));
		oRemoteApiConfig.oAddr.dwAddr       = 0;                    /* INADDR_ANY */
		oRemoteApiConfig.wPort              = wServerPort;
		oRemoteApiConfig.dwCycleTime        = REMOTE_CYCLE_TIME;    /* 2 msec */
		oRemoteApiConfig.dwWDTOLimit        = (REMOTE_WD_TO_LIMIT/REMOTE_CYCLE_TIME); /* WD Timeout after 10secs */
		oRemoteApiConfig.dwReConTOLimit     = 6000;                 /* Reconnect Timeout after 6000*2msec + 10secs */
		oRemoteApiConfig.dwMasterPrio       = MAIN_THREAD_PRIO;
		oRemoteApiConfig.dwClientPrio       = MAIN_THREAD_PRIO;
		oRemoteApiConfig.pvNotifCtxt        = pNotification;        /* Notification context */
		oRemoteApiConfig.pfNotification     = RasNotifyWrapper;     /* Notification function for emras Layer */
		oRemoteApiConfig.dwConcNotifyAmount = 100;                  /* for the first pre-allocate 100 Notification spaces */
		oRemoteApiConfig.dwMbxNotifyAmount  = 50;                   /* for the first pre-allocate 50 Notification spaces */
		oRemoteApiConfig.dwMbxUsrNotifySize = 3000;                 /* 3K user space for Mailbox Notifications */
		oRemoteApiConfig.dwCycErrInterval   = 500;                  /* span between to consecutive cyclic notifications of same type */
		if (0 != nVerbose) LogMsg("Start Remote API Server now\n");
		dwRes = emRasSrvStart(oRemoteApiConfig, &S_pvRemoteApiSrvH);
		if (EC_E_NOERROR != dwRes)
		{
			LogError("ERROR: Cannot spawn Remote API Server\n");
		}
		S_bRasSrvStarted = EC_TRUE;
	}
#endif
	
	/* Initialise master stack */
	InitEcMasterStack(pNotification,eCnfType,pbyCnfData,dwCnfDataLen,dwBusCycleTimeUsec,nVerbose,dwDuration,poLinkParms,pvTimingEvent,dwCpuIndex,bEnaPerfJobs,poLinkParmsRed,poLog);

PostInit:	
	/* Get Slave count */
	connectedSlaveCount = ecatGetNumConnectedSlaves();
	LogMsg("Total Slaves connected = %d", connectedSlaveCount);

	/* Print MAC address */
	if (nVerbose > 0)
	{
		ETHERNET_ADDRESS oSrcMacAddress;

		dwRes = ecatGetSrcMacAddress(&oSrcMacAddress);
		if (dwRes != EC_E_NOERROR)
		{
			LogError("Cannot get MAC address! (Result = 0x%x)", dwRes);
		}
		LogMsg("EtherCAT network adapter MAC: %02X-%02X-%02X-%02X-%02X-%02X\n",
				oSrcMacAddress.b[0], oSrcMacAddress.b[1], oSrcMacAddress.b[2], oSrcMacAddress.b[3], oSrcMacAddress.b[4], oSrcMacAddress.b[5]);
	}

	OsSleep(1000);		/* Delay for bus scan results print */

	/* Start EtherCAT bus --> set Master state to OPERATIONAL if ENI file provided */
	/*******************************************************************************/
	if (0 != nVerbose) LogMsg( "=====================" );
	if (0 != nVerbose) LogMsg( "Start EtherCAT Master" );
	if (0 != nVerbose) LogMsg( "=====================" );

	/* set master and bus state to INIT */
	dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_INIT);
	pNotification->ProcessNotificationJobs();
	if (dwRes != EC_E_NOERROR)
	{
		LogError("Cannot start set master state to INIT (Result = %s (0x%lx))", ecatGetText(dwRes), dwRes);
		dwRetVal = dwRes;
		goto Exit;
	}

	/******************************************************/
	/* Demo code: Remove/change this in your application  */
	/******************************************************/
	dwRes = myAppPrepare(poLog, nVerbose);
	if (EC_E_NOERROR != dwRes)
	{
		LogError((EC_T_CHAR*)"myAppPrepare failed, error code: 0x%x", dwRes);
		dwRetVal = dwRes;
		goto Exit;
	}
	/* set master and bus state to PREOP */
	dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_PREOP);
	pNotification->ProcessNotificationJobs();
	if (dwRes != EC_E_NOERROR)
	{
		LogError("Cannot start set master state to PREOP (Result = %s (0x%lx))", ecatGetText(dwRes), dwRes);
		dwRetVal = dwRes;
		goto Exit;
	}
	/* skip this step if demo started without ENI */
	if (pbyCnfData != EC_NULL)
	{
		/******************************************************/
		/* Demo code: Remove/change this in your application  */
		/******************************************************/
		/*dwRes = myAppSetup(poLog, nVerbose, S_dwClntId);
        		if (EC_E_NOERROR != dwRes)
        		{
					LogError((EC_T_CHAR*)"myAppSetup failed, error code: 0x%x", dwRes);
					dwRetVal = dwRes;
					goto Exit;
        		}
		 */

		/* set master and bus state to SAFEOP */
		dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_SAFEOP);
		pNotification->ProcessNotificationJobs();
		if (dwRes != EC_E_NOERROR)
		{
			LogError("Cannot start set master state to SAFEOP (Result = %s (0x%lx))", ecatGetText(dwRes), dwRes);
			dwRetVal = dwRes;
			goto Exit;
		}

		/* set master and bus state to OP */
		dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_OP);
		pNotification->ProcessNotificationJobs();
		if (dwRes != EC_E_NOERROR)
		{
			LogError("Cannot start set master state to OP (Result = %s (0x%lx))", ecatGetText(dwRes), dwRes);
			dwRetVal = dwRes;
			goto Exit;
		}
		else
		{
			bEcatDown = EC_FALSE;
		}
	}
	else
	{
		if (0 != nVerbose) LogMsg("No ENI file provided. EC-Master started with generated ENI file.");
	}

	if (S_bEnaPerfJobs)
	{
		LogMsg("");
		LogMsg("Job times during startup <INIT> to <%s>:", ecatStateToStr(ecatGetMasterState()));
		PERF_MEASURE_JOBS_SHOW();       /* show job times */
		LogMsg("");
		ecatPerfMeasReset(&S_TscMeasDesc, 0xFFFFFFFF);        /* clear job times of startup phase */
	}

#if (defined DEBUG) && (defined XENOMAI)
	/* Enabling mode switch warnings for shadowed task */
	dwRes = rt_task_set_mode(0, T_WARNSW, NULL);
	if (0 != dwRes)
	{
		OsDbgMsg("EnableRealtimeEnvironment: rt_task_set_mode returned an error\n");
		OsDbgAssert(EC_FALSE);
	}
#endif
	
	/* RUN MASTER STACK INFINITE */
	while(!bIsExitKeyPressed)
	{	
		/* Get public control task ID to send the event on every 250us */
		
			Ctrl_TID = taskNameToId(CTRL_TASK_NAME);

			if(ERROR == Ctrl_TID)
			{
			LogError("CONTROL TASK ID NOT FOUND \n");
		}
		
		/* run the demo */
		if (dwDuration != 0)
		{
			oTimeout.Start(dwDuration);
		}
		while (bRun && (!oTimeout.IsStarted() || !oTimeout.IsElapsed()))
		{
			if (nVerbose >= 2)
			{
				PERF_MEASURE_JOBS_SHOW();        //show job times 
			}
			bRun = !OsTerminateAppRequest(); 	//check if demo shall terminate 

			/**************************************************************************************/
			// Demo code: Remove/change this in your application: Do some diagnosis outside job task 
			/**************************************************************************************/
			//myAppDiagnosis(poLog, nVerbose);

			//process notification jobs 
			pNotification->ProcessNotificationJobs();

			OsSleep(5);
			if(bEcatDown)
			{
				ReInitEcMasterStack(pNotification,eCnfType,pbyCnfData,dwCnfDataLen,dwBusCycleTimeUsec,nVerbose,dwDuration,poLinkParms,pvTimingEvent,dwCpuIndex,bEnaPerfJobs,poLinkParmsRed,poLog);
				goto PostInit;
			}
		}
		OsSleep(1000);
	}

	S_shutdownflag = EC_TRUE;
	OsSleep(100);

	pNotification->ProcessNotificationJobs();
	OsSleep(1000);    

	if (S_bEnaPerfJobs)
	{
		LogMsg("");
		LogMsg("Job times before shutdown");
		PERF_MEASURE_JOBS_SHOW();       /* show job times */
	}

	Exit:

	if (0 != nVerbose) LogMsg( "========================" );
	if (0 != nVerbose) LogMsg( "Shutdown EtherCAT Master" );
	if (0 != nVerbose) LogMsg( "========================" );


	/* Stop EtherCAT bus --> Set Master state to INIT */
	dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_INIT);
	if (EC_E_NOERROR != dwRes)
	{
		LogError("Cannot stop EtherCAT-Master! %s (0x%lx)", ecatGetText(dwRes), dwRes);
	}

	/* Unregister client */
	if (S_dwClntId != 0)
	{
		dwRes = ecatUnregisterClient(S_dwClntId);
		if (EC_E_NOERROR != dwRes)
		{
			LogError("Cannot unregister client! %s (0x%lx)", ecatGetText(dwRes), dwRes);
		}
		S_dwClntId = 0;
	}


#if (defined DEBUG) && (defined XENOMAI)
	/* Disable PRIMARY to SECONDARY MODE switch warning */
	dwRes = rt_task_set_mode(T_WARNSW, 0, NULL);
	if (dwRes != 0)
	{
		OsDbgMsg("OsCreateThread: rt_task_set_mode returned error %d\n", dwRes);
		OsDbgAssert(EC_FALSE);
	}
#endif /* XENOMAI */



	/* Shutdown tEcJobTask */
	S_DemoThreadParam.bJobThreadShutdown = EC_TRUE;
	oTimeout.Start(2000);
	while (S_DemoThreadParam.bJobThreadRunning && !oTimeout.IsElapsed())
	{
		OsSleep(10);
	}
	if (S_pvtJobThread != EC_NULL)
	{
		OsDeleteThreadHandle(S_pvtJobThread);
		S_pvtJobThread = EC_NULL;
	}

	/* Deinitialize master */
	dwRes = ecatDeinitMaster();
	if (EC_E_NOERROR != dwRes)
	{
		LogError("Cannot de-initialize EtherCAT-Master! %s (0x%lx)", ecatGetText(dwRes), dwRes);
	}
#ifdef ATEMRAS_SERVER
	/* Stop RAS server */
	if (S_bRasSrvStarted)
	{
		LogMsg("Stop Remote Api Server\n");

		if (EC_E_NOERROR != emRasSrvStop(S_pvRemoteApiSrvH, 2000))
		{
			LogError("Remote API Server shutdown failed\n");
		}
	}
#endif
	if (S_bEnaPerfJobs)
	{
		PERF_MEASURE_JOBS_DEINIT();
	}
	/* delete notification context */
	SafeDelete(pNotification);

	return dwRetVal;
}


/********************************************************************************/
/** \brief  Trigger jobs to drive master, and update process data.
*
* \return N/A
*/
static EC_T_VOID tEcJobTask(EC_T_VOID* pvThreadParamDesc)
{
	//LogMsg("-----------------tScJobTask start------------------\n"); 
	
    EC_T_DWORD           dwRes             = EC_E_ERROR;
    T_DEMO_THREAD_PARAM* pDemoThreadParam  = (T_DEMO_THREAD_PARAM*)pvThreadParamDesc;
    EC_T_CPUSET          CpuSet;
    EC_T_BOOL            bPrevCycProcessed = EC_FALSE;
    EC_T_INT             nOverloadCounter  = 0;               /* counter to check if cycle time is to short */
    EC_T_BOOL            bOk;

    EC_CPUSET_ZERO(CpuSet);
    EC_CPUSET_SET(CpuSet, pDemoThreadParam->dwCpuIndex);
    bOk = OsSetThreadAffinity(EC_NULL, CpuSet);
    if (!bOk)
    {
        LogError("Error: Set job task affinity, invalid CPU index %d\n", pDemoThreadParam->dwCpuIndex);
        goto Exit;
    }

    /* Start periodic task right before where it is used,
     * otherwise, OsSleepTillTick() might fail if this thread is suspended before being able to call OsMakeThreadPeriodic() */
#if (defined XENOMAI)
    OsMakeThreadPeriodic(rt_task_self(), pDemoThreadParam->dwBusCycleTimeUsec);
#endif

    	/* demo loop */
    	pDemoThreadParam->bJobThreadRunning = EC_TRUE;
    	do
    	{
    		/* wait for next cycle (event from scheduler task) */
#if (defined RTAI) || (defined XENOMAI)
        OsSleepTillTick(); /* period is set after creating jobtask */
#else
        	OsWaitForEvent(pDemoThreadParam->pvTimingEvent, EC_WAITINFINITE);
#endif /* !RTAI && !XENOMAI*/
        	
        	PERF_JOB_END(PERF_CycleTime);
        	PERF_JOB_START(PERF_CycleTime);

        	/* process all received frames (read new input values) */
        	PERF_JOB_START(JOB_ProcessAllRxFrames);
        	dwRes = ecatExecJob( eUsrJob_ProcessAllRxFrames, &bPrevCycProcessed );
        	if (EC_E_NOERROR != dwRes && EC_E_INVALIDSTATE != dwRes && EC_E_LINK_DISCONNECTED != dwRes)
        	{
        		LogError("ERROR: ecatExecJob( eUsrJob_ProcessAllRxFrames): %s (0x%lx)", ecatGetText(dwRes), dwRes);
        	}
        	PERF_JOB_END(JOB_ProcessAllRxFrames);

        	if (EC_E_NOERROR == dwRes)
        	{
        		if (!bPrevCycProcessed)
        		{
        			/* it is not reasonable, that more than 5 continuous frames are lost */
        			nOverloadCounter += 10;
        			if (nOverloadCounter >= 50)
        			{
        				LogError( "Error: System overload: Cycle time too short or huge jitter!" );
        				bEcatDown = EC_TRUE;
        			}
        			else
        			{
        				LogError( "eUsrJob_ProcessAllRxFrames - not all previously sent frames are received/processed (frame loss)!" );
        			}
        		}
        		else
        		{
        			/* everything o.k.? If yes, decrement overload counter */
        			if (nOverloadCounter > 0)    nOverloadCounter--;
        		}
        	}



		/*****************************************************/
		/* Demo code: Working process data cyclic call */
		/*****************************************************/
        	if(S_shutdownflag == EC_FALSE && (ERROR != Ctrl_TID))
		{
            PERF_JOB_START(PERF_myAppWorkpd);
            /* This function should stop immediately after shutdown hence check global flag */
            myAppWorkpd(pDemoThreadParam->pLogInst, pDemoThreadParam->pNotInst->Verbose(), ecatGetProcessImageInputPtr(), ecatGetProcessImageOutputPtr());
            PERF_JOB_END(PERF_myAppWorkpd);
		}
        /* write output values of current cycle, by sending all cyclic frames */
        PERF_JOB_START(JOB_SendAllCycFrames);
        dwRes = ecatExecJob( eUsrJob_SendAllCycFrames, EC_NULL );
        if (EC_E_NOERROR != dwRes && EC_E_INVALIDSTATE != dwRes && EC_E_LINK_DISCONNECTED != dwRes)
        {
            LogError("ecatExecJob( eUsrJob_SendAllCycFrames,    EC_NULL ): %s (0x%lx)", ecatGetText(dwRes), dwRes);
        }
        PERF_JOB_END(JOB_SendAllCycFrames);

        /* remove this code when using licensed version */
        if (EC_E_EVAL_EXPIRED == dwRes )
        {
            bRun = EC_FALSE;        /* set shutdown flag */
        }

        /* Execute some administrative jobs. No bus traffic is performed by this function */
        PERF_JOB_START(JOB_MasterTimer);
        dwRes = ecatExecJob(eUsrJob_MasterTimer, EC_NULL);
        if (EC_E_NOERROR != dwRes && EC_E_INVALIDSTATE != dwRes)
        {
            LogError("ecatExecJob(eUsrJob_MasterTimer, EC_NULL): %s (0x%lx)", ecatGetText(dwRes), dwRes);
        }
        PERF_JOB_END(JOB_MasterTimer);

        /* send queued acyclic EtherCAT frames */
        PERF_JOB_START(JOB_SendAcycFrames);
        dwRes = ecatExecJob(eUsrJob_SendAcycFrames, EC_NULL);
        if (EC_E_NOERROR != dwRes && EC_E_INVALIDSTATE != dwRes && EC_E_LINK_DISCONNECTED != dwRes)
        {
            LogError("ecatExecJob(eUsrJob_SendAcycFrames, EC_NULL): %s (0x%lx)", ecatGetText(dwRes), dwRes);
        }
        PERF_JOB_END(JOB_SendAcycFrames);
        
#if !(defined NO_OS)
    } while (!pDemoThreadParam->bJobThreadShutdown);

    PERF_MEASURE_JOBS_SHOW();

    pDemoThreadParam->bJobThreadRunning = EC_FALSE;
#else
    /* in case of NO_OS the job task function is called cyclically within the timer ISR */
    } while (EC_FALSE);
    pDemoThreadParam->bJobThreadRunning = !pDemoThreadParam->bJobThreadShutdown;
#endif

Exit:
#if (defined EC_VERSION_RTEMS)
    rtems_task_delete(RTEMS_SELF);
#endif
    
    return;
}

/********************************************************************************/
/** \brief  Handler for master notifications
*
* \return  Status value.
*/
static EC_T_DWORD ecatNotifyCallback(
    EC_T_DWORD         dwCode,  /**< [in]   Notification code */
    EC_T_NOTIFYPARMS*  pParms   /**< [in]   Notification parameters */
                                         )
{
    EC_T_DWORD         dwRetVal                = EC_E_NOERROR;
    CEmNotification*   pNotifyInstance;

    if ((EC_NULL == pParms)||(EC_NULL==pParms->pCallerData))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* notification for application ? */
    if ((dwCode >= EC_NOTIFY_APP) && (dwCode <= EC_NOTIFY_APP+EC_NOTIFY_APP_MAX_CODE))
    {
        /*****************************************************/
        /* Demo code: Remove/change this in your application */
        /* to get here the API ecatNotifyApp(dwCode, pParms) has to be called */
        /*****************************************************/
        dwRetVal = myAppNotify(dwCode-EC_NOTIFY_APP, pParms);
    }
    else
    {
        pNotifyInstance = (CEmNotification*)pParms->pCallerData;
        
        /* call the default handler */
        dwRetVal = pNotifyInstance->ecatNotify(dwCode, pParms);
    }

Exit:
    return dwRetVal;
}


/********************************************************************************/
/** \brief  Handler for master RAS notifications
*
*
* \return  Status value.
*/
#ifdef ATEMRAS_SERVER 
static EC_T_DWORD RasNotifyWrapper(
                            EC_T_DWORD         dwCode, 
                            EC_T_NOTIFYPARMS*  pParms
                            )
{
    EC_T_DWORD                      dwRetVal                = EC_E_NOERROR;
    CEmNotification*                pNotInst                = EC_NULL;
    
    if ((EC_NULL == pParms)||(EC_NULL==pParms->pCallerData))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    pNotInst = (CEmNotification*)(pParms->pCallerData);
    dwRetVal = pNotInst->emRasNotify(dwCode, pParms);
Exit:
    
    return dwRetVal;
}
#endif

/*-MYAPP---------------------------------------------------------------------*/


/***************************************************************************************************/
/**
\brief  Initialize Application

\return EC_E_NOERROR on success, error code otherwise.
*/
static EC_T_DWORD myAppInit(
    CAtEmLogging*       poLog,          /* [in]  Logging instance */ 
    EC_T_INT            nVerbose        /* [in]  Verbosity level */
    )
{
    EC_UNREFPARM(poLog);
    EC_UNREFPARM(nVerbose);

    S_dwSlaveIdxTI437X	  = SLAVE_NOT_FOUND;
    S_dwSlaveIdxPc437x	  = SLAVE_NOT_FOUND ;
    S_dwSlaveIdxAc437x	  = SLAVE_NOT_FOUND ;

    OsMemset(S_aSlaveList, 0, DEMO_MAX_NUM_OF_SLAVES*sizeof(EC_T_CFG_SLAVE_INFO));
    OsMemset(&S_dwSlavePc437xCfgInfo , 0 , sizeof(EC_T_CFG_SLAVE_INFO));
    OsMemset(&S_dwSlaveAc437xCfginfo , 0 , sizeof(EC_T_CFG_SLAVE_INFO));

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Initialize Slave Instance.

Find slave parameters.
\return EC_E_NOERROR on success, error code otherwise.
*/
static EC_T_DWORD myAppPrepare(
    CAtEmLogging*       poLog,          /* [in]  Logging instance */ 
    EC_T_INT            nVerbose        /* [in]  Verbosity level */
    )
{
EC_T_WORD wFixedAddress = 0;
	EC_T_WORD nSlaves = 0;

    EC_UNREFPARM(nVerbose);


	/* Searching for: TI 437X Slaves                                                    */
	/* search for the first device at the bus and return its fixed (EtherCAT) address */
	LogMsg("");
	//for (nSlaves = 0; nSlaves < 2; nSlaves++)
	{
		LogMsg("Slave : 0x%x",nSlaves);
		if ( FindSlaveGetFixedAddr( INSTANCE_MASTER_DEFAULT
								  , poLog
								  , nSlaves
								  , ecvendor_texas_instruments
								  , ecprodcode_pc_437
								  , &wFixedAddress))
		{
			S_dwSlaveIdxPc437x 	= wFixedAddress ;
			SlaveIdPC			= S_dwSlaveIdxPc437x;
			LogMsg("TI Slave[%d] Pc found!", S_dwSlaveIdxPc437x);

			/* now get the offset of this device in the process data buffer and some other infos */
			if ( ecatGetCfgSlaveInfo( EC_TRUE
									, wFixedAddress
									, &S_dwSlavePc437xCfgInfo) != EC_E_NOERROR)
			{
				LogError("ERROR: ecatGetCfgSlaveInfo() returns with error.");
			}
		}
		
		if ( FindSlaveGetFixedAddr( INSTANCE_MASTER_DEFAULT
				  , poLog
				  , nSlaves
				  , ecvendor_texas_instruments
				  , ecprodcode_ac_437
				  , &wFixedAddress))
		{
				S_dwSlaveIdxAc437x 	= wFixedAddress ;
				SlaveIdAC 		   	= S_dwSlaveIdxAc437x;
				LogMsg("TI Slave[%d] Ac found!", S_dwSlaveIdxAc437x);

				/* now get the offset of this device in the process data buffer and some other infos */
				if ( ecatGetCfgSlaveInfo( EC_TRUE
							, wFixedAddress
							, &S_dwSlaveAc437xCfginfo) != EC_E_NOERROR)
				{
						LogError("ERROR: ecatGetCfgSlaveInfo() returns with error.");
				}
		}
	}

	LogMsg("");

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Setup slave parameters (normally done in PREOP state

  - SDO up- and Downloads
  - Read Object Dictionary

\return EC_E_NOERROR on success, error code otherwise.
*/
static EC_T_DWORD myAppSetup(
    CAtEmLogging*      poLog,           /* [in]  Logging instance */     
    EC_T_INT           nVerbose,        /* [in]  verbosity level */
    EC_T_DWORD         dwClntId         /* [in]  EtherCAT master client id */
    )
{
//    EC_T_DWORD           dwRes    = EC_E_ERROR;
//    EC_T_CFG_SLAVE_INFO* pMySlave = EC_NULL;


  //  EC_T_BYTE           bySlaveId = 0;


    // if (S_dwSlaveIdx4132 != SLAVE_NOT_FOUND)
//    if(S_dwSlaveIdxTI437X != SLAVE_NOT_FOUND)
//    {
//        for (bySlaveId = 0; bySlaveId < MAX_SLAVES; bySlaveId++)
//        {
//            /* Create Memory */
//            pbySlaveMbxData[bySlaveId]       = (EC_T_BYTE*)OsMalloc(COE_SDO_SLAVE_MBX_SIZE);
//            if(pMbxSdo == NULL)
//            {
//                goto Exit;
//            }
//            OsMemset(pbySlaveMbxData[bySlaveId],      0, COE_SDO_SLAVE_MBX_SIZE);
//
//            /* create required MBX Transfer Objects */
//            /* mailbox transfer object for OD list upload */
//            MbxTferDesc.dwMaxDataLen        = COE_SDO_SLAVE_MBX_SIZE;
//            MbxTferDesc.pbyMbxTferDescData  = pbySlaveMbxData[bySlaveId];
//
//            pMbxSdo[bySlaveId] = emMbxTferCreate(INSTANCE_MASTER_DEFAULT, &MbxTferDesc);
//            if (EC_NULL == pMbxSdo[bySlaveId])
//            {
//                dwRes = EC_E_NOMEMORY;
//                goto Exit;
//            }
//
//
//        }
//        /* now read object dict */
//        /* In a real application this is typically not necessary */
//        //dwRes = CoeReadObjectDictionary(INSTANCE_MASTER_DEFAULT, poLog, nVerbose, &bStopReading, dwClntId, pMySlave->dwSlaveId, EC_TRUE, MBX_TIMEOUT);
//    }
    return EC_E_NOERROR;


}


/***************************************************************************************************/
/**
\brief  demo application working process data function.

  This function is called in every cycle after the the master stack is started.
  
*/
static EC_T_DWORD myAppWorkpd(
    CAtEmLogging*       poLog,
    EC_T_INT            nVerbose,
    EC_T_BYTE*          pbyPDIn,        /* [in]  pointer to process data input buffer */
    EC_T_BYTE*          pbyPDOut        /* [in]  pointer to process data output buffer */
    )
{
	EC_T_CFG_SLAVE_INFO* pMySlave = EC_NULL;
	static EC_T_DWORD s_dwWorkpdCnt = 0;

	EC_T_BYTE pcTxPdoBuffer[TXPDO_INPUT_SIZE]	= {0x00};	/* PDO buffer */

	//EC_T_WORD wValue = 0;
	EC_T_DWORD dwRetVal = EC_E_NOERROR; /* Return value check           */
	EC_T_DWORD dwRes = EC_E_NOERROR; /* Return value check           */

	EC_T_WORD nSlaves = 0; /* Number of slaves             */
	EC_T_BYTE byRxPdoBuff[RXPDO_OUTPUT_SIZE];
	EC_T_BYTE bytxPDObuff[TXPDO_INPUT_SIZE];

	/* SDO download Request Test Parameters */
	EC_T_BOOL 	boolCompleteAccessVal 	= 0 ;
	EC_T_BYTE	bySubIndexParam 		= 0 ;
	EC_T_BYTE   byDataLength 			= 0 ;
	EC_T_DWORD  dwSubIndexOffset 		= 0 ;

	EC_T_CHAR						strTfrData[MAX_DATA_STRING_LENGTH];
	EC_T_CHAR						strTmp[MAX_BYTE_STRING_LENGTH] ;
	EC_T_BYTE 						index = 0 ;
	static EC_T_BYTE byNumPdoScenario = 0;
	static EC_T_BYTE byNumSdoScenario = 0 ;
	
	EC_UNREFPARM(poLog);

	/* process data are not modified not every cycle */
	s_dwWorkpdCnt++;

	if ((S_dwSlaveIdxPc437x != SLAVE_NOT_FOUND)
			&& (ecatGetMasterState() == eEcatState_OP)) 
	{
#ifdef TxPDO_SUPPORT
		/* Digital input slave available ? */
		if (pbyPDIn != EC_NULL) 
		{ 
			/* get slave information */
			pMySlave = &S_dwSlavePc437xCfgInfo;
			/* monitor input change if ENI was NOT generated with GenPreopENI */
			if (((EC_T_DWORD) -1) != pMySlave->dwPdOffsIn) 
			{
				UINT32 pos = 0;
				memset(bytxPDObuff,0x00,sizeof(bytxPDObuff));
				EC_GETBITS(pbyPDIn, bytxPDObuff, pMySlave->dwPdOffsIn , pMySlave->dwPdSizeIn);
				//OsMemcpy(&PC_TxPdo, bytxPDObuff, TXPDO_INPUT_SIZE );
				
				memcpy(&PC_TxPdo.Amplitude,bytxPDObuff,sizeof(PC_TxPdo.Amplitude));
				pos += sizeof(PC_TxPdo.Amplitude);
				memcpy(&PC_TxPdo.Phase,bytxPDObuff + pos,sizeof(PC_TxPdo.Phase));
				pos += sizeof(PC_TxPdo.Phase);
				memcpy(&PC_TxPdo.Power,bytxPDObuff + pos,sizeof(PC_TxPdo.Power));
				pos += sizeof(PC_TxPdo.Power);
				memcpy(&PC_TxPdo.Frequency,bytxPDObuff + pos,sizeof(PC_TxPdo.Frequency));
				pos += sizeof(PC_TxPdo.Frequency);
				memcpy(&PC_TxPdo.Current,bytxPDObuff + pos,sizeof(PC_TxPdo.Current));
				pos += sizeof(PC_TxPdo.Current);
				memcpy(&PC_TxPdo.PC_StatusEvent,bytxPDObuff + pos,sizeof(PC_TxPdo.PC_StatusEvent));
				pos += sizeof(PC_TxPdo.PC_StatusEvent);
				memcpy(&PC_TxPdo.PCState,bytxPDObuff + pos,sizeof(PC_TxPdo.PCState));
			
				memcpy(PCTX,&PC_TxPdo,sizeof(PC_TxPdo)); /* Copying to shared data(virtual address) */
							
#ifdef PRINT
				if((PcTxPdo_Diff()) && (nVerbose > 2))
				{
					logMsg("-------------TxPDO from PC------------",0,0,0,0,0,0);
					logMsg("PC Frequency	: %d",PC_TxPdo.Frequency,0,0,0,0,0);
					logMsg("PC Amplitude	: %d",PC_TxPdo.Amplitude,0,0,0,0,0);
					logMsg("PC Phase 		: %d",PC_TxPdo.Phase,0,0,0,0,0);
					logMsg("PC Power 		: %d",PC_TxPdo.Power,0,0,0,0,0);
					logMsg("PC PCState 	: %d",PC_TxPdo.PCState,0,0,0,0,0);
					logMsg("--------------------------------------\n",0,0,0,0,0,0);
				}
#endif
			} /* End of If dwPdOffsIn */

		} /* End of if on Slave check */
#else
#endif
#ifdef RxPDO_SUPPORT
		/* Send RxPDO to each slave */
		if (pbyPDOut != EC_NULL) 
		{
			/* get slave information */
			pMySlave = &S_dwSlavePc437xCfgInfo;
			/* flash output if ENI was NOT generated with GenPreopENI */
			if (((EC_T_DWORD) -1) != pMySlave->dwPdOffsOut) 
			{
				UINT32 idx = 0;
				
				/* Copying from shared data(virtual address) */
				
				//OsMemcpy(&PC_RxPdo,PCRX,sizeof(&PC_RxPdo));
				if(ERROR != taskNameToId(CTRL_TASK_NAME))
				{
					PC_RxPdo.MasterState     = PCRX->MasterState;
					PC_RxPdo.Amplitudechange = PCRX->Amplitudechange;
					PC_RxPdo.MasterEvents    = PCRX->MasterEvents;
					PC_RxPdo.Frequency 		 = PCRX->Frequency;
				}
				else
				{
					PC_RxPdo.MasterState     = SC_UNREACHABLE;
					PC_RxPdo.Amplitudechange = 0;
					PC_RxPdo.MasterEvents    = 0;
					PC_RxPdo.Frequency 		 = 0;
				}
				
				memcpy(byRxPdoBuff + idx, &PC_RxPdo.MasterState, sizeof(PC_RxPdo.MasterState));
				idx += sizeof(PC_RxPdo.MasterState);
				memcpy(byRxPdoBuff + idx, &PC_RxPdo.Amplitudechange, sizeof(PC_RxPdo.Amplitudechange));
				idx += sizeof(PC_RxPdo.Amplitudechange);
				memcpy(byRxPdoBuff + idx, &PC_RxPdo.MasterEvents, sizeof(PC_RxPdo.MasterEvents));
				idx += sizeof(PC_RxPdo.MasterEvents);
				memcpy(byRxPdoBuff + idx, &PC_RxPdo.Frequency, sizeof(PC_RxPdo.Frequency));
				EC_SETBITS(pbyPDOut, byRxPdoBuff, pMySlave->dwPdOffsOut, pMySlave->dwPdSizeOut);

#ifdef PRINT
				if((PcRxPdo_Diff()) && (nVerbose > 2))
				{
					logMsg("-------------RxPDO Sent for PC--------------\n",0,0,0,0,0,0);
					logMsg("PC MasterState		: %d\n",PC_RxPdo.MasterState,0,0,0,0,0);
					logMsg("PC Master Event 		: %d\n",PC_RxPdo.MasterEvents,0,0,0,0,0);
					logMsg("PC Amplitude 		: %d\n",PC_RxPdo.Amplitudechange,0,0,0,0,0);
					logMsg("PC Frequency 		: %d\n",PC_RxPdo.Frequency,0,0,0,0,0);
					logMsg("--------------------------------------------\n",0,0,0,0,0,0);
				}
#endif
			}
		}
#else
#endif
	}

	if ((S_dwSlaveIdxAc437x != SLAVE_NOT_FOUND)
			&& (ecatGetMasterState() == eEcatState_OP))
	{
#ifdef TxPDO_SUPPORT
		/* Digital input slave available ? */
		if (pbyPDIn != EC_NULL) 
		{
			/* get slave information */
			pMySlave = &S_dwSlaveAc437xCfginfo;
			if (((EC_T_DWORD) -1) != pMySlave->dwPdOffsIn) 
			{
				UINT32 pos = 0;
				memset(bytxPDObuff,0x00,sizeof(bytxPDObuff));
				EC_GETBITS(pbyPDIn, bytxPDObuff, pMySlave->dwPdOffsIn , pMySlave->dwPdSizeIn);
				//OsMemcpy(&AC_TxPdo, bytxPDObuff, TXPDO_INPUT_SIZE );
				
				memcpy(&AC_TxPdo.ActualForce,bytxPDObuff,sizeof(AC_TxPdo.ActualForce));
				pos += sizeof(AC_TxPdo.ActualForce);
				memcpy(&AC_TxPdo.ActualDistance,bytxPDObuff + pos,sizeof(AC_TxPdo.ActualDistance));
				pos += sizeof(AC_TxPdo.ActualDistance);
				memcpy(&AC_TxPdo.ACStatusEvent,bytxPDObuff + pos,sizeof(AC_TxPdo.ACStatusEvent));
				pos += sizeof(AC_TxPdo.ACStatusEvent);
				memcpy(&AC_TxPdo.ACState,bytxPDObuff + pos,sizeof(AC_TxPdo.ACState));
				pos += sizeof(AC_TxPdo.ACState);
				memcpy(&AC_TxPdo.ACInputs,bytxPDObuff + pos,sizeof(AC_TxPdo.ACInputs));
				
				memcpy(ACTX,&AC_TxPdo,sizeof(AC_TxPdo)); /* Copying to shared data(virtual address) */
#ifdef PRINT
				if((AcTxPdo_Diff()) && (nVerbose > 2))
				{
					logMsg("---------------TxPDO from AC---------------",0,0,0,0,0,0);
					logMsg("AC ActualForce		: %d",AC_TxPdo.ActualForce,0,0,0,0,0);
					logMsg("AC ActualDistance 	: %d",AC_TxPdo.ActualDistance,0,0,0,0,0);
					logMsg("AC StateEvent 		: %d",AC_TxPdo.ACStatusEvent,0,0,0,0,0);
					logMsg("AC State 		: %d",AC_TxPdo.ACState,0,0,0,0,0);
					logMsg("AC Inputs 		: %d",AC_TxPdo.ACInputs,0,0,0,0,0);
					logMsg("--------------------------------------------\n",0,0,0,0,0,0);
				}
#endif
			} /* End of If dwPdOffsIn */
		} /* End of if on Slave check */
#else
#endif
#ifdef RxPDO_SUPPORT
		/* Send RxPDO to each slave */
		if (pbyPDOut != EC_NULL) 
		{
			/* get slave information */
			pMySlave = &S_dwSlaveAc437xCfginfo;
			/* flash output if ENI was NOT generated with GenPreopENI */
			if (((EC_T_DWORD) -1) != pMySlave->dwPdOffsOut)
			{
			
			/* Copying from shared data(virtual address) */
				if(ERROR != taskNameToId(CTRL_TASK_NAME))
				{
					OsMemcpy(&AC_RxPdo,ACRX,sizeof(RxPDO_AC));
				}
				else
				{
					AC_RxPdo.ACControlEvent = 0;
					AC_RxPdo.ACOutputs		= 0;
					AC_RxPdo.SCState		= SC_UNREACHABLE;
				}
				
				OsMemcpy(byRxPdoBuff, &AC_RxPdo, RXPDO_OUTPUT_SIZE);
				EC_SETBITS(pbyPDOut, byRxPdoBuff, pMySlave->dwPdOffsOut, pMySlave->dwPdSizeOut);
				
				//logMsg("AC Control Event 	: %u\n",AC_RxPdo.ACControlEvent,0,0,0,0,0);
				
#ifdef PRINT
				if((AcRxPdo_Diff()) && (nVerbose > 2))
				{
					logMsg("----------------RxPDO Sent for AC---------------",0,0,0,0,0,0);
					logMsg("AC SCState		: %d",AC_RxPdo.SCState,0,0,0,0,0);
					logMsg("AC Outputs 		: %d",AC_RxPdo.ACOutputs,0,0,0,0,0);
					logMsg("AC Control Event 	: %u\n",AC_RxPdo.ACControlEvent,0,0,0,0,0);
					logMsg("------------------------------------------------\n",0,0,0,0,0,0);
				}
#endif
			}
		}
#else
#endif
	}
	byNumPdoScenario++ ;
	byNumPdoScenario %= NUM_PDO_SCENARIOS;
	return EC_E_NOERROR;

	Exit:
	return dwRetVal;
}


#if 0
/***************************************************************************************************/
/**
\brief  demo application doing some diagnostic tasks

  This function is called in sometimes from the main demo task
*/
static EC_T_DWORD myAppDiagnosis(
    CAtEmLogging*       poLog,          /* [in]  Logging instance */ 
    EC_T_INT            nVerbose        /* [in]  Verbosity level */
    )
{
    EC_T_DWORD           dwRes    = EC_E_ERROR;
    EC_T_CFG_SLAVE_INFO* pMySlave = EC_NULL;

    EC_UNREFPARM(poLog);
    EC_UNREFPARM(nVerbose);

    if (S_dwSlaveIdx4132 != SLAVE_NOT_FOUND)
    {
        EC_T_DWORD dwSize  = 0;
        EC_T_DWORD dwValue = 0;

        pMySlave = &S_aSlaveList[S_dwSlaveIdx4132];    

        dwRes = ecatCoeSdoUpload(pMySlave->dwSlaveId, 0x1018, 1,
            (EC_T_BYTE*)&dwValue, sizeof(EC_T_DWORD), &dwSize, MBX_TIMEOUT, 0);

        if (dwRes != EC_E_NOERROR)
        {
            LogError("myAppDiagnosis: error in COE SDO Upload of object 0x1018! %s (0x%x)", ecatGetText(dwRes), dwRes);
        }
    }

    return EC_E_NOERROR;
}
#endif

/********************************************************************************/
/** \brief  Gets the Offset of a Subindex of an Object
*
*  !
*
* \return  Offset value.
*/
EC_T_DWORD  getObjectSubIndexOffset(EC_T_DWORD dwODIndex, EC_T_DWORD dwODSubIndex)
{
    EC_T_DWORD   dwRes = EC_E_NOERROR;
        {
            dwRes = masterODList.byIndexOffset[dwODSubIndex];
        }

    return dwRes;
}
/****/
/** \brief  Api for SDO upload service for CoE
*
*  Takes care of Mailbox creation, initialization sending SDO upload to a requested Slave id
*  dwAccess = 0 (For complete access of SubIndices) accompanied with dwODSubIndex 0 or 1
*
* \return  Status value.
 */
EC_T_DWORD CoeSdoUploadReq(
        EC_T_WORD dwODIndex,
        EC_T_WORD dwODSubIndex,
        EC_T_WORD wSlaveInstance,
		EC_T_DWORD dwLength,
        EC_T_DWORD dwAccess,
        EC_T_WORD dwClientId
        )
{
    EC_T_DWORD      dwRetVal           = EC_E_NOERROR;
    EC_T_WORD		SlaveId				= EC_E_NOERROR ;
    EC_T_MBXTFER*   pSlaveMbxTfer      = EC_NULL;
    EC_T_DWORD      dwRes              = EC_E_NOERROR;
    EC_T_MBXTFER*   pMbxSdo            = EC_NULL;                /* mailbox transfer object for SDO Upload */
    EC_T_BYTE*      pbySlaveMbxData    = EC_NULL;        /* mailbox transfer Data for SDO upload   */
    EC_T_MBXTFER_DESC   MbxTferDesc             = {0};          /* mailbox transfer descriptor */
    EC_T_CFG_SLAVE_INFO* pMySlave = EC_NULL;
    EC_T_DWORD 			dwTmpTransfrId = 0 ;

    if((INSTANCE_PC_MODULE == wSlaveInstance) && (SLAVE_NOT_FOUND != S_dwSlaveIdxPc437x))
    {
    	pMySlave = &S_dwSlavePc437xCfgInfo ;
    }
    else if((INSTANCE_AC_MODULE == wSlaveInstance) && (SLAVE_NOT_FOUND != S_dwSlaveIdxAc437x))
    {
    	pMySlave = &S_dwSlaveAc437xCfginfo ;
    }
    else
    {
    	goto Exit ;
    }
    
    pbySlaveMbxData       = (EC_T_BYTE*)OsMalloc(dwLength);
    if(pbySlaveMbxData == NULL)
    {
        goto Exit;
    }
    OsMemset(pbySlaveMbxData,  0, dwLength);

    /* create required MBX Transfer Objects */

    MbxTferDesc.dwMaxDataLen        = dwLength;
    MbxTferDesc.pbyMbxTferDescData  = pbySlaveMbxData;

    pMbxSdo = emMbxTferCreate(INSTANCE_MASTER_DEFAULT, &MbxTferDesc);
    if (EC_NULL == pMbxSdo)
    {
        dwRes = EC_E_NOMEMORY;
        goto Exit;
    }


    /* get slave information */
    //pMySlave = &S_aSlaveList[SlaveId];

    /* SDO Upload request */
    pSlaveMbxTfer = pMbxSdo;
    pSlaveMbxTfer->dwClntId    = dwClientId; // GetClientID;
    /*	Transfer ID = Complete Access + Sub Index + Slave ID + unique Transfer ID counter */
    dwTmpTransfrId = S_wUniqTransferId++;
    if(1 == dwAccess)
    {
    	dwTmpTransfrId |= TFR_ID_ACCESS_MASK ;
    }

    pSlaveMbxTfer->dwTferId    = dwTmpTransfrId ;
    pSlaveMbxTfer->dwDataLen   = pSlaveMbxTfer->MbxTferDesc.dwMaxDataLen;
 
    //cout << "ecatCoe : ---------- dataLen of MBX : " << pSlaveMbxTfer->dwDataLen << endl;
    
    //LogMsg("\nSlaveId : 0x%x, Index : 0x%x, SubIndex : 0x%x, Access : 0x%x, Client Id : 0x%x, UniqueTransferId : 0x%x\n",pMySlave->dwSlaveId, dwODIndex, dwODSubIndex, dwAccess, dwClientId, S_wUniqTransferId);
    dwRes = ecatCoeSdoUploadReq(pSlaveMbxTfer, pMySlave->dwSlaveId , dwODIndex, dwODSubIndex, MBX_TIMEOUT,dwAccess); /* Complete Access */

    if (dwRes != EC_E_NOERROR) {
        LogMsg("SdoUpload failed with Error Code - %0x%lx", dwRes);
        dwRetVal = dwRes;
        goto Exit;
    }


Exit:
    return dwRetVal;

}
/****/
/** \brief  Function internally used to determine dataLength for SDO download request from
 * SubIndex entered in SDO tests.
*
*
* \return  DataLength.
 */
static EC_T_BYTE CoeSdoSubIndexToDataLength(EC_T_BYTE byValSubIndex)
{
	EC_T_BYTE byValDataLength = 0xFF ;

	switch(byValSubIndex)
	            		{
	            		case EL437x_SUBINDEX_ENTRIES:
	            			byValDataLength = EL437x_SUBINDEX_ENTRIES_SIZE ;
	            			break ;
	            		case EL437x_SUBINDEX_WELD_DELAY:
	            			byValDataLength = EL437x_SUBINDEX_WELD_DELAY_SIZE ;
	            			break;
	            		case EL437x_SUBINDEX_WELD_TIME:
	            			byValDataLength = EL437x_SUBINDEX_WELD_TIME_SIZE ;
	            			break;
	            		case EL437x_SUBINDEX_INTENSITY:
	            			byValDataLength = EL437x_SUBINDEX_INTENSITY_SIZE ;
	            			break;
	            		case EL437x_SUBINDEX_CTRL_RECIPE_OVERRIDES:
	            		    byValDataLength = EL437x_SUBINDEX_CTRL_RECIPE_OVERRIDES_SIZE ;
	            		    break;
	            		case EL437x_SUBINDEX_INTENSITY_FACTOR:
	            		    byValDataLength = EL437x_SUBINDEX_INTENSITY_FACTOR_SIZE ;
	            		   break;
	            		case EL437x_SUBINDEX_PART_REFLECTIVITY_FACTOR:
	            		    byValDataLength = EL437x_SUBINDEX_PART_REFLECTIVITY_FACTOR_SIZE ;
	            		    break;
	            		case EL437x_SUBINDEX_TOOL_REFLECTIVITY_FACTOR:
	            		    byValDataLength = EL437x_SUBINDEX_TOOL_REFLECTIVITY_FACTOR_SIZE ;
	            		    break;
	            		default :
	            			byValDataLength = 0xFF;
	            			break ;
	            		}
	return byValDataLength ;
}

/****/
/** \brief  Api for SDO Download service for CoE
*
*  Takes care of Mailbox creation, initialization sending SDO upload to a requested Slave id
*  dwAccess = 0 (For complete access of SubIndices) accompanied with dwODSubIndex 0 or 1
*
* \return  Status value.
 */
EC_T_DWORD CoeSdoDownloadReq(
        EC_T_WORD dwODIndex,
        EC_T_WORD dwODSubIndex,
        EC_T_WORD wSlaveInstance,
        EC_T_BYTE dwAccess,
        EC_T_WORD wdataLength,
        EC_T_WORD dwClientId,
		EC_T_BYTE *pSdoData
        )
{
    EC_T_DWORD      dwRetVal           = EC_E_NOERROR;
    EC_T_MBXTFER*   pSlaveMbxTfer      = EC_NULL;
    EC_T_DWORD      dwRes              = EC_E_NOERROR;
    EC_T_MBXTFER*   pMbxSdo            = EC_NULL;                /* mailbox transfer object for SDO Upload */
    EC_T_BYTE*      pbySlaveMbxData    = EC_NULL;                /* mailbox transfer Data for SDO upload   */
    EC_T_MBXTFER_DESC   MbxTferDesc             = {0};           /* mailbox transfer descriptor */
    EC_T_CFG_SLAVE_INFO* pMySlave = EC_NULL;
    EC_T_DWORD      dwSubIndexOffset     = 0;
    EC_T_DWORD		dwTmpTransferId = 0 ;


    if((INSTANCE_PC_MODULE == wSlaveInstance) && (SLAVE_NOT_FOUND != S_dwSlaveIdxPc437x))
    {
        pMySlave = &S_dwSlavePc437xCfgInfo ;
     }
     else if((INSTANCE_AC_MODULE == wSlaveInstance) && (SLAVE_NOT_FOUND != S_dwSlaveIdxAc437x))
     {
        pMySlave = &S_dwSlaveAc437xCfginfo ;
     }
     else
     {
    	 goto Exit ;
     }

    
    pbySlaveMbxData       = (EC_T_BYTE*)OsMalloc(wdataLength);
    if(pbySlaveMbxData == NULL)
    {
        goto Exit;
    }
    OsMemset(pbySlaveMbxData,  0, wdataLength);

    /* create required MBX Transfer Objects */


    MbxTferDesc.dwMaxDataLen        = wdataLength;
    MbxTferDesc.pbyMbxTferDescData  = pbySlaveMbxData;

    pMbxSdo = emMbxTferCreate(INSTANCE_MASTER_DEFAULT, &MbxTferDesc);
    if (EC_NULL == pMbxSdo)
    {
        dwRes = EC_E_NOMEMORY;
        goto Exit;
    }

    dwSubIndexOffset = getObjectSubIndexOffset(dwODIndex, dwODSubIndex);
    OsMemcpy(pbySlaveMbxData, pSdoData , wdataLength);
    /* SDO Upload request */
    pSlaveMbxTfer = pMbxSdo;
    pSlaveMbxTfer->dwClntId    = dwClientId; // GetClientID;
    /*	Transfer ID = Complete Access + Sub Index + Slave ID + unique Transfer ID counter */
    dwTmpTransferId = S_wUniqTransferId++;
    pSlaveMbxTfer->dwTferId    = dwTmpTransferId ;

    pSlaveMbxTfer->dwDataLen   = pSlaveMbxTfer->MbxTferDesc.dwMaxDataLen;

    //printf("\nSlaveId : 0x%x, Index : 0x%x, SubIndex : 0x%x, Access : 0x%x, Client Id : 0x%x, UniqueTransferId : 0x%x\n",pMySlave->dwSlaveId, dwODIndex, dwODSubIndex, dwAccess, dwClientId, S_wUniqTransferId);
    
    //LogMsg("\nSDO download request- Slave(0x%x): %s, Index: 0x%x, SubIndex: 0x%x, Access: 0x%x :",pMySlave->wStationAddress, ((INSTANCE_PC_MODULE == wSlaveInstance)?"PC":"AC"), dwODIndex, dwODSubIndex, dwAccess);
    
    /*for(EC_T_BYTE index = 0 ; index < pSlaveMbxTfer->dwDataLen;index++)
    {
    	printf("0x%x   ",pSdoData[index]);
    }
    printf("\n");
    */
    
    dwRes = ecatCoeSdoDownloadReq(pSlaveMbxTfer, pMySlave->dwSlaveId, dwODIndex, dwODSubIndex,MBX_TIMEOUT,dwAccess); /* Complete Access */

    if (dwRes != EC_E_NOERROR) {
        LogMsg("Sdo Download failed with Error Code - %d", dwRes);
        dwRetVal = dwRes;
        goto Exit;
    }

    return dwRes;


Exit:
    return dwRetVal;

}

/****/
/** \brief  Api for FoE Download 
 *
 *  Set Master State to INIT and Slave to BOOTSTRAP
 *  Download the Firmware files to Slaves
 *  Set Master and slave to OP-State
 * 
 * \return  Status value.
 */

EC_T_DWORD FoeDownloadReq(
		EC_T_WORD wSlaveInstance,
		EC_T_WORD dwFileNameLen, 
		EC_T_CHAR *szFileName, 
		EC_T_DWORD dwDataLen, 
		EC_T_BYTE *pbyData
)
{
	EC_T_DWORD      dwRetVal           = EC_E_NOERROR;
	EC_T_DWORD      dwRes              = EC_E_NOERROR;
	EC_T_CFG_SLAVE_INFO* pMySlave 	   = EC_NULL;
	EC_T_BYTE*      pbySlaveMbxData    = EC_NULL; 


	//Get the Slave Configuration using Slave Instance
	if((INSTANCE_PC_MODULE == wSlaveInstance) && (SLAVE_NOT_FOUND != S_dwSlaveIdxPc437x))
	{
		pMySlave = &S_dwSlavePc437xCfgInfo ;
	}
	else if((INSTANCE_AC_MODULE == wSlaveInstance) && (SLAVE_NOT_FOUND != S_dwSlaveIdxAc437x))
	{
		pMySlave = &S_dwSlaveAc437xCfginfo ;
	}
	else
	{
		dwRetVal = EC_E_ERROR;
		goto Exit ;
	}
	
	//Set the Master State to INIT
	dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_INIT);

	if (dwRes != EC_E_NOERROR)
	{
		dwRetVal = dwRes;
		goto Exit;
	}

	//Set the Slave State to BOOTSTRAP
	dwRes = ecatSetSlaveState(pMySlave->dwSlaveId, DEVICE_STATE_BOOTSTRAP, ETHERCAT_STATE_CHANGE_TIMEOUT);

	if (dwRes != EC_E_NOERROR)
	{
		dwRetVal = dwRes;
		goto Exit;
	}

	//Copy the file data dynamically
	pbySlaveMbxData = (EC_T_BYTE*)OsMalloc(dwDataLen);
	if(pbySlaveMbxData == NULL)
	{
		dwRetVal = EC_E_NOMEMORY;
		goto Exit;
	}
	
	OsMemset(pbySlaveMbxData,  0x00, dwDataLen);
	
	OsMemcpy(pbySlaveMbxData, pbyData , dwDataLen);
	
	LogMsg("\nFoE download started...");
	//Call FoE Download 
	dwRes = ecatFoeFileDownload(pMySlave->dwSlaveId, szFileName , dwFileNameLen, pbySlaveMbxData, dwDataLen, 0, FOE_TRANSFER_TIMEOUT);

	//free the Allocated memory
	SafeOsFree(pbySlaveMbxData);

	if (dwRes != EC_E_NOERROR)
	{
		LogMsg("\nFoE download Failure (0x%08X)!", dwRes);
		dwRetVal = dwRes;
		goto Exit;
	}
	else if ( dwRes == EC_E_NOERROR )
	{
		LogMsg("\nFoE download done (0x%08X)!", dwRes);
		
		//Set the Master to OP-state
		dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_OP);

		if (dwRes != EC_E_NOERROR)
		{
			dwRetVal = dwRes;
			goto Exit;
		}
		
		//Set the Slave to OP-state
		dwRes = ecatSetSlaveState(pMySlave->dwSlaveId, DEVICE_STATE_OP, ETHERCAT_STATE_CHANGE_TIMEOUT);
		if (dwRes != EC_E_NOERROR)
		{
			dwRetVal = dwRes;
			goto Exit;
		}		
	}
	
	return dwRes;

	Exit:
	return dwRetVal;

}


/********************************************************************************/
/** \brief  Handler for application notifications
 *
 *  !!! No blocking API shall be called within this function!!!
 *  !
 *
 * \return  Status value.
 */
/********************************************************************************/
/** \brief  Handler for application notifications
*
*  !!! No blocking API shall be called within this function!!! 
*  !!! Function is called by cylic task                    !!! 
*
* \return  Status value.
*/
static EC_T_DWORD myAppNotify(
    EC_T_DWORD              dwCode,     /* [in]  Application notification code */
    EC_T_NOTIFYPARMS*       pParms      /* [in]  Notification parameters */
    )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    EC_UNREFPARM(pParms);

    /* dispatch notification code */
    switch(dwCode)
    {
    case 1:
        /* dwRetVal = EC_E_NOERROR; */
        break;
    case 2:
        break;
    default:
        break;
    }

    return dwRetVal;
}

/*-END OF SOURCE FILE--------------------------------------------------------*/
