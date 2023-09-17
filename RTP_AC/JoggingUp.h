
#ifndef SRC_AC_JOGGING_UP_H_
#define SRC_AC_JOGGING_UP_H_

#include "ACState.h"

class JoggingUp: public ACState {
public:
	JoggingUp();
	virtual ~JoggingUp();
protected:
	void Enter();
	void Loop();
	void Exit();
};

#endif /* SRC_AC_JOGGING_UP_H_ */
