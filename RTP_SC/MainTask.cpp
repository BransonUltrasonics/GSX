/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Main task which spawns other tasks like Control task, SDO task, UI task, DB task etc...
 
**********************************************************************************************************/

#include  "MainTask.h"
#include  <sdLib.h>
#include  <ioLib.h>
#include "SysConfigEeprom.hpp"
#include "FirmwareUpgrade.h"
#include "Gpio.h"
#include "McSPI.h"
#include "AlarmManager.h"
#include  "BlueLed.h"
#include  <dirent.h>
#include  "AlarmManager.h"
#include "Eeprom.h"
#include "SQLiteDB.h"
#include "FieldBus/EthernetIP.h"

using namespace std;

/**************************************************************************//**
* \brief   - Initialize the task info structure
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
MainTask::MainTask()
{	
	#ifdef DEBUG
		LOGDBG("----------------Main task constructor called---------------",0,0,0);
	#endif
		
	char* taskName[TOTAL_NUM_OF_TASK] = {"/Ctrl_Task", "SDO_Task", "UI_Task", "DB_Task", "UI_DB_Task", "Alarm_Task", "BarcodeReader_Task", "ScDgtInput_Task","ScDgtOutput_Task",
			"WebService_Task", "Maintenance_Task","IOImage_Handler_Task","Mail_Box_Message_Handler_Task", "ConsoleApp_Task", "RtpMain_Task", "SocketRecv_Task", "FW_Upgrade_Task"};
	
	for(UINT8 index = 0; index < TOTAL_NUM_OF_TASK; index++)
	{
		cTaskName[index] = taskName[index];
	}
	
				
	CP = CommonProperty::getInstance();

	/* Enabling the flag allows business logic tasks to run continuously   */
	CP->bIsTaskRun = true;
	
	UINT32	g_Priority[NO_OF_TASK] 			= {CTRL_T_PRIORITY,SDO_T_PRIORITY,UI_T_PRIORITY,DB_T_PRIORITY,UI_DB_T_PRIORITY,ALARM_T_PRIORITY,BARCODE_READER_T_PRIORITY, DGTIN_T_PRIORITY, DGTOUT_T_PRIORITY,
			WEBSERVICE_T_PRIORITY,MAINTENANCE_T_PRIORITY, IO_IMAGE_HANDLER_T_PRIORITY, MAIL_BOX_MSG_HANDLER_T_PRIORITY};
	
	FUNC*	g_TaskFunc[NO_OF_TASK] 			= {Control_Task,EcSDO_Task,UserInterface_Task,DataBase_Task,UI_DataBase_Task,Alarm_Task,BarcodeReader_Task,ScDgtInputTask,ScDgtOutputTask,
			HttpWebService_Task,Maintenance_Task, IOImage_Handler_Task, Mail_Box_Message_Handler_Task};
	
	UINT32	g_StackSize[NO_OF_TASK]			= {CTRL_T_STACK_SIZE,SDO_T_STACK_SIZE,UI_T_STACK_SIZE,DB_T_STACK_SIZE,UI_DB_T_STACK_SIZE, ALARM_T_STACK_SIZE,
			BARCODE_READER_T_STACK_SIZE,DGTIN_T_STACK_SIZE, DGTOUT_T_STACK_SIZE,WEBSERVICE_T_STACK_SIZE,MAINTENANCE_T_STACK_SIZE, IO_IMAGE_HANDLER_T_STACK_SIZE, MAIL_BOX_MSG_HANDLER_T_STACK_SIZE};
	
	for(UINT32 t_index=0; t_index < NO_OF_TASK; t_index++)
	{			
		taskinfo[t_index].TaskName 		= cTaskName[t_index];
		taskinfo[t_index].Priority 		= g_Priority[t_index];
		taskinfo[t_index].TaskFunc	 	= g_TaskFunc[t_index];
		taskinfo[t_index].StackSize 	= g_StackSize[t_index];		
	}
}

/**************************************************************************//**
* \brief   - Creates the message queue for all the task spawned
*
* \param   - None
*
* \return  - bool - Success - return true, Failure - returns false.
*
******************************************************************************/
bool MainTask::CreateMsgQ()
{
	bool bIsSuccess = true;
	MSG_Q_ID mid;
	
	for(UINT32 t_index=0; t_index < NO_OF_TASK; t_index++)
	{
		if(t_index == 1)
		{
			mid =  msgQOpen("/SDO_Task", MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO, OM_CREATE, 0);
		}
		else if(t_index == 9)
		{
			mid = msgQOpen("/WebService_Task", MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO, OM_CREATE, 0);
		}
		else
		{
			mid = msgQCreate(MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO);
		}
		if(MSG_Q_ID_NULL != mid)
		{
			CP->SetMsgQId(taskinfo[t_index].TaskName,mid);
		}
		else
		{
			bIsSuccess = false;
		}
	}
	return bIsSuccess;
}


/**************************************************************************//**
* \brief   - Spawns the required tasks
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool MainTask::CreateTasks()
{
	bool bIsSuccess = true;
	TASK_ID TID = taskSpawn((char *)cTaskName[CONSOLE_T], CONSOLE_T_PRIORITY, VX_FP_TASK, CONSOLE_T_STACK_SIZE, (FUNCPTR)ConsoleApp_Task, 0,0,0,0,0,0,0,0,0,0);	
	CP->SetTaskId(cTaskName[CONSOLE_T], TID);
	
	TASK_ID SockRecv = taskSpawn((char *)cTaskName[SOCKSERV_T], RECV_T_PRIORITY, VX_FP_TASK, RECV_T_STACK_SIZE, (FUNCPTR)Socket_Recv,0,0,0,0,0,0,0,0,0,0);
	
	#if DEBUG
		LOGDBG("MAIN_T : -------------------Task creation started-------------",0,0,0);
	#endif
	
	for(UINT32 t_index=0; t_index < NO_OF_TASK; t_index++)
	{
		if(t_index == 0)
		{ 
			tid[t_index] = taskOpen((char *)taskinfo[t_index].TaskName.c_str(),taskinfo[t_index].Priority,VX_FP_TASK,OM_CREATE,NULL,taskinfo[t_index].StackSize,0,(FUNCPTR)taskinfo[t_index].TaskFunc,0,0,0,0,0,0,0,0,0,0);
		}
		else
		{
			//TO_BE_DONE - Check if system is GSXE2, if yes then do not spawn IOImage_Handler_Task and Mail_Box_Message_Handler_Task.
			tid[t_index] = taskSpawn((char *)taskinfo[t_index].TaskName.c_str(),taskinfo[t_index].Priority,VX_FP_TASK,taskinfo[t_index].StackSize,(FUNCPTR)taskinfo[t_index].TaskFunc,0,0,0,0,0,0,0,0,0,0);
		}
		
		if(TASK_ID_NULL != tid[t_index])
		{
			CP->SetTaskId(taskinfo[t_index].TaskName,tid[t_index]);
		}
		else
		{
			bIsSuccess = false;
		}
	}
	#if DEBUG
		LOGDBG("MAIN_T : -------------------Task creation ended-------------",0,0,0);
	#endif
	return bIsSuccess;
}

/**************************************************************************//**
* \brief   - Creates the shared data region for PDO DATA. 
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool MainTask::CreateSD()
{
	bool bIsSuccess = true ;
	SD_ID RxSD_PC = sdOpen(RX_DATA_PC,0,OM_CREATE,SD_SIZE,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &PC_RX);
	if(SD_ID_NULL == RxSD_PC)
	{
		cout << "MAIN : Error on SD creation - PC : " << RxSD_PC << endl;
		bIsSuccess = false;
	}

	SD_ID RxSD_AC = sdOpen(RX_DATA_AC,0,OM_CREATE,SD_SIZE,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &AC_RX);
	if(SD_ID_NULL == RxSD_AC)
	{
		cout << "MAIN : Error on SD creation - AC : " << RxSD_AC << endl;
		bIsSuccess = false;
	}

	SD_ID TxSD_PC = sdOpen(TX_DATA_PC,0,OM_CREATE,SD_SIZE,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &PC_TX);
	if(SD_ID_NULL == TxSD_PC)
	{
		cout << "MAIN : Error on SD creation - PC_TX : " << TxSD_PC << endl;
		bIsSuccess = false;
	}

	SD_ID TxSD_AC = sdOpen(TX_DATA_AC,0,OM_CREATE,SD_SIZE,0,SD_ATTR_RWX | SD_CACHE_OFF, (void **) &AC_TX);
	if(SD_ID_NULL == TxSD_AC)
	{
		cout << "MAIN : Error on SD creation - AC_TX : " << TxSD_AC << endl;
		bIsSuccess = false;
	}
	
	return bIsSuccess;
}

/**************************************************************************//**
* \brief   - Release memory and exits from spawned tasks
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void MainTask::TerminateTasks()
{
	/* Disabling the flag allows business logic tasks to exit from continuous continuously loop */
	CP->bIsTaskRun = false;
}

