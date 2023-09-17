/* wmHashLib.h -- header file for wmHashLib.c */

/* Copyright 2004,2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCwmHashLibh
#define __INCwmHashLibh


typedef ubyte4  (*fnHashKeyExtract)(sbyte * pName);
typedef BOOL    (*fnHashCompare)(sbyte * pName, void * pEntry);
typedef sbyte * (*fnHashNodeName)(void * pEntry);

typedef struct HASH_S
    {
    struct HASH_S * pNext;
    void *          pEntry;
    ubyte4          key;    /* full hash value -- not just offset */
    void *          owner;
#ifdef __WM_DEBUG__
    sbyte4          rid;    /* row id -- for debugging only */
#endif
    }
    HASH_T;

typedef struct HASH_BLOCK_T
    {
    struct HASH_BLOCK_T * pNext;
    HASH_T *    pNodes;
    sbyte4      max;
    sbyte4      count;
    }
    HASH_BLOCK_T;

typedef struct WMB_HASH_TABLE_T
    {
    MEM_POOL_ID         mem;
    sbyte4              tableSize;
    sbyte4              count;
    WM_BLOCK_T *        pBlock;
    HASH_T *            pNodes;
    fnHashKeyExtract    pFuncExtractKey;
    fnHashCompare       pFuncCompare;
    fnHashNodeName      pFuncName;
    } WMB_HASH_TABLE_T;


/* Prototypes */

#ifdef __cplusplus
extern "C" {
#endif

STATUS  wmHashConstruct  (MEM_POOL_ID memID, sbyte4 size,
                           fnHashKeyExtract pFuncExtractKey,
                           fnHashCompare pFuncCompare,
                           fnHashNodeName pFuncName,
                           WMB_HASH_TABLE_T ** ppTable);

void    wmHashDestruct   (WMB_HASH_TABLE_T * pHashTable);

STATUS  wmHashEntryAdd   (WMB_HASH_TABLE_T * pHashTable, sbyte * pName,
                           void * pId, void * pEntry);

void *  wmHashEntryFind  (WMB_HASH_TABLE_T * pHashTable, sbyte * pData,
                               void * pID);

ubyte4  wmHashKeyGenerate (sbyte * pStr);

#ifdef __WM_DEBUG__
void    wmHashDump (WMB_HASH_TABLE_T * pTable);
#endif

#ifdef __cplusplus
}
#endif


#endif /* __INCwmHashLibh */
