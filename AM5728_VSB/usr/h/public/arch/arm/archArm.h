/* archArm.h - Arm architecture specific header */

/*
* Copyright (c) 2003-2007, 2010, 2012, 2014-2015 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
13oct15,gls  made CPU_CACHE_ALIGN_SIZE 64 bytes for 
             _WRS_CONFIG_ARMV7_CORE_GENERIC
19jan15,sye  updated 64 bytes of cache line size for Cortex-A7. (US46486)
21oct14,cfm  defined _CPU_CACHE_ALIGN_SIZE for bALib.s (V7PRO-1293)
29mar12,rec  WIND00318272 - POSIX signals conformance
29jul10,pad  Moved extern C statement after include statements.
04may07,zl   updated portable library build configuration.
02feb06,mil  Updated for POSIX namespace conformance.
08feb05,scm  correct merge, remove INSTR define that was moved to
             archArmCommon.h.
02sep04,scm  correct macros based on setjmp...
02sep04,scm  add _WRS_FRAMEP_FROM_JMP_BUF & _WRS_RET_PC_FROM_JMP_BUF
06jul04,md   Added INSTR typedef
21jan04,scm  Adding PORTABLE definition for usr builds
11nov03,pes  written 
*/

#ifndef __INCarchArmh
#define __INCarchArmh

#include <arch/arm/archArmCommon.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Macros to obain FP and return PC from jmp_buf[] array.
 * On the user side, these macros are dependent on the setjmp()
 * implementation in the compiler library.
 *
 * The frame pointer represents the SP at the entry of setjmp(), and
 * the return PC is the next instruction address from where setjmp()
 * was called.
 */

#ifdef _WRS_CONFIG_CPU_ARMARCH8A

#define _WRS_FRAMEP_FROM_JMP_BUF(env)   ((char *) env[10])  /* fp(x29) */
#define _WRS_RET_PC_FROM_JMP_BUF(env)   ((INSTR *) env[11]) /* lr(x30) */

#else /* !_WRS_CONFIG_CPU_ARMARCH8A */

#define _WRS_FRAMEP_FROM_JMP_BUF(env)   ((char *) env[9])  /* sp */
#define _WRS_RET_PC_FROM_JMP_BUF(env)   ((INSTR *) env[10]) /* lr */

#endif /* _WRS_CONFIG_CPU_ARMARCH8A */

/* POSIX sigExcHandlerInstall needs some of the exception vector numbers */

#define EXC_OFF_RESET           0x00    /* reset */
#define EXC_OFF_UNDEF           0x04    /* undefined instruction */
#define EXC_OFF_SWI             0x08    /* software interrupt */
#define EXC_OFF_PREFETCH        0x0c    /* prefetch abort */
#define EXC_OFF_DATA            0x10    /* data abort */

#define ARM_HAS_HALFWORD_INSTRUCTIONS   TRUE    /* for bALib.s */

#if defined (_WRS_CONFIG_ARMV7_CORE_CTX_A8) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A15) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A7) || \
    defined (_WRS_CONFIG_ARMV7_CORE_GENERIC)

#define _CPU_CACHE_ALIGN_SIZE 64 /* Cortex A7, A8, A15 use 64 bytes */

#else /* !(A7 || A8 || A15) */

/*
 * Most ARM cores have a cache line size of 32 bytes.
 *
 * This can be overridden through use of the global variable
 * cacheArchAlignSize.  See below.
 */

#define _CPU_CACHE_ALIGN_SIZE 32

#endif /* A7 || A8 || A15  */

#ifdef __cplusplus
}
#endif

#endif /* __INCarchArmh */

