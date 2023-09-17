// $Header:   D:/databases/VMdb/archives/EN13849/StateMachine.c_v   1.17   09 Apr 2015 17:14:32   ewomack  $
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

This is the module for State machine implementation.

Module name: StateMachine

Filename:    StateMachine.c

--------------------------- TECHNICAL NOTES -------------------------------

The code is for different states involved in state machine and conditions
under which state transition takes place.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/StateMachine.c_v  $
 * 
 *    Rev 1.17   09 Apr 2015 17:14:32   ewomack
 * Modified SVON state to allow extra 25ms to detect part contact when palm buttons are removed.
 * 
 *    Rev 1.16   20 Jul 2011 16:59:12   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.15   07 Jul 2011 12:25:44   ASharma6
 * TRS Diagnostics for IW+ machine configuration is modified
 * 
 *    Rev 1.14   05 Jul 2011 11:01:20   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.11   22 Jun 2011 18:11:30   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.8   15 Jun 2011 17:46:10   ygupta
 * EStop and LE diagnostics fix
 *
 *    Rev 1.7   08 Jun 2011 07:52:28   pdwivedi
 * Added changes for phase 8.
 *
 *    Rev 1.6   20 May 2011 20:12:52   ygupta
 * Condition of 2HandStatus to go to SVON state from Ready State changed from "!= BothHandsOff" to "==BothHandsON".
 *
 *    Rev 1.5   02 May 2011 03:58:14   pdwivedi
 * Synchronize with CVS.
 *
 *    Rev 1.4   08 Apr 2011 08:37:06   PDwivedi
 * Added after sanity check.
 *
 *    Rev 1.3   01 Apr 2011 11:14:22   PDwivedi
 * Fixes after code review.
 *
 *    Rev 1.2   31 Mar 2011 08:00:30   ygupta
 * Modification done to comply to coding standards.
 *
 *    Rev 1.1   25 Mar 2011 09:10:12   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "TestMacros.h"
#include "StateMachine.h"
#include "PartContact.h"
#include "Global.h"
#include "Input.h"
#include "ProgramCounterTest.h"
#include "DeviceDiagnostics.h"
#include "ULSLEDIagnostics.h"

UT_VARIABLE(tSINT32 StatemachineDiagnostics)
UT_VARIABLE(tSINT32 PowerUpDiagnostic)
UT_VARIABLE(tSINT32 PreReadyDiagnostic)
UT_VARIABLE(tSINT32 ReadyDiagnostic)
UT_VARIABLE(tSINT32 DriveOnDiagnostic)
UT_VARIABLE(tSINT32 PCDetectedDiagnostic)
UT_VARIABLE(tSINT32 FaultDiagnostic)
UT_VARIABLE(tSINT32 FaultResetDiagnostic)
UT_VARIABLE(tSINT32 StateFillFault)
UT_VARIABLE(tSINT32 ChangeStateDiagnostics)

tSINT32 StateMachineTestResult;//For Diagnostics and unit testing
static tSINT8 PBDelay = 0;
extern tUINT32 FSum;
extern tSINT32 ForceOffset;
extern tSINT32 CurrPosition;

tUINT32 DriveONCtr = 0;


//		States						Entry code			Loop code			Exit code
 STATE PowerUpSt		=		{PowerUpEntry		,PowerUpState		,PowerUpExit};
 STATE PreReadySt 		= 		{PreReadyEntry		,PreReadyState		,PreReadyExit};
 STATE ReadySt 			= 		{ReadyEntry			,ReadyState			,ReadyExit};
 STATE FaultSt 			= 		{FaultEntry			,FaultState			,FaultExit};
 STATE FaultResetSt 	= 		{FaultResetEntry	,FaultResetState	,FaultResetExit};
 STATE DriveOnSt		=		{DriveOnEntry		,DriveOnState		,DriveOnExit};
 STATE PCDetectedSt		=		{PCDetectedEntry	,PCDetectedState	,PCDetectedExit};
/******************************************************************************************/

STATE *CurrState = &PowerUpSt;

/**************************************************************************//**
* 	\brief  -  PowerUp State of the State Machine.
* 
*  	\brief -		Entry Conditions:
*                     	1.> When power on or CPU reset.
*                     
*   \brief -		Action taken
*                     	1.> Checks if palm buttons are not pressed during power up.
*                     
*   \brief -		Exit condition
*                     	1.> When both PB1 and PB2 are released the state transitions to State PreReady.
*                         
* \param  - none
* 
* \return - none
*
******************************************************************************************/
void PowerUpState(void)
{
	 FUNCTION1(PowerUpDiagnostic, "PowerUpState()");
	 CHECKPOINT(PowerUpDiagnostic, BRANCH_1);
	 SV1 = OFF;
	
	if ((PB1_NO_IN == ON) || (PB2_NO_IN == ON) || PBStatus == NOK)
	{
		CHECKPOINT(PowerUpDiagnostic, BRANCH_2);
		PBStatus = NOK;
		BEEPER = BEEPING;
	}
	else
	{
		CHECKPOINT(PowerUpDiagnostic, BRANCH_3);
		ChangeState(PREREADY);
	}
}

