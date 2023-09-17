/* secVault.h - #BRIEF# library header file */

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
15dec16,y_f  added a new API for KEP backend (F8021)
24aug16,r_b  added SEC_VAULT_KEY_ID_MAX and SEC_VAULT_MAX_SECRET (F6718)
01jan14,r_b  #DESC#
*/

#ifndef __INCsecVaulth
#define __INCsecVaulth

/* includes */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* defines */

#ifndef SEC_VAULT_KEY_ID_MAX
#define SEC_VAULT_KEY_ID_MAX 64
#endif

#ifndef SEC_VAULT_MAX_SECRET
#define SEC_VAULT_MAX_SECRET 256
#endif

/* typedefs */

typedef struct secKepBackend
    {
    STATUS  (*kepGet)       /* get KEP from backend */
        (
        char *  buf,
        int *   len
        );
    }SEC_VAULT_KEP_BACKEND;

/* deprecated type */

typedef int (*SEC_VAULT_KEP_CB)(char* buf, int* len);

/* function declarations */

/* secure vault */

int                 secVaultImport (const char * keyId, const char * key, int
                                    keyLen);
int                 secVaultGet (const char * keyId, char * key, int * keyLen);
int                 secVaultDel (const char * keyId);

/* KEP backend */

STATUS              secVaultKepBackendSet (SEC_VAULT_KEP_BACKEND * pBackend);
STATUS              secVaultKepGet (char * buf, int * len);

/* deprecated API */

int                 secVaultSetKepCb (SEC_VAULT_KEP_CB callback);
SEC_VAULT_KEP_CB    secVaultGetKepCb (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCsecVaulth */
