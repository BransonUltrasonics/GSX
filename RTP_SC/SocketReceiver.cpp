/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Socket receiver task to receive inputs from QT UI  
 
**********************************************************************************************************/


#include "SocketReceiver.h"
#include <fstream.h>
#include "SasSocket.h"
#include "CommunicateInterface.h"
#include "RunTimeFeature.h"
#include "AlarmManager.h"
#include "ControlTask.h"
#include "UIDBInterface.h"
#include "Eeprom.h"
#include "CommonProperty.h"
#include "EthernetIP.h"
#include "Hil_ApplicationCmd.h"
#include "FieldBus.h"

using namespace std;

/**************************************************************************//**
* \brief    - SocketReciever Class constructor
*
* \param    - None
*
* \return   - None
*
******************************************************************************/
SocketReceiver::SocketReceiver() : m_UI_Q_ID(0),WEBSERVICE_MSG_Q_ID(0), m_UI_DB_Q_ID(0), m_FW_MSG_Q_ID(0), m_FW_TID(0), m_bIsFWTask(false), m_DB_Q_ID(0)
{
	CP 				= CommonProperty::getInstance();
	m_UI_Q_ID		= CP->GetMsgQId(cTaskName[UI_T]);
	m_UI_DB_Q_ID	= CP->GetMsgQId(cTaskName[UI_DB_T]);	
	m_RtpMain_TID	= CP->GetTaskId(cTaskName[MAIN_T]);
	m_DB_Q_ID		= CP->GetMsgQId(cTaskName[DB_T]);
	WEBSERVICE_MSG_Q_ID = CP->GetMsgQId(cTaskName[WEBSERVICE_T]);
}

/**************************************************************************//**
* 
* \brief   - Decodes the incoming message received from Q
*
* \param   - INT32 MsgId, const char *RecvBuffer, Message& message.
*
* \return  - None
*
******************************************************************************/
void SocketReceiver::MessageDeCoder(INT32 iMsgId , UINT32 iLen, const char *RecvBuffer, Message& message) 
{
	message.msgID = iMsgId;
	memcpy(message.Buffer, RecvBuffer, iLen);
}

