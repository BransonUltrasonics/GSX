// $Header:   D:/databases/VMdb/archives/EN13849/SVRelayDiagnostics.c_v   1.10   20 Jul 2011 16:59:18   ASharma6  $
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

This is the module for  Solenoid Valve Relays monitoring.

Module name: SVRelayDiagnostics

Filename:    SVRelayDiagnostics.c

--------------------------- TECHNICAL NOTES -------------------------------

The code is for diagnose SV Relay.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/SVRelayDiagnostics.c_v  $
 * 
 *    Rev 1.10   20 Jul 2011 16:59:18   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.9   05 Jul 2011 11:01:26   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.6   22 Jun 2011 18:11:38   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.4   01 Apr 2011 11:14:34   PDwivedi
 * Fixes after code review.
 *
 *    Rev 1.3   30 Mar 2011 10:57:12   ASharma6
 * Tabs converted to 3 spaces
 *
 *    Rev 1.2   29 Mar 2011 19:21:08   ASharma6
 * Modifications done to comply to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:10:50   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "inc/DriveDiagnostics.h"
#include "LPC17xx.h"
#include "Global.h"
#include "TestMacros.h"
#include "Diagnostics.h"
//#include "SVRelayDiagnostics.h"

/*-------------------------- Global Variables ------------------------------*/
tSINT32 DriveCount;

UT_VARIABLE(tSINT32 CheckDriveDiagnostics)
/*--------------------------------------------------------------------------*/

/**************************************************************************//**
* \brief -    	DriveDiagnostics
*
* \brief -     Diagnose the Drive.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void DriveDiagnostics(void)
{
   FUNCTION1(CheckDriveDiagnostics, "DriveDiagnostics()");
   CHECKPOINT(CheckDriveDiagnostics, BRANCH_1);

   if (EStop == !ESTOPPRESSED)
   {
      CHECKPOINT(CheckDriveDiagnostics, BRANCH_2);
   }
   else
   {
      /* EStop is not pressed */
      CHECKPOINT(CheckDriveDiagnostics, BRANCH_3);
      if ((DRIVE_FAIL == SV1)/*&&(SV2_FAIL == SV2)*/)
      {
          DriveCount = 0;
        /* Relay state is Drive */
         CHECKPOINT(CheckDriveDiagnostics, BRANCH_8);
         if (FaultReset == TRUE)
         {
            DriveStatus = OK;
            CHECKPOINT(CheckDriveDiagnostics, BRANCH_10);
         }
         else
         {
            CHECKPOINT(CheckDriveDiagnostics, BRANCH_9);
         }
      }
      else
      {
         /* Relay state is not Drive */
         CHECKPOINT(CheckDriveDiagnostics, BRANCH_4);
         DriveCount++;
         if (DriveCount > DRIVECOUNT)
         {
            CHECKPOINT(CheckDriveDiagnostics,BRANCH_6);
            DriveStatus = NOK;
         }
         else
         {
            CHECKPOINT(CheckDriveDiagnostics, BRANCH_5);
         }
      }
   }
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -    ResetHandler
*
* \brief -    ResetHandler is the entry for Drive Diagnostics unit testing.
*
* \param -    none
* 
* \return -   none
*
*****************************************************************************/
void ResetHandler(void)
{
   DiagInit();
   CheckDriveDiagnostics = 1;
   DriveCount = 0;

   // Test with EStops not pressed and Relay state as Drive, Fault Reset, expect it to pass
   EStop = ESTOPPRESSED;
   DRIVE_FAIL = SV1;
   FaultReset = TRUE;
   DriveDiagnostics();
   DiagPrintInt(DriveStatus, OK);

   // Test with EStops not pressed and Relay state as Drive, No Fault Reset, expect it to pass
   EStop = ESTOPPRESSED;
   DRIVE_FAIL = SV1;
   FaultReset = FALSE;
   DriveDiagnostics();
   DiagPrintInt(DriveStatus, OK);

   // Test with EStops not pressed and Relay state as not Drive, DriveCount is less than threshold,expect it to pass
   EStop = ESTOPPRESSED;
   DRIVE_FAIL = ~SV1;
   DriveCount = 2;
   DriveDiagnostics();
   DiagPrintInt(DriveStatus, OK);

   // Test with EStop1 not Pressed and Relay state not drive and count > Threshold, expect it to pass
   EStop = ESTOPPRESSED;
   DRIVE_FAIL = ~SV1;
   DriveCount = DRIVECOUNT+1;
   DriveDiagnostics();
   DiagPrintInt(DriveStatus, NOK);

   // Test with EStop2 Pressed and Relay state not drive and count > Threshold, expect it to pass
   EStop = !ESTOPPRESSED;
   DriveDiagnostics();
   DiagPrintInt(DriveStatus,NOK);

   DiagSummary();
}

#endif

