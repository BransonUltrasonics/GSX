/* 40vxbAm35xxSpi.cdf - TI AM3 SPI driver */

/*
 * Copyright (c) 2013-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
22apr15,y_f  updated for adapting new SPI library. (V7PRO-1908)
09oct14,c_t  update Name(V7PRO-1257)
04nov13,cfm  created
*/

Component       DRV_AM35XX_SPI {
    NAME        TI AM3 SPI driver
    SYNOPSIS    vxBus Driver for ti am3 SPI bus
    MODULES     vxbTiAm335xSpi.o
    LINK_SYMS   vxbOfAm335xSpiDrv
    REQUIRES    INCLUDE_VXBUS   \
                INCLUDE_SPI_BUS
    _CHILDREN   FOLDER_DRIVERS
}

