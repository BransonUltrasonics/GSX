/* httpMimeContentEncodingAfn.c - access functions for MIME header */

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
02d,02feb10,h_y   support for LP64 
02c,07aug03,jws  CONST and other minor fix-ups
02b,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02a,05apr98,ck   generic lists introduced.
01c,13may97,mb   osal introduced.
01b,10oct96,mb   added documentation.
01a,30sep96,ck   split from requtiul (scalability!).
*/

/*
DESCRIPTION
This file contains especially HTTP_REQ access functions which
are useful for content encoding getting/setting.

INCLUDE FILES: http/private/httpReq.h
               http/httpSem.h
               http/httpLib.h
               http/private/httpInt.h

NOMANUAL
*/


#include <string.h>

#include "httpLib.h"
#include "httpList.h"
#include "httpReq.h"

static const char * const szMimeEntityKey = "Content-Encoding";

/**************************************************************************
*
* httpMimeContentEncodingGet - get MIME Content-Encoding header field
*
* This routine gets Content-Encoding field of the selected MIME header.
* Select HDR_IN for the incoming MIME header, or HDR_OUT for the
* response header.
*
* NOTE:
* Do not change the returned string.
*
* RETURNS: A pointer to the Content-Encoding string, or an empty string.
*/

char * httpMimeContentEncodingGet
    (
    HTTP_REQ_ID      reqId,     /* Handle of the active request */
    unsigned short   usSelect   /* HDR_IN or HDR_OUT            */
    )
    {

    /* cast because httpMimeHdrGet() returns (const char *) */

    return (char *) httpMimeHdrGet (reqId, usSelect, szMimeEntityKey);
    }


/**************************************************************************
*
* httpMimeContentEncodingSet - set the MIME content-encoding header field
*
* This routine sets the Content-Encoding of the selected MIME header.
* Use HDR_IN for the incoming MIME header, or HDR_OUT for the
* response header.
*
* NOTE:
* httpMimeContentEncodingSet() uses memPoolAlloc() to store a copy of
* the string in the request's memory pool.
* Calling this function too often while
* processing a request decreases the amount of free memory in
* the HTTPd's memory pool.  This pool is cleaned up automatically
* after a request is processed.
*
* RETURNS: HTTP_OK if set operation was successful, HTTP_ERROR if not.
*/

HTTP_STATUS httpMimeContentEncodingSet
    (
    HTTP_REQ_ID     reqId,      /* Handle of the active request */
    unsigned short  usSelect,   /* HDR_IN or HDR_OUT            */
    char *          szType      /* Content encoding string      */
    )
    {
    return (httpMimeHdrSet (reqId, usSelect, szMimeEntityKey, szType));
    }

