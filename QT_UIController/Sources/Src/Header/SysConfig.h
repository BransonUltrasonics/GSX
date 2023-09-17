/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     System Configuration

***************************************************************************/

#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_

#include <stdio.h>

#include <QGuiApplication>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>
#define SINT8  	char

#define UINT16 quint16
#define BOOLEAN bool

#define EXT_STORAGE_SERIAL_NUMB_SIZE 15
#define MACHINE_NAME_SIZE 32
#define SC_SERIAL_NUMBER_SIZE 12
#define AUXBOX_SERIAL_NUMBER_SIZE 12
#define PC_SERIAL_NUMBER_SIZE 12
#define AC_SERIAL_NUMBER_SIZE 12

/* *************** Macros for Configuration values ****************** */

#define SERVO		0
#define PNEUMATIC	1
#define NEPTUNE     1
#define DHD 		1
#define CHD			0
#define INTERNAL 	0
#define EXTERNAL	1
#define ENABLED		1
#define DISABLED	0
#define DASHBOARD	1
#define ENGLISH		0
#define METRIC		1
#define CONTINUE	1
#define EXIT		0
#define SCAN        0
#define SEEK        1
/* **************** Global Buffer ************** */

struct ServiceParams
{
    bool boardReplaced;
    bool systemCleaned;
    QString lifeTimeWelds;
    QString lifeTimeAlarms;

};

struct BransonKey
{
    QString FirstLevelKey;
    QString SecondLevelKey;
};

enum ActuatorType
{/* */
    ACTUATOR_TYPE_SERVO = 0,
    ACTUATOR_TYPE_PNEUMATIC
};

enum ControlLevel
{/* */
    CONTROL_LEVEL_GSX_S = 0,
    CONTROL_LEVEL_P1_PNEUMATIC,
    CONTROL_LEVEL_P2_PNEUMATIC,
    CONTROL_LEVEL_I_IW,
    CONTROL_LEVEL_MERCURY,
    CONTROL_LEVEL_SATURNP1,
    CONTROL_LEVEL_SATURNP2,
    CONTROL_LEVEL_MARS,
    CONTROL_LEVEL_JUNO,
    CONTROL_LEVEL_VENUS,
    CONTROL_LEVEL_VENUSHH
};

enum PSType
{/* */
    PS_TYPE_CHD = 0,
    PS_TYPE_DHD
};

struct ActuatorConfig
{
    ActuatorType   ActuatorTypeIs;
    ControlLevel    ControlLevelIs;
    UINT8           MaxStrokelength;
};

enum SystemType
{
    /*Enums list update for System Type decision at SC side*/
    SYSTEM_TYPE_GSX_E1 = 0,
    SYSTEM_TYPE_P1_PNEUMATIC,
    SYSTEM_TYPE_P2_PNEUMATIC,
    SYSTEM_TYPE_I_IW,
    SYSTEM_TYPE_MERCURY,
    SYSTEM_TYPE_SATURNP1,
    SYSTEM_TYPE_SATURNP2,
    SYSTEM_TYPE_MARS,
    SYSTEM_TYPE_JUNO,
    SYSTEM_TYPE_VENUS,
    SYSTEM_TYPE_VENUSHH,
    SYSTEM_TYPE_ASX
};

struct PowerSupplyConfig
{
    ControlLevel ControlLevelIs;
    qint32   PSWatt;
    PSType   PsTypeIs;
    UINT16   PSFreq;
};

struct ExternalStorage
{
       bool IsConfig;
       char serialNumber[EXT_STORAGE_SERIAL_NUMB_SIZE];
};

enum App_Language
{
    LANGUAGE_ENGLISH = 0,
    LANGUAGE_FRENCH,
    LANGUAGE_GERMAN,
    LANGUAGE_SPANISH,
    LANGUAGE_KOREAN,
//    LANGUAGE_TRADITIONAL_CHINESE,
    LANGUAGE_SIMPLIFIED_CHINESE,
    LANGUAGE_ITALIAN,
    LANGUAGE_JAPANESE,
    LANGUAGE_SLOVAK,
    LANGUAGE_DANISH
};

enum StartScreen
{
    START_SCREEN_DASHBOARD = 0,
    START_SCREEN_PRODUCTION,
    START_SCREEN_RECIPES,
    START_SCREEN_ANALYTICS
};

enum Units
{
    UNIT_METRIC = 0,
    UNIT_IMPERIAL
};

enum PsPowerUpOptions
{
    PS_SEEK = 0,
    PS_SCAN,
    PS_NONE
};

enum MemFullAction
{
    MEM_FULL_STOP = 0,
    MEM_FULL_CONTINUE
};

enum AuthorityCheck
{
    AUTH_CHECK_DISABLED = 0,
//    AUTH_CHECK_MINIMAL,
    AUTH_CHECK_FULL
};


struct intf_SysConfig_t
{
    /* the first page at 0x180*/
    App_Language LanguageIs;
    StartScreen Sscreen;
    Units UnitIs;
    PsPowerUpOptions PsPowerUpOptionIs;
    PowerSupplyConfig PcConfig;
    bool bIsExternalPresets;
    bool bIsHornClamp;
    char chMachineName[MACHINE_NAME_SIZE];
    /* The second page at 0x1c0 */
    MemFullAction MemFullActionIs;
    AuthorityCheck AuthCheckLevel;
    bool bResetMem;
    bool bPowerUpMem;
    char PresetRecipeBarcodeStart;
    bool bPartIDSwitchStatus;
    SystemType systemType;
    char SC_SerialNumber[SC_SERIAL_NUMBER_SIZE];
    char AuxBoxSerialNumber[AUXBOX_SERIAL_NUMBER_SIZE];
    char PC_SerialNumber[PC_SERIAL_NUMBER_SIZE];
    char AC_SerialNumber[AC_SERIAL_NUMBER_SIZE];
    /* The third page 0x200 */
    ActuatorConfig ActConfig;
};

#endif /* SYSCONFIG_H_ */
