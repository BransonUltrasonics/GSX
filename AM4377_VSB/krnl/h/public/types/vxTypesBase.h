/* vxTypesBase.h - default types (when undefined) header file */

/*
 * Copyright (c) 1992-94, 1998-99, 2001, 2005-06, 2009-2010
 * Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */

/*
modification history
--------------------
02i,18may10,pad  Moved extern C statement after include statements.
02h,02oct09,pad  Removed all the macro definitions for the int<8/16/32/64>_t
		 and uint<8/16/32/64>_t types (now defined in stdint.h).
02g,14mar06,mil  Removed ssize_t and time_t in favor of base type files.
02f,05mar06,mil  Removed inclusion of stddef.h for non-POSIX use.
02e,25jan05,vvv  added new types for dual-stack support
02d,05dec01,mem  Added support for 64-bit types.
02c,14dec99,dra  Added simulator/coretools support.
02b,03mar98,bc   fixed _ARCH_*_MIN definitions to avoid compiler warnings
02b,01apr99,bc   Deleted size_t, etc.  They belong in a toolchain header (stddef.h).
02a,12feb98,mrs  fixed wchar_t for c++
01i,08nov94,dvs  cleaned values for _ARCH_FLT_EPSILON and _ARCH_FLT_MIN
		 that got munged in clear case conversion
01h,18oct93,cd   added __STDC__ definitions for unsigned maximum values.
01g,13nov92,dnw  pruned some non-POSIX stuff
01f,22sep92,rrr  added support for c++
01e,07sep92,smb  added __STDC__ conditional and documentation.
01d,30jul92,jwt  made safe for assembly language files.
01c,30jul92,gae  added timer_t; fixed clock_t, fpos_t.
01b,29jul92,smb  added type define for fpos_t.
01a,03jul92,smb  written
*/

/*
DESCRIPTION
Types not defined by the architecture specific header file will be
defined here so that a complete set of definitions are achieved.
For this reason this file, vxTypesBase.h  must be included after vxArch.h
in which the architecture definitions are located.
*/

#ifndef __INCvxTypesBaseh
#define __INCvxTypesBaseh

#ifndef	_ASMLANGUAGE

#if !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE) && \
    !defined(_POSIX_AEP_RT_CONTROLLER_C_SOURCE)
#include <stddef.h>
#endif  /* _POSIX_xxx_SOURCE */

