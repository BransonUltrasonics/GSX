/*-----------------------------------------------------------------------------
* EcSocket.cpp
* Copyright                acontis technologies GmbH, Weingarten, Germany
* Response                 Paul Bussmann
* Description              Socket abstraction
*---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/

#include <EcOs.h>
#if (defined EC_SOCKET_SUPPORTED)

#include <EcError.h>

#if (defined __INTIME__) || (defined EC_VERSION_QNX)
#include <netinet/tcp.h>
#endif

#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
#include <msgqueue.h>
#endif /* EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED */

#include "EcSocket.h"

/*-DEFINES-------------------------------------------------------------------*/
#if (defined EC_SOCKET_MSGQUEUE_SUPPORTED)
#define EC_SOCKET_INIT_RECV_TIMEOUT         (1000)   /* 1s */
#define EC_SOCKET_DISCONNECT_ACK_TIMEOUT    (1000)   /* 1s */
#endif /* EC_SOCKET_MSGQUEUE_SUPPORTED */

/*-TYPEDEFS------------------------------------------------------------------*/

/*-STATIC CLASS-MEMBERS------------------------------------------------------*/
#if (defined EC_SOCKET_IP_SUPPORTED)
EC_T_DWORD   CEcSocketIp::s_dwInitSocketLayerCount = 0;
#endif /* EC_SOCKET_IP_SUPPORTED */

/*-FUNCTIONS-----------------------------------------------------------------*/
#if (defined EC_SOCKET_MSGQUEUE_SUPPORTED)
/***************************************************************************************************/
/**
\brief  Constructor of CMsgQueue
*/
CMsgQueue::CMsgQueue()
{
    m_dwId      = EC_INVALID_QUEUE_ID;
    m_bOpened   = EC_FALSE;
    m_bCreated  = EC_FALSE;

#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    m_hOsMsgQueue = INVALID_HANDLE_VALUE;
    m_oMsgQueueOptions.dwFlags = MSGQUEUE_ALLOW_BROKEN;
    m_oMsgQueueOptions.dwSize  = sizeof(MSGQUEUEOPTIONS);
#endif
}

/***************************************************************************************************/
/**
\brief  Destructor of CMsgQueue
*/
CMsgQueue::~CMsgQueue()
{
    if (m_bOpened || m_bCreated)
    {
        /* opened/created queue should have been closed with Close()! */
        OsDbgAssert(EC_FALSE);
        this->Close();
    }
}

/***************************************************************************************************/
/**
\brief  Opens an existing native msg queue. See also Close() to free the resources.
 * Message Queue Point-to-Point API uses CreateMsgQueue to create or open a user-defined message queue.
*/
EC_T_DWORD CMsgQueue::Open(EC_T_CHAR* szMsgQueueNameFormat, EC_T_BOOL bReadAccess, EC_T_DWORD dwMaxMessage, EC_T_DWORD dwMaxMessages, EC_T_DWORD dwId)
{
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    DWORD      dwLastError      = 0;
    EC_T_CHAR  szMsgQueueName[255];
    wchar_t    awcMsgQueueName[255];

    if ((EC_INVALID_QUEUE_ID == dwId) && (NULL != strstr(szMsgQueueNameFormat, "%d")))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (m_bOpened || m_bCreated)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    m_oMsgQueueOptions.cbMaxMessage  = dwMaxMessage;
    m_oMsgQueueOptions.dwMaxMessages = dwMaxMessages;
    m_oMsgQueueOptions.bReadAccess   = bReadAccess;

    sprintf(szMsgQueueName, szMsgQueueNameFormat, dwId);
    swprintf(awcMsgQueueName, L"%S", szMsgQueueName);

    m_hOsMsgQueue = CreateMsgQueue(awcMsgQueueName, &m_oMsgQueueOptions);
    dwLastError = ::GetLastError();
    if (NULL == m_hOsMsgQueue)
    {
        m_hOsMsgQueue = INVALID_HANDLE_VALUE;
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }

    /* check for error that queue was not found */
    if (ERROR_ALREADY_EXISTS != dwLastError)
    {
        CloseMsgQueue(m_hOsMsgQueue);
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }

    m_dwId = dwId;
    m_bOpened   = EC_TRUE;
    dwRetVal    = EC_E_NOERROR;
Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(szMsgQueueNameFormat);EC_UNREFPARM(bReadAccess);EC_UNREFPARM(dwMaxMessage);EC_UNREFPARM(dwMaxMessages);EC_UNREFPARM(dwId);

    OsDbgAssert(EC_FALSE);
    return EC_E_NOTSUPPORTED;
#endif /* !EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED */
}

