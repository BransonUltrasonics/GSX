/*-----------------------------------------------------------------------------
 * CAtemRasCmdQueue.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/

#include "CAtemRasCmdQueue.h"
#include "AtemRasRemoteProtocol.h"
#include "EcCommon.h"

/*-CLASS---------------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  List instance constructor.
*/
CAtEmRasCmdQueue::CAtEmRasCmdQueue(
    EC_T_VOID
                                  )
{
    VERBOSEOUT(1, (">CAtEmRasCmdQueue::CAtEmRasCmdQueue()\n"));
    m_pvListLock    = OsCreateLockTyped(eLockType_SPIN);

    OsMemset(m_aList, 0, sizeof(m_aList));
    m_dwListLen = MAX_NUMOF_CMDQUEUE_ELEMENTS;

    m_dwNextFreeIndex = 0;
    m_dwNextSendIndex = 0;
    m_dwNextRecvIndex = 0;
    VERBOSEOUT(1, ("<CAtEmRasCmdQueue::CAtEmRasCmdQueue()\n"));
}

/***************************************************************************************************/
/**
\brief  List instance destructor.
*/
CAtEmRasCmdQueue::~CAtEmRasCmdQueue()
{
    VERBOSEOUT(1, (">CAtEmRasCmdQueue::~CAtEmRasCmdQueue()\n"));
    OsDeleteLock(m_pvListLock);
    VERBOSEOUT(1, ("<CAtEmRasCmdQueue::~CAtEmRasCmdQueue()\n"));
}

/***************************************************************************************************/
/**
\brief  Acquire new list entry.

Add a new entry to transmit queue list.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmRasCmdQueue::AddEntry(
    EC_T_DWORD  dwSeqNr   /**< [in]   Index, Request Id ("InvokeId") */,
    EC_T_DWORD  dwTxLen   /**< [in]   Length of send buffer */,
    EC_T_PBYTE  pbyTx     /**< [in]   Pointer to send buffer */,
    EC_T_DWORD* pdwRxLen  /**< [in]   Pointer to Length of receive buffer */,
    EC_T_PBYTE  pbyRx     /**< [in]   Pointer to receive buffer */,
    EC_T_DWORD* pdwIdx    /**< [out]  Buffer to hold List Entry Index (aka Handle) */
                                     )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    EC_T_DWORD                  dwScan      = 0;
    EC_T_DWORD                  dwCurIdx    = 0;
    EC_T_BOOL                   bListLock   = EC_FALSE;
    ATEMRAS_PT_CMDQUEUE_ELEMENT ptCur       = EC_NULL;
    ATEMRAS_PT_CMDQUEUE_ELEMENT ptAlloc     = EC_NULL;
    EC_T_BOOL                   bAutoDisp   = EC_FALSE;

#ifdef INCLUDE_LOG_MESSAGES
    ATEMRAS_T_PROTOHDR*         pHdr        = (ATEMRAS_T_PROTOHDR*) pbyTx;
    ATEMRAS_T_CMDTYPE           eCmdType    = (ATEMRAS_T_CMDTYPE) ATEMRAS_PROTOHDR_GET_CMD(pHdr);
