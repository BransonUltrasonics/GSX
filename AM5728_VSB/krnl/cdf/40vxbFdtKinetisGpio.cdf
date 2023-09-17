/* 40vxbFdtKinetisGpio.cdf - Component configuration file */

/* Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
27jun14,yjw  written (US40533)
*/

Component DRV_GPIO_FDT_KINETIS {
        NAME            FDT kinetis gpio Driver
        SYNOPSIS        FDT kinetis gpio Driver
        MODULES         vxbFdtKinetisGpio.o
        LINK_SYMS       vxbKinetisGpioDrv
        REQUIRES        DRV_BUS_FDT_ROOT \
                        INCLUDE_GPIO_SYS
        _CHILDREN       FOLDER_DRIVERS
}

