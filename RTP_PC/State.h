/*
 * State.h
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#ifndef STATE_H_
#define STATE_H_

#include "Common.h"

class State
{
	friend class PCStateMachine;
public:
	State(){};
	virtual ~State(){};
protected:
	void virtual Enter() = 0;
	void virtual Loop() = 0;
	void virtual Exit() = 0;
};

#endif /* STATE_H_ */
