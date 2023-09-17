// $Header:   D:/databases/VMdb/archives/EN13849/SBeamDiagnostics.c_v   1.13   20 Jul 2011 16:58:56   ASharma6  $
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

This is the module for SBeam diagnostics.

Module name: SBeamDiagnostics

Filename:    SBeamDiagnostics.c

--------------------------- TECHNICAL NOTES -------------------------------

This code Handles the SBeam diagnostics.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/SBeamDiagnostics.c_v  $
 * 
 *    Rev 1.13   20 Jul 2011 16:58:56   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.12   12 Jul 2011 18:34:58   ASharma6
 * Revision 19 with modifications in Part contact lost algorithm - Loss of force alongwith horn retraction.
 *
 *    Rev 1.11   05 Jul 2011 11:01:06   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.8   22 Jun 2011 18:11:12   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.4   01 Apr 2011 11:13:56   PDwivedi
 * Fixes after code review.
 *
 *    Rev 1.3   30 Mar 2011 10:57:10   ASharma6
 * Tabs coverted to 3 spaces
 *
 *    Rev 1.2   29 Mar 2011 19:21:10   ASharma6
 * Modifications done to comply to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:08:54   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "SBeamDiagnostics.h"
#include "TestMacros.h"
#include "Global.h"
#include "ADCTest.h"

UT_VARIABLE(tSINT32 sbeamTest)
UT_VARIABLE(tSINT32 sbeamDiagnostics)
UT_VARIABLE(tSINT32 SBeamFault1)
UT_VARIABLE(tSINT32 SBeamFault2)
UT_VARIABLE(tSINT32 SBeamFault3)
/*-------------------------- Global Variables ------------------------------*/


tSINT32 SBeamErrCount;
/*--------------------------------------------------------------------------*/

/*********************************************************************//**
* \brief -    	SBeamDiagnostics
*
* \brief -     Diagnostic the SBeam. SBeam voltage(CurrForce) should be in predefined limits.
*
* \param -		none
* 
* \return -   	none
*
****************************************************************************/
void SBeamDiagnostics(void)
{
   static tSINT32 OldADCChannel2Value;
   FUNCTION1(sbeamTest, "SBeamDiagnostics()");
   CHECKPOINT(sbeamDiagnostics, BRANCH_1);
//   FAULT(SBeamFault1,{CurrForce = ForceVal1;});
//   FAULT(SBeamFault2,{CurrForce = ForceVal2;});
//   FAULT(SBeamFault3,{CurrForce = ForceVal3;});
   if( (MachineConfiguration == GSXE) || (MachineConfiguration == GSXP))
   {
	   
      CHECKPOINT(sbeamDiagnostics, BRANCH_2);
      if(ADC0Value[2] >= MIN_FORCE && ADC0Value[2] <= MAX_FORCE)
      {
         CHECKPOINT(sbeamDiagnostics, BRANCH_3);
         SBeamErrCount = FaultClr;
         if (FaultReset == true)
         {
            CHECKPOINT(sbeamDiagnostics, BRANCH_5);
            SBeamStatus = OK;
         }
         else
         {
            CHECKPOINT(sbeamDiagnostics, BRANCH_4);
         }
      }
      else
      {
         CHECKPOINT(sbeamDiagnostics, BRANCH_6);
         ADC0Value[2] = OldADCChannel2Value;
         SBeamErrCount++;
         if (SBeamErrCount > MAXSBEAMERRCNT)
         {
            /*
             * S-Beam fault is raised after 150ms,
             * instead of 3ms that we had earlier.
             */
            CHECKPOINT(sbeamDiagnostics, BRANCH_7);
            SBeamStatus = NOK;
         }
         else
            CHECKPOINT(sbeamDiagnostics, BRANCH_8);
      }
   }
   else
   {

      CHECKPOINT(sbeamDiagnostics, BRANCH_9);
      SBeamStatus = OK;
   }
   OldADCChannel2Value = ADC0Value[2];
}

#if UNIT_TESTING

#include "Serial.h"
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for SBeamDiagnostics test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void)
{
   tSINT32 n;
   DiagInit();
   sbeamTest = 1;
   sbeamDiagnostics = 1;
   MachineConfiguration = GSXE;
   /*-------------Test 1-----------*/
   ADC0Value[2] = ForceVal1;
   FaultReset = 1;
   SBeamDiagnostics();
   DiagPrintInt(SBeamStatus, OK);

   /*-------------Test 2-----------*/
   ADC0Value[2] = ForceVal3;
   FaultReset = 0 ;
   SBeamDiagnostics();
   DiagPrintInt(SBeamStatus, OK);
   SBeamErrCount = MAXSBEAMERRCNT - 3;

/*-------------Test 3-5-----------*/
// Should pass first MAXSBEAMERRCNT times, then fail and exit
   FaultReset = 0;
   for (n = MAXSBEAMERRCNT - 3; n < MAXSBEAMERRCNT; n++)
   {
      ADC0Value[2] = ForceVal2;
      SBeamDiagnostics();
      DiagPrintInt(SBeamStatus, OK);
   }
   ADC0Value[2] = ForceVal2;
   SBeamDiagnostics();
   DiagPrintInt(SBeamStatus, NOK);

   /* -----------Test 6----------*/
   MachineConfiguration = GSXI;
   SBeamDiagnostics();
   DiagPrintInt(SBeamStatus, OK);
   DiagSummary();
}

#endif
