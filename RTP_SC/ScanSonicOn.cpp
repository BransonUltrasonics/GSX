/*
 * ScanSonicOn.cpp
 *
 *  Created on: Oct 29, 2017
 *      Author: lzhu
 */

#include "Common.h"
#include "ScanSonicOn.h"
#include "ControlTask.h"
#include "MainTask.h"
#include "WeldRecipe.h"
#include "AlarmManager.h"
#include "DataBaseManager.h"
#include<cmath>

#define NUMRESONANCES 		6
#define HS_NUMCOEFS 		17
//#define HS_SCALEFACT 32768
#define HS_SCALEFACT        15 // use as divide by 32768 using bit shifting
#define HS_CLUSTERPTS       10
#define HS_HALFWINDOWSIZE   2
#define HS_FAILLIMIT        3
#define SCAN_POINT_WINDOW   10

#define PERCENT_INTERVAL 	10

//#define MIN_NUM_OF_RES_FREQ_FOUND

const INT16 HS_FiltCoefs[] = {  -157,-361,-465,-152,     /* Filter coff used to smooth graph*/
                                 850,2548,4552,6190,
                                 6824,
                                 6190,4552,2548,850,
                                 -152,-465,-361,-157  };


/**************************************************************************//**
* 
* \brief  - ScanSonicOn Class constructor
*
* \param  -  None
*
* \return  - None
*
******************************************************************************/
ScanSonicOn::ScanSonicOn()
{
	stateType = SCAN_SONIC_ON;
}

/**************************************************************************//**
* 
* \brief  - ScanSonicOn Class destructor
*
* \param  -  None
*
* \return  - None
*
******************************************************************************/
ScanSonicOn::~ScanSonicOn()
{
}

/**************************************************************************//**  
* 
* \brief  -ScanSonicOn entry code
*
* \param  - none
*
* \return  - none
*
******************************************************************************/
void ScanSonicOn::Enter()
{
	ScanTimeCounter = 0;
	scanrecipe = CP->m_ScanRecipeObj.GetScanRecipe();
	
	DDSFrequency = scanrecipe.FrequencyStart;
	NumScanPoints = scanrecipe.FrequencyStop - scanrecipe.FrequencyStart;
	SCanDelayTimeCounter = (scanrecipe.TimeDelay*(1000 / ECAT_CYCLE_TIME_IN_US));
	
	if(NULL != CP->Signature)
	{
		delete [] (CP->Signature);
		CP->Signature = NULL;
	}
	
	CP->Signature = new ScanSignature[NumScanPoints+1];  /*SIZE_SCANBUFFER*/

	Amp = 1;
	HornScanGraphPointCounter = 0;
	SonicsOnCounter = 0;
    ScanFailed = false;
	
	//Initialize RxPDO to avoid Overload issues for consecutive Horn Scans
	PC_RX->Amplitudechange = 1;
	PC_RX->Frequency 		 = DDSFrequency;
	
	PercentInterval = floor(float(((NumScanPoints + 1)/scanrecipe.FrequencyStep) * PERCENT_INTERVAL) / 100);
	if(0 == PercentInterval)
	{
		PercentInterval = 1;
	}		
	IntervalCounter	= 1;
	Progress = 0;
	UI_Q_ID = CP->GetMsgQId(cTaskName[UI_T]);
	
#ifdef DEBUG
	printf("ScanSonicOn::Enter() : SCanDelayTimeCounter:%d\n", SCanDelayTimeCounter);
#endif
	
	Memoffset=0;
}