/**************************************************************************//**
* 
* \brief   - Process the incoming message received from Client Socket
*
* \param   - Message& SockMsg.
*
* \return  - None
*
******************************************************************************/
void SocketReceiver::ProcessMessage(Message& SockMsg)
{
	UINT32 SockMsgLen = 0;
		
	switch(SockMsg.msgID)
	{
		case SCBL_HEART_BEAT_REQ:
			SendHeartBeatResponse();
			break;
			
		case SCBL_IP_CONFIGURATION_READ_REQ :
			SockMsg.msgID = TO_UI_TASK_IP_CONFIGURATION_READ_REQ;
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			break;
			
		case SCBL_IP_CONFIGURATION_WRITE_REQ :
			SockMsg.msgID = TO_UI_TASK_IP_CONFIGURATION_WRITE_REQ;
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			break;
		
		case SCBL_FEATURERUN_READ_REQ:
			/*
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			*/
			GetSystemModel();
			break; 

		case SCBL_FEATURERUN_WRITE_REQ:
			SockMsg.msgID = TO_UI_TASK_FEATURERUN_WRITE_REQ;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_DEFAULT_FEATURERUN_REQ:
			SockMsg.msgID = TO_UI_TASK_DEFAULT_FEATURERUN_REQ;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break; 


		case SCBL_FACTORY_RESET_REQ:
			SockMsg.msgID = TO_UI_TASK_FACTORY_RESET_REQ;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break; 

		case SCBL_SYSCONFIG_READ_REQ:
			/*
			SockMsg.msgID = TO_UI_TASK_SYS_CONFIG_READ_REQ;
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			*/
			GetSystemConfiguration();
			break;

		case SCBL_SYSCONFIG_WRITE_REQ:
			SockMsg.msgID = TO_UI_TASK_SYS_CONFIG_WRITE_REQ;
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			break;
			
		case SCBL_SYSINFO_READ_REQ:
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;	
			
		case SCBL_WELDRECIPE_REQ_SC:
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_WELDRECIPE_REQ:
			SockMsg.msgID = TO_UI_TASK_WELD_RECIPE;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
		
		case SCBL_SEEK_RECIPE_REQ:
			SockMsg.msgID = TO_UI_TASK_SEEK_RECIPE;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_STACK_RECIPE_REQ:   
		    SockMsg.msgID = TO_UI_TASK_STACK_RECIPE; 
			SendToMsgQ(m_UI_Q_ID, SockMsg);
		    break;
		    
		case TO_UI_TASK_HORN_SCAN_RECIPE:
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
				
		case SCBL_SETUP_CMD_REQ: 
			SockMsg.msgID = TO_UI_TASK_SETUP_CMD;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;

		case SCBL_SET_NEXT_OPERATION_REQ: 
			SockMsg.msgID = TO_UI_TASK_SET_NEXT_OPERATION_REQ;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;

		case SCBL_SEEK_CMD_REQ:
			SockMsg.msgID = TO_UI_TASK_SEEK_CMD;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
		
		case TO_UI_TASK_HORN_SCAN_CMD:
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
		
		case TO_UI_TASK_HORN_SCAN_ABORT_CMD:
			SendToMsgQ(m_UI_Q_ID, SockMsg);		
			break;

		case SCBL_ALARM_RESET_REQ:
			SockMsg.msgID = TO_UI_TASK_RESET_ALARM_CMD;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_ALARM_RESET_ALL_REQ:
			SockMsg.msgID = TO_UI_TASK_CLEAR_ALL;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;

		case SCBL_ACTIVE_ALARM_REQ:
			SockMsg.msgID = TO_UI_TASK_ACTIVE_ALARM_REQ;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_SC_STATE_REQ:
			SockMsg.msgID = TO_UI_TASK_SC_STATE_REQ;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
		case SCBL_TEST_RECIPE_REQ:				
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;

		case SCBL_TEST_CMD_REQ:
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;

		case SCBL_TEST_ABORT_CMD_REQ:
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_CALIBRATION_CMD_REQ:
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_SYS_DATETIME_READ_REQ:
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			break;

		case SCBL_SYS_DATETIME_WRITE_REQ:
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;

		case SCBL_DEFAULT_ALARM_CONFIG_REQ:
			SockMsg.msgID = TO_UI_TASK_DEFAULT_ALARM_CONFIG_REQ;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;

		case SCBL_ALARM_CONFIG_READ_REQ:
			SockMsg.msgID = TO_UI_TASK_ALARM_CONFIG_READ_REQ;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;

		case SCBL_ALARM_CONFIG_SAVE_REQ:
			SockMsg.msgID = TO_UI_TASK_ALARM_CONFIG_SAVE_REQ;				
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;

		case SCBL_EMERGENCY_STOP_RESET_REQ:
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_DB_FILE_COPY_TO_USB_REQ:
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_DB_FILE_DELETE_FROM_USB_REQ:
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_BARCODE_RECIPE_RECALL_AUTHENTICATION_CHECK_RES:
			SockMsg.msgID = TO_UI_TASK_BARCODE_RECIPE_RECALL_AUTHENTICATION_CHECK_RES;				
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_BARCODE_RECIPE_RECALL_RSP:
			SockMsg.msgID = TO_UI_TASK_BARCODE_RECIPE_RECALL_RSP;				
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SHUTDOWN_APPLICATION:
			SendEvent(m_RtpMain_TID,SHUTDOWN_EVENT);
			break;
		
		case SCBL_BATCH_COUNT_RESET_REQ:
			SockMsg.msgID = TO_UI_TASK_BATCH_COUNT_RESET_REQ;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_GEN_SINGLE_REPORT_REQ :
			SockMsg.msgID = TO_UI_TASK_GEN_SINGLE_REPORT_REQ;
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			break;
			
		case SCBL_GEN_REPORT_ACTION_IND :
			SockMsg.msgID = TO_UI_TASK_GEN_REPORT_ACTION_IND;
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			break;
			
		case UIC_GEN_DB_REPORT_REMIND_LATER_REQ :
			SockMsg.msgID = TO_UI_GEN_DB_REPORT_REMIND_LATER_REQ;
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			break;
			
		case SCBL_FW_UPGRADE_REQ:
			m_FWUpgrdObj.ReadUSBResponseToUI();
			break;
			
		case SCBL_EIP_ACD_INFO_REQ:
			EthernetIP  *m_ACDInfoObj = EthernetIP::Getinstance();
			m_ACDInfoObj->SendACDInformation();
			break;

		case SCBL_USERIO_READ_REQ:
			SockMsg.msgID = TO_UI_TASK_USERIO_READ_REQ;
			SendToMsgQ(m_UI_Q_ID, SockMsg);
			break;
			
		case SCBL_FW_UPGRADE_CMD_REQ:
			if(!m_bIsFWTask)
			{
				FirmWareUpgradeHandler();
			}
			SockMsg.msgID = TO_FWUP_TASK_FW_UPGRADE_CMD_REQ;
			SendToMsgQ(m_FW_MSG_Q_ID,SockMsg);
			break;
			
		case SCBL_FW_UPGRADE_ETHER_REQ:
			if(!m_bIsFWTask)
			{
				FirmWareUpgradeHandler();
			}
			SockMsg.msgID = TO_FWUP_TASK_FW_UPGRADE_ETHER_REQ;
			SendToMsgQ(m_FW_MSG_Q_ID,SockMsg);
			break;
					
		case SCBL_BRANSON_KEY_REQ:
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			break;
			
		case SCBL_MEMORY_OFFSET_RESET_REQ:
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			break;
		
		case SCBL_WEBSERVICES_LOGIN_REQ:
			SendWebServicesLoggedInStatus();
			break;
			
		case SCBL_EMMC_WEAR_LEVEL_REQ:	
			LOGDBG("\nSOCKET_T : SCBL_EMMC_WEAR_LEVEL_REQ\n",0,0,0);
			SockMsg.msgID = TO_UI_TASK_EMMC_WEAR_LEVEL_REQ;
			SendToMsgQ(m_UI_Q_ID,SockMsg);
			break;
			
		case REQ_STOP_COLLECT_GRAPH_DATA:
			LOGDBG("\nSOCKET_T : not collect graph data\n",0,0,0);
		    CP->ind_CollectGraphData=(char)(atoi)(SockMsg.Buffer);			
		    break;

		case SCBL_OPTIMIZE_DATABASE_REQ:
			SockMsg.msgID = TO_UI_TASK_OPTIMIZE_DATABASE_REQ;
			SendToMsgQ(m_UI_Q_ID,SockMsg);
		    break;
		
		case SCBL_SET_HMI_LOGIN_REQ:
			setHMILoginStatus(SockMsg.Buffer);		
			break;
	
		case SCBL_WEBSERVICE_UI_VERSION_RES:
			SendToMsgQ(WEBSERVICE_MSG_Q_ID,SockMsg);
			break;
			
		case SCBL_PRIVATE_KEY_REQ:
			m_AuthenticationKeyObj.ReadUSBResponseForPrivateKeyToUI();
			break;
			
		case SCBL_PRIVATE_KEY_UPLOAD_REQ:
			m_AuthenticationKeyObj.ReadPrivateKeyFromFile(SockMsg.Buffer);
			break;
			
		case SCBL_WS_LOGIN_PERMISSION_RES:
			SendToMsgQ(WEBSERVICE_MSG_Q_ID,SockMsg);
			break;
			
		case SCBL_EIP_IP_CONFIG_REQ:
			EthernetIP  *m_IPStaticInfoObj = EthernetIP::Getinstance();
			
			//<! Get the IP address at TCP/IP Object - Attribute 5.
			m_IPStaticInfoObj->GetAttribute(TCPIP_INTERFACEOBJECT_CLASS, TCPIP_INTERFACEOBJECT_INSTANCE, TCPIP_INTERFACEOBJECT_INTERFACECONFIG_ATTRIBUTE);
			
			/* ToDo: Function is called directly here to set IP address, 
				but need to call depending upon the HMI parameter.Also whether configured as static/dhcp */
			
			//<! Set Static IP Address function to set the user defined address from HMI.
			m_IPStaticInfoObj->SetStaticIPAddr();
			break;

		default:
			SendToMsgQ(m_UI_DB_Q_ID, SockMsg);
			//cout << "Recv_T : Invalid CMD received over socket : "  << SockMsg.msgID << endl;
			break;
	}
}

/**************************************************************************//**
 * \brief   - Create Message Queue and spaw Firmwrae Upgrade task
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void SocketReceiver::FirmWareUpgradeHandler() 
{
	/* Create message Queue */

	m_FW_MSG_Q_ID = msgQCreate(MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO);
	if(MSG_Q_ID_NULL != m_FW_MSG_Q_ID) 
	{
		
		CP->SetMsgQId(cTaskName[FWUPGRADE_T], m_FW_MSG_Q_ID);
	}
	else
	{
		LOGERR("Recv_T : FW upgrade message queue failed",0,0,0);
	}

	//Spawn Firmware Task
	m_FW_TID = taskSpawn((char *)cTaskName[FWUPGRADE_T],FW_UPGRADE_T_PRIORITY, 0, FW_UPGRADE_T_SATCK_SIZE,(FUNCPTR) FirmwareUpgrade_Task, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	if(TASK_ID_NULL != m_FW_TID) 
	{
		CP->SetTaskId(cTaskName[FWUPGRADE_T], m_FW_TID);
		m_bIsFWTask = true;
	}
	else
	{
		LOGERR("Recv_T : FW upgrade task spawn failed",0,0,0);
	}
}

/**************************************************************************//**
* 
* \brief   - Post the encoded message to destination message queue.
*
* \param   - const MSG_Q_ID& MsgQID, Message& msg
*
* \return  - STATUS - OK or ERROR
*
******************************************************************************/
STATUS SocketReceiver::SendToMsgQ(const MSG_Q_ID &MsgQID, Message& msg)
{
	STATUS ret = OK;
	
	if(msgQSend(MsgQID,reinterpret_cast<char*>(&msg),sizeof(msg),NO_WAIT,MSG_PRI_NORMAL) != OK)
	{
		LOGERR("SockRecv_T : Message send failed to MSG Q",0,0,0);
		ret = ERROR;
	}
	
	return ret;
}

