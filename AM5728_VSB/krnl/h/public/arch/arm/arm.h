/* arm.h - ARM CPU flags etc */

/*
 * Copyright (c) 1996-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
12jul16,g_x  added Cortex-R support (F6411)
13oct15,gls  made CPU_CACHE_ALIGN_SIZE 64 bytes for 
             _WRS_CONFIG_ARMV7_CORE_GENERIC
02dec15,mpc  added Cortex-A57 support (US66795)
09oct15,yya  changed LPAE MMU attribute to 64 bits. (V7PRO-2477)
23apr15,cfm  removed _WRS_ARM_HAS_ASID, ARMARCH7 always support it. (V7PRO-1862)
19jan15,sye  added Cortex-A7 support. (US46486)
20feb14,cfm  updated copyright
30dec13,yjw  remove unsupported arch
21aug13,c_t  updated for vx7
08oct12,jdw  Update FAST MATH for ARMARCH7
14sep12,j_b  fix whitespace issues
28jun12,j_b  add Cortex-A15 cache support
09feb12,jdw  Update definitions for __ieee_status(), WIND00332894
01feb12,rec  WIND00329076 - cleanup assembly macros to be consistent
11aug11,jdw  Add support for ARMv7 core optimizations
23aug11,rec  WIND00292261 - transition to common power management API
15apr11,jdw  Enable cached page tables
13jan11,m_h  Define _CACHE_ALIGN_SIZE as cacheArchAlignSize for ARMARCH7
09jul10,fao  add endianness state bit define
21may10,m_h  Thumb-2 Support
02feb10,j_b  add _WRS_ARM_HAS_ASID macro, defined for ARMv6+
23sep09,j_b  add LDREX/STREX support for ARMARCH7
06jun09,mdo  Don't check IEEE status for FAST_MATH
25feb09,j_b  merge ARM SMP:
11jul07,j_b  add ARMMMU_11MPCORE support
11jul07,to   added ARM_HAS_LDREX_STREX feature macro
02jul07,m_h  Adding types for MPCore varient
27feb09,m_h  ARMARCH7 Support
12jun08,j_b  add Cortex-A MMU and Cortex-A/-R cache support
28apr08,m_h  Create macro for NOP after LDM^ and STM^
13jul06,m_h  MPU support
25jan06,m_h  Manzano support
10oct05,scm  correct SPR 111664
04apr05,jb   Adding ARMARCH6 and 1136 support
21mar05,mdo  Add define for ansiMath testing
02mar05,mdo  Add prototype for __ieee_status() and definitions for
             exception flags
10aug04,scm  remove MPU support...
25nov03,sn   Avoid gcc 3.3.2 preprocessor token pasting error
26aug03,rec  remove SYS_PWR_MGMT conditional
07jul03,rec  revert to non-portable kernel
01jul03,rec  code review comments
28may03,rec  Power Management
17dec02,jb   Adding ARM10 and 926E support
15jan02,m_h  ARMARCH5 thumb problem
05nov01,t_m  remove leading underscores via FUNC/VAR macros
05nov01,t_m  Undo underscores
19jul01,ed   Fixing to be compatible with Diab compiler.
17oct01,t_m  Merge in DIAB
10oct01,jb   Enabling removal of pre-pended underscores for new compilers
             (Diab/Gnu elf)
23jul01,scm  change XScale name to conform to coding standards...
11dec00,scm  replace references to ARMSA2 with XScale
16oct00,scm  reverse back I & F bits for SA2
01sep00,scm  add sa2 support...
08feb99,jpd  added further MMU/CACHE types.
20jan99,cdp  removed support for old ARM libraries.
24aug98,cdp  added MMU/cache types, ARM_HAS_MPU/ARM_THUMB, ARMST16/ARMST32.
21aug97,cdp  added _?_FUNCTION macros and HAS_HALFWORD_
08may96,cdp  written
*/

#ifndef __INCarmh
#define __INCarmh

