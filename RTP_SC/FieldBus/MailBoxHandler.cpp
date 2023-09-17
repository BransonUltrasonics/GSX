/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2012
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 It contains the Hilscher MailBox Handling related implementation   
 
**********************************************************************************************************/

#include <iostream>

#include "MailBoxHandler.h"
#include "EthernetIP.h"
#include "Common.h"
#include "cifXErrors.h"
#include "Logger.h"

/**************************************************************************//**
* 
* \brief   - Constructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
MailBoxHandler::MailBoxHandler()
{
	CP = CommonProperty::getInstance();
	
	memset(szMailBoxMsgBuffer, 0x00, sizeof(szMailBoxMsgBuffer));
	
	MAIL_BOX_HANDLER_MSG_Q_ID = CP->GetMsgQId(cTaskName[MAIL_BOX_HANDLER_T]);
	
	//<! Queue create with 30 Packets of CIFX_PACKET type.
	CIFXPKT_BUFFER_ID = rngCreate ((sizeof(CIFX_PACKET)) * 30);
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
MailBoxHandler::~MailBoxHandler()
{

}

/****************************************************************************
* 
* \brief   - UpdateMailBox. Checks if any packet is available for reading, 
*            if any available, reads it, and calls a function to process it
*            This function is called from Mail_Box_Message_Handler_Task() function.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
INT32 MailBoxHandler::UpdateMailBox()
{
	INT32	Ret = CIFX_GENERAL_ERROR;
	UINT32	PktCnt = 0,
			SendPktCnt = 0,
			ReceivePktCnt = 0;
	CIFX_PACKET RecvPkt, *SendPkt;
	char MailBoxBuff[CIFX_MAX_PACKET_SIZE];
	UINT32 SndPkt = 0;
	
// Read the mailbox state (packets pending to read / write)
	Ret = xChannelGetMBXState((PCHANNELINSTANCE)FieldBus::HChannel, &ReceivePktCnt, &SendPktCnt);
	if (CIFX_NO_ERROR != Ret)	// Unable to query the packet mailbox states
	{
		LOGCRIT("Unable to query the packet mailbox states", 0, 0, 0);
	}
	else 
	{	
	// Process receive packets
		if(ReceivePktCnt > 0)
		{
			for (PktCnt= 0; PktCnt < ReceivePktCnt; PktCnt++)
			{
				// Blank Receive Buffer
				memset(&RecvPkt, '\0', sizeof(RecvPkt));
				// Read a packet from the mailbox
				Ret= xChannelGetPacket ((PCHANNELINSTANCE)FieldBus::HChannel, sizeof(RecvPkt), &RecvPkt, GETPACKET_TIMEOUT);
				if (CIFX_NO_ERROR != Ret) 
				{
					LOGERR("Error Receiving Packet. Error Code: %08X!\r\n", Ret, 0, 0);
				}
				else
				{
					if (TRUE != ProcessMailBox(&RecvPkt))
					{	
						LOGERR("Error Processing Packet.\n", 0, 0, 0);					
					}
				} 
			}
		}
	 
		// Process send packets
		if(SendPktCnt > 0)
		{
			for (PktCnt = 0; PktCnt < SendPktCnt; PktCnt++)
			{
				// Setting the MailBoxBuff to zero.
				memset(MailBoxBuff, '\0', sizeof(MailBoxBuff));
				// Get every pending bytes to send packet from the RingId/mailbox queue.
				SndPkt = rngBufGet(CIFXPKT_BUFFER_ID, MailBoxBuff, sizeof(MailBoxBuff));
			
				if (SndPkt <= 0)
				{
					break;
				}
				SendPkt= (CIFX_PACKET*) MailBoxBuff;
				Ret = xChannelPutPacket((PCHANNELINSTANCE)FieldBus::HChannel, SendPkt, PUTPACKET_TIMEOUT);
				if (CIFX_NO_ERROR != Ret)
				{
					LOGERR("Error sending Packet. Command: %08X. Error Code: %08X!\r\n", SendPkt->tHeader.ulCmd, Ret, 0);
				}
			}
		}
	}
	return Ret;
}

/**************************************************************************//**
* 
* \brief   - MailBox Handler Task
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Mail_Box_Message_Handler_Task(void)
{
	UINT32 iEvents = 0;
	bool bIsFieldBusInitialized = false;
	do
	{
		bIsFieldBusInitialized = FieldBus::GetFieldBusInitialised();
		
		if(bIsFieldBusInitialized == true)
		{
			//TO_BE_DONE - Check if Hilscher card is configured as Ethernet/IP or Profibus and then decide 
			//whether to get EthernetIP or Profibus object instance.
			EthernetIP  *FbPtr = EthernetIP::Getinstance();	
		
			while(true)
			{	
				
				//Check if we have a message on message queue, if available then process it.
				if(eventReceive(MAILBOX_HANDLE_EVENT,EVENTS_WAIT_ANY,WAIT_FOREVER, &iEvents) != ERROR)
				{
					//Clear Buffer variable
					memset(FbPtr->szMailBoxMsgBuffer, 0x00, sizeof(FbPtr->szMailBoxMsgBuffer));
					
					FbPtr->UpdateMailBox();
					
				}
			}
		}
		else
		{
			//Wait for FieldBus to be ready/initialized
			taskDelay(1);
		}
	}while(bIsFieldBusInitialized == false);
	
}

