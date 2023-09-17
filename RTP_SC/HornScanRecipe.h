/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef HORNSCANRECIPE_H_
#define HORNSCANRECIPE_H_

#include  <sys/types.h>
#include  <iostream>

#define START_FREQUENCY_20KDEFAULT		19450
#define START_FREQUENCY_30KDEFAULT		29250
#define START_FREQUENCY_40KDEFAULT		38900

#define STOP_FREQUENCY_20KDEFAULT		20450
#define STOP_FREQUENCY_30KDEFAULT		30750
#define STOP_FREQUENCY_40KDEFAULT		40900

#define SCAN_TIME_DELAY_DEFAULT			10
#define SCAN_MAX_CURRENT_DEFAULT		10
#define SCAN_MAX_AMPLITUDE_DEFAULT		10
#define SCAN_FREQUENCY_STEP_DEFAULT		1
#define SCAN_DIGITALTUNE_DEFAULT		1

typedef struct ScanRecipe	
{
	UINT32 FrequencyStart;
	UINT32 FrequencyStop;
	UINT16 FrequencyStep;
	UINT16 TimeDelay;
	UINT16 MaxAmplitude;
	UINT16 MaxCurrent;
	UINT8  SetDigTuneWithHornScan;
}ScanRecipe;

class HornScanRecipe
{
public:
	HornScanRecipe();
	~HornScanRecipe();
	
	UINT8		GetDigitalTuneStatus();
	void		UpdateScanRecipe(ScanRecipe& recipe);
	ScanRecipe&	GetScanRecipe();
	
private:
	ScanRecipe	m_Recipe;
};

#endif /* HORNSCANRECIPE_H_ */
