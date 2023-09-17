#pragma once
#include "ACState.h"
class ReturnStroke: public ACState {
public:
	ReturnStroke();
	~ReturnStroke();
protected:
	void Enter();
	void Loop();
	void Exit();
private:
	UINT16 motionDelay;
	UINT16 tooSoon;
	INT32 readyPosition;
};

