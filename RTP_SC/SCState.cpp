/*
 * SCState.cpp
 *
 *  Created on: May 22, 2017
 *      Author: lzhu
 */


#include "SCState.h"
#include "SC.h"
#include "AlarmManager.h"

SCState::SCState()
{
	/* gets the instance of CommonProperty class, Which is singleton*/
	CP = CommonProperty::getInstance();
}

SCState::~SCState()
{
	CP = NULL;
}

void SCState::UpdatePDO(void *ODobject, UINT16 ui16Index, UINT8 ui8SubIndex)
{


}
void SCState::ReadPDO(void *ODobject, UINT16 ui16Index, UINT8 ui8SubIndex)
{
}

void SCState::ChangeState(SCSTATES newState)
{
	SC::stateMachine.ChangeState(newState);
}


SCSTATES SCState::GetStateType ()
{
	return  stateType;
}

/**************************************************************************//**
* \brief  - Returns total weld cycle time.
*
* \param  - None
*
* \return - UINT32 - Cycle time counter
*
******************************************************************************/
UINT32 SCState::getCycleCounter()
{
	return SC::stateMachine.CycleCounter();
}

/**************************************************************************//**
* \brief  - Resets all the state's counter for next cycle.
*
* \param  - None
*
* \return - None
*
******************************************************************************/
void SCState::ResetCycleTimeCounter()
{
	return SC::stateMachine.ResetCycleTimeCounter();
}

/**************************************************************************//**
* \brief  - Accumulates the weld signature data at the rate of E-CAT cycle.
* 			virtual member function.
*
* \param  - None
*
* \return - None
*
******************************************************************************/
void SCState::CaptureWeldData()
{
}

/**************************************************************************//**
* \brief  - Enters alarm to alarm manager and changes state to SC_ALARM
*
* \param  - Source and ID of the alarm
*
* \return  - none
*
******************************************************************************/
void SCState::ProcessAlarm (UINT8 origin, UINT16 alarmID)
{
	AlarmMgr::EnterAlarmEvent (origin, alarmID);
 	ChangeState(SC_ALARM); 
}


/**************************************************************************//**
* \brief   - Post message into message queue based on Q-ID.
*
* \param   - MSG_Q_ID msgQID, Message& msg.
*
* \return  - None.
*
******************************************************************************/
void SCState::SendToMsgQ(MSG_Q_ID msgQID,Message& msg)
{
	if(msgQSend(msgQID,reinterpret_cast<char*>(&msg), sizeof(msg),NO_WAIT, MSG_PRI_NORMAL) != OK)
	{
		LOGERR("----------- ::SendToMsgQ:msgQSend: Error ----------",0,0,0);
	}
}
