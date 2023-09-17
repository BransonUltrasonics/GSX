/*
 * ServoTaskClock.h
 *
 *  Created on: May 14, 2018
 *      Author: slurie
 */

#ifndef SERVOTASKCLOCK_H_
#define SERVOTASKCLOCK_H_

#include <vxworks.h>
#include <stdlib.h>
#include "AcCommon.h"

typedef void (*USR_HANDLER)();

class ServoTaskClock {
public:
	ServoTaskClock();
	virtual ~ServoTaskClock();
	static INT32 TimerInit();
private:
	static TASK_ID task_id;
	static USR_HANDLER pAuxClkHandler;
	static STATUS AuxClkRegister(USR_HANDLER pUsrHandler);
	static void TimerEvent_Handler();
};

#endif /* SERVOTASKCLOCK_H_ */
