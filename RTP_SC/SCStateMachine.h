/*
 * SCStateMachine.h
 *
 *  Created on: May 19, 2017
 *      Author: lzhu
 */

#ifndef INC_SCSTATEMACHINE_H_
#define INC_SCSTATEMACHINE_H_

#include "stdio.h"
#include "Powerup.h"
#include "PreReady.h"
#include "Ready.h"
#include "StartSwitch.h"
#include "WaitForTrigger.h"
#include "WeldSonicOn.h"
#include "Hold.h"
#include "WaitForReadyPosition.h"
#include "Alarm.h"
#include "WaitForPartContact.h"
#include "SeekSonicOn.h"
#include "ScanSonicOn.h"
#include "CalibrationFirstPoint.h"
#include "CalibrationSecondPoint.h"
#include "TestSonicsOn.h"
#include "HornDown.h"
#include "JogDown.h"
#include "JogUp.h"
#include "HornClampAlarm.h"
#include "AcToHome.h"
#include "AcToReady.h"
#include "DataBaseManager.h"


class SCStateMachine
{ 
	friend class SCState;
public:
	SCStateMachine();
	~SCStateMachine();
	void 				RunStateMachine	();
	SCSTATES 			GetStateType	();
	UINT32 				CycleCounter  	();
	void				ResetCycleTimeCounter();
	
	
protected:
	void 				ChangeState		(SCSTATES);
	 	
private:
	
	void				CheckSonicActiveOutput		();
	void				CheckHomePositionOutput		();
	void				CheckReadyPositionOutput	();
	STATUS 				EventLog 					(LOG_EVENT eventID, UINT32 prevVal = 0, UINT32 newVal = 0);

	
	SCState * 			currentState;
	
	Powerup 			PowerupObj;
	PreReady 			PreReadyObj;
	Ready 				ReadyObj;
	StartSwitch 		StartSwitchObj; 
	WaitForTrigger 		WaitForTriggerObj;
	WeldSonicOn 		WeldSonicOnobj;
	Hold 				Holdobj;
	WaitForReadyPosition WaitForReadyPosobj;
	Alarm 				Alarmobj;
	WaitForPartContact 	WaitForPartContactobj;
	SeekSonicOn 		SeekSonicOnobj;
	ScanSonicOn         ScanSonicOnObj;
	CalibrationFirstPoint CalibrationFirstPointObj;
	CalibrationSecondPoint CalibrationSecondPointObj;
	TestSonicOn			TestSonicOnObj;
	HornDown			HornDownObj;
	JogDown				JogDownObj;
	JogUp				JogUpObj;
	HornClampAlarm		HornClampAlarmObj;
	AcToHome			AcToHomeObj;
	AcToReady			AcToReadyObj;
	UINT32 				SlaveTimeoutCounter;
};





#endif /* INC_SCSTATEMACHINE_H_ */
