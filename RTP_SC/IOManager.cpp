/*
 * IOManager.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: dshilonie
 */

#include  <iostream>
#include  <fstream>
#include  "IOManager.h"
#include  "AlarmManager.h"
#include  "McSPI.h"
#include  "ScDgtInput.h"
#include  "ScDgtOutput.h"
#include  "IOFunctions.h"
#include  "Utils.h"
#include  "SC.h"
#include "EthernetIP.h"
using namespace std;

// used for parsing 
#define _STARTRECORD	"@"
#define _ENDRECORD 		";"
#define _STARTDATA 		":"

DigitalIOMgr * DigitalIOMgr::_pDigitalMgr = 0;

/**************************************************************************//**
* \brief  - upon creation, creates and saves available inputs and outputs 
*
* \param  - none
*
* \return  - none
*
******************************************************************************/
DigitalIOMgr::DigitalIOMgr ()
{
	UINT16 line;

#ifdef DIO_DBG
	printf("\nCTRL_T : I/O Manager Created\n");
#endif
	
	// used to access all functions 
	_pDigitalMgr = this;
	
	for (line = 0; line < DIGITAL_IO_MAX_OUTPUTS; line++)
		_outputs[line] = 0;

	for (line = 0; line < DIGITAL_IO_MAX_INPUTS; line++)
		_inputs[line] = 0;
	
	_ioMgrStatus = 0;
	DigitalIOMgr::LogErr ("DigitalIOMgr::DigitalIOMgr: _created");
}


DigitalIOMgr::~DigitalIOMgr ()
{
	_pDigitalMgr = 0;
	
	DeleteOutputs ();
	DeleteInputs ();
}


/**************************************************************************//**
* \brief  - returns the input function of a given input line to the caller
*
* \param  - inputLine - input line number
*
*
* \return  - LINE_FUNCTION - associated function of the input line 
*
******************************************************************************/
DigitalIO::LINE_FUNCTION DigitalIOMgr::GetInputFunction (UINT8 inputLine)
{
	_CHECKRETURN(inputLine < DIGITAL_IO_MAX_INPUTS,DigitalIO::INPUT_FUNCTION_NONE)
		
	UINT16 line = inputLine;
	DigitalIO::LINE_FUNCTION function = DigitalIO::INPUT_FUNCTION_NONE;

		if (_inputs[line] != NULL)
			function = ((DigitalInput *)_inputs[line])->GetFunction();
		
	return function;
}


/**************************************************************************//**
* \brief  - returns the input value of a given input line to the caller
* 			scan prior to reading the line
*
* \param  - inputLine - input line number
*
*
* \return  - LINE_FUNCTION - associated function of the input line 
*
******************************************************************************/
DigitalIO::LINE_VALUE DigitalIOMgr::GetInputValue (UINT8 inputLine)
{
	_CHECKRETURN(inputLine < DIGITAL_IO_MAX_INPUTS,DigitalIO::LINE_NONE)

	UINT16 line = inputLine;
	DigitalIO::LINE_VALUE value = DigitalIO::LINE_NONE;

		if (_inputs[line] != NULL)
			value = ((DigitalInput *)_inputs[line])->GetValue();
		
	return value;

}

DigitalIO::LINE_STATE DigitalIOMgr::GetInputState (UINT8 inputLine)
{
	_CHECKRETURN(inputLine < DIGITAL_IO_MAX_INPUTS,DigitalIO::STATE_NONE)
			
	UINT16 line = inputLine;
	DigitalIO::LINE_STATE state = DigitalIO::STATE_NONE;

		if (_inputs[line] != NULL)
			state = ((DigitalInput *)_inputs[line])->GetState();
		
	return state;

}


