/* secVaultP.h - #BRIEF# library header file */

/*
 * Copyright (C) 2015-2016 Wind River Systems, Inc.
 * 
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
24aug16,r_b  removed SEC_VAULT_KEY_ID_MAX and SEC_VAULT_MAX_SECRET (F6718)
01jan14,r_b  #DESC#
*/

#ifndef __INCsecVaultPh
#define __INCsecVaultPh

/* includes */
#include <secVault.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* defines */
#ifndef SEC_VAULT_PATH_MAX
#define SEC_VAULT_PATH_MAX 128
#endif

#define SEC_VAULT_TYPE_PKEY 1
#define SEC_VAULT_TYPE_OPAQUE 2

#define SEC_VAULT_MODE_READ 1
#define SEC_VAULT_MODE_WRITE 2

/* Variables */

/* typedefs */

/* function declarations */
/*******************************************************************************
*
* secVaultOpen - open the database for reading
*
* This routine returns a handle that can be used to iterate all key IDs in the
* database. Note that public keys and buffers/pre-shared keys have different
* name spaces.
*
* RETURNS: A handle to be used in subsequent calls to secVaultRead
* and secVaultClose.
* NULL in case of error.
*
* ERRNO:
*/
    void* secVaultOpen(void);


/*******************************************************************************
*
* secVaultRead - return next key ID from the database
*
* This routine returns the next key ID from the database.
*
* Parameter:
* \is
* \i <handle>
* handle returned from secVaultOpen
* \i <name>
* buffer to store the key ID in
* \i <name>
* length of name buffer
* \ie
*
* RETURNS:
* IPCOM_SUCCESS if successful
* IPCOM_ERR_END if no more entries can be read
* IPCOM_ERR_FAILED in case of error
*
* ERRNO:
*/
    int secVaultRead(void* handle, char* name, int name_len);


/*******************************************************************************
*
* secVaultClose - close handle
*
* This routine closes a handle used to iterate all key IDs in the
* database.
*
* Parameter:
* \is
* \i <handle>
* handle returned from secVaultOpen
* \ie
*
* RETURNS:
* IPCOM_SUCCESS if the handle was closed
* IPCOM_ERR_FAILED if the handle could not be closed;
*
* ERRNO:
*/
    int secVaultClose(void* handle);



    void* secVaultOpenDir(int type);
    void secVaultInit(const char* rootDir);

    int secVaultKepGet(char* buf, int* len);

    FILE* secVaultFpGet(const char* keyId, int type, int mode);

    void secVaultLock(void);
    void secVaultUnlock(void);
    STATUS secVaultGetDirPath(const int type, char* filePath, size_t pathLen);
    STATUS secVaultGetFilePath(const char* keyId, const int type, char* filePath, size_t pathLen);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCsecVaultPh */
