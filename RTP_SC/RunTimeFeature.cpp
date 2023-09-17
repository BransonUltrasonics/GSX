/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------------------------------------

		FetauresFlag reads the model present in the eeprom , checking the weld mode which is active
 
**********************************************************************************************************/

#include "RunTimeFeature.h"
#include "Eeprom.h"

using namespace std;

UINT8 featureGSX_E[FEATUREFLAG_SIZE]			=	FEATURE_GSX_E;  		/*GSX_E model default values*/
UINT8 featureP1Pneumatic[FEATUREFLAG_SIZE]		=	FEATURE_P1_PNEUMATIC;  	/*p1pneumatic model default values*/
UINT8 featureP2Pneumatic[FEATUREFLAG_SIZE]		=	FEATURE_P2_PNEUMATIC;  	/*p2pneumatic model default values*/
UINT8 featureIIW[FEATUREFLAG_SIZE]				=	FEATURE_I_IW;  			/*i(iw)model default values*/
UINT8 featureMercury[FEATUREFLAG_SIZE]			=	FEATURE_MERCURY; 		/*mercury model default values*/
UINT8 featureSaturnP1[FEATUREFLAG_SIZE]			=	FEATURE_SATURNP1;  		/*saturnp1 model default values*/
UINT8 featureSaturnP2[FEATUREFLAG_SIZE]			=	FEATURE_SATURNP2;  		/*saturnp2 model default values*/
UINT8 featureMars[FEATUREFLAG_SIZE]				=	FEATURE_MARS;			/*mars model default values*/
UINT8 featureJuno[FEATUREFLAG_SIZE]				=	FEATURE_JUNO;  			/*juno model default values*/
UINT8 featureVenus[FEATUREFLAG_SIZE]			=	FEATURE_VENUS;			/*venus model default values*/
UINT8 featureVenusHH[FEATUREFLAG_SIZE]			=	FEATURE_VENUSHH;  		/*Venus HH model default values*/
UINT8 featureASX[FEATUREFLAG_SIZE]			    =	FEATURE_ASX; 

WELDMODES RunTime::m_Wmodes = TIMEMODE;

/**************************************************************************//**
* \brief   -  Constructor
* 
* \param   -  None
*
* \return  -  None
*
******************************************************************************/
RunTime::RunTime()
{	
	
}

/**************************************************************************//**
* \brief   -  Destructor
* 
* \param   -  None
*
* \return  -  None
*
******************************************************************************/
RunTime::~RunTime()
{		
	
}

/**************************************************************************//**
* \brief   -  Reading EEPROM at the given address for the runtime feature configuration
* 
* \param   -  UINT32 ModelAddress
*
* \return  -  STATUS - TRUE/ERROR
*
******************************************************************************/
STATUS RunTime::FeaturesFlagRead(UINT32 ModelAddress)
{
	ssize_t ReadSize;
	if((ReadSize=m_EEPROM.Read((char *)&ModelFeature,FEATUREFLAG_SIZE,ModelAddress))==ERROR)
	{
		LOGDBG("RunTime_T : ------------ERROR IN FeaturesFlagRead------------",0,0,0);
		return ERROR;
	}
	
	return TRUE;
}

/**************************************************************************//**
* \brief   -  Writing EEPROM at the given address for the runtime 
* 			  feature configuration from UIC
* 
* \param   -  char *pRunTimeData : Run time data from UI
*
* \return  -  STATUS - TRUE/ERROR
*
******************************************************************************/
STATUS RunTime::FeaturesFlagWrite(char *pRunTimeData)
{	
	STATUS writeStatus = TRUE;
	if(m_EEPROM.Write(pRunTimeData, FEATUREFLAG_SIZE, EEPROM_MODEL_ADDRESS) == ERROR)
	{
		LOGERR("RunTime_T : ------------ERROR IN FeaturesFlagWrite------------",0,0,0);
		writeStatus = FALSE;
	}	
	return writeStatus;
}

/**************************************************************************//**
* \brief   - Setting the active weld mode
* 
* \param   - INT32 weldMode
*
* \return  - None
*
******************************************************************************/
void RunTime::SetModeValue(INT32 weldMode)
{
	m_Wmodes = (WELDMODES)weldMode;
}

/**************************************************************************//**
* \brief   - Checks the feature flag is enabled or disabled
* 
* \param   - UINT8 featureFlag
*
* \return  - bool bIsEnabled
*
******************************************************************************/
bool RunTime::IsFeatureEnabled(UINT8 featureFlag)
{
	bool bIsEnabled = false;
	
	if(ENABLE == featureFlag)
	{
		bIsEnabled = true;
	}
	
	return bIsEnabled;
}

/**************************************************************************//**
* \brief   - Checking the active recipe and the corresponding flag
* 
* \param   - None
*
* \return  - bool bIsEnabled
*
******************************************************************************/
bool RunTime::CheckWeldModes()
{
	bool bIsEnabled = false;
	
	FeaturesFlagRead(EEPROM_MODEL_ADDRESS);
	
	switch(m_Wmodes)
	{
		case TIMEMODE:
			bIsEnabled = IsFeatureEnabled(ModelFeature.RtFeature1.Time_Flag_B1);
			break;
	
		case ENERGYMODE: 
			bIsEnabled = IsFeatureEnabled(ModelFeature.RtFeature1.Energy_Flag_B2);
			break;
	
		case PEAKPOWERMODE: 
			bIsEnabled = IsFeatureEnabled(ModelFeature.RtFeature1.PeakPower_Flag_B3);
			break;
	
		case GRDDETECTMODE: 
			bIsEnabled = IsFeatureEnabled(ModelFeature.RtFeature1.GroundDetect_Flag_B6);
			break;
	
		case ABSMODE: 
			bIsEnabled = IsFeatureEnabled(ModelFeature.RtFeature1.AbsoluteDist_Flag_B5);
			break;
			
		case COLLAPSEMODE: 
			bIsEnabled = IsFeatureEnabled(ModelFeature.RtFeature1.CollapseDist_Flag_B4);
			break;
	
		case CONTINUOUS: 
			bIsEnabled = IsFeatureEnabled(ModelFeature.RtFeature1.Continous_Flag_B7);
			break;
			
		case DYNAMICMODE: 
			bIsEnabled = IsFeatureEnabled(ModelFeature.RtFeature2.Dynamic_Flag_B49);
			break;
	
		default :
			LOGERR("RunTime_T : ------------NO MODE SELECTED------------",0,0,0);
			break;
	}
	
	return bIsEnabled;
}

/**************************************************************************//**
* \brief   - Setting Model that is selected from Control level of system Configuration
* 
* \param   - UINT8* modelData
*
* \return  - bool bIsValid
*
******************************************************************************/
bool RunTime::WriteToEEPROM(UINT8* modelData)
{
	bool bIsValid = true;

	if(ERROR == m_EEPROM.Write((char *)modelData, FEATUREFLAG_SIZE, EEPROM_MODEL_ADDRESS))
	{
		LOGERR("RunTime_T : ------------ERROR IN SET MODEL------------",0,0,0);
		bIsValid = false;
	}

	return bIsValid;
}

/**************************************************************************//**
* \brief   - Setting SystemFeatures that is selected from system type of system Configuration
* 
* \param   - INT32 system type 
*
* \return  - bool bIsConfigured
*
******************************************************************************/
bool RunTime::SetSystemFeature(INT32 systemType)
{
	bool bIsConfigured = false;

	switch(systemType)
	{
		case GSX_E_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureGSX_E);
			break;
	
		case P1_PNEUMATIC_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureP1Pneumatic);
			break;
	
		case P2_PNEUMATIC_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureP2Pneumatic);
			break;
			
		case I_IW_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureIIW);
			break;
			
		case MERCURY_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureMercury);
			break;
	
		case SATURNP1_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureSaturnP1);
			break;
			
		case SATURNP2_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureSaturnP2);
			break;
			
		case MARS_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureMars);
			break;
			
		case JUNO_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureJuno);
			break;
			
		case VENUS_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureVenus);
			break;
			
		case VENUSHH_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureVenusHH);
			break;
			
		case ASX_SYSTEM:
			bIsConfigured = WriteToEEPROM(featureASX);
			break;	
			
		default:
			LOGERR("RunTime_T : ------------NO MODEL SET------------",0,0,0);
			break;
	}
	
	return bIsConfigured;
}

/**************************************************************************//**
* \brief   - Check run time feature is configured or not.If not then set default
* 			 model as GSX_E1. 
* 
* \param   - None
*
* \return  - None
*
******************************************************************************/
void RunTime::SetDefaultModel()
{
	char buffer = {0x00};
	char RunTimeData=RUNTIMEDATA;
	bool bIsConfigured = false;
	
	m_EEPROM.Read(&buffer, sizeof(buffer), RUN_TIME_CONFIGURE);
	
	if('R' != buffer)
	{
		m_EEPROM.Write(&RunTimeData,sizeof(buffer),RUN_TIME_CONFIGURE);
		
		bIsConfigured = SetSystemFeature(GSX_E_SYSTEM );
		
		if(false == bIsConfigured)
		{
			LOGERR("Failed to configure default model of system",0,0,0);
		}
		else
		{
			LOGDBG("GSX_E1 model has been set as default runtime feature",0,0,0);
		}
	}
}

/**************************************************************************//**
* \brief   - Get default model run time configuration
* 
* \param   - char *pRunTimeConf
*
* \return  - None
*
******************************************************************************/
void RunTime :: GetDefaultModelConf(char *pRunTimeConf)
{	
	memcpy(pRunTimeConf, featureGSX_E, FEATUREFLAG_SIZE);
}