/**************************************************************************//**
*\brief  - Based on information from the HMI, create all the outputs using:
* 			input line, function, active state and source. (i.e AC_READY, ACTIVE HIGH, DIO)
*
* \param  - DIGITAL_FUNCTION outputs - array of outputs
* 			configured to function, state and source
*
*
* \return  - success or failure in setting the inputs
******************************************************************************/
bool DigitalIOMgr::SetOutputs (const DigitalIO::DIGITAL_FUNCTION outputs[DIGITAL_IO_MAX_OUTPUTS])
{
	_CHECKRETURN(outputs,false)
			
	UINT16 line;

	// get outputs lines and functions layout from the UI here
	for (line = 0; line < DIGITAL_IO_MAX_OUTPUTS; line++)
	{
		if (_outputs[line] != NULL)
		{
			// make sure line is at 0 before removal
			if (((DigitalOutput *)_outputs[line])->GetState() == DigitalIO::STATE_ACTIVE_LOW)
				SetOutputFromFunction (((DigitalOutput *)_outputs[line])->GetFunction(), DigitalIO::LINE_HIGH);
			else
				SetOutputFromFunction (((DigitalOutput *)_outputs[line])->GetFunction(), DigitalIO::LINE_LOW);

			delete _outputs[line];
		}
		
		_outputs[line] = NULL;
		
		if (outputs[line].function != DigitalIO::OUTPUT_FUNCTION_NONE)
		{
			// create all outputs 
			_outputs[line] = new DigitalOutput (line , outputs[line].function, outputs[line].state, outputs[line].source);
		}
	}
	
	return true;
}


/**************************************************************************//**
* \brief  - return IO manager status bit (i.e refresh)
* 			some bit reading may be clearing that bit
*
* \param  - UINT16 bit 
*
* \return  - bool status
*
******************************************************************************/
bool DigitalIOMgr::GetStatus (UINT16 bit)
{
	_CHECKRETURN(_pDigitalMgr,false)

	bool status = false;
		
	switch (bit)
	{
	case IOMGR_REFRESH_OUTPUTS:
		
		if (_pDigitalMgr->_ioMgrStatus & BIT_MASK(bit))
			status = true;
		
		_pDigitalMgr->_ioMgrStatus &= ~BIT_MASK(bit);
		break;
	}
				
	return status;
}


/**************************************************************************//**
* \brief  - Based on information from the HMI, create all the inputs using:
* 			input line, function, active state and source. (i.e PART PRESENT, ACTIVE HIGH, DIO)
*
* \param  - DIGITAL_FUNCTION inputs - array of inputs
* 			configured to function, state and source
*
*
* \return  - success or failure in setting the inputs
*
******************************************************************************/
bool DigitalIOMgr::SetInputs (const DigitalIO::DIGITAL_FUNCTION inputs[DIGITAL_IO_MAX_INPUTS])
{
	_CHECKRETURN(inputs,false)
			
	UINT16 line;
	
	// set inputs thru lines from UI layout here
	for (line = 0; line < DIGITAL_IO_MAX_INPUTS; line++)
	{
		if (_inputs[line] != NULL)
			delete _inputs[line];

		_inputs[line] = NULL;

		if (inputs[line].function != DigitalIO::INPUT_FUNCTION_NONE)
		{
			// create all inputs
			_inputs[line] = new DigitalInput (line , inputs[line].function, inputs[line].state, inputs[line].source);
		}
	}
	
	return true;
}


