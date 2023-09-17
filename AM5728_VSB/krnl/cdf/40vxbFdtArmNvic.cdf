/* 40vxbFdtArmNvic.cdf - ARM NVIC interrupt controller */

/*
* Copyright (c) 2013 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River License agreement.
*/

/*
modification history
--------------------
30oct13,mpc  written (US18163)
*/

Component DRV_INTCTLR_FDT_ARM_NVIC {
    NAME        FDT ARM NVIC Interrupt Controller driver
    SYNOPSIS    FDT NVIC Interrupt Controller driver
    LINK_SYMS   vxbFdtArmNvicDrv
    REQUIRES    DRV_BUS_FDT_ROOT
    _CHILDREN   FOLDER_DRIVERS
}
