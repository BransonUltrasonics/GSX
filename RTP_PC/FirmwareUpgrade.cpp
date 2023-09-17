/*
 * FirmwareUpgrade.cpp
 *
 *  Created on: Mar 28, 2018
 *      Author: rpsharma
 */


#include <vxWorkscommon.h>
#include "FirmwareUpgrade.h"
#include "DynamicSystemCall.h"
#include "PCObjDictionary.h"
#include "Eeprom.h"
extern "C"
{
	#include <customSystemCall.h>
}

/**************************************************************************//**
 * \brief   - Constructor 
 *
 * \param   - None
 *            
 * \return  - None
 *
 ******************************************************************************/
FirmwareUpgrade::FirmwareUpgrade()
{
	
	
}

/**************************************************************************//**
 * \brief   - Get OD data properties like Offset and Object Size
 * 			 
 * \param   - None
 *            
 * \return  - Return 0 on success -1 on failure
 *
 ******************************************************************************/

INT16 FirmwareUpgrade:: GetFWInfoSizesFromOD()
{
	UINT16 subIndex, arrIndex, nextArrIndex;

	pObjEntryFWUpgrade = obj_getobjecthandleSc( FIRMWARE_UPGRADE_INDEX );
	
	if (pObjEntryFWUpgrade == NULL)
	{
		#ifdef ERROR_PRINT
			LOG("\npObjEntryFWUpgrade is NULL!!!\n");					
		#endif	
		return ERROR;
	}
	
	/* Offset calculation of 8 weld result parameters for OBJ Write operation*/
	for (UINT16 subIndex = 1, arrIndex = 0; subIndex <= NUM_OF_FWUPGRD_INFO_ENTRY;
			subIndex++, arrIndex++)	
	{
		/*Divide by 8 to convert bits to bytes*/
		FWUPGRDSizes[arrIndex] = (obj_getentryoffsetSc(subIndex, pObjEntryFWUpgrade) / 8);		
		
	}
	ObjSizeFWUPGRDInfo = obj_getobjectlengthSc(FIRMWARE_UPGRADE_INDEX, SUB_INDEX, 
						pObjEntryFWUpgrade, COMPLETE_ACCESS);
	if (ObjSizeFWUPGRDInfo <= 0)
	{
	
		#ifdef ERROR_PRINT
			LOG("\nObjSizeFWUPGRDInfo is NULL!!!\n");					
		#endif	
			
		return ERROR;
	}
	
	/*To store the Firmware data in to a buffer*/
	bufferFwUpgrade = (USHORT MBXMEM*)malloc(ObjSizeFWUPGRDInfo);
	if (bufferFwUpgrade == NULL)
	{
		#ifdef ERROR_PRINT
			LOG("\nbufferFwUpgrade is NULL!!!\n");					
		#endif	

	   return ERROR;
	}
	return OK;

}
/**************************************************************************//**
 * \brief   - Function which reads the Object dictionary AcFirmwareUpgradeInfo
 * 			  and based on the structure (AcFirmwareUpgradeInfo) member
 * 			  Values ,It sends message  to the queue "/Fw_Msgq_Notify"
 * \param   - None         
 * \return  - Return 0 on success -1 on failure
 *
 ******************************************************************************/
