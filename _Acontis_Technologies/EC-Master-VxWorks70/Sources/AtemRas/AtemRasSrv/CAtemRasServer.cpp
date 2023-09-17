/*-----------------------------------------------------------------------------
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Bussmann, Paul
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "CAtemRasServer.h"
#include <AtEmRasError.h>
#include <AtemRasRemoteProtocol.h>
#include "AtEmMarshal.h"

/*-DEFINES-------------------------------------------------------------------*/
#define SETM_OCONSTATE_LOCK(eVal)   \
{if(m_oConState!=eVal){\
    VERBOSEOUT(2,("SETM_OCONSTATE(%s)\n",ATEMRAS_T_CONNECTIONSTATE_TEXT(eVal)));m_pServer->LockList();m_oConState=eVal;m_pServer->UnlockList();}}

#define SETM_OCONSTATE_NOLOCK(eVal)   \
{if(m_oConState!=eVal){VERBOSEOUT(2,("SETM_OCONSTATE(%s)\n",ATEMRAS_T_CONNECTIONSTATE_TEXT(eVal)))};m_oConState=eVal;}

#define SETM_OCONSTATE(eVal,bLock) {if(bLock){SETM_OCONSTATE_LOCK(eVal);}else{SETM_OCONSTATE_NOLOCK(eVal);}}

/*-FUNCTION-DEFINITIONS------------------------------------------------------*/
const char* ATEMRAS_T_CONNECTIONSTATE_TEXT(ATEMRAS_T_CONNECTIONSTATE eVal)
{
    switch (eVal)
    {
    case emrasconstate_unknown:    return "UNKNOWN";
    case emrasconstate_init:       return "Init";
    case emrasconstate_idle:       return "Idle";
    case emrasconstate_cmdproc:    return "Cmd Proc";
    case emrasconstate_wdidle:     return "WD Idle";
    case emrasconstate_wdexpired:  return "WD Expired";
    case emrasconstate_disconnect: return "Disconnect";
    default:                       return "Undefined state!";
    }
}

EC_T_VOID CAtemRasSrvClient::RxThreadStepWrapper(EC_T_PVOID pvParams)
{
    CAtemRasSrvClient* pClient = (CAtemRasSrvClient*)pvParams;

    pClient->RxThreadStep();
}

/*-CAtemRasServer------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Constructor create a server instance.
*/
CAtemRasServer::CAtemRasServer(
    ATEMRAS_T_SRVPARMS oParms   /**< [in]   Configuration parameter structure */
                              )
{
    m_pServerAcceptorSocket = EC_NULL;
    m_pClientList           = EC_NULL;
    m_dwMastStackSize       = MASTER_STACK_SIZE_DEFAULT;
    m_dwClientStackSize     = CLIENT_STACK_SIZE_DEFAULT;
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    m_pAccessControl        = EC_NULL;
#endif

    OsMemcpy(&m_oConfig, &oParms, sizeof(ATEMRAS_T_SRVPARMS));

#if (defined EC_SOCKET_IP_SUPPORTED)
    CEcSocketIp::InitSocketLayer();
#endif /* EC_SOCKET_IP_SUPPORTED */
}

/***************************************************************************************************/
/**
\brief  Destructor
*/
CAtemRasServer::~CAtemRasServer()
{
    StopServer(SERVER_SHUTDOWNDELAY_DEFAULT);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasServer::~CAtemRasServer m_pAccessControl", m_pAccessControl, sizeof(CAtemRasAccessControl));
    SafeDelete(m_pAccessControl);
#endif

#if (defined EC_SOCKET_IP_SUPPORTED)
    CEcSocketIp::DeInitSocketLayer();
#endif /* EC_SOCKET_IP_SUPPORTED */
}

/***************************************************************************************************/
/**
\brief  Starts the server thread and sets the port to accept connections.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasServer::StartServer()
{
    EC_T_IPADDR oIpAddr     = {0};
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    EC_T_DWORD  dwRes       = EC_E_ERROR;

    if (EC_NULL != m_pServerAcceptorSocket)
    {
        VERBOSEOUT(1, ("Already running!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRes = CreateAcceptorSocket();
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    oIpAddr.dwAddr = m_oConfig.oAddr.dwAddr;

    /* bind socket */
    dwRes = m_pServerAcceptorSocket->Bind(&oIpAddr, (m_oConfig.wPort));
    if (EC_E_NOERROR != dwRes)
    {
        VERBOSEOUT(1, ("Bind for ServerAcceptorSocket failed\n"));
        dwRetVal = dwRes;
        goto Exit;
    }

    /* listen socket */
    dwRes = m_pServerAcceptorSocket->Listen();
    if (EC_E_NOERROR != dwRes)
    {
        VERBOSEOUT(1, ("Listen for ServerAcceptorSocket failed! %s (0x%lx)\n", ecatGetText(dwRes), dwRes));
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRes = m_AcceptorThread.Start(CAtemRasServer::AcceptorThreadStepWrapper, this, "RAS Server Acceptor Thread",
        m_oConfig.dwMasterPrio, m_dwMastStackSize, SERVER_STARTUPDELAY_DEFAULT);
    if (EC_E_NOERROR != dwRes)
    {
        VERBOSEOUT(1, ("Start ServerAcceptor thread: %s (0x%lx)!\n", ecatGetText(dwRes), dwRes));
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;

Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Stop all opened threads.

This function shuts down all connections, stops all client threads and the master thread.
\return EC_E_NOERROR on success, error code otherwise
*/
EC_T_DWORD CAtemRasServer::StopServer(
    EC_T_DWORD dwTimeout        /**< [in]   Timeout to wait for graceful Thread shut down */
                                     )
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;
    CAtemClientListEntry*   pTmp        = EC_NULL;

    /* deny new connections */
    dwRetVal = m_AcceptorThread.Stop(dwTimeout);
    if (EC_NULL != m_pServerAcceptorSocket)
    {
        m_pServerAcceptorSocket->Disconnect(EC_FALSE);
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasServer::~CAtemRasServer m_pServerAcceptorSocket", m_pServerAcceptorSocket,0);
        SafeDelete(m_pServerAcceptorSocket);
    }

    if (EC_E_NOERROR != m_oListLock.Lock(SERVER_SHUTDOWNDELAY_DEFAULT, EC_FALSE))
    {
        VERBOSEOUT(1, ("Shared client list lock failed\n"));
        OsDbgAssert(EC_FALSE);
    }

    pTmp = m_pClientList;
    while (EC_NULL != pTmp)
    {
        pTmp->GetClient()->Terminate();
        pTmp = pTmp->Next();
    }

    if (EC_E_NOERROR != m_oListLock.ChangeLock(SERVER_SHUTDOWNDELAY_DEFAULT, EC_TRUE))
    {
        VERBOSEOUT(1, ("Remove client list without lock\n"));
        OsDbgAssert(EC_FALSE);
    }

    while (EC_NULL != m_pClientList)
    {
        pTmp = m_pClientList;
        m_pClientList = m_pClientList->Next();

        if (!pTmp->GetClient()->IsDisconnect())
        {
            pTmp->GetClient()->Disconnect(EMRAS_EVT_SERVERSTOPPED);
        }
        pTmp->GetClient()->Terminate();

        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasServer::StopServer pTmp", pTmp, sizeof(CAtemClientListEntry));
        SafeDelete(pTmp);
    }

    m_oListLock.UnLock();

    return dwRetVal;
}

EC_T_DWORD CAtemRasServer::CreateAcceptorSocket(EC_T_VOID)
{
    EC_T_DWORD    dwRetVal  = EC_E_ERROR;
    EC_T_SOCKTYPE eSockType = ATEMRAS_GET_DEFAULT_SOCKTYPE(m_oConfig.wPort);

    switch (eSockType)
    {
#if (defined EC_SOCKET_IP_SUPPORTED)
    case emrassocktype_tcp:
        m_pServerAcceptorSocket = EC_NEW(CEcSocketIp(eSockType));
        break;
#endif /* EC_SOCKET_IP_SUPPORTED */
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    case emrassocktype_msg:
        EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasServer::CAtemRasServer m_pServerAcceptorSocket", m_pServerAcceptorSocket, sizeof(CEcSocketMsgQueue));
        m_pServerAcceptorSocket = EC_NEW(CEcSocketMsgQueue());
        break;
#endif /* EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED */
#if defined(EC_SOCKET_SHM_SUPPORTED)
    case emrassocktype_shm:
        EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasServer::CAtemRasServer m_pServerAcceptorSocket", m_pServerAcceptorSocket, sizeof(CEcSocketShm));
        m_pServerAcceptorSocket = EC_NEW(CEcSocketShm());
        break;
#endif /* EC_SOCKET_SHM_SUPPORTED */
#if defined(EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED)
    case emrassocktype_rtosshm:
        EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasServer::CAtemRasServer m_pServerAcceptorSocket", m_pServerAcceptorSocket, sizeof(CEcSocketRtosShm));
        m_pServerAcceptorSocket = EC_NEW(CEcSocketRtosShm());
        break;
#endif /* EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED */
#if defined(EC_SOCKET_RTOSLIB_SUPPORTED)
    case emrassocktype_rtoslib:
        EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasServer::CAtemRasServer m_pServerAcceptorSocket", m_pServerAcceptorSocket, sizeof(CEcSocketRtosLib));
        m_pServerAcceptorSocket = EC_NEW(CEcSocketRtosLib());
        break;
#endif /* EC_SOCKET_RTOSLIB_SUPPORTED */
    default:
        /* this may not happen */
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
    }
    if (EC_NULL == m_pServerAcceptorSocket)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Add Client Instance to list.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasServer::AppendClientEntry(
    CAtemRasSrvClient* pSlave    /**< [in]   Slave Instance to enqueue */
                                           )
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;
    CAtemClientListEntry*    pTmp        = EC_NULL;
    EC_T_BOOL               bListLock   = EC_FALSE;

    if (EC_NULL == pSlave)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    LockList();
    bListLock = EC_TRUE;

    if (EC_NULL == m_pClientList)
    {
        m_pClientList = EC_NEW(CAtemClientListEntry(pSlave));
        EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasServer::AppendClientEntry m_pClientList", m_pClientList, sizeof(CAtemClientListEntry));
    }
    else
    {
        pTmp = m_pClientList;
        while(EC_NULL!=pTmp->Next())
        {
            pTmp = pTmp->Next();
        }

        pTmp->Next(EC_NEW(CAtemClientListEntry(pSlave)));
        if (EC_NULL == pTmp->Next())
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        pTmp->Next()->Prev(pTmp);
    }

    bListLock = EC_FALSE;
    UnlockList();

Exit:
    if( bListLock )
    {
        UnlockList();
    }

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Garbage Collector.

Remove disconnected slaves from list
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasServer::GarbageCollect()
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;
    CAtemClientListEntry*    pCurEntry   = EC_NULL;
    CAtemClientListEntry*    pTmpEntry   = EC_NULL;

    LockList();

    pCurEntry = m_pClientList;

    while (EC_NULL != pCurEntry)
    {
        if (pCurEntry->GetClient()->IsDisconnect())
        {
            pCurEntry->GetClient()->Terminate();
        }
        if (pCurEntry->GetClient()->IsTerminated())
        {
            pTmpEntry = pCurEntry;

            if( pCurEntry == m_pClientList )
            {
                m_pClientList   = pTmpEntry->Next();
                pCurEntry       = m_pClientList;
            }
            else
            {
                pCurEntry       = pTmpEntry->Next();
            }

            if (EC_NULL != pTmpEntry->Prev())
            {
                pTmpEntry->Prev()->Next(pTmpEntry->Next());
            }
            if (EC_NULL != pTmpEntry->Next())
            {
                pTmpEntry->Next()->Prev(pTmpEntry->Prev());
            }

            EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasServer::GarbageCollect pTmpEntry", pTmpEntry, sizeof(CAtemClientListEntry));
            SafeDelete(pTmpEntry);
        }
        else
        {
            pCurEntry = pCurEntry->Next();
        }
    }

    UnlockList();

    dwRetVal = EC_E_NOERROR;

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Find client entry by given cookie.

\return Pointer to found client on success, EC_NULL otherwise.
*/
CAtemRasSrvClient* CAtemRasServer::FindClientByCookie(
    EC_T_DWORD dwCookie     /**< [in]   Cookie to seek for */
                                                   )
{
    CAtemClientListEntry* pTmpEntry    = EC_NULL;
    CAtemRasSrvClient*    pFoundClient = EC_NULL;

    LockList();

    pTmpEntry = m_pClientList;

    while (EC_NULL != pTmpEntry)
    {
        if (dwCookie == (pTmpEntry->GetClient()->Cookie()))
        {
            pFoundClient = pTmpEntry->GetClient();
            break;
        }

        pTmpEntry = pTmpEntry->Next();
    }
    UnlockList();

    return pFoundClient;
}

/***************************************************************************************************/
/**
\brief  Notify AtemRas Layer Startup application.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasServer::NotifyCallee(
    EC_T_DWORD dwCode,          /**< [in]   Notification code */
    EC_T_DWORD dwInSize,        /**< [in]   Size of In Data */
    EC_T_PBYTE pbyInData        /**< [in]   In Data */
                                      )
{
            EC_T_DWORD          dwRetVal = EC_E_ERROR;
    static  EC_T_NOTIFYPARMS    oNotify;

    if (EC_NULL == m_oConfig.pfNotification)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    OsMemset(&oNotify, 0, sizeof(EC_T_NOTIFYPARMS));

    oNotify.dwInBufSize     = dwInSize;
    oNotify.pbyInBuf        = pbyInData;
    oNotify.dwOutBufSize    = 0;
    oNotify.pbyOutBuf       = EC_NULL;

    oNotify.pCallerData = m_oConfig.pvNotifCtxt;

    m_oConfig.pfNotification(dwCode, &oNotify);

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

#if (defined INCLUDE_RAS_SPOCSUPPORT)
/***************************************************************************************************/
/**
\brief  Configure Access Control.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasServer::ConfigureAccessControl(
    EC_T_BYTE*  pbyData     /**< [in]   Configuration data */
   ,EC_T_DWORD  dwLen       /**< [in]   Size of configuration data */
    )
{
    if (EC_NULL != m_pAccessControl)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasServer::ConfigureAccessControl m_pAccessControl", m_pAccessControl,sizeof(CAtemRasAccessControl));
        SafeDelete(m_pAccessControl);
    }

    m_pAccessControl = EC_NEW(CAtemRasAccessControl(pbyData, dwLen));
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasServer::ConfigureAccessControl m_pAccessControl", m_pAccessControl, sizeof(CAtemRasAccessControl));
    if (EC_NULL == m_pAccessControl)
    {
        return EC_E_NOMEMORY;
    }

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Change currently active access level for a call class.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasServer::SetAccessLevel(
    EC_T_DWORD  dwClass     /**< [in]   Call class to modify, 0 = all classes */
   ,EC_T_DWORD  dwLevel     /**< [in]   New Access level to set */
   )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    if (EC_NULL == m_pAccessControl)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRetVal = m_pAccessControl->ModifyClassState(dwClass, dwLevel);
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get Access Level for a specific Call.

\return EC_E_NOERROR on access, error code otherwise.
*/
EC_T_DWORD CAtemRasServer::GetAccessLevelByCall(
     EC_T_DWORD  dwCallOrdinal               /**< [in]   Call Ordinal (mandatory) */
    ,EC_T_DWORD  dwCallIndex /* = 0 */       /**< [in]   Call Index (optional) */
    ,EC_T_DWORD  dwCallSubIndex /* = 0 */    /**< [in]   Call SubIndex (optional) */
    ,EC_T_BYTE*  pbyCSE /* = EC_NULL */      /**< [in]   Call Specific enhancement (optional) */
    )
{
    if (EC_NULL == m_pAccessControl)
    {
        return EC_E_NOERROR;
    }
    return m_pAccessControl->CallAccess(dwCallOrdinal, dwCallIndex, dwCallSubIndex, pbyCSE);
    }
#endif

/*-CAtemRasSrvClient----------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Client class constructor.
*/
CAtemRasSrvClient::CAtemRasSrvClient(
    CAtemRasServer* pServer,            /**< [in]   Server instance */
    CEcSocket*      pSocket,            /**< [in]   Associated socket */
    EC_T_DWORD      dwCookie,           /**< [in]   Generated Cookie */
    EC_T_DWORD      dwSilenceLimit,     /**< [in]   Limit of silence before leaving idle state, multiplied by
                                          *         cycle time*/
    EC_T_DWORD      dwCycleTime,        /**< [in]   Timespan of single cycle */
    EC_T_DWORD      dwThreadPrio,
    EC_T_DWORD      dwThreadStackSize
                                  )
: m_pServer(pServer),
  m_pSocket(pSocket),
  m_dwCookie(dwCookie),
  m_dwCycleTime(dwCycleTime),
  m_dwThreadPrio(dwThreadPrio),
  m_dwThreadStackSize(dwThreadStackSize),
  m_dwWDReload(dwSilenceLimit*dwCycleTime)
{
    m_bEntryValid = EC_FALSE;

    m_dwRxBufSize = 0;
    m_pRxBuffer = EC_NULL;

    m_pNotifyStore = EC_NULL;
    m_pWorkerCmdFifo = EC_NULL;
    m_pWorkerFreeCmdFifo = EC_NULL;

    m_oWatchdog.Start(m_dwWDReload);

    m_oConState = emrasconstate_unknown;
    SETM_OCONSTATE_NOLOCK(emrasconstate_unknown);

    m_bSockDisposable   = EC_TRUE;
    m_pCmdQueue         = EC_NULL;
    m_pMasterStore      = EC_NULL;
    m_dwSeqNr           = 0;
    m_dwClientVersion   = 0;
    m_dwProtocolVersion = (EC_T_DWORD)ATEMRASSRV_VERSION;
}


CAtemRasSrvClient::~CAtemRasSrvClient()
{
    ATEMRAS_T_BUFFER_DESC* pBufDesc = EC_NULL;
    EC_T_BOOL bSocketLocked = EC_FALSE;

    Terminate();

    if (m_bSockDisposable)
    {
        if (EC_E_NOERROR != m_oSockLock.Lock(SERVER_SHUTDOWNDELAY_DEFAULT, EC_TRUE))
        {
            VERBOSEOUT(1, ("Delete socket without lock!\n"));
            OsDbgAssert(EC_FALSE);
        }
        else
        {
            bSocketLocked = EC_TRUE;
        }

        if (EC_NULL != m_pSocket)
        {
            m_pSocket->Disconnect(EC_FALSE);

            /* CAtemRasServer::m_oListLock already taken */
            SETM_OCONSTATE_NOLOCK(emrasconstate_disconnect);

            EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasSrvClient::~CAtemRasSrvClient m_pSocket", m_pSocket,0);
            SafeDelete(m_pSocket);
        }

        if (bSocketLocked)
        {
            m_oSockLock.UnLock();
        }
    }

    SafeDelete(m_pMasterStore);

    SafeDelete(m_pCmdQueue);

    if (EC_NULL != m_pWorkerCmdFifo)
    {
        while (m_pWorkerCmdFifo->RemoveNoLock(pBufDesc))
        {
            SafeOsFree(pBufDesc->pbyData);
            SafeOsFree(pBufDesc);
        }
    }
    SafeDelete(m_pWorkerCmdFifo);

    if (EC_NULL != m_pWorkerFreeCmdFifo)
    {
        while (m_pWorkerFreeCmdFifo->RemoveNoLock(pBufDesc))
        {
            SafeOsFree(pBufDesc->pbyData);
            SafeOsFree(pBufDesc);
        }
    }
    SafeDelete(m_pWorkerFreeCmdFifo);

    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasSrvClient::ClientWorker m_pRxBuffer", m_pRxBuffer, m_dwRxBufSize);
    SafeOsFree(m_pRxBuffer);
}

/***************************************************************************************************/
/**
\brief  Initialize pointer elements.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::InitClient(
    EC_T_VOID
                                      )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    EC_T_DWORD  dwRes       = EC_E_ERROR;

    m_pCmdQueue = EC_NEW(CAtEmRasCmdQueue());
    if (EC_NULL == m_pCmdQueue)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    m_pMasterStore = EC_NEW(CAtEmMasterStore());
    if (EC_NULL == m_pMasterStore)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    m_dwRxBufSize = DEFAULT_RX_BUFF_SIZE;
    m_pRxBuffer = (EC_T_PBYTE)OsMalloc(m_dwRxBufSize);
    if (EC_NULL == m_pRxBuffer)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(m_pRxBuffer, 0, m_dwRxBufSize);

#ifdef DEBUGTRACE
    m_pWorkerCmdFifo = EC_NEW(CFiFoListDyn<ATEMRAS_T_BUFFER_DESC*>(MAX_NUMOF_CMDQUEUE_ELEMENTS, EC_NULL, EC_NULL, EC_MEMTRACE_RAS));
#else
    m_pWorkerCmdFifo = EC_NEW(CFiFoListDyn<ATEMRAS_T_BUFFER_DESC*>(MAX_NUMOF_CMDQUEUE_ELEMENTS, EC_NULL, EC_NULL));
#endif
    if (EC_NULL == m_pWorkerCmdFifo)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

#ifdef DEBUGTRACE
    m_pWorkerFreeCmdFifo = EC_NEW(CFiFoListDyn<ATEMRAS_T_BUFFER_DESC*>(MAX_NUMOF_CMDQUEUE_ELEMENTS, EC_NULL, EC_NULL, EC_MEMTRACE_RAS));
#else
    m_pWorkerFreeCmdFifo = EC_NEW(CFiFoListDyn<ATEMRAS_T_BUFFER_DESC*>(MAX_NUMOF_CMDQUEUE_ELEMENTS, EC_NULL, EC_NULL));
#endif
    if (EC_NULL == m_pWorkerFreeCmdFifo)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRes = WaitForLogon();
    /* if logon fails with error, dispose instance */
    if /* ((EMRAS_EVT_RECONEXPIRED != dwRes) && */ (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        VERBOSEOUT(1, ("Logon expired or failed with error %s (0x%lx)\n", emRasErrorText(dwRes), dwRes));
        goto Exit;
    }

    /* this instance is a newly spawned logon, continue */
    VERBOSEOUT(5, ("Client logged on. Protocol version 0x%08x\n", m_dwProtocolVersion));
    if ((m_dwClientVersion & 0xffff) != 0)
    {
        VERBOSEOUT(5, ("Client version 0x%08x\n", m_dwClientVersion));
    }

    dwRes = m_RxThread.Start(RxThreadStepWrapper, this, (EC_T_CHAR*)"AtemRasServerRxThread", m_dwThreadPrio, m_dwThreadStackSize, SERVER_STARTUPDELAY_DEFAULT);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        VERBOSEOUT(1, ("m_RxThread.Start(...): %s (0x%lx)!\n", emRasErrorText(dwRes), dwRes));
        goto Exit;
    }

    dwRes = m_WorkerThread.Start(WorkerThreadStepWrapper, this, (EC_T_CHAR*)"AtemRasServerWorkerThread", m_dwThreadPrio, m_dwThreadStackSize, SERVER_STARTUPDELAY_DEFAULT);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        VERBOSEOUT(1, ("m_WorkerThread.Start(...): %s (0x%lx)!\n", emRasErrorText(dwRes), dwRes));
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        m_WorkerThread.Stop(SERVER_SHUTDOWNDELAY_DEFAULT);
        m_RxThread.Stop(SERVER_SHUTDOWNDELAY_DEFAULT);

        SafeDelete(m_pCmdQueue);
        SafeDelete(m_pMasterStore);
        SafeDelete(m_pRxBuffer);
        SafeDelete(m_pWorkerCmdFifo);
        SafeDelete(m_pWorkerFreeCmdFifo);

        SETM_OCONSTATE_LOCK(emrasconstate_terminated);
    }

    return dwRetVal;
}

