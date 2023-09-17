/* 40vxbAdxl345.cdf - ADXL345 accelerometer Driver */

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
14aug14,yya  created. (US43304)
*/

Component DRV_ADXL345 {
    NAME            ADXL345 accelerometer devices
    SYNOPSIS        ADXL345 accelerometer devices
    MODULES         vxbAdxl345.o
    LINK_SYMS       vxbAdxl345Drv
    REQUIRES        DRV_BUS_FDT_ROOT    \
                    INCLUDE_GPIO_SYS
    _CHILDREN       FOLDER_DRIVERS
}
