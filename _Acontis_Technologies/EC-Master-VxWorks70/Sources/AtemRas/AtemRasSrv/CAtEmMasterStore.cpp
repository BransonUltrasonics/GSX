/*-----------------------------------------------------------------------------
 * CAtEmMasterStore.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              description of file
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "CAtemRasServer.h"
#include <AtEmRasType.h>
#include <AtEthercat.h>
#include <CAtemRasCmdQueue.h>
#include "CAtEmMasterStore.h"
#include <EcCommon.h>

/*-CLASS---------------------------------------------------------------------*/
/***************************************************************************************************/
/**
\brief  Default constructor.
*/
CAtEmMasterStore::CAtEmMasterStore()
{
    m_pInstRoot = EC_NULL;
    m_dwClientVersion = 0;
}

/***************************************************************************************************/
/**
\brief  Default destructor.
*/
CAtEmMasterStore::~CAtEmMasterStore()
{
    CAtEmMasterInstance* pTmp   = EC_NULL;
    
    pTmp = m_pInstRoot;

    while( EC_NULL != pTmp )
    {
        m_pInstRoot = pTmp->Next();
        
        if( EC_NULL != m_pInstRoot )
        {
            m_pInstRoot->Prev(EC_NULL);
        }

        pTmp->DeleteMailboxes();

        if( pTmp->IsRegistrationPresent() )
        {
            pTmp->UnregisterClients();
        }

        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterStore::~CAtEmMasterStore pTmp", pTmp, sizeof(CAtEmMasterInstance));
        SafeDelete(pTmp);

        pTmp = m_pInstRoot;
    }
}

/***************************************************************************************************/
/**
\brief  Add new client registration.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmMasterStore::AddRegistration(
    EC_T_DWORD                      dwInstance,     /**< [in]   Master Instance Id */
    EC_T_REGISTERRESULTS*           pRegInfo,       /**< [in]   Response from registration IOCTL */
    EMMARSH_PT_NOTIFICATION_CTXT    pContext        /**< [in]   Notification Context */
                                            )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    CAtEmMasterInstance* pInst = EC_NULL;
    
    /* check parameter */
    if (EC_NULL == pRegInfo)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* get instance */
    pInst = InstanceById(dwInstance, EC_TRUE);
    if (EC_NULL == pInst)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    /* add registration information */
    pContext->pMasterInstance = pInst;
    dwRetVal = pInst->AddRegInfo(pRegInfo, pContext);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Remove a client registration.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmMasterStore::RemoveRegistration(
    EC_T_DWORD dwInstance,      /**< [in]   Master Instance Id */
    EC_T_DWORD dwClientId       /**< [in]   Client Id */
                                               )
{
    EC_T_DWORD              dwRetVal        = EC_E_ERROR;
    EC_T_DWORD              dwRes           = EC_E_ERROR;
    CAtEmMasterInstance*    pTmpInstance    = EC_NULL;

    pTmpInstance = InstanceById(dwInstance, EC_FALSE);

    if( EC_NULL == pTmpInstance )
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }

    dwRes = pTmpInstance->RemoveRegInfo(dwClientId);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    if( !pTmpInstance->IsRegistrationPresent() )
    {
        m_oInstListLock.Lock();
        if( m_pInstRoot == pTmpInstance )
        {
            m_pInstRoot = pTmpInstance->Next();
        }
        else
        {
            if( EC_NULL != pTmpInstance->Prev() )
            {
                pTmpInstance->Prev()->Next(pTmpInstance->Next());
            }
            if( EC_NULL != pTmpInstance->Next() )
            {
                pTmpInstance->Next()->Prev(pTmpInstance->Prev());
            }
        }
        m_oInstListLock.UnLock();
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterStore::RemoveRegistration pTmpInstance", pTmpInstance, sizeof(CAtEmMasterInstance));
        SafeDelete(pTmpInstance);
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Remove all client registrations

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_VOID CAtEmMasterStore::RemoveAllRegistrations(EC_T_VOID)
{
    CAtEmMasterInstance* pTmpInstance = EC_NULL;

    while (EC_NULL != m_pInstRoot)
    {

        /* remove instance from the list */
        m_oInstListLock.Lock();
        pTmpInstance = m_pInstRoot;
        if (EC_NULL != pTmpInstance)
        {
            m_pInstRoot = pTmpInstance->Next();
        }
        m_oInstListLock.UnLock();

        if (EC_NULL != pTmpInstance)
        {
            /* unregister all clients by master */
            pTmpInstance->UnregisterClients();

            /* delete instance */
            EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterStore::RemoveAllRegistrations pTmpInstance", pTmpInstance, sizeof(CAtEmMasterInstance));
            SafeDelete(pTmpInstance);
        }
    }
}

/***************************************************************************************************/
/**
\brief  Get Master Instance by ID.

If Instance does not exist, it is created.
\return EC_NULL on error, pointer to instance on success.
*/
CAtEmMasterInstance* CAtEmMasterStore::InstanceById(
    EC_T_DWORD dwInstanceID,    /**< [in]   ID of master instance to find / create */
    EC_T_BOOL  bSpawn           /**< [in]   Create instance if not found */
                                                   )
{
    CAtEmMasterInstance*    pTmp    = EC_NULL;
    CAtEmMasterInstance*    pFound  = EC_NULL;

    m_oInstListLock.Lock();
    pTmp = m_pInstRoot;

    while( (EC_NULL != pTmp) && (EC_NULL == pFound) )
    {
        if( dwInstanceID == pTmp->InstanceId() )
        {
            pFound = pTmp;
        }
        else
        {
            pTmp = pTmp->Next();
        }
    }

    if( (EC_NULL == pFound) && bSpawn )
    {
        if( (EC_NULL == m_pInstRoot) )
        {
            m_pInstRoot = EC_NEW(CAtEmMasterInstance(dwInstanceID, m_dwClientVersion));
            EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtEmMasterStore::InstanceById m_pInstRoot", m_pInstRoot, sizeof(CAtEmMasterInstance));
            pFound = m_pInstRoot;
        }
        else
        {
            pTmp = m_pInstRoot;
            while( EC_NULL != pTmp->Next() )
            {
                pTmp = pTmp->Next();
            }

            pTmp->Next(EC_NEW(CAtEmMasterInstance(dwInstanceID, m_dwClientVersion)));
            EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtEmMasterStore::InstanceById m_pInstRoot", m_pInstRoot, sizeof(CAtEmMasterInstance));
            if( EC_NULL != pTmp->Next() )
            {
                pFound = pTmp->Next();
                pFound->Prev(pTmp);
            }
        }
    }
    m_oInstListLock.UnLock();

    return pFound;
}

/***************************************************************************************************/
/**
\brief  Add Mailbox Transfer Object.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmMasterStore::AddMbxTferObj(
    EC_T_DWORD      dwInstanceID,       /**< [in]   ID of master instance*/
    EC_T_MBXTFER*   pMbxTferObj         /**< [in]   Allocated Transfer Object */
                                          )
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;
    CAtEmMasterInstance*    pTmpInstance    = EC_NULL;

    if( EC_NULL == pMbxTferObj )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pTmpInstance = InstanceById(dwInstanceID);

    if( EC_NULL == pTmpInstance )
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRetVal = pTmpInstance->AddMbxTferObj(pMbxTferObj);

