/* httpStrcpy.c - utility functions for http server */

/* Copyright 1999,2010 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* Project: Wind Web Server, Version 2.0
 *
 * Copyright (c) 1996/97, 3Soft GmbH
 *
 * 3Soft GmbH, Wetterkreuz 19a
 * D-91058 Erlangen, Germany
 * infohttp@dreisoft.de, http://www.dreisoft.de
 */

/*
modification history
--------------------
02c,26jan10,h_y  support for LP64
02b,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02a,30sep96,ck   split off from httputil.
01a,01aug96,ck   initial creation.
*/

/*
DESCRIPTION

INCLUDE FILES:

NOMANUAL
*/

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "httpLib.h"


/*******************************************************************************
*
* httpStrCpy - copy overlapping string within one buffer
*
* This routine is useful when shifting a string within a buffer
* to eliminate or insert characters.
* It is designed to work when the source and target location
* overlap.
*
* RETURNS: Pointer to the resulting string <szTo>.
*/

char * httpStrCpy
    (
    char * szTo,     /* destination pointer */
    char * szFrom    /* source pointer      */
    )
    {
    char * szSrc;
    char * szDest;

    if (szTo == szFrom)
        return szTo;

    if (szTo < szFrom)
        {                  /* copy from left to right, shorten string */
        szSrc  = szFrom;
        szDest = szTo;
        while ( (*szDest = *szSrc) != 0)
            {
            szDest++;
            szSrc++;
            }
        }
    else
        {                  /* copy from right to left, lengthen string */
        szSrc  = szFrom + strlen (szFrom);
        szDest = szTo + strlen (szFrom);
        while (szDest >= szFrom)
            {
            *szDest = *szSrc;
            szDest--;
            szSrc--;
            }
        }
    return szTo;
    }

