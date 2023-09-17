/* 40vxbFdtTiAM572xPcie.cdf - VxBus PCI Express driver for TI AM572X */

/*
 * Copyright (c) 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
12oct15,y_f  created. (US64525)
*/

Component DRV_FDT_TI_AM572X_PCIE
    {
    NAME            PCIE support for TI AM572X compatible devices
    SYNOPSIS        TI AM572X PCIE support
    MODULES         vxbFdtTiAM572xPcie.o
    LINK_SYMS       vxbFdtTiAm572xPcieDrv
    REQUIRES        DRV_BUS_FDT_ROOT        \
                    DRV_GPIO_FDT_OMAP       \
                    INCLUDE_PCI_BUS         \
                    INCLUDE_PCI_AUTOCONF
    _CHILDREN       FOLDER_DRIVERS
    }
