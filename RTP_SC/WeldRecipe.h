/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef WELDRECIPE_H_
#define WELDRECIPE_H_

#include "Common.h"

#define USER_NAME_SIZE						16
#define RECIPE_NAME_SIZE					16
#define SYSINFO_SIZE						16
#define MAC_ADDR_SIZE      					18

/* Macro defined for set max range value of AC-Weld recipe */ 
#define AC_MAX_WeldForce					3500
#define AC_MAX_ForceRampTime				1000
#define AC_MAX_HoldMode						10
#define AC_MAX_HoldTime						30000
#define AC_MAX_HoldForce					3500
#define AC_MAX_HoldForceRampTime			1000		// changed on 11/12/2017
#define AC_MAX_ExpectedPartContactPosition	125000	
#define AC_MAX_TotalCollapseTarget			50000		// changed on 11/12/2017
#define AC_MAX_ReadyPosition				125000
#define AC_MAX_DownAcceleration				10000
#define AC_MAX_DownMaxVelocity				500
#define AC_MAX_DownDeceleration				10000
#define AC_MAX_ReturnAcceleration			10000
#define AC_MAX_ReturnMaxVelocity			500
#define AC_MAX_ReturnDeceleration			10000
#define AC_MAX_NumForceSteps				10
#define AC_MAX_ForceStepAt					7			// changed on 11/12/2017
#define AC_MAX_ForceStepForce				3500		// changed on 11/12/2017
#define AC_MAX_ForceStepRampTime			1000		// changed on 11/12/2017
#define AC_MAX_PartContactWindowPlus		1			/* Need to get default values */
#define AC_MAX_PartContactWindowMinus		1
#define AC_MAX_ExcepectedPartContactOffset	1


/* Macro defined for set min range value of AC-Weld recipe */
#define AC_MIN_WeldForce					3
#define AC_MIN_ForceRampTime		        0
#define AC_MIN_HoldMode				        0
#define AC_MIN_HoldTime						0	
#define AC_MIN_HoldForce					3	
#define AC_MIN_HoldForceRampTime			0
#define AC_MIN_ExpectedPartContactPosition	0
#define AC_MIN_TotalCollapseTarget			0		// changed on 11/12/2017
#define AC_MIN_ReadyPosition				0
#define AC_MIN_DownAcceleration				1
#define AC_MIN_DownMaxVelocity				1		
#define AC_MIN_DownDeceleration				1
#define AC_MIN_ReturnAcceleration		    1
#define AC_MIN_ReturnMaxVelocity			1
#define AC_MIN_ReturnDeceleration		    1
#define AC_MIN_NumForceSteps				0		// changed on 11/12/2017
#define AC_MIN_ForceStepAt					1		// changed on 11/12/2017
#define AC_MIN_ForceStepForce				3		// changed on 11/12/2017
#define AC_MIN_ForceStepRampTime		    0
#define AC_MIN_ForceStepAtValue				1
#define AC_MIN_PartContactWindowPlus		1		/* Need to get default values */
#define AC_MIN_PartContactWindowMinus		1
#define AC_MIN_ExcepectedPartContactOffset	1


/* Macro defined for set MAX range value of PC-Weld recipe */
#define PC_MAX_AmplitudeLoopC1				20000   
#define PC_MAX_AmplitudeLoopC2				20000
#define PC_MAX_PhaseLoop					500
#define PC_MAX_PhaseLoopCF					500

#define PC_MAX_20KFrequencyLow	    		500    	//changed on 11/12/2017
#define PC_MAX_30KFrequencyLow		    	750     //changed on 11/12/2017
#define PC_MAX_40KFrequencyLow			    1000    //changed on 11/12/2017

#define PC_MAX_20KFrequencyHigh			    500    	//changed on 11/12/2017
#define PC_MAX_30KFrequencyHigh			    750     //changed on 11/12/2017
#define PC_MAX_40KFrequencyHigh			    1000    //changed on 11/12/2017

