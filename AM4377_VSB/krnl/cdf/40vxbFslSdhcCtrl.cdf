/* 40vxbFslSdhcCtrl.cdf - Freescale SDHC Driver */

/*
 * Copyright (c) 2014 - 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
30jun16,zly  fix typo issue. (V7STO-662)
21jun16,zly  include DMA32 when driver needed in LP64 mode.
26oct15,myt  add the support of reliable write for eMMC (V7STO-500)
07may14,e_d  add vxbus Gen2 model support. (US29286)
*/

#ifdef _WRS_CONFIG_VXBUS
Component INCLUDE_SDMMC_CFG {
    NAME        FSL_IMX_SDHC config
    SYNOPSIS    FSL_IMX_SDHC config
    _CHILDREN   FOLDER_SDMMC
    CONFIGLETTES usrSdMmc.c
    _INIT_ORDER usrRoot
    INIT_RTN    usrSdMmcInit();
    INIT_AFTER  usrIosCoreInit
    INIT_BEFORE usrIosExtraInit
}

Component DRV_FSL_SDHC_CTRL {
    NAME        FSL SDHC host controller driver
    SYNOPSIS    FSL SDHC host controller driver
    _CHILDREN   FOLDER_SDMMC 
    LINK_SYMS   vxbFslSdhcCtrlDrv
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_SD_BUS \
                INCLUDE_SDMMC_CFG
}
#endif

#ifdef _WRS_CONFIG_VXBUS_LEGACY
Component DRV_FSL_SDHC_CTRL {
    NAME        FSL SDHC host controller driver
    SYNOPSIS    FSL SDHC host controller driver
    MODULES     vxbFslSdhcCtrl.o
    CONFIGLETTES usrSdMmc.c
    REQUIRES    INCLUDE_VXBUS   \
                INCLUDE_PLB_BUS \
                INCLUDE_SD_BUS  \
                INCLUDE_ERF
    _INIT_ORDER hardWareInterFaceBusInit
    _CHILDREN   FOLDER_SDMMC
    INIT_RTN    fslSdhcRegister(); \
                usrSdMmcInit();
    PROTOTYPE   void fslSdhcRegister(void);
    INIT_AFTER  INCLUDE_PLB_BUS  \
                INCLUDE_SD_BUS 
}
#endif

#ifdef    _WRS_CONFIG_LP64
Component DRV_FSL_SDHC_CTRL {
    REQUIRES += INCLUDE_CACHE_DMA32_LIB
}
#endif    /* _WRS_CONFIG_LP64 */
