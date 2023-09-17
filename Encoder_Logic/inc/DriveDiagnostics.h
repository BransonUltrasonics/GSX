// $Header:   D:/databases/VMdb/archives/EN13849/inc/SVRelayDiagnostics.h_v   1.9   20 Jul 2011 16:48:28   ASharma6  $
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
 This module has the diagnostics of the SVRelay used in the hardware
 implementation

 Module name: SVRelayDiagnostics
 File name: SVRelayDiagnostics.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/SVRelayDiagnostics.h_v  $
 * 
 *    Rev 1.9   20 Jul 2011 16:48:28   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.8   05 Jul 2011 10:57:32   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.5   22 Jun 2011 18:15:20   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.3   30 Mar 2011 11:22:20   ASharma6
 * Converted tabs to 3 spaces each
 *
 *    Rev 1.2   29 Mar 2011 19:19:14   ASharma6
 * Modifications done for compliance to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:20:26   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/


#ifndef DRIVEDIAGNOSTICS_H_
#define DRIVEDIAGNOSTICS_H_

#include "DeviceDiagnostics.h"

#define DRIVECOUNT 100
#define ESTOPPRESSED 0

/* Function prototypes */
void DriveDiagnostics(void);

#endif /* DRIVEDIAGNOSTICS_H_ */
