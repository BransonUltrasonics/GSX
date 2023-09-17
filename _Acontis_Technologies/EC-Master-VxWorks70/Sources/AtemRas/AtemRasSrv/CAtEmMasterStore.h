/*-----------------------------------------------------------------------------
 * CAtEmMasterStore.h       file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Willig, Andreas
 * Description              description of file
 * Date                     2007/5/14::15:38
 *---------------------------------------------------------------------------*/

#ifndef INC_CATEMMASTERSTORE
#define INC_CATEMMASTERSTORE      1

/*-INCLUDES------------------------------------------------------------------*/

#include "AtemRasRemoteProtocol.h"
#include "EcTimer.h"

/*-DEFINES-------------------------------------------------------------------*/

#define MAX_ETHERNET_FRAME_SIZE 1536

/* size by how many bytes standard notifications are enhanced to store user data larger than regular notifications */
#define MAXIMUM_USER_NOTIFICATION_STORE (0)

#define EC_INVALID_LIST_INDEX   (EC_T_DWORD)-1

#define TIMER_NOTIFY_CYCCMD_WKC_ERROR                   ((EC_T_DWORD)0)
#define TIMER_NOTIFY_EOE_MBXSND_WKC_ERROR               ((EC_T_DWORD)4)
#define TIMER_NOTIFY_COE_MBXSND_WKC_ERROR               ((EC_T_DWORD)5)
#define TIMER_NOTIFY_FOE_MBXSND_WKC_ERROR               ((EC_T_DWORD)6)
#define TIMER_NOTIFY_FRAME_RESPONSE_ERROR               ((EC_T_DWORD)7)
#define TIMER_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL        ((EC_T_DWORD)8)
#define TIMER_NOTIFY_STATUS_SLAVE_ERROR                 ((EC_T_DWORD)9)
#define TIMER_NOTIFY_SLAVE_NOT_ADDRESSABLE              ((EC_T_DWORD)10)
#define TIMER_NOTIFY_SOE_MBXSND_WKC_ERROR               ((EC_T_DWORD)12)
#define TIMER_NOTIFY_VOE_MBXSND_WKC_ERROR               ((EC_T_DWORD)13)
#define NOTIFY_TIMER_AMOUNT                             ((EC_T_DWORD)14)

/*-TYPEDEFS------------------------------------------------------------------*/

typedef union _u_t_dummyUnion
{
    EC_T_ERROR_NOTIFICATION_DESC    pErr;
    EC_T_NOTIFICATION_DESC          pNot;
    EC_T_STATECHANGE                pStChng;
    EC_T_MBXTFER                    pMbox;
    EC_T_BYTE                       aBuffer[512];       /* for emergency mailbox notifications */
} u_t_dummyUnion;

#define LARGEST_NOTIFICATION    sizeof(u_t_dummyUnion)

class CAtemRasServer;
class CAtemRasSrvClient;
class CAtEmClientRegistry;
class CAtEmMasterInstance;
class CAtEmClientLinkDrv;

typedef struct _EMMARSH_T_NOTIFICATION_CTXT
{
    CAtEmClientRegistry*    pClientReg;         /**< [in]   Client Registration instance. EC_NULL: dwClientId invalid */
    CAtemRasServer*         pServer;            /**< [in]   Server handle */
    CAtemRasSrvClient*      pConnection;        /**< [in]   Connection Instance */
    CAtEmMasterInstance*    pMasterInstance;    /**< [in]   Master Instance storage */
    EC_T_DWORD              dwMasterInstance;   /**< [in]   Master Instance */
    EC_T_DWORD              dwClientId;         /**< [in]   Client ID. pClientReg == EC_NULL: invalid. */
    EC_T_DWORD              dwCookie;           /**< [in]   Session Cookie */
} EMMARSH_T_NOTIFICATION_CTXT, *EMMARSH_PT_NOTIFICATION_CTXT;

typedef struct _EMMARSH_T_NOTIFICATION_ALLOC
{
    EC_T_DWORD                  dwDataSize;
    EC_T_DWORD                  dwMbxDataSize;
    EC_T_BYTE*                  pbyMbxData;
    EC_T_BYTE                   byData[1]; /* ATEMRAS_PT_PROTOHDR */
}   EMMARSH_T_NOTIFICATION_ALLOC, *EMMARSH_PT_NOTIFICATION_ALLOC;

/*-CLASS---------------------------------------------------------------------*/

class CAtEmRasNotificationStore
{
public:
    CAtEmRasNotificationStore(                      EC_T_DWORD                      dwEntries, 
                                                    EC_T_DWORD                      dwUserSize);
    ~CAtEmRasNotificationStore();
    EC_T_DWORD                  Initialize(         EC_T_VOID                                       );

