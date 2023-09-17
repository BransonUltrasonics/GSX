/* httpMimeEtag.c - parse functions for etag header */

/*
 * Copyright (c) 2010-2011, 2015-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
17feb16,r_w  fix compiler and static analysis warnings.
11jun15,r_w  fix static analysis issue V7MAN-154.
26sep14,r_w  fix static CHECKED_RETURN issue.
26apr11,m_z  initiate file CQID: WIND00203773
*/

/*
DESCRIPTION
This file contains http etag head functions which
are useful for cache implementation.

INCLUDE FILES: http/private/httpReq.h
               http/httpLib.h

NOMANUAL
*/

/* includes */
#include <stdlib.h>
#include <string.h>
#include "wm.h"
#include "memPoolLib.h"
#include "httpType.h"
#include "httpSem.h"
#include "httpList.h"
#include "httpLib.h"
#include "httpMimeEtag.h"
#include "httpReq.h"
#include "httpCfg.h"

#ifdef HTTP_ETAG_ENABLE

static HTTP_SEM_ID listEtagFilesSem = 0;
static HTTP_GEN_LIST_ID listEtagFiles = 0;
 /*******************************************************************************
*
* httpMimeInEtag - extract HTTP Etag header
*
* This function is called by the cache header parser to extract
* the "Etag" header from the client's HTTP request.
* The cache parser calls httpHdrInEtagGet() only if "Etag:"
* is found in a request header.
* See httpconfig.c to see how cache parsing should be configured or
* manipulated.
*
* NOTE: Do not modify <szLine>.
*
* RETURNS: OK or ERROR.
*/

HTTP_STATUS httpMimeInEtag
    (
    HTTP_REQ_ID reqId,      /* Handle of the active request */
    char * szLine           /* line to parse                */
    )
    {
    logInfoMsg ("HEADER Etag: %s detected",szLine);
    httpSetEnv ( reqId , "Etag" , szLine );
    httpSetEnv ( reqId , "ETAG" , szLine );

    return (httpMimeHdrSet (reqId, HDR_IN, "Etag", szLine));
    }

/***************************************************************************
*
* httpEtagInit - initiate HTTP Etag header support
*
* This function is called when web server initialization
*
* RETURNS: OK or ERROR.
*/
STATUS httpEtagInit
    (
    unsigned short usMax 
    )
    {

    listEtagFilesSem = httpOsalSemMCreate(HTTP_SEM_Q_PRIORITY);
    if (listEtagFilesSem == NULL)
        return (HTTP_ERROR);
 
    /* initiate etag file list */
    if (NULL == (listEtagFiles = httpGenListCreate (
        httpGlobalMemPartIdGet(), 
        usMax, 
        HTTP_TRUE)))
    {
        (void) httpOsalSemDelete(listEtagFilesSem);
        listEtagFilesSem = 0;
        return (HTTP_ERROR);
    }


    return HTTP_OK;
    }

/***************************************************************************
*
* httpEtagDestroy - disable HTTP Etag header support
*
* This function is called when web server shutdown 
*
* RETURNS: OK .
*/

STATUS httpEtagDestroy
    (
    )
    {

    if (0 != listEtagFiles) 
    {
        httpGenListDestroy(listEtagFiles);
        listEtagFiles = 0;
    }

   if (0 != listEtagFilesSem)
        {
        (void) httpOsalSemDelete(listEtagFilesSem);
        listEtagFilesSem = 0;
        }

    return HTTP_OK;
    }


/***************************************************************************
*
* httpEtagFileTestAndSet - register file for etag generating, get entry if 
*   registered 
*
* RETURNS: OK or ERROR.
*/

STATUS httpEtagFileTestAndSet
    (
    char *          fileName, 
    unsigned long * pEntryId
    )
    {
    HTTP_GEN_LIST_ENTRY_ID fileEntry;
    char *      entryName;
    MEM_POOL_ID memPartId = httpGenListMemPartIdGet(listEtagFiles);

    if (0 != listEtagFilesSem)
        (void) httpOsalSemTake(listEtagFilesSem);

    if(NULL == (fileEntry = httpGenListFind(listEtagFiles, fileName)))
        {
        if (NULL == (entryName = memPoolAlloc(memPartId, strlen(fileName)+1)))
            {
            (void) httpOsalSemGive(listEtagFilesSem);
            return HTTP_ERROR;
            }
        strncpy(entryName, fileName,strlen(fileName));

        if (NULL == (fileEntry = httpGenListDataAppend(listEtagFiles, entryName, 0)))
            {
            memPoolFree(memPartId, entryName);
            httpOsalSemGive(listEtagFilesSem);
            return HTTP_ERROR;
            }
        fileEntry->listItem.pData = fileEntry;
        if (NULL != pEntryId)
            *pEntryId = (unsigned long)fileEntry;
        httpOsalSemGive(listEtagFilesSem);
        return HTTP_OK;
        }
    else
        {
        *pEntryId = (unsigned long)fileEntry->listItem.pData;
        (void) httpOsalSemGive(listEtagFilesSem);
        return HTTP_OK;
        }
    
/*     httpOsalSemGive(listEtagFilesSem); */
/*     return HTTP_OK; */
    }

