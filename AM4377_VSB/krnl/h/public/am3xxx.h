/* am3xxx.h - header files for TI AM3XXX processor */

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
09sep15,yya  added warm reboot support. (US63670)
15sep14,swu  updated PSL definition. (V7PRO-1243)
22may14,g_x  Remove reference to BOARD_FUNC_TBL. (V7PRO-882)
13may14,c_t  update function defines for US38742.
13dec13,c_t  derived from ti_am335x_evm bsp.
*/

#ifndef __INCam3xxxh
#define __INCam3xxxh

#ifdef __cplusplus
extern "C" {
#endif

/* Handy sizes */

#define SZ_4K                           (0x00001000)
#define SZ_8K                           (0x00002000)
#define SZ_16K                          (0x00004000)
#define SZ_32K                          (0x00008000)
#define SZ_64K                          (0x00010000)
#define SZ_128K                         (0x00020000)
#define SZ_256K                         (0x00040000)
#define SZ_512K                         (0x00080000)

#define SZ_1M                           (0x00100000)
#define SZ_2M                           (0x00200000)
#define SZ_4M                           (0x00400000)
#define SZ_8M                           (0x00800000)
#define SZ_16M                          (0x01000000)
#define SZ_32M                          (0x02000000)
#define SZ_64M                          (0x04000000)
#define SZ_128M                         (0x08000000)
#define SZ_256M                         (0x10000000)
#define SZ_512M                         (0x20000000)

#define SZ_1G                           (0x40000000)
#define SZ_2G                           (0x80000000)

/* 48 MHz input clock */

#define UART_CLK_FREQ                   (48000000)
#define SYS_32K_CLK_FREQ                (32000)
#define SYS_CLK_FREQ                    (24000000)


#define AM3XXX_CLK_SOURCE               AM3XXX_CLK_SOURCE_32K
#define AM3XXX_SYS_TIMER_CLK_FREQ       (24000000)
#define AM3XXX_AUX_TIMER_CLK_FREQ       (24000000)

#define MPU_INTERRUPT_NUM               (128)

/* TI AM3XXX Board Registers Offset */

#define AM3XXX_SRAM0_START              (0x40300000)

#define AM3XXX_GPTIMER0_BASE            (0x44E05000)
#define AM3XXX_GPTIMER1_BASE            (0x44E31000)  /* 1ms timer */
#define AM3XXX_GPTIMER2_BASE            (0x48040000)
#define AM3XXX_GPTIMER3_BASE            (0x48042000)
#define AM3XXX_GPTIMER4_BASE            (0x48044000)
#define AM3XXX_GPTIMER5_BASE            (0x48046000)
#define AM3XXX_GPTIMER6_BASE            (0x48048000)

#define AM3XXX_GPTIMER7_BASE            (0x4804A000)

#define AM3XXX_GPIO0_BASE               (0x44E07000)
#define AM3XXX_GPIO1_BASE               (0x4804C000)
#define AM3XXX_GPIO2_BASE               (0x481AC000)
#define AM3XXX_GPIO3_BASE               (0x481AE000)

#define AM3XXX_ELM_BASE                 (0x48080000)
#define AM3XXX_EMIF4_0_CFG_BASE         (0x4C000000)
#define AM3XXX_EMIF4_1_CFG_BASE         (0x4D000000)
#define AM3XXX_DMM_BASE                 (0x4E000000)
#define AM3XXX_GPMC_BASE                (0x50000000)
#define AM3XXX_PLL_SUBSYS_BASE          (0x481C5000)

#define AM3XXX_PRCM_BASE                (0x44E00000)

#define AM3XXX_PRCM_SIZE                (0x00004000)

#define AM3XXX_SDRC_NCS0                (0x80000000)
#define AM3XXX_SDRC_NCS0_END            (0xffffffff)
#define AM3XXX_SDRC_NCS0_SIZE           (SZ_2G)

#define AM3XXX_UART0_BASE               (0x44e09000)
#define AM3XXX_UART0_END                (0x44e0afff)
#define AM3XXX_UART0_SIZE               (SZ_8K)

#define AM3XXX_UART1_BASE               (0x48022000)
#define AM3XXX_UART1_END                (0x48023fff)
#define AM3XXX_UART1_SIZE               (SZ_8K)

#define AM3XXX_UART2_BASE               (0x48024000)
#define AM3XXX_UART2_END                (0x48025fff)
#define AM3XXX_UART2_SIZE               (SZ_8K)

#define AM3XXX_UART3_BASE               (0x481a6000)
#define AM3XXX_UART3_END                (0x481a7fff)
#define AM3XXX_UART3_SIZE               (SZ_8K)

#define AM3XXX_I2C0_BASE                (0x44e0b000)
#define AM3XXX_I2C0_END                 (0x44e0bfff)
#define AM3XXX_I2C0_SIZE                (SZ_4K)

#define AM3XXX_I2C1_BASE                (0x4802a000)
#define AM3XXX_I2C1_END                 (0x4802afff)
#define AM3XXX_I2C1_SIZE                (SZ_4K)

#define AM3XXX_I2C2_BASE                (0x4819c000)
#define AM3XXX_I2C2_END                 (0x4819cfff)
#define AM3XXX_I2C2_SIZE                (SZ_4K)

#define AM3XXX_DCAN0_BASE               (0x481CC000)
#define AM3XXX_DCAN0_END                (0x481CDFFF)
#define AM3XXX_DCAN0_SIZE               (SZ_8K)

#define AM3XXX_DCAN1_BASE               (0x481D0000)
#define AM3XXX_DCAN1_END                (0x481D1FFF)
#define AM3XXX_DCAN1_SIZE               (SZ_8K)

#define AM3XXX_RTC_BASE                 (0x44E3E000)
#define AM3XXX_RTC_SIZE                 (0x00002000)

#define AM3XXX_SPI0_BASE                (0x48030000)
#define AM3XXX_SPI0_END                 (0x48030fff)
#define AM3XXX_SPI0_SIZE                (SZ_4K)

#define AM3XXX_SPI1_BASE                (0x481A0000)
#define AM3XXX_SPI1_END                 (0x481A0fff)
#define AM3XXX_SPI1_SIZE                (SZ_4K)

#define AM3XXX_MMC0_BASE                (0x48060000)
#define AM3XXX_MMC0_SDHC_BASE           (AM3XXX_MMC0_BASE + 0x100)
#define AM3XXX_MMC0_END                 (0x48060fff)
#define AM3XXX_MMC0_SIZE                (SZ_4K)

#define AM3XXX_USBSS_BASE               (0x47400000)
#define AM3XXX_USB0_BASE                (AM3XXX_USBSS_BASE + 0x1000)
#define AM3XXX_USB1_BASE                (AM3XXX_USBSS_BASE + 0x1800)

#define AM3XXX_MPU_INTC                 (0x48200000)
#define OMAP_INTCPS_MIR(n)              (0x0084 + (0x20 * (n)))

#define AM3XXX_CTRL_BASE                (0x44E10000)

#define AM3XXX_GPMC_BASE                (0x50000000)
#define AM3XXX_GPMC_END                 (0x50ffffff)
#define AM3XXX_GPMC_SIZE                (SZ_16M)

#define AM3XXX_EMAC0_BASE               (0x4A100000)
#define AM3XXX_EMAC0_END                (0x4A11ffff)
#define AM3XXX_EMAC0_SIZE               (SZ_128K)

#define AM3XXX_EMAC1_BASE               (0x4A120000)
#define AM3XXX_EMAC1_END                (0x4A13ffff)
#define AM3XXX_EMAC1_SIZE               (SZ_128K)

#define AM3XXX_SGX_BASE                 (0x56000000)
#define AM3XXX_SGX_SIZE                 (SZ_64K)

#define AM3XXX_EDMA_BASE                (0x49000000)
#define AM3XXX_EDMA_SIZE                (SZ_16M)

#define AM3XXX_MAILBOX_BASE             (0x480C8000)
#define AM3XXX_MAILBOX_SIZE             (SZ_8K)

#define AM3XXX_SPINLOCK_BASE            (0x480CA000)
#define AM3XXX_SPINLOCK_SIZE            (SZ_8K)

#define AM3XXX_LCDC_BASE                (0x4830E000)
#define AM3XXX_LCDC_SIZE                (SZ_8K)

#define CLKSEL_TIMER2_CLK               (AM3XXX_PRCM_BASE + 0x500 + 0x8)
#define AM3XXX_PRM_DEVICE_BASE          (AM3XXX_PRCM_BASE + 0xF00)
#define AM3XXX_PRM_RSTCTRL              (AM3XXX_PRM_DEVICE_BASE)
#define AM3XXX_PRM_RSTTIME              (AM3XXX_PRM_DEVICE_BASE + 4)

#define AM3XXX_PRM_RSTCTRL_COLD         (2)
#define AM3XXX_PRM_RSTCTRL_WARM         (1)


/* OMAP GPMC domain definitions, mainly for NAND */

#define OMAP_GPMC_BASE                  (AM3XXX_GPMC_BASE)
#define OMAP_GPMC_REVISION              (0x0)
#define OMAP_GPMC_SYSCONFIG             (0x10)
#define OMAP_GPMC_IRQSTATUS             (0x18)
#define OMAP_GPMC_IRQENABLE             (0x1C)
#define OMAP_GPMC_TIMEOUT               (0x40)
#define OMAP_GPMC_CONFIG                (0x50)
#define OMAP_GPMC_STATUS                (0x54)

#define OMAP_GPMC_CS_CONFIG1(cs)        (0x60 + (0x30 * (cs)))
#define OMAP_GPMC_CS_CONFIG2(cs)        (0x64 + (0x30 * (cs)))
#define OMAP_GPMC_CS_CONFIG3(cs)        (0x68 + (0x30 * (cs)))
#define OMAP_GPMC_CS_CONFIG4(cs)        (0x6C + (0x30 * (cs)))
#define OMAP_GPMC_CS_CONFIG5(cs)        (0x70 + (0x30 * (cs)))
#define OMAP_GPMC_CS_CONFIG6(cs)        (0x74 + (0x30 * (cs)))
#define OMAP_GPMC_CS_CONFIG7(cs)        (0x78 + (0x30 * (cs)))

#define OMAP_GPMC_CS_NAND_CMD(cs)       (0x7C + (0x30 * (cs)))
#define OMAP_GPMC_CS_NAND_ADDR(cs)      (0x80 + (0x30 * (cs)))
#define OMAP_GPMC_CS_NAND_DATA(cs)      (0x84 + (0x30 * (cs)))

#define GPMC_NO_IDLE                    (0x8)
#define CPMC_IRQ_WAIT0EDGE              (0x100)
#define CPMC_IRQ_WAIT1EDGE              (0x200)
#define CPMC_CONFIG_WP                  (0x10)
#define CPMC_CONFIG_LIMITED_ADDR        (0x2)
#define GPMC_CS_VALID                   (0x40)

/* TI AM3XXX specific settings */

#define OMAP_NAND_GPMC_CONFIG1          (0x00000800)
#define OMAP_NAND_GPMC_CONFIG2          (0x001e1e00)
#define OMAP_NAND_GPMC_CONFIG3          (0x001e1e00)
#define OMAP_NAND_GPMC_CONFIG4          (0x16051807)
#define OMAP_NAND_GPMC_CONFIG5          (0x00151e1e)
#define OMAP_NAND_GPMC_CONFIG6          (0x16000f80)
#define OMAP_NAND_GPMC_CONFIG7          (0x00000008)

/* GPIO Registers Offset */

#define AM3XXX_GPIO_REVISION            (0x000)
#define AM3XXX_GPIO_SYSCONFIG           (0x010)
#define AM3XXX_GPIO_IRQSTATUS0_RAW      (0x024)
#define AM3XXX_GPIO_IRQSTATUS1_RAW      (0x028)
#define AM3XXX_GPIO_IRQSTATUS0          (0x02C)
#define AM3XXX_GPIO_IRQSTATUS1          (0x030)
#define AM3XXX_GPIO_IRQSTATUS_SET0      (0x034)
#define AM3XXX_GPIO_IRQSTATUS_SET1      (0x038)
#define AM3XXX_GPIO_IRQSTATUS_CLR0      (0x03C)
#define AM3XXX_GPIO_IRQSTATUS_CLR1      (0x040)
#define AM3XXX_GPIO_IRQWAKEN0           (0x044)
#define AM3XXX_GPIO_IRQWAKEN1           (0x048)

#define AM3XXX_GPIO_SYSSTATUS           (0x114)
#define AM3XXX_GPIO_CTRL                (0x130)
#define AM3XXX_GPIO_OE                  (0x134)
#define AM3XXX_GPIO_DATAIN              (0x138)
#define AM3XXX_GPIO_DATAOUT             (0x13C)
#define AM3XXX_GPIO_LEVELDETECT0        (0x140)
#define AM3XXX_GPIO_LEVELDETECT1        (0x144)
#define AM3XXX_GPIO_RISINGDETECT        (0x148)
#define AM3XXX_GPIO_FALLINGDETECT       (0x14C)
#define AM3XXX_GPIO_DEBOUNCENABLE       (0x150)
#define AM3XXX_GPIO_DEBOUNCINGTIME      (0x154)
#define AM3XXX_GPIO_CLEARDATAOUT        (0x190)
#define AM3XXX_GPIO_SETDATAOUT          (0x194)

/* TI AM3XXX Board Configuration Stuff */

#define CONFIG_NAND_SIZE_256MB          (0x0) /* 256MB */
#define NAND_SIZE_256MB                 (256 * 1024 * 1024)
#define CONFIG_NAND_BASE                (0x08000000)
#define GPMC_MAX_CS                     (8)

#define CONFIG_ROM_SIZE                 (0x65000)
#define CONFIG_ROM_OFFSET               (0x20000)

#ifndef BIT
#define BIT(x)                          (1 << (x))
#endif

#define CM_DPLL_OFFSET                  (AM3XXX_PRCM_BASE + 0x0500)

#define AM3XXX_CM_TIMER1_CLKSEL         (CM_DPLL_OFFSET + 0x24)
#define AM3XXX_CM_TIMER2_CLKSEL         (CM_DPLL_OFFSET + 0x08)
#define AM3XXX_CM_TIMER3_CLKSEL         (CM_DPLL_OFFSET + 0x0C)
#define AM3XXX_CM_TIMER4_CLKSEL         (CM_DPLL_OFFSET + 0x10)
#define AM3XXX_CM_TIMER5_CLKSEL         (CM_DPLL_OFFSET + 0x18)
#define AM3XXX_CM_TIMER6_CLKSEL         (CM_DPLL_OFFSET + 0x1C)
#define AM3XXX_CM_TIMER7_CLKSEL         (CM_DPLL_OFFSET + 0x04)

#define RMII_REFCLK_SRC                 (AM3XXX_PLL_SUBSYS_BASE + 0x2E8)
#define GMII_SEL                        (AM3XXX_CTRL_BASE + 0x650)
#define CPTS_RFT_CLK_SOURCE             (0x4)
#define REFCLK_SOURCE                   (0x0)

/* cache maintence operation */

#define ARM_IMM #

#define ARM_LOC_MASK #0x7000000     /* level of coherency mask of CLIDR */


#define CACHE_DC_ENABLE   (1<<2)    /* (data) cache enable */
#define CACHE_IC_ENABLE   (1<<12)   /* Instruction cache enable */

/*
 * Common code for cache operations on entire data/unified caches,
 * performed by set/way to the point of coherency (PoC).
 * This code is based on 'Example code for cache maintenance operations'
 * provided in "ARM Architecture Reference Manual ARMv7-A and ARMv7-R edition
 * (ARM DDI 0406)" .
 *
 * Registers used: r0-r8, though r4-r8 are saved/restored
 * Also, note that r0 is 0 when this code completes
 */

#define _CORTEX_AR_ENTIRE_DATA_CACHE_OP(crm) \
                    \
    MRC    p15, 1, r0, c0, c0, 1    /* r0 = Cache Lvl ID register info */;\
    ANDS   r3, r0, ARM_LOC_MASK     /* get level of coherency (LoC) */;\
    MOV    r3, r3, LSR ARM_IMM 23   /* r3 = LoC << 1 */;\
    BEQ    5f            ;\
                    \
    MOV    r7, ARM_IMM 0            /* r7 = cache level << 1; start at 0 */;\
                    \
1:                    ;\
    AND    r1, r0, ARM_IMM 0x7      /* r1 = cache type(s) for this level */;\
    CMP    r1, ARM_IMM 2        ;\
    BLT    4f                       /* no data cache at this level */;\
                    \
    MCR    p15, 2, r7, c0, c0, 0    /* select the Cache Size ID register */;\
    MCR    p15, 0, r7, c7, c5, 4    /* ISB: sync change to Cache Size ID */;\
    MRC    p15, 1, r1, c0, c0, 0    /* r1 = current Cache Size ID info */;\
    AND    r2, r1, ARM_IMM 0x7      /* r2 = line length */;\
    ADD    r2, r2, ARM_IMM 4        /* add line length offset = log2(16 bytes) */;\
    LDR    r4, =0x3FF        ;\
    ANDS   r4, r4, r1, LSR ARM_IMM 3 /* r4 = (# of ways - 1); way index */;\
    CLZ    r5, r4                    /* r5 = bit position of way size increment */;\
    LDR    r6, =0x00007FFF        ;\
    ANDS    r6, r6, r1, LSR ARM_IMM 13/* r6 = (# of sets - 1); set index */;\
                    \
2:                    ;\
    MOV    r1, r4                 /* r1 = working copy of way number */;\
3:                    ;\
    ORR    r8, r7, r1, LSL r5     /* r8 = set/way operation data word: */;\
    ORR    r8, r8, r6, LSL r2     /* cache level, way and set info */;\
                    \
    MCR    p15, 0, r8, c7, crm, 2 /* dcache operation by set/way */;\
                    \
    SUBS   r1, r1, ARM_IMM 1      /* decrement the way index */;\
    BGE    3b            ;\
    SUBS   r6, r6, ARM_IMM 1      /* decrement the set index */;\
    BGE    2b            ;\
                    \
4:                    ;\
    ADD    r7, r7, ARM_IMM 2      /* increment cache index = level << 1 */;\
    CMP    r3, r7                 /* done when LoC is reached */;\
    MOVGT  r0, r0, LSR ARM_IMM 3  /* rt-align type of next cache level */;\
    BGT    1b            ;\
                    \
5:                    ;\

/* EMAC Control Module Registers offset */

#define WRAPPER_OFFSET                      (0x0900)

/* DMA_RAM offset */

#define WRAPPER_RAM_OFFSET                  (0x2000)
#define MDIO_OFFSET                         (0x0800)
#define EMAC_CLK                            (250000000)

/* Control Module related registers */

#define AM3XXX_DEV_ID                       (AM3XXX_CTRL_BASE + 0x600)
#define AM3XXX_MACID0_LO                    (AM3XXX_CTRL_BASE + 0x630)
#define AM3XXX_MACID0_HI                    (AM3XXX_CTRL_BASE + 0x634)
#define AM3XXX_MACID1_LO                    (AM3XXX_CTRL_BASE + 0x638)
#define AM3XXX_MACID1_HI                    (AM3XXX_CTRL_BASE + 0x63C)
#define AM3XXX_TIMER_EVT_CAP                (AM3XXX_CTRL_BASE + 0xFD0)

#define AM3XXX_INIT_STACK_SIZE              (0x3000)
#define AM3XXX_ROM_HEADER_SIZE              (0x8)
#define AM3XXX_NANDBOOT_RAM_ADRS            (AM3XXX_SRAM0_START)

#define MAC_MII_SEL                         (AM3XXX_CTRL_BASE + 0x650)
#define RGMII_MODE_ENABLE                   (0xA)

#define am3xxxInLong(a)                     (*(volatile unsigned int *)(a))
#define am3xxxOutLong(a, v)                 (*(volatile unsigned int *)(a) = (v))
#define am3xxxInWord(a)                     (*(volatile unsigned short *)(a))
#define am3xxxOutWord(a, v)                 (*(volatile unsigned short *)(a) = (v))
#define am3xxxInByte(a)                     (*(volatile unsigned char *)(a))
#define am3xxxOutByte(a, v)                 (*(volatile unsigned char *)(a) = (v))

/* definitions for delay timer */

#define TIMER_SYNC_WRITE(reg, val, syncReg, bit) \
    do { \
        unsigned int __v;\
        am335xOutLong (reg, val); \
        do { \
            __v = am335xInLong (syncReg); \
        } while (__v & bit); \
    } while (0)

#define TIMER_SYNC_READ(reg, syncReg, bit, data) \
    do { \
        unsigned int __v; \
        do { \
            __v = am335xInLong (syncReg); \
        } while (__v & bit); \
        data = am335xInLong (reg); \
    } while (0)

#define TIDR              (0x0)
#define TIOCP_CFG         (0x10)
#define IRQ_EIQ           (0x20)
#define IRQSTATUS_RAW     (0x24)
#define IRQSTATUS         (0x28)
#define IRQENABLE_SET     (0x2C)
#define IRQENABLE_CLR     (0x30)
#define IRQWAKEEN         (0x34)
#define TCLR              (0x38)
#define TCRR              (0x3C)
#define TLDR              (0x40)
#define TTGR              (0x44)
#define TWPS              (0x48)
#define TMAR              (0x4C)
#define TCAR1             (0x50)
#define TSICR             (0x54)
#define TCAR2             (0x58)

#define TCAR_EN_FLAG      (0x1 << 2)
#define CAPT_MODE         (0x1 << 13)
#define CAPT_LOW_TO_HIGH  (0x1 << 8)
#define CAPT_HIGH_TO_LOW  (0x2 << 8)
#define CAPT_BOTH         (0x3 << 8)

#define W_PEND_TMAR       (0x10)
#define W_PEND_TTGR       (0x8)
#define W_PEND_TLDR       (0x4)
#define W_PEND_TCRR       (0x2)
#define W_PEND_TCLR       (0x1)

/* watch dog timer */

#define AM3XXX_WDT_BASE   (0x44E35000)
#define WDT_WIDR          (0x0)
#define WDT_WDSC          (0x10)
#define WDT_WDST          (0x14)
#define WDT_WISR          (0x18)
#define WDT_WIER          (0x1c)
#define WDT_WCLR          (0x24)
#define WDT_WCRR          (0x28)
#define WDT_WLDR          (0x2c)
#define WDT_WTGR          (0x30)
#define WDT_WWPS          (0x34)
#define WDT_WDLY          (0x44)
#define WDT_WSPR          (0x48)
#define WDT_WIRQSTATRAW   (0x54)
#define WDT_WIRQSTAT      (0x58)
#define WDT_WIRQENSET     (0x5c)
#define WDT_WIRQENCLR     (0x60)

/* Interrupt proirity */

#define AM3XXXINT_PRI_UART_0          3 /* UART 0 */
#define AM3XXXINT_PRI_UART_1          3 /* UART 1 */
#define AM3XXXINT_PRI_UART_2          3 /* UART 2 */
#define AM3XXXINT_PRI_UART_3          3 /* UART 3 */
#define AM3XXXINT_PRI_TIMER_1         0 /* timer 1 */
#define AM3XXXINT_PRI_TIMER_2         6 /* timer 2 */
#define AM3XXXINT_PRI_TIMER_3         8 /* timer 3 */
#define AM3XXXINT_PRI_ENET            4 /* ethernet interrupt(GPIO-E) */
#define AM3XXXINT_PRI_MAIL_U0_MPU     9 /* mailbox interrupt */
#define AM3XXXINT_PRI_IVA2_MMU        9 /* iva2 subsystem interrupt */
#define AM3XXXINT_PRI_MHCI            10 /* mhci */
#define AM3XXXINT_PRI_EHCI            7  /* ehci */
#define AM3XXXINT_PRI_SGX             7  /* SGX */
#define AM3XXXINT_PRI_DSS             7  /* DSS */
#define AM3XXXINT_PRI_NIRQ            7  /* NIRQ */
#define AM3XXXINT_PRI_MMC             8  /* MMC1 */
#define AM3XXXINT_PRI_SATA            8  /* SATA */
#define AM3XXXINT_PRI_EDMACOMP        5  /* EDMA Completion */
#define AM3XXXINT_PRI_EDMAMAPERR      6  /* EDMA memory protection */
#define AM3XXXINT_PRI_EDMAERRINT      6  /* EDMA error */
#define AM3XXXINT_PRI_GPIOINT0B       8  /* TOUCHSCREEN */
#define AM3XXXINT_PRI_MASP2           6  /* MCASP2 */
#define AM3XXXINT_PRI_I2C             6  /* I2C */
#define AM3XXXINT_PRI_RTC             1  /* I2C */
#define AM3XXXINT_PRI_SPI0            6  /* SPI0 */
#define AM3XXXINT_PRI_GPIO            8  /* GPIO */
#define AM3XXXINT_PRI_DCAN            6  /* GPIO */

/* IRQ numbers */

#define AM3XXXM_IRQ_0 0
#define AM3XXXM_IRQ_1 1
#define AM3XXXM_IRQ_2 2
#define AM3XXXM_IRQ_3 3
#define AM3XXXM_IRQ_4 4
#define AM3XXXM_IRQ_5 5
#define AM3XXXM_IRQ_6 6
#define AM3XXXM_IRQ_7 7
#define AM3XXXM_IRQ_8 8
#define AM3XXXM_IRQ_9 9
#define AM3XXXM_IRQ_10 10
#define AM3XXXM_IRQ_11 11
#define AM3XXXM_IRQ_12 12
#define AM3XXXM_IRQ_13 13
#define AM3XXXM_IRQ_14 14
#define AM3XXXM_IRQ_15 15
#define AM3XXXM_IRQ_16 16
#define AM3XXXM_IRQ_17 17
#define AM3XXXM_IRQ_18 18
#define AM3XXXM_IRQ_19 19
#define AM3XXXM_IRQ_20 20
#define AM3XXXM_IRQ_21 21
#define AM3XXXM_IRQ_22 22
#define AM3XXXM_IRQ_23 23
#define AM3XXXM_IRQ_24 24
#define AM3XXXM_IRQ_25 25
#define AM3XXXM_IRQ_26 26
#define AM3XXXM_IRQ_27 27
#define AM3XXXM_IRQ_28 28
#define AM3XXXM_IRQ_29 29
#define AM3XXXM_IRQ_30 30
#define AM3XXXM_IRQ_31 31
#define AM3XXXM_IRQ_32 32
#define AM3XXXM_IRQ_33 33
#define AM3XXXM_IRQ_34 34
#define AM3XXXM_IRQ_35 35
#define AM3XXXM_IRQ_36 36
#define AM3XXXM_IRQ_37 37
#define AM3XXXM_IRQ_38 38
#define AM3XXXM_IRQ_39 39
#define AM3XXXM_IRQ_40 40
#define AM3XXXM_IRQ_41 41
#define AM3XXXM_IRQ_42 42
#define AM3XXXM_IRQ_43 43
#define AM3XXXM_IRQ_44 44
#define AM3XXXM_IRQ_45 45
#define AM3XXXM_IRQ_46 46
#define AM3XXXM_IRQ_47 47
#define AM3XXXM_IRQ_48 48
#define AM3XXXM_IRQ_49 49
#define AM3XXXM_IRQ_50 50
#define AM3XXXM_IRQ_51 51
#define AM3XXXM_IRQ_52 52
#define AM3XXXM_IRQ_53 53
#define AM3XXXM_IRQ_54 54
#define AM3XXXM_IRQ_55 55
#define AM3XXXM_IRQ_56 56
#define AM3XXXM_IRQ_57 57
#define AM3XXXM_IRQ_58 58
#define AM3XXXM_IRQ_59 59
#define AM3XXXM_IRQ_60 60
#define AM3XXXM_IRQ_61 61
#define AM3XXXM_IRQ_62 62
#define AM3XXXM_IRQ_63 63
#define AM3XXXM_IRQ_64 64
#define AM3XXXM_IRQ_65 65
#define AM3XXXM_IRQ_66 66
#define AM3XXXM_IRQ_67 67
#define AM3XXXM_IRQ_68 68
#define AM3XXXM_IRQ_69 69
#define AM3XXXM_IRQ_70 70
#define AM3XXXM_IRQ_71 71
#define AM3XXXM_IRQ_72 72
#define AM3XXXM_IRQ_73 73
#define AM3XXXM_IRQ_74 74
#define AM3XXXM_IRQ_75 75
#define AM3XXXM_IRQ_76 76
#define AM3XXXM_IRQ_77 77
#define AM3XXXM_IRQ_78 78
#define AM3XXXM_IRQ_79 79
#define AM3XXXM_IRQ_80 80
#define AM3XXXM_IRQ_81 81
#define AM3XXXM_IRQ_82 82
#define AM3XXXM_IRQ_83 83
#define AM3XXXM_IRQ_84 84
#define AM3XXXM_IRQ_85 85
#define AM3XXXM_IRQ_86 86
#define AM3XXXM_IRQ_87 87
#define AM3XXXM_IRQ_88 88
#define AM3XXXM_IRQ_89 89
#define AM3XXXM_IRQ_90 90
#define AM3XXXM_IRQ_91 91
#define AM3XXXM_IRQ_92 92
#define AM3XXXM_IRQ_93 93
#define AM3XXXM_IRQ_94 94
#define AM3XXXM_IRQ_95 95
#define AM3XXXM_IRQ_96 96
#define AM3XXXM_IRQ_97 97
#define AM3XXXM_IRQ_98 98
#define AM3XXXM_IRQ_99 99
#define AM3XXXM_IRQ_100 100
#define AM3XXXM_IRQ_101 101
#define AM3XXXM_IRQ_102 102
#define AM3XXXM_IRQ_103 103
#define AM3XXXM_IRQ_104 104
#define AM3XXXM_IRQ_105 105
#define AM3XXXM_IRQ_106 106
#define AM3XXXM_IRQ_107 107
#define AM3XXXM_IRQ_108 108
#define AM3XXXM_IRQ_109 109
#define AM3XXXM_IRQ_110 110
#define AM3XXXM_IRQ_111 111
#define AM3XXXM_IRQ_112 112
#define AM3XXXM_IRQ_113 113
#define AM3XXXM_IRQ_114 114
#define AM3XXXM_IRQ_115 115
#define AM3XXXM_IRQ_116 116
#define AM3XXXM_IRQ_117 117
#define AM3XXXM_IRQ_118 118
#define AM3XXXM_IRQ_119 119
#define AM3XXXM_IRQ_120 120
#define AM3XXXM_IRQ_121 121
#define AM3XXXM_IRQ_122 122
#define AM3XXXM_IRQ_123 123
#define AM3XXXM_IRQ_124 124
#define AM3XXXM_IRQ_125 125
#define AM3XXXM_IRQ_126 126             /* reserved */
#define AM3XXXM_IRQ_127 127             /* reserved */

/* Interrupt Mappings to the MPU Subsystem */

#define AM3XXX_EMUINT        AM3XXXM_IRQ_0    /* MPU emulation(2) */
#define AM3XXX_COMMTX        AM3XXXM_IRQ_1    /* MPU emulation(2) */
#define AM3XXX_COMMRX        AM3XXXM_IRQ_2    /* MPU emulation(2) */
#define AM3XXX_BENCH         AM3XXXM_IRQ_3    /* MPU emulation(2) */
#define AM3XXX_ELM_IRQ       AM3XXXM_IRQ_4    /* ELM */

#define AM3XXX_NMI           AM3XXXM_IRQ_7    /* NMIn Pin */

#define AM3XXX_L3DEBUG       AM3XXXM_IRQ_9    /* L3 Interconnect */
#define AM3XXX_L3APPINT      AM3XXXM_IRQ_10   /* L3 Interconnect */
#define AM3XXX_EDMACOMPINT   AM3XXXM_IRQ_12   /* EDMA CC Completion */
#define AM3XXX_EDMAMAPERR    AM3XXXM_IRQ_13   /* EDMA Memory Protection Error */
#define AM3XXX_EDMAERRINT    AM3XXXM_IRQ_14   /* EDMA CC Error */
#define AM3XXX_WDTINT0       AM3XXXM_IRQ_15   /* Watchdog Timer 0 */
#define AM3XXX_SATAINT       AM3XXXM_IRQ_16   /* SATA */
#define AM3XXX_USBSSINT      AM3XXXM_IRQ_17   /* USB Subsystem */
#define AM3XXX_USBINT0       AM3XXXM_IRQ_18   /* USB0 */
#define AM3XXX_USBINT1       AM3XXXM_IRQ_19   /* USB1 */

#define AM3XXX_SDINT1        AM3XXXM_IRQ_28   /* MMC/SD1 */
#define AM3XXX_SDINT2        AM3XXXM_IRQ_29   /* MMC/SD2 */
#define AM3XXX_I2CINT2       AM3XXXM_IRQ_30   /* I2C2 */
#define AM3XXX_GPIOINT2A     AM3XXXM_IRQ_32   /* GPIO2 A */
#define AM3XXX_GPIOINT2B     AM3XXXM_IRQ_33   /* GPIO2 B */
#define AM3XXX_USBWAKEUP     AM3XXXM_IRQ_34   /* USB Subsystem Wakeup */
#define AM3XXX_PCIEWAKEUP    AM3XXXM_IRQ_35   /* PCIe Wakeup */
#define AM3XXX_DSSINT        AM3XXXM_IRQ_36   /* HDVPSS */
#define AM3XXX_SGXINT        AM3XXXM_IRQ_37   /* SGX530 */
#define AM3XXX_HDMINT        AM3XXXM_IRQ_38   /* HDMI */
#define AM3XXX_ISS_IRQ_5     AM3XXXM_IRQ_39   /* ISS */
#define AM3XXX_3PGSWRXTHR0   AM3XXXM_IRQ_40   /* EMAC Switch RX Threshold */
#define AM3XXX_3PGSWRXINT0   AM3XXXM_IRQ_41   /* EMAC Switch Receive */
#define AM3XXX_3PGSWTXINT0   AM3XXXM_IRQ_42   /* EMAC Switch Transmit */
#define AM3XXX_3PGSWMISC0    AM3XXXM_IRQ_43   /* EMAC Switch Miscellaneous */
#define AM3XXX_UARTINT3      AM3XXXM_IRQ_44   /* UART3 */
#define AM3XXX_UARTINT4      AM3XXXM_IRQ_45   /* UART4 */
#define AM3XXX_UARTINT5      AM3XXXM_IRQ_46   /* UART5 */

#define AM3XXX_DCAN0_INT0    AM3XXXM_IRQ_52   /* DCAN0 */
#define AM3XXX_DCAN0_INT1    AM3XXXM_IRQ_53   /* DCAN0 */
#define AM3XXX_DCAN0_PARITY  AM3XXXM_IRQ_54   /* DCAN0 Parity */
#define AM3XXX_DCAN1_INT0    AM3XXXM_IRQ_55   /* DCAN1 */
#define AM3XXX_DCAN1_INT1    AM3XXXM_IRQ_56   /* DCAN1 */
#define AM3XXX_DCAN1_PARITY  AM3XXXM_IRQ_57   /* DCAN1 Parity */

#define AM3XXX_GPIOINT3A     AM3XXXM_IRQ_62   /* GPIO3 */
#define AM3XXX_GPIOINT3B     AM3XXXM_IRQ_63   /* GPIO3 */
#define AM3XXX_SDINT0        AM3XXXM_IRQ_64   /* MMC/SD0 */
#define AM3XXX_SPIINT0       AM3XXXM_IRQ_65   /* SPI0 */

#define AM3XXX_TINT0         AM3XXXM_IRQ_66   /* TIMER0 */
#define AM3XXX_TINT1         AM3XXXM_IRQ_67   /* TIMER1 */
#define AM3XXX_TINT2         AM3XXXM_IRQ_68   /* TIMER2 */
#define AM3XXX_TINT3         AM3XXXM_IRQ_69   /* TIMER3 */
#define AM3XXX_I2CINT0       AM3XXXM_IRQ_70   /* I2C0 */
#define AM3XXX_I2CINT1       AM3XXXM_IRQ_71   /* I2C1 */
#define AM3XXX_UARTINT0      AM3XXXM_IRQ_72   /* UART0 */
#define AM3XXX_UARTINT1      AM3XXXM_IRQ_73   /* UART1 */
#define AM3XXX_UARTINT2      AM3XXXM_IRQ_74   /* UART2 */
#define AM3XXX_RTCINT        AM3XXXM_IRQ_75   /* RTC */
#define AM3XXX_RTCALARMINT   AM3XXXM_IRQ_76   /* RTC Alarm */
#define AM3XXX_MBINT         AM3XXXM_IRQ_77   /* Mailbox */

#define AM3XXX_MCATXINT0     AM3XXXM_IRQ_80   /* McASP0 Transmit */
#define AM3XXX_MCARXINT0     AM3XXXM_IRQ_81   /* McASP0 Receive */
#define AM3XXX_MCATXINT1     AM3XXXM_IRQ_82   /* McASP1 Transmit */
#define AM3XXX_MCARXINT1     AM3XXXM_IRQ_83   /* McASP1 Receive */
#define AM3XXX_MCATXINT2     AM3XXXM_IRQ_84   /* McASP2 Transmit */
#define AM3XXX_MCARXINT2     AM3XXXM_IRQ_85   /* McASP2 Receive */
#define AM3XXX_MCBSPINT      AM3XXXM_IRQ_86   /* McBSP */

#define AM3XXX_TINT4         AM3XXXM_IRQ_92   /* TIMER4 */
#define AM3XXX_TINT5         AM3XXXM_IRQ_93   /* TIMER5 */
#define AM3XXX_TINT6         AM3XXXM_IRQ_94   /* TIMER6 */
#define AM3XXX_TINT7         AM3XXXM_IRQ_95   /* TIMER7 */
#define AM3XXX_GPIOINT0A     AM3XXXM_IRQ_96   /* GPIO0 */
#define AM3XXX_GPIOINT0B     AM3XXXM_IRQ_97   /* GPIO0 */
#define AM3XXX_GPIOINT1A     AM3XXXM_IRQ_98   /* GPIO1 */
#define AM3XXX_GPIOINT1B     AM3XXXM_IRQ_99   /* GPIO1 */
#define AM3XXX_GPMCINT       AM3XXXM_IRQ_100   /* GPMC */
#define AM3XXX_DDRERR0       AM3XXXM_IRQ_101   /* DDR0 */
#define AM3XXX_DDRERR1       AM3XXXM_IRQ_102   /* DDR1 */

#define AM3XXX_MCATXINT3     AM3XXXM_IRQ_105   /* McASP3 Transmit */
#define AM3XXX_MCARXINT3     AM3XXXM_IRQ_106   /* McASP3 Receive */

#define AM3XXX_MCATXINT4     AM3XXXM_IRQ_108   /* McASP4 Transmit */
#define AM3XXX_MCARXINT4     AM3XXXM_IRQ_109   /* McASP4 Receive */
#define AM3XXX_MCATXINT5     AM3XXXM_IRQ_110   /* McASP5 Transmit */
#define AM3XXX_MCARXINT5     AM3XXXM_IRQ_111   /* McASP5 Receive */

#define AM3XXX_TCERRINT0     AM3XXXM_IRQ_112   /* EDMA TC 0 Error */
#define AM3XXX_TCERRINT1     AM3XXXM_IRQ_113   /* EDMA TC 1 Error */
#define AM3XXX_TCERRINT2     AM3XXXM_IRQ_114   /* EDMA TC 2 Error */
#define AM3XXX_TCERRINT3     AM3XXXM_IRQ_115   /* EDMA TC 3 Error */

#define AM3XXX_SMRFLX_ARM    AM3XXXM_IRQ_120   /* SmartReflex ARM Domain */
#define AM3XXX_SMRFLX_CORE   AM3XXXM_IRQ_121   /* SmartReflex CORE Domain */

#define AM3XXX_MMUINT        AM3XXXM_IRQ_122   /* System MMU */
#define AM3XXX_MCMMUINT      AM3XXXM_IRQ_123   /* Media Controller */
#define AM3XXX_DMMINT        AM3XXXM_IRQ_124   /* DMM */
#define AM3XXX_SPIINT1       AM3XXXM_IRQ_125   /* SPI1 */

#define AM3XXX_IRQ_NUMS      128
#define AM3XXX_LVL_NUMS      8

/* timer capture event */

#define PWM_EVENT_UART0        1
#define PWM_EVENT_UART1        2
#define PWM_EVENT_UART2        3
#define PWM_EVENT_UART3        4
#define PWM_EVENT_UART4        5
#define PWM_EVENT_UART5        6
#define PWM_EVENT_3PGSWRXTHR0  7
#define PWM_EVENT_3PGSWRXINT0  8
#define PWM_EVENT_3PGSWTXINT0  9
#define PWM_EVENT_3PGSWMISC0   10
#define PWM_EVENT_MCATXINT0    11
#define PWM_EVENT_MCARXINT0    12
#define PWM_EVENT_MCATXINT1    13
#define PWM_EVENT_MCARXINT1    14
#define PWM_EVENT_GPIOINT0A    17
#define PWM_EVENT_GPIOINT0B    18
#define PWM_EVENT_GPIOINT1A    19
#define PWM_EVENT_GPIOINT1B    20
#define PWM_EVENT_GPIOINT2A    21
#define PWM_EVENT_GPIOINT2B    22
#define PWM_EVENT_GPIOINT3A    23
#define PWM_EVENT_GPIOINT3B    24
#define PWM_EVENT_DCAN0_INT0   25
#define PWM_EVENT_DCAN0_INT1   26
#define PWM_EVENT_DCAN0_PARITY 27
#define PWM_EVENT_DCAN1_INT0   28
#define PWM_EVENT_DCAN1_INT1   29
#define PWM_EVENT_DCAN1_PARITY 30

/* functions */

BOOL am3xxxProbe (char * compat);
void am3xxxInit (void);
void am3xxxEarlyInit (void);
char * am3xxxModel (void);
void am3xxxUsDelay (int us);
void am3xxxReset (int startType);

#ifdef __cplusplus
}
#endif

#endif /* __INCam3xxxh */
