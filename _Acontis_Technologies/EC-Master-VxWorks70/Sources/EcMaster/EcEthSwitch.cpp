/*-----------------------------------------------------------------------------
 * EcEthSwitch.cpp          cpp file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description
*---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>

#if (defined INCLUDE_EOE_SUPPORT)

#include <EcMasterCommon.h>
#include <EcEthSwitch.h>

/*
every CEthernetPort adds itself to the port map(m_pPortMap) by calling m_pSwitch->PortConnect(this)
in the constructor of the calls.
Entries to the Mac-map are added only in calls to switchFrame. if the map is full an old entry is used
Several ports could be addressed so a reference counter is utilized.
*/


/*****************************************************************************/
/** \brief constructor
*
* \return
*/
CEcEthSwitch::CEcEthSwitch(EC_T_INT maxPorts, EC_T_INT maxFrames, EC_T_INT maxMACs, struct _EC_T_MEMORY_LOGGER* pMLog)
:m_pMLog(pMLog)
{
    EC_T_INT i = 0;
    SWITCH_DATA_BUFFER* pData = EC_NULL;
    m_poFreeDataLock = OsCreateLockTyped(eLockType_SPIN);

    m_currTTL     = 0;
    m_pDataBuffer = EC_NULL;
    m_pMacMap     = EC_NULL;
    m_pPortMap    = EC_NULL;
    InitializeListHead(&m_listFreeData);
    InitializeListHead(&m_listTtlMac);

    maxPorts = EC_MAX(maxPorts, 1);

    if (0 == maxFrames)
    {
        maxFrames = 10 * maxPorts;
    }

    if (0 == maxMACs)
    {
        maxMACs = 10 * maxPorts;
    }

    /* allocate buffer for frames this includes framedata and pointer to next frame */
    m_pDataBuffer = EC_NEW(SWITCH_DATA_BUFFER[maxFrames]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SWITCH, "CEcEthSwitch::m_pDataBuffer", m_pDataBuffer, maxFrames * sizeof(SWITCH_DATA_BUFFER));
    if (EC_NULL == m_pDataBuffer)
    {
        /* EC_E_NOMEMORY! */
        goto Exit;
    }
    m_dwMaxFrames = maxFrames;

    /* initialize switch list */
    OsMemset(m_pDataBuffer, 0, maxFrames * sizeof(SWITCH_DATA_BUFFER));
    for (i = 0, pData = m_pDataBuffer; i < maxFrames; i++, pData++)
    {
        InsertTailList(&m_listFreeData, (EC_T_LISTENTRY*)pData);
    }

    /* initialize map for the MAC addresses */
    m_pMacMap   = EC_NEW((CHashTableDyn<ETHERNET_ADDRESS, SWITCH_MAC_ENTRY>))(EC_MEMTRACE_CORE_SWITCH, maxMACs, GetMemLog());
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SWITCH, "CEcEthSwitch::m_pMacMap", m_pMacMap, sizeof(CHashTableDyn<ETHERNET_ADDRESS, SWITCH_MAC_ENTRY>));

    /* initialize map for the etherrnet ports */
    m_pPortMap  = EC_NEW((CHashTableDyn<EC_T_UINT64, SWITCH_PORT_ENTRY>))(EC_MEMTRACE_CORE_SWITCH, maxPorts, GetMemLog());
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SWITCH, "CEcEthSwitch::m_pPortMap", m_pPortMap, sizeof(CHashTableDyn<EC_T_UINT64, SWITCH_PORT_ENTRY>));

Exit:
    if ((EC_NULL == m_pDataBuffer) || (EC_NULL == m_pMacMap) || (EC_NULL == m_pPortMap))
    {
        EC_DBGMSG("ERROR: cannot create virtual switch: EC_E_NOMEMORY (0x9811000A)!\n");
    }
}

/********************************************************************************/
/** \brief destructor
*
* \return
*/
CEcEthSwitch::~CEcEthSwitch()
{
    /* disconnect all ports */
	if (EC_NULL != m_pPortMap)
	{
        EC_T_VOID*          pVoid = EC_NULL;
        SWITCH_PORT_ENTRY   entry;
        OsMemset(&entry, 0, sizeof(SWITCH_PORT_ENTRY));

        while (m_pPortMap->GetFirstEntry(entry, pVoid))
        {
            entry.ipPort->Disconnect();
            PortDisconnect(static_cast<CEthernetPort*>(entry.ipPort));
        }
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SWITCH, "CEcEthSwitch::~m_pDataBuffer", m_pDataBuffer, m_dwMaxFrames * sizeof(SWITCH_DATA_BUFFER));
    SafeDeleteArray(m_pDataBuffer);
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SWITCH, "CEcEthSwitch::~m_pPortMap", m_pPortMap, sizeof(CHashTableDyn<EC_T_UINT64, SWITCH_PORT_ENTRY>));
    //OsDbgAssert(m_pPortMap->KeyCount() == 0); /* TODO */
    SafeDelete(m_pPortMap);
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SWITCH, "CEcEthSwitch::~m_pMacMap", m_pMacMap, sizeof(CHashTableDyn<EC_T_UINT64, SWITCH_PORT_ENTRY>));
    SafeDelete(m_pMacMap);

    OsDeleteLock(m_poFreeDataLock);
}

/********************************************************************************/
/** \brief
*
* \return
*/
EC_T_DWORD CEcEthSwitch::AllocDataDesc(SWITCH_DATA_BUFFER** ppData)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    OsLock(m_poFreeDataLock);

    /* check the "free buffers list" for available buffers. */
    if (IsListEmpty(&m_listFreeData))
    {
        /* none available */
        *ppData = EC_NULL;
        dwRetVal = EC_S_INSUFFICIENT_RESOURCES;
        goto Exit;
    }

    /* return empty buffer */
    *ppData = (SWITCH_DATA_BUFFER*)RemoveHeadList(&m_listFreeData);
    dwRetVal = EC_S_SUCCESS;

Exit:
    OsUnlock(m_poFreeDataLock);
    return dwRetVal;
}

/********************************************************************************/
/** \brief
*
* \return
*/
EC_T_DWORD  CEcEthSwitch::ReturnDataDesc(SWITCH_DATA_BUFFER* pData)
{
    OsLock(m_poFreeDataLock);
    pData->nData = 0;
    InsertTailList(&m_listFreeData, (EC_T_LISTENTRY*)pData);
    OsUnlock(m_poFreeDataLock);

    return EC_S_SUCCESS;
}


