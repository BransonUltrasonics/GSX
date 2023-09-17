/* httpMimeContentTypeAfn.c - access functions for MIME header */

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
02d,27jan10,h_y  support for LP64
02c,07aug03,jws  CONST and minor fix-ups
02b,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02a,05apr98,ck   generic lists introduced.
01b,13may97,mb   osal introduced.
01a,30sep96,ck   split from requtiul (scalability!).
*/

/*
DESCRIPTION
This file contains especially HTTP_REQ access functions which
are useful content type getting/setting.

INCLUDE FILES: http/private/httpReq.h
               http/httpLib.h
               http/httpList.h

NOMANUAL
*/


#include <string.h>

#include "httpLib.h"
#include "httpReq.h"

static const char * const szMimeEntityKey = "Content-Type";

/**************************************************************************
*
* httpMimeContentTypeGet - get a pointer to the MIME Content-Type header field
*
* This routine gets the Content-Type field of the selected MIME Header.
* Use HDR_IN for the incoming MIME header, or HDR_OUT for the
* response header.
*
* RETURNS: A pointer to the Content-Type field, or an empty string if no
* Content-Type is set.
*/

char * httpMimeContentTypeGet
    (
    HTTP_REQ_ID  reqId,        /* Handle of the active request */
    ubyte2       usSelect      /* HDR_IN or HDR_OUT            */
    )
    {

    /* cast because httpMimeHdrGet() returns (const char *) */

    return (char *) httpMimeHdrGet (reqId, usSelect, szMimeEntityKey);
    }


/**************************************************************************
*
* httpMimeContentTypeSet - set the value of the MIME Content-Type header field
*
* This routine sets the Content-Type field of the selected MIME header.
* Use HDR_IN for the incoming MIME header, or HDR_OUT for the
* response header.
*
* NOTE:
* httpMimeContentTypeSet() uses memPoolAlloc() to store a copy of the
* string provided to it in the request memory pool.  Calling this routine
* too often while processing a request decreases the available memory
* in HTTPd's memory pool.  This pool is cleaned up automatically
* every time a request is completed.
*
* RETURNS: HTTP_OK if successful, HTTP_ERROR otherwise.
*/

HTTP_STATUS httpMimeContentTypeSet
    (
    HTTP_REQ_ID     reqId,     /* Handle of the active request */
    unsigned short  usSelect,  /* HDR_IN or HDR_OUT            */
    char *          szType     /* Mime content type            */
    )
    {
    return httpMimeHdrSet (reqId, usSelect, szMimeEntityKey, szType);
    }

