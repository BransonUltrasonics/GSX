/*
 * Ready.cpp
 *
 *  Created on: May 22, 2017
 *      Author: lzhu
 */

#include "Common.h"
#include "Ready.h"
#include "fioLib.h"
#include <usrFsLib.h>
#include  "SCStateMachine.h"
#include  "SC_Common.h"
#include  "SC.h"
#include  "ControlTask.h"
#include <usrFsLib.h>
#include "IOManager.h"
#include "MainTask.h"
#include "AlarmManager.h"
#include"DataBaseManager.h"
#include "ReportGeneration.h"

/**************************************************************************//**
* \brief   - Ready Class constructor
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/

Ready::Ready()
{
	stateType = SC_READY;
	SeekCounter = 0; 
}

/**************************************************************************//**
* \brief   - Ready Class destructor
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/

Ready::~Ready()
{
}

/**************************************************************************//**
* \brief   - This method will be executed when entering the Ready state
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/

void Ready::Enter()
{
#ifdef DEBUG
	 printf ("\nEnter Ready\n");
#endif

	UINT32 TargetSeekCounterTemp = 0;
	ControlTask::SetWeldOpFlag(FALSE);
	ControlTask::SetPostWeldDoneFlag(FALSE);
	uiReadyLoopCounter = 0;
	SeekCounter = 0;
	ControlTask::TriggerConsoleTask();
    OPERATION opType = ControlTask::GetOperationType();
	
	
	if (CP->m_RecipeSC.IsTimedSeekEnabled() == TRUE)
	{
		TargetSeekCounterTemp= CP->m_RecipeSC.GetTimedSeek() * 60000;     /* Converting mins to ms */
		TargetSeekCounter = TargetSeekCounterTemp * (1000/ECAT_CYCLE_TIME_IN_US);
	}
	
	//reset cooling valve
	AC_RX->ACOutputs &= (UINT8)(~BIT_MASK(CONVERTER_COOLING));
	
	
	switch (opType)
	{
	case FIND_PART_CONTACT_OP:
	case CALIBRATION:
	case HORN_DOWN_CLAMP_ON:
	case HORN_DOWN_CLAMP_OFF:
	case JOG_UP_OP:
	case JOG_DOWN_OP: 
		// reset output for system ready
		DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_SYSTEM_READY, DigitalIO::LINE_LOW);
		break;
	
	default:
		// set output for system ready
		DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_SYSTEM_READY, DigitalIO::LINE_HIGH);
		break;
	}
	  
		
	// set output for cycle OK 
	if (AlarmMgr::IsCycleOK ())
		DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_CYCLE_OK, DigitalIO::LINE_HIGH);
	
	// reset output for cycle RUN 
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_CYCLE_RUN, DigitalIO::LINE_LOW);
	
	// reset output for PB RELEASE 
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_PB_RELEASE, DigitalIO::LINE_LOW);

	DigitalIOMgr::GetStatus(IOMGR_REFRESH_OUTPUTS);

	// at this point all alarms if any, should be non-reset required alarms
	if (AlarmMgr::GetAlarmCount() > 0 && AlarmMgr::GetAlarmActionCount (COUNT_RESET_REQ) == 0)
	{
		message.msgID = TO_ALARM_TASK_CLEAR_ALL;
		msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
	}

	// this is to ensure protection for early on RR alarm occurances from being cleared
	if (AlarmMgr::GetAlarmActionCount (COUNT_RESET_REQ) > 0)
		ChangeState(SC_ALARM);
}

/**************************************************************************//**
* \brief   - This method will be executed every 250us while waiting for the
*                       start switch signal
*                       both PC and AC in the ready state.
*                       This function is using multiple returns to resolve digital inputs
*                       lines status as quick and in line. The purpose is to resolve to a fault
*                       thus showing the return statements rather than them being hidden
*                       in a sub-routine call. This gives faster resolution and execution and simplifies
*                       the flow of the code.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/

void Ready::Loop()
{
	UINT8 TimeSeekFlag=0;
	OPERATION opType = ControlTask::GetOperationType();
	DigitalIO::INPUT_STATE inputDisable = DigitalIO::INPUT_NOT_ASSERTED;
	
	uiReadyLoopCounter=uiReadyLoopCounter+1;
	
	
	if((PC_TX->PC_StatusEvent & BIT_MASK(STATUS_TMR_INTERVAL))==BIT_MASK(STATUS_TMR_INTERVAL))
	{
		PC_RX->MasterEvents &=~BIT_MASK(CTRL_TMR_INTERVAL);
	}
	
	if((PC_TX->PC_StatusEvent & BIT_MASK(STATUS_PRETRIGGER_ENABLE))==BIT_MASK(STATUS_PRETRIGGER_ENABLE))
	{
		PC_RX->MasterEvents &=~BIT_MASK(CTRL_PRETRIGGER_ENABLE);
	}
	
	if((PC_TX->PC_StatusEvent & BIT_MASK(STATUS_AFTERBURST_ENABLE))==BIT_MASK(STATUS_AFTERBURST_ENABLE))
	{
		PC_RX->MasterEvents &=~BIT_MASK(CTRL_AFTERBURST_ENABLE);
	}
	
	if(FirmwareUpgrade::bIsFirmwareUpgrade == false)
	{
		if(ReportGeneration::GetReportGenerationFlag() == true)
		{
			if(AC_TX->ACState == AC_ESTOP)
			{
				ProcessAlarm (ALARM_AC, ALARM_ESTOP_NCA);
			}
			else if(AC_TX->ACState == AC_ALARM)
			{
				ProcessAlarm (ALARM_AC);
			}
			else if(PC_TX->PCState == PC_ALARM)
			{
				ProcessAlarm (ALARM_PC);
			}
			else if(AlarmMgr::IsAlarmState())
			{
				ChangeState(SC_ALARM);
			}
			else if(PC_RX->MasterEvents &BIT_MASK(CTRL_POWER_UP_RECIPE))
			{// wait in preready for the PC to finish processing the new powerup recipe
				ChangeState (PREREADY);
			}
			else
			{
				// Note: this call keeps a status that can be checked later
				DigitalIOMgr::ReadInputs ();
				inputDisable = DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_DISABLE);
				
				if (opType != NO_OPERATION)
				{
					switch (opType)
					{
					case SEEK:
					case SCAN:
					case TEST:
						// set output for system ready
						DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_SYSTEM_READY, DigitalIO::LINE_HIGH);
						
						if (inputDisable != DigitalIO::INPUT_ASSERTED)
						{
							// ignore INPUT_DISABLE process input chain here
						
							if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_CYCLE_ABORT) == DigitalIO::INPUT_ASSERTED)
							{
								ProcessAlarm (ALARM_SC, ALARM_CYCLE_ABORT_NCA);
								return;
							}
							if (AC_TX->ACInputs & GRDDETMASK)
							{
								ProcessAlarm (ALARM_SC, ALARM_GND_DETECT_EFA);
								return;
							}
							if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_SONIC_DISABLE) == DigitalIO::INPUT_ASSERTED)
							{
								AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_SONICS_DISABLE_WRA);
								return;
							}
						}
					
						ProcessLoop (opType);
						break;
					
					case WELD:
						// set output for system ready
						DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_SYSTEM_READY, DigitalIO::LINE_HIGH);												
													
						if (inputDisable != DigitalIO::INPUT_ASSERTED)
						{
							// check if the SEEk discrete input is active
							if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_HORN_SEEK) == DigitalIO::INPUT_ASSERTED)
							{
								// switch operation, start seek state
								ControlTask::SetOperationType(SEEK);
								ChangeState(SEEK_SONIC_ON);
								return;
							}
						}
	
					case FIND_PART_CONTACT_OP:
					case CALIBRATION:
					case HORN_DOWN_CLAMP_ON:
					case HORN_DOWN_CLAMP_OFF:
					case JOG_UP_OP:
					case JOG_DOWN_OP:
						if(opType == WELD)
						{
							// set output for system ready
							DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_SYSTEM_READY, DigitalIO::LINE_HIGH);
						}
						else
						{
							// reset output for system ready
							DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_SYSTEM_READY, DigitalIO::LINE_LOW);														
						}
						
						if (inputDisable != DigitalIO::INPUT_ASSERTED)
						{
							// ignore INPUT_DISABLE process input chain here
					
							if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_CYCLE_ABORT) == DigitalIO::INPUT_ASSERTED)
							{	
								ProcessAlarm (ALARM_SC, ALARM_CYCLE_ABORT_NCA);
								return;
							}
							if (AC_TX->ACInputs & GRDDETMASK)
							{
								ProcessAlarm (ALARM_SC, ALARM_GND_DETECT_EFA);
								return;
							}
					
							if (DigitalIOMgr::ScanForInputFunction (DigitalIO::INPUT_PART_PRESENT) &&
									DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_PART_PRESENT) != DigitalIO::INPUT_ASSERTED)
							{
								ChangeState (PREREADY);
								return;
							}
						}
				
						if(((AC_TX->ACInputs & SS1MASK) == SS1MASK) || ((AC_TX->ACInputs & SS2MASK) == SS2MASK))
						{
							// actuator move operation 
							if (inputDisable != DigitalIO::INPUT_ASSERTED)
							{
								if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_AC_HOME) == DigitalIO::INPUT_ASSERTED)
								{
									if (AC_TX->ACStatusEvent & BIT_MASK(HOME_POS))
									{
										ChangeState (PREREADY);
										return;
									}
									else
									{
										// switch operation for AC go home (not E.T)
										ControlTask::SetOperationType(AC_GO_HOME);
									}
								}
								else if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_AC_READY) == DigitalIO::INPUT_ASSERTED)
								{
									if (AC_TX->ACStatusEvent & BIT_MASK(READY_POS))
									{
										ChangeState (PREREADY);
										return;
									}
									else
									{
										// switch operation for AC go to ready position
										ControlTask::SetOperationType(AC_GO_READY);
									}
								}
							}
								
							ProcessLoop (opType);
						}
						else 
						{
							TimeSeekFlag = CP->m_RecipeSC.IsTimedSeekEnabled() ;
							if (TimeSeekFlag == TRUE)
							{
								if ((++SeekCounter) >= TargetSeekCounter)
								{
									CP->setIsTimedSeekEnabledForEvent(true);
									ControlTask::SetOperationType(SEEK);
									ChangeState(SEEK_SONIC_ON);
							
								}
							}
							
							if(CP->req_OptimizeDB || CP->bIsWSLogAttempt)
								ChangeState (PREREADY);
						}
						break;
				
					default:
						// set output for system ready
						DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_SYSTEM_READY, DigitalIO::LINE_HIGH);												
						break;
					}
				}
				else
				{
					ChangeState (PREREADY);
				}
			}
		}
		else
		{
			ChangeState (PREREADY);
		}
	}
}

/**************************************************************************//**
* \brief   - This method will be use to compartmentalize repeating
*      					loop code 
*
* \param   - cmdID - command to process
*
* \return  - None
*
******************************************************************************/

