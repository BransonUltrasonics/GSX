/* 40vxbFdtKinetisCache.cdf - Kinetis unified cache  */

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
27jun14,yjw created (US18163)
*/

Component DRV_CACHE_FDT_KINETIS {
    NAME        FDT Kinetis unified Cache
    SYNOPSIS    FDT Kinetis unified Cache driver
    LINK_SYMS   vxbFdtKinetisCacheDrv
    REQUIRES    DRV_BUS_FDT_ROOT  \
		INCLUDE_CACHE_SUPPORT
    _CHILDREN   FOLDER_NOT_VISIBLE
}
