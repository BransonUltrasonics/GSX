/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/



#ifndef MAINTASK_H_
#define MAINTASK_H_

#include "Common.h"
#include "CommonProperty.h"
#include <string.h>
#include "commons.h"
#include "ScDgtInput.h"
#include "ScDgtOutput.h"
#include "MaintenanceTask.h"

/* Message queue OFFSETS */
#define MAX_MSG					1000 
#define MAX_MSG_LEN				1024 
#define MSG_Q_FIFO				0x00
#define MSG_Q_PRIORITY			0x01

/* No of task going to create in Supervisory Controller */
#define NO_OF_TASK 				13
#define SHUTDOWN_STATUS			1

/* Priority of tasks in Supervisory Controller */

enum TASK_PRIORITY
{
	CONSOLE_T_PRIORITY			= 10,
	CTRL_T_PRIORITY				= 220,
	IO_IMAGE_HANDLER_T_PRIORITY,
	SDO_T_PRIORITY,
	ALARM_T_PRIORITY,
	UI_T_PRIORITY,
	DB_T_PRIORITY,
	UI_DB_T_PRIORITY,
	DGTIN_T_PRIORITY,
	DGTOUT_T_PRIORITY,
	RECV_T_PRIORITY,
	MAIL_BOX_MSG_HANDLER_T_PRIORITY,
	BARCODE_READER_T_PRIORITY,
	WEBSERVICE_T_PRIORITY,
	MAINTENANCE_T_PRIORITY
};

#define FW_UPGRADE_T_PRIORITY   225

/* Stack size of tasks in Supervisory Controller */
#define CONSOLE_T_STACK_SIZE	(1024 * 6)
#define CTRL_T_STACK_SIZE		(1024 * 20)
#define SDO_T_STACK_SIZE		(1024 * 6)
#define ALARM_T_STACK_SIZE		(1024 * 6)
#define UI_T_STACK_SIZE			(1024 * 6)
#define UI_DB_T_STACK_SIZE		(1024 * 20)
#define DB_T_STACK_SIZE			(1024 * 20)
#define RECV_T_STACK_SIZE		(1024 * 8)
#define FW_UPGRADE_T_SATCK_SIZE (1024 * 8)
#define DGTIN_T_STACK_SIZE		(1024 * 4)
#define DGTOUT_T_STACK_SIZE		(1024 * 4)
#define BARCODE_READER_T_STACK_SIZE (1024 * 2)
#define WEBSERVICE_T_STACK_SIZE (1024 * 20)
#define MAINTENANCE_T_STACK_SIZE (1024 * 8)
#define IO_IMAGE_HANDLER_T_STACK_SIZE (1024 * 8)
#define MAIL_BOX_MSG_HANDLER_T_STACK_SIZE (1024 * 20)//Will recalculate stack size properly in the future

/* Shared data object size for PDO data */
#define SD_SIZE					512

/* Shared data public object name */
#define RX_DATA_PC				"/RxPDO_PC"
#define RX_DATA_AC				"/RxPDO_AC"
#define TX_DATA_PC				"/TxPDO_PC"
#define TX_DATA_AC				"/TxPDO_AC"

/* Global structure object for PDO DATA */
TxPDO_PC *PC_TX;
TxPDO_AC *AC_TX;
RxPDO_PC *PC_RX;
RxPDO_AC *AC_RX;

/* List of business logic tasks function routine declaration*/
void Control_Task(void);
void EcSDO_Task(void);
void UserInterface_Task(void);
void DataBase_Task(void);
void UI_DataBase_Task(void);
void Socket_Recv(void);
void ConsoleApp_Task(void);
void Alarm_Task(void);
void FirmwareUpgrade_Task(void);
void BarcodeReader_Task(void);
void HttpWebService_Task(void);
void Maintenance_Task(void);
void IOImage_Handler_Task(void);
void Mail_Box_Message_Handler_Task(void);

/* FUNCPTR for available business logic tasks */
typedef void FUNC(void);
FUNC Control_Task,EcSDO_Task,UserInterface_Task,DataBase_Task,UI_DataBase_Task,Alarm_task,FirmwareUpgrade_Task,ScDgtInputTask,ScDgtOutPutTask,BarcodeReader_Task,HttpWebService_Task;

struct TaskInfo
{
	std::string	TaskName;
	UINT32 		Priority;
	FUNC*   	TaskFunc;
	UINT32 		StackSize;	
};

class MainTask
{
public:
	MainTask();
	~MainTask();

	bool 			CreateSD();
	bool			CreateMsgQ();
	bool			CreateTasks();
	void			ReleaseMemory();
	bool			DestroyAllMsgQ();
	void			SetEepromValue();
	void			TerminateTasks();
	bool			bIsTaskRunStatus();
	void 			ExitEcatMasterSysCall();
	void 			SetTaskId(std::string T_Name, TASK_ID TID);
	void 			UpdateFirmwareInfo();
	void 			UpdateFirmwareInfoCRC();
	void			FindDBFilePath();
	void 			PostMessageToTask(Message &);
	void 			DeleteTasks();
		
	void 			ProcessAlarm (UINT8 org, UINT16 alarmID = 0);
	void 			SendToMsgQ(Message& message, MSG_Q_ID msgQID);
	void 			CleanUp();

	
private:
	struct TaskInfo	taskinfo[NO_OF_TASK];
	TASK_ID			tid[NO_OF_TASK];
	CommonProperty	*CP;
};

#endif /* MAINTASK_H_ */
