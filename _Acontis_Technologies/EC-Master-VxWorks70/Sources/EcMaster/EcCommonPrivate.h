/*-----------------------------------------------------------------------------
 * EcCommonPrivate.h               
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              Common private header shared by all AT-EM layers.
 *---------------------------------------------------------------------------*/

#ifndef INC_ECCOMMONPRIVATE
#define INC_ECCOMMONPRIVATE

/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ECTYPE
  #include "EcType.h"
#endif
#ifndef INC_ECERROR
  #include "EcError.h"
#endif
#ifndef INC_ECOS
  #include "EcOs.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*-DEFINES-------------------------------------------------------------------*/
#define MAX_QUEUED_ETH_FRAMES               127     /* more than this amount of frames cannot be queued due to IDX number limitation! */
#define DEFAULT_PHYSICAL_MBX_POLLING_TIME    10     /* default per slave physical mailbox polling */
#define SLAVE_STATE_UPDATE_TIMEOUT         1000     /* timeout waiting for slave state update (AL_STATUS command request result) */
#define SLAVE_ERROR_ACK_STATUS_TIMEOUT     1000     /* timeout waiting for slave error acknowledge status */
#define EC_INITCMDRETRYTIMEOUT               10     /* timeout after which retry value in InitCmds is decremented */

#define IS_CLIENT_RAW_CMD            0x80000000     /* marker to signal that this is a client specific raw command */

#define EC_MBX_DEFAULT_TIMEOUT       0x80002710     /* 10000 if Mbx cmd is commited with no_wait use this
                                                     * timeout in msecs for internal handling
                                                     * The highest bit 0x80000000 is set to recognize the 
                                                     * default timeout using IsDefaultTimeout()
                                                     */

/*-TYPEDEFS------------------------------------------------------------------*/
typedef struct _EC_T_MEMORY_LOGGER
{
    size_t      nCurrUsage;         /* current dynamic memory usage */
    size_t      nMaxUsage;          /* maximum dynamic memory usage */
#ifdef DEBUGTRACE
    size_t*     anUsage;            /* dynamic memory usage array */
    EC_T_DWORD  dwUsageCurrIndex;   /* index into dynamic memory usage array */
    EC_T_VOID*  pMemTable;          /* storage of MemTable addresses 
                                       real type is CHashTableDyn<EC_T_UINT64, size_t>* 
                                       this struct is used in C code */
#endif /* DEBUGTRACE */
} EC_T_MEMORY_LOGGER;

/*-MACROS--------------------------------------------------------------------*/

#define TESTBIT(evt, probe) (((EC_T_WORD)probe)&(1<<((EC_T_BYTE)evt)))

EC_T_VOID EcMemLogFree(EC_T_MEMORY_LOGGER* pLog);

/* debug tracing */
#ifdef DEBUGTRACE

EC_T_VOID EcMemLogAddMem(EC_T_MEMORY_LOGGER* pLog, EC_T_DWORD dwMask, char* szLoc, EC_T_ADDRESS dwAddress, size_t nSize);
EC_T_VOID EcMemLogSubMem(EC_T_MEMORY_LOGGER* pLog, EC_T_DWORD dwMask, char* szLoc, EC_T_ADDRESS dwAddress, size_t nSize);

extern EC_T_DWORD G_dwTraceMask;    /* trace mask */
extern EC_T_DWORD G_dwMemTraceMask; /* memory trace mask */

#define EC_TRACEMSG(Mask, Msg) \
            ((void)((((Mask) & G_dwTraceMask) != 0)?(OsTrcMsg Msg),1:0))

#define EC_TRACE_ADDMEM_LOG(Mask, pLogger, szLoc, dwAddress, dwSize) \
            EcMemLogAddMem(pLogger,(Mask),(szLoc),(EC_T_ADDRESS)(dwAddress),(dwSize))

#define EC_TRACE_SUBMEM_LOG(Mask, pLogger, szLoc, dwAddress, dwSize) \
            EcMemLogSubMem(pLogger,(Mask),(szLoc),(EC_T_ADDRESS)(dwAddress), (dwSize))

#else  /* DEBUGTRACE */

EC_T_VOID EcMemLogAddMem(EC_T_MEMORY_LOGGER* pLog, size_t nSize);
EC_T_VOID EcMemLogSubMem(EC_T_MEMORY_LOGGER* pLog, size_t nSize);

#define EC_TRACEMSG(Mask, Msg)
#define EC_TRACE_ADDMEM_LOG(Mask, pLogger, szLoc, dwAddress, dwSize) \
            EcMemLogAddMem(pLogger,dwSize)
#define EC_TRACE_SUBMEM_LOG(Mask, pLogger, szLoc, dwAddress, dwSize) \
            EcMemLogSubMem(pLogger, dwSize)

#endif /* DEBUGTRACE */

