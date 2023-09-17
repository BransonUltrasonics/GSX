/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef ALARM_H_
#define ALARM_H_

#pragma once
#include "ACState.h"

#ifndef INDEX_AC_ALARM 
#define INDEX_AC_ALARM    0x4006
#define NUM_OF_ALARM_ENTRY 		1
#endif


class Alarm : public ACState
{		
	public:
		Alarm();
		~Alarm();
		
		static void 	SetAlarmReason		(UINT32 TempAlarmReason);
		static void 	ClearAlarmReason	(UINT32 TempAlarmReason);
		static UINT32 	GetAlarmMask		();
	protected:
		void Enter();
		void Loop();
		void Exit();
		
		static UINT32 AlarmsReasonMask;
	private:
		UINT16 motionDelay;
		UINT32 ReadyPosition;
		UINT32 target;
		bool bStackReturnedFlag;
		bool bDoNotSetTarget;
};

#endif /* ALARM_H_ */
