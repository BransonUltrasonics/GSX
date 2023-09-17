/* 40vxbFdtKinetisCache.cdf - Kinetis unified cache  */

/*
 * Copyright (c) 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
27oct15,yjw created (US64528)
*/

Component DRV_CACHE_TI_IPU {
    NAME        TI Sitara IPU unified Cache
    SYNOPSIS    TI Sitara IPU unified Cache driver
    LINK_SYMS   vxbAmIPUCacheDrv
    REQUIRES    DRV_BUS_FDT_ROOT  \
		INCLUDE_CACHE_SUPPORT
    _CHILDREN   FOLDER_NOT_VISIBLE
}
