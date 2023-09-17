/* httpParseUtils.c - Utilities for parsing argument strings */

/*
 * Copyright (c) 1996-2008,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03a,26jan10,h_y  support for LP64
02i,28aug08,q_z  change the some function return declared  (WIND00033655)
02h,08aug03,jws  HTTP_BOOL -> BOOL
02g,17jan00,csr  updating API documentation for httpSpecialCharEscape;
                 also added a check for buffer overflow in the same routine;
                 changed httpSpecialCharEscape to httpSpecialCharEscapePrint:
                 now the routine calls httpPrintf() and avoids any problem of
                 string truncation.
02f,02dec99,csr  fixing SPR # 21704 (Wind Web Server does not properly escape
                 text such as quote, lessthan, greaterthan and ampersand for
                 html form processing.)
02e,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02e,10dec98,mb   bugfix: httpTagValueGet() several boolean conditions did not
                 compare to HTTP_TRUE/HTTP_FALSE.
02e,10dec98,mb   bugfix: httpTagValueGet() set string termination byte behind
                 buffer limits.
02d,21apr98,mb   bugfix: httpTagValueGet() did not restrict strings to
                 lBufferSize.
02c,13may97,mb   osal introduced.
01a,28oct96,ck   written.
*/

/*
DESCRIPTION
This module provides utilities for parsing argument strings.

INCLUDE FILES:
    httpLib.h


*/

#include <string.h>
#include <ctype.h>

#include "httpLib.h"

/**************************************************************************
*
* httpSpecialCharEscapePrint - Escapes special HTML chars: '"', '&', '<', '>'
*
*    This routine takes a character string, and loops through the entire
*    string printing all characters with httpPrintf() and replaces special
*    characters with the required HTML representations ("&quot;", "&amp;",
*    etc.).  If no special characters are found, all characters are printed
*    without replacements.
*
* RETURNS
*    HTTP_OK if httpPrintf() does not fail or HTTP_ERROR otherwise.
*
* NOTE
*    This routine performs extra processing that is not required unless it is
*    known that a string contains special characters.  It is used by
*    the TextArea, Textfield, and Hidden form processing utilities.
*/

HTTP_STATUS httpSpecialCharEscapePrint
    (
    HTTP_REQ_ID  reqId,     /* current request structure       */
    char *       szArg     /* string to print                 */
    )
    {
    unsigned int iLoop;     /* temporary index   */
    char         szTmp[2];  /* temporary buffer  */

    if ((szArg == NULL) || (reqId == NULL)) return HTTP_ERROR;

    /* initialize the temporary buffer */

    szTmp[0] = '\0';
    szTmp[1] = '\0';

    for (iLoop=0; iLoop < strlen(szArg); iLoop++)
        {

        /* for each special character, print the appropriate string */

        switch(szArg[iLoop])
            {
            case '\"':
        if (httpPrintf (reqId, "&quot;") == HTTP_ERROR)
            return HTTP_ERROR;
                break;
            case '<':
        if (httpPrintf (reqId, "&lt;") == HTTP_ERROR)
            return HTTP_ERROR;
                break;
            case '>':
        if (httpPrintf (reqId, "&gt;") == HTTP_ERROR)
            return HTTP_ERROR;
                break;
            case '&':
        if (httpPrintf (reqId, "&amp;") == HTTP_ERROR)
            return HTTP_ERROR;
                break;
            default:
                szTmp[0] = szArg[iLoop];
        if (httpPrintf (reqId, szTmp) == HTTP_ERROR)
            return HTTP_ERROR;
                break;
            }
        }

    return HTTP_OK;
    }

/**************************************************************************
*
* httpTagValueGet - Get the value of an argument tag.
*
* This routine parses an argument string and extracts the assigned values of
* an option. It copies the result to the specified buffer, or generates an
* empty string if no matching value is found.
*
* The separator is a space character, assignment is made with the equal
* sign (`='). Double quote characters (`"') are used
* to include strings that contain space or `=' characters.
*
* RETURNS: HTTP_OK, or HTTP_ERROR if parsing failed.
*/

