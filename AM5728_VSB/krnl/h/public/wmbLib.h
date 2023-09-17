/* wmbLib.h - header file for RCB library */

/* Copyright 2000-2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01e,30mar10,f_f  VxWorks 64bit support (WIND00193649)
01d,07sep09,m_z  eliminate unused parameter for ValidateHandler
                 (WIN00174035)
01c,27apr04,adb  replaced all buffer size constants with kMagicMarkupBufferSize 
01b,01aug01,pns  adjusted.
01a,04mar01,kw   Written.
*/

/*
 * DESCRIPTION:
 *   wmbLib.h contains the definitions for constants, data structures,
 *   enumerations and function prototypes used to implement RCB2.
 *
 *   This header defines only external structures, defines, and enums that
 *   are required for RCB2 consumers. The producers must also include the
 *   wmbLibP.h header files for internal structures, defines, and enums
 *   statments.
 */

#ifndef __INCwmbLibh
#define __INCwmbLibh


/* message IDs */

typedef enum
    {
    WMB_MSG_INVALID_K,
    WMB_MSG_REGISTER_K,   /* register remote backplane with parent */
    WMB_MSG_SHARE_K,      /* shared memory info from parent backplane */
    WMB_MSG_RESOLVE_K,    /* resolve object in remote backplane */
    WMB_MSG_FOUND_K,      /* response from remote resolve */
    WMB_MSG_EXEC_K,       /* execute object handler in remote backplane */
    WMB_MSG_DONE_K,       /* response from remote execution */
    WMB_MSG_EVENT_K,      /* message is WMB event */
    WMB_MSG_COMMAND_K,    /* resolve and execute object remote handler */
    WMB_MSG_DEREGISTER_K, /* component is shutting down */
    WMB_MSG_SHUTDOWN_K,   /* shut down entire WindManage backplane */
    WMB_MSG_JOIN_K        /* superceded by REGISTER?????? */
    } WMB_MSG_T;



/* WindMark macros */

#ifdef  __SNMP_API_ENABLED__
#define kMaxArgsLen                 600
#else
#define kMaxArgsLen                 40
#endif

#define kMaxMagicMarkupIdLen                    (40 + kMaxArgsLen)
#define kMaxMagicMarkupSubstitutionValueLen     kMagicMarkupBufferSize

#define RM_PAIR(a, b)               ((a * 0x100) + b)

#ifdef __RL_FIXED_MM_DELIM__
#  ifdef  __BRACKET_DELIMS__
#    define kMagicMarkupStart       RM_PAIR('[', '[')
#    define kMagicMarkupEnd         RM_PAIR(']', ']')
#  else  /* default delimitators */
#    define kMagicMarkupStart       RM_PAIR('$', '%')
#    define kMagicMarkupEnd         RM_PAIR('#', '$')
#  endif
#else
#  define kMagicMarkupStart         RM_PAIR(kMagicMarkupStartChar0, kMagicMarkupStartChar1)
#  define kMagicMarkupEnd           RM_PAIR(kMagicMarkupEndChar0, kMagicMarkupEndChar1)
#  define kMagicMarkupStartLen      2
#  define kMagicMarkupEndLen        2
#endif /* __RL_FIXED_MM_DELIM__ */

#define kSizeofMagicMarkupStart     sizeof(sbyte2)
#define kSizeofMagicMarkupEnd       sizeof(sbyte2)


#ifdef __MACRO_REPEAT_NEST__
#define kMaxMagicMarkupEndLen                   (40 + 40)
#endif

typedef struct WMB_HEADER_T
    {
    ubyte4      magic;      /* for debugging only       */
    ubyte2      headerLen;  /* offset to data           */
    ubyte2      replyPort;  /* port to reply to         */
    ubyte4      dataLen;    /* excludes header itself   */
    ubyte4      flags;      /* misc flags               */
    ubyte4      version;    /* header version           */
    ubyte4      domain;     /* protection domain        */
    ubyte4      cmd;        /* enumerated command       */
    void *      pCookie;    /* based on cmd type        */
    }
    WMB_HEADER_T;

/* to send back shared mem info to sub backplane */


#define wmbHeaderBless(x)           wmMagicBless(((WMB_BACKPLANE_T *) x), MAGIC_WMB_HEADER_K)
#define wmbHeaderBad(x)             wmMagicBad(((WMB_BACKPLANE_T *) x), MAGIC_WMB_HEADER_K)

