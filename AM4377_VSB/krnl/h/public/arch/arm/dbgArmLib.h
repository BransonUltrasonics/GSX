/* dbgArmLib.h - header file for ARM-dependent part of debugger */

/*
 * Copyright 1996-1997, 2004-2005, 2010, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
08apr14,mpc  added hardware breakpoint support (US35179)
20feb14,cfm  code clean
26jul10,jdw  Update with support for THUMB2
29apr10,pad  Moved extern C statement after include statements.
24feb05,jb   Correct definition of SYSCALL_INST
09dec04,scm  resolve DBG_SYSCALL_SIZE warning, define DBG_SYSCALL_INST
11feb04,elg  Add syscall instruction size macro.
12nov98,cdp  added support for ARM generic Thumb library.
20apr98,dbt  modified for new breakpoint scheme
24apr97,cdp  added Thumb (ARM7TDMI_T) support;
             changed DBG_BREAK_INST for non-Thumb processors.
09may96,cdp  created
*/

#ifndef __INCdbgArmLibh
#define __INCdbgArmLibh

#include <esf.h>
#include <vsbConfig.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BREAK_ESF               ESF
#define TRACE_ESF               ESF

#define DBG_SYSCALL_SIZE        (sizeof (INSTR) / sizeof (INSTR))

#if (ARM_THUMB2)
#define DBG_SYSCALL_INST        0xDF00      /* SVC */
#define DBG_SYSCALL_MASK        0xFF00
#else  /* (ARM_THUMB2) */
#define DBG_SYSCALL_INST        0xEF0000FA  /* SVC (formerly SWI) */
#endif /* (ARM_THUMB2) */

#if (ARM_THUMB2)
/*
 * For Thumb2 we can not support cret() without a valid frame pointer so
 * support is disabled here.
 */
#define DBG_CRET FALSE
#endif /* (ARM_THUMB2) */

#if (ARM_THUMB) || (ARM_THUMB2)

#define DBG_BREAK_INST          0xDEFE      /* The undefined instruction used as
                                             * a breakpoint for Thumb-state */
#define DBG_INST_ALIGN          2
#else /* (ARM_THUMB) || (ARM_THUMB2) */

#define DBG_BREAK_INST          0xE7FDDEFE  /* The undefined instruction used as
                                             * a breakpoint for ARM-state */

#define DBG_INST_ALIGN          4
#endif   /* (ARM_THUMB) || (ARM_THUMB2) */

#define DBG_NO_SINGLE_STEP      1           /* no hardware trace support */

#ifdef _WRS_CONFIG_ARM_DBG_HARDWARE_BP

/* hardware breakpoint support */

#define DBG_HARDWARE_BP		1           /* hardware breakpoint support */

#else /* !_WRS_CONFIG_ARM_DBG_HARDWARE_BP */

#define DBG_HARDWARE_BP		0           /* hardware breakpoint unsupport */

#endif  /* _WRS_CONFIG_ARM_DBG_HARDWARE_BP */

#if DBG_HARDWARE_BP    
#define BRK_INST            (0x00)          /* instruction hardware breakpoint   */

#define BRK_R_MASK          (0x1)           /* read mask */
#define BRK_W_MASK          (0x1 << 1)      /* write mask */

#define BRK_DATAW           2               /* write */
#define BRK_DATAR           1               /* read */
#define BRK_DATARW          3               /* read/write */
#define DEFAULT_HW_BP       (BRK_DATARW)    /* default hardware breakpoint */

#define BRK_HARDWARE        (0x10)          /* hardware breakpoint bit */
#define BRK_HARDMASK        (0x0f)          /* hardware breakpoint mask */

#define MAX_HW_BP_NUM       16              /* breakpoint number  */
#define MAX_HW_WP_NUM       16              /* watchpoint number */

#define DBGBVR_BASE         64
#define DBGBCR_BASE         80
#define DBGWVR_BASE         96
#define DBGWCR_BASE         112

#define DSCR_OFFSET         0x88
#define BVR_OFFSET	    0x100
#define BCR_OFFSET	    0x140
#define WVR_OFFSET	    0x180
#define WCR_OFFSET	    0x1c0

#ifndef _ASMLANGUAGE

/* hardware breakpoint registers */

typedef struct          /* DBG_REGS */
    {
    UINT32              bvr[MAX_HW_BP_NUM];   /* breakpoints value */
    UINT32              bcr[MAX_HW_BP_NUM];   /* breakpoints control */
    UINT32              wvr[MAX_HW_WP_NUM];   /* watchpoints value */
    UINT32              wcr[MAX_HW_WP_NUM];   /* watchpoints control */
    } DBG_REGS;

#endif /* !_ASMLANGUAGE */

#endif /* DBG_HARDWARE_BP */

#ifdef __cplusplus
}
#endif

#endif  /* __INCdbgArmLibh */
