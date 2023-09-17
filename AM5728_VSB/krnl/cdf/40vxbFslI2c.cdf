/* 40vxbFslI2c.cdf - Component Definition file for Freescale IIC */

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
01a,23may13,y_y  created
*/

Component       DRV_FSL_I2C {
    NAME        Freescale I2C bus controller vxBus driver
    SYNOPSIS    Freescale I2C bus controller vxBus driver
    MODULES     vxbFslI2c.o
    LINK_SYMS   vxbOfFslI2cCtlrDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
