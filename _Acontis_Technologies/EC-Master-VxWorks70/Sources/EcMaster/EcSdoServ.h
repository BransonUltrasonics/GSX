/*-----------------------------------------------------------------------------
 * EcSdoServ.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Willig, Andreas
 * Description              description of file
 *---------------------------------------------------------------------------*/

#ifndef INC_ECSDOSERV
#define INC_ECSDOSERV 1

/*-INCLUDES------------------------------------------------------------------*/
#include <EcObjDef.h>
#include <EcMaster.h>

/*-FORWARD DECLARATIONS------------------------------------------------------*/
class CEcBusSlave;
class CEcDevice;
class CEcMaster;
class CEcSdoServ;
class CEcSlave;

/*-TYPEDEFS------------------------------------------------------------------*/
/* -NOT PACKED STRUCTURES--------------------------------------------------- */
typedef EC_T_DWORD (*EC_PF_ACCESSFUNC)( CEcSdoServ* pInst,
                                        EC_T_WORD   wIndex, 
                                        EC_T_BYTE   bySubindex, 
                                        EC_T_DWORD  dwSize, 
                                        EC_T_WORD*  pwData, 
                                        EC_T_BOOL   bCompleteAccess             
                                      );

/* -PACKED STRUCTURES------------------------------------------------------- */
#include EC_PACKED_INCLUDESTART(1)

typedef struct _EC_T_SDOINFOOBJDESC
{
    EC_T_WORD   wDataType;          /* refer to "Extended Data Type Area" in ETG.1000.6 */
    EC_T_WORD   wObjFlags;
    /*EC_T_CHAR   szName[];           / * rest of mailbox data * / */
} EC_PACKED(1) EC_T_SDOINFOOBJDESC;

/* SDO Information / Entry Description:*/
typedef struct _EC_T_SDOINFOENTRYDESC
{
    EC_T_WORD   wDataType;          /* refer to data type index */
    EC_T_WORD   wBitLength;         
    EC_T_WORD   wObjAccess;         /* Bit 0: Read Access in Pre-Op
                                     * Bit 1: Read Access in Safe-Op
                                     * Bit 2: Read Access in Op
                                     * Bit 3: Write Access in Pre-Op
                                     * Bit 4: Write Access in Safe-Op
                                     * Bit 5: Write Access in Op
                                     * Bit 6: mapable in RxPDO
                                     * Bit 7: mapable in TxPDO
                                     * Bit 8: entry will be included in backup
                                     * Bit 9: entry will be included in settings
                                     */
/*
    EC_T_WORD   wUnitType;          / * optional if bit3 of valueInfo * /
    EC_T_BYTE   abyDefaultValue[];  / * optional if bit4 of valueInfo * /
    EC_T_BYTE   abyMinValue[];      / * optional if bit5 of valueInfo * /
    EC_T_BYTE   abyMaxValue[];      / * optional if bit6 of valueInfo * /
    EC_T_CHAR   szDesc[];           / * rest of mailbox data * /
*/
} EC_PACKED(1) EC_T_SDOINFOENTRYDESC;

/* Structure is used to store OBJ3XXX */
struct EC_T_OBJ3XXX_ENTRY : public EC_T_OBJ3XXX
{
    /* struct length should by 4 bytes aligned */
    EC_T_BYTE    abyReserved[3];
} /* no pack */ ;

struct EC_T_OBJ2004_ENTRY : public EC_T_OBJ2004
{
    /* struct length should by 4 bytes aligned */
    EC_T_BYTE               byReserved[3];
} /* no pack */ ;


struct EC_T_OBJ9XXX_ENTRY : public EC_T_OBJ9XXX
{
    /* struct length should by 4 bytes aligned */
    EC_T_BYTE    abyReserved[2];
} /* no pack */ ;

/* This structure handles parameter information passed to diagnosis message create function */
typedef struct 
{
   EC_T_WORD        wParamFlags;                     /* Parameter flags */
   EC_T_VOID*       pvAddress;                       /* Address to Parameter pointer */
} /* no pack */ EC_T_DIAGMSGPARAMINFO;

