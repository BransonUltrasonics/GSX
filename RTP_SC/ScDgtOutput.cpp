
#include <vxWorks.h>        //for vxworks main
#include <iostream>
#include <semLib.h>
#include "McSPI.h"
#include "GPIO.h"
#include "Logger.h"
#include "CommonProperty.h"
#include "AlarmManager.h"
#include "IOManager.h"

extern "C"
{
	#include <customSystemCall.h>
}

#include "ScDgtOutput.h"

using namespace std;

//#define SC_DO_TEST

static UINT16 SpiDout_tx[1];
static UINT16 SpiDout_rx[1];  //Initial No faults, Output level < 7V for all of 8 lines ;

static UINT16 dio_output_line;
static UINT16 dio_output_value = SC_DOUT_CONFIG_SET;
static char	recvMsgBuffer[MAX_SIZE_OF_MSG_LENGTH];
static SEM_ID ScDgtOutMutex = 0;

/**************************************************************************//**
* \brief   - This function is responsible for setting Industrial digital IO outputs.
*            Transfer value by SPI.
*
* \param   - line: Set which output line.
* 			 val : Set high or low to the line selected.
*
* \return  - DIO_OK or DIO_ERROR.
*
******************************************************************************/
INT16 ScDgtSetDout(UINT16 val)
{	
	INT16  stat = DIO_OUTPUTS_ERROR;

	//Setup outputs
	SpiDout_tx[0] = val;
		
	//Set the line 1 - 8 output
	stat = McSPI_SnglTrnsfr_DIOOutputs((UINT8 *)&SpiDout_tx[0], (UINT8 *)&SpiDout_rx[0], sizeof(SpiDout_tx[0]));
#ifdef SC_DO_TEST	
	printf("_tx: 0x%x, _rx: 0x%x \n\n  ", SpiDout_tx[0], SpiDout_rx[0]);		
#endif  //SC_DO_TEST
	
	return stat;
}

/**************************************************************************//**
* \brief   - This function is API for IO manager, responsible for setting output value 
*            for dedicated line.
*
* \param   - For which line and what value to be set.
*
* \return  - DIO_OK or DIO_ERROR.
*
******************************************************************************/

INT16 ScDgtSetDoutValue(UINT16 line, UINT16 val)
{	
	Message message;
	INT16 stat = DIO_OUTPUTS_ERROR;
	UINT16 old_dio_value;
	
	// Intertask output access (i.e. alarm task and control task)
	ScDgtOutputMutexTake();
	
	old_dio_value = dio_output_value;

	if(DOUT_LINE_MAX > line)  //The lines 0-7
	{
		if(LINE_HIGH == val)
		{
			dio_output_value |= 1 << (line + 8);   //Set value high
		}
		else if(LINE_LOW == val)
		{
			dio_output_value &= ~(1 << (line + 8));	//Set value low
		}		
		
		if (old_dio_value != dio_output_value)
		{
			// set the message
			memcpy (&message.Buffer, &dio_output_value, sizeof (dio_output_value));
			message.msgID = TO_OUTPUT_TASK_SET_OUTPUT;
			
			// send it to the output task here
			if (msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[DGTOUT_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL) == OK)
				stat = DIO_OUTPUTS_OK;
		}
		else
			stat = DIO_OUTPUTS_OK;
			
	}
    
	ScDgtOutputMutexGive();
	
    return stat;
}

/*
**********************************************************************************

*      Function    :   AcDgtOutputTask
*      Description :   AC Digital IO Outputs Task for transmit Digital outputs
*      Input       :   No arguments
*      Output      :   No return type
*      Called by   :   AcMain

**********************************************************************************
*/

void ScDgtOutputTask()
{
	Message message;
	INT16 stat = ERROR;
	UINT16 val;
	MSG_Q_ID OUTPUT_MSG_Q_ID = CommonProperty::getInstance()->GetMsgQId(cTaskName[DGTOUT_T]);
	
	// Initialize the intertask output access
	ScDgtOutMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
	
	LOGDBG("------------------------ DgtOutput Task spawned ------------------------\n\n",0,0,0);			
		
	while(TRUE)	
	{	
		if(msgQReceive (OUTPUT_MSG_Q_ID, recvMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR)
		{
			memcpy (&message, recvMsgBuffer, sizeof (message));
			
			switch(message.msgID)
			{
			case TO_OUTPUT_TASK_SET_OUTPUT:
					
				// check if this is the last message
				if (msgQNumMsgs (OUTPUT_MSG_Q_ID) == 0)
				{
					memcpy (&val, message.Buffer, sizeof (val));
					
					// Waiting for access to digital outputs
					McSPI::ScDgtInOutSemTake (DGTOUT_T);
					
					McSPI::ScDgtInOutMutexTake ();

					stat = ScDgtSetDout(val);
					
					McSPI::ScDgtInOutMutexGive ();
					
					if (ERROR == stat)
					{
						AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_INTERNAL_COMM_EFA);
						DigitalIOMgr::LogErr ("ScDgtOutputTask:ScDgtSetDout:status: ERROR");
					}
				}
				break;
			}
		}
	}
}


/**************************************************************************//**
* \brief   - This function is responsible for getting output value.
*
* \param   - none.
*
* \return  - The current of output line value, high or low.
*
******************************************************************************/

UINT16 ScDoutGetValue(void)
{			
    return dio_output_value;
}

/**************************************************************************//**
* \brief   - This function is responsible for getting the fault status of any output lines.
*
* \param   - none.
*
* \return  - 16 bits value, only lower 8 bits available, 1: fault; 0: no fault.
*
******************************************************************************/

UINT16 ScDoutGetFault(void)
{	
	UINT16 output_faults;
	
    output_faults = (UINT16 )SpiDout_rx[0];
    output_faults >>= 8;
    
    return output_faults;
}

/**************************************************************************//**
* \brief   - This function is responsible for getting the output levels of any output lines.
*
* \param   - none.
*
* \return  - 16 bits value, only lower 5 bits available, 1: > 7 V; 0: < 7 V.
*
******************************************************************************/
UINT16 ScDoutGetLevel(void)
{	
	UINT16 output_level;
	
    output_level = (UINT16 )SpiDout_rx[0];
    output_level &= 0xff;
    
    return output_level;
}


/**************************************************************************//**
* \brief    - 	Askes for a semaphore for exclusive access to a shared resource
*				between the various tasks using the digital output
*
* \param    - 	None
*
* \return   - 	None
*
******************************************************************************/
void ScDgtOutputMutexTake()
{
	semTake (ScDgtOutMutex, WAIT_FOREVER);
}


/**************************************************************************//**
* \brief    - 	Gives a semaphore for exclusive access to a shared resource
*				between the various tasks using the digital output
*
* \param    - 	None
*
* \return   - 	None
*
******************************************************************************/
void ScDgtOutputMutexGive()
{
	semGive (ScDgtOutMutex);
}
