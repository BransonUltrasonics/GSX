// $Header:   D:/databases/VMdb/archives/EN13849/Application Code/IntegrationStep1.c_v   1.7   09 Apr 2015 17:08:54   ewomack  $
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

This is the first integration step. The first module that gets integrated is Inputs.

Filename:    IntegrationStep1.c

--------------------------- TECHNICAL NOTES -------------------------------

 Integration of software modules happens in steps with integration of 1 white box
 tested module at a time.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/Application Code/IntegrationStep1.c_v  $
 * 
 *    Rev 1.7   09 Apr 2015 17:08:54   ewomack
 * Compiler warning cleanup.
 * 
 *    Rev 1.6   20 Jul 2011 16:57:14   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.5   05 Jul 2011 10:59:40   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.2   22 Jun 2011 18:09:30   ASharma6
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

/* This define is to be enabled only during Integration testing. Once done, it shall
 * be disabled. It can be re-enabled for debugging.
 */
#define INTEGRATION_TESTING 1

#ifdef INTEGRATION_TESTING
tSINT32 OldULS;
tSINT32 OldTRS ;
tSINT32 OldSV1_REQ;
tSINT32 OldSV2_REQ;
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
tSINT32 ADCFlag;
#endif

volatile BOOL OneMsFlag = FALSE;

/**************************************************************************//**
* \brief -    IntegratedSysTickCallback
*
* \brief -     This the SystemTick(1Ms) interrupt Call back function.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void IntegratedSysTickCallback(void)
{
   /* The OneMsFlag is set here, so that all the processing need not be done
    * in interrupt context
    */
   OneMsFlag = TRUE;
}

