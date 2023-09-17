/* regsArm.h - ARM registers */

/*
 * Copyright (c) 2004, 2012, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
04aug16,h_k  added tlsbase to REG_SET.
06apr16,h_k  added ARMv8 support.(F5261)
21mar12,jpb  Updated for POSIX namespace isolation conformance (part of 
             defect Defect WIND000307800).
06jul04,md   derived from 01k kernel version
*/
#ifndef	__INCregsArmh
#define	__INCregsArmh

#ifndef _ASMLANGUAGE
#include <base/e_vxTypesOld.h>
#endif  /* !_ASMLANGUAGE */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef	_WRS_CONFIG_ARM_ARMV8A
#define _Vx_GREG_NUM	31	/* has 32 32-bit general registers */
#else
#define _Vx_GREG_NUM	15	/* has 15 32-bit general registers */
#endif	/* _WRS_CONFIG_ARM_ARMV8A */

#if !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE) && \
    !defined(_POSIX_AEP_RT_CONTROLLER_C_SOURCE)
#define GREG_NUM	_Vx_GREG_NUM
#endif  /* !_POSIX_xxx_SOURCE */

#ifndef	_ASMLANGUAGE

/* REG_SET - ARM Register set
 * Note that the exception stack frame relies on the order of items
 * in this so don't change this without changing the ESF and the stub
 * in excALib which creates it.
 */

typedef struct _Vx_register_set	/* ARM register set */
    {
    _Vx_ULONG r[_Vx_GREG_NUM];	/* general purpose registers 0-14 */
#ifdef	_WRS_CONFIG_ARM_ARMV8A
    _Vx_ULONG sp;		/* stack pointer */
#endif	/* _WRS_CONFIG_ARM_ARMV8A */
    _Vx_INSTR *pc;		/* program counter */
    _Vx_ULONG cpsr;		/* current PSR */
    _Vx_ULONG ttbase;		/* Trans Table Base */
    _Vx_ULONG tlsbase;		/* TLS Base */
    } _Vx_REGISTER_SET;

#if !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE) && \
    !defined(_POSIX_AEP_RT_CONTROLLER_C_SOURCE)
#ifdef	_WRS_CONFIG_ARM_ARMV8A
#define fpReg		r[29]	/* frame pointer */
#define spReg		sp	/* stack pointer */
#else
#define fpReg		r[11]	/* frame pointer */
#define spReg		r[13]	/* stack pointer */
#endif	/* _WRS_CONFIG_ARM_ARMV8A */
#define reg_pc		pc	/* program counter */
#define reg_sp		spReg	/* stack pointer */
#define reg_fp		fpReg	/* frame pointer */
#endif  /* !_POSIX_xxx_SOURCE */

#endif	/* !_ASMLANGUAGE */

#if !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE) && \
    !defined(_POSIX_AEP_RT_CONTROLLER_C_SOURCE)

#ifdef	_WRS_CONFIG_LP64
#define	ARM_REG_SIZE	8
#else
#define	ARM_REG_SIZE	4
#endif	/* _WRS_CONFIG_LP64 */
#define REG_SET_G_REG_BASE	0x00	/* data reg's base offset to REG_SET */
#define REG_SET_G_REG_OFFSET(n)	(REG_SET_G_REG_BASE + (n)*ARM_REG_SIZE)
#ifdef	_WRS_CONFIG_ARM_ARMV8A
#define REG_SET_SP_OFFSET	(REG_SET_G_REG_OFFSET(GREG_NUM))
#define REG_SET_PC_OFFSET	(REG_SET_SP_OFFSET + ARM_REG_SIZE)
#else
#define REG_SET_PC_OFFSET	(REG_SET_G_REG_OFFSET(GREG_NUM))
#endif	/* _WRS_CONFIG_ARM_ARMV8A */
#define REG_SET_CPSR_OFFSET	(REG_SET_PC_OFFSET + ARM_REG_SIZE)

#define REG_SET_TTBASE_OFFSET	(REG_SET_CPSR_OFFSET + ARM_REG_SIZE)
#define REG_SET_TLSBASE_OFFSET  (REG_SET_TTBASE_OFFSET + ARM_REG_SIZE)

/* bits in the PSR */

#define	V_BIT	(1<<28)
#define	C_BIT	(1<<29)
#define	Z_BIT	(1<<30)
#define	N_BIT	(1<<31)
#define I_BIT   (1<<7)
#define F_BIT   (1<<6)
#define	T_BIT	(1<<5)
#endif  /* !_POSIX_xxx_SOURCE */

#ifdef __cplusplus
}
#endif

#endif	/* __INCregsArmh */
