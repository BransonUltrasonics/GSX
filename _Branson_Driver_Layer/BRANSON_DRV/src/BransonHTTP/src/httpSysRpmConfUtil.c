/* httpSysRpmConfUtil.c - Utilities for the http RPM configuration table */

/* Copyright 1999,2007,2010 Wind River Systems, Inc. */

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
01e,26jan10,h_y  support for lp64
01d,01oct07,msa  Do not add RPMs that are already in the list
01c,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01b,15may97,mb   osal introduced.
01a,17oct96,ck   written.
*/

/*
DESCRIPTION

INCLUDE FILES: http/httpLib.h
           http/httpRpms.h

NOMANUAL
*/


/* includes */
#include "httpLib.h"
#include "httpRpms.h"

static short sHttpSysRpmDispTabMax_Limit HTTP_PREDEFINE ( 0 );


/****************************************************************************
*
* httpSysRpmConfInit - initialize the sizes of the system RPM tables
*
* This routine initializes the internal boundaries needed for
* subsequent calls of all the other API functions to access the
* system RPM configuration tables.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*
* SEE ALSO: httpSysRpmConfAdd()
*/

HTTP_STATUS httpSysRpmConfInit
    (
    short sHttpSysRpmDispTabMax /* Number of entries in predefined table */
    )
    {
    sHttpSysRpmDispTabMax_Limit = sHttpSysRpmDispTabMax;
    return (HTTP_OK);
    }


/****************************************************************************
*
* httpSysRpmConfAdd - add a entry to the system RPM table
*
* This routine adds the specified entry to the system RPM table.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*
* SEE ALSO: httpSysRpmConfInit()
*/

HTTP_STATUS httpSysRpmConfAdd
    (
    HTTP_RPM  pRpm        /* Pointer to the RPM-function */
    )
    {
    short        sTableLength;

    for (sTableLength = 0; sTableLength < sHttpSysRpmDispTabMax_Limit;
    		sTableLength ++)
    	{
    	if (httpSystemRpms [sTableLength] == NULL)
    		break;    		
    	else if (httpSystemRpms [sTableLength] == pRpm)
    		return (HTTP_OK);
    	}

    if (httpSystemRpms [sTableLength] != NULL)
        {
        logError ("httpSysRpmConfAdd: Table full/damaged!");
        return (HTTP_ERROR);
        }

    httpSystemRpms [sTableLength] = pRpm;
    sTableLength++;
    httpSystemRpms [sTableLength] = NULL;

    return (HTTP_OK);
    }