/***************************************************************************************************/
/**
\brief  Creates a new native msg queue. See also Close() to free the resources.
 * EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED: provides a new unique ID if dwId is EC_INVALID_QUEUE_ID and szMsgQueueNameFormat contains "%d"
*/
EC_T_DWORD CMsgQueue::Create(EC_T_CHAR* szMsgQueueNameFormat, EC_T_BOOL bReadAccess, EC_T_DWORD dwMaxMessage, EC_T_DWORD dwMaxMessages, EC_T_DWORD dwId)
{
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    EC_T_DWORD dwRetVal         = EC_E_ERROR;
    EC_T_BOOL  bProvideNewId    = EC_FALSE;
    EC_T_DWORD dwFreeIdFound    = EC_INVALID_QUEUE_ID;
    DWORD      dwLastError      = 0;
    EC_T_CHAR  szMsgQueueName[255];
    wchar_t    awcMsgQueueName[255];

    if (m_bOpened || m_bCreated)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if ((EC_INVALID_QUEUE_ID == dwId) && (NULL != strstr(szMsgQueueNameFormat, "%d")))
    {
        bProvideNewId = EC_TRUE;
    }

    m_oMsgQueueOptions.cbMaxMessage  = dwMaxMessage;
    m_oMsgQueueOptions.dwMaxMessages = dwMaxMessages;

    if (bProvideNewId)
    {
        m_oMsgQueueOptions.bReadAccess = EC_FALSE;

        /* scan for a free ID */
        for (dwId = EC_LOGON_QUEUE_ID + 2; (EC_INVALID_QUEUE_ID == dwFreeIdFound) && (dwId <= EC_MAX_QUEUE_ID); dwId++)
        {
            sprintf(szMsgQueueName, szMsgQueueNameFormat, dwId);
            swprintf(awcMsgQueueName, L"%S", szMsgQueueName);
            m_hOsMsgQueue = CreateMsgQueue(awcMsgQueueName, &m_oMsgQueueOptions);
            dwLastError = ::GetLastError();
            if (NULL == m_hOsMsgQueue)
            {
                m_dwId = EC_INVALID_QUEUE_ID;
                dwRetVal = EC_E_OPENFAILED;
                goto Exit;
            }

            CloseMsgQueue(m_hOsMsgQueue);

            /* check if free ID was found */
            if (ERROR_ALREADY_EXISTS != dwLastError)
            {
                dwFreeIdFound = dwId;
            }
        }
        /* check for error that all IDs are already used */
        if (EC_INVALID_QUEUE_ID == dwFreeIdFound)
        {
            m_dwId = EC_INVALID_QUEUE_ID;
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        /* free ID was found */
        dwId = dwFreeIdFound;

        /* assume we got a valid queue id */
        OsDbgAssert(EC_INVALID_QUEUE_ID != dwId);
    }

    sprintf(szMsgQueueName, szMsgQueueNameFormat, dwId);
    swprintf(awcMsgQueueName, L"%S", szMsgQueueName);
    m_oMsgQueueOptions.bReadAccess = bReadAccess;

    m_hOsMsgQueue = CreateMsgQueue(awcMsgQueueName, &m_oMsgQueueOptions);
    dwLastError = ::GetLastError();
    if (NULL == m_hOsMsgQueue)
    {
        m_hOsMsgQueue = INVALID_HANDLE_VALUE;
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }
    /* check for error that queue already exists */
    if (ERROR_ALREADY_EXISTS == dwLastError)
    {
        CloseMsgQueue(m_hOsMsgQueue);
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }

    m_dwId = dwId;
    m_bCreated = EC_TRUE;
#if (defined DEBUG)
    OsSnprintf(m_szQueueName, 254, szMsgQueueNameFormat, dwId);
#endif
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
#else
    EC_UNREFPARM(szMsgQueueNameFormat);EC_UNREFPARM(bReadAccess);EC_UNREFPARM(dwMaxMessage);EC_UNREFPARM(dwMaxMessages);EC_UNREFPARM(dwId);

    OsDbgAssert(EC_FALSE);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Check if currently at least one reader and one writer has opened or created the queue.
*/
EC_T_BOOL CMsgQueue::IsConnected(EC_T_VOID)
{
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    EC_T_BOOL bRetVal = EC_FALSE;
    EC_T_BOOL bRes    = EC_FALSE;
    MSGQUEUEINFO MsgQueueInfo;

    if (INVALID_HANDLE_VALUE == m_hOsMsgQueue)
    {
        bRetVal = EC_FALSE;
        goto Exit;
    }

    bRes = GetMsgQueueInfo(m_hOsMsgQueue, &MsgQueueInfo);
    if (!bRes)
    {
        /* m_hOsMsgQueue is not INVALID_HANDLE_VALUE, so GetMsgQueueInfo must succeed. */
        OsDbgAssert(EC_FALSE);
        bRetVal = EC_FALSE;
        goto Exit;
    }

    bRetVal = (MsgQueueInfo.wNumReaders > 0) && (MsgQueueInfo.wNumWriters > 0);
Exit:
    return bRetVal;
#else
    return EC_FALSE;
#endif
}

/***************************************************************************************************/
/**
\brief  Closes an opened or created native msg queue. See also Open(...), Create(...).
*/
EC_T_DWORD CMsgQueue::Close(EC_T_VOID)
{
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    if (!m_bOpened && !m_bCreated)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (!CloseMsgQueue(m_hOsMsgQueue))
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_ERROR;
    }
    m_hOsMsgQueue = INVALID_HANDLE_VALUE;
    m_bOpened     = EC_FALSE;
    m_bCreated    = EC_FALSE;
    OsDbgAssert(EC_E_ERROR != dwRetVal);

Exit:
    return dwRetVal;
#else
    OsDbgAssert(EC_FALSE);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Reads a single msg.
*/
EC_T_DWORD CMsgQueue::Read(EC_T_BYTE* pbyBuffer, EC_T_DWORD* pdwNumberOfBytesRead, EC_T_DWORD dwTimeout)
{
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    DWORD dwFlags;
    DWORD dwRes;
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    OsDbgAssert(EC_TRUE == m_oMsgQueueOptions.bReadAccess);

    *pdwNumberOfBytesRead = 0;
    if (!ReadMsgQueue(m_hOsMsgQueue, pbyBuffer, m_oMsgQueueOptions.cbMaxMessage,
        (DWORD*)pdwNumberOfBytesRead, dwTimeout, &dwFlags))
    {
        dwRes = ::GetLastError();
        switch (dwRes)
        {
        case ERROR_INSUFFICIENT_BUFFER:
            dwRetVal = EC_E_INVALIDSIZE;
            break;
        case ERROR_PIPE_NOT_CONNECTED:
            dwRetVal = EC_E_INVALIDSTATE;
            break;
        case ERROR_TIMEOUT:
            dwRetVal = EC_E_TIMEOUT;
            break;
        }
    }
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
#else
    EC_UNREFPARM(pbyBuffer);
    EC_UNREFPARM(pdwNumberOfBytesRead);
    EC_UNREFPARM(dwTimeout);

    OsDbgAssert(EC_FALSE);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Writes a single msg.
*/
EC_T_DWORD CMsgQueue::Write(EC_T_BYTE* pbyBuffer, EC_T_DWORD dwDataSize, EC_T_DWORD dwTimeout)
{
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    DWORD      dwRes    = ERROR_SUCCESS;
    OsDbgAssert(EC_FALSE == m_oMsgQueueOptions.bReadAccess);

    if (!WriteMsgQueue(m_hOsMsgQueue, pbyBuffer, dwDataSize, dwTimeout, 0))
    {
        dwRes = ::GetLastError();
        switch (dwRes)
        {
        case ERROR_INSUFFICIENT_BUFFER:
            dwRetVal = EC_E_INVALIDSIZE;
            break;
        case ERROR_OUTOFMEMORY:
            dwRetVal = EC_E_NOMEMORY;
    	    break;
        case ERROR_PIPE_NOT_CONNECTED:
            dwRetVal = EC_E_INVALIDSTATE;
            break;
        case ERROR_TIMEOUT:
            dwRetVal = EC_E_TIMEOUT;
            break;
        }
    }
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
#else
    EC_UNREFPARM(pbyBuffer);
    EC_UNREFPARM(dwDataSize);
    EC_UNREFPARM(dwTimeout);

    OsDbgAssert(EC_FALSE);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Gets the maximum mgs size that can be sent/received.
*/
EC_T_DWORD CMsgQueue::GetMaxMsgSize(EC_T_VOID)
{
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    return m_oMsgQueueOptions.cbMaxMessage;
#else
    OsDbgAssert(EC_FALSE);
    return EC_E_NOTSUPPORTED;
#endif /* EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED */
}

/***************************************************************************************************/
/**
\brief  Gets the currently connected reader count.
*/
EC_T_DWORD CMsgQueue::GetReaderCount(EC_T_DWORD* pdwReaderCount)
{
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    MSGQUEUEINFO oInfo;
    if (!GetMsgQueueInfo(m_hOsMsgQueue, &oInfo))
    {
        return EC_E_INVALIDSTATE;
    }
    *pdwReaderCount = oInfo.wNumReaders;
    return EC_E_NOERROR;
#else
    EC_UNREFPARM(pdwReaderCount);
    OsDbgAssert(EC_FALSE);
    return EC_E_NOTSUPPORTED;
#endif
}

/***************************************************************************************************/
/**
\brief  Gets the currently connected writer count.
*/
EC_T_DWORD CMsgQueue::GetWriterCount(EC_T_DWORD* pdwWriterCount)
{
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    MSGQUEUEINFO oInfo;
    if (!GetMsgQueueInfo(m_hOsMsgQueue, &oInfo))
    {
        return EC_E_INVALIDSTATE;
    }
    *pdwWriterCount = oInfo.wNumWriters;
    return EC_E_NOERROR;
#else
    EC_UNREFPARM(pdwWriterCount);
    OsDbgAssert(EC_FALSE);
    return EC_E_NOTSUPPORTED;
#endif
}

#endif /* EC_SOCKET_MSGQUEUE_SUPPORTED */

/***************************************************************************************************/
/**
\brief  Constructor of CEcSocket
*/
CEcSocket::CEcSocket()
{
    m_bConnected    = EC_FALSE;
    m_bBound        = EC_FALSE;
    m_bDisc         = EC_TRUE;
    m_eType         = emrassocktype_unknown;
    m_dwAddr		= 0;
    m_wPort			= 0;
}

/***************************************************************************************************/
/**
\brief  Shutdown open socket connection.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocket::Disconnect(
    EC_T_BOOL bRespawn  /**< [in]   if EC_TRUE a new socket is created after Disconnect */
    )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    if (m_bConnected)
    {
        dwRes = Shutdown();
        if (EC_E_ERROR == dwRes)
        {
            dwRetVal = EC_E_ACCESSDENIED; /* for compatibility TODO: remove */
            goto Exit;
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    Close();

    if (bRespawn && (EC_E_NOERROR == dwRetVal))
    {
        /* after shutdown we need to re-create a new socket ! */
        dwRes = Respawn();
        dwRetVal = dwRes;
    }

    m_bDisc = EC_TRUE;
    m_bConnected = EC_FALSE;

    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

#if (defined EC_SOCKET_IP_SUPPORTED)
/***************************************************************************************************/
/**
\brief  Default Constructor
*/
CEcSocketIp::CEcSocketIp(
     EC_T_SOCKTYPE eType     /**< [in]   Type of socket */)
{
    m_pSockHandle   = EC_INVALID_SOCKET;
    m_bDisc         = EC_TRUE;
    m_bBound        = EC_FALSE;
    m_eType         = eType;


    OsMemset(&m_oSockAddr, 0, sizeof(EC_T_SOCKADDR_IN));

    switch( m_eType )
    {
    case emrassocktype_tcp:
        {
        EC_T_INT nRes    = 0;
        EC_T_INT nEnable = 1;

            m_pSockHandle = OsSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#if (defined RTOS_32)
            nRes = ioctlsocket(m_pSockHandle, FIONBIO, (DWORD*)&nEnable);
            OsDbgAssert(0 == nRes);
            nEnable = 0;
            nRes = OsSocketSetSockOpt(m_pSockHandle, SOL_SOCKET, SO_NAGLE, (EC_T_CHAR*)&nEnable, sizeof(EC_T_INT));
#else
            nRes = OsSocketSetSockOpt(m_pSockHandle, IPPROTO_TCP, TCP_NODELAY, (EC_T_CHAR*)&nEnable, sizeof(EC_T_INT));
#endif
            if (0 != nRes)
            {
            	OsDbgAssert(EC_FALSE);
            }
        } break;
    case emrassocktype_udp:
        {
            m_pSockHandle = OsSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        } break;
    default:
        {
            OsDbgAssert(EC_FALSE);
            goto Exit;
        } break;
    }

    switch( m_eType )
    {
    case emrassocktype_tcp:
    case emrassocktype_udp:
        {
            if( EC_INVALID_SOCKET == m_pSockHandle )
            {
                goto Exit;
            }

#if (defined SO_DONTLINGER)
            {
                EC_T_INT    nEnable     = EC_TRUE;

                OsSocketSetSockOpt( m_pSockHandle,      /* socket affected */
                    SOL_SOCKET,         /* set option at Socket level */
                    SO_DONTLINGER,      /* name of option */
                    (char *) &nEnable,  /* the cast is historical cruft */
                    sizeof(EC_T_INT)    /* length of option value */
                    );
            }
#endif
        } break;

    default:
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }
Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Spawn Constructor.

Used for a spawned socket instance.
*/
CEcSocketIp::CEcSocketIp(
    EC_T_SOCKTYPE          eType,
    EC_T_SOCKET            pSock,
    EC_T_SOCKADDR_IN       oAddr
                               )
{
    m_eType           = eType;
    m_pSockHandle     = pSock;

    OsMemcpy(&m_oSockAddr, &oAddr, sizeof(m_oSockAddr));

    OsMemcpy(&m_dwAddr, &m_oSockAddr.sin_addr, EC_MIN(sizeof(EC_T_DWORD), sizeof(m_oSockAddr)));
    m_wPort = m_oSockAddr.sin_port;

    if( EC_INVALID_SOCKET == m_pSockHandle )
    {
        OsDbgAssert(EC_FALSE);
    }

    m_bDisc      = EC_FALSE;
    m_bConnected = EC_TRUE;
    m_bBound     = EC_TRUE;
}

/***************************************************************************************************/
/**
\brief  Destructor of CEcSocketIp
*/
CEcSocketIp::~CEcSocketIp()
{
    if( EC_INVALID_SOCKET != m_pSockHandle )
    {
        Disconnect(EC_FALSE);
    }

    if( EC_INVALID_SOCKET != m_pSockHandle )
    {
        OsSocketCloseSocket(m_pSockHandle);
    }
}

/***************************************************************************************************/
/**
\brief  Initialize Winsock.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketIp::InitSocketLayer(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    if (s_dwInitSocketLayerCount == 0)
    {
        dwRes = OsSocketInit();
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    s_dwInitSocketLayerCount++;

    dwRetVal = EC_E_NOERROR;

Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  De-Initialize Winsock.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketIp::DeInitSocketLayer(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes = EC_E_ERROR;

    if (s_dwInitSocketLayerCount == 0)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    s_dwInitSocketLayerCount--;

    if (s_dwInitSocketLayerCount == 0)
    {
        dwRes = OsSocketDeInit();
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Bind Socket to specific port.
This function is called by a TCP Server, or a UDP Receiver
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketIp::Bind(
    EC_T_IPADDR*  pAddr,          /**< [in]   IP Address to bind to */
    EC_T_WORD     wPort           /**< [in]   Port Nr to bind to */
                                      )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

#if (defined LINUX)
    int on = 1;
#endif /* LINUX */

    if( EC_NULL == pAddr )
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if( EC_INVALID_SOCKET == m_pSockHandle )
    {
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }

#if (defined LINUX)
    setsockopt(m_pSockHandle, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
#endif /* LINUX */

    m_oSockAddr.sin_family = AF_INET;
    m_oSockAddr.sin_port   = OsHTONS(wPort);
#if (defined EC_VERSION_SYSBIOS)
    EC_BYTEN((&(m_oSockAddr.sin_addr)), 3) = EC_BYTE0(((EC_T_DWORD)pAddr->dwAddr));
    EC_BYTEN((&(m_oSockAddr.sin_addr)), 2) = EC_BYTE1(((EC_T_DWORD)pAddr->dwAddr));
    EC_BYTEN((&(m_oSockAddr.sin_addr)), 1) = EC_BYTE2(((EC_T_DWORD)pAddr->dwAddr));
    EC_BYTEN((&(m_oSockAddr.sin_addr)), 0) = EC_BYTE3(((EC_T_DWORD)pAddr->dwAddr));
#else
    EC_SETDWORD(&(m_oSockAddr.sin_addr), pAddr->dwAddr);
#endif

    if ((int)EC_SOCKET_ERROR == (int)OsSocketBind(m_pSockHandle, (EC_T_SOCKADDR*)&m_oSockAddr, sizeof(m_oSockAddr)))
    {
        dwRetVal = EC_E_ACCESSDENIED;
        goto Exit;
    }

    m_bBound = EC_TRUE;
    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Listen for incoming connections.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketIp::Listen(EC_T_VOID)
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    if (emrassocktype_tcp != m_eType)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (EC_INVALID_SOCKET == m_pSockHandle)
    {
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }

    if (EC_SOCKET_ERROR == OsSocketListen(m_pSockHandle, SOMAXCONN))
    {
        dwRetVal = EC_E_ACCESSDENIED;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Accept connection.

This function waits for given amount of time for a connection from a client. If time expires without
connection EC_E_TIMEOUT is returned.
\return EC_E_NOERROR on new connection, EC_E_TIMEOUT if no new connection, error code otherwise.
*/
EC_T_DWORD CEcSocketIp::Accept(
    CEcSocket**    ppoNewSocket,      /**< [out]  New generated socket */
    EC_T_DWORD     dwTimeout          /**< [in]   Timeout to wait for a socket connection in msecs, cannot be INFINITE! */
                                        )
{
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;
    EC_T_FD_SET         fdSet;
    EC_T_TIMEVAL        tvTimeout;
    EC_T_SOCKADDR_IN    oSpAddr;
    EC_T_SOCKET         pNewSockHandle  = EC_INVALID_SOCKET;
    EC_T_INT            nAddrsize;
    EC_T_INT            selectRetVal;

    if (EC_NULL == ppoNewSocket)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    (*ppoNewSocket) = EC_NULL;

    EC_FD_ZERO(&fdSet);

    if (EC_INVALID_SOCKET == m_pSockHandle)
    {
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }
#if (defined WIN32)
#pragma warning(disable:4127 4018 4389)
#endif
    EC_FD_SET(m_pSockHandle, &fdSet);
#if (defined WIN32)
#pragma warning(default:4127 4018 4389)
#endif

    tvTimeout.tv_sec    = dwTimeout/1000;
    tvTimeout.tv_usec   = (dwTimeout%1000)*1000;

    selectRetVal = OsSocketSelect((int)(m_pSockHandle+1), &fdSet, EC_NULL, EC_NULL, &tvTimeout);
    if (EC_SOCKET_ERROR == selectRetVal)
    {
        dwRetVal = EC_E_ACCESSDENIED;
        goto Exit;
    }

    if (0 == selectRetVal)
    {
        /* no new connection */
        dwRetVal = EC_E_TIMEOUT;
        goto Exit;
    }

    if (EC_INVALID_SOCKET == m_pSockHandle)
    {
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }

    nAddrsize = sizeof(oSpAddr);
    pNewSockHandle = OsSocketAccept(m_pSockHandle, (EC_T_SOCKADDR*)&oSpAddr, &nAddrsize);

    if (EC_INVALID_SOCKET == pNewSockHandle)
    {
        dwRetVal = EC_E_ACCESSDENIED;
        goto Exit;
    }

    (*ppoNewSocket) = EC_NEW(CEcSocketIp(m_eType, pNewSockHandle, oSpAddr));
    /* EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CEcSocketIp::Accept", (*ppoNewSocket), sizeof(CEcSocketIp)); */

    if (EC_NULL == (*ppoNewSocket))
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Connect to a server port.

Only used by Client application, depending on socket Type (TCP/UDP) the behavior is different.
TCP: a connection to the server is established actively.
UDP: the connection data is stored internally, but no connection is established since UDP is a
connectionless protocol.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketIp::Connect(
    EC_T_IPADDR*   pAddr,      /**< [in]   Server IP Address */
    EC_T_WORD      wPort       /**< [in]   Server Port value */
    )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (EC_NULL == pAddr)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    m_oSockAddr.sin_family = AF_INET;
    EC_SETDWORD(&(m_oSockAddr.sin_addr), pAddr->dwAddr);
    m_oSockAddr.sin_port = OsHTONS(wPort);

    switch (m_eType)
    {
    case emrassocktype_tcp:
        {
            /* establish connection */
            if (EC_INVALID_SOCKET == m_pSockHandle)
            {
                dwRetVal = EC_E_SOCKET_DISCONNECTED;
                goto Exit;
            }

            if (EC_SOCKET_ERROR == OsSocketConnect(m_pSockHandle, (EC_T_SOCKADDR*)&m_oSockAddr, sizeof(m_oSockAddr)))
            {
                dwRetVal = EC_E_ACCESSDENIED;
                goto Exit;
            }
        } break;

    case emrassocktype_unknown:
    default:
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    m_bConnected = EC_TRUE;
    m_bBound     = EC_TRUE;
    m_bDisc      = EC_FALSE;

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Disables sends or receives.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketIp::Shutdown(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_INT nRes       = EC_SOCKET_ERROR;

    if (EC_INVALID_SOCKET == m_pSockHandle)
    {
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }

    nRes = OsSocketShutdown(m_pSockHandle, SD_BOTH);
    switch (nRes)
    {
    case EC_SOCKET_NOERROR:
        break;
    case EC_SOCKET_ERROR:
        dwRetVal = EC_E_SOCKET_DISCONNECTED;

        nRes = OsSocketGetLastError();
        switch (nRes)
        {
        case 0:
#if (defined EC_E_BSD_EINVAL)
        /* accept partner initiating socket shutdown in parallel */
        case EC_E_BSD_EINVAL:
#endif
#if (defined EC_E_BSD_ENOTCONN)
        case EC_E_BSD_ENOTCONN:
#endif
#if (defined EC_E_BSD_ECONNRESET)
        case EC_E_BSD_ECONNRESET:
#endif
#if (defined EC_E_BSD_EPIPE)
        case EC_E_BSD_EPIPE:
#endif
            break;
        default:
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_ERROR;
            break;
        }

        goto Exit;
        break;
    default:
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_ERROR;
        goto Exit;
        break;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Closes an existing socket.
*/
EC_T_DWORD CEcSocketIp::Close(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    if (EC_INVALID_SOCKET == m_pSockHandle)
    {
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }

    dwRes = OsSocketCloseSocket(m_pSockHandle);
    dwRetVal = dwRes;
    m_pSockHandle = EC_INVALID_SOCKET;

Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get a new socket handler for re-usage.
*/
EC_T_DWORD CEcSocketIp::Respawn(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    switch (m_eType)
    {
    case emrassocktype_tcp:
        {
            m_pSockHandle = OsSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        } break;
    case emrassocktype_udp:
        {
            m_pSockHandle = OsSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        } break;
    default:
        {
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        } break;
    }

    if (EC_INVALID_SOCKET == m_pSockHandle)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Read data from socket.

Does not support peek!
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketIp::RecvData(
        EC_T_PBYTE      pbyData,
        EC_T_DWORD*     pdwLen,            /**< [in, out]  Size of data Buffer / used buffer size */
        EC_T_DWORD      dwTimeout)
{
    return this->RecvData(pbyData, pdwLen, dwTimeout, EC_NULL, EC_NULL);
}
EC_T_DWORD CEcSocketIp::RecvData(
    EC_T_PBYTE     pbyData,          /**< [in, out]  Data buffer for recv data */
    EC_T_DWORD*    pdwLen,           /**< [in, out]  Size of data buffer / used buffer size */
    EC_T_DWORD     dwTimeout,        /**< [in]       Time-out to wait for recv Data */
    EC_T_SOCKADDR* poSrcAddress,     /**< [out]      Source IP address   */
    EC_T_DWORD*    pdwSrcAddressLen  /**< [out]      Source IP address length */
                                    )
{
EC_T_DWORD   dwRetVal        = EC_E_ERROR;
EC_T_DWORD   dwRes           = EC_E_ERROR;
EC_T_DWORD   dwCurRx         = 0;
EC_T_DWORD   dwReceivedBytes = 0;
CEcTimer oTimeOut;
EC_T_FD_SET  fdSetRead;
EC_T_FD_SET  fdSetExcept;
EC_T_TIMEVAL tvTimeout;
EC_T_INT     nRes   = 0;

    if ((EC_NULL == pdwLen) || (EC_NULL == pbyData) || (0 == *pdwLen)
        || (EC_NOWAIT == dwTimeout) || (EC_WAITINFINITE == dwTimeout))
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    OsDbgAssert((emrassocktype_tcp == m_eType) || (emrassocktype_udp == m_eType));

    oTimeOut.Start(dwTimeout);
    do
    {
        if (EC_INVALID_SOCKET == m_pSockHandle)
        {
            m_bDisc = EC_TRUE;
            dwRetVal = EC_E_SOCKET_DISCONNECTED;
            goto Exit;
        }

        EC_FD_ZERO(&fdSetRead);
        EC_FD_ZERO(&fdSetExcept);

        /* Macro tells expression is constant, avoid this by disable this warning temporary */
#if (defined WIN32)
#pragma warning(disable:4127 4018 4389)
#endif
        EC_FD_SET(m_pSockHandle, &fdSetRead);
        EC_FD_SET(m_pSockHandle, &fdSetExcept);
#if (defined WIN32)
#pragma warning(default:4127 4018 4389)
#endif
        tvTimeout.tv_sec    = dwTimeout / 1000;
        tvTimeout.tv_usec   = (dwTimeout % 1000) * 1000;

        /* wait for received data if needed */
        nRes = OsSocketSelect((int)(m_pSockHandle + 1), &fdSetRead, EC_NULL, &fdSetExcept, &tvTimeout);
        if (EC_SOCKET_ERROR == nRes)
        {
            dwRetVal = EC_E_SOCKET_DISCONNECTED;
            goto Exit;
        }

        /* if the OsSocketSelect call returns with 0 the timeout has expired */
        if (0 == nRes)
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }

        if (EC_INVALID_SOCKET == m_pSockHandle)
        {
            m_bDisc = EC_TRUE;
            dwRetVal = EC_E_SOCKET_DISCONNECTED;
            goto Exit;
        }

        /* socket caused an error? */
        if (EC_FD_ISSET(m_pSockHandle, &fdSetExcept))
        {
            dwRes = OsSocketGetLastError();
            OsDbgAssert(EC_E_BSD_EMSGSIZE != dwRes);
            dwRetVal = EC_E_SOCKET_DISCONNECTED;
            m_bDisc = EC_TRUE;
            goto Exit;
        }

        /* no error reported, timeout not elapsed? */
        if (!EC_FD_ISSET(m_pSockHandle, &fdSetRead))
        {
            /* socket has no new data */
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }

        if (poSrcAddress != EC_NULL && pdwSrcAddressLen != EC_NULL)
        {
            dwCurRx = OsSocketRecvFrom(m_pSockHandle, (EC_T_CHAR*)(pbyData+dwReceivedBytes), (EC_T_INT)((*pdwLen)-dwReceivedBytes), 0,poSrcAddress, (EC_T_INT*)pdwSrcAddressLen);
        }
        else
        {
            dwCurRx = OsSocketRecv(m_pSockHandle, (EC_T_CHAR*)(pbyData+dwReceivedBytes), (EC_T_INT)((*pdwLen)-dwReceivedBytes), 0);
        }

        if (((EC_SOCKET_ERROR == (EC_T_INT)dwCurRx) || ((0 == dwCurRx) && (m_eType == emrassocktype_tcp))))
        {
            dwRes = OsSocketGetLastError();
            OsDbgAssert(EC_E_BSD_EMSGSIZE != dwRes);
            switch(dwRes)
            {
            case EC_SOCKET_NOERROR:
                /* don't change socket state */
                break;
            case EC_E_BSD_ENOTCONN:
            case EC_E_BSD_ENOTSOCK:
            case EC_E_BSD_ESHUTDOWN:
            case EC_E_BSD_EINVAL:
#if (defined EC_E_BSD_ECONNABORTED)
            case EC_E_BSD_ECONNABORTED:
#endif
            case EC_E_BSD_ETIMEDOUT:
#if (defined EC_E_BSD_ECONNRESET)
            case EC_E_BSD_ECONNRESET:
#endif
#if (defined EC_E_BSD_EPIPE)
            case EC_E_BSD_EPIPE:
#endif
                m_bDisc = EC_TRUE;
                break;
            default:
                /* should never happen */
                OsDbgAssert(EC_FALSE);
                break;
            }

            dwRetVal = EC_E_SOCKET_DISCONNECTED;
            goto Exit;
        }

        /* increment rx bytes count about the read chunk */
        dwReceivedBytes += dwCurRx;
    }
    while ((dwReceivedBytes < *pdwLen) && !oTimeOut.IsElapsed() && (m_eType == emrassocktype_tcp));

    dwRetVal = EC_E_NOERROR;
Exit:
    if (EC_NULL != pdwLen)
    {
        *pdwLen = dwReceivedBytes;
    }
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Transmit data to opponent.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketIp::SendData(
        EC_T_PBYTE      pbyData,
        EC_T_DWORD      dwLen)
{
    return this->SendData(pbyData, dwLen, EC_NULL, EC_NULL);
}
EC_T_DWORD CEcSocketIp::SendData(
                                    EC_T_PBYTE pbyData,          /**< [in]   Data Pointer */
                                    EC_T_DWORD dwLen,            /**< [in]   Size of data */
                                    EC_T_SOCKADDR* poDstAddress, /**< [in]   Destination IP address */
                                    EC_T_DWORD dwDstAddressLen   /**< [in]   Destination IP address length */
                                    )
{
    EC_T_DWORD dwRetVal    = EC_E_ERROR;
    EC_T_DWORD dwSendBytes = 0;
    EC_T_DWORD dwSentBytes = 0;

    if (0 == dwLen)
    {
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }
    if (EC_NULL == pbyData)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (EC_INVALID_SOCKET == m_pSockHandle)
    {
        m_bDisc = EC_TRUE;
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }

    for (dwSendBytes = dwLen; dwSendBytes > 0; dwSendBytes = dwLen - dwSentBytes)
    {
        EC_T_INT nSentBytesCur = 0;

        switch (m_eType)
        {
        case emrassocktype_tcp:
        {
            nSentBytesCur = OsSocketSend(m_pSockHandle, (EC_T_CHAR*)&pbyData[dwSentBytes], dwSendBytes, 0);
        } break;
        case emrassocktype_udp:
        {
            nSentBytesCur = OsSocketSendTo(m_pSockHandle, (EC_T_CHAR*)&pbyData[dwSentBytes], dwSendBytes, 0, poDstAddress, dwDstAddressLen);
        } break;
        default:
        {
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        } break;
        }

        if (-1 == nSentBytesCur)
        {
#if (defined EC_E_BSD_EINPROGRESS)
            if (EC_E_BSD_EINPROGRESS == OsSocketGetLastError())
            {
                OsSleep(1);                
            }
            else
#endif
            {
                m_bDisc = EC_TRUE;
                dwRetVal = EC_E_SOCKET_DISCONNECTED;
                goto Exit;
            }
        }
        else
        {
            dwSentBytes = dwSentBytes + (EC_T_DWORD)nSentBytesCur;
        }
    } 

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}
#endif /* EC_SOCKET_IP_SUPPORTED */

#if (defined EC_SOCKET_MSGQUEUE_SUPPORTED)
/***************************************************************************************************/
/**
\brief  Constructor of CEcSocketMsgQueue
*/
CEcSocketMsgQueue::CEcSocketMsgQueue()
{
#if defined(EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    m_eType = emrassocktype_msg;
#else
    m_eType = emrassocktype_unknown;
#endif
    m_poLogonQueue = EC_NULL;
    m_poRxMsgQueue = EC_NULL;
    m_poTxMsgQueue = EC_NULL;
    OsMemset(&m_RxBuffer, 0, sizeof(EC_T_MSGQUEUE_MSG));
    m_dwRxLen = 0;
    // m_dwAddr = (127 << 24) + (0 << 16) + (0 << 8) + (1 << 0);
    m_dwAddr = 0;
    m_wPort = 0;
}

/***************************************************************************************************/
/**
\brief  Destructor of CEcSocketMsgQueue
*/
CEcSocketMsgQueue::~CEcSocketMsgQueue()
{
    if( EC_NULL != m_poLogonQueue )
    {
        // EC_TRACE_SUBMEM(EC_MEMTRACE_RAS_MSGQUEUE, "CAtemRasServer::~CAtemRasServer m_hLogonQueueRxHandle", m_hLogonQueueRxHandle);
        SafeDelete(m_poLogonQueue);
    }

    if (EC_NULL != m_poRxMsgQueue)
    {
        SafeDelete(m_poRxMsgQueue);
    }

    if (EC_NULL != m_poTxMsgQueue)
    {
        SafeDelete(m_poTxMsgQueue);
    }
}

/***************************************************************************************************/
/**
\brief  Accept connection.

This function waits for given amount of time for a connection from a client. If time expires without
connection EC_E_TIMEOUT is returned.
\return EC_E_NOERROR on new connection, EC_E_TIMEOUT if no new connection, error code otherwise.
*/
EC_T_DWORD CEcSocketMsgQueue::Accept(CEcSocket** ppoNewSocket, EC_T_DWORD dwTimeout)
{
#if (defined EC_SOCKET_MSGQUEUE_SUPPORTED)
    EC_T_DWORD dwRetVal             = EC_E_ERROR;
    EC_T_DWORD dwRes                = EC_E_ERROR;
    EC_T_DWORD dwQueueId            = 0;
    EC_T_DWORD dwBufferSize         = MSGQUEUE_MSG_SIZE;
    CEcSocketMsgQueue*  poNewSocket = EC_NULL;
    EC_T_MSGQUEUE_MSG          oMessage;

    if (EC_NULL == ppoNewSocket)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    (*ppoNewSocket) = EC_NULL;

    if (EC_NULL == m_poLogonQueue)
    {
        /* Listen() was not called before! */
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* read dwQueueId from m_poLogonQueue */
    dwRes = m_poLogonQueue->Read(oMessage.abyBuffer, &dwBufferSize, dwTimeout);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    if (dwBufferSize != sizeof(EC_T_DWORD))
    {
        dwRetVal = EC_E_INVALIDDATA;
        goto Exit;
    }
    dwQueueId = EC_GET_FRM_DWORD(oMessage.abyBuffer);

    /* create new socket */
    switch (m_eType)
    {
#if defined(EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    case emrassocktype_msg:
        {
            poNewSocket = new CEcSocketMsgQueue();
        } break;
#endif /* EC_SOCKET_MSGQUEUE_SUPPORTED */
#if defined(EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED)
    case emrassocktype_rtosshm:
        {
            poNewSocket = new CEcSocketRtosShm();
        } break;
#endif /* EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED */
    default:
        {
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_ERROR;
            goto Exit;
        } break;
    }
    if (EC_NULL == poNewSocket)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRes = poNewSocket->OpenMsgQueues(dwQueueId);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRes = poNewSocket->SendData(oMessage.abyBuffer, sizeof(EC_T_DWORD));
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    *ppoNewSocket = poNewSocket;
    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
#else
    EC_UNREFPARM(ppoNewSocket);EC_UNREFPARM(dwTimeout);
    OsDbgAssert(EC_FALSE);
    return EC_E_NOTSUPPORTED;
#endif /* EC_SOCKET_MSGQUEUE_SUPPORTED */
}

/***************************************************************************************************/
/**
\brief  Connect to the server.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketMsgQueue::Connect(EC_T_IPADDR* pAddr, EC_T_WORD wPort)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
    EC_T_DWORD dwLen    = 0;
    EC_T_BOOL  bLogonQueueOpened = EC_FALSE;
    EC_T_MSGQUEUE_MSG  oMessage;

    EC_UNREFPARM(pAddr);EC_UNREFPARM(wPort);

    dwRes = OpenLogonQueue();
    if( EC_E_NOERROR != dwRes )
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    bLogonQueueOpened = EC_TRUE;

    dwRes = CreateCommunicationQueues();
    if( EC_E_NOERROR != dwRes )
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    m_bDisc = EC_FALSE;

    /* send tx queue id to server */
    EC_SET_FRM_DWORD(oMessage.abyBuffer, m_poTxMsgQueue->GetId());
    dwRes = m_poLogonQueue->Write(oMessage.abyBuffer, sizeof(EC_T_DWORD), EC_SOCKET_MSGQUEUE_SENT_TIMEOUT);
    if( EC_E_NOERROR != dwRes )
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    /* wait for answer from server */
    dwLen = sizeof(EC_T_DWORD);
    dwRes = RecvData(oMessage.abyBuffer, &dwLen, EC_SOCKET_INIT_RECV_TIMEOUT);
    if( EC_E_NOERROR != dwRes )
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    /* check answer from server */
    if ((sizeof(EC_T_DWORD) != dwLen) || (EC_GET_FRM_DWORD(oMessage.abyBuffer) != m_poTxMsgQueue->GetId()))
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDDATA;
        goto Exit;
    }

    OsDbgAssert(m_bDisc == EC_FALSE);
    m_bConnected = EC_TRUE;

    dwRetVal = EC_E_NOERROR;
Exit:
    if (bLogonQueueOpened)
    {
        dwRes = m_poLogonQueue->Close();
        if( EC_E_NOERROR != dwRes )
        {
            dwRetVal = dwRes;
        }
    }

    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Disables sends or receives.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketMsgQueue::Shutdown(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
    EC_T_DWORD dwReaderCount = 1;
    EC_T_DWORD dwDisconnectData = 0xdead;
    CEcTimer oDisconnectTimeout;
    if (/*m_bDisc ||*/ (EC_NULL == m_poTxMsgQueue) || (EC_NULL == m_poRxMsgQueue)
        || (m_poTxMsgQueue->GetId() == EC_INVALID_QUEUE_ID) || (m_poRxMsgQueue->GetId() == EC_INVALID_QUEUE_ID))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    m_bDisc = EC_TRUE;
    oDisconnectTimeout.Start(EC_SOCKET_DISCONNECT_ACK_TIMEOUT);

    dwRes = m_poRxMsgQueue->Close();
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    dwRes = m_poTxMsgQueue->Write((EC_T_BYTE*)&dwDisconnectData, sizeof(EC_T_DWORD), EC_SOCKET_DISCONNECT_ACK_TIMEOUT);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    while ((dwReaderCount > 0) && !oDisconnectTimeout.IsElapsed())
    {
        dwRes = m_poTxMsgQueue->GetReaderCount(&dwReaderCount);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }

        if (0 == dwReaderCount)
        {
            break;
        }

        OsSleep(10);
    }
    if (oDisconnectTimeout.IsElapsed())
    {
        dwRetVal = EC_E_TIMEOUT;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    /* if (m_bDisc)
    {
        m_poTxMsgQueue->Close();
        SafeDelete(m_poTxMsgQueue);
        SafeDelete(m_poRxMsgQueue);
        m_bConnected = EC_FALSE;
    } */

    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Close the socket
*/
EC_T_DWORD CEcSocketMsgQueue::Close(EC_T_VOID)
{
    EC_T_DWORD  dwRetVal    = EC_E_NOERROR;
    EC_T_DWORD  dwRes       = EC_E_ERROR;
    EC_T_BOOL   bAnyQueueClosed = EC_FALSE;
    if ((EC_NULL != m_poTxMsgQueue) && (m_poTxMsgQueue->GetId() != EC_INVALID_QUEUE_ID))
    {
        dwRes = m_poTxMsgQueue->Close();
        bAnyQueueClosed = EC_TRUE;
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
        }
    }
    if ((EC_NULL != m_poRxMsgQueue) && (m_poRxMsgQueue->GetId() != EC_INVALID_QUEUE_ID))
    {
        dwRes = m_poRxMsgQueue->Close();
        bAnyQueueClosed = EC_TRUE;
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
        }
    }
    if ((EC_NULL != m_poLogonQueue) && (m_poLogonQueue->GetId() != EC_INVALID_QUEUE_ID))
    {
        dwRes = m_poLogonQueue->Close();
        bAnyQueueClosed = EC_TRUE;
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
        }
    }
    if (!bAnyQueueClosed)
    {
        dwRetVal = EC_E_INVALIDSTATE;
    }

    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Read data from socket.

Does not support peek!
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketMsgQueue::RecvData(EC_T_PBYTE pbyData, EC_T_DWORD* pdwLen, EC_T_DWORD dwTimeout)
{
EC_T_DWORD   dwRetVal        = EC_E_ERROR;
EC_T_DWORD   dwCurRx         = 0;
EC_T_DWORD   dwReceivedBytes = 0;
CEcTimer oTimeOut;

EC_T_DWORD  dwRes            = EC_E_ERROR;

    if ((EC_NULL == pdwLen) || (EC_NULL == pbyData) || (0 == *pdwLen)
        || (EC_NOWAIT == dwTimeout) || (EC_WAITINFINITE == dwTimeout))
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    oTimeOut.Start(dwTimeout);
    do
    {
        if ((EC_NULL == m_poRxMsgQueue) || (m_poRxMsgQueue->GetId() == EC_INVALID_QUEUE_ID))
        {
            m_bDisc = EC_TRUE;
            dwRetVal = EC_E_SOCKET_DISCONNECTED;
            goto Exit;
        }
        /* was m_RxBuffer completely read? */
        if (m_dwRxLen == m_RxBuffer.dwLen)
        {
            m_RxBuffer.dwLen = m_dwRxLen = sizeof(m_RxBuffer.abyBuffer);
            dwRes = m_poRxMsgQueue->Read(m_RxBuffer.abyBuffer, &m_RxBuffer.dwLen, dwTimeout);
            /* socket disconnected if errors on receive */
            switch (dwRes)
            {
            case EC_E_NOERROR:
                break;
            case EC_E_TIMEOUT:
                {
                    m_RxBuffer.dwLen = m_dwRxLen;
                    dwRetVal = dwRes;
                    goto Exit;
                } break;
            case EC_E_INVALIDSTATE:
                {
                    m_RxBuffer.dwLen = m_dwRxLen;
                    dwRetVal = EC_E_SOCKET_DISCONNECTED;
                    goto Exit;
                } break;
            default:
                {
                    /* add case for return code on assert */
                    OsDbgAssert(EC_FALSE);
                    m_RxBuffer.dwLen = m_dwRxLen;
                    m_bDisc = EC_TRUE;
                    dwRetVal = EC_E_SOCKET_DISCONNECTED;
                    goto Exit;
                } break;
            }

            /* buffer filled, rewind head */
            m_dwRxLen = 0;
        }

        dwCurRx = EC_MIN((*pdwLen) - dwReceivedBytes, m_RxBuffer.dwLen - m_dwRxLen);
        OsMemcpy(&pbyData[dwReceivedBytes], &m_RxBuffer.abyBuffer[m_dwRxLen], dwCurRx);
        m_dwRxLen       += dwCurRx;
        dwReceivedBytes += dwCurRx;
    }
    while ((dwReceivedBytes < *pdwLen) && !oTimeOut.IsElapsed());

    dwRetVal = EC_E_NOERROR;
Exit:
    if (EC_NULL != pdwLen)
    {
        *pdwLen = dwReceivedBytes;
    }
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Transmit data to opponent.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSocketMsgQueue::SendData(EC_T_PBYTE pbyData, EC_T_DWORD dwLen)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    if ((EC_NULL == pbyData) || (0 == dwLen))
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if ((EC_NULL == m_poTxMsgQueue) || (m_poTxMsgQueue->GetId() == EC_INVALID_QUEUE_ID))
    {
        dwRetVal    = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }

    while (dwLen > m_poTxMsgQueue->GetMaxMsgSize())
    {
        dwRes = m_poTxMsgQueue->Write(pbyData, m_poTxMsgQueue->GetMaxMsgSize(), EC_SOCKET_MSGQUEUE_SENT_TIMEOUT);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
        dwLen = dwLen - m_poTxMsgQueue->GetMaxMsgSize();
        pbyData = pbyData + m_poTxMsgQueue->GetMaxMsgSize();
    }
    dwRes = m_poTxMsgQueue->Write(pbyData, dwLen, EC_SOCKET_MSGQUEUE_SENT_TIMEOUT);
    if (EC_E_NOERROR != dwRes)
    {
        m_bDisc = EC_TRUE;
        dwRetVal = dwRes;
        goto Exit;
    }

    m_bConnected = EC_TRUE;

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Creates new communication queues. Determines queue IDs.
\return EC_E_NOERROR on success
\return EC_E_INVALIDSTATE if memory for communication queue(s) has already been allocated.
\return EC_E_NOMEMORY if RAM or connections exceeded.
\return Passes return code of CMsgQueue::Create on error.
*/
EC_T_DWORD CEcSocketMsgQueue::CreateCommunicationQueues(EC_T_VOID)
{
    EC_T_DWORD dwRetVal           = EC_E_ERROR;
    EC_T_DWORD dwRes              = EC_E_ERROR;
    EC_T_BOOL  bTxMsgQueueCreated = EC_FALSE;
    EC_T_BOOL  bRxMsgQueueCreated = EC_FALSE;
    if ((EC_NULL != m_poRxMsgQueue) || (EC_NULL != m_poTxMsgQueue))
    {
        /* don't clean up */
        return EC_E_INVALIDSTATE;
    }

    m_poRxMsgQueue = EC_NEW(CMsgQueue());
    m_poTxMsgQueue = EC_NEW(CMsgQueue());
    if ((EC_NULL == m_poRxMsgQueue) || (EC_NULL == m_poTxMsgQueue))
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRes = m_poTxMsgQueue->Create(DATA_QUEUE_CLIENT_TO_SERVER_NAME_FORMAT, EC_FALSE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, EC_INVALID_QUEUE_ID);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    bTxMsgQueueCreated = EC_TRUE;

    dwRes = m_poRxMsgQueue->Create(DATA_QUEUE_SERVER_TO_CLIENT_NAME_FORMAT, EC_TRUE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, m_poTxMsgQueue->GetId());
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    bRxMsgQueueCreated = EC_TRUE;

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);

    /* clean up in case of error */
    if (dwRetVal != EC_E_NOERROR)
    {
        if (bRxMsgQueueCreated)
        {
            m_poRxMsgQueue->Close();
        }
        if (EC_NULL != m_poRxMsgQueue)
        {
            SafeDelete(m_poRxMsgQueue);
        }
        if (bTxMsgQueueCreated)
        {
            m_poTxMsgQueue->Close();
        }
        if (EC_NULL != m_poTxMsgQueue)
        {
            SafeDelete(m_poTxMsgQueue);
        }
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Creates logon queue. Fails e.g. if server already started.
\return EC_E_NOERROR on success
\return EC_E_INVALIDSTATE if RAM exceeded before
\return EC_E_NOMEMORY if RAM exceeded.
\return Passes return code of CMsgQueue::Create, e.g. EC_E_INVALIDSTATE on error.
*/
EC_T_DWORD CEcSocketMsgQueue::CreateLogonQueue(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    if (EC_NULL != m_poLogonQueue)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    m_poLogonQueue = EC_NEW(CMsgQueue());
    if (EC_NULL == m_poLogonQueue)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    /* VERBOSEOUT(2, ("Open logon message queue\n")); */
    dwRes = m_poLogonQueue->Create((EC_T_CHAR*)LOGON_QUEUE_NAME, EC_TRUE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, EC_LOGON_QUEUE_ID);
    if( EC_E_NOERROR != dwRes )
    {
        /* VERBOSEOUT(1, ("Unable to open logon message queue!\n")); */
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Opens socket's message queues.
*/
EC_T_DWORD CEcSocketMsgQueue::OpenMsgQueues(EC_T_DWORD dwClientToServerQueueId)
{
#if (defined EC_SOCKET_MSGQUEUE_SUPPORTED)
    EC_T_DWORD dwRetVal = EC_E_ERROR;
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    EC_T_DWORD dwRes    = EC_E_ERROR;
#endif
    EC_T_WORD  wPort    = (EC_T_WORD)(dwClientToServerQueueId & 0xffff);
    EC_T_BOOL  bTxMsgQueueOpened = EC_FALSE;
    EC_T_BOOL  bRxMsgQueueOpened = EC_FALSE;
    if (EC_INVALID_QUEUE_ID == dwClientToServerQueueId)
    {
        /* don't clean up */
        return EC_E_INVALIDPARM;
    }

#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    if ((EC_NULL != m_poRxMsgQueue) || (EC_NULL != m_poTxMsgQueue))
    {
        /* don't clean up */
        return EC_E_INVALIDSTATE;
    }

    m_poRxMsgQueue = EC_NEW(CMsgQueue());
    if (EC_NULL == m_poRxMsgQueue)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    m_poTxMsgQueue = EC_NEW(CMsgQueue());
    if (EC_NULL == m_poTxMsgQueue)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRes = m_poTxMsgQueue->Open(DATA_QUEUE_SERVER_TO_CLIENT_NAME_FORMAT, EC_FALSE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, dwClientToServerQueueId);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }
    bTxMsgQueueOpened = EC_TRUE;

    dwRes = m_poRxMsgQueue->Open(DATA_QUEUE_CLIENT_TO_SERVER_NAME_FORMAT, EC_TRUE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, dwClientToServerQueueId);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }
    bRxMsgQueueOpened = EC_TRUE;

#endif

    m_bConnected = EC_TRUE;
    m_bBound     = EC_TRUE;
    m_bDisc      = EC_FALSE;
    m_wPort = wPort;
    dwRetVal = EC_E_NOERROR;
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
Exit:
#endif
    OsDbgAssert(EC_E_ERROR != dwRetVal);

    /* clean up in case of error */
    if (dwRetVal != EC_E_NOERROR)
    {
        if (bRxMsgQueueOpened)
        {
            m_poRxMsgQueue->Close();
        }
        if (EC_NULL != m_poRxMsgQueue)
        {
            SafeDelete(m_poRxMsgQueue);
        }
        if (bTxMsgQueueOpened)
        {
            m_poTxMsgQueue->Close();
        }
        if (EC_NULL != m_poTxMsgQueue)
        {
            SafeDelete(m_poTxMsgQueue);
        }
    }
    return dwRetVal;
#else
    OsDbgAssert(EC_FALSE);
    return EC_E_NOTSUPPORTED;
#endif /* EC_SOCKET_MSGQUEUE_SUPPORTED */
}

/***************************************************************************************************/
/**
\brief  Opens socket's logon queue.
*/
EC_T_DWORD CEcSocketMsgQueue::OpenLogonQueue(EC_T_VOID)
{
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    if (EC_NULL != m_poLogonQueue)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    m_poLogonQueue = EC_NEW(CMsgQueue());
    if (EC_NULL == m_poLogonQueue)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRes = m_poLogonQueue->Open((EC_T_CHAR*)LOGON_QUEUE_NAME, EC_FALSE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, EC_LOGON_QUEUE_ID);
    if( EC_E_NOERROR != dwRes )
    {
        /* VERBOSEOUT(1, ("Unable to open logon message queue!\n")); */
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
#else
    OsDbgAssert(EC_FALSE);
    return EC_E_NOTSUPPORTED;
#endif
}
#endif /* EC_SOCKET_MSGQUEUE_SUPPORTED */
#endif /* EC_SOCKET_SUPPORTED */
/*-END OF SOURCE FILE--------------------------------------------------------*/
