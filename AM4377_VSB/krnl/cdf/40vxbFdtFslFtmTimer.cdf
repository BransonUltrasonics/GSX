/* 40vxbFdtFslFtmTimer.cdf - Component Definition file for Freescale Flex timer */

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
05nov14 g_x  created (US40538)
*/

Component       DRV_TIMER_FDT_FSL_FTM {
    NAME        Freescale Flex Timer Driver
    SYNOPSIS    Freescale Flex Timer Driver
    MODULES     vxbFdtFslFtmTimer.o
    LINK_SYMS   vxbFdtFslFlexTimerDrv
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_TIMER_SYS \
                DRV_BUS_FDT_ROOT
    _CHILDREN   FOLDER_DRIVERS
}