/**************************************************************************//**
* 	\brief  -  PreReady State of the State Machine.
* 
*  	\brief -		Entry Conditions:
*                     	1.> Power on or CPU Reset.
*                     	2.> EStop Active from any other state.
*                     	3.> From Ready and DriveOn state in case of PB released or
*                         Two hand timing not satisfied.
*                     	4.> From PCDetect and DriveOn state if PB released before PartContact
*                         or SV ON becomes inactive.
*                     	5.> From Fault Reset state when Fault Flag becomes false.
*                     	6.> From PCDetected if part is lost in normal condition.
*                     
*   \brief -		Action taken
*                     	1.> In this state Solenoid Valve (SV) for GSXP and GSXI is off and Safe Torque-OFF(STO) for GSXE relay is turned on.
*                     	2.> Sonics_EN signal is enabled when E-Stop not pressed and disabled at pressed.
*                     
*   \brief -		Exit condition
*                     	1.> When both PB1 and PB2 are released, SV1_REQ is OFF and emergency stop is inactive
*                         the state transitions to State Ready.
*      					2.> Also if a fault flag is set then the state transitions to
*                         State Fault.
*
* \param  - none
* 
* \return - none
*
******************************************************************************************/
void PreReadyState(void)
{
   FUNCTION1(PreReadyDiagnostic, "PreReadyState()");
   CHECKPOINT(PreReadyDiagnostic, BRANCH_1);
   BEEPER = OFF;
   PB_OUT = PB_DISABLED;//PB are disabled to Weld Controller.
   PartContactStatus = NOTDETECTED;
   ALARM |= (BIT_MASK(ALARM_0) | BIT_MASK(ALARM_1) | BIT_MASK(ALARM_2) | BIT_MASK(ALARM_3));   //clear alarms 
   PBDelay = 25;
   if(MachineConfiguration == GSXE)
   {
	   CHECKPOINT(PreReadyDiagnostic, BRANCH_2);
	   SV1 = ON;  //enable STO
	   if((CurrPosition - QEIPositionOld) > MINDISTANCE)
	   {
		   CHECKPOINT(PreReadyDiagnostic, BRANCH_4);
		   LEStatus = NOK;
	   }
	   else if(CurrPosition < QEIPositionOld)
	   {
		   CHECKPOINT(PreReadyDiagnostic, BRANCH_5);
		   QEIPositionOld = CurrPosition;
		   LEStatus = OK;
	   }
	   else
	   {
		   CHECKPOINT(PreReadyDiagnostic, BRANCH_6);
	   }
   }
   else
   {
	   CHECKPOINT(PreReadyDiagnostic, BRANCH_3);
	   SV1 = OFF;	//disable solenoid drive
   }
   if (FaultFlag)// Go to fault state in case of any Fault.
   {
      CHECKPOINT(PreReadyDiagnostic, BRANCH_7);
      ChangeState(FAULT);
   }
   else
   {
      NotMyFault = FALSE;
      LastFault = 0;
      CHECKPOINT(PreReadyDiagnostic, BRANCH_8);
      if (EStop == NOTPRESSED)//Check PB only if EStop is inactive.
      {
    	  SONICS_EN = ON;
         CHECKPOINT(PreReadyDiagnostic, BRANCH_9);
         if (BothHandStatus == BOTHHANDS_OFF && SV1_REQ == OFF)//Both PBs are released and the previous cycle has ended.
         {
            CHECKPOINT(PreReadyDiagnostic, BRANCH_11);
            ChangeState(READY);
         }
         else//One or Both PBs are pressed.
         {
            CHECKPOINT(PreReadyDiagnostic, BRANCH_12);
         }
      }
      else//Estop is Active
      {
    	  SONICS_EN = OFF;
    	  SV1 = OFF;	
         CHECKPOINT(PreReadyDiagnostic, BRANCH_10);
      }
   }
}

