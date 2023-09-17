#pragma once
#include "ACState.h"


class ForceCalibration: public ACState {
public:
	ForceCalibration();
	~ForceCalibration();
protected:
	void Enter();
	void Loop();
	void Exit();
private:
	INT32 top, bottom, start;
};