/* Structure is used to handle allocated message memory */
typedef struct 
{
   EC_T_WORD        wMessageLength;         /* Message length */
   EC_T_OBJ10F3_DIAGMSG *pMessage;          /* Message buffer */
} /* no pack */ EC_T_DIAGMSGHANDLE;


/***************************************************************************************************/
/**
\brief  Object dictionary structure.
*/
typedef struct _EC_T_OBJECTDICT
{
    EC_T_WORD               wIndex;
    EC_T_SDOINFOOBJDESC     oObjDesc;
    EC_T_SDOINFOENTRYDESC*  pEntryDesc;
    EC_T_BYTE*              pName;
    EC_T_VOID*              pVarPtr;
    
    EC_PF_ACCESSFUNC        pfRead;
    EC_PF_ACCESSFUNC        pfWrite;
} EC_T_OBJECTDICT;

#include EC_PACKED_INCLUDESTOP

/*-CLASS---------------------------------------------------------------------*/
class CEcSdoServ
{
public:
                        CEcSdoServ(                 CEcMaster*              pMaster
                                                   ,CEcDevice*              pDevice
                                                   ,EC_T_WORD               wMaxSlaveObjs
                                                    /*IEcDevice*              pDevice*/
                                                                                                        );
                        ~CEcSdoServ(                EC_T_VOID                                           );
                                                                            
    EC_T_DWORD          SDOUpload(                  PEcMailboxCmd           pCmd                        );
                                                                            
    EC_T_DWORD          SDODownload(                PEcMailboxCmd           pCmd                        );
                                                                            
    EC_T_DWORD          GetObjDictList(             PEcMailboxCmd           pCmd                        );
    EC_T_DWORD          GetObjDescription(                                  
                                                    PEcMailboxCmd           pCmd                        );
                                                                            
    EC_T_DWORD          GetEntryDescription(        PEcMailboxCmd           pCmd                        );
                                                                            
    EC_T_DWORD          GetEntryBitLength(          EC_T_WORD               wIndex,
                                                    EC_T_BYTE               bySubindex,
                                                    EC_T_WORD*              pwBitLength                 );

    static                                                                  
    EC_T_DWORD          ReadWrapper(                CEcSdoServ*             pInst,
                                                    EC_T_WORD               wIndex,
                                                    EC_T_BYTE               bySubindex,
                                                    EC_T_DWORD              dwObjSize,
                                                    EC_T_WORD*              pwData,
                                                    EC_T_BOOL               bCompleteAccess             )
                            {
                                return pInst->ActiveRead(wIndex, bySubindex, dwObjSize, pwData, bCompleteAccess);
                            }
    static                                                                  
    EC_T_DWORD          WriteWrapper(               CEcSdoServ*             pInst,
                                                    EC_T_WORD               wIndex,
                                                    EC_T_BYTE               bySubindex,
                                                    EC_T_DWORD              dwDataSize,
                                                    EC_T_WORD*              pwData,
                                                    EC_T_BOOL               bCompleteAccess             )
                            {
                                return pInst->ActiveWrite(wIndex, bySubindex, dwDataSize, pwData, bCompleteAccess);
                            }

    /* Workers, accessed by Master */
    EC_T_VOID           EntrySetMasterState_State(  EC_T_DWORD              dwMasterState               );
    EC_T_VOID           EntrySet_MastInReqState(    EC_T_BOOL               bInReqState                 );
    EC_T_VOID           EntrySet_BusScanMatch(      EC_T_BOOL               bMatch                      );
    EC_T_VOID           EntrySet_SetDCEnabled(      EC_T_BOOL               bEnabled                    );
    EC_T_VOID           EntrySet_SetDCInSync(       EC_T_BOOL               bDCInSync                   );
    EC_T_VOID           EntrySet_SetDCMInSync(      EC_T_BOOL               bDCMInSync                  );
    EC_T_VOID           EntrySet_SetDCBusy(         EC_T_BOOL               bDCBusy                     );
    EC_T_VOID           EntrySet_SetLinkUp(         EC_T_BOOL               bLinkUp                     );
    