#endif

    VERBOSEOUT(3, (">CAtEmRasCmdQueue(this=0x%08X)::AddEntry(dwSegNr=%d, dwTxLen=%d, pbyTx=0x%x, pdwRxLen=0x%x (%d), pbyRx=0x%x, pdwIdx=0x%x), CmdType=%s %s\n",
        this, dwSeqNr, dwTxLen, pbyTx, pdwRxLen, (EC_NULL == pdwRxLen)?0:(*pdwRxLen), pbyRx, pdwIdx,
        ATEMRAS_T_CMDTYPE_TEXT(eCmdType),
              ( emrascmd_emapicommand == eCmdType ? ATEMRAS_T_APICMDTYPE_TEXT((ATEMRAS_T_APICMDTYPE)EC_GET_FRM_DWORD(ATEMRAS_PROTOHDR_DATA(pHdr))) : "" )));

    if (EC_NULL == pdwIdx)
    {
        bAutoDisp = EC_TRUE;
    }

    if (bAutoDisp && (((EC_NULL != pdwRxLen) && (0 != *pdwRxLen)) || (EC_NULL != pbyRx)))
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::AddEntry: Error 0x%x (auto-dispose needs allocated RX-buffer)!\n", EC_E_INVALIDPARM));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    /* Administrative Note:
     * pbyTx&dwTxLen != 0   :   send data   (A)
     * pbyRx&dwRxLen != 0   :   receive Data(B)
     * A    B
     * 0    0   invalid -> error
     * 0    1   invalid -> error
     * 1    0   send data, entry is done, when send.
     * 1    1   send data, entry is done when response is received.
     */

    if ((EC_NULL == pbyTx) || (0 == dwTxLen))
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::AddEntry: if( (EC_NULL == pbyTx) || (0 == dwTxLen) ) goto Exit, 0x%x\n", EC_E_INVALIDPARM));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if ((((EC_NULL == pdwRxLen) || (0 == *pdwRxLen)) && (EC_NULL != pbyRx)) 
     || (((EC_NULL != pdwRxLen) && (0 != *pdwRxLen)) && (EC_NULL == pbyRx)))
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::AddEntry: if((((EC_NULL == pdwRxLen)||(0==EC_GET_FRM_DWORD(pdwRxLen)))&&(EC_NULL != pbyRx))||((EC_NULL != pdwRxLen)&&(0 != EC_GET_FRM_DWORD(pdwRxLen) && (EC_NULL == pbyRx))) ) goto Exit, 0x%x\n", EC_E_INVALIDPARM));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* ok so far. Seek for a free list element */
    OsLock(m_pvListLock);
    bListLock = EC_TRUE;

    for (dwScan = 0; (dwScan < m_dwListLen) && (EC_NULL == ptAlloc); dwScan++)
    {
        dwCurIdx = (m_dwNextFreeIndex + dwScan) % m_dwListLen;
        ptCur = &(m_aList[dwCurIdx]);

        if (!ptCur->bEntryUsed)
        {
            /* found a free entry */
            ptAlloc = ptCur;
            if (EC_NULL != pdwIdx) 
            {
                *pdwIdx = dwCurIdx;
            }
            
            break;
        }
    }

    if (EC_NULL == ptAlloc)
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::AddEntry: if( EC_NULL == ptAlloc ) goto Exit, 0x%x\n", EC_E_NOMEMORY));
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    m_dwNextFreeIndex = (m_dwNextFreeIndex + 1) % m_dwListLen;
            
    ptAlloc->dwSequenceNr   = dwSeqNr;
    ptAlloc->dwSendSize     = dwTxLen;
    ptAlloc->pdwRecvSize    = pdwRxLen;
    ptAlloc->pbyTxBufferHdr = pbyTx;
    ptAlloc->pbyRxBuffer    = pbyRx;
    ptAlloc->bEntrySent     = EC_FALSE;
    ptAlloc->bEntryRcvd     = EC_FALSE;
    ptAlloc->bEntryUsed     = EC_TRUE;
    ptAlloc->bAutoDispose   = bAutoDisp;
    ptAlloc->bCommCanceled  = EC_FALSE;

    bListLock = EC_FALSE;
    OsUnlock(m_pvListLock);

    dwRetVal = EC_E_NOERROR;
Exit:
    if( bListLock )
    {
        bListLock = EC_FALSE;
        OsUnlock(m_pvListLock);
    }

    VERBOSEOUT((EC_E_NOERROR == dwRetVal)?3:1, ("<CAtEmRasCmdQueue(this=0x%08X)::AddEntry(*pdwIdx=0x%08X)=0x%x\n", 
        this, (pdwIdx?*pdwIdx:0), dwRetVal));
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Remove List entry (mark as free).
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmRasCmdQueue::RemoveEntry(
    EC_T_DWORD dwIndex      /**< [in]   List index (handle) */
                                        )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    ATEMRAS_PT_CMDQUEUE_ELEMENT pElement    = EC_NULL;
    VERBOSEOUT(3, (">CAtEmRasCmdQueue::RemoveEntry(0x%x, dwIndex=%d)\n", this, dwIndex));

    OsLock(m_pvListLock);
    if (0 == m_dwListLen)
    {
        VERBOSEOUT(1, ("CAtEmRasCmdQueue::RemoveEntry: List size 0! 0x%x\n", EC_E_ERROR));
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }
    if (dwIndex >= m_dwListLen)
    {
        VERBOSEOUT(1, ("CAtEmRasCmdQueue::RemoveEntry: dwIndex %d out of range 0 ... %d! 0x%x\n", dwIndex, (m_dwListLen-1), EC_E_INVALIDINDEX));
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }

    pElement = &(m_aList[dwIndex]);

    OsMemset(pElement, 0, sizeof(ATEMRAS_T_CMDQUEUE_ELEMENT));

    dwRetVal = EC_E_NOERROR;
