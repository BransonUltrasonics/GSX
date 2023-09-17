#ifndef INC_JOGDOWN_H_
#define INC_JOGDOWN_H_

#include <stdio.h>
#include <iostream>
#include "SCState.h"

using namespace std;

class JogDown :public SCState
{
public:
	JogDown();
	~JogDown();
	
protected:
	void Enter();
	void Loop();
	void Exit();
	
private:
	UINT32 m_TimerCount;
};


#endif /* INC_JOGDOWN_H_ */
