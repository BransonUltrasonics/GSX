/* 40vxbFslEdma.cdf - Component configuration file */

/* Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
17nov14,y_f  created. (US47599)
*/

Component DRV_DMA_FDT_FSL_EDMA {
    NAME            DMA support for Freescale eDMA compatible devices
    SYNOPSIS        Freescale eDMA support
    MODULES         vxbFslEdma.o
    LINK_SYMS       vxbFdtFslEdmaDrv
    REQUIRES        DRV_BUS_FDT_ROOT    \
                    INCLUDE_DMA_SYS
    _CHILDREN       FOLDER_DRIVERS
}