/**************************************************************************//**
*
* 	\brief -   Ready State of the State Machine
* 	
*   \brief -		Entry Conditions:
*                      1.> State transition from State Pre-ready.
*                      
*   \brief -		Action taken
*                      1.> No outputs are changed in this state.
*                      
*   \brief -		Exit condition
*                      1.> When the output of Two Hand Operation is BothHandsOn
*                           and SVREQ id Active the state transitions to State SVOn.
*                      2.> emergency stop gets activated, then the state transitions
*                           back to State Pre-ready.
*                      3.> Also if a fault bit is set then the state transitions to State Fault.
*
* 	\param  -  none
* 	
* 	\return - none
*
******************************************************************************************/
void ReadyState(void)
{
   FUNCTION1(ReadyDiagnostic, "ReadyState()");
   CHECKPOINT(ReadyDiagnostic, BRANCH_1);
   BEEPER = OFF;
   ALARM |= (BIT_MASK(ALARM_0) | BIT_MASK(ALARM_1) | BIT_MASK(ALARM_2) | BIT_MASK(ALARM_3)); // Clear alarms
   if(MachineConfiguration == GSXE)
      {
	   	   CHECKPOINT(ReadyDiagnostic, BRANCH_2);
   	   	   SV1 = ON;  //enable STO
      }
      else
      {
    	  CHECKPOINT(ReadyDiagnostic, BRANCH_3);
    	  SV1 = OFF;	//disable solenoid drive
      }
   
   if((CurrPosition - QEIPositionOld) > MINDISTANCE)
	{
	   CHECKPOINT(ReadyDiagnostic, BRANCH_4);
	   LEStatus = NOK;
	}
	else if(CurrPosition < QEIPositionOld)
	{
		CHECKPOINT(ReadyDiagnostic, BRANCH_5);
		QEIPositionOld = CurrPosition;
		LEStatus = OK;
	}
	else
	{
		CHECKPOINT(ReadyDiagnostic, BRANCH_6);
	}
   
   if (FaultFlag)// Go to fault state in case of any Fault.
   {
      CHECKPOINT(ReadyDiagnostic, BRANCH_7);
      ChangeState(FAULT);
   }
   else // Fault Flag is False.
   {
      CHECKPOINT(ReadyDiagnostic, BRANCH_8);
      if ((EStop == PRESSED) || EStopStatus == INDETERMINATE)
      {
         CHECKPOINT(ReadyDiagnostic, BRANCH_9);
         ChangeState(PREREADY);
      }
      else //Check PB,SV only if EStop is inactive.
      {
         CHECKPOINT(ReadyDiagnostic, BRANCH_10);
         if (BothHandStatus != SECONDHANDTOOLATE)//Two Hand Timing Satisfied
         {
            CHECKPOINT(ReadyDiagnostic, BRANCH_12);
            if (BothHandStatus == BOTHHANDS_ON)//Both PBs are Pressed.
            {
               CHECKPOINT(ReadyDiagnostic, BRANCH_14);
               /*
                * In Ready state, if palm buttons are pressed, then the countdown
                * timer of Trigger switch diagnostics is made 0, so that the
                * diagnostics can start again.
               */
               NoTRSDiagCntr = 0;
               if (SV1_REQ == ON)//SVReq is active
               {
                  CHECKPOINT(ReadyDiagnostic, BRANCH_16);
                  PartContactStatus = NOTDETECTED;
                  
                  //Reset Part Contact parameters.
                  PeakVelocity = CurrVelocity;
                  PCForce = 0;
                  ForceOffset = FSum * 2;
                  ChangeState(DRIVEON);
               }
               else//SVReq is not active yet.
               {
                  CHECKPOINT(ReadyDiagnostic, BRANCH_15);
               }
            }
            else//only one PB is pressed.
            {
              CHECKPOINT(ReadyDiagnostic, BRANCH_13);
            }
         }
         else//Two Hand Timing not Satisfied
         {
            CHECKPOINT(ReadyDiagnostic, BRANCH_11);
            ChangeState(PREREADY);
         }
      }
   }
}

/**************************************************************************//**
*
* 	\brief -    DriveOn State of the State Machine
* 	
*   \brief -		Entry Conditions:
*                      1.> State transition from State Ready.
*                      
*   \brief -		Action taken
*                      1.> SV relay is turned ON in order to bring the horn down.
*                      
*   \brief -		Exit condition
*                      1.> The out put of TwoHand Operation is BothHandsOFF or
*                          ESTOP is pressed or SVREQ become inactive state transitions
*                          to state Pre-Ready.
*                      2.> When Part Contact is being made the state transitions to
*                          State PCDetected.
*                      3.> Also if a fault bit is set then the state transitions to State Fault.
*
* 	\param  -  none
* 
* 	\return - none
*
******************************************************************************************/
void DriveOnState(void)
{
   FUNCTION1(DriveOnDiagnostic, "DriveOnState()");
   CHECKPOINT(DriveOnDiagnostic, BRANCH_1);
   DriveONCtr++;
   if (FaultFlag)// Go to fault state in case of any Fault.
   {
      CHECKPOINT(DriveOnDiagnostic, BRANCH_2);
      ChangeState(FAULT);
   }
   else// Fault Flag is False.
   {
      CHECKPOINT(DriveOnDiagnostic, BRANCH_3);
      if ((EStop == PRESSED) || EStopStatus == INDETERMINATE)
      {
         CHECKPOINT(DriveOnDiagnostic, BRANCH_4);
         QEIPositionOld = CurrPosition;
         ChangeState(PREREADY);
      }
      else //Check PB,SV Req only if EStop is inactive.
      {
         CHECKPOINT(DriveOnDiagnostic, BRANCH_5);
         ALARM |= (BIT_MASK(ALARM_0) | BIT_MASK(ALARM_1) | BIT_MASK(ALARM_2) | BIT_MASK(ALARM_3)); 
         BEEPER = OFF;
         SV1 = ON;
       
         if (SV1_REQ == ON)//SV1 Req is active
         {
            CHECKPOINT(DriveOnDiagnostic, BRANCH_6);
            if (BothHandStatus == BOTHHANDS_ON || PBDelay > 0)//Both PBs are pressed.
            {
                CHECKPOINT(DriveOnDiagnostic, BRANCH_8);
            	if (BothHandStatus != BOTHHANDS_ON)
            	{
            		CHECKPOINT(DriveOnDiagnostic, BRANCH_10);
            		PBDelay--;
            	}
               if ( (CurrForce - ForceOffset) > SMALL_FORCE)// Force buildup detected
               {
            	   CHECKPOINT(DriveOnDiagnostic, BRANCH_11);
            	   if(DriveONCtr > MOTIONDELAY && (CurrPosition == QEIPositionOld))
            	   {
            		   CHECKPOINT(DriveOnDiagnostic, BRANCH_13);
            		   LEStatus = NOK;
            	   }
            	   else
            	   {
            		   CHECKPOINT(DriveOnDiagnostic, BRANCH_14);
            		   if(PartContactStatus==DETECTED)
            		   {
            			   CHECKPOINT(DriveOnDiagnostic, BRANCH_15);
            			   QEIPositionOld = CurrPosition;
            			   LEStatus = OK;
            			   ChangeState(PCDETECTED);
            		   }
            		   else
            		   {
            			   CHECKPOINT(DriveOnDiagnostic, BRANCH_16);
            		   }
            	   }

               }
               else//Wait for part contact.
               {
            	   CHECKPOINT(DriveOnDiagnostic, BRANCH_12);
               }
            }
            else//Wait for SV1 Req
            {
               CHECKPOINT(DriveOnDiagnostic, BRANCH_9);
               QEIPositionOld = CurrPosition;
               ChangeState(PREREADY);
            }
         }
         else // One or Both PBs are released.
         {
            CHECKPOINT(DriveOnDiagnostic, BRANCH_7);
            QEIPositionOld = CurrPosition;
            ChangeState(PREREADY);
         }
      }
   }
}

