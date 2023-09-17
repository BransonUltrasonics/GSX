/* 20board.cdf - Board component description file */

/*
 * Copyright (c) 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
30Oct2017,l fli added Hilscher driver, also added back DRV_AM57XX_SPI
24sep15,yjw  created (US64529)
*/

Component DRV_HILSCHER_CIFX {
	INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_MICREL_PHY {
    INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_USB_SHOW {
    INCLUDE_WHEN    INCLUDE_END
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

Component DRV_AM57XX_SPI {
	INCLUDE_WHEN    INCLUDE_END
}

Component DRV_I2C_RTC {
	REQUIRES  DRV_FDT_RTC_AM3 
    INCLUDE_WHEN    INCLUDE_END
}

//Component INCLUDE_FBDEV_TIDSS_0 {
//	REQUIRES  INCLUDE_FBDEV_MEMORY		\
//			  INCLUDE_FBDEV_SPLASH		\
//			  INCLUDE_TILCON_DEMO		\
//			  INCLUDE_TILCON_KERNEL		\
//			  INCLUDE_USB_PHY_FSL_ONCHIP
//			  INCLUDE_WHEN    INCLUDE_END
//}
Component BRANSON_INCLUDE_LEDDRV {
    REQUIRES BRANSON_DRV_I2C_LED 				\
			 DRV_AM38XX_I2C
    INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_USB_GEN2_MOUSE {
	REQUIRES += INCLUDE_USB	\
				INCLUDE_USB_GEN2_HELPER	
				INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_USB_GEN2_MOUSE_INIT {
	REQUIRES += INCLUDE_USB_GEN2_MOUSE	\
				INCLUDE_USB_HOST_CLASS_INIT	\
				INCLUDE_USB_INIT	\
				INCLUDE_USB_TRACK_GEN2_MSE
				INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_USB_XHCI_HCD   {
	REQUIRES += INCLUDE_DMA_SYS	\
				INCLUDE_USBD 
				INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_USB_XHCI_HCD_INIT    {
	REQUIRES  INCLUDE_HCD_BUS
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

Component BRANSON_DRV_AM5728_EHRPWM {
/*    REQUIRES    +=    BRANSON_DRV_EHRPWM */
	INCLUDE_WHEN    INCLUDE_END
}
Component INCLUDE_GPIO_SYS {
    REQUIRES += DRV_GPIO_FDT_OMAP
}

Component INCLUDE_DMA_SYS {
    REQUIRES += DRV_TI_EDMA3
}

Component INCLUDE_USB_GEN2_STORAGE_INIT {
		INCLUDE_WHEN	INCLUDE_END
}


Parameter LOCAL_MEM_PHYS_ADRS {
    NAME        local memory physical base address
    SYNOPSIS    Base physical address. For LPAE, alignment of address must be 2MB.
    DEFAULT     0x84000000
}

Parameter LOCAL_MEM_SIZE {
    NAME        local memory size
    SYNOPSIS    Fixed (static) memory size
    DEFAULT     0x7c000000
}

Parameter DTS_FILE {
    NAME        dts file name to be used
    DEFAULT     am572x_idk_ca15.dts
}

Parameter IMA_SIZE {
    NAME        IMA(Initial Mapped Area) size
    SYNOPSIS    Initial Mapped Area size.       
    DEFAULT     0x10000000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel entry address
    DEFAULT     (INCLUDE_BOOT_APP)::(0x02000000) \
                0x00100000
}

Parameter KERNEL_LOAD_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_APP)::(0x86000000) \
                0x84100000
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