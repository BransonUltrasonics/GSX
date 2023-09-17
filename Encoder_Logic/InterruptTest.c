// $Header:   D:/databases/VMdb/archives/EN13849/InterruptTest.c_v   1.9   09 Apr 2015 17:11:10   ewomack  $
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

This is the module for the Timer Interrupt Test.

Module name: InterruptTest

Filename:    InterruptTest.c

--------------------------- TECHNICAL NOTES -------------------------------

This code Test the system Tick(1ms) interrupt rate in 1sec against 1sec RTC interrupt.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/InterruptTest.c_v  $
 * 
 *    Rev 1.9   09 Apr 2015 17:11:10   ewomack
 * Compiler warning cleanup.
 * 
 *    Rev 1.8   20 Jul 2011 16:58:04   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.7   05 Jul 2011 11:00:18   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.4   22 Jun 2011 18:10:14   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 12:11:46   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:05:36   ygupta
 * Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/

#include "TestMacros.h"
#include "RTC.h"
#include "Diagnostics.h"
#include "Global.h"

#define ONESEC_MINCOUNT 998
#define ONESEC_MAXCOUNT 1002

/* counts 1ms timeTicks */
tUINT32 TimerIntCount = 0;

UT_VARIABLE(tSINT32 testInterruptTimer)
tSINT32 TestInterruptTimer(tSINT32 TimerIntCount);

/**************************************************************************//**
* \brief -		TestInterruptTimer
*
* \brief -		Test if System Tick count in one second is within the limit.
*
* \param -		TimerIntCount: Tick count in last second.
* 
* \return -		Result equals to TRUE if Success else FALSE.
*
*****************************************************************************/
tSINT32 TestInterruptTimer(tSINT32 TimerIntCount)
{
#define SKIPRUNCOUNT 2 // 3 sec Time taken by CPU to settle down the RTC clock
   static tSINT32 ErrorCount = 0;

   FUNCTION1(testInterruptTimer, "TestInterruptTimer()");
   CHECKPOINT(testInterruptTimer, BRANCH_1);
   if (TimerIntCount >= ONESEC_MINCOUNT && TimerIntCount <= ONESEC_MAXCOUNT)
   {
      CHECKPOINT(testInterruptTimer, BRANCH_2);
      TimerIntCount = 0;
      ErrorCount = 0;
      InterruptTestResult = TRUE;
   }
   else
   {
      ErrorCount++;
      CHECKPOINT(testInterruptTimer, BRANCH_3);
      TimerIntCount = 0;
      if (ErrorCount > 3)
      {
          CHECKPOINT(testInterruptTimer, BRANCH_4);
          InterruptTestResult = FALSE;
      }
      else
      {
          CHECKPOINT(testInterruptTimer, BRANCH_5);
      }
   }
   return InterruptTestResult;
}
/**************************************************************************//**
* \brief -		TimerCallback
*
* \brief -		RTC Interrupt call back function. called every second.
*
 \param -		none
* 
* \return -	none
*
*****************************************************************************/
void TimerCallback(void)
{
    TestInterruptTimer(MsInterruptCounter);
}

/**************************************************************************//**
* \brief -		SysTickCallback
*
* \brief -		System Tick Interrupt callback function.Called every MilliSecond.
*
* \param -		none
* 
* \return -		none
*
*****************************************************************************/
void SysTickCallback(void)
{
    TimerIntCount++;
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for InterruptTest test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void)
{
    tSINT32 Result = 0;
    DiagInit();
    RTC_Callback = TimerCallback;
    SysTick_Callback = SysTickCallback;
    if (SysTick_Config(SystemCoreClock / 1000))
        while (1)
            ; // Capture error
    RTC_Init(LPC_RTC);
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
    RTC_SetTime(LPC_RTC, RTC_TIMETYPE_SECOND, 0);

    TimerIntCount = 999;
    Result = TestInterruptTimer(TimerIntCount);
    TimerIntCount = 999;
    Result = TestInterruptTimer(TimerIntCount);
    testInterruptTimer = 1;

    /*----------1---------*/
    TimerIntCount = 999;
    Result = TestInterruptTimer(TimerIntCount);
    DiagPrint(Result, 1);
    /*----------2---------*/
    TimerIntCount = 996;
    Result = TestInterruptTimer(TimerIntCount);
    DiagPrint(Result, 1);
    /*----------3---------*/
    TimerIntCount = 999;
    Result = TestInterruptTimer(TimerIntCount);
    DiagPrint(Result, 1);
    /*----------4---------*/
    TimerIntCount = 995;
    Result = TestInterruptTimer(TimerIntCount);
    DiagPrint(Result, 1);
    /*----------5---------*/
    TimerIntCount = 996;
    Result = TestInterruptTimer(TimerIntCount);
    DiagPrint(Result, 1);
    /*----------6---------*/
    TimerIntCount = 1001;
    Result = TestInterruptTimer(TimerIntCount);
    DiagPrint(Result, 1);
    /*----------7---------*/
    TimerIntCount = 1004;
    Result = TestInterruptTimer(TimerIntCount);
    DiagPrint(Result, 1);
    /*----------8---------*/
    TimerIntCount = 1005;
    Result = TestInterruptTimer(TimerIntCount);
    DiagPrint(Result, 1);
    /*----------9---------*/
    TimerIntCount = 1007;
    Result = TestInterruptTimer(TimerIntCount);
    DiagPrint(Result, 1);
    /*----------10--------*/
    TimerIntCount = 1004;
    Result = TestInterruptTimer(TimerIntCount);
    DiagPrint(Result, 0);
    DiagSummary();
    testInterruptTimer = 0;

    for (;;) {
    }
}

#endif

