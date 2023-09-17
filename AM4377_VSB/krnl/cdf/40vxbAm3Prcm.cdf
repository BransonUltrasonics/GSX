/* 40vxbAm3Prcm.cdf - AM3XXX Power management and Clock Control */

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
13oct13,c_t  created
*/

Component DRV_CLK_FDT_AM3 {
        NAME            FDT AM3XXX PRCM driver
        SYNOPSIS        FDT AM3XXX PRCM driver
        MODULES         vxbAm3Prcm.o
        LINK_SYMS       vxbAm3PrcmDrv
        REQUIRES        DRV_BUS_FDT_ROOT \
                        INCLUDE_DEVCLK_SYS
        _CHILDREN       FOLDER_DRIVERS

}
