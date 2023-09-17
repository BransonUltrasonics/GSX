/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef IO_IMAGE_HANDLER_H_
#define IO_IMAGE_HANDLER_H_

#include "CommonProperty.h"

#define MAX_FBEXCHANGESIZE 240
#define MAX_FB_CONIFG_RETRIES 5

//IOImageHandler Abstract class.
class IOImageHandler
{
public:	
	IOImageHandler();
	virtual ~IOImageHandler();
	
	INT32 UpdateIOImage(/*PCHANNELINSTANCE PtChannel*/);
	void ProcessControlSignal();
	void PrepareStatusSignal();	
	virtual INT32 UpdateInputCyclic(UINT8* InputPtr) = 0;
	virtual INT32 UpdateOutputCyclic(UINT8* OutputPtr) = 0;
	
//	//Cyclic data sent to Fieldbus is first copied to this buffer.
	UINT8 SendBuffer[MAX_FBEXCHANGESIZE];
//	//Cyclic data received from Fieldbus is first copied to this buffer.
	UINT8 RecvBuffer[MAX_FBEXCHANGESIZE];
	
private:
	MSG_Q_ID CTRL_MSG_Q_ID;
	MSG_Q_ID ALARM_MSG_Q_ID;
	
	CommonProperty 	*CP;
	
	friend void IOImage_Handler_Task(void);
};

#endif /* IO_IMAGE_HANDLER_H_ */
