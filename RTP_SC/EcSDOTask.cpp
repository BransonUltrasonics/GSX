/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Service Data object task for Weld/Seek/Horn scan OD related data communication with slaves
 
**********************************************************************************************************/

#include "EcSDOTask.h"

using namespace std;

/**************************************************************************//**
* \brief   - Initialize and assign the message queue of DB and SDO.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
EcSDOTask::EcSDOTask() : SDO_MSG_Q_ID(0), DB_MSG_Q_ID(0),pRecvBuffer(NULL)
{
	CP			  = CommonProperty::getInstance();
	SDO_MSG_Q_ID  = CP->GetMsgQId(cTaskName[SDO_T]);
	UI_MSG_Q_ID   = CP->GetMsgQId(cTaskName[UI_T]);
	DB_MSG_Q_ID   = CP->GetMsgQId(cTaskName[DB_T]);
	CTRL_MSG_Q_ID = CP->GetMsgQId(cTaskName[CTRL_T]);
}

/**************************************************************************//**
* \brief   - Takes the local copy of weld results (SDO upload).
*
* \param   - UINT8 sdoULType, char *pRecvBuffer.
*
* \return  - None.
*
******************************************************************************/
void EcSDOTask::GetSdoFromOD(UINT8 sdoULType, char *pRecvBuffer)
{
	if(ALARM_DATA_PC == sdoULType)
	{
		/*	copy SDO DATA from ALARM	*/
		CP->m_PcAlarmObj.SetAlarmData(pRecvBuffer);
	}
	else if(ALARM_DATA_AC == sdoULType)
	{
		/*	copy SDO DATA from ALARM	*/
		CP->m_AcAlarmObj.SetAlarmData(pRecvBuffer);
	}
	else
	{
		/*	Error: weld result object not valid	*/
		LOGERR("Error on result : ",sdoULType,0,0);
	}
}

/**************************************************************************//**
* \brief   - Decodes the incoming message received from Q.
*
* \param   - char *pRecvBuffer, Message& message(dest buffer).
*
* \return  - None.
*
******************************************************************************/
void EcSDOTask::DeCoder(char *pRecvBuffer, Message& message) 
{	
	memcpy(&message, pRecvBuffer, sizeof(message));
}