EC_T_DWORD CAtemRasSrvClient::Disconnect(EC_T_DWORD dwCause, EC_T_BOOL bLock)
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;
    EC_T_DWORD  dwWrtLen = 0;
    EC_T_BYTE   abyTxBuffer[ATEMRAS_T_PROTOHDR_SIZE + sizeof(EC_T_DWORD)];
    ATEMRAS_T_CONNOTIFYDESC oConNotDesc;

    /* mark client entry as obsolete */
    SETM_OCONSTATE(emrasconstate_disconnect, bLock);
    m_oWatchdog.Stop();

    if (!IsSocketDisconnect())
    {
        emrasProtoCreateDisconnectNoAlloc(m_dwCookie, dwCause, abyTxBuffer, &dwWrtLen);
        SendData(abyTxBuffer, dwWrtLen);
        m_pSocket->Disconnect(EC_FALSE);
    }
    m_pMasterStore->RemoveAllRegistrations();

    /* notify application */
    oConNotDesc.dwCause     = dwCause;
    oConNotDesc.dwCookie    = m_dwCookie;
    m_pServer->NotifyCallee(ATEMRAS_NOTIFY_CONNECTION, sizeof(ATEMRAS_T_CONNOTIFYDESC), (EC_T_PBYTE)&oConNotDesc);
    return dwRetVal;
}

EC_T_DWORD CAtemRasSrvClient::Terminate(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_DWORD dwRes = EC_E_NOERROR;

    if (emrasconstate_disconnect == m_oConState)
    {
        dwRes = m_RxThread.Stop(SERVER_SHUTDOWNDELAY_DEFAULT);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
        }
        dwRes = m_WorkerThread.Stop(SERVER_SHUTDOWNDELAY_DEFAULT);
        if ((EC_E_NOERROR != dwRes) && (EC_E_NOERROR == dwRetVal))
        {
            dwRetVal = dwRes;
        }

        SETM_OCONSTATE_NOLOCK(emrasconstate_terminated);
    }

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get And Increment Sequence Nr.

\return Next Sequence Number.
*/
EC_T_DWORD CAtemRasSrvClient::GetAndIncSeqNr(EC_T_VOID)
{
    return m_dwSeqNr++;
}

