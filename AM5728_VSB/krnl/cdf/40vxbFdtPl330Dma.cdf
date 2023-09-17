/* 40vxbFdtPl330Dma.cdf - ARM Pl330 DMA Controller Driver */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
20feb14,mpc  written (US29330).
*/

Component DRV_DMA_FDT_PL330 {
    NAME        ARM Pl330 DMA Controller FDT Driver
    SYNOPSIS    ARM Pl330 DMA Controller FDT Driver
    MODULES     vxbFdtPl330Dma.o
    LINK_SYMS   vxbFdtPl310DmaDrv
    REQUIRES    DRV_BUS_FDT_ROOT \
                INCLUDE_DMA_SYS
    _CHILDREN   FOLDER_DRIVERS
}
