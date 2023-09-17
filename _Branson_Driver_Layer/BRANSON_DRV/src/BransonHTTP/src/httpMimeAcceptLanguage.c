/* httpMimeAcceptLanguage.c - MIME header processing function for Accept-Language */

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
01g,02feb10,h_y   support for LP64 
01f,30sep03,jws  remove assert()'s
01e,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01d,04jun97,mb   httpMimeAcceptLanguage() uses environment to pass information.
01c,13may97,mb   osal introduced.
01b,10oct96,mb   documentation added.
01a,01oct96,mb   initial creation.
*/

/*
DESCRIPTION
This file contains a MIME header processing function to retrieve
the information transmitted in the "Accept-Language" header field.
This is useful for HTTP language negotiation, eg to provide an HTML
page in the human language prefered by the client user.

INCLUDE FILES: http/httpLib.h
               http/httpMime.h

NOMANUAL
*/

#include <string.h>
#include <stdio.h>

#include "httpLib.h"
#include "httpMime.h"
#include "httpCfg.h"

#ifdef INCLUDE_HTTP_LANGUAGE_NEGOTIATION
/*******************************************************************************
*
* httpMimeAcceptLanguage - parse HTTP Accept-Language
*
* This routine is called by the MIME parser to extract
* the Accept-Language header field from the client's HTTP request.
* The MIME parser calls httpMimeAcceptLanguage() only if "Accept-Language:"
* is found in a request.
* See httpconf.c to see how MIME parsing should be configured or manipulated.
*
* NOTE: The value of the Accept-Language header field is stored in the
* HTTP_ACCEPT environment variable.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpMimeAcceptLanguage
    (
    HTTP_REQ_ID reqId,
    char *      szLine
    )
    {

    return ( (HTTP_STATUS)(httpSetEnv ( reqId , "HTTP_ACCEPT" , szLine ) != NULL
                 ? HTTP_OK : HTTP_ERROR ));
    }
#endif

