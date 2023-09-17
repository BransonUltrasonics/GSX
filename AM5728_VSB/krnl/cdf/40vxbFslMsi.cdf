/* 40vxbFslMsi.cdf - Freescale MSIi controller */

/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
28apr14,y_y  fix comment issue. (US37630)
23may13,y_y  created
*/

Component DRV_FSL_MSI {
    NAME        VxBus Freescale MSI driver
    SYNOPSIS    VxBus Freescale MSI driver
    MODULES     vxbFslMsi.o
    LINK_SYMS   vxbFslMsiDrv
    REQUIRES    DRV_BUS_FDT_ROOT \
                INCLUDE_INTCTLR_DYNAMIC_LIB
    _CHILDREN   FOLDER_DRIVERS
}
