/* 40vxbTiMmchsCtrl.cdf - TI MMCHS Driver */

/*
 * Copyright (c) Branson.
 *
 */

/*
modification history
--------------------
06Apr18,r_r  add eMMC support - host controller
*/

Component DRV_EMMCHS_CTRL {
    NAME        EMMCHS host controller driver
    SYNOPSIS    EMMCHS host controller driver
    MODULES     vxbeMmchsCtrl.o
    LINK_SYMS   vxbeMmchsCtrlDrv
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_ERF \
                INCLUDE_EMMC_BUS \
                INCLUDE_GPIO_SYS \
                INCLUDE_PINMUX_SYS \
                INCLUDE_DMA_SYS
    _CHILDREN   FOLDER_DRIVERS 
}
