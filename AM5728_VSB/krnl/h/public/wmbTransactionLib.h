/* wmbTransactionLib.h - transaction methods for RCB */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
12oct05,wdz      changed '{' to 'extern "C" {'
                 for SPR#98444
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbTransactionLibh
#define __INCwmbTransactionLibh

/*
Transaction Manager Notification definitions. Used if callback function is provided
*/
typedef enum
    {
    WMB_TRANSACTION_BEGIN_K = 0x0001,
    WMB_TRANSACTION_ABORT_K,
    WMB_TRANSACTION_TEST_K,
    WMB_TRANSACTION_COMMIT_K,
    WMB_TRANSACTION_ERROR_K,
    WMB_TRANSACTION_COMPLETE_K,
    WMB_TRANSACTION_DEADLOCK_K,
    WMB_TRANSACTION_TIMEOUT_K
    }WMB_TRANSACTION_NOTIFY_T;


/* 1.1.3.3  Transaction Structure
The transaction structure is passed an argument for all RCB2 transactions.  It contains information about this specific transaction.  It also points to the object list that includes information about the resources on which this transaction should be performed.
*/

typedef void (*WMB_TRANS_CALLBACK_T)(WMB_TID_T tid, WMB_TRANSACTION_NOTIFY_T, void *pAsyncCtl);

#define WMB_TM_POLLING_INTERVAL          1000   /* 1 second     */
#define WMB_DEFAULT_TRANSACTION_TIMEOUT    10   /* 10 seconds   */
#define WMB_TM_NOTIMEOUT                    0

/*
Flags for creating transactions
*/
#define WMB_TM_NOUNDO           0x00000001
#define WMB_TM_SYNCONLY         0x00000002
#define WMB_TM_DOASYNC          0x00000004
#define WMB_TM_DOSYNC           0x00000008
#define WMB_TM_RETRY            0x00000010
#define WMB_TM_NOTIFY           0x00000020

/*
Transaction states
*/
typedef enum
    {
    TR_NULL = 0,
    TR_ACTIVE,
    TR_TESTING,
    TR_TESTED,
    TR_COMMITTING,
    TR_ABORTING,
    TR_REQUESTING,
    TR_COMPLETED
    } WMB_TRSTATE;

#define WMB_TRSTATE_LIMIT   TR_COMPLETED
typedef WMB_TRSTATE     TRANSACTION_STATE_T;

typedef struct _WMB_TRANSACTION_T
    {
#ifndef __NO_MAGIC__
    ubyte4                  magic;          /* data integrity */
#endif
    WMB_TRANSACTION_T *     pNext;
    WMB_TRANSACTION_T *     pSessionNext;   /* Next transaction of the session  */
    WMB_SESSION_T *         pSession;       /* The session that owns this tranaction */
    WMB_OBJECT_T *          pQueue;         /* Flat list of objects             */
    WMB_OBJECT_T *          pErrQueue;      /* List of objects where the last command wasn't run */
    OS_MEM_SHARE_T *        pMemory;        /* shared memory                    */
    MEM_POOL_ID             pool;           /* transaction memory pool          */
    OS_MSGQ_T               qid;            /* msg queue to reply to            */
#if defined __WMB_TM_EXPLICIT_TID__
    WMB_TID_T               tid;            /* Transaction ID                   */
#endif
    OS_SEM_T                sem;            /* Transaction control semaphore    */
    TRANSACTION_STATE_T     state;          /* transaction handling     */
    STATUS                  status;         /* overall status (error)   */
    COMMAND_T               command;        /* Transaction type (get, set, etc.  */
    ubyte2                  callCount;      /* Number of handler calls in progress */
    ubyte4                  flags;          /* Transaction status       */
    ubyte4                  numObject;      /* objects in this transaction    */
    ubyte4                  timeout;        /* Transaction timeout period for commit or rollback */
    ubyte4                  created;        /* Point in time the transaction was created    */
    sbyte *                 pPath;          /* current offset in eval   */
    ubyte4                  pathLen;        /* length of pPath          */
    void *                  pAsyncCtl;      /* asych control info       */
    WMB_TRANS_CALLBACK_T    pFnComp;        /* Asynch completion        */
    WMB_LOCK_REQ_T          *pLockList;     /* List of locks this transaction has   */
    } _WMB_TRANSACTION_T;

/* 1.1.3.3.2    Transaction Structure Access */

