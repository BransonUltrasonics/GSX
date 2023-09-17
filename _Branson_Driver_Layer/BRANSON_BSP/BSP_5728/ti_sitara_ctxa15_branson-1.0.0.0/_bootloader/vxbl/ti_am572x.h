/* ti_am572x.h - header files for TI AM572X */

/*
 * Copyright (c) 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
15mar16,jnl  created (US75898)
*/

#ifndef __INCam572x
#define __INCam572x

#include <vxboot.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 48 MHz input clock */

#define UART_CLK_FREQ                   (48000000)
#define SYS_32K_CLK_FREQ                (32000)
#define SYS_CLK_FREQ                    (24000000)

#define AM572X_SYS_TIMER_CLK_FREQ       (24000000)
#define AM572X_AUX_TIMER_CLK_FREQ       (24000000)

#define MPU_INTERRUPT_NUM               (128)

/* TI AM572X Board Registers Offset */

#define AM572X_SRAM0_START              (0x40300000)

#define AM572X_GPTIMER0_BASE            (0x44E05000)
#define AM572X_GPTIMER1_BASE            (0x44E31000)  /* 1ms timer */

#define AM572X_GPIO6_BASE               (0x4805D000)

/* GPIO Registers Offset */

#define AM572X_GPIO_REVISION            (0x000)
#define AM572X_GPIO_SYSCONFIG           (0x010)
#define AM572X_GPIO_IRQSTATUS0_RAW      (0x024)
#define AM572X_GPIO_IRQSTATUS1_RAW      (0x028)
#define AM572X_GPIO_IRQSTATUS0          (0x02C)
#define AM572X_GPIO_IRQSTATUS1          (0x030)
#define AM572X_GPIO_IRQSTATUS_SET0      (0x034)
#define AM572X_GPIO_IRQSTATUS_SET1      (0x038)
#define AM572X_GPIO_IRQSTATUS_CLR0      (0x03C)
#define AM572X_GPIO_IRQSTATUS_CLR1      (0x040)
#define AM572X_GPIO_IRQWAKEN0           (0x044)
#define AM572X_GPIO_IRQWAKEN1           (0x048)

#define AM572X_GPIO_SYSSTATUS           (0x114)
#define AM572X_GPIO_CTRL                (0x130)
#define AM572X_GPIO_OE                  (0x134)
#define AM572X_GPIO_DATAIN              (0x138)
#define AM572X_GPIO_DATAOUT             (0x13C)
#define AM572X_GPIO_LEVELDETECT0        (0x140)
#define AM572X_GPIO_LEVELDETECT1        (0x144)
#define AM572X_GPIO_RISINGDETECT        (0x148)
#define AM572X_GPIO_FALLINGDETECT       (0x14C)
#define AM572X_GPIO_DEBOUNCENABLE       (0x150)
#define AM572X_GPIO_DEBOUNCINGTIME      (0x154)
#define AM572X_GPIO_CLEARDATAOUT        (0x190)
#define AM572X_GPIO_SETDATAOUT          (0x194)

#define AM572X_ELM_BASE                 (0x48080000)
#define AM572X_EMIF4_0_CFG_BASE         (0x4C000000)
#define AM572X_EMIF4_1_CFG_BASE         (0x4D000000)
#define AM572X_DMM_BASE                 (0x4E000000)
#define AM572X_GPMC_BASE                (0x50000000)
#define AM572X_PLL_SUBSYS_BASE          (0x481C5000)

#define AM572X_PRCM_BASE                (0x44E00000)
#define AM572X_PRCM_SIZE                (0x00004000)

#define AM572X_SDRC_NCS0                (0x80000000)
#define AM572X_SDRC_NCS0_END            (0xffffffff)
#define AM572X_SDRC_NCS0_SIZE           (SZ_2G)

#define AM572X_UART0_BASE               (0x44e09000)
#define AM572X_UART0_END                (0x44e0afff)
#define AM572X_UART0_SIZE               (SZ_8K)

#define AM572X_UART1_BASE               (0x48022000)
#define AM572X_UART1_END                (0x48023fff)
#define AM572X_UART1_SIZE               (SZ_8K)

#define AM572X_UART2_BASE               (0x48024000)
#define AM572X_UART2_END                (0x48025fff)
#define AM572X_UART2_SIZE               (SZ_8K)

#define AM572X_UART3_BASE               (0x481a6000)
#define AM572X_UART3_END                (0x481a7fff)
#define AM572X_UART3_SIZE               (SZ_8K)

#define AM572X_MMC1_BASE                (0x4809C000)
#define AM572X_MMC1_SDHC_BASE           (AM572X_MMC1_BASE + 0x100)
#define AM572X_MMC1_END                 (0x4809CFFF)
#define AM572X_MMC1_SIZE                (SZ_4K)

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

#define AM572X_INIT_STACK_SIZE              (0x3000)
#define AM572X_ROM_HEADER_SIZE              (0x8)
#define AM572X_NANDBOOT_RAM_ADRS            (AM572X_SRAM0_START)

#define am572xInLong(a)                     (*(volatile unsigned int *)(a))
#define am572xOutLong(a, v)                 (*(volatile unsigned int *)(a) = (v))
#define am572xInWord(a)                     (*(volatile unsigned short *)(a))
#define am572xOutWord(a, v)                 (*(volatile unsigned short *)(a) = (v))
#define am572xInByte(a)                     (*(volatile unsigned char *)(a))
#define am572xOutByte(a, v)                 (*(volatile unsigned char *)(a) = (v))

/* definitions for delay timer */

#define TIMER_SYNC_WRITE(reg, val, syncReg, bit) \
    do { \
        unsigned int __v;\
        am572xOutLong (reg, val); \
        do { \
            __v = am572xInLong (syncReg); \
        } while (__v & bit); \
    } while (0)

#define TIMER_SYNC_READ(reg, syncReg, bit, data) \
    do { \
        unsigned int __v; \
        do { \
            __v = am572xInLong (syncReg); \
        } while (__v & bit); \
        data = am572xInLong (reg); \
    } while (0)

#define TIDR          0x0
#define TIOCP_CFG     0x10
#define IRQ_EIQ       0x20
#define IRQSTATUS_RAW 0x24
#define IRQSTATUS     0x28
#define IRQENABLE_SET 0x2C
#define IRQENABLE_CLR 0x30
#define IRQWAKEEN     0x34
#define TCLR          0x38
#define TCRR          0x3C
#define TLDR          0x40
#define TTGR          0x44
#define TWPS          0x48
#define TMAR          0x4C
#define TCAR1         0x50
#define TSICR         0x54
#define TCAR2         0x58

#define W_PEND_TMAR   0x10
#define W_PEND_TTGR   0x8
#define W_PEND_TLDR   0x4
#define W_PEND_TCRR   0x2
#define W_PEND_TCLR   0x1

#ifdef __cplusplus
}
#endif

#endif /* __INCam572x */