    EC_T_DWORD          SetLinkLayerParms(          EC_T_LINK_PARMS*        pLinkParms                  );
    EC_T_VOID           SetLinkLayerPolling(        EC_T_BOOL               bActive                     );
    EC_T_VOID           SetLinkLayerAllocSupport(   EC_T_BOOL               bActive                     );

    EC_T_VOID           SetDCSlaveDevLimit(         EC_T_DWORD              dwLimitNsec                 );
    EC_T_VOID           SetMacAddress(              EC_T_BOOL               bRedIf,
                                                    EC_T_BYTE               byMac[6]                    );

    EC_T_VOID           SetCfgMacAddress(           EC_T_BOOL               bSrc,
                                                    EC_T_BYTE               byMac[6]                    );

    EC_T_VOID           CleanSlaves(                EC_T_WORD               wNumOfSlavesToClean         );

    EC_T_VOID           AddSlave(                   EC_T_DWORD              dwSlaveId,
                                                    EC_T_DWORD              dwVendorId,
                                                    EC_T_DWORD              dwProdCode,
                                                    EC_T_DWORD              dwRevisionNo,
                                                    EC_T_DWORD              dwSerialNo,
                                                    EC_T_WORD               wMbxSupportedProtocols,
                                                                            
                                                    EC_T_WORD               wAutoIncAddr,
                                                    EC_T_WORD               wFixedAddr,
                                                    EC_T_WORD               wAliasAddr,
                                                                            
                                                    EC_T_WORD               wPortState,
                                                    EC_T_BOOL               bDCSupport,
                                                    EC_T_BOOL               bDC64Support,
                                                                            
                                                    EC_T_DWORD              dwSbErrorCode,

                                                    EC_T_BYTE               byFmmusSupported,
                                                    EC_T_BYTE               bySyncManagersSupd,
                                                    EC_T_BYTE               byRamSizeKb, 
                                                    EC_T_BYTE               byPortDescriptor,
                                                    EC_T_BYTE               byESCType,
                                                    CEcSlave*               pCfgSlave                   );
    EC_T_VOID           AddConfiguredSlave(         EC_T_DWORD              dwSlaveId,
                                                    CEcSlave*               pCfgSlave                   );
    EC_T_VOID           AddConnectedSlave(          CEcBusSlave*            pCfgBusSlave                );


    EC_T_VOID           SetSlaveError(              EC_T_DWORD              dwSDOSlaveIdx, 
                                                    EC_T_DWORD              dwALStatus,
                                                    EC_T_DWORD              dwStatusCode                );
    EC_T_VOID           UpdateSlaveState(           EC_T_DWORD              dwSDOSlaveIdx,
                                                    EC_T_WORD               wALStatus                   );
    EC_T_VOID           UpdateReqSlaveState(        EC_T_DWORD              dwSDOSlaveIdx,
                                                    EC_T_STATE              eReqState                   );
    EC_T_VOID           UpdateErrorCounter(         EC_T_DWORD              dwSDOSlaveIdx,
                                                    EC_T_BYTE*              pbyDataErrReg               );
    EC_T_VOID           SetSlaveAmount(             EC_T_DWORD              dwSBSlaves,
                                                    EC_T_DWORD              dwSBDCSlaves,
                                                    EC_T_DWORD              dwCfgSlaves,
                                                    EC_T_DWORD              dwCfgMbxSlaves              );
#ifdef INCLUDE_RED_DEVICE
    EC_T_VOID           UpdateRedundancy(           EC_T_BYTE               byRedEnabled,
                                                    EC_T_WORD               wNumOfMainSlaves,
                                                    EC_T_WORD               wNumOfRedSlaves,
                                                    EC_T_BYTE               byLineBreak                 );
#endif
    EC_T_VOID           AddNotification(            EC_T_DWORD              dwCode                      );

    EC_T_VOID           ClearNotifications(                                                             );

    EC_T_DWORD*         GetTXFrameCrtPtr(           EC_T_VOID                                           )
                        { return (EC_T_DWORD*)(((EC_T_BYTE*)(&m_oBusDiagnosisObject))+OBJ2002_TXFRM_OFFSET); }
                        /*{ return &(m_oBusDiagnosisObject.dwTXFrames); }*/
                            
