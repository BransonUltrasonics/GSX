/*
 * State.h
 *
 *  Created on: May 19, 2017
 *      Author: lzhu
 */

#ifndef HEADER_FILES_STATE_H_
#define HEADER_FILES_STATE_H_

#include <stdio.h>
#include "vxWorks.h"
#include "ControlTaskCommon.h"
#include "Common.h"


class state
{
	friend class SCStateMachine;
public:
	state(){};
	virtual ~state(){};
protected:
	virtual void  Enter() = 0;
	virtual void Loop() = 0;
	virtual void Exit()= 0;
};





#endif /* HEADER_FILES_STATE_H_ */
