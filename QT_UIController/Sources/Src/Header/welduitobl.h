/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Weld UI to BL

***************************************************************************/
#ifndef WELDUITOBL_H
#define WELDUITOBL_H

typedef qint8 INT8;
typedef qint16 INT16;
typedef qint32 INT32;
typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
#define MAX_NO_OF_FORCE_STEP 10
#define MAX_NO_OF_AMPLITUDE_STEP 10


struct Limits
{
    bool IsEnabled;
    bool IsLowEnabled;
    bool IsHighEnabled;
    UINT32 Low;
    UINT32 High;
};
struct ControlLimits
{
    bool IsEnabled;
    UINT32 Value;
};
struct SuspectReject
{
    Limits Time;
    Limits Energy;
    Limits PeakPower;
    Limits CollapseDistance;
    Limits AbsoluteDistance;
    Limits TriggerDistance;
    Limits EndWeldForce;
    Limits Frequency;
};

struct Control
{
    ControlLimits     PeakPowerCutoff;
    ControlLimits     AbsoluteCutoff;
    ControlLimits     CollapseCutoff;
    ControlLimits     TimeCutoff;
    ControlLimits     FrequencyLow;
    ControlLimits     FrequencyHigh;
    ControlLimits     EnergyCutoff;
    bool              GroundDetectCutoff;

};

struct Burst
{
    bool   bIsEnabled;
    UINT8  BurstAmplitude;
    UINT32 BurstDelay;
    UINT32 BurstTime;
};

struct PreTrigger
{
    bool   bIsEnabled;
    bool   bIsAutoEnabled;
    UINT8  TriggerAmplitude;
    bool   bIsDistanceEnabled;
    UINT32 PreTriggerDistance;
};
struct Brake
{
    bool bIsEnabled;
    UINT8 EnergyBrakeAmplitude;
    UINT32 EnergyBrakeTime;
};
enum WELDFORCECONTROL
{
    CONTROL_LOW = 1,
    CONTROL_MEDIUM,
    CONTROL_HIGH
};

struct Dynamic
{
    INT32           Reactivity;
    bool            IsForceLevelEnabled;
    INT32           ForceLevel;
    INT32           ForceLevelTime;
};

struct  WeldRecipe_SC
{
    INT32           RecipeNumber;
    INT32           RecipeVerNumber;
    INT32           WeldMode;
    INT32           ModeValue;
    INT32           HoldTime;
    INT32           TriggerForce;
    INT32           TriggerDistance;
    INT32           MaxWeldTimeout;
    UINT8           NumAmplitudeSteps;
    UINT8           AmplitudeStepAt;
    UINT8           NumForceSteps;
    UINT8           ForceStepAt;
    UINT32          ForceStepValue[MAX_NO_OF_FORCE_STEP];
    UINT32          AmplitudeStepValue[MAX_NO_OF_AMPLITUDE_STEP];
    UINT16          AmplitudeStep[MAX_NO_OF_AMPLITUDE_STEP];
    bool            bIsCoolingValeEnabled;
    bool            IsGlobalSuspectEnabled;
    bool            IsGlobalRejectEnabled;
    bool            bIsControlLimitEnabled;
    Control         ControlLimits;
    PreTrigger      preTrigger;
    Burst           AfterBurst;
    Brake           EnergyBrake;
    SuspectReject   SuspectLimit;
    SuspectReject   RejectLimit;
    UINT32          TimedSeekPeriod;
    bool            bIsTimedSeekEnabled;
    bool            bIsPreWeldSeekEnabled;
    bool            bIsPostWeldSeekEnabled;
    bool            bIsRecipeValidated;
    UINT32          StackSerialNo;
    char            User[16];
//  char            RecipeName[16];
    Dynamic         DynamicModeParam;
    UINT32          ScrubAmplitude;
};

struct WeldRecipePC
{
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
    INT32	MemoryOffset;
    INT32	DigitalTune;
    INT32	FrequencyOffset;
};

