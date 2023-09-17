

#ifndef SRC_INC_CALIBRATIONSECONDPOINT_H_
#define SRC_INC_CALIBRATIONSECONDPOINT_H_



#include <stdio.h>
#include "SCState.h"
#include "DataBaseManager.h"

using namespace std;

class CalibrationSecondPoint :public SCState
{
	friend class SCStateMachine;
public:
	CalibrationSecondPoint();
	~CalibrationSecondPoint();
	
protected:
	void Enter();
	void Loop();
	void Exit();
private:
	void 	ProcessLoop ();
	STATUS 	EventLog (LOG_EVENT eventID, UINT32 prevVal = 0, UINT32 newVal = 0);
};



#endif /* SRC_INC_CALIBRATIONSECONDPOINT_H_ */
