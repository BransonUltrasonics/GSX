/* 40vxbCpm2IntCtlr.cdf - Component Definition file for CPM PIC interrupt controller */

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
01a,23oct13,syt  created.(F2505)
*/

Component       DRV_INTCTLR_CPM_PIC {
    NAME        CPM PIC Interrupt Controller Driver
    SYNOPSIS    vxBus Driver for CPM PIC Interrupt Controller
    MODULES     vxbCpmIntCtlr.o
    LINK_SYMS   vxbCpmPicIntCtlrDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
