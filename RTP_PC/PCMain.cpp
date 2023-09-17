/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Main Task that spawns other tasks like Control Task, FPGA and PCStub task etc...
 
**********************************************************************************************************/

/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Main Task that spawns other tasks like Control Task, FPGA and PCStub task etc...
 
**********************************************************************************************************/

#include<iostream>
#include <fstream>
#include"PCTimer.h"
#include"PCMain.h"
#include <msgQLib.h>
#include "versions.h"
#include "GpioAPI.h"
#include "FirmwareUpgrade.h"
#include "Eeprom.h"
#include "PowerUpRecipe.h"

using namespace std;

MSG_Q_ID StubMid,CtrlMid;


#define GPIO_BANK1_PIN0 0x20
#define GPIO_DIR_OUTPUT 1
#define GPIO_VALUE_LOW 0


// local functions
static void VersionDisplay	();

/*
******************************************************************************

*      Function    : updateFirmwareInfo()
*      Description : Function which get invoked at the start of the PCMain.
*      				 This function reads the firmware structure FwInfo from the flash
*      				 and if there is firmware upgradation,it will updates 
*      				 the structure FwInfo
*      				 
*      Input       : No arguments
*      Output      : void
*      Called by   : ACMain()

*******************************************************************************
*/
void updateFirmwareInfo() 
{
	INT8 ret;
	FwInfo FirmWareInfoStruct;
	FwInfo *FirmWareInfo = &FirmWareInfoStruct;
	Eeprom epclass;
	bool ReadEeprom = false;

	GpioAlloc(GPIO_BANK1_PIN0);
	GpioSetDir(GPIO_BANK1_PIN0, GPIO_DIR_OUTPUT);
	GpioSetValue(GPIO_BANK1_PIN0, GPIO_DIR_OUTPUT);
	GpioSetValue(GPIO_BANK1_PIN0, GPIO_VALUE_LOW);
	
	/* Read partition info From QSPI */
	if(QspiLibRead(FW_PARTITION_INFO_QSPI_OFFSET, (unsigned int) FirmWareInfo,
				sizeof(FwInfo)) != OK)
	{
#ifdef PRINT
		LOG("Main_T : ------ Failed to Read Flash Info from QSPI -------\n");
#endif
		ReadEeprom = true;
	}
	/*Check Partition info is Corrupted or not */
	if ((ReadEeprom) || ((FirmWareInfo->activePart != ACTIVE_SLAVE_FIRMWARE_START)
				&& (FirmWareInfo->activePart != PASSIVE_SLAVE_FIRMWARE_START)))
	{
		/* Read Partition info from EEPROM */
		
		ret = epclass.Read((char*) FirmWareInfo, sizeof(FwInfo),
								FW_PARTITION_INFO_EEPROM_OFFSET);
	
		/*check the data from EEPROM is proper */
		if ((ret < ZERO_BYTES) || ((FirmWareInfo->activePart != ACTIVE_SLAVE_FIRMWARE_START)
				&& (FirmWareInfo->activePart != PASSIVE_SLAVE_FIRMWARE_START)))
		{
			LOG("Main_T: -------RESET PARTITION INFO --------- \n");
			FirmWareInfo->bootMode = BRANSON_ACTIVE_BOOTMODE;
			FirmWareInfo->fwUpgrade = BRANSON_NORMAL_BOOT;
			FirmWareInfo->activePart = ACTIVE_SLAVE_FIRMWARE_START;
			FirmWareInfo->passivePart = PASSIVE_SLAVE_FIRMWARE_START;
	
			ret = epclass.Write((char *) FirmWareInfo, sizeof(FwInfo),
									FW_PARTITION_INFO_EEPROM_OFFSET);
			if (ret < ZERO_BYTES)
			{
#ifdef PRINT
				LOG("Main_T : ------ Writing Firmware Part info to EEPROM failed -------\n");
#endif
			}
		}
		
		/*Erase QSPI before write the structure FwInfo*/
		QspiLibBlockErase(FIRMWARE_LOC_BLOCK_NUM);
		
		/*Update the structure FwInfo into flash*/
		if(QspiLibWrite(FW_PARTITION_INFO_QSPI_OFFSET, (uint32_t)FirmWareInfo, sizeof(FwInfo)) != OK)
		{
#ifdef PRINT
			LOG("Main_T : ------ Failed to Write Flash Info to QSPI -------\n");
#endif
			return ;
		}
	}
		
	
	/* Change to active partition */
	if (FirmWareInfo->fwUpgrade == BRANSON_FWUPGRADE
			&& FirmWareInfo->bootMode == BRANSON_PASSIVE_BOOTMODE) 
	{
		FirmWareInfo->fwUpgrade = BRANSON_NORMAL_BOOT;
		FirmWareInfo->bootMode = BRANSON_ACTIVE_BOOTMODE;

		/*Swapping the partitions Active <--> Passive */
		if (FirmWareInfo->activePart == ACTIVE_SLAVE_FIRMWARE_START)
		{
			FirmWareInfo->activePart = PASSIVE_SLAVE_FIRMWARE_START;
			FirmWareInfo->passivePart = ACTIVE_SLAVE_FIRMWARE_START;
		} 
		else 
		{
			FirmWareInfo->activePart = ACTIVE_SLAVE_FIRMWARE_START;
			FirmWareInfo->passivePart = PASSIVE_SLAVE_FIRMWARE_START;
		}

		ret = epclass.Write((char *) FirmWareInfo, sizeof(FwInfo),
				FW_PARTITION_INFO_EEPROM_OFFSET);

		if (ret < ZERO_BYTES)
		{
#ifdef PRINT
			LOG("Main_T : ------ Writing Firmware Part info to EEPROM failed -------\n");
#endif
		}
		/*Erase QSPI before write the structure FwInfo*/
		QspiLibBlockErase(FIRMWARE_LOC_BLOCK_NUM);
		
		/*Update the structure FwInfo into flash*/
		if(QspiLibWrite(FW_PARTITION_INFO_QSPI_OFFSET, (uint32_t)FirmWareInfo, sizeof(FwInfo)) != OK)
		{
#ifdef PRINT
			LOG("Main_T : ------ Failed to Write Flash Info to QSPI -------\n");
#endif
		}
		else
		{
#ifdef PRINT
			LOG("Main_T : ------ Firmware Upgrade Done -------\n");
#endif
		}
	}
}
/*
******************************************************************************

*      Function    : updateFirmwareInfo()
*      Description : Function which get invoked at the start of the PCMain.
*      				 This function reads the firmware structure FwInfo from the flash
*      				 and if there is firmware upgradation,it will updates 
*      				 the structure FwInfo
*      				 
*      Input       : No arguments
*      Output      : void
*      Called by   : ACMain()

*******************************************************************************
*/
void updateFirmwareInfoCRC() 
{
	INT8 ret;
	FwInfoCRC FirmWareInfoStruct;
	FwInfoCRC *FirmWareInfo = &FirmWareInfoStruct;
	Eeprom epclass;
	bool ReadEeprom = false;

	GpioAlloc(GPIO_BANK1_PIN0);
	GpioSetDir(GPIO_BANK1_PIN0, GPIO_DIR_OUTPUT);
	GpioSetValue(GPIO_BANK1_PIN0, GPIO_DIR_OUTPUT);
	GpioSetValue(GPIO_BANK1_PIN0, GPIO_VALUE_LOW);


	/* Read partition info From QSPI */
	if(QspiLibRead(FW_PARTITION_INFO_QSPI_OFFSET, (unsigned int) FirmWareInfo,
				sizeof(FwInfoCRC)) != OK)
	{
#ifdef PRINT
		LOG("Main_T : ------ Failed to Read Flash Info from QSPI -------\n");
#endif
		ReadEeprom = true;
	}
	/*Check Partition info is Corrupted or not */
	if ((ReadEeprom) || ((FirmWareInfo->partition_a_offset != ACTIVE_SLAVE_FIRMWARE_START)
				&& (FirmWareInfo->partition_a_offset != PASSIVE_SLAVE_FIRMWARE_START)))
	{
		/* Read Partition info from EEPROM */
		
		ret = epclass.Read((char*) FirmWareInfo, sizeof(FwInfoCRC),
								FW_PARTITION_INFO_EEPROM_OFFSET);
	
		/*check the data from EEPROM is proper */
		if ((ret < ZERO_BYTES) || ((FirmWareInfo->partition_a_offset != ACTIVE_SLAVE_FIRMWARE_START)
				&& (FirmWareInfo->partition_a_offset != PASSIVE_SLAVE_FIRMWARE_START)))
		{
			LOG("Main_T: -------RESET PARTITION INFO --------- \n");
			FirmWareInfo->boot_status = BRANSON_ACTIVE_BOOTMODE;
			FirmWareInfo->fw_upgrade = BRANSON_NORMAL_BOOT;
			FirmWareInfo->partition_a_offset = ACTIVE_SLAVE_FIRMWARE_START;
			FirmWareInfo->partition_b_offset = PASSIVE_SLAVE_FIRMWARE_START;
	
			ret = epclass.Write((char *) FirmWareInfo, sizeof(FwInfoCRC),
									FW_PARTITION_INFO_EEPROM_OFFSET);
			if (ret < ZERO_BYTES)
			{
#ifdef PRINT
				LOG("Main_T : ------ Writing Firmware Part info to EEPROM failed -------\n");
#endif
			}
		}
		
		/*Erase QSPI before write the structure FwInfo*/
		QspiLibBlockErase(FIRMWARE_LOC_BLOCK_NUM);
		
		/*Update the structure FwInfo into flash*/
		if(QspiLibWrite(FW_PARTITION_INFO_QSPI_OFFSET, (uint32_t)FirmWareInfo, sizeof(FwInfoCRC)) != OK)
		{
#ifdef PRINT
			LOG("Main_T : ------ Failed to Write Flash Info to QSPI -------\n");
#endif
			return ;
		}
	}
		
	
	/* Change to active partition */
	if (FirmWareInfo->fw_upgrade == BRANSON_FWUPGRADE
			&& FirmWareInfo->boot_status == BRANSON_PASSIVE_BOOTMODE) 
	{
		FirmWareInfo->fw_upgrade = BRANSON_NORMAL_BOOT;
		FirmWareInfo->boot_status = BRANSON_ACTIVE_BOOTMODE;

		/*Swapping the partitions Active <--> Passive */
		if (FirmWareInfo->partition_a_offset == ACTIVE_SLAVE_FIRMWARE_START)
		{
			FirmWareInfo->partition_a_offset = PASSIVE_SLAVE_FIRMWARE_START;
			FirmWareInfo->partition_b_offset = ACTIVE_SLAVE_FIRMWARE_START;
		} 
		else 
		{
			FirmWareInfo->partition_a_offset = ACTIVE_SLAVE_FIRMWARE_START;
			FirmWareInfo->partition_b_offset = PASSIVE_SLAVE_FIRMWARE_START;
		}

		ret = epclass.Write((char *) FirmWareInfo, sizeof(FwInfoCRC),
				FW_PARTITION_INFO_EEPROM_OFFSET);

		if (ret < ZERO_BYTES)
		{
#ifdef PRINT
			LOG("Main_T : ------ Writing Firmware Part info to EEPROM failed -------\n");
#endif
		}
		/*Erase QSPI before write the structure FwInfo*/
		QspiLibBlockErase(FIRMWARE_LOC_BLOCK_NUM);
		
		/*Update the structure FwInfo into flash*/
		if(QspiLibWrite(FW_PARTITION_INFO_QSPI_OFFSET, (uint32_t)FirmWareInfo, sizeof(FwInfoCRC)) != OK)
		{
#ifdef PRINT
			LOG("Main_T : ------ Failed to Write Flash Info to QSPI -------\n");
#endif
		}
		else
		{
#ifdef PRINT
			LOG("Main_T : ------ Firmware Upgrade Done -------\n");
#endif
		}
	}
	
	/* ADDED BY MOUNIKA*/
	if(FirmWareInfo->partition_a_offset == ACTIVE_SLAVE_FIRMWARE_START)
	{
		FWCrc::Set(FW_CRC_PC,FirmWareInfo->partition_a_crc);
	}
		
	if(FirmWareInfo->partition_a_offset == PASSIVE_SLAVE_FIRMWARE_START)
	{
		FWCrc::Set(FW_CRC_PC,FirmWareInfo->partition_b_crc);
	}
}