/********************************************************************************/
/** \brief
*
* \return
*/
EC_T_DWORD  CEcEthSwitch::SendFrameToPort(
    EC_T_BOOL           bIsInitCmd
   ,SWITCH_DATA_BUFFER* pData
   ,CEthernetPort*      ipDestPort
   ,EC_T_DWORD          prio
                                         )
{

    EC_T_DWORD Status = ipDestPort->SendFrame(bIsInitCmd, pData, prio);
    return Status;
}

/********************************************************************************/
/** \brief this method is called by the ndisport
*
* \return
*/
EC_T_DWORD  CEcEthSwitch::SwitchFrame(EC_T_BOOL bIsInitCmd, SWITCH_DATA_BUFFER* pData, CEthernetPort* ipSrcPort, EC_T_DWORD prio)
{
    CEthernetPort* ipDestPort = EC_NULL;
    SWITCH_MAC_ENTRY* pSrcEntry = EC_NULL;
    SWITCH_MAC_ENTRY* pDestEntry = EC_NULL;
#ifdef  __TMS470__
    ETHERNET_ADDRESS* pSource = EC_NULL;
    ETHERNET_ADDRESS* pDestination = EC_NULL;
    ETHERNET_ADDRESS* pMac = EC_NULL;
#endif

    if (0 != OsMemcmp(pData->head.Destination.b, BroadcastEthernetAddress.b, ETHERNET_ADDRESS_LEN))
    {
#ifdef  __TMS470__
        pDestination = &pData->head.Destination;
        pDestEntry = m_pMacMap->Lookup(*pDestination);
#else
        pDestEntry = m_pMacMap->Lookup(pData->head.Destination);
#endif
        if (EC_NULL != pDestEntry)
        {
            ipDestPort = pDestEntry->ipPort;
        }
    }

#ifdef  __TMS470__
    pSource = &pData->head.Source;
    pSrcEntry = m_pMacMap->Lookup(*pSource);
#else
    pSrcEntry = m_pMacMap->Lookup(pData->head.Source);
#endif
    if (EC_NULL == pSrcEntry)
    {   /* src unknown -> create new entry */
        SWITCH_MAC_ENTRY srcEntry;
        OsMemset(&srcEntry, 0, sizeof(SWITCH_MAC_ENTRY));
        srcEntry.ttl    = m_currTTL;
        srcEntry.ipPort = ipSrcPort;
#ifdef  __TMS470__
        pMac = &srcEntry.mac;
        pSource = &pData->head.Source;
        *pMac = *pSource;
#else
        srcEntry.mac    = pData->head.Source;
#endif
        EC_DBGMSG("virtual switch port \"%s\": add mac address %02x:%02x:%02x:%02x:%02x:%02x\n",
                  ipSrcPort->GetPortName(), srcEntry.mac.b[0],srcEntry.mac.b[1],srcEntry.mac.b[2],srcEntry.mac.b[3],srcEntry.mac.b[4],srcEntry.mac.b[5]);
        if (!m_pMacMap->Add(srcEntry.mac, srcEntry))
        {
            /* no free entry -> delete oldest and add again */
            OsLock(m_poFreeDataLock);
            pSrcEntry = (PSWITCH_MAC_ENTRY)RemoveTailList(&m_listTtlMac);
            OsUnlock(m_poFreeDataLock);
            EC_DBGMSG( "virtual switch port \"%s\": remove mac address %02x:%02x:%02x:%02x:%02x:%02x\n",
                      ipSrcPort->GetPortName(), pSrcEntry->mac.b[0],pSrcEntry->mac.b[1],pSrcEntry->mac.b[2],pSrcEntry->mac.b[3],pSrcEntry->mac.b[4],pSrcEntry->mac.b[5]);
            m_pMacMap->Remove(pSrcEntry->mac);
            m_pMacMap->Add(srcEntry.mac, srcEntry );
        }
        if (EC_NULL != (pSrcEntry = m_pMacMap->Lookup(srcEntry.mac)))
        {
            EC_DBGMSG( "virtual switch add port \"%s\"\n", pSrcEntry->ipPort->GetPortName());
            OsLock(m_poFreeDataLock);
            InsertHeadList(&m_listTtlMac, (EC_T_LISTENTRY*)pSrcEntry);
            OsUnlock(m_poFreeDataLock);
        }
    }
    else
    {
        /* src known -> learn new port if MacMap contains entry to another port */
        if (ipSrcPort != pSrcEntry->ipPort)
        {
            pSrcEntry->ipPort = ipSrcPort;
        }
    }

    if (EC_NULL == ipDestPort)
    {
        /* dest unknown -> send to all ports except the source port */
        EC_T_VOID*          pVoid = EC_NULL;
        SWITCH_PORT_ENTRY   entry;
        OsMemset(&entry, 0, sizeof(SWITCH_PORT_ENTRY));

        while (m_pPortMap->GetNextEntry(entry, pVoid))
        {
            if (entry.ipPort != ipSrcPort)
            {
                /* Before sending the frame out the port, we must duplicate it.
                   Think of it: We are a switch and forward a frame, which came
                   in on a single port to multiple ports.

                   Remark: Do it the quick and easy way (regarding programing effort).
                           Maybe find the time to implement a smarter solution later!?! */
                SWITCH_DATA_BUFFER* pCopiedBuf = EC_NULL;
                AllocDataDesc(&pCopiedBuf);
                if (EC_NULL != pCopiedBuf)
                {
                    OsMemcpy((EC_T_PVOID)pCopiedBuf, (EC_T_PVOID)pData, sizeof(SWITCH_DATA_BUFFER));
                    SendFrameToPort(bIsInitCmd, pCopiedBuf, entry.ipPort, prio);
                }
            }
        }

        /* buffer for each send allocated and copied. mark given data to be not used any more. */
        ReturnDataDesc(pData);
    }
    else
    {
        /* send to known destination but not back on received port */
        if (ipSrcPort != ipDestPort)
        {
            SendFrameToPort(bIsInitCmd, pData, ipDestPort, prio);
        }
        else
        {
            ReturnDataDesc(pData);
        }
    }

    return EC_S_SUCCESS;
}

/********************************************************************************/
/** \brief
*
* \return
*/
EC_T_DWORD  CEcEthSwitch::PortConnect(CEthernetPort* ipPort)
{
    SWITCH_PORT_ENTRY entry;
    OsMemset(&entry, 0, sizeof(SWITCH_PORT_ENTRY));
    entry.ipPort    = ipPort;
    if ( m_pPortMap->Add((EC_T_UINT64)((EC_T_BYTE*)ipPort - (EC_T_BYTE*)0), entry) )
        return EC_S_SUCCESS;
    else
        return EC_S_INSUFFICIENT_RESOURCES;
}

