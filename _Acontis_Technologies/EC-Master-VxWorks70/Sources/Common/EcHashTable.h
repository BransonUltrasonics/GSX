/*-----------------------------------------------------------------------------
 * CHashTable.h             header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              
 *---------------------------------------------------------------------------*/

#ifndef INC_CHASHTABLE
#define INC_CHASHTABLE

#include "EcOs.h"

#include "EcError.h"

#ifdef _MSC_VER
#pragma warning (disable: 4710)
#endif

/*-CHashTable----------------------------------------------------------------*/

#define BIG_HASHTABLESIZE     49999   /* should be a prime */
#define SMALL_HASHTABLESIZE      97   /* should be a prime */

/*-PrimeTable----------------------------------------------------------------*/
const EC_T_INT  C_anPrimes[] =
{
        3,     5,     7,    11,    13,    17,    19,    23,    29,    31,    37,    41,    43,    47,
       53,   101,   151,   211,   251,   307,   353,   401,   457,   503,   557,   601,   653,   701,
      751,   809,   853,   907,   953,  1009,  1103,  1201,  1301,  1409,  1511,  1601,  1709,  1801,  
     1901,  2003,  2111,  2203,  2309,  2411,  2503,  2609,  2707,  2801,  2903,  3001,  3203,  3407,  
     3607,  3803,  4001,  4201,  4409,  4603,  4801,  5003,  6007,  7001,  8009,  9001, 10007, 12007, 
    14009, 16001, 18013, 20011, 22003, 24001, 26003, 28001, 30011, 35023, 40009, 45007, 50021, 55001, 
    60013, 65003, 65537, 70439, 90619, 99991, 115163, 135929, 153259, 166561
};


/*-CHashTableDyn-------------------------------------------------------------*/
template <class KEY, class VALUE>
class CHashTableDyn
{

typedef EC_T_VOID (*PForeEachFunc)( EC_T_VOID* pThis, VALUE* pValue );

protected:
    typedef struct HashEntry
    {
        HashEntry*  pNext;
        HashEntry*  pNextFree;
        HashEntry*  pNextGlob;
        KEY         key;
        VALUE       value;
    } *PHashEntry;

    EC_T_INT    m_nSize;
    PHashEntry  m_pHashEntrys;
    PHashEntry  m_pFreeEntrys;
    PHashEntry  m_pFirstEntry;
    PHashEntry* m_pEntrys;
    EC_T_INT    m_nKeyCount;
    EC_T_INT    m_nHashTableSize;

    EC_T_VOID*  m_poLock;

    struct _EC_T_MEMORY_LOGGER* m_pLog;

    EC_T_DWORD  m_dwMemTrace;

public:


    /***************************************************************************************************/
    /**
    \brief  Constructor to create templated Dynamic Hash Table..
    */
    CHashTableDyn(
        EC_T_DWORD dwMemTrace,      /**< [in]   Memory trace parameter */
        EC_T_DWORD dwRequiredSize,  /**< [in]   Required space in Hash Table */
        struct _EC_T_MEMORY_LOGGER* pLog    /**< [in]   Pointer to the memory logger */
                 )
    {
        EC_T_INT nNextPrime = NextHigherPrime(dwRequiredSize);

        OsDbgAssert( 0 != nNextPrime );
        if( 0 == nNextPrime )
        {
            return;
        }

        m_dwMemTrace        = dwMemTrace;
        m_pLog              = pLog;

        m_poLock            = OsCreateLockTyped(eLockType_SPIN);

        m_nHashTableSize    = nNextPrime;
        m_nSize             = dwRequiredSize;

        m_pEntrys           = EC_NEW(PHashEntry[m_nHashTableSize]);
        OsMemset( m_pEntrys, 0, m_nHashTableSize * sizeof(PHashEntry) );
        
        m_pHashEntrys       = EC_NEW(HashEntry[m_nSize]);

        EC_TRACE_ADDMEM_LOG(dwMemTrace, pLog, "CHashTableDyn::CHashTableDyn", m_pEntrys, m_nHashTableSize * sizeof(PHashEntry));
        EC_TRACE_ADDMEM_LOG(dwMemTrace, pLog, "CHashTableDyn::CHashTableDyn", m_pHashEntrys, m_nSize * sizeof(HashEntry));
        
        OsMemset( m_pHashEntrys, 0, m_nSize*sizeof(HashEntry) );
        
        for ( EC_T_INT i = 1; i < m_nSize; i++ )
        {
            (m_pHashEntrys[(i-1)].pNextFree)    = &m_pHashEntrys[i];
        }
        
        m_pHashEntrys[(m_nSize-1)].pNextFree    = EC_NULL;

        m_pFreeEntrys       = &m_pHashEntrys[0];
        m_pFirstEntry       = EC_NULL;
                            
        m_nKeyCount         = 0;
    }