/**************************************************************************//**
* \brief   - Delete the created message queue.
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool MainTask::DestroyAllMsgQ()
{
	#ifdef DEBUG
		cout << "------------MsgQ deleting----------------" << endl;
	#endif
		
	for(INT32 t_index = 0; t_index < NO_OF_TASK; t_index++)
	{
		if(msgQDelete(CP->GetMsgQId(taskinfo[t_index].TaskName)) != OK)
		{
			LOGDBG("MessageQ deletion failed",0,0,0);
		}
	}		
	return true;
}

/**************************************************************************//**
* \brief   - De-initialize the allocated stack memory for CommonProperty. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void MainTask::ReleaseMemory()
{
	CommonProperty::DeleteInstance();
}

/**************************************************************************//**
* \brief   - Assign Console_Task id with DS. 
*
* \param   - string Task name, TASK_ID TID
*
* \return  - None
*
******************************************************************************/
void MainTask::SetTaskId(string T_Name, TASK_ID TID)
{
	CP->SetTaskId(T_Name,TID);
}

/**************************************************************************//**
* \brief   - Get the task life time status. 
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool MainTask::bIsTaskRunStatus()
{
	return CP->bIsTaskRun;
}

/**************************************************************************//**
* \brief   - Notify the EcMaster application to exit 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void MainTask::ExitEcatMasterSysCall()
{
	syscall(0, 0, 0, 0 , 0, 0, 0, 0, SYSCALL_NUMBER(3,3));
}

/**************************************************************************//**
* \brief   - Write default values into EEPROM when new board detected.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void MainTask::SetEepromValue()
{
	SystemConfiguration SysConfig;
	intf_SysConfig_t 	sysConfigData;

	SysConfig.ConfigureDefaultSettings();
	SysConfig.SysConfigGet((char *)&sysConfigData);

	switch(sysConfigData.PcConfig.PSFreq)
	{
		case KHz_20:
			CP->powerupRecipe.Frequency = FREQ_20KHz;		
			CP->powerupRecipe.Power	  	= sysConfigData.PcConfig.PSWatt;
			break;
	
		case KHz_30:
			CP->powerupRecipe.Frequency	= FREQ_30KHz;		
			CP->powerupRecipe.Power  	= sysConfigData.PcConfig.PSWatt;
			break;
	
		case KHz_40:
			CP->powerupRecipe.Frequency	= FREQ_40KHz;		
			CP->powerupRecipe.Power	  	= sysConfigData.PcConfig.PSWatt;
			break;
	
		default:
			LOGERR("Invalid system frequency, Changing to default values - 20KHz",0,0,0);
			CP->powerupRecipe.Frequency	= FREQ_20KHz;		
			CP->powerupRecipe.Power  	= POWER_20KHz;
			break;
	}

	CP->SetSystemFrequency();			/* Check the system frequency type - 20, 30 or 40 Khz */
	CP->SetSysConfContext(&sysConfigData);
	SysConfig.GetSystemTypeName(SystemConfiguration::systemTypeName);
}