/**************************************************************************//**
* \brief   - Post the encoded message to DB_Task message queue.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EcSDOTask::SendToMsgQ()
{
	if(msgQSend(DB_MSG_Q_ID, MsgBuffer,MAX_SIZE_OF_MSG_LENGTH, NO_WAIT, MSG_PRI_NORMAL) != OK)
	{
		LOGERR("SDO_T : -----------Error on msgQSend----------",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Post the encoded message to DB_Task message queue.
*
* \param   - Message& msg, MSG_Q_ID.
*
* \return  - None.
*
******************************************************************************/
void EcSDOTask::SendToMsgQ(Message& msg, const MSG_Q_ID &MsgQID)
{	
	if(msgQSend(MsgQID,reinterpret_cast<char*>(&msg), sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) != OK)
	{
		LOGERR("SDO_T : -----------Error on msgQSend----------",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Prepare Updated Weld recipe buffer with StartFreq.
*
* \param   - unsigned char *pRecipe.
*
* \return  - bool - status.
*
******************************************************************************/
bool EcSDOTask::GetPcWeldRecipe(unsigned char *pRecipe)
{
	bool bStatus = true;
	WeldRecipePC recipe;
	PcWeldRecipe *PC = new PcWeldRecipe(CP->getPcWeldRecipeInstance());
		
	if(NULL != PC)
	{
		PC->UpdateResonantFreq(CP->recipe["StartFrequency"]);	/* Later , Need to remove this code */
		PC->UpdateMemoryOffset(CP->recipe["MemoryOffset"]);
		PC->UpdateDigitalTune(CP->recipe["DigitalTune"]);
#ifdef SDO_DEBUG
		LOGDBG("EcSDOTask digitaltune=%d", CP->recipe["DigitalTune"],0,0);
#endif
		PC->UpdateFrequencyOffset(CP->recipe["FrequencyOffset"]);
#ifdef SDO_DEBUG
		LOGDBG("EcSDOTask FrequencyOffset=%d", CP->recipe["FrequencyOffset"],0,0);
#endif
		PC->GetWeldRecipe(recipe);
#ifdef SDO_DEBUG
		LOGDBG("SDO_T : Resonant Freq : %d, CP->SF : %d",recipe.StartFrequency,CP->recipe["StartFrequency"],0);
		LOGDBG("---------------------memory ofset : %d",recipe.MemoryOffset,0,0);
#endif
		memcpy(pRecipe,&recipe,sizeof(recipe));
		weldRecipePC = recipe;		/* only for debug - Print and check the values */
		delete PC;
		PC = NULL;
	}
	else
	{
		bStatus = false;
	}
	
	return bStatus;
}

/**************************************************************************//**
* \brief   - Prepare Updated Weld recipe buffer with ExceptedpartContact.
*
* \param   - unsigned char *pRecipe.
*
* \return  - bool - bStatus.
*
******************************************************************************/
bool EcSDOTask::GetAcWeldRecipe(unsigned char *pRecipe)
{
	bool bStatus = true;
	WeldRecipeAC recipe;
	WeldRecipeAcCompatibleWithAcBoard recipeTemp;
	AcWeldRecipe *AC = new AcWeldRecipe(CP->getAcWeldRecipeInstance());
	
	if(NULL != AC)
	{
		AC->GetWeldRecipe(recipe);
		/*copying weldRecipeAC structure param value to WeldRecipeAcCompatibleWithAcBoard structure local varaible
		 * To get Actuator Setup Advanced feature param value in correct format (structure format similar to present at AC)
		 * before it pass to AC */
		recipeTemp.WeldForce = recipe.WeldForce;
		recipeTemp.ForceRampTime = recipe.ForceRampTime;
		recipeTemp.HoldMode = recipe.HoldMode;
		recipeTemp.TotalCollapseTarget = recipe.TotalCollapseTarget;
		recipeTemp.HoldForce = recipe.HoldForce;
		recipeTemp.HoldForceRampTime = recipe.HoldForceRampTime;
		recipeTemp.ExpectedPartContactPosition = recipe.ExpectedPartContactPosition;
		recipeTemp.ReadyPosition = recipe.ReadyPosition;

		/* Actuator Setup Advanced Feature Parameter gets divided to convert it in to Uint16 byte. 
 	 	 * while part contact, Uint16 byte value is passing to AC. this is for imperial conversion support*/
		recipeTemp.DownAcceleration = recipe.DownAcceleration / 10000;
		recipeTemp.DownMaxVelocity = recipe.DownMaxVelocity / 10000;
		recipeTemp.DownDeceleration = recipe.DownDeceleration / 10000;
		recipeTemp.ReturnAcceleration = recipe.ReturnAcceleration / 10000;
		recipeTemp.ReturnMaxVelocity = recipe.ReturnMaxVelocity / 10000;
		recipeTemp.ReturnDeceleration = recipe.ReturnDeceleration /10000;

		recipeTemp.ExpectedPartContactOffset = recipe.ExpectedPartContactOffset;
		recipeTemp.PartContactWindowMinus = recipe.PartContactWindowMinus;
		recipeTemp.PartContactWindowPlus = recipe.PartContactWindowPlus;
		recipeTemp.NumForceSteps = recipe.NumForceSteps;
		recipeTemp.ReadyPositionToggle = recipe.ReadyPositionToggle;
		recipeTemp.WeldForceControl = recipe.WeldForceControl;

		copy(recipe.ForceStepRampTime, recipe.ForceStepRampTime + (sizeof(recipe.ForceStepRampTime)/sizeof(recipe.ForceStepRampTime[0])), recipeTemp.ForceStepRampTime);
		copy(recipe.ForceStepForce, recipe.ForceStepForce + (sizeof(recipe.ForceStepForce)/sizeof(recipe.ForceStepForce[0])), recipeTemp.ForceStepForce);

		memcpy(pRecipe,&recipeTemp,sizeof(recipeTemp));
		weldRecipeAC = recipe;		/* only for debug - Print and check the values */
		delete AC;
		AC = NULL;
	}
	else
	{
		bStatus = false;
	}
	
	return bStatus;
}

/**************************************************************************//**
* \brief   - Receive message from Q and process.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EcSDOTask::ProcessMessage()
{
	Message message;
	memset(pData,0x00,sizeof(pData));

	UINT32 ret = 0;
	
	if(msgQReceive(SDO_MSG_Q_ID, RecvMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR)
	{
		DeCoder(RecvMsgBuffer,message);

		switch(message.msgID)		
		{
			case TO_SDO_TASK_WELD_RECIPE:
				/* Sending WELD RECIPE to PC, AC using - SDO DOWNLOAD REQ */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : WELD_RECIPE_REQ_TO_PC",0,0,0);
				#endif
				memcpy(&weldRecipePC,&message.Buffer[0], sizeof(WeldRecipePC));
				memcpy(pData,&message.Buffer[0], sizeof(WeldRecipePC));
				CoeSdoDownloadReqSysCall(OD_INDEX_WELD_RECIPE_PC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_PC, SDO_ACCESS_TYPE_COMPLETE, sizeof(WeldRecipePC),pData);
		
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : WELD_RECIPE_REQ_TO_AC",0,0,0);
				#endif
				memcpy(&weldRecipeAcCompatibleWithAcBoard,&message.Buffer[0] + sizeof(WeldRecipePC), sizeof(WeldRecipeAcCompatibleWithAcBoard));
				memset(pData,0x00,sizeof(pData));
				memcpy(pData,&message.Buffer[0] + sizeof(WeldRecipePC), sizeof(WeldRecipeAcCompatibleWithAcBoard));
				CoeSdoDownloadReqSysCall(OD_INDEX_WELD_RECIPE_AC,OD_SUB_INDEX_FIRST_INDEX,SLAVE_IDX_AC, SDO_ACCESS_TYPE_COMPLETE, sizeof(WeldRecipeAcCompatibleWithAcBoard),pData);
#ifdef SDO_DEBUG
				PrintRecipe(weldRecipePC);
				PrintRecipe(weldRecipeAC);
#endif
				break;

			case TO_SDO_TASK_WELD_RECIPE_PC:
				/* Sending WELD RECIPE to PC using - SDO DOWNLOAD REQ */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : WELD_RECIPE_REQ_TO_PC",0,0,0);
				#endif
				GetPcWeldRecipe(pData);
				CoeSdoDownloadReqSysCall(OD_INDEX_WELD_RECIPE_PC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_PC, SDO_ACCESS_TYPE_COMPLETE, sizeof(WeldRecipePC),pData);
				#ifdef SDO_DEBUG
					PrintRecipe(weldRecipePC);
				#endif	
				break;

			case TO_SDO_TASK_WELD_RECIPE_AC:
				/* Sending WELD RECIPE to AC using - SDO DOWNLOAD REQ */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : WELD_RECIPE_REQ_TO_AC",0,0,0);
				#endif
				GetAcWeldRecipe(pData);
                CoeSdoDownloadReqSysCall(OD_INDEX_WELD_RECIPE_AC,OD_SUB_INDEX_FIRST_INDEX,SLAVE_IDX_AC, SDO_ACCESS_TYPE_COMPLETE, sizeof(WeldRecipeAcCompatibleWithAcBoard),pData);
				#ifdef SDO_DEBUG
					PrintRecipe(weldRecipeAC);
				#endif
				break;

			case TO_SDO_TASK_VERSION_REQUEST:
				/* upload request on startup in hopes that there is a response waiting */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : VERSION_REQ_TO_AC",0,0,0);
				#endif
					CoeSdoUploadReqSysCall(OD_INDEX_FW_VERSION_AC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_AC, sizeof(FirmwareVersion),SDO_ACCESS_TYPE_COMPLETE); 
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : VERSION_REQ_TO_PC",0,0,0);
				#endif
					CoeSdoUploadReqSysCall(OD_INDEX_FW_VERSION_PC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_PC, sizeof(FirmwareVersion),SDO_ACCESS_TYPE_COMPLETE);
				break;
				
			case TO_SDO_CALIBRATION_DATA_REQUEST:
				/* upload request on startup in hopes that there is a response waiting */
					CoeSdoUploadReqSysCall(OD_INDEX_CALIBRATION_READ_AC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_AC, sizeof(CalibrationReadings),SDO_ACCESS_TYPE_COMPLETE); 
				break;
				
			case TO_SDO_TASK_SEEK_RECIPE:
				/* Sending SEEK RECIPE to PC using - SDO DOWNLOAD REQ */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : SEEK_RECIPE_REQ_TO_PC",0,0,0);
				#endif
				memcpy(&m_seekRecipe, &message.Buffer, (sizeof(m_seekRecipe) - sizeof(m_seekRecipe.SeekTime)));
				PrintRecipe(m_seekRecipe);
				memcpy(pData, message.Buffer, (sizeof(m_seekRecipe) - sizeof(m_seekRecipe.SeekTime)));
				CoeSdoDownloadReqSysCall(OD_INDEX_SEEK_RECIPE_PC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_PC, SDO_ACCESS_TYPE_COMPLETE, sizeof(SeekRecipe),pData);
				break;

			case TO_SDO_TASK_POWER_UP_RECIPE:
				/* Sending POWERUP RECIPE to PC using - SDO DOWNLOAD REQ */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : POWER_UP_RECIPE_REQ_TO_PC",0,0,0);
				#endif
				memcpy(pData,&message.Buffer[0], sizeof(PowerUpData));
				CoeSdoDownloadReqSysCall(OD_INDEX_POWERUP_RECIPE, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_PC, SDO_ACCESS_TYPE_COMPLETE, sizeof(PowerUpData),pData);
				break;

			case TO_SDO_TASK_HORN_SCAN_RECIPE:
				/* Sending HORN SCAN RECIPE to PC using - SDO DOWNLOAD REQ */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : HORN_SCAN_RECIPE_REQ_TO_PC",0,0,0);
				#endif
				memcpy(&scanRecipe,&message.Buffer[0], sizeof(ScanRecipe));
				PrintRecipe(scanRecipe);
				memcpy(pData,&message.Buffer[0], sizeof(ScanRecipe));
				CoeSdoDownloadReqSysCall(OD_INDEX_HORN_SCAN_RECIPE, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_PC, SDO_ACCESS_TYPE_COMPLETE, sizeof(ScanRecipe),pData);
				break;
				
			case TO_SDO_TASK_TEST_RECIPE_REQ:									
				/* Debug purpose only */
				memcpy(&weldRecipePC, message.Buffer, sizeof(WeldRecipePC));
				PrintRecipe(weldRecipePC);
				/* Sending TEST RECIPE to PC using - SDO DOWNLOAD REQ */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : TEST_RECIPE_REQ_TO_PC",0,0,0);
				#endif	
				memcpy(pData,&weldRecipePC, sizeof(WeldRecipePC));
				CoeSdoDownloadReqSysCall(OD_INDEX_TEST_RECIPE, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_PC, SDO_ACCESS_TYPE_COMPLETE, sizeof(WeldRecipePC),pData);
				break;
				
			case TO_SDO_TASK_CALIBRATION_READ_REQ:
				/* Sending Calibration Readings to AC using - SDO DOWNLOAD REQ */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : CALIBRATION_READINGS_REQ_TO_AC",0,0,0);
				#endif
				memcpy(pData,&message.Buffer[0], sizeof(CalibrationReadings));
				CoeSdoDownloadReqSysCall(OD_INDEX_CALIBRATION_READ_AC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_AC, SDO_ACCESS_TYPE_COMPLETE, sizeof(CalibrationReadings),pData);
				break;
				
			case TO_SDO_TASK_WELD_RECIPE_RESP_PC:
				/* Received response of WELD RECIPE from PC - SDO DOWNLOAD RESP */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : WELD_RECIPE_RESP_FROM_PC",0,0,0);
				#endif
				message.msgID = TO_UI_TASK_WELD_RECIPE_RESP_PC;
				SendToMsgQ(message,UI_MSG_Q_ID);
				break;

			case TO_SDO_TASK_WELD_RECIPE_RESP_AC:
				/* Received response of WELD RECIPE from AC - SDO DOWNLOAD RESP */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : WELD_RECIPE_RESP_FROM_AC",0,0,0);
				#endif
				message.msgID = TO_UI_TASK_WELD_RECIPE_RESP_AC;
				SendToMsgQ(message,UI_MSG_Q_ID);
				break;

			case TO_SDO_TASK_DOWNLOAD_SEEK_RESP:
				/* Received response of SEEK RECIPE from PC - SDO DOWNLOAD RESP */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : SEEK_RECIPE_RESP_FROM_PC",0,0,0);
				#endif
				message.msgID = TO_UI_TASK_SEEK_RECIPE_CONFIGURED;
				SendToMsgQ(message,UI_MSG_Q_ID);
				break;

			case TO_SDO_TASK_DOWNLOAD_POWERUP_RESP:
				/* Received response of POWER-UP RECIPE from PC - SDO DOWNLOAD RESP */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : POWER_UP_RECIPE_RESP_FROM_PC",0,0,0);
				#endif
				message.msgID = TO_UI_TASK_DOWNLOAD_POWERUP_RESP;
				SendToMsgQ(message,UI_MSG_Q_ID);
				break;
				
			case TO_SDO_TASK_HORN_SCAN_RECIPE_RESP:
				/* Received response of HORN SCAN RECIPE from PC - SDO DOWNLOAD RESP */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : HORN_SCAN_RECIPE_RESP_FROM_PC",0,0,0);
				#endif
				message.msgID = TO_UI_TASK_HORN_SCAN_RECIPE_RESP;
				SendToMsgQ(message,UI_MSG_Q_ID);
				break;
				
			case TO_SDO_TASK_UL_ALARM_DATA_REQ_PC:
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : ALARM_DATA_REQ_TO_PC",0,0,0);
				#endif
				ret = CoeSdoUploadReqSysCall(OD_INDEX_ALARM_DATA_PC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_PC,sizeof(PCAlarmData),SDO_ACCESS_TYPE_COMPLETE);
				if(ret != SDO_ERROR)
				{
					LOGERR("SDO Upload Error for alarm data pc : %d",ret,0,0);
				}
				break;

			case TO_SDO_TASK_UL_ALARM_DATA_REQ_AC:
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : ALARM_DATA_REQ_TO_AC",0,0,0);
				#endif
				ret = CoeSdoUploadReqSysCall(OD_INDEX_ALARM_DATA_AC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_AC,sizeof(ACAlarmData),SDO_ACCESS_TYPE_COMPLETE);
				if(ret != SDO_ERROR)
				{
					LOGERR("SDO Upload Error for alarm data ac : %d",ret,0,0);
				}
				break;

			case TO_SDO_TASK_UL_ALARM_DATA_RESP_PC:
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : ALARM_DATA_RESP_FROM_PC",0,0,0);
				#endif
				GetSdoFromOD(ALARM_DATA_PC, message.Buffer);
				message.msgID = TO_DB_TASK_UL_ALARM_DATA_PC;
				SendToMsgQ(message,DB_MSG_Q_ID);
				break;

			case TO_SDO_TASK_UL_ALARM_DATA_RESP_AC:
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : ALARM_DATA_RESP_FROM_AC",0,0,0);
				#endif
				GetSdoFromOD(ALARM_DATA_AC, message.Buffer);
				message.msgID = TO_DB_TASK_UL_ALARM_DATA_AC;
				SendToMsgQ(message,DB_MSG_Q_ID);
				break;
				
			case TO_SDO_TASK_VERSION_REQUEST_RESP_AC:
				/* this is where response for AC version request is captured */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : VERSION_REQUEST_RESP_FROM_AC",0,0,0);
				#endif

				message.msgID = TO_UI_TASK_VERSION_REQUEST_RESP_AC;

				pRecvBuffer = reinterpret_cast<char*>(&message);
				msgQSend (UI_MSG_Q_ID, pRecvBuffer, sizeof (message.msgID) + sizeof (FirmwareVersion), NO_WAIT,MSG_PRI_NORMAL);
				break;
				
			case TO_SDO_CALIBRATION_DATA_REQUEST_RESP:
				/* this is where response for AC version request is captured */
				message.msgID = TO_UI_CALIBRATION_DATA_REQUEST_RESP;

				pRecvBuffer = reinterpret_cast<char*>(&message);
				msgQSend (UI_MSG_Q_ID, pRecvBuffer, sizeof (message.msgID) + sizeof (CalibrationReadings), NO_WAIT,MSG_PRI_NORMAL);
				break;


			case TO_SDO_TASK_VERSION_REQUEST_RESP_PC:
				/* this is where response for PC version request is captured */						
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : VERSION_REQUEST_RESP_FROM_PC",0,0,0);
				#endif
				message.msgID = TO_UI_TASK_VERSION_REQUEST_RESP_PC;

				pRecvBuffer = reinterpret_cast<char*>(&message);
				msgQSend (UI_MSG_Q_ID, pRecvBuffer, sizeof (message.msgID) + sizeof (FirmwareVersion), NO_WAIT,MSG_PRI_NORMAL);
				break;
				
			case TO_SDO_TASK_TMR_INTVL_DATA_PC:		
										/* Sending TIMER INTERVAL to PC/AC using - SDO DOWNLOAD REQ */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : TMR_INTVL_DATA_PC",0,0,0);
				#endif
				memcpy(pData,&message.Buffer[0], sizeof(TimerInterval));
				CoeSdoDownloadReqSysCall(OD_INDEX_TIMER_INTERVAL_PC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_PC, SDO_ACCESS_TYPE_COMPLETE, sizeof(TimerInterval),pData);
				break;
													
			case TO_SDO_TASK_TMR_INTVL_RESP_PC:		
				/* this is where response for PC/AC timer interval is captured */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : TMR_INTVL_RESP_PC",0,0,0);
				#endif
				message.msgID = TO_UI_TASK_TMR_INTVL_RESP_PC;
				SendToMsgQ(message,UI_MSG_Q_ID);
				break;
				
			 case TO_SDO_TASK_TEST_RECIPE_RES:
				 /* Received response of TEST RECIPE from PC - SDO DOWNLOAD RESP */
				 #ifdef SDO_DEBUG
				 	 LOGDBG("SDO_T : TEST_RECIPE_RESP_FROM_PC",0,0,0);
				 #endif
				 message.msgID = TO_UI_TASK_TEST_RECIPE_RES;
				 SendToMsgQ(message, UI_MSG_Q_ID);
				 break;
				 
			 case TO_SDO_TASK_CALIBRATION_READ_RES:
				 /* Received response of Calibration reading from AC - SDO DOWNLOAD RESP */
				 #ifdef SDO_DEBUG
				 	 LOGDBG("SDO_T : CALIBRATION_READINGS_RESP_FROM_AC",0,0,0);
				 #endif
				 message.msgID = TO_UI_TASK_CALIBRATION_READ_RES;
				 SendToMsgQ(message,UI_MSG_Q_ID);
				 break;
				 
			 case TO_SDO_TASK_FW_UPGRADE_INFO_REQ_AC:
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : FW_UPGRADE_INFO_REQ_TO_AC",0,0,0);
				#endif
				ret = CoeSdoUploadReqSysCall(OD_INDEX_FW_UPGRADE_AC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_AC,sizeof(FirmWareUpgradeInfo),SDO_ACCESS_TYPE_COMPLETE);
				if(ret != SDO_ERROR)
				{
					LOGERR("SDO Upload Error for Firmware Upgrade Info ac : %d",ret,0,0);
				}
				 break;
				 
			 case TO_SDO_TASK_FW_UPGRADE_INFO_RESP_AC:
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : FW_UPGRADE_INFO_RESP_AC",0,0,0);
				#endif
				memset(&FirmwareUpgrade::fwUpgrdInfoAC, 0x00, sizeof(FirmWareUpgradeInfo));
				memcpy(&FirmwareUpgrade::fwUpgrdInfoAC, message.Buffer, sizeof(FirmWareUpgradeInfo));
				 break;
				 
			 case TO_SDO_TASK_FW_UPGRADE_INFO_REQ_PC:
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : FW_UPGRADE_INFO_REQ_TO_PC",0,0,0);
				#endif
				ret = CoeSdoUploadReqSysCall(OD_INDEX_FW_UPGRADE_PC, OD_SUB_INDEX_FIRST_INDEX, SLAVE_IDX_PC,sizeof(FirmWareUpgradeInfo),SDO_ACCESS_TYPE_COMPLETE);
				if(ret != SDO_ERROR)
				{
					LOGERR("SDO Upload Error for Firmware Upgrade Info pc : %d",ret,0,0);
				}
				break;	
				
			 case TO_SDO_TASK_FW_UPGRADE_INFO_RESP_PC:
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : FW_UPGRADE_INFO_RESP_PC",0,0,0);
				#endif
				memset(&FirmwareUpgrade::fwUpgrdInfoPC, 0x00, sizeof(FirmWareUpgradeInfo));
				memcpy(&FirmwareUpgrade::fwUpgrdInfoPC, message.Buffer, sizeof(FirmWareUpgradeInfo));
				 break;
				 
			 case TO_SDO_TASK_FW_UPGRADE_STATUS_AC:
				/* Sending Firmware Upgrade Status to AC using - SDO DOWNLOAD REQ */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : FW_UPGRADE_STATUS_TO_AC",0,0,0);
				#endif
					
				memset(pData ,0x00, sizeof(pData));
				memcpy(pData,&FirmwareUpgrade::fwUpgrdInfoAC,sizeof(FirmwareUpgrade::fwUpgrdInfoAC));
				
				CoeSdoDownloadReqSysCall(OD_INDEX_FW_UPGRADE_AC,OD_SUB_INDEX_FIRST_INDEX,SLAVE_IDX_AC, SDO_ACCESS_TYPE_COMPLETE, sizeof(FirmWareUpgradeInfo),pData);
				break;
				
			case TO_SDO_TASK_FW_UPGRADE_STATUS_PC:
				/* Sending Firmware Upgrade Status to PC using - SDO DOWNLOAD REQ */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : FW_UPGRADE_STATUS_TO_PC",0,0,0);
				#endif
					
				memset(pData ,0x00, sizeof(pData));
				memcpy(pData,&FirmwareUpgrade::fwUpgrdInfoPC,sizeof(FirmwareUpgrade::fwUpgrdInfoPC));
				
				CoeSdoDownloadReqSysCall(OD_INDEX_FW_UPGRADE_PC,OD_SUB_INDEX_FIRST_INDEX,SLAVE_IDX_PC, SDO_ACCESS_TYPE_COMPLETE, sizeof(FirmWareUpgradeInfo),pData);
				break;
				
			case TO_SDO_TASK_FW_UPGRADE_STATUS_RESP_AC:
				/* Received response of FW_UPGRADE_STATUS_RESP_FROM_AC - SDO DOWNLOAD RESP */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : FW_UPGRADE_STATUS_RESP_FROM_AC",0,0,0);
				#endif
				break;
				
			case TO_SDO_TASK_FW_UPGRADE_STATUS_RESP_PC:
				/* Received response of FW_UPGRADE_STATUS_RESP_FROM_PC - SDO DOWNLOAD RESP */
				#ifdef SDO_DEBUG
					LOGDBG("SDO_T : FW_UPGRADE_STATUS_RESP_FROM_PC",0,0,0);
				#endif
				break;		
				
			case SERVER_ETHRNT_FW_UPLOAD_NOTIFY:
				LOGDBG("SDO_T : FW_UPGRADE_ETHER_NOTIFY_ID : %d",message.msgID ,0,0);
				LOGDBG("SDO_T : FW_UPGRADE_ETHER_NOTIFY_DATA : %s",(_Vx_usr_arg_t )message.Buffer ,0,0);
				message.msgID = TO_UI_TASK_FW_UPGRADE_ETHER_IND;
				SendToMsgQ(message, UI_MSG_Q_ID);
				break;
							
			case SHUTDOWN_APPLICATION:
				break;
				
			default:
				LOGWARN("SDO_T : ----------Received Invalid MsgID : ---------",message.msgID,0,0);
				break;
		}
	}/*	end of message queue receive.	*/
}

