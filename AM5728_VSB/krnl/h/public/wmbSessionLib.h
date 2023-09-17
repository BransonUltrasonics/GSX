/* wmbSessionLib.h */

/* Copyright 2003-2004, 2006, 2011-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,16feb12,f_f  make command "who" can get ipv6 address (WIND00329147)
01b,25jan11,m_z  command who on ssh show real IP address
                 CQID: Fix WIND00203639
01a,26jul06,ps   update WMB_SESSION_T structure to handle IPV6 connections
12oct05,wdz      '#ifdef __cplusplus' statements added
                 for SPR#98444
01c,13sep04,dkg  added clientIndex field in structure _WMB_SESSION_T to 
                 support JavaScript Authentication feature
01b,14aug03,adb  incorporated SMTP modifications
01a,04aug03,dlr  added backwards support for RCB handlers - indexing counters
*/

#ifndef __INCwmbSessionLibh
#define __INCwmbSessionLibh

/* Session Structure
The session structure is passed as an argument for all WMB transactions.  It contains information which is likely to remain constant throughout the life of a session.  The concept of a session is consumer-specific (one HTTP connection for web, one Telnet session for CLI, one transaction for SNMP, etc.).
*/

#ifdef __cplusplus
#include "wmFileLib.h"
#endif /* __cplusplus */

/* Flags for Session structue  */
#define WMB_SESSION_NO_DEF_TRANSACTION  0x00000001

typedef struct WMB_LOCAL_S
    {
    WMB_DB_T *  pDB;    /* working variables db */
    sbyte *     pData;  /* beginning of storage */
    sbyte *     pFree;  /* free space           */
    sbyte4      free;   /* amount remaining     */
    } WMB_LOCAL_T;

#ifndef SESSION_MAX_ENV
#define SESSION_MAX_ENV 12
#endif

#if defined __WMB_VIEW_ACCESS_MODEL__
typedef struct WMB_SESSION_VIEW_S
    {
    WMB_VACM_VIEW_T     *viewReadList;
    WMB_VACM_VIEW_T     *viewWriteList;
    }WMB_SESSION_VIEW_T;
#endif

/* backwards compatability for RC3x structure table Indexes */
#ifdef __ENABLE_RCB_SUPPORT__
#define kRcbMacroRepeatNestMax	4		   /* fix this value here, since the HTTP value is not available */
typedef struct RC_ENVIRONMENT_S
{
    ubyte4              MacroIndex;		   /* original un-nested index counter */
    ubyte4              Macro_NestDepth;   /* current nesting depth */
    ubyte4              Macro_Index[kRcbMacroRepeatNestMax];	/* nested index counter */
    WM_LIST_T*          p_MacroLstIndexedValues[kRcbMacroRepeatNestMax];	/* list of indexed rapidmarks*/
    int                 PostValid;
} RC_ENVIRONMENT_T;
#endif

typedef struct _WMB_SESSION_T
    {
#ifndef __NO_MAGIC__
    ubyte4              magic;          /* data integrity */
#endif
    OS_SOCKET_T         sock;
    ubyte4              id;             /* unique id of this session */

    WMNET_IP_ADDR       wmnet_IpAddr;

    WMB_COMPONENT_T *   pComponent;     /* component, e.g., CLI, WEB */
    void *              pInfo;          /* pointer to consumer specific state */
    void *              table;
    WMB_TID_T           tid;
    WM_ACCESS_T         access;         /* read/write security credentials */
#if defined __WMB_VIEW_ACCESS_MODEL__
    WMB_SESSION_VIEW_T  viewList;       /* List of views    */
#endif
    ubyte4              flags;          /* flags associated with session */
    WMB_POOL_T *        pPool;
#if 0
    sbyte *             variables[SESSION_MAX_ENV];
    Access              UserLevel;      /* TEMP!!! */
#endif
    void *              pSnmpData;      /* TEMP!!!! need different scheme */
#ifdef ENVOY_STACK_K
    void*               pIntSetSnmpPkt;
#endif
    WM_DIR_T *          pDir;           /* session directory */
    sbyte4              type;           /* session type, e.g., CLI, WEB */
    sbyte4              fileIncludeLevel; /* recursive level for fileInclude windMark */
    void *              pBladeData;     /* cookie reserved for the current consumer blade        */
#ifdef __ENABLE_CUSTOM_STRUCT__
    void *              pCustomData;    /* custom pointer to bu used by the clients bo backplane */
#endif


#ifdef __ENABLE_RCB_SUPPORT__
	RC_ENVIRONMENT_T    pRcEnvironment; /* This structure supplies support for RC3x migration */
#endif
    sbyte4                clientIndex;
    sbyte                 clientAddr[64];
    } _WMB_SESSION_T;

#ifdef __ENABLE_CUSTOM_STRUCT__
/* vector function for initializing a custom struct */
typedef STATUS (*WMB_CUSTOM_CONSTRUCT)(WMB_SESSION_T *pSession, void **ppCustomData);

/* vector function for destroying a custom struct */
typedef void (*WMB_CUSTOM_DESTRUCT) (void *pCustomObject);
#endif


#define wmbSession(x)   ((_WMB_SESSION_T *) x)

#define SESSION_CHANNEL_GET_M(x)    (NOT_NULL_M(x) ? x->pChannel : NULL)
#define SESSION_SOCK_GET_M(x)       ((x)->pChannel->sock)

