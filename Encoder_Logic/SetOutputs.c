// $Header:   D:/databases/VMdb/archives/EN13849/SetOutputs.c_v   1.15   02 Jan 2015 14:53:54   ewomack  $
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

This is the module for writing on particular Micro Controller  port.

Module name: SetOutputs

Filename:    SetOutputs.c

--------------------------- TECHNICAL NOTES -------------------------------

The code is for writing values of value of SV,PilotLight and PB_OUT to port.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/SetOutputs.c_v  $
 * 
 *    Rev 1.15   02 Jan 2015 14:53:54   ewomack
 * Unit Test fixes
 * 
 *    Rev 1.14   20 Jul 2011 16:59:04   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.13   12 Jul 2011 18:35:04   ASharma6
 * Revision 19 with modifications in Part contact lost algorithm - Loss of force alongwith horn retraction.
 * 
 *    Rev 1.12   05 Jul 2011 11:01:12   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.9   22 Jun 2011 18:11:22   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.2   31 Mar 2011 12:13:56   PDwivedi
 * Fixes after review.
 *
 *    Rev 1.1   25 Mar 2011 09:09:26   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include <stdio.h>
#include "Diagnostics.h"
#include "TestMacros.h"
#include "StateMachine.h"
#include "Global.h"
#include "SetOutputs.h"
#include "CrossMonitoring.h"

#define WDTIME   5

UT_VARIABLE(tSINT32 SetOutputsDiagnostics)
UT_VARIABLE(tSINT32 SetSVRelayDiagnostics)
UT_VARIABLE(tSINT32 SetEN_ALARMcodeDiagnostics)
UT_VARIABLE(tSINT32 HandleBeeperDiagnostics)
UT_VARIABLE(tSINT32 pinSelSetOutputDiagnostics)
UT_VARIABLE(tSINT32 HandleBeeperDiagnostics)
UT_VARIABLE(tSINT32 SetPBsDiagnostics)
UT_VARIABLE(tSINT32 SetResetWDDiag)
UT_VARIABLE(tSINT32 OutputInit)
UT_VARIABLE(tSINT32 SetPBFault)
UT_VARIABLE(tSINT32 OFFBeeperDiagnostics)
UT_VARIABLE(tSINT32 BeepBeeperDiagnostics)
UT_VARIABLE(tSINT32 HandleBeeperDiagnostics)
UT_VARIABLE(tSINT32 HandleBeeperFault)
UT_VARIABLE(tSINT32 OFFBeeperFault)
UT_VARIABLE(tSINT32 OFFBeeperFault1)
UT_VARIABLE(tSINT32 OFFBeeperFault2)

static tSINT32 Beeper;
static tSINT32 WDFeedTimer;
tSINT32 FlashingTimer = 0;
tSINT32  FlashCounter = 0;
tSINT32  BeeperOFF = 0;
tSINT32  Beeping = 0;

/**************************************************************************//**
* \brief -   	PINSEL_SetOutput
*
* \brief -  	Functions Sets the PORT pin status.
*
* \param - 	port:Port number.
*            	pin: Pin number.
*           	state: value to be set on that port pin.
*                      
* \return -  	none
*
****************************************************************************/
void PINSEL_SetOutput(tSINT32 port, tSINT32 pin, tSINT32 state)
{
   FUNCTION1(pinSelSetOutputDiagnostics, "PINSEL_SetOutput()");
   CHECKPOINT(pinSelSetOutputDiagnostics, BRANCH_1);

   tSINT32 * pFioVal = (tSINT32 *) (LPC_GPIO0_PIN_BASE + LPC_GPIO_PIN_PORTOFFSET * port);
   if (state)
   {
      CHECKPOINT(pinSelSetOutputDiagnostics, BRANCH_2);
      *pFioVal |= 1 << pin;
      ChkFunReturn = 1;
   }
   else
   {
      CHECKPOINT(pinSelSetOutputDiagnostics, BRANCH_3);
      *pFioVal &= ~(1 << pin);
      ChkFunReturn = 0;
   }
}
/**************************************************************************//**
* 	\brief -        Functions Handles the Beeper status i.e. ON/OFF according to the current state.
*
* 	\param - none
* 
* 	\return - none
*
*****************************************************************************/
void OFFBeeper(void)
{
   FUNCTION1(OFFBeeperDiagnostics, "OFFBeeper()");
   CHECKPOINT(OFFBeeperDiagnostics, BRANCH_1);
    FlashingTimer++;
    Beeper = OFF;
    if(FlashingTimer > FLASHOFFTIMEOFFPL)
    {
        CHECKPOINT(OFFBeeperDiagnostics, BRANCH_2);
        FlashingTimer = 0;
        BeeperOFF = 0;
    }
    CHECKPOINT(OFFBeeperDiagnostics, BRANCH_3);
}

