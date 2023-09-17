/* 40vxbFslDma.cdf - Component Definition file for Freescale DMA */

/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
15apr14,ylu  added dependency to INCLUDE_DMA_SYS driver.
23may13,y_y  created
*/

Component       DRV_FSL_DMA {
    NAME        m85xx DMA Driver
    SYNOPSIS    vxBus Driver for DMA Timer
    MODULES     vxbFslDma.o
    LINK_SYMS   vxbFslDmaDrv
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_DMA_SYS
    _CHILDREN   FOLDER_DRIVERS
}
