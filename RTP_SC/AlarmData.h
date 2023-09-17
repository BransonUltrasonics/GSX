/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef ALARMDATA_H_
#define ALARMDATA_H_

#include "Common.h"
#include "AlarmFunctions.h"
#include <map>

typedef struct PCAlarmData
{
	UINT32 Overloads;
	UINT32 Alarms;
}PCAlarmData;

typedef struct ACAlarmData
{
	UINT32 ACAlarm;
	UINT8  ActuatorType;
	UINT8  JogSpeed;
	UINT8  GTPSpeed;
	UINT16 CalibrationForce;
	UINT16 FW_CRC;
	UINT32 GTPPosition;
	UINT32 SpringConstant;
	UINT32 ActuatorCycles;
	UINT32 CycleActuatorMaintenance;
	UINT32 CycleFullStroke;
	UINT32 FW_Version;	
}ACAlarmData;

class AlarmDataSC
{
public:	
	AlarmDataSC();
	~AlarmDataSC();
	
	void    		GetAlarmData(std::string date, std::string& alarmData);
	void 			SetAlarmData(UINT16 alarmId,UINT16 alarmType, AlarmActions alarmActions);
	UINT16			GetAlarmId() const;
	UINT16			GetAlarmtype() const;
	AlarmActions 	GetAlarmActions () const;
	void				ResetAllAlarmCounter();
	bool				GetAlarmFlag();
	void				SetAlarmFlag(bool alarmSet);
	void				SetBatchCountAlarmFlag(bool batchCountAlarm);
	bool				GetBatchCountAlarmFlag();
private:
	UINT16			m_AlarmType;
	UINT16 			m_AlarmId;
	UINT16 		    m_ResetAlarmCounter;
	UINT16  		m_NonResetAlarmCounter;
	bool			m_AlarmSet;
	bool			m_BatchCountAlarmFlag;
	AlarmActions	m_AlarmActions;
};

class AlarmDataPC
{
public:	
	AlarmDataPC();
	~AlarmDataPC();
	
	void    		GetAlarmData(std::string date, std::string& alarmData, char * alarmInfo);
	void  			SetAlarmData(char *pRecvBuffer);
	UINT32			GetAlarmId() const;
	PCAlarmData *	GetAlarmLog ();
	
private:
	PCAlarmData m_AlarmLog;
};

class AlarmDataAC
{
public:	
	AlarmDataAC();
	~AlarmDataAC();
	
	void    		GetAlarmData(std::string date, std::string& alarmData,  char * alarmInfo);
	void  			SetAlarmData(char *pRecvBuffer);
	UINT32			GetAlarmId() const;
	ACAlarmData * 	GetAlarmLog();
	
private:
	ACAlarmData m_AlarmLog;
};


#endif /* ALARMDATA_H_ */
