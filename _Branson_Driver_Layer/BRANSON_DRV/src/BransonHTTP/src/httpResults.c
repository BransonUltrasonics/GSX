/* httpResults.c - Generate HTTP result messages */

/* Copyright 1999-2004,2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01g,26jan10,h_y  support for LP64
01f,13feb07,msa  Fix comments to remove APIgen errors.
01e,16jan04,jws  remove per project dependencies
01d,01oct03,jws  remove assert()'s
01c,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01b,13may97,mb   osal introduced.
01a,03sep96,ck   initial creation.
*/

/*
DESCRIPTION
This module should provide a scalable interface to supply (more or
less) verbose HTTP result messages.

INCLUDE FILES: http/httpLib.h
               http/private/httpInt.h

NOMANUAL
*/

#include <stdio.h>
#include "errno.h"
#include <string.h>

#include "httpLib.h"
#include "private/httpInt.h"

/* externals - will move to header file */

IMPORT const BOOL httpVarBasicMsgs;

/***************************************************************************
*
* httpStatus - Return current status
*
* RETURNS:
*
* NOMANUAL
*/
char * httpStatus
    (
    short sStatusNumber   /* see .h file for applicable keys */
    )
    {
    char * szStatusString = NULL;
    short  i = 0;

    if (httpVarBasicMsgs)
        sStatusNumber = (short) (100 * (short)(sStatusNumber / 100));

    do
        {
        if ( (sStatusNumber == httpStatusMessages[i].sNumber) ||
             (httpStatusMessages[i].sNumber == 0))
            szStatusString = httpStatusMessages[i].szMessage;
        i++;
        }
    while (szStatusString == NULL);

    return szStatusString;
    }

