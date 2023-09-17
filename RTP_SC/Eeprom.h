/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef EEPROM_H_
#define EEPROM_H_

#include <vxWorks.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ioLib.h>

/* Macros */
#define MAX_EEPROM_SIZE 32768
#define EEPROM_PATH	"/eeprom/0"

//define the EEPROM address of the parameters

/* intf_SysConfig_t in the file SysConfigEeprom.hpp, will move here late */
/* 
 * current structure intf_SysConfig_t start from 0x60, size is 154 bytes
 * the flag is in address 0x5f
 * 
 * new structure intf_SysConfig_t start from 0x180, size is 137 bytes
 * the flag is in address 0x17f
 * 
 */

/*0x20 to 0x5E is max available eeprom for partition info*/
#define FW_PARTITION_INFO_EEPROM_OFFSET 	0x00000020

#define RUN_TIME_CONFIGURE			0x100
#define EEPROM_MODEL_ADDRESS		0x110

/*Macros for IP Configuration*/
#define EEPROM_IPCONF_OFFSET        0x160

/* System configuration OFFSETS */
#define SYSCONFIG_BASE				0x17F
#define SYSCONFIG_ADDR				0x180
#define OLD_SYSCONFIG_BASE 			0x5F
#define OLD_SYSCONFIG_ADDR			0x60

/* Macros for MAC address */
#define EEPROM_MAC_ADDRESS_OFFSET   0x2E0
#define EEPROM_MAC_ADDRESS_SIZE   	0x06

/* system counters address */
#define COUNTER_BASE 				0x300
#define COUNTER_ADDR				0x310
#define OVERLOAD_BASE 				0x330
#define OVERLOAD_ADDR				0x340
#define ACTUATION_BASE 				0x360
#define ACTUATION_ADDR				0x370
#define ALARMS_BASE 				0x390
#define ALARMS_ADDR					0x3A0

/* Event log related eeprom variables */
#define EVENTLOG_BASE					0x400
#define EVENTLOG_DBDELETED_SIG_OFFSET	0
#define EVENTLOG_DBDELETED_SIG_SIZE		1
#define EVENTLOG_DBDELETED_SIG_ACTIVE	0xF1
#define EVENTLOG_DBDELETED_SIG_INACTIVE	0xFF
#define EVENTLOG_USERNAME_OFFSET		1
#define EVENTLOG_USERNAME_SIZE			16

#define COUNTERSIG 	  				0x54 
#define COUNTER_IDENTIFIER_SIZE 	1
#define OVERLOADSIG 	  			0x55 
#define OVERLOAD_IDENTIFIER_SIZE 	1
#define ACTUATIONSIG 	  			0x56 
#define ACTUATION_IDENTIFIER_SIZE 	1
#define ALARMSSIG 	  				0x57 
#define ALARMS_IDENTIFIER_SIZE 		1

/* 174 bytes, will be removed later because they are in the database */
#define SHUTDOWN_EEPROM_OFFSET 		 0x500

/* firmware upgrade eeprom variables */
#define   FW_FLAG_EEPROM_OFFSET 	 0x1000
#define   SC_FW_VER_EEPROM_OFFSET    0x1020
#define   PC_FW_VER_EEPROM_OFFSET    0x1040
#define   AC_FW_VER_EEPROM_OFFSET    0x1060
#define   UIC_FW_VER_EEPROM_OFFSET   0x1080
#define   SYSINFO_SIZE				 16

/* private key eeprom variables, Authentication Key, 32 bytes */
#define   PRIVATE_KEY_EEPROM_OFFSET     0x10A0   

/* Webservices and FW upgrade over Ethernet Switch status */
#define   WEBSERVICE_FOE_ENABLE_OFFSET	0x10D0
#define   WEBSERVICE_FOE_ENABLE_SIZE	3

/* IP details, 2 Bytes */
 
#define  IP_DETAILS_EEPROM_OFFSET        0x1601                
#define  IP_DETAILS_EEPROM_SIZE          2



class Eeprom
{
	public:	
		Eeprom();
		ssize_t Write(char *pBuf, ssize_t length);
		ssize_t Write(char *pBuf, ssize_t length, ssize_t offset);
		ssize_t Read(char *pBuf, ssize_t length);
		ssize_t Read(char *pBuf, ssize_t length, ssize_t offset);
		ssize_t SetCurOffset(ssize_t offset);
		ssize_t GetCurOffset();
		ssize_t SizeAvlbl();
		~Eeprom();
		
	private:	
		INT32    fd;
};

#endif /* EEPROM_H_ */