#define EC_TRACE_ADDMEM(Mask, szLoc, dwAddress, dwSize) \
            EC_TRACE_ADDMEM_LOG(Mask, GetMemLog(), szLoc, dwAddress, dwSize)
#define EC_TRACE_SUBMEM(Mask, szLoc, dwAddress, dwSize) \
            EC_TRACE_SUBMEM_LOG(Mask, GetMemLog(), szLoc, dwAddress, dwSize)

/**********************************************************/
/* EtherCAT master stack internal performance measurement */
/**********************************************************/

/* settings */

#define EC_MSMT_eUsrJob_ProcessAllRxFrames          0
#define EC_MSMT_ProcessCycFrames                    1
#define EC_MSMT_ProcessAcycFrames                   2
#define EC_MSMT_ProcessAcycFramesErrHdl             3
#define EC_MSMT_ProcessAcycFramesCore               4
#define EC_MSMT_ProcessAcycFramesSingleCmd          5
#define EC_MSMT_ProcessSlaveCmd                     6
#define EC_MSMT_ProcessMasterCmd                    7
#define EC_MSMT_NotifyAndFree                       8
#define EC_MSMT_NotifyCallback                      9
#define EC_MSMT_LinkRecvFrame                       10 
#define EC_MSMT_LinkFreeRecvFrame                   11
#define EC_MSMT_LinkFreeSendFrame                   12
#define EC_MSMT_MboxNotify                         13
#define EC_MSMT_eUsrJob_ProcessAllCycFrames        14
#define EC_MSMT_eUsrJob_ProcessAllAcycFrames       15
#define EC_MSMT_eUsrJob_SendAllCycFrames           16
#define EC_MSMT_eUsrJob_SendCycFramesByTaskId      17
#define EC_MSMT_eUsrJob_RunMcSm                    18
#define EC_MSMT_eUsrJob_MasterTimer                19
#define EC_MSMT_LinkSendAndFreeFrame               20 
#define EC_MSMT_FlushQueuedCmds                    21
#define EC_MSMT_LinkSendFrame                      22
#define EC_MSMT_FlushQueuedCmdsFrameDescCreate     23
#define EC_MSMT_LinkAllocSendFrame                 24 
#define EC_MSMT_LinkGetStatus                      25
#define EC_MSMT_QueueEcatCmdReq                    26
#define EC_MSMT_QueueRegisterCmdReq                27
#define EC_MSMT_eUsrJob_SendAcycFrames             28

#define EC_MSMT_HCSM                               30
#define EC_MSMT_BTSM                               31
#define EC_MSMT_BTSSM                              32
#define EC_MSMT_BTCHKSM                            33
#define EC_MSMT_BTEEPSM                            34
#define EC_MSMT_EEPSM                              35
#define EC_MSMT_DCSM                               36
#define EC_MSMT_BT_INIT_CHECK_CONFIG               37
#define EC_MSMT_BT_CHECK_CONFIG                    38
#define NUM_EC_MSMT_POINTS                         39   /* add strings into G_aszMeasInfo[] in EcTscMeas.cpp! */


extern EC_T_TSC_MEAS_DESC G_TscMeasDesc;
extern EC_T_CHAR* G_aszMeasInfo[];

#ifdef INCLUDE_EC_INTERNAL_TSC_MEASUREMENT
    #define PerfMeasInit( pTscMeasDesc, dwlFreqSet, dwNumMeas, pfnMessage )       ecatPerfMeasInit(pTscMeasDesc, dwlFreqSet, dwNumMeas, pfnMessage)
    #define PerfMeasDeinit( pTscMeasDesc )                  ecatPerfMeasDeinit(pTscMeasDesc)
    #define PerfMeasShow(pTscMeasDesc)                      ecatPerfMeasShow(pTscMeasDesc,0xFFFFFFFF, G_aszMeasInfo)
    #define PerfMeasStart(pTscMeasDesc, dwIndex)            ecatPerfMeasStart(pTscMeasDesc,(dwIndex))
    #define PerfMeasEnd(pTscMeasDesc, dwIndex)              ecatPerfMeasEnd(pTscMeasDesc,(dwIndex))
    #define PerfMeasReset( pTscMeasDesc, dwIndex)           ecatPerfMeasReset(pTscMeasDesc, dwIndex)
#else
    #define PerfMeasInit(pTscMeasDesc, pfnMessage)
    #define PerfMeasDeinit(pTscMeasDesc)
    #define PerfMeasShow(pTscMeasDesc)
    #define PerfMeasStart(pTscMeasDesc, dwIndex)
    #define PerfMeasEnd(pTscMeasDesc, dwIndex)
    #define PerfMeasReset(pTscMeasDesc, dwIndex)
#endif

#ifdef __cplusplus
} /* extern "C"*/
#endif


#endif /* INC_ECCOMMONPRIVATE */


/*-END OF SOURCE FILE--------------------------------------------------------*/
