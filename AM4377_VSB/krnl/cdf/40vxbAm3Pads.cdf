/* 40vxbAm3Pads.cdf  - AM3XXX System Control Module */

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

Component DRV_PINMUX_FDT_AM3 {
        NAME            FDT AM3XXX PADS driver
        SYNOPSIS        FDT AM3XXX PADS driver
        MODULES         vxbAm3Pads.o
        LINK_SYMS       vxbAm3PadsDrv
        REQUIRES        DRV_BUS_FDT_ROOT \
                        INCLUDE_PINMUX_SYS
        _CHILDREN       FOLDER_DRIVERS

}
