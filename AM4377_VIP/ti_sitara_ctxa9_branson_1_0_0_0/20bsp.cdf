/* 20board.cdf - Board component description file */

/*
 * Copyright (c) 2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
07dec16,mca  Break hard dependency between END and IPNET (US85582)
21oct16,yya  refactored secure mode interface (F6283)
17may16,sye  use GTC as timestamp by default (V7PRO-3079)
05jan16,m_w  replace DRV_VXBEND_TI_CPSW with DRV_END_FDT_TI_CPSW (V7PRO-2729)
25nov15,yya  updated default IMA_SIZE (V7PRO-2531)
10oct15,sye  updated INCLUDE_END dependency (V7PRO-2512)
01jul14,swu  created (US40646)
*/

Component DRV_I2C_EEPROM {
    REQUIRES    DRV_AM38XX_I2C INCLUDE_EEPROMDRV
    _REQUIRES   INCLUDE_EEPROMDRV
}
/*Component DRV_AM57XX_SPI {*/
Component DRV_AM43XX_SPI {
	INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_L2_CACHE {
    NAME        L2 cache support
    SYNOPSIS    include L2 cache support
    _CHILDREN   FOLDER_MEMORY
    REQUIRES    INCLUDE_CACHE_SUPPORT           \
                DRV_CACHE_FDT_PL310
    INCLUDE_WHEN  INCLUDE_CACHE_SUPPORT
}

Component BRANSON_DRV_I2C_TPS62362_VOLTAGE_REGULATOR {
    INCLUDE_WHEN    INCLUDE_END
}

Component DRV_I2C_PCFRTC {
    INCLUDE_WHEN    INCLUDE_END
}
Component BRANSON_DRV_I2C_PLL_CLK_SYNTH {
    INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_SHOW_ROUTINES {
    REQUIRES += DRV_I2C_EEPROM
}

Component INCLUDE_END {
    REQUIRES += INCLUDE_BOOT_LINE_INIT       \
                INCLUDE_IPNET_IFCONFIG_1
}

Component DRV_END_FDT_TI_CPSW {
    REQUIRES DRV_VXBEND_TI_CPSW              \
             INCLUDE_IPWRAP_IFSHOW           \
             INCLUDE_IFCONFIG
    INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_MICREL_PHY {
    INCLUDE_WHEN    INCLUDE_END
}

Component BRANSON_DRV_END_FDT_TI_PRU_ESC {
	REQUIRES BRANSON_DRV_END_FDT_TI_PRUSS	\
			BRANSON_DRV_END_FDT_TI_PRU_ESC	\
			BRANSON_DRV_CORE_END_FDT_TI_PRUSS	\
			BRANSON_DRV_INTC_END_FDT_TI_PRUSS	\
			BRANSON_DRV_MDIO_END_FDT_TI_PRUSS	\
			BRANSON_INCLUDE_PRUDRV
			
    INCLUDE_WHEN    INCLUDE_END
}

Component BRANSON_INCLUDE_LEDDRV {
    REQUIRES 		BRANSON_DRV_I2C_LED
    INCLUDE_WHEN    INCLUDE_END
}

Component DRV_TI_MMCHS_CTRL {
    REQUIRES  +=  INCLUDE_VXBUS \
                INCLUDE_ERF \
                INCLUDE_SD_BUS \
                INCLUDE_GPIO_SYS \
                INCLUDE_PINMUX_SYS \
                INCLUDE_DMA_SYS \
                INCLUDE_SDMMC_CFG
	INCLUDE_WHEN    INCLUDE_END
}

Component DRV_SDSTORAGE_CARD {
    REQUIRES  +=  INCLUDE_VXBUS  \
                INCLUDE_SD_BUS \
                INCLUDE_XBD
	INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_DOSFS_FAT {
    REQUIRES    +=    INCLUDE_DOSFS_MAIN
	INCLUDE_WHEN    INCLUDE_END
}

Component BRANSON_DRV_EPWM {
    REQUIRES    +=    BRANSON_DRV_EHRPWM
	INCLUDE_WHEN    INCLUDE_END
}

Component BRANSON_DRV_EQEP {
	INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_GPIO_SYS {
    REQUIRES += DRV_GPIO_FDT_OMAP
}

Component INCLUDE_DMA_SYS {
    REQUIRES += DRV_TI_EDMA3
}

Selection BOARD_SELECTION {
    NAME        board selection
    COUNT       1-1
    CHILDREN    INCLUDE_TI_AM437X_EVM
    DEFAULTS    INCLUDE_TI_AM437X_EVM
    _CHILDREN   FOLDER_BSP_CONFIG
}

Component INCLUDE_TI_AM437X_EVM {
    NAME        TI AM437x Evaluation Module
    SYNOPSIS    TI AM437x Evaluation Module support.
}

Parameter LOCAL_MEM_PHYS_ADRS {
    NAME        local memory physical base address
    SYNOPSIS    Base physical address.
    DEFAULT     0x80000000
}

Parameter DTS_FILE {
    NAME        dts file name to be used
    DEFAULT     (INCLUDE_TI_AM437X_EVM)::(am437x-idk-UW.dts)
}

Parameter IMA_SIZE {
    NAME        IMA(Initial Mapped Area) size
    SYNOPSIS    IMA(Initial Mapped Area) will be mapped to MMU for \
                the early initialization phase before usrMmuInit(), \
                so the size should be large enough to hold the entire VxWorks Kernel image.
    DEFAULT     0x80000000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel entry address
    DEFAULT     (INCLUDE_BOOT_APP)::(0x02000000) \
                0x00100000
}

Parameter KERNEL_LOAD_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_APP)::(0x82000000) \
                0x80100000
}

Parameter RAM_HIGH_ADRS {
    NAME        Runtime kernel high address
    DEFAULT     (INCLUDE_BOOT_APP)::(0x28000000) \
                0x01000000
}

Parameter VEC_BASE_ADRS {
    NAME        Vector base address
    SYNOPSIS    Vector base address
    DEFAULT     ((char *) (LOCAL_MEM_LOCAL_ADRS) + 0x20000)
}

Parameter DTB_RELOC_ADDR {
    NAME        DTB relocateds address
    SYNOPSIS    DTB(Device Tree Blob) need be relocated to one safe address
                to avoid be overwritten, so it should be below RAM_LOW_ADRS
                and the reserved start region, and enough for the entire
                DTB.
    TYPE        void *
    DEFAULT     (LOCAL_MEM_LOCAL_ADRS + 0x10000)
}

Parameter ISR_STACK_SIZE {
    NAME        ISR stack size
    SYNOPSIS    ISR Stack size (bytes)
    DEFAULT     0x2000
}

Parameter DEFAULT_BOOT_LINE {
    NAME        default boot line
    SYNOPSIS    Default boot line string
    TYPE        string
    DEFAULT     "cpsw(0,0)host:vxWorks h=192.168.1.1 e=192.168.1.100 u=target pw=vxTarget f=0x0"
}

Parameter CONSOLE_TTY {
    NAME        console serial port
    DEFAULT     0
}

Parameter NUM_TTY {
     NAME       number of serial ports
     DEFAULT    1
}

Parameter CONSOLE_BAUD_RATE {
    NAME        baud rate of console port
    DEFAULT     115200
}

Profile BSP_DEFAULT {
    PROFILES    PROFILE_OS_DEFAULT
}

Parameter SYSCLK_TIMER_NAME {
    NAME        System Clock Device Name (NULL is auto-assign)
    TYPE        string
    DEFAULT     "omap35xxTimer"
}

Parameter SYSCLK_TIMER_NUM {
    NAME        System Clock Timer Number
    TYPE        int
    DEFAULT     1
}

Component DRV_AUDIO_TI_AIC3106 {
    REQUIRES += DRV_AM38XX_I2C
}

Component DRV_TIMER_FDT_ARM_GTC {
    INCLUDE_WHEN    INCLUDE_VXB_TIMESTAMP
}

Parameter TIMESTAMP_TIMER_NAME {
    NAME        Timestamp Device Name (NULL is auto-assign)
    TYPE        string
    DEFAULT     (DRV_TIMER_FDT_ARM_GTC)::("armGtc") \
                NULL
}

Selection SELECT_SECURE_CONFIG {
    NAME            ARM secure mode configuration
    SYNOPSIS        ARM secure mode configuration
    COUNT           1-1
    _REQUIRES       INCLUDE_CPU_INIT_ARCH
    _CHILDREN       FOLDER_BSP_CONFIG
    CHILDREN        INCLUDE_SECURE_CONFIG_TISM
    DEFAULTS        INCLUDE_SECURE_CONFIG_TISM
}

Component INCLUDE_SECURE_CONFIG_STANDARD {
	REQUIRES        COMPONENT_NOT_SUPPORTED
}

Component INCLUDE_SECURE_CONFIG_TISM {
	REQUIRES        -= COMPONENT_NOT_SUPPORTED
}