/**************************************************************************//**
*
* 	\brief -     PCDetected State of the State Machine
* 	
*   \brief -		Entry Conditions:
*                      1.> State transition from State DriveOn.
*                      
*   \brief -		Action taken
*                      1.> SV relay is turned ON in order to bring the horn down.
*                      
*   \brief -		Exit condition
*                      1.> When Part contact is lost state transitions to state PreReady.
*                      2.> Loss of SV On signal leads the state to transition back to Pre-ready state.
*                      3.> If emergency stop gets activated, then the state transitions
*                          back to State Pre-ready.
*                      4.> Also if a fault bit is set then the state transitions to State Fault.
*
* 	\param  -  none
* 
* 	\return - none
*
******************************************************************************************/
void PCDetectedState(void)
{
   FUNCTION1(PCDetectedDiagnostic, "PCDetectedState()");
   CHECKPOINT(PCDetectedDiagnostic, BRANCH_1);
   
   BEEPER = OFF;
   ALARM |= (BIT_MASK(ALARM_0) | BIT_MASK(ALARM_1) | BIT_MASK(ALARM_2) | BIT_MASK(ALARM_3));
   SV1 = ON;
   
   if (FaultFlag)// Go to fault state in case of any Fault.
   {
      CHECKPOINT(PCDetectedDiagnostic, BRANCH_2);
      ChangeState(FAULT);
   }
   else// Fault Flag is False.
   {
      CHECKPOINT(PCDetectedDiagnostic, BRANCH_3);
      if ((EStop == PRESSED) || EStopStatus == INDETERMINATE)
      {
         CHECKPOINT(PCDetectedDiagnostic, BRANCH_4);
         QEIPositionOld = CurrPosition;
         ChangeState(PREREADY);
      }
      else //Check PartContact and SVREQ only if EStop is inactive.
      {
         CHECKPOINT(PCDetectedDiagnostic, BRANCH_5);
         if (PartContactStatus != DETECTED)//PartContact Lost
         {
            CHECKPOINT(PCDetectedDiagnostic, BRANCH_6);
            QEIPositionOld = CurrPosition;
            ChangeState(PREREADY);
         }
         else // Part Contact is still there.
         {
            CHECKPOINT(PCDetectedDiagnostic, BRANCH_7);
            if (SV1_REQ == ON)//SVREQ is Active.
            {
               CHECKPOINT(PCDetectedDiagnostic,BRANCH_8);
            }
            else//SVREQ becomes inactive.
            {
               CHECKPOINT(PCDetectedDiagnostic,BRANCH_9);
               QEIPositionOld = CurrPosition;
#if UNIT_TESTING
#else
               printf("SV Request gone\n");
#endif
               PartContactStatus = NOTDETECTED;
               ChangeState(PREREADY);
            }
         }
      }
   }
}

/**************************************************************************//**
* 
* 	\brief -    Fault State of the State Machine
* 	
*   \brief -		Entry Conditions:
*                      1.> State transition from any other state in case of FaultFlag is true.
*                      
*   \brief -		Action taken
*                      1.> SV relay is turned OFF in order to bring the horn back to the safe state.
*                      2.> Buzzer is signaled to beep the fault condition. Beeping
*                          indicates fault condition.
*                      3.> Palm Buttons output is signaled high which will bypass the PB signals
*                          from weld controller.
*                      4.> Sonics_EN signal is turned OFF.
*                          
*   \brief -		Exit condition
*                      1.> If Both Hands status is BothHandOFF and ESTOP is not pressed state
*                          transitions to State FaultReset.
*
* 	\param  -  none
* 
* 	\return - none
*
******************************************************************************************/
void FaultState(void)
{
   FUNCTION1(FaultDiagnostic, "FaultState()");
   CHECKPOINT(FaultDiagnostic, BRANCH_1);
   
   BEEPER = BEEPING;
   SV1 = OFF;
   SONICS_EN = OFF;
   if (EStop == NOTPRESSED)////Check PBs only if EStop is inactive.
   {
      CHECKPOINT(FaultDiagnostic,BRANCH_2);
      if (BothHandStatus == BOTHHANDS_OFF)//GoTo FaultReset state if PBs are released.
      {
         CHECKPOINT(FaultDiagnostic,BRANCH_4);
         ChangeState(FAULTRESET);
      }
      else//PBs are still pressed.
      {
         CHECKPOINT(FaultDiagnostic,BRANCH_5);
      }
   }
   else //Estop is Active
   {
      CHECKPOINT(FaultDiagnostic, BRANCH_3);
   }
}

