/*-----------------------------------------------------------------------------
* EcSocket.cpp             file
* Copyright                acontis technologies GmbH, Weingarten, Germany
* Response                 Bussmann, Paul
* Description              Implementation of RTOS32Win RAS over Shared Memory
* Date                     2012/11/29::11:30
*---------------------------------------------------------------------------*/

#include "EcOs.h"
#if defined(EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED)

/*-INCLUDES------------------------------------------------------------------*/

#include "EcSocket.h"
#include "EcCommon.h"
#include "EcTimer.h"


/*-STATIC CLASS-MEMBERS------------------------------------------------------*/

EC_T_DWORD      CMsgQueueRtosShm::s_dwRtosLibInitCount             = 0;
HANDLE          CMsgQueueRtosShm::s_hRtosLibApiSyncStartStopMutex  = NULL;
EC_T_RTOSSHM*   CMsgQueueRtosShm::s_poMap                          = EC_NULL;
EC_T_BOOL       CMsgQueueRtosShm::s_bShmInitialized                = EC_FALSE;


/*-FUNCTIONS-----------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Constructor of CMsgQueueRtosShm
*/
CMsgQueueRtosShm::CMsgQueueRtosShm() 
{
	OsMemset(&m_oMsgQueueOptions, 0, sizeof(EC_T_MSGQUEUE_OPTIONS));
    m_hDataAvailableEvent = INVALID_HANDLE_VALUE;
    m_poFiFo = EC_NULL;

    RtosLibInit();
}

/***************************************************************************************************/
/**
\brief  Destructor of CMsgQueueRtosShm
*/
CMsgQueueRtosShm::~CMsgQueueRtosShm() 
{
    if (EC_INVALID_QUEUE_ID != m_dwId)
    {
        /* Queue was not closed! Closing now. */
        Close();
    }
    RtosLibDeinit();
}

