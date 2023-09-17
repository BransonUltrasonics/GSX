// $Header:   D:/databases/VMdb/archives/EN13849/PBDiagnostics.c_v   1.9   02 Jan 2015 14:50:58   ewomack  $
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 1995,96, 2009        */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------

The aim of Palm button diagnostics is to detect if there is a short between
PB1 and PB2 wires. The safety feature is that both the palm buttons need to
be pressed, but if they are shorted then the system is no more safe.The drives
to the palm buttons is now not a contiguous one, but is driven through two
separate PWM channels which are always off from each other. At the mid-point
of the drives, the palm buttons are read. If both read as pressed, then a
problem is reported. While both the controllers are driving the PWMs, the drive
from only one of them is used in the hardware. Hence, the PWMs are fed to GPIOs
so that the decision of reading the palm buttons is only done through the GPIOs
and not from the PWMs.The palm buttons inputs going to processors are swapped.
This doesn't make any difference for normal execution. This is done because each
processor is driving a PB_OUT signal for the weld controller. So, the software
for both the controllers can still be the same while they drive, say PB1 as
output but it becomes PB1 for one controller and PB2 for the second controller.
The state machine is also modified for Palm buttons to function properly. In the
following states, the PB_OUT is allowed to go through:
Ready
SVON
PC
And in the following states, the PB_OUT is not allowed to go through:
Pre-Ready
Fault
FaultReset
The outputs module ensures that PB_OUT is driven only when PB1 is pressed and state machine has allowed PB_OUT to be driven.

Module name: PBDiagnostics

Filename:    PBDiagnostics.c

--------------------------- TECHNICAL NOTES -------------------------------

This code handles the Palm Button diagnostics.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/PBDiagnostics.c_v  $
 * 
 *    Rev 1.9   02 Jan 2015 14:50:58   ewomack
 * Unit Test changes
 * 
 *    Rev 1.8   20 Jul 2011 16:58:22   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.7   05 Jul 2011 11:00:36   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.4   22 Jun 2011 18:10:38   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient

-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "TestMacros.h"
#include "Global.h"
#include "core_cm3.h"
#include "lpc_types.h"
#include "Serial.h"
#include "system_LPC17xx.h"
#include "SystemInitialization.h"
#include "Led.h"
#include "PBDiagnostics.h"
#include "DeviceDiagnostics.h"
#include "StateMachine.h"
UT_VARIABLE(tSINT32 PBDiagtest)
UT_VARIABLE(tSINT32 EINT3_IRQHandlerFault)
UT_VARIABLE(tSINT32 DisableInterrupt)
UT_VARIABLE(tSINT32 EINT3_IRQHandlerTest)

volatile tUINT32 match_counter0, match_counter1;
tSINT32 PBErrorCounter = 0;

