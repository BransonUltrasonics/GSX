/* iv.h - interrupt vectors */

/* Copyright 1984-2003, 2009-2010 Wind River Systems, Inc. */

/*
modification history
--------------------
02h,29apr10,pad  Moved extern C statement after include statements.
02g,19jun09,mze  replacing PAD64
02f,20jan09,jb   Adding 64-bit support
02e,17apr03,dbt  Added SIMLINUX support.
02d,22oct01,dee  Merge from T2.1.0 ColdFire
02c,01mar00,frf  Add SH4 support for T2.
02b,23apr97,hk   added SH support.
02b,15aug97,cym  added SIMNT support.
02b,28nov96,cdp  added ARM support.
02a,26may94,yao  added PPC support.
01r,12jul95,ism  added simsolaris support
01q,19mar95,dvs  removed #ifdef TRON - tron no longer supported.
01p,02dec93,pme  added Am29K family support
01o,11aug93,gae  vxsim hppa.
01n,20jun93,gae  vxsim.
01m,09jun93,hdn  added support for I80X86
01l,22sep92,rrr  added support for c++
01k,04jul92,jcf  cleaned up.
01j,26may92,rrr  the tree shuffle
		  -changed includes to have absolute path from h/
01i,09jan92,jwt  converted CPU==SPARC to CPU_FAMILY==SPARC.
01h,04oct91,rrr  passed through the ansification filter
		  -fixed #else and #endif
		  -changed copyright notice
01g,02Aug91,ajm  included MIPS support
01f,19jul91,gae  renamed architecture specific include file to be xx<arch>.h.
01e,29apr91,hdn  added defines and macros for TRON architecture.
01d,25oct90,shl  fixed CPU_FAMILY logic so 68k and sparc won't clash when
		 compiling for sparc.
01c,05oct90,shl  added copyright notice.
                 made #endif ANSI style.
01b,28sep90,del  added include i960/iv.h for I960 CPU_FAMILY.
01a,07aug89,gae  written.
*/

#ifndef __INCivh
#define __INCivh

#if 	CPU_FAMILY==I960
#include "arch/i960/ivI960.h"
#endif	/* CPU_FAMILY==I960 */

#if 	CPU_FAMILY==MC680X0
#include "arch/mc68k/ivMc68k.h"
#endif	/* CPU_FAMILY==MC680X0 */

#if 	CPU_FAMILY==COLDFIRE
#include "arch/coldfire/ivColdfire.h"
#endif	/* CPU_FAMILY==COLDFIRE */

#if     CPU_FAMILY==MIPS
#include "arch/mips/ivMips.h"
#endif	/* CPU_FAMILY==MIPS */

#if     CPU_FAMILY==PPC
#include "arch/ppc/ivPpc.h"
#endif  /* CPU_FAMILY==PPC */

#if	CPU_FAMILY==SIMLINUX
#include "arch/simlinux/ivSimlinux.h"
#endif	/* CPU_FAMILY==SIMLINUX */

#if	CPU_FAMILY==SIMNT
#include "arch/simnt/ivSimnt.h"
#endif	/* CPU_FAMILY==SIMNT */

#if	CPU_FAMILY==SIMSPARCSOLARIS
#include "arch/simsolaris/ivSimsolaris.h"
#endif	/* CPU_FAMILY==SIMSPARCSOLARIS */

#if	CPU_FAMILY==SPARC
#include "arch/sparc/ivSparc.h"
#endif	/* CPU_FAMILY==SPARC */

#if     CPU_FAMILY==I80X86
#ifndef _WRS_CONFIG_LP64
#include "arch/i86/ivI86.h"
#else
#include "arch/i86/x86_64/ivX86_64.h"
#endif  /* LP64 */
#endif	/* CPU_FAMILY==I80X86 */

#if     CPU_FAMILY==AM29XXX
#include "arch/am29k/ivAm29k.h"
#endif	/* CPU_FAMILY==AM29XXX */

#if	CPU_FAMILY==SH
#include "arch/sh/ivSh.h"
#endif	/* CPU_FAMILY==SH */

#if     CPU_FAMILY==ARM
#include "arch/arm/ivArm.h"
#endif  /* CPU_FAMILY==ARM */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __INCivh */