/**************************************************************************//**
* 	\brief -    FaultReset State of the State Machine
* 	
*   \brief -		Entry Conditions:
*                      1.> State transition from State Fault.
*                      
*   \brief -		Action taken
*                      1.> Beeper is signaled to display the fault condition. Buzzer is ON 
*                          indicates a fault condition.
*                      2.> Sonics_EN signal is disabled during this state.
*                          
*	\brief - 		Exit condition
*                      1.> Once the Fault Flag is False in Diagnostics the state transitions
*                          to state Pre-ready.
*
* 	\param  -  none
* 
* 	\return  -  none
*
******************************************************************************************/
void FaultResetState(void)
{
   FUNCTION1(FaultResetDiagnostic, "FaultResetState()");
   CHECKPOINT(FaultResetDiagnostic, BRANCH_1);
   BEEPER = BEEPING;
   PB_OUT = PB_DISABLED;//PB are disabled to Weld Controller.
   SONICS_EN = OFF;
   
   if (BothHandStatus == BOTHHANDS_ON)// Both PBs are pressed to reset the fault.
   {
      CHECKPOINT(FaultResetDiagnostic, BRANCH_2);
      FaultReset = TRUE;// Set Flag to Reset the Fault if possible.
   }
   else//One or both PB are not pressed.
   {
      CHECKPOINT(FaultResetDiagnostic, BRANCH_3);
      if (FaultReset == TRUE)//Two hand Operation is already done to reset the fault.
      {
         CHECKPOINT(FaultResetDiagnostic, BRANCH_4);
         if (FaultFlag)//Fault is still present.
         {
            CHECKPOINT(FaultResetDiagnostic, BRANCH_6);
            ChangeState(FAULT);
            FaultReset = FALSE;
         }
         else//Fault is Gone.
         {
            CHECKPOINT(FaultResetDiagnostic, BRANCH_7);
            ChangeState(PREREADY);
            FaultReset = FALSE;
            PB_OUT = PB_DISABLED;//PB are disabled to Weld Controller.
            BEEPER = OFF;
            SONICS_EN = ON;
            QEIPositionOld = CurrPosition;
         }
      }
      else//Two hand Operation is not done yet to reset the fault.
      {
         CHECKPOINT(FaultResetDiagnostic, BRANCH_5);
      }
   }
}

/**************************************************************************//**
* \brief -   	StateMachine
*
* \brief -    	State Machine driver which calls the appropriate State functions based
*           	upon the value of State variable.
*
* \param -		none
* 
* \return -  	none
*
******************************************************************************************/
void StateMachine(void)
{
    FUNCTION1(StatemachineDiagnostics, "StateMachine()");
    CHECKPOINT(StatemachineDiagnostics, BRANCH_1);
    switch (State)
    {
        case PREREADY:
            CHECKPOINT(StatemachineDiagnostics, BRANCH_2);
            StateMachineTestResult = 0;//For Unit Testing diagnostics.
        break;
        case READY:
            CHECKPOINT(StatemachineDiagnostics, BRANCH_3);
            StateMachineTestResult = 1;//For Unit Testing diagnostics.
        break;
        case FAULT:
            CHECKPOINT(StatemachineDiagnostics, BRANCH_4);
            StateMachineTestResult = 2;//For Unit Testing diagnostics.
        break;
        case FAULTRESET:
            CHECKPOINT(StatemachineDiagnostics, BRANCH_5);
            StateMachineTestResult = 3;//For Unit Testing diagnostics.
        break;
        case DRIVEON:
            CHECKPOINT(StatemachineDiagnostics, BRANCH_6);
            StateMachineTestResult = 4;//For Unit Testing diagnostics.
        break;
        case PCDETECTED:
            CHECKPOINT(StatemachineDiagnostics, BRANCH_7);
            StateMachineTestResult = 5;//For Unit Testing diagnostics.
        break;
        case POWERUP:
            CHECKPOINT(StatemachineDiagnostics, BRANCH_8);
            StateMachineTestResult = 6;//For Unit Testing diagnostics.
      	break;
        default:
        	CHECKPOINT(StatemachineDiagnostics, BRANCH_9);
        	StateMachineTestResult = 7;//For Unit Testing diagnostics.
        break;
    }
    CurrState->Loop();
    
    CHECKPOINT(StatemachineDiagnostics, BRANCH_10);
}

