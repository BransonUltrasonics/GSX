/*
 * AlarmFunctions.h
 *
 *  Created on: Mar 6, 2018
 *      Author: DShilonie
 *      
 *      Defines the various characteristics of an individual alarm event that
 *      is detected during a weld, setup process or seek/scan 
 */


#ifndef ALARMFUNCTIONS_H_
#define ALARMFUNCTIONS_H_

#include "Common.h"
#include "ControlTaskCommon.h"
#include "commons.h"

// *********** RBF
#define 	ALARMMGR_DBG

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define ALARMTYPE_MASK  	0x0F00
#define ALARMSUBTYPE_MASK  	0x00F0

// source generating the alarm
enum ALARM_FROM
{
	NO_SOURCE		= 0,
	ALARM_SC,
	ALARM_PC,
	ALARM_AC,
	
	MAX_SOURCE
};

// alarm configurable actions
typedef struct AlarmActions
{
	UINT8  ResetRequired		: 	1;
	UINT8  LogAlarm				: 	1; 
	UINT8  GeneralAlarm			: 	1;
	UINT8  CustomAlarmOutput	: 	1;
	UINT8  CycleCounter			: 	1;
	UINT8  Persistent			: 	1;
	UINT8  Reserved				: 	2;
} AlarmActions,  * pAlarmActions;

// types (groups) of alarms
enum ALARM_TYPE
{
		TYPE_OVERLOAD 			= 0,
		TYPE_CYCLE_MODIFIED		= 0x300,
		TYPE_WARNING 			= 0x400,
		TYPE_PROCESS_LIMIT		= 0x500,
		TYPE_HARDWARE_FAILURE	= 0x600,
		TYPE_NOCYCLE			= 0x700,
		TYPE_NOCYCLE_OVERLOAD 	= 0x800,
		TYPE_EN_FAULT			= 0xE00
} ALARM_TYPE;

// sub types within the group of alarms
enum ALARM_SUB_TYPE
{
	OVERLOAD_WELD 			= 0x000,
	OVERLOAD_ENERGY_BRAKE 	= 0x010,
	OVERLOAD_AFTER_BURST 	= 0x020,
	OVERLOAD_POST_WELD 		= 0x030,
	
	OVERLOAD_TEST 			= 0x040,
	OVERLOAD_PRE_TRIGGER	= 0x050,
	OVERLOAD_SEEK 			= 0x060,
	OVERLOAD_THERMAL		= 0x070,
	OVERLOAD_PRE_WELD_SEEK	= 0x080,

} ALARM_SUB_TYPE;


class AlarmEvent
{
public:

	typedef	enum
	{
		STATE_INACTIVE,
		STATE_ACTIVE,
		STATE_PENDING,
		STATE_CLEARED,

		STATE_MAX
	} ALARM_STATE;
	
	typedef	enum
	{
		PRIORITY_HIGHEST,
		PRIORITY_HIGH,
		PRIORITY_MEDIUM,
		PRIORITY_LOW,
		PRIORITY_LOWEST,

		PRIORITY_MAX
		
	} ALARM_PRIORITY;
	
	AlarmEvent (UINT16 id, UINT16 alarmType, UINT16 source, ALARM_STATE state, ALARM_PRIORITY priority, AlarmActions flags, UINT16 scState, char * description, char *alarmLog);
	
	virtual ~AlarmEvent	();
	
	UINT16			GetType 				();
	UINT16			GetID 					();
	ALARM_PRIORITY	GetAlarmPriority		();
	UINT16			GetAlarmSource 			();
	AlarmActions	GetFlags 				();
	SCSTATES		GetScState				();
	char * 			GetLog					();
	char * 			GetAlarmDescription		();
	void 			CopyAlarmLog			(Message& message);
	void 			CopyAlarmDescription	(Message& message);
	
	UINT16			GetAlarmState 			();
	void			SetAlarmState 			(ALARM_STATE state);
	void			ClearAlarmState 		(ALARM_STATE state);
	
protected:

	UINT16			_type;
	UINT16			_id;
	UINT16			_source;
	ALARM_PRIORITY 	_priority;
	UINT16			_scState;
	
	UINT16			_state;
	AlarmActions	_flags;
	
	char			_description[MAX_SIZE_OF_MSG_BUFF];
	char			_alarmLog[MAX_SIZE_OF_MSG_BUFF];
};


#endif /* ALARMFUNCTIONS_H_ */
