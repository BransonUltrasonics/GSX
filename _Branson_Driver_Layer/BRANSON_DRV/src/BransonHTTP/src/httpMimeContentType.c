/* httpMimeContentType.c - access functions for MIME header */

/* Copyright 1999,2010 ,2014 Wind River Systems, Inc. */
#include "copyright_wrs.h"

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
26sep14,r_w  fix static CHECKED_RETURN issue.
27jan10,h_y  support for LP64
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
05apr98,ck   heavy rework for new MIME header concept;
                httpMimeBoundary renamed to httpBoundary.
13may97,mb   osal introduced.
10oct96,mb   added documentation.
30sep96,ck   split from other modules (scalability!).
*/

/*
DESCRIPTION
This file contains especially HTTP_REQ access functions which
are useful for content type getting/setting.

INCLUDE FILES: http/private/httpReq.h
               http/httpSem.h
               http/httpLib.h
           http/private/httpInt.h

NOMANUAL
*/


#include <string.h>

#include "httpLib.h"
#include "private/httpInt.h"
#include "httpReq.h"


HTTP_STATUS httpMimeContentType(HTTP_REQ_ID reqId,char *      szLine );
/*******************************************************************************
*
* httpMimeContentType - parse MIME content type
*
* This routine is called by the MIME parser to extract
* the "Content-Type" header from the client's HTTP request.
* The MIME parser  calls httpMimeContentType() only if "Content-Type:"
* is found in a request.
* See httpconfig.c to see how MIME parsing should be configured or
* manipulated.
*
* NOTE: Do not modify <szLine>.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpMimeContentType
    (
    HTTP_REQ_ID reqId,   /* Handle of the active request */
    char *      szLine   /* Line to parse                */
    )
    {
    char * szType;
    char * szTmp;
    char * szNext;
    char * szBoundary = NULL;

    szType = strchr (szLine, ' ');
    if (szType == NULL)
        szType = szLine;
    else
        szType++;
    szNext = strchr (szType, ';');
    if (szNext != NULL)
        {
        szNext[0] = (char) 0;
        szNext++;
        if ( ( szTmp = strstr (szNext, "boundary")) != NULL )
        {
        if ( ( szBoundary = strchr (szTmp, '=')) != NULL)
        {
        szBoundary ++;
        logInfoMsg ("BOUNDARY: >%s<",szBoundary);
        (void) httpBoundarySet (reqId, szBoundary);
        }
        }
        }
    logInfoMsg ("MIME CONTENT TYPE: >%s< detected",szType);
    httpSetEnv ( reqId , "CONTENT_TYPE" , szType );
    (void) httpMimeHdrSet (reqId, HDR_IN, "Content-Type", szType);

    return (HTTP_OK);
    }


/*******************************************************************************
*
* httpBoundaryGet - get the boundary header field (multipart docs)
*
* This routine gets the boundary string for multipart documents of the selected
* MIME header.  Use HDR_IN for the incoming MIME header, or
* HDR_OUT for the response header.
*
* RETURNS: A pointer to the boundary string, or to an empty string.
*/

char * httpBoundaryGet
    (
    HTTP_REQ_ID     reqId     /* Handle of the active request */
    )
    {
    return (reqId->szBoundary ? reqId->szBoundary : (char *)&httpNullByte);
    }


/*******************************************************************************
*
* httpBoundarySet - set the boundary header field (multipart docs)
*
* This routine sets the boundary string for multipart documents of the
* selected MIME header.  Use HDR_IN for the incoming MIME header,
* or HDR_OUT for the response header.
*
* NOTE:
* httpBoundarySet() uses memPoolAlloc() to store a copy of the
* string provided to it in the request's memory pool.  Calling this routine
* too often while processing a request decreases the available memory
* in the HTTPd's memory pool.  This pool is cleaned up automatically
* every time request processing is completed.
* Normally you do not need to set the MIME boundary header field,
* as the kernel sets it automatically.
*
* RETURNS: HTTP_OK if successful, HTTP_ERROR otherwise.
*/

HTTP_STATUS httpBoundarySet
    (
    HTTP_REQ_ID     reqId,       /* Handle of the active request */
    char *          szBoundary   /* Boundary string              */
    )
    {
    char *        szMem;

    if ((szMem = memPoolAlloc (httpReqMemPartIdGet (reqId), strlen (szBoundary)+1)) == NULL)
    {
    logInfo ("httpBoundarySet: Out of pool memory");
    return (HTTP_ERROR);
    }

    strcpy (szMem,szBoundary);
    if (reqId->szBoundary)
    (void)memPoolFree (httpReqMemPartIdGet (reqId), reqId->szBoundary);
    reqId->szBoundary = szMem;

    return (HTTP_OK);
    }

