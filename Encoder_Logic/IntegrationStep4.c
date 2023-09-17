// $Header:   D:/databases/VMdb/archives/EN13849/Application Code/IntegrationStep4.c_v   1.7   09 Apr 2015 17:10:16   ewomack  $
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

This is the fourth integration step. The SetOutputs module
is integrated with rest of the modules.

Filename:    IntegrationStep4.c

--------------------------- TECHNICAL NOTES -------------------------------

 Integration of software modules happens in steps with integration of 1 white box
 tested module at a time.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/Application Code/IntegrationStep4.c_v  $
 * 
 *    Rev 1.7   09 Apr 2015 17:10:16   ewomack
 * Compiler warning cleanup.
 * 
 *    Rev 1.6   20 Jul 2011 16:57:38   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.5   05 Jul 2011 10:59:56   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.2   22 Jun 2011 18:09:46   ASharma6
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
tSINT32 OldTRS;
tSINT32 OldSV1_REQ;
tSINT32 OldESTOP1;
tSINT32 OldESTOP2;
tSINT32 OldEStop;
tSINT32 OldPB1_NO_IN;
tSINT32 OldPB2_NO_IN;
tUINT32 OldADC0Value;
tSINT32 OldQEIPosition;
tSINT32 OldUFAIL24V;
tSINT32 OldU2RESET;
tSINT32 OldDRIVE_HI_FAIL;
tSINT32 OldDRIVE_LOW_FAIL;
tSINT32 OldSV2_HI_FAIL;
tSINT32 OldSV2_LOW_FAIL;
tSINT32 OldSONICS_HI_FAIL;
tSINT32 OldSONICS_LOW_FAIL;
tSINT32 OldBothHandStatus;
tSINT32 ADCFlag;