/**************************************************************************//**
* 
* \brief   - Send events to consoleApp or MainTask
*
* \param   - TASK_ID& tid, INT32 evt
*
* \return  - None
*
******************************************************************************/
void SocketReceiver::SendEvent(TASK_ID &tid, INT32 evt)
{
	if(eventSend(tid,evt) != OK)
	{
		LOGERR("SendEvent : Event failed to invoke console task",0,0,0);
	}
}

/**************************************************************************//**
* 
* \brief   - Read the port details from file for socket connection.
*
* \param   - INT32 Port
*
* \return  - bool SUCCESS or ERROR
*
******************************************************************************/
bool SocketReceiver::GetSocketPort(INT32& Port)
{
	ifstream fp;
	string str;
	bool bIsRead = true;
	
	fp.open(PORT_CFG_PATH, ios::in);
	if(fp.is_open())
	{
		getline(fp,str);
		Port = atoi(str.c_str());
		fp.close();
	}
	
	if(Port <= 0)
	{
		bIsRead = false;
	}
	
	return bIsRead;
}

/**************************************************************************//**
* 
* \brief   - Read the Message ID and length from file for socket connection.
*
* \param   - MsgID, MessageIDLength
*
* \return  - length
*
******************************************************************************/
UINT32 SocketReceiver::SetMsgIdAndLen(char *MsgIdLen,INT32& iMsgId)
{
	INT32 len 	= 0;
	INT32 iPos 	= 0;
	memcpy(&iMsgId,MsgIdLen + iPos,sizeof(iMsgId));
	iPos += sizeof(iMsgId);
	memcpy(&len,MsgIdLen + iPos,sizeof(len));	
	return len;
}

/**************************************************************************//**
* 
* \brief   - Sends the message to client
*
* \param   - Client_Message& SendBuffer,UINT32 SockMsgLen
*
* \return  - None
*
******************************************************************************/
void SocketReceiver::SendMessageToClient(Client_Message &SendBuffer,UINT32 SockMsgLen)
{
	CommunicateInterface *InterfaceObject = CommunicateInterface::getinstance();

	if(SockMsgLen >= MAX_SIZE_OF_MSG_LENGTH)
	{
		LOGCRIT("Recv_T : SendMessageToClient, Socket message length exceeded : %d",SockMsgLen, 0, 0);
	}
	else
	{
		InterfaceObject->Send(reinterpret_cast<char*>(&SendBuffer),SockMsgLen);
	}
}

