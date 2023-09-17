/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Common file to send the SDO Upload and Download request for slaves  
 
**********************************************************************************************************/


#include "Common.h"

MSG_Q_ID SDO_MSG_Q_ID  = MSG_Q_ID_NULL; 

void SendSdoDownloadResp(UINT32 IndexAddr, UINT32 SlaveAddr, UINT32 ErrCode)
{
	STATUS MsgQStatus = OK;
	bool bIsCoeResp	  = true;
	Message msg;
	
	if(MSG_Q_ID_NULL == SDO_MSG_Q_ID)
	{
		AssignSdoMsgQID();
	}
	
	if(SlaveIdPC == SlaveAddr)
	{
		if(IndexAddr == WELD_RECIPE_RESP_PC_INDEX)
		{
			msg.msgID = TO_SDO_TASK_WELD_RECIPE_RESP_PC;
		}
		else if(IndexAddr == DOWNLOAD_POWERUP_RESP_INDEX)
		{
			msg.msgID = TO_SDO_TASK_DOWNLOAD_POWERUP_RESP;
		}
		else if(IndexAddr == HORN_SCAN_RECIPE_RESP_INDEX)
		{
			msg.msgID = TO_SDO_TASK_HORN_SCAN_RECIPE_RESP;
		}
		else if(IndexAddr == DOWNLOAD_SEEK_RESP_INDEX)
		{
			msg.msgID = TO_SDO_TASK_DOWNLOAD_SEEK_RESP;
		}
		else if(IndexAddr == DOWNLOAD_TEST_RESP_INDEX)
		{			
			msg.msgID = TO_SDO_TASK_TEST_RECIPE_RES;
		}
		else if(IndexAddr == DOWNLOAD_TMRINTERVAL_RESP_INDEX)		
		{ 
			msg.msgID = TO_SDO_TASK_DOWNLOAD_TMR_INTVL_RESP; 
		}
		else if(IndexAddr == FW_UPGRADE_INFO_RESP_PC_INDEX)
		{
			msg.msgID = TO_SDO_TASK_FW_UPGRADE_STATUS_RESP_PC;
		}
		else
		{
			bIsCoeResp = false;
		}
	}
	else if(SlaveIdAC == SlaveAddr)
	{
		if(IndexAddr == WELD_RECIPE_RESP_AC_INDEX)
		{
			msg.msgID = TO_SDO_TASK_WELD_RECIPE_RESP_AC;
		}
		else if(IndexAddr == CALIBRATION_READINGS_RESP_AC_INDEX)
		{
			msg.msgID = TO_SDO_TASK_CALIBRATION_READINGS_RESP_AC;
		}
		else if(IndexAddr == FW_UPGRADE_INFO_RESP_AC_INDEX)
		{
			msg.msgID = TO_SDO_TASK_FW_UPGRADE_STATUS_RESP_AC;
		}
		else
		{
			bIsCoeResp = false;
		}
	}
	else
	{
		bIsCoeResp = false;
		logMsg("EcMasterCommon : Invalid SDO DOWNLOAD RESP\n",0,0,0,0,0,0);
	}

	if(bIsCoeResp)
	{
		memcpy(msg.Buffer,&ErrCode,sizeof(ErrCode));	
		//logMsg("MSG ID : %d\n",msg.msgID,0,0,0,0,0);
		if(msgQSend(SDO_MSG_Q_ID,reinterpret_cast<char*>(&msg),sizeof(Message),NO_WAIT,MSG_PRI_NORMAL) != OK)
		{
			logMsg("EcMasterCommon : Download Response message sent failed to SDO Task\n",0,0,0,0,0,0);
		}
	}
}

void SendSdoUploadResp(UINT32 IndexAddr, UINT32 SlaveAddr, unsigned char *pData, UINT32 iLength)
{
	STATUS MsgQStatus = OK;
	bool bIsCoeResp	  = true;
	Message msg;

	if(MSG_Q_ID_NULL == SDO_MSG_Q_ID)
	{
		AssignSdoMsgQID();
	}
	
	if(SlaveIdPC == SlaveAddr)
	{
		if(IndexAddr == ALARM_DATA_RESP_PC_INDEX)
		{
			msg.msgID = TO_SDO_TASK_UL_ALARM_DATA_RESP_PC;
		}
		else if(IndexAddr == FW_DATA_RESP_PC_INDEX)
		{
			msg.msgID = TO_SDO_TASK_VERSION_REQUEST_RESP_PC;
		}
		else if(IndexAddr == FW_UPGRADE_INFO_RESP_PC_INDEX)
		{
			msg.msgID = TO_SDO_TASK_FW_UPGRADE_INFO_RESP_PC;
		}
		else
		{
			bIsCoeResp = false;
		}
	}
	else if(SlaveIdAC == SlaveAddr)
	{
		if(IndexAddr == ALARM_DATA_RESP_AC_INDEX)
		{
			msg.msgID = TO_SDO_TASK_UL_ALARM_DATA_RESP_AC;
		}
		else if(IndexAddr == FW_DATA_RESP_AC_INDEX)
		{
			msg.msgID = TO_SDO_TASK_VERSION_REQUEST_RESP_AC;
		}
		else if(IndexAddr == FW_UPGRADE_INFO_RESP_AC_INDEX)
		{
			msg.msgID = TO_SDO_TASK_FW_UPGRADE_INFO_RESP_AC;
		}
		else if(IndexAddr == CALIBRATION_READINGS_RESP_AC_INDEX)
		{
			msg.msgID = TO_SDO_CALIBRATION_DATA_REQUEST_RESP;
		}
		else
		{
			bIsCoeResp = false;
		}
	}
	else
	{
		bIsCoeResp = false;
		logMsg("EcMasterCommon : Invalid SDO UPLOAD RESP\n",0,0,0,0,0,0);
	}

	if(bIsCoeResp)
	{
		memcpy(msg.Buffer, pData, iLength);
		//logMsg("MSG ID : %d\n",msg.msgID,0,0,0,0,0);
		if(msgQSend(SDO_MSG_Q_ID,reinterpret_cast<char*>(&msg),sizeof(Message),NO_WAIT,MSG_PRI_NORMAL) != OK)
		{
			logMsg("EcMasterCommon : Upload Response message sent failed to SDO Task\n",0,0,0,0,0,0);
		}
	}
}

void AssignSdoMsgQID()
{
	SDO_MSG_Q_ID = msgQOpen(SDO_MSG_Q_NMAE, 0, 0, 0, 0, 0);
	
	if(MSG_Q_ID_NULL == SDO_MSG_Q_ID)
	{
	   	logMsg("SDO_MSG_Q_ID RETURNED NULL\n",0,0,0,0,0,0);
	}
}

void EnCodeErrorResp(char *DestMsg,char* ErrMsg, UINT32 ErrCode)
{
	UINT8 pos   		= 0; 
	UINT32 iLen = strlen(ErrMsg);
	logMsg("RESP MSG LEN: %d\n",iLen,0,0,0,0,0);
	printf("RESP MSG : %s\n",ErrMsg);
	memcpy(DestMsg + pos, &ErrCode, sizeof(ErrCode));
	pos += sizeof(ErrCode);
	memcpy(DestMsg + pos, &iLen, sizeof(iLen));
	pos += sizeof(iLen);
	memcpy(DestMsg + pos, ErrMsg, iLen);
}
