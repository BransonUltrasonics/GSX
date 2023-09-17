/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef AUTHENTICATEPRIVATEKEY_H
#define AUTHENTICATEPRIVATEKEY_H

#include "Common.h"
#include "CommonProperty.h"
#include "CommunicateInterface.h"
#include "Eeprom.h"
#include <string>
#include <usrFsLib.h>

#define   PRIVATEKEY_FILE_COUNT_MAX  10
#define   FILE_NAME_MAX_SIZE     	 255
#define   ERROR_BUFF_SIZE    		 4
#define   USB_PATH            		"/bd0:1/"
#define   HTTP_FILE_DIR             "/ram0/"
#define   CACHE_FILE_PK        		"/emmc0:0/PrivateKey.txt"

#define   READ_BINARY        		"rb"
#define   READ_ONLY          		"r"
#define   RDWR_CREAT         		"w+"
#define   ZERO_BYTES                 0
#define	  READ_BYTE           		 1


/* private key Error values */
#define   KEY_SUCCESS				 0x0000
#define   KEY_USB_READ_FAIL	       	 0x0001
#define   KEY_NO_FILES_PRESENT	     0x0002
#define   KEY_FILE_SIZE_ZERO	     0x0003
#define   KEY_FILE_OPEN_ERROR		 0x0004
#define   KEY_FILE_READ_ERROR		 0x0005
#define   KEY_WRONG_FORMAT			 0x0006
#define   KEY_INAVLID_SIZE			 0x0007

/* private key file name format */
#define   PRIVTAE_KEY_FILE_NAME_FORMAT        "*.key"

class AuthenticationKey 
{
public:
	AuthenticationKey();
	~AuthenticationKey();
	
	void 			ReadPrivateKeyFromFile(char *fileName);
    void            ReadUSBResponseForPrivateKeyToUI();
    void 			GetAuthenticationKey(char *pPrivateKey);

private:

	char *			USBRead();
	char *			USBDetect();
	void			SendToClient(Message& , UINT32 );
    void 			SendResponseToUI(UINT32) ;
    INT8            ValidateFileSizeAndStoreInEEPROM(string &);
    void 			SetAuthenticationKey(char *pPrivateKey);
	
	CommonProperty  *CP;
	CommunicateInterface *Interface;
	FILE			*fp;
	char 			m_pPrivateKeyBuffer[PRIVATE_KEY_SIZE];
	char 			pErrorBuffer[ERROR_BUFF_SIZE];	
	STATUS 			usbReadStatus;
	string 			strFileNames,pkFilePath,pkFileDirectory;
	UINT16 			m_fileCount;
	UINT32			m_fileSize;
	char		    *pBuf;
	Message			message;
};

#endif /* AUTHENTICATEPRIVATEKEY_H */
