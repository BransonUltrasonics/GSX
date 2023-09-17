// $Header:   D:/databases/VMdb/archives/EN13849/inc/stdio.h_v   1.7   20 Jul 2011 16:48:26   ASharma6  $
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 2010-11              */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------
 MCB1700

 Module name: stdio
 File name: stdio.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/stdio.h_v  $
 * 
 *    Rev 1.7   20 Jul 2011 16:48:26   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.6   05 Jul 2011 10:57:28   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:15:16   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   01 Apr 2011 11:30:34   PDwivedi
 * Fixex after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:20:14   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef STDIO_H_
#define STDIO_H_

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)

typedef __builtin_va_list __gnuc_va_list;
typedef __gnuc_va_list va_list;

int printf(const char *, ...);
int pprintf(const char *, ...);
int puts(const char *);
int sprintf(char *, const char *, ...);
// This is an exception, but it is here because it's too closely related.
int getcharWait(int timeout);
char * gets(char * buf);

#endif /* STDIO_H_ */
