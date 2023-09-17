/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Main Task that spawns other tasks like Control Task, Servo task etc...
 
**********************************************************************************************************/



/* Header files and Global Variables */
#include "AcMainTask.h"
#include <msgQLib.h>
#include "versions.h"
#include "ServoTaskClock.h"
#include "FirmwareUpgrade.h"
#include "GpioAPI.h"
#include "SPI.h"
#include "Eeprom.h"
#include <errnoLib.h>

/*Needs integration with C library, so to avoid Name Mangling*/	
extern "C"
{
	int AcMain();		
}

// Message queue Ids for Stub Task(Alarm)
MSG_Q_ID StubQid, CtrlQid;		


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
	
		if (ret < ZERO_BYTES)
		{
#ifdef PRINT
			LOG("Main_T : ------ Reading Firmware Part info from EEPROM failed -------\n");
#endif
		}
	
		/*check the data from EEPROM is proper */
		if ((ret < ZERO_BYTES) || ((FirmWareInfo->activePart != ACTIVE_SLAVE_FIRMWARE_START)
				&& (FirmWareInfo->activePart != PASSIVE_SLAVE_FIRMWARE_START)))
		{
			LOG("Main_T : ------ Reset Firmware Part info  -------\n");
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
	
		if (ret < ZERO_BYTES)
		{
#ifdef PRINT
			LOG("Main_T : ------ Reading Firmware Part info from EEPROM failed -------\n");
#endif
		}
	
		/*check the data from EEPROM is proper */
		if ((ret < ZERO_BYTES) || ((FirmWareInfo->partition_a_offset != ACTIVE_SLAVE_FIRMWARE_START)
				&& (FirmWareInfo->partition_a_offset != PASSIVE_SLAVE_FIRMWARE_START)))
		{
			LOG("Main_T : ------ Reset Firmware Part info  -------\n");
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
		FWCrc::Set(FW_CRC_AC,FirmWareInfo->partition_a_crc);
	}
				
	if(FirmWareInfo->partition_a_offset == PASSIVE_SLAVE_FIRMWARE_START)
	{
		FWCrc::Set(FW_CRC_AC,FirmWareInfo->partition_b_crc);
	}
}

/*
******************************************************************************

*      Function    :   AcMain
*      Description :   Main Task responsible for spawning other tasks
*      Input       :   No arguments
*      Output      :   int - Main Task return value. 0: Success, -1: Failure
*      Called by   :   :: AcMain

*******************************************************************************
*/

