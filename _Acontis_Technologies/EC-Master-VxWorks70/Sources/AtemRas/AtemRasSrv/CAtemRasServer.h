/*-----------------------------------------------------------------------------
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 *---------------------------------------------------------------------------*/

#ifndef INC_CATEMRASSERVER
#define INC_CATEMRASSERVER    1

/*-INCLUDES------------------------------------------------------------------*/
#include "AtEmRasType.h"
#include "EcSocket.h"
#include "AtEmRasSrv.h"
#include "CAtemRasCmdQueue.h"
#include "EcLock.h"
#include "CAtEmMasterStore.h"
#include "EcThread.h"
#include "EcCommon.h"

/*-DEFINES-------------------------------------------------------------------*/

#define  SERVER_STARTUPDELAY_DEFAULT    5000
#define  SERVER_SHUTDOWNDELAY_DEFAULT   5000

#if (defined UNDER_RTSS) && (defined _AMD64_)
#define  MASTER_STACK_SIZE_DEFAULT      0x8000
#define  CLIENT_STACK_SIZE_DEFAULT      0x8000
#else
#define  MASTER_STACK_SIZE_DEFAULT      0x4000
#define  CLIENT_STACK_SIZE_DEFAULT      0x4000
#endif
#define  DEFAULT_RX_BUFF_SIZE           (8*1024)

#define  THREADNAME_MAX_LEN             32

/*-TYPEDEFS------------------------------------------------------------------*/
typedef enum _ATEMRAS_T_CONNECTIONSTATE
{
    emrasconstate_unknown   = 0x00,
    emrasconstate_init      = 0x01,
    emrasconstate_idle      = 0x02,
    emrasconstate_cmdproc   = 0x04,
    emrasconstate_wdidle    = 0x08,
    emrasconstate_wdexpired = 0x10,

    /* internally send/ack disconnect
       internally deny new API calls
       externally "Terminate()" to stop threads */
    emrasconstate_disconnect= 0x20,

    /* internally deny new API calls
       externally delete object
       threads are stopped */
    emrasconstate_terminated= 0x40,

    /* Borland C++ datatype alignment correction */
    emrasconstate_BCppDummy = 0xFFFFFFFF
} ATEMRAS_T_CONNECTIONSTATE;

typedef struct _ATEMRAS_T_BUFFER_DESC
{
    EC_T_BYTE*  pbyData;
    EC_T_DWORD  dwDataLen;
    EC_T_DWORD  dwMaxLen;
} ATEMRAS_T_BUFFER_DESC;

/*-CLASS---------------------------------------------------------------------*/

class CAtemRasServer;

class CAtemRasSrvClient
{
public:
    CAtemRasSrvClient(CAtemRasServer*    pServer, 
                     CEcSocket*         pSocket, 
                     EC_T_DWORD         dwCookie, 
                     EC_T_DWORD         dwSilenceLimit,
                     EC_T_DWORD         dwCycleTime,
                     EC_T_DWORD         dwThreadPrio,
                     EC_T_DWORD         dwThreadStackSize
                    );
    ~CAtemRasSrvClient();

    EC_T_DWORD  InitClient(EC_T_VOID);
    
    EC_T_DWORD  GetAndIncSeqNr(EC_T_VOID);

    EC_T_VOID   Aging(EC_T_VOID);

    ATEMRAS_T_CONNECTIONSTATE ConState(EC_T_VOID){ return m_oConState; }
    EC_T_VOID   ConState(ATEMRAS_T_CONNECTIONSTATE oConState){ m_oConState = oConState; }
    
    EC_T_DWORD  Cookie(EC_T_VOID){ return m_dwCookie; }
    EC_T_VOID   Cookie(EC_T_DWORD dwVal){ m_dwCookie = dwVal; }

    EC_T_DWORD  ExchangeSocket(CEcSocket* pNewSocket);
    EC_T_VOID   FinalizeExchange(EC_T_VOID);
    EC_T_DWORD  GetClientVersion(EC_T_VOID){ return m_dwClientVersion; }

