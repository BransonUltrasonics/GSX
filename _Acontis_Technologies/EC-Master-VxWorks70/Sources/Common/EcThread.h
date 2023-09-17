/*-----------------------------------------------------------------------------
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              Thread class header
 *---------------------------------------------------------------------------*/

#ifndef INC_ECTHREAD
#define INC_ECTHREAD

class CEcThread
{
public:
    CEcThread();
    virtual ~CEcThread();

    EC_T_DWORD Start(EC_PF_THREADENTRY pfThreadEntry, EC_T_VOID* pvParams,
        const EC_T_CHAR* szThreadName, EC_T_DWORD dwPrio, EC_T_DWORD dwStackSize, EC_T_DWORD dwTimeout);

    EC_T_DWORD Stop(EC_T_DWORD dwTimeout = EC_NOWAIT);

protected:
    /* threadProc is run in separate thread and calls listenStep while thread is not stopped */
    static EC_T_DWORD threadProc(EC_T_PVOID pvParams);

    /* step function which is called from threadProc until thread is not stopped */
    virtual EC_T_VOID threadStep(EC_T_VOID);
    
    EC_INLINESTART EC_T_BOOL isStopped(EC_T_VOID) const { return (EC_NULL == m_pfThreadEntry) && (EC_NULL == m_pvParams); } EC_INLINESTOP
    EC_INLINESTART EC_T_BOOL isReady(EC_T_VOID) const { return m_bThreadReady; } EC_INLINESTOP

    EC_T_VOID stopThread(EC_T_VOID) { m_bThreadStop = EC_TRUE; }

    EC_INLINESTART EC_T_VOID setThreadProc(EC_PF_THREADENTRY pfThreadEntry, EC_T_VOID* pvParams)
    {
        m_pfThreadEntry = pfThreadEntry;
        m_pvParams = pvParams;
    } EC_INLINESTOP

private:
    /* explicitly restrict copy */
    CEcThread(const CEcThread&);
    CEcThread& operator=(const CEcThread&);

protected:
    EC_PF_THREADENTRY m_pfThreadEntry;
    EC_T_VOID* m_pvParams;

private:
    EC_T_PVOID m_hThread; /* thread handle */

    EC_T_BOOL m_bThreadStop;    /* indicates that thread should be stopped */
    EC_T_BOOL m_bThreadReady;   /* indicates that thread was really started and ready for operation */
};

#endif /* INC_ECTHREAD */
