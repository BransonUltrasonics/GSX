/* 40vxbFslSpi.cdf - Component Definition file for Freescale SPI */

/*
 * Copyright (c) 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
14oct15,wqi  created (US66526)
*/

Component       DRV_FSL_SPI {
    NAME        Freescale SPI Controller Driver
    SYNOPSIS    vxBus Driver for Freescale SPI Controller
    MODULES     vxbFdtFslSpi.o
    LINK_SYMS   vxbFdtFslSpiDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
