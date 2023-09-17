/* usrSdMmc.c - SD/MMC initialization */

/*
 * Copyright (c) 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,26oct15,myt written
*/

/*
DESCRIPTION
*/

/* include */

#include <vxbSdMmcLib.h>

/* defines */

/* typedefs */

/* globals */

/* externs */
BOOL mmcHrfsInclude = FALSE;
	
/***************************************************************************
*
* usrSdMmcInit -
*/

void usrSdMmcInit (void)
    {
#if defined(INCLUDE_HRFS)
    mmcHrfsInclude = TRUE;
#endif
    }
