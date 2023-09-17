/*-----------------------------------------------------------------------------
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              Thread class implementation
 *---------------------------------------------------------------------------*/

#include "EcOs.h"
#include "EcThread.h"
#include "EcTimer.h"

#include "EcError.h"

CEcThread::CEcThread()
    : m_pfThreadEntry(EC_NULL)
    , m_pvParams(EC_NULL)
	, m_hThread(EC_NULL)
    , m_bThreadStop(EC_FALSE)
    , m_bThreadReady(EC_FALSE)
{
}

CEcThread::~CEcThread()
{
    /* thread should be stopped */
    OsDbgAssert(isStopped());
    
    OsDbgAssert(m_hThread == EC_NULL);
}

EC_T_DWORD CEcThread::Start(EC_PF_THREADENTRY pfThreadEntry, EC_T_VOID* pvParams, const EC_T_CHAR* szThreadName, 
                            EC_T_DWORD dwPrio, EC_T_DWORD dwStackSize, EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    CEcTimer startTimeout; 

    setThreadProc(pfThreadEntry, pvParams);

    m_bThreadStop = EC_FALSE;
    m_hThread = OsCreateThread((EC_T_CHAR*)szThreadName, (EC_PF_THREADENTRY)threadProc,
                               dwPrio, dwStackSize, this);

    if (EC_NULL == m_hThread)
    {
        /* reset thread entry */
        setThreadProc(EC_NULL, EC_NULL);

        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    for (startTimeout.Start(dwTimeout); !startTimeout.IsElapsed(); )
    {
        if (isReady()) 
        {
            break;
        }
        OsSleep(1);
    }
    if (!isReady())
    {
        dwRetVal = EC_E_TIMEOUT;
        EC_DBGMSG("Time-out starting thread %s!\n", szThreadName);
    }

Exit:
    return dwRetVal;
}

EC_T_DWORD CEcThread::Stop(EC_T_DWORD dwTimeout)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    CEcTimer stopTimeout;

    stopThread();

    if (EC_NOWAIT != dwTimeout)
    {
        for (stopTimeout.Start(dwTimeout); !stopTimeout.IsElapsed(); OsSleep(1))
        {
            if (isStopped()) break;
        }
        if (!isStopped())
        {
            dwRetVal = EC_E_TIMEOUT;
            EC_DBGMSG("Time-out stopping thread!\n");
        }
    }

    if (m_hThread)
    {
        OsDeleteThreadHandle(m_hThread);
    }
    m_hThread = EC_NULL;

    return dwRetVal;
}

EC_T_DWORD CEcThread::threadProc(EC_T_PVOID pvParams)
{
    CEcThread* pThis = (CEcThread*)pvParams;
    pThis->m_bThreadReady = EC_TRUE;
    
    while (!pThis->m_bThreadStop)
    {
        pThis->threadStep();
    }

    /* indicate that stopped */
    pThis->setThreadProc(EC_NULL, EC_NULL);

    return 0;
}

EC_T_VOID CEcThread::threadStep(EC_T_VOID)
{
    m_pfThreadEntry(m_pvParams);
}