Exit:
    return dwRetVal;
}

/*-CAtEmClientMbxTferObject--------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Constructor.
*/
CAtEmClientMbxTferObject::CAtEmClientMbxTferObject(
    EC_T_MBXTFER* pMbxTferObj   /**< [in]   Allocated Mbx Transfer object */
                                                  )
{
    m_pMbxTferObj   = pMbxTferObj;
    m_dwTferId      = 0;
    m_pNext         = EC_NULL;
    m_pPrev         = EC_NULL;
}

/***************************************************************************************************/
/**
\brief  Destructor.
*/
CAtEmClientMbxTferObject::~CAtEmClientMbxTferObject()
{
    m_pMbxTferObj = EC_NULL;
}

/*-CAtEmMasterInstance-------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Default Constructor
*/
CAtEmMasterInstance::CAtEmMasterInstance(
    EC_T_DWORD dwInstanceID     /**< [in]   Instance ID */
   ,EC_T_DWORD dwClientVersion)
{
    m_dwInstanceId      = dwInstanceID;
    m_dwClientVersion   = dwClientVersion;
    m_pRegRoot          = EC_NULL;
    m_pNotifyStore      = EC_NULL;
    m_pMbxTferObjList   = EC_NULL;

    m_pLinkDrvList      = EC_NULL;
    m_dwNextLinkDrvId   = 0x1000;

    m_pNext             = EC_NULL;
    m_pPrev             = EC_NULL;
}

/***************************************************************************************************/
/**
\brief  Destructor
*/
CAtEmMasterInstance::~CAtEmMasterInstance()
{
    CAtEmClientRegistry*        pTmp    = EC_NULL;
    CAtEmClientMbxTferObject*   pTmp2   = EC_NULL;
    
    while( EC_NULL != m_pRegRoot )
    {
        pTmp = m_pRegRoot;
        m_pRegRoot = pTmp->Next();
        
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::~CAtEmMasterInstance pTmp", pTmp, sizeof(CAtEmClientRegistry));
        SafeDelete(pTmp);
    }

    while( EC_NULL != m_pMbxTferObjList )
    {
        pTmp2 = m_pMbxTferObjList;
        m_pMbxTferObjList = pTmp2->Next();
        
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::~CAtEmMasterInstance pTmp2", pTmp2, sizeof(CAtEmClientMbxTferObject));
        SafeDelete(pTmp2);
    }

    if (EC_NULL != m_pNotifyStore)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::~CAtEmMasterInstance m_pNotifyStore", m_pNotifyStore, sizeof(CAtEmRasNotificationStore));
        SafeDelete(m_pNotifyStore);
    }

    UnregisterLinkDrivers();
}

