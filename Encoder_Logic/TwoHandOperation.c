// $Header:   D:/databases/VMdb/archives/EN13849/TwoHandOperation.c_v   1.11   20 Jul 2011 16:59:40   ASharma6  $
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

This is the module for handling Two-Hand Operation.

Module name: TwoHandOperation

Filename:    TwoHandOperation.c

--------------------------- TECHNICAL NOTES -------------------------------

The code is for handling both PB and both should be pressed within 500Ms
for a successful operation.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/TwoHandOperation.c_v  $
 * 
 *    Rev 1.11   20 Jul 2011 16:59:40   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.10   05 Jul 2011 11:01:46   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.7   22 Jun 2011 18:11:58   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.4   01 Apr 2011 11:15:00   PDwivedi
 * Fixes after code review.
 *
 *    Rev 1.3   30 Mar 2011 10:57:18   ASharma6
 * Tabs coverted to 3 spaces
 *
 *    Rev 1.2   29 Mar 2011 19:21:04   ASharma6
 * Modifications done to comply to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:12:08   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "TestMacros.h"
#include "StateMachine.h"
#include "Global.h"


static volatile tSINT32 BothHandTimer = 0;
#define PBTIMEOUT 500

// For Testing
#define TESTTIMERVAL1  400
#define TESTTIMERVAL2  600

UT_VARIABLE(tSINT32 input)

/**************************************************************************//**
* \brief -    	TwoHandOperation
*
* \brief -     Handles the TwoHand Operation for Palm Buttons.Both PB
*          	should be pressed within 500Ms for a successful operation.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void  TwoHandOperation(void)
{
   FUNCTION1(input, "TwoHandOperation()");
   CHECKPOINT(input, BRANCH_1);

   if ((PB1_NO_IN == OFF) && (PB2_NO_IN == OFF)) // Both Off
   {
      CHECKPOINT(input, BRANCH_2);
      BothHandTimer = 0;
      BothHandStatus = BOTHHANDS_OFF;
      //printf("PB:OFF\n");
   }
   else
   {
      CHECKPOINT(input, BRANCH_3);
      if ((PB1_NO_IN == ON) && (PB2_NO_IN  == ON)) // Both On
      {
         CHECKPOINT(input, BRANCH_4);
         
         if (BothHandStatus != ONEHAND_OFF)
         {
            CHECKPOINT(input, BRANCH_9);
            if(BothHandTimer >= PBTIMEOUT)
            {
                CHECKPOINT(input, BRANCH_10);
                BothHandStatus = SECONDHANDTOOLATE;

            }
            else
            {
                CHECKPOINT(input, BRANCH_11);
                BothHandStatus = BOTHHANDS_ON;
               // printf("PB:2Hand\n");
            }
         }
         else
         {
        	 //printf("PB:1Hand\n");
            CHECKPOINT(input, BRANCH_12);
         }
      }
      else // One hand ON
      {
         CHECKPOINT(input, BRANCH_5);
         if (BothHandStatus == BOTHHANDS_ON)
         {
            // Both hands were ON, but one taken off
            CHECKPOINT(input, BRANCH_6);
            BothHandStatus = ONEHAND_OFF;
         }
         else if (BothHandStatus != ONEHAND_OFF)
         {
            // Second button hasn't been pressed yet
            CHECKPOINT(input, BRANCH_7);
            if(BothHandTimer < PBTIMEOUT)
            {
            	BothHandTimer++;
            }
           // BothHandStatus = BOTHHANDS_OFF;
         }
         else
         {
            // Now in one hand taken off stage
            CHECKPOINT(input, BRANCH_8);
         }
      }
   }
}


#if UNIT_TESTING
/**************************************************************************//**
* \brief -    	ResetHandler
*
* \brief -     ResetHandler is the entry for TwoHandOperation unit testing.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void ResetHandler(void)
{
   DiagInit();
   input = 1;

   // Test path 1,2
   PB1_NO_IN = OFF;
   PB2_NO_IN = OFF;
   BothHandTimer = PBTIMEOUT/2;
   TwoHandOperation();
   DiagPrintInt(BothHandStatus, BOTHHANDS_OFF);

   // Test path 1,3,4,9,11
   PB1_NO_IN = ON;
   PB2_NO_IN = ON;
   BothHandTimer = PBTIMEOUT/2;
   TwoHandOperation();
   DiagPrintInt(BothHandStatus, BOTHHANDS_ON);

   // Test path 1,3,4,9,10
   PB1_NO_IN = ON;
   PB2_NO_IN = ON;
   BothHandTimer = PBTIMEOUT*2;
   TwoHandOperation();
   DiagPrintInt(BothHandStatus, SECONDHANDTOOLATE);

   // Test path 1,3,4,12
   PB1_NO_IN = ON;
   PB2_NO_IN = ON;
   BothHandStatus = ONEHAND_OFF;
   BothHandTimer = PBTIMEOUT/2;
   TwoHandOperation();
   DiagPrintInt(BothHandStatus, ONEHAND_OFF);

   // Test path 1,3,5,6
   PB1_NO_IN = ON;
   PB2_NO_IN = OFF;
   BothHandStatus = BOTHHANDS_ON;
   BothHandTimer = PBTIMEOUT/2;
   TwoHandOperation();
   DiagPrintInt(BothHandStatus, ONEHAND_OFF);

   // Test path 1,3,5,8
   PB1_NO_IN = ON;
   PB2_NO_IN = OFF;
   BothHandTimer = PBTIMEOUT/2;
   TwoHandOperation();
   DiagPrintInt(BothHandStatus, ONEHAND_OFF);

   // Test path 1,3,5,7
   PB1_NO_IN = ON;
   PB2_NO_IN = OFF;
   BothHandStatus = BOTHHANDS_OFF;
   BothHandTimer = PBTIMEOUT/2;
   TwoHandOperation();
   DiagPrintInt(BothHandStatus, BOTHHANDS_OFF);

   DiagSummary();
}

#endif