/**************************************************************************//**
* 
* \brief   - Get the UI task message queue ID .
*
* \param   - None
*
* \return  - MSG_Q_ID - UI_MSG_Q_ID
*
******************************************************************************/
MSG_Q_ID SocketReceiver::GetUIMsgQId()
{
	return m_UI_Q_ID;
}

/**************************************************************************//**
* 
* \brief   - Get the task life time status.
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool SocketReceiver::bIsTaskRunStatus()
{
	return CP->bIsTaskRun;
}

/**************************************************************************//**
* 
* \brief   - Closes the client socket descriptor and stops the system cycle
*
* \param   - INT32 fdClienSocket
* \param   - Message& sockMsg
*
* \return  - None
*
******************************************************************************/
void SocketReceiver::CloseClientConnection(INT32 fdClienSocket, Message& sockMsg)
{
	close(fdClienSocket);
	CP->bIsUIConnected = false;

	if (!CommonProperty::getInstance()->bIsWebServicesLogged)
	{
		/* When client socket closed(UIC) then SC-BL changes to NO_OPERATION operation type */
		UINT8 opType = NO_OPERATION;
		memset(sockMsg.Buffer,0x00,sizeof(sockMsg.Buffer));
	
		sockMsg.msgID = SCBL_SET_NEXT_OPERATION_REQ;
		memcpy(sockMsg.Buffer, &opType, sizeof(opType));
		ProcessMessage(sockMsg);
	}

	LOGERR("Client socket connection closed and waiting for client request",0,0,0);
}

/**************************************************************************//**
* 
* \brief   - Log alarm when socket connection lost.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SocketReceiver::LogSocketConnectionLostAlarm()
{
	AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_INTERNAL_COMM_ENET);
}

/**************************************************************************//**
* 
* \brief   - Sends the System ready or not indication to UI
*
* \param   - Message& sysReadyIndMsg
*
* \return  - None
*
******************************************************************************/
void SocketReceiver::SendSysReadyIndToUIC(Message& sysReadyIndMsg)
{
	SendToMsgQ(GetUIMsgQId(), sysReadyIndMsg);
}