/**************************************************************************//**
* \brief  - Jump to the next state.
*
* \param  - NextState.
*			
* \return  - none.
*
******************************************************************************/
void ChangeState(ENSTATES NextState)
{
	FUNCTION1(ChangeStateDiagnostics, "ChangeState()");
	CHECKPOINT(ChangeStateDiagnostics, BRANCH_1);
	static int FirstTime = TRUE;  //Avoids execute exit code at power up
	if(FirstTime)
	{
		FirstTime = FALSE;
	}
	else
	{
		#if UNIT_TESTING
		#else
		CurrState->Exit();
		#endif
	}
	
	State = NextState;
	switch (State)
	    {
	        case PREREADY:
	            CHECKPOINT(ChangeStateDiagnostics, BRANCH_2);
	        	CurrState = &PreReadySt;
	        break;
	        case READY:
	            CHECKPOINT(ChangeStateDiagnostics, BRANCH_3);
	        	CurrState = &ReadySt;
	        break;
	        case FAULT:
	            CHECKPOINT(ChangeStateDiagnostics, BRANCH_4);
	        	CurrState = &FaultSt;
	        break;
	        case FAULTRESET:
	            CHECKPOINT(ChangeStateDiagnostics, BRANCH_5);
	        	CurrState = &FaultResetSt;
	        break;
	        case DRIVEON:
	            CHECKPOINT(ChangeStateDiagnostics, BRANCH_6);
	        	CurrState = &DriveOnSt;
	        break;
	        case PCDETECTED:
	            CHECKPOINT(ChangeStateDiagnostics, BRANCH_7);
	        	CurrState = &PCDetectedSt;
	        break;
	        case POWERUP:
	        	CHECKPOINT(ChangeStateDiagnostics, BRANCH_8);
	        	CurrState = &PowerUpSt;
	        break;
	        default:
	            CHECKPOINT(ChangeStateDiagnostics, BRANCH_9);
	        	CurrState = &FaultSt;
	        break;
	    }
	#if UNIT_TESTING
	#else
	CurrState->Entry();
	#endif
	CHECKPOINT(ChangeStateDiagnostics, BRANCH_10);
}

/**************************************************************************//**
* \brief  -   	PCDummy5
*
* \brief  -    	Dummy function used for the Program Counter Test
*
* \param  -		none
* 
* \return  -  	Function5_Return
*
******************************************************************************/
tSINT32 PCDummy5(void)
{
    return Function5_Return;
}

#if UNIT_TESTING

