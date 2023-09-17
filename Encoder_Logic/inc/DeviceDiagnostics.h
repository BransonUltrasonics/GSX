// $Header:   D:/databases/VMdb/archives/EN13849/inc/DeviceDiagnostics.h_v   1.13   20 Jul 2011 16:47:06   ASharma6  $
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

This is the module to check if all the devices that are related to the
safety function are functioning properly.

Module name: DeviceDiagnostics

Filename:    DeviceDiagnostics.h

--------------------------- TECHNICAL NOTES -------------------------------

This code checks the status of the fault variables, that belong to the
devices.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/DeviceDiagnostics.h_v  $
 * 
 *    Rev 1.13   20 Jul 2011 16:47:06   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.12   05 Jul 2011 10:56:30   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.9   22 Jun 2011 18:14:06   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.8   15 Jun 2011 17:39:18   ygupta
 * EStop and LE diagnostics fix
 *
 *    Rev 1.7   06 May 2011 11:41:36   pdwivedi
 * Added changes for Setoutputs, Inputs and SystemInitialization.
 *
 *    Rev 1.6   04 May 2011 08:58:20   pdwivedi
 * Added changes for cross monitoring(module level test cases), Device Diagnostics(channged) and SBeam Diagnostics(modified).
 *
 *    Rev 1.5   02 May 2011 04:00:54   pdwivedi
 * Synchronize with CVS.
 *
 *    Rev 1.4   30 Mar 2011 11:25:50   ASharma6
 * Coverted tabs to 3 spaces each
 *
 *    Rev 1.3   30 Mar 2011 08:10:16   ygupta
 * Corrected Typo.
 *
 *    Rev 1.2   29 Mar 2011 19:19:22   ASharma6
 * Modifications done for compliance to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:16:10   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef DEVICEDIAGNOSTICS_H_
#define DEVICEDIAGNOSTICS_H_

#include "LPC17xx.h"

enum DEVICEDIAGSTATUS
{
   OK,
   NOK,
   INDETERMINATE,
};
enum FaultFlags
{
	PBFaultFlag = 1,
	UFAIL24FaultFlag = 2,
	EStopFaultFlag = 4,
	LEFaultFlag = 8,
	SBeamFaultFlag = 16,
	TRSFaultFlag = 32,
	DriveFaultFlag = 64,
	CMFaultFlag = 128,
	LUFaultFlag = 256,
	SonicsEnFaultFlag = 512

};


/* Function prototypes */
void ADC_Test(void);
void UFAIL24VDiagnostics(void);
void DriveDiagnostics(void) ;
void SBeamDiagnostics(void);
void TRSDiagnostics(void);
void ULSLEDiagnostics(void);
void EStopDiagnostics();
void DeviceDiagnostics(void);
void SonicsEnDiagnostics(void);
tUINT16 FirstBitFoundPos(tUINT32);

#endif /* DEVICEDIAGNOSTICS_H_ */
