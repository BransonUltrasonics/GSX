/*
 * Alarm.cpp
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#include <vxworks.h>
#include "Alarm.h"
#include "FPGA.h"
#include "ADC.h"
#include "PCStateMachine.h"
#include <stdio.h>
#include "DynamicSystemCall.h"
#include "Commons.h"

UINT8 Alarm::AlarmFlag = FALSE;
UINT32 Alarm::AlarmsReasonMask;

/**************************************************************************//**
* \brief  - Constructor.
*
* \param  - none
*
*
* \return  - none.
*
******************************************************************************/
Alarm::Alarm()
{
	//Constructor
}

/**************************************************************************//**
* \brief  - Destructor.
*
* \param  - none
*
*
* \return  - none.
*
******************************************************************************/
Alarm::~Alarm()
{
	//Destructor
}

/**************************************************************************//**
* \brief  - State entry code.
* \brief  - Stops the sonics when an alarm occurred
*
* \param  - none
*
*
* \return  - none.
*
******************************************************************************/
void Alarm::Enter()
{
	OBJCONST TOBJECT OBJMEM * pObjEntry;
	UINT8 size;
	pObjEntry = obj_getobjecthandleSc( INDEX_PC_ALARM );
	UINT16 offset;

	size= sizeof(AlarmsReasonMask);
	offset= obj_getentryoffsetSc(1, pObjEntry) / 8;
	obj_writeSc( INDEX_PC_ALARM, &AlarmsReasonMask, &offset, &size, NUM_OF_ALARM_ENTRY);
	
	LOG("ALARM:Enter:\n");

	if ((AlarmsReasonMask & (UINT32) ERR_TEMP_OVERLOAD) != NO_ERROR)
		LOG("AlarmsReason.AlarmTypeMask: ERR_TEMP_OVERLOAD\n");
	if ((AlarmsReasonMask & (UINT32) ERR_CURRENT_OVERLOAD) != NO_ERROR)
		LOG("AlarmsReason.AlarmTypeMask: ERR_CURRENT_OVERLOAD\n");
	if ((AlarmsReasonMask & (UINT32) ERR_VOLTAGE_OVERLOAD) != NO_ERROR)
		LOG("AlarmsReason.AlarmTypeMask: ERR_VOLTAGE_OVERLOAD\n");
	if ((AlarmsReasonMask & (UINT32) ERR_POWER_OVERLOAD) != NO_ERROR)
		LOG("AlarmsReason.AlarmTypeMask: ERR_POWER_OVERLOAD\n");
	if ((AlarmsReasonMask & (UINT32) ERR_15V_LOST) != NO_ERROR)
		LOG("AlarmsReason.AlarmTypeMask: ERR_15V_LOST\n");
	
	FPGA::StopSonic();
	FPGA::ResetFPGAOverload();
}

/**************************************************************************//**
* \brief  - State exit code.
* \brief  - Clears alarm flag
*
* \param  - none
*
*
* \return  - none.
*
******************************************************************************/
void Alarm::Exit()
{
	LOG("Alarm::Exit\n");

	Alarm::AlarmFlag = FALSE;
	PCStateMachine::alarm=FALSE;
	
	FPGA::ResetFPGAOverloadClear();
	
	AlarmsReasonMask = NO_ERROR;
	LOG("ALARM:Exit\n");
}

/**************************************************************************//**
* \brief  - State loop code.
* \brief  - Alarm state at overload or weld error found. Sonics is stopped and waits until 
*    supervisor is in pre-ready state.
*
* \param  - none
*
*
* \return  - none.
*
******************************************************************************/
void Alarm::Loop()
{
	if (PC_RxPdo.MasterEvents & (UINT16) BIT_MASK(CTRL_PC_CLEAR_ALARMS))
	{
		if((AlarmsReasonMask & (UINT32)ERR_15V_LOST) == ERR_15V_LOST)
		{
			ADC::SetADCReInitRequest(TRUE);
		}
		AlarmsReasonMask = NO_ERROR;
	}
	
	if (AlarmsReasonMask == NO_ERROR)
	{		
		PC_TxPdo.PCState=PC_READY;
		ChangeState(PC_READY);
	}
}

/**************************************************************************//**
* \brief  - Update the mask with the TempAlarmReason Given.
*
* \param  - TempAlarmReason: New alarm to be set on the Mask
*
*
* \return  - none.
*
******************************************************************************/
void Alarm::SetAlarmReason(UINT32 TempAlarmReason)
{
	AlarmsReasonMask |= TempAlarmReason;
}

/**************************************************************************//**
* \brief  - Get the mask value.
*
* \param  - none
*
*
* \return  - none.
*
******************************************************************************/
UINT32 Alarm::GetAlarmReason()
{
	return AlarmsReasonMask;
}

/**************************************************************************//**
* \brief  - Sets the AlarmFlag true.
*
* \param  - none
*
*
* \return  - none.
*
******************************************************************************/
void Alarm::SetAlarmFlag()
{
	AlarmFlag = TRUE;
	
}

/**************************************************************************//**
* \brief  - Clears the AlarmFlag true.
*
* \param  - none
*
*
* \return  - none.
*
******************************************************************************/
void Alarm::ClearAlarmFlag()
{
	AlarmFlag = FALSE;
	
}


/**************************************************************************//**
* \brief  - Gets AlarmFlag status .
*
* \param  - none
*
*
* \return  - Alarm flag status.
*
******************************************************************************/
UINT8 Alarm::GetAlarmFlag()
{
	return (AlarmFlag);
	
}

/**************************************************************************//**
* \brief  - Clears overloads for FPGA and alarm reason mask
*
* \param  - none
*
*
* \return  - none.
*
******************************************************************************/
void Alarm::ClearOverloads()
{
	AlarmsReasonMask &= ~(ERR_TEMP_OVERLOAD | ERR_CURRENT_OVERLOAD |ERR_VOLTAGE_OVERLOAD | ERR_POWER_OVERLOAD);
	FPGA::ResetFPGAOverload();
}