    /********************************************************************************/
    /** \brief
    *
    * \return 
    */
    ~CHashTableDyn()
    {
        EC_TRACE_SUBMEM_LOG(m_dwMemTrace, m_pLog, "CHashTableDyn::~CHashTableDyn", m_pHashEntrys, m_nSize * sizeof(HashEntry));
        EC_TRACE_SUBMEM_LOG(m_dwMemTrace, m_pLog, "CHashTableDyn::~CHashTableDyn", m_pEntrys, m_nHashTableSize * sizeof(PHashEntry));
        SafeDeleteArray(m_pHashEntrys);
        SafeDeleteArray(m_pEntrys);
        OsDeleteLock(m_poLock);
    }

    /********************************************************************************/
    /** \brief
    *
    * \return 
    */
    EC_T_BOOL   Add( KEY key, VALUE newValue );

    /********************************************************************************/
    /** \brief 
    *
    * \return
    */
    EC_T_BOOL   Lookup( KEY key, VALUE& rValue )
    {
        EC_T_BOOL   ret     = EC_FALSE;
        EC_T_INT    nHash   = GetHashValue( key );

        OsLock(m_poLock);

        PHashEntry p    = m_pEntrys[nHash];

        while ( p != EC_NULL )
        {
            if ( p->key == key )
                break;
            p = p->pNext;
        }
        if ( p )
        {
            rValue  = p->value;
            ret     = EC_TRUE;
        }

        OsUnlock(m_poLock);

        return ret;
    }

    /********************************************************************************/
    /** \brief 
    *
    * \return 
    */
    VALUE* Lookup( KEY key );

    /********************************************************************************/
    /** \brief 
    *
    * \return 
    */
    EC_T_BOOL   Remove( KEY key )
    {
        EC_T_BOOL   ret     = EC_FALSE;
        EC_T_INT    nHash       = GetHashValue( key );
        PHashEntry l    = EC_NULL;
        PHashEntry p    = m_pEntrys[nHash];

        OsLock(m_poLock);

        while ( p )                                     /* searching right entry */
        {
            if ( p->key == key )
                break;                                  /* entry found */

            l = p;                                      /* l is previous entry */
            p = p->pNext;
        }
        
        if ( p )
        {
            /*  removing from hash-list */
            if ( l )                                        /* if not first entry */
                l->pNext                = p->pNext;     /*  entry removed */
            else                                            /*  if first entry */
                m_pEntrys[nHash]    = p->pNext;     /*  entry removed */

            /* removing from global-list */
            if ( p == m_pFirstEntry )
                m_pFirstEntry = p->pNextGlob;
            else
            {
                l = m_pFirstEntry;
                while ( l->pNextGlob != p )
                    l = l->pNextGlob;

                l->pNextGlob = p->pNextGlob;
            }
            
            m_nKeyCount--;

            /*
            OsUnlock(m_poLock);
            OsLock(m_poLock);
            */
    
            p->pNextFree    = m_pFreeEntrys;
            m_pFreeEntrys   = p;

            ret             = EC_TRUE;
        }
        OsUnlock(m_poLock);

        return ret;
    }

    /********************************************************************************/
    /** \brief
    *
    * \return 
    */
    EC_T_BOOL   RemoveFirstEntry( VALUE& rValue )
    {
        if ( m_pFirstEntry )
        {
            rValue = m_pFirstEntry->value;
            
            return Remove(m_pFirstEntry->key);
        }
        else
            return EC_FALSE;
    }

    /********************************************************************************/
    /** \brief 
    *
    * \return 
    */
    EC_T_VOID   RemoveAll()
    {
        OsLock(m_poLock);

        OsMemset( m_pEntrys, 0, m_nHashTableSize * sizeof(PHashEntry) );

        OsMemset( m_pHashEntrys, 0, m_nSize*sizeof(HashEntry) );
        
        for ( EC_T_INT i = 1; i < m_nSize; i++ )
            m_pHashEntrys[i-1].pNextFree = &m_pHashEntrys[i];

        m_pHashEntrys[m_nSize-1].pNextFree = EC_NULL;

        m_pFreeEntrys   = &m_pHashEntrys[0];
        m_pFirstEntry   = EC_NULL;

        m_nKeyCount     = 0;

        OsUnlock(m_poLock);
    }