/**************************************************************************//**
* \brief   - Custom system call for triggering SDO DOWNLOAD REQ. 
*
* \param   - UINT8, UINT32* pData.
*
* \return  - UINT32 Success or failed.
*
******************************************************************************/
UINT32 EcSDOTask::CoeSdoDownloadReqSysCall(UINT16 iIndex, UINT16 iSubIndex, UINT16 iSlaveID, UINT8 iAccessType, UINT16 iLength,unsigned char *pData)
{
	return syscall((_Vx_usr_arg_t)iIndex, (_Vx_usr_arg_t)iSubIndex, (_Vx_usr_arg_t)iSlaveID, (_Vx_usr_arg_t)iAccessType , (_Vx_usr_arg_t)iLength , (_Vx_usr_arg_t)pData, 0, 0, SYSCALL_NUMBER(3,0));
}

/**************************************************************************//**
* \brief   - Custom system call for triggering SDO UPLOAD REQ. 
*
* \param   - UINT8, UINT32* pData.
*
* \return  - UINT32 Success or failed.
*
******************************************************************************/
UINT32 EcSDOTask::CoeSdoUploadReqSysCall(UINT16 iIndex, UINT16 iSubIndex, UINT16 iSlaveID, UINT16 iLength, UINT16 iAccessType)
{
	return syscall((_Vx_usr_arg_t)iIndex, (_Vx_usr_arg_t)iSubIndex, (_Vx_usr_arg_t)iSlaveID, (_Vx_usr_arg_t)iLength , (_Vx_usr_arg_t)iAccessType , 0, 0, 0, SYSCALL_NUMBER(3,1));
}

/**************************************************************************//**
* \brief   - Gets state of EtherCat master stack state.
*
* \param   - None.
*
* \return  - EC_MASTER_STATE (init, pre-op, safe-op or operational).
*
******************************************************************************/
EC_MASTER_STATE EcSDOTask::EcatGetMasterStateSysCall()
{
	INT32 iState = syscall(0, 0, 0, 0 , 0, 0, 0, 0, SYSCALL_NUMBER(3,2));
	return (EC_MASTER_STATE)iState;
}

/**************************************************************************//**
* \brief   - Get the task life time status.
*
* \param   - None.
*
* \return  - bool.
*
******************************************************************************/
bool EcSDOTask::bIsTaskRunStatus()
{
	return CP->bIsTaskRun;
}

/**************************************************************************//**
* \brief   - De-Initialize allocated stack memory of EcSDOTask.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
EcSDOTask::~EcSDOTask()
{
	CP = NULL;
}

/**************************************************************************//**
* \brief   - Handles the SDO download & upload request and response.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
void EcSDO_Task(void)
{
	EcSDOTask *SDO 	= new(nothrow) EcSDOTask();

	if(NULL != SDO)
	{
		/* EcSDO Task loop and the bIsTaskRun flag enabled when task created */
		while(SDO->bIsTaskRunStatus())
		{
			EC_MASTER_STATE EcMasterState = SDO->EcatGetMasterStateSysCall();

			/* check the state of the Ec Master before reading the MsgQ to send the SDO down load/Up load req	*/
			if(ecat_OP == EcMasterState)
			{
				SDO->ProcessMessage();
			}
			else
			{
#ifdef DEBUG
					LOGCRIT("SDO_T : ---------Master is not in right state-------- : ",EcMasterState,0,0);
#endif
				taskDelay(ONE_SEC_DELAY);
			}
		}
		
		delete SDO;
	}
	else
	{
		cout << "EcSDO_T : ----------------Memory allocation failed----------------" << endl;
	}
	
	SDO = NULL;
	LOGDBG("----------------EcSDOTask Suspended----------------",0,0,0);
	taskSuspend(taskIdSelf());
	
}