#define wmbHeaderDataGet(x)         (((char *) x) + (x)->headerLen)
#define wmbHeaderCommandGet(x)      (x)->cmd
#define wmbHeaderDomainGet(x)       (x)->domain
#define wmbHeaderCookieGet(x)       (x)->pCookie
#define wmbHeaderFromGet(x)         (((char *) x) + sizeof(*x))
#define wmbHeaderCommandSet(x,y)    (x)->cmd = y
#define wmbHeaderDomainSet(x,y)     (x)->domain = y
#define wmbHeaderCookieSet(x,y)     (x)->pCookie = y



typedef enum WMB_SHARES_K_tag
    {
    WMB_SHARE_INVALID_K,
    WMB_SHARE_OBJECT_K,
    WMB_SHARE_BUFFER_K,
    WMB_SHARE_DATA_K
    } WMB_SHARE_T;

/*
2   External API Description
2.1 Registration Processing API
The registration API actually consists of four separate APIs that
will be implemented in sub-modules.  All components register using
the Component Registration API.  After the initial registration,
data producers register their resources via the Resource Registration API,
and most producers will also register archives using the
Archive Registration API.

Event consumers register to receive events using the Event Registration API.
*/


/* utility to return cid using name lookup */
STATUS wmbCidByName
    (
    sbyte *     pBackplane,
    sbyte *     pNameOfInterest,
    WMB_CID_T * pCid
    );


/*
2.2.2   Backplane-to-Data Producer API
The backplane then calls one or more data handlers to invoke the transaction on all of the objects in the object list.  In flat memory systems (such as Tornado 2.0), these handlers will be called directly by the backplane.  In memory protected systems (such as Tornado AE), they will be called via an IPC mechanism.
 The original session structure and transaction structure are passed through unmodified.  For each call, a separate object ID list pointer is included for the group of object IDs that are handled by this particular access function.
*/

#if defined __WMB_VACM_ACCESS_MODEL__
typedef struct WMB_VIEW_ACESS_MODEL_S
    {
    sbyte *         groupName;
    void *          nextEntry;
    }WMB_VIEW_ACESS_MODEL_T;
#endif



/*
A component may also be de-registered (using the call passed in on registration) by the backplane if, for example, the backplane is shutting down.
2.1.2   Resource Registration and Deregistration API
Data consumers register their resources via the resource registration API.  The function returns a success or failure indication.  This call can be made multiple times to register resource groups.
*/
STATUS wmbResourceRegister
    (
    sbyte *         pComponent,     /* Component Name                   */
    sbyte *         pPath,          /* Path name registered under       */
    WMB_ENTRY_T *   pEntries,       /* Resource Entry Array             */
    sbyte4          entryCount,     /* Resource Entry Count             */
    WMB_TABLE_T *   pTable,         /* Table Entry Array                */
    sbyte4          tableSize,      /* Resource Entry Count             */
    WMB_HANDLER_T   handler         /* Default Handler                  */
    );

/*
At any time, a component can de-register some or all of its resources using the following API call.
Resources must be deregistered using the resource ID that was supplied on the original registration.
*/
void wmbResourceDeregister (WMB_CID_T cid, WMB_RESOURCE_ID_T *pResId);


/*
2.2.1   Data Consumer-to-Backplane API
The data consumer calls the resource access function with the session (and consumer) information and the transaction information.
*/

STATUS  wmbResourceAccess(WMB_TID_T tid);



/* 2.2.3    Data Consumer-to-Backplane Single-Resource Fast API
In the final version of RCB2 there will be a set of API calls that are
designed for fast access to individual resources without needing to
explicitly build a full transaction record, nor to handle
asynchronous returns. The supported set of "direct" API calls will
include the functions GET, SET, GET-NEXT, and VALIDATE.
These functions are only valid for synchronously-accessible resources,
and the calls themselves are synchronous.
*/


typedef STATUS ValidateHandler(sbyte *pData, struct DTTypeInfo *);


STATUS wmbValidate
    (
    WMB_SESSION_T * pEnv,
    sbyte *         pString,
    DTTypeInfo *    pTypeInfo,
    BOOL            errorMsg
    );

sbyte4 wmbShareString (MEM_POOL_ID pool, char * pString);

