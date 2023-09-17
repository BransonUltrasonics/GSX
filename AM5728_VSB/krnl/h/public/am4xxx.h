/* am4xxx.h - header files for TI AM4XXX processor */

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
09sep15,yya  added warm reboot support. (US63670)
15sep14,swu  updated PSL definition. (V7PRO-1243)
01jul14,swu  created.(US40646)
*/

#ifndef __INCam4xxxh
#define __INCam4xxxh

#ifdef __cplusplus
extern "C" {
#endif

/* Handy sizes */

#define SZ_4K               (0x00001000)
#define SZ_8K               (0x00002000)
#define SZ_16K              (0x00004000)
#define SZ_32K              (0x00008000)
#define SZ_64K              (0x00010000)
#define SZ_128K             (0x00020000)
#define SZ_256K             (0x00040000)
#define SZ_512K             (0x00080000)

#define SZ_1M               (0x00100000)
#define SZ_2M               (0x00200000)
#define SZ_4M               (0x00400000)
#define SZ_8M               (0x00800000)
#define SZ_16M              (0x01000000)
#define SZ_32M              (0x02000000)
#define SZ_64M              (0x04000000)
#define SZ_128M             (0x08000000)
#define SZ_256M             (0x10000000)
#define SZ_512M             (0x20000000)

#define SZ_1G               (0x40000000)
#define SZ_2G               (0x80000000)

#ifndef BIT
#define BIT(x)              (1 << (x))
#endif

/* watch dog timer register offset */

#define WDT_WIDR            (0x0)
#define WDT_WDSC            (0x10)
#define WDT_WDST            (0x14)
#define WDT_WISR            (0x18)
#define WDT_WIER            (0x1c)
#define WDT_WCLR            (0x24)
#define WDT_WCRR            (0x28)
#define WDT_WLDR            (0x2c)
#define WDT_WTGR            (0x30)
#define WDT_WWPS            (0x34)
#define WDT_WDLY            (0x44)
#define WDT_WSPR            (0x48)
#define WDT_WIRQSTATRAW     (0x54)
#define WDT_WIRQSTAT        (0x58)
#define WDT_WIRQENSET       (0x5c)
#define WDT_WIRQENCLR       (0x60)

/* L2 cache */

#define L2_CONTROL                      (0x100)
#define L2_AUX_CONTROL                  (0x104)
#define L2_CLEAN_INVALIDATE_WAY         (0x7FC)

#define L2_AUX_CTL_ASSO_MSK             (0x1<<16)

/* GIC */

#define GICD_CTRL                       (0x0)
#define GICD_ISENABLER0                 (0x100)
#define GICD_ICENABLER0                 (0x180)
#define GICD_ISPENDR0                   (0x200)
#define GICD_ICPENDR0                   (0x280)
#define GICD_ISACTIVER0                 (0x300)
#define GICD_ICACTIVER0                 (0x380)

#define GICC_CTRL                       (0x0)
#define GICC_PMR                        (0x4)
#define GICC_EOIR                       (0x10)

#define INT_LVL_MPCORE_CPC              (0)

#define SGI_INT_MAX                     (16)
#define ARM_GIC_IPI_COUNT               (16)    /* MPCore IPI count         */
#define SPI_START_INT_NUM               (32)    /* SPI start at ID32        */
#define PPI_START_INT_NUM               (16)    /* PPI start at ID16        */
#define GIC_INT_MAX_NUM                 (1020)  /* GIC max interrupts count */

/* cache maintence operation */

#define ARM_IMM             #
#define ARM_LOC_MASK        #0x7000000  /* level of coherency mask of CLIDR */
#define CACHE_DC_ENABLE     (1<<2)    /* (data) cache enable */
#define CACHE_IC_ENABLE     (1<<12)   /* Instruction cache enable */

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

#define _CORTEX_AR_ENTIRE_DATA_CACHE_OP(crm)    \
                                                \
    MRC    p15, 1, r0, c0, c0, 1    /* r0 = Cache Lvl ID register info */;\
    ANDS   r3, r0, ARM_LOC_MASK     /* get level of coherency (LoC) */;\
    MOV    r3, r3, LSR ARM_IMM 23   /* r3 = LoC << 1 */;\
    BEQ    5f            ;                      \
                                                \
    MOV    r7, ARM_IMM 0            /* r7 = cache level << 1; start at 0 */;\
                                                \
1:                    ;                         \
    AND    r1, r0, ARM_IMM 0x7      /* r1 = cache type(s) for this level */;\
    CMP    r1, ARM_IMM 2        ;               \
    BLT    4f                       /* no data cache at this level */;\
                                                \
    MCR    p15, 2, r7, c0, c0, 0    /* select the Cache Size ID register */;\
    MCR    p15, 0, r7, c7, c5, 4    /* ISB: sync change to Cache Size ID */;\
    MRC    p15, 1, r1, c0, c0, 0    /* r1 = current Cache Size ID info */;\
    AND    r2, r1, ARM_IMM 0x7      /* r2 = line length */;\
    ADD    r2, r2, ARM_IMM 4        /* add line length offset = log2(16 bytes) */;\
    LDR    r4, =0x3FF        ;                  \
    ANDS   r4, r4, r1, LSR ARM_IMM 3 /* r4 = (# of ways - 1); way index */;\
    CLZ    r5, r4                    /* r5 = bit position of way size increment */;\
    LDR    r6, =0x00007FFF        ;             \
    ANDS    r6, r6, r1, LSR ARM_IMM 13/* r6 = (# of sets - 1); set index */;\
                                                \
2:                    ;                         \
    MOV    r1, r4                 /* r1 = working copy of way number */;\
3:                    ;                         \
    ORR    r8, r7, r1, LSL r5     /* r8 = set/way operation data word: */;\
    ORR    r8, r8, r6, LSL r2     /* cache level, way and set info */;\
                                                \
    MCR    p15, 0, r8, c7, crm, 2 /* dcache operation by set/way */;\
                                                \
    SUBS   r1, r1, ARM_IMM 1      /* decrement the way index */;\
    BGE    3b            ;                      \
    SUBS   r6, r6, ARM_IMM 1      /* decrement the set index */;\
    BGE    2b            ;                      \
                                                \
4:                    ;                         \
    ADD    r7, r7, ARM_IMM 2      /* increment cache index = level << 1 */;\
    CMP    r3, r7                 /* done when LoC is reached */;\
    MOVGT  r0, r0, LSR ARM_IMM 3  /* rt-align type of next cache level */;\
    BGT    1b            ;                      \
                                                \
5:                    ;                         \

#ifndef sysRead32
#define sysRead32(a)             (*(volatile unsigned int *)(a))
#endif /* sysRead32 */
#ifndef sysWrite32
#define sysWrite32(a, v)         (*(volatile unsigned int *)(a) = (v))
#endif /* sysWrite32 */
#ifndef sysRead16
#define sysRead16(a)             (*(volatile unsigned short *)(a))
#endif /* sysRead16 */
#ifndef sysWrite16
#define sysWrite16(a, v)         (*(volatile unsigned short *)(a) = (v))
#endif /* sysWrite16 */
#ifndef sysRead8
#define sysRead8(a)              (*(volatile unsigned char *)(a))
#endif /* sysRead8 */
#ifndef sysWrite8
#define sysWrite8(a, v)          (*(volatile unsigned char *)(a) = (v))
#endif /* sysWrite8 */

/* functions */

BOOL   am4xxxProbe     (char * compat);
void   am4xxxInit      (void);
void   am4xxxEarlyInit (void);
char * am4xxxModel     (void);
void   am4xxxUsDelay   (int us);
void   am4xxxReset     (int startType);

#ifdef __cplusplus
}
#endif

#endif /* __INCam4xxxh */