/**************************************************************************//**
* \brief   - Prints the WeldRecipePC and WeldRecipeAC
*
* \param   - WeldRecipePC& weldRecipePC,WeldRecipeAC& weldRecipeAC
*
* \return  - None
*
******************************************************************************/
void EcSDOTask::PrintRecipe(WeldRecipePC& weldRecipePC)
{
	printf("\n---------PC WELD RECIPE BEFORE SENT-----------\n");
	printf("AmplitudeLoopC1		: %d\n",weldRecipePC.AmplitudeLoopC1);
	printf("AmplitudeLoopC2 		: %d\n",weldRecipePC.AmplitudeLoopC2);
	printf("PhaseLoop			: %d\n",weldRecipePC.PhaseLoop);
	printf("PhaseLoopCF			: %d\n",weldRecipePC.PhaseLoopCF);
	printf("FrequencyLow		: %d\n",weldRecipePC.FrequencyLow);
	printf("FrequencyHigh		: %d\n",weldRecipePC.FrequencyHigh);
	printf("PhaseLimitTime 		: %d\n",weldRecipePC.PhaseLimitTime);
	printf("PhaseLimit		: %d\n",weldRecipePC.PhaseLimit);
	printf("ControlMode		: %d\n",weldRecipePC.ControlMode);
	printf("BlindtimeSeek		: %d\n",weldRecipePC.BlindtimeSeek);
	printf("BlindtimeWeld		: %d\n",weldRecipePC.Blindtimeweld);
	printf("DDSSwitchtime		: %d\n",weldRecipePC.DDSSwitchtime);
	printf("WeldRampTime		: %d\n",weldRecipePC.WeldRampTime);
	printf("Parameter7		: %d\n",weldRecipePC.Parameter7);
	printf("Parameter8		: %d\n",weldRecipePC.Parameter8);
	printf("Parameter9		: %d\n",weldRecipePC.Parameter9);
	printf("Parameter10		: %d\n",weldRecipePC.Parameter10);
	printf("Parameter11		: %d\n",weldRecipePC.Parameter11);
	printf("Parameter12		: %d\n",weldRecipePC.Parameter12);
	printf("Parameter13		: %d\n",weldRecipePC.Parameter13);
	printf("Parameter14		: %d\n",weldRecipePC.Parameter14);
	printf("Parameter15		: %d\n",weldRecipePC.Parameter15);
	printf("F_LimitTime		: %d\n",weldRecipePC.FLimitTime);
	printf("AmpProportionalGain	: %d\n",weldRecipePC.AmpProportionalGain);
	printf("AmpIntegralGain		: %d\n",weldRecipePC.AmpIntegralGain);
	printf("PhaseProportionalGain	: %d\n",weldRecipePC.PhaseProportionalGain);
	printf("FrequencyWindowSize	: %d\n",weldRecipePC.FrequencyWindowSize);
	printf("PFofPhasecontrolloop	: %d\n",weldRecipePC.PFofPhasecontrolloop);
	printf("PIFrequencyLow		: %d\n",weldRecipePC.PIFrequencyLow);
	printf("PIFrequencyHigh		: %d\n",weldRecipePC.PIFrequencyHigh);
	printf("PIPhaseLimitTime		: %d\n",weldRecipePC.PIPhaseLimitTime);
	printf("PIPhaseLimit		: %d\n",weldRecipePC.PIPhaseLimit);
	printf("StartFrequency		: %d\n",weldRecipePC.StartFrequency);
	printf("Memory Offset       : %d\n",weldRecipePC.MemoryOffset);
	printf("Digital Tune       : %d\n",weldRecipePC.DigitalTune);
	printf("Frequency Offset       : %d\n",weldRecipePC.FrequencyOffset);
	printf("\n");

}