/********************************************************************************/
/** \brief
*
* \return
*/
EC_T_DWORD CEcEthSwitch::PortDisconnect(CEthernetPort* ipPort)
{
    FlushMacMap();

    if (m_pPortMap->Remove((EC_T_UINT64)((EC_T_BYTE*)ipPort - (EC_T_BYTE*)0)))
        return EC_S_SUCCESS;
    else
        return EC_S_UNSUCCESSFUL;
}

/*---------------------------------------------------------------------------*/

/********************************************************************************/
/** \brief
*
* \return
*/
CEthernetPort::CEthernetPort(EC_T_DWORD dwInstanceID, PSendFrameCallback pCallback, EC_T_VOID* pContext, EC_T_CHAR* szPortName)
{
    CEcEthSwitch* ipSwitch = EC_NULL;
    m_bConnected = EC_FALSE;
    m_dwInstanceId = dwInstanceID;
    m_pCallback = pCallback;
    m_pContext = pContext;
    m_pPendingReceive = EC_NULL;
    m_pPendingSend = EC_NULL;
    m_bSendFrameLocked = EC_FALSE;
    m_poSendFifoLock = OsCreateLockTyped(eLockType_SPIN);
    m_poPendingReceiveLock = OsCreateLockTyped(eLockType_SPIN);
    InitializeListHead(&m_listSendFifoL);
    InitializeListHead(&m_listSendFifoH);
    SAFE_STRCPY( m_szPortName, szPortName, PORT_NAME_MAXLEN );
    ipSwitch = CEcEthSwitch::GetInstance(dwInstanceID);
    if (EC_NULL != ipSwitch)
    {
        m_bConnected = ipSwitch->PortConnect(static_cast<CEthernetPort*>(this)) == EC_S_SUCCESS;
    }
    CEcEthSwitch::ReleaseInstance(ipSwitch);
}

/********************************************************************************/
/** \brief
*
* \return
*/
CEthernetPort::~CEthernetPort()
{
    Disconnect();
    if (EC_NULL != m_poPendingReceiveLock) OsDeleteLock(m_poPendingReceiveLock);
    if (EC_NULL != m_poSendFifoLock) OsDeleteLock(m_poSendFifoLock);
}

EC_T_VOID CEthernetPort::Disconnect(EC_T_VOID)
{
    CEcEthSwitch* ipSwitch = EC_NULL;
    if (!IsConnected()) return;
    ipSwitch = CEcEthSwitch::GetInstance(m_dwInstanceId);
    if (EC_NULL == ipSwitch)
    {
        return;
    }

    ipSwitch->PortDisconnect(static_cast<CEthernetPort*>(this));
    m_bConnected = EC_FALSE;

    if (m_pPendingReceive)
	{
        ipSwitch->ReturnDataDesc(m_pPendingReceive);
	}

#if (defined INCLUDE_EOE_DEFFERED_SWITCHING)
	while (!IsListEmpty(&m_listDefferedSwitchingFifo))
	{   /* more frames in FIFO */
		SWITCH_DATA_BUFFER* pData = (SWITCH_DATA_BUFFER*)RemoveHeadList(&m_listDefferedSwitchingFifo);
        ipSwitch->ReturnDataDesc(pData);
	}
#endif
	while (!IsListEmpty(&m_listSendFifoH))
	{   /* more frames in FIFO */
		SWITCH_DATA_BUFFER* pData = (SWITCH_DATA_BUFFER*)RemoveHeadList(&m_listSendFifoH);
        ipSwitch->ReturnDataDesc(pData);
	}
	while (!IsListEmpty(&m_listSendFifoL))
	{   /* more frames in FIFO */
		SWITCH_DATA_BUFFER* pData = (SWITCH_DATA_BUFFER*)RemoveHeadList(&m_listSendFifoL);
        ipSwitch->ReturnDataDesc(pData);
	}
    CEcEthSwitch::ReleaseInstance(ipSwitch);
}

/********************************************************************************/
/** \brief
*
* \return
*/
EC_T_DWORD CEthernetPort::SendFrame(EC_T_BOOL bIsInitCmd, SWITCH_DATA_BUFFER* pDataBuf, EC_T_DWORD prio)
{
    EC_T_DWORD          Status = EC_S_PENDING;
    EC_T_BOOL           bQueueFrame = EC_TRUE;
    EC_T_BOOL           bSendFrameLocked = EC_FALSE;
    SWITCH_DATA_BUFFER* pDataBufFifo = EC_NULL;
    EC_T_DWORD          dwPrio = 0;

    CEcEthSwitch* ipSwitch = EC_NULL;
    if (!IsConnected()) return EC_E_INVALIDSTATE;
    ipSwitch = CEcEthSwitch::GetInstance(m_dwInstanceId);
    if (EC_NULL == ipSwitch)
    {
        return EC_E_INVALIDSTATE;
    }

    OsLock(m_poPendingReceiveLock);

    /* try to lock sendframe function to protect against concurrent calls */
    {
        OsLock(m_poSendFifoLock);
        if (!m_bSendFrameLocked)
        {
            m_bSendFrameLocked = EC_TRUE;
              bSendFrameLocked = EC_TRUE;
        }
        OsUnlock(m_poSendFifoLock);
        if (!bSendFrameLocked)
        {
            goto Exit;
        }
    }

    if (EC_NULL != m_pPendingReceive)
    {
        Status = EC_S_PENDING;
        goto Exit;
    }

    /* send frames from high and low prio FIFO */
    do
    {
        pDataBufFifo = GetQueuedSendFrameNoLock(&dwPrio);
        if (EC_NULL == pDataBufFifo)
        {
            break;
        }
        /* m_pPendingReceive = pDataBufFifo is part of GetQueuedSendFrameNoLock */
        Status = (*m_pCallback)(bIsInitCmd, m_pContext, &pDataBufFifo->head, pDataBufFifo->nData);

        switch (Status)
        {
        case EC_S_PENDING:
        {
            goto Exit;
        } /* no break */
        case EC_S_DEVICE_BUSY:
        {
            m_pPendingReceive = EC_NULL;
            OsLock(m_poSendFifoLock);
            if (dwPrio >= ETHERNET_SWITCH_HIGH_PRIO)
            {
                InsertHeadList(&m_listSendFifoH, (EC_T_LISTENTRY*)pDataBufFifo);
            }
            else
            {
                InsertHeadList(&m_listSendFifoL, (EC_T_LISTENTRY*)pDataBufFifo);
            }
            OsUnlock(m_poSendFifoLock);
            Status = EC_S_PENDING;
            goto Exit;
        } /* no break */
        case EC_S_SUCCESS:
        {
            m_pPendingReceive = EC_NULL;
            ipSwitch->ReturnDataDesc(pDataBufFifo);
        } break;
        default:
        {
            OsDbgAssert(EC_FALSE);
        } break;
        }
    } while (EC_NULL != pDataBufFifo);

    /* queues empty, send frame */
    m_pPendingReceive = pDataBuf;
    Status = (*m_pCallback)(bIsInitCmd, m_pContext, &pDataBuf->head, pDataBuf->nData);
    switch (Status)
    {
    case EC_S_SUCCESS:
    {
        bQueueFrame = EC_FALSE;
        m_pPendingReceive = EC_NULL;
        ipSwitch->ReturnDataDesc(pDataBuf);
    } break;
    case EC_S_PENDING:
    {
        bQueueFrame = EC_FALSE;
    } break;
    case EC_S_DEVICE_BUSY:
    {
        m_pPendingReceive = EC_NULL;
        Status = EC_S_PENDING;
        /* goto Exit; */
    } break;
    default:
    {
        OsDbgAssert(EC_FALSE);
    } break;
    }
Exit:
    /* queue frame if needed */
    if (bQueueFrame)
    {
        OsLock(m_poSendFifoLock);
        if (prio >= ETHERNET_SWITCH_HIGH_PRIO)
        {
            InsertTailList(&m_listSendFifoH, (EC_T_LISTENTRY*)pDataBuf);
        }
        else
        {
            InsertTailList(&m_listSendFifoL, (EC_T_LISTENTRY*)pDataBuf);
        }
        OsUnlock(m_poSendFifoLock);
    }
    /* unlock send */
    if (bSendFrameLocked)
    {
        m_bSendFrameLocked = EC_FALSE;
    }
    OsUnlock(m_poPendingReceiveLock);

    CEcEthSwitch::ReleaseInstance(ipSwitch);
    return Status;
}

