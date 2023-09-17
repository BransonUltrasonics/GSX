/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef ACCONTROLTASK_H_
#define ACCONTROLTASK_H_

/* Header files */
#include "AcCommon.h"
#include "ACOD.h"


/* Macros */
#define SET 1
#define RESET 0
#define NUM_OF_SC_STATES 9

#define CALIBRATIONTIME_BASE 			0x8F
#define CALIBRATIONTIME_ADDR 			0x90
#define CALIBRATIONTIME_SIG 	  		0x46
#define CALIBRATIONTIME_IDENTIFIER_SIZE 1


/* AC Control Task class for Reading/Writing RxPdo/TxPdo and Weld Recipe/Weld Result data respectively */
class AcControlTask 
{   	
	// To let control task access AcRealTime Shared data between kernel and RTP
	friend void controlTask();		
	public:
		AcControlTask();		
   		~AcControlTask();	
   		
   		//Read Real Time data from Master to Slave Local OD
   		INT16 ReadRtFromOD();
   		
   		//Write Real Time data from Slave Local OD to Master
   		INT16 WriteRtToOD();		

   		// To Read Weld Recipe data from Master to Slave Local OD
   		static INT16 ReadWeldRecipeFromOD();
   		
   		// To Read calibration data from Master to Slave Local OD
   		INT16 ReadCalibrationReadingsFromOD();
   		// To write calibration data from slave to Master
   		INT16 WriteCalibrationTimeStampFromEepromToOD();
   		// To Read calibration timestamp from master to slave eeprom
   		INT16 WriteCalibrationTimeStampToEeprom(CalibrationReadings calReadings);
		// Write firmware info data from Slave Local OD to Master   	
   		INT16 WriteFirmwareInfoToOD();		
		   		
   		// Function to Process respective tasks based on events received 
   		void ProcessEvent();
   		    		
	private:
   		AcCtrltaskRt *pRtData;		// Pointer to Shared data
   		AcCtrltaskRt AcRealTime;	//Shared data
   		bool masterChanged;  		//To print TxPdo/RxPdo data only when state change  		
};


   	   
#endif /* ACCONTROLTASK_H_ */