INT16 FirmwareUpgrade :: ReadFwUpgradeFromOD()
{
	char msg[MSG_BUFF_SIZE];
	INT8 status, errorStatus;						
	
	/*Clear buffer content for a fresh read in subsequent cycles*/	
	memset (bufferFwUpgrade, 0,ObjSizeFWUPGRDInfo);	
	
	/*Read the Weld Recipe data to buffer*/	
	status = obj_readSc(FIRMWARE_UPGRADE_INDEX, SUB_INDEX, ObjSizeFWUPGRDInfo,pObjEntryFWUpgrade, (USHORT MBXMEM *)bufferFwUpgrade, COMPLETE_ACCESS);			
	

	if (status == SUCCESS)
	{					
		memcpy(&ObjectDictionary::PcFwUpgrade,bufferFwUpgrade,
				sizeof(ObjectDictionary::PcFwUpgrade));
		#ifdef PRINT
			LOG("\nReading Firmware Upgrade From Object Dictionary\n");						
			LOG("\nFirmware Upgrade Read Successfully\n\n\n");			
		#endif	//PRINT	
		errorStatus = SUCCESS;
		LOG("\nObjectDictionary::PcFwUpgrade.upgrade_status = %d\n",
				ObjectDictionary::PcFwUpgrade.upgrade_status);	
		
		switch (ObjectDictionary::PcFwUpgrade.upgrade_status)
		{
			case 2:
				msg[ArrIndexZero] = FIRMWARE_UPGRADE_START_REQ;
				if(msgQSend(FWmsgQId,msg,MSG_BUFF_SIZE,0,0) != OK)
				{
						#ifdef ERROR_PRINT
						LOG("\nSending Notification FromOBJ_Write to FirmwareUpgrade "
								"task Failed In ReadFwUpgradeFromOD() \n");
						#endif
				}
				
			break;
			default:
			#ifdef PRINT
				LOG("\n ReadFwUpgradeFromOD() Default");
			#endif
			break;
		}
	}
	else
	{
		#ifdef ERROR_PRINT
			LOG("\nFailure in Object Read!!!\n");					
		#endif				
		errorStatus = FAILURE;
	}		
	return errorStatus;	;	
	
}
/**************************************************************************//**
 * \brief   - Function to send Message to the Firmware upgrade::ProcessFwUpgradeMsgq()
 * \param   - None         
 * \return  - Return 0 on success -1 on failure
 *
 ******************************************************************************/
INT16 FirmwareUpgrade :: SendMsgToProcessFwUpgradeMsgq(UINT16 FWUPGRDMsg)
{
	char MsgQBuff[MSG_BUFF_SIZE];
	
	switch(FWUPGRDMsg)
	{
		case FIRMWARE_UPGRADE_DONE:
			MsgQBuff[ArrIndexZero] = FIRMWARE_UPGRADE_DONE;
		break;
		case FIRMWARE_UPGRADE_FAILED:
			MsgQBuff[ArrIndexZero] = FIRMWARE_UPGRADE_FAILED;
		break;
		default:
			MsgQBuff[ArrIndexZero] = NULL;
		break;
	}
	
	if(MsgQBuff[ArrIndexZero] != NULL)
	{
		if(msgQSend(FWmsgQId,MsgQBuff,MSG_BUFF_SIZE,0,0) != OK)
		{
			return ERROR;
		}
		else
		{
			return OK;
		}
	}
	else
	{
		return ERROR;
	}

}
/**************************************************************************//**
 * \brief   - Check new bootloader with CRC
 * \param   - 
 *
 * \return  - INT8 -true / false 
 *
 ******************************************************************************/
bool FirmwareUpgrade :: IsNewBootloaderForCRC(void)
{
	UINT32 Version;
	UINT8 ret = 0;
	QspiLibInit();
	QspiLibPrintId();

	
		if(QspiLibRead(UBOOT_VERSION_QSPI_OFFSET, (UINT32)&Version,
					sizeof(Version)) != OK)
		{
			LOG("Main_T : ------ Failed to Read Flash Info from QSPI -------\n");
		}
		if(Version == UBOOT_VERSION)
		{
#ifdef PRINT
			printf("uboot version is %d\n", Version);
#endif
			return TRUE;
		}
		else
			return FALSE;
}


/**************************************************************************//**
 * \brief   - Function which process the global message received from the 
 * 			  ethercat task.This function writes to the object dictionary 
 * 			  AcFirmwareUpgradeInfo based on the message received from
 * 			  the ethercat.
 * \param   - None         
 * \return  - Return 0 on success -1 on failure
 *
 ******************************************************************************/
