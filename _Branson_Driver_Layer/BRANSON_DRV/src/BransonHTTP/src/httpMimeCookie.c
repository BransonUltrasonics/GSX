/* httpMimeCookie.c - parse functions for MIME header */

/* Copyright (c) 1996-2011 2013-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
11jun15,r_w  fix static analysis issue V7MAN-154.
21feb13,r_w  fix static analysis issue(defect WIND00404092)
30mar11,r_w  fix defect WIND00257291 make API reference readable
27jan10,h_y  support for LP64
13feb07,msa  Fix comments to remove APIgen errors.
18aug03,jws  remove HTTP_BOOL
15jan99,ck   written.
*/

/*
DESCRIPTION
This module contains especially HTTP_REQ access functions which
are useful for server cookie handling.

INCLUDE FILES: http/private/httpReq.h
               http/httpSem.h
               http/httpLib.h
           http/private/httpInt.h

*/

/* includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "httpLib.h"
#include "private/httpInt.h"
#include "httpReq.h"

HTTP_STATUS httpMimeReadCookie(HTTP_REQ_ID reqId, char * szLine );
/*******************************************************************************
*
* httpMimeReadCookie - parse the "Cookie:" header field
*
* This routine parse the "Cookie:" header field. It is invoked by the MIME
* parser to extract the "Cookie:" header from the client's HTTP request.
* The MIME parser will only call this routine if "Cookie:" was found in
* a request header.
* See "httpconfig.c" to learn how MIME parsing should be configured or
* manipulated.
*
* NOTE:
* Do not change <szLine>.
*
* RETURNS: HTTP_OK if set operation was successful, HTTP_ERROR otherwise.
*/

HTTP_STATUS httpMimeReadCookie
    (
    HTTP_REQ_ID reqId,      /* Handle of the active request */
    char * szLine           /* line to parse                */
    )
    {
    char * szCurrent;
    char * szEnd;
    char * szEquals;
    char * szName;
    char * szValue;
    char * szTmp;
    char cStore;

    szCurrent = szLine;
    szEnd     = szLine + strlen (szLine);

    logInfoMsg ("Process COOKIE: <%s>", szLine);

    while (szCurrent < szEnd)
    {
    if ((reqId->recCookieListId == NULL) ||
        (httpGenListItemsGet (reqId->recCookieListId) >= httpMaxRecCookies))
        {
        logInfoMsg ("No memory slot available to store cookie: %s", szLine);
        break;
        }

    /* find the first equals sign  */
    szEquals = strchr (szCurrent, '=');
    if (szEquals == NULL)
        break;
    /* hmm... well, the name is left of it until a space or semicolon is found */
    for (szTmp = szEquals; (szTmp > szCurrent) && (*(szTmp-1) != ' ') && (*(szTmp-1) != ';'); szTmp--);



    *szEquals = (char)0;   /* temporary terminate the name and duplicate it*/
    szName = httpStrdup (reqId->memPool, szTmp);           /* store it! */
    *szEquals = '=';  /* restore original condition */

    if (szName == NULL)
        {
        logInfoMsg ("out of memory storing Cookie-Name >%s<", szTmp);
        break;
        }

    /* .... and the value is left */
    szEquals ++;
    for (szTmp = szEquals; szTmp < szEnd && *szTmp != ';'; szTmp ++);

    if ((cStore = *szTmp) != (char)0)    /* if not at end, terminate temporary */
        *szTmp = (char)0;
    szValue = httpStrdup (reqId->memPool, szEquals);                     /* store it! */
    *szTmp = cStore;                      /* restore original condition */
    if (szValue == NULL)
        {
        logInfoMsg ("out of memory munching Cookie value >%s<", szEquals);
        break;
        }

    /* Now, we have full cookie name and value -> store it in a cool place */

    if (httpGenListDataAppend (reqId->recCookieListId, szName, szValue) == NULL)
        {
        logInfo ("No memory to store cookie in list");
        break;
        }

    logInfo ("Cookie successfully stored!");
    szCurrent = szTmp;
    }

    return (HTTP_OK);
    }


/*******************************************************************************
*
* httpMimeOutCookieSet - set the "Cookie:" header field
*
* This routine sets the "Cookie:" header field. The header field is sent
* to the client upon completing the response header of the result.
*
* The cookies which are sent conform to RFC 2109.
*
* If the NULL pointer is passed for the strings (except the name),
* the value will be omitted and the client should apply defaults.
*
* RETURNS: HTTP_OK if set operation was successful, HTTP_ERROR otherwise.
*/

