/*
 * AlarmManager.h
 *
 *  Created on: Mar 15, 2018
 *      Author: DShilonie
 */

#ifndef ALARMMANAGER_H_
#define ALARMMANAGER_H_

#include <lstLib.h>
#include "state.h"
#include "AlarmFunctions.h"

// various alarm actions
typedef enum ALARM_ACTION_COUNT
{
	COUNT_ALL,
	COUNT_RESET_REQ,
	COUNT_LOG_ALARM, 
	COUNT_GENERAL_OUTPUT,
	COUNT_CUSTOM_OUTPUT,
	COUNT_CYCLE_COUNT
} ALARM_ACTION_COUNT;

// counts for various alarm types
typedef enum ALARM_TYPE_COUNT
{
		COUNT_OVERLOAD,
		COUNT_CYCLE_MODIFIED,
		COUNT_WARNING,
		COUNT_PROCESS_SUSPECT,
		COUNT_PROCESS_REJECT,
		COUNT_HARDWARE_FAILURE,
		COUNT_NOCYCLE,
		COUNT_NOCYCLE_OVERLOAD
} ALARM_TYPE_COUNT;


// message buffer description used for prototyping 
typedef struct ALARM_DESCRIPTION {
	UINT16			id;
	char 			description[MAX_SIZE_OF_MSG_BUFF - sizeof (UINT16)];
	
} ALARM_DESCRIPTION, * pALARM_DESCRIPTION;

// alarm event information
typedef struct ALARM_INFO {
	UINT16						id;
	UINT16 						source;
	AlarmEvent::ALARM_PRIORITY 	priority;
	AlarmActions				flags;
	UINT16						alarmType;
	UINT16						scState;
	char *						description;
	char *						alarmLog;
	
} ALARM_INFO, * pALARM_INFO;

// alarm configuration in configurable actions
typedef struct AlarmConfiguration
{
	AlarmActions  OverloadAlarmActions;
	AlarmActions  CycleModifiedAlarmActions;
	AlarmActions  WarningAlarmActions;	
	AlarmActions  SuspectAlarmActions;
	AlarmActions  RejectAlarmActions;	
	AlarmActions  NoCycleAlarmActions;
	AlarmActions  HardwareFailureAlarmActions;
	AlarmActions  NoCycleOverloadAlarmActions;	
} AlarmConfiguration, * pAlarmConfiguration;


class AlarmMgr
{
public:

	typedef struct ALARM_EVENT {
		
		ALARM_EVENT (UINT16	id, 
				UINT16	type,
				UINT16 	source,
				AlarmEvent::ALARM_PRIORITY priority,
				AlarmActions flags,
				UINT16 scState,		
				char * description,
				char * alarmLog);
		
		~ALARM_EVENT ();

		NODE			alarmNode;
		AlarmEvent *  	pAlarmEvent;
		
	} ALARM_EVENT, * pALARM_EVENT;

	AlarmMgr ();
	~AlarmMgr ();
	
	static bool			EnterAlarmEvent			(UINT8 origin, UINT16 alarmID = 0);
	static void			ExitAlarmEvent			(UINT8 origin);
	static void			ExitPersistAlarmEvent	(UINT8 origin, UINT16 alarmID = 0);
	static bool			IsAlarmState			();
	static bool			IsAlarmInfo 			(UINT16 alarmID);
	
	static bool			IsCycleOK				();
	static void			ClearCycleOK			();
	static void			FirstWeldCycle			();
	
	bool 				SaveAlarmInfo			(ALARM_INFO& alarmInfo);
	bool 				GetAlarmInfo			(UINT16 alarmID, ALARM_INFO& alarmInfo);
	bool 				ClearAlarmInfo			(UINT16 alarmID);
	bool				CheckPersistentAlarm 	(UINT16 alarmID);
	void				IsPersistentAlarm		(ALARM_INFO& alarmInfo);
	void 				ClearAllAlarmInfo		();
	int 				ClearAlarmResetInput	();
	
	void				SetAlarmState			(UINT16 alarmID, AlarmEvent::ALARM_STATE stateBit);
	UINT16				GetAlarmState			(UINT16 alarmID);
	void				ClearAlarmState			(UINT16 alarmID, AlarmEvent::ALARM_STATE stateBit);

	static UINT16		GetAlarmActionCount		(ALARM_ACTION_COUNT action, UINT16 source = (ALARM_FROM) NO_SOURCE);
	static UINT16		GetAlarmCount			(UINT16 source = (ALARM_FROM) NO_SOURCE);
	UINT16				GetPersistentCount		(UINT16 source = (ALARM_FROM) NO_SOURCE);
	bool				GetLastAlarm 			(Message& msg);
	bool				GetAllActiveAlarms		(Message& msg);
	
	void				GetDefaultConfiguration (pAlarmConfiguration defaultConfig);
	void 				GetCurrentConfiguration	(pAlarmConfiguration alarmConfig);
	void 				SetCurrentConfiguration	(pAlarmConfiguration alarmConfig);
	void				GetAlarmActions			(UINT16 alarmType, UINT16 alarmId, AlarmActions& alarmAction);
	SCSTATES			GetAlarmSCState			(UINT16 source);
	
private:
	
	static void			GetActions				(UINT16 alarmType, UINT16 alarmId, AlarmActions& alarmAction);
	void				SetDefaultConfiguration (); 
	
	// processing regular alarms
	static bool			_alarmProcessingAc;
	static bool			_alarmProcessingPc;
	static bool			_alarmProcessingSc;
	
	// SC state at the time of the alarm
	static SCSTATES 	_alarmAcSCState;
	static SCSTATES 	_alarmPcSCState;
	
	// reset required
	static bool			_alarmAc;
	static bool			_alarmPc;
	static bool			_alarmSc;

	// processing persistent alarms
	static bool			_persistEstopAc;
	static bool			_persistAc;
	static bool			_persistPc;
	static bool			_persistSc;

	static UINT16		_weldAlarmCount;
	static bool			_firstWeldCycle;

	static AlarmConfiguration	_configurations[2];

	static LIST			_alarms;
	UINT16				_alarmMgrStatus;
	
	
}; 	

#endif /* ALARMMANAGER_H_ */
