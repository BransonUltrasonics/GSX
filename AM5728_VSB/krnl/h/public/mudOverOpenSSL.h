/* mudOverSSL.h - Header file facilitating MUD over (Open)SSL integration */

/* Copyright (c) 2003-2004, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,12mar12,r_w  implement CGI program support SSL.
                 Requirement: WIND00299862
01f,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01e,14may04,adb  moved MUD SSL support functions to mudLib.h
01d,12feb04,adb  moved configuration to mudLib.h
01c,23jan04,adb  introduced error BIO
01b,19jan04,adb  added to source control
01a,17dec03,adb  created
*/

#ifndef __INCmudOverSSLh
#define __INCmudOverSSLh


#include <iosLib.h>
#include "openssl/bio.h"
#include "openssl/crypto.h"
#include "openssl/x509.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/engine.h"

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define SSL_1 1 /* 
                 * Most but not all SSL functions return 1 on success.
                 * Read OpenSSL documentation and look out for exceptions.
                 * We define SSL_1 to avoid using 1 as a magic number.
                 * We do not define SSL_OK in order to raise the 
                 * OpenSSL-VxWorks integrator programmer's awareness.
                 */

/* typedefs and structure implementations */

/* MUD_SSL_STATE and MUD_SSL_STATE_ID are defined in mudLib.h */
struct mudSSLState
    {
    BIO *        pErrorBio;
    SSL_METHOD * pSSLMethod;
    };   

/* MUD_PORT_SSL_STATE and MUD_PORT_SSL_STATE_ID are defined in mudLib.h */
struct mudPortSSLState
    {
    SSL_CTX * pSSLCtx;
    };

/* MUD_CONN_SSL_STATE and MUD_CONN_SSL_STATE_ID are defined in mudLib.h */
struct mudConnSSLState
    {
    SSL *   pSSL;
    DEV_HDR *pDevHdr;
    int  SSLDevDrvNum;
    int fd;
    };

#ifdef __cplusplus
}
#endif

#endif /* __INCmudOverSSLh */