/**************************************************************************//**
* \brief   - Prints the WeldRecipePC parameters
*
* \param   - WeldRecipePC& weldRecipePC
*
* \return  - None
*
******************************************************************************/
void EcSDOTask::PrintRecipe(WeldRecipeAC& weldRecipeAC)
{
	printf("\n---------AC WELD RECIPE BEFORE SENT-----------\n");
	printf("WeldForce		: %d\n",weldRecipeAC.WeldForce);
	printf("ForceRampTime		: %d\n",weldRecipeAC.ForceRampTime);
	printf("HoldMode			: %d\n",weldRecipeAC.HoldMode);
	printf("TotalCollapseTarget	: %d\n",weldRecipeAC.TotalCollapseTarget);
	printf("HoldForce		: %d\n",weldRecipeAC.HoldForce);
	printf("HoldForceRampTime	: %d\n",weldRecipeAC.HoldForceRampTime);
	printf("ExpectedPartContactPosition : %d\n",weldRecipeAC.ExpectedPartContactPosition);
	printf("ReadyPosition		: %d\n",weldRecipeAC.ReadyPosition);
	printf("DownAcceleration		: %d\n",weldRecipeAC.DownAcceleration);
	printf("DownMaxVelocity		: %d\n",weldRecipeAC.DownMaxVelocity);
	printf("DownDeceleration		: %d\n",weldRecipeAC.DownDeceleration);
	printf("ReturnAcceleration	: %d\n",weldRecipeAC.ReturnAcceleration);
	printf("ReturnMaxVelocity	: %d\n",weldRecipeAC.ReturnMaxVelocity);
	printf("ReturnDeceleration	: %d\n",weldRecipeAC.ReturnDeceleration);
	printf("ExpectedPartContactOffset: %d\n",weldRecipeAC.ExpectedPartContactOffset);
	printf("PartContactWindowMinus: %d\n",weldRecipeAC.PartContactWindowMinus);
	printf("PartContactWindowPlus: %d\n",weldRecipeAC.PartContactWindowPlus);
	printf("NumForceSteps 		: %d\n",weldRecipeAC.NumForceSteps);
	printf("ReadyPositionToggle	: %d\n",weldRecipeAC.ReadyPositionToggle);
	printf("WeldForceControl	: %d\n",weldRecipeAC.WeldForceControl);
	
	for(int idx = 0; idx < weldRecipeAC.NumForceSteps; idx++)
	{
		printf("ForceStepForce[%d]	: %d\n",idx,weldRecipeAC.ForceStepForce[idx]);
		printf("ForceStepRampTime[%d] 	: %d\n",idx,weldRecipeAC.ForceStepRampTime[idx]);
	}
	printf("\n");
}

