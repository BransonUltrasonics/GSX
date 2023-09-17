/*
 * ServoTaskClock.cpp
 *
 *  Created on: May 14, 2018
 *      Author: slurie
 */

#include "ServoTaskClock.h"

TASK_ID ServoTaskClock::task_id;
USR_HANDLER ServoTaskClock::pAuxClkHandler;


/**************************************************************************//**
* \brief  - ISR for Aux clock to trigger the servo task by giving servoTaskSem
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void vxAuxClkIsr()
{ 
	static UINT16 dgt_timer = 0;
		
	semGive(servoTaskSem);
	
//polling Digital IO Inputs each ms
	if(dgt_timer > 9)
	{
		semGive(AcDgtInputTaskSem);
		dgt_timer = 0;
	}
	else
	{
		dgt_timer++;		
	}
}


/**************************************************************************//**
* \brief  - Timer Handler to trigger ISR from RTP
* 			
* \param  - void
*
*
* \return  - void
*
******************************************************************************/
void ServoTaskClock::TimerEvent_Handler()
{		
	for(;;)
    {  			
		if (eventReceive (VXEV01, EVENTS_WAIT_ANY, WAIT_FOREVER, NULL) == ERROR)
 		{
 			LOG("Event Receive failed!!!\n"); 				
 		}
		pAuxClkHandler();		//Calling User Timer Callback
		//printf("%s\n", __FUNCTION__);
    }
}


/**************************************************************************//**
* \brief  - Registers or Connects the Timer ISR
* 			
* \param  - pUsrHandler - Function Pointer to ISR
*
*
* \return  - Success or Failure
*
******************************************************************************/
STATUS ServoTaskClock::AuxClkRegister(USR_HANDLER pUsrHandler)
{	
	task_id = taskOpen("/TimerEvent0", 100, 0, OM_CREATE, NULL, 0x1000, 0, (FUNCPTR)TimerEvent_Handler,0,0,0,0,0,0,0,0,0,0);	
	pAuxClkHandler = pUsrHandler;		
	AuxClkConnect(task_id);		/* Register Timer ISR */
	return 0;
}


/**************************************************************************//**
* \brief  - Default constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
ServoTaskClock::ServoTaskClock() 
{

}


/**************************************************************************//**
* \brief  - Default destructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
ServoTaskClock::~ServoTaskClock() 
{

}


/**************************************************************************//**
* \brief  - Creates timer interrupt
* 			
* \param  - void
*
*
* \return  - Task ID of timer event handler task (-1 if init fails)
*
******************************************************************************/
INT32 ServoTaskClock::TimerInit() 
{
	AuxClkDisable();
	AuxClkRateSet(10000);
	AuxClkRegister(vxAuxClkIsr);
	AuxClkEnable();
	return task_id;
}