/***************************************************************************************************/
/**
\brief  Add client register info to list.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmMasterInstance::AddRegInfo(
    EC_T_REGISTERRESULTS*           pRegInfo,       /**< [in]   Client registration response */
    EMMARSH_PT_NOTIFICATION_CTXT    pContext        /**< [in]   Notification context */
                                          )
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;
    EC_T_DWORD              dwRes       = EC_E_ERROR;
    CAtEmClientRegistry*    pRegNew     = EC_NULL;
    CAtEmClientRegistry*    pRegPrev    = EC_NULL;

    if ((EC_NULL == pRegInfo) || (0 == pRegInfo->dwClntId))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (EC_NULL == m_pNotifyStore)
    {
        m_pNotifyStore = EC_NEW(CAtEmRasNotificationStore(pContext->pServer->Config()->dwConcNotifyAmount, 0));
        EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtEmClientRegistry::Initialize m_pNotifyStore", m_pNotifyStore, sizeof(CAtEmRasNotificationStore));

        if ((EC_NULL == m_pNotifyStore))
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        dwRes = m_pNotifyStore->Initialize();    
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }

    /* create new Registry entry */
    pRegNew = EC_NEW(CAtEmClientRegistry(pRegInfo));
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::AddRegInfo m_pRegRoot", m_pRegRoot, sizeof(CAtEmClientRegistry));
    if (EC_NULL == pRegNew)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRes = pRegNew->Initialize(pContext, m_pNotifyStore);
    if (EC_E_NOERROR != dwRes)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::AddRegInfo m_pRegRoot", pRegNew, sizeof(CAtEmClientRegistry));
        SafeDelete(pRegNew);
        dwRetVal = dwRes;
        goto Exit;
    }

    /* add pRegNew to list */
    if (EC_NULL == m_pRegRoot)
    {
        m_pRegRoot = pRegNew;
    }
    else
    {
        /* add pRegNew to last entry */
        for (pRegPrev = m_pRegRoot; EC_NULL != pRegPrev; pRegPrev = pRegPrev->Next())
        {
            if (EC_NULL == pRegPrev->Next())
            {
                pRegPrev->Next(pRegNew);
                pRegNew->Prev(pRegPrev);
                break;
            }
        }
    }

    OsDbgAssert(0 != pRegNew->ClientId());
    pContext->pClientReg = pRegNew;

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Remove client registration info.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmMasterInstance::RemoveRegInfo(
    EC_T_DWORD dwClientId       /**< [in]   client Id to remove */
                                             )
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;
    CAtEmClientRegistry*    pTmp        = EC_NULL;
    CAtEmClientRegistry*    pFound      = EC_NULL;

    pTmp = m_pRegRoot;

    while( (EC_NULL != pTmp) && (EC_NULL == pFound) )
    {
        if( pTmp->ClientId() == dwClientId )
        {
            pFound = pTmp;
            continue;
        }

        pTmp = pTmp->Next();
    }

    if( EC_NULL == pFound )
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }

    if( m_pRegRoot == pFound )
    {
        m_pRegRoot = pFound->Next();
        if( EC_NULL != m_pRegRoot )
        {
            m_pRegRoot->Prev(EC_NULL);
        }
    }
    else
    {
        if( EC_NULL != pFound->Prev() )
        {
            pFound->Prev()->Next(pFound->Next());
        }
        
        if( EC_NULL != pFound->Next() )
        {
            pFound->Next()->Prev(pFound->Prev());
        }
    }
    /* Remove all notification(s) */
    pFound->m_pNotifyStore->RemoveAllEntries(); /* TODO: only for dwClientId */

    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::RemoveRegInfob pFound", pFound, sizeof(CAtEmClientRegistry));
    SafeDelete(pFound);

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Un-register all registered clients.

Called by garbage collector.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmMasterInstance::UnregisterClients(EC_T_VOID)
{
    EC_T_DWORD              dwRetVal    = EC_E_NOERROR;
    EC_T_DWORD              dwRes       = EC_E_NOERROR;
    CAtEmClientRegistry*    pTmp        = EC_NULL;
    EC_T_IOCTLPARMS         oIoCtlParms = {0};    
    EC_T_DWORD              dwCurClient = 0;
    EC_T_DWORD              dwNumOData  = 0;

    pTmp = m_pRegRoot;

    while( EC_NULL != pTmp )
    {
        dwCurClient = pTmp->ClientId();

        oIoCtlParms.pbyInBuf      = (EC_T_BYTE*)&dwCurClient;
        oIoCtlParms.dwInBufSize   = sizeof(EC_T_DWORD);
        oIoCtlParms.pbyOutBuf     = EC_NULL;
        oIoCtlParms.dwOutBufSize  = 0;
        oIoCtlParms.pdwNumOutData = &dwNumOData;
        dwRes = emIoControl(m_dwInstanceId, EC_IOCTL_UNREGISTERCLIENT, &oIoCtlParms);
        if (dwRes != EC_E_NOERROR)
        {
            dwRetVal = dwRes;
        }
        pTmp = pTmp->Next();
        RemoveRegInfo(dwCurClient);
    }

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Delete Mailbox Transfer Objects.

Called by garbage collector.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmMasterInstance::DeleteMailboxes(EC_T_VOID)
{
    EC_T_DWORD                  dwRetVal       = EC_E_ERROR;
    CAtEmClientMbxTferObject*   pTmp           = EC_NULL;
    EC_T_BYTE*                  pbyMbxTferData = EC_NULL;
    
    while( EC_NULL != m_pMbxTferObjList )
    {
        pTmp = m_pMbxTferObjList;
        pbyMbxTferData = pTmp->TferObj()->pbyMbxTferData;

        emMbxTferDelete(m_dwInstanceId, pTmp->TferObj() );

        if( EC_NULL != pbyMbxTferData )
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "DeleteMailboxes", pbyMbxTferData, 0);
            OsFree(pbyMbxTferData);
        }

        m_pMbxTferObjList = pTmp->Next();

        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::DeleteMailboxes pTmp", pTmp, sizeof(CAtEmClientMbxTferObject));
        SafeDelete(pTmp);
    }

    dwRetVal = EC_E_NOERROR;

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Add Mailbox Transfer Object.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmMasterInstance::AddMbxTferObj(
    EC_T_MBXTFER* pMbxTferObj       /**< [in]   Allocated Transfer Object */
                                             )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    CAtEmClientMbxTferObject*   pNewEntry   = EC_NULL;

    if( EC_NULL == pMbxTferObj )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if( EC_NULL == m_pMbxTferObjList )
    {
        m_pMbxTferObjList = EC_NEW(CAtEmClientMbxTferObject(pMbxTferObj));
        EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::AddMbxTferObj m_pMbxTferObjList", m_pMbxTferObjList, sizeof(CAtEmClientMbxTferObject));
        pNewEntry = m_pMbxTferObjList;
    }
    else
    {
        pNewEntry = m_pMbxTferObjList;

        while( EC_NULL != (pNewEntry->Next()) )
        {
            OsDbgAssert(EC_NULL != pNewEntry->Next() );

            pNewEntry = pNewEntry->Next();
        }

        pNewEntry->Next(EC_NEW(CAtEmClientMbxTferObject(pMbxTferObj)));
        EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::AddMbxTferObj pNewEntry->Next()", pNewEntry->Next(), sizeof(CAtEmClientMbxTferObject));
        if( EC_NULL != pNewEntry->Next() )
        {
            pNewEntry->Next()->Prev(pNewEntry);
        }
        pNewEntry = pNewEntry->Next();
    }

    if( EC_NULL == pNewEntry )
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Find Tfer Object in list.

