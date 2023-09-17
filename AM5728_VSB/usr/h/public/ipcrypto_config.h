/*
 * Copyright (c) 2006-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
25aug14,ljl  Complete openssl layer and cleanup crypto/ssl, US44321
07may14,ljl  Support Ecliptic Curve Algorithms, US36856
*/

#ifndef IPCRYPTO_CONFIG_H
#define IPCRYPTO_CONFIG_H

/*
 ****************************************************************************
 * 1                    DESCRIPTION
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 2                    INCLUDE FILES
 ****************************************************************************
 */

#include <openssl/opensslconf.h>

#ifdef __cplusplus
extern "C" {
#endif
/*
 ****************************************************************************
 * 3                    DEFINES
 ****************************************************************************
 */

#ifdef IPCRYPTO_MINIMUM_FOOTPRINT

    /* Hash algorithms */
#define OPENSSL_NO_MD2
#define OPENSSL_NO_MD4
#define OPENSSL_NO_RIPEMD
#define OPENSSL_NO_RIPEMD160
#define OPENSSL_NO_RMD160

    /* Symmetrical algorithms */
    /*#define OPENSSL_NO_AES*/
#define OPENSSL_NO_BF
#define OPENSSL_NO_CAST
#define OPENSSL_NO_CHAIN_VERIFY
#define OPENSSL_NO_DESCBCM
#define OPENSSL_NO_RC4
#define OPENSSL_NO_RC2

    /* Asymmetrical algorithms */
/* These algorithms are needed by IPIKE, IPSSH, IPSSL, IPWEBS */
/* #define OPENSSL_NO_DH */
/* #define OPENSSL_NO_DSA */
/* ipwebs and ipsslproxy require RSA */
/* #define OPENSSL_NO_RSA */


#define OPENSSL_NO_KRB5
/* #define OPENSSL_NO_OCSP */

#endif

    /* The following algorithms have an unclear patent situation, hence we exclude them */
#define OPENSSL_NO_MDC2
#define OPENSSL_NO_IDEA


#ifdef OPENSSL_FIPS
#define OPENSSL_NO_MD2
/*#define OPENSSL_NO_MD4*/
#define OPENSSL_NO_RIPEMD
#define OPENSSL_NO_RIPEMD160
#define OPENSSL_NO_RMD160

#define OPENSSL_NO_BF
#define OPENSSL_NO_CAST
#define OPENSSL_NO_RC4
#define OPENSSL_NO_RC2
#endif


/*
 *===========================================================================
 *                         IPCRYPTO_DEFAULT_CONFIG_FILE
 *===========================================================================
 */
#define IPCRYPTO_DEFAULT_CONFIG_FILE "/ram/openssl.cnf"


/*
 *===========================================================================
 *                         IPCRYPTO_USE_APPS
 *===========================================================================
 * Define to include openssl shell command applications.
 */
#if !defined(IPCRYPTO_MINIMUM_FOOTPRINT)
#define IPCRYPTO_USE_APPS
#endif


/*
 *===========================================================================
 *                         IPCRYPTO_USE_TEST
 *===========================================================================
 * Define to include openssl shell command test tools.
#if !defined(IP_PORT_ITRON) && !defined(IPCRYPTO_MINIMUM_FOOTPRINT)
#endif
 */
#define IPCRYPTO_USE_TEST



/*
 ****************************************************************************
 * 4                    TYPES
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 5                    FUNCTIONS
 ****************************************************************************
 */


#ifdef __cplusplus
}
#endif

#endif


/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */
