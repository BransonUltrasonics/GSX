/*-----------------------------------------------------------------------------
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              Interface description (EcMaster, RAS)
 *---------------------------------------------------------------------------*/

#ifndef INC_ECTYPE
#error Do not include this file directly!
#endif

#ifndef INC_ECMASTER_INTERFACE_BASE
#define INC_ECMASTER_INTERFACE_BASE

class CEcMasterInterfaceBase
{
public:
    virtual ~CEcMasterInterfaceBase() {}

    virtual EC_T_DWORD GetDefaultClntId(EC_T_DWORD              dwInstanceID        ) = 0;

    virtual EC_T_DWORD IoControl       (EC_T_DWORD              dwInstanceID,
                                        EC_T_DWORD              dwIoCtlCode,
                                        EC_T_IOCTLPARMS*        pParms              ) = 0;

            EC_T_DWORD RegisterClient  (EC_T_DWORD              dwInstanceID,  
                                        EC_PF_NOTIFY            pfnNotify,
                                        EC_T_VOID*              pCallerData,     
                                        EC_T_REGISTERRESULTS*   pRegResults         );

            EC_T_DWORD DeregisterClient(EC_T_DWORD              dwInstanceID,
                                        EC_T_DWORD              dwClntId            );

    virtual EC_T_STATE GetMasterState  (EC_T_DWORD              dwInstanceID        ) = 0;

    virtual EC_T_DWORD GetSlaveId      (EC_T_DWORD              dwInstanceID,
                                        EC_T_WORD               wStationAddress     ) = 0;

    virtual EC_T_MBXTFER* MbxTferCreate(EC_T_DWORD              dwInstanceID,
                                        EC_T_MBXTFER_DESC*      pMbxTferDesc        ) = 0;

    virtual EC_T_VOID MbxTferDelete    (EC_T_DWORD              dwInstanceID,
                                        EC_T_MBXTFER*           pMbxTfer            ) = 0;

    static  EC_T_BOOL IsValidCoeSdoParms(
                                        EC_T_WORD               wObIndex        /* not checked */, 
                                        EC_T_BYTE               byObSubIndex    /* must be 0 for complete access */, 
                                        EC_T_BYTE*              pbyData         /* must not be EC_NULL */, 
                                        EC_T_DWORD              dwDataLen       /* must not be 0 */, 
                                        EC_T_DWORD              dwTimeout       /* must not be EC_NOWAIT */, 
                                        EC_T_DWORD              dwFlags         /* must not be complete access for subindex != 0 */)
    {
        EC_UNREFPARM(wObIndex);

        if (EC_NOWAIT == dwTimeout)
        {
            EC_ERRORMSGC(("IsValidCoeSdoParms: dwTimeout = EC_NOWAIT is invalid!\n"));
            return EC_FALSE;
        }
        if ((EC_NULL == pbyData) && (0 == dwDataLen))
        {
            EC_ERRORMSGC(("IsValidCoeSdoParms: pbyData = NULL and dwDataLen = NULL is invalid!\n"));
            return EC_FALSE;
        }

        /* deny sdo complete access to subindex greater 1 */
        if ((1 == (dwFlags & EC_MAILBOX_FLAG_SDO_COMPLETE)) && (0 != byObSubIndex) && (1 != byObSubIndex))
        {
            EC_ERRORMSGC(("IsValidCoeSdoParms: bad subindex(%d) for complete access!\n", byObSubIndex));
            return EC_FALSE;
        }
        return EC_TRUE;
    }

    virtual EC_T_DWORD CoeSdoDownloadReq(EC_T_DWORD             dwInstanceID,
                                        EC_T_MBXTFER*           pMbxTfer,
                                        EC_T_DWORD              dwSlaveId,
                                        EC_T_WORD               wObIndex,
                                        EC_T_BYTE               byObSubIndex,
                                        EC_T_DWORD              dwTimeout,
                                        EC_T_DWORD              dwFlags             ) = 0;

    virtual EC_T_DWORD CoeSdoUploadReq( EC_T_DWORD              dwInstanceID,
                                        EC_T_MBXTFER*           pMbxTfer, 
                                        EC_T_DWORD              dwSlaveId, 
                                        EC_T_WORD               wObIndex, 
                                        EC_T_BYTE               byObSubIndex, 
                                        EC_T_DWORD              dwTimeout,
                                        EC_T_DWORD              dwFlags             ) = 0;

    virtual EC_T_DWORD CoeSdoUpload(    EC_T_DWORD              dwInstanceID,
                                        EC_T_DWORD              dwSlaveId,
                                        EC_T_WORD               wObIndex,
                                        EC_T_BYTE               byObSubIndex,
                                        EC_T_BYTE*              pbyData,
                                        EC_T_DWORD              dwDataLen,
                                        EC_T_DWORD*             pdwOutDataLen,
                                        EC_T_DWORD              dwTimeout,
                                        EC_T_DWORD              dwFlags             ) = 0;

    virtual EC_T_DWORD FoeUploadReq(    EC_T_DWORD              dwInstanceID,
                                        EC_T_MBXTFER*           pMbxTfer,
                                        EC_T_DWORD              dwSlaveId,
                                        EC_T_CHAR               szFileName[MAX_FILE_NAME_SIZE],
                                        EC_T_DWORD              dwFileNameLen,
                                        EC_T_DWORD              dwPassWd,
                                        EC_T_DWORD              dwTimeout) = 0;

