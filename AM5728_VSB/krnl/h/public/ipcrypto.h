/* ipcrypto.h - public API of Wind River Crypto */

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
*/

#ifndef IPCRYPTO_H
#define IPCRYPTO_H

/*
 ****************************************************************************
 * 1                    DESCRIPTION
 ****************************************************************************
 */

/*
DESCRIPTION
This library contains the API for Wind River Crypto
*/

/*
 ****************************************************************************
 * 2                    CONFIGURATION
 ****************************************************************************
 */
#include "ipcrypto_config.h"


/*
 ****************************************************************************
 * 3                    INCLUDE FILES
 ****************************************************************************
 */


#include <openssl/aes.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 ****************************************************************************
 * 4                    DEFINES
 ****************************************************************************
 */

/* IPCRYPTO release XYyZz,
 * X  = major version
 * Yy = minor version (two different versions might be incompatible)
 * Zz = patch version (two different versions are always compatible)
 */
#define IPCRYPTO_RELEASE 70

#ifndef AES_BLOCKSIZE
#define AES_BLOCKSIZE 16
#endif

#define AES_KEY_128_BYTES       16
#define AES_KEY_192_BYTES       24
#define AES_KEY_256_BYTES       32
#define AESKW_BLOCKSIZE         8


/*
 ****************************************************************************
 * 5                    TYPES
 ****************************************************************************
 */

typedef struct _AES_CMAC_CONTEXT
{
    unsigned char      IV[AES_BLOCKSIZE];         /* IV initialization vector   */
    unsigned int      ivLength;                    /* IV length */
    unsigned char      k1[AES_BLOCKSIZE];         /* k1 subkey                  */
    unsigned char      k2[AES_BLOCKSIZE];         /* k2 subkey                  */
    AES_KEY            aesKey;                     /* AES_KEY used for cipher */
} AES_CMAC_CTX;

    /* Forward declaration */
    struct env_md_st;

/*
 ****************************************************************************
 * 6                    FUNCTIONS
 ****************************************************************************
 */

/*******************************************************************************
*
* ipcrypto_crc32 -
*
* \NOMANUAL:
*
*/
    extern unsigned int ipcrypto_crc32(
				    const unsigned char* s,
				    unsigned int len
				    );

/*******************************************************************************
*
* ipcrypto_key_fingerprint_bubblebabble -
*
* \NOMANUAL:
*
*/
    extern char * ipcrypto_key_fingerprint_bubblebabble(
				    unsigned char* dgst_raw,
				    unsigned int dgst_raw_len
				    );
/*******************************************************************************
*
* aesCmacInit - initialize a new AES CMAC context
*
* This routine is used to initialize an AES_CMAC_CTX object for a new CMAC
* calculation. It calculates the CMAC subkeys and stores them
* in the AES CMAC context.
*
* Parameters:
* \is
* \i <cmacCtx>
* Pointer to the AES_CMAC_CTX structure to be initialized.
*
* \i <key>
* Pointer to the key buffer. If 'key==NULL' then the subkeys stored in 'cmacCtx' are
* used. In this way the context can be reused even after calling aesCmacFinal() if the
* key is the same.
*
* \i <keyLength>
* Length of the key in bytes.
* \ie
*
* RETURNS: 1 on success, -1 on error
*
*/
    extern int  aesCmacInit(
			       AES_CMAC_CTX *cmacCtx,
			       unsigned char *key,
			       int keyLength
			       );
/*******************************************************************************
*
* aesCmacUpdate - update the AES CMAC context with data
*
* This routine is used to add data to the AES CMAC context without finalizing the
* calculation. It may be called multiple times to add to the message
* to be authenticated.
*
* Parameters:
* \is
* \i <cmacCtx>
* Pointer to the AES_CMAC_CTX structure to be used.
*
* \i <input>
* Pointer to the input message.
*
* \i <inputLength>
* Length of the input buffer.  Must be a multiple of AES_BLOCKSIZE.
* \ie
*
* RETURNS: 1 on success, -1 on error
*
*/
    extern int  aesCmacUpdate(
				 AES_CMAC_CTX *cmacCtx,
				 unsigned char *input,
				 int inputLength
				 );
/*******************************************************************************
*
* aesCmacFinal - update the AES CMAC context with data and finalize the CMAC calculation.
*
* This routine is used to add data to the AES CMAC context and generate the CMAC for
* data that has been added to the context.
*
* Parameters:
* \is
* \i <cmacCtx>
* Pointer to the AES_CMAC_CTX structure to be used.
*
* \i <input>
* Pointer to the input message.
*
* \i <inputLength>
* Length of the input buffer. Must be a multiple of AES_BLOCKSIZE.
*
* \i <outputMac>
* Pointer to the buffer where the output CMAC is copied.
*
* \i <outputMacLength>
* Pointer to a variable containing the length of the outputMac buffer. On input, '*outputMacLength'
* must be set to the size of the 'outputMac' buffer. On output, '*outputMacLength' is updated to the length
* of data copied into the 'outputMac' buffer.
* \ie
*
* RETURNS: 1 on success, -1 on error
*
*/

    extern int  aesCmacFinal(
				AES_CMAC_CTX *cmacCtx,
				unsigned char *input,
				int inputLength,
				unsigned char *outputMac,
				int *outputMacLength
				);
