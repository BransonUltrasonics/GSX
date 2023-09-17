/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef MAIL_BOX_HANDLER_H_
#define MAIL_BOX_HANDLER_H_

#include "CommonProperty.h"
#include "cifXUser.h"
#include "rngLib.h"

//MailBoxHandler Abstract class.
class MailBoxHandler
{
public:	
	MailBoxHandler();
	virtual ~MailBoxHandler();
	
	//<! Initialize queue to hold the acyclic mailbox packets need to be sent to Hilschier slave card.
	RING_ID CIFXPKT_BUFFER_ID;
	
	INT32 UpdateMailBox();

	virtual bool ProcessMailBox(CIFX_PACKET *MBXPkt) = 0;
		
private:
	MSG_Q_ID MAIL_BOX_HANDLER_MSG_Q_ID;
	CommonProperty 	*CP;
	char szMailBoxMsgBuffer[MAX_SIZE_OF_MSG_LENGTH];
	
	friend void Mail_Box_Message_Handler_Task(void);
};

#endif /* MAIL_BOX_HANDLER_H_ */
