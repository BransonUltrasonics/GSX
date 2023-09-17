/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/localio.h,v 1.3 2003/01/15 13:58:56 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/*****************************************************************************
 *  Copyright 1986-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: localio.h,v $
 * Revision 1.3  2003/01/15 13:58:56  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/06 21:50:40  josh
 * second (and hopefully final) pass of new path hacking
 *
 * Revision 1.1.1.1  2001/11/05 17:47:22  tneale
 * Tornado shuffle
 *
 * Revision 9.4  2001/01/19 22:22:05  paul
 * Update copyright.
 *
 * Revision 9.3  2000/03/17 19:48:34  meister
 * Update copyright notice
 *
 * Revision 9.2  2000/03/09 20:58:33  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.1  2000/03/09 16:56:08  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.0  1998/10/16 22:10:14  sar
 * Update version stamp to match release
 *
 * Revision 8.3  1998/06/22 03:11:10  sar
 * Changed the type used for lengths in localio.  We now use ALENGTH_Ts
 * to make it match the rest of the code and keep compilers happy
 *
 * Revision 8.2  1998/02/25 15:21:23  sra
 * Finish moving types.h, bug.h, and bugdef.h to common/h/.
 *
 * Revision 8.1  1998/02/25 04:50:36  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:25  sar
 * Updated revision to 8.0
 *
 * Revision 7.3  1997/03/20 06:48:11  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.2  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.1  1997/01/16 20:46:35  sar
 * changed uchar to bits8_t
 *
 * Revision 7.0  1996/03/18  20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.1  1996/01/10  16:23:25  sar
 * Removed some no_pp stuff
 * Added the start of buffer macro
 *
 * Revision 6.0  1995/05/31  21:46:43  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 3.3  1993/05/19  20:07:52  sar
 * tiding up types to make compilers happy, (timestamps to UINT_32_T,
 * sizes to ALENGTH_T).
 *
 * Revision 3.2  1993/02/17  20:48:03  sar
 * Added Lcl_Next - macro: get current position in buffer.
 * Added Lcl_Dup  - defin: duplicate a lcl_file.
 * Changed Lcl_Size - macro: corrected lbuf->next to lbuf_next.
 *
 * Revision 3.1  92/07/31  11:54:43  dab
 * Added Lcl_Size().
 * 
 * Revision 3.0  92/04/03  19:52:27  dab
 * Release 3.0
 * 
 * Revision 2.101  92/02/05  18:49:59  dab
 * In lcl_* macros don't cast L as (LCL_FILE *) because it already
 * should be.
 * Made sure all returns of Lcl_Getc() were the same type.
 * 
 * Revision 2.100  91/08/09  14:09:46  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:26  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:11:26
 * Release 2.00
 * 
 *    Rev 1.2   19 Sep 1988 17:48:36
 * Added header and version identification lines.  No code changes.
*/

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(localio_inc))
#define localio_inc

#ifdef __cplusplus
extern"C" {
#endif

/* Get the standard types */
#if !defined(EPILOGUE_TYPES_H)
#include <wrn/wm/common/types.h>
#endif

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct LCL_FILE_S
	{
	bits8_t	 lcl_flags;
	bits8_t	*lbuf_start;
	bits8_t	*lbuf_next;
	bits8_t	*lbuf_end;	/* Address AFTER last byte */
	} LCL_FILE;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

#define LCL_MALLOC  0x01    /* LCL_FILE was malloc-ed at open */
#define LCL_EOF	    0x80    /* EOF encountered */

/* Start of buffer */
#define Lcl_Start(L) (L)->lbuf_start

#define Lcl_Eof(L) (((L)->lcl_flags&LCL_EOF)?-1:0)
			 
/* Current position in stream. */
#define Lcl_Tell(L) (ALENGTH_T)((L)->lbuf_next - (L)->lbuf_start)

/* Address of current positon in buffer */
#define Lcl_Next(L) (L)->lbuf_next

/* Amount of data left in stream. */
#define Lcl_Size(L) (ALENGTH_T)((L)->lbuf_end - (L)->lbuf_next)

#define GETC_MACRO
#if defined(GETC_MACRO)
/* The following is an macro version of this routine Lcl_Getc */
#define Lcl_Getc(L)   ( (((L)->lcl_flags & LCL_EOF) ? -1 :	\
			 (((L)->lbuf_next < (L)->lbuf_end) ?	\
			  (int)(*((L)->lbuf_next++)) :	\
			  (((L)->lcl_flags |= LCL_EOF), -1))))
#endif	/* GETC_MACRO */

extern	LCL_FILE    *Lcl_Open   (LCL_FILE *, bits8_t *, ALENGTH_T);
extern	void	     Lcl_Close  (LCL_FILE *);
#if !defined(GETC_MACRO)
extern	int	     Lcl_Getc   (LCL_FILE *);
#endif	/* GETC_MACRO */
extern	int	     Lcl_Peekc  (LCL_FILE *);
extern	ALENGTH_T    Lcl_Read   (LCL_FILE *, bits8_t *, ALENGTH_T);
extern	int	     Lcl_Seek   (LCL_FILE *, ALENGTH_T, int);
extern	ALENGTH_T    Lcl_Resize (LCL_FILE *, ALENGTH_T, int);
extern	LCL_FILE    *Lcl_Dup    (LCL_FILE *, LCL_FILE *);

#ifdef __cplusplus
}
#endif

#endif

