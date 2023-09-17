/*
 * AcToReady.h
 *
 *  Created on: Apr 2, 2019
 *      Author: DShilonie
 */

#ifndef ACTOREADY_H_
#define ACTOREADY_H_


#include "SCState.h"

using namespace std;


class AcToReady :public SCState
{
	friend class SCStateMachine;
public:
	AcToReady();
	~AcToReady();
protected:
	
	void Enter	();
	void Loop	();
	void Exit	();
	
private:
	void ProcessAlarm (UINT8 org, UINT16 alarmID = 0);
	
};



#endif /* ACTOREADY_H_ */
