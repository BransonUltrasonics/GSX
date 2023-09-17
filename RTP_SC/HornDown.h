
#ifndef HORNDOWN_H_
#define HORNDOWN_H_

#include <stdio.h>
#include <iostream>
#include "SCState.h"

using namespace std;

class HornDown : public SCState
{
	friend class SCStateMachine;
public:
	HornDown();
	~HornDown();
	
protected:
	void Enter();
	void Loop();
	void Exit();

private:
	void ProcessLoop ();
	
	bool onPart;
	UINT32 m_TimerCount;
};

#endif /* HORNDOWN_H_ */
