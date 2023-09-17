#pragma once
#include "ACState.h"
class Ready: public ACState {
public:
	Ready();
	~Ready();
protected:
	void Enter();
	void Loop();
	void Exit();
};