#ifdef __cplusplus
extern "C" {
#endif

#undef  _ARCH_SUPPORTS_GCC
#define _ARCH_SUPPORTS_DCC

/* define to indicate ARM supports device power management */

#define _WRS_ARCH_HAS_DEV_PWR_MGMT

/*
 * For SMP, any shared data region that is created from the RAM need
 * to be memory coherent.
 * So we need to set the page attribute to MMU_ATTR_CACHE_COHERENCY.
 * The arches, need to specify this via the macro
 * VM_DEFAULT_CACHE_COHERENCY_ATTR
 */

#ifdef _WRS_CONFIG_SMP
    /* Limit SMP build to ARMARCH7 little endian */
#if !defined(ARMEB) && !defined(__ARMEB__)
#define VM_DEFAULT_CACHE_COHERENCY_ATTR MMU_ATTR_CACHE_COHERENCY
#else
#error SMP build is NOT supported by this architecture/tool combination
#endif /* !defined(ARMEB) && !defined(__ARMEB__) */
#endif /* _WRS_CONFIG_SMP */

/* Say VFP is Supported */

#define _WRS_HW_VFP_SUPPORT

/* physical addresses are represented with 64 bits for these CPUs */

#define _TYPE_PHYS_ADDR           typedef UINT64 PHYS_ADDR
#define _WRS_PHYS_ADDR_IS_64_BITS

/* variants that support the clz instruction, "count leading zeroes" */

#define _WRS_CPU_HAS_CLZ_INSTR

/* LPAE uses 64-bit MMU attribute */

#ifdef _WRS_CONFIG_ARM_LPAE
#define _WRS_MMU_HAS_64BIT_ARCH_ATTR
#endif /* _WRS_CONFIG_ARM_LPAE */

/* Make sure excArmLib.h also has same settings */

/* bits in the PSR */

#define V_BIT                   (1<<28)
#define C_BIT                   (1<<29)
#define Z_BIT                   (1<<30)
#define N_BIT                   (1<<31)
#define E_BIT                   (1<<9)
#define I_BIT                   (1<<7)
#define F_BIT                   (1<<6)
#define T_BIT                   (1<<5)


/* mode bits */

#define MODE_SYSTEM32           0x1F
#define MODE_UNDEF32            0x1B
#define MODE_ABORT32            0x17
#define MODE_SVC32              0x13
#define MODE_IRQ32              0x12
#define MODE_FIQ32              0x11
#define MODE_USER32             0x10
#define MODE_MONITOR            0x16

/* masks for getting bits from PSR */

#ifdef ARM_INTERWORKINGSUPPORT
#define MASK_MODE               0x0000003F
#else  /*ARM_INTERWORKINGSUPPORT*/
#define MASK_MODE               0x0000001F
#endif /*ARM_INTERWORKINGSUPPORT*/

#define MASK_32MODE             0x0000001F
#define MASK_SUBMODE            0x0000000F
#define MASK_INT                0x000000C0
#define MASK_CC                 0xF0000000

/* shifts to access bits in the PSR */

#define INT_MASK_SHIFT          6

/* The coprocessor number of the MMU System Control Processor */

#define CP_MMU                  15

/* check CPU/cache/MMU features */

/* Cache Line sizes */

#if defined (_WRS_CONFIG_ARMV7_CORE_CTX_A8) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A15) || \
    defined (_WRS_CONFIG_ARMV8_AARCH32_CORE_CTX_A57) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A7) || \
    defined (_WRS_CONFIG_ARMV7_CORE_GENERIC)

#define _CPU_CACHE_ALIGN_SIZE   64 /* Cortex A7, A8, A15 use 64 bytes */

#else /* !(A7 || A8 || A15 || A57) */

/*
 * Most ARM cores have a cache line size of 32 bytes.
 *
 * This can be overridden through use of the global variable
 * cacheArchAlignSize.  See below.
 */

#define _CPU_CACHE_ALIGN_SIZE   32

#endif /* A7 || A8 || A15 || A57 */

/*
 * The way in which _CACHE_ALIGN_SIZE is defined has been changed
 * somewhat and is no longer strictly related to the CPU type defined,
 * but is instead related to the cache type selected.  Therefore, it would
 * be better if drivers that need to know the cache line size, use this
 * variable, rather than the constant.  The variable will be initialized
 * to the appropriate cache-type-dependent value by the variant of the
 * cache library initialisation code.
 */

#ifndef _ASMLANGUAGE
extern int cacheArchAlignSize;
#endif  /* _ASMLANGUAGE */

/*
 * By default cache alignment for ARMv7 is 32 bytes.  This is accomplished by
 * assigning cacheArchAlignSize as _CPU_CACHE_ALIGN_SIZE where it is declared
 * in cacheArchVars.c.  But some cores can have a cache alignment that is not
 * 32 bytes.  These cores will adjust the global variable cacheAlignSize
 * during runtime initialization.  See cacheAimCortexA8LibInit for an example
 * of this.
 */

#undef _CACHE_ALIGN_SIZE
#define _CACHE_ALIGN_SIZE cacheArchAlignSize

/* this cpu family supports power management framework */

#define _WRS_ARCH_HAS_CPU_PWR_MGMT

#define ARM_HAS_HALFWORD_INSTRUCTIONS TRUE

#ifndef _WRS_CONFIG_ARMV7_CTX_R 
#define ARM_HAS_MPU FALSE
#define ARM_HAS_NO_MMU FALSE
#else
#define ARM_HAS_MPU TRUE
#define ARM_HAS_NO_MMU TRUE
#define ARMCPUMMULESS
#endif /* _WRS_CONFIG_ARMV7_CTX_R */

#define ARM_THUMB FALSE

#ifdef _WRS_CONFIG_THUMB2_ISA
#define ARM_THUMB2 TRUE
#else /* _WRS_CONFIG_THUMB2_ISA */
#define ARM_THUMB2 FALSE
#endif /* _WRS_CONFIG_THUMB2_ISA */

