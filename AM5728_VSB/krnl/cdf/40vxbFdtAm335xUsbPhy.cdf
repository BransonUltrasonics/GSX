/* 40vxbFdtAm335xUsbPhy.cdf - AM335x USB PHY controller */

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
09apr14,m_y  created
*/

Component INCLUDE_USB_PHY_AM335X {
        NAME            AM335X USB PHY Controller driver
        SYNOPSIS        This model is the FDT AM335X USB PHY Controller driver
        LINK_SYMS       vxbFdtUsbPhyAm335xDrv
        REQUIRES        DRV_BUS_FDT_ROOT 
        _CHILDREN       FOLDER_USB_PHY_DRV
}
