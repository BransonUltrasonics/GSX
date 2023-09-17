/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     This file Contains Communication Interface functions for the SC Module
 
**********************************************************************************************************/


#include "communicateInterface.h"


/**************************************************************************//**
* \brief  - Constructor
* 
* \param  - None
*
* \return - None
* 
******************************************************************************/
CommunicateInterface::CommunicateInterface()
{
}

/**************************************************************************//**
* \brief  - Destructor
* 
* \param  - None
*
* \return - None
* 
******************************************************************************/
CommunicateInterface::~CommunicateInterface() 
{
}

CommunicateInterface* CommunicateInterface::obj = NULL;

/**************************************************************************//**
* \brief  - Creates the new instance of the object if not present else returns 
* 			returns the object if already present.
* 
* \param  - None
*
* \return - CommunicateInterface* Object
* 
******************************************************************************/

CommunicateInterface* CommunicateInterface::getinstance()
{
	if(obj != NULL)
	{
		return obj;
	}
	else
	{
		obj = new CommunicateInterface;
	}
	return obj;
}