/***************************************************************************************************/
/** 
\brief  Prepares the OS for Shared Memory Usage
*/
EC_T_DWORD CMsgQueueRtosShm::RtosLibInit(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
#if (defined _WINDOWS)
    EC_T_DWORD dwMutex  = 0;
    EC_T_CHAR  szMutexName[64];
    SECURITY_ATTRIBUTES SecurityAttributes;
    CHAR   achSecurityDesc[SECURITY_DESCRIPTOR_MIN_LENGTH];
#endif

    if (s_dwRtosLibInitCount == 0)
    {
        dwRes = ::RtosLibInit();
        if (dwRes != RTE_SUCCESS)
        {
            dwRetVal = EC_E_NOTFOUND;
            goto Exit;
        }

        dwRes = RtosCommStart();
        if (dwRes != RTE_SUCCESS)
        {
            dwRetVal = EC_E_NOTFOUND;
            goto Exit;
        }

#if (defined _WINDOWS)
        OsSnprintf(szMutexName, 64, TEXT("Global\\RtosLibApiSyncMutex%d"), dwMutex);

        /* init SECURITY_ATTRIBUTES */
        SecurityAttributes.nLength              = sizeof(SECURITY_ATTRIBUTES);
        SecurityAttributes.lpSecurityDescriptor = &achSecurityDesc;
        SecurityAttributes.bInheritHandle       = FALSE;
        InitializeSecurityDescriptor(SecurityAttributes.lpSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION);
        SetSecurityDescriptorDacl(SecurityAttributes.lpSecurityDescriptor, TRUE, 0, FALSE);

        s_hRtosLibApiSyncStartStopMutex = OpenMutexA( MUTEX_ALL_ACCESS, FALSE, TEXT("Global\\RtosLibApiSyncStartStopMutex"));
        if (NULL == s_hRtosLibApiSyncStartStopMutex)
        {
            /* Mutex missing! RtosLib version incorrect? */
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_ERROR;
            goto Exit;
        }
#endif /* _WINDOWS */
    }
    s_dwRtosLibInitCount++;

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/** 
\brief  Frees resources for Shared Memory Usage
*/
EC_T_DWORD CMsgQueueRtosShm::RtosLibDeinit(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
    
    if (s_dwRtosLibInitCount == 0)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    s_dwRtosLibInitCount--;

    if (s_dwRtosLibInitCount == 0)
    {
#if (defined _WINDOWS)
        /* close API sync mutex handle */
        if (NULL != s_hRtosLibApiSyncStartStopMutex)
        {
            if (!CloseHandle(s_hRtosLibApiSyncStartStopMutex))
            {
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_ERROR;
                goto Exit;
            }
            s_hRtosLibApiSyncStartStopMutex = NULL;
        }
#endif /* _WINDOWS */
        dwRes = RtosCommStop();
        if (dwRes != RTE_SUCCESS)
        {
            dwRetVal = EC_E_NOTFOUND;
            goto Exit;
        }

        /* resources for Shm freed by ::RtosLibDeinit() */
        if (s_bShmInitialized)
        {
            s_poMap->dwSignature = EC_T_RTOSSHM_SIGNATURE_UNINITIALIZED;
            s_bShmInitialized = EC_FALSE;
        }
        s_poMap = EC_NULL;

        dwRes = ::RtosLibDeinit();
        if (dwRes != RTE_SUCCESS)
        {
            dwRetVal = EC_E_NOTFOUND;
            goto Exit;
        }

#if (defined EC_SOCKET_SHM_SUPPORTED)
        if (!UnmapViewOfFile(m_poMap))
        {
            /* Unable to unmap memory region! */
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_ERROR;
            goto Exit;
        }
        m_poMap = EC_NULL;
        if (!CloseHandle(m_hMapFile))
        {
            /* Unable to close memory map! */
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_ERROR;
            goto Exit;
        }
        m_hMapFile = INVALID_HANDLE_VALUE;
#endif /* EC_SOCKET_SHM_SUPPORTED */
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/** 
\brief  Lock from Rtos Start/Stop
*/
EC_T_DWORD CMsgQueueRtosShm::RtosLibApiSyncEnter(EC_T_DWORD dwTimeout)
{
#if (defined _WINDOWS)
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    DWORD dwRes = NOERROR;

    if (0 == s_dwRtosLibInitCount)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    OsDbgAssert(NULL != s_hRtosLibApiSyncStartStopMutex);
    dwRes = WaitForSingleObject(s_hRtosLibApiSyncStartStopMutex, dwTimeout);
    switch (dwRes)
    {
    case WAIT_OBJECT_0:
    case WAIT_ABANDONED:
        {
            /* object signaled or abandoned */
        } break;
    case WAIT_TIMEOUT:
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        } break;
    default:
        {
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_ERROR;
            goto Exit;
        } break;
    }

    if (!RtosRunning())
    {
        RtosLibApiSyncExit();
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
#else /* _WINDOWS */
    EC_UNREFPARM(dwTimeout);
    return EC_E_NOERROR;
#endif /* _WINDOWS */
}

/***************************************************************************************************/
/** 
\brief  Unlock from Rtos Start/Stop
*/
EC_T_DWORD CMsgQueueRtosShm::RtosLibApiSyncExit(EC_T_VOID)
{
#if (defined _WINDOWS)
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (0 == s_dwRtosLibInitCount)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    OsDbgAssert(NULL != s_hRtosLibApiSyncStartStopMutex);
    if (!ReleaseMutex(s_hRtosLibApiSyncStartStopMutex))
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
#else /* _WINDOWS */
    return EC_E_NOERROR;
#endif /* _WINDOWS */
}

/***************************************************************************************************/
/** 
\brief  Changes reader/writer count with InterlockedCompareExchange
*/
EC_T_DWORD InterlockedCompareExchangedReaderWriterIncrement(
    EC_PT_WRITERREADERCOUNT pWriterReaderCount, EC_T_BOOL bReader, EC_T_BOOL bIncrement, EC_T_BOOL bOpen
    )
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;
    EC_T_DWORD  dwRes    = EC_E_ERROR;
    EC_T_BOOL   bExchanged = EC_FALSE;
    EC_T_WRITERREADERCOUNT WriterReaderCount    = *pWriterReaderCount;
    EC_T_WRITERREADERCOUNT NewWriterReaderCount = WriterReaderCount;

    if ((bReader && !bIncrement && (WriterReaderCount.s.wReaderCount == 0)) ||
       (!bReader && !bIncrement && (WriterReaderCount.s.wWriterCount == 0)))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if ( bReader &&  bIncrement) NewWriterReaderCount.s.wReaderCount++;
    if ( bReader && !bIncrement && (NewWriterReaderCount.s.wReaderCount > 0)) NewWriterReaderCount.s.wReaderCount--;
    if (!bReader &&  bIncrement) NewWriterReaderCount.s.wWriterCount++;
    if (!bReader && !bIncrement && (NewWriterReaderCount.s.wWriterCount > 0)) NewWriterReaderCount.s.wWriterCount--;

    while (!bExchanged)
    {
        /* check readers/writers count */
        if ((bOpen && bIncrement && bReader && (WriterReaderCount.s.wWriterCount == 0))
            || (bOpen && bIncrement && !bReader && (WriterReaderCount.s.wReaderCount != (EC_T_WORD)0x0001))
            || (!bOpen && bIncrement && ((WriterReaderCount.s.wWriterCount != 0) || (WriterReaderCount.s.wReaderCount != 0))))
        {
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }

        dwRes = RtosInterlockedCompareExchange(&pWriterReaderCount->dwData, NewWriterReaderCount.dwData, WriterReaderCount.dwData, &WriterReaderCount.dwData);
        if (RTE_SUCCESS != dwRes)
        {
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_ERROR;
            goto Exit;
        }

        if ((bReader && bIncrement && (WriterReaderCount.s.wReaderCount + 1 == NewWriterReaderCount.s.wReaderCount))
        || (bReader && !bIncrement && (WriterReaderCount.s.wReaderCount == NewWriterReaderCount.s.wReaderCount + 1))
        || (!bReader && bIncrement && (WriterReaderCount.s.wWriterCount + 1 == NewWriterReaderCount.s.wWriterCount))
        || (!bReader && !bIncrement && (WriterReaderCount.s.wWriterCount == NewWriterReaderCount.s.wWriterCount + 1))
        || (bReader && !bIncrement && (WriterReaderCount.s.wReaderCount == 0))
        || (!bReader && !bIncrement && (WriterReaderCount.s.wWriterCount == 0))
            )
        {
            bExchanged = EC_TRUE;
        }
        else
        {
            OsSleep(10);
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Opens an existing msg queue from RtosShm. See also Close() to free the resources.
*/
EC_T_DWORD CMsgQueueRtosShm::Open(EC_T_CHAR* szMsgQueueNameFormat, EC_T_BOOL bReadAccess, EC_T_DWORD dwMaxMessage, EC_T_DWORD dwMaxMessages, EC_T_DWORD dwId)
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;
    EC_T_DWORD  dwRes    = EC_E_ERROR;
    EC_T_BOOL   bRtosLibApiSyncEntered  = EC_FALSE;
    EC_T_BOOL   bQueueOpened            = EC_FALSE;

    if ((dwMaxMessage > MSGQUEUE_MSG_SIZE) || (dwMaxMessages > MSGQUEUE_MAX_MESSAGES))
    {
        dwRetVal = EC_E_INVALIDSIZE;
        goto Exit;
    }

    if (m_bOpened || m_bCreated)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* map Shm should be done once per process and only if needed */
    if (EC_NULL == s_poMap)
    {
        dwRes = MapShm();
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }

    /* lock Shm for allocation */
    dwRes = RtosLibApiSyncEnter(EC_SOCKET_RTOSSHM_API_SYNC_TIMEOUT);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    bRtosLibApiSyncEntered = EC_TRUE;
    if (EC_NULL == s_poMap)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* check if Shm is initialized */
    if (EC_T_RTOSSHM_SIGNATURE_INITIALIZED != s_poMap->dwSignature)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* open the queue */
    dwRes = InterlockedCompareExchangedReaderWriterIncrement(&s_poMap->Queue[dwId].WriterReaderCount, bReadAccess, EC_TRUE, EC_TRUE);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    bQueueOpened = EC_TRUE;

    /* create fifo-list */
#if (defined DEBUGTRACE)
    m_poFiFo = EC_NEW(CFiFoList<EC_T_DWORD>(&s_poMap->Queue[dwId].FifoDesc, s_poMap->Queue[dwId].adwFifoData, EC_NULL, szMsgQueueNameFormat, 0));
#else
    m_poFiFo = EC_NEW(CFiFoList<EC_T_DWORD>(&s_poMap->Queue[dwId].FifoDesc, s_poMap->Queue[dwId].adwFifoData, EC_NULL, szMsgQueueNameFormat));
#endif /* DEBUGTRACE */
    if (EC_NULL == m_poFiFo)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    /* create event */
    dwRes = CreateEvent(dwId);
    if (EC_E_NOERROR != dwRes)
    {
        /* CreateEvent failed! */
        dwRetVal = dwRes;
        goto Exit;
    }

    /* all resources allocated */
    m_oMsgQueueOptions.bReadAccess = bReadAccess;
    m_oMsgQueueOptions.cbMaxMessage = dwMaxMessage;
    m_oMsgQueueOptions.dwMaxMessages = dwMaxMessages;
    m_dwId      = dwId;
    m_bOpened   = EC_TRUE;

    dwRetVal = EC_E_NOERROR;
Exit:
    if ((EC_E_NOERROR != dwRetVal) && bQueueOpened)
    {
        InterlockedCompareExchangedReaderWriterIncrement(&s_poMap->Queue[dwId].WriterReaderCount, bReadAccess, EC_FALSE, EC_TRUE);
    }

    if (bRtosLibApiSyncEntered)
    {
        dwRes = CMsgQueueRtosShm::RtosLibApiSyncExit();
        bRtosLibApiSyncEntered = EC_FALSE;
        if (EC_E_NOERROR != dwRes)
        {
            if (EC_E_NOERROR == dwRetVal)
            {
                dwRetVal = dwRes;
            }
        }
    }

    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Creates a new msg queue in RtosShm. See also Close() to free the resources.
*/
EC_T_DWORD CMsgQueueRtosShm::Create(EC_T_CHAR* szMsgQueueNameFormat, EC_T_BOOL bReadAccess, EC_T_DWORD dwMaxMessage, EC_T_DWORD dwMaxMessages, EC_T_DWORD dwId/* = EC_INVALID_QUEUE_ID*/)
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;
    EC_T_DWORD  dwRes    = EC_E_ERROR;
    EC_T_BOOL   bRtosLibApiSyncEntered  = EC_FALSE;
    EC_T_BOOL   bQueueOpened            = EC_FALSE;
    EC_T_DWORD  dwSearchID              = 0;
    EC_T_DWORD  dwShmSegmentIndex       = 0;
    EC_T_DWORD  dwSignature;

    if ((dwMaxMessage > MSGQUEUE_MSG_SIZE) || (dwMaxMessages > MSGQUEUE_MAX_MESSAGES))
    {
        dwRetVal = EC_E_INVALIDSIZE;
        goto Exit;
    }

    if (m_bOpened || m_bCreated)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* map Shm should be done once per process and only if needed */
    if (EC_NULL == s_poMap)
    {
        dwRes = MapShm();
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }

    /* lock Shm for allocation */
    dwRes = RtosLibApiSyncEnter(EC_SOCKET_RTOSSHM_API_SYNC_TIMEOUT);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    bRtosLibApiSyncEntered = EC_TRUE;
    if (EC_NULL == s_poMap)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

#if (RTOSLIB_API_VERSION >= 50)
    /* Note: UserShm is NOT initialized (at least not before RtosLib API 5.0) */
    /* initialize Shm if dwId = 0 -> (error if already initialized) */
    if ((EC_T_RTOSSHM_SIGNATURE_INITIALIZED != s_poMap->dwSignature) && (0 != dwId)) 
    {
        /* Invalid combination of s_poMap->dwSignature and dwId! */
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
#endif
    if (0 == dwId)
    {
        dwRes = RtosInterlockedCompareExchange(&s_poMap->dwSignature, EC_T_RTOSSHM_SIGNATURE_INITIALIZING, EC_T_RTOSSHM_SIGNATURE_UNINITIALIZED, &dwSignature);
        if (RTE_SUCCESS != dwRes)
        {
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }

        /* initialize Shm */
        OsMemset(((EC_T_PBYTE)s_poMap) + sizeof(EC_T_DWORD), 0, sizeof(EC_T_RTOSSHM) - sizeof(EC_T_DWORD));

        /* mark Shm initialized */
        dwRes = RtosInterlockedCompareExchange(&s_poMap->dwSignature, EC_T_RTOSSHM_SIGNATURE_INITIALIZED, EC_T_RTOSSHM_SIGNATURE_INITIALIZING, &dwSignature);
        if (RTE_SUCCESS != dwRes)
        {
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_ERROR;
            goto Exit;
        }
        s_bShmInitialized = EC_TRUE;
    }
    OsDbgAssert(EC_T_RTOSSHM_SIGNATURE_INITIALIZED == s_poMap->dwSignature);

    /* search for free TX-Queue if no valid dwId was provided */
    while (!bQueueOpened)
    {
        if (EC_INVALID_QUEUE_ID == dwId)
        {
            for (dwSearchID = 1; dwSearchID < 2 * EC_SOCKET_RTOSSHM_MAX_CLIENTS; dwSearchID = dwSearchID + 2)
            {
                if ((0 == s_poMap->Queue[dwSearchID].WriterReaderCount.s.wWriterCount) && (0 == s_poMap->Queue[dwSearchID].WriterReaderCount.s.wReaderCount))
                {
                    dwId = dwSearchID;
                    break;
                }
            }
        }
        if (EC_INVALID_QUEUE_ID == dwId)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }

        /* open the queue */
        dwRes = InterlockedCompareExchangedReaderWriterIncrement(&s_poMap->Queue[dwId].WriterReaderCount, bReadAccess, EC_TRUE, EC_FALSE);
        if (EC_E_NOERROR != dwRes)
        {
            OsSleep(10);
            continue;
        }
        bQueueOpened = EC_TRUE;
    }

    s_poMap->Queue[dwId].FifoDesc.nFirst = 0;
    s_poMap->Queue[dwId].FifoDesc.nLast = 0;
    s_poMap->Queue[dwId].FifoDesc.nSize = dwMaxMessages;
    

    /* create fifo-list */
#if (defined DEBUGTRACE)
    m_poFiFo = EC_NEW(CFiFoList<EC_T_DWORD>(&s_poMap->Queue[dwId].FifoDesc, s_poMap->Queue[dwId].adwFifoData, EC_NULL, szMsgQueueNameFormat, 0));
#else
    m_poFiFo = EC_NEW(CFiFoList<EC_T_DWORD>(&s_poMap->Queue[dwId].FifoDesc, s_poMap->Queue[dwId].adwFifoData, EC_NULL, szMsgQueueNameFormat));
#endif /* DEBUGTRACE */

    if (EC_NULL == m_poFiFo)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    /* create event */
    dwRes = CreateEvent(dwId);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    /* all resources allocated */
    m_oMsgQueueOptions.bReadAccess = bReadAccess;
    m_oMsgQueueOptions.cbMaxMessage = dwMaxMessage;
    m_oMsgQueueOptions.dwMaxMessages = dwMaxMessages;

    for (dwShmSegmentIndex = 0; dwShmSegmentIndex < m_oMsgQueueOptions.dwMaxMessages; dwShmSegmentIndex++)
    {
        /* free message entry */
        s_poMap->Queue[dwId].Message[dwShmSegmentIndex].dwLen = 0;
    }

    m_dwId      = dwId;
    m_bCreated  = EC_TRUE;

    dwRetVal    = EC_E_NOERROR;
Exit:
    if ((EC_E_NOERROR != dwRetVal) && bQueueOpened)
    {
        InterlockedCompareExchangedReaderWriterIncrement(&s_poMap->Queue[dwId].WriterReaderCount, bReadAccess, EC_FALSE, EC_FALSE);
    }

    if (bRtosLibApiSyncEntered)
    {
        dwRes = CMsgQueueRtosShm::RtosLibApiSyncExit();
        bRtosLibApiSyncEntered = EC_FALSE;
        if (EC_E_NOERROR != dwRes)
        {
            if (EC_E_NOERROR == dwRetVal)
            {
                dwRetVal = dwRes;
            }
        }
    }

    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Check if currently at least one reader and one writer has opened or created the queue.
*/
EC_T_BOOL  CMsgQueueRtosShm::IsConnected(EC_T_VOID)
{
    if ((EC_NULL == s_poMap) || (EC_INVALID_QUEUE_ID == m_dwId))
    {
            return EC_E_INVALIDSTATE;
    }
    return ((0 > s_poMap->Queue[m_dwId].WriterReaderCount.s.wWriterCount) 
        && (0 > s_poMap->Queue[m_dwId].WriterReaderCount.s.wReaderCount));
}

/***************************************************************************************************/
/**
\brief  Closes an opened or created msg queue in RtosShm. See also Open(...), Create(...).
*/
EC_T_DWORD CMsgQueueRtosShm::Close(EC_T_VOID) 
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
    EC_T_BOOL  bRtosLibApiSyncEntered = EC_FALSE;

    if ((EC_INVALID_QUEUE_ID == m_dwId) || (!m_bOpened && !m_bCreated))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRes = CMsgQueueRtosShm::RtosLibApiSyncEnter(EC_SOCKET_RTOSSHM_API_SYNC_TIMEOUT);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    bRtosLibApiSyncEntered = EC_TRUE;
    OsDbgAssert(EC_NULL != s_poMap);

    if (INVALID_HANDLE_VALUE != m_hDataAvailableEvent)
    {
        /* close Rtos event */
        dwRes = RtosCloseEvent(m_hDataAvailableEvent);
        if( dwRes != RTE_SUCCESS )
        {
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_ERROR;
            goto Exit;
        }
        m_hDataAvailableEvent = INVALID_HANDLE_VALUE;
    }

    OsDbgAssert(EC_NULL != m_poFiFo);
    SafeDelete(m_poFiFo);

    /* close the queue */
    dwRes = InterlockedCompareExchangedReaderWriterIncrement(&s_poMap->Queue[m_dwId].WriterReaderCount, m_oMsgQueueOptions.bReadAccess, EC_FALSE, EC_FALSE);
    m_dwId = EC_INVALID_QUEUE_ID;
    m_bOpened = EC_FALSE;
    m_bCreated = EC_FALSE;
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if (EC_E_INVALIDSTATE == dwRes)
    {
        /* invalidate states */
        m_dwId = EC_INVALID_QUEUE_ID;
        m_bOpened = EC_FALSE;
        m_bCreated = EC_FALSE;
    }

    if (bRtosLibApiSyncEntered)
    {
        dwRes = CMsgQueueRtosShm::RtosLibApiSyncExit();
        bRtosLibApiSyncEntered = EC_FALSE;
        if (EC_E_NOERROR != dwRes)
        {
            if (EC_E_NOERROR == dwRetVal)
            {
                dwRetVal = dwRes;
            }
        }
    }

    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Reads a single msg.
*/
EC_T_DWORD CMsgQueueRtosShm::Read(EC_T_BYTE* pbyBuffer, EC_T_DWORD* pdwNumberOfBytesRead, EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
    EC_T_BOOL  bRtosLibApiSyncEntered = EC_FALSE;
    EC_T_DWORD dwNumberOfBytesRead = 0;
    EC_T_MSGQUEUE_MSG* poMessage = EC_NULL;
    CEcTimer oTimeout;
    EC_T_DWORD dwMsgId = 0;
#if (defined EC_MSGQUEUE_TRACESUPPORT)
    EC_T_DWORD dwIdx = 0;
    static EC_T_CHAR achMsgDump[MSGQUEUE_MSG_SIZE * 3 + sizeof("\n\n") + 1];
#endif /* EC_MSGQUEUE_TRACESUPPORT */
    if ((EC_NULL == pdwNumberOfBytesRead) || (0 == *pdwNumberOfBytesRead))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwRes = CMsgQueueRtosShm::RtosLibApiSyncEnter(EC_SOCKET_RTOSSHM_API_SYNC_TIMEOUT);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    bRtosLibApiSyncEntered = EC_TRUE;

    if (EC_NULL == m_poFiFo)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (0 == m_poFiFo->GetSize())
    {
        /* happens if server was restarted without announcement */
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* wait for data if needed */
    if ((m_poFiFo->GetCount() == 0) && (EC_NOWAIT != dwTimeout))
    {
        oTimeout.Start(dwTimeout);

        while (m_poFiFo->GetCount() == 0)
        {
            dwRes = CMsgQueueRtosShm::RtosLibApiSyncExit();
            bRtosLibApiSyncEntered = EC_FALSE;
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }

            dwRes = RtosWaitForEvent(m_hDataAvailableEvent, dwTimeout);
            switch (dwRes)
            {
            case RTE_SUCCESS:
                break;
            case RTE_ERROR:
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_ERROR;
                goto Exit;
                break;
            case RTE_ERROR_TIMEOUT:
                dwRetVal = EC_E_TIMEOUT;
                goto Exit;
                break;
            default:
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_ERROR;
                goto Exit;
            }

            dwRes = CMsgQueueRtosShm::RtosLibApiSyncEnter(EC_SOCKET_RTOSSHM_API_SYNC_TIMEOUT);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            bRtosLibApiSyncEntered = EC_TRUE;

            if (oTimeout.IsElapsed() && (m_poFiFo->GetCount() == 0))
            {
                dwRetVal = EC_E_TIMEOUT;
                goto Exit;
            }
        }
    }
    /* data available? */
    if (m_poFiFo->GetCount() == 0)
    {
        OsDbgAssert(EC_NOWAIT == dwTimeout);
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    }

    m_poFiFo->PeakNoLock(dwMsgId);
    poMessage = &s_poMap->Queue[m_dwId].Message[dwMsgId];

    /* read at most requested data amount */
    dwNumberOfBytesRead = EC_MIN((EC_T_WORD)(*pdwNumberOfBytesRead), poMessage->dwLen);
    OsDbgAssert(dwNumberOfBytesRead > 0);
    OsMemcpy(pbyBuffer, poMessage->abyBuffer, dwNumberOfBytesRead);
    poMessage->dwLen = 0;

    m_poFiFo->RemoveNoLock(dwMsgId);

    dwRes = CMsgQueueRtosShm::RtosLibApiSyncExit();
	bRtosLibApiSyncEntered = EC_FALSE;
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if (bRtosLibApiSyncEntered)
    {
        dwRes = CMsgQueueRtosShm::RtosLibApiSyncExit();
    	bRtosLibApiSyncEntered = EC_FALSE;
        if (EC_E_NOERROR != dwRes)
        {
            if (EC_E_NOERROR == dwRetVal)
            {
                dwRetVal = dwRes;
            }
        }
    }

    if ((EC_E_NOERROR == dwRetVal) && (EC_NULL != pdwNumberOfBytesRead))
    {
        *pdwNumberOfBytesRead = dwNumberOfBytesRead;
#if (defined EC_MSGQUEUE_TRACESUPPORT)
        for (dwIdx = 0; dwIdx < dwNumberOfBytesRead; dwIdx++)
        {
            sprintf(&achMsgDump[dwIdx * 3], "%02X ", pbyBuffer[dwIdx]);
        }
        OsDbgMsg("CMsgQueueRtosShm::Read(...): Read %d bytes from Queue ID %d. Dump: %s\n\n", dwNumberOfBytesRead, m_dwId, achMsgDump);
#endif /* EC_MSGQUEUE_TRACESUPPORT */
    }
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Writes a single msg.
*/
EC_T_DWORD CMsgQueueRtosShm::Write(EC_T_BYTE* pbyBuffer, EC_T_DWORD dwDataSize, EC_T_DWORD dwTimeout) 
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
    EC_T_BOOL  bRtosLibApiSyncEntered = EC_FALSE;
    EC_T_DWORD dwShmSegmentIndex;
    EC_T_MSGQUEUE_MSG* poMessage = EC_NULL;

    if (EC_NULL == m_poFiFo)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if ((EC_NULL == pbyBuffer) || (0 == dwDataSize) || (dwDataSize > m_oMsgQueueOptions.cbMaxMessage))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwRes = CMsgQueueRtosShm::RtosLibApiSyncEnter(EC_SOCKET_RTOSSHM_API_SYNC_TIMEOUT);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    bRtosLibApiSyncEntered = EC_TRUE;

    /* wait for free entry if needed */
    if (EC_WAITINFINITE == dwTimeout)
    {
        while (m_poFiFo->IsFull())
        {
            dwRes = CMsgQueueRtosShm::RtosLibApiSyncExit();
	        bRtosLibApiSyncEntered = EC_FALSE;
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            OsSleep(10);
            dwRes = CMsgQueueRtosShm::RtosLibApiSyncEnter(EC_SOCKET_RTOSSHM_API_SYNC_TIMEOUT);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            bRtosLibApiSyncEntered = EC_TRUE;
        }
    } 
    else if (EC_NOWAIT != dwTimeout)
    {
        if (m_poFiFo->IsFull())
        {
            dwRes = CMsgQueueRtosShm::RtosLibApiSyncExit();
	        bRtosLibApiSyncEntered = EC_FALSE;
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            OsSleep(dwTimeout);
            dwRes = CMsgQueueRtosShm::RtosLibApiSyncEnter(EC_SOCKET_RTOSSHM_API_SYNC_TIMEOUT);
            if (EC_E_NOERROR != dwRes)
            {
                dwRetVal = dwRes;
                goto Exit;
            }
            bRtosLibApiSyncEntered = EC_TRUE;
        }
    }
    if (m_poFiFo->IsFull())
    {
        if (EC_NOWAIT != dwTimeout)
        {
            dwRetVal = EC_E_TIMEOUT;
        }
        else
        {
            dwRetVal = EC_E_BUSY;
        }
        goto Exit;
    }

    /* search a free message entry */
    for (dwShmSegmentIndex = 0; dwShmSegmentIndex < m_oMsgQueueOptions.dwMaxMessages; dwShmSegmentIndex++)
    {
        poMessage = &s_poMap->Queue[m_dwId].Message[dwShmSegmentIndex];
        if (0 == poMessage->dwLen)
        {
            /* lock message entry */
            poMessage->dwLen = (EC_T_WORD)dwDataSize;
            break;
        }
    }
    /* unable to find a free message entry? */
    if (dwShmSegmentIndex == m_oMsgQueueOptions.dwMaxMessages)
    {
        /* this may happen if 
           - server was restarted without announcement OR
           - there are multiple writers to the queue */
        OsDbgAssert((0 == m_poFiFo->GetSize()) || 
            (s_poMap->Queue[m_dwId].WriterReaderCount.s.wWriterCount > 1)); 
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    OsDbgAssert(EC_NULL != poMessage);
    OsDbgAssert(poMessage->dwLen == dwDataSize);

    /* copy payload to Shm */
    OsMemcpy(poMessage->abyBuffer, pbyBuffer, dwDataSize);
    m_poFiFo->Add(dwShmSegmentIndex);

    dwRes = CMsgQueueRtosShm::RtosLibApiSyncExit();
	bRtosLibApiSyncEntered = EC_FALSE;
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    /* signal reader that data is available */
    dwRes = RtosSetEvent(m_hDataAvailableEvent);
    if (RTE_SUCCESS != dwRes)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if (bRtosLibApiSyncEntered)
    {
        dwRes = CMsgQueueRtosShm::RtosLibApiSyncExit();
    	bRtosLibApiSyncEntered = EC_FALSE;
        if (EC_E_NOERROR != dwRes)
        {
            if (EC_E_NOERROR == dwRetVal)
            {
                dwRetVal = dwRes;
            }
        }
    }

    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Gets the currently connected reader count. 
*/
EC_T_DWORD CMsgQueueRtosShm::GetReaderCount(EC_T_DWORD* pdwReaderCount)
{
    if (EC_INVALID_QUEUE_ID == m_dwId)
    {
        return EC_E_INVALIDSTATE;
    }
    *pdwReaderCount = s_poMap->Queue[m_dwId].WriterReaderCount.s.wReaderCount;
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Gets the currently connected writer count. 
*/
EC_T_DWORD CMsgQueueRtosShm::GetWriterCount(EC_T_DWORD* pdwWriterCount)
{
    if (EC_INVALID_QUEUE_ID == m_dwId)
    {
        return EC_E_INVALIDSTATE;
    }
    *pdwWriterCount = s_poMap->Queue[m_dwId].WriterReaderCount.s.wWriterCount;
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Get a pointer to the Shm (s_poMap). Allocated ressources are freed by RtosLibDeinit()
*/
EC_T_DWORD CMsgQueueRtosShm::MapShm(EC_T_VOID)
{
EC_T_DWORD   dwRetVal = EC_E_ERROR;
UINT32       dwRes    = 0;
UINT32       dwReqSize = sizeof(EC_T_RTOSSHM);
EC_T_VOID*   pvShmAddr = EC_NULL;
#if (defined EC_SOCKET_VMFSHM_SUPPORTED) // TODO: wird das unterstützt?
VMF_SHM_INFO ShmInfo = {0};

int nResult;
#else
UINT32       dwShmSize;
#endif
EC_T_BOOL    bRtosLibApiSyncEntered = EC_FALSE;

#if( RTOSLIB_API_VERSION >= 50 )
UINT32       dwShmId;
#endif /* RTOSLIB_API_VERSION >= 50 */

    if (EC_NULL != s_poMap)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }

    dwRes = RtosLibApiSyncEnter(EC_SOCKET_RTOSSHM_API_SYNC_TIMEOUT);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    bRtosLibApiSyncEntered = EC_TRUE;

#if( RTOSLIB_API_VERSION >= 50 )
    /* RTOSLIB_API_VERSION >= 50 needs correct user shared memory name */
    dwRes = RtosGetIdByName(TEXT(EC_SOCKET_RTOSSHM_NAME), RTOS_ID_SHM, &dwShmId);
    if( dwRes != RTE_SUCCESS )
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
#endif

	dwShmSize = 0;
    pvShmAddr = NULL;

	dwRes = RtosShmAddrGet(
#if( RTOSLIB_API_VERSION >= 50 )
							dwShmId,
#endif
							dwReqSize,
							&dwShmSize,
							&pvShmAddr
							);
    if( dwRes != RTE_SUCCESS )
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    if( sizeof(EC_T_RTOSSHM) != dwShmSize )
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }

    s_poMap = (EC_T_RTOSSHM *)pvShmAddr;
#if (defined EC_SOCKET_VMFSHM_SUPPORTED)
    dwRes = vmfIdGetByName(TEXT(EC_SOCKET_RTOSSHM_NAME),  VMF_ID_SHM, &dwShmId);
    if( dwRes != RTE_SUCCESS )
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    ShmInfo.dwSize = sizeof(VMF_SHM_INFO);

    /* get infomation about reserved Shm */
    dwRes = vmfShmGetInfo( dwShmId, &ShmInfo );
    if( dwRes != RTE_SUCCESS )
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }

    /* reserve and map shared memory, virtual == physical */
    pvMapBase = (EC_T_VOID*)ShmInfo.qwMemoryBasePhysical;
    nResult = RTReserveVirtualAddress( &pvMapBase, ShmInfo.dwMemorySize, 0);
    if( nResult != RT_MAP_SUCCESS )
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    nResult = RTMapMem( (const EC_T_VOID*)ShmInfo.qwMemoryBasePhysical, (const EC_T_VOID*)pvMapBase, ShmInfo.dwMemorySize, RT_PG_USERREADWRITE);
    if( nResult != RT_MAP_SUCCESS )
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    m_poMap = (EC_T_RTOSSHM *)pvMapBase;
#endif /* EC_SOCKET_VMFSHM_SUPPORTED */

#if (defined EC_SOCKET_SHM_SUPPORTED)
    /* create/connect to Shm */
    m_hMapFile = CreateFileMapping(
             INVALID_HANDLE_VALUE,    // use paging file
             NULL,                    // default security
             PAGE_READWRITE,          // read/write access
             0,                       // maximum object size (high-order DWORD)
             sizeof(EC_T_RTOSSHM),   // maximum object size (low-order DWORD)
             EC_SOCKET_RTOSSHM_NAME);// name of mapping object

    m_poMap = (EC_T_RTOSSHM*) MapViewOfFile(m_hMapFile,   // handle to map object
                    FILE_MAP_ALL_ACCESS, // read/write permission
                    0,
                    0,
                    sizeof(EC_T_RTOSSHM));
#endif

    dwRetVal = EC_E_NOERROR;
Exit:
    if (bRtosLibApiSyncEntered)
    {
        dwRes = CMsgQueueRtosShm::RtosLibApiSyncExit();
        bRtosLibApiSyncEntered = EC_FALSE;
        if (EC_E_NOERROR != dwRes)
        {
            if (EC_E_NOERROR == dwRetVal)
            {
                dwRetVal = dwRes;
            }
        }
    }

    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Creates the inter-OS synchronization event to signal data available.
*/
EC_T_DWORD CMsgQueueRtosShm::CreateEvent(EC_T_DWORD dwId)
{
    EC_T_CHAR  szResourceName[256];
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    /* event may not be set */
    if (INVALID_HANDLE_VALUE != m_hDataAvailableEvent)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    OsSnprintf(szResourceName, 255, TEXT("Global\\RtosShmMsgQueue_%d_Event"), dwId);
    szResourceName[255] = '\0';
    dwRes = RtosCreateEvent(FALSE, FALSE, szResourceName, &m_hDataAvailableEvent);
    if (RTE_SUCCESS != dwRes)
    {
        m_hDataAvailableEvent = INVALID_HANDLE_VALUE;
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Constructor of CEcSocketRtosShm
*/
CEcSocketRtosShm::CEcSocketRtosShm()
{ 
    m_poRxMsgQueue      = EC_NULL;
    m_poTxMsgQueue      = EC_NULL;
    m_poLogonQueue      = EC_NULL;
    m_eType	            = emrassocktype_rtosshm;
}

/***************************************************************************************************/
/**
\brief  Destructor of CEcSocketRtosShm
*/
CEcSocketRtosShm::~CEcSocketRtosShm() 
{
}

/***************************************************************************************************/
/**
\brief  Creates new communication queues. Determines queue IDs. 
\return EC_E_NOERROR on success
\return EC_E_INVALIDSTATE if RAM exceeded before
\return EC_E_NOMEMORY if RAM or EC_SOCKET_RTOSSHM_MAX_CLIENTS connections exceeded.
\return Passes return code of CMsgQueueRtosShm::Create on error.
*/
EC_T_DWORD CEcSocketRtosShm::CreateCommunicationQueues(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
    EC_T_BOOL  bMsgQueueObjectsCreated = EC_FALSE;

    if ((EC_NULL != m_poRxMsgQueue) || (EC_NULL != m_poTxMsgQueue))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    m_poRxMsgQueue = EC_NEW(CMsgQueueRtosShm());
    m_poTxMsgQueue = EC_NEW(CMsgQueueRtosShm());
    bMsgQueueObjectsCreated = EC_TRUE;
    if ((EC_NULL == m_poRxMsgQueue) || (EC_NULL == m_poTxMsgQueue))
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    dwRes = m_poTxMsgQueue->Create(DATA_QUEUE_CLIENT_TO_SERVER_NAME_FORMAT, EC_FALSE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, EC_INVALID_QUEUE_ID);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    dwRes = m_poRxMsgQueue->Create(DATA_QUEUE_SERVER_TO_CLIENT_NAME_FORMAT, EC_TRUE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, m_poTxMsgQueue->GetId() + 1);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    if ((EC_E_NOERROR != dwRetVal) && bMsgQueueObjectsCreated)
    {
        SafeDelete(m_poRxMsgQueue);
        SafeDelete(m_poTxMsgQueue);
    }

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Creates logon queue. Fails e.g. if server already started.
\return EC_E_NOERROR on success
\return EC_E_INVALIDSTATE if RAM exceeded before
\return EC_E_NOMEMORY if RAM exceeded.
\return Passes return code of CMsgQueueRtosShm::Create, e.g. EC_E_INVALIDSTATE on error.
*/
EC_T_DWORD CEcSocketRtosShm::CreateLogonQueue(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;

    if (EC_NULL != m_poLogonQueue)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    m_poLogonQueue = new CMsgQueueRtosShm();
    if (EC_NULL == m_poLogonQueue)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRes = m_poLogonQueue->Create((EC_T_CHAR*)LOGON_QUEUE_NAME, EC_TRUE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, 0);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Opens existing communication queues.
*/
EC_T_DWORD CEcSocketRtosShm::OpenMsgQueues(EC_T_DWORD dwClientToServerQueueId)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
    if ((EC_NULL != m_poRxMsgQueue) || (EC_NULL != m_poTxMsgQueue))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    m_poRxMsgQueue = EC_NEW(CMsgQueueRtosShm());
    m_poTxMsgQueue = EC_NEW(CMsgQueueRtosShm());
    if ((EC_NULL == m_poRxMsgQueue) || (EC_NULL == m_poTxMsgQueue))
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRes = m_poTxMsgQueue->Open(DATA_QUEUE_SERVER_TO_CLIENT_NAME_FORMAT, EC_FALSE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, dwClientToServerQueueId + 1);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }

    dwRes = m_poRxMsgQueue->Open(DATA_QUEUE_CLIENT_TO_SERVER_NAME_FORMAT, EC_TRUE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, dwClientToServerQueueId);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }

    m_bDisc = EC_FALSE;

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Opens existing logon queue.
*/
EC_T_DWORD CEcSocketRtosShm::OpenLogonQueue(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes    = EC_E_ERROR;
    EC_T_BOOL  bLogonQueueObjectCreated = EC_FALSE;

    if (EC_NULL != m_poLogonQueue)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    m_poLogonQueue = new CMsgQueueRtosShm();
    if (EC_NULL == m_poLogonQueue)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    bLogonQueueObjectCreated = EC_TRUE;

    dwRes = m_poLogonQueue->Open((EC_T_CHAR*)LOGON_QUEUE_NAME, EC_FALSE, MSGQUEUE_MSG_SIZE, MSGQUEUE_MAX_MESSAGES, 0);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    if ((EC_E_NOERROR != dwRetVal) && bLogonQueueObjectCreated)
    {
        SafeDelete(m_poLogonQueue);
    }

    return dwRetVal;
}
#endif /* EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED */
/*-END OF SOURCE FILE--------------------------------------------------------*/
