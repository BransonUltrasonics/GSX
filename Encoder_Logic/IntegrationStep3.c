// $Header:   D:/databases/VMdb/archives/EN13849/Application Code/IntegrationStep3.c_v   1.6   20 Jul 2011 16:57:34   ASharma6  $
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

This is the third integration step. The Device Diagnostics module
is integrated with rest of the modules.

Filename:    IntegrationStep3.c

--------------------------- TECHNICAL NOTES -------------------------------

 Integration of software modules happens in steps with integration of 1 white box
 tested module at a time.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/Application Code/IntegrationStep3.c_v  $
 * 
 *    Rev 1.6   20 Jul 2011 16:57:34   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.5   05 Jul 2011 10:59:52   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.2   22 Jun 2011 18:09:42   ASharma6
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
#include "SystemInitialization.h"
#include "Input.h"
#include "TwoHandOperation.h"

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
tSINT32 OldDriveStatus = NOK;
tSINT32 OldSonicsEnStatus = NOK;
tSINT32 OldPBStatus = NOK;

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
   SystemInitialization(); //Initialize the Whole System including CPU and Peripherals.
   static int FirstTime = TRUE;
   PrintfEnabled = TRUE;
   
   InitInputs();
#ifdef INTEGRATION_TESTING
   FaultReset = TRUE;
#endif
   SysTick_Callback = IntegratedSysTickCallback; // System Tick Call Back function.
   if (SysTick_Config(SystemCoreClock / 1000))//Configure System Tick
      while (1)
            ; // Capture error
   printf("\n\nSystem has been initialized.\n");
   printf("Software Version %d\n\n",SWREVISION);
   printf("\n\nIntegration Step 3.\n");
   SV1 = ON;
   SONICS_EN = ON;
   PINSEL_SetOutput(PINSEL_PORT_1, PINSEL_PIN_27,SV1);  //SV1 to active solid state relay for DRIVE_OUT signal
   PINSEL_SetOutput(PINSEL_PORT_1,PINSEL_PIN_15,SONICS_EN);   //SONICS_EN to active solid state relay for SONICS_5V signal
   do
	{
		if(OneMsFlag) // One Ms flag
		{
			OneMsFlag = FALSE;
			ResetExternalWatchdog();
			GetInputs();
			TwoHandOperation();
			DeviceDiagnostics();
			FaultFlag = FALSE;
//			if((QEIPosition - QEIPositionOld) > MINDISTANCE)
//			{
//				LEStatus = NOK;
//			}
//			else if(QEIPosition < QEIPositionOld)
//			{
//				QEIPositionOld = QEIPosition;
//				LEStatus = OK;
//			}

			
#ifdef INTEGRATION_TESTING
         /* The following code is to test the integration , this shall not
          * be part of the build of the final integrated code */
         if (FirstTime)
         {

        	 if(IsThisCPU1)
        		 printf("CPU 1\n\n");
        	 else
        		 printf("CPU 2\n\n");
				
            FirstTime = FALSE;
            if (EStopStatus == OK)
               printf("EStop Button status - OK\n");
            else
               printf("EStop Button status - FAILED\n");
            OldEStopStatus = EStopStatus;

            if (TRSStatus == OK)
               printf("TRS (GSXI ONLY) status - OK\n");
            else
               printf("TRS (GSXI ONLY) status - FAILED\n");
            OldTRSStatus = TRSStatus;

            if (UFAIL24Status == OK)
               printf("UFAIL24Status status - OK\n");
            else
               printf("UFAIL24Status status - FAILED\n");
            OldUFAIL24Status = UFAIL24Status;

            if (DriveStatus == OK)
               printf("DriveStatus status - OK\n");
            else
               printf("DriveStatus status - FAILED\n");
            OldDriveStatus = DriveStatus;
            
            if(SonicsEnStatus == OK)
            	printf("SonicsEnStatus status - OK\n");
            else
            	printf("SonicsEnStatus status - FAILED\n");
            OldSonicsEnStatus = SonicsEnStatus;

            if (SBeamStatus == OK)
               printf("SBeamStatus status - OK\n");
            else
               printf("SBeamStatus status - FAILED\n");
            OldSBeamStatus = SBeamStatus;

            if (PBStatus == OK)
               printf("PBStatus status - OK\n\n");
            else
               printf("PBStatus status - FAILED\n\n");
            OldPBStatus = PBStatus;

       }
			if(OldEStopStatus != EStopStatus)
			{
				if(EStopStatus == OK)
					printf("EStop Button status - OK\n");
				else
					printf("EStop Button status - FAILED\n");
				OldEStopStatus = EStopStatus;
			}
			if(OldTRSStatus != TRSStatus)
			{
				if(TRSStatus == OK)
					printf("TRS (GSXI ONLY) status - OK\n");
				else
					printf("TRS (GSXI ONLY) status - FAILED\n");
				OldTRSStatus = TRSStatus;
			}
			if(OldUFAIL24Status!= UFAIL24Status)
			{
				if(UFAIL24Status == OK)
					printf("UFAIL24 status - OK\n");
				else
					printf("UFAIL24 status - FAILED\n");
				OldUFAIL24Status = UFAIL24Status;
			}

			if(OldDriveStatus != DriveStatus)
			{
				if(DriveStatus == OK)
					printf("DriveStatus status - OK\n");
				else
					printf("DriveStatus status - FAILED\n");
				OldDriveStatus = DriveStatus;
			}
			
			if(OldSonicsEnStatus != SonicsEnStatus)
			{
				if(SonicsEnStatus == OK)
					printf("SonicsEnStatus status - OK\n");
				else
					printf("SonicsEnStatus status - FAILED\n");
				OldSonicsEnStatus = SonicsEnStatus;
			}
			
			if(OldSBeamStatus != SBeamStatus)
			{
				if(SBeamStatus == OK)
					printf("SBeamStatus status - OK\n");
				else
					printf("SBeamStatus status - FAILED\n");
				OldSBeamStatus = SBeamStatus;
			}
			if(OldPBStatus != PBStatus)
			{
				if(PBStatus == OK)
					printf("PBStatus status - OK\n");
				else
					printf("PBStatus status - FAILED\n");
				OldPBStatus = PBStatus;
			}
#endif
		}
	} while (TRUE);

}
