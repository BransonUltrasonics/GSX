/*-----------------------------------------------------------------------------
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 *---------------------------------------------------------------------------*/

#ifndef INC_ECLOCK
#define INC_ECLOCK        1

/*-CLASS---------------------------------------------------------------------*/
class CEcExclusiveLock
{
public:
    CEcExclusiveLock()
    {
        m_pvLock = OsCreateLock();
    }
    ~CEcExclusiveLock()
    {
        if (EC_NULL != m_pvLock)
        {
            OsDeleteLock(m_pvLock);
        }
    }
    EC_T_VOID Lock() { OsLock(m_pvLock); }
    EC_T_VOID UnLock() { OsUnlock(m_pvLock); }
private:
    EC_T_PVOID m_pvLock;
};

class CEcSharedLock
{
public:
    CEcSharedLock();
    ~CEcSharedLock();

    EC_T_DWORD Lock(EC_T_DWORD dwTimeout=EC_WAITINFINITE, EC_T_BOOL bExclusive=EC_FALSE);
    EC_T_DWORD ChangeLock(EC_T_DWORD dwTimeout, EC_T_BOOL bExclusive);
    EC_T_VOID  UnLock(EC_T_VOID);

private:
    EC_T_PVOID      m_pvLock;
    EC_T_BOOL       m_bLockedExclusive;
    EC_T_DWORD      m_dwExclusiveLockReqCount;
    EC_T_DWORD      m_dwSharedLockCount;

#if (defined _DEBUG)
    EC_T_DWORD      m_dwExLockThreadId;
#endif
};

#endif /* INC_ECLOCK */
