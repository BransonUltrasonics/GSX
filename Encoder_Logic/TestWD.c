// $Header:   D:/databases/VMdb/archives/EN13849/TestWD.c_v   1.15   09 Apr 2015 17:14:52   ewomack  $
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

This is the module for reseting and testing Watch Dog.

Module name: TestWD

Filename:    TestWD.c

--------------------------- TECHNICAL NOTES -------------------------------

The code is for reseting WD by writing inverted value each time.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/TestWD.c_v  $
 * 
 *    Rev 1.15   09 Apr 2015 17:14:52   ewomack
 * Compiler warning cleanup.
 * 
 *    Rev 1.14   02 Jan 2015 14:54:46   ewomack
 * Unit Test fixes
 * 
 *    Rev 1.13   25 Jul 2011 15:25:56   ASharma6
 * Modifications in WatchDog module - At power up, the voltage monitor resets the CPU first. On coming from a reset, the CPU should wait indefinitely for a WatchDog to reset it.
 * 
 *    Rev 1.12   20 Jul 2011 16:59:30   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.11   12 Jul 2011 18:35:20   ASharma6
 * Revision 19 with modifications in Part contact lost algorithm - Loss of force alongwith horn retraction.
 * 
 *    Rev 1.10   05 Jul 2011 11:01:38   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.7   22 Jun 2011 18:11:50   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 12:14:30   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:11:34   ygupta
 * Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/

#include "TestMacros.h"
#include "DeviceDiagnostics.h"
#include "Global.h"

tSINT32 WDStatus;

UT_VARIABLE(tSINT32 TestWDDiag)
UT_VARIABLE(tSINT32 WDFault)
UT_VARIABLE(tSINT32 WDFault2)
UT_VARIABLE(tSINT32 WDFault3)
UT_VARIABLE(tSINT32 WDFault4)
/**************************************************************************//**
* \brief -  	TestWD
*
* \brief -  	Test the value of RSID register and see if CPU was not
*      			reset by WD than sit in a while loop so that WD will
*          		reset the CPU and next time it will get the CPU reset by WD.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void TestWD(void)
{
   printf("\f");
   FUNCTION1(TestWDDiag, "TestWD()");
   CHECKPOINT(TestWDDiag, BRANCH_1);
   LPC_PINCON->PINSEL1 = LPC_PINCON->PINSEL1 & FIOPINSEL_P18FUN0_MASK; //P1:18 U_WDO
   //Data direction setting for WatchDog outputs
   LPC_GPIO1->FIODIR   = LPC_GPIO1->FIODIR |  FIODIR_D18_MASK;
#ifndef PROTOTYPEBOARD
   PINSEL_SetOutput(PINSEL_PORT_0, PINSEL_PIN_11, 0);
#endif
   if ((LPC_SC->RSID & RSID_EXTR) && ((LPC_SC->RSID & RSID_POR) == 0))
   {
      CHECKPOINT(TestWDDiag, BRANCH_2);
      WDStatus =OK;
      printf("WD is OK\n");
   }
   else
   {
      CHECKPOINT(TestWDDiag, BRANCH_3);
      LPC_RTC->GPREG0 = 1;
      LPC_SC->RSID = 0x000F;
      WDStatus = NOK;
      printf("WD is NOK\n");
      printf("\nhere it is:%08lX\n",LPC_SC->RSID);
      //while (1);
   }
}

#if UNIT_TESTING
/**************************************************************************//**
** \brief -		ResetHandler
**
** \brief -		ResetHandler is the entry point for WatchDog Diag test unit testing.
**
** \param -		none
** 
** \return -	none
**
*****************************************************************************/
void ResetHandler(void)
{
   DiagInit();
   TestWDDiag = 1;
   WDFault = 0;
   WDFault2 = 0;
   WDFault3 = 1;
   WDFault4 = 1;
   TestWD();
   if(WDStatus == NOK)
   {
	   DiagPrintInt(WDStatus, NOK);
	   DiagSummary();
	   while(true);
   }
   
   DiagPrintInt(WDStatus, OK);
//   WDFault = 0;
//   WDFault2 = 1;
//   WDFault3 = 0;
//   WDFault4 = 1;
//   TestWD();
//   DiagPrintInt(WDStatus, OK);
//
//   WDFault = 0;
//   WDFault2 = 1;
//   WDFault3 = 1;
//   WDFault4 = 0;
//   TestWD();
//   DiagPrintInt(WDStatus, OK);
//   WDFault = 1;
//   TestWD();
//   DiagPrintInt(WDStatus, NOK);
   DiagSummary();
   while (1)
      ResetExternalWatchdog();
}

#endif