void ScanSonicOn::Loop()
{
	if(AC_TX->ACState == AC_ESTOP)
	{
		AlarmMgr::EnterAlarmEvent (ALARM_AC, ALARM_ESTOP_NCA);
		ChangeState(SC_ALARM);
	}
	else if(PC_TX->PCState == PC_ALARM)
	{
		AlarmMgr::EnterAlarmEvent (ALARM_PC);
		ChangeState(SC_ALARM);
	}

#ifndef PRINT
	printf("MasterState-PC	:	%u\n", PC_RX->MasterState);
	printf("PC-state	:	%u\n", PC_TX->PCState);
#endif

	OpType=ControlTask::GetOperationType();

	if(PC_TX->PCState == SCAN_RUN)
	{    
		SonicsOnCounter = SonicsOnCounter + 1;
		if(((DDSFrequency >= scanrecipe.FrequencyStart) && (DDSFrequency <= scanrecipe.FrequencyStop)) &&(OpType!=ABORT))	
		{
			if(SonicsOnCounter % SCanDelayTimeCounter == 0)
			{
				if((PC_TX->Current > scanrecipe.MaxCurrent) || (PC_TX->Amplitude > scanrecipe.MaxAmplitude))
				{
					//Decrease FPGA output amplitude
					if(Amp > 1)
					{
						Amp -= 1;
					}
					else
					{
						//cout << "Skipping Amp-=1" << endl;						
					}
				}
				//Check if feedback Current and amplitude is less than half of Preset values
				else if((PC_TX->Current< (scanrecipe.MaxCurrent >> 1)))// && (ActualAmplitudeSM < (MaxAmplitude >> 1)))
				{
					//Increase FPGA output amplitude
					Amp += 1;
				}
				
#ifdef DEBUG
				if(Amp > 10)
					cout << "Amp > 10% : " << Amp << endl ;
#endif //DEBUG				
				
				CP->Signature[HornScanGraphPointCounter].Frequency 	= (PC_TX->Frequency/10);
				CP->Signature[HornScanGraphPointCounter].Amplitude 	= PC_TX->Amplitude;
				CP->Signature[HornScanGraphPointCounter].Current   	= (PC_TX->Current/100);
				CP->Signature[HornScanGraphPointCounter].Phase 	   	= PC_TX->Phase/10;
			
				ScanBuffer[HornScanGraphPointCounter].Current 	= PC_TX->Current;
				ScanBuffer[HornScanGraphPointCounter].Phase 	= PC_TX->Phase;		 
				ScanBuffer[HornScanGraphPointCounter].Amplitude = PC_TX->Amplitude;
				ScanBuffer[HornScanGraphPointCounter].Frequency = DDSFrequency;
				HornScanRawPhase[HornScanGraphPointCounter]		= PC_TX->Phase;  
			    HornScanRawFrequency[HornScanGraphPointCounter]	= DDSFrequency;
				DDSFrequency = DDSFrequency + scanrecipe.FrequencyStep;				
				HornScanGraphPointCounter = HornScanGraphPointCounter + 1;
				
				PC_RX->Amplitudechange = Amp;
				PC_RX->Frequency 	   = DDSFrequency;
				
				ProgressBar();				/* Tracks the scan process completeness */
				
#ifdef DEBUG
			    cout << "C:" << CP->Signature[HornScanGraphPointCounter].Current  << "A:" << PC_TX->Amplitude << "Ph:" << PC_TX->Phase;
			    cout << "Fr:" <<  DDSFrequency << endl;
#endif //DEBUG			    
			}
		}
		else if((OpType==ABORT) || (DDSFrequency > scanrecipe.FrequencyStop))
		{
			LOGDBG("DDSFrequency : %d",DDSFrequency,0,0);
			ChangeState(PREREADY);
		}
	}
}

/**************************************************************************//**  
* 
* \brief  -ScanSonicOn Exit code
*
* \param  - none
*
* \return  - none
*
******************************************************************************/
void ScanSonicOn::Exit()
{
	if (OpType!=ABORT)
	{
	  if(HornScanGraphPointCounter > 0)
	  {
		CP->NoOfSamples = HornScanGraphPointCounter;
		/* To store resonant frequency into DB */
		/*CP->ScanResult.SetResonantFrequency(HornScanResults::scanResultIdx = SECOND_PARALLEL_FREQ, CP->Signature[HornScanGraphPointCounter-1].Frequency);
		CP->ScanResult.SetResonantFrequency(HornScanResults::scanResultIdx = MAIN_PARALLEL_FREQ, CP->Signature[HornScanGraphPointCounter-1].Frequency);
		CP->ScanResult.SetResonantFrequency(HornScanResults::scanResultIdx = TERT_PARALLEL_FREQ, CP->Signature[HornScanGraphPointCounter-1].Frequency);
		CP->ScanResult.SetResonantFrequency(HornScanResults::scanResultIdx = FIRST_SERIES_FREQ, CP->Signature[HornScanGraphPointCounter-1].Frequency);
		CP->ScanResult.SetResonantFrequency(HornScanResults::scanResultIdx = SECOND_SERIES_FREQ, CP->Signature[HornScanGraphPointCounter-1].Frequency);
		CP->ScanResult.SetResonantFrequency(HornScanResults::scanResultIdx = TERT_SERIES_FREQ, CP->Signature[HornScanGraphPointCounter-1].Frequency);*/
	}	

	  DetectResonance();
	  UpdateDigitalTuneAndStartFrequency();
	}
	else if ((OpType==ABORT))
	{
		HornScanGraphPointCounter=0;
		delete [] CP->Signature;  
		CP->Signature = NULL;  
		CP->NoOfSamples = 0;    
	}
	
	ControlTask::SetResultData(true);	/* To collect scan results */
	
#ifdef DEBUG
	printf("\n HornScanGraphPointCounter: %d NumScanPoints:%d  RsnntFrq:%d \n",
			HornScanGraphPointCounter, NumScanPoints, CP->Signature[HornScanGraphPointCounter-1].Frequency);
	printf("ScanSonicOn::Exit : IntervalCounter : %u, IntervalPercent : %u, Progress : %u\n",IntervalCounter,PercentInterval,Progress);
#endif //DEBUG
	//UpdateDigitalTuneAndStartFrequency();
}

