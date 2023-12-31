/* floatio.h - definitions for ANSI standard stdio module */

/* Copyright (c) 1992, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */


/*
modification history
--------------------
01e,06sep10,fan  added _func_fioFltFormatRtn and _func_fioFltScanRtn
01d,03apr09,mcm  Moved fioFltInstall here from fioLib.h
01c,22sep92,rrr  added support for c++
01a,29jul92,smb  taken from UCB stdio library.
*/

#ifndef __INCfloatioPh
#define __INCfloatioPh

#ifdef __cplusplus
extern "C" {
#endif

/*
DESCRIPTION
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)floatio.h	5.1 (Berkeley) 1/20/91
 *
 * Floating point scanf/printf (input/output) definitions.

INCLUDE FILE: 

SEE ALSO: American National Standard X3.159-1989
*/

/* 11-bit exponent (VAX G floating point) is 308 decimal digits */

#define	MAXEXP		308

/* 128 bit fraction takes up 39 decimal digits; max reasonable precision */

#define	MAXFRACT	39

/* function declarations */
extern void     fioFltInstall (FUNCPTR formatRtn, FUNCPTR scanRtn);

extern int  (* _func_fioFltFormatRtn)
    (
    va_list *   pVaList,        /* vararg list */
    int         precision,      /* precision */
    BOOL        doAlt,          /* doAlt boolean */
    int         fmtSym,         /* format symbol */
    BOOL *      pDoSign,        /* where to fill in doSign result */
    char *      pBufStart,      /* buffer start */
    char *      pBufEnd         /* buffer end */
    );


extern BOOL  (* _func_fioFltScanRtn)
    (
    FAST int *  pReturn,
    int         returnSpec,     /* 0, 'l', 'L' */
    int         fieldwidth,
    FAST GET_NEXT_CHAR_FUNCPTR getRtn,
    FAST void    *getArg,
    int *       pCh,
    int *       pNchars
    );

#ifdef __cplusplus
}
#endif

#endif /* __INCfloatioPh */
