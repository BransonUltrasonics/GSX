/*
 * SC.h
 *
 *  Created on: May 23, 2017
 *      Author: lzhu
 */

#ifndef SRC_INC_SC_H_
#define SRC_INC_SC_H_

#include "stdio.h"
#include "SCStateMachine.h"

class SC
{
public:
	SC();
	~SC();
	static SCStateMachine stateMachine;
};





#endif /* SRC_INC_SC_H_ */