\return Pointer to transfer object on success, EC_NULL otherwise.
*/
CAtEmClientMbxTferObject* CAtEmMasterInstance::MbxTferObj(
    EC_T_DWORD dwTferId     /**< [in]   Unique transfer id */
                                                         )
{
    CAtEmClientMbxTferObject*   pTmp    = EC_NULL;
    CAtEmClientMbxTferObject*   pFound  = EC_NULL;

    pTmp = m_pMbxTferObjList;

    while( (EC_NULL != pTmp) && (EC_NULL == pFound) )
    {
        if (pTmp->TferObjId() == dwTferId)
        {
            pFound = pTmp;
            continue;
        }

        pTmp = pTmp->Next();
    }

    return pFound;
}

/***************************************************************************************************/
/**
\brief  Remove Transfer object registration.
*/
EC_T_VOID CAtEmMasterInstance::RemoveMbxTferObj(
    CAtEmClientMbxTferObject* pObj      /**< [in]   Transfer object instance to remove */
                                               )
{
    if( EC_NULL == pObj )
    {
        goto Exit;
    }

    if( pObj == m_pMbxTferObjList)
    {
        m_pMbxTferObjList = pObj->Next();
        if( EC_NULL != m_pMbxTferObjList)
        {
            m_pMbxTferObjList->Prev(EC_NULL);
        }
    }
    else
    {
        if( EC_NULL != pObj->Prev() )
        {
            pObj->Prev()->Next(pObj->Next());
        }
        if( EC_NULL != pObj->Next() )
        {
            pObj->Next()->Prev(pObj->Prev());
        }
    }

    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::RemoveMbxTferObj pObj", pObj, sizeof(CAtEmClientMbxTferObject));
    SafeDelete(pObj);

Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Unregister Link Drivers and coresponding opened instances.
Called by garbage collector.
*/
void CAtEmMasterInstance::UnregisterLinkDrivers()
{
    CAtEmClientLinkDrv* pTmpDrv = EC_NULL;
    
    /* Close drivers */
    while (EC_NULL != m_pLinkDrvList)
    {
        pTmpDrv = m_pLinkDrvList;
        m_pLinkDrvList = pTmpDrv->Next();
    
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::UnregisterLinkDrivers pTmpDrv", pTmpDrv, sizeof(CAtEmClientLinkDrv));
        SafeDelete(pTmpDrv);
    }
}

/***************************************************************************************************/
/**
\brief  Add Link Driver Descriptor.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmMasterInstance::AddLinkDrvDtor(
    EC_T_LINK_DRV_DESC* pLinkDrvDtor,       /**< [in]   Link Driver Descriptor */
    EC_T_DWORD*         pdwHandle       /**< [out] Portable handle for use in the RAS protocol  */
    )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    CAtEmClientLinkDrv*   pNewEntry   = EC_NULL;
    CAtEmClientLinkDrv*   pTmpEntry   = EC_NULL;

    if (EC_NULL == pLinkDrvDtor || EC_NULL == pdwHandle)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pNewEntry = EC_NEW(CAtEmClientLinkDrv(this, pLinkDrvDtor, m_dwNextLinkDrvId));
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::AddLinkDrvDtor pTmpEntry", pNewEntry, sizeof(CAtEmClientLinkDrv));
    if (EC_NULL == pNewEntry)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    *pdwHandle = m_dwNextLinkDrvId;
    m_dwNextLinkDrvId++;

    if (EC_NULL == m_pLinkDrvList)
    {
        m_pLinkDrvList = pNewEntry;
    }
    else
    {
        pTmpEntry = m_pLinkDrvList;

        while ( EC_NULL != (pTmpEntry->Next()) )
        {
            OsDbgAssert(EC_NULL != pTmpEntry->Next() );
            pTmpEntry = pTmpEntry->Next();
        }

        pTmpEntry->Next(pNewEntry);

        if (EC_NULL != pTmpEntry->Next())
        {
            pTmpEntry->Next()->Prev(pTmpEntry);
        }

        pTmpEntry = pTmpEntry->Next();
    }

    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/*-CAtEmClientLinkDrv--------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Destructor.
*/
CAtEmClientLinkDrv::~CAtEmClientLinkDrv()
{
    UnregisterDriver();
}

