/*-----------------------------------------------------------------------------
* AtemRasRemoteProtocol.cpp
* Copyright                acontis technologies GmbH, Weingarten, Germany
* Response                 Paul Bussmann
* Description              (De-)serilize RAS commands
*---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "AtemRasRemoteProtocol.h"
#include <AtEmRasError.h>
#include <AtEmRasType.h>
#include "EcCommon.h"

/*-FUNCTION-DEFINITIONS------------------------------------------------------*/
const EC_T_CHAR* ATEMRAS_T_CMDTYPE_TEXT( ATEMRAS_T_CMDTYPE eVal )
{
    switch (eVal)
    {
    case emrascmd_nop:
        return "NOP";
    case emrascmd_idle:
        return "Idle";
    case emrascmd_connect:
        return "Connect";
    case emrascmd_connect_oem:
        return "Connect (OEM)";
    case emrascmd_disconnect:
        return "Disconnect";
    case emrascmd_emapicommand:
        return "emapicommand";
    case emrascmd_emapinotification:
        return "emapinotification";
    default:
        return "Unknown CMDTYPE!";
    }
}

const EC_T_CHAR* ATEMRAS_T_APICMDTYPE_TEXT( ATEMRAS_T_APICMDTYPE eVal )
{
    switch (eVal)
    {
    case emrasapicmd_nop:
        return "nop";
    case emrasapicmd_emStart:
        return "Start";
    case emrasapicmd_emStop:
        return "Stop";
    case emrasapicmd_emiocontrol:
        return "iocontrol";
    case emrasapicmd_emGetSlaveId:
        return "GetSlaveId";
    case emrasapicmd_emGetSlaveIdAtPosition:
        return "GetSlaveIdAtPosition";
    case emrasapicmd_emGetSlaveProp:
        return "GetSlaveProp";
    case emrasapicmd_emGetSlaveState:
        return "GetSlaveState";
    case emrasapicmd_emSetSlaveState:
        return "SetSlaveState";
    case emrasapicmd_emTferSingleRawCmd:
        return "TferSingleRawCmd";
    case emrasapicmd_emQueueRawCmd:
        return "QueueRawCmd";
    case emrasapicmd_emGetNumConfiguredSlaves:
        return "GetNumConfiguredSlaves";
    case emrasapicmd_emMbxTferCreate:
        return "MbxTferCreate";
    case emrasapicmd_emMbxTferDelete:
        return "MbxTferDelete";
    case emrasapicmd_emCoeSdoDownloadReq:
        return "CoeSdoDownloadReq";
    case emrasapicmd_emCoeSdoUploadReq:
        return "CoeSdoUploadReq";
    case emrasapicmd_emCoeGetODList:
        return "CoeGetODList";
    case emrasapicmd_emCoeGetObjectDesc:
        return "CoeGetObjectDesc";
    case emrasapicmd_emCoeGetEntryDesc:
        return "CoeGetEntryDesc";
    case emrasapicmd_emConfigureMaster:
        return "ConfigureMaster";
    case emrasapicmd_emSetMasterState:
        return "SetMasterState";
    case emrasapicmd_emGetMasterState:
        return "GetMasterState";

        /*
        emrasapicmd_emInitMaster                =   0x0001,
        emrasapicmd_emDeinitMaster              =   0x0002,
        emrasapicmd_emClkTickAnnounce           =   0x0005,
        emrasapicmd_emRefreshProcessData        =   0x0007,

        emrasapicmd_emCoeRxPdoTfer              =   0x0017,
        emrasapicmd_emExecJob                   =   0x001A,
        */

    case emrasapicmd_emGetProcessData:
        return "GetProcessData";
    case emrasapicmd_emSetProcessData:
        return "SetProcessData";

        /*
        emrasapicmd_emFoeDownloadReq            =   0x0023,
        emrasapicmd_emFoeUploadReq              =   0x0024,
        */

    case emrasapicmd_emFoeDownload:
        return "FoeDownload";
    case emrasapicmd_emFoeDownloadReq:
        return "FoeDownloadReq";
    case emrasapicmd_emFoeUpload:
        return "FoeUpload";
    case emrasapicmd_emFoeUploadReq:
        return "FoeUploadReq";
    case emrasapicmd_emCoeSdoDownload:
        return "CoeSdoDownload";
    case emrasapicmd_emCoeSdoUpload:
        return "CoeSdoUpload";
    case emrasapicmd_emGetNumConnectedSlaves:
        return "GetNumConnectedSlaves";
    case emrasapicmd_emResetSlavecontroller:
        return "ResetSlavecontroller";
    case emrasapicmd_emGetSlaveInfo:
        return "GetSlaveInfo";
    case emrasapicmd_emIsSlavePresent:
        return "IsSlavePresent";
    case emrasapicmd_emGetSlaveInpVarInfoNumOf:
        return "GetSlaveInpVarInfoNumOf";
    case emrasapicmd_emGetSlaveOutpVarInfoNumOf:
        return "GetSlaveOutpVarInfoNumOf";
    case emrasapicmd_emGetSlaveInpVarInfo:
        return "GetSlaveInpVarInfo";
    case emrasapicmd_emGetFixedAddr:
        return "GetFixedAddr";
    case emrasapicmd_emGetSlaveOutpVarInfo:
        return "GetSlaveOutpVarInfo";
    case emrasapicmd_emFindOutpVarByName:
        return "FindOutpVarByName";
    case emrasapicmd_emFindInpVarByName:
        return "FindInpVarByName";
    case emrasapicmd_emGetProcessDataBits:
        return "GetProcessDataBits";
    case emrasapicmd_emSetProcessDataBits:
        return "SetProcessDataBits";
    case emrasapicmd_emReadSlaveEEPRom:
        return "ReadSlaveEEPRom";
    case emrasapicmd_emWriteSlaveEEPRom:
        return "WriteSlaveEEPRom";
    case emrasapicmd_emAssignSlaveEEPRom:
        return "AssignSlaveEEPRom";
    case emrasapicmd_emReloadSlaveEEPRom:
        return "ReloadSlaveEEPRom";
    case emrasapicmd_emSoeRead:
        return "SoeRead";
    case emrasapicmd_emSoeWrite:
        return "SoeWrite";
    case emrasapicmd_emSoeAbortProcCmd:
        return "SoeAbortProcCmd";
    case emrasapicmd_emAoeWrite:
        return "AoeWrite";
    case emrasapicmd_emAoeWriteReq:
        return "AoeWriteReq";
    case emrasapicmd_emAoeRead:
        return "AoeRead";
    case emrasapicmd_emAoeReadReq:
        return "AoeReadReq";
    case emrasapicmd_emAoeGetSlaveNetId:
        return "AoeGetSlaveNetId";
    case emrasapicmd_emAoeWriteControl:
        return "AoeWriteControl";
    case emrasapicmd_emGetNumConnectedSlavesMain:
        return "GetNumConnectedSlavesMain";
    case emrasapicmd_emGetNumConnectedSlavesRed:
        return "GetNumConnectedSlavesRed";
    case emrasapicmd_emNotifyApp:
        return "NotifyApp";
    case emrasapicmd_emGetSlaveInpVarInfoEx:
        return "GetSlaveInpVarInfoEx";
    case emrasapicmd_emGetSlaveOutpVarInfoEx:
        return "GetSlaveOutpVarInfoEx";
    case emrasapicmd_emFindOutpVarByNameEx:
        return "FindOutpVarByNameEx";
    case emrasapicmd_emFindInpVarByNameEx:
        return "FindInpVarByNameEx";
    case emrasapicmd_emGetCfgSlaveInfo:
        return "GetCfgSlaveInfo";
    case emrasapicmd_emGetBusSlaveInfo:
        return "GetBusSlaveInfo";
    case emrasapicmd_emReadSlaveIdentification:
        return "ReadSlaveIdentification";
    case emrasapicmd_emSetSlaveDisabled:
        return "SetSlaveDisabled";
    case emrasapicmd_emSetSlaveDisconnected:
        return "SetSlaveDisconnected";
    case emrasapicmd_emRescueScan:
        return "RescueScan";
    case emrasapicmd_emGetMasterInfo:
        return "GetMasterInfo";
    case emrasapicmd_emConfigExtend:
        return "ConfigExtend";
    case emrasapicmd_emSetMbxProtocolsSerialize:
        return "SetMbxProtocolsSerialize";
    default:
        return "Unknown APICMDTYPE!";
    }
}

