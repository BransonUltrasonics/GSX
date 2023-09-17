// $Header:   D:/databases/VMdb/archives/EN13849/Application Code/IntegrationStep10.c_v   1.14   09 Apr 2015 17:09:56   ewomack  $
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

This is the tenth integration step. The BIST function is included.
This eventually becomes the main executable.

Filename:    IntegrationStep10.c

--------------------------- TECHNICAL NOTES -------------------------------

 Integration of software modules happens in steps with integration of 1 white box
 tested module at a time.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/Application Code/IntegrationStep10.c_v  $
 * 
 *    Rev 1.14   09 Apr 2015 17:09:56   ewomack
 * Disable integration testing diagnostics and add watchdog test to initialization sequence.
 * 
 *    Rev 1.13   20 Sep 2011 17:23:56   ASharma6
 * CRC of the executable is printed out on hyperterminal when program boots up.
 * 
 *    Rev 1.12   24 Aug 2011 11:24:22   ASharma6
 * For IW+ only - if in part contact state, if palm buttons are still pressed then stay in that state and the horn position becomes the new reference for collapse until the hands are released.
 * 
 *    Rev 1.10   03 Aug 2011 10:32:24   ASharma6
 * For machine configuration C, the part contact lost algorithm shall declare lost 150ms after trigger goes away
 * 
 *    Rev 1.9   20 Jul 2011 16:57:20   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.8   05 Jul 2011 10:59:44   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:09:34   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient

-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "uut/TestMacros.h"
#include "Global.h"
#include "ADCTest.h"
#include "CrossMonitoring.h"
#include "core_cm3.h"
#include "DeviceDiagnostics.h"
#include "Diagnostics.h"
#include "EStopDiagnostics.h"
#include "inc/DriveDiagnostics.h"
#include "inc/SonicsEnableDiagnostics.h"
#include "Led.h"
#include "lpc_types.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_ssp.h"
#include "PartContact.h"
#include "ProgramCounterTest.h"
#include "QEI.h"
#include "RTC.h"
#include "SBeamDiagnostics.h"
#include "Serial.h"
#include "SSP.h"
#include "StateMachine.h"
#include "stdio.h"
#include "system_LPC17xx.h"
#include "TRSDiagnostics.h"
#include "ULSLEDiagnostics.h"
#include "Input.h"
#include "TwoHandOperation.h"
#include "SystemInitialization.h"
#include "SetOutputs.h"

volatile BOOL OneMsFlag = FALSE;

/**************************************************************************//**
* \brief -    IntegratedSysTickCallback
*
* \brief -     This the SystemTick(1Ms) interrupt Call back function.
*
* \param -  	none
* 
*  \return -   none
*
*****************************************************************************/
void IntegratedSysTickCallback(void)
{
   /* The OneMsFlag is set here, so that all the processing need not be done
	 * in interrupt context
	 */
   MsInterruptCounter++;
   OneMsFlag = TRUE;
}

/* This define is to be enabled only during Integration testing. Once done, it shall
 * be disabled. It can be re-enabled for debugging.
 */
//#define INTEGRATION_TESTING 1

#ifdef INTEGRATION_TESTING

tSINT32 OldULS;
tSINT32 OldTRSVal;
tSINT32 OldSVREQ;
tSINT32 OldEStop;
tSINT32 OldSV_FAIL;
tSINT32 OldBothHandStatus;

tSINT32 OldEStopStatus = NOK;
tSINT32 OldTRSStatus = NOK;
tSINT32 OldUFAIL24Status = NOK;
tSINT32 OldLEStatus = NOK;
tSINT32 OldSBeamStatus = NOK;
tSINT32 OldSVRelayStatus = NOK;
tSINT32 OldPBStatus = NOK;
tUINT32 OldState;

#endif