void ScanSonicOn::DetectResonance()
{
	bool Status = false;
    UINT8 Indx;
	bool EventFlag = false;
	
	   // Initially, check to see if any zero crossings exist
	
	Status = FindZeroCrossing(HornScanRawPhase, (NumScanPoints + HS_NUMCOEFS));
	
	if (Status == true )
	{
	      // Apply a low pass filter to smooth some of the high frequency noise in the raw phase data.
	   printf("find the zero crossing\n");
	   ApplyLowPassFilter(HornScanRawPhase, (NumScanPoints + HS_NUMCOEFS));
	   //Find the resonance points i.e, points around the zero crossing
	   FindResonanceFrequency();
	 }
	 else
	 {
	     ScanFailed = true;	
	     cout << "HornScanFailed - NoZeroCrossing" << endl ;
	 }
	 
}


/**************************************************************************//**
* 
* \brief  -This method applies a low pass filter to smooth some of the high frequency
*          noise in the raw phase data.
*
* \param  -  INT16 *Phase     buffer contains the all phase value
*            UINT16 DataSize  size of the buffer
*
* \return  - none
*
******************************************************************************/
void ScanSonicOn::ApplyLowPassFilter(INT16 *Phase, UINT16 DataSize)
{
	UINT16 Index1,  // Points to location to store filtered value
		Index2;  // Selects co-efficient and filter input.
	INT32 Filtsum; // Holder for filter calculation.

	for (Index1 = 0; Index1 < (DataSize - HS_NUMCOEFS); ++Index1)
	{
		Filtsum = 0;

		for (Index2 = 0; Index2 < HS_NUMCOEFS; Index2++)
			Filtsum = Filtsum + (HS_FiltCoefs[Index2] * Phase[Index1 + Index2]);
		Phase[Index1] = (Filtsum >> HS_SCALEFACT);
	}

}


/**************************************************************************//**
* 
* \brief  -This method checks to see if any zero crossings exist throughout the scan.
*
* \param  - INT *Phase    buffer contains the phase value
*           DataSize      size of the buffer
*
* \return  - return true if the zero crossing exists
*
******************************************************************************/
bool ScanSonicOn::FindZeroCrossing(INT16 *Phase, UINT16 DataSize)
{
	UINT16 i;
	bool status = false;
	INT16 maxval = -1;
	INT16 minval = 1;

	for (i = 0; i< DataSize; ++i)
	{
		if (Phase[i] > maxval)
		{
			maxval = Phase[i];
		}

		if (Phase[i] < minval)
		{
			minval = Phase[i];
		}
	}

	if ((maxval > 0) && (minval < 0))
	{
		status = true;    // Crossings exist.
	}

	return status;
}

/**************************************************************************//**
* 
* \brief  - This method does linear regression to fit the line.
*
* \param  - UINT32* Freq  buffer contains the frequency value
*           INT *Phase    buffer contains the phase value
*           DataSize      size of the buffer
*
* \return  - Return value of the frequency where the phase is zero
*
******************************************************************************/
UINT16 ScanSonicOn::LineofFitData(UINT32* Freq, INT16* Phase, UINT16 DataSize)
{
	unsigned int Indx;                      //Loop control
		double SumXY = 0,                 //Sum of (x*y)
			SumX = 0, SumY = 0,        //Sum of x, Sum of y.
			SumX2 = 0;                 //Sum of x^2
		double Slope;                     //Slope of line.

		for (Indx = 0; Indx < DataSize; Indx++){
			SumX = SumX + Freq[Indx];
			SumY = SumY + Phase[Indx];
			SumXY = SumXY + (Freq[Indx] * Phase[Indx]);
			SumX2 = SumX2 + (Freq[Indx] * Freq[Indx]);
		}
		Slope = (SumXY - ((SumX * SumY) / DataSize)) / (SumX2 - ((SumX * SumX) / DataSize));
		return ((SumX - (SumY / Slope)) / DataSize);
}


