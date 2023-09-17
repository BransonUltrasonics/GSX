// $Header:   D:/databases/VMdb/archives/EN13849/EStopDiagnostics.c_v   1.15   20 Jul 2011 16:56:42   ASharma6  $
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

This is the module for the DeviceDiagnostics for all the devices on the Actuator controller board.

Module name: EStopDiagnostics

Filename:    EStopDiagnostics.c

--------------------------- TECHNICAL NOTES -------------------------------

This code Diagnoses the  states of EStop Button and Relay devices connected to the controller.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/EStopDiagnostics.c_v  $
 * 
 *    Rev 1.15   20 Jul 2011 16:56:42   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.14   05 Jul 2011 10:59:26   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.11   22 Jun 2011 18:09:14   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.8   15 Jun 2011 17:44:34   ygupta
 * EStop and LE diagnostics fix
 *
 *    Rev 1.7   20 May 2011 20:05:50   ygupta
 * EStop Diagnostics are done only when EStop is not pressed.
 *
 *    Rev 1.6   04 May 2011 08:57:30   pdwivedi
 * Added changes for cross monitoring(module level test cases), Device Diagnostics(channged) and SBeam Diagnostics(modified).
 *
 *    Rev 1.5   02 May 2011 03:56:30   pdwivedi
 * Synchronize with CVS.
 *
 *    Rev 1.4   01 Apr 2011 11:13:16   PDwivedi
 * Fixes after code review.
 *
 *    Rev 1.3   30 Mar 2011 10:57:06   ASharma6
 * Tabs coverted to 3 spaces
 *
 *    Rev 1.2   29 Mar 2011 19:21:16   ASharma6
 * Modifications done to comply to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:04:34   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "EStopDiagnostics.h"
#include "TestMacros.h"
#include "Global.h"

UT_VARIABLE(tSINT32 eStopDiagnostics)

tSINT32 EStopCount;
/*--------------------------------------------------------------------------*/

/**************************************************************************//**
* \brief -    	EStopDiagnostics
*
* \brief -     Diagnose the ESTOP Button and Relay conditions.
*
* \param -		none
*
* \return -   	none
*
*****************************************************************************/
void EStopDiagnostics(void)
{
   FUNCTION1(eStopDiagnostics, "EStopDiagnostics()");
   CHECKPOINT(eStopDiagnostics, BRANCH_1);

      /* When EStop button is pressed and a user is trying to release it,
      there can be a long period when ESTOP1 == ESTOP2, depending upon how
      slowly the user is releasing the button. The diagnostics need not take
      place until the ESTOP is detected as released because nothing harmful
      can take place anyways
      */
      if (ESTOP1 != ESTOP2)
      {
         // Two poles - one normally open, one normally closed.
         CHECKPOINT(eStopDiagnostics, BRANCH_2);
         EStopCount = FaultClr;
         if (FaultReset == TRUE)
         {
            CHECKPOINT(eStopDiagnostics, BRANCH_4);
            EStopStatus = OK;
         }
         else
         {
            CHECKPOINT(eStopDiagnostics, BRANCH_5);
            if (EStopStatus == INDETERMINATE)
            {
               CHECKPOINT(eStopDiagnostics, BRANCH_6);
               EStopStatus = OK;
            }
            else
               CHECKPOINT(eStopDiagnostics, BRANCH_7);
         }
      }
      else
      {
         // EStop1 == EStop2
         CHECKPOINT(eStopDiagnostics, BRANCH_3);
         EStopCount++;
         if (EStopCount > ESTOPCNT)
         {
            CHECKPOINT(eStopDiagnostics, BRANCH_8);
            EStopStatus = NOK;
         }
         else
         {
            /*
             * It takes 2 seconds to declare a fault
             * on Estop diagnostics if the two poles are read
             * same. Until then, the hazardous motions shall be disabled.
             */
            CHECKPOINT(eStopDiagnostics, BRANCH_9);
            EStopStatus = INDETERMINATE;
         }
      }
  
}

#if UNIT_TESTING

#include "Serial.h"
/**************************************************************************//**
* \brief -    	ResetHandler
*
* \brief -     ResetHandler is the entry for EStop unit testing.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void ResetHandler(void)
{
   tSINT32 n;
   DiagInit();
   eStopDiagnostics = 1;
   EStopCount = 0;

   /* --------------------------------------------------------------------------------------- */
   // Just the button diagnostics required
   // Test with EStop1 & EStop2 not pressed
   ESTOP1 = ESTOP1NOTPRESSEDCPU1;
   ESTOP2 = ESTOP2NOTPRESSEDCPU1;
   FaultReset = TRUE;
   EStopDiagnostics();
   DiagPrintInt(EStopStatus, OK);

   // ------------------------------------------
   // Test with EStop1 and EStop2 pressed. Test whether FaultReset clears EStopCount
   ESTOP1 = ESTOP1PRESSEDCPU1;
   ESTOP2 = ESTOP2PRESSEDCPU1;
   EStopCount = ESTOPCNT+1;
   FaultReset = FALSE;
   EStopDiagnostics();
   DiagPrintInt(EStopStatus, OK);

   // ------------------------------------------
   // Test with EStop1 and EStop2 not pressed. Test the lone path remaining with FaultReset clear
   ESTOP1 = ESTOP1NOTPRESSEDCPU1;
   ESTOP2 = ESTOP2NOTPRESSEDCPU1;
   EStopCount = ESTOPCNT+1;
   FaultReset = FALSE;
   EStopStatus = INDETERMINATE;
   EStopDiagnostics();
   DiagPrintInt(EStopStatus, OK);

   // ------------------------------------------
   // Test with EStop1 pressed and EStop2 not pressed
   ESTOP1 = ESTOP1PRESSEDCPU1;
   ESTOP2 = ESTOP2NOTPRESSEDCPU1;
   EStopCount = ESTOPCNT - 2;
   FaultReset = FALSE;
   // Should pass first ESTOPCNT times, then fail and exit
   for (n = 0; n < 2; n++)
   {
      EStopDiagnostics();
      DiagPrintInt(EStopStatus, INDETERMINATE);
   }
   EStopDiagnostics();
   DiagPrintInt(EStopStatus, NOK);

   // ------------------------------------------
   // Test with EStop1 & EStop2 pressed
   ESTOP1 = ESTOP1PRESSEDCPU1;
   ESTOP2 = ESTOP2PRESSEDCPU1;
   FaultReset = TRUE;
   EStopDiagnostics();
   DiagPrintInt(EStopStatus, OK);

   // ------------------------------------------
   // Test with EStop1 not pressed and EStop2 pressed
   ESTOP1 = ESTOP1NOTPRESSEDCPU1;
   ESTOP2 = ESTOP2PRESSEDCPU1;
   EStopCount = ESTOPCNT - 2;
   FaultReset = FALSE;
   // Should pass first ESTOPCNT times, then fail and exit
   for (n = 0; n < 2; n++)
   {
      EStopDiagnostics();
      DiagPrintInt(EStopStatus, INDETERMINATE);
   }
   EStopDiagnostics();
   DiagPrintInt(EStopStatus, NOK);
//   EStopStatus = OK;
//   //EStop = TRUE;
//   EStopDiagnostics();
//   DiagPrintInt(EStopStatus, OK);

   // That's it for the EStop Buttons
   /* --------------------------------------------------------------------------------------- */
   DiagSummary();
}

#endif