/***************************************************************************************************/
/**
\brief  Constructor of CAtemRasHelper
*/
CAtemRasHelper::CAtemRasHelper() {
    m_dwCmd = 0;
    m_dwTimeout = 0;
    m_dwInstanceID = 0;
    m_dwSize = 0;

    m_pList = EC_NULL;
    m_pLast = EC_NULL;
    m_pTransferBuffer = EC_NULL;
    m_bSkipFree = EC_FALSE;
}

/***************************************************************************************************/
/**
\brief  Destructor of CAtemRasHelper
*/
CAtemRasHelper::~CAtemRasHelper() {
    if (m_bSkipFree == EC_FALSE)
    {
        SafeOsFree(m_pTransferBuffer);
    }

    struct _ATEMRAS_T_HELPER_ELEM* p = m_pList;
    while (p)
    {
        struct _ATEMRAS_T_HELPER_ELEM* tmp = p;
        p = p->pNext;

        SafeOsFree(tmp->pBuffer);
        SafeOsFree(tmp);
    }
}

/***************************************************************************************************/
/**
\brief  Calculates the offset of the index element

\return Calculated offset
*/
EC_T_DWORD CAtemRasHelper::calcOffset(EC_T_DWORD dwIdx)
{
    EC_T_DWORD dwOffset = 0;
    EC_T_DWORD i = 0;

    struct _ATEMRAS_T_HELPER_ELEM* p = m_pList;
    while (p)
    {
        if (i++ == dwIdx+3)
            return dwOffset;

        dwOffset += p->dwBufferSize;
        p = p->pNext;
    }

    return 0; // Error: not found!
}