/*******************************************************************************
*
* aesCmacBlock - calculate the CMAC of a complete input message
*
* This routine is used to calculate the CMAC of the input message in one operation.
* If the same key is going to be used for multiple CMAC calculations, the AES_CMAC_CTX can
* be used to store the subkeys between calls.
*
* Parameters:
* \is
* \i <cmacCtx>
* Pointer to the AES_CMAC_CTX structure to be used. If 'cmacCtx==NULL' no subkeys are returned.
* If 'cmacCtx!=NULL' and 'key!=NULL', then the subkeys are calculated and returned in 'cmacCtx'.
* If 'cmacCtx!=NULL' and 'key==NULL', then the subkeys stored in 'cmacCtx' from a previous calculation are
* used, saving a recalculation.
*
* \i <key>
* Pointer to the variable length key
*
* \i <keyLength>
* Length in bytes of the key.
*
* \i <input>
* Pointer to the input message.
*
* \i <inputLength>
* Length of the input buffer.  Must be a multiple of AES_BLOCKSIZE.
*
* \i <outputMac>
* Pointer to the buffer where the output CMAC is copied.
*
* \i <outputMacLength>
* Pointer to a variable containing the length of the outputMac buffer. On input, '*outputMacLength'
* must be set to the size of the 'outputMac' buffer. On output, '*outputMacLength' is updated to the length
* of data copied into the 'outputMac' buffer.
* \ie
*
* RETURNS: 1 on success, -1 on error
*
*/
    extern int  aesCmacBlock(
				AES_CMAC_CTX *cmacCtx,
				unsigned char *key,
				int keyLength,
				unsigned char *input,
				int inputLength,
				unsigned char *outputMac,
				int *outputMacLength
				);

/*******************************************************************************
*
* aesCmacPrf128 - AES CMAC pseudo random function
*
* This routine is used to calculate the AES CMAC PRF128 (RFC 4615) used in IKEv2.
*
* Parameters:
* \is
* \i <key>
* Pointer to the variable length key.
*
* \i <keyLength>
* Length in bytes of the key.
*
* \i <input>
* Pointer to the input message.
*
* \i <inputLength>
* Length of the input buffer. Must be a multiple of AES_BLOCKSIZE.
*
* \i <output>
* Pointer to the buffer where the output is copied.
*
* \i <outputLength>
* Pointer to a variable containing the length of the output buffer. On input, '*outputMacLength'
* must be set to the size of the 'outputMac' buffer, and must be at least AES_BLOCKSIZE. On output, '*outputMacLength' is updated to the length
* of data copied into the 'outputMac' buffer.
* \ie
*
* RETURNS: 1 on success, -1 on error
*
*/

    extern int  aesCmacPrf128(
				 unsigned char *key,
				 int keyLength,
				 unsigned char *input,
				 int inputLength,
				 unsigned char *output,
				 int *outputMacLength
				 );

/*******************************************************************************
*
* aeskw_wrap - encrypt a key using AES Keywrap
*
* This routine encrypts a key using AES Keywrap.
*
* Parameters:
* \is
* \i <key>
* Pointer to key encryption key buffer.
*
* \i <keyLength>
* Length of key in key buffer in bytes. Must be 16, 24 or 32 bytes.
*
* \i <iv>
* Pointer to initialization vector buffer.
*
* \i <ivLength>
* Length of IV in bytes. Must be AESKW_BLOCKSIZE.
*
* \i <plainText>
* Pointer to buffer to encrypt.
*
* \i <plainTextLength>
* Length of plainText buffer.  Must be a multiple of AESKW_BLOCKSIZE.
*
* \i <cipherText>
* Pointer to output buffer.
*
* \i <cipherTextLength>
* Pointer to a variable containing the length of the output buffer. On input, '*cipherTextLength'
* must be set to the size of the 'cipherText' buffer. On output, '*cipherTextLength' is updated to the length
* of data copied into the 'cipherText' buffer.
*
* \ie
*
* RETURNS: 1 on success, -1 on error
*
*/
    extern int  aeskw_wrap(
			      unsigned char *key,
			      int keyLength,
			      unsigned char *iv,
			      int ivLength,
			      unsigned char *plainText,
			      int plainTextLength,
			      unsigned char *cipherText,
			      int *cipherTextLength
			      );

/*******************************************************************************
*
* aeskw_unwrap - decrypt a key using AES Keywrap
*
* This routine decrypts a key using AES Keywrap.
*
* Parameters:
* \is
* \i <key>
* Pointer to key encryption key buffer.
*
* \i <keyLength>
* Length of key in key buffer in bytes. Must be 16, 24 or 32 bytes.
*
* \i <iv>
* Pointer to initialization vector buffer.
*
* \i <ivLength>
* Length of IV in bytes. Must be AESKW_BLOCKSIZE.
*
* \i <cipherText>
* Pointer to buffer to decrypt.
*
* \i <cipherTextLength>
* Length of cipherText buffer. Must be a multiple of AESKW_BLOCKSIZE.
*
* \i <plainText>
* Pointer to output buffer.
*
* \i <plainTextLength>
* Pointer to a variable containing the length of the output buffer. On input, '*plainTextLength'
* must be set to the size of the 'plainText' buffer. On output, '*plainTextLength' is updated to the length
* of data copied into the 'plainText' buffer.
*
* \ie
*
* RETURNS: 1 on success, -1 on error
*
*/

    extern int  aeskw_unwrap(
				unsigned char *key,
				int keyLength,
				unsigned char *iv,
				int ivLength,
				unsigned char *cipherText,
				int cipherTextLength,
				unsigned char *plainText,
				int *plainTextLength
				);


void ipcrypto_rsa_oaep_encode(const struct env_md_st* hash,
                                        unsigned char *in,
                                        int inlen,
                                        unsigned char *out,
                                        int outlen);

int ipcrypto_rsa_oaep_decode(const struct env_md_st* hash,
                                          unsigned char *in,
                                          int in_len,
                                          unsigned char *out,
                                          int* out_len);

    struct rand_meth_st * vxRandLibMeth(void);

#ifdef __cplusplus
}
#endif

#endif

/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */
