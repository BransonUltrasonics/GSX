/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Sysconfig EEprom class functions for system configuration read and write  
 
**********************************************************************************************************/

#include "SysConfigEeprom.hpp"
#include <string.h>
#include "RunTimeFeature.h"
#include "CommonProperty.h"
#include "commons.h"
#include "AlarmManager.h"

using namespace std;
char  SystemConfiguration::systemTypeName[SYSINFO_SIZE]="";
//#define EE_DEBUG
/**************************************************************************//**
* \brief   - Constructor - initialize the data members of SysConfig.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
SystemConfiguration::SystemConfiguration()
{	
}

/**************************************************************************//**
* \brief   -  Check if system configuration data read from EEPROM is corrupted
*      		 
*
* \param   - none 
*
* \return  - bool: true-- Not Corrupted; false--Corrupted
*
******************************************************************************/
bool  SystemConfiguration::SysConfigCheck()
{
	bool notCorrupted = false;
	INT32 i = 0;
	
#ifdef EE_DEBUG	
   	printf("Enter SysConfigCheck()\n");
#endif
   	
	/* Check if old System Configuration structure is corrupted */
	if ((OLD_m_SysConfig.LanguageIs >= ENGLISH ) && (OLD_m_SysConfig.LanguageIs < MAX_Language))
	{
		if ((OLD_m_SysConfig.Sscreen >= DASHBOARD) && (OLD_m_SysConfig.Sscreen < MAX_StartScreen))
		{
			if ((OLD_m_SysConfig.UnitIs >= METRIC) && (OLD_m_SysConfig.UnitIs < MAX_Units))
			{
				if ((OLD_m_SysConfig.PsPowerUpOptionIs >= PS_SEEK) && (OLD_m_SysConfig.PsPowerUpOptionIs < MAX_PsPowerUpOptions))
				{
					if ((OLD_m_SysConfig.MemFullActionIs >= MEM_FULL_STOP) && (OLD_m_SysConfig.MemFullActionIs <= MEM_FULL_CONTINUE))
					{
						if ((OLD_m_SysConfig.AuthCheckLevel >= DISABLED) && (OLD_m_SysConfig.AuthCheckLevel <= FULL))
						{
							notCorrupted = true;
						}
					}
				}
			}				
		}
	}

	if (notCorrupted == true)
	{
		if (!(((OLD_m_SysConfig.ActConfig.ActuatorTypeIs >= SERVO ) && (OLD_m_SysConfig.ActConfig.ActuatorTypeIs < MAX_ActuatorType))
		   &&((OLD_m_SysConfig.ActConfig.ControlLevelIs >= GSX_E) && (OLD_m_SysConfig.ActConfig.ControlLevelIs < MAX_ControlLevel))
		   &&((OLD_m_SysConfig.ActConfig.MaxStrokelength >= 0) && (OLD_m_SysConfig.ActConfig.MaxStrokelength <= 125))))
		{
			notCorrupted = false;
#ifdef EE_DEBUG			
		   	printf("Old System Configuration corrupted1 !\n");
#endif
		}
	}
	else
	{
	   	printf("Old System Configuration corrupted1 !\n");	
	}
	
	if (notCorrupted == true)
	{
		if (!(((OLD_m_SysConfig.PcConfig.ControlLevelIs >= GSX_E) && (OLD_m_SysConfig.PcConfig.ControlLevelIs < MAX_ControlLevel))
				&&((OLD_m_SysConfig.PcConfig.PSFreq >= 0) && (OLD_m_SysConfig.PcConfig.PSFreq <= 40000))
				&&((OLD_m_SysConfig.PcConfig.PSWatt >= 0) && (OLD_m_SysConfig.PcConfig.PSWatt <= 4000))
				&&(OLD_m_SysConfig.PcConfig.PsTypeIs < MAX_PSType)))		      
		{
			notCorrupted = false;
#ifdef EE_DEBUG			
		   	printf("Old System Configuration corrupted2!\n");
			printf("OLD_m_SysConfig.PcConfig.ControlLevelI:%d\n",OLD_m_SysConfig.PcConfig.ControlLevelIs);
			printf("OLD_m_SysConfig.PcConfig.PSFreq:%d\n",OLD_m_SysConfig.PcConfig.PSFreq);
			printf("OLD_m_SysConfig.PcConfig.PSWatt:%d\n",OLD_m_SysConfig.PcConfig.PSWatt);
			printf("OLD_m_SysConfig.PcConfig.PsTypeIs:%d\n",OLD_m_SysConfig.PcConfig.PsTypeIs);
#endif
		}
	}
	
	if ((notCorrupted == true)&&(OLD_m_SysConfig.systemType >= MAX_SystemType))
	{
			notCorrupted = false;	
#ifdef EE_DEBUG			
		   	printf("Old System Configuration corrupted3!\n");
#endif
	}
	
	
	if (notCorrupted == true)
	{
#ifdef EE_DEBUG
		printf("Machine Name:%s\n",OLD_m_SysConfig.machineName);
		printf("SC_SerialNumber:%s\n",OLD_m_SysConfig.SC_SerialNumber);
		printf("PC_SerialNumber:%s\n",OLD_m_SysConfig.PC_SerialNumber);
		printf("AC_SerialNumber:%s\n",OLD_m_SysConfig.AC_SerialNumber);
		printf("AuxBoxSerialNumber:%s\n",OLD_m_SysConfig.AuxBoxSerialNumber);
#endif
		for (i = 0; (OLD_m_SysConfig.machineName[i] != '\0') && (i < (MACHINE_NAME_SIZE - 1)); i++)
		{			
			if ((OLD_m_SysConfig.machineName[i] < 0x20) || (OLD_m_SysConfig.machineName[i] > 0x7E))
			{
				notCorrupted = false;
#ifdef EE_DEBUG
			   	printf("Old System Configuration corrupted4!\n");
#endif
			}			
		}
		
		i = 0;
		for (i = 0; (OLD_m_SysConfig.SC_SerialNumber[i] != '\0') && (i < (SC_SERIAL_NUMBER_SIZE - 1)); i++)
		{
			if ((OLD_m_SysConfig.SC_SerialNumber[i] < 0x20) || (OLD_m_SysConfig.SC_SerialNumber[i] > 0x7E))
			{
				notCorrupted = false;
#ifdef EE_DEBUG				
			   	printf("Old System Configuration corrupted5!\n");
#endif			   	
			}
		}
		
		i = 0;
		for (i = 0; (OLD_m_SysConfig.AuxBoxSerialNumber[i] != '\0') && (i < (AUXBOX_SERIAL_NUMBER_SIZE - 1)); i++)
		{
			if ((OLD_m_SysConfig.AuxBoxSerialNumber[i] < 0x20) || (OLD_m_SysConfig.AuxBoxSerialNumber[i] > 0x7E))
			{
				notCorrupted = false;
#ifdef EE_DEBUG				
			   	printf("Old System Configuration corrupted6!\n");
#endif
			}
		}

		i = 0;
		for (i = 0; (OLD_m_SysConfig.PC_SerialNumber[i] != '\0') && (i < (PC_SERIAL_NUMBER_SIZE -1)); i++)
		{
			if ((OLD_m_SysConfig.PC_SerialNumber[i] < 0x20) || (OLD_m_SysConfig.PC_SerialNumber[i] > 0x7E))	
			{
				notCorrupted = false;
#ifdef EE_DEBUG				
			   	printf("Old System Configuration corrupted7!\n");
#endif			   	
			} 
		}

		i = 0;
		for (i = 0; (OLD_m_SysConfig.AC_SerialNumber[i] != '\0') && (i < (AC_SERIAL_NUMBER_SIZE - 1)); i++)
		{
			if ((OLD_m_SysConfig.AC_SerialNumber[i] < 0x20) || (OLD_m_SysConfig.AC_SerialNumber[i] > 0x7E))
			{
				notCorrupted = false;
#ifdef EE_DEBUG				
			   	printf("Old System Configuration corrupted8!\n");
#endif
			} 
		}		
	}
	
	return notCorrupted;
}

/**************************************************************************//**
* \brief   - Verify EEPROM configured with system configuration 
*      		details or not.
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool SystemConfiguration::ValidateEEPROM()
{
	bool bIsEmpty = false;
	char eeprom_sig_old, eeprom_sig_new;
	Eeprom epclass;
	bool notCorrupted = false;
		
#ifdef EE_DEBUG
	INT32 psFreq = 0, psPower = 0;

   	printf("Enter ValidateEEPROM()\n");
#endif
   	   	
	//Check if new SysConfig structure is already written to EEPROM
	epclass.Read(&eeprom_sig_new,SYSCONFIG_IDENTIFIER_SIZE,SYSCONFIG_BASE);
	MsDelay(6);

	if (eeprom_sig_new != SYSCONFIGSIG)
	{		
		epclass.Read(&eeprom_sig_old,SYSCONFIG_IDENTIFIER_SIZE,OLD_SYSCONFIG_BASE);
		MsDelay(6);

#if(0)
#ifdef EE_DEBUG		
		OLD_m_SysConfig.bIsExternalPresets 		= true;
		OLD_m_SysConfig.bIsHornClamp	 		= false;
		OLD_m_SysConfig.LanguageIs 				= ENGLISH;
		OLD_m_SysConfig.UnitIs					= METRIC;
		OLD_m_SysConfig.PsPowerUpOptionIs 		= PS_SEEK;
		OLD_m_SysConfig.MemFullActionIs 		= MEM_FULL_CONTINUE;
		OLD_m_SysConfig.AuthCheckLevel			= FULL;
		OLD_m_SysConfig.PresetRecipeBarcodeStart= 'R';				
		OLD_m_SysConfig.bPartIDSwitchStatus		= false;
		
		OLD_m_SysConfig.ActConfig.ActuatorTypeIs= SERVO;
		OLD_m_SysConfig.ActConfig.ControlLevelIs= GSX_E;
		OLD_m_SysConfig.ActConfig.MaxStrokelength= 125;
		OLD_m_SysConfig.PcConfig.ControlLevelIs = GSX_E;
		OLD_m_SysConfig.PcConfig.PsTypeIs 		= PS_STANDARD;
		OLD_m_SysConfig.bPowerUpMem				= true;
		OLD_m_SysConfig.bResetMem				= true;
		OLD_m_SysConfig.systemType				= GSX_E1;

		strcpy(OLD_m_SysConfig.machineName,"UW-M/C-NAME");
		strcpy(OLD_m_SysConfig.SC_SerialNumber,"DEF12345");
		strcpy(OLD_m_SysConfig.AuxBoxSerialNumber,"DEF12345");
		strcpy(OLD_m_SysConfig.PC_SerialNumber,"DEF12345");
		strcpy(OLD_m_SysConfig.AC_SerialNumber,"DEF12345");
		psFreq 	= FREQ_20KHz;
		psPower = POWER_20KHz;
		
		if(FREQ_20KHz == psFreq)
		{
			OLD_m_SysConfig.PcConfig.PSFreq 	= KHz_20;
		}
		else if(FREQ_30KHz == psFreq)
		{
			OLD_m_SysConfig.PcConfig.PSFreq 	= KHz_30;
		}
		else if(FREQ_40KHz == psFreq)
		{
			OLD_m_SysConfig.PcConfig.PSFreq 	= KHz_40;
		}
		
		OLD_m_SysConfig.PcConfig.PSWatt 		= psPower;
							
		epclass.Write((char *)&OLD_m_SysConfig,sizeof(OLD_m_SysConfig),OLD_SYSCONFIG_ADDR);
		MsDelay(20);
#endif
#endif	
		if (eeprom_sig_old == SYSCONFIGSIG)
		{
#ifdef EE_DEBUG
			printf("eeprom_sig_old\n");
#endif
				
			epclass.Read((char *)&OLD_m_SysConfig,sizeof(OLD_m_SysConfig),OLD_SYSCONFIG_ADDR);
			MsDelay(6);
			
#ifdef EE_DEBUG	
			printf("\n************************************* OLD SYSTEM CONFIGURATION *************************************\n\n");
			printf("______________________________________________________________________________________________________________\n");
			printf("SERIAL NO.	PARAMETER				:			CURRENT VALUE\n\n");
			printf("2.			ExternalPresets			:			%d\n",OLD_m_SysConfig.bIsExternalPresets);
			printf("3.			HornClamp			:			%d\n",OLD_m_SysConfig.bIsHornClamp);
			printf("4.			Language			:			%d\n",OLD_m_SysConfig.LanguageIs);
			printf("5.			StartScreen			:			%d\n",OLD_m_SysConfig.Sscreen);
			printf("6.			Units				:			%d\n",OLD_m_SysConfig.UnitIs);
			printf("7.			PsPowerUpOption			:			%d\n",OLD_m_SysConfig.PsPowerUpOptionIs);
			printf("8.			MemFullActionIs			:			%d\n",OLD_m_SysConfig.MemFullActionIs);
			printf("9.			AuthCheckLevel			:			%d\n",OLD_m_SysConfig.AuthCheckLevel);
			printf("10.			ActuatorConfig			:			%d\n",OLD_m_SysConfig.ActConfig.ActuatorTypeIs);
			printf("11.			MaxStrokelength			:			%d\n",OLD_m_SysConfig.ActConfig.MaxStrokelength);
			printf("12.			ControlLevelIs			:			%d\n",OLD_m_SysConfig.PcConfig.ControlLevelIs); 
			printf("13.			PSFreq				:			%d\n",OLD_m_SysConfig.PcConfig.PSFreq);
			printf("14.			PSWatt				:			%d\n",OLD_m_SysConfig.PcConfig.PSWatt);
			printf("15.			PsType				:			%d\n",OLD_m_SysConfig.PcConfig.PsTypeIs);
			printf("17.			PowerUpMem			:			%d\n",OLD_m_SysConfig.bPowerUpMem);
			printf("15.			Machine Name			:			%s\n",OLD_m_SysConfig.machineName);
			printf("16.			Recipe Prefix			:			%c\n",OLD_m_SysConfig.PresetRecipeBarcodeStart);
			printf("17.			PartID Switch			:			%d\n",OLD_m_SysConfig.bPartIDSwitchStatus);
			printf("15.			SC Serial Number		:			%s\n",OLD_m_SysConfig.SC_SerialNumber);
			printf("16.			System type			:			%d\n",OLD_m_SysConfig.systemType);
			printf("________________________________________________________________________________________________________________\n");				
#endif	
			
			notCorrupted = SysConfigCheck();
#ifdef EE_DEBUG			
		   	printf("notCorrupted: %d\n", notCorrupted);
#endif
		   	notCorrupted = true;
		   	
			if(notCorrupted == true)
			{	
#ifdef EE_DEBUG				
			   	printf("Enter copy\n");
#endif
			   	/* copy the old SysConfig structure to the new one in the first page at 0x180*/			
				m_SysConfig.LanguageIs 				 	= OLD_m_SysConfig.LanguageIs;
				m_SysConfig.Sscreen 					= OLD_m_SysConfig.Sscreen;
				m_SysConfig.UnitIs						= OLD_m_SysConfig.UnitIs;
				m_SysConfig.PsPowerUpOptionIs 			= OLD_m_SysConfig.PsPowerUpOptionIs;
				m_SysConfig.PcConfig.ControlLevelIs 	= OLD_m_SysConfig.PcConfig.ControlLevelIs;
				m_SysConfig.PcConfig.PSWatt 			= OLD_m_SysConfig.PcConfig.PSWatt;
				m_SysConfig.PcConfig.PsTypeIs 			= OLD_m_SysConfig.PcConfig.PsTypeIs;
				m_SysConfig.PcConfig.PSFreq 			= OLD_m_SysConfig.PcConfig.PSFreq;
				m_SysConfig.bIsExternalPresets 			= OLD_m_SysConfig.bIsExternalPresets;
				m_SysConfig.bIsHornClamp	 			= OLD_m_SysConfig.bIsHornClamp;
				strcpy(m_SysConfig.machineName,OLD_m_SysConfig.machineName);
	
				/* copy the old SysConfig structure to the new one in the second page at 0x1c0 */
				m_SysConfig.MemFullActionIs 			= OLD_m_SysConfig.MemFullActionIs;
				m_SysConfig.AuthCheckLevel				= OLD_m_SysConfig.AuthCheckLevel;
				m_SysConfig.bResetMem					= OLD_m_SysConfig.bResetMem;
				m_SysConfig.bPowerUpMem					= OLD_m_SysConfig.bPowerUpMem;
				m_SysConfig.PresetRecipeBarcodeStart	= OLD_m_SysConfig.PresetRecipeBarcodeStart;				
				m_SysConfig.bPartIDSwitchStatus			= OLD_m_SysConfig.bPartIDSwitchStatus;
				m_SysConfig.systemType					= OLD_m_SysConfig.systemType;
				strcpy(m_SysConfig.SC_SerialNumber,OLD_m_SysConfig.SC_SerialNumber);
				strcpy(m_SysConfig.AuxBoxSerialNumber,OLD_m_SysConfig.AuxBoxSerialNumber);
				strcpy(m_SysConfig.PC_SerialNumber,OLD_m_SysConfig.PC_SerialNumber);
				strcpy(m_SysConfig.AC_SerialNumber,OLD_m_SysConfig.AC_SerialNumber);
				
				/* copy the old SysConfig structure to the new one in the third page 0x200 */
				m_SysConfig.ActConfig.ActuatorTypeIs= OLD_m_SysConfig.ActConfig.ActuatorTypeIs;
				m_SysConfig.ActConfig.ControlLevelIs= OLD_m_SysConfig.ActConfig.ControlLevelIs;
				m_SysConfig.ActConfig.MaxStrokelength= OLD_m_SysConfig.ActConfig.MaxStrokelength;	
				
				epclass.Write((char *)&m_SysConfig,sizeof(m_SysConfig),SYSCONFIG_ADDR);		
				MsDelay(20);

				eeprom_sig_new = SYSCONFIGSIG;
				epclass.Write(&eeprom_sig_new,SYSCONFIG_IDENTIFIER_SIZE,SYSCONFIG_BASE);	
				MsDelay(6);
			   	printf("Write(&eeprom_sig_new\n");


#ifdef EE_DEBUG								
				epclass.Read((char *)&m_SysConfig,sizeof(m_SysConfig),SYSCONFIG_ADDR);	
				MsDelay(1);
				SysConfigDetails(m_SysConfig);	
#endif				
			}
			else
			{
				/* If old EEPROM System Configuration corrupted, Using default settings */
				bIsEmpty = true;

				/* Send Old System Configuration corrupted Alarm */
#ifdef EE_DEBUG								
			   	printf("Old System Configuration corrupted!\n");
#endif				
				LOGERR("Old System Configuration corrupted!\n", 0, 0, 0);

				AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_EEPROM_SYSCONFIG_WRA);					
			}
		}
		else
		{
			bIsEmpty = true;
		}		
	}
		
	return bIsEmpty;
}

/**************************************************************************//**
* \brief   - Configure eeprom with default system configuration 
*      		settings.
*
* \param   - None
*
* \return  - bool - bIsEmpty (TRUE or FALSE)
*
******************************************************************************/
bool SystemConfiguration::ConfigureDefaultSettings()
{	
	INT32 psFreq = 0, psPower = 0;
	RunTime RT;
	CommonProperty *CP = CommonProperty::getInstance();
	Eeprom epclass;
	char eeprom_sig_new = SYSCONFIGSIG;
	
	bool bIsEmpty = ValidateEEPROM();	/* Cross checks system configuration eeprom memory */
	RT.SetDefaultModel();				/* Cross checks runtime configuration eeprom memory */
	RT.FeaturesFlagRead(EEPROM_MODEL_ADDRESS);
	
	m_SysConfig.bIsExternalPresets 		= true;
	m_SysConfig.bIsHornClamp	 		= false;
	m_SysConfig.LanguageIs 				= ENGLISH;
	m_SysConfig.UnitIs					= METRIC;
	m_SysConfig.PsPowerUpOptionIs 		= PS_SEEK;
	m_SysConfig.MemFullActionIs 		= MEM_FULL_CONTINUE;
	m_SysConfig.AuthCheckLevel			= FULL;
	m_SysConfig.PresetRecipeBarcodeStart= 'R';				
	m_SysConfig.bPartIDSwitchStatus		= false;
	
	m_SysConfig.ActConfig.ActuatorTypeIs= SERVO;
	m_SysConfig.ActConfig.ControlLevelIs= GSX_E;
	m_SysConfig.ActConfig.MaxStrokelength= 125;
	m_SysConfig.PcConfig.ControlLevelIs = GSX_E;
	m_SysConfig.PcConfig.PsTypeIs 		= PS_STANDARD;
	m_SysConfig.bPowerUpMem				= true;
	m_SysConfig.bResetMem				= true;
	m_SysConfig.systemType				= GSX_E_SYSTEM ;
	
	strcpy(m_SysConfig.machineName,"UW-M/C-NAME");
	strcpy(m_SysConfig.SC_SerialNumber,"DEF1234");
	strcpy(m_SysConfig.AuxBoxSerialNumber,"DEF1234");
	strcpy(m_SysConfig.PC_SerialNumber,"DEF1234");
	strcpy(m_SysConfig.AC_SerialNumber,"DEF1234");

	/* Validates the feature is enabled or not for current model */ 
	if(ENABLE == RT.ModelFeature.RtFeature2.DashBoard_Flag_B32)
	{
		m_SysConfig.Sscreen = DASHBOARD;
	}
	else
	{
		m_SysConfig.Sscreen = PRODUCTION;
	}

	psFreq 	= FREQ_20KHz;
	psPower = POWER_20KHz;
	
	if(FREQ_20KHz == psFreq)
	{
		m_SysConfig.PcConfig.PSFreq 	= KHz_20;
	}
	else if(FREQ_30KHz == psFreq)
	{
		m_SysConfig.PcConfig.PSFreq 	= KHz_30;
	}
	else if(FREQ_40KHz == psFreq)
	{
		m_SysConfig.PcConfig.PSFreq 	= KHz_40;
	}
	
	m_SysConfig.PcConfig.PSWatt 		= psPower;
	
	/* Checks the eeprom configured with system configuration, If not, Then writes default configuration */
	if(bIsEmpty)	
	{	
		SysConfigSet((char *)&m_SysConfig);
		//After setting the System Configuration to default it's necessary to set the SYSCONFIG_BASE to 0x55 
		//so it does not reset again on the next power cycle.
		epclass.Write(&eeprom_sig_new,SYSCONFIG_IDENTIFIER_SIZE,SYSCONFIG_BASE);	
		MsDelay(6);
	}

	
	return bIsEmpty;
}

/**************************************************************************//**
* \brief   -  Reads system configuration data from EEPROM & copies
*      		 into pSystemConfigData buffer.
*
* \param   - char *pSysConfigData 
*
* \return  - None
*
******************************************************************************/
void SystemConfiguration::SysConfigGet(char *pSysConfigData)
{
	Eeprom epclass;
	CommonProperty *CP = CommonProperty::getInstance();
	
	epclass.Read(pSysConfigData,sizeof(m_SysConfig),SYSCONFIG_ADDR);

	memcpy(&m_SysConfig,pSysConfigData,sizeof(m_SysConfig));

	SysConfigDetails(m_SysConfig);
	
	CP->stackRecipe.SetData(StackRecipe::CLEARMEMORY_AT_POWERUP, m_SysConfig.bPowerUpMem);
	CP->stackRecipe.SetData(StackRecipe::CLEARMEMORY_WITH_RESET, m_SysConfig.bResetMem);
}

/**************************************************************************//**
* \brief   -  Writes system configuration data into EEPROM.
*
* \param   - char *pSysConfigData 
*
* \return  - None
*
******************************************************************************/
void SystemConfiguration::SysConfigSet(char *pSysConfigData)
{
	RunTime RT;
	Eeprom epclass;
	char _systemConfig_read[MAX_SIZE_OF_MSG_BUFF];
	intf_SysConfig_t _systemConfig_old;
	
	memcpy(&m_SysConfig,pSysConfigData,sizeof(m_SysConfig));
	
	//Reads actual system configuration values.
	epclass.Read(_systemConfig_read, sizeof(_systemConfig_old), SYSCONFIG_ADDR);
	memcpy(&_systemConfig_old, _systemConfig_read, sizeof (_systemConfig_old));
	
	//If there was a SYSTEM TYPE change the RunTime Features are changed accordingly.
	if (_systemConfig_old.systemType != m_SysConfig.systemType)
		RT.SetSystemFeature(m_SysConfig.systemType);

	epclass.Write((char *)&m_SysConfig,sizeof(m_SysConfig),SYSCONFIG_ADDR);

#ifdef EE_DEBUG
	SysConfigDetails(m_SysConfig);
#endif

#ifdef DEBUG
	SysConfigDetails(m_SysConfig);
#endif
}

/**************************************************************************//**
* \brief   - Display the system configuration in detail.
*
* \param   - char *pSysConfigData 
*
* \return  - None
*
******************************************************************************/
void SystemConfiguration::SysConfigDetails(intf_SysConfig_t& sysConfig)
{	
#ifdef DEBUG
	printf("\n************************************* SYSTEM CONFIGURATION *************************************\n\n");
	printf("______________________________________________________________________________________________________________\n");
	printf("SERIAL NO.	PARAMETER			:    CURRENT VALUE\n\n");
	printf("2.			ExternalPresets		:    %d\n",sysConfig.bIsExternalPresets);
	printf("3.			HornClamp			:    %d\n",sysConfig.bIsHornClamp);
	printf("4.			Language			:    %d\n",sysConfig.LanguageIs);
	printf("5.			StartScreen			:    %d\n",sysConfig.Sscreen);
	printf("6.			Units				:    %d\n",sysConfig.UnitIs);
	printf("7.			PsPowerUpOption		:    %d\n",sysConfig.PsPowerUpOptionIs);
	printf("8.			MemFullActionIs		:    %d\n",sysConfig.MemFullActionIs);
	printf("9.			AuthCheckLevel		:    %d\n",sysConfig.AuthCheckLevel);
	printf("10.			ActuatorConfig		:    %d\n",sysConfig.ActConfig.ActuatorTypeIs);
	printf("11.			AC Serial Number	:    %s\n",sysConfig.AC_SerialNumber);
	printf("12.			MaxStrokelength		:    %d\n",sysConfig.ActConfig.MaxStrokelength);
	printf("13.			ControlLevelIs		:    %d\n",sysConfig.PcConfig.ControlLevelIs); 
	printf("14.			PSFreq				:    %d\n",sysConfig.PcConfig.PSFreq);
	printf("15.			PSWatt				:    %d\n",sysConfig.PcConfig.PSWatt);
	printf("16.			PsType				:    %d\n",sysConfig.PcConfig.PsTypeIs);
	printf("17.			PowerUpMem			:    %d\n",sysConfig.bPowerUpMem);
	printf("18.			Machine Name		:    %s\n",sysConfig.machineName);
	printf("19.			Recipe Prefix		:    %c\n",sysConfig.PresetRecipeBarcodeStart);
	printf("20.			PartID Switch		:    %d\n",sysConfig.bPartIDSwitchStatus);
	printf("21.			SC Serial Number	:    %-12s\n",sysConfig.SC_SerialNumber);
	printf("22.			System type			:    %d\n",sysConfig.systemType);
	printf("________________________________________________________________________________________________________________\n");
	
#endif	
}


/**************************************************************************//**
* \brief   -  Factory reset everything in eeprom.
*
* \param   - None 
*
* \return  - STATUS (-1 or  0)
*
******************************************************************************/
STATUS SystemConfiguration :: FactoryResetEeprom()		//TODO:Note it requires a reboot of the system, clearing only flags.
{		
	Eeprom 	eeprom;
	char resetEeprom = 0;	
	STATUS factoryResetStatus = 0;
	if(eeprom.Write(&resetEeprom, SYSCONFIG_IDENTIFIER_SIZE, SYSCONFIG_BASE) == ERROR)	
	{
		LOGERR("Failed to Factory reset system configuration!!!", 0, 0, 0);
		factoryResetStatus = ERROR;
	}
	if(eeprom.Write(&resetEeprom, SYSCONFIG_IDENTIFIER_SIZE, RUN_TIME_CONFIGURE) == ERROR)
	{
		LOGERR("Failed to Factory reset run time configuration!!!", 0, 0, 0);
		factoryResetStatus = ERROR;
	}
	return factoryResetStatus;		
}


/**************************************************************************//**
* \brief   - Destructor
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
SystemConfiguration::~SystemConfiguration()
{
}

/**************************************************************************//**
* \brief   - Caches the system configuration structures to callers instance
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void SystemConfiguration::SysConfigCache()
{
	Eeprom epclass;
	CommonProperty *CP = CommonProperty::getInstance();
	
	epclass.Read ((char *) &m_SysConfig, sizeof(m_SysConfig), SYSCONFIG_ADDR);
	bIsCached = true;
}


/**************************************************************************//**
* \brief   - Returns cache reference of the system configuration structures
*			 to callers instance
*
* \param   - None
*
* \return  - intf_SysConfig_t&
*
******************************************************************************/
intf_SysConfig_t& SystemConfiguration::SysConfigGetCache()
{
	return m_SysConfig;
}

