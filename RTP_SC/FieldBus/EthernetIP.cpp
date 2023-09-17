/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2012
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 It contains the EthernetIP class related implementation   
 
**********************************************************************************************************/

#include <iostream>

#include  "SC.h"
#include "EthernetIP.h"
#include "cifXErrors.h"
#include "eip_eif_packetdefinitions_packets.h"
#include "eip_eif_packetdefinitions_commands.h"
#include "cifXEndianess.h"
#include "cifXHWFunctions.h"
#include "common.h"
#include "SocketReceiver.h"
#include "Common.h"
#include "Hil_Results.h"
#include "Hil_ApplicationCmd.h"
#include "VendorSpecificInfo.h"

EthernetIP* EthernetIP::m_objEthIP = NULL;

/**************************************************************************//**
* 
* \brief   - Constructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
EthernetIP::EthernetIP()
{
	ConnectionState = 0;
	ConnectionType  = 0;
	ImplicitOutputImage.StatusWord1 = 0;
	ImplicitOutputImage.StatusWord2 = 0;
	ImplicitOutputImage.StatusWord3 = 0;
	
	ImplicitInputImage.ControlWord1 = 0;
	ImplicitInputImage.ControlWord2 = 0;
	ImplicitInputImage.ControlWord3 = 0;
			
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
EthernetIP::~EthernetIP()
{

}

/**************************************************************************//**
* \brief  - Creates the new instance of the object if not present else returns 
* 		    the object if already present (Singleton).
* 
* \param  - None
*
* \return - EthernetIP Object Pointer
* 
******************************************************************************/

EthernetIP* EthernetIP::Getinstance()
{
	if(m_objEthIP == NULL)
	{
		m_objEthIP = new (std::nothrow) EthernetIP;
	}

	return m_objEthIP;
}

/**************************************************************************//**
* 
* \brief   - GetWarmStartParams
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
bool EthernetIP::GetWarmStartParams()
{
	bool bResult = false;
	return bResult;
}

/**************************************************************************//**
* 
* \brief   - ReadExtendedStatus
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EthernetIP::ReadExtendedStatus()
{
	
}

/****************************************************************************************************
* 
* \brief   - Registers all the GSX Vendor Specific Objects
*
* \param   - CIFXHANDLE HChannel. Handle to Hilscher DPM communication channel.
*
* \return  - RetVal. The status for registering all the vendor specific object.
*
****************************************************************************************************/
INT32 EthernetIP::GSXE2RegisterVendorSpecificObjects(CIFXHANDLE HChannel)
{
	INT32 RetVal= CIFX_GENERAL_ERROR,
			VendorObject;

	char StatusMessage[STATUS_MESSAGES_LENGTH]= "";

	// Register all vendor specific objects
	for (INT32 VendorObject= WELD_RECIPE_OBJECT; VendorObject < ETHIP_VENDOR_OBJECT_LAST_USED; VendorObject++)
	{	
	
		RetVal= RegisterVendorSpecificClass(HChannel, VendorObject);
		
		if (CIFX_NO_ERROR != RetVal)
		{	
			sprintf(StatusMessage, "%sRegistering of Vendor Specific Object suspended at Object #%08X\n%s", KYEL, VendorObject, KNRM);
			LOGERR (StatusMessage, 0, 0, 0);
			break;		
		}
		else {
			sprintf(StatusMessage, "\nRegistering of Vendor Specific Object #%08X\n", VendorObject);
			LOGDBG(StatusMessage, 0, 0, 0);
		}
	}

	return RetVal;
}

