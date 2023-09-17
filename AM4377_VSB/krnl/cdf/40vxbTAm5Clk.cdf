/* 40vxbTiAm5Clk.cdf - Component configuration file */

/* Copyright (c) 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
11nov15,yjw  written.(US64532)
*/

Component DRV_CLK_FDT_TI_AM5 {
    NAME            Clock management support for TI AM5 clock compatible devices
    SYNOPSIS        Freescale vybrid Clock support
    MODULES         vxbFdtTiAm5Clk.o
    LINK_SYMS       vxbfdtTiAm5ClkDrv
    REQUIRES        DRV_BUS_FDT_ROOT INCLUDE_DEVCLK_SYS
    _CHILDREN       FOLDER_DRIVERS
}
