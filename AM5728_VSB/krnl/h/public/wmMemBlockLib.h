/* wmMemBlockLib.h - header for wmMemBlockLib.c */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCwmMemBlockLibh
#define __INCwmMemBlockLibh

/* pool of memory blocks for WindManage*/

typedef struct WM_BLOCK_T
    {
#ifndef __NO_MAGIC__
    ubyte4              magic;      /* integrity checks */
#endif
    struct WM_BLOCK_T * pNext;
    void *              memID;      /* mem pool id          */
    long                used;       /* elements in use      */
    long                max;        /* size of pool         */
    long                size;       /* element size         */
    void *              free;       /* bitflags if free     */
    void *              pData;      /* data                 */
    }
    WM_BLOCK_T;


#define wmMemBlock(x)           ((WM_BLOCK_T *) x)
#define wmMemBlockCountGet(x)   wmMemBlock(x)->count
#define wmMemBlockMaxGet(x)     wmMemBlock(x)->max
#define wmMemBlockMemGet(x)     wmMemBlock(x)->memID
#define wmMemBlockMemSet(x,y)   wmMemBlock(x)->memID = y

#define MAGIC_BLOCK_K           0xcafebabe

#define wmMemBlockBless(x)      wmMagicBless (wmMemBlock(x), MAGIC_BLOCK_K)
#define wmMemBlockBad(x)        wmMagicBad   (wmMemBlock(x), MAGIC_BLOCK_K)


#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WRS_CONFIG_LP64
WM_BLOCK_T *    wmMemBlockCreate (void * memId, WM_BLOCK_T * pPrior,
                                  int elements, int size);
void *          wmMemBlockEntryGet (WM_BLOCK_T * pBlock, int index);
#else
WM_BLOCK_T *    wmMemBlockCreate (void * memId, WM_BLOCK_T * pPrior,
                                  long elements, long size);
void *          wmMemBlockEntryGet (WM_BLOCK_T * pBlock, long index);
#endif
void *          wmMemBlockEntryCreate (WM_BLOCK_T * pBlock);
STATUS          wmMemBlockFree (WM_BLOCK_T * pBlock, void * pData);
void            wmMemBlockDestroy (WM_BLOCK_T * pBlock);

#ifdef __cplusplus
}
#endif


#endif /* __INCwmMemBlockLibh */
