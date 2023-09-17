// $Header:   D:/databases/VMdb/archives/EN13849/UFail24VDiagnostics.c_v   1.10   20 Jul 2011 16:59:44   ASharma6  $
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

This is the module for UFAIL24V signal monitoring.

Module name: UFAIL24VDiagnostics

Filename:    UFAIL24VDiagnostics.c

--------------------------- TECHNICAL NOTES -------------------------------

This code is for diagnostic related to UFAIL24V .

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/UFail24VDiagnostics.c_v  $
 * 
 *    Rev 1.10   20 Jul 2011 16:59:44   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.9   05 Jul 2011 11:01:48   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.6   22 Jun 2011 18:12:02   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.4   01 Apr 2011 11:15:12   PDwivedi
 * Fixes after code review.
 *
 *    Rev 1.3   30 Mar 2011 10:57:16   ASharma6
 * Tabs coverted to 3 spaces
 *
 *    Rev 1.2   29 Mar 2011 19:21:02   ASharma6
 * Modifications done to comply to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:12:24   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "TestMacros.h"
#include "Global.h"
#include "DeviceDiagnostics.h"
#include "StateMachine.h"

UT_VARIABLE(tSINT32 UFAIL24VDiag)
UT_VARIABLE(tSINT32 UFAILFillOK)
UT_VARIABLE(tSINT32 UFAILFault)
UT_VARIABLE(tSINT32 FaultResetFillOK)
UT_VARIABLE(tSINT32 FaultResetFault)

/**************************************************************************//**
* \brief -    	UFAIL24VDiagnostics
*
* \brief -     Diagnose the 24Volt status.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void UFAIL24VDiagnostics(void)
{
   FUNCTION1(UFAIL24VDiag, "UFAIL24VDiagnostics()");
   CHECKPOINT(UFAIL24VDiag, BRANCH_1);
   FAULT(UFAILFillOK,{UFAIL24V = OFF;});
   FAULT(UFAILFault,{UFAIL24V = ON;});
   if (UFAIL24V == ON) //24V is OK
   {
      CHECKPOINT(UFAIL24VDiag, BRANCH_3);
      FAULT(FaultResetFillOK,{FaultReset = TRUE;});
      FAULT(FaultResetFault,{FaultReset = FALSE;});
      if(FaultReset == TRUE)
      {
         CHECKPOINT(UFAIL24VDiag, BRANCH_5);
         UFAIL24Status = OK;
      }
      else
      {
         CHECKPOINT(UFAIL24VDiag, BRANCH_4);
      }
   }
   else
   {
      CHECKPOINT(UFAIL24VDiag, BRANCH_2);
      UFAIL24Status  = NOK;
   }
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -   	ResetHandler
*
* \brief -     ResetHandler is the entry for UFAIL24 volt unit testing.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void ResetHandler(void)
{
   DiagInit();
   UFAIL24VDiag = 1;

   //UFAIL24VDiagnostics Test
   UFAILFillOK = 0;
   UFAILFault = 0;
   FaultResetFillOK = 0;
   FaultResetFault = 1;

   UFAIL24VDiagnostics();
   DiagPrintInt(UFAIL24Status, OK);

   UFAILFillOK = 0;
   UFAILFault = 0;
   FaultResetFillOK = 1;
   FaultResetFault = 0;

   UFAIL24VDiagnostics();
   DiagPrintInt(UFAIL24Status, OK);

   UFAILFillOK = 0;
   UFAILFault = 1;
   FaultResetFillOK = 0;
   FaultResetFault = 1;

   UFAIL24VDiagnostics();
   DiagPrintInt(UFAIL24Status, NOK);

   UFAIL24VDiag = 0;

   DiagSummary();
}

#endif
