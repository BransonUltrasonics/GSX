/* httpRpmCgi.c - CGI RPM */

/*
 * Copyright (c) 1996/97, 1999-2004,2007-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26sep14,r_w  fix static CHECKED_RETURN issue. 
30oct12,r_w  eliminate comments that out of date. defect WIND00384019
20jul12,r_w  eliminate memory leak issue. defect WIND00359749
12mar12,r_w  add puts() support for cgi routine
10oct11,r_w  remove tmp_stdin,tmp_stdout and related operation due to no need in vxWorks6.9 release
                CQID: Fix WIND00310593 
07jan11,r_w  not support stderr redirection yet, so remove tmp_stderr in httpRpmCgi() routine (WIND00236428)
02feb10,h_y  support for LP64 
06jan10,h_y  add free stdin,stdout,stderr for cgi routine (WIND00194202)
09mar09,y_z  remove free stdin for cgi routine
09feb09,y_z  change codes of restore STDIN and STDOUT 
31jan08,ps   Fix for defect 115598. memory leak in CGI path (unreclaimed
                 FILE *)
23feb07,msa  Correct the apigen comments so they match the actual
                 routines.
31jan07,ps   redirect stdin to socket as well for CGI emulation. defect
                 WIND00035216, fix WIND00086829
13apr04,adb  testing return code of mudStdoutToSocket and mudStdoutRestore
13jan04,gsr  cleanup routine for SPR 93009
16dec03,adb  corrected MUD over WMNET layering violation
10nov03,adb  WRS coding conventions modifications
31oct03,adb  added httpCgiConfigurationShow routine
11apr03,jws  fix compiler warning SPR 87426
07feb03,jws  fix compiler warnings from SPR 84193
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
15may97,mb   osal introduced.
04mar97,mb   generic lists support added.
25sep96,mb   initial creation.
*/

/*
DESCRIPTION
This RPM implements the Common Gateway Interface standard for WWW server software
to be able to port some CGI programs from other server software. To clarify the
motivation of this implementation: it is only intended for use because of
compatibility reasons, NOT for common use! CGI programs need more resources inside
the server kernel than normal RPMs do!

INCLUDE FILES: httpLib.h
               private/httpReq.h
               httpRpms.h
               httpShow.h
*/

/* includes */

#include <string.h>
#include <stdio.h>

#include "httpLib.h"
#include "httpReq.h"
#include "httpRpms.h"
#include "httpShow.h"
#include "httpCfg.h"

#ifdef INCLUDE_HTTP_CGI

/* globals */

/* locals */
LOCAL HTTP_GEN_LIST_ID httpCgiConfigId HTTP_PREDEFINE ( NULL ) ;

OS_SEM_T cgiExecuteSem = NULL;
/*
 * httpCgiExecuteSemInit() - create the CGI execution semaphore
 */

LOCAL OS_SEM_T httpCgiExecuteSemInit 
     (
      void
     )
     {
     if  ((OK != wmosSemMCreate
                   (&cgiExecuteSem, 
                    SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE))
         )
        {
        wmError (ERROR_GENERAL,
                 "httpCgiSemInit: failed to create cgiExecuteSem");

        cgiExecuteSem = NULL;
        }
     return cgiExecuteSem;
     }


/***************************************************************************
*
* httpCgiFnConfInit - initialize the sizes of the tables
*
* This routine initializes the internal boundaries needed for the
* subsequent calls of all the other API routines to access the
* CGI routine configuration list.
*
* RETURNS: List ID of the CGI routine configuration list or
* NULL if the list could not be initialized.
*
* SEE ALSO: httpCgiFnConfAdd()
*/

HTTP_GEN_LIST_ID httpCgiFnConfInit
    (
    unsigned short usTabMax /* Maximum number of entries in predefined table */
    )
    {
    httpCgiExecuteSemInit ();
    return ( httpCgiConfigId = httpGenListCreate ( httpGlobalMemPartIdGet(), usTabMax , HTTP_TRUE ) );
    }

/***************************************************************************
*
* httpCgiFnConfCleanup - cleanup the sizes of the tables
*
* This routine Cleanup the resources used by httpCgiFnConfInit
*
* RETURNS: HTTP_OK always 
*
* SEE ALSO: httpCgiFnConfInit
*/

HTTP_STATUS httpCgiFnConfCleanup 
    (
    void
    )
    {
    if  (NULL != httpCgiConfigId)
        {
        httpGenListDestroy (httpCgiConfigId);
        }

    httpCgiConfigId = NULL;
    
    if (cgiExecuteSem != NULL)
        {
    	(void)wmosSemDestroy(cgiExecuteSem);
        cgiExecuteSem = NULL;
        }

    return (HTTP_OK);

    } /* httpCgiFnConfCleanup */

/***************************************************************************
*
* httpCgiFnConfAdd - add an entry to the CGI RPM table
*
* This routine adds the specified function pointer to the
* CGI configuration table.
*
* NOTE: The string is not copied.  Only a pointer to its location is
* stored.
*
* RETURNS: List entry ID of the CGI routine table or
* NULL if the rouatine could not be registered in the list.
*
* SEE ALSO: httpCgiFnConfInit()
*/

HTTP_GEN_LIST_ENTRY_ID httpCgiFnConfAdd
    (
    char *   szName,    /* Name for the GCI function      */
    HTTP_CGI_FUNC pCgi  /* Corresponding function pointer */
    )
    {
    return ( httpGenListFuncAppend ( httpCgiConfigId , szName , (HTTP_FUNCPTR)pCgi ) );
    }

