/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef RUNTIME_H_
#define RUNTIME_H_


#include <vxWorks.h>
#include <iostream>
#include <stdio.h>
#include "Eeprom.h"
#include "Common.h"
#include "SysConfigEeprom.hpp"


#define ENABLE								1
#define DISABLE 							0
#define RUNTIMEDATA							0X52

/*Model default feature settings in bits*/
#define FEATURE_GSX_E                     	{0X55,0X15,0X45,0X55,0X55,0X55,0X55,0XD5,0X55,0XD5,0X5D,0X15,0X00}                                                                                             
#define FEATURE_P1_PNEUMATIC                {0X55,0X15,0X55,0XA8,0X57,0X55,0X05,0XF4,0XD5,0XD3,0X7F,0X24,0X00} 
#define FEATURE_P2_PNEUMATIC                {0X55,0X15,0X55,0X55,0X55,0X55,0X55,0XF5,0X55,0XD5,0X5D,0X35,0X00} 
#define FEATURE_I_IW                       	{0X34,0X1F,0X05,0XA8,0X04,0X50,0X01,0X00,0X08,0X20,0X00,0X40,0X00}                                                                                                 
#define FEATURE_MERCURY                     {0X55,0X55,0X55,0X55,0X55,0X55,0X55,0X54,0X57,0XD1,0X5D,0X55,0X00}                                                                                         
#define FEATURE_SATURNP1                    {0X55,0X55,0X55,0XA9,0X57,0X55,0X55,0X74,0X57,0XD1,0X7F,0X64,0X00}                                                                        
#define FEATURE_SATURNP2                    {0X55,0X55,0X55,0X55,0X55,0X55,0X55,0X74,0X57,0XD1,0X5D,0X75,0X00}                                                                       
#define FEATURE_MARS                        {0X14,0X5F,0X15,0XA8,0X04,0X50,0X01,0X00,0X08,0X20,0X00,0X80,0X00}                                                                                 
#define FEATURE_JUNO                        {0X55,0X50,0X40,0X10,0X55,0X55,0X51,0X45,0X56,0XE5,0X5C,0X90,0X00}                                                                                              
#define FEATURE_VENUS                       {0X54,0X50,0X00,0X10,0X07,0X10,0X01,0X00,0XDA,0XE3,0X0C,0XA0,0X00}                                                                                             
#define FEATURE_VENUSHH                     {0X54,0X50,0X00,0X00,0X00,0X10,0X01,0X00,0X08,0X20,0X00,0XB0,0X00} 
#define FEATURE_ASX                     	{0X15,0X05,0X45,0X55,0X55,0X55,0X55,0XD5,0X55,0XD5,0X5D,0X15,0X00} 
#define MODELID_MASK						0xF0


typedef struct
 {
	
	UINT8	Multimode_Flag_B0										:2;
	UINT8	Time_Flag_B1                       						:2;
	UINT8  	Energy_Flag_B2                    						:2;
	UINT8  	PeakPower_Flag_B3                  						:2;
	UINT8  	CollapseDist_Flag_B4               						:2;
	UINT8  	AbsoluteDist_Flag_B5               						:2;
	UINT8 	GroundDetect_Flag_B6                   					:2;
	UINT8  	Continous_Flag_B7              							:2;
	UINT8  	PreTrigger_Flag_B8                   					:2;
	UINT8  	AfterBurst_Flag_B9                   					:2;
	UINT8  	EnergyBrake_Flag_B10                   					:2;
	UINT8  	AmplitudeStep_Flag_B11                   				:2;
	UINT8  	ForceStep_Flag_B12                   					:2;
	UINT8 	AlarmLog_Flag_B13                   					:2;
	UINT8  	EventLog_Flag_B14                   					:2;
	UINT8  	WeldDataLog_Flag_B15                   					:2;
	UINT8  	Reporting_Flag_B16                   					:2;
	UINT8  	CrackedHorn_Flag_B17                   					:2;
	UINT8  	USB_Flag_B18                   							:2;
	UINT8 	Binary_Flag_B19                   						:2;
	UINT8  	WebServices_Flag_B20                   					:2;
	UINT8  	OPC_UA_FLAG_B21                   						:2;
	UINT8  	UserIO_Flag_B22                   						:2;
	UINT8  	ActuatorIO_Flag_B23                   					:2;
	UINT8  	CycleCounter_Flag_B24                   				:2;
	UINT8  	BatchCounter_Flag_B25                   				:2;
	UINT8  	PowerMatchCurve_Flag_B26                   				:2;
	UINT8  	SmartConverter_Flag_B27                   				:2;
	UINT8  	ControlSpeed_Flag_B28                   				:2;
	UINT8  	Sequencing_Flag_B29                   					:2;
	UINT8  	AutoConverterCooling_Flag_B30                   		:2;
	UINT8  	FieldBus_Flag_B31                   					:2;

}RTF_LIST1;


typedef struct 
{
	
		UINT8  DashBoard_Flag_B32                   				:2;
		UINT8  Password_Flag_B33                   					:2;
		UINT8  Mobile_Flag_B34                   					:2;
		UINT8  Overlay_Flag_B35                   					:2;
		UINT8  Trends_Flag_B36                   					:2;
		UINT8  Optitune_Flag_B37                   					:2;
		UINT8  Limits_Flag_B38                   					:2;
		UINT8  HDMode_Flag_B39                   					:2;
		UINT8  TeachMode_Flag_B40                   				:2;
		UINT8  AdvComm_Flag_B41                   					:2;
		UINT8  ExtraDataStorage_Flag_B42                   			:2;
		UINT8  TroubleShoot_Suggestion_Flag_B43                   	:2;
		UINT8  CFR_Flag_B44                     					:2;
		UINT8  HourCounter_Flag_B45                					:2;
	    UINT8  Reporting_Flag_Csv_B46                               :2;
	    UINT8  Reporting_Flag_Pdf_B47                               :2;
	    UINT8  IODiagnostics_Flag_B48                               :2;
	    UINT8  Dynamic_Flag_B49                                     :2;
	    UINT8  Reserved_B50											:2;
	    UINT8  Reserved_B51											:2;
}RTF_LIST2;

typedef struct
{
	RTF_LIST1 RtFeature1;
	RTF_LIST2 RtFeature2;
	
}FeaturesFlag;

#define FEATUREFLAG_SIZE sizeof(FeaturesFlag)

class RunTime
{
public:	
	RunTime();
	~RunTime();
	
	void		SetDefaultModel();
	bool 		CheckWeldModes();
	bool 		SetSystemFeature(INT32 systemType);
	static void SetModeValue(INT32 weldMode);
	STATUS 		FeaturesFlagRead(UINT32 ModelAddress);
	STATUS 		FeaturesFlagWrite(char *pRunTimeData);
	void 		GetDefaultModelConf(char *pRunTimeConf);
	
private:
	bool		IsFeatureEnabled(UINT8 featureFlag);
	bool 		WriteToEEPROM(UINT8* modelData);
	
public:
	FeaturesFlag ModelFeature;
	Eeprom m_EEPROM;

private:
	static WELDMODES m_Wmodes;
};

#endif /* RUNTIME_H_ */
