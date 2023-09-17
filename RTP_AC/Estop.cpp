/*************************************************************************** 

       Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************
 * Estop.cpp
 *
 *  Created on: Jun 22, 2018
 *      Author: DShilonie
 */

#include "Alarm.h"
#include "Estop.h"
#include "AC.h"
#include "DynamicSystemCall.h"
#include "Commons.h"
#include "ACStateMachine.h"


Estop::Estop()
{
	stateType = AC_ESTOP;
}

Estop::~Estop()
{
}

void Estop::Enter()
{
	printf("\n AC Estop Enter\n");

}

void Estop::Loop()
{
	 if (!ACStateMachine::GetEstop())
	 {
		 printf("\n AC Estop: ChangeState SERVO_RESET\n");
		 ChangeState(SERVO_RESET);
	 }
}

void Estop::Exit()
{
}

