/* httpStrCaseCmp.c - utility functions for http server */

/* Copyright (c) 1999-2003,2010-2011 Wind River Systems, Inc. */

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
02g,30mar11,r_w fix defect WIND00257291 make API reference readable
02f,26jan10,h_y  support for 2010
02e,01oct03,jws  remove assert()'s
02d,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02c,13may97,mb   osal introduced.
02b,04oct96,mb   bugfix (returned TRUE/FALSE if sStrLen was too small).
02a,30sep96,ck   splitt off from httputil.
01b,17jul96,pk   documented, beautified.
01a,01jul96,ck   written.
*/

/*
DESCRIPTION
This module provides utility functions for http server.

INCLUDE FILES: http/httpLib.h

*/

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "httpLib.h"


/***************************************************************************
*
* httpStrNCaseCmp - compare two strings without regard to case
*
* This routine compares two strings without regard to case.
* Since many elements of HTTP request processing should be case insensitive,
* it is good practice to use this routine.
* This routine compares two strings, with up to <sStrLen> characters.
* If one
* string is shorter, they are considered to be not equal.
*
* NOTE:
* Because of performance issues, the comparison is done from [sSreLen]
* to [0], so that strings with same prefix, but different endings
* are found more quickly (for example, URL tables).
*
* RETURNS:
* An integer greater than, equal to, or less than 0, according to
* whether s1 is lexicographically greater than, equal to, or less
* than s2, respectively.
*/

short httpStrNCaseCmp
    (
    const char * const szString1,  /* String one */
    const char * const szString2,  /* String two */
    unsigned long      sStrLen     /* Number of characters to compare */
    )
    {

    if ((szString1 == NULL) && (szString2 == NULL))
        return 0;
    else if (szString2 == NULL)
        return 1;
    else if (szString1 == NULL)
        return -1;

    if (sStrLen <=0)  return (0);

    if (sStrLen > strlen(szString1) || sStrLen > strlen(szString2))
        return (1);

    /* done with special cases */

    while ((--sStrLen>0) &&
          (toupper (szString1[sStrLen]) == toupper (szString2[sStrLen])));

    return ((short)(toupper (szString1[sStrLen]) - toupper (szString2[sStrLen])));
    }

