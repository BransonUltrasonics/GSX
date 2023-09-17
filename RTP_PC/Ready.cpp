/*
 * Ready.cpp
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#include "Ready.h"
#include "PCStateMachine.h"
#include "PCObjDictionary.h"
#include <stdio.h>
#include "TimerInterval.h"
#include <commons.h>
#include "FPGA.h"
#include "PowerUpRecipe.h"

/******************************************************************************
* \brief  - Ready
* 			constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
Ready::Ready() 
{
	//Constructor
	stateType = PC_READY;
}



/******************************************************************************
* \brief  - ~Ready
* 			destructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
Ready::~Ready()
{
	//Destructor
}



/******************************************************************************
* \brief  - Enter
* 			This is Entry code for Ready State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Ready::Enter()
{
LOG("Ready::Enter\n");
	//Enter code
}



/******************************************************************************
* \brief  - Exit
* 			This is Exit code for Ready State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Ready::Exit()
{
	LOG("Ready::Exit\n");

	//Read recipes
	SeekRecipobj.ReadSeekRecipeFromOD();
	//If estop is pressed from the power up it does not get the correct freq from the SC
	WeldRecipeObj.ReadWeldRecipeFromOD();
	
	FPGA::SetFrequency(WeldRecipeObj.PCWeldRecipe.StartFrequency);
	
	if(!Alarm::GetAlarmReason())
	{
		Alarm::ClearAlarmFlag();
	}
}



/******************************************************************************
* \brief  - Loop
* 			This is Loop code for Ready State. It will wait for seek or weld 
* 			state from supervisor controller.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Ready::Loop()
{

	if((PCStateMachine::masterState == PREREADY) && (PC_RxPdo.MasterEvents & BIT_MASK(CTRL_POWER_UP_RECIPE)) == BIT_MASK(CTRL_POWER_UP_RECIPE)) 
	{
		PowerUpData::ReadPowerUpDataFromOD();
		PC_TxPdo.PC_StatusEvent |= BIT_MASK(STATUS_POWER_UP_RECIPE);
	}
	else
	{
		PC_TxPdo.PC_StatusEvent &= ~BIT_MASK(STATUS_POWER_UP_RECIPE);
	}

	if (((PC_RxPdo.MasterEvents & BIT_MASK(CTRL_ESTOP)) != BIT_MASK(CTRL_ESTOP)) && (SC_UNREACHABLE != PCStateMachine::masterState))
	{
		if (PCStateMachine::alarm)
		{
			ChangeState(PC_ALARM);
			LOG("PC_ALARM\n");
		}
		else if (PCStateMachine::masterState == WELD_SONIC_ON)
		{
			ChangeState(WELD_RUN);
			
		}
		else if (PCStateMachine::masterState == SEEK_SONIC_ON  || PCStateMachine::masterState == TEST_SONICS_ON)
		{
			LOG("SEEK_RUN\n");

			printf("changed to seek or test sonic\n");
			ChangeState(SEEK_RUN);
		}
		else if(PCStateMachine::masterState == WAIT_FOR_TRIGGER) //this condition is for the pretrigger functionality 
		{
			if((PC_RxPdo.MasterEvents & BIT_MASK(CTRL_PRETRIGGER_ENABLE)) == BIT_MASK(CTRL_PRETRIGGER_ENABLE))
			{
		#ifdef DEBUG
				LOG("\n Pretrigger Distance Received Event by PC \n");
		#endif
				PC_TxPdo.PC_StatusEvent |= BIT_MASK(STATUS_PRETRIGGER_ENABLE);
				ChangeState(WELD_RUN);
			} 
			else
			{
				PC_TxPdo.PC_StatusEvent &= ~BIT_MASK(STATUS_PRETRIGGER_ENABLE);	
			}
		}
		else if(PCStateMachine::masterState == WAIT_FOR_READY_POSITION) //this condition is for the afterburst functionality 
		{
			if((PC_RxPdo.MasterEvents & BIT_MASK(CTRL_AFTERBURST_ENABLE)) == BIT_MASK(CTRL_AFTERBURST_ENABLE))
			{
				#ifdef DEBUG
					LOG("\n Afterburst ON Received Event by SC \n");
				#endif
				PC_TxPdo.PC_StatusEvent |= BIT_MASK(STATUS_AFTERBURST_ENABLE);
				ChangeState(WELD_RUN);
			} 
			else
			{
				PC_TxPdo.PC_StatusEvent &= ~BIT_MASK(STATUS_AFTERBURST_ENABLE);	
			}
		}		
		
		else if(PCStateMachine::masterState == SCAN_SONIC_ON)
		{
			ChangeState(SCAN_RUN);
		}
	
		if((PC_RxPdo.MasterEvents & BIT_MASK(CTRL_TMR_INTERVAL)) == BIT_MASK(CTRL_TMR_INTERVAL)) 
		{
			//TimerInterval::ReadTimerIntervalfromOD();
			PC_TxPdo.PC_StatusEvent |= BIT_MASK(STATUS_TMR_INTERVAL);
		}
		else
		{
			PC_TxPdo.PC_StatusEvent &= ~BIT_MASK(STATUS_TMR_INTERVAL);
		}
	}
}


