/*
 * Sonics.h
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#ifndef SONICS_H_
#define SONICS_H_

#include "PCState.h"
#include <sys/types.h>
#include "TimerInterval.h"

class Sonics : public PCState
{

	friend class PCStateMachine;
	public:
		Sonics();
		virtual ~Sonics();
		static void SetDefaultFreqAndFFactor();
		static UINT32 GetF_Factor();
		static UINT8  Get_SonicsConfigDoneFlag();
		
	protected:
		void virtual Enter() = 0;
		void virtual Loop() = 0;
		void virtual Exit() = 0;
		static void Control(void);
		static void AmplitudeControl(void);
		static void FrequencyControl(void);
		static void initIntergalfactor();
		 
		
		static UINT64 FinalAmplitude;
		static UINT16 WeldError;
		static UINT32 Midband;
		static INT32 LastAmpDifference;
		static INT32 PWMLastValue;
		static INT32 CurrentPowerFactor; 
		static INT32 PhaseI;
		static INT32 AmplitudeGain;
		static INT32 AmplitudeIntegral;	
		static UINT8 SonicsConfigDoneFlag ;	
		static UINT32 PCSampleTimeStep;
		static UINT32 BlindTime;
		static UINT16 BlindTimeCounter;
		static UINT8 PhaseShift;
		
	private:
		static UINT32 F_Factor;
		static INT32 IntegralFactor;
		static INT32 PhaseValueI;
		static INT32 IntegralValue;
};		

#endif /* SONICS_H_ */
