/*
 * TimerInterval.h
 *
 *  Created on: Jan 9, 2018
 *      Author: eguajardo
 */

#include "Common.h"
#include"PCMain.h"
#include <fstream>

#ifndef TIMERINTERVAL_H_
#define TIMERINTERVAL_H_

#define TIMER_INTERVAL_RECIPE_PATH_FILE  "/sd0:1/TimerIntervalRecipefromsc.txt"		// TIMER INTERVAL file Path
#define TIMER_INTERVAL_RECIPE_PATH  "/romfs/TimerIntervalRecipefromsc.txt"		// TIMER INTERVAL file Path in romfs

#define STRUCT_PACKED __attribute__((packed))
 
#define MAX_TIMEINTERVAL_US		1000
#define DEFAULT_TIMEINTERVAL_US 1000
#define MIN_TIMEINTERVAL_US		50

typedef struct 
{
	UINT16 NumofEntries;
	INT32  TmrIntervalPC ;
}STRUCT_PACKED
 TimerIntervalDataParam;

class TimerInterval
{
	public:
		TimerInterval();
		static void ReadTimerIntervalfromOD(void);
		static UINT32 GetTimerInterval(void);
		static void ReadTimerIntervalfromtxt(void);
	private:
		static TimerIntervalDataParam TmrInterval;
};


#endif /* TIMERINTERVAL_H_ */