    virtual EC_T_DWORD FoeDownloadReq(  EC_T_DWORD              dwInstanceID,
                                        EC_T_MBXTFER*           pMbxTfer,
                                        EC_T_DWORD              dwSlaveId,
                                        EC_T_CHAR               szFileName[MAX_FILE_NAME_SIZE],
                                        EC_T_DWORD              dwFileNameLen,
                                        EC_T_DWORD              dwPassWd,
                                        EC_T_DWORD              dwTimeout) = 0;

    virtual EC_T_DWORD FoeFileUpload(   EC_T_DWORD              dwInstanceID,
                                        EC_T_DWORD              dwSlaveId,
                                        EC_T_CHAR*              achFileName,
                                        EC_T_DWORD              dwFileNameLen,
                                        EC_T_BYTE*              pbyData,
                                        EC_T_DWORD              dwDataLen,
                                        EC_T_DWORD*             pdwOutDataLen,
                                        EC_T_DWORD              dwPassword,
                                        EC_T_DWORD              dwTimeout           ) = 0;

    virtual EC_T_DWORD FoeFileDownload( EC_T_DWORD              dwInstanceID,
                                        EC_T_DWORD              dwSlaveId,
                                        EC_T_CHAR*              szFileName,
                                        EC_T_DWORD              dwFileNameLen,
                                        EC_T_BYTE*              pbyData,
                                        EC_T_DWORD              dwDataLen,
                                        EC_T_DWORD              dwPassWd,
                                        EC_T_DWORD              dwTimeout           ) = 0;

    static EC_INLINESTART EC_T_BOOL IsValidSoeTransferParms(
        EC_T_BYTE   byDriveNo,          /**< [in]  number of drive in slave */
        EC_T_BYTE*  pbyElementFlags,    /**< [in/out]  which element of object is read */
        EC_T_WORD   wIDN,               /**< [in]  IDN of the object */
        EC_T_BYTE*  pbyData,            /**< [out] IDN data */
        EC_T_DWORD  dwDataLen,          /**< [in]  length of pbyData buffer */
        EC_T_DWORD* pdwOutDataLen,      /**< [out] IDN data length (number of received bytes) */
        EC_T_DWORD  dwTimeout,          /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
        EC_T_DWORD* pdwRetVal
        )
    {
        EC_UNREFPARM(wIDN);
        if (EC_NOWAIT == dwTimeout)
        {
            EC_ERRORMSGC(("IsValidSoeTransferParms: dwTimeout = EC_NOWAIT is invalid!\n"));
            *pdwRetVal = EC_E_INVALIDPARM;
            return EC_FALSE;
        }
        if ((EC_NULL == pbyData) && (0 == dwDataLen))
        {
            EC_ERRORMSGC(("IsValidSoeTransferParms: pbyData = NULL and dwDataLen = NULL is invalid!\n"));
            *pdwRetVal = EC_E_INVALIDPARM;
            return EC_FALSE;
        }
        if (EC_NULL == pdwOutDataLen)
        {
            EC_ERRORMSGC(("IsValidSoeTransferParms: pdwOutDataLen = NULL is invalid!\n"));
            *pdwRetVal = EC_E_INVALIDPARM;
            return EC_FALSE;
        }
        if (byDriveNo > 7)
        {
            EC_ERRORMSGC(("IsValidSoeTransferParms: bad DriveNo (%d)!\n", byDriveNo));
            *pdwRetVal = EC_E_SOE_ERRORCODE_INVL_DRIVE_NO;
            return EC_FALSE;
        }
        if (EC_NULL == pbyElementFlags)
        {
            EC_ERRORMSGC(("IsValidSoeTransferParms: pbyElementFlags = NULL is invalid!\n"));
            *pdwRetVal = EC_E_INVALIDPARM;
            return EC_FALSE;
        }
        if (0 == *pbyElementFlags)
        {
            EC_ERRORMSGC(("IsValidSoeTransferParms: *pbyElementFlags = 0 (not addressing anything) is invalid!\n"));
            *pdwRetVal = EC_E_SOE_ERRCODE_NO_ELEM_ADR;
            return EC_FALSE;
        }

        return EC_TRUE;
    } EC_INLINESTOP

