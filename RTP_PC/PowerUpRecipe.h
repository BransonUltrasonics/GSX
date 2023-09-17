/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef POWERUP_RECIPE_H_
#define POWERUP_RECIPE_H_

#include <iostream>
#include <fstream>
#include<sys/types.h>
#include"PCMain.h"
#include"Common.h"
using namespace std;

#define STRUCT_PACKED __attribute__((packed))
#define POWERUP_RECIPE_PATH		"/sd0:1/PowerupRecipefromsc.txt"		// POWER RECIPE file Path

#ifndef INDEX_POWERUPDATA 
#define INDEX_POWERUPDATA 0x2001
#endif

#define SYSTEM_FREQUENCY_EEPROM_OFFSET   0x0000100
#define SYSTEM_POWER_EEPROM_OFFSET       0x0000104

#define SYSTEM_FREQUENCY_SIZE            4
#define SYSTEM_POWER_SIZE                4

typedef struct 
{
	UINT16 NumofEntries;
	INT32  Frequency ;
	INT32  Power ;
	INT32  SYSTEMTYPE;
	INT32  CheckSum ;
	INT8   SerialNo ;
}STRUCT_PACKED
 PowerUpDataParam;

class PowerUpData
{
	private:
		static PowerUpDataParam PCPowerUpData;		
		static bool PowerUpDataStored;
	public:
		static void ReadPowerUpDataFromOD();
		static void ReadPowerUpDataFromEEPROM();
		static INT32 GetPSFrequency() { return PCPowerUpData.Frequency; }
};

#endif /* POWERUP_RECIPE_H_ */

