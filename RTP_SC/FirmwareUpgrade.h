/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef FIRMWAREUPGRADE_H_
#define FIRMWAREUPGRADE_H_

#include "Common.h"
#include "CommonProperty.h"
#include <usrFsLib.h>
#include <string>
#include "CommunicateInterface.h"
#include "versions.h"
#include "Eeprom.h"

#define   SUCCESS  					 0
#define   FAILURE					-1

/* macros for FoE system call */
#define   GROUP_NUMBER		  		 3
#define   FOE_DL_SYSCALL_ENTRY   	 4

#define   FW_UPGRADE_FILE_COUNT_MAX  10
#define   FILE_NAME_MAX_SIZE     	 255
#define   ERROR_BUFF_SIZE    		 4
#define   USB_PATH            		"/bd0:1/"
#define   HTTP_FILE_DIR             "/ram0/"
#define   CACHE_FILE          		"/emmc0:0/FirmwareFiles.txt"

#define   READ_BINARY        		"rb"
#define   READ_ONLY          		"r"
#define   RDWR_CREAT         		"w+"
#define   ZERO_BYTES                 0
#define	  READ_BYTE           		 1

#define   FW_UPGRADE_SUCCESS         0x11
 
/* Upgrade Error values */
#define   EC_NOERROR 		  		 0x0000
#define   USB_READ_FAIL		       	 0x0001
#define   NO_FILES_PRESENT		     0x0002
#define   FILE_SIZE_ZERO		     0x0003
#define   USB_FILE_LIST_ERROR      	 0x0004

/* Firmware upgrade file name format */
#define   AC_FILE_NAME_FORMAT        "AC_*.bin"
#define   PC_FILE_NAME_FORMAT        "PC_*.bin"
#define   SC_FILE_NAME_FORMAT        "SC_*.bin"


/* File headers for SC, PC & AC */
#define   SC_FILE_HEADER			 "SC_"
#define   PC_FILE_HEADER		     "PC_"
#define   AC_FILE_HEADER		     "AC_"

/* Indexes for the message received from UIC */
#define   MSG_INDEX_ZERO      		 0
#define   MSG_INDEX_ONE              1
#define   MSG_INDEX_TWO              2
#define   MSG_INDEX_THREE            3
#define   MSG_INDEX_FOUR             4 
#define   MSG_INDEX_FIVE             5

/* Allocated size of active VxWorks bin */
#define ACTIVE_PART_FIRMWARE_START	 	0x001E0000 

/* Allocated size of passive VxWorks bin */
#define PASSIVE_PART_FIRMWARE_START	 	0x02000000 

#define UBOOT_VERSION_QSPI_OFFSET			0x00020000
#define FW_PARTITION_INFO_QSPI_OFFSET		0x001C0000
#define FW_PARTITION_INFO_QSPI_OFFSET_CRC	0x001D0000
#define FIRMWARE_LOC_BLOCK_NUM 		 		0x0000000D

#define BOOTMODE_DEFAULT            	0xFFFFFFFF

#define FIRMWARE_UPGRADE_FALSE			0x00
#define FIRMWARE_UPGRADE_TRUE	 		0x01

#define ACTIVE_BOOTMODE 				0X00
#define PASSIVE_BOOTMODE  				0x01
/*	uboot version+timestamp	*/
#define UBOOT_VERSION					0x133C648 //20170312

/* AC progress response to UIC */
typedef enum
{
	AC_PROGRESS_ONE = 11,
	AC_PROGRESS_TWO,
	AC_PROGRESS_THREE,
	AC_PROGRESS_FOUR,
	AC_PROGRESS_FIVE	
}ACProgressResponseToUI;

/* PC progress response to UIC */
typedef enum
{
	PC_PROGRESS_ONE = 21,
	PC_PROGRESS_TWO,
	PC_PROGRESS_THREE,
	PC_PROGRESS_FOUR,
	PC_PROGRESS_FIVE	
}PCProgressResponseToUI;

/* SC progress response to UIC */
typedef enum
{
	SC_PROGRESS_ONE = 31,
	SC_PROGRESS_TWO,
	SC_PROGRESS_THREE,
	SC_PROGRESS_FOUR,
	SC_PROGRESS_FIVE	
}SCProgressResponseToUI;

/* Slave response to SC during upgrade */ 
typedef enum {
	 SLAVE_PROGRESS_RESP_ONE = 1,
	 SLAVE_PROGRESS_RESP_TWO,
	 SLAVE_PROGRESS_RESP_THREE,
	 SLAVE_PROGRESS_RESP_FOUR,
	 SLAVE_PROGRESS_RESP_FIVE,
	 SLAVE_PROGRESS_RESP_ERROR
}SlaveProgressResponseToSC;


#define  CRC_VALIDATION_SUCCESS		 0x0002
#define  CRC_BIT_SHIFT               8