    EMMARSH_T_NOTIFICATION_ALLOC* AllocEntry(       EC_T_VOID                                       );


    EC_T_DWORD                  StoreEntry(         EMMARSH_T_NOTIFICATION_ALLOC*   pAlloc          );

    EMMARSH_T_NOTIFICATION_ALLOC* RemoveEntry(      EC_T_VOID                                       );
    EC_T_VOID                   FreeEntry(          EMMARSH_T_NOTIFICATION_ALLOC*   pAlloc          );

    EC_T_DWORD                  GetEntrySize(       EC_T_VOID                                       )
                                    { return m_dwSingleEntrySize; }

    EC_T_DWORD                  RemoveAllEntries(    EC_T_VOID                                       );

    EC_T_BOOL                   IsThrottledNotificationInQueue(
                                                    EC_T_DWORD  dwThrottleTimerId                   )
                                    { return m_abThrottledNotificationInQueue[dwThrottleTimerId]; }

    EC_T_BOOL                       m_abThrottledNotificationInQueue[NOTIFY_TIMER_AMOUNT];
private:
    EC_T_DWORD                      m_dwEntries;
    EC_T_DWORD                      m_dwSingleEntrySize;
    EMMARSH_PT_NOTIFICATION_ALLOC*  m_ppNotificationList;
    
    CFiFoListDyn<EMMARSH_T_NOTIFICATION_ALLOC*>* m_pFreeEntriesFifo;
    CFiFoListDyn<EMMARSH_T_NOTIFICATION_ALLOC*>* m_pPendEntriesFifo;
};

class CAtEmClientRegistry
{
public:
    CAtEmClientRegistry(                            EC_T_REGISTERRESULTS*           pRegResult      );
    ~CAtEmClientRegistry();
    EC_T_DWORD                  Initialize(         EMMARSH_PT_NOTIFICATION_CTXT    pContext
                                                   ,CAtEmRasNotificationStore*      pNotifyStore    );

    EC_T_DWORD                  ClientId(           EC_T_VOID                                       )
                                    { return m_oRegInfo.dwClntId; }

    EC_T_BOOL                   ChkAndTriggerNotificationType(
                                                    EC_T_DWORD                      dwNotifyCode    );
    EMMARSH_PT_NOTIFICATION_CTXT
                                GetNotifyContext(   EC_T_VOID                                       )
        { return m_pContext; }
    EC_T_REGISTERRESULTS*
                                GetRegResults(      EC_T_VOID                                       )
        { return &m_oRegInfo; }
    
    CAtEmClientRegistry*        Next(               EC_T_VOID                                       )
                                    { return m_pNext; }
    EC_T_VOID                   Next(               CAtEmClientRegistry*            pN              )
                                    { m_pNext = pN; }
    CAtEmClientRegistry*        Prev(               EC_T_VOID                                       )
                                    { return m_pPrev; }
    EC_T_VOID                   Prev(               CAtEmClientRegistry*            pP              )
                                    { m_pPrev = pP; }

    CAtEmRasNotificationStore*      m_pNotifyStore;
private:
    EC_T_REGISTERRESULTS            m_oRegInfo;
    EMMARSH_PT_NOTIFICATION_CTXT    m_pContext;


    CEcTimer                        m_aoNotificationTimers[NOTIFY_TIMER_AMOUNT];

    CAtEmClientRegistry*            m_pNext;
    CAtEmClientRegistry*            m_pPrev;
};

class CAtEmClientMbxTferObject
{
public:
    CAtEmClientMbxTferObject(                       EC_T_MBXTFER*                   pMbxTferObj     );
    ~CAtEmClientMbxTferObject();

    EC_T_MBXTFER*               TferObj(            EC_T_VOID                                       )
                                    { return m_pMbxTferObj; }
    EC_T_DWORD                  TferObjId(             EC_T_VOID                                       )
                                    { return ((EC_T_DWORD)((EC_T_BYTE*)m_pMbxTferObj - (EC_T_BYTE*)EC_NULL)); }

    EC_T_VOID                   SetTferId(EC_T_DWORD dwTferId)
                                    { m_dwTferId = dwTferId; }

    EC_T_DWORD                  GetTferId(EC_T_VOID)
                                    { return m_dwTferId; }

    CAtEmClientMbxTferObject*   Next(               EC_T_VOID                                       )
                                    { return m_pNext; }
    EC_T_VOID                   Next(               CAtEmClientMbxTferObject*       pN              )
                                    { m_pNext = pN; }
    CAtEmClientMbxTferObject*   Prev(               EC_T_VOID                                       )
                                    { return m_pPrev; }
    EC_T_VOID                   Prev(               CAtEmClientMbxTferObject*       pP              )
                                    { m_pPrev = pP; }
   
private:
    EC_T_MBXTFER*               m_pMbxTferObj; /* m_pMbxTferObj->dwTferId exchanged with Master */
    EC_T_DWORD                  m_dwTferId; /* exchanged with AtemRasClnt */

