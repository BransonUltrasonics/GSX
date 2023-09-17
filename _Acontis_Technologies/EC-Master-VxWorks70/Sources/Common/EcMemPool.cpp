/*-----------------------------------------------------------------------------
* EcMemPool.cpp            Implementation file
* Copyright                acontis technologies GmbH, Weingarten, Germany
* Response                 Holger Oelhaf
* Description              
*---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "EcMemPool.h"

/***************************************************************************************************/
/**
\brief  Initialize a memory pool in a preallocated memory area.

\return Memory pool description
*/
EC_T_MEMPOOL_DESC* EcMemPoolInitialize(
    EC_T_VOID* pvAddress,   /**< [in] Start address of the memory pool */
    EC_T_DWORD dwSize       /**< [in] Size of the memory pool. At least: sizeof(EC_T_MEMPOOL_DESC) + EC_MEMPOOL_BLOCK_OVERHEAD + EC_MEMPOOL_MIN_BLOCK_SIZE) (~54 bytes) */
    )
{
    EC_T_MEMPOOL_DESC* pMemPool = EC_NULL;
    /* dwSize should be big enough to allocated one block */
    if (EC_NULL == pvAddress || dwSize < EC_MEMPOOL_MIN_POOL_SIZE)
        goto Exit;

    pMemPool = (EC_T_MEMPOOL_DESC*)pvAddress;
    OsMemset(pMemPool, 0, sizeof(EC_T_MEMPOOL_DESC));
    /* initialize memory pool description */
    pMemPool->dwSize = dwSize;
    pMemPool->dwMinBlockSize = EC_MEMPOOL_MIN_BLOCK_SIZE;
    pMemPool->dwAllocatedMem = sizeof(EC_T_MEMPOOL_DESC);
    pMemPool->pFirstFreeBlock = (EC_T_MEMPOOL_BLOCK*)EC_ENDOF(pMemPool);
    OsDbgAssert(0 == ((EC_T_DWORD) ((EC_T_BYTE*)pMemPool->pFirstFreeBlock - (EC_T_BYTE*)EC_NULL) % EC_MEMPOOL_ALIGNMENT));

    /* initialize first free block */
    pMemPool->dwAllocatedMem += EC_MEMPOOL_BLOCK_OVERHEAD; /* "allocate" first memory block */
    pMemPool->pFirstFreeBlock->dwSize = pMemPool->dwSize - pMemPool->dwAllocatedMem;
    pMemPool->pFirstFreeBlock->pNext = EC_NULL;

Exit:
    return pMemPool;
}
/***************************************************************************************************/
/**
\brief  Allocates a block of dwSize bytes of memory from a given pool      

\return On success pointer to memory, otherwise EC_NULL
*/
EC_T_VOID* EcMemPoolAlloc(EC_T_MEMPOOL_DESC* pMemPool, EC_T_DWORD dwSize)
{
    EC_T_VOID* pvMem = EC_NULL;
    EC_T_MEMPOOL_BLOCK* pCurrFree = EC_NULL;
    EC_T_MEMPOOL_BLOCK* pPrevFree = EC_NULL;
    EC_T_DWORD dwAllocationSize = 0;
    /* check parameters*/
    if (EC_NULL == pMemPool || 0 == dwSize)
    {
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }

    dwAllocationSize = EC_MAX(dwSize, pMemPool->dwMinBlockSize); /* allocate not to small blocks to keep fragmentation low */
    if (dwAllocationSize % EC_MEMPOOL_ALIGNMENT) /* increase allocation size to fit alignment */
    {
        dwAllocationSize += EC_MEMPOOL_ALIGNMENT - (dwAllocationSize % EC_MEMPOOL_ALIGNMENT);
    }

    if (dwAllocationSize > (pMemPool->dwSize - pMemPool->dwAllocatedMem))
    {
        goto Exit;
    }

    /* First-Fit Allocation Algorithm */
    pCurrFree = pPrevFree = pMemPool->pFirstFreeBlock;
    while (pCurrFree != EC_NULL)
    {
        if (pCurrFree->dwSize >= dwAllocationSize)
        {
            EC_T_MEMPOOL_BLOCK* pNextFree = EC_NULL;

            /* split block if it's big enough otherwise take the whole one */
            if (pMemPool->dwMinBlockSize < (pCurrFree->dwSize - dwAllocationSize))
            {
                pNextFree = EC_MEMPOOL_GET_NEXT_BLOCK(pCurrFree, dwAllocationSize);
                pNextFree->dwSize = pCurrFree->dwSize - dwAllocationSize - EC_MEMPOOL_BLOCK_OVERHEAD;
                pNextFree->pNext = pCurrFree->pNext;
                pMemPool->dwAllocatedMem += EC_MEMPOOL_BLOCK_OVERHEAD; /* add overhead because a new block was created */
                pCurrFree->dwSize = dwAllocationSize;
            }
            else
            {
                pNextFree = pCurrFree->pNext;
            }
            /* block found update memory pool desc */
            pMemPool->dwAllocatedMem += pCurrFree->dwSize;
#if(defined DEBUG)
            pMemPool->dwBlocksAllocated++;
            pMemPool->dwMaxBlocksAllocated = EC_MAX(pMemPool->dwBlocksAllocated, pMemPool->dwMaxBlocksAllocated);
            pMemPool->dwMaxAllocatedMem = EC_MAX(pMemPool->dwMaxAllocatedMem, pMemPool->dwAllocatedMem);
#endif
            /* update free block list */
            if (pCurrFree == pMemPool->pFirstFreeBlock)
            {
                pMemPool->pFirstFreeBlock = pNextFree;
            }
            else
            {
                pPrevFree->pNext = pNextFree;
            }
            /* return pointer to allocated memory */
            pCurrFree->pNext = EC_MEMPOOL_GET_NEXT_BLOCK(pCurrFree, pCurrFree->dwSize);
            pvMem = (EC_T_VOID*)EC_ENDOF(pCurrFree);
            break;
        }
        pPrevFree = pCurrFree;
        pCurrFree = pCurrFree->pNext;
    }

Exit:
    if (EC_NULL == pvMem)
    { 
        EC_ERRORMSG(("Allocation of %d Bytes failed\n", dwSize));
    }
    OsDbgAssert((((EC_T_BYTE*)pvMem - (EC_T_BYTE*)EC_NULL) % EC_MEMPOOL_ALIGNMENT) == 0);
    return pvMem;
}

