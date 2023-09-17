/*  mudLib.h   Header file for mudLib.c   */

/* Copyright 1996-2010, 2013 Wind River Systems, Inc. */

/*
modification history
--------------------
01q,12sep13,f_f  ipv6 support improvement
01p,28jul10,m_z  eliminate compile warning
01o,06jul10,h_y  modify for defect WIND00184097
01n,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
01m,07mar07,ps  fixes for RTP builds. defect WIND00044864
01l,14sep04,dkg  removed ifdef MUD_DEBUG for mudConnSocketGet() to support 
                 JavaScript Authentication feature
01k,14may04,adb  added mudSSLSupportInitialize and mudSSLSupportTerminate
01j,27apr04,adb  defined MUD_MIN_THREADS
01i,05mar04,adb  introduced mudConnSSLVerify
01h,26feb04,adb  introduced mudTasksDelete and mudInfrastructureDelete
01g,12feb04,adb  introduced CONN_STATE_NEW
01f,09feb04,adb  introduced mudLibInit and mudLibShutdown
01e,31jan04,adb  multiport functionality necessitated changes
01d,26jan04,adb  enhanced MUD_CONN_STATUS
01c,22jan04,jws  eliminate compiler warning from line 41
01b,21jan04,adb  added single port SSL support and prepared multiport transition
01a,16dec03,adb  reorganized and added MUD I/O functions
*/

#ifndef __INCmudLibh
#define __INCmudLibh

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define MUD_MAX_NAME_SIZE        7 /* 0xffff in decimal, 65535, fits in it */
#define MUD_MAX_THREADS        256
#define MUD_MIN_THREADS          3
#define MUD_THREAD_PRIORITY    200
#define MUD_THREAD_STACK_SIZE 3000

/* SSL defines */
#if 0
#define MUD_SSL_CERTIFICATE_FILE "/romfs/htdocs/server.pem" 
                                 /* certificate (chain) and private key file */
#define MUD_SSL_PRIVATE_KEY_FILE "/romfs/htdocs/server.pem" 
                                 /* both certificate (chain) and key file    */
#define MUD_SSL_CIPHER_LIST      "DEFAULT"
                                 /* or "RC4-SHA:DES-CBC3-SHA:DES-CBC-SHA"    */

/* we do not use SSL_CTX_load_verify_locations */
#define MUD_SSL_CA_FILE          ""
#define MUD_SSL_CA_PATH          ""
#endif

/* The possible states of a MUD connection */
typedef enum mudConnState
    {
    CONN_STATE_INVALID = -1, /* it is expected that it will never occur       */
    CONN_STATE_CLOSED = 0,   /* the connection is not in use                  */
    CONN_STATE_OPEN,         /* 
                              * permanent state of a port listener connection
                              * and initial state of a data connection 
                              */
    CONN_STATE_NEW,          /* 
                              * a data connection is locked by a handler 
                              * directly after its initial reception 
                              */
    CONN_STATE_BUSY,         /* a data connection is locked by a handler      */
    CONN_STATE_PERSISTS      /* 
                              * an established persistent data connection 
                              * is being monitored by the MUD for activity
                              */
    } MUD_CONN_STATE;



typedef enum mud_conn_status
    {
    MUD_CONN_INTERNAL_ERROR = -2,          /* the notifying thread should die */
    MUD_CONN_INVALID = -1,
    MUD_CONN_OK = 0,
    MUD_CONN_CLOSED_BY_CLIENT,
    MUD_CONN_INVALID_REQUEST,
    MUD_CONN_INVALID_ACTION_REQUEST,
    MUD_CONN_SOCKET_ERROR,
    MUD_CONN_SOCKET_TIMEOUT,
    MUD_CONN_SYSTEM_LIMITS_EXCEEDED,
    MUD_CONN_OUT_OF_BOUNDS,
    MUD_CONN_REQUESTS_PERMISSION_TO_CLOSE, 
    MUD_CONN_INACCESSIBLE,
    MUD_CONN_FAILED_TO_SET_HANDLERS
    } MUD_CONN_STATUS;

/*
 * the connection handler returns one of the following values
 * to suggest an action on the connection he has accessed
 */
