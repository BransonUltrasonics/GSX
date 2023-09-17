/* httpMimeIfUnmodifiedSince.c - parse functions for cache-related header */

/* Copyright 2011 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01a,20apr11,m_z initiate file
                CQID: WIND00203773
*/

/*
DESCRIPTION
This file contains especially HTTP_REQ access functions which
are useful for cache implementation.

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

HTTP_STATUS httpHdrIfUnmodifiedSince(HTTP_REQ_ID reqId,char * szLine);
/*******************************************************************************
*
* httpHdrIfUnmodifiedSince - extract HTTP If-Unmodified-Since header
*
* This function is called by the cache header parser to extract
* the "If-Unmodified-Since" header from the client's HTTP request.
* The cache parser calls httpHdrIfUnmodifiedSince() only if "If-Unmodified-Since:"
* is found in a request header.
* See httpconfig.c to see how cache parsing should be configured or
* manipulated.
*
* NOTE: Do not modify <szLine>.
*
* RETURNS: OK or ERROR.
*/

HTTP_STATUS httpHdrIfUnmodifiedSince
    (
    HTTP_REQ_ID reqId,      /* Handle of the active request */
    char * szLine           /* line to parse                */
    )
    {
    logInfoMsg ("HEADER IF-MODIFIED-SINCE: %s detected",szLine);
    httpSetEnv ( reqId , "If-Unmodified-Since" , szLine );
    httpSetEnv ( reqId , "IF-UNMODIFIED-SINCE" , szLine );
    httpSetEnv ( reqId , "IF_UNMODIFIED_SINCE" , szLine );

    return (httpMimeHdrSet (reqId, HDR_IN, "If-Unmodified-Since", szLine));
    }

