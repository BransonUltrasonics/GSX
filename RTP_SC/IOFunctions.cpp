/*
 * IOFunactions.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: dshilonie
 */

#include  <sys/types.h>
#include  <cstdlib>
#include  <cstdio>
#include  <iostream>
#include <fstream>
#include  <string>
#include  <cstring>
#include  "IOManager.h"
#include  "McSPI.h"
#include  "ScDgtInput.h"
#include  "ScDgtOutput.h"
#include  "IOFunctions.h"
#include  "Utils.h"

using namespace std;

/**************************************************************************//**
* \brief    - create output resource upon request based on line, function,
* 			  convention and source of IO and retain this setup 
*
* \param    - line - physical line number of this IO
* 			  function - digital function of this IO
* 			  state - STATE_ACTIVE_LOW or STATE_ACTIVE_HIGH 
* 			  source - SOURCE_POWERSUPPLY or  SOURCE_ACTUATOR
*
* \return  - None
*
******************************************************************************/
DigitalOutput::DigitalOutput (UINT16 line, LINE_FUNCTION function, LINE_STATE state, LINE_SOURCE source) : DigitalIO ((UINT16) line, function, state, source)
{
#ifdef DIO_DBG
	printf ("\nCTRL_T : ___Output: %d function: %d active state: %d source: %d %s_configured%s\n", _line, _function, _state, _source, KGRN, KNRM);
#endif
}

/**************************************************************************//**
* \brief  - relinquish resources upon deletion 
*
* \param  - none
*
* \return  - none
*
******************************************************************************/
DigitalOutput::~DigitalOutput () 
{
#ifdef DIO_DBG
	printf ("\nCTRL_T : ___Output: %d function: %d active state: %d source: %d %s_removed%s\n", _line, _function, _state, _source, KBLU, KNRM);
#endif
}

/**************************************************************************//**
* \brief  - calls lower layer driver to assert a specific output line to a value. 
*
* \param  - value - i.e. OUTPUT_LOW
*
* \return  - outputSet - output was set successfully or not 
*
******************************************************************************/
bool DigitalOutput::SetValue (LINE_VALUE value)
{
	_CHECKRETURN(value<DigitalIO::LINE_NONE,false)
	bool outputSet = false;

	// call IO driver API here
	if (_source == DigitalIO::SOURCE_POWERSUPPLY)
	{
		if (_state == STATE_ACTIVE_HIGH)
		{
			if (ScDgtSetDoutValue ((_line - DIGITAL_IO_PS_OUTPUT_START) , (UINT16) value == LINE_HIGH ? 1:0 ) == DIO_OUTPUTS_OK)
				outputSet = true;
		}
		else if (_state == STATE_ACTIVE_LOW)
		{
			if (ScDgtSetDoutValue ((_line - DIGITAL_IO_PS_OUTPUT_START), (UINT16) value == LINE_HIGH ? 0:1 ) == DIO_OUTPUTS_OK)
				outputSet = true;
		}
	}
	else if (_source == DigitalIO::SOURCE_ACTUATOR)
	{
		if (_state == STATE_ACTIVE_HIGH)
		{
			if (value == LINE_HIGH)
				AC_RX->ACOutputs |= (1 << MapJ710ToJ58 (_line - DIGITAL_IO_AC_OUTPUT_START));
			else if (value == LINE_LOW)
				AC_RX->ACOutputs &= ~(1 << MapJ710ToJ58 (_line - DIGITAL_IO_AC_OUTPUT_START));
		}
		else if (_state == STATE_ACTIVE_LOW)
		{
			if (value == LINE_HIGH)
				AC_RX->ACOutputs &= ~(1 << MapJ710ToJ58 (_line - DIGITAL_IO_AC_OUTPUT_START));
			else if (value == LINE_LOW)
				AC_RX->ACOutputs |= (1 << MapJ710ToJ58 (_line - DIGITAL_IO_AC_OUTPUT_START));
		}
		
		outputSet = true;
	}

	return outputSet;
}


/**************************************************************************//**
* \brief  - Maps output line on J710 to J58 DB connector. 
*
* \param  - UINT16 line
*
* \return  - UINT16 j58_line 
*
******************************************************************************/
UINT16 DigitalOutput::MapJ710ToJ58 (UINT16 line)
{
	UINT16 j58_line = sizeof(UINT8) * 8;

	switch (line)
	{
	case 0:
		j58_line = 0;
		break;
	case 1:
		j58_line = 2;
		break;
	case 2:
		j58_line = 4;
		break;
	case 3:
		j58_line = 1;
		break;
	case 4:
		j58_line = 3;
		break;
	default:
		break;
	}
	
	return j58_line;
}


