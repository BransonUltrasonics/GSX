/*
 * WeldRun.h
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#ifndef WELDRUN_H_
#define WELDRUN_H_
#include "Sonics.h"
#include "PowerUpRecipe.h"

#define AMPLOOPC1_20KDEFAULT     	100
#define AMPLOOPC1_30KDEFAULT     	205
#define AMPLOOPC1_40KDEFAULT     	205

#define AMPLOOPC2_20KDEFAULT     	300
#define AMPLOOPC2_30KDEFAULT     	370
#define AMPLOOPC2_40KDEFAULT     	370

#define PLOOP_20KDEFAULT         	450
#define PLOOP_30KDEFAULT         	100
#define PLOOP_40KDEFAULT          	60

#define PLOOPCF_20KDEFAULT 	   		450
#define PLOOPCF_30KDEFAULT 			200
#define PLOOPCF_40KDEFAULT 	   		200

#define MAX_AMPLITUDE_PERCENTAGE	100

class WeldRun : public Sonics
{
	friend class WeldResult;

	public:
		WeldRun();
		virtual ~WeldRun();
		static void WeldControlLoop();

		
	protected:
		
		void  Enter();
		void  Loop();
		void  Exit();
		
			
	private:
		static 	INT32 PhaseCount;
		static 	UINT32 TempHDMode;
		static 	UINT8 RampDownFlag;
		static	UINT64 WeldRampTimer;
		static	UINT32 WeldAmplitudeLimit;
		static 	UINT32 WeldAmplitudeLimitPrev;
		static 	UINT8 WeldRampDoneFlag;
		static UINT32 ddsFrequencyHighWeld;
		static UINT32 ddsFrequencyLowWeld;
		static INT32  WeldRamptime;

		static INT16 CheckWeldLimits(void);
		static void WeldRamp();
		static void CalculateRampStep();
		
#ifdef DEBUG_CURRENT_OVERLOAD
		static UINT64 FinalAmplitudedata[4000];
		static UINT32 WeldAmplitudeLimitdata[4000];
		static UINT32 i;
#endif //DEBUG_CURRENT_OVERLOAD
};

#endif /* WELDRUN_H_ */
