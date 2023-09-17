/* 40vxbVirtBus.cdf - Component Definition file for virtual bus driver */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
12mar14,l_z  created
*/

#ifdef _WRS_CONFIG_VXBUS_VIRT
Component       DRV_BUS_VIRT {
    NAME        VxBus Virtual bus controller driver
    SYNOPSIS    VxBus Virtual bus controller driver
    MODULES     vxbVirtBus.o
    LINK_SYMS   vxbVirtBusDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
#endif /* _WRS_CONFIG_VXBUS_VIRT */