/**************************************************************************//**
* \brief  - Based on information from a recipe file, sets all the inputs based on
* 			input line and then function then state and source (i.e PART PRESENT, ACTIVE HIGH, DIO)
*
* \param  - DIGITAL_FUNCTION inputs - reference to an input array
* 		  - DIGITAL_FUNCTION outputs - reference to an output array
* 		  - Message message - message buffer containing the setup data		
*
*
* \return  - success or failure in setting the inputs
*
******************************************************************************/
bool DigitalIOMgr::ReadConfig (Message& message, DigitalIO::DIGITAL_FUNCTION inputs[DIGITAL_IO_MAX_INPUTS], 
		DigitalIO::DIGITAL_FUNCTION outputs[DIGITAL_IO_MAX_OUTPUTS])
{
	_CHECKRETURN(inputs,false)
	_CHECKRETURN(outputs,false)
	
	USERIO userIO;
	
	memcpy(&userIO, message.Buffer,sizeof (userIO));
		
	inputs[0].function = AdjustPSInputFunction (userIO.PS_USERIO.PsDigitalInput.J116_01.PS_EVENT); 
	inputs[0].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalInput.J116_01.Status; 
	inputs[0].source = DigitalIO::SOURCE_POWERSUPPLY; 
	inputs[1].function = AdjustPSInputFunction (userIO.PS_USERIO.PsDigitalInput.J116_02.PS_EVENT); 
	inputs[1].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalInput.J116_02.Status; 
	inputs[1].source = DigitalIO::SOURCE_POWERSUPPLY; 
	inputs[2].function = AdjustPSInputFunction (userIO.PS_USERIO.PsDigitalInput.J116_03.PS_EVENT); 
	inputs[2].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalInput.J116_03.Status; 
	inputs[2].source = DigitalIO::SOURCE_POWERSUPPLY; 
	inputs[3].function = AdjustPSInputFunction (userIO.PS_USERIO.PsDigitalInput.J116_04.PS_EVENT); 
	inputs[3].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalInput.J116_04.Status; 
	inputs[3].source = DigitalIO::SOURCE_POWERSUPPLY; 
	inputs[4].function = AdjustPSInputFunction (userIO.PS_USERIO.PsDigitalInput.J116_11.PS_EVENT); 
	inputs[4].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalInput.J116_11.Status; 
	inputs[4].source = DigitalIO::SOURCE_POWERSUPPLY; 
	inputs[5].function = AdjustPSInputFunction (userIO.PS_USERIO.PsDigitalInput.J116_12.PS_EVENT); 
	inputs[5].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalInput.J116_12.Status; 
	inputs[5].source = DigitalIO::SOURCE_POWERSUPPLY; 
	inputs[6].function = AdjustPSInputFunction (userIO.PS_USERIO.PsDigitalInput.J116_13.PS_EVENT); 
	inputs[6].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalInput.J116_13.Status; 
	inputs[6].source = DigitalIO::SOURCE_POWERSUPPLY; 
	inputs[7].function = AdjustPSInputFunction (userIO.PS_USERIO.PsDigitalInput.J116_16.PS_EVENT); 
	inputs[7].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalInput.J116_16.Status; 
	inputs[7].source = DigitalIO::SOURCE_POWERSUPPLY; 
	inputs[8].function = AdjustPSInputFunction (userIO.PS_USERIO.PsDigitalInput.J116_23.PS_EVENT); 
	inputs[8].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalInput.J116_23.Status; 
	inputs[8].source = DigitalIO::SOURCE_POWERSUPPLY; 
	
	inputs[9].function = AdjustACInputFunction (userIO.AC_USERIO.ACDigitalInput.J710_01.AC_EVENT); 
	inputs[9].state = (DigitalIO::LINE_STATE) userIO.AC_USERIO.ACDigitalInput.J710_01.Status; 
	inputs[9].source = DigitalIO::SOURCE_ACTUATOR; 
	inputs[10].function = AdjustACInputFunction (userIO.AC_USERIO.ACDigitalInput.J710_02.AC_EVENT); 
	inputs[10].state = (DigitalIO::LINE_STATE) userIO.AC_USERIO.ACDigitalInput.J710_02.Status; 
	inputs[10].source = DigitalIO::SOURCE_ACTUATOR; 
	inputs[11].function = AdjustACInputFunction (userIO.AC_USERIO.ACDigitalInput.J710_03.AC_EVENT); 
	inputs[11].state = (DigitalIO::LINE_STATE) userIO.AC_USERIO.ACDigitalInput.J710_03.Status; 
	inputs[11].source = DigitalIO::SOURCE_ACTUATOR; 
	inputs[12].function = AdjustACInputFunction (userIO.AC_USERIO.ACDigitalInput.J710_04.AC_EVENT); 
	inputs[12].state = (DigitalIO::LINE_STATE) userIO.AC_USERIO.ACDigitalInput.J710_04.Status; 
	inputs[12].source = DigitalIO::SOURCE_ACTUATOR; 
	inputs[13].function = AdjustACInputFunction (userIO.AC_USERIO.ACDigitalInput.J710_05.AC_EVENT); 
	inputs[13].state = (DigitalIO::LINE_STATE) userIO.AC_USERIO.ACDigitalInput.J710_05.Status; 
	inputs[13].source = DigitalIO::SOURCE_ACTUATOR; 
	
	outputs[0].function = AdjustPSOutputFunction (userIO.PS_USERIO.PsDigitalOutput.J116_07.PS_EVENT); 
	outputs[0].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalOutput.J116_07.Status; 
	outputs[0].source = DigitalIO::SOURCE_POWERSUPPLY; 
	outputs[1].function = AdjustPSOutputFunction (userIO.PS_USERIO.PsDigitalOutput.J116_08.PS_EVENT); 
	outputs[1].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalOutput.J116_08.Status; 
	outputs[1].source = DigitalIO::SOURCE_POWERSUPPLY; 
	outputs[2].function = AdjustPSOutputFunction (userIO.PS_USERIO.PsDigitalOutput.J116_09.PS_EVENT); 
	outputs[2].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalOutput.J116_09.Status; 
	outputs[2].source = DigitalIO::SOURCE_POWERSUPPLY; 
	outputs[3].function = AdjustPSOutputFunction (userIO.PS_USERIO.PsDigitalOutput.J116_10.PS_EVENT); 
	outputs[3].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalOutput.J116_10.Status; 
	outputs[3].source = DigitalIO::SOURCE_POWERSUPPLY; 
	outputs[4].function = AdjustPSOutputFunction (userIO.PS_USERIO.PsDigitalOutput.J116_19.PS_EVENT); 
	outputs[4].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalOutput.J116_19.Status; 
	outputs[4].source = DigitalIO::SOURCE_POWERSUPPLY; 
	outputs[5].function = AdjustPSOutputFunction (userIO.PS_USERIO.PsDigitalOutput.J116_20.PS_EVENT); 
	outputs[5].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalOutput.J116_20.Status; 
	outputs[5].source = DigitalIO::SOURCE_POWERSUPPLY; 
	outputs[6].function = AdjustPSOutputFunction (userIO.PS_USERIO.PsDigitalOutput.J116_21.PS_EVENT); 
	outputs[6].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalOutput.J116_21.Status; 
	outputs[6].source = DigitalIO::SOURCE_POWERSUPPLY; 
	outputs[7].function = AdjustPSOutputFunction (userIO.PS_USERIO.PsDigitalOutput.J116_22.PS_EVENT); 
	outputs[7].state = (DigitalIO::LINE_STATE) userIO.PS_USERIO.PsDigitalOutput.J116_22.Status; 
	outputs[7].source = DigitalIO::SOURCE_POWERSUPPLY; 

	outputs[8].function = AdjustACOutputFunction (userIO.AC_USERIO.ACDigitalOutput.J710_09.AC_EVENT); 
	outputs[8].state = (DigitalIO::LINE_STATE) userIO.AC_USERIO.ACDigitalOutput.J710_09.Status; 
	outputs[8].source = DigitalIO::SOURCE_ACTUATOR; 
	outputs[9].function = AdjustACOutputFunction (userIO.AC_USERIO.ACDigitalOutput.J710_10.AC_EVENT); 
	outputs[9].state = (DigitalIO::LINE_STATE) userIO.AC_USERIO.ACDigitalOutput.J710_10.Status; 
	outputs[9].source = DigitalIO::SOURCE_ACTUATOR; 
	outputs[10].function = AdjustACOutputFunction (userIO.AC_USERIO.ACDigitalOutput.J710_11.AC_EVENT); 
	outputs[10].state = (DigitalIO::LINE_STATE) userIO.AC_USERIO.ACDigitalOutput.J710_11.Status; 
	outputs[10].source = DigitalIO::SOURCE_ACTUATOR; 
	outputs[11].function = AdjustACOutputFunction (userIO.AC_USERIO.ACDigitalOutput.J710_12.AC_EVENT); 
	outputs[11].state = (DigitalIO::LINE_STATE) userIO.AC_USERIO.ACDigitalOutput.J710_12.Status; 
	outputs[11].source = DigitalIO::SOURCE_ACTUATOR; 
	outputs[12].function = AdjustACOutputFunction (userIO.AC_USERIO.ACDigitalOutput.J710_13.AC_EVENT); 
	outputs[12].state = (DigitalIO::LINE_STATE) userIO.AC_USERIO.ACDigitalOutput.J710_13.Status; 
	outputs[12].source = DigitalIO::SOURCE_ACTUATOR; 
	
	return true;
}


