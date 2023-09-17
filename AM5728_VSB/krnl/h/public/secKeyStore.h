/* secKeyStore.h - VxWorks Key Store library header file */

/*
 * Copyright (C) 2015 Wind River Systems, Inc.
 * 
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
21sep15,r_b  Removed obsolete comments
12Aug15,r_b  Created
*/

#ifndef __INCsecKeyStoreh
#define __INCsecKeyStoreh

/* includes */
#include <secVault.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* defines */

/* typedefs */

    struct evp_pkey_st;
    struct x509_st;

/* function declarations */
    struct evp_pkey_st* secKeyStoreGet(const char* key_id);


    struct evp_pkey_st* secKeyStoreX509Get(struct x509_st* x509,
                                           char* id,
                                           int* len);

    int secKeyStoreImport(const char* key_id,
                          const struct evp_pkey_st* key);


    int secKeyStoreDel(const char* key_id);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCsecKeyStoreh */
