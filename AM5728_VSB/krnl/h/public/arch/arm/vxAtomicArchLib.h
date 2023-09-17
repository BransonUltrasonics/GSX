/* vxAtomicArchLib.h - ARM specific atomic library header */

/*
 * Copyright (c) 2007-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
29apr10,pad  Moved extern C statement after include statements.
15may09,gls  made memory barriers available in user space
25feb09,j_b  merge ARM SMP support:
05mar08,j_b  fix gnu inline assembly declarations
15feb08,j_b  add inline _dsb (Data Synchronization Barrier)
09jul07,to   added memory barrier macros for ARMARCH6
16jun08,zl   use _WRS_BARRIER() for memory barrier implementations
07may07,m_h  Atomic Operator definitions
07may07,m_h  Created
*/

#ifndef __INCvxAtomicArchLibh
#define __INCvxAtomicArchLibh

#include <arch/arm/vxAtomicArchLibCommon.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __INCvxAtomicArchLibh */
