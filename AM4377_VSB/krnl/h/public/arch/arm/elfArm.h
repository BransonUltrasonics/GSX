/* elfArm.h - ARM-specific elf loader header */

/*
 * Copyright (c) 2001, 2004-2005, 2009-2010, 2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
23aug16,mpc  added ARM64 support (F5261)
27feb14,yjw  Added more relocation type (VXW7-2255)
11oct10,v_r  Added support for the R_ARM_CALL and R_ARM_THM_JUMP24
             relocation types (CQ:WIND00234720).
             Removed unused types (by both the kernel loader and the
             userspace shared lib loader).
14dec09,pgh  Add EM_ARCH_MACHINE_x macro definition.
25jul05,v_r  Updated copyright.
18feb05,dbt  Cleanup.
18sep04,jn   Move towards shared code for host and target loaders
17feb04,jn   Merged version from AE.
30nov01,pad  Moved definitions of CHECK_FITS and SIGN_EXTEND into
             loadElfLibP.h
24aug01,jn   created, based on vxWorksAE 1.1 
             share/src/loader/elfArm.h@@/main/tor3_x/5
*/ 

#ifndef __INCelfArmh
#define __INCelfArmh

#ifdef _WRS_CONFIG_CPU_ARMARCH8A
#include <arch/arm/elfArmv8.h>
#else /* !_WRS_CONFIG_CPU_ARMARCH8A */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * macro to check that a signed <val> fits in <nBits> without overflow
 * i.e. that the "sign-bit" is correctly propagated into the excess high bits
 */
   
#define CHECK_FITS(val,nBits) \
        ((((val) & ~((1 << ((nBits)-1))-1)) == 0) || \
         (((val) & ~((1 << ((nBits)-1))-1)) == ~(UINT32)((1 << ((nBits)-1))-1)))

/* macros to insert the low n bits of a value into a buffer. */

#define LOW31_INSERT(buf,value)	INSERT ((buf), (value), 0x7fffffff)

#define LOW24_INSERT(buf,value)	INSERT ((buf), (value), 0x00ffffff)

#define LOW16_INSERT(buf,value)	INSERT ((buf), (value), 0x0000ffff)

#define LOW12_INSERT(buf,value)	INSERT ((buf), (value), 0x00000fff)

#define LOW11_INSERT(buf,value)	INSERT ((buf), (value), 0x000007ff)

#define LOW8_INSERT(buf,value)	INSERT ((buf), (value), 0x000000ff)

#define BITS6_10_INSERT(buf,value)	INSERT ((buf), (value) << 6, 0x000007c0)

#define UBIT_INSERT(buf,value)	INSERT ((buf), (value) << 23 , 0x0080000)

/*
 * The following macro defines the ELF machine type value that can be loaded
 * into an ARM CPU.
 */

#define EM_ARCH_MACHINE_1   EM_ARM

#define EM_ARCH_MACHINE		EM_ARM
#define EM_ARCH_MACH_ALT	EM_ARM    	/* not used */

/* Relocation Type Definitions */

#define R_ARM_NONE           0  /* No reloc */
#define R_ARM_PC24           1
#define R_ARM_ABS32          2
#define R_ARM_REL32          3
#define R_ARM_PC13           4
#define R_ARM_ABS16          5
#define R_ARM_ABS12          6
#define R_ARM_THM_ABS5       7
#define R_ARM_ABS8           8
#define R_ARM_THM_PC22      10
#define R_ARM_THM_PC8       11
#define R_ARM_COPY          20  /* copy symbol at runtime */
#define R_ARM_GLOB_DAT      21  /* create GOT entry */
#define R_ARM_JUMP_SLOT     22  /* create PLT entry */
#define R_ARM_RELATIVE      23  /* adjust by program base */
#define R_ARM_PLT32         27
#define R_ARM_CALL	    28
#define R_ARM_JUMP24        29
#define R_ARM_THM_JUMP24    30
#define R_ARM_TARGET2       41
#define R_ARM_PREL31        42

#define R_ARM_THM_PC11      102		/* cygnus extension to abi: */
					/* thumb unconditional branch */
#define R_ARM_THM_PC9       103		/* cygnus extension to abi: */
					/* thumb conditional branch */

#ifdef __cplusplus
}
#endif
#endif /* _WRS_CONFIG_CPU_ARMARCH8A */
#endif /* __INCelfArmh */
