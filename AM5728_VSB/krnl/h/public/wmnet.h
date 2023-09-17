/* wmnet.h - WindManage Network Abstraction Layer header file */

/* Copyright (c) 2004-2007, 2010-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
25may16,r_w support IPv6 for SMTP on wmnetTcpConnect (US78868).
26aug13,r_w add routine wmnetSocketGroupWaitRdWrEt for defect WIND00430497
12mar12,r_w implement CGI program support SSL.
                Requirement: WIND00299862
07jan11,m_z correct compiling error on linux
                CQID: Fix WIND00249657
29jan10,m_z VxWorks 64-bit support
                (WIND00193649)
07mar07,ps  fixes for RTP builds. defect WIND00044864
16feb06,ps  changes for webcli on linux
20feb04,jws added copyright and mod history
*/

/*
DESCRIPTION
This header file includes the prototypes and type definitions required to
abstract network functionality from different IP stacks.
*/


#ifndef __INCwmneth
#define __INCwmneth

/* Defaults */

#define kFIXED_PORT                     kHTTP_FIXED_PORT

#ifndef kMaxUdpPacketSize
#define kMaxUdpPacketSize 512
#endif

/* typedefs and constants common to all ports  (they may be redefined for
   a specific  port if needed) */

#ifndef ANY_PORT_K
#define ANY_PORT_K      0
#endif

#if defined (__CUSTOMER_SPECIFIC_OS__)

typedef int                 OS_SOCKET_T;
typedef struct fd_set       OS_FDSET_T;

/*END __CUSTOMER_SPECIFIC_OS__*/

#elif defined (__BSD_OS__)

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

typedef int                 OS_SOCKET_T;

typedef struct fd_set_tag
    {
    fd_set fdSet;
    int    fdMax;
    } OS_FDSET_T;

#elif defined (__POSIX_OS__)

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

typedef int                 OS_SOCKET_T;
/*
typedef fd_set              OS_FDSET_T;
*/
typedef struct fd_set_tag
    {
    fd_set fdSet;
    int    fdMax;
    } OS_FDSET_T;

typedef struct wmnetIpAddr
    {
    ubyte       ipAddrBytes[16];
    ubyte       family;
    ubyte       len;
    } WMNET_IP_ADDR;


#elif defined (__SOLARIS_OS__)

#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdarg.h>
#include <unistd.h>
/*
#include <sys/types.h>
*/

typedef int                 OS_SOCKET_T;
/*
typedef fd_set              OS_FDSET_T;
*/
typedef struct fd_set_tag
    {
    fd_set fdSet;
    int    fdMax;
    } OS_FDSET_T;

#elif defined (__VXWORKS_OS__) || defined (__VXWORKS_AE_OS__)

#include <selectLib.h>
#include <sockLib.h>
#include <netinet/in.h>
#include <hostLib.h>
#include <stdarg.h>

typedef int                 OS_SOCKET_T;
typedef struct fd_set_tag
    {
    fd_set fdSet;
    int    fdMax;
    } OS_FDSET_T;

typedef struct wmnetIpAddr
    {
    ubyte       ipAddrBytes[16];
    ubyte       family;
    ubyte       len;
    } __attribute__((packed)) WMNET_IP_ADDR;

/*END __VXWORKS_OS__*/

#elif defined (__WIN32_OS__)

typedef SOCKET                              OS_SOCKET_T;
typedef struct fd_set_tag
    {
    OS_SOCKET_T sock[MAXIMUM_WAIT_OBJECTS];
    OS_PIPE_T   pipe[MAXIMUM_WAIT_OBJECTS];
    HANDLE      events[MAXIMUM_WAIT_OBJECTS]; /* WIN32 events */
    int         fdMax;
    int         active;
    } OS_FDSET_T;

/* END __WIN32_OS__ */
#endif

#include "commonCfg.h"
#ifdef INCLUDE_OPENSSL_SUPPORT
#include "memPoolLib.h"
#include "mudLib.h"
#include "mudOverOpenSSL.h"
#endif /* INCLUDE_OPENSSL_SUPPORT */