/**************************************************************************//**
* \brief  -   	ResetHandler
*
* \brief  -    ResetHandler is the entry point for State Machine unit testing.
*
* \param  -	none
* 
* \return  -  	none
*
*****************************************************************************/
void ResetHandler(void)
{
    DiagInit();
    StatemachineDiagnostics = 1;

    // Individual branch check for PowerUp State
    /***************PowerUp State Sr No .1*********************/
	State = POWERUP;
	PowerUpDiagnostic = 1;
	FaultFlag = FALSE;
	PB1_NO_IN = ON;
	PB2_NO_IN = ON;
 	PowerUpState();
	DiagPrintInt(State, POWERUP);
	/***********************************************************/
	
	/***************PowerUp State Sr No .2*********************/
	State = POWERUP;
	FaultFlag = FALSE;
	PBStatus = NOK;
	BEEPER = OFF;
	PowerUpState();
	DiagPrintInt(State, POWERUP);
	/***********************************************************/
	/***************PowerUp State Sr No .3*********************/
	State = POWERUP;
	FaultFlag = FALSE;
	PB1_NO_IN = OFF;
	PB2_NO_IN = OFF;
	PBStatus = OK;
	BEEPER = OFF;
	PowerUpState();
	DiagPrintInt(State,PREREADY);
	/***********************END************************************/
	PowerUpDiagnostic = 0;
    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();
        
    /***************PreReady State Sr No .1*********************/
    State = PREREADY;
    PreReadyDiagnostic = 1;
    FaultFlag = FALSE;
    BothHandStatus = BOTHHANDS_OFF;
    SV1_REQ = OFF;
    EStop = NOTPRESSED;
    PreReadyState();
    DiagPrintInt(State, READY);
    /***********************************************************/
    /***************PreReady State Sr No .2*********************/
    State = PREREADY;
    FaultFlag = TRUE;
    PreReadyState();
    DiagPrintInt(State, FAULT);
    /***********************************************************/
    /***************PreReady State Sr No .3******************************/
    State = PREREADY;
    FaultFlag = FALSE;
    EStop = PRESSED;
    PreReadyState();
    DiagPrintInt(State, PREREADY);
    /***********************************************************/
    /***************PreReady State Sr No .4******************************/
    State = PREREADY;
    FaultFlag = FALSE;
    EStop = NOTPRESSED;
    BothHandStatus = BOTHHANDS_ON;
    PreReadyState();
    DiagPrintInt(State, PREREADY);
    /***********************END************************************/
    PreReadyDiagnostic = 0;

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    // Individual branch Check for Ready State

    /***************Ready State Sr No .1******************************/
    State = READY;
    ReadyDiagnostic = 1;
    FaultFlag = FALSE;
    BothHandStatus = BOTHHANDS_ON;
    EStop = NOTPRESSED;
    SV1_REQ = ON;
    ReadyState();
    DiagPrintInt(State, DRIVEON);
    /***********************************************************/
    /***************Ready State Sr No .2******************************/
    State = READY;
    FaultFlag = FALSE;
    BothHandStatus = BOTHHANDS_ON;
    EStop = NOTPRESSED;
    SV1_REQ = OFF;
    ReadyState();
    DiagPrintInt(State, READY);
    /***********************************************************/
    /***************Ready State Sr No .3******************************/
    State = READY;
    FaultFlag = TRUE;
    ReadyState();
    DiagPrintInt(State, FAULT);
    /***********************************************************/
    /***************Ready State Sr No .4******************************/
    State = READY;
    FaultFlag = FALSE;
    EStop = PRESSED;
    ReadyState();
    DiagPrintInt(State, PREREADY);
    /***********************************************************/

    /***************Ready State Sr No .5******************************/
    State = READY;
    FaultFlag = FALSE;
    EStop = NOTPRESSED;
    BothHandStatus = SECONDHANDTOOLATE;
    ReadyState();
    DiagPrintInt(State, PREREADY);
    /***********************************************************/

    /***************Ready State Sr No .6******************************/
    State = READY;
    FaultFlag = FALSE;
    EStop = NOTPRESSED;
    BothHandStatus = BOTHHANDS_OFF;//ANYHANDS_OFF;
    ReadyState();
    DiagPrintInt(State, READY);
    /**********************END*************************************/

    ReadyDiagnostic = 0;

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    // Individual branch Check for Drive On State

    /***************Drive On State Sr No .1******************************/
    State = DRIVEON;
    PBDelay = 25;
    DriveOnDiagnostic = 1;
    FaultFlag = FALSE;
    BothHandStatus = BOTHHANDS_ON;
    CurrPosition = QEIPositionOld + 1;
    CurrForce = SMALL_FORCE + 1;
    ForceOffset=0;
    EStop = NOTPRESSED;
    SV1_REQ = ON;
    PartContactStatus = DETECTED;
    DriveOnState();
    DiagPrintInt(State, PCDETECTED);
    /***********************************************************/

    /***************Drive On State Sr No .2******************************/
    State = DRIVEON;
    FaultFlag = TRUE;
    DriveOnState();
    DiagPrintInt(State, FAULT);
    /***********************************************************/
    /***************Drive On State Sr No .3******************************/
    State = DRIVEON;
    FaultFlag = FALSE;
    EStop = PRESSED;
    DriveOnState();
    DiagPrintInt(State, PREREADY);
    /***********************************************************/

    /***************Drive On State Sr No .4******************************/
    State = DRIVEON;
    FaultFlag = FALSE;
    EStop = NOTPRESSED;
    CurrPosition = QEIPositionOld + 1;
    BothHandStatus = BOTHHANDS_OFF;//ANYHANDS_OFF;
    DriveOnState();
    DiagPrintInt(State, PCDETECTED);
    /***********************************************************/


    /***************Drive On State Sr No .5******************************/
    State = DRIVEON;
    SV1_REQ = OFF;
    FaultFlag = FALSE;
    EStop = NOTPRESSED;
    BothHandStatus = BOTHHANDS_OFF;//ANYHANDS_OFF;
    DriveOnState();
    DiagPrintInt(State, PREREADY);
    /***********************************************************/

    /***************Drive On State Sr No .6******************************/
    State = DRIVEON;
    FaultFlag = FALSE;
    StateFillFault = 0;
    EStop = NOTPRESSED;
    BothHandStatus = BOTHHANDS_OFF;
    PBDelay = 0;
    SV1_REQ = ON;
    DriveOnState();
    DiagPrintInt(State, PREREADY);

    /****************Drive On State Sr No .7******************************/
    State = DRIVEON;
    FaultFlag = FALSE;
    BothHandStatus = BOTHHANDS_ON;
    DriveONCtr = MOTIONDELAY;
    CurrPosition = QEIPositionOld;
    CurrForce = SMALL_FORCE + 1;
    ForceOffset=0;
    EStop = NOTPRESSED;
    PartContactStatus = NOTDETECTED;
    DriveOnState();
    DiagPrintInt(State, DRIVEON);
    
    /****************Drive On State Sr No .8******************************/
    State = DRIVEON;
	FaultFlag = FALSE;
	BothHandStatus = BOTHHANDS_ON;
	CurrForce = SMALL_FORCE - 1;
  	ForceOffset=0;
   	EStop = NOTPRESSED;
 	DriveOnState();
 	DiagPrintInt(State, DRIVEON);
 	
 	/****************Drive On State Sr No .9******************************/
 	State = DRIVEON;
	PBDelay = 25;
 	DriveOnDiagnostic = 1;
	FaultFlag = FALSE;
 	BothHandStatus = BOTHHANDS_ON;
	CurrPosition = QEIPositionOld + 1;
 	CurrForce = SMALL_FORCE + 1;
 	ForceOffset=0;
 	EStop = NOTPRESSED;
	PartContactStatus = NOTDETECTED;
 	DriveOnState();
	DiagPrintInt(State, DRIVEON);
    /*******************END****************************************/
    DriveOnDiagnostic = 0;

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    // Individual branch Check for PC Detected State

    /***************PC detected State Sr No .1******************************/
    State = PCDETECTED;
    PCDetectedDiagnostic = 1;
    FaultFlag = FALSE;
    PartContactStatus = DETECTED;
    EStop = NOTPRESSED;
    SV1_REQ = ON;
    PCDetectedState();
    DiagPrintInt(State, PCDETECTED);
    /***********************************************************/
    /***************PC detected State Sr No .2******************************/
    State = PCDETECTED;
    FaultFlag = TRUE;
    PCDetectedState();
    DiagPrintInt(State, FAULT);
    /***********************************************************/
    /***************PC detected State Sr No .3******************************/
    State = PCDETECTED;
    FaultFlag = FALSE;
    EStop = PRESSED;
    PCDetectedState();
    DiagPrintInt(State, PREREADY);
    /***********************************************************/
    /***************PC detected State Sr No .4******************************/
    State = PCDETECTED;
    FaultFlag = FALSE;
    EStop = NOTPRESSED;
    PartContactStatus = NOTDETECTED;
    PCDetectedState();
    DiagPrintInt(State, PREREADY);
    /***********************************************************/

    /***************PC detected State Sr No .5******************************/
    State = PCDETECTED;
    FaultFlag = FALSE;
    EStop = NOTPRESSED;
    PartContactStatus = DETECTED;
    SV1_REQ = OFF;
    PCDetectedState();
    DiagPrintInt(State, PREREADY);
    /*********************END**************************************/
    PCDetectedDiagnostic = 0;

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    // Individual branch Check for Fault State

    /***************Fault State Sr No .1******************************/
    State = FAULT;
    FaultDiagnostic = 1;
    BothHandStatus = BOTHHANDS_OFF;
    EStop = NOTPRESSED;
    FaultState();
    DiagPrintInt(State, FAULTRESET);
    /***********************************************************/
    /***************Fault State Sr No .2******************************/
    State = FAULT;
    EStop = PRESSED;
    FaultState();
    DiagPrintInt(State, FAULT);
    /***********************************************************/

    /***************Fault State Sr No .3******************************/
    State = FAULT;
    EStop = NOTPRESSED;
    BothHandStatus = BOTHHANDS_ON;
    FaultState();
    DiagPrintInt(State, FAULT);
    /*********************END**************************************/

    FaultDiagnostic = 0;

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    // Individual branch Check for Fault Reset State

    /***************FaultReset State Sr No .1******************************/
    State = FAULTRESET;
    FaultResetDiagnostic = 1;
    FaultFlag = FALSE;
    BothHandStatus = BOTHHANDS_OFF;
    FaultReset = TRUE;
    FaultResetState();
    DiagPrintInt(State, PREREADY);
    /***********************************************************/
    /***************FaultReset State Sr No .2******************************/
    State = FAULTRESET;
    FaultFlag = TRUE;
    BothHandStatus = BOTHHANDS_OFF;
    FaultReset = TRUE;
    FaultResetState();
    DiagPrintInt(State, FAULT);
    /***********************************************************/
    /***************FaultReset State Sr No .3******************************/
    State = FAULTRESET;
    BothHandStatus = BOTHHANDS_ON;
    FaultResetState();
    DiagPrintInt(State, FAULTRESET);
    /***********************************************************/
    /***************FaultReset State Sr No .4******************************/
    State = FAULTRESET;
    BothHandStatus = BOTHHANDS_OFF;
    FaultReset = FALSE;
    FaultResetState();
    DiagPrintInt(State, FAULTRESET);
    /***********************************************************/
    FaultResetDiagnostic = 0;
    // Print Summary
    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    //State Machine check
    /***************PreReady State******************************/
    StateMachineTestResult = 0;
    State = PREREADY;
    StateMachine();
    DiagPrintInt(StateMachineTestResult, 0);
    /***********************************************************/

    /***************Ready State*********************************/
    StateMachineTestResult = 0;
    State = READY;
    StateMachine();
    DiagPrintInt(StateMachineTestResult, 1);
    /***********************************************************/

    /***************Fault State*********************************/
    StateMachineTestResult = 0;
    State = FAULT;
    StateMachine();
    DiagPrintInt(StateMachineTestResult, 2);
    /***********************************************************/
    /***************Fault Reset  State**************************/
    StateMachineTestResult = 0;
    State = FAULTRESET;
    StateMachine();
    DiagPrintInt(StateMachineTestResult, 3);
    /***********************************************************/
    /***************SVOn State**********************************/
    StateMachineTestResult = 0;
    State = DRIVEON;
    StateMachine();
    DiagPrintInt(StateMachineTestResult, 4);
    /***********************************************************/
    /***************PC Detected State***************************/
    StateMachineTestResult = 0;
    State = PCDETECTED;
    StateMachine();
    DiagPrintInt(StateMachineTestResult, 5);
    /***********************************************************/
    /***************Power Up State***************************/
        StateMachineTestResult = 0;
        State = POWERUP;
        StateMachine();
        DiagPrintInt(StateMachineTestResult, 6);
        /***********************************************************/
    /***************Default  State******************************/
    StateMachineTestResult = 0;
    State = 99;
    StateMachine();
    DiagPrintInt(StateMachineTestResult, 7);
    /***********************************************************/
    StatemachineDiagnostics = 0;

    DiagSummary();
}

#endif
