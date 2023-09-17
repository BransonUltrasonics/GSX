/* 40vxbTiEdma3.cdf - TI EDMA3 driver */

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
12dec13,y_f  created
*/

Component       DRV_TI_EDMA3 {
    NAME        TI EDMA3 driver
    SYNOPSIS    vxBus Driver for ti edma3
    MODULES     vxbTiEdma3.o
    LINK_SYMS   vxbFdtTiEdma3Drv
    REQUIRES    INCLUDE_VXBUS   \
                INCLUDE_DMA_SYS
    _CHILDREN   FOLDER_DRIVERS
}

