/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef SOCKETRECEIVER_H_
#define SOCKETRECEIVER_H_

#include "Common.h"
#include "CommonProperty.h"
#include "MainTask.h"
#include "FirmwareUpgrade.h"
#include "AuthenticationKey.h"

#define PORT_CFG_PATH			"/emmc0:0/Port.cfg"
#define SOCKET_RETRY_ATTEMPT	2

class SocketReceiver
{
public:
	SocketReceiver();
	~SocketReceiver();

	MSG_Q_ID		GetUIMsgQId();
	bool			bIsTaskRunStatus();
	bool 			GetSocketPort(INT32& Port);
	void 			ProcessMessage(Message& SockMsg);
	void 			SendEvent(TASK_ID &tid, INT32 evt);
	UINT32 			SetMsgIdAndLen(char *MsgIdLen,INT32& iMsgId);
	STATUS 			SendToMsgQ(const MSG_Q_ID& MsgQID, Message& msg);
	void 			SendMessageToClient(Client_Message& SendBuffer,UINT32 SockMsgLen);
	void 			MessageDeCoder(INT32 iMsgId, UINT32 iLen, const char *RecvBuffer, Message& SockMsg);
	void 			SendSysReadyIndToUIC(Message& sysReadyIndMsg);
	void			FirmWareUpgradeHandler();
	void			CloseClientConnection(INT32 fdClienSocket, Message& sockMsg);
	void			LogSocketConnectionLostAlarm();
	void			GetSystemModel();
	void			GetSystemConfiguration();
	void 			SendWebServicesLoggedInStatus();
	void 			setHMILoginStatus(char *buff);
	int test();
	
private:
	void			SendHeartBeatResponse();
	
private:
	MSG_Q_ID 		m_UI_Q_ID, m_UI_DB_Q_ID, m_FW_MSG_Q_ID,WEBSERVICE_MSG_Q_ID, m_DB_Q_ID;
	TASK_ID			m_RtpMain_TID,m_FW_TID;
	Client_Message  Client;
	CommonProperty *CP;
	FirmwareUpgrade m_FWUpgrdObj;
	AuthenticationKey m_AuthenticationKeyObj;
	PowerUpData		PowerUpRecipe;
	bool			m_bIsFWTask;
};

#endif /* SOCKETRECEIVER_H_ */
