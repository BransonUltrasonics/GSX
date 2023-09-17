/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2019
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef SRC_AC_HOMEING_H_
#define SRC_AC_HOMEING_H_
#define HOME_MASK 0x80

#include "ACState.h"

class Homeing: public ACState {
public:
	Homeing();
	virtual ~Homeing();
protected:
	void Enter();
	void Loop();
	void Exit();
private:
	UINT8 direction;
	UINT16 overshoot, settleTime;
	bool goBackUp;
};

#endif /* SRC_AC_HOMEING_H_ */
