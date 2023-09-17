/*-----------------------------------------------------------------------------
 * AtEthercat.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              interface to the ethercat master
 *----------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>
#include "AtEthercat.h"

#include <EcInterface.h>
#include <EcServices.h>
#include <EcConfig.h>

#include <EcMasterCommon.h>
#include "EcFiFo.h"
#include <EcEthSwitch.h>
#include <EcIoImage.h>
#include <EcDevice.h>
#if (defined INCLUDE_RED_DEVICE)
#include <EcRedDevice.h>
#endif
#include <EcDeviceFactory.h>

#include <EcMaster.h>
#include <EcSlave.h>

#include <AtEthercatPrivate.h>

#include <EcBusTopology.h>
#include <EcHotConnect.h>

#if (defined INCLUDE_DC_SUPPORT)
#include <EcDistributedClocks.h>
#endif

#if (defined INCLUDE_MASTER_OBD)
#include <EcSdoServ.h>
#endif

#if (defined INCLUDE_ADS_ADAPTER) && (defined EC_SOCKET_IP_SUPPORTED)
#include "AdsAdapter.h"
#endif

#include "EcText.h"

#if (defined INSTRUMENT_MASTER)
#include "AtEthercatInstr.h"
#endif

#include "EcMasterInterfaceBase.h"

/* last include to check previous declarations */
#include "EcInterfaceSanityCheck.h"

/*-MACROS--------------------------------------------------------------------*/
#undef  EcLinkErrorMsg
#define EcLinkErrorMsg m_oAtEcatDesc.poEcDevice->LinkErrorMsg
#undef  EcLinkDbgMsg
#define EcLinkDbgMsg   m_oAtEcatDesc.poEcDevice->LinkDbgMsg

//#define WV_EVENTS
#ifdef WV_EVENTS
#include "wvLib.h"
#define WVEVENT(num,buf,len) wvEvent((num),(buf),(len))
#else
#define WVEVENT(num,buf,len)
#endif

#define WVTRACE(offset) \
    WVEVENT((2000+offset), NULL, 0)


/*-TYPEDEFS------------------------------------------------------------------*/

/*-GLOBAL VARIABLES----------------------------------------------------------*/
#ifdef INCLUDE_EC_INTERNAL_TSC_MEASUREMENT
EC_T_TSC_MEAS_DESC G_TscMeasDesc;
EC_T_CHAR* G_aszMeasInfo[NUM_EC_MSMT_POINTS] =
{
    "Job_ProcessAllRxFrames     ",              /*    EC_MSMT_eUsrJob_ProcessAllRxFrames          0 */
    "ProcessCycFrames           ",              /*    EC_MSMT_ProcessCycFrames                    1 */
    "ProcessAcycFrames          ",              /*    EC_MSMT_ProcessAcycFrames                   2 */
    "ProcessAcycFramesErrHdl    ",              /*    EC_MSMT_ProcessAcycFramesErrHdl             3 */
    "ProcessAcycFramesCore      ",              /*    EC_MSMT_ProcessAcycFramesCore               4 */
    "ProcessAcycFramesSingleCmd ",              /*    EC_MSMT_ProcessAcycFramesSingleCmd          5 */
    "ProcessSlaveCmd            ",              /*    EC_MSMT_ProcessSlaveCmd                     6 */
    "ProcessMasterCmd           ",              /*    EC_MSMT_ProcessMasterCmd                    7 */
    "NotifyAndFree              ",              /*    EC_MSMT_NotifyAndFree                       8 */
    "NotifyCallback             ",              /*    EC_MSMT_NotifyCallback                      9 */
    "LinkRecvFrame              ",              /*    EC_MSMT_LinkRecvFrame                      10 */
    "LinkFreeRecvFrame          ",              /*    EC_MSMT_LinkFreeRecvFrame                  11 */
    "LinkFreeSendFrame          ",              /*    EC_MSMT_LinkFreeSendFrame                  12 */
    "MboxNotify                 ",              /*    EC_MSMT_MboxNotify                         13 */
    "Job_ProcessAllCycFrames    ",              /*    EC_MSMT_eUsrJob_ProcessAllCycFrames        14 */
    "Job_ProcessAllAcycFrames   ",              /*    EC_MSMT_eUsrJob_ProcessAllAcycFrames       15 */
    "Job_SendAllCycFrames       ",              /*    EC_MSMT_eUsrJob_SendAllCycFrames           16 */
    "Job_SendCycFramesByTaskId  ",              /*    EC_MSMT_eUsrJob_SendCycFramesByTaskId      17 */
    "Job_RunMcSm                ",              /*    EC_MSMT_eUsrJob_RunMcSm                    18 */
    "Job_MasterTimer            ",              /*    EC_MSMT_eUsrJob_MasterTimer                19 */
    "LinkSendAndFreeFrame       ",              /*    EC_MSMT_LinkSendAndFreeFrame               20 */
    "FlushQueuedCmds            ",              /*    EC_MSMT_FlushQueuedCmds                    21 */
    "LinkSendFrame              ",              /*    EC_MSMT_LinkSendFrame                      22 */
    "FlushQueuedCmdsFrameDescCre",              /*    EC_MSMT_FlushQueuedCmdsFrameDescCreate     23 */
    "LinkAllocSendFrame         ",              /*    EC_MSMT_LinkAllocSendFrame                 24 */
    "LinkGetStatus              ",              /*    EC_MSMT_LinkGetStatus                      25 */
    "QueueEcatCmdReq            ",              /*    EC_MSMT_QueueEcatCmdReq                    26 */
    "QueueRegisterCmdReq        ",              /*    EC_MSMT_QueueRegisterCmdReq                27 */
    "Job_SendAcycFrames         ",              /*    EC_MSMT_eUsrJob_SendAcycFrames             28 */
    "Job_WaitForIOUpdateCompleti",              /*    EC_MSMT_eUsrJob_WaitForIOUpdateCompletion  29 */
    "HC StateMachine            ",              /*    EC_MSMT_HCSM                               30 */
    "BT StateMachine            ",              /*    EC_MSMT_BTSM                               31 */
    "BT SubStateMachine         ",              /*    EC_MSMT_BTSSM                              32 */
    "BT CheckStateMachine       ",              /*    EC_MSMT_BTCHKSM                            33 */
    "BT EEPStateMachine         ",              /*    EC_MSMT_BTEEPSM                            34 */
    "EEPROM StateMachine        ",              /*    EC_MSMT_EEPSM                              35 */
    "DC StateMachine            ",              /*    EC_MSMT_DCSM                               36 */
    "BT InitializeCheckConfigMod",              /*    EC_MSMT_BT_INIT_CHECK_CONFIG               37 */
    "BT CheckConfigModel        ",              /*    EC_MSMT_BT_CHECK_CONFIG                    38 */
};
#endif /* INCLUDE_EC_INTERNAL_TSC_MEASUREMENT */

/*-FUNCTIONS-----------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Get master state machine state name
\return Error Text.
*/
const EC_T_CHAR*  EcMcSmStateString(EC_T_MCSM_STATE McSmState)
{
#ifdef INCLUDE_LOG_MESSAGES
    return GetEcMcSmStateString(McSmState);
#else
    EC_UNREFPARM(McSmState);
    return EC_NULL;
#endif
}


/***************************************************************************************************/
/**
\brief  Get master state machine event string
\return Error Text.
*/
const EC_T_CHAR*  EcMcSmOrderString(EC_T_MCSM_ORDER_TYPE EMcSmOrder)
{
#ifdef INCLUDE_LOG_MESSAGES
    return GetEcMcSmOrderString(EMcSmOrder);
#else
    EC_UNREFPARM(EMcSmOrder);
    return EC_NULL;
#endif
}


/*-CLASS---------------------------------------------------------------------*/

/***************************************************************************************************/
/** \brief Add order to McSM order queue

\return EC_E_NOERROR on success, EC_E_BUSY if order queue is full.
*/

EC_T_DWORD EC_T_ATECATDESC::McSmOrderQueueAdd
(   EC_T_MCSM_ORDER_TYPE        eMcSmOrderType
   ,EC_T_MCSM_ORDER**           ppoMcSmOrder
   ,EC_T_BOOL                   bAutoRelease
   ,EC_T_DWORD                  dwTimeout
   ,EC_T_STATE                  eReqState
   ,EC_T_MCSM_STATE             eMcSmReqState
   ,EC_T_MCSM_ORDER_CONTEXT*    pOrderContext 
)
{
EC_T_DWORD       dwRetVal = EC_E_ERROR;
EC_T_MCSM_ORDER* poMcSmOrder = EC_NULL;

    /* get order descriptor */
    if (bAutoRelease)
    {
        pFreeMcSmOrderFifoAutoRelease->Remove(poMcSmOrder);
    }
    else
    {
        pFreeMcSmOrderFifoApi->Remove(poMcSmOrder);
    }
    if (EC_NULL == poMcSmOrder)
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }
    /* fill descriptor */
    poMcSmOrder->eMcSmOrderType        = eMcSmOrderType;
    poMcSmOrder->eReqState             = eReqState;
    poMcSmOrder->eMcSmReqState         = eMcSmReqState;
    poMcSmOrder->dwOrderId             = dwOrderId++;
    poMcSmOrder->dwRetryCnt            = 0;
    poMcSmOrder->bAutoRelease          = bAutoRelease;
    poMcSmOrder->EMcSmErrorState       = eEcatMcSmState_UNKNOWN;
    poMcSmOrder->EMcSmReqState         = eEcatMcSmState_UNKNOWN;
    poMcSmOrder->dwMcSmLastErrorStatus = EC_E_BUSY;
    poMcSmOrder->eOrderState           = eEcatOrderState_Pending;
    poMcSmOrder->bCancel               = EC_FALSE;
    poMcSmOrder->pContext              = pOrderContext;

    /* start timeout if requested */
    if (EC_WAITINFINITE != dwTimeout)
    {
        poMcSmOrder->oTimeout.Start(dwTimeout, poMaster->GetMsecCounterPtr());
    }
    else
    {
        OsDbgAssert(!poMcSmOrder->oTimeout.IsStarted());
        poMcSmOrder->oTimeout.Stop();
    }
    /* add to new pending order FIFO */
    pNewPendingMcSmOrderFifo->Add(poMcSmOrder);

    /* no error */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_NULL != ppoMcSmOrder)
    {
        *ppoMcSmOrder = poMcSmOrder;
    }
    return dwRetVal;
}

EC_T_VOID EC_T_ATECATDESC::McSmOrderFinish(EC_T_MCSM_ORDER* poMcSmOrder)
{
    /* remove from pending order list */
    RemoveEntryList(&(poMcSmOrder->ListEntry));

    /* stop timeout */
    poMcSmOrder->oTimeout.Stop();

    /* refresh descriptor */
    poMcSmOrder->EMcSmErrorState = EMcSmCurrState;
    poMcSmOrder->EMcSmReqState   = EMcSmReqState;
    poMcSmOrder->eOrderState = eEcatOrderState_Finished;

    /* release descriptor if required */
    if (poMcSmOrder->bAutoRelease)
    {
        poMcSmOrder->eOrderState = eEcatOrderState_Idle;
        poMcSmOrder->pFreeMcSmOrderFifo->AddNoLock(poMcSmOrder);
    }
}

EC_T_VOID EC_T_ATECATDESC::McSmOrderRelease(EC_T_MCSM_ORDER* poMcSmOrder)
{
    OsDbgAssert(EC_FALSE == poMcSmOrder->bAutoRelease);

    /* refresh descriptor */
    poMcSmOrder->eOrderState = eEcatOrderState_Idle;
    /* return to free order FIFO */
    poMcSmOrder->pFreeMcSmOrderFifo->Add(poMcSmOrder);
}

EC_T_VOID EC_T_ATECATDESC::McSmCancelAllOrders(EC_T_VOID)
{
    /* cancel all orders */
    if (!IsListEmpty(&oPendingMcSmOrderList))
    {
    EC_T_LISTENTRY* pListIterator = EC_NULL;

        for (pListIterator  = GetListHeadEntry(&oPendingMcSmOrderList);
             pListIterator != &oPendingMcSmOrderList;
             pListIterator  = GetListNextEntry(pListIterator))
        {
            ((EC_T_MCSM_ORDER*)pListIterator)->bCancel = EC_TRUE;
        }
    }
}

/*-LOCAL VARIABLES-----------------------------------------------------------*/
static EC_T_ATECATMULTIMASTER S_aMultiMaster[MAX_NUMOF_MASTER_INSTANCES] = {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};

static EC_T_VOID*             S_poInitConfigLock     = EC_NULL;
static EC_T_DWORD             S_dwInitConfigLockCnt  = 0;

#if (defined INSTRUMENT_MASTER)
CAtEmInterface* emGetInstance(EC_T_DWORD dwInstanceID)
{
    return S_aMultiMaster[dwInstanceID].pInst;
}
EC_T_DWORD emSetInstance(EC_T_DWORD dwInstanceID, CAtEmInterface* pInst)
{
    if ((EC_NULL != S_aMultiMaster[dwInstanceID].pInst) && (EC_NULL != pInst))
    {
        return EC_E_INVALIDSTATE;
    }

    S_aMultiMaster[dwInstanceID].pInst = pInst;
    return EC_E_NOERROR;
}
#endif /* INSTRUMENT_MASTER */

/***************************************************************************************************/
/** \brief Constructor interface class
*/
CAtEmInterface::CAtEmInterface()
{
    m_pMbxCoeEmergencyTferPriv      = EC_NULL;
    m_pMbxSoeEmergencyTferPriv      = EC_NULL;
    m_pMbxSoeNotificationTferPriv   = EC_NULL;
    OsMemset(&m_oMasterConfigEx, 0, sizeof(EC_T_MASTER_CONFIG_EX));
    OsMemset(&m_oAtEcatDesc, 0, sizeof(EC_T_ATECATDESC));
#if (defined INCLUDE_ADS_ADAPTER) && (defined EC_SOCKET_IP_SUPPORTED)
    m_pAdsSrvHandle = EC_NULL;
#endif /* INCLUDE_ADS_ADAPTER  && EC_SOCKET_IP_SUPPORTED */
    m_dwInternalIOControlCode = 0;

    /* initialization for completeness */
    m_pFactory = EC_NULL;
    m_poEcConfigData = EC_NULL;
#if (defined INCLUDE_CONFIG_EXTEND)
    m_poEcConfigDataExtended = EC_NULL;
#endif
    OsMemset(&m_oMemLog, 0, sizeof(EC_T_MEMORY_LOGGER));
}

/***************************************************************************************************/
/**
\brief  Destructor.
*/
CAtEmInterface::~CAtEmInterface()
{
    DeinitMasterEx(EC_FALSE, EC_TRUE);
    OsMemset(&m_oAtEcatDesc, 0, sizeof(EC_T_ATECATDESC));
}

/***************************************************************************************************/
/**
\brief  Check if parameters given to Interface are OK.
*/
EC_T_DWORD CAtEmInterface::CheckMasterParms(EC_T_INIT_MASTER_PARMS* pParms)
{
    /* check parameters */
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (EC_NULL == pParms)
    {
        EC_ERRORMSGC(("CAtEmInterface::CheckMasterParms(): no parameters provided!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* verify signature */
    {
    EC_T_DWORD dwPattern      =  pParms->dwSignature & 0xFFF00000;
    EC_T_DWORD dwVersionMaj   = (pParms->dwSignature >> 16) &   0xF;
    EC_T_DWORD dwVersionMin   = (pParms->dwSignature >> 12) &   0xF;
/*  EC_T_DWORD dwVersionSP    = (pParms->dwSignature >>  8) &   0xF;
    EC_T_DWORD dwVersionBuild = (pParms->dwSignature >>  0) &  0xFF; */

        if (dwPattern != ATECAT_SIGNATURE_PATTERN)
        {
            EC_ERRORMSGC(("CAtEmInterface::CheckMasterParms() Invalid validation pattern!\n"));
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        if ((dwVersionMaj != ATECAT_VERS_MAJ) || (dwVersionMin != ATECAT_VERS_MIN))
        {
            EC_ERRORMSGC(("CAtEmInterface::CheckMasterParms() Invalid version (incompatible)!\n"));
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    }

    /* EC_T_INIT_MASTER_PARMS is subject to be extended but it must be at least 84 bytes as of V2.7 */
    if (pParms->dwSize < 84)
    {
        EC_ERRORMSGC(("CAtEmInterface::CheckMasterParms() parameter set too small!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (0 == pParms->dwBusCycleTimeUsec)
    {
        EC_ERRORMSGC(("CAtEmInterface::CheckMasterParms() dwBusCycleTimeUsec missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_DWORD CAtEmInterface::GetMasterParms(EC_T_INIT_MASTER_PARMS* pParms, EC_T_DWORD dwParmsBufSize)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRemainSize = dwParmsBufSize;
    EC_T_BYTE* pParmsCur = (EC_T_BYTE*)pParms;

    if (dwParmsBufSize < m_oAtEcatDesc.pInitMasterParms->dwSize)
    {
        EC_ERRORMSGC(("CAtEmInterface::GetMasterParms(): buffer too small!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* copy Master parms */
    OsMemcpy(pParmsCur, m_oAtEcatDesc.pInitMasterParms, m_oAtEcatDesc.pInitMasterParms->dwSize);
    dwRemainSize -= m_oAtEcatDesc.pInitMasterParms->dwSize;
    pParmsCur    += m_oAtEcatDesc.pInitMasterParms->dwSize;

    /* set pointers to EC_NULL, will be set to area inside Parms, if BufSize sufficient */
    pParms->pOsParms = EC_NULL;
    pParms->pLinkParms = EC_NULL;
    pParms->pLinkParmsRed = EC_NULL;

    /* copy OS parms */
    if (EC_NULL != m_oAtEcatDesc.pInitMasterParms->pOsParms)
    {
        if (dwRemainSize >= m_oAtEcatDesc.pInitMasterParms->pOsParms->dwSize)
        {
            OsMemcpy(pParmsCur, m_oAtEcatDesc.pInitMasterParms->pOsParms, m_oAtEcatDesc.pInitMasterParms->pOsParms->dwSize);
            pParms->pOsParms = (EC_T_OS_PARMS*)pParmsCur;

            dwRemainSize -= m_oAtEcatDesc.pInitMasterParms->pOsParms->dwSize;
            pParmsCur    += m_oAtEcatDesc.pInitMasterParms->pOsParms->dwSize;
        }
        else
        {
            EC_DBGMSG("CAtEmInterface::GetMasterParms(): buffer too small to store OS parms\n");
        }
    }

    /* copy Main Link parms */
    if (EC_NULL != m_oAtEcatDesc.pInitMasterParms->pLinkParms)
    {
        if (dwRemainSize >= m_oAtEcatDesc.pInitMasterParms->pLinkParms->dwSize)
        {
            OsMemcpy(pParmsCur, m_oAtEcatDesc.pInitMasterParms->pLinkParms, m_oAtEcatDesc.pInitMasterParms->pLinkParms->dwSize);
            pParms->pLinkParms = (EC_T_LINK_PARMS*)pParmsCur;

            dwRemainSize -= m_oAtEcatDesc.pInitMasterParms->pLinkParms->dwSize;
            pParmsCur    += m_oAtEcatDesc.pInitMasterParms->pLinkParms->dwSize;
        }
        else
        {
            EC_DBGMSG("CAtEmInterface::GetMasterParms(): buffer too small to store Main Link parms\n");
        }
    }

    /* copy Red Link parms */
    if (EC_NULL != m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed)
    {
        if (dwRemainSize >= m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed->dwSize)
        {
            OsMemcpy(pParmsCur, m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed, m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed->dwSize);
            pParms->pLinkParmsRed = (EC_T_LINK_PARMS*)pParmsCur;

            dwRemainSize -= m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed->dwSize;
            pParmsCur    += m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed->dwSize;
        }
        else
        {
            EC_DBGMSG("CAtEmInterface::GetMasterParms(): buffer too small to store Red Link parms\n");
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Check if two structure instances with size information match
*/
/** \return EC_TRUE if both are EC_NULL or content matches else EC_FALSE
*/
template <typename T>
static EC_T_BOOL SizedStructureEquals(T* pSet, T* pAct)
{
    if (pSet == pAct) return EC_TRUE;
    if (EC_NULL == pSet) return EC_FALSE;
    if (EC_NULL == pAct) return EC_FALSE;
    if (pSet->dwSize != pAct->dwSize) return EC_FALSE;
    return (0 == OsMemcmp(pSet, pAct, pSet->dwSize));
}

#define EXIT_IF_MASTER_PARM_CHANGED(pSetParms,pActParms,Parm,Msg)\
    if (pSetParms->Parm != pActParms->Parm)\
    {\
        EC_ERRORMSGC(("CAtEmInterface::ChangeMasterParms(), changing %s is not supported!\n"));\
        dwRetVal = EC_E_NOTSUPPORTED;\
        goto Exit;\
    }

EC_T_DWORD CAtEmInterface::ChangeMasterParms(EC_T_INIT_MASTER_PARMS* pParms)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    /* check if parameters are OK */
    dwRes = CheckMasterParms(pParms);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    /* OS parms (changes currently not supported!) */
    if (!SizedStructureEquals(pParms->pOsParms, m_oAtEcatDesc.pInitMasterParms->pOsParms))
    {
        EC_ERRORMSGC(("CAtEmInterface::ChangeMasterParms(): changing OS parms is not supported!\n"));
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }

    /* Main Link parms (changes currently not supported!) */
    if (!SizedStructureEquals(pParms->pLinkParms, m_oAtEcatDesc.pInitMasterParms->pLinkParms))
    {
        EC_ERRORMSGC(("CAtEmInterface::ChangeMasterParms(), Main Link parms: changing Link parms is not supported!\n"));
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }

    /* Red Link parms (changes currently not supported!) */
    if (!SizedStructureEquals(pParms->pLinkParmsRed, m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed))
    {
        EC_ERRORMSGC(("CAtEmInterface::ChangeMasterParms(), Red Link parms: changing Link parms is not supported!\n"));
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }

    /* check for not supported changes in direct members of EC_T_INIT_MASTER_PARMS */
    EXIT_IF_MASTER_PARM_CHANGED(m_oAtEcatDesc.pInitMasterParms, pParms, dwMaxBusSlaves,             "dwMaxBusSlaves");
    EXIT_IF_MASTER_PARM_CHANGED(m_oAtEcatDesc.pInitMasterParms, pParms, dwMaxQueuedEthFrames,       "dwMaxQueuedEthFrames");
    EXIT_IF_MASTER_PARM_CHANGED(m_oAtEcatDesc.pInitMasterParms, pParms, dwAdditionalEoEEndpoints,   "dwAdditionalEoEEndpoints");
    EXIT_IF_MASTER_PARM_CHANGED(m_oAtEcatDesc.pInitMasterParms, pParms, bVLANEnable,                "bVLANEnable");
    EXIT_IF_MASTER_PARM_CHANGED(m_oAtEcatDesc.pInitMasterParms, pParms, wVLANId,                    "wVLANId");
    EXIT_IF_MASTER_PARM_CHANGED(m_oAtEcatDesc.pInitMasterParms, pParms, byVLANPrio,                 "byVLANPrio");

    /* check for supported changes in direct members of EC_T_INIT_MASTER_PARMS */
    if (m_oAtEcatDesc.pInitMasterParms->dwEcatCmdTimeout != pParms->dwEcatCmdTimeout)
    {
        EC_DBGMSG("CAtEmInterface::ChangeMasterParms(): changing dwEcatCmdTimeout from %d to %d\n",
            (EC_T_INT)m_oAtEcatDesc.pInitMasterParms->dwEcatCmdTimeout, (EC_T_INT)pParms->dwEcatCmdTimeout);

        m_oAtEcatDesc.pInitMasterParms->dwEcatCmdTimeout = pParms->dwEcatCmdTimeout;
        m_oAtEcatDesc.poMaster->m_MasterConfig.dwEcatCmdTimeout = pParms->dwEcatCmdTimeout;
        m_oAtEcatDesc.poMaster->CalculateEcatCmdTimeout(pParms->dwBusCycleTimeUsec);        
    }
    if (m_oAtEcatDesc.pInitMasterParms->dwBusCycleTimeUsec != pParms->dwBusCycleTimeUsec)
    {
        EC_DBGMSG("CAtEmInterface::ChangeMasterParms(): changing dwBusCycleTimeUsec from %d to %d\n",
            (EC_T_INT)m_oAtEcatDesc.pInitMasterParms->dwBusCycleTimeUsec, (EC_T_INT)pParms->dwBusCycleTimeUsec);

        dwRes = SetMasterInitParmBusCycleTimeUsec(pParms->dwBusCycleTimeUsec);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    if (m_oAtEcatDesc.pInitMasterParms->dwMaxSlaveCmdPerFrame != pParms->dwMaxSlaveCmdPerFrame)
    {
        EC_DBGMSG("CAtEmInterface::ChangeMasterParms(): changing dwMaxSlaveCmdPerFrame from %d to %d\n",
            (EC_T_INT)m_oAtEcatDesc.pInitMasterParms->dwMaxSlaveCmdPerFrame, (EC_T_INT)pParms->dwMaxSlaveCmdPerFrame);

        m_oAtEcatDesc.pInitMasterParms->dwMaxSlaveCmdPerFrame = pParms->dwMaxSlaveCmdPerFrame;
        m_oAtEcatDesc.poMaster->m_MasterConfig.dwMaxSlaveCmdPerFrame = pParms->dwMaxSlaveCmdPerFrame;
    }
    if (m_oAtEcatDesc.pInitMasterParms->dwMaxSentQueuedFramesPerCycle != pParms->dwMaxSentQueuedFramesPerCycle)
    {
        EC_DBGMSG("CAtEmInterface::ChangeMasterParms(): changing dwMaxSentQueuedFramesPerCycle from %d to %d\n",
            (EC_T_INT)m_oAtEcatDesc.pInitMasterParms->dwMaxSentQueuedFramesPerCycle, (EC_T_INT)pParms->dwMaxSentQueuedFramesPerCycle);

        m_oAtEcatDesc.pInitMasterParms->dwMaxSentQueuedFramesPerCycle = pParms->dwMaxSentQueuedFramesPerCycle;
        m_oAtEcatDesc.poMaster->m_MasterConfig.dwMaxSentQueuedFramesPerCyc = pParms->dwMaxSentQueuedFramesPerCycle;
    }
    if (m_oAtEcatDesc.pInitMasterParms->dwMaxSlavesProcessedPerCycle != pParms->dwMaxSlavesProcessedPerCycle)
    {
        EC_DBGMSG("CAtEmInterface::ChangeMasterParms(): changing dwMaxSlavesProcessedPerCycle from %d to %d\n",
            (EC_T_INT)m_oAtEcatDesc.pInitMasterParms->dwMaxSlavesProcessedPerCycle, (EC_T_INT)pParms->dwMaxSlavesProcessedPerCycle);

        m_oAtEcatDesc.pInitMasterParms->dwMaxSlavesProcessedPerCycle = pParms->dwMaxSlavesProcessedPerCycle;
        m_oAtEcatDesc.poMaster->m_MasterConfig.dwMaxSlavesProcessedPerCycle =
            (0 == pParms->dwMaxSlavesProcessedPerCycle)
            ? m_oAtEcatDesc.poMaster->GetMaxBusSlaves() : pParms->dwMaxSlavesProcessedPerCycle;
    }
    if (m_oAtEcatDesc.pInitMasterParms->dwEcatCmdMaxRetries != pParms->dwEcatCmdMaxRetries)
    {
        EC_DBGMSG("CAtEmInterface::ChangeMasterParms(): changing dwEcatCmdMaxRetries from %d to %d\n",
            (EC_T_INT)m_oAtEcatDesc.pInitMasterParms->dwEcatCmdMaxRetries, (EC_T_INT)pParms->dwEcatCmdMaxRetries);

        m_oAtEcatDesc.pInitMasterParms->dwEcatCmdMaxRetries = pParms->dwEcatCmdMaxRetries;
        m_oAtEcatDesc.poMaster->m_MasterConfig.dwEcatCmdMaxRetries = pParms->dwEcatCmdMaxRetries;
    }
    if (m_oAtEcatDesc.pInitMasterParms->dwLogLevel != pParms->dwLogLevel)
    {
        EC_DBGMSG("CAtEmInterface::ChangeMasterParms(): changing dwLogLevel from %d to %d\n",
            (EC_T_INT)m_oAtEcatDesc.pInitMasterParms->dwLogLevel, (EC_T_INT)pParms->dwLogLevel);

        m_oAtEcatDesc.pInitMasterParms->dwLogLevel = pParms->dwLogLevel;
        m_oAtEcatDesc.poMaster->m_MasterConfig.dwLogLevel = pParms->dwLogLevel;

        {
            /* 0=off, 1=LogMsg, 2=LinkMsg, 3=LogMsg+Linkmsg */
            EC_T_BYTE byLogDst = (EC_T_BYTE)((pParms->dwLogLevel >= EC_LOG_LEVEL_VERBOSE) ? (1 + (pParms->bLogToLinkLayer ? 2 : 0)) : 0);

            m_oAtEcatDesc.poMaster->m_MasterConfig.dwStateChangeDebug = (/* BASE_CLOCK_DBG_LEVEL */                  0 << 18)
                                                                                           | (/* MASTER_CONTROL_STATE_CHANGE_DBG_LEVEL */ byLogDst << 16)
                                                                                           | (/* HOT_CONNECT_DBG_LEVEL */                 byLogDst << 4)
                                                                                           | (/* MASTER_ENH_MODULE_DBG_LEVEL */           byLogDst << 2)
                                                                                           | (/* STATE_CHANGE_DBG_LEVEL */                byLogDst);
        }
    }
    if (m_oAtEcatDesc.pInitMasterParms->dwEoETimeout != pParms->dwEoETimeout)
    {
        EC_DBGMSG("CAtEmInterface::ChangeMasterParms(): changing dwEoETimeout from %d to %d\n",
            (EC_T_INT)m_oAtEcatDesc.pInitMasterParms->dwEoETimeout, (EC_T_INT)pParms->dwEoETimeout);

        m_oAtEcatDesc.pInitMasterParms->dwEoETimeout = pParms->dwEoETimeout;
        m_oAtEcatDesc.poMaster->m_MasterConfig.dwEoeTimeout = pParms->dwEoETimeout;
    }
    if (m_oAtEcatDesc.pInitMasterParms->pfLogMsgCallBack != pParms->pfLogMsgCallBack)
    {
        EC_DBGMSG("CAtEmInterface::ChangeMasterParms(): changing pfLogMsgCallBack!\n");
        OsAddDbgMsgHook(pParms->pfLogMsgCallBack);
        m_oAtEcatDesc.pInitMasterParms->pfLogMsgCallBack = pParms->pfLogMsgCallBack;
    }
    if (m_oAtEcatDesc.pInitMasterParms->bLogToLinkLayer != pParms->bLogToLinkLayer)
    {
        EC_DBGMSG("CAtEmInterface::ChangeMasterParms(): changing bLogToLinkLayer from %d to %d\n",
            (EC_T_INT)m_oAtEcatDesc.pInitMasterParms->bLogToLinkLayer, (EC_T_INT)pParms->bLogToLinkLayer);

        m_oAtEcatDesc.pInitMasterParms->bLogToLinkLayer = pParms->bLogToLinkLayer;
        m_oAtEcatDesc.poMaster->m_MasterConfig.dwErrorMsgToLinkLayer = pParms->bLogToLinkLayer;
    }

    /* EC_T_INIT_MASTER_PARMS::dwFoEBusyTimeout is obsolete */

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** */
/** \brief Initialize the EtherCAT master stack

  This function should be called at first, before the EtherCAT master stack can be used.
  After this call success, the function Start() should be call to place the EtherCAT master stack in
  operational state. The stack parameters are given through a structure EC_T_INIT_MASTER_PARMS with following
  members:
  pLinkParms         Parameters for the link layer; the type of the parameters  depends on the LinkLayer
  implementation. This parameter is normally a string to identify the network adapter.
  pvOsParms           Parameters for the OS layer; the type of the parameters depends on the OsLayer
  implementation.
  nBaseTimerPeriod    Base timer period in microsecond; this period is used to generate the master
  timer and  the cyclic commands timer.
  from the base timer. The stack state machine and watchdog is based on this timer.
*/
/** \return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::InitMaster(
    EC_T_DWORD                      dwInstanceID,
    EC_T_BOOL                       bFirstInitialization,
    EC_T_INIT_MASTER_PARMS*         pParms,                 /**< [in] Initialization parameters */
    EC_T_INTFEATUREFIELD_DESC*      pFeatures,
    EC_T_LINK_DEV_RESTORE_PARAM*    pRestoreParam,
    EC_T_LINK_DEV_RESTORE_PARAM*    pRedRestoreParam,
    EC_T_CYCFRAME_RX_CBDESC*        pCycRxDesc,
    EC_T_CNF_TYPE                   eCnfType,               /**< [in] Enum type of configuration data provided */
    EC_T_PBYTE                      pbyCnfData,             /**< [in] Configuration data */
    EC_T_DWORD                      dwCnfDataLen            /**< [in] Length of configuration data in byte */
)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    dwRes = InitMasterEx(dwInstanceID, bFirstInitialization, pParms, pFeatures, pRestoreParam, pRedRestoreParam, pCycRxDesc, eCnfType, pbyCnfData, dwCnfDataLen);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    /* enable jobs now */
    EnableJobs();

    /* no error */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

EC_T_DWORD CAtEmInterface::InitMasterEx(
    EC_T_DWORD                      dwInstanceID,
    EC_T_BOOL                       bFirstInitialization,
    EC_T_INIT_MASTER_PARMS*         pParms,                 /**< [in] Initialization parameters */
    EC_T_INTFEATUREFIELD_DESC*      pFeatures,
    EC_T_LINK_DEV_RESTORE_PARAM*    pRestoreParam,
    EC_T_LINK_DEV_RESTORE_PARAM*    pRedRestoreParam,
    EC_T_CYCFRAME_RX_CBDESC*        pCycRxDesc,
    EC_T_CNF_TYPE                   eCnfType,               /**< [in] Enum type of configuration data provided */
    EC_T_PBYTE                      pbyCnfData,             /**< [in] Configuration data */
    EC_T_DWORD                      dwCnfDataLen            /**< [in] Length of configuration data in byte */
)
{
    EC_T_DWORD           dwRetVal            = EC_E_ERROR;
    EC_T_DWORD           dwRes               = 0;
    EC_T_DWORD           dwIdx               = 0;
    EC_T_PVOID           pvDeviceParms       = EC_NULL;
    EC_T_MBXTFER_DESC    MbxTferDesc         = {0};
    EC_T_DWORD           dwSupportedFeatures = 0;

    /* check if already initialized */
    if (m_oAtEcatDesc.bInitialized)
    {
        EC_ERRORMSG(("ecatInitMaster() Stack already initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRes = CheckMasterParms(pParms);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    /* initialize OS layer */
    if (bFirstInitialization)
    {
    EC_T_OS_PARMS  OsParms;
    EC_T_OS_PARMS* pOsParms;

        if (EC_NULL == pParms->pOsParms)
        {
            OsMemset(&OsParms, 0, sizeof(EC_T_OS_PARMS));
            pOsParms = &OsParms;
        }
        else
        {
            pOsParms = pParms->pOsParms;
        }
        pOsParms->dwSupportedFeatures = 0xFFFFFFFF;
        dwRes = OsInit(pOsParms);
        if (EC_E_NOERROR != dwRes)
        {
            EC_ERRORMSG(("ecatInitMaster() Cannot initialize OS layer!\n"));
            dwRetVal = dwRes;
            goto Exit;
        }
        /* evaluate pParms->pfLogMsgCallBack internally instead of calling EC_DBGMSG */
        OsAddDbgMsgHook(pParms->pfLogMsgCallBack);
        dwSupportedFeatures = pOsParms->dwSupportedFeatures;
        
        /* show version of the stack */
        if (0 == OsStrlen(ATECAT_PLATFORMSTR))
        {
            EC_DBGMSG("EC-Master V%s %s\n", ATECAT_FILEVERSIONSTR, ATECAT_COPYRIGHT);
        }
        else
        {
            EC_DBGMSG("EC-Master V%s for %s %s\n", ATECAT_FILEVERSIONSTR, ATECAT_PLATFORMSTR, ATECAT_COPYRIGHT);
        }
#ifdef DEBUG
        EC_DBGMSG("#####################################################\n");
        EC_DBGMSG("#################### DEBUG BUILD ####################\n");
        EC_DBGMSG("#####################################################\n");
#endif
    }
    else
    {
        /* to reach this code, the previous license check were successfull  */
        /* enable here all features support within th8is function, next     */
        /* check will be done using EcMaster::dwSupportedFeatures, restored */
        /* by Set/GetFeatureField                                           */
        dwSupportedFeatures = 0xFFFFFFFF;
    }

    /* check EtherCAT Master feature */
    if (0 == (dwSupportedFeatures & EC_FEATURES_MASTER_MASK))
    {
        EC_ERRORMSGC(("ecatInitMaster() EtherCAT Master is not licensed!\n"));
        dwRetVal = EC_E_FEATURE_DISABLED;
        goto Exit;
    }
    /* check link layer(s) parameter */
    if ((EC_NULL == pParms->pLinkParms) && (EC_NULL != pParms->pLinkParmsRed))
    {
        EC_ERRORMSGC(("ecatInitMaster() Invalid link layer parameter (Null pointer at main, but red not null)!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if ((EC_NULL != pParms->pLinkParms) && (EC_LINK_PARMS_SIGNATURE_PATTERN != (pParms->pLinkParms->dwSignature & 0xFFF00000)))
    {
        EC_ERRORMSGC(("ecatInitMaster() Invalid link layer parameter (wrong signature)!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* cable redundancy related checks */
    if (EC_NULL != pParms->pLinkParmsRed)
    {
#if (defined INCLUDE_RED_DEVICE)
        /* check EtherCAT Master cable redundancy feature */
        if (0 == (dwSupportedFeatures & EC_FEATURES_RED_MASK))
        {
            EC_ERRORMSGC(("ecatInitMaster() EtherCAT Master cable redundancy is not licensed!\n"));
            dwRetVal = EC_E_FEATURE_DISABLED;
            goto Exit;
        }
        if (EC_LINK_PARMS_SIGNATURE_PATTERN != (pParms->pLinkParmsRed->dwSignature & 0xFFF00000))
        {
            EC_ERRORMSGC(("ecatInitMaster() Invalid redundancy link layer parameter (wrong signature)!\n"));
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
#else /* INCLUDE_RED_DEVICE */
        EC_ERRORMSGC(("ecatInitMaster() EtherCAT Master cable redundancy is not supported!\n"));
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
#endif /* INCLUDE_RED_DEVICE */
    }
    if (pParms->dwMaxQueuedEthFrames > MAX_QUEUED_ETH_FRAMES)
    {
        EC_ERRORMSGC(("ecatInitMaster(): configuration error (dwMaxQueuedEthFrames) - maximum number of queued ethernet (acyclic) frames is %d (requested: %d)!\n", MAX_QUEUED_ETH_FRAMES, pParms->dwMaxQueuedEthFrames));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /*
     * Check consistency for ecat command timeout (timeout for receiving queued acyclic frames):
     *
     * Mechanism of queuing  EtherCAT commands:
     * All EtherCAT commands are queued by calling CEcMaster::QueueEcatCmdReq().
     * As long as the ecat command fits into the currently allocated Ethernet frame m_oAtEcatDesc.pCurrPendingSlaveFrameDesc
     * and m_oAtEcatDesc.MasterConfig.dwMaxSlaveCmdPerFrame is not reached the new command is appended after the last
     * queued command.
     * The pending frame (and thus all commands within this frame) is finally flushed into CEcDevice::m_oAtEcatDesc.PendingSlaveFrameDescList
     * in FlushCurrPendingSlaveFrame().
     *
     * Mechanism of sending queued EtherCAT frames:
     * Queued frames will be sent by CEcDevice::SendQueuedFrames().
     * The number of frames sent within a single call to SendQueuedFrames() is limited by the configuration
     * variable pParms->pMasterConfig->dwMaxSentQueuedFramesPerCyc.
     * The master normally sends queued frames in CEcDevice::StartIo() after sending cyclic frames.
     * In master triggered mode this method will be called by CEcMaster::OnMasterTimer()
     *
     * The master verifies if the time between flushing the EtherCAT frame in FlushCurrPendingSlaveFrame() and
     * receiving the frame in CEcMaster::CheckFrame() does not exceed the timeout value set by the configuration
     * variable pParms->pMasterConfig->dwEcatCmdTimeout
     *
     * Consistency:
     * a) The timeout value must be higher than one master cycle (as it is only verified after this time)
     * b) If the dwSendAcycAfterSyncFramesOnly parameter is set to EC_TRUE then sending acyclic frames
     *    is linked to sending cyclic frames: acyclic frames are only sent after sending cyclic frames.
     *    In this case the queued command timeout value has to be higher than the timeout for sending cyclic frames.
     */

    /* reset EtherCAT descriptor */
    OsMemset(&m_oAtEcatDesc, 0, sizeof(EC_T_ATECATDESC));

    /* initialize the ethercat descriptor */
    if (bFirstInitialization)
    {
        /* copy master init parameters */
        m_oAtEcatDesc.pInitMasterParms = (EC_T_INIT_MASTER_PARMS*)OsMalloc(EC_MAX(pParms->dwSize, sizeof(EC_T_INIT_MASTER_PARMS)));
        EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_oAtEcatDesc.pInitMasterParms", m_oAtEcatDesc.pInitMasterParms, EC_MAX(pParms->dwSize, sizeof(EC_T_INIT_MASTER_PARMS)));
        if (EC_NULL == m_oAtEcatDesc.pInitMasterParms)
        {
            EC_ERRORMSGC(("ecatInitMaster() Cannot store master init parameters!\n"));
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        OsMemset(m_oAtEcatDesc.pInitMasterParms, 0, sizeof(EC_T_INIT_MASTER_PARMS));
        OsMemcpy(m_oAtEcatDesc.pInitMasterParms, pParms, pParms->dwSize);

        /* copy OS parameters */
        if (EC_NULL != pParms->pOsParms)
        {
            m_oAtEcatDesc.pInitMasterParms->pOsParms = (EC_T_OS_PARMS*)OsMalloc(pParms->pOsParms->dwSize);
            EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_oAtEcatDesc.pInitMasterParms->pOsParms", m_oAtEcatDesc.pInitMasterParms->pOsParms, pParms->pOsParms->dwSize);
            if (EC_NULL == m_oAtEcatDesc.pInitMasterParms->pOsParms)
            {
                EC_ERRORMSGC(("ecatInitMaster() Cannot store OS parameters!\n"));
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }
            OsMemcpy(m_oAtEcatDesc.pInitMasterParms->pOsParms, pParms->pOsParms, pParms->pOsParms->dwSize);
        }
        /* copy link layer(s) parameters */
        if (EC_NULL != pParms->pLinkParms)
        {
            m_oAtEcatDesc.pInitMasterParms->pLinkParms = (EC_T_LINK_PARMS*)OsMalloc(pParms->pLinkParms->dwSize);
            EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_oAtEcatDesc.pInitMasterParms->pLinkParms", m_oAtEcatDesc.pInitMasterParms->pLinkParms, pParms->pLinkParms->dwSize);
            if (EC_NULL == m_oAtEcatDesc.pInitMasterParms->pLinkParms)
            {
                EC_ERRORMSGC(("ecatInitMaster() Cannot store link layer parameters!\n"));
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }
            OsMemcpy(m_oAtEcatDesc.pInitMasterParms->pLinkParms, pParms->pLinkParms, pParms->pLinkParms->dwSize);
        }
#if (defined INCLUDE_RED_DEVICE)
        /* copy redundancy link layer parameters */
        if (EC_NULL != pParms->pLinkParmsRed)
        {
            m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed = (EC_T_LINK_PARMS*)OsMalloc(pParms->pLinkParmsRed->dwSize);
            EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed", m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed, pParms->pLinkParmsRed->dwSize);
            if (EC_NULL == m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed)
            {
                EC_ERRORMSGC(("ecatInitMaster() Cannot store redundancy link layer parameters!\n"));
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }
            OsMemcpy(m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed, pParms->pLinkParmsRed, pParms->pLinkParmsRed->dwSize);
        }
        else
#endif
        {
            m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed = EC_NULL;
        }
        
        /* access to application's parameters not needed any more */
        pParms = m_oAtEcatDesc.pInitMasterParms;
    }
    else
    {
        /* restore parameters */
        m_oAtEcatDesc.pInitMasterParms = pParms;
        OsMemcpy(&m_oAtEcatDesc.oCycRxDesc, pCycRxDesc, sizeof(EC_T_CYCFRAME_RX_CBDESC));
    }
    /* store instance ID */
    m_oAtEcatDesc.dwInstanceID = dwInstanceID;

    /* create feature field */
    if (bFirstInitialization)
    {
        pFeatures = CEcMaster::CreateFeatureField(dwSupportedFeatures, GetMemLog());
#if (defined VLAN_FRAME_SUPPORT)
        m_oAtEcatDesc.poMaster->InsertVlanInFeatureField(pFeatures, pParms->bVLANEnable, pParms->wVLANId, pParms->byVLANPrio);
#endif
    }
    /* create timeout objects */
    OsDbgAssert(m_oAtEcatDesc.poMcSlaveErrAckRetryTimeout == EC_NULL);
    m_oAtEcatDesc.poMcSlaveErrAckRetryTimeout = EC_NEW(CEcTimer());
    EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_oAtEcatDesc.poMcSlaveErrAckRetryTimeout", m_oAtEcatDesc.poMcSlaveErrAckRetryTimeout, sizeof(CEcTimer));

    /* create config data storage */
    switch (eCnfType)
    {
    case eCnfType_None:
    case eCnfType_Filename:
    case eCnfType_Data:
    case eCnfType_Datadiag:
        if (EC_NULL == m_poEcConfigData)
        {
            m_poEcConfigData = EC_NEW(CEcConfigData(dwInstanceID, GetMemLog()));
            EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_poEcConfigData", m_poEcConfigData, sizeof(CEcConfigData));
            if (EC_NULL == m_poEcConfigData)
            {
                EC_ERRORMSG(("ecatInitMaster() Cannot create config data!\n"));
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }
        }     
        m_poEcConfigData->Initialize();
        break;
    case eCnfType_GenPreopENI:
    case eCnfType_GenPreopENIWithCRC:
    case eCnfType_GenOpENI:
    case eCnfType_ConfigData:
    {
        CEcConfigData* poConfigData = (CEcConfigData*)pbyCnfData;
        if (m_poEcConfigData != poConfigData)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_poEcConfigData", m_poEcConfigData, sizeof(CEcConfigData));
            SafeDelete(m_poEcConfigData);
        }

        m_poEcConfigData = poConfigData;
    }   break;
    case eCnfType_Unknown:
    default:
        EC_ERRORMSGC(("ecatInitMaster() Unknown config type\n"));
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }
    /* create EtherCAT device factory */
    m_pFactory = EC_NEW(CEcDeviceFactory(m_poEcConfigData));
    EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_pFactory", m_pFactory, sizeof(CEcDeviceFactory));
    if (EC_NULL == m_pFactory)
    {
        EC_ERRORMSGC(("ecatInitMaster() Cannot create device factory!\n"));
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    /* create EtherCAT device */
    {
    EC_T_MASTER_CONFIG oMasterConfig;
    EC_T_DEVICE_FACTORY_PARMS oFactoryParms;

        /* fill the config descriptor */
        OsMemset(&oMasterConfig, 0, sizeof(EC_T_MASTER_CONFIG));
        oMasterConfig.dwEcatCmdTimeout              = pParms->dwEcatCmdTimeout;
        oMasterConfig.dwEcatCmdMaxRetries           = pParms->dwEcatCmdMaxRetries;
        oMasterConfig.dwEoeTimeout                  = pParms->dwEoETimeout;
        oMasterConfig.dwMaxQueuedEthFrames          = pParms->dwMaxQueuedEthFrames;
        oMasterConfig.dwAdditionalEoEEndpoints      = pParms->dwAdditionalEoEEndpoints;
        oMasterConfig.dwMaxSlaveCmdPerFrame         = pParms->dwMaxSlaveCmdPerFrame;
        oMasterConfig.dwBusCycleTimeUsec            = pParms->dwBusCycleTimeUsec;
        oMasterConfig.dwLogLevel                    = pParms->dwLogLevel;
        oMasterConfig.dwErrorMsgToLinkLayer         = pParms->bLogToLinkLayer;
        {
        /* 0=off, 1=LogMsg, 2=LinkMsg, 3=LogMsg+Linkmsg */
        EC_T_BYTE byLogDst = (EC_T_BYTE)((pParms->dwLogLevel >= EC_LOG_LEVEL_VERBOSE) ? ( 1 + (pParms->bLogToLinkLayer ? 2 : 0)) : 0);

                oMasterConfig.dwStateChangeDebug = (/* BASE_CLOCK_DBG_LEVEL */                  0        << 18)
                                                 | (/* MASTER_CONTROL_STATE_CHANGE_DBG_LEVEL */ byLogDst << 16)
                                                 | (/* HOT_CONNECT_DBG_LEVEL */                 byLogDst << 4)
                                                 | (/* MASTER_ENH_MODULE_DBG_LEVEL */           byLogDst << 2)
                                                 | (/* STATE_CHANGE_DBG_LEVEL */                byLogDst);
        }
        oMasterConfig.dwMaxSentQueuedFramesPerCyc   = pParms->dwMaxSentQueuedFramesPerCycle;
        if (bFirstInitialization)
        {
            m_oMasterConfigEx.bOnlyProcDataInImage  = EC_FALSE;
            m_oMasterConfigEx.dwInstanceID          = dwInstanceID;
            m_oMasterConfigEx.bUseRedundancy        = (EC_NULL != pParms->pLinkParmsRed);
            m_oMasterConfigEx.wMaxBusSlaves = (EC_T_WORD)pParms->dwMaxBusSlaves;
            if (0 == m_oMasterConfigEx.wMaxBusSlaves)
            {
                m_oMasterConfigEx.wMaxBusSlaves = EC_SB_DEFAULTSLAVEAMOUNT;
            }
#if (defined INCLUDE_MASTER_OBD)
            m_oMasterConfigEx.wMaxBusSlaves = (EC_T_WORD)EC_MIN(m_oMasterConfigEx.wMaxBusSlaves, 4095);
#endif
            /* fill the configuration descriptor */
            m_oCfgParm.eCnfType = eCnfType_None;
            m_oCfgParm.pbyData  = EC_NULL;
            m_oCfgParm.dwLen    = 0;
        }
        else
        {
            m_oCfgParm.eCnfType = eCnfType;
            m_oCfgParm.pbyData  = pbyCnfData;
            m_oCfgParm.dwLen    = dwCnfDataLen;
        }

        oMasterConfig.dwMaxSlavesProcessedPerCycle = pParms->dwMaxSlavesProcessedPerCycle;
        if (0 == oMasterConfig.dwMaxSlavesProcessedPerCycle)
        {
            oMasterConfig.dwMaxSlavesProcessedPerCycle = m_oMasterConfigEx.wMaxBusSlaves;
        }

        oFactoryParms.pConfigParms      = &m_oCfgParm;
        oFactoryParms.pfMbCallback      = WrapperToMboxNotify;
        oFactoryParms.pfNotifyCallback  = WrapperToNotifyAllClients;
        oFactoryParms.pMasterConfig     = &oMasterConfig;
        oFactoryParms.pMasterConfigEx   = &m_oMasterConfigEx;
        oFactoryParms.dwVersion         = ATECAT_VERSION;
        oFactoryParms.bCreateTemporary  = bFirstInitialization;
        oFactoryParms.pFeatures         = pFeatures;
        oFactoryParms.pInitMasterParms  = m_oAtEcatDesc.pInitMasterParms;
        oFactoryParms.pMLog             = GetMemLog();

        dwRes =  m_pFactory->CreateDevice(&oFactoryParms, &m_oAtEcatDesc.poEcDevice);
        if (EC_E_NOERROR != dwRes)
        {
            EC_ERRORMSGC(("ecatInitMaster() Cannot create EtherCAT device!\n"));
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    if (bFirstInitialization)
    {
        /* this call is needed to cleanup some temporary stuff */
        dwRes = m_pFactory->ConfigApply(m_oAtEcatDesc.poEcDevice, EC_TRUE);
        if (EC_E_NOERROR != dwRes)
        {
            EC_ERRORMSGC(("ecatInitMaster() Cannot apply the configuration to the EtherCAT device!\n"));
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    /* create/open link layer(s) */
    if (bFirstInitialization)
    {
        /* use open device parameters */
        pvDeviceParms = m_oAtEcatDesc.pInitMasterParms->pLinkParms;
    }
    else
    {
        /* use restore device parameters */
        pvDeviceParms = pRestoreParam;
    }
    dwRes = m_oAtEcatDesc.poEcDevice->OpenDevice(pvDeviceParms);
    if (EC_E_NOERROR != dwRes)
    {
        EC_ERRORMSGC(("ecatInitMaster(): Error opening EtherCAT device!\n"));
        dwRetVal = dwRes;
        goto Exit;
    }

    m_oAtEcatDesc.eLinkMode = m_oAtEcatDesc.poEcDevice->LinkGetMode();
#if (defined INCLUDE_RED_DEVICE)
    if (EC_NULL != pParms->pLinkParmsRed)
    {
        /* signalize redundancy link layer to main link layer */
        m_oAtEcatDesc.poEcDevice->m_bRedConfigured = EC_TRUE;

        /* create redundancy link layer object */
        m_oAtEcatDesc.poEcRedDevice = EC_NEW(CEcRedDevice(m_oAtEcatDesc.poEcDevice));
        EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:poEcRedDevice", m_oAtEcatDesc.poEcRedDevice, sizeof(CEcRedDevice));
        if (m_oAtEcatDesc.poEcRedDevice == EC_NULL)
        {
            EC_ERRORMSGC(("ecatInitMaster(): Cannot create redundant EtherCAT device!\n"));
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        /* open redundant link layer */
        if (bFirstInitialization)
        {
            /* use open device parameters */
            pvDeviceParms = m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed;
        }
        else
        {
            /* use restore device parameters */
            pvDeviceParms = pRedRestoreParam;
        }
        dwRes = m_oAtEcatDesc.poEcRedDevice->OpenDevice(pvDeviceParms);
        if (EC_E_NOERROR != dwRes)
        {
            EC_ERRORMSGC(("ecatInitMaster(): Cannot open redundant EtherCAT device!\n"));
            dwRetVal = dwRes;
            goto Exit;
        }
        /* give a chance to RedPort slave to detect link */
        OsSleep(ECBT_TOPOCHANGE_DELAY);

        /* enable redundancy link layer */
        dwRes = m_oAtEcatDesc.poEcDevice->AddRedDevice(m_oAtEcatDesc.poEcRedDevice);
        if (EC_E_NOERROR != dwRes)
        {
            EC_ERRORMSGC(("ecatInitMaster(): Cannot add redundant EtherCAT device!\n"));
            dwRetVal = dwRes;
            goto Exit;
        }
        if(m_oAtEcatDesc.poEcRedDevice->LinkGetMode() != m_oAtEcatDesc.eLinkMode)
        {
            EC_ERRORMSGC(("ecatInitMaster(): Main device and redundant device with different modes (POLLING<->INTERRUPT) is not supported!\n"));
            dwRetVal = EC_E_NOTSUPPORTED;
            goto Exit;
        }
    }
#else
    EC_UNREFPARM(pRedRestoreParam);
#endif

    /* create locks */
    m_oAtEcatDesc.poSetProcessDataDescLock    = OsCreateLockTyped(eLockType_INTERFACE);
    m_oAtEcatDesc.poGetProcessDataDescLock    = OsCreateLockTyped(eLockType_INTERFACE);
#if (defined INCLUDE_FORCE_PROCESSDATA)
    m_oAtEcatDesc.poForceRelProcessDataDescLock = OsCreateLockTyped(eLockType_INTERFACE);
#endif
    m_oAtEcatDesc.poClntDescLock              = OsCreateLockTyped(eLockType_SPIN);
    m_oAtEcatDesc.dwClntDescInUse             = 0;
    m_oAtEcatDesc.byNextUniqueClntId          = EC_MAX_CLIENTS;
    m_oAtEcatDesc.byNumRegisteredClnts        = 0;

    /* get EtherCAT master object */
    m_oAtEcatDesc.poMaster = m_oAtEcatDesc.poEcDevice->GetMaster();
    if (EC_NULL == m_oAtEcatDesc.poMaster)
    {
        EC_ERRORMSGC(("ecatInitMaster() Invalid master pointer!\n"));
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    /* create base timer */
    m_oAtEcatDesc.dwOnMasterTimerCnt = 0;
    m_oAtEcatDesc.EMasterOpStateCyclicErrorNotificationsStatus = eEnableStatus_UNKNOWN;

    /* set master to init state */
    m_oAtEcatDesc.eMasterState = eEcatState_UNKNOWN;

    /* initialize master control state machine */
    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_UNKNOWN;
    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_UNKNOWN;
    m_oAtEcatDesc.EMcSmReqMasterOpState = eEcatState_UNKNOWN;

    /* initialize McSm oder management */
#ifdef DEBUGTRACE
    m_oAtEcatDesc.pFreeMcSmOrderFifoApi = EC_NEW(CFiFoListDyn<EC_T_MCSM_ORDER*>(EC_NUMOF_MCSM_ORDERS_API, EC_NULL, EC_NULL, EC_MEMTRACE_CORE_MASTER));
    m_oAtEcatDesc.pFreeMcSmOrderFifoInternal = EC_NEW(CFiFoListDyn<EC_T_MCSM_ORDER*>(EC_NUMOF_MCSM_ORDERS_INTERNAL, EC_NULL, EC_NULL, EC_MEMTRACE_CORE_MASTER));
    m_oAtEcatDesc.pFreeMcSmOrderFifoAutoRelease = EC_NEW(CFiFoListDyn<EC_T_MCSM_ORDER*>(EC_NUMOF_MCSM_ORDERS_AUTO_RELEASE, EC_NULL, EC_NULL, EC_MEMTRACE_CORE_MASTER));
    m_oAtEcatDesc.pNewPendingMcSmOrderFifo = EC_NEW(CFiFoListDyn<EC_T_MCSM_ORDER*>(EC_NUMOF_MCSM_ORDERS, EC_NULL, EC_NULL, EC_MEMTRACE_CORE_MASTER));
#else
    m_oAtEcatDesc.pFreeMcSmOrderFifoApi = EC_NEW(CFiFoListDyn<EC_T_MCSM_ORDER*>(EC_NUMOF_MCSM_ORDERS_API, EC_NULL, EC_NULL));
    m_oAtEcatDesc.pFreeMcSmOrderFifoInternal = EC_NEW(CFiFoListDyn<EC_T_MCSM_ORDER*>(EC_NUMOF_MCSM_ORDERS_INTERNAL, EC_NULL, EC_NULL));
    m_oAtEcatDesc.pFreeMcSmOrderFifoAutoRelease = EC_NEW(CFiFoListDyn<EC_T_MCSM_ORDER*>(EC_NUMOF_MCSM_ORDERS_AUTO_RELEASE, EC_NULL, EC_NULL));
    m_oAtEcatDesc.pNewPendingMcSmOrderFifo = EC_NEW(CFiFoListDyn<EC_T_MCSM_ORDER*>(EC_NUMOF_MCSM_ORDERS, EC_NULL, EC_NULL));
#endif
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "ecatInitMaster:m_oAtEcatDesc.pFreeMcSmOrderFifoApi", m_oAtEcatDesc.pFreeMcSmOrderFifoApi, sizeof(CFiFoListDyn<EC_T_MCSM_ORDER*>));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "ecatInitMaster:m_oAtEcatDesc.pFreeMcSmOrderFifoInternal", m_oAtEcatDesc.pFreeMcSmOrderFifoInternal, sizeof(CFiFoListDyn<EC_T_MCSM_ORDER*>));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "ecatInitMaster:m_oAtEcatDesc.pFreeMcSmOrderFifoAutoRelease", m_oAtEcatDesc.pFreeMcSmOrderFifoAutoRelease, sizeof(CFiFoListDyn<EC_T_MCSM_ORDER*>));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "ecatInitMaster:m_oAtEcatDesc.pNewPendingMcSmOrderFifo", m_oAtEcatDesc.pNewPendingMcSmOrderFifo, sizeof(CFiFoListDyn<EC_T_MCSM_ORDER*>));
    InitializeListHead(&m_oAtEcatDesc.oPendingMcSmOrderList);

    OsMemset(m_oAtEcatDesc.aMcSmOrders, 0, sizeof(m_oAtEcatDesc.aMcSmOrders));
    for (dwIdx = 0; dwIdx < EC_NUMOF_MCSM_ORDERS; dwIdx++)
    {
        EC_T_MCSM_ORDER* pMcSmOrder = &m_oAtEcatDesc.aMcSmOrders[dwIdx];

        pMcSmOrder->oTimeout = ::CEcTimer();

        /* add to free FIFO */
        if (EC_NUMOF_MCSM_ORDERS_API > dwIdx)
        {
            pMcSmOrder->pFreeMcSmOrderFifo = m_oAtEcatDesc.pFreeMcSmOrderFifoApi;
            m_oAtEcatDesc.pFreeMcSmOrderFifoApi->AddNoLock(pMcSmOrder);
        }
        else if ((EC_NUMOF_MCSM_ORDERS_API + EC_NUMOF_MCSM_ORDERS_INTERNAL) > dwIdx)
        {
            pMcSmOrder->pFreeMcSmOrderFifo = m_oAtEcatDesc.pFreeMcSmOrderFifoInternal;
            m_oAtEcatDesc.pFreeMcSmOrderFifoInternal->AddNoLock(pMcSmOrder);
        }
        else
        {
            pMcSmOrder->pFreeMcSmOrderFifo = m_oAtEcatDesc.pFreeMcSmOrderFifoAutoRelease;
            m_oAtEcatDesc.pFreeMcSmOrderFifoAutoRelease->AddNoLock(pMcSmOrder);
        }
    }
    CalculateDefaultMcSmOrderTimeout(pParms->dwBusCycleTimeUsec);
    {
    /* 0=off, 1=LogMsg, 2=LinkMsg, 3=LogMsg+Linkmsg */
    EC_T_BYTE byLogDst = (EC_T_BYTE)((m_oAtEcatDesc.pInitMasterParms->dwLogLevel >= EC_LOG_LEVEL_VERBOSE) ? ( 1 + (m_oAtEcatDesc.pInitMasterParms->bLogToLinkLayer ? 2 : 0)) : 0);

            m_oAtEcatDesc.dwStateChangeDebug = (/* BASE_CLOCK_DBG_LEVEL */                  0        << 18)
                                             | (/* MASTER_CONTROL_STATE_CHANGE_DBG_LEVEL */ byLogDst << 16)
                                             | (/* HOT_CONNECT_DBG_LEVEL */                 byLogDst << 4)
                                             | (/* MASTER_ENH_MODULE_DBG_LEVEL */           byLogDst << 2)
                                             | (/* STATE_CHANGE_DBG_LEVEL */                byLogDst);
    }

    /* create CoE emergency mbx transfer object */
    MbxTferDesc.dwMaxDataLen       = sizeof(ETHERCAT_EMERGENCY_HEADER);
    MbxTferDesc.pbyMbxTferDescData = (EC_T_PBYTE)OsMalloc(MbxTferDesc.dwMaxDataLen);
    EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:MbxTferDesc.pbyMbxTferDescData", MbxTferDesc.pbyMbxTferDescData, MbxTferDesc.dwMaxDataLen);
    OsDbgAssert( EC_NULL != MbxTferDesc.pbyMbxTferDescData);
    if (EC_NULL != MbxTferDesc.pbyMbxTferDescData)
    {
        OsMemset(MbxTferDesc.pbyMbxTferDescData, 0, MbxTferDesc.dwMaxDataLen);
    }
    m_pMbxCoeEmergencyTferPriv = (EC_T_MBXTFERP*)MbxTferCreate( &MbxTferDesc, GetMemLog() );
    OsDbgAssert( m_pMbxCoeEmergencyTferPriv != EC_NULL );
    m_pMbxCoeEmergencyTferPriv->oEcMbxCmd.pvContext         = m_pMbxCoeEmergencyTferPriv;
    m_pMbxCoeEmergencyTferPriv->oEcMbxCmd.dwInvokeId        = m_pMbxCoeEmergencyTferPriv->MbxTfer.dwTferId;
    m_pMbxCoeEmergencyTferPriv->oEcMbxCmd.length            = m_pMbxCoeEmergencyTferPriv->MbxTfer.dwDataLen;
    m_pMbxCoeEmergencyTferPriv->oEcMbxCmd.pbyData           = m_pMbxCoeEmergencyTferPriv->MbxTfer.pbyMbxTferData;
    m_pMbxCoeEmergencyTferPriv->oEcMbxCmd.bInUseByInterface = EC_FALSE;
    m_pMbxCoeEmergencyTferPriv->MbxTfer.eTferStatus         = eMbxTferStatus_Idle;
    m_pMbxCoeEmergencyTferPriv->MbxTfer.dwErrorCode         = EC_E_NOERROR;
    m_pMbxCoeEmergencyTferPriv->MbxTfer.dwClntId            = 0;
    m_pMbxCoeEmergencyTferPriv->MbxTfer.eMbxTferType        = eMbxTferType_COE_EMERGENCY;
    m_pMbxCoeEmergencyTferPriv->dwSlaveId                   = 0;
    m_oAtEcatDesc.poMaster->SetCoeEmergencyMbxCmd(&(m_pMbxCoeEmergencyTferPriv->oEcMbxCmd));

#if (defined INCLUDE_SOE_SUPPORT)
    /* create SoE emergency mbx transfer object */
    MbxTferDesc.dwMaxDataLen       = sizeof(ETHERCAT_SOE_EMERGENCY_HEADER);
    MbxTferDesc.pbyMbxTferDescData = (EC_T_PBYTE)OsMalloc(MbxTferDesc.dwMaxDataLen);
    EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:MbxTferDesc.pbyMbxTferDescData", MbxTferDesc.pbyMbxTferDescData, MbxTferDesc.dwMaxDataLen);
    OsDbgAssert( EC_NULL != MbxTferDesc.pbyMbxTferDescData);
    if (EC_NULL != MbxTferDesc.pbyMbxTferDescData)
    {
        OsMemset(MbxTferDesc.pbyMbxTferDescData, 0, MbxTferDesc.dwMaxDataLen);
    }
    m_pMbxSoeEmergencyTferPriv = (EC_T_MBXTFERP*)MbxTferCreate( &MbxTferDesc, GetMemLog() );
    OsDbgAssert( m_pMbxSoeEmergencyTferPriv != EC_NULL );
    m_pMbxSoeEmergencyTferPriv->oEcMbxCmd.pvContext         = m_pMbxSoeEmergencyTferPriv;
    m_pMbxSoeEmergencyTferPriv->oEcMbxCmd.dwInvokeId        = m_pMbxSoeEmergencyTferPriv->MbxTfer.dwTferId;
    m_pMbxSoeEmergencyTferPriv->oEcMbxCmd.length            = m_pMbxSoeEmergencyTferPriv->MbxTfer.dwDataLen;
    m_pMbxSoeEmergencyTferPriv->oEcMbxCmd.pbyData           = m_pMbxSoeEmergencyTferPriv->MbxTfer.pbyMbxTferData;
    m_pMbxSoeEmergencyTferPriv->oEcMbxCmd.bInUseByInterface = EC_FALSE;
    m_pMbxSoeEmergencyTferPriv->MbxTfer.eTferStatus         = eMbxTferStatus_Idle;
    m_pMbxSoeEmergencyTferPriv->MbxTfer.dwErrorCode         = EC_E_NOERROR;
    m_pMbxSoeEmergencyTferPriv->MbxTfer.dwClntId            = 0;
    m_pMbxSoeEmergencyTferPriv->MbxTfer.eMbxTferType        = eMbxTferType_SOE_EMERGENCY;
    m_pMbxSoeEmergencyTferPriv->dwSlaveId                   = 0;
    m_oAtEcatDesc.poMaster->SetSoeEmergencyMbxCmd(&(m_pMbxSoeEmergencyTferPriv->oEcMbxCmd));

    /* create SoE emergency mbx transfer object */
    MbxTferDesc.dwMaxDataLen       = sizeof(ETHERCAT_SOE_NOTIFICATION_HEADER);
    MbxTferDesc.pbyMbxTferDescData = (EC_T_PBYTE)OsMalloc(MbxTferDesc.dwMaxDataLen);
    EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:MbxTferDesc.pbyMbxTferDescData", MbxTferDesc.pbyMbxTferDescData, MbxTferDesc.dwMaxDataLen);
    OsDbgAssert( EC_NULL != MbxTferDesc.pbyMbxTferDescData);
    if (EC_NULL != MbxTferDesc.pbyMbxTferDescData)
    {
        OsMemset(MbxTferDesc.pbyMbxTferDescData, 0, MbxTferDesc.dwMaxDataLen);
    }
    m_pMbxSoeNotificationTferPriv = (EC_T_MBXTFERP*)MbxTferCreate( &MbxTferDesc, GetMemLog() );
    OsDbgAssert( m_pMbxSoeNotificationTferPriv != EC_NULL );
    m_pMbxSoeNotificationTferPriv->oEcMbxCmd.pvContext         = m_pMbxSoeNotificationTferPriv;
    m_pMbxSoeNotificationTferPriv->oEcMbxCmd.dwInvokeId        = m_pMbxSoeNotificationTferPriv->MbxTfer.dwTferId;
    m_pMbxSoeNotificationTferPriv->oEcMbxCmd.length            = m_pMbxSoeNotificationTferPriv->MbxTfer.dwDataLen;
    m_pMbxSoeNotificationTferPriv->oEcMbxCmd.pbyData           = m_pMbxSoeNotificationTferPriv->MbxTfer.pbyMbxTferData;
    m_pMbxSoeNotificationTferPriv->oEcMbxCmd.bInUseByInterface = EC_FALSE;
    m_pMbxSoeNotificationTferPriv->MbxTfer.eTferStatus         = eMbxTferStatus_Idle;
    m_pMbxSoeNotificationTferPriv->MbxTfer.dwErrorCode         = EC_E_NOERROR;
    m_pMbxSoeNotificationTferPriv->MbxTfer.dwClntId            = 0;
    m_pMbxSoeNotificationTferPriv->MbxTfer.eMbxTferType        = eMbxTferType_SOE_NOTIFICATION;
    m_pMbxSoeNotificationTferPriv->dwSlaveId                   = 0;
    m_oAtEcatDesc.poMaster->SetSoeNotificationMbxCmd(&(m_pMbxSoeNotificationTferPriv->oEcMbxCmd));
#endif
    /* EtherCAT descriptor is now initialized */
    m_oAtEcatDesc.bInitialized = EC_TRUE;

    /* initialize master internal performance measurement */
#if (defined INCLUDE_EC_INTERNAL_TSC_MEASUREMENT)
    PerfMeasInit(&G_TscMeasDesc, 0, NUM_EC_MSMT_POINTS, EC_NULL);
    ecatPerfMeasEnable(&G_TscMeasDesc);
#endif
    /* enable master timer */
    m_oAtEcatDesc.poMaster->SetMasterTimerEnabled(EC_TRUE);
#if (defined EVAL_VERSION)
    m_oAtEcatDesc.poMaster->SetTimeLimit(m_oAtEcatDesc.poMaster->GetTimeLimit() + (OsQueryMsecCount() % 200));
#endif
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (dwRetVal == EC_E_NOMEMORY)
    {
        if (EC_NULL != m_oAtEcatDesc.pInitMasterParms)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_oAtEcatDesc.pInitMasterParms->pLinkParms", m_oAtEcatDesc.pInitMasterParms->pLinkParms, m_oAtEcatDesc.pInitMasterParms->pLinkParms->dwSize);
            SafeOsFree(m_oAtEcatDesc.pInitMasterParms->pLinkParms);
#if (defined INCLUDE_RED_DEVICE)
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed", m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed, m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed->dwSize);
            SafeOsFree(m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed);
#endif
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_oAtEcatDesc.pInitMasterParms->pOsParms", m_oAtEcatDesc.pInitMasterParms->pOsParms, m_oAtEcatDesc.pInitMasterParms->pOsParms->dwSize);
            SafeOsFree(m_oAtEcatDesc.pInitMasterParms->pOsParms);
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_oAtEcatDesc.pInitMasterParms", m_oAtEcatDesc.pInitMasterParms, m_oAtEcatDesc.pInitMasterParms->dwSize);
            SafeOsFree(m_oAtEcatDesc.pInitMasterParms);
        }
    }

    /* cleanup */
    if (bFirstInitialization)
    {
        /* delete factory */
        if (EC_NULL != m_pFactory)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "ecatInitMaster:m_pFactory", m_pFactory, sizeof(CEcDeviceFactory));
            SafeDelete(m_pFactory);
        }
        /* feature field was allocated here */
        CEcMaster::FreeFeatureField(pFeatures, GetMemLog());
    }
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** */
/** \brief De-initialize the EtherCAT master stack

    This function releases all the resources created and initialized by the function MasterInit() and
    by using the EtherCAT master stack API.
*/
/** \return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::DeinitMaster(EC_T_VOID)
{
    /* disable jobs */
    DisableJobs(250);

    return DeinitMasterEx(EC_FALSE, EC_TRUE);
}

/***************************************************************************************************/
/** */
/** \brief De-initialize the EtherCAT master stack

    This function releases all the resources created and initialized by the function MasterInit() and
    by using the EtherCAT master stack API.
*/
/** \return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::DeinitMasterEx(
    EC_T_BOOL bDeinitForConfiguration,  /**< [in]  EC_TRUE if de-init only for ConfigureMaster() */
    EC_T_BOOL bResetStates              /**< [in]  EC_TRUE if state machines shall be reset */
)
{
    EC_T_DWORD     dwRetVal  = EC_E_NOERROR;
    EC_T_DWORD     dwRes     = EC_E_NOERROR;
    EC_T_CLNTDESC* pClntDesc = EC_NULL;
    CEcTimer oTimeout;

    if (!bDeinitForConfiguration)
    {
        /* de-initialize master internal performance measurement */
#if (defined INCLUDE_EC_INTERNAL_TSC_MEASUREMENT)
        PerfMeasDeinit(&G_TscMeasDesc);
#endif
    }
    if (bResetStates)
    {
        if (!m_oAtEcatDesc.bInitialized)
        {
            /* EC_ERRORMSGC(("DeinitMasterEx() Stack already de-initialized!\n")); */
            dwRetVal = EC_E_NOERROR;
            goto Exit;
        }
        /* cancel all orders */
        m_oAtEcatDesc.McSmCancelAllOrders();

        /* wait until McSm stable and order queue empty */
        dwRes = WaitUntilStableMcSm(ECAT_MCSM_STABLE_TIMEOUT_DEFAULT);
        if (EC_E_NOERROR != dwRes)
        {
            EC_ERRORMSGC(("DeinitMasterEx(): Master control state machine doesn't become stable\n"));
        }
    } /* if (bResetStates ) */

    /* */
    if (EC_NULL != m_oAtEcatDesc.poMaster)
    {
        m_oAtEcatDesc.poMaster->SetMasterTimerEnabled(EC_FALSE);

        /* remove CoE emergency mbx transfer object */

        m_oAtEcatDesc.poMaster->SetCoeEmergencyMbxCmd(EC_NULL);

        if (EC_NULL != m_pMbxCoeEmergencyTferPriv)
        {
            while (m_pMbxCoeEmergencyTferPriv->MbxTfer.eTferStatus != eMbxTferStatus_Idle)
            {
                /* waiting for master to return the mailbox transfer object */
                EC_ERRORMSGC(("DeinitMasterEx(): Waiting for master to return the emergency mailbox transfer object...\n"));
                OsSleep(1);
            }

            if (EC_NULL != m_pMbxCoeEmergencyTferPriv->oEcMbxCmd.pbyData)
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "DeinitMasterEx", m_pMbxCoeEmergencyTferPriv->oEcMbxCmd.pbyData, m_pMbxCoeEmergencyTferPriv->oEcMbxCmd.length);
                OsFree(m_pMbxCoeEmergencyTferPriv->oEcMbxCmd.pbyData);
                m_pMbxCoeEmergencyTferPriv->oEcMbxCmd.pbyData = EC_NULL;
            }

            MbxTferDelete((EC_T_MBXTFER*)m_pMbxCoeEmergencyTferPriv, GetMemLog());
        }
        m_pMbxCoeEmergencyTferPriv = EC_NULL;
#if (defined INCLUDE_SOE_SUPPORT)
        /* remove SoE emergency mbx transfer object */
        m_oAtEcatDesc.poMaster->SetSoeEmergencyMbxCmd(EC_NULL);

        if (EC_NULL != m_pMbxSoeEmergencyTferPriv)
        {
            while (eMbxTferStatus_Idle != m_pMbxSoeEmergencyTferPriv->MbxTfer.eTferStatus)
            {
                /* waiting for master to return the mailbox transfer object */
                EC_DBGMSG("DeinitMasterEx(): Waiting for master to return the Soe emergency mailbox transfer object...\n");
                OsSleep(1);
            }

            if (EC_NULL != m_pMbxSoeEmergencyTferPriv->oEcMbxCmd.pbyData)
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "DeinitMasterEx", m_pMbxSoeEmergencyTferPriv->oEcMbxCmd.pbyData, m_pMbxSoeEmergencyTferPriv->oEcMbxCmd.length);
                OsFree(m_pMbxSoeEmergencyTferPriv->oEcMbxCmd.pbyData);
                m_pMbxSoeEmergencyTferPriv->oEcMbxCmd.pbyData = EC_NULL;
            }

            MbxTferDelete((EC_T_MBXTFER*)m_pMbxSoeEmergencyTferPriv, GetMemLog());
        }
        m_pMbxSoeEmergencyTferPriv = EC_NULL;

        /* remove SoE notification mbx transfer object */

        m_oAtEcatDesc.poMaster->SetSoeNotificationMbxCmd(EC_NULL);

        if (EC_NULL != m_pMbxSoeNotificationTferPriv)
        {
            while (eMbxTferStatus_Idle != m_pMbxSoeNotificationTferPriv->MbxTfer.eTferStatus)
            {
                /* waiting for master to return the mailbox transfer object */
                EC_DBGMSG("DeinitMasterEx(): Waiting for master to return the Soe notification mailbox transfer object...\n");
                OsSleep(1);
            }

            if (EC_NULL != m_pMbxSoeNotificationTferPriv->oEcMbxCmd.pbyData)
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "DeinitMasterEx", m_pMbxSoeNotificationTferPriv->oEcMbxCmd.pbyData, m_pMbxSoeNotificationTferPriv->oEcMbxCmd.length);
                OsFree(m_pMbxSoeNotificationTferPriv->oEcMbxCmd.pbyData);
                m_pMbxSoeNotificationTferPriv->oEcMbxCmd.pbyData = EC_NULL;
            }

            MbxTferDelete((EC_T_MBXTFER*)m_pMbxSoeNotificationTferPriv, GetMemLog());
        }
        m_pMbxSoeNotificationTferPriv = EC_NULL;
#endif
    }
    /* reset initialized flag */
    m_oAtEcatDesc.bInitialized = EC_FALSE;
    if (EC_NULL != m_oAtEcatDesc.poMaster)
    {
        m_oAtEcatDesc.poMaster->SetConfigLoaded(EC_FALSE);
        m_oAtEcatDesc.poMaster->SetConfigured(EC_FALSE);
    }
    /* cleanup client descriptor list */
    pClntDesc = m_oAtEcatDesc.pClntDescHead;
    while (EC_NULL != pClntDesc)
    {
        EC_T_DWORD          dwDeinitForConfiguration = bDeinitForConfiguration ? 1 : 0;
        EC_T_CLNTDESC*      pClntDescTmp   = pClntDesc;
        EC_T_NOTIFYPARMS    oNotifyParms;
        OsMemset(&oNotifyParms, 0, sizeof(EC_T_NOTIFYPARMS));
        
        oNotifyParms.pCallerData  = pClntDesc->pCallerData;
        oNotifyParms.pbyInBuf     = (EC_T_BYTE*)&dwDeinitForConfiguration;
        oNotifyParms.dwInBufSize  = sizeof(EC_T_DWORD);

        if (EC_NULL != pClntDesc->S_pfnNotify)
        {
            pClntDesc->S_pfnNotify(EC_NOTIFY_CLIENTREGISTRATION_DROPPED, &oNotifyParms);
        }

        pClntDesc = pClntDesc->pNext;
        EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "DeinitMasterEx", pClntDescTmp, sizeof(EC_T_CLNTDESC));
        OsFree(pClntDescTmp);
    }
    m_oAtEcatDesc.pClntDescHead = EC_NULL;

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
    if (EC_NULL != m_oAtEcatDesc.poMaster)
    {
        m_oAtEcatDesc.poMaster->PassThroughSrvStop(1000);
    }
#endif

    if (bResetStates)
    {
#if defined WIN32 && !defined UNDER_CE
        if (EC_NULL != m_oAtEcatDesc.poEcDevice)
        {
            /* Windows XP WinPCap layer sometimes has problems.... */
            m_oAtEcatDesc.poEcDevice->LinkIoctl(EC_LINKIOCTL_RESTART, EC_NULL);
        }

#if (defined INCLUDE_RED_DEVICE)
        if (EC_NULL != m_oAtEcatDesc.poEcRedDevice)
        {
            m_oAtEcatDesc.poEcRedDevice->LinkIoctl(EC_LINKIOCTL_RESTART, EC_NULL);
        }
#endif /* INCLUDE_RED_DEVICE */
#endif /* WIN32 && !UNDER_CE */
    }

Exit:
    /* cleanup */
    if (EC_NULL != m_oAtEcatDesc.poEcDevice)
    {
        if (EC_NULL != m_oAtEcatDesc.poMaster)
        {
            /* release preallocated frames IN_DMA mode */
            if (eCycFrameLayout_IN_DMA == m_oAtEcatDesc.poMaster->GetCycFrameLayout())
            {
                EC_T_DWORD dwIdxEntry = 0;
                EC_T_DWORD dwIdxFrame = 0;
                CEcMaster* poMaster = m_oAtEcatDesc.poMaster;
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
                for (dwIdxEntry = 0; dwIdxEntry < poMaster->GetNumCycConfigEntries(); dwIdxEntry++)
#endif
                {
                    CYCLIC_ENTRY_MGMT_DESC* pCycEntry = poMaster->GetCycEntryDesc(dwIdxEntry);
                    if (EC_NULL != pCycEntry)
                    {
                        for (dwIdxFrame = 0; dwIdxFrame < pCycEntry->wNumFrames; dwIdxFrame++)
                        {
                            EcCycFrameDesc* pFrame = pCycEntry->apEcCycFrameDesc[dwIdxFrame];
                            EC_T_LINK_FRAMEDESC oLinkFrameDesc;
                            OsMemset(&oLinkFrameDesc, 0, sizeof(EC_T_LINK_FRAMEDESC));

                            if (EC_NULL == pFrame->pbyFrame)
                                continue;
                            /* fill link frame description and free frame */
                            oLinkFrameDesc.pbyFrame = pFrame->pbyFrame;
                            oLinkFrameDesc.pvContext = pFrame->pvContext;
                            m_oAtEcatDesc.poEcDevice->FrameFree(&oLinkFrameDesc);
                        }
                    }
                }
            }
            m_oAtEcatDesc.poMaster->ReturnPDInBasePointer();
        }

        m_oAtEcatDesc.poMaster = EC_NULL;
        m_oAtEcatDesc.poEcDevice->CloseDevice(bDeinitForConfiguration);
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_FACTORY, "CEcDeviceFactory::DeleteDevice", m_oAtEcatDesc.poEcDevice, sizeof(CEcDevice));
        SafeDelete(m_oAtEcatDesc.poEcDevice)

#if (defined INCLUDE_RED_DEVICE)
        if (EC_NULL != m_oAtEcatDesc.poEcRedDevice)
        {
            m_oAtEcatDesc.poEcRedDevice->CloseDevice(bDeinitForConfiguration);
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "DeinitMasterEx:m_oAtEcatDesc.poEcRedDevice", m_oAtEcatDesc.poEcRedDevice, sizeof(CEcRedDevice));
            SafeDelete(m_oAtEcatDesc.poEcRedDevice);
        }
#endif
    }
    if (EC_NULL != m_oAtEcatDesc.poGetProcessDataDescLock)
    {
        oTimeout.Start(2000);
        while(!oTimeout.IsElapsed() && m_oAtEcatDesc.oGetProcessDataDesc.bRequestStarted)
        {
            OsSleep(1);
        }
        OsDeleteLock(m_oAtEcatDesc.poGetProcessDataDescLock);
        m_oAtEcatDesc.poGetProcessDataDescLock = EC_NULL;
    }
    if (EC_NULL != m_oAtEcatDesc.poSetProcessDataDescLock)
    {
        oTimeout.Start(2000);
        while (!oTimeout.IsElapsed() && m_oAtEcatDesc.oSetProcessDataDesc.bRequestStarted)
        {
            OsSleep(1);
        }
        OsDeleteLock(m_oAtEcatDesc.poSetProcessDataDescLock);
        m_oAtEcatDesc.poSetProcessDataDescLock = EC_NULL;
    }
#if (defined INCLUDE_FORCE_PROCESSDATA)
    if (EC_NULL != m_oAtEcatDesc.poForceRelProcessDataDescLock)
    {
        oTimeout.Start(2000);
        while (!oTimeout.IsElapsed() && m_oAtEcatDesc.oForceProcessDataDesc.bRequestStarted)
        {
            OsSleep(1);
        }
        OsDeleteLock(m_oAtEcatDesc.poForceRelProcessDataDescLock);
        m_oAtEcatDesc.poForceRelProcessDataDescLock = EC_NULL;
    }
#endif
    if (EC_NULL != m_oAtEcatDesc.poClntDescLock)
    {
        OsDeleteLock(m_oAtEcatDesc.poClntDescLock);
        m_oAtEcatDesc.poClntDescLock = EC_NULL;
    }
    /* de-init master control state machine */
    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_UNKNOWN;
    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_UNKNOWN;
    m_oAtEcatDesc.EMcSmReqMasterOpState = eEcatState_UNKNOWN;

    OsDbgAssert(m_oAtEcatDesc.poEcDevice == EC_NULL);
    OsDbgAssert(m_oAtEcatDesc.poEcRedDevice == EC_NULL);

    if (EC_NULL != m_oAtEcatDesc.poMcSlaveErrAckRetryTimeout)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "DeinitMasterEx:m_oAtEcatDesc.poMcSlaveErrAckRetryTimeout", m_oAtEcatDesc.poMcSlaveErrAckRetryTimeout, sizeof(CEcTimer));
        SafeDelete(m_oAtEcatDesc.poMcSlaveErrAckRetryTimeout);
    }
    /* de-init McSm oder management */
    if (EC_NULL != m_oAtEcatDesc.pFreeMcSmOrderFifoApi)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "DeinitMasterEx:m_oAtEcatDesc.pFreeMcSmOrderFifoApi", m_oAtEcatDesc.pFreeMcSmOrderFifoApi, sizeof(CFiFoListDyn<EC_T_MCSM_ORDER*>));
        SafeDelete(m_oAtEcatDesc.pFreeMcSmOrderFifoApi);
    }
    if (EC_NULL != m_oAtEcatDesc.pFreeMcSmOrderFifoInternal)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "DeinitMasterEx:m_oAtEcatDesc.pFreeMcSmOrderFifoInternal", m_oAtEcatDesc.pFreeMcSmOrderFifoInternal, sizeof(CFiFoListDyn<EC_T_MCSM_ORDER*>));
        SafeDelete(m_oAtEcatDesc.pFreeMcSmOrderFifoInternal);
    }
    if (EC_NULL != m_oAtEcatDesc.pFreeMcSmOrderFifoAutoRelease)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "DeinitMasterEx:m_oAtEcatDesc.pFreeMcSmOrderFifoAutoRelease", m_oAtEcatDesc.pFreeMcSmOrderFifoAutoRelease, sizeof(CFiFoListDyn<EC_T_MCSM_ORDER*>));
        SafeDelete(m_oAtEcatDesc.pFreeMcSmOrderFifoAutoRelease);
    }
    if (EC_NULL != m_oAtEcatDesc.pNewPendingMcSmOrderFifo)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "DeinitMasterEx:m_oAtEcatDesc.pNewPendingMcSmOrderFifo", m_oAtEcatDesc.pNewPendingMcSmOrderFifo, sizeof(CFiFoListDyn<EC_T_MCSM_ORDER*>));
        SafeDelete(m_oAtEcatDesc.pNewPendingMcSmOrderFifo);
    }
#if (defined INCLUDE_CONFIG_EXTEND)
    /* delete extended config data storage */
    if (EC_NULL != m_poEcConfigDataExtended)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "ecatDeinitMaster:m_poEcConfigDataExtended", m_poEcConfigDataExtended, sizeof(CEcConfigData));
        SafeDelete(m_poEcConfigDataExtended);
    }
#endif

    if (!bDeinitForConfiguration)
    {
        if (EC_NULL != m_oAtEcatDesc.pInitMasterParms)
        {
            if (m_oAtEcatDesc.pInitMasterParms->pLinkParms)
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "DeinitMasterEx:m_oAtEcatDesc.pInitMasterParms->pLinkParms", m_oAtEcatDesc.pInitMasterParms->pLinkParms, m_oAtEcatDesc.pInitMasterParms->pLinkParms->dwSize);
                SafeOsFree(m_oAtEcatDesc.pInitMasterParms->pLinkParms);
            }
#if (defined INCLUDE_RED_DEVICE)
            if (m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed)
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "DeinitMasterEx:m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed", 
                    m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed, m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed->dwSize);
                SafeOsFree(m_oAtEcatDesc.pInitMasterParms->pLinkParmsRed);
            }
#endif
            if (m_oAtEcatDesc.pInitMasterParms->pOsParms)
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "DeinitMasterEx:m_oAtEcatDesc.pInitMasterParms->pOsParms", 
                    m_oAtEcatDesc.pInitMasterParms->pOsParms, m_oAtEcatDesc.pInitMasterParms->pOsParms->dwSize);
                SafeOsFree(m_oAtEcatDesc.pInitMasterParms->pOsParms);
            }
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "DeinitMasterEx:m_oAtEcatDesc.pInitMasterParms", m_oAtEcatDesc.pInitMasterParms, m_oAtEcatDesc.pInitMasterParms->dwSize);
            SafeOsFree(m_oAtEcatDesc.pInitMasterParms);
        }
        /* delete factory */
        if (EC_NULL != m_pFactory)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "DeinitMasterEx:m_pFactory", m_pFactory, sizeof(CEcDeviceFactory));
            SafeDelete(m_pFactory);
        }
        /* delete config data storage */
        if (EC_NULL != m_poEcConfigData)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "ecatDeinitMaster:m_poEcConfigData", m_poEcConfigData, sizeof(CEcConfigData));
            SafeDelete(m_poEcConfigData);
        }
        EcMemLogFree(GetMemLog());

        /* de-initialize OS layer */
        dwRetVal = OsDeinit();
    }
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \return EC_TRUE if ADS or Pass Through Server is running
*/
EC_T_BOOL CAtEmInterface::AdsIsRunning()
{
#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
    if (m_oAtEcatDesc.poMaster->m_oPtsControlDesc.eCurPtsState == ePtsStateRunningEnabled)
    {
        return EC_TRUE;
    }
#endif

#if (defined INCLUDE_ADS_ADAPTER) && (defined EC_SOCKET_IP_SUPPORTED)
    if (EC_NULL != m_pAdsSrvHandle)
    {
        return EC_TRUE;
    }
#endif

    return EC_FALSE;
}

EC_T_DWORD CAtEmInterface::ConfigLoad(EC_T_CNF_TYPE eCnfType, EC_T_PBYTE pbyCnfData, EC_T_DWORD dwCnfDataLen)
{
EC_T_DWORD                  dwRetVal               = EC_E_ERROR;
EC_T_DWORD                  dwRes                  = 0;
EC_T_DWORD                  dwInstanceID           = 0;
EC_T_INIT_MASTER_PARMS*     pInitMasterParmsBackup = {0};
EC_T_INTFEATUREFIELD_DESC*  pFeaturesBackup        = EC_NULL;
EC_T_LINK_DEV_RESTORE_PARAM oRestoreParam          = {{0}};
EC_T_LINK_DEV_RESTORE_PARAM oRedRestoreParam       = {{0}};
EC_T_CYCFRAME_RX_CBDESC     oCycRxDescBackup       = {0};

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
EC_T_IPADDR                 oPtsIpAddrBackup       = {0};
EC_T_WORD                   wPtsPortBackup         = 0;
EC_T_DWORD                  dwPtsThreadPriority    = 0;
EC_PTS_STATE                eCurPtsStateBackup     = ePtsStateNone;
#endif

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ConfigLoad() Temporary Master Stack not Initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check if ADS adapter is running */
    if (AdsIsRunning())
    {
        dwRetVal = EC_E_ADS_IS_RUNNING;
        goto Exit;
    }
    /* set master to INIT */
    if (DEVICE_STATE_UNKNOWN != m_oAtEcatDesc.poMaster->GetMasterHighestDeviceState()
        && DEVICE_STATE_INIT != m_oAtEcatDesc.poMaster->GetMasterHighestDeviceState())
    {
        dwRes = SetMasterStateEx(m_oAtEcatDesc.poMaster->GetStateChangeDefaultTimeout(), eEcatState_INIT);
        if (EC_E_NOERROR != dwRes)
        {
            EC_DBGMSG("ConfigLoad(): could not set master into INIT state Error=0x%08X\n", dwRes);
        }
    }
    /* disable jobs */
    DisableJobs(250);

    /* wait until the McSm is stable and the order queue is empty */
    dwRes = WaitUntilStableMcSm(ECAT_MCSM_STABLE_TIMEOUT_DEFAULT);
    if (EC_E_NOERROR != dwRes)
    {
        EC_ERRORMSGC(("DeinitMasterEx(): Master control state machine doesn't become stable\n"));
    }
    /* get master init parameters and features */
    pInitMasterParmsBackup = m_oAtEcatDesc.pInitMasterParms;
    pFeaturesBackup        = m_oAtEcatDesc.poMaster->GetFeatureField();

    /* flush rx frames */
    m_oAtEcatDesc.poEcDevice->DeviceFlushRecvFrames();
#if (defined INCLUDE_RED_DEVICE)
    if (EC_NULL != m_oAtEcatDesc.poEcRedDevice)
    {
        m_oAtEcatDesc.poEcRedDevice->DeviceFlushRecvFrames();
    }
#endif
    /* store link layers descriptors and set the validation pattern to 0 avoid the de-initialisation of the LinkLayer */
    OsMemcpy(&(oRestoreParam.szDriverIdent), m_oAtEcatDesc.poEcDevice->CEcDeviceBase::m_szDriverIdent, MAX_DRIVER_IDENT_LEN);
    OsMemcpy(&(oRestoreParam.oLinkDrv), &(m_oAtEcatDesc.poEcDevice->m_LinkDrvDesc), sizeof(EC_T_LINK_DRV_DESC));
    m_oAtEcatDesc.poEcDevice->m_LinkDrvDesc.dwValidationPattern = 0;
#if (defined INCLUDE_RED_DEVICE)
    if (EC_NULL != m_oAtEcatDesc.poEcRedDevice)
    {
        OsMemcpy(&(oRedRestoreParam.szDriverIdent), m_oAtEcatDesc.poEcRedDevice->CEcDeviceBase::m_szDriverIdent, MAX_DRIVER_IDENT_LEN);
        OsMemcpy(&(oRedRestoreParam.oLinkDrv), &(m_oAtEcatDesc.poEcRedDevice->m_LinkDrvDesc), sizeof(EC_T_LINK_DRV_DESC));
        m_oAtEcatDesc.poEcRedDevice->m_LinkDrvDesc.dwValidationPattern = 0;
    }
#endif
    /* store cyclic RX descriptor */
    OsMemcpy(&oCycRxDescBackup, &m_oAtEcatDesc.oCycRxDesc, sizeof(EC_T_CYCFRAME_RX_CBDESC));

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
    /* we have to stop the running pts server because the master object (that includes the pts)
       will be destroyed and recreated afterwards */
    if (   m_oAtEcatDesc.poMaster->m_oPtsControlDesc.eCurPtsState == ePtsStateRunningEnabled
        || m_oAtEcatDesc.poMaster->m_oPtsControlDesc.eCurPtsState == ePtsStateRunningDisabled)
    {
        /* back up the pts start parameter for the restart */
        OsMemcpy(&oPtsIpAddrBackup, &m_oAtEcatDesc.poMaster->m_oPtsControlDesc.oIpAddr, sizeof(oPtsIpAddrBackup));
        wPtsPortBackup = m_oAtEcatDesc.poMaster->m_oPtsControlDesc.wPort;
        dwPtsThreadPriority = m_oAtEcatDesc.poMaster->m_oPtsControlDesc.dwPtsThreadPriority;
        eCurPtsStateBackup = m_oAtEcatDesc.poMaster->m_oPtsControlDesc.eCurPtsState;

        /* stop the pts */
        m_oAtEcatDesc.poMaster->PassThroughSrvStop(1000);
    }
#endif
    /* backup instance ID */    
    dwInstanceID = m_oAtEcatDesc.dwInstanceID;

    /* deinit master */
    dwRes = DeinitMasterEx(EC_TRUE, EC_TRUE);
    if (EC_E_NOERROR != dwRes)
    {
        EC_ERRORMSG(("ConfigLoad() Cannot release Temporary Master Stack!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* init master */
    dwRes = InitMasterEx(dwInstanceID, EC_FALSE, pInitMasterParmsBackup, pFeaturesBackup, &oRestoreParam, &oRedRestoreParam, &oCycRxDescBackup, eCnfType, pbyCnfData, dwCnfDataLen);
    if (EC_E_NOERROR != dwRes)
    {
        EC_ERRORMSGC(("ConfigLoad() Cannot initialize Master Stack!\n"));
        dwRetVal = dwRes;
        goto Exit;
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
    /* check if we have to restart the pts */
    if (eCurPtsStateBackup != ePtsStateNone)
    {
        EC_T_PTS_SRV_START_PARMS oPtsSrvStartParms;

        /* use the previously stored back up parameter...  */
        OsMemcpy(&oPtsSrvStartParms.oIpAddr, &oPtsIpAddrBackup, sizeof(oPtsSrvStartParms.oIpAddr));
        oPtsSrvStartParms.wPort = wPtsPortBackup;
        oPtsSrvStartParms.dwPtsThreadPriority = dwPtsThreadPriority;

        /* ... and restart the pts */
        m_oAtEcatDesc.poMaster->PassThroughSrvStart(&oPtsSrvStartParms, 1000);
    }
#endif

Exit:
    /* cleanup */
    if (EC_E_NOERROR != dwRetVal)
    {
        /* De-Init Master in case of error, destructors are not called afterwards again */
        DeinitMasterEx(EC_TRUE, EC_FALSE);

        /* Handle the de-initialization of the LinkLayer(s) in case of an error before OpenDevice was called.
           (DeinitMasterEx has not de-initialized the LinkLayer in this case because of m_oAtEcatDesc.poEcDevice->m_LinkDrvDesc.dwValidationPattern == 0)
           Now call the close function of the link layer if needed.
        */
        if (LINK_LAYER_DRV_DESC_PATTERN == oRestoreParam.oLinkDrv.dwValidationPattern)
        {
            dwRes = (*oRestoreParam.oLinkDrv.pfEcLinkClose)(oRestoreParam.oLinkDrv.pvLinkInstance);
            if (EC_E_NOERROR != dwRes)
            {
                EC_ERRORMSG(("ConfigLoad()...LinkLayer: Failed to close link layer!\n"));
            }
        }
#if (defined INCLUDE_RED_DEVICE)
        if (LINK_LAYER_DRV_DESC_PATTERN == oRedRestoreParam.oLinkDrv.dwValidationPattern )
        {
            dwRes = (*oRedRestoreParam.oLinkDrv.pfEcLinkClose)(oRedRestoreParam.oLinkDrv.pvLinkInstance);
            if (EC_E_NOERROR != dwRes)
            {
                EC_ERRORMSG(("ConfigLoad()...RedLinkLayer: Failed to close link layer!\n"));
            }
        }
#endif
        if (EC_NULL != m_pFactory)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "ConfigLoad:m_pFactory", m_pFactory, sizeof(CEcDeviceFactory));
            SafeDelete(m_pFactory);
        }

        if (EC_NULL != m_poEcConfigData)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "ConfigLoad:m_poEcConfigData", m_poEcConfigData, sizeof(CEcConfigData));
            SafeDelete(m_poEcConfigData);
        }
    }
    CEcMaster::FreeFeatureField(pFeaturesBackup, GetMemLog());

    if ((eCnfType_None != eCnfType) && (EC_E_NOERROR == dwRetVal))
    {
        m_oAtEcatDesc.poMaster->SetConfigLoaded(EC_TRUE);
    }
    else
    {
        if (EC_NULL != m_oAtEcatDesc.poMaster)
        {
            m_oAtEcatDesc.poMaster->SetConfigLoaded(EC_FALSE);
        }
    }
    return OsSetLastError(dwRetVal);
}

EC_T_DWORD CAtEmInterface::ConfigExcludeSlave(EC_T_WORD wPhysAddress)
{
    if (m_oAtEcatDesc.poMaster->IsConfigLoaded())
    {
        return m_pFactory->ConfigExcludeSlave(m_oAtEcatDesc.poEcDevice, wPhysAddress);
    }
    else
    {
        return EC_E_INVALIDSTATE;
    }
}

EC_T_DWORD CAtEmInterface::ConfigIncludeSlave(EC_T_WORD wPhysAddress)
{
    if (m_oAtEcatDesc.poMaster->IsConfigLoaded())
    {
        return m_pFactory->ConfigIncludeSlave(m_oAtEcatDesc.poEcDevice, wPhysAddress);
    }
    else
    {
        return EC_E_INVALIDSTATE;
    }
}

EC_T_DWORD CAtEmInterface::ConfigSetPreviousPort(EC_T_WORD wPhysAddress, EC_T_WORD wPhysAddressPrev, EC_T_WORD wPortPrev)
{
    if (m_oAtEcatDesc.poMaster->IsConfigLoaded())
    {
        return m_pFactory->ConfigSetPreviousPort(m_oAtEcatDesc.poEcDevice, wPhysAddress, wPhysAddressPrev, wPortPrev);
    }
    else
    {
        return EC_E_INVALIDSTATE;
    }
}

EC_T_DWORD CAtEmInterface::ConfigApply(EC_T_VOID)
{
EC_T_DWORD        dwRetVal  = EC_E_ERROR;
EC_T_DWORD        dwRes     = 0;

    if (m_oAtEcatDesc.poMaster->IsConfigLoaded())
    {
        dwRes = m_pFactory->ConfigApply(m_oAtEcatDesc.poEcDevice, EC_FALSE);
        if (EC_E_NOERROR != dwRes)
        {
            EC_ERRORMSGC(("ConfigApply() Cannot apply the configuration to the EtherCAT device!\n"));
            dwRetVal = dwRes;
            goto Exit;
        }
        /* callback function to inform application that all cyclic frame(s) is(are) received */
        if (m_oAtEcatDesc.oCycRxDesc.pfnCallback != EC_NULL)
        {
            m_oAtEcatDesc.poMaster->SetCycFrameReceivedCallback(&m_oAtEcatDesc.oCycRxDesc);
        }
        /* no errors */
        m_oAtEcatDesc.poMaster->SetConfigured(EC_TRUE);

        /* trigger a bus scan */
        dwRetVal = McSmOrderAdd(eMcSmOrderType_REQ_SB, EC_NULL, EC_TRUE, EC_WAITINFINITE);
    }
    /* enable jobs now */
    EnableJobs();

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    /* delete the factory */
    if (m_pFactory != EC_NULL)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "ConfigApply:m_pFactory", m_pFactory, sizeof(CEcDeviceFactory));
        SafeDelete(m_pFactory);
    }
    /* cleanup on error */
    if (EC_E_NOERROR != dwRetVal)
    {
        ConfigLoad(eCnfType_None, EC_NULL, 0);
    }
    return dwRetVal;
}

#if (defined INCLUDE_GEN_OP_ENI) && (defined INCLUDE_CONFIG_EXTEND)
EC_T_DWORD CAtEmInterface::ConfigExtend(EC_T_BOOL bResetConfig, EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_NOERROR;
    EC_T_DWORD dwIdx    = 0;
    CEcTimer   oTimeout;
    EC_T_DWORD dwRemainingTime = dwTimeout;
    EC_T_BOOL bNotificationsMasked = EC_FALSE;

    CEcMaster*      poMaster      = EC_NULL;
    CEcBusTopology* poBusTopology = EC_NULL;

    CEcSlave**   apoSlavesExtend = EC_NULL;
    CEcSlave**   apoSlaves       = EC_NULL;
    CEcMbSlave** apoMbSlaves     = EC_NULL;

    CEcBusSlaveGenEni* poBusSlaveGenEniPool = EC_NULL;
    EC_T_BYTE*         pbyBusSlavesMem      = EC_NULL;
    EC_T_DWORD         dwBusSlavesMemSize   = 0;

    EC_T_WORD  wMaxNumSlaves    = 0;
    EC_T_WORD  wSlaveTotalCnt   = 0;
    EC_T_WORD  wMbSlaveTotalCnt = 0;
    EC_T_DWORD dwUnexpectedBusSlaveCnt = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("emConfigExtend(): Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* make some local copy */
    poMaster      = m_oAtEcatDesc.poMaster;
    poBusTopology = poMaster->m_pBusTopology;
    dwUnexpectedBusSlaveCnt = poBusTopology->GetUnexpectedSlaveCount();

    /* check parms */
    if (EC_NOWAIT == dwTimeout)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* start timeout */
    oTimeout.Start(dwTimeout);

    /* disable notifications */
    poMaster->m_pBusTopology->SetPortOperationsEnabled(EC_FALSE);
    poMaster->IncNotifySBStatusMaskedCnt();
    poMaster->IncNotifySBMismatchMaskedCnt();
    bNotificationsMasked = EC_TRUE;

    /* reset config */
    if (bResetConfig && (EC_NULL != m_poEcConfigDataExtended))
    {
        EC_T_DWORD dwOrderId = 0;        
        EC_T_MCSM_ORDER* poMcSmOrder = EC_NULL;
        EC_T_MCSM_ORDER_CONTEXT oContext;
        OsMemset(&oContext, 0, sizeof(EC_T_MCSM_ORDER_CONTEXT));

        dwRemainingTime = oTimeout.GetRemainingTime();
        if (0 == dwRemainingTime)
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }

        oContext.ConfigExtend.bMcSmRequestResetConfig = EC_TRUE;
        dwRes = McSmOrderAdd(eMcSmOrderType_REQ_EXTENDED_CONFIG, &poMcSmOrder, EC_FALSE, dwRemainingTime, eEcatState_UNKNOWN, eEcatMcSmState_UNKNOWN, &oContext);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
        dwOrderId = poMcSmOrder->dwOrderId;

        for (;;)
        {
            /* sleep a while */
            OsSleep(1);

            /* purge extended config if requested from McSm */
            if (poMcSmOrder->pContext->ConfigExtend.bApiRequestResetConfig)
            {
                EC_T_DWORD dwNumSlavesExtended = m_poEcConfigDataExtended->GetNumSlaves();

                for (dwIdx = dwNumSlavesExtended; 0 != dwIdx; dwIdx--)
                {
                    CEcSlave* poSlave = poMaster->GetSlaveByCfgFixedAddr(m_poEcConfigDataExtended->GetEniSlave(dwIdx - 1)->wPhysAddr);
                    poSlave->SetRemoveOnTimer(EC_TRUE);

                    while (poSlave->ShallRemoveOnTimer())
                    {
                        /* sleep a while */
                        OsSleep(1);
                    }
                    EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend ~poSlavesExtend", poSlave, poSlave->GetThisSize());
                    SafeDelete(poSlave);
                }
                EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend ~m_poEcConfigDataExtended", m_poEcConfigDataExtended, sizeof(CEcConfigData));
                SafeDelete(m_poEcConfigDataExtended);

                /* recreate cfg topology */
                poMaster->SBDeleteCfgTopology();
                poMaster->SBCreateCfgTopology();

                /* redetermine free fixed address ranges */
                for (dwIdx = 0; dwIdx < poMaster->SBFreeRanges(); dwIdx++)
                {
                    ECX_T_ADDRRANGE* poRange = m_poEcConfigData->GetAddressRange(dwIdx);

                    if (EC_NULL != poRange)
                    {
                        poMaster->SBSetFreeRange(dwIdx, poRange->wBegin, poRange->wEnd);
                    }
                    else
                    {
                        poMaster->SBSetFreeRange(dwIdx, 0, 0);
                    }
                }
                /* confirm reset request proceeded to McSm */
                poMcSmOrder->pContext->ConfigExtend.bApiRequestResetConfig = EC_FALSE;
            }
            /* wait until the order is finished */
            if (poMcSmOrder->eOrderState == eEcatOrderState_Finished)
            {
                if (dwOrderId != poMcSmOrder->dwOrderId)
                {
                    /* order was overwritten, should never happen */
                    OsDbgAssert(EC_FALSE);
                    dwRetVal = EC_E_BUSY;
                    goto Exit;
                }
                /* requested state is reached */
                dwRes = poMcSmOrder->dwMcSmLastErrorStatus;
                if (EC_E_NOERROR != dwRes)
                {
                    EC_DBGMSG("emConfigExtend()(Purge extended config): Error 0x%x in McSm state '%s' for requested state '%s'\n",
                        dwRetVal,
                        EcMcSmStateString(poMcSmOrder->EMcSmErrorState),
                        EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                    dwRetVal = dwRes;
                    goto Exit;
                }
                break;
            }
        }
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    /* if neccessary trigger a bus scan to determine number of unknown bus slaves */
    if (bResetConfig || (0 == dwUnexpectedBusSlaveCnt))
    {
        dwRemainingTime = oTimeout.GetRemainingTime();
        if (0 == dwRemainingTime)
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }            
        ScanBus(dwRemainingTime);
    }
    if (poMaster->GetMaxBusSlaves() < poBusTopology->GetConnectedSlaveCount())
    {
        dwRetVal = EC_E_MAX_BUS_SLAVES_EXCEEDED;
        goto Exit;
    }
    dwUnexpectedBusSlaveCnt = poBusTopology->GetUnexpectedSlaveCount();
    if (0 == dwUnexpectedBusSlaveCnt)
    {
        /* nothing to do */
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }
    /* create extended config data storage */
    if (EC_NULL == m_poEcConfigDataExtended)
    {
        m_poEcConfigDataExtended = EC_NEW(CEcConfigData(m_oAtEcatDesc.dwInstanceID, GetMemLog()));
        if (EC_NULL == m_poEcConfigDataExtended)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend() m_poEcConfigDataExtended", m_poEcConfigDataExtended, sizeof(CEcConfigData));
        m_poEcConfigDataExtended->InitializeConfigData();

        /* copy address ranges */
        OsMemcpy(m_poEcConfigDataExtended->m_abHugeRanges, m_poEcConfigData->m_abHugeRanges, sizeof(m_poEcConfigData->m_abHugeRanges));
    }

    wMaxNumSlaves = (EC_T_WORD)(m_poEcConfigData->GetNumSlaves() + m_poEcConfigDataExtended->GetNumSlaves() + dwUnexpectedBusSlaveCnt);

    /* allocate new slave storages for replacement with current(smaller) storage in CEcMaster */
    apoSlaves = EC_NEW(CEcSlave*[wMaxNumSlaves]);
    if (EC_NULL == apoSlaves)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend() apoSlaves", apoSlaves, sizeof(CEcSlave*) * wMaxNumSlaves);
    OsMemset(apoSlaves, 0, sizeof(CEcSlave*) * wMaxNumSlaves);

    apoMbSlaves = EC_NEW(CEcMbSlave*[wMaxNumSlaves]);
    if (EC_NULL == apoMbSlaves)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend() apoMbSlaves", apoMbSlaves, sizeof(CEcMbSlave*) * wMaxNumSlaves);
    OsMemset(apoMbSlaves, 0, sizeof(CEcMbSlave*) * wMaxNumSlaves);

    /* allocate temporary slaves storages to extend configuration */
    apoSlavesExtend = EC_NEW(CEcSlave*[dwUnexpectedBusSlaveCnt]);
    if (EC_NULL == apoSlavesExtend)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend() apoSlavesExtend", apoSlavesExtend, sizeof(CEcSlave*) * dwUnexpectedBusSlaveCnt);
    OsMemset(apoSlavesExtend, 0, sizeof(CEcSlave*) * dwUnexpectedBusSlaveCnt);

    /* allocate temporary BusSlaveGenEni pool and memory for unexpected bus slaves */
    dwBusSlavesMemSize = 0x400 * dwUnexpectedBusSlaveCnt;
    pbyBusSlavesMem = (EC_T_BYTE*)OsMalloc(dwBusSlavesMemSize);
    if (EC_NULL == pbyBusSlavesMem)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend() pbyBusSlavesMem ", pbyBusSlavesMem, dwBusSlavesMemSize);

    poBusSlaveGenEniPool = EC_NEW(CEcBusSlaveGenEni[dwUnexpectedBusSlaveCnt]);
    if (EC_NULL == poBusSlaveGenEniPool)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend() poBusSlaveGenEniList", poBusSlaveGenEniPool, sizeof(CEcBusSlaveGenEni*) * dwUnexpectedBusSlaveCnt);

    /* initialize BusSlaveGenEni pool */
    for (dwIdx = 0; dwIdx < dwUnexpectedBusSlaveCnt - 1; dwIdx++)
    {
        poBusSlaveGenEniPool[dwIdx].SetNext(&poBusSlaveGenEniPool[dwIdx + 1]);
        poBusSlaveGenEniPool[dwIdx + 1].SetNext(EC_NULL);
    }
    
    /* trigger a bus scan to fill BusSlaveGenEni slaves */
    {
        EC_T_DWORD dwOrderId = 0;
        EC_T_MCSM_ORDER* poMcSmOrder = EC_NULL;
        EC_T_MCSM_ORDER_CONTEXT oContext;
        OsMemset(&oContext, 0, sizeof(EC_T_MCSM_ORDER_CONTEXT));

        dwRemainingTime = oTimeout.GetRemainingTime();
        if (0 == dwRemainingTime)
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }

        oContext.ConfigExtend.bMcSmRequestBusScan = EC_TRUE;
        oContext.ConfigExtend.poBusSlavePool      = poBusSlaveGenEniPool;
        oContext.ConfigExtend.pbyBusSlavesMem     = pbyBusSlavesMem;
        oContext.ConfigExtend.dwBusSlavesMemSize  = dwBusSlavesMemSize;
        dwRes = McSmOrderAdd(eMcSmOrderType_REQ_EXTENDED_CONFIG, &poMcSmOrder, EC_FALSE, dwRemainingTime, eEcatState_UNKNOWN, eEcatMcSmState_UNKNOWN, &oContext);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
        dwOrderId = poMcSmOrder->dwOrderId;

        /* wait until the order is finished or timeout */
        for (;;)
        {
            /* sleep a while */
            OsSleep(1);
            if (poMcSmOrder->eOrderState == eEcatOrderState_Finished)
            {
                if (dwOrderId != poMcSmOrder->dwOrderId)
                {
                    /* order was overwritten, should never happen */
                    OsDbgAssert(EC_FALSE);
                    dwRetVal = EC_E_BUSY;
                    goto Exit;
                }
                /* requested state is reached */
                dwRes = poMcSmOrder->dwMcSmLastErrorStatus;
                if ((EC_E_NOERROR != dwRes) && (EC_E_BUSCONFIG_MISMATCH != dwRes))
                {
                    EC_DBGMSG("emConfigExtend()(Trigger BusScanEni): Error 0x%x in McSm state '%s' for requested state '%s'\n",
                        dwRetVal,
                        EcMcSmStateString(poMcSmOrder->EMcSmErrorState),
                        EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                    dwRetVal = dwRes;
                    goto Exit;
                }
                break;
            }
        }
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }

    /* prepare BusSlaveGenEni linked list for configuration */
    for (dwIdx = 0; dwIdx < dwUnexpectedBusSlaveCnt - 1; dwIdx++)
    {
        poBusSlaveGenEniPool[dwIdx].SetNext(&poBusSlaveGenEniPool[dwIdx + 1]);
        poBusSlaveGenEniPool[dwIdx + 1].SetNext(EC_NULL);
    }
    
    {
        EC_T_DWORD dwNumEniSlaveBefore = m_poEcConfigDataExtended->GetNumSlaves();

        /* generate extended configuration from unexpected slaves pool */
        dwRes = poMaster->ConfigGenerate(m_poEcConfigDataExtended, poBusSlaveGenEniPool, eEcatState_PREOP, EC_TRUE);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
        
        wSlaveTotalCnt   = 0;
        wMbSlaveTotalCnt = 0;
    
        /* add existing slaves to new storage  */
        for (dwIdx = 0; dwIdx < poMaster->GetCfgSlaveCount(); dwIdx++)
        {
            CEcSlave* pSlave = poMaster->GetCfgSlaveBySlaveId(dwIdx);

            apoSlaves[wSlaveTotalCnt++] = pSlave;

            if (pSlave->HasMailBox())
            {
                apoMbSlaves[wMbSlaveTotalCnt++] = (CEcMbSlave*)pSlave;
            }
        }

        /* create new slaves from extended configuration */
        for (dwIdx = 0; dwIdx < dwUnexpectedBusSlaveCnt; dwIdx++)
        {
            CEcSlave*  pSlave = EC_NULL;
    
            pSlave = poMaster->CreateSlave(m_poEcConfigDataExtended->GetEniSlave(dwNumEniSlaveBefore + dwIdx));
            if (EC_NULL == pSlave)
            {
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }
            apoSlavesExtend[dwIdx] = pSlave;

            pSlave->SetSlaveId(wSlaveTotalCnt);
            apoSlaves[wSlaveTotalCnt++] = pSlave;

            if (pSlave->HasMailBox())
            {
                apoMbSlaves[wMbSlaveTotalCnt++] = (CEcMbSlave*)pSlave;
            }
        }
    }
    /* apply new slaves and extend slave storage to the master */
    {
        EC_T_DWORD dwOrderId = 0;
        EC_T_MCSM_ORDER* poMcSmOrder = EC_NULL;
        EC_T_MCSM_ORDER_CONTEXT oContext;
        OsMemset(&oContext, 0, sizeof(EC_T_MCSM_ORDER_CONTEXT));

        dwRemainingTime = oTimeout.GetRemainingTime();
        if (0 == dwRemainingTime)
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }

        oContext.ConfigExtend.bMcSmRequestApplyConfig = EC_TRUE;
        oContext.ConfigExtend.apoSlaves     = apoSlaves;
        oContext.ConfigExtend.apoMbSlaves   = apoMbSlaves;
        oContext.ConfigExtend.wSlaveCnt     = wSlaveTotalCnt;
        oContext.ConfigExtend.wMbSlaveCnt   = wMbSlaveTotalCnt;
        oContext.ConfigExtend.wMaxNumSlaves = wMaxNumSlaves;
        dwRes = McSmOrderAdd(eMcSmOrderType_REQ_EXTENDED_CONFIG, &poMcSmOrder, EC_FALSE, dwRemainingTime, eEcatState_UNKNOWN, eEcatMcSmState_UNKNOWN, &oContext);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
        dwOrderId = poMcSmOrder->dwOrderId;

        for (;;)
        {
            /* sleep a while */
            OsSleep(1);
            /* recreate cfg topology if requested from McSm */
            if (poMcSmOrder->pContext->ConfigExtend.bApiRequestRecreateCfgTopology)
            {
                poMaster->SBDeleteCfgTopology();
                poMaster->SBCreateCfgTopology();
                /* redetermine free fixed address ranges */
                for (dwIdx = 0; dwIdx < poMaster->SBFreeRanges(); dwIdx++)
                {
                    ECX_T_ADDRRANGE* poRange = m_poEcConfigDataExtended->GetAddressRange(dwIdx);

                    if (EC_NULL != poRange)
                    {
                        poMaster->SBSetFreeRange(dwIdx, poRange->wBegin, poRange->wEnd);
                    }
                    else
                    {
                        poMaster->SBSetFreeRange(dwIdx, 0, 0);
                    }
                }
                
                /* confirm request proceeded to McSm */
                poMcSmOrder->pContext->ConfigExtend.bApiRequestRecreateCfgTopology = EC_FALSE;
            }
            /* wait until the order is finished or timeout */
            if (poMcSmOrder->eOrderState == eEcatOrderState_Finished)
            {
                if (dwOrderId != poMcSmOrder->dwOrderId)
                {
                    /* order was overwritten, should never happen */
                    OsDbgAssert(EC_FALSE);
                    dwRetVal = EC_E_BUSY;
                    goto Exit;
                }
                /* requested state is reached */
                dwRes = poMcSmOrder->dwMcSmLastErrorStatus;
                if (EC_E_NOERROR != dwRes)
                {
                    EC_DBGMSG("emConfigExtend()(Apply unexpected slaves): Error 0x%x in McSm state '%s' for requested state '%s'\n",
                        dwRetVal,
                        EcMcSmStateString(poMcSmOrder->EMcSmErrorState),
                        EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                    dwRetVal = dwRes;
                    goto Exit;
                }
                break;
            }
        }
        /* swap storage pointers to free previous slave storages */
        apoSlaves = poMcSmOrder->pContext->ConfigExtend.apoSlaves;
        apoMbSlaves = poMcSmOrder->pContext->ConfigExtend.apoMbSlaves;
        wMaxNumSlaves = poMcSmOrder->pContext->ConfigExtend.wMaxNumSlaves;
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);

        /* remove currently added slaves from temporary storage to prevent erroneous deletion in case of error */
        OsMemset(apoSlavesExtend, 0, sizeof(CEcSlave*) * dwUnexpectedBusSlaveCnt);
    }   
    /* Trigger bus scan to link the new config slaves with corresponding bus slaves */
    {
        dwRemainingTime = oTimeout.GetRemainingTime();
        if (0 == dwRemainingTime)
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }
        dwRes = ScanBus(dwRemainingTime);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    
    dwRetVal = EC_E_NOERROR;
Exit:
    /* clean up */
    if (EC_E_NOERROR != dwRetVal)
    {
        /* delete extend config slaves */
        for (dwIdx = 0; (EC_NULL != apoSlavesExtend) && (dwIdx < dwUnexpectedBusSlaveCnt); dwIdx++)
        {
            if (EC_NULL != apoSlavesExtend[dwIdx])
            {
                /* unlink bus slave */
                if (EC_NULL != apoSlavesExtend[dwIdx]->m_pBusSlave)
                {
                    apoSlavesExtend[dwIdx]->m_pBusSlave->SetCfgSlave(EC_NULL);
                }
                EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CAtEmInterface::ConfigExtend ~pSlave", apoSlavesExtend[dwIdx], apoSlavesExtend[dwIdx]->GetThisSize());
                SafeDelete(apoSlavesExtend[dwIdx]);
            }
        }
    
        /* delete empty extend config data */
        if ((EC_NULL != m_poEcConfigDataExtended) && (0 == m_poEcConfigDataExtended->GetNumSlaves()))
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CAtEmInterface::ConfigExtend ~m_poEcConfigDataExtended", m_poEcConfigDataExtended, sizeof(CEcConfigData));
            SafeDelete(m_poEcConfigDataExtended);
        }
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend ~apoSlavesExtend", apoSlavesExtend, sizeof(CEcSlave*) * dwUnexpectedBusSlaveCnt);
    SafeDeleteArray(apoSlavesExtend);
    
    EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend() ~pbyBusSlavesMem", pbyBusSlavesMem, dwBusSlavesMemSize);
    SafeDeleteArray(pbyBusSlavesMem);
    
    EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend() ~poBusSlaveGenEniList", poBusSlaveGenEniPool, sizeof(CEcBusSlaveGenEni*) * dwUnexpectedBusSlaveCnt);
    SafeDeleteArray(poBusSlaveGenEniPool);
    
    /* delete leftover slave storage from previous configuration */
    EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend ~apoSlaves", apoSlaves, sizeof(CEcSlave*) * wMaxNumSlaves);
    SafeDeleteArray(apoSlaves);
    
    EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "CAtEmInterface::ConfigExtend ~apoMbSlaves", apoMbSlaves, sizeof(CEcMbSlave*) * wMaxNumSlaves);
    SafeDeleteArray(apoMbSlaves);

    /* reenable notifications */
    if (bNotificationsMasked)
    {
        poMaster->m_pBusTopology->SetPortOperationsEnabled(EC_TRUE);
        poMaster->DecNotifySBStatusMaskedCnt();
        poMaster->DecNotifySBMismatchMaskedCnt();
    }
    return dwRetVal;
}
#endif

EC_T_DWORD CAtEmInterface::SetMasterInitParmBusCycleTimeUsec(EC_T_DWORD dwBusCycleTimeUsec)
{
    if (0 == dwBusCycleTimeUsec)
    {
        return EC_E_INVALIDPARM;
    }

    m_oAtEcatDesc.pInitMasterParms->dwBusCycleTimeUsec = dwBusCycleTimeUsec;
    CalculateDefaultMcSmOrderTimeout(dwBusCycleTimeUsec);

    m_oAtEcatDesc.poMaster->m_MasterConfig.dwBusCycleTimeUsec = dwBusCycleTimeUsec; 
    m_oAtEcatDesc.poMaster->CalculateEcatCmdTimeout(dwBusCycleTimeUsec);

    return EC_E_NOERROR;
}

EC_T_VOID CAtEmInterface::CalculateDefaultMcSmOrderTimeout(EC_T_DWORD dwBusCycleTimeUsec)
{
    m_oAtEcatDesc.dwOrderTimeout = (ECAT_MASTER_ORDER_TIMEOUT_DEFAULT * dwBusCycleTimeUsec) / 1000;
}

/***************************************************************************************************/
/**
\brief  Configure the Master.

This function has to be called after InitMaster and prior to calling Start. Among others
the EtherCAT topology defined in the given XML configuration file will be stored internally. The
Ethernet link layer and the base timer clock will be initialized, too.
\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::ConfigureMaster(
    EC_T_CNF_TYPE eCnfType,     /**< [in] Enum type of configuration data provided */
    EC_T_PBYTE    pbyCnfData,   /**< [in] Configuration data */
    EC_T_DWORD    dwCnfDataLen  /**< [in] Length of configuration data in byte */
)
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;
    EC_T_DWORD  dwRes    = 0;

    /* ENI generated automatically based on bus-scan result ? */
    if ((eCnfType_GenPreopENI == eCnfType) || (eCnfType_GenPreopENIWithCRC == eCnfType) || (eCnfType_GenOpENI == eCnfType))
    {
#if (defined INCLUDE_GEN_OP_ENI)
    EC_T_DWORD dwCfgSlavesCnt = 0;
    EC_T_DWORD dwMaxBusSlaves = 0;
    EC_T_MCSM_ORDER* poMcSmOrder = EC_NULL;
    EC_T_STATE eMaxEcatState = (eCnfType_GenOpENI == eCnfType) ? eEcatState_OP : eEcatState_PREOP;

        /* cancel all orders */
        m_oAtEcatDesc.McSmCancelAllOrders();

        /* disable jobs */
        DisableJobs(250);

        /* wait until McSm stable and order queue empty */
        dwRes = WaitUntilStableMcSm(ECAT_MCSM_STABLE_TIMEOUT_DEFAULT);
        if (EC_E_NOERROR != dwRes)
        {
            EC_ERRORMSGC(("ConfigureMaster(): Master control state machine doesn't become stable\n"));
        }
        /* read config slaves count to deinitialize master correctly */
        dwCfgSlavesCnt = m_oAtEcatDesc.poMaster->GetCfgSlaveCount();

        /* realloc bus slaves pool */
        m_oAtEcatDesc.poMaster->SetConfigured(EC_FALSE);
        m_oAtEcatDesc.poMaster->SetCfgSlaveCount(0);
        dwMaxBusSlaves = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlavesPoolSize();
        m_oAtEcatDesc.poMaster->m_pBusTopology->RecreateBusSlavesPool(dwMaxBusSlaves, EC_TRUE);
        m_oAtEcatDesc.poMaster->SBSetFreeRange(0, 0x0001, 0xFFFF);

        /* enable jobs now */
        EnableJobs();

        /* scan bus */
        dwRetVal = McSmOrderAdd(eMcSmOrderType_REQ_SB_FOR_GENERATE_ENI, &poMcSmOrder, EC_FALSE, EC_WAITINFINITE);
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }
        /* wait until the order is finished */
        for(;;)
        {
            /* sleep a while */
            OsSleep(1);
            if (poMcSmOrder->eOrderState == eEcatOrderState_Finished)
            {
                break;
            }
        }
        /* restore config slaves count to deinitialize master correctly */
        m_oAtEcatDesc.poMaster->SetCfgSlaveCount(dwCfgSlavesCnt);

#if (defined INCLUDE_RED_DEVICE)
        /* check if line break detected */
        if ((m_oAtEcatDesc.poMaster->m_bRedLineBreak) && (eCnfType_GenOpENI == eCnfType))
        {
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }
#endif
        /* set master to INIT. After ->ConfigGenerate master config is broken */
        if (DEVICE_STATE_UNKNOWN != m_oAtEcatDesc.poMaster->GetMasterHighestDeviceState())
        {
            dwRes = SetMasterStateEx(m_oAtEcatDesc.poMaster->GetStateChangeDefaultTimeout(), eEcatState_INIT);
            if (EC_E_NOERROR != dwRes)
            {
                EC_DBGMSG("ConfigureMaster(): GenxxxENI could not set master into INIT state Error=0x%08X\n", dwRes);
            }
        }

        /* generate config data */
        dwRes = m_oAtEcatDesc.poMaster->ConfigGenerate(m_poEcConfigData, m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveList(), eMaxEcatState);

        /* release scan bus order */
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);

        /* load new configuration from config data */
        if (EC_E_NOERROR == dwRes)
        {
            if (eCnfType_GenPreopENIWithCRC == eCnfType)
            {
                m_poEcConfigData->SetConfigCheckSum(0xFFFFFFFF);
            }

            dwRes = ConfigLoad(eCnfType, (EC_T_PBYTE)m_poEcConfigData, 0);
        }
#else
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
#endif
    }
    else
    {
        dwRes = ConfigLoad(eCnfType, pbyCnfData, dwCnfDataLen);
    }
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    dwRes = ConfigApply();
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/** \brief Start the EtherCAT master stack

  This function place the EtherCAT master stack in operational state. There are two possibilities to
  handle the stack start:
  1) The function Start() is blocking
  The parameter dwTimeout must be not equal to EC_NOWAIT. Is this case the function
  returns when the operational state is reached, or if the timeout raised.
  2) The function Start() is not blocking
  The parameter dwTimeout must be equal to EC_NOWAIT. Is this case the function returns
  immediately. The calling application must check the operational state of the stack
  waiting for the notify (EC_NOTIFY_STATECHANGED) call.

    \return Depends on the implementation of the function OsSetLastError(),
    normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::Start(
    EC_T_DWORD  dwTimeout   /**< [in] Timeout in milliseconds */
)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    dwRetVal = SetMasterStateEx(dwTimeout, eEcatState_OP);
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Set the EtherCAT master stack into the requested state.

    \return Depends on the implementation of the function OsSetLastError(),
    normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::SetMasterState(
    EC_T_DWORD  dwTimeout,  /**< [in] Timeout in milliseconds */
    EC_T_STATE  eReqState   /**< [in] Requested System state  */
)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("SetMasterState: Master Stack not Initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP1_ECATAPI, ETHTYP0_ECATAPI_SET_MASTER_STATE,
        "SetMasterState(%d, %s) ...\n", dwTimeout, ecatStateToStr(eReqState)));

    dwRetVal = SetMasterStateEx(dwTimeout, eReqState);

    EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP1_ECATAPI, ETHTYP0_ECATAPI_SET_MASTER_STATE,
        "SetMasterState(%d, %s) = %s\n", dwTimeout, ecatStateToStr(eReqState), EcErrorText(dwRetVal)));

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Set the EtherCAT master stack into the requested state.

    \return Depends on the implementation of the function OsSetLastError(),
    normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::SetMasterStateEx(
    EC_T_DWORD  dwTimeout,  /**< [in] Timeout in milliseconds */
    EC_T_STATE  eReqState   /**< [in] Requested System state  */
)
{
    EC_T_DWORD          dwRetVal = EC_E_ERROR;
    EC_T_MCSM_STATE     EMcSmReqStateNew = EcStateToEcMcSmState(eReqState);
    EC_T_MCSM_ORDER*    poMcSmOrder = EC_NULL;
    EC_T_DWORD          dwOrderId;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check if ADS adapter is running */
    if (AdsIsRunning())
    {
        dwRetVal = EC_E_ADS_IS_RUNNING;
        goto Exit;
    }
    /* check parameters */
    if ((eEcatState_UNKNOWN == eReqState) || (eEcatState_BOOTSTRAP == eReqState))
    {
        EC_ERRORMSGC(("ecatSetMasterState(): invalid requested master state %s\n", ecatStateToStr(eReqState)));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if ((0 == m_oAtEcatDesc.poMaster->GetCfgSlaveCount()) && m_oAtEcatDesc.poMaster->GetAllSlavesMustReachState())
    {
        EC_DBGMSG("ecatSetMasterState(): No slaves configured!\n");
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if ((eCnfType_GenPreopENI == m_oCfgParm.eCnfType) || (eCnfType_GenPreopENIWithCRC == m_oCfgParm.eCnfType))
    {
        /* With generated ENI only INIT and PREOP are valid target states. */
        if (!((eEcatState_INIT == eReqState) || (eEcatState_PREOP == eReqState)))
        {
            EC_DBGMSG("ecatSetMasterState(): ERROR: Configuration doesn't support SAFEOP and OP requested state\n");
            dwRetVal = EC_E_ENI_NO_SAFEOP_OP_SUPPORT;
            goto Exit;
        }
    }

    /* request master state change */
    dwRetVal = McSmOrderAdd(eMcSmOrderType_REQ_MASTER_STATE, &poMcSmOrder, (EC_NOWAIT == dwTimeout), ((EC_NOWAIT == dwTimeout) ? m_oAtEcatDesc.poMaster->GetStateChangeDefaultTimeout() : dwTimeout), eReqState, EMcSmReqStateNew);
    if ((EC_E_NOERROR != dwRetVal) || (EC_NOWAIT == dwTimeout))
    {
        goto Exit;
    }
    dwOrderId = poMcSmOrder->dwOrderId;

    /* wait until the order is finished or timeout */
    for(;;)
    {
        /* sleep a while */
        OsSleep(1);
        if (eEcatOrderState_Finished == poMcSmOrder->eOrderState)
        {
            if (dwOrderId != poMcSmOrder->dwOrderId)
            {
                /* order was overwritten, should never happen */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }
            /* requested state is reached */
            dwRetVal = poMcSmOrder->dwMcSmLastErrorStatus;
            if (EC_E_NOERROR != dwRetVal)
            {
                EC_DBGMSG("ecatSetMasterState() Error 0x%x in McSm state '%s' for requested state '%s'\n",
                    dwRetVal, EcMcSmStateString(poMcSmOrder->EMcSmErrorState), EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                goto Exit;
            }
            dwRetVal = EC_E_NOERROR;
            break;
        }
        if (!m_oAtEcatDesc.bJobsActive)
        {
            UsrCtlClkHelper();
        }

    } /* wait until the order was finished */

    if (EC_E_NOERROR != dwRetVal)
    {
        EC_ERRORMSGC(("ecatSetMasterState() Error 0x%x waiting for state %s!\n", dwRetVal, SlaveDevStateText(((EC_T_WORD)eReqState))));

        goto Exit;
    }

Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);

    /* release order if needed */
    if ((EC_NULL != poMcSmOrder) && (EC_NOWAIT != dwTimeout))
    {
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    return dwRetVal;
}


/***************************************************************************************************/
/** \brief Stop the EtherCAT master stack

This function place the EtherCAT master stack in init state.

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::Stop(
    EC_T_DWORD  dwTimeout   /**< [in] Timeout in milliseconds */
)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;

    dwRetVal = SetMasterStateEx(dwTimeout, eEcatState_INIT);
    return OsSetLastError(dwRetVal);
}


/***************************************************************************************************/
/** \brief EtherCAT wildcard function

  This function supports a number of control functions supported by the EtherCAT master stack. The
  data transfer is made through a structure EC_T_IOCTLPARMS with following members:
  pbyInBuf            Input data buffer
  dwInBufSize         Size of input data buffer in byte
  pbyOutBuf           Output data buffer
  dwOutBufSize        Size of output data buffer in byte
  pdwNumOutData       Number of output data bytes stored in output data buffer

    The following control functions are currently defined:
    EC_IOCTL_REGISTERCLIENT     Register a client for process data and notifications

    EC_IOCTL_UNREGISTERCLIENT   Un-register client for process data and notifications

    \return Depends on the implementation of the function OsSetLastError(),
    normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::IoControl(
    EC_T_DWORD       dwCode,    /**< [in]     Control code */
    EC_T_IOCTLPARMS* pParms     /**< [in/out] Data transferred */
)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_oAtEcatDesc.dwStateChangeDebug >> 4) & 3);
#endif
EC_T_DWORD   dwRes           = EC_E_ERROR;
EC_T_DWORD   dwRetVal        = EC_E_ERROR;
EC_T_BOOL    bClntDescLocked = EC_FALSE;
CEcBusSlave* pBusSlave       = EC_NULL;
CEcSlave*    pCfgSlave       = EC_NULL;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check if ADS adapter is running */
    if (AdsIsRunning() && m_dwInternalIOControlCode != dwCode)
    {
        dwRetVal = EC_E_ADS_IS_RUNNING;
        goto Exit;
    }

    /* reset return values */
    if ((pParms != EC_NULL) && (pParms->pdwNumOutData != EC_NULL))
    {
        EC_SETDWORD(pParms->pdwNumOutData, 0);
    }

    /* handle iocontrol */
    switch (dwCode)
    {
    /* #######################################################################################################
       ##### Generic (EC_IOCTL_GENERIC 0x00000000)                                                       #####
       #######################################################################################################*/

    case EC_IOCTL_REGISTERCLIENT:                       /* (EC_IOCTL_GENERIC |  2) */
        {
            /* check parameters */
            if ((EC_NULL                     == pParms)
             || (EC_NULL                     == pParms->pbyInBuf)
             || (EC_NULL                     == pParms->pbyOutBuf)
             || (sizeof(EC_T_REGISTERPARMS)   > pParms->dwInBufSize)
             || (sizeof(EC_T_REGISTERRESULTS) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            /* register client */
            dwRes = RegisterClient((EC_T_REGISTERPARMS*)pParms->pbyInBuf, (EC_T_REGISTERRESULTS*)pParms->pbyOutBuf);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            if (pParms->pdwNumOutData != EC_NULL )
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_REGISTERRESULTS));
            }
            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_UNREGISTERCLIENT:                     /* (EC_IOCTL_GENERIC |  3) */
        {
            /* check parameters */
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            /* unregister client */
            dwRetVal = UnregisterClient(EC_GETDWORD(pParms->pbyInBuf));
        } break;

    case EC_IOCTL_ISLINK_CONNECTED:                     /* (EC_IOCTL_GENERIC |  6) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyOutBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            EC_SETDWORD(pParms->pbyOutBuf, (m_oAtEcatDesc.poEcDevice->IsLinkConnected() ? 1 : 0));
            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_DWORD));
            }

            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_SET_CYC_ERROR_NOTIFY_MASK:            /* (EC_IOCTL_GENERIC |  8) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->SetCycErrorNotifyMask(EC_GETDWORD(pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_LINKLAYER_DBG_MSG:                    /* (EC_IOCTL_GENERIC | 10) */
        {
            EC_T_LINKLAYER_DBG_MSG_DESC* poDbgMsgDesc;

            if ((EC_NULL                            == pParms)
             || (EC_NULL                            == pParms->pbyInBuf)
             || (sizeof(EC_T_LINKLAYER_DBG_MSG_DESC) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            poDbgMsgDesc = (EC_T_LINKLAYER_DBG_MSG_DESC*)pParms->pbyInBuf;
            EC_DBGMSGL((2, poDbgMsgDesc->byEthTypeByte0, poDbgMsgDesc->byEthTypeByte1, poDbgMsgDesc->szMsg));
            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_RESET_SLAVE:                          /* (EC_IOCTL_GENERIC | 13) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            dwRetVal = m_oAtEcatDesc.poMaster->ResetSlaveById(EC_GETDWORD(pParms->pbyInBuf));
        } break;
    case EC_IOCTL_SLAVE_LINKMESSAGES:                   /* (EC_IOCTL_GENERIC | 14) */
        {
            EC_T_SLAVE_LINKMSG_DESC oReq;

            /* check parameters */
            if ((EC_NULL                        == pParms)
             || (EC_NULL                        == pParms->pbyInBuf)
             || (sizeof(EC_T_SLAVE_LINKMSG_DESC) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            /* check pre-requirements */
            if (!m_oAtEcatDesc.poMaster->IsConfigured())
            {
                dwRetVal = EC_E_INVALIDSTATE;
                goto Exit;
            }
            OsMemcpy(&oReq, pParms->pbyInBuf, sizeof(EC_T_SLAVE_LINKMSG_DESC));

            if (((EC_T_DWORD)-1) == oReq.dwSlaveId )
            {
                /* access all slaves */
                for(EC_T_DWORD dwScan = 0; dwScan < m_oAtEcatDesc.poMaster->GetCfgSlaveCount(); dwScan++)
                {
                    pCfgSlave = m_oAtEcatDesc.poMaster->GetSlaveByIndex(dwScan);
                    if (EC_NULL == pCfgSlave)
                    {
                        continue;
                    }
                    pCfgSlave->LinkMessages(oReq.bEnableLogging);
                }
            }
            else
            {
                /* access single slave */
                pCfgSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(oReq.dwSlaveId);
                if (EC_NULL == pCfgSlave)
                {
                    dwRetVal = EC_E_NOTFOUND;
                    goto Exit;
                }

                pCfgSlave->LinkMessages(oReq.bEnableLogging);
            }

            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_GET_CYCLIC_CONFIG_INFO:               /* (EC_IOCTL_GENERIC | 15) */
        {
            EC_T_CYC_CONFIG_DESC*   pCycConfigDesc;
            EC_T_DWORD              dwCycEntryIndex;

            if ((EC_NULL                     == pParms)
             || (EC_NULL                     == pParms->pbyInBuf)
             || (EC_NULL                     == pParms->pbyOutBuf)
             || (sizeof(EC_T_DWORD)           > pParms->dwInBufSize)
             || (sizeof(EC_T_CYC_CONFIG_DESC) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            dwCycEntryIndex = *(EC_T_DWORD*)pParms->pbyInBuf;
            pCycConfigDesc = (EC_T_CYC_CONFIG_DESC*)pParms->pbyOutBuf;
            dwRetVal = m_oAtEcatDesc.poMaster->GetMasterCycConfig(dwCycEntryIndex, pCycConfigDesc);
            if (EC_E_NOERROR != dwRetVal)
            {
                goto Exit;
            }

            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD( pParms->pdwNumOutData, sizeof(EC_T_CYC_CONFIG_DESC ));
            }
        } break;
    case EC_IOCTL_GET_LINKLAYER_MODE:                   /* (EC_IOCTL_GENERIC | 16) */
        {
            EC_T_LINKLAYER_MODE_DESC    oLinkLayerMode;

            if ((EC_NULL                         == pParms)
             || (EC_NULL                         == pParms->pbyOutBuf)
             || (sizeof(EC_T_LINKLAYER_MODE_DESC) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            if (EC_NULL != m_oAtEcatDesc.poEcDevice)
            {
                oLinkLayerMode.eLinkMode    = m_oAtEcatDesc.poEcDevice->LinkGetMode();
            }
            else
            {
                oLinkLayerMode.eLinkMode    = EcLinkMode_UNDEFINED;
            }
#if (defined INCLUDE_RED_DEVICE)
            if (EC_NULL != m_oAtEcatDesc.poEcRedDevice)
            {
                oLinkLayerMode.eLinkModeRed = m_oAtEcatDesc.poEcRedDevice->LinkGetMode();
            }
            else
            {
                oLinkLayerMode.eLinkModeRed = EcLinkMode_UNDEFINED;
            }
#endif
            OsMemcpy(pParms->pbyOutBuf, &oLinkLayerMode, sizeof(EC_T_LINKLAYER_MODE_DESC));

            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_LINKLAYER_MODE_DESC));
            }
            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_IS_SLAVETOSLAVE_COMM_CONFIGURED:  /* (EC_IOCTL_GENERIC |  17) */
        {
            if ((EC_NULL             == pParms)
             || (EC_NULL             == pParms->pbyOutBuf)
             || (sizeof(EC_T_DWORD ) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            EC_SETDWORD(pParms->pbyOutBuf, ((m_oAtEcatDesc.poMaster->IsSlaveToSlaveCommConfigured())?1:0));

            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD( pParms->pdwNumOutData, sizeof(EC_T_DWORD ));
            }

            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_INITIATE_UPDATE_ALL_SLAVE_STATE:      /* (EC_IOCTL_GENERIC | 19) */
        {
            dwRetVal = McSmOrderAdd(eMcSmOrderType_REQ_ALSTATUS_REFRESH, EC_NULL, EC_TRUE, EC_WAITINFINITE);
        } break;

#if (defined INCLUDE_DC_SUPPORT )
    case EC_IOCTL_ADD_BRD_SYNC_WINDOW_MONITORING:            /* add BRD to 0x92C to cyclic frame */
        {
            m_oAtEcatDesc.poMaster->SetAddBrdSyncWindowMonitoring(EC_TRUE);

            dwRetVal = EC_E_NOERROR;
        } break;
#endif /* INCLUDE_DC_SUPPORT */

    case EC_IOCTL_ONLY_PROCESS_DATA_IN_IMAGE:            /* Only process data in input image */
        {
            m_oMasterConfigEx.bOnlyProcDataInImage = EC_TRUE;

            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_REGISTER_CYCFRAME_RX_CB:              /* Register Callback for "Cyclic frame received" */
        {
            EC_T_CYCFRAME_RX_CBDESC* pCycRxDesc;

            if ((EC_NULL                        == pParms)
             || (EC_NULL                        == pParms->pbyInBuf)
             || (sizeof(EC_T_CYCFRAME_RX_CBDESC) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            pCycRxDesc = (EC_T_CYCFRAME_RX_CBDESC*)pParms->pbyInBuf;

            /* callback function to inform application that all cyclic frame(s) is(are) received */
            if (pCycRxDesc != EC_NULL)
            {
                OsMemcpy(&m_oAtEcatDesc.oCycRxDesc, pCycRxDesc, sizeof(EC_T_CYCFRAME_RX_CBDESC));
                m_oAtEcatDesc.poMaster->SetCycFrameReceivedCallback(pCycRxDesc);
            }

            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_SET_PD_OFFSET_COMPAT_MODE:            /* process data offset are in compatibility mode (before V2.1) */
        {
            m_oAtEcatDesc.poMaster->SetProcessDataCompatMode();

            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_IS_MAIN_LINK_CONNECTED:                     /* (EC_IOCTL_GENERIC |  24) */
        {
            if ((EC_NULL             == pParms)
             || (EC_NULL             == pParms->pbyOutBuf)
             || (sizeof(EC_T_DWORD ) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            EC_SETDWORD(pParms->pbyOutBuf, ((m_oAtEcatDesc.poEcDevice->IsMainLinkConnected())?1:0));
            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD( pParms->pdwNumOutData, sizeof(EC_T_DWORD ));
            }

            dwRetVal = EC_E_NOERROR;
        } break;
#if(defined INCLUDE_RED_DEVICE)
    case EC_IOCTL_IS_RED_LINK_CONNECTED:                     /* (EC_IOCTL_GENERIC |  25) */
        {
            if(!m_oAtEcatDesc.poEcDevice->m_bRedConfigured)
            {
              dwRetVal = EC_E_NOTSUPPORTED;
              goto Exit;
            }
            if ((EC_NULL             == pParms)
             || (EC_NULL             == pParms->pbyOutBuf)
             || (sizeof(EC_T_DWORD ) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            EC_SETDWORD(pParms->pbyOutBuf, ((m_oAtEcatDesc.poEcDevice->IsRedLinkConnected())?1:0));
            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD( pParms->pdwNumOutData, sizeof(EC_T_DWORD ));
            }

            dwRetVal = EC_E_NOERROR;
        } break;
#endif

    case EC_IOCTL_ADD_COE_INITCMD:                     /* (EC_IOCTL_GENERIC |  26) */
        {
            if ((EC_NULL             == pParms)
             || (EC_NULL             == pParms->pbyInBuf)
             || (sizeof(EC_T_ADD_COE_INITCMD_DESC) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            pCfgSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(((EC_T_ADD_COE_INITCMD_DESC*)pParms->pbyInBuf)->dwSlaveId);

            if (EC_NULL == pCfgSlave)
            {
                dwRetVal = EC_E_NOTFOUND;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->m_pvSlaveCoeInitCmdCallbackParm = ((EC_T_ADD_COE_INITCMD_DESC*)pParms->pbyInBuf)->pvCallbackParm;
            m_oAtEcatDesc.poMaster->m_pfnSlaveCoeInitCmdCallback = ((EC_T_ADD_COE_INITCMD_DESC*)pParms->pbyInBuf)->pfnCallback;
            dwRetVal = pCfgSlave->AddCoeInitCmds(((EC_T_ADD_COE_INITCMD_DESC*)pParms->pbyInBuf)->pbCoeInitCmds, ((EC_T_ADD_COE_INITCMD_DESC*)pParms->pbyInBuf)->wCount);
        } break;

    /* (EC_IOCTL_GENERIC | 27 - 39 ) unused */

    case EC_IOCTL_GET_PDMEMORYSIZE:                     /* (EC_IOCTL_GENERIC | 40) */
        {
            EC_T_MEMREQ_DESC   MemReq;

            if ((EC_NULL                 == pParms)
             || (EC_NULL                 == pParms->pbyOutBuf)
             || (sizeof(EC_T_MEMREQ_DESC) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            MemReq.dwPDInSize     = m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_IN);
            MemReq.dwPDOutSize    = m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_OUT);

            OsMemcpy(pParms->pbyOutBuf, &MemReq, sizeof(EC_T_MEMREQ_DESC));

            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_MEMREQ_DESC));
            }
            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_REGISTER_PDMEMORYPROVIDER:            /* (EC_IOCTL_GENERIC | 41) */
        {
            EC_T_MEMPROV_DESC  MemProv;

            if ((EC_NULL                  == pParms)
             || (EC_NULL                  == pParms->pbyInBuf)
             || (sizeof(EC_T_MEMPROV_DESC) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            OsMemcpy(&MemProv, pParms->pbyInBuf, sizeof(EC_T_MEMPROV_DESC));

            dwRetVal = m_oAtEcatDesc.poMaster->SetPDMemProvider(&MemProv);
        } break;
    case EC_IOCTL_FORCE_BROADCAST_DESTINATION:          /* (EC_IOCTL_GENERIC | 42) */
        {
            dwRetVal = EC_E_NOERROR;
        } break;
#if (defined INCLUDE_SLAVE_STATISTICS)
    case EC_IOCTL_GET_SLVSTAT_PERIOD:                   /* (EC_IOCTL_GENERIC | 66) */
        {
            if ((EC_NULL == pParms)
                || (EC_NULL == pParms->pbyOutBuf)
                || (sizeof(EC_T_DWORD) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            EC_SETDWORD(pParms->pbyOutBuf, m_oAtEcatDesc.poMaster->GetSlaveStatisticsPeriod());
            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_DWORD));
            }
            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_SET_SLVSTAT_PERIOD:                   /* (EC_IOCTL_GENERIC | 43) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            dwRetVal = m_oAtEcatDesc.poMaster->SetSlaveStatisticsPeriod(EC_GETDWORD(pParms->pbyInBuf));
        } break;
    case EC_IOCTL_FORCE_SLVSTAT_COLLECTION:             /* (EC_IOCTL_GENERIC | 44) */
        {
            m_oAtEcatDesc.poMaster->RequestSlaveStatistics();
            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_GET_SLVSTATISTICS:                    /* (EC_IOCTL_GENERIC | 45) */
        {
            /* check parameters */
            if ((EC_NULL                        == pParms)
             || (EC_NULL                        == pParms->pbyInBuf)
             || (EC_NULL                        == pParms->pbyOutBuf)
             || (sizeof(EC_T_DWORD)              > pParms->dwInBufSize)
             || (sizeof(EC_T_SLVSTATISTICS_DESC) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            /* check pre-requirements */
            if (!m_oAtEcatDesc.poMaster->IsConfigured())
            {
                dwRetVal = EC_E_INVALIDSTATE;
                goto Exit;
            }
            /* access single slave */
            pCfgSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(EC_GETDWORD(pParms->pbyInBuf));
            if (EC_NULL == pCfgSlave)
            {
                dwRetVal = EC_E_NOTFOUND;
                goto Exit;
            }
            dwRetVal = pCfgSlave->GetStatisticCounters((EC_T_SLVSTATISTICS_DESC*)pParms->pbyOutBuf);
            if (EC_E_NOERROR != dwRetVal)
            {
                goto Exit;
            }
            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_SLVSTATISTICS_DESC));
            }
        } break;
    case EC_IOCTL_CLR_SLVSTATISTICS:             /* (EC_IOCTL_GENERIC | 46) */
        {
            m_oAtEcatDesc.poMaster->ClearSlaveStatistics();

            dwRetVal = EC_E_NOERROR;
        } break;
#endif
    case EC_IOCTL_SET_MBX_RETRYACCESS_COUNT:     /* (EC_IOCTL_GENERIC | 47) */
        {
            /* check parameters */
            if ((EC_NULL              == pParms)
             || (EC_NULL              == pParms->pbyInBuf)
             || (3 * sizeof(EC_T_WORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            /* check pre-requirements */
            if (!m_oAtEcatDesc.poMaster->IsConfigured())
            {
                dwRetVal = EC_E_INVALIDSTATE;
                goto Exit;
            }
            pCfgSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(EC_GETDWORD(&pParms->pbyInBuf[0]));
            if (EC_NULL == pCfgSlave)
            {
                dwRetVal = EC_E_NOTFOUND;
                goto Exit;
            }
            ((CEcMbSlave*)pCfgSlave)->SetRetryAccessCount(EC_GETWORD(&pParms->pbyInBuf[4]));

            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_SET_MBX_RETRYACCESS_PERIOD:    /* (EC_IOCTL_GENERIC | 48) */
        {
            /* check parameters */
            if ((EC_NULL              == pParms)
             || (EC_NULL              == pParms->pbyInBuf)
             || (3 * sizeof(EC_T_WORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            /* check pre-requirements */
            if (!m_oAtEcatDesc.poMaster->IsConfigured())
            {
                dwRetVal = EC_E_INVALIDSTATE;
                goto Exit;
            }
            pCfgSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(EC_GETDWORD(&pParms->pbyInBuf[0]));
            if (EC_NULL == pCfgSlave)
            {
                dwRetVal = EC_E_NOTFOUND;
                goto Exit;
            }
            ((CEcMbSlave*)pCfgSlave)->SetRetryAccessPeriod(EC_GETWORD(&pParms->pbyInBuf[4]));

            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_ALL_SLAVES_MUST_REACH_MASTER_STATE:   /* (EC_IOCTL_GENERIC | 49) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->SetAllSlavesMustReachState((EC_T_BOOL)(0 != EC_GETDWORD(pParms->pbyInBuf)));

            dwRetVal = EC_E_NOERROR;
        } break;
#if (defined INCLUDE_MASTER_OBD)
    case EC_IOCTL_MASTEROD_SET_VALUE:                   /* (EC_IOCTL_GENERIC | 51) */
        {
            if ((EC_NULL                                    == pParms)
             || (EC_NULL                                    == pParms->pbyInBuf)
             || (sizeof(EC_T_MASTEROD_OBJECT_PARMS) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            EC_T_MASTEROD_OBJECT_PARMS* pCtlParms = (EC_T_MASTEROD_OBJECT_PARMS*)pParms->pbyInBuf;

            dwRetVal = m_oAtEcatDesc.poMaster->m_pSDOService->Write(pCtlParms->wIndex, pCtlParms->bySubindex, pCtlParms->dwLength,
                (EC_T_OBJECTDICT*)EC_NULL, (EC_T_WORD*)pCtlParms->pbyData, EC_FALSE, EC_TRUE);
        } break;
#endif

    case EC_IOCTL_SET_CYCFRAME_LAYOUT:                  /* (EC_IOCTL_GENERIC | 52) */
        {
        EC_T_CYCFRAME_LAYOUT* poCycFrameLayout = EC_NULL;

            if ((EC_NULL                  == pParms)
             || (EC_NULL                  == pParms->pbyInBuf)
             || (sizeof(EC_T_CYCFRAME_LAYOUT) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            poCycFrameLayout = (EC_T_CYCFRAME_LAYOUT*)pParms->pbyInBuf;
            dwRetVal = m_oAtEcatDesc.poMaster->SetCycFrameLayout(*poCycFrameLayout);
        } break;

    case EC_IOCTL_SET_NOTIFICATION_ENABLED:             /* (EC_IOCTL_GENERIC | 53) */
        {
            EC_T_SET_NOTIFICATION_ENABLED_PARMS* pCtlParms = EC_NULL;
            EC_T_CLNTDESC* pClntDesc = EC_NULL;
            EC_T_DWORD dwPartialNotificationMask = 0;
            EC_T_DWORD dwOldNotificationMask = 0;
            EC_T_DWORD dwNewNotificationMask = 0;

            /* check parameters */
            if ((EC_NULL == pParms)
             || (EC_NULL == pParms->pbyInBuf)
             || (sizeof(EC_T_SET_NOTIFICATION_ENABLED_PARMS) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            pCtlParms = (EC_T_SET_NOTIFICATION_ENABLED_PARMS*)pParms->pbyInBuf;

            /* check if EC_NOTIFY_... code supported */
            dwPartialNotificationMask = NotificationCodeToNotificationMask(pCtlParms->dwCode);
            if ((0 == dwPartialNotificationMask) && (EC_ALL_NOTIFICATIONS != pCtlParms->dwCode))
            {
                dwRetVal = EC_E_NOTSUPPORTED;
                goto Exit;
            }

            /* get current NotificationMask */
            if (0 == pCtlParms->dwClientId)
            {
                dwOldNotificationMask = m_oAtEcatDesc.poMaster->GetNotificationMask();
            }
            else
            {
                /* search client descriptor */
                OsLock(m_oAtEcatDesc.poClntDescLock);
                for (pClntDesc = m_oAtEcatDesc.pClntDescHead;
                    (pClntDesc != EC_NULL) && (pClntDesc->dwId != pCtlParms->dwClientId);
                    pClntDesc = pClntDesc->pNext);

                OsUnlock(m_oAtEcatDesc.poClntDescLock);

                if (EC_NULL == pClntDesc)
                {
                    EC_ERRORMSGC(("EC_IOCTL_SET_NOTIFICATION_ENABLED: Invalid client ID %d\n", pCtlParms->dwClientId));
                    dwRetVal = EC_E_NOTFOUND;
                    goto Exit;
                }
                dwOldNotificationMask = pClntDesc->dwNotificationMask;
            }

            /* apply request to notification mask */
            switch (pCtlParms->dwEnabled)
            {
            case EC_NOTIFICATION_DISABLED:
                    if (EC_ALL_NOTIFICATIONS == pCtlParms->dwCode)
                    {
                        dwNewNotificationMask = 0;
                    }
                    else
                    {
                        dwNewNotificationMask = dwOldNotificationMask & ~dwPartialNotificationMask;
                    }
                break;

            case EC_NOTIFICATION_ENABLED:
                    if (EC_ALL_NOTIFICATIONS == pCtlParms->dwCode)
                    {
                        dwNewNotificationMask = EC_ALL_NOTIFICATIONS;
                    }
                    else
                    {
                        dwNewNotificationMask = dwOldNotificationMask | dwPartialNotificationMask;
                    }
                break;

            case EC_NOTIFICATION_DEFAULT:
                    if (EC_ALL_NOTIFICATIONS == pCtlParms->dwCode)
                    {
                        dwNewNotificationMask = NOTIFICATION_MASK_DEFAULT;
                    }
                    else
                    {
                        dwNewNotificationMask = dwOldNotificationMask
                            | (dwPartialNotificationMask & NOTIFICATION_MASK_DEFAULT);
                    }
                break;
            default:
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_ERROR;
                goto Exit;
            }

            /* set new notification mask */
            if (0 == pCtlParms->dwClientId)
            {
                m_oAtEcatDesc.poMaster->SetNotificationMask(dwNewNotificationMask);
            }
            else
            {
                pClntDesc->dwNotificationMask = dwNewNotificationMask;
            }

            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_GET_NOTIFICATION_ENABLED:             /* (EC_IOCTL_GENERIC | 54) */
        {
            EC_T_CLNTDESC* pClntDesc = EC_NULL;
            EC_T_GET_NOTIFICATION_ENABLED_PARMS* pCtlParms = EC_NULL;
            EC_T_BOOL* pCtlResults = EC_NULL;
            EC_T_DWORD dwPartialNotificationMask = 0;
            EC_T_DWORD dwNotificationMask = 0;
            EC_T_DWORD bNotificationEnabled = EC_FALSE;

            if ((EC_NULL                                    == pParms)
             || (EC_NULL                                    == pParms->pbyInBuf)
             || (sizeof(EC_T_GET_NOTIFICATION_ENABLED_PARMS)   > pParms->dwInBufSize)
             || (EC_NULL                                    == pParms->pbyOutBuf)
             || (sizeof(EC_T_BOOL) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            pCtlParms = (EC_T_GET_NOTIFICATION_ENABLED_PARMS*)pParms->pbyInBuf;
            pCtlResults = (EC_T_BOOL*)pParms->pbyOutBuf;

            /* check if EC_NOTIFY_... code supported */
            dwPartialNotificationMask = NotificationCodeToNotificationMask(pCtlParms->dwCode);
            if (0 == dwPartialNotificationMask)
            {
                dwRetVal = EC_E_NOTSUPPORTED;
                goto Exit;
            }

            if (0 == pCtlParms->dwClientId)
            {
                dwNotificationMask = m_oAtEcatDesc.poMaster->GetNotificationMask();
            }
            else
            {
                /* search client descriptor */
                OsLock(m_oAtEcatDesc.poClntDescLock);
                for (pClntDesc = m_oAtEcatDesc.pClntDescHead;
                    (pClntDesc != EC_NULL) && (pClntDesc->dwId != pCtlParms->dwClientId);
                    pClntDesc = pClntDesc->pNext);

                OsUnlock(m_oAtEcatDesc.poClntDescLock);

                if (EC_NULL == pClntDesc)
                {
                    dwRetVal = EC_E_INVALIDPARM;
                    goto Exit;
                }

                dwNotificationMask = pClntDesc->dwNotificationMask;
            }

            bNotificationEnabled = (0 != (dwNotificationMask & dwPartialNotificationMask));
            *pCtlResults = bNotificationEnabled;

            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_BOOL));
            }
            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_SET_MASTER_DEFAULT_TIMEOUTS:          /* (EC_IOCTL_GENERIC | 55) */
        {
            EC_T_MASTERDEFAULTTIMEOUTS_DESC* pSetDefaultTimeoutsParms = EC_NULL;

            if ((EC_NULL != pParms) && (EC_NULL != pParms->pbyInBuf))
            {
                if (sizeof(EC_T_MASTERDEFAULTTIMEOUTS_DESC) > pParms->dwInBufSize)
                {
                    dwRetVal = EC_E_INVALIDPARM;
                    goto Exit;
                }
                pSetDefaultTimeoutsParms = (EC_T_MASTERDEFAULTTIMEOUTS_DESC*)pParms->pbyInBuf;
            }

            dwRetVal = m_oAtEcatDesc.poMaster->SetDefaultTimeouts(pSetDefaultTimeoutsParms);
        } break;

    case EC_IOCTL_SET_COPYINFO_IN_SENDCYCFRAMES:        /* (EC_IOCTL_GENERIC | 56) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->SetCopyInfoInSendCycFrames((EC_T_BOOL)(0 != EC_GETDWORD(pParms->pbyInBuf)));

            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_SET_BUS_CYCLE_TIME:               /* (EC_IOCTL_GENERIC | 57) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            dwRetVal = SetMasterInitParmBusCycleTimeUsec(EC_GETDWORD(pParms->pbyInBuf));		
        } break;
#if (defined INCLUDE_VARREAD)
    case EC_IOCTL_ADDITIONAL_VARIABLES_FOR_SPECIFIC_DATA_TYPES: /* (EC_IOCTL_GENERIC | 58) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_BOOL) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->SetAdditionalVariablesForSpecificDataTypesEnabled(EC_GETBOOL(pParms->pbyInBuf));
            dwRetVal = EC_E_NOERROR;
        } break;
#endif /* INCLUDE_VARREAD */

    case EC_IOCTL_SET_IGNORE_INPUTS_ON_WKC_ERROR: /* (EC_IOCTL_GENERIC | 59) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->SetIgnoreInputsOnWkcError((EC_T_BOOL)(0 != EC_GETDWORD(pParms->pbyInBuf)));

            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_SET_GENENI_ASSIGN_EEPROM_BACK_TO_ECAT: /* (EC_IOCTL_GENERIC | 60) */
    {
        if ((EC_NULL == pParms)
            || (EC_NULL == pParms->pbyInBuf)
            || (sizeof(EC_T_BOOL) > pParms->dwInBufSize))
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        m_oAtEcatDesc.poMaster->SetGenEniAssignEepromBackToEcat(EC_GETBOOL(pParms->pbyInBuf));

        dwRetVal = EC_E_NOERROR;
    } break;

    case EC_IOCTL_SET_AUTO_ACK_AL_STATUS_ERROR_ENABLED: /* (EC_IOCTL_GENERIC | 61) */
    {
        if ((EC_NULL == pParms)
            || (EC_NULL == pParms->pbyInBuf)
            || (sizeof(EC_T_BOOL) > pParms->dwInBufSize))
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        m_oAtEcatDesc.poMaster->SetAutoAckAlStatusError(EC_GETBOOL(pParms->pbyInBuf));

        dwRetVal = EC_E_NOERROR;
    } break;

    case EC_IOCTL_SET_AUTO_ADJUST_CYCCMD_WKC_ENABLED: /* (EC_IOCTL_GENERIC | 62) */
    {
        if ((EC_NULL == pParms)
            || (EC_NULL == pParms->pbyInBuf)
            || (sizeof(EC_T_BOOL) > pParms->dwInBufSize))
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        if (m_oAtEcatDesc.poMaster->IsConfigured())
        {
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }
        m_oAtEcatDesc.poMaster->SetAutoAdjustCycCmdWkc(EC_GETBOOL(pParms->pbyInBuf));

        dwRetVal = EC_E_NOERROR;
    } break;
    
    case EC_IOCTL_CLEAR_MASTER_INFO_COUNTERS: /* (EC_IOCTL_GENERIC | 63) */
    {
        EC_T_CLEAR_MASTER_INFO_COUNTERS_PARMS* pCounters = EC_NULL;
        if ((EC_NULL == pParms)
            || (EC_NULL == pParms->pbyInBuf)
            || (pParms->dwInBufSize < sizeof(EC_T_CLEAR_MASTER_INFO_COUNTERS_PARMS)))
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        pCounters = (EC_T_CLEAR_MASTER_INFO_COUNTERS_PARMS*)pParms->pbyInBuf;

#if (defined INCLUDE_MASTER_OBD)
        m_oAtEcatDesc.poMaster->m_pSDOService->ClearBusDiagnosisCounters(pCounters->dwClearBusDiagnosisCounters);
#endif /* INCLUDE_MAILBOX_STATISTICS */
#if (defined INCLUDE_MAILBOX_STATISTICS)
        m_oAtEcatDesc.poMaster->ClearMailboxStatisticsCounters(pCounters->qwMailboxStatisticsClearCounters);
#endif /* INCLUDE_MAILBOX_STATISTICS */

        dwRetVal = EC_E_NOERROR;
    } break;

    case EC_IOCTL_SET_ADJUST_CYCFRAMES_AFTER_SLAVES_STATE_CHANGE: /* (EC_IOCTL_GENERIC | 65) */
    {
        if ((EC_NULL == pParms)
            || (EC_NULL == pParms->pbyInBuf)
            || (sizeof(EC_T_BOOL) > pParms->dwInBufSize))
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        m_oAtEcatDesc.poMaster->SetAdjustCycFramesAfterslavesStateChange(EC_GETBOOL(pParms->pbyInBuf));

        dwRetVal = EC_E_NOERROR;
    } break;

    case EC_IOCTL_SET_ZERO_INPUTS_ON_WKC_ZERO: /* (EC_IOCTL_GENERIC | 69) */
    {
        if ((EC_NULL == pParms)
            || (EC_NULL == pParms->pbyInBuf)
            || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        m_oAtEcatDesc.poMaster->SetZeroInputsOnWkcZero((EC_T_BOOL)(0 != EC_GETDWORD(pParms->pbyInBuf)));

        dwRetVal = EC_E_NOERROR;
    } break;
    case EC_IOCTL_SET_ZERO_INPUTS_ON_WKC_ERROR: /* (EC_IOCTL_GENERIC | 70) */
    {
        if ((EC_NULL == pParms)
            || (EC_NULL == pParms->pbyInBuf)
            || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        m_oAtEcatDesc.poMaster->SetZeroInputsOnWkcError((EC_T_BOOL)(0 != EC_GETDWORD(pParms->pbyInBuf)));

        dwRetVal = EC_E_NOERROR;
    } break;
#if (defined INCLUDE_DC_SUPPORT)
    /* #######################################################################################################
       ##### Distributed Clocks (EC_IOCTL_DC 0x00030000)                                                 #####
       #######################################################################################################*/

    case EC_IOCTL_REG_DC_SLV_SYNC_NTFY:                 /* (EC_IOCTL_DC |  3) */
        {
            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_UNREG_DC_SLV_SYNC_NTFY:               /* (EC_IOCTL_DC |  4) */
        {
            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_DC_SLV_SYNC_STATUS_GET:               /* (EC_IOCTL_DC |  5) */
        {
            if ((EC_NULL                       == pParms)
             || (EC_NULL                       == pParms->pbyOutBuf)
             || (sizeof(EC_T_DC_SYNC_NTFY_DESC) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            OsMemcpy(pParms->pbyOutBuf, m_oAtEcatDesc.poMaster->m_pDistributedClks->GetSlaveSyncNotification(), sizeof(EC_T_DC_SYNC_NTFY_DESC));

            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_DC_SYNC_NTFY_DESC));
            }
            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_DC_SLV_SYNC_DEVLIMIT_SET:             /* (EC_IOCTL_DC |  6) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->m_pDistributedClks->SetDeviationLimit(EC_GETDWORD(pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_DC_SLV_SYNC_DEVLIMIT_GET:             /* (EC_IOCTL_DC |  7) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyOutBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            EC_SETDWORD(pParms->pbyOutBuf, m_oAtEcatDesc.poMaster->m_pDistributedClks->GetDeviationLimit());

            if (pParms->pdwNumOutData != EC_NULL )
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_DWORD));
            }
            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_DC_SHIFT_SYSTIME:                     /* (EC_IOCTL_DC | 16) */
        {
            EC_T_DC_SHIFTSYSTIME_DESC oSysTimeDesc;

            if ((EC_NULL                          == pParms)
             || (EC_NULL                          == pParms->pbyInBuf)
             || (sizeof(EC_T_DC_SHIFTSYSTIME_DESC) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            OsMemcpy(&oSysTimeDesc, pParms->pbyInBuf, sizeof(EC_T_DC_SHIFTSYSTIME_DESC));

            m_oAtEcatDesc.poMaster->m_pDistributedClks->DcShiftSystemTime(&oSysTimeDesc);

            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_DC_SETSYNCSTARTOFFSET:                /* (EC_IOCTL_DC | 17) */
        {
            EC_T_DC_STARTCYCSAFETY_DESC oStartCycSafety;

            if ((EC_NULL                            == pParms)
             || (EC_NULL                            == pParms->pbyInBuf)
             || (sizeof(EC_T_DC_STARTCYCSAFETY_DESC) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            OsMemcpy(&oStartCycSafety, pParms->pbyInBuf, sizeof(EC_T_DC_STARTCYCSAFETY_DESC));

            m_oAtEcatDesc.poMaster->m_pDistributedClks->SetDcStartTimeSafetyNsec(EC_MAKEQWORD((oStartCycSafety.dwStartCycSafetyHi), (oStartCycSafety.dwStartCycSafetyLo)));

            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_DC_FIRST_DC_SLV_AS_REF_CLOCK:         /* (EC_IOCTL_DC | 18) */
        {
            if ((EC_NULL == pParms)
                || (EC_NULL == pParms->pbyInBuf)
                || (sizeof(EC_T_BOOL) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            m_oAtEcatDesc.poMaster->m_pDistributedClks->SetFirstDcSlaveAsRefClock(EC_GETBOOL(pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;
#endif /* INCLUDE_DC_SUPPORT */

    /* #######################################################################################################
       ##### Bus Topology Scan (EC_IOCTL_SB 0x00050000)                                                  #####
       #######################################################################################################*/
    case EC_IOCTL_SB_RESTART:                           /* (EC_IOCTL_SB | 1) */
        {
            dwRetVal = McSmOrderAdd(eMcSmOrderType_REQ_SB, &m_oAtEcatDesc.pMcSmOrderBusScan, EC_TRUE, EC_WAITINFINITE);

        } break;
    case EC_IOCTL_SB_ACCEPT_TOPOLOGY_CHANGE:            /* (EC_IOCTL_SB | 16) */
        {
            EC_DBGMSGL((byDbgLvl, ETHTYP1_ECATAPI, ETHTYP0_GENERIC, "EC_IOCTL_SB_ACCEPT_TOPOLOGY_CHANGE\n"));
            dwRetVal = McSmOrderAdd(eMcSmOrderType_SB_ACCEPT_TOPOLOGY_CHANGE, &m_oAtEcatDesc.pMcSmOrderBusScan, EC_TRUE, EC_WAITINFINITE);

        } break;
    case EC_IOCTL_SB_STATUS_GET:                        /* (EC_IOCTL_SB | 2) */
        {
            EC_T_SB_STATUS_NTFY_DESC    ScanbusStatus;

            if ((EC_NULL                         == pParms)
             || (EC_NULL                         == pParms->pbyOutBuf)
             || (sizeof(EC_T_SB_STATUS_NTFY_DESC) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            if ((m_oAtEcatDesc.pMcSmOrderBusScan != EC_NULL) && (m_oAtEcatDesc.pMcSmOrderBusScan->eOrderState != eEcatOrderState_Idle))
            {
                ScanbusStatus.dwResultCode = EC_E_BUSY;
            }
            else
            {
                m_oAtEcatDesc.pMcSmOrderBusScan = EC_NULL;
                ScanbusStatus.dwResultCode = m_oAtEcatDesc.poMaster->GetBusScanResult();
            }
            ScanbusStatus.dwSlaveCount = m_oAtEcatDesc.poMaster->GetScanBusSlaveCount();
            OsMemcpy(pParms->pbyOutBuf, &ScanbusStatus, sizeof(EC_T_SB_STATUS_NTFY_DESC));
            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_SB_STATUS_NTFY_DESC));
            }
            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_SB_SET_BUSCNF_VERIFY:                 /* (EC_IOCTL_SB | 3) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->SetSBVerificationEnable((EC_TRUE == EC_GETDWORD(pParms->pbyInBuf)));

            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_SB_SET_BUSCNF_VERIFY_PROP:            /* (EC_IOCTL_SB | 4) */
        {
            EC_T_SCANBUS_PROP_DESC oProperty;

            if ((EC_NULL                       == pParms)
             || (EC_NULL                       == pParms->pbyInBuf)
             || (sizeof(EC_T_SCANBUS_PROP_DESC) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            OsMemcpy(&oProperty, pParms->pbyInBuf, sizeof(EC_T_SCANBUS_PROP_DESC));

            dwRetVal = m_oAtEcatDesc.poMaster->m_pBusTopology->SetCheckEEPROMEntryByIoctl(&oProperty);
        } break;
    case EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO:              /* (EC_IOCTL_SB | 5) */
        {
            EC_T_WORD wAutoIncrementAddr;

            if ((EC_NULL                       == pParms)
             || (EC_NULL                       == pParms->pbyInBuf)
             || (EC_NULL                       == pParms->pbyOutBuf)
             || (sizeof(EC_T_WORD)              > pParms->dwInBufSize)
             || (sizeof(EC_T_SB_SLAVEINFO_DESC) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            wAutoIncrementAddr = EC_GETWORD(pParms->pbyInBuf);

            dwRetVal = m_oAtEcatDesc.poMaster->SbGetSlaveInfo(wAutoIncrementAddr, (EC_T_SB_SLAVEINFO_DESC*)pParms->pbyOutBuf);
            if (EC_E_NOERROR != dwRetVal)
            {
                goto Exit;
            }
            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_SB_SLAVEINFO_DESC));
            }
        } break;
    case EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO_EEP:          /* (EC_IOCTL_SB | 6) */
        {
            if ((EC_NULL                               == pParms)
             || (EC_NULL                               == pParms->pbyInBuf)
             || (EC_NULL                               == pParms->pbyOutBuf)
             || (sizeof(EC_T_SB_SLAVEINFO_EEP_REQ_DESC) > pParms->dwInBufSize)
             || (sizeof(EC_T_SB_SLAVEINFO_EEP_RES_DESC) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            dwRetVal = m_oAtEcatDesc.poMaster->SbGetSlaveInfoEep((EC_T_SB_SLAVEINFO_EEP_REQ_DESC*)pParms->pbyInBuf, (EC_T_SB_SLAVEINFO_EEP_RES_DESC*)pParms->pbyOutBuf);
            if (EC_E_NOERROR != dwRetVal)
            {
                goto Exit;
            }
            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_SB_SLAVEINFO_EEP_RES_DESC));
            }
        } break;
    case EC_IOCTL_SB_ENABLE:                            /* (EC_IOCTL_SB | 7) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->SetSBEnabled(EC_GETDWORD(pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO_EX:           /* (EC_IOCTL_SB | 9) */
        {
            if ((EC_NULL                           == pParms)
             || (EC_NULL                           == pParms->pbyInBuf)
             || (EC_NULL                           == pParms->pbyOutBuf)
             || (sizeof(EC_T_SB_SLAVEINFO_REQ_DESC) > pParms->dwInBufSize)
             || (sizeof(EC_T_SB_SLAVEINFO_RES_DESC) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            dwRetVal = m_oAtEcatDesc.poMaster->SbGetSlaveInfoEx((EC_T_SB_SLAVEINFO_REQ_DESC*)pParms->pbyInBuf, (EC_T_SB_SLAVEINFO_RES_DESC*)pParms->pbyOutBuf);
            if (EC_E_NOERROR != dwRetVal)
            {
                goto Exit;
            }
            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_SB_SLAVEINFO_RES_DESC));
            }
        } break;
    case EC_IOCTL_SLV_ALIAS_ENABLE:                     /* (EC_IOCTL_SB |10) */
        {
            dwRetVal = m_oAtEcatDesc.poMaster->EnableAliasAddressing(EC_TRUE);
        } break;
    case EC_IOCTL_SB_SET_BUSCNF_READ_PROP:            /* (EC_IOCTL_SB | 12) */
        {
            EC_T_SCANBUS_PROP_DESC  oProperty;

            if ((EC_NULL                       == pParms)
             || (EC_NULL                       == pParms->pbyInBuf)
             || (sizeof(EC_T_SCANBUS_PROP_DESC) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            OsMemcpy(&oProperty, pParms->pbyInBuf, sizeof(EC_T_SCANBUS_PROP_DESC));

            dwRetVal = m_oAtEcatDesc.poMaster->m_pBusTopology->SetReadEEPROMEntryByIoctl(&oProperty);
        } break;
    case EC_IOCTL_SB_SET_TOPOLOGY_CHANGED_DELAY:        /* (EC_IOCTL_SB |13) */
        {
            if ((EC_NULL           == pParms)
             || (EC_NULL           == pParms->pbyInBuf)
             || (sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->m_pBusTopology->SetTopologyChangeDelay(*((EC_T_DWORD*)pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    case EC_IOCTL_SB_SET_ERROR_ON_CROSSED_LINES:        /* (EC_IOCTL_SB |14) */
        {
            if ((EC_NULL          == pParms)
             || (EC_NULL          == pParms->pbyInBuf)
             || (sizeof(EC_T_BOOL) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->m_pBusTopology->SetErrorOnLineCrossed(*((EC_T_DWORD*)pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;
#endif /* INCLUDE_LINE_CROSSED_DETECTION */
    case EC_IOCTL_SB_SET_TOPOLOGY_CHANGE_AUTO_MODE:     /* (EC_IOCTL_SB |15) */
        {
            if(
                    (EC_NULL == pParms)
                ||  (EC_NULL == pParms->pbyInBuf)
                ||  (pParms->dwInBufSize < sizeof(EC_T_BOOL ))
              )
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            EC_DBGMSGL((byDbgLvl, ETHTYP1_ECATAPI, ETHTYP0_GENERIC, "EC_IOCTL_SB_SET_TOPOLOGY_CHANGE_AUTO_MODE\n"));
            m_oAtEcatDesc.poMaster->m_pBusTopology->SetTopologyChangeAutoMode(*((EC_T_DWORD*)pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_SB_NOTIFY_UNEXPECTED_BUS_SLAVES:     /* (EC_IOCTL_SB |17) */
        {
            if(
                    (EC_NULL == pParms)
                ||  (EC_NULL == pParms->pbyInBuf)
                ||  (pParms->dwInBufSize < sizeof(EC_T_BOOL ))
              )
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->m_pBusTopology->SetNotifyUnexpectedBusSlaves(*((EC_T_DWORD*)pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;

#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
    case EC_IOCTL_SB_SET_RED_ENHANCED_LINE_CROSSED_DETECTION_ENABLED: /* (EC_IOCTL_SB |18) */
        {
            if(
                    (EC_NULL == pParms)
                ||  (EC_NULL == pParms->pbyInBuf)
                ||  (pParms->dwInBufSize < sizeof(EC_T_BOOL ))
              )
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->m_pBusTopology->SetRedEnhancedLineCrossedDetectionEnabled(*((EC_T_BOOL*)pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;
#endif /* INCLUDE_RED_DEVICE */

#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    case EC_IOCTL_SB_SET_NOTIFY_NOT_CONNECTED_PORT_A: /* (EC_IOCTL_SB |19) */
        {
            if(
                    (EC_NULL == pParms)
                ||  (EC_NULL == pParms->pbyInBuf)
                ||  (pParms->dwInBufSize < sizeof(EC_T_BOOL ))
              )
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->m_pBusTopology->SetNotifyNotConnectedPortA(*((EC_T_BOOL*)pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_SB_SET_NOTIFY_UNEXPECTED_CONNECTED_PORT: /* (EC_IOCTL_SB |20) */
        {
            if(
                    (EC_NULL == pParms)
                ||  (EC_NULL == pParms->pbyInBuf)
                ||  (pParms->dwInBufSize < sizeof(EC_T_BOOL ))
              )
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->m_pBusTopology->SetNotifyUnexpectedConnectedPort(*((EC_T_BOOL*)pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;
#endif /* INCLUDE_LINE_CROSSED_DETECTION */

#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    case EC_IOCTL_SB_SET_JUNCTION_REDUNDANCY_ENABLED: /* (EC_IOCTL_SB |21) */
        {
            if(
                    (EC_NULL == pParms)
                ||  (EC_NULL == pParms->pbyInBuf)
                ||  (pParms->dwInBufSize < sizeof(EC_T_BOOL ))
              )
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            m_oAtEcatDesc.poMaster->m_pBusTopology->SetJunctionRedundancyEnabled(*((EC_T_BOOL*)pParms->pbyInBuf));

            dwRetVal = EC_E_NOERROR;
        } break;
#endif /* INCLUDE_JUNCTION_REDUNDANCY */

    case EC_IOCTL_SB_GET_BUS_SLAVE_PORTS_INFO: /* (EC_IOCTL_SB |21) */
        {
            /* check parameters */
            if ((EC_NULL                          == pParms)
             || (EC_NULL                          == pParms->pbyInBuf)
             || (EC_NULL                          == pParms->pbyOutBuf)
             || (sizeof(EC_T_DWORD)                > pParms->dwInBufSize)
             || (sizeof(EC_T_BUS_SLAVE_PORTS_INFO) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            pBusSlave = m_oAtEcatDesc.poMaster->GetBusSlaveBySlaveId(EC_GETDWORD(pParms->pbyInBuf));
            if (EC_NULL == pBusSlave)
            {
                dwRetVal = EC_E_NOTFOUND;
                goto Exit;
            }
            dwRetVal = pBusSlave->GetPortsInfo((EC_T_BUS_SLAVE_PORTS_INFO*)pParms->pbyOutBuf);
            if (EC_E_NOERROR != dwRetVal)
            {
                goto Exit;
            }
            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_BUS_SLAVE_PORTS_INFO));
            }
        } break;

    /* #######################################################################################################
       ##### Hot Connect (EC_IOCTL_HC 0x00060000)                                                        #####
       #######################################################################################################*/
#if (defined INCLUDE_HOTCONNECT)
    case EC_IOCTL_HC_SETMODE:                           /* (EC_IOCTL_HC | 1) */
        {
            if ((EC_NULL                     == pParms)
             || (EC_NULL                     == pParms->pbyInBuf)
             || (sizeof(EC_T_EHOTCONNECTMODE) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            dwRetVal = m_oAtEcatDesc.poMaster->SetHCMode((EC_T_EHOTCONNECTMODE)EC_GETDWORD(pParms->pbyInBuf));
        } break;

    case EC_IOCTL_HC_GETMODE:                           /* (EC_IOCTL_HC | 2) */
        {
            if ((EC_NULL                       == pParms)
             || (EC_NULL                       == pParms->pbyOutBuf)
             || (sizeof(EC_T_EHOTCONNECTMODE) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            EC_SETDWORD(pParms->pbyOutBuf, m_oAtEcatDesc.poMaster->GetHCMode());

            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_EHOTCONNECTMODE));
            }
            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_HC_CONFIGURETIMEOUTS:                 /* (EC_IOCTL_HC | 2) */
        {
            EC_T_HC_CONFIGURETIMEOUTS_DESC   oConfigTimeouts = {0};
            if ((EC_NULL                               == pParms)
             || (EC_NULL                               == pParms->pbyInBuf)
             || (sizeof(EC_T_HC_CONFIGURETIMEOUTS_DESC) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            OsMemcpy(&oConfigTimeouts, pParms->pbyInBuf, sizeof(EC_T_HC_CONFIGURETIMEOUTS_DESC));

            dwRetVal = m_oAtEcatDesc.poMaster->HCConfigureTimeouts(&oConfigTimeouts);
        } break;
#endif /* INCLUDE_HOTCONNECT */
#if (defined INCLUDE_DC_SUPPORT)
    /* #######################################################################################################
       ##### Distributed Clocks Master Sync (EC_IOCTL_DCM 0x00070000)                                    #####
       #######################################################################################################*/
    case EC_IOCTL_DCM_REGISTER_TIMESTAMP:               /* (EC_IOCTL_DCM |  1) */
        {
            if ((EC_NULL                        == pParms)
             || (EC_NULL                        == pParms->pbyInBuf)
             || (EC_NULL                        == pParms->pbyOutBuf)
             || (sizeof(EC_T_REGISTER_TSPARMS)   > pParms->dwInBufSize)
             || (sizeof(EC_T_REGISTER_TSRESULTS) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            dwRetVal = m_oAtEcatDesc.poMaster->m_pDistributedClks->DcmRegisterTimeStampCallback((EC_T_REGISTER_TSPARMS*)pParms->pbyInBuf, (EC_T_REGISTER_TSRESULTS*)pParms->pbyOutBuf);
            if (EC_E_NOERROR != dwRetVal)
            {
                goto Exit;
            }
            if (pParms->pdwNumOutData != EC_NULL)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_REGISTER_TSRESULTS));
            }
            dwRetVal = EC_E_NOERROR;
        } break;
    case EC_IOCTL_DCM_UNREGISTER_TIMESTAMP:             /* (EC_IOCTL_DCM |  2) */
        {
            dwRetVal = m_oAtEcatDesc.poMaster->m_pDistributedClks->DcmUnregisterTimeStampCallback();
        } break;
    case EC_IOCTL_DCM_REGISTER_STARTSO_CALLBACK:        /* (EC_IOCTL_DCM |  3) */
        {
            EC_T_DC_SYNCSO_REGDESC* pRegCBParms     = EC_NULL;

            if ((EC_NULL                       == pParms)
             || (EC_NULL                       == pParms->pbyInBuf)
             || (sizeof(EC_T_DC_SYNCSO_REGDESC) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            pRegCBParms = (EC_T_DC_SYNCSO_REGDESC*)pParms->pbyInBuf;

            dwRetVal = m_oAtEcatDesc.poMaster->m_pDistributedClks->DcmRegisterDcStartTimeCallback(pRegCBParms);
        } break;
    case EC_IOCTL_DCM_GET_LOG:                          /* (EC_IOCTL_DCM |  4) */
        {
            if ((EC_NULL             == pParms)
             || (EC_NULL             == pParms->pbyOutBuf)
             || (sizeof(EC_T_DCM_LOG) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            dwRetVal = m_oAtEcatDesc.poMaster->m_pDistributedClks->DcmGetLog((EC_T_DCM_LOG*)pParms->pbyOutBuf);

            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_DCM_LOG));
            }
        } break;
#endif /* INCLUDE_DC_SUPPORT */

    /* #######################################################################################################
       ##### Private (EC_IOCTL_PRIVATE 0x00FF0000)                                                       #####
       #######################################################################################################*/
#if (defined FRAME_LOSS_SIMULATION)
    case EC_IOCTL_SET_FRAME_LOSS_SIMULATION:            /* (EC_IOCTL_PRIVATE | 1) */
    case EC_IOCTL_SET_RXFRAME_LOSS_SIMULATION:          /* (EC_IOCTL_PRIVATE | 2) */
    case EC_IOCTL_SET_TXFRAME_LOSS_SIMULATION:          /* (EC_IOCTL_PRIVATE | 3) */
        {
            if ((EC_NULL               == pParms)
             || (EC_NULL               == pParms->pbyInBuf)
             || (4 * sizeof(EC_T_DWORD) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            if ((dwCode == EC_IOCTL_SET_FRAME_LOSS_SIMULATION) || (dwCode == EC_IOCTL_SET_RXFRAME_LOSS_SIMULATION))
            {
                m_oAtEcatDesc.poEcDevice->SetRxFrameLossSimulation(((EC_T_DWORD*)pParms->pbyInBuf)[0], ((EC_T_DWORD*)pParms->pbyInBuf)[1], ((EC_T_DWORD*)pParms->pbyInBuf)[2], ((EC_T_DWORD*)pParms->pbyInBuf)[3]);
            }
            if ((dwCode == EC_IOCTL_SET_FRAME_LOSS_SIMULATION) || (dwCode == EC_IOCTL_SET_TXFRAME_LOSS_SIMULATION))
            {
                m_oAtEcatDesc.poEcDevice->SetTxFrameLossSimulation(((EC_T_DWORD*)pParms->pbyInBuf)[0], ((EC_T_DWORD*)pParms->pbyInBuf)[1], ((EC_T_DWORD*)pParms->pbyInBuf)[2], ((EC_T_DWORD*)pParms->pbyInBuf)[3]);
            }
            dwRetVal = EC_E_NOERROR;
        } break;
#endif /* FRAME_LOSS_SIMULATION */

    case EC_IOCTL_GET_FAST_CONTEXT:          /* (EC_IOCTL_PRIVATE | 4) */
        {
            EC_T_FAST_MASTER_DESC   oDesc;

            /* check IO parameters */
            if ((EC_NULL == pParms)
                || (EC_NULL == pParms->pbyOutBuf)
                || (sizeof(EC_T_FAST_MASTER_DESC) > pParms->dwOutBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            /* check if jobs are allowed */
            if (!m_oAtEcatDesc.bJobsActive)
            {
                dwRetVal = EC_E_INVALIDSTATE;
                goto Exit;
            }

            /* check pre-requirements */
            if (!m_oAtEcatDesc.poMaster->IsConfigured()
                || EcLinkMode_POLLING != m_oAtEcatDesc.eLinkMode
                || eCycFrameLayout_IN_DMA != m_oAtEcatDesc.poMaster->GetCycFrameLayout()
                )
            {
                dwRetVal = EC_E_INVALIDSTATE;
                goto Exit;
            }

            /* fill descriptor */
            oDesc.wNumSendCycFrames = 0;
            oDesc.pPDOut = m_oAtEcatDesc.poMaster->GetPDOutPtr();
            oDesc.pPDIn = m_oAtEcatDesc.poMaster->GetPDInPtr();

            oDesc.pCyclicEntryMgmtDesc = m_oAtEcatDesc.poMaster->GetCycEntryDesc(0);
            oDesc.poLinkDrvDesc = &(m_oAtEcatDesc.poEcDevice->m_LinkDrvDesc);

            OsMemcpy(pParms->pbyOutBuf, &oDesc, sizeof(EC_T_FAST_MASTER_DESC));

            if (EC_NULL != pParms->pdwNumOutData)
            {
                EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_FAST_MASTER_DESC));
            };
            dwRetVal = EC_E_NOERROR;
        } break;

#if (defined INCLUDE_OEM)
    case EC_IOCTL_SET_OEM_KEY:
        {
            /* check IO parameters */
            if ((EC_NULL == pParms)
                || (EC_NULL == pParms->pbyInBuf)
                || (sizeof(EC_T_UINT64) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            SetOemKey(EC_GETQWORD(pParms->pbyInBuf));
            dwRetVal = EC_E_NOERROR;
        } break;

    case EC_IOCTL_CHECK_OEM_KEY:
        {
            /* check IO parameters */
            if ((EC_NULL == pParms)
                || (EC_NULL == pParms->pbyInBuf)
                || (sizeof(EC_T_UINT64) > pParms->dwInBufSize))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }

            dwRetVal = CheckOemKey(EC_GETQWORD(pParms->pbyInBuf));
        } break;
        break;
#endif

    default:
        {
            /* handle linklayer iocontrol */
            if ((EC_IOCTL_LINKLAYER_MAIN < dwCode) && (EC_IOCTL_LINKLAYER_RED > dwCode))
            {
                OsDbgAssert(sizeof(EC_T_IOCTLPARMS) == sizeof(EC_T_LINK_IOCTLPARMS));
                dwRetVal = m_oAtEcatDesc.poEcDevice->LinkIoctl(dwCode & ~EC_IOCTL_LINKLAYER_MAIN, (EC_T_LINK_IOCTLPARMS*)pParms);
            }
#if (defined INCLUDE_RED_DEVICE)
            else if ((EC_IOCTL_LINKLAYER_RED < dwCode) && (EC_IOCTL_LINKLAYER_LAST >= dwCode) && (m_oAtEcatDesc.poEcRedDevice != EC_NULL))
            {
                OsDbgAssert(sizeof(EC_T_IOCTLPARMS) == sizeof(EC_T_LINK_IOCTLPARMS));
                dwRetVal = m_oAtEcatDesc.poEcRedDevice->LinkIoctl(dwCode & ~EC_IOCTL_LINKLAYER_RED, (EC_T_LINK_IOCTLPARMS*)pParms);
            }
#endif /* INCLUDE_RED_DEVICE */
            else
            {
                dwRetVal = EC_E_NOTSUPPORTED;
                goto Exit;
            }
        }
    }

Exit:
    if (bClntDescLocked )
    {
        OsUnlock(m_oAtEcatDesc.poClntDescLock);
    }
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Scans all connected slaves
    \return EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::ScanBus(
    EC_T_DWORD  dwTimeout   /**< [in] Time-out in ms */
)
{
    EC_T_DWORD          dwRetVal    = EC_E_ERROR;
    EC_T_MCSM_ORDER*    poMcSmOrder = EC_NULL;
    EC_T_DWORD          dwOrderId   = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ScanBus: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check if ADS adapter is running */
    if (AdsIsRunning())
    {
        dwRetVal = EC_E_ADS_IS_RUNNING;
        goto Exit;
    }

    dwRetVal = McSmOrderAdd(eMcSmOrderType_REQ_SB, &poMcSmOrder, (EC_NOWAIT == dwTimeout), ((EC_NOWAIT == dwTimeout) ? m_oAtEcatDesc.poMaster->GetScanBusDefaultTimeout() : dwTimeout));
    if ((EC_E_NOERROR != dwRetVal) || (EC_NOWAIT == dwTimeout))
    {
        goto Exit;
    }
    dwOrderId = poMcSmOrder->dwOrderId;

    /* wait until the order is finished or timeout */
    for(;;)
    {
        /* sleep a while */
        OsSleep(1);
        if (poMcSmOrder->eOrderState == eEcatOrderState_Finished)
        {
            if (dwOrderId != poMcSmOrder->dwOrderId)
            {
                /* order was overwritten, should never happen */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }
            /* requested state is reached */
            dwRetVal = poMcSmOrder->dwMcSmLastErrorStatus;
            if (EC_E_NOERROR != dwRetVal)
            {
                goto Exit;
            }
            dwRetVal = EC_E_NOERROR;
            break;
        }
    } /* wait until the order was finished */

Exit:
    /* release order if needed */
    if ((EC_NULL != poMcSmOrder) && (EC_NOWAIT != dwTimeout))
    {
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    return dwRetVal;
}

#if (defined INCLUDE_RESCUE_SCAN)
EC_T_DWORD CAtEmInterface::RescueScan(EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatRescueScan() Temporary Master Stack not Initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (EC_NOWAIT == dwTimeout)
    {
        EC_ERRORMSGC(("ecatRescueScan() timeout missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    {
        CRescueScan RescueScan(this);
        dwRetVal = RescueScan.RescueScan(dwTimeout);
    }
Exit:
    return dwRetVal;
}
#endif /* INCLUDE_RESCUE_SCAN */

EC_T_DWORD CAtEmInterface::GetMasterInfo(EC_T_MASTER_INFO* pMasterInfo)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("GetMasterInfo: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check if ADS adapter is running */
    if (AdsIsRunning())
    {
        dwRetVal = EC_E_ADS_IS_RUNNING;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMasterInfo)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    pMasterInfo->dwMasterVersion = ATECAT_VERSION;

#if (defined INCLUDE_MASTER_OBD)
    /* copy Bus Diagnosis Info */
    {
        pMasterInfo->BusDiagnosisInfo.dwCRC32ConfigCheckSum = m_poEcConfigData->GetConfigCheckSum();
        pMasterInfo->BusDiagnosisInfo.dwNumSlavesFound = m_oAtEcatDesc.poMaster->GetScanBusSlaveCount();
        pMasterInfo->BusDiagnosisInfo.dwNumDCSlavesFound = m_oAtEcatDesc.poMaster->m_pBusTopology->GetDCSlaveCount();
        pMasterInfo->BusDiagnosisInfo.dwNumCfgSlaves = m_oAtEcatDesc.poMaster->GetCfgSlaveCount();
        pMasterInfo->BusDiagnosisInfo.dwNumMbxSlaves = (EC_T_DWORD)m_oAtEcatDesc.poMaster->GetMbSlaveCount();

        pMasterInfo->BusDiagnosisInfo.dwTXFrames = *(m_oAtEcatDesc.poMaster->m_pSDOService->GetTXFrameCrtPtr());
        pMasterInfo->BusDiagnosisInfo.dwRXFrames = *(m_oAtEcatDesc.poMaster->m_pSDOService->GetRXFrameCrtPtr());
        pMasterInfo->BusDiagnosisInfo.dwLostFrames = *(m_oAtEcatDesc.poMaster->m_pSDOService->GetLostFrameCrtPtr());

        pMasterInfo->BusDiagnosisInfo.dwCyclicFrames = *(m_oAtEcatDesc.poMaster->m_pSDOService->GetCycFrameCrtPtr());
        pMasterInfo->BusDiagnosisInfo.dwCyclicDatagrams = *(m_oAtEcatDesc.poMaster->m_pSDOService->GetCycDgramCrtPtr());
        pMasterInfo->BusDiagnosisInfo.dwAcyclicFrames = *(m_oAtEcatDesc.poMaster->m_pSDOService->GetAcycFrameCrtPtr());
        pMasterInfo->BusDiagnosisInfo.dwAcyclicDatagrams = *(m_oAtEcatDesc.poMaster->m_pSDOService->GetAcycDgramCrtPtr());
        pMasterInfo->BusDiagnosisInfo.dwClearCounters = m_oAtEcatDesc.poMaster->m_pSDOService->GetBusDiagnosisClearCounters();
    }
#endif /* INCLUDE_MASTER_OBD */

#if (defined INCLUDE_MAILBOX_STATISTICS)
    /* copy Mailbox Statistics */
    OsMemcpy(&pMasterInfo->MailboxStatistics, &m_oAtEcatDesc.poMaster->m_MailboxStatistics, sizeof(EC_T_MAILBOX_STATISTICS));
#endif /* INCLUDE_MAILBOX_STATISTICS */

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get Process Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::GetProcessData(
    EC_T_BOOL   bOutputData,    /**< [in]   EC_TRUE:Output data, EC_FALSE: Input data */
    EC_T_DWORD  dwOffset,       /**< [in]   Offset in PD Image */
    EC_T_BYTE*  pbyData,        /**< [in]   Data container */
    EC_T_DWORD  dwLength,       /**< [in]   Data Bytes to read */
    EC_T_DWORD  dwTimeout       /**< [in]   Timeout value */
)
{
    return GetProcessDataBits(bOutputData, dwOffset * 8, pbyData, dwLength * 8, dwTimeout);
}

/***************************************************************************************************/
/**
\brief  Set Process Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::SetProcessData(
    EC_T_BOOL   bOutputData,    /**< [in]   EC_TRUE:Output data, EC_FALSE: Input data */
    EC_T_DWORD  dwOffset,       /**< [in]   Offset in PD Image */
    EC_T_BYTE*  pbyData,        /**< [in]   Data container */
    EC_T_DWORD  dwLength,       /**< [in]   Data Bytes to write */
    EC_T_DWORD  dwTimeout       /**< [in]   Timeout value */
)
{
    return SetProcessDataBits(bOutputData, dwOffset*8, pbyData, dwLength*8, dwTimeout);
}


/***************************************************************************************************/
/**
\brief  Get Process Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::GetProcessDataBits(
    EC_T_BOOL   bOutputData,     /**< [in]   EC_TRUE:Output data, EC_FALSE: Input data */
    EC_T_DWORD  dwBitOffsetPd,   /**< [in]   Bit offset in PD Image */
    EC_T_BYTE*  pbyDataDst,      /**< [in]   Destination buffer to store the data */
    EC_T_DWORD  dwBitLengthDst,  /**< [in]   Data destination bits to read */
    EC_T_DWORD  dwTimeout        /**< [in]   Timeout value */
)
{
    EC_T_DWORD      dwRetVal    = EC_E_NOERROR;
    CEcTimer  oTimeout;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
	/* check parameters */
    if (EC_NULL == pbyDataDst || 0 == dwBitLengthDst || (EC_NOWAIT == dwTimeout))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (bOutputData)
    {
        if ((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_OUT) * 8) < dwBitOffsetPd )
        {
            dwRetVal = EC_E_INVALIDOFFSET;
            goto Exit;
        }

        if ((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_OUT) * 8) < (dwBitOffsetPd + dwBitLengthDst))
        {
            dwRetVal = EC_E_INVALIDSIZE;
            goto Exit;
        }
    }
    else
    {
        if ((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_IN) * 8) < dwBitOffsetPd )
        {
            dwRetVal = EC_E_INVALIDOFFSET;
            goto Exit;
        }

        if (((EC_T_DWORD)m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_IN) * 8) < (dwBitOffsetPd + dwBitLengthDst))
        {
            dwRetVal = EC_E_INVALIDSIZE;
            goto Exit;
        }
    }

    /* Start timeout */
    oTimeout.Start(dwTimeout);

    /* Only one GetProcessData()-call can be processed at one time */
    OsLock(m_oAtEcatDesc.poGetProcessDataDescLock);

    /* setup copy request structure for job task */
    m_oAtEcatDesc.oGetProcessDataDesc.bOutputData     = bOutputData;
    m_oAtEcatDesc.oGetProcessDataDesc.dwBitOffset     = dwBitOffsetPd;
    m_oAtEcatDesc.oGetProcessDataDesc.pbyData         = pbyDataDst;
    m_oAtEcatDesc.oGetProcessDataDesc.dwBitLength     = dwBitLengthDst;

    /* Set the bRequestStarted-flag for the job task (CAtEmInterface::ExecJobPrivate) */
    m_oAtEcatDesc.oGetProcessDataDesc.bRequestStarted = EC_TRUE;

    do
    {
        OsSleep(1);

        /* check if another request is currently pending */
        if(m_oAtEcatDesc.oGetProcessDataDesc.bRequestStarted == EC_FALSE)
        {
            break;
        }
    } while(!oTimeout.IsElapsed());

    OsUnlock(m_oAtEcatDesc.poGetProcessDataDescLock);

    if(oTimeout.IsElapsed())
    {
        dwRetVal = EC_E_TIMEOUT;
    }
    else
    {
        dwRetVal = EC_E_NOERROR;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Set Process Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::SetProcessDataBits(
    EC_T_BOOL   bOutputData,    /**< [in]   EC_TRUE:Output data, EC_FALSE: Input data */
    EC_T_DWORD  dwBitOffsetPd,  /**< [in]   Data destination (process image) bit offset */
    EC_T_BYTE*  pbyDataSrc,     /**< [in]   Data source */
    EC_T_DWORD  dwBitLengthSrc, /**< [in]   Data source bit length */
    EC_T_DWORD  dwTimeout       /**< [in]   Timeout value */
)
{
    EC_T_DWORD      dwRetVal        = EC_E_ERROR;
    CEcTimer  oTimeout;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pbyDataSrc || 0 == dwBitLengthSrc || (EC_NOWAIT == dwTimeout))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (!bOutputData)
    {
        dwRetVal = EC_E_ACCESSDENIED;
        goto Exit;
    }
    if ((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_OUT) * 8) < dwBitOffsetPd )
    {
        dwRetVal = EC_E_INVALIDOFFSET;
        goto Exit;
    }

    if ((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_OUT) * 8) < (dwBitOffsetPd + dwBitLengthSrc))
    {
        dwRetVal = EC_E_INVALIDSIZE;
        goto Exit;
    }


    /* Start timeout */
    oTimeout.Start(dwTimeout);

    /* Only one SetProcessData()-call can be processed at one time */
    OsLock(m_oAtEcatDesc.poSetProcessDataDescLock);

    /* setup copy request structure for job task */
    m_oAtEcatDesc.oSetProcessDataDesc.bOutputData     = bOutputData;
    m_oAtEcatDesc.oSetProcessDataDesc.dwBitOffset     = dwBitOffsetPd;
    m_oAtEcatDesc.oSetProcessDataDesc.pbyData         = pbyDataSrc;
    m_oAtEcatDesc.oSetProcessDataDesc.dwBitLength     = dwBitLengthSrc;

    /* Set the bRequestStarted-flag for the job task (CAtEmInterface::ExecJobPrivate) */
    m_oAtEcatDesc.oSetProcessDataDesc.bRequestStarted = EC_TRUE;

    /* wait while another request pending */
    do
    {
        OsSleep(1);

        /* check if another request is currently pending */
        if(m_oAtEcatDesc.oSetProcessDataDesc.bRequestStarted == EC_FALSE)
        {
            break;
        }
    } while(!oTimeout.IsElapsed());

    OsUnlock(m_oAtEcatDesc.poSetProcessDataDescLock);

    if(oTimeout.IsElapsed())
    {
        dwRetVal = EC_E_TIMEOUT;
    }
    else
    {
        dwRetVal = EC_E_NOERROR;
    }

Exit:
    return dwRetVal;
}

#if (defined INCLUDE_FORCE_PROCESSDATA)
/***************************************************************************************************/
/**
\brief  Force Process Data: Overrules the data set by application!

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::ForceProcessDataBits(
    EC_T_DWORD  dwClientId,
    EC_T_BOOL   bOutputData,    /**< [in]   EC_TRUE:Output data, EC_FALSE: Input data */
    EC_T_DWORD  dwBitOffsetPd,  /**< [in]   Data destination (process image) bit offset */
    EC_T_WORD   wBitLength,     /**< [in]   Data bit length */
    EC_T_BYTE*  pbyData,     /**< [in]   Data */
    EC_T_DWORD  dwTimeout       /**< [in]   Timeout value */
)
{
    EC_T_DWORD      dwRetVal        = EC_E_ERROR;
    CEcTimer        oTimeout;
    EC_T_DWORD      dwCopylen       = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (0 == dwClientId)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (0 == wBitLength || (EC_NOWAIT == dwTimeout))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwCopylen = (wBitLength + 7) /8;

    if(dwCopylen > sizeof(m_oAtEcatDesc.oForceProcessDataDesc.abyData))
    {
        dwRetVal = EC_E_INVALIDSIZE;
        goto Exit;
    }

    if(bOutputData)
    {
       /* process data outputs */
       if ((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_OUT) * 8) < dwBitOffsetPd )
       {
           dwRetVal = EC_E_INVALIDOFFSET;
           goto Exit;
       }

       if ((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_OUT) * 8) < (dwBitOffsetPd + wBitLength))
       {
           dwRetVal = EC_E_INVALIDSIZE;
           goto Exit;
       }
    }
    else
    {
       /* process data inputs */
       if ((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_IN) * 8) < dwBitOffsetPd )
       {
           dwRetVal = EC_E_INVALIDOFFSET;
           goto Exit;
       }

       if ((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_IN) * 8) < (dwBitOffsetPd + wBitLength))
       {
           dwRetVal = EC_E_INVALIDSIZE;
           goto Exit;
       }
    }

    /* Start timeout */
    oTimeout.Start(dwTimeout);

    /* Only one call can be processed at one time */
    OsLock(m_oAtEcatDesc.poForceRelProcessDataDescLock);

    /* setup copy request structure for job task */
    m_oAtEcatDesc.oForceProcessDataDesc.dwClientId      = dwClientId;
    m_oAtEcatDesc.oForceProcessDataDesc.bOutput         = bOutputData;
    m_oAtEcatDesc.oForceProcessDataDesc.bRelease        = EC_FALSE;
    m_oAtEcatDesc.oForceProcessDataDesc.bReleaseAll     = EC_FALSE;
    m_oAtEcatDesc.oForceProcessDataDesc.dwBitOffset     = dwBitOffsetPd;
    m_oAtEcatDesc.oForceProcessDataDesc.wBitLength      = wBitLength;
    OsMemset(m_oAtEcatDesc.oForceProcessDataDesc.abyData, 0, sizeof(m_oAtEcatDesc.oForceProcessDataDesc.abyData));
    OsMemcpy(m_oAtEcatDesc.oForceProcessDataDesc.abyData, pbyData, dwCopylen);

    /* Set the bRequestStarted-flag for the job task (CAtEmInterface::ExecJobPrivate) */
    m_oAtEcatDesc.oForceProcessDataDesc.bRequestStarted = EC_TRUE;

    /* wait while another request pending */
    do
    {
        OsSleep(1);

        /* check if another request is currently pending */
        if(m_oAtEcatDesc.oForceProcessDataDesc.bRequestStarted == EC_FALSE)
        {
            break;
        }
    } while(!oTimeout.IsElapsed());

    OsUnlock(m_oAtEcatDesc.poForceRelProcessDataDescLock);

    if (oTimeout.IsElapsed())
    {
        dwRetVal = EC_E_TIMEOUT;
    }
    else
    {
        dwRetVal = EC_E_NOERROR;
    }

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Release (unforce) Process Data

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::ReleaseProcessDataBits(
    EC_T_DWORD  dwClientId,
    EC_T_BOOL   bOutputData,    /**< [in]   EC_TRUE:Output data, EC_FALSE: Input data */
    EC_T_BOOL   bReleaseAll,    /**< [in]   EC_TRUE: Release All */
    EC_T_DWORD  dwBitOffsetPd,  /**< [in]   Data destination (process image) bit offset */
    EC_T_WORD   wBitLength,      /**< [in]   Data bit length */
    EC_T_DWORD  dwTimeout       /**< [in]   Timeout value */
)
{
EC_T_DWORD dwRetVal    = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NOWAIT == dwTimeout)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* Only one call can be processed at one time */
    OsLock(m_oAtEcatDesc.poForceRelProcessDataDescLock);

    /* setup copy request structure for job task */
    m_oAtEcatDesc.oForceProcessDataDesc.dwClientId      = dwClientId;
    m_oAtEcatDesc.oForceProcessDataDesc.bOutput         = bOutputData;
    m_oAtEcatDesc.oForceProcessDataDesc.bRelease        = EC_TRUE;
    m_oAtEcatDesc.oForceProcessDataDesc.bReleaseAll     = bReleaseAll;
    m_oAtEcatDesc.oForceProcessDataDesc.dwBitOffset     = dwBitOffsetPd;
    m_oAtEcatDesc.oForceProcessDataDesc.wBitLength      = wBitLength;

    /* Set the bRequestStarted-flag for the job task (CAtEmInterface::ExecJobPrivate) */
    m_oAtEcatDesc.oForceProcessDataDesc.bRequestStarted = EC_TRUE;
    for (; dwTimeout != 0; dwTimeout--)
    {
        OsSleep(1);

        /* check if another request is currently pending */
        if(m_oAtEcatDesc.oForceProcessDataDesc.bRequestStarted == EC_FALSE)
        {
            break;
        }
    }
    m_oAtEcatDesc.oForceProcessDataDesc.bRequestStarted = EC_FALSE;

    OsUnlock(m_oAtEcatDesc.poForceRelProcessDataDescLock);

    if (0 == dwTimeout)
    {
        dwRetVal = EC_E_TIMEOUT;
    }
    else
    {
        dwRetVal = EC_E_NOERROR;
    }

Exit:
    return dwRetVal;
}
#endif

/***************************************************************************************************/
/** \brief Enable EtherCAT jobs

\return N/A
*/
EC_T_VOID CAtEmInterface::EnableJobs(EC_T_VOID)
{
    m_oAtEcatDesc.bReqJobsInactive = EC_FALSE;
    m_oAtEcatDesc.bJobsActive      = EC_TRUE;
}

/***************************************************************************************************/
/** \brief Disable EtherCAT jobs

This function executes a single specific EtherCAT job.

\return status value
*/
EC_T_VOID CAtEmInterface::DisableJobs(EC_T_DWORD dwTimeout)
{
CEcTimer oTimeout;

    m_oAtEcatDesc.bReqJobsInactive = EC_TRUE;
    oTimeout.Start(dwTimeout);
    while (m_oAtEcatDesc.bJobsActive && !oTimeout.IsElapsed())
    {
        OsSleep(1);
    }
    /* ignore timeout because if job task is not running, it can not reset flags */
    m_oAtEcatDesc.bReqJobsInactive = EC_FALSE;
    m_oAtEcatDesc.bJobsActive      = EC_FALSE;
}

/***************************************************************************************************/
/** \brief Execute a single EtherCAT job

This function executes a single specific EtherCAT job.

\return status value
*/
EC_T_DWORD CAtEmInterface::ExecJobPrivate(
    EC_T_USER_JOB  EUserJob,    /* [in]  user requested job */
    EC_T_VOID*     pvParam,     /* [in]  optional parameter */
    EC_T_BOOL      bForceJob    /* [in]  force job without considering bJobsActive */
)
{
    EC_T_BYTE* pbyProcessData           = EC_NULL;
    EC_T_DWORD dwRetVal                 = EC_E_NOERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
/*      EC_ERRORMSGC(("ecatExecJob: Stack not initialized!\n")); */
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check if ADS adapter is running */
    if (AdsIsRunning())
    {
        dwRetVal = EC_E_ADS_IS_RUNNING;
        goto Exit;
    }
    /* check if jobs are allowed */
    if (!bForceJob)
    {
        /* check if jobs are allowed */
        if (!m_oAtEcatDesc.bJobsActive)
        {
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }
        /* reguest to disable jobs ? */
        if (m_oAtEcatDesc.bReqJobsInactive)
        {
            m_oAtEcatDesc.bReqJobsInactive = EC_FALSE;
            m_oAtEcatDesc.bJobsActive      = EC_FALSE;
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }
    }
    switch (EUserJob)
    {
    case eUsrJob_ProcessAllRxFrames:
        {
            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_eUsrJob_ProcessAllRxFrames);

            if (EcLinkMode_POLLING == m_oAtEcatDesc.eLinkMode)
            {
                /* call memory provider request */
                m_oAtEcatDesc.poMaster->GetPDInBasePointer();

#if (defined INCLUDE_RED_DEVICE)
                if (EC_NULL != m_oAtEcatDesc.poEcRedDevice)
                {
                    /* Rx Primary Sent frames */
                    dwRetVal = m_oAtEcatDesc.poEcRedDevice->HandleLinkPolling();
                }

                if (EC_NULL != m_oAtEcatDesc.poEcDevice)
                {
                    dwRetVal = m_oAtEcatDesc.poEcDevice->HandleLinkPolling();
                }
#else
                dwRetVal = m_oAtEcatDesc.poEcDevice->HandleLinkPolling();
#endif
                m_oAtEcatDesc.wNumSendCycFrames = 0;

                /* check link status */
                if (!m_oAtEcatDesc.poEcDevice->IsLinkConnected())
                {
                    dwRetVal = EC_E_LINK_DISCONNECTED;
                }

                /* this PD release is important in case of a cyclic frame loss */
                m_oAtEcatDesc.poMaster->ReturnPDInBasePointer();
            }

            if (EC_NULL != pvParam)
            {
                EC_T_BOOL bCycFramesAreProcessed = EC_FALSE;

                /* determine if all previously sent cyclic frames are processed */
                bCycFramesAreProcessed = m_oAtEcatDesc.poMaster->AreAllCycFramesProcessed();
                EC_SETDWORD(((EC_T_BOOL*)pvParam), bCycFramesAreProcessed);
            }

            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_eUsrJob_ProcessAllRxFrames);
        } break;

    case eUsrJob_SendAllCycFrames:
#if (defined INCLUDE_DC_SUPPORT)
    case eUsrJob_StampSendAllCycFrames:
#endif
        {
            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_eUsrJob_SendAllCycFrames);

#if (defined INCLUDE_DC_SUPPORT)
            EC_T_BOOL               bStampJob = EC_FALSE;
            if (eUsrJob_StampSendAllCycFrames == EUserJob)
            {
                bStampJob = EC_TRUE;
            }
#endif

            /* refresh link status */
            m_oAtEcatDesc.poEcDevice->RefreshLinkStatus();
            m_oAtEcatDesc.dwRefreshLinkStatusCnt++;

            if (m_oAtEcatDesc.poMaster->IsConfigured())
            {
                /* check if all used link layers are in interrupt mode and reset the m_oAtEcatDesc.dwNumSendCycFrames.*/
                if (m_oAtEcatDesc.eLinkMode == EcLinkMode_INTERRUPT)
                {
                    m_oAtEcatDesc.wNumSendCycFrames = 0;
                }
                if (eCycFrameLayout_IN_DMA == m_oAtEcatDesc.poMaster->GetCycFrameLayout())
                {
                    dwRetVal = m_oAtEcatDesc.poMaster->SendCycFramesInDma(&m_oAtEcatDesc.wNumSendCycFrames);
                }
                else
                {
                    dwRetVal = m_oAtEcatDesc.poMaster->SendCycFramesByTaskId(0xFFFFFFFF
#if (defined INCLUDE_DC_SUPPORT)
                        , bStampJob
#endif
                        , &m_oAtEcatDesc.wNumSendCycFrames);
                }
                if (m_oAtEcatDesc.wNumSendCycFrames == 0)
                {
                    m_oAtEcatDesc.poMaster->CycFrameReceivedCallback();
                }
                else
                {
                    if (!m_oAtEcatDesc.poEcDevice->IsMainLinkConnected()
#if (defined INCLUDE_RED_DEVICE)
                        && ((m_oAtEcatDesc.poEcRedDevice == EC_NULL) || ((m_oAtEcatDesc.poEcRedDevice != EC_NULL) && !m_oAtEcatDesc.poEcDevice->IsRedLinkConnected()))
#endif
                    )
                    {
                        m_oAtEcatDesc.poMaster->CycFrameReceivedCallback();
                    }
                }
            }
            else
            {
                /* callback function to inform application that all cyclic frame(s) are received */
                m_oAtEcatDesc.poMaster->CycFrameReceivedCallback();

                /* If master is not configured, eUsrJob_SendAllCycFrames and eUsrJob_StampSendAllCycFrames are invalid */
                dwRetVal = EC_E_INVALIDSTATE;
            }
            if (pvParam != EC_NULL)
            {
                EC_SETDWORD(((EC_T_DWORD*)pvParam), m_oAtEcatDesc.wNumSendCycFrames);
            }
            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_eUsrJob_SendAllCycFrames);
        } break;

    case eUsrJob_SendCycFramesByTaskId:
#if (defined INCLUDE_DC_SUPPORT)
    case eUsrJob_StampSendCycFramesByTaskId:
#endif
        {
            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_eUsrJob_SendCycFramesByTaskId);

#if (defined INCLUDE_DC_SUPPORT)
            EC_T_BOOL               bStampJob = EC_FALSE;
            if (eUsrJob_StampSendCycFramesByTaskId == EUserJob)
            {
                bStampJob = EC_TRUE;
            }
#endif
            if (EC_NULL != pvParam)
            {
                EC_T_DWORD dwTaskId;
                dwTaskId = EC_GETDWORD((EC_T_DWORD*)pvParam);

                if (m_oAtEcatDesc.poMaster->IsConfigured())
                {
                    dwRetVal = m_oAtEcatDesc.poMaster->SendCycFramesByTaskId(dwTaskId
#if (defined INCLUDE_DC_SUPPORT)
                        , bStampJob
#endif
                        , &m_oAtEcatDesc.wNumSendCycFrames);
                }
            }
            else
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_eUsrJob_SendCycFramesByTaskId);
        } break;

    case eUsrJob_RunMcSm:
        dwRetVal = EC_E_NOERROR;
        break;

    case eUsrJob_MasterTimer:
        {
            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_eUsrJob_RunMcSm);

            /* update msec counter for timer */
            m_oAtEcatDesc.poMaster->SetMsecCounter(OsQueryMsecCount());

            McStateMachine();
            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_eUsrJob_RunMcSm);

            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_eUsrJob_MasterTimer);

            /* trigger master timer */
            m_oAtEcatDesc.dwOnMasterTimerCnt++;

            dwRetVal = m_oAtEcatDesc.poMaster->OnMasterTimer();

            /* Check if GetProcessData (bRequestStarted == EC_TRUE) was called and copy the process data into the request struct */
            if (m_oAtEcatDesc.oGetProcessDataDesc.bRequestStarted)
            {
                /* Get the process data pointer */
                if (m_oAtEcatDesc.oGetProcessDataDesc.bOutputData)
                {
                    OsDbgAssert((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_OUT) * 8) >= (m_oAtEcatDesc.oGetProcessDataDesc.dwBitOffset + m_oAtEcatDesc.oGetProcessDataDesc.dwBitLength));
                    pbyProcessData = m_oAtEcatDesc.poMaster->GetPDOutBasePointer();
                }
                else
                {
                    OsDbgAssert((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_IN) * 8) >= (m_oAtEcatDesc.oGetProcessDataDesc.dwBitOffset + m_oAtEcatDesc.oGetProcessDataDesc.dwBitLength));
                    pbyProcessData = m_oAtEcatDesc.poMaster->GetPDInBasePointer();
                }

                if (pbyProcessData != EC_NULL)
                {
                    /* check if bit offset and  bit length is byte aligned. If this is the case ... */
                    if (((m_oAtEcatDesc.oGetProcessDataDesc.dwBitOffset % 8) == 0) && ((m_oAtEcatDesc.oGetProcessDataDesc.dwBitLength % 8) == 0))
                    {
                    EC_T_DWORD dwByteOffset =  m_oAtEcatDesc.oGetProcessDataDesc.dwBitOffset / 8;
                    EC_T_DWORD dwByteLength = (m_oAtEcatDesc.oGetProcessDataDesc.dwBitLength + 7) / 8;

                        /* ... we can use the ordinary (and fast) OsMemcpy. */
                        if (m_oAtEcatDesc.oGetProcessDataDesc.bOutputData == EC_TRUE)
                        {
                            OsMemcpyPdOut(m_oAtEcatDesc.oGetProcessDataDesc.pbyData, (EC_T_BYTE*)(pbyProcessData + dwByteOffset), dwByteLength);
                        }
                        else
                        {
                            OsMemcpyPdIn(m_oAtEcatDesc.oGetProcessDataDesc.pbyData, (EC_T_BYTE*)(pbyProcessData + dwByteOffset), dwByteLength);
                        }
                    }
                    else
                    {
                        /* otherwise we need the bit copy function */
                        if (m_oAtEcatDesc.oGetProcessDataDesc.bOutputData == EC_TRUE)
                        {
                            OsCopyBitsPdOut(m_oAtEcatDesc.oGetProcessDataDesc.pbyData,0,  pbyProcessData, (EC_T_WORD)m_oAtEcatDesc.oGetProcessDataDesc.dwBitOffset, (EC_T_WORD)m_oAtEcatDesc.oGetProcessDataDesc.dwBitLength);
                        }
                        else
                        {
                            OsCopyBitsPdIn(m_oAtEcatDesc.oGetProcessDataDesc.pbyData,0,  pbyProcessData, (EC_T_WORD)m_oAtEcatDesc.oGetProcessDataDesc.dwBitOffset, (EC_T_WORD)m_oAtEcatDesc.oGetProcessDataDesc.dwBitLength);
                        }
                    }

#if (defined INCLUDE_MEMORY_PROVIDER)
                    if (m_oAtEcatDesc.oGetProcessDataDesc.bOutputData == EC_TRUE)
                    {
                        m_oAtEcatDesc.poMaster->ReturnPDOutBasePointer();
                    }
                    else
                    {
                        m_oAtEcatDesc.poMaster->ReturnPDInBasePointer();
                    }
#endif
                }

                m_oAtEcatDesc.oGetProcessDataDesc.bRequestStarted = EC_FALSE;
            }

            /* Check if SetProcessData was called and copy the data into the process image */
            if (m_oAtEcatDesc.oSetProcessDataDesc.bRequestStarted)
            {
                /* get the process image pointer */
                OsDbgAssert((EC_T_DWORD)(m_oAtEcatDesc.poEcDevice->ProcessDataSize(VG_OUT) * 8) >= (m_oAtEcatDesc.oSetProcessDataDesc.dwBitOffset + m_oAtEcatDesc.oSetProcessDataDesc.dwBitLength));
                pbyProcessData = m_oAtEcatDesc.poMaster->GetPDOutBasePointer();

                /* copy the data into the process image */
                if (pbyProcessData != EC_NULL)
                {
                    /* check if bit offset and  bit length is byte aligned. If this is the case ... */
                    if (((m_oAtEcatDesc.oSetProcessDataDesc.dwBitOffset % 8) == 0) && ((m_oAtEcatDesc.oSetProcessDataDesc.dwBitLength % 8) == 0))
                    {
                    EC_T_DWORD dwByteOffset =  m_oAtEcatDesc.oSetProcessDataDesc.dwBitOffset / 8;
                    EC_T_DWORD dwByteLength = (m_oAtEcatDesc.oSetProcessDataDesc.dwBitLength + 7) / 8;

                        /*... we can use the ordinary (and fast) OsMemcpy ... */
                        OsMemcpyPdOut((EC_T_BYTE*)(pbyProcessData + dwByteOffset), m_oAtEcatDesc.oSetProcessDataDesc.pbyData, dwByteLength);
                    }
                    else
                    {
                        /* otherwise we need the bit copy function */
                        OsCopyBitsPdOut(pbyProcessData, (EC_T_WORD)m_oAtEcatDesc.oSetProcessDataDesc.dwBitOffset, m_oAtEcatDesc.oSetProcessDataDesc.pbyData, 0, (EC_T_WORD)m_oAtEcatDesc.oSetProcessDataDesc.dwBitLength);
                    }
#if (defined INCLUDE_MEMORY_PROVIDER)
                    m_oAtEcatDesc.poMaster->ReturnPDOutBasePointer();
#endif
                }

                m_oAtEcatDesc.oSetProcessDataDesc.bRequestStarted = EC_FALSE;
            }

#if (defined INCLUDE_FORCE_PROCESSDATA)
            /* force/release process data */
            if (m_oAtEcatDesc.oForceProcessDataDesc.bRequestStarted)
            {
               m_oAtEcatDesc.poMaster->ManageForceData(&m_oAtEcatDesc.oForceProcessDataDesc);

               m_oAtEcatDesc.oForceProcessDataDesc.bRequestStarted = EC_FALSE;
            }
#endif

            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_eUsrJob_MasterTimer);
        } break;

    case eUsrJob_FlushQueuedCmds:
        dwRetVal = EC_E_NOERROR;
        break;

    case eUsrJob_SendAcycFrames:
        {
            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_eUsrJob_SendAcycFrames);

            /* RefreshLinkStatus was not called in the job SendCycFrames, so we call it here */
            if (m_oAtEcatDesc.dwPrevRefreshLinkStatusCnt == m_oAtEcatDesc.dwRefreshLinkStatusCnt)
            {
                /* refresh link status */
                m_oAtEcatDesc.poEcDevice->RefreshLinkStatus();
            }

            /* set the old refresh counter to remember the next cycle */
            m_oAtEcatDesc.dwPrevRefreshLinkStatusCnt = m_oAtEcatDesc.dwRefreshLinkStatusCnt;

            dwRetVal = m_oAtEcatDesc.poMaster->SendAcycFrames(&m_oAtEcatDesc.wNumSendAcycFrames);

            if (pvParam != EC_NULL)
            {
                EC_SETDWORD(((EC_T_DWORD*)pvParam), m_oAtEcatDesc.wNumSendAcycFrames);
            }
            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_eUsrJob_SendAcycFrames);
    } break;

    case eUsrJob_MasterTimerMinimal:
        {
            /* update msec counter for timeout helper */
            m_oAtEcatDesc.poMaster->SetMsecCounter(OsQueryMsecCount());
            m_oAtEcatDesc.dwOnMasterTimerCnt++;
        } break;
    default:
        dwRetVal = EC_E_INVALIDPARM;
        break;
    }
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);

    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Internal clock helper if user controlled mode is used.

This function runs a clock tick for user controlled mode when the user is not able to
run jobs (e.g. when master is shut down).

\return N/A
*/
EC_T_VOID CAtEmInterface::UsrCtlClkHelper(EC_T_VOID)
{
    ExecJobPrivate(eUsrJob_ProcessAllRxFrames,    EC_NULL, EC_TRUE);
    ExecJobPrivate(eUsrJob_MasterTimer,           EC_NULL, EC_TRUE);
    ExecJobPrivate(eUsrJob_SendAcycFrames,        EC_NULL, EC_TRUE);
}

/***************************************************************************************************/
/** \brief Get the current master stack state

  This function determines the current master stack state and converts it to the EC_T_STATE type.

  a) no transition:
     => returns the current state
  b) transition from a lower to a higher state:
     => returns the lower state
  c) transition from a higher to a lower state:
     => returns the higher state

    \return Master current state
*/
EC_T_STATE CAtEmInterface::GetMasterState(EC_T_VOID)
{
    EC_T_WORD  wMasterState = 0;
    EC_T_STATE eMasterState = eEcatState_UNKNOWN;

    /* get master state */
    if ((m_oAtEcatDesc.poMaster != EC_NULL) && (m_oAtEcatDesc.bInitialized))
    {
        wMasterState = m_oAtEcatDesc.poMaster->GetMasterDeviceState();

        /* convert state */
        switch (wMasterState)
        {
        case DEVICE_STATE_UNKNOWN: eMasterState = eEcatState_UNKNOWN; break;
        case DEVICE_STATE_INIT:    eMasterState = eEcatState_INIT;    break;
        case DEVICE_STATE_PREOP:   eMasterState = eEcatState_PREOP;   break;
        case DEVICE_STATE_SAFEOP:  eMasterState = eEcatState_SAFEOP;  break;
        case DEVICE_STATE_OP:      eMasterState = eEcatState_OP;      break;
        default: OsDbgAssert(EC_FALSE); break;
        }
    }
    return eMasterState;
}

/***************************************************************************************************/
/** \brief Register a new client for the process data and notification

  This function register a new client for the process data and the notifications. This function
  create a new client descriptor and place it in the linked list. The parameters are given through a
  EC_T_REGISTERPARMS structure with following members:
  pCallerData         A pointer to a specific caller data block; this data block will be used in
  all callback functions.
  pfnNotify           Notify callback function pointer; this function pointer is used to notify
  the registered client about specific events (see EC_PF_NOTIFY)

  The result of the call are placed in a EC_T_REGISTERRESULTS structure with following members:
  dwClntId            Client ID; used to identify the new registered client
  pbyPDIn             Incoming process data shared memory
  dwPDInSize          Size of the incoming process data area
  pbyPDOut            Outgoing process data shared memory
  dwPDOutSize         Size of the outgoing process data area

    \return Depends on the implementation of the function OsSetLastError(),
    normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::RegisterClient(
    EC_T_REGISTERPARMS*     pParms,             /**< [in] Register parameters */
    EC_T_REGISTERRESULTS*   pResults            /**< [in] Register results */
)
{
    EC_T_CLNTDESC*  pClntDesc   = EC_NULL;
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    struct _EC_T_CLNTDESC* pNextClntDesc;
    struct _EC_T_CLNTDESC* pLastClntDesc;
    EC_T_BOOL bLocked = EC_FALSE;
    EC_T_DWORD      dwWaitCount = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("RegisterClient: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if ((EC_NULL == pParms->pfnNotify) && (EC_NULL == pResults))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* create new client descriptor */
    pClntDesc = (EC_T_CLNTDESC*)OsMalloc(sizeof(EC_T_CLNTDESC));
    EC_TRACE_ADDMEM(EC_MEMTRACE_INTERFACE, "ecatRegisterClient", pClntDesc, sizeof(EC_T_CLNTDESC));
    if (pClntDesc == EC_NULL)
    {
        goto Exit;
    }

    OsMemset(pClntDesc, 0, sizeof(EC_T_CLNTDESC));

    pClntDesc->dwNotificationMask = NOTIFICATION_MASK_DEFAULT;
    if (EC_NULL != pParms)
    {
        pClntDesc->pCallerData = pParms->pCallerData;
        pClntDesc->S_pfnNotify = pParms->pfnNotify;
    }
    pClntDesc->pNext       = EC_NULL;

    /* get process data pointers */
    m_oAtEcatDesc.poMaster->GetPDOut((EC_T_BYTE**)&pResults->pbyPDOut, (EC_T_DWORD*)&pResults->dwPDOutSize);
    m_oAtEcatDesc.poMaster->GetPDIn( (EC_T_BYTE**)&pResults->pbyPDIn,  (EC_T_DWORD*)&pResults->dwPDInSize);

    /* synchronize descriptor access */
    OsLock(m_oAtEcatDesc.poClntDescLock);
    while(0 < m_oAtEcatDesc.dwClntDescInUse)
    {
        OsUnlock(m_oAtEcatDesc.poClntDescLock);
        if(dwWaitCount == 1000)
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }
        OsSleep(1);
        dwWaitCount++;
        OsLock(m_oAtEcatDesc.poClntDescLock);
    }
    bLocked = EC_TRUE;
    if (m_oAtEcatDesc.byNumRegisteredClnts == EC_MAX_CLIENTS)
    {
        /* maximum number of clients exceeded */
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    m_oAtEcatDesc.byNumRegisteredClnts++;

    pClntDesc->byUniqueClntId = m_oAtEcatDesc.byNextUniqueClntId;
    pClntDesc->dwId        = pClntDesc->byUniqueClntId;
    pResults->dwClntId     = pClntDesc->byUniqueClntId;

    /* determine next client ID */
    {
    EC_T_CLNTDESC* pClntDescChk = EC_NULL;

        do
        {
            /* decrement ID */
            m_oAtEcatDesc.byNextUniqueClntId--;
            if (0 == m_oAtEcatDesc.byNextUniqueClntId)
            {
                m_oAtEcatDesc.byNextUniqueClntId = EC_MAX_CLIENTS;
            }
            /* check if already in use */
            for (pClntDescChk = m_oAtEcatDesc.pClntDescHead; pClntDescChk != EC_NULL; pClntDescChk = pClntDescChk->pNext)
            {
                if (pClntDescChk->byUniqueClntId == m_oAtEcatDesc.byNextUniqueClntId)
                    break;
            }
        } while (EC_NULL != pClntDescChk);
    }
    /* add to client list */
    if (m_oAtEcatDesc.pClntDescHead == EC_NULL )
    {
        m_oAtEcatDesc.pClntDescHead = pClntDesc;
    }
    else
    {
        /* search client descriptor tail */
        pLastClntDesc = m_oAtEcatDesc.pClntDescHead;
        pNextClntDesc = pLastClntDesc->pNext;
        while( pNextClntDesc != EC_NULL )
        {
            pLastClntDesc = pNextClntDesc;
            pNextClntDesc = pLastClntDesc->pNext;
        }
        /* add new client at tail */
        pLastClntDesc->pNext = pClntDesc;
    }

    /* no errors */
    dwRetVal = EC_E_NOERROR;
    OsDbgAssert(0 != pResults->dwClntId);

Exit:
    /* synchronize call */
    if (bLocked)
    {
        OsUnlock(m_oAtEcatDesc.poClntDescLock);
        bLocked = EC_FALSE;
    }
    if (EC_E_NOERROR != dwRetVal)
    {
        /* release resources */
        if (pClntDesc != EC_NULL )
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "RegisterClient:pClntDesc", pClntDesc, sizeof(EC_T_CLNTDESC));
            OsFree( pClntDesc );
        }
    }

    return OsSetLastError(dwRetVal);
}


/***************************************************************************************************/
/** \brief Un-register a client

  This function unregister a client which has been registered previously calling the function
  RegisterClient(). The client descriptor is taken out of the linked list. The client is identified
  using the ID returned previously by the function RegisterClient().

    \return N/A
*/
EC_T_DWORD CAtEmInterface::UnregisterClient(EC_T_DWORD  dwClntId /**< [in] Client ID */)
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    EC_T_CLNTDESC*  pClntDesc = EC_NULL;
    EC_T_CLNTDESC*  pPrevClntDesc = EC_NULL;
    EC_T_DWORD      dwWaitCount = 0;
    EC_T_BOOL       bLocked = EC_FALSE;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("UnregisterClient: Stack not initialized!\n"));
        return EC_E_INVALIDSTATE;
    }
#if (defined INCLUDE_FORCE_PROCESSDATA)
    /* release process data bits outside client lock */
    ReleaseProcessDataBits(dwClntId, EC_FALSE, EC_TRUE, 0, 0, 1000);
#endif
    /* synchronize call */
    OsLock(m_oAtEcatDesc.poClntDescLock);
    bLocked = EC_TRUE;
    while (0 < m_oAtEcatDesc.dwClntDescInUse)
    {
        OsUnlock(m_oAtEcatDesc.poClntDescLock);
        bLocked = EC_FALSE;
        if (dwWaitCount == 1000)
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }
        OsSleep(1);
        dwWaitCount++;
        OsLock(m_oAtEcatDesc.poClntDescLock);
        bLocked = EC_TRUE;
    }
    /* search client descriptor */
    for (pClntDesc = m_oAtEcatDesc.pClntDescHead; pClntDesc != EC_NULL; pClntDesc = pClntDesc->pNext)
    {
        if (pClntDesc->byUniqueClntId == dwClntId) break;
        pPrevClntDesc = pClntDesc;
    }
    if (EC_NULL == pClntDesc)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }

    /* remove from linked list  */
    m_oAtEcatDesc.byNumRegisteredClnts--;
    OsDbgAssert( m_oAtEcatDesc.byNumRegisteredClnts != 0xFF );

    if (EC_NULL == pPrevClntDesc)
    {
        /* all clients are unregistered now */
        m_oAtEcatDesc.pClntDescHead = pClntDesc->pNext;
    }
    else
    {
        /* remove the client currently unregistered from linked list */
        pPrevClntDesc->pNext = pClntDesc->pNext;
    }

    /* free memory */
    EC_TRACE_SUBMEM(EC_MEMTRACE_INTERFACE, "UnregisterClient:pClntDesc", pClntDesc, sizeof(EC_T_CLNTDESC));
    OsFree(pClntDesc);

    dwRetVal = EC_E_NOERROR;
Exit:
    /* synchronize call */
    if (bLocked)
    {
        OsUnlock(m_oAtEcatDesc.poClntDescLock);
    }

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Block invalid node from bus.

Blocks node to avoid influence to ScanBus.
\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CAtEmInterface::BlockNode(
    EC_T_SB_MISMATCH_DESC*  pMisMatch   /**< [in]   Mismatch descriptor from EC_NOTIFY_SB_MISMATCH */
   ,EC_T_DWORD              dwTimeout   /**< [in]   Timeout value for operation */
)
{
#if (defined INCLUDE_PORT_OPERATION)
    EC_T_DWORD       dwRetVal    = EC_E_ERROR;
    EC_T_DWORD       dwRes       = EC_E_ERROR;
    CEcSlave*        pCfgSlave   = EC_NULL;
    CEcBusSlave*     pBusSlave   = EC_NULL;
    EC_T_WORD        wPrevPort   = pMisMatch->wPrevPort;
    EC_T_DWORD       dwSlaveIdx  = 0;
    EC_T_MCSM_ORDER* poMcSmOrder = EC_NULL;
    EC_T_DWORD       dwOrderId   = 0;
    EC_T_HOTCONNECT_GROUP* pHcGroup = EC_NULL;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("BlockNode: Stack not initialized!\n"));
        return EC_E_INVALIDSTATE;
    }
    /* check parameters */
    if ((EC_NULL == pMisMatch))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (ESC_PORT_INVALID == pMisMatch->wPrevPort)
    {
        /* cut-off not possible */
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }
    if (0 == pMisMatch->wPrevFixedAddress)
    {
        /* bus root cut-off not possible */
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }
    /* get config slave */
    pCfgSlave = m_oAtEcatDesc.poMaster->GetSlaveByCfgFixedAddr(pMisMatch->wPrevFixedAddress);
    if (EC_NULL == pCfgSlave)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get bus slave */
    pBusSlave = pCfgSlave->m_pBusSlave;
    if (EC_NULL == pBusSlave)
    {
        /* bus root cut-off not possible (this can happen in case of redundancy) */
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }
    if (pBusSlave->IsLineCrossed())
    {
        /* block node not supported on line crossed slave */
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }
    /* get HC group */
    pHcGroup = m_oAtEcatDesc.poMaster->m_oHotConnect.GetGroup(pCfgSlave->m_dwHCGroupId);
    if (EC_NULL == pHcGroup)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* handle HC group */
    for (dwSlaveIdx = 0; dwSlaveIdx < pHcGroup->dwGrpMembers; dwSlaveIdx++)
    {
        if (!pHcGroup->ppGroupMembers[dwSlaveIdx]->IsPresentOnBus())
        {
            break;
        }
    }
    if (dwSlaveIdx < pHcGroup->dwGrpMembers)
    {
        /* incomplete HC group, try to block the complete HC group */
        if (EC_NULL != pHcGroup->ppGroupMembers[0]->m_pBusSlave)
        {
            wPrevPort = pHcGroup->ppGroupMembers[0]->m_pBusSlave->GetPortAtParent();
            pBusSlave = pHcGroup->ppGroupMembers[0]->m_pBusSlave->GetParentSlave();
        }
    }
    if (EC_NULL == pBusSlave)
    {
        /* block node is impossible on first slave */
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }
    dwRes = m_oAtEcatDesc.poMaster->m_oHotConnect.QueueChangePortRequest(
        pBusSlave->GetFixedAddress(),
        wPrevPort,
        m_oAtEcatDesc.poMaster->m_oHotConnect.CreatePortChangeFlags(EC_TRUE, EC_TRUE, PORTCHG_SOURCE_BLOCKNODE));
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    dwRes = McSmOrderAdd(eMcSmOrderType_PORTOPERATION, &poMcSmOrder, (EC_NOWAIT == dwTimeout), ((EC_NOWAIT == dwTimeout) ? m_oAtEcatDesc.poMaster->GetScanBusDefaultTimeout() : dwTimeout));
    if ((EC_E_NOERROR != dwRes) || (EC_NOWAIT == dwTimeout))
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    dwOrderId = poMcSmOrder->dwOrderId;

    /* wait until the order is finished or timeout */
    for(;;)
    {
        /* sleep a while */
        OsSleep(1);
        if (eEcatOrderState_Finished == poMcSmOrder->eOrderState)
        {
            if (dwOrderId != poMcSmOrder->dwOrderId)
            {
                /* order was overwritten, should never happen */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }
            /* requested state is reached */
            dwRetVal = poMcSmOrder->dwMcSmLastErrorStatus;
            if (EC_E_NOERROR != dwRetVal)
            {
                EC_DBGMSG("ecatBlockNode() Error 0x%x in McSm state '%s' for requested state '%s'\n",
                    dwRetVal, EcMcSmStateString(poMcSmOrder->EMcSmErrorState), EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                goto Exit;
            }
            dwRetVal = EC_E_NOERROR;
            break;
        }
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;
Exit:
    /* release order if needed */
    if ((EC_NULL != poMcSmOrder) && (EC_NOWAIT != dwTimeout))
    {
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    return dwRetVal;
#else
    EC_UNREFPARM(pMisMatch);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif /* INCLUDE_PORT_OPERATION */
}

/***************************************************************************************************/
/**
\brief  Block invalid node from bus.

Blocks node to avoid influence to ScanBus.
\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CAtEmInterface::OpenBlockedPorts(
    EC_T_DWORD              dwTimeout   /**< [in]   Timeout value for operation */
)
{
#if (defined INCLUDE_PORT_OPERATION)
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;
    EC_T_MCSM_ORDER*    poMcSmOrder     = EC_NULL;
    EC_T_DWORD          dwOrderId       = 0;
    EC_T_DWORD          dwCountPorts    = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("OpenBlockedPorts: Stack not initialized!\n"));
        return EC_E_INVALIDSTATE;
    }
    if (EC_NOWAIT == dwTimeout)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwCountPorts = m_oAtEcatDesc.poMaster->m_oHotConnect.QueuePortsBySource(PORTCHG_SOURCE_BLOCKNODE);

    if (dwCountPorts != m_oAtEcatDesc.poMaster->m_oHotConnect.CountClosedPortsBySource(PORTCHG_SOURCE_BLOCKNODE))
    {
        dwRetVal = EC_E_PORTOPEN;
        goto Exit;
    }

    if (0 == dwCountPorts )
    {
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }

    dwRetVal = McSmOrderAdd(eMcSmOrderType_PORTOPERATION, &poMcSmOrder, (EC_NOWAIT == dwTimeout), ((EC_NOWAIT == dwTimeout) ? m_oAtEcatDesc.poMaster->GetScanBusDefaultTimeout() : dwTimeout));
    if ((EC_E_NOERROR != dwRetVal) || (EC_NOWAIT == dwTimeout))
    {
        goto Exit;
    }
    dwOrderId = poMcSmOrder->dwOrderId;

    for(;;)
    {
        /* sleep a while */
        OsSleep(1);
        if (eEcatOrderState_Finished == poMcSmOrder->eOrderState)
        {
            if (dwOrderId != poMcSmOrder->dwOrderId)
            {
                /* order was overwritten, should never happen */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }
            /* requested state is reached */
            dwRetVal = poMcSmOrder->dwMcSmLastErrorStatus;
            if (EC_E_NOERROR != dwRetVal)
            {
                EC_DBGMSG("ecatOpenBlockedPorts() Error 0x%x in McSm state '%s' for requested state '%s'\n",
                    dwRetVal, EcMcSmStateString(poMcSmOrder->EMcSmErrorState), EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                goto Exit;
            }
            dwRetVal = EC_E_NOERROR;
            break;
        }
    } /* wait until the order was finished */

    if (0 != m_oAtEcatDesc.poMaster->m_oHotConnect.CountClosedPortsBySource(PORTCHG_SOURCE_BLOCKNODE))
    {
        dwRetVal = EC_E_PORTOPEN;
        goto Exit;
    }
Exit:
    /* release order if needed */
    if ((EC_NULL != poMcSmOrder) && (EC_NOWAIT != dwTimeout))
    {
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    return dwRetVal;
#else
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif /* INCLUDE_PORT_OPERATION */
}

/***************************************************************************************************/
/**
\brief  Force trigger topology Change (HC TopoChange State Machine).

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::ForceTopologyChange(EC_T_VOID)
{
    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ForceTopologyChange: Stack not initialized!\n"));
        return EC_E_INVALIDSTATE;
    }
    m_oAtEcatDesc.poMaster->m_pBusTopology->SetTopologyChanged();

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Returns whether a specific slave is currently present on bus.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::IsSlavePresent(
    EC_T_DWORD      dwSlaveId       /**< [in]   Slave Id */
   ,EC_T_BOOL*      pbPresence      /**< [out]  Presence state of slave on bus */
                                         )
{
EC_T_DWORD dwRetVal  = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("IsSlavePresent: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("IsSlavePresent: Invalid slave ID!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (EC_NULL == pbPresence)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* try to get values first from cfg slave, then from bus slave */
    {
    CEcSlave* pCfgSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);

        if (EC_NULL != pCfgSlave)
        {
            EC_SETDWORD(pbPresence, (pCfgSlave->IsPresentOnBus()?EC_TRUE:EC_FALSE));
        }
        else
        {
        CEcBusSlave* pBusSlave = m_oAtEcatDesc.poMaster->GetBusSlaveBySlaveId(dwSlaveId);

            if (EC_NULL == pBusSlave)
            {
                dwRetVal = EC_E_NOTFOUND;
                goto Exit;
            }
            EC_SETDWORD(pbPresence, EC_TRUE);
        }
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

EC_T_DWORD CAtEmInterface::IsTopologyChangeDetected(
    EC_T_BOOL*      pbIsTopologyChangeDetected  /**< [out]  Topology Change Detected */
                                         )
{
EC_T_DWORD dwRetVal  = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("IsTopologyChangeDetected: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pbIsTopologyChangeDetected)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    EC_SETBOOL(pbIsTopologyChangeDetected, m_oAtEcatDesc.poMaster->TopologyChangedDetected());

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief Waits until master control state machine is stable or exits by timeout.

\return EC_E_NOERROR or EC_E_TIMEOUT.
*/
EC_T_DWORD CAtEmInterface::WaitUntilStableMcSm(EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal=EC_E_ERROR;
    CEcTimer oTimeout;

    oTimeout.Start(dwTimeout);

    while ((m_oAtEcatDesc.EMcSmCurrState != m_oAtEcatDesc.EMcSmReqState) // McSm is not stable
        || !IsListEmpty(&m_oAtEcatDesc.oPendingMcSmOrderList)            // McSm orders are still pending
        || (0 != m_oAtEcatDesc.pNewPendingMcSmOrderFifo->GetCount())     // McSm orders are still pending
        )
    {
        if (oTimeout.IsElapsed())
        {
            dwRetVal=EC_E_TIMEOUT;
            goto Exit;
        }
        if (!m_oAtEcatDesc.bJobsActive)
        {
            UsrCtlClkHelper();
        }
        OsSleep(1);
    }
    /* no errors */
    dwRetVal=EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief Sends Restart command to the master.

\return EC_E_NOERROR or error code.
*/
EC_T_DWORD CAtEmInterface::RestartMasterForPts()
{
    m_dwInternalIOControlCode = EC_IOCTL_SB_RESTART;
    EC_T_DWORD dwRetVal = McSmOrderAdd(eMcSmOrderType_REQ_SB, EC_NULL, EC_TRUE, EC_WAITINFINITE);
    m_dwInternalIOControlCode = 0;

    return dwRetVal;
}

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)


/***************************************************************************************************/
/**
\brief  Gets the status of the Pass-through server.

\return Pass-through server status
*/
EC_PTS_STATE CAtEmInterface::PassThroughSrvGetStatus()
{
    EC_PTS_STATE eCurPtsState = ePtsStateNone;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        goto Exit;
    }

    eCurPtsState = m_oAtEcatDesc.poMaster->PassThroughSrvGetStatus();

Exit:
    return eCurPtsState;
}

/***************************************************************************************************/
/**
\brief Enables the Pass-through server.

\return EC_E_NOERROR or error code.
*/
EC_T_DWORD CAtEmInterface::PassThroughSrvEnable(EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("PassThroughSrvEnable: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRes = WaitUntilStableMcSm(dwTimeout);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRes = m_oAtEcatDesc.poMaster->PassThroughSrvEnable(dwTimeout);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}



/***************************************************************************************************/
/**
\brief Disables the Pass-through server.

\return EC_E_NOERROR or error code.
*/
EC_T_DWORD CAtEmInterface::PassThroughSrvDisable(EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("PassThroughSrvDisable: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRetVal = RestartMasterForPts();
    if(dwRetVal == EC_E_NOERROR)
    {
        dwRetVal = m_oAtEcatDesc.poMaster->PassThroughSrvDisable(dwTimeout);
    }

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief Starts the Pass Through Server

\return EC_E_NOERROR or error code.
*/
EC_T_DWORD CAtEmInterface::PassThroughSrvStart(EC_T_PTS_SRV_START_PARMS* poPtsStartParams, EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    if(poPtsStartParams == EC_NULL)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("PassThroughSrvStart: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

     dwRetVal = m_oAtEcatDesc.poMaster->PassThroughSrvStart(poPtsStartParams, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief Stops the Pass Through Server

\return EC_E_NOERROR or error code.
*/
EC_T_DWORD CAtEmInterface::PassThroughSrvStop(EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("PassThroughSrvStop: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRetVal =  PassThroughSrvDisable(dwTimeout);
    if(dwRetVal == EC_E_NOERROR)
    {
        dwRetVal = m_oAtEcatDesc.poMaster->PassThroughSrvStop(dwTimeout);
    }

Exit:
    return dwRetVal;
}

#endif /* #if (defined INCLUDE_PASS_THROUGH_SERVER )*/

#if (defined INCLUDE_ADS_ADAPTER) && (defined EC_SOCKET_IP_SUPPORTED)
/***************************************************************************************************/
/**
\brief Starts the ADS Pass Through Server

\return EC_E_NOERROR or error code.
*/
EC_T_DWORD CAtEmInterface::AdsAdapterStart(EC_T_ADS_ADAPTER_START_PARMS* poStartParams,
                                                  EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes = EC_E_ERROR;
    EC_T_ADS_ADAPTER_PARMS AdsParams;

    if ((EC_NULL == poStartParams) || (poStartParams->dwSize < sizeof(EC_T_ADS_ADAPTER_START_PARMS)))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("AdsAdapterStart: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check if ADS adapter is running */
    if (AdsIsRunning())
    {
        /* protect from double run */
        dwRetVal = EC_E_ADS_IS_RUNNING;
        goto Exit;
    }

    dwRes = WaitUntilStableMcSm(dwTimeout);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    /* prepare ADS params */
    AdsParams.m_poPtsParams = poStartParams;
    AdsParams.m_pLinkLayer =
        &(m_oAtEcatDesc.poMaster->EthDevice()->m_LinkDrvDesc);

    dwRes = ::AdsAdapterStart(&AdsParams, &m_pAdsSrvHandle);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief Stops the ADS Pass Through Server

\return EC_E_NOERROR or error code.
*/
EC_T_DWORD CAtEmInterface::AdsAdapterStop(EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("AdsAdapterStop: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRetVal = RestartMasterForPts();
    if(dwRetVal == EC_E_NOERROR)
    {
        ::AdsAdapterStop(&m_pAdsSrvHandle);
        m_pAdsSrvHandle = EC_NULL;
    }

    EC_UNREFPARM(dwTimeout);

Exit:
    return dwRetVal;
}
#endif /* #if (defined INCLUDE_ADS_ADAPTER) && (defined EC_SOCKET_IP_SUPPORTED) */

/***************************************************************************************************/
/** \brief Notify all the registered clients about a specific event

  This function call the notify callback function of all registered client.

    \return number of clients that were notified.
*/
EC_T_DWORD CAtEmInterface::NotifyAllClients(
                                      EC_T_DWORD          dwCode,                 /**< [in] Notify code */
                                      EC_T_DWORD          dwClientID,             /**< [in] Notify a specific client */
                                      EC_T_NOTIFYPARMS*   pParms                  /**< [in/out] Data transferred */
                                      )
{
    EC_T_DWORD          dwNumNotify = 0;
    EC_T_CLNTDESC*      pClntDesc = m_oAtEcatDesc.pClntDescHead;
    EC_T_DWORD          dwNotificationMask = NotificationCodeToNotificationMask(dwCode);
    EC_T_BOOL           bNotificationMaskable = (0 != dwNotificationMask);
    EC_T_BOOL           bNotificationEnabled = EC_FALSE;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("NotifyAllClients: Stack not initialized!\n"));
        return 0;
    }

    /* synchronize with register/unregister client API */
    OsLock(m_oAtEcatDesc.poClntDescLock);
    m_oAtEcatDesc.dwClntDescInUse++;
    OsUnlock(m_oAtEcatDesc.poClntDescLock);

    for ( pClntDesc = m_oAtEcatDesc.pClntDescHead; (pClntDesc != EC_NULL) && (pClntDesc->S_pfnNotify != EC_NULL); pClntDesc = pClntDesc->pNext)
    {
        if ((0 == dwClientID) || (pClntDesc->byUniqueClntId == (dwClientID&(~IS_CLIENT_RAW_CMD))))
        {
            /* change client caller data */
            pParms->pCallerData = pClntDesc->pCallerData;
            bNotificationEnabled = (0 != (pClntDesc->dwNotificationMask & dwNotificationMask));

            /* notify client */
            if (!bNotificationMaskable || bNotificationEnabled)
            {
                pClntDesc->S_pfnNotify(dwCode, pParms);
                dwNumNotify++;
            }
        }
    }

    if (!m_oAtEcatDesc.bJobsActive && (dwNumNotify == 0))
    {
        /* we are shutting down, set number of notified clients to 1 to avoid error messages/assertions */
        dwNumNotify++;
    }
    OsLock(m_oAtEcatDesc.poClntDescLock);
    m_oAtEcatDesc.dwClntDescInUse--;
    OsUnlock(m_oAtEcatDesc.poClntDescLock);

    return dwNumNotify;
}


/***************************************************************************************************/
/** \brief Wrapper function to C++ NotifyAllClients routine

    \return number of clients that were notified.
*/
EC_T_DWORD CAtEmInterface::WrapperToNotifyAllClients(
                                    EC_T_DWORD          dwInstanceID,
                                    EC_T_DWORD          dwClientID,
                                    EC_T_DWORD          dwCode,                 /**< [in]     Notify code */
                                    EC_T_NOTIFYPARMS*   pParms                  /**< [in/out] Data transferred */
                                    )
{
CAtEmInterface* pEmInst = S_aMultiMaster[dwInstanceID].pInst;

    OsDbgAssert(dwInstanceID < MAX_NUMOF_MASTER_INSTANCES);

    if (EC_NULL == pEmInst)
    {
        /* if called wihtin DeinitMaster() */
        return 0;
    }
    return pEmInst->NotifyAllClients(dwCode, dwClientID, pParms);
}


/***************************************************************************************************/
/** \brief Determine the number of slaves which are configured in the XML file.

  \return number of slaves
*/
EC_T_DWORD CAtEmInterface::GetNumConfiguredSlaves(EC_T_VOID)
{
    EC_T_DWORD dwSlaveCnt = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatGetNumConfiguredSlaves: Stack not initialized!\n"));
        goto Exit;
    }

    dwSlaveCnt = m_oAtEcatDesc.poMaster->GetCfgSlaveCount();
Exit:
    return dwSlaveCnt;
}

/***************************************************************************************************/
/** \brief Determine slave id according to its station address

  \return slave id or INVALID_SLAVE_ID if the slave could not be found
*/
EC_T_DWORD CAtEmInterface::GetSlaveId
(   EC_T_WORD wStationAddress )     /**< [in]  slave node number (physical address) */
{
EC_T_DWORD dwSlaveId = (EC_T_DWORD)INVALID_SLAVE_ID;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatGetSlaveId: Stack not initialized!\n"));
        goto Exit;
    }

    dwSlaveId = m_oAtEcatDesc.poMaster->GetSlaveIdByFixedAddress(wStationAddress);

Exit:
    return dwSlaveId;
}

/***************************************************************************************************/
/** \brief Determine slave fixed address according to its slave id.

  \return EC_E_NOERROR on success or EC_E_INVALIDPARM; EC_E_NOTFOUND (Slave not found)
*/
EC_T_DWORD  CAtEmInterface::GetSlaveFixedAddr(
    EC_T_DWORD              dwSlaveId,           /**< [in]  Slave id */
    EC_T_WORD*              pwFixedAddr          /**< [in]  Corresponding fixed address */
)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatGetSlaveFixedAddr: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("ecatGetSlaveFixedAddr: Invalid slave ID!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (EC_NULL == pwFixedAddr)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* try to get values first from cfg slave, then from bus slave */
    {
    CEcSlave* pCfgSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);

        if (EC_NULL != pCfgSlave)
        {
            (*pwFixedAddr) = pCfgSlave->GetCfgFixedAddr();
        }
        else
        {
        CEcBusSlave* pBusSlave = m_oAtEcatDesc.poMaster->GetBusSlaveBySlaveId(dwSlaveId);

            if (EC_NULL == pBusSlave)
            {
                dwRetVal = EC_E_NOTFOUND;
                goto Exit;
            }
            (*pwFixedAddr) = pBusSlave->GetFixedAddress();
        }
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}




/***************************************************************************************************/
/** \brief Determine slave id according to its station address

\return slave id or INVALID_SLAVE_ID if the slave could not be found
*/
EC_T_DWORD CAtEmInterface::GetSlaveIdAtPosition
(   EC_T_WORD wAutoIncAddress )     /**< [in]  slave auto increment address (set in the XML configuration file) */
{
EC_T_DWORD dwSlaveId = (EC_T_DWORD)INVALID_SLAVE_ID;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatGetSlaveIdAtPosition: Stack not initialized!\n"));
        goto Exit;
    }

    if (m_oAtEcatDesc.poMaster->IsConfigured())
    {
    CEcSlave* pCfgSlave = m_oAtEcatDesc.poMaster->GetSlaveByCfgAutoIncAddr(wAutoIncAddress);

        if (EC_NULL == pCfgSlave)
        {
            goto Exit;
        }
        dwSlaveId = pCfgSlave->GetSlaveId();
    }
    else
    {
    CEcBusSlave* pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wAutoIncAddress);

        if (EC_NULL == pBusSlave)
        {
            goto Exit;
        }
        dwSlaveId = pBusSlave->GetBusIndex();
    }

Exit:
    return dwSlaveId;
}


/***************************************************************************************************/
/** \brief Get slave properties

\return EC_TRUE if the slave exists, EC_FALSE if the slave id is invalid
*/
EC_T_BOOL CAtEmInterface::GetSlaveProp
(   EC_T_DWORD          dwSlaveId,      /**< [in]  slave id */
    EC_T_SLAVE_PROP*    pSlaveProp )    /**< [out] slave properties */
{
EC_T_BOOL bRetVal = EC_FALSE;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("GetSlaveProp: Stack not initialized!\n"));
        goto Exit;
    }
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("GetSlaveProp: Invalid slave ID!\n"));
        goto Exit;
    }
    if (EC_NULL == pSlaveProp)
    {
        EC_ERRORMSGC(("GetSlaveProp: NULL pointer passed!\n"));
        goto Exit;
    }    
    /* try to get values first from cfg slave, then from bus slave */
    {
    CEcSlave* pCfgSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);

        if (EC_NULL != pCfgSlave)
        {
            pCfgSlave->GetSlaveProp(pSlaveProp);
        }
        else
        {
        CEcBusSlave* pBusSlave = m_oAtEcatDesc.poMaster->GetBusSlaveBySlaveId(dwSlaveId);

            if (EC_NULL == pBusSlave)
            {
                goto Exit;
            }
            pBusSlave->GetSlaveProp(pSlaveProp);
        }
    }
    /* no errors */
    bRetVal = EC_TRUE;

Exit:
    return bRetVal;
}


/***************************************************************************************************/
/** \brief Get slave port state

\return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
EC_T_DWORD CAtEmInterface::GetSlavePortState
(   EC_T_DWORD          dwSlaveId,      /**< [in]  slave id */
    EC_T_WORD*          pwPortState)   /**< [out] slave properties */
{
EC_T_DWORD   dwRetVal  = EC_E_ERROR;
CEcBusSlave* pBusSlave = EC_NULL;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("GetSlavePortState: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("GetSlavePortState: Invalid slave ID!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (EC_NULL == pwPortState)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* try to get values first from cfg slave, then from bus slave */
    {
    CEcSlave* pCfgSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);

        if (EC_NULL != pCfgSlave)
        {
            if (!pCfgSlave->IsPresentOnBus())
            {
                dwRetVal = EC_E_SLAVE_NOT_PRESENT;
                goto Exit;
            }
            pBusSlave = pCfgSlave->m_pBusSlave;
        }
        else
        {
            pBusSlave = m_oAtEcatDesc.poMaster->GetBusSlaveBySlaveId(dwSlaveId);
        }
    }
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    *pwPortState = pBusSlave->GetPortState();

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/** \brief Get slave state

\return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
EC_T_DWORD CAtEmInterface::GetSlaveState
(   EC_T_DWORD dwSlaveId,           /**< [in]  slave id */
    EC_T_WORD* pwCurrDevState,      /**< [out]  current device state. See DEVICE_STATE_... */
    EC_T_WORD* pwReqDevState        /**< [out]  requested device state. See DEVICE_STATE_... */
)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("GetSlaveState: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("GetSlaveState: Invalid slave ID!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (EC_NULL == pwCurrDevState || EC_NULL == pwReqDevState)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (!m_oAtEcatDesc.poEcDevice->IsLinkConnected())
    {
        dwRetVal = EC_E_LINK_DISCONNECTED;
        goto Exit;
    }
    /* try to get values first from cfg slave, then from bus slave */
    {
    CEcSlave* pCfgSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);

        if (EC_NULL != pCfgSlave)
        {
            if (!pCfgSlave->IsPresentOnBus())
            {
                dwRetVal = EC_E_SLAVE_NOT_PRESENT;
                goto Exit;
            }
            *pwCurrDevState = pCfgSlave->GetDeviceState();
            *pwReqDevState  = pCfgSlave->GetSmReqDeviceState();
        }
        else
        {
        CEcBusSlave* pBusSlave = m_oAtEcatDesc.poMaster->GetBusSlaveBySlaveId(dwSlaveId);

            if (EC_NULL == pBusSlave)
            {
                dwRetVal = EC_E_NOTFOUND;
                goto Exit;
            }
            *pwCurrDevState = (EC_T_WORD)(pBusSlave->GetAlStatus() & DEVICE_STATE_MASK);
            *pwReqDevState  = DEVICE_STATE_UNKNOWN;
        }
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    OsDbgAssert( EC_E_ERROR != dwRetVal );

    return OsSetLastError(dwRetVal);
}

EC_T_DWORD  CAtEmInterface::ReadSlaveRegister(
    EC_T_BOOL   bFixedAddressing,   /**< [in]  EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD   wSlaveAddress,      /**< [in]  Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_WORD   wRegisterOffset,    /**< [in]  register offset inside the slave memory  */
    EC_T_VOID*  pvData,             /**< [out] pointer to data */
    EC_T_WORD   wLen,               /**< [in]  data length */
    EC_T_DWORD  dwTimeout           /**< [in]  timeout in msec */
)
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    EC_T_DWORD  dwRes       = EC_E_ERROR;
    CEcBusSlave* pBusSlave  = EC_NULL;
    EC_CMD_TYPE eEcCmd      = EC_CMD_TYPE_NOP;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ReadSlaveRegister: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (EC_NOWAIT == dwTimeout)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRes = m_oAtEcatDesc.poMaster->GetBusScanResult();
    if (EC_E_NOTREADY == dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    if (EC_E_BUSY == dwRes)
    {
    CEcTimer oTimeout;

        oTimeout.Start(dwTimeout);
        do
        {
            OsSleep(1);
            dwRes = m_oAtEcatDesc.poMaster->GetBusScanResult();
        } while (!oTimeout.IsElapsed() && ((EC_E_NOTREADY == dwRes) || (EC_E_BUSY == dwRes)));
        if (oTimeout.IsElapsed())
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }
    }

    if (bFixedAddressing)
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
        eEcCmd = EC_CMD_TYPE_FPRD;
    }
    else
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
        eEcCmd = EC_CMD_TYPE_APRD;
    }

    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    dwRetVal = m_oAtEcatDesc.poMaster->EcTransferRawCmd(0, 0, (EC_T_BYTE)eEcCmd, (((EC_T_DWORD)wSlaveAddress) << 16) | wRegisterOffset, pvData, wLen, dwTimeout);

Exit:
    return OsSetLastError(dwRetVal);
}

EC_T_DWORD  CAtEmInterface::WriteSlaveRegister(
    EC_T_BOOL   bFixedAddressing,   /**< [in]  EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD   wSlaveAddress,      /**< [in]  Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_WORD   wRegisterOffset,    /**< [in]  register offset inside the slave memory  */
    EC_T_VOID*  pvData,             /**< [out] pointer to data */
    EC_T_WORD   wLen,               /**< [in]  data length */
    EC_T_DWORD  dwTimeout           /**< [in]  timeout in msec */
)
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    EC_T_DWORD  dwRes       = EC_E_ERROR;
    CEcBusSlave* pBusSlave  = EC_NULL;
    EC_CMD_TYPE eEcCmd      = EC_CMD_TYPE_NOP;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("WriteSlaveRegister: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (EC_NOWAIT == dwTimeout)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRes = m_oAtEcatDesc.poMaster->GetBusScanResult();
    if (EC_E_NOTREADY == dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    if (EC_E_BUSY == dwRes)
    {
    CEcTimer oTimeout;

        oTimeout.Start(dwTimeout);
        do
        {
            OsSleep(1);
            dwRes = m_oAtEcatDesc.poMaster->GetBusScanResult();
        } while (!oTimeout.IsElapsed() && ((EC_E_NOTREADY == dwRes) || (EC_E_BUSY == dwRes)));
        if (oTimeout.IsElapsed())
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }
    }

    if (bFixedAddressing)
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
        eEcCmd = EC_CMD_TYPE_FPWR;
    }
    else
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
        eEcCmd = EC_CMD_TYPE_APWR;
    }

    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    dwRetVal = m_oAtEcatDesc.poMaster->EcTransferRawCmd(0, 0, (EC_T_BYTE)eEcCmd, (((EC_T_DWORD)wSlaveAddress) << 16) | wRegisterOffset, pvData, wLen, dwTimeout);

Exit:
    return OsSetLastError(dwRetVal);
}

EC_T_DWORD CAtEmInterface::ReadSlaveIdentification(
    EC_T_BOOL   bFixedAddressing,
    EC_T_WORD   wSlaveAddress,
    EC_T_WORD   wAdo,
    EC_T_WORD*  pwValue,
    EC_T_DWORD  dwTimeout)
{
    EC_T_DWORD   dwRetVal     = EC_E_ERROR;
    EC_T_DWORD   dwRes        = EC_E_ERROR;
    CEcBusSlave* pBusSlave    = EC_NULL;
    EC_CMD_TYPE  eEcCmdRd     = EC_CMD_TYPE_NOP;
    EC_CMD_TYPE  eEcCmdWr     = EC_CMD_TYPE_NOP;
    EC_T_BOOL    bIdRequested = EC_FALSE;
    EC_T_WORD    wIdValue     = 0;
    EC_T_BYTE    abyDataFrm[] = { 0, 0, 0, 0, 0, 0 };

    if (EC_NOWAIT == dwTimeout)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (EC_NULL == pwValue)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ReadSlaveIdentification: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRes = m_oAtEcatDesc.poMaster->GetBusScanResult();
    if (EC_E_NOTREADY == dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    if (EC_E_BUSY == dwRes)
    {
        CEcTimer oTimeout;

        oTimeout.Start(dwTimeout);
        do
        {
            OsSleep(1);
            dwRes = m_oAtEcatDesc.poMaster->GetBusScanResult();
        } while (!oTimeout.IsElapsed() && ((EC_E_NOTREADY == dwRes) || (EC_E_BUSY == dwRes)));
        if (oTimeout.IsElapsed())
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }
    }

    if (bFixedAddressing)
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
        eEcCmdRd = EC_CMD_TYPE_FPRD;
        eEcCmdWr = EC_CMD_TYPE_FPWR;
    }
    else
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
        eEcCmdRd = EC_CMD_TYPE_APRD;
        eEcCmdWr = EC_CMD_TYPE_APWR;
    }
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    if (ECREG_AL_STATUSCODE == wAdo)
    {
        CEcTimer oTimeout;

        /* explicit devide identification requires firmware */
        if (pBusSlave->IsDeviceEmulation())
        {
            dwRetVal = EC_E_ADO_NOT_SUPPORTED;
            goto Exit;
        }
        /* request identification value */
        EC_SET_FRM_WORD(abyDataFrm, (EC_T_WORD)(DEVICE_STATE_IDREQUEST | (pBusSlave->GetAlStatus() & DEVICE_STATE_MASK)));
        dwRes = m_oAtEcatDesc.poMaster->EcTransferRawCmd(0, 0, (EC_T_BYTE)eEcCmdWr, (((EC_T_DWORD)wSlaveAddress) << 16) | ECREG_AL_CONTROL, abyDataFrm, 2, dwTimeout);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
        bIdRequested = EC_TRUE;

        /* wait for acknowledge */
        for (oTimeout.Start(dwTimeout); !oTimeout.IsElapsed();)
        {
            dwRes = m_oAtEcatDesc.poMaster->EcTransferRawCmd(0, 0, (EC_T_BYTE)eEcCmdRd, (((EC_T_DWORD)wSlaveAddress) << 16) | ECREG_AL_STATUS, abyDataFrm, 6, dwTimeout);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            if (0 != (EC_GET_FRM_WORD(abyDataFrm) & DEVICE_STATE_IDREQUEST))
            {
                break;
            }
        }
        if (oTimeout.IsElapsed())
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }
	    wIdValue = EC_GET_FRM_WORD(abyDataFrm + 4);
    }
    else
    {
        /* direct id mechanism */
        dwRes = m_oAtEcatDesc.poMaster->EcTransferRawCmd(0, 0, (EC_T_BYTE)eEcCmdRd, (((EC_T_DWORD)wSlaveAddress) << 16) | wAdo, abyDataFrm, 2, dwTimeout);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }

        wIdValue = EC_GET_FRM_WORD(abyDataFrm);
    }
    /* return identification value */
    *pwValue = wIdValue;

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (bIdRequested)
    {
        /* reset request */
        EC_SET_FRM_WORD(abyDataFrm, (EC_T_WORD)(pBusSlave->GetAlStatus() & DEVICE_STATE_MASK));
        dwRes = m_oAtEcatDesc.poMaster->EcTransferRawCmd(0, 0, (EC_T_BYTE)eEcCmdWr, (((EC_T_DWORD)wSlaveAddress) << 16) | ECREG_AL_CONTROL, abyDataFrm, 2, dwTimeout);
    }
    return dwRetVal;
}

EC_T_DWORD CAtEmInterface::SetSlaveDisabled(
    EC_T_BOOL   bFixedAddressing,
    EC_T_WORD   wSlaveAddress,
    EC_T_BOOL   bDisabled)
{
EC_T_DWORD dwRetVal  = EC_E_ERROR;
CEcSlave*  pCfgSlave = EC_NULL;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("SetSlaveDisabled: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (bFixedAddressing)
    {
        pCfgSlave = m_oAtEcatDesc.poMaster->GetSlaveByCfgFixedAddr(wSlaveAddress);
    }
    else
    {
        pCfgSlave = m_oAtEcatDesc.poMaster->GetSlaveByCfgAutoIncAddr(wSlaveAddress);
    }
    if (EC_NULL == pCfgSlave)
    {
        EC_ERRORMSGC(("SetSlaveDisabled: Slave not found!\n"));
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    pCfgSlave->SetIgnoreAbsence(bDisabled);
    pCfgSlave->SetMaxState(bDisabled?eEcatState_PREOP:eEcatState_OP);

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    OsDbgAssert( EC_E_ERROR != dwRetVal );

    return OsSetLastError(dwRetVal);
}

EC_T_DWORD CAtEmInterface::SetSlaveDisconnected(
    EC_T_BOOL   bFixedAddressing,
    EC_T_WORD   wSlaveAddress,
    EC_T_BOOL   bDisconnected)
{
EC_T_DWORD dwRetVal  = EC_E_ERROR;
CEcSlave*  pCfgSlave = EC_NULL;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("SetSlaveDisconnected: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (bFixedAddressing)
    {
        pCfgSlave = m_oAtEcatDesc.poMaster->GetSlaveByCfgFixedAddr(wSlaveAddress);
    }
    else
    {
        pCfgSlave = m_oAtEcatDesc.poMaster->GetSlaveByCfgAutoIncAddr(wSlaveAddress);
    }
    if (EC_NULL == pCfgSlave)
    {
        EC_ERRORMSGC(("SetSlaveDisconnected: Slave not found!\n"));
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    pCfgSlave->SetIgnoreAbsence(bDisconnected);
    pCfgSlave->SetMaxState(bDisconnected?eEcatState_INIT:eEcatState_OP);

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    OsDbgAssert( EC_E_ERROR != dwRetVal );

    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Transfer single Raw command

  \return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
EC_T_DWORD CAtEmInterface::TferSingleRawCmd(
    EC_T_DWORD  dwSlaveId,              /**< [in]  slave id */
    EC_T_BYTE   byCmd,                  /**< [in]  ecat command EC_CMD_TYPE_... */
    EC_T_DWORD  dwMemoryAddress,        /**< [in]  memory address inside the slave (physical or logical) */
    EC_T_VOID*  pvData,                 /**< [in, out]  pointer to data */
    EC_T_WORD   wLen,                   /**< [in]  data length */
    EC_T_DWORD  dwTimeout )             /**< [in]  timeout in msec */
{
    CEcSlave*   poSlave     = EC_NULL;
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("TferSingleRawCmd: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("TferSingleRawCmd: Invalid slave ID!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (MASTER_SLAVE_ID == dwSlaveId )
    {
        dwRetVal = m_oAtEcatDesc.poMaster->EcTransferRawCmd(0, 0, byCmd, dwMemoryAddress, pvData, wLen, dwTimeout );
        goto Exit;
    }
    if (!m_oAtEcatDesc.poMaster->IsConfigured())
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* get config slave */
    poSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);
    if (EC_NULL == poSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    if (!poSlave->IsPresentOnBus())
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    dwRetVal = poSlave->EcTransferRawCmd(0, 0, byCmd, dwMemoryAddress, pvData, wLen, dwTimeout );

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Transfer Raw command

  \return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
EC_T_DWORD CAtEmInterface::QueueRawCmd
(   EC_T_DWORD dwClntId,            /**< [in]  client id, 0 if it is broadcast */
    EC_T_DWORD dwSlaveId,           /**< [in]  slave id */
    EC_T_WORD  wInvokeId,           /**< [in]  Invoke Id */
    EC_T_BYTE  byCmd,               /**< [in]  ecat command */
    EC_T_DWORD dwMemoryAddress,     /**< [in]  memory address inside the slave (physical or logical) */
    EC_T_VOID* pvData,              /**< [in]  pointer to data */
    EC_T_WORD  wLen                 /**< [in]  data length */
)
{
    CEcSlave*       poSlave     = EC_NULL;
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    EC_T_CLNTDESC*  pClntDesc   = EC_NULL;
    EC_T_DWORD      dwUniqueClntId = 0;
    EC_T_BOOL       bLocked     = EC_FALSE;
    CEcTimer  oTimeout;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("QueueRawCmd: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("QueueRawCmd: Invalid slave ID!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (dwClntId != 0)
    {
        /* synchronize descriptor access */
        oTimeout.Start(1000);
        OsLock(m_oAtEcatDesc.poClntDescLock);
        while (0 < m_oAtEcatDesc.dwClntDescInUse)
        {
            OsUnlock(m_oAtEcatDesc.poClntDescLock);
            if (oTimeout.IsElapsed())
            {
                dwRetVal = EC_E_TIMEOUT;
                goto Exit;
            }
            OsSleep(1);
            OsLock(m_oAtEcatDesc.poClntDescLock);
        }
        bLocked = EC_TRUE;

        pClntDesc = m_oAtEcatDesc.pClntDescHead;
        while ((EC_NULL != pClntDesc) && (pClntDesc->byUniqueClntId != dwClntId))
        {
            pClntDesc = pClntDesc->pNext;
        }
        if (EC_NULL == pClntDesc)
        {
            goto Exit;
        }
        dwUniqueClntId = IS_CLIENT_RAW_CMD | pClntDesc->byUniqueClntId;
        OsUnlock(m_oAtEcatDesc.poClntDescLock);
        bLocked = EC_FALSE;
    }
    if (MASTER_SLAVE_ID == dwSlaveId)
    {
        dwRetVal = m_oAtEcatDesc.poMaster->EcTransferRawCmd(dwUniqueClntId, wInvokeId, byCmd, dwMemoryAddress, pvData, wLen, EC_NOWAIT);
        goto Exit;
    }
    if (!m_oAtEcatDesc.poMaster->IsConfigured())
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* get config slave */
    poSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);
    if (EC_NULL == poSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    if (!poSlave->IsPresentOnBus())
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    dwRetVal = poSlave->EcTransferRawCmd(dwUniqueClntId, wInvokeId, byCmd, dwMemoryAddress, pvData, wLen, EC_NOWAIT);

Exit:
    if (bLocked)
    {
        OsUnlock(m_oAtEcatDesc.poClntDescLock);
    }
    return OsSetLastError(dwRetVal);
}


/***************************************************************************************************/
/** \brief set slave device state

\return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
EC_T_DWORD CAtEmInterface::SetSlaveState
(   EC_T_DWORD dwSlaveId,           /**< [in]  slave id */
    EC_T_WORD  wNewReqDevState,     /**< [in]  requested new device state. See DEVICE_STATE_... */
    EC_T_DWORD dwTimeout)           /**< [in]  timeout value */
{
    CEcSlave* poSlave           = EC_NULL;
    EC_T_WORD wCurrDevState     = DEVICE_STATE_UNKNOWN;
    EC_T_DWORD dwRetVal         = EC_E_ERROR;
    EC_T_DWORD dwRes            = EC_E_ERROR;
    CEcTimer oTimeout;

    EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP1_ECATAPI, ETHTYP0_ECATAPI_SET_SLAVE_STATE,
        "SetSlaveState(%d, %s) ...\n", dwSlaveId, SlaveDevStateText(wNewReqDevState)));

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("SetSlaveState: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("SetSlaveState: Invalid slave ID!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (!m_oAtEcatDesc.poMaster->IsConfigured())
    {
        EC_ERRORMSGC(("SetSlaveState: Stack not configured!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* get config slave */
    poSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);
    if (EC_NULL == poSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    if (!poSlave->IsPresentOnBus())
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    /* Get the slave's state. No waiting needed, because the EC-Master keeps the state cache up-to-date. */
    wCurrDevState = poSlave->GetDeviceState();

    /* manually ack error */
    if ((EC_NULL != poSlave->m_pBusSlave) && poSlave->m_pBusSlave->IsAlStatusErrorPending())
    {
        EC_T_WORD wAlControlFrm = 0;
        EC_SET_FRM_WORD((EC_T_BYTE*)&wAlControlFrm, poSlave->GetAlStatus());
        dwRes = TferSingleRawCmd(dwSlaveId, EC_CMD_TYPE_FPWR, ECREG_AL_CONTROL, &wAlControlFrm, sizeof(EC_T_WORD), dwTimeout);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
        /* Synchronous call? */
        if (EC_NOWAIT != dwTimeout)
        {
	        oTimeout.Start(dwTimeout);
	        do
	        {
	            OsSleep(1);
	        }
	        while (!oTimeout.IsElapsed() && poSlave->m_pBusSlave->IsAlStatusErrorPending());
        }
    }

    dwRes = poSlave->SetDeviceState(wNewReqDevState);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    /* Synchronous call? */
    if (EC_NOWAIT == dwTimeout)
    {
        /* Finished without errors */
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }

    /* wait until the slave state machine is stable or timeout */
    oTimeout.Start(dwTimeout);
    while (!poSlave->SlaveStateMachIsStable() && !oTimeout.IsElapsed())
    {
        OsSleep(1);
    }

    if (poSlave->TargetStateNotReachable())
    {
        dwRetVal = EC_E_SLAVE_ERROR;
        goto Exit;
    }

    if ((DEVICE_STATE_BOOTSTRAP == wNewReqDevState) && (!poSlave->m_pBusSlave->GetBootStrapSupported()))
    {
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }

    if (oTimeout.IsElapsed())
    {
        /* a timeout occurred */
        dwRetVal = EC_E_TIMEOUT;
        goto Exit;
    }

    /* if slave state was transisted to another state meanwhile, stop with BUSY */
    if (wNewReqDevState != poSlave->GetDeviceState())
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;

Exit:
    OsDbgAssert( EC_E_ERROR != dwRetVal );

    if ((EC_NULL != m_oAtEcatDesc.pInitMasterParms) && (m_oAtEcatDesc.pInitMasterParms->dwLogLevel >= EC_LOG_LEVEL_VERBOSE))
    {
        OsDbgMsg("SetSlaveState: %d, %s, %s: %s (0x%x)\n", ((EC_NULL != poSlave) ? (EC_T_INT)poSlave->GetFixedAddr() : -1),
            SlaveDevStateText(wCurrDevState), SlaveDevStateText(wNewReqDevState),
            EcErrorText(dwRetVal), dwRetVal);
    }

    return OsSetLastError(dwRetVal);
}


/***************************************************************************************************/
/** \brief Create mailbox transfer object

\return pointer to the mailbox transfer object, EC_NULL in case of error
*/
EC_T_MBXTFER* CAtEmInterface::MbxTferCreate
(   EC_T_MBXTFER_DESC* pMbxTferDesc,    /**< [in]  mailbox transfer descriptor */
    struct _EC_T_MEMORY_LOGGER* pMLog )
{
    EC_T_MBXTFERP*  pMbxTferPriv = EC_NULL;

    /* check parameters */
    if (EC_NULL == pMbxTferDesc)
    {
        EC_ERRORMSGC(("MbxTferCreate: pMbxTferDesc missing!\n"));
        goto Exit;
    }
    /* allocate the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)OsMalloc( sizeof(EC_T_MBXTFERP));
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_INTERFACE, pMLog, "ecatMbxTferCreate", pMbxTferPriv, sizeof(EC_T_MBXTFERP));
    if (pMbxTferPriv == EC_NULL )
    {
        goto Exit;
    }
    /* reset the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    OsMemcpy(&(pMbxTferPriv->MbxTfer.MbxTferDesc), pMbxTferDesc, sizeof(EC_T_MBXTFER_DESC));

    pMbxTferPriv->MbxTfer.dwDataLen      = pMbxTferDesc->dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = (0 == pMbxTferDesc->dwMaxDataLen)?EC_NULL:pMbxTferDesc->pbyMbxTferDescData;

    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

Exit:
    return (EC_T_MBXTFER*)pMbxTferPriv;
}

/***************************************************************************************************/
/** \brief Abort mailbox transfer
 - Also sends Abort to slave
 - No error is returned even if MbxTferStatus is not Pending.

 \return EC_E_NOERROR or EC_E_NOTSUPPORTED for all Mbx protocols but CoE, FoE
*/
EC_T_DWORD CAtEmInterface::MbxTferAbort
(   EC_T_MBXTFER* pMbxTfer )    /**< [in]  mailbox transfer object */
{
    EC_T_DWORD     dwRetVal     = EC_E_ERROR;
    EC_T_MBXTFERP* pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;

    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("MbxTferAbort: pMbxTferDesc missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* only FoE, CoE transfer can be currently aborted */
    switch (pMbxTfer->eMbxTferType)
    {
    case eMbxTferType_COE_SDO_DOWNLOAD:
    case eMbxTferType_COE_SDO_UPLOAD:
    case eMbxTferType_FOE_FILE_DOWNLOAD:
    case eMbxTferType_FOE_FILE_UPLOAD:
    case eMbxTferType_FOE_SEG_DOWNLOAD:
    case eMbxTferType_FOE_SEG_UPLOAD:
        break;
    default:
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }
    pMbxTferPriv->bAbort = EC_TRUE;
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Delete mailbox transfer object

\return N/A
*/
EC_T_VOID CAtEmInterface::MbxTferDelete
(   EC_T_MBXTFER* pMbxTfer     /**< [in]  mailbox transfer object */
  , struct _EC_T_MEMORY_LOGGER* pMLog )
{
    EC_T_MBXTFERP* pMbxTferPriv       = (EC_T_MBXTFERP*)pMbxTfer;

    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("MbxTferDelete: pMbxTferDesc missing!\n"));
        return;
    }
    OsDbgAssert(pMbxTferPriv->MbxTfer.eTferStatus != eMbxTferStatus_Pend);
    OsDbgAssert(!pMbxTferPriv->oEcMbxCmd.bInUseByInterface);

    /* free the mailbox transfer object */
    EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_INTERFACE, pMLog, "ecatMbxTferDelete", pMbxTferPriv, sizeof(EC_T_MBXTFERP));
    SafeOsFree(pMbxTferPriv);
}

/***************************************************************************************************/
/** \brief Initiate mailbox transfer

This function handles the generic mailbox transfer stuff.

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::MbxTfer(
    EC_T_MBXTFERP* pMbxTferPriv,    /**< [in] mailbox transfer object */
    EC_T_DWORD     dwSlaveId,       /**< [in] slave id to download the SDO */
    EC_T_DWORD     dwTimeout,        /**< [in] Timeout in milliseconds until transfer object shall be idle */
    EC_T_BOOL      bWait
    )
{
    EC_T_DWORD      dwRes           = EC_E_ERROR;
    EC_T_DWORD      dwRetVal        = EC_E_ERROR;
    EC_T_CLNTDESC*  pClntDesc       = EC_NULL;
    CEcSlave*       poSlave         = EC_NULL;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("MbxTfer: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check if ADS adapter is running */
    if (AdsIsRunning())
    {
        dwRetVal = EC_E_ADS_IS_RUNNING;
        goto Exit;
    }
    /* check parameters */
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("MbxTfer: Invalid slave ID!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    OsDbgAssert(pMbxTferPriv->MbxTfer.eTferStatus != eMbxTferStatus_Pend);
    OsDbgAssert(!pMbxTferPriv->oEcMbxCmd.bInUseByInterface);

    /* check state if slave is addressed */
    if (MASTER_SLAVE_ID > dwSlaveId)
    {
        if (!m_oAtEcatDesc.poMaster->IsConfigured())
        {
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }
        /* get config slave */
        poSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);
        if (EC_NULL == poSlave)
        {
            dwRetVal = EC_E_NOTFOUND;
            goto Exit;
        }
        if (!poSlave->IsPresentOnBus())
        {
            dwRetVal = EC_E_SLAVE_NOT_PRESENT;
            goto Exit;
        }
        if (!poSlave->HasMailBox())
        {
            dwRetVal = EC_E_NO_MBX_SUPPORT;
            goto Exit;
        }

        /* in unknown state mailbox transfers to slaves are not possible */
        if (eEcatState_UNKNOWN == m_oAtEcatDesc.eMasterState)
        {
            EC_ERRORMSGC(("MbxTfer: Master State is UNKNOWN!\n"));
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }

        if (pMbxTferPriv->oEcMbxCmd.wMbxType == ETHERCAT_MBOX_TYPE_FILEACCESS)
        {
#if (defined INCLUDE_FOE_SUPPORT)
            CEcMbSlave* pMbSlave = (CEcMbSlave*)poSlave;

            if (pMbSlave->GetMbxSize(VG_OUT) < EC_FOE_MIN_MBX_LEN)
            {
                EC_ERRORMSGC(("MbxTfer: Mbx too small for FoE (set min %d, act %d)!\n", EC_FOE_MIN_MBX_LEN, pMbSlave->GetMbxSize(VG_OUT)));
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            if (pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength > pMbSlave->GetFoEMaxFilenameLen())
            {
                EC_ERRORMSGC(("MbxTfer: Mbx sized %d bytes too small for FoE file name with len %d!\n", pMbSlave->GetMbxSize(VG_OUT), pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength));
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
#endif /* INCLUDE_FOE_SUPPORT */
        }
        else
        {
            /* in Bootstrap only FoE is supported */
            if ((DEVICE_STATE_BOOTSTRAP == ((EC_T_WORD)(poSlave->GetDeviceState())))
                || (DEVICE_STATE_BOOTSTRAP == poSlave->GetSmReqDeviceState()))
            {
                dwRetVal = EC_E_INVALID_SLAVE_STATE;
                goto Exit;
            }
        }
    }
    /* for simple API no notification needed hence no clientid appended */
    if (0 != pMbxTferPriv->MbxTfer.dwClntId)
    {
        /* search client descriptor */
        OsLock(m_oAtEcatDesc.poClntDescLock);
        pClntDesc = m_oAtEcatDesc.pClntDescHead;
        while ((pClntDesc != EC_NULL) && (pClntDesc->byUniqueClntId != pMbxTferPriv->MbxTfer.dwClntId))
        {
            pClntDesc = pClntDesc->pNext;
        }
        OsUnlock(m_oAtEcatDesc.poClntDescLock);

        if (EC_NULL == pClntDesc)
        {
            EC_ERRORMSGC(("MbxTfer: Invalid client ID %d", pMbxTferPriv->MbxTfer.dwClntId));
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    }
    /* refresh mailbox tfer private descriptor */
    pMbxTferPriv->bAbort                            = EC_FALSE;

    /* refresh mailbox command descriptor */
    pMbxTferPriv->oEcMbxCmd.pvContext               = pMbxTferPriv;
    pMbxTferPriv->oEcMbxCmd.dwInvokeId              = pMbxTferPriv->MbxTfer.dwTferId;
    pMbxTferPriv->oEcMbxCmd.length                  = pMbxTferPriv->MbxTfer.dwDataLen;
    pMbxTferPriv->oEcMbxCmd.pbyData                 = pMbxTferPriv->MbxTfer.pbyMbxTferData;
#ifdef INCLUDE_FOE_SUPPORT
    if ( (eMbxTferType_FOE_FILE_UPLOAD   == pMbxTferPriv->MbxTfer.eMbxTferType)
      || (eMbxTferType_FOE_FILE_DOWNLOAD == pMbxTferPriv->MbxTfer.eMbxTferType))
    {
        pMbxTferPriv->MbxTfer.MbxData.FoE.dwFileSize = pMbxTferPriv->MbxTfer.dwDataLen;
    }
#endif
    pMbxTferPriv->oEcMbxCmd.bInUseByInterface       = EC_TRUE;

    /* refresh mailbox transfer object */
    pMbxTferPriv->MbxTfer.eTferStatus               = eMbxTferStatus_Pend;
    pMbxTferPriv->MbxTfer.dwErrorCode               = EC_E_NOERROR;
    pMbxTferPriv->dwSlaveId                         = dwSlaveId;

    if (EC_NOWAIT == dwTimeout)
    {
        pMbxTferPriv->oEcMbxCmd.oTimeout.Start(m_oAtEcatDesc.poMaster->GetMbxCmdTimeout());
    }
    else
    {
        pMbxTferPriv->oEcMbxCmd.oTimeout.Start(dwTimeout);
    }

    /* send mailbox command */
    dwRes = m_oAtEcatDesc.poMaster->SendMailboxCmd(dwSlaveId, &pMbxTferPriv->oEcMbxCmd);
    if (EC_E_NOERROR != dwRes)
    {
        pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_TferReqError;
        pMbxTferPriv->MbxTfer.dwErrorCode = dwRes;
        pMbxTferPriv->oEcMbxCmd.bInUseByInterface = EC_FALSE;

        dwRetVal = dwRes;
        goto Exit;
    }
    /* in case of timeout: wait until transfer has finished */
    if (bWait)
    {
        /* wait for transfer finish or timeout */
        while (pMbxTferPriv->MbxTfer.eTferStatus == eMbxTferStatus_Pend)
        {
            OsSleep(1);
        }
        if (pMbxTferPriv->MbxTfer.eTferStatus == eMbxTferStatus_TferReqError)
        {
            dwRetVal = pMbxTferPriv->MbxTfer.dwErrorCode;
            goto Exit;
        }
    }
    /* no error */
    dwRetVal = EC_E_NOERROR;

#if (defined INCLUDE_MASTER_OBD)
    /* SDO Download is a request to change master state. Since SDO system cannot McSM, this is handled
     * right here in API
     */
    if (pMbxTferPriv->MbxTfer.eMbxTferType == eMbxTferType_COE_SDO_DOWNLOAD)
    {
        if ( 0xffff != m_oAtEcatDesc.poMaster->GetMasterStChngRequest())
        {
            switch( m_oAtEcatDesc.poMaster->GetMasterStChngRequest())
            {
            case DEVICE_STATE_INIT:
                {
                    SetMasterStateEx(EC_NOWAIT, eEcatState_INIT);
                } break;
            case DEVICE_STATE_PREOP:
                {
                    SetMasterStateEx(EC_NOWAIT, eEcatState_PREOP);
                } break;
            case DEVICE_STATE_SAFEOP:
                {
                    SetMasterStateEx(EC_NOWAIT, eEcatState_SAFEOP);
                } break;
            case DEVICE_STATE_OP:
                {
                    SetMasterStateEx(EC_NOWAIT, eEcatState_OP);
                } break;
            default:
                {
                    /* do nothing */
                } break;
            }
            m_oAtEcatDesc.poMaster->SetMasterStChngRequest(0xffff);
        }
    }
#endif

Exit:
    if (EC_NULL != pMbxTferPriv)
    {
        if (EC_E_NOERROR != dwRetVal)
        {
            pMbxTferPriv->MbxTfer.dwErrorCode = dwRetVal;
            pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_TferReqError;
            pMbxTferPriv->oEcMbxCmd.bInUseByInterface = EC_FALSE;
        }
        dwRetVal = pMbxTferPriv->MbxTfer.dwErrorCode;
    }
    return OsSetLastError(dwRetVal);
}

#if (defined INCLUDE_RAWMBX_SUPPORT)
/***************************************************************************************************/
/** \brief Send raw mailbox command

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::SendRawMbx(EC_T_DWORD dwClntId, EC_T_BYTE* pbyMbxCmd, EC_T_DWORD dwMbxCmdLen, EC_T_DWORD dwTimeout)
{
    EC_T_DWORD          dwRetVal       = EC_E_ERROR;
    EC_T_DWORD          dwRes          = EC_E_ERROR;
    EC_T_WORD           wFixedAddress  = INVALID_FIXED_ADDR;
    CEcBusSlave*        pBusSlave      = EC_NULL;
    CEcSlave*           pCfgSlave      = EC_NULL;
    EC_T_DWORD          dwSlaveId      = INVALID_SLAVE_ID;
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv   = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    ETHERCAT_MBOX_HEADER* pMBox = (ETHERCAT_MBOX_HEADER*)pbyMbxCmd;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* mailbox transfer always requires timeout */
    if (EC_NOWAIT == dwTimeout)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get slave fixed address */
    if (sizeof(ETHERCAT_MBOX_HEADER) > dwMbxCmdLen)
    {
        dwRetVal = EC_E_INVALIDSIZE;
        goto Exit;
    }
    wFixedAddress = EC_GET_FRM_WORD(&pMBox->wAddress);
    
    if(wFixedAddress != 0)
    {
        /* get bus slave */
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wFixedAddress);
        if (EC_NULL == pBusSlave)
        {
            dwRetVal = EC_E_SLAVE_NOT_PRESENT;
            goto Exit;
        }
        /* get config slave */
        pCfgSlave = pBusSlave->GetCfgSlave();
        if (EC_NULL == pCfgSlave)
        {
            dwRetVal = EC_E_SLAVE_NOT_PRESENT;
            goto Exit;
        }
        /* get mailbox slave */
        if (!pCfgSlave->HasMailBox())
        {
            dwRetVal = EC_E_NO_MBX_SUPPORT;
            goto Exit;
        }
        dwSlaveId = pCfgSlave->GetSlaveId();
    }
    else
    {
        /* addressing the master OD */
        dwSlaveId = MASTER_SLAVE_ID;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = dwMbxCmdLen;
    oMbxTferDesc.pbyMbxTferDescData = (dwMbxCmdLen == 0)?EC_NULL:pbyMbxCmd;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = dwClntId;
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_RAWMBX;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId        = MAILBOXCMD_ECAT_RAWMBX;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType  = EC_MAILBOX_CMD_UPLOAD; /* internal */

    /* send mailbox request to the master */
    dwRes = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    dwRetVal = pMbxTferPriv->MbxTfer.dwErrorCode;

Exit:
    return OsSetLastError(dwRetVal);
}
#endif /* INCLUDE_RAWMBX_SUPPORT */

/***************************************************************************************************/
/** \brief Initiate CoE SDO download transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::CoeSdoDownloadReq(
    EC_T_MBXTFER*   pMbxTfer,       /**< [in]   mailbox transfer object */
    EC_T_DWORD      dwSlaveId,      /**< [in]   slave id to download the SDO */
    EC_T_WORD       wObIndex,       /**< [in]   object index */
    EC_T_BYTE       byObSubIndex,   /**< [in]   object sub-index */
    EC_T_DWORD      dwTimeout,      /**< [in]   Timeout in milliseconds */
    EC_T_DWORD      dwFlags         /**< [in]   mailbox transfer flags */
                                            )
{
    EC_T_MBXTFERP*  pMbxTferPriv;
    EC_T_DWORD      dwRetVal;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("CoeSdoDownloadReq: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("CoeSdoDownloadReq(): pMbxTfer = NULL is invalid!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    OsDbgAssert(eMbxTferStatus_Idle == pMbxTfer->eTferStatus);
    if ((pMbxTfer->MbxTferDesc.dwMaxDataLen < pMbxTfer->dwDataLen))
    {
        EC_ERRORMSGC(("CoeSdoDownloadReq(): mismatching data length pMbxTfer->MbxTferDesc.dwMaxDataLen = %d, pMbxTfer->dwDataLen = %d!\n", pMbxTfer->MbxTferDesc.dwMaxDataLen, pMbxTfer->dwDataLen));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (!CEcMasterInterfaceBase::IsValidCoeSdoParms(wObIndex, byObSubIndex, pMbxTfer->pbyMbxTferData, pMbxTfer->dwDataLen, dwTimeout, dwFlags))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_COE_SDO_DOWNLOAD;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_CANOPEN_SDO;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_CANOPEN;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ETHERCAT_CANOPEN_TYPE_SDOREQ;
    pMbxTferPriv->oEcMbxCmd.sIndex.index      = wObIndex;
    pMbxTferPriv->oEcMbxCmd.sIndex.subIndex   = byObSubIndex;
    pMbxTferPriv->oEcMbxCmd.dwMailboxFlags    = dwFlags;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_FALSE);

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief CoE SDO download transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::CoeSdoDownload(
     EC_T_DWORD     dwSlaveId       /* [in]  slave ID */
    ,EC_T_WORD      wObIndex        /* [in]  SDO index */
    ,EC_T_BYTE      byObSubIndex    /* [in]  SDO sub-index */
    ,EC_T_BYTE*     pbyData         /* [in]  SDO data */
    ,EC_T_DWORD     dwDataLen       /* [in]  length of pbyData buffer */
    ,EC_T_DWORD     dwTimeout       /* [in]  timeout in msec, must not be set to EC_NOWAIT */
    ,EC_T_DWORD     dwFlags         /* [in]  mailbox transfer flags, see EC_MAILBOX_FLAG */
                                         )
{
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("CoeSdoDownload: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (!CEcMasterInterfaceBase::IsValidCoeSdoParms(wObIndex, byObSubIndex, pbyData, dwDataLen, dwTimeout, dwFlags))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = dwDataLen;
    oMbxTferDesc.pbyMbxTferDescData = (dwDataLen == 0)?EC_NULL:pbyData;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = 0;  /* no notification needed for simple API so don't include clientID */
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_COE_SDO_DOWNLOAD;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_CANOPEN_SDO;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_CANOPEN;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ETHERCAT_CANOPEN_TYPE_SDOREQ;
    pMbxTferPriv->oEcMbxCmd.sIndex.index      = wObIndex;
    pMbxTferPriv->oEcMbxCmd.sIndex.subIndex   = byObSubIndex;
    pMbxTferPriv->oEcMbxCmd.dwMailboxFlags    = dwFlags;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }
    /* return mailbox transfer result */
    dwRetVal = pMbxTferPriv->MbxTfer.dwErrorCode;

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief CoE SDO upload transfer

\return Depends on the implementation of the function OsSetLastError(),
        normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::CoeSdoUpload(
    EC_T_DWORD      dwSlaveId,      /**< [in]   slave id to download the SDO */
    EC_T_WORD       wObIndex,       /**< [in]   object index */
    EC_T_BYTE       byObSubIndex,   /**< [in]   object sub-index */
    EC_T_BYTE*      pbyData,        /**< [out]  SDO data */
    EC_T_DWORD      dwDataLen,      /**< [in]   length of pbyData buffer */
    EC_T_DWORD*     pdwOutDataLen,  /**< [out]  SDO data length (number of received bytes) */
    EC_T_DWORD      dwTimeout,      /**< [in]   Timeout in milliseconds */
    EC_T_DWORD      dwFlags         /**< [in]   mailbox transfer flags, see EC_MAILBOX_FLAG */
                                       )
{
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv    = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("CoeSdoUpload: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (!CEcMasterInterfaceBase::IsValidCoeSdoParms(wObIndex, byObSubIndex, pbyData, dwDataLen, dwTimeout, dwFlags))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = dwDataLen;
    oMbxTferDesc.pbyMbxTferDescData = (dwDataLen == 0)?EC_NULL:pbyData;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = 0;  /* no notification needed for simple API so don't include clientID */
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_COE_SDO_UPLOAD;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_CANOPEN_SDO;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_CANOPEN;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ETHERCAT_CANOPEN_TYPE_SDORES;
    pMbxTferPriv->oEcMbxCmd.sIndex.index      = wObIndex;
    pMbxTferPriv->oEcMbxCmd.sIndex.subIndex   = byObSubIndex;
    pMbxTferPriv->oEcMbxCmd.dwMailboxFlags    = dwFlags;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }

    if (EC_NULL != pdwOutDataLen)
    {
        EC_SETDWORD(pdwOutDataLen, EC_MIN(dwDataLen, pMbxTferPriv->MbxTfer.dwDataLen));
    }

    /* return mailbox transfer result */
    dwRetVal = pMbxTferPriv->MbxTfer.dwErrorCode;

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Initiate CoE SDO upload transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::CoeSdoUploadReq(
    EC_T_MBXTFER*   pMbxTfer,       /**< [in]   mailbox transfer object */
    EC_T_DWORD      dwSlaveId,      /**< [in]   slave id to download the SDO */
    EC_T_WORD       wObIndex,       /**< [in]   object index */
    EC_T_BYTE       byObSubIndex,   /**< [in]   object sub-index */
    EC_T_DWORD      dwTimeout,      /**< [in]   Timeout in milliseconds */
    EC_T_DWORD      dwFlags         /**< [in]   mailbox transfer flags, see EC_MAILBOX_FLAG */
                                          )
{
    EC_T_MBXTFERP*  pMbxTferPriv;
    EC_T_DWORD      dwRetVal;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("CoeSdoUploadReq: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("CoeSdoUploadReq(): pMbxTfer = NULL is invalid!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    OsDbgAssert(eMbxTferStatus_Idle == pMbxTfer->eTferStatus);
    if ((pMbxTfer->MbxTferDesc.dwMaxDataLen < pMbxTfer->dwDataLen))
    {
        EC_ERRORMSGC(("CoeSdoUploadReq(): mismatching data length pMbxTfer->MbxTferDesc.dwMaxDataLen = %d, pMbxTfer->dwDataLen = %d!\n", pMbxTfer->MbxTferDesc.dwMaxDataLen, pMbxTfer->dwDataLen));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (!CEcMasterInterfaceBase::IsValidCoeSdoParms(wObIndex, byObSubIndex, pMbxTfer->pbyMbxTferData, pMbxTfer->dwDataLen, dwTimeout, dwFlags))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_COE_SDO_UPLOAD;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_CANOPEN_SDO;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_CANOPEN;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ETHERCAT_CANOPEN_TYPE_SDORES;
    pMbxTferPriv->oEcMbxCmd.sIndex.index      = wObIndex;
    pMbxTferPriv->oEcMbxCmd.sIndex.subIndex   = byObSubIndex;
    pMbxTferPriv->oEcMbxCmd.dwMailboxFlags    = dwFlags;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_FALSE);

Exit:
    return OsSetLastError(dwRetVal);
}


/***************************************************************************************************/
/** \brief Initiate CoE Object Dictionary request transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::CoeGetODList
(   EC_T_MBXTFER* pMbxTfer,         /**< [in] mailbox transfer object */
    EC_T_DWORD dwSlaveId,           /**< [in] slave id to determine the OD list */
    EC_T_COE_ODLIST_TYPE eListType, /**< [in] which object types shall be transferred */
    EC_T_DWORD  dwTimeout )         /**< [in] Timeout in milliseconds */
{
    EC_T_MBXTFERP*  pMbxTferPriv;
    EC_T_DWORD      dwRetVal;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("CoeGetODList: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("CoeGetODList: pMbxTfer missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_COE_GETODLIST;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_CANOPEN_SDO_INFO_LIST;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_CANOPEN;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ETHERCAT_CANOPEN_TYPE_SDOINFO;
    pMbxTferPriv->oEcMbxCmd.sIndex.index      = (EC_T_WORD)eListType;

    /* send mailbox request to the master */
    if (EC_NOWAIT == dwTimeout)
    {
        /* legacy support */
        dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, EC_MBX_DEFAULT_TIMEOUT, EC_FALSE);
    }
    else
    {
        dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    }

Exit:
    return OsSetLastError(dwRetVal);
}


/***************************************************************************************************/
/** \brief Initiate CoE Get Object Description transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::CoeGetObjectDesc
(   EC_T_MBXTFER* pMbxTfer,     /**< [in] mailbox transfer object */
    EC_T_DWORD dwSlaveId,       /**< [in] slave id to download the SDO */
    EC_T_WORD wObIndex,         /**< [in] object index */
    EC_T_DWORD  dwTimeout )     /**< [in] Timeout in milliseconds */
{
    EC_T_MBXTFERP*  pMbxTferPriv;
    EC_T_DWORD      dwRetVal;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("CoeGetObjectDesc: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("CoeGetODList: pMbxTfer missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* prepare the mailbox transfer descriptor */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_COE_GETOBDESC;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_CANOPEN_SDO_INFO_OBJ;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_CANOPEN;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ETHERCAT_CANOPEN_TYPE_SDOINFO;
    pMbxTferPriv->oEcMbxCmd.sIndex.index      = wObIndex;

    /* send mailbox request to the master */
    if (EC_NOWAIT == dwTimeout)
    {
        /* legacy support */
        dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, EC_MBX_DEFAULT_TIMEOUT, EC_FALSE);
    }
    else
    {
        dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    }

Exit:
    return OsSetLastError(dwRetVal);
}



/***************************************************************************************************/
/** \brief Initiate CoE Get Object Entry Description transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::CoeGetEntryDesc
(   EC_T_MBXTFER* pMbxTfer,     /**< [in] mailbox transfer object */
    EC_T_DWORD dwSlaveId,       /**< [in] slave id to download the SDO */
    EC_T_WORD wObIndex,         /**< [in] object index */
    EC_T_BYTE byObSubIndex,     /**< [in] object sub-index */
    EC_T_BYTE byValueInfo,      /**< [in] value info selection */
    EC_T_DWORD  dwTimeout )     /**< [in] Timeout in milliseconds */
{
    EC_T_MBXTFERP*  pMbxTferPriv;
    EC_T_DWORD      dwRetVal;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("CoeGetEntryDesc: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("CoeGetODList: pMbxTfer missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_COE_GETENTRYDESC;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_CANOPEN_SDO_INFO_ENTRY;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_CANOPEN;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ETHERCAT_CANOPEN_TYPE_SDOINFO;
    pMbxTferPriv->oEcMbxCmd.sIndex.index      = wObIndex;
    pMbxTferPriv->oEcMbxCmd.sIndex.subIndex   = byObSubIndex;
    pMbxTferPriv->oEcMbxCmd.sIndex.valueInfo  = byValueInfo;

    /* send mailbox request to the master */
    if (EC_NOWAIT == dwTimeout)
    {
        /* legacy support */
        dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, EC_MBX_DEFAULT_TIMEOUT, EC_FALSE);
    }
    else
    {
        dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    }

Exit:
    return OsSetLastError(dwRetVal);
}


#if (defined INCLUDE_COE_PDO_SUPPORT)
/***************************************************************************************************/
/** \brief Initiate CoE RxPDO transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::CoeRxPdoTfer
(   EC_T_MBXTFER* pMbxTfer,     /**< [in]  mailbox transfer object */
    EC_T_DWORD dwSlaveId,       /**< [in]  slave id to download the SDO */
    EC_T_DWORD dwNumber,        /**< [in]  PDO number */
    EC_T_DWORD  dwTimeout )     /**< [in]  Timeout in milliseconds */
{
    EC_T_MBXTFERP*  pMbxTferPriv;
    EC_T_DWORD      dwRetVal;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("CoeRxPdoTfer: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_COE_RX_PDO;

    /* prepare mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->pEcMbxCmd->cmdId          = MAILBOXCMD_CANOPEN_RX_PDO;
    pMbxTferPriv->pEcMbxCmd->wMbxCmdType    = EC_MAILBOX_CMD_DOWNLOAD;
    pMbxTferPriv->pEcMbxCmd->wMbxType       = ETHERCAT_MBOX_TYPE_CANOPEN;
    pMbxTferPriv->pEcMbxCmd->wMbxSubType    = ETHERCAT_CANOPEN_TYPE_RXPDO;
    pMbxTferPriv->pEcMbxCmd->dwNumber       = dwNumber;

    /* send RxPDO request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout );

Exit:
    return OsSetLastError(dwRetVal);
}
#endif /* INCLUDE_COE_PDO_SUPPORT */

#if (defined INCLUDE_SOE_SUPPORT)
/***************************************************************************************************/
/** \brief Soe write transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/

EC_T_DWORD CAtEmInterface::SoeWrite(
    EC_T_DWORD  dwSlaveId         /**< [in]  ethercat slave ID */
   ,EC_T_BYTE   byDriveNo         /**< [in]  number of drive in slave */
   ,EC_T_BYTE*  pbyElementFlags   /**< [in]  which element of object is written */
   ,EC_T_WORD   wIDN              /**< [in]  IDN of the object */
   ,EC_T_BYTE*  pbyData           /**< [in]  data of the element to be written */
   ,EC_T_DWORD  dwDataLen         /**< [in]  length of pbyData buffer */
   ,EC_T_DWORD  dwTimeout         /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
   )
{
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv    = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatSoeWrite: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (!CEcMasterInterfaceBase::IsValidSoeTransferParms(byDriveNo, pbyElementFlags, wIDN,
        pbyData, dwDataLen, &dwDataLen, dwTimeout, &dwRetVal))
    {
        goto Exit;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = dwDataLen;
    oMbxTferDesc.pbyMbxTferDescData = (dwDataLen == 0)?EC_NULL:pbyData;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = 0;  /* no notification needed for simple API so don't include clientID */
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_SOE_WRITEREQUEST;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_SOE_WRITEREQUEST;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_SOE;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ECAT_SOE_OPCODE_WRQ;
    pMbxTferPriv->oEcMbxCmd.wIDN              = wIDN;
    pMbxTferPriv->oEcMbxCmd.pbyElementFlags   = pbyElementFlags;
    pMbxTferPriv->oEcMbxCmd.byDriveNo         = byDriveNo;

    /* send SOE download request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }

    /* return mailbox transfer result */
    dwRetVal = pMbxTferPriv->MbxTfer.dwErrorCode;

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Initiate Soe write transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/

EC_T_DWORD CAtEmInterface::SoeWriteReq(
    EC_T_MBXTFER*   pMbxTfer,     /**< [in]  mailbox transfer object */
    EC_T_DWORD  dwSlaveId         /**< [in]  ethercat slave ID */
   ,EC_T_BYTE   byDriveNo         /**< [in]  number of drive in slave */
   ,EC_T_BYTE*  pbyElementFlags   /**< [in]  which element of object is written */
   ,EC_T_WORD   wIDN              /**< [in]  IDN of the object */
   ,EC_T_DWORD  dwTimeout         /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
   )
{
    EC_T_MBXTFERP*      pMbxTferPriv    = EC_NULL;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatSoeWriteReq: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (!CEcMasterInterfaceBase::IsValidSoeTransferParmsNonBlocking(pMbxTfer, byDriveNo, pbyElementFlags, wIDN,
        dwTimeout, &dwRetVal))
    {
        goto Exit;
    }

    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_SOE_WRITEREQUEST;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_SOE_WRITEREQUEST;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_SOE;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ECAT_SOE_OPCODE_WRQ;
    pMbxTferPriv->oEcMbxCmd.wIDN              = wIDN;
    pMbxTferPriv->oEcMbxCmd.pbyElementFlags   = pbyElementFlags;
    pMbxTferPriv->oEcMbxCmd.byDriveNo         = byDriveNo;

    /* send SOE download request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_FALSE);

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief SoE read transfer

\return Depends on the implementation of the function OsSetLastError(),
        normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::SoeRead(
    EC_T_DWORD  dwSlaveId,          /**< [in]  ethercat slave ID */
    EC_T_BYTE   byDriveNo,          /**< [in]  number of drive in slave */
    EC_T_BYTE*  pbyElementFlags,    /**< [in/out]  which element of object is read */
    EC_T_WORD   wIDN,               /**< [in]  IDN of the object */
    EC_T_BYTE*  pbyData,            /**< [out] IDN data */
    EC_T_DWORD  dwDataLen,          /**< [in]  length of pbyData buffer */
    EC_T_DWORD* pdwOutDataLen,      /**< [out] IDN data length (number of received bytes) */
    EC_T_DWORD  dwTimeout           /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
                                   )
{
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv    = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatSoeRead: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (!CEcMasterInterfaceBase::IsValidSoeTransferParms(byDriveNo, pbyElementFlags, wIDN,
        pbyData, dwDataLen, pdwOutDataLen, dwTimeout, &dwRetVal))
    {
        goto Exit;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = dwDataLen;
    oMbxTferDesc.pbyMbxTferDescData = (dwDataLen == 0)?EC_NULL:pbyData;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = 0;  /* no notification needed for simple API so don't include clientID */
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_SOE_READREQUEST;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_SOE_READREQUEST;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_SOE;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ECAT_SOE_OPCODE_RRQ;
    pMbxTferPriv->oEcMbxCmd.wIDN              = wIDN;
    pMbxTferPriv->oEcMbxCmd.pbyElementFlags   = pbyElementFlags;
    pMbxTferPriv->oEcMbxCmd.byDriveNo         = byDriveNo;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }

    EC_SETDWORD(pdwOutDataLen, EC_MIN(dwDataLen, pMbxTferPriv->MbxTfer.dwDataLen));

    /* return mailbox transfer result */
    dwRetVal = pMbxTferPriv->MbxTfer.dwErrorCode;

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Initiate SoE read transfer

\return Depends on the implementation of the function OsSetLastError(),
        normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::SoeReadReq(
    EC_T_MBXTFER*   pMbxTfer,           /**< [in]  mailbox transfer object */
    EC_T_DWORD      dwSlaveId,          /**< [in]  ethercat slave ID */
    EC_T_BYTE       byDriveNo,          /**< [in]  number of drive in slave */
    EC_T_BYTE*      pbyElementFlags,    /**< [in]  which element of object is read */
    EC_T_WORD       wIDN,               /**< [in]  IDN of the object */
    EC_T_DWORD      dwTimeout           /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
                                     )
{
    EC_T_MBXTFERP*      pMbxTferPriv    = EC_NULL;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatSoeReadReq: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (!CEcMasterInterfaceBase::IsValidSoeTransferParmsNonBlocking(pMbxTfer, byDriveNo, pbyElementFlags, wIDN,
        dwTimeout, &dwRetVal))
    {
        goto Exit;
    }

    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_SOE_READREQUEST;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_SOE_READREQUEST;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_SOE;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ECAT_SOE_OPCODE_RRQ;
    pMbxTferPriv->oEcMbxCmd.wIDN              = wIDN;
    pMbxTferPriv->oEcMbxCmd.pbyElementFlags   = pbyElementFlags;
    pMbxTferPriv->oEcMbxCmd.byDriveNo         = byDriveNo;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_FALSE);

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Abort a previous send procedure command

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/

EC_T_DWORD CAtEmInterface::SoeAbortProcCmd(
    EC_T_DWORD  dwSlaveId         /**< [in]  ethercat slave ID */
   ,EC_T_BYTE   byDriveNo         /**< [in]  number of drive in slave */
   ,EC_T_BYTE*  pbyElementFlags   /**< [in]  which element of object is written */
   ,EC_T_WORD   wIDN              /**< [in]  IDN of the object */
   ,EC_T_DWORD  dwTimeout         /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
   )
{
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv    = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatSoeAbortProcCmd: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (EC_NOWAIT == dwTimeout)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = 0;
    oMbxTferDesc.pbyMbxTferDescData = EC_NULL;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = 0;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = EC_NULL;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = 0;  /* no notification needed for simple API so don't include clientID */
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_SOE_WRITEREQUEST;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_SOE_ABORT_COMMAND;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_SOE;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ECAT_SOE_OPCODE_WRQ;
    pMbxTferPriv->oEcMbxCmd.wIDN              = wIDN;
    pMbxTferPriv->oEcMbxCmd.pbyElementFlags   = pbyElementFlags;
    pMbxTferPriv->oEcMbxCmd.byDriveNo         = byDriveNo;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }

    /* return mailbox transfer result */
    dwRetVal = pMbxTferPriv->MbxTfer.dwErrorCode;

Exit:
    return OsSetLastError(dwRetVal);
}
#endif /* INCLUDE_SOE_SUPPORT */


#if (defined INCLUDE_FOE_SUPPORT)
/***************************************************************************************************/
/** \brief Initiate Foe upload transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::FoeUploadReq(
    EC_T_MBXTFER* pMbxTfer,     /**< [in]    Pointer to the corresponding mailbox transfer object. */
    EC_T_DWORD dwSlaveId,       /**< [in]    EtherCAT slave ID. */
    EC_T_CHAR* achFileName,     /**< [in]    Name of slave file to read */
    EC_T_DWORD dwFileNameLen,   /**< [in]    Length of slave file name in bytes*/
    EC_T_DWORD dwPassword,      /**< [in]    Slave Password. */
    EC_T_DWORD dwTimeout        /**< [in]    Timeout in milliseconds. */
    )
{
    EC_T_MBXTFERP*  pMbxTferPriv;
    EC_T_DWORD      dwRetVal;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("FoeUploadReq: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("FoeUploadReq: pMbxTfer missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (dwFileNameLen > MAX_FILE_NAME_SIZE)
    {
        EC_ERRORMSGC(("FoeUploadReq: dwFileNameLen too long (set max %d, act %d)!\n", MAX_FILE_NAME_SIZE, dwFileNameLen));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_FOE_FILE_UPLOAD;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_FOE_FOPENREAD;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_FILEACCESS;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ECAT_FOE_OPCODE_RRQ;
    pMbxTferPriv->oEcMbxCmd.FoE.dwPassword    = dwPassword;
    pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength = dwFileNameLen;
    OsMemset(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, 0, sizeof(EC_T_CHAR) * MAX_FILE_NAME_SIZE);
    if (EC_NULL != achFileName)
    {
        OsMemcpy(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, achFileName, pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength);
    }

    /* prepare the mailbox data descriptor */
    pMbxTferPriv->MbxTfer.MbxData.FoE.dwTransferredBytes = 0;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_FALSE);

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief FoE upload transfer

\return Depends on the implementation of the function OsSetLastError(),
        normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::FoeFileUpload(
                                          EC_T_DWORD     dwSlaveId       /* [in]  slave ID */
                                         ,EC_T_CHAR*     achFileName     /* [in]  file name */
                                         ,EC_T_DWORD     dwFileNameLen   /* [in]  file name length */
                                         ,EC_T_BYTE*     pbyData         /* [in]  foe data buffer */
                                         ,EC_T_DWORD     dwDataLen       /* [in]  length of pbyData buffer */
                                         ,EC_T_DWORD*    pdwOutDataLen   /* [out] FOE data length (number of received bytes) */
                                         ,EC_T_DWORD     dwPassword      /* [in]  timeout in msec, must not be set to EC_NOWAIT */
                                         ,EC_T_DWORD     dwTimeout       /* [in]  mailbox transfer flags */
                                       )


{
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv    = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("FoEFileUpload: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (dwFileNameLen > MAX_FILE_NAME_SIZE)
    {
        EC_ERRORMSGC(("FoeFileUpload: dwFileNameLen too long (set max %d, act %d)!\n", MAX_FILE_NAME_SIZE, dwFileNameLen));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (EC_NOWAIT == dwTimeout)
    {
        EC_ERRORMSGC(("FoeFileUpload: dwTimeout = EC_NOWAIT not supported!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = dwDataLen;
    oMbxTferDesc.pbyMbxTferDescData = (dwDataLen == 0)?EC_NULL:pbyData;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = 0;  /* no notification needed for simple API so don't include clientID */
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_FOE_FILE_UPLOAD;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_FOE_FOPENREAD;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_FILEACCESS;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ECAT_FOE_OPCODE_RRQ;
    pMbxTferPriv->oEcMbxCmd.FoE.dwPassword    = dwPassword;
    pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength = dwFileNameLen;
    OsMemset(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, 0, sizeof(EC_T_CHAR) * MAX_FILE_NAME_SIZE);
    if (EC_NULL != achFileName)
    {
        OsMemcpy(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, achFileName, pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength);
    }

    /* prepare the mailbox data descriptor */
    pMbxTferPriv->MbxTfer.MbxData.FoE.dwTransferredBytes = 0;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }

    EC_SETDWORD(pdwOutDataLen, EC_MIN(dwDataLen, pMbxTferPriv->MbxTfer.dwDataLen));

    /* return mailbox transfer result */
    dwRetVal = pMbxTferPriv->MbxTfer.dwErrorCode;

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Initiate Foe download transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::FoeDownloadReq(
    EC_T_MBXTFER* pMbxTfer,     /**< [in]    Pointer to the corresponding mailbox transfer object. */
    EC_T_DWORD dwSlaveId,       /**< [in]    EtherCAT slave ID. */
    EC_T_CHAR* achFileName,     /**< [in]    Name of slave file to read */
    EC_T_DWORD dwFileNameLen,   /**< [in]    Length of slave file name in bytes*/
    EC_T_DWORD dwPassword,      /**< [in]    Slave Password. */
    EC_T_DWORD dwTimeout        /**< [in]    Timeout in milliseconds. */
)
{
    EC_T_MBXTFERP*  pMbxTferPriv;
    EC_T_DWORD      dwRetVal;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("FoeDownloadReq: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("FoeDownloadReq: pMbxTfer missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (dwFileNameLen > MAX_FILE_NAME_SIZE)
    {
        EC_ERRORMSGC(("FoeDownloadReq: dwFileNameLen too long (set max %d, act %d)!\n", MAX_FILE_NAME_SIZE, dwFileNameLen));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_FOE_FILE_DOWNLOAD;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_FOE_FOPENWRITE;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_FILEACCESS;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ECAT_FOE_OPCODE_WRQ;
    pMbxTferPriv->oEcMbxCmd.FoE.dwPassword    = dwPassword;
    pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength = dwFileNameLen;
    OsMemset(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, 0, sizeof(EC_T_CHAR) * MAX_FILE_NAME_SIZE);
    if (EC_NULL != achFileName)
    {
        OsMemcpy(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, achFileName, pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength);
    }

    /* prepare the mailbox data descriptor */
    pMbxTferPriv->MbxTfer.MbxData.FoE.dwTransferredBytes = 0;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_FALSE);

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Initiate Foe download transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::FoeSegmentedUploadReq(
    EC_T_MBXTFER* pMbxTfer,     /**< [in]   Pointer to the corresponding mailbox transfer object. */
    EC_T_DWORD  dwSlaveId,      /**< [in]   EtherCAT slave ID. */
    EC_T_CHAR*  szFileName,     /**< [in]   file name at slave */
    EC_T_DWORD  dwFileNameLen,  /**< [in]   length of szFileName */
    EC_T_DWORD  dwFileSize,     /**< [in]   file size */
    EC_T_DWORD  dwPassword,     /**< [in]   Slave Password. */
    EC_T_DWORD  dwTimeout       /**< [in]   Timeout in milliseconds. */
)
{
    EC_T_MBXTFERP*  pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    EC_T_DWORD      dwRetVal     = EC_E_ERROR;
    EC_T_DWORD      dwRes        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("FoeSegmentedUploadReq: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("FoeSegmentedUploadReq: pMbxTfer missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if ((EC_NULL != szFileName) && (dwFileNameLen > MAX_FILE_NAME_SIZE))
    {
        EC_ERRORMSGC(("FoeSegmentedUploadReq: szDstFileName %s longer than %d bytes!\n", szFileName, MAX_FILE_NAME_SIZE));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (eMbxTferStatus_Pend == pMbxTferPriv->MbxTfer.eTferStatus)
    {
        EC_ERRORMSGC(("FoeSegmentedUploadReq: pMbxTfer currently owned by master!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    switch (pMbxTferPriv->MbxTfer.eTferStatus)
    {
    /* start FoE Download */
    case eMbxTferStatus_Idle:
    case eMbxTferStatus_TferDone:
    case eMbxTferStatus_TferReqError:
        {
            pMbxTferPriv->dwSlaveId = dwSlaveId;
            dwRes = m_oAtEcatDesc.poMaster->FoeSegmentedUploadStart(pMbxTferPriv, szFileName, dwFileNameLen, dwFileSize, dwPassword);
            if (EC_E_NOERROR != dwRes)
            {
                pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_TferReqError;
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_FALSE);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;

    /* continue FoE Download */
    case eMbxTferStatus_TferWaitingForContinue:
        {
            m_oAtEcatDesc.poMaster->FoeSegmentedUploadContinue(pMbxTferPriv);
        } break;

    case eMbxTferStatus_Pend:
    case eMbxTferStatus_BCppDummy:
    default:
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Initiate Foe download transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::FoeSegmentedDownloadReq(
    EC_T_MBXTFER* pMbxTfer,     /**< [in]   Pointer to the corresponding mailbox transfer object. */
    EC_T_DWORD  dwSlaveId,      /**< [in]   EtherCAT slave ID. */
    EC_T_CHAR*  szFileName,     /**< [in]   file name at slave */
    EC_T_DWORD  dwFileNameLen,  /**< [in]   length of szFileName */
    EC_T_DWORD  dwFileSize,     /**< [in]   file size */
    EC_T_DWORD  dwPassword,     /**< [in]   Slave Password. */
    EC_T_DWORD  dwTimeout       /**< [in]   Timeout in milliseconds. */
)
{
    EC_T_MBXTFERP*  pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    EC_T_DWORD      dwRetVal     = EC_E_ERROR;
    EC_T_DWORD      dwRes        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("FoeSegmentedDownloadReq: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("FoeSegmentedDownloadReq: pMbxTfer missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if ((EC_NULL != szFileName) && (dwFileNameLen > MAX_FILE_NAME_SIZE))
    {
        EC_ERRORMSGC(("FoeSegmentedDownloadReq: szDstFileName %s longer than %d bytes!\n", szFileName, MAX_FILE_NAME_SIZE));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (eMbxTferStatus_Pend == pMbxTferPriv->MbxTfer.eTferStatus)
    {
        EC_ERRORMSGC(("FoeSegmentedDownloadReq: pMbxTfer currently owned by master!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    switch (pMbxTferPriv->MbxTfer.eTferStatus)
    {
    /* start FoE Download */
    case eMbxTferStatus_Idle:
    case eMbxTferStatus_TferDone:
    case eMbxTferStatus_TferReqError:
        {
            pMbxTferPriv->dwSlaveId = dwSlaveId;
            dwRes = m_oAtEcatDesc.poMaster->FoeSegmentedDownloadStart(pMbxTferPriv, szFileName, dwFileNameLen, dwFileSize, dwPassword);
            if (EC_E_NOERROR != dwRes)
            {
                pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_TferReqError;
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_FALSE);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;

    /* continue FoE Download */
    case eMbxTferStatus_TferWaitingForContinue:
        {
            m_oAtEcatDesc.poMaster->FoeSegmentedDownloadContinue(pMbxTferPriv);
        } break;

    case eMbxTferStatus_Pend:
    case eMbxTferStatus_BCppDummy:
    default:
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Initiate FoE File download transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::FoeFileDownload(
     EC_T_DWORD     dwSlaveId,      /* [in]  slave ID */
     EC_T_CHAR*     achFileName,    /* [in]  file name */
     EC_T_DWORD     dwFileNameLen,  /* [in]  file name length */
     EC_T_BYTE*     pbyData,        /* [in]  foe data buffer */
     EC_T_DWORD     dwDataLen,      /* [in]  length of pbyData buffer */
     EC_T_DWORD     dwPassword,     /* [in]  timeout in msec, must not be set to EC_NOWAIT */
     EC_T_DWORD     dwTimeout       /* [in]  mailbox transfer flags */
)
{
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("FoeFileDownload: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (dwFileNameLen > MAX_FILE_NAME_SIZE)
    {
        EC_ERRORMSGC(("FoeFileDownload: dwFileNameLen too long (set max %d, act %d)!\n", MAX_FILE_NAME_SIZE, dwFileNameLen));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (EC_NOWAIT == dwTimeout)
    {
        EC_ERRORMSGC(("FoeFileDownload: dwTimeout = EC_NOWAIT not supported!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = dwDataLen;
    oMbxTferDesc.pbyMbxTferDescData = (dwDataLen == 0) ? EC_NULL : pbyData;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = 0;  /* no notification needed for simple API so don't include clientID */
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_FOE_FILE_DOWNLOAD;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_FOE_FOPENWRITE;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_FILEACCESS;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ECAT_FOE_OPCODE_WRQ;
    pMbxTferPriv->oEcMbxCmd.FoE.dwPassword    = dwPassword;
    pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength = dwFileNameLen;
    OsMemset(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, 0, sizeof(EC_T_CHAR) * MAX_FILE_NAME_SIZE);
    if (EC_NULL != achFileName)
    {
        OsMemcpy(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, achFileName, pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength);
    }

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }
    /* return mailbox transfer result */
    dwRetVal = pMbxTferPriv->MbxTfer.dwErrorCode;

Exit:
    return OsSetLastError(dwRetVal);
}
#endif /* INCLUDE_FOE_SUPPORT */


#if (defined INCLUDE_EOE_SUPPORT) && (defined INCLUDE_EOE_ENDPOINT)
/***************************************************************************************************/
/** \brief Register the EoE endpoint.

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/

EC_T_DWORD CAtEmInterface::EoeRegisterEndpoint(
    EC_T_CHAR*           szEoEDrvIdent      /**< [in]  String identifying the EoE endpoint (driver) */
   ,EC_T_LINK_DRV_DESC*  pLinkDrvDesc       /**< [out] Pointer to an empty link layer descriptor    */
                                              )
{
EC_T_DWORD    dwRetVal = EC_E_ERROR;
CEcEthSwitch* pSwitch  = EC_NULL;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("EoeRegisterEndpoint: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pSwitch = m_oAtEcatDesc.poEcDevice->GetReferenceToSwitch();
    if (EC_NULL == pSwitch)
    {
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }
    dwRetVal = CEcEoEUplink::EoELinkRegister(m_oAtEcatDesc.dwInstanceID, pSwitch, szEoEDrvIdent, pLinkDrvDesc);

Exit:
    return dwRetVal;
}
EC_T_DWORD CAtEmInterface::EoeUnregisterEndpoint(
    EC_T_LINK_DRV_DESC*  pLinkDrvDesc       /**< [out] Pointer to an empty link layer descriptor    */
                                              )
{
    return CEcEoEUplink::EoELinkUnregister(pLinkDrvDesc);
}
CEcEthSwitch* CEcEthSwitch::GetInstance(EC_T_DWORD dwInstanceID)
{
    CAtEmInterface* pEmInst = EC_NULL;
    CEcEthSwitch* pSwitch = EC_NULL;
    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        /* EC_E_INVALIDPARM */
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        /* EC_E_INVALIDSTATE */
        goto Exit;
    }
    pSwitch = S_aMultiMaster[dwInstanceID].pInst->m_oAtEcatDesc.poEcDevice->GetReferenceToSwitch();
    if (EC_NULL == pSwitch)
    {
        /* EC_E_NOTSUPPORTED */
        goto Exit;
    }

Exit:
    return pSwitch;
}

EC_T_VOID CEcEthSwitch::ReleaseInstance(CEcEthSwitch* pSwitch) { EC_UNREFPARM(pSwitch); }

#endif /* defined INCLUDE_EOE_ENDPOINT */


#if (defined INCLUDE_VOE_SUPPORT)
/***************************************************************************************************/
/** \brief VoE read transfer

\return Depends on the implementation of the function OsSetLastError(),
        normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::VoeRead(
     EC_T_DWORD     dwSlaveId       /* [in]  slave ID */
    ,EC_T_BYTE*     pbyData         /* [in]  VoE data buffer */
    ,EC_T_DWORD     dwDataLen       /* [in]  length of pbyData buffer */
    ,EC_T_DWORD*    pdwOutDataLen   /* [out] VOE data length (number of received bytes) */
    ,EC_T_DWORD     dwTimeout       /* [in]  mailbox transfer timeout */
)
{
    CEcTimer      oVoeReadTimeout;
    CEcSlave*           poSlave;
    EC_T_MBXTFERP*      pVoeMbxTferObj   = EC_NULL;
    EC_T_DWORD          dwRetVal         = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("VoeRead: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* parameter check */
    if (pbyData == EC_NULL || dwDataLen <= 0)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (!m_oAtEcatDesc.poMaster->IsConfigured())
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* get config slave */
    poSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);
    if (EC_NULL == poSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    if (!poSlave->IsPresentOnBus())
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    /* get the VoE mailbox transfer object of the specific slave */
    dwRetVal = m_oAtEcatDesc.poMaster->GetVoeMbxTferObjBySlaveId(dwSlaveId, (EC_T_VOID**)((EC_T_VOID*)&pVoeMbxTferObj));
    if(dwRetVal != EC_E_NOERROR)
    {
        goto Exit;
    }

    /* if a timeout was passed, wait here until VoE read timeout has elapsed or a VoE mailbox was received */
    if (dwTimeout != EC_NOWAIT)
    {
        oVoeReadTimeout.Start(dwTimeout);

        /* wait for answer of the VoE slave as long the upload timer has not elapsed */
        while (!oVoeReadTimeout.IsElapsed())
        {
            if(pVoeMbxTferObj->MbxTfer.eTferStatus == eMbxTferStatus_TferDone)
            {
                break;
            }
            OsSleep(1);
        }
    }

    /* check if a VoE mailbox was received. if this is the case we copy the received data into the application buffer and unlock the
       VoE mailbox object to receive the next VoE mailbox */
    if (pVoeMbxTferObj->MbxTfer.eTferStatus == eMbxTferStatus_TferDone)
    {
        /* the VoE slave has sent VoE mailbox data, which can now copied to the application buffer */
        OsMemcpy(pbyData, pVoeMbxTferObj->MbxTfer.MbxTferDesc.pbyMbxTferDescData, EC_MIN(dwDataLen, pVoeMbxTferObj->MbxTfer.dwDataLen));
        *pdwOutDataLen = EC_MIN(dwDataLen , pVoeMbxTferObj->MbxTfer.dwDataLen);
        dwRetVal = pVoeMbxTferObj->MbxTfer.dwErrorCode;

        /* now reset the VoE mailbox object to get ready for the next receive */
        OsMemset(pVoeMbxTferObj->MbxTfer.MbxTferDesc.pbyMbxTferDescData, 0, pVoeMbxTferObj->MbxTfer.MbxTferDesc.dwMaxDataLen);
        pVoeMbxTferObj->MbxTfer.dwErrorCode = EC_E_NOERROR;
        pVoeMbxTferObj->MbxTfer.dwClntId    = 0;  /* no notification needed for simple API so don't include clientID */
/*      pVoeMbxTferObj->MbxTfer.dwTferId    = dwSlaveId; * done once in CEcMbSlave constructor */

        /* unlock the VoE mailbox object to be able to receive the next VoE message */
        pVoeMbxTferObj->MbxTfer.eTferStatus = eMbxTferStatus_Idle;
    }
    else
    {
        dwRetVal = EC_E_VOE_NO_MBX_RECEIVED;
    }

    /* when the upload timer is elapsed that means we have received no VoE mailbox from the
       slave within the application timeout  */
    if (oVoeReadTimeout.IsElapsed() && (dwTimeout != EC_NOWAIT))
    {
        dwRetVal = EC_E_TIMEOUT;
        goto Exit;
    }

    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Initiate VoE write transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/

EC_T_DWORD CAtEmInterface::VoeWriteReq
(
    EC_T_MBXTFER* pMbxTfer,      /**< [in]    Pointer to the corresponding mailbox transfer object. */
    EC_T_DWORD dwSlaveId,        /**< [in]    EtherCAT slave ID. */
    EC_T_DWORD dwTimeout         /**< [in]    Timeout in milliseconds. */
)
{
    EC_T_MBXTFERP*  pMbxTferPriv = EC_NULL;
    EC_T_DWORD      dwRetVal     = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("VoeWriteReq: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* parameter check */
    if (pMbxTfer == EC_NULL)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_VOE_MBX_WRITE;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_VOE_WRITE;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_VOE;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_FALSE);

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief Initiate VoE MBX download transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CAtEmInterface::VoeWrite(
     EC_T_DWORD     dwSlaveId       /* [in]  slave ID */
    ,EC_T_BYTE*     pbyData         /* [in]  VoE data buffer */
    ,EC_T_DWORD     dwDataLen       /* [in]  length of pbyData buffer */
    ,EC_T_DWORD     dwTimeout       /* [in]  mailbox transfer flags */
                                         )
{
    EC_T_MBXTFERP  oMbxTfer;
    EC_T_MBXTFER*  pMbxTfer = (EC_T_MBXTFER*)&oMbxTfer;
    EC_T_DWORD     dwRetVal = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("VoeWrite: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* parameter check */
    if (EC_NOWAIT == dwTimeout || pbyData == EC_NULL)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    OsMemset(pMbxTfer, 0, sizeof(EC_T_MBXTFER));

    /* inititalize the mailbox transfer descriptor */
    pMbxTfer->MbxTferDesc.pbyMbxTferDescData = pbyData;
    pMbxTfer->MbxTferDesc.dwMaxDataLen       = dwDataLen;

    /* inititalize the mailbox transfer object */
    pMbxTfer->dwDataLen         = pMbxTfer->MbxTferDesc.dwMaxDataLen;
    pMbxTfer->pbyMbxTferData    = pMbxTfer->MbxTferDesc.pbyMbxTferDescData;
    pMbxTfer->eTferStatus       = eMbxTferStatus_Idle;
    pMbxTfer->dwErrorCode       = EC_E_NOERROR;
    pMbxTfer->dwClntId          = 0;
    pMbxTfer->dwDataLen         = dwDataLen;
    pMbxTfer->dwTferId          = 0;
    pMbxTfer->eMbxTferType = eMbxTferType_VOE_MBX_WRITE;

    /* prepare the mailbox command descriptor */
    OsMemset(&oMbxTfer.oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    oMbxTfer.oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_VOE_WRITE;
    oMbxTfer.oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;
    oMbxTfer.oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_VOE;

    /* start the mailbox transfer */
    dwRetVal = MbxTfer(&oMbxTfer, dwSlaveId, dwTimeout, EC_TRUE);
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }
    /* return mailbox transfer result */
    dwRetVal = pMbxTfer->dwErrorCode;

Exit:
    return OsSetLastError(dwRetVal);
}
#endif /* INCLUDE_VOE_SUPPORT */


#if (defined INCLUDE_AOE_SUPPORT)
/***************************************************************************************************/
/**
\brief  Retrieves the NetID of a specific EtherCAT device
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::AoeGetSlaveNetId(
    EC_T_DWORD              dwSlaveId,      /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID*         poAoeNetId      /**< [out]  AoE NetID of the corresponding slave */
    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("AoeGetSlaveNetId: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check if ADS adapter is running */
    if (AdsIsRunning())
    {
        dwRetVal = EC_E_ADS_IS_RUNNING;
        goto Exit;
    }

    dwRetVal = m_oAtEcatDesc.poMaster->GetSlaveNetId(dwSlaveId, poAoeNetId);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Performs a AoE mailbox read request to an EtherCAT slave device.
A non blocking version of this function (ecatAoeReadReq) is also available.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
EC_T_DWORD CAtEmInterface::AoeRead(
    EC_T_DWORD  dwSlaveId,          /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId().*/
    EC_T_WORD   wTargetPort,        /**< [in]   Target port. */
    EC_T_DWORD  dwIndexGroup,       /**< [in]   AoE read command index group. */
    EC_T_DWORD  dwIndexOffset,      /**< [in]   AoE read command index offset*/
    EC_T_DWORD  dwDataLen,          /**< [in]   Number of bytes to read from the target device. Must correspond with the size of pbyData. */
    EC_T_BYTE*  pbyData,            /**< [in]   Data buffer to store the read data */
    EC_T_DWORD* pdwDataOutLen,      /**< [out]  Number of bytes read from the target device */
    EC_T_DWORD* pdwErrorCode,       /**< [out]  AoE response error code*/
    EC_T_DWORD* pdwCmdResult,       /**< [out]  AoE read command result code */
    EC_T_DWORD  dwTimeout           /**< [in]   Timeout in milliseconds. The function will block at most for this time */
    )
{
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("AoeRead: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if ((EC_NOWAIT == dwTimeout) || (0 == dwDataLen) || (EC_NULL == pbyData) 
        || (EC_NULL == pdwDataOutLen) || (EC_NULL == pdwErrorCode) || (EC_NULL == pdwCmdResult))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = dwDataLen;
    oMbxTferDesc.pbyMbxTferDescData = (dwDataLen == 0)?EC_NULL:pbyData;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = 0;  /* no notification needed for simple API so don't include clientID */
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_AOE_READ;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_AOE_READ;    /* internal: AoE command id */
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;       /* internal */
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_ADS;      /* mailbox header: type ADS over Ethercat (0x1) */
    pMbxTferPriv->oEcMbxCmd.wAoeCmdId         = ECAT_AOEHDR_COMMANDID_READ;  /* AoE header: command id (0x2) */
    OsMemcpy(pMbxTferPriv->oEcMbxCmd.oTargetNetId, poTargetNetId, 6);        /* AoE header: net id */
    pMbxTferPriv->oEcMbxCmd.wTargetPort       = wTargetPort;                 /* AoE header: port */
    pMbxTferPriv->oEcMbxCmd.dwIndexGroup      = dwIndexGroup;                /* AoE header: index group */
    pMbxTferPriv->oEcMbxCmd.dwIndexOffset     = dwIndexOffset;               /* AoE header: index offset */

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
	
    EC_SETDWORD(pdwDataOutLen, EC_MIN(dwDataLen, pMbxTferPriv->MbxTfer.dwDataLen));
    EC_SETDWORD(pdwErrorCode, pMbxTferPriv->MbxTfer.MbxData.AoE_Response.dwErrorCode);
    EC_SETDWORD(pdwCmdResult, pMbxTferPriv->MbxTfer.MbxData.AoE_Response.dwCmdResult);

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/**
\brief  Performs a AoE mailbox read request to an EtherCAT slave device.
If the timeout value was set to EC_NOWAIT this function returns immediately without blocking.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
EC_T_DWORD CAtEmInterface::AoeReadReq
(
    EC_T_MBXTFER*   pMbxTfer,       /**< [in]   Pointer to the corresponding mailbox transfer object. The data to write have to be stored at pMbxTfer->pbyData. */
    EC_T_DWORD      dwSlaveId,      /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used.*/
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId(). */
    EC_T_WORD       wTargetPort,    /**< [in]   Target port.  */
    EC_T_DWORD      dwIndexGroup,   /**< [in]   AoE read command index group */
    EC_T_DWORD      dwIndexOffset,  /**< [in]   AoE read command index offset*/
    EC_T_DWORD      dwTimeout       /**< [in]   Timeout in milliseconds. The function will block at most for this time.*/
    )

{
    EC_T_MBXTFERP*  pMbxTferPriv    = EC_NULL;
    EC_T_DWORD      dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("AoeReadReq: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("AoeReadReq: pMbxTfer missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_AOE_READ;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_AOE_READ;             /* internal: AoE command id */
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;                /* internal */
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_ADS;               /* mailbox header: type ADS over Ethercat (0x1) */
    pMbxTferPriv->oEcMbxCmd.wAoeCmdId         = ECAT_AOEHDR_COMMANDID_READ;           /* AoE header: command id (0x2) */
    OsMemcpy(pMbxTferPriv->oEcMbxCmd.oTargetNetId, poTargetNetId, 6);                 /* AoE header: net id */
    pMbxTferPriv->oEcMbxCmd.wTargetPort       = wTargetPort;                          /* AoE header: port */
    pMbxTferPriv->oEcMbxCmd.dwIndexGroup      = dwIndexGroup;                         /* AoE header: index group */
    pMbxTferPriv->oEcMbxCmd.dwIndexOffset     = dwIndexOffset;                        /* AoE header: index offset */

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_FALSE);

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/**
\brief  Performs a AoE mailbox write request to an EtherCAT slave device.
A non blocking version of this function (ecatAoeWriteReq) is also available.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
EC_T_DWORD CAtEmInterface::AoeWrite(
    EC_T_DWORD  dwSlaveId,          /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId(). */
    EC_T_WORD   wTargetPort,        /**< [in]   Target port.  */
    EC_T_DWORD  dwIndexGroup,       /**< [in]   AoE write command index group. */
    EC_T_DWORD  dwIndexOffset,      /**< [in]   AoE write command index offset*/
    EC_T_DWORD  dwDataLen,          /**< [in]   Number of bytes to write to the target device. Must correspond with the size of pbyData.*/
    EC_T_BYTE*  pbyData,            /**< [in]   Data buffer with data that shall be written*/
    EC_T_DWORD* pdwErrorCode,       /**< [out]  AoE response error code. */
    EC_T_DWORD* pdwCmdResult,       /**< [out]  AoE write command result code. */
    EC_T_DWORD  dwTimeout           /**< [in]   Timeout in milliseconds. The function will block at most for this time. */
    )
{
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("AoeWrite: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if ((EC_NOWAIT == dwTimeout) || (0 == dwDataLen) || (EC_NULL == pbyData)
        || (EC_NULL == pdwErrorCode) || (EC_NULL == pdwCmdResult))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = dwDataLen;
    oMbxTferDesc.pbyMbxTferDescData = (dwDataLen == 0)?EC_NULL:pbyData;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = 0;  /* no notification needed for simple API so don't include clientID */
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_AOE_WRITE;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_AOE_WRITE;   /* internal: AoE command id */
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;     /* internal */
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_ADS;      /* mailbox header: type ADS over Ethercat (0x1) */
    pMbxTferPriv->oEcMbxCmd.wAoeCmdId         = ECAT_AOEHDR_COMMANDID_WRITE; /* AoE header: command id (0x2) */
    OsMemcpy(pMbxTferPriv->oEcMbxCmd.oTargetNetId, poTargetNetId, 6);        /* AoE header: net id */
    pMbxTferPriv->oEcMbxCmd.wTargetPort       = wTargetPort;                 /* AoE header: port */
    pMbxTferPriv->oEcMbxCmd.dwIndexGroup      = dwIndexGroup;                /* AoE header: index group */
    pMbxTferPriv->oEcMbxCmd.dwIndexOffset     = dwIndexOffset;               /* AoE header: index offset */

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    
    EC_SETDWORD(pdwErrorCode, pMbxTferPriv->MbxTfer.MbxData.AoE_Response.dwErrorCode);
    EC_SETDWORD(pdwCmdResult, pMbxTferPriv->MbxTfer.MbxData.AoE_Response.dwCmdResult);

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/**
\brief  Performs a AoE mailbox write request to an EtherCAT slave device.
If the timeout value was set to EC_NOWAIT this function returns immediately without blocking.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
EC_T_DWORD CAtEmInterface::AoeWriteReq
(
    EC_T_MBXTFER* pMbxTfer,         /**< [in]   Pointer to the corresponding mailbox transfer object. The data to write have to be stored at pMbxTfer->pbyData. */
    EC_T_DWORD  dwSlaveId,          /**< [in]   [in]    EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId(). */
    EC_T_WORD   wTargetPort,        /**< [in]   Target port.  */
    EC_T_DWORD  dwIndexGroup,       /**< [in]   AoE write command index group. */
    EC_T_DWORD  dwIndexOffset,      /**< [in]   AoE write command index offset*/
    EC_T_DWORD  dwTimeout           /**< [in]   Timeout in milliseconds. The function will block at most for this time. */
    )

{
    EC_T_MBXTFERP*  pMbxTferPriv    = EC_NULL;
    EC_T_DWORD      dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("AoeWriteReq: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pMbxTfer)
    {
        EC_ERRORMSGC(("AoeWriteReq: pMbxTfer missing!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* prepare the mailbox transfer object */
    pMbxTferPriv = (EC_T_MBXTFERP*)pMbxTfer;
    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_AOE_WRITE;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_AOE_WRITE;            /* internal: AoE command id */
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;              /* internal */
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_ADS;               /* mailbox header: type ADS over Ethercat (0x1) */
    pMbxTferPriv->oEcMbxCmd.wAoeCmdId         = ECAT_AOEHDR_COMMANDID_WRITE;          /* AoE header: command id (0x2) */
    OsMemcpy(pMbxTferPriv->oEcMbxCmd.oTargetNetId, poTargetNetId, 6);                 /* AoE header: net id */
    pMbxTferPriv->oEcMbxCmd.wTargetPort       = wTargetPort;                          /* AoE header: port */
    pMbxTferPriv->oEcMbxCmd.dwIndexGroup      = dwIndexGroup;                         /* AoE header: index group */
    pMbxTferPriv->oEcMbxCmd.dwIndexOffset     = dwIndexOffset;                        /* AoE header: index offset */

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_FALSE);

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/**
\brief  Performs a AoE mailbox read/write request to an EtherCAT slave device.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
EC_T_DWORD CAtEmInterface::AoeReadWrite(
    EC_T_DWORD  dwSlaveId,          /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId().*/
    EC_T_WORD   wTargetPort,        /**< [in]   Target port. */
    EC_T_DWORD  dwIndexGroup,       /**< [in]   AoE read/write command index group. */
    EC_T_DWORD  dwIndexOffset,      /**< [in]   AoE read/write command index offset*/
    EC_T_DWORD  dwReadDataLen,      /**< [in]   Number of bytes to read from the target device. Must correspond with the size of pbyData. */
    EC_T_DWORD  dwWriteDataLen,     /**< [in]   Number of bytes to write to the target device. Must correspond with the size of pbyData. */
    EC_T_BYTE*  pbyData,            /**< [in]   Data buffer to store the read data */
    EC_T_DWORD* pdwDataOutLen,      /**< [out]  Number of bytes read from the target device */
    EC_T_DWORD* pdwErrorCode,       /**< [out]  AoE response error code*/
    EC_T_DWORD* pdwCmdResult,       /**< [out]  AoE read/write command result code */
    EC_T_DWORD  dwTimeout           /**< [in]   Timeout in milliseconds. The function will block at most for this time */
    )
{
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("AoeReadWrite: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if ((EC_NOWAIT == dwTimeout) || (0 == dwReadDataLen) || (EC_NULL == pbyData)
        || (EC_NULL == pdwDataOutLen) || (EC_NULL == pdwErrorCode) || (EC_NULL == pdwCmdResult))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = dwWriteDataLen;
    oMbxTferDesc.pbyMbxTferDescData = pbyData;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = 0;  /* no notification needed for simple API so don't include clientID */
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_AOE_READWRITE;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_AOE_READWRITE;    /* internal: AoE command id */
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;       /* internal */
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_ADS;      /* mailbox header: type ADS over Ethercat (0x1) */
    pMbxTferPriv->oEcMbxCmd.wAoeCmdId         = ECAT_AOEHDR_COMMANDID_READWRITE;  /* AoE header: command id */
    OsMemcpy(pMbxTferPriv->oEcMbxCmd.oTargetNetId, poTargetNetId, 6);        /* AoE header: net id */
    pMbxTferPriv->oEcMbxCmd.wTargetPort       = wTargetPort;                 /* AoE header: port */
    pMbxTferPriv->oEcMbxCmd.dwIndexGroup      = dwIndexGroup;                /* AoE header: index group */
    pMbxTferPriv->oEcMbxCmd.dwIndexOffset     = dwIndexOffset;               /* AoE header: index offset */
    pMbxTferPriv->oEcMbxCmd.dwAoeReadLength   = dwReadDataLen;

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);

    EC_SETDWORD(pdwDataOutLen, pMbxTferPriv->MbxTfer.dwDataLen);
    EC_SETDWORD(pdwErrorCode, pMbxTferPriv->MbxTfer.MbxData.AoE_Response.dwErrorCode);
    EC_SETDWORD(pdwCmdResult, pMbxTferPriv->MbxTfer.MbxData.AoE_Response.dwCmdResult);

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/**
\brief  Performs a AoE mailbox write control request to an EtherCAT slave device.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
EC_T_DWORD CAtEmInterface::AoeWriteControl(
    EC_T_DWORD  dwSlaveId,          /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId().*/
    EC_T_WORD   wTargetPort,        /**< [in]   Target port. */
    EC_T_WORD   wAoEState,          /**< [in]   AoE state */
    EC_T_WORD   wDeviceState,       /**< [in]   Device specific state */
    EC_T_DWORD  dwDataLen,          /**< [in]   Number of bytes to write to the target device. Must correspond with the size of pbyData.*/
    EC_T_BYTE*  pbyData,            /**< [in]   Data buffer with data that shall be written*/
    EC_T_DWORD* pdwErrorCode,       /**< [out]  AoE response error code. */
    EC_T_DWORD* pdwCmdResult,       /**< [out]  AoE write command result code. */
    EC_T_DWORD  dwTimeout           /**< [in]   Timeout in milliseconds. The function will block at most for this time. */
    )
{
    EC_T_MBXTFERP       oMbxTferPriv;
    EC_T_MBXTFERP*      pMbxTferPriv = &oMbxTferPriv;
    EC_T_MBXTFER_DESC   oMbxTferDesc;
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("AoeReadWrite: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (EC_NOWAIT == dwTimeout)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = dwDataLen;
    oMbxTferDesc.pbyMbxTferDescData = pbyData;

    /* create the mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_Idle;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    /* prepare the mailbox transfer object */
    pMbxTferPriv->MbxTfer.dwClntId       = 0;  /* no notification needed for simple API so don't include clientID */
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_AOE_WRITECONTROL;

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_AOE_WRITECONTROL; /* internal: AoE command id */
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;      /* internal */
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_ADS;     /* mailbox header: type ADS over Ethercat (0x1) */
    pMbxTferPriv->oEcMbxCmd.wAoeCmdId         = ECAT_AOEHDR_COMMANDID_WRITECONTROL; /* AoE header: command id */
    OsMemcpy(pMbxTferPriv->oEcMbxCmd.oTargetNetId, poTargetNetId, 6);       /* AoE header: net id */
    pMbxTferPriv->oEcMbxCmd.wTargetPort       = wTargetPort;                /* AoE header: port */
    pMbxTferPriv->oEcMbxCmd.wAoEState         = wAoEState;                  /* AoE header: AoE state */
    pMbxTferPriv->oEcMbxCmd.wDeviceState      = wDeviceState;               /* AoE header: device specific state */

    /* send mailbox request to the master */
    dwRetVal = MbxTfer(pMbxTferPriv, dwSlaveId, dwTimeout, EC_TRUE);
    
    EC_SETDWORD(pdwErrorCode, pMbxTferPriv->MbxTfer.MbxData.AoE_Response.dwErrorCode);
    EC_SETDWORD(pdwCmdResult, pMbxTferPriv->MbxTfer.MbxData.AoE_Response.dwCmdResult);

Exit:
    return OsSetLastError(dwRetVal);
}
#endif /* INCLUDE_AOE_SUPPORT */


/***************************************************************************************************/
/**
\brief  Returns total amount of currently connected Slaves.

BRD AlStatus Read WKC Value.
\return WKC Value.
*/
EC_T_DWORD CAtEmInterface::GetNumConnectedSlaves(EC_T_VOID )
{
    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        return 0;
    }
    return m_oAtEcatDesc.poMaster->GetScanBusSlaveCount();
}

#if (defined INCLUDE_RED_DEVICE)
/***************************************************************************************************/
/**
\brief  Returns amount of currently connected Slaves to main side

BRD AlStatus Read WKC Value.
\return WKC Value.
*/
EC_T_DWORD CAtEmInterface::GetNumConnectedSlavesMain(EC_T_VOID )
{
    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        return 0;
    }
    if (m_oAtEcatDesc.poMaster->IsRedConfigured())
    {
        return m_oAtEcatDesc.poMaster->m_wRedNumSlavesOnMain;
    }
    else
    {
        return m_oAtEcatDesc.poMaster->GetScanBusSlaveCount();
    }
}

/***************************************************************************************************/
/**
\brief  Returns amount of currently connected Slaves to red side

BRD AlStatus Read WKC Value.
\return WKC Value.
*/
EC_T_DWORD CAtEmInterface::GetNumConnectedSlavesRed(EC_T_VOID )
{
    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        return 0;           /* no slaves connected */
    }
    if (m_oAtEcatDesc.poMaster->IsRedConfigured())
    {
        return m_oAtEcatDesc.poMaster->m_wRedNumSlavesOnRed;
    }
    else
    {
        return 0;
    }
}
#endif /* INCLUDE_RED_DEVICE */


/***************************************************************************************************/
/** \brief Mailbox transfer notification

Function called by master on transfer progress or finish.

\return
    - EC_E_NOERROR
    - EC_E_CANCEL: master should abort current mbx transfer
*/
EC_T_DWORD CAtEmInterface::MboxNotify(
    PEcMailboxCmd   pCmd
   ,EC_T_BYTE*      pbyMbxData
   ,EC_T_DWORD      dwMbxDataLength
   ,EC_T_DWORD      dwMbxResult
)
{
EC_T_DWORD     dwRetVal       = EC_E_ERROR;
EC_T_MBXTFERP* pMbxTferPriv   = (EC_T_MBXTFERP*)(pCmd->pvContext);
EC_T_BOOL      bTferDone      = EC_TRUE;
EC_T_BOOL      bTferAbort     = EC_FALSE;
EC_T_BOOL      bTferNotifyAll = EC_FALSE;
EC_T_BOOL      bTferIdle      = EC_FALSE;

    OsDbgAssert(m_oAtEcatDesc.bInitialized && (EC_NULL != m_oAtEcatDesc.poMaster));

    PerfMeasStart(&G_TscMeasDesc, EC_MSMT_MboxNotify);

    /* check parameter */
    if (EC_NULL == pMbxTferPriv)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* generate flags bTferDone, bTferAbort, bTferNotify, bTferIdle */
    bTferAbort = pMbxTferPriv->bAbort;
#if (defined INCLUDE_FOE_SUPPORT)
    if ((eMbxTferType_FOE_FILE_UPLOAD   == pMbxTferPriv->MbxTfer.eMbxTferType)
     || (eMbxTferType_FOE_FILE_DOWNLOAD == pMbxTferPriv->MbxTfer.eMbxTferType))
    {
        if ((EC_E_BUSY == dwMbxResult) && (EC_NULL == pbyMbxData))
        {
            bTferDone = EC_FALSE;
        }
        else if (EC_E_NOERROR == dwMbxResult)
        {
        }
    }
    if ((eMbxTferType_FOE_SEG_UPLOAD == pMbxTferPriv->MbxTfer.eMbxTferType)
        || (eMbxTferType_FOE_SEG_DOWNLOAD == pMbxTferPriv->MbxTfer.eMbxTferType))
    {
        if (EC_E_BUSY == dwMbxResult)
        {
            bTferDone = EC_FALSE;
            pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_TferWaitingForContinue;
        }
        else if (EC_E_NOERROR != dwMbxResult)
        {
            pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_TferReqError;
        }
        else
        {
            pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_TferDone;
        }
    }

#endif
    if ((eMbxTferType_COE_EMERGENCY    == pMbxTferPriv->MbxTfer.eMbxTferType)
#if (defined INCLUDE_SOE_SUPPORT)
     || (eMbxTferType_SOE_NOTIFICATION == pMbxTferPriv->MbxTfer.eMbxTferType)
     || (eMbxTferType_SOE_EMERGENCY    == pMbxTferPriv->MbxTfer.eMbxTferType)
#endif
      )
    {
        /* notify to all clients */
        bTferNotifyAll = EC_TRUE;
        bTferIdle      = EC_TRUE;
    }
#if (defined INCLUDE_VOE_SUPPORT)
    else if (eMbxTferType_VOE_MBX_READ == pMbxTferPriv->MbxTfer.eMbxTferType)
    {
        /* notify to all clients */
        bTferNotifyAll = EC_TRUE;
    }
#endif
    
    /* complete mailbox transfer object */
    {
    EC_T_BYTE* pbyEcMbxData = pbyMbxData;
    EC_T_WORD* pwEcMbxData  = (EC_T_WORD*)pbyEcMbxData;

        switch (pMbxTferPriv->MbxTfer.eMbxTferType)
        {
        case eMbxTferType_COE_SDO_DOWNLOAD:
        case eMbxTferType_COE_SDO_UPLOAD:
        case eMbxTferType_COE_GETODLIST:
        case eMbxTferType_COE_GETOBDESC:
        case eMbxTferType_COE_GETENTRYDESC:
        case eMbxTferType_COE_EMERGENCY:
#if (defined INCLUDE_COE_PDO_SUPPORT)
        case eMbxTferType_COE_RX_PDO:
#endif
            {
                /* fill EC_T_MBX_DATA.CoE */
                switch (pMbxTferPriv->MbxTfer.eMbxTferType)
                {
                case eMbxTferType_COE_SDO_DOWNLOAD:
                case eMbxTferType_COE_SDO_UPLOAD:
                {
                    
                    GetSlaveFixedAddr(pMbxTferPriv->dwSlaveId, &pMbxTferPriv->MbxTfer.MbxData.CoE.wStationAddress); // TODO dwRes
                    pMbxTferPriv->MbxTfer.MbxData.CoE.wIndex     = pMbxTferPriv->oEcMbxCmd.sIndex.index;
                    pMbxTferPriv->MbxTfer.MbxData.CoE.bySubIndex = pMbxTferPriv->oEcMbxCmd.sIndex.subIndex;
                    pMbxTferPriv->MbxTfer.MbxData.CoE.bCompleteAccess = (0 != (pMbxTferPriv->oEcMbxCmd.dwMailboxFlags & 1));
                } break;
                default: break;
                }

                if (dwMbxResult == EC_E_NOERROR)
                {
                    switch (pMbxTferPriv->MbxTfer.eMbxTferType)
                    {
                    case eMbxTferType_COE_SDO_DOWNLOAD:
                        break;
                    case eMbxTferType_COE_SDO_UPLOAD:
                        {
                            pMbxTferPriv->MbxTfer.dwDataLen = dwMbxDataLength;
                            if (pMbxTferPriv->MbxTfer.pbyMbxTferData != pbyEcMbxData)
                            {
                                OsMemcpy(pMbxTferPriv->MbxTfer.pbyMbxTferData, pbyEcMbxData, dwMbxDataLength);
                            }
                        }
                        break;
                    case eMbxTferType_COE_GETODLIST:
                        {
                            if (0 == dwMbxDataLength )
                            {
                                /* empty list */
                                dwMbxResult = EC_E_INVALIDSIZE;
                                /*
                                pMbxTferPriv->MbxTfer.dwDataLen = 0;
                                pMbxTferPriv->MbxTfer.pbyMbxTferData   = EC_NULL;
                                pMbxTferPriv->MbxTfer.MbxData.CoE_ODList.eOdListType      = (EC_T_COE_ODLIST_TYPE)0;
                                pMbxTferPriv->MbxTfer.MbxData.CoE_ODList.wLen             = 0;
                                pMbxTferPriv->MbxTfer.MbxData.CoE_ODList.pwOdList         = EC_NULL;
                                */
                            }
                            else if (dwMbxDataLength < sizeof(EC_T_WORD))
                            {
                                dwMbxResult = EC_E_INVALIDSIZE;
                            }
                            else if (1 == (dwMbxDataLength & 1))
                            {
                                /* length must be a multiple of 2 */
                                dwMbxResult = EC_E_INVALIDSIZE;
                            }
                            else if (dwMbxDataLength == sizeof(EC_T_WORD))
                            {
                                pMbxTferPriv->MbxTfer.dwDataLen                         = dwMbxDataLength;
                                if(pMbxTferPriv->MbxTfer.pbyMbxTferData != pbyEcMbxData)
                                {
                                    OsMemcpy(pMbxTferPriv->MbxTfer.pbyMbxTferData, pbyEcMbxData, dwMbxDataLength);
                                }
                                pMbxTferPriv->MbxTfer.MbxData.CoE_ODList.eOdListType    = (EC_T_COE_ODLIST_TYPE)EC_GET_FRM_WORD(&(pwEcMbxData[0]));
                                pMbxTferPriv->MbxTfer.MbxData.CoE_ODList.wLen           = 0;
                                pMbxTferPriv->MbxTfer.MbxData.CoE_ODList.pwOdList       = EC_NULL;
                            }
                            else
                            {
                                pMbxTferPriv->MbxTfer.dwDataLen                         = dwMbxDataLength;
                                if(pMbxTferPriv->MbxTfer.pbyMbxTferData != pbyEcMbxData)
                                {
                                    OsMemcpy(pMbxTferPriv->MbxTfer.pbyMbxTferData, pbyEcMbxData, dwMbxDataLength);
                                }
                                pMbxTferPriv->MbxTfer.MbxData.CoE_ODList.eOdListType    = (EC_T_COE_ODLIST_TYPE)EC_GET_FRM_WORD(&(pwEcMbxData[0]));
                                pMbxTferPriv->MbxTfer.MbxData.CoE_ODList.wLen           = (EC_T_WORD)(dwMbxDataLength/sizeof(EC_T_WORD) - 1);
                                pMbxTferPriv->MbxTfer.MbxData.CoE_ODList.pwOdList       = &pwEcMbxData[1];
#if (defined EC_BIG_ENDIAN)
                                for( EC_T_DWORD dwIdx = 1; dwIdx < (EC_T_DWORD)(pMbxTferPriv->MbxTfer.MbxData.CoE_ODList.wLen + 1); dwIdx++ )
                                {
                                    EC_SETWORD((&(pwEcMbxData[dwIdx])), EC_GET_FRM_WORD((&(pwEcMbxData[dwIdx]))));
                                }
#endif
                            }
                        }
                        break;
                    case eMbxTferType_COE_GETOBDESC:
                        if (dwMbxDataLength < 6 )
                        {
                            dwMbxResult = EC_E_INVALIDSIZE;
                        }
                        else
                        {
                            pMbxTferPriv->MbxTfer.dwDataLen = dwMbxDataLength;
                            if(pMbxTferPriv->MbxTfer.pbyMbxTferData != pbyEcMbxData)
                            {
                                OsMemcpy(pMbxTferPriv->MbxTfer.pbyMbxTferData, pbyEcMbxData, dwMbxDataLength);
                            }

                            pMbxTferPriv->MbxTfer.MbxData.CoE_ObDesc.wObIndex         = EC_GET_FRM_WORD(&(pwEcMbxData[0]));
                            pMbxTferPriv->MbxTfer.MbxData.CoE_ObDesc.wDataType        = EC_GET_FRM_WORD(&(pwEcMbxData[1]));
                            pMbxTferPriv->MbxTfer.MbxData.CoE_ObDesc.byMaxNumSubIndex = pbyEcMbxData[4];
                            pMbxTferPriv->MbxTfer.MbxData.CoE_ObDesc.byObjCode        = (EC_T_BYTE)(pbyEcMbxData[5] & 0x0F);
                            pMbxTferPriv->MbxTfer.MbxData.CoE_ObDesc.byObjCategory    = (EC_T_BYTE)((pbyEcMbxData[5] & 0x10) >> 4);
                            pMbxTferPriv->MbxTfer.MbxData.CoE_ObDesc.pchObName        = (EC_T_CHAR*)&pbyEcMbxData[6];
                            pMbxTferPriv->MbxTfer.MbxData.CoE_ObDesc.wObNameLen       = (EC_T_WORD)(pMbxTferPriv->MbxTfer.dwDataLen - 6);
                        }
                        break;
                    case eMbxTferType_COE_GETENTRYDESC:
                        if (dwMbxDataLength < 10 )
                        {
                            dwMbxResult = EC_E_INVALIDSIZE;
                        }
                        else
                        {
                        EC_T_DWORD dwIndex = 0;
                        EC_T_BYTE  byData  = 0;

                            pMbxTferPriv->MbxTfer.dwDataLen = dwMbxDataLength;
                            if(pMbxTferPriv->MbxTfer.pbyMbxTferData != pbyEcMbxData)
                            {
                                OsMemcpy(pMbxTferPriv->MbxTfer.pbyMbxTferData, pbyEcMbxData, dwMbxDataLength);
                            }
                            pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.wObIndex                = EC_GET_FRM_WORD(&(pbyEcMbxData[dwIndex])); dwIndex += 2;
                            pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.byObSubIndex            = pbyEcMbxData[dwIndex];                     dwIndex += 1;
                            pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.byValueInfo             = pbyEcMbxData[dwIndex];                     dwIndex += 1;
                            pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.wDataType               = EC_GET_FRM_WORD(&(pbyEcMbxData[dwIndex])); dwIndex += 2;
                            pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.wBitLen                 = EC_GET_FRM_WORD(&(pbyEcMbxData[dwIndex])); dwIndex += 2;
                            byData = *(EC_T_BYTE*)&(pbyEcMbxData[dwIndex]);                                                                  dwIndex += 1;
                            pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.byObAccess              = (EC_T_BYTE)(byData & 0x3F);
                            pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.bRxPdoMapping           = (byData & 0x40) != 0;
                            pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.bTxPdoMapping           = (byData & 0x80) != 0;
                            byData = *(EC_T_BYTE*)&(pbyEcMbxData[dwIndex]);                                                                  dwIndex += 1;
                            pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.bObCanBeUsedForBackup   = (byData & 0x01) != 0;
                            pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.bObCanBeUsedForSettings = (byData & 0x02) != 0;
                            if (pMbxTferPriv->MbxTfer.dwDataLen < dwIndex )
                            {
                                dwMbxResult = EC_E_INVALIDSIZE;
                            }
                            else
                            {
                                pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.wDataLen      = (EC_T_WORD)(pMbxTferPriv->MbxTfer.dwDataLen - dwIndex);
                                if (pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.wDataLen > 0 )
                                {
                                    pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.pbyData       = &pbyEcMbxData[dwIndex];
                                }
                                else
                                {
                                    pMbxTferPriv->MbxTfer.MbxData.CoE_EntryDesc.pbyData       = EC_NULL;
                                }
                            }
                        }
                        break;
                    case eMbxTferType_COE_EMERGENCY:
                        /* at least ETHERCAT_EMERGENCY_HEADER::__ErrorCode */
                        if (dwMbxDataLength < sizeof(EC_T_DWORD))
                        {
                            dwMbxResult = EC_E_INVALIDSIZE;
                        }
                        else
                        {
                        EC_T_COE_EMERGENCY* pCoeEmergency = EC_NULL;

                            pMbxTferPriv->MbxTfer.dwDataLen = dwMbxDataLength;
                            OsMemcpy(pMbxTferPriv->MbxTfer.pbyMbxTferData, pbyEcMbxData, EC_MIN(dwMbxDataLength, pMbxTferPriv->MbxTfer.MbxTferDesc.dwMaxDataLen));
                            pCoeEmergency = (EC_T_COE_EMERGENCY*)pwEcMbxData;
                            pMbxTferPriv->MbxTfer.MbxData.CoE_Emergency.wErrorCode      = EC_GET_FRM_WORD(&(pCoeEmergency->wErrorCode));
                            pMbxTferPriv->MbxTfer.MbxData.CoE_Emergency.byErrorRegister = pCoeEmergency->byErrorRegister;
                            OsMemcpy(pMbxTferPriv->MbxTfer.MbxData.CoE_Emergency.abyData, pCoeEmergency->abyData, sizeof(EC_T_BYTE) * EC_COE_EMERGENCY_DATASIZE);
                            pMbxTferPriv->MbxTfer.MbxData.CoE_Emergency.wStationAddress = EC_LOWORD(pCmd->dwInvokeId);
                        }
                        break;
#if (defined INCLUDE_COE_PDO_SUPPORT)
                    case eMbxTferType_COE_RX_PDO:
                        break;
#endif
                    default:
                        OsDbgAssert( EC_FALSE);
                        goto Exit;
                    }
                } /* if (dwMbxResult == EC_E_NOERROR) */
            } break;
#if (defined INCLUDE_FOE_SUPPORT)
        case eMbxTferType_FOE_FILE_UPLOAD:
        case eMbxTferType_FOE_FILE_DOWNLOAD:
            {
            } break;
#endif
#if (defined INCLUDE_SOE_SUPPORT)
        case eMbxTferType_SOE_READREQUEST:
        case eMbxTferType_SOE_WRITEREQUEST:
        case eMbxTferType_SOE_NOTIFICATION:
        case eMbxTferType_SOE_EMERGENCY:
            {
            EC_T_SOE_EMERGENCY*    pSoeEmergency    = EC_NULL;
            EC_T_SOE_NOTIFICATION* pSoeNotification = EC_NULL;

                if (dwMbxResult == EC_E_NOERROR)
                {
                    switch( pMbxTferPriv->MbxTfer.eMbxTferType)
                    {
                    case eMbxTferType_SOE_WRITEREQUEST:
                        *pMbxTferPriv->oEcMbxCmd.pbyElementFlags = pMbxTferPriv->MbxTfer.MbxData.SoE.byElementFlags;
                        break;
                    case eMbxTferType_SOE_READREQUEST:
                        *pMbxTferPriv->oEcMbxCmd.pbyElementFlags = pMbxTferPriv->MbxTfer.MbxData.SoE.byElementFlags;
                        pMbxTferPriv->MbxTfer.dwDataLen = dwMbxDataLength;
                        if (pMbxTferPriv->MbxTfer.pbyMbxTferData != pbyEcMbxData)
                        {
                            OsMemcpy(pMbxTferPriv->MbxTfer.pbyMbxTferData, pbyEcMbxData, dwMbxDataLength);
                        }
                        break;
                    case eMbxTferType_SOE_EMERGENCY:
                        if (dwMbxDataLength < sizeof(ETHERCAT_SOE_EMERGENCY_HEADER))
                        {
                            dwMbxResult = EC_E_INVALIDSIZE;
                        }
                        else
                        {
                            pMbxTferPriv->MbxTfer.dwDataLen = dwMbxDataLength;
                            OsMemcpy(pMbxTferPriv->MbxTfer.pbyMbxTferData, pbyEcMbxData, EC_MIN(dwMbxDataLength, pMbxTferPriv->MbxTfer.MbxTferDesc.dwMaxDataLen));
                            pSoeEmergency = (EC_T_SOE_EMERGENCY*)pwEcMbxData;
                            pMbxTferPriv->MbxTfer.MbxData.SoE_Emergency.wHeader      = EC_GET_FRM_WORD(&(pSoeEmergency->wHeader));
                            OsMemcpy(pMbxTferPriv->MbxTfer.MbxData.SoE_Emergency.abyData, pSoeEmergency->abyData, sizeof(EC_T_BYTE) * EC_SOE_EMERGENCY_DATASIZE);
                            pMbxTferPriv->MbxTfer.MbxData.SoE_Emergency.wStationAddress = EC_LOWORD(pCmd->dwInvokeId);
                        }
                        break;
                    case eMbxTferType_SOE_NOTIFICATION:
                        if (dwMbxDataLength < sizeof(ETHERCAT_SOE_NOTIFICATION_HEADER))
                        {
                            dwMbxResult = EC_E_INVALIDSIZE;
                        }
                        else
                        {
                            pMbxTferPriv->MbxTfer.dwDataLen = dwMbxDataLength;
                            OsMemcpy(pMbxTferPriv->MbxTfer.pbyMbxTferData, pbyEcMbxData, EC_MIN(dwMbxDataLength, pMbxTferPriv->MbxTfer.MbxTferDesc.dwMaxDataLen));
                            pSoeNotification = (EC_T_SOE_NOTIFICATION*)pwEcMbxData;
                            pMbxTferPriv->MbxTfer.MbxData.SoE_Notification.wHeader      = EC_GET_FRM_WORD(&(pSoeNotification->wHeader));
                            pMbxTferPriv->MbxTfer.MbxData.SoE_Notification.wIdn            = EC_GET_FRM_WORD(&(pSoeNotification->wIdn));
                            OsMemcpy(pMbxTferPriv->MbxTfer.MbxData.SoE_Notification.abyData, pSoeNotification->abyData, sizeof(EC_T_BYTE) * EC_SOE_NOTIFICATION_DATASIZE);
                            pMbxTferPriv->MbxTfer.MbxData.SoE_Notification.wStationAddress = EC_LOWORD(pCmd->dwInvokeId);
                        }
                        break;

                    default:
                        OsDbgAssert( EC_FALSE);
                        goto Exit;
                    }
                }

            } break;
#endif
#if (defined INCLUDE_AOE_SUPPORT)
        case eMbxTferType_AOE_READ:
        case eMbxTferType_AOE_WRITE:
        case eMbxTferType_AOE_READWRITE:
        case eMbxTferType_AOE_WRITECONTROL:
            {
                if (dwMbxResult == EC_E_NOERROR)
                {
                    switch( pMbxTferPriv->MbxTfer.eMbxTferType)
                    {
                    case eMbxTferType_AOE_WRITE:
                        break;
                    case eMbxTferType_AOE_READ:
                    case eMbxTferType_AOE_READWRITE:
                    case eMbxTferType_AOE_WRITECONTROL:
                        pMbxTferPriv->MbxTfer.dwDataLen = dwMbxDataLength;
                        OsMemcpy(pMbxTferPriv->MbxTfer.pbyMbxTferData, pbyEcMbxData, dwMbxDataLength);
                        break;
                    default:
                        OsDbgAssert( EC_FALSE);
                        goto Exit;
                    }
                }
                else
                {
                    pMbxTferPriv->MbxTfer.dwDataLen = 0;
                }

                pMbxTferPriv->MbxTfer.MbxData.AoE_Response.dwCmdResult = pMbxTferPriv->oEcMbxCmd.dwAoeCmdResult;
                pMbxTferPriv->MbxTfer.MbxData.AoE_Response.dwErrorCode = pMbxTferPriv->oEcMbxCmd.dwAoeErrorCode;
            } break;
#endif

#if (defined INCLUDE_RAWMBX_SUPPORT)
        case eMbxTferType_RAWMBX:
            pMbxTferPriv->MbxTfer.dwDataLen = dwMbxDataLength;
            break;
#endif

        default:
            break;
        }
        if (bTferDone)
        {
            if (0 == pMbxTferPriv->MbxTfer.dwErrorCode)
            {
                pMbxTferPriv->MbxTfer.dwErrorCode = dwMbxResult;
            }
            if (EC_E_NOERROR == pMbxTferPriv->MbxTfer.dwErrorCode)
            {
                pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_TferDone;
            }
            else
            {
                pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_TferReqError;
            }
            pMbxTferPriv->oEcMbxCmd.bInUseByInterface = EC_FALSE;
        }
    }
    /* notify application */
    if (bTferNotifyAll || (0 != pMbxTferPriv->MbxTfer.dwClntId))
    {
        EC_T_NOTIFYPARMS oNotifyParms;

        OsMemset(&oNotifyParms, 0, sizeof(EC_T_NOTIFYPARMS));
        oNotifyParms.pbyInBuf      = (EC_T_BYTE*)pMbxTferPriv;
        oNotifyParms.dwInBufSize   = sizeof(EC_T_MBXTFER);
        if (bTferNotifyAll)        
        {
            NotifyAllClients(EC_NOTIFY_MBOXRCV, 0, &oNotifyParms);
        }
        else
        {
            OsLock(m_oAtEcatDesc.poClntDescLock);
            m_oAtEcatDesc.dwClntDescInUse++;
            OsUnlock(m_oAtEcatDesc.poClntDescLock);
            for (EC_T_CLNTDESC* pClntDesc = m_oAtEcatDesc.pClntDescHead; EC_NULL != pClntDesc; pClntDesc = pClntDesc->pNext)
            {
                if (pClntDesc->byUniqueClntId == pMbxTferPriv->MbxTfer.dwClntId)
                {
                    if (EC_NULL != pClntDesc->S_pfnNotify)
                    {
                        oNotifyParms.pCallerData = pClntDesc->pCallerData;
                        pClntDesc->S_pfnNotify(EC_NOTIFY_MBOXRCV, &oNotifyParms);
                    }
                    break;
                }
            }
            OsLock(m_oAtEcatDesc.poClntDescLock);
            m_oAtEcatDesc.dwClntDescInUse--;
            OsUnlock(m_oAtEcatDesc.poClntDescLock);
        }
    }
    /* refresh mailbox transfer object */
    if (bTferIdle)
    {
        pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_Idle;
    }

Exit:
    PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_MboxNotify);

    if (bTferAbort)
    {
        dwRetVal = EC_E_CANCEL;
    }
    else
    {
        dwRetVal = EC_E_NOERROR;
    }
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Wrapper function to C++ MboxNotify routine

    \return void
*/
EC_T_DWORD CAtEmInterface::WrapperToMboxNotify(
    EC_T_DWORD      dwInstanceID
   ,PEcMailboxCmd   pCmd
   ,EC_T_BYTE*      pbyMbxData
   ,EC_T_DWORD      dwMbxDataLength
   ,EC_T_DWORD      dwMbxResult
                                             )
{
    CAtEmInterface  *pEmInst;

    OsDbgAssert(dwInstanceID < MAX_NUMOF_MASTER_INSTANCES);
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    OsDbgAssert(pEmInst != EC_NULL);
    if (EC_NULL == pEmInst)
    {
        return EC_E_INVALIDSTATE;
    }
    return pEmInst->MboxNotify(pCmd, pbyMbxData, dwMbxDataLength, dwMbxResult);
}

/***************************************************************************************************/
/**
\brief  Accept changed topology.

Re-check all HC Groups / carve up new connected slaves.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::HCAcceptTopoChange( EC_T_VOID )
{
#if (defined INCLUDE_HOTCONNECT)
EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatIoControl: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* enqueue to McSM if in manaual mode */
    if (  ((m_oAtEcatDesc.poMaster->GetHCMode() & echm_automan_mask) == echm_manual)
       || ((m_oAtEcatDesc.poMaster->GetHCMode() & echm_automan_mask) == echm_fullmanual)
       )
    {
        dwRetVal = McSmOrderAdd(eMcSmOrderType_HC_ACCEPT_TOPOLOGY_CHANGE, EC_NULL, EC_TRUE, m_oAtEcatDesc.poMaster->HCTopoChangeTimeoutValue());
    }
Exit:
    return dwRetVal;
#else
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Open / Close slave port.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::SetSlavePortState(
    EC_T_DWORD      dwSlaveId       /**< [in]   Slave ID */
   ,EC_T_WORD       wPort           /**< [in]   Port Nr. to modify (0-3) */
   ,EC_T_BOOL       bClose          /**< [in]   EC_TRUE: open port, EC_FALSE: close port */
   ,EC_T_BOOL       bForce          /**< [in]   EC_TRUE: force mode (always open / closed),
                                      *         EC_FALSE: grace mode (close after link down, open auto) */
   ,EC_T_DWORD      dwTimeout       /**< [in]   Timeout for appliance of change */
                                            )
{
#if (defined INCLUDE_PORT_OPERATION)
EC_T_DWORD          dwRetVal    = EC_E_ERROR;
EC_T_WORD           wFixedAddr  = INVALID_FIXED_ADDR;
EC_T_MCSM_ORDER*    poMcSmOrder = EC_NULL;
EC_T_DWORD          dwOrderId   = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("SetSlavePortState: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("SetSlavePortState: Invalid slave ID!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* try to get values first from cfg slave, then from bus slave */    
    {
    CEcSlave*    pCfgSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);
    CEcBusSlave* pBusSlave = EC_NULL;

        if (EC_NULL != pCfgSlave)
        {
            if (!pCfgSlave->IsPresentOnBus())
            {
                dwRetVal = EC_E_SLAVE_NOT_PRESENT;
                goto Exit;
            }
            pBusSlave = pCfgSlave->m_pBusSlave;
        }
        else
        {
            pBusSlave = m_oAtEcatDesc.poMaster->GetBusSlaveBySlaveId(dwSlaveId);
        }
        if (EC_NULL == pBusSlave)
        {
            dwRetVal = EC_E_NOTFOUND;
            goto Exit;
        }
        wFixedAddr = pBusSlave->GetFixedAddress();
    }
    dwRetVal = m_oAtEcatDesc.poMaster->m_oHotConnect.QueueChangePortRequest(
        wFixedAddr,
        EC_LOWORD(wPort%4),
        m_oAtEcatDesc.poMaster->m_oHotConnect.CreatePortChangeFlags(bClose, bForce, PORTCHG_SOURCE_MANUAL)
                                                                           );
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }

    dwRetVal = McSmOrderAdd(eMcSmOrderType_PORTOPERATION, &poMcSmOrder, (EC_NOWAIT == dwTimeout), ((EC_NOWAIT == dwTimeout) ? m_oAtEcatDesc.poMaster->GetScanBusDefaultTimeout() : dwTimeout));
    if ((EC_E_NOERROR != dwRetVal) || (EC_NOWAIT == dwTimeout))
    {
        goto Exit;
    }
    dwOrderId = poMcSmOrder->dwOrderId;

    /* wait until the order is finished or timeout */
    for(;;)
    {
        /* sleep a while */
        OsSleep(1);
        if (eEcatOrderState_Finished == poMcSmOrder->eOrderState)
        {
            if (dwOrderId != poMcSmOrder->dwOrderId)
            {
                /* order was overwritten, should never happen */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }
            /* requested state is reached */
            dwRetVal = poMcSmOrder->dwMcSmLastErrorStatus;
            if (EC_E_NOERROR != dwRetVal)
            {
                EC_DBGMSG("ecatSetSlavePortState() Error 0x%x in McSm state '%s' for requested state '%s'\n",
                    dwRetVal, EcMcSmStateString(poMcSmOrder->EMcSmErrorState), EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                goto Exit;
            }
            dwRetVal = EC_E_NOERROR;
            break;
        }
    } /* wait until the order was finished */

Exit:
    /* release order if needed */
    if ((EC_NULL != poMcSmOrder) && (EC_NOWAIT != dwTimeout))
    {
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    return dwRetVal;
#else
    EC_UNREFPARM(dwTimeout);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(wPort);
    EC_UNREFPARM(bClose);
    EC_UNREFPARM(bForce);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif /* INCLUDE_PORT_OPERATION */
}


/***************************************************************************************************/
/**
\brief  Read slave EEPRom Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::ReadSlaveEEPRom(
    EC_T_BOOL   bFixedAddressing,                   /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD   wSlaveAddress,                      /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_WORD   wEEPRomStartOffset,                 /**< [in]   Address to start EEPRom Read from.*/
    EC_T_WORD*  pwReadData,                         /**< [in]   Pointer to WORD array to carry the read data */
    EC_T_DWORD  dwReadLen,                          /**< [in]   Size of the EC_T_WORD array provided at pwReadData (in EC_T_WORDs) */
    EC_T_DWORD* pdwNumOutData,                      /**< [out]  Pointer to DWORD carrying actually read data after completion */
    EC_T_DWORD  dwTimeout                           /**< [in]   Timeout in milliseconds */
                                          )
{
    EC_T_DWORD          dwRetVal    = EC_E_ERROR;
    EC_T_DWORD          dwRes       = EC_E_ERROR;
    CEcBusSlave*        pBusSlave   = EC_NULL;
    EC_T_MCSM_ORDER*    poMcSmOrder = EC_NULL;
    EC_T_DWORD          dwOrderId   = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatReadSlaveEEPRom: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(!m_oAtEcatDesc.poEcDevice->IsLinkConnected())
    {
        dwRetVal = EC_E_LINK_DISCONNECTED;
        goto Exit;
    }
    if (bFixedAddressing)
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
    }
    else
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
    }
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }
    dwRes = m_oAtEcatDesc.poMaster->PrepareEEPRomRead(EC_TRUE, pBusSlave->GetFixedAddress(), wEEPRomStartOffset, pwReadData, dwReadLen, pdwNumOutData, dwTimeout);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = McSmOrderAdd(eMcSmOrderType_EEP_OPERATION, &poMcSmOrder, (EC_NOWAIT == dwTimeout), ((EC_NOWAIT == dwTimeout) ? m_oAtEcatDesc.dwOrderTimeout : dwTimeout));
    if ((EC_E_NOERROR != dwRetVal) || (EC_NOWAIT == dwTimeout))
    {
        goto Exit;
    }
    dwOrderId = poMcSmOrder->dwOrderId;

    /* wait until the order is finished or timeout */
    for(;;)
    {
        /* sleep a while */
        OsSleep(1);
        if (eEcatOrderState_Finished == poMcSmOrder->eOrderState)
        {
            if (dwOrderId != poMcSmOrder->dwOrderId)
            {
                /* order was overwritten, should never happen */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }
            /* requested state is reached */
            dwRetVal = poMcSmOrder->dwMcSmLastErrorStatus;
            if (EC_E_NOERROR != dwRetVal)
            {
                EC_DBGMSG("ecatReadSlaveEEPRom() Error 0x%x in McSm state '%s' for requested state '%s'\n",
                    dwRetVal, EcMcSmStateString(poMcSmOrder->EMcSmErrorState), EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                goto Exit;
            }
            dwRetVal = EC_E_NOERROR;
            break;
        }
    } /* wait until the order was finished */

Exit:
    /* release order if needed */
    if ((EC_NULL != poMcSmOrder) && (EC_NOWAIT != dwTimeout))
    {
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Write slave EEPRom Data.

\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CAtEmInterface::WriteSlaveEEPRom(
    EC_T_BOOL               bFixedAddressing,       /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD               wSlaveAddress,          /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_WORD               wEEPRomStartOffset,     /**< [in]   Address to start EEPRom Write from */
    EC_T_WORD*              pwWriteData,            /**< [in]   Pointer to WORD array carrying the write data */
    EC_T_DWORD              dwWriteLen,             /**< [in]   Sizeof Write Data WORD array (in WORDS) */
    EC_T_DWORD              dwTimeout               /**< [in]   Timeout in milliseconds */
                                           )
{
    EC_T_DWORD          dwRetVal    = EC_E_ERROR;
    EC_T_DWORD          dwRes       = EC_E_ERROR;
    CEcBusSlave*        pBusSlave   = EC_NULL;
    EC_T_MCSM_ORDER*    poMcSmOrder = EC_NULL;
    EC_T_DWORD          dwOrderId   = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatWriteSlaveEEPRom: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(!m_oAtEcatDesc.poEcDevice->IsLinkConnected())
    {
        dwRetVal = EC_E_LINK_DISCONNECTED;
        goto Exit;
    }

    if (bFixedAddressing)
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
    }
    else
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
    }
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    dwRes = m_oAtEcatDesc.poMaster->PrepareEEPRomWrite(EC_TRUE, pBusSlave->GetFixedAddress(), wEEPRomStartOffset, pwWriteData, dwWriteLen, dwTimeout);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = McSmOrderAdd(eMcSmOrderType_EEP_OPERATION, &poMcSmOrder, (EC_NOWAIT == dwTimeout), ((EC_NOWAIT == dwTimeout) ? m_oAtEcatDesc.dwOrderTimeout : dwTimeout));
    if ((EC_E_NOERROR != dwRetVal) || (EC_NOWAIT == dwTimeout))
    {
        goto Exit;
    }
    dwOrderId = poMcSmOrder->dwOrderId;

    /* wait until the order is finished or timeout */
    for(;;)
    {
        /* sleep a while */
        OsSleep(1);
        if (eEcatOrderState_Finished == poMcSmOrder->eOrderState)
        {
            if (dwOrderId != poMcSmOrder->dwOrderId)
            {
                /* order was overwritten, should never happen */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }
            /* requested state is reached */
            dwRetVal = poMcSmOrder->dwMcSmLastErrorStatus;
            if (EC_E_NOERROR != dwRetVal)
            {
                EC_DBGMSG("ecatWriteSlaveEEPRom() Error 0x%x in McSm state '%s' for requested state '%s'\n",
                    dwRetVal, EcMcSmStateString(poMcSmOrder->EMcSmErrorState), EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                goto Exit;
            }
            dwRetVal = EC_E_NOERROR;
            break;
        }
    } /* wait until the order was finished */

Exit:
    /* release order if needed */
    if ((EC_NULL != poMcSmOrder) && (EC_NOWAIT != dwTimeout))
    {
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Reload slave EEPRom.

\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CAtEmInterface::ReloadSlaveEEPRom(
    EC_T_BOOL               bFixedAddressing,       /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD               wSlaveAddress,          /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_DWORD              dwTimeout               /**< [in]   Timeout in milliseconds */
                                           )
{
    EC_T_DWORD          dwRetVal    = EC_E_ERROR;
    EC_T_DWORD          dwRes       = EC_E_ERROR;
    CEcBusSlave*        pBusSlave   = EC_NULL;
    EC_T_MCSM_ORDER*    poMcSmOrder = EC_NULL;
    EC_T_DWORD          dwOrderId   = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatReloadSlaveEEPRom: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(!m_oAtEcatDesc.poEcDevice->IsLinkConnected())
    {
        dwRetVal = EC_E_LINK_DISCONNECTED;
        goto Exit;
    }

    if (bFixedAddressing)
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
    }
    else
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
    }
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    dwRes = m_oAtEcatDesc.poMaster->PrepareEEPRomReload(EC_TRUE, pBusSlave->GetFixedAddress(), dwTimeout);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = McSmOrderAdd(eMcSmOrderType_EEP_OPERATION, &poMcSmOrder, (EC_NOWAIT == dwTimeout), ((EC_NOWAIT == dwTimeout) ? m_oAtEcatDesc.dwOrderTimeout : dwTimeout));
    if ((EC_E_NOERROR != dwRetVal) || (EC_NOWAIT == dwTimeout))
    {
        goto Exit;
    }
    dwOrderId = poMcSmOrder->dwOrderId;

    /* wait until the order is finished or timeout */
    for(;;)
    {
        /* sleep a while */
        OsSleep(1);
        if (eEcatOrderState_Finished == poMcSmOrder->eOrderState)
        {
            if (dwOrderId != poMcSmOrder->dwOrderId)
            {
                /* order was overwritten, should never happen */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }
            /* requested state is reached */
            dwRetVal = poMcSmOrder->dwMcSmLastErrorStatus;
            if (EC_E_NOERROR != dwRetVal)
            {
                EC_DBGMSG("ecatReloadSlaveEEPRom() Error 0x%x in McSm state '%s' for requested state '%s'\n",
                    dwRetVal, EcMcSmStateString(poMcSmOrder->EMcSmErrorState), EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                goto Exit;
            }
            dwRetVal = EC_E_NOERROR;
            break;
        }
    } /* wait until the order was finished */

Exit:
    /* release order if needed */
    if ((EC_NULL != poMcSmOrder) && (EC_NOWAIT != dwTimeout))
    {
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Reset ESC Slave Node.

Issue Slave Reset command using register 0x40.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::ResetSlaveController(
    EC_T_BOOL   bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
   ,EC_T_WORD   wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_DWORD  dwTimeout               /**< [in]   Timeout in milliseconds */
                                               )
{
    EC_T_DWORD          dwRetVal    = EC_E_ERROR;
    EC_T_DWORD          dwRes       = EC_E_ERROR;
    CEcBusSlave*        pBusSlave   = EC_NULL;
    EC_T_MCSM_ORDER*    poMcSmOrder = EC_NULL;
    EC_T_DWORD          dwOrderId   = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatResetSlaveController: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(!m_oAtEcatDesc.poEcDevice->IsLinkConnected())
    {
        dwRetVal = EC_E_LINK_DISCONNECTED;
        goto Exit;
    }

    if (bFixedAddressing)
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
    }
    else
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
    }
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    dwRes = m_oAtEcatDesc.poMaster->PrepareSlaveReset(EC_TRUE, pBusSlave->GetFixedAddress(), dwTimeout);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = McSmOrderAdd(eMcSmOrderType_EEP_OPERATION, &poMcSmOrder, (EC_NOWAIT == dwTimeout), ((EC_NOWAIT == dwTimeout) ? m_oAtEcatDesc.dwOrderTimeout : dwTimeout));
    if ((EC_E_NOERROR != dwRetVal) || (EC_NOWAIT == dwTimeout))
    {
        goto Exit;
    }
    dwOrderId = poMcSmOrder->dwOrderId;

    /* wait until the order is finished or timeout */
    for(;;)
    {
        /* sleep a while */
        OsSleep(1);
        if (eEcatOrderState_Finished == poMcSmOrder->eOrderState)
        {
            if (dwOrderId != poMcSmOrder->dwOrderId)
            {
                /* order was overwritten, should never happen */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }
            /* requested state is reached */
            dwRetVal = poMcSmOrder->dwMcSmLastErrorStatus;
            if (EC_E_NOERROR != dwRetVal)
            {
                EC_DBGMSG("ecatResetSlaveController() Error 0x%x in McSm state '%s' for requested state '%s'\n",
                    dwRetVal, EcMcSmStateString(poMcSmOrder->EMcSmErrorState), EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                goto Exit;
            }
            dwRetVal = EC_E_NOERROR;
            break;
        }
    } /* wait until the order was finished */

Exit:
    /* release order if needed */
    if ((EC_NULL != poMcSmOrder) && (EC_NOWAIT != dwTimeout))
    {
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Assign Slave EEPRom to Slave PDI Application.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::AssignSlaveEEPRom(
    EC_T_BOOL   bFixedAddressing            /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
   ,EC_T_WORD   wSlaveAddress               /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_BOOL   bSlavePDIAccessEnable       /**< [in]   EC_TRUE: assign EEPRom to slave PDI application, EC_FALSE: assign EEPRom to ECat Master */
   ,EC_T_BOOL   bForceAssign                /**< [in]   EC_TRUE: force Assignment, only available for ECat Master Assignment, EC_FALSE: don't force */
   ,EC_T_DWORD  dwTimeout                   /**< [in]   Timeout in milliseconds */
                                            )
{
    EC_T_DWORD          dwRetVal    = EC_E_ERROR;
    EC_T_DWORD          dwRes       = EC_E_ERROR;
    CEcBusSlave*        pBusSlave   = EC_NULL;
    EC_T_MCSM_ORDER*    poMcSmOrder = EC_NULL;
    EC_T_DWORD          dwOrderId   = 0;

    if (EC_NOWAIT == dwTimeout)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatAssignSlaveEEPRom: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (!m_oAtEcatDesc.poEcDevice->IsLinkConnected())
    {
        dwRetVal = EC_E_LINK_DISCONNECTED;
        goto Exit;
    }

    if (bFixedAddressing)
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
    }
    else
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
    }
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    dwRes = m_oAtEcatDesc.poMaster->PrepareEEPRomAssign(EC_TRUE, pBusSlave->GetFixedAddress(), bSlavePDIAccessEnable, bForceAssign, dwTimeout);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = McSmOrderAdd(eMcSmOrderType_EEP_OPERATION, &poMcSmOrder, (EC_NOWAIT == dwTimeout), ((EC_NOWAIT == dwTimeout) ? m_oAtEcatDesc.dwOrderTimeout : dwTimeout));
    if ((EC_E_NOERROR != dwRetVal) || (EC_NOWAIT == dwTimeout))
    {
        goto Exit;
    }
    dwOrderId = poMcSmOrder->dwOrderId;

    /* wait until the order is finished or timeout */
    for(;;)
    {
        /* sleep a while */
        OsSleep(1);
        if (eEcatOrderState_Finished == poMcSmOrder->eOrderState)
        {
            if (dwOrderId != poMcSmOrder->dwOrderId)
            {
                /* order was overwritten, should never happen */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }
            /* requested state is reached */
            dwRetVal = poMcSmOrder->dwMcSmLastErrorStatus;
            if (EC_E_NOERROR != dwRetVal)
            {
                EC_DBGMSG("ecatAssignSlaveEEPRom() Error 0x%x in McSm state '%s' for requested state '%s'\n",
                    dwRetVal, EcMcSmStateString(poMcSmOrder->EMcSmErrorState), EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                goto Exit;
            }
            dwRetVal = EC_E_NOERROR;
            break;
        }
    } /* wait until the order was finished */

Exit:
    /* release order if needed */
    if ((EC_NULL != poMcSmOrder) && (EC_NOWAIT != dwTimeout))
    {
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Check whether Slave PDI application marks EEPRom Access active.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::ActiveSlaveEEProm(
    EC_T_BOOL   bFixedAddressing            /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
   ,EC_T_WORD   wSlaveAddress               /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_BOOL*  pbSlavePDIAccessActive      /**< [out]  Pointer holding Boolean buffer, EC_TRUE: Slave PDI application EEPRom access active, EC_FALSE: not active */
   ,EC_T_DWORD  dwTimeout                   /**< [in]   Timeout in milliseconds */
                                            )
{
    EC_T_DWORD          dwRetVal    = EC_E_ERROR;
    EC_T_DWORD          dwRes       = EC_E_ERROR;
    CEcBusSlave*        pBusSlave   = EC_NULL;
    EC_T_MCSM_ORDER*    poMcSmOrder = EC_NULL;
    EC_T_DWORD          dwOrderId   = 0;
    static
    EC_T_BOOL           bActive     = EC_FALSE;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("ecatActiveSlaveEEProm: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (!m_oAtEcatDesc.poEcDevice->IsLinkConnected())
    {
        dwRetVal = EC_E_LINK_DISCONNECTED;
        goto Exit;
    }

    if (EC_NULL == pbSlavePDIAccessActive)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (bFixedAddressing)
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wSlaveAddress);
    }
    else
    {
        pBusSlave = m_oAtEcatDesc.poMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wSlaveAddress);
    }
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    dwRes = m_oAtEcatDesc.poMaster->PrepareEEPRomActive(EC_TRUE, pBusSlave->GetFixedAddress(), &bActive, dwTimeout);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = McSmOrderAdd(eMcSmOrderType_EEP_OPERATION, &poMcSmOrder, (EC_NOWAIT == dwTimeout), ((EC_NOWAIT == dwTimeout) ? m_oAtEcatDesc.dwOrderTimeout : dwTimeout));
    if ((EC_E_NOERROR != dwRetVal) || (EC_NOWAIT == dwTimeout))
    {
        goto Exit;
    }
    dwOrderId = poMcSmOrder->dwOrderId;

    /* wait until the order is finished or timeout */
    for(;;)
    {
        /* sleep a while */
        OsSleep(1);
        if (eEcatOrderState_Finished == poMcSmOrder->eOrderState)
        {
            if (dwOrderId != poMcSmOrder->dwOrderId)
            {
                /* order was overwritten, should never happen */
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }
            /* requested state is reached */
            dwRetVal = poMcSmOrder->dwMcSmLastErrorStatus;
            if (EC_E_NOERROR != dwRetVal)
            {
                EC_DBGMSG("ecatActiveSlaveEEProm() Error 0x%x in McSm state '%s' for requested state '%s'\n",
                    dwRetVal, EcMcSmStateString(poMcSmOrder->EMcSmErrorState), EcMcSmStateString(poMcSmOrder->EMcSmReqState));
                goto Exit;
            }
            dwRetVal = EC_E_NOERROR;
            break;
        }
    } /* wait until the order was finished */

Exit:
    if (EC_NULL != pbSlavePDIAccessActive)
    {
        EC_SETDWORD(pbSlavePDIAccessActive, bActive);
    }
    /* release order if needed */
    if ((EC_NULL != poMcSmOrder) && (EC_NOWAIT != dwTimeout))
    {
        m_oAtEcatDesc.McSmOrderRelease(poMcSmOrder);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Mailbox transfers are serialized (block parallel transfers)

  \return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
EC_T_DWORD CAtEmInterface::SlaveSerializeMbxTfers
    (EC_T_DWORD dwSlaveId)           /**< [in]  slave id */
{
    CEcSlave*   poSlave     = EC_NULL;
    EC_T_DWORD  dwRetVal    = EC_E_NOERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("SlaveSerializeMbxTfers: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("SlaveSerializeMbxTfers: Invalid slave ID!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (!m_oAtEcatDesc.poMaster->IsConfigured())
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* get config slave */
    poSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);
    if (EC_NULL == poSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }

    if (!poSlave->HasMailBox())
    {
        dwRetVal = EC_E_NO_MBX_SUPPORT;
        goto Exit;
    }

    ((CEcMbSlave*)poSlave)->SerializeMbxTfers();

Exit:
    return OsSetLastError(dwRetVal);
}


/***************************************************************************************************/
/** \brief Mailbox transfers are processed in parallel

  \return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
EC_T_DWORD CAtEmInterface::SlaveParallelMbxTfers
    (EC_T_DWORD dwSlaveId)           /**< [in]  slave id */
{
    CEcSlave*   poSlave     = EC_NULL;
    EC_T_DWORD  dwRetVal    = EC_E_NOERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("SlaveParallelMbxTfers: Stack not initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (INVALID_SLAVE_ID == dwSlaveId)
    {
        EC_ERRORMSGC(("SlaveParallelMbxTfers: Invalid slave ID!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (!m_oAtEcatDesc.poMaster->IsConfigured())
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* get config slave */
    poSlave = m_oAtEcatDesc.poMaster->GetCfgSlaveBySlaveId(dwSlaveId);
    if (EC_NULL == poSlave)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }

    if (!poSlave->HasMailBox())
    {
        dwRetVal = EC_E_NO_MBX_SUPPORT;
        goto Exit;
    }
    ((CEcMbSlave*)poSlave)->ParallelMbxTfers();

Exit:
    return OsSetLastError(dwRetVal);
}

/***************************************************************************************************/
/** \brief EtherCAT master control state machine

  This function runs the master control state machine (setting the system into different operational states)

\return EC_E_NOERROR if sm was executed, EC_E_BUSY if a request cannot be initiated due to a already pending one
*/
EC_T_VOID CAtEmInterface::McStateMachine(EC_T_VOID)
{
EC_T_DWORD       dwRes    = EC_E_NOERROR;
EC_T_MCSM_ORDER* poMcSmOrder = EC_NULL;
EC_T_BOOL        bContinueSm = EC_TRUE;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        EC_ERRORMSGC(("McSm: Stack not initialized!\n"));
        goto Exit;
    }
    /*************************************/
    /* insert new orders to pending list */
    /*************************************/
    while (0 != m_oAtEcatDesc.pNewPendingMcSmOrderFifo->GetCount())
    {
        m_oAtEcatDesc.pNewPendingMcSmOrderFifo->RemoveNoLock(poMcSmOrder);

        /* cancel request master state order if newer order with lower state is present in the order queue */
        if (eMcSmOrderType_REQ_MASTER_STATE == poMcSmOrder->eMcSmOrderType)
        {
            if (!IsListEmpty(&m_oAtEcatDesc.oPendingMcSmOrderList))
            {
            EC_T_LISTENTRY* pListIterator = EC_NULL;

                for (pListIterator  = GetListHeadEntry(&m_oAtEcatDesc.oPendingMcSmOrderList);
                     pListIterator != &m_oAtEcatDesc.oPendingMcSmOrderList;
                     pListIterator  = GetListNextEntry(pListIterator))
                {
                EC_T_MCSM_ORDER* poMcSmOrderChk = (EC_T_MCSM_ORDER*)pListIterator;

                    if ((poMcSmOrderChk->eMcSmOrderType == eMcSmOrderType_REQ_MASTER_STATE) && (poMcSmOrderChk->eReqState > poMcSmOrder->eReqState))
                    {
                        poMcSmOrderChk->dwMcSmLastErrorStatus = EC_E_CANCEL;
                        McSmOrderCancel(poMcSmOrderChk);
                    }
                }
            }
        }
        InsertHeadList(&m_oAtEcatDesc.oPendingMcSmOrderList, &poMcSmOrder->ListEntry);
    }
    /**************************************/
    /* terminate orders on cancel/timeout */
    /**************************************/
    if (!IsListEmpty(&m_oAtEcatDesc.oPendingMcSmOrderList))
    {
    EC_T_LISTENTRY* pListIterator = EC_NULL;

        for (pListIterator  = GetListHeadEntry(&m_oAtEcatDesc.oPendingMcSmOrderList);
             pListIterator != &m_oAtEcatDesc.oPendingMcSmOrderList;
             pListIterator  = GetListNextEntry(pListIterator))
        {
        EC_T_MCSM_ORDER* poMcSmOrderChk = (EC_T_MCSM_ORDER*)pListIterator;

            if (poMcSmOrderChk->bCancel)
            {
                poMcSmOrderChk->dwMcSmLastErrorStatus = EC_E_CANCEL;
                McSmOrderCancel(poMcSmOrderChk);
            }
            if (poMcSmOrderChk->oTimeout.IsElapsed())
            {
                poMcSmOrderChk->dwMcSmLastErrorStatus = EC_E_TIMEOUT;
                McSmOrderCancel(poMcSmOrderChk);
            }
        }
    }
    /***************************************/
    /* react on detected network changes   */
    /* interrupt current order if required */
    /***************************************/
    if (m_oAtEcatDesc.bJobsActive && m_oAtEcatDesc.poMaster->m_pBusTopology->GetPortOperationsEnabled())
    {
    EC_T_BOOL bTopologyChangeDetected    = m_oAtEcatDesc.poMaster->TopologyChangedDetected();
    EC_T_BOOL bDLStatusInterruptDetected = m_oAtEcatDesc.poMaster->DLStatusInterruptDetected();
    EC_T_BOOL bSlaveErrorDetected        = m_oAtEcatDesc.poMaster->GetSlaveErrorDetected();
    EC_T_BOOL bALStatusInterruptDetected = m_oAtEcatDesc.poMaster->ALStatusInterruptDetected(EC_FALSE);

        if (bTopologyChangeDetected)
        {
            if (DEVICE_STATE_UNKNOWN == m_oAtEcatDesc.poMaster->GetMasterDeviceState())
            {
                /* ignore topology changes if master is in UNKNOWN state */
                m_oAtEcatDesc.poMaster->m_pBusTopology->TopologyChangeDone();
                bTopologyChangeDetected = EC_FALSE;
            }
        }
        if (bSlaveErrorDetected)
        {
        CEcTimer* poRetryTimer = m_oAtEcatDesc.poMcSlaveErrAckRetryTimeout;

            /* if no slave with error was found in last "ack cycle" skip retries until some time has elapsed */
            if (poRetryTimer->IsStarted() && !poRetryTimer->IsElapsed())
            {
                bSlaveErrorDetected = EC_FALSE;
            }
            else
            {
                /* do request the next error ack cycle before at least 1000 msec have elapsed */
                /* use case: connect a slave in error state with an invalid address */
                poRetryTimer->Start(1000, m_oAtEcatDesc.poMaster->GetMsecCounterPtr());
            }
        }

        if (bTopologyChangeDetected || bDLStatusInterruptDetected || bSlaveErrorDetected || bALStatusInterruptDetected)
        {
            /* check for running order */
            if (!IsListEmpty(&m_oAtEcatDesc.oPendingMcSmOrderList))
            {
                /* get current running order */
                poMcSmOrder = (EC_T_MCSM_ORDER*)GetListTailEntry(&m_oAtEcatDesc.oPendingMcSmOrderList);
                if ((poMcSmOrder->eOrderState == eEcatOrderState_Running) || (poMcSmOrder->eOrderState == eEcatOrderState_Canceling))
                {
                    /* topology change should interrupt running master state change request */
                    if (bTopologyChangeDetected && (poMcSmOrder->eMcSmOrderType == eMcSmOrderType_REQ_MASTER_STATE))
                    {
                        /* race condition in WaitForBTStateMachine(), order is not interruptable if BTStateMachine was triggered */
                        if (eEcatMcSmState_WAIT_SB != m_oAtEcatDesc.EMcSmCurrState)
                        {
                            /* interrupt order */
                            EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                                "McSm: Interrupt order %s (Id=%d, RetryCnt=%d)\n", EcMcSmOrderString(poMcSmOrder->eMcSmOrderType), poMcSmOrder->dwOrderId, poMcSmOrder->dwRetryCnt));
#if (defined INCLUDE_DC_SUPPORT)
                            /* cancel DC state machine if needed */
                            if (eEcatMcSmState_WAIT_DC_INIT == m_oAtEcatDesc.EMcSmCurrState)
                            {
                                m_oAtEcatDesc.poMaster->m_pDistributedClks->CancelRequest();

                                /* DC initialization done within HC state machine (interrupting current order) */
                            }
#endif /* INCLUDE_DC_SUPPORT */
                            /* mark order as interrupted */
                            poMcSmOrder->eOrderState = eEcatOrderState_Interrupted;

                            /* don't wait for current order */
                            poMcSmOrder = EC_NULL;
                        }
                    }
                }
                else
                {
                    /* don't wait for current order */
                    poMcSmOrder = EC_NULL;
                }
            }
            else
            {
                /* no order currently pending */
                poMcSmOrder = EC_NULL;
            }
            /* insert new order if possible */
            if (EC_NULL == poMcSmOrder)
            {
                if (m_oAtEcatDesc.pFreeMcSmOrderFifoInternal->RemoveNoLock(poMcSmOrder))
                {
                    /* fill order descriptor following the priority of event detected */
                    if (bTopologyChangeDetected)
                    {
                        EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_TOPCH, ETHTYP1_MCSM,
                           "McSm: new topology change detected\n"));

                        poMcSmOrder->eMcSmOrderType = eMcSmOrderType_TOPOLOGY_CHANGE;
                    }
                    else if (bDLStatusInterruptDetected)
                    {
                        EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                            "McSm: DL Status interrupt detected\n"));

                        poMcSmOrder->eMcSmOrderType = eMcSmOrderType_REQ_DLSTATUS_IST;
                    }
                    else if (bSlaveErrorDetected)
                    {
                        EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_SLERR, ETHTYP1_MCSM,
                            "McSm: New slave error detected\n"));

                        poMcSmOrder->eMcSmOrderType = eMcSmOrderType_REQ_ALSTATUS_REFRESH;
                    }
                    else if (bALStatusInterruptDetected)
                    {
                        EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                            "McSm: AL Status interrupt detected\n"));

                        m_oAtEcatDesc.poMaster->ALStatusInterruptDetected(EC_TRUE);
                        poMcSmOrder->eMcSmOrderType = eMcSmOrderType_REQ_ALSTATUS_REFRESH;
                    }
                    else
                    {
                        OsDbgAssert(EC_FALSE);
                        poMcSmOrder->eMcSmOrderType = eMcSmOrderType_UNKNOWN;
                    }
                    poMcSmOrder->eReqState             = eEcatState_UNKNOWN;
                    poMcSmOrder->eMcSmReqState         = eEcatMcSmState_UNKNOWN;
                    poMcSmOrder->dwOrderId             = m_oAtEcatDesc.dwOrderId++;
                    poMcSmOrder->dwRetryCnt            = 0;
                    poMcSmOrder->bAutoRelease          = EC_TRUE;
                    poMcSmOrder->EMcSmErrorState       = eEcatMcSmState_UNKNOWN;
                    poMcSmOrder->EMcSmReqState         = eEcatMcSmState_UNKNOWN;
                    poMcSmOrder->dwMcSmLastErrorStatus = EC_E_BUSY;
                    poMcSmOrder->eOrderState           = eEcatOrderState_Pending;

                    /* insert at tail (before pending orders) */
                    InsertTailList(&m_oAtEcatDesc.oPendingMcSmOrderList, &poMcSmOrder->ListEntry);
                }
                else
                {
                    /* should never happen, network change will be processed later as soon as order descriptor are available again */
                    OsDbgAssert(EC_FALSE);
                }
            }
            /* else: wait for current running order */
        }
    }
    /************************/
    /* manage current order */
    /************************/
    if (!IsListEmpty(&m_oAtEcatDesc.oPendingMcSmOrderList))
    {
        poMcSmOrder = (EC_T_MCSM_ORDER*)GetListTailEntry(&m_oAtEcatDesc.oPendingMcSmOrderList);
    }
    else
    {
        /* no order available */
        poMcSmOrder = EC_NULL;
    }
    if (EC_NULL != poMcSmOrder)
    {
        switch (poMcSmOrder->eOrderState)
        {
        case eEcatOrderState_Pending:
        {
            EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORNEW, ETHTYP1_MCSM,
                "McSm: Start order %s (Id=%d)\n", EcMcSmOrderString(poMcSmOrder->eMcSmOrderType), poMcSmOrder->dwOrderId));

            poMcSmOrder->eOrderState = eEcatOrderState_Running;
            poMcSmOrder->dwMcSmLastErrorStatus = EC_E_BUSY;

            /* start default timeout if no timeout started until here         */
            /* TODO: delete this code to be able to support real WAITINFINITE */
            if (!poMcSmOrder->oTimeout.IsStarted())
            {
                poMcSmOrder->oTimeout.Start(m_oAtEcatDesc.dwOrderTimeout, m_oAtEcatDesc.poMaster->GetMsecCounterPtr());
            }
            m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START;

            switch (poMcSmOrder->eMcSmOrderType)
            {
            case eMcSmOrderType_REQ_MASTER_STATE:
                {
                    m_oAtEcatDesc.EMcSmReqMasterOpState = poMcSmOrder->eReqState;
                    m_oAtEcatDesc.EMcSmReqState         = poMcSmOrder->eMcSmReqState;
                } break;

            case eMcSmOrderType_TOPOLOGY_CHANGE:
                {
                    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_TOPOCHANGE_DONE;
                } break;
            case eMcSmOrderType_REQ_SB:
                {
                    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_SB_DONE;
                } break;
            case eMcSmOrderType_REQ_SB_FOR_GENERATE_ENI:
                {
                    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_SB_DONE;
                } break;
            case eMcSmOrderType_SB_ACCEPT_TOPOLOGY_CHANGE:
                {
                    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_SB_DONE;
                } break;
#if (defined INCLUDE_HOTCONNECT)
            case eMcSmOrderType_HC_ACCEPT_TOPOLOGY_CHANGE:
                {
                    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_ACCEPTTOPOCHANGE_DONE;
                } break;
#endif
            case eMcSmOrderType_PORTOPERATION:
                {
                    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_PORTOPERATION_DONE;
                } break;
            case eMcSmOrderType_EEP_OPERATION:
                {
                    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_EEPROMOP_DONE;
                } break;
            case eMcSmOrderType_REQ_DLSTATUS_IST:
                {
                    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_DLSTATUS_IST_DONE;
                } break;
            case eMcSmOrderType_REQ_ALSTATUS_REFRESH:
                {
                    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_ALSTATUS_REFRESH_DONE;
                } break;
#if (defined INCLUDE_CONFIG_EXTEND)
            case eMcSmOrderType_REQ_EXTENDED_CONFIG:
                {
                    m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_EXTENDED_CONFIG_DONE;
                } break;
#endif
            default:
                {
                    EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                        "McSm: Internal Error - Invalid event %d\n", poMcSmOrder->eMcSmOrderType));
                    OsDbgAssert(EC_FALSE);
                    poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
                    McSmOrderFinish(poMcSmOrder);
                    poMcSmOrder = EC_NULL;
                } break;
            }
        } break;
        case eEcatOrderState_Running:
        {
            /* nothing to do, handle on exit */
        } break;
        case eEcatOrderState_Interrupted:
        {
            /* resume order */
            EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                "McSm: Resume order %s (Id=%d, RetryCnt=%d)\n", EcMcSmOrderString(poMcSmOrder->eMcSmOrderType), poMcSmOrder->dwOrderId, poMcSmOrder->dwRetryCnt));

            poMcSmOrder->dwRetryCnt++;
            poMcSmOrder->eOrderState            = eEcatOrderState_Running;
            m_oAtEcatDesc.EMcSmReqMasterOpState = poMcSmOrder->eReqState;
            m_oAtEcatDesc.EMcSmReqState         = poMcSmOrder->eMcSmReqState;
            m_oAtEcatDesc.EMcSmCurrState        = eEcatMcSmState_START;
        } break;
        case eEcatOrderState_Canceling:
        {
            EC_T_BOOL bStabilizeMcSm = EC_FALSE;
            EC_T_BOOL bFinishMcSmOrder = EC_TRUE;

            switch (poMcSmOrder->eMcSmOrderType)
            {
            case eMcSmOrderType_EEP_OPERATION:
                if (EC_E_BUSY == m_oAtEcatDesc.poMaster->PollEEPROMOperation())
                {
                    /* wait for EEPROM operation */
                    bFinishMcSmOrder = EC_FALSE;
                }
                break;
#if (defined INCLUDE_PORT_OPERATION)
            case eMcSmOrderType_PORTOPERATION:
                if (EC_E_BUSY == m_oAtEcatDesc.poMaster->m_oHotConnect.PollChangePort())
                {
                    /* wait for HotConnect operation */
                    bFinishMcSmOrder = EC_FALSE;
                }
                break;
#endif
            case eMcSmOrderType_REQ_MASTER_STATE:
                switch (m_oAtEcatDesc.EMcSmCurrState)
                {
                case eEcatMcSmState_WAIT_SB:
                //    /* wait for BusScan */
                //    m_oAtEcatDesc.poMaster->CancelBTStateMachine();
                //    bFinishMcSmOrder = EC_FALSE;
                //    bStabilizeMcSm = EC_FALSE;
                //    break;
#if (defined INCLUDE_DC_SUPPORT)
                case eEcatMcSmState_WAIT_DC_INIT:
                    /* wait for DC state machine */
                    m_oAtEcatDesc.poMaster->m_pDistributedClks->CancelRequest();
                    bFinishMcSmOrder = EC_FALSE;
                    bStabilizeMcSm = EC_FALSE;
                    break;
#endif
                default:
                    if (!m_oAtEcatDesc.poMaster->MasterStateMachIsStable())
                    {
                        /* wait for MasterStateMachine */
                        bFinishMcSmOrder = EC_FALSE;
                    }
                    break;
                }
                break;
            case eMcSmOrderType_REQ_SB:
                if (EC_E_BUSY == m_oAtEcatDesc.poMaster->WaitForBTStateMachine())
                {
                    /* wait for BusScan */
                    bFinishMcSmOrder = EC_FALSE;
                }
                break;
            case eMcSmOrderType_REQ_EXTENDED_CONFIG:
                if (m_oAtEcatDesc.EMcSmCurrState == eEcatMcSmState_WAIT_EXTENDED_CONFIG)
                {
                    /* wait for config extend */
                    bFinishMcSmOrder = EC_FALSE;
                    bStabilizeMcSm = EC_FALSE;
                }
                break;
            default:
                bFinishMcSmOrder = EC_TRUE;
                break;
            }

            if (bFinishMcSmOrder)
            {
                OsDbgAssert(EC_E_NOERROR != poMcSmOrder->dwMcSmLastErrorStatus);
                McSmOrderFinish(poMcSmOrder);
                poMcSmOrder = EC_NULL;
            }
            if (bStabilizeMcSm)
            {
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START;
                m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_START;
            }
        } break;
        case eEcatOrderState_Finished:
        {
            McSmOrderFinish(poMcSmOrder);
            poMcSmOrder = EC_NULL;
        } break;
        case eEcatOrderState_Idle:
        default:
        {
            /* should never happen */
            OsDbgAssert(EC_FALSE);
            poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
            McSmOrderFinish(poMcSmOrder);
            poMcSmOrder = EC_NULL;
        }
        }
    }
    if (EC_NULL == poMcSmOrder)
    {
        /* nothing to do */
        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_UNKNOWN;
        m_oAtEcatDesc.EMcSmReqState = eEcatMcSmState_UNKNOWN;
        bContinueSm = EC_FALSE;
    }
    /****************************/
    /* Execute MC state machine */
    /****************************/
    while (bContinueSm)
    {
    EC_T_MCSM_STATE  EMcSmOldState   = m_oAtEcatDesc.EMcSmCurrState;
    EC_T_STATE       EMasterStateOld = m_oAtEcatDesc.eMasterState;

        bContinueSm = EC_FALSE;

        if (EC_NULL == poMcSmOrder)
        {
            OsDbgAssert(EC_FALSE);
            break;
        }
        OsDbgAssert(eEcatOrderState_Finished != poMcSmOrder->eOrderState);

        switch (m_oAtEcatDesc.EMcSmCurrState)
        {
        /************************************************************************/
        case eEcatMcSmState_UNKNOWN:
            OsDbgAssert(EC_FALSE);
            break;

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        /* beginning in this state the state machine will try to reach the requested final state stored in m_oAtEcatDesc.EMcSmReqState */
        case eEcatMcSmState_START:
            /* check state */
            if(m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_STOP_DONE)
            {
                /* request stop state */
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_STOP_REQ;
                bContinueSm = EC_TRUE;
            }
            else if(m_oAtEcatDesc.EMcSmReqState != eEcatMcSmState_START)
            {
                if (  (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_INIT_DONE)
                   || (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_PREOP_DONE)
                   || (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_SAFEOP_DONE)
                   || (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_OP_DONE)
                   || (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_SB_DONE)
                  )
                {
                    if ((m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_SB_DONE) || (m_oAtEcatDesc.poMaster->GetAllSlavesMustReachState() && (EC_E_NOERROR != m_oAtEcatDesc.poMaster->GetBusScanResult()) && (EC_E_BUSY != m_oAtEcatDesc.poMaster->GetBusScanResult())))
                    {
                        /* request a post-init bus scan */
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START_SB;
                        bContinueSm = EC_TRUE;
                    }
                    else
                    {
                        /* leave init and next step is determination of the next state transition */
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_TRANSITION;
                        bContinueSm = EC_TRUE;
                    }
                }
                else if (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_TOPOCHANGE_DONE)
                {
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START_TOPOCHANGE;
                    bContinueSm = EC_TRUE;
                }
                else if (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_ACCEPTTOPOCHANGE_DONE)
                {
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START_ACCCEPTTOPOCHANGE;
                    bContinueSm = EC_TRUE;
                }
                else if (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_PORTOPERATION_DONE)
                {
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START_PORTOPERATION;
                    bContinueSm = EC_TRUE;
                }
                else if (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_EEPROMOP_DONE)
                {
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START_EEPROMOP;
                    bContinueSm = EC_TRUE;
                }
                else if (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_DLSTATUS_IST_DONE)
                {
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START_DLSTATUS_IST;
                    bContinueSm = EC_TRUE;
                }
                else if (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_ALSTATUS_REFRESH_DONE)
                {
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START_ALSTATUS_REFRESH;
                    bContinueSm = EC_TRUE;
                }
                else if (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_EXTENDED_CONFIG_DONE)
                {
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START_EXTENDED_CONFIG;
                    bContinueSm = EC_TRUE;
                }
                else
                {
                    // stay in START
                }
            }
            else
            {
                // stay in START
            }
            break;

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_STOP_REQ:
            /* currently nothing to do */
            m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_STOP;
            bContinueSm = EC_TRUE;
            break;

        case eEcatMcSmState_WAIT_STOP:
            m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_STOP_DONE;
            bContinueSm = EC_TRUE;
            break;

        case eEcatMcSmState_STOP_DONE:
            if(m_oAtEcatDesc.EMcSmReqState != eEcatMcSmState_STOP_DONE)
            {
                /* next step after leaving the stop state is enter the init state */
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START;
                bContinueSm = EC_TRUE;
            }
            break;

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_START_SB:
            if (eMcSmOrderType_SB_ACCEPT_TOPOLOGY_CHANGE == poMcSmOrder->eMcSmOrderType)
            {
                dwRes = m_oAtEcatDesc.poMaster->StartBTAcceptTopologyChange();
            }
            else
            {
                dwRes = m_oAtEcatDesc.poMaster->StartBTBusScan(poMcSmOrder->oTimeout.GetDuration());
            }
            if (EC_E_NOERROR != dwRes)
            {
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_SB_DONE;
                bContinueSm = EC_TRUE;
            }
            else
            {
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_SB;
            }
            break;
        case eEcatMcSmState_WAIT_SB:
            dwRes = m_oAtEcatDesc.poMaster->WaitForBTStateMachine();
            if (EC_E_BUSY != dwRes)
            {
                m_oAtEcatDesc.poMaster->ReleaseBTStateMachine(dwRes);
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_SB_DONE;
                bContinueSm = EC_TRUE;
            }
            break;
        case eEcatMcSmState_SB_DONE:
            /* notify application */
            if (  (EC_E_LINK_DISCONNECTED    != poMcSmOrder->dwMcSmLastErrorStatus)
               && (EC_E_BUSCONFIG_TOPOCHANGE != poMcSmOrder->dwMcSmLastErrorStatus))
            {
                m_oAtEcatDesc.poMaster->NotifyScanBusStatus(m_oAtEcatDesc.poMaster->GetBusScanResult(), m_oAtEcatDesc.poMaster->GetScanBusSlaveCount());
            }
#if (defined INCLUDE_HOTCONNECT)
            m_oAtEcatDesc.poMaster->m_oHotConnect.HCSmHandleGroupAddedNotification();
#endif
            /* scan triggered by ecatScanBus(), no more action required */
            if (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_SB_DONE)
            {
                break;
            }
            /* scan triggered by ecatSetMasterState() */
            OsDbgAssert(eMcSmOrderType_REQ_MASTER_STATE == poMcSmOrder->eMcSmOrderType);

            /* eEcatMcSmState_WAIT_SB set last error to EC_E_NOERROR or requested state is lower than current state, ignore mismatch */
            if ((EC_E_NOERROR == poMcSmOrder->dwMcSmLastErrorStatus) || (m_oAtEcatDesc.EMcSmReqMasterOpState < m_oAtEcatDesc.eMasterState))
            {
                poMcSmOrder->dwMcSmLastErrorStatus = EC_E_BUSY;
            }
            /* error detected don't go further */
            if (EC_E_BUSY != poMcSmOrder->dwMcSmLastErrorStatus)
            {
                break;
            }
            if (  (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_INIT_DONE)
               || (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_PREOP_DONE)
               || (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_SAFEOP_DONE)
               || (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_OP_DONE)
              )
            {
                /* next step after bus scan is determination of the next state transition */
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_TRANSITION;
                bContinueSm = EC_TRUE;
            }
            else if (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_STOP_DONE)
            {
                /* continue with INIT state */
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START;
                bContinueSm = EC_TRUE;
            }
            else
            {
                OsDbgAssert(EC_FALSE);
            }
            break;

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_START_DLSTATUS_IST:
            dwRes = m_oAtEcatDesc.poMaster->StartBTDlStatusInt();
            if (EC_E_NOERROR != dwRes)
            {
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_DLSTATUS_IST_DONE;
                bContinueSm = EC_TRUE;
            }
            else
            {
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_DLSTATUS_IST;
            }
            break;
        case eEcatMcSmState_WAIT_DLSTATUS_IST:
            dwRes = m_oAtEcatDesc.poMaster->WaitForBTStateMachine();
            if (EC_E_BUSY != dwRes)
            {
                m_oAtEcatDesc.poMaster->ReleaseBTStateMachine(dwRes);
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_DLSTATUS_IST_DONE;
                bContinueSm = EC_TRUE;
            }
            break;
        case eEcatMcSmState_DLSTATUS_IST_DONE:
            /* check AL Status and WKC now */
            m_oAtEcatDesc.poMaster->SetNotAllDevOpNotificationEnabled();

            /* call interrupt done to be sure any pending interrupt is acknowleged */
            m_oAtEcatDesc.poMaster->DLStatusInterruptDone();

            break;

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_START_ALSTATUS_REFRESH:
            dwRes = m_oAtEcatDesc.poMaster->StartBTAlStatusRefresh();
            if (EC_E_NOERROR != dwRes)
            {
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_ALSTATUS_REFRESH_DONE;
                bContinueSm = EC_TRUE;
            }
            else
            {
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_ALSTATUS_REFRESH;
            }
            break;
        case eEcatMcSmState_WAIT_ALSTATUS_REFRESH:
            dwRes = m_oAtEcatDesc.poMaster->WaitForBTStateMachine();
            if (EC_E_BUSY != dwRes)
            {
                m_oAtEcatDesc.poMaster->ReleaseBTStateMachine(dwRes);
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_ALSTATUS_REFRESH_DONE;
                bContinueSm = EC_TRUE;
            }
            break;
        case eEcatMcSmState_ALSTATUS_REFRESH_DONE:
            /* slave error detection done */
            m_oAtEcatDesc.poMaster->SetSlaveErrorDetected(EC_FALSE);

            break;

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_START_TOPOCHANGE:
            dwRes = m_oAtEcatDesc.poMaster->StartHCTopologyChange();
            if (EC_E_NOERROR != dwRes)
            {
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_TOPOCHANGE_DONE;
                bContinueSm = EC_TRUE;
            }
            else
            {
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_TOPOCHANGE;
            }
            break;
        case eEcatMcSmState_WAIT_TOPOCHANGE:
            dwRes = m_oAtEcatDesc.poMaster->WaitForHCStateMachine();
            if (EC_E_BUSY != dwRes)
            {
                dwRes = m_oAtEcatDesc.poMaster->ReleaseHCStateMachine(dwRes);
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_TOPOCHANGE_DONE;
                bContinueSm = EC_TRUE;
            }
            break;
        case eEcatMcSmState_TOPOCHANGE_DONE:
            /* check AL Status and WKC now */
            m_oAtEcatDesc.poMaster->SetNotAllDevOpNotificationEnabled();

            break;

#if (defined INCLUDE_HOTCONNECT)
        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_START_ACCCEPTTOPOCHANGE:
            dwRes = m_oAtEcatDesc.poMaster->StartHCManualContinue();
            if (EC_E_NOERROR != dwRes)
            {
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_ACCEPTTOPOCHANGE_DONE;
                bContinueSm = EC_TRUE;
            }
            else
            {
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_ACCEPTTOPOCHANGE;
            }
            break;
        case eEcatMcSmState_WAIT_ACCEPTTOPOCHANGE:
            dwRes = m_oAtEcatDesc.poMaster->WaitForHCStateMachine();
            if (EC_E_BUSY != dwRes)
            {
                dwRes = m_oAtEcatDesc.poMaster->ReleaseHCStateMachine(dwRes);
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_ACCEPTTOPOCHANGE_DONE;
                bContinueSm = EC_TRUE;
            }
            break;
        case eEcatMcSmState_ACCEPTTOPOCHANGE_DONE:
            break;
#endif /* INCLUDE_HOTCONNECT */

#if (defined INCLUDE_PORT_OPERATION)
        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_START_PORTOPERATION:
            dwRes = m_oAtEcatDesc.poMaster->m_oHotConnect.StartChangePort(poMcSmOrder->oTimeout.GetDuration());
            if (EC_E_NOERROR != dwRes)
            {
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_PORTOPERATION_DONE;
                bContinueSm = EC_TRUE;
            }
            else
            {
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_PORTOPERATION;
            }
            break;
        case eEcatMcSmState_WAIT_PORTOPERATION:
            dwRes = m_oAtEcatDesc.poMaster->m_oHotConnect.PollChangePort();
            if (EC_E_BUSY != dwRes)
            {
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_PORTOPERATION_DONE;
                bContinueSm = EC_TRUE;
            }
            break;
        case eEcatMcSmState_PORTOPERATION_DONE:
            break;
#endif /* INCLUDE_PORT_OPERATION */

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_START_EEPROMOP:
            dwRes = m_oAtEcatDesc.poMaster->StartEEPROMOperation();
            if (EC_E_NOERROR != dwRes)
            {
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_EEPROMOP_DONE;
                bContinueSm = EC_TRUE;
            }
            else
            {
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_EEPROMOP;
            }
            break;
        case eEcatMcSmState_WAIT_EEPROMOP:
            dwRes = m_oAtEcatDesc.poMaster->PollEEPROMOperation();
            if (EC_E_BUSY != dwRes)
            {
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_EEPROMOP_DONE;
                bContinueSm = EC_TRUE;
            }
            break;
        case eEcatMcSmState_EEPROMOP_DONE:
            break;

#if (defined INCLUDE_GEN_OP_ENI) && (defined INCLUDE_CONFIG_EXTEND)
        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_START_EXTENDED_CONFIG:
            if (poMcSmOrder->pContext->ConfigExtend.bMcSmRequestBusScan)
            {
                CEcBusTopology* poBusTopology = m_oAtEcatDesc.poMaster->m_pBusTopology;

                EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug >> 16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                    "McSm: Request BusScan to extend configuration\n"));

                poBusTopology->DeleteUnexpectedBusSlaves();
                /* swap bus slave pool and memory */
                {
                    CEcBusSlave* poBusSlavePool     = poBusTopology->m_poBusSlavesPoolFree;
                    EC_T_BYTE*   pbyBusSlavesMem    = poBusTopology->m_pbyBusSlavesMem;
                    EC_T_DWORD   dwBusSlavesMemSize = poBusTopology->m_dwBusSlavesMemSize;

                    poBusTopology->m_poBusSlavesPoolFree = poMcSmOrder->pContext->ConfigExtend.poBusSlavePool;
                    poMcSmOrder->pContext->ConfigExtend.poBusSlavePool = poBusSlavePool;
                    poBusTopology->m_pbyBusSlavesMem = poBusTopology->m_pbyBusSlavesMemCur = poMcSmOrder->pContext->ConfigExtend.pbyBusSlavesMem;
                    poMcSmOrder->pContext->ConfigExtend.pbyBusSlavesMem = pbyBusSlavesMem;
                    poBusTopology->m_dwBusSlavesMemSize = poMcSmOrder->pContext->ConfigExtend.dwBusSlavesMemSize;
                    poMcSmOrder->pContext->ConfigExtend.dwBusSlavesMemSize = dwBusSlavesMemSize;
                }

                poBusTopology->SetNextBusScanGenEni();
                
                dwRes = m_oAtEcatDesc.poMaster->StartBTBusScan(poMcSmOrder->oTimeout.GetDuration());
                if (EC_E_NOERROR != dwRes)
                {
                    poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_EXTENDED_CONFIG_DONE;
                    bContinueSm = EC_TRUE;
                }
                else
                {
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_EXTENDED_CONFIG;
                }
            }
            else if (poMcSmOrder->pContext->ConfigExtend.bMcSmRequestResetConfig)
            {
                EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug >> 16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                    "McSm: Reset extended configuration\n"));
                /* redirect deletion of extended config back to api */
                poMcSmOrder->pContext->ConfigExtend.bApiRequestResetConfig = EC_TRUE;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_EXTENDED_CONFIG;
            }
            else if (poMcSmOrder->pContext->ConfigExtend.bMcSmRequestApplyConfig)
            {
                EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug >> 16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                    "McSm: Apply extended configuration to master\n"));
                
                m_oAtEcatDesc.poMaster->ConfigExtendApply(&poMcSmOrder->pContext->ConfigExtend.apoSlaves,
                                                          poMcSmOrder->pContext->ConfigExtend.wSlaveCnt,
                                                          &poMcSmOrder->pContext->ConfigExtend.apoMbSlaves,
                                                          poMcSmOrder->pContext->ConfigExtend.wMbSlaveCnt,
                                                          &poMcSmOrder->pContext->ConfigExtend.wMaxNumSlaves);

                /* redirect recreation of cfg topology back to api */
                poMcSmOrder->pContext->ConfigExtend.bApiRequestRecreateCfgTopology = EC_TRUE;             
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_EXTENDED_CONFIG;
            }
            else
            {
                poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_EXTENDED_CONFIG_DONE;
                bContinueSm = EC_TRUE;
            }
            break;
        case eEcatMcSmState_WAIT_EXTENDED_CONFIG:
            if (poMcSmOrder->pContext->ConfigExtend.bMcSmRequestBusScan)
            {
                EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug >> 16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                    "McSm: Wait for BusScan to extend configuration\n"));
                dwRes = m_oAtEcatDesc.poMaster->WaitForBTStateMachine();
                if (EC_E_BUSY != dwRes)
                {
                    m_oAtEcatDesc.poMaster->ReleaseBTStateMachine(dwRes);
                    poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_EXTENDED_CONFIG_DONE;
                    bContinueSm = EC_TRUE;
                }
            }
            else if (poMcSmOrder->pContext->ConfigExtend.bMcSmRequestResetConfig)
            {
                EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug >> 16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                    "McSm: Wait for Api to finish reset extended config request\n"));
                /* wait while API has deleted the extended config avoiding any other orders in this time */
                if (!poMcSmOrder->pContext->ConfigExtend.bApiRequestResetConfig)
                {
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_EXTENDED_CONFIG_DONE;
                    bContinueSm = EC_TRUE;
                }
            }
            else if (poMcSmOrder->pContext->ConfigExtend.bMcSmRequestApplyConfig)
            {
                EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug >> 16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                    "McSm: Wait for Api to finish configuration to master request\n"));
                /* wait while API has recreated the cfg topology avoiding any other orders in this time */
                if (!poMcSmOrder->pContext->ConfigExtend.bApiRequestRecreateCfgTopology)
                {
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_EXTENDED_CONFIG_DONE;
                    bContinueSm = EC_TRUE;
                }
            }
            else
            {
                poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_EXTENDED_CONFIG_DONE;
                bContinueSm = EC_TRUE;
            }
            break;
        case eEcatMcSmState_EXTENDED_CONFIG_DONE:
            if (poMcSmOrder->pContext->ConfigExtend.bMcSmRequestBusScan)
            {
                CEcBusTopology* poBusTopology = m_oAtEcatDesc.poMaster->m_pBusTopology;
               
                EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug >> 16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                    "McSm: BusScan to extend configuration is done\n"));

                poBusTopology->RemoveBusSlaveGenEniFromList();
                
                /* swap bus slave pool and memory */
                {
                    CEcBusSlave* poBusSlavePool     = poBusTopology->m_poBusSlavesPoolFree;
                    EC_T_BYTE*   pbyBusSlavesMem    = poBusTopology->m_pbyBusSlavesMem;
                    EC_T_DWORD   dwBusSlavesMemSize = poBusTopology->m_dwBusSlavesMemSize;

                    poBusTopology->m_poBusSlavesPoolFree = poMcSmOrder->pContext->ConfigExtend.poBusSlavePool;
                    poMcSmOrder->pContext->ConfigExtend.poBusSlavePool = poBusSlavePool;
                    poBusTopology->m_pbyBusSlavesMem = poBusTopology->m_pbyBusSlavesMemCur = poMcSmOrder->pContext->ConfigExtend.pbyBusSlavesMem;
                    poMcSmOrder->pContext->ConfigExtend.pbyBusSlavesMem = pbyBusSlavesMem;
                    poBusTopology->m_dwBusSlavesMemSize = poMcSmOrder->pContext->ConfigExtend.dwBusSlavesMemSize;
                    poMcSmOrder->pContext->ConfigExtend.dwBusSlavesMemSize = dwBusSlavesMemSize;
                }
            }
            break;
#endif /* INCLUDE_CONFIG_EXTEND */

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_TRANSITION:
            if (  (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_INIT_DONE)
               || (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_PREOP_DONE)
               || (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_SAFEOP_DONE)
               || (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_MASTER_OP_DONE)
              )
            {
#if (defined INCLUDE_DC_SUPPORT)
                /* if order interrupted, execute DC initialization if needed */
                if (m_oAtEcatDesc.bMcSmDcInitReq && (eEcatState_INIT < m_oAtEcatDesc.eMasterState) && (eEcatState_INIT < m_oAtEcatDesc.EMcSmReqMasterOpState))
                {
                    OsDbgAssert(m_oAtEcatDesc.poMaster->GetDcSupportEnabled());
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START_DC_INIT;
                    bContinueSm = EC_TRUE;
                    break;
                }
#endif /* INCLUDE_DC_SUPPORT */
                switch (m_oAtEcatDesc.eMasterState)
                {
                case eEcatState_UNKNOWN:
                    /* master state is unknown, set in init state first */
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_INIT;
                    bContinueSm = EC_TRUE;
                    break;
                case eEcatState_INIT:
                    switch( m_oAtEcatDesc.EMcSmReqState)
                    {
                    case eEcatMcSmState_MASTER_INIT_DONE:
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_INIT;
                        bContinueSm = EC_TRUE;
                        break;
                    case eEcatMcSmState_MASTER_PREOP_DONE:
                    case eEcatMcSmState_MASTER_SAFEOP_DONE:
                    case eEcatMcSmState_MASTER_OP_DONE:
                        /* transition from INIT to any higher state: request PREOP next */
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_PREOP;
                        bContinueSm = EC_TRUE;
                        break;
                    default:
                        EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                            "McSm: Internal Error - Invalid state transition %d -> %d\n", m_oAtEcatDesc.eMasterState, m_oAtEcatDesc.EMcSmReqState));
                        OsDbgAssert(EC_FALSE);
                        poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
                        break;
                    }
                    break;
                case eEcatState_PREOP:
                    switch( m_oAtEcatDesc.EMcSmReqState)
                    {
                    case eEcatMcSmState_MASTER_INIT_DONE:
                        /* transition from PREOP to INIT state: request INIT next */
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_INIT;
                        bContinueSm = EC_TRUE;
                        break;
                    case eEcatMcSmState_MASTER_PREOP_DONE:
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_PREOP;
                        bContinueSm = EC_TRUE;
                        break;
                    case eEcatMcSmState_MASTER_SAFEOP_DONE:
                    case eEcatMcSmState_MASTER_OP_DONE:
                        /* transition from PREOP to next higher state: request SAFEOP next */
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_SAFEOP;
                        bContinueSm = EC_TRUE;
                        break;
                    default:
                        EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                            "McSm: Internal Error - Invalid state transition %d -> %d\n", m_oAtEcatDesc.eMasterState, m_oAtEcatDesc.EMcSmReqState));
                        OsDbgAssert(EC_FALSE);
                        poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
                        break;
                    }
                    break;
                case eEcatState_SAFEOP:
                    switch( m_oAtEcatDesc.EMcSmReqState)
                    {
                    case eEcatMcSmState_MASTER_INIT_DONE:
                        /* request init state and skip preop */
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_INIT;
                        bContinueSm = EC_TRUE;
                        break;
                    case eEcatMcSmState_MASTER_PREOP_DONE:
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_PREOP;
                        bContinueSm = EC_TRUE;
                        break;
                    case eEcatMcSmState_MASTER_SAFEOP_DONE:
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_SAFEOP;
                        bContinueSm = EC_TRUE;
                        break;
                    case eEcatMcSmState_MASTER_OP_DONE:
                        /* transition from SAFEOP to OP: request OP next */
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_OP;
                        bContinueSm = EC_TRUE;
                        break;
                    default:
                        EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                            "McSm: Internal Error - Invalid state transition %d -> %d\n", m_oAtEcatDesc.eMasterState, m_oAtEcatDesc.EMcSmReqState));
                        OsDbgAssert(EC_FALSE);
                        poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
                        break;
                    }
                    break;
                case eEcatState_OP:
                    switch( m_oAtEcatDesc.EMcSmReqState)
                    {
                    case eEcatMcSmState_MASTER_INIT_DONE:
                        /* request init state and skip safeop/preop */
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_INIT;
                        bContinueSm = EC_TRUE;
                        break;
                    case eEcatMcSmState_MASTER_PREOP_DONE:
                        /* request preop state and skip safeop */
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_PREOP;
                        bContinueSm = EC_TRUE;
                        break;
                    case eEcatMcSmState_MASTER_SAFEOP_DONE:
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_SAFEOP;
                        bContinueSm = EC_TRUE;
                        break;
                    case eEcatMcSmState_MASTER_OP_DONE:
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_REQ_MASTER_OP;
                        bContinueSm = EC_TRUE;
                        break;
                    default:
                        EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                            "McSm: Internal Error - Invalid state transition %d -> %d\n", m_oAtEcatDesc.eMasterState, m_oAtEcatDesc.EMcSmReqState));
                        OsDbgAssert(EC_FALSE);
                        poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
                        break;
                    }
                    break;
                default:
                    break;
                }
                OsDbgAssert( m_oAtEcatDesc.EMcSmCurrState != eEcatMcSmState_TRANSITION );
            }
            else
            {
                EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                    "McSM: Internal error - Unknown EMcSmReqState = %d\n", m_oAtEcatDesc.EMcSmReqState));
                OsDbgAssert(EC_FALSE);
                poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
            }
            break;

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_REQ_MASTER_INIT:
#if (defined INCLUDE_DC_SUPPORT)
            if (m_oAtEcatDesc.poMaster->GetDcSupportEnabled())
            {
                m_oAtEcatDesc.bMcSmDcInitReq = EC_TRUE;
                m_oAtEcatDesc.poMaster->m_pDistributedClks->m_bAcycDistributionActive = EC_FALSE;
            }
#endif /* INCLUDE_DC_SUPPORT */
            m_oAtEcatDesc.poMaster->RequestMasterSmState(eEcatState_INIT);
            /* next step: wait for master init state */
            m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_MASTER_INIT;
            break;

        /************************************************************************/
        case eEcatMcSmState_WAIT_MASTER_INIT:
            if (m_oAtEcatDesc.poMaster->MasterStateMachIsStable())
            {
                dwRes = m_oAtEcatDesc.poMaster->GetMasterStateMachineResult();
                if (EC_E_NOERROR != dwRes)
                {
                    poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                }
                else
                {
                    m_oAtEcatDesc.eMasterState = GetMasterState();
                    if (eEcatState_INIT == m_oAtEcatDesc.eMasterState)
                    {
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_MASTER_INIT_DONE;
                        bContinueSm = EC_TRUE;
                    }
                    else
                    {
                        poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
                    }
                }
            }
            break;

        /************************************************************************/
        case eEcatMcSmState_MASTER_INIT_DONE:
            if (m_oAtEcatDesc.EMcSmCurrState != m_oAtEcatDesc.EMcSmReqState)
            {
                if (m_oAtEcatDesc.EMcSmReqState == eEcatMcSmState_STOP_DONE)
                {
                    /* continue with INIT state */
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START;
                    bContinueSm = EC_TRUE;
                }
                else
                {
                    /* next step after reaching master init state is determination of the next state transition */
                    m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_TRANSITION;
                    bContinueSm = EC_TRUE;
                }
            }
            break;

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_REQ_MASTER_PREOP:
            OsDbgAssert( m_oAtEcatDesc.eMasterState != eEcatState_UNKNOWN );
            m_oAtEcatDesc.poMaster->RequestMasterSmState(eEcatState_PREOP);
            /* next step: wait for master preop state */
            m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_MASTER_PREOP;
            break;

        /************************************************************************/
        case eEcatMcSmState_WAIT_MASTER_PREOP:
            if (m_oAtEcatDesc.poMaster->MasterStateMachIsStable())
            {
                dwRes = m_oAtEcatDesc.poMaster->GetMasterStateMachineResult();
                if (EC_E_NOERROR != dwRes)
                {
                    poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                }
                else
                {
                    m_oAtEcatDesc.eMasterState = GetMasterState();
                    if (eEcatState_PREOP == m_oAtEcatDesc.eMasterState)
                    {
#if (defined INCLUDE_DC_SUPPORT)
                        if (m_oAtEcatDesc.bMcSmDcInitReq && (eEcatState_INIT < m_oAtEcatDesc.EMcSmReqMasterOpState))
                        {
                            OsDbgAssert(m_oAtEcatDesc.poMaster->GetDcSupportEnabled());
                            m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_START_DC_INIT;
                        }
                        else
#endif /* INCLUDE_DC_SUPPORT */
                        {
                            /* next step: master op done */
                            m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_MASTER_PREOP_DONE;
                        }
                    }
                    else
                    {
                        poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
                    }
                }
            }
            break;

        /************************************************************************/
        case eEcatMcSmState_MASTER_PREOP_DONE:
            /* next step after reaching master preop state is determination of the next state transition */
            if (m_oAtEcatDesc.EMcSmCurrState != m_oAtEcatDesc.EMcSmReqState)
            {
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_TRANSITION;
                bContinueSm = EC_TRUE;
            }
            break;

#if (defined INCLUDE_DC_SUPPORT)
        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_START_DC_INIT:
            OsDbgAssert(m_oAtEcatDesc.poMaster->GetDcSupportEnabled());
            dwRes = m_oAtEcatDesc.poMaster->m_pDistributedClks->StartInitDC();
            if (EC_E_NOERROR != dwRes)
            {
                /* store error information */
                poMcSmOrder->dwMcSmLastErrorStatus = dwRes;

                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_DC_INIT_DONE;
                bContinueSm = EC_TRUE;
            }
            else
            {
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_DC_INIT;
            }
            break;

        /************************************************************************/
        case eEcatMcSmState_WAIT_DC_INIT:
            dwRes = m_oAtEcatDesc.poMaster->m_pDistributedClks->WaitForStateMachine();
            if (EC_E_BUSY != dwRes)
            {
                /* store error information */
                if (EC_E_NOERROR != dwRes)
                {
                    poMcSmOrder->dwMcSmLastErrorStatus = dwRes;

                    m_oAtEcatDesc.bMcSmDcInitReq = EC_TRUE;
                }
                else
                {
                    m_oAtEcatDesc.bMcSmDcInitReq = EC_FALSE;
                }
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_DC_INIT_DONE;
                bContinueSm = EC_TRUE;
                break;
            }
            break;

        /************************************************************************/
        case eEcatMcSmState_DC_INIT_DONE:
            m_oAtEcatDesc.poMaster->NotifyDcStatus(((EC_E_BUSY != poMcSmOrder->dwMcSmLastErrorStatus)?poMcSmOrder->dwMcSmLastErrorStatus:EC_E_NOERROR));
            if (EC_E_BUSY == poMcSmOrder->dwMcSmLastErrorStatus)
            {
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_TRANSITION;
                bContinueSm = EC_TRUE;
            }
            break;
#endif /* INCLUDE_DC_SUPPORT */

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_REQ_MASTER_SAFEOP:
            OsDbgAssert( m_oAtEcatDesc.eMasterState != eEcatState_UNKNOWN );
            m_oAtEcatDesc.poMaster->RequestMasterSmState(eEcatState_SAFEOP);
            /* next step: wait for master safeop state */
            m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_MASTER_SAFEOP;
            break;

        /************************************************************************/
        case eEcatMcSmState_WAIT_MASTER_SAFEOP:
            if (m_oAtEcatDesc.poMaster->MasterStateMachIsStable())
            {
                dwRes = m_oAtEcatDesc.poMaster->GetMasterStateMachineResult();
                if (EC_E_NOERROR != dwRes)
                {
                    poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                }
                else
                {
                    m_oAtEcatDesc.eMasterState = GetMasterState();
                    if (eEcatState_SAFEOP == m_oAtEcatDesc.eMasterState)
                    {
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_MASTER_SAFEOP_DONE;
                        bContinueSm = EC_TRUE;
                    }
                    else
                    {
                        poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
                    }
                }
            }
            break;

        /************************************************************************/
        case eEcatMcSmState_MASTER_SAFEOP_DONE:
            if (m_oAtEcatDesc.EMcSmCurrState != m_oAtEcatDesc.EMcSmReqState)
            {
                /* next step after reaching master safeop state is determination of the next state transition */
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_TRANSITION;
                bContinueSm = EC_TRUE;
            }
            break;

        /************************************************************************/
        /************************************************************************/
        /************************************************************************/
        case eEcatMcSmState_REQ_MASTER_OP:
            OsDbgAssert( m_oAtEcatDesc.eMasterState != eEcatState_UNKNOWN );
            m_oAtEcatDesc.poMaster->RequestMasterSmState(eEcatState_OP);
            /* next step: wait for master op state */
            m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_WAIT_MASTER_OP;
            break;

        /************************************************************************/
        case eEcatMcSmState_WAIT_MASTER_OP:
            if (m_oAtEcatDesc.poMaster->MasterStateMachIsStable())
            {
                dwRes = m_oAtEcatDesc.poMaster->GetMasterStateMachineResult();
                if (EC_E_NOERROR != dwRes)
                {
                    poMcSmOrder->dwMcSmLastErrorStatus = dwRes;
                }
                else
                {
                    m_oAtEcatDesc.eMasterState = GetMasterState();
                    if (eEcatState_OP == m_oAtEcatDesc.eMasterState)
                    {
                        m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_MASTER_OP_DONE;
                        bContinueSm = EC_TRUE;
                    }
                    else
                    {
                        poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
                    }
                }
            }
            break;

        /************************************************************************/
        case eEcatMcSmState_MASTER_OP_DONE:
            if (m_oAtEcatDesc.EMcSmCurrState != m_oAtEcatDesc.EMcSmReqState)
            {
                /* next step after reaching master op state is determination of the next state transition */
                m_oAtEcatDesc.EMcSmCurrState = eEcatMcSmState_TRANSITION;
                bContinueSm = EC_TRUE;
            }
            break;

        /************************************************************************/
        default:
            EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                "McSm: Internal Error - m_oAtEcatDesc.EMcSmCurrState value = %d\n", m_oAtEcatDesc.EMcSmCurrState));
            OsDbgAssert(EC_FALSE);
            poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
            break;
        } /* switch( m_oAtEcatDesc.EMcSmCurrState) */

        /* if eMasterState did not change during switch(){case:} re-ask for current master state
         * IMPORTANT: do not call GetMasterState twice a cycle because State Change is missed otherwise
         */
        if (EMasterStateOld == m_oAtEcatDesc.eMasterState)
        {
            m_oAtEcatDesc.eMasterState = GetMasterState();
        }

#if (defined INCLUDE_MASTER_OBD)
        /* Update Object 0x2001  Master State Summary, Bit 9: Master in requested State */
        m_oAtEcatDesc.poMaster->m_pSDOService->EntrySet_MastInReqState(m_oAtEcatDesc.EMcSmReqMasterOpState == m_oAtEcatDesc.eMasterState);
#endif

        /* TODO: Refactor to CEcMaster::NotifyStateChanges(EC_T_STATE EMasterStateOld) */
        /* create state change notifications if necessary */
        if (EMasterStateOld != m_oAtEcatDesc.eMasterState)
        {
            /* notify all slaves (dis-)appeared */
            m_oAtEcatDesc.poMaster->NotifySlavesPresence();

            /* notify all slaves state changed since last NotifySlavesStateChanged */
            m_oAtEcatDesc.poMaster->NotifySlavesStateChanged();

            /* notify all slaves in unexpected state since last NotifySlavesUnexpectedState */
            m_oAtEcatDesc.poMaster->NotifySlavesUnexpectedState();

            /* notify all slave errors */
            m_oAtEcatDesc.poMaster->NotifySlavesError();

            m_oAtEcatDesc.poMaster->NotifyStateChanged(EMasterStateOld, m_oAtEcatDesc.eMasterState);

#if (defined INCLUDE_SLAVE_STATISTICS)
            m_oAtEcatDesc.poMaster->RequestSlaveStatistics();
#endif
        }
        if (EMcSmOldState != m_oAtEcatDesc.EMcSmCurrState)
        {
            EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_STCNG, ETHTYP1_MCSM,
                "McSm: %s -> %s (%s)\n", EcMcSmStateString(EMcSmOldState), EcMcSmStateString(m_oAtEcatDesc.EMcSmCurrState), EcMcSmStateString(m_oAtEcatDesc.EMcSmReqState)));
        }
    }
Exit:
    /************************/
    /* manage current order */
    /************************/
    if (EC_NULL != poMcSmOrder)
    {
        switch (poMcSmOrder->eOrderState)
        {
        case eEcatOrderState_Running:
        {
            /* finish order if McSm is stable */
            if (m_oAtEcatDesc.EMcSmReqState == m_oAtEcatDesc.EMcSmCurrState)
            {
                McSmOrderFinish(poMcSmOrder);
                poMcSmOrder = EC_NULL;
                break;
            }
            /* check if error detected */
            if (EC_E_BUSY != poMcSmOrder->dwMcSmLastErrorStatus)
            {
                OsDbgAssert(EC_E_NOERROR != poMcSmOrder->dwMcSmLastErrorStatus);

                EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
                    "McSm: %s (0x%x) in state %s, requested state %s\n", EcErrorText(poMcSmOrder->dwMcSmLastErrorStatus), poMcSmOrder->dwMcSmLastErrorStatus, EcMcSmStateString(m_oAtEcatDesc.EMcSmCurrState), EcMcSmStateString(m_oAtEcatDesc.EMcSmReqState)));

                if (eMcSmOrderType_REQ_MASTER_STATE == poMcSmOrder->eMcSmOrderType)
                {
                    m_oAtEcatDesc.poMaster->CancelMasterStateMachine();
                    poMcSmOrder->eOrderState = eEcatOrderState_Canceling;
                }
                else
                {
                    McSmOrderFinish(poMcSmOrder);
                    poMcSmOrder = EC_NULL;
                }
#if (defined INCLUDE_SLAVE_STATISTICS)
                m_oAtEcatDesc.poMaster->RequestSlaveStatistics();
#endif
                break;
            }
        } break;
        default:
        {
            /* nothing to do */
            break;
        }
        }
    }
    return;
}

/***************************************************************************************************/
/** \brief Cancel an McSm order with error state

  This function finishes an order after an error was detected

\return N/A
*/
EC_T_DWORD CAtEmInterface::McSmOrderAdd(
    EC_T_MCSM_ORDER_TYPE        eMcSmOrderType
   ,EC_T_MCSM_ORDER**           ppoMcSmOrder
   ,EC_T_BOOL                   bAutoRelease
   ,EC_T_DWORD                  dwTimeout
   ,EC_T_STATE                  eReqState
   ,EC_T_MCSM_STATE             eMcSmReqState
   ,EC_T_MCSM_ORDER_CONTEXT*    pOrderContext
)
{
EC_T_DWORD       dwRetVal    = EC_E_ERROR;
EC_T_DWORD       dwRes       = EC_E_ERROR;
EC_T_MCSM_ORDER* poMcSmOrder = EC_NULL;

    if (EC_NOWAIT == dwTimeout)
    {
        /* should never happen */
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRes = m_oAtEcatDesc.McSmOrderQueueAdd(eMcSmOrderType, &poMcSmOrder, bAutoRelease, dwTimeout, eReqState, eMcSmReqState, pOrderContext);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORADD, ETHTYP1_MCSM,
        "McSm: Queue order %s (Id=%d, Timeout=%d)\n", EcMcSmOrderString(poMcSmOrder->eMcSmOrderType), poMcSmOrder->dwOrderId, dwTimeout));

    if (EC_NULL != ppoMcSmOrder)
    {
        *ppoMcSmOrder = poMcSmOrder;
    }
    else
    {
        OsDbgAssert(bAutoRelease);
    }
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Cancel an McSm order with error state

  This function finishes an order after an error was detected

\return N/A
*/
EC_T_VOID CAtEmInterface::McSmOrderCancel(EC_T_MCSM_ORDER* poMcSmOrder)
{
    EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORINT, ETHTYP1_MCSM,
        "McSm: Cancel order %s, dwMcSmLastErrorStatus = 0x%08X\n", EcMcSmOrderString(poMcSmOrder->eMcSmOrderType), poMcSmOrder->dwMcSmLastErrorStatus));

    /* terminate order */
    switch (poMcSmOrder->eOrderState)
    {
    case eEcatOrderState_Pending:
    {
        McSmOrderFinish(poMcSmOrder);
    } break;
    case eEcatOrderState_Running:
    case eEcatOrderState_Interrupted:
    {
        if (eMcSmOrderType_REQ_MASTER_STATE == poMcSmOrder->eMcSmOrderType)
        {
            switch (m_oAtEcatDesc.EMcSmCurrState)
            {
            case eEcatMcSmState_WAIT_SB:
                m_oAtEcatDesc.poMaster->StabilizeMasterStateMachine(EC_E_CANCEL, m_oAtEcatDesc.poMaster->GetMasterLowestDeviceState());
                break;
#if (defined INCLUDE_DC_SUPPORT)
            case eEcatMcSmState_WAIT_DC_INIT:
                m_oAtEcatDesc.poMaster->m_pDistributedClks->CancelRequest();

                /* next master state request must trigger DC state machine */
                m_oAtEcatDesc.bMcSmDcInitReq = EC_TRUE;

                m_oAtEcatDesc.poMaster->StabilizeMasterStateMachine(EC_E_CANCEL, m_oAtEcatDesc.poMaster->GetMasterLowestDeviceState());
                break;
#endif /* INCLUDE_DC_SUPPORT */
            default:
                m_oAtEcatDesc.poMaster->CancelMasterStateMachine();
                break;
            }
        }
        poMcSmOrder->eOrderState = eEcatOrderState_Canceling;
    } break;
    case eEcatOrderState_Canceling:
    {
        /* nothing to do */
    } break;
    default:
    {
        OsDbgAssert(EC_FALSE);
        poMcSmOrder->dwMcSmLastErrorStatus = EC_E_MCSM_FATAL_ERROR;
        McSmOrderFinish(poMcSmOrder);
    } break;
    }
}

/***************************************************************************************************/
/** \brief Finish an McSm order with error state

  This function finishes an order after an error was detected

\return N/A
*/
EC_T_VOID CAtEmInterface::McSmOrderFinish(EC_T_MCSM_ORDER* poMcSmOrder)
{
    /* store information in error descriptor */
    poMcSmOrder->EMcSmErrorState = m_oAtEcatDesc.EMcSmCurrState;
    poMcSmOrder->EMcSmReqState   = m_oAtEcatDesc.EMcSmReqState;

    /* if no error set, order was successful see McSmOrderAdd() */
    if (EC_E_BUSY == poMcSmOrder->dwMcSmLastErrorStatus)
    {
        poMcSmOrder->dwMcSmLastErrorStatus = EC_E_NOERROR;
    }
    EC_DBGMSGL((m_oAtEcatDesc.dwStateChangeDebug>>16, ETHTYP0_MCSM_ORFIN, ETHTYP1_MCSM,
        "McSm: Finish order %s (Id=%d), Result = 0x%x)\n", EcMcSmOrderString(poMcSmOrder->eMcSmOrderType), poMcSmOrder->dwOrderId, poMcSmOrder->dwMcSmLastErrorStatus));

    m_oAtEcatDesc.McSmOrderFinish(poMcSmOrder);
}


/* --------------------------------------------------------------------------------- */
/* Multi Instance API C --> C++ Wrapper Calls */
/* --------------------------------------------------------------------------------- */

/***************************************************************************************************/
/** */
/** \brief Initialize the EtherCAT master stack

  This function should be called at first, before the EtherCAT master stack can be used.
  After this call success, the function Start() should be call to place the EtherCAT master stack in
  operational state. The stack parameters are given through a structure EC_T_INIT_MASTER_PARMS with following
  members:
  pLinkParms          Parameters for the link layer; the type of the parameters  depends on the LinkLayer
  implementation. This parameter is normally a string to identify the network adapter.
  pvOsParms           Parameters for the OS layer; the type of the parameters depends on the OsLayer
  implementation.
  nBaseTimerPeriod    Base timer period in microsecond; this period is used to generate the master
  timer and  the cyclic commands timer.
  from the base timer. The stack state machine and watchdog is based on this timer.
  nCyclicMultiplier   Multiplier for the cyclic commands timer; this value is used to generate the
  cyclic commands timer from the base timer. This is the process data refresh cycle.
  If this value is set to 0, the process data refresh must be done with the
  function RefreshProcessData().
*/
/** \return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emInitMaster(
    EC_T_DWORD              dwInstanceID,   /**< [in] Instance Id */
    EC_T_INIT_MASTER_PARMS* pParms          /**< [in] Initialization parameters */
                                    )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master instance */
    if (EC_NULL != S_aMultiMaster[dwInstanceID].pInst)
    {
        EC_ERRORMSGC(("ecatInitMaster() Stack already initialized!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    S_aMultiMaster[dwInstanceID].bExtClkShutdownReq = EC_FALSE;
    S_aMultiMaster[dwInstanceID].bExtClkShutdownAck = EC_FALSE;
    S_aMultiMaster[dwInstanceID].pInst = EC_NEW(CAtEmInterface());
    if (EC_NULL == S_aMultiMaster[dwInstanceID].pInst)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_INTERFACE, S_aMultiMaster[dwInstanceID].pInst->GetMemLog(),  "emInitMaster:S_aMultiMaster", S_aMultiMaster[dwInstanceID].pInst, sizeof(CAtEmInterface));

    /* try to prevent simultaneous access to Init/Config */
    /* race condition still subsist here IRQ off or Interlocked access would be needed  */
    if (0 == S_dwInitConfigLockCnt++)
    {
        S_poInitConfigLock = OsCreateLockTyped(eLockType_INTERFACE);
    }
    OsLock(S_poInitConfigLock);
    dwRetVal = S_aMultiMaster[dwInstanceID].pInst->InitMaster(dwInstanceID, EC_TRUE, pParms, EC_NULL, EC_NULL, EC_NULL, EC_NULL, eCnfType_None, EC_NULL, 0);
    OsUnlock(S_poInitConfigLock);

    if (EC_E_NOERROR != dwRetVal)
    {
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_INTERFACE, S_aMultiMaster[dwInstanceID].pInst->GetMemLog(), "ecatInitMaster:S_aMultiMaster[dwInstanceID]", S_aMultiMaster[dwInstanceID].pInst, sizeof(CAtEmInterface));
        SafeDelete(S_aMultiMaster[dwInstanceID].pInst);
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** */
/** \brief De-initialize the EtherCAT master stack

    This function releases all the resources created and initialized by the function MasterInit() and
    by using the EtherCAT master stack API.
*/
/** \return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emDeinitMaster(
    EC_T_DWORD              dwInstanceID    /**< [in]   Instance Id*/
                                      )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    OsLock(S_poInitConfigLock);
    EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_INTERFACE, pEmInst->GetMemLog(), "ecatInitMaster:S_aMultiMaster[dwInstanceID]", pEmInst, sizeof(CAtEmInterface));
    dwRetVal = pEmInst->DeinitMaster();
    S_aMultiMaster[dwInstanceID].pInst = EC_NULL;
    OsUnlock(S_poInitConfigLock);

    delete pEmInst;

    S_dwInitConfigLockCnt--;
    if (0 == S_dwInitConfigLockCnt)
    {
        OsDeleteLock(S_poInitConfigLock);
        S_poInitConfigLock = EC_NULL;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** */
/** \brief Get Master Parms provided by ecatInitMaster, ecatSetMasterParms

    Also fills OS parms, Main Link parms, Red Link parms if buffer at pParms is big enough
    according to dwParmsBufSize.
*/
/** \return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emGetMasterParms(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID or'ed with session mask */
    EC_T_INIT_MASTER_PARMS* pParms,         /**< [out]  Buffer to store Master parameters */
    EC_T_DWORD              dwParmsBufSize  /**< [in]   Size buffer to store Master parameters */
                                 )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->GetMasterParms(pParms, dwParmsBufSize);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** */
/** \brief Changes Master Parms provided by ecatInitMaster

    Currently OS parms, Main Link parms, Red Link parms, dwMaxBusSlaves, dwMaxQueuedEthFrames,
    dwAdditionalEoEEndpoints, bVLANEnable, wVLANId, byVLANPrio cannot be changed!
*/
/** \return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emSetMasterParms(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID or'ed with session mask */
    EC_T_INIT_MASTER_PARMS* pParms          /**< [in]   New Master parameters */
                                 )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->ChangeMasterParms(pParms);

Exit:
    return dwRetVal;
}

/* --------------------------------------------------------------------------------- */

/***************************************************************************************************/
/**
\brief  Load the configuration.

This function has to be called after InitMaster and prior to calling Start. Among others
the EtherCAT topology defined in the given XML configuration file will be stored internally. The
Ethernet link layer and the base timer clock will be initialized, too.

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emConfigLoad(
    EC_T_DWORD              dwInstanceID,   /**< [in] Master Instance ID */
    EC_T_CNF_TYPE           eCnfType,       /**< [in] Enum type of configuration data provided */
    EC_T_PBYTE              pbyCnfData,     /**< [in] Configuration data */
    EC_T_DWORD              dwCnfDataLen    /**< [in] Length of configuration data in byte */
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    OsLock(S_poInitConfigLock);
    dwRetVal = pEmInst->ConfigLoad(eCnfType, pbyCnfData, dwCnfDataLen);
    OsUnlock(S_poInitConfigLock);

Exit:
    return dwRetVal;
}

/* --------------------------------------------------------------------------------- */

/***************************************************************************************************/
/**
\brief  Exclude a slave from the configuration.

This function has to be called after InitMaster and prior to calling Start. Among others
the EtherCAT topology defined in the given XML configuration file will be stored internally. The
Ethernet link layer and the base timer clock will be initialized, too.

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emConfigExcludeSlave(
    EC_T_DWORD              dwInstanceID,   /**< [in] Master Instance ID */
    EC_T_WORD               wPhysAddress    /**< [in] Physical address of the slave to be excluded */
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    OsLock(S_poInitConfigLock);
    dwRetVal = pEmInst->ConfigExcludeSlave(wPhysAddress);
    OsUnlock(S_poInitConfigLock);

Exit:
    return dwRetVal;
}

/* --------------------------------------------------------------------------------- */

/***************************************************************************************************/
/**
\brief  Include a slave from the configuration.

This function has to be called after InitMaster and prior to calling Start. Among others
the EtherCAT topology defined in the given XML configuration file will be stored internally. The
Ethernet link layer and the base timer clock will be initialized, too.

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emConfigIncludeSlave(
    EC_T_DWORD              dwInstanceID,   /**< [in] Master Instance ID */
    EC_T_WORD               wPhysAddress    /**< [in] Physical address of the slave to be included */
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    OsLock(S_poInitConfigLock);
    dwRetVal = pEmInst->ConfigIncludeSlave(wPhysAddress);
    OsUnlock(S_poInitConfigLock);

Exit:
    return dwRetVal;
}

/* --------------------------------------------------------------------------------- */

/***************************************************************************************************/
/**
\brief  Set previous port information of a slave

This function has to be called after InitMaster and prior to calling Start. Among others
the EtherCAT topology defined in the given XML configuration file will be stored internally. The
Ethernet link layer and the base timer clock will be initialized, too.

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emConfigSetPreviousPort(
    EC_T_DWORD              dwInstanceID,   /**< [in] Master Instance ID */
    EC_T_WORD               wPhysAddress,   /**< [in] Physical address of the slave to be included */
    EC_T_WORD               wPhysAddressPrev,
    EC_T_WORD               wPortPrev
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    OsLock(S_poInitConfigLock);
    dwRetVal = pEmInst->ConfigSetPreviousPort(wPhysAddress, wPhysAddressPrev, wPortPrev);
    OsUnlock(S_poInitConfigLock);

Exit:
    return dwRetVal;
}

/* --------------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------------- */

/***************************************************************************************************/
/**
    \brief Apply the configuration

\return EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emConfigApply(
    EC_T_DWORD              dwInstanceID    /**< [in]   Instance Id*/
                                      )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    OsLock(S_poInitConfigLock);
    dwRetVal = pEmInst->ConfigApply();
    OsUnlock(S_poInitConfigLock);

Exit:
    return dwRetVal;
}

/* --------------------------------------------------------------------------------- */

/***************************************************************************************************/
/**
\brief  Configure the Master.

This function has to be called after InitMaster and prior to calling Start. Among others
the EtherCAT topology defined in the given XML configuration file will be stored internally. The
Ethernet link layer and the base timer clock will be initialized, too.

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emConfigureMaster(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_CNF_TYPE           eCnfType,       /**< [in] Enum type of configuration data provided */
    EC_T_PBYTE              pbyCnfData,     /**< [in] Configuration data */
    EC_T_DWORD              dwCnfDataLen    /**< [in] Length of configuration data in byte */
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    OsLock(S_poInitConfigLock);
    dwRetVal = pEmInst->ConfigureMaster(eCnfType, pbyCnfData, dwCnfDataLen);
    OsUnlock(S_poInitConfigLock);

Exit:
    return dwRetVal;
}

/* --------------------------------------------------------------------------------- */

/***************************************************************************************************/
/**
\brief  Extend the configuration.

This function extends the existing configuration to allow mailbox communication with unexpected slaves.
Unexpected slaves can only reach PREOP state.

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emConfigExtend(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_BOOL               bResetConfig,   /**< [in]   If EC_TRUE extended configuration will be removed */
    EC_T_DWORD              dwTimeout       /**< [in]   Timeout in milliseconds */
                                      )
{
#if (defined INCLUDE_GEN_OP_ENI) && (defined INCLUDE_CONFIG_EXTEND)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    OsLock(S_poInitConfigLock);
    dwRetVal = pEmInst->ConfigExtend(bResetConfig, dwTimeout);
    OsUnlock(S_poInitConfigLock);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(bResetConfig);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/** \brief Start the EtherCAT master stack

  This function place the EtherCAT master stack in operational state. There are two possibilities to
  handle the stack start:
  1) The function Start() is blocking
  The parameter dwTimeout must be not equal to EC_NOWAIT. Is this case the function
  returns when the operational state is reached, or if the timeout raised.
  2) The function Start() is not blocking
  The parameter dwTimeout must be equal to EC_NOWAIT. Is this case the function returns
  immediately. The calling application must check the operational state of the stack
  waiting for the notify (EC_NOTIFY_STATECHANGED) call.

    \return Depends on the implementation of the function OsSetLastError(),
    normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emStart(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD              dwTimeout       /**< [in]   Timeout in milliseconds */
                               )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->Start(dwTimeout);

Exit:
    return dwRetVal;
}

/* --------------------------------------------------------------------------------- */

/***************************************************************************************************/
/** \brief Stop the EtherCAT master stack

This function place the EtherCAT master stack in init state.

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emStop(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD              dwTimeout       /**< [in]   Timeout in milliseconds */
                              )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->Stop(dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief EtherCAT wildcard function

  This function supports a number of control functions supported by the EtherCAT master stack. The
  data transfer is made through a structure EC_T_IOCTLPARMS with following members:
  pbyInBuf            Input data buffer
  dwInBufSize         Size of input data buffer in byte
  pbyOutBuf           Output data buffer
  dwOutBufSize        Size of output data buffer in byte
  pdwNumOutData       Number of output data bytes stored in output data buffer

    The following control functions are currently defined:
    EC_IOCTL_REGISTERCLIENT     Register a client for process data and notifications

    EC_IOCTL_UNREGISTERCLIENT   Un-register client for process data and notifications

    \return Depends on the implementation of the function OsSetLastError(),
    normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD emIoControl(
    EC_T_DWORD          dwInstanceID,   /**< [in]       Master Instance ID or'ed with session mask */
    EC_T_DWORD          dwCode,         /**< [in]       Control code */
    EC_T_IOCTLPARMS*    pParms          /**< [in/out]   Data transferred */
                                 )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->IoControl(dwCode, pParms);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Scans all connected slaves
    \return EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD emScanBus(
    EC_T_DWORD  dwInstanceID,   /**< [in] Master Instance ID */
    EC_T_DWORD  dwTimeout       /**< [in] Time-out in ms */
)
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->ScanBus(dwTimeout);
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Close and open ports on network to rule out slaves which permanently discard frames
    \return EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD emRescueScan(
    EC_T_DWORD  dwInstanceID,   /**< [in] Master Instance ID */
    EC_T_DWORD  dwTimeout       /**< [in] Time-out in ms */
    )
{
#if (defined INCLUDE_RESCUE_SCAN)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->RescueScan(dwTimeout);
Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/** \brief Get generic information on the Master Instance
\return EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD emGetMasterInfo(
    EC_T_DWORD        dwInstanceID /**< [in] Master Instance ID */,
    EC_T_MASTER_INFO* pMasterInfo
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->GetMasterInfo(pMasterInfo);
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Determine slave id according to its station address

  \return slave id or INVALID_SLAVE_ID if the slave could not be found or stack is not initialized
*/
ATECAT_API EC_T_DWORD  emGetSlaveId(
    EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_WORD               wStationAddress     /**< [in]  slave node number (physical address) */
                                    )
{
    EC_T_DWORD      dwSlaveId = INVALID_SLAVE_ID;
    CAtEmInterface* pEmInst   = EC_NULL;

    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        EC_ERRORMSGC(("ecatGetSlaveId: Stack not initialized!\n"));
        goto Exit;
    }
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        EC_ERRORMSGC(("ecatGetSlaveId: Stack not initialized!\n"));
        goto Exit;
    }
    dwSlaveId = pEmInst->GetSlaveId(wStationAddress);

Exit:
    return dwSlaveId;
}

/***************************************************************************************************/
/** \brief Determine slave fixed address according to its slave id.

  \return EC_E_NOERROR on success or EC_E_INVALIDPARM; EC_E_NOTFOUND (Slave not found)
*/
ATECAT_API EC_T_DWORD  emGetSlaveFixedAddr(
    EC_T_DWORD              dwInstanceID,        /**< [in]  Master Instance ID */
    EC_T_DWORD              dwSlaveId,           /**< [in]  Slave id */
    EC_T_WORD*              pwFixedAddr          /**< [in]  Corresponding fixed address */
                                      )
{
    CAtEmInterface  *pEmInst    = EC_NULL;
    EC_T_DWORD dwRetVal         = EC_E_NOERROR;

    if(dwSlaveId == INVALID_SLAVE_ID || pwFixedAddr == EC_NULL)
    {
        dwRetVal = EC_E_INVALIDPARM;
        (*pwFixedAddr) = 0;
        goto Exit;
    }
    (*pwFixedAddr) = 0;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->GetSlaveFixedAddr(dwSlaveId, pwFixedAddr);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Determine slave id according to its station address

\return slave id or INVALID_SLAVE_ID if the slave could not be found
*/
ATECAT_API EC_T_DWORD  emGetSlaveIdAtPosition(
    EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_WORD               wAutoIncAddress     /**< [in]  slave auto increment address (set in the XML configuration file) */
                                              )
{
    EC_T_DWORD      dwSlaveId = INVALID_SLAVE_ID;
    CAtEmInterface*  pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
    dwSlaveId = pEmInst->GetSlaveIdAtPosition(wAutoIncAddress);

Exit:
    return dwSlaveId;
}

/***************************************************************************************************/
/** \brief Get slave properties

\return EC_TRUE if the slave exists, EC_FALSE if the slave id is invalid
*/
ATECAT_API EC_T_BOOL   emGetSlaveProp(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD              dwSlaveId,      /**< [in]  slave id */
    EC_T_SLAVE_PROP*        pSlaveProp      /**< [out] slave properties */
                                      )
{
    EC_T_BOOL       bRetVal = EC_FALSE;
    CAtEmInterface* pEmInst = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
    bRetVal = pEmInst->GetSlaveProp(dwSlaveId, pSlaveProp);

Exit:
    return bRetVal;
}

/***************************************************************************************************/
/** \brief Get slave port state

\return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
ATECAT_API EC_T_DWORD  emGetSlavePortState(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD              dwSlaveId,      /**< [in]  slave id */
    EC_T_WORD*              pwPortState     /**< [out] slave properties */
                                      )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->GetSlavePortState(dwSlaveId, pwPortState);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Get slave state

\return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
ATECAT_API EC_T_DWORD  emGetSlaveState(
    EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_DWORD              dwSlaveId,          /**< [in]   slave id */
    EC_T_WORD*              pwCurrDevState,     /**< [out]  current device state */
    EC_T_WORD*              pwReqDevState       /**< [out]  requested device state */
                                       )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->GetSlaveState(dwSlaveId, pwCurrDevState, pwReqDevState);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief set slave device state

\return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
ATECAT_API EC_T_DWORD  emSetSlaveState(
    EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_DWORD              dwSlaveId,          /**< [in]  slave id */
    EC_T_WORD               wNewReqDevState,    /**< [in]  requested new device state. See DEVICE_STATE_... */
    EC_T_DWORD              dwTimeout           /**< [in]  timeout value in ms */
                                       )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->SetSlaveState(dwSlaveId, wNewReqDevState, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Transfer single Raw command

  \return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
ATECAT_API EC_T_DWORD  emTferSingleRawCmd(
    EC_T_DWORD              dwInstanceID,       /**< [in]  Master Instance ID */
    EC_T_DWORD              dwSlaveId,          /**< [in]  slave id */
    EC_T_BYTE               byCmd,              /**< [in]  ecat command EC_CMD_TYPE_... */
    EC_T_DWORD              dwMemoryAddress,    /**< [in]  memory address inside the slave (physical or logical) */
    EC_T_VOID*              pvData,             /**< [in, out]  pointer to data */
    EC_T_WORD               wLen,               /**< [in]  data length */
    EC_T_DWORD              dwTimeout           /**< [in]  timeout in msec */
                                          )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->TferSingleRawCmd(dwSlaveId, byCmd, dwMemoryAddress, pvData, wLen, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Read from slave register

\return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
ATECAT_API EC_T_DWORD  emReadSlaveRegister(
    EC_T_DWORD              dwInstanceID,       /**< [in]  Master Instance ID */
    EC_T_BOOL               bFixedAddressing,   /**< [in]  EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD               wSlaveAddress,      /**< [in]  Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_WORD               wRegisterOffset,    /**< [in]  register offset inside the slave memory  */
    EC_T_VOID*              pvData,             /**< [out] pointer to data */
    EC_T_WORD               wLen,               /**< [in]  data length */
    EC_T_DWORD              dwTimeout           /**< [in]  timeout in msec */
                                            )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->ReadSlaveRegister(bFixedAddressing, wSlaveAddress, wRegisterOffset, pvData, wLen, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Write into slave register

  \return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
ATECAT_API EC_T_DWORD  emWriteSlaveRegister(
    EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddressing,   /**< [in]  EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD               wSlaveAddress,      /**< [in]  Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_WORD               wRegisterOffset,    /**< [in]  register offset inside the slave memory */
    EC_T_VOID*              pvData,             /**< [in]  pointer to data */
    EC_T_WORD               wLen,               /**< [in]  data length */
    EC_T_DWORD              dwTimeout           /**< [in]  timeout in msec */
                                            )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->WriteSlaveRegister(bFixedAddressing, wSlaveAddress, wRegisterOffset, pvData, wLen, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Read from slave register

\return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
ATECAT_API EC_T_DWORD  emReadSlaveIdentification(
    EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddressing,   /**< [in]  EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD               wSlaveAddress,      /**< [in]  Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_WORD               wAdo,               /**< [in]  Address offset */
    EC_T_WORD*              pwValue,            /**< [out] Identification value */
    EC_T_DWORD              dwTimeout           /**< [in]  Timeout in msec */
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->ReadSlaveIdentification(bFixedAddressing, wSlaveAddress, wAdo, pwValue, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Disable slave

\return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
ATECAT_API EC_T_DWORD  emSetSlaveDisabled(
    EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddressing,   /**< [in]  EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD               wSlaveAddress,      /**< [in]  Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_BOOL               bDisabled           /**< [in]  EC_TRUE: slave is disabled, EC_FALSE: slave is not disabled */
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->SetSlaveDisabled(bFixedAddressing, wSlaveAddress, bDisabled);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Prepare slave for disconnection

\return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
ATECAT_API EC_T_DWORD  emSetSlaveDisconnected(
    EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddressing,   /**< [in]  EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD               wSlaveAddress,      /**< [in]  Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_BOOL               bDisconnected       /**< [in]  EC_TRUE: slave prepared for disconnection, EC_FALSE: slave is prepared for disconnection */
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->SetSlaveDisconnected(bFixedAddressing, wSlaveAddress, bDisconnected);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Transfer Raw command

  \return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
ATECAT_API EC_T_DWORD  emQueueRawCmd(
    EC_T_DWORD              dwInstanceID,       /**< [in]  Master Instance ID */
    EC_T_DWORD              dwSlaveId,          /**< [in]  slave id */
    EC_T_WORD               wInvokeId,          /**< [in]  Invoke Id */
    EC_T_BYTE               byCmd,              /**< [in]  ecat command */
    EC_T_DWORD              dwMemoryAddress,    /**< [in]  memory address inside the slave (physical or logical) */
    EC_T_VOID*              pvData,             /**< [in]  pointer to data */
    EC_T_WORD               wLen                /**< [in]  data length */
                                    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->QueueRawCmd(0, dwSlaveId, wInvokeId, byCmd, dwMemoryAddress, pvData, wLen);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Transfer Raw command

  \return EC_E_NOERROR on success, EC_E_NOTFOUND if the slave does not exist, other error codes.
*/
ATECAT_API EC_T_DWORD  emClntQueueRawCmd(
    EC_T_DWORD              dwInstanceID,       /**< [in]  Master Instance ID */
    EC_T_DWORD              dwClntId,           /**< [in]  client id */
    EC_T_DWORD              dwSlaveId,          /**< [in]  slave id */
    EC_T_WORD               wInvokeId,          /**< [in]  Invoke Id */
    EC_T_BYTE               byCmd,              /**< [in]  ecat command */
    EC_T_DWORD              dwMemoryAddress,    /**< [in]  memory address inside the slave (physical or logical) */
    EC_T_VOID*              pvData,             /**< [in]  pointer to data */
    EC_T_WORD               wLen                /**< [in]  data length */
                                    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->QueueRawCmd(dwClntId, dwSlaveId, wInvokeId, byCmd, dwMemoryAddress, pvData, wLen );

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Determine the number of slaves which are configured in the XML file.

  \return number of slaves
*/
EC_T_DWORD  emGetNumConfiguredSlaves(
    EC_T_DWORD              dwInstanceID    /**< [in]   Master Instance ID */
                                                )
{
    EC_T_DWORD  dwNumConfiguredSlaves = 0;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
    dwNumConfiguredSlaves = pEmInst->GetNumConfiguredSlaves();

Exit:
    return dwNumConfiguredSlaves;
}

/***************************************************************************************************/
/** \brief Create mailbox transfer object

\return pointer to the mailbox transfer object, EC_NULL in case of error
*/
EC_T_MBXTFER* emMbxTferCreate(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER_DESC*      pMbxTferDesc    /**< [in]  mailbox transfer descriptor */
                                         )
{
    EC_T_MBXTFER*   pMbxTfer = EC_NULL;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
    pMbxTfer = pEmInst->MbxTferCreate(pMbxTferDesc, pEmInst->GetMemLog());

Exit:
    return pMbxTfer;
}

/***************************************************************************************************/
/** \brief Abort mailbox transfer

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD emMbxTferAbort(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER*           pMbxTfer        /**< [in]  mailbox transfer descriptor */
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->MbxTferAbort(pMbxTfer);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Delete mailbox transfer object

\return N/A
*/
ATECAT_API EC_T_VOID     emMbxTferDelete(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER*           pMbxTfer        /**< [in]  mailbox transfer object */
                                         )
{
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
    pEmInst->MbxTferDelete(pMbxTfer, S_aMultiMaster[dwInstanceID].pInst->GetMemLog());

Exit:
   return ;
}

/***************************************************************************************************/
/** \brief Send a raw mailbox command

\return N/A
*/
ATECAT_API EC_T_DWORD emClntSendRawMbx(
    EC_T_DWORD  dwInstanceID,   /**< [in] Master instance ID */
    EC_T_DWORD  dwClntId,       /**< [in] Client ID */
    EC_T_BYTE*  pbyMbxCmd,      /**< [in] Buffer containing the raw mbx cmd starting with mailbox header */
    EC_T_DWORD  dwMbxCmdLen,    /**< [in] Length of pbyMbxCmd buffer */
    EC_T_DWORD  dwTimeout       /**< [in] Timeout in msec */
    )
{
#if (defined INSTRUMENT_MASTER)
    if (emClntSendRawMbxInstr)
    {
        return emClntSendRawMbxInstr(dwInstanceID, dwClntId, pbyMbxCmd, dwMbxCmdLen, dwTimeout);
    }
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->SendRawMbx(dwClntId, pbyMbxCmd, dwMbxCmdLen, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwClntId);
    EC_UNREFPARM(pbyMbxCmd);
    EC_UNREFPARM(dwMbxCmdLen);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/** \brief Perform SoE read request

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emSoeRead(
      EC_T_DWORD  dwInstanceID      /**< [in]  Master Instance ID */
     ,EC_T_DWORD  dwSlaveId         /**< [in]  ethercat slave ID */
     ,EC_T_BYTE   byDriveNo         /**< [in]  number of drive in slave */
     ,EC_T_BYTE*  pbyElementFlags   /**< [in/out]  which element of object is read */
     ,EC_T_WORD   wIDN              /**< [in]  IDN of the object */
     ,EC_T_BYTE*  pbyData           /**< [out] IDN data */
     ,EC_T_DWORD  dwDataLen         /**< [in]  length of pbyData buffer */
     ,EC_T_DWORD* pdwOutDataLen     /**< [out] IDN data length (number of received bytes) */
     ,EC_T_DWORD  dwTimeout         /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
                                         )
{
#if defined( INCLUDE_SOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->SoeRead(dwSlaveId, byDriveNo, pbyElementFlags, wIDN, pbyData, dwDataLen, pdwOutDataLen, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(byDriveNo);
    EC_UNREFPARM(pbyElementFlags);
    EC_UNREFPARM(wIDN);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(dwDataLen);
    EC_UNREFPARM(pdwOutDataLen);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/** \brief Initiate SoE read request

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emSoeReadReq(
    EC_T_DWORD      dwInstanceID      /**< [in]  Master Instance ID */
   ,EC_T_MBXTFER*   pMbxTfer
   ,EC_T_DWORD      dwSlaveId         /**< [in]  ethercat slave ID */
   ,EC_T_BYTE       byDriveNo         /**< [in]  number of drive in slave */
   ,EC_T_BYTE*      pbyElementFlags   /**< [in]  which element of object is read */
   ,EC_T_WORD       wIDN              /**< [in]  IDN of the object */
   ,EC_T_DWORD      dwTimeout         /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
                                   )
{
#if defined( INCLUDE_SOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->SoeReadReq(pMbxTfer, dwSlaveId, byDriveNo, pbyElementFlags, wIDN, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pMbxTfer);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(byDriveNo);
    EC_UNREFPARM(pbyElementFlags);
    EC_UNREFPARM(wIDN);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/** \brief Perform SoE write request

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emSoeWrite(
     EC_T_DWORD    dwInstanceID      /**< [in]  Master Instance ID */
    ,EC_T_DWORD    dwSlaveId         /**< [in]  ethercat slave ID */
    ,EC_T_BYTE     byDriveNo         /**< [in]  number of drive in slave */
    ,EC_T_BYTE*    pbyElementFlags   /**< [in]  which element of object is written */
    ,EC_T_WORD     wIDN              /**< [in]  IDN of the object */
    ,EC_T_BYTE*    pbyData           /**< [in]  data of the element to be written */
    ,EC_T_DWORD    dwDataLen         /**< [in]  length of pbyData buffer */
    ,EC_T_DWORD    dwTimeout         /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
                                         )
{
#if defined( INCLUDE_SOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->SoeWrite(dwSlaveId, byDriveNo, pbyElementFlags, wIDN, pbyData, dwDataLen, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(byDriveNo);
    EC_UNREFPARM(pbyElementFlags);
    EC_UNREFPARM(wIDN);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(dwDataLen);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/** \brief Initiate SoE write request

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emSoeWriteReq(
    EC_T_DWORD      dwInstanceID        /**< [in]  Master Instance ID */
   ,EC_T_MBXTFER*   pMbxTfer
   ,EC_T_DWORD      dwSlaveId         /**< [in]  ethercat slave ID */
   ,EC_T_BYTE       byDriveNo         /**< [in]  number of drive in slave */
   ,EC_T_BYTE*      pbyElementFlags   /**< [in]  which element of object is written */
   ,EC_T_WORD       wIDN              /**< [in]  IDN of the object */
   ,EC_T_DWORD      dwTimeout         /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
                                    )
{
#if defined( INCLUDE_SOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->SoeWriteReq(pMbxTfer, dwSlaveId, byDriveNo, pbyElementFlags, wIDN, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pMbxTfer);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(byDriveNo);
    EC_UNREFPARM(pbyElementFlags);
    EC_UNREFPARM(wIDN);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/** \brief Initiate SoE write request

\return Depends on the implementation of the function OsSetLastError(),
normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emSoeAbortProcCmd(
       EC_T_DWORD  dwInstanceID        /**< [in]  Master Instance ID */
      ,EC_T_DWORD    dwSlaveId         /**< [in]  ethercat slave ID */
      ,EC_T_BYTE     byDriveNo         /**< [in]  number of drive in slave */
      ,EC_T_BYTE*    pbyElementFlags   /**< [in]  which element of object is written */
      ,EC_T_WORD     wIDN              /**< [in]  IDN of the object */
      ,EC_T_DWORD    dwTimeout         /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
                                         )
{
#if defined( INCLUDE_SOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->SoeAbortProcCmd(dwSlaveId, byDriveNo, pbyElementFlags, wIDN, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(byDriveNo);
    EC_UNREFPARM(pbyElementFlags);
    EC_UNREFPARM(wIDN);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}


/***************************************************************************************************/
/** \brief Initiate CoE SDO download transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emCoeSdoDownloadReq(
    EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_MBXTFER*           pMbxTfer,           /**< [in]   mailbox transfer object */
    EC_T_DWORD              dwSlaveId,          /**< [in]   slave id to download the SDO */
    EC_T_WORD               wObIndex,           /**< [in]   object index */
    EC_T_BYTE               byObSubIndex,       /**< [in]   object sub-index */
    EC_T_DWORD              dwTimeout,          /**< [in]   Timeout in milliseconds */
    EC_T_DWORD              dwFlags             /**< [in]   mailbox transfer flags */
                                           )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        EC_ERRORMSGC(("emCoeSdoDownloadReq: Invalid Master Instance ID %d\n", dwInstanceID));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        EC_ERRORMSGC(("emCoeSdoDownloadReq: Master Instance not initialized %d\n", dwInstanceID));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->CoeSdoDownloadReq(pMbxTfer, dwSlaveId, wObIndex, byObSubIndex, dwTimeout, dwFlags);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief CoE SDO download transfer

\return Depends on the implementation of the function OsSetLastError(),
        normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD emCoeSdoDownload(
     EC_T_DWORD     dwInstanceID    /* [in]  Master Instance ID */
    ,EC_T_DWORD     dwSlaveId       /* [in]  slave ID */
    ,EC_T_WORD      wObIndex        /* [in]  SDO index */
    ,EC_T_BYTE      byObSubIndex    /* [in]  SDO sub-index */
    ,EC_T_BYTE*     pbyData         /* [in]  SDO data */
    ,EC_T_DWORD     dwDataLen       /* [in]  length of pbyData buffer */
    ,EC_T_DWORD     dwTimeout       /* [in]  timeout in msec, must not be set to EC_NOWAIT */
    ,EC_T_DWORD     dwFlags         /* [in]  mailbox transfer flags */
                                      )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        EC_ERRORMSGC(("emCoeSdoDownload: Invalid Master Instance ID %d\n", dwInstanceID));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        EC_ERRORMSGC(("emCoeSdoDownload: Master Instance not initialized %d\n", dwInstanceID));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwRetVal = pEmInst->CoeSdoDownload(dwSlaveId, wObIndex, byObSubIndex, pbyData, dwDataLen, dwTimeout, dwFlags);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Initiate CoE SDO upload transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emCoeSdoUploadReq(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER*           pMbxTfer,       /**< [in]   mailbox transfer object */
    EC_T_DWORD              dwSlaveId,      /**< [in]   slave id to upload the SDO */
    EC_T_WORD               wObIndex,       /**< [in]   object index */
    EC_T_BYTE               byObSubIndex,   /**< [in]   object sub-index */
    EC_T_DWORD              dwTimeout,      /**< [in]   Timeout in milliseconds */
    EC_T_DWORD              dwFlags         /**< [in]   mailbox transfer flags, see EC_MAILBOX_FLAG */
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->CoeSdoUploadReq(pMbxTfer, dwSlaveId, wObIndex, byObSubIndex, dwTimeout, dwFlags);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief CoE SDO upload transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emCoeSdoUpload(
     EC_T_DWORD     dwInstanceID    /* [in]   Master Instance ID */
    ,EC_T_DWORD     dwSlaveId       /* [in]  slave ID */
    ,EC_T_WORD      wObIndex        /* [in]  SDO index */
    ,EC_T_BYTE      byObSubIndex    /* [in]  SDO sub-index */
    ,EC_T_BYTE*     pbyData         /* [out] SDO data */
    ,EC_T_DWORD     dwDataLen       /* [in]  length of pbyData buffer */
    ,EC_T_DWORD*    pdwOutDataLen   /* [out] SDO data length (number of received bytes) */
    ,EC_T_DWORD     dwTimeout       /* [in]  timeout in msec, must not be set to EC_NOWAIT */
    ,EC_T_DWORD     dwFlags         /* [in]  mailbox transfer flags, see EC_MAILBOX_FLAG */
                                      )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->CoeSdoUpload(dwSlaveId, wObIndex, byObSubIndex, pbyData, dwDataLen, pdwOutDataLen, dwTimeout, dwFlags);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/** \brief Initiate CoE Object Dictionary request transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emCoeGetODList(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER*           pMbxTfer,       /**< [in] mailbox transfer object */
    EC_T_DWORD              dwSlaveId,      /**< [in] slave id to determine the OD list */
    EC_T_COE_ODLIST_TYPE    eListType,      /**< [in] which object types shall be transferred */
    EC_T_DWORD              dwTimeout       /**< [in] Timeout in milliseconds */
                                      )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        EC_ERRORMSGC(("emCoeGetODList: Invalid Master Instance ID %d\n", dwInstanceID));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        EC_ERRORMSGC(("emCoeGetODList: Master Instance not initialized %d\n", dwInstanceID));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->CoeGetODList(pMbxTfer, dwSlaveId, eListType, dwTimeout);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/** \brief Initiate CoE Get Object Description transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emCoeGetObjectDesc(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER*           pMbxTfer,       /**< [in] mailbox transfer object */
    EC_T_DWORD              dwSlaveId,      /**< [in] slave id to download the SDO */
    EC_T_WORD               wObIndex,       /**< [in] object index */
    EC_T_DWORD              dwTimeout       /**< [in] Timeout in milliseconds */
                                          )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->CoeGetObjectDesc(pMbxTfer, dwSlaveId, wObIndex, dwTimeout);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/** \brief Initiate CoE Get Object Entry Description transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emCoeGetEntryDesc(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER*           pMbxTfer,       /**< [in] mailbox transfer object */
    EC_T_DWORD              dwSlaveId,      /**< [in] slave id to download the SDO */
    EC_T_WORD               wObIndex,       /**< [in] object index */
    EC_T_BYTE               byObSubIndex,   /**< [in] object sub-index */
    EC_T_BYTE               byValueInfo,    /**< [in] value info selection */
    EC_T_DWORD              dwTimeout       /**< [in] Timeout in milliseconds */
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->CoeGetEntryDesc(pMbxTfer, dwSlaveId, wObIndex, byObSubIndex, byValueInfo, dwTimeout);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/** \brief Initiate CoE RxPDO transfer

\return Depends on the implementation of the function OsSetLastError(),
            normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD emCoeRxPdoTfer(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER*           pMbxTfer,       /**< [in]  mailbox transfer object */
    EC_T_DWORD              dwSlaveId,      /**< [in]  slave id to download the SDO */
    EC_T_DWORD              dwNumber,       /**< [in]  PDO number */
    EC_T_DWORD              dwTimeout       /**< [in]  Timeout in milliseconds */
                                     )
{
#if (defined INCLUDE_COE_PDO_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->CoeRxPdoTfer(pMbxTfer, dwSlaveId, dwNumber, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pMbxTfer);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(dwNumber);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/** \brief Set the EtherCAT master stack into the requested state.

    \return Depends on the implementation of the function OsSetLastError(),
    normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emSetMasterState(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD              dwTimeout,     /**< [in] Timeout in milliseconds */
    EC_T_STATE              eReqState      /**< [in] Requested System state  */
                                          )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->SetMasterState(dwTimeout, eReqState);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/** \brief Get the EtherCAT master current state.

\return Current Master State
*/
ATECAT_API EC_T_STATE  emGetMasterState(
    EC_T_DWORD              dwInstanceID    /**< [in]   Master Instance ID */
                                        )
{
    EC_T_STATE      eState  = eEcatState_UNKNOWN;
    CAtEmInterface* pEmInst = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        eState = eEcatState_UNKNOWN;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        eState = eEcatState_UNKNOWN;
        goto Exit;
    }
    eState = pEmInst->GetMasterState();

Exit:
    return eState;
}


/***************************************************************************************************/
/** \brief Execute a single EtherCAT job (user controlled mode)

This function executes a single specific EtherCAT job.

\return Depends on the implementation of the function OsSetLastError(),
        normally returns EC_E_NOERROR if it succeeds or error code if it fails
*/
ATECAT_API EC_T_DWORD  emExecJob(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_USER_JOB           eUserJob,   /**< [in]  user requested job */
    EC_T_PVOID              pvParam     /**< [in]  optional parameter */
                                   )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRetVal = pEmInst->ExecJobPrivate(eUserJob, pvParam, EC_FALSE);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get Process Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetProcessData(
    EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_BOOL   bOutputData,    /**< [in]   EC_TRUE if requested data is output data */
    EC_T_DWORD  dwOffset,       /**< [in]   Process data offset */
    EC_T_BYTE*  pbyData,        /**< [in]   Data buffer */
    EC_T_DWORD  dwLength,       /**< [in]   Buffer Length */
    EC_T_DWORD  dwTimeout       /**< [in]   Timeout value */
                                        )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->GetProcessData(bOutputData, dwOffset, pbyData, dwLength, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Set Process Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emSetProcessData(
    EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_BOOL   bOutputData,    /**< [in]   EC_TRUE if requested data is output data */
    EC_T_DWORD  dwOffset,       /**< [in]   Process data offset */
    EC_T_BYTE*  pbyData,        /**< [in]   Data buffer */
    EC_T_DWORD  dwLength,       /**< [in]   Buffer Length */
    EC_T_DWORD  dwTimeout       /**< [in]   Timeout value */
                                        )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->SetProcessData(bOutputData, dwOffset, pbyData, dwLength, dwTimeout);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Writes a specific number of bits from a given buffer to the process image with a bit offset.
        This function must not be called from the job task (death lock).

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emSetProcessDataBits(
    EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_BOOL   bOutputData,
    EC_T_DWORD  dwBitOffsetPd,
    EC_T_BYTE*  pbyDataSrc,
    EC_T_DWORD  dwBitLengthSrc,
    EC_T_DWORD  dwTimeout
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->SetProcessDataBits(bOutputData, dwBitOffsetPd, pbyDataSrc, dwBitLengthSrc, dwTimeout);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Read a specific number of bits from the process image to the given buffer with a bit offset.
        This function must not be called from the job task (death lock).

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetProcessDataBits(
    EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_BOOL   bOutputData,
    EC_T_DWORD  dwBitOffsetPd,
    EC_T_BYTE*  pbyDataDst,
    EC_T_DWORD  dwBitLengthDst,
    EC_T_DWORD  dwTimeout                   )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->GetProcessDataBits(bOutputData, dwBitOffsetPd, pbyDataDst, dwBitLengthDst, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Force Process Data: Overrules the data set by application!
        This function must not be called from the job task (death lock).

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emForceProcessDataBits(
    EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD  dwClientId,
    EC_T_BOOL   bOutputData,
    EC_T_DWORD  dwBitOffsetPd,
    EC_T_WORD   wBitLength,
    EC_T_BYTE*  pbyData,
    EC_T_DWORD  dwTimeout)
{
#if (defined INCLUDE_FORCE_PROCESSDATA)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->ForceProcessDataBits(dwClientId, bOutputData, dwBitOffsetPd, wBitLength, pbyData, dwTimeout);
Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwClientId);
    EC_UNREFPARM(bOutputData);
    EC_UNREFPARM(dwBitOffsetPd);
    EC_UNREFPARM(wBitLength);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(dwTimeout);

    return EC_E_NOTSUPPORTED;
#endif
}


/***************************************************************************************************/
/**
\brief  Release Process Data
        This function must not be called from the job task (death lock).

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emReleaseProcessDataBits(
    EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD  dwClientId,
    EC_T_BOOL   bOutputData,
    EC_T_DWORD  dwBitOffsetPd,
    EC_T_WORD   wBitLength,
    EC_T_DWORD  dwTimeout)
{
#if (defined INCLUDE_FORCE_PROCESSDATA)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->ReleaseProcessDataBits(dwClientId, bOutputData, EC_FALSE, dwBitOffsetPd, wBitLength, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwClientId);
    EC_UNREFPARM(bOutputData);
    EC_UNREFPARM(dwBitOffsetPd);
    EC_UNREFPARM(wBitLength);
    EC_UNREFPARM(dwTimeout);

    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Release All Process Data
        This function must not be called from the job task (death lock).

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emReleaseAllProcessDataBits(
    EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD  dwClientId,
    EC_T_DWORD  dwTimeout)
{
#if (defined INCLUDE_FORCE_PROCESSDATA)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->ReleaseProcessDataBits(dwClientId, EC_FALSE, EC_TRUE, 0, 0, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwClientId);
    EC_UNREFPARM(dwTimeout);

    return EC_E_NOTSUPPORTED;
#endif
}


/***************************************************************************************************/
/**
\brief  Perform a FoE File Upload.

A File is uploaded from the Slave. Call blocks until successful upload or timeout expiry.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emFoeFileUpload(
    EC_T_DWORD      dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD      dwSlaveId,      /**< [in]   slave id to upload the File */
    EC_T_CHAR*      achFileName,    /**< [in]   slave file name to upload */
    EC_T_DWORD      dwFileNameLen,  /**< [in]   length of achFileName */
    EC_T_BYTE*      pbyData,        /**< [in]   buffer to upload file to */
    EC_T_DWORD      dwDataLen,      /**< [in]   size of data buffer */
    EC_T_DWORD*     pdwOutDataLen,  /**< [out]  FOE data length (number of received bytes) */
    EC_T_DWORD      dwPassword,     /**< [in]   slave password */
    EC_T_DWORD      dwTimeout       /**< [in]   timeout in msecs */
)
{
#if (defined INCLUDE_FOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->FoeFileUpload(dwSlaveId, achFileName, dwFileNameLen, pbyData, dwDataLen, pdwOutDataLen, dwPassword, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(achFileName);
    EC_UNREFPARM(dwFileNameLen);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(dwDataLen);
    EC_UNREFPARM(pdwOutDataLen);
    EC_UNREFPARM(dwPassword);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}


/***************************************************************************************************/
/**
\brief  Perform a FoE file download from buffer.

A File is downloaded to the Slave. Call blocks until successful download or timeout expiry.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emFoeFileDownload(
    EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD  dwSlaveId,      /**< [in]   slave id to download the File */
    EC_T_CHAR*  achFileName,    /**< [in]   slave file name to download */
    EC_T_DWORD  dwFileNameLen,  /**< [in]   length of achFileName */
    EC_T_BYTE*  pbyData,        /**< [in]   buffer to download file from */
    EC_T_DWORD  dwDataLen,      /**< [in]   size of data buffer */
    EC_T_DWORD  dwPassword,     /**< [in]   slave password */
    EC_T_DWORD  dwTimeout       /**< [in]   timeout in msecs */
                                         )
{
#if (defined INCLUDE_FOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->FoeFileDownload(dwSlaveId, achFileName, dwFileNameLen, pbyData, dwDataLen, dwPassword, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(achFileName);
    EC_UNREFPARM(dwFileNameLen);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(dwDataLen);
    EC_UNREFPARM(dwPassword);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Initiate a FoE File Upload.

A File is uploaded from the Slave. Call returns immediately result is raised by a notification.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emFoeUploadReq(
    EC_T_DWORD      dwInstanceID,                       /**< [in]   Master Instance ID */
    EC_T_MBXTFER*   pMbxTfer,                           /**< [in]   previously allocated mailbox transfer object, carrying data buffers */
    EC_T_DWORD      dwSlaveId,                          /**< [in]   slave id to upload the file */
    EC_T_CHAR*      achFileName,                        /**< [in]   slave file name to download */
    EC_T_DWORD      dwFileNameLen,                      /**< [in]   length of achFileName */
    EC_T_DWORD      dwPassword,                         /**< [in]   slave password*/
    EC_T_DWORD      dwTimeout                           /**< [in]   timeout in msecs */
                                      )
{
#if (defined INCLUDE_FOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->FoeUploadReq(pMbxTfer, dwSlaveId, achFileName, dwFileNameLen, dwPassword, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pMbxTfer);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(achFileName);
    EC_UNREFPARM(dwFileNameLen);
    EC_UNREFPARM(dwPassword);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Initiate a FoE File Download.

A File is downloaded to the Slave. Call returns immediately result is raised by a notification.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emFoeDownloadReq(
    EC_T_DWORD      dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER*   pMbxTfer,       /**< [in]   previously allocated mailbox transfer object, carrying data buffers */
    EC_T_DWORD      dwSlaveId,      /**< [in]   slave id to download the file */
    EC_T_CHAR*      achFileName,    /**< [in]   Name of slave file to read */
    EC_T_DWORD      dwFileNameLen,  /**< [in]   length of achFileName */
    EC_T_DWORD      dwPassword,     /**< [in]   slave password*/
    EC_T_DWORD      dwTimeout       /**< [in]   timeout in msecs */
)
{
#if (defined INCLUDE_FOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->FoeDownloadReq(pMbxTfer, dwSlaveId, achFileName, dwFileNameLen, dwPassword, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pMbxTfer);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(achFileName);
    EC_UNREFPARM(dwFileNameLen);
    EC_UNREFPARM(dwPassword);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Performs segmented FoE upload to file.

        Upload file from Slave. Call returns immediately. Master raises result as notification.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emFoeSegmentedUploadReq(
    EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER* pMbxTfer,     /**< [in]   previously allocated mailbox transfer object (context) */
    EC_T_DWORD  dwSlaveId,      /**< [in]   slave id to download the File */
    EC_T_CHAR*  szFileName,     /**< [in]   file name at slave */
    EC_T_DWORD  dwFileNameLen,  /**< [in]   length of szFileName */
    EC_T_DWORD  dwFileSize,     /**< [in]   file size */
    EC_T_DWORD  dwPassword,     /**< [in]   slave password */
    EC_T_DWORD  dwTimeout       /**< [in]   timeout in msecs */
)
{
#if (defined INCLUDE_FOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->FoeSegmentedUploadReq(pMbxTfer, dwSlaveId, szFileName, dwFileNameLen, dwFileSize, dwPassword, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pMbxTfer);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(szFileName);
    EC_UNREFPARM(dwFileNameLen);
    EC_UNREFPARM(dwFileSize);
    EC_UNREFPARM(dwPassword);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Performs segmented FoE download from file.

        Download file to Slave. Call returns immediately. Master raises result as notification.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emFoeSegmentedDownloadReq(
    EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER* pMbxTfer,     /**< [in]   previously allocated mailbox transfer object (context) */
    EC_T_DWORD  dwSlaveId,      /**< [in]   slave id to download the File */
    EC_T_CHAR*  szFileName,     /**< [in]   file name at slave */
    EC_T_DWORD  dwFileNameLen,  /**< [in]   length of szFileName */
    EC_T_DWORD  dwFileSize,     /**< [in]   file size */
    EC_T_DWORD  dwPassword,     /**< [in]   slave password */
    EC_T_DWORD  dwTimeout       /**< [in]   timeout in msecs */
)
{
#if (defined INCLUDE_FOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->FoeSegmentedDownloadReq(pMbxTfer, dwSlaveId, szFileName, dwFileNameLen, dwFileSize, dwPassword, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pMbxTfer);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(szFileName);
    EC_UNREFPARM(dwFileNameLen);
    EC_UNREFPARM(dwFileSize);
    EC_UNREFPARM(dwPassword);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Register the EoE Endpoint (driver)
        See lots of documentation in the ``CEoEUplink`` class and the ``EoELinkRegister()`` member
        function.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emEoeRegisterEndpoint(
     EC_T_DWORD              dwInstanceID             /**< [in]   Master Instance ID                     */
    ,EC_T_CHAR*              szEoEDrvIdent            /**< [in]   String identifying the EoE endpoint    */
    ,EC_T_LINK_DRV_DESC*     pLinkDrvDesc             /**< [out]  Pointer to a EoE link layer descriptor */
    )
{
#if (defined INCLUDE_EOE_ENDPOINT) && (defined INCLUDE_EOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->EoeRegisterEndpoint(szEoEDrvIdent, (EC_T_LINK_DRV_DESC*)pLinkDrvDesc);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(szEoEDrvIdent);
    EC_UNREFPARM(pLinkDrvDesc);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Unregister the EoE Endpoint (driver)
        See lots of documentation in the ``CEoEUplink`` class and the ``EoELinkRegister()`` member
        function.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emEoeUnregisterEndpoint(
     EC_T_DWORD              dwInstanceID             /**< [in]   Master Instance ID                     */
    ,EC_T_LINK_DRV_DESC*     pLinkDrvDesc             /**< [in]   Pointer to a EoE link layer descriptor */
    )
{
#if (defined INCLUDE_EOE_ENDPOINT) && (defined INCLUDE_EOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->EoeUnregisterEndpoint(pLinkDrvDesc);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pLinkDrvDesc);
    return EC_E_NOTSUPPORTED;
#endif
}


/***************************************************************************************************/
/**
\brief  Perform a VoE read.

A VoE Mailbox is uploaded from the Slave. Call blocks until successful upload or timeout expiry.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emVoeRead(
    EC_T_DWORD      dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD      dwSlaveId,      /**< [in]   slave id */
    EC_T_BYTE*      pbyData,        /**< [in]   buffer to upload VoE mailbox */
    EC_T_DWORD      dwDataLen,      /**< [in]   size of data buffer */
    EC_T_DWORD*     pdwOutDataLen,  /**< [out]  number of received bytes */
    EC_T_DWORD      dwTimeout       /**< [in]   timeout in msecs */
                                       )
{
#if (defined INCLUDE_VOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->VoeRead(dwSlaveId, pbyData, dwDataLen, pdwOutDataLen, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(dwDataLen);
    EC_UNREFPARM(pdwOutDataLen);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Perform a VoE write.

A File is downloaded to the Slave. Call blocks until successful download or timeout expiry.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emVoeWrite(
    EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD  dwSlaveId,      /**< [in]   slave id */
    EC_T_BYTE*  pbyData,        /**< [in]   buffer to download VoE mailbox */
    EC_T_DWORD  dwDataLen,      /**< [in]   size of data buffer */
    EC_T_DWORD  dwTimeout       /**< [in]   timeout in msecs */
                                         )
{
#if (defined INCLUDE_VOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->VoeWrite(dwSlaveId,  pbyData, dwDataLen, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(dwDataLen);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Initiate a VoE write.

A File is downloaded to the Slave. Call returns immediately result is raised by a notification.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emVoeWriteReq(
    EC_T_DWORD      dwInstanceID,                       /**< [in]   Master Instance ID */
    EC_T_MBXTFER*   pMbxTfer,                           /**< [in]   previously allocated mailbox transfer object, carrying data buffers */
    EC_T_DWORD      dwSlaveId,                          /**< [in]   slave id to download the VoE mailbox */
    EC_T_DWORD      dwTimeout                           /**< [in]   timeout in msecs */
                                        )
{
#if (defined INCLUDE_VOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->VoeWriteReq(pMbxTfer, dwSlaveId, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pMbxTfer);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Retrieves the NetID of a specific EtherCAT device
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emAoeGetSlaveNetId(
    EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_DWORD              dwSlaveId,      /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID*         poAoeNetId      /**< [out]  AoE NetID of the corresponding slave */
    )
{
#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->AoeGetSlaveNetId(dwSlaveId, poAoeNetId);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(poAoeNetId);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Performs a AoE mailbox read request to an EtherCAT slave device.
A non blocking version of this function (ecatAoeReadReq) is also available.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
ATECAT_API EC_T_DWORD  emAoeRead(
    EC_T_DWORD  dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_DWORD  dwSlaveId,          /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId().*/
    EC_T_WORD   wTargetPort,        /**< [in]   Target port. */
    EC_T_DWORD  dwIndexGroup,       /**< [in]   AoE read command index group. */
    EC_T_DWORD  dwIndexOffset,      /**< [in]   AoE read command index offset*/
    EC_T_DWORD  dwDataLen,          /**< [in]   Number of bytes to read from the target device. Must correspond with the size of pbyData. */
    EC_T_BYTE*  pbyData,            /**< [in]   Data buffer to store the read data */
    EC_T_DWORD* pdwDataOutLen,      /**< [out]  Number of bytes read from the target device */
    EC_T_DWORD* pdwErrorCode,       /**< [out]  AoE response error code*/
    EC_T_DWORD* pdwCmdResult,       /**< [out]  AoE read command result code */
    EC_T_DWORD  dwTimeout           /**< [in]   Timeout in milliseconds. The function will block at most for this time */
    )
{
#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->AoeRead(dwSlaveId, poTargetNetId, wTargetPort, dwIndexGroup, dwIndexOffset, dwDataLen, pbyData, pdwDataOutLen, pdwErrorCode, pdwCmdResult, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(poTargetNetId);
    EC_UNREFPARM(wTargetPort);
    EC_UNREFPARM(dwIndexGroup);
    EC_UNREFPARM(dwIndexOffset);
    EC_UNREFPARM(dwDataLen);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(pdwDataOutLen);
    EC_UNREFPARM(pdwErrorCode);
    EC_UNREFPARM(pdwCmdResult);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Performs a AoE mailbox read request to an EtherCAT slave device.
If the timeout value was set to EC_NOWAIT this function returns immediately without blocking.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
ATECAT_API EC_T_DWORD  emAoeReadReq(
    EC_T_DWORD      dwInstanceID,   /**< [in]   Master Instance ID */
    EC_T_MBXTFER*   pMbxTfer,       /**< [in]   Pointer to the corresponding mailbox transfer object. The data to write have to be stored at pMbxTfer->pbyData. */
    EC_T_DWORD      dwSlaveId,      /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used.*/
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId(). */
    EC_T_WORD       wTargetPort,    /**< [in]   Target port.  */
    EC_T_DWORD      dwIndexGroup,   /**< [in]   AoE read command index group */
    EC_T_DWORD      dwIndexOffset,  /**< [in]   AoE read command index offset*/
    EC_T_DWORD      dwTimeout       /**< [in]   Timeout in milliseconds. The function will block at most for this time.*/
    )
{
#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->AoeReadReq(pMbxTfer, dwSlaveId, poTargetNetId, wTargetPort, dwIndexGroup, dwIndexOffset, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pMbxTfer);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(poTargetNetId);
    EC_UNREFPARM(wTargetPort);
    EC_UNREFPARM(dwIndexGroup);
    EC_UNREFPARM(dwIndexOffset);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Performs a AoE mailbox write request to an EtherCAT slave device.
A non blocking version of this function (ecatAoeWriteReq) is also available.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
ATECAT_API EC_T_DWORD  emAoeWrite(
    EC_T_DWORD  dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_DWORD  dwSlaveId,          /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId(). */
    EC_T_WORD   wTargetPort,        /**< [in]   Target port.  */
    EC_T_DWORD  dwIndexGroup,       /**< [in]   AoE write command index group. */
    EC_T_DWORD  dwIndexOffset,      /**< [in]   AoE write command index offset*/
    EC_T_DWORD  dwDataLen,          /**< [in]   Number of bytes to write to the target device. Must correspond with the size of pbyData.*/
    EC_T_BYTE*  pbyData,            /**< [in]   Data buffer with data that shall be written*/
    EC_T_DWORD* pdwErrorCode,       /**< [out]  AoE response error code. */
    EC_T_DWORD* pdwCmdResult,       /**< [out]  AoE write command result code. */
    EC_T_DWORD  dwTimeout           /**< [in]   Timeout in milliseconds. The function will block at most for this time. */
    )
{
#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->AoeWrite(dwSlaveId, poTargetNetId, wTargetPort, dwIndexGroup, dwIndexOffset, dwDataLen, pbyData, pdwErrorCode, pdwCmdResult, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(poTargetNetId);
    EC_UNREFPARM(wTargetPort);
    EC_UNREFPARM(dwIndexGroup);
    EC_UNREFPARM(dwIndexOffset);
    EC_UNREFPARM(dwDataLen);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(pdwErrorCode);
    EC_UNREFPARM(pdwCmdResult);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Performs a AoE mailbox write request to an EtherCAT slave device.
If the timeout value was set to EC_NOWAIT this function returns immediately without blocking.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
ATECAT_API EC_T_DWORD  emAoeWriteReq(
    EC_T_DWORD  dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_MBXTFER* pMbxTfer,         /**< [in]   Pointer to the corresponding mailbox transfer object. The data to write have to be stored at pMbxTfer->pbyData. */
    EC_T_DWORD  dwSlaveId,          /**< [in]   [in]    EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId(). */
    EC_T_WORD   wTargetPort,        /**< [in]   Target port.  */
    EC_T_DWORD  dwIndexGroup,       /**< [in]   AoE write command index group. */
    EC_T_DWORD  dwIndexOffset,      /**< [in]   AoE write command index offset*/
    EC_T_DWORD  dwTimeout           /**< [in]   Timeout in milliseconds. The function will block at most for this time. */
    )
{
#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->AoeWriteReq(pMbxTfer, dwSlaveId, poTargetNetId, wTargetPort, dwIndexGroup, dwIndexOffset, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pMbxTfer);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(poTargetNetId);
    EC_UNREFPARM(wTargetPort);
    EC_UNREFPARM(dwIndexGroup);
    EC_UNREFPARM(dwIndexOffset);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Performs a AoE mailbox read/write request to an EtherCAT slave device.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
ATECAT_API EC_T_DWORD  emAoeReadWrite(
    EC_T_DWORD  dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_DWORD  dwSlaveId,          /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId().*/
    EC_T_WORD   wTargetPort,        /**< [in]   Target port. */
    EC_T_DWORD  dwIndexGroup,       /**< [in]   AoE read/write command index group. */
    EC_T_DWORD  dwIndexOffset,      /**< [in]   AoE read/write command index offset*/
    EC_T_DWORD  dwReadDataLen,      /**< [in]   Number of bytes to read from the target device. Must correspond with the size of pbyData. */
    EC_T_DWORD  dwWriteDataLen,     /**< [in]   Number of bytes to read from the target device. Must correspond with the size of pbyData. */
    EC_T_BYTE*  pbyData,            /**< [in]   Data buffer to store the read data */
    EC_T_DWORD* pdwDataOutLen,      /**< [out]  Number of bytes read from the target device */
    EC_T_DWORD* pdwErrorCode,       /**< [out]  AoE response error code*/
    EC_T_DWORD* pdwCmdResult,       /**< [out]  AoE read command result code */
    EC_T_DWORD  dwTimeout           /**< [in]   Timeout in milliseconds. The function will block at most for this time */
    )
{
#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->AoeReadWrite(dwSlaveId, poTargetNetId, wTargetPort, dwIndexGroup, dwIndexOffset, dwReadDataLen, dwWriteDataLen, pbyData, pdwDataOutLen, pdwErrorCode, pdwCmdResult, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(poTargetNetId);
    EC_UNREFPARM(wTargetPort);
    EC_UNREFPARM(dwIndexGroup);
    EC_UNREFPARM(dwIndexOffset);
    EC_UNREFPARM(dwReadDataLen);
    EC_UNREFPARM(dwWriteDataLen);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(pdwDataOutLen);
    EC_UNREFPARM(pdwErrorCode);
    EC_UNREFPARM(pdwCmdResult);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Performs a AoE write control request to an EtherCAT slave device.

\return
    - EC_E_NOERROR
    - EC_E_AOE_VENDOR_SPECIFIC: will be returned in case the AoE device has reponsed with an user defined error code.
    - Other error codes
*/
ATECAT_API EC_T_DWORD  emAoeWriteControl(
    EC_T_DWORD  dwInstanceID,       /**< [in]   Master Instance ID */
    EC_T_DWORD  dwSlaveId,          /**< [in]   EtherCAT slave ID. To determine the slave ID the function ecatGetSlaveId has to be used. */
    EC_T_AOE_NETID* poTargetNetId,  /**< [in]   Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId(). */
    EC_T_WORD   wTargetPort,        /**< [in]   Target port.  */
    EC_T_WORD   wAoEState,          /**< [in]   AoE state */
    EC_T_WORD   wDeviceState,       /**< [in]   Device specific state */
    EC_T_DWORD  dwDataLen,          /**< [in]   Number of bytes to write to the target device. Must correspond with the size of pbyData.*/
    EC_T_BYTE*  pbyData,            /**< [in]   Data buffer with data that shall be written*/
    EC_T_DWORD* pdwErrorCode,       /**< [out]  AoE response error code. */
    EC_T_DWORD* pdwCmdResult,       /**< [out]  AoE write command result code. */
    EC_T_DWORD  dwTimeout           /**< [in]   Timeout in milliseconds. The function will block at most for this time. */
    )
{
#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->AoeWriteControl(dwSlaveId, poTargetNetId, wTargetPort, wAoEState, wDeviceState, dwDataLen, pbyData, pdwErrorCode, pdwCmdResult, dwTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwSlaveId);
    EC_UNREFPARM(poTargetNetId);
    EC_UNREFPARM(wTargetPort);
    EC_UNREFPARM(wAoEState);
    EC_UNREFPARM(wDeviceState);
    EC_UNREFPARM(dwDataLen);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(pdwErrorCode);
    EC_UNREFPARM(pdwCmdResult);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}


/***************************************************************************************************/
/**
\brief  Returns total amount of currently connected Slaves.

BRD AlStatus Read WKC Value.
\return WKC Value.
*/
ATECAT_API EC_T_DWORD emGetNumConnectedSlaves(
    EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
                                            )
{
    EC_T_DWORD      dwNumSlaves = 0;
    CAtEmInterface* pEmInst     = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
    dwNumSlaves = pEmInst->GetNumConnectedSlaves();

Exit:
    return dwNumSlaves;
}

/***************************************************************************************************/
/**
\brief  Returns amount of currently connected Slaves on main side.

BRD AlStatus Read WKC Value.
\return WKC Value.
*/
ATECAT_API EC_T_DWORD emGetNumConnectedSlavesMain(
                                   EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
                                   )
{
    EC_T_DWORD      dwNumSlaves = 0;
    CAtEmInterface* pEmInst     = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
#if (defined INCLUDE_RED_DEVICE)
    dwNumSlaves = pEmInst->GetNumConnectedSlavesMain();
#else
    dwNumSlaves = pEmInst->GetNumConnectedSlaves();
#endif
Exit:
    return dwNumSlaves;
}

/***************************************************************************************************/
/**
\brief  Returns amount of currently connected Slaves on red side.

BRD AlStatus Read WKC Value.
\return WKC Value.
*/
ATECAT_API EC_T_DWORD emGetNumConnectedSlavesRed(
                                       EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
                                       )
{
    EC_T_DWORD      dwNumSlaves = 0;
    CAtEmInterface* pEmInst     = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
#if (defined INCLUDE_RED_DEVICE)
    dwNumSlaves = pEmInst->GetNumConnectedSlavesRed();
#endif
Exit:
    return dwNumSlaves;
}

/***************************************************************************************************/
/**
\brief  Read slave EEPRom Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emReadSlaveEEPRom(
    EC_T_DWORD              dwInstanceID,           /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddressing,       /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD               wSlaveAddress,          /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_WORD               wEEPRomStartOffset,     /**< [in]   Address to start EEPRom Read from.*/
    EC_T_WORD*              pwReadData,             /**< [in]   Pointer to WORD array to carry the read data */
    EC_T_DWORD              dwReadLen,              /**< [in]   Size of the EC_T_WORD array provided at pwReadData (in EC_T_WORDs) */
    EC_T_DWORD*             pdwNumOutData,          /**< [out]  Pointer to DWORD carrying actually read data after completion */
    EC_T_DWORD              dwTimeout               /**< [in]   Timeout in milliseconds */
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->ReadSlaveEEPRom(bFixedAddressing, wSlaveAddress, wEEPRomStartOffset, pwReadData, dwReadLen, pdwNumOutData, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Write slave EEPRom Data.

\return EC_E_NOERROR on success error code otherwise.
*/
ATECAT_API EC_T_DWORD  emWriteSlaveEEPRom(
    EC_T_DWORD              dwInstanceID,           /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddressing,       /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD               wSlaveAddress,          /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_WORD               wEEPRomStartOffset,     /**< [in]   Address to start EEPRom Write from */
    EC_T_WORD*              pwWriteData,            /**< [in]   Pointer to WORD array carrying the write data */
    EC_T_DWORD              dwWriteLen,             /**< [in]   Sizeof Write Data WORD array (in WORDS) */
    EC_T_DWORD              dwTimeout               /**< [in]   Timeout in milliseconds */
                                          )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->WriteSlaveEEPRom(bFixedAddressing, wSlaveAddress, wEEPRomStartOffset, pwWriteData, dwWriteLen, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Reload slave EEPRom.

\return EC_E_NOERROR on success error code otherwise.
*/
ATECAT_API EC_T_DWORD  emReloadSlaveEEPRom(
    EC_T_DWORD              dwInstanceID,           /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddressing,       /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD               wSlaveAddress,          /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_DWORD              dwTimeout               /**< [in]   Timeout in milliseconds */
                                          )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->ReloadSlaveEEPRom(bFixedAddressing, wSlaveAddress, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Reset slave ESC Node.

\return EC_E_NOERROR on success error code otherwise.
*/
ATECAT_API EC_T_DWORD  emResetSlaveController(
    EC_T_DWORD              dwInstanceID,           /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddressing,       /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    EC_T_WORD               wSlaveAddress,          /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
    EC_T_DWORD              dwTimeout               /**< [in]   Timeout in milliseconds */
                                  )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->ResetSlaveController(bFixedAddressing, wSlaveAddress, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Assign Slave EEPRom to Slave PDI Application.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emAssignSlaveEEPRom(
     EC_T_DWORD  dwInstanceID                /**< [in]   Master Instance ID */
    ,EC_T_BOOL   bFixedAddressing            /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    ,EC_T_WORD   wSlaveAddress               /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
    ,EC_T_BOOL   bSlavePDIAccessEnable       /**< [in]   EC_TRUE: assign EEPRom to slave PDI application, EC_FALSE: assign EEPRom to ECat Master */
    ,EC_T_BOOL   bForceAssign                /**< [in]   EC_TRUE: force Assignment, only available for ECat Master Assignment, EC_FALSE: don't force */
    ,EC_T_DWORD  dwTimeout                   /**< [in]   Timeout in milliseconds */
                                          )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->AssignSlaveEEPRom( bFixedAddressing, wSlaveAddress, bSlavePDIAccessEnable, bForceAssign, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Check whether Slave PDI application marks EEPRom Access active.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emActiveSlaveEEPRom(
     EC_T_DWORD  dwInstanceID                /**< [in]   Master Instance ID */
    ,EC_T_BOOL   bFixedAddressing            /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
    ,EC_T_WORD   wSlaveAddress               /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
    ,EC_T_BOOL*  pbSlavePDIAccessActive      /**< [out]  Pointer holding Boolean buffer, EC_TRUE: Slave PDI application EEPRom access active, EC_FALSE: not active */
    ,EC_T_DWORD  dwTimeout                   /**< [in]   Timeout in milliseconds */
                                          )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->ActiveSlaveEEProm( bFixedAddressing, wSlaveAddress, pbSlavePDIAccessActive, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Accept Topology Change.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emHCAcceptTopoChange(
    EC_T_DWORD  dwInstanceID        /**< [in]   Master Instance ID */
                                          )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->HCAcceptTopoChange();

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Determine number of slaves that belong to a specific group.

\return 0: group does not exist. >0: number of slaves.
*/
ATECAT_API EC_T_DWORD emHCGetNumGroupMembers(
      EC_T_DWORD dwInstanceID,      /**< [in]  Master Instance ID */
      EC_T_DWORD dwGroupIndex )     /**< [in]  group index, first group (=0) is the mandatory group */
{
    EC_T_DWORD      dwNumSlaves = 0;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
    dwNumSlaves = pEmInst->HCGetNumGroupMembers( dwGroupIndex );

Exit:
    return dwNumSlaves;
}

/***************************************************************************************************/
/**
\brief  Determine number of slaves that belong to a specific group.

\return 0: group does not exist. >0: number of slaves.
*/
EC_T_DWORD CAtEmInterface::HCGetNumGroupMembers(
    EC_T_DWORD dwGroupIndex )     /**< [in]  group index, first group (=0) is the mandatory group */
{
#if (defined INCLUDE_HOTCONNECT)
    EC_T_DWORD dwNumSlaves = 0;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        goto Exit;
    }
    dwNumSlaves = m_oAtEcatDesc.poMaster->HCGetNumGroupMembers(dwGroupIndex);
Exit:
    return dwNumSlaves;
#else
    EC_UNREFPARM(dwGroupIndex);
    return 0;
#endif
}

/***************************************************************************************************/
/**
\brief  Determine slave IDs of a given HotConnect group.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emHCGetSlaveIdsOfGroup(
    EC_T_DWORD dwInstanceID,        /**< [in]  Master Instance ID */
    EC_T_DWORD  dwGroupIndex,       /**< [in]  group index, first group (=0) is the mandatory group */
    EC_T_DWORD* adwSlaveId,         /**< [out] slave IDs of the group members */
    EC_T_DWORD  dwMaxNumSlaveIds )  /**< [in]  maximum number of slave ids to be stored in adwSlaveId */
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->HCGetSlaveIdsOfGroup( dwGroupIndex, adwSlaveId, dwMaxNumSlaveIds );

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Determine slave IDs of a given HotConnect group.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmInterface::HCGetSlaveIdsOfGroup(
    EC_T_DWORD  dwGroupIndex,       /**< [in]  group index, first group (=0) is the mandatory group */
    EC_T_DWORD* adwSlaveId,         /**< [out] slave IDs of the group members */
    EC_T_DWORD  dwMaxNumSlaveIds )  /**< [in]  maximum number of slave ids to be stored in adwSlaveId */
{
#if (defined INCLUDE_HOTCONNECT)
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* check instance state */
    if (!m_oAtEcatDesc.bInitialized || (EC_NULL == m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = m_oAtEcatDesc.poMaster->HCGetSlaveIdsOfGroup(dwGroupIndex,adwSlaveId,dwMaxNumSlaveIds);
   
Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwGroupIndex);
    EC_UNREFPARM(adwSlaveId);
    EC_UNREFPARM(dwMaxNumSlaveIds);
    return EC_E_NOTSUPPORTED;
#endif  /* #if (defined INCLUDE_HOTCONNECT) */
}

/***************************************************************************************************/
/**
\brief  Open / Close slave port.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emSetSlavePortState(
    EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_T_DWORD      dwSlaveID       /**< [in]   Slave ID */
   ,EC_T_WORD       wPort           /**< [in]   Port Nr. to modify (0-3) */
   ,EC_T_BOOL       bClose          /**< [in]   EC_TRUE: close port, EC_FALSE: open port */
   ,EC_T_BOOL       bForce          /**< [in]   EC_TRUE: force mode (always open / closed),
                                      *         EC_FALSE: grace mode (close after link down, open auto) */
   ,EC_T_DWORD      dwTimeout       /**< [in]   Timeout for appliance of change */
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->SetSlavePortState(dwSlaveID, wPort, bClose, bForce, dwTimeout);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Mailbox transfers are serialized (block parallel transfers)

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emSlaveSerializeMbxTfers(
    EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_T_DWORD      dwSlaveID       /**< [in]   Slave ID */
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->SlaveSerializeMbxTfers(dwSlaveID);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Mailbox transfers are processed in parallel

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emSlaveParallelMbxTfers(
    EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_T_DWORD      dwSlaveID       /**< [in]   Slave ID */
                                         )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->SlaveParallelMbxTfers(dwSlaveID);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Register a client for process data and notifications

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emRegisterClient(
    EC_T_DWORD              dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_PF_NOTIFY            pfnNotify       /**< [in]   notify callback function pointer */
   ,EC_T_VOID*              pCallerData     /**< [in]   used by all callback functions */
   ,EC_T_REGISTERRESULTS*   pRegResults     /**< [out]  Client register results */
                                   )
{
#if (defined INSTRUMENT_MASTER)
    if ( emRegisterClientInstr )
        return emRegisterClientInstr(dwInstanceID, pfnNotify, pCallerData,pRegResults);
#endif
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    {
    EC_T_REGISTERPARMS oRegisterParms;

        oRegisterParms.pCallerData = pCallerData;
        oRegisterParms.pfnNotify   = pfnNotify;

        dwRetVal = pEmInst->RegisterClient(&oRegisterParms, pRegResults);
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Unregister a client for process data and notifications

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emUnregisterClient(
    EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_T_DWORD      dwClntId        /**< [in] Client ID */
                                   )
{
#if (defined INSTRUMENT_MASTER)
    if ( emUnregisterClientInstr )
        return emUnregisterClientInstr(dwInstanceID, dwClntId);
#endif
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    pEmInst->UnregisterClient(dwClntId);
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Enable DC Support

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcEnable(
    EC_T_DWORD          dwInstanceID    /**< [in]   Master Instance ID */
   )
{
    EC_UNREFPARM(dwInstanceID);
#if (defined INCLUDE_DC_SUPPORT )
    return EC_E_NOERROR;
#else
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Disable DC Support

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcDisable(
    EC_T_DWORD          dwInstanceID    /**< [in]   Master Instance ID */
   )
{
    EC_UNREFPARM(dwInstanceID);
#if (defined INCLUDE_DC_SUPPORT )
    return EC_E_NOERROR;
#else
    return EC_E_NOTSUPPORTED;
#endif
}


/***************************************************************************************************/
/**
\brief  Check if DC is enabled in configuration

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcIsEnabled(
    EC_T_DWORD          dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_T_BOOL*          pbDcIsEnabled
   )
{
    EC_UNREFPARM(dwInstanceID);
#if (defined INCLUDE_DC_SUPPORT )
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (pbDcIsEnabled == EC_NULL)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    *pbDcIsEnabled = pEmInst->m_oAtEcatDesc.poMaster->GetDcSupportEnabled();

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
#else
    EC_UNREFPARM( dwInstanceID );
    EC_UNREFPARM( pbDcIsEnabled );
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Enable DC Continuous Propagation Delay Compensation

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcContDelayCompEnable(
    EC_T_DWORD          dwInstanceID    /**< [in]   Master Instance ID */
   )
{
#if (defined INCLUDE_DC_SUPPORT )
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pEmInst->m_oAtEcatDesc.poMaster->m_pDistributedClks->EnableContinuousMeasuring();
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Disable DC Continuous Propagation Delay Compensation

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcContDelayCompDisable(
    EC_T_DWORD          dwInstanceID    /**< [in]   Master Instance ID */
   )
{
#if (defined INCLUDE_DC_SUPPORT )
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pEmInst->m_oAtEcatDesc.poMaster->m_pDistributedClks->DisableContinuousMeasuring();
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Configure DC

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcConfigure(
    EC_T_DWORD             dwInstanceID,       /**< [in]   Master Instance ID */
    struct _EC_T_DC_CONFIGURE* pDcConfigure    /**< [in]   DC Configure parameters */
    )
{
#if (defined INCLUDE_DC_SUPPORT )
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pDcConfigure)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->m_pDistributedClks->Configure(pDcConfigure);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pDcConfigure);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Configure DC master synchonization

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emDcmConfigure(
    EC_T_DWORD       dwInstanceID
   ,struct _EC_T_DCM_CONFIG* pDcmConfig
   ,EC_T_DWORD       dwInSyncTimeout
   )
{
#if (defined INCLUDE_DC_SUPPORT )
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->m_pDistributedClks->DcmConfigure(pDcmConfig);

    /* wait for InSync or timeout, currently not implemented */
    EC_UNREFPARM(dwInSyncTimeout);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pDcmConfig);
    EC_UNREFPARM(dwInSyncTimeout);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Get DC master synchonization status

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcmGetStatus(
      EC_T_DWORD              dwInstanceID    /**< [in]  Master Instance ID */
    , EC_T_DWORD*             pdwErrorCode    /**< [out] DCM controller error code */
    , EC_T_INT*               pnDiffCur       /**< [out] Current difference between set value and actual value of controller in nanosec. */
    , EC_T_INT*               pnDiffAvg       /**< [out] Average difference between set value and actual value of controller in nanosec.*/
    , EC_T_INT*               pnDiffMax       /**< [out] Maximum difference between set value and actual value of controller in nanosec.*/
   )
{
#if (defined INCLUDE_DC_SUPPORT )
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if ((EC_NULL == pdwErrorCode) || (EC_NULL == pnDiffCur) || (EC_NULL == pnDiffAvg) || (EC_NULL == pnDiffMax))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->m_pDistributedClks->DcmGetStatus(pdwErrorCode, pnDiffCur, pnDiffAvg, pnDiffMax);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM( dwInstanceID );
    EC_UNREFPARM( pdwErrorCode);
    EC_UNREFPARM( pnDiffCur );
    EC_UNREFPARM( pnDiffAvg );
    EC_UNREFPARM( pnDiffMax );
    return EC_E_NOTSUPPORTED;
#endif
}


/***************************************************************************************************/
/**
\brief  Get DC master external synchonization status

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcxGetStatus(
      EC_T_DWORD              dwInstanceID      /**< [in]  Master Instance ID */
    , EC_T_DWORD*             pdwErrorCode      /**< [out] DCX controller error code */
    , EC_T_INT*               pnDiffCur         /**< [out] Current difference between set value and actual value of controller in nanosec. */
    , EC_T_INT*               pnDiffAvg         /**< [out] Average difference between set value and actual value of controller in nanosec.*/
    , EC_T_INT*               pnDiffMax         /**< [out] Maximum difference between set value and actual value of controller in nanosec.*/
    , EC_T_INT64*             pnTimeStampDiff   /**< [out] Difference between external and internal timestamp */
    )
{
#if (defined INCLUDE_DCX)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if ((EC_NULL == pdwErrorCode) 
        || (EC_NULL == pnDiffCur) || (EC_NULL == pnDiffAvg) || (EC_NULL == pnDiffMax) 
        || (EC_NULL == pnTimeStampDiff))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->m_pDistributedClks->DcxGetStatus(pdwErrorCode, 
                                                                                 pnDiffCur, 
                                                                                 pnDiffAvg, 
                                                                                 pnDiffMax, 
                                                                                 pnTimeStampDiff);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pdwErrorCode);
    EC_UNREFPARM(pnDiffCur);
    EC_UNREFPARM(pnDiffAvg);
    EC_UNREFPARM(pnDiffMax);
    EC_UNREFPARM(pnTimeStampDiff);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Reset DC master synchonization status

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcmResetStatus(
    EC_T_DWORD              dwInstanceID
   )
{
#if (defined INCLUDE_DC_SUPPORT )
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->m_pDistributedClks->DcmResetStatus();

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM( dwInstanceID );
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Check if DCM Bus Shift is configured/possible in configuration (ENI file)

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcmGetBusShiftConfigured(
    EC_T_DWORD              dwInstanceID
   ,EC_T_BOOL*              pbBusShiftConfigured
   )
{
#if (defined INCLUDE_DC_SUPPORT )
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (pbBusShiftConfigured == EC_NULL)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    *pbBusShiftConfigured = pEmInst->m_oAtEcatDesc.poMaster->m_pDistributedClks->DcmGetBusShiftConfigured();
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM( dwInstanceID );
    EC_UNREFPARM( pbBusShiftConfigured );
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Get current DC master synchronization logging string

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcmGetLog(
    EC_T_DWORD              dwInstanceID
   ,EC_T_CHAR**             pszLog
   )
{
#if (defined INCLUDE_DC_SUPPORT )
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pszLog)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->m_pDistributedClks->DcmGetLog(pszLog);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM( dwInstanceID );
    EC_UNREFPARM( pszLog );
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Show DC master synchonization status as DbgMsg (for development purposes only)

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcmShowStatus(
    EC_T_DWORD              dwInstanceID
   )
{
#if (defined INCLUDE_DC_SUPPORT )
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->m_pDistributedClks->DcmShowStatus();

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM( dwInstanceID );
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  returns timer adjustment when not CtlOff

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emDcmGetAdjust(
    EC_T_DWORD              dwInstanceID,
    EC_T_INT*               pnAdjustPermil
   )
{
#if (defined INCLUDE_DC_SUPPORT )
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->m_pDistributedClks->GetAdjust(pnAdjustPermil);

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM( dwInstanceID );
    EC_UNREFPARM( pnAdjustPermil );
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Get Slave Info

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetSlaveInfo(
    EC_T_DWORD              dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_T_BOOL               bFixedAddress   /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
   ,EC_T_WORD               wSlaveAddress   /**< [in]   Slave address */
   ,EC_T_GET_SLAVE_INFO*    pGetSlaveInfo   /**< [out]  Slave information */
   )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pGetSlaveInfo)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetAllSlaveInfo(bFixedAddress, wSlaveAddress, pGetSlaveInfo);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get config slave info

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetCfgSlaveInfo(
    EC_T_DWORD              dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_T_BOOL               bStationAddress /**< [in]   EC_TRUE: Use station address, EC_FALSE: use auto increment address */
   ,EC_T_WORD               wSlaveAddress   /**< [in]   Slave address */
   ,EC_T_CFG_SLAVE_INFO*    pSlaveInfo      /**< [out]  Slave information */
   )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pSlaveInfo)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetCfgSlaveInfo(bStationAddress, wSlaveAddress, pSlaveInfo);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get bus slave info

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetBusSlaveInfo(
    EC_T_DWORD              dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_T_BOOL               bStationAddress /**< [in]   EC_TRUE: Use station address, EC_FALSE: use auto increment address */
   ,EC_T_WORD               wSlaveAddress   /**< [in]   Slave address */
   ,EC_T_BUS_SLAVE_INFO*    pSlaveInfo      /**< [out]  Slave information */
   )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pSlaveInfo)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetBusSlaveInfo(bStationAddress, wSlaveAddress, pSlaveInfo);

Exit:
    return dwRetVal;
}

#if (defined INCLUDE_VARREAD)
/***************************************************************************************************/
/**
\brief  Gets the number of input variables of a specific slave. This function mainly will be used
        in connection with emGetSlaveInpVarInfo.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetSlaveInpVarInfoNumOf(
    EC_T_DWORD              dwInstanceID,                /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,               /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,               /**< [in]   Slave address */
    EC_T_WORD*              pwSlaveInpVarInfoNumOf       /**< [out]  Number of found process variable entries */
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pwSlaveInpVarInfoNumOf)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    {
    EC_T_WORD wDummy;

        dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetSlaveProcVarInfoNumOf(bFixedAddress, wSlaveAddress, pwSlaveInpVarInfoNumOf, &wDummy, &wDummy, EC_TRUE);
    }
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Gets the number of output variables of a specific slave. This function mainly will be used
        in connection with emGetSlaveOutpVarInfo.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetSlaveOutpVarInfoNumOf(
    EC_T_DWORD              dwInstanceID,              /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,             /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,             /**< [in]   Slave address */
    EC_T_WORD*              pwSlaveOutpVarInfoNumOf    /**< [out]  Number of found process variables */
     )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pwSlaveOutpVarInfoNumOf)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    {
    EC_T_WORD wDummy;

        dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetSlaveProcVarInfoNumOf(bFixedAddress, wSlaveAddress, pwSlaveOutpVarInfoNumOf, &wDummy, &wDummy, EC_FALSE);
    }
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Gets the process variable information entries of an specific slave.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetSlaveInpVarInfo(
    EC_T_DWORD              dwInstanceID,               /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,              /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,              /**< [in]   Slave address */
    EC_T_WORD               wNumOfVarsToRead,           /**< [in]   Number of process variable information entries to read */
    EC_T_PROCESS_VAR_INFO*  pSlaveProcVarInfoEntries,   /**< [out]  The read process variable information entries */
    EC_T_WORD*              pwReadEntries               /**< [out]  Number process variable entries that have been stored in pSlaveProcVarInfoEntries */

    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if ((EC_NULL == pSlaveProcVarInfoEntries) || (EC_NULL == pwReadEntries))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetSlaveProcVarInfoEntries(bFixedAddress, wSlaveAddress, pSlaveProcVarInfoEntries, EC_NULL, wNumOfVarsToRead, pwReadEntries, EC_TRUE);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Gets the process variable information entries of an specific slave. Extended version

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetSlaveInpVarInfoEx(
    EC_T_DWORD              dwInstanceID,               /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,              /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,              /**< [in]   Slave address */
    EC_T_WORD               wNumOfVarsToRead,           /**< [in]   Number of process variable information entries to read */
    EC_T_PROCESS_VAR_INFO_EX*  pSlaveProcVarInfoEntries,   /**< [out]  The read process variable information entries */
    EC_T_WORD*              pwReadEntries               /**< [out]  Number process variable entries that have been stored in pSlaveProcVarInfoEntries */

    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pSlaveProcVarInfoEntries ||  EC_NULL == pwReadEntries)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetSlaveProcVarInfoEntries(bFixedAddress, wSlaveAddress, EC_NULL, pSlaveProcVarInfoEntries, wNumOfVarsToRead, pwReadEntries, EC_TRUE);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Gets the process variable information entries of a specific slave.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetSlaveOutpVarInfo(
    EC_T_DWORD              dwInstanceID,               /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,              /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,              /**< [in]   Slave address */
    EC_T_WORD               wNumOfVarsToRead,           /**< [in]   Number of process variable information entries to read */
    EC_T_PROCESS_VAR_INFO*  pSlaveProcVarInfoEntries,   /**< [out]  The read process variable information entries */
    EC_T_WORD*              pwReadEntries               /**< [out]  Number process variable entries that have been stored in pSlaveProcVarInfoEntries */

    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pSlaveProcVarInfoEntries ||  EC_NULL == pwReadEntries)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetSlaveProcVarInfoEntries(bFixedAddress, wSlaveAddress, pSlaveProcVarInfoEntries, EC_NULL, wNumOfVarsToRead, pwReadEntries, EC_FALSE);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Gets the process variable information entries of a specific slave. Extended version

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetSlaveOutpVarInfoEx(
    EC_T_DWORD              dwInstanceID,               /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,              /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,              /**< [in]   Slave address */
    EC_T_WORD               wNumOfVarsToRead,           /**< [in]   Number of process variable information entries to read */
    EC_T_PROCESS_VAR_INFO_EX*  pSlaveProcVarInfoEntries,   /**< [out]  The read process variable information entries */
    EC_T_WORD*              pwReadEntries               /**< [out]  Number process variable entries that have been stored in pSlaveProcVarInfoEntries */

    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pSlaveProcVarInfoEntries ||  EC_NULL == pwReadEntries)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetSlaveProcVarInfoEntries(bFixedAddress, wSlaveAddress, EC_NULL, pSlaveProcVarInfoEntries, wNumOfVarsToRead, pwReadEntries, EC_FALSE);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Finds a process variable information entry by the variable name

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emFindOutpVarByName(
    EC_T_DWORD              dwInstanceID,                    /**< [in]   Master Instance ID */
    EC_T_CHAR*              szVariableName,                  /**< [in]   Variable name */
    EC_T_PROCESS_VAR_INFO*  pProcessVarInfoEntry             /**< [out]  Process variable information entry */
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pProcessVarInfoEntry ||  EC_NULL == szVariableName)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->FindProcVarByName(szVariableName, pProcessVarInfoEntry, EC_NULL, EC_FALSE);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Finds a process variable information entry by the variable name. Extended version

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emFindOutpVarByNameEx(
    EC_T_DWORD              dwInstanceID,                    /**< [in]   Master Instance ID */
    EC_T_CHAR*              szVariableName,                  /**< [in]   Variable name */
    EC_T_PROCESS_VAR_INFO_EX*  pProcessVarInfoEntry             /**< [out]  Process variable information entry */
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pProcessVarInfoEntry ||  EC_NULL == szVariableName)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->FindProcVarByName(szVariableName, EC_NULL, pProcessVarInfoEntry, EC_FALSE);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Finds a process variable information entry by the variable name

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emFindInpVarByName(
    EC_T_DWORD              dwInstanceID,                    /**< [in]   Master Instance ID */
    EC_T_CHAR*              szVariableName,                  /**< [in]   Variable name */
    EC_T_PROCESS_VAR_INFO*  pProcessVarInfoEntry             /**< [out]  Process variable information entry */
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pProcessVarInfoEntry ||  EC_NULL == szVariableName)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->FindProcVarByName(szVariableName, pProcessVarInfoEntry, EC_NULL, EC_TRUE);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Finds a process variable information entry by the variable name. Extended version.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emFindInpVarByNameEx(
    EC_T_DWORD              dwInstanceID,                    /**< [in]   Master Instance ID */
    EC_T_CHAR*              szVariableName,                  /**< [in]   Variable name */
    EC_T_PROCESS_VAR_INFO_EX*  pProcessVarInfoEntry             /**< [out]  Process variable information entry */
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if(EC_NULL == pProcessVarInfoEntry ||  EC_NULL == szVariableName)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->FindProcVarByName(szVariableName, EC_NULL, pProcessVarInfoEntry, EC_TRUE);

Exit:
    return dwRetVal;
}


#else /* (defined INCLUDE_VARREAD) */


/***************************************************************************************************/
/**
\brief  Gets the number of input variables of a specific slave. This function mainly will be used
        in connection with emGetSlaveInpVarInfo.


\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetSlaveInpVarInfoNumOf(
    EC_T_DWORD              dwInstanceID,                /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,               /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,               /**< [in]   Slave address */
    EC_T_WORD*              pwSlaveInpVarInfoNumOf       /**< [out]  Number of found process variable entries */
    )
{

    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(bFixedAddress);
    EC_UNREFPARM(wSlaveAddress);
    EC_UNREFPARM(pwSlaveInpVarInfoNumOf);

    return EC_E_NOTSUPPORTED;
}

/***************************************************************************************************/
/**
\brief  Gets the number of output variables of a specific slave. This function mainly will be
        used in connection with emGetSlaveOutpVarInfo.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetSlaveOutpVarInfoNumOf(
    EC_T_DWORD              dwInstanceID,              /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,             /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,             /**< [in]   Slave address */
    EC_T_WORD*              pwSlaveOutpVarInfoNumOf    /**< [out]  Number of found process variables */
     )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(bFixedAddress);
    EC_UNREFPARM(wSlaveAddress);
    EC_UNREFPARM(pwSlaveOutpVarInfoNumOf);
    return EC_E_NOTSUPPORTED;
}

/***************************************************************************************************/
/**
\brief  Gets the output process variable information entries of a specific slave.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetSlaveOutpVarInfo(
    EC_T_DWORD              dwInstanceID,               /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,              /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,              /**< [in]   Slave address */
    EC_T_WORD               wNumOfVarsToRead,           /**< [in]   Number process variable entries that have been stored in pSlaveProcVarInfoEntries */
    EC_T_PROCESS_VAR_INFO*  pSlaveProcVarInfoEntries,   /**< [out]  The read process variable information entries */
    EC_T_WORD*              pwReadEntries               /**< [out]  The number of read process variable information entries */
    )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(bFixedAddress);
    EC_UNREFPARM(wSlaveAddress);
    EC_UNREFPARM(wNumOfVarsToRead);
    EC_UNREFPARM(pSlaveProcVarInfoEntries);
    EC_UNREFPARM(pwReadEntries);
    return EC_E_NOTSUPPORTED;
}
ATECAT_API EC_T_DWORD emGetSlaveOutpVarInfoEx(
    EC_T_DWORD              dwInstanceID,               /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,              /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,              /**< [in]   Slave address */
    EC_T_WORD               wNumOfVarsToRead,           /**< [in]   Number process variable entries that have been stored in pSlaveProcVarInfoEntries */
    EC_T_PROCESS_VAR_INFO_EX*  pSlaveProcVarInfoEntries,   /**< [out]  The read process variable information entries */
    EC_T_WORD*              pwReadEntries               /**< [out]  The number of read process variable information entries */
    )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(bFixedAddress);
    EC_UNREFPARM(wSlaveAddress);
    EC_UNREFPARM(wNumOfVarsToRead);
    EC_UNREFPARM(pSlaveProcVarInfoEntries);
    EC_UNREFPARM(pwReadEntries);
    return EC_E_NOTSUPPORTED;
}

/***************************************************************************************************/
/**
\brief  Gets the process variable information entries of an specific slave.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emGetSlaveInpVarInfo(
    EC_T_DWORD              dwInstanceID,               /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,              /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,              /**< [in]   Slave address */
    EC_T_WORD               wNumOfVarsToRead,           /**< [in]   Number process variable entries that have been stored in pSlaveProcVarInfoEntries */
    EC_T_PROCESS_VAR_INFO*  pSlaveProcVarInfoEntries,   /**< [out]  The read process variable information entries */
    EC_T_WORD*              pwReadEntries               /**< [out]  The number of read process variable information entries */
    )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(bFixedAddress);
    EC_UNREFPARM(wSlaveAddress);
    EC_UNREFPARM(wNumOfVarsToRead);
    EC_UNREFPARM(pSlaveProcVarInfoEntries);
    EC_UNREFPARM(pwReadEntries);
    return EC_E_NOERROR;
}
ATECAT_API EC_T_DWORD emGetSlaveInpVarInfoEx(
    EC_T_DWORD              dwInstanceID,               /**< [in]   Master Instance ID */
    EC_T_BOOL               bFixedAddress,              /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
    EC_T_WORD               wSlaveAddress,              /**< [in]   Slave address */
    EC_T_WORD               wNumOfVarsToRead,           /**< [in]   Number process variable entries that have been stored in pSlaveProcVarInfoEntries */
    EC_T_PROCESS_VAR_INFO_EX*  pSlaveProcVarInfoEntries,   /**< [out]  The read process variable information entries */
    EC_T_WORD*              pwReadEntries               /**< [out]  The number of read process variable information entries */
    )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(bFixedAddress);
    EC_UNREFPARM(wSlaveAddress);
    EC_UNREFPARM(wNumOfVarsToRead);
    EC_UNREFPARM(pSlaveProcVarInfoEntries);
    EC_UNREFPARM(pwReadEntries);
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Finds a process variable information entry by the variable name

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emFindInpVarByName(
    EC_T_DWORD              dwInstanceID,                    /**< [in]   Master Instance ID */
    EC_T_CHAR*              szVariableName,                  /**< [in]   Variable name */
    EC_T_PROCESS_VAR_INFO*  pProcessVarInfoEntry             /**< [out]  Process variable information entry */
    )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(szVariableName);
    EC_UNREFPARM(pProcessVarInfoEntry);

    return EC_E_NOTSUPPORTED;
}
ATECAT_API EC_T_DWORD emFindInpVarByNameEx(
    EC_T_DWORD              dwInstanceID,                    /**< [in]   Master Instance ID */
    EC_T_CHAR*              szVariableName,                  /**< [in]   Variable name */
    EC_T_PROCESS_VAR_INFO_EX*  pProcessVarInfoEntry             /**< [out]  Process variable information entry */
    )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(szVariableName);
    EC_UNREFPARM(pProcessVarInfoEntry);

    return EC_E_NOTSUPPORTED;
}

/***************************************************************************************************/
/**
\brief  Finds a process variable information entry by the variable name

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emFindOutpVarByName(
    EC_T_DWORD              dwInstanceID,                    /**< [in]   Master Instance ID */
    EC_T_CHAR*              szVariableName,                  /**< [in]   Variable name */
    EC_T_PROCESS_VAR_INFO*  pProcessVarInfoEntry             /**< [out]  Process variable information entry */
    )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(szVariableName);
    EC_UNREFPARM(pProcessVarInfoEntry);
    return EC_E_NOTSUPPORTED;
}
ATECAT_API EC_T_DWORD emFindOutpVarByNameEx(
    EC_T_DWORD              dwInstanceID,                    /**< [in]   Master Instance ID */
    EC_T_CHAR*              szVariableName,                  /**< [in]   Variable name */
    EC_T_PROCESS_VAR_INFO_EX*  pProcessVarInfoEntry             /**< [out]  Process variable information entry */
    )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(szVariableName);
    EC_UNREFPARM(pProcessVarInfoEntry);
    return EC_E_NOTSUPPORTED;
}
#endif /* (defined INCLUDE_VARREAD) */


/***************************************************************************************************/
/**
\brief  Send a debug message to the EtherCAT link layer.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emEthDbgMsg(
                       EC_T_DWORD   dwInstanceID        /**< [in]  Master Instance ID */
                      ,EC_T_BYTE    byEthTypeByte0      /**< [in]  Ethernet type byte 0 */
                      ,EC_T_BYTE    byEthTypeByte1      /**< [in]  Ethernet type byte 0 */
                      ,EC_T_CHAR*   szMsg               /**< [in]  message to send to link layer */
                      )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (EC_NULL == szMsg)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
#ifdef INCLUDE_LOG_MESSAGES
    pEmInst->EC_DBGMSGL((2, byEthTypeByte0, byEthTypeByte1, szMsg));
#else
    EC_UNREFPARM(byEthTypeByte0);
    EC_UNREFPARM(byEthTypeByte1);
#endif

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Block invalid connected nodes from bus.

\return EC_E_NOERROR on success error code otherwise.
*/
ATECAT_API EC_T_DWORD emBlockNode(
    EC_T_DWORD              dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_T_SB_MISMATCH_DESC*  pMisMatch       /**< [in]   Mismatch descriptor from EC_NOTIFY_SB_MISMATCH */
   ,EC_T_DWORD              dwTimeout       /**< [in]   Timeout value to perform operation */
                        )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if(EC_NULL == pMisMatch)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->BlockNode(pMisMatch, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Probe open blocked ports (ecatBlockNode).

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emOpenBlockedPorts(
    EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_T_DWORD      dwTimeout       /**< [in]   Timeout value to perform operation */

                             )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (EC_NOWAIT == dwTimeout)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->OpenBlockedPorts(dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Force changed topology (trigger HC State Machine).

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emForceTopologyChange(
    EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
                                )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->ForceTopologyChange();

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Returns whether topology change detected

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emIsTopologyChangeDetected(
    EC_T_DWORD      dwInstanceID                /**< [in]   Master Instance ID */
   ,EC_T_BOOL*      pbTopologyChangeDetected    /**< [out]  Topology Change Detected */
                                )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->IsTopologyChangeDetected(pbTopologyChangeDetected);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Returns whether a specific slave is currently present on bus.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emIsSlavePresent(
    EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
   ,EC_T_DWORD      dwSlaveId       /**< [in]   Slave Id */
   ,EC_T_BOOL*      pbPresence      /**< [out]  Presence state of slave on bus */
                                )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->IsSlavePresent(dwSlaveId, pbPresence);

Exit:
    return dwRetVal;
}

#if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED)
/***************************************************************************************************/
/**
\brief  Gets the status of the Pass-through server.

\return Pass-through server status
*/
ATECAT_API EC_PTS_STATE emPassThroughSrvGetStatus(
    EC_T_DWORD dwInstanceID       /**< [in]   Master Instance ID */
    )
{
    EC_PTS_STATE    eRetVal = ePtsStateNone;
    CAtEmInterface* pEmInst = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
    eRetVal = pEmInst->PassThroughSrvGetStatus();

Exit:
    return eRetVal;
}

/***************************************************************************************************/
/**
\brief  Enables the Pass-through server.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emPassThroughSrvEnable(
    EC_T_DWORD dwInstanceID,      /**< [in]   Master Instance ID */
    EC_T_DWORD dwTimeout          /**< [in]   Timeout */
)
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->PassThroughSrvEnable(dwTimeout);

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Disables the Pass-through server.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emPassThroughSrvDisable(
    EC_T_DWORD dwInstanceID,      /**< [in]   Master Instance ID */
    EC_T_DWORD dwTimeout          /**< [in]   Timeout */

                                   )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->PassThroughSrvDisable(dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief Starts the Pass Through Server

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emPassThroughSrvStart(
    EC_T_DWORD                dwInstanceID,      /**< [in]   Master Instance ID */
    EC_T_PTS_SRV_START_PARMS* poPtsStartParams,  /**< [in]   Pass through server start parameter */
    EC_T_DWORD                dwTimeout          /**< [in]   Timeout */
                                   )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->PassThroughSrvStart(poPtsStartParams, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief Stops the Pass Through Server

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emPassThroughSrvStop(
    EC_T_DWORD dwInstanceID,    /**< [in]   Master Instance ID */
    EC_T_DWORD dwTimeout        /**< [in]   Timeout */
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->PassThroughSrvStop(dwTimeout);

Exit:
    return dwRetVal;
}

#else /* #if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED) */

/***************************************************************************************************/
/**
\brief  Gets the status of the Pass-through server.

\return Pass-through server status
*/
ATECAT_API EC_PTS_STATE  emPassThroughSrvGetStatus(
    EC_T_DWORD dwInstanceID      /**< [in]   Master Instance ID */
    )
{
    EC_UNREFPARM(dwInstanceID);
    return ePtsStateNone;
}

/***************************************************************************************************/
/**
\brief  Enables the Pass-through server.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emPassThroughSrvEnable(
    EC_T_DWORD dwInstanceID,      /**< [in]   Master Instance ID */
    EC_T_DWORD dwTimeout        /**< [in]   Stop timeout */
    )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
}

/***************************************************************************************************/
/**
\brief  Disables the Pass-through server.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emPassThroughSrvDisable(
    EC_T_DWORD dwInstanceID,    /**< [in]   Master Instance ID */
    EC_T_DWORD dwTimeout        /**< [in]   Stop timeout */
    )
{
    EC_UNREFPARM(dwTimeout);
    EC_UNREFPARM(dwInstanceID);
    return EC_E_NOTSUPPORTED;
}

/***************************************************************************************************/
/**
\brief

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emPassThroughSrvStart(
    EC_T_DWORD                dwInstanceID,      /**< [in]   Master Instance ID */
    EC_T_PTS_SRV_START_PARMS* poPtsStartParams,  /**< [in]   Pass through server start parameter */
    EC_T_DWORD                dwTimeout          /**< [in]   Timeout */
                                 )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(poPtsStartParams);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
}

/***************************************************************************************************/
/**
\brief Stops the Pass Through Server

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emPassThroughSrvStop(
    EC_T_DWORD dwInstanceID,    /**< [in]   Master Instance ID */
    EC_T_DWORD dwTimeout        /**< [in]   Timeout */
                                )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOTSUPPORTED;
}

#endif /* #if (defined INCLUDE_PASS_THROUGH_SERVER) && (defined EC_SOCKET_IP_SUPPORTED) */

#if (defined INCLUDE_ADS_ADAPTER) && (defined EC_SOCKET_IP_SUPPORTED)

/***************************************************************************************************/
/**
\brief Starts the Ads Pass Through Server

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emAdsAdapterStart(
    EC_T_DWORD                dwInstanceID,      /**< [in]   Master Instance ID */
    EC_T_ADS_ADAPTER_START_PARMS* poStartParams, /**< [in]   Pass through server start parameter */
    EC_T_DWORD                dwTimeout          /**< [in]   Timeout */
                                   )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->AdsAdapterStart(poStartParams, dwTimeout);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief Stops the Ads Pass Through Server

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emAdsAdapterStop(
    EC_T_DWORD dwInstanceID,    /**< [in]   Master Instance ID */
    EC_T_DWORD dwTimeout        /**< [in]   Timeout */
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEmInst->AdsAdapterStop(dwTimeout);

Exit:
    return dwRetVal;
}

#else /* #if (defined INCLUDE_ADS_ADAPTER) && (defined EC_SOCKET_IP_SUPPORTED) */

/***************************************************************************************************/
/**
\brief Starts the Ads Pass Through Server

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emAdsAdapterStart(
    EC_T_DWORD                dwInstanceID,      /**< [in]   Master Instance ID */
    EC_T_ADS_ADAPTER_START_PARMS* poStartParams, /**< [in]   Pass through server start parameter */
    EC_T_DWORD                dwTimeout          /**< [in]   Timeout */
                                   )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(poStartParams);
    EC_UNREFPARM(dwTimeout);

    return EC_E_NOTSUPPORTED;
}

/***************************************************************************************************/
/**
\brief Stops the Ads Pass Through Server

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emAdsAdapterStop(
    EC_T_DWORD dwInstanceID,    /**< [in]   Master Instance ID */
    EC_T_DWORD dwTimeout        /**< [in]   Timeout */
    )
{
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(dwTimeout);

    return EC_E_NOTSUPPORTED;
}
#endif /* #if (defined INCLUDE_ADS_ADAPTER) && (defined EC_SOCKET_IP_SUPPORTED) */

/***************************************************************************************************/
/**
\brief  Gets the process data input image pointer

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_BYTE* emGetProcessImageInputPtr(
    EC_T_DWORD dwInstanceID    /**< [in]   Master Instance ID */
    )
{
    EC_T_BYTE*      pRetVal  = EC_NULL;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        goto Exit;
    }

    pRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetPDInPtr();
Exit:
    return pRetVal;
}

/***************************************************************************************************/
/**
\brief  Gets the process data output image pointer

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_BYTE* emGetProcessImageOutputPtr(
    EC_T_DWORD dwInstanceID    /**< [in]   Master Instance ID */
    )
{
    EC_T_BYTE*      pRetVal  = EC_NULL;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        goto Exit;
    }

    pRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetPDOutPtr();
Exit:
    return pRetVal;
}

/***************************************************************************************************/
/**
\brief  Get diagnosis image pointer

\return The diagnosis image pointer or EC_NULL
*/
ATECAT_API EC_T_BYTE* emGetDiagnosisImagePtr(
    EC_T_DWORD dwInstanceID    /**< [in]   Master Instance ID */
    )
{
#if (defined INCLUDE_WKCSTATE)
    EC_T_BYTE*      pRetVal  = EC_NULL;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        goto Exit;
    }
    pRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetDiagnosisImagePtr();

Exit:
    return pRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    return EC_NULL;
#endif /* INCLUDE_WKCSTATE */
}

/***************************************************************************************************/
/**
\brief  Notify application with a user notification

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emNotifyApp(
     EC_T_DWORD          dwInstanceID    /**< [in]   Master Instance ID */
    ,EC_T_DWORD          dwCode          /**< [in]   Notification code */
    ,EC_T_NOTIFYPARMS*   pParms          /**< [in]   Notification data */
    )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check parameters */
    if (EC_NULL == pParms)
    {
        EC_ERRORMSGC(("emNotifyApp: EC_T_NOTIFYPARMS* is NULL!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (dwCode > EC_NOTIFY_APP_MAX_CODE)
    {
        EC_ERRORMSGC(("emNotifyApp: Notification code is out of range  %d > %d\n", dwCode, EC_NOTIFY_APP_MAX_CODE));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwCode = dwCode + EC_NOTIFY_APP;
    pEmInst->WrapperToNotifyAllClients( dwInstanceID, 0, dwCode, pParms );

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Setup callback function to log all frames

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emLogFrameEnable(
    EC_T_DWORD          dwInstanceID            /**< [in]   Master Instance ID */
   ,EC_T_PFLOGFRAME_CB  pvLogFrameCallBack      /**< [in]   pointer to function called to log frame */
   ,EC_T_VOID*          pvContext               /**< [in]   context pointer for callback function   */
                                  )
{
#if (defined INCLUDE_FRAME_LOGGING)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (EC_NULL == pvLogFrameCallBack)
    {
        EC_ERRORMSGC(("emLogFrameEnable: EC_T_PFLOGFRAME_CB is NULL!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* an other callback already set ? */
    if(pEmInst->m_oAtEcatDesc.poMaster->GetLogFrameCallback() != EC_NULL)
    {
        pEmInst->m_oAtEcatDesc.poMaster->ClrLogFrameCallback();
    }

    /* setup callback function */
    pEmInst->m_oAtEcatDesc.poMaster->SetLogFrameCallback(pvLogFrameCallBack, pvContext);

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pvLogFrameCallBack);
    EC_UNREFPARM(pvContext);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Clear callback function to log all frames

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emLogFrameDisable(
                                       EC_T_DWORD          dwInstanceID            /**< [in]   Master Instance ID */
                                       )
{
#if (defined INCLUDE_FRAME_LOGGING)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pEmInst->m_oAtEcatDesc.poMaster->ClrLogFrameCallback();

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(dwInstanceID);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Gets the source MAC address, that is sent in each EtherCAT telegram

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emGetSrcMacAddress(
                                       EC_T_DWORD          dwInstanceID,           /**< [in]   Master Instance ID */
                                       ETHERNET_ADDRESS*   pMacSrc
                                       )
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    dwRetVal = pEmInst->m_oAtEcatDesc.poMaster->GetSrcMacAddress(pMacSrc);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Set license key

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emSetLicenseKey(  EC_T_DWORD          dwInstanceID,           /**< [in]   Master Instance ID */
                                        EC_T_CHAR*          pszLicenseKey)
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    {
    CEcCRC32        oCrc32;
    ETHERNET_ADDRESS    oSrcMacAddress;
    EC_T_UINT64         qwLicenseKey;
    EC_T_DWORD          dwLkHigh = 0, dwLkLow = 0;
    EC_T_DWORD          dwLicenseCrc = 0;
    EC_T_CHAR           szKeyBuf[16];

        if(pszLicenseKey == EC_NULL)
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        /* check length of license key: "DA1099F2-15C249E9-54327FBC" */
        if(OsStrlen(pszLicenseKey) != 8 + 1 + 8 + 1 + 8)
        {
            EC_ERRORMSG(("emSetLicenseKey: Wrong license key length. Expected 26 characters !\n"));
            dwRetVal = EC_E_INVALIDSIZE;
            goto Exit;
        }

        /* get and convert first part of license key */
        memcpy(szKeyBuf, &pszLicenseKey[0], 8);
        szKeyBuf[8] = 0;
        dwLkHigh = OsStrtoul(szKeyBuf, EC_NULL, 16);

        /* get and convert second part of license key */
        memcpy(szKeyBuf, &(pszLicenseKey[8+1]), 8);
        dwLkLow = OsStrtoul(szKeyBuf, EC_NULL, 16);

        qwLicenseKey = EC_MAKEQWORD(dwLkHigh, dwLkLow);

        /* get and convert license crc */
        memcpy(szKeyBuf, &(pszLicenseKey[8+1+8+1]), 8);
        dwLicenseCrc = OsStrtoul(szKeyBuf, EC_NULL, 16);

        /* get MAC address */
        pEmInst->m_oAtEcatDesc.poMaster->GetSrcMacAddress(&oSrcMacAddress);

        /* validate license key */
        oCrc32.Init();
        oCrc32.AddData((EC_T_BYTE*)&qwLicenseKey, sizeof(qwLicenseKey));
        oCrc32.AddData((EC_T_BYTE*)&oSrcMacAddress, sizeof(oSrcMacAddress));

        if(oCrc32.GetSum() != dwLicenseCrc)
        {
            dwRetVal = EC_E_LICENSE_MISSING;
            goto Exit;
        }
        else
        {
            /* save key */
            pEmInst->m_oAtEcatDesc.poMaster->SetLicenseKey(qwLicenseKey);
        }
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

#if (defined INCLUDE_OEM)
EC_T_VOID CAtEmInterface::SetOemKey(EC_T_UINT64 qwOemKey)
{
    m_oAtEcatDesc.poMaster->SetOemKey(qwOemKey);
}
EC_T_DWORD CAtEmInterface::CheckOemKey(EC_T_UINT64 qwOemKey)
{
    return m_oAtEcatDesc.poMaster->CheckOemKey(qwOemKey);
}
#endif /* INCLUDE_OEM */

/***************************************************************************************************/
/**
\brief  Gets master properties (settings)

\return EC_E_NOERROR or error code.
*/
ATECAT_API EC_T_DWORD emGetMasterProperties(
    EC_T_DWORD              dwInstanceID,           /**< [in]   Master Instance ID */
    EC_T_DWORD*             pdwMasterPropNumEntries,
    EC_T_MASTER_PROP_DESC** ppaMasterPropEntries)
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (pdwMasterPropNumEntries != EC_NULL)
    {
        *pdwMasterPropNumEntries = pEmInst->m_oAtEcatDesc.poMaster->m_dwMasterPropNumEntries;
    }
    if (ppaMasterPropEntries != EC_NULL)
    {
        *ppaMasterPropEntries = pEmInst->m_oAtEcatDesc.poMaster->m_aMasterPropEntries;
    }

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

ATECAT_API EC_T_DWORD emGetVersion(EC_T_DWORD dwInstanceID, EC_T_DWORD *pdwVersion)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_UNREFPARM(dwInstanceID);

    if (EC_NULL == pdwVersion)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    *pdwVersion = ATECAT_VERSION;
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

ATECAT_API const EC_T_CHAR* ecatGetText(EC_T_DWORD dwTextId)
{
    return EcErrorText(dwTextId);
}

ATECAT_API const EC_T_CHAR* ecatGetNotifyText(EC_T_DWORD dwNotifyCode)
{
    return EcNotifyText(dwNotifyCode);
}

ATECAT_API EC_T_DWORD ecatConvertEcErrorToAdsError(EC_T_DWORD dwErrorCode)
{
	return EcConvertEcErrorToAdsError(dwErrorCode);
}

ATECAT_API EC_T_DWORD ecatConvertEcErrorToFoeError(EC_T_DWORD dwErrorCode)
{
	return EcConvertEcErrorToFoeError(dwErrorCode);
}

ATECAT_API EC_T_DWORD ecatConvertEcErrorToSoeError(EC_T_DWORD dwErrorCode)
{
	return EcConvertEcErrorToSoeError(dwErrorCode);
}

ATECAT_API EC_T_DWORD ecatConvertEcErrorToCoeError(EC_T_DWORD dwErrorCode)
{
	return EcConvertEcErrorToCoeError(dwErrorCode);
}

ATECAT_API EC_T_DWORD emTraceDataConfig(EC_T_DWORD dwInstanceID, EC_T_WORD wTraceDataSize)
{
#if (defined INCLUDE_TRACE_DATA)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if ((dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES) /* || (wTraceDataSize > MAX_TRACE_DATA_SIZE) */)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster)
        || pEmInst->m_oAtEcatDesc.poMaster->IsConfigured())
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if ((eCycFrameLayout_FIXED == pEmInst->m_oAtEcatDesc.poMaster->GetCycFrameLayout()) && (wTraceDataSize > 0))
    {
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }
    pEmInst->m_oAtEcatDesc.poMaster->m_wTraceDataSize = wTraceDataSize;

    /* no errors */
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
#else /* !INCLUDE_TRACE_DATA */
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(wTraceDataSize);
    return EC_E_NOTSUPPORTED;
#endif /* !INCLUDE_TRACE_DATA */
}

ATECAT_API EC_T_DWORD emTraceDataGetInfo(EC_T_DWORD dwInstanceID, EC_T_TRACE_DATA_INFO* pTraceDataInfo)
{
#if (defined INCLUDE_TRACE_DATA)
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    CAtEmInterface* pEmInst  = EC_NULL;

    /* check parameters */
    if ((dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES) || (EC_NULL == pTraceDataInfo))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get master instance */
    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    if (EC_NULL == pEmInst)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check master */
    if (!pEmInst->m_oAtEcatDesc.bInitialized || (EC_NULL == pEmInst->m_oAtEcatDesc.poMaster)
        || !pEmInst->m_oAtEcatDesc.poMaster->IsConfigured())
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    pTraceDataInfo->pbyData = pEmInst->m_oAtEcatDesc.poMaster->GetPDOutPtr();
    pTraceDataInfo->dwOffset = pEmInst->m_oAtEcatDesc.poMaster->m_dwTraceDataOffset;
    pTraceDataInfo->wSize = pEmInst->m_oAtEcatDesc.poMaster->m_wTraceDataSize;

    /* no errors */
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
#else /* !INCLUDE_TRACE_DATA */
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pTraceDataInfo);
    return EC_E_NOTSUPPORTED;
#endif /* !INCLUDE_TRACE_DATA */
}

ATECAT_API EC_T_DWORD emFastModeInitNotSupported(EC_T_DWORD dwInstanceID)
{
#if (defined INCLUDE_FAST_MODE)
    /* TODO */
#else
    EC_UNREFPARM(dwInstanceID);
    return EC_E_NOTSUPPORTED;
#endif
}

ATECAT_API EC_T_DWORD emFastSendAllCycFramesNotSupported(EC_T_DWORD dwInstanceID)
{
#if (defined INCLUDE_FAST_MODE)
    /* TODO */
#else
    EC_UNREFPARM(dwInstanceID);
    return EC_E_NOTSUPPORTED;
#endif
}

ATECAT_API EC_T_DWORD emFastProcessAllRxFramesNotSupported(EC_T_DWORD dwInstanceID, EC_T_BOOL* pbAreAllCycFramesProcessed)
{
#if (defined INCLUDE_FAST_MODE)
    /* TODO */
#else
    EC_UNREFPARM(dwInstanceID);
    EC_UNREFPARM(pbAreAllCycFramesProcessed);
    return EC_E_NOTSUPPORTED;
#endif
}

ATECAT_API EC_T_DWORD emGetMemoryUsage(EC_T_DWORD dwInstanceID,
                                       EC_T_DWORD* pdwCurrentUsage,
                                       EC_T_DWORD* pdwMaxUsage)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    struct _EC_T_MEMORY_LOGGER* pMLog = EC_NULL;
    CAtEmInterface* pEmInst  = EC_NULL;

    if (dwInstanceID >= MAX_NUMOF_MASTER_INSTANCES || EC_NULL == pdwCurrentUsage || EC_NULL == pdwMaxUsage)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pEmInst = S_aMultiMaster[dwInstanceID].pInst;
    OsDbgAssert(pEmInst != EC_NULL);
    if (EC_NULL == pEmInst)
    {
        return EC_E_INVALIDSTATE;
    }

    pMLog = pEmInst->GetMemLog();
    *pdwCurrentUsage = (EC_T_DWORD)pMLog->nCurrUsage;
    *pdwMaxUsage = (EC_T_DWORD)pMLog->nMaxUsage;

    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/* GetInitMasterParms() for EC-Master pre V3.0 */
struct _EC_T_INIT_MASTER_PARMS* CEcMaster::GetInitMasterParms(EC_T_VOID)
{
    return S_aMultiMaster[m_MasterConfigEx.dwInstanceID].pInst->m_oAtEcatDesc.pInitMasterParms;
}

/*-END OF SOURCE FILE--------------------------------------------------------*/
