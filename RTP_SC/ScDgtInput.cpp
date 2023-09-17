
#include <vxWorks.h>
#include <semLib.h>
#include <iostream>
#include <stdio.h>         
#include "McSPI.h"
#include "GPIO.h"
#include "ScDgtInput.h"
#include "Commons.h"
#include "AlarmManager.h"
#include "IOManager.h"

extern "C"
{
	#include <customSystemCall.h>
}


using namespace std;

//#define SC_DI_TEST

// Buffer containing the mask bits for interrupt out
static UINT16 txBuf[1];

// Buffer containing the received data
static UINT16 rxBuf[1];

static UINT16 dio_input_value;

/**************************************************************************//**
* \brief   - This function is responsible for receiving Digital IO inputs.
*
* \param   - UINT16 *.
*
* \return  - the 16 bit port value.
*
******************************************************************************/
INT16 ScDgtGetDinPort(UINT16* val)
{	
	INT16 stat = DIO_INPUTS_ERROR;

	txBuf[0] = 0x1ff;  //Enable 9 inputs
	// Buffer containing the received data
	rxBuf[0] = 0x0;

	//Receive the serial output
	stat = McSPI_SnglTrnsfr_DIOInputs((UINT8 *)&txBuf[0], (UINT8 *)&rxBuf[0], sizeof(rxBuf));

	if (SPI_OK == stat)
	{
		//Don want whole port value, not the single line value
		*val = rxBuf[0];
	    dio_input_value = rxBuf[0];
#ifdef SC_DI_TEST
		printf("*val: 0x%x\n ",rxBuf[0] );		
#endif  //SC_DI_TEST		
	}
	else
	{
#ifdef PRINT	
//			LOGDBG("SPI DIO Transfer \n",0,0,0);
#endif
	}

	if (SPI_OK == stat)
	{
		stat = DIO_INPUTS_OK;
	}
	
	return stat;
}

/**************************************************************************//**
* \brief   - This function is responsible for saving the value received from Digital IO inputs.
*
* \param   - UINT16.
*
* \return  - None.
*
******************************************************************************/
void ScDgtSaveDinValue(UINT16 in_val)
{			
    dio_input_value = in_val;
}

/**************************************************************************//**
* \brief   - This function is responsible for read value from received Digital IO inputs.
*
* \param   - None.
*
* \return  - the 16 bit port value.
*
******************************************************************************/
UINT16 ScDgtGetDinValue(void)
{			
    return dio_input_value;
}

/**************************************************************************//**
* \brief   - This function is responsible for Digital IO Inputs Task for receive Digital inputs
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ScDgtInputTask()
{
	INT16 stat = ERROR;
	static  UINT16      Val;
    static  UINT16      inVal;
    
	LOGDBG("------------------------ DgtInput Task spawned ------------------------\n\n",0,0,0);			

	while(TRUE)	
	{	
		// Waiting for access to digital inputs
		McSPI::ScDgtInOutSemTake (DGTIN_T);

		McSPI::ScDgtInOutMutexTake ();
		
		stat = ScDgtGetDinPort(&Val);            //Digital Inputs receive the data from Digital Outputs lines
		
		McSPI::ScDgtInOutMutexGive ();

		if (ERROR == stat)
		{
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_INTERNAL_COMM_EFA);
			DigitalIOMgr::LogErr ("ScDgtInputTask:ScDgtGetDinPort:status: ERROR");
		}
		else
		{
			inVal = ScDgtGetDinValue();
		}
	}			
}

