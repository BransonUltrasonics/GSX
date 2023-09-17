/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2012
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 It contains the Hilscher IOImage Handling related implementation   
 
**********************************************************************************************************/

#include <iostream>

#include "IOImageHandler.h"
#include "EthernetIP.h"
#include "Common.h"
#include "cifXToolkit.h"
#include "cifXErrors.h"
#include "VendorSpecificInfo.h"
#include "FieldBus.h"
//#include "cifXHWFunctions.h"
//#include "SerialDPMInterface.h"
//#include "OS_Spi.h"

//<! Testing purpose of cyclic data.

/*****************************************************************************/
/*! Displays a hex dump on the debug console (16 bytes per line)
*   \param pbData     Pointer to dump data
*   \param ulDataLen  Length of data dump                                    */
/*****************************************************************************/
static void DumpData(uint8_t* pbData, uint32_t ulDataLen)
{
  uint32_t ulIdx = 0;
  for(ulIdx = 0; ulIdx < ulDataLen; ++ulIdx)
  {
    if(0 == (ulIdx % 16))
      printf("\r\n");

    printf("%02X ", pbData[ulIdx]);
  }
  printf("\r\n");
}

/**************************************************************************//**
* 
* \brief   - Constructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
IOImageHandler::IOImageHandler()
{
	CP = CommonProperty::getInstance();
		
	CTRL_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[CTRL_T]);
	ALARM_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[ALARM_T]);
}

/**************************************************************************//**
* 
* \brief   - Destructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
IOImageHandler::~IOImageHandler()
{

}

/**************************************************************************//**
* 
* \brief   - UpdateIOImage
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
INT32 IOImageHandler::UpdateIOImage(/*PCHANNELINSTANCE PtChannel*/)
{
	INT32 lRet = CIFX_GENERAL_ERROR;
	static uint8_t readErrorPrintOnce, readSuccessPrintOnce  = 0,
				   writeErrorPrintOnce, writeSuccessPrintOnce = 0;
	
//	//PIOINSTANCE PtCommIn;
//	//PIOINSTANCE PtCommOut;
//	
//	//Check if Channel is still open
//	if(DEV_IsCommunicating(PtChannel, &Ret)){
//		
//		//Handle Input data
//		
//		/*Check handshake mode*/
//			/*if(Uncontrolled mode)*/
//				/*Get data from DPM*/
//				PtDevInstance->pfnHwIfRead(PtDevInstance, PtChannel->pptIOInputAreas[0]->pbDPMAreaStart, &RecvBuffer, 6);
//			/*else (Controlled mode)*/
//				/*Wait for flag*/
//				/*Get data from DPM*/
//			/*End if*/
//	
		
		UpdateOutputCyclic(SendBuffer);

	lRet = xChannelIORead(FieldBus::HChannel, 0, 0, sizeof(RecvBuffer), RecvBuffer, 10);
	if(CIFX_NO_ERROR != lRet)
	{
		readSuccessPrintOnce = 0;
		if(0 == readErrorPrintOnce)
		{
			LOGERR("Error reading IO Data area. Error: %08x\r\n", lRet, 0, 0);
			readErrorPrintOnce = 1;
		}
	}
	else
	{
		readErrorPrintOnce = 0;
		if(0 == readSuccessPrintOnce)
		{
			LOGDBG("IO Data read success\r\n", 0,0,0);
			readSuccessPrintOnce = 1;
		}
	}
	UpdateInputCyclic(RecvBuffer);
		
	lRet = xChannelIOWrite(FieldBus::HChannel, 0, 0, sizeof(SendBuffer), SendBuffer, 10);
	if(CIFX_NO_ERROR != lRet)
	{
		writeSuccessPrintOnce = 0;
		if(0 == writeErrorPrintOnce)
		{
			LOGERR("Error writing to IO Data area. Error: %08x\r\n", lRet, 0, 0);
			writeErrorPrintOnce = 1;
		}
	}
	else
	{
		writeErrorPrintOnce = 0;
		if(0 == writeSuccessPrintOnce)
		{
			LOGDBG("IO Write Data successful\r\n", 0,0,0);
			writeSuccessPrintOnce = 1;
		}

	}

//	}
//	
//		//Handle Output data
//	
//		/*Check handshake mode*/
//			/*if(Uncontrolled mode)*/
//				/*Set data to DPM*/
//				PtDevInstance->pfnHwIfWrite(PtDevInstance, PtChannel->pptIOOutputAreas[0]->pbDPMAreaStart, &SendBuffer, 6);
//			/*else (Controlled mode)*/
//				/*Wait for flag*/
//				/*Get data from DPM*/
//			/*End if*/
//	}
//	else{
//		//Increase communication counter timeout
//		/*if (counter > MAX_timeout)*/
//			//Trigger alarm/event
//			//attempt reconnection
//		/*else*/
//			//Loop
//	}
//	
//	/*End if*/

	return lRet;
}

