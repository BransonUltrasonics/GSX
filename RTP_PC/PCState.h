/*
 * PCState.h
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#ifndef PCSTATE_H_
#define PCSTATE_H_

#include "State.h"


class PCState : public State
{
	friend class PCStateMachine;
public:
	
	PCState();
	virtual ~PCState();
	
protected:
	PCSTATES stateType;
	void virtual Enter() = 0;
	void virtual Loop() = 0;
	void virtual Exit() = 0;
	void ChangeState(PCSTATES);
};



#endif /* PCSTATE_H_ */
