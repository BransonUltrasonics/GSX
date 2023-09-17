/* secVaultKepObfuscatorCfg.c - Handle obfuscation of key-encrypting password */

/*
 * Copyright (C) 2015, 2016 Wind River Systems, Inc.
 * 
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01nov15,rbo  Created
*/

/*
DESCRIPTION
This configlette handles the obfuscation of the key-encrypting password of the VxWorks Vault
...
INCLUDE FILES: [fileName.h ... | N/A]
*/

/* includes */
#include <string.h>

/* defines */

/* typedefs */

/* globals */

/* locals */

/* forward declarations */


char secVaultKepObfuscatorGetChar(int chr)
{
    char ret;

    ret = (char)(chr % 126);
    if (ret < 32)
        ret = (char)(ret + 32);

    return ret;
}

int secVaultKepObfuscator(char* buf, int* len)
    {
    int i;
    char last = 17;

    for (i=0; i<(int)strlen(SEC_VAULT_KEY_ENCRYPTING_PW) && i<(*len)-1; i++)
    {
        buf[i] = secVaultKepObfuscatorGetChar((int)SEC_VAULT_KEY_ENCRYPTING_PW[i]+(int)last);
        last = buf[i];
    }
    buf[i] = 0;
    if (*len >= (int)strlen(SEC_VAULT_KEY_ENCRYPTING_PW))
        *len = (int)strlen(SEC_VAULT_KEY_ENCRYPTING_PW)-1;

    return OK;
    }
