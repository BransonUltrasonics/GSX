/* 40vxbI2cPcfRtc.cdf - Component configuration file */

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
02jan14,mpc written (US22546)
*/

Component   DRV_I2C_PCFRTC {
    NAME        PCF8564 I2C RTC vxBus driver
    SYNOPSIS    PCF8564 I2C RTC vxBus driver
    MODULES     vxbI2cPcfRtc.o
    LINK_SYMS   vxbFdtI2cPcfRtcDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