/**************************************************************************//**
* \brief -    En13849
*
* \brief -		En13849 is the Main controller Loop Function.
*
* \param -		none
* 
* \return -		none.
*
*****************************************************************************/
void En13849(void)
{
   static unsigned long counter;
   //TestWD();
   SystemInitialization(); //Initialize the Whole System including CPU and Peripherals.

   SysTick_Callback = IntegratedSysTickCallback; // System Tick Call Back function.
   RTC_Callback = TimerCallback;
   if (SysTick_Config(SystemCoreClock / 1000))//Configure System Tick
      while (1)
            ; // Capture error
   if(WDStatus==NOK)
      {
   	   while(WDCount < WDTIMEOUT)
   	   {
   		   if(OneMsFlag)
   		   {
   			   WDCount++;
   			   OneMsFlag = FALSE;
   		   }
   	   }
   	   printf("\nWatch Dog Test Not Passed\n");
   	   while(true);
      }
      else
      {
   	   printf("\nWatch Dog Test Passed\n");
      }
   
   InitInputs();
   PrintfEnabled = TRUE;
   printf("\f\n\nSoftware Version %d\n\n",SWREVISION);
   // Comment the line below to keep the printfs enabled
   PrintfEnabled = FALSE;
   if (LogicUnitTest()) // Do the powerON Logic Unit Test.
   {
		printf("L U Test passed\n");
   }
	else
	{
		printf("L U Test not passed\n");
		//Logic Unit Test failed. Nothing to do. Wait with Beeping.
		while(1)
		{
		   /* If LU test fails, then don't go forward */
			if (OneMsFlag)
			{
				OneMsFlag = false;
				SV1 = OFF;
				SONICS_EN = OFF;
				BEEPER = OFF;
				PB_OUT = ON;
				SetOutputs();
			}
		}
	}
   PrintfEnabled = TRUE;
   printf("CRC = 0x%lx\n", StoredFlashCRC);
   PrintfEnabled = FALSE;
   ChangeState(POWERUP);
   counter = 0;
   while (counter < 500)
   {
      if (OneMsFlag)
      {
         /* Settling time for the sensors */
         OneMsFlag = FALSE;
         counter++;
         GetInputs();
         SetOutputs();
      }
   }
   do
	{
		if (OneMsFlag) // One Ms flag
		{
		   /* This loop runs infinitely every milli-second */
			OneMsFlag = FALSE;
			GetInputs();
			TwoHandOperation();
			DeviceDiagnostics();
			CrossMonitoring();
			PartContact();
			StateMachine();
			SetOutputs();
			FaultFlag = FALSE;
			if(BIST() && InterruptTestResult == TRUE)
			{
				//printf(" *");
			}
			else
			{
				if(InterruptTestResult == FALSE)
					printf("Interrupt Test ");
				PrintfEnabled = TRUE;
				printf("BIST failed \n");
				PrintfEnabled = FALSE;
				FaultFlag = LUFaultFlag;
				ALARM = FirstBitFoundPos(LUFaultFlag);
				SV1 = OFF;
				BEEPER = BEEPING;
				PB_OUT = ON;
				SetOutputs();
			}
#ifdef INTEGRATION_TESTING

         /* The following code is to test the inputs, this shall not
          * be part of the build of the final integrated code */
         if (FirstTime)
         {
            FirstTime = FALSE;
            if (EStopStatus == OK)
               printf("EStop Button is OK\n");
            else
               printf("EStop Button is NOK\n");

            if (TRSStatus == OK)
               printf("TRS is OK\n");
            else
               printf("TRS is NOK\n");

            if (UFAIL24Status == OK)
               printf("UFAIL24Status is OK\n");
            else
               printf("UFAIL24Status is NOK\n");

            if (LEStatus == OK)
               printf("LEStatus is OK\n");
            else
               printf("LEStatus is NOK\n");

            if (SVRelayStatus == OK)
               printf("SVRelayStatus is OK\n");
            else
               printf("SVRelayStatus is NOK\n");

            if (SBeamStatus == OK)
               printf("SBeamStatus is OK\n");
            else
               printf("SBeamStatus is NOK\n");

            if (PBStatus == OK)
               printf("PBStatus is OK\n");
            else
               printf("PBStatus is NOK\n");
            if (TRS == TRSON)
               printf("TRS is ON\n");
            else
               printf("TRS is OFF\n");
            OldTRSVal = TRSON;

       }

         if(OldState != State)
         {
            switch (State)
            {
               case PREREADY:
                  printf("State PREREADY\n");
               break;
               case READY:
                  printf("State READY\n");
               break;
               case FAULT:
                  printf("State FAULT\n");
               break;
               case FAULTRESET:
                  printf("State FAULTRESET\n");
               break;
               case SVON:
                  printf("State SVON\n");
               break;
               case PCDETECTED:
                  printf("State PCDETECTED\n");
               break;
               default:
                  printf("State FAULT\n");
               break;
            }
            OldState= State;
         }
         if(OldEStopStatus != EStopStatus)
         {
            if (EStopStatus == NOK)
               printf("EStop Button is NOK\n");
            else
               printf("EStop Button is OK\n");
            OldEStopStatus = EStopStatus;
         }
         if(OldTRSStatus != TRSStatus)
         {
            if(TRSStatus == OK)
               printf("TRS is OK\n");
            else
               printf("TRS is NOK\n");
            OldTRSStatus = TRSStatus;
         }
         if(OldUFAIL24Status!= UFAIL24Status)
         {
            if(UFAIL24Status == OK)
               printf("UFAIL24Status is OK\n");
            else
               printf("UFAIL24Status is NOK\n");
            OldUFAIL24Status = UFAIL24Status;
         }
         if(OldLEStatus != LEStatus)
         {
            if(LEStatus == OK)
               printf("LEStatus is OK\n");
            else
               printf("LEStatus is NOK\n");
            OldLEStatus = LEStatus;
         }
         if(OldSVRelayStatus != SVRelayStatus)
         {
            if(SVRelayStatus == OK)
               printf("SVRelayStatus is OK\n");
            else
               printf("SVRelayStatus is NOK\n");
            OldSVRelayStatus = SVRelayStatus;
         }
         if(OldSBeamStatus != SBeamStatus)
         {
            if(SBeamStatus == OK)
               printf("SBeamStatus is OK\n");
            else
               printf("SBeamStatus is NOK\n");
            OldSBeamStatus = SBeamStatus;
         }
         if(OldPBStatus != PBStatus)
         {
            if(PBStatus == OK)
            {
               printf("PBStatus is OK\n");
            }
            else
            {
               printf("PBStatus is NOK\n");
            }
            OldPBStatus = PBStatus;
         }
         if(OldBothHandStatus != BothHandStatus)
         {
            if(BothHandStatus == BOTHHANDS_OFF)
               printf("2-HandStatus = BothHandsOFF\n");
            else if(BothHandStatus == SECONDHANDTOOLATE)
               printf("2-HandStatus = SECONDHANDTOOLATE\n");
            else if(BothHandStatus == ONEHAND_OFF)
               printf("2-HandStatus = ONEHAND_OFF\n");
            else
               printf("2-HandStatus = BothHands_ON\n");

            OldBothHandStatus = BothHandStatus;
         }

         if (OldTRSVal != TRS)
         {
            if (TRS == TRSON)
               printf("TRS is ON\n");
            else
               printf("TRS is OFF\n");
            OldTRSVal = TRS;
         }

         if (OldSVREQ != SVREQ)
         {
           if (SVREQ == ON)
              printf("SVREQ is ON\n");
           else
              printf("SVREQ is OFF\n");
           OldSVREQ = SVREQ;
         }
#endif
		}
   } while (TRUE);
}

/**************************************************************************//**
* \brief -    ResetHandler
*
* \brief -		This is invoked on power-up/coming out of a reset.
*				Inputs are tested in this step.
*
* \param -		none
* 
* \return -		none.
*
*****************************************************************************/
void ResetHandler(void)
{
	En13849();
	}
