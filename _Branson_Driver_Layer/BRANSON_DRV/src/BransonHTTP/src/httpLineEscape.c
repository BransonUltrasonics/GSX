/* httpLineEscape.c - escaping utility functions for http server */

/* Copyright (c) 1999-2003,2010,2015 Wind River Systems, Inc. */

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
11jun15,r_w  fix static analysis issue V7MAN-154.
02feb10,h_y   support for LP64 
18jun03,jws  remove assert.h include
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
13may97,mb   osal introduced.
24oct96,ck   initial creation.
*/

/*
DESCRIPTION
This module provides escaping utility functions.

INCLUDE FILES: http/httpType.h
               http/httpSem.h
               http/httpLib.h

NOMANUAL
*/

#include "wm.h"

#include "ctype.h"
#include <string.h>
#include "stdlib.h"
#include <stdio.h>

#include "httpType.h"
#include "httpSem.h"
#include "httpLib.h"


/*******************************************************************************
*
* httpLineEscape - convert a string to escaped characters in buffer
*
* This routine converts a plain string to its "escaped" form,
* consisting only of printable ASCII characters.
*
* When you want to include any character that is not part of the standard
* set allowed in URLs, you can do so by specifying its hex value
* in the format %xx, where xx is the hex representation.
* In addition, a '+' character will be substituted for a space;
* for example:
* .CS
*         "test size" -> "test+size"
* .CE
* NOTE:
* The converted string can be up to three times longer than the original.
* You must therefore be careful to reserve sufficient buffer space.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*
* SEE ALSO: httpLineUnescape()
*/

HTTP_STATUS httpLineEscape
    (
    char *  szString,           /* plain string            */
    short   sMaxStringLength    /* size of reserved buffer */
    )
    {
    short sLength;
    short i = 0;
    char  szBuffer[5];
    char  cChar;

    if (szString == NULL)
      return (HTTP_OK);

    sLength = (short) strlen (szString);

    while (i < sLength)
        {
        cChar = szString[i];
        if (szString[i] == ' ')
            szString[i] = '+';         /* replace spaces by '+' */
        if (! isprint((int) cChar) || cChar == '+' || cChar == '%')
            {
            if (sLength+3 < sMaxStringLength)
                {
                httpStrCpy (&(szString[i])+2, &(szString[i]));
                sLength = (short) (sLength + 2);
                (void)snprintf (szBuffer, 4,"%#4.2x", cChar);
                strncpy (&(szString[i])+1,&(szBuffer[2]),2);
                szString[i] = '%';
                i = (short) (i + 2);
                }
            else
                {
                logInfo ("httpLineEscape: Buffer too small");
                return (HTTP_ERROR);   /* buffer too short */
                }
            }
        i++;
        }
    return (HTTP_OK);
    }

