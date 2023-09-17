/**********************************************************************************************************

` Copyright (c) Branson Ultrasonics Corporation, 1996-2018

 This program is the property of Branson Ultrasonics Corporation
 Copying of this software is expressly forbidden, without the prior
 written consent of Branson Ultrasonics Corporation.


 ---------------------------- MODULE DESCRIPTION ----------------------------

 Firmware Upgrade task for Slaves AC/PC and Master

 **********************************************************************************************************/
#include "FirmwareUpgrade.h"
#include "commons.h"
#include "Eeprom.h"
#include "DataBaseManager.h"
#include "version_common.h"

/* Static member initializations */
FirmWareUpgradeInfo FirmwareUpgrade::fwUpgrdInfoAC = { 0 };
FirmWareUpgradeInfo FirmwareUpgrade::fwUpgrdInfoPC = { 0 };
bool FirmwareUpgrade::bIsFirmwareUpgrade = false;

/**************************************************************************//**
 * \brief   - Initialization of class members and memory for object.
 *
 * \param   - None.
 *
 * \return  - None
 *
 ******************************************************************************/
FirmwareUpgrade::FirmwareUpgrade() :
		FW_MSG_Q_ID(0), SDO_MSG_Q_ID(0), fwUpgrdCRC(0), pBuf(NULL), fp(NULL), Ctrl_TID(
				0), SDO_TID(0), UI_TID(0), DB_TID(0), UI_DB_TID(0)
{
	CP = CommonProperty::getInstance();
	SDO_MSG_Q_ID = CP->GetMsgQId(cTaskName[SDO_T]);
	Interface = CommunicateInterface::getinstance();
}

/**************************************************************************//**
 * \brief   - Custom system call for triggering FOE DL REQ.
 *
 * \param   - UINT16 iSlaveID, UINT16 dwFileNameLen, char *dwFileName, 
 *            ULONG dwDataLen, unsigned char *pData
 *
 * \return  - UINT32 Success / Failure
 *
 ******************************************************************************/
UINT32 FirmwareUpgrade::FoeDownloadReqSysCall(UINT16 iSlaveID,
		UINT16 dwFileNameLen, char *dwFileName, UINT32 dwDataLen,
		unsigned char *pData) 
{

	return syscall((_Vx_usr_arg_t) iSlaveID, (_Vx_usr_arg_t) dwFileNameLen,
			(_Vx_usr_arg_t) dwFileName, (_Vx_usr_arg_t) dwDataLen,
			(_Vx_usr_arg_t) pData, 0, 0, 0,
			SYSCALL_NUMBER(GROUP_NUMBER, FOE_DL_SYSCALL_ENTRY));

}

/**************************************************************************//**
 * \brief   - Post the encoded message to Different Task message queue
 *
 * \param   - Message& msg, MSG_Q_ID
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::SendToMsgQ(Message& msg, const MSG_Q_ID &MsgQID) 
{
	if (msgQSend(MsgQID, reinterpret_cast<char*>(&msg), sizeof(msg), NO_WAIT,
			MSG_PRI_NORMAL) != OK)
	{
		LOGDBG("FOE_T : -----------Error on msgQSend----------", 0, 0, 0);
	}
}

/**************************************************************************//**
 * \brief   - Read the slave upgrade file and trigger FoE DL 
 *
 * \param   - UINT16 iSlaveID, UINT32 fwFileDataLen, char *dwFileName
 *
 * \return  - INT8 - Success / Failure or Error values
 *
 ******************************************************************************/
INT8 FirmwareUpgrade::SlaveFirmwareUpgrade(UINT16 iSlaveID, UINT32 fwFileDataLen,
		char *dwFileName)
{
	UINT32 dwRes = 0;

	/* Add the file Name to the path */
	fwFilePath.clear();
	fwFilePath.append(fwFileDirectory);
	fwFilePath.append(dwFileName);

	/* Open the Firmware Upgrade File */
	fp = fopen((char *) fwFilePath.c_str(), READ_BINARY);
	if (fp == NULL)
	{
		LOGDBG("FW_T : UNABLE TO OPEN FILE %s",
				(_Vx_usr_arg_t )fwFilePath.c_str(), 0, 0);
		return FAILURE;
	}

	/* Allocate memory */
	pBuf = (char *) malloc(fwFileDataLen);
	if (pBuf == NULL)
	{
		LOGDBG("FW_T : MEMMORY ALLOCATION FAILED", 0, 0, 0);
		fclose(fp);
		return FAILURE;
	}

	LOGDBG("FW_T : READING %d BYTES FROM FILE : %s", fwFileDataLen,
			(_Vx_usr_arg_t )dwFileName, 0);

	/* Read file contents into  Buffer */
	bytesRead = fread(pBuf, fwFileDataLen, READ_BYTE, fp);

	if ((bytesRead == ZERO_BYTES) && (ferror(fp)))
	{
		LOGDBG("FW_T : READING FILE CONTENTS FAILED", 0, 0, 0);
		if (pBuf != NULL)
		{
			free(pBuf);
			pBuf = NULL;
		}
		fclose(fp);
		return FAILURE;
	}

	/* close the file */
	fclose(fp);

	/* Send the Progress Response to UI */
	if (iSlaveID == SLAVE_IDX_AC)
	{
		ProgressResponseToUI(AC_PROGRESS_ONE);
	} 
	else if (iSlaveID == SLAVE_IDX_PC)
	{
		ProgressResponseToUI(PC_PROGRESS_ONE);
	}

	/* call the FoE System Call */
	dwRes = FoeDownloadReqSysCall(iSlaveID, strlen(dwFileName), dwFileName,
			fwFileDataLen, (unsigned char *) pBuf);
	if (dwRes != EC_NOERROR)
	{
		LOGDBG("FW_T : FOE DOWNLOAD FAILURE", 0, 0, 0);
		if (pBuf != NULL)
		{
			free(pBuf);
			pBuf = NULL;
		}
		return FAILURE;
	}
	LOGDBG("FW_T : FOE DOWNLOAD SUCCESS", 0, 0, 0);

	/* calculate CRC of the Upgrade File */
	fwUpgrdCRC = crc16_ccitt((char *) pBuf, fwFileDataLen);
	
	LOGDBG("FW_T : CRC -> %d", (_Vx_usr_arg_t )fwUpgrdCRC, 0, 0);

	/* Free the Memory Allocated */
	if (pBuf != NULL)
	{
		free(pBuf);
		pBuf = NULL;
	}

	/* Get the Response From Slaves */
	if (iSlaveID == SLAVE_IDX_AC) 
	{
		slaveStatus = ProgressResponseFromAC();
	} 
	else if (iSlaveID == SLAVE_IDX_PC)
	{
		slaveStatus = ProgressResponseFromPC();
	}

	if (slaveStatus == ERROR) 
	{
		return FAILURE;
	}

	return SUCCESS;
}

/**************************************************************************//**
 * \brief   - Check whether USB is connected or not ,if connected read firmware
 * 			  upgrade files
 * \param   - void
 *
 * \return  - char * - FileNames / USB_READ_FAIL
 *
 ******************************************************************************/
char * FirmwareUpgrade::USBDetect() 
{
	int fd = 0;

	/* open the USB */
	fd = open(USB_PATH, O_RDONLY);

	/* check Whether it is connected or not */
	if (fd < 0) 
	{
		LOGDBG("FW_T : USB IS NOT DETECTED  %s", (_Vx_usr_arg_t )USB_PATH, 0,
				0);
		/* send USB Read Fail Message to UI */
		memset(pErrorBuffer, 0x00, ERROR_BUFF_SIZE);
		sprintf(pErrorBuffer, "%u", USB_READ_FAIL);
		return pErrorBuffer;
	} 
	else 
	{
		LOGDBG("FW_T : USB DETECTED  %s", (_Vx_usr_arg_t )USB_PATH, 0, 0);
		close(fd);
		/* Read the Firmware Upgrade files From USB */
		return USBRead();
	}
}

