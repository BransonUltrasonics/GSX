/*-----------------------------------------------------------------------------
 * Copyright    acontis technologies GmbH, Weingarten, Germany
 * Response     Paul Bussmann
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "EcOs.h"
#include "EcLock.h"
#include "EcTimer.h"
#include "AtEmRasError.h"

/*-DEFINES-------------------------------------------------------------------*/
/*#define DEBUG_LOCKS     1*/

/*-FUNCTIONS-----------------------------------------------------------------*/
CEcSharedLock::CEcSharedLock()
{
    m_pvLock            = OsCreateLock();
    m_bLockedExclusive  = EC_FALSE;
    m_dwExclusiveLockReqCount = 0;
    m_dwSharedLockCount = 0;
#if (defined _DEBUG)
    m_dwExLockThreadId  = 0;
#endif
}

CEcSharedLock::~CEcSharedLock()
{
    OsDeleteLock(m_pvLock);
    m_pvLock = EC_NULL;
    OsDbgAssert(EC_FALSE==m_bLockedExclusive);
    OsDbgAssert(0==m_dwExclusiveLockReqCount);
    OsDbgAssert(0==m_dwSharedLockCount);
}

/***************************************************************************************************/
/**
\brief  Acquire Lock.

\return EC_E_NOERROR on success
        EC_E_TIMEOUT if called with given timeout value and lock could not be acquired
        EC_E_BUSY    if called with EC_NOWAIT and lock could not be acquired
*/
EC_T_DWORD CEcSharedLock::Lock(
    EC_T_DWORD  dwTimeout,      /**< [in]   Timeout to acquire Lock (EC_WAITINFINITE = wait forever) */
    EC_T_BOOL   bExclusive      /**< [in]   EC_TRUE: Acquire excl. Lock, EC_FALSE: Acquire shared lock */
                                   )
{
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    EC_T_BOOL       bAcquired   = EC_FALSE;
    CEcTimer  oTo;

#ifdef DEBUG_LOCKS
    OsDbgMsg("Lock 0x%x, %d\n", dwTimeout, bExclusive);
#endif

    if (bExclusive)
    {
        OsLock(m_pvLock);
        m_dwExclusiveLockReqCount++;
        OsUnlock(m_pvLock);
    }

    if ((EC_WAITINFINITE != dwTimeout) && (EC_NOWAIT != dwTimeout))
    {
        oTo.Start(dwTimeout);
    }
    while (!bAcquired)
    {
        OsLock(m_pvLock);

        /* skip if already exclusively locked -> sleep */
        if (!m_bLockedExclusive)
        {
            if (bExclusive)
            {
                /* check lock count if exclusive lock needed */
                if (0 == m_dwSharedLockCount)
                {
#if (defined _DEBUG && !defined __INTIME__ && !defined _ECOS && !defined RTOS_32)
                    m_dwExLockThreadId  = GetCurrentThreadId();
#endif
                    m_bLockedExclusive  = EC_TRUE;
                    bAcquired           = EC_TRUE;
                }
            }
            else
            {
                /* shared lock shall be acquired */
                if (0 == m_dwExclusiveLockReqCount)
                {
                    /* not exclusively locked and exclusive lock not needed */
                    bAcquired = EC_TRUE;
                    m_dwSharedLockCount++;
                }
            }
        }

        OsUnlock(m_pvLock);

        if (!bAcquired)
        {
            if (oTo.IsElapsed())
            {
                dwRetVal = EC_E_TIMEOUT;
                goto Exit;
            }
            if (EC_NOWAIT == dwTimeout)
            {
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }

#ifdef DEBUG_LOCKS
            OsDbgMsg(".");
#endif
            OsSleep(1);
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if (bExclusive)
    {
        OsLock(m_pvLock);
        m_dwExclusiveLockReqCount--;
        OsUnlock(m_pvLock);
    }

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Up- / downgrade lock level.
        NOTE: Lock lost on EC_E_TIMEOUT to prevent dead-lock!

\return EC_E_NOERROR        on success
        EC_E_INVALIDSTATE   if exclusive was requested although currently not shared and vice versa.
        EC_E_TIMEOUT        if timeout expired prior to success.
        EC_E_BUSY           if called with EC_NOWAIT and lock level could not be up- / downgraded.
*/
EC_T_DWORD CEcSharedLock::ChangeLock(
    EC_T_DWORD  dwTimeout,      /**< [in]   Timeout to acquire Lock (EC_WAITINFINITE = wait forever)*/
    EC_T_BOOL   bExclusive      /**< [in]   EC_TRUE: Acquire excl. Lock, EC_FALSE: Acquire shared lock */
                                         )
{
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    EC_T_BOOL       bAcquired   = EC_FALSE;
    EC_T_BOOL       bReleased   = EC_FALSE;
    EC_T_BOOL       bLocked     = EC_FALSE;
    CEcTimer  oTo;

#ifdef DEBUG_LOCKS
    OsDbgMsg("Changelock 0x%x, %d\n", dwTimeout, bExclusive);
#endif

    if (bExclusive)
    {
        /* change from shared lock to exclusive lock */
        OsLock(m_pvLock);
        bLocked = EC_TRUE;

        m_dwExclusiveLockReqCount++;

        /* release previously acquired shared lock. needed for re-entrant usage. */
        if (0 == m_dwSharedLockCount)
        {
            /* can happen if ChangeLock(..., EC_TRUE) is called although lock is already exclusively locked. */
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }
        m_dwSharedLockCount--;

        bLocked = EC_FALSE;
        OsUnlock(m_pvLock);
    }
    else
    {
        /* change from exclusive lock to shared lock */
        if (!m_bLockedExclusive)
        {
            /* can happen if ChangeLock(..., EC_FALSE) is called although exclusive lock was already released. */
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }
    }

    if ((EC_WAITINFINITE != dwTimeout) && (EC_NOWAIT != dwTimeout))
    {
        oTo.Start(dwTimeout);
    }
    while (!bAcquired)
    {
        OsLock(m_pvLock);
        bLocked = EC_TRUE;

        if (bExclusive)
        {
            /* check lock count if exclusive lock needed */
            if (!m_bLockedExclusive && (0 == m_dwSharedLockCount))
            {
                m_bLockedExclusive = EC_TRUE;
#if (defined _DEBUG && !defined __INTIME__ && !defined _ECOS && !defined RTOS_32)
                m_dwExLockThreadId = GetCurrentThreadId();
#endif
                bAcquired = EC_TRUE;
            }
        }
        else
        {
            /* release the exclusive lock */
            if (!bReleased)
            {
                m_bLockedExclusive = EC_FALSE;
                bReleased = EC_TRUE;
            }

            /* shared lock shall be acquired. pending exclusive lock requests served first. */
            if (!m_bLockedExclusive && (0 == m_dwExclusiveLockReqCount))
            {
                m_dwSharedLockCount++;
                bAcquired = EC_TRUE;
            }
        }

        bLocked = EC_FALSE;
        OsUnlock(m_pvLock);

        if (!bAcquired)
        {
            if (oTo.IsElapsed())
            {
                dwRetVal = EC_E_TIMEOUT;
                goto Exit;
            }
            if (EC_NOWAIT == dwTimeout)
            {
                dwRetVal = EC_E_BUSY;
                goto Exit;
            }

#ifdef DEBUG_LOCKS
            OsDbgMsg(",");
#endif
            OsSleep(1);
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    if (bLocked)
    {
        bLocked = EC_FALSE;
        OsUnlock(m_pvLock);
    }

    if (bExclusive)
    {
        OsLock(m_pvLock);
        m_dwExclusiveLockReqCount--;
        OsUnlock(m_pvLock);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Release Lock.
*/
EC_T_VOID CEcSharedLock::UnLock(EC_T_VOID)
{
    OsLock(m_pvLock);
    if (m_bLockedExclusive)
    {
        m_bLockedExclusive = EC_FALSE;

#if (defined _DEBUG)
#if (!defined __INTIME__ && !defined _ECOS && !defined RTOS_32)
        OsDbgAssert(m_dwExLockThreadId == GetCurrentThreadId());
#endif
        m_dwExLockThreadId = 0;
#endif

        OsDbgAssert(0 == m_dwSharedLockCount);
    }
    else
    {
        OsDbgAssert(m_dwSharedLockCount > 0);
        m_dwSharedLockCount--;
    }

    OsUnlock(m_pvLock);
}
