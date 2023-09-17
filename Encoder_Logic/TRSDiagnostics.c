// $Header:   D:/databases/VMdb/archives/EN13849/TRSDiagnostics.c_v   1.15   20 Jul 2011 16:59:34   ASharma6  $
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

This is the module for monitoring TRS.

Module name: TRSDiagnostics

Filename:    TRSDiagnostics.c

--------------------------- TECHNICAL NOTES -------------------------------

The code is for diagnose TRS switch.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/TRSDiagnostics.c_v  $
 * 
 *    Rev 1.15   20 Jul 2011 16:59:34   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.14   12 Jul 2011 18:35:24   ASharma6
 * Revision 19 with modifications in Part contact lost algorithm - Loss of force alongwith horn retraction.
 *
 *    Rev 1.13   07 Jul 2011 12:25:58   ASharma6
 * TRS Diagnostics for IW+ machine configuration is modified
 *
 *    Rev 1.12   05 Jul 2011 11:01:42   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.9   22 Jun 2011 18:11:54   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.4   01 Apr 2011 11:14:48   PDwivedi
 * Fixes after code review.
 *
 *    Rev 1.3   30 Mar 2011 10:57:20   ASharma6
 * Tabs coverted to 3 spaces
 *
 *    Rev 1.2   29 Mar 2011 19:21:06   ASharma6
 * Modifications done to comply to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:11:52   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "TRSDiagnostics.h"

#include "TestMacros.h"
#include "Global.h"
#include "Input.h"
#include "PartContact.h"

/*
 * Trigger switch got modified for version 18 and this is why.
 * When the horn comes up and the Upper Limit Switch becomes active,
 * then at the point in time, the Trigger Switch diagnostics should be
 * suspended for 200ms. But if a new cycle is initiated within 200ms,
 * then the diagnostics should resume again.
 */
/*-------------------------- Global Variables ------------------------------*/
UT_VARIABLE(tSINT32 TRSTest)
UT_VARIABLE(tSINT32 trsDiagnostics)

tUINT32 OldULSPosition;
tUINT32 NoTRSDiagCntr;
tUINT32 TRSErrCntr;

/*--------------------------------------------------------------------------*/

/**************************************************************************//**
* \brief -     Diagnose the TRS only in case of GSXI. If ULS is true than TRS can not be
* 				true.
*
* \param  -  none
* 
* \return - none
*
*****************************************************************************/
void TRSDiagnostics(void)
{
   FUNCTION1(TRSTest, "TRSDiagnostics()");
   CHECKPOINT(trsDiagnostics, BRANCH_1);

   if (MachineConfiguration == GSXI)
   {
      CHECKPOINT(trsDiagnostics, BRANCH_3);
      if (FaultReset == TRUE)
      {
         CHECKPOINT(trsDiagnostics, BRANCH_4);
         TRSStatus = OK;
      }
      else
      {
         CHECKPOINT(trsDiagnostics, BRANCH_5);
      }
      if (ULSPosition == HOMEPOSITION)
      {
         CHECKPOINT(trsDiagnostics, BRANCH_6);
         if (OldULSPosition == AWAYPOSITION)
         {
            CHECKPOINT(trsDiagnostics, BRANCH_7);
            NoTRSDiagCntr = NoTRSDiagThres;
         }
         else
         {
            CHECKPOINT(trsDiagnostics, BRANCH_8);
         }
         if (NoTRSDiagCntr > 0)
         {
            CHECKPOINT(trsDiagnostics, BRANCH_9);
            NoTRSDiagCntr--;
         }
         else
         {
            CHECKPOINT(trsDiagnostics, BRANCH_10);
            /* This is the diagnostics part, the TRS is not OK if the trigger switch is ON
             * while the horn is in home position
             */
            if (TRS == TRSON)
            {
               CHECKPOINT(trsDiagnostics, BRANCH_11);
               TRSErrCntr++;
               if (TRSErrCntr > TRSErrThres)
               {
                  CHECKPOINT(trsDiagnostics, BRANCH_12);
                  TRSStatus = NOK;
               }
               else
               {
                  CHECKPOINT(trsDiagnostics, BRANCH_13);
               }
            }
            else
            {
               CHECKPOINT(trsDiagnostics, BRANCH_14);
               TRSErrCntr = 0;
            }
         }
      }
      else
      {
         CHECKPOINT(trsDiagnostics, BRANCH_15);
      }
   }
   else
   {
      CHECKPOINT(trsDiagnostics, BRANCH_2);
      TRSStatus = OK;
   }
   OldULSPosition = ULSPosition;
}



#if UNIT_TESTING

#include "Serial.h"
/**************************************************************************//**
** \brief -    	ResetHandler
**
** \brief -     ResetHandler is the entry for TRS unit testing.
**
** \param - 	none
** 
** \return -   	none
**
*****************************************************************************/
void ResetHandler(void)
{
   tSINT32 n;
   DiagInit();
   TRSTest = 1;
   trsDiagnostics = 1;

    /*-------------Test 1-----------*/
   MachineConfiguration = GSXI;
   FaultReset = TRUE;
   ULSPosition = AWAYPOSITION;
   TRSDiagnostics();
   DiagPrintInt(TRSStatus, OK);

   /*-------------Test 2 Status will be same as previous-----------*/
   MachineConfiguration = GSXI;
   ULSPosition = AWAYPOSITION;
   FaultReset = FALSE;
   TRSDiagnostics();
   DiagPrintInt(TRSStatus, OK);

    /*-------------Test 3 Status will be same as previous-----------*/
   MachineConfiguration = GSXI;
   ULSPosition = HOMEPOSITION;
   OldULSPosition = AWAYPOSITION;
   TRSDiagnostics();
   DiagPrintInt(TRSStatus, OK);
   NoTRSDiagCntr = 1;
    /*-------------Test 4-----------*/
   OldULSPosition = HOMEPOSITION;
   TRSDiagnostics();
   DiagPrintInt(TRSStatus, OK);
   /*-------------Test 4-----------*/
   TRS = TRSOFF;
   TRSDiagnostics();
   DiagPrintInt(TRSStatus, OK);
    /*-------------Test 5,6,7-----------*/
   TRS = TRSON;
   TRSErrCntr = TRSErrThres - 3;
   for (n = 0; n < 3; n++)
   {
      TRSDiagnostics();
      DiagPrintInt(TRSStatus, OK);
   }
   /*-------------Test 8-----------*/
   TRSDiagnostics();
   DiagPrintInt(TRSStatus, NOK);
   /*-------------Test 9-----------*/
  MachineConfiguration = GSXE;
  ULSPosition = HOMEPOSITION;
  TRS = TRUE;
  TRSDiagnostics();
  DiagPrintInt(TRSStatus, OK);
  DiagSummary();
}

#endif
