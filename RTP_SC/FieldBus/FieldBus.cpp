/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2012
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 It contains the Fieldbus Abstract class related implementation   
 
**********************************************************************************************************/

#include <iostream>
#include "cifXToolkit.h"
#include "cifXHWFunctions.h"
#include "cifXErrors.h"
#include "SerialDPMInterface.h"
#include "Hil_ApplicationCmd.h"
#include "Hil_Results.h"
#include "cifXEndianess.h"
//#include "OS_Spi.h"
#include "FieldBus.h"
#include "EthernetIP.h"

bool FieldBus::m_bisInitialised = false;
CIFXHANDLE FieldBus::HDriver;
CIFXHANDLE FieldBus::HChannel;
PDEVICEINSTANCE PtDevInstance;

/**************************************************************************//**
* 
* \brief   - Constructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
FieldBus::FieldBus()
{
//	HDriver = NULL;
//	HChannel = NULL;
}

/**************************************************************************//**
* 
* \brief   - Destructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
FieldBus::~FieldBus()
{

}

/**************************************************************************//**
* 
* \brief   - Check if FieldBus is Initialized and ready
*
* \param   - None.
*
* \return  - m_bisInitialised flag.
*
******************************************************************************/
bool FieldBus::GetFieldBusInitialised()
{
	return m_bisInitialised;
}

/**************************************************************************//**
* 
* \brief   - Set m_bisInitialised to true to indicate FieldBus is Initialized
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void FieldBus::SetFieldBusInitialised()
{
	m_bisInitialised = true;
}

/**************************************************************************//**
* 
* \brief   - Prints the cifX error description to the terminal
*
* \param   - lRetErr - Contains error code returned by Toolkit functions.
*
* \return  - None.
*
******************************************************************************/
void FieldBus::cifXErrorDescription(INT32 lRetErr)
{
	char szError[1024] = {0}; 
	
	xDriverGetErrorDescription(lRetErr, szError, sizeof(szError));
	LOGERR("Hilscher initialization error: Code: %08X Description: %s\n", lRetErr, (_Vx_usr_arg_t)szError, 0);
}

/**************************************************************************//**
* 
* \brief   - SendWarmStart
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void FieldBus::SendWarmStart()
{
	
}

/**************************************************************************//**
* 
* \brief   - This function prints the System and Communication channel
* 			information.
*
* \param   - None.
*
* \return  - lRetErr: Contains error code returned by Toolkit functions.
*
******************************************************************************/
UINT32 FieldBus::GetHilModuleInfo()
{
	UINT32 ulBoard = 0, ulChannel = 0;
	INT32 lRetErr = CIFX_GENERAL_ERROR;
	CIFXHANDLE HSystem = NULL;
	
	lRetErr = xSysdeviceOpen(FieldBus::HDriver, "cifX0", &HSystem);
	
	if(CIFX_NO_ERROR == lRetErr){
		
		memset(&tSysInfo, 0, sizeof(tSysInfo));
		
		lRetErr = xSysdeviceInfo(HSystem, CIFX_INFO_CMD_SYSTEM_INFO_BLOCK, sizeof(tSysInfo), &tSysInfo);
		
		if(CIFX_NO_ERROR == lRetErr)
		{
			LOGDBG("Device Number    : %lu\r\n", tSysInfo.ulDeviceNumber,0,0);
			LOGDBG("Serial Number    : %lu\r\n", tSysInfo.ulSerialNumber,0,0);
			LOGDBG("Manufacturer     : %u\r\n", tSysInfo.usManufacturer,0,0);
			LOGDBG("HW Revision      : %u\r\n", tSysInfo.bHwRevision,0,0);
			
			memset(&tChannelInfo, 0, sizeof(tChannelInfo));
			
			lRetErr = xDriverEnumChannels(FieldBus::HDriver, ulBoard, ulChannel, sizeof(tChannelInfo), &tChannelInfo);
			
			if(CIFX_NO_ERROR == lRetErr)
			{
				LOGDBG("Channel %u:\r\n",                   (int)ulChannel,0,0);
				LOGDBG("Firmware: %s\r\n",                  (_Vx_usr_arg_t)tChannelInfo.abFWName,0,0);
				LOGDBG("Version : %u.%u.%u ",
															tChannelInfo.usFWMajor,
															tChannelInfo.usFWMinor,
															tChannelInfo.usFWRevision);
				LOGDBG("build %u\r\n", 						tChannelInfo.usFWBuild,0,0);
				LOGDBG("Date: %02u/%02u/%04u\r\n",
															tChannelInfo.bFWMonth,
															tChannelInfo.bFWDay,
															tChannelInfo.usFWYear);
			}
			else
				LOGERR("Communication Channel not available\n",0,0,0);
		}
		else
			LOGERR("System Channel not available\n",0,0,0);
	}
	
	xSysdeviceClose(HSystem);
	
	return lRetErr;
}

