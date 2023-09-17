/* wmbIdbLib.h - Method Routine for WMB */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbIdbLibh__
#define __INCwmbIdbLibh__

#ifdef __cplusplus
extern "C" {
#endif

#define IDB_NAME_K  "IDBPlus"

STATUS idbWmbRegister(int maxMIBObjects);
STATUS idbWmbShutdown(void * pDummy);
STATUS idbWmbObjectRegisterComplete(void);
STATUS idbWmbMibObjectRegister
    (
    char        * objName,  /* object name of the node */
    ulong_t     * Id,       /* object id of the node */
    uchar_t     oidLen,     /* length of object id */
    uchar_t     Type,       /* data type of the object */
    uchar_t     Access,     /* access allowed on the object */
    uchar_t     IdxCnt,     /* index count for the instance part */
    void        * Param     /* cookie parameter */
    );

#ifdef __cplusplus
}
#endif

#endif /* __INCwmbIdbLibh__ */
