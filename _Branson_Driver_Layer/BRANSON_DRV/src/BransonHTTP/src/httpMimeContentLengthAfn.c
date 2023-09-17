/* httpMimeContentLengthAfn.c - access functions for MIME header */

/* Copyright(c) 1999-2003,2010,2015 Wind River Systems, Inc. */

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
11jun15,r_w  fix static analysis issue V7MAN-154.
27jan10,h_y  support for LP64
07aug03,jws  CONST and minor fix-ups
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
05apr98,ck   generic lists introduced.
13may97,mb   osal introduced.
30sep96,ck   split from requtiul (scalability!).
*/

/*
DESCRIPTION
This file contains especially HTTP_REQ access functions which
are useful for content length getting/setting

INCLUDE FILES: http/private/httpReq.h
               http/httpSem.h
               http/httpLib.h
               http/private/httpInt.h

NOMANUAL
*/

#include "stdlib.h"
#include <string.h>

#include "httpLib.h"
#include "httpReq.h"
#include "httpList.h"

static const char * const szMimeEntityKey = "Content-Length";

/**************************************************************************
*
* httpMimeContentLengthGet - get the MIME Content-Length header field
*
* This routine gets the Content-Length field of the selected MIME header.
* Use HDR_IN for the incoming MIME header, or HDR_OUT for the
* response header.
*
* RETURNS: The length of the HTTP body.
*/

long httpMimeContentLengthGet
    (
    HTTP_REQ_ID    reqId,     /* Handle of the active request */
    unsigned short usSelect   /* HDR_IN or HDR_OUT            */
    )
    {
    const char * szLength;

    szLength = httpMimeHdrGet (reqId, usSelect, szMimeEntityKey);

    if (*szLength == 0)  /* header not found! */
        return (-1);

    return (atol (szLength));
    }


/**************************************************************************
*
* httpMimeContentLengthSet - set the MIME Content-Length header field
*
* This routine sets the Content-Length field of the selected MIME header.
* Use HDR_IN for the incoming MIME header, or HDR_OUT for the
* response header.
*
* RETURNS: HTTP_OK if successful, HTTP_ERROR otherwise.
*/

HTTP_STATUS httpMimeContentLengthSet
    (
    HTTP_REQ_ID    reqId,     /* Handle of the active request */
    unsigned short usSelect,  /* HDR_IN or HDR_OUT            */
    long           lLength    /* New content length           */
    )
    {
    char szTmpBuffer [16];

    (void)snprintf (szTmpBuffer, 15,"%ld", lLength);

    return (httpMimeHdrSet (reqId, usSelect, szMimeEntityKey, szTmpBuffer));
    }

