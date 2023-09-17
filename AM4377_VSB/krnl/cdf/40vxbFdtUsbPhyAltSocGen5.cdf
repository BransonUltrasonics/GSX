/* 40vxbFdtUsbPhyAltSocGen5.cdf - Altera Cyclone V USB PHY Controller */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
15sep14,tnk  created
*/

Component INCLUDE_USB_PHY_ALT5 {
        NAME            Altera Cyclone V USB PHY Controller driver
        SYNOPSIS        Altera Cyclone V USB PHY Controller driver
        LINK_SYMS       vxbFdtUsbPhyAltSocGen5Drv
        REQUIRES        DRV_BUS_FDT_ROOT 
        _CHILDREN       FOLDER_USB_PHY_DRV
}