/**************************************************************************//**
* 
* \brief  - This method finds the Maximum Impedance around the resonance Point.
*
* \param  - UINT16 Index  Index in HornScanRawFrequency.
*
* \return  - Return the point with the Max Impedence
*
******************************************************************************/
UINT16 ScanSonicOn::FindHighestImpedencePoint(UINT16 Index)
{
	UINT16 Impedence = 0, FinalImpedence = 0, ResonancePoint = 0, Tempvar = 0;
	UINT32  TempAmp;
	for (Tempvar = (Index - SCAN_POINT_WINDOW); (Tempvar < (Index + SCAN_POINT_WINDOW)) && (Tempvar < SIZE_SCANBUFFER) && (Tempvar > 0); Tempvar++){
		//if (CP->Signature[Tempvar].Current == 0)
		//	CP->Signature[Tempvar].Current = 1;
		//TempAmp = CP->Signature[Tempvar].Amplitude;
		//TempAmp <<= 11;      //2047times more so all data bits makes contribution to calculation
		//Impedence = (TempAmp / (unsigned long)CP->Signature[Tempvar].Current);
		if (ScanBuffer[Tempvar].Current==0)
		{
		  ScanBuffer[Tempvar].Current = 1;
		}
		TempAmp=(UINT32)(ScanBuffer[Tempvar].Amplitude);
		TempAmp <<= 11; 
		Impedence = (TempAmp / (UINT32)ScanBuffer[Tempvar].Current);
		if (FinalImpedence <= Impedence){
			ResonancePoint = Tempvar;
			FinalImpedence = Impedence;
		}
	}

	for (Tempvar = (Index + SCAN_POINT_WINDOW); (Tempvar >= Index) && (Tempvar < SIZE_SCANBUFFER) && (Tempvar > 0); Tempvar--){
		//if (CP->Signature[Tempvar].Current == 0)
		//	CP->Signature[Tempvar].Current = 1;
		//TempAmp = (CP->Signature[Tempvar].Amplitude);
		//TempAmp <<= 11;      //2047times more so all data bits makes contribution to calculation
		//Impedence = (TempAmp / CP->Signature[Tempvar].Current);
		if (ScanBuffer[Tempvar].Current==0)
		{
		   ScanBuffer[Tempvar].Current = 1;
		}
		TempAmp=(UINT32)(ScanBuffer[Tempvar].Amplitude);
		TempAmp <<= 11; 
		Impedence = (TempAmp / (UINT32)ScanBuffer[Tempvar].Current);
		if (FinalImpedence <= Impedence){
			ResonancePoint = Tempvar;
			FinalImpedence = Impedence;
		}
	}

	return (ResonancePoint);
}

/**************************************************************************//**
* 
* \brief  - This method finds the Minimum Impedance around the resonance Point.
*
* \param  - UINT16 Index  Index in HornScanRawFrequency.
*
* \return  - Return the point with the Min Impedence
*
******************************************************************************/
UINT16 ScanSonicOn::FindLowestImpedencePoint(UINT16 Index)
{
        UINT16 Impedence = 0;
        UINT16 ResonancePoint = 0;
        UINT16 Tempvar = 0;
        INT16 FinalImpedence = 0, idx=0;
        UINT32 TempAmp, TempAmpInit;
 
        
       idx=Index - SCAN_POINT_WINDOW;
       TempAmpInit = (UINT32)(ScanBuffer[idx].Amplitude);
       TempAmpInit <<= 11;
       FinalImpedence=(TempAmpInit/(UINT32)ScanBuffer[idx].Current);

       for (Tempvar = (Index - SCAN_POINT_WINDOW); (Tempvar < (Index + SCAN_POINT_WINDOW)) && (Tempvar < SIZE_SCANBUFFER) && (Tempvar > 0); Tempvar++){
                //if (CP->Signature[Tempvar].Current == 0)
                //        CP->Signature[Tempvar].Current = 1;
                //TempAmp = (UINT32)(CP->Signature[Tempvar].Amplitude);
    	        if (ScanBuffer[Tempvar].Current == 0)
    	        {
    	        	ScanBuffer[Tempvar].Current = 1;
    	        }
    	        
    	        TempAmp = (UINT32)(ScanBuffer[Tempvar].Amplitude);
    	        TempAmp <<= 11;
    	        Impedence = (TempAmp / (UINT32)ScanBuffer[Tempvar].Current);
               // Impedence = (TempAmp / (UINT32)CP->Signature[Tempvar].Current);
                if (FinalImpedence >= Impedence)
                {      //FinalImpedence is Min. This condition is always true for the first time
                     ResonancePoint = Tempvar;
                     FinalImpedence = Impedence;
                }
        }

        for (Tempvar = (Index + SCAN_POINT_WINDOW); (Tempvar >= Index) && (Tempvar < SIZE_SCANBUFFER) && (Tempvar > 0); Tempvar--){
        ///for (Tempvar = (Index + SCAN_POINT_WINDOW); (Tempvar >= Index) && (Tempvar < HornScanGraphPointCounter) && (Tempvar > 0); Tempvar--){
                //if (CP->Signature[Tempvar].Current == 0)
                //        CP->Signature[Tempvar].Current = 1;
                //TempAmp = (UINT32)(CP->Signature[Tempvar].Amplitude);
                //TempAmp <<= 11;      //2047times more so all data bits makes contribution to calculation
                //Impedence = (TempAmp / (UINT32)CP->Signature[Tempvar].Current);
        	    if (ScanBuffer[Tempvar].Current == 0)
        	    {
        	      ScanBuffer[Tempvar].Current = 1;
        	    }
        	    	        
        	    TempAmp = (UINT32)(ScanBuffer[Tempvar].Amplitude);
        	    TempAmp <<= 11;
        	    Impedence = (TempAmp / (UINT32)ScanBuffer[Tempvar].Current);
                if (FinalImpedence >= Impedence){
                        ResonancePoint = Tempvar;
                        FinalImpedence = Impedence;
                }
        }
        return (ResonancePoint);
}

