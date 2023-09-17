#pragma once
#include "ACState.h"
class GoingToPosition: public ACState {
public:
	GoingToPosition();
	~GoingToPosition();
protected:
	void Enter();
	void Loop();
	void Exit();
private:
	UINT16 motionDelay;
	INT32 targetPosition, top, bottom, start;
};

