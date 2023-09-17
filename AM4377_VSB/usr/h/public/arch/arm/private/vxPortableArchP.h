/* vxPortableArchP.h - ARM portable library configuration */

/*
 * Copyright (c) 2007, 2012-2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
11apr16,h_k  aaded ARMARCH8A support.(F5261)
21oct14,cfm  defined _WRS_PORTABLE_bLib to use optimized routines in bALib.s
             (V7PRO-1293)
30dec13,yjw  remove unsupported arch
12sep12,rec  WIND00375980 - build portable atomic libraries for ARMARCH6
28aug12,rec  WIND00371885 - user mode atomic library build is disabled
14may07,jmg  created
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

#ifdef _WRS_CONFIG_CPU_ARMARCH8A
#undef _WRS_PORTABLE_ffsLib

#ifndef _WRS_PORTABLE_ffsLib
#undef _WRS_PORTABLE_ffsTbl
#endif  /* !_WRS_PORTABLE_ffsLib */

#undef _WRS_PORTABLE_ffs64Lib
#endif	/* _WRS_CONFIG_CPU_ARMARCH8A */

#endif /* PORTABLE */

/* the following cannot be overridden with PORTABLE */

#if defined(_WRS_CONFIG_CPU_ARMARCH7) || defined(_WRS_CONFIG_CPU_ARMARCH8A)
#undef _WRS_PORTABLE_vxAtomicLib
#endif

#ifdef __cplusplus
}
#endif

#endif /* __INCvxPortableArchPh */
