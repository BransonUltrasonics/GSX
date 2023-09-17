/* httpMimePersistentConnectionAfn.c - access functions for MIME header */

/* Copyright 1999-2003,2010 Wind River Systems, Inc. */

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
01e,02feb10,h_y   support for LP64 
01d,19aug03,jws  remove HTTP_BOOL
01c,31aug99,km   fixed incorrect heading for httpMimePersistentConnectionSet
                 for docs/refgen.
01b,18jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01a,05apr98,ck   created.
*/

/*
DESCRIPTION
This file contains especially HTTP_REQ access functions which
are useful for connection determination (HTTP1.1, persistent).

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


/*******************************************************************************
*
* httpMimePersistentConnectionGet - get the persistent connection status
*
* This routine is used to get the connection type of the current transmission.
*
* NOTE:
* Do not change the type after the header generation!
*
* RESULTS: TRUE for persistent connection, or FALSE otherwise.
*/

BOOL httpMimePersistentConnectionGet
    (
    HTTP_REQ_ID      reqId     /* Handle of the active request */
    )
    {
    return reqId->fPersistent;
    }


/*******************************************************************************
*
* httpMimePersistentConnectionSet - set the persistent connection status
*
* This routine is used to set the connection type of the current transmission.
* Normally this is done automatically by the MIME-type parser.
* This routine should only be called once.
*
* fPersistent = TRUE will cause a persistent connection.
* Eventually you will need to do some chunked transfer encoding!
*
* NOTE:
* The status can not be changed after the header generation is completed!
*
* RESULTS: HTTP_OK if set operation was successful, HTTP_ERROR otherwise.
*
* SEE ALSO: httpHeaderGenerate(), httpMimeTransferEncoding()
*/

HTTP_STATUS httpMimePersistentConnectionSet
    (
    HTTP_REQ_ID     reqId,       /* Handle of the active request */
    BOOL            fPersistent  /* Status of the connection     */
    )
    {
    if (reqId->fHdrWritten)  return HTTP_ERROR;

    reqId->fPersistent = fPersistent;

    return HTTP_OK;
    }

