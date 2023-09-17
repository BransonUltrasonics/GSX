/* httpMimeContentLength.c - parse functions for MIME header */

/* Copyright 1999,2010 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* Project: Wind Web Server, Version 2.0
 *
 * Copyright (c) 1996/97/98, 3Soft GmbH
 *
 * 3Soft GmbH, Wetterkreuz 19a
 * D-91058 Erlangen, Germany
 * infohttp@dreisoft.de, http://www.3soft.de/
 */

/*
modification history
--------------------
02d,27jan10,h_y  support for LP64
02c,31jan07,ps  set correct CONTENT_LENGTH variable for CGI. defect
                 WIND00035216, fix WIND00086829
02b,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02a,05apr98,ck   generic lists introduced.
01b,12oct97,mb   access functions added.
01a,28sep96,ck   created.
*/

/*
DESCRIPTION
This file contains especially HTTP_REQ access functions which
are useful for content length determination.

INCLUDE FILES: http/private/httpReq.h
               http/httpLib.h

NOMANUAL
*/

/* includes */
#include <string.h>
#include <stdio.h>
#include "stdlib.h"

#include "httpLib.h"
#include "httpReq.h"

HTTP_STATUS httpMimeContentLength(HTTP_REQ_ID reqId,char * szLine);
/*******************************************************************************
*
* httpMimeContentLength - extract HTTP Content-Length header
*
* This function is called by the MIME parser to extract
* the "Content-Length" header from the client's HTTP request.
* The MIME parser calls httpMimeContentLength() only if "Content-Length:"
* is found in a request header.
* See httpconfig.c to see how MIME parsing should be configured or
* manipulated.
*
* NOTE: Do not modify <szLine>.
*
* RETURNS: OK or ERROR.
*/

HTTP_STATUS httpMimeContentLength
    (
    HTTP_REQ_ID reqId,      /* Handle of the active request */
    char * szLine           /* line to parse                */
    )
    {
    char * szTmp;

    szTmp = strrchr (szLine, ' ');    /* only one length specified */
    if (szTmp == NULL)
        szTmp = szLine;

    logInfoMsg ("MIME CONTENT LENGTH: %s detected",szTmp);
    httpSetEnv ( reqId , "Content-Length" , szTmp );
    httpSetEnv ( reqId , "CONTENT-LENGTH" , szTmp );
    httpSetEnv ( reqId , "CONTENT_LENGTH" , szTmp );

    return (httpMimeHdrSet (reqId, HDR_IN, "Content-Length", szTmp));
    }

