/* intArmLib.h - ARM-specific interrupt library header file */

/*
 * Copyright (c) 1997-1998, 2007-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03dec10,j_b  add unified FIQ/IRQ handling support
23oct09,j_b  fix _inline_intCpuMicroUnlock for gnu
18may09,j_b  add _inline_intCpuMicroLock/Unlock support
25feb09,j_b  merge ARM SMP support:
             21jun07,jb3  Convert to vxBus
16jun08,zl   added memory clobber for GNUC version of inline routines.
20may08,j_b  fix memory accesses outside lock in
             _inline_intCpuLock/Unlock for gnu (WIND00121559)
07aug07,jb3  Defect00095620 wrong gnu assembler instruction.
21jun07,zl   updated inline build configuration.
22aug03,to   added inline intLock()/intUnlock().
20aug98,cdp  added intIFLock(), intIFUnlock().
25nov97,cdp  created.
*/

#ifndef __INCintArmLibh
#define __INCintArmLibh

#ifdef __cplusplus
extern "C" {
#endif

/* interrupt model feature selection bits (OR'd together) */

#define INT_NON_PREEMPT_MODEL 0
#define INT_PREEMPT_MODEL 1
#define INT_HDWE_LOCK_LVL 2


#ifndef _ASMLANGUAGE

/* inline intCpuLock()/intCpuUnlock() and intCpuMicroLock/Unlock() */

#if	!(ARM_THUMB)
#if	defined (__DCC__)

__asm volatile int _inline_intCpuLock ()
{
! "r1"						/* r1 clobbered */
	mrs		r1, cpsr		/* get current status */
#ifdef _WRS_CONFIG_UNIFIED_FIQ_IRQ
	and		r0, r1, #0xC0		/* return current I & F in r0 */
	orr		r1, r1, #0xC0		/* set I & F (1<<7 | 1<<6) */
#else
	and		r0, r1, #0x80		/* return current I_BIT in r0 */
	orr		r1, r1, #0x80		/* set I_BIT (1<<7) */
#endif /* _WRS_CONFIG_UNIFIED_FIQ_IRQ */
	msr		cpsr, r1		/* set new status */
}

__asm volatile void _inline_intCpuUnlock (int key)
{
% reg key;					/* input: key in r0 */
! "r0", "r1"					/* r0, r1 clobbered */
	mrs		r1, cpsr		/* get current status */
#ifdef _WRS_CONFIG_UNIFIED_FIQ_IRQ
	bic		r1, r1, #0xC0		/* clear I & F (1<<7 | 1<<6) */
	and		r0, key, #0xC0		/* get passed I & F values */
#else
	bic		r1, r1, #0x80		/* clear I_BIT (1<<7) */
	and		r0, key, #0x80		/* get passed I_BIT value */
#endif /* _WRS_CONFIG_UNIFIED_FIQ_IRQ */
	orr		r1, r1, r0		/* or in passed value */
	msr		cpsr,r1			/* set new status */
}

__asm volatile int _inline_intCpuMicroLock ()
{
! "r1"						/* r1 clobbered */
	mrs		r0, cpsr		/* get, return current status */
#ifdef _WRS_CONFIG_UNIFIED_FIQ_IRQ
	orr		r1, r0, #0xC0		/* set I & F (1<<7 | 1<<6) */
#else
	orr		r1, r0, #0x80		/* set I_BIT (1<<7) */
#endif /* _WRS_CONFIG_UNIFIED_FIQ_IRQ */
	msr		cpsr_c, r1		/* set new status */
}

__asm volatile void _inline_intCpuMicroUnlock (int key)
{
% reg key;					/* input: key in r0 */
	msr		cpsr_c, key		/* restore previous status */
}

#elif	defined (__GNUC__)

static __inline__ int _inline_intCpuLock (void)
    {
    int key;
    __asm__ volatile
	(
	"mrs	r1, cpsr;		"	/* get current status */
#ifdef _WRS_CONFIG_UNIFIED_FIQ_IRQ
	"and	%0, r1, #0xC0;		"	/* return current I & F */
	"orr	r1, r1, #0xC0;		"	/* set I & F (1<<7 | 1<<6) */
#else
	"and	%0, r1, #0x80;		"	/* return current I_BIT */
	"orr	r1, r1, #0x80;		"	/* set I_BIT (1<<7) */
#endif /* _WRS_CONFIG_UNIFIED_FIQ_IRQ */
	"msr	cpsr, r1		"	/* set new status */
	: "=&r" (key)				/* output: key is %0 */
	:					/* no input */
	: "r1", "memory"			/* r1 clobbered; memory */
						/* clobber for code barrier */
	);
    return key;
    }

static __inline__ void _inline_intCpuUnlock (int key)
    {
    __asm__ volatile
	(
	"mrs	r1, cpsr;		"	/* get current status */
#ifdef _WRS_CONFIG_UNIFIED_FIQ_IRQ
	"bic	r1, r1, #0xC0;		"	/* clear I & F (1<<7 | 1<<6) */
	"and	r0, %0, #0xC0;		"	/* get passed I & F values */
#else
	"bic	r1, r1, #0x80;		"	/* clear I_BIT (1<<7) */
	"and	r0, %0, #0x80;		"	/* get passed I_BIT value */
#endif /* _WRS_CONFIG_UNIFIED_FIQ_IRQ */
	"orr	r1, r1, r0;		"	/* or in passed I_BIT value */
	"msr	cpsr, r1"			/* set new status */
	: 					/* no output */
	: "r" (key)				/* input: key is %0 */
	: "r0", "r1", "memory"			/* r0, r1 clobbered; memory */
						/* clobber for code barrier */
	);	
    }

static __inline__ int _inline_intCpuMicroLock (void)
    {
    int key;
    __asm__ volatile
	(
	"mrs	%0, cpsr;		"	/* get, return current status */
#ifdef _WRS_CONFIG_UNIFIED_FIQ_IRQ
	"orr	r1, %0, #0xC0;		"	/* set I & F (1<<7 | 1<<6) */
#else
	"orr	r1, %0, #0x80;		"	/* set I_BIT (1<<7) */
#endif /* _WRS_CONFIG_UNIFIED_FIQ_IRQ */
	"msr	cpsr_c, r1		"	/* set new status */
	: "=&r" (key)				/* output: key is %0 */
	:					/* no input */
	: "r1", "memory"			/* r1 clobbered; memory */
						/* clobber for code barrier */
	);
    return key;
    }

static __inline__ void _inline_intCpuMicroUnlock (int key)
    {
    __asm__ volatile
	(
	"msr	cpsr_c, %0"			/* restore previous status */
	: 					/* no output */
	: "r" (key)				/* input: key is %0 */
						/* no regs, memory clobbered */
	);	
    }

#else

#define _inline_intCpuLock()		intCpuLock()
#define _inline_intCpuUnlock(key)	intCpuUnlock(key)

#define _inline_intCpuMicroLock()	intCpuMicroLock()
#define _inline_intCpuMicroUnlock(key)	intCpuMicroUnlock(key)

#endif	/* __DCC__ */
#endif	/* !ARM_THUMB */


/* functions in intArchLib.c and intALib.s */

extern STATUS intDisable (int);
extern STATUS intEnable (int);
extern STATUS intLibInit (int, int, int);
extern int    intIFLock (void);
extern void   intIFUnlock (int);

#ifdef _WRS_CONFIG_UNIFIED_FIQ_IRQ
extern int    intILock (void);
extern void   intIUnlock (int);
extern int    intFLock (void);
extern void   intFUnlock (int);
#endif /* _WRS_CONFIG_UNIFIED_FIQ_IRQ */


/* 
 * Function pointers used in intArchLib.c and interrupt driver
 */
IMPORT FUNCPTR sysIntLvlVecChkRtn;
IMPORT FUNCPTR sysIntLvlVecAckRtn;
IMPORT FUNCPTR sysIntLvlChgRtn;
IMPORT FUNCPTR sysIntLvlEnableRtn;
IMPORT FUNCPTR sysIntLvlDisableRtn;

/* 
 * vxBus function pointers used in intArchLib.c and interrupt driver
 */
IMPORT FUNCPTR _func_IntLvlVecChkRtn;
IMPORT FUNCPTR _func_IntLvlVecAckRtn;
IMPORT FUNCPTR _func_IntLvlChgRtn;
IMPORT FUNCPTR _func_IntLvlEnableRtn;
IMPORT void * intCtlrDevID;
IMPORT FUNCPTR _func_IntLvlDisableRtn;

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCintArmLibh */
