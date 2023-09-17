
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
#define INTEGRATION_TESTING 1

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
   static unsigned long counter;
   
   SystemInitialization(); //Initialize the Whole System including CPU and Peripherals.

   SysTick_Callback = IntegratedSysTickCallback; // System Tick Call Back function.
   RTC_Callback = TimerCallback;
   if (SysTick_Config(SystemCoreClock / 1000))//Configure System Tick
      while (1)
            ; // Capture error
   PrintfEnabled = TRUE;
   printf("\n\nSystem has been initialized.\n");
   printf("Software Version %d\n\n",SWREVISION);
   printf("\n\nIntegration Step 10_CpuStuck.\n");            
	if(IsThisCPU1)
    	printf("CPU 1\n\n");
   	else
   		printf("CPU 2\n\n");
    	
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
   
   if (LogicUnitTest()) // Do the powerON Logic Unit Test.
   {
		printf("L U Test passed\n");
   }
	else
	{
		printf("L U Test not passed\n");
		//Logic Unit Test failed. Nothing to do. Wait with flashing PL.
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
 
   printf("CRC = 0x%lx\n", StoredFlashCRC);
   //PrintfEnabled = FALSE;
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
			
			if(OldState != State)
						{
							switch (State)
							{
								case POWERUP:
									printf("State POWERUP\n");
								break;
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
								case DRIVEON:
									printf("State DRIVEON\n");
									if(IsThisCPU1)
									{
										printf("Suspending processing in state DRIVEON\n");
										while(TRUE) //Infinite loop to simulate CPU1 stucks in this state when CPU2 is normal operation
										{
											ResetExternalWatchdog(); 
										}
									}
								break;
								case PCDETECTED:
									printf("State PCDETECTED\n");
									if(!IsThisCPU1)
									{
										printf("Suspending processing in state PCDETECTED\n");
										while(TRUE) //Infinite loop to simulate CPU2 stucks in this state when CPU1 is normal operation
										{
											ResetExternalWatchdog();
										}
									}
								break;
								default:
									printf("State FAULT\n");
								break;
							}
							OldState= State;
						}
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

		}
   } while (TRUE);
}


