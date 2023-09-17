/*
 * CommFPGATask.cpp
 *
 *  Created on: Jul 20, 2017
 *      Author: lzhu
 */

#include <iostream>
#include "PCObjDictionary.h"
#include <vxWorks.h>        //for vxworks main
#include <stdio.h>          //for printf(),scanf(),fgets()
#include <string.h>         //for strcmp()
#include <taskLib.h>        //for taskdelay()

#include "SPI.h"
#include "FPGA.h"
#include "ADC.h"
#include "WeldRun.h"
#include "Scan.h"
#include "Common.h"
#include "PCStateMachine.h"
#include "CommFPGATask.h"
#include "PC.h"
#include "Commons.h"
#include "GpioAPI.h"
#include "FirmWareUpgrade.h"
extern "C" 
{
#include <customSystemCall.h>
}


/**************************************************************************//**
* \brief  - void CommFPGATask() 
* 			This function reads digital and analog inputs from ADC channels,
*    		MI memory mapped I/O ,from FPGA and processes control loops. 
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void CommFPGATask()
{
	UINT32 events = 0;
	UINT8 FPGAInputs = 0;
	UINT16 FPGAStatus = 0;
	UINT32 FPGAFrequency = 0;
	INT32 TempPhase;
	UINT32 TempF_Factor;
	UINT8 FanStatus = false;
	UINT8 TestFPGAFlag = FALSE;
	UINT8 Fpga_Off_flag = FALSE;
	UINT32 FPGA_ReInit_Retries = 0;
	UINT8 FPGA_ReInitRequestFlag = FALSE;
	INT16 stat = GPIO_ERROR;
	UINT16 val_5V;
	UINT16 val_15V;
	SPI::Init();	
    ADC::SetADCSequence();
	FPGA::Init();
	if(!FPGA::PowerUpSequence())
	{
		Alarm::SetAlarmReason(ERR_FPGA_FB_ERR);
		FPGA::DisableSpiXfer();
		Alarm::SetAlarmFlag();
		LOG("ERR_FPGA_FB_ERR\n");
	}
	FPGAStatus = FPGA::GetStatus();
	Sonics::SetDefaultFreqAndFFactor();
	
	while(1)
	{
			
		if (eventReceive(EVENT_CTRL_TASK,EVENTS_WAIT_ANY,WAIT_FOREVER, &events) == OK )
		{
										
			if(((FPGAStatus== FALSE)&&(FPGA_ReInitRequestFlag == FALSE)))
			{
				
				if(Fpga_Off_flag == FALSE)
				{
					Fpga_Off_flag = TRUE;
					FPGA::DisableSpiXfer();
					LOG("DisableSpiXfer\n");
				}
				if(FPGA::VerifyFPGAComm())
				{
					FPGA_ReInitRequestFlag = TRUE;
					LOG("VerifyFPGAComm\n");
				}
			}
			else if(Fpga_Off_flag == TRUE)
			{
				if(!FPGA::Re_Init())
				{
					FPGA_ReInit_Retries++;
					LOG("Re_Init()\n");
					
					if(FPGA_ReInit_Retries >= INIT_DONE_MAX_RETRIES)
					{
						FPGA_ReInit_Retries = 0;
						Alarm::SetAlarmReason(ERR_FPGA_FB_ERR);
						Alarm::SetAlarmFlag();
						Fpga_Off_flag = FALSE;
						LOG("SetAlarmReason\n");
					}
				}
				else
				{
					FPGA_ReInit_Retries = 0;
					Fpga_Off_flag = FALSE;
					FPGA_ReInitRequestFlag = FALSE;
					Alarm::ClearOverloads();
					Alarm::ClearAlarmFlag();
					LOG("ClearOverloads()\n");
				}
			}
			
			
			PC_TxPdo.Amplitude 	= (UINT16) ADC::GetAmpPercent();
			PC_TxPdo.Current 	= (UINT16) ADC::GetDCCPercent();
			TempPhase 	= 		 ADC::GetPhaseInDeg();
			
			if ((FPGA::GetGPI() & FPGA_SIGN) == 0)
			{
				TempPhase = (-TempPhase);
			}
			PC_TxPdo.Phase 		= (UINT16)TempPhase;
			PC_TxPdo.Power 		= (UINT16)ADC::GetTotalPwr();
			PC_TxPdo.Frequency  = FPGA::GetFrequency();
					
			FPGA::SetPhase(TempPhase);
			
			switch(FPGA::GetSonicCommand())
			{
				case STOP:
					FPGA::StopSonic();
					FPGA::SetPwmVal(0);
				break;
					
				case START:
					FPGA::StartSonic();
					FPGA::SetPwmVal(0);
					FPGA::SetSonicCommand(RUN);
				break;
					
				case RUN:
					
					break;
					
				case SCAN:
					FPGA::StartSonic();
					break;
				
				default:
					//Put default case
			}
			
			FPGA::SPITransfer( INX_GPI,  INX_GPO );
			FPGA::SPITransfer( INX_RFPGA_STAT,  INX_PWM_VAL );
			FPGA::SPITransfer( INX_FREQ_VAL_OUT,  INX_FREQ_VAL_IN_HI );
					
			FPGAInputs = FPGA::GetGPI();
			FPGAStatus = FPGA::GetStatus();
			stat = GpioGetVal(GPIO_PC_15V_LOW, &val_15V); 
			
			if((GPIO_OK == stat)&&(GPIO_VALUE_LOW == val_15V))
			{
				//15V is not present, send alarm
				Alarm::SetAlarmReason(ERR_15V_LOST);
				PC_TxPdo.PC_StatusEvent|=BIT_MASK(STATUS_SOURCE_LOST);
				PCStateMachine::alarm = TRUE;
				LOG("15V_LOST\n");
			}
			else if (!(FPGAStatus & FPGA_STAT_RUN))
			{
			//FPGA is not running
				//Startup conditions cause FPGA FB error causin the FPGA_STAT_RUN to be 0 due
				//to watchdog check on FPGA side each mlisecond
				if ((Alarm::GetAlarmFlag() == FALSE) && (FPGAStatus & FPGA_STAT_RUN))
				{
					Alarm::SetAlarmFlag();
					Alarm::SetAlarmReason( ERR_CURRENT_OVERLOAD);
				}
			}
			else if ((((~FPGAInputs) & OVL_FUNCTION) == OVL_FUNCTION) && (FPGAInputs != 0))
			{
			// Overload
				if (((~FPGAInputs) & OL_CURRENT) == OL_CURRENT){
					Alarm::SetAlarmFlag();
					Alarm::SetAlarmReason( ERR_CURRENT_OVERLOAD);
					LOG("OL_CURRENT\n");
				}

				if (((~FPGAInputs) & OL_VOLTAGE) == OL_VOLTAGE) {
					Alarm::SetAlarmReason(ERR_VOLTAGE_OVERLOAD);
					Alarm::SetAlarmFlag();
					LOG("OL_VOLTAGE\n");
				}

				if (((~FPGAInputs) & OL_POWER) == OL_POWER) {
					Alarm::SetAlarmReason( ERR_POWER_OVERLOAD );
					Alarm::SetAlarmFlag();
					LOG("OL_POWER\n");
				}

				if (((~FPGAInputs) & OL_TEMPERATURE) == OL_TEMPERATURE){
					Alarm::SetAlarmReason(ERR_TEMP_OVERLOAD);
					Alarm::SetAlarmFlag();
					LOG("OL_TEMP\n");
				}
			}
			else
			{
				PC_TxPdo.PC_StatusEvent &= ~BIT_MASK(STATUS_SOURCE_LOST);
				if(ADC::IsADCReInitRequested())
				{
					ADC::SetADCSequence();
					Alarm::ClearOverloads();
					ADC::SetADCReInitRequest(FALSE);
					LOG("IsADCReInitRequested\n");
				}
				ADC::ReadADC() ;
				ADC::ScaleADCValues();
			}
						
			if(FanStatus == FALSE)
			{
				if(ADC::GetTemperatureInDegC() > FANENABLE_TEMP)
				{
					GpioSetVal(FAN_EN,FALSE); //Turn on FAN
					FanStatus =  TRUE;
				}
			}
			if ((Sonics::Get_SonicsConfigDoneFlag() == TRUE) && ((FPGAStatus & FPGA_STAT_RUN) != 0)) 
			{
				if(PC_TxPdo.PCState == WELD_RUN)
				{
					WeldRun::WeldControlLoop();
				}
				else if(PC_TxPdo.PCState == SEEK_RUN)
				{
					SeekRun::SeekControlLoop();
				}
			}
			if( Alarm::GetAlarmFlag() == TRUE || (PCStateMachine::alarm==TRUE))
			{
				FPGA::StopSonic();
			}
			//GpioSetVal(GPIO_5_19, GPIO_VALUE_LOW); ///Test I/O
		}
    }
}
/*
**********************************************************************************

*      Function    :   FwUpgradeRHandler
*      Description :   Function which Handles the firmware up-gradation of PC module 
*      Input       :   No arguments
*      Output      :   No return type
*      Called by   :   PCMain

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
