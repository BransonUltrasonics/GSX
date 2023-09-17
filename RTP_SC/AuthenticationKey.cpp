/**********************************************************************************************************

` Copyright (c) Branson Ultrasonics Corporation, 1996-2018

 This program is the property of Branson Ultrasonics Corporation
 Copying of this software is expressly forbidden, without the prior
 written consent of Branson Ultrasonics Corporation.


 ---------------------------- MODULE DESCRIPTION ----------------------------

 Authentication key EEprom class functions for Authentication key read and write  

 **********************************************************************************************************/
#include "AuthenticationKey.h"
#include "commons.h"
#include "DataBaseManager.h"

/**************************************************************************//**
 * \brief   - Initialization of class members and memory for object.
 *
 * \param   - None.
 *
 * \return  - None
 *
 ******************************************************************************/
AuthenticationKey::AuthenticationKey() 
{
	CP = CommonProperty::getInstance();
	Interface = CommunicateInterface::getinstance();
}

/**************************************************************************//**
 * \brief   - Check whether USB is connected or not ,if connected read Private
 * 			  key files
 * \param   - void
 *
 * \return  - char * - FileNames / KEY_USB_READ_FAIL
 *
 ******************************************************************************/
char * AuthenticationKey::USBDetect() 
{
	int fd = 0;

	/* open the USB */
	fd = open(USB_PATH, O_RDONLY);

	/* check Whether it is connected or not */
	if (fd < 0) 
	{
		LOGDBG("AUTH_KEY : USB IS NOT DETECTED  %s", (_Vx_usr_arg_t )USB_PATH, 0,
				0);
		/* send USB Read Fail Message to UI */
		memset(pErrorBuffer, 0x00, ERROR_BUFF_SIZE);
		sprintf(pErrorBuffer, "%u", KEY_USB_READ_FAIL);
		return pErrorBuffer;
	} 
	else 
	{
		LOGDBG("AUTH_KEY : USB DETECTED  %s", (_Vx_usr_arg_t )USB_PATH, 0, 0);
		close(fd);
		/* Read the private key files From USB */
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
char * AuthenticationKey::USBRead()
{
	char fileBuffer[FILE_NAME_MAX_SIZE] = { 0x00 };
	INT32 fwFileNameRead = 1;
	UINT32 fileListError = 0;
	int fd = 0;

	/* change the default directory to USB */
	LOGDBG("AUTH_KEY : CHANGING THE DIRECTORY TO USB", 0, 0, 0);
	usbReadStatus = chdir(USB_PATH);

	/* Check Whether USB is Opened */
	if (usbReadStatus == ERROR) 
	{
		LOGDBG("AUTH_KEY : FAILED TO OPEN USB", 0, 0, 0);
		return NULL;
	}

	/* Open Cache File to store private key File List */
	fp = fopen( CACHE_FILE_PK, RDWR_CREAT);
	if (fp == NULL) 
	{
		LOGDBG("AUTH_KEY : UNABLE TO CREATE CACHE", 0, 0, 0);
		return NULL;
	}

	/* create File descriptor from File pointer */
	fd = fileno(fp);

	/* generate a long listing of directory contents */

	/* Read private key File names */
	usbReadStatus = dirList(fd, PRIVTAE_KEY_FILE_NAME_FORMAT, FALSE, FALSE);
	if (usbReadStatus == ERROR)
	{
		LOGDBG("AUTH_KEY : UNABLE TO READ PRIVATE KEY FILES FROM USB", 0, 0, 0);
		fileListError ++;
	}

	/* Check Error Occurred While Listing File Names */
	if (fileListError) 
	{
		LOGDBG("AUTH_KEY : UNABLE TO LIST FW UPGRADE FILES FROM USB", 0, 0, 0);
		/* close and remove cache file */
		fclose(fp);
		if (OK != remove(CACHE_FILE_PK))
		{
			LOGDBG("AUTH_KEY : UNABLE TO REMOVE CACHE FILE", 0, 0, 0);
		}
		return NULL;
	}

	/* seek to start of the file */
	fseek(fp, ZERO_BYTES, SEEK_SET);

	/* If files are present send the file names to UIC */
	/* clear each module buffer */
	strFileNames.clear();

	/* Clear the file Count of each module */
	m_fileCount = 0;

	/* Read the private key file names from cache file */
	while (fwFileNameRead > ZERO_BYTES) 
	{
		/* Clear the buffer before storing */
		memset(fileBuffer, '\0', sizeof(char) * FILE_NAME_MAX_SIZE);

		/* Read the file names from cache file */
		fwFileNameRead = fscanf(fp, "%s", fileBuffer);

		/* Check file count */
		if (m_fileCount < PRIVATEKEY_FILE_COUNT_MAX && sizeof(fileBuffer) > ZERO_BYTES) 
		{
			/* Append to private key File name Buffer */
			strFileNames.append(fileBuffer);
			strFileNames.append(",");
			m_fileCount++;
		}
	}
	if(m_fileCount)
	{
		strFileNames.erase(strFileNames.end() - 1);
	}
	/* close and remove cache file */
	fclose(fp);

	if (OK != remove(CACHE_FILE_PK))
	{
		LOGDBG("AUTH_KEY : UNABLE TO REMOVE CACHE FILE", 0, 0, 0);
	}
	/* Check whether Files are present or not */
	if (strFileNames.empty()) 
	{
		LOGDBG("AUTH_KEY : NO FILES PRESENT IN USB USB", 0, 0, 0);
		/* Send No Files Present Message to UI */
		sprintf(pErrorBuffer, "%u", KEY_NO_FILES_PRESENT);
		return pErrorBuffer;
	}
	LOGDBG("AUTH_KEY : FW UPGRADE FILES :- %s\n", (_Vx_usr_arg_t )strFileNames.c_str(), 0, 0);
	/* Send File names to UIC */
	return (char *) strFileNames.c_str();
}

/**************************************************************************//**
 * \brief   - Detect the USB and Read the private key files and send to UI
 *
 * \param   - void
 *
 * \return  - void
 *
 ******************************************************************************/
void AuthenticationKey::ReadUSBResponseForPrivateKeyToUI() 
{
	char *srcBuffer = NULL;
	UINT32 iLen = 0;
	USBResponseMsg respMsg;

	memset(respMsg.Buffer, 0x00, READ_USB_RESP_MSG_BUFFER_MAX_SIZE);
	/* Call your function and copy file names to socket buffer */
	srcBuffer = USBDetect();
	if (srcBuffer != NULL) 
	{
		memcpy(respMsg.Buffer, srcBuffer, READ_USB_RESP_MSG_BUFFER_MAX_SIZE);
		respMsg.Buffer[READ_USB_RESP_MSG_BUFFER_MAX_SIZE-1] = 0;
	} 
	else 
	{
		/* Send error message when error occurs */
		sprintf(respMsg.Buffer, "%u", RESPONSE_FAIL);
	}

	/* Send message to UIC */
	respMsg.msgID = UIC_PRIVATE_KEY_RES;
	respMsg.msglen = strlen(respMsg.Buffer);
	iLen = sizeof(respMsg.msgID) + sizeof(respMsg.msglen) + respMsg.msglen;

	Interface->Send(reinterpret_cast<char*>(&respMsg), iLen);
}

/**************************************************************************//**
 * \brief   - Read private from selected file and send response to UI
 * 
 * \param   - char *fileName
 *
 * \return  - None
 *
 ******************************************************************************/
void AuthenticationKey::ReadPrivateKeyFromFile(char *fileName)
{
	INT8 fileStatus = 0;
	strFileNames.clear();
	strFileNames.append(fileName);
	string details;
	
	fileStatus = ValidateFileSizeAndStoreInEEPROM(strFileNames);
	
	// send it to the event log
	if (fileStatus == KEY_SUCCESS)
	{
		details = "Identifier:Authentication Key Loaded:From:N/A:To:N/A:";
		DataBaseManager::EnterEventLog (WEB_SERVICES, details);
	}

	SendResponseToUI(fileStatus);
}

/**************************************************************************//**
 * \brief   - calculate the private key file size and check file size
 * 			  is ZERO KB or not, reverse the private key and Store it in EEPROM
 * \param   - string privateKeyFile
 *
 * \return  - INT8 Success / failure
 *
 ******************************************************************************/
INT8 AuthenticationKey::ValidateFileSizeAndStoreInEEPROM(string &privateKeyFile)
{
	UINT32 dwFileSize,bytesRead;
	pkFilePath.clear();
	pkFilePath.append(USB_PATH);
	pkFilePath.append(privateKeyFile);
	int dwFirst,dwLast,index = 0;
	char *temp;
	char pPrivateKey[33] = {0};

	/* Open the private key File */
	fp = fopen((char *) pkFilePath.c_str(), READ_BINARY);
	if (fp == NULL) 
	{
		LOGDBG("AUTH_KEY : UNABLE TO OPEN FILE %s",
				(_Vx_usr_arg_t )pkFilePath.c_str(), 0, 0);
		return KEY_FILE_OPEN_ERROR;
	}

	/* Get file length */
	fseek(fp, ZERO_BYTES, SEEK_END);
	dwFileSize = ftell(fp);
	fseek(fp, ZERO_BYTES, SEEK_SET);

	/* Check the File size */
	if (dwFileSize == ZERO_BYTES)
	{
		LOGDBG("AUTH_KEY : FILE SIZE IS 0KB", 0, 0, 0);
		fclose(fp);
		return KEY_FILE_SIZE_ZERO;
	}

	if (dwFileSize >= 64)
	{
		LOGDBG("AUTH_KEY : FILE SIZE IS TOO LARGE", 0, 0, 0);
		fclose(fp);
		return KEY_INAVLID_SIZE;
	}
	m_fileSize = dwFileSize;

	/* Allocate memory */
	pBuf = (char *) malloc(m_fileSize);

	if (pBuf == NULL)
	{
		LOGDBG("AUTH_KEY : MEMMORY ALLOCATION FAILED", 0, 0, 0);
		fclose(fp);
		return KEY_FILE_SIZE_ZERO;
	}

	LOGDBG("AUTH_KEY : READING %d BYTES FROM FILE : %s", m_fileSize,
			(_Vx_usr_arg_t )privateKeyFile.c_str(), 0);

	/* Read file contents into pBuf */
	bytesRead = fread(pBuf, m_fileSize, READ_BYTE, fp);

	if ((bytesRead == ZERO_BYTES) && (ferror(fp)))
	{
		LOGDBG("AUTH_KEY : UANBLE TO READ PRIAVTE KEY FILE", 0, 0, 0);
		/* free the allocated memory */
		if (pBuf != NULL)
		{
			free(pBuf);
			pBuf = NULL;
		}
		fclose(fp);
		return KEY_FILE_READ_ERROR;
	}

	/* close the file */
	fclose(fp);

	temp = strstr(pBuf,"@BEGIN_KEY@");
	if(temp != NULL)
	{
		temp = temp + strlen("@BEGIN_KEY@");
	}
	while(*temp)
	{		
		if((*temp != '@'))
		{
			pPrivateKey[index] = *temp;
			index++;
		}
		else
			break;
		*temp++;
	}

	if(index != PRIVATE_KEY_SIZE)
	{
		LOGDBG("AUTH_KEY : PRIAVTE KEY IS IN WRONG FORMAT", 0, 0, 0);
		return KEY_INAVLID_SIZE;
	}
	/* Check weather key is alpha numeric or not*/
	for(int it = 0; it < index -1;it++)
	{
		if(!isalnum(pPrivateKey[it]))
		{
			return KEY_WRONG_FORMAT;
		}
	}

	/* Reversing the private key before setting in EEPROM */
	for(dwFirst = 0, dwLast = index - 1; dwFirst < dwLast; dwFirst++,dwLast--) 
		swap(pPrivateKey[dwFirst], pPrivateKey[dwLast]);

	SetAuthenticationKey(pPrivateKey);

	if (pBuf != NULL)
	{
		free(pBuf);
		pBuf = NULL;
	}
	return KEY_SUCCESS;
}


/**************************************************************************//**
 * \brief   -  Writes private key data into EEPROM.
 *
 * \param   - char *pPrivateKey 
 *
 * \return  - None
 *
 ******************************************************************************/
void AuthenticationKey::SetAuthenticationKey(char *pPrivateKey)
{
	Eeprom epclass;
	memcpy(&m_pPrivateKeyBuffer,pPrivateKey,PRIVATE_KEY_SIZE);

	epclass.Write((char *)&m_pPrivateKeyBuffer,PRIVATE_KEY_SIZE,PRIVATE_KEY_EEPROM_OFFSET);
}

/**************************************************************************//**
 * \brief   -  Reads private key  data from EEPROM & copies
 *      		 into m_pPrivateKeyBuffer buffer.
 *
 * \param   - char *pPrivateKey 
 *
 * \return  - None
 *
 ******************************************************************************/
void AuthenticationKey::GetAuthenticationKey(char *pPrivateKey)
{
	Eeprom epclass;

	epclass.Read(pPrivateKey,PRIVATE_KEY_SIZE,PRIVATE_KEY_EEPROM_OFFSET);

	memcpy(&m_pPrivateKeyBuffer,pPrivateKey,sizeof(m_pPrivateKeyBuffer));

}

/**************************************************************************//**
 * \brief   - send the Messages to UIC 
 *
 * \param   - Message& message, UINT32 buffLen
 *
 * \return  - None
 *
 ******************************************************************************/
void AuthenticationKey::SendToClient(Message& message, UINT32 buffLen) 
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
 * \brief   - Send the response to UIC
 *
 * \param   - UINT32 dwResponseVal
 *
 * \return  - None
 *
 ******************************************************************************/
void AuthenticationKey::SendResponseToUI(UINT32 dwResponseVal) 
{
	UINT32 msgLen = 0;

	/* Send the response to UIC */
	message.msgID = UIC_PRIVATE_KEY_UPLOAD_RES;
	memset(message.Buffer, 0x00, sizeof(message.Buffer));
	sprintf(message.Buffer, "%u", dwResponseVal);
	msgLen = strlen(message.Buffer);
	SendToClient(message, msgLen);
}

/**************************************************************************//**
 * \brief   - De-Initialize allocated stack memory 
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
AuthenticationKey::~AuthenticationKey() 
{
	CP = NULL;
	Interface = NULL;
}
