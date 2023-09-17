/* httpRpmJsPost.c - Demangle form elements mangled by the js() WindMark */

/* Copyright 2003-2004,2010 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01d,02feb10,h_y   support for LP64 
01c,22feb07,ps  apigen reformatting
01b,19feb04,jws  copyright added
01a,15jan03,jc   initial creation.
*/

/*
DESCRIPTION

This module looks for mangled form elements (i.e. prefixed with __js_) in the
server's environment.  If one is found, it will be demangled so the following
RPM (which should be httpRpmWmbPost) finds valid WindMarks instead.  See
function jsMangleHandle, in httpWmbHandlers.c to see the mangling format.

INCLUDE FILES:

*/


#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "httpLib.h"
#include "httpRpms.h"
#include "httpReq.h"
#include "private/httpInt.h"

/* constants */
#define  JS_PREFIX_FIRST_CHAR '_'
#define  JS_PREFIX  "__js_"

/* locals */

LOCAL void jsDemangle (HTTP_REQ_ID reqId, HTTP_GEN_LIST_ENTRY_ID lstEntry);

/*******************************************************************************
*
* httpRpmJsPost - process mangled form elements processed to the server
*
* RETURNS: RPM_OK (since another RPM (httpRpmWmbPost) must finish processing
*                  this request)
*/

short httpRpmJsPost
    (
    HTTP_REQ_ID reqId   /* Handle of the active request, needed for all */
                        /* http - API function calls                    */
    )
    {

    sbyte   *pElemName, *pVal;
    sbyte*  pInst = NULL;
    sbyte*  pStrTokState;
    sbyte*  pArgMagicMarkup;
    sbyte4  instLen = 0;
    WMB_OBJ_T obj;

    HTTP_GEN_LIST_ENTRY_ID lstEntry = NULL;

    if (NULL == reqId || NULL == reqId->argListId)
        return wmError (ERROR_GENERAL, "null pointer!");

    if (NULL == (lstEntry = httpGenListFind (reqId->argListId, "END_OF_HEADERS")))
        return wmError (ERROR_GENERAL, "corrupted environment");

    while (NULL != httpGenListIterator(reqId->argListId, &lstEntry))
        {
        pElemName = httpGenListIdGet (lstEntry);

        /* check for JS_PREFIX */

        if (JS_PREFIX_FIRST_CHAR == *pElemName)
            {
            pVal = strstr (pElemName, JS_PREFIX);
            if (pVal == pElemName)
                jsDemangle(reqId, lstEntry);
            }
        }

    return RPM_OK;
    }

/*******************************************************************************
*
* jsDemangle - inplace demangling of WindMark names processed by jsMangleHandle
*
* NOMANUAL
*/


LOCAL void jsDemangle
    (
    HTTP_REQ_ID            reqId,
    HTTP_GEN_LIST_ENTRY_ID lstEntry
    )
    {
    sbyte *      pElemName;
    sbyte *      pBuffer;
    sbyte *      pWmName;
    sbyte *      pWmPrefix;
    sbyte *      pWmSuffix;
    sbyte4       elemLen;

    /* pElem contains a pointer to the list entry's key, not a copy! */

    pElemName = httpGenListIdGet (lstEntry);
    elemLen = strlen (pElemName);
    pBuffer = httpBufferGet (reqId);

    /* httpReqMemPartSize is a global that holds the size of the servr buffer */

    if (httpReqMemPartSize < elemLen)
        return;

    strcpy (pBuffer, pElemName);

    /* skip prefix */

    pBuffer = pBuffer + strlen (JS_PREFIX);

    pWmPrefix = STRTOK_REENTRANT (pBuffer, "_", &pWmSuffix);
    pWmName = STRTOK_REENTRANT (NULL, "_", &pWmSuffix);

    if (NULL == pWmName)      /* case __js_wmname */
        {
        strcpy (pElemName, pBuffer);
        return;
        }

    if (isdigit (*pWmName))   /* case __js_wmname_1_2_3_4 */
        {
        pBuffer = pWmName;
        if (NULL != pWmSuffix) (*--pWmSuffix)='_';     /* undo last strtok \0 */
        while (NULL != (pBuffer = strchr(pBuffer, '_')))
            (*pBuffer) = '.';
        strcpy (pElemName, pWmPrefix);
        strcat (pElemName, ".");
        strcat (pElemName, pWmName);
        return;
        }

    if (NULL == pWmSuffix)     /* case __js_prefix_wmname */
        {
        strcpy (pElemName, pWmPrefix);
        strcat (pElemName, ":");
        strcat (pElemName, pWmName);
        return;
        }

    if (isdigit (*pWmSuffix))  /* case __js_prefix_wmname_1_2_3_4 (typical) */
        {
        pBuffer = pWmSuffix;
        while (NULL != (pBuffer = strchr(pBuffer, '_')))
            (*pBuffer) = '.';
        strcpy (pElemName, pWmPrefix);
        strcat (pElemName, ":");
        strcat (pElemName, pWmName);
        strcat (pElemName, ".");
        strcat (pElemName, pWmSuffix);
        return;
        }
    }


