/* 40vxbZynqSdhcCtrl.cdf - Zynq SDHC Driver */

/*
 * Copyright (c) 2014 - 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26oct15,myt  add the support of reliable write for eMMC (V7STO-500)
07aug14,myt add sd support for zynq. (US43873)
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

Component DRV_ZYNQ_SDHC_CTRL {
    NAME        ZYNQ SDHC host controller driver
    SYNOPSIS    ZYNQ SDHC host controller driver
    _CHILDREN   FOLDER_SDMMC 
    LINK_SYMS   vxbZynqSdhcCtrlDrv
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_SD_BUS \
                INCLUDE_SDMMC_CFG
}
#endif

#ifdef _WRS_CONFIG_VXBUS_LEGACY
Component DRV_ZYNQ_SDHC_CTRL {
    NAME        ZYNQ SDHC host controller driver
    SYNOPSIS    ZYNQ SDHC host controller driver
    MODULES     vxbZynqSdhcCtrl.o
    CONFIGLETTES usrSdMmc.c
    REQUIRES    INCLUDE_VXBUS   \
                INCLUDE_PLB_BUS \
                INCLUDE_SD_BUS  \
                INCLUDE_ERF
    _INIT_ORDER hardWareInterFaceBusInit
    _CHILDREN   FOLDER_SDMMC
    INIT_RTN    zynqSdhcRegister();\
                usrSdMmcInit();
    PROTOTYPE   void zynqSdhcRegister(void);
    INIT_AFTER  INCLUDE_PLB_BUS  \
                INCLUDE_SD_BUS 
}
#endif
