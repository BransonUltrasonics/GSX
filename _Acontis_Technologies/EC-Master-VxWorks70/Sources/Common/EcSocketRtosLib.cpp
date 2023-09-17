/*-----------------------------------------------------------------------------
* EcSocket.cpp             file
* Copyright                acontis technologies GmbH, Weingarten, Germany
* Response                 Bussmann, Paul
* Description              Implementation of RTOS32Win RAS over Shared Memory
* Date                     2012/11/29::11:30
*
* History:
* 21nov14,PBN Fix: Upstream-merge from V2.6 and applied CEcTimer refactoring
* 14mar14,GTt Upd: RtosMsgQueue uses OS id instead of IP address.
* 13mar14,GTT Fix: Handle RtosSocketRead error RTE_ERROR_SOCKET_DISCONNECTED.
*
*---------------------------------------------------------------------------*/

#include <EcOs.h>
#if defined(EC_SOCKET_RTOSLIB_SUPPORTED)

/*-INCLUDES------------------------------------------------------------------*/

#include <EcSocket.h>
#include <EcCommon.h>
#include <EcTimer.h>

/*-STATIC CLASS-MEMBERS------------------------------------------------------*/
EC_T_INT    CEcSocketRtosLib::s_nRtosLibInitCount   = 0;


/*-FUNCTIONS-----------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Constructor of CEcSocketRtosLib
*/
CEcSocketRtosLib::CEcSocketRtosLib() :
    m_hRtosSocket(NULL)
{
    RtosLibInit();

    m_eType = emrassocktype_rtoslib;
    RtosSocketCreate( RTOSSOCKET_FAMILY_RTE, RTOSSOCKET_TYPE_STREAM, RTOSSOCKET_PROTOCOL_TCP, &m_hRtosSocket );
}


/***************************************************************************************************/
/**
\brief  Constructor of CEcSocketRtosLib
*/
CEcSocketRtosLib::CEcSocketRtosLib( RTOSLIB_HANDLE hNewSocket ) :
    m_hRtosSocket(hNewSocket)
{
    m_bDisc = EC_FALSE;
    RtosLibInit();
}


/***************************************************************************************************/
/**
\brief  Destructor of CEcSocketRtosLib
*/
CEcSocketRtosLib::~CEcSocketRtosLib() 
{
    if (NULL != m_hRtosSocket)
    {
        /* Queue was not closed! Closing now. */
        RtosSocketClose( m_hRtosSocket );
        m_hRtosSocket = NULL;
    }

    RtosLibDeinit();
}