STATUS wmbStart
    (
    sbyte *         pName,              /* backplane name                   */
    sbyte4          defaultComponents,  /* number of components per block   */
    sbyte4          dbSize,             /* default database size            */
    sbyte4          tableSize,          /* default table size               */
    WMBFUNCPTR      dereg,              /* system deregistration handler    */
    void *          pCookie,            /* system deregistration state      */
    DTTypeInfo *    pAccessInfo         /* access group info                */
    );

STATUS  wmbStop (sbyte * pName);

#define wmbStartup  wmbStart
#define wmbShutdown wmbStop

MEM_POOL_ID wmbSharedPool (void);

void  wmbDump(void);

void wmbProbe
    (
    WMB_SESSION_T * pSession,
    sbyte *         pAddress,
    ubyte2          port,
    sbyte *         cmd,
    sbyte *         pData
    );

#ifndef WMB_MAX_CMD_NAMES_K
#define WMB_MAX_CMD_NAMES_K 32
#endif

typedef struct WMB_CMD_PAIR_T
    {
    COMMAND_T   cmd;
    sbyte       name[32];
    } WMB_CMD_PAIR_T;


/* wmbLib.h */


/* other wmbLib.h - main header file for RCB2 */

#define WMB_NAME_LEN    32

typedef int WMB_RID_T;
typedef int WMB_DOMAIN_T;

typedef struct WMB_S
    {
    sbyte           name[WMB_NAME_LEN];
    WMB_RID_T       id;
    WMB_DOMAIN_T    domain;
    } WMB_T;