/* CRC table */
const unsigned short crc16tab[256]= {
        0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
        0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
        0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
        0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
        0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
        0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
        0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
        0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
        0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
        0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
        0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
        0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
        0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
        0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
        0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
        0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
        0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
        0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
        0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
        0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
        0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
        0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
        0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
        0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
        0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
        0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
        0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
        0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
        0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
        0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
        0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
        0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};


/*-------------------------------------------------------------------------*/
/*                                                                         */
/*  Lookup Tables.  The "hardware" method of computing CRCs involves bit   */
/*  manipulations, which is very inefficient for a software computation.   */
/*  Instead of computing the CRC bit-by-bit, a 256-element lookup table    */
/*  can be used to perform the equivalent of 8 bit operations at a time.   */
/*  (This is described in "Byte-wise CRC Calculations" in IEEE Micro,      */
/*  June 1983, pp. 40-50.)  For a CRC-16, the lookup table consists of 256 */
/*  2-byte WORDs (see below, or the CRC16.PAS unit for the actual table,   */
/*  or the CRCTable program for computation of the lookup table for the    */
/*  x16 + x15 + x2 + 1 generator polynomial):                              */
/*                                                                         */
/*-------------------------------------------------------------------------*/
const unsigned short CRCTable[256] =
{
  0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0,
  0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 0xCC01, 0x0CC0, 0x0D80, 0xCD41,
  0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0,
  0x0880, 0xC841, 0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
  0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1,
  0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341,
  0x1100, 0xD1C1, 0xD081, 0x1040, 0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1,
  0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
  0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0,
  0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 0x2800, 0xE8C1, 0xE981, 0x2940,
  0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1,
  0xEC81, 0x2C40, 0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
  0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 0xA001, 0x60C0,
  0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740,
  0xA501, 0x65C0, 0x6480, 0xA441, 0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0,
  0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
  0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0,
  0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 0xB401, 0x74C0, 0x7580, 0xB541,
  0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0,
  0x7080, 0xB041, 0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
  0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 0x9C01, 0x5CC0,
  0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40,
  0x9901, 0x59C0, 0x5880, 0x9841, 0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1,
  0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
  0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0,
  0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};




class FirmwareUpgrade 
{
public:
	FirmwareUpgrade();
	
	UINT32 			FoeDownloadReqSysCall(UINT16 , UINT16, char *, UINT32 , unsigned char *);
	char *			USBRead();
	char *			USBDetect();
	INT8 			ProcessFirmwareMsg(char *);
	INT8 			MasterFirmwareUpgrade(UINT32 , char * );
	INT8			MasterFirmwareUpgradeCRC(UINT32 , char * );
	INT8 			SlaveFirmwareUpgrade(UINT16 , UINT32 , char *);
	void 			FirmwareUpgradeMsgHandler(void ); /* Task function routine */
	void			SendToClient(Message& , UINT32 );
	void 			SendToMsgQ(Message& msg, const MSG_Q_ID &MsgQID);
    void            ProgressResponseToUI(UINT32 );
    bool 			DeleteTasks(void );
    void            ReadUSBResponseToUI();
    unsigned short  crc16_ccitt(const void *, int );
    unsigned short ByteCRC(char * Ptr, unsigned int Length);
    STATUS          ProgressResponseFromAC(void);
    STATUS          ProgressResponseFromPC(void);
    INT8            ValidateFileSize(UINT16, string );
    void 			VersionUpdateToEeprom(void);
    bool			IsNewBootloaderForCRC(void);
    
	~FirmwareUpgrade();
	
private:
	MSG_Q_ID 		FW_MSG_Q_ID, SDO_MSG_Q_ID;
	TASK_ID			Ctrl_TID, SDO_TID, UI_TID, DB_TID, UI_DB_TID, ALARM_TID, BARCODE_TID, DGTIN_TID, DGTOUT_TID, IO_Image_TID, Mail_Box_TID;
	Message			message;
	char 			pErrorBuffer[ERROR_BUFF_SIZE];	
	UINT16          fwUpgrdCRC, fileCountSC, fileCountPC, fileCountAC;
	string 			fwFileName, fwFileNameSC, fwFileNamePC , fwFileNameAC ,fwFilePath;
	STATUS 			slaveStatus, usbReadStatus;
	INT8            upgradeStatus, bytesRead;
	UINT32 			fileSizeSC, fileSizePC, fileSizeAC;
	string          fwFileDirectory;
	FILE			*fp;
	char		    *pBuf;
	CommonProperty  *CP;
	CommunicateInterface *Interface;
	bool			UbootVersionFlag;
	
public:
	static FirmWareUpgradeInfo	fwUpgrdInfoAC, fwUpgrdInfoPC;
	static bool 	bIsFirmwareUpgrade;
};

#endif /* FIRMWAREUPGRADE_H_ */
