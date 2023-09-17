/* vxInlineArch.h - ARM inline macro configuration */

/*
 * Copyright (c) 2007, 2009-2010, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
09nov16,h_k  added inline ffsXxx, vxAtomicXxx and vxCas on AMRv8.
             added inline KERNEL_GLOBAL_ACCESS and taskIdSelf on AMRv8 in SMP.
21may10,m_h  Thumb-2 Support
18may09,j_b  add _inline_intCpuMicroLock/Unlock support
19jun07,zl   created
*/

#ifndef __INCvxInlineArchh
#define __INCvxInlineArchh

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This file is included from <vxInline.h>. It may define _WRS_INLINE_xxx
 * build configuration macros in order to enable architecture specific
 * inlined implementation of a macro definition.
 */

#if	!(ARM_THUMB) && !(ARM_THUMB2)

#define _WRS_INLINE_intCpuLock
#define _WRS_INLINE_intCpuUnlock
#define _WRS_INLINE_intCpuMicroLock
#define _WRS_INLINE_intCpuMicroUnlock

#endif /* !ARM_THUMB && !ARM_THUMB2 */

#ifdef	_WRS_CONFIG_ARM_ARMV8A
#if	defined(_WRS_CONFIG_SMP) && defined(_WRS_ARMV8_WINDVARS_IN_X18)
#define _WRS_INLINE_KERNEL_GLOBAL_ACCESS
#define _WRS_INLINE_taskIdSelf
#endif	/* _WRS_CONFIG_SMP && _WRS_ARMV8_WINDVARS_IN_X18 */

#define _WRS_INLINE_ffsLsb
#define _WRS_INLINE_ffsMsb
#define _WRS_INLINE_ffs64Lsb
#define _WRS_INLINE_ffs64Msb

#define _WRS_INLINE_vxAtomicAdd
#define _WRS_INLINE_vxAtomicSub
#define _WRS_INLINE_vxAtomicInc
#define _WRS_INLINE_vxAtomicDec
#define _WRS_INLINE_vxAtomicGet
#define _WRS_INLINE_vxAtomicSet
#define _WRS_INLINE_vxAtomicClear
#define _WRS_INLINE_vxAtomicAnd
#define _WRS_INLINE_vxAtomicOr
#define _WRS_INLINE_vxCas
#endif	/* _WRS_CONFIG_ARM_ARMV8A */

#ifdef __cplusplus
}
#endif

#endif /* __INCvxInlineArchh */
