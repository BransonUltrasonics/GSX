/* httpDate.c - Access functions for OSAL date functions */

/* Copyright 1999,2010 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* Project: Wind Web Server, Version 2.0
 *
 * Copyright (c) 1996/97/98, 3Soft GmbH
 *
 * 3Soft GmbH, Wetterkreuz 19a
 * D-91058 Erlangen, Germany
 * infohttp@dreisoft.de, http://www.dreisoft.de/
 */

/*
modification history
-------------------- 
03c,02feb10,h_y   support for LP64
03b,18jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
03a,08jul98,ck   created.
*/

/*
DESCRIPTION
This module provides a calling interface for OSAL date conversion/aquisition
routines.

INCLUDE FILES:
	httpLib.h
	private/httpDate.h


*/

#include "httpLib.h"
#include "private/httpDate.h"

/****************************************************************************
*
* httpDateGet - Get System Date.
*
* This routine gets the system date and puts it in the string pointed to by
* szDate. You must provide space for 32 characters.
*
* At the moment it uses a call to clock_gettime, which does not work
* correctly on all targets. A better version will be supplied.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpDateGet
    (
    char * const szDate		/* Pointer to buffer to hold the date string */
    )
    {
    return (httpOsalDateGet (szDate));
    }

