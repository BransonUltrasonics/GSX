// $Header:   D:/databases/VMdb/archives/EN13849/inc/TRSDiagnostics.h_v   1.11   19 Sep 2011 10:36:02   ASharma6  $
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 2010-11            */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------
 This module has the diagnostics of the Trigger Switch used in the Plastic
 Welder.

 Module name: TRSDiagnostics
 File name: TRSDiagnostics.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/TRSDiagnostics.h_v  $
 * 
 *    Rev 1.11   19 Sep 2011 10:36:02   ASharma6
 * For IW+ only: After ULS, the Trigger switch is not looked at for declaring part contact until the horn goes 1/8 of an inch past the ULS.
 * 
 *    Rev 1.9   20 Jul 2011 16:48:36   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.8   05 Jul 2011 10:57:38   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.5   22 Jun 2011 18:15:28   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.4   30 Mar 2011 11:24:24   ASharma6
 * Converted tabs to 3 spaces
 *
 *    Rev 1.3   30 Mar 2011 08:10:30   ygupta
 * Corrected Typo.
 *
 *    Rev 1.2   29 Mar 2011 19:19:18   ASharma6
 * Modifications done for compliance to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:20:48   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef TRSDIAGNOSTICS_H_
#define TRSDIAGNOSTICS_H_

#include "DeviceDiagnostics.h"

#define NoTRSDiagThres 200
#define TRSErrThres    30

/* Function prototypes */
void TRSDiagnostics(void);


#endif /* TRSDIAGNOSTICS_H_ */