/******************************************************************************
*      Function    :   PrintMsg
*      Description :   Function for printing log Messages
*      Input       :   void
*      Output      :   void
*      Called by   :   -
*******************************************************************************/
void LogInfo::PrintMsg(char *msg, _Vx_usr_arg_t val1, _Vx_usr_arg_t val2, _Vx_usr_arg_t val3, _Vx_usr_arg_t val4, _Vx_usr_arg_t val5, _Vx_usr_arg_t val6)
{
       printf(msg, val1, val2, val3, val4, val5, val6);
}

/*
******************************************************************************

*      Function    :   VersionDisplay
*      Description :   Initializes version data for console display
*      Input       :   No arguments
*      Output      :   No return type
*      Called by   :   PcMain

*******************************************************************************
*/
static void VersionDisplay	()
{
	// this method will be used to populate an outgoing SDO
	LOG("--------------------- VERSION ------------------------------------\n");
	LOG("   PC Major         = %d\n", FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR));
	LOG("   PC Minor         = %d\n", FWVersion::Get(FW_VERSION_PC, VERSION_MINOR));
	LOG("   PC Build         = %d\n", FWVersion::Get(FW_VERSION_PC, VERSION_BUILD));
	LOG("------------------------------------------------------------------\n");	
}

/******************************************************************************
*      Function    :   main
*      Description :   Entry point function for PC module
*      Input       :   void
*      Output      :   int
*      Called by   :   SC module
*******************************************************************************/
extern "C" int PCMain()
{
	FirmwareUpgrade Fwupgrade;
	bool UbootVersionFlag = false;
	UbootVersionFlag = Fwupgrade.IsNewBootloaderForCRC();
	if(UbootVersionFlag)
	{
		updateFirmwareInfoCRC();
	}
	else
	{
		updateFirmwareInfo();
	}

    VersionDisplay ();
	PCTaskCreation pc;
	pc.SpawnTask();
	PowerUpData::ReadPowerUpDataFromEEPROM();
	PCTimer::Timer_Init();
	GpioInit();
	taskExit(0);
	return SUCCESS;
}