/********************************************************************************/
/** \brief
*
* \return
*/
SWITCH_DATA_BUFFER* CEthernetPort::GetQueuedSendFrameNoLock(EC_T_DWORD* pdwPrio)
{
    EC_T_DWORD          dwPrio  = 0;
    SWITCH_DATA_BUFFER* pDataBufFifo = EC_NULL;

    /* look for HIGH prio frame first, then LOW prio */
    OsLock(m_poSendFifoLock);
    if (!IsListEmpty(&m_listSendFifoH))
    {
        pDataBufFifo = (SWITCH_DATA_BUFFER*)RemoveHeadList(&m_listSendFifoH);
        dwPrio       = ETHERNET_SWITCH_HIGH_PRIO;
    }
    else
    {
        if (!IsListEmpty(&m_listSendFifoL))
        {
            pDataBufFifo = (SWITCH_DATA_BUFFER*)RemoveHeadList(&m_listSendFifoL);
            dwPrio       = 0;
        }
    }
    OsUnlock(m_poSendFifoLock);

    /* no frame found */
    if (EC_NULL == pDataBufFifo)
    {
        goto Exit;
    }
    m_pPendingReceive = pDataBufFifo;

    if (EC_NULL != pdwPrio)
    {
        *pdwPrio = dwPrio;
    }

Exit:
    return pDataBufFifo;
}

SWITCH_DATA_BUFFER* CEthernetPort::GetQueuedSendFrame(EC_T_DWORD* pdwPrio)
{
    EC_T_BOOL  bSendFrameLocked = EC_FALSE;
    SWITCH_DATA_BUFFER* pDataBufFifo = EC_NULL;

    OsLock(m_poPendingReceiveLock);

    /* try to lock sendframe function to protect against concurrent calls */
    {
        OsLock(m_poSendFifoLock);
        if (!m_bSendFrameLocked)
        {
            m_bSendFrameLocked = EC_TRUE;
              bSendFrameLocked = EC_TRUE;
        }
        OsUnlock(m_poSendFifoLock);
        if (!bSendFrameLocked)
        {
            goto Exit;
        }
    }

    if (EC_NULL != m_pPendingReceive)
    {
        goto Exit;
    }

    pDataBufFifo = GetQueuedSendFrameNoLock(pdwPrio);
Exit:
    /* unlock send */
    if (bSendFrameLocked)
    {
        m_bSendFrameLocked = EC_FALSE;
    }
    OsUnlock(m_poPendingReceiveLock);

    return pDataBufFifo;
}

/********************************************************************************/
/** \brief
*
* \return
*/
EC_T_DWORD  CEthernetPort::SendFrameComplete
(   EC_T_BOOL bIsInitCmd,
    ETHERNET_FRAME* pData )
{
    EC_T_DWORD          Status       = EC_S_UNSUCCESSFUL;
    SWITCH_DATA_BUFFER* pDataBufFifo = EC_NULL;

    CEcEthSwitch*       ipSwitch     = EC_NULL;
    if (!IsConnected()) return EC_E_INVALIDSTATE;
    ipSwitch = CEcEthSwitch::GetInstance(m_dwInstanceId);
    if (EC_NULL == ipSwitch)
    {
        return EC_E_INVALIDSTATE;
    }

    OsLock(m_poPendingReceiveLock);

    if (EC_NULL == m_pPendingReceive)
    {
        /* nothing to do */
    }
    else
    {
        if (&m_pPendingReceive->head == pData)
        {
            /* Sending done. Now we can return the data descriptor */
            ipSwitch->ReturnDataDesc(m_pPendingReceive);
            m_pPendingReceive = EC_NULL;
        }
        else
        {
           OsDbgAssert(EC_FALSE);
           goto Exit;
        }
    }
    /* send frames from the high priority FIFO */
    do
    {
        OsLock(m_poSendFifoLock);
        if (!IsListEmpty(&m_listSendFifoH))
        {
            pDataBufFifo = (SWITCH_DATA_BUFFER*)RemoveHeadList(&m_listSendFifoH);
        }
        else
        {
            pDataBufFifo = EC_NULL;
        }
        OsUnlock(m_poSendFifoLock);
        m_pPendingReceive = pDataBufFifo;
        if ( EC_NULL != pDataBufFifo )
        {
            Status = (*m_pCallback)(bIsInitCmd, m_pContext, &pDataBufFifo->head, pDataBufFifo->nData);
            if ( Status == EC_S_PENDING )
            {
                goto Exit;
            }
            else if ( Status == EC_S_DEVICE_BUSY )
            {
                m_pPendingReceive = NULL;
                OsLock(m_poSendFifoLock);
                InsertHeadList(&m_listSendFifoH, (EC_T_LISTENTRY*)pDataBufFifo);
                OsUnlock(m_poSendFifoLock);
                Status = EC_S_PENDING;
                goto Exit;
            }
            else if ( Status == EC_S_SUCCESS )
            {
                m_pPendingReceive = NULL;
                ipSwitch->ReturnDataDesc(pDataBufFifo);
            }
            else
            {
                OsDbgAssert(EC_FALSE);
            }
        }
    } while (EC_NULL != pDataBufFifo);

    /* send frames from the low priority FIFO */
    do
    {
        OsLock(m_poSendFifoLock);
        if (!IsListEmpty(&m_listSendFifoL))
        {
            pDataBufFifo = (SWITCH_DATA_BUFFER*)RemoveHeadList(&m_listSendFifoL);
        }
        else
        {
            pDataBufFifo = EC_NULL;
        }
        OsUnlock(m_poSendFifoLock);
        m_pPendingReceive = pDataBufFifo;
        if (EC_NULL != pDataBufFifo)
        {
            Status = (*m_pCallback)(bIsInitCmd, m_pContext, &pDataBufFifo->head, pDataBufFifo->nData);
            if ( Status == EC_S_PENDING )
            {
                goto Exit;
            }
            else if ( Status == EC_S_DEVICE_BUSY )
            {
                m_pPendingReceive = NULL;
                OsLock(m_poSendFifoLock);
                InsertHeadList(&m_listSendFifoL, (EC_T_LISTENTRY*)pDataBufFifo);
                OsUnlock(m_poSendFifoLock);
                Status = EC_S_PENDING;
                goto Exit;
            }
            else if ( Status == EC_S_SUCCESS )
            {
                m_pPendingReceive = NULL;
                ipSwitch->ReturnDataDesc(pDataBufFifo);
            }
            else
            {
                OsDbgAssert(EC_FALSE);
            }
        }
    } while (EC_NULL != pDataBufFifo);

    /* no receive pending */
    Status = EC_S_SUCCESS;

Exit:
    OsUnlock(m_poPendingReceiveLock);
    CEcEthSwitch::ReleaseInstance(ipSwitch);
    return Status;
}

