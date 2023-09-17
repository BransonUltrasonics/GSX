/* bsp.cdf - Board definition description file */

/*
 * Copyright (c) 2015,2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
04may16,m_w  update clock system, replace DRV_CLK_FDT_AM5
             with DRV_CLK_FDT_AM4. (F7158)
26sep15,yjw  created (US64529)
*/

Bsp ti_sitara_ctxa15_branson {
    NAME                am572x board support package
    CPU                 ARMARCH7
    ENDIAN              little
    MP_OPTIONS          SMP
    BUILD_SPECS         default
    BUILD_TARGETS       uVxWorks vxWorks vxWorks.bin
    FP                  vector soft
    REQUIRES            INCLUDE_KERNEL                  \
                        INCLUDE_CPU_INIT                \
                        INCLUDE_MEM_DESC                \
                        INCLUDE_BOARD_LIB_INIT          \
                        DRV_BUS_FDT_SIMPLE              \
                        INCLUDE_EARLY_MMU               \
                        DRV_PINMUX_FDT_AM3              \
                        DRV_CLK_FDT_AM4                 \
                        DRV_INTCTLR_FDT_ARM_GIC         \
						DRV_ARMV7_GEN_TIMER             \
                        DRV_TIMER_FDT_OMAP3             \
                        DRV_SIO_TI_AM3                  \
                        INCLUDE_TIMER_SYS               \
                        BOARD_SELECTION                 \
                        INCLUDE_CACHE_SUPPORT
}