/**************************************************************************//**
* 
* \brief   - This function initializes everything related to Hilscher card
* 				interface.
*
* \param   - None.
*
* \return  - lRetErr: Contains error code returned by Toolkit functions.
*
******************************************************************************/
INT32 FieldBus::HilscherInit()
{
	INT32 lRetErr = CIFX_GENERAL_ERROR;
	INT32 iSerDPMType = SERDPM_UNKNOWN; //This variable contains the netX chip type
	
	/*Initialize toolkit*/
	lRetErr = cifXTKitInit();
	
	if (CIFX_NO_ERROR == lRetErr) {
		
		LOGDBG("Toolkit initialization success\n",0,0,0);
		
		/*Instantiate PtDevInstance*/
		PtDevInstance = (PDEVICEINSTANCE) OS_Memalloc(sizeof(*PtDevInstance));
		OS_Memset(PtDevInstance, 0, sizeof(*PtDevInstance));
		
		if(NULL != PtDevInstance){
			
			/*Fill the device information*/
			PtDevInstance->pvOSDependent = PtDevInstance; //User dependent data, used for OS_xxx and USER_xxx functions
			PtDevInstance->ulDPMSize = 0x10000; //might need to be changed
			PtDevInstance->fIrqEnabled = 0; //We are not using interruptions
			OS_Strncpy(PtDevInstance->szName, "cifX0", sizeof(PtDevInstance->szName));
			
			/*Initialize Serial DPM*/
			iSerDPMType = SerialDPM_Init((DEVICEINSTANCE*)PtDevInstance);
			
			if (SERDPM_UNKNOWN != iSerDPMType) {
				
				LOGDBG("Serial DPM initialization success\n",0,0,0);
				
				char* szSerDPM[] = {"", "NETX10", "NETX50", "NETX51", "NETX100"};
				LOGDBG("Serial DPM protocol detected: %s\r\n", (_Vx_usr_arg_t)szSerDPM[iSerDPMType],0,0);
				
				/*Add device to the toolkit*/
				lRetErr = cifXTKitAddDevice(PtDevInstance);
				
				if (CIFX_NO_ERROR == lRetErr){
					LOGDBG("Device added successfully\n",0,0,0);
						
					/*Open Driver*/
					lRetErr = xDriverOpen(&HDriver);
					
					if (CIFX_NO_ERROR == lRetErr){
						LOGDBG("Driver opened successfully\n",0,0,0);
							   
						/*Open (Communication) Channel*/
						lRetErr = xChannelOpen(HDriver, PtDevInstance->szName, 0, &HChannel);
						
						if (CIFX_NO_ERROR == lRetErr){
							LOGDBG("Comm Channel opened successfully\n",0,0,0);
							lRetErr = GetHilModuleInfo();
						}
						else
						{
							xDriverClose(&HDriver);
							cifXTKitRemoveDevice(PtDevInstance->szName,1);
						}
					}
					else
						cifXTKitRemoveDevice(PtDevInstance->szName,1);
				}
			}
			else
				lRetErr = CIFX_INVALID_BOARD;
		}
		else
			lRetErr = CIFX_INVALID_POINTER;
	}
	
	if(CIFX_NO_ERROR != lRetErr)
	{
		//Hilscher configuration failed
		LOGERR("Hilscher module configuration failed\n",0,0,0);
		cifXErrorDescription(lRetErr);
		cifXTKitDeinit();
	}
	
	return lRetErr;
}

/**************************************************************************//**
* 
* \brief   - This function sends the configuration packets to Hilscher card
* 				interface.
*
* \param   - None.
*
* \return  - void.
*
******************************************************************************/
void FieldBus::configuration()
{
	CIFX_PACKET tSendPkt = {{0}};
	CIFX_PACKET tRecvPkt = {{0}};
	INT32 lRet = 0, StatusProtocolSpecific;
	EthernetIP  *FbPtr = EthernetIP::Getinstance();
	UINT32 SendPktCnt = 0,ReceivePktCnt = 0;
	char StatusMessage[STATUS_MESSAGES_LENGTH]= "";
	OS_Memset(&tSendPkt, 0, sizeof(tSendPkt));
	OS_Memset(&tRecvPkt, 0, sizeof(tRecvPkt));
	
	tSendPkt.tHeader.ulDest = HOST_TO_LE32(HIL_PACKET_DEST_DEFAULT_CHANNEL);
	tSendPkt.tHeader.ulSrc = 0;
	tSendPkt.tHeader.ulCmd = HOST_TO_LE32(HIL_REGISTER_APP_REQ);
	
	if(CIFX_NO_ERROR != (lRet = xChannelPutPacket(HChannel, &tSendPkt, 10)))
	{
		sprintf(StatusMessage,"Error sending packet to device! 0x%08x \r\n",lRet);
		LOGERR(StatusMessage, 0, 0, 0);
	}
	else
	{
		if(CIFX_NO_ERROR != (lRet = xChannelGetPacket(HChannel, sizeof(tRecvPkt), &tRecvPkt, 20)) )
		{
			sprintf(StatusMessage,"Error getting packet from device! 0x%08x \r\n",lRet);
			LOGERR(StatusMessage, 0, 0, 0);
		}
		sprintf(StatusMessage,"\n%sApp Register 1 ulCmd: 0x%08x , ulState: 0x%08x%s\n","\x1B[31m", tRecvPkt.tHeader.ulCmd, tRecvPkt.tHeader.ulState, "\x1B[0m");
		LOGDBG(StatusMessage, 0, 0, 0);
		
		if(CIFX_NO_ERROR != (lRet = xChannelGetPacket(HChannel, sizeof(tRecvPkt), &tRecvPkt, 20)) )
		{
			sprintf(StatusMessage,"Error getting packet from device! 0x%08x \r\n",lRet);
			LOGERR(StatusMessage, 0, 0, 0);
		}
		sprintf(StatusMessage,"\n%sApp Register 2 ulCmd: 0x%08x , ulState: 0x%08x%s\n","\x1B[31m", tRecvPkt.tHeader.ulCmd, tRecvPkt.tHeader.ulState, "\x1B[0m");
		LOGDBG(StatusMessage, 0, 0, 0);
	}
	      
	
	OS_Memset(&tSendPkt, 0, sizeof(tSendPkt));
	OS_Memset(&tRecvPkt, 0, sizeof(tRecvPkt));
	
	USER_GetWarmstartParameters(&tSendPkt);
	
	if(CIFX_NO_ERROR != (lRet = xChannelPutPacket(HChannel, &tSendPkt, 10)))
	{
		sprintf(StatusMessage,"Error sending packet to device! 0x%08x \r\n",lRet); 
		LOGERR(StatusMessage, 0, 0, 0);
	} 
	else
	{
		if(CIFX_NO_ERROR != (lRet = xChannelGetPacket(HChannel, sizeof(tRecvPkt), &tRecvPkt, 20)) )
		{
			sprintf(StatusMessage,"Error getting packet from device! 0x%08x \r\n",lRet);
			LOGERR(StatusMessage, 0, 0, 0);
		}
		sprintf(StatusMessage,"\n%sConfig Packet ulCmd: 0x%08x , ulState: 0x%08x%s\n","\x1B[32m", tRecvPkt.tHeader.ulCmd, tRecvPkt.tHeader.ulState, "\x1B[0m");
		LOGDBG(StatusMessage, 0, 0, 0);
	}
	
	
	OS_Memset(&tSendPkt, 0, sizeof(tSendPkt));
	OS_Memset(&tRecvPkt, 0, sizeof(tRecvPkt));
	
	tSendPkt.tHeader.ulDest = HOST_TO_LE32(HIL_PACKET_DEST_DEFAULT_CHANNEL);
	tSendPkt.tHeader.ulCmd = HOST_TO_LE32(HIL_CHANNEL_INIT_REQ);
	
	if(CIFX_NO_ERROR != (lRet = xChannelPutPacket(HChannel, &tSendPkt, 10)))
	{
		sprintf(StatusMessage,"Error sending packet to device! 0x%08x \r\n",lRet);
		LOGERR(StatusMessage, 0, 0, 0);
	} 
	else
	{
		if(CIFX_NO_ERROR != (lRet = xChannelGetPacket(HChannel, sizeof(tRecvPkt), &tRecvPkt, 50)) )
		{
			sprintf(StatusMessage,"Error getting packet from device! 0x%08x \r\n",lRet);
			LOGERR(StatusMessage, 0, 0, 0);
		} 
		sprintf(StatusMessage,"\n%sChannel Init Packet ulCmd: 0x%08x , ulState: 0x%08x%s\n","\x1B[33m", tRecvPkt.tHeader.ulCmd, tRecvPkt.tHeader.ulState, "\x1B[0m");
		LOGDBG(StatusMessage, 0, 0, 0);
	}
	
	StatusProtocolSpecific= FbPtr ->ProtocolSpecificInit(FieldBus::HChannel);
	sprintf(StatusMessage,"Status %x\r\n", StatusProtocolSpecific);
	LOGDBG(StatusMessage, 0, 0, 0);
	OS_Memset(&tSendPkt, 0, sizeof(tSendPkt));
	OS_Memset(&tRecvPkt, 0, sizeof(tRecvPkt));
	
	HIL_START_STOP_COMM_REQ_T* ptReq = (HIL_START_STOP_COMM_REQ_T*)&tSendPkt;
			  
	ptReq->tHead.ulDest = HOST_TO_LE32(HIL_PACKET_DEST_DEFAULT_CHANNEL);
	ptReq->tHead.ulSrc = 0;
	ptReq->tHead.ulCmd = HOST_TO_LE32(HIL_START_STOP_COMM_REQ);
	ptReq->tHead.ulLen  = HOST_TO_LE32(sizeof(ptReq->tData));
	ptReq->tData.ulParam = HOST_TO_LE32(HIL_START_STOP_COMM_PARAM_START);
	
	/* Do a basic Packet Transfer */
	if(CIFX_NO_ERROR != (lRet = xChannelPutPacket(HChannel, &tSendPkt, 10)))
	{
		sprintf(StatusMessage,"Error sending packet to device! 0x%08x \r\n",lRet);
		LOGERR(StatusMessage, 0, 0, 0);
	} 
	else
	{
		if(CIFX_NO_ERROR != (lRet = xChannelGetPacket(HChannel, sizeof(tRecvPkt), &tRecvPkt, 20)) )
		{
			sprintf(StatusMessage,"Error getting packet from device! 0x%08x \r\n",lRet);
			LOGERR(StatusMessage, 0, 0, 0);
		} 
		sprintf(StatusMessage,"\n%sHost Ready Packet ulCmd: 0x%08x , ulState: 0x%08x%s\n","\x1B[34m", tRecvPkt.tHeader.ulCmd, tRecvPkt.tHeader.ulState, "\x1B[0m");
		LOGDBG(StatusMessage, 0, 0, 0);
	}
	
	lRet = xChannelGetMBXState((PCHANNELINSTANCE)FieldBus::HChannel, &ReceivePktCnt, &SendPktCnt);
	
	//Make sure all packets received during initialization are consumed
	while (ReceivePktCnt>0)
	{
		lRet = xChannelGetPacket(HChannel, sizeof(tRecvPkt), &tRecvPkt, 20);
		sprintf(StatusMessage,"\n%sPacket ulCmd: 0x%08x , ulState: 0x%08x%s\n","\x1B[34m", tRecvPkt.tHeader.ulCmd, tRecvPkt.tHeader.ulState, "\x1B[0m");
		LOGDBG(StatusMessage, 0, 0, 0);
		ReceivePktCnt--;
	}
}

