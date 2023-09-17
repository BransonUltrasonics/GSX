/* httpMimeAccept.c - MIME header processing function for Accept */

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
* httpMimeAccept - extract Accept header field from HTTP request
*
* This routine is called by the MIME parser to extract
* the Accept header field from the client's HTTP request.
* The MIME parser calls httpMimeAcceptLanguage() only if "Accept:"
* is found in a request.
*
* NOTE: The value of the Accept header field is stored in the
* ACCEPT environment variable.
*
* NOMANUAL
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpMimeAccept
    (
    HTTP_REQ_ID reqId,
    char *      szLine
    )
    {

    return ( (HTTP_STATUS)((httpSetEnv ( reqId , "ACCEPT" , szLine ) != NULL)
                 ? HTTP_OK : HTTP_ERROR ));
    }