#define wmbTransaction(x)                   ((_WMB_TRANSACTION_T *) x)

#define wmbTransactionSessionGet(tx)        wmbTransaction(tx)->pSession
#define wmbTransactionNextGet(tx)           wmbTransaction(tx)->pNext
#define wmbTransactionSessionNextGet(tx)    wmbTransaction(tx)->pSessionNext
#define wmbTransactionObjectGet(tx)         wmbTransaction(tx)->pObject
#define wmbTransactionCommandGet(tx)        wmbTransaction(tx)->command
#define wmbTransactionFlagsGet(tx)          wmbTransaction(tx)->flags
#define wmbTransactionStateGet(tx)          wmbTransaction(tx)->state
#if defined __WMB_TM_EXPLICIT_TID__
#define wmbTransactionTidGet(tx)            wmbTransaction(tx)->tid
#else
#define wmbTransactionTidGet(tx)            tx
#endif
#define wmbTransactionStatusGet(tx)         wmbTransaction(tx)->status
#define wmbTransactionFnCompGet(tx)         wmbTransaction(tx)->pFnComp
#define wmbTransactionAsyncCtlGet(tx)       wmbTransaction(tx)->pAsyncCtl
#define wmbTransactionTimeoutGet(tx)        wmbTransaction(tx)->timeout
#define wmbTransactionCreatedGet(tx)        wmbTransaction(tx)->created
#define wmbTransactionObjCountGet(tx)       wmbTransaction(tx)->numObject
#define wmbTransactionGroupGet(tx)          wmbTransaction(tx)->stGroup.pList
#define wmbTransactionLockGet(tx)           wmbTransaction(tx)->pLockList
#define wmbTransactionSemGet(tx)            wmbTransaction(tx)->sem
#define wmbTransactionMemGet(tx)            wmbTransaction(tx)->pool
#define wmbTransactionConsumerTypeGet(tx)   wmbSessionTypeGet(wmbTransactionSessionGet(tx))

#define wmbTransactionSessionSet(tx, vx)    wmbTransaction(tx)->pSession    = vx
#define wmbTransactionObjectSet(tx, vx)     wmbTransaction(tx)->pObject     = vx
#define wmbTransactionCommandSet(tx, vx)    wmbTransaction(tx)->command     = vx
#define wmbTransactionFlagsSet(tx, vx)      wmbTransaction(tx)->flags      |= vx
#define wmbTransactionStateSet(tx, vx)      wmbTransaction(tx)->state       = vx
#if defined __WMB_TM_EXPLICIT_TID__
#define wmbTransactionTidSet(tx, vx)        wmbTransaction(tx)->tid         = vx
#else
#define wmbTransactionTidSet(tx, vx)
#endif
#define wmbTransactionStatusSet(tx, vx)     wmbTransaction(tx)->status      = vx
#define wmbTransactionFnCompSet(tx, vx)     wmbTransaction(tx)->pFnComp     = vx
#define wmbTransactionAsyncCtlSet(tx, vx)   wmbTransaction(tx)->pAsyncCtl   = vx
#define wmbTransactionTimeoutSet(tx, vx)    wmbTransaction(tx)->timeout     = vx
#define wmbTransactionCreatedSet(tx, vx)    wmbTransaction(tx)->created     = vx
#define wmbTransactionGroupSet(tx, vx)      wmbTransaction(tx)->stGroup.pList = vx
#define wmbTransactionObjCountSet(tx, vx)   wmbTransaction(tx)->objCount    = vx
#define wmbTransactionObjCountAdd(tx, vx)   wmbTransaction(tx)->objCount    += vx
#define wmbTransactionObjCountSub(tx, vx)   wmbTransaction(tx)->objCount    -= vx
#define wmbTransactionRedoCountSet(tx, vx)  wmbTransaction(tx)->redoCount   = vx
#define wmbTransactionRedoCountAdd(tx, vx)  wmbTransaction(tx)->redoCount   += vx
#define wmbTransactionRedoCountSub(tx, vx)  wmbTransaction(tx)->redoCount   -= vx
#define wmbTransactionUndoCountSet(tx, vx)  wmbTransaction(tx)->undoCount   = vx
#define wmbTransactionUndoCountAdd(tx, vx)  wmbTransaction(tx)->undoCount   += vx
#define wmbTransactionUndoCountSub(tx, vx)  wmbTransaction(tx)->undoCount   -= vx
#define wmbTransactionLockSet(tx, vx)       wmbTransaction(tx)->pLockList   = vx
#define wmbTransactionSemSet(tx, vx)        wmbTransaction(tx)->sem         = vx