/**************************************************************************//**
* 
* \brief  - This method finds the resonance points around the zero crossing.
*
* \param  - None.
*
* \return  - None
*
******************************************************************************/
void ScanSonicOn::FindResonanceFrequency()
{
    bool Found=false;
    UINT16 XaxisIndex, YaxisIndex, ZaxisIndex, Numfails, Indx;
    UINT32 TempFreq[HS_CLUSTERPTS];   // Frequency Indexes for cluster points.
    INT16 TempPhase[HS_CLUSTERPTS];  // Phase Indexes for cluster points.
    UINT16 TempAmplitude[HS_CLUSTERPTS];
    UINT16 TempCurrent[HS_CLUSTERPTS];
    INT16 ClusterIndex[HS_CLUSTERPTS];
	bool ExitFlag = false;
	bool ParallelFound = false;
	UINT32 Voltage = 0, Current = 0;
	UINT16 ActualResonancePoint = 0, ActualResonanceFrequency = 0;
    INT16 TempParallelImpedence = 0;
    INT16 TempSeriesImpedence = -1;
    UINT32 MainParallelResonanceFreq, MainSeriesResonanceFreq;
    INT32 SignValue;
    UINT32  ScanOutcomeFreq = 0, ScanOutcomeFreqA =0;
    UINT32 ScanAmpAtParallelFrequency[NUMRESONANCES];
    UINT16 ScanCurrentAtParallelFrequncy[NUMRESONANCES];
    UINT32 ScanImpedenceAtParallelFrequncy[NUMRESONANCES];
    UINT32 TotalSeriesFreq = 0;
    INT32 MemoryValue = 0;
    
    XaxisIndex = YaxisIndex = ZaxisIndex = Numfails = Indx = 0;
    ///for(XaxisIndex = HS_HALFWINDOWSIZE; (XaxisIndex < (2000- HS_HALFWINDOWSIZE)) && (ZaxisIndex < NUMRESONANCES); XaxisIndex++){
    for(XaxisIndex = HS_HALFWINDOWSIZE; (XaxisIndex < (NumScanPoints - HS_HALFWINDOWSIZE)) && (ZaxisIndex < NUMRESONANCES); XaxisIndex++){
   		 //printf("all=%d \n", HornScanRawFrequency[XaxisIndex]);
   		 if((HornScanRawPhase[XaxisIndex - HS_HALFWINDOWSIZE] > 0) && (HornScanRawPhase[XaxisIndex + HS_HALFWINDOWSIZE] < 0)){
          Found = true;  //Cluster is found.
   		  printf("series \n");
            ResonanceFreqType[ZaxisIndex] = FREQ_SERIES;
            Numfails = 0;
         }
         else if((HornScanRawPhase[XaxisIndex - HS_HALFWINDOWSIZE] < 0) && (HornScanRawPhase[XaxisIndex + HS_HALFWINDOWSIZE] > 0)){
            Found = true;   //Cluster is found
   		    printf("parallel =%d \n", HornScanRawFrequency[XaxisIndex]);
            ResonanceFreqType[ZaxisIndex] = FREQ_PARALLEL;
            Numfails = 0;
         }
         else
            Numfails++;

         if(Found){
            TempFreq[YaxisIndex] 		= HornScanRawFrequency[XaxisIndex];
            TempPhase[YaxisIndex] 		= HornScanRawPhase[XaxisIndex];
            TempAmplitude[YaxisIndex] = ScanBuffer[XaxisIndex].Amplitude;
            TempCurrent[YaxisIndex] = ScanBuffer[XaxisIndex].Current;
			//TempAmplitude[YaxisIndex] 	= CP->Signature[XaxisIndex].Amplitude;
			//TempCurrent[YaxisIndex] 	= CP->Signature[XaxisIndex].Current;            
            ClusterIndex[YaxisIndex++]  = XaxisIndex;
         }
         /* the following is to make sure that we don't overflow the cluster vectors*/
   	if (YaxisIndex >= HS_CLUSTERPTS)
   		//Too many points, so force analysis.
   		Numfails = HS_FAILLIMIT;

   	if (Found && (Numfails >= HS_FAILLIMIT)){  //Found end of cluster.
   		ResonanceFreqs[ZaxisIndex] = LineofFitData(TempFreq, TempPhase, YaxisIndex);
   		
   		if (ResonanceFreqType[ZaxisIndex] == FREQ_PARALLEL)
   		{
   			ScanOutcomeFreq = ResonanceFreqs[ZaxisIndex];
   			//found parallel frequency. Search the nearest of same in TempFreq array.Save the amplitude for further usage.
   			for (Indx = 0; ((Indx < (YaxisIndex - 1)) && (ExitFlag == false)); Indx++){
   				if (ScanOutcomeFreq == (UINT32)TempFreq[Indx]){
   					ActualResonancePoint = FindHighestImpedencePoint(ClusterIndex[Indx]);   					
   					ActualResonanceFrequency = HornScanRawFrequency[ActualResonancePoint];
   					ResonanceFreqs[ZaxisIndex] = ActualResonanceFrequency;
   					printf("FREQ_PARALLEL - ResonanceFrequency-1=%d\n", ActualResonanceFrequency);
					//ScanAmpAtParallelFrequency[ZaxisIndex] = CP->Signature[ActualResonancePoint].Amplitude;
					//ScanCurrentAtParallelFrequncy[ZaxisIndex] = CP->Signature[ActualResonancePoint].Current;
   					ScanAmpAtParallelFrequency[ZaxisIndex] = ScanBuffer[ActualResonancePoint].Amplitude;
   					ScanCurrentAtParallelFrequncy[ZaxisIndex] = ScanBuffer[ActualResonancePoint].Current;
   					ExitFlag = true;
   				}
                else if ((ScanOutcomeFreq > (UINT32)TempFreq[Indx]) && (ScanOutcomeFreq < (UINT32)TempFreq[Indx + 1])){   					
                	ActualResonancePoint = FindHighestImpedencePoint(ClusterIndex[Indx]);
   					ActualResonanceFrequency = HornScanRawFrequency[ActualResonancePoint];
   					ResonanceFreqs[ZaxisIndex] = ActualResonanceFrequency;
   					printf("FREQ_PARALLEL - ResonanceFrequency-2=%d\n", ActualResonanceFrequency);
   					ScanAmpAtParallelFrequency[ZaxisIndex] = ScanBuffer[ActualResonancePoint].Amplitude;
   					ScanCurrentAtParallelFrequncy[ZaxisIndex] = ScanBuffer[ActualResonancePoint].Current;
   					ExitFlag = true;
   				}
   			}//for Indx    
   			ExitFlag = false;
   		}//FREQ_PARALLEL
   		else if (ResonanceFreqType[ZaxisIndex] == FREQ_SERIES) ////case FREQ_SERIES:
   		{
   			printf("series freq found\n");
   			ScanOutcomeFreqA = ResonanceFreqs[ZaxisIndex];
   			//found parallel frequency. Search the nearest of same in TempFreq array. Save the amplitude for further usage.
   			for (Indx = 0; ((Indx < (YaxisIndex - 1)) && (ExitFlag == false)); Indx++)
   			{
   				if (ScanOutcomeFreqA == (UINT32)TempFreq[Indx])
   				{
   					ActualResonancePoint = FindLowestImpedencePoint(ClusterIndex[Indx]);
   					ActualResonanceFrequency = HornScanRawFrequency[ActualResonancePoint];
   					ResonanceFreqs[ZaxisIndex] = ActualResonanceFrequency;
   					printf("FREQ_SERIES ResonanceFrequency-1=%d\n", ActualResonanceFrequency);
   					ScanAmpAtParallelFrequency[ZaxisIndex] = ScanBuffer[ActualResonancePoint].Amplitude;
   					ScanCurrentAtParallelFrequncy[ZaxisIndex] = ScanBuffer[ActualResonancePoint].Current;
   					ExitFlag = true;
   				}
   				else if ((ScanOutcomeFreqA > (UINT32)TempFreq[Indx]) && (ScanOutcomeFreqA < (UINT32)TempFreq[Indx + 1]))
   				{
   					ActualResonancePoint = FindLowestImpedencePoint(ClusterIndex[Indx]);
   					ActualResonanceFrequency = HornScanRawFrequency[ActualResonancePoint];
   					ResonanceFreqs[ZaxisIndex] = ActualResonanceFrequency;
   					printf("FREQ_SERIES ResonanceFrequency-2=%d\n", ActualResonanceFrequency);
   					ScanAmpAtParallelFrequency[ZaxisIndex] = ScanBuffer[ActualResonancePoint].Amplitude;
   					ScanCurrentAtParallelFrequncy[ZaxisIndex] = ScanBuffer[ActualResonancePoint].Current;
   					ExitFlag = true;
   				}
   			}//for Indx   		
   			ExitFlag = false;
   		}//FREQ_SERIES
   		 
   		ZaxisIndex++;
   		Found = false;
   		Numfails = 0;
   		YaxisIndex = 0;   		
   	}//if Found
   	
   } //End of for loop to search for clusters. 
    
    
    XaxisIndex = 0;
    ExitFlag = false;

	if (ZaxisIndex >= 1) {
		while (ExitFlag == false) {
			if (ScanCurrentAtParallelFrequncy[XaxisIndex] == 0)
				ScanCurrentAtParallelFrequncy[XaxisIndex] = 1;
			if (ScanAmpAtParallelFrequency[XaxisIndex] == 0)
				ScanAmpAtParallelFrequency[XaxisIndex] = 1;
			Voltage = (UINT32) ScanAmpAtParallelFrequency[XaxisIndex];
			Voltage <<= 11;   //multiply by 2048 so all bits contribute
			Current = (UINT32) ScanCurrentAtParallelFrequncy[XaxisIndex];
			ScanImpedenceAtParallelFrequncy[XaxisIndex] = (UINT32) (Voltage
					/ Current);
			if (ResonanceFreqType[XaxisIndex] == FREQ_PARALLEL) {
				ParallelFound = true;
				printf("\n Freq Par %d", ResonanceFreqs[XaxisIndex]);
				if (ScanImpedenceAtParallelFrequncy[XaxisIndex]
						> TempParallelImpedence) {
					MainParallelResonanceFreq = ResonanceFreqs[XaxisIndex];
					TempParallelImpedence = ScanImpedenceAtParallelFrequncy[XaxisIndex];
					CP->ScanResult.SetResonantFrequency(HornScanResults::scanResultIdx = MAIN_PARALLEL_FREQ, MainParallelResonanceFreq);
				}
			}
			if (ResonanceFreqType[XaxisIndex] == FREQ_SERIES) {
				printf("\n Freq Ser %d", ResonanceFreqs[XaxisIndex]);
				if ((ScanImpedenceAtParallelFrequncy[XaxisIndex]
						< TempSeriesImpedence)
						&& (ResonanceFreqs[XaxisIndex] != 0)) {
					MainSeriesResonanceFreq = ResonanceFreqs[XaxisIndex];
					TempSeriesImpedence = ScanImpedenceAtParallelFrequncy[XaxisIndex];
					CP->ScanResult.SetResonantFrequency(HornScanResults::scanResultIdx = FIRST_SERIES_FREQ, MainSeriesResonanceFreq);
				}
			}
			printf("Impedence %d", ScanImpedenceAtParallelFrequncy[XaxisIndex]);
			if (++XaxisIndex >= ZaxisIndex)
				ExitFlag = true;
		}
		//Ignore the parrallel frequency which has impedence less than 10 % of peak parallel impedence
		for (UINT32 i = 0; i < NUMRESONANCES; i++) {
			if (ResonanceFreqType[i] == FREQ_PARALLEL) {
				if (TempParallelImpedence > (ScanImpedenceAtParallelFrequncy[i] * 10))
					ResonanceFreqType[i] = FREQ_NONE;
			}
		}
		bool SeriesFreq = false;
		TotalSeriesFreq = 0;
		UINT8 SeriesIndex = 0;
		for (INT32 i = 0; i < NUMRESONANCES; i++) {
			if (ResonanceFreqType[i] == FREQ_SERIES) {
				//take the avarage of consecutive series frequencies and
				//declare it as series resonance frequency
				if (SeriesFreq == true) {
					TotalSeriesFreq += ResonanceFreqs[i];
					ResonanceFreqType[SeriesIndex] = FREQ_NONE;
					ResonanceFreqs[i] = TotalSeriesFreq / 2;
					SeriesIndex = i;
				} else {
					SeriesIndex = i;
					SeriesFreq = true;
					TotalSeriesFreq += ResonanceFreqs[i];
				}
			} else if (ResonanceFreqType[i] == FREQ_PARALLEL)
				SeriesFreq = false;
		}
	}
	if (ParallelFound == true) {
		// Found a parallel one
		ScanOutcomeFreq = MainParallelResonanceFreq;
		printf("\n Main Parallel Frequency %d", MainParallelResonanceFreq);		
		MemoryValue = ScanOutcomeFreq;
		//CP->stackRecipe.SetData(StackRecipe::DIGITAL_TUNE, MainParallelResonanceFreq*10);
		//CP->stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, Memoffset);
	} else {
		printf("\n not Parallel freq"); 
		//CP->stackRecipe.SetData(StackRecipe::DIGITAL_TUNE, CP->stackRecipe.GetData(StackRecipe::MID_BAND));
		//CP->stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, Memoffset);
		
	}
}

