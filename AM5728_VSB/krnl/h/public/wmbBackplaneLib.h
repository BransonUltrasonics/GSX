/* wmbBackplaneLib.h - backplane header */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
12oct05,wdz      '#ifdef __cplusplus' statements added
                 for SPR#98444
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbBackplaneLibh
#define __INCwmbBackplaneLibh

#ifdef __cplusplus
#include "wmMemBlockLib.h"
#include "wmnet.h"
#endif /* __cplusplus */

/* WMB Event Id's. These are inserted in eventID field of WMB_EVENT_T structure */
#define WMB_BACKPLANE_CREATED_EVENT     0x0001
#define WMB_BACKPLANE_DELETED_EVENT     0x0002

typedef struct _WMB_POOL_T
    {
    WM_BLOCK_T *    pObjects;
    WM_BLOCK_T *    pBuffer;
    MEM_POOL_ID     pool;
    void *          pData;
    } WMB_POOL_T;

#define WMB_BACKPLANE_NAME_MAX_K    32

#if defined __WMB_VIEW_ACCESS_MODEL__
typedef struct WMB_VIEW_DB_S
    {
    struct WMB_COMPONENT_T          *pComponent;
    ubyte2                          numViews;
    struct WMB_VACM_VIEW_ENTRY_S    *pEntry;
    struct WMB_VIEW_DB_S            *pNext;
    }WMB_VIEW_DB_T;
#endif


typedef struct _WMB_BACKPLANE_T
    {
#ifndef __NO_MAGIC__
    ubyte4                      magic;      /* data integrity */
#endif
    OS_SOCKET_T                 sock;
    ubyte2                      port;
    ubyte2                      flags;
    WMB_DOMAIN_T                domain;
    sbyte                       name[WMB_BACKPLANE_NAME_MAX_K];
    struct WMB_COMPONENT_T *    components[32];
    ubyte2                      componentCount;
    ubyte4                      startTime;
    DTTypeInfo *                accessInfo;
    struct WMB_DB_T *           pDB;
    struct WMB_DB_T *           pTemp;      /* session variables    */
#ifdef __cplusplus
    WMB_SESSION_T *      pSession;   /* default session      */
#else
    struct WMB_SESSION_T *      pSession;   /* default session      */
#endif /* __cplusplus */
#if defined __WMB_VIEW_ACCESS_MODEL__
    ubyte2                      numViewDB;
    WMB_VIEW_DB_T               *pViewDB;
#endif
    OS_MEM_SHARE_T              shared;     /* shared memory info   */
    WMB_POOL_T                  pooled;     /* local pooled memory  */
    } WMB_BACKPLANE_T;

/* to register a non-primary backplane */

typedef struct WMB_REG_T
    {
    ubyte4      cmd;
    OS_MSGQ_T   qid;
    sbyte       name[32];
    }
    WMB_REG_T;


#define WMB_BP_MASTER_K         0x0001
#define WMB_BP_DOMAINS_K        0x0002
#define WMB_BP_UDP_K            0x0004
#define WMB_BP_ALLOC_NAME_K     0x0008

#define wmbBackplane(x)         ((WMB_BACKPLANE_T *) x)
#define wmbBackplaneBless(x)    wmMagicBless (wmbBackplane(x), MAGIC_WMB_HEADER_K)
#define wmbBackplaneBad(x)      wmMagicBad   (wmbBackplane(x), MAGIC_WMB_HEADER_K)

#define STRDUPE(dest, src)  (NULL != (dest = wmMalloc(STRLEN((char *) src)))) ? STRCPY((char *) dest, (char *) src) : 0

