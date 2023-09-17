/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     It is used to set and get Alarm data  
 
**********************************************************************************************************/

#include <iostream>
#include <string>
#include "AlarmData.h"
#include "AlarmManager.h"
#include "CommonProperty.h"
#include "ControlTask.h"

using namespace std;

/**************************************************************************//**
* \brief   - Initialization of class members and memory for object.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
AlarmDataSC::AlarmDataSC() : m_ResetAlarmCounter(0), m_BatchCountAlarmFlag(false), m_AlarmSet(false)
{
}

/**************************************************************************//**
* \brief   - Returns the alarm id.
*
* \param   - None.
*
* \return  - UINT16
*
******************************************************************************/
UINT16 AlarmDataSC::GetAlarmId() const
{
	return m_AlarmId;
}

/**************************************************************************//**
* \brief   - Returns the alarm Type.
*
* \param   - None.
*
* \return  - UINT16
*
******************************************************************************/
UINT16 AlarmDataSC::GetAlarmtype() const
{
	return m_AlarmType;
}

/**************************************************************************//**
* \brief   - Returns the alarm action.
*
* \param   - None.
*
* \return  - AlarmActions
*
******************************************************************************/
AlarmActions AlarmDataSC::GetAlarmActions () const
{
	return m_AlarmActions;
}


/**************************************************************************//**
* \brief   - Prepares alarm data to insert into DB
*
* \param   - string date, string& alarmData
*
* \return  - None
*
******************************************************************************/
void AlarmDataSC::GetAlarmData(string date, string& alarmData)
{
	char insertQuery[MSG_SIZE] = {0x00};
	UINT32 recipeNum    = CommonProperty::weldResult.Get(WeldResults::RECIPE_NUM);
	UINT32 recipeRevNum = CommonProperty::weldResult.Get(WeldResults::RECIPE_REV_NUM);
	UINT32 cycleCounter = CommonProperty::weldResult.Get(WeldResults::CYCLE_COUNTER);
	string user 		= (CommonProperty::weldResult.GetUserName() == "ConsoleUser" || CommonProperty::weldResult.GetUserName() == "0") ? "N/A" : CommonProperty::weldResult.GetUserName();
	
	if((TYPE_NOCYCLE == GetAlarmtype()) || (TYPE_HARDWARE_FAILURE == GetAlarmtype()))
	{
		cycleCounter = 0;
	}

	sprintf (insertQuery,"INSERT INTO AlarmLogs(RecipeNumber,AlarmID,UserName,AlarmType,AlarmSubType,DateTime,CycleCounter,RecipeVerNumber) VALUES(%d,'%X','%s','%X','%X','%s',%d,%d);",
			recipeNum, GetAlarmId(), (char *)user.c_str(), GetAlarmtype(), 0, (char *)date.c_str(),cycleCounter,recipeRevNum);
	
	alarmData = insertQuery;
}

/**************************************************************************//**
* \brief   - Set the alarm id and type
*
* \param   - UINT8 alarmId, UINT8 alarmTypea
*
* \return  - None
*
******************************************************************************/
void AlarmDataSC::SetAlarmData(UINT16 alarmId,UINT16 alarmType, AlarmActions alarmActions)
{
	m_AlarmType	= alarmType;
	m_AlarmId	= alarmId;
	m_AlarmActions = alarmActions;
}

/**************************************************************************//**
* \brief   - Gets the the AlarmSet variable
*
* \param   - None
*
* \return  - bool - status
*
******************************************************************************/
bool AlarmDataSC::GetAlarmFlag()
{
	return m_AlarmSet; 
}

/**************************************************************************//**
* \brief   - Sets the the Alarm Flag variable
*
* \param   - bool AlarmSet
*
* \return  - bool - status
*
******************************************************************************/
void AlarmDataSC::SetAlarmFlag(bool AlarmSet)
{
	m_AlarmSet = AlarmSet; 
}

/**************************************************************************//**
* \brief   - Gets the the Batch Count alarm flag  variable
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
bool AlarmDataSC::GetBatchCountAlarmFlag()
{
	return m_BatchCountAlarmFlag; 
}

/**************************************************************************//**
* \brief   - Sets the the Batch count Alarm Flag variable
*
* \param   - bool batchCountAlarm
*
* \return  - None
*
******************************************************************************/
void AlarmDataSC::SetBatchCountAlarmFlag(bool batchCountAlarm)
{
	m_BatchCountAlarmFlag = batchCountAlarm; 
}

/**************************************************************************//**
* \brief   - Memory release of object.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
AlarmDataSC::~AlarmDataSC()
{
}

/**************************************************************************//**
* \brief   - Initialization of class members and memory for object.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
AlarmDataPC::AlarmDataPC()
{

}

/**************************************************************************//**
* \brief   - Returns the alarm id.
*
* \param   - None
*
* \return  - UINT32
*
******************************************************************************/
UINT32 AlarmDataPC::GetAlarmId() const
{
	return m_AlarmLog.Overloads;
}

/**************************************************************************//**
* \brief   - Validate alarm ID & Prepares data to insert into DB.
*
* \param   - string date, string& alarmData
*
* \return  - None
*
******************************************************************************/
void AlarmDataPC::GetAlarmData(string date, string& alarmData, char * alarmInfo)
{
	char insertQuery[MSG_SIZE] = {0x00};
	UINT32 recipeNum = CommonProperty::weldResult.Get(WeldResults::RECIPE_NUM);
	UINT32 recipeRevNum = CommonProperty::weldResult.Get(WeldResults::RECIPE_REV_NUM);
	UINT32 cycleCounter = 0;
	string user 		= (CommonProperty::weldResult.GetUserName() == "ConsoleUser" || CommonProperty::weldResult.GetUserName() == "0") ? "N/A" : CommonProperty::weldResult.GetUserName();
	
	if((WELD == ControlTask::GetPrevOperationType()) && (TYPE_NOCYCLE_OVERLOAD != ((pALARM_INFO) alarmInfo)->alarmType) && (TYPE_HARDWARE_FAILURE != ((pALARM_INFO) alarmInfo)->alarmType))
	{
		cycleCounter = CommonProperty::weldResult.Get(WeldResults::CYCLE_COUNTER);
	}

	sprintf (insertQuery,"INSERT INTO AlarmLogs(RecipeNumber,AlarmID,UserName,AlarmType,AlarmSubType,DateTime,CycleCounter,RecipeVerNumber) VALUES(%d,'%X','%s','%X','%X','%s',%d,%d);",
			recipeNum, ((pALARM_INFO) alarmInfo)->id, (char *)user.c_str(), ((pALARM_INFO) alarmInfo)->alarmType, (((pALARM_INFO) alarmInfo)->id & ALARMSUBTYPE_MASK) >> 8 , (char *)date.c_str(),cycleCounter,recipeRevNum);

	alarmData = insertQuery;
}

/**************************************************************************//**
* \brief   - Assign AlarmDataPC values to member data.
*
* \param   - char *pRecvBuffer
*
* \return  - None
*
******************************************************************************/
void AlarmDataPC::SetAlarmData(char *pRecvBuffer)
{
	memcpy(&m_AlarmLog,pRecvBuffer,sizeof(m_AlarmLog));
}

PCAlarmData * AlarmDataPC::GetAlarmLog()
{
	return &m_AlarmLog;
}


/**************************************************************************//**
* \brief   - Memory release of object.
*
* \param   - None
*
* \return  - None
*
*****************************************************************************/
AlarmDataPC::~AlarmDataPC()
{
}

/**************************************************************************//**
* \brief   - Initialization of class members and memory for object.
*
* \param   - None
*
* \return  - None
*
*****************************************************************************/
AlarmDataAC::AlarmDataAC()
{

}

/**************************************************************************//**
* \brief   - Returns the alarm id.
*
* \param   - None
*
* \return  - UINT32
*
*****************************************************************************/
UINT32 AlarmDataAC::GetAlarmId() const
{
	return m_AlarmLog.ACAlarm;
}

/**************************************************************************//**
* \brief   - Validate alarm ID & Prepares data to insert into DB.
*
* \param   - string date, string& alarmData
*
* \return  - None
*
*****************************************************************************/
void AlarmDataAC::GetAlarmData(string date, string& alarmData,  char * alarmInfo)
{
	char insertQuery[MSG_SIZE] = {0x00};
	UINT32 recipeNum = CommonProperty::weldResult.Get(WeldResults::RECIPE_NUM);
	UINT32 recipeRevNum = CommonProperty::weldResult.Get(WeldResults::RECIPE_REV_NUM);
	UINT32 cycleCounter = 0;
	string user 		= (CommonProperty::weldResult.GetUserName() == "ConsoleUser" || CommonProperty::weldResult.GetUserName() == "0") ? "N/A" : CommonProperty::weldResult.GetUserName();

	/* Required more info on AlarmType or category from AlarmManager implementation to identify AC alarms that are raised after trigger */
	if(WELD == ControlTask::GetPrevOperationType())
	{
		cycleCounter = CommonProperty::weldResult.Get(WeldResults::CYCLE_COUNTER);
	}

	sprintf (insertQuery,"INSERT INTO AlarmLogs(RecipeNumber,AlarmID,UserName,AlarmType,AlarmSubType,DateTime,CycleCounter,RecipeVerNumber) VALUES(%d,'%X','%s','%X','%X','%s',%d,%d);",
			recipeNum, ((pALARM_INFO) alarmInfo)->id, (char *)user.c_str(), ((pALARM_INFO) alarmInfo)->alarmType, 0, (char *)date.c_str(),cycleCounter,recipeRevNum);
	
	alarmData = insertQuery;
}

/**************************************************************************//**
* \brief   - Assign AlarmDataAC values to member data.
*
* \param   - char *pRecvBuffer
*
* \return  - None
*
*****************************************************************************/
void AlarmDataAC::SetAlarmData(char *pRecvBuffer)
{
	memcpy(&m_AlarmLog,pRecvBuffer,sizeof(m_AlarmLog));
}

ACAlarmData * AlarmDataAC::GetAlarmLog()
{
	return &m_AlarmLog;
}


/**************************************************************************//**
* \brief   - Memory release of object.
*
* \param   - None
*
* \return  - None
*
*****************************************************************************/
AlarmDataAC::~AlarmDataAC()
{
}