/* 
****************************************************************************
* \brief   - Calculate horn scan progress status.
*
* \param   - None.
*
* \return  - None.
******************************************************************************
*/
void ScanSonicOn::ProgressBar()
{
	if(HornScanGraphPointCounter % PercentInterval == 0)
	{
		Progress 		= PERCENT_INTERVAL * IntervalCounter;
		IntervalCounter = IntervalCounter + 1;
		if(Progress <= 100)
		{
			UpdateProgressBar(Progress);
		}
	}
}

/**************************************************************************//**
* \brief   - Update horn scan progress to QT - UI.
*
* \param   - UINT8 Status.
*
* \return  - None.
******************************************************************************/
void ScanSonicOn::UpdateProgressBar(UINT8 Status)
{
	Message Send;
	Send.msgID = TO_UI_TASK_PROGRESS_BAR_STATUS;
	memcpy(Send.Buffer,&Status,sizeof(Status));

	if(msgQSend(UI_Q_ID,reinterpret_cast<char*>(&Send), sizeof(Send),NO_WAIT, MSG_PRI_NORMAL) != OK)
	{
		LOGERR("-----------Error on msgQSend----------",0,0,0);
	}
}

void ScanSonicOn::Display()
{
	int idx = 1;
	for(;idx <= HornScanGraphPointCounter; idx++)
	{
		printf("Counter : %d\n",idx);
		printf("Frequency : %d\n",CP->Signature[idx].Frequency);
		printf("Amplitude : %d\n",CP->Signature[idx].Amplitude);
		printf("Phase 	  : %f\n",CP->Signature[idx].Phase);
		printf("Curent 	  : %d\n",CP->Signature[idx].Current);
		printf("\n");
	}
}

