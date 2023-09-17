#pragma once
#include "State.h"
#include "Commons.h"

class ACState: public state {
	friend class ACStateMachine;
public:
	ACState();
	virtual ~ACState();
protected:
	ACSTATES stateType;
	void virtual Enter() = 0;
	void virtual Loop() = 0;
	void virtual Exit() = 0;
	void ChangeState(ACSTATES);
};