#ifdef __cplusplus
extern "C" {
#endif

/* global typedefs */

#ifndef _TYPE_sig_atomic_t
#define _TYPE_sig_atomic_t      typedef unsigned int sig_atomic_t
#endif

#ifndef _TYPE_timer_t
struct __timer;
#define _TYPE_timer_t           typedef struct __timer *timer_t
#endif

#ifndef _TYPE_clock_t
#define _TYPE_clock_t           typedef unsigned int clock_t
#endif

#ifndef _TYPE_fpos_t
#define _TYPE_fpos_t            typedef long fpos_t
#endif

#ifndef _TYPE_div_t
#define _TYPE_div_t		typedef struct { int quot; int rem; } div_t
#endif

#ifndef _TYPE_ldiv_t
#define _TYPE_ldiv_t		typedef struct { long quot; long rem; } ldiv_t
#endif

/* The following types were added to support the dual-stack */

#ifndef _TYPE_u_int8_t
#define  _TYPE_u_int8_t         typedef unsigned char       u_int8_t
#endif

#ifndef _TYPE_u_int16_t
#define  _TYPE_u_int16_t        typedef unsigned short      u_int16_t
#endif

#ifndef _TYPE_u_int32_t
#define  _TYPE_u_int32_t        typedef unsigned int        u_int32_t
#endif

#ifndef _TYPE_u_int64_t
#define  _TYPE_u_int64_t        typedef unsigned long long  u_int64_t
#endif

#ifndef _TYPE_u_quad_t
#define  _TYPE_u_quad_t         typedef unsigned long long  u_quad_t
#endif

#ifndef _TYPE_quad_t
#define  _TYPE_quad_t           typedef long long           quad_t
#endif


/*
 * stuff for setjmp.h
 */
#ifndef _ARCH_jmp_buf_len
#define _ARCH_jmp_buf_len		8
#endif

/*
 * stuff for stdarg.h
 */
#ifndef _ARCH_va_arg
#define _ARCH_va_arg(list, type)	((type *)(list += sizeof(type)))[-1]
#endif

#ifndef _ARCH_va_end
#define _ARCH_va_end(list)
#endif

#ifndef _ARCH_va_start
#define _ARCH_va_start(list, last_arg)	(list = (va_list)(&last_arg + 1))
#endif

#ifndef _ARCH_va_list
#define _ARCH_va_list			typedef char *va_list
#endif

#endif	/* _ASMLANGUAGE */

/*
 * stuff for limits.h
 */
#ifndef _ARCH_MB_LEN_MAX
#define _ARCH_MB_LEN_MAX		1
#endif

#ifndef _ARCH_CHAR_BIT
#define _ARCH_CHAR_BIT			8
#endif

#ifndef _ARCH_CHAR_MAX
#define _ARCH_CHAR_MAX			127
#endif

#ifndef _ARCH_CHAR_MIN
#define _ARCH_CHAR_MIN			(-127-1)
#endif

#ifndef _ARCH_SHRT_MAX
#define _ARCH_SHRT_MAX			32767
#endif

#ifndef _ARCH_SHRT_MIN
#define _ARCH_SHRT_MIN			(-32767-1)
#endif

#ifndef _ARCH_INT_MAX
#define _ARCH_INT_MAX			2147483647
#endif

#ifndef _ARCH_INT_MIN
#define _ARCH_INT_MIN			(-2147483647-1)
#endif

#ifndef _ARCH_LONG_MAX
#define _ARCH_LONG_MAX			2147483647
#endif

#ifndef _ARCH_LONG_MIN
#define _ARCH_LONG_MIN			(-2147483647-1)
#endif

#ifndef _ARCH_SCHAR_MAX
#define _ARCH_SCHAR_MAX			127
#endif

#ifndef _ARCH_SCHAR_MIN
#define _ARCH_SCHAR_MIN			(-127-1)
#endif

#ifndef _ARCH_UCHAR_MAX
#define _ARCH_UCHAR_MAX			255
#endif

#ifndef _ARCH_USHRT_MAX
#define _ARCH_USHRT_MAX			65535
#endif

#ifndef _ARCH_UINT_MAX
#ifdef __STDC__
#define _ARCH_UINT_MAX			4294967295u
#else
#define _ARCH_UINT_MAX			4294967295
#endif
#endif

#ifndef _ARCH_ULONG_MAX
#ifdef __STDC__
#define _ARCH_ULONG_MAX			4294967295u
#else
#define _ARCH_ULONG_MAX			4294967295
#endif
#endif

/*
 * stuff for float.h
 */
#ifndef _ARCH_FLT_RADIX
#define _ARCH_FLT_RADIX			2
#endif

#ifndef _ARCH_FLT_MANT_DIG
#define _ARCH_FLT_MANT_DIG		24
#endif

#ifndef _ARCH_FLT_DIG
#define _ARCH_FLT_DIG			7	/* not correct in ANSI spec.s */
#endif

#ifndef _ARCH_FLT_ROUNDS
#define _ARCH_FLT_ROUNDS		1
#endif

#ifndef _ARCH_FLT_EPSILON
#define _ARCH_FLT_EPSILON		1.19209290e-07F
#endif

#ifndef _ARCH_FLT_MIN_EXP
#define _ARCH_FLT_MIN_EXP		(-125)
#endif

#ifndef _ARCH_FLT_MIN
#define _ARCH_FLT_MIN			1.17549435e-38F
#endif

#ifndef _ARCH_FLT_MIN_10_EXP
#define _ARCH_FLT_MIN_10_EXP		(-37)
#endif

#ifndef _ARCH_FLT_MAX_EXP
#define _ARCH_FLT_MAX_EXP		128
#endif

#ifndef _ARCH_FLT_MAX
#define _ARCH_FLT_MAX			3.40282347e+38F
#endif

#ifndef _ARCH_FLT_MAX_10_EXP
#define _ARCH_FLT_MAX_10_EXP		38
#endif

#ifndef _ARCH_DBL_MANT_DIG
#define _ARCH_DBL_MANT_DIG		53
#endif

#ifndef _ARCH_DBL_DIG
#define _ARCH_DBL_DIG			15
#endif

#ifndef _ARCH_DBL_EPSILON
#define _ARCH_DBL_EPSILON		2.2204460492503131e-16
#endif

#ifndef _ARCH_DBL_MIN_EXP
#define _ARCH_DBL_MIN_EXP		(-1021)
#endif

#ifndef _ARCH_DBL_MIN
#define _ARCH_DBL_MIN			2.2250738585072014e-308
#endif

#ifndef _ARCH_DBL_MIN_10_EXP
#define _ARCH_DBL_MIN_10_EXP		(-307)
#endif

#ifndef _ARCH_DBL_MAX_EXP
#define _ARCH_DBL_MAX_EXP		1024
#endif

#ifndef _ARCH_DBL_MAX
#define _ARCH_DBL_MAX			1.7976931348623157e+308
#endif

#ifndef _ARCH_DBL_MAX_10_EXP
#define _ARCH_DBL_MAX_10_EXP		308
#endif

#ifndef _ARCH_LDBL_MANT_DIG
#define _ARCH_LDBL_MANT_DIG		53
#endif

#ifndef _ARCH_LDBL_DIG
#define _ARCH_LDBL_DIG			15
#endif

#ifndef _ARCH_LDBL_EPSILON
#define _ARCH_LDBL_EPSILON		2.2204460492503131e-16L
#endif

#ifndef _ARCH_LDBL_MIN_EXP
#define _ARCH_LDBL_MIN_EXP		(-1021)
#endif

#ifndef _ARCH_LDBL_MIN
#define _ARCH_LDBL_MIN			2.2250738585072014e-308L
#endif

#ifndef _ARCH_LDBL_MIN_10_EXP
#define _ARCH_LDBL_MIN_10_EXP		(-307)
#endif

#ifndef _ARCH_LDBL_MAX_EXP
#define _ARCH_LDBL_MAX_EXP		1024
#endif

#ifndef _ARCH_LDBL_MAX
#define _ARCH_LDBL_MAX			1.7976931348623157e+308L
#endif

#ifndef _ARCH_LDBL_MAX_10_EXP
#define _ARCH_LDBL_MAX_10_EXP		308
#endif

/*
 * stuff for math.h
 */
#ifndef _ARCH_HUGH_VAL
#define _ARCH_HUGH_VAL			_ARCH_DBL_MAX
#endif

/*
 * stuff for the kernel
 */
#ifndef _ARCH_BYTE_ORDER
#define _ARCH_BYTE_ORDER		_PARM_BIG_ENDIAN
#endif

#ifndef _ARCH_STACK_DIR
#define _ARCH_STACK_DIR			_PARM_STACK_GROWS_DOWN
#endif

#ifndef _ARCH_ALIGN_STACK
#define _ARCH_ALIGN_STACK		4
#endif

#ifndef _ARCH_ALIGN_MEMORY
#define _ARCH_ALIGN_MEMORY		4
#endif

#ifndef _ARCH_ALIGN_REGS
#define _ARCH_ALIGN_REGS		4
#endif

#ifndef _ARCH_MOVE_SIZE
#define _ARCH_MOVE_SIZE			4
#endif

#ifdef __cplusplus
}
#endif

#endif /* __INCvxTypesBaseh */