/**************************************************************************//**
* 
* \brief   - Function is used to Signal host application is ready and switch On Bus state flag 
*        	 in the communication channel host handshake flags.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void FieldBus::InitIOCommunication()
{
	UINT32 lRet = 0,
		   ulState = 0;

	/* Signal application is ready */
	lRet = xChannelHostState(FieldBus::HChannel, CIFX_HOST_STATE_READY, &ulState, HOSTSTATE_TIMEOUT);
	if(CIFX_NO_ERROR != lRet)
	{
		LOGERR("Unable to Signal ready. Error: %08x\r\n", lRet, 0, 0);
	}
	else
	{
		/* Set BUS state flag */
		lRet =  xChannelBusState(FieldBus::HChannel, CIFX_BUS_STATE_ON, &ulState, HOSTSTATE_TIMEOUT);
		if(CIFX_NO_ERROR != lRet)
		{
			if(CIFX_DEV_NO_COM_FLAG != lRet)
			{
				LOGERR("Unable to start the filed bus. Error: %08x\r\n", lRet, 0, 0);
			}
			else if(1 == ulState)
			{
				LOGDBG("Bus is ON!\r\n", 0,0,0);
			}
		}
		else
		{
			LOGERR("Bus Communication Failure. Error = %08x", lRet,0,0);
		}
	}
}

