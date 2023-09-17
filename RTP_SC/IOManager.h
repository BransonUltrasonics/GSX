/*
 +* IOManager.h
 *
 *  Created on: Feb 2, 2018
 *      Author: DShilonie
 */

#ifndef IOMANAGER_H_
#define IOMANAGER_H_

#include "state.h"
#include "MainTask.h"
#include "IOFunctions.h"
#include <fstream>
#include "EthernetIP.h"

// *********** RBF AC only input line demo
#define DIO_DBG

#define	DIGITAL_IO_MAX_INPUTS 		14
#define	DIGITAL_IO_MAX_OUTPUTS 		13

#define	DIGITAL_IO_PS_INPUT_START 		0
#define	DIGITAL_IO_AC_INPUT_START 		9
#define	DIGITAL_IO_AC_INPUT_BIT_START 	0x08
#define	DIGITAL_IO_PS_OUTPUT_START 		0
#define	DIGITAL_IO_AC_OUTPUT_START 		8

enum
{
	IOMGR_REFRESH_OUTPUTS,
};

class DigitalIOMgr
{
public:
	
	enum
	{
		INPUT_READ_ERR = 0x01,
	};
	
	DigitalIOMgr ();
	~DigitalIOMgr ();
	
	DigitalIO::LINE_FUNCTION		GetInputFunction		(UINT8);
	DigitalIO::LINE_VALUE			GetInputValue 			(UINT8);
	DigitalIO::LINE_STATE			GetInputState 			(UINT8);
	
	bool							ReadConfig				(Message& message, DigitalIO::DIGITAL_FUNCTION inputs[DIGITAL_IO_MAX_INPUTS], 
																DigitalIO::DIGITAL_FUNCTION outputs[DIGITAL_IO_MAX_OUTPUTS]);
	bool							SetInputs 				(const DigitalIO::DIGITAL_FUNCTION inputs[DIGITAL_IO_MAX_INPUTS]);
	bool							SetOutputs				(const DigitalIO::DIGITAL_FUNCTION function[DIGITAL_IO_MAX_OUTPUTS]);
	void							RefreshOutputs			();
	
	static bool						GetStatus				(UINT16 bit);
	static void						ReadInputs				();
	static bool						ScanForInputFunction 	(DigitalIO::LINE_FUNCTION);
	static DigitalIO::INPUT_STATE	GetInputFromFunction 	(DigitalIO::LINE_FUNCTION);
	static bool						ScanForOutputFunction 	(DigitalIO::LINE_FUNCTION);
	static bool						SetOutputFromFunction 	(DigitalIO::LINE_FUNCTION, DigitalIO::LINE_VALUE val, const DigitalIO::LINE_VALUE values[] = 0);
	static bool						LogErr 					(const char* logErr);
		
	
private:
	
	void 							DeleteOutputs 			();
	void 							DeleteInputs 			();

	DigitalIO::LINE_FUNCTION    	AdjustPSInputFunction	(PowerSupplyInput ps_event);
	DigitalIO::LINE_FUNCTION    	AdjustACInputFunction	(ActuatorInput ac_event);
	DigitalIO::LINE_FUNCTION    	AdjustPSOutputFunction	(PowerSupplyOutput ps_event);
	DigitalIO::LINE_FUNCTION    	AdjustACOutputFunction	(ActuatorOutput ac_event);
	
	static DigitalIOMgr *			_pDigitalMgr;
	UINT16							_ioMgrStatus;
	
	DigitalIO *						_inputs[DIGITAL_IO_MAX_INPUTS];
	DigitalIO *						_outputs[DIGITAL_IO_MAX_OUTPUTS];
}; 	



#endif /* IOMANAGER_H_ */
