/*
 * ScanSonicOn.h
 *
 *  Created on: Oct 29, 2017
 *      Author: lzhu
 */

#ifndef SRC_SCANSONICON_H_
#define SRC_SCANSONICON_H_

#include <stdio.h>
#include <iostream>
#include "SonicOn.h"
#include "WeldSonicOn.h"
#include "EcSDOTask.h"

using namespace std;

#define SIZE_SCANBUFFER 2100
#define NUMRESONANCES     6 



class ScanSonicOn : public SonicOn
{
	friend class SCStateMachine;
public:
	ScanSonicOn();
	~ScanSonicOn();
	UINT32 ScanTime;
	UINT32 ScanTimeCounter;
	UINT32 DDSFrequency;
	//UINT16 NumScanPoint;
	UINT32 ScanDelayTime;
	UINT32 SCanDelayTimeCounter;
	UINT32 HornScanGraphPointCounter;
	ScanRecipe scanrecipe;
	
protected:
	void Enter();
	void Loop();
	void Exit();
	
private:
	UINT16 NumScanPoints;
	INT16  HornScanRawPhase[SIZE_SCANBUFFER];
	UINT32 HornScanRawFrequency[SIZE_SCANBUFFER];	
	
	UINT16 LineofFitData(UINT32* Freq, INT16* Phase, UINT16 DataSize);
	UINT16 FindHighestImpedencePoint(UINT16 Index);
	UINT16 FindLowestImpedencePoint(UINT16 Index);
	ScanSignature ScanBuffer[SIZE_SCANBUFFER];	
	UINT16 HighestImpedencePoint(UINT16 Index);
	UINT16 LowestImpedencePoint(UINT16 Index);
	UINT32 ResonanceFreqs[NUMRESONANCES];
	UINT16 Amp;
	void DetectResonance();
	bool FindZeroCrossing(INT16 *Phase, UINT16 DataSize);
	
	void FindResonanceFrequency();
	void ApplyLowPassFilter(INT16 *Phase, UINT16 DataSize);
	RESONANCE_FREQ_TYPE ResonanceFreqType[NUMRESONANCES];
	
	
	UINT16 PercentInterval;
	UINT8 IntervalCounter;
	UINT8 Progress;
	MSG_Q_ID UI_Q_ID;
	UINT16 OpType;
	void ProgressBar();
	void UpdateProgressBar(UINT8 Status);
	
	void Display();
	INT32 Memoffset;
	void UpdateDigitalTuneAndStartFrequency();
	bool ScanFailed;
};




#endif /* SRC_SCANSONICON_H_ */
