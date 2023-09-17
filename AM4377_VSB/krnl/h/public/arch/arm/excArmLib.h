/* excArmLib.h - ARM exception library header file */

/*
 * Copyright (c) 1996, 2000-2001, 2003, 2006, 2009, 2013-2014, 2016
 * Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
15dec16,cww  added _WRS_ARCH_HAS_EXCSIGLIB (V7COR-3939)
16dec16,cfm  added the prototype of excVBARGet (F7142)
11apr14,mpc  added DEBUG envent (US35179)
30dec13,yjw  remove unsupported arch
23mar09,m_h  Support for non-zero exception vector base
01feb06,jb   Adding VFP Support
26nov03,pes  Correct _WRS_IS_SUPV_EXC macro.
18nov03,pes  Add _WRS_IS_SUPV_EXC macro.
23jul01,scm  change XScale name to conform to coding standards...
11dec00,scm  replace references to ARMSA2 with XScale
16oct00,scm  reverse back I & F bits for SA2
15sep00,scm  update for SA2 support...
09may96,cdp  created
*/

#ifndef __INCexcArmLibh
#define __INCexcArmLibh

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE

/*
 * NOTE: if this structure changes, the TCB offsets in h/private/taskLibP.h
 *       must be updated. See also taskLib.h.
 */

typedef struct
    {
    UINT32      valid;          /* indicators that following fields are valid */
    UINT32      vecAddr;        /* exception vector address */
    INSTR *     pc;             /* PC */
    UINT32      cpsr;           /* CPSR */
    } EXC_INFO;

#define _WRS_IS_SUPV_EXC() (((excInfo.cpsr & MASK_MODE) == MODE_USER32) ? FALSE : TRUE)

extern FUNCPTR     excExcepHook;        /* add'l rtn to call when exceptions occur */
extern VOIDFUNCPTR _func_excBreakpoint; /* called to handle breakpoints */

extern void   excVecBaseSet (UINT32 base);  /* non-zero exception base address */
extern UINT32 excVBARGet (void);            /* internal use only */

extern STATUS  	excIntConnect (VOIDFUNCPTR *, VOIDFUNCPTR);
extern void	excVecSet (FUNCPTR *, FUNCPTR);
extern FUNCPTR	excVecGet (FUNCPTR *);

#endif	/* ASMLANGUAGE */

/*
 * Indicates the presence of excSigArchLib. This can be removed once all
 * supported architectures have split the signal related exception handling
 * code into their own modules
 */

#define _WRS_ARCH_HAS_EXCSIGLIB

/* exception information valid bits */

#define EXC_INFO_VECADDR        0x01    /* vector is valid */
#define EXC_INFO_PC             0x02    /* PC is valid */
#define EXC_INFO_CPSR           0x04    /* CPSR is valid */
#define EXC_INFO_VFPCSR         0x08    /* VFP CSR is valid */


/* exception vector addresses */

#define EXC_OFF_RESET           0x00    /* reset */
#define EXC_OFF_UNDEF           0x04    /* undefined instruction */
#define EXC_OFF_SWI             0x08    /* software interrupt */
#define EXC_OFF_PREFETCH        0x0c    /* prefetch abort */
#define EXC_OFF_DATA            0x10    /* data abort */
#define EXC_OFF_IRQ             0x18    /* interrupt */
#define EXC_OFF_FIQ             0x1C    /* fast interrupt */

/* Exception Vector Base */

#define ARM_EXC_VEC_BASE        0x00

#ifndef _WRS_CONFIG_ARM_LPAE
#define IFSR_DEBUG_MASK         0x40F
#define IFSR_DEBUG              0x02
#define DFSR_DEBUG_MASK         0x40F
#define DFSR_DEBUG              0x02
#else  /* _WRS_CONFIG_ARM_LPAE */
#define IFSR_DEBUG_MASK         0x3F
#define IFSR_DEBUG              0x22
#define DFSR_DEBUG_MASK         0x3F
#define DFSR_DEBUG              0x22
#endif /* !_WRS_CONFIG_ARM_LPAE */

#ifdef __cplusplus
}
#endif

#endif  /* __INCexcArmLibh */

