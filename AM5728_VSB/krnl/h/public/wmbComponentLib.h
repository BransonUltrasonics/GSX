/* wmbComponentLib.h */

/* Copyright 2004-2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,30mar10,f_f  VxWorks 64bit support (WIND00193649)
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbComponenth
#define __INCwmbComponenth

#ifndef WMB_COMPONENT_REG_MAX_K
#define WMB_COMPONENT_REG_MAX_K     32
#endif

/*
1.1.2.1	Component Registration
The component registration structure is passed as an argument to the component
registration function.  It contains information about the component being
registered, including a function to call to de-register the component if
the backplane is shutting down.
*/

/*
typedef WMB_COMP_DEREG_T (struct WMB_RID_T *cid);
*/


#ifndef WMB_COMPONENT_NAME_MAX_K
#define WMB_COMPONENT_NAME_MAX_K 32
#endif

typedef struct WMB_COMPONENT_T
    {
#ifndef __NO_MAGIC__
    ubyte4              magic;          /* data integrity */
#endif
    WMB_ID_T            backplane;
    WMB_DOMAIN_T        domain;
    sbyte               name[WMB_COMPONENT_NAME_MAX_K];
    ubyte4            	flags;			/* flags associated with this component */
    WMBFUNCPTR          deregister;		/* de-registration callback */
    void *	            cookie;		    /* de-registration control information */
    OS_MSGQ_T           qid;            /* listener message queue */
    WMB_HANDLER_T       handler;        /* default object handler */
    WMB_POOL_T *        pPooled;
    void *              pState;
    WMB_LOCK_HEAD_T *   pLockHead;      /* Pointer to the head of the lock queue    */
    }
    WMB_COMPONENT_T;

/* 1.1.2.1.1	Component Registration Structure Creation and Destruction */


#define wmbComponent(x)                 ((WMB_COMPONENT_T *) x)

#define wmbComponentNameGet(x)          wmbComponent(x)->name
#define wmbComponentCidGet(x)           wmbComponent(x)->cid
#define wmbComponentFlagsGet(x)	        wmbComponent(x)->flags
#define wmbComponentDeregGet(x)         wmbComponent(x)->deregister
#define wmbComponentCtrlGet(x)          wmbComponent(x)->cookie
#define wmbComponentQidGet(x)           wmbComponent(x)->qid
#define wmbComponentListenerGet(x)      wmbComponent(x)->listener
#define wmbComponentStateGet(x)         wmbComponent(x)->pState
#define wmbComponentObjectsGet(x)       wmbComponent(x)->pPooled->pObjects
#define wmbComponentBuffersGet(x)       wmbComponent(x)->pPooled->pBuffer
#define wmbComponentDataGet(x)          wmbComponent(x)->pPooled->pData
/*
#define wmbComponentMemGet(x)           wmbComponent(x)->pPooled->pool
*/
#define wmbComponentLockHeadGet(x)      wmbComponent(x)->pLockHead
#define wmbComponentHandlerGet(x,y)     wmbComponent(x)->handler
#define wmbComponentPoolGet(x)          wmbComponent(x)->pPooled

#define MAGIC_COMPONENT_K       0xabbafaba

#define wmbComponentBless(x)    wmMagicBless (wmbComponent(x), MAGIC_COMPONENT_K)
#define wmbComponentBad(x)      wmMagicBad   (wmbComponent(x), MAGIC_COMPONENT_K)


/*2.1.1	Component Registration and Deregistration API */
/* Components register via a single function call that
returns a success or failure indication.

This call will be made once per component.
*/

/* prototypes */

#ifdef __cplusplus
extern "C" {
#endif

MEM_POOL_ID wmbComponentMemGet (WMB_CID_T cid);

STATUS  wmbComponentRegister (sbyte * pBackplaneName, sbyte * pComponentName,
                              WMBFUNCPTR pfDeregister, void * deregCookie,
                              WMB_CID_T * pCid);

STATUS wmbComponentDeregister (WMB_CID_T cid);

WMB_ID_T wmbComponentBackplaneGet(WMB_CID_T component);

WMB_CID_T wmbComponentLookup (sbyte * pName);

BOOL    wmbComponentLocal(WMB_CID_T component);

STATUS  wmbComponentRemote(WMB_CID_T component, OS_MSGQ_T queue);

STATUS  wmbComponentResourceLoad (WMB_CID_T component, WMB_ENTRY_T * pEntries,
                                  sbyte4 rows, sbyte * pPath);

STATUS  wmbComponentTableLoad (WMB_CID_T component, WMB_TABLE_T * pTable,
                               sbyte4 rows, sbyte * pPath);

STATUS  wmbComponentStart (sbyte4 blocksize);

void    wmbComponentStop (WMB_ID_T backplane);

void    wmbComponentShow (WMB_SESSION_T * pSession);

/* 1.1.2.1.2	Component Registration Structure Access */

void    wmbComponentListener(WMB_CID_T component);

STATUS  wmbComponentSend (WMB_CID_T component, void * pData, size_t length);

void    wmbComponentLockHeadSet(WMB_CID_T component, WMB_LOCK_HEAD_T * pHead);

void    wmbComponentPoolSet(WMB_CID_T component, WMB_POOL_T * pPool);

void    wmbComponentStateSet (WMB_CID_T component, void * pState);

#if defined __WMB_VIEW_ACCESS_MODEL__

STATUS  wmbComponentViewLoad (WMB_CID_T pComponent, WMB_VACM_VIEW_ENTRY_T * pEntry,
                              ubyte2 numEntries);

STATUS  wmbComponentViewDelete (WMB_CID_T component);

#endif /* __WMB_VIEW_ACCESS_MODEL__ */

#ifdef __cplusplus
}
#endif

/* for old generated code */

#define wmbComponentNameLookup(x, y)   wmbComponentNameGet(y)

#endif /* __INCwmbComponenth */
