/* 40Omap35xxTimer.cdf - TI Omap3 Timer Component configuration file */

/* Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
20jun13,c_t  update
*/

Component DRV_TIMER_FDT_OMAP3 {
    NAME        FDT OMAP35xx timer Driver
    SYNOPSIS    FDT OMAP35xx timer Driver
    MODULES     vxbOmap3Timer.o
    LINK_SYMS   vxbOfOmap3TimerDrv
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
}

