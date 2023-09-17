/* 40vxbFdtUsbPhyUlpiViewPort.cdf - General USB ULPI Viewport PHY controller */

/*
 * Copyright (c) 2013, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
16jan14,s_z  code clean up
29dec13,s_z  created
*/

Component INCLUDE_USB_PHY_ULPI_VIEWPORT {
        NAME            FDT USB Viewport ULPI PHY Controller driver
        SYNOPSIS        This model is used as FDT USB ULPI PHY driver to access general ULPI PHYs(such as ISP1504, SMSC3300 and etc) over viewport register.
        MODULES         vxbFdtUsbPhyUlpiViewPort.o
        LINK_SYMS       vxbFdtUsbPhyUlpiViewPortDrv
        REQUIRES        DRV_BUS_FDT_ROOT 
        _CHILDREN       FOLDER_USB_PHY_DRV
}
