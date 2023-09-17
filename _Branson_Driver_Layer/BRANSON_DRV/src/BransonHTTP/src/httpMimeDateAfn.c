/* httpMimeDateAfn.c - access functions for MIME header */

/* Copyright 1999,2010 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* Project: Wind Web Server, Version 2.0
 *
 * Copyright (c) 1996/97/98, 3Soft GmbH
 *
 * 3Soft GmbH, Wetterkreuz 19a
 * D-91058 Erlangen, Germany
 * infohttp@dreisoft.de, http://www.dreisoft.de/
 */

/*
modification history
--------------------
26sep14,r_w  fix static CHECKED_RETURN issue.
27jan10,h_y  support for LP64
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
08jul98,ck   separated out deprecated functions.
05apr98,ck   generic lists introduced.
08sep97,ics  created.
*/

/*
DESCRIPTION
This file contains functions to allow access to the "Date" and
"Expires" fields of an HTTP request.

INCLUDE FILES: http/private/httpDate.h
               http/private/httpReq.h
               http/httpSem.h
               http/httpLib.h
               http/private/httpInt.h
               http/httpList.h

NOMANUAL
*/


#include <string.h>

#include "httpLib.h"
#include "private/httpInt.h"
#include "private/httpDate.h"
#include "httpReq.h"


/*******************************************************************************
*
* httpMimeExpiresSetImmediate - set the "Expires" header to immediate expiry
*
* This routine sets the "Expires" field of the response MIME Header to
* immediate expiry. This indicates to the client that the response is valid
* only once. This will set the "Expires" header field to the value of the
* "Date header field. If the "Date" field is not yet set, it will be set now.
*
* Note:
* Setting the "Date" field after calling this function could cause
* the "Expires" field to become invalid.
*
* RESULTS: HTTP_OK if set operation was successful, HTTP_ERROR otherwise.
*/

HTTP_STATUS httpMimeExpiresSetImmediate
    (
    HTTP_REQ_ID     reqId       /* Handle of the active request */
    )
    {
    char * szDate;

    if ( (szDate = memPoolAlloc (httpReqMemPartIdGet (reqId), 40)) == NULL)
    {
    logInfo ("httpMimeExpiresSetImmediate: Out of pool memory");
    return (HTTP_ERROR);
    }

    /* If there's no "Date" header yet, set it to now! */
    (void) httpOsalDateGet(szDate);
    (void) httpMimeHdrSet (reqId, HDR_OUT, "Date", szDate);

    /* Now set the expiry date/time to the value of Date. */
    (void) httpMimeHdrSet (reqId, HDR_OUT, "Expires", szDate);

    (void)memPoolFree (httpReqMemPartIdGet (reqId), szDate);

    return HTTP_OK;
    }

