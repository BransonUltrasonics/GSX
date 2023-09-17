/*
 * WaitForPartContact.cpp
 *
 *  Created on: May 30, 2017
 *      Author: lzhu
 */


#include "WaitForPartContact.h"
#include "SC_Common.h"
#include "MainTask.h"
#include "ControlTask.h"
#include "AlarmManager.h"
#include "Counters.h"

/**************************************************************************//**
* 
* \brief   - WaitForPartContact Class constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WaitForPartContact::WaitForPartContact()
{
	stateType = WAIT_FOR_PART_CONTACT;
}

/**************************************************************************//**
* 
* \brief   - WaitForPartContact Class destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WaitForPartContact::~WaitForPartContact()
{

}

/**************************************************************************//**
* 
* \brief   - This method will be executed when entering the
* 			 WaitForPartContact state.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForPartContact::Enter()
{
#ifdef DEBUG
	LOGDBG("Enter WaitForPartContact",0,0,0);
#endif
	Message message;
	SetUpPartContactPos = 0;

	message.msgID = TO_MAINTENANCE_TASK_CNTRS_INC_ACTUATIONS;
	SendToMsgQ (CP->GetMsgQId(cTaskName[MAINTENANCE_T]),message);
}

/**************************************************************************//**
* 
* \brief   - This method will be executed every 250us while waiting for the
* 			 PART_CONTACT_FOUND from AC.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForPartContact::Loop()
{	
	if(AC_TX->ACState == AC_ALARM)
	{
		ProcessAlarm (ALARM_AC);
	}
	else if(AC_TX->ACState == AC_ESTOP)
	{
		ProcessAlarm (ALARM_AC, ALARM_ESTOP_NCA);
	}
	else if(AC_TX->ACStatusEvent == BIT_MASK(PART_CONTACT_FOUND))
	{
	    SetUpPartContactPos = AC_TX->ActualDistance;	    	    

	    // send it to the event log
		EventLog (PART_CONTACT_PERFORM, CP->m_RecipeAC.GetPartContactPos(), SetUpPartContactPos);

		ControlTask::SetActuatorSetupData(EXP_PART_CONTACT,SetUpPartContactPos);
		ControlTask::SetActuatorSetupData(PRE_PART_CONTACT,SetUpPartContactPos);
		ControlTask::SetActuatorSetupData(ABS_POSITION,SetUpPartContactPos);		
		CP->m_RecipeAC.UpdatePartContactPos(SetUpPartContactPos);
		/* CP->UpdateAcWeldRecipe(CP->m_RecipeAC); Not required to do */
		ChangeState(WAIT_FOR_READY_POSITION);
	}
	else if(((AC_TX->ACInputs & SS1MASK) | (AC_TX->ACInputs & SS2MASK)) != BOTHSTARTSWITCHMASK)
	{
		ProcessAlarm (ALARM_SC, ALARM_START_SWITCH_LOST_EFA);
	}
	else 
	{
		DigitalIOMgr::ReadInputs ();
		
		if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_DISABLE) != DigitalIO::INPUT_ASSERTED)
		{
			
			// process input chain here
			if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_CYCLE_ABORT) == DigitalIO::INPUT_ASSERTED)
			{
				ProcessAlarm (ALARM_SC, ALARM_CYCLE_ABORT_NCA);
			}
			else if (AC_TX->ACInputs & GRDDETMASK)
			{
				ProcessAlarm (ALARM_SC, ALARM_GND_DETECT_EFA);
			}
		}
	}
}


/**************************************************************************//**
* 
* \brief   - This method will be executed when exiting the 
* 			 WaitForPartContact state.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForPartContact::Exit()
{
#ifdef DEBUG
	LOGDBG("Exit WaitForPartContact",0,0,0);
#endif
}

UINT32 WaitForPartContact::getSetupPartContactPos()
{
	return SetUpPartContactPos;
}


/**************************************************************************//**
* \brief   - Prepares log information for specific requests to be logged in
* 			the event log, and sends it to the DB task for further processing
*
* \param   - LOG_EVENT 	eventID
* \param   - UINT32 prevVal
* \param   - UINT32 newVal
*
* \return  - STATUS status
*
******************************************************************************/
STATUS WaitForPartContact::EventLog (LOG_EVENT eventID, UINT32 prevVal,	UINT32 newVal)
{
	Message _message;
	STATUS _status = OK;
	string details;
	
	// add cases here
	switch (eventID)
	{
	case PART_CONTACT_PERFORM:
		snprintf(_message.Buffer, sizeof(_message.Buffer),"Identifier:N/A:From:%d:To:%d:" , prevVal, newVal);
		details = _message.Buffer;
		break;
	default:
		_status = ERROR;
		break;
	}
	
	if (_status != ERROR)
		_status = DataBaseManager::EnterEventLog (eventID, details);
	
	return _status;
}
