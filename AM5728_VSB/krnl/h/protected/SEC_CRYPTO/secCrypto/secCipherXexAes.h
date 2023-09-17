/* secCipherXexAes.h - XEX-AES Private Header File */

/*
 * Copyright (C) 2016 Wind River Systems, Inc.
 * 
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
31may16,y_f  created (F6718)
*/

#ifndef __INCsecCipherXexAesh
#define __INCsecCipherXexAesh

/* includes */

#include <types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vxWorks.h>
#include <secCrypto.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* typedefs */

typedef struct secXexAesCtx
    {
    SEC_CIPHER_CTX  key1EncrCtx;
    SEC_CIPHER_CTX  key1DecrCtx;
    SEC_CIPHER_CTX  key2EncrCtx;
    size_t          sectorSize;
    long long       sectorNum;
    UINT8 *         pSectorTweak;
    } SEC_XEX_AES_CTX;

/* function declarations */

STATUS  secXexAesInit (SEC_XEX_AES_CTX * pCtx, const SEC_CIPHER_TEMPLATE *
                       pAesCipher, UINT8* key1, UINT8* key2, size_t sectorSize);
STATUS  secXexAesUpdate (SEC_XEX_AES_CTX * pCtx, unsigned char * out, int *
                         outLen, const unsigned char * in, int inLen, long long
                         sectorNum, int encr);
STATUS  secXexAesCleanup (SEC_XEX_AES_CTX * pCtx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCsecCipherXexAesh */
