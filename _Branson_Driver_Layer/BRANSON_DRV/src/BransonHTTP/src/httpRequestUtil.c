/* httpRequestUtil.c - access functions for internal data of http server */

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
02f,26jan10,h_y  support for LP64
02e,12jan04,dkg  added API function httpAbsPathGet() to allow system
                 RPMs to read in the absolute path, for SPR 92564
02d,24sep03,jws  fix compiler warnings, SPR 90843
02c,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02b,13may97,mb   osal introduced.
02a,30sep96,ck   several routines split off into seperate files.
01b,12sep96,pk   beautified headers.
01a,09jun96,mb   written.
*/

/*
DESCRIPTION
This module contains small access functions which depend on special
structures of the HTTP server, esp the HTTP_REQ type.

INCLUDE FILES: http/private/httpReq.h
               http/httpLib.h

NOMANUAL
*/

#include <string.h>

#include "httpLib.h"
#include "httpReq.h"

/*******************************************************************************
*
* httpRpmIdentAndDataSet - set the URL path fragments that identify the RPM
*
* This routine sets the first part of the absolute path of the URL,
* which identifies the invoked RPM (the RPM identifier),
* and the last part of the absolute path of the URL,
* which should be an argument to the RPM.
*
* NOTE: The strings are not copied, only the pointer is stored.
*
* RETURNS: N/A
*
* SEE ALSO: httpRpmIdentGet(), httpRpmDataGet()
*/

void httpRpmIdentAndDataSet
    (
    HTTP_REQ_ID reqId,        /* Handle of the active request */
    char *      szNewIdent,   /* New RPM identifier string    */
    char *      szNewData     /* New RPM data string          */
    )
    {
    reqId->szRpmIdent = szNewIdent;
    reqId->szRpmData  = szNewData;
    }


/*******************************************************************************
*
* httpRpmIdentGet - get the URL path fragment that identifies the RPM
*
* This routine gets the first part of the absolute path of the URL, which
* identified the invoked RPM.
*
* RETURNS: A pointer to the RPM name.
*
* SEE ALSO: httpRpmDataGet(), httpRpmIdentAndDataSet()
*/

char * httpRpmIdentGet
    (
    HTTP_REQ_ID reqId      /* Handle of the active request */
    )
    {
    return (reqId->szRpmIdent);
    }

#ifndef WEBCLI_STANDALONE_WEBSERVER
/*******************************************************************************
*
* httpWmbSessionGet - get the current WMB session
*
* This routing returns a pointer to the currently active Backplane session
*
* RETURNS: A pointer to the WMB session
*
*/

WMB_SESSION_T * httpWmbSessionGet
    (
    HTTP_REQ_ID reqId      /* Handle of the active request */
    )
    {
    return (reqId->pSession);
    }
#endif

/*******************************************************************************
*
* httpRpmDataGet - get the URL path fragment following the RPM identifier
*
* This routine gets the last part of the absolute path of the URL, which should
* be an argument to the RPM identified with the first part of the
* absolute path.
*
* RETURNS: A pointer to the RPM path argument.
*
* SEE ALSO: httpRpmIdentGet(), httpRpmIdentAndDataSet()
*/

char * httpRpmDataGet
    (
    HTTP_REQ_ID reqId      /* Handle of the active request */
    )
    {
    return (reqId->szRpmData);
    }


/*******************************************************************************
*
* httpStatusSet - set the HTTP status code for the response to the client
*
* This routine sets the HTTP status code for the response to the client.
* This routine is neccessary if httpError() will be invoked to specify the
* error code and the standard short description of the error.
*
* For conformance with the HTTP protocol, use one of the following:
*
* .TS
* tab(|);
* lf3 lf3
* l n.
* Message               | Number
* _
* HTTP_CONTINUE         | 100
* HTTP_SWITCH_PROT      | 101
* HTTP_OK               | 200
* HTTP_CREATED          | 201
* HTTP_ACCEPTED         | 202
* HTTP_NON_AUTH_INF     | 203
* HTTP_NO_CONTENT       | 204
* HTTP_RESET_CONTENT    | 205
* HTTP_PART_CONTENT     | 206
* HTTP_MULT_CHOICE      | 300
* HTTP_MOVED_PERM       | 301
* HTTP_MOVED_TEMP       | 302
* HTTP_SEE_OTHER        | 303
* HTTP_NOT_MODIFIED     | 304
* HTTP_USE_PROXY        | 305
* HTTP_BAD_REQUEST      | 400
* HTTP_UNAUTHORIZED     | 401
* HTTP_PAYMENT_REQ      | 402
* HTTP_FORBIDDEN        | 403
* HTTP_NOT_FOUND        | 404
* HTTP_METHOD_NA        | 405
* HTTP_NOT_ACCEPTED     | 406
* HTTP_PROXY_AUTH_REQ   | 407
* HTTP_REQ_TIME_OUT     | 408
* HTTP_CONFLICT         | 409
* HTTP_GONE             | 410
* HTTP_LENGTH_REQIRED   | 411
* HTTP_PRECOND_FAIL     | 412
* HTTP_ENTITY_TOO_LARGE | 413
* HTTP_URI_TOO_LARGE    | 414
* HTTP_UNSUP_MEDIA_TYPE | 415
* HTTP_INTERNAL_ERROR   | 500
* HTTP_NOT_IMPLEMENTED  | 501
* HTTP_BAD_GATEWAY      | 502
* HTTP_SERV_UNAVAILABLE | 503
* HTTP_GATEWAY_TIMEOUT  | 504
* HTTP_VERSION_NOT_SUPP | 505
* .TE
*
* For the mapping from those numbers to status messages, see the
* httpStatusMessages[] table in the file httpconfig.c.
*
* RETURNS: N/A
*
* SEE ALSO: httpError(), httpHeaderGenerate()
*/

