#ifndef STATE_H
#define STATE_H

#include "Common.h"

class state {
	friend class ACStateMachine;
public:
	state() {
	}
	;
	virtual ~state() {
	}
	;
protected:
	void virtual Enter() = 0;
	void virtual Loop() = 0;
	void virtual Exit() = 0;
};

#endif
