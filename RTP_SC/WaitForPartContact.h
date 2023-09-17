/*
 * WaitForPartContact.h
 *
 *  Created on: May 30, 2017
 *      Author: lzhu
 */

#ifndef SRC_INC_WAITFORPARTCONTACT_H_
#define SRC_INC_WAITFORPARTCONTACT_H_



#include <stdio.h>
#include <iostream>
#include "SCState.h"
#include "DataBaseManager.h"

using namespace std;

UINT32 SetUpPartContactPos;
class WaitForPartContact :public SCState
{
	friend class SCStateMachine;
public:
	WaitForPartContact();
	~WaitForPartContact();
	static UINT32 getSetupPartContactPos();
	
protected:
	void Enter();
	void Loop();
	void Exit();
	
private:
	STATUS EventLog (LOG_EVENT eventID, UINT32 prevVal, UINT32 newVal);
	
};



#endif /* SRC_INC_WAITFORPARTCONTACT_H_ */
