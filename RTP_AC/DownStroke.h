#pragma once
#include "ACState.h"
class DownStroke: public ACState {
public:
	DownStroke();
	~DownStroke();
protected:
	void Enter();
	void Loop();
	void Exit();
private:
	INT32 top, bottom, start;
};

