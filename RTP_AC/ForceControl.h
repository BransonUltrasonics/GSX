#pragma once
#include "ACState.h"
class ForceControl: public ACState {
public:
	ForceControl();
	~ForceControl();
protected:
	void virtual Enter();
	void virtual Loop();
	void virtual Exit();
private:
	UINT16 GetStepForce();
	UINT16 GetStepRampTime();
	void LookForForceStep();
	void ControlForce();
	INT32 top, bottom;
	bool rampDone;
	enum forceSteps { START, STEP1, STEP2, STEP3, STEP4, STEP5, STEP6, STEP7, STEP8, STEP9, STEP10, HOLD_FORCE };
	UINT8 forceStep;
	UINT16 rampCounter, rampStartForce;
};

