/* secCrypto.h - SEC_CRYPTO library header file */

/*
 * Copyright (C) 2014,2015 Wind River Systems, Inc.
 * 
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
11dec15,rbo  Added VxWorks header file template
14aug14,rbo  Created
*/

#ifndef __INCsecCryptoh
#define __INCsecCryptoh

/* includes */

#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* defines */

#define SEC_CRYPTO_ID_BASE  1000
#define SEC_CRYPTO_ID_AES_128_CBC     1 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_AES_192_CBC     2 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_AES_256_CBC     3 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_AES_128_CTR     4 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_AES_192_CTR     5 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_AES_256_CTR     6 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_AES_128_ECB     8 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_AES_256_ECB     9 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_CAST_CBC       10 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_DES_CBC        11 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_3DES_CBC       12 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_RC4            13 + SEC_CRYPTO_ID_BASE
#define SEC_CRYPTO_ID_BF_CBC         14 + SEC_CRYPTO_ID_BASE


#define SEC_CIPHER_DECR              0
#define SEC_CIPHER_ENCR              1
#define SEC_CIPHER_FLAG_ENCR              1
#define SEC_CIPHER_FLAG_PKCS_PAD          (1 << 1)

#define SEC_CIPHER_CTRL_SET_PADDING  1
#define SEC_CIPHER_CTRL_GET_PADDING  2
#define SEC_CIPHER_CTRL_ENCR         3
#define SEC_CIPHER_CTRL_DECR         4
#define SEC_CIPHER_CTRL_GET_IV       5
#define SEC_CIPHER_CTRL_SET_IV       6

#define SEC_CIPHER_MAX_BLOCK_SIZE		32
#define SEC_CIPHER_AES_BLOCK_SIZE		16

/* typedefs */

    struct SEC_CIPHER_CTX_ST;

typedef struct SEC_CIPHER_TEMPLATE_ST
 {
     int type;
     int blockSize; /* bytes */
     int keyLen;    /* bytes */
     int ivLen;     /* bytes */
     int (*cipherInit)(struct SEC_CIPHER_CTX_ST* ctx, 
                       const unsigned char *key, 
                       const unsigned char *iv,
                       const int encr);
     int (*cipherUpdate)(struct SEC_CIPHER_CTX_ST* ctx, unsigned char* out, int* outLen, const unsigned char* in,int inLen);
     int (*final)(struct SEC_CIPHER_CTX_ST* ctx, unsigned char* out, int* outLen);
     int (*ctrl)(struct SEC_CIPHER_CTX_ST* ctx, int type, void* arg, unsigned int* len);
     int (*cleanup)(struct SEC_CIPHER_CTX_ST* ctx);
     int (*copy)(struct SEC_CIPHER_CTX_ST* copy, struct SEC_CIPHER_CTX_ST* org);
} SEC_CIPHER_TEMPLATE;

typedef struct SEC_CIPHER_CTX_ST
{
    const SEC_CIPHER_TEMPLATE* cipher;
    unsigned long flags;
    void* ctxData;
    unsigned char cache[SEC_CIPHER_MAX_BLOCK_SIZE]; /* Place to store temporary bytes when cipherUpdate input is not a full block */
    unsigned int cachedBytes; /* Number of bytes currently stored in cache */
    unsigned char finalCache[SEC_CIPHER_MAX_BLOCK_SIZE]; /* Place to temporarily store the last decrypted block when padding is used */
    unsigned int finalCachedBytes; /* Number of bytes currently stored in cache */

} SEC_CIPHER_CTX ;


/* function declarations */

STATUS secCipherInit(SEC_CIPHER_CTX* ctx, const SEC_CIPHER_TEMPLATE* template, const unsigned char* key, const unsigned char* iv, const int encr);
STATUS secCipherUpdate(SEC_CIPHER_CTX* ctx, unsigned char* out, int* outLen, const unsigned char* in,int inLen);
STATUS secCipherFinal(SEC_CIPHER_CTX* ctx, unsigned char* out, int* outLen);
STATUS secCipherCtrl(SEC_CIPHER_CTX* ctx, int type, void* arg, unsigned int* len);
STATUS secCipherCleanup(SEC_CIPHER_CTX* ctx);
STATUS secCipherCopy(SEC_CIPHER_CTX* copy, SEC_CIPHER_CTX* org);

    /* Sets the default template functions. These APIs are used to register a new provider
     * of an encryption algorithm  */
void secCipherAes128CbcTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherAes128CtrTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherAes128EcbTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherAes192CbcTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherAes192CtrTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherAes256CbcTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherAes256CtrTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherAes256EcbTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherAesCtrTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherBlowfishCbcTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherCastCbcTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherDesCbcTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipher3DesCbcTemplateSet(const SEC_CIPHER_TEMPLATE* ctx);
void secCipherRc4TemplateSet(const SEC_CIPHER_TEMPLATE* ctx);

    /* These APIs are used to fetch the default template for symmetrical encryption  algorithms */
const SEC_CIPHER_TEMPLATE* secCipherAes128CbcTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipherAes128CtrTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipherAes128EcbTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipherAes192CbcTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipherAes192CtrTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipherAes256CbcTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipherAes256CtrTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipherAes256EcbTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipherBlowfishCbcTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipherCastCbcTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipherDesCbcTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipher3DesCbcTemplateGet(void);
const SEC_CIPHER_TEMPLATE* secCipherRc4TemplateGet(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCsecCryptoh */
