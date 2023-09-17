/* httpStrdup.c - strdup function as known by several UNIX versions */

/* Copyright 1999,2010 Wind River Systems, Inc. */
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
01f,26jan10,h_y  support for lp64
01e,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01d,17apr98,mb   bugfix: do not perform strlen(szString) if szString is NULL!
01c,13may97,mb   osal introduced.
01b,19mar97,mb   do a malloc with ( sLength + 1) chars
01a,31oct96,mb   written.
*/

/*
DESCRIPTION
This file implements a simple string copy function as known from
several UNIX versions.

INCLUDE FILES: memPoolLib.h
               http/httpLib.h

NOMANUAL
*/


/* includes */
#include <string.h>
#include <stdlib.h>

#include "httpLib.h"
#include "commonCfg.h"


/*******************************************************************************
*
* httpStrdup -  duplicate a string
*
* This routine performs the same function as the UNIX strdup() routine,
* duplicating a string.
* It uses
* memPoolAlloc() to reserve enough memory for the copy from the given
* memory pool.
*
* RETURNS: A pointer to the copy of the string.
*/

char * httpStrdup
    (
     MEM_POOL_ID   memPartID,  /* partition to allocate from */
     const char * const szString    /* string to copy             */
    )
    {
    char * szTmp = NULL;

    if (szString != NULL)
    {
    if ((szTmp = (char *) memPoolAlloc ( memPartID , strlen (szString) +1 )) != NULL)
            {
            strcpy (szTmp, szString);
            }
#ifdef DEBUG_INFO
        else
        logInfo ("httpStrdup: memPoolAlloc Failed!");
#endif
    }
    return (szTmp);
    }

