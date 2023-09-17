/*
 * AcToHome.h
 *
 *  Created on: Mar 21, 2019
 *      Author: DShilonie
 */

#ifndef ACTOHOME_H_
#define ACTOHOME_H_


#include "SCState.h"

using namespace std;


class AcToHome :public SCState
{
	friend class SCStateMachine;
public:
	AcToHome();
	~AcToHome();
protected:
	
	void Enter	();
	void Loop	();
	void Exit	();
	
private:
	void ProcessAlarm (UINT8 org, UINT16 alarmID = 0);
	
};



#endif /* ACTOHOME_H_ */