typedef enum mudConnAction
    {
    CONN_CLOSE = -1,     /* 
                          * reminiscent of the VxWorks convention (-1 == ERROR)
                          *
                          * close the connection 
                          */
    CONN_KEEP = 0        /*
                          * reminiscent of the VxWorks convention (0 == OK)
                          *
                          * keep connection open; this action should not occur 
                          * if the NON_PERSISTENT connection option is selected
                          */

                         
    } MUD_CONN_ACTION;

typedef OS_SOCKET_T MUD_SOCKET;

typedef WMNET_IP_ADDR MUD_IP_ADDR;

typedef struct mud MUD;
typedef MUD * MUD_ID;

typedef struct mudPort MUD_PORT;
typedef MUD_PORT * MUD_PORT_ID;

typedef struct mudConn MUD_CONN;
typedef MUD_CONN * MUD_CONN_ID;

/* Prototype of MUD_HANDLER */

typedef MUD_CONN_ACTION (*MUD_HANDLER) (MUD_CONN_ID pConn);

/* Prototype of MUD_CALLBACK */

typedef void (*MUD_CALLBACK) (void * clbkArg);

/* SSL abstract type definitions */

typedef struct sslConfiguration
    {
    const char *    pCertificateFile;
    const char *    pPrivateKeyFile;
    const char *    pCipherList;
    const char *    pCAFile;
    const char *    pCAPath;
    } SSL_CONFIGURATION;

typedef SSL_CONFIGURATION * SSL_CONFIGURATION_ID;

typedef struct mudSSLState MUD_SSL_STATE;
typedef MUD_SSL_STATE * MUD_SSL_STATE_ID;

typedef struct mudPortSSLState MUD_PORT_SSL_STATE;
typedef MUD_PORT_SSL_STATE * MUD_PORT_SSL_STATE_ID;

typedef struct mudConnSSLState MUD_CONN_SSL_STATE;
typedef MUD_CONN_SSL_STATE * MUD_CONN_SSL_STATE_ID;

/* Exported function prototypes */

/* Scope: mudLib */

STATUS mudLibInit (void);

STATUS mudLibShutdown (void);

/* Scope: MUD */

STATUS mudInfrastructureCreate
    (
    char *              pMudName,           /* unique MUD name               */
    void *              pGlobalAccessData,  /* to be determined              */
    ubyte2              maxActivePorts,     /* maximum number of ports       */
    ubyte2              maxActiveConns,     /* maximum number of connections */
    ubyte4              timeOutInSecs,      /* socket timeout (0 = infinity) */
    ubyte2              numOfThreads,       /* size of thread pool           */
    ubyte2              priority,           /* use 0 for default             */
#ifndef _WRS_CONFIG_LP64
    ubyte4              stackSize,          /* use 0 for default             */
    ubyte4              sizeOfPermMemPool,  /* use 0 to use own mem          */
    ubyte4              sizeOfTempMemPool,  /* use 0 to use own mem          */
#else
    ubyte8              stackSize,          /* use 0 for default             */
    ubyte8              sizeOfPermMemPool,  /* use 0 to use own mem          */
    ubyte8              sizeOfTempMemPool,  /* use 0 to use own mem          */
#endif
    MUD_ID *            ppNewMud            /* return value: p to new MUD    */
    );
#if 0
STATUS mudCreate /* deprecated */
    (
    ubyte4              acceptIP,           /* IP Adress to accept, (0 = ANY) */
    ubyte2              port,               /* port to listen to              */
    ubyte2              maxActiveConns,     /* maximum number of connections  */
    ubyte4              timeOutInSecs,      /* socket timeout (0 = infinity)  */
    ubyte2              numOfThreads,       /* size of thread pool            */
    ubyte2              priority,           /* use 0 for default              */
#ifndef _WRS_CONFIG_LP64
    ubyte4              stackSize,          /* use 0 for default              */
    ubyte4              sizeOfPermMemPool,  /* use 0 to use own memory        */
    ubyte4              sizeOfTempMemPool,  /* use 0 to use own memory        */
#else
    ubyte8              stackSize,          /* use 0 for default              */
    ubyte8              sizeOfPermMemPool,  /* use 0 to use own memory        */
    ubyte8              sizeOfTempMemPool,  /* use 0 to use own memory        */
#endif
    MUD_HANDLER         connHandler,        /* handle connection data         */
    MUD_HANDLER         errorHandler,       /* handle connection errors       */
    MUD_ID *            ppNewMud            /* return value: p to new mud     */
    );
