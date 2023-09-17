// $Header:   D:/databases/VMdb/archives/EN13849/Application Code/IntegrationStep9.c_v   1.6   20 Jul 2011 16:57:58   ASharma6  $
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

This is the ninth integration step. The PartContact module is included.

Filename:    IntegrationStep9.c

--------------------------- TECHNICAL NOTES -------------------------------

 Integration of software modules happens in steps with integration of 1 white box
 tested module at a time.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/Application Code/IntegrationStep9.c_v  $
 * 
 *    Rev 1.6   20 Jul 2011 16:57:58   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.5   05 Jul 2011 11:00:14   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.2   22 Jun 2011 18:10:08   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient

-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "TestMacros.h"
#include "Global.h"
#include "ADCTest.h"
#include "CrossMonitoring.h"
#include "core_cm3.h"
#include "DeviceDiagnostics.h"
#include "Diagnostics.h"
#include "EStopDiagnostics.h"
#include "inc/DriveDiagnostics.h"
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
   OneMsFlag = TRUE;
}


/* This define is to be enabled only during Integration testing. Once done, it shall
 * be disabled. It can be re-enabled for debugging.
 */
#define INTEGRATION_TESTING 1

#ifdef INTEGRATION_TESTING

tSINT32 OldULS;
tSINT32 OldTRS ;
tSINT32 OldSVREQ;
tSINT32 OldESTOP1;
tSINT32 OldESTOP2;
tSINT32 OldEStop;
tSINT32 OldPB1_IN;
tSINT32 OldPB2_IN;
tUINT32 OldADC0Value;
tSINT32 OldQEIPosition;
tSINT32 OldUFAIL24V;
tSINT32 OldU2RESET;
tSINT32 OldSV_FAIL;
tSINT32 OldBothHandStatus;

tSINT32 OldEStopStatus = NOK;
tSINT32 OldTRSStatus = NOK;
tSINT32 OldUFAIL24Status = NOK;
tSINT32 OldLEStatus = NOK;
tSINT32 OldSBeamStatus = NOK;
tSINT32 OldSVRelayStatus = NOK;
tSINT32 OldPBStatus = NOK;
tSINT32 OldSV = ON;
tSINT32 OldPB_OUT  = PB_ENABLED;
tSINT32 OldPL = ON;

tUINT8 OldState;

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
   //TestWD();
   SystemInitialization(); //Initialize the Whole System including CPU and Peripherals.
   PrintfEnabled = TRUE;
  // printf("\nWatch Dog Test Passed\n");
  
  // InitInputs();
//   FaultReset = TRUE;
   SysTick_Callback = IntegratedSysTickCallback; // System Tick Call Back function.
   if (SysTick_Config(SystemCoreClock / 1000))//Configure System Tick
      while (1)
            ; // Capture error
   	  printf("\n\nSystem has been initialized.\n");
      printf("Software Version %d\n\n",SWREVISION);
      printf("\n\nIntegration Step 9.\n");
                    
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
   if(LogicUnitTest()) // Do the powerON Logic Unit Test.
		printf("L U Test passed\n");
	else
	{
		printf("L U Test not passed\n");
		//Logic Unit Test failed. Nothing to do. Wait with flashing PL.
		while(1)
		{
			if(OneMsFlag)
			{
				OneMsFlag = false;
				SV1 = OFF;
				BEEPER = BEEPING;
				PB_OUT = ON;
				SetOutputs();
			}
		}
	}
   ChangeState(POWERUP);
   do
	{
		if(OneMsFlag) // One Ms flag
		{
			OneMsFlag = FALSE;
			GetInputs();
			TwoHandOperation();
			DeviceDiagnostics();
//#ifdef INTEGRATION_TESTING
	//		FaultFlag = FALSE;
//#endif
			CrossMonitoring();
			PartContact();
			StateMachine();
			SetOutputs();
			FaultFlag = FALSE;

#ifdef INTEGRATION_TESTING

         /* The following code is to test the inputs, this shall not
          * be part of the build of the final integrated code */
			//For testing State machine
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
					break;
					case PCDETECTED:
						printf("State PCDETECTED\n");
					break;
					default:
						printf("\nState FAULT");
					break;
				}
				OldState= State;
			}
#endif
		}
   } while (TRUE);
}
