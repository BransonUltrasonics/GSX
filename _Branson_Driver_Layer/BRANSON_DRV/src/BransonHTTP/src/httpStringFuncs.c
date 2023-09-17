/* httpStringFuncs.c - utility functions for http server */

/* Copyright (c) 1999-2004, 2010-2011 Wind River Systems, Inc. */

/*
modification history
--------------------
02f,30mar11,r_w fix defect WIND00257291 make API reference readable
02e,26jan10,h_y  support for lp64
02d,13jan04,jws  remove HTTP_BOOL
02c,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02b,20apr98,mb   cleaned up.
02a,30sep96,ck   splitt off from httputil.
01b,17jul96,pk   documented, beautified.
01a,01jul96,ck   written.
*/

/*
DESCRIPTION
This module provides string utility functions for http server.

INCLUDE FILES: http/httpType.h

*/

#include "ctype.h"
#include <string.h>
#include "stdlib.h"

#include "httpType.h"


BOOL httpStringToUpper(char * szString);
/*******************************************************************************
*
* httpStringToUpper - convert a string to upper case
*
* This routine converts a whole string to upper case characters, using the
* routine toupper().
*
* RETURNS: TRUE, or FALSE if the string contains no
* characters that can be converted.
*
* SEE ALSO: httpStrNCaseCmp()
*/

BOOL httpStringToUpper
    (
    char * szString      /* Zero terminated string to convert */
    )
    {
    BOOL fConvertable = FALSE;
    long      i = -1;

    if ( szString != NULL )
	{
	fConvertable = TRUE;
	while ( szString[++i] != 0 )
	    szString[i] = (char)toupper (szString[i]);
	}
    return (fConvertable);
    }