struct WeldRecipeAC{
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
    UINT16  NumForceSteps;
    UINT16	ForceStepRampValue[MAX_NO_OF_FORCE_STEP];
    UINT16	ForceStep[MAX_NO_OF_FORCE_STEP];
    UINT16  ReadyPositionToggle;
    UINT16  WeldForceControl;
} ;

struct WeldRecipe_PC_AC
{
    WeldRecipePC m_weldRecipe_PC;
    WeldRecipeAC m_weldRecipe_AC;
};

struct RTF_LIST1
{
    UINT8	Multimode_Flag_B0                                   :2;
    UINT8	Time_Flag_B1                   						:2;
    UINT8  	Energy_Flag_B2                						:2;
    UINT8  	PeakPower_Flag_B3              						:2;
    UINT8  	CollapseDist_Flag_B4           						:2;
    UINT8  	AbsoluteDist_Flag_B5           						:2;
    UINT8 	GroundDetect_Flag_B6               					:2;
    UINT8  	Continous_Flag_B7                   				:2;
    UINT8  	PreTrigger_Flag_B8                      			:2;
    UINT8  	AfterBurst_Flag_B9                          		:2;
    UINT8  	EnergyBrake_Flag_B10                            	:2;
    UINT8  	AmplitudeStep_Flag_B11                              :2;
    UINT8  	ForceStep_Flag_B12                 					:2;
    UINT8 	AlarmLog_Flag_B13               					:2;
    UINT8  	EventLog_Flag_B14               					:2;
    UINT8  	WeldDataLog_Flag_B15               					:2;
    UINT8  	Reporting_Flag_B16               					:2;
    UINT8  	CrackedHorn_Flag_B17                				:2;
    UINT8  	USB_Flag_B18                            			:2;
    UINT8 	Binary_Flag_B19                             		:2;
    UINT8  	WebServices_Flag_B20                            	:2;
    UINT8  	OPC_UA_FLAG_B21                                     :2;
    UINT8  	UserIO_Flag_B22               						:2;
    UINT8  	ActuatorIO_Flag_B23               					:2;
    UINT8  	CycleCounter_Flag_B24               				:2;
    UINT8  	BatchCounter_Flag_B25               				:2;
    UINT8  	PowerMatchCurve_Flag_B26               				:2;
    UINT8  	SmartConverter_Flag_B27               				:2;
    UINT8  	ControlSpeed_Flag_B28                   			:2;
    UINT8  	Sequencing_Flag_B29                         		:2;
    UINT8  	AutoConverterCooling_Flag_B30                   	:2;
    UINT8  	FieldBus_Flag_B31                                   :2;

};

struct RTF_LIST2
{
    UINT8  DashBoard_Flag_B32                   				:2;
    UINT8  Password_Flag_B33                   					:2;
    UINT8  Mobile_Flag_B34                   					:2;
    UINT8  Overlay_Flag_B35                   					:2;
    UINT8  Trends_Flag_B36                   					:2;
    UINT8  Optitune_Flag_B37                   					:2;
    UINT8  Limits_Flag_B38                   					:2;
    UINT8  HDMode_Flag_B39                   					:2;
    UINT8  TeachMode_Flag_B40                   				:2;
    UINT8  AdvComm_Flag_B41                   					:2;
    UINT8  ExtraDataStorage_Flag_B42                   			:2;
    UINT8  TroubleShoot_Suggestion_Flag_B43                   	:2;
    UINT8  CFR_Flag_B44                     					:2;
    UINT8  HourCounter_Flag_B45                					:2;
    /* TnM Feature start */

    UINT8  Reporting_Flag_Csv_B46                               :2;
    UINT8  Reporting_Flag_Pdf_B47                               :2;
    UINT8  IODiagnostics_Flag_B48                               :2;

    /* TnM Feature end */

    UINT8  Dynamic_Flag_B49                                     :2;
    UINT8  Reserved_B50											:2;
    UINT8  Reserved_B51											:2;

};

struct FeatureFlag
{
    RTF_LIST1 RtFeature1;
    RTF_LIST2 RtFeature2;
};

#endif // WELDUITOBL_H
