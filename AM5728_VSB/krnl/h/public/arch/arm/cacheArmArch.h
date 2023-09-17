/* cacheArmArch.h - ARM ARCH7 cache library header file */

/*
 * Copyright (c) 2013, 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02dec15,mpc  added Cortex-A57 support (US66795)
24jun15,cfm  added missing ISB in _CORTEX_AR_ENTIRE_DATA_CACHE_OP_POX (V7PRO-2060)
19jan15,sye  added Cortex-A7 macro for cacheArchL2EcrGet. (US46486)
28nov13,cfm  created from cacheArmArch7.h, rev 01h
*/

#ifndef __INCcacheArmArchh
#define __INCcacheArmArchh

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Common definitions for Arm Arch7 Cache support
 */

/* cache levels: 7 possible, indexed starting from 0 */

#define ARMV7_CACHE_LVL1                0
#define ARMV7_CACHE_LVL2                1
#define ARMV7_CACHE_LVL3                2
#define ARMV7_CACHE_LVL4                3
#define ARMV7_CACHE_LVL5                4
#define ARMV7_CACHE_LVL6                5
#define ARMV7_CACHE_LVL7                6

/* cache types from Cache Level ID register (CLIDR) */

#define ARMV7_CLID_ICACHE_TYPE          0x1     /* instruction cache */
#define ARMV7_CLID_DCACHE_TYPE          0x2     /* data cache */
#define ARMV7_CLID_UCACHE_TYPE          0x4     /* unified cache */

/* cache types for Cache Size Selection register (CSSEL) */

#define ARMV7_CSSEL_ICACHE_TYPE         0x1     /* select instruction cache */
#define ARMV7_CSSEL_DCACHE_TYPE         0x0     /* select data or unified cache */

/* cache size info from Cache Size ID register (CCSIDR) */

#define ARMV7_CCSID_LINESIZE_MSK        0x7     /* line size bit mask */
#define ARMV7_CCSID_WAYNUM_MSK          0x1FF8  /* associativity bit mask */
#define ARMV7_CCSID_SETNUM_MSK          0xFFFE000  /* set number bit mask */
#define ARMV7_CCSID_MODE_MSK            0xF0000000  /* mode bit mask */

#define ARMV7_CCSID_WAYNUM_SHFT         3       /* associativity field shift */
#define ARMV7_CCSID_SETNUM_SHFT         13      /* set number field shift */

#define ARMV7_CCSID_MODE_WT             0x80000000  /* write-through mode */
#define ARMV7_CCSID_MODE_WB             0x40000000  /* write-back mode */
#define ARMV7_CCSID_MODE_RA             0x20000000  /* read-allocate mode */
#define ARMV7_CCSID_MODE_WA             0x10000000  /* write-allocate mode */

#if 1
#define ARMCACHE_NOT_COHERENT           TRUE
#define ARMCACHE_HAS_WRITETHROUGH       TRUE
#define ARMCACHE_HAS_MINICACHE          FALSE
#define ARMCACHE_NEEDS_IMB              TRUE
#define ARMCACHE_IS_PHYSICALLY_ADDRESSED TRUE
#endif

#ifndef _ASMLANGUAGE

IMPORT STATUS cacheArchIInvalidate (void * start, void * end, UINT32 step);
IMPORT STATUS cacheArchIInvalidateAll (void);
IMPORT STATUS cacheArchDFlush (void * start, void * end, UINT32 step);
IMPORT STATUS cacheArchDFlushPoU (void * start, void * end, UINT32 step);
IMPORT STATUS cacheArchDFlushAll (void);
IMPORT STATUS cacheArchDFlushAllPoU (void);
IMPORT STATUS cacheArchDInvalidate (void * start, void * end, UINT32 step);
IMPORT STATUS cacheArchDInvalidateAll (void);
IMPORT STATUS cacheArchDClear (void * start, void * end, UINT32 step);
IMPORT STATUS cacheArchDClearAll (void);
IMPORT size_t cacheArchLocSizeGet (void);
IMPORT size_t cacheArchLouSizeGet (void);
/* routine that locks interrupts according to the mask above */