/**************************************************************************//**
* \brief   - returns true if system configuration structures have been cached
*			 on callers instance
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool SystemConfiguration::IsSysConfigCached()
{
	return bIsCached;
}

/**************************************************************************//**
* \brief   - Check the system type name  
* 
* \param   - char *systemTypeName
*
* \return  - None
*
******************************************************************************/
void SystemConfiguration::GetSystemTypeName(char *systemTypeName)
{
	char systemType;
	Eeprom epclass;
	epclass.Read(&systemType, sizeof(char), (SYSCONFIG_ADDR + SYSCONFIG_SIZE - SYSTEM_TYPE_OFFSET));
	
	if(systemType ==GSX_E_SYSTEM )
	{
		sprintf(systemTypeName, "%s", "GSX-E1");
	}
	else if(systemType==P1_PNEUMATIC_SYSTEM)
	{
		sprintf(systemTypeName, "%s", "P1 PNEUMATIC");
	}
	else if(systemType==P2_PNEUMATIC_SYSTEM)
	{
		sprintf(systemTypeName, "%s", "P2 PNEUMATIC");
	}
	else if(systemType==I_IW_SYSTEM)
	{
		sprintf(systemTypeName, "%s", "IS I(IW)");
	}
	else if(systemType==MERCURY_SYSTEM)
	{
		sprintf(systemTypeName, "%s", "MERCURY");
	}
	else if(systemType==SATURNP1_SYSTEM)
	{
		sprintf(systemTypeName, "%s", "SATURN P1");
	}
	else if(systemType==SATURNP2_SYSTEM)
	{
		sprintf(systemTypeName, "%s", "SATURN P2");
	}
	else if(systemType==MARS_SYSTEM)
	{
		sprintf(systemTypeName, "%s", "MARS");
	}
	else if(systemType==JUNO_SYSTEM)
	{
		sprintf(systemTypeName, "%s", "JUNO");
	}
	else if(systemType==VENUS_SYSTEM)
	{
		sprintf(systemTypeName, "%s", "VENUS");
	}
	else if(systemType==VENUSHH_SYSTEM)
	{
		sprintf(systemTypeName, "%s", "VENUS-HH");
	}
	else if(systemType==ASX_SYSTEM)
	{
		sprintf(systemTypeName, "%s", "ASX");
	}
}

