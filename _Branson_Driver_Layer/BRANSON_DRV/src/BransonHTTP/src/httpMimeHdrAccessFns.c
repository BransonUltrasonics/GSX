/* httpMimeHdrAccessFns.c - MIME header processing functions */

/* Copyright 1999-2003,2010 Wind River Systems, Inc. */

/* Project: Wind Web Server, Version 2.0
 *
 * Copyright (c) 1996/97/98, 3Soft GmbH
 *
 * 3Soft GmbH, Wetterkreuz 19a
 * D-91058 Erlangen, Germany
 * infohttp@dreisoft.de, http://www.dreisoft.de/
 */

/*
modification history
--------------------
01e,27jan10,h_y  support for LP64
01d,07aug03,jws  remove assert()'s; SPR 88050
01c,18jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01b,20apr98,mb   assertion and error condition check bug fixed.
01a,04apr98,ck   initial creation.
*/

/*
DESCRIPTION
This file contains a MIME header processing function to retrieve
the information stored as generic lists.

INCLUDE FILES: http/private/httpReq.h
               http/httpLib.h
               http/private/httpInt.h
               http/httpMime.h
               http/httpList.h

NOMANUAL
*/

#include <string.h>
#include <stdio.h>

#include "httpLib.h"
#include "httpReq.h"
#include "private/httpInt.h"


/**************************************************************************
*
* httpMimeHdrGet - get a MIME Header line by name
*
* This routine gets a MIME header line value by name. This method is also used
* by various access functions, for backward compatibility.
*
* NOTE: Do not modify the returned value string
*
* RETURNS: pointer to value string, or pointer to NUL otherwise.
*/

const char * httpMimeHdrGet
    (
    HTTP_REQ_ID        reqId,     /* Handle of the active request     */
    ubyte2             usMimeHdr, /* Select Incoming/outgoing header */
    const char * const szName     /* Name of the entry                */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID argEntryId;
    static const char NullByte =  0 ;  /* here's a NUL byte */

    /* check parameters */

    if (((usMimeHdr != HDR_IN) && (usMimeHdr != HDR_OUT)) ||
        (szName == NULL) ||
        (reqId->MimeHdrListId[usMimeHdr] == NULL))
        {
        return &NullByte;
        }

    /* try to find a name and return a ptr to its value */

    argEntryId = httpGenListFind (reqId->MimeHdrListId[usMimeHdr],
                                  (char *) szName);
    if (argEntryId != NULL)
        {
        return ( httpGenListDataGet ( argEntryId ) );
        }

    return &NullByte;
    }


/**************************************************************************
*
* httpMimeHdrSet - set a MIME Header line by name
*
* This routine sets a MIME header line value by name. This method is also
* used by various access functions, for backward compatibility.
* Storage is done within the request memory pool using httpStrdup(),
* therefore all memory will be deallocated and erased after the request
* has been processed.
*
* RETURNS: HTTP_OK, or HTTP_ERROR if the variable list was full
*          or httpStrdup failed.
*/

HTTP_STATUS httpMimeHdrSet
    (
    HTTP_REQ_ID        reqId,     /* handle of the active request     */
    unsigned short     usMimeHdr, /* Select incoming/outgoing header */
    const char * const szName,    /* name of the MIME option          */
    const char * const szValue    /* value of the MIME option         */
    )
    {
    char *                 szId;
    char *                 szData;
    HTTP_GEN_LIST_ENTRY_ID argEntryId;
    HTTP_GEN_LIST_ID       list;
    MEM_POOL_ID            reqMemId;

    /* check parameters; we assume reqId is OK! */

    if (((usMimeHdr != HDR_IN) && (usMimeHdr != HDR_OUT)) ||
        (szName == NULL))
        {
        return HTTP_ERROR;
        }

    list = reqId->MimeHdrListId[usMimeHdr];  /* ptr to in or out list */
    if (list == NULL)
        {
        return HTTP_ERROR;
        }

    reqMemId = httpReqMemPartIdGet (reqId);

    /* If an entry exists for this name, get rid of it. */

    argEntryId = httpGenListFind (list, (char *) szName);
    if (argEntryId != NULL)
        {
        szId = httpGenListIdGet     (argEntryId);
        szData = httpGenListDataGet (argEntryId);
        httpGenListRemove (list, argEntryId);
        (void)memPoolFree   (reqMemId, szId);
        (void)memPoolFree   (reqMemId, szData);
        }

    szId   = httpStrdup (reqMemId, szName);   /* copy strings */
    szData = httpStrdup (reqMemId, szValue);

    /* Now, create a new entry in the list */

    if ((szId != NULL) && (szData != NULL))
        {
        argEntryId = httpGenListDataAppend (list, szId, szData);
        if (argEntryId != NULL)
            {
            return (HTTP_OK);
            }
        }

    /*
     * Storage for szId and izData is reclaimed when the request
     * is finished.  This might be dangerous, but we will assume it
     * is OK.
     */

    return (HTTP_ERROR);
    }