#define makeTrans(x)                        ((_WMB_TRANSACTION_T *) x)

#define MAGIC_TID_K                         0xdeadbeef
#define UNBLESS_TID_K                       0xabbaabba
#define wmbTransactionBless(x)              wmMagicBless(x, MAGIC_TID_K)
#define wmbTransactionUnbless(x)            wmMagicBless(x, UNBLESS_TID_K)

#define wmbTransactionHeadGet(x)            (wmbTransactionBad(x) ? NULL : makeTrans(x)->pQueue)
#define wmbTransactionHeadSet(x,y)          (makeTrans(x)->pQueue = y)

#define wmbTransactionErrQGet(x)            (wmbTransactionBad(x) ? NULL : makeTrans(x)->pErrQueue)
#define wmbTransactionErrQSet(x,y)          (makeTrans(x)->pErrQueue = y)

/* Function Prototypes */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
STATUS wmbTransactionCreate
    (
    WMB_SESSION_T * pSession,   /* [in] Pointer to the Session that owns this transaction */
    ubyte4          timeout,    /* [in] Tranaction timeout period               */
    ubyte4          flags,      /* [in] Flags define how transactions are processed */
    WMB_TID_T *     pTid        /* [out] Returned ID for transaction when OK    */
    );
STATUS wmbTransactionBegin
    (
    WMB_TID_T   tid,    /* [in] Transaction ID to begin transaction     */
    COMMAND_T   cmd     /* [in] Command to process     */
    );
STATUS wmbTransactionTest
    (
    WMB_TID_T    tid    /* [in] Transaction ID of the transaction     */
    );
STATUS wmbTransactionCommit
    (
    WMB_TID_T    tid    /* [in] Transaction ID of the transaction     */
    );
STATUS wmbTransactionComplete
    (
    WMB_TID_T    tid    /* [in] Transaction ID of the transaction     */
    );
STATUS wmbTransactionAbort
    (
    WMB_TID_T    tid    /* [in] Transaction ID of the transaction     */
    );
STATUS wmbTransactionEnd
    (
    WMB_TID_T    tid    /* [in] Transaction ID of the transaction     */
    );
STATUS wmbTransactionProcess
    (
    WMB_TID_T   tid,    /* [in] Transaction ID of the transaction       */
    COMMAND_T   cmd     /* [in] Command to process (CMD_SET_K...)       */
    );
STATUS wmbTransactionObjectAdd
    (
    WMB_TID_T   tid,        /* [in] Transaction Identifier  */
    WMB_OBJ_T   obj         /* [in] Object to add           */
    );
STATUS wmbTransactionObjectRemove
    (
    WMB_TID_T   tid,        /* [in] Transaction Identifier  */
    WMB_OBJ_T   obj         /* [in] Object to remove        */
    );
BOOL wmbTransactionBad
    (
    WMB_TID_T   tid        /* [in] Transaction Identifier  */
    );
STATUS wmbTransactionLockDelete
    (
    WMB_TID_T       tid,    /* [in] Transaction Identifier  */
    WMB_LOCK_REQ_T *pLock   /* [in] The Lock to delete      */
    );
STATUS wmbTransactionLockAdd
    (
    WMB_TID_T       tid,    /* [in] Transaction Identifier  */
    WMB_LOCK_REQ_T  *pLock  /* [in] The Lock to Add      */
    );
void wmbTransactionSessionTidAppend
    (
    WMB_TID_T   tid,    /* [in] Pointer to the head of TCB to append */
    WMB_TID_T   head    /* [in] Head of the transaction list         */
    );
WMB_ID_T wmbTransactionBackplaneGet
    (
    WMB_TID_T       tid     /* [in] Transaction Identifier  */
    );
OS_MEM_SHARE_T * wmbTransactionSharedGet
    (
    WMB_TID_T tid       /* [in] Transaction Identifier  */
    );
OS_MSGQ_T wmbTransactionReplyGet
    (
    WMB_TID_T tid       /* [in] Transaction Identifier  */
    );
STATUS wmbTransactionStart(void);
void wmbTransactionStop(void);
BOOL wmbTransactionIsBestEffort
    (
    WMB_TID_T       tid     /* [in] Transaction Identifier  */
    );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCwmbTransactionLibh */
