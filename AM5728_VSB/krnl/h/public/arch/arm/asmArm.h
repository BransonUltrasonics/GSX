/* asmArm.h - ARM assembler definitions header file */

/*
 * Copyright (c) 1996, 2004, 2010, 2012, 2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
12jul16,g_x  added Cortex-R support (F6411)
20apr15,cfm  added PLDGT. (V7PRO-1694)
18mar15,cfm  added MRRC and MCRR macro (V7PRO-1862)
19jan15,sye  Added descriptions of Cortex-A7 for _ARM_MIDR_PPN_GET. (US46486)
06jan14,yjw  Remove unsupport arch
16oct12,jdw  Add SPSR set macro (WIND00259770)
18jan12,rec  WIND00329076 - add FUNC_END, cleanup macros to be consistent
             with other arch defines.  Remove THUMB support.
09jan12,jdw  Add macro definitions for pld instructions
21may10,m_h  Thumb-2 Support
06jul04,md   added function definition macros
09may96,cdp  created
*/

#ifndef __INCasmArmh
#define __INCasmArmh

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _ASMLANGUAGE

/* Build for Gnu elf or Diab compiler */
/* useful, assembly-language macros */
#define FUNC(a) a
#define VAR(name) name

/*
 * These macros are used to declare assembly language symbols that need
 * to be typed properly(func or data) to be visible to the OMF tool.
 * So that the build tool could mark them as an entry point to be linked
 * correctly.
 */

#ifdef __DCC__
 #if ARM_THUMB2
 #define GTEXT(sym) FUNC(sym) ;  .type   FUNC(sym),@tfunc
 #else /* ARM_THUMB2 */
 #define GTEXT(sym) FUNC(sym) ;  .type   FUNC(sym),@function
 #endif /* ARM_THUMB2 */
 #define GDATA(sym) VAR(sym) ;  .type   VAR(sym),@object
#elif (defined __GNUC__) /* GNU macros */
 #define GTEXT(sym) FUNC(sym) ;  .type   FUNC(sym),%function
 #define GDATA(sym) VAR(sym) ;  .type   VAR(sym),%object
#else /* neither __DCC__ nor __GNUC__ */
 #error "error!!! unsupported compiler!!!"
#endif /* __DCC__ */

/* abstract assembler idiosyncrasies */

#define FUNC_LABEL(func)        func:
#define VAR_LABEL(name)         name:

#define FUNC_EXPORT(func)       .globl  GTEXT(func)
#define DATA_EXPORT(var)        .globl  GDATA(var)
#define FUNC_IMPORT(func)       .extern FUNC(func)
#define DATA_IMPORT(var)        .extern VAR(var)
#define FUNC_END(func)          .size   FUNC(func), . - FUNC(func)

/*
 * Using "#" in the middle of a complex #define generates garbage
 * passed to the assembler which results in a syntax error.  IMM is
 * used by this file in order to tell the assembler to use an immediate
 * value.
 */

#ifndef IMM
#define IMM #
#endif

#if ((!defined __GNUC__) && (!defined __DCC__))
 #error "error!!! unsupported assembler!!!"
#endif /* (defined __GNUC__) && (!defined __DCC__) */

/*
 * The DCC assembler has problems with spaces before the function label.
 * To avoid assembler parse problems, the FUNC_LABEL macros below are
 * not indented.
 */

#if ARM_THUMB2
#if (defined __GNUC__)
 #define        FUNC_BEGIN(func) \
    .syntax unified;\
    .code 16;\
    .balign 4;\
FUNC_LABEL(func)
#else /* !__GNUC__ */
 #define        FUNC_BEGIN(func) \
    .thumb;\
    .balign 4;\
FUNC_LABEL(func)
#endif /* __GNUC__ */
#else /* !ARM_THUMB2 */
 #define        FUNC_BEGIN(func) \
     .code 32;\
     .balign    4;\
FUNC_LABEL(func)
#endif /* ARM_THUMB2 */

/*
 * The following two macros are deprecated. Definitions are provided for
 * backward compatibility.
 */

#define _ARM_FUNCTION_CALLED_FROM_C FUNC_BEGIN
#define _ARM_FUNCTION FUNC_BEGIN

/* Define macros to use the PLD instruction on ARM v7 processors */

#define PLD(...)  pld     __VA_ARGS__
#define PLDGT(r, s, n)          \
        cmp     r, IMM n       ;\
        bcc     0f             ;\
        pld     [s, IMM n]     ;\
        0:

/*
 * The SPSR has changed and additional bits have been added as newer ARM
 * architecture versions have been released. The new bit definitions are
 * marked as RAZ/SBZP in older architecture revisions and shouldn't be
 * modified. Newer versions of the Diab compiler have changed the behavior of
 * the 'MSR spsr, <reg>' operation and no longer assumes modification of only
 * spsr_cf fields when specific mask fields are not specified. GCC translates
 * 'MSR spsr, <reg>' to only modify spsr_cf fields by default, if specific
 * fields are not specified. See the ARM Architecture Reference Manual for
 * definitions of the SPSR fields and MSR instruction behavior.
 *
 * The _ARM_SPSR_SET() macro abstracts arch and compiler issues from common
 * code.
 */

/*
 * For ARMv7 (ARM & Thumb2) SPSR <23:20> are RAZ/SBZP, all others are
 * defined... Write all bits.
 */

#define _ARM_SPSR_SET(reg) \
        MSR     spsr_cxsf, reg

/*
 * _ARM_MIDR_PPN_GET (reg)
 * Get the Primary Part number field in the MIDR register. Value is resulted
 * in <reg>.
 * Part Type            Return Value
 * Cortex-A5            #5
 * Cortex-A7            #7
 * Cortex-A8            #8
 * Cortex-A9            #9
 * Cortex-A15           #15
 */

#define _ARM_MIDR_PPN_GET(reg) \
        MRC     p15, 0, reg, c0, c0, 0  /* read main ID register */     ;\
        MOV     reg, reg, LSR IMM 4                                     ;\
        AND     reg, reg, IMM 0xff

/*
 * MRRC(coproc, opc, rt, rt2, crm)
 * MCRR(coproc, opc, rt, rt2, crm)
 * diab do not support MCRR, so use this macro instead.
 */

#define MRRC(coproc, opc, rt, rt2, crm) \
        .long   0xec500000 | (rt2 << 16) | (rt << 12) | (coproc << 8) | (opc << 4) | crm

#define MCRR(coproc, opc, rt, rt2, crm) \
        .long   0xec400000 | (rt2 << 16) | (rt << 12) | (coproc << 8) | (opc << 4) | crm

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif  /* __INCasmArmh */
