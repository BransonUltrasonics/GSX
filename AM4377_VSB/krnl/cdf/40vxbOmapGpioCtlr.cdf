/* 40vxbOmapGpioCtlr.cdf - OMAP GPIO controller */

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
13oc613,c_t  created
*/

Component DRV_GPIO_FDT_OMAP {
        NAME            FDT OMAP 35xx GPIO Controller driver
        SYNOPSIS        FDT OMAP 35xx GPIO Controller driver
        MODULES         vxbOmapGpioCtlr.o
        LINK_SYMS       vxbFdtOmapGpioDrv
        REQUIRES        DRV_BUS_FDT_ROOT \
                        INCLUDE_GPIO_SYS
        _CHILDREN       FOLDER_DRIVERS

}
