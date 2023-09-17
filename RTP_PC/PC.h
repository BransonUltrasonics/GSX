/*
 * PC.h
 *
 *  Created on: May 24, 2017
 *      Author: eguajardo
 */

#ifndef PC_H_
#define PC_H_

#include "PCStateMachine.h"
class PC
{
public:
	PC();
	~PC();
	static PCStateMachine stateMachine;
};



#endif /* PC_H_ */
