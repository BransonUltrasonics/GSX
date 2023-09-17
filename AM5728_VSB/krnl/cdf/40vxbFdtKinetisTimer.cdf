/* 40vxbFdtKinetisTimer.cdf - Component Definition file for Kinetis timer */

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
26jun14 mpc  created (US40538)
*/

Component       DRV_TIMER_FDT_KINETIS {
    NAME        Kinetis Timer Driver
    SYNOPSIS    Freescale Kinetis Timer Driver
    MODULES     vxbFdtKinetisTimer.o
    LINK_SYMS   vxbFdtKinetisTimerDrv
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
}