    static EC_INLINESTART EC_T_BOOL IsValidSoeTransferParmsNonBlocking(
        EC_T_MBXTFER* pMbxTfer,
        EC_T_BYTE     byDriveNo,          /**< [in]  number of drive in slave */
        EC_T_BYTE*    pbyElementFlags,    /**< [in/out]  which element of object is read */
        EC_T_WORD     wIDN,               /**< [in]  IDN of the object */
        EC_T_DWORD    dwTimeout,          /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
        EC_T_DWORD*   pdwRetVal)
    {
        if (EC_NULL == pMbxTfer)
        {
            EC_ERRORMSGC(("IsValidSoeTransferParmsNonBlocking: pMbxTfer = NULL is invalid!\n"));
            *pdwRetVal = EC_E_INVALIDPARM;
            return EC_FALSE;
        }
        OsDbgAssert(eMbxTferStatus_Idle == pMbxTfer->eTferStatus);
        if ((pMbxTfer->MbxTferDesc.dwMaxDataLen < pMbxTfer->dwDataLen))
        {
            EC_ERRORMSGC(("IsValidSoeTransferParmsNonBlocking: mismatching data length pMbxTfer->MbxTferDesc.dwMaxDataLen = %d, pMbxTfer->dwDataLen = %d!\n", pMbxTfer->MbxTferDesc.dwMaxDataLen, pMbxTfer->dwDataLen));
            *pdwRetVal = EC_E_INVALIDPARM;
            return EC_FALSE;
        }
        return IsValidSoeTransferParms(byDriveNo, pbyElementFlags, wIDN, pMbxTfer->pbyMbxTferData, pMbxTfer->dwDataLen, &pMbxTfer->dwDataLen, dwTimeout, pdwRetVal);
    } EC_INLINESTOP

    virtual EC_T_DWORD GetProcessDataBits(
                                        EC_T_DWORD              dwInstanceID,
                                        EC_T_BOOL               bOutputData,
                                        EC_T_DWORD              dwBitOffsetPd,
                                        EC_T_BYTE*              pbyDataDst,
                                        EC_T_DWORD              dwBitLengthDst,
                                        EC_T_DWORD              dwTimeout           ) = 0;

    virtual EC_T_DWORD SetProcessData(  EC_T_DWORD              dwInstanceID,
                                        EC_T_BOOL               bOutputData,
                                        EC_T_DWORD              dwOffset,
                                        EC_T_BYTE*              pbyData,
                                        EC_T_DWORD              dwLength,
                                        EC_T_DWORD              dwTimeout           ) = 0;

    virtual EC_T_DWORD SetProcessDataBits( 
                                        EC_T_DWORD              dwInstanceID,
                                        EC_T_BOOL               bOutputData,    
                                        EC_T_DWORD              dwBitOffsetPd,  
                                        EC_T_BYTE*              pbyDataSrc,     
                                        EC_T_DWORD              dwBitLengthSrc, 
                                        EC_T_DWORD              dwTimeout           ) = 0;

    virtual EC_T_DWORD EoeRegisterEndpoint(
                                        EC_T_DWORD              dwInstanceID,
                                        EC_T_CHAR*              szEoEDrvIdent,
                                        EC_T_LINK_DRV_DESC*     pLinkDrvDesc        ) = 0;

    virtual EC_T_DWORD EoeUnregisterEndpoint(
                                        EC_T_DWORD              dwInstanceID,
                                        EC_T_LINK_DRV_DESC*     pLinkDrvDesc        ) = 0;

    virtual EC_T_DWORD ReadSlaveIdentification(
                                        EC_T_DWORD              dwInstanceID,
                                        EC_T_BOOL               bFixedAddressing,
                                        EC_T_WORD               wSlaveAddress,
                                        EC_T_WORD               wAdo,
                                        EC_T_WORD*              pwValue,
                                        EC_T_DWORD              dwTimeout           ) = 0;

    virtual EC_T_DWORD SetSlaveDisabled(EC_T_DWORD              dwInstanceID,
                                        EC_T_BOOL               bFixedAddressing,
                                        EC_T_WORD               wSlaveAddress,
                                        EC_T_BOOL               bDisabled           ) = 0;

    virtual EC_T_DWORD SetSlaveDisconnected(
                                        EC_T_DWORD              dwInstanceID,
                                        EC_T_BOOL               bFixedAddressing,
                                        EC_T_WORD               wSlaveAddress,
                                        EC_T_BOOL               bDisconnected       ) = 0;

    virtual EC_T_DWORD RescueScan(      EC_T_DWORD              dwInstanceID,
                                        EC_T_DWORD              dwTimeout           ) = 0;

    virtual EC_T_DWORD GetMasterInfo(   EC_T_DWORD              dwInstanceID,
                                        EC_T_MASTER_INFO*       pMasterInfo         ) = 0;

    virtual EC_T_DWORD FindProcVarByName(
                                        EC_T_DWORD              dwInstanceID, 
                                        EC_T_CHAR*              szVariableName,
                                        EC_T_PROCESS_VAR_INFO*  pProcessVarInfo, 
                                        EC_T_PROCESS_VAR_INFO_EX* pProcessVarInfoEx,    
                                        EC_T_BOOL               bInputProcessVars   ) = 0;

    virtual EC_T_DWORD ConfigExtend(    EC_T_DWORD              dwInstanceID,
                                        EC_T_BOOL               bResetConfig,
                                        EC_T_DWORD              dwTimeout           ) = 0;

    virtual EC_T_DWORD Start(           EC_T_DWORD              dwInstanceID,
                                        EC_T_DWORD              dwTimeout           ) = 0;

    virtual EC_T_DWORD  Stop(
        EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_DWORD              dwTimeout       /**< [in]   Timeout in milliseconds */
        ) = 0;