INT16 FirmwareUpgrade :: ProcessFwUpgradeMsgq()
{
	UINT32 arrFWUpgrd[NUM_OF_FWUPGRD_INFO_ENTRY];
	UINT8 arrFWUpgrdSize[NUM_OF_FWUPGRD_INFO_ENTRY] = {2,1,1};
	SD_ID FWUP;
	
	/*Get OD data properties like Offset and Object Size*/
	if(GetFWInfoSizesFromOD() != OK)
	{
		#ifdef ERROR_PRINT
			LOG("Failed to Open Data Pointers!!!\n");
		#endif
		return ERROR;	
	}
	
	/*Open the global msgq*/
	FWmsgQId = msgQOpen(FW_UPGRD_MSGQ,0,0,0,0,0);
	if(FWmsgQId == MSG_Q_ID_NULL)
	{
		#ifdef ERROR_PRINT
			LOG("Failed to Open FW_UPGRD_MSGQ!!!\n");
		#endif
		return ERROR;	
	}
	
	FWUP = sdOpen(FW_UPGRD_SHARED_DATA, 0, 0, 0, 0, 
			SD_ATTR_RWX | SD_CACHE_OFF, (void **) &FWUpgradeBufff);
	
	if(SD_ID_NULL == FWUP)
	{
		#ifdef ERROR_PRINT
		LOG("Failed to Open Shared Data!!!\n");
		#endif
		return ERROR;		
	}
	
	/*wait forever to process firmware upgrade message*/
	while(1)
	{
		if(msgQReceive(FWmsgQId,MsgRecvBuff,MSG_BUFF_SIZE,WAIT_FOREVER) != ERROR)
		{
			SwitchIndex =  MsgRecvBuff[MSGIndexZero];
			switch(SwitchIndex)
			{
			/*Firmware Upgrade Notification From  Ethercat Stack*/
			case FIRMWARE_UPGRADE_ACK_REQ :

#ifdef PRINT
				LOG("\nFIRMWARE_UPGRADE_ACK \n");
#endif

				PcFirmUpGrdStr.progress = FWUpgrdReqReceive;		
				arrFWUpgrd[ArrIndexTwo] = PcFirmUpGrdStr.progress;

				obj_writeSc( FIRMWARE_UPGRADE_INDEX, arrFWUpgrd, FWUPGRDSizes, 
						arrFWUpgrdSize, NUM_OF_FWUPGRD_INFO_ENTRY);

				/*Give cpu time to ethercat task, to update OD*/
				taskDelay(FW_DELAY_TIME);

				break;
				/*Firmware File Received  Notification From  Ethercat Stack*/
			case FIRMWARE_FILE_RECVD_ACK_REQ :

#ifdef PRINT
				LOG("\nFIRMWARE_FILE_RECVD \n");
#endif

				PcFirmUpGrdStr.progress = FWUpgrdFileReceived;
				PcFirmUpGrdStr.CRC_Checksum = FWUpgradeBufff->FW_CRC_Checksum;
				PcFirmUpGrdStr.upgrade_status = FWUpgrdReqReceive;

#ifdef PRINT
				LOG("\nPcFirmUpGrdStr.CRC_Checksum = %d",PcFirmUpGrdStr.CRC_Checksum);	
#endif

				arrFWUpgrd[ArrIndexZero] = PcFirmUpGrdStr.CRC_Checksum;
				arrFWUpgrd[ArrIndexOne] = PcFirmUpGrdStr.upgrade_status;
				arrFWUpgrd[ArrIndexTwo] = PcFirmUpGrdStr.progress;

				obj_writeSc( FIRMWARE_UPGRADE_INDEX, arrFWUpgrd, FWUPGRDSizes, 
						arrFWUpgrdSize, NUM_OF_FWUPGRD_INFO_ENTRY);

				/*Give cpu time to ethercat task, to update OD*/
				taskDelay(FW_DELAY_TIME);

				break;
				/*Firmware Upgrade Start Notification From  Ethercat Stack*/
			case FIRMWARE_UPGRADE_START_REQ :
#ifdef PRINT
				LOG("\nFIRMWARE_UPGRADE_START \n");
#endif
				PcFirmUpGrdStr.upgrade_status = FWUpgrdStart;
				PcFirmUpGrdStr.progress = FWUpgrdStart;	

				arrFWUpgrd[ArrIndexOne] = PcFirmUpGrdStr.upgrade_status;
				arrFWUpgrd[ArrIndexTwo] = PcFirmUpGrdStr.progress;	

				obj_writeSc( FIRMWARE_UPGRADE_INDEX, arrFWUpgrd,FWUPGRDSizes,
						arrFWUpgrdSize, NUM_OF_FWUPGRD_INFO_ENTRY);

				/*Give cpu time to ethercat task, to update OD*/
				taskDelay(FW_DELAY_TIME);
				UbootVersionFlag = IsNewBootloaderForCRC();
				if(UbootVersionFlag)
				{
					if( EraseAndFlashQSPICRC() != OK)
					{
						/*Firmware Upgrade Failed*/
						PcFirmUpGrdStr.upgrade_status = FWUpgrdFailed;	
						arrFWUpgrd[ArrIndexTwo] = PcFirmUpGrdStr.upgrade_status;		
						obj_writeSc( FIRMWARE_UPGRADE_INDEX, arrFWUpgrd, FWUPGRDSizes,
								arrFWUpgrdSize, NUM_OF_FWUPGRD_INFO_ENTRY);

						/*Give cpu time to ethercat task, to update OD*/
						taskDelay(FW_DELAY_TIME);
					}
				}		
				else
				{
					if( EraseAndFlashQSPI() != OK)
					{
						/*Firmware Upgrade Failed*/
						PcFirmUpGrdStr.upgrade_status = FWUpgrdFailed;	
						arrFWUpgrd[ArrIndexTwo] = PcFirmUpGrdStr.upgrade_status;		
						obj_writeSc( FIRMWARE_UPGRADE_INDEX, arrFWUpgrd, FWUPGRDSizes,
								arrFWUpgrdSize, NUM_OF_FWUPGRD_INFO_ENTRY);

						/*Give cpu time to ethercat task, to update OD*/
						taskDelay(FW_DELAY_TIME);
					}
					
				}
							
			break;	
			/*Firmware File is flashed to QSPI Flash Notification From FimwareUpgrade Task Itself*/
			case FIRMWARE_UPGRADE_DONE :

#ifdef PRINT
				LOG("\nFIRMWARE_UPGRADE_DONE \n");	
#endif

				PcFirmUpGrdStr.progress = FWUpgrdDone;	

				arrFWUpgrd[ArrIndexTwo] = PcFirmUpGrdStr.progress;						
				obj_writeSc( FIRMWARE_UPGRADE_INDEX, arrFWUpgrd, FWUPGRDSizes,
						arrFWUpgrdSize, NUM_OF_FWUPGRD_INFO_ENTRY);

				/*Give cpu time to ethercat task, to update OD*/
				taskDelay(FW_DELAY_TIME);

				break;

				/*Change in AC Firmware Upgrade OD ,Notification From Ethercat stack*/
			case CHANGE_IN_FIRMWARE_UPGRADE_OD:

#ifdef PRINT
				LOG("\nCHANGE_IN_FIRMWARE_UPGRADE_OD \n");
#endif

				if(ReadFwUpgradeFromOD() != OK)
				{
					LOG("\nRead FwUpgradeFromOD  Failed\n");	
					PcFirmUpGrdStr.upgrade_status = FWUpgrdFailed;	
					arrFWUpgrd[ArrIndexTwo] = PcFirmUpGrdStr.upgrade_status;	

					obj_writeSc( FIRMWARE_UPGRADE_INDEX, arrFWUpgrd, FWUPGRDSizes,
							arrFWUpgrdSize, NUM_OF_FWUPGRD_INFO_ENTRY);

					/*Give cpu time to ethercat task, to update OD*/
					taskDelay(FW_DELAY_TIME);
				}
				break;
				/*Flashing to QSPI Flash Failed Notification From FimwareUpgrade Task Itself*/
			case FIRMWARE_UPGRADE_FAILED:

#ifdef PRINT
				LOG("\nFIRMWARE_UPGRADE_FAILED \n");
#endif

				PcFirmUpGrdStr.upgrade_status = FWUpgrdFailed;	
				arrFWUpgrd[ArrIndexTwo] = PcFirmUpGrdStr.upgrade_status;	

				obj_writeSc( FIRMWARE_UPGRADE_INDEX, arrFWUpgrd, FWUPGRDSizes,
						arrFWUpgrdSize, NUM_OF_FWUPGRD_INFO_ENTRY);

				/*Give cpu time to ethercat task, to update OD*/
				taskDelay(FW_DELAY_TIME);
				break;

			default:
#ifdef PRINT
				LOG("\n IN ProcessFwUpgradeMsgq() Default\n");
#endif
				break;
			}
		}
		else
		{
			LOG("Failed to receive message\n");
		}
	}
		
	return OK;
}

/**************************************************************************//**
 * \brief   - Calculates CRC of Firmware Upgrade Files
 *
 * \param   - const unsigned char *dataBuffer, UINT32 buffLen
 *
 * \return  - UINT16 - CRC value
 *
 ******************************************************************************/
unsigned short FirmwareUpgrade::ByteCRC(char * Ptr, unsigned int Length)
{
   unsigned short CRCRegister = 0;
   unsigned char  Index;
   while(Length-- > 0){
      Index = (unsigned char)((CRCRegister ^ * Ptr++) & 0xff);
      CRCRegister = CRCRegister >> 8;
      CRCRegister = CRCRegister ^ CRCTable[Index];
   }
   return(CRCRegister);
}
/**************************************************************************//**
 * \brief   - Function to erase the QSPI Flash and writes Firmware binary
 *			  file data into the QSPI flash.
 * 			 
 * \param   - None
 *            
 * \return  - Return 0 on success -1 on failure
 *
 ******************************************************************************/
INT16 FirmwareUpgrade::EraseAndFlashQSPI()
{
	FwInfo FWInfoBuffStruct;
	FwInfo *FWInfoBuff = &FWInfoBuffStruct;
	
	char msg[MSG_BUFF_SIZE];
	UINT32 arrFWUpgrd[NUM_OF_FWUPGRD_INFO_ENTRY];
	UINT8 arrFWUpgrdSize[NUM_OF_FWUPGRD_INFO_ENTRY] = {2,1,1};
	INT8 ret = 0;
	
	UINT32  startBlockNumber = 0;
	UINT32 endBlockNumber = 0;
	UINT32 eraseCount =0;	
	UINT32 offsetAddr;
	Eeprom epclass;
	bool ReadEeprom = false;

	
	/*Read the Firmware information from the structure FirmWareUpgradeInfo*/
	SD_ID FWUP = sdOpen(FW_UPGRD_SHARED_DATA, 0, 0, 0, 0,
					SD_ATTR_RWX | SD_CACHE_OFF, (void **) &FWUpgradeBufff);
	if(SD_ID_NULL == FWUP)
	{
		#ifdef ERROR_PRINT
			LOG("Failed to Open Shared Data!!!\n");
		#endif
		return ERROR;		
	}
	
	/* Read partition info From QSPI */
	if(QspiLibRead(FW_PARTITION_INFO_QSPI_OFFSET, (unsigned int) FWInfoBuff,
				sizeof(FwInfo)) != OK)
	{
		#ifdef ERROR_PRINT
			LOG("Failed to Read Flash Info from QSPI\n");
		#endif
		ReadEeprom = true;
	}
	
	/*Check Partition info is Corrupted or not */
	if ((ReadEeprom) || ((FWInfoBuff->activePart != ACTIVE_SLAVE_FIRMWARE_START)
				&& (FWInfoBuff->activePart != PASSIVE_SLAVE_FIRMWARE_START)))
	{
		/* Read Partition info from EEPROM */
		ret = epclass.Read((char*) FWInfoBuff, sizeof(FwInfo),
									FW_PARTITION_INFO_EEPROM_OFFSET);
		{
			#ifdef ERROR_PRINT
				
			#endif
		}
	
		/*check the data from EEPROM is proper */
		if ((ret < ZERO_BYTES) || ((FWInfoBuff->activePart != ACTIVE_SLAVE_FIRMWARE_START)
				&& (FWInfoBuff->activePart != PASSIVE_SLAVE_FIRMWARE_START)))
		{
			LOG("Reading Firmware Part info from EEPROM failed \n");
			FWInfoBuff->bootMode = BRANSON_ACTIVE_BOOTMODE;
			FWInfoBuff->fwUpgrade = BRANSON_NORMAL_BOOT;
			FWInfoBuff->activePart = ACTIVE_SLAVE_FIRMWARE_START;
			FWInfoBuff->passivePart = PASSIVE_SLAVE_FIRMWARE_START;
		}	
	}
	
	offsetAddr = FWInfoBuff->passivePart;
	startBlockNumber = (offsetAddr / BLOCK_SIZE);
	endBlockNumber = (offsetAddr + FWUpgradeBufff->FW_Length) / BLOCK_SIZE;
	
	/*Erase the QSPI flash before write*/
	for (eraseCount = startBlockNumber; eraseCount <= endBlockNumber; eraseCount++)
	{
		QspiLibBlockErase(eraseCount);
	}
	
	/*we are  not posting FIRMWARE_UPGRADE_FLASH_ERASED message to the message queue Id FWmsgQId*/
	/*We found that ,if we post FIRMWARE_UPGRADE_FLASH_ERASED message in the queue,the message queue processing is not  immediate */
	/*custom system call is taking more priority than message queue and hence we are writing FIRMWARE_UPGRADE_FLASH_ERASED to the OD and then proceed to The QSPI write */
	/*Following code updates OD Immediately */
	
	PcFirmUpGrdStr.progress = FWUpgrdFlashErased;	
	arrFWUpgrd[ArrIndexTwo] = PcFirmUpGrdStr.progress;						
	obj_writeSc( FIRMWARE_UPGRADE_INDEX, arrFWUpgrd, FWUPGRDSizes,
			arrFWUpgrdSize, NUM_OF_FWUPGRD_INFO_ENTRY);
	
	/*Give cpu time to ethercat task, to update OD*/
	taskDelay(FW_DELAY_TIME);
	
	/*Write Entire Firmware upgrade binary file into QSPI Flash*/
	if(QspiLibWrite(offsetAddr,FWUpgradeBufff->FW_FileBuffAddress,FWUpgradeBufff->FW_Length) != OK)
	{
		/*Post Message to the SendMsgToProcessFwUpgradeMsgq*/
		if(SendMsgToProcessFwUpgradeMsgq(FIRMWARE_UPGRADE_FAILED) != OK)
		{
			#ifdef ERROR_PRINT
				LOG("\nSending Notification FromOBJ_Write to FirmwareUpgrade "
						"task Failed In ReadFwUpgradeFromOD() \n");
			#endif
			return ERROR;
		}
	}
	
	/*set flag once upgrade is done */
	
	FWInfoBuff->fwUpgrade = BRANSON_FWUPGRADE;

	/*Update the structure FwInfo into EEPROM*/
	if(epclass.Write((char *)FWInfoBuff,sizeof(FwInfo),FW_PARTITION_INFO_EEPROM_OFFSET) < ZERO_BYTES)
	{
		#ifdef ERROR_PRINT
			LOG("\nFailed To Write Flash Info to EEPROM\n");
		#endif
	}
	
	
	/*Erase QSPI before write the structure */
	QspiLibBlockErase(FIRMWARE_LOC_BLOCK_NUM);
	
	/*Update the structure FwInfo into flash*/
	if(QspiLibWrite(FW_PARTITION_INFO_QSPI_OFFSET, (uint32_t)FWInfoBuff, sizeof(FwInfo)) != OK)
	{
		#ifdef ERROR_PRINT
			LOG("Failed to Write Flash Info to QSPI\n");
		#endif
		return ERROR;
	}
	
	
	/*Post Message to the SendMsgToProcessFwUpgradeMsgq*/
	if(SendMsgToProcessFwUpgradeMsgq(FIRMWARE_UPGRADE_DONE) != OK)
	{
		#ifdef ERROR_PRINT
			LOG("\nSending Notification FromOBJ_Write to FirmwareUpgrade "
					"task Failed In ReadFwUpgradeFromOD() \n");
		#endif
		return ERROR;
	}

	return OK;
}

