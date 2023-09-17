/*************************************************************************** 

       Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************
 * Estop.h
 *
 *  Created on: Jun 22, 2018
 *      Author: DShilonie
 */

#ifndef ESTOP_H_
#define ESTOP_H_

#include "ACState.h"

class Estop : public ACState
{		
	public:
	Estop();
		~Estop();
		
	protected:
		void Enter();
		void Loop();
		void Exit();
};


#endif /* ESTOP_H_ */