/********************************************************************************/
/** \brief called from outside, e.g. AtemRasSrv deMarshalLinkSendAndFreeFrame
*   via CEcEoEUplink::EoELinkSendAndFreeFrame
* \return
*/
EC_T_DWORD  CEthernetPort::SwitchFrame(
    EC_T_BOOL       bIsInitCmd,
    ETHERNET_FRAME* pData,
    EC_T_DWORD      nData,
    EC_T_DWORD      prio,
    EC_T_DWORD      nTimeStamp,
    EC_T_DWORD      nPort
                                      )
{
    SWITCH_DATA_BUFFER* pDataDesc = m_pPendingSend;
    EC_T_DWORD          dwRes     = EC_S_UNSUCCESSFUL;
    CEcEthSwitch*       ipSwitch  = EC_NULL;
    if (!IsConnected()) return EC_E_INVALIDSTATE;
    ipSwitch = CEcEthSwitch::GetInstance(m_dwInstanceId);
    if (EC_NULL == ipSwitch)
    {
        return EC_E_INVALIDSTATE;
    }

    if ((nData > ETHERNET_MAX_FRAMEBUF_LEN) || (nData < ETHERNET_FRAME_LEN))
    {
        dwRes = EC_E_INVALIDSIZE;
        goto Exit;
    }

    if ((EC_NULL != pDataDesc) && (&pDataDesc->head == pData))
    {
        /* If pending frame already exists and matches this frame, then switch this frame.
           This is true when the calling slave was able to do an ``AllocSendBuffer()``
           before and the Ethernet frame was received over ECAT in multiple chunks. */
        pDataDesc->nData        = nData;
        pDataDesc->nTimeStamp   = nTimeStamp;
        pDataDesc->nPort        = nPort;

        m_pPendingSend          = EC_NULL;

        dwRes = ipSwitch->SwitchFrame(bIsInitCmd, pDataDesc, static_cast<CEthernetPort*>(this), prio);
    }
    else
    {
        // Come here when the EoE Ethernet frame was received on ECAT in a single chunk.
        // Then the calling slave had no chance to allocate a data descriptor, so
        // we must do it for him.
        pDataDesc = EC_NULL;
        if (ipSwitch->AllocDataDesc(&pDataDesc) == EC_S_SUCCESS )
        {
            pDataDesc->nData        = nData;
            pDataDesc->nTimeStamp   = nTimeStamp;
            pDataDesc->nPort        = nPort;

            OsMemcpy(&pDataDesc->head, pData, nData);

            dwRes = ipSwitch->SwitchFrame(bIsInitCmd, pDataDesc, static_cast<CEthernetPort*>(this), prio);
        }
        else
        {
            dwRes = EC_S_INSUFFICIENT_RESOURCES;
        }
    }
Exit:
    CEcEthSwitch::ReleaseInstance(ipSwitch);
    return dwRes;
}

/********************************************************************************/
/** \brief
*
* \return
*/
EC_T_DWORD CEthernetPort::AllocSendBuffer(EC_T_DWORD nData, ETHERNET_FRAME** ppData)
{
    EC_T_DWORD    dwRetVal = EC_E_ERROR;
    CEcEthSwitch* ipSwitch = EC_NULL;
    if (!IsConnected()) return EC_E_INVALIDSTATE;
    ipSwitch = CEcEthSwitch::GetInstance(m_dwInstanceId);
    if (EC_NULL == ipSwitch)
    {
        return EC_E_INVALIDSTATE;
    }
    EC_UNREFPARM(nData);

    if (EC_NULL != m_pPendingSend)
    {
        // The class ``CEthernetPort`` only supports a single send buffer to be
        // allocated at a time.
        dwRetVal = EC_S_DEVICE_BUSY;
        goto Exit;
    }

    if (ipSwitch->AllocDataDesc(&m_pPendingSend) == EC_S_SUCCESS)
    {
        *ppData = &m_pPendingSend->head;

        dwRetVal = EC_S_SUCCESS;
        goto Exit;
    }
    else
    {
        dwRetVal = EC_S_INSUFFICIENT_RESOURCES;
        goto Exit;
    }
Exit:
    CEcEthSwitch::ReleaseInstance(ipSwitch);
    return dwRetVal;
}