int AcMain()								//Main Task
{   
	INT16 stat = ERROR;
	FirmwareUpgrade Fwupgrade;
	bool UbootVerionFlag;
	UbootVerionFlag = Fwupgrade.IsNewBootloaderForCRC();
	if(UbootVerionFlag)
	{
		updateFirmwareInfoCRC();
	}
	else
	{
		updateFirmwareInfo();
	}
	
	#ifdef DEBUG
		LOG("--------------------------- Process Created --------------------------\n\n");			    	
	#endif
 
    //Initialize SPI and Digital IO
	GpioSetVal(UP_DIGIO_RESET, GPIO_VALUE_HIGH); //Disable the chips Max14913 and PCA9704
	stat = McSPI_Init_DIO();
	if (ERROR == stat)
	{
		LOG("SPI init failed!!!\n");
	}
		
    TaskInfo taskinfo;			
    taskinfo.TaskName[taskinfo.CONTROLTASK_INDEX] = "/ControlTask";	//Triggered by Timer        
    //Spawns the Control Task. "NULL" User Options and No arguments to the Routine called by "pCtrl"
    taskinfo.tidControl = taskSpawn(taskinfo.TaskName[taskinfo.CONTROLTASK_INDEX], taskinfo.CONTROLTASK_PRIORITY, VX_FP_TASK, STACK_SIZE, (FUNCPTR)controlTask, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);    
    if (taskinfo.tidControl == TASK_ID_ERROR)
    {
		#ifdef ERROR_PRINT
    		LOG("\nError spawning Control task!!!\n");    		    		
		#endif
    	return FAILURE;
    }
      
    

    
    taskDelay(DELAY);     
    taskinfo.TaskName[taskinfo.SERVOTASK_INDEX] = "ServoTask";    
    //Spawns the servo Task. "NULL" User Options and No arguments to the Routine called by "pServo"    
    taskinfo.tidServo = taskSpawn(taskinfo.TaskName[taskinfo.SERVOTASK_INDEX], taskinfo.SERVOTASK_PRIORITY, VX_FP_TASK, STACK_SIZE, (FUNCPTR)servoTask, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    if (taskinfo.tidServo == TASK_ID_ERROR)
    {
		#ifdef ERROR_PRINT
    		LOG("\nError spawning Servo task!!!\n");    		    		
		#endif
    	return FAILURE;
    }
 
    taskDelay(DELAY);     
    taskinfo.TaskName[taskinfo.DGTINPUT_TASK_INDEX] = "AcDgtInputTask";    
    //Spawns the Digital Input Task.     
    taskinfo.tidDgtInput = taskSpawn(taskinfo.TaskName[taskinfo.DGTINPUT_TASK_INDEX], taskinfo.DGTINPUT_TASK_PRIORITY, VX_FP_TASK, STACK_SIZE, (FUNCPTR)AcDgtInputTask, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    if (taskinfo.tidDgtInput == TASK_ID_ERROR)
    {
		#ifdef ERROR_PRINT
    		LOG("\nError spawning DgtInput task!!!\n");    		    		
		#endif
    	return FAILURE;
    }

    taskDelay(DELAY);     
    taskinfo.TaskName[taskinfo.DGTOUTPUT_TASK_INDEX] = "AcDgtOutputTask";    
    //Spawns the Digital Output Task.   
    taskinfo.tidDgtOutput = taskSpawn(taskinfo.TaskName[taskinfo.DGTOUTPUT_TASK_INDEX], taskinfo.DGTOUTPUT_TASK_PRIORITY, VX_FP_TASK, STACK_SIZE, (FUNCPTR)AcDgtOutput::AcDgtOutputTask, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    if (taskinfo.tidDgtOutput == TASK_ID_ERROR)
    {
		#ifdef ERROR_PRINT
    		LOG("\nError spawning DgtOutput task!!!\n");    		    		
		#endif
    	return FAILURE;
    }

    taskDelay(DELAY);
    
    taskinfo.TaskName[taskinfo.FWUPGRD_TASK_INDEX] = "FwUpgradeTask";    
	/*Spawns the FwUpgradeTask*/ 
	taskinfo.tidFwUpgrade = taskSpawn(taskinfo.TaskName[taskinfo.FWUPGRD_TASK_INDEX], taskinfo.FWUPGRADE_TASK_PRIORITY, VX_FP_TASK, STACK_SIZE, (FUNCPTR)FwUpgradeRHandler, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	if (taskinfo.tidFwUpgrade == TASK_ID_ERROR)
	{
		#ifdef ERROR_PRINT
			LOG("\nError spawning FwUpgradeTask!!!\n");    		    		
		#endif
		return FAILURE;
	}
	taskDelay(DELAY);
    //Initialize Digital Input SPI interface
    AcDgtCtlInTask();
        
    // this method will be used to populate an outgoing SDO
    VersionDisplay ();
        
    taskExit(0);
    
    return SUCCESS;
}


/*
******************************************************************************

*      Function    :   VersionDisplay
*      Description :   Initializes version data for console display
*      Input       :   No arguments
*      Output      :   No return type
*      Called by   :   AcMain

*******************************************************************************
*/
static void VersionDisplay	()
{
	// this method will be used to populate an outgoing SDO
	LOG("---------- VERSION ----------\n");
	LOG("AC Major: %d\n", FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR));
	LOG("AC Minor: %d\n", FWVersion::Get(FW_VERSION_AC, VERSION_MINOR));
	LOG("AC Build: %d\n", FWVersion::Get(FW_VERSION_AC, VERSION_BUILD));
	LOG("-----------------------------\n");	
}



/*
**********************************************************************************

*      Function    :   Ctrltask
*      Description :   AC Control Task for TxPdo and RxPdo communication
*      Input       :   No arguments
*      Output      :   No return type
*      Called by   :   AcMain

**********************************************************************************
*/

void controlTask()
{	
	UINT32 events = 0;						//To avoid Garbage value, this value gets overwritten
	SD_ID RtSD;		
	INT32 Timeout = WAIT_FOREVER;
	
	#ifdef DEBUG	
		LOG("------------------------ Control Task spawned ------------------------\n\n");			
	#endif	
	AcControlTask* ctrltask = new AcControlTask;					
	ctrltask->pRtData = &ctrltask->AcRealTime;
	
	// Open Shared Data for OD Real Time data transfer	
	RtSD = sdOpen("/RealTimeData", 0, 0, 0, 0, SD_ATTR_RWX | SD_CACHE_OFF, (void **) &ctrltask->pRtData);
	if(SD_ID_NULL == RtSD)
	{
		LOG("Failed to Open Shared Data!!!\n");
		exit(FAILURE);		
	}			
	ACObjDict :: acOD.GetODDataProp();
	// one time write of firmware info into OD
	ctrltask->WriteFirmwareInfoToOD();	
	if(FAILURE == ctrltask->WriteCalibrationTimeStampFromEepromToOD())
	{
		LOG("Failed to write calibration data from eeprom to OD\n");
	}
	while(TRUE)		// For Continuous Exchange of Real and Non-Real time data
	{	
		if(eventReceive(PDI_INTERRUPT, EVENTS_WAIT_ANY, Timeout, &events) == OK)
		{	

			Timeout = PDO_TIMEOUT;
			ctrltask->ProcessEvent();		//Process the received event						
		}
		else if (errnoGet() == S_eventLib_TIMEOUT)
		{
			ObjectDictionary::rpdo.SC_State = SC_UNREACHABLE;
			AC :: stateMachine.RunStateMachine();	
		}
					
	}	
	
}


/*
**********************************************************************************

*      Function    :   servoTask
*      Description :   AC Servo Task for servo motor control
*      Input       :   No arguments
*      Output      :   No return type
*      Called by   :   AcMain

**********************************************************************************
*/

void servoTask()
{
	#ifdef DEBUG	
		LOG("------------------------ Servo Task spawned ------------------------\n\n");			
	#endif
		//TODO add setup stuff
	servoTaskSem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	AC::servo.servoInit();
	ServoTaskClock::TimerInit();
	while(TRUE)	
	{	
		semTake(servoTaskSem,WAIT_FOREVER);
		AC::servo.calculatePeriod();		
	}			
}


/*
**********************************************************************************

*      Function    :   FwUpgradeRHandler
*      Description :   Function which Handles the firmware up-gradation of AC module 
*      Input       :   No arguments
*      Output      :   No return type
*      Called by   :   AcMain

**********************************************************************************
*/
void FwUpgradeRHandler()
{
	FirmwareUpgrade FwUpgradeObj; 
	if(FwUpgradeObj.ProcessFwUpgradeMsgq() != OK)
	{
		#ifdef ERROR_PRINT
			LOG("\nFirmware Upgrade Task Exit!!!\n");					
		#endif	
	}

}