/***************************************************************************************************/
/**
\brief  Add Link Driver instance
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmClientLinkDrv::AddLinkDrvInstance(
    EC_T_VOID *pvHandle, /**< [in]   Handle from link open */
    EC_T_DWORD *pdwHandle  /**< [out] Portable handle for use in the RAS protocol  */
    )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    CAtEmClientLinkDrvInstance*     pNewEntry   = EC_NULL;
    CAtEmClientLinkDrvInstance*     pTmpEntry   = EC_NULL;

    if (EC_NULL == pvHandle || EC_NULL == pdwHandle)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pNewEntry = EC_NEW(CAtEmClientLinkDrvInstance(this, pvHandle, m_dwNextLinkDrvInstanceId));
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtEmClientLinkDrv::AddLinkDrvInstance pNewEntry", pNewEntry, sizeof(CAtEmClientLinkDrvInstance));
    if (EC_NULL == pNewEntry)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    *pdwHandle = m_dwNextLinkDrvInstanceId;
    m_dwNextLinkDrvInstanceId++;

    if (EC_NULL == m_pLinkDrvInstancesList)
    {
        m_pLinkDrvInstancesList = pNewEntry;
    }
    else
    {
        pTmpEntry = m_pLinkDrvInstancesList;

        while ( EC_NULL != (pTmpEntry->Next()) )
        {
            OsDbgAssert(EC_NULL != pTmpEntry->Next() );
            pTmpEntry = pTmpEntry->Next();
        }

        pTmpEntry->Next(pNewEntry);

        if (EC_NULL != pTmpEntry->Next())
        {
            pTmpEntry->Next()->Prev(pTmpEntry);
        }

        pTmpEntry = pTmpEntry->Next();
    }

    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Unregister Link Driver and opened driver instances.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmClientLinkDrv::UnregisterDriver()
{
    if (EC_NULL == m_pLinkDrvDesc) return EC_E_NOERROR;

    CAtEmClientLinkDrvInstance* pTmpInstance = EC_NULL;

    /* Close driver instances */
    while (EC_NULL != m_pLinkDrvInstancesList)
    {
        pTmpInstance = m_pLinkDrvInstancesList;
        m_pLinkDrvInstancesList = pTmpInstance->Next();
    
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmClientLinkDrv::UnregisterDriver pTmpInstance", pTmpInstance, sizeof(CAtEmClientLinkDrvInstance));
        SafeDelete(pTmpInstance);
    }

    /* Unregister EoE Link Driver */
    EC_T_DWORD dwRet = emEoeUnregisterEndpoint(m_pMasterInstance->InstanceId(), m_pLinkDrvDesc);

    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmClientLinkDrv::UnregisterDriver m_pLinkDrvDesc", m_pLinkDrvDesc, sizeof(EC_T_LINK_DRV_DESC));
    SafeDelete(m_pLinkDrvDesc);
    
    return dwRet;
}