/**************************************************************************//**
* \brief  - deletes all outputs and frees all memory
*
* \param  - None
*
*
* \return  - none
*
******************************************************************************/
void DigitalIOMgr::DeleteOutputs ()
{
	UINT16 line;

	for (line = 0; line < DIGITAL_IO_MAX_OUTPUTS; line++)
	{
		if (_outputs[line] != NULL)
			delete _outputs[line];
		
			_outputs[line] = NULL;
	}
}


/**************************************************************************//**
* \brief  - deletes all inputs and frees all memory
*
* \param  - None
*
*
* \return  - none
*
******************************************************************************/
void DigitalIOMgr::DeleteInputs ()
{
	UINT16 line;

	for (line = 0; line < DIGITAL_IO_MAX_INPUTS; line++)
	{
		if (_inputs[line] != NULL)
			delete _inputs[line];
		
			_inputs[line] = NULL;
	}
}



/**************************************************************************//**
* \brief  - Reads all digital inputs. This method will record any errors
* 			in a status byte
*
* \param  - none
*
*
* \return  - inputsRead - success or failure
*
******************************************************************************/
void DigitalIOMgr::ReadInputs ()
{
	UINT16 	line;
	UINT16	SCInputs = 0;
	UINT8   ACInputs = 0;
	
	if (_pDigitalMgr)
	{
		// call IO driver to read SC inputs
		SCInputs = ScDgtGetDinValue ();

		// copy AC inputs from PDO
		ACInputs = AC_TX->ACInputs;
		
		for (line = 0; line < DIGITAL_IO_MAX_INPUTS; line++)
		{
			if (_pDigitalMgr->_inputs[line] != NULL)
			{
				if (((DigitalInput *)_pDigitalMgr->_inputs[line])->GetSource() == DigitalIO::SOURCE_POWERSUPPLY)
				{
					((DigitalInput *)_pDigitalMgr->_inputs[line])->SetValue ((SCInputs & (1 << line)) ? DigitalIO::LINE_HIGH : DigitalIO::LINE_LOW );
				}
				else if (((DigitalInput *)_pDigitalMgr->_inputs[line])->GetSource() == DigitalIO::SOURCE_ACTUATOR)
				{
					((DigitalInput *)_pDigitalMgr->_inputs[line])->SetValue ((ACInputs & (DIGITAL_IO_AC_INPUT_BIT_START << ((DigitalInput *)_pDigitalMgr->_inputs[line])->MapJ710ToJ58(line - DIGITAL_IO_AC_INPUT_START))) ? DigitalIO::LINE_HIGH : DigitalIO::LINE_LOW );
				}
				else
				{
					// other input source here 
				}
			}
		}
	}
}