/*------------------ Prototypes ------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

STATUS wmnetInitialize        ( void );
STATUS wmnetUdpSocketCreate   (OS_SOCKET_T * pSock);
STATUS wmnetUdpBind           (OS_SOCKET_T * pSock, ubyte2 port);
STATUS wmnetUdpRcvFrom        (OS_SOCKET_T sock, ubyte4 * pAddr, ubyte2 port,
                               sbyte * pBuffer, size_t bufSize,
                               ssize_t * pBytesRead);
STATUS wmnetUdpSendTo         (OS_SOCKET_T sock, ubyte4 addr, ubyte2 port,
                               sbyte * pBuffer, size_t bufSize);
STATUS wmnetUdpIpSocketCreate
    (
    OS_SOCKET_T *   pSock,
    ubyte           family
    );

STATUS wmnetUdpIpBind
    (
    OS_SOCKET_T *   pSock,
    ubyte2          port,
    ubyte           family
    );

STATUS wmnetUdpIpRcvFrom
    (
    OS_SOCKET_T     sock,
    WMNET_IP_ADDR * pAddr,
    ubyte2          port,
    sbyte *         pBuffer,
    size_t          bufSize,
    ssize_t *        pBytesRead
    );

STATUS wmnetUdpIpSendTo
    (
    OS_SOCKET_T     sock,
    WMNET_IP_ADDR * pAddr,
    ubyte2          port,
    sbyte *         pBuffer,
    ubyte4          bufSize
    );

STATUS wmnetAddressResolve    (char * pName, ubyte4 * pAddress);
STATUS wmnetClientAddrGet     (OS_SOCKET_T clientSock, ubyte4 * pAddr);

STATUS wmnetIpAddrResolve
    (
    char *          pName,
    WMNET_IP_ADDR * pWmnetIpAddr
    );

STATUS wmnetClientIpAddrGet
    (
    OS_SOCKET_T     clientSock,
    WMNET_IP_ADDR * pWmnetIpAddr
    );

STATUS wmnetTcpConnect        (OS_SOCKET_T *pSock, sbyte * pName, ubyte2 port);

STATUS wmnetTcpServerOpen     (OS_SOCKET_T * pSock, ubyte2 port);
STATUS wmnetTcpOpen           (OS_SOCKET_T * pSock, ubyte2 port, ubyte family);
STATUS wmnetTcpLoopbackServerOpen ( OS_SOCKET_T *pSoc, ubyte2 port);
STATUS wmnetTcpAccept         (OS_SOCKET_T * pSock, OS_SOCKET_T * pAccept,
                               ubyte2 port);
STATUS wmnetTcpAcceptConn     (OS_SOCKET_T * pSock, OS_SOCKET_T * pAccept,
                               ubyte2 port, ubyte family);
STATUS wmnetTcpRead           (OS_SOCKET_T sock, sbyte * pBuf, size_t bufSize,
                               ssize_t * pBytesRead);
STATUS wmnetTcpPeek           (OS_SOCKET_T sock, sbyte * pBuf, size_t bufSize,
                               ssize_t * pBytesRead);
STATUS wmnetTcpWrite          (OS_SOCKET_T sock, sbyte * pBuf, size_t bufSize);
STATUS wmnetSocketBlocking    (OS_SOCKET_T sock, BOOL block);
STATUS wmnetSocketGroupWait   (OS_FDSET_T * setOfSocks,
                               ubyte4 timeoutInSecs);
STATUS wmnetSocketGroupWaitRdWrEt(OS_FDSET_T * setOfSocks,
                               ubyte4 timeoutInSecs,
                               sbyte4 flags);
STATUS wmnetSocketGroupAddTo  (OS_SOCKET_T sock, OS_FDSET_T * setOfSocks);
STATUS wmnetSocketGroupAddPipe(OS_PIPE_T pipe, OS_FDSET_T * pSetOfSockets);
STATUS wmnetSocketGroupClear  (OS_FDSET_T * setOfSocks);
sbyte4 wmnetSocketPrintf      (OS_SOCKET_T sock, sbyte * szFormat, ...);
STATUS wmnetSocketVprintf     (OS_SOCKET_T sock, sbyte * szFormat, va_list args,
                                sbyte4 * pBytesPrinted);
BOOL   wmnetSocketWantsAction (OS_SOCKET_T sock, OS_FDSET_T * setOfSocks);
BOOL   wmnetPipeWantsAction   (OS_PIPE_T pipe, OS_FDSET_T * pSetOfSockets);
STATUS wmnetStdoutToSocket    (OS_SOCKET_T sock, sbyte4 * pOldStdout);
STATUS wmnetStdoutRestore     (sbyte4 oldStdout);
STATUS wmnetStdinToSocket     (OS_SOCKET_T sock, sbyte4 * pOldStdin);
STATUS wmnetStdinRestore      (sbyte4 oldStdin);

#ifdef INCLUDE_OPENSSL_SUPPORT
STATUS wmnetStdoutToSSL       (MUD_CONN_SSL_STATE_ID pMudConnSSLState,  sbyte4 * pOldStdout);
STATUS wmnetStdinToSSL        (MUD_CONN_SSL_STATE_ID pMudConnSSLState,  sbyte4 * pOldStdin);
STATUS wmnetStdoutRestoreSSL  (MUD_CONN_SSL_STATE_ID pMudConnSSLState,  sbyte4 oldStdout);
STATUS wmnetStdinRestoreSSL   (MUD_CONN_SSL_STATE_ID pMudConnSSLState,  sbyte4 oldStdin);
#endif /* INCLUDE_OPENSSL_SUPPORT */

void   wmnetHexdump           (sbyte * pMsg, sbyte * pBuf, Counter BufLen);

STATUS wmnetSocketClose       (OS_SOCKET_T sock);

#ifdef __cplusplus
}
#endif


/* generic snmp interface constants */

#define kMaxMibLength           64
#define kMaxExpResultLen        255
#define kMaxQueryStringLen      128
#define kMaxSnmpInstance        128
#define kMaxStringInteger       10
#define kMaxCommString          128

#define kMibwayMaxHostNameLength 32

typedef struct
    {
    void*   p_lstOidTrackers;
    void*   pEnv;
    sbyte   CommunityString[kMaxCommString];
    sbyte   CommunityStringSet[kMaxCommString];
    ubyte4  flags;
    void*   pSetList;
    void*   pIntSetSnmpPkt;

#ifdef __SNMP_API_ENABLED__
#if (defined (__GENERIC_SNMP_V1__) || defined (__CMU_SNMP_V1__))
	void*   pCmuSessions;

	char    defaultHost[kMibwayMaxHostNameLength];
	int     snmpPort;
#endif /* #if (defined (__GENERIC_SNMP_V1__) || defined (__CMU_SNMP_V1__)) */
#endif /* __SNMP_API_ENABLED__ */

    } MIB_ENV_T;    /* interface to a SNMP */


#endif /*__INCwmneth*/
