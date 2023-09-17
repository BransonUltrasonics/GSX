// $Header:   D:/databases/VMdb/archives/EN13849/inc/ULSLEDiagnostics.h_v   1.11   20 Jul 2011 16:48:42   ASharma6  $
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
 This module has the diagnostics of the ULS and the LE used in the Plastic
 Welder.

 Module name: ULSLEDiagnostics
 File name: ULSLEDiagnostics.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/ULSLEDiagnostics.h_v  $
 * 
 *    Rev 1.11   20 Jul 2011 16:48:42   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.10   05 Jul 2011 10:57:44   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.5   22 Jun 2011 18:15:32   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.4   15 Jun 2011 17:40:18   ygupta
 * EStop and LE diagnostics fix
 *
 *    Rev 1.3   30 Mar 2011 11:22:26   ASharma6
 * Converted tabs to 3 spaces each
 *
 *    Rev 1.2   29 Mar 2011 19:19:20   ASharma6
 * Modifications done for compliance to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:21:02   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/


#ifndef ULSLEDIAGNOSTICS_H_
#define ULSLEDIAGNOSTICS_H_

#include "DeviceDiagnostics.h"

#define LE_ZEROPOSITION         0
#define LE_POSITIONTESTVALUE2   80
#define LE_POSITIONTESTVALUE1   100
#define MINDISTANCE				20000  //2mm

#define LE_ERRTHRES             30

/* Function prototypes */
void ULSLEDiagnostics(void);

#endif /* ULSLEDIAGNOSTICS_H_ */