#endif

STATUS mudAddConnInitHandler /* deprecated */
    (
    MUD_ID      pMud, 
    MUD_HANDLER initHandler
    );

STATUS mudStart 
    (
    MUD_ID pIdleMud
    );

STATUS mudStop 
    (
    MUD_ID pRunningMud
    );

STATUS mudTasksDelete
    (
    MUD_ID pMud
    );

STATUS mudInfrastructureDelete
    (
    MUD_ID pMud
    );

STATUS mudDestroy 
    (
    MUD_ID pCondemnedMud
    );

STATUS mudPermMemPoolIdGet 
    (
    MUD_ID        pMud, 
    MEM_POOL_ID * ppMemPool /* exit/shutdown reference holders before owner */
    );

/* Port Access Data */

typedef struct mudPortAccessData MUD_PORT_ACCESS_DATA;
typedef MUD_PORT_ACCESS_DATA * MUD_PORT_ACCESS_DATA_ID;

/* Scope: MUD Port */

STATUS mudPortAdd
    (
    MUD_ID                  pMud,              /* execution context           */
    ubyte2                  port,              /* TCP port number             */
    MUD_PORT_ACCESS_DATA_ID pMudPortAccessData,/* NULL signifies free access  */
    SSL_CONFIGURATION_ID    pSSLConfiguration, /* NULL signifies SSL agnostic */
    MUD_HANDLER             connAcceptHandler, /* Handles initialization      */
    MUD_HANDLER             connReceiveHandler,/* Handles incoming data       */
    MUD_HANDLER             connErrorHandler,  /* Handles errors              */
    MUD_CALLBACK            connCloseCallback, /* Is called at closing        */
    void *                  connCloseClbkArg   /* connCloseCallback argument  */
    );

#ifdef __WM_INET6__
STATUS mudPortAdd6
    (
    MUD_ID                  pMud,              /* execution context           */
    ubyte2                  port,              /* TCP port number             */
    MUD_PORT_ACCESS_DATA_ID pMudPortAccessData,/* NULL signifies free access  */
    SSL_CONFIGURATION_ID    pSSLConfiguration, /* NULL signifies SSL agnostic */
    MUD_HANDLER             connAcceptHandler, /* Handles initialization      */
    MUD_HANDLER             connReceiveHandler,/* Handles incoming data       */
    MUD_HANDLER             connErrorHandler,  /* Handles errors              */
    MUD_CALLBACK            connCloseCallback, /* Is called at closing        */
    void *                  connCloseClbkArg  /* connCloseCallback argument  */
    );
#endif


STATUS mudPortRemove
    (
    MUD_ID  pMud,   /* execution context */   
    ubyte2  port    /* TCP port number */
    );

/* Scope: MUD connection */

/* thread-safe direct connection access APIs */

STATUS mudConnTempMemPoolIdGet /* WARNING! shutdown users before closing */
    (
    MUD_CONN_ID     pConn, 
    MEM_POOL_ID *   ppMemPool
    );

STATUS mudConnStateGet 
    (
    MUD_CONN_ID         pConn, 
    MUD_CONN_STATE *    pMudConnState
    );

STATUS mudConnStatusGet 
    (
    MUD_CONN_ID         pConn, 
    MUD_CONN_STATUS *   pMudConnStatus
    );

/* direct connection access trusted public APIs that are not thread-safe */

STATUS mudConnPortGet (MUD_CONN_ID pConn, ubyte2 * pPort);
STATUS mudConnEnvSet (MUD_CONN_ID pConn, void * pEnv);
STATUS mudConnEnvGet (MUD_CONN_ID pConn, void ** ppEnv);
STATUS mudConnCookiePass (MUD_CONN_ID, ubyte4 cookie);
STATUS mudConnCookieRetrieve (MUD_CONN_ID, ubyte4 * pCookie);

BOOL mudConnSSLVerify (MUD_CONN_ID pConn);

