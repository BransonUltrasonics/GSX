/* wmbObjectLib.h - RCB object management library header */

/* Copyright (c) 2004-2012 Wind River Systems, Inc. 
*
* The right to copy, distribute, modify, or otherwise make use 
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01h,18sep12,shi Add APIs wmbObjectValueExportAdapt() and wmbObjectValueImportAdapt().
                requirement WIND00270285
01g,28mar11,r_w fix defect WIND00257014
                to make some declares of functions comply with definitions
01f,30dec10,m_z enable windmark range test
                CQID: Fix WIND00245970
01e,17dec10,m_z part of APIs changed
                CQID: Fix WIND00247096
01d,30mar10,f_f VxWorks 64bit support (WIND00193649)
01c,12oct05,wdz '#ifdef __cplusplus' statements added
                for SPR#98444
01b,04apr04,dlr SPR 92292
01a,19feb04,jws add mod history and copyright, SPR92723
*/

#ifndef __INCwmbObjectLibh
#define __INCwmbObjectLibh
#ifdef __cplusplus
#include "wmb.h"
#include "wmOidLib.h"
#endif /* __cplusplus */

/* path delimiter */

#ifndef WMB_DELIM_K
#define WMB_DELIM_K '/'
#endif

/* prefix to indicate variable is temporary, or local */

#ifndef WMB_LOCAL_PREFIX_K
#define WMB_LOCAL_PREFIX_K '$'
#endif

typedef void        WMB_TRANS_STATE_T;
typedef ubyte4      WMB_ASYNC_CONTROL_T;
typedef ubyte4      WMB_PRODUCER_STATE_T;
typedef ubyte4      WMB_TYPE_NATIVE_T;          /* Native data type */
typedef ubyte       WMB_TRANSACTION_TYPE_T;
typedef ubyte2      WMB_TYPE_LOCAL_T;           /* Module specific data type */

typedef VOIDFUNCPTR         WMB_COMPLETE_T;

#ifdef __cplusplus
/*enum OBJ_ID
    {
    OBJ_ID_INVALID_K,
    OBJ_ID_NONE_K,
    OBJ_ID_SESSION_K,
    OBJ_ID_COMPONENT_K
    };*/
#else
enum OBJ_ID
    {
    OBJ_ID_INVALID_K,
    OBJ_ID_NONE_K,
    OBJ_ID_SESSION_K,
    OBJ_ID_COMPONENT_K
    };
#endif /* __cplusplus */

/* flags for object status */

#define OBJ_FLAG_DATA_PTR_K     0x00000001  /* data is a pointer            */
#define OBJ_FLAG_DATA_FREE_K    0x00000002  /* free data on close           */
#define OBJ_FLAG_DATA_BUFFER_K  0x00000004  /* data is buffer               */
#define OBJ_FLAG_DATA_SHARED_K  0x00000008  /* data is in shared memory     */

#define OBJ_FLAG_DATA_K        (OBJ_FLAG_DATA_PTR_K     | \
                                OBJ_FLAG_DATA_FREE_K    | \
                                OBJ_FLAG_DATA_BUFFER_K)

#define OBJ_FLAG_NAME_PTR_K     0x00000010  /* data is a pointer            */
#define OBJ_FLAG_NAME_FREE_K    0x00000020  /* free name on close           */
#define OBJ_FLAG_NAME_BUFFER_K  0x00000040  /* name is buffer               */
#define OBJ_FLAG_NAME_SHARED_K  0x00000080  /* name is in shared memory     */

#define OBJ_FLAG_NAME_K        (OBJ_FLAG_NAME_PTR_K     | \
                                OBJ_FLAG_NAME_FREE_K    | \
                                OBJ_FLAG_NAME_BUFFER_K  | \
                                OBJ_FLAG_NAME_SHARED_K)

#define OBJ_FLAG_INST_BUFFER_K  0x00000100  /* instance is a buffer         */
#define OBJ_FLAG_INST_SHARED_K  0x00000200  /* instance is a buffer         */
#define OBJ_FLAG_INST_PTR_K     0x00000400  /* instance is a buffer         */