/**************************************************************************//**
* \brief  - scans all inputs for a given function
*
* \param  - function 	- function of an input to look for
*
*
* \return  - inputFound - success or failure
*
******************************************************************************/
bool DigitalIOMgr::ScanForInputFunction (DigitalIO::LINE_FUNCTION function)
{
	_CHECKRETURN(function < DigitalIO::INPUT_FUNCTION_MAX,false)
			
	bool inputFound = false;
	UINT16 line;
	
	if (_pDigitalMgr)
	{
		for (line = 0; line < DIGITAL_IO_MAX_INPUTS; line++)
		{
			if (_pDigitalMgr->_inputs[line] != NULL)
			{
				if (((DigitalInput *)_pDigitalMgr->_inputs[line])->GetFunction() == function)
				{
					inputFound = true;
				}
			}
		}
	}
	
	return inputFound;
}


/**************************************************************************//**
* \brief  - scans all inputs for a given function and returns the input line
* 			with that particular function
*
* \param  - function -	function of an input to look for
*
*
* \return  - INPUT_STATE  	INPUT_ASSERTED or INPUT_NOT_ASSERTED 							
*
******************************************************************************/
DigitalIO::INPUT_STATE DigitalIOMgr::GetInputFromFunction (DigitalIO::LINE_FUNCTION function)
{
	_CHECKRETURN(function < DigitalIO::INPUT_FUNCTION_MAX,DigitalIO::INPUT_NONE)
			
	UINT16 line;
	DigitalIO::INPUT_STATE inputState = DigitalIO::INPUT_NONE;

	if (_pDigitalMgr)
	{
		for (line = 0; line < DIGITAL_IO_MAX_INPUTS; line++)
		{
			if (_pDigitalMgr->_inputs[line] != NULL)
			{
				if (((DigitalInput *)_pDigitalMgr->_inputs[line])->GetFunction() == function)
				{
					 switch (((DigitalInput *)_pDigitalMgr->_inputs[line])->GetValue ())
					 {
					 case DigitalIO::LINE_HIGH:
						 inputState = (((DigitalInput *)_pDigitalMgr->_inputs[line])->GetState() == DigitalIO::STATE_ACTIVE_LOW ) ? DigitalIO::INPUT_NOT_ASSERTED
								 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 : DigitalIO::INPUT_ASSERTED;
						 break;
					 case DigitalIO::LINE_LOW:
						 inputState = (((DigitalInput *)_pDigitalMgr->_inputs[line])->GetState() == DigitalIO::STATE_ACTIVE_LOW ) ? DigitalIO::INPUT_ASSERTED
								 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 : DigitalIO::INPUT_NOT_ASSERTED;
						 break;
					 default:
						 inputState = DigitalIO::INPUT_NOT_ASSERTED;
					 }
				}
			}
		}
	}
	
	return inputState;
}