/* a handler cannot close a connection; it can only return CONN_CLOSE */

MUD_CONN_STATUS mudConnectionHandlersSet
    (
    MUD_CONN_ID     pConn,              /* MUD connection */
    MUD_HANDLER     connReceiveHandler, /* 
                                         * Handles incoming data.
                                         * NULL signifies no change.
                                         */
    MUD_HANDLER     connErrorHandler,   /* 
                                         * Handles errors.
                                         * NULL signifies no change. 
                                         */
    MUD_CALLBACK    connCloseCallback,  /* 
                                         * Is called at closing.
                                         * It may be set to NULL.
                                         */
    void *          connCloseClbkArg    /* connCloseCallback argument */

    /* The connAcceptHandler is configurable only at the MUD port level. */
    );

/* MUD I/O functions */

STATUS mudWrite
    (
    MUD_CONN_ID pConn,
    sbyte *      pBuf,
#ifndef _WRS_CONFIG_LP64
    ubyte4      bufSize
#else
    ubyte8      bufSize
#endif
    );

STATUS mudRead
    (
    MUD_CONN_ID pConn,
    sbyte *      pBuf,
#ifndef _WRS_CONFIG_LP64
    ubyte4      bufSize,
    ubyte4 *    pBytesRead
#else
    ubyte8      bufSize,
    ubyte8 *    pBytesRead
#endif
    );

STATUS mudPeek
    (
    MUD_CONN_ID pConn,
    sbyte *      pBuf,
#ifndef _WRS_CONFIG_LP64
    ubyte4      bufSize,
    ubyte4 *    pBytesRead
#else
    ubyte8      bufSize,
    ubyte8 *    pBytesRead
#endif
    );

STATUS mudSocketVprintf
    (
    MUD_CONN_ID pConn,
    sbyte *      format,
    va_list     args,
    sbyte4 *    pBytesPrinted
    );

STATUS mudClientIpAddrGet
    (
    MUD_CONN_ID     pClientConn,
    MUD_IP_ADDR *   pMudIpAddr
    );

STATUS mudStdoutToSocket
    (
    MUD_CONN_ID pConn,
    sbyte4 *    pOldStdout
    );

STATUS mudStdoutRestore
    (
    MUD_CONN_ID pConn,
    sbyte4      oldStdout
    );

STATUS mudStdinToSocket
    (
    MUD_CONN_ID pConn,
    sbyte4 *    pOldStdin
    );

STATUS mudStdinRestore
    (
    MUD_CONN_ID pConn,
    sbyte4      oldStdin
    );

#ifdef MUD_DEBUG
/* debugging functions */
STATUS mudShowConns (MUD_ID mud, MUD_SOCKET outSocket);
#endif /* MUD_DEBUG */

/* mud internal functions that may also be externally invoked for debugging */
STATUS  mudConnSocketGet (MUD_CONN_ID pConn, MUD_SOCKET * pSocket);
 
#if 0
#ifdef MUD_CONNECTION_VERIFICATION_IS_REQUIRED
/* guarded application-MUD interface definitions */

typedef ubyte4 MUD_CONN_HANDLE;

typedef MUD_CONN_ACTION (*MUD_GUARDED_HANDLER) (MUD_CONN_HANDLE handle);
                                                /* extra level of indirection */

STATUS mudConnVerify (MUD_CONN_HANDLE handle);
      /*
       * implementation depended verification
       * makes it unlikely to use a stale handle
       * or a handle owned by another application
       */

STATUS mudConnAccess (MUD_CONN_HANDLE handle, MUD_CONN_ID ppConn);
       /* obtain corresponding MUD_CONN_ID pointer */

#endif /* MUD_CONNECTION_VERIFICATION_IS_REQUIRED */
#endif
/* SSL Support functions */

STATUS mudSSLSupportInitialize 
    (
    MUD_ID pMud
    );

STATUS mudSSLSupportTerminate 
    (
    MUD_ID pMud
    );

/* 
 * Calling mudSSLSupportTerminate will result in removing
 * all MUD ports that offer connection over SSL. 
 */

#ifdef __cplusplus
}
#endif

#endif /* __INCmudLibh */