Exit:
    OsUnlock(m_pvListLock);

    VERBOSEOUT((EC_E_NOERROR == dwRetVal)?3:1, ("<CAtEmRasCmdQueue::RemoveEntry(0x%x)=0x%x\n", this, dwRetVal));
    return dwRetVal;
}

/*! \fn EC_T_DWORD CAtEmRasCmdQueue::IsEntryDone( EC_T_DWORD dwIndex ) 
 *  \brief                Is entry at dwIndex completely processed?
 *  \return 
 *    EC_E_NOERROR: Finished without error\n
 *    EC_E_BUSY:    Not finished yet\n\n
 *
 *    Errors:\n
 *    EC_E_NOTREADY:                The element at dwIndex is currently not used.\n
 *    EC_E_INVALIDINDEX:            dwIndex exceeds list size.\n
 *    EC_E_MASTERCORE_INACCESSIBLE: Cmd was cancelled because connection was terminated.
 */
EC_T_DWORD CAtEmRasCmdQueue::IsEntryDone(
    EC_T_DWORD dwIndex      /**< [in]   List index (handle) */
                                        )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    ATEMRAS_PT_CMDQUEUE_ELEMENT pElement    = EC_NULL;
    EC_T_BOOL                   bFinished   = EC_FALSE;

    VERBOSEOUT(3, (">CAtEmRasCmdQueue::IsEntryDone(0x%x, dwIndex=%d)\n", this, dwIndex));

    OsLock(m_pvListLock);
    if (dwIndex >= m_dwListLen)
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::IsEntryDone: if (dwIndex >= m_dwListLen) goto Exit, 0x%x\n", EC_E_INVALIDINDEX));
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }
    
    pElement = &(m_aList[dwIndex]);
    
    if (!pElement->bEntryUsed)
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::IsEntryDone: if( !pElement->bEntryUsed ) goto Exit, 0x%x\n", EC_E_NOTREADY));
        dwRetVal = EC_E_NOTREADY;
        goto Exit;
    }

    if (pElement->bCommCanceled)
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::IsEntryDone: if( pElement->bCommCanceled ) goto Exit, 0x%x\n", EC_E_MASTERCORE_INACCESSIBLE));
        dwRetVal = EC_E_MASTERCORE_INACCESSIBLE;
        goto Exit;
    }

    bFinished = pElement->bEntrySent && pElement->bEntryRcvd;
    
    if (!bFinished)
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::IsEntryDone: if( !bFinished ) goto Exit, 0x%x\n", EC_E_BUSY));
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsUnlock(m_pvListLock);

    VERBOSEOUT(((EC_E_NOERROR == dwRetVal)||(EC_E_BUSY == dwRetVal))?3:1, 
        ("<CAtEmRasCmdQueue::IsEntryDone(0x%x)=0x%x\n", this, dwRetVal));
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Find entry by Seq.Nr. ("InvokeId").

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmRasCmdQueue::FindEntry(
    EC_T_DWORD  dwSequenceNr,   /**< [in]   Sequence Nr. to find */
    EC_T_DWORD* pdwIdx        /**< [out]  Buffer to hold List index (handle) */
                                      )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    EC_T_DWORD                  dwScan      = 0;
    EC_T_DWORD                  dwCurIdx    = 0;
    EC_T_BOOL                   bFound      = EC_FALSE;
    ATEMRAS_PT_CMDQUEUE_ELEMENT pTmp        = EC_NULL;
    
    VERBOSEOUT(3, (">CAtEmRasCmdQueue::FindEntry(0x%x, dwSequenceNr=%d, pdwIdx=0x%x)\n", this, dwSequenceNr, pdwIdx));

    if (EC_NULL == pdwIdx)
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::FindEntry: pdwIdx is EC_NULL!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    OsLock(m_pvListLock);
    for (dwScan = 0; dwScan < m_dwListLen; dwScan++)
    {
        dwCurIdx = (m_dwNextRecvIndex + dwScan) % m_dwListLen;

        pTmp = &(m_aList[dwCurIdx]);
        if ((dwSequenceNr == pTmp->dwSequenceNr) && pTmp->bEntryUsed)
        {
            *pdwIdx = dwCurIdx;
            bFound = EC_TRUE;
            break;
        }
    }
    
    OsUnlock(m_pvListLock);

    if (!bFound)
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::FindEntry: dwSequenceNr %d not found!\n", dwSequenceNr));
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    VERBOSEOUT((EC_E_NOERROR == dwRetVal)?3:1, ("<CAtEmRasCmdQueue::FindEntry(0x%x)=0x%x\n", this, dwRetVal));
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get Rx Entry Pointers.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmRasCmdQueue::GetRxEntry(
    EC_T_DWORD      dwIndex,        /**< [in]   List index (handle) */
    EC_T_PBYTE*     ppbyRxBuffer,   /**< [out]  Pointer to hold Rx Buffer Reference */
    EC_T_DWORD**    ppdwRxLen       /**< [out]  Pointer to hold Rx Length Reference */
                                       )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    ATEMRAS_PT_CMDQUEUE_ELEMENT pElement    = EC_NULL;
    
    VERBOSEOUT(3, (">CAtEmRasCmdQueue::GetRxEntry(0x%x, dwIndex=%d, ppbyRxBuffer=0x%x, ppdwRxLen=0x%x)\n", 
        this, dwIndex, ppbyRxBuffer, ppdwRxLen
              ));

    OsLock(m_pvListLock);
    if (dwIndex >= m_dwListLen)
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::GetRxEntry: if (dwIndex >= m_dwListLen) goto Exit, 0x%x\n", EC_E_INVALIDINDEX));
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }

    if ((EC_NULL == ppbyRxBuffer) || (EC_NULL == ppdwRxLen))
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::GetRxEntry: if( (EC_NULL == ppbyRxBuffer) || (EC_NULL == ppdwRxLen) ) goto Exit, 0x%x\n", EC_E_INVALIDPARM));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    pElement = &(m_aList[dwIndex]);
    
    *ppbyRxBuffer = pElement->pbyRxBuffer;
    *ppdwRxLen    = pElement->pdwRecvSize;

    dwRetVal = EC_E_NOERROR;
