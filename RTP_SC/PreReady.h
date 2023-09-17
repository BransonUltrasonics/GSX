/*
 * PreReady.h
 *
 *  Created on: May 19, 2017
 *      Author: lzhu
 */

#ifndef INC_PREREADY_H_
#define INC_PREREADY_H_

#include <stdio.h>
#include <iostream>
#include "SCState.h"

using namespace std;

class PreReady :public SCState
{
	friend class SCStateMachine;
public:
	PreReady();
	~PreReady();
protected:
	
	void Enter();
	void Loop();
	void Exit();
	
private:

	void ProcessLoop ();
	bool bOneTimeSend;
	UINT32 	pbActiveTimeout;
	UINT32 	partPresentActiveTimeout;
	UINT8 	ui8input;
	Message message;
};



#endif /* INC_PREREADY_H_ */