/**************************************************************************//**
 * \brief   - Function to erase the QSPI Flash and writes Firmware binary
 *			  file data into the QSPI flash.
 * 			 
 * \param   - None
 *            
 * \return  - Return 0 on success -1 on failure
 *
 ******************************************************************************/
INT16 FirmwareUpgrade::EraseAndFlashQSPICRC()
{
	FwInfoCRC FWInfoBuffStruct;
	FwInfoCRC *FWInfoBuff = &FWInfoBuffStruct;
	
	char msg[MSG_BUFF_SIZE];
	UINT32 arrFWUpgrd[NUM_OF_FWUPGRD_INFO_ENTRY];
	UINT8 arrFWUpgrdSize[NUM_OF_FWUPGRD_INFO_ENTRY] = {2,1,1};
	INT8 ret = 0;
	
	UINT32  startBlockNumber = 0;
	UINT32 endBlockNumber = 0;
	UINT32 eraseCount =0;	
	UINT32 offsetAddr;
	Eeprom epclass;
	bool ReadEeprom = false;

	
	/*Read the Firmware information from the structure FirmWareUpgradeInfo*/
	SD_ID FWUP = sdOpen(FW_UPGRD_SHARED_DATA, 0, 0, 0, 0,
					SD_ATTR_RWX | SD_CACHE_OFF, (void **) &FWUpgradeBufff);
	if(SD_ID_NULL == FWUP)
	{
		#ifdef ERROR_PRINT
			LOG("Failed to Open Shared Data!!!\n");
		#endif
		return ERROR;		
	}
	
	
	/* Read partition info From QSPI */
	if(QspiLibRead(FW_PARTITION_INFO_QSPI_OFFSET, (unsigned int) FWInfoBuff,
				sizeof(FwInfoCRC)) != OK)
	{
		#ifdef ERROR_PRINT
			LOG("Failed to Read Flash Info from QSPI\n");
		#endif
		ReadEeprom = true;
	}
	
	/*Check Partition info is Corrupted or not */
	if ((ReadEeprom) || ((FWInfoBuff->partition_a_offset != ACTIVE_SLAVE_FIRMWARE_START)
				&& (FWInfoBuff->partition_a_offset != PASSIVE_SLAVE_FIRMWARE_START)))
	{
		/* Read Partition info from EEPROM */
		ret = epclass.Read((char*) FWInfoBuff, sizeof(FwInfoCRC),
									FW_PARTITION_INFO_EEPROM_OFFSET);
		{
			#ifdef ERROR_PRINT
				
			#endif
		}
	
		/*check the data from EEPROM is proper */
		if ((ret < ZERO_BYTES) || ((FWInfoBuff->partition_a_offset != ACTIVE_SLAVE_FIRMWARE_START)
				&& (FWInfoBuff->partition_a_offset != PASSIVE_SLAVE_FIRMWARE_START)))
		{
			LOG("Reading Firmware Part info from EEPROM failed \n");
			FWInfoBuff->boot_status = BRANSON_ACTIVE_BOOTMODE;
			FWInfoBuff->fw_upgrade = BRANSON_NORMAL_BOOT;
			FWInfoBuff->partition_a_offset = ACTIVE_SLAVE_FIRMWARE_START;
			FWInfoBuff->partition_b_offset = PASSIVE_SLAVE_FIRMWARE_START;
		}	
	}
	
	offsetAddr = FWInfoBuff->partition_b_offset;
	startBlockNumber = (offsetAddr / BLOCK_SIZE);
	endBlockNumber = (offsetAddr + FWUpgradeBufff->FW_Length) / BLOCK_SIZE;
	
	/*Erase the QSPI flash before write*/
	for (eraseCount = startBlockNumber; eraseCount <= endBlockNumber; eraseCount++)
	{
		QspiLibBlockErase(eraseCount);
	}
	
	/*we are  not posting FIRMWARE_UPGRADE_FLASH_ERASED message to the message queue Id FWmsgQId*/
	/*We found that ,if we post FIRMWARE_UPGRADE_FLASH_ERASED message in the queue,the message queue processing is not  immediate */
	/*custom system call is taking more priority than message queue and hence we are writing FIRMWARE_UPGRADE_FLASH_ERASED to the OD and then proceed to The QSPI write */
	/*Following code updates OD Immediately */
	
	PcFirmUpGrdStr.progress = FWUpgrdFlashErased;	
	arrFWUpgrd[ArrIndexTwo] = PcFirmUpGrdStr.progress;						
	obj_writeSc( FIRMWARE_UPGRADE_INDEX, arrFWUpgrd, FWUPGRDSizes,
			arrFWUpgrdSize, NUM_OF_FWUPGRD_INFO_ENTRY);
	
	/*Give cpu time to ethercat task, to update OD*/
	taskDelay(FW_DELAY_TIME);
	
	/*Write Entire Firmware upgrade binary file into QSPI Flash*/
	if(QspiLibWrite(offsetAddr,FWUpgradeBufff->FW_FileBuffAddress,FWUpgradeBufff->FW_Length) != OK)
	{
		/*Post Message to the SendMsgToProcessFwUpgradeMsgq*/
		if(SendMsgToProcessFwUpgradeMsgq(FIRMWARE_UPGRADE_FAILED) != OK)
		{
			#ifdef ERROR_PRINT
				LOG("\nSending Notification FromOBJ_Write to FirmwareUpgrade "
						"task Failed In ReadFwUpgradeFromOD() \n");
			#endif
			return ERROR;
		}
	}
	
	/*set flag once upgrade is done */
	
	FWInfoBuff->fw_upgrade = BRANSON_FWUPGRADE;
	if(offsetAddr == ACTIVE_SLAVE_FIRMWARE_START)
	{
		FWInfoBuff->partition_a_crc = ByteCRC((char *) FWUpgradeBufff->FW_FileBuffAddress, FWUpgradeBufff->FW_Length);
		FWInfoBuff->partition_a_size = FWUpgradeBufff->FW_Length;
	}
	else if(offsetAddr == PASSIVE_SLAVE_FIRMWARE_START)
	{
		FWInfoBuff->partition_b_crc = ByteCRC((char *) FWUpgradeBufff->FW_FileBuffAddress, FWUpgradeBufff->FW_Length);
		FWInfoBuff->partition_b_size = FWUpgradeBufff->FW_Length;
	}	
	/*Update the structure FwInfo into EEPROM*/
	if(epclass.Write((char *)FWInfoBuff,sizeof(FwInfoCRC),FW_PARTITION_INFO_EEPROM_OFFSET) < ZERO_BYTES)
	{
		#ifdef ERROR_PRINT
			LOG("\nFailed To Write Flash Info to EEPROM\n");
		#endif
	}
	
	
	/*Erase QSPI before write the structure */
	QspiLibBlockErase(FIRMWARE_LOC_BLOCK_NUM);
	
	/*Update the structure FwInfo into flash*/
	if(QspiLibWrite(FW_PARTITION_INFO_QSPI_OFFSET, (uint32_t)FWInfoBuff, sizeof(FwInfoCRC)) != OK)
	{
		#ifdef ERROR_PRINT
			LOG("Failed to Write Flash Info to QSPI\n");
		#endif
		return ERROR;
	}
	
	
	/*Post Message to the SendMsgToProcessFwUpgradeMsgq*/
	if(SendMsgToProcessFwUpgradeMsgq(FIRMWARE_UPGRADE_DONE) != OK)
	{
		#ifdef ERROR_PRINT
			LOG("\nSending Notification FromOBJ_Write to FirmwareUpgrade "
					"task Failed In ReadFwUpgradeFromOD() \n");
		#endif
		return ERROR;
	}

	return OK;
}

/**************************************************************************//**
 * \brief   - Get OD data properties like Offset and Object Size
 * 			 
 * \param   - Deallocates the object memory
 *            
 * \return  - None
 *
 ******************************************************************************/
FirmwareUpgrade::~FirmwareUpgrade()
{
	LOG("\n~FirmwareUpgrade()\n");
	free(bufferFwUpgrade);
	msgQClose(FWmsgQId);
}