#define wmbBackplaneStartGet(x)             wmbBackplane(x)->startTime
#define wmbBackplaneNameGet(x)              wmbBackplane(x)->name
/*
#define wmbBackplaneDBGet(x)                wmbBackplane(x)->pDB
*/
#if defined __WMB_VIEW_ACCESS_MODEL__
#define wmbBackplaneViewDBGet(x)            wmbBackplane(x)->pViewDB
#define wmbBackplaneViewDBSet(x,y)          (wmbBackplane(x)->pViewDB = y)
#define wmbBackplaneIncNumViewDB(x)         wmbBackplane(x)->numViewDB++;
#define wmbBackplaneDecNumViewDB(x)         wmbBackplane(x)->numViewDB--;
#define wmbBackplaneNumViewDBGet(x)         wmbBackplane(x)->numViewDB;
#define wmbBackplaneNumViewDBSet(x,y)       wmbBackplane(x)->numViewDB = y;
#endif /* __WMB_VIEW_ACCESS_MODEL__ */


#define wmbBackplaneComponentCountGet(x)    wmbBackplane(x)->componentCount
#define wmbBackplaneComponentsGet(x)        wmbBackplane(x)->components
#define wmbBackplaneEventsGet(x)            wmbBackplane(x)->pEvents
#define wmbBackplaneAccessGet(x)            wmbBackplane(x)->accessInfo
#define wmbBackplaneSessionGet(x)           wmbBackplane(x)->pSession
#define wmbBackplaneObjectsGet(x)           wmbBackplane(x)->pooled.pObjects
#define wmbBackplaneBuffersGet(x)           wmbBackplane(x)->pooled.pBuffer
#define wmbBackplaneDataGet(x)              wmbBackplane(x)->pooled.pData
#define wmbBackplaneTempGet(x)              wmbBackplane(x)->pTemp
#define wmbBackplaneMemGet(x)               wmbBackplane(x)->pooled.pool
#define wmbBackplanePoolGet(x)             &(wmbBackplane(x)->pooled)


/* prototypes */

#ifdef __cplusplus
extern "C" {
#endif

STATUS  wmbBackplaneBind (WMB_ID_T id, WMB_CID_T component);
STATUS  wmbBackplaneComponentGet (WMB_ID_T backplane, WMB_CID_T * pComponent);
STATUS  wmbBackplaneComponentCreate (WMB_ID_T backplane, WMB_CID_T * pComponent);
STATUS  wmbBackplaneComponentDestroy(WMB_CID_T component);
void    wmbBackplaneClose (sbyte * pName);
STATUS  wmbBackplaneCreate (sbyte * pName, sbyte4 memSize,
                            sbyte4 dbSize, sbyte4 dbTableSize,
                            DTTypeInfo * pAccessInfo, WMB_ID_T * pBackplane);
/*
STATUS  wmbBackplaneDBAdd (WMB_ID_T wmb_id, struct WMB_DB_T * pDB);
*/
struct WMB_DB_T * wmbBackplaneDBGet (WMB_ID_T wmb_id);
STATUS  wmbBackplaneStop (void);
WMB_ID_T wmbBackplaneGet (void);
STATUS  wmbBackplaneStart (void);
WMB_ID_T wmbBackplaneLookup (sbyte * pBackplane);
BOOL    wmbBackplaneMaster(void);
void    wmbBackplaneNameSet(WMB_ID_T wmb, sbyte * pName);
STATUS  wmbBackplaneRegister (sbyte * pName);
void    wmbBackplaneShareSend (OS_MSGQ_T qid);
OS_MEM_SHARE_T * wmbBackplaneSharedGet(WMB_ID_T backplane);
void    wmbBackplaneShareSet (sbyte * pBuffer, sbyte4 bufSize);
void    wmbBackplaneShutdown (sbyte * pName);

#if defined __WMB_VIEW_ACCESS_MODEL__

STATUS  wmbBackplaneViewAdd    (WMB_ID_T backplane, WMB_VIEW_DB_T * viewDB);
STATUS  wmbBackplaneViewDelete (WMB_ID_T backplane, WMB_VIEW_DB_T * viewDB);

#endif /* __WMB_VIEW_ACCESS_MODEL__ */

#ifdef __cplusplus
}
#endif

#endif /* __INCwmbBackplaneLibh */