/***************************************************************************************************/
/** 
\brief  Prepares the OS for Shared Memory Usage
*/
EC_T_DWORD CEcSocketRtosLib::RtosLibInit(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
    EC_T_INT   nRes;


    /* Use interlock function because multi-thread - multi-cpu constellations */
    RtosInterlockedExchangeAdd( &s_nRtosLibInitCount, 1, &nRes );
    if( 0 == nRes )
    {
        dwRes = ::RtosLibInit();
        if (dwRes != RTE_SUCCESS)
        {
            dwRetVal = EC_E_NOTFOUND;
            goto Exit;
        }

#if !defined(_WINDOWS)
        /* Required for RTOS-32 only; Handled by RtosService on Windows-side */
        dwRes = RtosCommStart();
        if (dwRes != RTE_SUCCESS)
        {
            dwRetVal = EC_E_NOTFOUND;
            goto Exit;
        }
#endif /* !_WINDOWS */

    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/** 
\brief  Frees resources for Shared Memory Usage
*/
EC_T_DWORD CEcSocketRtosLib::RtosLibDeinit(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
    EC_T_INT   nRes;


    /* Use interlock function because multi-thread - multi-cpu constellations */
    RtosInterlockedExchangeAdd( &s_nRtosLibInitCount, -1, &nRes );
    if( 0 >= nRes )
    {
        RtosInterlockedExchangeAdd( &s_nRtosLibInitCount, 1, &nRes );
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (1 == nRes)
    {
#if !defined(_WINDOWS)
        /* Required for RTOS-32 only; Handled by RtosService on Windows-side */
        dwRes = RtosCommStop();
        if (dwRes != RTE_SUCCESS)
        {
            dwRetVal = EC_E_NOTFOUND;
            goto Exit;
        }
#endif /* !_WINDOWS */

        dwRes = ::RtosLibDeinit();
        if (dwRes != RTE_SUCCESS)
        {
            dwRetVal = EC_E_NOTFOUND;
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
EC_T_DWORD CEcSocketRtosLib::Bind(
    EC_T_IPADDR*  pAddr,          /**< [in]   IP Address to bind to */
    EC_T_WORD     wPort           /**< [in]   Port Nr to bind to */
    )
{
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;


    EC_UNREFPARM(pAddr);

    if( NULL == m_hRtosSocket )
    {
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }

    if( RTE_SUCCESS != RtosSocketBind( m_hRtosSocket, wPort ) )
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
EC_T_DWORD CEcSocketRtosLib::Listen(EC_T_VOID)
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    EC_T_DWORD  dwRes;


    if( NULL == m_hRtosSocket )
    {
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }

    dwRes = RtosSocketListen( m_hRtosSocket );
    switch( RTE_ERROR_GET_ERROR(dwRes) )
    {
    case RTE_SUCCESS:
        break;
    case RTE_ERROR_TIMEOUT:
        dwRetVal = EC_E_TIMEOUT;
        goto Exit;
    case RTE_ERROR_INVALID_HANDLE_UNLOADED:
    case RTE_ERROR_SOCKET_DISCONNECTED:
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    default:
        dwRetVal = EC_E_ACCESSDENIED;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if(EC_E_NOERROR != dwRetVal)
    {
        m_bDisc = EC_TRUE; 
    }
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
EC_T_DWORD CEcSocketRtosLib::Accept(
    CEcSocket**    ppoNewSocket,      /**< [out]  New generated socket */
    EC_T_DWORD     dwTimeout          /**< [in]   Timeout to wait for a socket connection in msecs, cannot be INFINITE! */
                                        )
{
    EC_T_DWORD        dwRetVal         = EC_E_ERROR;
    EC_T_DWORD        dwRes;
    RTOSLIB_HANDLE    hSocketNewHandle = NULL;
    RTOSSOCKET_ADDR   SocketNewAddr;


    if (EC_NULL == ppoNewSocket)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    (*ppoNewSocket) = EC_NULL;

    if( NULL == m_hRtosSocket )
    {
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }

    SocketNewAddr.bySize = sizeof(RTOSSOCKET_ADDR);
    dwRes = RtosSocketAccept( m_hRtosSocket, &hSocketNewHandle, &SocketNewAddr, dwTimeout );
    switch( RTE_ERROR_GET_ERROR(dwRes) )
    {
    case RTE_SUCCESS:
        break;
    case RTE_ERROR_TIMEOUT:
        dwRetVal = EC_E_TIMEOUT;
        goto Exit;
    case RTE_ERROR_INVALID_HANDLE_UNLOADED:
    case RTE_ERROR_SOCKET_DISCONNECTED:
        m_bDisc = EC_TRUE;
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    default:
        dwRetVal = EC_E_ACCESSDENIED;
        goto Exit;
    }

    (*ppoNewSocket) = EC_NEW( CEcSocketRtosLib( hSocketNewHandle ) );
    if (EC_NULL == (*ppoNewSocket))
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    m_bDisc = EC_FALSE;
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
EC_T_DWORD CEcSocketRtosLib::Connect(
    EC_T_IPADDR*   pAddr,      /**< [in]   Server IP Address */
    EC_T_WORD      wPort       /**< [in]   Server Port value */
    )
{
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    EC_T_DWORD      dwRes;
    RTOSSOCKET_ADDR Addr;


    if( EC_NULL == pAddr )
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if( NULL == m_hRtosSocket )
    {
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }

    Addr.bySize         = sizeof(RTOSSOCKET_ADDR);
    Addr.byFamily       = RTOSSOCKET_FAMILY_RTE;
    Addr.wPort          = wPort;
    Addr.u.Rte.dwOsId   = pAddr->dwAddr;

    dwRes = RtosSocketConnect( m_hRtosSocket, &Addr );
    switch( RTE_ERROR_GET_ERROR(dwRes) )
    {
    case RTE_SUCCESS:
        break;
    case RTE_ERROR_TIMEOUT:
        dwRetVal = EC_E_TIMEOUT;
        goto Exit;
    case RTE_ERROR_INVALID_HANDLE_UNLOADED:
    case RTE_ERROR_SOCKET_DISCONNECTED:
        m_bDisc = EC_TRUE;
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    default:
        dwRetVal = EC_E_ACCESSDENIED;
        goto Exit;
    }

#if (defined EC_SOCKET_TRACESUPPORT)
    OsSnprintf(m_szDumpFileName, sizeof(m_szDumpFileName), 
        "dump0x%lx.%d.rascap",
        pAddr->dwAddr,
        wPort 
        );
#endif /* EC_SOCKET_TRACESUPPORT */

    m_bDisc = EC_FALSE;
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
EC_T_DWORD CEcSocketRtosLib::Shutdown(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    if( NULL == m_hRtosSocket )
    {
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }

    dwRes = RtosSocketShutdown( m_hRtosSocket );
    switch( RTE_ERROR_GET_ERROR(dwRes) )
    {
    case RTE_SUCCESS:
    	break;
    case RTE_ERROR_TIMEOUT:
        dwRetVal = EC_E_TIMEOUT;
        goto Exit;
    case RTE_ERROR_INVALID_HANDLE_UNLOADED:
    case RTE_ERROR_SOCKET_DISCONNECTED:
        m_bDisc = EC_TRUE;
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    default:
        dwRetVal = EC_E_ERROR;
        goto Exit;
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
EC_T_DWORD CEcSocketRtosLib::Close(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;


    if( NULL == m_hRtosSocket )
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRes = RtosSocketClose( m_hRtosSocket );
    /* After return of RtosSocketClose the handle is INVALID! */
    m_hRtosSocket = NULL;

    if( RTE_SUCCESS != dwRes )
    {
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }    
    
    dwRetVal = EC_E_NOERROR;

Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get a new socket handler for re-usage.
*/
EC_T_DWORD CEcSocketRtosLib::Respawn(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;


    if( NULL != m_hRtosSocket )
    {
        dwRetVal = EC_E_ACCESSDENIED;
        goto Exit;
    }

    if( RTE_SUCCESS != RtosSocketCreate( RTOSSOCKET_FAMILY_RTE, RTOSSOCKET_TYPE_STREAM, RTOSSOCKET_PROTOCOL_TCP, &m_hRtosSocket ) )
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
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
EC_T_DWORD CEcSocketRtosLib::RecvData(
        EC_T_PBYTE      pbyData,
        EC_T_DWORD*     pdwLen,            /**< [in, out]  Size of data Buffer / used buffer size */ 
        EC_T_DWORD      dwTimeout)
{
    return this->RecvData(pbyData, pdwLen, dwTimeout, EC_NULL, EC_NULL);
}
EC_T_DWORD CEcSocketRtosLib::RecvData(
    EC_T_PBYTE     pbyData,          /**< [in, out]  Data buffer for recv data */
    EC_T_DWORD*    pdwLen,           /**< [in, out]  Size of data buffer / used buffer size */
    EC_T_DWORD     dwTimeout,        /**< [in]       Time-out to wait for recv Data */
    EC_T_SOCKADDR* poSrcAddress,     /**< [out]      Source IP address   */
    EC_T_DWORD*    pdwSrcAddressLen  /**< [out]      Source IP address length */
                                    )
{
EC_T_DWORD   dwRetVal        = EC_E_ERROR;
EC_T_DWORD   dwRes           = EC_E_ERROR;
RTOSSOCKET_ADDR Addr;
EC_T_DWORD   dwCurRx         = 0;
EC_T_DWORD   dwReceivedBytes = 0;
CEcTimer oTimeOut;


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
        if (NULL == m_hRtosSocket)
        {
            m_bDisc = EC_TRUE;
            dwRetVal = EC_E_SOCKET_DISCONNECTED;
            goto Exit;
        }

        if (poSrcAddress != EC_NULL && pdwSrcAddressLen != EC_NULL)
        {
            Addr.bySize = sizeof( Addr );

            dwRes = RtosSocketReadFrom( m_hRtosSocket, &pbyData[dwReceivedBytes], (EC_T_INT)((*pdwLen)-dwReceivedBytes), &dwCurRx, &Addr, oTimeOut.GetDuration() );
            if( RTE_SUCCESS == dwRes )
            {
                /* TODO: move data from Addr to poSrcAddress */
                OsDbgAssert( FALSE );
            }
        }
        else
        {
            dwRes = RtosSocketRead( m_hRtosSocket, &pbyData[dwReceivedBytes], (EC_T_INT)((*pdwLen)-dwReceivedBytes), &dwCurRx, oTimeOut.GetDuration() );
        }
        switch( RTE_ERROR_GET_ERROR(dwRes) )
        {
        case RTE_SUCCESS:
            /* increment rx bytes count about the read chunk */
            dwReceivedBytes += dwCurRx;
            break;
        case RTE_ERROR_TIMEOUT:
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        case RTE_ERROR_INVALID_HANDLE_UNLOADED:
        case RTE_ERROR_SOCKET_DISCONNECTED:
            m_bDisc = EC_TRUE;
            dwRetVal = EC_E_SOCKET_DISCONNECTED;
            goto Exit;
        default:
            dwRetVal = EC_E_ERROR;
            goto Exit;
        }
    }
    while ( (dwReceivedBytes < *pdwLen) && !oTimeOut.IsElapsed() );

#if (defined EC_SOCKET_TRACESUPPORT)
    if (s_bTraceEnabled)
    {
        DumpPkt(EC_FALSE, pbyData, dwReceivedBytes);
    }
#endif

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
EC_T_DWORD CEcSocketRtosLib::SendData(
        EC_T_PBYTE      pbyData,
        EC_T_DWORD      dwLen)
{
    return this->SendData(pbyData, dwLen, EC_NULL, EC_NULL);
}
EC_T_DWORD CEcSocketRtosLib::SendData(
                                    EC_T_PBYTE pbyData,          /**< [in]   Data Pointer */
                                    EC_T_DWORD dwLen,            /**< [in]   Size of data */
                                    EC_T_SOCKADDR* poDstAddress, /**< [in]   Destination IP address */
                                    EC_T_DWORD dwDstAddressLen   /**< [in]   Destination IP address length */
                                    )
{
    EC_T_DWORD dwRetVal    = EC_E_ERROR;
    EC_T_DWORD dwRes;
    EC_T_DWORD dwSentBytes = 0;
    RTOSSOCKET_ADDR Addr;

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

    if (NULL == m_hRtosSocket)
    {
        m_bDisc = EC_TRUE;
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    }

    if (poDstAddress != EC_NULL && dwDstAddressLen != EC_NULL)
    {
        Addr.bySize         = sizeof(RTOSSOCKET_ADDR);
        Addr.byFamily       = RTOSSOCKET_FAMILY_RTE;
        /* TODO: move data from poDstAddress to Addr
        Addr.wPort          = ?;
        Addr.Ip.V4.dwData   = ?;
        */
        OsDbgAssert( FALSE );
        dwRes = RtosSocketSendTo( m_hRtosSocket, pbyData, dwLen, &dwSentBytes, &Addr );
    }
    else
    {
        dwRes = RtosSocketSend( m_hRtosSocket, pbyData, dwLen, &dwSentBytes );
    }

#if (defined EC_SOCKET_TRACESUPPORT)
    if (s_bTraceEnabled)
    {
        DumpPkt(EC_TRUE, pbyData, EC_MIN(dwSentBytes, dwLen));
    }
#endif

    switch( RTE_ERROR_GET_ERROR(dwRes) )
    {
    case RTE_SUCCESS:
        break;
    case RTE_ERROR_TIMEOUT:
        dwRetVal = EC_E_TIMEOUT;
        goto Exit;
    case RTE_ERROR_INVALID_HANDLE_UNLOADED:
    case RTE_ERROR_SOCKET_DISCONNECTED:
        m_bDisc = EC_TRUE;
        dwRetVal = EC_E_SOCKET_DISCONNECTED;
        goto Exit;
    default:
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

#endif /* EC_SOCKET_RTOSLIB_SUPPORTED */
/*-END OF SOURCE FILE--------------------------------------------------------*/
