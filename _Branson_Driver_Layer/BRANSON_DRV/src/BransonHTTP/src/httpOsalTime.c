/* httpOsalTime.c - utility functions for http server */

/*
 * Copyright (c) 1996-2010, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26sep14,r_w  fix static CHECKED_RETURN issue.
05may14,r_w  fix static analysis issues(US35919)
27jan10,h_y  support for LP64
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
08jul98,ck   renamed to osal name conventions.
30sep96,ck   split off from httputil.
24aug96,ck   initial creation.
*/

/*
DESCRIPTION

INCLUDE FILES:

NOMANUAL
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "private/httpDate.h"
#include "private/httpOsalTime.h"


/****************************************************************************
*
* httpOsalDateGet - get system date
*
* This routine gets the system date and puts it in the string pointed to by
* szDate. You have to provide space for 32 characters..
*
* NOMANUAL
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpOsalDateGet
    (
    char * const szDate     /* Pointer to buffer to hold the date string */
    )
    {
    time_t      t;
    struct tm * pTs;

    (void) time(&t);
    /* !! warning, the next function is not reentrant.  Needs to be fixed !! */
    pTs = gmtime(&t);
    if(NULL == pTs)
        return HTTP_ERROR;
    
    if (strftime (szDate, 32, "%a, %d %b %Y %H:%M:%S GMT", pTs) == 0)
        return HTTP_ERROR;

    return HTTP_OK;
    }