/***************************************************************************************************/
/**
\brief  Idle on open socket until Logged in or not.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::WaitForLogon()
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;
    EC_T_BOOL               bLogOnPend  = EC_TRUE;
    EC_T_DWORD              dwRes       = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR      oRxHdr;
    EC_T_BYTE               abyData[ATEMRAS_T_CONNECT_OEM_TRANSFER_SIZE];
    EC_T_DWORD              dwReadLen   = 0;
    EC_T_BOOL               bSocketLocked    = EC_FALSE;
    ATEMRAS_T_CONNOTIFYDESC oConNotDesc;

    if (EC_NULL == m_pSocket)
    {
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }

    while (bLogOnPend)
    {
        /* Receive next Protocol Header from Stack */
        OsMemset(&oRxHdr, 0, ATEMRAS_T_PROTOHDR_SIZE);
        dwReadLen = ATEMRAS_T_PROTOHDR_SIZE;

        OsDbgAssert(!bSocketLocked);
        m_oSockLock.Lock();
        bSocketLocked = EC_TRUE;

        if (EC_NULL == m_pSocket)
        {
            bLogOnPend = EC_FALSE;
            dwRetVal = EMRAS_E_LOGONCANCELLED;
            Disconnect(dwRetVal);
            goto Exit;
        }

        /* The client has to sent its first request */
        dwRes = m_pSocket->RecvData(oRxHdr.byData, &dwReadLen, RAS_CMD_RECV_TIMEOUT * 100 /* TODO: reduce */);
        if ((EC_E_INVALIDSTATE == dwRes) || (EC_E_SOCKET_DISCONNECTED == dwRes))
        {
            /* connection dropped */
            m_pSocket->Disconnect(EC_FALSE);
            dwRetVal = EMRAS_E_LOGONCANCELLED;
            Disconnect(dwRetVal);
            goto Exit;
        }
        else if ((EC_E_NOERROR != dwRes) || (dwReadLen < ATEMRAS_T_PROTOHDR_SIZE))
        {
            bLogOnPend  = EC_TRUE;

            SETM_OCONSTATE_LOCK(emrasconstate_init);
        }
        else
        {
            /* we did rx the correct size */
            EC_T_BYTE byStatus = ATEMRAS_PROTOHDR_GET_STATUS_REQ(&oRxHdr);
            EC_T_BYTE byCmd = ATEMRAS_PROTOHDR_GET_CMD(&oRxHdr);

            if (byStatus && (
                (emrascmd_idle == byCmd)
             || (emrascmd_connect == byCmd)
             || (emrascmd_connect_oem == byCmd)
             || (emrascmd_disconnect == byCmd)
               )
              )
            {
                /* read data, e.g. protocol version from command */
                dwReadLen = ATEMRAS_PROTOHDR_GET_LENGTH(&oRxHdr);

                /* check data size. see emrasProtoCreate...() */
                switch (byCmd)
                {
                case emrascmd_connect:
                case emrascmd_disconnect:
                    if (sizeof(EC_T_DWORD) != dwReadLen)
                    {
                        dwRetVal = EMRAS_E_INVALIDDATARECEIVED;
                        goto Exit;
                    } break;
                case emrascmd_connect_oem:
                    if (ATEMRAS_T_CONNECT_OEM_TRANSFER_SIZE != dwReadLen)
                    {
                        dwRetVal = EMRAS_E_INVALIDDATARECEIVED;
                        goto Exit;
                    } break;
                case emrascmd_idle:
                    if (0 != dwReadLen)
                    {
                        dwRetVal = EMRAS_E_INVALIDDATARECEIVED;
                        goto Exit;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        dwRetVal = EMRAS_E_INVALIDDATARECEIVED;
                        goto Exit;
                    } break;
                }

                if (dwReadLen > 0)
                {
                    OsDbgAssert(dwReadLen <= ATEMRAS_T_CONNECT_OEM_TRANSFER_SIZE);

                    dwRes = m_pSocket->RecvData(
                        abyData,
                        &dwReadLen,
                        RAS_CMD_RECV_TIMEOUT);

                    switch (dwRes)
                    {
                    case EC_E_NOERROR:
                        break;
                    case EC_E_SOCKET_DISCONNECTED:
                    {
                        /* socket dropped, cancel logon */
                        m_pSocket->Disconnect(EC_FALSE);
                        bLogOnPend  = EC_FALSE;

                        SETM_OCONSTATE_LOCK(emrasconstate_disconnect);

                        dwRetVal    = EMRAS_E_LOGONCANCELLED;
                        goto Exit;
                    } break;
                    default:
                        dwRetVal = dwRes;
                        goto Exit;
                    }
                }

                VERBOSEOUT(2, ("CAtemRasSrvClient.WaitForLogon(...) Got %s CMD.\n",
                    ATEMRAS_T_CMDTYPE_TEXT((ATEMRAS_T_CMDTYPE) ATEMRAS_PROTOHDR_GET_CMD(&oRxHdr))));

                /* it is a valid request, process it */
                switch (ATEMRAS_PROTOHDR_GET_CMD(&oRxHdr))
                {
                case emrascmd_connect:
                case emrascmd_connect_oem:
                    {
                        EC_T_DWORD  dwCookie = 0;

                        m_dwClientVersion = EC_GET_FRM_DWORD(abyData);
                        m_pMasterStore->SetClientVersion(m_dwClientVersion);

                        dwRes = emrasProtoValidateConnect(&oRxHdr, ATEMRASSRV_VERSION, dwReadLen, abyData);

                        if (EC_E_NOERROR != dwRes)
                        {
                            /* logon not ok! */
                            EC_T_PBYTE          pbySend     = EC_NULL;
                            EC_T_DWORD          dwWriteLen  = 0;

                            VERBOSEOUT(1, ("Logon invalid: %s (0x%lx)\n", emRasErrorText(dwRes), dwRes));
                            dwRetVal = dwRes;

                            /* send nack */
                            dwRes = emrasProtoCreateNack(&oRxHdr, dwRes, &pbySend, &dwWriteLen);
                            if ((EC_E_NOERROR != dwRes) || (EC_NULL == pbySend) || (0 == dwWriteLen))
                            {
                                ATEMRAS_PROTOHDR_SET_LENGTH(&oRxHdr, 0);
                                ATEMRAS_PROTOHDR_SET_STATUS_ACK(&oRxHdr, EC_FALSE);
                                ATEMRAS_PROTOHDR_SET_STATUS_REQ(&oRxHdr, EC_FALSE);
                                dwRes = SendData(oRxHdr.byData, ATEMRAS_T_PROTOHDR_SIZE);

                                if (EC_NULL != pbySend)
                                {
                                    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasSrvClient::WaitForLogon pbySend", pbySend, 0);
                                    OsFree(pbySend);
                                    pbySend = EC_NULL;
                                    dwWriteLen = 0;
                                }

                                if (EC_E_SOCKET_DISCONNECTED == dwRes)
                                {
                                    m_pSocket->Disconnect(EC_FALSE);
                                    bLogOnPend  = EC_FALSE;

                                    SETM_OCONSTATE_LOCK(emrasconstate_disconnect);

                                    dwRetVal = EMRAS_E_LOGONCANCELLED;
                                    goto Exit;
                                }
                            }
                            else
                            {
                                dwRes = SendData(pbySend, dwWriteLen);

                                if (EC_NULL != pbySend)
                                {
                                    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasSrvClient::WaitForLogon pbySend", pbySend, 0);
                                    OsFree(pbySend);
                                    pbySend = EC_NULL;
                                    dwWriteLen = 0;
                                }

                                if (EC_E_SOCKET_DISCONNECTED == dwRes)
                                {
                                    m_pSocket->Disconnect(EC_FALSE);
                                    bLogOnPend = EC_FALSE;

                                    SETM_OCONSTATE_LOCK(emrasconstate_disconnect);

                                    dwRetVal = EMRAS_E_LOGONCANCELLED;
                                    goto Exit;
                                }
                            }

                            OsDbgAssert(bSocketLocked);
                            bSocketLocked = EC_FALSE;
                            m_oSockLock.UnLock();

                            continue;
                        }

                        dwCookie = ATEMRAS_PROTOHDR_GET_COOKIE(&oRxHdr);

                        if ((dwCookie == m_dwCookie) || (0 == dwCookie))
                        {
                            /* new connection */
                            EC_T_WORD   wVersion = (ATEMRASSRV_VERS_MAJ<<8) | ATEMRASSRV_VERS_MIN;

                            ATEMRAS_PROTOHDR_SET_COOKIE(&oRxHdr, m_dwCookie);
                            ATEMRAS_PROTOHDR_SET_STATUS_ACK(&oRxHdr, EC_TRUE);
                            ATEMRAS_PROTOHDR_SET_STATUS_REQ(&oRxHdr, EC_FALSE);
                            ATEMRAS_PROTOHDR_SET_VERSIONENR(&oRxHdr, wVersion);
                            ATEMRAS_PROTOHDR_SET_LENGTH(&oRxHdr, 0);

                            dwRes = SendData(oRxHdr.byData, ATEMRAS_T_PROTOHDR_SIZE);
                            if (EC_E_NOERROR != dwRes)
                            {
                                m_pSocket->Disconnect(EC_FALSE);
                                bLogOnPend  = EC_FALSE;

                                SETM_OCONSTATE_LOCK(emrasconstate_disconnect);

                                dwRetVal = EMRAS_E_LOGONCANCELLED;
                                goto Exit;
                            }

                            bLogOnPend = EC_FALSE;
                            SETM_OCONSTATE_LOCK(emrasconstate_idle);
                        }
                        else
                        {
                            /* search existing connection */
                            CAtemRasSrvClient*  pFoundClient = EC_NULL;
                            EC_T_PBYTE          pbySend      = EC_NULL;
                            EC_T_DWORD          dwWriteLen   = 0;

                            pFoundClient = m_pServer->FindClientByCookie(dwCookie);
                            if (EC_NULL == pFoundClient)
                            {
                                /* cannot find slave for reconnect, send NACK */
                                dwRes = emrasProtoCreateNack(&oRxHdr, EMRAS_E_INVALIDCOOKIE, &pbySend, &dwWriteLen);
                                if ((EC_E_NOERROR != dwRes) || (EC_NULL == pbySend) || (0 == dwWriteLen))
                                {
                                    ATEMRAS_PROTOHDR_SET_LENGTH(&oRxHdr, 0);
                                    ATEMRAS_PROTOHDR_SET_STATUS_ACK(&oRxHdr, EC_FALSE);
                                    ATEMRAS_PROTOHDR_SET_STATUS_REQ(&oRxHdr, EC_FALSE);
                                    dwRes = SendData(oRxHdr.byData, ATEMRAS_T_PROTOHDR_SIZE);

                                    if (EC_NULL != pbySend)
                                    {
                                        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasSrvClient::WaitForLogon pbySend", pbySend, 0);
                                        OsFree(pbySend);
                                        pbySend = EC_NULL;
                                        dwWriteLen = 0;
                                    }

                                    if (EC_E_SOCKET_DISCONNECTED == dwRes)
                                    {
                                        m_pSocket->Disconnect(EC_FALSE);
                                        bLogOnPend  = EC_FALSE;

                                        SETM_OCONSTATE_LOCK(emrasconstate_disconnect);

                                        dwRetVal    = EMRAS_E_LOGONCANCELLED;
                                        goto Exit;
                                    }
                                }
                                else
                                {
                                    dwRes = SendData(pbySend, dwWriteLen);

                                    if (EC_NULL != pbySend)
                                    {
                                        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasSrvClient::WaitForLogon pbySend", pbySend, 0);
                                        OsFree(pbySend);
                                        pbySend = EC_NULL;
                                        dwWriteLen = 0;
                                    }

                                    if (EC_E_SOCKET_DISCONNECTED == dwRes)
                                    {
                                        m_pSocket->Disconnect(EC_FALSE);
                                        bLogOnPend  = EC_FALSE;

                                        SETM_OCONSTATE_LOCK(emrasconstate_disconnect);

                                        dwRetVal    = EMRAS_E_LOGONCANCELLED;
                                        goto Exit;
                                    }
                                }
                            }
                            else
                            {
                                /* did find exchangeable client */
                                dwRes = pFoundClient->ExchangeSocket(m_pSocket);
                                if( EC_E_NOERROR != dwRes )
                                {
                                    m_bSockDisposable = EC_TRUE;
                                    /* cannot find slave for reconnect, send NACK */
                                    dwRes = emrasProtoCreateNack(&oRxHdr, dwRes, &pbySend, &dwWriteLen);
                                    if( (EC_E_NOERROR != dwRes) || (EC_NULL == pbySend) || (0 == dwWriteLen) )
                                    {
                                        ATEMRAS_PROTOHDR_SET_LENGTH(&oRxHdr, 0);
                                        ATEMRAS_PROTOHDR_SET_STATUS_ACK(&oRxHdr, EC_FALSE);
                                        ATEMRAS_PROTOHDR_SET_STATUS_REQ(&oRxHdr, EC_FALSE);
                                        dwRes = SendData(oRxHdr.byData, ATEMRAS_T_PROTOHDR_SIZE);

                                        if (EC_NULL != pbySend)
                                        {
                                            EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasSrvClient::WaitForLogon pbySend", pbySend, 0);
                                            OsFree(pbySend);
                                            pbySend = EC_NULL;
                                            dwWriteLen = 0;
                                        }

                                        if (EC_E_SOCKET_DISCONNECTED == dwRes)
                                        {
                                            m_pSocket->Disconnect(EC_FALSE);
                                            bLogOnPend  = EC_FALSE;

                                            SETM_OCONSTATE_LOCK(emrasconstate_disconnect);

                                            dwRetVal    = EMRAS_E_LOGONCANCELLED;
                                            goto Exit;
                                        }
                                    }
                                    else
                                    {
                                        dwRes = SendData(pbySend, dwWriteLen);

                                        if (EC_NULL != pbySend)
                                        {
                                            EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasSrvClient::WaitForLogon pbySend", pbySend, 0);
                                            OsFree(pbySend);
                                            pbySend = EC_NULL;
                                            dwWriteLen = 0;
                                        }

                                        if (EC_E_SOCKET_DISCONNECTED == dwRes)
                                        {
                                            m_pSocket->Disconnect(EC_FALSE);
                                            bLogOnPend  = EC_FALSE;

                                            SETM_OCONSTATE_LOCK(emrasconstate_disconnect);

                                            dwRetVal    = EMRAS_E_LOGONCANCELLED;
                                            goto Exit;
                                        }
                                    }
                                } /* exchange error */
                                else
                                {
                                    /* exchange ok */
                                    ATEMRAS_PROTOHDR_SET_LENGTH(&oRxHdr, 0);
                                    ATEMRAS_PROTOHDR_SET_STATUS_ACK(&oRxHdr, EC_TRUE);
                                    ATEMRAS_PROTOHDR_SET_STATUS_REQ(&oRxHdr, EC_FALSE);

                                    SendData(oRxHdr.byData, ATEMRAS_T_PROTOHDR_SIZE);

                                    bLogOnPend          = EC_FALSE;
                                    m_bSockDisposable   = EC_FALSE;

                                    SETM_OCONSTATE_LOCK(emrasconstate_disconnect);

                                    /* free this thread socket usage */
                                    OsDbgAssert(!bSocketLocked);
                                    m_oSockLock.ChangeLock(EC_WAITINFINITE, EC_TRUE);
                                    bSocketLocked = EC_TRUE;
                                    pFoundClient->FinalizeExchange();

                                    dwRetVal = EMRAS_EVT_RECONNECT;
                                    goto Exit;
                                }
                            }
                        } /* re con */
                    } break;
                case emrascmd_disconnect:
                    {
                        /* respond ack and close socket */
                        ATEMRAS_PROTOHDR_SET_LENGTH(&oRxHdr, 0);
                        ATEMRAS_PROTOHDR_SET_STATUS_REQ(&oRxHdr, EC_FALSE);
                        ATEMRAS_PROTOHDR_SET_STATUS_ACK(&oRxHdr, EC_TRUE);

                        dwRes = SendData(oRxHdr.byData, ATEMRAS_T_PROTOHDR_SIZE);
                        if( EC_E_SOCKET_DISCONNECTED == dwRes )
                        {
                            /* socket closed */
                            m_pSocket->Disconnect(EC_FALSE);
                            bLogOnPend  = EC_FALSE;

                            SETM_OCONSTATE_LOCK(emrasconstate_disconnect);

                            dwRetVal    = EMRAS_E_LOGONCANCELLED;
                            goto Exit;
                        }

                        /* now close down socket */
                        m_pSocket->Disconnect(EC_FALSE);
                        SETM_OCONSTATE_LOCK(emrasconstate_disconnect);

                        m_oWatchdog.Stop();

                        dwRetVal = EMRAS_E_LOGONCANCELLED;
                        goto Exit;
                    } break;
                case emrascmd_idle:
                    {
                        /* respond ack */
                        ATEMRAS_PROTOHDR_SET_LENGTH(&oRxHdr, 0);
                        ATEMRAS_PROTOHDR_SET_STATUS_REQ(&oRxHdr, EC_FALSE);
                        ATEMRAS_PROTOHDR_SET_STATUS_ACK(&oRxHdr, EC_TRUE);

                        dwRes = SendData(oRxHdr.byData, ATEMRAS_T_PROTOHDR_SIZE);
                        if( EC_E_SOCKET_DISCONNECTED == dwRes )
                        {
                            /* socket closed */
                            m_pSocket->Disconnect(EC_FALSE);
                            bLogOnPend  = EC_FALSE;
                            SETM_OCONSTATE_LOCK(emrasconstate_disconnect);
                            dwRetVal    = EMRAS_E_LOGONCANCELLED;
                            goto Exit;
                        }
                    } break;
                default:
                    {
                        /* shall never be here ! */
                        OsDbgAssert(EC_FALSE);
                    } break;
                }
            }
            else
            {
                /* it is a response or invalid request -> invalid header, retry */
                bLogOnPend = EC_TRUE;
                SETM_OCONSTATE_LOCK(emrasconstate_init);
            }
        }

        if (bSocketLocked)
        {
            bSocketLocked = EC_FALSE;
            m_oSockLock.UnLock();
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if (bSocketLocked)
    {
        bSocketLocked = EC_FALSE;
        m_oSockLock.UnLock();
    }

    /* notify logon */
    oConNotDesc.dwCause  = dwRetVal;
    oConNotDesc.dwCookie = m_dwCookie;

    m_pServer->NotifyCallee(ATEMRAS_NOTIFY_CONNECTION, sizeof(ATEMRAS_T_CONNOTIFYDESC), (EC_T_PBYTE)&oConNotDesc);

    VERBOSEOUT( 2, ("<CAtemRasSrvClient(this=0x%08X)::WaitForLogon()=0x%lx(%s) \n", this, dwRetVal,
        ((EMRAS_E_ERROR==(dwRetVal&EMRAS_E_ERROR))?emRasErrorText(dwRetVal):emRasEventText(dwRetVal))) );

    if (EC_E_SOCKET_DISCONNECTED == dwRetVal)
    {
        SETM_OCONSTATE_LOCK(emrasconstate_disconnect);
    }

    return dwRetVal;
} /* WaitForLogon */

