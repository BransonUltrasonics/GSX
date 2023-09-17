/* 40vxbFdtOmapMailbox.cdf - VxBus OMAP mailbox driver for TI AM572X */

/*
 * Copyright (c) 2016-2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
24jan17,ghl  updated SYNOPSIS (F8373)
01jun16,mw1  Clean up cdf files (F5252).
10may16,mw1  created for TI AM572x platform.
*/

#ifdef _WRS_CONFIG_OPENAMP
Component DRV_FDT_TI_OMAP_MAILBOX {
    NAME            Mailbox driver for TI OMAP compatible devices
    SYNOPSIS        TI AM572X OMAP Mailbox driver support to OPENAMP.
    LINK_SYMS       vxbOmapMailboxDrv
    REQUIRES        DRV_BUS_FDT_ROOT
    _CHILDREN       FOLDER_DRIVERS
}
#endif

