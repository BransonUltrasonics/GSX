/* 40vxbFdtPl310L2Cache.cdf - L2 cache PL310  */

/*
 * Copyright (c) 2013, 2015-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
21oct16,yya  refactored secure config (F6283)
22apr16,emj  CERT: Update to produce partially linked objects (US79607)
07apr16,gls  added function pointers for register accesss routines
10nov15,cfm  added CONFIGLETTES pl310Access.c and added INCLUDE_PL310_NEED_SMC (US69494)
24dec13,mpc  created
*/

Component DRV_CACHE_FDT_PL310 {
        NAME            FDT L2 Cache PL310
        SYNOPSIS        FDT L2 Cache PL310 driver
        CONFIGLETTES    pl310Access.c
        INIT_RTN        vxPl310AccessInit();
        INIT_BEFORE     INCLUDE_CACHE_ENABLE
        INIT_AFTER      INCLUDE_CPU_INIT
        _INIT_ORDER     usrInit
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
        MODULES         vxbFdtPl310L2Cache.o
        LINK_SYMS       vxbFdtPl310L2CacheDrv
#else
        _REQUIRES INCLUDE_KERNEL
#endif
        REQUIRES        DRV_BUS_FDT_ROOT 
        _CHILDREN       FOLDER_NOT_VISIBLE
}