/**************************************************************************//**
* 
* \brief   - ProcessControlSignal
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void IOImageHandler::ProcessControlSignal()
{
	
}

/**************************************************************************//**
* 
* \brief   - PrepareStatusSignal
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void IOImageHandler::PrepareStatusSignal()
{
	
}

/**************************************************************************//**
* 
* \brief   - IOImage Handler Task
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void IOImage_Handler_Task(void)
{
	UINT32 iEvents = 0, iRetries = 0;
	INT32 lRetErr = CIFX_GENERAL_ERROR,
			StatusProtocolSpecific,
			BasicConfigError= CIFX_GENERAL_ERROR;
	
	//TO_BE_DONE - Check if Hilscher card is configured as Ethernet/IP or Profibus and then decide 
	//Whether to get EthernetIP or Profibus  object instance.
	//If protocol used is EIP
		EthernetIP  *FbPtr = EthernetIP::Getinstance();
	//Else if protocol is PBus
		//Profibus *FbPts = Profibus::Getinstance();
	
	if(FbPtr != NULL)
	{
		//Provisional delay to wait for the module to be instantiated before SPI configuration
		//TODO implement handshake instead of delay
		MsDelay(15000);
		
		//Initialize Everything specific to Hilscher interface
		do{
			
			lRetErr = FbPtr->HilscherInit();
			
			if(CIFX_NO_ERROR != lRetErr)
			{
				iRetries++;
				LOGWARN("Hilscher initialization number of retries: %d\n",iRetries,0,0);
				MsDelay(1000);
			}
			
		}while((CIFX_NO_ERROR != lRetErr) && (MAX_FB_CONIFG_RETRIES > iRetries));
		
		if(CIFX_NO_ERROR == lRetErr)
		{
			// Register vendor specific objects and configure protocol settings
//			StatusProtocolSpecific= FbPtr->ProtocolSpecificInit(FieldBus::HChannel);
		}
		
		/*Trigger Fieldbus configuration alarm here*/
		if(CIFX_NO_ERROR != lRetErr || CIFX_NO_ERROR != StatusProtocolSpecific)
		{
			//TODO AlarmMgr::EnterAlarmEvent(ALARM_FB_CONFIG);
		}
		
		FbPtr->configuration();
// FbPtr->configuration() function will be used as provitional configuration code
// ToDo: Use FbPtr->BasicConfigurationPacketSet() function instead in the future
/*		 BasicConfigError= FbPtr->BasicConfigurationPacketSet();
		if (CIFX_NO_ERROR != BasicConfigError)
		{
// ToDo: Placeholder for  Hilscher Configuration Failure alarm
			LOGWARN("Hilscher Configuration Failure alarm\n", 0, 0, 0);
		}
*/		
		//<! This function call sets the flags require for communication handshake.
		FbPtr->InitIOCommunication();

		FbPtr->SetFieldBusInitialised();
		
		while(true)
		{
			//Wait on an event here. ControlTask should trigger this event every 1 millisecond.
			if(eventReceive(IO_IMAGE_HANDLE_EVENT,EVENTS_WAIT_ANY, WAIT_FOREVER, &iEvents) != ERROR)
			{
				//<!ToDo: add type of connection to check for only implicit owner connection.
				if(FbPtr->GetConnectionState()) //check ConnectionState -> EIP_OBJECT_CHANGE_IND indication to the HOST application.
				{
					FbPtr->UpdateIOImage();
				}
				else
				{
					//<! do nothing wait for re-connection.
				}
			}
		}
	}
}