#define PC_MAX_PhaseLimitTime				20000    //changed on 11/12/2017
#define PC_MAX_PhaseLimit					9765	
#define PC_MAX_ControlMode					2		 //changed on 11/12/2017		
#define PC_MAX_BlindtimeSeek				1000     //changed on 7/10/2017
#define PC_MAX_Blindtimeweld				1000
#define PC_MAX_DDSSwitchtime				10000
#define PC_MAX_WeldRampTime                 999
#define PC_MAX_SeekAmplitude				100
#define PC_MAX_WeldAmplitude				100             //changed on 7/10/2017
#define PC_MAX_WeldPWM			         	50              //changed on 7/10/2017
#define PC_MAX_Parameter7					1234111106      //changed on 7/10/2017
#define PC_MAX_Parameter8					1234111107      //changed on 7/10/2017
#define PC_MAX_Parameter9					1234111108      //changed on 7/10/2017
#define PC_MAX_Parameter10					1234111109      //changed on 7/10/2017
#define PC_MAX_Parameter11					1234111110      //changed on 7/10/2017
#define PC_MAX_Parameter12					1234111111      //changed on 7/10/2017
#define PC_MAX_Parameter13					1234111112      //changed on 7/10/2017
#define PC_MAX_Parameter14					1234111113      //changed on 7/10/2017
#define PC_MAX_Parameter15					1234111114      //changed on 7/10/2017
#define PC_MAX_FLimitTime					1000
#define PC_MAX_AmpProportionalGain			500
#define PC_MAX_AmpIntegralGain				500
#define PC_MAX_PhaseProportionalGain		500
#define PC_MAX_FrequencyWindowSize			500
#define PC_MAX_PFofPhasecontrolloop			500             //changed on 7/10/2017

#define PC_MAX_20KPIFrequencyLow			500				//changed on 11/12/2017
#define PC_MAX_30KPIFrequencyLow			750				//changed on 11/12/2017
#define PC_MAX_40KPIFrequencyLow			1000			//changed on 11/12/2017

#define PC_MAX_20KPIFrequencyHigh			500				//changed on 11/12/2017
#define PC_MAX_30KPIFrequencyHigh			750				//changed on 11/12/2017
#define PC_MAX_40KPIFrequencyHigh			1000			//changed on 11/12/2017

#define PC_MAX_PIPhaseLimitTime				20000
#define PC_MAX_PIPhaseLimit					9765

/* Macro defined for set MIN range value of PC-Weld recipe */
#define PC_MIN_ENTRY 						38						
#define PC_MIN_AmplitudeLoopC1				1
#define PC_MIN_AmplitudeLoopC2				1				
#define PC_MIN_PhaseLoop					50    //changed on 7/10/2017					
#define PC_MIN_PhaseLoopCF					50	 //changed on 7/10/2017		

#define PC_MIN_20KFrequencyLow   		    1	 //changed on 7/10/2017
#define PC_MIN_30KFrequencyLow  		    1	 //changed on 7/10/2017
#define PC_MIN_40KFrequencyLow  		    1	 //changed on 7/10/2017

#define PC_MIN_20KFrequencyHigh  			1	 //changed on 11/12/2017
#define PC_MIN_30KFrequencyHigh	    		1	 //changed on 11/12/2017
#define PC_MIN_40KFrequencyHigh 			1	 //changed on 11/12/2017

