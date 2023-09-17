/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef ECSDO_TASK_H_
#define ECSDO_TASK_H_

#include "Common.h"
#include "CommonProperty.h"
#include "WeldRecipe.h"
#include "AlarmData.h"
#include "FirmwareUpgrade.h"

//#define 	SDO_DEBUG

class EcSDOTask
{
public:
	EcSDOTask();
	~EcSDOTask();

	void 			SendToMsgQ();
	void 			ProcessMessage();
	bool			bIsTaskRunStatus();
	void 			EnCoder(UINT32 msg_id);
	EC_MASTER_STATE EcatGetMasterStateSysCall();
	void 			DeCoder(char *RecvBuffer, Message& message);
	void 			SendToMsgQ(Message& msg, const MSG_Q_ID &MsgQID);
	void 			GetSdoFromOD(UINT8 sdoULType, char *pRecvBuffer);
	UINT32 			CoeSdoUploadReqSysCall(UINT16 iIndex, UINT16 iSubIndex, UINT16 iSlaveID, UINT16 iLength, UINT16 iAccessType);
	UINT32 			CoeSdoDownloadReqSysCall(UINT16 iIndex, UINT16 iSubIndex, UINT16 iSlaveID, UINT8 iAccessType, UINT16 iLength,unsigned char *pData);
	
private:
	bool 			GetPcWeldRecipe(unsigned char *pRecipe);
	bool 			GetAcWeldRecipe(unsigned char *pRecipe);
	void 			PrintRecipe(seekRecipe& );
	void 			PrintRecipe(ScanRecipe &);
	void 			PrintRecipe(WeldRecipePC& );
	void			PrintRecipe(WeldRecipeAC& );
	void 			PrintRecipe(TestRecipe &);
			
	
private:
	MSG_Q_ID 		SDO_MSG_Q_ID, UI_MSG_Q_ID, DB_MSG_Q_ID,CTRL_MSG_Q_ID;
	WeldRecipePC 	                   weldRecipePC;
	WeldRecipeAC 	                   weldRecipeAC;
	WeldRecipeAcCompatibleWithAcBoard  weldRecipeAcCompatibleWithAcBoard;
	seekRecipe		                   m_seekRecipe;
	ScanRecipe		                   scanRecipe;
	char 			                   *pRecvBuffer;
	char 			                   MsgBuffer[MAX_SIZE_OF_MSG_LENGTH];
	char 			                   RecvMsgBuffer[MAX_SIZE_OF_MSG_LENGTH];
	unsigned char	                   pData[MAX_SIZE_OF_MSG_LENGTH];
	CommonProperty                     *CP;
	INT32  			                   DigitalTune;
	INT32  			                   MemOffset;
	UINT32 			                   Memory;
};


#endif /* ECSDO_TASK_H_ */
