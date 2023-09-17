/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/buffer.h,v 1.2 2001/11/06 21:35:01 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1988-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: buffer.h,v $
 * Revision 1.2  2001/11/06 21:35:01  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:22  tneale
 * Tornado shuffle
 *
 * Revision 9.4  2001/01/19 22:22:04  paul
 * Update copyright.
 *
 * Revision 9.3  2000/03/17 19:48:31  meister
 * Update copyright notice
 *
 * Revision 9.2  2000/03/09 20:58:33  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.1  2000/03/09 16:55:08  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.0  1998/10/16 22:10:07  sar
 * Update version stamp to match release
 *
 * Revision 8.3  1998/06/18 04:42:00  sar
 * Modified bflags in ebuffer to be unsigned ints and updated
 * calling routines accordingly
 *
 * Revision 8.2  1998/05/23 20:01:13  sar
 * Added EBufferAllocateLoad and updated some types
 *
 * Revision 8.1  1998/02/25 04:50:28  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:23  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:05  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:34  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 3.6  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo.h> where it was correct.
 *
 * Revision 3.5  1993/04/22  20:14:13  sar
 * Updating of macros and defines to use new naming scheme and get the
 * names to be more useful in general.  (use INSTALL_ENVOY_SNMP_VERSION_1
 * for example).
 *
 * Revision 3.4  1993/03/20  00:21:50  sar
 * Replaced EBufferAdd with EBufferAdjust which takes a length and if
 * necessary extends the buffer.
 * Added EBufferReplace which puts the given string into the buffer
 * extending the buffer if necessary.
 *
 * Revision 3.3  1993/03/05  03:56:25  dab
 * fixed up types
 *
 * Revision 3.2  1993/03/02  19:04:38  sar
 * added definition of EBuffer_add which extends the string in a buffer.
 *
 * Revision 3.1  1993/02/17  20:46:41  sar
 * Added EBufferStart - macro: obtain address of first byte in a buffer.
 * Added EBufferSeek - definition: move the pointer in a buffer.
 *
 * Revision 3.0  92/04/03  19:52:27  dab
 * Release 3.0
 * 
 * Revision 2.102  1992/03/31  14:04:08  dab
 * Fixed EBufferReset to fixup the remaining field too.
 *
 * Revision 2.101  92/02/04  11:04:37  dab
 * Changed the EBufferReset macro to return void.
 * 
 * Revision 2.100  91/08/09  14:09:44  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:21  romkey
 * Initial revision
 * 
 * 
 * Revision 2.0   31-Mar-1990 15:11:22 dab
 * Release 2.00
 * 
 *    Rev 1.1   19 Sep 1988 17:27:04
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:12
 * Initial revision.
*/

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(buffer_inc))
#define buffer_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

/* The EBUFFER_T structure is used to control encoding into a buffer	*/
typedef struct EBUFFER_S
	{
	unsigned int bflags;
	bits8_t     *start_bp;	/* Start of the buffer.			    */
	bits8_t     *next_bp;   /* Next location in buffer to be filled     */
	ALENGTH_T    remaining; /* Number of empty spots remaining in buffer*/
	} EBUFFER_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

/* Values for bflags	*/
#define	BFL_IS_ALLOC		0x0002	/* Buffer must be obtained */
#define	BFL_IS_DYNAMIC		0x0001	/* Buffer was obtained by malloc    */
#define	BFL_IS_STATIC		0x0000	/* Buffer is statically allocated   */

