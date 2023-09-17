/* httpMem.c - Memory management in pools. */

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
04b,17dec10,m_z part of APIs changed
                CQID: Fix WIND00247096
04a,02feb10,h_y   support for LP64 
03d,31jan04,adb  multiport functionality necessitated changes
03c,24sep03,jws  fix compiler warnings, SPR 90843
03b,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
03a,05apr98,ck   MIME-Hdr initialized with generic lists.
02b,13may97,mb   osal introduced.
02a,13mar97,ck   converted to use httpMemPartLib routines.
01b,07sep96,ck   added allocation of MIME hdrs in memInit().
01a,04sep96,mb   initial creation.
*/

/*
DESCRIPTION
This module provides pool memory management routines. To solve several
problems arising with dynamic request processing, an adequate memory
management is useful. In this file memory pools are encapsulated to provide
reusable memory for per-request-data without having any problems
with freeing it or system memory fragmentation.

INCLUDE FILES: http/httpLib.h private/httpReq.h
*/

#include "stdlib.h"
#include <string.h>

#include "httpLib.h"
#include "httpReq.h"

HTTP_REQ_ID httpReqStructInit( MEM_POOL_ID memId );

/**************************************************************************
*
* httpReqStructInit - (re)initialize memory pool and a new HTTP_REQ in it.
*
* The whole pool is cleared with zeros and a completely initialized HTTP_REQ
* is generated afterwards.
*
* RETURNS: A pointer to initialized HTTP_REQ residing inside pool memory.
*
* NOMANUAL
*/

HTTP_REQ_ID httpReqStructInit
    (
    MEM_POOL_ID memId
    )
    {
    HTTP_REQ_ID reqId;

    if (OK != memPoolCleanup(memId))
        {
        logInfo ("httpReqStructInit: memPoolCleanup() failed!");
        return (NULL);
        }

    if (NULL == (reqId = memPoolAlloc (memId, (ubyte4) sizeof (HTTP_REQ))))
        {
        logInfo ("httpReqStructInit: failed allocating HTTP_REQ!");
        return (NULL);
        }

    reqId->szAbsPath = memPoolAlloc (memId, httpMaxUrlSize);
    reqId->szBuffer  = memPoolAlloc (memId, httpMaxTempBufSize);
    reqId->MimeHdrListId[HDR_OUT] = httpGenListCreate (memId, httpMaxMimeLines,
                                                       HTTP_FALSE);
    reqId->MimeHdrListId[HDR_IN] = httpGenListCreate (memId, httpMaxMimeLines,
                                                       HTTP_FALSE);
    reqId->argListId = httpGenListCreate (memId, httpMaxArguments,
                                                       HTTP_FALSE);
    reqId->wmbPostErrListId = httpGenListCreate (memId, httpMaxArguments,
                                                       HTTP_FALSE);

    /* Those might be indeed zero, if no cookies configured! */

    reqId->sendCookieListId = httpGenListCreate (memId, httpMaxSendCookies,
                                                       HTTP_FALSE);
    reqId->recCookieListId = httpGenListCreate (memId, httpMaxRecCookies,
                                                       HTTP_FALSE);

    if ( (reqId->szAbsPath == NULL) ||
         (reqId->szBuffer  == NULL) ||
         (reqId->argListId == NULL) ||
         (reqId->MimeHdrListId[HDR_OUT] == NULL) ||
         (reqId->MimeHdrListId[HDR_IN]  == NULL) )
        {
        logInfo ("httpReqStructInit: not enough request memory!");
        return (NULL);
        }

    reqId->memPool = memId;
    return (reqId);
    }

/**************************************************************************
*
* httpReqMemPartIdGet - Get the ID of the memory pool of a request.
*
* This routine returns the ID of the memory pool for the current HTTP request.
* With this ID, memory can be allocated from the pool using memPoolAlloc().
* Allocating memory from the pool may fail if the pool is too small.
*
* The pool from which memory is allocated is generated upon receipt
* of an HTTP request, and is freed as soon as the request is completed.
* Therefore, the memory obtained by this call is only valid while
* the request is being processed, and is freed automatically thereafter.
*
* RETURNS: The ID of the memory partition, or NULL if error.
*
* SEE ALSO: memPoolFree(), memPoolAlloc(), httpGlobalMemPartIdGet()
*/

MEM_POOL_ID httpReqMemPartIdGet
    (
    HTTP_REQ_ID reqId
    )
    {

    return (reqId == NULL)  ?  NULL : reqId->memPool;
    }


/**************************************************************************
*
* httpGlobalMemPartIdGet - Get the ID of the global memory pool.
*
* This routine returns the ID of the global HTTPd memory pool.
* With this ID, you can allocate memory from the pool using memPoolAlloc().
* Before memory can be allocated from the pool, it must be created
* with  httpGlobalMemPartCreate(), which is usually done at server
* startup.
*
* Allocating memory from the pool will fail if the pool is too small.
*
* Warning: exit/shutdown reference holders before owner
*
* RETURNS: The ID of memory partition, or NULL if the pool is not initialized.
*
* SEE ALSO: httpGlobalMemPartCreate(), memPoolFree(), memPoolAlloc(),
* httpReqMemPartIdGet()
*/

MEM_POOL_ID httpGlobalMemPartIdGet (void)
    {
    MEM_POOL_ID memPoolId = NULL;

    if  (OK != mudPermMemPoolIdGet (httpMud, &memPoolId))
        {
        return (NULL);
        }
    else
        {
        return (memPoolId);
        }
    }

/**************************************************************************
*
* httpConservativeAlloc - Alloc a mem block without using up the entire pool.
*
* This function tries to allocate the the requested amount of memory
* from the request memory pool. If allocating that block would result in
* leaving less than 1 kbyte of memory in the pool, the function will
* cut the requested size in half in order to leave some mem to other RPMs.
*
*
* The allocated memory should be freed with memPoolFree
*
* RETURNS:
* \is
* \i <pMem>
*  A pointer to the newly allocated memory bloc, or NULL
* \i <pSize>
* [in]  The requested memory size, [out] The size of the allocated block
* (by reference).
* \ie
*/
sbyte * httpConservativeAlloc
    (
    MEM_POOL_ID memPool,
    size_t *    pSizeInBytes
    )
    {
    sbyte * newMem = NULL;

    unsigned long ulMaxTransBufSize = memPoolAvail (memPool, 1);
    unsigned long ulReqMemAvailSize = memPoolAvail (memPool, 0);

    ulMaxTransBufSize = ((ulReqMemAvailSize - ulMaxTransBufSize) > 1024) ?
                          ulMaxTransBufSize : (ulMaxTransBufSize/2);

    /* use available memory, align */
    ulMaxTransBufSize = (unsigned long) (ulMaxTransBufSize / 128) * 128;

    (*pSizeInBytes) = (ulMaxTransBufSize > (*pSizeInBytes)) ?
                      (*pSizeInBytes) : ulMaxTransBufSize;
    if ((*pSizeInBytes))
        newMem = memPoolAlloc (memPool, (*pSizeInBytes));
    else (*pSizeInBytes) = 0;
    return newMem;

    }




