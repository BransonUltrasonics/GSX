/*-----------------------------------------------------------------------------
 * AtEmMarshal.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              Perform requests
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/

#include <AtEmRasType.h>
#include "CAtemRasServer.h"
#include <CAtemRasCmdQueue.h>

#include <AtemRasRemoteProtocol.h>
#include "CAtEmMasterStore.h"

#include "AtEmMarshal.h"

/*-DEFINES-------------------------------------------------------------------*/

/*-TYPEDEFS------------------------------------------------------------------*/

/*-CLASS---------------------------------------------------------------------*/

/*-FUNCTION DECLARATION------------------------------------------------------*/
EC_T_DWORD marshalNotify(                EC_T_DWORD              dwCode, 
                                         EC_T_NOTIFYPARMS*       pParms       );

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FUNCTION-DEFINITIONS------------------------------------------------------*/
       
/***************************************************************************************************/
/**
\brief  Main marshaling function.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshal(
    ATEMRAS_T_PROTOHDR*     pHeader,        /**< [in]   received protocol header for response */
    EC_T_DWORD              dwLen,          /**< [in]   Length of data portion */
    EC_T_BYTE*              pbyData         /**< [in]   Data received */
                    )
{
    EC_T_DWORD                  dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                  dwRes               = EC_E_ERROR;
    EC_T_BYTE*                  pbyTransfer         = EC_NULL;
    EC_T_DWORD dwWrtLen            = 0;
    ATEMRAS_T_APICMDTYPE        eCmdType            = emrasapicmd_nop;
    
    VERBOSEOUT(2, (">deMarshal()\n"));

    if ((EC_NULL == m_pMasterStore) || (EC_NULL == m_pCmdQueue) || (0 == dwLen) || (EC_NULL == pbyData) || (EC_NULL == pHeader))
    {
        VERBOSEOUT(1, ("deMarshal(): EC_E_INVALIDPARM!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwWrtLen = ATEMRAS_T_PROTOHDR_SIZE + dwLen;

    /* now work with newly allocated buffer, because it contains all data */
    eCmdType = (ATEMRAS_T_APICMDTYPE)EC_GET_FRM_DWORD(ATEMRAS_PROTOHDR_DATA(pHeader));

    VERBOSEOUT(2, ("deMarshal(): Got %s APICMD with len %d.\n", 
    ATEMRAS_T_APICMDTYPE_TEXT(eCmdType), dwLen));

    switch (eCmdType)
    {
        case emrasapicmd_emiocontrol:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalIoControl(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emConfigureMaster:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalConfigureMaster(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emStart:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalStart(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emSetMasterState:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalSetMasterState(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetMasterState:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetMasterState(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emStop:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalStop(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetNumConfiguredSlaves:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetConfiguredSlaves(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetSlaveIdAtPosition:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetSlvIdAtPos(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetSlaveId:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetSlvId(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetSlaveProp:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetSlaveProp(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetSlaveState:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetSlaveState(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emSetSlaveState:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalSetSlaveState(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emTferSingleRawCmd:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalTferSingleRawCmd(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emQueueRawCmd:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalQueueRawCmd(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emMbxTferCreate:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalMbxTferCreate(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emMbxTferDelete:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalMbxTferDelete(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emCoeSdoDownloadReq:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalCoeSdoDownloadReq(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emCoeSdoUploadReq:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalCoeSdoUploadReq(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emCoeSdoDownload:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalCoeSdoDownload(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emCoeSdoUpload:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalCoeSdoUpload(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emNotifyApp:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalNotifyApp(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emCoeGetODList:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalCoeGetODList(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emCoeGetObjectDesc:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalCoeGetObjectDesc(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emCoeGetEntryDesc:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalCoeGetEntryDesc(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetProcessData:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetProcessData(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emSetProcessData:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalSetProcessData(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emFoeUploadReq:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalFoeUploadReq(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emFoeDownloadReq:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalFoeDownloadReq(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emFoeDownload:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalFoeFileDownload(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emFoeUpload:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalFoeFileUpload(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emAoeWriteReq:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalAoeWriteReq(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emAoeReadReq:
        {
            dwRes = emrasProtoCreateCmdResponse(pHeader, dwLen, pbyData, &pbyTransfer, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            dwRes = deMarshalAoeReadReq(pbyTransfer, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emAoeReadWriteReq:
        {
        } break;
        case emrasapicmd_emAoeWrite:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalAoeWrite(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emAoeRead:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalAoeRead(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emAoeReadWrite:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalAoeReadWrite(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emAoeGetSlaveNetId:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalAoeGetSlaveNetId(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emAoeWriteControl:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalAoeWriteControl(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break; 
        case emrasapicmd_emGetNumConnectedSlaves:
        case emrasapicmd_emGetNumConnectedSlavesMain:
        case emrasapicmd_emGetNumConnectedSlavesRed:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetNumHelper(eCmdType, pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emResetSlavecontroller:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalResetSlaveController(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetSlaveInfo:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetSlaveInfo(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetCfgSlaveInfo:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetCfgSlaveInfo(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetBusSlaveInfo:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetBusSlaveInfo(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emIsSlavePresent:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalIsSlavePresent(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetFixedAddr:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetFixedAddr(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetSlaveInpVarInfoNumOf:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetSlaveInpVarInfoNumOf(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetSlaveOutpVarInfoNumOf:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetSlaveOutpVarInfoNumOf(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetSlaveInpVarInfo:
        case emrasapicmd_emGetSlaveInpVarInfoEx:
        {
            EC_T_BOOL bEntryEx = (eCmdType == emrasapicmd_emGetSlaveInpVarInfoEx) ? EC_TRUE : EC_FALSE; 
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetSlaveInpVarInfo(bEntryEx, pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetSlaveOutpVarInfo:
        case emrasapicmd_emGetSlaveOutpVarInfoEx:
        {
            EC_T_BOOL bEntryEx = (eCmdType == emrasapicmd_emGetSlaveOutpVarInfoEx) ? EC_TRUE : EC_FALSE; 
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetSlaveOutpVarInfo(bEntryEx, pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emFindInpVarByName:
        case emrasapicmd_emFindInpVarByNameEx:
        {
            EC_T_BOOL bEntryEx = (eCmdType == emrasapicmd_emFindInpVarByNameEx) ? EC_TRUE : EC_FALSE; 
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalFindInpVarByName(bEntryEx, pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emFindOutpVarByName:
        case emrasapicmd_emFindOutpVarByNameEx:
        {
            EC_T_BOOL bEntryEx = (eCmdType == emrasapicmd_emFindOutpVarByNameEx) ? EC_TRUE : EC_FALSE; 
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalFindOutpVarByName(bEntryEx, pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetProcessDataBits:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetProcessDataBits(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emSetProcessDataBits:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalSetProcessDataBits(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emForceProcessDataBits:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalForceProcessDataBits(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emReleaseProcessDataBits:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalReleaseProcessDataBits(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emReleaseAllProcessDataBits:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalReleaseAllProcessDataBits(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emReadSlaveEEPRom:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalReadSlaveEEPRom(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emWriteSlaveEEPRom:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalWriteSlaveEEPRom(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emAssignSlaveEEPRom:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalAssignSlaveEEPRom(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emReloadSlaveEEPRom:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalReloadSlaveEEPRom(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emSoeRead:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalSoeRead(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emSoeWrite:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalSoeWrite(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emSoeAbortProcCmd:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalSoeAbortProcCmd(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetVersion:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetVersion(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetSrcMacAddress:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetSrcMacAddress(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emEoeRegisterEp:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalEoERegisterEndpoint(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emEoeUnregisterEp:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalEoEUnregisterEndpoint(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emLinkOpen:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalLinkOpen(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emLinkClose:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalLinkClose(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emLinkSendAndFreeFrame:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalLinkSendAndFreeFrame(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emLinkRecvFrame:
        {
            dwRes = deMarshalLinkRecvFrame(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emReadSlaveIdentification:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalReadSlaveIdentification(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emSetSlaveDisabled:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalSetSlaveDisabled(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emSetSlaveDisconnected:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalSetSlaveDisconnected(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emRescueScan:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalRescueScan(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emGetMasterInfo:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalGetMasterInfo(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emConfigExtend:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalConfigExtend(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        case emrasapicmd_emSetMbxProtocolsSerialize:
        {
            emrasProtoCreateCmdResponseNoAlloc(pHeader, dwLen);
            dwRes = deMarshalSetMbxProtocolsSerialize(pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
        default:
        {
            emrasProtoCreateNackNoAlloc(pHeader, EC_E_INVALIDCMD, (EC_T_BYTE*)pHeader, &dwWrtLen);

            /* call locks command queue himself */
            dwRes = SendData((EC_T_BYTE*)pHeader, dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    VERBOSEOUT(2, ("<deMarshal()=0x%08X\n", dwRetVal));
    return dwRetVal;
}

EC_T_DWORD deMarshalIoControlCheckAccess(EC_T_DWORD dwIoctlCode)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    EC_T_DWORD dwRes = EC_E_ERROR;
#endif

    /* first DWORD contains command code */
    switch (dwIoctlCode)
    {
        /* following codes are checked for access (EC_E_NOERROR (default) / EMRAS_E_ACCESSLESS) */
        case EC_IOCTL_REGISTERCLIENT: // TODO: complete!
        case EC_IOCTL_UNREGISTERCLIENT:
        case EC_IOCTL_ISLINK_CONNECTED:
        case EC_IOCTL_SET_CYC_ERROR_NOTIFY_MASK:
        case EC_IOCTL_SLAVE_LINKMESSAGES:
        case EC_IOCTL_IS_MAIN_LINK_CONNECTED:
        case EC_IOCTL_IS_RED_LINK_CONNECTED:
        case EC_IOCTL_GET_PDMEMORYSIZE:             /* (EC_IOCTL_GENERIC | 40) */
        case EC_IOCTL_SET_SLVSTAT_PERIOD:           /* (EC_IOCTL_GENERIC | 43) */
        case EC_IOCTL_FORCE_SLVSTAT_COLLECTION:     
        case EC_IOCTL_GET_SLVSTATISTICS:            
        case EC_IOCTL_CLR_SLVSTATISTICS:            
        case EC_IOCTL_ALL_SLAVES_MUST_REACH_MASTER_STATE:
        case EC_IOCTL_DC_SLV_SYNC_STATUS_GET:
        case EC_IOCTL_SB_RESTART:
        case EC_IOCTL_SB_STATUS_GET:
        case EC_IOCTL_SB_SET_BUSCNF_VERIFY:
        case EC_IOCTL_SB_SET_BUSCNF_VERIFY_PROP:
        case EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO:
        case EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO_EEP:
        case EC_IOCTL_SB_ENABLE:
        case EC_IOCTL_SB_SET_BUSCNF_READ_PROP:
        case EC_IOCTL_SET_NOTIFICATION_ENABLED:
        case EC_IOCTL_GET_NOTIFICATION_ENABLED:
        case EC_IOCTL_CLEAR_MASTER_INFO_COUNTERS:
        case EC_IOCTL_SET_ZERO_INPUTS_ON_WKC_ZERO:
        case EC_IOCTL_SET_ZERO_INPUTS_ON_WKC_ERROR:
        {
    #if (defined INCLUDE_RAS_SPOCSUPPORT)
            dwRes = m_pServer->GetAccessLevelByCall(ord_emIoControl, dwIoctlCode);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
    #endif /* INCLUDE_RAS_SPOCSUPPORT */
        } break;

        /* following codes are not supported (EC_E_NOTSUPPORTED) */
        case EC_IOCTL_LINKLAYER_DBG_MSG:
        case EC_IOCTL_GET_CYCLIC_CONFIG_INFO:
        case EC_IOCTL_REGISTER_PDMEMORYPROVIDER:
        case EC_IOCTL_RESET_SLAVE:
        case EC_IOCTL_SET_FRAME_LOSS_SIMULATION:
        case EC_IOCTL_SET_RXFRAME_LOSS_SIMULATION:
        case EC_IOCTL_SET_TXFRAME_LOSS_SIMULATION:
        case EC_IOCTL_FORCE_BROADCAST_DESTINATION:
        case EC_IOCTL_SLV_ALIAS_ENABLE:
        case EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO_EX:
        case EC_IOCTL_SB_SET_TOPOLOGY_CHANGED_DELAY:
        case EC_IOCTL_SET_MASTER_DEFAULT_TIMEOUTS:
        case EC_IOCTL_SET_COPYINFO_IN_SENDCYCFRAMES:
        case EC_IOCTL_SET_BUS_CYCLE_TIME:
        case EC_IOCTL_ADDITIONAL_VARIABLES_FOR_SPECIFIC_DATA_TYPES:
        case EC_IOCTL_SET_IGNORE_INPUTS_ON_WKC_ERROR:
        case EC_IOCTL_SET_GENENI_ASSIGN_EEPROM_BACK_TO_ECAT:
        case EC_IOCTL_SET_AUTO_ACK_AL_STATUS_ERROR_ENABLED:
        case EC_IOCTL_SET_AUTO_ADJUST_CYCCMD_WKC_ENABLED:
        case EC_IOCTL_SET_ADJUST_CYCFRAMES_AFTER_SLAVES_STATE_CHANGE:
        case EC_IOCTL_REG_DC_SLV_SYNC_NTFY:
        case EC_IOCTL_UNREG_DC_SLV_SYNC_NTFY:
        case EC_IOCTL_DCM_REGISTER_TIMESTAMP:
        case EC_IOCTL_DCM_UNREGISTER_TIMESTAMP:
        case EC_IOCTL_DCM_REGISTER_STARTSO_CALLBACK:
        default:
        {
            dwRetVal = EC_E_NOTSUPPORTED;
            goto Exit;
        } break;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

#if (defined EC_BIG_ENDIAN)
static EC_INLINESTART EC_T_DWORD deMarshalIoControlSwapParms(EC_T_DWORD dwIoctlCode, EC_T_BYTE* pbyInBuf, EC_T_DWORD dwInBufSize)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    switch (dwIoctlCode)
    {
    case EC_IOCTL_UNREGISTERCLIENT:
    {
        if (dwInBufSize >= sizeof(EC_T_DWORD))
        {
            EC_T_DWORD dwHelp = EC_GET_FRM_DWORD(pbyInBuf);
            EC_SETDWORD(pbyInBuf, dwHelp);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_SET_CYC_ERROR_NOTIFY_MASK:
    {
        if (dwInBufSize >= sizeof(EC_T_DWORD))
        {
            EC_T_DWORD dwHelp = EC_GET_FRM_DWORD(pbyInBuf);
            EC_SETDWORD(pbyInBuf, dwHelp);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_SLAVE_LINKMESSAGES:
    {
        if (dwInBufSize >= sizeof(EC_T_SLAVE_LINKMSG_DESC))
        {
            EC_T_SLAVE_LINKMSG_DESC* pRes = (EC_T_SLAVE_LINKMSG_DESC*)(pbyInBuf);
            pRes->dwSlaveId        = EC_DWORDSWAP(pRes->dwSlaveId);
            pRes->bEnableLogging   = EC_DWORDSWAP(pRes->bEnableLogging);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_SET_SLVSTAT_PERIOD:
    {
        if (dwInBufSize >= sizeof(EC_T_DWORD))
        {
            EC_T_DWORD dwHelp = EC_GET_FRM_DWORD(pbyInBuf);
            EC_SETDWORD(pbyInBuf, dwHelp);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_FORCE_SLVSTAT_COLLECTION:
        /* no input parameter */
        break;
    case EC_IOCTL_GET_SLVSTATISTICS:
    {
        if (dwInBufSize >= sizeof(EC_T_DWORD))
        {
            EC_T_DWORD dwHelp = EC_GET_FRM_DWORD(pbyInBuf);
            EC_SETDWORD(pbyInBuf, dwHelp);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_CLR_SLVSTATISTICS:
        /* no input parameter */
        break;
    case EC_IOCTL_ALL_SLAVES_MUST_REACH_MASTER_STATE:
    {
        if (dwInBufSize >= sizeof(EC_T_DWORD))
        {
            EC_T_DWORD dwHelp = EC_GET_FRM_DWORD(pbyInBuf);
            EC_SETDWORD(pbyInBuf, dwHelp);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_SET_NOTIFICATION_ENABLED:
    {
        if (dwInBufSize >= sizeof(EC_T_SET_NOTIFICATION_ENABLED_PARMS))
        {
            EC_T_SET_NOTIFICATION_ENABLED_PARMS* pDesc =
                (EC_T_SET_NOTIFICATION_ENABLED_PARMS*)(pbyInBuf);

            pDesc->dwClientId = EC_DWORDSWAP(pDesc->dwClientId);
            pDesc->dwCode = EC_DWORDSWAP(pDesc->dwCode);
            pDesc->dwEnabled = EC_DWORDSWAP(pDesc->dwEnabled);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_GET_NOTIFICATION_ENABLED:
    {
        if (dwInBufSize >= sizeof(EC_T_GET_NOTIFICATION_ENABLED_PARMS))
        {
            EC_T_GET_NOTIFICATION_ENABLED_PARMS* pDesc =
                (EC_T_GET_NOTIFICATION_ENABLED_PARMS*)(pbyInBuf);
            pDesc->dwClientId = EC_DWORDSWAP(pDesc->dwClientId);
            pDesc->dwCode = EC_DWORDSWAP(pDesc->dwCode);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_SB_RESTART:
        /* no input parameter */
        break;
    case EC_IOCTL_SB_STATUS_GET:
        /* no input parameter */
        break;
    case EC_IOCTL_SB_SET_BUSCNF_VERIFY:
    {
        if (dwInBufSize >= sizeof(EC_T_DWORD))
        {
            EC_T_DWORD dwHelp = EC_GET_FRM_DWORD(pbyInBuf);
            EC_SETDWORD(pbyInBuf, dwHelp);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_SB_SET_BUSCNF_VERIFY_PROP:
    {
        if (dwInBufSize >= sizeof(EC_T_SCANBUS_PROP_DESC))
        {
            EC_T_SCANBUS_PROP_DESC* pRes = (EC_T_SCANBUS_PROP_DESC*)(pbyInBuf);
            pRes->eEEPROMEntry  = (EC_T_eEEPENTRY)EC_DWORDSWAP(pRes->eEEPROMEntry);
            pRes->dwVerify      = EC_DWORDSWAP(pRes->dwVerify);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO:
    {
        if (dwInBufSize >= sizeof(EC_T_WORD))
        {
            EC_T_WORD wHelp = EC_GET_FRM_WORD(pbyInBuf);
            EC_SETWORD(pbyInBuf, wHelp);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO_EEP:
    {
        if (dwInBufSize >= sizeof(EC_T_SB_SLAVEINFO_EEP_REQ_DESC))
        {
            EC_T_SB_SLAVEINFO_EEP_REQ_DESC* pRes = (EC_T_SB_SLAVEINFO_EEP_REQ_DESC*)(pbyInBuf);
            pRes->eSbSlaveInfoType  = (EC_T_eSBSlaveInfoType)EC_DWORDSWAP(pRes->eSbSlaveInfoType);
            pRes->wAutoIncAddress      = EC_DWORDSWAP(pRes->wAutoIncAddress);
            pRes->eEEPROMEntry      = (EC_T_eEEPENTRY)EC_DWORDSWAP(pRes->eEEPROMEntry);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_SB_ENABLE:
    {
        if (dwInBufSize >= sizeof(EC_T_DWORD))
        {
            EC_T_DWORD dwHelp = EC_GET_FRM_DWORD(pbyInBuf);
            EC_SETDWORD(pbyInBuf, dwHelp);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_SB_SET_BUSCNF_READ_PROP:
    {
        if (dwInBufSize >= sizeof(EC_T_SCANBUS_PROP_DESC))
        {
            EC_T_SCANBUS_PROP_DESC* pRes = (EC_T_SCANBUS_PROP_DESC*)(pbyInBuf);

            pRes->eEEPROMEntry = (EC_T_eEEPENTRY)EC_DWORDSWAP(pRes->eEEPROMEntry);
            pRes->dwVerify     = EC_DWORDSWAP(pRes->dwVerify);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_CLEAR_MASTER_INFO_COUNTERS:
    {
        if (dwInBufSize >= sizeof(EC_T_CLEAR_MASTER_INFO_COUNTERS_PARMS))
        {
            EC_T_CLEAR_MASTER_INFO_COUNTERS_PARMS* pDesc =
                (EC_T_CLEAR_MASTER_INFO_COUNTERS_PARMS*)(pbyInBuf);

            pDesc->dwClearBusDiagnosisCounters = EC_DWORDSWAP(pDesc->dwClearBusDiagnosisCounters);
            pDesc->qwMailboxStatisticsClearCounters = EC_QWORDSWAP(pDesc->qwMailboxStatisticsClearCounters);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    } break;
    case EC_IOCTL_SET_ZERO_INPUTS_ON_WKC_ZERO:
    case EC_IOCTL_SET_ZERO_INPUTS_ON_WKC_ERROR:
    {
        if (dwInBufSize >= sizeof(EC_T_DWORD))
        {
            EC_T_DWORD dwHelp = EC_GET_FRM_DWORD(pbyInBuf);
            EC_SETDWORD(pbyInBuf, dwHelp);
        }
        else
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    }
    default: break;
    } /* switch (dwIoctlCode) */
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

static EC_INLINESTART EC_T_DWORD deMarshalIoControlSwapResults(EC_T_DWORD dwIoctlCode, EC_T_BYTE* pbyOutBuf, EC_T_DWORD dwOutBufSize)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    switch (dwIoctlCode)
    {
        /* IoCtls without result structure */
        case EC_IOCTL_SB_ENABLE:
        case EC_IOCTL_SB_RESTART:
        case EC_IOCTL_SB_SET_BUSCNF_READ_PROP:
        case EC_IOCTL_SB_SET_BUSCNF_VERIFY:
        case EC_IOCTL_SB_SET_BUSCNF_VERIFY_PROP:
        case EC_IOCTL_SET_NOTIFICATION_ENABLED:
        case EC_IOCTL_UNREGISTERCLIENT:
        case EC_IOCTL_SET_ZERO_INPUTS_ON_WKC_ZERO:
        case EC_IOCTL_SET_ZERO_INPUTS_ON_WKC_ERROR:
            break;

        /* IoCtls with output parameters */
        case EC_IOCTL_REGISTERCLIENT:
        {
            if (dwOutBufSize >= sizeof(ATEMRAS_T_REGISTERRESULTSTRANSFER))
            {
                ATEMRAS_T_REGISTERRESULTSTRANSFER* pRegRes = (ATEMRAS_T_REGISTERRESULTSTRANSFER*)(pbyOutBuf);
                pRegRes->dwClntId = EC_DWORDSWAP(pRegRes->dwClntId);
                pRegRes->dwPDInSize = EC_DWORDSWAP(pRegRes->dwPDInSize);
                pRegRes->dwPDOutSize = EC_DWORDSWAP(pRegRes->dwPDOutSize);
            }
            else
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
        } break;
        case EC_IOCTL_ISLINK_CONNECTED:
        case EC_IOCTL_IS_MAIN_LINK_CONNECTED:
        case EC_IOCTL_IS_RED_LINK_CONNECTED:
        {
            if (dwOutBufSize >= sizeof(EC_T_DWORD))
            {
                EC_T_DWORD dwHelp = EC_GET_FRM_DWORD(pbyOutBuf);
                EC_SETDWORD(pbyOutBuf, dwHelp);
            }
            else
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
        } break;
        case EC_IOCTL_GET_PDMEMORYSIZE:
        {
            if (dwOutBufSize >= sizeof(EC_T_MEMREQ_DESC))
            {
                EC_T_MEMREQ_DESC* pRes = (EC_T_MEMREQ_DESC*)(pbyOutBuf);
                pRes->dwPDOutSize  = EC_DWORDSWAP(pRes->dwPDOutSize);
                pRes->dwPDInSize   = EC_DWORDSWAP(pRes->dwPDInSize);
            }
            else
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
        } break;
        case EC_IOCTL_SET_SLVSTAT_PERIOD:
        case EC_IOCTL_FORCE_SLVSTAT_COLLECTION:
        case EC_IOCTL_CLR_SLVSTATISTICS:
            /* no ouput parameter */
            break;
        case EC_IOCTL_GET_SLVSTATISTICS:
        {
            if (dwOutBufSize >= sizeof(EC_T_SLVSTATISTICS_DESC))
            {
                EC_T_SLVSTATISTICS_DESC* pRes = (EC_T_SLVSTATISTICS_DESC*)(pbyOutBuf);
                pRes->wRxErrorCounter[0] = EC_WORDSWAP(pRes->wRxErrorCounter[0]);
                pRes->wRxErrorCounter[1] = EC_WORDSWAP(pRes->wRxErrorCounter[1]);
                pRes->wRxErrorCounter[2] = EC_WORDSWAP(pRes->wRxErrorCounter[2]);
                pRes->wRxErrorCounter[3] = EC_WORDSWAP(pRes->wRxErrorCounter[3]);
                pRes->wAlStatusCode  = EC_WORDSWAP(pRes->wAlStatusCode);
            }
            else
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
        } break;
        case EC_IOCTL_DC_SLV_SYNC_STATUS_GET:
        {
            if (dwOutBufSize >= sizeof(EC_T_DC_SYNC_NTFY_DESC))
            {
                EC_T_DC_SYNC_NTFY_DESC* pRes = (EC_T_DC_SYNC_NTFY_DESC*)(pbyOutBuf);

                pRes->IsInSync      = EC_DWORDSWAP(pRes->IsInSync);
                pRes->IsNegative    = EC_DWORDSWAP(pRes->IsNegative);
                pRes->dwDeviation   = EC_DWORDSWAP(pRes->dwDeviation);
                pRes->SlaveProp.wStationAddress   = EC_WORDSWAP(pRes->SlaveProp.wStationAddress);
                pRes->SlaveProp.wAutoIncAddr      = EC_WORDSWAP(pRes->SlaveProp.wAutoIncAddr);
            }
            else
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
        } break;
        case EC_IOCTL_SB_STATUS_GET:
        {
            if (dwOutBufSize >= sizeof(EC_T_SB_STATUS_NTFY_DESC))
            {
                EC_T_SB_STATUS_NTFY_DESC* pRes = (EC_T_SB_STATUS_NTFY_DESC*)(pbyOutBuf);
                pRes->dwResultCode  = EC_DWORDSWAP(pRes->dwResultCode);
                pRes->dwSlaveCount  = EC_DWORDSWAP(pRes->dwSlaveCount);
            }
            else
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
        } break;
        case EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO:
        {
            if (dwOutBufSize >= sizeof(EC_T_SB_SLAVEINFO_DESC))
            {
                EC_T_SB_SLAVEINFO_DESC* pRes = (EC_T_SB_SLAVEINFO_DESC*)(pbyOutBuf);
                pRes->dwScanBusStatus   = EC_DWORDSWAP(pRes->dwScanBusStatus);
                pRes->dwVendorId        = EC_DWORDSWAP(pRes->dwVendorId);
                pRes->dwProductCode     = EC_DWORDSWAP(pRes->dwProductCode);
                pRes->dwRevisionNumber  = EC_DWORDSWAP(pRes->dwRevisionNumber);
                pRes->dwSerialNumber    = EC_DWORDSWAP(pRes->dwSerialNumber);
            }
            else
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
        } break;
        case EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO_EEP:
        {
            if (dwOutBufSize >= sizeof(EC_T_SB_SLAVEINFO_EEP_RES_DESC))
            {
                EC_T_SB_SLAVEINFO_EEP_RES_DESC* pRes = (EC_T_SB_SLAVEINFO_EEP_RES_DESC*)(pbyOutBuf);
                pRes->dwScanBusStatus   = EC_DWORDSWAP(pRes->dwScanBusStatus);
                pRes->eEEPROMEntry        = (EC_T_eEEPENTRY)EC_DWORDSWAP(pRes->eEEPROMEntry);
                pRes->dwEEPROMValue     = EC_DWORDSWAP(pRes->dwEEPROMValue);
            }
            else
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
        } break;
        case EC_IOCTL_GET_NOTIFICATION_ENABLED:
        {
            if (dwOutBufSize >= sizeof(EC_T_BOOL))
            {
                EC_T_BOOL* pbRes = (EC_T_BOOL*)(pbyOutBuf);
                *pbRes      = EC_DWORDSWAP(*pbRes);
            }
            else
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
        } break;
    } /* switch (dwIoctlCode) */
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}
#endif /* EC_BIG_ENDIAN */

EMMARSH_T_NOTIFICATION_CTXT* CAtemRasSrvClient::AllocateNotificationContext(ATEMRAS_T_PROTOHDR* pHeader)
{
    EMMARSH_PT_NOTIFICATION_CTXT    pNotCtxt            = EC_NULL;
    pNotCtxt = (EMMARSH_PT_NOTIFICATION_CTXT)OsMalloc(sizeof(EMMARSH_T_NOTIFICATION_CTXT));
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "deMarshalIoControl pNotCtxt", pNotCtxt, sizeof(EMMARSH_T_NOTIFICATION_CTXT));
    if (EC_NULL == pNotCtxt)
    {
        goto Exit;
    }
    OsMemset(pNotCtxt, 0, sizeof(EMMARSH_T_NOTIFICATION_CTXT));
    pNotCtxt->dwCookie          = ATEMRAS_PROTOHDR_GET_COOKIE(pHeader);
    pNotCtxt->dwMasterInstance  = (EC_T_DWORD)-1;
    pNotCtxt->pServer           = m_pServer;
    pNotCtxt->pConnection       = this;
    pNotCtxt->dwClientId        = (EC_T_DWORD)-1;

Exit:
    return pNotCtxt;
}

EC_T_VOID CAtemRasSrvClient::FreeNotificationContext(EMMARSH_T_NOTIFICATION_CTXT* pNotCtxt)
{
    SafeOsFree(pNotCtxt);
}

/***************************************************************************************************/
/**
\brief  Perform IO Control.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalIoControl(
    ATEMRAS_T_PROTOHDR*     pHeader,        /**< [in]   original received protocol header for response */
    EC_T_DWORD dwWrtLen        /**< [in]   Size of working buffer */
                             )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_IOCTLTRANSFER        pIoctlTransfer      = EC_NULL;
    EC_T_BYTE*                      pbyTmp              = EC_NULL;

    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_DWORD                      dwIoctlCode         = 0;
    EC_T_IOCTLPARMS                 oIoCtlParms         = {0};
    EC_T_DWORD                      dwNumOutData        = 0;
    EC_T_DWORD                      dwIoctlRetVal       = EC_E_ERROR;

    EMMARSH_PT_NOTIFICATION_CTXT    pNotCtxt            = EC_NULL;
    EC_T_IOCTLPARMS                 oIoCtlParmsCallee   = {0};
    EC_T_REGISTERPARMS              oRegParms           = {0};
    EC_T_REGISTERRESULTS            oRegResults         = {0};

    pIoctlTransfer              = (ATEMRAS_PT_IOCTLTRANSFER)ATEMRAS_PROTOHDR_DATA(pHeader);
    pbyTmp                      = ATEMRAS_PROTOHDR_DATA(pHeader);
    
    dwMasterInstance            = ATEMRAS_IOCTL_GET_INSTANCEID(pIoctlTransfer);
    dwIoctlCode                 = ATEMRAS_IOCTL_GET_IOCTLCODE(pIoctlTransfer);
    
    oIoCtlParms.dwInBufSize     = ATEMRAS_IOCTL_GET_INBUFSIZE(pIoctlTransfer);
    oIoCtlParms.pbyInBuf        = (pbyTmp+ATEMRAS_IOCTL_GET_INBUFOFFSET(pIoctlTransfer));
    oIoCtlParms.dwOutBufSize    = ATEMRAS_IOCTL_GET_OUTBUFSIZE(pIoctlTransfer);
    oIoCtlParms.pbyOutBuf       = (pbyTmp+ATEMRAS_IOCTL_GET_OUTBUFOFFSET(pIoctlTransfer));
    oIoCtlParms.pdwNumOutData   = &dwNumOutData;
    
    VERBOSEOUT(2, ("deMarshalIoControl(): IoCtlCode 0x%x, InBufSize %d, OutBufSize %d\n",
        dwIoctlCode, oIoCtlParms.dwInBufSize, oIoCtlParms.dwOutBufSize));

    /* excluded IoCtls not supported with RAS or denied by SPOC */
    dwRes = deMarshalIoControlCheckAccess(dwIoctlCode);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    /* backup IoCtlParms */
    OsMemcpy(&oIoCtlParmsCallee, &oIoCtlParms, sizeof(EC_T_IOCTLPARMS));

#if (defined EC_BIG_ENDIAN)
    deMarshalIoControlSwapParms(dwIoctlCode, oIoCtlParms.pbyInBuf, oIoCtlParms.dwInBufSize);
#endif

    if (EC_IOCTL_REGISTERCLIENT == dwIoctlCode)
    {
        /* allocate notification context */
        pNotCtxt = AllocateNotificationContext(pHeader);
        if (EC_NULL == pNotCtxt)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        /* replace IoCtlParms from callee by local IoCtlParms */
        {
            OsMemset(&oRegParms, 0, sizeof(EC_T_REGISTERPARMS));
            oRegParms.pCallerData    = pNotCtxt;
            oRegParms.pfnNotify      = marshalNotify;
            OsMemset(&oRegResults, 0, sizeof(EC_T_REGISTERRESULTS));
            oIoCtlParms.pbyInBuf     = (EC_T_BYTE*)&oRegParms;
            oIoCtlParms.dwInBufSize  = sizeof(EC_T_REGISTERPARMS);
            oIoCtlParms.pbyOutBuf    = (EC_T_BYTE*)&oRegResults;
            oIoCtlParms.dwOutBufSize = sizeof(EC_T_REGISTERRESULTS);
        }
    }
    else if (EC_IOCTL_UNREGISTERCLIENT == dwIoctlCode)
    {
        /* TODO: validate ClientId */
    }

    /* execute API */
    /***************/
    dwIoctlRetVal = emIoControl(dwMasterInstance, dwIoctlCode, &oIoCtlParms);
    ATEMRAS_IOCTL_SET_NUMOUTDATA(pIoctlTransfer, dwNumOutData);
    ATEMRAS_IOCTL_SET_SRVRESULT(pIoctlTransfer, dwIoctlRetVal);
    if (EC_E_NOERROR != dwIoctlRetVal)
    {
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }
    
    /* do administrative storage of master connection */
    if (EC_IOCTL_REGISTERCLIENT == dwIoctlCode)
    {
        EC_T_REGISTERRESULTS* pRegRes  = (EC_T_REGISTERRESULTS*)(oIoCtlParms.pbyOutBuf);

        /* PD pointers returned by Master IoControl are invalid for RAS client */
        pRegRes->pbyPDIn = EC_NULL;
        pRegRes->pbyPDOut = EC_NULL;

        /* add registration */
        pNotCtxt->dwMasterInstance = dwMasterInstance;
        pNotCtxt->dwClientId       = pRegRes->dwClntId;
        dwRes = m_pMasterStore->AddRegistration(dwMasterInstance, pRegRes, pNotCtxt);
        if (EC_E_NOERROR != dwRes)
        {
            /* revoke client registration at master if registration tracking at RAS server failed */
            emUnregisterClient(dwMasterInstance, pRegRes->dwClntId);

            dwIoctlRetVal = dwRes;
            dwRetVal      = EC_E_NOERROR;
            goto Exit;
        }
        /* notify RAS server application */
        {
            ATEMRAS_T_REGNOTIFYDESC oRegNot;

            OsMemset(&oRegNot, 0, sizeof(ATEMRAS_T_REGNOTIFYDESC));
            oRegNot.dwCookie     = ATEMRAS_PROTOHDR_GET_COOKIE(pHeader);
            oRegNot.dwInstanceId = dwMasterInstance;
            oRegNot.dwClientId   = pRegRes->dwClntId;
            oRegNot.dwResult     = EC_E_NOERROR;
            m_pServer->NotifyCallee(ATEMRAS_NOTIFY_REGISTER, sizeof(ATEMRAS_T_REGNOTIFYDESC), (EC_T_BYTE*)&oRegNot);
        }
        /* return registration results to callee */
        {
            ATEMRAS_T_REGISTERRESULTSTRANSFER* pRegResCallee = (ATEMRAS_T_REGISTERRESULTSTRANSFER*)(oIoCtlParmsCallee.pbyOutBuf);

            pRegResCallee->dwClntId    = pRegRes->dwClntId;
            pRegResCallee->dwPDInSize  = pRegRes->dwPDInSize;
            pRegResCallee->dwPDOutSize = pRegRes->dwPDOutSize;
        }
    }
    else if ((EC_IOCTL_UNREGISTERCLIENT == dwIoctlCode) && (EC_E_NOERROR == dwRes))
    {
        EC_T_DWORD dwClientId  = EC_GET_FRM_DWORD(oIoCtlParms.pbyInBuf);

        /* remove registration */
        dwRes = m_pMasterStore->RemoveRegistration(dwMasterInstance, dwClientId);

        /* notify RAS server application */
        {
            ATEMRAS_T_REGNOTIFYDESC oRegNot;

            OsMemset(&oRegNot, 0, sizeof(ATEMRAS_T_REGNOTIFYDESC));
            oRegNot.dwCookie     = ATEMRAS_PROTOHDR_GET_COOKIE(pHeader);
            oRegNot.dwInstanceId = dwMasterInstance;
            oRegNot.dwClientId   = dwClientId;
            oRegNot.dwResult     = EC_E_NOERROR;
            m_pServer->NotifyCallee(ATEMRAS_NOTIFY_UNREGISTER, sizeof(ATEMRAS_T_REGNOTIFYDESC), (EC_T_BYTE*)&oRegNot);
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:
#if (defined EC_BIG_ENDIAN)
    deMarshalIoControlSwapResults(dwIoctlCode, oIoCtlParmsCallee.pbyOutBuf, oIoCtlParmsCallee.dwOutBufSize);
#endif

    /* set error code in answer to RAS client */
    if ((EC_E_NOERROR != dwRetVal) || (EC_E_NOERROR != dwIoctlRetVal))
    {
        emrasProtoCreateNackNoAlloc(pHeader, (EC_E_NOERROR != dwRetVal)?dwRetVal:dwIoctlRetVal, (EC_T_BYTE*)pHeader, &dwWrtLen);
        FreeNotificationContext(pNotCtxt);
    }

    /* send answer to RAS client */
    SendData((EC_T_BYTE*)pHeader, dwWrtLen);

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Perform emConfigureMaster.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalConfigureMaster(
    EC_T_BYTE*          pbyTransfer, 
    EC_T_DWORD          dwWrtLen
    )
{
    EC_T_DWORD                           dwRetVal         = EC_E_ERROR;
    EC_T_DWORD                           dwRes            = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR*                  pTransferHead    = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    ATEMRAS_PT_EMCONFIGUREMASTERTRANSFER pECcnfTransfer   = (ATEMRAS_PT_EMCONFIGUREMASTERTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);;
    EC_T_BYTE*                           pbyTmp           = ATEMRAS_PROTOHDR_DATA(pTransferHead);
    EC_T_DWORD                           dwMasterInstance = ATEMRAS_EMCONFIGUREMASTER_GET_INSTANCEID(pECcnfTransfer);

    if ((eCnfType_Data != ATEMRAS_EMCONFIGUREMASTER_GET_CONFIGTYPE(pECcnfTransfer)) && 
        (eCnfType_Datadiag != ATEMRAS_EMCONFIGUREMASTER_GET_CONFIGTYPE(pECcnfTransfer)) &&
        (eCnfType_GenPreopENI != ATEMRAS_EMCONFIGUREMASTER_GET_CONFIGTYPE(pECcnfTransfer)))
    {
        /* remote not available ioctl */
        dwRetVal = EC_E_INVALIDPARM;
        ATEMRAS_EMCONFIGUREMASTER_SET_SRVRESULT(pECcnfTransfer, dwRetVal);
        m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, (EC_T_BYTE*)pTransferHead, EC_NULL, EC_NULL, EC_NULL);
        goto Exit;
    }

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emConfigureMaster);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emConfigureMaster(
            dwMasterInstance, 
            ((EC_T_CNF_TYPE)ATEMRAS_EMCONFIGUREMASTER_GET_CONFIGTYPE(pECcnfTransfer)),
            pbyTmp+ATEMRAS_EMCONFIGUREMASTER_GET_CNFDATAOFFSET(pECcnfTransfer),
            ATEMRAS_EMCONFIGUREMASTER_GET_CNFDATASIZE(pECcnfTransfer)
        );
    }

    ATEMRAS_EMCONFIGUREMASTER_SET_SRVRESULT(pECcnfTransfer, dwRes);

    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, (EC_T_BYTE*)pTransferHead, EC_NULL, EC_NULL, EC_NULL); 
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
\brief  Perform emStart.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalStart(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMSTARTTRANSFER      pECStartTransfer    = (ATEMRAS_PT_EMSTARTTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    EC_T_DWORD                      dwMasterInstance    = ATEMRAS_EMSTARTTRANSFER_GET_INSTANCEID(pECStartTransfer);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emStart);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emStart(dwMasterInstance, ATEMRAS_EMSTARTTRANSFER_GET_TIMEOUT(pECStartTransfer));
    }

    ATEMRAS_EMSTARTTRANSFER_SET_SRVRESULT(pECStartTransfer, dwRes);

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emSetMasterState.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalSetMasterState(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMSETMASTERSTATETRANSFER pECSetMSTransfer    = (ATEMRAS_PT_EMSETMASTERSTATETRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    EC_T_DWORD                          dwMasterInstance    = ATEMRAS_EMSETMASTERSTATETRANSFER_GET_INSTANCEID(pECSetMSTransfer);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSetMasterState);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emSetMasterState(
            dwMasterInstance, 
            ATEMRAS_EMSETMASTERSTATETRANSFER_GET_TIMEOUT(pECSetMSTransfer),
            (EC_T_STATE)ATEMRAS_EMSETMASTERSTATETRANSFER_GET_REQSTATE(pECSetMSTransfer)
        );
    }

    ATEMRAS_EMSETMASTERSTATETRANSFER_SET_SRVRESULT(pECSetMSTransfer, dwRes);

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetMasterState.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetMasterState(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    EC_T_STATE                          eState              = eEcatState_UNKNOWN;
    ATEMRAS_PT_EMGETMASTERSTATETRANSFER pECGetMSTransfer    = (ATEMRAS_PT_EMGETMASTERSTATETRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    EC_T_DWORD                          dwMasterInstance    = ATEMRAS_EMGETMASTERSTATETRANSFER_GET_INSTANCEID(pECGetMSTransfer);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetMasterState);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        eState = emGetMasterState(dwMasterInstance);
    }

    ATEMRAS_EMGETMASTERSTATETRANSFER_SET_STATE(pECGetMSTransfer, (EC_T_WORD)eState);

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emStop.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalStop(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMSTOPTRANSFER       pECStopTransfer    = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    
    pECStopTransfer             = (ATEMRAS_PT_EMSTOPTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_EMSTOPTRANSFER_GET_INSTANCEID(pECStopTransfer);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emStop);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emStop(dwMasterInstance, ATEMRAS_EMSTOPTRANSFER_GET_TIMEOUT(pECStopTransfer));
    }
    
    ATEMRAS_EMSTOPTRANSFER_SET_SRVRESULT(pECStopTransfer, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetConfiguredSlaves.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetConfiguredSlaves(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMGETNUMCNFGSLVTRANSFER  pECGetCnfgSlvTransf = EC_NULL;
    EC_T_DWORD                          dwMasterInstance    = 0;
    
    pECGetCnfgSlvTransf         = (ATEMRAS_PT_EMGETNUMCNFGSLVTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_EMGETNUMCNFGSLVTRANSFER_GET_INSTANCEID(pECGetCnfgSlvTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetNumConfiguredSlaves);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emGetNumConfiguredSlaves(dwMasterInstance);
    }
    
    ATEMRAS_EMGETNUMCNFGSLVTRANSFER_SET_NUMSLV(pECGetCnfgSlvTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetSlaveIdAtPosition.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSlvIdAtPos(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SLVIDATPTRANSFER     pGetSlvIdAtpTransf  = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    
    pGetSlvIdAtpTransf          = (ATEMRAS_PT_SLVIDATPTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_SLVIDATPTRANSFER_GET_INSTANCEID(pGetSlvIdAtpTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetSlaveIdAtPosition);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emGetSlaveIdAtPosition( dwMasterInstance, EC_LOWORD(ATEMRAS_SLVIDATPTRANSFER_GET_AINCADDR(pGetSlvIdAtpTransf)));
    }
    
    ATEMRAS_SLVIDATPTRANSFER_SET_SLVID(pGetSlvIdAtpTransf, dwRes);

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetSlaveId.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSlvId(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SLVIDTRANSFER        pGetSlvIdTransf     = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    
    pGetSlvIdTransf             = (ATEMRAS_PT_SLVIDTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_SLVIDTRANSFER_GET_INSTANCEID(pGetSlvIdTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetSlaveId);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emGetSlaveId(dwMasterInstance, EC_LOWORD(ATEMRAS_SLVIDTRANSFER_GET_STATADDR(pGetSlvIdTransf)));
    }
    
    ATEMRAS_SLVIDTRANSFER_SET_SLVID(pGetSlvIdTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetSlaveProp.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSlaveProp(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SLVPROPTRANSFER      pECGetSlvPropTransf = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_SLAVE_PROP                 oSlaveProp          = {0};
    
    pECGetSlvPropTransf         = (ATEMRAS_PT_SLVPROPTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_SLVPROPTRANSFER_GET_INSTANCEID(pECGetSlvPropTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetSlaveProp);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        if (!emGetSlaveProp(
            dwMasterInstance,
            ATEMRAS_SLVPROPTRANSFER_GET_SLAVEID(pECGetSlvPropTransf),
            &oSlaveProp
            ))
        {
            dwRes = EC_E_ERROR;
        }
        else
        {
            dwRes = EC_E_NOERROR;
        }
    }
    
    ATEMRAS_SLVPROPTRANSFER_SET_SRVRESULT(pECGetSlvPropTransf, dwRes);
    ATEMRAS_SLVPROPTRANSFER_SET_STATADDR(pECGetSlvPropTransf, oSlaveProp.wStationAddress);
    ATEMRAS_SLVPROPTRANSFER_SET_AINCADDR(pECGetSlvPropTransf, oSlaveProp.wAutoIncAddr);
    OsMemcpy(ATEMRAS_SLVPROPTRANSFER_NAME(pECGetSlvPropTransf), oSlaveProp.achName, sizeof(oSlaveProp.achName));

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetSlaveState.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSlaveState(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_GETSLVSTATETRANSFER  pECGetSlvStatTransf = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_WORD                       wCurrDevState       = 0;
    EC_T_WORD                       wReqDevState       = 0;
                                            
    pECGetSlvStatTransf         = (ATEMRAS_PT_GETSLVSTATETRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_GETSLVSTATETRANSFER_GET_INSTANCEID(pECGetSlvStatTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetSlaveState);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emGetSlaveState(
            dwMasterInstance,
            ATEMRAS_GETSLVSTATETRANSFER_GET_SLAVEID(pECGetSlvStatTransf),
            &wCurrDevState, &wReqDevState);
    }

    ATEMRAS_GETSLVSTATETRANSFER_SET_SRVRESULT(pECGetSlvStatTransf, dwRes);
    ATEMRAS_GETSLVSTATETRANSFER_SET_DEVICESTATECURR(pECGetSlvStatTransf, wCurrDevState);
    ATEMRAS_GETSLVSTATETRANSFER_SET_DEVICESTATEREQ(pECGetSlvStatTransf, wReqDevState);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emSetSlaveState.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalSetSlaveState(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SETSLVSTATETRANSFER  pECSetSlvStatTransf = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
                                            
    pECSetSlvStatTransf         = (ATEMRAS_PT_SETSLVSTATETRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_SETSLVSTATETRANSFER_GET_INSTANCEID(pECSetSlvStatTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSetSlaveState);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emSetSlaveState(
            dwMasterInstance,
            ATEMRAS_SETSLVSTATETRANSFER_GET_SLAVEID(pECSetSlvStatTransf),
            EC_LOWORD(ATEMRAS_SETSLVSTATETRANSFER_GET_NEWREQDEVICESTATE(pECSetSlvStatTransf)),
            ATEMRAS_SETSLVSTATETRANSFER_GET_TIMEOUT(pECSetSlvStatTransf)
            );
    }

    ATEMRAS_SETSLVSTATETRANSFER_SET_SRVRESULT(pECSetSlvStatTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emTferSingleRawCmd.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalTferSingleRawCmd(
    EC_T_BYTE*          pbyTransfer,
    EC_T_DWORD          dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR* pTransferHead       = EC_NULL;
    ATEMRAS_PT_TFERSIRCTRANSFER     pECTfSiRCTransf     = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
                                            
    pTransferHead               = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    pECTfSiRCTransf             = (ATEMRAS_PT_TFERSIRCTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_TFERSIRCTRANSFER_GET_INSTANCEID(pECTfSiRCTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emTferSingleRawCmd, EC_LOBYTE(EC_LOWORD(ATEMRAS_TFERSIRCTRANSFER_GET_COMMAND(pECTfSiRCTransf))));
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emTferSingleRawCmd(
            dwMasterInstance,
            ATEMRAS_TFERSIRCTRANSFER_GET_SLAVEID(pECTfSiRCTransf),
            EC_LOBYTE(EC_LOWORD(ATEMRAS_TFERSIRCTRANSFER_GET_COMMAND(pECTfSiRCTransf))),
            ATEMRAS_TFERSIRCTRANSFER_GET_MEMADDR(pECTfSiRCTransf),
            ((EC_T_PVOID)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_TFERSIRCTRANSFER_GET_DATAOFFSET(pECTfSiRCTransf))),
            EC_LOWORD(ATEMRAS_TFERSIRCTRANSFER_GET_DATASIZE(pECTfSiRCTransf)),
            ATEMRAS_TFERSIRCTRANSFER_GET_TIMEOUT(pECTfSiRCTransf)
            );
    }
    
    ATEMRAS_TFERSIRCTRANSFER_SET_SRVRESULT(pECTfSiRCTransf, dwRes);
                                                                                    
    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, pbyTransfer, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emQueueRawCmd.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalQueueRawCmd(
    EC_T_BYTE*          pbyTransfer, 
    EC_T_DWORD           dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR* pTransferHead       = EC_NULL;
    ATEMRAS_PT_TFERQRCTRANSFER      pECQRCTransfer      = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
                                            
    pTransferHead               = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    pECQRCTransfer              = (ATEMRAS_PT_TFERQRCTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_TFERQRCTRANSFER_GET_INSTANCEID(pECQRCTransfer);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emQueueRawCmd, EC_LOBYTE(EC_LOWORD(ATEMRAS_TFERQRCTRANSFER_GET_COMMAND(pECQRCTransfer))));
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emQueueRawCmd(
            dwMasterInstance,
            ATEMRAS_TFERQRCTRANSFER_GET_SLAVEID(pECQRCTransfer),
            EC_LOWORD(ATEMRAS_TFERQRCTRANSFER_GET_INVOKEID(pECQRCTransfer)),
            EC_LOBYTE(EC_LOWORD(ATEMRAS_TFERQRCTRANSFER_GET_COMMAND(pECQRCTransfer))),
            ATEMRAS_TFERQRCTRANSFER_GET_MEMADDR(pECQRCTransfer),
            ((EC_T_PVOID)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_TFERQRCTRANSFER_GET_DATAOFFSET(pECQRCTransfer))),
            EC_LOWORD(ATEMRAS_TFERQRCTRANSFER_GET_DATASIZE(pECQRCTransfer))
            );
    }

    ATEMRAS_TFERQRCTRANSFER_SET_SRVRESULT(pECQRCTransfer, dwRes);
    
    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, pbyTransfer, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emMbxTferCreate.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalMbxTferCreate(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_MBXCREATETRANSFER    pMbxCreTransfer     = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_MBXTFER_DESC               oMbxTferDesc;

    pMbxCreTransfer                  = (ATEMRAS_PT_MBXCREATETRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance                 = ATEMRAS_MBXCREATETRANSFER_GET_INSTANCEID(pMbxCreTransfer);
    oMbxTferDesc.dwMaxDataLen        = ATEMRAS_MBXCREATETRANSFER_GET_MAXDATALEN(pMbxCreTransfer);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emMbxTferCreate);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        oMbxTferDesc.pbyMbxTferDescData = (EC_T_BYTE*)OsMalloc(oMbxTferDesc.dwMaxDataLen);
        EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "deMarshalMbxTferCreate oMbxTferDesc.pbyMbxTferDescData", oMbxTferDesc.pbyMbxTferDescData, oMbxTferDesc.dwMaxDataLen);
        if (EC_NULL == oMbxTferDesc.pbyMbxTferDescData)
        {
            dwRes = emrasProtoCreateNackNoAlloc(pTransferHead, EC_E_NOMEMORY, (EC_T_BYTE*)pTransferHead, &dwWrtLen);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        }
        else
        {
            EC_T_MBXTFER* pLocMbx = emMbxTferCreate(dwMasterInstance, &oMbxTferDesc);
            if (EC_NULL == pLocMbx)
            {
                dwRes = emrasProtoCreateNackNoAlloc(pTransferHead, EC_E_NOMEMORY, (EC_T_BYTE*)pTransferHead, &dwWrtLen);
                if (EC_E_NOERROR != dwRes)
                {
                    dwRetVal = dwRes;
                    goto Exit;
                }
            }
            else
            {
                ATEMRAS_T_MBXTFER* pMbxRemTfer = ATEMRAS_MBXCREATETRANSFER_TFEROBJ(pMbxCreTransfer);
                pLocMbx->dwTferId = (EC_T_DWORD)((EC_T_BYTE*)pLocMbx - (EC_T_BYTE*)EC_NULL); /* set obj ID */

                /* enqueue mailbox object to storage */
                dwRes = m_pMasterStore->AddMbxTferObj(dwMasterInstance, pLocMbx);
                if (EC_E_NOERROR != dwRes)
                {
                    dwRetVal = dwRes;
                    goto Exit;
                }

                ATEMRAS_MBXCREATETRANSFER_SET_MBXOBJID(pMbxCreTransfer, pLocMbx->dwTferId);

                ATEMRAS_MBXTFERTRANSFER_SET_CLIENTID(pMbxRemTfer, pLocMbx->dwClntId);
                ATEMRAS_MBXTFERTRANSFER_SET_MBXTFERTYPE(pMbxRemTfer, (EC_T_DWORD)(pLocMbx->eMbxTferType));
                ATEMRAS_MBXTFERTRANSFER_SET_MAXDATALEN(pMbxRemTfer, pLocMbx->MbxTferDesc.dwMaxDataLen);
                ATEMRAS_MBXTFERTRANSFER_SET_DATALEN(pMbxRemTfer, pMbxRemTfer->dwDataLen);
                ATEMRAS_MBXTFERTRANSFER_SET_STATUS(pMbxRemTfer, ((EC_T_DWORD)(pLocMbx->eTferStatus)));
                ATEMRAS_MBXTFERTRANSFER_SET_ERRORCODE(pMbxRemTfer, pLocMbx->dwErrorCode);
            }
        }
    }

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emMbxTferDelete.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalMbxTferDelete(
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_MBXDELETETRANSFER    pMbxDelTransfer     = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    CAtEmMasterInstance*            pMasterInstance     = EC_NULL;
    CAtEmClientMbxTferObject*       pMbxAdm             = EC_NULL;
    EC_T_MBXTFER*                   pLocMbx             = EC_NULL;
    EC_T_BYTE*                      pbyMbxTferData      = EC_NULL;
       
    pMbxDelTransfer = (ATEMRAS_PT_MBXDELETETRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance = ATEMRAS_MBXDELETETRANSFER_GET_INSTANCEID(pMbxDelTransfer);
    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emMbxTferDelete);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        pMbxAdm = pMasterInstance->MbxTferObj(ATEMRAS_MBXDELETETRANSFER_GET_MBXOBJID(pMbxDelTransfer));
        if (EC_NULL != pMbxAdm)
        {
            CEcTimer oAbortTimeout(5000);
            pLocMbx = pMbxAdm->TferObj();
            OsDbgAssert(EC_NULL != pLocMbx);
            pbyMbxTferData = pLocMbx->pbyMbxTferData;

            if (eMbxTferStatus_Pend == pLocMbx->eTferStatus)
            {
                emMbxTferAbort(dwMasterInstance, pLocMbx);
            }
            while ((eMbxTferStatus_Pend == pLocMbx->eTferStatus) && !oAbortTimeout.IsElapsed())
            {
                OsSleep(10);
            }

            emMbxTferDelete(dwMasterInstance, pLocMbx);
            pMasterInstance->RemoveMbxTferObj(pMbxAdm);

            if (EC_NULL != pbyMbxTferData)
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "deMarshalMbxTferDelete", pbyMbxTferData, 0);
                OsFree(pbyMbxTferData);
            }
        }
    }
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emCoeSdoDownloadReq.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalCoeSdoDownloadReq(
    EC_T_BYTE*          pbyTransfer, 
    EC_T_DWORD          dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR*                 pTransferHead       = EC_NULL;
    CAtEmMasterInstance*                pMasterInstance     = EC_NULL;
    ATEMRAS_PT_COESDODOWNLOADTRANSFER   pCOESDODwnTransfer  = EC_NULL;
    ATEMRAS_PT_MBXTFER                  pMbxRemote          = EC_NULL;
    EC_T_DWORD                          dwMasterInstance    = 0;
    CAtEmClientMbxTferObject*           pMbxAdm             = EC_NULL;
    EC_T_MBXTFER*                       pLocMbx             = EC_NULL;
    
    pTransferHead               = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    pCOESDODwnTransfer          = (ATEMRAS_PT_COESDODOWNLOADTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    pMbxRemote                  = ATEMRAS_COESDODOWNLOADTRANSFER_TFEROBJ(pCOESDODwnTransfer);
    dwMasterInstance            = ATEMRAS_COESDODOWNLOADTRANSFER_GET_INSTANCEID(pCOESDODwnTransfer);

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    pMbxAdm = pMasterInstance->MbxTferObj(ATEMRAS_COESDODOWNLOADTRANSFER_GET_MBXOBJID(pCOESDODwnTransfer));
    if (EC_NULL == pMbxAdm)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pLocMbx = pMbxAdm->TferObj();
    OsDbgAssert(EC_NULL != pLocMbx);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emCoeSdoDownloadReq);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        pLocMbx->dwClntId = ATEMRAS_MBXTFERTRANSFER_GET_CLIENTID(pMbxRemote);
        OsDbgAssert(0 != pLocMbx->dwClntId); /* mbx transfer objects refreshed at AtemRasClnt by notification */
        pLocMbx->MbxTferDesc.dwMaxDataLen = ATEMRAS_MBXTFERTRANSFER_GET_MAXDATALEN(pMbxRemote);
        pLocMbx->dwDataLen = ATEMRAS_MBXTFERTRANSFER_GET_DATALEN(pMbxRemote);
        
        if (0 < pLocMbx->dwDataLen)
        {
            OsMemcpy(
                pLocMbx->pbyMbxTferData,
                (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead) + ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)),
                pLocMbx->dwDataLen
                );
        }
        
        pLocMbx->eTferStatus = (EC_T_MBXTFER_STATUS)ATEMRAS_MBXTFERTRANSFER_GET_STATUS(pMbxRemote);
        pLocMbx->dwErrorCode = ATEMRAS_MBXTFERTRANSFER_GET_ERRORCODE(pMbxRemote);
        OsDbgAssert(ATEMRAS_COESDODOWNLOADTRANSFER_GET_MBXOBJID(pCOESDODwnTransfer) == pLocMbx->dwTferId); /* mbx transfer objects refreshed at AtemRasSrv by notification */
        pMbxAdm->SetTferId(ATEMRAS_MBXTFERTRANSFER_GET_TRANSFERID(pMbxRemote)); /* store AtemRasClient MbxTfer Id */
        
        dwRes = emCoeSdoDownloadReq(
            dwMasterInstance,
            pLocMbx,
            ATEMRAS_COESDODOWNLOADTRANSFER_GET_SLAVEID(pCOESDODwnTransfer),
            ATEMRAS_COESDODOWNLOADTRANSFER_GET_OBINDEX(pCOESDODwnTransfer),
            ATEMRAS_COESDODOWNLOADTRANSFER_GET_OBSUBINDEX(pCOESDODwnTransfer),
            ATEMRAS_COESDODOWNLOADTRANSFER_GET_TIMEOUT(pCOESDODwnTransfer),
            ATEMRAS_COESDODOWNLOADTRANSFER_GET_FLAGS(pCOESDODwnTransfer)
            );
    }

    ATEMRAS_MBXTFERTRANSFER_SET_CLIENTID(pMbxRemote, pLocMbx->dwClntId);
    ATEMRAS_MBXTFERTRANSFER_SET_MBXTFERTYPE(pMbxRemote, pLocMbx->eMbxTferType);
    ATEMRAS_MBXTFERTRANSFER_SET_MAXDATALEN(pMbxRemote, pLocMbx->MbxTferDesc.dwMaxDataLen);
    
    if (EC_E_NOERROR != pLocMbx->dwErrorCode)
    {
        pLocMbx->dwDataLen = 0;
    }

    ATEMRAS_MBXTFERTRANSFER_SET_DATALEN(pMbxRemote, pLocMbx->dwDataLen);

    if ((0 < pLocMbx->dwDataLen) && (EC_E_NOERROR == pLocMbx->dwErrorCode))
    {
        OsMemcpy(
            (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)), 
            pLocMbx->pbyMbxTferData,
            pLocMbx->dwDataLen
            );
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_STATUS(pMbxRemote, (EC_T_DWORD)pLocMbx->eTferStatus);
    ATEMRAS_MBXTFERTRANSFER_SET_ERRORCODE(pMbxRemote, pLocMbx->dwErrorCode);
    ATEMRAS_MBXTFERTRANSFER_SET_TRANSFERID(pMbxRemote, pMbxAdm->GetTferId());
    
    ATEMRAS_COESDODOWNLOADTRANSFER_SET_SRVRESULT(pCOESDODwnTransfer, dwRes);
    
    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, pbyTransfer, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emCoeSdoUploadReq.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalCoeSdoUploadReq(
    EC_T_BYTE*      pbyTransfer,
    EC_T_DWORD      dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR*                 pTransferHead       = EC_NULL;
    CAtEmMasterInstance*                pMasterInstance     = EC_NULL;
    ATEMRAS_PT_COESDOUPLOADTRANSFER     pCOESDOUpTransfer   = EC_NULL;
    ATEMRAS_PT_MBXTFER                  pMbxRemote          = EC_NULL;
    EC_T_DWORD                          dwMasterInstance    = 0;
    CAtEmClientMbxTferObject*           pMbxAdm             = EC_NULL;
    EC_T_MBXTFER*                       pLocMbx             = EC_NULL;
    
    pTransferHead               = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    pCOESDOUpTransfer           = (ATEMRAS_PT_COESDOUPLOADTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    pMbxRemote                  = ATEMRAS_COESDOUPLOADTRANSFER_TFEROBJ(pCOESDOUpTransfer);
    dwMasterInstance            = ATEMRAS_COESDOUPLOADTRANSFER_GET_INSTANCEID(pCOESDOUpTransfer);

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    pMbxAdm = pMasterInstance->MbxTferObj(ATEMRAS_COESDOUPLOADTRANSFER_GET_MBXOBJID(pCOESDOUpTransfer));
    if (EC_NULL == pMbxAdm)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pLocMbx = pMbxAdm->TferObj();
    OsDbgAssert(EC_NULL != pLocMbx);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emCoeSdoUploadReq);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        pLocMbx->dwClntId = ATEMRAS_MBXTFERTRANSFER_GET_CLIENTID(pMbxRemote);
        OsDbgAssert(0 != pLocMbx->dwClntId); /* mbx transfer objects refreshed at AtemRasClnt by notification */
        pLocMbx->MbxTferDesc.dwMaxDataLen = ATEMRAS_MBXTFERTRANSFER_GET_MAXDATALEN(pMbxRemote);
        pLocMbx->dwDataLen = ATEMRAS_MBXTFERTRANSFER_GET_DATALEN(pMbxRemote);
        
        if (0 < pLocMbx->dwDataLen)
        {
            OsMemcpy(
                pLocMbx->pbyMbxTferData,
                (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead) + ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)),
                pLocMbx->dwDataLen
                );
        }
        
        pLocMbx->eTferStatus = (EC_T_MBXTFER_STATUS)ATEMRAS_MBXTFERTRANSFER_GET_STATUS(pMbxRemote);
        pLocMbx->dwErrorCode = ATEMRAS_MBXTFERTRANSFER_GET_ERRORCODE(pMbxRemote);
        OsDbgAssert(ATEMRAS_COESDOUPLOADTRANSFER_GET_MBXOBJID(pCOESDOUpTransfer) == pLocMbx->dwTferId); /* mbx transfer objects refreshed at AtemRasSrv by notification */
        pMbxAdm->SetTferId(ATEMRAS_MBXTFERTRANSFER_GET_TRANSFERID(pMbxRemote)); /* store AtemRasClient MbxTfer Id */
        
        dwRes = emCoeSdoUploadReq(
            dwMasterInstance,
            pLocMbx,
            ATEMRAS_COESDOUPLOADTRANSFER_GET_SLAVEID(pCOESDOUpTransfer),
            ATEMRAS_COESDOUPLOADTRANSFER_GET_OBINDEX(pCOESDOUpTransfer),
            ATEMRAS_COESDOUPLOADTRANSFER_GET_OBSUBINDEX(pCOESDOUpTransfer),
            ATEMRAS_COESDOUPLOADTRANSFER_GET_TIMEOUT(pCOESDOUpTransfer),
            ATEMRAS_COESDOUPLOADTRANSFER_GET_FLAGS(pCOESDOUpTransfer)
            );
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_CLIENTID(pMbxRemote, pLocMbx->dwClntId);
    ATEMRAS_MBXTFERTRANSFER_SET_MBXTFERTYPE(pMbxRemote, pLocMbx->eMbxTferType);
    ATEMRAS_MBXTFERTRANSFER_SET_MAXDATALEN(pMbxRemote, pLocMbx->MbxTferDesc.dwMaxDataLen);
    
    if (EC_E_NOERROR != pLocMbx->dwErrorCode)
    {
        pLocMbx->dwDataLen = 0;
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_DATALEN(pMbxRemote, pLocMbx->dwDataLen);
    
    if ((0 < pLocMbx->dwDataLen) && (EC_E_NOERROR == pLocMbx->dwErrorCode))
    {
        OsMemcpy(
            (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)), 
            pLocMbx->pbyMbxTferData,
            pLocMbx->dwDataLen
            );
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_STATUS(pMbxRemote, (EC_T_DWORD)pLocMbx->eTferStatus);
    ATEMRAS_MBXTFERTRANSFER_SET_ERRORCODE(pMbxRemote, pLocMbx->dwErrorCode);
    ATEMRAS_MBXTFERTRANSFER_SET_TRANSFERID(pMbxRemote, pMbxAdm->GetTferId());
    ATEMRAS_COESDOUPLOADTRANSFER_SET_SRVRESULT(pCOESDOUpTransfer, dwRes);
    
    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, pbyTransfer, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emCoeSdoDownload.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalCoeSdoDownload(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                              dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                              dwRes               = EC_E_ERROR;
    ATEMRAS_PT_COESDODOWNLOADCMPLTRANSFER   pCOESDODwnTransfer  = EC_NULL;
    EC_T_DWORD                              dwMasterInstance    = 0;
    CAtEmMasterInstance*                    pMasterInstance     = EC_NULL;

    pCOESDODwnTransfer          = (ATEMRAS_PT_COESDODOWNLOADCMPLTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_COESDODOWNLOADCMPLTRANSFER_GET_INSTANCEID(pCOESDODwnTransfer);

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emCoeSdoDownload);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emCoeSdoDownload(
            dwMasterInstance,
            ATEMRAS_COESDODOWNLOADCMPLTRANSFER_GET_SLAVEID(pCOESDODwnTransfer),
            ATEMRAS_COESDODOWNLOADCMPLTRANSFER_GET_OBINDEX(pCOESDODwnTransfer),
            ATEMRAS_COESDODOWNLOADCMPLTRANSFER_GET_OBSUBINDEX(pCOESDODwnTransfer),
            ATEMRAS_COESDODOWNLOADCMPLTRANSFER_DATA(pCOESDODwnTransfer),
            ATEMRAS_COESDODOWNLOADCMPLTRANSFER_GET_DATALEN(pCOESDODwnTransfer),
            ATEMRAS_COESDODOWNLOADCMPLTRANSFER_GET_TIMEOUT(pCOESDODwnTransfer),
            ATEMRAS_COESDODOWNLOADCMPLTRANSFER_GET_FLAGS(pCOESDODwnTransfer)
            );
    }
    
    ATEMRAS_COESDODOWNLOADCMPLTRANSFER_SET_SRVRESULT(pCOESDODwnTransfer, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emCoeSdoUpload.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalCoeSdoUpload(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_PT_COESDOUPLOADCMPLTRANSFER pCOESDOUpTransfer   = EC_NULL;
    EC_T_DWORD                          dwMasterInstance    = 0;
    CAtEmMasterInstance*                pMasterInstance     = EC_NULL;
    EC_T_DWORD                          dwOutDataLen        = 0;

    pCOESDOUpTransfer           = (ATEMRAS_PT_COESDOUPLOADCMPLTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_COESDOUPLOADCMPLTRANSFER_GET_INSTANCEID(pCOESDOUpTransfer);

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwOutDataLen = ATEMRAS_COESDOUPLOADCMPLTRANSFER_GET_OUTDATALEN(pCOESDOUpTransfer);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emCoeSdoUpload);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emCoeSdoUpload(
            dwMasterInstance,
            ATEMRAS_COESDOUPLOADCMPLTRANSFER_GET_SLAVEID(pCOESDOUpTransfer),
            ATEMRAS_COESDOUPLOADCMPLTRANSFER_GET_OBINDEX(pCOESDOUpTransfer),
            ATEMRAS_COESDOUPLOADCMPLTRANSFER_GET_OBSUBINDEX(pCOESDOUpTransfer),
            ATEMRAS_COESDOUPLOADCMPLTRANSFER_DATA(pCOESDOUpTransfer),
            ATEMRAS_COESDOUPLOADCMPLTRANSFER_GET_DATALEN(pCOESDOUpTransfer),
            &dwOutDataLen,
            ATEMRAS_COESDOUPLOADCMPLTRANSFER_GET_TIMEOUT(pCOESDOUpTransfer),
            ATEMRAS_COESDOUPLOADCMPLTRANSFER_GET_FLAGS(pCOESDOUpTransfer)
            );
    }
    
    ATEMRAS_COESDOUPLOADCMPLTRANSFER_SET_OUTDATALEN(pCOESDOUpTransfer, dwOutDataLen);
    ATEMRAS_COESDOUPLOADCMPLTRANSFER_SET_SRVRESULT(pCOESDOUpTransfer, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform deMarshalNotifyApp.

\return EC_E_NOERROR on success. error code otherwise.
*/

EC_T_DWORD CAtemRasSrvClient::deMarshalNotifyApp(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD           dwRetVal         = EC_E_ERROR;
    EC_T_DWORD           dwCode           = EC_E_ERROR;
    EC_T_DWORD           dwRes            = EC_E_ERROR;
    EC_T_BYTE*           pTransfer        = ATEMRAS_PROTOHDR_DATA(pTransferHead);
    EC_T_DWORD           dwMasterInstance = 0;
    CAtEmMasterInstance* pMasterInstance  = EC_NULL;
    EC_T_DWORD*          pdwNumOutData    = EC_NULL;
    EC_T_DWORD*          pResult          = EC_NULL;
    
    CAtemRasHelper cHlp;
    cHlp.setAsDword(0); // dwCode
    cHlp.setAsDword(0); // dwInBufSize
    cHlp.setAsDword(0); // dwOutBufSize
    cHlp.setAsDword(0); // pdwNumOutData
    cHlp.setAsDword(0); // pCallerData
    cHlp.setAsDword(0); // dwResult
    cHlp.setAsByte(0); // pbyInBuf (only symbolic)
    cHlp.setAsByte(0); // pbyOutBuf (only symbolic)
    if (!cHlp.analyzeReceiveBuffer(pTransfer))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    dwMasterInstance = cHlp.getInstanceIDforSrv();

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emNotifyApp);

    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        EC_T_NOTIFYPARMS   tParms;
#if (defined EC_BIG_ENDIAN)
        tParms.dwInBufSize = EC_DWORDSWAP(cHlp.getAsDword(1));
        tParms.dwOutBufSize = EC_DWORDSWAP(cHlp.getAsDword(2));
#else
        tParms.dwInBufSize = cHlp.getAsDword(1);
        tParms.dwOutBufSize = cHlp.getAsDword(2);
#endif /* EC_BIG_ENDIAN */

        tParms.pdwNumOutData = cHlp.getAsDwordPtr(3);
        tParms.pCallerData = (EC_T_BYTE*)EC_NULL + cHlp.getAsDword(4);
        tParms.pbyInBuf = cHlp.getAsBytePtr(6);
        tParms.pbyOutBuf = cHlp.getAsBytePtr(6) + tParms.dwInBufSize;

#if (defined EC_BIG_ENDIAN)
        dwCode = EC_DWORDSWAP(cHlp.getAsDword(0));
#else
        dwCode = cHlp.getAsDword(0);
#endif /* EC_BIG_ENDIAN */

        dwRes = emNotifyApp( dwMasterInstance, dwCode, &tParms);

        /* Set num of out data */
        pdwNumOutData = cHlp.getAsDwordPtr(3);
        EC_SET_FRM_DWORD(pdwNumOutData, *tParms.pdwNumOutData);
    }

    /* Set result */
    pResult = cHlp.getAsDwordPtr(5);
    EC_SET_FRM_DWORD(pResult, dwRes);

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emCoeGetODList.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalCoeGetODList(
    EC_T_BYTE*          pbyTransfer, 
    EC_T_DWORD          dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR*                 pTransferHead       = EC_NULL;
    ATEMRAS_PT_COEGETODLTRANSFER        pCOEODLGetTransfer  = EC_NULL;
    ATEMRAS_PT_MBXTFER                  pMbxRemote          = EC_NULL;
    EC_T_DWORD                          dwMasterInstance    = 0;
    CAtEmMasterInstance*                pMasterInstance     = EC_NULL;
    CAtEmClientMbxTferObject*           pMbxAdm             = EC_NULL;
    EC_T_MBXTFER*                       pLocMbx             = EC_NULL;
    EC_T_BYTE*                          pBuf                = EC_NULL;
    EC_T_DWORD                          dwByteSize          = 0;
#if (defined EC_BIG_ENDIAN)
    EC_T_WORD*                          pwOdList            = EC_NULL;
#endif /* EC_BIG_ENDIAN */

    VERBOSEOUT(2, (">deMarshalCoeGetODList()\n"));
    
    pTransferHead      = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    pCOEODLGetTransfer = (ATEMRAS_PT_COEGETODLTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    pMbxRemote         = ATEMRAS_COEGETODLTRANSFER_TFEROBJ(pCOEODLGetTransfer);
    dwMasterInstance   = ATEMRAS_COEGETODLTRANSFER_GET_INSTANCEID(pCOEODLGetTransfer);

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        VERBOSEOUT(1, ("deMarshalCoeGetODList(): Invalid MasterInstance!\n"));

        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    pMbxAdm = pMasterInstance->MbxTferObj(ATEMRAS_COEGETODLTRANSFER_GET_MBXOBJID(pCOEODLGetTransfer));
    if (EC_NULL == pMbxAdm)
    {
        VERBOSEOUT(1, ("deMarshalCoeGetODList(): Invalid Mailbox Object!\n"));
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pLocMbx = pMbxAdm->TferObj();
    OsDbgAssert(EC_NULL != pLocMbx);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emCoeGetODList);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        pLocMbx->dwClntId                    = ATEMRAS_MBXTFERTRANSFER_GET_CLIENTID(pMbxRemote);
        pLocMbx->MbxTferDesc.dwMaxDataLen    = ATEMRAS_MBXTFERTRANSFER_GET_MAXDATALEN(pMbxRemote);
        pLocMbx->dwDataLen                   = ATEMRAS_MBXTFERTRANSFER_GET_DATALEN(pMbxRemote);
    
        if (0 < pLocMbx->dwDataLen)
        {
            OsMemcpy(
                pLocMbx->pbyMbxTferData,
                (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)),
                pLocMbx->dwDataLen
                    );
        }
        pLocMbx->eTferStatus = (EC_T_MBXTFER_STATUS)ATEMRAS_MBXTFERTRANSFER_GET_STATUS(pMbxRemote);
        pLocMbx->dwErrorCode = ATEMRAS_MBXTFERTRANSFER_GET_ERRORCODE(pMbxRemote);
        OsDbgAssert(ATEMRAS_COEGETODLTRANSFER_GET_MBXOBJID(pCOEODLGetTransfer) == pLocMbx->dwTferId); /* mbx transfer objects refreshed at AtemRasSrv by notification */
        pMbxAdm->SetTferId(ATEMRAS_MBXTFERTRANSFER_GET_TRANSFERID(pMbxRemote)); /* store AtemRasClient MbxTfer Id */

        dwRes = emCoeGetODList(
            dwMasterInstance,
            pLocMbx,
            ATEMRAS_COEGETODLTRANSFER_GET_SLAVEID(pCOEODLGetTransfer),
            (EC_T_COE_ODLIST_TYPE)ATEMRAS_COEGETODLTRANSFER_GET_LISTTYPE(pCOEODLGetTransfer),
            ATEMRAS_COEGETODLTRANSFER_GET_TIMEOUT(pCOEODLGetTransfer)
                              );

        if (EC_E_NOERROR != dwRes)
        {
            VERBOSEOUT(1, ("deMarshalCoeGetODList(): API Call Error %s(0x%08X)\n", ecatGetText(dwRes), dwRes));
        }
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_CLIENTID(pMbxRemote, pLocMbx->dwClntId);
    ATEMRAS_MBXTFERTRANSFER_SET_MBXTFERTYPE(pMbxRemote, pLocMbx->eMbxTferType);
    ATEMRAS_MBXTFERTRANSFER_SET_MAXDATALEN(pMbxRemote, pLocMbx->MbxTferDesc.dwMaxDataLen);
    
    if (EC_E_NOERROR != pLocMbx->dwErrorCode)
    {
        pLocMbx->dwDataLen = 0;
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_DATALEN(pMbxRemote, pLocMbx->dwDataLen);
    
    if (0 < pLocMbx->dwDataLen)
    {
       pBuf = (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote));
       dwByteSize = pLocMbx->dwDataLen;
       OsMemcpy(
            pBuf,
           pLocMbx->pbyMbxTferData,
            dwByteSize
            );

#if (defined EC_BIG_ENDIAN)
       pwOdList = (EC_T_WORD *) pBuf;
       pwOdList++; // Skip list type!
       for (EC_T_INT i = 0; i < (EC_T_INT)(dwByteSize / sizeof(EC_T_WORD) - 1); ++i)
       {
          pwOdList[i] = EC_WORDSWAP(pwOdList[i]);
       }
#endif /* EC_BIG_ENDIAN */
    }

    ATEMRAS_MBXTFERTRANSFER_SET_STATUS(pMbxRemote, (EC_T_DWORD)pLocMbx->eTferStatus);
    ATEMRAS_MBXTFERTRANSFER_SET_ERRORCODE(pMbxRemote, pLocMbx->dwErrorCode);
    ATEMRAS_MBXTFERTRANSFER_SET_TRANSFERID(pMbxRemote, pMbxAdm->GetTferId());
    ATEMRAS_COEGETODLTRANSFER_SET_SRVRESULT(pCOEODLGetTransfer, dwRes);
                                                                                    
    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, pbyTransfer, EC_NULL, EC_NULL, EC_NULL);
    if (EC_E_NOERROR != dwRes)
    {
        VERBOSEOUT(1, ("deMarshalCoeGetODList(): Enqueue Response Error %s(0x%08X)\n", ecatGetText(dwRes), dwRes));
        dwRetVal = dwRes;
        goto Exit;
    }
    
    dwRetVal = EC_E_NOERROR;
Exit:
    VERBOSEOUT(2, ("<deMarshalCoeGetODList(...)=0x%08X\n", dwRetVal));
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Perform emCoeGetObjectDesc.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalCoeGetObjectDesc(
    EC_T_BYTE*          pbyTransfer, 
    EC_T_DWORD          dwWrtLen 
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR*                 pTransferHead       = EC_NULL;
    ATEMRAS_PT_COEGETOBDESCTRANSFER     pCOEOBDGetTransfer  = EC_NULL;
    ATEMRAS_PT_MBXTFER                  pMbxRemote          = EC_NULL;
    EC_T_DWORD                          dwMasterInstance    = 0;
    CAtEmMasterInstance*                pMasterInstance     = EC_NULL;
    CAtEmClientMbxTferObject*           pMbxAdm             = EC_NULL;
    EC_T_MBXTFER*                       pLocMbx             = EC_NULL;
#if (defined EC_BIG_ENDIAN)
    EC_T_COE_OBDESC*                    pObjDesc            = EC_NULL;
#endif /* EC_BIG_ENDIAN */

    pTransferHead               = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    pCOEOBDGetTransfer          = (ATEMRAS_PT_COEGETOBDESCTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    pMbxRemote                  = ATEMRAS_COEGETOBDESCTRANSFER_TFEROBJ(pCOEOBDGetTransfer);
    dwMasterInstance            = ATEMRAS_COEGETOBDESCTRANSFER_GET_INSTANCEID(pCOEOBDGetTransfer);

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    pMbxAdm = pMasterInstance->MbxTferObj(ATEMRAS_COEGETOBDESCTRANSFER_GET_MBXOBJID(pCOEOBDGetTransfer));
    if (EC_NULL == pMbxAdm)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pLocMbx = pMbxAdm->TferObj();
    OsDbgAssert(EC_NULL != pLocMbx);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emCoeGetObjectDesc);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        pLocMbx->dwClntId                    = ATEMRAS_MBXTFERTRANSFER_GET_CLIENTID(pMbxRemote);
        pLocMbx->MbxTferDesc.dwMaxDataLen    = ATEMRAS_MBXTFERTRANSFER_GET_MAXDATALEN(pMbxRemote);
        pLocMbx->dwDataLen                   = ATEMRAS_MBXTFERTRANSFER_GET_DATALEN(pMbxRemote);

        if (0 < pLocMbx->dwDataLen)
        {
            OsMemcpy(
                pLocMbx->pbyMbxTferData,
                (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)), 
                pLocMbx->dwDataLen
                    );
        }
        pLocMbx->eTferStatus = (EC_T_MBXTFER_STATUS)ATEMRAS_MBXTFERTRANSFER_GET_STATUS(pMbxRemote);
        pLocMbx->dwErrorCode = ATEMRAS_MBXTFERTRANSFER_GET_ERRORCODE(pMbxRemote);
        OsDbgAssert(ATEMRAS_COEGETOBDESCTRANSFER_GET_MBXOBJID(pCOEOBDGetTransfer) == pLocMbx->dwTferId); /* mbx transfer objects refreshed at AtemRasSrv by notification */
        pMbxAdm->SetTferId(ATEMRAS_MBXTFERTRANSFER_GET_TRANSFERID(pMbxRemote)); /* store AtemRasClient MbxTfer Id */

        dwRes = emCoeGetObjectDesc(
            dwMasterInstance,
            pLocMbx,
            ATEMRAS_COEGETOBDESCTRANSFER_GET_SLAVEID(pCOEOBDGetTransfer),
            EC_LOWORD(ATEMRAS_COEGETOBDESCTRANSFER_GET_OBINDEX(pCOEOBDGetTransfer)),
            ATEMRAS_COEGETOBDESCTRANSFER_GET_TIMEOUT(pCOEOBDGetTransfer)
                                  );
    }

    ATEMRAS_MBXTFERTRANSFER_SET_CLIENTID(pMbxRemote, pLocMbx->dwClntId);
    ATEMRAS_MBXTFERTRANSFER_SET_MBXTFERTYPE(pMbxRemote, pLocMbx->eMbxTferType);
    ATEMRAS_MBXTFERTRANSFER_SET_MAXDATALEN(pMbxRemote, pLocMbx->MbxTferDesc.dwMaxDataLen);
    
    if (EC_E_NOERROR != pLocMbx->dwErrorCode)
    {
        pLocMbx->dwDataLen = 0;
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_DATALEN(pMbxRemote, pLocMbx->dwDataLen);

    if (0 < pLocMbx->dwDataLen)
    {
#if (defined EC_BIG_ENDIAN)
        pObjDesc = &pLocMbx->MbxData.CoE_ObDesc;
        pObjDesc->wObIndex = EC_WORDSWAP(pObjDesc->wObIndex);
        pObjDesc->wObNameLen = EC_WORDSWAP(pObjDesc->wObNameLen);
        pObjDesc->wRes = EC_WORDSWAP(pObjDesc->wRes);
#endif /* EC_BIG_ENDIAN */

        OsMemcpy(
            (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)), 
            pLocMbx->pbyMbxTferData,
            pLocMbx->dwDataLen
            );
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_STATUS(pMbxRemote, (EC_T_DWORD)pLocMbx->eTferStatus);
    ATEMRAS_MBXTFERTRANSFER_SET_ERRORCODE(pMbxRemote, pLocMbx->dwErrorCode);
    ATEMRAS_MBXTFERTRANSFER_SET_TRANSFERID(pMbxRemote, pMbxAdm->GetTferId());
    ATEMRAS_COEGETOBDESCTRANSFER_SET_SRVRESULT(pCOEOBDGetTransfer, dwRes);
    
    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, pbyTransfer, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emCoeGetEntryDesc.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalCoeGetEntryDesc(
    EC_T_BYTE*      pbyTransfer, 
    EC_T_DWORD      dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR*                 pTransferHead       = EC_NULL;
    ATEMRAS_PT_COEGETENTDESCTRANSFER    pCOEGetEDTransfer   = EC_NULL;
    ATEMRAS_PT_MBXTFER                  pMbxRemote          = EC_NULL;
    EC_T_DWORD                          dwMasterInstance    = 0;
    CAtEmMasterInstance*                pMasterInstance     = EC_NULL;
    CAtEmClientMbxTferObject*           pMbxAdm             = EC_NULL;
    EC_T_MBXTFER*                       pLocMbx             = EC_NULL;

    pTransferHead               = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    pCOEGetEDTransfer           = (ATEMRAS_PT_COEGETENTDESCTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    pMbxRemote                  = ATEMRAS_COEGETENTDESCTRANSFER_TFEROBJ(pCOEGetEDTransfer);
    dwMasterInstance            = ATEMRAS_COEGETENTDESCTRANSFER_GET_INSTANCEID(pCOEGetEDTransfer);

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    pMbxAdm = pMasterInstance->MbxTferObj(ATEMRAS_COEGETENTDESCTRANSFER_GET_MBXOBJID(pCOEGetEDTransfer));
    if (EC_NULL == pMbxAdm)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pLocMbx = pMbxAdm->TferObj();
    OsDbgAssert(EC_NULL != pLocMbx);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emCoeGetEntryDesc);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        pLocMbx->dwClntId = ATEMRAS_MBXTFERTRANSFER_GET_CLIENTID(pMbxRemote);
        pLocMbx->MbxTferDesc.dwMaxDataLen = ATEMRAS_MBXTFERTRANSFER_GET_MAXDATALEN(pMbxRemote);
        pLocMbx->dwDataLen = ATEMRAS_MBXTFERTRANSFER_GET_DATALEN(pMbxRemote);

        if (0 < pLocMbx->dwDataLen)
        {
            OsMemcpy(
                pLocMbx->pbyMbxTferData,
                (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)), 
                pLocMbx->dwDataLen
                    );
        }
        pLocMbx->eTferStatus = (EC_T_MBXTFER_STATUS)ATEMRAS_MBXTFERTRANSFER_GET_STATUS(pMbxRemote);
        pLocMbx->dwErrorCode = ATEMRAS_MBXTFERTRANSFER_GET_ERRORCODE(pMbxRemote);
        OsDbgAssert(ATEMRAS_COEGETENTDESCTRANSFER_GET_MBXOBJID(pCOEGetEDTransfer) == pLocMbx->dwTferId); /* mbx transfer objects refreshed at AtemRasSrv by notification */
        pMbxAdm->SetTferId(ATEMRAS_MBXTFERTRANSFER_GET_TRANSFERID(pMbxRemote)); /* store AtemRasClient MbxTfer Id */
        
        dwRes = emCoeGetEntryDesc(
            dwMasterInstance,
            pLocMbx,
            ATEMRAS_COEGETENTDESCTRANSFER_GET_SLAVEID(pCOEGetEDTransfer),
            ATEMRAS_COEGETENTDESCTRANSFER_GET_OBINDEX(pCOEGetEDTransfer),
            ATEMRAS_COEGETENTDESCTRANSFER_GET_OBSUBINDEX(pCOEGetEDTransfer),
            ATEMRAS_COEGETENTDESCTRANSFER_GET_VALUEINFO(pCOEGetEDTransfer),
            ATEMRAS_COEGETENTDESCTRANSFER_GET_TIMEOUT(pCOEGetEDTransfer)
                                 );
    }

    ATEMRAS_MBXTFERTRANSFER_SET_CLIENTID(pMbxRemote, pLocMbx->dwClntId);
    ATEMRAS_MBXTFERTRANSFER_SET_MBXTFERTYPE(pMbxRemote, pLocMbx->eMbxTferType);
    ATEMRAS_MBXTFERTRANSFER_SET_MAXDATALEN(pMbxRemote, pLocMbx->MbxTferDesc.dwMaxDataLen);
    
    if (EC_E_NOERROR != pLocMbx->dwErrorCode)
    {
        pLocMbx->dwDataLen = 0;
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_DATALEN(pMbxRemote, pLocMbx->dwDataLen);
    
    if (0 < pLocMbx->dwDataLen)
    {
        OsMemcpy(
            (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)), 
            pLocMbx->pbyMbxTferData,
            pLocMbx->dwDataLen
            );
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_STATUS(pMbxRemote, (EC_T_DWORD)pLocMbx->eTferStatus);
    ATEMRAS_MBXTFERTRANSFER_SET_ERRORCODE(pMbxRemote, pLocMbx->dwErrorCode);
    ATEMRAS_MBXTFERTRANSFER_SET_TRANSFERID(pMbxRemote, pMbxAdm->GetTferId());
    ATEMRAS_COEGETENTDESCTRANSFER_SET_SRVRESULT(pCOEGetEDTransfer, dwRes);
                                                                                    
    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, pbyTransfer, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emGetProcessData.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetProcessData(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_PT_PROCESSDATATRANSFER      pProcDataTransfer   = EC_NULL;
    EC_T_DWORD                          dwMasterInstance    = 0;
    EC_T_BOOL                           bOutputData         = EC_FALSE;
    EC_T_DWORD                          dwOffset            = 0;
    EC_T_BYTE*                          pbyData             = EC_NULL;
    EC_T_DWORD                          dwLength            = 0;
    EC_T_DWORD                          dwTimeout           = EC_NOWAIT;

    pProcDataTransfer           = (ATEMRAS_PT_PROCESSDATATRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_PROCESSDATATRANSFER_GET_INSTANCEID(pProcDataTransfer);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetProcessData);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        bOutputData = ATEMRAS_PROCESSDATATRANSFER_GET_DIRECTION(pProcDataTransfer);
        dwOffset    = ATEMRAS_PROCESSDATATRANSFER_GET_OFFSET(pProcDataTransfer);
        pbyData     = ATEMRAS_PROCESSDATATRANSFER_DATA(pProcDataTransfer);
        dwLength    = ATEMRAS_PROCESSDATATRANSFER_GET_LENGTH(pProcDataTransfer);
        dwTimeout   = ATEMRAS_PROCESSDATATRANSFER_GET_TIMEOUT(pProcDataTransfer);

        dwRes = emGetProcessData(
            dwMasterInstance, 
            bOutputData, 
            dwOffset, 
            pbyData, 
            dwLength, 
            dwTimeout
        );
    }

    ATEMRAS_PROCESSDATATRANSFER_SET_SRVRESULT(pProcDataTransfer, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emSetProcessData.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalSetProcessData(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_PT_PROCESSDATATRANSFER      pProcDataTransfer   = EC_NULL;
    EC_T_DWORD                          dwMasterInstance    = 0;
    EC_T_BOOL                           bOutputData         = EC_FALSE;
    EC_T_DWORD                          dwOffset            = 0;
    EC_T_BYTE*                          pbyData             = EC_NULL;
    EC_T_DWORD                          dwLength            = 0;
    EC_T_DWORD                          dwTimeout           = EC_NOWAIT;

    pProcDataTransfer           = (ATEMRAS_PT_PROCESSDATATRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_PROCESSDATATRANSFER_GET_INSTANCEID(pProcDataTransfer);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSetProcessData);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
    bOutputData = ATEMRAS_PROCESSDATATRANSFER_GET_DIRECTION(pProcDataTransfer);
    dwOffset    = ATEMRAS_PROCESSDATATRANSFER_GET_OFFSET(pProcDataTransfer);
    pbyData     = ATEMRAS_PROCESSDATATRANSFER_DATA(pProcDataTransfer);
    dwLength    = ATEMRAS_PROCESSDATATRANSFER_GET_LENGTH(pProcDataTransfer);
    dwTimeout   = ATEMRAS_PROCESSDATATRANSFER_GET_TIMEOUT(pProcDataTransfer);
    
    dwRes = emSetProcessData(
        dwMasterInstance, 
        bOutputData, 
        dwOffset, 
        pbyData, 
        dwLength, 
        dwTimeout
                            );
    
    }

    ATEMRAS_PROCESSDATATRANSFER_SET_SRVRESULT(pProcDataTransfer, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emFoeFileDownload.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalFoeFileDownload(
    EC_T_BYTE*          pbyTransfer, 
    EC_T_DWORD          dwWrtLen
    )
{
    EC_T_DWORD                              dwRetVal         = EC_E_ERROR;
    EC_T_DWORD                              dwRes            = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR* pTransferHead    = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    ATEMRAS_T_FOEDOWNLOADCMPLTRANSFER*      pFoeDownTransfer = (ATEMRAS_T_FOEDOWNLOADCMPLTRANSFER*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    EC_T_DWORD                              dwMasterInstance = ATEMRAS_FOEDOWNLOADCMPLTRANSFER_GET_INSTANCEID(pFoeDownTransfer);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emFoeFileDownload);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emFoeFileDownload(
            dwMasterInstance, 
            ATEMRAS_FOEDOWNLOADCMPLTRANSFER_GET_SLAVEID(pFoeDownTransfer),
            ATEMRAS_FOEDOWNLOADCMPLTRANSFER_FILENAME(pFoeDownTransfer),
            ATEMRAS_FOEDOWNLOADCMPLTRANSFER_GET_FILENAMELENGH(pFoeDownTransfer, m_dwProtocolVersion),
            ATEMRAS_FOEDOWNLOADCMPLTRANSFER_DATA(pFoeDownTransfer, m_dwProtocolVersion),
            ATEMRAS_FOEDOWNLOADCMPLTRANSFER_GET_DATALEN(pFoeDownTransfer, m_dwProtocolVersion),
            ATEMRAS_FOEDOWNLOADCMPLTRANSFER_GET_PASSWD(pFoeDownTransfer, m_dwProtocolVersion),
            ATEMRAS_FOEDOWNLOADCMPLTRANSFER_GET_TIMEOUT(pFoeDownTransfer, m_dwProtocolVersion)
        );
    }

    ATEMRAS_FOEDOWNLOADCMPLTRANSFER_SET_SRVRESULT(pFoeDownTransfer, dwRes, m_dwProtocolVersion);
                                                                                    
    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, (EC_T_BYTE*)pTransferHead, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emFoeFileUpload.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalFoeFileUpload(
    EC_T_BYTE*      pbyTransfer, 
    EC_T_DWORD      dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal         = EC_E_ERROR;
    EC_T_DWORD                          dwRes            = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR* pTransferHead    = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    ATEMRAS_T_FOEUPLOADCMPLTRANSFER*    pFoeUpTransfer   = (ATEMRAS_T_FOEUPLOADCMPLTRANSFER*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    EC_T_DWORD                          dwMasterInstance = ATEMRAS_FOEUPLOADCMPLTRANSFER_GET_INSTANCEID(pFoeUpTransfer);
    EC_T_DWORD                          dwOutDataLen     = 0;

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emFoeFileUpload);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwOutDataLen = ATEMRAS_FOEUPLOADCMPLTRANSFER_GET_OUTDATALEN(pFoeUpTransfer, m_dwProtocolVersion);

        dwRes = emFoeFileUpload(
            dwMasterInstance,
            ATEMRAS_FOEUPLOADCMPLTRANSFER_GET_SLAVEID(pFoeUpTransfer),

            ATEMRAS_FOEUPLOADCMPLTRANSFER_FILENAME(pFoeUpTransfer),
            ATEMRAS_FOEUPLOADCMPLTRANSFER_GET_FILENAMELENGH(pFoeUpTransfer, m_dwProtocolVersion),
            ATEMRAS_FOEUPLOADCMPLTRANSFER_DATA(pFoeUpTransfer, m_dwProtocolVersion),
            ATEMRAS_FOEUPLOADCMPLTRANSFER_GET_DATALEN(pFoeUpTransfer, m_dwProtocolVersion),
            &dwOutDataLen,
            ATEMRAS_FOEUPLOADCMPLTRANSFER_GET_PASSWD(pFoeUpTransfer, m_dwProtocolVersion),
            ATEMRAS_FOEUPLOADCMPLTRANSFER_GET_TIMEOUT(pFoeUpTransfer, m_dwProtocolVersion)
        );
    }
    
    ATEMRAS_FOEUPLOADCMPLTRANSFER_SET_OUTDATALEN(pFoeUpTransfer, dwOutDataLen, m_dwProtocolVersion);
    ATEMRAS_FOEUPLOADCMPLTRANSFER_SET_SRVRESULT(pFoeUpTransfer, dwRes, m_dwProtocolVersion);
    
    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, (EC_T_BYTE*)pTransferHead, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emFoeFileUploadReq.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalFoeUploadReq(
    EC_T_PBYTE              pbyTransfer,    /**< [in]   Working buffer */
    EC_T_DWORD              dwWrtLen        /**< [in]   Size of working buffer */
)
{
    EC_T_DWORD                   dwRetVal         = EC_E_ERROR;
    EC_T_DWORD                   dwRes            = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR*          pTransferHead    = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    ATEMRAS_T_FOEUPLOADTRANSFER* pFoeUpTransfer   = (ATEMRAS_T_FOEUPLOADTRANSFER*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    ATEMRAS_T_MBXTFER*           pMbxRemote       = ATEMRAS_FOEUPLOADTRANSFER_TFEROBJ(pFoeUpTransfer, m_dwProtocolVersion);
    EC_T_DWORD                   dwMasterInstance = ATEMRAS_FOEUPLOADTRANSFER_GET_INSTANCEID(pFoeUpTransfer);
    CAtEmMasterInstance*         pMasterInstance  = EC_NULL;
    CAtEmClientMbxTferObject*    pMbxAdm          = EC_NULL;
    EC_T_MBXTFER*                pLocMbx          = EC_NULL;

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    pMbxAdm = pMasterInstance->MbxTferObj(ATEMRAS_FOEUPLOADTRANSFER_GET_MBXOBJID(pFoeUpTransfer));
    if (EC_NULL == pMbxAdm)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pLocMbx = pMbxAdm->TferObj();
    OsDbgAssert(EC_NULL != pLocMbx);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emFoeFileUpload);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        pLocMbx->dwClntId = ATEMRAS_MBXTFERTRANSFER_GET_CLIENTID(pMbxRemote);
        OsDbgAssert(0 != pLocMbx->dwClntId); /* mbx transfer objects refreshed at AtemRasClnt by notification */
        pLocMbx->MbxTferDesc.dwMaxDataLen = ATEMRAS_MBXTFERTRANSFER_GET_MAXDATALEN(pMbxRemote);
        pLocMbx->dwDataLen = ATEMRAS_MBXTFERTRANSFER_GET_DATALEN(pMbxRemote);

        if (pLocMbx->dwDataLen > 0)
        {
            OsMemcpy(
                pLocMbx->pbyMbxTferData,
                (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead) + ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)),
                pLocMbx->dwDataLen
                );
        }

        pLocMbx->eTferStatus = (EC_T_MBXTFER_STATUS)ATEMRAS_MBXTFERTRANSFER_GET_STATUS(pMbxRemote);
        pLocMbx->dwErrorCode = ATEMRAS_MBXTFERTRANSFER_GET_ERRORCODE(pMbxRemote);
        OsDbgAssert(ATEMRAS_FOEUPLOADTRANSFER_GET_MBXOBJID(pFoeUpTransfer) == pLocMbx->dwTferId); /* mbx transfer objects refreshed at AtemRasSrv by notification */
        pMbxAdm->SetTferId(ATEMRAS_MBXTFERTRANSFER_GET_TRANSFERID(pMbxRemote)); /* store AtemRasClient MbxTfer Id */
        //EcLogMsg(EC_LOG_LEVEL_VERBOSE_ACYC, (pEcLogContext, EC_LOG_LEVEL_VERBOSE_ACYC, "deMarshalFoeUploadReq: MbxTferObject ID: %d, MbxTfer ID %d\n", pLocMbx->dwTferId, pMbxAdm->GetTferId()));

        dwRes = emFoeUploadReq(
            dwMasterInstance,
            pLocMbx,
            ATEMRAS_FOEUPLOADTRANSFER_GET_SLAVEID(pFoeUpTransfer),
            ATEMRAS_FOEUPLOADTRANSFER_FILENAME(pFoeUpTransfer),
            ATEMRAS_FOEUPLOADTRANSFER_GET_FILENAMELENGH(pFoeUpTransfer, m_dwProtocolVersion),
            ATEMRAS_FOEUPLOADTRANSFER_GET_PASSWD(pFoeUpTransfer, m_dwProtocolVersion),
            ATEMRAS_FOEUPLOADTRANSFER_GET_TIMEOUT(pFoeUpTransfer, m_dwProtocolVersion)
        );
    }
    ATEMRAS_MBXTFERTRANSFER_SET_CLIENTID(pMbxRemote, pLocMbx->dwClntId);
    ATEMRAS_MBXTFERTRANSFER_SET_MBXTFERTYPE(pMbxRemote, pLocMbx->eMbxTferType);
    ATEMRAS_MBXTFERTRANSFER_SET_MAXDATALEN(pMbxRemote, pLocMbx->MbxTferDesc.dwMaxDataLen);

    if (EC_E_NOERROR != pLocMbx->dwErrorCode)
    {
        pLocMbx->dwDataLen = 0;
    }

    ATEMRAS_MBXTFERTRANSFER_SET_DATALEN(pMbxRemote, pLocMbx->dwDataLen);

    if ((pLocMbx->dwDataLen > 0) && (EC_E_NOERROR == pLocMbx->dwErrorCode))
    {
        OsMemcpy((EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead) + ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)),
            pLocMbx->pbyMbxTferData, pLocMbx->dwDataLen);
    }

    ATEMRAS_MBXTFERTRANSFER_SET_STATUS(pMbxRemote, (EC_T_DWORD)pLocMbx->eTferStatus);
    ATEMRAS_MBXTFERTRANSFER_SET_ERRORCODE(pMbxRemote, pLocMbx->dwErrorCode);
    ATEMRAS_MBXTFERTRANSFER_SET_TRANSFERID(pMbxRemote, pMbxAdm->GetTferId());
    ATEMRAS_FOEUPLOADTRANSFER_SET_SRVRESULT(pFoeUpTransfer, dwRes, m_dwProtocolVersion);

    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, (EC_T_BYTE*)pTransferHead, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emFoeDownloadReq.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD  CAtemRasSrvClient::deMarshalFoeDownloadReq(
    EC_T_PBYTE              pbyTransfer,    /**< [in]   Working buffer */
    EC_T_DWORD              dwWrtLen       /**< [in]   Size of working buffer */
)
{
    EC_T_DWORD                     dwRetVal         = EC_E_ERROR;
    EC_T_DWORD                     dwRes            = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR*            pTransferHead    = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    ATEMRAS_T_FOEDOWNLOADTRANSFER* pFoeDownTransfer = (ATEMRAS_T_FOEDOWNLOADTRANSFER*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    ATEMRAS_T_MBXTFER*             pMbxRemote       = ATEMRAS_FOEDOWNLOADTRANSFER_TFEROBJ(pFoeDownTransfer, m_dwProtocolVersion);
    EC_T_DWORD                     dwMasterInstance = ATEMRAS_FOEDOWNLOADTRANSFER_GET_INSTANCEID(pFoeDownTransfer);
    CAtEmMasterInstance*           pMasterInstance  = EC_NULL;
    CAtEmClientMbxTferObject*      pMbxAdm          = EC_NULL;
    EC_T_MBXTFER*                  pLocMbx          = EC_NULL;

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    pMbxAdm = pMasterInstance->MbxTferObj(ATEMRAS_FOEDOWNLOADTRANSFER_GET_MBXOBJID(pFoeDownTransfer));
    if (EC_NULL == pMbxAdm)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pLocMbx = pMbxAdm->TferObj();
    OsDbgAssert(EC_NULL != pLocMbx);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emFoeFileDownload);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        pLocMbx->dwClntId = ATEMRAS_MBXTFERTRANSFER_GET_CLIENTID(pMbxRemote);
        OsDbgAssert(0 != pLocMbx->dwClntId); /* mbx transfer objects refreshed at AtemRasClnt by notification */
        pLocMbx->MbxTferDesc.dwMaxDataLen = ATEMRAS_MBXTFERTRANSFER_GET_MAXDATALEN(pMbxRemote);
        pLocMbx->dwDataLen = ATEMRAS_MBXTFERTRANSFER_GET_DATALEN(pMbxRemote);

        if (pLocMbx->dwDataLen > 0)
        {
            OsMemcpy(
                pLocMbx->pbyMbxTferData,
                (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead) + ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)),
                pLocMbx->dwDataLen
                );
        }

        pLocMbx->eTferStatus = (EC_T_MBXTFER_STATUS)ATEMRAS_MBXTFERTRANSFER_GET_STATUS(pMbxRemote);
        pLocMbx->dwErrorCode = ATEMRAS_MBXTFERTRANSFER_GET_ERRORCODE(pMbxRemote);
        OsDbgAssert(ATEMRAS_FOEDOWNLOADTRANSFER_GET_MBXOBJID(pFoeDownTransfer) == pLocMbx->dwTferId); /* mbx transfer objects refreshed at AtemRasSrv by notification */
        pMbxAdm->SetTferId(ATEMRAS_MBXTFERTRANSFER_GET_TRANSFERID(pMbxRemote)); /* store AtemRasClient MbxTfer Id */
        //EcLogMsg(EC_LOG_LEVEL_VERBOSE_ACYC, (pEcLogContext, EC_LOG_LEVEL_VERBOSE_ACYC, "deMarshalFoeDownloadReq: MbxTferObject ID: %d, MbxTfer ID %d\n", pLocMbx->dwTferId, pMbxAdm->GetTferId()));

        dwRes = emFoeDownloadReq(
            dwMasterInstance,
            pLocMbx,
            ATEMRAS_FOEDOWNLOADTRANSFER_GET_SLAVEID(pFoeDownTransfer),
            ATEMRAS_FOEDOWNLOADTRANSFER_FILENAME(pFoeDownTransfer),
            ATEMRAS_FOEDOWNLOADTRANSFER_GET_FILENAMELENGH(pFoeDownTransfer, m_dwProtocolVersion),
            ATEMRAS_FOEDOWNLOADTRANSFER_GET_PASSWD(pFoeDownTransfer, m_dwProtocolVersion),
            ATEMRAS_FOEDOWNLOADTRANSFER_GET_TIMEOUT(pFoeDownTransfer, m_dwProtocolVersion)
        );
    }
    ATEMRAS_MBXTFERTRANSFER_SET_CLIENTID(pMbxRemote, pLocMbx->dwClntId);
    ATEMRAS_MBXTFERTRANSFER_SET_MBXTFERTYPE(pMbxRemote, pLocMbx->eMbxTferType);
    ATEMRAS_MBXTFERTRANSFER_SET_MAXDATALEN(pMbxRemote, pLocMbx->MbxTferDesc.dwMaxDataLen);

    if (EC_E_NOERROR != pLocMbx->dwErrorCode)
    {
        pLocMbx->dwDataLen = 0;
    }

    ATEMRAS_MBXTFERTRANSFER_SET_DATALEN(pMbxRemote, pLocMbx->dwDataLen);

    if ((pLocMbx->dwDataLen > 0) && (EC_E_NOERROR == pLocMbx->dwErrorCode))
    {
        OsMemcpy((EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead) + ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)),
            pLocMbx->pbyMbxTferData, pLocMbx->dwDataLen);
    }

    ATEMRAS_MBXTFERTRANSFER_SET_STATUS(pMbxRemote, (EC_T_DWORD)pLocMbx->eTferStatus);
    ATEMRAS_MBXTFERTRANSFER_SET_ERRORCODE(pMbxRemote, pLocMbx->dwErrorCode);
    ATEMRAS_MBXTFERTRANSFER_SET_TRANSFERID(pMbxRemote, pMbxAdm->GetTferId());
    ATEMRAS_FOEDOWNLOADTRANSFER_SET_SRVRESULT(pFoeDownTransfer, dwRes, m_dwProtocolVersion);

    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, (EC_T_BYTE*)pTransferHead, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emAoeWriteReq.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalAoeWriteReq(
    EC_T_BYTE*      pbyTransfer, 
    EC_T_DWORD      dwWrtLen
    )
{
    EC_T_DWORD                     dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                     dwRes               = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR*            pTransferHead       = EC_NULL;
    ATEMRAS_PT_AOEWRITEREQTRANSFER pTransferData       = EC_NULL;
    ATEMRAS_PT_MBXTFER             pMbxRemote          = EC_NULL;
    EC_T_DWORD                     dwMasterInstance    = 0;
    CAtEmMasterInstance*           pMasterInstance     = EC_NULL;
    CAtEmClientMbxTferObject*      pMbxAdm             = EC_NULL;
    EC_T_AOE_NETID                 oTargetNetId;
    EC_T_MBXTFER*                  pLocMbx             = EC_NULL;
    
    OsMemset(&oTargetNetId, 0, sizeof(EC_T_AOE_NETID));
    
    pTransferHead               = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    pTransferData               = (ATEMRAS_PT_AOEWRITEREQTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    pMbxRemote                  = ATEMRAS_AOEWRITEREQTRANSFER_TFEROBJ(pTransferData);
    dwMasterInstance            = ATEMRAS_AOEWRITEREQTRANSFER_GET_INSTANCEID(pTransferData);

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    pMbxAdm = pMasterInstance->MbxTferObj(ATEMRAS_AOEWRITEREQTRANSFER_GET_MBXOBJID(pTransferData));
    if (EC_NULL == pMbxAdm)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pLocMbx = pMbxAdm->TferObj();
    OsDbgAssert(EC_NULL != pLocMbx);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emAoeWriteReq);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        pLocMbx->dwClntId = ATEMRAS_MBXTFERTRANSFER_GET_CLIENTID(pMbxRemote);
        OsDbgAssert(0 != pLocMbx->dwClntId); /* mbx transfer objects refreshed at AtemRasClnt by notification */
        pLocMbx->MbxTferDesc.dwMaxDataLen = ATEMRAS_MBXTFERTRANSFER_GET_MAXDATALEN(pMbxRemote);
        pLocMbx->dwDataLen = ATEMRAS_MBXTFERTRANSFER_GET_DATALEN(pMbxRemote);

        if (0 < pLocMbx->dwDataLen)
        {
            OsMemcpy(
                pLocMbx->pbyMbxTferData,
                (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead) + ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)),
                pLocMbx->dwDataLen
                );
        }

        pLocMbx->eTferStatus = (EC_T_MBXTFER_STATUS)ATEMRAS_MBXTFERTRANSFER_GET_STATUS(pMbxRemote);
        pLocMbx->dwErrorCode = ATEMRAS_MBXTFERTRANSFER_GET_ERRORCODE(pMbxRemote);
        OsDbgAssert(ATEMRAS_AOEWRITEREQTRANSFER_GET_MBXOBJID(pTransferData) == pLocMbx->dwTferId); /* mbx transfer objects refreshed at AtemRasSrv by notification */
        pMbxAdm->SetTferId(ATEMRAS_MBXTFERTRANSFER_GET_TRANSFERID(pMbxRemote)); /* store AtemRasClient MbxTfer Id */
        
        dwRes = emAoeWriteReq(
            dwMasterInstance,
            pLocMbx,
            ATEMRAS_AOEWRITEREQTRANSFER_GET_SLAVEID(pTransferData),
            ATEMRAS_AOEWRITEREQTRANSFER_GET_TARGETNETID(pTransferData, &oTargetNetId),
            ATEMRAS_AOEWRITEREQTRANSFER_GET_TARGETPORT(pTransferData),
            ATEMRAS_AOEWRITEREQTRANSFER_GET_INDEXGROUP(pTransferData),
            ATEMRAS_AOEWRITEREQTRANSFER_GET_INDEXOFFSET(pTransferData),
            ATEMRAS_AOEWRITEREQTRANSFER_GET_TIMEOUT(pTransferData)
        );
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_CLIENTID(pMbxRemote, pLocMbx->dwClntId);
    ATEMRAS_MBXTFERTRANSFER_SET_MBXTFERTYPE(pMbxRemote, pLocMbx->eMbxTferType);
    ATEMRAS_MBXTFERTRANSFER_SET_MAXDATALEN(pMbxRemote, pLocMbx->MbxTferDesc.dwMaxDataLen);
    
    if (EC_E_NOERROR != pLocMbx->dwErrorCode)
    {
        pLocMbx->dwDataLen = 0;
    }

    ATEMRAS_MBXTFERTRANSFER_SET_DATALEN(pMbxRemote, pLocMbx->dwDataLen);

    if ((0 < pLocMbx->dwDataLen) && (EC_E_NOERROR == pLocMbx->dwErrorCode))
    {
        OsMemcpy(
            (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)),
            pLocMbx->pbyMbxTferData,
            pLocMbx->dwDataLen
            );
    }

    ATEMRAS_MBXTFERTRANSFER_SET_STATUS(pMbxRemote, (EC_T_DWORD)pLocMbx->eTferStatus);
    ATEMRAS_MBXTFERTRANSFER_SET_ERRORCODE(pMbxRemote, pLocMbx->dwErrorCode);
    ATEMRAS_MBXTFERTRANSFER_SET_TRANSFERID(pMbxRemote, pMbxAdm->GetTferId());
    ATEMRAS_AOEWRITEREQTRANSFER_SET_SRVRESULT(pTransferData, dwRes);
    
    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, pbyTransfer, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emAoeReadReq.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalAoeReadReq(
    EC_T_BYTE*      pbyTransfer, 
    EC_T_DWORD      dwWrtLen
    )
{
    EC_T_DWORD                    dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                    dwRes               = EC_E_ERROR;
    ATEMRAS_T_PROTOHDR*           pTransferHead       = EC_NULL;
    ATEMRAS_PT_AOEREADREQTRANSFER pTransferData       = EC_NULL;
    ATEMRAS_PT_MBXTFER            pMbxRemote          = EC_NULL;
    EC_T_DWORD                    dwMasterInstance    = 0;
    CAtEmMasterInstance*          pMasterInstance     = EC_NULL;
    CAtEmClientMbxTferObject*     pMbxAdm             = EC_NULL;
    EC_T_AOE_NETID                oTargetNetId;
    EC_T_MBXTFER*                 pLocMbx             = EC_NULL;
    
    OsMemset(&oTargetNetId, 0, sizeof(EC_T_AOE_NETID));
    
    pTransferHead               = (ATEMRAS_T_PROTOHDR*)pbyTransfer;
    pTransferData               = (ATEMRAS_PT_AOEREADREQTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    pMbxRemote                  = ATEMRAS_AOEREADREQTRANSFER_TFEROBJ(pTransferData);
    dwMasterInstance            = ATEMRAS_AOEREADREQTRANSFER_GET_INSTANCEID(pTransferData);

    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    pMbxAdm = pMasterInstance->MbxTferObj(ATEMRAS_AOEREADREQTRANSFER_GET_MBXOBJID(pTransferData));
    if (EC_NULL == pMbxAdm)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pLocMbx = pMbxAdm->TferObj();
    OsDbgAssert(EC_NULL != pLocMbx);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emAoeReadReq);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        pLocMbx->dwClntId = ATEMRAS_MBXTFERTRANSFER_GET_CLIENTID(pMbxRemote);
        OsDbgAssert(0 != pLocMbx->dwClntId); /* mbx transfer objects refreshed at AtemRasClnt by notification */
        pLocMbx->MbxTferDesc.dwMaxDataLen = ATEMRAS_MBXTFERTRANSFER_GET_MAXDATALEN(pMbxRemote);
        pLocMbx->dwDataLen = ATEMRAS_MBXTFERTRANSFER_GET_DATALEN(pMbxRemote);

        if (0 < pLocMbx->dwDataLen)
        {
            OsMemcpy(
                pLocMbx->pbyMbxTferData,
                (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead) + ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)),
                pLocMbx->dwDataLen
                );
        }
        
        pLocMbx->eTferStatus = (EC_T_MBXTFER_STATUS)ATEMRAS_MBXTFERTRANSFER_GET_STATUS(pMbxRemote);
        pLocMbx->dwErrorCode = ATEMRAS_MBXTFERTRANSFER_GET_ERRORCODE(pMbxRemote);
        OsDbgAssert(ATEMRAS_AOEREADREQTRANSFER_GET_MBXOBJID(pTransferData) == pLocMbx->dwTferId); /* mbx transfer objects refreshed at AtemRasSrv by notification */
        pMbxAdm->SetTferId(ATEMRAS_MBXTFERTRANSFER_GET_TRANSFERID(pMbxRemote)); /* store AtemRasClient MbxTfer Id */
        
        dwRes = emAoeReadReq(
            dwMasterInstance,
            pLocMbx,
            ATEMRAS_AOEREADREQTRANSFER_GET_SLAVEID(pTransferData),
            ATEMRAS_AOEREADREQTRANSFER_GET_TARGETNETID(pTransferData, &oTargetNetId),
            ATEMRAS_AOEREADREQTRANSFER_GET_TARGETPORT(pTransferData),
            ATEMRAS_AOEREADREQTRANSFER_GET_INDEXGROUP(pTransferData),
            ATEMRAS_AOEREADREQTRANSFER_GET_INDEXOFFSET(pTransferData),
            ATEMRAS_AOEREADREQTRANSFER_GET_TIMEOUT(pTransferData)
            );
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_CLIENTID(pMbxRemote, pLocMbx->dwClntId);
    ATEMRAS_MBXTFERTRANSFER_SET_MBXTFERTYPE(pMbxRemote, pLocMbx->eMbxTferType);
    ATEMRAS_MBXTFERTRANSFER_SET_MAXDATALEN(pMbxRemote, pLocMbx->MbxTferDesc.dwMaxDataLen);
    
    if (EC_E_NOERROR != pLocMbx->dwErrorCode)
    {
        pLocMbx->dwDataLen = 0;
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_DATALEN(pMbxRemote, pLocMbx->dwDataLen);
    
    if ((0 < pLocMbx->dwDataLen) && (EC_E_NOERROR == pLocMbx->dwErrorCode))
    {
        OsMemcpy(
            (EC_T_BYTE*)(ATEMRAS_PROTOHDR_DATA(pTransferHead)+ATEMRAS_MBXTFERTRANSFER_GET_DATAOFFSET(pMbxRemote)), 
            pLocMbx->pbyMbxTferData,
            pLocMbx->dwDataLen
            );
    }
    
    ATEMRAS_MBXTFERTRANSFER_SET_STATUS(pMbxRemote, (EC_T_DWORD)pLocMbx->eTferStatus);
    ATEMRAS_MBXTFERTRANSFER_SET_ERRORCODE(pMbxRemote, pLocMbx->dwErrorCode);
    ATEMRAS_MBXTFERTRANSFER_SET_TRANSFERID(pMbxRemote, pMbxAdm->GetTferId());
    ATEMRAS_AOEREADREQTRANSFER_SET_SRVRESULT(pTransferData, dwRes);
    
    dwRes = m_pCmdQueue->AddEntry(ATEMRAS_PROTOHDR_GET_SEQUENCENR(pTransferHead), dwWrtLen, pbyTransfer, EC_NULL, EC_NULL, EC_NULL);
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
\brief  Perform emAoeWrite.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalAoeWrite(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                  dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                  dwRes               = EC_E_ERROR;
    ATEMRAS_PT_AOEWRITETRANSFER pTransferData       = EC_NULL;
    EC_T_DWORD                  dwMasterInstance    = 0;
    EC_T_AOE_NETID              oTargetNetId;
    EC_T_DWORD                  dwErrorCode         = 0;
    EC_T_DWORD                  dwCmdResult         = 0;
    
    OsMemset(&oTargetNetId, 0, sizeof(EC_T_AOE_NETID));
    
    pTransferData          = (ATEMRAS_PT_AOEWRITETRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance       = ATEMRAS_AOEWRITETRANSFER_GET_INSTANCEID(pTransferData);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emAoeWrite);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwErrorCode = ATEMRAS_AOEWRITETRANSFER_GET_ERRORCODE(pTransferData);
        dwCmdResult = ATEMRAS_AOEWRITETRANSFER_GET_CMDRESULT(pTransferData);
        
        dwRes = emAoeWrite(
            dwMasterInstance, 
            ATEMRAS_AOEWRITETRANSFER_GET_SLAVEID(pTransferData),
            ATEMRAS_AOEWRITETRANSFER_GET_TARGETNETID(pTransferData, &oTargetNetId),
            ATEMRAS_AOEWRITETRANSFER_GET_TARGETPORT(pTransferData),
            ATEMRAS_AOEWRITETRANSFER_GET_INDEXGROUP(pTransferData),
            ATEMRAS_AOEWRITETRANSFER_GET_INDEXOFFSET(pTransferData),
            ATEMRAS_AOEWRITETRANSFER_GET_DATALENGTH(pTransferData),
            ATEMRAS_AOEWRITETRANSFER_DATA(pTransferData),
            &dwErrorCode,
            &dwCmdResult,
            ATEMRAS_AOEWRITETRANSFER_GET_TIMEOUT(pTransferData)
            );
    }
    
    ATEMRAS_AOEWRITETRANSFER_SET_ERRORCODE(pTransferData, dwErrorCode);
    ATEMRAS_AOEWRITETRANSFER_SET_CMDRESULT(pTransferData, dwCmdResult);
    ATEMRAS_AOEWRITETRANSFER_SET_SRVRESULT(pTransferData, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emAoeRead

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalAoeRead(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                 dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                 dwRes               = EC_E_ERROR;
    ATEMRAS_PT_AOEREADTRANSFER pTransferData       = EC_NULL;
    EC_T_DWORD                 dwMasterInstance    = 0;
    EC_T_DWORD                 dwOutDataLen        = 0;
    EC_T_DWORD                 dwErrorCode         = 0;
    EC_T_DWORD                 dwCmdResult         = 0;
    EC_T_AOE_NETID             oTargetNetId;
    
    OsMemset(&oTargetNetId, 0, sizeof(EC_T_AOE_NETID));
    
    pTransferData              = (ATEMRAS_PT_AOEREADTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance           = ATEMRAS_AOEREADTRANSFER_GET_INSTANCEID(pTransferData);

    dwOutDataLen = ATEMRAS_AOEREADTRANSFER_GET_OUTDATALEN(pTransferData);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emAoeRead);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwErrorCode = ATEMRAS_AOEREADTRANSFER_GET_TIMEOUT(pTransferData);
        dwCmdResult = ATEMRAS_AOEREADTRANSFER_GET_CMDRESULT(pTransferData);

        dwRes = emAoeRead(
            dwMasterInstance,
            ATEMRAS_AOEREADTRANSFER_GET_SLAVEID(pTransferData),
            ATEMRAS_AOEREADTRANSFER_GET_TARGETNETID(pTransferData, &oTargetNetId),
            ATEMRAS_AOEREADTRANSFER_GET_TARGETPORT(pTransferData),
            ATEMRAS_AOEREADTRANSFER_GET_INDEXGROUP(pTransferData),
            ATEMRAS_AOEREADTRANSFER_GET_INDEXOFFSET(pTransferData),
            ATEMRAS_AOEREADTRANSFER_GET_DATALENGTH(pTransferData),
            ATEMRAS_AOEREADTRANSFER_DATA(pTransferData),
            &dwOutDataLen,
            &dwErrorCode,
            &dwCmdResult,
            ATEMRAS_AOEREADTRANSFER_GET_TIMEOUT(pTransferData)
            );
    }
    
    ATEMRAS_AOEREADTRANSFER_SET_OUTDATALEN(pTransferData, dwOutDataLen);
    ATEMRAS_AOEREADTRANSFER_SET_ERRORCODE(pTransferData, dwErrorCode);
    ATEMRAS_AOEREADTRANSFER_SET_CMDRESULT(pTransferData, dwCmdResult);
    ATEMRAS_AOEREADTRANSFER_SET_SRVRESULT(pTransferData, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emAoeReadWrite.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalAoeReadWrite(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                 dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                 dwRes               = EC_E_ERROR;
    ATEMRAS_PT_AOEREADWRITETRANSFER pTransferData  = EC_NULL;
    EC_T_DWORD                 dwMasterInstance    = 0;
    EC_T_DWORD                 dwOutDataLen        = 0;
    EC_T_DWORD                 dwErrorCode         = 0;
    EC_T_DWORD                 dwCmdResult         = 0;
    EC_T_AOE_NETID             oTargetNetId;
    
    OsMemset(&oTargetNetId, 0, sizeof(EC_T_AOE_NETID));
    
    pTransferData              = (ATEMRAS_PT_AOEREADWRITETRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance           = ATEMRAS_AOEREADWRITETRANSFER_GET_INSTANCEID(pTransferData);

    dwOutDataLen = ATEMRAS_AOEREADWRITETRANSFER_GET_OUTDATALEN(pTransferData);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emAoeReadWrite);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwErrorCode = ATEMRAS_AOEREADWRITETRANSFER_GET_TIMEOUT(pTransferData);
        dwCmdResult = ATEMRAS_AOEREADWRITETRANSFER_GET_CMDRESULT(pTransferData);

        dwRes = emAoeReadWrite(
            dwMasterInstance,
            ATEMRAS_AOEREADWRITETRANSFER_GET_SLAVEID(pTransferData),
            ATEMRAS_AOEREADWRITETRANSFER_GET_TARGETNETID(pTransferData, &oTargetNetId),
            ATEMRAS_AOEREADWRITETRANSFER_GET_TARGETPORT(pTransferData),
            ATEMRAS_AOEREADWRITETRANSFER_GET_INDEXGROUP(pTransferData),
            ATEMRAS_AOEREADWRITETRANSFER_GET_INDEXOFFSET(pTransferData),
            ATEMRAS_AOEREADWRITETRANSFER_GET_READDATALENGTH(pTransferData),
            ATEMRAS_AOEREADWRITETRANSFER_GET_WRITEDATALENGTH(pTransferData),
            ATEMRAS_AOEREADWRITETRANSFER_DATA(pTransferData),
            &dwOutDataLen,
            &dwErrorCode,
            &dwCmdResult,
            ATEMRAS_AOEREADWRITETRANSFER_GET_TIMEOUT(pTransferData)
            );
    }

    
    ATEMRAS_AOEREADWRITETRANSFER_SET_OUTDATALEN(pTransferData, dwOutDataLen);
    ATEMRAS_AOEREADWRITETRANSFER_SET_ERRORCODE(pTransferData, dwErrorCode);
    ATEMRAS_AOEREADWRITETRANSFER_SET_CMDRESULT(pTransferData, dwCmdResult);
    ATEMRAS_AOEREADWRITETRANSFER_SET_SRVRESULT(pTransferData, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emAoeGetSlaveNetId.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalAoeGetSlaveNetId(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_PT_AOEGETSLAVENETIDTRANSFER pTransferData       = EC_NULL;
    EC_T_DWORD                          dwMasterInstance    = 0;
    EC_T_AOE_NETID                      oTargetNetId;
    
    OsMemset(&oTargetNetId, 0, sizeof(EC_T_AOE_NETID));
    
    pTransferData               = (ATEMRAS_PT_AOEGETSLAVENETIDTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_AOEGETSLAVENETIDTRANSFER_GET_INSTANCEID(pTransferData);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emAoeGetSlaveNetId);
    if (EC_E_NOERROR == dwRes)

#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emAoeGetSlaveNetId(
            dwMasterInstance,
            ATEMRAS_AOEGETSLAVENETIDTRANSFER_GET_SLAVEID(pTransferData),
            ATEMRAS_AOEGETSLAVENETIDTRANSFER_GET_TARGETNETID(pTransferData, &oTargetNetId)
            );
    }

    ATEMRAS_AOEGETSLAVENETIDTRANSFER_SET_SRVRESULT(pTransferData, dwRes);
    ATEMRAS_AOEGETSLAVENETIDTRANSFER_SET_TARGETNETID(pTransferData, &oTargetNetId);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emAoeWriteControl.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalAoeWriteControl(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                  dwRetVal = EC_E_ERROR;
    EC_T_DWORD                  dwRes = EC_E_ERROR;
    ATEMRAS_T_AOEWRITECONTROLTRANSFER* pTransferData = EC_NULL;
    EC_T_DWORD                  dwMasterInstance = 0;
    EC_T_AOE_NETID              oTargetNetId;
    EC_T_DWORD                  dwErrorCode = 0;
    EC_T_DWORD                  dwCmdResult = 0;
    
    OsMemset(&oTargetNetId, 0, sizeof(EC_T_AOE_NETID));
    
    pTransferData = (ATEMRAS_T_AOEWRITECONTROLTRANSFER*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance = ATEMRAS_AOEWRITECONTROLTRANSFER_GET_INSTANCEID(pTransferData);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emAoeWriteControl);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwErrorCode = ATEMRAS_AOEWRITECONTROLTRANSFER_GET_ERRORCODE(pTransferData);
        dwCmdResult = ATEMRAS_AOEWRITECONTROLTRANSFER_GET_CMDRESULT(pTransferData);

        dwRes = emAoeWriteControl(
            dwMasterInstance,
            ATEMRAS_AOEWRITECONTROLTRANSFER_GET_SLAVEID(pTransferData),
            ATEMRAS_AOEWRITECONTROLTRANSFER_GET_TARGETNETID(pTransferData, &oTargetNetId),
            ATEMRAS_AOEWRITECONTROLTRANSFER_GET_TARGETPORT(pTransferData),
            ATEMRAS_AOEWRITECONTROLTRANSFER_GET_AOESTATE(pTransferData),
            ATEMRAS_AOEWRITECONTROLTRANSFER_GET_DEVICESTATE(pTransferData),
            ATEMRAS_AOEWRITECONTROLTRANSFER_GET_DATALENGTH(pTransferData),
            ATEMRAS_AOEWRITECONTROLTRANSFER_DATA(pTransferData),
            &dwErrorCode,
            &dwCmdResult,
            ATEMRAS_AOEWRITECONTROLTRANSFER_GET_TIMEOUT(pTransferData)
            );
    }

    ATEMRAS_AOEWRITECONTROLTRANSFER_SET_ERRORCODE(pTransferData, dwErrorCode);
    ATEMRAS_AOEWRITECONTROLTRANSFER_SET_CMDRESULT(pTransferData, dwCmdResult);
    ATEMRAS_AOEWRITECONTROLTRANSFER_SET_SRVRESULT(pTransferData, dwRes);

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetConnectedSlaves.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetNumHelper(
    ATEMRAS_T_APICMDTYPE        eCmdType,
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                          dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                          dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMGETNUMCONSLVTRANSFER   pECGetConSlvTransf  = EC_NULL;
    EC_T_DWORD                          dwMasterInstance    = 0;
    
    pECGetConSlvTransf          = (ATEMRAS_PT_EMGETNUMCONSLVTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_EMGETNUMCONSLVTRANSFER_GET_INSTANCEID(pECGetConSlvTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    if (eCmdType == emrasapicmd_emGetNumConnectedSlaves)
      dwRes = m_pServer->GetAccessLevelByCall(ord_emGetNumConnectedSlaves);
    else if (eCmdType == emrasapicmd_emGetNumConnectedSlavesMain)
      dwRes = m_pServer->GetAccessLevelByCall(ord_emGetNumConnectedSlavesMain);
    else if (eCmdType == emrasapicmd_emGetNumConnectedSlavesRed)
      dwRes = m_pServer->GetAccessLevelByCall(ord_emGetNumConnectedSlavesRed);
    
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        if (eCmdType == emrasapicmd_emGetNumConnectedSlaves)
            dwRes = emGetNumConnectedSlaves(dwMasterInstance);
        else if (eCmdType == emrasapicmd_emGetNumConnectedSlavesMain)
            dwRes = emGetNumConnectedSlavesMain(dwMasterInstance);
        else if (eCmdType == emrasapicmd_emGetNumConnectedSlavesRed)
            dwRes = emGetNumConnectedSlavesRed(dwMasterInstance);
    
    }
    
    ATEMRAS_EMGETNUMCONSLVTRANSFER_SET_NUMSLV(pECGetConSlvTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetConnectedSlaves.

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalResetSlaveController(
    ATEMRAS_T_PROTOHDR*     pTransferHead,
    EC_T_DWORD              dwWrtLen
    )
{
    EC_T_DWORD                              dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                              dwRes               = EC_E_ERROR;
    ATEMRAS_PT_RESETSLAVECONTROLERTRANSFER  pECResSlvCtlTransf  = EC_NULL;
    EC_T_DWORD                              dwMasterInstance    = 0;
    EC_T_BOOL                               bFixedAddressing    = EC_FALSE;
    EC_T_WORD                               wSlaveAddress       = 1;
    EC_T_DWORD                              dwTimeout           = EC_WAITINFINITE;
    
    pECResSlvCtlTransf          = (ATEMRAS_PT_RESETSLAVECONTROLERTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_RESETSLAVECONTROLERTRANSFER_GET_INSTANCEID(pECResSlvCtlTransf);
    bFixedAddressing            = ATEMRAS_RESETSLAVECONTROLERTRANSFER_GET_FIXEDADDR(pECResSlvCtlTransf);
    wSlaveAddress               = ATEMRAS_RESETSLAVECONTROLERTRANSFER_GET_SLVADDR(pECResSlvCtlTransf);
    dwTimeout                   = ATEMRAS_RESETSLAVECONTROLERTRANSFER_GET_TIMEOUT(pECResSlvCtlTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emResetSlaveController);
    if (EC_E_NOERROR == dwRes)

#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emResetSlaveController( dwMasterInstance, bFixedAddressing, wSlaveAddress, dwTimeout);
    }
    
    ATEMRAS_RESETSLAVECONTROLERTRANSFER_SET_RESULT(pECResSlvCtlTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetSlaveInfo

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSlaveInfo(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                              dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                              dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMGETSLAVEINFOTRANSFER       pECResSlvCtlTransf  = EC_NULL;
    EC_T_DWORD                              dwMasterInstance    = 0;
    EC_T_BOOL                               bFixedAddressing    = EC_FALSE;
    EC_T_WORD                               wSlaveAddress       = 1;
    EC_T_GET_SLAVE_INFO*                    poGetSlaveInfo      = EC_NULL;
        
    pECResSlvCtlTransf          = (ATEMRAS_PT_EMGETSLAVEINFOTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_EMGETSLAVEINFOTRANSFER_GET_INSTANCEID(pECResSlvCtlTransf);
    bFixedAddressing            = ATEMRAS_EMGETSLAVEINFOTRANSFER_GET_FIXEDADDR(pECResSlvCtlTransf);
    wSlaveAddress               = ATEMRAS_EMGETSLAVEINFOTRANSFER_GET_SLVADDR(pECResSlvCtlTransf);
    poGetSlaveInfo              = ATEMRAS_EMGETSLAVEINFOTRANSFER_GET_GETSLAVEINFO_POINTER(pECResSlvCtlTransf);


#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetSlaveInfo);
    if (EC_E_NOERROR == dwRes)

#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emGetSlaveInfo(dwMasterInstance, bFixedAddressing, wSlaveAddress, poGetSlaveInfo);

#if (defined EC_BIG_ENDIAN)
        poGetSlaveInfo->dwScanBusStatus = EC_DWORDSWAP(poGetSlaveInfo->dwScanBusStatus);
        poGetSlaveInfo->dwVendorId = EC_DWORDSWAP(poGetSlaveInfo->dwVendorId);
        poGetSlaveInfo->dwProductCode = EC_DWORDSWAP(poGetSlaveInfo->dwProductCode);
        poGetSlaveInfo->dwRevisionNumber = EC_DWORDSWAP(poGetSlaveInfo->dwRevisionNumber);
        poGetSlaveInfo->dwSerialNumber = EC_DWORDSWAP(poGetSlaveInfo->dwSerialNumber);
        poGetSlaveInfo->wPortState = EC_WORDSWAP(poGetSlaveInfo->wPortState);
        poGetSlaveInfo->wReserved = EC_WORDSWAP(poGetSlaveInfo->wReserved);
        poGetSlaveInfo->bDcSupport = EC_DWORDSWAP(poGetSlaveInfo->bDcSupport);
        poGetSlaveInfo->bDc64Support = EC_DWORDSWAP(poGetSlaveInfo->bDc64Support);
        poGetSlaveInfo->wAliasAddress = EC_WORDSWAP(poGetSlaveInfo->wAliasAddress);
        poGetSlaveInfo->wPhysAddress = EC_WORDSWAP(poGetSlaveInfo->wPhysAddress);
        poGetSlaveInfo->dwPdOffsIn = EC_DWORDSWAP(poGetSlaveInfo->dwPdOffsIn);
        poGetSlaveInfo->dwPdSizeIn = EC_DWORDSWAP(poGetSlaveInfo->dwPdSizeIn);
        poGetSlaveInfo->dwPdOffsOut = EC_DWORDSWAP(poGetSlaveInfo->dwPdOffsOut);
        poGetSlaveInfo->dwPdSizeOut = EC_DWORDSWAP(poGetSlaveInfo->dwPdSizeOut);
        poGetSlaveInfo->dwPdOffsIn2 = EC_DWORDSWAP(poGetSlaveInfo->dwPdOffsIn2);
        poGetSlaveInfo->dwPdSizeIn2 = EC_DWORDSWAP(poGetSlaveInfo->dwPdSizeIn2);
        poGetSlaveInfo->dwPdOffsOut2 = EC_DWORDSWAP(poGetSlaveInfo->dwPdOffsOut2);
        poGetSlaveInfo->dwPdSizeOut2 = EC_DWORDSWAP(poGetSlaveInfo->dwPdSizeOut2);
        poGetSlaveInfo->dwPdOffsIn3 = EC_DWORDSWAP(poGetSlaveInfo->dwPdOffsIn3);
        poGetSlaveInfo->dwPdSizeIn3 = EC_DWORDSWAP(poGetSlaveInfo->dwPdSizeIn3);
        poGetSlaveInfo->dwPdOffsOut3 = EC_DWORDSWAP(poGetSlaveInfo->dwPdOffsOut3);
        poGetSlaveInfo->dwPdSizeOut3 = EC_DWORDSWAP(poGetSlaveInfo->dwPdSizeOut3);
        poGetSlaveInfo->dwPdOffsIn4 = EC_DWORDSWAP(poGetSlaveInfo->dwPdOffsIn4);
        poGetSlaveInfo->dwPdSizeIn4 = EC_DWORDSWAP(poGetSlaveInfo->dwPdSizeIn4);
        poGetSlaveInfo->dwPdOffsOut4 = EC_DWORDSWAP(poGetSlaveInfo->dwPdOffsOut4);
        poGetSlaveInfo->dwPdSizeOut4 = EC_DWORDSWAP(poGetSlaveInfo->dwPdSizeOut4);
        poGetSlaveInfo->wCfgPhyAddress = EC_WORDSWAP(poGetSlaveInfo->wCfgPhyAddress);
        poGetSlaveInfo->wReserved2 = EC_WORDSWAP(poGetSlaveInfo->wReserved2);
        poGetSlaveInfo->bIsMailboxSlave = EC_DWORDSWAP(poGetSlaveInfo->bIsMailboxSlave);
        poGetSlaveInfo->dwMbxOutSize = EC_DWORDSWAP(poGetSlaveInfo->dwMbxOutSize);
        poGetSlaveInfo->dwMbxInSize = EC_DWORDSWAP(poGetSlaveInfo->dwMbxInSize);
        poGetSlaveInfo->dwMbxOutSize2 = EC_DWORDSWAP(poGetSlaveInfo->dwMbxOutSize2);
        poGetSlaveInfo->dwMbxInSize2 = EC_DWORDSWAP(poGetSlaveInfo->dwMbxInSize2);
        poGetSlaveInfo->dwErrorCode = EC_DWORDSWAP(poGetSlaveInfo->dwErrorCode);
        poGetSlaveInfo->dwSBErrorCode = EC_DWORDSWAP(poGetSlaveInfo->dwSBErrorCode);
        poGetSlaveInfo->wSupportedMbxProtocols = EC_WORDSWAP(poGetSlaveInfo->wSupportedMbxProtocols);
        poGetSlaveInfo->wAlStatusValue = EC_WORDSWAP(poGetSlaveInfo->wAlStatusValue);
        poGetSlaveInfo->wAlStatusCode = EC_WORDSWAP(poGetSlaveInfo->wAlStatusCode);
        poGetSlaveInfo->bIsOptional = EC_DWORDSWAP(poGetSlaveInfo->bIsOptional);
        poGetSlaveInfo->bIsPresent = EC_DWORDSWAP(poGetSlaveInfo->bIsPresent);
        poGetSlaveInfo->wNumProcessVarsInp = EC_WORDSWAP(poGetSlaveInfo->wNumProcessVarsInp);
        poGetSlaveInfo->wNumProcessVarsOutp = EC_WORDSWAP(poGetSlaveInfo->wNumProcessVarsOutp);
        poGetSlaveInfo->dwSlaveId = EC_DWORDSWAP(poGetSlaveInfo->dwSlaveId);
        poGetSlaveInfo->bIsHCGroupPresent = EC_DWORDSWAP(poGetSlaveInfo->bIsHCGroupPresent);
        poGetSlaveInfo->aPortSlaveIds[0] = EC_DWORDSWAP(poGetSlaveInfo->aPortSlaveIds[0]);
        poGetSlaveInfo->aPortSlaveIds[1] = EC_DWORDSWAP(poGetSlaveInfo->aPortSlaveIds[1]);
        poGetSlaveInfo->aPortSlaveIds[2] = EC_DWORDSWAP(poGetSlaveInfo->aPortSlaveIds[2]);
        poGetSlaveInfo->aPortSlaveIds[3] = EC_DWORDSWAP(poGetSlaveInfo->aPortSlaveIds[3]);
        poGetSlaveInfo->dwSystemTimeDifference = EC_DWORDSWAP(poGetSlaveInfo->dwSystemTimeDifference);
#endif /* EC_BIG_ENDIAN */
    }

    ATEMRAS_EMGETSLAVEINFOTRANSFER_SET_RESULT(pECResSlvCtlTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetCfgSlaveInfo

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetCfgSlaveInfo(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                              dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                              dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMGETCFGSLAVEINFOTRANSFER    pECResSlvCtlTransf  = EC_NULL;
    EC_T_DWORD                              dwMasterInstance    = 0;
    EC_T_BOOL                               bStationAddressing  = EC_FALSE;
    EC_T_WORD                               wSlaveAddress       = 1;
    EC_T_CFG_SLAVE_INFO*                    poGetCfgInfo        = EC_NULL;
        
    pECResSlvCtlTransf  = (ATEMRAS_PT_EMGETCFGSLAVEINFOTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance    = ATEMRAS_EMGETCFGSLAVEINFOTRANSFER_GET_INSTANCEID(pECResSlvCtlTransf);
    bStationAddressing  = ATEMRAS_EMGETCFGSLAVEINFOTRANSFER_GET_STATIONADDRESS(pECResSlvCtlTransf);
    wSlaveAddress       = ATEMRAS_EMGETCFGSLAVEINFOTRANSFER_GET_SLVADDR(pECResSlvCtlTransf);
    poGetCfgInfo        = ATEMRAS_EMGETCFGSLAVEINFOTRANSFER_GET_GETCFGSLAVEINFO_POINTER(pECResSlvCtlTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetCfgSlaveInfo);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emGetCfgSlaveInfo(dwMasterInstance, bStationAddressing, wSlaveAddress, poGetCfgInfo);

#if (defined EC_BIG_ENDIAN)
        poGetCfgInfo->dwSlaveId = EC_DWORDSWAP(poGetCfgInfo->dwSlaveId);
        poGetCfgInfo->dwHCGroupIdx = EC_DWORDSWAP(poGetCfgInfo->dwHCGroupIdx);
        poGetCfgInfo->bIsPresent = EC_DWORDSWAP(poGetCfgInfo->bIsPresent);
        poGetCfgInfo->bIsHCGroupPresent = EC_DWORDSWAP(poGetCfgInfo->bIsHCGroupPresent);
        poGetCfgInfo->dwVendorId = EC_DWORDSWAP(poGetCfgInfo->dwVendorId);
        poGetCfgInfo->dwProductCode = EC_DWORDSWAP(poGetCfgInfo->dwProductCode);
        poGetCfgInfo->dwRevisionNumber = EC_DWORDSWAP(poGetCfgInfo->dwRevisionNumber);
        poGetCfgInfo->dwSerialNumber = EC_DWORDSWAP(poGetCfgInfo->dwSerialNumber);
        poGetCfgInfo->wStationAddress = EC_WORDSWAP(poGetCfgInfo->wStationAddress);
        poGetCfgInfo->wAutoIncAddress = EC_WORDSWAP(poGetCfgInfo->wAutoIncAddress);
        poGetCfgInfo->dwPdOffsIn = EC_DWORDSWAP(poGetCfgInfo->dwPdOffsIn);
        poGetCfgInfo->dwPdSizeIn = EC_DWORDSWAP(poGetCfgInfo->dwPdSizeIn);
        poGetCfgInfo->dwPdOffsOut = EC_DWORDSWAP(poGetCfgInfo->dwPdOffsOut);
        poGetCfgInfo->dwPdSizeOut = EC_DWORDSWAP(poGetCfgInfo->dwPdSizeOut);
        poGetCfgInfo->dwPdOffsIn2 = EC_DWORDSWAP(poGetCfgInfo->dwPdOffsIn2);
        poGetCfgInfo->dwPdSizeIn2 = EC_DWORDSWAP(poGetCfgInfo->dwPdSizeIn2);
        poGetCfgInfo->dwPdOffsOut2 = EC_DWORDSWAP(poGetCfgInfo->dwPdOffsOut2);
        poGetCfgInfo->dwPdSizeOut2 = EC_DWORDSWAP(poGetCfgInfo->dwPdSizeOut2);
        poGetCfgInfo->dwPdOffsIn3 = EC_DWORDSWAP(poGetCfgInfo->dwPdOffsIn3);
        poGetCfgInfo->dwPdSizeIn3 = EC_DWORDSWAP(poGetCfgInfo->dwPdSizeIn3);
        poGetCfgInfo->dwPdOffsOut3 = EC_DWORDSWAP(poGetCfgInfo->dwPdOffsOut3);
        poGetCfgInfo->dwPdOffsOut3 = EC_DWORDSWAP(poGetCfgInfo->dwPdOffsOut3);
        poGetCfgInfo->dwPdSizeOut3 = EC_DWORDSWAP(poGetCfgInfo->dwPdSizeOut3);
        poGetCfgInfo->dwPdOffsIn4 = EC_DWORDSWAP(poGetCfgInfo->dwPdOffsIn4);
        poGetCfgInfo->dwPdSizeIn4 = EC_DWORDSWAP(poGetCfgInfo->dwPdSizeIn4);
        poGetCfgInfo->dwPdOffsOut4 = EC_DWORDSWAP(poGetCfgInfo->dwPdOffsOut4);
        poGetCfgInfo->dwPdSizeOut4 = EC_DWORDSWAP(poGetCfgInfo->dwPdSizeOut4);
        poGetCfgInfo->dwMbxSupportedProtocols = EC_DWORDSWAP(poGetCfgInfo->dwMbxSupportedProtocols);
        poGetCfgInfo->dwMbxOutSize = EC_DWORDSWAP(poGetCfgInfo->dwMbxOutSize);
        poGetCfgInfo->dwMbxInSize = EC_DWORDSWAP(poGetCfgInfo->dwMbxInSize);
        poGetCfgInfo->dwMbxInSize = EC_DWORDSWAP(poGetCfgInfo->dwMbxInSize);
        poGetCfgInfo->dwMbxOutSize2 = EC_DWORDSWAP(poGetCfgInfo->dwMbxOutSize2);
        poGetCfgInfo->dwMbxInSize2 = EC_DWORDSWAP(poGetCfgInfo->dwMbxInSize2);
        poGetCfgInfo->bDcSupport = EC_DWORDSWAP(poGetCfgInfo->bDcSupport);
        poGetCfgInfo->wNumProcessVarsInp = EC_WORDSWAP(poGetCfgInfo->wNumProcessVarsInp);
        poGetCfgInfo->wNumProcessVarsOutp = EC_WORDSWAP(poGetCfgInfo->wNumProcessVarsOutp);
        poGetCfgInfo->wPrevStationAddress = EC_WORDSWAP(poGetCfgInfo->wPrevStationAddress);
        poGetCfgInfo->wPrevPort = EC_WORDSWAP(poGetCfgInfo->wPrevPort);
        poGetCfgInfo->wIdentifyAdo = EC_WORDSWAP(poGetCfgInfo->wIdentifyAdo);
        poGetCfgInfo->wIdentifyData = EC_WORDSWAP(poGetCfgInfo->wIdentifyData);
#endif /* EC_BIG_ENDIAN */
    }
    
    ATEMRAS_EMGETCFGSLAVEINFOTRANSFER_SET_RESULT(pECResSlvCtlTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetBusSlaveInfo

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetBusSlaveInfo(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                              dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                              dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMGETBUSSLAVEINFOTRANSFER    pECResSlvCtlTransf  = EC_NULL;
    EC_T_DWORD                              dwMasterInstance    = 0;
    EC_T_BOOL                               bStationAddressing  = EC_FALSE;
    EC_T_WORD                               wSlaveAddress       = 1;
    EC_T_BUS_SLAVE_INFO*                    poGetBusInfo        = EC_NULL;
#if (defined EC_BIG_ENDIAN)
    EC_T_DWORD                              dwPos               = 0;
#endif /* EC_BIG_ENDIAN */
        
    pECResSlvCtlTransf  = (ATEMRAS_PT_EMGETBUSSLAVEINFOTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance    = ATEMRAS_EMGETBUSSLAVEINFOTRANSFER_GET_INSTANCEID(pECResSlvCtlTransf);
    bStationAddressing  = ATEMRAS_EMGETBUSSLAVEINFOTRANSFER_GET_STATIONADDRESS(pECResSlvCtlTransf);
    wSlaveAddress       = ATEMRAS_EMGETBUSSLAVEINFOTRANSFER_GET_SLVADDR(pECResSlvCtlTransf);
    poGetBusInfo        = ATEMRAS_EMGETBUSSLAVEINFOTRANSFER_GET_GETBUSSLAVEINFO_POINTER(pECResSlvCtlTransf);


#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetBusSlaveInfo);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emGetBusSlaveInfo(dwMasterInstance, bStationAddressing, wSlaveAddress, poGetBusInfo);

#if (defined EC_BIG_ENDIAN)
        poGetBusInfo->dwSlaveId = EC_DWORDSWAP(poGetBusInfo->dwSlaveId);

        for (dwPos = 0; dwPos < sizeof(poGetBusInfo->adwPortSlaveIds) / sizeof(EC_T_DWORD); dwPos++)
        {
            poGetBusInfo->adwPortSlaveIds[dwPos] = EC_DWORDSWAP(poGetBusInfo->adwPortSlaveIds[dwPos]);
        }

        poGetBusInfo->wPortState = EC_WORDSWAP(poGetBusInfo->wPortState);
        poGetBusInfo->wAutoIncAddress = EC_WORDSWAP(poGetBusInfo->wAutoIncAddress);
        poGetBusInfo->bDcSupport = EC_DWORDSWAP(poGetBusInfo->bDcSupport);
        poGetBusInfo->bDc64Support = EC_DWORDSWAP(poGetBusInfo->bDc64Support);
        poGetBusInfo->dwVendorId = EC_DWORDSWAP(poGetBusInfo->dwVendorId);
        poGetBusInfo->dwProductCode = EC_DWORDSWAP(poGetBusInfo->dwProductCode);
        poGetBusInfo->dwRevisionNumber = EC_DWORDSWAP(poGetBusInfo->dwRevisionNumber);
        poGetBusInfo->dwSerialNumber = EC_DWORDSWAP(poGetBusInfo->dwSerialNumber);
        poGetBusInfo->wESCBuild = EC_WORDSWAP(poGetBusInfo->wESCBuild);
        poGetBusInfo->wFeaturesSupported = EC_WORDSWAP(poGetBusInfo->wFeaturesSupported);
        poGetBusInfo->wStationAddress = EC_WORDSWAP(poGetBusInfo->wStationAddress);
        poGetBusInfo->wAliasAddress = EC_WORDSWAP(poGetBusInfo->wAliasAddress);
        poGetBusInfo->wAlStatus = EC_WORDSWAP(poGetBusInfo->wAlStatus);
        poGetBusInfo->wAlStatusCode = EC_WORDSWAP(poGetBusInfo->wAlStatusCode);
        poGetBusInfo->dwSystemTimeDifference = EC_DWORDSWAP(poGetBusInfo->dwSystemTimeDifference);
        poGetBusInfo->wMbxSupportedProtocols = EC_WORDSWAP(poGetBusInfo->wMbxSupportedProtocols);
        poGetBusInfo->wDlStatus = EC_WORDSWAP(poGetBusInfo->wDlStatus);
        poGetBusInfo->wPrevPort = EC_WORDSWAP(poGetBusInfo->wPrevPort);
        poGetBusInfo->wIdentifyData = EC_WORDSWAP(poGetBusInfo->wIdentifyData);
        poGetBusInfo->bLineCrossed = EC_DWORDSWAP(poGetBusInfo->bLineCrossed);

        for (dwPos = 0; dwPos < sizeof(poGetBusInfo->adwReserved) / sizeof(EC_T_DWORD); dwPos++)
        {
            poGetBusInfo->adwReserved[dwPos] = EC_DWORDSWAP(poGetBusInfo->adwReserved[dwPos]);
        }
#endif /* EC_BIG_ENDIAN */
    }
    
    ATEMRAS_EMGETBUSSLAVEINFOTRANSFER_SET_RESULT(pECResSlvCtlTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emIsSlavePresent

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalIsSlavePresent(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                              dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                              dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMISSLAVEPRESENTTRANSFER     pECResSlvIPTransf   = EC_NULL;
    EC_T_DWORD                              dwMasterInstance    = 0;
    EC_T_DWORD                              dwSlaveId           = 0;
    EC_T_BOOL                               bIsPresent          = EC_FALSE;
        
    pECResSlvIPTransf           = (ATEMRAS_PT_EMISSLAVEPRESENTTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_EMISSLAVEPRESENTTRANSFER_GET_INSTANCEID(pECResSlvIPTransf);
    dwSlaveId                   = ATEMRAS_EMISSLAVEPRESENTTRANSFER_GET_SLAVEID(pECResSlvIPTransf);
    bIsPresent                  = ATEMRAS_EMISSLAVEPRESENTTRANSFER_GET_PRESENT(pECResSlvIPTransf);
    
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emIsSlavePresent);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emIsSlavePresent(dwMasterInstance, dwSlaveId, &bIsPresent);
    }
    
    ATEMRAS_EMISSLAVEPRESENTTRANSFER_SET_PRESENT(pECResSlvIPTransf, bIsPresent);
    ATEMRAS_EMISSLAVEPRESENTTRANSFER_SET_RESULT(pECResSlvIPTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetFixedAddr

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetFixedAddr(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                              dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                              dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SLVFIXEDADDRTRANSFER         pECResSlvIPTransf   = EC_NULL;
    EC_T_DWORD                              dwMasterInstance    = 0;
    EC_T_DWORD                              dwSlaveId           = 0;
    EC_T_WCHAR                              wFixedAddr          = 0;
        
    pECResSlvIPTransf           = (ATEMRAS_PT_SLVFIXEDADDRTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_SLVFIXEDADDRTRANSFER_GET_INSTANCEID(pECResSlvIPTransf);
    dwSlaveId                   = ATEMRAS_SLVFIXEDADDRTRANSFER_GET_SLVID(pECResSlvIPTransf);
    wFixedAddr                  = ATEMRAS_SLVFIXEDADDRTRANSFER_GET_FIXEDADDR(pECResSlvIPTransf);
    

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetFixedAddr);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emGetSlaveFixedAddr(dwMasterInstance, dwSlaveId, &wFixedAddr);
    }
    
    ATEMRAS_SLVFIXEDADDRTRANSFER_SET_FIXEDADDR(pECResSlvIPTransf, wFixedAddr);
    ATEMRAS_SLVFIXEDADDRTRANSFER_SET_RESULTCODE(pECResSlvIPTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetSlaveInpVarInfoNumOf or emGetSlaveOutpVarInfoNumOf depending on bInput

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSlaveProcVarInfoNumOf(
    EC_T_BOOL               bInput /**< [in]   EC_TRUE: deMarshalGetSlaveInpVarInfoNumOf; EC_FALSE: deMarshalGetSlaveOutpVarInfoNumOf */,
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )
{
    EC_T_DWORD                              dwRetVal               = EC_E_ERROR;
    EC_T_DWORD                              dwRes                  = EC_E_ERROR;
    ATEMRAS_PT_EMGETSLAVEVARINFONUMOF       pECResSlvIPTransf      = EC_NULL;
    EC_T_DWORD                              dwMasterInstance       = 0;
    EC_T_BOOL                               bFixedAddress          = 0;
    EC_T_WORD                               wSlaveAddress          = 0;
    EC_T_WORD                               wSlaveProcVarInfoNumOf = 0;
        
    pECResSlvIPTransf           = (ATEMRAS_PT_EMGETSLAVEVARINFONUMOF)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_EMGETSLAVEVARINFONUMOF_GET_INSTANCEID(pECResSlvIPTransf);
    bFixedAddress               = ATEMRAS_EMGETSLAVEVARINFONUMOF_GET_FIXEDADDR( pECResSlvIPTransf);
    wSlaveAddress               = ATEMRAS_EMGETSLAVEVARINFONUMOF_GET_SLVADDR(pECResSlvIPTransf);
    wSlaveProcVarInfoNumOf      = ATEMRAS_EMGETSLAVEVARINFONUMOF_GET_PROCESSVARSNUMOF(pECResSlvIPTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetSlaveProcVarInfoNumOf);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        if (bInput)
        {
            dwRes = emGetSlaveInpVarInfoNumOf(dwMasterInstance, bFixedAddress, wSlaveAddress, &wSlaveProcVarInfoNumOf);
        }
        else
        {
            dwRes = emGetSlaveOutpVarInfoNumOf(dwMasterInstance, bFixedAddress, wSlaveAddress, &wSlaveProcVarInfoNumOf);
        }
    }

    ATEMRAS_EMGETSLAVEVARINFONUMOF_SET_RESULT(pECResSlvIPTransf, dwRes);
    ATEMRAS_EMGETSLAVEVARINFONUMOF_SET_PROCESSVARSNUMOF(pECResSlvIPTransf, wSlaveProcVarInfoNumOf);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetSlaveInpVarInfoNumOf

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSlaveInpVarInfoNumOf(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )    
{
    return deMarshalGetSlaveProcVarInfoNumOf(EC_TRUE, pTransferHead, dwWrtLen);
}

/***************************************************************************************************/
/**
\brief  Perform emGetSlaveProcVarInfoNumOf

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSlaveOutpVarInfoNumOf(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )    
{
    return deMarshalGetSlaveProcVarInfoNumOf(EC_FALSE, pTransferHead, dwWrtLen);
}

/***************************************************************************************************/
/**
\brief  Perform emGetSlaveOutpVarInfo or emGetSlaveInpVarInfo depending on bInput

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSlaveProcVarInfo(
    EC_T_BOOL               bInput, 
    EC_T_BOOL               bEntryEx,
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )    
{
    EC_T_DWORD                              dwRetVal               = EC_E_ERROR;
    EC_T_DWORD                              dwRes                  = EC_E_ERROR;
    ATEMRAS_PT_EMGETSLAVEPROCVARINFOTRANSFER pECResSlvIPTransf      = EC_NULL;
    EC_T_DWORD                              dwMasterInstance       = 0;
    EC_T_BOOL                               bFixedAddress          = 0;
    EC_T_WORD                               wSlaveAddress          = 0;
    EC_T_WORD                               wNumVarsToRead         = 0;
    EC_T_WORD                               wReadEntries           = 0;
    EC_T_PROCESS_VAR_INFO*                  pProcessVarInfo        = EC_NULL;
    EC_T_PROCESS_VAR_INFO_EX*               pProcessVarInfoEx      = EC_NULL;

    pECResSlvIPTransf           = (ATEMRAS_PT_EMGETSLAVEPROCVARINFOTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_EMGETSLAVEPROCVARINFOTRANSFER_GET_INSTANCEID(pECResSlvIPTransf);
    bFixedAddress               = ATEMRAS_EMGETSLAVEPROCVARINFOTRANSFER_GET_FIXEDADDR( pECResSlvIPTransf);
    wSlaveAddress               = ATEMRAS_EMGETSLAVEPROCVARINFOTRANSFER_GET_SLVADDR(pECResSlvIPTransf);
    wNumVarsToRead              = ATEMRAS_EMGETSLAVEPROCVARINFOTRANSFER_GET_NUMVARSTOREAD(pECResSlvIPTransf);
    wReadEntries                = ATEMRAS_EMGETSLAVEPROCVARINFOTRANSFER_GET_READENTRIES(pECResSlvIPTransf);
    pProcessVarInfo             = ATEMRAS_EMGETSLAVEPROCVARINFOTRANSFER_GET_PROCVARINFO_POINTER_STD(pECResSlvIPTransf);
    pProcessVarInfoEx           = ATEMRAS_EMGETSLAVEPROCVARINFOTRANSFER_GET_PROCVARINFO_POINTER_EX(pECResSlvIPTransf);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetSlaveProcVarInfo);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        if (bEntryEx)
        {
            if (bInput)
                dwRes = emGetSlaveInpVarInfoEx(dwMasterInstance, bFixedAddress, wSlaveAddress, wNumVarsToRead, pProcessVarInfoEx, &wReadEntries);
            else
                dwRes = emGetSlaveOutpVarInfoEx(dwMasterInstance, bFixedAddress, wSlaveAddress, wNumVarsToRead, pProcessVarInfoEx, &wReadEntries);
        }
        else
        {
            if (bInput)
                dwRes = emGetSlaveInpVarInfo(dwMasterInstance, bFixedAddress, wSlaveAddress, wNumVarsToRead, pProcessVarInfo, &wReadEntries);
            else
                dwRes = emGetSlaveOutpVarInfo(dwMasterInstance, bFixedAddress, wSlaveAddress, wNumVarsToRead, pProcessVarInfo, &wReadEntries);
        }
        
#if (defined EC_BIG_ENDIAN)
        for (EC_T_INT i = 0; i < wReadEntries; ++i)
        {
            if (bEntryEx)
            {
                pProcessVarInfoEx[i].wDataType = EC_WORDSWAP(pProcessVarInfoEx[i].wDataType);
                pProcessVarInfoEx[i].wFixedAddr = EC_WORDSWAP(pProcessVarInfoEx[i].wFixedAddr);
                pProcessVarInfoEx[i].nBitSize = EC_DWORDSWAP(pProcessVarInfoEx[i].nBitSize);
                pProcessVarInfoEx[i].nBitOffs = EC_DWORDSWAP(pProcessVarInfoEx[i].nBitOffs);
                pProcessVarInfoEx[i].bIsInputData = EC_DWORDSWAP(pProcessVarInfoEx[i].bIsInputData);
                pProcessVarInfoEx[i].wIndex = EC_WORDSWAP(pProcessVarInfoEx[i].wIndex);
                pProcessVarInfoEx[i].wSubIndex = EC_WORDSWAP(pProcessVarInfoEx[i].wSubIndex);
                pProcessVarInfoEx[i].wPdoIndex = EC_WORDSWAP(pProcessVarInfoEx[i].wPdoIndex);
            }
            else
            {
                pProcessVarInfo[i].wDataType = EC_WORDSWAP(pProcessVarInfo[i].wDataType);
                pProcessVarInfo[i].wFixedAddr = EC_WORDSWAP(pProcessVarInfo[i].wFixedAddr);
                pProcessVarInfo[i].nBitSize = EC_DWORDSWAP(pProcessVarInfo[i].nBitSize);
                pProcessVarInfo[i].nBitOffs = EC_DWORDSWAP(pProcessVarInfo[i].nBitOffs);
                pProcessVarInfo[i].bIsInputData = EC_DWORDSWAP(pProcessVarInfo[i].bIsInputData);
            }
        }
#endif /* EC_BIG_ENDIAN */
    }

    ATEMRAS_EMGETSLAVEPROCVARINFOTRANSFER_SET_RESULT(pECResSlvIPTransf, dwRes);
    ATEMRAS_EMGETSLAVEPROCVARINFOTRANSFER_SET_READENTRIES(pECResSlvIPTransf, wReadEntries);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetSlaveInpVarInfo

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSlaveInpVarInfo(
    EC_T_BOOL               bEntryEx,
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )    
{
    return deMarshalGetSlaveProcVarInfo(EC_TRUE, bEntryEx, pTransferHead, dwWrtLen);
}

/***************************************************************************************************/
/**
\brief  Perform emGetSlaveOutpVarInfo

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSlaveOutpVarInfo( 
    EC_T_BOOL               bEntryEx,
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )    
{
    return deMarshalGetSlaveProcVarInfo(EC_FALSE, bEntryEx, pTransferHead, dwWrtLen);
}

/***************************************************************************************************/
/**
\brief  Perform emFindOutpVarByName or emFindInpVarByName

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalFindProcVarByName(
    EC_T_BOOL               bInput, 
    EC_T_BOOL               bEntryEx,
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )    
{
    EC_T_DWORD                              dwRetVal               = EC_E_ERROR;
    EC_T_DWORD                              dwRes                  = EC_E_ERROR;
    ATEMRAS_T_FINDPROCVARBYNAMETRANSFER_STD*    pECResSlvIPTransf      = EC_NULL;
    EC_T_DWORD                              dwMasterInstance       = 0;
    EC_T_CHAR*                              szVariablename         = EC_NULL;
    EC_T_PROCESS_VAR_INFO*                  pProcessVarInfo        = EC_NULL;
    EC_T_PROCESS_VAR_INFO_EX*               pProcessVarInfoEx      = EC_NULL;
    
    pECResSlvIPTransf           = (ATEMRAS_T_FINDPROCVARBYNAMETRANSFER_STD*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_FINDPROCVARBYNAMETRANSFER_GET_INSTANCEID(pECResSlvIPTransf);
    szVariablename              = ATEMRAS_FINDPROCVARBYNAMETRANSFER_GET_VARNAME_POINTER(pECResSlvIPTransf);
    pProcessVarInfo             = ATEMRAS_FINDPROCVARBYNAMETRANSFER_GET_PROCVARINFO_POINTER_STD(pECResSlvIPTransf);
    pProcessVarInfoEx           = ATEMRAS_FINDPROCVARBYNAMETRANSFER_GET_PROCVARINFO_POINTER_EX(pECResSlvIPTransf);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emFindProcVarByName);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        if (bEntryEx)
        {
            if (bInput)
                dwRes = emFindInpVarByNameEx(dwMasterInstance, szVariablename, pProcessVarInfoEx);
            else
                dwRes = emFindOutpVarByNameEx(dwMasterInstance, szVariablename, pProcessVarInfoEx);
        }
        else
        {
            if (bInput)
                dwRes = emFindInpVarByName(dwMasterInstance, szVariablename, pProcessVarInfo);
            else
                dwRes = emFindOutpVarByName(dwMasterInstance, szVariablename, pProcessVarInfo);
        }

#if (defined EC_BIG_ENDIAN)
        if (bEntryEx)
        {
            pProcessVarInfoEx->wDataType    = EC_WORDSWAP(pProcessVarInfoEx->wDataType);
            pProcessVarInfoEx->wFixedAddr   = EC_WORDSWAP(pProcessVarInfoEx->wFixedAddr);
            pProcessVarInfoEx->nBitSize     = EC_DWORDSWAP(pProcessVarInfoEx->nBitSize);
            pProcessVarInfoEx->nBitOffs     = EC_DWORDSWAP(pProcessVarInfoEx->nBitOffs);
            pProcessVarInfoEx->bIsInputData = EC_DWORDSWAP(pProcessVarInfoEx->bIsInputData);
            pProcessVarInfoEx->wIndex       = EC_WORDSWAP(pProcessVarInfoEx->wIndex);
            pProcessVarInfoEx->wSubIndex    = EC_WORDSWAP(pProcessVarInfoEx->wSubIndex);
            pProcessVarInfoEx->wPdoIndex    = EC_WORDSWAP(pProcessVarInfoEx->wPdoIndex);
        }
        else
        {
            pProcessVarInfo->wDataType      = EC_WORDSWAP(pProcessVarInfo->wDataType);
            pProcessVarInfo->wFixedAddr     = EC_WORDSWAP(pProcessVarInfo->wFixedAddr);
            pProcessVarInfo->nBitSize       = EC_DWORDSWAP(pProcessVarInfo->nBitSize);
            pProcessVarInfo->nBitOffs       = EC_DWORDSWAP(pProcessVarInfo->nBitOffs);
            pProcessVarInfo->bIsInputData   = EC_DWORDSWAP(pProcessVarInfo->bIsInputData);
        }
#endif /* EC_BIG_ENDIAN */
    }
    
    ATEMRAS_FINDPROCVARBYNAMETRANSFER_SET_RESULT(pECResSlvIPTransf, dwRes);
    if (bEntryEx)
        OsMemcpy(ATEMRAS_FINDPROCVARBYNAMETRANSFER_GET_PROCVARINFO_POINTER_EX(pECResSlvIPTransf), pProcessVarInfoEx, sizeof(EC_T_PROCESS_VAR_INFO_EX));
    else
        OsMemcpy(ATEMRAS_FINDPROCVARBYNAMETRANSFER_GET_PROCVARINFO_POINTER_STD(pECResSlvIPTransf), pProcessVarInfo, sizeof(EC_T_PROCESS_VAR_INFO));
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emFindInpVarByName

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalFindInpVarByName(
    EC_T_BOOL               bEntryEx,
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )    
{
    return deMarshalFindProcVarByName(EC_TRUE, bEntryEx, pTransferHead, dwWrtLen);    
}

/***************************************************************************************************/
/**
\brief  Perform emFindOutpVarByName

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalFindOutpVarByName(
    EC_T_BOOL               bEntryEx,
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )    
{
    return deMarshalFindProcVarByName(EC_FALSE, bEntryEx, pTransferHead, dwWrtLen);        
}

/***************************************************************************************************/
/**
\brief  Perform emGetProcessDataBits

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetProcessDataBits(   
    ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen)    
{
    EC_T_DWORD dwRetVal         = EC_E_ERROR;
    EC_T_DWORD dwRes            = EC_E_ERROR;
    ATEMRAS_T_EMSETGETPROCESSDATABITSTRANSFER* pECResSlvIPTransf = EC_NULL;
    EC_T_DWORD dwMasterInstance = 0;
    EC_T_BOOL bOutputdata       = EC_FALSE;
    EC_T_DWORD dwBitOffsetPd    = 0;
    EC_T_BYTE* pbyDataDst       = EC_NULL;
    EC_T_DWORD dwBitLengthDst   = 0;
    EC_T_DWORD dwTimeout        = 0;
    
    pECResSlvIPTransf           = (ATEMRAS_T_EMSETGETPROCESSDATABITSTRANSFER*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_INSTANCEID(pECResSlvIPTransf);
    bOutputdata                 = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_OUTPUTDATA(pECResSlvIPTransf);
    dwBitOffsetPd               = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_BITOFFSETPD(pECResSlvIPTransf);
    dwTimeout                   = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_TIMEOUT(pECResSlvIPTransf);
    dwBitLengthDst              = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_BITLENGTH(pECResSlvIPTransf);
    
    pbyDataDst                  = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_DATA(pECResSlvIPTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetProcessDataBits);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emGetProcessDataBits(dwMasterInstance, bOutputdata, dwBitOffsetPd, pbyDataDst, dwBitLengthDst, dwTimeout);
    }
    
    ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_SET_RESULT(pECResSlvIPTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emSetProcessDataBits

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalSetProcessDataBits(   
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )    
{
    EC_T_DWORD dwRetVal         = EC_E_ERROR;
    EC_T_DWORD dwRes            = EC_E_ERROR;
    ATEMRAS_T_EMSETGETPROCESSDATABITSTRANSFER* pECResSlvIPTransf = EC_NULL;
    EC_T_DWORD dwMasterInstance = 0;
    EC_T_BOOL bOutputdata       = EC_FALSE;
    EC_T_DWORD dwBitOffsetPd    = 0;
    EC_T_BYTE* pbyDataSrc       = EC_NULL;
    EC_T_DWORD dwBitLengthSrc   = 0;
    EC_T_DWORD dwTimeout        = 0;
    
    pECResSlvIPTransf           = (ATEMRAS_T_EMSETGETPROCESSDATABITSTRANSFER*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_INSTANCEID(pECResSlvIPTransf);
    bOutputdata                 = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_OUTPUTDATA(pECResSlvIPTransf);
    dwBitOffsetPd               = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_BITOFFSETPD(pECResSlvIPTransf);
    dwTimeout                   = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_TIMEOUT(pECResSlvIPTransf);
    dwBitLengthSrc              = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_BITLENGTH(pECResSlvIPTransf);
    pbyDataSrc                  = ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_GET_DATA(pECResSlvIPTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSetProcessDataBits);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emSetProcessDataBits(dwMasterInstance, bOutputdata, dwBitOffsetPd, pbyDataSrc, dwBitLengthSrc, dwTimeout);
    }
    
    ATEMRAS_EMSETGETPROCESSDATABITSTRANSFER_SET_RESULT(pECResSlvIPTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

#ifdef INCLUDE_FORCE_PROCESSDATA
/***************************************************************************************************/
/**
\brief  Perform emForceProcessDataBits

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalForceProcessDataBits(   
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )    
{
    EC_T_DWORD dwRetVal         = EC_E_ERROR;
    EC_T_DWORD dwRes            = EC_E_ERROR;
    ATEMRAS_T_EMFORCERELEASEPROCESSDATABITSTRANSFER* pECResSlvIPTransf = EC_NULL;
    EC_T_DWORD dwMasterInstance = 0;
    EC_T_BOOL bOutputdata       = EC_FALSE;
    EC_T_DWORD dwBitOffsetPd    = 0;
    EC_T_BYTE* pbyData          = EC_NULL;
    EC_T_WORD wBitLength        = 0;
    EC_T_DWORD dwClientId       = 0;
    EC_T_DWORD dwTimeout        = 0;
    
    pECResSlvIPTransf           = (ATEMRAS_T_EMFORCERELEASEPROCESSDATABITSTRANSFER*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_INSTANCEID(pECResSlvIPTransf);
    dwClientId                  = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_CLIENTID(pECResSlvIPTransf);
    bOutputdata                 = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_OUTPUT(pECResSlvIPTransf);
    dwBitOffsetPd               = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_BITOFFSET(pECResSlvIPTransf);
    wBitLength                  = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_BITLENGTH(pECResSlvIPTransf);
    dwTimeout                   = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_TIMEOUT(pECResSlvIPTransf);
    pbyData                     = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_PBYDATA(pECResSlvIPTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSetProcessDataBits);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emForceProcessDataBits(dwMasterInstance, dwClientId, bOutputdata, dwBitOffsetPd, wBitLength, pbyData, dwTimeout);
    }
    
    ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_SET_RESULT(pECResSlvIPTransf, dwRes);
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emReleaseProcessDataBits

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalReleaseProcessDataBits(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD dwRetVal         = EC_E_ERROR;
    EC_T_DWORD dwRes            = EC_E_ERROR;
    ATEMRAS_T_EMFORCERELEASEPROCESSDATABITSTRANSFER* pECResSlvIPTransf = EC_NULL;
    EC_T_DWORD dwMasterInstance = 0;
    EC_T_BOOL bOutputdata       = EC_FALSE;
    EC_T_DWORD dwBitOffsetPd    = 0;
    EC_T_WORD wBitLength        = 0;
    EC_T_DWORD dwClientId       = 0;
    EC_T_DWORD dwTimeout        = 0;
    
    pECResSlvIPTransf           = (ATEMRAS_T_EMFORCERELEASEPROCESSDATABITSTRANSFER*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_INSTANCEID(pECResSlvIPTransf);
    bOutputdata                 = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_OUTPUT(pECResSlvIPTransf);
    dwBitOffsetPd               = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_BITOFFSET(pECResSlvIPTransf);
    wBitLength                  = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_BITLENGTH(pECResSlvIPTransf);
    dwClientId                  = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_CLIENTID(pECResSlvIPTransf);
    dwTimeout                   = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_TIMEOUT(pECResSlvIPTransf);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSetProcessDataBits);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emReleaseProcessDataBits(dwMasterInstance, dwClientId, bOutputdata, dwBitOffsetPd, wBitLength, dwTimeout);
    }
    
    ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_SET_RESULT(pECResSlvIPTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emReleaseAllProcessDataBits

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalReleaseAllProcessDataBits(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD dwRetVal         = EC_E_ERROR;
    EC_T_DWORD dwRes            = EC_E_ERROR;
    ATEMRAS_T_EMFORCERELEASEPROCESSDATABITSTRANSFER* pECResSlvIPTransf = EC_NULL;
    EC_T_DWORD dwMasterInstance = 0;
    EC_T_DWORD dwClientId       = 0;
    EC_T_DWORD dwTimeout    = 0;
    
    pECResSlvIPTransf           = (ATEMRAS_T_EMFORCERELEASEPROCESSDATABITSTRANSFER*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_INSTANCEID(pECResSlvIPTransf);
    dwClientId                  = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_CLIENTID(pECResSlvIPTransf);
    dwTimeout                   = ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_GET_TIMEOUT(pECResSlvIPTransf);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSetProcessDataBits);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emReleaseAllProcessDataBits(dwMasterInstance, dwClientId, dwTimeout);
    }
    
    ATEMRAS_EMFORCERELEASEPROCESSDATABITSTRANSFER_SET_RESULT(pECResSlvIPTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}
#endif

/***************************************************************************************************/
/**
\brief  Perform emReloadSlaveEEPRom

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalReloadSlaveEEPRom(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )    
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SLAVEEEPROMOP        pECTransf           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
                                            
    pECTransf                   = (ATEMRAS_PT_SLAVEEEPROMOP)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_SLAVEEEPROMOP_GET_INSTANCEID(pECTransf);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emReloadSlaveEEPRom);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emReloadSlaveEEPRom(dwMasterInstance,
            ATEMRAS_SLAVEEEPROMOP_GET_FIXEDADDRESSING(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_SLAVEADDRESS(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_TIMEOUT(pECTransf)
            );
    }
    
    ATEMRAS_SLAVEEEPROMOP_SET_SRVRESULT(pECTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emReadSlaveEEPRom

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalReadSlaveEEPRom(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )    
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SLAVEEEPROMOP        pECTransf           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_DWORD dwNumOutData                             = 0;
    
    pECTransf                   = (ATEMRAS_PT_SLAVEEEPROMOP)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_SLAVEEEPROMOP_GET_INSTANCEID(pECTransf);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emReadSlaveEEPRom);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emReadSlaveEEPRom(dwMasterInstance,
            ATEMRAS_SLAVEEEPROMOP_GET_FIXEDADDRESSING(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_SLAVEADDRESS(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_EEPROMSTARTOFFSET(pECTransf),
            (EC_T_WORD*)ATEMRAS_SLAVEEEPROMOP_DATA(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_READWRITELEN(pECTransf),
            &dwNumOutData,
            ATEMRAS_SLAVEEEPROMOP_GET_TIMEOUT(pECTransf)
            );
    }
    
    ATEMRAS_SLAVEEEPROMOP_SET_SRVRESULT(pECTransf, dwRes);
    ATEMRAS_SLAVEEEPROMOP_SET_NUMOUTDATA(pECTransf, dwNumOutData);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emWriteSlaveEEPRom

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalWriteSlaveEEPRom(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SLAVEEEPROMOP        pECTransf           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
                                            
    pECTransf                   = (ATEMRAS_PT_SLAVEEEPROMOP)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_SLAVEEEPROMOP_GET_INSTANCEID(pECTransf);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emWriteSlaveEEPRom);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emWriteSlaveEEPRom(dwMasterInstance,
            ATEMRAS_SLAVEEEPROMOP_GET_FIXEDADDRESSING(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_SLAVEADDRESS(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_EEPROMSTARTOFFSET(pECTransf),
            (EC_T_WORD*)ATEMRAS_SLAVEEEPROMOP_DATA(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_READWRITELEN(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_TIMEOUT(pECTransf)
            );
    }
    
    ATEMRAS_SLAVEEEPROMOP_SET_SRVRESULT(pECTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emAssignSlaveEEPRom

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalAssignSlaveEEPRom(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SLAVEEEPROMOP        pECTransf           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
                                            
    pECTransf                   = (ATEMRAS_PT_SLAVEEEPROMOP)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_SLAVEEEPROMOP_GET_INSTANCEID(pECTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emAssignSlaveEEPRom);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emAssignSlaveEEPRom(dwMasterInstance,
            ATEMRAS_SLAVEEEPROMOP_GET_FIXEDADDRESSING(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_SLAVEADDRESS(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_SLAVEPDIACCESS(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_FORCEASSIGN(pECTransf),
            ATEMRAS_SLAVEEEPROMOP_GET_TIMEOUT(pECTransf)
            );
    }
    
    ATEMRAS_SLAVEEEPROMOP_SET_SRVRESULT(pECTransf, dwRes);

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emSoeRead

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalSoeRead(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SOE_OP               pECTransf           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_BYTE                       byElementFlags      = 0;
    EC_T_DWORD                      dwNumOutData        = 0;
                                              
    pECTransf                   = (ATEMRAS_PT_SOE_OP)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_SOE_OP_GET_INSTANCEID(pECTransf);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSoeRead);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        byElementFlags = ATEMRAS_SOE_OP_GET_ELEMENTFLAGS(pECTransf);

        dwRes = emSoeRead(dwMasterInstance,
            ATEMRAS_SOE_OP_GET_SLAVEID(pECTransf),
            ATEMRAS_SOE_OP_GET_DRIVENO(pECTransf),
            &byElementFlags,
            ATEMRAS_SOE_OP_GET_IDN(pECTransf),
            ATEMRAS_SOE_OP_DATA(pECTransf),
            ATEMRAS_SOE_OP_GET_READWRITEDATALEN(pECTransf),
            &dwNumOutData,
            ATEMRAS_SOE_OP_GET_TIMEOUT(pECTransf)
            );
    }
    
    ATEMRAS_SOE_OP_SET_SRVRESULT(pECTransf, dwRes);
    ATEMRAS_SOE_OP_SET_ELEMENTFLAGS(pECTransf, byElementFlags);
    ATEMRAS_SOE_OP_SET_NUMOUTDATA(pECTransf, dwNumOutData);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emSoeWrite

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalSoeWrite(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SOE_OP               pECTransf           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_BYTE                       byElementFlags      = 0;
                                            
    pECTransf                   = (ATEMRAS_PT_SOE_OP)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_SOE_OP_GET_INSTANCEID(pECTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSoeWrite);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        byElementFlags = ATEMRAS_SOE_OP_GET_ELEMENTFLAGS(pECTransf);

        dwRes = emSoeWrite(dwMasterInstance,
            ATEMRAS_SOE_OP_GET_SLAVEID(pECTransf),
            ATEMRAS_SOE_OP_GET_DRIVENO(pECTransf),
            &byElementFlags,
            ATEMRAS_SOE_OP_GET_IDN(pECTransf),
            ATEMRAS_SOE_OP_DATA(pECTransf),
            ATEMRAS_SOE_OP_GET_READWRITEDATALEN(pECTransf),
            ATEMRAS_SOE_OP_GET_TIMEOUT(pECTransf)
            );
    }
    
    ATEMRAS_SOE_OP_SET_SRVRESULT(pECTransf, dwRes);
    ATEMRAS_SOE_OP_SET_ELEMENTFLAGS(pECTransf, byElementFlags);

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emSoeAbortProcCmd

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalSoeAbortProcCmd(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SOE_OP               pECTransf           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_BYTE                       byElementFlags      = 0;
                                            
    pECTransf                   = (ATEMRAS_PT_SOE_OP)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance            = ATEMRAS_SOE_OP_GET_INSTANCEID(pECTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSoeAbortProcCmd);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        byElementFlags = ATEMRAS_SOE_OP_GET_ELEMENTFLAGS(pECTransf);
    
        dwRes = emSoeAbortProcCmd(dwMasterInstance,
            ATEMRAS_SOE_OP_GET_SLAVEID(pECTransf),
            ATEMRAS_SOE_OP_GET_DRIVENO(pECTransf),
            &byElementFlags,
            ATEMRAS_SOE_OP_GET_IDN(pECTransf),
            ATEMRAS_SOE_OP_GET_TIMEOUT(pECTransf)
            );
    }
    
    ATEMRAS_SOE_OP_SET_SRVRESULT(pECTransf, dwRes);
    ATEMRAS_SOE_OP_SET_ELEMENTFLAGS(pECTransf, byElementFlags);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Get Master Version

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetVersion(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD dwVersion;
    ecatGetVersion(&dwVersion);

    ATEMRAS_PT_EMGETVERSIONTRANSFER pTransfer = (ATEMRAS_PT_EMGETVERSIONTRANSFER)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    ATEMRAS_EMGETVERSIONTRANSFER_SET_VERSION(pTransfer, dwVersion);

    return SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
}

/***************************************************************************************************/
/**
\brief  Get Src Mac Address

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetSrcMacAddress(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )  
{
    ATEMRAS_PT_EMGETSRCMACADDRESS   pTransfer           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    ETHERNET_ADDRESS                oSrcMacAddress;

    pTransfer                   = (ATEMRAS_PT_EMGETSRCMACADDRESS)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_EMGETSRCMACADDRESS_GET_INSTANCEID(pTransfer);

    emGetSrcMacAddress(dwMasterInstance, &oSrcMacAddress);

    OsMemcpy((EC_T_BYTE*)pTransfer + ATEMRAS_EMGETSRCMACADDRESS_OFFS_ADDRESS, &oSrcMacAddress, ETHERNET_ADDRESS_LEN);

    return SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
}

/***************************************************************************************************/
/**
\brief EoE register endpoint

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalEoERegisterEndpoint(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )  
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMEOEREGISTEREP      pTransfer           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_CHAR*                      szEoEDrvIdent = EC_NULL;
    EC_T_LINK_DRV_DESC*             pDesc = EC_NULL;
    EC_T_DWORD                      dwHandle = (EC_T_DWORD)-1;
    CAtEmMasterInstance*            pMasterInstance = EC_NULL;

    pTransfer                   = (ATEMRAS_PT_EMEOEREGISTEREP)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_EMEOEREGISTEREP_GET_INSTANCEID(pTransfer);
    szEoEDrvIdent               = ATEMRAS_EMEOEREGISTEREP_EOEDRVIDENT(pTransfer);

    pDesc = EC_NEW(EC_T_LINK_DRV_DESC);
    if (EC_NULL == pDesc) return EC_E_NOMEMORY;

    OsMemset(pDesc, 0, sizeof(*pDesc));

    pDesc->dwInterfaceVersion = ATEMRAS_EMEOEREGISTEREP_GET_INTERFACEVERSION(pTransfer);
    pDesc->dwValidationPattern = ATEMRAS_EMEOEREGISTEREP_GET_VALIDATIONPATTERN(pTransfer);

    /* Register Endpoint */
    dwRes = emEoeRegisterEndpoint(dwMasterInstance, szEoEDrvIdent, pDesc);

    ATEMRAS_EMEOEREGISTEREP_SET_RESULT(pTransfer, dwRes);
    
    if (EC_E_NOERROR == dwRes)
    {
        /* Some houskeeping ... */

        pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
        if (EC_NULL == pMasterInstance)
        {
            dwRetVal = EC_E_INVALIDINDEX;
            goto Exit;
        }

        dwRes = pMasterInstance->AddLinkDrvDtor(pDesc, &dwHandle);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }

        ATEMRAS_EMEOEREGISTEREP_SET_SERVERHANDLE(pTransfer, dwHandle);
    }

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief EoE unregister endpoint

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalEoEUnregisterEndpoint(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )  
{
    /* This function supports EoE only. TODO */

    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMLINKFRAME          pTransfer           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_DWORD                      dwDriverHandle = (EC_T_DWORD)-1;
    CAtEmMasterInstance*            pMasterInstance = EC_NULL;
    CAtEmClientLinkDrv*             pLinkDrv = EC_NULL;

    pTransfer                   = (ATEMRAS_PT_EMLINKFRAME)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_EMLINKFRAME_GET_INSTANCEID(pTransfer);
    dwDriverHandle              = ATEMRAS_EMLINKFRAME_GET_DRIVERHANDLE(pTransfer);

    /* Lookup driver */
    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }

    pLinkDrv = pMasterInstance->LinkDrv(dwDriverHandle);
    if (EC_NULL == pLinkDrv)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* Close driver */
    dwRes = pLinkDrv->UnregisterDriver();
    
    ATEMRAS_EMLINKFRAME_SET_RESULT(pTransfer, dwRes);

    if (EC_E_NOERROR == dwRes)
    {
        /* Remove housekeeping */
        pMasterInstance->RemoveLinkDrv(pLinkDrv);
    }
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief Open Link Driver instance

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalLinkOpen(
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )  
{
    /* This function supports EoE only. TODO */

    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMLINKOPEN           pTransfer           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_DWORD                      dwDriverHandle = (EC_T_DWORD)-1;
    EC_T_VOID*                      pvInstanceHandle = EC_NULL;
    EC_T_DWORD                      dwInstanceHandle = (EC_T_DWORD)-1;
    CAtEmMasterInstance*            pMasterInstance = EC_NULL;
    CAtEmClientLinkDrv*             pLinkDrv = EC_NULL;
    EC_T_LINK_DRV_DESC*             pLinkDrvDtor = EC_NULL;
    EC_T_LINK_OPENPARMS_EOE         eoeOpenPrm; 
    
    pTransfer                   = (ATEMRAS_PT_EMLINKOPEN)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = EC_GET_FRM_DWORD(&pTransfer->dwInstanceID);
    dwDriverHandle              = EC_GET_FRM_DWORD(&pTransfer->LinkSpecificParms.EoE.dwDriverHandle);
    
    /* Lookup driver */
    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }
    
    pLinkDrv = pMasterInstance->LinkDrv(dwDriverHandle);
    if (EC_NULL == pLinkDrv)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    pLinkDrvDtor = pLinkDrv->LinkDrvDtor();
    
    OsMemset(&eoeOpenPrm, 0, sizeof(eoeOpenPrm));
    OsStrcpy(eoeOpenPrm.szEoEDrvIdent, "eoe");
    eoeOpenPrm.pvUplinkInstance = pLinkDrvDtor->pvLinkInstance;
    eoeOpenPrm.dwEmInstanceID = dwMasterInstance;
    
    /* Open Link Driver instance */
    
    dwRes = pLinkDrvDtor->pfEcLinkOpen(&eoeOpenPrm, EC_NULL, EC_NULL, EC_NULL, &pvInstanceHandle);
    EC_SET_FRM_DWORD(&pTransfer->dwResult, dwRes);
    if (EC_E_NOERROR == dwRes)
    {
        /* Some housekeeping. Store instance context. */
    
        dwRes = pLinkDrv->AddLinkDrvInstance(pvInstanceHandle, &dwInstanceHandle);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    
        EC_SET_FRM_DWORD(&pTransfer->dwInstanceHandle, dwInstanceHandle);
    }
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief Close Link Driver instance

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalLinkClose(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )  
{
    /* This function supports EoE only. TODO */

    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMLINKFRAME          pTransfer           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_DWORD                      dwDriverHandle      = (EC_T_DWORD)-1;
    EC_T_DWORD                      dwInstanceHandle    = (EC_T_DWORD)-1;
    CAtEmMasterInstance*            pMasterInstance     = EC_NULL;
    CAtEmClientLinkDrv*             pLinkDrv            = EC_NULL;
    CAtEmClientLinkDrvInstance*     pLinkDrvInstance    = EC_NULL;
    
    pTransfer                   = (ATEMRAS_PT_EMLINKFRAME)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_EMLINKFRAME_GET_INSTANCEID(pTransfer);
    dwDriverHandle              = ATEMRAS_EMLINKFRAME_GET_DRIVERHANDLE(pTransfer);
    dwInstanceHandle            = ATEMRAS_EMLINKFRAME_GET_INSTANCEHANDLE(pTransfer);
    
    /* Lookup driver */
    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }
    
    pLinkDrv = pMasterInstance->LinkDrv(dwDriverHandle);
    if (EC_NULL == pLinkDrv)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    pLinkDrvInstance = pLinkDrv->LinkDrvInstance(dwInstanceHandle);
    if (EC_NULL == pLinkDrvInstance)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    /* Close driver instance */
    dwRes = pLinkDrvInstance->Close();
    
    ATEMRAS_EMLINKFRAME_SET_RESULT(pTransfer, dwRes);
    
    if (EC_E_NOERROR == dwRes)
    {
        /* Remove housekeeping */
        pLinkDrv->RemoveLinkDrvInstance(pLinkDrvInstance);
    }
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief Link Driver SendAndFreeFrame

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalLinkSendAndFreeFrame(
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )  
{
    /* This function supports EoE only. TODO */

    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMLINKFRAME          pTransfer           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_DWORD                      dwDriverHandle      = (EC_T_DWORD)-1;
    EC_T_DWORD                      dwInstanceHandle    = (EC_T_DWORD)-1;
    CAtEmMasterInstance*            pMasterInstance     = EC_NULL;
    CAtEmClientLinkDrv*             pLinkDrv            = EC_NULL;
    CAtEmClientLinkDrvInstance*     pLinkDrvInstance    = EC_NULL;
    EC_T_LINK_DRV_DESC*             pLinkDrvDtor        = EC_NULL;
    EC_T_LINK_FRAMEDESC             frameDtor;
    EC_T_DWORD                      dwFrameLength   = 0;
    
    pTransfer                   = (ATEMRAS_PT_EMLINKFRAME)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_EMLINKFRAME_GET_INSTANCEID(pTransfer);
    dwDriverHandle              = ATEMRAS_EMLINKFRAME_GET_DRIVERHANDLE(pTransfer);
    dwInstanceHandle            = ATEMRAS_EMLINKFRAME_GET_INSTANCEHANDLE(pTransfer);
    
    /* Lookup driver */
    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }
    
    pLinkDrv = pMasterInstance->LinkDrv(dwDriverHandle);
    if (EC_NULL == pLinkDrv)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    pLinkDrvDtor = pLinkDrv->LinkDrvDtor();
    
    pLinkDrvInstance = pLinkDrv->LinkDrvInstance(dwInstanceHandle);
    if (EC_NULL == pLinkDrvInstance)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    /* Allocate frame. TODO avoid memcpy. Implement EOE SendFrame() */
    
    dwFrameLength = ATEMRAS_EMLINKFRAME_GET_FRAMELENGTH(pTransfer);
    OsMemset(&frameDtor, 0, sizeof(frameDtor));
    dwRes = pLinkDrvDtor->pfEcLinkAllocSendFrame(pLinkDrvInstance->InternalHandle(), &frameDtor, dwFrameLength);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    
    OsMemcpy(frameDtor.pbyFrame, ATEMRAS_EMLINKFRAME_FRAMEBUFFER(pTransfer), dwFrameLength);
    
    /* Send frame */
    
    dwRes = pLinkDrvDtor->pfEcLinkSendAndFreeFrame(pLinkDrvInstance->InternalHandle(), &frameDtor);
    ATEMRAS_EMLINKFRAME_SET_RESULT(pTransfer, dwRes);
    if (EC_E_NOERROR != dwRes)
    {
        pLinkDrvDtor->pfEcLinkFreeSendFrame(pLinkDrvInstance->InternalHandle(), &frameDtor);
    }
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief Link Driver RecvFrame

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalLinkRecvFrame(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )  
{
    /* This function supports EoE only. TODO */
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_EMLINKFRAME          pTransfer           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_DWORD                      dwDriverHandle      = (EC_T_DWORD)-1;
    EC_T_DWORD                      dwInstanceHandle    = (EC_T_DWORD)-1;
    CAtEmMasterInstance*            pMasterInstance     = EC_NULL;
    CAtEmClientLinkDrv*             pLinkDrv            = EC_NULL;
    CAtEmClientLinkDrvInstance*     pLinkDrvInstance    = EC_NULL;
    EC_T_LINK_DRV_DESC*             pLinkDrvDtor         = EC_NULL;
    EC_T_LINK_FRAMEDESC             frameDtor;
    
    pTransfer                   = (ATEMRAS_PT_EMLINKFRAME)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    dwMasterInstance            = ATEMRAS_EMLINKFRAME_GET_INSTANCEID(pTransfer);
    dwDriverHandle              = ATEMRAS_EMLINKFRAME_GET_DRIVERHANDLE(pTransfer);
    dwInstanceHandle            = ATEMRAS_EMLINKFRAME_GET_INSTANCEHANDLE(pTransfer);
    
    /* Lookup driver */
    pMasterInstance = m_pMasterStore->InstanceById(dwMasterInstance, EC_FALSE);
    if (EC_NULL == pMasterInstance)
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }
    
    pLinkDrv = pMasterInstance->LinkDrv(dwDriverHandle);
    if (EC_NULL == pLinkDrv)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    pLinkDrvDtor = pLinkDrv->LinkDrvDtor();
    
    pLinkDrvInstance = pLinkDrv->LinkDrvInstance(dwInstanceHandle);
    if (EC_NULL == pLinkDrvInstance)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    OsMemset(&frameDtor, 0, sizeof(frameDtor));
    
    /* Receive polling */
    dwRes = pLinkDrvDtor->pfEcLinkRecvFrame(pLinkDrvInstance->InternalHandle(), &frameDtor);
    
    ATEMRAS_EMLINKFRAME_SET_RESULT(pTransfer, dwRes);
    ATEMRAS_EMLINKFRAME_SET_FRAMELENGTH(pTransfer, frameDtor.dwSize);
    
    /* Fill RAS header and set payload length */
    emrasProtoCreateCmdResponseNoAlloc(pTransferHead, ATEMRAS_T_EMLINKFRAME_SIZE + frameDtor.dwSize);
    
    /* Send RAS header + payload header */
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Dealloc;
    }

    if (frameDtor.dwSize > 0)
    {
        /* Send raw frame as payload */
        dwRes = SendData(frameDtor.pbyFrame, frameDtor.dwSize);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Dealloc;
        }
    }

Dealloc:
    if (frameDtor.dwSize > 0)
    {
        pLinkDrvDtor->pfEcLinkFreeRecvFrame(pLinkDrvInstance->InternalHandle(), &frameDtor);
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Perform emReadSlaveIdentification

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalReadSlaveIdentification(
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SLAVEIDENTIFICATION  pECTransf           = EC_NULL;
    EC_T_DWORD                      dwMasterInstance    = 0;
    EC_T_WORD                       wValue              = 0;

    pECTransf = (ATEMRAS_PT_SLAVEIDENTIFICATION)ATEMRAS_PROTOHDR_DATA(pTransferHead);

    dwMasterInstance = ATEMRAS_SLAVEIDENTIFICATION_GET_INSTANCEID(pECTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emReadSlaveIdentification);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emReadSlaveIdentification(dwMasterInstance,
            ATEMRAS_SLAVEIDENTIFICATION_GET_FIXEDADDRESSING(pECTransf),
            ATEMRAS_SLAVEIDENTIFICATION_GET_SLAVEADDRESS(pECTransf),
            ATEMRAS_SLAVEIDENTIFICATION_GET_ADO(pECTransf),
            &wValue,
            ATEMRAS_SLAVEIDENTIFICATION_GET_TIMEOUT(pECTransf)
            );
    }
    
    ATEMRAS_SLAVEIDENTIFICATION_SET_VALUE(pECTransf, wValue);
    ATEMRAS_SLAVEIDENTIFICATION_SET_SRVRESULT(pECTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emSetSlaveDisabled

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalSetSlaveDisabled(
    ATEMRAS_T_PROTOHDR*         pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD                  dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                  dwRes               = EC_E_ERROR;
    ATEMRAS_PT_SETSLAVEDISABLED pECTransf           = EC_NULL;
    EC_T_DWORD                  dwMasterInstance    = 0;

    pECTransf = (ATEMRAS_PT_SETSLAVEDISABLED)ATEMRAS_PROTOHDR_DATA(pTransferHead);

    dwMasterInstance = ATEMRAS_SETSLAVEDISABLED_GET_INSTANCEID(pECTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSetSlaveDisabled);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emSetSlaveDisabled(dwMasterInstance,
            ATEMRAS_SETSLAVEDISABLED_GET_FIXEDADDRESSING(pECTransf),
            ATEMRAS_SETSLAVEDISABLED_GET_SLAVEADDRESS(pECTransf),
            ATEMRAS_SETSLAVEDISABLED_GET_DISABLED(pECTransf)
            );
    }


    ATEMRAS_SETSLAVEDISABLED_SET_SRVRESULT(pECTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emSetSlaveDisconnected

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalSetSlaveDisconnected(
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )
{
    EC_T_DWORD                  dwRetVal        = EC_E_ERROR;
    EC_T_DWORD                  dwRes           = EC_E_ERROR;
    ATEMRAS_PT_SETSLAVEDISCONNECTED pECTransf   = EC_NULL;
    EC_T_DWORD                  dwMasterInstance = 0;

    pECTransf = (ATEMRAS_PT_SETSLAVEDISCONNECTED)ATEMRAS_PROTOHDR_DATA(pTransferHead);

    dwMasterInstance = ATEMRAS_SETSLAVEDISCONNECTED_GET_INSTANCEID(pECTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emSetSlaveDisconnected);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emSetSlaveDisconnected(dwMasterInstance,
            ATEMRAS_SETSLAVEDISCONNECTED_GET_FIXEDADDRESSING(pECTransf),
            ATEMRAS_SETSLAVEDISCONNECTED_GET_SLAVEADDRESS(pECTransf),
            ATEMRAS_SETSLAVEDISCONNECTED_GET_DISCONNECTED(pECTransf)
            );
    }
    

    ATEMRAS_SETSLAVEDISCONNECTED_SET_SRVRESULT(pECTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emRescueScan

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalRescueScan(
    ATEMRAS_T_PROTOHDR*     pTransferHead,
    EC_T_DWORD              dwWrtLen
    )
{
    EC_T_DWORD            dwRetVal          = EC_E_ERROR;
    EC_T_DWORD            dwRes             = EC_E_ERROR;
    ATEMRAS_T_RESCUESCAN* pECTransf         = EC_NULL;
    EC_T_DWORD            dwMasterInstance  = 0;
    
    pECTransf = (ATEMRAS_T_RESCUESCAN*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    
    dwMasterInstance = ATEMRAS_RESCUESCAN_GET_INSTANCEID(pECTransf);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emRescueScan);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emRescueScan(dwMasterInstance, ATEMRAS_RESCUESCAN_GET_TIMEOUT(pECTransf));
    }

    ATEMRAS_RESCUESCAN_SET_SRVRESULT(pECTransf, dwRes);

    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emGetMasterInfo

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalGetMasterInfo(
    ATEMRAS_T_PROTOHDR*     pTransferHead, 
    EC_T_DWORD              dwWrtLen
    )
{
    EC_T_DWORD               dwRetVal = EC_E_ERROR;
    EC_T_DWORD               dwRes = EC_E_ERROR;
    ATEMRAS_T_GETMASTERINFO* pECTransf = (ATEMRAS_T_GETMASTERINFO*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    EC_T_DWORD               dwMasterInstance = ATEMRAS_GETMASTERINFO_GET_INSTANCEID(pECTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emGetMasterInfo);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emGetMasterInfo(dwMasterInstance, ATEMRAS_GETMASTERINFO_GET_MASTER_INFO_POINTER(pECTransf));
#if (defined EC_BIG_ENDIAN)
        /* swap all EC_T_DWORDS of EC_T_MASTER_INFO */
        {
            EC_T_DWORD* pdwInfoEntrySerialized = (EC_T_DWORD*)ATEMRAS_GETMASTERINFO_GET_MASTER_INFO_POINTER(pECTransf);
            for (size_t nEntries = sizeof(EC_T_MASTER_INFO) / sizeof(EC_T_DWORD); nEntries > 0; nEntries--, pdwInfoEntrySerialized++)
            {
                EC_SET_FRM_DWORD(pdwInfoEntrySerialized, *pdwInfoEntrySerialized);
            }
        }
#endif
    }
    
    ATEMRAS_GETMASTERINFO_SET_RESULT(pECTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Perform emConfigExtend

\return EC_E_NOERROR on success. error code otherwise.
*/
EC_T_DWORD CAtemRasSrvClient::deMarshalConfigExtend(
    ATEMRAS_T_PROTOHDR*     pTransferHead,  
    EC_T_DWORD              dwWrtLen
    )
{
    EC_T_DWORD               dwRetVal           = EC_E_ERROR;
    EC_T_DWORD               dwRes              = EC_E_ERROR;
    ATEMRAS_T_CONFIGEXTEND*  pECTransf          = (ATEMRAS_T_CONFIGEXTEND*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    EC_T_DWORD               dwMasterInstance   = ATEMRAS_CONFIGEXTEND_GET_INSTANCEID(pECTransf);
    
#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emConfigExtend);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        dwRes = emConfigExtend(dwMasterInstance,
            ATEMRAS_CONFIGEXTEND_GET_RESETCONFIG(pECTransf),
            ATEMRAS_CONFIGEXTEND_GET_TIMEOUT(pECTransf)
            );
    }
    
    ATEMRAS_CONFIGEXTEND_SET_SRVRESULT(pECTransf, dwRes);
    
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_DWORD CAtemRasSrvClient::deMarshalSetMbxProtocolsSerialize(
    ATEMRAS_T_PROTOHDR*        pTransferHead, 
    EC_T_DWORD                  dwWrtLen
    )
{
    EC_T_DWORD               dwRes                  = EC_E_ERROR;
    EC_T_DWORD               dwRetVal               = EC_E_ERROR;
    ATEMRAS_T_SETMBXPROTOCOLSSERIALIZE*  pECTransf  = (ATEMRAS_T_SETMBXPROTOCOLSSERIALIZE*)ATEMRAS_PROTOHDR_DATA(pTransferHead);
    EC_T_DWORD               dwMasterInstance       = ATEMRAS_SETMBXPROTOCOLSSERIALIZE_GET_INSTANCEID(pECTransf);

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    dwRes = m_pServer->GetAccessLevelByCall(ord_emConfigExtend);
    if (EC_E_NOERROR == dwRes)
#endif /* INCLUDE_RAS_SPOCSUPPORT */
    {
        if (ATEMRAS_SETMBXPROTOCOLSSERIALIZE_GET_SERIALIZE(pECTransf))
        {
            dwRes = emSlaveSerializeMbxTfers(dwMasterInstance, ATEMRAS_SETMBXPROTOCOLSSERIALIZE_GET_SLAVEID(pECTransf));
        }
        else
        {
            dwRes = emSlaveParallelMbxTfers(dwMasterInstance, ATEMRAS_SETMBXPROTOCOLSSERIALIZE_GET_SLAVEID(pECTransf));
        }
    }
    
    ATEMRAS_SETMBXPROTOCOLSSERIALIZE_SET_SRVRESULT(pECTransf, dwRes);
    dwRes = SendData((EC_T_BYTE*)pTransferHead, dwWrtLen);
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
\brief  Remote Notification routine.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD marshalNotify(
    EC_T_DWORD          dwCode,     /**< [in]   Notification code */
    EC_T_NOTIFYPARMS*   pParms      /**< [in]   Notification parameters */
                        )
{
    EC_T_DWORD                      dwRetVal            = EC_E_ERROR;
    EC_T_DWORD                      dwRes               = EC_E_ERROR;
    EMMARSH_PT_NOTIFICATION_CTXT    pContext            = EC_NULL;
    ATEMRAS_T_PROTOHDR*             pNotHeader          = EC_NULL;
    ATEMRAS_PT_NOTIFICATIONTRANSFER pNotBody            = EC_NULL;
    EC_T_BYTE*                      pbyWrtData          = EC_NULL;
    EC_T_DWORD                      dwSendData          = 0;
    EC_T_BYTE*                      pbyMbxData          = EC_NULL;
    EC_T_DWORD                      dwMbxDataSize       = 0;
    ATEMRAS_T_NONOTIFYMEMORYDESC    NoNotifyMemoryDesc;
    EMMARSH_T_NOTIFICATION_ALLOC*   pNotificationAlloc  = EC_NULL;

    if (EC_NULL == pParms)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pContext = (EMMARSH_PT_NOTIFICATION_CTXT)pParms->pCallerData;
    if ((EC_NULL == pContext) 
     || (EC_NULL == pContext->pClientReg) 
     || (EC_NULL == pContext->pConnection)
     || (emrasconstate_disconnect == pContext->pConnection->ConState())
     || (EC_NULL == pContext->pServer))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    
    if ((EC_NULL != pContext) && (EC_NULL != pContext->pClientReg))
    {
        if (!pContext->pClientReg->ChkAndTriggerNotificationType(dwCode))
        {
            dwRetVal = EC_E_NOERROR;
            goto Exit;
        }
    }

    /* Prepare NoNotifyMemory notification */
    EC_SET_FRM_DWORD(&NoNotifyMemoryDesc.dwCookie, pContext->dwCookie);
    EC_SET_FRM_DWORD(&NoNotifyMemoryDesc.dwCode, dwCode);
    
    switch (dwCode)
    {
    case EC_NOTIFY_DC_STATUS:
    case EC_NOTIFY_DC_SLV_SYNC:
/*
    case EC_NOTIFY_DC_MAST_SYNC:
    case EC_NOTIFY_DC_MAST_SYNC_CYC:
*/
    case EC_NOTIFY_DCL_STATUS:
#ifdef INCLUDE_COE_PDO_SUPPORT
    case EC_NOTIFY_COE_TX_PDO:
#endif
    case EC_NOTIFY_EOE_MBXSND_WKC_ERROR:
    case EC_NOTIFY_FOE_MBXSND_WKC_ERROR:
    case EC_NOTIFY_RAWCMD_DONE:
    {
        /* not supported by Remote API */
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    } break;
    case EC_NOTIFY_STATECHANGED:
    case EC_NOTIFY_CYCCMD_WKC_ERROR:
    case EC_NOTIFY_MASTER_INITCMD_WKC_ERROR:
    case EC_NOTIFY_SLAVE_INITCMD_WKC_ERROR:
    case EC_NOTIFY_COE_MBXSND_WKC_ERROR:
    case EC_NOTIFY_SLAVE_NOT_ADDRESSABLE:
        
    case EC_NOTIFY_FRAME_RESPONSE_ERROR:
    case EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR:
    case EC_NOTIFY_MBSLAVE_INITCMD_TIMEOUT:
    case EC_NOTIFY_MASTER_INITCMD_RESPONSE_ERROR:
    case EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL:
    case EC_NOTIFY_ALL_DEVICES_OPERATIONAL:
    case EC_NOTIFY_STATUS_SLAVE_ERROR:
    case EC_NOTIFY_SLAVE_ERROR_STATUS_INFO:
    case EC_NOTIFY_ETH_LINK_NOT_CONNECTED:
    case EC_NOTIFY_RED_LINEBRK:
    case EC_NOTIFY_RED_LINEFIXED:
    case EC_NOTIFY_FOE_MBSLAVE_ERROR:
    case EC_NOTIFY_ETH_LINK_CONNECTED:
    case EC_NOTIFY_SB_STATUS:
    case EC_NOTIFY_CLIENTREGISTRATION_DROPPED:
    case EC_NOTIFY_SLAVE_STATECHANGED:
    case EC_NOTIFY_SLAVES_STATECHANGED:
    case EC_NOTIFY_SB_MISMATCH:
    case EC_NOTIFY_SB_DUPLICATE_HC_NODE:
    case EC_NOTIFY_SLAVE_APPEARS:
    case EC_NOTIFY_SLAVE_DISAPPEARS:
    case EC_NOTIFY_HC_TOPOCHGDONE:
    case EC_NOTIFY_SLAVE_UNEXPECTED_STATE:
    case EC_NOTIFY_SLAVES_UNEXPECTED_STATE:
    case EC_NOTIFY_SLAVE_PRESENCE:
    case EC_NOTIFY_SLAVES_PRESENCE:
    case EC_NOTIFY_SLAVES_ERROR_STATUS:
    case EC_NOTIFY_FRAMELOSS_AFTER_SLAVE:
    case EC_NOTIFY_LINE_CROSSED:
        {
            /* supported */
            if (
                (EC_NULL == pContext) 
             || (EC_NULL == pContext->pClientReg) 
             || (pParms->dwInBufSize > pContext->pClientReg->m_pNotifyStore->GetEntrySize()) 
              )
            {
                pContext->pServer->NotifyCallee(ATEMRAS_NOTIFY_STDNOTIFYMEMORYSMALL, sizeof(ATEMRAS_T_NONOTIFYMEMORYDESC), (EC_T_BYTE*)&NoNotifyMemoryDesc);
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }
            /* allocate notification to send */
            pNotificationAlloc = pContext->pClientReg->m_pNotifyStore->AllocEntry();
            if (pNotificationAlloc == EC_NULL)
            {
                pContext->pServer->NotifyCallee(ATEMRAS_NOTIFY_NONOTIFYMEMORY, sizeof(ATEMRAS_T_NONOTIFYMEMORYDESC), (EC_T_BYTE*)&NoNotifyMemoryDesc);
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
    case EC_NOTIFY_MBOXRCV:
        {
            /* supported */
            if (
                (EC_NULL == pContext) 
             || (EC_NULL == pContext->pClientReg) 
             || (pParms->dwInBufSize > pContext->pClientReg->m_pNotifyStore->GetEntrySize()) 
              )
            {
                pContext->pServer->NotifyCallee(ATEMRAS_NOTIFY_MBXNOTIFYMEMORYSMALL, sizeof(ATEMRAS_T_NONOTIFYMEMORYDESC), (EC_T_BYTE*)&NoNotifyMemoryDesc);
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }

            /* allocate notification to send */
            pNotificationAlloc = pContext->pClientReg->m_pNotifyStore->AllocEntry();
            if (pNotificationAlloc == EC_NULL)
            {
                pContext->pServer->NotifyCallee(ATEMRAS_NOTIFY_NONOTIFYMEMORY, sizeof(ATEMRAS_T_NONOTIFYMEMORYDESC), (EC_T_BYTE*)&NoNotifyMemoryDesc);
                dwRetVal = dwRes;
                goto Exit;
            }
        } break;
    default:
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwRes = emrasProtoInsertNotificationHeader(pContext->dwCookie, pNotificationAlloc->byData, pContext->pClientReg->m_pNotifyStore->GetEntrySize());
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    pNotHeader  = (ATEMRAS_T_PROTOHDR*)pNotificationAlloc->byData;
    pNotBody    = (ATEMRAS_PT_NOTIFICATIONTRANSFER)ATEMRAS_PROTOHDR_DATA(pNotHeader);
    pbyWrtData  = ATEMRAS_NOTIF_DATA(pNotBody);
    
    dwSendData  = ATEMRAS_T_PROTOHDR_SIZE + ATEMRAS_T_NOTIFICATIONTRANSFER_SIZE;
    
    ATEMRAS_NOTIF_SET_INSTANCEID(pNotBody, (pContext->dwMasterInstance));
    ATEMRAS_NOTIF_SET_CLIENTID(pNotBody, (pContext->dwClientId));
    ATEMRAS_NOTIF_SET_CODE(pNotBody, dwCode);
    ATEMRAS_NOTIF_SET_INBUFFOFFSET(pNotBody, (EC_T_DWORD)(pbyWrtData-ATEMRAS_PROTOHDR_DATA(pNotHeader)));
    ATEMRAS_NOTIF_SET_INBUFSIZE(pNotBody, pParms->dwInBufSize);
    
    switch (dwCode)
    {
    case EC_NOTIFY_STATECHANGED:
        {
            /* supported by Remote API */
            EC_T_STATECHANGE* pStChngDeserialized = (EC_T_STATECHANGE*)(pParms->pbyInBuf);
            EC_T_STATECHANGE* pStChngSerialized = (EC_T_STATECHANGE*)pbyWrtData;
            
            OsMemcpy(pStChngSerialized, pStChngDeserialized, sizeof(EC_T_STATECHANGE));
            
#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD((EC_T_DWORD*)&(pStChngSerialized->oldState), (EC_T_DWORD)pStChngDeserialized->oldState);
            EC_SET_FRM_DWORD((EC_T_DWORD*)&(pStChngSerialized->newState), (EC_T_DWORD)pStChngDeserialized->newState);
#endif /* EC_BIG_ENDIAN */
            dwSendData += sizeof(EC_T_STATECHANGE);
        } break;
    case EC_NOTIFY_SLAVE_STATECHANGED:
        {
            EC_T_SLAVE_STATECHANGED_NTFY_DESC* pNotifyParaDeserialized = 
                (EC_T_SLAVE_STATECHANGED_NTFY_DESC*)(pParms->pbyInBuf);
            
            EC_T_SLAVE_STATECHANGED_NTFY_DESC* pNotifyParaSerialized = 
                (EC_T_SLAVE_STATECHANGED_NTFY_DESC*)pbyWrtData;
            
            OsMemcpy( pNotifyParaSerialized, pNotifyParaDeserialized, sizeof(EC_T_SLAVE_STATECHANGED_NTFY_DESC));
            
#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_WORD (&(pNotifyParaSerialized->SlaveProp.wStationAddress), 
                    pNotifyParaDeserialized->SlaveProp.wStationAddress);
            EC_SET_FRM_WORD (&(pNotifyParaSerialized->SlaveProp.wAutoIncAddr),
                    pNotifyParaDeserialized->SlaveProp.wAutoIncAddr);
            EC_SET_FRM_DWORD((EC_T_DWORD*)&(pNotifyParaSerialized->newState),
                    (EC_T_DWORD)pNotifyParaDeserialized->newState);
#endif /* EC_BIG_ENDIAN */
            dwSendData += sizeof(EC_T_SLAVE_STATECHANGED_NTFY_DESC);
        } break;
    
    case EC_NOTIFY_SLAVES_STATECHANGED:
        {
            EC_T_SLAVES_STATECHANGED_NTFY_DESC* pNotifyParaDeserialized = 
                (EC_T_SLAVES_STATECHANGED_NTFY_DESC*)(pParms->pbyInBuf);
            
            EC_T_SLAVES_STATECHANGED_NTFY_DESC* pNotifyParaSerialized = 
                (EC_T_SLAVES_STATECHANGED_NTFY_DESC*)pbyWrtData;
            
            OsMemcpy( pNotifyParaSerialized, pNotifyParaDeserialized, sizeof(EC_T_SLAVES_STATECHANGED_NTFY_DESC));
            
#if (defined EC_BIG_ENDIAN)
            for (EC_T_DWORD dwSlaveIdx = 0; dwSlaveIdx < pNotifyParaDeserialized->wCount; dwSlaveIdx++)
            {
                EC_SET_FRM_WORD (&(pNotifyParaSerialized->SlaveStates[dwSlaveIdx].wStationAddress), 
                        pNotifyParaDeserialized->SlaveStates[dwSlaveIdx].wStationAddress);
            }
#endif /* EC_BIG_ENDIAN */
            dwSendData += sizeof(EC_T_SLAVES_STATECHANGED_NTFY_DESC);
        } break;

    case EC_NOTIFY_CYCCMD_WKC_ERROR:
    case EC_NOTIFY_MASTER_INITCMD_WKC_ERROR:
    case EC_NOTIFY_SLAVE_INITCMD_WKC_ERROR:
    case EC_NOTIFY_COE_MBXSND_WKC_ERROR:
    case EC_NOTIFY_SLAVE_NOT_ADDRESSABLE:
        {
            /* supported by Remote API */
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_ERROR_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescSerialized   = (EC_T_ERROR_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(EC_T_ERROR_NOTIFICATION_DESC) - sizeof(pErrorNotificationDescDeserialized->desc) + sizeof(EC_T_WKCERR_DESC);
    
            OsMemcpy( pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);
    
#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->dwNotifyErrorCode),  pErrorNotificationDescDeserialized->dwNotifyErrorCode);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.WkcErrDesc.SlaveProp.wStationAddress), pErrorNotificationDescDeserialized->desc.WkcErrDesc.SlaveProp.wStationAddress);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.WkcErrDesc.SlaveProp.wAutoIncAddr),    pErrorNotificationDescDeserialized->desc.WkcErrDesc.SlaveProp.wAutoIncAddr);
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->desc.WkcErrDesc.dwAddr),  pErrorNotificationDescDeserialized->desc.WkcErrDesc.dwAddr);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.WkcErrDesc.wWkcSet), pErrorNotificationDescDeserialized->desc.WkcErrDesc.wWkcSet);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.WkcErrDesc.wWkcAct), pErrorNotificationDescDeserialized->desc.WkcErrDesc.wWkcAct);
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;
    
    case EC_NOTIFY_FRAME_RESPONSE_ERROR:
        {
            /* supported by Remote API */
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_ERROR_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescSerialized   = (EC_T_ERROR_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(EC_T_ERROR_NOTIFICATION_DESC) - sizeof(pErrorNotificationDescDeserialized->desc) + sizeof(EC_T_FRAME_RSPERR_DESC);

            OsMemcpy( pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->dwNotifyErrorCode),  pErrorNotificationDescDeserialized->dwNotifyErrorCode);
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->desc.FrameRspErrDesc.bIsCyclicFrame), pErrorNotificationDescDeserialized->desc.FrameRspErrDesc.bIsCyclicFrame);
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->desc.FrameRspErrDesc.EErrorType),     pErrorNotificationDescDeserialized->desc.FrameRspErrDesc.EErrorType);
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;

    case EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR:
    case EC_NOTIFY_MBSLAVE_INITCMD_TIMEOUT:
    case EC_NOTIFY_MASTER_INITCMD_RESPONSE_ERROR:
        {
            /* supported by Remote API */
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_ERROR_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescSerialized   = (EC_T_ERROR_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(EC_T_ERROR_NOTIFICATION_DESC) - sizeof(pErrorNotificationDescDeserialized->desc) + sizeof(EC_T_INITCMD_ERR_DESC);

            OsMemcpy( pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->dwNotifyErrorCode),  pErrorNotificationDescDeserialized->dwNotifyErrorCode);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.InitCmdErrDesc.SlaveProp.wStationAddress), pErrorNotificationDescDeserialized->desc.InitCmdErrDesc.SlaveProp.wStationAddress);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.InitCmdErrDesc.SlaveProp.wAutoIncAddr),    pErrorNotificationDescDeserialized->desc.InitCmdErrDesc.SlaveProp.wAutoIncAddr);
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->desc.InitCmdErrDesc.EErrorType), pErrorNotificationDescDeserialized->desc.InitCmdErrDesc.EErrorType);
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;

    case EC_NOTIFY_SLAVE_ERROR_STATUS_INFO:
        {
            /* supported by Remote API */
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_ERROR_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescSerialized   = (EC_T_ERROR_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(EC_T_ERROR_NOTIFICATION_DESC) - sizeof(pErrorNotificationDescDeserialized->desc) + sizeof(EC_T_SLAVE_ERROR_INFO_DESC);

            OsMemcpy( pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->dwNotifyErrorCode),  pErrorNotificationDescDeserialized->dwNotifyErrorCode);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.SlaveErrInfoDesc.SlaveProp.wStationAddress), pErrorNotificationDescDeserialized->desc.SlaveErrInfoDesc.SlaveProp.wStationAddress);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.SlaveErrInfoDesc.SlaveProp.wAutoIncAddr),    pErrorNotificationDescDeserialized->desc.SlaveErrInfoDesc.SlaveProp.wAutoIncAddr);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.SlaveErrInfoDesc.wStatus),     pErrorNotificationDescDeserialized->desc.SlaveErrInfoDesc.wStatus);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.SlaveErrInfoDesc.wStatusCode), pErrorNotificationDescDeserialized->desc.SlaveErrInfoDesc.wStatusCode);
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;
    
    case EC_NOTIFY_RED_LINEBRK:
        {
            /* supported by Remote API */
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_ERROR_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescSerialized   = (EC_T_ERROR_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(EC_T_ERROR_NOTIFICATION_DESC) - sizeof(pErrorNotificationDescDeserialized->desc) + sizeof(EC_T_RED_CHANGE_DESC);

            OsMemcpy( pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->dwNotifyErrorCode),  pErrorNotificationDescDeserialized->dwNotifyErrorCode);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.RedChangeDesc.wNumOfSlavesMain), pErrorNotificationDescDeserialized->desc.RedChangeDesc.wNumOfSlavesMain);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.RedChangeDesc.wNumOfSlavesRed),  pErrorNotificationDescDeserialized->desc.RedChangeDesc.wNumOfSlavesRed);
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;

    case EC_NOTIFY_LINE_CROSSED:
        {
            /* supported by Remote API */
            EC_T_NOTIFICATION_DESC* pNotificationDescDeserialized = (EC_T_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_NOTIFICATION_DESC* pNotificationDescSerialized = (EC_T_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(EC_T_NOTIFICATION_DESC) - sizeof(pNotificationDescDeserialized->desc) + sizeof(EC_T_LINE_CROSSED_DESC);

            OsMemcpy(pNotificationDescSerialized, pNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_WORD(&(pNotificationDescSerialized->desc.CrossedLineDesc.wInputPort), pNotificationDescDeserialized->desc.CrossedLineDesc.wInputPort);
            EC_SET_FRM_WORD(&(pNotificationDescSerialized->desc.CrossedLineDesc.SlaveProp.wAutoIncAddr), pNotificationDescDeserialized->desc.CrossedLineDesc.SlaveProp.wAutoIncAddr);
            EC_SET_FRM_WORD(&(pNotificationDescSerialized->desc.CrossedLineDesc.SlaveProp.wStationAddress), pNotificationDescDeserialized->desc.CrossedLineDesc.SlaveProp.wStationAddress);
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;
    case EC_NOTIFY_SB_MISMATCH:
    case EC_NOTIFY_SB_DUPLICATE_HC_NODE:
        {
            /* supported by Remote API */
            EC_T_NOTIFICATION_DESC* pNotificationDescDeserialized = (EC_T_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_NOTIFICATION_DESC* pNotificationDescSerialized   = (EC_T_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(EC_T_SB_MISMATCH_DESC);

            OsMemcpy( pNotificationDescSerialized, pNotificationDescDeserialized, dwCopySize);

#ifndef EC_BIG_ENDIAN
            EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.ScanBusMismatch.wPrevFixedAddress), pNotificationDescSerialized->desc.ScanBusMismatch.wPrevFixedAddress);

            EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.ScanBusMismatch.wPrevPort), pNotificationDescSerialized->desc.ScanBusMismatch.wPrevPort);
            EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.ScanBusMismatch.wPrevAIncAddress), pNotificationDescSerialized->desc.ScanBusMismatch.wPrevAIncAddress);
                                                
            EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.ScanBusMismatch.wBusAIncAddress), pNotificationDescSerialized->desc.ScanBusMismatch.wBusAIncAddress);
            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.ScanBusMismatch.dwBusVendorId), pNotificationDescSerialized->desc.ScanBusMismatch.dwBusVendorId);
            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.ScanBusMismatch.dwBusProdCode), pNotificationDescSerialized->desc.ScanBusMismatch.dwBusProdCode);
            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.ScanBusMismatch.dwBusRevisionNo), pNotificationDescSerialized->desc.ScanBusMismatch.dwBusRevisionNo);
            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.ScanBusMismatch.dwBusSerialNo), pNotificationDescSerialized->desc.ScanBusMismatch.dwBusSerialNo);
                                                
            EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.ScanBusMismatch.wBusFixedAddress), pNotificationDescSerialized->desc.ScanBusMismatch.wBusFixedAddress);
            EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.ScanBusMismatch.wIdentificationVal), pNotificationDescSerialized->desc.ScanBusMismatch.wIdentificationVal);
                        
            EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.ScanBusMismatch.wCfgFixedAddress), pNotificationDescSerialized->desc.ScanBusMismatch.wCfgFixedAddress);
            EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.ScanBusMismatch.wCfgAIncAddress), pNotificationDescSerialized->desc.ScanBusMismatch.wCfgAIncAddress);
                        
            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.ScanBusMismatch.dwCfgVendorId), pNotificationDescSerialized->desc.ScanBusMismatch.dwCfgVendorId);
            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.ScanBusMismatch.dwCfgProdCode), pNotificationDescSerialized->desc.ScanBusMismatch.dwCfgProdCode);
            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.ScanBusMismatch.dwCfgRevisionNo), pNotificationDescSerialized->desc.ScanBusMismatch.dwCfgRevisionNo);
            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.ScanBusMismatch.dwCfgSerialNo), pNotificationDescSerialized->desc.ScanBusMismatch.dwCfgSerialNo);
                                                
            EC_SET_FRM_BOOL (&(pNotificationDescSerialized->desc.ScanBusMismatch.bIdentValidationError), pNotificationDescSerialized->desc.ScanBusMismatch.bIdentValidationError);

            for( EC_T_INT iIdentCmdHdrPos = 0; iIdentCmdHdrPos < 5; iIdentCmdHdrPos++)
            {
                EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.ScanBusMismatch.oIdentCmdHdr[iIdentCmdHdrPos]), pNotificationDescSerialized->desc.ScanBusMismatch.oIdentCmdHdr[iIdentCmdHdrPos]);
            }

            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.ScanBusMismatch.dwCmdData), pNotificationDescSerialized->desc.ScanBusMismatch.dwCmdData);
            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.ScanBusMismatch.dwCmdVMask), pNotificationDescSerialized->desc.ScanBusMismatch.dwCmdVMask);
            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.ScanBusMismatch.dwCmdVData), pNotificationDescSerialized->desc.ScanBusMismatch.dwCmdVData);
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;

    case EC_NOTIFY_SLAVE_APPEARS:
    case EC_NOTIFY_SLAVE_DISAPPEARS:
        {
            /* obsolete */
            EC_T_NOTIFICATION_DESC* pNotificationDescDeserialized = (EC_T_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_NOTIFICATION_DESC* pNotificationDescSerialized   = (EC_T_NOTIFICATION_DESC*)pbyWrtData;

            OsMemcpy( pNotificationDescSerialized, pNotificationDescDeserialized, sizeof(EC_T_HC_SLAVE_CHANGE_DESC));

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.HCSlvChgDesc.SlaveProp.wStationAddress), pNotificationDescDeserialized->desc.HCSlvChgDesc.SlaveProp.wStationAddress);
            EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.HCSlvChgDesc.SlaveProp.wAutoIncAddr),    pNotificationDescDeserialized->desc.HCSlvChgDesc.SlaveProp.wAutoIncAddr);
#endif /* EC_BIG_ENDIAN */
            dwSendData += sizeof(EC_T_HC_SLAVE_CHANGE_DESC);
        } break;

    case EC_NOTIFY_MBXRCV_INVALID_DATA:
        {
            /* supported by Remote API */
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_ERROR_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescSerialized   = (EC_T_ERROR_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(EC_T_ERROR_NOTIFICATION_DESC) - sizeof(pErrorNotificationDescDeserialized->desc) + sizeof(EC_T_MBXRCV_INVALID_DATA_DESC);

            OsMemcpy( pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->dwNotifyErrorCode),  pErrorNotificationDescDeserialized->dwNotifyErrorCode);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.MbxRcvInvalidDataDesc.SlaveProp.wStationAddress), pErrorNotificationDescDeserialized->desc.MbxRcvInvalidDataDesc.SlaveProp.wStationAddress);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.MbxRcvInvalidDataDesc.SlaveProp.wAutoIncAddr),    pErrorNotificationDescDeserialized->desc.MbxRcvInvalidDataDesc.SlaveProp.wAutoIncAddr);
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;

    case EC_NOTIFY_SLAVE_UNEXPECTED_STATE:
        {
            /* supported by Remote API */
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_ERROR_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescSerialized   = (EC_T_ERROR_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(EC_T_ERROR_NOTIFICATION_DESC) - sizeof(pErrorNotificationDescDeserialized->desc) + sizeof(EC_T_SLAVE_UNEXPECTED_STATE_DESC);

            OsMemcpy( pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->dwNotifyErrorCode),  pErrorNotificationDescDeserialized->dwNotifyErrorCode);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.SlaveUnexpectedStateDesc.SlaveProp.wStationAddress), pErrorNotificationDescDeserialized->desc.SlaveUnexpectedStateDesc.SlaveProp.wStationAddress);
            EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.SlaveUnexpectedStateDesc.SlaveProp.wAutoIncAddr),    pErrorNotificationDescDeserialized->desc.SlaveUnexpectedStateDesc.SlaveProp.wAutoIncAddr);
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->desc.SlaveUnexpectedStateDesc.curState), pErrorNotificationDescDeserialized->desc.SlaveUnexpectedStateDesc.curState);
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->desc.SlaveUnexpectedStateDesc.expState), pErrorNotificationDescDeserialized->desc.SlaveUnexpectedStateDesc.expState);
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;

    case EC_NOTIFY_SLAVES_UNEXPECTED_STATE:
        {
            /* supported by Remote API */
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_ERROR_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescSerialized   = (EC_T_ERROR_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD dwCopySize = sizeof(EC_T_DWORD) + pErrorNotificationDescDeserialized->desc.SlavesUnexpectedStateDesc.wCount * sizeof(EC_T_SLAVES_UNEXPECTED_STATE_DESC_ENTRY);

            OsMemcpy( pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->dwNotifyErrorCode),  pErrorNotificationDescDeserialized->dwNotifyErrorCode);
            EC_SET_FRM_WORD(&(pErrorNotificationDescSerialized->desc.SlavesUnexpectedStateDesc.wCount),  pErrorNotificationDescDeserialized->desc.SlavesUnexpectedStateDesc.wCount);
            for (EC_T_DWORD dwSlaveIdx = 0; dwSlaveIdx < pErrorNotificationDescSerialized->desc.SlavesUnexpectedStateDesc.wCount; dwSlaveIdx++)
            {
                EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.SlavesUnexpectedStateDesc.SlaveStates[dwSlaveIdx].wStationAddress),
                        pErrorNotificationDescDeserialized->desc.SlavesUnexpectedStateDesc.SlaveStates[dwSlaveIdx].wStationAddress);

                EC_SET_FRM_DWORD (&(pErrorNotificationDescSerialized->desc.SlavesUnexpectedStateDesc.SlaveStates[dwSlaveIdx].curState),
                        (EC_T_DWORD)pErrorNotificationDescDeserialized->desc.SlavesUnexpectedStateDesc.SlaveStates[dwSlaveIdx].curState);

                EC_SET_FRM_DWORD (&(pErrorNotificationDescSerialized->desc.SlavesUnexpectedStateDesc.SlaveStates[dwSlaveIdx].expState),
                        (EC_T_DWORD)pErrorNotificationDescDeserialized->desc.SlavesUnexpectedStateDesc.SlaveStates[dwSlaveIdx].expState);
            }
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;

    case EC_NOTIFY_SLAVE_PRESENCE:
        {
            /* supported by Remote API */
            EC_T_NOTIFICATION_DESC* pNotificationDescDeserialized = (EC_T_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_NOTIFICATION_DESC* pNotificationDescSerialized   = (EC_T_NOTIFICATION_DESC*)pbyWrtData;

            OsMemcpy(pNotificationDescSerialized, pNotificationDescDeserialized, sizeof(EC_T_SLAVE_PRESENCE_NTFY_DESC));

#if (defined EC_BIG_ENDIAN)            
            EC_SET_FRM_WORD (&(pNotificationDescSerialized->desc.SlavePresenceDesc.wStationAddress), pNotificationDescDeserialized->desc.SlavePresenceDesc.wStationAddress);
            EC_SET_FRM_BOOL (&(pNotificationDescSerialized->desc.SlavePresenceDesc.bPresent),        pNotificationDescDeserialized->desc.SlavePresenceDesc.bPresent);
#endif /* EC_BIG_ENDIAN */
            dwSendData += sizeof(EC_T_SLAVE_PRESENCE_NTFY_DESC);
        } break;

    case EC_NOTIFY_SLAVES_PRESENCE:
        {
            /* supported by Remote API */
            EC_T_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_NOTIFICATION_DESC* pErrorNotificationDescSerialized   = (EC_T_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD dwCopySize = sizeof(EC_T_DWORD) + pErrorNotificationDescDeserialized->desc.SlavesPresenceDesc.wCount * sizeof(EC_T_SLAVE_PRESENCE_NTFY_DESC);

            OsMemcpy(pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_WORD(&(pErrorNotificationDescSerialized->desc.SlavesPresenceDesc.wCount),  pErrorNotificationDescDeserialized->desc.SlavesPresenceDesc.wCount);
            for (EC_T_DWORD dwSlaveIdx = 0; dwSlaveIdx < pErrorNotificationDescSerialized->desc.SlavesPresenceDesc.wCount; dwSlaveIdx++)
            {
                EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.SlavesPresenceDesc.SlavePresence[dwSlaveIdx].wStationAddress),
                                 pErrorNotificationDescDeserialized->desc.SlavesPresenceDesc.SlavePresence[dwSlaveIdx].wStationAddress);
                EC_SET_FRM_BOOL (&(pErrorNotificationDescSerialized->desc.SlavesPresenceDesc.SlavePresence[dwSlaveIdx].bPresent),
                                 pErrorNotificationDescDeserialized->desc.SlavesPresenceDesc.SlavePresence[dwSlaveIdx].bPresent);
            }
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;
        
    case EC_NOTIFY_SLAVES_ERROR_STATUS:
        {
            /* supported by Remote API */
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_ERROR_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescSerialized   = (EC_T_ERROR_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(EC_T_DWORD) + pErrorNotificationDescSerialized->desc.SlavesErrDesc.wCount * sizeof(EC_T_SLAVES_ERROR_DESC_ENTRY);

            OsMemcpy(pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->dwNotifyErrorCode),  pErrorNotificationDescDeserialized->dwNotifyErrorCode);
            EC_SET_FRM_WORD(&(pErrorNotificationDescSerialized->desc.SlavesErrDesc.wCount),  
                            pErrorNotificationDescDeserialized->desc.SlavesErrDesc.wCount);
            for (EC_T_DWORD dwSlaveIdx = 0; dwSlaveIdx < pErrorNotificationDescSerialized->desc.SlavesErrDesc.wCount; dwSlaveIdx++)
            {
                EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.SlavesErrDesc.SlaveError[dwSlaveIdx].wStationAddress),
                                 pErrorNotificationDescDeserialized->desc.SlavesErrDesc.SlaveError[dwSlaveIdx].wStationAddress);
                EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.SlavesErrDesc.SlaveError[dwSlaveIdx].wStatus),
                                 pErrorNotificationDescDeserialized->desc.SlavesErrDesc.SlaveError[dwSlaveIdx].wStatus);
                EC_SET_FRM_WORD (&(pErrorNotificationDescSerialized->desc.SlavesErrDesc.SlaveError[dwSlaveIdx].wStatusCode),
                                 pErrorNotificationDescDeserialized->desc.SlavesErrDesc.SlaveError[dwSlaveIdx].wStatusCode);
            }
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;

    case EC_NOTIFY_HC_TOPOCHGDONE:
        {
            /* supported by Remote API */
            EC_T_NOTIFICATION_DESC* pNotificationDescDeserialized = (EC_T_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_NOTIFICATION_DESC* pNotificationDescSerialized   = (EC_T_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(pNotificationDescSerialized->desc.StatusCode);

            OsMemcpy( pNotificationDescSerialized, pNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pNotificationDescSerialized->desc.StatusCode),  pNotificationDescDeserialized->desc.StatusCode);
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;

    case EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL:
    case EC_NOTIFY_ALL_DEVICES_OPERATIONAL:
    case EC_NOTIFY_STATUS_SLAVE_ERROR:
    case EC_NOTIFY_ETH_LINK_NOT_CONNECTED:
    case EC_NOTIFY_RED_LINEFIXED:
    case EC_NOTIFY_ETH_LINK_CONNECTED:
        {
            /* supported by Remote API */
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_ERROR_NOTIFICATION_DESC*)pParms->pbyInBuf;
            EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescSerialized   = (EC_T_ERROR_NOTIFICATION_DESC*)pbyWrtData;
            EC_T_DWORD  dwCopySize = sizeof(EC_T_ERROR_NOTIFICATION_DESC) - sizeof(pErrorNotificationDescDeserialized->desc);

            OsMemcpy( pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pErrorNotificationDescSerialized->dwNotifyErrorCode),  pErrorNotificationDescDeserialized->dwNotifyErrorCode);
#endif /* EC_BIG_ENDIAN */
            dwSendData += dwCopySize;
        } break;

    case EC_NOTIFY_CLIENTREGISTRATION_DROPPED:
        {
            if (pParms->dwInBufSize >= sizeof(EC_T_DWORD))
            {
                /* supported by Remote API */
                EC_T_DWORD* pNotificationParamDeserialized = (EC_T_DWORD*)pParms->pbyInBuf;
                EC_T_DWORD* pNotificationParamSerialized   = (EC_T_DWORD*)pbyWrtData;

                EC_SET_FRM_DWORD(pNotificationParamSerialized, *pNotificationParamDeserialized);
                dwSendData += sizeof(EC_T_DWORD);
            }
        } break;
    
    case EC_NOTIFY_SB_STATUS:
        {
            /* supported by Remote API */
            EC_T_SB_STATUS_NTFY_DESC* pDeserializedNotification = (EC_T_SB_STATUS_NTFY_DESC*)pParms->pbyInBuf;
            EC_T_SB_STATUS_NTFY_DESC* pSerializedNotification = (EC_T_SB_STATUS_NTFY_DESC*)pbyWrtData;

            OsMemcpy( pSerializedNotification, pDeserializedNotification, sizeof(EC_T_SB_STATUS_NTFY_DESC));

#if (defined EC_BIG_ENDIAN)
            EC_SET_FRM_DWORD(&(pSerializedNotification->dwResultCode), pDeserializedNotification->dwResultCode);
            EC_SET_FRM_DWORD(&(pSerializedNotification->dwSlaveCount), pDeserializedNotification->dwSlaveCount);
#endif /* EC_BIG_ENDIAN */

            dwSendData += sizeof(EC_T_SB_STATUS_NTFY_DESC);
        } break;

    case EC_NOTIFY_FRAMELOSS_AFTER_SLAVE:
    {
        /* supported by Remote API */
        EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescDeserialized = (EC_T_ERROR_NOTIFICATION_DESC*)pParms->pbyInBuf;
        EC_T_ERROR_NOTIFICATION_DESC* pErrorNotificationDescSerialized = (EC_T_ERROR_NOTIFICATION_DESC*)pbyWrtData;
        EC_T_DWORD dwCopySize = sizeof(EC_T_ERROR_NOTIFICATION_DESC) - sizeof(pErrorNotificationDescDeserialized->desc) + sizeof(EC_T_FRAMELOSS_AFTER_SLAVE_NTFY_DESC);

        OsMemcpy(pErrorNotificationDescSerialized, pErrorNotificationDescDeserialized, dwCopySize);

#if (defined EC_BIG_ENDIAN)
        EC_SET_FRM_WORD(&(pErrorNotificationDescSerialized->desc.FramelossAfterSlaveDesc.SlaveProp.wStationAddress),
            pErrorNotificationDescDeserialized->desc.FramelossAfterSlaveDesc.SlaveProp.wStationAddress);
        EC_SET_FRM_WORD(&(pErrorNotificationDescSerialized->desc.FramelossAfterSlaveDesc.SlaveProp.wAutoIncAddr),
            pErrorNotificationDescDeserialized->desc.FramelossAfterSlaveDesc.SlaveProp.wAutoIncAddr);
        EC_SET_FRM_WORD(&(pErrorNotificationDescSerialized->desc.FramelossAfterSlaveDesc.wPort),
            pErrorNotificationDescDeserialized->desc.FramelossAfterSlaveDesc.wPort);
#endif /* EC_BIG_ENDIAN */

        dwSendData += dwCopySize;
    } break;

    case EC_NOTIFY_MBOXRCV:
        {
            ATEMRAS_PT_MBXNOTIFICATIONTRANSFER  pRemMbx     = EC_NULL;
            ATEMRAS_PT_MBXTFER                  pRemMbxTfer = EC_NULL;
            CAtEmClientMbxTferObject*           pMbxAdm     = EC_NULL;
            EC_T_MBXTFER*                       pLocMbx     = EC_NULL;
            EC_T_DWORD                          dwLocMbxId  = 0;
            EC_T_INT                            nRemSize    = 0;

            pMbxAdm = pContext->pMasterInstance->MbxTferObj(((EC_T_MBXTFER*)pParms->pbyInBuf)->dwTferId);

            nRemSize = pContext->pClientReg->m_pNotifyStore->GetEntrySize();
            nRemSize -= sizeof(ATEMRAS_T_PROTOHDR);
            nRemSize -= sizeof(ATEMRAS_T_NOTIFICATIONTRANSFER);
            nRemSize -= ATEMRAS_T_MBXNOTIFICATIONTRANSFER_SIZE;
            if (0 >= nRemSize)
            {
                pContext->pServer->NotifyCallee(ATEMRAS_NOTIFY_MBXNOTIFYMEMORYSMALL, sizeof(ATEMRAS_T_NONOTIFYMEMORYDESC), (EC_T_BYTE*)&NoNotifyMemoryDesc);
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }

            if (EC_NULL == pMbxAdm)
            {
                /* non-administered MBX Transfer like CoE/SoE emergency */
                pLocMbx     = ((EC_T_MBXTFER*)pParms->pbyInBuf);
                dwLocMbxId  = ((EC_T_DWORD)(pParms->pbyInBuf-(EC_T_BYTE*)EC_NULL));
            }
            else
            {
                pLocMbx     = pMbxAdm->TferObj();
                dwLocMbxId  = pMbxAdm->TferObjId();
            }

#if (defined DEBUG)
            if (((EC_T_MBXTFER*)pParms->pbyInBuf)->dwClntId != 0)
            {
                OsDbgAssert(pContext->pClientReg->ClientId() == ((EC_T_MBXTFER*)pParms->pbyInBuf)->dwClntId);
            }
            else
            {
                switch (pLocMbx->eMbxTferType)
                {
                case eMbxTferType_COE_EMERGENCY:
                case eMbxTferType_SOE_EMERGENCY:
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                }
            }
#endif /* DEBUG */

            pRemMbx     = ((ATEMRAS_PT_MBXNOTIFICATIONTRANSFER)(ATEMRAS_PROTOHDR_DATA(pNotHeader)+ATEMRAS_NOTIF_GET_INBUFOFFSET(pNotBody)));
            pRemMbxTfer = ATEMRAS_MBXNOTIFICATIONTRANSFER_TFEROBJ(pRemMbx);
            pbyWrtData  = ATEMRAS_MBXNOTIFICATIONTRANSFER_DATA(pRemMbx);
            //EC_T_BYTE * pbyTest = pbyWrtData;

            dwSendData += ATEMRAS_T_MBXNOTIFICATIONTRANSFER_SIZE;

            ATEMRAS_MBXNOTIFICATIONTRANSFER_SET_MBXOBJID(pRemMbx, dwLocMbxId);

            ATEMRAS_MBXTFERTRANSFER_SET_CLIENTID(pRemMbxTfer,       pLocMbx->dwClntId);
            ATEMRAS_MBXTFERTRANSFER_SET_MBXTFERTYPE(pRemMbxTfer,    (EC_T_DWORD)pLocMbx->eMbxTferType);
            ATEMRAS_MBXTFERTRANSFER_SET_MAXDATALEN(pRemMbxTfer,     pLocMbx->MbxTferDesc.dwMaxDataLen);
            ATEMRAS_MBXTFERTRANSFER_SET_DATAOFFSET(pRemMbxTfer,     (EC_T_DWORD)(pbyWrtData-ATEMRAS_PROTOHDR_DATA(pNotHeader)));
            
            if (EC_E_NOERROR != pLocMbx->dwErrorCode)
            {
                pLocMbx->dwDataLen = 0;
            }
            
            ATEMRAS_MBXTFERTRANSFER_SET_DATALEN(pRemMbxTfer,        pLocMbx->dwDataLen);
            ATEMRAS_MBXTFERTRANSFER_SET_STATUS(pRemMbxTfer,     (EC_T_DWORD)(pLocMbx->eTferStatus));
            ATEMRAS_MBXTFERTRANSFER_SET_ERRORCODE(pRemMbxTfer,  pLocMbx->dwErrorCode);

            /* non-administered MBX Transfer like CoE/SoE emergency? */
            if (EC_NULL == pMbxAdm)
            {
                EC_T_DWORD  dwCopySize = EC_MIN(nRemSize, ((EC_T_INT)pLocMbx->dwDataLen));
                
                OsMemcpy(pbyWrtData, pLocMbx->pbyMbxTferData, dwCopySize);
                dwSendData += dwCopySize;

                pbyWrtData = pbyWrtData + dwCopySize;
                nRemSize -= dwCopySize;

                /* buffer exceeded? */
                if (nRemSize < (EC_T_INT)(sizeof(EC_T_MBX_DATA)))
                {
                    pContext->pServer->NotifyCallee(ATEMRAS_NOTIFY_MBXNOTIFYMEMORYSMALL, sizeof(ATEMRAS_T_NONOTIFYMEMORYDESC), (EC_T_BYTE*)&NoNotifyMemoryDesc);
                }
                else
                {
                    switch (pLocMbx->eMbxTferType)
                    {
                    case eMbxTferType_COE_EMERGENCY:
                        {
                            /* supported by Remote API */
                            EC_T_COE_EMERGENCY* pMbxDataDeserialized = (EC_T_COE_EMERGENCY*)&pLocMbx->MbxData;
                            EC_T_COE_EMERGENCY* pMbxDataSerialized   = (EC_T_COE_EMERGENCY*)pbyWrtData;

                            OsMemcpy(pMbxDataSerialized, pMbxDataDeserialized, sizeof(EC_T_COE_EMERGENCY));
#if (defined EC_BIG_ENDIAN)
                            EC_SET_FRM_WORD(&pMbxDataSerialized->wErrorCode, pMbxDataDeserialized->wErrorCode);
                            EC_SET_FRM_WORD(&pMbxDataSerialized->wStationAddress, pMbxDataDeserialized->wStationAddress);
#endif /* EC_BIG_ENDIAN */

                            dwSendData += sizeof(EC_T_COE_EMERGENCY);
                            nRemSize -= sizeof(EC_T_COE_EMERGENCY);
                        } break;
                    case eMbxTferType_SOE_NOTIFICATION:
                        {
                            /* not supported by Remote API */
                            EC_T_SOE_NOTIFICATION* pMbxDataDeserialized = (EC_T_SOE_NOTIFICATION*)&pLocMbx->MbxData;
                            EC_T_SOE_NOTIFICATION* pMbxDataSerialized   = (EC_T_SOE_NOTIFICATION*)pbyWrtData;

                            OsMemcpy(pMbxDataSerialized, pMbxDataDeserialized, sizeof(EC_T_SOE_NOTIFICATION));
#if (defined EC_BIG_ENDIAN)
                            EC_SET_FRM_WORD(&pMbxDataSerialized->wHeader, pMbxDataDeserialized->wHeader);
                            EC_SET_FRM_WORD(&pMbxDataSerialized->wIdn, pMbxDataDeserialized->wIdn);
                            EC_SET_FRM_WORD(&pMbxDataSerialized->wStationAddress, pMbxDataDeserialized->wStationAddress);
#endif /* EC_BIG_ENDIAN */

                            dwSendData += sizeof(EC_T_SOE_NOTIFICATION);
                            nRemSize -= sizeof(EC_T_SOE_NOTIFICATION);
                        } break;
                    case eMbxTferType_SOE_EMERGENCY:
                        {
                            /* not supported by Remote API */
                            EC_T_SOE_EMERGENCY* pMbxDataDeserialized = (EC_T_SOE_EMERGENCY*)&pLocMbx->MbxData;
                            EC_T_SOE_EMERGENCY* pMbxDataSerialized   = (EC_T_SOE_EMERGENCY*)pbyWrtData;

                            OsMemcpy(pMbxDataSerialized, pMbxDataDeserialized, sizeof(EC_T_SOE_EMERGENCY));
#if (defined EC_BIG_ENDIAN)
                            EC_SET_FRM_WORD(&pMbxDataSerialized->wHeader, pMbxDataDeserialized->wHeader);
                            EC_SET_FRM_WORD(&pMbxDataSerialized->wStationAddress, pMbxDataDeserialized->wStationAddress);
#endif /* EC_BIG_ENDIAN */

                            dwSendData += sizeof(EC_T_SOE_EMERGENCY);
                            nRemSize -= sizeof(EC_T_SOE_EMERGENCY);
                        } break;
                    case eMbxTferType_VOE_MBX_READ:
                        dwRetVal = EC_E_NOTSUPPORTED;
                        goto Exit;
                        /* no break */
                    default:
                        /* non-administrated type, non-supported by Remote API */
                        OsDbgAssert(EC_FALSE);
                        goto Exit;
                    }
                }
            }
            else
            {
                switch (pLocMbx->eMbxTferType)
                {
                case eMbxTferType_FOE_FILE_UPLOAD:
                    pNotificationAlloc->pbyMbxData = pLocMbx->pbyMbxTferData;
                    pNotificationAlloc->dwMbxDataSize = pLocMbx->dwDataLen;

                    /* no break */
                case eMbxTferType_FOE_FILE_DOWNLOAD:
                {

                    EC_T_MBX_DATA_FOE* pMbxDataDeserialized = &pLocMbx->MbxData.FoE;
                    EC_T_MBX_DATA_FOE* pMbxDataSerialized = (EC_T_MBX_DATA_FOE*)pbyWrtData;


                    OsMemcpy(pMbxDataSerialized, pMbxDataDeserialized, sizeof(EC_T_MBX_DATA_FOE));
#if (defined EC_BIG_ENDIAN)
                    EC_SET_FRM_DWORD(&pMbxDataSerialized->dwTransferredBytes, pMbxDataDeserialized->dwTransferredBytes);
                    EC_SET_FRM_DWORD(&pMbxDataSerialized->dwRequestedBytes, pMbxDataDeserialized->dwRequestedBytes);
                    EC_SET_FRM_DWORD(&pMbxDataSerialized->dwBusyDone, pMbxDataDeserialized->dwBusyDone);
                    EC_SET_FRM_DWORD(&pMbxDataSerialized->dwBusyEntire, pMbxDataDeserialized->dwBusyEntire);
                    EC_SET_FRM_DWORD(&pMbxDataSerialized->dwFileSize, pMbxDataDeserialized->dwFileSize);
#endif /* EC_BIG_ENDIAN */

                    dwSendData += sizeof(EC_T_MBX_DATA_FOE);

                } break;
                default:
                    /* use memory of create mailbox transfer object */
                    
                    break;
                }
            
                pbyMbxData = pLocMbx->pbyMbxTferData;
                dwMbxDataSize = pLocMbx->dwDataLen;
                ATEMRAS_MBXTFERTRANSFER_SET_TRANSFERID(pRemMbxTfer, pMbxAdm->GetTferId());
            }

            /* just to be sure, do not send more data than buffer size :) */
            dwSendData  = EC_MIN(dwSendData, pContext->pClientReg->m_pNotifyStore->GetEntrySize());

        } break;
    
    default:
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    }

    /* insert corrected header length */
    dwRes = emrasProtoInsertNotificationHeader(pContext->dwCookie, pNotificationAlloc->byData, dwSendData + dwMbxDataSize);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    pNotificationAlloc->dwDataSize = dwSendData;
    pNotificationAlloc->dwMbxDataSize = dwMbxDataSize;
    pNotificationAlloc->pbyMbxData = pbyMbxData;
    pContext->pClientReg->m_pNotifyStore->StoreEntry(pNotificationAlloc);

    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        /* release resources */
        if (EC_NULL != pNotificationAlloc)
        {
            pContext->pClientReg->m_pNotifyStore->FreeEntry(pNotificationAlloc);
        }
    }
    return dwRetVal;
}

/*-END OF SOURCE FILE--------------------------------------------------------*/