    virtual EC_T_DWORD  GetSlaveFixedAddr(
        EC_T_DWORD              dwInstanceID,        /**< [in]  Master Instance ID */
        EC_T_DWORD              dwSlaveId,           /**< [in]  Slave id */
        EC_T_WORD*              pwFixedAddr          /**< [in]  Corresponding fixed address */
        ) = 0;

    virtual EC_T_DWORD  GetSlaveIdAtPosition(
        EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
        EC_T_WORD               wAutoIncAddress     /**< [in]  slave auto increment address (set in the XML configuration file) */
        ) = 0;

    virtual EC_T_BOOL   GetSlaveProp(
        EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_DWORD              dwSlaveId,      /**< [in]  slave id */
        EC_T_SLAVE_PROP*        pSlaveProp      /**< [out] slave properties */
        ) = 0;

    virtual EC_T_DWORD  GetSlaveState(
        EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
        EC_T_DWORD              dwSlaveId,          /**< [in]   slave id */
        EC_T_WORD*              pwCurrDevState,     /**< [out]  current device state */
        EC_T_WORD*              pwReqDevState       /**< [out]  requested device state */
        ) = 0;

    virtual EC_T_DWORD  SetSlaveState(
        EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
        EC_T_DWORD              dwSlaveId,          /**< [in]   slave id */
        EC_T_WORD               wNewReqDevState,    /**< [in]   requested new device state */
        EC_T_DWORD              dwTimeout           /**< [in]   timeout value */
        ) = 0;

    virtual EC_T_DWORD  ReadSlaveRegister(
        EC_T_DWORD              dwInstanceID,       /**< [in]  Master Instance ID */
        EC_T_BOOL               bFixedAddressing,   /**< [in]  EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
        EC_T_WORD               wSlaveAddress,      /**< [in]  Slave Address, fixed or auto increment address depending on bFixedAddressing */
        EC_T_WORD               wRegisterOffset,    /**< [in]  register offset inside the slave memory  */
        EC_T_VOID*              pvData,             /**< [out] pointer to data */
        EC_T_WORD               wLen,               /**< [in]  data length */
        EC_T_DWORD              dwTimeout           /**< [in]  timeout in msec */
        ) = 0;

    virtual EC_T_DWORD  WriteSlaveRegister(
        EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
        EC_T_BOOL               bFixedAddressing,   /**< [in]  EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
        EC_T_WORD               wSlaveAddress,      /**< [in]  Slave Address, fixed or auto increment address depending on bFixedAddressing */
        EC_T_WORD               wRegisterOffset,    /**< [in]  register offset inside the slave memory */
        EC_T_VOID*              pvData,             /**< [in]  pointer to data */
        EC_T_WORD               wLen,               /**< [in]  data length */
        EC_T_DWORD              dwTimeout           /**< [in]  timeout in msec */
        ) = 0;

    virtual EC_T_DWORD  ReadSlaveEEPRom(
        EC_T_DWORD              dwInstanceID,           /**< [in]   Master Instance ID */
        EC_T_BOOL               bFixedAddressing,       /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
        EC_T_WORD               wSlaveAddress,          /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
        EC_T_WORD               wEEPRomStartOffset,     /**< [in]   Address to start EEPRom Read from.*/
        EC_T_WORD*              pwReadData,             /**< [in]   Pointer to WORD array to carry the read data */
        EC_T_DWORD              dwReadLen,              /**< [in]   Sizeof Read Data WORD array (in WORDS) */
        EC_T_DWORD*             pdwNumOutData,          /**< [out]  Pointer to DWORD carrying actually read data after completion */
        EC_T_DWORD              dwTimeout               /**< [in]   Timeout in milliseconds */
        ) = 0;

    virtual EC_T_DWORD  WriteSlaveEEPRom(
        EC_T_DWORD              dwInstanceID,           /**< [in]   Master Instance ID */
        EC_T_BOOL               bFixedAddressing,       /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
        EC_T_WORD               wSlaveAddress,          /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
        EC_T_WORD               wEEPRomStartOffset,     /**< [in]   Address to start EEPRom Write from */
        EC_T_WORD*              pwWriteData,            /**< [in]   Pointer to WORD array carrying the write data */
        EC_T_DWORD              dwWriteLen,             /**< [in]   Sizeof Write Data WORD array (in WORDS) */
        EC_T_DWORD              dwTimeout               /**< [in]   Timeout in milliseconds */
        ) = 0;

    virtual EC_T_DWORD AssignSlaveEEPRom(
        EC_T_DWORD  dwInstanceID                /**< [in]   Master Instance ID */
        , EC_T_BOOL   bFixedAddressing            /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
        , EC_T_WORD   wSlaveAddress               /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
        , EC_T_BOOL   bSlavePDIAccessEnable       /**< [in]   EC_TRUE: assign EEPRom to slave PDI application, EC_FALSE: assign EEPRom to ECat Master */
        , EC_T_BOOL   bForceAssign                /**< [in]   EC_TRUE: force Assignment, only available for ECat Master Assignment, EC_FALSE: don't force */
        , EC_T_DWORD  dwTimeout                   /**< [in]   Timeout in milliseconds */
        ) = 0;

