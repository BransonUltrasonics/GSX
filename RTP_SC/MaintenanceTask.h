/*
  * MaintenanceTask.h
 *
 *  Created on: May 26, 2020
 *      Author: DShilonie
 */

#ifndef MAINTENANCETASK_H_
#define MAINTENANCETASK_H_

#include  "Common.h"
#include  "CommonProperty.h"
#include  "SC_Common.h"
#include  "ControlTaskCommon.h"

class MaintenanceTask
{
    
public:
	MaintenanceTask();
	~MaintenanceTask();
	
	void			ProcessCommandMessage		();
	bool			bIsTaskRunStatus			();
	
private:
	EC_MASTER_STATE EcatGetMasterStateSysCall	();
	
	MSG_Q_ID 		DB_MSG_Q_ID;
	MSG_Q_ID 		SDO_MSG_Q_ID;
	MSG_Q_ID 		CTRL_MSG_Q_ID;
	MSG_Q_ID 		ALARM_MSG_Q_ID;
	MSG_Q_ID 		UI_MSG_Q_ID;
	MSG_Q_ID 		MAINTENANCE_MSG_Q_ID;

	char 			recvMsgBuffer[MAX_SIZE_OF_MSG_LENGTH];
	Message 		message;
	CommonProperty 	*CP;
};

#endif /* MAINTENANCETASK_H_ */
