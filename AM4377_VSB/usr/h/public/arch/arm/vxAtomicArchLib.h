/* vxAtomicArchLibCommon.h - ARM specific atomic library header */

/*
 * Copyright (c) 2009-2011, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
06apr16,h_k  added ARMv8 support.(F5261)
04mar11,j_b  replace hardcoded DMB, DSB opcodes after compiler fix
12jan11,jdw  Fix inline assembly opcodes for T2 (WIND00249791)
21oct10,j_b  add memory barrier support for ARMv7
07jun09,kk   add VX_INSTR_BARRIER() back
05jun09,kk   move inlines from vxAtomicArchLib.h to here
17may09,gls  written.
*/

#ifndef __INCvxAtomicArchLibCommonh
#define __INCvxAtomicArchLibCommonh

#ifdef __cplusplus
extern "C" {
#endif

/* Remove this line when all arch implement memory barrier API */

#define _WRS_ARCH_SUPPORTS_MEM_BARRIER

/* 
 * assume that memory barriers will be kept as is from arch 7, so leave
 * it in the common file. However if there are any change then we have
 * to make it in different child arch.
 */


/* ARMARCH7 provides memory barrier instructions */

#    if defined (__DCC__)

	__asm volatile void __dmb ()
{
	DMB
}

__asm volatile void __dsb ()
{
	DSB
}

#    elif defined (__GNUC__) || defined (__llvm__)

#ifdef _WRS_CONFIG_CPU_ARMARCH8A
static __inline__ void __dmb (char option)
    {
    __asm__ volatile (
	"dmb %[flag]"
        : /* no output */
        : [flag] "I" (option));
    }

static __inline__ void __dsb (char option)
    {
    __asm__ volatile (
	"dsb %[flag]"
        : /* no output */
        : [flag] "I" (option));
    }

static __inline__ void __isb (void)
    {
    __asm__ volatile (
	"isb sy");
    }
#else
static __inline__ void __dmb (void)
    {
    __asm__ volatile (
	"dmb"
        : /* no output */
        : /* no input */
        : "memory");
    }

static __inline__ void __dsb (void)
    {
    __asm__ volatile (
	"dsb"
        : /* no output */
        : /* no input */
        : "memory");
    }

#endif /* _WRS_CONFIG_CPU_ARMARCH8A */

#    else  /* !__DCC_ && !__GNUC__ */
#error "error!!! unsupported compiler!!!"
#    endif  /* __DCC__ */

#ifdef _WRS_CONFIG_CPU_ARMARCH8A
#define VX_MEM_ARM64_LD		0xd		/* Full system, load  */
#define VX_MEM_ARM64_ST		0xe		/* Full system, store */
#define VX_MEM_ARM64_SY		0xf		/* Full system, all   */

#define VX_MEM_BARRIER_R()	_WRS_BARRIER("dmb ld")	/* Full system, load  */
#define VX_MEM_BARRIER_W()	_WRS_BARRIER("dmb st")	/* Full system, store */
#define VX_MEM_BARRIER_RW()	_WRS_BARRIER("dmb sy")	/* Full system, all   */
#define VX_INSTR_BARRIER()	_WRS_BARRIER("isb sy")	/* Full system, all   */
#define VX_SYNC_BARRIER()	_WRS_BARRIER("dsb sy")	/* Full system, all   */
#else
#define VX_MEM_BARRIER_R()	__dmb()
#define VX_MEM_BARRIER_W()	__dmb()
#define VX_MEM_BARRIER_RW()	__dmb()
#define VX_INSTR_BARRIER()	__dmb()
#define VX_SYNC_BARRIER()	__dsb()
#endif /* _WRS_CONFIG_CPU_ARMARCH8A */

#ifdef __cplusplus
}
#endif

#endif /* __INCvxAtomicArchLibCommonh */