IMPORT UINT32 cacheArchIntLock (void);
IMPORT void cacheArchIClearDisable (void);
IMPORT void cacheArchDClearDisable (void);
IMPORT UINT32 cacheArchSizeGet (UINT32 level, UINT32 type);
#if defined (_WRS_CONFIG_ARMV7_CORE_GENERIC) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A15) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A7) || \
    defined (_WRS_CONFIG_ARMV8_AARCH32_CORE_CTX_A57) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A8)
IMPORT UINT32 cacheArchL2CacrGet (void);
IMPORT void cacheArchL2CacrSet (UINT32 cacr);
#endif /* CORE_GENERIC || CORE_CTX_A15 || CORE_CTX_A7 || CORE_CTX_A8 */

#if defined (_WRS_CONFIG_ARMV7_CORE_GENERIC) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A15) || \
    defined (_WRS_CONFIG_ARMV8_AARCH32_CORE_CTX_A57) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A7)
IMPORT UINT32 cacheArchL2EcrGet (void);
IMPORT void cacheArchL2EcrSet (UINT32 ecr);
#endif /* CORE_GENERIC || CORE_CTX_A15 || CORE_CTX_A7 */


#if defined (_WRS_CONFIG_SMP) && defined (_WRS_CONFIG_COMPAT69_ARM_ARCH)
IMPORT VOID cacheArchSMPInit (void);
#endif /* _WRS_CONFIG_SMP && _WRS_CONFIG_COMPAT69_ARM_ARCH */

extern UINT32   cacheArchState;

#else   /* _ASMLANGUAGE */

#define LoUIS_MASK      0x00e00000      /* level of coherency mask of CLIDR */
#define LoUIS_SHIFT     20              /* level of coherency mask of CLIDR */
#define LoC_MASK        0x07000000      /* level of coherency mask of CLIDR */
#define LoC_SHIFT       23              /* level of coherency mask of CLIDR */
#define LoUU_MASK       0x38000000      /* level of coherency mask of CLIDR */
#define LoUU_SHIFT      26              /* level of coherency mask of CLIDR */

#if ARM_THUMB2
#   define ORR_REG_LSL_REG(rd, rn, rm, rs)      \
        LSL    r9, rm, rs   ;                   \
        ORR    rd, rn, r9
#define rl              r9
#else
#   define ORR_REG_LSL_REG(rd, rn, rm, rs)      \
    ORR    rd, rn, rm, LSL rs
#define rl              r8
#endif
/*
 * Common code for cache operations on entire data/unified caches,
 * performed by set/way to the point of given level.
 * This code is based on 'Example code for cache maintenance operations'
 * provided in "ARM Architecture Reference Manual ARMv7-A and ARMv7-R edition
 * (ARM DDI 0406)" .
 *
 * Registers used: For ARM ISA, r0-r8, though r4-r8 are saved/restored.
 *                 For THUMB-2, r0-r9, though r4-r9 are saved/restored.
 *                 Also, note that r0 is 0 when this code completes.
 */

#define _CORTEX_AR_ENTIRE_DATA_CACHE_OP_POX(crm, LoX) \
        MRC     p15, 1, r0, c0, c0, 1   /* r0 = Cache Lvl ID register info */;\
        ANDS    r3, r0, IMM LoX##_MASK  /* get level */;\
        MOV     r3, r3, LSR IMM LoX##_SHIFT      /* r3 = LoC << 1 */;\
        BEQ     5f                      ;\
                                        \
        MOV     r7, IMM 0               /* r7 = cache level << 1; start at 0 */;\
                                        \
1:                                      ;\
        AND     r1, r0, IMM 0x7         /* r1 = cache type(s) for this level */;\
        CMP     r1, IMM 2               ;\
        BLT     4f                      /* no data cache at this level */;\
                                        \
        MCR     p15, 2, r7, c0, c0, 0   /* select the Cache Size ID register */;\
        ISB                             /* ISB: sync change to Cache Size ID */;\
        MRC     p15, 1, r1, c0, c0, 0   /* r1 = current Cache Size ID info */;\
        AND     r2, r1, IMM 0x7         /* r2 = line length */;\
        ADD     r2, r2, IMM 4           /* add line length offset = log2(16 bytes) */;\
        LDR     r4, =0x3FF              ;\
        ANDS    r4, r4, r1, LSR IMM 3   /* r4 = (# of ways - 1); way index */;\
        CLZ     r5, r4            /* r5 = bit position of way size increment */;\
        LDR     r6, =0x00007FFF         ;\
        ANDS    r6, r6, r1, LSR IMM 13  /* r6 = (# of sets - 1); set index */;\
                                        \
2:                                      ;\
        MOV     r1, r4                  /* r1 = working copy of way number */;\
3:                                      ;\
        /* r8 = set/way operation data word: */ \
        ORR_REG_LSL_REG (r8, r7, r1, r5); \
                                        \
        ORR_REG_LSL_REG (r8, r8, r6, r2); \
                    ;\
        MCR     p15, 0, r8, c7, crm, 2  /* dcache operation by set/way */;\
                                        \
        SUBS    r1, r1, IMM 1           /* decrement the way index */;\
        BGE     3b                      ;\
        SUBS    r6, r6, IMM 1           /* decrement the set index */;\
        BGE     2b                      ;\
                                        \
4:                                      ;\
        ADD     r7, r7, IMM 2           /* increment cache index = level << 1 */;\
        CMP     r3, r7                  /* done when LoC is reached */;\
        MOVGT   r0, r0, LSR IMM 3       /* rt-align type of next cache level */;\
        BGT     1b                      ;\
                                        \
5:                                      ;\
        DSB                             ;\
        ISB                             ;

/*
 * Default, performed by set/way to the point of coherency (PoC).
 */

#define _CORTEX_AR_ENTIRE_DATA_CACHE_OP(crm)             \
        STMFD   sp!, {r4-rl}                            ;\
        _CORTEX_AR_ENTIRE_DATA_CACHE_OP_POX (crm, LoC)   \
        LDMIA   sp!, {r4-rl};

/*
 * Performed by set/way to the point of Unification Uniprocessor (PoUU).
 */

#define _CORTEX_AR_ENTIRE_DATA_CACHE_OP_PoU(crm)         \
        STMFD   sp!, {r4-rl}                            ;\
        _CORTEX_AR_ENTIRE_DATA_CACHE_OP_POX (crm, LoUU)  \
        LDMIA   sp!, {r4-rl};

/*
 * Performed by set/way to the point of Unification Inner Shareable (PoUIS).
 */

#define _CORTEX_AR_ENTIRE_DATA_CACHE_OP_PoUIS(crm)       \
        STMFD   sp!, {r4-rl}                            ;\
        _CORTEX_AR_ENTIRE_DATA_CACHE_OP_POX (crm, LoUIS) \
        LDMIA   sp!, {r4-rl};

/*
 * Safely disable and clear cache without data access in between.
 */

#define _CORTEX_AR_DATA_CACHE_CLEAR_DISABLE              \
        STMFD   sp!, {r4-rl}                            ;\
                                                         \
        MRC p15, 0, r0, c1, c0, 0                       ;\
        BIC r0, r0, IMM 4                               ;\
        MCR p15, 0, r0, c1, c0, 0                       ;\
                                                         \
        DSB                                             ;\
        ISB                                             ;\
                                                         \
        _CORTEX_AR_ENTIRE_DATA_CACHE_OP_POX(c14, LoC)    \
                                                         \
        LDMIA   sp!, {r4-rl};

#endif  /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif  /* __INCcacheArmArchh */