/**************************************************************************//**
 * \brief   - Read USB data
 *
 * \param   - void
 *
 * \return  - char * - FileNames / Error
 *
 ******************************************************************************/
char * FirmwareUpgrade::USBRead()
{
	char fileBuffer[FILE_NAME_MAX_SIZE] = { 0x00 };
	INT32 fwFileNameRead = 1;
	UINT32 fileListError = 0;
	int fd = 0;

	/* change the default directory to USB */
	LOGDBG("FW_T : CHANGING THE DIRECTORY TO USB", 0, 0, 0);
	usbReadStatus = chdir(USB_PATH);

	/* Check Whether USB is Opened */
	if (usbReadStatus == ERROR) 
	{
		LOGDBG("FW_T : FAILED TO OPEN USB", 0, 0, 0);
		return NULL;
	}

	/* Open Cache File to store Firmware Upgrade File List */
	fp = fopen( CACHE_FILE, RDWR_CREAT);
	if (fp == NULL) 
	{
		LOGDBG("FW_T : UNABLE TO CREATE CACHE", 0, 0, 0);
		return NULL;
	}

	/* create File descriptor from File pointer */
	fd = fileno(fp);

	/* generate a long listing of directory contents */

	/* Read SC File names */
	usbReadStatus = dirList(fd, SC_FILE_NAME_FORMAT, FALSE, FALSE);
	if (usbReadStatus == ERROR)
	{
		LOGDBG("FW_T : UNABLE TO READ SC FILES FROM USB", 0, 0, 0);
		fileListError += 1;
	}

	/* Read PC File names */
	usbReadStatus = dirList(fd, PC_FILE_NAME_FORMAT, FALSE, FALSE);
	if (usbReadStatus == ERROR)
	{
		LOGDBG("FW_T : UNABLE TO READ PC FILES FROM USB", 0, 0, 0);
		fileListError += 1;
	}

	/* Read AC File names */
	usbReadStatus = dirList(fd, AC_FILE_NAME_FORMAT, FALSE, FALSE);
	if (usbReadStatus == ERROR) 
	{
		LOGDBG("FW_T : UNABLE TO READ AC FILES FROM USB", 0, 0, 0);
		fileListError += 1;
	}

	/* Check Error Occurred While Listing File Names */
	if (fileListError == USB_FILE_LIST_ERROR) 
	{
		LOGDBG("FW_T : UNABLE TO LIST FW UPGRADE FILES FROM USB", 0, 0, 0);
		/* close and remove cache file */
		fclose(fp);
		if (OK != remove(CACHE_FILE))
		{
			LOGDBG("FW_T : UNABLE TO REMOVE CACHE FILE", 0, 0, 0);
		}
		return NULL;
	}

	/* seek to start of the file */
	fseek(fp, ZERO_BYTES, SEEK_SET);

	/* If files are present send the file names to UIC */
	/* clear each module buffer */
	fwFileNameSC.clear();
	fwFileNamePC.clear();
	fwFileNameAC.clear();

	/* Clear the file Count of each module */
	fileCountAC = fileCountPC = fileCountSC = 0;

	/* Read the Firmware upgrade file names from cache file */
	while (fwFileNameRead > ZERO_BYTES) 
	{
		/* Clear the buffer before storing */
		memset(fileBuffer, '\0', sizeof(char) * FILE_NAME_MAX_SIZE);

		/* Read the file names from cache file */
		fwFileNameRead = fscanf(fp, "%s", fileBuffer);

		/* Check SC file name and file count */
		if ((strncmp(fileBuffer, SC_FILE_HEADER, strlen(SC_FILE_HEADER)) == 0)
				&& (fileCountSC < FW_UPGRADE_FILE_COUNT_MAX)) 
		{
			/* Append to SC File name Buffer */
			fwFileNameSC.append(fileBuffer);
			fwFileNameSC.append(",");
			fileCountSC++;
		}
		/* Check PC file name and file count */
		if ((strncmp(fileBuffer, PC_FILE_HEADER, strlen(PC_FILE_HEADER)) == 0)
				&& (fileCountPC < FW_UPGRADE_FILE_COUNT_MAX)) 
		{
			/* Append to PC File name Buffer */
			fwFileNamePC.append(fileBuffer);
			fwFileNamePC.append(",");
			fileCountPC++;
		}
		/* Check AC file name and file count */
		if ((strncmp(fileBuffer, AC_FILE_HEADER, strlen(AC_FILE_HEADER)) == 0)
				&& (fileCountAC < FW_UPGRADE_FILE_COUNT_MAX)) 
		{
			/* Append to AC File name Buffer */
			fwFileNameAC.append(fileBuffer);
			fwFileNameAC.append(",");
			fileCountAC++;
		}
	}

	/* close and remove cache file */
	fclose(fp);
	
	if (OK != remove(CACHE_FILE))
	{
		LOGDBG("FW_T : UNABLE TO REMOVE CACHE FILE", 0, 0, 0);
	}
	/* Check whether Files are present or not */
	if (fwFileNameSC.empty() && fwFileNamePC.empty() && fwFileNameAC.empty()) 
	{
		LOGDBG("FW_T : NO FILES PRESENT IN USB USB", 0, 0, 0);
		/* Send No Files Present Message to UI */
		sprintf(pErrorBuffer, "%u", NO_FILES_PRESENT);
		return pErrorBuffer;
	}

	/* Separate each module name with delimiters */
	if (fwFileNameSC.empty() == FALSE) 
	{
		fwFileNameSC.erase(fwFileNameSC.end() - 1);
	}
	if (fwFileNamePC.empty() == FALSE)
	{
		fwFileNamePC.erase(fwFileNamePC.end() - 1);
	}
	if (fwFileNameAC.empty() == FALSE) 
	{
		fwFileNameAC.erase(fwFileNameAC.end() - 1);
	}
	/* Append with delimiters at the end */
	fwFileNameSC.append("||");
	fwFileNameAC.append("||");
	fwFileNamePC.append("||");
	fwFileName.clear();
	/* Append all the modules file names to a single message buffer */
	fwFileName.append(fwFileNameAC);
	fwFileName.append(fwFileNamePC);
	fwFileName.append(fwFileNameSC);

	cout << "FW_T : FW UPGRADE FILES :- \n" << fwFileName.c_str() <<endl;
	/* Send File names to UIC */
	return (char *) fwFileName.c_str();
}

/**************************************************************************//**
 * \brief   - Detect the USB and Read the firmware upgrade files and send to UI
 *
 * \param   - void
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::ReadUSBResponseToUI() 
{
	char *srcBuffer = NULL;
	UINT32 iLen = 0;
	USBResponseMsg respMsg;

	memset(respMsg.Buffer, 0x00, sizeof(respMsg.Buffer));
	/* Call your function and copy file names to socket buffer */
	srcBuffer = USBDetect();
	if (srcBuffer != NULL) 
	{
		memcpy(respMsg.Buffer, srcBuffer, strlen(srcBuffer));
	} 
	else 
	{
		/* Send error message when error occurs */
		sprintf(respMsg.Buffer, "%u", RESPONSE_FAIL);
	}

	/* Send message to UIC */
	respMsg.msgID = UIC_FW_UPGRADE_RES;
	respMsg.msglen = strlen(respMsg.Buffer);
	iLen = sizeof(respMsg.msgID) + sizeof(respMsg.msglen) + respMsg.msglen;

	Interface->Send(reinterpret_cast<char*>(&respMsg), iLen);
}

/**************************************************************************//**
 * \brief   - Check new bootloader with CRC
 * \param   - 
 *
 * \return  - INT8 -true / false 
 *
 ******************************************************************************/
