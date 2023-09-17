/* httpMimeIfMatch.c - parse functions for cache-related header */

/* Copyright 2011 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01a,26apr11,m_z initiate file
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

HTTP_STATUS httpHdrIfMatch(HTTP_REQ_ID reqId,char * szLine);
/*******************************************************************************
*
* httpMimeIfMatch - extract HTTP If-Match header
*
* This function is called by the cache header parser to extract
* the "If-Match" header from the client's HTTP request.
* The cache parser calls httpHdrIfMatch() only if "If-Match:"
* is found in a request header.
* See httpconfig.c to see how cache parsing should be configured or
* manipulated.
*
* NOTE: Do not modify <szLine>.
*
* RETURNS: OK or ERROR.
*/

HTTP_STATUS httpMimeIfMatch
    (
    HTTP_REQ_ID reqId,      /* Handle of the active request */
    char * szLine           /* line to parse                */
    )
    {
    logInfoMsg ("HEADER IF-MATCH: %s detected",szLine);
    httpSetEnv ( reqId , "If-Match" , szLine );
    httpSetEnv ( reqId , "IF-MATCH" , szLine );
    httpSetEnv ( reqId , "IF_MATCH" , szLine );

    return (httpMimeHdrSet (reqId, HDR_IN, "If-Match", szLine));
    }