    EC_T_DWORD*         GetRXFrameCrtPtr(           EC_T_VOID                                           )
                        { return (EC_T_DWORD*)(((EC_T_BYTE*)(&m_oBusDiagnosisObject))+OBJ2002_RXFRM_OFFSET); }
                        /*{ return &(m_oBusDiagnosisObject.dwRXFrames); }*/
    EC_T_DWORD*         GetLostFrameCrtPtr(         EC_T_VOID                                           )
                        { return (EC_T_DWORD*)(((EC_T_BYTE*)(&m_oBusDiagnosisObject))+OBJ2002_LOSFRM_OFFSET); }
                        /*{ return &(m_oBusDiagnosisObject.dwLostFrames); }*/
    EC_T_DWORD*         GetCycFrameCrtPtr(          EC_T_VOID                                           )
                        { return (EC_T_DWORD*)(((EC_T_BYTE*)(&m_oBusDiagnosisObject))+OBJ2002_CYCFRM_OFFSET); }
                        /*{ return &(m_oBusDiagnosisObject.dwCyclicFrames); }*/
    EC_T_DWORD*         GetCycDgramCrtPtr(          EC_T_VOID                                           )
                        { return (EC_T_DWORD*)(((EC_T_BYTE*)(&m_oBusDiagnosisObject))+OBJ2002_CYCDGR_OFFSET); }
                        /*{ return &(m_oBusDiagnosisObject.dwCyclicDatagrams); }*/
    EC_T_DWORD*         GetAcycFrameCrtPtr(         EC_T_VOID                                           )
                        { return (EC_T_DWORD*)(((EC_T_BYTE*)(&m_oBusDiagnosisObject))+OBJ2002_ACYCFRM_OFFSET); }
                        /*{ return &(m_oBusDiagnosisObject.dwAcyclicFrames); }*/
    EC_T_DWORD*         GetAcycDgramCrtPtr(         EC_T_VOID                                           )
                        { return (EC_T_DWORD*)(((EC_T_BYTE*)(&m_oBusDiagnosisObject))+OBJ2002_ACYCDGR_OFFSET); }
                        /*{ return &(m_oBusDiagnosisObject.dwAcyclicDatagrams); }*/
    EC_T_DWORD          GetBusDiagnosisClearCounters(EC_T_VOID) { return m_oBusDiagnosisObject.dwClearCounters; }

    EC_T_VOID           SetConfigCRC32ChkSum(       EC_T_DWORD              dwCRC32Sum                  );
    
    EC_T_VOID           AddHistoryNotification(     EC_T_DWORD              dwCode,
                                                    EC_T_NOTIFICATION_DATA* pNotificationData           );
    EC_T_VOID           Diag_CreateNewMessage(      EC_T_DWORD              dwDiagCode,
                                                    EC_T_BYTE               byType, 
                                                    EC_T_WORD               wTextID,
                                                    EC_T_BYTE               byNumParam, 
                                                    EC_T_DIAGMSGPARAMINFO*  pParam);

    EC_T_VOID           ClearBusDiagnosisCounters(  EC_T_DWORD dwClear                                  );

    EC_T_DWORD          InitObjectDir(              EC_T_WORD               wMaxSlavesObjs              );
    EC_T_VOID           DeInitObjectDir(                                                                );

    EC_T_DWORD          Read(                       EC_T_WORD               wIndex,
                                                    EC_T_BYTE               bySubindex,
                                                    EC_T_DWORD              dwObjSize,
                                                    EC_T_OBJECTDICT*        pObjEntry,
                                                    EC_T_WORD*              pwData,
                                                    EC_T_BOOL               bCompleteAccess             );
                                                                            
    EC_T_DWORD          Write(                      EC_T_WORD               wIndex,
                                                    EC_T_BYTE               bySubindex,
                                                    EC_T_DWORD              dwDataSize,
                                                    EC_T_OBJECTDICT*        pObjEntry,
                                                    EC_T_WORD*              pwData,
                                                    EC_T_BOOL               bCompleteAccess,
                                                    EC_T_BOOL               bForce                      );

