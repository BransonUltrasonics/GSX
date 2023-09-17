/* 40vxbI2cEeprom.cdf - Component Definition file for I2C EEPROM */

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

Component       DRV_I2C_EEPROM {
    NAME        I2c eeprom Driver
    SYNOPSIS    vxBus Driver for I2c eeprom
    MODULES     vxbI2cEeprom.o
    LINK_SYMS   vxbOfI2cEepromDrv
    REQUIRES    INCLUDE_VXBUS INCLUDE_EEPROMDRV
    _CHILDREN   FOLDER_DRIVERS
}