typedef enum
    {
    WMB_TYPE_INVALID_K,
    WMB_TYPE_WEB_K,
    WMB_TYPE_CLI_K,
    WMB_TYPE_SNMP_K,
    WMB_TYPE_SMTP_K
    } WMB_TYPE_T;

typedef STATUS SESSION_VALID_T
    (
    WMB_SESSION_T * pSession,
    sbyte *         pName,
    sbyte *         pPassword
    );

/* session flags */

#define FLAG_SESSION_ALL_ACCESS         0x0001

/* Session Structure Access */

#define wmbSessionAccessGet(pX)         (pX)->access
#define wmbSessionComponentGet(pX)      (pX)->pComponent
#define wmbSessionReadSecGet( pX)       (pX)->pReadSec
#define wmbSessionWriteSecGet(pX)       (pX)->pWriteSec
/*
#define wmbSessionFlagsGet(   pX)       (pX)->flags
*/
#define wmbSessionStateGet(pX)          (pX)->pBladeData
#define wmbSessionTidGet(pX)            (pX)->tid
#define wmbSessionSockGet(pX)           (pX)->sock
#define wmbSessionMemGet(pX)            ((NULL == (pX)->pPool) ? NULL : (pX)->pPool->pool)
#define wmbSessionTypeGet(pX)           (pX)->type
#define wmbSessionTableGet(pX)          (pX)->table
#define wmbSessionPoolGet(pX)           (pX)->pPool
#define wmbSessionDirGet(pX)            (pX)->pDir
#define wmbSessionIdGet(pX)             (pX)->id


#define wmbSessionComponentSet(pX, pY)  (pX)->pComponent   = pY
#define wmbSessionReadSecSet( pX, pY)   (pX)->pReadSec     = pY
#define wmbSessionWriteSecSet(pX, pY)   (pX)->pWriteSec    = pY
/*
#define wmbSessionFlagsSet(   pX, pY)   (pX)->flags        = pY
*/
#define wmbSessionStateSet(   pX, pY)   (pX)->pBladeData   = pY
#define wmbSessionSockSet(    pX, pY)   (pX)->sock         = pY
#define wmbSessionMemSet(     pX, pY)   (pX)->memID        = pY
#define wmbSessionTypeSet(    pX, pY)   (pX)->type         = pY
#define wmbSessionTableSet(   pX, pY)   (pX)->table        = pY
#define wmbSessionPoolSet(    pX, pY)   (pX)->pPool        = pY
#define wmbSessionDirSet(pX, pY)        (pX)->pDir         = pY
#define wmbSessionIdSet(pX, pY)         (pX)->id           = pY


#define MAGIC_SESSION_K                 0xfacedeaf
#define wmbSessionBless(x)              wmMagicBless(x, MAGIC_SESSION_K);
#define wmbSessionBad(x)                wmMagicBad(x,  MAGIC_SESSION_K)

#ifdef __ENABLE_SESSION_DIRECTORIES__
#define SESSION_CWD_GET_M(pEnv)             LEGACY(pEnv)->cwd
#define MSESSION_DirGet(pEnv)               LEGACY(pEnv)->pDirHandle
#define MSESSION_DirSet(pEnv, x)            LEGACY(pEnv)->pDirHandle = x
#else
#define SESSION_CWD_GET_M(pEnv)             mDefaultPath
#define MSESSION_DirGet(pEnv)               NULL
#define MSESSION_DirSet(pEnv)
#endif



#ifdef __cplusplus
extern "C" {
#endif

STATUS  wmbSessionStart (void);

STATUS  wmbSessionStop (void);

STATUS  wmbSessionCreate (WMB_CID_T component, OS_SOCKET_T sock, ubyte4 flags,
                          WMB_SESSION_T ** ppSession);

STATUS  wmbSessionDestroy (WMB_SESSION_T * pSession);

WMB_ID_T wmbSessionBackplaneGet (WMB_SESSION_T * pSession);

STATUS  wmbSessionAccessInfo(WMB_SESSION_T * pSession, DTTypeInfo * pAccess);

sbyte * wmbSessionEnvironmentGet (WMB_SESSION_T * pSession, sbyte4 index);

sbyte4  wmbSessionPrintf (WMB_SESSION_T * pSession, sbyte * szFormat, ...);

STATUS  wmbSessionAccessSet (WMB_SESSION_T * pSession, sbyte * pAccessStr);

STATUS wmbSessionViewSet (WMB_SESSION_T *pSession, sbyte *userName);

void wmbSessionTidSet(WMB_SESSION_T *pSession, WMB_TID_T tid);

ubyte4 wmbSessionAddressGet (WMB_SESSION_T * pEnv);

sbyte * wmbSessionCwdGet(WMB_SESSION_T * pEnv);

#ifdef __ENABLE_CUSTOM_STRUCT__
void wmbSessionCustomVectorSetup(WMB_CUSTOM_CONSTRUCT p_funcConstructCustom, WMB_CUSTOM_DESTRUCT p_funcDestructCustom);
void *wmbSessionCustomDataPointerGet(WMB_SESSION_T*  pSession);
#endif

#ifdef __cplusplus
}
#endif


#endif /* __INCwmbSessionLibh */
