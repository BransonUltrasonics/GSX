/* httpRpmLog.c -  Logging functions */

/* Copyright (c) 1996-2004, 2010-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
11jun15,r_w  fix static analysis issue V7MAN-154.
26sep14,r_w  fix static CHECKED_RETURN issue.
17sep12,r_w  prevent log file bigger than max setting value
                 defect: WIND00375406  
06apr11,r_w  fix defect WIND00256669 make the log file to switch after it reaches a certain size
30mar11,r_w  fix defect WIND00257291 make API reference readable
02feb10,h_y  support for LP64 
09jan04,jws  eliminate HTTP_PREDEFINE use
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
05apr98,ck   authentication removed from logging.
15may97,mb   osal introduced.
07may97,mb   httpLogConfInit() added to get rid of global file name var.
28sep96,mb   configuration of logfile by variable in httpconfig.c.
23sep96,ck   written.
*/

/*
DESCRIPTION

INCLUDE FILES: http/private/httpReq.h
           http/httpLib.h
           http/httpRpms.h
           http/private/httpInt.h
           httpCfg.h

*/


#include <string.h>
#include <stdio.h>

#include "httpLib.h"
#include "httpRpms.h"
#include "httpReq.h"
#include "private/httpInt.h"
#include "httpCfg.h"

#ifdef INCLUDE_HTTP_LOG

/* locals */

static BOOL fLoggingActive = TRUE;
static char * szLogFile = NULL;
static OS_SEM_T logSem;
HTTP_STATUS httpRpmLogStatusSet(BOOL fStatus );
BOOL httpRpmLogStatusGet(void);

#ifndef HTTP_LOG_FILE_SIZE_MAX_BYTES
#define HTTP_LOG_FILE_SIZE_MAX_BYTES	100000
#endif

/***************************************************************************
*
* httpLogConfInit - initialize request logging module
*
* This routine sets the file name to use for logging and adds the logging
* module to the HTTP server system RPM list.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpLogConfInit
    (
    char * szFile     /* file name to store request history in */
    )
    {
    szLogFile = szFile;
    fLoggingActive = TRUE;
    (void)wmosSemCreate(&logSem, 1, 0);

    return ( (HTTP_STATUS)( ( httpSysRpmConfAdd (httpRpmLog) == HTTP_OK) &&
             ( szFile != NULL )  ? HTTP_OK : HTTP_ERROR ));
    }


/**************************************************************************
*
* httpRpmLog - log requests to file
*
* This routine logs all requests to the server to a file.
* Specify the file name with httpLogConfInit().
* For each request, a line containing the current time, the
* IP-Nr of the caller, the method, and so on, are written to the file.
*
* RETURNS: RPM_DONE if successful, RPM_ERROR otherwise.
*
* SEE ALSO: httpRpmLogStatusSet(), httpRpmLogStatusGet()
*/

short httpRpmLog
    (
    HTTP_REQ * pReq      /* Handle of the active request, needed for all */
                         /* http - API function calls                    */
    )
    {
    FILE * fdLogFile;
    char * szTmpBuffer;
    char previousLogFileName[256];
    unsigned int uCount;
    
    if (!fLoggingActive)
        return (RPM_OK);

    wmosSemTake(logSem, WAIT_FOREVER_K);    
    fdLogFile = fopen (szLogFile, "a");
    if (fdLogFile == NULL)
    {
      logError ("Error creating logfile!");
      (void) wmosSemGive(logSem);
      return (RPM_OK);                     /* we dont want to stop everything here */
    }
    
    szTmpBuffer = httpBufferGet (pReq);  /* get address of scratch buffer */

    (void) httpDateGet (szTmpBuffer);           /* get date string */
    strncat(szTmpBuffer,": from ",8);
    
    (void)httpClientIPGet (pReq, szTmpBuffer+strlen(szTmpBuffer)); /* get client ip address */
    
    uCount = (unsigned int)(strlen(szTmpBuffer) + strlen(httpMethods[pReq->sMethod].szName) + strlen(pReq->szAbsPath) + 9);
    
    if ((ftell(fdLogFile) + uCount) > HTTP_LOG_FILE_SIZE_MAX_BYTES)
    {
      (void) fclose (fdLogFile);
     	fdLogFile = NULL;
     	(void)snprintf(previousLogFileName, 255, "%s.prev",szLogFile);
      (void)remove(previousLogFileName);					 

     	if(rename(szLogFile, previousLogFileName) < 0)
     	{
     	  logError("Error rename workinglogfile!");
     	  (void) wmosSemGive(logSem);
     	  return (RPM_OK); 
     	}
     		     			
     	fdLogFile = fopen (szLogFile, "a");
      if (fdLogFile == NULL)
      {
        logError ("Error creating logfile!");
        wmosSemGive(logSem);
        return (RPM_OK); 
      }
    }
    
    (void)fprintf(fdLogFile,"%s : %s on %s.\n",
            szTmpBuffer, httpMethods[pReq->sMethod].szName, pReq->szAbsPath);

    (void) fclose (fdLogFile);
    (void) wmosSemGive(logSem);

    return (RPM_OK);
    }


/***************************************************************************
*
* httpRpmLogStatusSet - switch logging on or off
*
* This routine switches logging on or off.
*
* NOTE:
* The routine httpRpmLog() must be included in the kernel.
*
* RETURNS: HTTP_OK.
*
* SEE ALSO: httpRpmLog(), httpRpmLogStatusGet()
*/

HTTP_STATUS httpRpmLogStatusSet
    (
    BOOL fStatus      /* TRUE or FALSE to switch logging ON or OFF */
    )
    {
    fLoggingActive = fStatus;
    return (HTTP_OK);
    }


/***************************************************************************
*
* httpRpmLogStatusGet - get the current logging status.
*
* This routine gets the current logging status.
*
* NOTE:
* The routine httpRpmLog() must be included in the kernel.
*
* RETURNS: TRUE or FALSE, when logging is ON or OFF.
*
* SEE ALSO: httpRpmLog(), httpRpmLogStatusSet()
*/

BOOL httpRpmLogStatusGet
    (
    void
    )
    {
    return (fLoggingActive);
    }
#endif /* INCLUDE_HTTP_LOG */

