/* httpLineUnescape.c - Line unescaping functions for the Web server. */

/* Copyright 1999-2007 Wind River Systems, Inc. */

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
02g,15apr10,h_y  Web Server might fail in basic authentication by some user name and password.(WIND00207102)
02f,08nov04,gsr  extra check for '+' in boundary conditions SPR#102869
02e,18jun03,jws  remove assert.h include
02d,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02c,13may97,mb   osal introduced.
02b,21oct96,mb   bugfix (NULL pointer may be a valid argument now).
02a,30sep96,ck   splitt off from httputil.
01a,15sep96,ck   initial creation.
*/

/*
DESCRIPTION
This module provides unescaping utility functions.

INCLUDE FILES:
    httpType.h
    httpSem.h
    httpLib.h

*/

#include "wm.h"

#include "ctype.h"
#include <string.h>
#include "stdlib.h"

#include "httpType.h"
#include "httpSem.h"
#include "httpLib.h"


/****************************************************************************
*
* httpLineUnescape - Convert escaped characters in buffer to ASCII.
*
* This routine can be used to convert an "escaped" form of a URL or
* parameter (appended to a URL) to standard ASCII format.
* The escaping is done by the browser on the client side, for
* transferring characters not allowed by the HTTP protocol.
* For example, a whitespace character is not allowed in an URL.
* It must be substituted by an escape sequence to be transferred.
*
* ESCAPING
* When you want to include any character, not part of the standard
* set allowed in URLs, you can do this by specifying its hex value
* in the format %xx, where xx is the hex representation.
* In addition, every '+' character will be substituted by a space.
*
* NOTE
* Usually, with the Wind River Web Server, you need not be concerned with
* unescaping arguments, because the server handles this for you.  This
* routine is only supplied for cases in which you want to explicitly
* unescape some strings in the body of a CGI-post request body,
* for example:
* \cs
*         "test+size" -> "test size"
*         "demo+%64"  -> "test d"
* \ce
* RETURNS: HTTP_OK or HTTP_ERROR.
*
* SEE ALSO: httpLineEscape()
*/

HTTP_STATUS httpLineUnescape
    (
    char * szString  /* escaped string to unescape */
    )
    {
    unsigned long ulLength;
    unsigned long i = 0;
    char          szBuffer[3];
    char          cNewChar;
    char*         pBoundary;
    char*         pAuthorization;

    if (szString == NULL)
    return (HTTP_OK);

    ulLength = strlen (szString);
    pBoundary = strstr( szString, "boundary=" );
    pAuthorization = strstr( szString, "Authorization:" );

    while (i < ulLength)
        {
        if (szString[i] == '+' && (pBoundary == NULL) )
        	if(pAuthorization == NULL)
        	{
        		szString[i] = ' ';                    /* replace '+' by spaces */
        	}
            
        if ((szString[i] == '%') && (i < ulLength-2))
            if (isxdigit ((int) szString[i+1]) && isxdigit ((int) szString[i+2]))
                {
                strncpy (szBuffer, &(szString[i+1]), 2);
                szBuffer[2] = 0;
                cNewChar = (char) strtol (szBuffer, NULL, 16);
                if (cNewChar != 0)
                    {
                    httpStrCpy (&(szString [i]),  /* move string 2 chars   */
                                &(szString[i+2]));
                    szString[i] = cNewChar;       /* replace % by new char */
                    ulLength -= 2;
                    }
                }
        i++;
        }
    return (HTTP_OK);
    }