/**************************************************************************//**
* 	\brief -         Functions Handles the Beeping of the beeper(buzzer)
*
*  	\param - none
* 
* 	\return - none
*
*****************************************************************************/
void BeepBeeper(void)
{
   FUNCTION1(BeepBeeperDiagnostics, "BeepBeeper()");
   FlashingTimer++;
    CHECKPOINT(BeepBeeperDiagnostics, BRANCH_1);
    if(Beeper == ON)
    {
        CHECKPOINT(BeepBeeperDiagnostics, BRANCH_2);
        if(FlashingTimer > FLASHONTIME)
        {
            CHECKPOINT(BeepBeeperDiagnostics, BRANCH_4);
            FlashingTimer = 0;
            Beeper = OFF;
            FlashCounter--;
        }
        else
        {
            CHECKPOINT(BeepBeeperDiagnostics, BRANCH_5);
            Beeper = ON;
        }
    }
    else
    {
        CHECKPOINT(BeepBeeperDiagnostics, BRANCH_3);
        if(FlashingTimer > FLASHOFFTIME)
        {
            CHECKPOINT(BeepBeeperDiagnostics, BRANCH_6);
            FlashingTimer = 0;
            Beeper = ON;
        }
        else
        {
            CHECKPOINT(BeepBeeperDiagnostics, BRANCH_7);
            Beeper = OFF;
        }
    }
}

/**************************************************************************//**
* \brief -       Functions Handles the Beeper status i.e. ON/OFF according to the current state.
*
* \param - none
* 
* \return - none
*
*****************************************************************************/
void HandleBeeper(void)
{
   int i;
   FUNCTION1(HandleBeeperDiagnostics, "HandleBeeper()");
   CHECKPOINT(HandleBeeperDiagnostics, BRANCH_1);
   if(NotMyFault == TRUE)
   {
      CHECKPOINT(HandleBeeperDiagnostics, BRANCH_2);
      Beeper = OFF;
      return;

   }
   CHECKPOINT(HandleBeeperDiagnostics, BRANCH_3);
   if (BEEPER ==  BEEPING)
   {
      /* There is some fault and the system is in Fault or FaultReset State */
      CHECKPOINT(HandleBeeperDiagnostics, BRANCH_5);
      if ((FlashCounter == 0) && (BeeperOFF == 0))
      {
         /* FlashCounter has no of beeps indicating the error no
          * OFFBeeper, if 1, means the Beeper will be OFF for OFF period
          */
         CHECKPOINT(HandleBeeperDiagnostics, BRANCH_7);
         for (i = 0; i < 10; i++)
         {
            /* Find out which ls flag is high, starting from ls bit signifying most important flag */
            CHECKPOINT(HandleBeeperDiagnostics, BRANCH_8);
            if ((FaultFlag >> i) & 1)
            {
               CHECKPOINT(HandleBeeperDiagnostics, BRANCH_10);
               FlashCounter = i+1; // Started with 0
               i = 10; // Exit if found a fault
            }
            else
            {
               CHECKPOINT(HandleBeeperDiagnostics, BRANCH_9);
            }
         }
         FAULT(HandleBeeperFault,{FlashCounter = 0 ;});
         if (FaultFlag)
         {
            CHECKPOINT(HandleBeeperDiagnostics, BRANCH_11);
            Beeping = 1;
         }
         else
         {
            CHECKPOINT(HandleBeeperDiagnostics, BRANCH_12);
            Beeping = 0; // Beeper will be OFF, so that the other CPU can flash error
         }
      }
      else
      {
         CHECKPOINT(HandleBeeperDiagnostics, BRANCH_6);
      }
      if (Beeping)
      {
         CHECKPOINT(HandleBeeperDiagnostics, BRANCH_13);
         BeepBeeper();
         CHECKPOINT(HandleBeeperDiagnostics, BRANCH_15);
      }
      else
      {
         CHECKPOINT(HandleBeeperDiagnostics, BRANCH_14);
      }
      FAULT(OFFBeeperFault,{Beeping = 0 ;});
      FAULT(OFFBeeperFault1,{Beeping = 1 ;});
      if(FlashCounter == 0 && Beeping == 1)
      {
         CHECKPOINT(HandleBeeperDiagnostics, BRANCH_17);
         Beeping = 0;
         BeeperOFF = 1;
      }
      else
      {
         CHECKPOINT(HandleBeeperDiagnostics, BRANCH_16);
      }
      FAULT(OFFBeeperFault2,{BeeperOFF = 0 ;});
      if(BeeperOFF == 1)
      {
         CHECKPOINT(HandleBeeperDiagnostics, BRANCH_19);
         OFFBeeper();
         CHECKPOINT(HandleBeeperDiagnostics, BRANCH_20);
      }
      else
      {
         CHECKPOINT(HandleBeeperDiagnostics, BRANCH_18);
      }
   }
   else if(BEEPER ==  ON)
   {
	   Beeper = ON;
	   FlashCounter = 0; 
   }
   else
   {
      CHECKPOINT(HandleBeeperDiagnostics, BRANCH_4);
      Beeper = OFF;
      FlashCounter = 0;  //Reset counter to avoid accumulated counts of the previous alarm
      CHECKPOINT(HandleBeeperDiagnostics, BRANCH_21);
   }
}