#define PC_MIN_PhaseLimitTime				1				
#define PC_MIN_PhaseLimit					4					
#define PC_MIN_ControlMode					1	
#define PC_MIN_BlindtimeSeek				0			
#define PC_MIN_Blindtimeweld				0				
#define PC_MIN_DDSSwitchtime				0				
#define PC_MIN_SeekAmplitude  				10		//changed on 7/10/2017
#define PC_MIN_WeldRampTime				    1
#define PC_MIN_WeldAmplitude				10	   //changed on 7/10/2017	
#define PC_MIN_WeldPWM      				10	   //changed on 7/10/2017
#define PC_MIN_Parameter7					0					
#define PC_MIN_Parameter8					0					
#define PC_MIN_Parameter9					0					
#define PC_MIN_Parameter10					0					
#define PC_MIN_Parameter11					0					
#define PC_MIN_Parameter12					0					
#define PC_MIN_Parameter13					0					
#define PC_MIN_Parameter14					0					
#define PC_MIN_Parameter15					0					
#define PC_MIN_FLimitTime					0					
#define PC_MIN_AmpProportionalGain			1			
#define PC_MIN_AmpIntegralGain				1				
#define PC_MIN_PhaseProportionalGain		1		
#define PC_MIN_FrequencyWindowSize			1			
#define PC_MIN_PFofPhasecontrolloop			1	    //changed on 7/10/2017		
#define PC_MIN_PIFrequencyLow				1				
#define PC_MIN_PIFrequencyHigh				1				
#define PC_MIN_PIPhaseLimitTime				1				
#define PC_MIN_PIPhaseLimit					4
#define MAX_NO_OF_AMPLITUDE_STEP 			10

/* Macro defined for set default value of Weld recipe */
#define AMPLOOPC1_20KDEFAULT     	  		205
#define AMPLOOPC1_30KDEFAULT     	  		205
#define AMPLOOPC1_40KDEFAULT     	  		205

#define AMPLOOPC2_20KDEFAULT     	  		370
#define AMPLOOPC2_30KDEFAULT     	  		370
#define AMPLOOPC2_40KDEFAULT     	  		370

#define PLOOP_20KDEFAULT         	  		140
#define PLOOP_30KDEFAULT         	  		100
#define PLOOP_40KDEFAULT          	  		60

#define PLOOPCF_20KDEFAULT 	   		  		200
#define PLOOPCF_30KDEFAULT 			  		200
#define PLOOPCF_40KDEFAULT 	   		  		200

#define FREQENCYLOW_20KDEFAULT 	   	  		500	  	//changed on 11/12/2017
#define FREQENCYLOW_30KDEFAULT 	      		750		//changed on 11/12/2017
#define FREQENCYLOW_40KDEFAULT 	   	  		1000		//changed on 11/12/2017

#define FREQENCYHIGH_20KDEFAULT 	  		500		//changed on 11/12/2017
#define FREQENCYHIGH_30KDEFAULT 	  		750		//changed on 11/12/2017
#define FREQENCYHIGH_40KDEFAULT 	  		1000		//changed on 11/12/2017

#define PHASELIMITTIME_DEFAULT        		500   //changed on 7/6/2017
#define PHASELIMIT_DEFAULT	          		1000  //changed on 7/6/2017
#define BLINDTIMESEEK_DEFAULT         		0     //changed on 7/6/2017
#define BLINDTIMEWELD_DEFAULT 	     		0     //changed on 7/6/2017

#define DDSSWITCHTIME_20KDEFAULT      		500   //changed on 7/6/2017
#define DDSSWITCHTIME_30KDEFAULT      		200   //changed on 7/6/2017
#define DDSSWITCHTIME_40KDEFAULT      		200   //changed on 7/6/2017

#define WELDAMPLITUDE_DEFAULT         		100   //changed on 7/10/2017
#define WELDPWM_DEFAULT               		50    //changed on 7/10/2017

#define PARAMETER7_20KDEFAULT         		800   //changed on 7/6/2017
#define PARAMETER7_30KDEFAULT         		1200  //changed on 7/6/2017 
#define PARAMETER7_40KDEFAULT         		0     //changed on 7/6/2017

#define PARAMETER8_20KDEFAULT         		1000  //changed on 7/6/2017
#define PARAMETER8_30KDEFAULT         		1000  //changed on 7/6/2017
#define PARAMETER8_40KDEFAULT         		0     //changed on 7/6/2017

#define PARAMETER9_20KDEFAULT         		200   //changed on 7/6/2017
#define PARAMETER9_30KDEFAULT         		200   //changed on 7/6/2017
#define PARAMETER9_40KDEFAULT         		0     //changed on 7/6/2017

