/*-----------------------------------------------------------------------------
* EcMemPool.h
* Copyright                acontis technologies GmbH, Weingarten, Germany
* Response                 Holger Oelhaf
* Description              
*---------------------------------------------------------------------------*/
#ifndef INC_ECMEMPOOL
#define INC_ECMEMPOOL
/*-INCLUDES------------------------------------------------------------------*/
#include "EcType.h"
#include "EcOs.h"

/*-DEFINES-------------------------------------------------------------------*/
#define EC_MEMPOOL_MIN_BLOCK_SIZE 32
#define EC_MEMPOOL_ALIGNMENT 4
#define EC_MEMPOOL_BLOCK_OVERHEAD EC_MAX(sizeof(EC_T_MEMPOOL_BLOCK), EC_MEMPOOL_ALIGNMENT)
#define EC_MEMPOOL_MIN_POOL_SIZE sizeof(EC_T_MEMPOOL_DESC) + EC_MEMPOOL_BLOCK_OVERHEAD + EC_MEMPOOL_MIN_BLOCK_SIZE

/*-MACROS--------------------------------------------------------------------*/
#define EC_MEMPOOL_GET_BLOCK(addr) (EC_T_MEMPOOL_BLOCK*)(((EC_T_BYTE*)(addr)) - EC_MEMPOOL_BLOCK_OVERHEAD)
#define EC_MEMPOOL_GET_NEXT_BLOCK(addr, size) (EC_T_MEMPOOL_BLOCK*)(((EC_T_BYTE*)(addr)) + ((size) + EC_MEMPOOL_BLOCK_OVERHEAD))

/*-TYPEDEFS------------------------------------------------------------------*/
typedef struct _EC_T_MEMPOOL_BLOCK
{
    EC_T_DWORD                  dwSize;         /* Size of memory block */
    struct _EC_T_MEMPOOL_BLOCK* pNext;          /* Next memory block */
} EC_T_MEMPOOL_BLOCK;

typedef struct _EC_T_MEMPOOL_DESC
{
    EC_T_DWORD              dwSize;                 /* Size of the memory pool */
    EC_T_DWORD              dwMinBlockSize;         /* Minimal size of a memory block inside the pool */
    EC_T_DWORD              dwAllocatedMem;         /* Total amount of current allocated memory */
#if (defined DEBUG)
    EC_T_DWORD              dwBlocksAllocated;      /* Number of memory blocks allocated inside the pool */
    EC_T_DWORD              dwMaxAllocatedMem;      /* all-time peak of memory allocated inside the pool */
    EC_T_DWORD              dwMaxBlocksAllocated;   /* all-time peak of blocks allocated inside the pool */
#endif
    EC_T_MEMPOOL_BLOCK*     pFirstFreeBlock;        /* Pointer to first free memory block */
} EC_T_MEMPOOL_DESC;

/*-FUNCTION DECLARATION------------------------------------------------------*/
EC_T_MEMPOOL_DESC* EcMemPoolInitialize(EC_T_VOID* pvAddress, EC_T_DWORD dwSize);
EC_T_VOID*         EcMemPoolAlloc(EC_T_MEMPOOL_DESC* pMemPool, EC_T_DWORD dwSize);
EC_T_VOID*         EcMemPoolRealloc(EC_T_MEMPOOL_DESC* pMemPool, EC_T_VOID* pvMem, EC_T_DWORD dwSize);
EC_T_VOID          EcMemPoolFree(EC_T_MEMPOOL_DESC* pMemPool, EC_T_VOID* pvMem);

#endif
/*-END OF SOURCE FILE--------------------------------------------------------*/
