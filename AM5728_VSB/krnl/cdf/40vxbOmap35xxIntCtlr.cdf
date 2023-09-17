/* 40vxbOmap35xxIntCtlr.cdf - OMAP 3 interrupt controller */

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
13sep13,c_t  created
*/

Component DRV_INTCTLR_FDT_OMAP3 {
        NAME            FDT OMAP 35xx Interrupt Controller driver
        SYNOPSIS        FDT OMAP 35xx Interrupt Controller driver
        MODULES         vxbOmap3IntCtlr.o
        LINK_SYMS       vxbFdtOmap3IntCtlrDrv
        REQUIRES        DRV_BUS_FDT_ROOT
        _CHILDREN       FOLDER_DRIVERS
}
