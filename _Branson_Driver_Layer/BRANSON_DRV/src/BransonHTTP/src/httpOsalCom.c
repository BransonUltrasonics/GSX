/* httpOsalCom.c - common OSAL functions useful for several purposes  */

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
01d,27jan10,h_y  support for LP64
01c,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01b,29jun98,ck   added task ID to output.
01a,20may97,mb   introduced httpOsalInit(), httpOsalLogInfo(),
                 and httpOsalLogError().
*/

/*
DESCRIPTION
This module provides functions common to the OSAL layer.
(DEPRECATED)

INCLUDE FILES: http/httpType.h
               http/private/httpOsalCom.h

*/

/* includes */

/* general includes */
#include "stdlib.h"
#include <stdio.h>

/* HTTP specific */
#include "httpLib.h"
#include "private/httpOsalCom.h"


HTTP_STATUS httpOsalInit(void);

/*******************************************************************************
*
* httpOsalInit - initialize the osal package
*
* This function should be called once at server startup, before any other osal
* functions are called.
*
* RETURNS: N/A
* NOMANUAL
*/

HTTP_STATUS httpOsalInit(void)
    {
    return HTTP_OK;
    }


/*******************************************************************************
*
* httpOsalLogInfoFunction - Log a Info mesage to stderr
*
* This function can be used when writing new code for the server to provide
* verbose output to the developer. When you compile your source with the
* define HTTP_OSAL_DEBUG_INFO, the string will be printed using fprintf to
* stderr. When you do not have defined HTTP_OSAL_DEBUG_INFO (typically in
* the release version), the string will no longer be compiled in and you
* thus save memory.
*
* RETURNS: N/A
* NOMANUAL
*/

void httpOsalLogInfoFunction
    (
    char * szMessage
    )
    {
    perror (szMessage);
    }


/*******************************************************************************
*
* httpOsalLogErrorFunction - Log a Error mesage to stderr
*
* This function can be used when writing new code for the server to provide
* verbose output to the developer.  When you compile your source with the
* define HTTP_OSAL_DEBUG_ERROR, the string will be printed using perror.
* When you do not have defined HTTP_OSAL_DEBUG_ERROR (typically in the memory
* critical version), the string will no longer be compiled in and you thus
* save memory.
*
* RETURNS: N/A
* NOMANUAL
*/

void httpOsalLogErrorFunction
    (
    char * szMessage /* String to print with perror */
    )
    {
    perror (szMessage);
    }

