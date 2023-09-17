/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef SCAN_H_
#define SCAN_H_

#include "Sonics.h"

#define PWMVALUE_PER_HUND_OF_PERCENT (6.5535)
#define AMPLITUDEFACTOR 	(100)



static UINT32 PDOFrequency;
UINT32 OperatingFreq;

class Scan : public Sonics
{
	
	public:
		Scan();
		virtual ~Scan();
		static UINT16 ConvertAmp2PWM(UINT16 Amplitude);
		void UpdateHornScanData();

	public:
		UINT32 FrequencyStart;
		UINT32 Count;
		bool StartSCan;
	protected:
		void Enter();
		void Loop() ;
		void Exit() ;
		
	private:
		static UINT8 ScanEnableFlag;
		UINT32 Timercounter;
		UINT32 SamplesCounter;
		UINT32 AmplitudeChange;
		UINT16 PWM;
};


#endif /* SCAN_H_ */
