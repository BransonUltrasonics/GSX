/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef WELDRESULTS_H_
#define WELDRESULTS_H_

#include "Common.h"
#include "WeldRecipe.h"

using namespace std;

const char DEFAULT_PART_ID[]= {'0',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

typedef enum  
{
	GOOD 		= 1,
	THROUGHPUT,
	SUSPECT,
	REJECTED
}WELD_STATUS;

class WeldResults
{
public:
	typedef enum  
	{
		MAX_WELD_FORCE,
		END_HOLD_FORCE,
		WELD_ABSOLUTE,
		TOTAL_ABSOLUTE,
		CYCLE_COUNTER,
		RECIPE_NUM,
		WELD_TIME,
		CYCLE_TIME,
		TOTAL_ENERGY,
		PEAK_POWER,
		START_FREQ,
		FREQ_CHANGE,
		WELD_COLLAPSE_DIST,
		HOLD_COLLAPSE_DIST,
		TOTAL_COLLAPSE_DIST,
		TRIGGER_TIME,		/* T-W-H points */
		WELD_SONIC_TIME,
		HOLD_TIME,
		VELOCITY,
		TRIGGER_DISTANCE,
		IS_ALARM,
		RECIPE_REV_NUM,
		WELD_MODE,
		STACK_SERIAL_NUM,
		WELD_STATUS,
		WELD_RECIPE_STATUS
	} WELD_RESULT_IDX;

	WeldResults ();
	~WeldResults();

	UINT32 	Get(WELD_RESULT_IDX idx);
	void 	Set(WELD_RESULT_IDX idx, UINT32 data);	
	void 	SetUserName(char* name);
	string  GetUserName();
	void 	GetWeldStatus(UINT8 weldStatus, string& weldStatusName,UINT8& status);
	void 	SetWeldMode(INT32 weldMode);
	void	ResetWeldResult();
	bool  	GetPartIDScanStatus();
	void  	SetPartIDScanStatus(bool PartIDScanStatus);
	void  	GetPartID(char *bcPartId);
	void  	SetPartID(const char *bcPartId);
	WeldResult	GetWeldData();
	
private:
	WeldResult 	results;
	bool 		m_PartIDScanStatus;	
};

#endif /* WELDRESULTS_H_ */
