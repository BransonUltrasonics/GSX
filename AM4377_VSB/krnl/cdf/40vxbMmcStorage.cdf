/* 40vxbMmcStorage.cdf - MMC card Driver */

/*
 * Copyright (c) 2012, 2014 - 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
46apr15,myt  add hardWareInterFaceBusInit (V7STO-311)
27may14,e_d  add requires with xbd. (V7STO-167)
07may14,e_d  add vxbus Gen2 model support. (US29286)
04jan12,e_d  written.
*/

#ifdef _WRS_CONFIG_VXBUS
Component DRV_MMCSTORAGE_CARD {
    NAME        MMC card driver
    SYNOPSIS    MMC card driver
    _CHILDREN   FOLDER_SDMMC 
    LINK_SYMS   vxbMmcStorageDrv
    REQUIRES    INCLUDE_VXBUS  \
                INCLUDE_SD_BUS \
                INCLUDE_XBD
}
#endif /* _WRS_CONFIG_VXBUS */

#ifdef _WRS_CONFIG_VXBUS_LEGACY
Component DRV_MMCSTORAGE_CARD {
    NAME        MMC card driver
    SYNOPSIS    MMC card driver
    MODULES     vxbMmcStorage.o
    _INIT_ORDER hardWareInterFaceBusInit
    REQUIRES    INCLUDE_VXBUS  \
                INCLUDE_SD_BUS \
                INCLUDE_ERF    \
                INCLUDE_XBD
    _CHILDREN   FOLDER_SDMMC
    INIT_RTN    vxbMmcStorageRegister();
    PROTOTYPE   void vxbMmcStorageRegister(void);
    INIT_AFTER  INCLUDE_SD_BUS
}
#endif /* _WRS_CONFIG_VXBUS_LEGACY */