/***************************************************************************************************/
/**
\brief  Remove LinkDrv instance object
*/
EC_T_VOID CAtEmClientLinkDrv::RemoveLinkDrvInstance(
    CAtEmClientLinkDrvInstance *pObj /** [in] LinkDrv Instance object */
    )
{
    if (EC_NULL == pObj)
    {
        goto Exit;
    }

    if (pObj == m_pLinkDrvInstancesList)
    {
        m_pLinkDrvInstancesList = pObj->Next();
        if (EC_NULL != m_pLinkDrvInstancesList)
        {
            m_pLinkDrvInstancesList->Prev(EC_NULL);
        }
    }
    else
    {
        if (EC_NULL != pObj->Prev())
        {
            pObj->Prev()->Next(pObj->Next());
        }
        if (EC_NULL != pObj->Next())
        {
            pObj->Next()->Prev(pObj->Prev());
        }
    }

    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmClientLinkDrv::RemoveLinkDrvInstance pObj", pObj, sizeof(CAtEmClientLinkDrvInstance));
    SafeDelete(pObj);

Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Lookup LinkDrvInstance object
\return Pointer to found LinkDrvInstance object on success, EC_NULL otherwise.
*/
CAtEmClientLinkDrvInstance* CAtEmClientLinkDrv::LinkDrvInstance(
    EC_T_DWORD dwHandle /**< [in] Portable handle for use in the RAS protocol  */
    )
{
    CAtEmClientLinkDrvInstance* pTmp = m_pLinkDrvInstancesList;

    while (EC_NULL != pTmp)
    {
        if (pTmp->PortableHandle() == dwHandle)  return pTmp;
        pTmp = pTmp->Next();
    }

    return EC_NULL;
}

/*-CAtEmClientLinkDrvInstance--------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Destructor
*/
CAtEmClientLinkDrvInstance::~CAtEmClientLinkDrvInstance()
{
    Close();
}

/***************************************************************************************************/
/**
\brief  Close Link Driver instance
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmClientLinkDrvInstance::Close()
{
    if (EC_NULL == m_pvInternalHandle) return EC_E_NOERROR;

    EC_T_DWORD dwRet = m_pLinkDrv->LinkDrvDtor()->pfEcLinkClose(InternalHandle());
    m_pvInternalHandle = EC_NULL;

    return dwRet;
}

/***************************************************************************************************/
/**
\brief  Lookup LinkDrv object
\return Pointer to found LinkDrv object on success, EC_NULL otherwise.
*/
CAtEmClientLinkDrv* CAtEmMasterInstance::LinkDrv(
    EC_T_DWORD dwHandle /**< [in] Portable handle for use in the RAS protocol  */
                                                         )
{
    CAtEmClientLinkDrv* pTmp = m_pLinkDrvList;

    while (EC_NULL != pTmp)
    {
        if (pTmp->PortableHandle() == dwHandle)  return pTmp;
        pTmp = pTmp->Next();
    }

    return EC_NULL;
}

/***************************************************************************************************/
/**
\brief  Remove LinkDrv object registration from list.
*/
EC_T_VOID CAtEmMasterInstance::RemoveLinkDrv(
    CAtEmClientLinkDrv* pObj      /**< [in]   object to remove */
                                               )
{
    if( EC_NULL == pObj )
    {
        goto Exit;
    }

    if( pObj == m_pLinkDrvList)
    {
        m_pLinkDrvList = pObj->Next();
        if( EC_NULL != m_pLinkDrvList)
        {
            m_pLinkDrvList->Prev(EC_NULL);
        }
    }
    else
    {
        if( EC_NULL != pObj->Prev() )
        {
            pObj->Prev()->Next(pObj->Next());
        }
        if( EC_NULL != pObj->Next() )
        {
            pObj->Next()->Prev(pObj->Prev());
        }
    }

    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmMasterInstance::RemoveLinkDrv pObj", pObj, sizeof(CAtEmClientLinkDrv));
    SafeDelete(pObj);

Exit:
    return;
}

/*-CAtEmClientRegistry-------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Default Constructor
*/
CAtEmClientRegistry::CAtEmClientRegistry(
    EC_T_REGISTERRESULTS*           pRegResult      /**< [in]   Client registration response */
                                        )
{
    OsDbgAssert(0 != pRegResult->dwClntId);
    OsMemcpy(&m_oRegInfo, pRegResult, sizeof(EC_T_REGISTERRESULTS));

    m_pNotifyStore = EC_NULL;
    m_pContext     = EC_NULL;

    m_pNext        = EC_NULL;
    m_pPrev        = EC_NULL;
}

/***************************************************************************************************/
/**
\brief  Initialize memory.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmClientRegistry::Initialize(EMMARSH_PT_NOTIFICATION_CTXT pContext, CAtEmRasNotificationStore* pNotifyStore)
{
    m_pContext = pContext;
    m_pNotifyStore = pNotifyStore;

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Default destructor.
*/
CAtEmClientRegistry::~CAtEmClientRegistry()
{
    if( EC_NULL != m_pContext )
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmClientRegistry::~CAtEmClientRegistry m_pContext", m_pContext, sizeof(EMMARSH_T_NOTIFICATION_CTXT));
        OsFree(m_pContext);
        m_pContext = EC_NULL;
    }
}

EC_T_DWORD NotifyCodeToTimerId(EC_T_DWORD dwNotifyCode)
{
    EC_T_DWORD dwTimerId = EC_INVALID_LIST_INDEX;
    switch (dwNotifyCode)
    {
    /* throttled notifications */
    case EC_NOTIFY_CYCCMD_WKC_ERROR:            dwTimerId = TIMER_NOTIFY_CYCCMD_WKC_ERROR; break;
    case EC_NOTIFY_EOE_MBXSND_WKC_ERROR:        dwTimerId = TIMER_NOTIFY_EOE_MBXSND_WKC_ERROR; break;
    case EC_NOTIFY_COE_MBXSND_WKC_ERROR:        dwTimerId = TIMER_NOTIFY_COE_MBXSND_WKC_ERROR; break;
    case EC_NOTIFY_FOE_MBXSND_WKC_ERROR:        dwTimerId = TIMER_NOTIFY_FOE_MBXSND_WKC_ERROR; break;
    case EC_NOTIFY_FRAME_RESPONSE_ERROR:        dwTimerId = TIMER_NOTIFY_FRAME_RESPONSE_ERROR; break;
    case EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL: dwTimerId = TIMER_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL; break;
    case EC_NOTIFY_STATUS_SLAVE_ERROR:          dwTimerId = TIMER_NOTIFY_STATUS_SLAVE_ERROR; break;
    case EC_NOTIFY_SLAVE_NOT_ADDRESSABLE:       dwTimerId = TIMER_NOTIFY_SLAVE_NOT_ADDRESSABLE; break;
    case EC_NOTIFY_SOE_MBXSND_WKC_ERROR:        dwTimerId = TIMER_NOTIFY_SOE_MBXSND_WKC_ERROR; break;
    case EC_NOTIFY_VOE_MBXSND_WKC_ERROR:        dwTimerId = TIMER_NOTIFY_VOE_MBXSND_WKC_ERROR; break;

    /* not throttled notifications */
    default:
        dwTimerId = EC_INVALID_LIST_INDEX;
        break;
    }

    return dwTimerId;
}

