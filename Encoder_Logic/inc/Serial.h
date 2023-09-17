// $Header:   D:/databases/VMdb/archives/EN13849/inc/Serial.h_v   1.8   09 Apr 2015 17:18:44   ewomack  $
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
 Module name: Serial
 File name: Serial.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/Serial.h_v  $
 * 
 *    Rev 1.8   09 Apr 2015 17:18:44   ewomack
 * Compiler warning cleanup.
 * 
 *    Rev 1.7   20 Jul 2011 16:48:16   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.6   05 Jul 2011 10:57:18   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:15:04   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 11:58:20   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:19:38   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef SERIAL_H_
#define SERIAL_H_

#include "Global.h"

void SerialOut(char * string);
void SerialOutChar(tSINT32 c);
tSINT32 SerialRecvChar(void);
#endif
