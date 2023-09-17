/*
 $Header:   D:/databases/VMdb/archives/EN13849/Reset.s_v   1.7   20 Jul 2011 16:58:46   ASharma6  $
 */
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

This is the Main module for CPU Register Test.

Module name: Reset

Filename:    Reset.s

--------------------------- TECHNICAL NOTES -------------------------------

This code Handles the CPU Register Test.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/Reset.s_v  $
; 
;    Rev 1.7   20 Jul 2011 16:58:46   ASharma6
; Modifications for review comments
; 
;    Rev 1.6   05 Jul 2011 11:00:56   ASharma6
; ULS, CM changes
; 
;    Rev 1.3   22 Jun 2011 18:11:04   ASharma6
; LE Fault reset allowed in horn up state
; Part contact lost made more resilient
; Cross monitoring detects other processor was reset and resets itself
; SBeam diagnostics is more resilient
; 
;    Rev 1.2   31 Mar 2011 12:13:20   PDwivedi
; Fixes after review.
; 
;    Rev 1.1   25 Mar 2011 09:08:22   ygupta
; Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/

		.thumb
		.global ResetHandler
		.thumb_func
ResetHandler:
		ldr		r0,=_vStackTop
		movs	sp,r0
		ldr		r0,=RegTest
		blx		r0
		ldr		r0,=Reg1Test
		blx		r0
   		ldr		r0,=Reg2Test
		blx		r0
		ldr		r0,=Reg3Test
		blx		r0
		ldr		r0,=Reg4Test
		blx		r0
		ldr		r0,=Reg5Test
		blx		r0
		ldr		r0,=Reg6Test
		blx		r0
		ldr		r0,=Reg7Test
		blx		r0
		ldr		r0,=Reg8Test
		blx		r0
		ldr		r0,=Reg9Test
		blx		r0
		ldr		r0,=Reg10Test
		blx		r0
		ldr		r0,=Reg11Test
		blx		r0
		ldr		r0,=Reg12Test
		blx		r0
		ldr		r0,=Reg14Test
		blx		r0
		ldr		r0,=MSPRegTest
		blx 	r0
		ldr		r0,=PSPRegTest
		blx 	r0
		ldr		r0,=APSRRegTest
		blx 	r0
		ldr		r0,=PRIMASKRegTest
		blx		r0
		ldr		r0,=FAULTMASKRegTest
		blx		r0
		ldr		r0,=BASEPRIREGTest
		blx		r0
		b	  .