/***************************************************************************************************/
/**
\brief  Check whether a specific type of Notification is allowed to send or needs to be throttled.

\return EC_TRUE: sending allowed, Timer triggered (if applicable), EC_FALSE: sending not allowed (throttled).
*/
EC_T_BOOL   CAtEmClientRegistry::ChkAndTriggerNotificationType(
    EC_T_DWORD dwNotifyCode 
                                                              )
{
    EC_T_BOOL   bRetVal = EC_TRUE;
    EC_T_DWORD  dwTimerId = NotifyCodeToTimerId(dwNotifyCode);

    /* notification throttled? */
    if (EC_INVALID_LIST_INDEX != dwTimerId)
    {
        if (m_pNotifyStore->IsThrottledNotificationInQueue(dwTimerId)
            || (m_aoNotificationTimers[dwTimerId].IsStarted() && !m_aoNotificationTimers[dwTimerId].IsElapsed()))
        {
            /* throttle */
            bRetVal = EC_FALSE;
        }
        else
        {
            /* start throttling timeout and send */
            m_aoNotificationTimers[dwTimerId].Start(m_pContext->pServer->Config()->dwCycErrInterval);
        }
    }

    return bRetVal;
}

/*-CAtEmRasNotificationStore-------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Constructor.

Prepare Notification buffers.
*/
CAtEmRasNotificationStore::CAtEmRasNotificationStore(
    EC_T_DWORD          dwEntries,      /**< [in]   Amount of Notifications to pre-alloc */
    EC_T_DWORD          dwUserSize      /**< [in]   User addable size for Notifications */
                                                    )
: m_dwEntries(dwEntries)
{
    
    m_dwSingleEntrySize = sizeof(EMMARSH_T_NOTIFICATION_ALLOC) + ATEMRAS_T_PROTOHDR_SIZE + 
                          ATEMRAS_T_NOTIFICATIONTRANSFER_SIZE + LARGEST_NOTIFICATION + dwUserSize;

    m_ppNotificationList = EC_NULL;

    OsMemset(m_abThrottledNotificationInQueue, 0, sizeof(EC_T_BOOL) * NOTIFY_TIMER_AMOUNT);

#ifdef DEBUGTRACE
    m_pFreeEntriesFifo = EC_NEW(CFiFoListDyn<EMMARSH_T_NOTIFICATION_ALLOC*>(dwEntries + 1, EC_NULL, EC_NULL, EC_MEMTRACE_RAS));
    m_pPendEntriesFifo = EC_NEW(CFiFoListDyn<EMMARSH_T_NOTIFICATION_ALLOC*>(dwEntries + 1, EC_NULL, EC_NULL, EC_MEMTRACE_RAS));
#else
    m_pFreeEntriesFifo = EC_NEW(CFiFoListDyn<EMMARSH_T_NOTIFICATION_ALLOC*>(dwEntries + 1, EC_NULL, EC_NULL));
    m_pPendEntriesFifo = EC_NEW(CFiFoListDyn<EMMARSH_T_NOTIFICATION_ALLOC*>(dwEntries + 1, EC_NULL, EC_NULL));
#endif
}

/***************************************************************************************************/
/**
\brief  Allocate the memory pool.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmRasNotificationStore::Initialize(EC_T_VOID)
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;
    EC_T_DWORD      dwIdx    = 0;
                    
    m_ppNotificationList = (EMMARSH_PT_NOTIFICATION_ALLOC*)OsMalloc(sizeof(EMMARSH_PT_NOTIFICATION_ALLOC)*m_dwEntries);
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtEmRasNotificationStore::Initialize m_ppNotificationList", m_ppNotificationList, sizeof(EMMARSH_PT_NOTIFICATION_ALLOC)*m_dwEntries);
    if( EC_NULL == m_ppNotificationList )
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    OsMemset(m_ppNotificationList, 0, (sizeof(EMMARSH_PT_NOTIFICATION_ALLOC)*m_dwEntries));

    for (dwIdx = 0; dwIdx < m_dwEntries; dwIdx++)
    {
        m_ppNotificationList[dwIdx] = (EMMARSH_PT_NOTIFICATION_ALLOC)OsMalloc(m_dwSingleEntrySize);
        EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtEmRasNotificationStore::Initialize m_ppNotificationList[dwIdx]", m_ppNotificationList[dwIdx], m_dwSingleEntrySize);
        if( EC_NULL == m_ppNotificationList[dwIdx] )
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        OsMemset(m_ppNotificationList[dwIdx], 0, m_dwSingleEntrySize);
        m_pFreeEntriesFifo->Add(m_ppNotificationList[dwIdx]);
    }

    dwRetVal = EC_E_NOERROR;    
Exit:

    if( EC_E_NOMEMORY == dwRetVal )
    {
        if( EC_NULL != m_ppNotificationList )
        {
            for( dwIdx = 0; dwIdx < m_dwEntries; dwIdx++ )
            {
                if( EC_NULL != m_ppNotificationList[dwIdx] )
                {
                    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmRasNotificationStore::Initialize m_ppNotificationList[dwIdx]", m_ppNotificationList[dwIdx], 0);
                    OsFree(m_ppNotificationList[dwIdx]);
                    m_ppNotificationList[dwIdx] = EC_NULL;
                }
            }
            EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmRasNotificationStore::Initialize m_ppNotificationList", m_ppNotificationList, 0);
            OsFree(m_ppNotificationList);
            m_ppNotificationList = EC_NULL;
        }
    }

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Destructor.
*/
CAtEmRasNotificationStore::~CAtEmRasNotificationStore()
{
    EC_T_DWORD  dwIdx   = 0;

    SafeDelete(m_pFreeEntriesFifo);
    SafeDelete(m_pPendEntriesFifo);

    if( EC_NULL != m_ppNotificationList )
    {
        for( dwIdx = 0; dwIdx < m_dwEntries; dwIdx++ )
        {
            if( EC_NULL != m_ppNotificationList[dwIdx] )
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmRasNotificationStore::~CAtEmRasNotificationStore m_ppNotificationList[dwIdx]", m_ppNotificationList[dwIdx], m_dwSingleEntrySize);
                OsFree(m_ppNotificationList[dwIdx]);
                m_ppNotificationList[dwIdx] = EC_NULL;
            }
        }
        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtEmRasNotificationStore::~CAtEmRasNotificationStore m_ppNotificationList", m_ppNotificationList, sizeof(EMMARSH_PT_NOTIFICATION_ALLOC)*m_dwEntries);
        OsFree(m_ppNotificationList);
        m_ppNotificationList = EC_NULL;
    }
}

