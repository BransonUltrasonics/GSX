/*
 * StartSwitch.h
 *
 *  Created on: May 19, 2017
 *      Author: lzhu
 */

#ifndef INC_STARTSWITCH_H_
#define INC_STARTSWITCH_H_

#include <stdio.h>
#include <iostream>
#include "SCState.h"

using namespace std;

class StartSwitch :public SCState
{
	friend class SCStateMachine;
public:
	StartSwitch();
	~StartSwitch();
	UINT32 Stagg_Time;
	UINT32 Stagg_Time_Counter;
	UINT32 uiStartSwitchCounter;
protected:
	void Enter();
	void Loop();
	void Exit();
	
private:
	void ProcessLoop (INT32 cmdID);
	UINT32 m_PartContactPos;
};



#endif /* INC_STARTSWITCH_H_ */
