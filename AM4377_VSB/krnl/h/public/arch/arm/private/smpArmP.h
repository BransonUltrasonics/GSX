/* private/smpArmP.h - ARM-specific definitions for SMP support */

/*
 * Copyright (c) 2007-2012, 2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Any compilation using definitions from this file will not *
 * be binary-compatible between the UP and SMP environments. *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
modification history
--------------------
17oct16,wqi  added cpuIndexMap only when clusters and smp enabled
23aug16,mpc  added ARM64 support (F5261)
02dec15,wqi  added cortex-a57 support (US66795)
07jan14,zgl  added LPAE support.
10sep12,ggm  updated for tes.
02dec11,jdw  Add _fpStatus per WIND00129622
30nov10,j_b  add unified FIQ/IRQ handling support
29jul10,pad  Moved extern C statement after include statements.
07jul10,jdw  Add _WRS_ARCH_HAS_OBJ_SAFE
21may10,m_h  Thumb-2 Support
15may09,gls  moved VX_INSTR_BARRIER() to vxAtomicArchLibCommon.h
25feb09,j_b  merge ARM SMP support:
03feb09,j_b  define _WRS_SNOOP_NEEDS_DMA_MALLOC_SUPPORT so
             cacheAimArch6DmaMalloc/Free are used with CACHE_SNOOP_ENABLE
04oct07,j_b  define _WRS_ARCH_REQUIRES_CACHE_TEXT_UPDATE_CPC
31aug07,m_h  Fix _ARM_PER_CPU_* macros for CPU != 0
18jun07,m_h  syntax error non-SMP _ARM_CPU_INDEX_GET
18sep08,gls  added _WRS_KERNEL_LOCK_VAR_ALIGN
27aug08,jpb  Renamed VSB header file
24jun08,jpb  Added include path for kernel configurations options set in
             vsb.  Renamed _WRS_VX_SMP to _WRS_CONFIG_SMP.
20jun08,zl   replaced _WRS_ASM() with _WRS_BARRIER()
10jan08,gls  added VX_INSTR_BARRIER()
09may07,lei  renamed reschedNeeded to reschedMode.
09may07,m_h  Fix per-CPU SMP macros, cleanup kernel global offsets
03may07,m_h  SMP definitions
20apr07,m_h  SMP Private definitions
04apr07,m_h  written.
*/

#ifndef __INCsmpArmPh
#define __INCsmpArmPh

#ifdef _WRS_CONFIG_CPU_ARMARCH8A
#include <arch/arm/private/smpArmv8P.h>
#else /* !_WRS_CONFIG_CPU_ARMARCH8A */
#include <vsbConfig.h>

#ifdef __cplusplus
extern "C" {
#endif
 
#ifdef _WRS_CONFIG_SMP

/*
 * ARM has been adapted to use the new object safe count feature for SMP.
 * When all architectures have been adapted this can be removed from
 * the code.
 */

#define _WRS_ARCH_HAS_OBJ_SAFE

/*
 * Much like the _WRS_WIND_VARS_ALIGN the _WRS_KERNEL_LOCK_VAR_ALIGN macro
 * must be defined for the common CPU build variants (e.g. PPC32, MIPS32, 
 * XSCALE, etc), representing the maximum cache line size of the variants of 
 * the respective arch. 
 *
 * For ARM this is 128 bytes.
 */

#define _WRS_KERNEL_LOCK_VAR_ALIGN  128

/* Trigger software cacheTextUpdate CPC */

#define _WRS_ARCH_REQUIRES_CACHE_TEXT_UPDATE_CPC

/* ARM specific extension fields to the WIND_VARS struct */

#ifndef _ASMLANGUAGE

typedef struct wind_vars_arch
    {
    long     cpu_intNestingLevel;
    long     cpu_maxIntNestingLevel;
    UINT32   cpu_cacheArchState;
    char *   cpu_undefSaveArea;
    char *   cpu_abortSaveArea;
    char *   cpu_swiSaveArea;
    char *   cpu_irqStack;
    UINT32   cpu_fpStatus;
#ifdef _WRS_CONFIG_UNIFIED_FIQ_IRQ
    long     cpu_fiqNestingLevel;
    long     cpu_maxFiqNestingLevel;
    char *   cpu_fiqStack;
#endif /* _WRS_CONFIG_UNIFIED_FIQ_IRQ */
    } WIND_VARS_ARCH;

#define _WRS_WIND_VARS_ARCH WIND_VARS_ARCH cpu_archVars

/* macro to access the arch-specific fields of the vxKernelVars struct */
#define _WRS_WIND_VARS_ARCH_ACCESS(glob)         \
                      _WRS_KERNEL_GLOBAL_ACCESS (archVars.cpu_##glob)

#define _WRS_WIND_VARS_ARCH_SET(cpuid,glob,var)  \
                      _WRS_KERNEL_CPU_GLOBAL_SET(cpuid,archVars.cpu_##glob,var)

#define _WRS_WIND_VARS_ARCH_GET(cpuid,glob)      \
                      _WRS_KERNEL_CPU_GLOBAL_GET(cpuid,archVars.cpu_##glob)

#define _WRS_ARCH_LOCKED_GLOBAL_SET(glob, var)   \
                      _WRS_KERNEL_LOCKED_GLOBAL_SET(archVars.cpu_##glob,var)

#define _WRS_ARCH_LOCKED_GLOBAL_GET(glob, var)   \
                      _WRS_KERNEL_LOCKED_GLOBAL_GET(archVars.cpu_##glob,var)


#ifdef _WRS_CONFIG_COMPAT69_ARM_ARCH
#undef  _WRS_ARCH_SUPPORTS_VX_CPU_STATE_INIT
#else
#define _WRS_ARCH_SUPPORTS_VX_CPU_STATE_INIT

typedef struct
    {
    FUNCPTR         pspEntry;      /* Address of 'C' based startup code */
    ULONG           stack;         /* Stack pointer for startup */
    FUNCPTR         kernelEntry;   /* kernel entry point */
#ifndef _WRS_CONFIG_ARM_LPAE
    UINT32          ttbr;          /* Translation Table Base */
#else /* _WRS_CONFIG_ARM_LPAE */
    UINT32          ttbcr;         /* Translation Table Base Control Register */
    UINT64          ttbr;          /* 64 bits Translation Table Base */
#endif /* !_WRS_CONFIG_ARM_LPAE */
    } ARM_MPCORE_STARTUP_ARGS;

IMPORT ARM_MPCORE_STARTUP_ARGS sysMpCoreArgs [VX_MAX_SMP_CPUS];

#endif

#else /* !_ASMLANGUAGE */

/* offsets required for assembly macros */

#define WIND_VARS_ARCH_BASE     (_WRS_WIND_VARS_OFFSET_archVars)

#endif /* _ASMLANGUAGE */

/*
 * _WRS_WIND_VARS_ALIGN sets the alignment constraint for vxKernelVars.  The
 * value must be: a power of two no smaller than sizeof(vxKernelVars), and a
 * multiple of the cache line size (which is 32) on every ARM variant that
 * supports SMP.
 */
#define _WRS_WIND_VARS_ALIGN            128
#define _WRS_WIND_VARS_ALIGN_SHIFT      7

/*
 * _ARM_ATOMIC_OPS_SEPARATION defines the minimum desirable separation between
 * words on which SMP-atomic operations (including spinlock) will be
 * performed.  This is the cache line size on ARM MP.
 */

#define _ARM_ATOMIC_OPS_SEPARATION      _CPU_CACHE_ALIGN_SIZE

# ifdef _CPU_CACHE_ALIGN_SIZE
/*
 * Validate _WRS_WIND_VARS_ALIGN vs current build's _CPU_CACHE_ALIGN_SIZE
 *
 * If any SMP-enabled variant fails this test, _WRS_WIND_VARS_ALIGN must be
 * increased.  Since this limit also applies to any subsequent arch-only
 * release against the same generic binary release, it may be desirable to
 * allow for growth.
 */


#  if (_CPU_CACHE_ALIGN_SIZE > _WRS_WIND_VARS_ALIGN)
#error Must increase "_WRS_WIND_VARS_ALIGN" to at least _CPU_CACHE_ALIGN_SIZE
#  endif  /* _CPU_CACHE_ALIGN_SIZE > _WRS_WIND_VARS_ALIGN */

/*
 * Sanity check on _ARM_ATOMIC_OPS_SEPARATION
 *
 * A miss here generates a warning instead of an error, since it is merely
 * likely (not inevitable) that the lwarx/stwcx. granularity matches the
 * cache line size; also the performance penalty is less.  (Two instances of
 * vxKernelVars within the same cache line will cause continual interference,
 * but two atoms within the same reservation granule will only occasionally
 * cause additional trips around the lwarx/stwcx. loop due to loss of the
 * reservation to an unrelated store.)
 */

#  if (_CPU_CACHE_ALIGN_SIZE > _ARM_ATOMIC_OPS_SEPARATION)
#warning Check "_ARM_ATOMIC_OPS_SEPARATION" setting; it likely should be at least _CPU_CACHE_ALIGN_SIZE
#  endif  /* _CPU_CACHE_ALIGN_SIZE > _ARM_ATOMIC_OPS_SEPARATION */
# endif /* _CPU_CACHE_ALIGN_SIZE */

/*
 * Just call the (assembly) function for now, but to optimize
 * performance this should eventually become a C-callable
 * _WRS_ASM construct (whose syntax varies depending on TOOL).
 */

#define _WRS_CPU_INDEX_GET()    vxCpuIndexGet()
#else /* _WRS_CONFIG_SMP */

/* macro to access the arch-specific fields */
#define _WRS_WIND_VARS_ARCH_ACCESS(glob)          \
                      _WRS_KERNEL_GLOBAL_ACCESS (glob)

#define _WRS_WIND_VARS_ARCH_SET(ignored,glob,var) \
                      _WRS_KERNEL_CPU_GLOBAL_SET(ignored,glob,var)

#define _WRS_WIND_VARS_ARCH_GET(ignored,glob)      \
                      _WRS_KERNEL_CPU_GLOBAL_GET(ignored,archVars.cpu_##glob)

#define _WRS_ARCH_LOCKED_GLOBAL_SET(glob, var)   \
                      _WRS_KERNEL_LOCKED_GLOBAL_SET(glob,var)

#define _WRS_ARCH_LOCKED_GLOBAL_GET(glob, var)   \
                      _WRS_KERNEL_LOCKED_GLOBAL_GET(glob,var)


#define _WRS_CPU_INDEX_GET()    0
#endif /* _WRS_CONFIG_SMP */


#ifdef _ASMLANGUAGE 

#include <private/windLibP.h>

# ifdef _WRS_CONFIG_SMP

#include <private/kernelLockLibP.h>

/* 
 * Sticking a "#" in the middle of a complex #define generates garbage 
 * passed to the assembler which results in a syntax error.  ARM_HASH is
 * used by this file in order to tell the assembler to use an immediate
 * value.
 */

#define ARM_HASH #

#define _ARM_WIND_VARS_OFFSET(label) _WRS_WIND_VARS_OFFSET_##label
#define _ARM_WIND_VARS_OFFSET_reschedMode      \
                      #_WRS_WIND_VARS_OFFSET_reschedMode

/* ARM specific per-CPU globals */
#define _WRS_WIND_VARS_OFFSET_intNestingLevel    \
                      (_WRS_WIND_VARS_OFFSET_archVars+0x0)

#define _WRS_WIND_VARS_OFFSET_maxIntNestingLevel    \
                      (_WRS_WIND_VARS_OFFSET_archVars+0x4)

#define _WRS_WIND_VARS_OFFSET_cacheArchState    \
                      (_WRS_WIND_VARS_OFFSET_archVars+0x8)

#define _WRS_WIND_VARS_OFFSET_undefSaveArea   \
                      (_WRS_WIND_VARS_OFFSET_archVars+0xC)


#define _WRS_WIND_VARS_OFFSET_abortSaveArea   \
                      (_WRS_WIND_VARS_OFFSET_archVars+0x10)


#define _WRS_WIND_VARS_OFFSET_swiSaveArea   \
                      (_WRS_WIND_VARS_OFFSET_archVars+0x14)


#define _WRS_WIND_VARS_OFFSET_irqStack   \
                      (_WRS_WIND_VARS_OFFSET_archVars+0x18)

#define _WRS_WIND_VARS_OFFSET_fpStatus   \
                      (_WRS_WIND_VARS_OFFSET_archVars+0x1C)

#ifdef _WRS_CONFIG_UNIFIED_FIQ_IRQ

#define _WRS_WIND_VARS_OFFSET_fiqNestingLevel    \
                      (_WRS_WIND_VARS_OFFSET_archVars+0x20)

#define _WRS_WIND_VARS_OFFSET_maxFiqNestingLevel    \
                      (_WRS_WIND_VARS_OFFSET_archVars+0x24)

#define _WRS_WIND_VARS_OFFSET_fiqStack   \
                      (_WRS_WIND_VARS_OFFSET_archVars+0x28)

#endif /* _WRS_CONFIG_UNIFIED_FIQ_IRQ */


#define _ARM_CPU_ID_MASK #0xF

#define ARM_WIND_VARS_ALIGN_SHIFT #(_WRS_WIND_VARS_ALIGN_SHIFT)

/* Using CPU ID, get the index into the CPU's vxKernelVars */
/* Note: reading CPU ID requires CPU privileged mode */

#ifdef _WRS_CONFIG_MULTI_CLUSTERS

#define _ARM_CPU_INDEX_GET(r) 			   \
        stmfd	sp!, {r5} 			 ; \
        stmfd	sp!, {r0,r1,r2,r3,r4}    	 ; \
        MRC p15, 0, r0, c0, c0, 5        	 ; \
        bic	r0, r0, IMM 0xff000000		 ; \
        mov	r3, IMM 0			 ; \
        ldr	r1, =cpuIndexMap		 ; \
99999:                                           ; \
        ldr	r4, [r1], IMM 4			 ; \
        mov	r2, r3				 ; \
        cmp	r4, r0				 ; \
        beq	55555f  			 ; \
        add	r3, r3, IMM 1			 ; \
        cmp	r3, IMM VX_MAX_SMP_CPUS		 ; \
        blt	99999b				 ; \
        mvn	r2, IMM 0			 ; \
55555:						 ; \
        str  r2,[sp, IMM 20]        	 	 ; \
        ldmfd	sp!, {r0,r1,r2,r3,r4}    	 ; \
        ldmfd	sp!, {r}

#else  /* !_WRS_CONFIG_MULTI_CLUSTERS */

#define _ARM_CPU_INDEX_GET(r) \
        MRC  p15, 0, r, c0, c0, 5        ; \
        AND  r, r, _ARM_CPU_ID_MASK

#endif /* _WRS_CONFIG_MULTI_CLUSTERS */

/* place the address of label into register r */
#define _ARM_PER_CPU_ADRS_GET(r, scratch, label)                  \
        _ARM_CPU_INDEX_GET(r)                                   ; \
        LDR  scratch, L$_vxKernelVars                           ; \
        ADD  scratch, scratch, r, LSL ARM_WIND_VARS_ALIGN_SHIFT ; \
        ADD  r, scratch, ARM_HASH _ARM_WIND_VARS_OFFSET(label)  ; \

/*
 * In some configurations (e.g. Thumb-2) there are special restrictions on
 * usage of the sp register.  Also there appears to be a defect in some
 * versions of the Wind River assembler related to similar use of the lr
 * register.  So use these macros for setting up sp or lr.
 */

#define _ARM_PER_CPU_ADRS_GET_SPLR(r_sp, scratch, label)   \
        _ARM_PER_CPU_ADRS_GET(r_sp, scratch, label)

/* place the value at label into register r */
#define _ARM_PER_CPU_VALUE_GET_SPLR(r_sp, scratch, label)  \
        _ARM_PER_CPU_VALUE_GET(r_sp, scratch, label)

/* place the value at label into register r */
#define _ARM_PER_CPU_VALUE_GET(r,scratch,label)                        \
        _ARM_CPU_INDEX_GET(r)                                        ; \
        LDR  scratch, L$_vxKernelVars                                ; \
        ADD  scratch, scratch, r, LSL ARM_WIND_VARS_ALIGN_SHIFT      ; \
        ADD  scratch, scratch, ARM_HASH _ARM_WIND_VARS_OFFSET(label) ; \
        LDR  r, [scratch]

/* place value at label in rv, also place address at label in ra */
#define _ARM_PER_CPU_VALUE_AND_ADRS_GET(rv,ra,label)           \
        _ARM_CPU_INDEX_GET(rv)                               ; \
        LDR  ra, L$_vxKernelVars                             ; \
        ADD  ra, ra, rv, LSL ARM_WIND_VARS_ALIGN_SHIFT       ; \
        ADD  ra, ra, ARM_HASH _ARM_WIND_VARS_OFFSET(label)   ; \
        LDR  rv, [ra]

/*
 * ro = register to store kernel lock owner
 * ri = register to store cpu index
 *
 * typedef struct smpLock
 * {
 * volatile int state;         /@ state of lock (free/taken) @/
 * volatile int owner;         /@ CPU which owns the lock @/
 * int          count;         /@ recursive ownership nesting count @/
 * int          options;       /@ options applicable to this lock @/
 */

#define _ARM_KERNEL_LOCK_OWNER_AND_INDEX_GET(ro, ri)   \
        _ARM_CPU_INDEX_GET(ri)                                 ; \
        LDR     ro, L$_kernelStateLock                         ; \
        ADD     ro, ro, ARM_HASH _WRS_KERNEL_LOCK_OFFSET_cpuIndex       ; \
        LDR     ro, [ro]        /* kernel spin lock owner */   ; \


# else /* _WRS_CONFIG_SMP */

#define IMM0 #0
/* Macro unconditionally returns 0 when built for UP */
#define _ARM_CPU_INDEX_GET(r)   MOV     r, IMM0

/*
 * WARNING:  UP build assumes that scratch register is not used.
 *           There is only one scratch register available in
 *           excEnterCommon
 */

/* place the address of label into register r */
#define _ARM_PER_CPU_ADRS_GET(r, scratch, label)   \
        LDR     r, L$_##label

/* place the value at label into register r */
#define _ARM_PER_CPU_VALUE_GET(r, scratch, label)  \
        LDR     r, L$_##label           ; \
        LDR     r, [r]

/* place value at label in ra, also place address at label in rv */
#define _ARM_PER_CPU_VALUE_AND_ADRS_GET(rv,ra,label)  \
        LDR     ra, L$_##label          ; \
        LDR     rv, [ra]


/*
 * In some configurations (e.g. Thumb-2) there are special restrictions on
 * usage of the sp register.  Also there appears to be a defect in some
 * versions of the Wind River assembler related to similar use of the lr
 * register.  So use these macros for setting up sp or lr.
 */

#  if (ARM_THUMB2)
/* place the address of label into register r */
#define _ARM_PER_CPU_ADRS_GET_SPLR(r_sp, scratch, label)   \
        LDR     scratch, L$_##label           ; \
        MOV     r_sp, scratch

/* place the value at label into register r */
#define _ARM_PER_CPU_VALUE_GET_SPLR(r_sp, scratch, label)  \
        LDR     scratch, L$_##label           ; \
        LDR     r_sp, [scratch]

#  else /* (ARM_THUMB2) */
/* place the address of label into register r */
#define _ARM_PER_CPU_ADRS_GET_SPLR(r_sp, scratch, label)   \
        _ARM_PER_CPU_ADRS_GET(r_sp, scratch, label)

/* place the value at label into register r */
#define _ARM_PER_CPU_VALUE_GET_SPLR(r_sp, scratch, label)  \
        _ARM_PER_CPU_VALUE_GET(r_sp, scratch, label)
#  endif /* (ARM_THUMB2) */

# endif /* _WRS_CONFIG_SMP */
#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif
#endif /* _WRS_CONFIG_CPU_ARMARCH8A */
#endif /* __INCsmpArmPh */
