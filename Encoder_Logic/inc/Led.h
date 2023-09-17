// $Header:   D:/databases/VMdb/archives/EN13849/inc/Led.h_v   1.7   20 Jul 2011 16:47:20   ASharma6  $
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

 Module name: Led
 File name: Led.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/Led.h_v  $
 * 
 *    Rev 1.7   20 Jul 2011 16:47:20   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.6   05 Jul 2011 10:56:42   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:14:20   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 11:57:20   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:16:58   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef LED_H_
#define LED_H_

#include "Global.h"

#define LED1_GMASK   0xFFFFFF3F
#define LED1_RMASK   0xFFFFFCFF
#define LED2_GMASK   0xFFFFF3FF
#define LED2_RMASK   0xFFFFCFFF

#define LEDOUTPUTMASK 0x00780000

void LED_On(tUINT32 num);
void LED_Off(tUINT32 num);
void LED_Out(unsigned int value);

#endif /* LED_H_ */
