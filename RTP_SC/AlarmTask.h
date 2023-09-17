/*
 * AlarmTask.h
 *
 *  Created on: Mar 9, 2018
 *      Author: DShilonie
 *      
 *      The alarm task will receive messages from the control task, SDO task and possibly the database task, whenever an
 *      alarm is raised and will process them. Processing could include requesting additional information from the source
 *      of the alarm, saving information in the DB and notifying the UIC. It will do so by using member alarm manager which
 *      encapsulates the management of the alarm event list. 
 *      
 */

#ifndef ALARMTASK_H_
#define ALARMTASK_H_

#include  "Common.h"
#include  "CommonProperty.h"
#include  "SC_Common.h"
#include  "ControlTaskCommon.h"
#include "AlarmManager.h"

class AlarmTask
{
    
public:
	AlarmTask();
	~AlarmTask();
	
	void			ProcessCommandMessage		();
	bool			bIsTaskRunStatus			();
	
private:
	EC_MASTER_STATE EcatGetMasterStateSysCall	();
	
	void			ProcessAlarmEvent			(ALARM_INFO& alarm_info);
	void			ProcessAlarmEstop			(ALARM_INFO& alarm_info);
	void 			ProcessAlarmInfo 			(UINT16 source, ALARM_INFO& alarm_info);
	void 			ProcessAlarmReset 			(ALARM_INFO& alarm_info);
	void 			ProcessEstopReset 			(ALARM_INFO& alarm_info);
	void 			ProcessResetAll				(UINT32 msgID);
	void 			ProcessResetInput			();
	bool			ReceiveAlarmEventUI			(INT32	msgID);
	STATUS			SendAlarmEventUI			(INT32	msgID, char * altMsg = NULL, _Vx_ticks_t waitType = NO_WAIT);
	STATUS			SendEstopEventUI			(UINT32 msgID);
	void			ProcessAlarmID				(UINT16 source, UINT16 id, ALARM_INFO& alarm_info);
	void			ProcessAlarmType			(UINT16 source, ALARM_INFO& alarm_info);
	void			ProcessAlarmUnknown			(ALARM_INFO& alarm_info);
	STATUS			SendAlarmDataToDataBase		(ALARM_INFO& alarm_info);
	STATUS			SendToMsgQ					(Message& msgBuffer, const MSG_Q_ID &msgQID, _Vx_ticks_t waitType);
	void			DeCoder						(char * recvBuffer, Message& MsgDecoder);

	MSG_Q_ID 		DB_MSG_Q_ID;
	MSG_Q_ID 		SDO_MSG_Q_ID;
	MSG_Q_ID 		CTRL_MSG_Q_ID;
	MSG_Q_ID 		ALARM_MSG_Q_ID;
	MSG_Q_ID 		UI_MSG_Q_ID;
	MSG_Q_ID		MAINTENANCE_MSG_Q_ID;
	
	AlarmMgr		alarmMgr;

	char 			recvMsgBuffer[MAX_SIZE_OF_MSG_LENGTH];
	Message 		message;
	CommonProperty 	*CP;
};


#endif /* ALARMTASK_H_ */