    /********************************************************************************/
    /** \brief 
    *
    * \return 
    */
    EC_T_BOOL GetFirstEntry( VALUE& rValue, EC_T_VOID*& pVoid )
    {
        pVoid = m_pFirstEntry;

        if ( pVoid )
        {
            rValue = ((PHashEntry)pVoid)->value;
            return EC_TRUE;
        }
        else
            return EC_FALSE;
    }

    /********************************************************************************/
    /** \brief 
    *
    * \return 
    */
    EC_T_BOOL GetNextEntry( VALUE& rValue, EC_T_VOID*& pVoid )
    {
        if ( !pVoid )
            pVoid = m_pFirstEntry;
        else
            pVoid = ((PHashEntry)pVoid)->pNextGlob;

        if ( pVoid )
        {
            rValue = ((PHashEntry)pVoid)->value;
            return EC_TRUE;
        }
        else
            return EC_FALSE;
    }

    /********************************************************************************/
    /** \brief 
    *
    * \return 
    */
    EC_T_BOOL GetNextEntry( KEY& rKey, VALUE*& pValue, EC_T_VOID*& pVoid )
    {
        if ( !pVoid )
            pVoid = m_pFirstEntry;
        else
            pVoid = ((PHashEntry)pVoid)->pNextGlob;

        if ( pVoid )
        {
            pValue  = &(((PHashEntry)pVoid)->value);
            rKey        = ((PHashEntry)pVoid)->key;
            return EC_TRUE;
        }
        else
            return EC_FALSE;
    }

    /********************************************************************************/
    /** \brief Verify if the key is already in use
    *
    * \return EC_TRUE if it is in used, EC_FALSE otherwise.
    */
    EC_T_BOOL KeyInUse( KEY key ) const
    {
        EC_T_INT            nHash   = GetHashValue( key );
        OsLock(m_poLock);
        PHashEntry  p       = m_pEntrys[nHash];
        while ( p )
        {
            if ( p->key == key )
            {
                OsUnlock(m_poLock);
                return EC_TRUE;
            }
            p = p->pNext;
        }
        OsUnlock(m_poLock);
        return EC_FALSE;
    }

    /********************************************************************************/
    /** \brief 
    *
    * \return 
    */
    EC_T_VOID ForEach( PForeEachFunc pFunc, EC_T_VOID* pUser ) const
    {
        EC_T_INT i;
        PHashEntry  p;
        for ( i = 0; i < m_nHashTableSize; i++ )
        {
            p = m_pEntrys[i];
            while ( p )
            {
                pFunc( pUser, &p->value );
                p = p->pNext;
            }
        }
    }

    /********************************************************************************/
    /** \brief 
    *
    * \return 
    */
    EC_T_DWORD KeyCount() const
    {
        return m_nKeyCount;
    }

protected:
    /********************************************************************************/
    /** \brief Verify if the key is already in use
    *
    * \return EC_TRUE if it is in used, EC_FALSE otherwise.
    */
    PHashEntry KeyInUse( KEY key, EC_T_INT nHash ) const
    {
        PHashEntry p = m_pEntrys[nHash];
        while ( p )
        {
            if ( p->key == key )
                return p;
            p = p->pNext;
        }
        return EC_NULL;
    }

    /********************************************************************************/
    /** \brief Generate a hash value for a given key
    *
    * \return hash value
    */
    EC_T_INT GetHashValue( KEY key ) const
        { return (EC_T_INT)( key % m_nHashTableSize ); }


    /***************************************************************************************************/
    /**
    \brief  Find next higher prime (not closest next, but configured next) of a number (size).
    \return Found Prime.
    */
    EC_T_INT NextHigherPrime( 
        EC_T_INT nBase  /**< [in]   Value to find prime to */
                            )
    {
        EC_T_INT nNextPrime = 0;

        for( EC_T_INT nIdx = 0; nIdx < ((EC_T_INT)(sizeof(C_anPrimes)/sizeof(C_anPrimes[0]))); nIdx++ )
        {
            if( C_anPrimes[nIdx] > nBase )
            {
                nNextPrime = C_anPrimes[nIdx];
                break;
            };
        }

        return nNextPrime;
    }

};