/**************************************************************************//**
* 
* \brief   - Reads system model based on runtime configuration 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SocketReceiver::GetSystemModel()
{
	UINT32 numOfBytes = 0;
	Client_Message sendMsg;
	RunTime runTimeObj;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	
	sendMsg.msgID = UIC_FEATURERUN_READ_RES;
	
	if(ERROR != (runTimeObj.FeaturesFlagRead(EEPROM_MODEL_ADDRESS)))
	{
		memcpy(sendMsg.Buffer, &runTimeObj.ModelFeature, sizeof(FeaturesFlag));
		sendMsg.msglen 	= sizeof(FeaturesFlag);
		numOfBytes		= sizeof(sendMsg.msgID) + sizeof(sendMsg.msglen) + sizeof(FeaturesFlag);
	}
	else
	{
		sendMsg.msglen 	= sizeof(char);
		numOfBytes 		= sizeof(sendMsg.msgID) + sizeof(sendMsg.msglen) + sizeof(char);
	}
	
	if(numOfBytes >= MAX_SIZE_OF_MSG_LENGTH)
	{
		LOGCRIT("Recv_T : GetSystemModel, Socket message length exceeded : %d",numOfBytes, 0, 0);
	}
	else
	{
		SendMessageToClient(sendMsg, numOfBytes);
	}
}

/**************************************************************************//**
* 
* \brief   - Reads system configuration data from EEPROM 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void SocketReceiver::GetSystemConfiguration()
{
	UINT32 numOfBytes = 0;
	Client_Message sendMsg;
	SystemConfiguration SysConfig;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	
	sendMsg.msgID 	= UIC_SYSCONFIG_READ_RES;
	sendMsg.msglen 	= sizeof(intf_SysConfig_t);
	numOfBytes		= sizeof(sendMsg.msgID) + sizeof(sendMsg.msglen) + sizeof(intf_SysConfig_t);
	SysConfig.SysConfigGet(sendMsg.Buffer);
	
	if(numOfBytes >= MAX_SIZE_OF_MSG_LENGTH)
	{
		LOGCRIT("Recv_T : GetSystemConfiguration, Socket message length exceeded : %d",numOfBytes, 0, 0);
	}
	else
	{
		SendMessageToClient(sendMsg, numOfBytes);
	}
}

/**************************************************************************//**
* 
* \brief   - Sends response to UIC for heart beat request with parts  per minute
* 			 counter. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void SocketReceiver::SendHeartBeatResponse()
{
	CP->bIsUIConnected = true;
	UINT32 numOfBytes = 0;
	Client_Message sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
	
	sendMsg.msgID 	= UIC_HEART_BEAT_RES;
	sprintf(sendMsg.Buffer, "%d", ControlTask::GetPartsPerMinCounter());
	sendMsg.msglen	= strlen(sendMsg.Buffer);
	numOfBytes 		= sizeof(sendMsg.msgID) + sizeof(sendMsg.msglen) + strlen(sendMsg.Buffer);
	
	if(numOfBytes >= MAX_SIZE_OF_MSG_LENGTH)
	{
		LOGCRIT("Recv_T : SendHeartBeatResponse, Socket message length exceeded : %d",numOfBytes, 0, 0);
	}
	else
	{
		SendMessageToClient(sendMsg, numOfBytes);
	}
}

/**************************************************************************//**
* 
* \brief   - check whether web services is logged in or not
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void SocketReceiver::SendWebServicesLoggedInStatus()
{
	UINT32 numOfBytes = 0;
	Client_Message sendMsg;
	memset(sendMsg.Buffer, 0x00, sizeof(sendMsg.Buffer));
			
	sendMsg.msgID 	= UIC_WEBSERVICES_LOGIN_RES;
	sprintf(sendMsg.Buffer, "%d", CP->bIsWebServicesLogged);
	sendMsg.msglen	= strlen(sendMsg.Buffer);
	numOfBytes 		= sizeof(sendMsg.msgID) + sizeof(sendMsg.msglen) + sendMsg.msglen;
		
	if(numOfBytes >= MAX_SIZE_OF_MSG_LENGTH)
	{
		LOGCRIT("Recv_T : SendWebServicesLoggedInStatus, Socket message length exceeded : %d",numOfBytes, 0, 0);
	}
	else
	{
		SendMessageToClient(sendMsg, numOfBytes);
	}	
}

void SocketReceiver::setHMILoginStatus(char *pRecv_Msg)
{
	UINT8 opStatus;
		
	memcpy(&opStatus, pRecv_Msg, sizeof(opStatus));
	
	CP->bIsHMILogged = (HMISTATUS)opStatus;
}


/**************************************************************************//**
* 
* \brief   - Socket receiver class destructor
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
SocketReceiver::~SocketReceiver()
{	
}

/**************************************************************************//**
* 
* \brief   - Socket receiver task entry point
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void Socket_Recv(void)
{
	INT32  iPort		= 0;
	INT32  iMsgId		= 0;
	UINT32  iLen		= 0;
	INT32  Client_SockId= 0;
	INT32  iRetVal		= 0;
	UINT32 timeOutCount = 0;
	SocketReceiver *Recv= new(nothrow) SocketReceiver();
	char MsgIdLen[8]	= {0x00};
	char RecvBuffer[MAX_SIZE_OF_MSG_LENGTH-8]	= {0x00};	
	Message SockMsg;
	Client_Message	SendMsg;
	SasSocket socket;
	//Server is connecting with the new client 
	CommunicateInterface *InterfaceObject = CommunicateInterface::getinstance();
	bool bIsRet;
	
	if(NULL != Recv)
	{
		bIsRet = Recv->GetSocketPort(iPort);
		if(!bIsRet)
		{
			LOGERR("Recv_T : Error in reading Port.cfg file",0,0,0);
		}

		bIsRet = socket.Create();
		if(!bIsRet)
		{
			LOGERR("Socket creation failed",0,0,0);
		}

		bIsRet = socket.Bind(iPort);
		if(!bIsRet)
		{
			LOGERR("socket binding failed",0,0,0);
		}

		LOGDBG("PORT : %d",iPort,0,0);

		bIsRet = socket.Listen();
		if(!bIsRet)
		{
			LOGERR("socket listen failed",0,0,0);
		}

		Client_SockId = socket.Accept();

		if(!Client_SockId)
		{
			LOGERR("Error on socket accept",0,0,0);
		}
		else
		{
			InterfaceObject->dwsock 		= Client_SockId;
			InterfaceObject->msockaddr_in 	= socket.msockaddr_in;
			InterfaceObject->bIsServer 		= socket.bIsServer;

			/* System Ready Indication to UIC */ 
			SockMsg.msgID 		= TO_UI_TASK_SYSREADY_IND_REQ;
			Recv->SendSysReadyIndToUIC(SockMsg);
		}

		while(Recv->bIsTaskRunStatus()) 
		{
			/* Each count is equal to 3 seconds, On 3rd count, closes the client connection and accepts new. 
			 * Condition will be executed only when connection time out between client and server */
			if(timeOutCount > SOCKET_RETRY_ATTEMPT)
			{
				timeOutCount 	= 0;
				iRetVal 		= ERROR;
				
				if (!CommonProperty::getInstance()->bIsWebServicesLogged)
					Recv->LogSocketConnectionLostAlarm();
				
				LOGERR("Recv_T : Heart beat not received from client or socket connection broken",0,0,0);
			}
			else
			{
				memset(RecvBuffer,0x00,sizeof(RecvBuffer));
				memset(MsgIdLen,0x00,sizeof(MsgIdLen));
				memset(SockMsg.Buffer,0x00,sizeof(SockMsg.Buffer));
				iRetVal = InterfaceObject->ReadExact(MsgIdLen,sizeof(MsgIdLen));
			}
			
			if(ERROR == iRetVal)
			{
				Recv->CloseClientConnection(Client_SockId, SockMsg);
				Client_SockId = 0;
				Client_SockId = socket.Accept();
				timeOutCount  = 0;

				if(!Client_SockId)
				{
					LOGERR("Error on socket accept",0,0,0);
				}
				else
				{
					/* Server is connecting with the new client */ 
					CommunicateInterface *InterfaceObject = CommunicateInterface::getinstance();
					InterfaceObject->dwsock 		= Client_SockId;
					InterfaceObject->msockaddr_in 	= socket.msockaddr_in;
					InterfaceObject->bIsServer 		= socket.bIsServer;
				}
			}
			else if(0 == iRetVal)
			{
				/* Socket read time out and polling to read */
				timeOutCount = timeOutCount + 1;
			}
			else
			{
				timeOutCount  = 0;
#ifdef DEBUG
				LOGDBG("Msg received from socket",0,0,0);
#endif
				iLen = Recv->SetMsgIdAndLen(MsgIdLen,iMsgId);

				if(iLen < 0)
				{
					LOGERR("Recv_T : Invalid length received : %d, for MsgID : %d",iLen,iMsgId,0);
					SendMsg.msgID  = 1000;
					strcpy(SendMsg.Buffer,"Invalid Message Length");
					SendMsg.msglen = strlen(SendMsg.Buffer);
					
					iLen = sizeof(SendMsg.msgID) + sizeof(SendMsg.msglen) + SendMsg.msglen;
					if(iLen >= MAX_SIZE_OF_MSG_LENGTH)
					{
						LOGCRIT("Recv_T : Invalid length received and Socket message length exceeded : %d",iLen, 0, 0);
					}
					else
					{
						InterfaceObject->Send(reinterpret_cast<char*>(&SendMsg),iLen);
						LOGERR("Recv_T : Msg sent to client",0,0,0);
					}
				}
				else
				{
				 	if(iMsgId != 0)
					{
						LOGDBG("Recv_T : Length received : %d, for MsgID : %d",iLen,iMsgId,0);
					}
				 	iRetVal = InterfaceObject->ReadExact(RecvBuffer,iLen);
				 	if(ERROR == iRetVal)
				 	{
				 		Recv->CloseClientConnection(Client_SockId, SockMsg);
				 		Client_SockId = 0;
				 		timeOutCount  = 0;
				 	}
				 	else
				 	{
				 		Recv->MessageDeCoder(iMsgId,iLen,RecvBuffer,SockMsg);
				 		Recv->ProcessMessage(SockMsg);
				 	}
				}
			}
		}
		
		delete Recv;
	}
	else
	{
		LOGERR("SockServ_T : -------Memory allocation failed-----------",0,0,0);
	}
	
	Recv = NULL;
	LOGDBG("SockServ_T: ----------------Socket Receiver Task Terminated----------------",0,0,0);
}