Exit:
    OsUnlock(m_pvListLock);
    
    VERBOSEOUT((EC_E_NOERROR == dwRetVal)?3:1, ("<CAtEmRasCmdQueue(this=0x%08X)::GetRxEntry()=0x%08X: pElement=0x%08X, pbyRecvData=0x%08X, pdwRecvSize=%08X\n", this,
        dwRetVal, pElement, (*ppbyRxBuffer), (*ppdwRxLen)));
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get Tx Entry Pointers.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmRasCmdQueue::GetTxEntry(
    EC_T_DWORD      dwIndex,        /**< [in]   List index (handle) */
    EC_T_PBYTE*     ppbyTxBuffer,   /**< [out]  Pointer to hold Tx Buffer Reference */
    EC_T_DWORD*     pdwTxLen,       /**< [out]  Pointer to hold Tx Length Reference */
    EC_T_DWORD*     pdwSeqNr        /**< [out]  Pointer to hold SeqNr */
                                       )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    ATEMRAS_PT_CMDQUEUE_ELEMENT pElement    = EC_NULL;
    
    VERBOSEOUT(3, (">CAtEmRasCmdQueue::GetTxEntry(0x%x, dwIndex=%d, ppbyTxBuffer=0x%x, pdwTxLen=0x%x)\n", 
        this, dwIndex, ppbyTxBuffer, pdwTxLen
              ));
    OsLock(m_pvListLock);
    if (dwIndex >= m_dwListLen)
    {
    	OsDbgAssert( EC_FALSE );
        VERBOSEOUT(2, ("CAtEmRasClntStore::GetTxEntry: if (dwIndex >= m_dwListLen) goto Exit, 0x%x\n", EC_E_INVALIDINDEX));
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }

    if ((EC_NULL == ppbyTxBuffer) || (EC_NULL == pdwTxLen))
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::GetTxEntry: if( (EC_NULL == ppbyTxBuffer) || (EC_NULL == pdwTxLen) ) goto Exit, 0x%x\n", EC_E_INVALIDPARM));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    pElement = &(m_aList[dwIndex]);
    
    *ppbyTxBuffer = pElement->pbyTxBufferHdr;
    *pdwTxLen     = pElement->dwSendSize;
    *pdwSeqNr     = pElement->dwSequenceNr;

    dwRetVal = EC_E_NOERROR;