/****************************************************************************

NAME:  EBufferInitialize

PURPOSE:  Initialize a buffer to a know, but not usable state
	  EBufferSetup must be used to make the buffer ready for use.

PARAMETERS:
	    EBUFFER_T *	    Buffer descriptor

RETURNS:    Nothing

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferInitialize(E) (((EBUFFER_T *)(E))->bflags=0,   \
			      ((EBUFFER_T *)(E))->next_bp=(bits8_t *)0,  \
			      ((EBUFFER_T *)(E))->start_bp=(bits8_t *)0,  \
			      ((EBUFFER_T *)(E))->remaining=(ALENGTH_T)0)

/****************************************************************************

NAME:  EBufferSetup

PURPOSE:  Setup a buffer to receive ASN.1 encoded data

PARAMETERS:
	    unsigned int    BFL_xxx flags from buffer.h
	    EBUFFER_T *	    Buffer descriptor
            bits8_t *       Address of the buffer
            ALENGTH_T       Length of the buffer

RETURNS:    Nothing

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferSetup(F,E,B,L) (((EBUFFER_T *)(E))->bflags=(F),  \
			       ((EBUFFER_T *)(E))->start_bp=(bits8_t *)(B),  \
			       ((EBUFFER_T *)(E))->next_bp=(bits8_t *)(B),  \
			       ((EBUFFER_T *)(E))->remaining=(ALENGTH_T)(L))

/****************************************************************************

NAME:  EBufferPreLoad

PURPOSE:  Pre-load data into a buffer

PARAMETERS:
	    unsigned int    BFL_xxx flags from buffer.h
	    EBUFFER_T *	    Buffer descriptor
            bits8_t *       Address of the buffer containing the data
            ALENGTH_T       Length of the data

RETURNS:    Nothing

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferPreLoad(F,E,B,L) (	\
	((EBUFFER_T *)(E))->bflags=(F),	\
	((EBUFFER_T *)(E))->start_bp=(bits8_t *)(B),	\
	((EBUFFER_T *)(E))->next_bp=((bits8_t *)(B)+(ALENGTH_T)(L)),	\
	((EBUFFER_T *)(E))->remaining=(ALENGTH_T)0)

/****************************************************************************

NAME:  EBufferNext

PURPOSE:  Obtain address of next free byte in a buffer

PARAMETERS:
	    EBUFFER_T *	    Buffer descriptor

RETURNS:    bits8_t *	    Buffer descriptor

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferNext(E)	(((EBUFFER_T *)(E))->next_bp)

/****************************************************************************

NAME:  EBufferStart

PURPOSE:  Obtain address of first byte in a buffer

PARAMETERS:
	    EBUFFER_T *	    Buffer descriptor

RETURNS:    bits8_t *	    Buffer descriptor

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferStart(E)	(((EBUFFER_T *)(E))->start_bp)

/****************************************************************************

NAME:  EBufferUsed

PURPOSE:  Indicate how many octets are currently in the buffer.

PARAMETERS:
	    EBUFFER_T *	    Buffer descriptor

RETURNS:    ALENGTH_T	    Number of octets used.

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferUsed(E)	((ALENGTH_T)((((EBUFFER_T *)(E))->next_bp) -	\
				     (((EBUFFER_T *)(E))->start_bp)))

/****************************************************************************

NAME:  EBufferRemaining

PURPOSE:  Indicate how many octets are currently unused in the buffer.

PARAMETERS:
	    EBUFFER_T *	    Buffer descriptor

RETURNS:    ALENGTH_T	    Number of octets unused.

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferRemaining(E)	(((EBUFFER_T *)(E))->remaining)

/****************************************************************************

NAME:  EBufferReset

PURPOSE:  Reset the buffer to an empty state, just like after EBufferSetup

PARAMETERS:
	    EBUFFER_T *	    Buffer descriptor

RETURNS:    Nothing

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferReset(E)	( \
			 (((EBUFFER_T *)(E))->remaining += \
			  ((EBUFFER_T *)(E))->next_bp - \
			  ((EBUFFER_T *)(E))->start_bp), \
			 ((void)(((EBUFFER_T *)(E))->next_bp =	\
				 ((EBUFFER_T *)(E))->start_bp)))

extern	int	EBufferClone   __((EBUFFER_T *, EBUFFER_T *));
extern	void	EBufferClean   __((EBUFFER_T *));
extern	void	EBufferAppend  __((EBUFFER_T *, EBUFFER_T *));
extern  int     EBufferSeek    __((EBUFFER_T *, ALENGTH_T, int));
extern  int	EBufferAdjust  __((EBUFFER_T *, ALENGTH_T));
extern  int	EBufferReplace __((EBUFFER_T *, bits8_t *, ALENGTH_T));
extern  int     EBufferAllocateLoad __((unsigned int, EBUFFER_T *,
					bits8_t *, ALENGTH_T));

#ifdef __cplusplus
}
#endif

#endif	/* buffer_inc */
