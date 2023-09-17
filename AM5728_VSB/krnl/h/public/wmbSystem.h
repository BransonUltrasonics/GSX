/* wmbSystem.h - header for WindManage built-in elements */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbSystemh
#define __INCwmbSystemh

#ifdef __cplusplus
extern "C" {
#endif

STATUS wmbHandler (WMB_TRANSACTION_T * pTransaction, WMB_OBJECT_T * pObject,
                   COMMAND_T command);

STATUS wmbSystemLoad (sbyte * pName);


#ifdef __cplusplus
}
#endif

#endif /* __INCwmbSystemh */