/**************************************************************************//**
* \brief   - Clean up unwanted data available in secondary storage
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void MainTask::CleanUp()
{
	STATUS ret = 0;
	if(!g_bIsDBInDevice)
	{
		if(!chdir(EMMC_DISK_PATH))
		{
			ret = xdelete("~DelMe:*");
			if(ERROR == ret)
			{
				LOGERR("Main_T : Error on deleting residual files or no residual file exist: %d",ret,0,0);
			}
			else
			{
				LOGDBG("Main_T : DelMe file deleted",0,0,0);
			}
			ret = xdelete("etilqs_*");
			if(ERROR == ret)
			{
				LOGERR("Main_T : Error on deleting residual files or no residual file exist: %d",ret,0,0);
			}
			else
			{
				LOGDBG("Main_T : etilqs temp file deleted",0,0,0);
			}			
		}
		else
		{
			LOGERR("Main_T : not able to switch to emmc directory",0,0,0);
		}
	}
	//TODO cleanup for external device will added later
	else
	{
		LOGDBG("Main_T :External device is set as DB path ",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Finds the DB path and set flag to load DB file from specified 
*			 location.
*			 
* \param   - None
*
* \return  - None
*
******************************************************************************/
void MainTask::FindDBFilePath()
{
	
	if(CP->IsFileExist(DB_EMMC))		/* Looking DB file availability in EMMC storage */
	{
		g_bIsDBInDevice = false;
		chdir(EMMC_DISK_PATH);
	}
	else								/* Sets External device path to load DB file from SD storage */
	{
	//TODO external device switch will be handled with other sata changes
		LOGERR("DB file doesn't exist in EMMC",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Post Alarm configuration read message to DB Task
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void MainTask::PostMessageToTask(Message &Send)
{	
	MSG_Q_ID DBmsgQId = CP->GetMsgQId(cTaskName[DB_T]);	
	if(msgQSend(DBmsgQId, reinterpret_cast<char*>(&Send), sizeof(Send), NO_WAIT, MSG_PRI_NORMAL) != OK)
	{		
		LOGERR("Main_T : -----------Error on Post Message To Task msgQSend----------",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - This method will be use to raise Alarm 
*
* \param   - UINT8
*
* \return  - None
*
******************************************************************************/
void MainTask :: ProcessAlarm (UINT8 origin, UINT16 alarmID)
{	
	Message  message;
	AlarmMgr alarmMgr;
	UINT8 	 resetReq;
	MSG_Q_ID UI_MSG_Q_ID, DB_MSG_Q_ID;  
	UI_MSG_Q_ID		= CP->GetMsgQId(cTaskName[UI_T]);		
	DB_MSG_Q_ID		= CP->GetMsgQId(cTaskName[DB_T]);
	LOGDBG("Raising SATA Missing alarm!!!", 0, 0, 0);
	((pALARM_INFO) message.Buffer)->alarmType = alarmID & ALARMTYPE_MASK;
	((pALARM_INFO) message.Buffer)->source = (ALARM_FROM)origin;
	((pALARM_INFO) message.Buffer)->id = alarmID;
	((pALARM_INFO) message.Buffer)->priority = AlarmEvent::PRIORITY_HIGH;
	alarmMgr.GetAlarmActions(((pALARM_INFO) message.Buffer)->alarmType, alarmID, ((pALARM_INFO) message.Buffer)->flags);	
	resetReq = ((pALARM_INFO) message.Buffer)->flags.ResetRequired;
		
	message.msgID = TO_DB_TASK_ALARM_DATA_SC;
	SendToMsgQ(message, DB_MSG_Q_ID);	
	message.msgID = TO_DB_TASK_ALARM_MGR_ALARM_DATA;  
	/* Send data to the DB task to save in the database as well */
	if (((pALARM_INFO) message.Buffer)->flags.LogAlarm)
	{		
		SendToMsgQ(message, DB_MSG_Q_ID);
	}
 
	memset(&message, 0x00, sizeof(message));
	message.msgID = TO_UI_TASK_ALARM_DATA_INDICATION;
	snprintf(message.Buffer, MAX_SIZE_OF_MSG_BUFF, "%d%X", resetReq, alarmID);			
	SendToMsgQ(message, UI_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   - Post the encoded message to destination message queue.
*
* \param   - Message& message, MSG_Q_ID - Destination queue.
* 
* \return  - None
*
******************************************************************************/
void MainTask::SendToMsgQ(Message& message, MSG_Q_ID msgQID)
{
	if(msgQSend(msgQID,reinterpret_cast<char*>(&message),sizeof(message),NO_WAIT,MSG_PRI_NORMAL) != OK)
	{
		LOGERR("Main_T : Message send failed to MSG Q",0,0,0);
	}	
}

/**************************************************************************//**
* \brief   - Deletes all tasks
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
 
void MainTask::DeleteTasks()
{
	UINT8 taskIndex=0;
	
	for(; taskIndex<(TOTAL_NUM_OF_TASK-1); taskIndex++)
	{
		taskDelete(CP->GetTaskId(cTaskName[taskIndex]));
	}
	
}

/**************************************************************************//**
* \brief   - De-initialize the allocated stack memory for MainTask Class
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
MainTask::~MainTask()
{
	#ifdef DEBUG
		/*	need to decide on calling DestroyTask function from here.	*/
		cout << "-----------Main task destructor is called------------" << endl;
	#endif
	CP = NULL;
}

/**************************************************************************//**
* \brief   - Process Begin
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
int main()
{
	INT16 stat = ERROR;
	UINT32 iEvent 		= 0;
	UINT8  status		= SHUTDOWN_STATUS;
	bool bIsDataStruct  = true;
	TASK_ID tid			= taskIdSelf();
	MainTask *MT 		= new(nothrow) MainTask();
	Message SendMsg;
	FirmwareUpgrade Fwupdate;
	bool UbootVersionFlag;
	
	char RecvBuffer[MAX_SIZE_OF_MSG_LENGTH]	= {0x00};
	bool bIsRet;
	GpioExtPwrEn();
	GpioInit();
	BlueLedOn();
	GpioIOResetDis();
	stat = McSPI_Init_DIO();
	if (ERROR == stat)
	{
		AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_INTERNAL_COMM_EFA);
	}
	
	UbootVersionFlag = Fwupdate.IsNewBootloaderForCRC();
	if(NULL != MT)
	{
		if(UbootVersionFlag)
		{
			LOGDBG("Main_T : new bootloader present with crc partition table",0,0,0);
			MT->UpdateFirmwareInfoCRC();
		}
		else
		{
			LOGDBG("Main_T : old bootloader present without crc partition table",0,0,0);
			MT->UpdateFirmwareInfo();
		}
		MT->SetEepromValue();
		MT->SetTaskId(cTaskName[MAIN_T], tid);
		bIsDataStruct = MT->CreateSD();

		if(bIsDataStruct)
		{
			bIsDataStruct = MT->CreateMsgQ();
		}
		
		MT->FindDBFilePath();
		MT->CleanUp();
		
		SQLiteDB DataBaseObj;
		if(DataBaseObj.SetDBParameters() != SQLITE_OK)
		{
			LOGERR("MainTask : SetDBParameters Failure",0,0,0);
		}
		DataBaseObj.Close();

		if(bIsDataStruct)
		{
			bIsDataStruct = MT->CreateTasks();
		}

#ifdef SHUTDOWN_EVENT_REGISTER
		
		/*Calling this custom system call to register the shutdown event*/
		status=Gpio24VLowShutdown();
		
		if(SHUTDOWN_STATUS != status)
		{
			LOGERR("Main_T : Error in Registering Shutdown Event",0,0,0);
		}
		
#endif
		
		SendMsg.msgID = TO_DB_TASK_ALARM_CONFIG_RW_REQ;		
		memset(SendMsg.Buffer, 0x00, sizeof(SendMsg.Buffer));	
		MT->PostMessageToTask(SendMsg);
		
		if(bIsDataStruct)
		{
			while(MT->bIsTaskRunStatus())
			{
				if(eventReceive(SHUTDOWN_EVENT,EVENTS_WAIT_ANY,WAIT_FOREVER, &iEvent) != ERROR)
				{
					MT->TerminateTasks();
					taskDelay(FORTY_MS_DELAY);		/* Delay for exit all the business logic tasks */
					MT->DeleteTasks();					/*Delete all tasks*/
					MT->ExitEcatMasterSysCall();
					MT->DestroyAllMsgQ();
				}	
			}
		}
		else
		{
			LOGERR("Main_T : Data structure creation failed",0,0,0);
			MT->ExitEcatMasterSysCall();
			MT->DestroyAllMsgQ();
			MT->TerminateTasks();
		}

		MT->ReleaseMemory();
		delete MT;
	}
	else
	{
		LOGERR("Main_T : --------Memory allocation failed------------",0,0,0);
	}
	
	MT = NULL;
	LOGDBG("Main_T : EXIT FROM RTP PROCESS",0,0,0);
	return 0;
}

/**************************************************************************//**
 * \brief   - Get partition info from QSPI and Update Active and Passive
 * 			  Partition
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void MainTask::UpdateFirmwareInfo() 
{

	INT8 ret;
	FirmwarePartitionInfo fwPartInfoStruct;
	FirmwarePartitionInfo *fwPartInfo = &fwPartInfoStruct;
	Eeprom epclass;
	bool ReadEeprom =false;

	/* Read the Firmware Part Info from Qspi */
	ret = QSPIFlashRead(FW_PARTITION_INFO_QSPI_OFFSET, sizeof(FirmwarePartitionInfo), (uint32_t) fwPartInfo);

	if (ret == FAILURE) 
	{
		LOGERR("Main_T : ------ Reading Firmware Part info from QSPI failed -------", 0, 0, 0);
		/*if failed, get info from EEPROM */
		ReadEeprom = true;
	}

	/* check part info is corrupted or not */
	if ((ReadEeprom) || ((fwPartInfo->activePart != ACTIVE_PART_FIRMWARE_START) && (fwPartInfo->activePart != PASSIVE_PART_FIRMWARE_START)))
	{

		/*if corrupted , get info from EEPROM */
		ret = epclass.Read((char*) fwPartInfo, sizeof(FirmwarePartitionInfo), FW_PARTITION_INFO_EEPROM_OFFSET);

		/* Reset data if no data available */
		if ((ret < ZERO_BYTES) || ((fwPartInfo->activePart != ACTIVE_PART_FIRMWARE_START) && (fwPartInfo->activePart != PASSIVE_PART_FIRMWARE_START)))
		{
			LOGERR("Main_T : ------ Reset firmware partition info -------", 0, 0, 0);
			fwPartInfo->bootMode = ACTIVE_BOOTMODE;
			fwPartInfo->fwUpgrade = FIRMWARE_UPGRADE_FALSE;
			fwPartInfo->activePart = ACTIVE_PART_FIRMWARE_START;
			fwPartInfo->passivePart = PASSIVE_PART_FIRMWARE_START;

			/* write info to EEPROM */
			ret = epclass.Write((char *) fwPartInfo, sizeof(FirmwarePartitionInfo), FW_PARTITION_INFO_EEPROM_OFFSET);

			if (ret < ZERO_BYTES)
			{
				LOGERR("Main_T : ------ Failed reseting firmware info to EEPROM -------", 0, 0, 0);
			}
		}
		
		/* Erase the data before writing */
		QSPIFlashErase(FW_PARTITION_INFO_QSPI_OFFSET, sizeof(FirmwarePartitionInfo));

		/* Write the data to QSPI */
		ret = QSPIFlashWrite(FW_PARTITION_INFO_QSPI_OFFSET, sizeof(FirmwarePartitionInfo), (uint32_t) fwPartInfo);

		if (ret == FAILURE)
		{
			LOGCRIT("Main_T : ------ Failed reseting firmware info to QSPI -------", 0, 0, 0);
			return;
		}
	}
	
	/* Change to active partition */
	if (fwPartInfo->fwUpgrade == FIRMWARE_UPGRADE_TRUE && fwPartInfo->bootMode == PASSIVE_BOOTMODE) 
	{
		fwPartInfo->fwUpgrade = FIRMWARE_UPGRADE_FALSE;
		fwPartInfo->bootMode = ACTIVE_BOOTMODE;

		/*Swapping the partitions Active <--> Passive */
		if (fwPartInfo->activePart == ACTIVE_PART_FIRMWARE_START)
		{
			fwPartInfo->activePart = PASSIVE_PART_FIRMWARE_START;
			fwPartInfo->passivePart = ACTIVE_PART_FIRMWARE_START;
		} 
		else
		{
			fwPartInfo->activePart = ACTIVE_PART_FIRMWARE_START;
			fwPartInfo->passivePart = PASSIVE_PART_FIRMWARE_START;
		}

		/* write the updated value to EEPROM*/
		ret = epclass.Write((char *) fwPartInfo, sizeof(FirmwarePartitionInfo), FW_PARTITION_INFO_EEPROM_OFFSET);

		if (ret < ZERO_BYTES) 
		{
			LOGERR("Main_T : ------ Failed writing firmware info to EEPROM -------", 0, 0, 0);
		} 
		
		/* Erase the data before writing */
		QSPIFlashErase(FW_PARTITION_INFO_QSPI_OFFSET, sizeof(FirmwarePartitionInfo));

		/* Write the data to QSPI */
		ret = QSPIFlashWrite(FW_PARTITION_INFO_QSPI_OFFSET, sizeof(FirmwarePartitionInfo), (uint32_t) fwPartInfo);

		if (ret == FAILURE)
		{
			LOGCRIT("Main_T : ------ Failed writing firmware info to QSPI -------", 0, 0, 0);
		}
		else
		{
			LOGDBG("Main_T : ------ Firmware Upgrade Done -------", 0, 0, 0);
		}
	}

}

/**************************************************************************//**
 * \brief   - Get partition info from QSPI and Update Active and Passive
 * 			  Partition
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void MainTask::UpdateFirmwareInfoCRC() 
{

	INT8 ret;
	FirmwarePartitionInfoCRC fwPartInfoStruct;
	FirmwarePartitionInfoCRC *fwPartInfo = &fwPartInfoStruct;
	Eeprom epclass;
	bool ReadEeprom =false;

	/* Read the Firmware Part Info from Qspi */
	ret = QSPIFlashRead(FW_PARTITION_INFO_QSPI_OFFSET_CRC, sizeof(FirmwarePartitionInfoCRC), (uint32_t) fwPartInfo);

	if (ret == FAILURE) 
	{
		LOGERR("Main_T : ------ Reading Firmware Part info from QSPI failed -------", 0, 0, 0);
		/*if failed, get info from EEPROM */
		ReadEeprom = true;
	}

	/* check part info is corrupted or not */
	if ((ReadEeprom) || ((fwPartInfo->partition_a_offset != ACTIVE_PART_FIRMWARE_START) && (fwPartInfo->partition_a_offset != PASSIVE_PART_FIRMWARE_START)))
	{

		/*if corrupted , get info from EEPROM */
		ret = epclass.Read((char*) fwPartInfo, sizeof(FirmwarePartitionInfoCRC), FW_PARTITION_INFO_EEPROM_OFFSET);

		/* Reset data if no data available */
		if ((ret < ZERO_BYTES) || ((fwPartInfo->partition_a_offset != ACTIVE_PART_FIRMWARE_START) && (fwPartInfo->partition_a_offset != PASSIVE_PART_FIRMWARE_START)))
		{
			LOGERR("Main_T : ------ Reset firmware partition info -------", 0, 0, 0);
			fwPartInfo->boot_status = ACTIVE_BOOTMODE;
			fwPartInfo->fw_upgrade = FIRMWARE_UPGRADE_FALSE;
			fwPartInfo->partition_a_offset = ACTIVE_PART_FIRMWARE_START;
			fwPartInfo->partition_b_offset = PASSIVE_PART_FIRMWARE_START;

			/* write info to EEPROM */
			ret = epclass.Write((char *) fwPartInfo, sizeof(FirmwarePartitionInfoCRC), FW_PARTITION_INFO_EEPROM_OFFSET);

			if (ret < ZERO_BYTES)
			{
				LOGERR("Main_T : ------ Failed reseting firmware info to EEPROM -------", 0, 0, 0);
			}
		}
		
		/* Erase the data before writing */
		QSPIFlashErase(FW_PARTITION_INFO_QSPI_OFFSET_CRC, sizeof(FirmwarePartitionInfoCRC));

		/* Write the data to QSPI */
		ret = QSPIFlashWrite(FW_PARTITION_INFO_QSPI_OFFSET_CRC, sizeof(FirmwarePartitionInfoCRC), (uint32_t) fwPartInfo);

		if (ret == FAILURE)
		{
			LOGCRIT("Main_T : ------ Failed reseting firmware info to QSPI -------", 0, 0, 0);
			return;
		}
	}
	
	/* Change to active partition */
	if (fwPartInfo->fw_upgrade == FIRMWARE_UPGRADE_TRUE && fwPartInfo->boot_status == PASSIVE_BOOTMODE) 
	{
		fwPartInfo->fw_upgrade = FIRMWARE_UPGRADE_FALSE;
		fwPartInfo->boot_status = ACTIVE_BOOTMODE;

		/*Swapping the partitions Active <--> Passive */
		if (fwPartInfo->partition_a_offset == ACTIVE_PART_FIRMWARE_START)
		{
			fwPartInfo->partition_a_offset = PASSIVE_PART_FIRMWARE_START;
			fwPartInfo->partition_b_offset = ACTIVE_PART_FIRMWARE_START;
		} 
		else
		{
			fwPartInfo->partition_a_offset = ACTIVE_PART_FIRMWARE_START;
			fwPartInfo->partition_b_offset = PASSIVE_PART_FIRMWARE_START;
		}

		/* write the updated value to EEPROM*/
		ret = epclass.Write((char *) fwPartInfo, sizeof(FirmwarePartitionInfoCRC), FW_PARTITION_INFO_EEPROM_OFFSET);

		if (ret < ZERO_BYTES) 
		{
			LOGERR("Main_T : ------ Failed writing firmware info to EEPROM -------", 0, 0, 0);
		} 
		
		/* Erase the data before writing */
		QSPIFlashErase(FW_PARTITION_INFO_QSPI_OFFSET_CRC, sizeof(FirmwarePartitionInfoCRC));

		/* Write the data to QSPI */
		ret = QSPIFlashWrite(FW_PARTITION_INFO_QSPI_OFFSET_CRC, sizeof(FirmwarePartitionInfoCRC), (uint32_t) fwPartInfo);

		if (ret == FAILURE)
		{
			LOGCRIT("Main_T : ------ Failed writing firmware info to QSPI -------", 0, 0, 0);
		}
		else
		{
			LOGDBG("Main_T : ------ Firmware Upgrade Done -------", 0, 0, 0);
		}
	}
	
	/* ADDED BY MOUNIKA*/
	if(fwPartInfo->partition_a_offset == ACTIVE_PART_FIRMWARE_START)
	{
		FWCrc::Set(FW_CRC_SC,fwPartInfo->partition_a_crc);
		printf("************SC FirmwarePartitionInfoCRC get from Fwcrc : %d\n",FWCrc::Get(FW_CRC_SC));
	}	
	if(fwPartInfo->partition_a_offset == PASSIVE_PART_FIRMWARE_START)
	{
		FWCrc::Set(FW_CRC_SC,fwPartInfo->partition_b_crc);
		printf("************SC FirmwarePartitionInfoCRC get from Fwcrc : %d\n",FWCrc::Get(FW_CRC_SC));
	}

}