    virtual EC_T_DWORD  TferSingleRawCmd(
        EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
        EC_T_DWORD              dwSlaveId,          /**< [in]  slave id */
        EC_T_BYTE               byCmd,              /**< [in]  ecat command */
        EC_T_DWORD              dwMemoryAddress,    /**< [in]  memory address inside the slave (physical or logical) */
        EC_T_VOID*              pvData,             /**< [in, out]  pointer to data */
        EC_T_WORD               wLen,               /**< [in]  data length */
        EC_T_DWORD              dwTimeout           /**< [in]  timeout in msec */
        ) = 0;
    virtual EC_T_DWORD  QueueRawCmd(
        EC_T_DWORD              dwInstanceID,       /**< [in]   Master Instance ID */
        EC_T_DWORD              dwSlaveId,          /**< [in]  slave id */
        EC_T_WORD               wInvokeId,          /**< [in]  Invoke Id */
        EC_T_BYTE               byCmd,              /**< [in]  ecat command */
        EC_T_DWORD              dwMemoryAddress,    /**< [in]  memory address inside the slave (physical or logical) */
        EC_T_VOID*              pvData,             /**< [in]  pointer to data */
        EC_T_WORD               wLen                /**< [in]  data length */
        ) = 0;


    virtual EC_T_DWORD  GetNumConfiguredSlaves(
        EC_T_DWORD              dwInstanceID    /**< [in]   Master Instance ID */
        ) = 0;
    virtual EC_T_DWORD CoeSdoDownload(
        EC_T_DWORD     dwInstanceID    /* [in]  Master Instance ID */
        , EC_T_DWORD     dwSlaveId       /* [in]  slave ID */
        , EC_T_WORD      wObIndex        /* [in]  SDO index */
        , EC_T_BYTE      byObSubIndex    /* [in]  SDO sub-index */
        , EC_T_BYTE*     pbyData         /* [in]  SDO data */
        , EC_T_DWORD     dwDataLen       /* [in]  length of pbyData buffer */
        , EC_T_DWORD     dwTimeout       /* [in]  timeout in msec, must not be set to EC_NOWAIT */
        , EC_T_DWORD     dwFlags         /* [in]  mailbox transfer flags */
        ) = 0;

    virtual EC_T_DWORD NotifyApp(
        EC_T_DWORD          dwInstanceID    /**< [in]   Master Instance ID */
        , EC_T_DWORD          dwCode          /**< [in]   Notification code */
        , EC_T_NOTIFYPARMS*   pParms          /**< [in]   Notification data */
        ) = 0;

    virtual EC_T_DWORD  CoeGetODList(
        EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_MBXTFER*           pMbxTfer,       /**< [in] mailbox transfer object */
        EC_T_DWORD              dwSlaveId,      /**< [in] slave id to determine the OD list */
        EC_T_COE_ODLIST_TYPE    eListType,      /**< [in] which object types shall be transferred */
        EC_T_DWORD              dwTimeout       /**< [in] Timeout in milliseconds */
        ) = 0;


    virtual EC_T_DWORD  CoeGetObjectDesc(
        EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_MBXTFER*           pMbxTfer,       /**< [in] mailbox transfer object */
        EC_T_DWORD              dwSlaveId,      /**< [in] slave id to download the SDO */
        EC_T_WORD               wObIndex,       /**< [in] object index */
        EC_T_DWORD              dwTimeout       /**< [in] Timeout in milliseconds */
        ) = 0;

    virtual EC_T_DWORD  CoeGetEntryDesc(
        EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_MBXTFER*           pMbxTfer,       /**< [in] mailbox transfer object */
        EC_T_DWORD              dwSlaveId,      /**< [in] slave id to download the SDO */
        EC_T_WORD               wObIndex,       /**< [in] object index */
        EC_T_BYTE               byObSubIndex,   /**< [in] object sub-index */
        EC_T_BYTE               byValueInfo,    /**< [in] value info selection */
        EC_T_DWORD              dwTimeout       /**< [in] Timeout in milliseconds */
        ) = 0;

    virtual EC_T_DWORD AoeWriteReq(
        EC_T_DWORD  dwInstanceID,       /**< [in]   Master Instance ID */
        EC_T_MBXTFER* pMbxTfer,         /**< [in]	Pointer to the corresponding mailbox transfer object. The data to write have to be stored at pMbxTfer->pbyData. */
        EC_T_DWORD  dwSlaveId,          /**< [in]	[in]	EtherCAT slave ID. To determine the slave ID the function GetSlaveId has to be used. */
        EC_T_AOE_NETID* poTargetNetId,  /**< [in]	Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId(). */
        EC_T_WORD   wTargetPort,        /**< [in]	Target port.  */
        EC_T_DWORD  dwIndexGroup,       /**< [in]	AoE write command index group. */
        EC_T_DWORD  dwIndexOffset,      /**< [in]	AoE write command index offset*/
        EC_T_DWORD  dwTimeout           /**< [in]	Timeout in milliseconds. The function will block at most for this time. */
        ) = 0;

