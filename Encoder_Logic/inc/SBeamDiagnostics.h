// $Header:   D:/databases/VMdb/archives/EN13849/inc/SBeamDiagnostics.h_v   1.11   20 Jul 2011 16:48:14   ASharma6  $
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
 RTC Driver.
 Module name: SBeamDiagnostics
 File name: SBeamDiagnostics.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/SBeamDiagnostics.h_v  $
 * 
 *    Rev 1.11   20 Jul 2011 16:48:14   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.10   05 Jul 2011 10:57:16   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.7   22 Jun 2011 18:15:00   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.3   30 Mar 2011 11:22:18   ASharma6
 * Converted tabs to 3 spaces each
 *
 *    Rev 1.2   29 Mar 2011 19:19:12   ASharma6
 * Modifications done for compliance to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:19:28   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/


#ifndef SBEAMDIAGNOSTICS_H_
#define SBEAMDIAGNOSTICS_H_

#include "DeviceDiagnostics.h"

#define MAX_FORCE 2870  //1000lb compression
#define MIN_FORCE 138	//110lb  tension

#define MAXSBEAMERRCNT        150

#define ForceVal1 2500
#define ForceVal2 100
#define ForceVal3 2500

/* Function prototypes */
void SBeamDiagnostics(void);

#endif /* SBEAMDIAGNOSTICS_H_ */