/***************************************************************************************************/
/**
\brief  "Allocate" a Notification entry.

\return EC_E_NOERROR on success, error code otherwise.
*/
EMMARSH_T_NOTIFICATION_ALLOC* CAtEmRasNotificationStore::AllocEntry(EC_T_VOID)
{
EMMARSH_T_NOTIFICATION_ALLOC* pRetVal = EC_NULL;

    m_pFreeEntriesFifo->RemoveNoLock(pRetVal);

#if (defined DEBUG)
    /* dump list of currently allocated notifications if out of memory */
    if (EC_NULL == pRetVal)
    {
    EC_T_DWORD  dwIdx = 0;
    EC_T_DWORD  dwNotifyCode = EC_NOTIFY_GENERIC;

        OsDbgMsg("Error allocating space for Notification! List dump:\n");
        for (dwIdx = 0; dwIdx < m_dwEntries; dwIdx++)
        {
            /* get notification code from list */
            dwNotifyCode = ATEMRAS_NOTIF_GET_CODE(
                (ATEMRAS_PT_NOTIFICATIONTRANSFER)ATEMRAS_PROTOHDR_DATA((ATEMRAS_PT_PROTOHDR)m_ppNotificationList[dwIdx]->byData)
                );

            OsDbgMsg("List entry %03d: %s (0x%08x)\n", dwIdx, ecatGetNotifyText(dwNotifyCode), dwNotifyCode);
        }
    }
#endif
    return pRetVal;
}

/***************************************************************************************************/
/**
\brief  "Store" a Notification entry.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmRasNotificationStore::StoreEntry(EMMARSH_T_NOTIFICATION_ALLOC* pEntry)
{
    /* handle notification throtteling */
    {
    EC_T_DWORD  dwNotifyCode = ATEMRAS_NOTIF_GET_CODE((ATEMRAS_PT_NOTIFICATIONTRANSFER)ATEMRAS_PROTOHDR_DATA((ATEMRAS_PT_PROTOHDR)pEntry->byData));
    EC_T_DWORD  dwTimerId    = NotifyCodeToTimerId(dwNotifyCode);

        if (EC_INVALID_LIST_INDEX != dwTimerId)
        {
            m_abThrottledNotificationInQueue[dwTimerId] = EC_TRUE;
        }
    }
    m_pPendEntriesFifo->Add(pEntry);

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  "Free" a Notification entry.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_VOID CAtEmRasNotificationStore::FreeEntry(EMMARSH_T_NOTIFICATION_ALLOC* pEntry)
{
EC_T_DWORD dwNotifyCode = ATEMRAS_NOTIF_GET_CODE((ATEMRAS_PT_NOTIFICATIONTRANSFER)ATEMRAS_PROTOHDR_DATA((ATEMRAS_PT_PROTOHDR)pEntry->byData));

    if (EC_INVALID_LIST_INDEX != NotifyCodeToTimerId(dwNotifyCode))
    {
        m_abThrottledNotificationInQueue[NotifyCodeToTimerId(dwNotifyCode)] = EC_FALSE;
    }
#if (defined DEBUG)
    OsMemset(pEntry, 0, sizeof(EMMARSH_T_NOTIFICATION_ALLOC));
#endif
#if 0
    ATEMRAS_NOTIF_SET_CODE(
        (ATEMRAS_PT_NOTIFICATIONTRANSFER)ATEMRAS_PROTOHDR_DATA((ATEMRAS_PT_PROTOHDR)pEntry->byData), 0);
#endif
    m_pFreeEntriesFifo->Add(pEntry);
}

/***************************************************************************************************/
/**
\brief  "Remove" a Notification entry.

\return EC_E_NOERROR on success, error code otherwise.
*/
EMMARSH_T_NOTIFICATION_ALLOC* CAtEmRasNotificationStore::RemoveEntry(EC_T_VOID)
{
EMMARSH_T_NOTIFICATION_ALLOC* pRetVal = EC_NULL;

    m_pPendEntriesFifo->RemoveNoLock(pRetVal);

    return pRetVal;
}

/***************************************************************************************************/
/**
\brief  Remove all Pending notification entries.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CAtEmRasNotificationStore::RemoveAllEntries(EC_T_VOID)
{
EMMARSH_T_NOTIFICATION_ALLOC* pNotificationAlloc = EC_NULL;

    for (pNotificationAlloc = RemoveEntry(); EC_NULL != pNotificationAlloc; pNotificationAlloc = RemoveEntry())
    {
        FreeEntry(pNotificationAlloc);
    }
    OsMemset(m_abThrottledNotificationInQueue, 0, sizeof(EC_T_BOOL) * NOTIFY_TIMER_AMOUNT);

    return EC_E_NOERROR;
}

/*-END OF SOURCE FILE--------------------------------------------------------*/
