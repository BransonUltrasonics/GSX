/* httpOsalTask.h - HTTP server OSAL task header */

/* Copyright 1997-1999 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,09sep99,csr  merging for zlib
01b,09jun99,km   merge of Wind Web Server 2.0 code.
01a,14may97,mb   written.
*/

#ifndef __INChttpOsalTaskh
#define __INChttpOsalTaskh

#ifdef __cplusplus
extern "C" {
#endif

/* function declarations */

HTTP_STATUS httpOsalInit();
void httpOsalLogErrorFunction (char *msg);
void httpOsalLogInfoFunction (char *msg);

#ifdef DEBUG_ERROR
#define httpOsalLogError(msg) httpOsalLogErrorFunction (msg)
#else
#define httpOsalLogError(msg)
#endif /* DEBUG_ERROR */

#ifdef DEBUG_INFO
#define httpOsalLogInfo(msg)  httpOsalLogInfoFunction (msg)
#else
#define httpOsalLogInfo(msg)  
#endif /* DEBUG_INFO */


#ifdef __cplusplus
}
#endif

#endif /* __INChttpOsalTaskh */