/**************************************************************************//**
* \brief -       Functions Sets the PinOutput for ALARM0-3 according to the ALARM code.
*
* \param -		none
* 
* \return -		none
*
*****************************************************************************/
void SetEN_ALARMcode(void)
{
   FUNCTION1(SetEN_ALARMcodeDiagnostics, "SetEN_ALARMcode()");
   CHECKPOINT(SetEN_ALARMcodeDiagnostics, BRANCH_1);
   HandleBeeper();
   CHECKPOINT(SetEN_ALARMcodeDiagnostics, BRANCH_2);
//#ifdef PROTOTYPEBOARD
//   PINSEL_SetOutput(PINSEL_PORT_0, PINSEL_PIN_11, !Beeper);    //Beeper Active low
//#else
   PINSEL_SetOutput(PINSEL_PORT_0, PINSEL_PIN_11, Beeper);    //Beeper Active high
//#endif
	PINSEL_SetOutput(PINSEL_PORT_2, PINSEL_PIN_0, (ALARM & BIT_MASK(ALARM_3)));
	PINSEL_SetOutput(PINSEL_PORT_2, PINSEL_PIN_1, (ALARM & BIT_MASK(ALARM_2)));
	PINSEL_SetOutput(PINSEL_PORT_2, PINSEL_PIN_2, (ALARM & BIT_MASK(ALARM_1)));
  	PINSEL_SetOutput(PINSEL_PORT_2, PINSEL_PIN_4, (ALARM & BIT_MASK(ALARM_0)));
  	
   CHECKPOINT(SetEN_ALARMcodeDiagnostics, BRANCH_3);
}

/**************************************************************************//**
* \brief -        Functions Sets the PinOutput for SVRelay according to the SV1 variable.
*
* \param -		none
* 
* \return -		none
*
*****************************************************************************/
void SetSVRelay(void)
{
   FUNCTION1(SetSVRelayDiagnostics, "SetSVRelay()");
   CHECKPOINT(SetSVRelayDiagnostics, BRANCH_1);
   PINSEL_SetOutput(PINSEL_PORT_1, PINSEL_PIN_27,SV1);
   CHECKPOINT(SetSVRelayDiagnostics, BRANCH_2);
   PINSEL_SetOutput(PINSEL_PORT_1,PINSEL_PIN_15,SONICS_EN);
   CHECKPOINT(SetSVRelayDiagnostics,BRANCH_3);
}

/**************************************************************************//**
* \brief -		SetPBs
*
* \brief -		Functions Sets the PinOutput for PBs according to the PBOUT variable.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void SetPBs(void)
{
   FUNCTION1(SetPBsDiagnostics, "SetPBs()");
   CHECKPOINT(SetPBsDiagnostics, BRANCH_1);
   FAULT(SetPBFault, {PB1_NO_IN = ON;});
   FAULT(SetPBFault, {PB1_NO_IN = OFF;});
   /* When enabled, this will be equivalent to a Palm button being pressed. Both
   the micro-controllers shall control one palm button each. The palm buttons are
   routed oppositely, so the left palm button becomes PB1 on uC1 and PB2 on uC2 and
   vice versa. It is sufficient for both the micro-controllers to use PB1 and drive
   PB_OUT with it as in effect, they are driving using left and right palm buttons
   respectively.
   State machine to decide whether PB_OUT is enabled or not
   */
   PB_OUT =  PB1_NO_IN;
   PINSEL_SetOutput(PINSEL_PORT_1, PINSEL_PIN_31,PB_OUT);      //PB_OUT:Active low
   CHECKPOINT(SetPBsDiagnostics, BRANCH_2);
}