    struct _EC_T_MEMORY_LOGGER* GetMemLog() { return m_pMaster->GetMemLog(); }
                                                                            
protected:              
    EC_T_DWORD          ActiveRead(                 EC_T_WORD               wIndex, 
                                                    EC_T_BYTE               bySubindex, 
                                                    EC_T_DWORD              dwSize, 
                                                    EC_T_WORD*              pwData, 
                                                    EC_T_BOOL               bCompleteAccess             );
    EC_T_DWORD          ActiveWrite(                EC_T_WORD               wIndex, 
                                                    EC_T_BYTE               bySubindex, 
                                                    EC_T_DWORD              dwSize, 
                                                    EC_T_WORD*              pwData, 
                                                    EC_T_BOOL               bCompleteAccess             );
                        
                        
    EC_T_WORD           GetNoOfObjects(             EC_T_BYTE               byListType                  );
                                        
    EC_T_SDOINFOENTRYDESC*
                        GetEntryDesc(               EC_T_OBJECTDICT*        pObjEntry,
                                                    EC_T_BYTE               bySubindex                  );
    EC_T_DWORD          GetObjectList(              EC_T_WORD               wListType,
                                                    EC_T_WORD*              pwIndex,
                                                    EC_T_DWORD              dwSize,
                                                    EC_T_WORD*              pwData                      );
                                                                            
    EC_T_OBJECTDICT*    GetObjectHandle(            EC_T_WORD               wIndex                      );
                                                                            
    EC_T_WORD           GetDesc(                    EC_T_WORD               wIndex,
                                                    EC_T_WORD               bySubindex,
                                                    EC_T_OBJECTDICT*        pObjEntry,
                                                    EC_T_WORD*              pwData                      );
    EC_T_VOID           CopyNumberToString(         EC_T_BYTE*              pbyStr,
                                                    EC_T_BYTE               byNumber                    );
                                                                            
    EC_T_WORD           GetObjectLength(            EC_T_WORD               wIndex,
                                                    EC_T_BYTE               bySubindex,
                                                    EC_T_OBJECTDICT*        pObjEntry,
                                                    EC_T_BOOL               bCompleteAccess             );
    EC_T_DWORD          GetEntryOffset(             EC_T_BYTE               bySubindex, 
                                                    EC_T_OBJECTDICT*        pObjEntry                   );

    static  
    EC_T_DWORD          ReplaceAllocatedString(     CEcSdoServ* pInst,
                                                    EC_T_WORD   wIndex, 
                                                    EC_T_BYTE   bySubindex, 
                                                    EC_T_DWORD  dwSize, 
                                                    EC_T_WORD*  pwData, 
                                                    EC_T_BOOL   bCompleteAccess                         );

    EC_T_DWORD          ClearMessageHistory(        EC_T_VOID                                           );
    EC_T_DWORD          Read0x10F3(                 EC_T_BYTE bySubindex, EC_T_DWORD dwSize, EC_T_BYTE* pData, EC_T_BOOL bCompleteAccess);
    EC_T_DWORD          Read0x2004(                 EC_T_BYTE bySubindex, EC_T_DWORD dwSize, EC_T_BYTE* pData, EC_T_BOOL bCompleteAccess);
#if (defined INCLUDE_MAILBOX_STATISTICS)
    EC_T_DWORD          Read0x2006(                 EC_T_BYTE bySubindex, EC_T_DWORD dwSize, EC_T_BYTE* pData, EC_T_BOOL bCompleteAccess);
#endif /* INCLUDE_MAILBOX_STATISTICS */

    EC_T_VOID           FreeEntry0x1xxxDesc(EC_T_OBJECTDICT* pEntry);
    EC_T_VOID           FreeEntry0x1xxxVarPtr(EC_T_OBJECTDICT* pEntry);
    EC_T_VOID           FreeEntries0x1XXX();

private:
    CEcMaster*          m_pMaster;
    /*IEcDevice*          m_pDevice;*/
    CEcDevice*          m_pDevice;

    EC_T_WORD           m_wMaxSlavesObjs;
    