void Ready::ProcessLoop (OPERATION operation)
{
	bool bIsEnabled = false;

	switch (operation)
	{
	case SEEK:
		ChangeState(SEEK_SONIC_ON);
		break;

	case SCAN:	
		ChangeState(SCAN_SONIC_ON);		
		break;

	case WELD:
		ControlTask::SetWeldOpFlag(TRUE);
		bIsEnabled = RT.CheckWeldModes();

		if(bIsEnabled)
		{
			//HMI and WebServices login flags were added to the statement so the user can make weld cycles if he is logged in through WebServices.
			if((CP->m_RecipeSC.IsRecipeValidated()) || (VALIDATED == WeldRecipeStatus::GetRecipeStatus()) || (!CP->bIsHMILogged && CP->bIsWebServicesLogged))
				//added GetRecipeStatus because the value from the UI wasn't right on the first weld after logging in as operator
			{
				/* Allow to weld only when batch limit exceed alarms are cleared */
				if(!CP->m_ScAlarmObj.GetBatchCountAlarmFlag() && (!ControlTask::GetLimitExceedAlarm()))
				{
					SysConfigClass.SysConfigGet((char *)&systemConfig);
					/* If the Part ID Switch is ON in the UIC, then only after the Part ID scan weld should happen */
					if((systemConfig.bPartIDSwitchStatus && CP->weldResult.GetPartIDScanStatus()) || (! systemConfig.bPartIDSwitchStatus))			
					{		
						if(! CP->weldResult.GetPartIDScanStatus())
							CP->weldResult.SetPartID(DEFAULT_PART_ID);
						message.msgID = TO_UI_TASK_RECIPE_UPDATE_STOP_REQ;		/* Stops updating active weld recipe, If it is modifying from UIC */
						SendToMsgQ(CP->GetMsgQId(cTaskName[UI_T]),message);
						CP->weldResult.SetPartIDScanStatus(false);				/* Part ID scan set to false after a weld */
						ChangeState(START_SWITCH);
					}
				}
				else		/* Generate alarm until batch setup limit is not reset from UIC */
				{
					ProcessAlarm (ALARM_SC, ALARM_BATCH_COUNT_NCA);
				}
			}
			else	/* Generate alarm if the user tries to weld with not validated recipe */
			{
				ProcessAlarm (ALARM_SC, ALARM_RECIPE_NOT_VALIDATED_NCA);
			}
		}
		else
		{ 
			LOGDBG("READY_T : ------------MODE DISABLED------------",0,0,0);
			ChangeState(PREREADY);
		}
		break;

	case FIND_PART_CONTACT_OP:
	case HORN_DOWN_CLAMP_ON:
	case HORN_DOWN_CLAMP_OFF:
	case JOG_UP_OP:
	case JOG_DOWN_OP:
	case AC_GO_HOME:
	case AC_GO_READY:
		ChangeState(START_SWITCH);
		break;

	case CALIBRATION:
		if(1 == ControlTask::GetCalibrationStep())  
			ChangeState(START_SWITCH);
		break;

	case TEST:
		ChangeState(TEST_SONICS_ON);
		break;

	default:
		break;
	}
}


/**************************************************************************//**
* \brief   - This method will be executed when exiting the Exit state
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/

void Ready::Exit()
{
	// clear output for system ready
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_SYSTEM_READY, DigitalIO::LINE_LOW);
	
	message.msgID = TO_SDO_TASK_WELD_RECIPE_PC;
	SendToMsgQ(CP->GetMsgQId(cTaskName[SDO_T]),message);
	
#ifdef DEBUG
	LOGDBG("Exit Ready",0,0,0);
#endif
	

}

void Ready::GetLocalCopyOD(void* ODObject)
{
	/*TBD*/
}
