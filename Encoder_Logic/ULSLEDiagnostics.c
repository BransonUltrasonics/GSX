// $Header:   D:/databases/VMdb/archives/EN13849/ULSLEDiagnostics.c_v   1.17   09 Apr 2015 17:15:42   ewomack  $
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 2010-11              */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------

This is the module for the ULS and linear Encoder monitoring.

Module name: ULSLEDiagnostics

Filename:    ULSLEDiagnostics.c

--------------------------- TECHNICAL NOTES -------------------------------

This code is for diagnostic related to ULS and Linear Encoder.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/ULSLEDiagnostics.c_v  $
 * 
 *    Rev 1.17   09 Apr 2015 17:15:42   ewomack
 * Only zero linear encoder when SV is OFF and ULS is HOME.
 * 
 *    Rev 1.16   02 Jan 2015 14:55:50   ewomack
 * Unit Test fixes
 * 
 *    Rev 1.15   20 Jul 2011 16:59:48   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.14   12 Jul 2011 18:35:30   ASharma6
 * Revision 19 with modifications in Part contact lost algorithm - Loss of force alongwith horn retraction.
 *
 *    Rev 1.13   05 Jul 2011 11:01:52   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.10   22 Jun 2011 18:12:06   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.7   15 Jun 2011 17:46:36   ygupta
 * EStop and LE diagnostics fix
 *
 *    Rev 1.6   08 Jun 2011 07:53:58   pdwivedi
 * Added changes for phase 8.
 *
 *    Rev 1.5   10 May 2011 07:32:44   pdwivedi
 * Added ULS input ,removed LED and integration changes.
 *
 *    Rev 1.4   01 Apr 2011 11:15:24   PDwivedi
 * Fixes after code review.
 *
 *    Rev 1.3   30 Mar 2011 10:57:14   ASharma6
 * Tabs coverted to 3 spaces
 *
 *    Rev 1.2   29 Mar 2011 19:21:00   ASharma6
 * Modifications done to comply to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:12:40   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "ULSLEDiagnostics.h"
#include "Global.h"
#include "Input.h"
#include "TestMacros.h"
#include "QEI.h"
#include "StateMachine.h"

/*-------------------------- Global Variables ------------------------------*/
UT_VARIABLE(tSINT32 ULSTest)
UT_VARIABLE(tSINT32 ulsleDiagnostics)
/*--------------------------------------------------------------------------*/
tUINT32 LEErrCnt = 0;
/**************************************************************************//**
* \brief -    	ULSLEDiagnostics
*
* \brief -     	Diagnostic the LE. If ULS is at home position then reset
*          		the LE position. If ULS is not at home and LE position
*            	is zero than this is a fault condition.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void ULSLEDiagnostics(void)
{
   FUNCTION1(ULSTest, "ULSLEDiagnostics()");
   CHECKPOINT(ulsleDiagnostics, BRANCH_1);
   /* When the horn is in home position or coming to home position,
    * reset QEI. The ULS declares home for horn even when it has
    * 1/8th of an inch further to go up. The ULS is not required for GSXS system.
    */
   if(MachineConfiguration == GSXE)
   {
	   CHECKPOINT(ulsleDiagnostics, BRANCH_2);
	   
	   if (FaultReset == TRUE)
	     {
	        CHECKPOINT(ulsleDiagnostics, BRANCH_4);
	        LEStatus = OK;
	     }
	   else
	   {
		   CHECKPOINT(ulsleDiagnostics, BRANCH_5);
	   }
	   
	   return; //TEMPORARY
   }
   else
   {
	   CHECKPOINT(ulsleDiagnostics, BRANCH_3);
	   if (ULSPosition == HOMEPOSITION/*(State==READY)*/ && (SV1_REQ == OFF ))
	   {
		   CHECKPOINT(ulsleDiagnostics, BRANCH_6);
		   QEIPosition = LE_ZEROPOSITION;
		   QEI_ResetPosition();
		   LEErrCnt = 0;
	   }
	   else
	   {
		   /* Horn is not in home position, check if QEI still says 0 (means am home) */
		   CHECKPOINT(ulsleDiagnostics, BRANCH_7);
		   if (QEIPosition == LE_ZEROPOSITION)
		   {
			   /* Threshold built to allow few zero readings */
			   CHECKPOINT(ulsleDiagnostics, BRANCH_9);
			   LEErrCnt++;
			   if (LEErrCnt > LE_ERRTHRES)
			   {
				   CHECKPOINT(ulsleDiagnostics, BRANCH_10);
				   LEStatus = NOK;
			   }
			   else
			   {
				   CHECKPOINT(ulsleDiagnostics, BRANCH_11);
			   }
		   }
		   else
		   {
			   /* Horn is not home, QEI says not at home - both are in sync */
			   CHECKPOINT(ulsleDiagnostics, BRANCH_8);
			   LEErrCnt = 0;
		   }
	   }
   }
   
   
   if (FaultReset == TRUE)
   {
      /* Fault reset is allowed in home position also. Linear encoder fault
       * is not raised for 30ms, instead of 5 ms earlier due to the fact
       * that the Upper Limit Switch goes through a debouncing logic which
       * delays the output by 10ms and it was giving us some wrong faults.
       */
      CHECKPOINT(ulsleDiagnostics, BRANCH_12);
      LEStatus = OK;
   }
   else
   {
      CHECKPOINT(ulsleDiagnostics, BRANCH_13);
   }
}

#if UNIT_TESTING

#include "Serial.h"
/**************************************************************************//**
* \brief -    	ResetHandler
*
* \brief -     ResetHandler is the entry for ULS and LE unit testing.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void ResetHandler(void)
{
   DiagInit();
   QuadratureEncoderInit(MAXQEIPOS);
   ULSTest = 1;
   FaultReset = TRUE;
   ulsleDiagnostics = 1;
   LEStatus = OK;
   SV1 = OFF;

   /*-------------Test 1-----------*/
   ULSPosition = HOMEPOSITION;
   FaultReset = TRUE;
   ULSLEDiagnostics();
   DiagPrintInt(LEStatus, OK);

   /*-------------Test 2--------------------------*/
   ULSPosition = HOMEPOSITION;
   FaultReset = FALSE;
   QEIPosition = LE_ZEROPOSITION + 1;
   ULSLEDiagnostics();
   DiagPrintInt(LEStatus, OK);

   /*-------------Test 3--------------------------*/
   ULSPosition = AWAYPOSITION;
   FaultReset = TRUE;
   QEIPosition = LE_ZEROPOSITION;
   LEErrCnt = LE_ERRTHRES - 1;
   ULSLEDiagnostics();
   DiagPrintInt(LEStatus, OK);

   /*--------------Test 4--------------------------*/
   LEErrCnt = LE_ERRTHRES + 1;
   FaultReset = FALSE;
   ULSLEDiagnostics();
   DiagPrintInt(LEStatus, NOK);

   //Test 5
   QEIPosition = LE_ZEROPOSITION + 1;
   FaultReset = FALSE;
   ULSLEDiagnostics();
   DiagPrintInt(LEStatus, NOK);

   //Test 6
   FaultReset = TRUE;
   ULSLEDiagnostics();
   DiagPrintInt(LEStatus, OK);

   DiagSummary();
}

#endif