/****************************************************************************
* 
* \brief   - BasicConfigurationPacketSet
*			Sends all the required configuration packets
*			
* \param   - None.
*
* \return  - ErrorCode. Indicates if an error has occurred
*
******************************************************************************/
INT32 FieldBus::BasicConfigurationPacketSet()
{
	INT32 ErrorCode= CIFX_GENERAL_ERROR;
	CIFX_PACKET	tSendPkt = {{0}},
				tRecvPkt = {{0}};
	EthernetIP  *FbPtr = EthernetIP::Getinstance();
	char StatusMessage[STATUS_MESSAGES_LENGTH]= "";
	HIL_START_STOP_COMM_REQ_T* ptrSendPkt = (HIL_START_STOP_COMM_REQ_T*)&tSendPkt;
	
	sprintf(StatusMessage, "\n%sSending Configuration Packets for Hilscher Module%s", KCYN, KNRM);
	LOGDBG(StatusMessage, 0, 0, 0);

	      
// ToDo: Sent all the configuration packets and set the ErrorCode value
	
// Register Application
	OS_Memset(&tSendPkt, 0, sizeof(tSendPkt));
	OS_Memset(&tRecvPkt, 0, sizeof(tRecvPkt));
	      
	tSendPkt.tHeader.ulDest = HOST_TO_LE32(HIL_PACKET_DEST_DEFAULT_CHANNEL);
	tSendPkt.tHeader.ulSrc = HOST_TO_LE32(0); //Unique application identifier
	tSendPkt.tHeader.ulCmd = HOST_TO_LE32(HIL_REGISTER_APP_REQ);

	ErrorCode= DEV_TransferPacket(HChannel, &tSendPkt, &tRecvPkt, sizeof(tRecvPkt), PUTPACKET_TIMEOUT, NULL, NULL);
	if ((CIFX_NO_ERROR != ErrorCode) || (SUCCESS_HIL_OK != tRecvPkt.tHeader.ulState))
	{
		sprintf(StatusMessage, "\n%sRegistering Hilscher Application Failed! Error Code: 0x%08x, Status: 0x%08x%s", KRED, ErrorCode, tRecvPkt.tHeader.ulState, KNRM);
		LOGERR(StatusMessage, 0, 0, 0);
		return ErrorCode; // this will avoid to continue executing the configuration if registering is not successful
	}
	else 
	{
		sprintf(StatusMessage, "\n%sHilscher Application Registered Successfully!%s", KGRN, KNRM);
		LOGDBG(StatusMessage, 0, 0, 0);
	}

// Send Optional Configuration Packets
	OS_Memset(&tSendPkt, 0, sizeof(tSendPkt));
	OS_Memset(&tRecvPkt, 0, sizeof(tRecvPkt));
		
    //USER_SetConfigParamService(&tSendPkt); // Set Configuration Parameters Service (Warmstart) function
    USER_GetWarmstartParameters(&tSendPkt);
    
//	ErrorCode= DEV_TransferPacket(HChannel, &tSendPkt, &tRecvPkt, sizeof(tRecvPkt), PUTPACKET_TIMEOUT, NULL, NULL);
	ErrorCode = xChannelPutPacket(HChannel, &tSendPkt, 10);
	ErrorCode = xChannelGetPacket(HChannel, sizeof(tRecvPkt), &tRecvPkt, 20);
	if ((CIFX_NO_ERROR != ErrorCode) || (SUCCESS_HIL_OK != tRecvPkt.tHeader.ulState))
	{
		sprintf(StatusMessage, "\n%sSetting Hilscher Configuration Parameters Failed! Error Code: 0x%08x, Status: 0x%08x%s", KRED, ErrorCode, tRecvPkt.tHeader.ulState, KNRM);
		LOGERR(StatusMessage, 0, 0, 0);
	// ToDo: Rise an alarm to alert configuration error
		return ErrorCode; // this will avoid to continue executing the configuration if registering is not successful
	}
	else 
	{
		sprintf(StatusMessage, "\n%sHilscher Configuration Parameters Set Successfully!%s", KGRN, KNRM);
		LOGDBG(StatusMessage, 0, 0, 0);
	}
//<! Channel Init
	OS_Memset(&tSendPkt, 0, sizeof(tSendPkt));
	OS_Memset(&tRecvPkt, 0, sizeof(tRecvPkt));

	tSendPkt.tHeader.ulDest  = HOST_TO_LE32(HIL_PACKET_DEST_DEFAULT_CHANNEL);
	tSendPkt.tHeader.ulSrc   = HOST_TO_LE32(0);
	tSendPkt.tHeader.ulLen   = HOST_TO_LE32(0);
	tSendPkt.tHeader.ulCmd   = HOST_TO_LE32(HIL_CHANNEL_INIT_REQ);
	tSendPkt.tHeader.ulState = HOST_TO_LE32(SUCCESS_HIL_OK);
	
	ErrorCode = DEV_TransferPacket(HChannel, &tSendPkt, &tRecvPkt, sizeof(tRecvPkt), PUTPACKET_TIMEOUT, NULL, NULL);
	
	if ((CIFX_NO_ERROR == ErrorCode) && (SUCCESS_HIL_OK == tRecvPkt.tHeader.ulState))
	{
		sprintf(StatusMessage, "\n%sHilscher Channel Initialized Successfully!%s", KGRN, KNRM);
		LOGDBG(StatusMessage, 0, 0, 0);
	}
	else
	{
		sprintf(StatusMessage, "\n%sError transferring channel init request packet! Error Code: 0x%08x, Status: 0x%08x%s", KRED, ErrorCode, tRecvPkt.tHeader.ulState, KNRM);
		LOGERR(StatusMessage, 0, 0, 0);
		return ErrorCode;	//<! To avoid executing the configuration if channel init is not successful
	}
	
// Protocol Specific Init
	ErrorCode= FbPtr ->ProtocolSpecificInit(FieldBus::HChannel);
	if (CIFX_NO_ERROR != ErrorCode)
	{
		sprintf(StatusMessage, "\n%sInitializing Hilscher Protocol Specific Failed! Error Code: 0x%08x, Status: 0x%08x%s", KRED, ErrorCode, tRecvPkt.tHeader.ulState, KNRM);
		LOGERR(StatusMessage, 0, 0, 0);
		return ErrorCode; // this will avoid to continue executing the configuration if registering is not successful
	}
	else 
	{
		sprintf(StatusMessage, "\n%sHilscher Protocol Specific Initialized Successfully!%s", KGRN, KNRM);
		LOGDBG(StatusMessage, 0, 0, 0);
	}
	
// Start Communication
	OS_Memset(&tSendPkt, 0, sizeof(tSendPkt));
	OS_Memset(&tRecvPkt, 0, sizeof(tRecvPkt));
		  
	ptrSendPkt->tHead.ulDest = HOST_TO_LE32(HIL_PACKET_DEST_DEFAULT_CHANNEL);
	ptrSendPkt->tHead.ulLen = HOST_TO_LE32(sizeof(HIL_START_STOP_COMM_REQ_DATA_T));
	ptrSendPkt->tHead.ulCmd = HOST_TO_LE32(HIL_START_STOP_COMM_REQ);
	ptrSendPkt->tData.ulParam = HOST_TO_LE32(HIL_START_STOP_COMM_PARAM_START);

	ErrorCode= DEV_TransferPacket(HChannel, &tSendPkt, &tRecvPkt, sizeof(tRecvPkt), PUTPACKET_TIMEOUT, NULL, NULL);
	if ((CIFX_NO_ERROR != ErrorCode) || (SUCCESS_HIL_OK != tRecvPkt.tHeader.ulState))
	{
		sprintf(StatusMessage, "\n%sStarting Hilscher Communication Failed! Error Code: 0x%08x, Status: 0x%08x%s", KRED, ErrorCode, tRecvPkt.tHeader.ulState, KNRM);
		LOGERR(StatusMessage, 0, 0, 0);
		return ErrorCode; // this will avoid to continue executing the configuration if registering is not successful
	}
	else 
	{
		sprintf(StatusMessage, "\n%sHilscher Communication Started Successfully!%s", KGRN, KNRM);
		LOGDBG(StatusMessage, 0, 0, 0);
	}

	sprintf(StatusMessage, "\n%sConfiguration Packets for Hilscher Module Successfully Sent..\n%s", KCYN, KNRM);
	LOGDBG(StatusMessage, 0, 0, 0);
	return ErrorCode;
		
}

/**************************************************************************//**
* 
* \brief   - RegisterApp
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void FieldBus::RegisterApp()
{
	
}

/**************************************************************************//**
* 
* \brief   - ReadCommonStatus
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void FieldBus::ReadCommonStatus()
{
	
}

/**************************************************************************//**
* 
* \brief   - ProtocolSpecificInit
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void FieldBus::ProtocolSpecificInit()
{
	
}

/**************************************************************************//**
* 
* \brief   - GetFaultData
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void FieldBus::GetFaultData()
{
	
}