Exit:
    OsUnlock(m_pvListLock);
    
    VERBOSEOUT((EC_E_NOERROR == dwRetVal)?3:1, ("<CAtEmRasCmdQueue::GetTxEntry(0x%x)=0x%x\n", this, dwRetVal));
    return dwRetVal;
}               

/***************************************************************************************************/
/**
\brief  Mark Receive Done.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmRasCmdQueue::SetRxDone(
    EC_T_DWORD dwIndex      /**< [in]   List index (handle) */
                                     )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    ATEMRAS_PT_CMDQUEUE_ELEMENT pElement    = EC_NULL;
    
    VERBOSEOUT(3, (">CAtEmRasCmdQueue::SetRxDone(0x%x, dwIndex=%d)\n", 
        this, dwIndex
              ));

    OsLock(m_pvListLock);
    if (dwIndex >= m_dwListLen)
    {
        VERBOSEOUT(2, ("CAtEmRasClntStore::GetTxEntry: if (dwIndex >= m_dwListLen) goto Exit, 0x%x\n", EC_E_INVALIDINDEX));
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }
    
    pElement = &(m_aList[dwIndex]);
    
    m_dwNextRecvIndex = ((dwIndex+1)%m_dwListLen);

    pElement->bEntryRcvd = EC_TRUE;
    
    dwRetVal = EC_E_NOERROR;
Exit:
    OsUnlock(m_pvListLock);
    
    VERBOSEOUT((EC_E_NOERROR == dwRetVal)?3:1, ("<CAtEmRasCmdQueue::SetRxDone(0x%x)=0x%x\n", this, dwRetVal));
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Mark Transmit Done.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmRasCmdQueue::SetTxDone(
    EC_T_DWORD dwIndex      /**< [in]   List index (handle) */
                                     )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    ATEMRAS_PT_CMDQUEUE_ELEMENT pElement    = EC_NULL;
    EC_T_VOID*                  pbySendData = EC_NULL;
    
    VERBOSEOUT(3, (">CAtEmRasCmdQueue::SetTxDone(0x%x, dwIndex=%d)\n", this, dwIndex));

    OsLock(m_pvListLock);
    if (dwIndex >= m_dwListLen)
    {
    	OsDbgAssert(EC_FALSE);
        VERBOSEOUT(2, ("CAtEmRasClntStore::SetTxDone: if (dwIndex >= m_dwListLen) goto Exit, 0x%x\n", EC_E_INVALIDINDEX));
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }
    
    pElement = &(m_aList[dwIndex]);
    if (EC_NULL == pElement->pbyTxBufferHdr)
    {
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }
    
    pElement->byCmdType = ATEMRAS_PROTOHDR_GET_CMD((ATEMRAS_PT_PROTOHDR)pElement->pbyTxBufferHdr);

    m_dwNextSendIndex = ((dwIndex+1)%m_dwListLen);

    pElement->bEntrySent = EC_TRUE;

    if ((pElement->bAutoDispose) 
     && (EC_NULL == pElement->pbyRxBuffer) 
     && ((EC_NULL == pElement->pdwRecvSize) || (0 == *(pElement->pdwRecvSize))))
    {
        pbySendData = pElement->pbyTxBufferHdr;
        OsMemset(pElement, 0, sizeof(ATEMRAS_T_CMDQUEUE_ELEMENT));
    }
    
    dwRetVal = EC_E_NOERROR;
