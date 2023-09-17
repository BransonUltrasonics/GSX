/* trcLib.h - header file for trcLib.c */

/*
 * Copyright (c) 1990-1992, 2003, 2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,29apr10,pad  Moved extern C statement after include statements.
01j,03jun09,jmp  Adapted for 64-bit support. Removed the obsolete trcStack_r.
01i,24oct03,dbs  added re-entrant trcStack option
01h,22sep92,rrr  added support for c++
01g,18sep92,jcf  added include of regs.h.
01f,04jul92,jcf  cleaned up.
01e,26may92,rrr  the tree shuffle
01d,07apr92,yao  made ANSI prototype of trcStack() consistent for all
		 architecutres.
01c,09jan92,jwt  modified trcStack() prototype for SPARC.
01b,04oct91,rrr  passed through the ansification filter
		  -fixed #else and #endif
		  -changed VOID to void
		  -changed copyright notice
01a,05oct90,shl created.
*/

#ifndef __INCtrcLibh
#define __INCtrcLibh

#include <regs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* function declarations */

extern void 	trcStack (REG_SET * pRegs, FUNCPTR printRtn, TASK_ID tid);

#ifdef __cplusplus
}
#endif

#endif /* __INCtrcLibh */