    virtual EC_T_DWORD  AoeReadReq(
        EC_T_DWORD      dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_MBXTFER*   pMbxTfer,       /**< [in]	Pointer to the corresponding mailbox transfer object. The data to write have to be stored at pMbxTfer->pbyData. */
        EC_T_DWORD      dwSlaveId,      /**< [in]	EtherCAT slave ID. To determine the slave ID the function GetSlaveId has to be used.*/
        EC_T_AOE_NETID* poTargetNetId,  /**< [in]	Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId(). */
        EC_T_WORD       wTargetPort,    /**< [in]	Target port.  */
        EC_T_DWORD      dwIndexGroup,   /**< [in]	AoE read command index group */
        EC_T_DWORD      dwIndexOffset,  /**< [in]	AoE read command index offset*/
        EC_T_DWORD      dwTimeout       /**< [in]	Timeout in milliseconds. The function will block at most for this time.*/
        ) = 0;

    virtual EC_T_DWORD AoeWrite(
        EC_T_DWORD  dwInstanceID,       /**< [in]   Master Instance ID */
        EC_T_DWORD  dwSlaveId,          /**< [in]	EtherCAT slave ID. To determine the slave ID the function GetSlaveId has to be used. */
        EC_T_AOE_NETID* poTargetNetId,  /**< [in]	Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId(). */
        EC_T_WORD   wTargetPort,        /**< [in]	Target port.  */
        EC_T_DWORD  dwIndexGroup,       /**< [in]	AoE write command index group. */
        EC_T_DWORD  dwIndexOffset,      /**< [in]	AoE write command index offset*/
        EC_T_DWORD  dwDataLength,       /**< [in]	Number of bytes to write to the target device. Must correspond with the size of pbyData.*/
        EC_T_BYTE*  pbyData,            /**< [in]	Data buffer with data that shall be written*/
        EC_T_DWORD* pdwErrorCode,       /**< [out]	AoE response error code. */
        EC_T_DWORD* pdwCmdResult,       /**< [out]	AoE write command result code. */
        EC_T_DWORD  dwTimeout           /**< [in]	Timeout in milliseconds. The function will block at most for this time. */
        ) = 0;

    virtual EC_T_DWORD  AoeRead(
        EC_T_DWORD  dwInstanceID,       /**< [in]   Master Instance ID */
        EC_T_DWORD  dwSlaveId,          /**< [in]	EtherCAT slave ID. To determine the slave ID the function GetSlaveId has to be used. */
        EC_T_AOE_NETID* poTargetNetId,  /**< [in]	Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId().*/
        EC_T_WORD   wTargetPort,        /**< [in]	Target port. */
        EC_T_DWORD  dwIndexGroup,       /**< [in]	AoE read command index group. */
        EC_T_DWORD  dwIndexOffset,      /**< [in]	AoE read command index offset*/
        EC_T_DWORD  dwDataLength,       /**< [in]	Number of bytes to read from the target device. Must correspond with the size of pbyData. */
        EC_T_BYTE*  pbyData,            /**< [in]	Data buffer to store the read data */
        EC_T_DWORD* pdwDataOutLen,      /**< [out]	Number of bytes read from the target device */
        EC_T_DWORD* pdwErrorCode,       /**< [out]	AoE response error code*/
        EC_T_DWORD* pdwCmdResult,       /**< [out]	AoE read command result code */
        EC_T_DWORD  dwTimeout           /**< [in]	Timeout in milliseconds. The function will block at most for this time */
        ) = 0;

    virtual EC_T_DWORD  AoeReadWrite(
        EC_T_DWORD  dwInstanceID,       /**< [in]   Master Instance ID */
        EC_T_DWORD  dwSlaveId,          /**< [in]	EtherCAT slave ID. To determine the slave ID the function GetSlaveId has to be used. */
        EC_T_AOE_NETID* poTargetNetId,  /**< [in]	Target NetID. The Target NetID of a AoE slave device can be retrieved by ecatAoeGetSlaveNetId().*/
        EC_T_WORD   wTargetPort,        /**< [in]	Target port. */
        EC_T_DWORD  dwIndexGroup,       /**< [in]	AoE read command index group. */
        EC_T_DWORD  dwIndexOffset,      /**< [in]	AoE read command index offset*/
        EC_T_DWORD  dwReadDataLen,      /**< [in]	Number of bytes to read from the target device. Must correspond with the size of pbyData. */
        EC_T_DWORD  dwWriteDataLen,     /**< [in]	Number of bytes to read from the target device. Must correspond with the size of pbyData. */
        EC_T_BYTE*  pbyData,            /**< [in]	Data buffer to store the read data */
        EC_T_DWORD* pdwDataOutLen,      /**< [out]	Number of bytes read from the target device */
        EC_T_DWORD* pdwErrorCode,       /**< [out]	AoE response error code*/
        EC_T_DWORD* pdwCmdResult,       /**< [out]	AoE read command result code */
        EC_T_DWORD  dwTimeout           /**< [in]	Timeout in milliseconds. The function will block at most for this time */
        ) = 0;

    virtual EC_T_DWORD  AoeGetSlaveNetId(
        EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_DWORD              dwSlaveId,      /**< [in]	EtherCAT slave ID. To determine the slave ID the function GetSlaveId has to be used. */
        EC_T_AOE_NETID*         poAoeNetId      /**< [out]	AoE NetID of the corresponding slave */
        ) = 0;

    virtual EC_T_DWORD AoeWriteControl(
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
        ) = 0;