#if (ARM_THUMB2)
#define PORTABLE
#endif /* ARM_THUMB2 */

 /*
  * For ARMv5 and earlier, STM(2) and LDM(2) instructions
  * must not be immediately followed by an
  * instruction that accesses a banked register.
  * Banked registers are: R8-R14.  STM(2) and LDM(2) are
  * the variations of the OP codes that end with "^"
  * (load or store registers from a different mode -- user mode)
  * and do not load the PC (do not branch).
  *
  * One possible solution to this (suggested by the
  * ARM Architecture Reference Manual) is to insert
  * a NOP between the STM or LDM call and the access
  * to a banked register.
  *
  * Here is an example of where a NOP would be required
  * because sp is one of the banked registers:
  *
  *   LDMIA   r1, {sp, lr}^
  *   NOP
  *   LDR     r1, [sp, #SC_SAVE_USERSR]
  *
  * The asm macro ARM_NOP_AFTER_USER_LDM_STM is used for
  * inserting the NOP only if the version of the architecture
  * requires it.
  */

#define ARM_NOP_AFTER_USER_LDM_STM

/** Power management for ARM **/

/* The coprocessor number of the Power Mode Register */

#define CP_CORECTL          15

/* OPCODE for idle */

#define PWRMODE_IDLE        1

/* VFP related property */

#define VFP_VERSION         3   /* VFP version */
#define VFP_DOUBLE_PRECISE      /* use double precise VFP */
#undef  VFP_SINGLE_PRECISE      /* not use single precise VFP */
#define NEON_AVAILABLE          /* neon available */

/*
 * bellowing code should be put into vfpxxx.h, but for compatibility
 * reason, keep it in arm.h, so each child arch may keep the same define.
 */

/*
 * The default for ARMARCH7 is to use _WRS_CONFIG_FAST_MATH to
 * optimize builds created with VFP enabled. The flag to enable this behavior
 * is from the make fragments for a CPU/TOOL pair.
 *
 * Builds that use a SW FP tool chain should not use FAST MATH to get best
 * performance.
 *
 * Override VSB options here because VSB configuration does not detect the
 * tool chain / floating point configuration properly.
 */

#if defined(ARM_USE_VFP)
/* Vector floating point builds use FAST MATH */
#undef _WRS_CONFIG_FAST_MATH
#define _WRS_CONFIG_FAST_MATH       1
#else
/* Software floating point build does NOT use FAST MATH */
#undef _WRS_CONFIG_FAST_MATH
#endif /* defined(USE_ARM_FP) ... */

#if !defined(_WRS_CONFG_FAST_MATH) || (VFPVARIANT != VFPV3)

/* Define __ieee_status exception flags */
#define FE_IEEE_FLUSHZERO           (0x01000000)
#define FE_IEEE_ROUND_TONEAREST     (0x00000000)
#define FE_IEEE_ROUND_UPWARD        (0x00400000)
#define FE_IEEE_ROUND_DOWNWARD      (0x00800000)
#define FE_IEEE_ROUND_TOWARDZERO    (0x00C00000)
#define FE_IEEE_ROUND_MASK          (0x00C00000)
#define FE_IEEE_MASK_INVALID        (0x00000100)
#define FE_IEEE_MASK_DIVBYZERO      (0x00000200)
#define FE_IEEE_MASK_OVERFLOW       (0x00000400)
#define FE_IEEE_MASK_UNDERFLOW      (0x00000800)
#define FE_IEEE_MASK_INEXACT        (0x00001000)
#define FE_IEEE_MASK_ALL_EXCEPT     (0x00001F00)
#define FE_IEEE_INVALID             (0x00000001)
#define FE_IEEE_DIVBYZERO           (0x00000002)
#define FE_IEEE_OVERFLOW            (0x00000004)
#define FE_IEEE_UNDERFLOW           (0x00000008)
#define FE_IEEE_INEXACT             (0x00000010)
#define FE_IEEE_ALL_EXCEPT          (0x0000001F)

/* Define which of the IEEE status bits are reserved */
#define IEEE_RESERVED_MASK          (0xF63FE0E0)

#ifndef _ASMLANGUAGE
extern unsigned int __ieee_status (unsigned int mask, unsigned int flags);
#endif

/* Define for ansiMath Testing */
#define _WRS_ARM_IEEE_STATUS
#endif

#ifdef _WRS_CONFIG_ARMV7_CORE_GENERIC
#define CORTEXA5                    5
#define CPU_TYPE_CORTEXA7           7
#define CPU_TYPE_CORTEXA8           8
#define CPU_TYPE_CORTEXA9           9
#define CPU_TYPE_CORTEXA12          12
#define CPU_TYPE_CORTEXA15          15
#define CPU_TYPE_CORTEXA53          53
#define CPU_TYPE_CORTEXA57          57
#define CPU_TYPE_INVALIDE           -1
#endif /* _WRS_CONFIG_ARMV7_CORE_GENERIC */

#ifdef __cplusplus
}
#endif

#endif  /* __INCarmh */