/********************************************************************************/
/** \brief
*
* \return
*/
EC_T_DWORD  CEthernetPort::FreeSendBuffer(ETHERNET_FRAME* pData)
{
    EC_T_DWORD    dwRetVal = EC_E_ERROR;
    CEcEthSwitch* ipSwitch = EC_NULL;
    if (!IsConnected()) return EC_E_INVALIDSTATE;
    ipSwitch = CEcEthSwitch::GetInstance(m_dwInstanceId);
    if (EC_NULL == ipSwitch)
    {
        return EC_E_INVALIDSTATE;
    }
    if ((EC_NULL != m_pPendingSend) && (&m_pPendingSend->head == pData))
    {
        ipSwitch->ReturnDataDesc(m_pPendingSend);
        m_pPendingSend = EC_NULL;
        dwRetVal = EC_S_SUCCESS;
        goto Exit;
    }
    else
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_S_UNSUCCESSFUL;
        goto Exit;
    }
Exit:
    CEcEthSwitch::ReleaseInstance(ipSwitch);
    return dwRetVal;
}

#if (defined INCLUDE_EOE_ENDPOINT)
/********************************************************************************/
/** \brief Constructor
*/
CEoEUplinkEthernetPort::CEoEUplinkEthernetPort(EC_T_DWORD dwInstanceId, PSendFrameCallback pCallback, CEcEoEUplink* pContext, EC_T_CHAR* szPortName )
:  CEthernetPort(dwInstanceId, pCallback,pContext, szPortName)
{
    OsDbgAssert(pContext != EC_NULL);
    m_pEoEUplink = pContext;
}



/********************************************************************************/
/** \brief  Overwrites the CEthernetPort::SendFrame and calls CEthernetPort::SendFrame
*           only if an EoE-Driver was registered.
* \return   Returns the return value of CEthernetPort::SendFrame or EC_S_UNSUCCESSFUL
*           if no EoE-Driver was registered (m_pEoEUplink->m_bIsOpen == false)
*/
EC_T_DWORD CEoEUplinkEthernetPort::SendFrame( EC_T_BOOL bIsInitCmd, SWITCH_DATA_BUFFER* pDesc, EC_T_DWORD prio )
{
    EC_T_DWORD dwRetVal = EC_S_UNSUCCESSFUL;

    if(m_pEoEUplink == EC_NULL)
    {
        OsDbgAssert(m_pEoEUplink != EC_NULL);
        dwRetVal = EC_S_UNSUCCESSFUL;
        goto Exit;
    }

    if(m_pEoEUplink->m_bIsOpen)
    {
        dwRetVal = CEthernetPort::SendFrame(bIsInitCmd, pDesc, prio);
    }

Exit:
    return dwRetVal;
}

#endif   /* (defined INCLUDE_EOE_ENDPOINT) */



//#############################################################################
//    Class ``CEcEoEUplink`` member functions
//#############################################################################


#if (defined INCLUDE_EOE_ENDPOINT)
/********************************************************************************/
/*  Member function ``CEcEoEUplink`` (constructor)
*/
CEcEoEUplink::CEcEoEUplink(EC_T_DWORD dwInstanceId, CEcEthSwitch* pVirtualEthernetSwitch) :
              m_dwInstanceId(dwInstanceId)
{
    m_bIsOpen = EC_FALSE;

    /* initialization for completeness */
    m_pfNotificationCbk = 0;
    m_pfReceiveCbk = 0;
    m_pvCallerContext = 0;
    EC_UNREFPARM(pVirtualEthernetSwitch);

    // Create our own instance of an Ethernet port.
    // The new port is connected to our parent switch automatically during construction.
    m_pEthernetPort = EC_NEW(CEoEUplinkEthernetPort(dwInstanceId,
                                        (PSendFrameCallback)(EoELinkIncomingFrameCbk),
                                        this,
                                        (EC_T_CHAR*)"UplinkPort"));
    if (m_pEthernetPort)
    {
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_DEVICE, "CEcEoEUplink::m_pEthernetPort", m_pEthernetPort, sizeof(CEoEUplinkEthernetPort));
    }
}

struct _EC_T_MEMORY_LOGGER* CEcEoEUplink::GetMemLog()
{
    struct _EC_T_MEMORY_LOGGER* pMemLog = EC_NULL;
    CEcEthSwitch* ipSwitch = EC_NULL;
    ipSwitch = CEcEthSwitch::GetInstance(m_dwInstanceId);
    if (EC_NULL == ipSwitch)
    {
        goto Exit;
    }
    pMemLog = ipSwitch->GetMemLog();
Exit:
    CEcEthSwitch::ReleaseInstance(ipSwitch);
    return pMemLog;
}

/********************************************************************************/
/*  Member function ``~CEcEoEUplink`` (destructor)
*/
CEcEoEUplink::~CEcEoEUplink()
{
    /* frames may not be switched to this uplink anymore */
    SafeDelete(m_pEthernetPort);
}