template <class KEY, class VALUE>
EC_T_BOOL   CHashTableDyn<KEY,VALUE>::Add( KEY key, VALUE newValue )
{
    EC_T_BOOL   ret = EC_FALSE;
    EC_T_INT    nHash = GetHashValue( key );
    PHashEntry p, n;

    OsLock(m_poLock);

    /*OsTrcMsg( "AddHashTbl, hash value = %d\n", nHash);*/

    /* if ( EC_NULL == (p = KeyInUse( key, nHash )) ) */
    if ( EC_NULL != (p = KeyInUse( key, nHash )) )
    {
        p->value = newValue;
        ret     = EC_TRUE;
    }

    else
    {
        /* the key is already in use */
        if ( m_pFreeEntrys )
        {
            n                   = m_pFreeEntrys;
            m_pFreeEntrys       = m_pFreeEntrys->pNextFree;

            /*OsUnlock(m_poLock);*/
    
            n->value                = newValue;
            n->key              = key;

            /*OsLock(m_poLock);*/
    
            n->pNext            = m_pEntrys[nHash];
            m_pEntrys[nHash]    = n;

            n->pNextGlob        = m_pFirstEntry;
            m_pFirstEntry       = n;
            
            ret = EC_TRUE;
            m_nKeyCount++;
            OsDbgAssert(m_nKeyCount <= m_nSize);
            OsDbgAssert(m_nKeyCount <= m_nHashTableSize);
        }
        else
        {
            /* hash table is too small! */
            OsDbgAssert(EC_FALSE);
        }
    }

    OsUnlock(m_poLock);

    return ret;
}

template <class KEY, class VALUE>
VALUE* CHashTableDyn<KEY,VALUE>::Lookup( KEY key )
{
    EC_T_INT    nHash       = GetHashValue( key );
    VALUE* pValue = EC_NULL;
    
    OsLock(m_poLock);
    
    PHashEntry p    = m_pEntrys[nHash];

    while ( p != EC_NULL )
    {
        if ( p->key == key )
            break;
        p = p->pNext;
    }

    if ( p )
        pValue = &p->value;

    OsUnlock(m_poLock);

    return pValue;
}

/*-CListR0<TYPE, ARG_TYPE>---------------------------------------------------*/
#if !defined(_AFXDLL)  && !defined(__AFX_H__)
    typedef EC_T_VOID*      POSITION;
#endif

template<class TYPE, class ARG_TYPE, EC_T_UINT SIZE>
class CListR0
{
protected:
    struct CNode
    {
        CNode* pNext;
        CNode* pPrev;
        TYPE data;
    };
public:
/* Construction */
    CListR0();

/* Attributes (head and tail) */
    /* count of elements */
    EC_INLINESTART EC_T_INT GetCount() const EC_INLINESTOP;
    EC_T_BOOL IsEmpty() const;

    /* peek at head or tail */
    TYPE& GetHead();
    TYPE GetHead() const;
    TYPE& GetTail();
    TYPE GetTail() const;

/* Operations */
    /* get head or tail (and remove it) - don't call on empty list ! */
    TYPE RemoveHead();
    TYPE RemoveTail();

    /* add before head or after tail */
    POSITION AddHead(ARG_TYPE newElement);
    POSITION AddTail(ARG_TYPE newElement);

    /* iteration */
    POSITION GetHeadPosition() const;
    POSITION GetTailPosition() const;
    TYPE& GetNext(POSITION& rPosition); /* return *Position++ */
    TYPE GetNext(POSITION& rPosition) const; /* return *Position++ */
    TYPE& GetPrev(POSITION& rPosition); /* return *Position-- */
    TYPE GetPrev(POSITION& rPosition) const; /* return *Position-- */

    /* getting/modifying an element at a given position */
    TYPE& GetAt(POSITION position);
    TYPE GetAt(POSITION position) const;
    EC_T_VOID SetAt(POSITION pos, ARG_TYPE newElement);
    EC_T_VOID RemoveAt(POSITION position);

    /* inserting before or after a given position */
    POSITION InsertBefore(POSITION position, ARG_TYPE newElement);
    POSITION InsertAfter(POSITION position, ARG_TYPE newElement);

    /* helper functions (note: O(n) speed) */
    POSITION Find(ARG_TYPE searchValue, POSITION startAfter = EC_NULL) const;
        /* defaults to starting at the HEAD, return EC_NULL if not found */
    POSITION FindIndex(EC_T_INT nIndex) const;
        /* get the 'nIndex'th element (may return EC_NULL) */

/* Implementation */
protected:
    CNode*      m_pNodeHead;
    CNode*      m_pNodeTail;
    CNode*      m_pNodeFree;
    EC_T_INT    m_nCount;
    CNode       m_arrNodeFree[SIZE];
    EC_T_VOID*  m_poLock;
public:
    ~CListR0();
/*  EC_T_VOID Serialize(CArchive&); */
};


#endif

/*-END OF SOURCE FILE--------------------------------------------------------*/
