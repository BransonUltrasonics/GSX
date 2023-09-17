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
21oct16,h_k  added vxAtomicXxx inline functions for ARMv8.
18aug16,mpc  added ARM64 support (F5261)
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

#ifdef	_WRS_INLINE_vxAtomicAdd
static __inline__ atomic64Val_t vxAtomic64Add_inline
    (
    atomic64_t *	target,
    atomic64Val_t	value
    )
    {
    atomic64Val_t	oldValue;
    atomic64Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%x0, [%x3];"
	"add	%x1, %x0, %x4;"
	"stxr	%w2, %x1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %x0 */
	  "=&r" (newValue),	/*         newValue is %x1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target),	/* input:  target   is %x3 */
	  "r"   (value)		/*         value    is %x4 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }

static __inline__ atomic32Val_t vxAtomic32Add_inline
    (
    atomic32_t *	target,
    atomic32Val_t	value
    )
    {
    atomic32Val_t	oldValue;
    atomic32Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%w0, [%x3];"
	"add	%w1, %w0, %w4;"
	"stxr	%w2, %w1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %w0 */
	  "=&r" (newValue),	/*         newValue is %w1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target),	/* input:  target   is %x3 */
	  "r"   (value)		/*         value    is %w4 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }
#endif	/* _WRS_INLINE_vxAtomicAdd */

#ifdef	_WRS_INLINE_vxAtomicSub
static __inline__ atomic64Val_t vxAtomic64Sub_inline
    (
    atomic64_t *	target,
    atomic64Val_t	value
    )
    {
    atomic64Val_t	oldValue;
    atomic64Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%x0, [%x3];"
	"sub	%x1, %x0, %x4;"
	"stxr	%w2, %x1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %x0 */
	  "=&r" (newValue),	/*         newValue is %x1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target),	/* input:  target   is %x3 */
	  "r"   (value)		/*         value    is %x4 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }

static __inline__ atomic32Val_t vxAtomic32Sub_inline
    (
    atomic32_t *	target,
    atomic32Val_t	value
    )
    {
    atomic32Val_t	oldValue;
    atomic32Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%w0, [%x3];"
	"sub	%w1, %w0, %w4;"
	"stxr	%w2, %w1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %w0 */
	  "=&r" (newValue),	/*         newValue is %w1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target),	/* input:  target   is %x3 */
	  "r"   (value)		/*         value    is %w4 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }
#endif	/* _WRS_INLINE_vxAtomicSub */

#ifdef	_WRS_INLINE_vxAtomicInc
static __inline__ atomic64Val_t vxAtomic64Inc_inline
    (
    atomic64_t *	target
    )
    {
    atomic64Val_t	oldValue;
    atomic64Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%x0, [%x3];"
	"add	%x1, %x0, #1;"
	"stxr	%w2, %x1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %x0 */
	  "=&r" (newValue),	/*         newValue is %x1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target)	/* input:  target   is %x3 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }

static __inline__ atomic32Val_t vxAtomic32Inc_inline
    (
    atomic32_t *	target
    )
    {
    atomic32Val_t	oldValue;
    atomic32Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%w0, [%x3];"
	"add	%w1, %w0, #1;"
	"stxr	%w2, %w1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %w0 */
	  "=&r" (newValue),	/*         newValue is %w1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target)	/* input:  target   is %x3 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }
#endif	/* _WRS_INLINE_vxAtomicInc */

#ifdef	_WRS_INLINE_vxAtomicDec
static __inline__ atomic64Val_t vxAtomic64Dec_inline
    (
    atomic64_t *	target
    )
    {
    atomic64Val_t	oldValue;
    atomic64Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%x0, [%x3];"
	"sub	%x1, %x0, #1;"
	"stxr	%w2, %x1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %x0 */
	  "=&r" (newValue),	/*         newValue is %x1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target)	/* input:  target   is %x3 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }

static __inline__ atomic32Val_t vxAtomic32Dec_inline
    (
    atomic32_t *	target
    )
    {
    atomic32Val_t	oldValue;
    atomic32Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%w0, [%x3];"
	"sub	%w1, %w0, #1;"
	"stxr	%w2, %w1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %w0 */
	  "=&r" (newValue),	/*         newValue is %w1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target)	/* input:  target   is %x3 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }
#endif	/* _WRS_INLINE_vxAtomicDec */

#ifdef	_WRS_INLINE_vxAtomicSet
static __inline__ atomic64Val_t vxAtomic64Set_inline
    (
    atomic64_t *	target,
    atomic64Val_t	value
    )
    {
    atomic64Val_t	oldValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%x0, [%x2];"
	"stxr	%w1, %x3, [%x2];"
	: "=&r" (oldValue),	/* output: oldValue is %x0 */
	  "=&r" (isFailed) 	/*         isFailed is %w1 */
	: "r"   (target),	/* input:  target   is %x2 */
	  "r"   (value)		/*         value    is %x3 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }

static __inline__ atomic32Val_t vxAtomic32Set_inline
    (
    atomic32_t *	target,
    atomic32Val_t	value
    )
    {
    atomic32Val_t	oldValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%w0, [%x2];"
	"stxr	%w1, %w3, [%x2];"
	: "=&r" (oldValue),	/* output: oldValue is %w0 */
	  "=&r" (isFailed) 	/*         isFailed is %w1 */
	: "r"   (target),	/* input:  target   is %x2 */
	  "r"   (value)		/*         value    is %w3 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }
#endif	/* _WRS_INLINE_vxAtomicSet */

#ifdef	_WRS_INLINE_vxAtomicClear
static __inline__ atomic64Val_t vxAtomic64Clear_inline
    (
    atomic64_t *	target
    )
    {
    atomic64Val_t	oldValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%x0, [%x2];"
	"stxr	%w1, xzr, [%x2];"
	: "=&r" (oldValue),	/* output: oldValue is %x0 */
	  "=&r" (isFailed) 	/*         isFailed is %w1 */
	: "r"   (target)	/* input:  target   is %x2 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }

static __inline__ atomic32Val_t vxAtomic32Clear_inline
    (
    atomic32_t *	target
    )
    {
    atomic32Val_t	oldValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%w0, [%x2];"
	"stxr	%w1, wzr, [%x2];"
	: "=&r" (oldValue),	/* output: oldValue is %w0 */
	  "=&r" (isFailed) 	/*         isFailed is %w1 */
	: "r"   (target)	/* input:  target   is %x2 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }
#endif	/* _WRS_INLINE_vxAtomicClear */

#ifdef	_WRS_INLINE_vxAtomicAnd
static __inline__ atomic64Val_t vxAtomic64And_inline
    (
    atomic64_t *	target,
    atomic64Val_t	value
    )
    {
    atomic64Val_t	oldValue;
    atomic64Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%x0, [%x3];"
	"and	%x1, %x0, %x4;"
	"stxr	%w2, %x1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %x0 */
	  "=&r" (newValue),	/*         newValue is %x1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target),	/* input:  target   is %x3 */
	  "r"   (value)		/*         value    is %x4 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }

static __inline__ atomic32Val_t vxAtomic32And_inline
    (
    atomic32_t *	target,
    atomic32Val_t	value
    )
    {
    atomic32Val_t	oldValue;
    atomic32Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%w0, [%x3];"
	"and	%w1, %w0, %w4;"
	"stxr	%w2, %w1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %w0 */
	  "=&r" (newValue),	/*         newValue is %w1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target),	/* input:  target   is %x3 */
	  "r"   (value)		/*         value    is %w4 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }
#endif	/* _WRS_INLINE_vxAtomicAnd */

#ifdef	_WRS_INLINE_vxAtomicOr
static __inline__ atomic64Val_t vxAtomic64Or_inline
    (
    atomic64_t *	target,
    atomic64Val_t	value
    )
    {
    atomic64Val_t	oldValue;
    atomic64Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%x0, [%x3];"
	"orr	%x1, %x0, %x4;"
	"stxr	%w2, %x1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %x0 */
	  "=&r" (newValue),	/*         newValue is %x1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target),	/* input:  target   is %x3 */
	  "r"   (value)		/*         value    is %x4 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }

static __inline__ atomic32Val_t vxAtomic32Or_inline
    (
    atomic32_t *	target,
    atomic32Val_t	value
    )
    {
    atomic32Val_t	oldValue;
    atomic32Val_t	newValue;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%w0, [%x3];"
	"orr	%w1, %w0, %w4;"
	"stxr	%w2, %w1, [%x3];"
	: "=&r" (oldValue),	/* output: oldValue is %w0 */
	  "=&r" (newValue),	/*         newValue is %w1 */
	  "=&r" (isFailed) 	/*         isFailed is %w2 */
	: "r"   (target),	/* input:  target   is %x3 */
	  "r"   (value)		/*         value    is %w4 */
	: "memory"
	);

    if (isFailed != FALSE)
	goto unlucky;

    return oldValue;
    }
#endif	/* _WRS_INLINE_vxAtomicOr */

#ifdef	_WRS_INLINE_vxCas
static __inline__ BOOL vxAtomic64Cas_inline
    (
    atomic64_t *	target,		/* memory location to comp-and-swap */
    atomic64Val_t	oldValue,	/* compare to this value */
    atomic64Val_t	newValue	/* swap with this value */
    )
    {
    atomic64Val_t	value;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%x0, [%x1];"
	: "=&r" (value)		/* output: value    is %x0 */
	: "r"   (target)	/* input:  target   is %x1 */
	: "memory"  
	);

    if (value != oldValue)
	{
	__asm__ volatile (
	    "clrex"
	    : /* no output */
	    : /* no input */
	    : "memory");
	return FALSE;
	}

    __asm__ volatile (
	"stxr	%w0, %x2, [%x1];"
	: "=&r" (isFailed)	/* output: isFailed is %w0 */
	: "r"   (target),	/* input:  target   is %x1 */
	  "r"   (newValue)	/*         newValue is %x2 */
	: "memory"  
	);

    if (isFailed != FALSE)
	goto unlucky;

    return TRUE;
    }

static __inline__ BOOL vxAtomic32Cas_inline
    (
    atomic32_t *	target,		/* memory location to comp-and-swap */
    atomic32Val_t	oldValue,	/* compare to this value */
    atomic32Val_t	newValue	/* swap with this value */
    )
    {
    atomic32Val_t	value;
    BOOL		isFailed;

unlucky:
    __asm__ volatile (
	"ldxr	%w0, [%x1];"
	: "=&r" (value)		/* output: value    is %w0 */
	: "r"   (target)	/* input:  target   is %x1 */
	: "memory"  
	);

    if (value != oldValue)
	{
	__asm__ volatile (
	    "clrex"
	    : /* no output */
	    : /* no input */
	    : "memory");
	return FALSE;
	}

    __asm__ volatile (
	"stxr	%w0, %w2, [%x1];"
	: "=&r" (isFailed)	/* output: isFailed is %w0 */
	: "r"   (target),	/* input:  target   is %x1 */
	  "r"   (newValue)	/*         newValue is %w2 */
	: "memory"  
	);

    if (isFailed != FALSE)
	goto unlucky;

    return TRUE;
    }
#endif	/* _WRS_INLINE_vxCas */
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

#    else  /* !__DCC_ && !__GNUC__ && !__llvm__ */
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

#ifdef	_WRS_INLINE_vxAtomicAdd
#define vxAtomicAdd_inline	vxAtomic64Add_inline
#endif	/* _WRS_INLINE_vxAtomicAdd */

#ifdef	_WRS_INLINE_vxAtomicSub
#define vxAtomicSub_inline	vxAtomic64Sub_inline
#endif	/* _WRS_INLINE_vxAtomicSub */

#ifdef	_WRS_INLINE_vxAtomicInc
#define vxAtomicInc_inline	vxAtomic64Inc_inline
#endif	/* _WRS_INLINE_vxAtomicInc */

#ifdef	_WRS_INLINE_vxAtomicDec
#define vxAtomicDec_inline	vxAtomic64Dec_inline
#endif	/* _WRS_INLINE_vxAtomicDec */

#ifdef	_WRS_INLINE_vxAtomicGet
#define vxAtomic64Get_inline(target)                                    \
        ((atomic64Val_t) (*(volatile atomic64_t *)(target)))

#define vxAtomic32Get_inline(target)                                    \
        ((atomic32Val_t) (*(volatile atomic32_t *)(target)))

#define vxAtomicGet_inline	vxAtomic64Get_inline
#endif	/* _WRS_INLINE_vxAtomicGet */

#ifdef	_WRS_INLINE_vxAtomicSet
#define vxAtomicSet_inline	vxAtomic64Set_inline
#endif	/* _WRS_INLINE_vxAtomicSet */

#ifdef	_WRS_INLINE_vxAtomicClear
#define vxAtomicClear_inline	vxAtomic64Clear_inline
#endif	/* _WRS_INLINE_vxAtomicClear */

#ifdef	_WRS_INLINE_vxAtomicAnd
#define vxAtomicAnd_inline	vxAtomic64And_inline
#endif	/* _WRS_INLINE_vxAtomicAnd */

#ifdef	_WRS_INLINE_vxAtomicOr
#define vxAtomicOr_inline	vxAtomic64Or_inline
#endif	/* _WRS_INLINE_vxAtomicOr */

#ifdef	_WRS_INLINE_vxCas
#define vxAtomicCas_inline	vxAtomic64Cas_inline
#else
#define vxAtomicCas_inline      vxAtomicCas
#define vxAtomic32Cas_inline    vxAtomic32Cas
#endif	/* _WRS_INLINE_vxCas */

#define vxCas_inline            vxAtomicCas_inline
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