/***************************************************************************************************/
/**
\brief  Adds item to helper list

\return None.
*/
void CAtemRasHelper::addToList(EC_T_VOID* pData, EC_T_DWORD dwDataSize, EC_T_BOOL fBack)
{
    EC_T_BYTE* pBuffer = (EC_T_BYTE*)OsMalloc(dwDataSize);
    OsMemcpy(pBuffer, pData, dwDataSize);

    struct _ATEMRAS_T_HELPER_ELEM* p = (struct _ATEMRAS_T_HELPER_ELEM*)OsMalloc(sizeof(struct _ATEMRAS_T_HELPER_ELEM));
    p->pBuffer = pBuffer;
    p->dwBufferSize = dwDataSize;
    p->pNext = EC_NULL;

    if (m_pLast == EC_NULL)
    {
        m_pList = p;
        m_pLast = p;
    }
    else
    {
        if (fBack)
        {
            m_pLast->pNext = p;
            m_pLast = p;
        }
        else
        {
            p->pNext = m_pList;
            m_pList = p;
        }
    }
}

/***************************************************************************************************/
/**
\brief  Create Send data for a NACK.

\return EC_E_NOERROR
*/
EC_T_DWORD emrasProtoCreateNackNoAlloc(ATEMRAS_PT_PROTOHDR ptRxHdr,    /**< [in]   Received Header with command to NACK */
                                       EC_T_DWORD          dwCause,    /**< [in]   Cause of NACK */
                                       EC_T_BYTE*          pbyData,    /**< [in]   provided buffer */
                                       EC_T_DWORD*         pdwLen      /**< [out]  provided DWORD is going to carry sizeof allocated data */)
{
    ATEMRAS_PT_PROTOHDR pNackHdr    = EC_NULL;

    pNackHdr = (ATEMRAS_PT_PROTOHDR)pbyData;

    /* copy info from rx Header */
    if (pNackHdr != ptRxHdr)
    {
        OsMemcpy(pNackHdr, ptRxHdr, ATEMRAS_T_PROTOHDR_SIZE);
    }

    ATEMRAS_PROTOHDR_SET_STATUS_ACK(pNackHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_REQ(pNackHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_WAIT(pNackHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_LENGTH(pNackHdr, sizeof(EC_T_DWORD));

    EC_SET_FRM_DWORD(ATEMRAS_PROTOHDR_DATA(pNackHdr), dwCause);

    *pdwLen  = ATEMRAS_T_PROTOHDR_SIZE + sizeof(EC_T_DWORD);
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Create Send data for a NACK.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emrasProtoCreateNack(ATEMRAS_PT_PROTOHDR ptRxHdr,    /**< [in]   Received Header with command to NACK */
                                EC_T_DWORD          dwCause,    /**< [in]   Cause of NACK */
                                EC_T_BYTE**         ppbyData,   /**< [out]  Buffer created herein, has to be freed by callee */
                                EC_T_DWORD*         pdwLen      /**< [out]  provided DWORD is going to carry sizeof allocated data */)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwBufferLen = ATEMRAS_T_PROTOHDR_SIZE + sizeof(EC_T_DWORD);

    if ((EC_NULL == ppbyData) || (EC_NULL == pdwLen))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    *ppbyData = (EC_T_BYTE*)OsMalloc(dwBufferLen);
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "emrasProtoCreateNack *ppbyData", *ppbyData, dwBufferLen);
    if (EC_NULL == *ppbyData)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    emrasProtoCreateNackNoAlloc(ptRxHdr, dwCause, *ppbyData, pdwLen);

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Create Send data for a ACK.

\return EC_E_NOERROR
*/
EC_T_DWORD emrasProtoCreateAckNoAlloc(ATEMRAS_PT_PROTOHDR ptRxHdr,    /**< [in]   Received Header with command to ACK */
                                      EC_T_DWORD          dwCause,    /**< [in]   Cause of ACK */
                                      EC_T_BYTE*          pbyData,    /**< [in]   provided buffer */
                                      EC_T_DWORD*         pdwLen,     /**< [out]  provided DWORD is going to carry sizeof allocated data */
                                      EC_T_BOOL           bWait       /**< [in]   Wait on client site for next ack ? */)
{
    ATEMRAS_PT_PROTOHDR pAckHdr = (ATEMRAS_PT_PROTOHDR)pbyData;

    /* copy info from rx Header */
    OsMemcpy(pAckHdr, ptRxHdr, ATEMRAS_T_PROTOHDR_SIZE);

    ATEMRAS_PROTOHDR_SET_STATUS_ACK(pAckHdr, EC_TRUE);
    ATEMRAS_PROTOHDR_SET_STATUS_REQ(pAckHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_WAIT(pAckHdr, ((EC_T_BYTE)bWait));
    ATEMRAS_PROTOHDR_SET_LENGTH(pAckHdr, sizeof(EC_T_DWORD));

    EC_SET_FRM_DWORD(ATEMRAS_PROTOHDR_DATA(pAckHdr), dwCause);

    *pdwLen = ATEMRAS_T_PROTOHDR_SIZE + sizeof(EC_T_DWORD);
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Create Send data for a ACK.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emrasProtoCreateAck(ATEMRAS_PT_PROTOHDR ptRxHdr,    /**< [in]   Received Header with command to ACK */
                               EC_T_DWORD          dwCause,    /**< [in]   Cause of ACK */
                               EC_T_BYTE**         ppbyData,   /**< [out]  Buffer created herein, has to be freed by callee */
                               EC_T_DWORD*         pdwLen,     /**< [out]  provided DWORD is going to carry sizeof allocated data */
                               EC_T_BOOL           bWait       /**< [in]   Wait on client site for next ACK? */)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwBufferLen = ATEMRAS_T_PROTOHDR_SIZE + sizeof(EC_T_DWORD);

    *ppbyData = (EC_T_BYTE*)OsMalloc(dwBufferLen);
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "emrasProtoCreateAck *ppbyData", *ppbyData, dwBufferLen);
    if (EC_NULL == *ppbyData)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    emrasProtoCreateAckNoAlloc(ptRxHdr, dwCause, *ppbyData, pdwLen, bWait);

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Create Send data for disconnect.

\return EC_E_NOERROR
*/
EC_T_DWORD emrasProtoCreateDisconnectNoAlloc(   EC_T_DWORD    dwCookie,   /**< [in]   Current session cookie */
                                                EC_T_DWORD    dwCause,    /**< [in]   Disconnection cause */
                                                EC_T_BYTE*    pbyData,    /**< [in]   Buffer */
                                                EC_T_DWORD*   pdwLen      /**< [out]  provided DWORD is going to carry sizeof allocated data */)
{
    EC_T_DWORD          dwBufferLen = ATEMRAS_T_PROTOHDR_SIZE + sizeof(EC_T_DWORD);
    ATEMRAS_PT_PROTOHDR pDiscHdr    = EC_NULL;

    OsMemset(pbyData, 0, dwBufferLen);

    pDiscHdr = (ATEMRAS_PT_PROTOHDR)pbyData;

    ATEMRAS_PROTOHDR_SET_COOKIE(pDiscHdr, dwCookie);
    ATEMRAS_PROTOHDR_SET_CMD(pDiscHdr, ((EC_T_BYTE)emrascmd_disconnect));
    ATEMRAS_PROTOHDR_SET_STATUS_ACK(pDiscHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_REQ(pDiscHdr, EC_TRUE);
    ATEMRAS_PROTOHDR_SET_STATUS_WAIT(pDiscHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_LENGTH(pDiscHdr, sizeof(EC_T_DWORD));

    EC_SET_FRM_DWORD(ATEMRAS_PROTOHDR_DATA(pDiscHdr), dwCause);

    *pdwLen  = dwBufferLen;
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Create Send data for connect.

\return EC_E_NOERROR
*/
EC_T_DWORD emrasProtoCreateConnectNoAlloc(  EC_T_DWORD  dwCookie,   /**< [in]   Cookie  to insert */
                                            EC_T_DWORD  dwVersion,  /**< [in]   Instance version */
                                            EC_T_BYTE*  pbyData,    /**< [out]  Buffer */
                                            EC_T_DWORD* pdwLen      /**< [out]  provided DWORD is going to carry sizeof allocated data */)
{
    EC_T_DWORD          dwBufferLen = ATEMRAS_T_PROTOHDR_SIZE + sizeof(EC_T_DWORD);
    ATEMRAS_PT_PROTOHDR pConHdr     = EC_NULL;

    OsMemset(pbyData, 0, dwBufferLen);

    pConHdr = (ATEMRAS_PT_PROTOHDR)pbyData;

    ATEMRAS_PROTOHDR_SET_COOKIE(pConHdr, dwCookie);
    ATEMRAS_PROTOHDR_SET_CMD(pConHdr, (EC_T_BYTE)emrascmd_connect);
    ATEMRAS_PROTOHDR_SET_STATUS_ACK(pConHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_REQ(pConHdr, EC_TRUE);
    ATEMRAS_PROTOHDR_SET_STATUS_WAIT(pConHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_LENGTH(pConHdr, sizeof(EC_T_DWORD));

    EC_SET_FRM_DWORD(ATEMRAS_PROTOHDR_DATA(pConHdr), dwVersion);

    *pdwLen  = dwBufferLen;
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Create Send data for disconnect.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emrasProtoCreateDisconnect(EC_T_DWORD    dwCookie,   /**< [in]   Current session cookie */
                                      EC_T_DWORD    dwCause,    /**< [in]   Disconnection cause */
                                      EC_T_BYTE**   ppbyData,   /**< [out]  Buffer created herein, has to be freed by callee */
                                      EC_T_DWORD*   pdwLen      /**< [out]  provided DWORD is going to carry sizeof allocated data */)
{
    EC_T_DWORD          dwRetVal    = EC_E_ERROR;
    EC_T_DWORD          dwBufferLen = ATEMRAS_T_PROTOHDR_SIZE + sizeof(EC_T_DWORD);

    *ppbyData = (EC_T_BYTE*)OsMalloc(dwBufferLen);
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "emrasProtoCreateDisconnect *ppbyData", *ppbyData, dwBufferLen);
    if (EC_NULL == *ppbyData)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRetVal = emrasProtoCreateDisconnectNoAlloc(dwCookie, dwCause, *ppbyData, pdwLen);
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Create Send data for connect (OEM).

\return EC_E_NOERROR
*/
EC_T_DWORD emrasProtoCreateConnectOemNoAlloc(   EC_T_DWORD  dwCookie    /**< [in]   Cookie  to insert */,
                                                EC_T_DWORD  dwVersion   /**< [in]   RAS Client version */,
                                                EC_T_DWORD  dwInstanceID/**< [in]   OEM Master Instance */,
                                                EC_T_UINT64 qwOemKey    /**< [in]   OEM Key */,
                                                EC_T_BYTE*  pbyData     /**< [out]  Buffer */,
                                                EC_T_DWORD* pdwLen      /**< [out]  provided DWORD is going to carry sizeof allocated data */)
{
    EC_T_DWORD                      dwBufferLen = ATEMRAS_T_PROTOHDR_SIZE + ATEMRAS_T_CONNECT_OEM_TRANSFER_SIZE;
    ATEMRAS_T_PROTOHDR*             pConHdr     = (ATEMRAS_T_PROTOHDR*)pbyData;
    ATEMRAS_T_CONNECT_OEM_TRANSFER* pTfer       = (ATEMRAS_T_CONNECT_OEM_TRANSFER*)ATEMRAS_PROTOHDR_DATA(pConHdr);

    OsMemset(pbyData, 0, dwBufferLen);

    ATEMRAS_PROTOHDR_SET_COOKIE(pConHdr, dwCookie);
    ATEMRAS_PROTOHDR_SET_CMD(pConHdr, (EC_T_BYTE)emrascmd_connect_oem);
    ATEMRAS_PROTOHDR_SET_STATUS_ACK(pConHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_REQ(pConHdr, EC_TRUE);
    ATEMRAS_PROTOHDR_SET_STATUS_WAIT(pConHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_LENGTH(pConHdr, ATEMRAS_T_CONNECT_OEM_TRANSFER_SIZE);

    ATEMRAS_CONNECT_OEM_SET_VERSION(pTfer, dwVersion);
    ATEMRAS_CONNECT_OEM_SET_INSTANCE(pTfer, dwInstanceID);
    ATEMRAS_CONNECT_OEM_SET_OEM_KEY(pTfer, qwOemKey);

    *pdwLen  = dwBufferLen;
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Create Send data for connect (OEM).
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emrasProtoCreateConnectOem(  EC_T_DWORD  dwCookie    /**< [in]   Cookie to insert */,
                                        EC_T_DWORD  dwVersion   /**< [in]   RAS Client version */,
                                        EC_T_DWORD  dwInstanceID/**< [in]   OEM Master Instance */,
                                        EC_T_UINT64 qwOemKey    /**< [in]   OEM Key */,
                                        EC_T_BYTE** ppbyData    /**< [out]  Buffer created herein, has to be freed by callee */,
                                        EC_T_DWORD* pdwLen      /**< [out]  provided DWORD is going to carry sizeof allocated data */)
{
    EC_T_DWORD dwRetVal    = EC_E_ERROR;
    EC_T_DWORD dwBufferLen = ATEMRAS_T_PROTOHDR_SIZE + 2 * sizeof(EC_T_DWORD);

    *ppbyData = (EC_T_BYTE*)OsMalloc(dwBufferLen);
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "emrasProtoCreateConnectOem *ppbyData", *ppbyData, dwBufferLen);
    if (EC_NULL == *ppbyData)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRetVal = emrasProtoCreateConnectOemNoAlloc(dwCookie, dwVersion, dwInstanceID, qwOemKey, *ppbyData, pdwLen);
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Create Send data for connect.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emrasProtoCreateConnect(EC_T_DWORD   dwCookie,   /**< [in]   Cookie  to insert */
                                   EC_T_DWORD   dwVersion,  /**< [in]   Instance version */
                                   EC_T_BYTE**  ppbyData,   /**< [out]  Buffer created herein, has to be freed by callee */
                                   EC_T_DWORD*  pdwLen      /**< [out]  provided DWORD is going to carry sizeof allocated data */)
{
    EC_T_DWORD dwRetVal    = EC_E_ERROR;
    EC_T_DWORD dwBufferLen = ATEMRAS_T_PROTOHDR_SIZE + sizeof(EC_T_DWORD);

    if ((EC_NULL == ppbyData) || (EC_NULL == pdwLen))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    *ppbyData = (EC_T_BYTE*)OsMalloc(dwBufferLen);
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "emrasProtoCreateConnect *ppbyData", *ppbyData, dwBufferLen);
    if (EC_NULL == *ppbyData)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRetVal = emrasProtoCreateConnectNoAlloc(dwCookie, dwVersion, *ppbyData, pdwLen);
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Validate logon request.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emrasProtoValidateConnect(ATEMRAS_PT_PROTOHDR pHdr        /**< [in]   Incoming data header */,
                                     EC_T_DWORD          dwVersion   /**< [in]   Instance Version */,
                                     EC_T_DWORD          dwDataLen   /**< [in]   Length of payload data */,
                                     EC_T_BYTE*          pbyData     /**< [in]   Payload data */)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_UNREFPARM(dwDataLen);

    VERBOSEOUT(2, ("emrasProtoValidateConnect(...) Got %s CMD.\n",
        ATEMRAS_T_CMDTYPE_TEXT((ATEMRAS_T_CMDTYPE) ATEMRAS_PROTOHDR_GET_CMD(pHdr))));

    if ((emrascmd_connect != ATEMRAS_PROTOHDR_GET_CMD(pHdr)) &&
        (emrascmd_connect_oem != ATEMRAS_PROTOHDR_GET_CMD(pHdr)))
    {
        dwRetVal = EC_E_INVALIDCMD;
        goto Exit;
    }

    if (emrascmd_connect_oem == ATEMRAS_PROTOHDR_GET_CMD(pHdr))
    {
        EC_T_DWORD dwInstance = ATEMRAS_CONNECT_OEM_GET_INSTANCE((ATEMRAS_T_CONNECT_OEM_TRANSFER*)pbyData);
        EC_T_UINT64 qwOemKey = ATEMRAS_CONNECT_OEM_GET_OEM_KEY((ATEMRAS_T_CONNECT_OEM_TRANSFER*)pbyData);
        dwRetVal = emCheckOemKey(dwInstance, qwOemKey);
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }
    }

    if ((EC_GET_FRM_DWORD(pbyData) & 0xFFFF0000) < (dwVersion & 0xFFFF0000))
    {
        dwRetVal = EMRAS_E_INVALIDVERSION;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Create Idle data.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emrasProtoCreateIdle(EC_T_DWORD          dwCookie,   /**< [in]   Cookie  to insert */
                                ATEMRAS_PT_PROTOHDR pConHdr     /**< [out]  Buffer */)
{
    OsMemset(pConHdr, 0, ATEMRAS_T_PROTOHDR_SIZE);

    ATEMRAS_PROTOHDR_SET_COOKIE(pConHdr, dwCookie);
    ATEMRAS_PROTOHDR_SET_CMD(pConHdr, ((EC_T_BYTE)emrascmd_idle));
    ATEMRAS_PROTOHDR_SET_STATUS_ACK(pConHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_REQ(pConHdr, EC_TRUE);
    ATEMRAS_PROTOHDR_SET_STATUS_WAIT(pConHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_LENGTH(pConHdr, 0);

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Insert CMD Header to existing buffer.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emrasProtoInsertCmdHeader(EC_T_DWORD dwCookie,   /**< [in]   Cookie  to insert */
                                     EC_T_BYTE* pbyData,    /**< [in]   Buffer to insert header */
                                     EC_T_DWORD dwLen       /**< [in]   Total size of buffer */)
{
    EC_T_DWORD          dwRetVal    = EC_E_ERROR;
    ATEMRAS_PT_PROTOHDR pHdr        = EC_NULL;

    if( (EC_NULL == pbyData) || (ATEMRAS_T_PROTOHDR_SIZE > dwLen) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* clear header portion */
    OsMemset(pbyData, 0, ATEMRAS_T_PROTOHDR_SIZE);

    pHdr = (ATEMRAS_PT_PROTOHDR)(pbyData);

    ATEMRAS_PROTOHDR_SET_COOKIE(pHdr, dwCookie);
    ATEMRAS_PROTOHDR_SET_STATUS_ACK(pHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_WAIT(pHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_REQ(pHdr, EC_TRUE);
    ATEMRAS_PROTOHDR_SET_SEQUENCENR(pHdr, 0);   /* real seq.nr is inserted by transmit */
    ATEMRAS_PROTOHDR_SET_CMD(pHdr, emrascmd_emapicommand);
    ATEMRAS_PROTOHDR_SET_LENGTH(pHdr, (EC_T_DWORD)(dwLen-ATEMRAS_T_PROTOHDR_SIZE));

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Insert Protocol header for notification transfer.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emrasProtoInsertNotificationHeader(
    EC_T_DWORD      dwCookie,   /**< [in]   Cookie  to insert */
    EC_T_BYTE*      pbyData,    /**< [in]   Buffer to insert header */
    EC_T_DWORD      dwLen       /**< [in]   Total size of buffer */
    )
{
    EC_T_DWORD          dwRetVal    = EC_E_ERROR;
    ATEMRAS_PT_PROTOHDR pHdr        = EC_NULL;

    if( (EC_NULL == pbyData) || (ATEMRAS_T_PROTOHDR_SIZE > dwLen) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* clear header portion */
    OsMemset(pbyData, 0, ATEMRAS_T_PROTOHDR_SIZE);

    pHdr = (ATEMRAS_PT_PROTOHDR)(pbyData);

    ATEMRAS_PROTOHDR_SET_COOKIE(pHdr, dwCookie);
    ATEMRAS_PROTOHDR_SET_STATUS_ACK(pHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_WAIT(pHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_REQ(pHdr, EC_TRUE);
    ATEMRAS_PROTOHDR_SET_SEQUENCENR(pHdr, 0);   /* real seq.nr is inserted by transmit */
    ATEMRAS_PROTOHDR_SET_CMD(pHdr, emrascmd_emapinotification);
    ATEMRAS_PROTOHDR_SET_LENGTH(pHdr, (EC_T_DWORD)(dwLen-ATEMRAS_T_PROTOHDR_SIZE));

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Merge Header and Cmd Data to single data portion in a new buffer to send aw.

\return EC_E_NOERROR
*/
EC_T_DWORD emrasProtoCreateCmdResponseNoAlloc(
    ATEMRAS_PT_PROTOHDR     pLocHdr,
    EC_T_DWORD              dwLen
    )
{
    /* now modify header bits for sending */
    ATEMRAS_PROTOHDR_SET_STATUS_ACK(pLocHdr, EC_TRUE);     /* assume ack */
    ATEMRAS_PROTOHDR_SET_STATUS_WAIT(pLocHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_REQ(pLocHdr, EC_FALSE);    /* response */
    ATEMRAS_PROTOHDR_SET_LENGTH(pLocHdr, dwLen);

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Merge Header and Cmd Data to single data portion in a new buffer to send aw.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emrasProtoCreateCmdResponse(
                                       ATEMRAS_PT_PROTOHDR     pHdr,
                                       EC_T_DWORD              dwLen,
                                       EC_T_BYTE*              pbyData,
                                       EC_T_BYTE**             ppbyDataNew,
                                       EC_T_DWORD*             pdwLenNew /* out */
                                       )
{
    EC_T_DWORD          dwRetVal    = EC_E_ERROR;
    EC_T_DWORD          dwBufferLen = ATEMRAS_T_PROTOHDR_SIZE + dwLen;
    ATEMRAS_PT_PROTOHDR pLocHdr     = EC_NULL;

    if( (EC_NULL == ppbyDataNew) || (EC_NULL == pdwLenNew) || (EC_NULL == pHdr) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    *ppbyDataNew = (EC_T_BYTE*)OsMalloc(dwBufferLen);
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "emrasProtoCreateCmdResponse *ppbyDataNew", *ppbyData, dwBufferLen);
    if (EC_NULL == *ppbyDataNew)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    OsMemcpy(*ppbyDataNew, pHdr, ATEMRAS_T_PROTOHDR_SIZE);

    pLocHdr = (ATEMRAS_PT_PROTOHDR)(*ppbyDataNew);

    OsMemcpy(ATEMRAS_PROTOHDR_DATA(pLocHdr), pbyData, dwLen);

    /* now modify header bits for sending */
    ATEMRAS_PROTOHDR_SET_STATUS_ACK(pLocHdr, EC_TRUE);     /* assume ack */
    ATEMRAS_PROTOHDR_SET_STATUS_WAIT(pLocHdr, EC_FALSE);
    ATEMRAS_PROTOHDR_SET_STATUS_REQ(pLocHdr, EC_FALSE);    /* response */
    ATEMRAS_PROTOHDR_SET_LENGTH(pLocHdr, dwLen);

    *pdwLenNew = dwBufferLen;
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/*-END OF SOURCE FILE--------------------------------------------------------*/
