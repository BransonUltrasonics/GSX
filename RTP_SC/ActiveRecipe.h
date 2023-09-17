/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef ACTIVERECIPE_H_
#define ACTIVERECIPE_H_

#include "WeldRecipe.h"

enum 
{
	SUSPECT_LIMIT = 1,
	REJECT_LIMIT
};

enum
{
	SR_TIME_EN = 1,
	SR_ENERGY_EN,
	SR_PEAKPOWER_EN,
	SR_COLLAPSE_EN,
	SR_ABSOLUTE_EN,
	SR_TRIGGER_EN,
	SR_WELDFORCE_EN,
	SR_FREQUENCY_EN
};

typedef 
enum 
{
	TIME_LIMIT_ML = 1,
	TIME_LIMIT_PL,
	ENERGY_LIMIT_ML,
	ENERGY_LIMIT_PL,
	PEAKPOWER_LIMIT_ML,
	PEAKPOWER_LIMIT_PL,
	COLLAPSE_LIMIT_ML,
	COLLAPSE_LIMIT_PL,
	ABSOLUTE_LIMIT_ML,
	ABSOLUTE_LIMIT_PL,
	TRIGGER_LIMIT_ML,
	TRIGGER_LIMIT_PL,
	WELDFORCE_LIMIT_ML,
	WELDFORCE_LIMIT_PL,
	FREQUENCY_LIMIT_ML,
	FREQUENCY_LIMIT_PL,
}LimitType;

typedef 
enum 
{
	PEAK_POWER_CUTOFF,
	COLLAPSE_DISTANCE_CUTOFF,
	ABSOLUTE_DISTANCE_CUTOFF,
	TIME_CUTOFF,
	FREQUENCY_LOW_CUTOFF,
	FREQUENCY_HIGH_CUTOFF,
	ENERGY_CUTOFF,
	GROUND_DETECT_CUTOFF,
}ControlLimitType;

class ActiveRecipe
{
public:
	ActiveRecipe();
	ActiveRecipe(const ActiveRecipe& srcObj);
	ActiveRecipe& operator=(const ActiveRecipe& srcObj);
	~ActiveRecipe();
	
	UINT32	GetHoldTime() const;
	WELDMODES GetWeldMode() const;
	INT32	GetModeValue() const;
	INT32	GetRecipeNumber() const;
	INT32	GetTriggerForce() const;
	UINT32	GetMaxWeldTimeout() const;
	
	/* Amplitude step interfaces */
	UINT16	InitialAmplitude() const;
	UINT8	GetAmplitudeSteps() const;
	UINT8	GetAmplitudeStepAt() const;
	INT32	GetTriggerDistance() const;
	UINT16	GetAmplitudeStep(UINT8 index) const;
	UINT32	GetAmplitudeStepAtValue(UINT8 index) const;
	
	/* Force step interfaces */
	UINT16	GetForceStepAt() const;
	UINT16	GetNumberOfForceSteps() const;
	UINT32  GetForceStepAtValue(UINT8 index) const;
	
	UINT32  GetScrubAmplitude() const;
	
	Trigger     GetPreTrigger() const;
	Burst		GetAfterBurst() const;
	Brake		GetEnergyBrake() const;
	Dynamic		GetDynamic() const;
	void	    GetUserName(std::string& userName);
	void		SetUserName(char* name);
	UINT32		GetRecipeNumber();
	UINT32		GetRecipeRevNumber();
	void 	    SetActiveRecipe(WeldRecipeSC& recipe);
	UINT32	    GetStackSerialNumber() const;
	void        UpdateForceStep(UINT16 noOfStep, UINT16 stepAt,UINT16 *stepValue);
	bool		IsGlobalLimitCheckEnabled(UINT8 typeOfLimit);
	bool		IsSRCheckEnabled(UINT8 typeOfLimit, UINT8 SRMode);
	bool		IsLimitCheckEnabled(UINT8 typeOfLimit,UINT8 typeOfMode);
	bool		ValidateRejectLimit(UINT8 typeOfMode,UINT32 value);
	bool		ValidateSuspectLimit(UINT8 typeOfMode,UINT32 value);
	UINT32		GetTimedSeek();
	bool 		IsPreWeldSeekEnabled();
	bool 		IsCoolingValveEnabled();
	bool 		IsPostWeldSeekEnabled();
	void		DisplaySuspectLimit();
	void		DisplayRejectLimit();
	void		DisplayForceStep();
	void		RecipeInformation();
	void		DisplayAmplitudeStep();
	void    	DisplayControlLimits();
	void   		DisplayPreTrigger();
	void    	DisplayAfterBurst();
	void		DisplayEnergyBrake();
	bool		IsTimedSeekEnabled();
	void		GetActiveRecipe(WeldRecipeSC& recipe);
	bool        ValidateCutoffLimit(UINT8 typeOfMode, UINT32 value);
	bool        IsCutoffLimitCheckEnabled(UINT8 typeOfMode);
	bool        IsGlobalCutOffLimitCheckEnabled();
	void        DisplaycutoffLimit();
	bool		IsRecipeValidated() const;
	
private:
	WeldRecipeSC 	m_ActiveRecipe;
};

#endif /* ACTIVERECIPE_H_ */