Exit:
    OsUnlock(m_pvListLock);
    if (pbySendData != EC_NULL)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmRasCmdQueue::SetTxDone pbySendData", pbySendData, 0);
        OsFree(pbySendData);
        pbySendData = EC_NULL;
    }
    
    OsDbgAssert(EC_E_ERROR != dwRetVal);

    VERBOSEOUT((EC_E_NOERROR == dwRetVal)?3:1, ("<CAtEmRasCmdQueue::SetTxDone(0x%x)=0x%x\n", this, dwRetVal));
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Invalidate (cancel) all pending requests (used entries).
*/
EC_T_VOID CAtEmRasCmdQueue::InvalidatePending(EC_T_VOID)
{
    ATEMRAS_T_CMDQUEUE_ELEMENT* pElement    = EC_NULL;
    EC_T_DWORD                  dwIdx       = 0;

    VERBOSEOUT(3, (">CAtEmRasCmdQueue::InvalidatePending(0x%x)\n", this));

    OsLock(m_pvListLock);

    for( dwIdx = 0; dwIdx < m_dwListLen; dwIdx++ )
    {
        pElement = &(m_aList[dwIdx]);

        if( pElement->bEntryUsed )
        {
            pElement->bCommCanceled = EC_TRUE;
        }
    }

    OsUnlock(m_pvListLock);
    VERBOSEOUT(3, ("<CAtEmRasCmdQueue::InvalidatePending(0x%x)\n", this));
}

EC_T_VOID CAtEmRasCmdQueue::SetCmdType(EC_T_DWORD dwIdx, EC_T_BYTE byCmdType)
{
    ATEMRAS_T_CMDQUEUE_ELEMENT* pElement = EC_NULL;

    OsLock(m_pvListLock);

    pElement = &m_aList[dwIdx];

    if (!pElement->bEntryUsed)
    {
        goto Exit;
    }

    pElement->byCmdType = byCmdType;

Exit:
    OsUnlock(m_pvListLock);
}

EC_T_BYTE CAtEmRasCmdQueue::GetCmdType(EC_T_DWORD dwIdx)
{
    EC_T_BYTE byRetVal = 0;
    ATEMRAS_T_CMDQUEUE_ELEMENT* pElement = EC_NULL;

    OsLock(m_pvListLock);

    pElement = &m_aList[dwIdx];

    if (!pElement->bEntryUsed)
    {
        byRetVal = 0;
        goto Exit;
    }

    byRetVal = pElement->byCmdType;

Exit:
    OsUnlock(m_pvListLock);
    return byRetVal;
}

/***************************************************************************************************/
/**
\brief  Get Next Entry Index to send.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmRasCmdQueue::GetNextSendId(
    EC_T_DWORD* pdwIdx    /**< [out]  Buffer to hold index (handle) */
                                          )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    EC_T_BOOL                   bFound      = EC_FALSE;
    EC_T_DWORD                  dwScan      = 0;
    EC_T_DWORD                  dwCurIdx    = 0;
    ATEMRAS_PT_CMDQUEUE_ELEMENT pElement    = EC_NULL;

    VERBOSEOUT(3, (">CAtEmRasCmdQueue::GetNextSendId(0x%x, pdwIdx=0x%x)\n", this, pdwIdx));

    if (EC_NULL == pdwIdx)
    {
        VERBOSEOUT(2, ("CAtEmRasCmdQueue::GetNextSendId: pdwIdx is EC_NULL!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    OsLock(m_pvListLock);

    for (dwScan = 0; dwScan < m_dwListLen; dwScan++)
    {
        dwCurIdx = (m_dwNextSendIndex + dwScan) % m_dwListLen;

        pElement = &(m_aList[dwCurIdx]);

        if (pElement->bEntryUsed && !pElement->bEntrySent && !pElement->bCommCanceled)
        {
            bFound = EC_TRUE;
            break;
        }
    }

    OsUnlock(m_pvListLock);

    if (!bFound)
    {
        VERBOSEOUT(3, ("CAtEmRasCmdQueue::GetNextSendId: Nothing to send.\n"));
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }

    *pdwIdx = dwCurIdx;

    dwRetVal = EC_E_NOERROR;
Exit:
    VERBOSEOUT(((EC_E_NOERROR == dwRetVal) || (EC_E_NOTFOUND == dwRetVal))?3:1, 
        ("<CAtEmRasCmdQueue(this=0x%08X)::GetNextSendId(dwCurIdx=0x%08X)=0x%08X.\n", this, dwCurIdx, dwRetVal));
    return dwRetVal;
}

/*-END OF SOURCE FILE--------------------------------------------------------*/
