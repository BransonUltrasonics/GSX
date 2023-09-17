/* 40vxbFslPci.cdf - Component Definition file for Freescale PCI/PCIe */

/*
 * Copyright (c) 2013, 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
10aug15,wap  Change dependency on VxBus to dependency on PCI bus
28may13,l_z  add law-bar require. (US37630)
23may13,j_z  created
*/

Component       DRV_FSL_PCI {
    NAME        VxBus Qoriq PCI controller Driver
    SYNOPSIS    VxBus Qoriq PCI controller Driver
    MODULES     vxbFslPci.o
    LINK_SYMS   fslPciDrv
    REQUIRES    INCLUDE_PCI_BUS \
		INCLUDE_PCI_AUTOCONF
                DRV_FSL_LAW
    _CHILDREN   FOLDER_DRIVERS
}