/****************************************************************************************************
* 
* \brief   - Initializes protocol specific parameters
*            Such as configuration capability, vendor specific objects, MSNS
*
* \param   - CIFXHANDLE HChannel. Handle to Hilscher DPM communication channel.
*
* \return  - RetVal. The status for registering all the vendor specific object.
*
****************************************************************************************************/
INT32 EthernetIP::ProtocolSpecificInit(CIFXHANDLE HChannel)
{
	INT32 RetVal= CIFX_GENERAL_ERROR;
	ImplicitOutputImage.StatusWord3 = 0;
	ImplicitOutputImage.StatusWord2 = 0;
	ImplicitOutputImage.StatusWord1 = 0;
	// Register all vendor specific objects
	RetVal= GSXE2RegisterVendorSpecificObjects(HChannel);

	return RetVal;
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
void EthernetIP::GetFaultData()
{
	
}

/**************************************************************************//**
* 
* \brief   - If the Hilscher EthernetIP slave card is detected at power up
* 				this function is called by IOImage_Handler_Task() every 1ms
* 				to update the input structure from the buffer data.
*
* \param   - UINT8* InputPtr - Pointer to the cyclic data buffer received 
* 								from EthernetIP master.
*
* \return  - INT32 lRetErr - Contains the error code.
*
******************************************************************************/
INT32 EthernetIP::UpdateInputCyclic(UINT8* InputPtr)
{
	UINT16 localBuffer[FB_INPUT_BYTES], RecipeInputBuffer;
	INT32 lRetErr = CIFX_GENERAL_ERROR;
	INT32 recipenum=0;
	memset(localBuffer, 0, sizeof(localBuffer));
	
	if(NULL != InputPtr){
		
		memcpy(localBuffer, InputPtr, sizeof(localBuffer));

		ImplicitInputImage.ControlWord1 = LE16_CONVERT(localBuffer[0]);
		ImplicitInputImage.ControlWord2 = LE16_CONVERT(localBuffer[1]);
		ImplicitInputImage.ControlWord3 = LE16_CONVERT(localBuffer[2]);

		//Weld Recipe bits are in Control Word 3
		RecipeInputBuffer = ImplicitInputImage.ControlWord3;
		
		//Remove unwanted bits
		RecipeInputBuffer &= ~BIT_MASK(CW3_RES1);
		recipenum = RecipeInputBuffer >> WELD_RECIPE_BIT_NUM_OFFSET;
		

		lRetErr = CIFX_NO_ERROR;
	}
	else{
		lRetErr = CIFX_INVALID_POINTER;
	}
	
	return lRetErr;
}

/**************************************************************************//**
* 
* \brief   - If the Hilscher EthernetIP slave card is detected at power up
* 				this function is called by IOImage_Handler_Task() every 1ms 
* 				to update the buffer data from the output structure.
*
* \param   - UINT8* OutputPtr - Pointer to the cyclic data buffer to be 
* 									sent to EthernetIP master.
*
* \return  - INT32 lRetErr - Contains the error code.
*
******************************************************************************/
INT32 EthernetIP::UpdateOutputCyclic(UINT8* OutputPtr)
{
	UINT16 localBuffer[FB_OUTPUT_BYTES], curByte = 0, inputIdx = 0;
	INT32 lRetErr = CIFX_GENERAL_ERROR;
	
	memset(localBuffer , 0 , sizeof(localBuffer));

   if(NULL != OutputPtr){
	   

	   localBuffer[0] = ImplicitOutputImage.StatusWord1;
	   localBuffer[1] = ImplicitOutputImage.StatusWord2;
	   localBuffer[2] = ImplicitOutputImage.StatusWord3;
	   memcpy(OutputPtr, localBuffer, sizeof(localBuffer));
	   //Get Active recipe number
	   
	   //Get Active user
	   
	   //Get Active alarm ID
	   
	   //Get HMI mode (weld/setup)
	   
	   //Get Graph data (11 graph values)

	   
	   //memcpy the non single bit parameters
	   
	   lRetErr = CIFX_NO_ERROR;
   }
   else{
	   lRetErr = CIFX_INVALID_POINTER;
   }
   
   return lRetErr;
}

/****************************************************************************
* 
* \brief   - ProcessMailBox
*    Processes the explicit received packets containing either indication (IND) or confirmation (CNF) messages.
*    This function is called by UpdateMailBox() function when a packet is received from Hilscher module.
*
* \param   - CIFX_PACKET * MBXPkt.
* 		Is the packet that contains the indication or confirmation messages
*
* \return  - None.
*
******************************************************************************/
bool EthernetIP::ProcessMailBox(CIFX_PACKET* MBXPkt)
{
	INT32 lRetErr = CIFX_GENERAL_ERROR;
	UINT32 RcvPkt = 0;
	CIFX_PACKET* ResPkt = new CIFX_PACKET;
	UINT32  SizeOfData=0;
	memset((void*) ResPkt, 0, sizeof(*ResPkt));
	
	/*Check if we have an IND or CNF command*/
	
	if (0 == (LE32_TO_HOST(MBXPkt->tHeader.ulCmd) & CIFX_MSK_PACKET_ANSWER)) // Check if we have an IND or CNF packet
	{		
		/*This is an IND command*/
		LOGDBG("Received IND Command: %08X code %08X", LE32_TO_HOST(MBXPkt->tHeader.ulCmd), LE32_TO_HOST(MBXPkt->tHeader.ulState), 0);
		
		switch (LE32_TO_HOST(MBXPkt->tHeader.ulCmd))
		{
			case HIL_LINK_STATUS_CHANGE_IND:
				
				LOGDBG("\nEIP OBJECT LINK STATUS CHANGE", 0, 0, 0);
				
				/*IND and RSP packet structure instance*/
				HIL_LINK_STATUS_CHANGE_IND_T* LinkInd = (HIL_LINK_STATUS_CHANGE_IND_T*) MBXPkt;
				HIL_LINK_STATUS_CHANGE_RES_T* LinkRes = (HIL_LINK_STATUS_CHANGE_RES_T*) ResPkt;
				
				/*IND Packet processing*/
				//No processing for this command
				
				/*RES Packet build*/
				LinkRes->tHead.ulDest = LinkInd->tHead.ulDest;
				LinkRes->tHead.ulSrc = LinkInd->tHead.ulSrc;
				LinkRes->tHead.ulId = LinkInd->tHead.ulId;
				LinkRes->tHead.ulCmd = HOST_TO_LE32(HIL_LINK_STATUS_CHANGE_RES);
				LinkRes->tHead.ulLen = HOST_TO_LE32(0);
				LinkRes->tHead.ulSta = HOST_TO_LE32(SUCCESS_HIL_OK);
				
				break; 

			case EIP_OBJECT_CONNECTION_IND:
				
				LOGDBG("\nEIP OBJECT CONNECTION CHANGE", 0, 0, 0);
				
				/*IND and RSP packet structure instance*/
				EIP_OBJECT_PACKET_CONNECTION_IND_T* ConnInd = (EIP_OBJECT_PACKET_CONNECTION_IND_T*) MBXPkt;
				EIP_OBJECT_PACKET_CONNECTION_RES_T* ConnRes = (EIP_OBJECT_PACKET_CONNECTION_RES_T*) ResPkt;
				
				/*IND Packet processing*/
				
				//<! Check the connection (Implicit/Explicit connection) state and type.
				ConnectionType  = ConnInd->tData.bConnType;
				ConnectionState = ConnInd->tData.ulConnectionState;
				
				/*RES Packet build*/
				ConnRes->tHead.ulDest = ConnInd->tHead.ulDest;
				ConnRes->tHead.ulSrc = ConnInd->tHead.ulSrc;
				ConnRes->tHead.ulId = ConnInd->tHead.ulId;
				ConnRes->tHead.ulCmd = HOST_TO_LE32(EIP_OBJECT_CONNECTION_RES);
				ConnRes->tHead.ulLen = HOST_TO_LE32(0);
				ConnRes->tHead.ulSta = HOST_TO_LE32(SUCCESS_HIL_OK);
				
				break;

			case EIP_OBJECT_CL3_SERVICE_IND:
				
				LOGDBG("EIP OBJECT CL3 SERVICE IND Command", 0, 0, 0);

				/*IND and RSP packet structure instance*/
				EIP_OBJECT_PACKET_CL3_SERVICE_IND_T* CL3Ind = (EIP_OBJECT_PACKET_CL3_SERVICE_IND_T*) MBXPkt;
				EIP_OBJECT_PACKET_CL3_SERVICE_RES_T* CL3Res = (EIP_OBJECT_PACKET_CL3_SERVICE_RES_T*) ResPkt;
				EthIPStausCode CL3_IND_Status;
				char data[]= {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
				
				//Vendor Specific Object 			
				LOGDBG("\n  - Class: %08X \n  - Instance: %08X  \n  - Attribute: %08X", CL3Ind->tData.ulObject, CL3Ind->tData.ulInstance, CL3Ind->tData.ulAttribute);
				
				/*IND Packet processing*/
				//Calling the function that checks instance and attribute limits and access type (read, write)
				VendorSpecificInfo * VSInfo = VendorSpecificInfo::Getinstance();
				
				CL3_IND_Status = VSInfo->ProcessRequest(CL3Ind->tData.bService, CL3Ind->tData.ulInstance, CL3Ind->tData.ulObject,CL3Ind->tData.abData, &SizeOfData, CL3Ind->tData.ulAttribute);
				
				/*RES Packet build*/
				CL3Res->tHead.ulDest = CL3Ind->tHead.ulDest;
				CL3Res->tHead.ulSrc = CL3Ind->tHead.ulSrc;
				CL3Res->tHead.ulId = CL3Ind->tHead.ulId;
				CL3Res->tHead.ulLen= HOST_TO_LE32(EIP_OBJECT_CL3_SERVICE_RES_SIZE+SizeOfData);
				CL3Res->tHead.ulSta = HOST_TO_LE32(SUCCESS_HIL_OK);
				CL3Res->tHead.ulCmd = HOST_TO_LE32(EIP_OBJECT_CL3_SERVICE_RES);
				
				CL3Res->tData.ulConnectionId = CL3Ind->tData.ulConnectionId;
				CL3Res->tData.bService = CL3Ind->tData.bService;
				CL3Res->tData.ulObject = CL3Ind->tData.ulObject;
				CL3Res->tData.ulInstance = CL3Ind->tData.ulInstance;
				CL3Res->tData.ulAttribute = CL3Ind->tData.ulAttribute;
				CL3Res->tData.ulGRC= HOST_TO_LE32(CL3_IND_Status.GSC);
				CL3Res->tData.ulERC= HOST_TO_LE32(CL3_IND_Status.ESC);

				memcpy(CL3Res->tData.abData, CL3Ind->tData.abData,SizeOfData);
				
				break;
				
			case EIP_OBJECT_CIP_OBJECT_CHANGE_IND:
					
				LOGDBG("\nEIP OBJECT CIP OBJECT CHANGE INDICATION",0,0,0);
					
				//<! IND and RES packet structure instance.
				EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_IND_T* PktInd = (EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_IND_T*) MBXPkt;
				EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_RES_T* PktRes = (EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_RES_T*) ResPkt;
					
				if((LE32_TO_HOST(PktInd->tData.ulClass)    == EIP_TCP_CLASS_NUMBER) && 
				   (LE32_TO_HOST(PktInd->tData.ulInstance) == EIP_TCP_MAX_INSTANCE))
				{
					if((LE32_TO_HOST(PktInd->tData.bService)   == SET_ATTRIBUTE_SINGLE) && 
					   (LE32_TO_HOST(PktInd->tData.ulAttribute) == EIP_TCP_ATTR_3_CFG_CONTROL))
					{
						//<! ToDo: Attribute to configure the network configuration STATIC, BOOTP or DHCP client. 
					}
					if((LE32_TO_HOST(PktInd->tData.bService)   == SET_ATTRIBUTE_SINGLE) && 
					   (LE32_TO_HOST(PktInd->tData.ulAttribute) == EIP_TCP_ATTR_5_INTERFACE_CFG))
					{
						//<! ToDo: Attribute to configure the IP, Netmask and gateway through master.
//						TCPIPInterfaceConfig = (EIP_TCP_INTERFACE_CONFIG_T*) PktInd->tData.abData;
//						EthipData->tInterfaceConfig.ulIpAddr      = LE32_TO_HOST(TCPIPInterfaceConfig->ulIpAddr);
//						EthipData->tInterfaceConfig.ulSubnetMask  = LE32_TO_HOST(TCPIPInterfaceConfig->ulSubnetMask);
//						EthipData->tInterfaceConfig.ulGatewayAddr = LE32_TO_HOST(TCPIPInterfaceConfig->ulGatewayAddr);
					}
					if((LE32_TO_HOST(PktInd->tData.bService)   == SET_ATTRIBUTE_SINGLE) && 
					   (LE32_TO_HOST(PktInd->tData.ulAttribute) == EIP_TCP_ATTR_11_LAST_CONFLICT_DETECTED))
					{
						AcdInfo = (EIP_TCP_ACD_LAST_CONFLICT_T*) PktInd->tData.abData;
						EthipData->tLastConflictDetected = *AcdInfo;
						
						memset(AcdInfoBuffer, 0, sizeof(AcdInfoBuffer));
						
						memcpy(AcdInfoBuffer, &AcdInfo, sizeof(AcdInfoBuffer));
					}
				}
				
				//<! RES Packet build
				PktRes->tHead.ulDest = PktInd->tHead.ulDest;
				PktRes->tHead.ulSrc  = PktInd->tHead.ulSrc;
				PktRes->tHead.ulId   = PktInd->tHead.ulId;
				PktRes->tHead.ulLen  = HOST_TO_LE32(0);
				PktRes->tHead.ulCmd  = HOST_TO_LE32(HOST_TO_LE32(MBXPkt->tHeader.ulCmd) | CIFX_MSK_PACKET_ANSWER);
				PktRes->tHead.ulSta  = HOST_TO_LE32(SUCCESS_HIL_OK);
				
				break;
					
				//ToDo: Add all the command ID's to be processed
					
			default:
				
				LOGERR("Unexpected Indication IND Command: %08X. Status Code: %08X", LE32_TO_HOST(MBXPkt->tHeader.ulCmd), LE32_TO_HOST(MBXPkt->tHeader.ulState), 0);
				
				ResPkt->tHeader.ulDest = MBXPkt->tHeader.ulDest;
				ResPkt->tHeader.ulSrc = MBXPkt->tHeader.ulSrc;
				ResPkt->tHeader.ulId = MBXPkt->tHeader.ulId;
				ResPkt->tHeader.ulLen = HOST_TO_LE32(sizeof(ResPkt));
				ResPkt->tHeader.ulState = HOST_TO_LE32(ERR_HIL_UNEXPECTED);
				ResPkt->tHeader.ulCmd = HOST_TO_LE32(HOST_TO_LE32(MBXPkt->tHeader.ulCmd) | CIFX_MSK_PACKET_ANSWER);
				
				return false;
				
		}
		
		//<! Fill the mailbox packet buffer with the expected RES packet.
		RcvPkt = rngBufPut(CIFXPKT_BUFFER_ID, (char *)ResPkt, sizeof(*ResPkt));
		
		if(RcvPkt <= 0)    //<! Check if the received packets is less than zero.
		{
			LOGERR("Error sending Response to Queue!\r\n", 0, 0, 0);
		}		
	}
	else 
	{	
		
		// This is a command confirmation CNF packet
		LOGDBG("Received Confirmation CNF Command: %08X / Status Code: %08X\n", LE32_TO_HOST(MBXPkt->tHeader.ulCmd), LE32_TO_HOST(MBXPkt->tHeader.ulState), 0);
		
		switch(LE32_TO_HOST(MBXPkt->tHeader.ulCmd))
		{
		
		//ToDo: Add all the command ID's to be processed
		
		default:
			
			LOGERR("\n Unexpected CNF Command: %08X with err code %08X", LE32_TO_HOST(MBXPkt->tHeader.ulCmd), LE32_TO_HOST(MBXPkt->tHeader.ulState), 0);
			
			return false;
		}
	}
	
	delete ResPkt;
	
	return true;
}

/**************************************************************************//**
* 
* \brief   - RegisterForMSNSChange
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EthernetIP::RegisterForMSNSChange()
{

}

/**************************************************************************//**
* 
* \brief   - RegisterAssembly
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EthernetIP::RegisterAssembly()
{
	
}

/**************************************************************************//**
* 
* \brief   - RegisterService
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EthernetIP::RegisterService()
{
	
}

/****************************************************************************************************
* 
* \brief   - Registers vendor specific objects. 
*
* \params	- CIFXHANDLE HChannel. Handle to Hilscher DPM communication channel.
* 			- UINT32 VendorObjClass. 
* 			  Is the ID for vendor specific object. Valid range 0x64 - 0xC7 & 0x300 - 0x4FF
*
* \return  - RetVal. The status for registering each vendor specific object
*
****************************************************************************************************/
INT32 EthernetIP::RegisterVendorSpecificClass(CIFXHANDLE HChannel, UINT32 VendorObjClass)
{
	EIP_OBJECT_MR_PACKET_REGISTER_REQ_T ptSendPkt;
	EIP_OBJECT_MR_PACKET_REGISTER_CNF_T ptRecvPkt;	
	INT32 RetVal= CIFX_GENERAL_ERROR,
		iRetries = 0;
	char StatusMessage[STATUS_MESSAGES_LENGTH]= "";

	ptSendPkt.tHead.ulDest = HOST_TO_LE32(ETHIP_PROTOCOL_STACK_DESTINATION);// Destination for Protocol Stack: 0x20
	ptSendPkt.tHead.ulLen = HOST_TO_LE32(EIP_OBJECT_MR_REGISTER_REQ_SIZE); // Packet data length in bytes
	ptSendPkt.tHead.ulSta = HOST_TO_LE32(SUCCESS_HIL_OK);
	ptSendPkt.tHead.ulCmd = HOST_TO_LE32(EIP_OBJECT_MR_REGISTER_REQ);// Command for Register Vendor Specific Class: 0x1A02 
	ptSendPkt.tData.ulReserved1 = HOST_TO_LE32(0);// Reserved, set to 0
	ptSendPkt.tData.ulClass = HOST_TO_LE32(VendorObjClass);// uniquely identifies the object class. Range 0x0064 - 0x00C7 & 0x300 - 0x4FF
	ptSendPkt.tData.ulOptionFlags = HOST_TO_LE32(EIP_OBJECT_MR_REGISTER_OPTION_FLAGS_USE_OBJECT_PROVIDED_BY_STACK & ~BIT_MASK(0));// For CIP objects not present in protocol stack set bit 0 to 0

/*	if(OnReset)
	{
		memcpy(&TempPkt, &Pkt, sizeof(Pkt));
		FieldbusTask::thisPtr->CifxPkt.Write(0, TempPkt, 0);
	}
	else
	{*/ 
		for (iRetries= 0; iRetries < MAX_FB_CONIFG_RETRIES; iRetries++)
		{
			RetVal = DEV_TransferPacket(HChannel, (CIFX_PACKET*) &ptSendPkt, (CIFX_PACKET*) &ptRecvPkt, sizeof(EIP_OBJECT_MR_PACKET_REGISTER_CNF_T), ETHIP_VENDORSPECIFIC_GSX_TIMEOUT, NULL, NULL);
			if (CIFX_NO_ERROR == RetVal && SUCCESS_HIL_OK == ptRecvPkt.tHead.ulSta)
				break;
			else
				MsDelay(ETHIP_VENDORSPECIFIC_GSX_RETRY_TIME);		
		}
	// Log results
		if (CIFX_NO_ERROR == RetVal && SUCCESS_HIL_OK == ptRecvPkt.tHead.ulSta) // Succeeded on both transmitting/receiving the command/response and registering the object
		{
			sprintf(StatusMessage, "%sVendor Specific Object #%08X Successfully Registered\n%s", KGRN, VendorObjClass, KNRM);
		}
		else  // Fail occurs when one of the conditions is false
		{	
			if (CIFX_NO_ERROR == RetVal && SUCCESS_HIL_OK != ptRecvPkt.tHead.ulSta) // failed registering the object
				sprintf(StatusMessage, "%sVendor Specific Object #%08X Not Registered. Failed Registering Object. Error: %08X\n%s", KRED, VendorObjClass, ptRecvPkt.tHead.ulSta, KNRM);
			else  // failed on either transmitting the command or receiving the response 
				sprintf(StatusMessage, "%sVendor Specific Object #%08X Not Registered. Failed Transmitting the Registering Command. Error %08X\n%s", KRED, VendorObjClass, RetVal, KNRM);
		}
		LOGDBG(StatusMessage, 0, 0, 0);
		if (iRetries > 0)
		{	
			sprintf(StatusMessage, "%sNumber of unsuccessful tries: %d\n%s", KYEL, iRetries, KNRM);
			LOGDBG(StatusMessage, 0, 0, 0);
		}

//	}
	return RetVal;
}

/**************************************************************************//**
* 
* \brief   - HandleResetServiceIndication
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EthernetIP::HandleResetServiceIndication()
{
	
}

/**************************************************************************//**
* 
* \brief   - CheckConnIndicationDetails
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EthernetIP::CheckConnIndicationDetails()
{
	
}

/**************************************************************************//**
* 
* \brief   - GetIOSignalState
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EthernetIP::GetIOSignalState()
{
	
}

/**************************************************************************//**
* 
* \brief   - SetIOSignalState
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EthernetIP::SetIOSignalState()
{
	
}

/**************************************************************************//**
* 
* \brief   - SetStaticIPAddr - To set the attribute data to slave card.
* 			 This function calls EIP Set Attribute single service.
* 
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EthernetIP::SetStaticIPAddr()
{
	INT32 lRet = 0;
	
	/* ToDo: The values for setting IP, subnet and gateway are hard coded, need to set as per user defined from HMI */
	Config.ulIpAddr        = IP_ADDRESS(192,168,0,110);
	Config.ulSubnetMask    = IP_ADDRESS(255,255,255,0);
	Config.ulGatewayAddr   = IP_ADDRESS(0,0,0,0);
	Config.ulNameServer    = IP_ADDRESS(0,0,0,0);
	Config.ulNameServer_2  = IP_ADDRESS(0,0,0,0);
	Config.abDomainName[0] = '\0';
	
	lRet = SetAttribute(TCPIP_INTERFACEOBJECT_CLASS, TCPIP_INTERFACEOBJECT_INSTANCE, TCPIP_INTERFACEOBJECT_INTERFACECONFIG_ATTRIBUTE, Config, HChannel);
	if(CIFX_NO_ERROR != lRet)
	{
		LOGERR("The Set Attribute service is unsuccessful\r\n", 0, 0, 0);
	}
	else
	{
		LOGDBG("The Set Attribute service is successful\r\n", 0, 0, 0);
	}
}

/**************************************************************************//**
* 
* \brief   - SetAttribute - To set the attribute data to slave card using EIP Set Attribute single service.
* 			 This function only sends the command to slave card.
* 
* \param   - ulClass- The preknown class for which set attribute service needs to be send
* 			 ulInstance- The instance of class
* 			 ulAttribute- The attribute of class
* 			 value- struct value to be set to the attribute
*	  		 HChannel- Hilscher's DPM Communication channel
*
* \return  - RetVal- The status to know if the Set attribute has executed successfully.
*
******************************************************************************/
INT32 EthernetIP::SetAttribute(UINT32 ulClass, UINT32 ulInstance, UINT32 ulAttribute, EIP_TCP_INTERFACE_CONFIG_Ttag &value, CIFXHANDLE HChannel)
{
	CIFX_PACKET TempPkt = {{0}};
	INT32 RetVal = CIFX_GENERAL_ERROR;
	PCHANNELINSTANCE PtChannel = (PCHANNELINSTANCE) HChannel;
	EIP_OBJECT_PACKET_CIP_SERVICE_CNF_T CnfReg;
	
	EIP_OBJECT_PACKET_CIP_SERVICE_REQ_T * ServicReqFromHost = (EIP_OBJECT_PACKET_CIP_SERVICE_REQ_T *)&TempPkt;

	ServicReqFromHost->tHead.ulDest   = HOST_TO_LE32(HIL_PACKET_DEST_DEFAULT_CHANNEL);
	ServicReqFromHost->tHead.ulSrc    = HOST_TO_LE32(0);
	ServicReqFromHost->tHead.ulDestId = HOST_TO_LE32(0);
	ServicReqFromHost->tHead.ulSrcId  = HOST_TO_LE32(0);
	ServicReqFromHost->tHead.ulLen    = EIP_OBJECT_CIP_SERVICE_REQ_SIZE;
	ServicReqFromHost->tHead.ulId     = HOST_TO_LE32(0x00000000);
	ServicReqFromHost->tHead.ulSta    = HOST_TO_LE32(0);
	ServicReqFromHost->tHead.ulCmd    = HOST_TO_LE32(EIP_OBJECT_CIP_SERVICE_REQ);
	ServicReqFromHost->tHead.ulExt    = HOST_TO_LE32(0);
	ServicReqFromHost->tHead.ulRout   = HOST_TO_LE32(0);

	ServicReqFromHost->tData.ulMember    = HOST_TO_LE32(0);
	ServicReqFromHost->tData.ulClass     = HOST_TO_LE32(ulClass);
	ServicReqFromHost->tData.ulInstance  = HOST_TO_LE32(ulInstance);
	ServicReqFromHost->tData.ulAttribute = HOST_TO_LE32(ulAttribute);
	ServicReqFromHost->tData.bService    = HOST_TO_LE32(SET_ATTRIBUTE_SINGLE);

	memcpy(&ServicReqFromHost->tData.abData, &value, sizeof(value));

	ServicReqFromHost->tHead.ulLen += (sizeof(value));
	ServicReqFromHost->tHead.ulLen = HOST_TO_LE32(ServicReqFromHost->tHead.ulLen);
	
	RetVal = xChannelPutPacket(PtChannel, (CIFX_PACKET*) &TempPkt, 10);
	if (CIFX_NO_ERROR != RetVal)			 
	{
		LOGERR("Error sending packet to device. Error: %08x\r\n", TempPkt.tHeader.ulState, 0, 0);
	} 
	else 
	{
		LOGDBG("Send Packet to device successfully\r\n", 0, 0, 0);
		
		RetVal = xChannelGetPacket(PtChannel, sizeof(EIP_OBJECT_PACKET_CIP_SERVICE_CNF_Ttag),(CIFX_PACKET*) &CnfReg, 20);
		if (CIFX_NO_ERROR != RetVal)
		{
			LOGERR("Error getting packet from device. Error: %08x\r\n", CnfReg.tHead.ulSta, 0, 0);
		} 
		else
		{
			LOGDBG("Received Packet from device successfully\r\n", 0, 0, 0);
			
			if (LE32_TO_HOST(CnfReg.tHead.ulCmd) == (EIP_OBJECT_CIP_SERVICE_CNF)) 
			{
				if (0 == LE32_TO_HOST(CnfReg.tHead.ulSta)) 
				{
					LOGDBG("Received CNF reg OBJ. Cmd: %08X\r\n", LE32_TO_HOST(CnfReg.tHead.ulCmd), 0, 0);
				}
				else
				{
					LOGERR("Received CNF reg OBJ Failed. Cmd: %08X. Error: %08X, GRC: %08X\r\n", LE32_TO_HOST(CnfReg.tHead.ulCmd), LE32_TO_HOST(CnfReg.tHead.ulSta), LE32_TO_HOST(CnfReg.tData.ulGRC));
				}
			}
			else
			{
				LOGERR("Command was not expected. Error: %08X\r\n", LE32_TO_HOST(CnfReg.tHead.ulCmd), 0, 0);
			}
		}
	}

	return RetVal;
}

/**************************************************************************//**
* 
* \brief   - GetAttribute - To get the attribute data from slave card using EIP Get Attribute single service.
* 			 This function only sends the command to slave card.
* 
* \param   - ulClass- The preknown class for which get attribute service needs to be send
* 			 ulInstance- The instance of class
* 			 ulAttribute- The attribute of class
*
* \return  - None.
*
******************************************************************************/
void EthernetIP::GetAttribute(UINT32 ulClass, UINT32 ulInstance, UINT32 ulAttribute)
{
	CIFX_PACKET TempPkt = {{0}};
	UINT32 RetVal = 0;
	PCHANNELINSTANCE PtChannel = (PCHANNELINSTANCE) HChannel;
	EIP_OBJECT_PACKET_CIP_SERVICE_CNF_T CnfReg;
	
	EIP_OBJECT_PACKET_CIP_SERVICE_REQ_T * ServicReqFromHost = (EIP_OBJECT_PACKET_CIP_SERVICE_REQ_T *)&TempPkt;

	ServicReqFromHost->tHead.ulDest   = HOST_TO_LE32(HIL_PACKET_DEST_DEFAULT_CHANNEL);
	ServicReqFromHost->tHead.ulSrc    = HOST_TO_LE32(0);
	ServicReqFromHost->tHead.ulDestId = HOST_TO_LE32(0);
	ServicReqFromHost->tHead.ulSrcId  = HOST_TO_LE32(0);
	ServicReqFromHost->tHead.ulLen    = HOST_TO_LE32(EIP_OBJECT_CIP_SERVICE_REQ_SIZE);
	ServicReqFromHost->tHead.ulId     = HOST_TO_LE32(0x00000000);
	ServicReqFromHost->tHead.ulSta    = HOST_TO_LE32(0);
	ServicReqFromHost->tHead.ulCmd    = HOST_TO_LE32(EIP_OBJECT_CIP_SERVICE_REQ);
	ServicReqFromHost->tHead.ulExt    = HOST_TO_LE32(0);
	ServicReqFromHost->tHead.ulRout   = HOST_TO_LE32(0);

	ServicReqFromHost->tData.ulMember    = HOST_TO_LE32(0);
	ServicReqFromHost->tData.ulClass     = HOST_TO_LE32(ulClass);
	ServicReqFromHost->tData.ulInstance  = HOST_TO_LE32(ulInstance);
	ServicReqFromHost->tData.ulAttribute = HOST_TO_LE32(ulAttribute);
	ServicReqFromHost->tData.bService    = HOST_TO_LE32(GET_ATTRIBUTE_SINGLE);
	
	RetVal = xChannelPutPacket(PtChannel, (CIFX_PACKET*) &TempPkt, 10);
	if (CIFX_NO_ERROR != RetVal)
	{
		LOGERR("Error sending packet to device. Error: %08x\r\n", TempPkt.tHeader.ulState, 0, 0);
	} 
	else
	{
		LOGDBG("Send Packet to device successfully\r\n", 0, 0, 0);
		
		RetVal = xChannelGetPacket(PtChannel, sizeof(EIP_OBJECT_PACKET_CIP_SERVICE_CNF_Ttag),(CIFX_PACKET*) &CnfReg, 20);
		if (CIFX_NO_ERROR != RetVal)
		{
			LOGERR("Error getting packet from device. Error: %08x\r\n", CnfReg.tHead.ulSta, 0, 0);
		} 
		else
		{
			LOGDBG("Received Packet from device successfully\r\n", 0, 0, 0);
			
			if (LE32_TO_HOST(CnfReg.tHead.ulCmd) == (EIP_OBJECT_CIP_SERVICE_CNF)) 
			{
				if (0 == LE32_TO_HOST(CnfReg.tHead.ulSta)) 
				{
					LOGDBG("Received CNF reg OBJ. Cmd: %08X\r\n", LE32_TO_HOST(CnfReg.tHead.ulCmd), 0, 0);
					
					//<! ToDo: copy the CnfRed.tData.abData and return the data whenever GetAttribute is called.
				}
				else
				{
					LOGERR("Received CNF reg OBJ Failed. Cmd: %08X. Error: %08X, GRC: %08X\r\n", LE32_TO_HOST(CnfReg.tHead.ulCmd), LE32_TO_HOST(CnfReg.tHead.ulSta), LE32_TO_HOST(CnfReg.tData.ulGRC));
				}
			}
			else
			{
				LOGERR("Command was not expected. Error: %08X\r\n", LE32_TO_HOST(CnfReg.tHead.ulCmd), 0, 0);
			}
		}
	}
}

/**************************************************************************//**
* 
* \brief   - SendACDInformation - Send the ACD data when HMI command is received.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void EthernetIP::SendACDInformation()
{
	UINT32 iLen = 0;
	Client_Message respMsg;

	memset(respMsg.Buffer, 0x00, sizeof(respMsg.Buffer));

    if (AcdInfoBuffer != NULL)
	{
		memcpy(respMsg.Buffer, AcdInfoBuffer, sizeof(AcdInfoBuffer));
	}
	else 
	{
		//<! Send error message when error occurs
		sprintf(respMsg.Buffer, "%u", RESPONSE_FAIL);
	}

	//<! Send message to UIC
	respMsg.msgID  = UIC_EIP_ACD_INFO_RES;
	respMsg.msglen = strlen(respMsg.Buffer);
	iLen = sizeof(respMsg.msgID) + sizeof(respMsg.msglen) + respMsg.msglen;

	Interface = CommunicateInterface::getinstance();
	Interface->Send(reinterpret_cast<char*>(&respMsg), iLen);
}

/**************************************************************************//**
* 
* \brief   - GetConnectionState - Get the Connection State based on the IND command received.
*
* \param   - None.
*
* \return  - Numeric value '1' and '0' of Connection State.
*
******************************************************************************/
UINT32 EthernetIP::GetConnectionState()
{
	return ConnectionState;
}

/**************************************************************************//**
* \brief  - Update the IOImage structure for Implicit Messaging
*
* \param  - function, value
*
*
* \return  - None
*
******************************************************************************/
void EthernetIP::SetOutputToIOImage (DigitalIO::LINE_FUNCTION function, DigitalIO::LINE_VALUE value)
{
	

	switch (function)
	{
		case DigitalIO::OUTPUT_CYCLE_RUN:
		{
			if((UINT16)value == LINE_HIGH_EIP)
				ImplicitOutputImage.StatusWord3 |= BIT_MASK(CYRU);
			else
				ImplicitOutputImage.StatusWord3 &= ~BIT_MASK(CYRU);
				break;
		}
		case DigitalIO::OUTPUT_HORN_SEEK:
		{
			if((UINT16)value == LINE_HIGH_EIP)
				ImplicitOutputImage.StatusWord3 |= BIT_MASK(H_SEEK);
			else
				ImplicitOutputImage.StatusWord3 &= ~BIT_MASK(H_SEEK);
				break;
		}
		case DigitalIO::OUTPUT_WELD_ACTIVE:
		{
			if((UINT16)value == LINE_HIGH_EIP)
				ImplicitOutputImage.StatusWord3 |= BIT_MASK(WACT);
			else
				ImplicitOutputImage.StatusWord3 &= ~BIT_MASK(WACT);
				break;
		}
		case DigitalIO::OUTPUT_HOLD_ACTIVE:
		{
			if((UINT16)value == LINE_HIGH_EIP)
				ImplicitOutputImage.StatusWord3 |= BIT_MASK(HACT);
			else
				ImplicitOutputImage.StatusWord3 &= ~BIT_MASK(HACT);
				break;
		}
		case DigitalIO::OUTPUT_AC_HOME:
		{
			if((UINT16)value == LINE_HIGH_EIP)
				ImplicitOutputImage.StatusWord3 |= BIT_MASK(HPOS);
			else
				ImplicitOutputImage.StatusWord3 &= ~BIT_MASK(HPOS);
				break;
		}
		case DigitalIO::OUTPUT_AC_READY:
		{
			if((UINT16)value == LINE_HIGH_EIP)
				ImplicitOutputImage.StatusWord3 |= BIT_MASK(RPOS);
			else
				ImplicitOutputImage.StatusWord3 &= ~BIT_MASK(RPOS);
			break;
		}
					
		case DigitalIO::OUTPUT_PB_RELEASE:
		{
			if((UINT16)value == LINE_HIGH_EIP)
				ImplicitOutputImage.StatusWord3 |= BIT_MASK(PBR);
			else
				ImplicitOutputImage.StatusWord3 &= ~BIT_MASK(PBR);
				break;
		}
		
		case DigitalIO::OUTPUT_ULTRASONIC:
		{
			if((UINT16)value == LINE_HIGH_EIP)
				ImplicitOutputImage.StatusWord2 |= BIT_MASK(SONIC_ON);
			else
				ImplicitOutputImage.StatusWord2 &= ~BIT_MASK(SONIC_ON);
				break;
		}
		case DigitalIO::OUTPUT_CYCLE_OK:
		{
			if((UINT16)value == LINE_HIGH_EIP)
				ImplicitOutputImage.StatusWord2 |= BIT_MASK(WELD_OK);
			else
				ImplicitOutputImage.StatusWord2 &= ~BIT_MASK(WELD_OK);
				break;
		}
		case DigitalIO::OUTPUT_ALARM_GENERAL:
		{
			if((UINT16)value == LINE_HIGH_EIP)
				ImplicitOutputImage.StatusWord3 |= BIT_MASK(GR_AL);
			else
				ImplicitOutputImage.StatusWord3 &= ~BIT_MASK(GR_AL);
				break;
		}
		case DigitalIO::OUTPUT_SYSTEM_READY:
		{
			if((UINT16)value == LINE_HIGH_EIP)
				 ImplicitOutputImage.StatusWord2 |= BIT_MASK(READY_ST);
			else
				ImplicitOutputImage.StatusWord2 &= ~BIT_MASK(READY_ST);
				break;
		}
		default:
		{
			break;
		}
	}
	


}


/**************************************************************************//**
* \brief  - Update the IOImage structure for Implicit Messaging(for outputs not 
* present in DigitalIO structure)
*
* \param  - function, value
*
*
* \return  - None
*
******************************************************************************/
void EthernetIP::SetOutputToSWIOImage (INT32 StatusWordNum, INT32 function, INT32 value)
{
	switch (StatusWordNum)
		{
			case SW1:
			{
				if(value == LINE_HIGH_EIP)
					ImplicitOutputImage.StatusWord1 |= BIT_MASK(function);
				else
					ImplicitOutputImage.StatusWord1 &= ~BIT_MASK(function);
				break;
				
				
			}
		}
}