#define PARAMETER10_20KDEFAULT	      		5     //changed on 7/6/2017
#define PARAMETER10_30KDEFAULT	      		5     //changed on 7/6/2017
#define PARAMETER10_40KDEFAULT	      		0     //changed on 7/6/2017

#define PARAMETER11_20KDEFAULT        		0     //changed on 7/6/2017
#define PARAMETER11_30KDEFAULT        		0     //changed on 7/6/2017
#define PARAMETER11_40KDEFAULT        		0     //changed on 7/6/2017

#define PARAMETER12_20KDEFAULT	      		0     //changed on 7/6/2017
#define PARAMETER12_30KDEFAULT	      		0     //changed on 7/6/2017
#define PARAMETER12_40KDEFAULT	      		0     //changed on 7/6/2017

#define PARAMETER13_DEFAULT           		20000  
#define PARAMETER14_DEFAULT	          		20000
#define PARAMETER15_DEFAULT	          		20000
#define FLIMITTIME_DEFAULT            		1000

#define AMPPROPGAIN_20KDEFAULT 	      		100            //changed on 7/10/2017
#define AMPPROPGAIN_30KDEFAULT 	      		205            //changed on 7/10/2017
#define AMPPROPGAIN_40KDEFAULT 	      		205            //changed on 7/10/2017

#define AMPINTEGRALGAIN_20KDEFAULT 	  		5           //changed on 7/10/2017
#define AMPINTEGRALGAIN_30KDEFAULT 	  		5           //changed on 7/10/2017
#define AMPINTEGRALGAIN_40KDEFAULT 	  		5           //changed on 7/10/2017

#define PHASEINTEGRALGAIN_20KDEFAULT  		22          //changed on 7/10/2017
#define PHASEINTEGRALGAIN_30KDEFAULT  		25          //changed on 7/10/2017
#define PHASEINTEGRALGAIN_40KDEFAULT  		41          //changed on 7/10/2017

#define PHASEPROPGAIN_20KDEFAULT   			22          //changed on 7/10/2017
#define PHASEPROPGAIN_30KDEFAULT   			25          //changed on 7/10/2017
#define PHASEPROPGAIN_40KDEFAULT   			41          //changed on 7/10/2017

#define FREQWINSIZE_DEFAULT         		250

#define PFOFPHASECONTROLLOOP_20KDEFAULT 	450    		//changed on 7/10/2017
#define PFOFPHASECONTROLLOOP_30KDEFAULT 	200    		//changed on 7/10/2017
#define PFOFPHASECONTROLLOOP_40KDEFAULT 	200    		//changed on 7/10/2017

#define PIFREQUENCYLOW_DEFAULT      		500
#define PIFREQUENCYHIGH_DEFAULT     		500
#define PIRPHASELIMITTIME_DEFAULT   		500
#define PIRPHASELIMIT_DEFAULT       		1000

#define START_FREQUENCY20KDEFAULT			199500
#define START_FREQUENCY30KDEFAULT			300000
#define START_FREQUENCY40KDEFAULT			399000

/* Default value of weld recipe for the AC */
#define WELDFORCE_DEFAULT           		10
#define FORCERAMPTIME_DEFAULT      			0
#define TOTALCOLLAPSETARGET_DEFAULT 		0
#define HOLDFORCE_DEFAULT           		10
#define HOLDFORCERAMPTIME_DEFAULT  			0
#define PARTCONTACTPOSITION_DEFAULT			0
#define TOTALCOLLAPSETAR_DEFAULT    		0
#define READYPOSITION_DEFAULT       		0
#define DOWNACCELERATION_DEFAULT    		1000
#define DOWNMAXVELOCITY_DEFAULT     		200
#define DOWNDECELERATION_DEFAULT    		10000
#define RETURNACCELERATION_DEFAULT  		10000
#define RETURNMAXVELOCITY_DEFAULT   		200
#define RETURNDECELERATION_DEFAULT  		10000
#define NUMFORCESTEPS_DEFAULT       		1		// changed 11/12/2017
#define	MAX_NO_OF_FORCE_STEP 				10
#define FORCE_AT 							1

