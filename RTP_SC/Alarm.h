/*
 * Alarm.h
 *
 *  Created on: May 19, 2017
 *      Author: lzhu
 */

#ifndef INC_ALARM_H_
#define INC_ALARM_H_

#include <stdio.h>
#include "SCState.h"

class Alarm :public SCState
{
public:
	Alarm();
	~Alarm();
protected:
	virtual void Enter();
	virtual void Loop();
	virtual void Exit();
private:
	UINT32 moveDoneDelay;
};

#endif /* INC_ALARM_H_ */
