
#ifndef SRC_AC_SERVO_RESET_H_
#define SRC_AC_SERVO_RESET_H_

#include "ACState.h"

class ServoReset: public ACState {
public:
	ServoReset();
	virtual ~ServoReset();
protected:
	void Enter();
	void Loop();
	void Exit();
private:
	UINT16 loopCount;
};

#endif /* SRC_AC_SERVO_RESET_H_ */
