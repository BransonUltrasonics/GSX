/* 40vxbTiMmchsCtrl.cdf - TI MMCHS Driver */

/*
 * Copyright (c) 2013-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26oct15,myt  add the support of reliable write for eMMC (V7STO-500)
18jun14,lxj  add requires for DRV_TI_EDMA3 in legacy mode. (V7STO-177)
13may14,lxj  add vxbus Gen2 model support. (US29286)
18sep13,e_d  written.
*/

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

Component DRV_TI_MMCHS_CTRL {
    NAME        TI MMCHS host controller driver
    SYNOPSIS    TI MMCHS host controller driver
    _CHILDREN   FOLDER_SDMMC

#ifdef _WRS_CONFIG_VXBUS_LEGACY 
    MODULES     vxbTiMmchsCtrl.o
    CONFIGLETTES usrSdMmc.c
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS  \
                INCLUDE_ERF \
                DRV_TI_EDMA3 \
                INCLUDE_SD_BUS
    INIT_RTN    tiMmchsRegister(); \
                usrSdMmcInit();
    PROTOTYPE   void tiMmchsRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS  \
                DRV_TI_EDMA3 \
                INCLUDE_SD_BUS
#else
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_ERF \
                INCLUDE_SD_BUS \
                INCLUDE_GPIO_SYS \
                INCLUDE_PINMUX_SYS \
                INCLUDE_DMA_SYS \
                INCLUDE_SDMMC_CFG
    LINK_SYMS   vxbTiMmchsCtrlDrv
#endif /* _WRS_CONFIG_VXBUS_LEGACY */
}
