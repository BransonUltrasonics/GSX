/* bsp.cdf - Board definition description file */

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
21oct16,yya  refactored secure mode interface (F6283)
11nov15,cfm  added SMC related components (US69494)
27aug15,sye  added BUILD_TARGETS. (V7PRO-2376)
23oct14,sye  updated to only support default BUILD_SPECS. (V7PRO-1394)
01jul14,swu  created (US40646)
*/

Bsp ti_sitara_ctxa9_branson {
    NAME                board support package
    CPU                 ARMARCH7
    ENDIAN              little
    BUILD_SPECS         default
    BUILD_TARGETS       uVxWorks vxWorks vxWorks.bin
    FP                  vector soft
    REQUIRES            INCLUDE_KERNEL                  \
                        INCLUDE_CPU_INIT                \
                        INCLUDE_MEM_DESC                \
                        INCLUDE_BOARD_LIB_INIT          \
                        DRV_BUS_FDT_SIMPLE              \
                        INCLUDE_EARLY_MMU               \
                        DRV_INTCTLR_FDT_ARM_GIC         \
                        DRV_TIMER_FDT_ARM_GTC           \
                        DRV_TIMER_FDT_OMAP3             \
                        DRV_SIO_TI_AM3                  \
                        DRV_CLK_FDT_AM4                 \
                        INCLUDE_TIMER_SYS               \
                        BOARD_SELECTION                 \
                        INCLUDE_CACHE_SUPPORT           \
                        DRV_PINMUX_FDT_AM3              \
                        INCLUDE_ACTLR_NEED_SMC          \
                        INCLUDE_PL310_NEED_SMC			\
						BRANSON_DRV_I2C_TPS62362_VOLTAGE_REGULATOR		\
						BRANSON_DRV_I2C_PLL_CLK_SYNTH		\
						BRANSON_DRV_END_FDT_TI_PRU_ESC		\
						BRANSON_INCLUDE_LEDDRV		\
						BRANSON_DRV_EPWM		\
						BRANSON_DRV_EQEP		
						
						
			
}
