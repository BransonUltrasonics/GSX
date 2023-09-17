/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     It contains  Periodic timer to send Event to FPGA Task
 
**********************************************************************************************************/
#include"PCTimer.h"
#include <sysLib.h>
#include "TimerInterval.h"
using namespace std;
UINT8 oneMilliTimer = 0;




/**************************************************************************
* \brief  - vxAuxClkIsr
* 			This function will be responsible for sending the event 
*			to task with Id TaskId
* \param  - TaskId
*
*
* \return  - void
*
******************************************************************************/

void vxAuxClkIsr(INT32 Task_Id)
{ 
	eventSend(TaskId[1],EVENT_CTRL_TASK);
		
}



/**************************************************************************
* \brief  - PCTimer
* 			Constructor for PCTimer object
* 			
* \param  - void
*
*
* \return  - void
*
******************************************************************************/
PCTimer::PCTimer()
{
	#ifdef PRINT
		LOG("----------------------- In PCtimer Constructor ------------------------\n\n");
	#endif
}


/**************************************************************************
* \brief  - ~PCTimer
* 			Destructor for PCTimer object
* 			
* \param  - void
*
*
* \return  - void
*
******************************************************************************/
PCTimer::~PCTimer()
{
	#ifdef PRINT
		LOG("------------------------ In PCtimer Destructor ------------------------\n\n");
	#endif
}




/**************************************************************************
* \brief  - TimerEvent_Handler
* 			Timer Handler to trigger ISR from RTP
* 			
* \param  - void
*
*
* \return  - void
*
******************************************************************************/

void TimerEvent_Handler()
{		
	for(;;)
    {  			
		if (eventReceive (VXEV01, EVENTS_WAIT_ANY, WAIT_FOREVER, NULL) == ERROR)
 		{
 			LOG("Event Receive failed!!!\n"); 				
 		}
		pAuxClkHandler(task_id);		//Calling User Timer Callback 			 	
    }
}




/**************************************************************************
* \brief  - AuxClkRegister
* 			Registers or Connects the Timer ISR
* 			
* \param  - pUsrHandler - Function Pointer to ISR
*
*
* \return  - Success or Failure
*
******************************************************************************/

STATUS AuxClkRegister(USR_HANDLER pUsrHandler)
{	
	task_id = taskOpen("/TimerEvent0", 100, 0, OM_CREATE, NULL, 0x1000, 0, (FUNCPTR)TimerEvent_Handler,0,0,0,0,0,0,0,0,0,0);	
	pAuxClkHandler = pUsrHandler;		
	AuxClkConnect(task_id);		/* Register Timer ISR */
	return 0;
}




/**************************************************************************
* \brief  - Timer_Init
* 			Creates timer interrupt
* 			
* \param  - void
*
*
* \return  - int - Timer creation return value
*
******************************************************************************/
INT32 PCTimer::Timer_Init()
{		
	
	TimerInterval::ReadTimerIntervalfromtxt();
	AuxClkDisable();		
	UINT32 SampleRate = TimerInterval::GetTimerInterval();
	if(SampleRate == 0)
	{
		SampleRate =DEFAULT_TIMEINTERVAL_US ;
	}
	printf(" TimerInterval %d us\n", SampleRate);
	SampleRate = CONV_MILLI_MICROSEC(1000) / SampleRate;
	if( OK != AuxClkRateSet(SampleRate))		// To Set Interrupt frequency
	{
		LOG("Error calling sysAuxClkRateSet!!!\n");		
		exit(0);
	}	
	if( OK != AuxClkRegister(vxAuxClkIsr) )					// Connect the ISR with the timer
	{
		LOG("Error calling sysAuxClkConnect!!!\n");		
		exit(0);
	}		
	AuxClkEnable( );										// Enable the Clock timer	
	return SUCCESS;	
}