void httpStatusSet
    (
    HTTP_REQ_ID reqId,  /* Handle of the active request */
    short sStatus       /* HTTP status                  */
    )
    {
    reqId->sStatus = sStatus;
    }


/*******************************************************************************
*
* httpBufferGet - get the pointer to the internal temporary buffer
*
* This routine provides access to the internal buffer for temporary use.
* Its maximum size is `HTTP_MAX_REQLINE_SIZE', which is specified in httpconf.h
* It is intended to be used as an internal scratch buffer.
*
* RETURNS: A pointer to a temporary buffer with length `HTTP_MAX_REQLINE_SIZE'
*          or NULL if the buffer was not allocated.
*/

char * httpBufferGet
    (
    HTTP_REQ_ID  reqId   /* Handle of the active request */
    )
    {
    return (reqId->szBuffer);
    }

/*******************************************************************************
*
* httpWmbErrorIterator - iterate through the WMB access error message if any
*
* This function should be used by httpPostReply to retrieve any WMB access
* errors that occurred during a post.
* This routine is called with an HTTP_GEN_LIST_ID, <listId>, and the last
* returned HTTP_GEN_LIST_ENTRY_ID or NULL. If the <pEntryId> is not NULL, then
* the next HTTP_GEN_LIST_ENTRY_ID in the list is returned, and the pointers
* pszFailedWindMark and pszErrorMsg are updated to point to the current error.
*
* On a subsequent call, with the return of this routine, the next
* HTTP_GEN_LIST_ENTRY_ID will be returned.
*
* EXAMPLE:
*    HTTP_GEN_LIST_ID entryId = NULL;
*    int i = 0;
*
*    while (NULL != errorId = httpWmbErrorIterator(someListId, errorId, &pWM, &pErr))
*        {
*        printf ("Setting WindMark:%s returned error %s), &pWM, &pErr);
*        }
*
* RETURNS: the lstEntry that should be used for the next iteration
*          a pointer to the WindMark name that failed           (by reference)
*          a pointer to the error message generated by the user (by reference)
*/
HTTP_GEN_LIST_ENTRY_ID httpWmbErrorIterator
    (
    HTTP_REQ_ID  reqId,                /* Handle of the active request        */
    HTTP_GEN_LIST_ENTRY_ID lstEntry,   /* If NULL, start from first error     */
    sbyte ** pszFailedWindMark,        /* On return it will point to failed WM*/
    sbyte ** pszErrorMsg               /* On return it will point to error msg*/
    )
    {

    if (NULL == reqId || NULL == pszFailedWindMark || NULL == pszErrorMsg)
        return NULL;

    if (NULL != httpGenListIterator(reqId->wmbPostErrListId, &lstEntry))
        {
        (*pszFailedWindMark) = httpGenListIdGet (lstEntry);
        (*pszErrorMsg) = httpGenListDataGet (lstEntry);
        } /* while */
    return (lstEntry);
    }

/*******************************************************************************
*
* httpWmbErrorsExist - find out if there were WMB access errors during post
*
* This function should be used by httpPostReply.  If TRUE, httpWmbErrorIterator
* should be used to retrieve them.
*
* RETURNS: TRUE if there are errors waiting to be read
*/

BOOL httpWmbErrorsExist
    (
    HTTP_REQ_ID  reqId                /* Handle of the active request        */
    )
    {
    return (0 != httpGenListItemsGet(reqId->wmbPostErrListId));
    }

/*******************************************************************************
*
* httpWmbErrorStore - store a meaningful error message for a WindMark
*
* This function can be used from httpWindMarkSetFailed to store an error message
* in the server's environment.  The message will be available to httpPostReply.
*
* Only a pointer to the provided strings is stored.  The application must ensure
* that the strings are not deallocated before the errors are retrieved by means
* of httpWmbErrorIterator
*
* RETURNS: non NULL if successful
*/

HTTP_GEN_LIST_ENTRY_ID httpWmbErrorStore
    (
    HTTP_REQ_ID  reqId,              /* Handle of the active request        */
    sbyte * szFailedWM,              /* Name of failed WindMark             */
    sbyte * szErrorMsg               /* Error message                       */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID entry;

    if (szFailedWM == NULL)  return NULL;

    if ( (entry = httpGenListFind (reqId->wmbPostErrListId,szFailedWM)) != NULL)
    {
    logInfoMsg("httpWmbErrorStore: replacing error for WindMark %s",szFailedWM);
    httpGenListRemove ( reqId->wmbPostErrListId , entry );
    }

    if (NULL != szErrorMsg)
    {
    logInfoMsg ( "httpWmbErrorStore: setting error msg for WM %s",szFailedWM );
    if (( entry = httpGenListDataAppend ( reqId->wmbPostErrListId ,
                                          szFailedWM , szErrorMsg )) != NULL)
        {
        logInfoMsg ("httpWmbErrorStore: msg >%s< set successfully",szErrorMsg);
        return entry;
        }
    }
    logInfoMsg ( "httpWmbErrorStore: couldn't set error msg for %s!",szFailedWM );
    return (NULL);
    }

/*******************************************************************************
*
* httpAbsPathGet - get the URL absolute path
*
* This routine gets the absolute path of the URL, for the given http request ID.
*
* RETURNS: A pointer to the URL absolute path
*
* ERRNO: N/A
*/

char * httpAbsPathGet
    (
    HTTP_REQ_ID reqId  /* Handle of the active request */
    ) 
    {
    return (reqId->szAbsPath); 
    }