/**************************************************************************//**
* \brief  - scans all outputs for a given function and returns true if
* 			that particular function is found
*
* \param  - LINE_FUNCTION function
*
* \return  - bool outputFound
*
******************************************************************************/
bool DigitalIOMgr::ScanForOutputFunction (DigitalIO::LINE_FUNCTION function)
{
	_CHECKRETURN(function > DigitalIO::INPUT_FUNCTION_MAX,false)
	_CHECKRETURN(function < DigitalIO::OUTPUT_FUNCTION_MAX,false)

	bool outputFound = false;
	UINT16 line;
	
	if (_pDigitalMgr)
	{
		for (line = 0; line < DIGITAL_IO_MAX_OUTPUTS; line++)
		{
			if (_pDigitalMgr->_outputs[line] != NULL)
			{
				if (((DigitalOutput *)_pDigitalMgr->_outputs[line])->GetFunction() == function)
				{
					outputFound = true;
				}
			}
		}
	}	
	
	return outputFound;
}


/**************************************************************************//**
* \brief  - checks an output for a particular function and asserts
* 			this line with output supplied line values
*
* \param  - function -	function of an input to look for
* 		  - line	 - 	a particular output line since there could be multiple
* 		  				output lines configured with to same output function
* 		  - values	 -	carries output line values to assert this line with 
*
*
* \return  - outputSet - success or failure  
*
******************************************************************************/
bool DigitalIOMgr::SetOutputFromFunction (DigitalIO::LINE_FUNCTION function, DigitalIO::LINE_VALUE value, const DigitalIO::LINE_VALUE values[])
{
	_CHECKRETURN(function > DigitalIO::INPUT_FUNCTION_MAX,false)
	_CHECKRETURN(function < DigitalIO::OUTPUT_FUNCTION_MAX,false)

	bool outputSet = false;
	UINT16 line;

	if (_pDigitalMgr)
	{
		for (line = 0; line < DIGITAL_IO_MAX_OUTPUTS; line++)
		{
			if (_pDigitalMgr->_outputs[line] != NULL)
			{
				if (((DigitalOutput *)_pDigitalMgr->_outputs[line])->GetFunction() == function)
				{
					// assert output with a set of values here, or only one value
					if (values)
					{
						//TODO:DSS what to do with the array of values
						if (((DigitalOutput *)_pDigitalMgr->_outputs[line])->SetValue (values[0]))
							outputSet = true;
					}
					else
					{
						if (((DigitalOutput *)_pDigitalMgr->_outputs[line])->SetValue (value))
							outputSet = true;
					}
				}
			}
		}
	
		if((FieldBus::GetFieldBusInitialised() == true))
		{
			EthernetIP  *FbPtr = EthernetIP::Getinstance();
			FbPtr->SetOutputToIOImage(function,value);
		}
	}
	
	return outputSet;
}



