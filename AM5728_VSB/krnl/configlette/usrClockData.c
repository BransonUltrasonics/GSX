/* usrClockData.c - user static clock data */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
17apr14,sye  renamed to usrClockData.c. (US36393)
07jan14,wqi  written.
*/

/*
DESCRIPTION

This file describes the static clock tree of sitara ctxa8 targets.
The following two structures are used to describe the clock tree: 
VXB_CLK_DOMAIN and VXB_CLK. The static clock tree handle is registered 
to vxbClkLib by vxbClkStaticDataSet().

See also: vxbClkLib.h
INCLUDE FILES: vxbAm3Prcm.h
*/

#include <vxWorks.h>

/* include board specific clock tree */

#ifdef INCLUDE_TI_AM335X_EVMSK
#include "am335xEvmSKClkData.c"
#else
#include "am335xEvmClkData.c"
#endif /* INCLUDE_TI_AM335X_EVMSK */

/*******************************************************************************
* 
* usrStaticClkDataInit - initialize the usr static clock tree
*
* This routine initializes the the usr static clock tree.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrStaticClkDataInit (void)
    {
    (void)vxbClkStaticDataSet (usrClocks, usrClocksDm);
    }
