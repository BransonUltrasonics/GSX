
/* Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
30oct13,mpc  written. (US18163)
*/

/* Generic CortexM System Tick Timer */

Component DRV_TIMER_FDT_CORTEXM {
        NAME            FDT Cortex-M System Tick Timer 
        SYNOPSIS        FDT Cortex-M systick system clock Timer
        LINK_SYMS       vxbCortexMSysTickDrv
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_TIMER_SYS
        _CHILDREN       FOLDER_DRIVERS
        _DEFAULTS       += FOLDER_DRIVERS
}