/**************************************************************************//**
* \brief  -		PWM_Init
*
* \brief  -		PWM initialization, setup GPIOs to PWM2,4 and 6,
*        		reset counter, all latches are enabled, interrupt
*         		on PWMMR5 and PWMR6, install PWM interrupt to the VIC table.
*
* \param  -		Duty cycle
* 
* \return  -	none
*
******************************************************************************/
void PWM_Init(tUINT32 cycle)
{
   FUNCTION1(PBDiagtest, "PWM_Init()");
   CHECKPOINT(PBDiagtest, BRANCH_1);

   LPC_PINCON->PINSEL4 |= (FIOPINSEL_P3FUN1_MASK |  FIOPINSEL_P5FUN1_MASK);/* set GPIOs for PWM 4,6 pins on PWM0 */
#ifdef UNIT_TESTING
   /* For prototype board */
   LPC_PINCON->PINSEL7 = FIOPINSEL_P25FUN2_MASK;   /* set GPIOs for PWM 2 pins on PWM0 */
#else
   /* This is the final board */
   LPC_PINCON->PINSEL4 |= FIOPINSEL_P1FUN1_MASK;
#endif
   LPC_PWM1->TCR = TCR_RESET; /* Counter Reset */
   LPC_PWM1->PR = 0x00;    /* count frequency:Fpclk */
   LPC_PWM1->MCR = 0x00 ; /* interrupt on PWMMR5 as rising edge and PWMR6 as falling edge*/
   LPC_PWM1->MR0 = cycle;     /* set PWM cycle */
   LPC_PWM1->MR1 = cycle ;
   LPC_PWM1->MR2 = cycle * 1/2 ;
   LPC_PWM1->MR3 = cycle * 1/2 ;
   LPC_PWM1->MR4 = cycle ;
   LPC_PWM1->MR5 = cycle * 1/4 ;
   LPC_PWM1->MR6 = cycle * 3/4 ;
   /* all PWM latch enabled */
   LPC_PWM1->LER = LER0_EN | LER1_EN | LER2_EN | LER3_EN | LER4_EN | LER5_EN | LER6_EN;
   LPC_PINCON->PINSEL4 = LPC_PINCON->PINSEL4 & FIOPINSEL_P11FUN0_MASK;//P2:11 pbint
   /* Enabling the interrupt */
   LPC_GPIO2->FIODIR   = LPC_GPIO2->FIODIR & (~(FIODIR_D11_MASK));
   LPC_GPIOINT->IO2IntEnF = 0x800;
   LPC_GPIOINT->IO2IntEnR = 0x800;
   FAULT(DisableInterrupt, { LPC_GPIO2->FIODIR   = 0x00; LPC_GPIOINT->IO2IntEnF = 0x00; LPC_GPIOINT->IO2IntEnR = 0x00;});
   NVIC_EnableIRQ(EINT3_IRQn);
   ChkFunReturn = 1;// for Diagnostics.
}
/**************************************************************************//**
* \brief  -    	PWM_Start
*
* \brief  -		Enable PWM by setting the PCR, PTCR registers
*
* \param  -		none
* 
* \return  -   	none
*
******************************************************************************/
void PWM_Start( void )
{
   FUNCTION1(PBDiagtest, "PWM_Start()");
   CHECKPOINT(PBDiagtest, BRANCH_1);
   LPC_PWM1->PCR = PWMSEL2 | PWMSEL4 | PWMSEL6 | PWMENA4 |PWMENA2 | PWMENA6;
   LPC_PWM1->TCR = TCR_CNT_EN | TCR_PWM_EN;  /* counter enable, PWM enable */
   ChkFunReturn = 1;// for Diagnostics.
}

/**************************************************************************//**
* \brief  -		PWM_Stop
*
* \brief  -		Stop all PWM channels
*
* \param  -		none
* 
* \return  -	none
*
******************************************************************************/
void PWM_Stop(void)
{
   FUNCTION1(PBDiagtest, "PWM_Stop()");
   CHECKPOINT(PBDiagtest, BRANCH_1);

   LPC_PWM1->PCR = 0;
   LPC_PWM1->TCR = 0x00;      /* Stop all PWMs */
   ChkFunReturn = 1; // for Diagnostics.
}

