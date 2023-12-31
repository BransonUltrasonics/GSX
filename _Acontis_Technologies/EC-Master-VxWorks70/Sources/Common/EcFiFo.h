/*-----------------------------------------------------------------------------
 * EcFiFo.h                 header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              
 *---------------------------------------------------------------------------*/

#ifndef INC_ECFIFO
#define INC_ECFIFO

/*-CFiFoList-----------------------------------------------------------------*/

typedef struct _EC_T_FIFO_DESC
{
    EC_T_INT nFirst;
    EC_T_INT nLast;
    EC_T_INT nSize;
} EC_T_FIFO_DESC, *EC_PT_FIFO_DESC;

template <class VALUE>
class CFiFoList
{
protected:
    VALUE*          m_pFiFoEntrys;
    volatile EC_PT_FIFO_DESC m_pDesc;
    EC_T_VOID*      m_poLock;
#ifdef DEBUGTRACE
    EC_T_DWORD      m_dwMemTrace;
#endif

public:
    CFiFoList(EC_PT_FIFO_DESC pDesc, VALUE* pFiFoEntrys, EC_T_VOID* poLock, EC_T_CHAR* szName, EC_T_DWORD dwMemTrace = 0)
    {
        EC_UNREFPARM(szName);
        m_pDesc         = pDesc;
        m_pFiFoEntrys   = pFiFoEntrys;
        m_poLock        = poLock;
#ifdef DEBUGTRACE
        m_dwMemTrace    = dwMemTrace;
#else
        EC_UNREFPARM(dwMemTrace);
#endif
    }
    CFiFoList(const CFiFoList&) {m_pFiFoEntrys = m_poLock = EC_NULL; OsDbgAssert(EC_FALSE);} /* objects are not copied */
    virtual ~CFiFoList() {}

    EC_T_VOID Lock(EC_T_VOID)
    {
        OsLock(m_poLock);
    }
    EC_T_VOID Unlock(EC_T_VOID)
    {
        OsUnlock(m_poLock);
    }

    /********************************************************************************/
    /** \brief Get amount of added entries.
    *
    * \return 
    */
    EC_T_INT GetCount(EC_T_VOID)
    { 
        if ((EC_NULL == m_pDesc) || (0 == m_pDesc->nSize))
        {
            return 0;
        }
        return (m_pDesc->nLast + m_pDesc->nSize - m_pDesc->nFirst) % m_pDesc->nSize;
    }

    /********************************************************************************/
    /** \brief Get amount of possible entries.
    *
    * \return 
    */
    EC_T_INT GetSize(EC_T_VOID)
    { 
        if (EC_NULL == m_pDesc)
        {
            return 0;
        }
        return (m_pDesc->nSize - 1);
    }

    /********************************************************************************/
    /** \brief States if Fifo is full.
    *
    * \return EC_TRUE if Fifo is full else EC_FALSE
    */
    EC_T_BOOL IsFull()
    {
        return GetCount() + 1 >= GetSize();
    }

    /********************************************************************************/
    /** \brief Add object.
    *
    * \return Add without OsLock. --> Just one "Adding" Thread is allowed!!!
    */
    EC_T_BOOL AddNoLock(VALUE newValue)
    {
        if ((EC_NULL == m_pDesc) || (0 == m_pDesc->nSize))
        {
            return EC_FALSE;
        }
        if (GetCount() >= m_pDesc->nSize - 1)
        {
            return EC_FALSE;
        }
        m_pFiFoEntrys[m_pDesc->nLast] = newValue;
        m_pDesc->nLast = (m_pDesc->nLast + 1) % m_pDesc->nSize;

        return EC_TRUE;
    }

    /********************************************************************************/
    /** \brief Add object. Thread safe in case of providing a OsLock object
    *          when constructing the FIFO.
    *
    * \return 
    */
    EC_T_BOOL Add(VALUE newValue)
    {
        EC_T_BOOL bRes = EC_FALSE;
        if (m_poLock != EC_NULL)
        {
            OsLock(m_poLock);
        }
        bRes = AddNoLock(newValue);
        if (m_poLock != EC_NULL)
        {
            OsUnlock(m_poLock);
        }
        return bRes;
    }

