#ifndef INC_JOGUP_H_
#define INC_JOGUP_H_

#include <stdio.h>
#include <iostream>
#include "SCState.h"

using namespace std;

class JogUp :public SCState
{
public:
	JogUp();
	~JogUp();
	
protected:
	void Enter();
	void Loop();
	void Exit();
	
private:
	UINT32 m_TimerCount;
};


#endif /* INC_JOGUP_H_ */