    CAtEmClientMbxTferObject*   m_pNext;
    CAtEmClientMbxTferObject*   m_pPrev;
};


/* Context for Link Driver instances returned by LinkOpen */
class CAtEmClientLinkDrvInstance
{
public:
    CAtEmClientLinkDrvInstance(CAtEmClientLinkDrv *pLinkDrv, EC_T_VOID *hInstanceHandle, EC_T_DWORD dwHandle)
        : m_pLinkDrv(pLinkDrv),
          m_pvInternalHandle(hInstanceHandle),
          m_dwPortableHandle(dwHandle),
          m_pNext(EC_NULL),
          m_pPrev(EC_NULL)
    {}

    ~CAtEmClientLinkDrvInstance();

    EC_T_DWORD PortableHandle() { return m_dwPortableHandle; }
    EC_T_VOID* InternalHandle() { return m_pvInternalHandle; }
    EC_T_DWORD Close();

    CAtEmClientLinkDrvInstance* Next() { return m_pNext; }
    EC_T_VOID Next(CAtEmClientLinkDrvInstance* pN) { m_pNext = pN; }
    CAtEmClientLinkDrvInstance* Prev() { return m_pPrev; }
    EC_T_VOID Prev(CAtEmClientLinkDrvInstance* pP) { m_pPrev = pP; }
   
private:
    CAtEmClientLinkDrv* m_pLinkDrv;
    EC_T_VOID* m_pvInternalHandle;
    EC_T_DWORD m_dwPortableHandle;

    CAtEmClientLinkDrvInstance* m_pNext;
    CAtEmClientLinkDrvInstance* m_pPrev;
};

/* Link Driver context */
class CAtEmClientLinkDrv
{
public:
    CAtEmClientLinkDrv(CAtEmMasterInstance* pMasterInstance, EC_T_LINK_DRV_DESC *pLinkDrvDesc, EC_T_DWORD dwHandle)
     : m_pLinkDrvDesc(pLinkDrvDesc),
       m_pMasterInstance(pMasterInstance),
       m_dwPortableHandle(dwHandle),
       m_pLinkDrvInstancesList(EC_NULL),
       m_dwNextLinkDrvInstanceId(0x1000000),
       m_pNext(EC_NULL),
       m_pPrev(EC_NULL)
    {}

    ~CAtEmClientLinkDrv();

    EC_T_LINK_DRV_DESC* LinkDrvDtor() { return m_pLinkDrvDesc; }
    EC_T_DWORD PortableHandle() { return m_dwPortableHandle; }
    EC_T_DWORD UnregisterDriver();
    EC_T_DWORD AddLinkDrvInstance(EC_T_VOID* pvHandle, EC_T_DWORD *pdwHandle);
    CAtEmClientLinkDrvInstance* LinkDrvInstance(EC_T_DWORD dwHandle);
    EC_T_VOID RemoveLinkDrvInstance(CAtEmClientLinkDrvInstance* pObj);
    
    CAtEmClientLinkDrv* Next() { return m_pNext; }
    EC_T_VOID Next(CAtEmClientLinkDrv* pN) { m_pNext = pN; }
    CAtEmClientLinkDrv* Prev() { return m_pPrev; }
    EC_T_VOID Prev(CAtEmClientLinkDrv* pP) { m_pPrev = pP; }
   
private:
    EC_T_LINK_DRV_DESC* m_pLinkDrvDesc;
    CAtEmMasterInstance* m_pMasterInstance;
    EC_T_DWORD m_dwPortableHandle;
    
    CAtEmClientLinkDrvInstance* m_pLinkDrvInstancesList;
    EC_T_DWORD m_dwNextLinkDrvInstanceId; /* Unique ID, Increasing only */
    
    CAtEmClientLinkDrv* m_pNext;
    CAtEmClientLinkDrv* m_pPrev;
};


class CAtEmMasterInstance
{
public:
    CAtEmMasterInstance(                            EC_T_DWORD                      dwInstanceID,
                                                    EC_T_DWORD                      dwClientVersion);
    ~CAtEmMasterInstance();

    EC_T_BOOL   IsRegistrationPresent(              EC_T_VOID                                       )
                    { return (EC_NULL != m_pRegRoot); }
    EC_T_DWORD  AddRegInfo(                         EC_T_REGISTERRESULTS*           pRegInfo, 
                                                    EMMARSH_PT_NOTIFICATION_CTXT    pContext        );
    EC_T_DWORD  RemoveRegInfo(                      EC_T_DWORD dwClientId);