    /********************************************************************************/
    /** \brief Remove element
    *
    * \return  Removing without OsLock. --> Just one "Removing" Thread is allowed!!!
    */
    EC_T_BOOL RemoveNoLock(VALUE& rValue)
    {
        if ((EC_NULL == m_pDesc) || (0 == m_pDesc->nSize))
        {
            return EC_FALSE;
        }
        if (m_pDesc->nFirst == m_pDesc->nLast)
        {
            return EC_FALSE;
        }
        rValue = m_pFiFoEntrys[m_pDesc->nFirst];
        m_pDesc->nFirst = (m_pDesc->nFirst + 1) % m_pDesc->nSize;

        return EC_TRUE;
    }

    /********************************************************************************/
    /** \brief Remove element.Thread safe in case of providing a OsLock object
    *          when constructing the FIFO.
    *
    * \return
    */
    EC_T_BOOL Remove(VALUE& rValue)
    {
        EC_T_BOOL bRes = EC_FALSE;

        if (m_poLock != EC_NULL)
        {
            OsLock(m_poLock);
        }
        bRes = RemoveNoLock(rValue);
        if (m_poLock != EC_NULL)
        {
            OsUnlock(m_poLock);
        }
        return bRes;
    }

    EC_T_VOID ClearNoLock()
    {
        VALUE Value;
        while (RemoveNoLock(Value)) {}
    }

    /********************************************************************************/
    /** \brief Get next element without remove
    *
    * \return  Peak without OsLock. --> Just one "Peak" Thread is allowed!!!
    */
    EC_T_BOOL PeakNoLock( VALUE& rValue )
    {
        if ((EC_NULL == m_pDesc) || (0 == m_pDesc->nSize))
        {
            return EC_FALSE;
        }
        if (m_pDesc->nFirst == m_pDesc->nLast)
        {
            return EC_FALSE;
        }
        rValue = m_pFiFoEntrys[m_pDesc->nFirst];

        return EC_TRUE;
    }
};

/*-CFiFoListDyn--------------------------------------------------------------*/
template <class VALUE>
class CFiFoListDyn : public CFiFoList<VALUE>
{
public:
    CFiFoListDyn(EC_T_INT nSize, EC_T_VOID* poLock, EC_T_CHAR* szName, EC_T_DWORD dwMemTrace = 0)
        : CFiFoList<VALUE>((EC_PT_FIFO_DESC)OsMalloc(sizeof(EC_T_FIFO_DESC)), EC_NEW(VALUE[nSize + 1]), poLock, szName, dwMemTrace)
    {
        if ((EC_NULL == CFiFoList<VALUE>::m_pDesc) || (EC_NULL == CFiFoList<VALUE>::m_pFiFoEntrys))
        {
            /* cleanup on allocation failure */
            SafeDeleteArray(CFiFoList<VALUE>::m_pFiFoEntrys);
            SafeOsFree(CFiFoList<VALUE>::m_pDesc);
            return;
        }

        OsMemset(CFiFoList<VALUE>::m_pDesc, 0, sizeof(EC_T_FIFO_DESC));
        OsMemset(CFiFoList<VALUE>::m_pFiFoEntrys, 0, (nSize + 1) * sizeof(VALUE));
        
        CFiFoList<VALUE>::m_pDesc->nSize = nSize + 1;
    }
    CFiFoListDyn(const CFiFoListDyn&) { OsDbgAssert(EC_FALSE); } /* objects are not copied */
    virtual ~CFiFoListDyn()
    {
        SafeDeleteArray(CFiFoList<VALUE>::m_pFiFoEntrys);
        SafeOsFree(CFiFoList<VALUE>::m_pDesc);
    }
};

#endif  /* INC_ECFIFO */

/*-END OF SOURCE FILE--------------------------------------------------------*/
