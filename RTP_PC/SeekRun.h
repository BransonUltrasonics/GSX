/*
  * SeekRun.h
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#ifndef SEEKRUN_H_
#define SEEKRUN_H_

#include "Sonics.h"
#include "PowerUpRecipe.h"
//SeekAmplitude in % compared to WeldAmplitude

#define SEEKAMPP_DEFAULT		    10
#define SEEKAMPLOOPC1_DEFAULT	   	205
#define SEEKAMPLOOPC2_DEFAULT	   	370

#define SEEKFREQHIGH_DEFAULT    	5000
#define SEEKFREQLOW_DEFAULT     	5000

#define SEEKPHASELOOPCF_DEFAULT  	200
#define SEEKRAMPTIME_DEFAULT		80
#define SEEKTIME_DEFAULT		   	500

#define SEEKPHASELOOP_20KDEFAULT 	140
#define SEEKPHASELOOP_30KDEFAULT 	100
#define SEEKPHASELOOP_40KDEFAULT  	60

class SeekRun : public Sonics
{
	
	public:
		SeekRun();
		virtual ~SeekRun();
		static UINT32 GetSeekTimer();
		static void SeekControlLoop();
        
	protected:
		void Enter();
		void Loop() ;
		void Exit() ;

	private:
		static UINT8 SeekRampDone;
		static UINT32 SeekRunTimeCounter;
		static UINT32 ddsFrequencyHighSeek;
		static UINT32 ddsFrequencyLowSeek;
		static INT32 SeekStartFreqOffset;
		static UINT32 SeekTimer;
		static INT16 CheckSeekLimits(void);
		static void SeekRamp();
};

#endif /* SEEKRUN_H_ */
