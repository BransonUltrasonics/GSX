/*
 * Alarm.h
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#ifndef ALARM_H_
#define ALARM_H_

#include "PCState.h"


#ifndef INDEX_PC_ALARM 
#define INDEX_PC_ALARM    0x2002
#define NUM_OF_ALARM_ENTRY 		1
#endif

#define STRUCT_PACKED __attribute__((packed))


class Alarm : public PCState
{
	
	public:
		Alarm();
		~Alarm();
		static void SetAlarmReason(UINT32 TempAlarmReason);
		static UINT32 GetAlarmReason();
		static void SetAlarmFlag();
		static void ClearAlarmFlag();
		static UINT8 GetAlarmFlag();
		static void ClearOverloads();
	protected:
		void Enter();
		void Loop();
		void Exit();
		
	private:

		static UINT8 AlarmFlag;
		static UINT32 AlarmsReasonMask;
		
};

#endif /* ALARM_H_ */