/**************************************************************************//**
* \brief  -    	PWM3_IRQHandler
*
* \brief  -		PWM1 interrupt handler
*       		Check for PB faults.Both PB can not be same at one interrupt.
*
* \param  -		none
* 
* \return  -   	none
*
******************************************************************************/
void EINT3_IRQHandler (void)
{
   tSINT32 volatile PB1_NO_Input;
   tSINT32 volatile PB2_NO_Input;
   tSINT32 volatile PB1Drv;
   tSINT32 volatile PB2Drv;
   tSINT32 volatile RisingEdge;
   tSINT32 PBError = FALSE;
   static tSINT32 PBDrv1Error = 0;
   static tSINT32 PBDrv2Error=0;

   FUNCTION1(EINT3_IRQHandlerTest, "EINT3_IRQHandler()");
   CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_1);
   PB1_NO_Input = (LPC_GPIO1->FIOPIN & PB1_NO_MASK)?1:0;
   PB2_NO_Input = (LPC_GPIO1->FIOPIN & PB2_NO_MASK)?1:0;
   PB2Drv   = (LPC_GPIO2->FIOPIN & PB2Drv_Mask)?1:0;
   PB1Drv   = (LPC_GPIO2->FIOPIN & PB1Drv_Mask)?1:0;
   RisingEdge = (LPC_GPIOINT->IO2IntStatR>>11)& RisingEdgeCondition;
   
   
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = TRUE; PB2Drv = FALSE; PB1_NO_Input = FALSE; PB2_NO_Input = TRUE;});//normal
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = FALSE; PB1Drv = TRUE; PB2Drv = FALSE; PB1_NO_Input = FALSE; PB2_NO_Input = TRUE;});//rising edge fault
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = FALSE; PB2Drv = TRUE; PB1_NO_Input = TRUE; PB2_NO_Input = FALSE;});//PB2 inside rising edge
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = TRUE; PB2Drv = TRUE; PB1_NO_Input = FALSE; PB2_NO_Input = TRUE;});//PB1DRV=PB2DRV
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = TRUE; PB2Drv = FALSE; PB1_NO_Input = TRUE; PB2_NO_Input = TRUE;});//PB1DRV=PB1Input 
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = FALSE; PB2Drv = TRUE; PB1_NO_Input = TRUE; PB2_NO_Input = TRUE;});//PB2DRV=PB2Input   
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = TRUE; PB2Drv = FALSE; PB1_NO_Input = FALSE; PB2_NO_Input = FALSE;});//PB1Input=PB2Input  
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = FALSE; PB2Drv = TRUE; PB1_NO_Input = TRUE; PB2_NO_Input = FALSE;});//PB1Drv = 0   
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = TRUE; PB2Drv = FALSE; PB1_NO_Input = FALSE; PB2_NO_Input = TRUE;});//PB2Drv = 0  
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = TRUE; PB2Drv = FALSE; PB1_NO_Input = FALSE; PB2_NO_Input = TRUE;PBError = TRUE;PBErrorCounter = PBErrorCount + 1;});   
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = TRUE; PB2Drv = FALSE; PB1_NO_Input = FALSE; PB2_NO_Input = TRUE;PBDrv1Error=PBErrorCount+1;PBDrv2Error=PBErrorCount+1;});//Decrement PBDrv1ErrorCount
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = FALSE; PB2Drv = TRUE; PB1_NO_Input = TRUE; PB2_NO_Input = FALSE;PBDrv1Error=PBErrorCount+1;PBDrv2Error=PBErrorCount+1;});//Decrement PBDrv2ErrorCount
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = FALSE; PB2Drv = TRUE; PB1_NO_Input = FALSE; PB2_NO_Input = TRUE;PBError = TRUE;PBErrorCounter = PBErrorCount - 1;});//PB2 inside rising edge
   FAULT(EINT3_IRQHandlerFault, {RisingEdge = TRUE; PB1Drv = FALSE; PB2Drv = TRUE; PB1_NO_Input = FALSE; PB2_NO_Input = TRUE;PBError = TRUE;PBErrorCounter = PBErrorCount + 1;})
   
   
   if (RisingEdge)/*To recognize the CPU*/
   {
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_2);
      if (PB1Drv == TRUE)/*If pin 66 of the CPU is 1 it is CPU2*/
      {
         CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_3);
         IsThisCPU1 = FALSE;
      }
      else/*If pin 66 of the CPU is 0 it is CPU1*/
      {
         CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_4);
         IsThisCPU1 = TRUE;
      }
   }
   else
   {
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_5);
   }
   if (PB1Drv == PB2Drv)/*PBDrive 1 and PBDrive 2 should always be opposite to each other*/
	{
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_6);
      PBError = TRUE;
   }
   else
   {
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_7);
   }
   if ((PB1Drv == TRUE) && (PB1_NO_Input == ON)) /*If PB1 drive is low then PB1 input should be high, i.e they are opposite to each other*/
   {
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_8);
      //PBError = TRUE;
      PBDrv1Error++;
   }
   else
   {
	   CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_9);
	   if((PBDrv1Error > 0) && (PB1_NO_Input == OFF))
	   {
		   CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_10);
		   PBDrv1Error--;
	   }
	   else
	   {
		   CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_11);
	   }
      
   }
   if ((PB2Drv == TRUE) && (PB2_NO_Input == ON))/*If PB2 drive is low then PB2 input should be high, i.e they are opposite to each other*/
   {
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_12);
      //PBError = TRUE;
      PBDrv2Error++;
   }
   else
   {
	   CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_13);
	   if((PBDrv2Error > 0) && (PB2_NO_Input == OFF))
	   {
		   CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_14);
		   PBDrv2Error--;
	   }
	   else
	   {
		   CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_15);
	   }
      
   }
   
   if ((PB1_NO_Input == ON) && (PB2_NO_Input == ON))/*Final Diagnostics is to make sure that the PB1 and PB@ are never same*/
   {
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_16);
      PBError = TRUE;
   }
   else
   {
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_17);
   }
  /*Read the value of the that PB whose Drive is low. This is important because the pins of Palm Buttons are swapped on the micros*/
   if (PB1Drv == FALSE)
   {
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_18);
      PB1_NO_INPUT = PB1_NO_Input;          
   }
   else
   {
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_19);
      if (PB2Drv == FALSE)
      {
         CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_20);
         PB2_NO_INPUT = PB2_NO_Input;
      }
      else
      {
         CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_21);
      }
   }
   //   FAULT(EINT3_IRQHandlerFault, {PBError = TRUE;});//PB1Error = 5
   //   FAULT(EINT3_IRQHandlerFault, {PBError = TRUE;PBErrorCounter = 24;});//PB1Error = 5
   if ((PBError == TRUE))
   {
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_22);
      PBErrorCounter++;
      if(PBErrorCounter > PBErrorCount)
      {
         CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_23);
         PBStatus = NOK;
      }
      else
      {
         CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_24);
      }
   }
   else
   {
      CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_25);
      if((PBDrv2Error > PBErrorCount) || (PBDrv1Error > PBErrorCount))
      {
    	  CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_26);
    	  PBStatus = NOK;
      }
      else
      {
    	  CHECKPOINT(EINT3_IRQHandlerTest, BRANCH_27);
    	  PBErrorCounter = 0;
      }
   }
   LPC_GPIOINT->IO2IntClr = 0x800;
   return;
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for PBDiagnostics test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void) {
   tUINT32 cycle = PWM_CYCLE;

   DiagInit(); //Initialize the Whole System including CPU and Peripherals.

   //Test PWM_Init function.
   PBDiagtest = 1;
   EINT3_IRQHandlerTest = 0;
   ChkFunReturn = 0;
   PWM_Init( cycle );
   DiagPrintInt(ChkFunReturn, 1);
   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

   //Test PWM_Start Function.
   ChkFunReturn = 0;
   DisableInterrupt = 0;
   PWM_Start( );
   DiagPrintInt(ChkFunReturn, 1);
   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

   //Test EINT3_IRQHandler function.
   //Test 1.
   EINT3_IRQHandlerTest = 1;
   PBStatus = OK;
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 0;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, OK);
   //Test 2.
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 1;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, OK);
   //Test 3.
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 2;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, OK);
   //Test 4.
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 3;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, OK);
   //Test 5.
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 4;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, OK);
   //Test 6.
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 5;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, OK);
   //Test 7.
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 6;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, OK);
   //Test 8.
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 7;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, OK);
   //Test 9.
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 8;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, OK);
   //Test 10.
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 9;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, NOK);
   //Test 11
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 10;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, NOK);
   //Test 12
   ChkFunReturn = 0;
   EINT3_IRQHandlerFault = 11;
   EINT3_IRQHandler();
   DiagPrintInt(PBStatus, NOK);

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();
   EINT3_IRQHandlerTest = 0;

   //Test PWM_Stop Function.
   ChkFunReturn = 0;
   PWM_Stop( );
   DiagPrintInt(ChkFunReturn, 1);
   DiagSummary();
}
#endif
