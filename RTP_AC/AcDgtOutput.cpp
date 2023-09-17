
#include <vxWorks.h>        
#include <iostream>
#include "AcControlTask.h"		
#include "SPI.h"
#include "GpioAPI.h"

extern "C"
{
	#include <customSystemCall.h>
}

#include "AcDgtOutput.h"

using namespace std;

//Initialize static variables
UINT16 AcDgtOutput::SpiDout_tx[1];
UINT16 AcDgtOutput::SpiDout_rx[1];
UINT16 AcDgtOutput::dio_output_value = AC_DOUT_CONFIG_SET;
SEM_ID AcDgtOutput::DgtOutputTaskSem;

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
INT16 AcDgtOutput::AcDgtSetDout(UINT16 val)
{	
	INT16  stat = DIO_OUTPUTS_ERROR;

	//Setup outputs
	SpiDout_tx[0] = val;
		
	//Set the line 1 - 5 output
	stat = McSPI_SnglTrnsfr_DIOOutputs((UINT8 *)&SpiDout_tx[0], (UINT8 *)&SpiDout_rx[0], sizeof(SpiDout_tx[0]));
#ifdef AC_DO_TEST	
	printf("_tx: 0x%x, _rx: 0x%x \n\n  ", SpiDout_tx[0], SpiDout_rx[0]);		
#endif  //AC_DO_TEST
	
	return stat;
}

/**************************************************************************//**
* \brief   - This function is API for IO manager, responsible for setting output value 
*
* \param   - value to be set, 5 LSB of word correspond to 5 output channels.
*
* \return  - DIO_OK or DIO_ERROR.
*
******************************************************************************/

INT16 AcDgtOutput::AcDgtSetDoutValue(UINT16 val)
{	
	INT16 stat = DIO_OUTPUTS_ERROR;		
	
	dio_output_value &= 0xff;
	dio_output_value |= (val << 8);
    
    stat = semGive(DgtOutputTaskSem);
   
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

void AcDgtOutput::AcDgtOutputTask()
{
	INT16 stat = ERROR;


	
	#ifdef PRINT	
		LOG("------------------------ DgtOutput Task spawned ------------------------\n\n");			
	#endif
	DgtOutputTaskSem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);

	while(TRUE)	
	{	
		semTake(DgtOutputTaskSem, WAIT_FOREVER);	

		//Receive the Digital Output here
		semTake(McSPI::AcDgtInOutTaskMutex, WAIT_FOREVER);
		
		stat = AcDgtSetDout(dio_output_value);
		
		semGive(McSPI::AcDgtInOutTaskMutex);
		
		if (ERROR == stat)
		{
			LOG("Set DgtOutput failed!!!\n");
		}
	}
}



/**************************************************************************//**
* \brief   - This function is responsible for getting the fault status of any output lines.
*
* \param   - none.
*
* \return  - 16 bits value, only lower 5 bits available, 1: fault; 0: no fault.
*
******************************************************************************/

UINT16 AcDgtOutput::AcDoutGetFault(void)
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

UINT16 AcDgtOutput::AcDoutGetLevel(void)
{	
	UINT16 output_level;
	
    output_level = (UINT16 )SpiDout_rx[0];
    output_level &= 0xff;
    
    return output_level;
}
