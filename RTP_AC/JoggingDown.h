
#ifndef SRC_AC_JOGGING_DOWN_H_
#define SRC_AC_JOGGING_DOWN_H_

#include "ACState.h"

class JoggingDown: public ACState {
public:
	JoggingDown();
	virtual ~JoggingDown();
protected:
	void Enter();
	void Loop();
	void Exit();
};

#endif /* SRC_AC_JOGGING_DOWN_H_ */