/***************************************************************************
*
* httpEtagFileRemove - deregister file entry  registered 
*
* RETURNS: OK or ERROR.
*/
STATUS httpEtagFileRemove
    (
    char *  fileName 
    )
    {
    HTTP_GEN_LIST_ENTRY_ID fileEntry;
    MEM_POOL_ID memPartId = httpGenListMemPartIdGet(listEtagFiles);
    char * entryName = NULL;

    (void) httpOsalSemTake(listEtagFilesSem);
    if( NULL != (fileEntry = httpGenListFind(listEtagFiles, fileName)))
        {
        entryName = httpGenListIdGet(fileEntry);
        httpGenListRemove(listEtagFiles, fileEntry);
        (void) memPoolFree(memPartId, entryName);
        (void) httpOsalSemGive(listEtagFilesSem);
        return HTTP_OK;
        }
    
    (void) httpOsalSemGive(listEtagFilesSem);
    return HTTP_ERROR;
    }

/***************************************************************************
* httpEtagValueSet - generate etag for specified register file
*/
STATUS httpEtagValueSet
    (
    HTTP_ETAG_BINARY *      src,    /* bin etag */
    char *                  dst     /* str etag , with min len 2*3*sizeof(long)+6 */
    )
    {
     (void)snprintf(dst, (HTTP_ETAG_MAX_LEN-1),
        "\"%08lX-%08lX-%08lX\"", 
        src->file, 
        src->size, 
        src->time);
    
    return HTTP_OK;
    }

/***************************************************************************
* httpEtagValueGet - analyse the etag to get binary tags
*
* RETURNS: OK or ERROR.
*/
STATUS httpEtagValueGet
    (
    const char *        src,    /* string etag */
    HTTP_ETAG_BINARY *  dst     /* binary etag */
    )
    {
    const char * pos;
    char c;
    int i, j;
    int start = 0, end = 0;
    unsigned long temp[3] = {0};

    if (NULL == src || NULL == dst)
        return HTTP_ERROR;

    pos = src;
    for (; (c = *pos) && !start; pos++)
        {
        if (c == ' ' || c == '\t')
            ;
        else if (c == '\"')
            start = 1;
        else if (c == 'W' && *(++pos) == '/')
            /*weak = 1;*/
            ;
        else
            return HTTP_ERROR;
        }

    if (!start)
        return HTTP_ERROR;

    for (i = 0; i < 3; i++)
        {
        for (j = 0; (c = *pos) && !end; j++, pos++)
            {
/*             if (j > sizeof(unsigned long)*2 ) */
/*                 return HTTP_ERROR; */

            if ('0' <= c && c <= '9')
                {
                c =(char)(c - '0');
                temp[i] <<= 4; 
                temp[i] |= c;
                }
            else if ('a' <= c && c <= 'f')
                {
                c =(char)(c - 'a');
                temp[i] <<= 4; 
                temp[i] |= c+10;
                }
            else if ('A' <= c && c <= 'F')
                {
                c = (char)(c - 'A');
                temp[i] <<= 4; 
                temp[i] |= c+10;
                }
            else if (c == '-')
                {
                pos++;
                break;
                }
            else if (c == '\"')
                {
/*                 if (i < 1) */
/*                     return HTTP_ERROR; */
                end = 1;
                break;
                }
            else
                {
                return HTTP_ERROR;
                }
            }

/*         if (j == 0) */
/*             return HTTP_ERROR; */
        }

    if (!end)
        return HTTP_ERROR;

    dst->file = temp[0];
    dst->size = temp[1];
    dst->time = temp[2];

    return HTTP_OK;
    }
#endif /* HTTP_ETAG_ENABLE */