/***************************************************************************
*
* httpRpmCgi - execute a CGI program
*
* This is the CGI RPM routine.
* This routine implements the Common Gateway Interface standard for WWW server
* software, which facilitates porting CGI programs from other server software.
* This RPM is is designed for compatibility, and not for general
* use.  CGI programs
* require more resources from the server kernel than normal RPMs.
*
* RETURNS: RPM_DONE if output was successful, RPM_ERROR otherwise.
*/

short httpRpmCgi
    (
    HTTP_REQ_ID reqId
    )
    {
    HTTP_CGI_FUNC          pCgi;
    char *                 szScript,*szTmp;
    MEM_POOL_ID       reqMemId = httpReqMemPartIdGet ( reqId );
    HTTP_GEN_LIST_ENTRY_ID configId;
    sbyte4                 oldStdout = 0;
    sbyte4                 oldStdin  = 0;
    sbyte2                 oldStdoutFlag = 0;
    sbyte2                 oldStdinFlag  = 0;     
    
    if ( (szScript = httpStrdup ( reqMemId , httpRpmDataGet( reqId ))) == NULL)
    {
    logError ("httpRpmCgi: not enough request memory!" );
    return RPM_ERROR;
    }
    if ( (szTmp = strchr ( szScript , '/' )) != NULL )
    szTmp[0] = 0;

    /* set some CGI specific env vars */
    httpSetEnv ( reqId , "GATEWAY_INTERFACE" , "CGI/1.1" );
    httpSetEnv ( reqId , "SCRIPT_NAME"       , szScript );
    if ( szTmp != NULL )
    {
    szTmp[0] = '/';
    httpSetEnv ( reqId , "PATH_INFO"     , szTmp );
    }
    else
    httpSetEnv ( reqId , "PATH_INFO"     , "/" );


    /* free temporary request memeory */
   (void) memPoolFree ( reqMemId , szScript );

    logInfoMsg ( "httpRpmCGI: CGI function %s called" , httpGetEnv ( reqId , "SCRIPT_NAME" ) );
    if ( ( configId = httpGenListFind ( httpCgiConfigId , httpGetEnv ( reqId , "SCRIPT_NAME" ) ) ) != NULL )
    if ( ( pCgi = (HTTP_CGI_FUNC)httpGenListFuncGet ( configId ) ) != NULL )
        {
        logInfo ( "httpRpmCgi: trying to execute CGI func" );

        wmosSemTake (cgiExecuteSem, WAIT_FOREVER_K); 

        if  (OK != mudStdoutToSocket (reqId->ioHandle, (sbyte4 *)&oldStdout))
            {
            httpStatusSet( reqId, HTTP_INTERNAL_ERROR);
            (void) httpError( reqId, httpErrStringGet (HTTP_ERROR_INTERNAL));
            (void) wmosSemGive (cgiExecuteSem);
            return (RPM_DONE);
            }
        if  (OK != mudStdinToSocket (reqId->ioHandle, (sbyte4 *)&oldStdin))
            {
            (void)mudStdoutRestore (reqId->ioHandle, oldStdout);
            httpStatusSet( reqId, HTTP_INTERNAL_ERROR);
            (void) httpError( reqId, httpErrStringGet (HTTP_ERROR_INTERNAL));
            (void) wmosSemGive (cgiExecuteSem);
            return (RPM_DONE);
            }
#ifdef _WRS_KERNEL
        /* turn on line buffering, make it like tty device when call puts() or gets() in CGI routine*/
        oldStdoutFlag = stdioFp(STD_OUT)->_flags;
        oldStdinFlag  = stdioFp(STD_IN)->_flags;
        stdioFp(STD_OUT)->_flags |= __SLBF;
        stdioFp(STD_IN)->_flags  |= __SLBF;
#endif
        if  (HTTP_OK != pCgi(0,NULL,reqId))
            {
            httpStatusSet (reqId,HTTP_INTERNAL_ERROR);
            (void) httpError (reqId, httpErrStringGet (HTTP_ERROR_INTERNAL));
            }
#ifdef _WRS_KERNEL
        /* restore buffering flags */
        stdioFp(STD_OUT)->_flags = oldStdoutFlag;
        stdioFp(STD_IN)->_flags  = oldStdinFlag;
#endif        
        
        if  ( !((OK == mudStdoutRestore (reqId->ioHandle, oldStdout))
              && (OK == mudStdinRestore (reqId->ioHandle, oldStdin)))
            )
            {
            httpStatusSet( reqId, HTTP_INTERNAL_ERROR);
            httpError( reqId, httpErrStringGet (HTTP_ERROR_INTERNAL));
            /* 
             * shut down this connection; we know things arent quite 
             * right any more.
             */
            reqId->fPersistent = HTTP_FALSE;
            }

        (void) wmosSemGive (cgiExecuteSem); 
        return (RPM_DONE);
        }
    httpStatusSet( reqId, HTTP_NOT_FOUND );
    (void) httpError( reqId, httpErrStringGet (HTTP_ERROR_FILE_NOT_FOUND));
    return (RPM_ERROR);
    }

#ifdef HTTP_SHOW
/***************************************************************************
*
* httpCgiConfigurationShow - displays HTTP CGI Configuration 
*
* The CGI configuration list is indexed by URL prefixes and each entry's 
* displayed data is the corresponding internally resolvable executable
* service function pointer.
* 
* RETURNS: NA
*
* NOMANUAL
*/
void httpCgiConfigurationShow
    (
    void
    )
    {
    httpListShow (httpCgiConfigId, 
                  "httpCgiUrlCgiFunctionMatchList", 
                  FALSE,    /* entry id is a string */
                  TRUE,     /* entry data is a pointer */
                  NULL      /* left margin is not required */
                 );

    return;
    } /* httpCgiConfigurationShow */
#endif /* HTTP_SHOW */

#endif /* INCLUDE_HTTP_CGI */
