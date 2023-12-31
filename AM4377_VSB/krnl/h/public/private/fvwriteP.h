/* fvwriteP.h - definitions for the ANSI stdio library */

/* Copyright 1992,2009 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,20mar09,xia  updated for LP64 data model
01b,22sep92,rrr  added support for c++
01a,29jul92,smb  taken from UCB stdio library
*/

#ifndef __INCfvwritePh
#define __INCfvwritePh

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
 *	@(#)fvwrite.h	5.1 (Berkeley) 1/20/91

SEE ALSO: American National Standard X3.159-1989
*/

/* I/O descriptors for __sfvwrite().  */

struct __siov 
    {
    void * iov_base;
    size_t iov_len;
    };

struct __suio 
    {
    struct __siov *uio_iov;
    int	   uio_iovcnt;
    size_t uio_resid;
    };

#if defined(__STDC__) || defined(__cplusplus)
extern int __sfvwrite(FILE *, struct __suio *);
#else
extern int __sfvwrite();
#endif

#ifdef __cplusplus
}
#endif

#endif /* __INCfvwritePh */