bool FirmwareUpgrade::IsNewBootloaderForCRC(void)
{
	UINT32 Version;
	UINT8 ret = 0;
	/* Read the Firmware Part Info from Qspi */
		ret = QSPIFlashRead(UBOOT_VERSION_QSPI_OFFSET,
				sizeof(Version), (UINT32)&Version);

		if (ret == FAILURE) 
		{
			LOGERR("FW_T : READING UBOOT Version FROM QSPI FAILED ", 0, 0, 0);
		}
		
		if(Version == UBOOT_VERSION)
		{
			LOGDBG("FW_T : UBOOT Version:%d", Version, 0, 0);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
}

/**************************************************************************//**
 * \brief   - Process the firmware upgrade messages received from UIC and trigger
 * 			  the master and slaves firmware upgrade
 * \param   - char *fwMsgBuff
 *
 * \return  - INT8 - Success / Failure or Error values
 *
 ******************************************************************************/
INT8 FirmwareUpgrade::ProcessFirmwareMsg(char *fwMsgBuff) 
{
	UINT32 msgCount = 0;
	INT8 fileSizeStatus = 0;
	char *fwMsgToken = NULL;
	bool bIsUpgradeSC = 0, bIsUpgradePC = 0, bIsUpgradeAC = 0;
	
	/* Message received from UIC */

	LOGDBG("FW_T : MESSAGE RECIEVED FROM UIC :- %s", (_Vx_usr_arg_t )fwMsgBuff,
			0, 0);

	/* clear each module buffer */
	fwFileNameSC.clear();
	fwFileNamePC.clear();
	fwFileNameAC.clear();

	/* Extract messages received */
	fwMsgToken = strtok(fwMsgBuff, "||");

	while (fwMsgToken != NULL)
	{
		/* Store the messages to corresponding variables */

		if (msgCount == MSG_INDEX_ZERO) 
		{
			/* Set upgrade flag for AC */
			bIsUpgradeAC = atoi(fwMsgToken);
		} 
		else if (msgCount == MSG_INDEX_ONE) 
		{
			/* Set upgrade flag for PC */
			bIsUpgradePC = atoi(fwMsgToken);
		} 
		else if (msgCount == MSG_INDEX_TWO)
		{
			/* Set upgrade flag for SC */
			bIsUpgradeSC = atoi(fwMsgToken);
		} 
		else if (msgCount == MSG_INDEX_THREE) 
		{
			/* Append AC file name */
			fwFileNameAC.append(fwMsgToken);
		} 
		else if (msgCount == MSG_INDEX_FOUR)
		{
			/* Append PC file name */
			fwFileNamePC.append(fwMsgToken);
		} 
		else if (msgCount == MSG_INDEX_FIVE)
		{
			/* Append SC file name */
			fwFileNameSC.append(fwMsgToken);
		}

		/* Extract all the Messages based on delimiter */
		fwMsgToken = strtok(NULL, "||");
		/* increment the message count */
		msgCount++;
	}
	
	/* Calculate the size of AC firmware upgrade file */
	if (bIsUpgradeAC == TRUE) 
	{
		fileSizeStatus = ValidateFileSize(SLAVE_IDX_AC, fwFileNameAC);
		 
		 /* Check the file size validation is Success */
		 if (fileSizeStatus != SUCCESS)
		 {
			 return fileSizeStatus;
		 }
	}
	
	/* Calculate the size of PC firmware upgrade file */
	if (bIsUpgradePC == TRUE) 
	{
		fileSizeStatus = ValidateFileSize(SLAVE_IDX_PC, fwFileNamePC);
		 
		/* Check the file size validation is Success */
		 if (fileSizeStatus != SUCCESS)
		 {
			 return fileSizeStatus;
		 }
	}
	
	/* Calculate the size of SC firmware upgrade file */
	if (bIsUpgradeSC == TRUE) 
	{
		fileSizeStatus = ValidateFileSize(MASTER_IDK_SC, fwFileNameSC);
		 
		/* Check the file size validation is Success */
		 if (fileSizeStatus != SUCCESS)
		 {
			 return fileSizeStatus;
		 }
	}
	
	/* Delete all other tasks except SDO, Socket Receiver and FW task */
	/* This is to ensure that no other tasks interrupt firmware upgrade task*/
	DeleteTasks();
	
	UbootVersionFlag = IsNewBootloaderForCRC();
	
	/* Check Firmware Upgrade is enabled for AC */
	if (bIsUpgradeAC == TRUE) 
	{

		/* Trigger Firmware Upgrade for AC */
		LOGDBG("FW_T : ############ AC UPGRADE STARTED ############", 0, 0, 0);

		upgradeStatus = SlaveFirmwareUpgrade(SLAVE_IDX_AC, fileSizeAC,
				(char *) fwFileNameAC.c_str());

		/* Check upgrade is success or failure */
		if (upgradeStatus != SUCCESS) 
		{
			LOGDBG("FW_T : ############ AC UPGRADE FAILED ############", 0, 0,
					0);
			return upgradeStatus;
		}
		else
			DataBaseManager::EnterEventLogNVM (USER_LOGIN, NULL);
	}

	/* Check Firmware Upgrade is enabled for PC */
	if (bIsUpgradePC == TRUE) 
	{

		/* Trigger Firmware Upgrade for PC */
		LOGDBG("FW_T : ############ PC UPGRADE STARTED ############", 0, 0, 0);

		upgradeStatus = SlaveFirmwareUpgrade(SLAVE_IDX_PC, fileSizePC,
				(char *) fwFileNamePC.c_str());

		/* Check upgrade is success or failure */
		if (upgradeStatus != SUCCESS) 
		{
			LOGDBG("FW_T : ############ PC UPGRADE FAILED ############", 0, 0,
					0);
			return upgradeStatus;
		}
		else
			DataBaseManager::EnterEventLogNVM (USER_LOGIN, NULL);
	}

	/* Check Firmware Upgrade is enabled for SC */
	if (bIsUpgradeSC == TRUE)
	{

		/* Trigger Firmware Upgrade for SC */
		LOGDBG("FW_T : ############ SC UPGRADE STARTED ############", 0, 0, 0);
		if(UbootVersionFlag)
		{
			upgradeStatus = MasterFirmwareUpgradeCRC(fileSizeSC,
					(char *) fwFileNameSC.c_str());
		}
		else
		{
			upgradeStatus = MasterFirmwareUpgrade(fileSizeSC,
								(char *) fwFileNameSC.c_str());	
		}

		/* Check upgrade is success or failure */
		if (upgradeStatus != SUCCESS) 
		{
			LOGDBG("FW_T : ############ SC UPGRADE FAILED ############", 0, 0,
					0);
			return upgradeStatus;
		}
		else
			DataBaseManager::EnterEventLogNVM (USER_LOGIN, NULL);
	}

	VersionUpdateToEeprom();
	/* Return success when upgrade is success for all modules */
	return SUCCESS;
}

/**************************************************************************//**
 * \brief   - update the current firmware versions and firmware upgrade status 
 *			  to eeprom
 * \param   - void
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::VersionUpdateToEeprom()
{
	char fwUpgradeFlag = 0x00;
	Eeprom eepromObj;
	char fwVersionSC[SYSINFO_SIZE] = { 0x00 };
	char fwVersionPC[SYSINFO_SIZE] = { 0x00 };
	char fwVersionAC[SYSINFO_SIZE] = { 0x00 };

#ifdef MAINLINE_BUGFIX_BUILD	
	sprintf(fwVersionSC, "%d.%d.%d.%d",
						FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR),
							FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),
								FWVersion::Get(FW_VERSION_SC, VERSION_BUILD), SC_FW_VERSION_AUTOS);
	sprintf(fwVersionPC, "%d.%d.%d.%d",
						FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),
							FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),
								FWVersion::Get(FW_VERSION_PC, VERSION_BUILD), ((FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR) == 2 && FWVersion::Get(FW_VERSION_PC, VERSION_MINOR) == 0) ? 1 : PC_FW_VERSION_AUTOS));
	
	sprintf(fwVersionAC, "%d.%d.%d.%d",
						FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),
							FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),
								FWVersion::Get(FW_VERSION_AC, VERSION_BUILD), AC_FW_VERSION_AUTOS);
#else
	sprintf(fwVersionSC, "%d.%d.%d",
						FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR),
							FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),
								FWVersion::Get(FW_VERSION_SC, VERSION_BUILD));
	sprintf(fwVersionPC, "%d.%d.%d",
						FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),
							FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),
								FWVersion::Get(FW_VERSION_PC, VERSION_BUILD));
	
	sprintf(fwVersionAC, "%d.%d.%d",
						FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),
							FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),
								FWVersion::Get(FW_VERSION_AC, VERSION_BUILD));
#endif
	
	
	/* Write Upgrade Success Flag to EEPROM */
	fwUpgradeFlag = FW_UPGRADE_SUCCESS;
	
	eepromObj.Write((char *) &fwUpgradeFlag, sizeof(fwUpgradeFlag),
													FW_FLAG_EEPROM_OFFSET);  
	
	eepromObj.Write((char *) fwVersionSC, SYSINFO_SIZE,
													SC_FW_VER_EEPROM_OFFSET);
	
	eepromObj.Write((char *) fwVersionPC, SYSINFO_SIZE,
													PC_FW_VER_EEPROM_OFFSET);
	
	eepromObj.Write((char *) fwVersionAC, SYSINFO_SIZE,
													AC_FW_VER_EEPROM_OFFSET); 
		
}