/**************************************************************************//**
* \brief -	ResetExternalWatchdog
*
* \brief -	Functions Send the reset pulse to Reset the external WatchDog.
*
* \param -	none
* 
* \return -	none
*
*****************************************************************************/
void ResetExternalWatchdog(void)
{
   FUNCTION1(SetResetWDDiag, "ResetExternalWatchdog()");
   CHECKPOINT(SetResetWDDiag, BRANCH_1);
   WDFeedTimer++;
   static tSINT32 WDResetPulse = 0;
   if (WDFeedTimer > WDTIME)
   {
      CHECKPOINT(SetResetWDDiag, BRANCH_3);
      WDFeedTimer = 0;
      PINSEL_SetOutput(PINSEL_PORT_1, PINSEL_PIN_18,WDResetPulse);
      WDResetPulse = !WDResetPulse;
      CHECKPOINT(SetResetWDDiag, BRANCH_4);
   }
   else
   {
      CHECKPOINT(SetResetWDDiag,BRANCH_2);
   }
}

/**************************************************************************//**
* \brief -       SetOutputs
*
* \brief -       Functions call different functions to set current outputs.
*
* \param -       none
* 
* \return -      none
*
*****************************************************************************/
void SetOutputs(void)
{
   FUNCTION1(SetOutputsDiagnostics, "SetOutputs()");
   CHECKPOINT(SetOutputsDiagnostics, BRANCH_1);
   SetSVRelay();
   CHECKPOINT(SetOutputsDiagnostics, BRANCH_2);
   SetEN_ALARMcode();
   CHECKPOINT(SetOutputsDiagnostics, BRANCH_3);
   SetPBs();
   CHECKPOINT(SetOutputsDiagnostics, BRANCH_4);
   if (U2Fault == FALSE)
   {
      ResetExternalWatchdog();
   }
   else
   {
	   printf("U2Fault\n");
   }
   CHECKPOINT(SetOutputsDiagnostics, BRANCH_5);
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for SetOutputs test unit testing.
*
* \param -		none
* 
* \return -		none
*
*****************************************************************************/
void ResetHandler(void)
{
   DiagInit();
   IOInit();

// Set SV relay
   SetSVRelayDiagnostics = 1;
   pinSelSetOutputDiagnostics = 0;
   SetSVRelay();
   DiagPrintInt(1, 1);

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();
   SetSVRelayDiagnostics = 0;

//OFFBeeper Test case 1
   OFFBeeperDiagnostics =1;
   FlashingTimer = 4000;
   OFFBeeper();
   DiagPrintInt(1, 1);

//OFFBeeper Test case 2
   FlashingTimer = 6000;
   OFFBeeper();
   DiagPrintInt(1, 1);

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

   OFFBeeperDiagnostics = 0;

   BeepBeeperDiagnostics = 1;
//Test case 1
   Beeper = ON;
   FlashingTimer = 1100;
   BeepBeeper();
   DiagPrintInt(1, 1);

//Test case 2
   Beeper = ON;
   FlashingTimer = 900;
   BeepBeeper();
   DiagPrintInt(1, 1);

//Test case 3
   Beeper =BEEPING;
   FlashingTimer = 1100;
   BeepBeeper();
   DiagPrintInt(1, 1);

//Test case 4
   Beeper = BEEPING;
   FlashingTimer = 900;
   BeepBeeper();
   DiagPrintInt(1, 1);

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

   BeepBeeperDiagnostics = 0;

   HandleBeeperDiagnostics = 1;
//Handle Beeper
//Beeper test 1
   BEEPER = ON;
   OFFBeeperFault = 1;
   OFFBeeperFault1 = 1;
   HandleBeeperFault = 1;
   OFFBeeperFault2 =1;

   HandleBeeper();
   DiagPrintInt(1, 1);

//Beeper test 2
   BEEPER = BEEPING;
   FlashCounter = 0;
   OFFBeeperFault = 1;
   OFFBeeperFault1 = 0;
   HandleBeeperFault = 0;
   OFFBeeperFault2 =1;
   Beeping = TRUE;
   Beeper = OFF;
   Beeping = 1;
   BeeperOFF = 1;
   HandleBeeper();
   DiagPrintInt(1, 1);

//Beeper test 3
   BEEPER = BEEPING;
   FlashCounter = 0;
   HandleBeeperFault = 1;
   OFFBeeperFault1 = 1;
   OFFBeeperFault2 =1;
   OFFBeeperFault = 0;
   BeeperOFF = 1;
   FaultFlag = FALSE;
   Beeping = TRUE;
   Beeper = OFF;
   HandleBeeper();
   DiagPrintInt(1, 1);

//Beeper test 4  **
   BEEPER = BEEPING;
   FlashCounter = 0;
   BeeperOFF = 1;
   HandleBeeperFault = 1;
   OFFBeeperFault = 1;
   OFFBeeperFault1 = 1;
   OFFBeeperFault2 =1;
   FaultFlag = TRUE;
   Beeping = 0;
   Beeper = OFF;
   HandleBeeper();
   DiagPrintInt(1, 1);

//Beeper test 5
   BEEPER = BEEPING;
   FlashCounter = 0;
   BeeperOFF = 0;
   HandleBeeperFault = 1;
   OFFBeeperFault1 = 1;
   OFFBeeperFault = 0;
   OFFBeeperFault2 =1;
   FaultFlag = TRUE;
   Beeper = OFF;
   FlashCounter = 0;
   Beeping = 1;
   HandleBeeper();
   DiagPrintInt(1, 1);

//Beeper test 6
  BEEPER = BEEPING;
  FlashCounter = 0;
  BeeperOFF = 1;
  Beeping = 0;
  HandleBeeperFault = 1;
  OFFBeeperFault1 = 1;
  OFFBeeperFault = 1;
  OFFBeeperFault2 = 0;
  FaultFlag = FALSE;
  Beeper = OFF;
  HandleBeeper();
  DiagPrintInt(1, 1);

//Beeper test 7
  BEEPER = BEEPING;
  FlashCounter = 0;
  BeeperOFF = 0;
  HandleBeeperFault = 1;
  OFFBeeperFault1 = 1;
  OFFBeeperFault = 1;
  OFFBeeperFault2 =1;
  FaultFlag = FALSE;
  Beeper = OFF;
  FlashCounter = 0;
  Beeping = 0;
  HandleBeeper();
  DiagPrintInt(1, 1);

  //Beeper test 9
   BEEPER = ON;
   NotMyFault = TRUE;

   HandleBeeper();
   DiagPrintInt(1, 1);

  HandleBeeperDiagnostics = 0;
// Print Summary
   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

// Set Beep
   SetEN_ALARMcodeDiagnostics = 1;
   pinSelSetOutputDiagnostics = 0;
   SetEN_ALARMcode();
   DiagPrintInt(1, 1);
   SetEN_ALARMcodeDiagnostics = 0;

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

// Set PBs
   SetPBsDiagnostics = 1;
   pinSelSetOutputDiagnostics = 0;
   SetPBs();
   DiagPrintInt(PB_OUT, PB1_NO_IN);
   SetPBsDiagnostics = 0;

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

// ResetExternalWatchdog test case no 1
   SetResetWDDiag = 1;
   WDFeedTimer = 6;
   pinSelSetOutputDiagnostics = 0;
   ResetExternalWatchdog();
   DiagPrintInt(1, 1);

// ResetExternalWatchdog test case no 2
   SetResetWDDiag = 1;
   WDFeedTimer = 2;
   ResetExternalWatchdog();
   DiagPrintInt(1, 1);

   SetResetWDDiag = 0;

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

// PINSEL_SetOutput case no 1
   pinSelSetOutputDiagnostics = 1;
   ChkFunReturn = 0;
   PINSEL_SetOutput(0,10,1);
   DiagPrintInt(ChkFunReturn, 1);

// PINSEL_SetOutput case no 2
   ChkFunReturn = 0;
   PINSEL_SetOutput(0,10,0);
   DiagPrintInt(ChkFunReturn, 0);

   pinSelSetOutputDiagnostics = 0;

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

// Set outputs
   SetOutputsDiagnostics = 1;
   SetOutputs();
   DiagPrintInt(1, 1);
   SetOutputsDiagnostics = 0;

   DiagSummary();
}

#endif