/**************************************************************************//**
* \brief   - Prints the SeekRecipe
*
* \param   - SeekRecipe& gSDODownloadSeek
*
* \return  - None
*
******************************************************************************/
void EcSDOTask::PrintRecipe(seekRecipe& gSDODownloadSeek)
{
	printf("\n---------SEEK RECIPE-----------\n");
	printf("AmplitudeLoopC1		: %d\n",gSDODownloadSeek.SeekAmplitudeLoopC1);
	printf("AmplitudeLoopC2 	: %d\n",gSDODownloadSeek.SeekAmplitudeLoopC2);
	printf("SeekFreqLimitHigh	: %d\n",gSDODownloadSeek.SeekFreqLimitHigh);
	printf("SeekFreqLimitLow	: %d\n",gSDODownloadSeek.SeekFreqLimitLow);
	printf("SeekFreqOffset		: %d\n",gSDODownloadSeek.SeekFreqOffset);
	printf("SeekPhaseloopCF		: %d\n",gSDODownloadSeek.SeekPhaseloopCF);
	printf("SeekPhaseloopI 		: %d\n",gSDODownloadSeek.SeekPhaseloopI);
	printf("SeekRampTime 		: %d\n",gSDODownloadSeek.SeekRampTime);
	printf("SeekMemClear 		: %d\n",gSDODownloadSeek.SeekMemClearBeforeSeek);
	printf("\n");
}

/**************************************************************************//**
* \brief   - Prints the Scan recipe
*
* \param   - ScanRecipe& gSDODownloadScan
*
* \return  - None
*
******************************************************************************/
void EcSDOTask::PrintRecipe(ScanRecipe& gSDODownloadScan)
{
	printf("\n---------SCAN RECIPE-----------\n");
	printf("FrequencyStart		: %d\n",gSDODownloadScan.FrequencyStart);
	printf("FrequencyStep 	: %d\n",gSDODownloadScan.FrequencyStep);
	printf("FrequencyStop			: %d\n",gSDODownloadScan.FrequencyStop);
	printf("MaxAmplitude			: %d\n",gSDODownloadScan.MaxAmplitude);
	printf("MaxCurrent		: %d\n",gSDODownloadScan.MaxCurrent);
	printf("TimeDelay 		: %d\n",gSDODownloadScan.TimeDelay);
	printf("\n");
}
