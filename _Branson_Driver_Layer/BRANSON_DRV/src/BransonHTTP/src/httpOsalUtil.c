/* httpOsalUtil.c - utility functions that depend on the OS */

/* Copyright 1999-2007,2010 Wind River Systems, Inc. */
#include "copyright_wrs.h"

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
01d,26jan10,h_y  support for LP64	
01c,23feb07,msa  Correct the apigen comments so they match the actual
                 routines.
01b,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01a,15may97,mb   initial creation.

*/

/*
DESCRIPTION
This module encapsulates the OS dependend utility functions
used in the http server.

INCLUDE FILES: httpType.h

NOMANUAL
*/


#include <string.h>

#include "httpLib.h"


/*******************************************************************************
*
* httpOsalStringToken - break a string into separate tokens step by step
*
* This routine considers the null-terminated string <string>
* as a sequence of zero or more text tokens separated by spans
* of one or more characters from the separator string <separa-
* tors>. The  argument <ppLast> points to a user-provided
* pointer which in turn points to the position within <string>
* at which scanning should begin.
*
* In the first call to this routine, <string> points to a
* null-terminated string; <separators> points to a null-
* terminated string of separator characters; and <ppLast>
* points to a NULL pointer. The function returns a pointer to
* the first character of the first token, writes a null char-
* acter into <string> immediately following the returned
* token, and updates the pointer to which <ppLast> points so
* that it points to the first character following the null
* written into <string>. (Note that because  the separator
* character is overwritten by a null character, the input
* string is modified as a result of this call.)
*
* In subsequent calls <string> must  be a NULL pointer and
* <ppLast> must be unchanged so that subsequent calls will
* move through the string <string>, returning successive
* tokens until no tokens remain. The separator string
* <separators> may be different from call to call. When no
* token remains in <string>, a NULL pointer is returned.
*
* NOTE: For vxWorks see also strtok_r() man page.
*
* RETURNS: A pointer to the first character of a token, or a
*          NULL pointer if there is no token.
*
* NOMANUAL
*/

char * httpOsalStringToken
    (
    char *        string,      /* string to break into tokens      */
    char *        separators,  /* the separators                   */
    char **       ppLast       /* pointer to serve as string index */
    )
    {
    sbyte   *pToken;
    size_t  tokenLen;

    if (( ppLast == NULL ) || ( separators == NULL ))
        return NULL;

    if ( string == NULL )
        {
        if ( *ppLast == NULL )
            return NULL;
        else
            string = *ppLast;
        }

    tokenLen =  strspn( string, separators );
    string     += tokenLen;
    if ( *string == '\0')
        {
        *ppLast = NULL;
        return NULL;
        }

    pToken = string;
    string   = strpbrk( pToken, separators );
    if ( string != NULL )
        {
        *string = '\0';
        *ppLast = string + 1;
        }
    else
        *ppLast = NULL;

    return pToken;
    }


/*******************************************************************************
*
* httpOsalStringError - map an OS error number to an error string
*
* This routine maps the error number in <errcode> to an error
* message string. It stores the error string in <buffer>.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*
* NOMANUAL
*/

HTTP_STATUS httpOsalStringError
    (
    long    errcode,  /* error code    */
    char *  buffer    /* string buffer */
    )
    {
    /* !! needs to be revisited after WMB error mechanism is defined !! */
    /* strerror_r ( errcode , buffer ) */
    return ( HTTP_OK );
    }