/***************************************************************************************************/
/**
\brief  Main Client worker.

Process all client requests on established connection.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::RxThreadStep()
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;
    EC_T_DWORD              dwRes       = EC_E_ERROR;
    EC_T_BOOL               bSocketLocked = EC_FALSE;
    EC_T_BOOL               bQueueLocked  = EC_FALSE;
    ATEMRAS_T_PROTOHDR*     pRxHdr      = EC_NULL;

    EC_T_DWORD              dwReadLen   = 0;
    EC_T_BOOL               bDoProcess  = EC_FALSE;
    EC_T_DWORD              dwSendId    = 0;

    if (emrasconstate_terminated == m_oConState)
    {
        OsSleep(1);
        return EC_E_NOERROR;
    }

    pRxHdr = (ATEMRAS_T_PROTOHDR*)m_pRxBuffer;

    {
        /* do aging if required */
        Aging();

        /* acquire socket */
        OsDbgAssert(!bSocketLocked);
        m_oSockLock.Lock();
        bSocketLocked = EC_TRUE;

        /* check socket */
        if ((EC_NULL == m_pSocket) || (m_pSocket->IsDisconnect()) || (EC_NULL == m_pCmdQueue))
        {
            /* no valid socket connection, wait for re-connect or timeout */
            bSocketLocked = EC_FALSE;
            m_oSockLock.UnLock();

            /* simulate cycle */
            OsSleep(m_dwCycleTime);

            goto Exit;
        }
        bDoProcess = EC_TRUE;

        /* send notifications to client */
        {
            for (CAtEmMasterInstance* pMasterInstance = m_pMasterStore->GetInstanceRoot(); EC_NULL != pMasterInstance; pMasterInstance = pMasterInstance->Next())
            {
            CAtEmRasNotificationStore* pNotifyStore = pMasterInstance->GetNotifyStore();

                if (EC_NULL != pNotifyStore)
                {
                    for (EMMARSH_T_NOTIFICATION_ALLOC* pNotificationAlloc = pNotifyStore->RemoveEntry(); EC_NULL != pNotificationAlloc; pNotificationAlloc = pNotifyStore->RemoveEntry())
                    {
                        dwRes = SendData(pNotificationAlloc->byData, pNotificationAlloc->dwDataSize, pNotificationAlloc->pbyMbxData, pNotificationAlloc->dwMbxDataSize);
                        if (EC_E_NOERROR != dwRes)
                        {
                            VERBOSEOUT(0, ("Error sending notification! 0x%08x\n", dwRes));
                            bDoProcess = EC_FALSE;
                        }
                        pNotifyStore->FreeEntry(pNotificationAlloc);
                        
                        if (!bDoProcess)
                        {
                            break;
                        }
                    }
                }
            }
        }
        /* Send data from Queue */
        m_pCmdQueue->LockQueue();
        bQueueLocked = EC_TRUE;

        while (EC_E_NOERROR == m_pCmdQueue->GetNextSendId(&dwSendId))
        {
            EC_T_PBYTE  pbyTrxData  = EC_NULL;
            EC_T_DWORD  dwTrxLen    = 0;
            EC_T_DWORD  dwSeqNr     = 0;

            dwRes = m_pCmdQueue->GetTxEntry(dwSendId, &pbyTrxData, &dwTrxLen, &dwSeqNr);
            if (EC_E_NOERROR != dwRes)
            {
                /* Error while getting GetTxEntry for dwSendId */
                OsDbgAssert(EC_FALSE);
                bDoProcess = EC_FALSE;
                break;
            }
            m_pCmdQueue->UnlockQueue();
            bQueueLocked = EC_FALSE;

            ATEMRAS_PROTOHDR_SET_SEQUENCENR(((ATEMRAS_PT_PROTOHDR)pbyTrxData), dwSeqNr);

            dwRes = SendData(pbyTrxData, dwTrxLen);
            if (EC_E_NOERROR != dwRes)
            {
                /* Error while getting GetTxEntry for dwSendId */
                VERBOSEOUT(1, ("Error sending header! 0x%08x\n", dwRes));
                bDoProcess = EC_FALSE;
                break;
            }

            m_pCmdQueue->LockQueue();
            bQueueLocked = EC_TRUE;

            m_pCmdQueue->SetTxDone(dwSendId);
        }
        if (bQueueLocked)
        {
            bQueueLocked = EC_FALSE;
            m_pCmdQueue->UnlockQueue();
        }

        if (!bDoProcess)
        {
            /* start new cycle */
            bSocketLocked = EC_FALSE;
            m_oSockLock.UnLock();

            /* simulate cycle */
            OsSleep(m_dwCycleTime);

            goto Exit;
        }

        /* acquire incoming data */
        OsMemset(pRxHdr, 0, ATEMRAS_T_PROTOHDR_SIZE);
        dwReadLen   = ATEMRAS_T_PROTOHDR_SIZE;

        /* don't wait the complete RAS recv timeout, because maybe there is queued data to be sent */
        dwRes = m_pSocket->RecvData(pRxHdr->byData, &dwReadLen, m_dwCycleTime);
        if (EC_E_SOCKET_DISCONNECTED == dwRes)
        {
            Disconnect(dwRes, EC_TRUE);
            goto Exit;
        }

        if ((EC_E_NOERROR != dwRes) || (EC_NULL == m_pSocket) || (m_pSocket->IsDisconnect()) || (0 == dwReadLen) )
        {
            OsDbgAssert((EC_E_TIMEOUT == dwRes) || (EC_E_ACCESSDENIED == dwRes) || (EC_E_SOCKET_DISCONNECTED == dwRes));
            bDoProcess = EC_FALSE;
        }

        if (bDoProcess)
        {
            if (ATEMRAS_T_PROTOHDR_SIZE < dwReadLen)
            {
                OsDbgAssert(EC_FALSE);
                dwRetVal = EMRAS_E_INVALIDDATARECEIVED;
                goto Exit;
            }

            if (m_dwCookie != ATEMRAS_PROTOHDR_GET_COOKIE(pRxHdr))
            {
                OsDbgAssert(EC_FALSE);
                dwRetVal = EMRAS_E_INVALIDDATARECEIVED;
                goto Exit;
            }

            /* ok valid header */
            EC_T_BYTE*  pbyDataRead     = EC_NULL;
            EC_T_DWORD  dwDataRead      = 0;

            if( m_oWatchdog.IsStarted() )
            {
                m_oWatchdog.Restart();
            }
            else
            {
                m_oWatchdog.Start(m_dwWDReload);
            }
            SETM_OCONSTATE_LOCK(emrasconstate_idle);

            dwDataRead  = (ATEMRAS_PROTOHDR_GET_LENGTH(pRxHdr));
            if (dwDataRead > 0)
            {
                /* extend memory */
                if (dwDataRead + ATEMRAS_T_PROTOHDR_SIZE > m_dwRxBufSize)
                {
                    ATEMRAS_T_PROTOHDR oHdrSave;
                    OsMemcpy(&oHdrSave, pRxHdr, ATEMRAS_T_PROTOHDR_SIZE);

                    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasSrvClient::ClientWorker m_pRxBuffer", m_pRxBuffer, m_dwRxBufSize);
                    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasSrvClient::ClientWorker m_pRxBuffer", m_pRxBuffer, dwDataRead + ATEMRAS_T_PROTOHDR_SIZE + 256);
                    m_pRxBuffer = (EC_T_BYTE*)OsRealloc(m_pRxBuffer, dwDataRead + ATEMRAS_T_PROTOHDR_SIZE + 256);
                    if (EC_NULL == m_pRxBuffer)
                    {
                        m_dwRxBufSize = 0;
                        VERBOSEOUT(1, ("Error: Not enough free memory left for CAtemRasSrvClient::m_pRxBuffer!\n"));
                        Disconnect(EC_E_NOMEMORY);
                        goto Exit;
                    }

                    m_dwRxBufSize = dwDataRead + ATEMRAS_T_PROTOHDR_SIZE + 256;
                    pRxHdr      = (ATEMRAS_T_PROTOHDR*)m_pRxBuffer;
                    OsMemcpy(pRxHdr, &oHdrSave, ATEMRAS_T_PROTOHDR_SIZE);
                }

                pbyDataRead = &m_pRxBuffer[ATEMRAS_T_PROTOHDR_SIZE];
                if (EC_NULL == pbyDataRead)
                {
                    /* only NACK if receival was a REQ */
                    if( ATEMRAS_PROTOHDR_GET_STATUS_REQ(pRxHdr) )
                    {
                        /* respond to client we cannot alloc data memory */
                        if( EC_E_NOERROR != emrasProtoCreateNackNoAlloc(pRxHdr, EC_E_NOMEMORY, pbyDataRead, &dwDataRead) )
                        {
                            dwDataRead  = ATEMRAS_T_PROTOHDR_SIZE;
                            pbyDataRead = pRxHdr->byData;

                            ATEMRAS_PROTOHDR_SET_LENGTH(pRxHdr, 0);
                            ATEMRAS_PROTOHDR_SET_STATUS_ACK(pRxHdr, EC_FALSE);
                            ATEMRAS_PROTOHDR_SET_STATUS_REQ(pRxHdr, EC_FALSE);
                        }

                        /* don't care about NACK result, socket errors are detected elsewhere */
                        SendData(pbyDataRead, dwDataRead);
                    }

                    bDoProcess = EC_FALSE;
                }
            }

            if( bDoProcess )
            {
                /* well we got memory so read */
                if (dwDataRead > 0)
                {
					EC_T_DWORD dwDataReadRest = dwDataRead;
					EC_T_BYTE* pbyDataReadCur = pbyDataRead;
					EC_T_DWORD dwDataReadCur =  0;

					do {
						if (dwDataReadRest > ATEMRAS_READ_PARTIAL_LENGTH)
						{
							dwDataReadCur = ATEMRAS_READ_PARTIAL_LENGTH;
						}
						else
						{
							dwDataReadCur = dwDataReadRest;
						}
						dwRes = m_pSocket->RecvData(pbyDataReadCur, &dwDataReadCur, RAS_CMD_RECV_TIMEOUT);
						if (EC_E_NOERROR != dwRes)
						{
							break;
						}
						dwDataReadRest -= dwDataReadCur;
						pbyDataReadCur += dwDataReadCur;

					} while (0 != dwDataReadRest);

                    if( EC_E_NOERROR != dwRes )
                    {
                        VERBOSEOUT(1, ("CAtemRasSrvClient.ClientWorker(...) Error reading data portion! 0x%08x\n", dwRes));
                        /* error reading data portion, send Nack if request */
                        /* only NACK if receival was a REQ */
                        if( ATEMRAS_PROTOHDR_GET_STATUS_REQ(pRxHdr) )
                        {
                            /* respond to client we cannot alloc data memory */
                            if( EC_E_NOERROR != emrasProtoCreateNackNoAlloc(pRxHdr, EC_E_INVALIDDATA, pbyDataRead, &dwDataRead) )
                            {
                                dwDataRead  = ATEMRAS_T_PROTOHDR_SIZE;
                                pbyDataRead = pRxHdr->byData;

                                ATEMRAS_PROTOHDR_SET_LENGTH(pRxHdr, 0);
                                ATEMRAS_PROTOHDR_SET_STATUS_ACK(pRxHdr, EC_FALSE);
                                ATEMRAS_PROTOHDR_SET_STATUS_REQ(pRxHdr, EC_FALSE);
                            }

                            /* don't care about NACK result, socket errors are detected elsewhere */
                            SendData(pbyDataRead, dwDataRead);
                        }

                        bDoProcess = EC_FALSE;
                    }
                }

                if( bDoProcess )
                {
                    /* Check if this is a request from the client or a response to a server's request */
                    if( !ATEMRAS_PROTOHDR_GET_STATUS_REQ(pRxHdr) )
                    {
                        /* response from client, find request and ack it */
                        EC_T_DWORD  dwReqIndex  = 0;
                        EC_T_PBYTE  pbyRxBuffer = EC_NULL;
                        EC_T_DWORD* pdwRxLength = EC_NULL;

                        m_pCmdQueue->LockQueue();

                        if( EC_E_NOERROR == m_pCmdQueue->FindEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pRxHdr), &dwReqIndex) )
                        {
                            /* found entry */
                            if( EC_E_NOERROR == m_pCmdQueue->GetRxEntry(dwReqIndex, &pbyRxBuffer, &pdwRxLength) )
                            {
                                /* got buffers */
                                if( (EC_NULL != pbyRxBuffer) && (EC_NULL != pdwRxLength))
                                {
                                    /* buffers valid */
                                    if( !(EC_GET_FRM_DWORD(pdwRxLength) < ATEMRAS_T_PROTOHDR_SIZE) )
                                    {
                                        OsMemcpy(pbyRxBuffer, pRxHdr->byData, ATEMRAS_T_PROTOHDR_SIZE);

                                        if( !(EC_GET_FRM_DWORD(pdwRxLength) < (ATEMRAS_T_PROTOHDR_SIZE+dwDataRead)) )
                                        {
                                            OsMemcpy(&(pbyRxBuffer[ATEMRAS_T_PROTOHDR_SIZE]), pbyDataRead, dwDataRead);
                                            EC_SET_FRM_DWORD(pdwRxLength, (ATEMRAS_T_PROTOHDR_SIZE+dwDataRead));
                                        } /* buffer for Header+data */
                                        else
                                        {
                                            EC_SET_FRM_DWORD(pdwRxLength, ATEMRAS_T_PROTOHDR_SIZE);
                                        } /* buffer for Header only */
                                    } /* buffer large enough for Header */
                                } /* got buffer pointers */

                                m_pCmdQueue->SetRxDone(dwReqIndex);

                            } /* getrxentry */
                        } /* find entry */

                        m_pCmdQueue->UnlockQueue();
                    }
                    else
                    {
                        /* request from client process and ack it */
                        EC_T_DWORD  dwWrtLen    = 0;
                        EC_T_BYTE   byTxBuffer[128];

                        static EC_T_BOOL bIdleCmdNotified = EC_FALSE;

                        if (!bIdleCmdNotified || (ATEMRAS_PROTOHDR_GET_CMD(pRxHdr) != emrascmd_idle))
                        {
                            VERBOSEOUT(2, ("CAtemRasSrvClient.ClientWorker(...) Got %s CMD.\n",
                                ATEMRAS_T_CMDTYPE_TEXT((ATEMRAS_T_CMDTYPE) ATEMRAS_PROTOHDR_GET_CMD(pRxHdr))));

                            if (ATEMRAS_PROTOHDR_GET_CMD(pRxHdr) == emrascmd_idle)
                            {
                                bIdleCmdNotified = EC_TRUE;
                            }
                        }

                        /* request from client, process it and ack it */
                        switch( ATEMRAS_PROTOHDR_GET_CMD(pRxHdr) )
                        {
                        case emrascmd_nop:
                        case emrascmd_idle:
                            {
                                /* nop or idle -> ack */
                                dwRes = emrasProtoCreateAckNoAlloc(pRxHdr, EC_E_NOERROR, byTxBuffer, &dwWrtLen);
                                if (EC_E_NOERROR == dwRes)
                                {
                                    dwRes = SendData(byTxBuffer, dwWrtLen);
                                }
                            } break;
                        case emrascmd_connect:
                            {
                                /* invalid state for connect */
                                dwRes = emrasProtoCreateNackNoAlloc(pRxHdr, EC_E_INVALIDSTATE, byTxBuffer, &dwWrtLen);
                                if (EC_E_NOERROR == dwRes)
                                {
                                    SendData(byTxBuffer, dwWrtLen);
                                }
                            } break;
                        case emrascmd_disconnect:
                            {
                                /* client wants to disconnect */
                                dwRes = emrasProtoCreateAckNoAlloc(pRxHdr, EC_E_NOERROR, byTxBuffer, &dwWrtLen);
                                if (EC_E_NOERROR == dwRes)
                                {
                                    dwRes = SendData(byTxBuffer, dwWrtLen);
                                }
                                Disconnect(EMRAS_EVT_CLNTDISC, EC_TRUE);
                            } break;
                        case emrascmd_emapicommand:
                            {
                                /* queue command */
                                dwRes = QueueCmd(pRxHdr, dwDataRead, pbyDataRead);
                                if (EC_E_NOERROR != dwRes)
                                {
                                    /* queued Nack in deMarshal */
                                    ATEMRAS_T_MARSHALERRORDESC oMarshError;
                                    VERBOSEOUT(1, ("Command rejected : %s (0x%lx)\n",
                                        emRasErrorText(dwRes), dwRes
                                            ));

                                    oMarshError.dwCookie        = m_dwCookie;
                                    oMarshError.dwCause         = dwRes;
                                    oMarshError.dwLenStatCmd    = ATEMRAS_PROTOHDR_GET_CMD(pRxHdr);
                                    if ((EC_NULL != pbyDataRead) && !(sizeof(EC_T_DWORD) > dwDataRead))
                                    {
                                        oMarshError.dwCommandCode = EC_GET_FRM_DWORD(pbyDataRead);
                                    }
                                    else
                                    {
                                        oMarshError.dwCommandCode = 0;
                                    }
                                    m_pServer->NotifyCallee(ATEMRAS_NOTIFY_MARSHALERROR, sizeof(ATEMRAS_T_MARSHALERRORDESC), (EC_T_PBYTE)&oMarshError);
                                }

                            } break;
                        case emrascmd_emapinotification:
                        default:
                            {
#if (defined DEBUG)
                                VERBOSEOUT(1, ("<-0x%x\n", ATEMRAS_PROTOHDR_GET_CMD(pRxHdr)));
#endif /*DEBUG */
                                /* Unhandled command do NACK */

                                dwRes = emrasProtoCreateNackNoAlloc(pRxHdr, EC_E_INVALIDCMD, byTxBuffer, &dwWrtLen);
                                if (EC_E_NOERROR == dwRes)
                                {
                                    SendData(byTxBuffer, dwWrtLen);
                                }
                            } break;
                        }
                    }
                }
            }
        }
        /* release socket to give a chance to an eventually waiting thread */
        bSocketLocked = EC_FALSE;
        m_oSockLock.UnLock();
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if (bQueueLocked)
    {
        bQueueLocked = EC_FALSE;
        m_pCmdQueue->UnlockQueue();
    }

    if (bSocketLocked)
    {
        bSocketLocked = EC_FALSE;
        m_oSockLock.UnLock();
    }

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Do aging of connection, based on m_dwSilenceCount.

