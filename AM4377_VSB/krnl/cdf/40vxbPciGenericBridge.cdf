/* 40vxbPciGenericBridge.cdf - Component Definition for generic PCI bridge driver */

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
10aug15,wap  Change dependency on INCLUDE_VXBUS to INCLUDE_PCI_BUS
08jun15,wap  Created
*/

Component	DRV_PCI_BRIDGE_GENERIC {
    NAME	Generic VxBus PCI bridge driver
    SYNOPSIS	Generic VxBus PCI bridge driver
    MODULES	vxbPciGenericBridge.o
    LINK_SYMS	vxbPcibDrv
    REQUIRES	INCLUDE_PCI_BUS
    INCLUDE_WHEN	INCLUDE_PCI_BUS
    _CHILDREN	FOLDER_BUSLIB_PCI
}
