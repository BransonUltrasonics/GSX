/*-----------------------------------------------------------------------------
 * SCInit.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT Master demo header
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
//#include "ATEMDemoConfig.h"
#include "SCConfig.h"
#include "ecatNotification.h"
#include "ecatDemoCommon.h"
#include "Common.h"
#ifdef VXWORKS
#include "wvLib.h"
#endif

/*-MACROS--------------------------------------------------------------------*/

/*-DEFINES-------------------------------------------------------------------*/
#define MAX_LINKLAYER 5

#if (defined EC_SOCKET_SUPPORTED)
/* the RAS server is necessary to support the EC-Engineer or other remote applications */
  #define ATEMRAS_SERVER
#endif
#define REMOTE_WD_TO_LIMIT          10000
#define REMOTE_CYCLE_TIME           2
#define MAX_OBJECT_INDEX            2

#define INSTANCE_PC_MODULE          0 
#define INSTANCE_AC_MODULE			1


/*-DEFINES-------------------------------------------------------------------*/

#define MBX_TIMEOUT                         10000

#define DEMO_MAX_NUM_OF_SLAVES  6
#define SLAVE_NOT_FOUND         0xFFFF


/* Global object for PDO DATA*/
TxPDO_PC PC_TxPdo,PrevPC_TxPdo,*PCTX;
TxPDO_AC AC_TxPdo,PrevAC_TxPdo,*ACTX;

RxPDO_PC PC_RxPdo,PrevPC_RxPdo,*PCRX;
RxPDO_AC AC_RxPdo,PrevAC_RxPdo,*ACRX;


/*-FORWARD DECLARATIONS------------------------------------------------------*/
//EC_T_DWORD ATEMDemo(
EC_T_DWORD SC_MAIN(
     CAtEmLogging*       poLog
    ,EC_T_CNF_TYPE       eCnfType
    ,EC_T_PBYTE          pbyCnfData
    ,EC_T_DWORD          dwCnfDataLen
    ,EC_T_DWORD          dwBusCycleTimeUsec
    ,EC_T_INT            nVerbose
    ,EC_T_DWORD          dwDuration
    ,EC_T_LINK_PARMS*    poLinkParms
    ,EC_T_VOID*          pvTimingEvent
    ,EC_T_DWORD          dwCpuIndex
    ,EC_T_BOOL           bEnaPerfJobs
#ifdef ATEMRAS_SERVER 
    ,EC_T_WORD           wServerPort
#endif
    ,EC_T_LINK_PARMS* poLinkParmsRed
    );

bool PcTxPdo_Diff();
bool AcTxPdo_Diff();
bool AcRxPdo_Diff();
bool PcRxPdo_Diff();

void GetSD_PDO();
void ProcessSdoMessage();

EC_T_DWORD ReInitEcMasterStack(
	CEmNotification* pNotification
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
);


/*--------------------------------------------------------------------------*/
/* Performance measurements of jobs                                         */
/* This is only available on CPUs with TSC support                          */
/*--------------------------------------------------------------------------*/

#define JOB_ProcessAllRxFrames  		0
#define JOB_SendAllCycFrames    		1
#define JOB_MasterTimer         		2
#define JOB_SendAcycFrames      		3
#define PERF_CycleTime          		4
#define PERF_myAppWorkpd                5
#define MAX_JOB_NUM             		6

#define PERF_MEASURE_JOBS_INIT(msgcb)   ecatPerfMeasInit(&S_TscMeasDesc,0,MAX_JOB_NUM,msgcb);ecatPerfMeasEnable(&S_TscMeasDesc)
#define PERF_MEASURE_JOBS_DEINIT()      ecatPerfMeasDeinit(&S_TscMeasDesc)
#define PERF_MEASURE_JOBS_SHOW()        ecatPerfMeasShow(&S_TscMeasDesc,0xFFFFFFFF,S_aszMeasInfo)
#define PERF_JOB_START(nJobIndex)       ecatPerfMeasStart(&S_TscMeasDesc,(EC_T_DWORD)(nJobIndex))
#define PERF_JOB_END(nJobIndex)         ecatPerfMeasEnd(&S_TscMeasDesc,(EC_T_DWORD)(nJobIndex))

/*---------------------------------------------------------------------------- */
/* CoESdoUploadReq
 * Purpose : Send Sdo upload Request to slave */
/*---------------------------------------------------------------------------- */
EC_T_DWORD CoeSdoUploadReq(EC_T_WORD ODIndex,EC_T_WORD ODSubIndex,EC_T_WORD SlaveInstance , EC_T_DWORD dwLength , EC_T_DWORD dwAccess,EC_T_WORD ClientId);

EC_T_DWORD CoeSdoDownloadReq(EC_T_WORD ODIndex,EC_T_WORD ODSubIndex,EC_T_WORD SlaveInstance,EC_T_BYTE dwAccess, EC_T_WORD wdataLength,EC_T_WORD ClientId, EC_T_BYTE* pSdoData);

/*---------------------------------------------------------------------------- */
/* FoEDownloadReq
 * Purpose : Send FoE Download Request to slave */
/*---------------------------------------------------------------------------- */
EC_T_DWORD FoeDownloadReq(EC_T_WORD wSlaveInstance, EC_T_WORD dwFileNameLen, EC_T_CHAR *szFileName, EC_T_DWORD dwDataLen, EC_T_BYTE *pbyData);

/*---------------------------------------------------------------------------- */
/* EcatGetBusScanTimeoutSysCall
 * Purpose : Read Bus scan failure count */
/*---------------------------------------------------------------------------- */
UINT32 EcatGetBusScanTimeoutSysCall();

/*-END OF SOURCE FILE--------------------------------------------------------*/


