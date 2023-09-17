/*
 * SonicOn.h
 *
 *  Created on: May 19, 2017
 *      Author: lzhu
 */

#ifndef INC_SONICON_H_
#define INC_SONICON_H_

#include <stdio.h>
#include <iostream>
#include "SCState.h"

#define MAX_SONICS_DELAY 10000 / ECAT_CYCLE_TIME_IN_US //10ms timeout for PC to enter sonic on state

using namespace std;

class SonicOn :public SCState
{
	friend class SCStateMachine;
public:
	SonicOn();
	virtual ~SonicOn();
	UINT32 Index;
	
protected:
	virtual void Enter();
	virtual void Loop();
	virtual void Exit();
	
	UINT32 SonicsOnCounter;
	UINT32 sonicsDelayCounter;
};




#endif /* INC_SONICON_H_ */
