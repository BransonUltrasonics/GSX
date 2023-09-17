/*
 * Ready.h
 *
 *  Created on: May 22, 2017
 *      Author: lzhu
 */

#ifndef READY_H_
#define READY_H_

#include <stdio.h>
#include <iostream>
#include "SCState.h"
#include "RunTimeFeature.h"
#include "ControlTask.h"

using namespace std;

class Ready :public SCState
{
	friend class SCStateMachine;
public:
	Ready();
	~Ready();
	UINT32 uiReadyLoopCounter;
	UINT32 SeekCounter;
	UINT32 TargetSeekCounter;
protected:
	void Enter();
	void Loop();
	void Exit();
	
private:
	void GetLocalCopyOD(void* ODObject);
	void ProcessLoop (OPERATION op);
	UINT16			m_AlarmData;
	AlarmActions	m_alarmAction;
	SystemConfiguration SysConfigClass;
	intf_SysConfig_t systemConfig;
	
	bool _flagSonicDisable;
	RunTime RT;
	Message message;
};

#endif /* READY_H_ */