/**************************************************************************//**
* \brief  - Correlates between UIC selected IO function and the I/O manager
* 			IO functions 
*
* \param  - ps_event -	UIC IO selected event
*
* \return - function - correlated IO function  
*
******************************************************************************/
DigitalIO::LINE_FUNCTION DigitalIOMgr::AdjustPSInputFunction(
		PowerSupplyInput ps_event)
{
	DigitalIO::LINE_FUNCTION function = DigitalIO::INPUT_FUNCTION_NONE;
	
	switch (ps_event)
	{
	case CYCLEABORT:
		function = DigitalIO::INPUT_CYCLE_ABORT;
		break;
	case US_DISABLE:
		function = DigitalIO::INPUT_SONIC_DISABLE;
		break;
	case RESET:
		function = DigitalIO::INPUT_ALARM_RESET;
		break;
	case CONFIRM_REJECT:
		function = DigitalIO::INPUT_REJECT_CONFIRM;
		break;
	case HOLD_DELAY:
		function = DigitalIO::INPUT_HOLD_DELAY;
		break;
	case HORN_SEEK_IN:
		function = DigitalIO::INPUT_HORN_SEEK;
		break;
	}
	
	return function;
}


/**************************************************************************//**
* \brief  - Correlates between UIC selected IO function and the I/O manager
* 			IO functions 
*
* \param  - ac_event -	UIC IO selected event
*
* \return - function - correlated IO function  
*
******************************************************************************/
DigitalIO::LINE_FUNCTION DigitalIOMgr::AdjustACInputFunction(
		ActuatorInput ac_event)
{
	DigitalIO::LINE_FUNCTION function = DigitalIO::INPUT_FUNCTION_NONE;
	
	switch (ac_event)
	{
	case TOOLING_IN:
		function = DigitalIO::INPUT_TOOLING_READY;
		break;
	case PARTPRESENT:
		function = DigitalIO::INPUT_PART_PRESENT;
		break;
	case HOMEPOSITION_IN:
		function = DigitalIO::INPUT_AC_HOME;
		break;
	case READY_POSITION_IN:
		function = DigitalIO::INPUT_AC_READY;
		break;
	}
	
	return function;
}


/**************************************************************************//**
* \brief  - Correlates between UIC selected IO function and the I/O manager
* 			IO functions 
*
* \param  - ps_event -	UIC IO selected event
*
* \return - function - correlated IO function  
*
******************************************************************************/
DigitalIO::LINE_FUNCTION DigitalIOMgr::AdjustPSOutputFunction(
		PowerSupplyOutput ps_event)
{
	DigitalIO::LINE_FUNCTION function = DigitalIO::OUTPUT_FUNCTION_NONE;
	    
	switch (ps_event)
	{
	case READY:
		function = DigitalIO::OUTPUT_SYSTEM_READY;
		break;
	case SONICS_ACTIVE:
		function = DigitalIO::OUTPUT_ULTRASONIC;
		break;
	case GENERAL_ALARM:
		function = DigitalIO::OUTPUT_ALARM_GENERAL;
		break;
	case CYCLE_RUNNING:
		function = DigitalIO::OUTPUT_CYCLE_RUN;
		break;
	case EXTERNAL_BEEPER:
		function = DigitalIO::OUTPUT_BEEPER;
		break;
	case CYCLE_OK:
		function = DigitalIO::OUTPUT_CYCLE_OK;
		break;
	case CUSTOM_ALARM:
		function = DigitalIO::OUTPUT_ALARM_CUSTOM;
		break;
	case REJECTPART_ALARM:
		function = DigitalIO::OUTPUT_ALARM_REJECT;
		break;
	case SUSPECTPART_ALARM:
		function = DigitalIO::OUTPUT_ALARM_SUSPECT;
		break;
	case HORN_SEEK_OUT:
		function = DigitalIO::OUTPUT_HORN_SEEK;
		break;
	case WELD_ACTIVE:
		function = DigitalIO::OUTPUT_WELD_ACTIVE;
		break;
	case PSHOLD_ACTIVE:
		function = DigitalIO::OUTPUT_HOLD_ACTIVE;
		break;
	}
	
	return function;
}


