/* wmbMibwayLib.h(wmmLib.h) */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCwmmLibh
#define __INCwmmLibh

#include "mibway.h"

#ifdef __cplusplus
extern "C" {
#endif

STATUS wmmSnmpInit  (WMB_SESSION_T * pSession);
STATUS wmmHandler   (WMB_TID_T tid, WMB_OBJ_T obj, COMMAND_T command);
STATUS wmmMain      (sbyte * pBackplaneName);
STATUS wmmTargetSet (WMB_SESSION_T * pSession, WM_BUFFER_T * pBuf);
STATUS wmmTargetGet (WMB_SESSION_T * pSession, WM_BUFFER_T * pBuf);
STATUS wmmSystemHandler (WMB_TID_T tid, WMB_OBJ_T obj, COMMAND_T command);


#ifdef __cplusplus
}
#endif


#endif /* ! __INCwmmLibh */