/********************************************************************************/
/*  Member function ``EoELinkRegister``
*/
EC_T_DWORD CEcEoEUplink::EoELinkRegister(EC_T_DWORD dwInstanceId, CEcEthSwitch* pSwitch, EC_T_CHAR* szEoEDrvIdent, EC_T_LINK_DRV_DESC* pLinkDrvDesc)
{
EC_T_DWORD    dwRetVal   = EC_E_ERROR;
CEcEoEUplink* pEoEUplink = EC_NULL;

    EC_UNREFPARM(szEoEDrvIdent);

    /* check parameters */
    if (EC_NULL == pLinkDrvDesc)
    {
        EC_DBGMSG("CEcEoEUplink::EoELinkRegister: drive descriptor is NULL");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (pLinkDrvDesc->dwValidationPattern != LINK_LAYER_DRV_DESC_PATTERN)
    {
        EC_DBGMSG("CEcEoEUplink::EoELinkRegister: invalid drive descriptor pattern 0x%x instead of 0x%x\n", pLinkDrvDesc->dwValidationPattern, LINK_LAYER_DRV_DESC_PATTERN);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (pLinkDrvDesc->dwInterfaceVersion != LINK_LAYER_DRV_DESC_VERSION)
    {
        EC_DBGMSG( "CEcEoEUplink::EoELinkRegister: invalid drive descriptor interface version 0x%x instead of 0x%x\n", pLinkDrvDesc->dwInterfaceVersion, LINK_LAYER_DRV_DESC_VERSION );
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* create uplink object */
    pEoEUplink = EC_NEW(CEcEoEUplink(dwInstanceId, pSwitch));
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_CORE_DEVICE, pSwitch->GetMemLog(), "CEcEoEUplink::CEcEoEUplink", pEoEUplink, sizeof(CEcEoEUplink));
    if (EC_NULL == pEoEUplink)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    /* fill driver descriptor */
    pLinkDrvDesc->pvLinkInstance = (EC_T_VOID*)pEoEUplink;
    pLinkDrvDesc->pfEcLinkOpen = EoELinkOpen;
    pLinkDrvDesc->pfEcLinkClose = EoELinkClose;
    pLinkDrvDesc->pfEcLinkSendFrame = EoELinkSendFrame;
    pLinkDrvDesc->pfEcLinkSendAndFreeFrame = EoELinkSendAndFreeFrame;
    pLinkDrvDesc->pfEcLinkRecvFrame = EoELinkRecvFrame;
    pLinkDrvDesc->pfEcLinkAllocSendFrame = EoELinkAllocSendFrame;
    pLinkDrvDesc->pfEcLinkFreeSendFrame  = EoELinkFreeSendFrame;
    pLinkDrvDesc->pfEcLinkFreeRecvFrame  = EoELinkFreeRecvFrame;
    pLinkDrvDesc->pfEcLinkGetEthernetAddress = EoELinkGetEthernetAddress;
    pLinkDrvDesc->pfEcLinkGetStatus = EoELinkGetStatus;
    pLinkDrvDesc->pfEcLinkGetSpeed = EoELinkGetSpeed;
    pLinkDrvDesc->pfEcLinkGetMode = EoELinkGetMode;
    pLinkDrvDesc->pfEcLinkIoctl = EoELinkIoctl;

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/********************************************************************************/
/*  Member function ``EoELinkUnregister``
*/
EC_T_DWORD CEcEoEUplink::EoELinkUnregister(EC_T_LINK_DRV_DESC* pLinkDrvDesc)
{
EC_T_DWORD    dwRetVal   = EC_E_ERROR;
CEcEoEUplink* pEoEUplink = EC_NULL;

    /* check parameters */
    if (EC_NULL == pLinkDrvDesc)
    {
        EC_DBGMSG("CEcEoEUplink::EoELinkUnregister: drive descriptor is NULL");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (pLinkDrvDesc->dwValidationPattern != LINK_LAYER_DRV_DESC_PATTERN)
    {
        EC_DBGMSG( "CEcEoEUplink::EoELinkUnregister: invalid drive descriptor pattern 0x%x instead of 0x%x\n", pLinkDrvDesc->dwValidationPattern, LINK_LAYER_DRV_DESC_PATTERN );
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (pLinkDrvDesc->dwInterfaceVersion != LINK_LAYER_DRV_DESC_VERSION)
    {
        EC_DBGMSG( "CEcEoEUplink::EoELinkUnregister: invalid drive descriptor interface version 0x%x instead of 0x%x\n", pLinkDrvDesc->dwInterfaceVersion, LINK_LAYER_DRV_DESC_VERSION );
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* delete uplink object */
    pEoEUplink = (CEcEoEUplink*)pLinkDrvDesc->pvLinkInstance;
    pLinkDrvDesc->pvLinkInstance = EC_NULL;
    EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_SWITCH, pEoEUplink->GetMemLog(), "CEcEoEUplink::CEcEoEUplink", pEoEUplink, sizeof(CEcEoEUplink));
    SafeDelete(pEoEUplink);

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/********************************************************************************/
/*  Member function ``EoELinkOpen``
*/
EC_T_DWORD CEcEoEUplink::EoELinkOpen(EC_T_VOID* pvLinkParms,                           /* [in]  link parameters */
                                     EC_T_RECEIVEFRAMECALLBACK pfReceiveFrameCallback, /* [in]  pointer to rx callback function (unused) */
                                     EC_T_LINK_NOTIFY pfLinkNotifyCallback,            /* [in]  pointer to notification callback function (unused) */
                                     EC_T_VOID* pvContext,                             /* [in]  caller context, to be used in callback functions */
                                     EC_T_VOID** ppvInstance)
{
EC_T_DWORD dwRetVal  = EC_E_ERROR;
EC_T_LINK_OPENPARMS_EOE* psOpenParameters = EC_NULL;
CEcEoEUplink*            pEoEUplink       = EC_NULL;

    EC_UNREFPARM(pfLinkNotifyCallback);

    psOpenParameters = (EC_T_LINK_OPENPARMS_EOE*)pvLinkParms;
    if ((EC_NULL == psOpenParameters) || (OsStrlen(psOpenParameters->szEoEDrvIdent) == 0) || (EC_NULL == psOpenParameters->pvUplinkInstance))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pEoEUplink = (CEcEoEUplink*)psOpenParameters->pvUplinkInstance;
    if (pEoEUplink->m_bIsOpen)
    {
        dwRetVal = EC_E_ACCESSDENIED;
        goto Exit;
    }

    pEoEUplink->m_pvCallerContext = pvContext;
    pEoEUplink->m_pfReceiveCbk = pfReceiveFrameCallback;
    pEoEUplink->m_bIsOpen = EC_TRUE;
    *ppvInstance = (EC_T_PVOID)pEoEUplink;

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/********************************************************************************/
/*  Member function ``EoELinkClose``
*/
EC_T_DWORD CEcEoEUplink::EoELinkClose(EC_T_VOID* pvInstance)
{
    CEcEoEUplink* pEoEUplink = ((CEcEoEUplink*)pvInstance);

    pEoEUplink->m_bIsOpen = EC_FALSE;
    return EC_E_NOERROR;
}


/********************************************************************************/
/*  Member function ``EoELinkSendFrame``
*/
EC_T_DWORD CEcEoEUplink::EoELinkSendFrame(EC_T_VOID* pvInstance,
                                          EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
{
    EC_UNREFPARM(pvInstance);
    EC_UNREFPARM(pLinkFrameDesc);

    // Not implemented. Must call ``EoELinkSendAndFreeFrame()`` instead and thus
    // force the user of the EoE Uplink API to allocate a frame buffer for every
    // single send.
    // This is due to limitations in the class CEthernetPort: Only allocation for
    // a single frame at a time is provided!!!
    return EC_E_NOTSUPPORTED;
}


/********************************************************************************/
/*  Member function ``EoELinkSendAndFreeFrame``
*/
EC_T_DWORD CEcEoEUplink::EoELinkSendAndFreeFrame(EC_T_VOID* pvInstance,
                                                 EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    CEcEoEUplink* pEoEUplink = ((CEcEoEUplink*)pvInstance);

    if ((EC_NULL == pvInstance) || (EC_NULL == pLinkFrameDesc))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwRetVal = pEoEUplink->m_pEthernetPort->SwitchFrame(EC_FALSE, (ETHERNET_FRAME*)pLinkFrameDesc->pbyFrame, pLinkFrameDesc->dwSize, 0, 0, 0);
Exit:
    return dwRetVal;
}


/********************************************************************************/
/*  Member function ``EoELinkRecvFrame``
*/
EC_T_DWORD CEcEoEUplink::EoELinkRecvFrame(EC_T_VOID* pvInstance,
                                          EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
{
    CEcEoEUplink* pEoEUplink = ((CEcEoEUplink*)pvInstance);
    EC_T_DWORD dwPrio = 0;
    SWITCH_DATA_BUFFER* pDataBufFifo = pEoEUplink->m_pEthernetPort->GetQueuedSendFrame(&dwPrio);

    /* Only have work to do when we have a frame that came in from the switch. */
    if (EC_NULL == pDataBufFifo)
    {
        return EC_S_UNSUCCESSFUL;
    }

    OsMemset(pLinkFrameDesc, 0, sizeof(EC_T_LINK_FRAMEDESC));
    pLinkFrameDesc->pvContext = (EC_T_VOID*)pEoEUplink;
    pLinkFrameDesc->pbyFrame = (EC_T_BYTE*)&pDataBufFifo->head;
    pLinkFrameDesc->dwSize   = pDataBufFifo->nData;

    return EC_S_SUCCESS;
}



/********************************************************************************/
/*  Member function ``EoELinkAllocSendFrame``
*/
EC_T_DWORD CEcEoEUplink::EoELinkAllocSendFrame(EC_T_VOID* pvInstance,
                                               EC_T_LINK_FRAMEDESC* pLinkFrameDesc,
                                               EC_T_DWORD dwSize)
{
EC_T_DWORD    dwRetVal   = EC_S_UNSUCCESSFUL;
CEcEoEUplink* pEoEUplink = (CEcEoEUplink*)pvInstance;

    /* clear frame descriptor */
    OsMemset(pLinkFrameDesc, 0, sizeof(EC_T_LINK_FRAMEDESC));

    /* allocate send buffer */
    pEoEUplink->m_pEthernetPort->AllocSendBuffer(0, (ETHERNET_FRAME**)(EC_T_VOID*)&pLinkFrameDesc->pbyFrame);
    if (EC_NULL == pLinkFrameDesc->pbyFrame)
    {
        dwRetVal = EC_S_INSUFFICIENT_RESOURCES;
        goto Exit;
    }
    pLinkFrameDesc->dwSize = dwSize;

    /* no errors */
    dwRetVal = EC_S_SUCCESS;
Exit:
    return dwRetVal;
}


/********************************************************************************/
/*  Member function ``EoELinkFreeSendFrame``
*/
EC_T_VOID CEcEoEUplink::EoELinkFreeSendFrame(EC_T_VOID* pvInstance,
                                             EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
{
    CEcEoEUplink* pEoEUplink = ((CEcEoEUplink*)pvInstance);

    // Release the attached memory buffer which we got from the Ethernet port.
    pEoEUplink->m_pEthernetPort->FreeSendBuffer((ETHERNET_FRAME*)pLinkFrameDesc->pbyFrame);
}


/********************************************************************************/
/*  Member function ``EoELinkFreeRecvFrame``
*/
EC_T_VOID CEcEoEUplink::EoELinkFreeRecvFrame(EC_T_VOID* pvInstance,
                                             EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
{
    CEcEoEUplink* pEoEUplink = ((CEcEoEUplink*)pvInstance);
 
    pEoEUplink->m_pEthernetPort->SendFrameComplete(EC_FALSE, (ETHERNET_FRAME*)pLinkFrameDesc->pbyFrame);
}


/********************************************************************************/
/*  Member function ``EoELinkGetEthernetAddress``
*/
EC_T_DWORD CEcEoEUplink::EoELinkGetEthernetAddress(EC_T_VOID* pvInstance,
                                                   EC_T_BYTE* pbyEthernetAddress)
{
    EC_UNREFPARM(pvInstance);
    EC_UNREFPARM(pbyEthernetAddress);

    return EC_E_NOERROR;
}


/********************************************************************************/
/*  Member function ``EoELinkGetStatus``
*/
EC_T_LINKSTATUS CEcEoEUplink::EoELinkGetStatus(EC_T_VOID* pvInstance)
{
    CEcEoEUplink* pEoEUplink = ((CEcEoEUplink*)pvInstance);

    // This function call does not really make sense for the ``CEcEoEUplink``
    // class, because we are not a classical link layer.

    if(pEoEUplink->m_bIsOpen == EC_TRUE)
    {
        return eLinkStatus_OK;
    }

    return eLinkStatus_UNDEFINED;
}


/********************************************************************************/
/*  Member function ``EoELinkGetSpeed``
*/
EC_T_DWORD CEcEoEUplink::EoELinkGetSpeed(EC_T_VOID* pvInstance)
{
    EC_UNREFPARM(pvInstance);

    // EtherCAT ``per ordre de mufti`` is 100 MBit.
    return 100;
}


/********************************************************************************/
/*  Member function ``EoELinkGetMode``
*/
EC_T_LINKMODE CEcEoEUplink::EoELinkGetMode(EC_T_VOID* pvInstance)
{
    EC_UNREFPARM(pvInstance);

    // So far (23-Jul-2008/KSE) only polling mode is implemented.
    return EcLinkMode_POLLING;
}


/********************************************************************************/
/*  Member function ``EoELinkIoctl``
*/
EC_T_DWORD CEcEoEUplink::EoELinkIoctl(EC_T_VOID* pvInstance,
                                      EC_T_DWORD dwCode,
                                      EC_T_LINK_IOCTLPARMS* pParms)
{
    EC_UNREFPARM(pvInstance);
    EC_UNREFPARM(dwCode);
    EC_UNREFPARM(pParms);

    return EC_E_NOTSUPPORTED;
}


/********************************************************************************/
/*  Member function ``EoELinkIncomingFrameCbk``
*/
//static EC_T_DWORD CEcEoEUplink::EoELinkIncomingFrameCbk(EC_T_BOOL bIsInitCmd,
EC_T_DWORD CEcEoEUplink::EoELinkIncomingFrameCbk(EC_T_BOOL bIsInitCmd,
                                                 EC_T_VOID* pContext,
                                                 ETHERNET_FRAME* pData,
                                                 EC_T_DWORD nData)
{
    EC_UNREFPARM(bIsInitCmd);
    EC_UNREFPARM(pContext);
    EC_UNREFPARM(pData);
    EC_UNREFPARM(nData);

    return EC_S_DEVICE_BUSY;
}

#endif /* INCLUDE_EOE_ENDPOINT */

#endif /* INCLUDE_EOE_SUPPORT */

/*-END OF SOURCE FILE--------------------------------------------------------*/
