/* 40vxbFdtArmGenIntCtlr.cdf - ARM generic interrupt controller */

/*
 * Copyright (c) 2013, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
22apr16,emj  CERT: Update to produce partially linked objects (US79607)
31oct13,sye  Created.
*/

Component DRV_INTCTLR_FDT_ARM_GIC {
    NAME        FDT ARM Generic Interrupt Controller driver
    SYNOPSIS    FDT ARM Generic Interrupt Controller driver
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
    MODULES     vxbFdtArmGenIntCtlr.o
    LINK_SYMS   vxbFdtArmGenIntCtlrDrv
#else
        _REQUIRES INCLUDE_KERNEL
#endif
    REQUIRES    DRV_BUS_FDT_ROOT 
    _CHILDREN   FOLDER_DRIVERS
}