    virtual EC_T_DWORD  ConfigureMaster(
        EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_CNF_TYPE           eCnfType,       /**< [in] Enum type of configuration data provided */
        EC_T_PBYTE              pbyCnfData,     /**< [in] Configuration data */
        EC_T_DWORD              dwCnfDataLen    /**< [in] Length of configuration data in byte */
        ) = 0;

    virtual EC_T_DWORD  SetMasterState(
        EC_T_DWORD              dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_DWORD              dwTimeout,      /**< [in] Timeout in milliseconds */
        EC_T_STATE              eReqState       /**< [in] Requested System state  */
        ) = 0;


    virtual EC_T_DWORD GetProcessData(
        EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_BOOL   bOutputData,    /**< [in]   EC_TRUE if requested data is output data */
        EC_T_DWORD  dwOffset,       /**< [in]   Process data offset */
        EC_T_BYTE*  pbyData,        /**< [in]   Data buffer */
        EC_T_DWORD  dwLength,       /**< [in]   Buffer Length */
        EC_T_DWORD  dwTimeout       /**< [in]   Timeout value */
        ) = 0;

    virtual EC_T_DWORD GetNumHelper(
        EC_T_DWORD dwCommandCode,
        EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
        ) = 0;

    virtual EC_T_DWORD GetSlaveInfo(
        EC_T_DWORD              dwInstanceID    /**< [in]   Master Instance ID */
        , EC_T_BOOL               bFixedAddress   /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
        , EC_T_WORD               wSlaveAddress   /**< [in]   Slave address */
        , EC_T_GET_SLAVE_INFO*    pGetSlaveInfo   /**< [out]  Slave information */
        ) = 0;

    virtual EC_T_DWORD GetCfgSlaveInfo(
        EC_T_DWORD              dwInstanceID    /**< [in]   Master Instance ID */
        , EC_T_BOOL               bStationAddress /**< [in]   EC_TRUE: Use station address, EC_FALSE: use auto increment address */
        , EC_T_WORD               wSlaveAddress   /**< [in]   Slave address */
        , EC_T_CFG_SLAVE_INFO*    pSlaveInfo      /**< [out]  Slave information */
        ) = 0;

    virtual EC_T_DWORD GetBusSlaveInfo(
        EC_T_DWORD              dwInstanceID    /**< [in]   Master Instance ID */
        , EC_T_BOOL               bStationAddress /**< [in]   EC_TRUE: Use station address, EC_FALSE: use auto increment address */
        , EC_T_WORD               wSlaveAddress   /**< [in]   Slave address */
        , EC_T_BUS_SLAVE_INFO*    pSlaveInfo      /**< [out]  Slave information */
        ) = 0;

    virtual EC_T_DWORD GetSlaveProcVarInfoNumOf(
        EC_T_BOOL               bInput,                    /**< [in]   EC_TRUE: ecatGetSlaveInpVarInfoNumOf; EC_FALSE: ecatGetSlaveOutpVarInfoNumOf */
        EC_T_DWORD              dwInstanceID,              /**< [in]   Master Instance ID */
        EC_T_BOOL               bFixedAddress,             /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
        EC_T_WORD               wSlaveAddress,             /**< [in]   Slave address */
        EC_T_WORD*              pwSlaveProcVarInfoNumOf    /**< [out]  Number of found process variable entries */
        ) = 0;

    virtual EC_T_DWORD GetSlaveProcVarInfo(
        EC_T_BOOL               bInput,                     /**< [in]   EC_TRUE: ecatGetSlaveInpVarInfo; EC_FALSE: ecatGetSlaveOutpVarInfo */
        EC_T_DWORD              dwInstanceID,               /**< [in]   Master Instance ID */
        EC_T_BOOL               bFixedAddress,              /**< [in]   EC_TRUE: Use fixed address, EC_FALSE: use AutoInc address */
        EC_T_WORD               wSlaveAddress,              /**< [in]   Slave address */
        EC_T_WORD               wNumOfVarsToRead,           /**< [in]   Number process variable entries that have been stored in pSlaveProcVarInfoEntries */
        EC_T_PROCESS_VAR_INFO*  pSlaveProcVarInfoEntries,   /**< [out]  The read process variable information entries */
        EC_T_PROCESS_VAR_INFO_EX*  pSlaveProcVarInfoEntriesEx,   /**< [out]  The read process variable information entries */
        EC_T_WORD*              pwReadEntries               /**< [out]  The number of read process variable information entries */
        ) = 0;

    virtual EC_T_DWORD  SoeRead(
        EC_T_DWORD  dwInstanceID      /**< [in]  Master Instance ID */
        , EC_T_DWORD  dwSlaveId         /**< [in]  ethercat slave ID */
        , EC_T_BYTE   byDriveNo         /**< [in]  number of drive in slave */
        , EC_T_BYTE*  pbyElementFlags   /**< [in/out]  which element of object is read */
        , EC_T_WORD   wIDN              /**< [in]  IDN of the object */
        , EC_T_BYTE*  pbyData           /**< [out] IDN data */
        , EC_T_DWORD  dwDataLen         /**< [in]  length of pbyData buffer */
        , EC_T_DWORD* pdwOutDataLen     /**< [out] IDN data length (number of received bytes) */
        , EC_T_DWORD  dwTimeout         /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
        ) = 0;