/**************************************************************************//**
* \brief  - returns the output function to the DigitalIOMgr as part of an output
* 			scan prior to asserting the line
*
* \param  - none
*
*
* \return  - output function
*
******************************************************************************/
DigitalIO::LINE_FUNCTION DigitalOutput::GetFunction 	()
{
	return _function;
}

/**************************************************************************//**
* \brief  - returns the output state to the DigitalIOMgr as part of an output
* 			scan prior to asserting the line
*
* \param  - none
*
*
* \return  - output line state
*
******************************************************************************/
DigitalIO::LINE_STATE DigitalOutput::GetState ()
{
	return _state;
}

/**************************************************************************//**
* \brief  - returns the output source to the DigitalIOMgr as part of an output
* 			write
*
* \param  - none
*
*
* \return  - output line source
*
******************************************************************************/
DigitalIO::LINE_SOURCE DigitalOutput::GetSource()
{
	return _source;
}

/**************************************************************************//**
* \brief  - create input resource upon request based on line, function,
* 			convention and source of IO and retain this setup 
*
* \param  - line - physical line number of this IO
* \param  - function - digital function of this IO
* \param  - state - STATE_ACTIVE_LOW or STATE_ACTIVE_HIGH 
* \param  - source - SOURCE_POWERSUPPLY or  SOURCE_ACTUATOR
*
* \return  - none
*
******************************************************************************/
DigitalInput::DigitalInput (UINT16 line, LINE_FUNCTION function, LINE_STATE state, LINE_SOURCE source) : DigitalIO (line, function, state, source)
{
	_value = LINE_NONE;
#ifdef DIO_DBG
	printf ("\nCTRL_T : ___Input: %d function: %d active state: %d source: %d %s_configured%s\n", _line, _function, _state, _source, KGRN, KNRM);
#endif
}

/**************************************************************************//**
* \brief  - relinquish resources upon deletion 
*
* \param  - none
*
* \return  - none
*
******************************************************************************/
DigitalInput::~DigitalInput () 
{
#ifdef DIO_DBG
	printf ("\nCTRL_T : ___Input: %d function: %d active state: %d source: %d %s_removed%s\n", _line, _function, _state, _source, KBLU, KNRM);
#endif
}


/**************************************************************************//**
* \brief  - returns the input function to the DigitalIOMgr as part of an input
* 			scan prior to reading the line
*
* \param  - none
*
*
* \return  - output function
*
******************************************************************************/
DigitalIO::LINE_FUNCTION DigitalInput::GetFunction 	()
{
	return _function;
}


/**************************************************************************//**
* \brief  - returns the input source to the DigitalIOMgr as part of an input
* 			read
*
* \param  - none
*
*
* \return  - input line source
*
******************************************************************************/
DigitalIO::LINE_SOURCE DigitalInput::GetSource()
{
	return _source;
}

/**************************************************************************//**
* \brief  - returns the input state to the DigitalIOMgr as part of an input
* 			scan prior to reading the line
*
* \param  - none
*
*
* \return  - input line state
*
******************************************************************************/
DigitalIO::LINE_STATE DigitalInput::GetState ()
{
	return _state;
}


/**************************************************************************//**
* \brief  - returns local value for a specific input
*
* \param  - none
*
*
* \return  - LINE_VALUE - LINE_HI, LINE_LOW or LINE_ERROR
*
******************************************************************************/
DigitalIO::LINE_VALUE DigitalInput::GetValue ()
{
	return _value;
}

/**************************************************************************//**
* \brief  - stores local input value for a specific input read 
*
* \param  - value - LINE_VALUE - LINE_HI, LINE_LOW
*
*
* \return  - nothing
*
******************************************************************************/
void DigitalInput::SetValue (DigitalIO::LINE_VALUE value)
{
	_CHECK(value<DigitalIO::LINE_NONE)
			
	_value = value;
}

UINT16 DigitalInput::MapJ710ToJ58(UINT16 line)
{
	UINT16 j58_line = sizeof(UINT8) * 8;
	
	switch (line)
	{
	case 0:
		j58_line = 0;
		break;
	case 1:
		j58_line = 2;
		break;
	case 2:
		j58_line = 4;
		break;
	case 3:
		j58_line = 1;
		break;
	case 4:
		j58_line = 3;
		break;
	default:
		break;
	}
	
	return j58_line;
}
