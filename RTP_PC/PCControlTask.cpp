/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     It contains Real time(PDO) and Non Real time(SDO) data processing functions
 
**********************************************************************************************************/

#include "PCObjDictionary.h"
#include <sysLib.h>
#include <semLib.h>
#include <sdLib.h>
#include <msgQLib.h>
#include <errnoLib.h>
#include "GpioAPI.h"
using namespace std;


PcRealtimeData *pRtData;		// Shared memory for Pdo data transfer requires global pointer
extern MSG_Q_ID CtrlMid;

/******************************************************************************
*      Function    :   PCControlTask
*      Description :   This function is responsible for receiving timer event 
*      Input       :   void
*      Output      :   void
*      Called by   :   timer_handler
*******************************************************************************/

void PCControlTask()
{	
	
	TxPDO txobj;
	RxPDO rxobj;
	Buffer = (UINT16 MBXMEM*)malloc(BUFFERSIZE);
	VersionInfoSDO versionInfo;
	UINT32 events = 0,RecivedData;
	UINT32 Ctr 	= 0;
	UINT8 RxPDO_Data_Recieved = 0;	
	INT32 result = FAILED;
	INT32 Timeout = WAIT_FOREVER;
	
	UINT8 OverloaData =0x00 ,ActionIData =0x00;	
	char RecBufferCtrl[100] = {0x00};
	SD_ID RtSD = sdOpen("/RealTimeData", 0, 0, 0, 0, SD_ATTR_RWX | SD_CACHE_OFF, (void **) &pRtData);
	if(SD_ID_NULL == RtSD)
	{
		LOG("Failed to Open Shared Data!!!\n");
		exit(-1);		
	}		
	versionInfo.WriteFirmwareInfoToOD ();
	while(1)
	{	
		
		if (eventReceive(VXEV03,EVENTS_WAIT_ANY, Timeout, &events) == OK )
		{	
			/*CtrlTask : Application Processing logic*/
			Timeout = PDO_TIMEOUT;
			rxobj.recieveRxPDOFromOD();	
			rxobj.processRxPDO();
			txobj.sendTxPDOToOD();
		}
		else if (errnoGet() == S_eventLib_TIMEOUT)
		{
//			Alarm::SetAlarmFlag();
//			LOG("TimeOut");
			PCStateMachine::masterState = SC_UNREACHABLE;
			rxobj.processRxPDO();
			
		}
	}
}