#ifdef __cplusplus
extern "C" {
#endif

    /* TODO: Need to fully define all structures and typedefs. */

/******************************************************************************
* RCB2 Resource component creation and destruction routines.
*/

/*
 * Notes on wmbInit()
 *    The pRcbName could be in the wmb_t structure.
 *    The wmb_t structure needs to be defined or whatever it is called.
 *    May want to pass back an ID value/pointer for the terminate call.
 */

/*
IMPORT STATUS   wmbInit( char * pRcbName, WMB_T ** ppRcb);
*/

IMPORT STATUS   wmbLegacyInit(void);

/*
 * We did not talk about the wmbterminate() function but this needs to be
 * present.
 */
IMPORT STATUS   wmbTerminate( WMB_T * pRcb);

/*
 * TODO:
 *   Need to the other functions Session, component, Event, and producer calls.
 */

/*
 * One reason for these macros and routines is that you can not mix the
 * type of access in any one transaction. These macros also make the code
 * easier to read, because the action is not hidden in the structures,
 * but passed as an argument to wmbResourceAccess().
 *
 * Note: The macros below could be real functions, if these macros need to
 *       be larger then one or two lines.
 *
 *       Another good reason for functions is you can put a break point on
 *       the first wmbResourceSet() and if they were macros then only
 *       wmbResouceAccess() could have a breakpoint and all non-set calls
 *       would hit the breakpoint.
 *
 *       For debugging making them functions is the best method. On most
 *       RISC based CPUs the added function call overhead is not very
 *       large, now CISC CPUs it does have more of a cost.
 *
 *       For the IDB+ like calls I would like to have the real function calls.
 */

sbyte4 wmbHtmlWriter(WMB_SESSION_T * pSession, sbyte * pData, sbyte4 len);



/******************************************************************************
* Standard RCB2 Helper Routines for single object requests in the IDB+ style.
*
* These routines will convert the simple arguments to RCB2 structures and
* call the correct RCB2 routines. The goal here is to define routines that
* are very simple to convert from IDB+ consumer calls to RCB2 calls.
*
* For single object requests RCB2 will emulate the IDB+ routines to allow
* easy conversion of IDB+ consumers.
*
* Note: These routines are very close to the IDB+ routines, just has the
*       pSession cookie added to the request. Also the arguments do not
*       require including the wmbLib.h header for any internal structure
*       typedefs from RCB2.
*
* TODO: Could the Session pointer be hidden to allow the these routines
*       to look just like IDB+ routines. Maybe use the varTask functions to
*       hide the pSession pointer for these calls.
*
* The pSessionId value is a global taskVar variable to this task.
*/

IMPORT STATUS wmbSessionThreadSet(WMB_SESSION_T * pSession );
IMPORT STATUS wmbSessionThreadGet(WMB_SESSION_T ** ppSession );
IMPORT STATUS wmbSessionThreadDelete( void );

/* misc stuff */

void wmbUdpListener(void * pDummy);

void wmbPipeListener(WMB_ID_T backplane);

BOOL wmbDataExists (WMB_SESSION_T * pSession, sbyte * pName);

/******************************************************************************
* Wrapper Functions for Error Reporting and Handling
*
* Note: These could be macros, but it would mean that everyone file that
*       included this header would also have to include a few other files
*       to resolve the ERRNO defines.
*/
/* Miscellaneous RCB2 errors */
IMPORT STATUS  wmbErrorIndexRange( void );
IMPORT STATUS  wmbErrorInvalidRange( void );
IMPORT STATUS  wmbErrorInvalidLength( void );
IMPORT STATUS  wmbErrorNullBuffer( void );
IMPORT STATUS  wmbErrorAltObject( void );
IMPORT STATUS  wmbErrorObjectHidden( void );
IMPORT STATUS  wmbErrorNoNextInstance( void );

/* This routine based on the action Get,Set,... will set the correct errno. */
/*
IMPORT STATUS  wmbErrorRequestMap( wmbAction_t action );
*/

/* SNMPv1 specific errors */
/* not used internally in TMS 2.0 */               /*  2 - noSuchName */
/* not used internally in TMS 2.0 */               /*  3 - badValue */
/* not used internally in TMS 2.0 */               /*  4 - readOnly */

/* SNMPv2/v3 errors */
IMPORT STATUS  wmbNoError( void );                 /*  0 - noError */
IMPORT STATUS  wmbErrorTooBig( void );             /*  1 - errorTooBig */
IMPORT STATUS  wmbErrorGenError( void );           /*  5 - genError */
IMPORT STATUS  wmbErrorNoAccess( void );           /*  6 - noAccess */
IMPORT STATUS  wmbErrorWrongType( void );          /*  7 - wrongType */
IMPORT STATUS  wmbErrorWrongLength( void );        /*  8 - wrongLength */
IMPORT STATUS  wmbErrorWrongEncoding( void );      /*  9 - wrongEncoding */
IMPORT STATUS  wmbErrorWrongValue( void );         /* 10 - wrongValue */
IMPORT STATUS  wmbErrorNoCreation( void );         /* 11 - noCreation */
IMPORT STATUS  wmbErrorInconsistValue( void );     /* 12 - inconsistValue */
IMPORT STATUS  wmbErrorResourceUnavail( void );    /* 13 - resourceUnavailable*/
IMPORT STATUS  wmbErrorCommitBaded( void );       /* 14 - commitBaded */
IMPORT STATUS  wmbErrorUndoBaded( void );         /* 15 - undoBaded */
IMPORT STATUS  wmbErrorAuthorizationError( void ); /* 16 - authorizationError */
IMPORT STATUS  wmbErrorNotWritable( void );        /* 17 - notWritable */
IMPORT STATUS  wmbErrorInconsistName( void );      /* 18 - inconsistName */

/* SNMPv2/v3 exceptions */
IMPORT STATUS  wmbErrorNoSuchObject( void );       /* 0 - noSuchObject */
IMPORT STATUS  wmbErrorNoSuchInstance( void );     /* 1 - noSuchInstance */
IMPORT STATUS  wmbErrorEndOfMibView( void );       /* 2 - endOfMibView */

IMPORT STATUS  wmbExec (VOIDFUNCPTR pHandlerFcn, sbyte * pTaskName,
                        void * pArg, sbyte4 priority,
                        size_t stackSize, OS_THREAD_T * pTid);

typedef sbyte4 wmbWriter(WMB_SESSION_T * pfWriter, sbyte * pData, sbyte4 len);

STATUS wmbTextEngine(WMB_SESSION_T * pSession,
                     sbyte *        pSrc,
                     sbyte4         buffLen,
                     wmbWriter *    pfWriter,
                     sbyte4 *       pWritten);

STATUS wmbHeaderCreate (sbyte * pFrom, ubyte4 cmd, sbyte * pString,
                        ubyte4 size, WMB_HEADER_T ** ppHeader);

STATUS wmbShareCreate
    (
    WMB_ID_T    backplane,
    sbyte4      type,
    void **     ppData
    );

STATUS wmbShareFree
    (
    WMB_ID_T    backplane,
    sbyte4      type,
    void *      pData
    );

STATUS  wmbRequest (WMB_CID_T componet, sbyte * pBuffer, sbyte4 bufSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCwmbLibh */
