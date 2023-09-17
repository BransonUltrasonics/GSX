/* vxPortableArchP.h - ARM portable library configuration */

/*
 * Copyright (c) 2007, 2009-2010, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
06jan14,yjw  Remove unsupported arch
29oct10,j_b  undefine _WRS_PORTABLE_vxCpuIdGet
08jun09,j_b  undef _WRS_PORTABLE_schedLib for uniprocessor
11jul07,to   undef _WRS_PORTABLE_vxAtomicLib if ldrex/strex are supported
06jul07,to   undef _WRS_PORTABLE_vxCpuIndexGet
04may07,zl   created
*/

#ifndef __INCvxPortableArchPh
#define __INCvxPortableArchPh

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This file is included from <private/vxPortableP.h>. It may undefine
 * portable library definitions in order to enable architecture specific
 * implementation of a library or routine.
 */

/* the following can be overridden with PORTABLE */

#ifndef PORTABLE

#undef _WRS_PORTABLE_bLib
#undef _WRS_PORTABLE_dllLib
#undef _WRS_PORTABLE_ffsLib

#ifdef _WRS_CPU_HAS_CLZ_INSTR
#  undef _WRS_PORTABLE_ffsTbl
#endif

#ifndef _WRS_CONFIG_SMP
#  undef _WRS_PORTABLE_schedLib
#endif

#endif /* !PORTABLE */

/* the following cannot be overridden with PORTABLE */

#undef _WRS_PORTABLE_vxAtomicLib
#undef _WRS_PORTABLE_vxCpuIdGet
#undef _WRS_PORTABLE_vxCpuIndexGet

#ifdef __cplusplus
}
#endif

#endif /* __INCvxPortableArchPh */
