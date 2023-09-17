/*-----------------------------------------------------------------------------
 * CAtemRasCmdQueue.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 *---------------------------------------------------------------------------*/

#ifndef INC_CATEMRASCMDQUEUE
#define INC_CATEMRASCMDQUEUE      1

/*-INCLUDES------------------------------------------------------------------*/

#include <AtEmRasType.h>
#include <AtEmRasError.h>

/*-DEFINES-------------------------------------------------------------------*/

#define  MAX_NUMOF_CMDQUEUE_ELEMENTS     200

/*-TYPEDEFS------------------------------------------------------------------*/

class CAtEmRasNotificationStore;

struct _EMMARSH_T_NOTIFICATION_ALLOC;

typedef struct _ATEMRAS_T_CMDQUEUE_ELEMENT
{
    EC_T_BOOL           bEntryUsed;     /**< [in]       List element in use */
    EC_T_BOOL           bEntrySent;     /**< [in]       List element Transmitted */
    EC_T_BOOL           bEntryRcvd;     /**< [in]       List element Response received */

    EC_T_BOOL           bAutoDispose;   /**< [in]       If set, data is disposed after send, not valid
                                          *             with Rx Data buffers !
                                          */
    EC_T_BOOL           bCommCanceled;  /**< [in]       Communication canceled by RAS Stack */

    EC_T_DWORD          dwSequenceNr;   /**< [in]       Seq.Nr. ("InvokeId") TODO: sollte CmdQueue ID (s.u. m_aList) sein */
    EC_T_DWORD          dwSendSize;     /**< [in]       Size of Transmit Data Buffer */
    EC_T_DWORD*         pdwRecvSize;    /**< [in,out]   Size of Receive Data Buffer / received data */
    EC_T_PBYTE          pbyTxBufferHdr; /**< [in]       Buffer holding data to transmit (header) */
    EC_T_PBYTE          pbyRxBuffer;    /**< [in]       Buffer to store receive data, incl. header */
    EC_T_BYTE           byCmdType;      /*              Cache CmdType independendly from pbyTxBufferHdr */
} ATEMRAS_T_CMDQUEUE_ELEMENT, *ATEMRAS_PT_CMDQUEUE_ELEMENT;

/*-CLASS---------------------------------------------------------------------*/

class CAtEmRasCmdQueue
{
public:
    CAtEmRasCmdQueue(void);
    ~CAtEmRasCmdQueue();

    EC_T_DWORD AddEntry( EC_T_DWORD dwSeqNr, 
        EC_T_DWORD dwTxLen, EC_T_PBYTE pbyTx, 
        EC_T_DWORD* pdwRxLen, EC_T_PBYTE pbyRx, EC_T_DWORD* pdwIndex);
    EC_T_DWORD RemoveEntry(EC_T_DWORD dwIndex);
    EC_T_DWORD IsEntryDone(EC_T_DWORD dwIndex);

    EC_T_DWORD FindEntry(EC_T_DWORD dwSequenceNr, EC_T_DWORD* pdwIndex);

    EC_T_DWORD GetNextSendId(EC_T_DWORD* pdwIndex);

    EC_T_DWORD GetRxEntry(EC_T_DWORD dwIndex, EC_T_PBYTE* ppbyRxBuffer, EC_T_DWORD** ppdwRxLen);
    EC_T_DWORD GetTxEntry(EC_T_DWORD dwIndex, 
        EC_T_PBYTE* ppbyTxBuffer, EC_T_DWORD* pdwTxLen, 
        EC_T_DWORD* pdwSeqNr);
    
    EC_T_DWORD SetRxDone(EC_T_DWORD dwIndex);
    EC_T_DWORD SetTxDone(EC_T_DWORD dwIndex);

    EC_T_VOID  LockQueue(EC_T_VOID) { OsLock(m_pvListLock); }
    EC_T_VOID  UnlockQueue(EC_T_VOID) { OsUnlock(m_pvListLock); }

    EC_T_VOID  InvalidatePending(EC_T_VOID);

    EC_T_VOID  SetCmdType(EC_T_DWORD dwSendId, EC_T_BYTE byCmdType);
    EC_T_BYTE  GetCmdType(EC_T_DWORD dwListElement);

private:
    ATEMRAS_T_CMDQUEUE_ELEMENT      m_aList[MAX_NUMOF_CMDQUEUE_ELEMENTS];

    EC_T_DWORD                      m_dwNextFreeIndex;
    EC_T_DWORD                      m_dwNextSendIndex;
    EC_T_DWORD                      m_dwNextRecvIndex;

    EC_T_DWORD                      m_dwListLen;
    EC_T_PVOID                      m_pvListLock;
};

/*-FUNCTION DECLARATIONS-----------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FUNCTIONS DEFINITIONS-----------------------------------------------------*/

#endif /* INC_CATEMRASCMDQUEUE */ 

/*-END OF SOURCE FILE--------------------------------------------------------*/
