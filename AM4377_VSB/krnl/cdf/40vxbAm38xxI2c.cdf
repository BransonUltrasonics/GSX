/* 40vxbAm38xxI2c.cdf - TI AM3 I2C driver */

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
29oct13,cfm  created
*/

Component       DRV_AM38XX_I2C {
    NAME        TI AM3 I2C driver
    SYNOPSIS    vxBus Driver for ti am3 I2C bus
    MODULES     vxbAm38xxI2c.o
    LINK_SYMS   vxbOfAm38xxI2cCtlrDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
