/* httpMimeHost.c - parse Host header on a HTTP request */

/* Copyright 2002-2004,2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,27jan10,h_y  support for LP64
01c,19feb04,jws  add copyright
01b,30sep03,jws  remove assert()'s
01a,31jan02,jc   initial creation.
*/

/*
DESCRIPTION
This file contains a MIME header processing function to retrieve
the information transmitted in the "Accept" header field.
This is useful for HTTP content negotiation, eg to provide pictures
only in some format the client browser can understand.

INCLUDE FILES: http/httpLib.h
               http/httpMime.h

NOMANUAL
*/

#include <string.h>
#include <stdio.h>

#include "httpLib.h"
#include "httpMime.h"


/*******************************************************************************
*
* httpMimeHost - parse MIME Host header
*
* This routine parses the MIME Host statement. It is invoked by
* the MIME parser to extract the "Host:" header from the client's
* HTTP request. The MIME parser will call httpMimeHost only
* if "Host:" was found in a request header.
*
* NOTE:
* Do not change szLine.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpMimeHost
    (
    HTTP_REQ_ID reqId,      /* Handle of the active request */
    char * szLine           /* line to parse                */
    )
    {

    return ((HTTP_STATUS)( httpSetEnv ( reqId , "HOST" , szLine ) != NULL
                 ? HTTP_OK : HTTP_ERROR ));
    }