HTTP_STATUS httpTagValueGet
    (
    char * szSource,                /* String, where to look for the token */
    const  char * const szToken,     /* Token to search                     */
    char * szBuffer,                /* Buffer to hold the result           */
    long   lBufferSize              /* Maximum size of buffer              */
    )
    {
    BOOL   fInString = FALSE;
    BOOL   fSeperate = TRUE;
    BOOL   fAssignment;
    size_t   TokenLen;
    char * szEnd;
    char * szIndex;
    char * szTmp;

    if ((szSource == NULL) || (szToken == NULL) ||
        (szBuffer == NULL) || (lBufferSize < 2))
        {
        logError ("httpTagValueGet: invalid arguments!");
        return (HTTP_ERROR);
        }

    TokenLen  = strlen (szToken);
    szEnd     = &szSource[strlen (szSource)];

    *szBuffer = 0;              /* ensure at least an empty return string */

    for (szIndex = szSource; szIndex < szEnd; szIndex++)
        {
        if (*szIndex == '"')
            fInString = fInString == TRUE ? FALSE : TRUE;
        else if (isspace ((int) *szIndex))
            fSeperate = TRUE;
        else
            {
            if ((!fInString) && (fSeperate) &&
                (httpStrNCaseCmp (szIndex, szToken, TokenLen) == 0) &&
                (isspace ((int) szIndex[TokenLen]) ||
                (szIndex[TokenLen] == '=')))
                {

                /*
                 * Now, szIndex points at the beginning of the searched
                 * Token, find the value
                 */

                for (fAssignment = FALSE; ((szIndex < szEnd) &&
                     !((*szIndex == ',') || isspace ((int) *szIndex) ||
                     (fAssignment==HTTP_TRUE))); szIndex++)
                    if (*szIndex == '=')
                        fAssignment = TRUE;

        while ( ( szIndex < szEnd ) && isspace ( (int) *szIndex ) )
            szIndex++;

                if ((szIndex < szEnd) && (fAssignment))
                    {

                    /*
                     * szIndex points at the beginning of the value,
                     * find where it ends!
                     */

            szTmp = szIndex;
                    while ( (szTmp < szEnd) && ((fInString) ||
                           !isspace ((int) *szTmp )) )
            {
            if ( *szTmp == '"' )
                fInString = fInString == TRUE ? FALSE : TRUE;
            szTmp++;
            }
            szEnd = szTmp;

            /*
             * decide if tag value is a comma separated list of values
             * or just a single value
             */

            szTmp = szIndex;
            if ( ( szTmp[0] == '"' ) && (szEnd[-1] == '"' ) )
            {
            fInString=TRUE;
            while ( ( ++szTmp < szEnd-1 ) && ( fInString==TRUE ) )
                {
                if ((szTmp[0] == '"') && (szTmp[1] == ',') &&
                    (szTmp[2] == '"') )
                {

                /*
                 * there are several values specified in comma separated
                 * list, keep quotes
                 */

                fInString = FALSE;
                }
                }
            if ( fInString )
                {

                /*
                 * tag value is a single item, not a list, so strip off
                 * quotes
                 */

                szIndex++;
                szEnd--;
                }
            }

            /* cut off strings longer than specified buffer size */
            if ( (szEnd - szIndex ) >= lBufferSize )
            return HTTP_ERROR;   /* buffer too small */
            lBufferSize = szEnd - szIndex;
                    strncpy (szBuffer, szIndex, (size_t) lBufferSize);
                    szBuffer [lBufferSize] = 0;
                    fInString = FALSE;
                    break;
                    }
                }
            fSeperate = FALSE;
            }
        }

    if (fInString)
        return (HTTP_ERROR);
    else
        return (HTTP_OK);
    }


/**************************************************************************
*
* httpTagValueIterate - Evaluates the values of an argument tag.
*
* This routine evaluates the values from a tag, obtained with httpTagValueGet()
* For the first call, <pszNext> must be a pointer to a NULL pointer.
* For all subsequent calls, this value must be passed on to the next call
* (as with strtok_r()). The separator is a comma (',').
*
* NOTE: The contents of <szSource> will be modified.
*
* RETURNS: A pointer to zero terminated value string or NULL for end.
*/

char * httpTagValueIterate
    (
    char *  szSource,
    char ** pszNext
    )
    {
    char * cpBegin;
    char * cpEnd;
    BOOL   fInString;

    if (*pszNext == NULL)     /* first call to function */
        {
        cpEnd = szSource + strlen(szSource);
        *pszNext = szSource;
        while  (*pszNext < cpEnd && (isspace ((int) **pszNext) ||
                **pszNext == '"'))
            if (**pszNext == '"')
                break;
            else
                (*pszNext)++;
        }
    else                    /* all further calls */
        {
        if (*pszNext < szSource)
            {
            return (NULL);
            }
        cpEnd = *pszNext + strlen(*pszNext);
        if (*pszNext >= cpEnd)
            return (NULL);
        *(*pszNext - 1) = ',';
        }

    /* pszNext now points at the beginning of the current output string */

    cpBegin   = *pszNext;
    fInString = FALSE;
    while  (*pszNext < cpEnd && (**pszNext!=',' || fInString))
        {
        if (**pszNext == '"')
            fInString = !fInString;
        (*pszNext)++;
        }
    if (*pszNext < cpEnd)
        {
        **pszNext = 0;
        (*pszNext)++;
        }
    return (cpBegin);
    }


