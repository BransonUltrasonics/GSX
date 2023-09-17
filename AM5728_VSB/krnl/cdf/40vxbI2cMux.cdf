/* 40vxbI2cMux.cdf - Component Definition file for I2C  Mux*/

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
23may13,y_y  created
*/

Component       DRV_I2C_PCA954X {
    NAME        PCA954x I2C mux/switch driver
    SYNOPSIS    PCA954x I2C mux/switch driver
    MODULES     vxbI2cPca954xMux.o vxbI2cMuxChan.o
    LINK_SYMS   vxbOfI2cPca954xMuxDrv vxbOfI2cMuxChanDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}