/**************************************************************************//**
 * \brief   - calculate the firmware upgrade file size and check file size
 * 			  is ZERO KB or not
 * \param   - UINT16 iModuleID, string fwUpgrdFile
 *
 * \return  - Success / failure
 *
 ******************************************************************************/
INT8 FirmwareUpgrade::ValidateFileSize(UINT16 iModuleID, string fwUpgrdFile)
{
	UINT32 fwFileSize;
	fwFilePath.clear();
	fwFilePath.append(fwFileDirectory);
	fwFilePath.append(fwUpgrdFile);

	/* Open the Firmware Upgrade File */
	fp = fopen((char *) fwFilePath.c_str(), READ_BINARY);
	if (fp == NULL) 
	{
		LOGDBG("FW_T : UNABLE TO OPEN FILE %s",
				(_Vx_usr_arg_t )fwFilePath.c_str(), 0, 0);
		return FAILURE;
	}

	/* Get file length */
	fseek(fp, ZERO_BYTES, SEEK_END);
	fwFileSize = ftell(fp);
	fseek(fp, ZERO_BYTES, SEEK_SET);

	/* close the file */
	fclose(fp);
	
	/* Check the File size */
	if (fwFileSize == ZERO_BYTES)
	{
		LOGDBG("FW_T : FILE SIZE IS 0KB", 0, 0, 0);
		return FILE_SIZE_ZERO;
	}
	
	/* Assign the calculated file size */
	if (iModuleID == MASTER_IDK_SC)
	{
		fileSizeSC = fwFileSize;
	}
	else if (iModuleID == SLAVE_IDX_PC)
	{
		fileSizePC = fwFileSize;
	}
	else if (iModuleID == SLAVE_IDX_AC)
	{
		fileSizeAC = fwFileSize;
	}
	
	return SUCCESS;
}
/**************************************************************************//**
 * \brief   - Receives the Messages From UIC 
 *
 * \param   - void
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::FirmwareUpgradeMsgHandler(void) 
{

	UINT32 msgLen = 0;
	char *srcBuffer = NULL;
	char pRecvBuffer[MAX_SIZE_OF_MSG_LENGTH] = { 0x00 };
	INT8 processMsgStatus;

	FW_MSG_Q_ID = CP->GetMsgQId(cTaskName[FWUPGRADE_T]);

	/* Listen to the Messages from UIC */
	while (1)
	{
		/* Receive the messages from UIC */
		if (msgQReceive(FW_MSG_Q_ID, pRecvBuffer, MAX_SIZE_OF_MSG_LENGTH,
				WAIT_FOREVER) != ERROR) 
		{

			/* Copy the received messages to Local buffer */
			memcpy(&message, pRecvBuffer, sizeof(message));

			/* Process the messages from UIC based on message ID */
			switch (message.msgID)
			{

			case TO_FWUP_TASK_FW_UPGRADE_CMD_REQ:
				
				/*firmware upgrade in progress*/
				bIsFirmwareUpgrade = true;
				
				/* Set the file directory to HTTP_FILE_DIR */
				fwFileDirectory.clear();
				fwFileDirectory.append(USB_PATH);
				
				/* Process the Message sent from UIC */
				processMsgStatus = ProcessFirmwareMsg(message.Buffer);
				memset(message.Buffer, 0x00, sizeof(message.Buffer));
				if (processMsgStatus == FAILURE) 
				{
					/* Send error message */
					sprintf(message.Buffer, "%u", RESPONSE_FAIL);
					bIsFirmwareUpgrade = false;

				} 
				else if (processMsgStatus == SUCCESS)
				{
					/* Send success message */
					sprintf(message.Buffer, "%u", RESPONSE_SUCCESS);
				}
				else if (processMsgStatus == FILE_SIZE_ZERO)
				{
					/* Send file size zero message */
					sprintf(message.Buffer, "%u", FILE_SIZE_ZERO);
					bIsFirmwareUpgrade = false;
				}

				/* Send the response */
				message.msgID = UIC_FW_UPGRADE_CMD_RES;
				msgLen = strlen(message.Buffer);
				SendToClient(message, msgLen);
				break;

			case TO_FWUP_TASK_FW_UPGRADE_ETHER_REQ:
				bIsFirmwareUpgrade = true;
				/* Set the file directory to HTTP_FILE_DIR */
				fwFileDirectory.clear();
				fwFileDirectory.append(HTTP_FILE_DIR);
							 
				/* Process the Message sent from UIC */
				processMsgStatus = ProcessFirmwareMsg(message.Buffer);
				memset(message.Buffer, 0x00, sizeof(message.Buffer));
				if (processMsgStatus == FAILURE) 
				{
					/* Send error message */
					sprintf(message.Buffer, "%u", RESPONSE_FAIL);
					bIsFirmwareUpgrade = false;	
				} 
				else if (processMsgStatus == SUCCESS)
				{
					/* Send success message */
					sprintf(message.Buffer, "%u", RESPONSE_SUCCESS);
				}
				else if (processMsgStatus == FILE_SIZE_ZERO)
				{
					/* Send file size zero message */
					sprintf(message.Buffer, "%u", FILE_SIZE_ZERO);
					bIsFirmwareUpgrade = false;
				}

				/* Send the response */
				message.msgID = UIC_FW_UPGRADE_CMD_RES;
				msgLen = strlen(message.Buffer);
				SendToClient(message, msgLen);
				break;
				
			default:
				LOGDBG("FW_T : --------Unknown Message ID----------- : ",
						message.msgID, 0, 0);
				break;

			}
		}
	}
}

/**************************************************************************//**
 * \brief   - send the Messages to UIC 
 *
 * \param   - Message& message, UINT32 buffLen
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::SendToClient(Message& message, UINT32 buffLen) 
{
	Client_Message sendBuffer;
	UINT32 iLen = 0;

	/* Send the Messages to UIC */
	memset(sendBuffer.Buffer, 0x00, sizeof(sendBuffer.Buffer));
	sendBuffer.msgID = message.msgID;
	sendBuffer.msglen = buffLen;
	memcpy(sendBuffer.Buffer, message.Buffer, sendBuffer.msglen);
	iLen = sizeof(sendBuffer.msgID) + sizeof(sendBuffer.msglen)
					+ sendBuffer.msglen;
	Interface->Send(reinterpret_cast<char*>(&sendBuffer), iLen);
}

