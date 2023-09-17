/* 40vxbFdtFslVybridADC.cdf - Component configuration file */

/* Copyright (c) 2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
22jan16,sye  moved to vxbus_drv to support i.MX6 SoloX (US74226)
04aug14,yya  created. (US43305)
*/

Component DRV_FSL_VYBRID_ADC {
    NAME            Freescale Vybrid ADC driver
    SYNOPSIS        Freescale Vybrid ADC driver
    MODULES         vxbFdtFslVybridADC.o
    LINK_SYMS       vxbFdtFslVybridADCDrv
    REQUIRES        DRV_BUS_FDT_ROOT
    _CHILDREN       FOLDER_DRIVERS
}