#define F20KHZMIDBAND   (199500)
#define F20KHZMINABS (F20KHZMIDBAND - 5000)
#define F20KHZMAXABS (F20KHZMIDBAND + 5000)

#define F30KHZMIDBAND (300000)
#define F30KHZMINABS (F30KHZMIDBAND - 7500)
#define F30KHZMAXABS (F30KHZMIDBAND + 7500)

#define F40KHZMIDBAND (399000)
#define F40KHZMINABS (F40KHZMIDBAND - 10000)
#define F40KHZMAXABS (F40KHZMIDBAND + 10000)

//typedef  
	//enum { STEPBYTIME, STEPBYPOWER, STEPBYENERGY, STEPBYABSOLUTEDISTANCE, STEPBYCOLLAPSEDISTANCE}STEPPINGMODE;
/* Modified - To in-line with QT-UI definitions */
typedef
	enum {STEPBYTIME = 1, STEPBYENERGY, STEPBYPOWER, STEPBYABSOLUTEDISTANCE, STEPBYCOLLAPSEDISTANCE }STEPPINGMODE;

/* PROPORTIONAL GAIN FACTOR - LOW = 1.5, MEDIUM = 3.0 and HIGH = 5.0 */
typedef
	enum {CONTROL_LOW = 1, CONTROL_MEDIUM, CONTROL_HIGH}WELDFORCECONTROL;
	
typedef enum {
	SAVED,
	VALIDATED,
	UNSAVED
}RECIPE_STATUS;
	
typedef struct Limits
{
   bool	bIsEnabled;
   bool bIsLowEnabled;
   bool bIsHighEnabled;
   UINT32 LowLimit;
   UINT32 HighLimit;
}Limits;

typedef struct SuspectReject
{
	Limits Time;	
	Limits Energy;
	Limits PeakPower;
	Limits CollapseDistance;
	Limits AbsoluteDistance;
	Limits TriggerDistance;
	Limits WeldForce;
	Limits Frequency;
}SuspectReject;

typedef struct CutoffLimitsProp
{
	bool 	bIsEnabled;
	UINT32 	Value;
}CutOffLimitsProp;

typedef struct CutOffLimits
{
	CutOffLimitsProp  	PeakPowerCutoff;
    CutOffLimitsProp  	AbsoluteDistanceCutoff;
	CutOffLimitsProp  	CollapseDistanceCutoff;
	CutOffLimitsProp  	TimeCutoff;  
	CutOffLimitsProp    FrequencyLowCutoff;
	CutOffLimitsProp    FrequencyHighCutoff;
	CutOffLimitsProp  	EnergyCutoff;
    bool			GroundDetectCutoff;
}CutOffLimits;

typedef struct Burst
{
	bool   bIsEnabled;
	UINT8  BurstAmplitude;
	UINT32 BurstDelay;
	UINT32 BurstTime;
}Burst;

typedef struct Trigger
{
	bool   bIsEnabled;
	bool   bIsAutoEnabled;
	UINT8  TriggerAmplitude;
	bool   bIsDistanceEnabled;
	UINT32 PreTriggerDistance;
}Trigger;

typedef struct Brake
{
	bool   bIsEnabled;
	UINT8  EnergyBrakeAmplitude;
	UINT32 EnergyBrakeTime;	
}EnergyBrake;

