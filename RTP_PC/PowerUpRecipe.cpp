/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     This file contains Power up data functionality.
 
**********************************************************************************************************/

#include "PowerUpRecipe.h"
#include "DynamicSystemCall.h"
#include "FPGA.h"
#include "Eeprom.h"
#include"PCTimer.h"

//Initialize static variables
PowerUpDataParam PowerUpData::PCPowerUpData;
bool PowerUpData::PowerUpDataStored = false;





/******************************************************************************
*      Function    :   ReadPowerUpDataFromEEPROM
*      Description :   This function will get PowerUpData from EEPROM
*      Input       :   void
*      Output      :   void
*      Called by   :   PCMain
*******************************************************************************/
void PowerUpData::ReadPowerUpDataFromEEPROM()
{
	Eeprom epclass;
	INT32  Frequency ;
	INT32  Power ;
	//Read EEPROM, put 2ms
	epclass.Read((char*)&Frequency, SYSTEM_FREQUENCY_SIZE, SYSTEM_FREQUENCY_EEPROM_OFFSET);
	MsDelay(3);
	epclass.Read((char*)&Power, SYSTEM_POWER_SIZE,SYSTEM_POWER_EEPROM_OFFSET);
	MsDelay(3);
	if((Frequency > 19 && Frequency < 41) && ((Power == P20KHZ4000W) || (Power == P20KHZ2500W) || (Power == P20KHZ1250W) || (Power == P30KHZ1500W) || (Power == P40KHZ800W)))
	{
		PCPowerUpData.Frequency = Frequency;
		PCPowerUpData.Power = Power;	
	}
	else //eeprom values invalid - use default 20k
	{
		PCPowerUpData.Frequency = F20KHZ;
		PCPowerUpData.Power = P20KHZ4000W;
		
	}
	//LOG("PC EEPROM Read Frequency   = %d\n", Frequency);
	//LOG("PC EEPROM Read Power   = %d\n", Power);
	FPGA::UpdateFPGAInitParameters(PCPowerUpData);
	
}


/******************************************************************************
*      Function    :   ReadPowerUpDataFromOD
*      Description :   This function will display PowerUpData Object Dictionary
*      Input       :   void
*      Output      :   void
*      Called by   :   TxPDO::sendTxPDOToOD
*******************************************************************************/
void PowerUpData::ReadPowerUpDataFromOD()
{	
	OBJCONST TOBJECT OBJMEM * pObjEntry;
	UINT32 PowerUpDataODLength=0;
	UINT8 Status;
	UINT8 SubIndex=0;
	Eeprom epclass;
	INT32  Frequency = PCPowerUpData.Frequency;
	INT32  Power = PCPowerUpData.Power;
#ifdef POWERUP_FILE_READ
	fstream fp;
	if(!PowerUpDataStored)
	{
		fp.open(POWERUP_RECIPE_PATH, ios::out);
		if(fp.is_open())
		{
			memset(&PCPowerUpData,0,sizeof(PowerUpDataParam));
			pObjEntry = obj_getobjecthandleSc( INDEX_POWERUPDATA );
			if(pObjEntry == NULL)
			{
				LOG("   Failed to get the pointer to  PowerUpData OBJ\n");
				exit(-1);
			}
			PowerUpDataODLength=obj_getobjectlengthSc( INDEX_POWERUPDATA, SubIndex, pObjEntry, COMPLETE_ACCESS);
			Status= obj_readSc( INDEX_POWERUPDATA, SubIndex, PowerUpDataODLength, pObjEntry, (UINT16 MBXMEM *)Buffer, COMPLETE_ACCESS );
			
			UINT8* pBuffer = (UINT8 *)Buffer;
			if(Status==SUCCESS)
			{	
			
				fp <<"Frequency:\t"<< PCPowerUpData.Frequency<<endl;
				fp <<"Power:\t"<< PCPowerUpData.Power<<endl;
				fp <<"SYSTEMTYPE:\t" << PCPowerUpData.SYSTEMTYPE<<endl;
				fp <<"CheckSum:\t" << PCPowerUpData.CheckSum<<endl;
				fp <<"SerialNo:\t" << PCPowerUpData.SerialNo<<endl;
				fp <<"@"<<endl;			//End character

				
#ifdef PRINT
					LOG("--------------------Power Up Data --------------------------------\n");
				
					LOG("   Frequency        	   = %d\n",PCPowerUpData.Frequency);
					LOG("   Power            	   = %d\n",PCPowerUpData.Power);
					LOG("   SYSTEMTYPE       	   = %d,\n",PCPowerUpData.SYSTEMTYPE);
					LOG("   CheckSum         	   = %d,\n",PCPowerUpData.CheckSum);
					LOG("   SerialNo        	   = %d,\n",PCPowerUpData.SerialNo);
					
					LOG("-----------------------------------------------------------------\n");
#endif		
					fp.close();
					FPGA::UpdateFPGAInitParameters();
					PowerUpDataStored = true;
			}
		}
		else
		{
#ifdef PRINT
				LOG("Error reading PowerUpData from Object Dictionary\n");
#endif
			fp.close();
		}
	}
#endif

	memset(&PCPowerUpData,0x00,sizeof(PowerUpDataParam));
	pObjEntry = obj_getobjecthandleSc( INDEX_POWERUPDATA );
	if(pObjEntry == NULL)
	{
		LOG("   Failed to get the pointer to  PowerUpData OBJ\n");
		exit(-1);
	}
	
	PowerUpDataODLength = obj_getobjectlengthSc( INDEX_POWERUPDATA, SubIndex, pObjEntry, COMPLETE_ACCESS);
	Status= obj_readSc( INDEX_POWERUPDATA, SubIndex, PowerUpDataODLength, pObjEntry, (UINT16 MBXMEM *)Buffer, COMPLETE_ACCESS );

	UINT8* pBuffer = (UINT8 *)Buffer;
	if(SUCCESS == Status)
	{	
		memcpy(&PCPowerUpData,pBuffer,sizeof(PowerUpDataParam));
		if(Power != PCPowerUpData.Power	|| Frequency != PCPowerUpData.Frequency)
		{
			Frequency = PCPowerUpData.Frequency;
			Power = PCPowerUpData.Power;
			epclass.Write((char *)&Frequency, SYSTEM_FREQUENCY_SIZE, SYSTEM_FREQUENCY_EEPROM_OFFSET);
			MsDelay(5);
			epclass.Write((char *)&Power, SYSTEM_POWER_SIZE, SYSTEM_POWER_EEPROM_OFFSET);
			MsDelay(5);
			//LOG("PC EEPROM Write Frequency   = %d\n", Frequency);
			//LOG("PC EEPROM Write Power   = %d\n", Power);
			FPGA::UpdateFPGAInitParameters(PCPowerUpData);
		}

#ifdef PRINT
		LOG("--------------------Power Up Data --------------------------------\n");

		LOG("   Frequency        	   = %d\n",PCPowerUpData.Frequency);
		LOG("   Power            	   = %d\n",PCPowerUpData.Power);
		LOG("   SYSTEMTYPE       	   = %d,\n",PCPowerUpData.SYSTEMTYPE);
		LOG("   CheckSum         	   = %d,\n",PCPowerUpData.CheckSum);
		LOG("   SerialNo        	   = %d,\n",PCPowerUpData.SerialNo);

		LOG("-----------------------------------------------------------------\n");				
#endif	
		//FPGA::UpdateFPGAInitParameters();
	}
	else
	{
		#ifdef PRINT
			LOG("Error reading PowerUpData from Object Dictionary\n");
		#endif
	}

	memset(Buffer,'\0',GLOBAL_BUFFER_SIZE);		
}