    EC_T_DWORD  Disconnect(EC_T_DWORD dwCause, EC_T_BOOL bLock = EC_FALSE);

    /* disconnecting or disconnected */
    EC_T_BOOL  IsDisconnect(EC_T_VOID) 
        { return emrasconstate_disconnect == m_oConState; }

    EC_T_DWORD Terminate(EC_T_VOID);

    EC_T_BOOL  IsTerminated(EC_T_VOID)
        { return emrasconstate_terminated == m_oConState; }

private:
    /* explicitly restrict copy */
    CAtemRasSrvClient(const CAtemRasSrvClient&);
    CAtemRasSrvClient& operator=(const CAtemRasSrvClient&);

    EC_T_DWORD  WaitForLogon(EC_T_VOID);

    static  EC_T_VOID   RxThreadStepWrapper(EC_T_PVOID pvParams);
            EC_T_DWORD  RxThreadStep(EC_T_VOID);

    static  EC_T_VOID   WorkerThreadStepWrapper(EC_T_PVOID pvParams);
            EC_T_VOID   WorkerThreadStep(EC_T_VOID);

    EC_T_DWORD SendData(EC_T_BYTE* pbyData, EC_T_DWORD dwLen)
    {
        EC_T_DWORD dwRetVal = EC_E_NOERROR;
        m_oSockSendLock.Lock();
        dwRetVal = m_pSocket->SendData(pbyData, dwLen);
        m_oSockSendLock.UnLock();
        return dwRetVal;
    }

    EC_T_DWORD SendData(EC_T_BYTE* pbyData, EC_T_DWORD dwLen, EC_T_BYTE* pbyData2, EC_T_DWORD dwLen2)
    {
        EC_T_DWORD dwRetVal = EC_E_NOERROR;

#ifdef DEBUG
        if (0 == dwLen)
        {
            OsDbgAssert(EC_FALSE);
        }
#endif

        m_oSockSendLock.Lock();
        
        dwRetVal = m_pSocket->SendData(pbyData, dwLen);
        
        if (EC_E_NOERROR != dwRetVal)
        {
             VERBOSEOUT(0, ("CAtemRasSrvClient::SendData: Error sending head! 0x%08x\n", dwRetVal));
        }
        else
        {
            /* send second block if available */
            if ((dwLen2 != 0) && (pbyData2 != EC_NULL))
            {
                dwRetVal = m_pSocket->SendData(pbyData2, dwLen2);
                if (EC_E_NOERROR != dwRetVal)
                {
                    VERBOSEOUT(0, ("CAtemRasSrvClient::SendData: Error sending tail! 0x%08x\n", dwRetVal));
                }
            }
        }

        m_oSockSendLock.UnLock();
        return dwRetVal;
    }

    EC_T_BOOL  IsSocketDisconnect(EC_T_VOID) 
        { return ((EC_NULL == m_pSocket) || m_pSocket->IsDisconnect()); }

    EC_T_DWORD QueueCmd(ATEMRAS_T_PROTOHDR* pRxHdr, EC_T_DWORD dwDataRead, EC_T_BYTE* pbyDataRead);
    EC_T_BOOL  DequeueCmd(ATEMRAS_T_BUFFER_DESC* &BufDesc) { return m_pWorkerCmdFifo->RemoveNoLock(BufDesc); }

