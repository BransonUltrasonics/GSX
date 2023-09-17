// $Header:   D:/databases/VMdb/archives/EN13849/inc/EStopDiagnostics.h_v   1.11   20 Jul 2011 16:47:12   ASharma6  $
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
This module check the proper functionality of the ESTOP, including the
Estop Button and the Estop Relay.

Module name: EStopDiagnostics

Filename:    EStopDiagnostics.c

--------------------------- TECHNICAL NOTES -------------------------------
This module updates the values of the Estop Status variables.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/EStopDiagnostics.h_v  $
 * 
 *    Rev 1.11   20 Jul 2011 16:47:12   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.10   05 Jul 2011 10:56:34   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.7   22 Jun 2011 18:14:12   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.6   15 Jun 2011 17:39:24   ygupta
 * EStop and LE diagnostics fix
 *
 *    Rev 1.5   20 May 2011 20:05:52   ygupta
 * EStop Diagnostics are done only when EStop is not pressed.
 *
 *    Rev 1.4   02 May 2011 04:01:06   pdwivedi
 * Synchronize with CVS.
 *
 *    Rev 1.3   30 Mar 2011 11:22:14   ASharma6
 * Converted tabs to 3 spaces each
 *
 *    Rev 1.2   29 Mar 2011 19:19:06   ASharma6
 * Modifications done for compliance to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:16:32   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef ESTOPDIAGNOSTICS_H_
#define ESTOPDIAGNOSTICS_H_

#include "DeviceDiagnostics.h"
#include "Global.h"

#define ESTOPCNT        2000

/* ESTOP button changed now
 * ESTOP1 is normally closed
 * ESTOP2 is normally open
 */
enum {
   ESTOP1PRESSEDCPU1,
   ESTOP1NOTPRESSEDCPU1
};

enum {
   ESTOP2NOTPRESSEDCPU1,
   ESTOP2PRESSEDCPU1
};
enum {
   ESTOP1NOTPRESSEDCPU2,
   ESTOP1PRESSEDCPU2
};

enum {
   ESTOP2PRESSEDCPU2,
   ESTOP2NOTPRESSEDCPU2
};

void EStopDiagnostics(void);

#endif /* ESTOPDIAGNOSTICS_H_ */
