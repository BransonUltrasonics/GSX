/* wmbLockMgrLib.h - Lock Manager header for WMB */

/* Copyright (c) 2004, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01b,20jul12,r_w  eliminate memory leak issue. defect WIND00359749
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbLockMgrLibh
#define __INCwmbLockMgrLibh

/* Lock levels as registered */
typedef enum
    {
    WMB_LOCK_LEVEL_OBJECT_K = 0,
    WMB_LOCK_LEVEL_TABLE_K,
    WMB_LOCK_LEVEL_COMP_K
    }WMB_LOCK_LEVEL_T;

/* Lock Head Structure
This is the anchor sturcture for the lock request queue.
*/

typedef enum
    {
    WMB_MODE_FREE = 0,
    WMB_MODE_SHARED,
    WMB_MODE_EXCLUSIVE,
    WMB_MODE_INTENT
    }WMB_LOCK_MODE_T;

typedef enum
    {
    WMB_STATUS_NULL = 0,
    WMB_STATUS_GRANTED,
    WMB_STATUS_WAITING,
    WMB_STATUS_TIMEOUT,
    WMB_STATUS_CONVERT,
    WMB_STATUS_ERROR
    }WMB_LOCK_STATUS_T;


typedef struct WMB_LOCK_HEAD_S
    {
    struct WMB_LOCK_REQ_S   *pLockReq;
    WMB_LOCK_MODE_T         granted;
    OS_SEM_T                semHead;
    OS_SEM_T                semExclusive;
    OS_SEM_T                semShared;
    ubyte2                  numLocks;
    }WMB_LOCK_HEAD_T;

/* Lock Request structure to track individual locks on a resource
*/

typedef struct WMB_LOCK_REQ_S
    {
    ubyte2                  lockCount;
    WMB_LOCK_MODE_T         lockMode;
    WMB_LOCK_STATUS_T       lockStatus;
    WMB_TID_T               tid;
    WMB_LOCK_HEAD_T         *pHead;
    struct WMB_LOCK_REQ_S   *pNextLock;
    struct WMB_LOCK_REQ_S   *pTranNext;
    struct WMB_LOCK_REQ_S   *pTranPrev;
    }WMB_LOCK_REQ_T;



/* Access macros    */
#define wmbLockHeadLockReqGet(x)        (x->pLockReq)
#define wmbLockHeadLockReqSet(x, y)     (x->pLockReq = y)
#define wmbLockHeadGrantedGet(x)        (x->granted)
#define wmbLockHeadGrantedSet(x, y)     (x->granted = y)
#define wmbLockHeadNumLocksGet(x)       (x->numLocks)
#define wmbLockHeadNumLocksSet(x, y)    (x->numLocks = y)
#define wmbLockHeadSemHeadGet(x)        (x->semHead)
#define wmbLockHeadSemHeadSet(x, y)     (x->semHead = y)
#define wmbLockHeadSemExclusiveGet(x)   (x->semExclusive)
#define wmbLockHeadSemExclusive(x, y)   (x->semExclusive = y)
#define wmbLockHeadSemSharedGet(x)      (x->semShared)
#define wmbLockHeadSemShared(x, y)      (x->semShared = y)
#define wmbLockReqLockCountGet(x)       (x->lockCount)
#define wmbLockReqLockCountSet(x, y)    (x->lockCount = y)
#define wmbLockReqLockModeGet(x)        (x->lockMode)
#define wmbLockReqLockModeSet(x, y)     (x->lockMode = y)
#define wmbLockReqLockStatusGet(x)      (x->lockStatus)
#define wmbLockReqLockStatusSet(x, y)   (x->lockStatus = y)
#define wmbLockReqTidGet(x)             (x->tid)
#define wmbLockReqTidSet(x, y)          (x->tid = y)
#define wmbLockReqHeadGet(x)            (x->pHead)
#define wmbLockReqHeadSet(x, y)         (x->pHead = y)
#define wmbLockReqNextLockGet(x)        (x->pNextLock)
#define wmbLockReqNextLockSet(x, y)     (x->pNextLock = y)
#define wmbLockReqTranNextGet(x)        (x->pTranNext)
#define wmbLockReqTranNextSet(x, y)     (x->pTranNext = y)
#define wmbLockReqTranPrevGet(x)        (x->pTranPrev)
#define wmbLockReqTranPrevSet(x, y)     (x->pTranPrev = y)


#ifndef __WM_NO_LOCKS__

#ifdef __cplusplus
extern "C" {
#endif


STATUS  wmbLockMgrInit (void);
STATUS  wmbLockMgrStop (void);

STATUS  wmbLock      (WMB_OBJ_T obj, WMB_TID_T tid, WMB_LOCK_MODE_T mode);
STATUS  wmbUnlock    (WMB_OBJ_T obj, WMB_TID_T tid);
STATUS  wmbAllUnlock (WMB_TID_T tid);
STATUS  wmbLockHeadNew (WMB_LOCK_HEAD_T **ppLockHead);
STATUS  wmbLockHeadFree (WMB_LOCK_HEAD_T *pLockHead);
WMB_LOCK_REQ_T  * wmbLockFromTIDFind (WMB_LOCK_HEAD_T * pHead,
                                      WMB_TID_T tid);
STATUS  wmbLockAdd (WMB_LOCK_HEAD_T * pHead, WMB_TID_T tid,
                    WMB_LOCK_MODE_T mode,WMB_LOCK_REQ_T * * ppLock);
STATUS  wmbLockRemove (WMB_LOCK_HEAD_T *pHead, WMB_TID_T tid);
WMB_TID_T wmbLockDeadlockFind(WMB_LOCK_REQ_T  *pLock, BOOL bStrict);


#ifdef __cplusplus
}
#endif

#else

#define wmbLock(obj, tid, mode)     OK
#define wmbUnlock(obj, tid)         OK
#define wmbUnlockAll(tid)           OK
#define wmbDeadlockFind(pLock)      NULL

#endif /* __WM_NO_LOCKS__ */

#endif /* __INCwmbLockMgrLibh */
