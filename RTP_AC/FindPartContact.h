#pragma once
#include "ACState.h"
class FindPartContact: public ACState {
public:
	FindPartContact();
	~FindPartContact();
protected:
	void Enter();
	void Loop();
	void Exit();
private:
	INT32 top, bottom, start;
	bool bFirstPartContact;
};