/**************************************************************************//**
* \brief  - Logs onto eMMC the I/O manager errors/status strings if called
* 			
*
* \param  - const char* logErr
*
* \return - bool written
*
******************************************************************************/
bool DigitalIOMgr::LogErr(const char* logErr)
{
	bool written = false;
	fstream _IOErr_fp;

	_IOErr_fp.open (IO_MANAGER_LOG_PATH, ios::out|ios::app);
	
	if(_IOErr_fp.is_open())
	{
		_IOErr_fp << logErr << endl;
		_IOErr_fp.close();
		written = true;
	}	
	
	return written;
}


/**************************************************************************//**
* \brief  - Refreshes the I/O manager active outputs to their 'active' state
*			after every user programming.	
* 			
*
* \param  - None
*
* \return - None
*
******************************************************************************/
void DigitalIOMgr::RefreshOutputs()
{
	switch (SC::stateMachine.GetStateType ())
	{
	case SC_READY:
		
		// set output for cycle OK 
		if (AlarmMgr::IsCycleOK ())
			DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_CYCLE_OK, DigitalIO::LINE_HIGH);

		// assert output for system ready 
		DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_SYSTEM_READY, DigitalIO::LINE_HIGH);

		_ioMgrStatus |= BIT_MASK(IOMGR_REFRESH_OUTPUTS);
		break;
		
	case SC_ALARM:
		
		// assert output for general alarm 
		DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_GENERAL, DigitalIO::LINE_HIGH);

		// set output for cycle OK 
		if (AlarmMgr::IsCycleOK ())
			DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_CYCLE_OK, DigitalIO::LINE_HIGH);
		
		_ioMgrStatus |= BIT_MASK(IOMGR_REFRESH_OUTPUTS);
		break;
		
	default:
		_ioMgrStatus &= ~BIT_MASK(IOMGR_REFRESH_OUTPUTS);
		break;
	}
	
#ifdef DIO_DBG
	if (_ioMgrStatus & BIT_MASK(IOMGR_REFRESH_OUTPUTS))
		printf("\nCTRL_T : DigitalIOMgr: ReadConfig: %s_refresh%s\n", KBLU, KNRM);
#endif

}

/**************************************************************************//**
* \brief  - Correlates between UIC selected IO function and the I/O manager
* 			IO functions 
*
* \param  - ac_event -	UIC IO selected event
*
* \return - function - correlated IO function  
*
******************************************************************************/
DigitalIO::LINE_FUNCTION DigitalIOMgr::AdjustACOutputFunction(
		ActuatorOutput ac_event)
{
	DigitalIO::LINE_FUNCTION function = DigitalIO::OUTPUT_FUNCTION_NONE;
	    
	switch (ac_event)
	{
	case HOMEPOSITION_OUT:
		function = DigitalIO::OUTPUT_AC_HOME;
		break;
	case READY_POSITION_OUT:
		function = DigitalIO::OUTPUT_AC_READY;
		break;
	case PB_RELEASE:
		function = DigitalIO::OUTPUT_PB_RELEASE;
		break;
	case TOOLING_OUT:
		function = DigitalIO::OUTPUT_TOOLING_READY;
		break;
	case SOLENOID_VALVE:
		function = DigitalIO::OUTPUT_SOLENOID;
		break;
	case HOLD_ACTIVE:
		function = DigitalIO::OUTPUT_HOLD_ACTIVE;
		break;
	}
	
	return function;
}
