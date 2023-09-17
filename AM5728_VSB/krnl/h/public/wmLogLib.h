/* wmLogLib.h - WindManage logging library header */

/* Copyright 2001 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01a,01aug01,pns  written
02a,07dec01,pns  renamed, updated
*/

/*
DESCRIPTION
This is the header for WindManage logging
It allows for logging to file and/or syslog servers
*/

/* includes */

#ifndef __INCwmLogLibh
#define __INCwmLogLibh

typedef enum
    {
    LOG_TYPE_INVALID,
    LOG_TYPE_FILE,
    LOG_TYPE_SYSLOG,
    LOG_TYPE_STDERR
    } LOG_TYPE_T;

typedef void * LOG_ID_T;

#ifdef __cplusplus
extern "C" {
#endif

STATUS  wmLogOpen       (LOG_TYPE_T type, char * pName);
void    wmLogWrite      (char * pText);
void    wmLogClose      (void);
void    wmLogPrintf     (char * pMsg, ...);
void    wmLogReset      (void);
void    wmLogStderr     (BOOL enabled);
BOOL    wmLogStderrGet  (void);
void    wmLogSyslogSend (char * pHost, char * pMsg);
void    wmLogSyslog     (char * pHost);
char *  wmLogSyslogGet  (void);
void    wmLogThreads    (BOOL enabled);
BOOL    wmLogThreadsGet (void);

#ifdef __cplusplus
}
#endif


#endif /* __INCwmLogLibh */