/***************************************************************************************************/
/**
\brief  A block of memory previously allocated is deallocated in the given pool,
        making it available again for further allocations.

\return 
*/
EC_T_VOID EcMemPoolFree(EC_T_MEMPOOL_DESC* pMemPool, EC_T_VOID* pvMem)
{
    EC_T_MEMPOOL_BLOCK* pMemBlock = EC_NULL;
    EC_T_MEMPOOL_BLOCK* pCurrFree = EC_NULL;
    EC_T_MEMPOOL_BLOCK* pPrevFree = EC_NULL;

    /* check parameters*/
    if (EC_NULL == pvMem || EC_NULL == pMemPool)
        goto Exit;
    /* check if pvMem is inside the pool */
    if (pvMem < pMemPool || pvMem >((EC_T_BYTE*)pMemPool + pMemPool->dwSize))
    {
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }
    /* acquire pointer to block header */
    pMemBlock = EC_MEMPOOL_GET_BLOCK(pvMem);
    /* check if block header is valid */
    if (pMemBlock->pNext != EC_MEMPOOL_GET_NEXT_BLOCK(pMemBlock, pMemBlock->dwSize))
    {
        EC_ERRORMSG(("EcMemPoolFree(): Memory block 0x%x corrupted\n", pMemBlock));
        goto Exit;
    }

    /* find the correct spot to place the block into the free list, increasing order sorted by address */
    for (pCurrFree = pMemPool->pFirstFreeBlock;
        EC_NULL != pCurrFree;
        pPrevFree = pCurrFree, pCurrFree = pCurrFree->pNext)
    {
        if (pMemBlock < pCurrFree)
        {
            break;
        }
    }
    /* place the block */
    if (pCurrFree == pMemPool->pFirstFreeBlock)
    {
        pMemBlock->pNext = pMemPool->pFirstFreeBlock;
        pMemPool->pFirstFreeBlock = pMemBlock;
    }
    else
    {
        pMemBlock->pNext = pPrevFree->pNext;
        pPrevFree->pNext = pMemBlock;
    }
    /* update memory pool */
#if (defined DEBUG)
    pMemPool->dwBlocksAllocated--;
#endif
    pMemPool->dwAllocatedMem -= pMemBlock->dwSize;
    /* try to merge with upper neighbor */
    if (pMemBlock->pNext == EC_MEMPOOL_GET_NEXT_BLOCK(pMemBlock, pMemBlock->dwSize))
    {
        EC_T_MEMPOOL_BLOCK* pNextBlock = EC_NULL;
        pNextBlock = EC_MEMPOOL_GET_NEXT_BLOCK(pMemBlock, pMemBlock->dwSize);

        pMemBlock->dwSize += pNextBlock->dwSize + EC_MEMPOOL_BLOCK_OVERHEAD;
        pMemBlock->pNext = pNextBlock->pNext;
        pMemPool->dwAllocatedMem -= EC_MEMPOOL_BLOCK_OVERHEAD;
    }
    /* try to merge with lower neighbor */
    if (pPrevFree && pPrevFree->pNext == EC_MEMPOOL_GET_NEXT_BLOCK(pPrevFree, pPrevFree->dwSize))
    {
        EC_T_MEMPOOL_BLOCK* pNextBlock = EC_NULL;
        pNextBlock = EC_MEMPOOL_GET_NEXT_BLOCK(pPrevFree, pPrevFree->dwSize);

        pPrevFree->dwSize += pNextBlock->dwSize + EC_MEMPOOL_BLOCK_OVERHEAD;
        pPrevFree->pNext = pNextBlock->pNext;
        pMemPool->dwAllocatedMem -= EC_MEMPOOL_BLOCK_OVERHEAD;
    }

Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Changes the size of the memory block pointed to by pvMem.
        The function may move the memory block to a new location.

\return On success pointer to memory, otherwise EC_NULL
*/
EC_T_VOID* EcMemPoolRealloc(EC_T_MEMPOOL_DESC* pMemPool, EC_T_VOID* pvMem, EC_T_DWORD dwSize)
{
    EC_T_MEMPOOL_BLOCK* pMemBlock = EC_NULL;
    EC_T_VOID* pvNewMem = EC_NULL;

    /* check parameters*/
    if (EC_NULL == pMemPool || 0 == dwSize)
        goto Exit;

    /* in case that pvMem is a null pointer, behave like malloc */
    if (EC_NULL == pvMem)
    {
        pvNewMem = EcMemPoolAlloc(pMemPool, dwSize);
        goto Exit;
    }
    /* new size fits in current block */
    pMemBlock = EC_MEMPOOL_GET_BLOCK(pvMem);
    if (dwSize <= pMemBlock->dwSize)
    {
        pvNewMem = pvMem;
        goto Exit;
    }
    /* basic realloc implementation */
    pvNewMem = EcMemPoolAlloc(pMemPool, dwSize);
    if (EC_NULL == pvNewMem)
        goto Exit;

    OsMemcpy(pvNewMem, pvMem, pMemBlock->dwSize);
    EcMemPoolFree(pMemPool, pvMem);

Exit:
    return pvNewMem;
}