    EC_T_WORD           m_wNumAllocObjectDict;      /* number of allocated objects in m_pObjectDict */

    EC_T_OBJECTDICT*    m_pObjectDict;
    EC_T_OBJECTDICT*    m_pSdoCurInfoObjEntry;
    
    EC_T_OBJECTDICT*    m_pEntry0x1008;             /* base ptr to object  0x1008 */
    EC_T_OBJECTDICT*    m_pEntry0x1009;             /* base ptr to object  0x1009 */
    EC_T_OBJECTDICT*    m_pEntry0x100A;             /* base ptr to object  0x100A */
    EC_T_OBJECTDICT*    m_pEntry0x10F3;             /* base ptr to object  0x10F3 */
    EC_T_OBJECTDICT*    m_pEntry0x2004;             /* base ptr to object  0x2004 */
    EC_T_OBJECTDICT*    m_pObject3000;              /* base ptr to objects 0x3000ff. */
    EC_T_OBJECTDICT*    m_pObject8000;              /* base ptr to objects 0x8000ff. */
    EC_T_OBJECTDICT*    m_pObject9000;              /* base ptr to objects 0x9000ff. */
    EC_T_OBJECTDICT*    m_pObjectA000;              /* base ptr to objects 0xA000ff. */
    EC_T_OBJECTDICT*    m_pObjectF000;              /* base ptr to objects 0xF000ff. */

    EC_T_OBJ10F3        m_oHistoryObject;
    EC_T_DIAGMSGHANDLE  m_aDiagMessages[MAX_DIAG_MSG];
    EC_T_BYTE           m_byLastDiagMsgRead;        /* Local Index to store the last read message */
    EC_T_DWORD          m_dwMasterStateChng;

    EC_T_DWORD          m_dwMasterStateSummary;     /* Object 0x2001  Master State Summary */
    EC_T_OBJ2002        m_oBusDiagnosisObject;      /* Object 0x2002  Bus Diagnosis Object */
#ifdef INCLUDE_RED_DEVICE
    EC_T_OBJ2003        m_oRedundancyObject;        /* Object 0x2003  Redundancy Diagnosis Object */
#endif
    EC_T_OBJ2004_ENTRY  m_oNotifyCounterObject;
    EC_T_OBJ2004_NOTIFYMSG m_aNotifyMessages[MAX_NOTIFICATIONS];
    EC_T_OBJ2005        m_oMacAddressObject;
    
    EC_T_OBJ2020        m_oMasterInitParmObject;
    EC_T_DWORD          m_dwDCDeviationLimit;
    EC_T_BOOL           m_bDCInSync;                /* DC slaves are in sync */
    EC_T_BOOL           m_bDCMInSync;               /* Master and DC ref.clock is in sync */
    EC_T_UINT64         m_qwSystemTime;
    EC_T_DWORD          m_dwPrevMsecCounter;
    EC_T_UINT64         m_qwMailboxStatisticsClearCounters;

public:
    EC_T_WORD           m_wAlStatusBrdValue;
    EC_T_BOOL           m_bIsAlStatusWkcOk;

    EC_T_OBJ2102        m_oDcmBusShiftObject;
    EC_T_OBJ2200        m_oBusLoadBaseObject;
    EC_T_OBJ3XXX_ENTRY* m_apSlaveObjects;
    CEcSlave**          m_apMasterSlave;            /* Master OD 0x3000 Slave Array */

    /* Modular Device Profiles: EtherCAT Master */
    EC_T_OBJ8XXX*       m_apCfgSlaveObjects;
    EC_T_OBJ9XXX_ENTRY* m_apConSlaveObjects;
    EC_T_OBJAXXX*       m_apDiagSlaveObjects;
    EC_T_OBJF000        m_oModularDeviceProfileObject;
    EC_T_OBJF002        m_oScanCmdObject;
    EC_T_OBJF02X*       m_apCfgAddressListObjects;
    EC_T_OBJF04X*       m_apConAddressListObjects;
};

#endif /* INC_ECSDOSERV */ 

/*-END OF SOURCE FILE--------------------------------------------------------*/