    virtual EC_T_DWORD  SoeWrite(
        EC_T_DWORD    dwInstanceID      /**< [in]  Master Instance ID */
        , EC_T_DWORD    dwSlaveId         /**< [in]  ethercat slave ID */
        , EC_T_BYTE     byDriveNo         /**< [in]  number of drive in slave */
        , EC_T_BYTE*    pbyElementFlags   /**< [in]  which element of object is written */
        , EC_T_WORD     wIDN              /**< [in]  IDN of the object */
        , EC_T_BYTE*    pbyData           /**< [in]  data of the element to be written */
        , EC_T_DWORD    dwDataLen         /**< [in]  length of pbyData buffer */
        , EC_T_DWORD    dwTimeout         /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
        ) = 0;

    virtual EC_T_DWORD  SoeAbortProcCmd(
        EC_T_DWORD    dwInstanceID      /**< [in]  Master Instance ID */
        , EC_T_DWORD    dwSlaveId         /**< [in]  ethercat slave ID */
        , EC_T_BYTE     byDriveNo         /**< [in]  number of drive in slave */
        , EC_T_BYTE*    pbyElementFlags   /**< [in]  which element of object is written */
        , EC_T_WORD     wIDN              /**< [in]  IDN of the object */
        , EC_T_DWORD    dwTimeout         /**< [in]  timeout in msec, must not be set to EC_NOWAIT */
        ) = 0;

    virtual EC_T_DWORD IsSlavePresent(
        EC_T_DWORD      dwInstanceID    /**< [in]   Master Instance ID */
        , EC_T_DWORD      dwSlaveId       /**< [in]   Slave Id */
        , EC_T_BOOL*      pbPresence      /**< [out]  Presence state of slave on bus */
        ) = 0;

    virtual EC_T_DWORD  ReloadSlaveEEPRom(
        EC_T_DWORD              dwInstanceID,           /**< [in]   Master Instance ID */
        EC_T_BOOL               bFixedAddressing,       /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
        EC_T_WORD               wSlaveAddress,          /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
        EC_T_DWORD              dwTimeout               /**< [in]   Timeout in milliseconds */
        ) = 0;

    virtual EC_T_DWORD  ResetSlaveController(
        EC_T_DWORD              dwInstanceID,           /**< [in]   Master Instance ID */
        EC_T_BOOL               bFixedAddressing,       /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */
        EC_T_WORD               wSlaveAddress,          /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
        EC_T_DWORD              dwTimeout               /**< [in]   Timeout in milliseconds */
        ) = 0;

    virtual EC_T_DWORD GetVersion(EC_T_DWORD dwInstanceID, EC_T_DWORD *pdwVersion) = 0;

    virtual EC_T_DWORD GetSrcMacAddress(
        EC_T_DWORD          dwInstanceID,           /**< [in]   Master Instance ID */
        ETHERNET_ADDRESS*   pMacSrc
        ) = 0;

    virtual EC_T_DWORD ForceProcessDataBits(
        EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_DWORD	dwClientId,
        EC_T_BOOL   bOutputData,
        EC_T_DWORD  dwBitOffsetPd,
        EC_T_WORD   wBitLength,
        EC_T_BYTE*  pbyData,
        EC_T_DWORD  dwTimeout) = 0;

    virtual EC_T_DWORD ReleaseProcessDataBits(
        EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_DWORD	dwClientId,
        EC_T_BOOL   bOutputData,
        EC_T_DWORD  dwBitOffsetPd,
        EC_T_WORD	wBitLength,
        EC_T_DWORD  dwTimeout
        ) = 0;

    virtual EC_T_DWORD ReleaseAllProcessDataBits(
        EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_DWORD	dwClientId,
        EC_T_DWORD  dwTimeout
        ) = 0;

    virtual EC_T_DWORD SlaveSerializeMbxTfers(
        EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_DWORD  dwSlaveID       /**< [in]   Slave ID */
        ) = 0;

    virtual EC_T_DWORD SlaveParallelMbxTfers(
        EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_DWORD  dwSlaveID       /**< [in]   Slave ID */
        ) = 0;

    virtual EC_T_DWORD SetMbxProtocolsSerialize(
        EC_T_DWORD  dwInstanceID,   /**< [in]   Master Instance ID */
        EC_T_DWORD  dwSlaveID,      /**< [in]   Slave ID */
        EC_T_BOOL   bSerialize
        ) = 0;

    virtual EC_T_DWORD LinkOpen(EC_T_VOID* pvLinkParms, EC_T_RECEIVEFRAMECALLBACK pfReceiveFrameCallback,
        EC_T_LINK_NOTIFY pfLinkNotifyCallback, EC_T_VOID* pvContext, EC_T_VOID** ppvInstance) = 0;
    virtual EC_T_DWORD LinkClose(EC_T_VOID* pvInstance) = 0;
    virtual EC_T_DWORD LinkSendAndFreeFrame(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc) = 0;
    virtual EC_T_DWORD LinkRecvFrame(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc) = 0;
};

#endif /* INC_ECMASTER_INTERFACE_BASE */
