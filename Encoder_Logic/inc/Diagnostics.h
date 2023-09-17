// $Header:   D:/databases/VMdb/archives/EN13849/inc/Diagnostics.h_v   1.7   20 Jul 2011 16:47:10   ASharma6  $
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
This module defines the various diagnostic functions that are used for
white-box testing

Module name: Diagnostics

Filename:    Diagnostics.c

--------------------------- TECHNICAL NOTES -------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/Diagnostics.h_v  $
 * 
 *    Rev 1.7   20 Jul 2011 16:47:10   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.6   05 Jul 2011 10:56:32   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:14:10   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   01 Apr 2011 11:28:20   PDwivedi
 * Fixex after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:16:22   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef DIAGNOSTICS_H_
#define DIAGNOSTICS_H_

void DiagInit(void);
void DiagCheckpoint(int number);
void DiagPrint(int result, int expected);
void DiagPrintInt(int result, int expected);
void DiagSummary(void);

void ResetdiagTests();
void ResetdiagErrors();

#endif