/**************************************************************************//**
 * \brief   - Send the Progress to UIC
 *
 * \param   - UINT32 pResponseVal
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::ProgressResponseToUI(UINT32 pResponseVal) 
{
	UINT32 msgLen = 0;

	/* Send the Progress bar response to UIC */
	message.msgID = UIC_FW_UPGRADE_PROGRESS_IND;
	memset(message.Buffer, 0x00, sizeof(message.Buffer));
	sprintf(message.Buffer, "%d", pResponseVal);
	msgLen = strlen(message.Buffer);
	SendToClient(message, msgLen);
}

/**************************************************************************//**
 * \brief   - Delete all other tasks except SDO , Socket receiver and Firmware
 * 			  upgrade task 
 * \param   - void
 *
 * \return  - void
 *
 ******************************************************************************/
bool FirmwareUpgrade::DeleteTasks()
{
	bool status = true;
	
	/* Get UI task ID */
	UI_TID = CP->GetTaskId(cTaskName[UI_T]);
	if (TASK_ID_NULL != UI_TID) 
	{
		/* Delete UI task */
		if(!taskDelete(UI_TID))
		{
			UI_TID = TASK_ID_NULL;
			CP->SetTaskId(cTaskName[UI_T], UI_TID);
			
		}
		else
		{
			LOGERR("FW_T:Fail to delete UI Task",0,0,0);
			status = false;
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get UI Task",0,0,0);
	}
	
	/* Get DB task ID */
	DB_TID = CP->GetTaskId(cTaskName[DB_T]);
	if (TASK_ID_NULL != DB_TID)
	{
		/* Delete DB task */
		if(!taskDelete(DB_TID))
		{
			DB_TID = TASK_ID_NULL;
			CP->SetTaskId(cTaskName[DB_T], DB_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete DB Task",0,0,0);
			status = false;
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get DB Task",0,0,0);
	}
	
	/* Get UI_DB task ID */
	UI_DB_TID = CP->GetTaskId(cTaskName[UI_DB_T]);
	if (TASK_ID_NULL != UI_DB_TID) 
	{
		/* Delete UI_DB task */
		if(!taskDelete(UI_DB_TID))
		{
			UI_DB_TID = TASK_ID_NULL;
			CP->SetTaskId(cTaskName[UI_DB_T], UI_DB_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete UIDB Task",0,0,0);
			status = false;
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get UIDB Task",0,0,0);
	}
	
	/* Get ALARM task ID */
	ALARM_TID = CP->GetTaskId(cTaskName[ALARM_T]);
	if (TASK_ID_NULL != ALARM_TID) 
	{
		/* Delete ALARM task */
		if(!taskDelete(ALARM_TID))
		{
			ALARM_TID = TASK_ID_NULL;
			CP->SetTaskId(cTaskName[ALARM_T], ALARM_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete ALARM Task",0,0,0);
			status = false;
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get ALARM Task",0,0,0);
	}
	
	/* Get BARCODE READER task ID */
	BARCODE_TID = CP->GetTaskId(cTaskName[BARCODE_READER_T]);
	if (TASK_ID_NULL != BARCODE_TID) 
	{
		/* Delete BARCODE READER task */
		if(!taskDelete(BARCODE_TID))
		{
			BARCODE_TID = TASK_ID_NULL;
			CP->SetTaskId(cTaskName[BARCODE_READER_T], BARCODE_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete BARCODE Task",0,0,0);
			status = false;
		}
		
	}
	else
	{
		LOGERR("FW_T:Fail to get BARCODE Task",0,0,0);
	}
	
	/* Get DIGITAL INPUT task ID */
	DGTIN_TID = CP->GetTaskId(cTaskName[DGTIN_T]);
	if (TASK_ID_NULL != DGTIN_TID) 
	{
		/* Delete DIGITAL INPUT task */
		if(!taskDelete(DGTIN_TID))
		{
			DGTIN_TID = TASK_ID_NULL;
			CP->SetTaskId(cTaskName[DGTIN_T], DGTIN_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete DIGITAL input Task",0,0,0);
			status = false;
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get DIGITAL input Task",0,0,0);
	}
	
	/* Get DIGITAL OUTPUT task ID */
	DGTOUT_TID = CP->GetTaskId(cTaskName[DGTOUT_T]);
	if (TASK_ID_NULL != DGTOUT_TID) 
	{
		/* Delete DIGITAL OUTPUT task */
		if(!taskDelete(DGTOUT_TID))
		{
			DGTOUT_TID = TASK_ID_NULL;
			CP->SetTaskId(cTaskName[DGTOUT_T], DGTOUT_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete DIGITAL output Task",0,0,0);
			status = false;
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get DIGITAL output Task",0,0,0);
	}
	
	/* Get IO_IMAGE task ID */
	IO_Image_TID = CP->GetTaskId(cTaskName[IO_IMAGE_HANDLER_T]);
	if (TASK_ID_NULL != IO_Image_TID) 
	{
		/* Delete IO_IMAGE task */
		if(!taskDelete(IO_Image_TID))
		{
			IO_Image_TID = TASK_ID_NULL;
			CP->SetTaskId(cTaskName[IO_IMAGE_HANDLER_T], IO_Image_TID);
			printf("\n%sDELETED TASK IO_IMAGE%s\n","\x1B[31m", "\x1B[0m");
		}
		else
		{
			LOGERR("FW_T:Fail to delete IO_IMAGE Task",0,0,0);
			status = false;
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get IO_IMAGE Task",0,0,0);
	}
	
	/* Get MAIL_BOX task ID */
	Mail_Box_TID = CP->GetTaskId(cTaskName[MAIL_BOX_HANDLER_T]);
	if (TASK_ID_NULL != Mail_Box_TID) 
	{
		/* Delete MAIL_BOX task */
		if(!taskDelete(Mail_Box_TID))
		{
			Mail_Box_TID = TASK_ID_NULL;
			CP->SetTaskId(cTaskName[MAIL_BOX_HANDLER_T], Mail_Box_TID);
			printf("\n%sDELETED TASK MAIL_BOX%s\n","\x1B[31m", "\x1B[0m");
		}
		else
		{
			LOGERR("FW_T:Fail to delete MAIL_BOX Task",0,0,0);
			status = false;
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get MAIL_BOX Task",0,0,0);
	}
	
	return status;
}

/**************************************************************************//**
 * \brief   - Task Function Routine called when task spawned
 *
 * \param   - void
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade_Task(void)
{
	/* Call the message handler function to process the Message received from UIC */
	FirmwareUpgrade *FwUpgradeObj = new FirmwareUpgrade();
	FwUpgradeObj->FirmwareUpgradeMsgHandler();
}

/**************************************************************************//**
 * \brief   - Calculates CRC of Firmware Upgrade Files
 *
 * \param   - const unsigned char *dataBuffer, UINT32 buffLen
 *
 * \return  - UINT16 - CRC value
 *
 ******************************************************************************/
unsigned short FirmwareUpgrade::crc16_ccitt(const void *buf, int len)
{

	int counter;
	unsigned short crc = 0;
	for (counter = 0; counter < len; counter++) 
	{
		crc = (crc << CRC_BIT_SHIFT)
			^ crc16tab[((crc >> CRC_BIT_SHIFT) ^ *(char *) buf) & 0x00FF];
		buf = ((char *) buf) + 1;
	}
	return crc;
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
 * \brief   - Process the responses received from Slave AC
 *
 * \param   - void
 *
 * \return  - STATUS - Success / Failure
 *
 ******************************************************************************/
STATUS FirmwareUpgrade::ProgressResponseFromAC(void) 
{
	UINT32 msgLen = 0;

	/* Check whether Upgrade success response sent by AC */
	while (fwUpgrdInfoAC.Progress != SLAVE_PROGRESS_RESP_FIVE) 
	{
		/* Get the FW Upgrade  information from AC through SDO task  */
		message.msgID = TO_SDO_TASK_FW_UPGRADE_INFO_REQ_AC;
		memset(message.Buffer, 0x00, sizeof(message.Buffer));
		msgLen = strlen(message.Buffer);
		SendToMsgQ(message, SDO_MSG_Q_ID);
		taskDelay(HALF_SEC_DELAY);

		/* Check the response  sent by AC */
		if (fwUpgrdInfoAC.Progress == SLAVE_PROGRESS_RESP_TWO)
		{
			/* Check the CRC calculated by Master and Sent by AC is matching */
			if (fwUpgrdInfoAC.CRC_CheckSum == fwUpgrdCRC)
			{
				fwUpgrdCRC = 0;
				LOGDBG("FW_T : CRC MATCH FOUND", 0, 0, 0);
				/* Send the CRC matching response to UIC */
				ProgressResponseToUI(AC_PROGRESS_TWO);

				/* send the CRC matching status to slave AC */
				memset(&fwUpgrdInfoAC, 0x00, sizeof(FirmWareUpgradeInfo));
				fwUpgrdInfoAC.Upgrade_Status = CRC_VALIDATION_SUCCESS;
				message.msgID = TO_SDO_TASK_FW_UPGRADE_STATUS_AC;
				memset(message.Buffer, 0x00, sizeof(message.Buffer));
				msgLen = strlen(message.Buffer);
				SendToMsgQ(message, SDO_MSG_Q_ID);
				taskDelay(HALF_SEC_DELAY);

			} 
			else 
			{
				fwUpgrdCRC = 0;
				LOGDBG("FW_T : CRC MATCH FAILED", 0, 0, 0);
				/* Clear the firmware upgrade info buffer */
				memset(&fwUpgrdInfoAC, 0x00, sizeof(FirmWareUpgradeInfo));
				return ERROR;
			}
		}

		else if (fwUpgrdInfoAC.Progress == SLAVE_PROGRESS_RESP_THREE) 
		{
			LOGDBG("FW_T : ERASING THE FLASH", 0, 0, 0);
			/* Send the Response to UIC */
			ProgressResponseToUI(AC_PROGRESS_THREE);

		}
		else if (fwUpgrdInfoAC.Progress == SLAVE_PROGRESS_RESP_FOUR)
		{
			LOGDBG("FW_T : WRITING TO FLASH", 0, 0, 0);
			/* Send the Response to UIC */
			ProgressResponseToUI(AC_PROGRESS_FOUR);

		}
		else if (fwUpgrdInfoAC.Progress == SLAVE_PROGRESS_RESP_ERROR)
		{
			/* If Upgrade failure response sent by AC, return error */
			LOGDBG("FW_T : AC UPGRADE FAILED RESP", 0, 0, 0);
			/* Clear the firmware upgrade info buffer */
			memset(&fwUpgrdInfoAC, 0x00, sizeof(FirmWareUpgradeInfo));
			return ERROR;
		}
	}

	/* If Upgrade Success is sent by AC */
	if (fwUpgrdInfoAC.Progress == SLAVE_PROGRESS_RESP_FIVE) 
	{
		LOGDBG("FW_T : WRITING TO FLASH COMPLETED", 0, 0, 0);
		/* Send the AC Upgrade response to UIC */
		ProgressResponseToUI(AC_PROGRESS_FIVE);
		/* Clear the firmware upgrade info buffer */
		memset(&fwUpgrdInfoAC, 0x00, sizeof(FirmWareUpgradeInfo));
	}

	LOGDBG("FW_T : ############ AC UPGRADE COMPLETED ############", 0, 0, 0);
	return OK;
}

/**************************************************************************//**
 * \brief   - Process the responses received from Slave PC
 *
 * \param   - void
 *
 * \return  - STATUS - Success / Failure
 *
 ******************************************************************************/
STATUS FirmwareUpgrade::ProgressResponseFromPC(void)
{
	UINT32 msgLen = 0;

	/* Check whether Upgrade success response sent by PC */
	while (fwUpgrdInfoPC.Progress != SLAVE_PROGRESS_RESP_FIVE)
	{
		/* Get the FW Upgrade  information from PC through SDO task  */
		message.msgID = TO_SDO_TASK_FW_UPGRADE_INFO_REQ_PC;
		memset(message.Buffer, 0x00, sizeof(message.Buffer));
		msgLen = strlen(message.Buffer);
		SendToMsgQ(message, SDO_MSG_Q_ID);
		taskDelay(HALF_SEC_DELAY);

		/* Check the response  sent by PC */
		if (fwUpgrdInfoPC.Progress == SLAVE_PROGRESS_RESP_TWO)
		{
			/* Check the CRC calculated by Master and Sent by PC is matching */
			if (fwUpgrdInfoPC.CRC_CheckSum == fwUpgrdCRC)
			{
				fwUpgrdCRC = 0;
				LOGDBG("FW_T : CRC MATCH FOUND", 0, 0, 0);
				/* Send the CRC matching response to UIC */
				ProgressResponseToUI(PC_PROGRESS_TWO);

				/* send the CRC matching status to slave PC */
				memset(&fwUpgrdInfoPC, 0x00, sizeof(FirmWareUpgradeInfo));
				fwUpgrdInfoPC.Upgrade_Status = CRC_VALIDATION_SUCCESS;
				message.msgID = TO_SDO_TASK_FW_UPGRADE_STATUS_PC;
				memset(message.Buffer, 0x00, sizeof(message.Buffer));
				msgLen = strlen(message.Buffer);
				SendToMsgQ(message, SDO_MSG_Q_ID);
				taskDelay(HALF_SEC_DELAY);

			} 
			else 
			{
				fwUpgrdCRC = 0;
				LOGDBG("FW_T : CRC MATCH FAILED", 0, 0, 0);
				/* Clear the firmware upgrade info buffer */
				memset(&fwUpgrdInfoPC, 0x00, sizeof(FirmWareUpgradeInfo));
				return ERROR;
			}
		}

		else if (fwUpgrdInfoPC.Progress == SLAVE_PROGRESS_RESP_THREE) 
		{
			LOGDBG("FW_T : ERASING THE FLASH", 0, 0, 0);
			/* Send the Response to UIC */
			ProgressResponseToUI(PC_PROGRESS_THREE);

		} 
		else if (fwUpgrdInfoPC.Progress == SLAVE_PROGRESS_RESP_FOUR)
		{
			LOGDBG("FW_T : WRITING TO FLASH", 0, 0, 0);
			/* Send the Response to UIC */
			ProgressResponseToUI(PC_PROGRESS_FOUR);

		} 
		else if (fwUpgrdInfoPC.Progress == SLAVE_PROGRESS_RESP_ERROR)
		{
			/* If Upgrade failure response sent by AC, return error */
			LOGDBG("FW_T : PC UPGRADE FAILED RESP", 0, 0, 0);
			/* Clear the firmware upgrade info buffer */
			memset(&fwUpgrdInfoPC, 0x00, sizeof(FirmWareUpgradeInfo));
			return ERROR;
		}

	}

	/* If Upgrade Success is sent by PC */
	if (fwUpgrdInfoPC.Progress == SLAVE_PROGRESS_RESP_FIVE) 
	{
		LOGDBG("FW_T : WRITING TO FLASH COMPLETED", 0, 0, 0);
		/* Send the PC Upgrade response to UIC */
		ProgressResponseToUI(PC_PROGRESS_FIVE);
		/* Clear the firmware upgrade info buffer */
		memset(&fwUpgrdInfoPC, 0x00, sizeof(FirmWareUpgradeInfo));
	}

	LOGDBG("FW_T : ############ PC UPGRADE COMPLETED ############", 0, 0, 0);
	return OK;
}

/**************************************************************************//**
 * \brief   - Read the SC upgrade file and Write to QSPI 
 *
 * \param   - UINT32 fwFileSizeSC, char *fwUpgrdFileSc
 *
 * \return  - INT8 - Success / Failure or Error values
 *
 ******************************************************************************/
INT8 FirmwareUpgrade::MasterFirmwareUpgrade(UINT32 fwFileSizeSC,
		char *fwUpgrdFileSc)
{
	INT8 ret = 0;
	UINT32 offsetAddr;
	FirmwarePartitionInfo fwPartInfostruct;
	FirmwarePartitionInfo *fwPartInfo = &fwPartInfostruct;
	Eeprom epclass;
	bool ReadEeprom = false;
	
	/* Add the file Name to the path */
	fwFilePath.clear();
	fwFilePath.append(fwFileDirectory);
	fwFilePath.append(fwUpgrdFileSc);
	
	/* Open the Firmware Upgrade File */
	fp = fopen((char *) fwFilePath.c_str(), READ_BINARY);
	if (fp == NULL) 
	{
		LOGDBG("FW_T : UNABLE TO OPEN FILE %s",
				(_Vx_usr_arg_t )fwFilePath.c_str(), 0, 0);
		return FAILURE;
	}
	
	/* Send the Response to UIC */
	ProgressResponseToUI(SC_PROGRESS_ONE);

	/* Allocate memory */
	pBuf = (char *) malloc(fwFileSizeSC);
	if (pBuf == NULL) {
		LOGDBG("FW_T : MEMMORY ALLOCATION FAILED", 0, 0, 0);
		fclose(fp);
		return FAILURE;
	}

	LOGDBG("FW_T : READING %d BYTES FROM FILE : %s", fwFileSizeSC,
			(_Vx_usr_arg_t )fwUpgrdFileSc, 0);

	/* Read file contents into pBuf */
	bytesRead = fread(pBuf, fwFileSizeSC, READ_BYTE, fp);

	if ((bytesRead == ZERO_BYTES) && (ferror(fp)))
	{
		LOGDBG("FW_T : UANBLE TO READ SC FIRMWARE UPGRADATION FILE", 0, 0, 0);
		/* free the allocated memory */
		if (pBuf != NULL)
		{
			free(pBuf);
			pBuf = NULL;
		}
		fclose(fp);
		return FAILURE;
	}

	/* Close the file once data is copied */
	fclose(fp);

	/* Send the Response to UIC */
	ProgressResponseToUI(SC_PROGRESS_TWO);

	/* Read the Firmware Part Info from Qspi */
	ret = QSPIFlashRead(FW_PARTITION_INFO_QSPI_OFFSET,
			sizeof(FirmwarePartitionInfo), (uint32_t) fwPartInfo);

	if (ret == FAILURE) 
	{
		LOGERR("FW_T : READING PARTITION INFO FROM QSPI FAILED ", 0, 0, 0);
		ReadEeprom = true;
	}

	/* check part info is corrupted or not */
	if ((ReadEeprom) || ((fwPartInfo->activePart != ACTIVE_PART_FIRMWARE_START)
			&& (fwPartInfo->activePart != PASSIVE_PART_FIRMWARE_START)))
	{

		/*if corrupted , get info from EEPROM */
		ret = epclass.Read((char*) fwPartInfo, sizeof(FirmwarePartitionInfo),
				FW_PARTITION_INFO_EEPROM_OFFSET);

		/* Reset data if no data available */
		if ((ret < ZERO_BYTES)
				|| ((fwPartInfo->activePart != ACTIVE_PART_FIRMWARE_START)
					&& (fwPartInfo->activePart != PASSIVE_PART_FIRMWARE_START))) 
		{
			LOGDBG("FW_T : RESET PARTITION INFO ", 0, 0, 0);
			fwPartInfo->bootMode = ACTIVE_BOOTMODE;
			fwPartInfo->fwUpgrade = FIRMWARE_UPGRADE_FALSE;
			fwPartInfo->activePart = ACTIVE_PART_FIRMWARE_START;
			fwPartInfo->passivePart = PASSIVE_PART_FIRMWARE_START;
		}
	}

	/* Get the Offset Value */
	offsetAddr = fwPartInfo->passivePart;

	/* Send the Response to UIC */
	ProgressResponseToUI(SC_PROGRESS_THREE);

	LOGDBG("FW_T : ERASING %d BYTES From 0x%08x", (_Vx_usr_arg_t )fwFileSizeSC,
			(_Vx_usr_arg_t )offsetAddr, 0);

	/* Erase the QSPI blocks */
	QSPIFlashErase(offsetAddr, fwFileSizeSC);

	/* Send the flash writing response to UIC */
	ProgressResponseToUI(SC_PROGRESS_FOUR);

	LOGDBG("FW_T : WRITING BINARY AT 0x%08x", (_Vx_usr_arg_t )offsetAddr, 0, 0);

	/* Write file image to QSPI flash */
	ret = QSPIFlashWrite(offsetAddr, fwFileSizeSC, (uint32_t) pBuf);

	if (ret == FAILURE) 
	{
		LOGCRIT("FW_T : WRITING BINARY TO FLASH FAILED", 0, 0, 0);
	}

	LOGDBG("FW_T : WRITING BINARY TO FLASH COMPLETED", 0, 0, 0);

	/* free the allocated memory for data */
	if (pBuf != NULL)
	{
		free(pBuf);
		pBuf = NULL;
	}

	/* Change the Normal boot mode */
	fwPartInfo->fwUpgrade = FIRMWARE_UPGRADE_TRUE;

	LOGDBG("FW_T : WRITING UPDATED FIRMWARE PARTITION INFO TO EEPROM", 0, 0, 0);

	/* Write the firmware part info data to EEPROM */
	ret = epclass.Write((char *) fwPartInfo, sizeof(FirmwarePartitionInfo),
			FW_PARTITION_INFO_EEPROM_OFFSET);

	if (ret < ZERO_BYTES)
	{
		LOGERR("FW_T : WRITING FIRMWARE PART INFO TO EEPROM FAILED", 0, 0, 0);
	}
	
	/* Erase the firmware part info data  before writing */
	QSPIFlashErase(FW_PARTITION_INFO_QSPI_OFFSET, sizeof(FirmwarePartitionInfo));

	LOGDBG("FW_T : WRITING UPDATED FIRMWARE PARTITION INFO TO QSPI FLASH", 0, 0, 0);

	/* Write the firmware part info data to QSPI */
	ret = QSPIFlashWrite(FW_PARTITION_INFO_QSPI_OFFSET, sizeof(FirmwarePartitionInfo),
			(uint32_t) fwPartInfo);

	if (ret == FAILURE)
	{
		LOGCRIT("FW_T : WRITING FIRMWARE PART INFO TO QSPI FLASH FAILED", 0, 0, 0);
		return FAILURE;
	}

	/* Send the Upgrade success response to UIC */
	ProgressResponseToUI(SC_PROGRESS_FIVE);
	
	LOGDBG("FW_T : ############ SC UPGRADE COMPLETED ############", 0, 0, 0);
	return SUCCESS;
}

/**************************************************************************//**
 * \brief   - Read the SC upgrade file and Write to QSPI with CRC update
 *
 * \param   - UINT32 fwFileSizeSC, char *fwUpgrdFileSc
 *
 * \return  - INT8 - Success / Failure or Error values
 *
 ******************************************************************************/
INT8 FirmwareUpgrade::MasterFirmwareUpgradeCRC(UINT32 fwFileSizeSC,
		char *fwUpgrdFileSc)
{
	INT8 ret = 0;
	UINT32 offsetAddr;
	FirmwarePartitionInfoCRC fwPartInfostruct;
	FirmwarePartitionInfoCRC *fwPartInfo = &fwPartInfostruct;
	Eeprom epclass;
	bool ReadEeprom = false;
	
	/* Add the file Name to the path */
	fwFilePath.clear();
	fwFilePath.append(USB_PATH);
	fwFilePath.append(fwUpgrdFileSc);
	
	/* Open the Firmware Upgrade File */
	fp = fopen((char *) fwFilePath.c_str(), READ_BINARY);
	if (fp == NULL) 
	{
		LOGDBG("FW_T : UNABLE TO OPEN FILE %s",
				(_Vx_usr_arg_t )fwFilePath.c_str(), 0, 0);
		return FAILURE;
	}
	
	/* Send the Response to UIC */
	ProgressResponseToUI(SC_PROGRESS_ONE);

	/* Allocate memory */
	pBuf = (char *) malloc(fwFileSizeSC);
	if (pBuf == NULL) {
		LOGDBG("FW_T : MEMMORY ALLOCATION FAILED", 0, 0, 0);
		fclose(fp);
		return FAILURE;
	}

	LOGDBG("FW_T : READING %d BYTES FROM FILE : %s", fwFileSizeSC,
			(_Vx_usr_arg_t )fwUpgrdFileSc, 0);

	/* Read file contents into pBuf */
	bytesRead = fread(pBuf, fwFileSizeSC, READ_BYTE, fp);

	if ((bytesRead == ZERO_BYTES) && (ferror(fp)))
	{
		LOGDBG("FW_T : UANBLE TO READ SC FIRMWARE UPGRADATION FILE", 0, 0, 0);
		/* free the allocated memory */
		if (pBuf != NULL)
		{
			free(pBuf);
			pBuf = NULL;
		}
		fclose(fp);
		return FAILURE;
	}

	/* Close the file once data is copied */
	fclose(fp);

	/* Send the Response to UIC */
	ProgressResponseToUI(SC_PROGRESS_TWO);

	/* Read the Firmware Part Info from Qspi */
	ret = QSPIFlashRead(FW_PARTITION_INFO_QSPI_OFFSET_CRC,
			sizeof(FirmwarePartitionInfoCRC), (uint32_t) fwPartInfo);

	if (ret == FAILURE) 
	{
		LOGERR("FW_T : READING PARTITION INFO FROM QSPI FAILED ", 0, 0, 0);
		ReadEeprom = true;
	}

	/* check part info is corrupted or not */
	if ((ReadEeprom) || ((fwPartInfo->partition_a_offset != ACTIVE_PART_FIRMWARE_START)
			&& (fwPartInfo->partition_a_offset != PASSIVE_PART_FIRMWARE_START)))
	{

		/*if corrupted , get info from EEPROM */
		ret = epclass.Read((char*) fwPartInfo, sizeof(FirmwarePartitionInfoCRC),
				FW_PARTITION_INFO_EEPROM_OFFSET);

		/* Reset data if no data available */
		if ((ret < ZERO_BYTES)
				|| ((fwPartInfo->partition_a_offset != ACTIVE_PART_FIRMWARE_START)
					&& (fwPartInfo->partition_a_offset != PASSIVE_PART_FIRMWARE_START))) 
		{
			LOGDBG("FW_T : RESET PARTITION INFO ", 0, 0, 0);
			fwPartInfo->boot_status = ACTIVE_BOOTMODE;
			fwPartInfo->fw_upgrade = FIRMWARE_UPGRADE_FALSE;
			fwPartInfo->partition_a_offset = ACTIVE_PART_FIRMWARE_START;
			fwPartInfo->partition_b_offset = PASSIVE_PART_FIRMWARE_START;
		}
	}

	/* Get the Offset Value */
	offsetAddr = fwPartInfo->partition_b_offset;

	/* Send the Response to UIC */
	ProgressResponseToUI(SC_PROGRESS_THREE);

	LOGDBG("FW_T : ERASING %d BYTES From 0x%08x", (_Vx_usr_arg_t )fwFileSizeSC,
			(_Vx_usr_arg_t )offsetAddr, 0);

	/* Erase the QSPI blocks */
	QSPIFlashErase(offsetAddr, fwFileSizeSC);

	/* Send the flash writing response to UIC */
	ProgressResponseToUI(SC_PROGRESS_FOUR);

	LOGDBG("FW_T : WRITING BINARY AT 0x%08x", (_Vx_usr_arg_t )offsetAddr, 0, 0);

	/* Write file image to QSPI flash */
	ret = QSPIFlashWrite(offsetAddr, fwFileSizeSC, (uint32_t) pBuf);

	if (ret == FAILURE) 
	{
		LOGCRIT("FW_T : WRITING BINARY TO FLASH FAILED", 0, 0, 0);
	}

	LOGDBG("FW_T : WRITING BINARY TO FLASH COMPLETED", 0, 0, 0);
	if(offsetAddr == ACTIVE_PART_FIRMWARE_START)
	{
		fwPartInfo->partition_a_crc = ByteCRC((char *) pBuf, fwFileSizeSC);
		fwPartInfo->partition_a_size = fwFileSizeSC;
	}
	else if(offsetAddr == PASSIVE_PART_FIRMWARE_START)
	{
		fwPartInfo->partition_b_crc = ByteCRC((char *) pBuf, fwFileSizeSC);
		fwPartInfo->partition_b_size = fwFileSizeSC;
	}
	/* Change the Normal boot mode */
	fwPartInfo->fw_upgrade = FIRMWARE_UPGRADE_TRUE;
	fwPartInfo->emmc_copy = 0x0;
	
	LOGDBG("FW_T : WRITING UPDATED FIRMWARE PARTITION INFO TO EEPROM", 0, 0, 0);
	/* free the allocated memory for data */
	if (pBuf != NULL)
	{
		free(pBuf);
		pBuf = NULL;
	}
	/* Write the firmware part info data to EEPROM */
	ret = epclass.Write((char *) fwPartInfo, sizeof(FirmwarePartitionInfoCRC),
			FW_PARTITION_INFO_EEPROM_OFFSET);

	if (ret < ZERO_BYTES)
	{
		LOGERR("FW_T : WRITING FIRMWARE PART INFO TO EEPROM FAILED", 0, 0, 0);
	}
	

	/* Erase the firmware part info data  before writing */
	QSPIFlashErase(FW_PARTITION_INFO_QSPI_OFFSET_CRC, sizeof(FirmwarePartitionInfoCRC));

	LOGDBG("FW_T : WRITING UPDATED FIRMWARE PARTITION INFO TO QSPI FLASH", 0, 0, 0);

	/* Write the firmware part info data to QSPI */
	ret = QSPIFlashWrite(FW_PARTITION_INFO_QSPI_OFFSET_CRC, sizeof(FirmwarePartitionInfoCRC),
			(uint32_t) fwPartInfo);

	if (ret == FAILURE)
	{
		LOGCRIT("FW_T : WRITING FIRMWARE PART INFO TO QSPI FLASH FAILED", 0, 0, 0);
		return FAILURE;
	}

	/* Send the Upgrade success response to UIC */
	ProgressResponseToUI(SC_PROGRESS_FIVE);
	
	LOGDBG("FW_T : ############ SC UPGRADE COMPLETED ############", 0, 0, 0);
	return SUCCESS;
}

/**************************************************************************//**
 * \brief   - De-Initialize allocated stack memory of EcSDOTask
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
FirmwareUpgrade::~FirmwareUpgrade() 
{

	CP = NULL;
	Interface = NULL;
}
