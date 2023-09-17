/* httpRpmMime.c - MIME RPM   */

/* Copyright 1999,2010 ,2014 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* Project: Wind Web Server, Version 2.0
 *
 * Copyright (c) 1996/97, 3Soft GmbH
 *
 * 3Soft GmbH, Wetterkreuz 19a
 * D-91058 Erlangen, Germany
 * infohttp@dreisoft.de, http://www.dreisoft.de/
 */

/*
modification history
-------------------- 
26sep14,r_w  fix static CHECKED_RETURN issue.
02feb10,h_y   support for LP64 
07aug03,adb  fixed SPR 90117: handling of URI's ending at a period.
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
20apr98,mb   cleaned up.
15may97,mb   osal introduced.
07may97,mb   added some initialization function.
28sep96,mb   configuration moved to httpconfig.c.
09sep96,ck   beta version.
19jul96,mb   beautified.
30jun96,pk   added function headers.
14jun96,ck   initial creation.
*/

/*
DESCRIPTION
Generates a default Mime header entry for the reply document

INCLUDE FILES: http/private/httpReq.h
               http/httpLib.h
           http/httpRpms.h
           http/private/httpInt.h

NOMANUAL
*/


/* includes */
#include <string.h>

#include "httpLib.h"
#include "httpRpms.h"
#include "httpReq.h"
#include "private/httpInt.h"


/* statics */
static HTTP_MIME_SUFFIX_GUESS * suffixTab HTTP_PREDEFINE ( NULL );


/* function declarations */


/*******************************************************************************
*
* httpMimeConfInit - initialize the MIME content-type RPM
*
* This routine initializes and activates the MIME content-type RPM.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpMimeConfInit
    (
    HTTP_MIME_SUFFIX_GUESS * mimeSuffixTab
    )
    {
    suffixTab = mimeSuffixTab;
    return  httpSysRpmConfAdd (httpRpmMime); /* MIME type checking system RPM  */
    }


/******************************************************************************
*
* httpRpmMime - generate default MIME header
*
* This RPM sets the outgoing MIME "Content-type:" header field for the
* given request, based on the suffix of the filename part of the
* requested URL.  This can be used in conjunction with the filesystem
* RPMs (httpRpmFs() or httpRpmSsi()) to automatically set the content-type
* of the response based on the name of the requested file.  The
* mapping from filename suffix to content type is configured by the
* table passed to httpMimeConfInit().
*
* RETURNS: RPM_OK if output was successful, ERROR otherwise.
*/

short httpRpmMime
    (
    HTTP_REQ_ID reqId    /* Handle of the active request, needed for all */
                         /* http - API function calls                    */
    )
    {
    char *  szTmp;
    short   sIndex = -1;
    unsigned long ulLength;

    logInfo ("httpRpmMime called...");

    szTmp = strrchr (reqId->szAbsPath, '.');    /* Pointer to suffix */
    if (szTmp != NULL)                          /* suffix found...   */
        {
        strcpy (reqId->szBuffer, ++szTmp);

        if  (0 == (ulLength = strlen (reqId->szBuffer)))
            {
			/* send error status and prohibit URI's ending at a period */
			httpStatusSet( reqId, HTTP_BAD_REQUEST );
        	(void) httpError( reqId, httpErrStringGet(HTTP_ERROR_WRONG_URL));
            return (RPM_ERROR);
            }

        (void) httpStringToUpper (reqId->szBuffer);

        while (suffixTab [++sIndex].szSuffix != NULL)
            {
            if (httpStrNCaseCmp (reqId->szBuffer,
                 suffixTab [sIndex].szSuffix,
                 ulLength) == 0)
                {
                logInfoMsg("httpRpmMime: MIME type %s",suffixTab [sIndex].szType);
                (void) httpMimeContentTypeSet (reqId, HDR_OUT, suffixTab [sIndex].szType);
                break;
                }
            }
        }
    return (RPM_OK);
    }