/**************************************************************************//**
* \brief   - This function sets the Digital Tune frequency
*                       for the system at start up
*
* \param   - None.
*
* \return  - None.
******************************************************************************/

void ScanSonicOn::UpdateDigitalTuneAndStartFrequency()
{
	UINT32 MidBand=0;
	UINT32 MinFrequency=0;
	UINT32 MaxFrequency=0;
	UINT32 ResonantFrequency=0;
	INT32  DigitalTune=0;
	UINT32 NewStartFrequency = 0;
	INT32  MemoryOffset=0;
	INT32  MemoryValue=0;
	INT32  RFequency=0;
	UINT32	_dt;
	Message _message;
	
	switch(CP->powerupRecipe.Frequency)
	{
	  case 20:
	  {
		MidBand=F20KHZMIDBAND;
		MinFrequency=F20KHZMINABS;
		MaxFrequency=F20KHZMAXABS;
 	  }
	    break;
	  case 30:
	  {
		MidBand=F30KHZMIDBAND;
		MinFrequency=F30KHZMINABS;
		MaxFrequency=F30KHZMAXABS;
	  }
	    break;
	  case 40:
	  {
		MidBand=F40KHZMIDBAND;
		MinFrequency=F40KHZMINABS;
		MaxFrequency=F40KHZMAXABS;
	  }
	    break;  
	  default:
		printf("Unknown Selection\n");
	    break;
	}
	CP->stackRecipe.SetData(StackRecipe::MID_BAND, MidBand);
	if (ScanFailed==true)
	{
		MemoryOffset=0;
	}
	else
	{
		ResonantFrequency=CP->ScanResult.GetResonantFrequency(MAIN_PARALLEL_FREQ)*10;
		if ((ResonantFrequency>=MinFrequency)&&(ResonantFrequency<=MaxFrequency))
		{ 
			DigitalTune=ResonantFrequency;
			RFequency=ResonantFrequency;
		}
		else
		{   
			DigitalTune=MidBand;
			RFequency=ResonantFrequency;
		}	

		if(scanrecipe.SetDigTuneWithHornScan == 1)
		{
			// send it to the event log
			_dt = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE);
			
			CP->stackRecipe.SetData(StackRecipe::DIGITAL_TUNE, DigitalTune);
			MemoryOffset=0;
			CP->recipe["DigitalTune"]=DigitalTune;
			_message.msgID = TO_DB_TASK_LOG_EVENT;
			
			snprintf(_message.Buffer, sizeof(_message.Buffer),"%d,Identifier:Recipe %d:From:%d:To:%d" , HORN_SCAN_COMPLETED,
						CP->getActiveRecipeInstance().GetRecipeNumber(), (_dt/10), (CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE)/10));
			
			msgQSend(CP->GetMsgQId(cTaskName[DB_T]),reinterpret_cast<char*>(&_message), sizeof(_message),NO_WAIT, MSG_PRI_NORMAL);
		}
		else 
		{
			MemoryOffset=RFequency-(CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE));
		}
	}
    LOGDBG("The Digital Tune is after = %d",CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE),0,0);
	LOGDBG("memoryoffset=%d", MemoryOffset,0,0);
	CP->stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, MemoryOffset);			
	MemoryValue=CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE)+MemoryOffset;
		
	CP->stackRecipe.SetData(StackRecipe::MEMORY, MemoryValue);
		
	if (CP->stackRecipe.GetData(StackRecipe::INTERNAL_OFFSET_FLAG)==1)
	{
		 NewStartFrequency = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE)+CP->stackRecipe.GetData(StackRecipe::MEMORY_OFFSET)+CP->stackRecipe.GetData(StackRecipe::INTERNAL_FREQ_OFFSET);
	}
	else
	{
		 NewStartFrequency = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE)+CP->stackRecipe.GetData(StackRecipe::MEMORY_OFFSET);
	}
	LOGDBG("new start frequency = %d", NewStartFrequency,0,0);
	CP->recipe["StartFrequency"] = NewStartFrequency;
	CP->recipe["MemoryOffset"]=MemoryOffset;
	
	CP->m_RecipePC.UpdateDigitalTune(CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE));	/* Updates Digital Tune*/
	CP->m_RecipePC.UpdateMemoryOffset(CP->stackRecipe.GetData(StackRecipe::MEMORY_OFFSET));	/* Updates MEMORY_OFFSET*/
	CP->m_RecipePC.UpdateResonantFreq(NewStartFrequency);
	
#ifdef DEBUG
	cout << "HornScan new SF:" << NewStartFrequency << endl;
#endif
}