/**************************************************************************//**
* \brief -    ResetHandler
*
* \brief -		This is invoked on power-up/coming out of a reset.
*				Inputs are tested in this step.
*
* \param -		none
* 
* \return -	none.
*
*****************************************************************************/
void ResetHandler(void) {
   SystemInitialization(); //Initialize the Whole System including CPU and Peripherals.
   static int FirstTime = TRUE;
   InitInputs();
   PrintfEnabled = TRUE;
   
   ADCFlag = 0;
   SysTick_Callback = IntegratedSysTickCallback; // System Tick Call Back function.
   if (SysTick_Config(SystemCoreClock / 1000))//Configure System Tick
   {
      while (TRUE)
         ; // Capture error
   }
   printf("\n\nSystem has been initialized.\n\n");
   printf("Software Version %d\n\n",SWREVISION);
   printf("\n\nIntegration Step 1.\n");
   PINSEL_SetOutput(PINSEL_PORT_1, PINSEL_PIN_27,ON);  //SV1 to active solid state relay for DRIVE_OUT signal
   PINSEL_SetOutput(PINSEL_PORT_1,PINSEL_PIN_15,ON);   //SONICS_EN to active solid state relay for SONICS_5V signal
   do {
      if(OneMsFlag) // One Ms flag
      {
         OneMsFlag = FALSE;
         ResetExternalWatchdog();
         GetInputs();
#ifdef INTEGRATION_TESTING
         /* The following code is to test the inputs, this shall not
          * be part of the build of the final integrated code */
         if (FirstTime)
         {
            FirstTime = FALSE;
            
            if(IsThisCPU1)
            	printf("CPU 1\n\n");
            else
            	printf("CPU 2\n\n");
            	
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
            
            if (SV2_REQ == ON)
            	printf("SV2_REQ is ON\n");
            else
            	printf("SV2_REQ is OFF\n");
            OldSV2_REQ = SV2_REQ;
            
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
            
            printf("Distance=%ld\n",QEIPosition);
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
            	if(DRIVE_FAIL == ON)
            		printf("DRIVE_HI_FAIL is ON\n");
            	else
            		printf("DRIVE_HI_FAIL is OFF\n");
            	
            	if (!(LPC_GPIO0->FIOPIN & DRIVEHIGHFAIL_MASK) == ON)
            		printf("DRIVE_LOW_FAIL is ON\n");
            	else
            		printf("DRIVE_LOW_FAIL is OFF\n");
            	
            	OldDRIVE_LOW_FAIL = !(LPC_GPIO0->FIOPIN & DRIVEHIGHFAIL_MASK);	//Active Low
            	OldDRIVE_HI_FAIL = DRIVE_FAIL;
            	
            	if(SV2_FAIL == ON)
            		printf("SV2_HI_FAIL is ON\n");
            	else
            		printf("SV2_HI_FAIL is OFF\n");
            	            	
            	if(!(LPC_GPIO1->FIOPIN & SV2HIGHFAIL_MASK) == ON)
            		printf("SV2_LOW_FAIL is ON\n");
            	else
            		printf("SV2_LOW_FAIL is OFF\n");
            	            	
            	OldSV2_LOW_FAIL = !(LPC_GPIO1->FIOPIN & SV2HIGHFAIL_MASK);		//Active Low
            	OldSV2_HI_FAIL = SV2_FAIL;
            	
            	if(SONICS_FAIL == ON)
            		printf("SONICS_HI_FAIL is ON\n");
            	else
            		printf("SONICS_HI_FAIL is OFF\n");
            	            	
            	if(!(LPC_GPIO0->FIOPIN & SONICSHIGHFAIL_MASK) == ON)
            		printf("SONICS_LOW_FAIL is ON\n\n");
            	else
            		printf("SONICS_LOW_FAIL is OFF\n\n");
            	            	
            	OldSONICS_LOW_FAIL = !(LPC_GPIO0->FIOPIN & SONICSHIGHFAIL_MASK); //Active Low
            	OldSONICS_HI_FAIL = SONICS_FAIL;
            }
            
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
         
         if(OldSV2_REQ != SV2_REQ)
         {
        	 if (SV2_REQ == ON)
        		 printf("SV2_REQ is ON\n");
        	 else
        		 printf("SV2_REQ is OFF\n");
        	 OldSV2_REQ = SV2_REQ;
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

         if (OldEStop != EStop)
         {
            if (EStop == true)
               printf("ESTOP is pressed.\n");
            else
               printf("ESTOP is released.\n");
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
        	 //ADCFlag = 0;
            printf("ADCChannel 0: %ld\tADCChannel 1: %ld\tForce(ADCChannel 2): %ld\tADCChannel 3: %ld"
            		"\tADCChannel 4: %ld\n",ADC0Value[0],ADC0Value[1],ADC0Value[2],ADC0Value[3],ADC0Value[4]);
         }
         
         if(OldQEIPosition != QEIPosition)
         {
            printf("Distance=%ld\n",QEIPosition);
            OldQEIPosition = QEIPosition;
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
        	 if(OldDRIVE_HI_FAIL != DRIVE_FAIL)
        	 {
        		 if(DRIVE_FAIL == ON)
        			 printf("DRIVE_HI_FAIL is ON\n");
        		 else
        			 printf("DRIVE_HI_FAIL is OFF\n");
        		 OldDRIVE_HI_FAIL = DRIVE_FAIL;
        	 }
        	 if(OldDRIVE_LOW_FAIL != (!(LPC_GPIO0->FIOPIN & DRIVEHIGHFAIL_MASK)))
        	 {
        		 if(!(LPC_GPIO0->FIOPIN & DRIVEHIGHFAIL_MASK) == ON)
        			 printf("DRIVE_LOW_FAIL is ON\n");
        		 else
        			 printf("DRIVE_LOW_FAIL is OFF\n");
        		 OldDRIVE_LOW_FAIL = !(LPC_GPIO0->FIOPIN & DRIVEHIGHFAIL_MASK);
        	 }
        	 
        	 //SV2 HI and LOW test
        	 if(OldSV2_HI_FAIL != SV2_FAIL )
        	 {
        		 if(SV2_FAIL == ON)
        			 printf("SV2_HI_FAIL is ON\n");
        		 else
        			 printf("SV2_HI_FAIL is OFF\n");
        		 OldSV2_HI_FAIL =  SV2_FAIL;
        	 }
        	 if(OldSV2_LOW_FAIL != (!(LPC_GPIO1->FIOPIN & SV2HIGHFAIL_MASK)))
        	 {
        		 if(!(LPC_GPIO1->FIOPIN & SV2HIGHFAIL_MASK) == ON)
        			 printf("SV2_LOW_FAIL is ON\n");
        		 else
        			 printf("SV2_LOW_FAIL is OFF\n");
        		 OldSV2_LOW_FAIL = !(LPC_GPIO1->FIOPIN & SV2HIGHFAIL_MASK);
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
        	 if(OldSONICS_LOW_FAIL != (!(LPC_GPIO0->FIOPIN & SONICSHIGHFAIL_MASK)))
        	 {
        		 if(!(LPC_GPIO0->FIOPIN & SONICSHIGHFAIL_MASK) == ON)
        			 printf("SONICS_LOW_FAIL is ON\n");
        		 else
        			 printf("SONICS_LOW_FAIL is OFF\n");
        		 OldSONICS_LOW_FAIL = !(LPC_GPIO0->FIOPIN & SONICSHIGHFAIL_MASK);
        	 }
         }
         
         
         if(ADCFlag <= 1000)
            ADCFlag++;
#endif
      } // OneMsFlag
   } while (TRUE);
}