    EC_T_DWORD deMarshal(ATEMRAS_T_PROTOHDR* pHeader, EC_T_DWORD dwLen, EC_T_BYTE* pbyData);
    EC_T_DWORD deMarshalIoControl(ATEMRAS_T_PROTOHDR* pHeader, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalConfigureMaster(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalStart(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalSetMasterState(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetMasterState(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalStop(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetConfiguredSlaves(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSlvIdAtPos(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSlvId(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetFixedAddr(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSlaveProp(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSlaveState(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalSetSlaveState(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalTferSingleRawCmd(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalQueueRawCmd(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalMbxTferCreate(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalMbxTferDelete(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalCoeSdoDownloadReq(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalCoeSdoUploadReq(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalCoeSdoDownload(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalCoeSdoUpload(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalNotifyApp(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalCoeGetODList(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalCoeGetObjectDesc(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalCoeGetEntryDesc(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetProcessData(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalSetProcessData(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalFoeFileDownload(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalFoeFileUpload(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalAoeWriteReq(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalAoeReadReq(EC_T_BYTE* pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalAoeWrite(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalAoeRead(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalAoeReadWrite(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalAoeGetSlaveNetId(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalAoeWriteControl(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetNumHelper(ATEMRAS_T_APICMDTYPE eCmdType, ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalResetSlaveController(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSlaveInfo(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetCfgSlaveInfo(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetBusSlaveInfo(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalIsSlavePresent(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSlaveProcVarInfoNumOf(EC_T_BOOL bInput, ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSlaveInpVarInfoNumOf(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSlaveOutpVarInfoNumOf(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSlaveProcVarInfo(EC_T_BOOL bInput, EC_T_BOOL bEntryEx, ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSlaveInpVarInfo(EC_T_BOOL bEntryEx, ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSlaveOutpVarInfo(EC_T_BOOL bEntryEx, ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalFindProcVarByName(EC_T_BOOL bInput, EC_T_BOOL bEntryEx, ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalFindOutpVarByName(EC_T_BOOL bEntryEx, ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalFindInpVarByName(EC_T_BOOL bEntryEx, ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalSetProcessDataBits(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetProcessDataBits(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalForceProcessDataBits(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalReleaseProcessDataBits(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalReleaseAllProcessDataBits(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalReadSlaveEEPRom(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalWriteSlaveEEPRom(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalAssignSlaveEEPRom(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalReloadSlaveEEPRom(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalSoeRead(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalSoeWrite(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalSoeAbortProcCmd(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetVersion(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetSrcMacAddress(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalEoERegisterEndpoint(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalEoEUnregisterEndpoint(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalLinkOpen(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalLinkClose(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalLinkSendAndFreeFrame(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalLinkRecvFrame(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalReadSlaveIdentification(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalSetSlaveDisabled(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalSetSlaveDisconnected(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalRescueScan(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalGetMasterInfo(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalConfigExtend(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalSetMbxProtocolsSerialize(ATEMRAS_T_PROTOHDR* pTransferHead, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalFoeDownloadReq(EC_T_PBYTE pbyTransfer, EC_T_DWORD dwWrtLen);
    EC_T_DWORD deMarshalFoeUploadReq(EC_T_PBYTE pbyTransfer, EC_T_DWORD dwWrtLen);

    EMMARSH_T_NOTIFICATION_CTXT* AllocateNotificationContext(ATEMRAS_T_PROTOHDR* pHeader);
	EC_T_VOID FreeNotificationContext(EMMARSH_T_NOTIFICATION_CTXT* pNotCtxt);

    CAtemRasServer*     m_pServer;
    CEcSocket*          m_pSocket;
    EC_T_DWORD          m_dwCookie;
    EC_T_DWORD          m_dwCycleTime;
    EC_T_DWORD          m_dwThreadPrio;
    EC_T_DWORD          m_dwThreadStackSize;

    EC_T_DWORD          m_dwProtocolVersion;
    EC_T_DWORD          m_dwClientVersion;
    ATEMRAS_T_CONNECTIONSTATE m_oConState;

    CEcSharedLock       m_oSockLock;
    CEcExclusiveLock    m_oSockSendLock;

    EC_T_BOOL           m_bSockDisposable;

    CAtEmRasCmdQueue*   m_pCmdQueue;

    CAtEmMasterStore*   m_pMasterStore;

    EC_T_DWORD          m_dwSeqNr;
    EC_T_BOOL           m_bEntryValid;
    EC_T_DWORD          m_dwWDReload;
    CEcTimer            m_oWatchdog;

    CEcThread           m_RxThread;
    EC_T_DWORD          m_dwRxBufSize;
    EC_T_BYTE*          m_pRxBuffer;

    CEcThread           m_WorkerThread;
    CAtEmRasNotificationStore*             m_pNotifyStore;
    CFiFoListDyn<ATEMRAS_T_BUFFER_DESC*>*  m_pWorkerCmdFifo;        /* cmds to be deMarshal'ed. single write, single read fifo without locking. */
    CFiFoListDyn<ATEMRAS_T_BUFFER_DESC*>*  m_pWorkerFreeCmdFifo;    /* completely deMarshal'ed cmds pool. single write, single read fifo without locking. */
};

class CAtemClientListEntry
{
public:
    CAtemClientListEntry(CAtemRasSrvClient* pClient):m_pClient(pClient){ m_pNext = m_pPrev = EC_NULL; };
    ~CAtemClientListEntry(){EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemClientListEntry::~CAtemClientListEntry m_pClient", m_pClient,sizeof(CAtemRasSrvClient)); SafeDelete(m_pClient);};

    CAtemRasSrvClient*       GetClient(EC_T_VOID){ return m_pClient; }

    CAtemClientListEntry*    Prev(EC_T_VOID){ return m_pPrev; }
    CAtemClientListEntry*    Next(EC_T_VOID){ return m_pNext; }
    EC_T_VOID               Prev( CAtemClientListEntry* pP ) { m_pPrev = pP; }
    EC_T_VOID               Next( CAtemClientListEntry* pN ) { m_pNext = pN; }
    
    struct _EC_T_MEMORY_LOGGER* GetMemLog() { return EC_NULL; }

private:
    CAtemRasSrvClient*       m_pClient;
    CAtemClientListEntry*    m_pNext;    
    CAtemClientListEntry*    m_pPrev;    
};

#if (defined INCLUDE_RAS_SPOCSUPPORT)

typedef struct _ATEMRAS_T_AC_CLASSSTATE
{
    EC_T_DWORD          dwClassId;
    EC_T_DWORD          dwAccessLevel;

    struct _ATEMRAS_T_AC_CLASSSTATE* pPrev;
    struct _ATEMRAS_T_AC_CLASSSTATE* pNext;
} ATEMRAS_T_AC_CLASSSTATE;

typedef struct _ATEMRAS_T_ACCFG_ENTRY
{
    EC_T_DWORD                  dwCallClass;
    EC_T_DWORD                  dwRequiredAccess;
                                
    EC_T_DWORD                  dwCallOrdinal;
    EC_T_DWORD                  dwCallIndex;
    EC_T_DWORD                  dwCallSubIndex;
    EC_T_DWORD                  dwCSESize;
    EC_T_BYTE*                  pbyCallSpecificEnhancements;
    ATEMRAS_T_AC_CLASSSTATE*    pCallState;

    struct _ATEMRAS_T_ACCFG_ENTRY* pPrev;
    struct _ATEMRAS_T_ACCFG_ENTRY* pNext;
} ATEMRAS_T_ACCFG_ENTRY ;


class CAtemRasAccessControl
{
public:
    CAtemRasAccessControl(EC_T_BYTE* pbyData, EC_T_DWORD dwLen);
    ~CAtemRasAccessControl();

    EC_T_DWORD  AppendClassCfgEntry( ATEMRAS_T_ACCFG_ENTRY* pEntry );

    ATEMRAS_T_AC_CLASSSTATE* 
                GetClassStateById(  EC_T_DWORD dwClassId, EC_T_BOOL bCreate = EC_FALSE );

    EC_T_DWORD  ModifyClassState(EC_T_DWORD dwClassId, EC_T_DWORD dwAccessLevel);

    EC_T_DWORD  CallAccess(EC_T_DWORD dwCallOrdinal, EC_T_DWORD dwCallIndex = 0, EC_T_DWORD dwCallSubIndex = 0, EC_T_BYTE* pbyCSE = EC_NULL);

private:
    EC_T_DWORD  ConfigFromData(EC_T_BYTE* pbyData, EC_T_DWORD dwLen);
    EC_T_DWORD  ConfigFromDataBin(EC_T_BYTE* pbyData, EC_T_DWORD dwLen);
    EC_T_DWORD  ConfigFromDataTxt(EC_T_BYTE* pbyData, EC_T_DWORD dwLen);
    
    ATEMRAS_T_ACCFG_ENTRY*      FindCallEntry( EC_T_DWORD dwCallOrdinal, EC_T_DWORD dwCallIndex = 0, EC_T_DWORD dwCallSubIndex = 0, EC_T_BYTE* pbyCSE = EC_NULL);

    ATEMRAS_T_ACCFG_ENTRY*      m_pEntryRoot;
    ATEMRAS_T_AC_CLASSSTATE*    m_pClassStateRoot;
};
#endif

class CAtemRasServer
{
public:
    CAtemRasServer(ATEMRAS_T_SRVPARMS oParms);
    ~CAtemRasServer();
    
    EC_T_DWORD  GetLastError(EC_T_VOID){ return m_dwLastError; }

    EC_T_DWORD  StartServer(EC_T_VOID);
    EC_T_DWORD  StopServer(EC_T_DWORD dwTimeout);

    EC_T_DWORD  AppendClientEntry(CAtemRasSrvClient* pSlave);

    EC_T_DWORD  GarbageCollect(EC_T_VOID);

    CAtemRasSrvClient* FindClientByCookie(EC_T_DWORD dwCookie);

    EC_T_VOID   LockList(EC_T_VOID){m_oListLock.Lock();}
    EC_T_VOID   UnlockList(EC_T_VOID){m_oListLock.UnLock();}

    EC_T_DWORD  NotifyCallee(EC_T_DWORD dwCode, EC_T_DWORD dwInSize, EC_T_BYTE* pbyInData);
    ATEMRAS_PT_SRVPARMS  Config(EC_T_VOID){ return &m_oConfig; }

private:
    /* explicitly restrict copy */
    CAtemRasServer(const CAtemRasServer&);
    CAtemRasServer& operator=(const CAtemRasServer&);

    EC_T_DWORD  CreateAcceptorSocket(EC_T_VOID);

    static  EC_T_VOID   AcceptorThreadStepWrapper(EC_T_PVOID pvParams);
            EC_T_VOID   AcceptorThreadStep(EC_T_VOID);

    ATEMRAS_T_SRVPARMS      m_oConfig;

    CEcSocket*              m_pServerAcceptorSocket;
    EC_T_DWORD              m_dwLastError;

    CEcThread               m_AcceptorThread;

    EC_T_DWORD              m_dwMastStackSize;        /**< [in]   Stack Size of RAS Master Thread */
    EC_T_DWORD              m_dwClientStackSize;      /**< [in]   Stack Size of RAS Client Threads */

    CAtemClientListEntry*   m_pClientList;

    CEcSharedLock           m_oListLock;

#if (defined INCLUDE_RAS_SPOCSUPPORT)
    CAtemRasAccessControl*  m_pAccessControl;

public:
    EC_T_DWORD  ConfigureAccessControl(EC_T_BYTE* pbyData, EC_T_DWORD dwLen);
    EC_T_DWORD  SetAccessLevel(EC_T_DWORD dwClass, EC_T_DWORD dwLevel);
    EC_T_DWORD  GetAccessLevelByCall(EC_T_DWORD dwCallOrdinal, EC_T_DWORD dwCallIndex = 0, EC_T_DWORD dwCallSubIndex = 0, EC_T_BYTE* pbyCSE = EC_NULL);
#endif
};

#endif /* INC_CATEMRASSERVER */ 
