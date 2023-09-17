/* 40vxbeMmcStorage.cdf - EMMC card Driver */

/*
 * Copyright (c) Branson.
 *
 */

/*
modification history
--------------------
06Apr18,r_r  add eMMC support - storage
*/

Component DRV_EMMC_STORAGE {
    NAME        EMMC card driver
    SYNOPSIS    EMMC card driver
    LINK_SYMS   vxbeMmcStorageDrv
    REQUIRES    INCLUDE_VXBUS  \
                INCLUDE_EMMC_BUS \
                INCLUDE_XBD
    _CHILDREN   FOLDER_DRIVERS 
}
