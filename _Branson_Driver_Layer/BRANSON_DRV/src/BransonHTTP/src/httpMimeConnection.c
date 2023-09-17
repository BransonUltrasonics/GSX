/* httpMimeConnection.c - parse functions for MIME header */

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
01c,27jan10,h_y  support for LP64
01b,18jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01a,05apr98,ck   created.
*/

/*
DESCRIPTION
This file contains especially HTTP_REQ access functions which
are useful connection type determination.

INCLUDE FILES: http/private/httpReq.h
               http/httpSem.h
               http/httpLib.h
           http/private/httpInt.h

NOMANUAL
*/

/* includes */
#include <string.h>
#include <stdio.h>
#include "stdlib.h"

#include "httpLib.h"
#include "private/httpInt.h"
#include "httpReq.h"

HTTP_STATUS httpMimeConnection(HTTP_REQ_ID reqId,char * szLine);
/*******************************************************************************
*
* httpMimeConnection - parse MIME connection statement
*
* This routine is parses the MIME connection statement. It is invoked by
* the MIME parser to extract the "Connection:" header from the clients
* HTTP request. The MIME parser will call httpMimeConnection only
* if "Connection:" was found in a request header.
* See "httpconfig.c" to learn how MIME parsing should be configured or
* manipulated.
*
* NOTE:
* Do not change the returned string.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpMimeConnection
    (
    HTTP_REQ_ID reqId,      /* Handle of the active request */
    char * szLine           /* line to parse                */
    )
    {
    char * szTmp;

    szTmp = strrchr (szLine, ' ');    /* only one token should be specified */
    if (szTmp == NULL)
        szTmp = szLine;
    else
    szTmp++;

    logInfoMsg ("MIME CONNECTION: >%s< detected", szTmp);

    if (httpStrNCaseCmp (szTmp, "close", 5) == 0)
    reqId->fPersistent = HTTP_FALSE;

    else if ((httpStrNCaseCmp (szTmp, "Keep-Alive", 10) == 0) ||
         (httpStrNCaseCmp (szTmp, "keepalive", 9) == 0) ||
         (httpStrNCaseCmp (szTmp, "Persist", 7) == 0))
    if (reqId->sProtocol >= 1)     /* If Protocol >= HTTP/1.1 (magic position) */
        reqId->fPersistent = HTTP_TRUE;

    return (HTTP_OK);
    }

