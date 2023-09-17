/* 40vxbMpicIntCtlr.cdf - Component Definition file for MPIC interrupt controller */

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
01a,23oct13,syt  created
*/

Component       DRV_INTCTLR_FDT_MPIC {
    NAME        MPIC Interrupt Controller Driver
    SYNOPSIS    vxBus Driver for MPIC Interrupt Controller
    MODULES     vxbMpicIntCtlr.o
    LINK_SYMS   vxbMpicIntCtlrDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
