/*
 * WeldSonicOn.h
 *
 *  Created on: May 31, 2017
 *      Author: lzhu
 */

#ifndef SRC_INC_WELDSONICON_H_
#define SRC_INC_WELDSONICON_H_

#define SONICS_LED_GPIO_TEST
#define TIMER_INTERVAL_250_MS 1000

#include <stdio.h>
#include <iostream>
#include "SonicOn.h"
#include "WeldRecipe.h"
#include "SysConfigEeprom.hpp"

using namespace std;

class WeldSonicOn :public SonicOn
{
	friend class SCStateMachine;
public:
	WeldSonicOn();
	~WeldSonicOn();
protected:
	void Enter();
	void Loop();
	void Exit();
private:
	UINT32 			EnergyinMilliJoules;
	UINT32 			EnergyNeededinMilliJoules;
	UINT32 			uiWeldSonicOnCounter;
	UINT16 			NewMaxWeldForce;
	WELDMODES 		modes;
	UINT32 			ScrubTime;
	UINT32 			ScrubTimeCounter;
	INT32  			modeValue;
	INT32			ScrubAmplitude;
	Dynamic			DynamicParams;
	INT32			Reactivity;
	INT32			ReactivityCounter;
	INT32			ReactivityFinishFlag;
	bool			ForceLevelEnabled;
	INT32			ForceLevel;
	INT32			ForceLevelCounter;
	INT32 			ForceLevelTime;
	bool			ForceLevelFinishFlag;
	INT32			WeldForce;
	UINT32 			StopFrequency;
	UINT32			FrequencyChange;
	UINT16 			currentForceStep;
	UINT16 			numForceSteps;
	UINT8  			m_NumAmplitudeSteps;
	UINT8  			m_AmplitudeStepAt;
	INT32			m_MaxWeldTimeoutCount;
	STEPPINGMODE 	forceStepAt;
	UINT32  		forceStepAtValue[MAX_NO_OF_FORCE_STEP];
    UINT16 			StepCounter;
    UINT16			TriggerLostCounter;
    bool 			AmplitudeSteppingFlag;
    SystemConfiguration SysConfig;
    intf_SysConfig_t 	sysConfigData;
    MSG_Q_ID		m_UI_MSG_Q_ID;

	/* virtual member function used for signature data */
	void CaptureWeldData();
#ifdef SONICS_LED_GPIO_TEST	
	void TurnonLed			();
	void TurnoffLed			();
#endif //SONICS_LED_GPIO_TEST
	
	void StateTrans			();
	void CheckForForceStep	();
	void AmplitudeStepping	();
	void ProcessLoop 		();
	void ProcessAlarm		(UINT8 org, UINT16 alarmID = 0);
	void SendMsgToUIMsgQ	(UINT16 power_data);
	bool CutoffLimitsValidation(); 
};



#endif /* SRC_INC_WELDSONICON_H_ */
