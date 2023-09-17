/* httpMimeKeepAlive.c - parse functions for MIME header */

/* Copyright 1999,2010 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* Project: Wind Web Server, Version 2.0
 *
 * Copyright (c) 1996/97/98/99, 3Soft GmbH
 *
 * 3Soft GmbH, Wetterkreuz 19a
 * D-91058 Erlangen, Germany
 * infohttp@dreisoft.de, http://www.3soft.de/
 */

/*
modification history
--------------------
01b,27jan10,h_y  support for LP64
01a,13jan99,ck   written.
*/

/*
DESCRIPTION
This file contains especially HTTP_REQ access functions which
are useful connection type determination.

NOMANUAL

INCLUDE FILES: http/private/httpReq.h
               http/httpSem.h
               http/httpLib.h
           http/private/httpInt.h

*/

/* includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "httpLib.h"
#include "httpReq.h"
#include "private/httpInt.h"

HTTP_STATUS httpMimeKeepAlive
    (
    HTTP_REQ_ID reqId,      /* Handle of the active request */
    char * szLine           /* line to parse                */
    );


/*******************************************************************************
*
* httpMimeKeepAlive - parse MIME "KeepAlive" header field
*
* This routine parses the MIME "KeepAlive header field. This routine is
* invoked by the MIME parser to extract the "KeepAlive:" header from the
* clients HTTP request. This tag is an alternative to "connection: keep-alive"
* used by pre HTTP 1.1 clients to establish an persistent connection.
* With HTTP/1.1 this token is ignored!
* See "httpconfig.c" to learn how MIME parsing should be configured or
* manipulated.
*
* NOTE:
* Do not modify <szLine>.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpMimeKeepAlive
    (
    HTTP_REQ_ID reqId,      /* Handle of the active request */
    char * szLine           /* line to parse                */
    )
    {
    logInfo ("MIME KEEP-ALIVE:  detected");

    if (reqId->sProtocol < 1)                       /* If Protocol <= HTTP/1.0 (magic position!) */
    reqId->fPersistent = HTTP_TRUE;

    return (HTTP_OK);
    }