The detailed description and the brief descriptions are separated by on blank line.
This function appears only in the detailed documentation.
\return Put the information about possible return values here.
*/
EC_T_VOID CAtemRasSrvClient::Aging(EC_T_VOID)
{
    if ((emrasconstate_wdidle == m_oConState) && m_oWatchdog.IsElapsed())
    {
        Disconnect(emrasconstate_wdexpired, EC_TRUE);
    }
}

/***************************************************************************************************/
/**
\brief  Give Slave instance a new socket.

This function is called on reconnect, to set newly spawned socket.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::ExchangeSocket(
    CEcSocket* pNewSocket      /**< [in]   New socket to add to Client object */
                                           )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    if (EC_NULL == pNewSocket)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    m_oSockLock.Lock(m_dwCycleTime, EC_TRUE);

    if( !m_pSocket->IsDisconnect() )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    if (EC_NULL != m_pSocket)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasSrvClient::ExchangeSocket m_pSocket", m_pSocket, 0);
        SafeDelete(m_pSocket);
    }

    m_pSocket           = pNewSocket;

    m_bSockDisposable   = EC_FALSE;

    SETM_OCONSTATE_LOCK(emrasconstate_idle);
    if( m_oWatchdog.IsStarted() )
    {
        m_oWatchdog.Restart();
    }
    else
    {
        m_oWatchdog.Start(m_dwWDReload);
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    /* leave socket locked ... unlock is done by call to FinalizeExchange! */

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Make socket usable agn.
*/
EC_T_VOID CAtemRasSrvClient::FinalizeExchange(EC_T_VOID)
{
    ATEMRAS_T_CONNOTIFYDESC oConNotDesc;

    oConNotDesc.dwCause     = EMRAS_EVT_SOCKCHANGE;
    oConNotDesc.dwCookie    = m_dwCookie;
    m_pServer->NotifyCallee(ATEMRAS_NOTIFY_CONNECTION, sizeof(ATEMRAS_T_CONNOTIFYDESC), (EC_T_PBYTE)&oConNotDesc);

    m_oSockLock.UnLock();
}

EC_T_VOID CAtemRasSrvClient::WorkerThreadStepWrapper(EC_T_PVOID pvParams)
{
    CAtemRasSrvClient* pClient = (CAtemRasSrvClient*)pvParams;

    pClient->WorkerThreadStep();
}

EC_T_DWORD CAtemRasSrvClient::QueueCmd(ATEMRAS_T_PROTOHDR* pHdr, EC_T_DWORD dwDataLen, EC_T_BYTE* pbyData)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwReqBufLen = ATEMRAS_T_PROTOHDR_SIZE + dwDataLen;
    ATEMRAS_T_BUFFER_DESC* pBufDesc = EC_NULL;

    if (m_pWorkerCmdFifo->IsFull())
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    /* fetch or create free desc from queue */
    if (!m_pWorkerFreeCmdFifo->RemoveNoLock(pBufDesc))
    {
        pBufDesc = (ATEMRAS_T_BUFFER_DESC*)OsMalloc(sizeof(ATEMRAS_T_BUFFER_DESC));
        if (EC_NULL == pBufDesc)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        OsMemset(pBufDesc, 0, sizeof(ATEMRAS_T_BUFFER_DESC));
    }

    /* increase buffer */
    if (pBufDesc->dwMaxLen < dwReqBufLen)
    {
        pBufDesc->pbyData = (EC_T_BYTE*)OsRealloc(pBufDesc->pbyData, dwReqBufLen);
        if (EC_NULL == pBufDesc->pbyData)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        pBufDesc->dwMaxLen = dwReqBufLen;
    }

    /* fill desc, header and data */
    OsMemcpy(pBufDesc->pbyData, (EC_T_BYTE*)pHdr, ATEMRAS_T_PROTOHDR_SIZE);
    if (dwDataLen > 0)
    {
        OsMemcpy(&pBufDesc->pbyData[ATEMRAS_T_PROTOHDR_SIZE], pbyData, dwDataLen);
    }
    pBufDesc->dwDataLen = dwReqBufLen;

    m_pWorkerCmdFifo->Add(pBufDesc);
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_VOID CAtemRasSrvClient::WorkerThreadStep(EC_T_VOID)
{
    EC_T_DWORD dwRes = EC_E_ERROR;
    ATEMRAS_T_BUFFER_DESC* pBufDesc = EC_NULL;
    EC_T_BYTE*  pbyDataRead     = EC_NULL;
    EC_T_DWORD  dwDataRead      = 0;
    ATEMRAS_T_PROTOHDR* pRxHdr = EC_NULL;

    if ((emrasconstate_terminated == m_oConState) || (emrasconstate_disconnect == m_oConState))
    {
        OsSleep(1);
        return;
    }

    if (DequeueCmd(pBufDesc))
    {
        pRxHdr = (ATEMRAS_T_PROTOHDR*)pBufDesc->pbyData;
        pbyDataRead = (pBufDesc->dwDataLen > ATEMRAS_T_PROTOHDR_SIZE) ? &pBufDesc->pbyData[ATEMRAS_T_PROTOHDR_SIZE] : EC_NULL;
        dwRes = deMarshal(pRxHdr, pBufDesc->dwDataLen - ATEMRAS_T_PROTOHDR_SIZE, pbyDataRead);
        switch (dwRes)
        {
        case EC_E_NOERROR: break;
        case EC_E_SOCKET_DISCONNECTED:
            {
                Disconnect(EC_E_SOCKET_DISCONNECTED);
                return;
            } break;
        default:
            {
                /* queued Nack in deMarshal */
                ATEMRAS_T_MARSHALERRORDESC oMarshError;
                VERBOSEOUT(1, ("Command rejected : %s (0x%lx)\n", emRasErrorText(dwRes), dwRes));

                oMarshError.dwCookie        = m_dwCookie;
                oMarshError.dwCause         = dwRes;
                oMarshError.dwLenStatCmd    = ATEMRAS_PROTOHDR_GET_CMD(pRxHdr);
                if ((EC_NULL != pbyDataRead) && !(sizeof(EC_T_DWORD)>dwDataRead))
                {
                    oMarshError.dwCommandCode = EC_GET_FRM_DWORD(pbyDataRead);
                }
                else
                {
                    oMarshError.dwCommandCode = 0;
                }
                m_pServer->NotifyCallee(ATEMRAS_NOTIFY_MARSHALERROR, sizeof(ATEMRAS_T_MARSHALERRORDESC), (EC_T_PBYTE)&oMarshError);
            }
        }
        //OsDbgAssert(!m_pWorkerFreeCmdFifo->IsFull());
        m_pWorkerFreeCmdFifo->Add(pBufDesc);
    }
    else
    {
        /* TODO: wait for event */
        OsSleep(1);
    }
}

/*-CAtemRasAccessControl-----------------------------------------------------*/

#if (defined INCLUDE_RAS_SPOCSUPPORT)

/***************************************************************************************************/
/**
\brief  Default constructor.
*/
CAtemRasAccessControl::CAtemRasAccessControl(
    EC_T_BYTE*              pbyData         /**< [in]   Configuration data or storage location of it */
   ,EC_T_DWORD              dwLen           /**< [in]   Length of data / filename */
                                            )
{
    m_pEntryRoot        = EC_NULL;
    m_pClassStateRoot   = EC_NULL;

    ConfigFromData(pbyData, dwLen);
}

/***************************************************************************************************/
/**
\brief  Default destructor.
*/
CAtemRasAccessControl::~CAtemRasAccessControl()
{
    ATEMRAS_T_ACCFG_ENTRY*  pTmp        = EC_NULL;

    while (EC_NULL != m_pEntryRoot)
    {
        pTmp = m_pEntryRoot;
        m_pEntryRoot = pTmp->pNext;

        if (EC_NULL != m_pEntryRoot)
        {
            m_pEntryRoot->pPrev = EC_NULL;
        }
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasAccessControl::~CAtemRasAccessControl pTmp", pTmp, sizeof(ATEMRAS_T_ACCFG_ENTRY));
        OsFree(pTmp);
    }
}

/***************************************************************************************************/
/**
\brief  Populate Entries from Data Pointer.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasAccessControl::ConfigFromData(
    EC_T_BYTE*  pbyData         /**< [in]   Configuration Data */
   ,EC_T_DWORD  dwLen           /**< [in]   Length of Configuration Data */
                                                )
{
    EC_T_DWORD  dwRetVal        = EC_E_ERROR;

    EC_T_DWORD  dwRemain        = dwLen;
    EC_T_BYTE*  pbyIterator     = pbyData;

    EC_T_BOOL   bIsBinaryCfg    = EC_FALSE;
    EC_T_DWORD  dwCmpLen        = OsStrlen("binaryconfig");

    /* first check for binary signature */
    if( 0 == OsMemcmp(pbyIterator, "binaryconfig", dwCmpLen) )
    {
        pbyIterator += dwCmpLen;
        dwRemain    -= dwCmpLen;
        bIsBinaryCfg = EC_TRUE;
    }

    if( bIsBinaryCfg )
    {
        dwRetVal = ConfigFromDataBin(pbyIterator, dwRemain);
    }
    else
    {
        dwRetVal = ConfigFromDataTxt(pbyIterator, dwRemain);
    }

    return dwRetVal;
}

