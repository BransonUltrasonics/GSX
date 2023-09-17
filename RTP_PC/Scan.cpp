/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     This file contains the HornScan Related functionality..
 
**********************************************************************************************************/
#include <vxWorks.h> 
#include "Scan.h"
#include <stdio.h>
#include "PC.h"
#include "Common.h"
#include "CommFPGATask.h"
#include "PCStateMachine.h"
#include "FPGA.h"
#include "PCObjDictionary.h"
#include "Commons.h"

using namespace std;

UINT8 Scan::ScanEnableFlag;

Scan::Scan()
{
	//Constructor
}

Scan::~Scan()
{
	//Destructor
}

UINT16 Scan::ConvertAmp2PWM(UINT16 Amplitude)
{
	UINT16 PWM=0;
	PWM=PWMVALUE_PER_HUND_OF_PERCENT * (Amplitude * AMPLITUDEFACTOR);
	return (PWM);
}

/******************************************************************************
*      Function    :   Scan::Enter()
*      Description :   This function prepares Scan cycle before Scan is started.
*      Input       :   void
*      Output      :   void
*      Called by   :   None
*******************************************************************************/
void Scan::Enter()
{
	printf ("\nEnter Scan\n");
	 
	WeldError = NO_ERROR;
	PDOFrequency=0;
	AmplitudeChange=0;
	PWM=655;
	OperatingFreq=(PC_RxPdo.Frequency*10);
	StartSCan=false;
	FPGA::SetFrequency(OperatingFreq);
	FPGA::SetPwmVal(0);
	PC_TxPdo.Frequency = FPGA::GetFrequency();
	
	cout << "Scan::Enter" << PC_TxPdo.Frequency << endl;
}

/**************************************************************************************
*      Function    :   Scan::Loop()
*      Description :   This function updated Scan data variables Like Frequency,Power,
*                      Amplitude,Current to the OD
*      Input       :   void
*      Output      :   void
*      Called by   :   None
***************************************************************************************/
void Scan::Loop()
{

	UINT16 TempPWM;

	PC_TxPdo.Frequency = FPGA::GetFrequency();
			
	if(WeldError == NO_ERROR)
	{
		if (PCStateMachine::masterState == PREREADY)
		{		
			PC_TxPdo.PCState=PC_READY;
			ChangeState(PC_READY);
			LOG("SCAN_SONIC_ON -> PC_READY \n");
		}
		else
		{
			if (PCStateMachine::masterState == SCAN_SONIC_ON)
			{				
				PC_TxPdo.PCState=SCAN_RUN;
				StartSCan=true;
				
				 if (PDOFrequency != PC_RxPdo.Frequency)
				 {					 
					 PDOFrequency = PC_RxPdo.Frequency;
					 
					 if (AmplitudeChange!=PC_RxPdo.Amplitudechange)
					 {
						 TempPWM = 655 *(PC_RxPdo.Amplitudechange-AmplitudeChange);
						 AmplitudeChange=PC_RxPdo.Amplitudechange;
						 if ((PWM==0)&&(TempPWM<0))
						 {  
							 TempPWM=0;							 
						 }
						 PWM=PWM+TempPWM;
					 }
					 
					 OperatingFreq=(PC_RxPdo.Frequency*10);
					 FPGA::SetPwmVal(PWM);
					 FPGA::SetFrequency(OperatingFreq);
					 FPGA::SetSonicCommand(SCAN);
			   }
			}
		}
	}			
	
	if((WeldError != NO_ERROR) || (Alarm::GetAlarmFlag() == TRUE) || (PCStateMachine::alarm==TRUE))
	{
		PC_TxPdo.PCState = PC_ALARM;
		ChangeState(PC_ALARM);
	}	
}

/**************************************************************************************
*      Function    :   Scan::Exit()
*      Description :   This function Rests Scan data variables Like Frequency,Power,
*                      Amplitude,Current,SamplesCounter,Timercounter.
*      Input       :   void
*      Output      :   void
*      Called by   :   None
***************************************************************************************/
void Scan::Exit()
{ 
	cout << "Scan::Exit" << endl;
	PC_TxPdo.Frequency = FPGA::GetFrequency();
    StartSCan=false;
	FPGA::SetSonicCommand(STOP);
	
	ObjectDictionary :: tpdo.Frequency = 0;
	ObjectDictionary :: tpdo.Current = 0;
	ObjectDictionary :: tpdo.Amplitude = 0;
	ObjectDictionary :: tpdo.Phase = 0;
	
	LOG("Exit Scan \n");
}
