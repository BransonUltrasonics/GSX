/*
 * WaitForTrigger.h
 *
 *  Created on: May 19, 2017
 *      Author: lzhu
 */

#ifndef INC_WAITFORTRIGGER_H_
#define INC_WAITFORTRIGGER_H_

#include <stdio.h>
#include <iostream>
#include "SCState.h"

using namespace std;

class WaitForTrigger :public SCState
{
	friend class SCStateMachine;
public:
	WaitForTrigger();
	~WaitForTrigger();
	UINT32 WaitForTriggerTimeCounter;
	UINT32 TriggerDistance;
	UINT32 TriggerForce;
	UINT32 WaitForTriTimeoutCounter;
	UINT32 WaitForTriTimeBegin;
	UINT32 WaitForTriTimeEnd;
	
protected:
	void Enter();
	void Loop();
	void Exit();

private:
	bool  PretriggerActiveFlag;
	bool  PretriggerFlag;
	INT32 PretriggerType;
	INT32 PretriggerAmplitude;
	INT32 PretriggerDistance;
	
	//added by int
	Trigger preTrigger;
	UINT16 m_WeldAmplitude;
	Message message;
	
	void CheckSonicsON();
	void ProcessAlarm (UINT8 org, UINT16 alarmID = 0);
	/* virtual member function used for signature data */
	void CaptureWeldData();
};



#endif /* INC_WAITFORTRIGGER_H_ */
