/* 40vxbFdtArmGlobalTimer.cdf - ARM Global Timer driver */

/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
19feb14,g_x  make the component visible in workbench (VXW7-1956)
18feb14,sye  fixed typo. (VXW7-1917)
10jan14,cfm  renamed the component name
11nov13,cfm  written
*/

Component DRV_TIMER_FDT_ARM_GTC {
    NAME        ARM Global timer driver
    SYNOPSIS    ARM Global timer driver
    MODULES     vxbFdtArmGlobalTimer.o
    LINK_SYMS   vxbFdtArmGlobalTimerDrv
    REQUIRES    INCLUDE_VXBUS           \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
}