typedef struct Dynamic
{
    INT32	Reactivity;
    bool    IsForceLevelEnabled;
    INT32   ForceLevel;
    INT32   ForceLevelTime;
}Dynamic;
typedef struct WeldRecipeSC
{
	INT32  			RecipeNumber;
	INT32			RecipeRevNumber;
	INT32  			WeldMode;
	INT32  			ModeValue;
	INT32  			HoldTime;
	INT32  			TriggerForce;
	INT32  			TriggerDistance;
	INT32 			MaxWeldTimeout;
	UINT8  			NumAmplitudeSteps;
	UINT8  			AmplitudeStepAt;
	UINT8			NumForceSteps;
	UINT8  			ForceStepAt;
	UINT32  		ForceStepAtValue[MAX_NO_OF_FORCE_STEP];
	UINT32 			AmplitudeStepValue[MAX_NO_OF_AMPLITUDE_STEP];
	UINT16 			AmplitudeStep[MAX_NO_OF_AMPLITUDE_STEP];
	bool   			bIsCoolingValveEnabled;
	bool   			bIsSuspectLimitEnabled;
	bool   			bIsRejectLimitEnabled;
	bool			bIsCutoffLimitEnabled;
	CutOffLimits    CutoffLimit;
	Trigger 		PreTrigger;
	Burst			AfterBurst;
	Brake			EnergyBrake;
	SuspectReject 	SuspectLimit;
	SuspectReject 	RejectLimit;
	UINT32			TimedSeekPeriod;
	bool			bIsTimedSeekEnabled;
	bool			bIsPreWeldSeekEnabled;
	bool			bIsPostWeldSeekEnabled;
	bool			bIsRecipeValidated;
	UINT32 			StackSerialNo;
	char   			User[USER_NAME_SIZE];
	Dynamic         DynamicModeParam;
	UINT32			ScrubAmplitude;
}WeldRecipeSC;


typedef struct StackRecipeSC
{
	INT32  DigitalTune;		
	INT32  MemOffset;
	INT32  InternalOffset;
	UINT32 DigitalTuneMemory;
	bool   MemoryClear;
	bool   HornScanPassed;
	UINT32 SystemMidBand;	
	UINT32 Memory;
	INT32  InternalFreqOffset;
	INT32  InternalOffsetFlag;
	INT32  EndofWeldStore;
	bool   ClearMemoryAtPowerUp;
    bool   ClearMemoryWithReset;
	
}StackRecipeSC;



typedef struct WeldRecipePC
{
public :
	WeldRecipePC();
	
public:
	INT32	AmplitudeLoopC1;
	INT32	AmplitudeLoopC2;
	INT32	PhaseLoop; 
	INT32	PhaseLoopCF;
	INT32	FrequencyLow;
	INT32	FrequencyHigh; 
	INT32	PhaseLimitTime;
	INT32	PhaseLimit ;
	INT32	ControlMode;
	INT32	BlindtimeSeek; 
	INT32	Blindtimeweld;
	INT32	DDSSwitchtime;
	INT32	Parameter7 ;
	INT32	Parameter8;
	INT32	Parameter9;
	INT32	Parameter10;
	INT32	Parameter11;
	INT32	Parameter12;
	INT32	Parameter13;
	INT32	Parameter14;
	INT32	Parameter15;
	INT32	FLimitTime;
	INT32	AmpProportionalGain;
	INT32	AmpIntegralGain;
	INT32 	PhaseIntegralGain;
	INT32	PhaseProportionalGain;
	INT32	FrequencyWindowSize;
	INT32	PFofPhasecontrolloop;
	INT32	PIFrequencyLow;
	INT32	PIFrequencyHigh;
	INT32	PIPhaseLimitTime;
	INT32	PIPhaseLimit;
	INT32	WeldRampTime;
	UINT32  StartFrequency;
	INT32   MemoryOffset;
	INT32   DigitalTune;
	INT32   FrequencyOffset;
}WeldRecipePC;