#define OBJ_FLAG_GROUP          0x00000800  /* First object in group        */
#define OBJ_FLAG_FREE_K         0x00001000  /* free obj structure on delete */
#define OBJ_FLAG_REMOTE_K       0x00002000  /* obj in different domain      */
#define OBJ_FLAG_RESOLVED_K     0x00004000  /* remote obj has a handler     */
#define OBJ_FLAG_TRANSACTION_K  0x00008000  /* obj is member of transaction */
#define OBJ_FLAG_COOKIE_UNDO_K  0x00010000  /* cookie is undo buffer        */
#define OBJ_FLAG_HANDLER_K      0x00020000  /* have a handler - we're done  */
#define OBJ_FLAG_COOKIE_TITLE_K 0x00040000  /* cookie is column title       */


typedef struct WMB_OBJECT_T
    {
#ifndef __NO_MAGIC__
    ubyte4                  magic;          /* data integrity */
#endif
    struct WMB_OBJECT_T *   pNext;          /* Next entry in list       */
    struct WMB_OBJECT_T *   pGroup;         /* Next entry in group      */
    struct WMB_OBJECT_T *   pJump;          /* Jump to next group       */
    void *                  pName;          /* Object name              */
    ubyte4                  namePathLen;    /* Offset to object name    */
    ubyte4                  flags;          /* Flags for this object    */
    WM_BUFFER_T *           pInstance;      /* Object instance          */
    WMB_HANDLER_T           handler;        /* Data access handler      */
    void *                  pValue;         /* Value of this object     */
    STATUS                  status;         /* Completion status        */
    WMB_DB_T *              pDB;            /* database                 */
    DB_ENTRY_T *            pEntry;         /* database entry           */
    ubyte4                  localId;        /* lookup by this id        */
    void *                  pCookie;        /* generic pointer (cookie) */
    } WMB_OBJECT_T;


typedef struct REMOTE_T
    {
    OS_SEM_T    sem;   /* semaphore for sychronizing */
    OS_MSGQ_T   qid;   /* remote queue */
    WMB_CID_T   cid;   /* remote component */
    WMB_TID_T   tid;   /* remote tid    */
    WMB_OBJ_T   obj;   /* remote object */
    WM_BUFFER_T * pValue; /* shared memory value    */
    MEM_POOL_ID   pool;   /* pool for shared memory */
    }
    REMOTE_T;


#define wmbObjectNew(x)                 (NULL != (x = wmMalloc(sizeof(wmbObject(x)))))
#define wmbObject(x)                ((WMB_OBJECT_T *) x)
#define MAGIC_OBJECT_K              0xcacadada
#define wmbObjectBless(x)           wmMagicBless(wmbObject(x), MAGIC_OBJECT_K)
#define wmbObjectBad(x)             wmMagicBad  (wmbObject(x), MAGIC_OBJECT_K)
#define wmbObjectGood(x)            wmMagicGood (wmbObject(x), MAGIC_OBJECT_K)

#define wmbObjectNameOnlyGet(x)     memberGet(wmbObject(x), name.pBuf) + x->namePathLen
#define wmbObjectNameFullGet(x)     memberGet(wmbObject(x), name.pBuf)

#define wmbObjectFlagIsSet(x,y)     FLAG_SET(wmbObject(x), y)
#define wmbObjectFlagSet(x,y)       SET_FLAG(wmbObject(x), y)
#define wmbObjectFlagClear(x,y)     CLEAR_FLAG(wmbObject(x), y)
#define wmbObjectInstanceGet(x)     ((WM_BUFFER_T*)(memberGet(wmbObject(x), pInstance)))
/* wmbObjectInstanceSet is a function. See below    */

#define wmbObjectPathlenGet(x)      memberValue(wmbObject(x), namePathLen, 0)
#define wmbObjectPathlenSet(x,y)    memberSet(wmbObject(x), namePathLen, y)