tSINT32 OldEStopStatus = NOK;
tSINT32 OldTRSStatus = NOK;
tSINT32 OldUFAIL24Status = NOK;
tSINT32 OldLEStatus = NOK;
tSINT32 OldSBeamStatus = NOK;
tSINT32 OldDriveStatus = NOK;
tSINT32 OldSonicsEnStatus = NOK;
tSINT32 OldPBStatus = NOK;
tSINT32 OldSV1 = ON;
tSINT32 OldSONICS_EN = ON;
tSINT32 OldPB_OUT  = PB_ENABLED;
tSINT32 OldBeeper = OFF;

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
   ADCFlag = 0;

   SysTick_Callback = IntegratedSysTickCallback; // System Tick Call Back function.
   if (SysTick_Config(SystemCoreClock / 1000))//Configure System Tick
      while (1)
            ; // Capture error
   printf("\n\nSystem has been initialized.\n");
   printf("Software Version %d\n\n",SWREVISION);
      printf("\n\nIntegration Step 4.\n");
   do
	{
		if(OneMsFlag) // One Ms flag
		{
			OneMsFlag = FALSE;
			ResetExternalWatchdog();
			GetInputs();
			TwoHandOperation();
			DeviceDiagnostics();
			SetOutputs();
			FaultFlag = FALSE;
         if (FirstTime)
         {
        	 if(IsThisCPU1)
        		 printf("CPU 1\n\n");
        	 else
        		 printf("CPU 2\n\n");
            FirstTime = FALSE;
            
            if (ULS == ON)
               printf("ULS (GSXI ONLY) is ON\n");
            else
               printf("ULS (GSXI ONLY) is OFF\n");
            OldULS = ULS;
            		
            if (TRS == TRSON)
               printf("TRS (GSXI ONLY) is ON\n");
            else
               printf("TRS (GSXI ONLY) is OFF\n");
            OldTRS = TRS;
            		
            if (SV1_REQ == ON)
               printf("SV1_REQ is ON\n");
            else
               printf("SV1_REQ is OFF\n");
            OldSV1_REQ = SV1_REQ;

            if(IsThisCPU1)
            {
            	printf("E-STOP NO,ESTOP2 is %ld\n", ESTOP2);
            	printf("E-STOP NC,ESTOP1 is %ld\n", ESTOP1);
            }
            else
            {
            	printf("E-STOP NO,ESTOP1 is %ld\n", ESTOP1);
            	printf("E-STOP NC,ESTOP2 is %ld\n", ESTOP2);
            }
            OldESTOP2 = ESTOP2;
            OldESTOP1  = ESTOP1;

            if (EStop == true)
               printf("ESTOP is pressed.\n");
            else
               printf("ESTOP is released.\n");
            OldEStop = EStop;
            
            if(IsThisCPU1)
            {
            	if (PB1_NO_IN == ON)
            		printf("PB1 is pressed.\n");
            	else
            		printf("PB1 is released.\n");

            	if(PB2_NO_IN == ON)
            		printf("PB2 is pressed.\n");
            	else
            		printf("PB2 is released.\n");
            }
            else
            {
            	if (PB2_NO_IN == ON)
            		printf("PB1 is pressed.\n");
            	else
            		printf("PB1 is released.\n");

            	if(PB1_NO_IN == ON)
            		printf("PB2 is pressed.\n");
            	else
            		printf("PB2 is released.\n");
            }
            OldPB1_NO_IN = PB1_NO_IN;
            OldPB2_NO_IN = PB2_NO_IN;

            printf("LE Position=%ld\n",QEIPosition);
            OldQEIPosition = QEIPosition;

            if (UFAIL24V == ON)
               printf("UFAIL24V PASS\n");
            else
               printf("UFAIL24V FAIL\n");
            OldUFAIL24V = UFAIL24V;

            if(IsThisCPU1)   //CPU1
            {
            	if (DRIVE_FAIL == ON)
            		printf("DRIVE_HI_FAIL is ON\n");
            	else
            		printf("DRIVE_HI_FAIL is OFF\n");
                        	
            	if(!(LPC_GPIO0->FIOPIN & DRIVELOWFAIL_MASK) == ON)
            		printf("DRIVE_LOW_FAIL is ON\n");
            	else
            		printf("DRIVE_LOW_FAIL is OFF\n");
                        	
            	OldDRIVE_HI_FAIL = DRIVE_FAIL;
            	OldDRIVE_LOW_FAIL = !(LPC_GPIO0->FIOPIN & DRIVELOWFAIL_MASK);  //Active Low
                        	
            	if(SV2_FAIL == ON)
            		printf("SV2_HI_FAIL is ON\n");
            	else
            		printf("SV2_HI_FAIL is OFF\n");
                        	            	
            	if(!(LPC_GPIO1->FIOPIN & SV2LOWFAIL_MASK) == ON)
            		printf("SV2_LOW_FAIL is ON\n");
            	else
            		printf("SV2_LOW_FAIL is OFF\n");
                        	            	
            	OldSV2_HI_FAIL = SV2_FAIL;
            	OldSV2_LOW_FAIL = !(LPC_GPIO1->FIOPIN & SV2LOWFAIL_MASK);		//Active Low
                        	
            	if(SONICS_FAIL == ON)
            		printf("SONICS_HI_FAIL is ON\n");
            	else
            		printf("SONICS_HI_FAIL is OFF\n");
                        	            	
            	if(!(LPC_GPIO0->FIOPIN & SONICSLOWFAIL_MASK) == ON)
            		printf("SONICS_LOW_FAIL is ON\n\n");
            	else
            		printf("SONICS_LOW_FAIL is OFF\n\n");
                        	            	
            	OldSONICS_HI_FAIL = SONICS_FAIL;
            	OldSONICS_LOW_FAIL = !(LPC_GPIO0->FIOPIN & SONICSLOWFAIL_MASK);		//Active Low
                        	
            }
            else    //CPU2
            {
            	if(!(LPC_GPIO0->FIOPIN & DRIVELOWFAIL_MASK) == ON)
            		printf("DRIVE_HI_FAIL is ON\n");
            	else
            		printf("DRIVE_HI_FAIL is OFF\n");
                        	
            	if (DRIVE_FAIL == ON)
            		printf("DRIVE_LOW_FAIL is ON\n");
            	else
            		printf("DRIVE_LOW_FAIL is OFF\n");
                        	
            	OldDRIVE_HI_FAIL = !(LPC_GPIO0->FIOPIN & DRIVELOWFAIL_MASK);	//Active Low
            	OldDRIVE_LOW_FAIL = DRIVE_FAIL;
                        	
            	if(!(LPC_GPIO1->FIOPIN & SV2LOWFAIL_MASK) == ON)
            		printf("SV2_HI_FAIL is ON\n");
            	else
            		printf("SV2_HI_FAIL is OFF\n");
                        	            	
            	if(SV2_FAIL == ON)
            		printf("SV2_LOW_FAIL is ON\n");
            	else
            		printf("SV2_LOW_FAIL is OFF\n");
                        	            	
            	OldSV2_HI_FAIL = !(LPC_GPIO1->FIOPIN & SV2LOWFAIL_MASK);		//Active Low
            	OldSV2_LOW_FAIL = SV2_FAIL;
                        	
            	if(!(LPC_GPIO0->FIOPIN & SONICSLOWFAIL_MASK) == ON)
            		printf("SONICS_HI_FAIL is ON\n");
            	else
            		printf("SONICS_HI_FAIL is OFF\n");
                        	            	
            	if(SONICS_FAIL == ON)
            		printf("SONICS_LOW_FAIL is ON\n\n");
            	else
            		printf("SONICS_LOW_FAIL is OFF\n\n");
                        	            	
            	OldSONICS_HI_FAIL = !(LPC_GPIO0->FIOPIN & SONICSLOWFAIL_MASK); //Active Low
            	OldSONICS_LOW_FAIL = SONICS_FAIL;
            }

            if (EStopStatus == OK)
               printf("EStop Button is OK\n");
            else
               printf("EStop Button is NOK\n");
            OldEStopStatus = EStopStatus;
            
            if (TRSStatus == OK)
               printf("TRS (GSXI ONLY) is OK\n");
            else
               printf("TRS (GSXI ONLY) is NOK\n");
            OldTRSStatus = TRSStatus;

            if (UFAIL24Status == OK)
               printf("UFAIL24Status is OK\n");
            else
               printf("UFAIL24Status is NOK\n");
            OldUFAIL24Status = UFAIL24Status;

            if (LEStatus == OK)
               printf("LEStatus is OK\n");
            else
               printf("LEStatus is NOK\n");
            OldLEStatus = LEStatus;

            if (DriveStatus == OK)
               printf("SVRelayStatus is OK\n");
            else
               printf("SVRelayStatus is NOK\n");
            OldDriveStatus = DriveStatus;

            if(SonicsEnStatus == OK)
            	printf("SonicsEnStatus is OK\n");
            else
            	printf("SonicsEnStatus is NOK\n");
            OldSonicsEnStatus = SonicsEnStatus;
            
            if (SBeamStatus == OK)
               printf("SBeamStatus is OK\n");
            else
               printf("SBeamStatus is NOK\n");
            OldSBeamStatus = SBeamStatus;

            if (PBStatus == OK)
               printf("PBStatus is OK\n");
            else
               printf("PBStatus is NOK\n");
            OldPBStatus = PBStatus;

       }
#ifdef INTEGRATION_TESTING
         /* The following code is to test the integration , this shall not
          * be part of the build of the final integrated code */

   if(BothHandStatus == BOTHHANDS_ON )
      SV1 = ON;
   else
      SV1 = OFF;
   if(BothHandStatus == BOTHHANDS_ON)
	   SONICS_EN = ON;
   else
	   SONICS_EN = OFF;
   if(BothHandStatus == BOTHHANDS_ON)
      PB_OUT = PB_ENABLED;
   else
      PB_OUT = PB_DISABLED;
   if(BothHandStatus == BOTHHANDS_ON)
      BEEPER = ON;
   else if(BothHandStatus == BOTHHANDS_OFF)
      BEEPER = OFF;
   else
   {
      BEEPER = OFF;
      FaultFlag = UFAIL24FaultFlag;
   }
   if (OldULS != ULS)
   {
      if (ULS == ON)
         printf("ULS (GSXI ONLY) is ON\n");
      else
         printf("ULS (GSXI ONLY) is OFF\n");
      OldULS = ULS;
   }

   if (OldTRS != TRS)
   {
      if (TRS == TRSON)
         printf("TRS (GSXI ONLY) is ON\n");
      else
         printf("TRS (GSXI ONLY) is OFF\n");
      OldTRS = TRS;
   }

   if (OldSV1_REQ != SV1_REQ)
   {
     if (SV1_REQ == ON)
        printf("SV1_REQ is ON\n");
     else
        printf("SV1_REQ is OFF\n");
     OldSV1_REQ = SV1_REQ;
   }

   if (OldEStop != EStop)
   {
      if (EStop == true)
         printf("ESTOP is pressed.\n");
      else
         printf("ESTOP is Released.\n");
      OldEStop = EStop;
   }

   if(IsThisCPU1)
	{
	   if (OldPB1_NO_IN != PB1_NO_IN)
	   {
		   if (PB1_NO_IN == ON)
			   printf("PB1 is pressed.\n");
		   else
			   printf("PB1 is released.\n");
		   OldPB1_NO_IN = PB1_NO_IN;
	   }

	   if (OldPB2_NO_IN != PB2_NO_IN)
	   {
		   if(PB2_NO_IN == ON)
			   printf("PB2 is pressed.\n");
		   else
			   printf("PB2 is released.\n");
		   OldPB2_NO_IN = PB2_NO_IN;
	   }
	}
	else
	{
		if (OldPB2_NO_IN != PB2_NO_IN)
		{
			if(PB2_NO_IN == ON)
				printf("PB1 is pressed.\n");
			else
				printf("PB1 is released.\n");
			OldPB2_NO_IN = PB2_NO_IN;
		}
           	 
		if (OldPB1_NO_IN != PB1_NO_IN)
		{
			if (PB1_NO_IN == ON)
				printf("PB2 is pressed.\n");
			else
				printf("PB2 is released.\n");
			OldPB1_NO_IN = PB1_NO_IN;
		}
	}
   
   if(ADCFlag == 1000)
   {
      printf("ADCChannel 0: %ld\tADCChannel 1: %ld\tForce(ADCChannel 2): %ld\n",ADC0Value[0],ADC0Value[1],ADC0Value[2]);
   }
   if(OldQEIPosition != QEIPosition)
   {
      printf("LE Position=%ld\n",QEIPosition);
      OldQEIPosition = QEIPosition;
   }
   
   if(IsThisCPU1)
	{
	   if (OldESTOP2 != ESTOP2)
	   {
		   if (ESTOP2 == 0)
			   printf("E-STOP NO,ESTOP2 is 0\n");
		   else
			   printf("E-STOP NO,ESTOP2 is 1\n");
		   OldESTOP2 = ESTOP2;
	   }
           	 
	   if (OldESTOP1 != ESTOP1)
	   {
		   if (ESTOP1 == 0)
			   printf("E-STOP NC,ESTOP1 is 0\n");
		   else
			   printf("E-STOP NC,ESTOP1 is 1\n");
		   OldESTOP1 = ESTOP1;
	   }

           	
	}
	else
	{
		if (OldESTOP1 != ESTOP1)
		{
			if (ESTOP1 == 0)
				printf("E-STOP NO,ESTOP1 is 0\n");
			else
				printf("E-STOP NO,ESTOP1 is 1\n");
			OldESTOP1 = ESTOP1;
		}
           	 
		if (OldESTOP2 != ESTOP2)
		{
			if (ESTOP2 == 0)
				printf("E-STOP NC,ESTOP2 is 0\n");
			else
				printf("E-STOP NC,ESTOP2 is 1\n");
			OldESTOP2 = ESTOP2;
		}
	}

   if (OldUFAIL24V != UFAIL24V)
   {
      if (UFAIL24V == ON)
         printf("UFAIL24V PASS\n");
      else
         printf("UFAIL24V FAIL\n");
      OldUFAIL24V = UFAIL24V;
   }
   
   if(IsThisCPU1)			//CPU 1
	{
	   //Drive HI and LOW test
	   if(OldDRIVE_HI_FAIL != DRIVE_FAIL)
	   {
		   if(DRIVE_FAIL == ON)
			   printf("DRIVE_HI_FAIL is ON\n");
		   else
			   printf("DRIVE_HI_FAIL is OFF\n");
		   OldDRIVE_HI_FAIL = DRIVE_FAIL;
	   }
           	 
	   if(OldDRIVE_LOW_FAIL != (!(LPC_GPIO0->FIOPIN & DRIVELOWFAIL_MASK)))
	   {
		   if(!(LPC_GPIO0->FIOPIN & DRIVELOWFAIL_MASK) == ON)
			   printf("DRIVE_LOW_FAIL is ON\n");
		   else
			   printf("DRIVE_LOW_FAIL is OFF\n");
		   OldDRIVE_LOW_FAIL = !(LPC_GPIO0->FIOPIN & DRIVELOWFAIL_MASK);
	   }
          
	   //SV2 HI and LOW test
	   if(OldSV2_HI_FAIL != SV2_FAIL)
	   {
		   if(SV2_FAIL == ON)
			   printf("SV2_HI_FAIL is ON\n");
		   else
			   printf("SV2_HI_FAIL is OFF\n");
		   OldSV2_HI_FAIL = SV2_FAIL;
	   }
           	 
	   if(OldSV2_LOW_FAIL != (!(LPC_GPIO1->FIOPIN & SV2LOWFAIL_MASK)))
	   {
		   if(!(LPC_GPIO1->FIOPIN & SV2LOWFAIL_MASK) == ON)
			   printf("SV2_LOW_FAIL is ON\n");
		   else
			   printf("SV2_LOW_FAIL is OFF\n");
		   OldSV2_LOW_FAIL =  !(LPC_GPIO1->FIOPIN & SV2LOWFAIL_MASK);
	   }
           	 
	   //SONICS HI and LOW test
	   if(OldSONICS_HI_FAIL != SONICS_FAIL)
	   {
		   if(SONICS_FAIL == ON)
			   printf("SONICS_HI_FAIL is ON\n");
		   else
			   printf("SONICS_HI_FAIL is OFF\n");
		   OldSONICS_HI_FAIL = SONICS_FAIL;
	   }
                
	   if(OldSONICS_LOW_FAIL != (!(LPC_GPIO0->FIOPIN & SONICSLOWFAIL_MASK)))
	   {
		   if(!(LPC_GPIO0->FIOPIN & SONICSLOWFAIL_MASK) == ON)
			   printf("SONICS_LOW_FAIL is ON\n");
		   else
			   printf("SONICS_LOW_FAIL is OFF\n");
		   OldSONICS_LOW_FAIL = !(LPC_GPIO0->FIOPIN & SONICSLOWFAIL_MASK);
	   }
           	 
	}
	else		//CPU 2
 	{
		//Drive HI and LOW test 
		if(OldDRIVE_HI_FAIL != (!(LPC_GPIO0->FIOPIN & DRIVELOWFAIL_MASK)))
		{
			if(!(LPC_GPIO0->FIOPIN & DRIVELOWFAIL_MASK) == ON)
				printf("DRIVE_HI_FAIL is ON\n");
			else
				printf("DRIVE_HI_FAIL is OFF\n");
			OldDRIVE_HI_FAIL = !(LPC_GPIO0->FIOPIN & DRIVELOWFAIL_MASK);
		}
		if(OldDRIVE_LOW_FAIL != DRIVE_FAIL)
		{
			if(DRIVE_FAIL == ON)
				printf("DRIVE_LOW_FAIL is ON\n");
			else
				printf("DRIVE_LOW_FAIL is OFF\n");
			OldDRIVE_LOW_FAIL = DRIVE_FAIL;
		}
           	 
		//SV2 HI and LOW test
		if(OldSV2_HI_FAIL != (!(LPC_GPIO1->FIOPIN & SV2LOWFAIL_MASK)))
		{
			if(!(LPC_GPIO1->FIOPIN & SV2LOWFAIL_MASK) == ON)
				printf("SV2_HI_FAIL is ON\n");
			else
				printf("SV2_HI_FAIL is OFF\n");
			OldSV2_HI_FAIL =  !(LPC_GPIO1->FIOPIN & SV2LOWFAIL_MASK);
		}
		if(OldSV2_LOW_FAIL != SV2_FAIL)
		{
			if(SV2_FAIL == ON)
				printf("SV2_LOW_FAIL is ON\n");
			else
				printf("SV2_LOW_FAIL is OFF\n");
			OldSV2_LOW_FAIL = SV2_FAIL;
		}
           	 
		//SONICS HI and LOW test
		if(OldSONICS_HI_FAIL != (!(LPC_GPIO0->FIOPIN & SONICSLOWFAIL_MASK)))
		{
			if(!(LPC_GPIO0->FIOPIN & SONICSLOWFAIL_MASK) == ON)
				printf("SONICS_HI_FAIL is ON\n");
			else
				printf("SONICS_HI_FAIL is OFF\n");
			OldSONICS_HI_FAIL = !(LPC_GPIO0->FIOPIN & SONICSLOWFAIL_MASK);
		}
		if(OldSONICS_LOW_FAIL != SONICS_FAIL)
		{
			if(SONICS_FAIL == ON)
				printf("SONICS_LOW_FAIL is ON\n");
			else
				printf("SONICS_LOW_FAIL is OFF\n");
			OldSONICS_LOW_FAIL = SONICS_FAIL;
		}
 	}
   
   if(ADCFlag <= 1000)
      ADCFlag++;
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

         if(OldSV1 != SV1)
           {
          if (SV1 == ON)
          {
             printf("SV1 is ON\n");
          }
          else
          {
             printf("SV1 is OFF\n");
          }
          OldSV1 = SV1;
       }
         if(OldSONICS_EN != SONICS_EN)
         {
        	 if(SONICS_EN == ON)
        	 {
        		 printf("SONICS is ON\n");	 
        	 }
        	 else
        	 {
        		 printf("SONICS is OFF\n");
        	 }
        	 OldSONICS_EN = SONICS_EN;
         }
       if(OldPB_OUT != PB_OUT)
       {
          if (PB_OUT == PB_ENABLED)
          {
             printf("PB_OUT is ENABLED\n");
          }
          else
          {
             printf("PB_OUT is DISABLED\n");
          }
          OldPB_OUT = PB_OUT;
       }
       if(OldBeeper != BEEPER)
       {
          if (BEEPER == ON)
          {
             printf("BEEPER is ON\n");
          }
          else if((BEEPER == OFF))
          {
             printf("BEEPER is OFF\n");
          }
          else
          {
             printf("BEEPER is beeping\n");
          }
          OldBeeper = BEEPER;
       }
#endif
		}
	} while (TRUE);
}