HTTP_STATUS httpMimeOutCookieSet
    (
    HTTP_REQ_ID reqId,       /* Handle of the active request */
    char * szName,           /* Cookie name                  */
    char * szValue,          /* Cookie value                 */
    char * szPath,           /* Cookie path                  */
    char * szDomain,         /* Cookie domain                */
    char * szComment,        /* User Comment                 */
    long   lMaxAge,          /* Maximum age of cookie in sec */
    BOOL   fSecure           /* Secure Cookie                */
    )
    {
    unsigned long ulLength;
    char * szCookie;

    if (szName == NULL || reqId->sendCookieListId == NULL)
    return (HTTP_ERROR);

    /* determine length of memory to allocate... including keywords if needed */

    ulLength = ((szValue  == NULL) ? 0 : strlen (szValue) + 4) +
    ((szPath   == NULL) ? 0 : strlen (szPath) + 4 + 5) +
    ((szComment== NULL) ? 0 : strlen (szComment) + 4 + 10) +
    ((szDomain == NULL) ? 0 : strlen (szDomain) +4 + 11) +
    ((fSecure)          ? 8:0) +
    ((lMaxAge  == 0)    ? 0 : 20) + 4 + 15;

    if ((szCookie = memPoolAlloc (reqId->memPool, ulLength)) == NULL)
    {
    logInfo ("Out of memory assembling cookie");
    return (HTTP_ERROR);
    }

    if (szValue  == NULL)
    *szCookie = (char)0;
    else
    (void)snprintf (szCookie, (ulLength-1), "%s",szValue);

    if (szPath   != NULL)
    {
    strncat (szCookie, "; PATH=",8);
    strncat (szCookie, szPath,strlen(szPath));
    }

    if (szComment  != NULL)
    {
    strncat (szCookie, "; COMMENT=",11);
    strncat (szCookie, szComment,strlen(szComment));
    }

    if (szDomain   != NULL)
    {
    strncat (szCookie, "; DOMAIN_NAME=",15);
    strncat (szCookie, szDomain,strlen(szDomain));
    }

    if (lMaxAge    != 0)
    {
    (void)snprintf (szCookie + strlen (szCookie), (ulLength - strlen (szCookie)-1), "; MAXAGE=%ld", lMaxAge);
    }

    if (fSecure)
    {
    strncat (szCookie, "; secure",9);
    if (NULL != szPath)
	    strncat (szCookie, szPath,strlen(szPath));
    }

    strncat (szCookie, "; VERSION=1",12);

    if (httpGenListDataAppend (reqId->sendCookieListId, szName, szCookie) == NULL)
    {
    logInfoMsg ("FAILED adding cookie: %s", szCookie);
    return (HTTP_ERROR);
    }

    logInfoMsg ("Added cookie: >%s<", szCookie);
    return (HTTP_OK);
    }


/*******************************************************************************
*
* httpMimeInCookieGet - get the "Cookie:" header field
*
* This routine gets the "Cookie:" value from the header field sent from the
* client.
*
* RETURNS: Cookie value as a string,  or NULL if no value is received.
*/

char * httpMimeInCookieGet
    (
    HTTP_REQ_ID reqId,      /* Handle of the active request */
    char * szName           /* Cookie name                  */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID listItem;

    if (szName == NULL || reqId->recCookieListId == NULL)
    return (NULL);

    listItem = httpGenListCaseFind (reqId->recCookieListId, szName);

    if (listItem == NULL)
    {
    logInfoMsg ("Cookie <%s> not found", szName);
    return (NULL);
    }

    return  (httpGenListDataGet (listItem));
    }


/*******************************************************************************
*
* httpMimeInCookieIterate - iterate all incoming cookies
*
* Get a cookie name, which was transmitted from the client with
* the request header.
*
* If the index is 0, httpGenListIndexIterator() will return the
* first entry id of the specified list.
* If the index exceeds the index of the last entry in the list, it will not
* be incremented any more, and httpGenListIndexIterator() returns NULL.
*
* RETURNS: Cookie name as a string, or NULL if end of list.
*
* SEE ALSO: httpGenListIndexIterator()
*/

char * httpMimeInCookieIterate
    (
    HTTP_REQ_ID      reqId,      /* Handle of the active request */
    unsigned short * pusIndex    /* Pointer to current cookie    */
                                 /* is zero for the first call.  */
    )
    {
    return  (httpGenListIdGet
         (httpGenListIndexIterator
          (reqId->recCookieListId, pusIndex)
         )
            );
    }

