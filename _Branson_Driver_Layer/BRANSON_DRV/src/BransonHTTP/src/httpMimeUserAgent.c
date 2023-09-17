/* httpMimeUserAgent.c - MIME header processing function for User-Agent */

/* Copyright 1999-2003,2010 Wind River Systems, Inc. */

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
01d,27jan10,h_y  support for LP64
01c,30sep03,jws  remove assert()'s
01b,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01a,04jun97,mb   initial creation.
*/

/*
DESCRIPTION
This file contains a MIME header processing function to retrieve
the information transmitted in the "User-Agent" header field.
This is useful to retrieve information about the client that produced
the current request.

INCLUDE FILES: http/httpLib.h
               http/httpMime.h

NOMANUAL
*/

#include <string.h>
#include <stdio.h>

#include "httpLib.h"

HTTP_STATUS httpMimeUserAgent
    (
    HTTP_REQ_ID reqId,
    char *      szLine
    );



/*******************************************************************************
*
* httpMimeUserAgent - parse the HTTP User-Agent header field
*
* This routine is invoked by the MIME parser to extract
* the "User-Agent" header from the client's HTTP request.
* The MIME parser calls httpMimeUserAgent() only if "User-Agent:"
* is found in a request.
*
* NOTE: The value of the "User-Agent" header field is stored in the
* USER_AGENT environment variable.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpMimeUserAgent
    (
    HTTP_REQ_ID reqId,
    char *      szLine
    )
    {

    if (szLine == NULL)  return HTTP_ERROR;

    return ( (HTTP_STATUS)(httpSetEnv ( reqId , "USER_AGENT" , szLine ) != NULL
                 ? HTTP_OK : HTTP_ERROR ));
    }

