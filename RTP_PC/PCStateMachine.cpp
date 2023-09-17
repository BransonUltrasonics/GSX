/*
 * PCStateMachine.cpp
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */


#include <stdio.h>
#include "PCStateMachine.h"
#include"PCObjDictionary.h"
#include"PCMain.h"
#include"Commons.h"
#include "FPGA.h"


	UINT8 PCStateMachine::alarm = FALSE;
	SCSTATES PCStateMachine::masterState = PREREADY;

/**************************************************************************//**
* \brief  - Constructor.
* \brief  - Initializes the state machine starting at ready state.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/	
PCStateMachine::PCStateMachine()
{
	//Constructor
	currentState = &ReadyObj;
	currentState->Enter();
}



/**************************************************************************//**
* \brief  - Destructor.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/	
PCStateMachine::~PCStateMachine()
{
	//Destructor
}

/**************************************************************************//**
* \brief  - Prints current master state.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/	
void PCStateMachine::PrintMasterState()
	{
		switch (PCStateMachine::masterState)
		{
#define		PRINT_MASTERSTATE		
#ifdef PRINT_MASTERSTATE
		case PREREADY:
			LOG("Pre-Ready");
			break;
		case SC_READY:
			LOG("Ready");
			break;
		case START_SWITCH:
			LOG("Start Switch");
			break;
		case WAIT_FOR_TRIGGER:
			LOG("Wait For Trigger");
			break;
		case WELD_SONIC_ON:
			LOG("Weld Sonics On");
			break;
		case HOLD:
			LOG("Hold");
			break;
		case WAIT_FOR_READY_POSITION:
			LOG("Wait for Ready Position");
			break;
		default:
			LOG("State # %i \n", PCStateMachine::masterState);
			break;
#endif
		}
	}

/**************************************************************************//**
* \brief  - Prints the current PC state.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/	
void PCStateMachine::PrintCurrentState()
	{
		switch (currentState->stateType)
		{
		case PC_READY:
		//printf("Ready");
		break;
		case WELD_RUN:
			//printf("WeldRun");
			break;
		case SEEK_RUN:
			//printf("SeekRun");
			break;
		case WELD_HD_RUN:
			//printf("Weld_HD_Run");
			break;
		case SCAN_RUN:
			//printf("PC_SCAN_Run");
			 break;
			 			 
#ifdef TEST_MODE
		case TEST_RUN:
			//printf("PC_TEST_Run");
			break;
#endif	//TEST_MODE			
			 
		default:
			//printf("State # %i \n", currentState->stateType);
			break;
		}
	}


/**************************************************************************//**
* \brief  - Executes the loop code for all states of the state machine.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/	
void PCStateMachine::RunStateMachine()
{
	
	if(((PC_RxPdo.MasterEvents & BIT_MASK(CTRL_ESTOP)) == BIT_MASK(CTRL_ESTOP)) && (currentState->stateType != PC_ESTOP))
	{
		FPGA::StopSonic();
		ChangeState(PC_ESTOP);
	}
	else if((PCStateMachine::masterState == SC_UNREACHABLE) && (currentState->stateType != PC_READY))
	{
		LOG("FPGA::StopSonic\n");

		FPGA::StopSonic();
		ChangeState(PC_READY);
	}
	currentState->Loop();
}

/*  PCStateMachine::ChangeState
 *
 *  Purpose:
 *   Executes the exit code of the previous state and gets the address of the next state to execute the entry code.
 *
 *   Entry condition: 
 *   PCSTATES NextState: ID of the next state to jump.
 *
 *   Exit condition: None.
 */
/**************************************************************************//**
* \brief  - Executes the loop code for all states of the state machine.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/	
void PCStateMachine::ChangeState(PCSTATES NextState)
{
	
	currentState->Exit();
		switch (NextState)
		{
		case PC_READY:
			LOG("PC_READY:\n");

			currentState = &ReadyObj;
			currentState->stateType = PC_READY;
			break;
		case WELD_RUN:
			LOG("WELD_RUN:\n");

			currentState = &WeldRunObj;
			currentState->stateType = WELD_RUN;
			break;
		case SEEK_RUN:
			LOG("SEEK_RUN:\n");

			currentState = &SeekRunObj;
			currentState->stateType = SEEK_RUN;
			break;
		case SCAN_RUN:
			LOG("SCAN_RUN:\n");

			currentState = &ScanRunObj;
			currentState->stateType = SCAN_RUN;
			break;
		case PC_ALARM:
			LOG("PC_ALARM:\n");

			currentState = &AlarmObj;
			currentState->stateType = PC_ALARM;
			break;
		case PC_ESTOP:
			currentState = &EstopObj;
			currentState->stateType = PC_ESTOP;
			break;
											
#ifdef TEST_MODE
		case TEST_RUN:
			currentState = &TestRunObj;
			currentState->stateType = TEST_RUN;
			break;
#endif //TEST_MODE				 			
			
		default:
			break;
		}
		PC_TxPdo.PCState = NextState;		
	#ifdef DEMO
		this->PrintCurrentState();
	#endif
		currentState->Enter();
}

