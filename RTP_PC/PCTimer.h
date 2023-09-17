/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef PCTIMER_H_
#define PCTIMER_H_

#pragma once
extern "C" 
{
#include <customSystemCall.h>
}
#include<eventLib.h>
#include"PCControlTask.h"

typedef void (*USR_HANDLER)(int);

#define INTERRUPT_FREQUENCY 1000

/* Variables and Function declarations */
TASK_ID task_id, clkTaskID;
USR_HANDLER pAuxClkHandler;
int AuxClkRegister(USR_HANDLER pUsrHandler);

void timer_handler(timer_t t ,int arg);

class PCTimer
{
	public:
		PCTimer();
		~PCTimer();
		static INT32 Timer_Init();		
	private:
		timer_t time;
		struct itimerspec timer_attr;
};


#endif /* PCTIMER_H_ */