typedef struct  WeldRecipeAC
{
public:
	WeldRecipeAC();

public:
	UINT16	WeldForce;
	UINT16	ForceRampTime;
	UINT16	HoldMode;
	UINT16	TotalCollapseTarget;
	UINT16	HoldForce;
	UINT16	HoldForceRampTime;
	UINT32	ExpectedPartContactPosition;
	UINT32	ReadyPosition;
	/*Actuator setup Advanced Feature parameter datatype range increased to 4 byte to support imperial conversion */
	UINT32	DownAcceleration;
	UINT32	DownMaxVelocity;
	UINT32	DownDeceleration;
	UINT32	ReturnAcceleration;
	UINT32	ReturnMaxVelocity;
	UINT32	ReturnDeceleration;
	UINT16  ExpectedPartContactOffset;
	UINT16  PartContactWindowMinus; 
	UINT16  PartContactWindowPlus;
	UINT16	NumForceSteps;
    UINT16	ForceStepRampTime[MAX_NO_OF_FORCE_STEP];
    UINT16	ForceStepForce[MAX_NO_OF_FORCE_STEP];
    UINT16	ReadyPositionToggle;
    UINT16	WeldForceControl;
}WeldRecipeAC;

/*Weld Recipe Ac structure similar to present at AC side for proper communication between SC and AC*/
typedef struct  WeldRecipeAcCompatibleWithAcBoard
{	
public:
	UINT16	WeldForce;
	UINT16	ForceRampTime;
	UINT16	HoldMode;
	UINT16	TotalCollapseTarget;
	UINT16	HoldForce;
	UINT16	HoldForceRampTime;
	UINT32	ExpectedPartContactPosition;
	UINT32	ReadyPosition;
	UINT16	DownAcceleration;
	UINT16	DownMaxVelocity;
	UINT16	DownDeceleration;
	UINT16	ReturnAcceleration;
	UINT16	ReturnMaxVelocity;
	UINT16	ReturnDeceleration;
	UINT16  ExpectedPartContactOffset;
	UINT16  PartContactWindowMinus; 
	UINT16  PartContactWindowPlus;
	UINT16	NumForceSteps;
    UINT16	ForceStepRampTime[MAX_NO_OF_FORCE_STEP];
    UINT16	ForceStepForce[MAX_NO_OF_FORCE_STEP];
    UINT16	ReadyPositionToggle;
    UINT16	WeldForceControl;
}WeldRecipeAcCompatibleWithAcBoard;

typedef struct WeldRecipe
{
	WeldRecipeSC	SCData;
	WeldRecipePC	PCData;
	WeldRecipeAC	ACData;
}WeldRecipe;


class WeldRecipeStatus
{
public:
	static RECIPE_STATUS GetRecipeStatus () { return recipeStatus;};
	static void SetRecipeStatus (RECIPE_STATUS status) { recipeStatus = status;};
	
private:
	static RECIPE_STATUS recipeStatus;
};


class PcWeldRecipe
{
public:
	PcWeldRecipe();
	PcWeldRecipe(const PcWeldRecipe& srcObj);
	PcWeldRecipe& operator=(const PcWeldRecipe& srcObj);
	~PcWeldRecipe();
	
	UINT32	GetResonantFreq();
	INT32	GetMemoryOffset();
	INT32	GetDigitalTune();
	INT32	GetFrequencyOffset();
	void   	UpdateResonantFreq(UINT32 freqeuncy);
	void    UpdateMemoryOffset(INT32 MemoryOffset);
	void    UpdateDigitalTune(INT32 DigitalTune);
	void    UpdateFrequencyOffset(INT32 FreqOffset);	
	void   	GetWeldRecipe(WeldRecipePC& recipe);
	void	SetWeldRecipe(WeldRecipePC& recipe);
		
private:
	WeldRecipePC m_Recipe;
};


class AcWeldRecipe
{
public:
	AcWeldRecipe();
	AcWeldRecipe(const AcWeldRecipe& srcObj);
	AcWeldRecipe& operator=(const AcWeldRecipe& srcObj);
	~AcWeldRecipe();
	
	UINT32	GetPartContactPos() const;
	UINT16  GetWeldForce() const;
	UINT16	GetForceStepForce(UINT8 index) const;
	void   	UpdatePartContactPos(UINT32 position);
	void   	GetWeldRecipe(WeldRecipeAC& recipe);
	void 	SetWeldRecipe(WeldRecipeAC& recipe);

private:
	WeldRecipeAC m_Recipe;
};

#endif /* WELDRECIPE_H_ */
