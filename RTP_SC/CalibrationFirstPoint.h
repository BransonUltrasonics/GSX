

#ifndef SRC_INC_CALIBRATIONFIRSTPOINT_H_
#define SRC_INC_CALIBRATIONFIRSTPOINT_H_



#include <stdio.h>
#include "SCState.h"

using namespace std;

class CalibrationFirstPoint :public SCState
{
	friend class SCStateMachine;
public:
	CalibrationFirstPoint();
	~CalibrationFirstPoint();
	
protected:
	void Enter();
	void Loop();
	void Exit();
private:
	bool onPart;
	void ProcessLoop ();
};



#endif /* SRC_INC_CALIBRATIONFIRSTPOINT_H_ */
