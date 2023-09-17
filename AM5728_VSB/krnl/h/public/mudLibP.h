/* mudLibP.c - Private interface to Multithreaded Universal Daemon */

/* Copyright (c) 1996-2006, 2010-2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01m,12sep13,f_f  ipv6 support improvement
01l,01nov11,r_w  HTTP header timeout support(WIND00307889)
01k,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01j,03may06,ps   add MUD_STATE_IGNORE_NEW_CONNECTIONS state.
01i,27apr04,adb  introduced variable size array of thread ids
01h,26feb04,adb  introduced MUD_STATE
01g,06feb04,adb  introduced mudShutdownSem
01f,31jan04,adb  multiport functionality necessitated changes
01e,30jan04,adb  added port listeners
01d,26jan04,adb  removed connection handlers from MUD structure
01c,22jan04,adb  conditional OpenSSL supporting code compilation
01b,21jan04,adb  added single port SSL support and prepared multiport transition
01a,16dec03,adb  reorganized 
*/

#ifndef __INCmudLibPh
#define __INCmudLibPh

#include "mudLib.h"
#include "commonCfg.h"

#ifdef INCLUDE_OPENSSL_SUPPORT
#include "mudOverOpenSSL.h"
#endif /* INCLUDE_OPENSSL_SUPPORT */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum mudState
    {
    MUD_STATE_RUNNING = 0,
    MUD_STATE_SHUTDOWN_REQUESTED,
    MUD_STATE_TASKS_DELETED,
    MUD_STATE_IGNORE_NEW_CONNECTIONS
    } MUD_STATE;

struct mudConn
    {
    OS_SOCKET_T           sock;               /* file descriptor              */
    MUD_CONN_STATE        connState; 
    MEM_POOL_ID           memPoolId;          /* connection memory pool       */
    void *                pEnv;               /* connection environment       */
    ubyte4                status;
    MUD_HANDLER           receiveHandler;     /* handle incoming data         */
    MUD_HANDLER           errorHandler;       /* handle errors                */
    MUD_CALLBACK          closeCallback;      /* report termination           */
    void *                closeClbkArg;       /* closeCallback argument       */
    MUD_CONN_SSL_STATE_ID pMudConnSSLState;   /* pointer to SSL state         */
    MUD_PORT_ID           pMudPort;           /* pointer to parent MUD port   */
#ifndef _WRS_CONFIG_LP64
    ubyte4               iHttpStartReadSecs; /* record time when http start to read line */
#else
    ubyte8               iHttpStartReadSecs; /* record time when http start to read line */
#endif    
    };

struct mudPort
    {
    ubyte                   family;             /* ip family type             */
    ubyte2                  port;               /* port to listen to          */
    MUD_PORT_ACCESS_DATA_ID pMudPortAccessData; /* NULL signifies free access */
    MUD_HANDLER             connAcceptHandler;  /* it ensues socket accept    */
    MUD_HANDLER             connReceiveHandler; /* handle conn incoming data  */
    MUD_HANDLER             connErrorHandler;   /* handle conn errors         */
    MUD_CALLBACK            connCloseCallback;  /* report conn termination    */
    void *                  connCloseClbkArg;   /* connCloseCallback argument */
    MUD_PORT_SSL_STATE_ID   pMudPortSSLState;   /* pointer to SSL state       */
    MUD_CONN                listener;           /* port listerner connection  */
    MUD_ID                  pMud;               /* pointer to parent MUD      */
    };

struct mud
    {
    char               mudName
                       [MUD_MAX_NAME_SIZE+1];/* mud name                      */

    MEM_POOL_ID        permMemPoolId;        /* permanent mem pool            */

    ubyte2             maxActivePorts;       /* maximum number of ports       */
    ubyte2             activePorts;          /* currently active ports        */
    MUD_PORT_ID *      arrayOfPorts;         /* array of ports                */
    ubyte2             portAttendIndex;      /* port round robin marker       */

    ubyte2             maxActiveConns;       /* maximum number of connections */
    MUD_CONN_ID *      arrayOfConns;         /* array of connections          */
    ubyte4             timeout;              /* socket timeout in secs        */
    ubyte2             connAttendIndex;      /* connection round robin marker */

    OS_SEM_T           mudDataSem;           /* protect array of connections  */
    OS_SEM_T           mudAttendantSem;      /* avoid concurrent select()s    */
    OS_SEM_T           mudShutdownSem;       /* synchronizes shutdown         */

    ubyte2             numOfThreads;         /* number of threads in mud      */
    ubyte4             priority;             /* mud tasks priority            */
    OS_THREAD_T *      arrayOfTids;          /* track thread ids              */

    OS_PIPE_T          pipeSignal;           /* write end of signaling pipe   */
    OS_PIPE_T          pipeSignalRcv;        /* read end of signaling pipe    */

    MUD_STATE          mudState;             /* facilitates synchronization   */

    MUD_SSL_STATE_ID   pMudSSLState;         /* pointer to SSL state          */

    /* fields below are not used and will be deprecated */

    MUD_CONN           tempConn;            /* legacy intent: send busy msg  */
    };

#ifdef __cplusplus
}
#endif

#endif /* __INCmudLibPh */