#define wmbObjectListNextSet(x, y)      memberSet(wmbObject(x), pNext, y)
#define wmbObjectListNextGet(x)         memberGet(wmbObject(x), pNext)
#define wmbObjectNextInList(x)          wmbObjectListNextGet(x)
#define wmbObjectNextSet(x, y)          memberSet(wmbObject(x), pGroup,  y)
#define wmbObjectNextGet(x)             memberGet(wmbObject(x), pGroup)
#define wmbObjectGroupNextSet(x, y)     memberSet(wmbObject(x), pJump,  y)
#define wmbObjectGroupNextGet(x)        memberGet(wmbObject(x), pJump)

#define wmbObjectLocalSet(x, y)         memberSet(wmbObject(x), localId, y)

#define wmbObjectSizeGet(x)         (wmbObjectValueGet(x) ? wmBufferUsedGet(wmbObjectValueGet(x)) : 0)
#define wmbObjectHandlerSet(x,y)    memberSet(wmbObject(x), handler, y)
#define wmbObjectDBGet(x)           memberGet(wmbObject(x), pDB)
#define wmbObjectDBSet(x,y)         memberSet(wmbObject(x), pDB, y)
#define wmbObjectEntrySet(x,y)      memberSet(wmbObject(x), pEntry, y)
#define wmbObjectBPGet(x)           memberGet(wmbObject(x), backplane)
#define wmbObjectBPSet(x,y)         memberSet(wmbObject(x), backplane, y)

#if defined __WMB_VIEW_ACCESS_MODEL__
#define wmbObjectAccessGroupGet(x)  NULL
#endif

#define wmbObjectIterate(x, y) \
    for (y = wmbTransactionHeadGet(x); y != NULL; y = wmbObjectNextGet(y))


