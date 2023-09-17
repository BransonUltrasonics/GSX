/* 40vxbM85xxTimer.cdf - Component Definition file for m85xx timer */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02Dec13,r_y  created
*/

Component       DRV_TIMER_OPENPIC {
    NAME        OpenPic Timer Driver
    SYNOPSIS    vxBus Driver for openPic Timer
    MODULES     vxbOpenPicTimer.o
    LINK_SYMS   vxbOpenPicTimerDrv
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
}
