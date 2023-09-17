/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef COMMON_H_
#define COMMON_H_

/* VxWorks API support header files */
#include  <sys/types.h>
#include  <eventLib.h>
#include  <msgQLib.h>
#include  <timerLib.h>
#include  <sysLib.h>
#include  <tickLib.h>
#include  <timerDev.h>
#include  <taskLib.h>
#include  <semLib.h>
#include  <sys/time.h>
#include  <logLib.h>
#include  <usrLib.h>
#include  <sdLib.h>
#include  <rebootLib.h>

/* Language support header files */
#include <iostream>
#include <string.h>

/* Event registers */
#define CTRL_250us				0x01
#define MAX_SIZE_OF_MSG_LENGTH	512
#define	SD_SIZE					512

/* PDO's stationary values*/
#define SC_UNREACHABLE 			0xFF

/* Public control task name */
#define	CTRL_TASK_NAME			"/Ctrl_Task"

/* Public shared data region name */
#define RX_DATA_PC				"/RxPDO_PC"
#define RX_DATA_AC				"/RxPDO_AC"
#define TX_DATA_PC				"/TxPDO_PC"
#define TX_DATA_AC				"/TxPDO_AC"

#define SDO_MSG_Q_NMAE			"/SDO_Task"

#define TO_SDO_TASK_DOWNLOAD_SEEK_RESP	   	10
#define TO_SDO_TASK_UL_SEEK_RESPONSE		14
#define TO_SDO_TASK_WELD_RECIPE_RESP_PC		24
#define TO_SDO_TASK_WELD_RECIPE_RESP_AC		25
#define TO_SDO_TASK_WELD_RESULT_RESP_PC		26
#define TO_SDO_TASK_WELD_RESULT_RESP_AC		27
#define TO_SDO_TASK_DOWNLOAD_POWERUP_RESP	30
#define TO_SDO_TASK_HORN_SCAN_RECIPE_RESP 	32
#define TO_SDO_TASK_UL_ALARM_DATA_RESP_PC	40
#define TO_SDO_TASK_UL_ALARM_DATA_RESP_AC	42
#define TO_SDO_TASK_VERSION_REQUEST_RESP_AC 44
#define TO_SDO_TASK_VERSION_REQUEST_RESP_PC 45
#define TO_SDO_TASK_DOWNLOAD_TMR_INTVL_RESP	132	
#define TO_SDO_TASK_CALIBRATION_READINGS_RESP_AC 63
#define TO_SDO_TASK_TEST_RECIPE_RES 		94
#define TO_SDO_TASK_FW_UPGRADE_INFO_RESP_AC     151
#define TO_SDO_TASK_FW_UPGRADE_INFO_RESP_PC     153
#define TO_SDO_TASK_FW_UPGRADE_STATUS_RESP_AC   155
#define TO_SDO_TASK_FW_UPGRADE_STATUS_RESP_PC   157
#define TO_SDO_CALIBRATION_DATA_REQUEST_RESP	166

/* Specific index for OD operations */
#define WELD_RECIPE_RESP_PC_INDEX			8192
#define DOWNLOAD_POWERUP_RESP_INDEX			8193
#define HORN_SCAN_RECIPE_RESP_INDEX			8200
#define DOWNLOAD_SEEK_RESP_INDEX			8208
#define FW_DATA_RESP_PC_INDEX				8197
#define WELD_RECIPE_RESP_AC_INDEX			16384
#define ALARM_DATA_RESP_PC_INDEX			8194
#define ALARM_DATA_RESP_AC_INDEX			16390
#define FW_DATA_RESP_AC_INDEX				16386
#define DOWNLOAD_TEST_RESP_INDEX			8198
#define DOWNLOAD_TMRINTERVAL_RESP_INDEX		8196  
#define CALIBRATION_READINGS_RESP_AC_INDEX	16388
#define FW_UPGRADE_INFO_RESP_PC_INDEX       8224
#define FW_UPGRADE_INFO_RESP_AC_INDEX       16416



/* Global variable to store bus index of connected slaves */
UINT32 	SlaveIdPC;
UINT32	SlaveIdAC;
TASK_ID Ctrl_TID;
bool	bIsExitKeyPressed;

/********* Test PDO Structures of PC and AC*******/
typedef struct RxPDO_PC
{
	UINT16	MasterState;
	UINT16	Amplitudechange;
	UINT16	MasterEvents;
	UINT32	Frequency;
}RxPDO_PC;

typedef struct RxPDO_AC
{
	UINT8	SCState;
	UINT8	ACControlEvent;
	UINT8	ACOutputs;
}RxPDO_AC;

typedef struct TxPDO_PC
{
	UINT16  Amplitude;
	INT16   Phase;
	UINT16  Power;
	UINT32  Frequency;
	UINT16  Current;
	UINT8   PC_StatusEvent;
	UINT8	PCState;
}TxPDO_PC;

typedef struct TxPDO_AC
{
	UINT16  ActualForce;
	UINT32 	ActualDistance;
	UINT8   ACStatusEvent;
	UINT8   ACState;
	UINT8   ACInputs;
}TxPDO_AC;

typedef struct ScanSignature
{
	UINT16  Amplitude;
	UINT16  Current;
	UINT16  Phase;
	UINT32  Frequency;
}ScanSignature;

typedef struct Message
{
	INT32	msgID;
	char	Buffer[MAX_SIZE_OF_MSG_LENGTH-4];	
}Message;


//void SendSdoDownloadResp(UINT32 IndexAddr, UINT32 SlaveAddr);
void SendSdoDownloadResp(UINT32 IndexAddr, UINT32 SlaveAddr, UINT32 ErrCode);
void SendSdoUploadResp(UINT32 IndexAddr, UINT32 SlaveAddr,  unsigned char* pData, UINT32 iLength);
void EnCodeErrorResp(char *DestMsg, char* ErrMsg,UINT32 ErrCode = 1);
void AssignSdoMsgQID();

#endif /* COMMON_H_ */
