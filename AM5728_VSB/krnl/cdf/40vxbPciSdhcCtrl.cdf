/* 40vxbPciSdhcCtrl.cdf - PCI SDHC Driver */

/*
 * Copyright (c) 2014 - 2015, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
04feb17,mw1  include INCLUDE_CACHE_DMA32_LIB in IA32_PAE (V7STO-764)
20nov15,myt  adapt to LP64 (V7STO-484)
16nov15,myt  add the support of SD/eMMC on IA arch target for 
             vxBus gen2 (V7STO-399)
26oct15,myt  add the support of reliable write for eMMC (V7STO-500)
07may14,e_d  written. (US29286)
*/

#ifdef _WRS_CONFIG_VXBUS
Component INCLUDE_SDMMC_CFG {
    NAME        PCI_IMX_SDHC config
    SYNOPSIS    PCI_IMX_SDHC config
    _CHILDREN   FOLDER_SDMMC
    CONFIGLETTES usrSdMmc.c
    _INIT_ORDER usrRoot
    INIT_RTN    usrSdMmcInit();
    INIT_AFTER  usrIosCoreInit
    INIT_BEFORE usrIosExtraInit
}
Component DRV_PCI_SDHC_CTRL {
    NAME        PCI SDHC host controller driver
    SYNOPSIS    PCI SDHC host controller driver
    _CHILDREN   FOLDER_SDMMC
    LINK_SYMS   vxbPciSdhcCtrlDrv
    REQUIRES    INCLUDE_VXBUS    \
                INCLUDE_SD_BUS   \
                INCLUDE_SDMMC_CFG
}

#if defined (_WRS_CONFIG_LP64) || defined (_WRS_CONFIG_IA32_PAE)
Component DRV_PCI_SDHC_CTRL {
    REQUIRES    += INCLUDE_CACHE_DMA32_LIB
}
#endif    /* _WRS_CONFIG_LP64 || _WRS_CONFIG_IA32_PAE */

#endif /* _WRS_CONFIG_VXBUS */

#ifdef _WRS_CONFIG_VXBUS_LEGACY
Component DRV_PCI_SDHC_CTRL {
    NAME        PCI SDHC host controller driver
    SYNOPSIS    PCI SDHC host controller driver
    MODULES     vxbPciSdhcCtrl.o
    CONFIGLETTES usrSdMmc.c
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PCI_BUS \
                INCLUDE_ERF \
                INCLUDE_SD_BUS
    _CHILDREN   FOLDER_SDMMC
    INIT_RTN    pciSdhcRegister();\
                usrSdMmcInit();
    PROTOTYPE   void pciSdhcRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PCI_BUS  \
                INCLUDE_SD_BUS
}
#endif