#ifdef __cplusplus
extern "C" {
#endif

STATUS wmbObjectSet (WMB_SESSION_T * pSession, sbyte * pName, void * pInstance,
                     sbyte4 instanceLength, sbyte * pData, long dataLength,
                     DATATYPE_T dataType);

STATUS wmbObjectGet (WMB_SESSION_T * pSession,COMMAND_T command, sbyte *pName, void  *pInstance,
                    sbyte4 instanceLength, sbyte *pBuffer, long bufferLength, DATATYPE_T bufferType,
                    BOOL copy, long *pWritten);

STATUS wmbObjectGetNext (WMB_SESSION_T * pSession, sbyte * pName, void * pInstance,
 					sbyte4* pInstanceLength, sbyte4 maxInstanceLength, DATATYPE_T instanceType,
					sbyte * pBuffer, long bufferLength, DATATYPE_T bufferType, BOOL copy, long * pWritten);

STATUS wmbObjectExecFar(WMB_CID_T component, MSG_IPC_T * pIpc);

STATUS wmbObjectDone       (WMB_CID_T component, sbyte * pMsg, sbyte4 msgSize);
STATUS wmbObjectGoFar      (WMB_CID_T component, sbyte * pMsg, sbyte4 msgSize);
STATUS wmbObjectFound      (WMB_CID_T component, sbyte * pMsg, sbyte4 msgSize);
STATUS wmbObjectResolveFar (WMB_CID_T component, sbyte * pMsg, sbyte4 msgSize);
WM_BUFFER_T * wmbObjectNameBufferGet (WMB_OBJ_T obj);
sbyte * wmbObjectNameGet(WMB_OBJ_T   obj);
WMB_CID_T wmbObjectComponentGet (WMB_OBJ_T obj);
STATUS  wmbObjectInstanceFromValue (WMB_OBJ_T obj);
STATUS  wmbObjectInstanceAppendString (WMB_OBJ_T obj, sbyte * pInstance);
BOOL    wmbObjectInstanceIsEmpty (WMB_OBJ_T obj);

ubyte4  wmbObjectAppend (WMB_OBJ_T pList, WMB_OBJ_T pNew);
STATUS  wmbObjectInsert (WMB_TID_T tid, WMB_OBJ_T  pObj);
STATUS  wmbObjectCreate (WMB_TID_T tid, sbyte * pName, void * pInstance,
                         sbyte4 instanceLength, WMB_OBJ_T * pObject);
void    wmbObjectDelete (WMB_OBJ_T  pObj);
STATUS  wmbObjectEnum   (WMB_TID_T tid, WMB_OBJ_T pObj, COMMAND_T cmd);
STATUS  wmbObjectGo     (WMB_TID_T tid, WMB_OBJ_T pObj, COMMAND_T cmd);
STATUS  wmbObjectPrev   (WMB_TID_T tid);
STATUS  wmbObjectTest   (WMB_TID_T tid, WMB_OBJ_T pObj, COMMAND_T cmd);
void    wmbObjectGroup  (WMB_OBJ_T prior, WMB_OBJ_T added);
void    wmbObjectGroupListSet  (WMB_OBJ_T first, WMB_OBJ_T last);
void    wmbObjectGroupClear (WMB_OBJ_T head);

STATUS  wmbObjectVariable (WMB_TID_T tid, WMB_OBJ_T pObj, COMMAND_T cmd);

STATUS  wmbObjectResolver (WMB_SESSION_T * pSession, WMB_CID_T cid, WMB_OBJ_T obj);

#define wmbObjectResolve(obj)              wmbObjectResolver(NULL, NULL, obj)
#define wmbObjectResolveSession(s, obj)    wmbObjectResolver(s, NULL, obj)
#define wmbObjectResolveComponent(c, obj)  wmbObjectResolver(NULL, c, obj)

STATUS  wmbObjectLoad (WMB_OBJ_T obj);
STATUS  wmbObjectStore (WMB_OBJ_T obj);

ubyte4  wmbObjectTypeLocalGet  (WMB_OBJ_T obj);
void    wmbObjectTypeLocalSet  (WMB_OBJ_T obj, ubyte4 type);
ubyte4  wmbObjectTypeNativeGet (WMB_OBJ_T obj);
void    wmbObjectTypeNativeSet (WMB_OBJ_T obj, ubyte4 type);


sbyte *  wmbObjectNamePtrGet(WMB_OBJ_T obj);
void     wmbObjectNamePtrSet(WMB_OBJ_T obj, sbyte * pName);

STATUS wmbObjectInit (WMB_OBJ_T obj, MEM_POOL_ID pool, sbyte * pName,
                    char * pInstance, sbyte4 instanceLength, BOOL copy);

STATUS wmbObjectGoInvalid  (WMB_TID_T tid, WMB_OBJ_T obj, CMD_T cmd);
STATUS wmbObjectGoComplete (WMB_TID_T tid, WMB_OBJ_T obj, CMD_T cmd);
STATUS wmbObjectGoGet      (WMB_TID_T tid, WMB_OBJ_T obj, CMD_T cmd);
STATUS wmbObjectGoSet      (WMB_TID_T tid, WMB_OBJ_T obj, CMD_T cmd);
STATUS wmbObjectGoAbort    (WMB_TID_T tid, WMB_OBJ_T obj, CMD_T cmd);
STATUS wmbObjectGoTest     (WMB_TID_T tid, WMB_OBJ_T obj, CMD_T cmd);
STATUS wmbObjectGoCommit   (WMB_TID_T tid, WMB_OBJ_T obj, CMD_T cmd);
STATUS wmbObjectGoCopy     (WMB_TID_T tid, WMB_OBJ_T obj, CMD_T cmd);
STATUS wmbObjectValueSet   (WMB_TID_T tid, WMB_OBJ_T  pObj, void * pValue,
                            sbyte4 length, DATATYPE_T dataType, BOOL copy);

WM_OID_T    wmbObjectOidLoad (WMB_SESSION_T * pSession, WMB_OBJ_T obj);

STATUS wmbObjectExport     (WMB_OBJ_T   obj, void * pDest,      
                            size_t destSize, DATATYPE_T  destType, size_t * pWritten);                            
STATUS wmbObjectImport     ( WMB_OBJ_T  obj, void * pSrc,
                            size_t srcSize, DATATYPE_T  srcType);
STATUS wmbObjectValueCreate (WMB_OBJ_T obj, void * pData,
                             size_t dataLength, sbyte4 dataType, BOOL copy);

void wmbObjectValueBufferSet (WMB_OBJ_T obj, WM_BUFFER_T * pBuffer);

WM_BUFFER_T *   wmbObjectValueGet (WMB_OBJ_T obj);
void            wmbObjectInstanceSet (WMB_OBJ_T obj, WM_BUFFER_T * pInstance);
sbyte4          wmbObjectLocalGet (WMB_OBJ_T obj);

void *          wmbObjectCookieGet (WMB_OBJ_T obj);
void            wmbObjectCookieSet (WMB_OBJ_T obj, void * pCookie);

sbyte4          wmbObjectLevelGet(WMB_OBJ_T obj);
DB_ENTRY_T *    wmbObjectEntryGet(WMB_OBJ_T obj);
DB_TABLE_T *    wmbObjectTableGet(WMB_OBJ_T obj);
void            wmbObjectStatusSet(WMB_OBJ_T obj, sbyte4 status);
sbyte4          wmbObjectStatusGet(WMB_OBJ_T obj);
WMB_HANDLER_T   wmbObjectHandlerGet(WMB_OBJ_T obj);
BOOL            wmbObjectHandlerLocked(WMB_OBJ_T obj);
sbyte *         wmbObjectNameBaseGet(WMB_OBJ_T obj);

ssize_t    wmbObjectNativeSizeGet(WMB_OBJ_T obj);
STATUS  wmbObjectSavedGet  (WMB_OBJ_T obj, void * pData, size_t size, sbyte4 type);
STATUS  wmbObjectSavedSet  (WMB_OBJ_T obj, void * pData, size_t size, sbyte4 type);

void    wmbObjectSavedFree (WMB_OBJ_T obj);
WM_BUFFER_T * wmbObjectStoredGet(WMB_OBJ_T obj);

STATUS  wmbObjectUndo(WMB_OBJ_T obj);
STATUS  wmbObjectRedo(WMB_OBJ_T obj);

void *  wmbObjectInstancePtrGet (WMB_OBJ_T obj);
STATUS wmbObjectInstanceSuffixGet(WMB_OBJ_T obj, ubyte4  **ppSuffix, sbyte4 *suffixLen);

STATUS  wmbObjectOid (WMB_OBJ_T obj, ubyte4 * * pOid, sbyte4 * pLen);
sbyte * wmbObjectValidationString(WMB_OBJ_T obj);

STATUS wmbObjectMake
    (
    WMB_TID_T       tid,
    WMB_OBJ_T       obj,
    sbyte *         pName,
    void *          pInstance,
    sbyte4          instanceLen,
    sbyte *         pBuffer,
    ssize_t         bufferLen,
    ubyte4          bufferType,
    BOOL            copy
    );

STATUS  wmbObjectLock (WMB_TID_T tid, WMB_OBJ_T obj, COMMAND_T cmd);
void    wmbObjectUnlock (WMB_TID_T tid, WMB_OBJ_T obj);
STATUS  wmbObjectPermission (WMB_SESSION_T * pSession, WMB_OBJ_T obj, COMMAND_T command);
STATUS  wmbObjectSetRangeTest ( WMB_OBJ_T obj);

STATUS wmbObjectValueExportAdapt(WMB_OBJ_T obj,DATATYPE_T exportType);
STATUS wmbObjectValueImportAdapt(WMB_OBJ_T obj,DATATYPE_T importType);
 
#ifdef __cplusplus
}
#endif

#define wmbObjectStateGet   wmbObjectCookieGet
#define wmbObjectStateSet   wmbObjectCookieSet

#endif /* __INCwmbObjectLibh */
