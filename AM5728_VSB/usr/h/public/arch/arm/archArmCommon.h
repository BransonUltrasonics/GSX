/* archArmCommon.h - common Arm architecture specific header */

/*
* Copyright (c) 2003-2004, 2006-2007, 2010, 2012, 2016 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
25oct16,mpc  updated the definition for _Vx_INSTR
21mar12,jpb  Updated for POSIX namespace isolation conformance (part of 
             defect Defect WIND000307800).
22jul10,jdw  Update size of INSTR for Thumb2
09jun10,fao  add SWAP32_BE8 and SWAP16_BE8 macro.
04may07,zl   updated portable library build configuration.
02feb06,mil  Updated for POSIX namespace conformance.
08sep04,scm  move INSTR from archArm.h
25aug04,scm  move to portable trcLib...
03aug04,scm  Enable Coprocessor Abstraction, disable Portable trcLib.
25nov03,pes  Move setting of _BYTE_ORDER macro into archArmCommon.h
12nov03,job  Moved some blib stuff from archArm.h
11nov03,pes  written.
*/

#ifndef __INCarchArmCommonh
#define __INCarchArmCommonh

#ifdef __cplusplus
extern "C" {
#endif

#if defined(ARMEB) || defined(__ARMEB__)
#define	_BYTE_ORDER		_BIG_ENDIAN
#else
#define	_BYTE_ORDER		_LITTLE_ENDIAN
#endif

#ifndef _ASMLANGUAGE
#if ((ARM_THUMB) || (ARM_THUMB2))
typedef unsigned short _Vx_INSTR;         /* 16 bit instructions */
#else /* ((ARM_THUMB) || (ARM_THUMB2)) */
typedef unsigned int _Vx_INSTR;          /* 32 bit word-aligned instructions */
#endif /* ((ARM_THUMB) || (ARM_THUMB2)) */
#endif /* !_ASMLANGUAGE */

#if !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE) && \
    !defined(_POSIX_AEP_RT_CONTROLLER_C_SOURCE)

#ifdef ARMBE8
#define SWAP32_BE8(x)   ((((x) & 0x000000ff) << 24) |   \
                         (((x) & 0x0000ff00) <<  8)  |   \
                         (((x) & 0x00ff0000) >>  8)  |   \
                         (((x)) >> 24))
#define SWAP16_BE8(x) ((((x) & 0x00ff0000) >>  8)  |   \
                       (((x)) >> 24))
#else
#define SWAP32_BE8(x) x
#define SWAP16_BE8(x) x
#endif  /* ARMBE8 */

/* from bLib.c */

#define _WRS_BLIB_ALIGNMENT     3       /* quad byte alignment mask */
#define _WRS_IMPROVED_PORTABLE_SWAB

/* Enable Coprocessor Abstraction.  Must do all or none. */
	
#define _WRS_PAL_COPROC_LIB

#ifndef _ASMLANGUAGE
typedef _Vx_INSTR	INSTR;  
#endif  /* !_ASMLANGUAGE */
#endif  /* !_POSIX_xxx_SOURCE */

#ifdef __cplusplus
}
#endif

#endif /* __INCarchArmCommonh */
