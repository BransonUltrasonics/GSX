/* 40vxbFdtSensorMma8451q.cdf - MMA8451Q accelerometer Driver */

/*
 * Copyright (c) 2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
27jan16,sye  removed dependency of DRV_GPIO_FDT_KINETIS (US74710)
18nov14,g_x  created. (US48533)
*/

Component DRV_SENSOR_FDT_MMA8451Q {
    NAME            MMA8451Q accelerometer devices
    SYNOPSIS        MMA8451Q accelerometer devices
    MODULES         vxbFdtSensorMma8451q.o
    LINK_SYMS       vxbFdtMma8451qDrv
    REQUIRES        DRV_BUS_FDT_ROOT    \
                    INCLUDE_GPIO_SYS
    _CHILDREN       FOLDER_DRIVERS
}
