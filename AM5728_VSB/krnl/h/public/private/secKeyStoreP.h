/* secKeyStoreP.h - #BRIEF# library header file */

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
01jan14,r_b  #DESC#
*/

#ifndef __INCsecKeyStorePh
#define __INCsecKeyStorePh

/* includes */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* defines */

/* typedefs */

/* function declarations */
    void* secKeyStoreOpen(void);
    int secKeyStoreRead(void* handle, char* name, int len);
    STATUS secKeyStoreClose(void* handle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCsecKeyStorePh */
