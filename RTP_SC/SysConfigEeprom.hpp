/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef SYSTEMCONFIGURATION_H_
#define SYSTEMCONFIGURATION_H_

#include <iostream>
#include <vxworks.h>
#include <ioLib.h>
#include "Eeprom.h"

#define SYSCONFIGSIG 	  				0x55 
#define SYSCONFIG_IDENTIFIER_SIZE 		1

#define MACHINE_NAME_SIZE				32
#define SC_SERIAL_NUMBER_SIZE			12
#define PC_SERIAL_NUMBER_SIZE			12
#define AC_SERIAL_NUMBER_SIZE			12
#define AUXBOX_SERIAL_NUMBER_SIZE		12
#define EXT_STORAGE_SERIAL_NUMB_SIZE	15

#define FREQ_20KHz						20
#define FREQ_30KHz						30
#define FREQ_40KHz						40

#define POWER_20KHz						4000
#define POWER_30KHz						1500
#define POWER_40KHz						800

#define SYSCONFIG_SIZE sizeof(intf_SysConfig_t)
#define SYSTEM_TYPE_OFFSET  64

typedef struct ExternalStorage
{
	bool IsConfig;
	char serialNumber[EXT_STORAGE_SERIAL_NUMB_SIZE];
}ExternalStorage;

typedef enum
{
	KHz_20 = 0,
	KHz_30 = 1,
	KHz_40 = 2
}SysFreqType;

typedef enum
{
	SERVO = 0,
	PNEUMATIC,
	MAX_ActuatorType
}ActuatorType;

typedef enum
{
	GSX_E 		= 0,
	P1_PNEUMATIC,
	P2_PNEUMATIC,
	I_IW,
	MERCURY,
	SATURNP1,
	SATURNP2,
	MARS,
	JUNO,
	VENUS,
	VENUSHH,
	MAX_ControlLevel
}ControlLevel;

typedef enum
{
	PS_STANDARD = 0,
	MAX_PSType
}PSType;

typedef struct
{
	ActuatorType   	ActuatorTypeIs;
	ControlLevel    ControlLevelIs;
	UINT8			MaxStrokelength;
}ActuatorConfig;

typedef struct
{
	ControlLevel 	ControlLevelIs;
	INT32      		PSWatt;
	PSType   		PsTypeIs;
	UINT16   		PSFreq;
}PowerSupplyConfig;

typedef struct
{
	ControlLevel 	ControlLevelIs;
	UINT16   		PSFreq;
	INT32      		PSWatt;
	PSType   		PsTypeIs;
}OLD_PowerSupplyConfig;

typedef enum
{
	ENGLISH = 0,
	FRENCH,
	GERMAN,
	SPANISH,
	KOREAN,
	TRADITIONAL_CHINESE,
	SIMPLIFIED_CHINESE,
	ITALIAN,
	JAPANESE,
	SLOVAK,
	DANISH,
	MAX_Language
}Language;

typedef enum
{
	DASHBOARD = 0,
	PRODUCTION,
	RECIPES,
	ANALYTICS,
	MAX_StartScreen
}StartScreen;

typedef enum
{
	METRIC = 0,
	IMPERIAL,
	MAX_Units
}Units;

typedef enum
{
	PS_SEEK = 0,
	PS_SCAN,
	PS_NONE,
	MAX_PsPowerUpOptions
}PsPowerUpOptions;

typedef enum
{
	MEM_FULL_STOP = 0,
	MEM_FULL_CONTINUE
}MemFullAction;

typedef enum
{
	DISABLED = 0,
	FULL
}AuthorityCheck;

typedef enum
{
	GSX_E_SYSTEM 		= 0,
	P1_PNEUMATIC_SYSTEM,
	P2_PNEUMATIC_SYSTEM,
	I_IW_SYSTEM,
	MERCURY_SYSTEM,
	SATURNP1_SYSTEM,
	SATURNP2_SYSTEM,
	MARS_SYSTEM,
	JUNO_SYSTEM,
	VENUS_SYSTEM,
	VENUSHH_SYSTEM,
	ASX_SYSTEM,
	MAX_SystemType
}SystemType;

/* new structure */
typedef struct
{
	/* the first page at 0x180*/
	Language                LanguageIs;
	StartScreen             Sscreen;
	Units                   UnitIs;
	PsPowerUpOptions   		PsPowerUpOptionIs;
	PowerSupplyConfig   	PcConfig;
	bool                 	bIsExternalPresets;
	bool					bIsHornClamp;
	char 					machineName[MACHINE_NAME_SIZE];	
	/* The second page at 0x1c0 */
	MemFullAction           MemFullActionIs;
	AuthorityCheck          AuthCheckLevel;	
	bool          			bResetMem;
	bool          			bPowerUpMem;
	char					PresetRecipeBarcodeStart;	
	bool					bPartIDSwitchStatus;	
	SystemType         		systemType;
	char                	SC_SerialNumber[SC_SERIAL_NUMBER_SIZE];	
	char					AuxBoxSerialNumber[AUXBOX_SERIAL_NUMBER_SIZE];
	char					PC_SerialNumber[PC_SERIAL_NUMBER_SIZE];
	char					AC_SerialNumber[AC_SERIAL_NUMBER_SIZE];
	/* The third page 0x200 */
	ActuatorConfig          ActConfig;

}intf_SysConfig_t;

/* old structure */
typedef struct
{
	bool					bIsExtraCooling;
	bool                 	bIsExternalPresets;
	bool					bIsHornClamp;
	Language                LanguageIs;
	StartScreen             Sscreen;
	Units                   UnitIs;
	PsPowerUpOptions   		PsPowerUpOptionIs;
	MemFullAction           MemFullActionIs;
	AuthorityCheck          AuthCheckLevel;
	ActuatorConfig          ActConfig;
	OLD_PowerSupplyConfig   	PcConfig;
	char 					machineName[MACHINE_NAME_SIZE];
	bool          			bResetMem;
	bool          			bPowerUpMem;
	char					PresetRecipeBarcodeStart;	
	bool					bPartIDSwitchStatus;	
	SystemType         		systemType;
	char                	SC_SerialNumber[SC_SERIAL_NUMBER_SIZE];	
	ExternalStorage			extStorage;
	char					AuxBoxSerialNumber[AUXBOX_SERIAL_NUMBER_SIZE];
	char					PC_SerialNumber[PC_SERIAL_NUMBER_SIZE];
	char					AC_SerialNumber[AC_SERIAL_NUMBER_SIZE];
}OLD_intf_SysConfig_t;


class SystemConfiguration
{
public:	
	SystemConfiguration();
	~SystemConfiguration();
	
	bool 				ConfigureDefaultSettings	();
	void 				SysConfigGet				(char *pSysConfigData);
	void 				SysConfigSet				(char *pSysConfigData);
	void 				SysConfigDetails			(intf_SysConfig_t& sysConfig);
	void 				SysConfigCache 				();
	intf_SysConfig_t& 	SysConfigGetCache			();
	bool				IsSysConfigCached 			();
	
	STATUS FactoryResetEeprom();
	void GetSystemTypeName(char *systemTypeName);

	static char  systemTypeName[SYSINFO_SIZE];
private:
	bool ValidateEEPROM();
	bool SysConfigCheck();

	
private:
	bool				bIsCached;
	intf_SysConfig_t 	m_SysConfig; 
	OLD_intf_SysConfig_t 	OLD_m_SysConfig; 
};

#endif /* SYSTEMCONFIGURATION_H_ */
