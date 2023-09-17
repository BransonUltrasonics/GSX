/* 40vxbFslImxSdhcCtrl.cdf - Freescale iMX SDHC Driver */

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
27may14,e_d  add requires with gpio and pin mux. (V7STO-167)
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
Component DRV_IMX_SDHC_CTRL {
    NAME        FSL IMX SDHC host controller driver
    SYNOPSIS    FSL IMX SDHC host controller driver
    _CHILDREN   FOLDER_SDMMC
    LINK_SYMS   vxbImxSdhcCtrlDrv
    REQUIRES    INCLUDE_VXBUS    \
                INCLUDE_SD_BUS   \
                INCLUDE_ERF      \
                INCLUDE_GPIO_SYS \
                INCLUDE_PINMUX_SYS \
                INCLUDE_SDMMC_CFG
}
#endif /* _WRS_CONFIG_VXBUS */

#ifdef _WRS_CONFIG_VXBUS_LEGACY
Component DRV_IMX_SDHC_CTRL {
    NAME        FSL IMX SDHC host controller driver
    SYNOPSIS    FSL IMX SDHC host controller driver
    MODULES     vxbFslImxSdhcCtrl.o
    CONFIGLETTES usrSdMmc.c
    REQUIRES    INCLUDE_VXBUS   \
                INCLUDE_PLB_BUS \
                INCLUDE_SD_BUS  \
                INCLUDE_ERF

    _INIT_ORDER hardWareInterFaceBusInit
    _CHILDREN   FOLDER_SDMMC
    INIT_RTN    imxSdhcRegister();\
                usrSdMmcInit();
    PROTOTYPE   void imxSdhcRegister(void);
    INIT_AFTER  INCLUDE_PLB_BUS  \
                INCLUDE_SD_BUS 
}
#endif /* _WRS_CONFIG_VXBUS_LEGACY */
