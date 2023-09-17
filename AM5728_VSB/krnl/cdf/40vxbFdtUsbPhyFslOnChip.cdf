/* 40vxbFdtUsbPhyFslOnChip.cdf - Freescale on-chip internal USB PHY controller */

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
20jan14,s_z  code clean up
29dec13,s_z  created
*/

Component INCLUDE_USB_PHY_FSL_ONCHIP {
        NAME            FDT Freescale Internal on-chip USB PHY Controller driver
        SYNOPSIS        This model is the FDT Freescale on-chip USB PHY Controller driver, which mainly used on fsl_p3041ds,fsl_p5020ds platfroms
        MODULES         vxbFdtUsbPhyFslOnChip.o
        LINK_SYMS       vxbFdtUsbPhyFslOnChipDrv
        REQUIRES        DRV_BUS_FDT_ROOT 
        _CHILDREN       FOLDER_USB_PHY_DRV
}