#define CFDB_INCREMENT(ptr, remain, inc) \
    (ptr) = (((EC_T_BYTE*)(ptr)) + ((inc))); ((remain))-=((inc));

#define CFDB_CHECK(ptr, remain, retVal) \
    if( (EC_NULL == ((EC_T_BYTE*)(ptr))) || (0 == ((EC_T_DWORD)(remain))) ){    \
        ((retVal)) = EMRAS_E_INVALIDACCESSCONFIG; goto Exit; }

/***************************************************************************************************/
/**
\brief  Populate Entries from Binary Data Pointer.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasAccessControl::ConfigFromDataBin(
    EC_T_BYTE*  pbyData         /**< [in]   Configuration Data */
   ,EC_T_DWORD  dwLen           /**< [in]   Length of Configuration Data */
                                                   )
{
    EC_T_DWORD                  dwRetVal                = EC_E_ERROR;

    EC_T_DWORD                  dwRemain                = dwLen;
    EC_T_BYTE*                  pbyIterator             = pbyData;
    EC_T_DWORD                  dwClassConfigEntries    = 0;
    ATEMRAS_T_AC_CLASSSTATE*    pCfgClassStates         = EC_NULL;
    ATEMRAS_T_AC_CLASSSTATE*    pCurClassState          = EC_NULL;
    EC_T_DWORD                  dwIdx                   = 0;
    ATEMRAS_T_ACCFG_ENTRY*      pCurEntry               = EC_NULL;
    EC_T_DWORD                  dwCallSpecificEnh       = 0;

    /* first read out configuration headers */

    CFDB_CHECK(pbyIterator, dwRemain, dwRetVal);

    dwClassConfigEntries = EC_GET_FRM_DWORD(pbyIterator); CFDB_INCREMENT(pbyIterator, dwRemain, sizeof(EC_T_DWORD));

    CFDB_CHECK(pbyIterator, dwRemain, dwRetVal);

    pCfgClassStates = (ATEMRAS_T_AC_CLASSSTATE*)OsMalloc((sizeof(ATEMRAS_T_AC_CLASSSTATE)*dwClassConfigEntries));
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasAccessControl::ConfigFromDataBin pCfgClassStates", pCfgClassStates, (sizeof(ATEMRAS_T_AC_CLASSSTATE)*dwClassConfigEntries));
    if (EC_NULL == pCfgClassStates)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    OsMemset(pCfgClassStates, 0, (sizeof(ATEMRAS_T_AC_CLASSSTATE)*dwClassConfigEntries));

    for(dwIdx = 0; dwIdx < dwClassConfigEntries; dwIdx++ )
    {
        pCfgClassStates[dwIdx].dwClassId        = EC_GET_FRM_DWORD(pbyIterator); CFDB_INCREMENT(pbyIterator, dwRemain, sizeof(EC_T_DWORD));
        CFDB_CHECK(pbyIterator, dwRemain, dwRetVal);

        pCfgClassStates[dwIdx].dwAccessLevel    = EC_GET_FRM_DWORD(pbyIterator); CFDB_INCREMENT(pbyIterator, dwRemain, sizeof(EC_T_DWORD));
        CFDB_CHECK(pbyIterator, dwRemain, dwRetVal);
}

    /* now read class configuration */
    do {
        pCurEntry = (ATEMRAS_T_ACCFG_ENTRY*)OsMalloc(sizeof(ATEMRAS_T_ACCFG_ENTRY));
        EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasAccessControl::ConfigFromDataBin pCurEntry", pCurEntry, sizeof(ATEMRAS_T_ACCFG_ENTRY));
        if (EC_NULL == pCurEntry)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }

        OsMemset(pCurEntry, 0, sizeof(ATEMRAS_T_ACCFG_ENTRY));
        dwCallSpecificEnh = 0;

        CFDB_CHECK(pbyIterator, dwRemain, dwRetVal);

        pCurEntry->dwCallClass = EC_GET_FRM_DWORD(pbyIterator); CFDB_INCREMENT(pbyIterator, dwRemain, sizeof(EC_T_DWORD));

        if( 0 == pCurEntry->dwCallClass )
        {
            continue;
        }

        CFDB_CHECK(pbyIterator, dwRemain, dwRetVal);

        pCurEntry->dwRequiredAccess = EC_GET_FRM_DWORD(pbyIterator); CFDB_INCREMENT(pbyIterator, dwRemain, sizeof(EC_T_DWORD));
        CFDB_CHECK(pbyIterator, dwRemain, dwRetVal);

        pCurEntry->dwCallOrdinal = EC_GET_FRM_DWORD(pbyIterator); CFDB_INCREMENT(pbyIterator, dwRemain, sizeof(EC_T_DWORD));
        CFDB_CHECK(pbyIterator, dwRemain, dwRetVal);

        pCurEntry->dwCallIndex = EC_GET_FRM_DWORD(pbyIterator); CFDB_INCREMENT(pbyIterator, dwRemain, sizeof(EC_T_DWORD));
        CFDB_CHECK(pbyIterator, dwRemain, dwRetVal);

        pCurEntry->dwCallSubIndex = EC_GET_FRM_DWORD(pbyIterator); CFDB_INCREMENT(pbyIterator, dwRemain, sizeof(EC_T_DWORD));
        CFDB_CHECK(pbyIterator, dwRemain, dwRetVal);

        dwCallSpecificEnh = EC_GET_FRM_DWORD(pbyIterator); CFDB_INCREMENT(pbyIterator, dwRemain, sizeof(EC_T_DWORD));

        if( 0 != dwCallSpecificEnh )
        {
            EC_T_BYTE* pbyEnhancement = EC_NULL;

            /* afterwards parse pvCallSpecificEnhancements if required by ordinal/index/subindex */
            pbyEnhancement = (EC_T_BYTE*)OsMalloc(dwCallSpecificEnh);
            EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasAccessControl::ConfigFromDataBin pbyEnhancement", pbyEnhancement, dwCallSpecificEnh);
            if (EC_NULL == pbyEnhancement)
            {
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }

            CFDB_CHECK(pbyIterator, dwRemain, dwRetVal);
            OsMemcpy(pbyEnhancement, pbyIterator, dwCallSpecificEnh); CFDB_INCREMENT(pbyIterator, dwRemain, dwCallSpecificEnh);

            pCurEntry->dwCSESize = dwCallSpecificEnh;
            pCurEntry->pbyCallSpecificEnhancements = pbyEnhancement;
        }

        pCurClassState = GetClassStateById(pCurEntry->dwCallClass);
        if (EC_NULL == pCurClassState)
        {
            pCurClassState = GetClassStateById(pCurEntry->dwCallClass, EC_TRUE);
        }

        if (EC_NULL == pCurClassState)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }

        pCurEntry->pCallState = pCurClassState;

        AppendClassCfgEntry(pCurEntry);

    } while( 0 != (pCurEntry->dwCallClass) );

    for(dwIdx = 0; dwIdx < dwClassConfigEntries; dwIdx++ )
    {
        ModifyClassState(pCfgClassStates[dwIdx].dwClassId, pCfgClassStates[dwIdx].dwAccessLevel);
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Populate Entries from Text Data Pointer.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasAccessControl::ConfigFromDataTxt(
    EC_T_BYTE*  pbyData         /**< [in]   Configuration Data */
   ,EC_T_DWORD  dwLen           /**< [in]   Length of Configuration Data */
                                                   )
{
    EC_T_DWORD  dwRetVal        = EC_E_ERROR;

    EC_T_DWORD  dwRemain        = dwLen;
    EC_T_BYTE*  pbyIterator     = pbyData;

    EC_UNREFPARM(dwRemain);
    EC_UNREFPARM(pbyIterator);

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Append Class Configuration entry.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasAccessControl::AppendClassCfgEntry(
    ATEMRAS_T_ACCFG_ENTRY*  pEntry      /**< [in]   Configuration entry to append */
                                                     )
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;
    ATEMRAS_T_ACCFG_ENTRY*  pTmp        = EC_NULL;

    if (EC_NULL == pEntry)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (EC_NULL == m_pEntryRoot)
    {
        m_pEntryRoot = pEntry;
    }
    else
    {
        pTmp = m_pEntryRoot;
        while( EC_NULL != pTmp->pNext ) pTmp=pTmp->pNext;

        pTmp->pNext = pEntry; pEntry->pPrev = pTmp;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get Handle to class state entry by id.

Class state entry is created, when not found and bCreate = TRUE.
\return Handle to class state entry, EC_NULL if not creatable / not found.
*/
ATEMRAS_T_AC_CLASSSTATE* CAtemRasAccessControl::GetClassStateById(
    EC_T_DWORD  dwClassId
   ,EC_T_BOOL   bCreate /* = EC_FALSE  */
                                                                 )
{
    ATEMRAS_T_AC_CLASSSTATE*    pRet    = EC_NULL;

    pRet = m_pClassStateRoot;

    while( EC_NULL != pRet )
    {
        if( pRet->dwClassId == dwClassId )
        {
            break;
        }

        pRet = pRet->pNext;
    }

    if (EC_NULL == pRet && bCreate)
    {
        if (EC_NULL == m_pEntryRoot)
        {
            pRet = m_pClassStateRoot = (ATEMRAS_T_AC_CLASSSTATE*)OsMalloc(sizeof(ATEMRAS_T_AC_CLASSSTATE));
            EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasAccessControl::GetClassStateById m_pClassStateRoot", m_pClassStateRoot, sizeof(ATEMRAS_T_AC_CLASSSTATE));
            if (EC_NULL != pRet)
            {
                OsMemset(pRet, 0, sizeof(ATEMRAS_T_AC_CLASSSTATE));
                pRet->dwClassId = dwClassId;
            }
        }
        else
        {
            pRet = m_pClassStateRoot;
            while( EC_NULL != pRet->pNext ) pRet = pRet->pNext;

            pRet->pNext = (ATEMRAS_T_AC_CLASSSTATE*)OsMalloc(sizeof(ATEMRAS_T_AC_CLASSSTATE));
            EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasAccessControl::GetClassStateById pRet->pNext", m_pClassStateRoot, sizeof(ATEMRAS_T_AC_CLASSSTATE));
            if (EC_NULL != pRet->pNext)
            {
                OsMemset(pRet->pNext, 0, sizeof(ATEMRAS_T_AC_CLASSSTATE));
                pRet->dwClassId = dwClassId;
                pRet->pNext->pPrev = pRet;
            }
            pRet = pRet->pNext;
        }
    }

    return pRet;
}

/***************************************************************************************************/
/**
\brief  Store Access Level for specific (existing) call class.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasAccessControl::ModifyClassState(
    EC_T_DWORD  dwClassId       /**< [in]   Call class ID */
   ,EC_T_DWORD  dwAccessLevel   /**< [in]   Access level to set */
                                                  )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    ATEMRAS_T_AC_CLASSSTATE*    pClassState = EC_NULL;

    if( 0 == dwClassId )
    {
        pClassState = m_pClassStateRoot;
        while( EC_NULL != pClassState )
        {
            pClassState->dwAccessLevel = dwAccessLevel;
            pClassState = pClassState->pNext;
        }
    }
    else
    {
        pClassState = GetClassStateById(dwClassId);
        if (EC_NULL == pClassState)
        {
            dwRetVal = EC_E_INVALIDINDEX;
            goto Exit;
        }

        pClassState->dwAccessLevel = dwAccessLevel;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Check Access permissions.

\return EC_E_NOERROR on access allowed, error code otherwise.
*/
EC_T_DWORD CAtemRasAccessControl::CallAccess(
    EC_T_DWORD  dwCallOrdinal               /**< [in]   Call Ordinal (mandatory) */
   ,EC_T_DWORD  dwCallIndex /* = 0 */       /**< [in]   Call Index (optional) */
   ,EC_T_DWORD  dwCallSubIndex /* = 0 */    /**< [in]   Call SubIndex (optional) */
   ,EC_T_BYTE*  pbyCSE /* = EC_NULL */      /**< [in]   Call Specific enhancement (optional) */
                                            )
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;
    ATEMRAS_T_ACCFG_ENTRY*  pCfgEntry   = EC_NULL;

    if (EC_NULL == m_pEntryRoot)
    {
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }

    pCfgEntry = FindCallEntry(dwCallOrdinal, dwCallIndex, dwCallSubIndex, pbyCSE);

    if (EC_NULL == pCfgEntry)
    {
        dwRetVal = EMRAS_E_ACCESSLESS;
        goto Exit;
    }

    if( pCfgEntry->dwRequiredAccess < pCfgEntry->pCallState->dwAccessLevel )
    {
        dwRetVal = EMRAS_E_ACCESSLESS;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Find Config entry.

Used to thread access level.
\return Handle to config entry on success, EC_NULL on error.
*/
ATEMRAS_T_ACCFG_ENTRY* CAtemRasAccessControl::FindCallEntry(
    EC_T_DWORD  dwCallOrdinal               /**< [in]   Call Ordinal (mandatory) */
   ,EC_T_DWORD  dwCallIndex /* = 0 */       /**< [in]   Call Index (optional) */
   ,EC_T_DWORD  dwCallSubIndex /* = 0 */    /**< [in]   Call SubIndex (optional) */
   ,EC_T_BYTE*  pbyCSE /* = EC_NULL */      /**< [in]   Call Specific enhancement (optional) */
                                                           )
{
    ATEMRAS_T_ACCFG_ENTRY*  pRet    = EC_NULL;
    EC_T_BOOL               bFound  = EC_FALSE;

    pRet = m_pEntryRoot;

    while( EC_NULL != pRet )
    {
            bFound = EC_TRUE;

        /* Ordinal does match ? */
        if( pRet->dwCallOrdinal != dwCallOrdinal )
            {
                bFound = EC_FALSE;
            }
        else
        {
            bFound = EC_TRUE;
        }

        if( bFound && (0xFFFFFFFF != pRet->dwCallIndex) )
            {
            if( pRet->dwCallIndex != dwCallIndex )
            {
                bFound = EC_FALSE;
            }
        }

        if( bFound && (0xffffffff != pRet->dwCallSubIndex) )
            {
            if( pRet->dwCallSubIndex != dwCallSubIndex )
            {
                bFound = EC_FALSE;
            }
        }
        if( bFound && ((EC_NULL != pbyCSE) && (EC_NULL != pRet->pbyCallSpecificEnhancements)) )
        {
            if( 0 != OsMemcmp(pbyCSE, pRet->pbyCallSpecificEnhancements, pRet->dwCSESize) )
            {
                bFound = EC_FALSE;
    }
}

        if( bFound )
            break;

        pRet = pRet->pNext;
    }

    return pRet;
}

#endif /* INCLUDE_RAS_SPOCSUPPORT */