    EC_T_DWORD  InstanceId(                         EC_T_VOID                                       )
                    { return m_dwInstanceId; }

    EC_T_DWORD  UnregisterClients(                  EC_T_VOID                                       );

    CAtEmRasNotificationStore* GetNotifyStore(      EC_T_VOID                                       )
                    { return m_pNotifyStore; }

    /* Mbx Transfer */

    EC_T_DWORD  DeleteMailboxes(                    EC_T_VOID                                       );

    EC_T_DWORD  AddMbxTferObj(                      EC_T_MBXTFER*                   pMbxTferObj     );
    
    CAtEmClientMbxTferObject*   MbxTferObj(         EC_T_DWORD                      dwTferId        );
    
    EC_T_VOID   RemoveMbxTferObj(                   CAtEmClientMbxTferObject*       pObj            );

    /* Link Driver */

    EC_T_VOID UnregisterLinkDrivers();

    EC_T_DWORD AddLinkDrvDtor(EC_T_LINK_DRV_DESC* pLinkDrvDtor, EC_T_DWORD *pdwPortableHandle);
    
    CAtEmClientLinkDrv* LinkDrv(EC_T_DWORD dwPortableHandle);
    
    EC_T_VOID RemoveLinkDrv(CAtEmClientLinkDrv* pObj);
    

    CAtEmMasterInstance*        Next(               EC_T_VOID                                       )
                                    { return m_pNext; }
    EC_T_VOID                   Next(               CAtEmMasterInstance*            pN              )
                                    { m_pNext = pN; }
    CAtEmMasterInstance*        Prev(               EC_T_VOID                                       )
                                    { return m_pPrev; }
    EC_T_VOID                   Prev(               CAtEmMasterInstance*            pP              )
                                    { m_pPrev = pP; }
    EC_T_DWORD  GetClientVersion(EC_T_VOID){ return m_dwClientVersion; }

private:
    EC_T_DWORD                  m_dwInstanceId;
    EC_T_DWORD                  m_dwClientVersion;
    CAtEmClientRegistry*        m_pRegRoot;

    CAtEmRasNotificationStore*  m_pNotifyStore;

    CAtEmClientMbxTferObject*   m_pMbxTferObjList;

    CAtEmClientLinkDrv*         m_pLinkDrvList;
    EC_T_DWORD                  m_dwNextLinkDrvId; /* Unique ID, Increasing only */

    CAtEmMasterInstance*        m_pNext;
    CAtEmMasterInstance*        m_pPrev;
};

class CAtEmMasterStore
{
public:
    CAtEmMasterStore();
    ~CAtEmMasterStore();

    EC_T_BOOL               IsInstanceAttached(     EC_T_VOID                                       )
                                { return (EC_NULL != m_pInstRoot); }

    EC_T_DWORD              AddRegistration(        EC_T_DWORD                      dwInstance, 
                                                    EC_T_REGISTERRESULTS*           pRegInfo, 
                                                    EMMARSH_PT_NOTIFICATION_CTXT    pContext        );
    EC_T_DWORD              RemoveRegistration(     EC_T_DWORD                      dwInstance, 
                                                    EC_T_DWORD                      dwClientId      );

    EC_T_VOID               RemoveAllRegistrations( EC_T_VOID                                       );

    CAtEmMasterInstance*    GetInstanceRoot(        EC_T_VOID                                       )
                                { return m_pInstRoot; }

    CAtEmMasterInstance*    InstanceById(           EC_T_DWORD                      dwInstanceID, 
                                                    EC_T_BOOL                       bSpawn=EC_TRUE  );

    EC_T_DWORD              AddMbxTferObj(          EC_T_DWORD                      dwInstanceID,   
                                                    EC_T_MBXTFER*                   pMbxTferObj     );

    EC_T_VOID   SetClientVersion(EC_T_DWORD dwClientVersion) { m_dwClientVersion = dwClientVersion; }
    EC_T_DWORD  GetClientVersion(EC_T_VOID){ return m_dwClientVersion; }

private:
    CAtEmMasterInstance*    m_pInstRoot;
    CEcExclusiveLock        m_oInstListLock;
    EC_T_DWORD              m_dwClientVersion;
};

/*-FUNCTION DECLARATIONS-----------------------------------------------------*/
EC_T_DWORD NotifyCodeToTimerId(EC_T_DWORD dwNotifyCode);

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FUNCTION DEFINITIONS------------------------------------------------------*/

#endif /* INC_CATEMMASTERSTORE */
 
/*-END OF SOURCE FILE--------------------------------------------------------*/
