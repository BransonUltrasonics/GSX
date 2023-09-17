/* cacheArmLib.h - ARM cache library header file */

/*
 * Copyright (c) 1996-2008, 2010-2012, 2014-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
20oct15,yjw  added lockRtn & unlockRtn in extCache (US67854)
14sep14,yjw  added dmaMallocRtn & dmaFreeRtn in extCache (US40546)
17jul12,j_b  Add Cortex-A15 cache support
07feb12,c_l  Fix compiling error. (WIND00294062) 
13jan11,m_h  moving cacheArchAlignSize to arm.h
29apr10,pad  Moved extern C statement after include statements.
02jul07,m_h  Adding support for MPCore
12jun08,j_b  add Cortex-A/-R cache support
22may06,m_h  946 Support
22feb06,m_h  Manzano Support
28mar05,jb   Adding 1136jfs Support
25jan05,jb   Removing in-advertant addition of 1136
07dec04,jb   Restructure for Base6
01dec03,scm  modify for AIM support...
17dec02,jb   Adding ARM10 and 926E support
24jan03,jb   Resolve SPR 81285 and 82533
02oct01,jpd  added definitions to support simplifications in source.
12nov01,to   added cacheArchIntMask (merged from AE)
23jul01,scm  change XScale name to conform to coding standards...
11dec00,scm  replace references to ARMSA2 with XScale
01sep00,scm  add sa2 support...
07sep99,jpd  added ARM920T support.
24nov98,jpd  added ARM940T, ARMSA1100, ARMSA1500 support;
	    cdp  restructured for multiple cache/MMU libraries.
09mar98,jpd  tidied.
16sep97,jpd  added cacheIMBRange(), 810 definitions for cacheDClear() and
		 cacheArchIntLock().
09may96,cdp  created
*/

/*
 * N.B. although this library contains code written for the ARM810
 * CPU, at the time of writing, this code has not been fully tested on
 * that CPU. YOU HAVE BEEN WARNED.
 */

#ifndef	__INCcacheArmLibh
#define	__INCcacheArmLibh

#ifndef	_ASMLANGUAGE

/*
 * Variable used to hold the interrupt mask used when disabling
 * interrupts for lengthy cache operations such as cache flushing.  This
 * is preinitialised to a mask that disables IRQs and FIQs.  The BSP may
 * change this.  Do not do so unless you are sure that you understand the
 * consequences.  In particular, if interrupts are allowed to occur
 * during these operations, the state of the cache afterwards may be
 * indeterminate.
 */

IMPORT UINT32	cacheArchIntMask;

#ifdef _WRS_CONFIG_EXTERNAL_CACHE
struct extCache
    {
    FUNCPTR     enableRtn;
    FUNCPTR     disableRtn;
    FUNCPTR     flushRtn;
    FUNCPTR     clearRtn;
    FUNCPTR     invalidateRtn;
    FUNCPTR     pipeFlushRtn;
    FUNCPTR     dmaMallocRtn;
    FUNCPTR     dmaFreeRtn;
    FUNCPTR     lockRtn;
    FUNCPTR     unlockRtn;
    };
#endif /* _WRS_CONFIG_EXTERNAL_CACHE */

#endif	/* _ASMLANGUAGE */

/*
 * Arm Arch HW specific bits used by cache routines...
 */

#define CACHE_MM_ENABLE   (1<<0)    /* MMU enable */
#define CACHE_DC_ENABLE   (1<<2)    /* (data) cache enable */
#define CACHE_WB_ENABLE   (1<<3)    /* write buffer enable */
#define CACHE_IC_ENABLE   (1<<12)   /* Instruction cache enable */

/*
 * Reset Architecture specific cache behavior.
 * It is set with the cpu include file.
 */

#undef ARMCACHE_NOT_COHERENT
#undef ARMCACHE_HAS_WRITETHROUGH
#undef ARMCACHE_HAS_MINICACHE
#undef ARMCACHE_NEEDS_IMB


#ifndef _WRS_CONFIG_UNSUPPORTS_CACHE
#include <arch/arm/cacheArmArch.h>
#endif /* _WRS_CONFIG_SUPPORT_CACHE */

#ifdef __cplusplus
extern "C" {
#endif

#if (ARMCACHE != ARMCACHE_NONE)

/*
 * Verify the cpu has established some special attributes
 */

#ifndef ARMCACHE_NOT_COHERENT
#error ARMCACHE value not supported in definition of ARMCACHE_NOT_COHERENT
#endif

#ifndef ARMCACHE_HAS_WRITETHROUGH
#error ARMCACHE value not supported in definition of ARMCACHE_HAS_WRITETHROUGH
#endif

#ifndef ARMCACHE_HAS_MINICACHE
#define ARMCACHE_HAS_MINICACHE FALSE
#endif

#ifndef ARMCACHE_NEEDS_IMB
#error ARMCACHE value not supported in definition of ARMCACHE_NEEDS_IMB
#endif

#endif /* ARMCACHE_NONE */

#ifdef __cplusplus
}
#endif

#endif	/* __INCcacheArmLibh */
