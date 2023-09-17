/*
+ * Powerup.h
 *
 *  Created on: Oct 9, 2018
 *      Author: DShilonie
 *      
 *     	This class defines the POWER_UP state as it handles all things power-up related.
 *      i.e. password override check, etc.
 */

#ifndef POWERUP_H_
#define POWERUP_H_

#include "SCState.h"
#include "DataBaseManager.h"

using namespace std;

//for RTC
#define MIN_YEAR                100
#define MAX_YEAR                199
#define DAYS_IN_YEAR             365
#define DAY_IN_WEEK              7
#define JANUARY                  1
#define FEBRUARY                 2
#define DECEMBER                 12
#define IS_LEAP_YEAR(year)      (((year % 4 == 0) && (year % 100 != 0)) \
                                || year % 400 == 0)



class Powerup :public SCState
{
	friend class SCStateMachine;
public:
	Powerup();
	~Powerup();
protected:
	
	void Enter	();
	void Loop	();
	void Exit	();
	
private:

	// password override
	void 	ProcessOverride ();
	
	bool 	_override;
	UINT16	_tCount;
	UINT16	_cCount;
	UINT16	_sCount;
	UINT32	_hcTime;
	
	// add here...
};



#endif /* POWERUP_H_ */
