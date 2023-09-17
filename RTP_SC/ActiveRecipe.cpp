/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Contains active recipe which is edit and set by user in UIC   
 
****************************************************************************/

#include "ActiveRecipe.h"

#define DUMMY_CHANGE_TRIGGER_BUILD 1

using namespace std;

/*
  Dummy change to include Frank I2C Diver Layer (Exclude), Changes and Sam QT Changes
 */

/**************************************************************************//**
* 
* \brief   - Initialization of class members and memory for object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
ActiveRecipe::ActiveRecipe()
{
	/* test 12 */
}

/**************************************************************************//**
* 
* \brief   - Copy constructor - Works as deep copy.
*
* \param   - const ActiveRecipe& srcObj.
*
* \return  - None.
*
******************************************************************************/
ActiveRecipe::ActiveRecipe(const ActiveRecipe& srcObj)
{
	*this = srcObj;
}

/**************************************************************************//**
* 
* \brief   - Assignment.
*
* \param   - const ActiveRecipe& srcObj.
*
* \return  - 	ActiveRecipe&
*
******************************************************************************/
ActiveRecipe& ActiveRecipe::operator=(const ActiveRecipe& srcObj)
{
	m_ActiveRecipe.RecipeNumber		= srcObj.m_ActiveRecipe.RecipeNumber;
	m_ActiveRecipe.RecipeRevNumber	= srcObj.m_ActiveRecipe.RecipeRevNumber;
	m_ActiveRecipe.WeldMode			= srcObj.m_ActiveRecipe.WeldMode;
	m_ActiveRecipe.ModeValue		= srcObj.m_ActiveRecipe.ModeValue;
	m_ActiveRecipe.HoldTime			= srcObj.m_ActiveRecipe.HoldTime;
	m_ActiveRecipe.MaxWeldTimeout	= srcObj.m_ActiveRecipe.MaxWeldTimeout;
	m_ActiveRecipe.TriggerForce		= srcObj.m_ActiveRecipe.TriggerForce;
	m_ActiveRecipe.TriggerDistance	= srcObj.m_ActiveRecipe.TriggerDistance;
	
	m_ActiveRecipe.NumAmplitudeSteps= srcObj.m_ActiveRecipe.NumAmplitudeSteps;
	m_ActiveRecipe.AmplitudeStepAt	= srcObj.m_ActiveRecipe.AmplitudeStepAt;
	m_ActiveRecipe.NumForceSteps	= srcObj.m_ActiveRecipe.NumForceSteps;
	m_ActiveRecipe.ForceStepAt	 	= srcObj.m_ActiveRecipe.ForceStepAt;
	m_ActiveRecipe.SuspectLimit		= srcObj.m_ActiveRecipe.SuspectLimit;
	m_ActiveRecipe.RejectLimit		= srcObj.m_ActiveRecipe.RejectLimit;
	m_ActiveRecipe.CutoffLimit      = srcObj.m_ActiveRecipe.CutoffLimit;
	m_ActiveRecipe.AfterBurst		= srcObj.m_ActiveRecipe.AfterBurst;
	m_ActiveRecipe.PreTrigger		= srcObj.m_ActiveRecipe.PreTrigger;
	m_ActiveRecipe.EnergyBrake		= srcObj.m_ActiveRecipe.EnergyBrake;
	m_ActiveRecipe.bIsSuspectLimitEnabled = srcObj.m_ActiveRecipe.bIsSuspectLimitEnabled;
	m_ActiveRecipe.bIsRejectLimitEnabled  = srcObj.m_ActiveRecipe.bIsRejectLimitEnabled;
	m_ActiveRecipe.bIsCutoffLimitEnabled  = srcObj.m_ActiveRecipe.bIsCutoffLimitEnabled;
	m_ActiveRecipe.bIsCoolingValveEnabled  = srcObj.m_ActiveRecipe.bIsCoolingValveEnabled;
	m_ActiveRecipe.TimedSeekPeriod		  = srcObj.m_ActiveRecipe.TimedSeekPeriod;
	m_ActiveRecipe.bIsTimedSeekEnabled	  = srcObj.m_ActiveRecipe.bIsTimedSeekEnabled;
	m_ActiveRecipe.bIsPostWeldSeekEnabled = srcObj.m_ActiveRecipe.bIsPostWeldSeekEnabled;
	m_ActiveRecipe.bIsPreWeldSeekEnabled  = srcObj.m_ActiveRecipe.bIsPreWeldSeekEnabled;
	m_ActiveRecipe.bIsRecipeValidated	  = srcObj.m_ActiveRecipe.bIsRecipeValidated;
	m_ActiveRecipe.StackSerialNo		  = srcObj.m_ActiveRecipe.StackSerialNo;
	m_ActiveRecipe.DynamicModeParam		  = srcObj.m_ActiveRecipe.DynamicModeParam;
	m_ActiveRecipe.ScrubAmplitude		  = srcObj.m_ActiveRecipe.ScrubAmplitude;
	
	/* Force step at values */
	copy(srcObj.m_ActiveRecipe.ForceStepAtValue, srcObj.m_ActiveRecipe.ForceStepAtValue + sizeof(srcObj.m_ActiveRecipe.ForceStepAtValue)/sizeof(srcObj.m_ActiveRecipe.ForceStepAtValue[0]),m_ActiveRecipe.ForceStepAtValue);
	copy(srcObj.m_ActiveRecipe.AmplitudeStepValue, srcObj.m_ActiveRecipe.AmplitudeStepValue + sizeof(srcObj.m_ActiveRecipe.AmplitudeStepValue)/sizeof(srcObj.m_ActiveRecipe.AmplitudeStepValue[0]),m_ActiveRecipe.AmplitudeStepValue);
	copy(srcObj.m_ActiveRecipe.AmplitudeStep, srcObj.m_ActiveRecipe.AmplitudeStep + sizeof(srcObj.m_ActiveRecipe.AmplitudeStep)/sizeof(srcObj.m_ActiveRecipe.AmplitudeStep[0]),m_ActiveRecipe.AmplitudeStep);
	
	copy(srcObj.m_ActiveRecipe.User, srcObj.m_ActiveRecipe.User + sizeof(srcObj.m_ActiveRecipe.User)/sizeof(srcObj.m_ActiveRecipe.User[0]),m_ActiveRecipe.User);
	
	/* Initial step value - assigned with min value. But, Actual implementation has to be from FORCE RAMP TIME ? */
	m_ActiveRecipe.ForceStepAtValue[0] = 1;
	
	return *this;		/* returns current instance */
}

/**************************************************************************//**
* 
* \brief   - Returns the weld mode of UW system.
*
* \param   - None.
*
* \return  - WELDMODES.
*
******************************************************************************/
WELDMODES ActiveRecipe::GetWeldMode() const
{
	return (WELDMODES)m_ActiveRecipe.WeldMode;
}

/**************************************************************************//**
* 
* \brief   - Returns the weld value of UW system based on WELD.
*
* \param   - None.
*
* \return  - INT32 - ModeValue.
*
******************************************************************************/
INT32 ActiveRecipe::GetModeValue() const
{
	return m_ActiveRecipe.ModeValue;
}

/**************************************************************************//**
* 
* \brief   - Returns the hold time which will be applied in weld.
*
* \param   - None.
*
* \return  - UINT32 - HoldTime.
*
******************************************************************************/
UINT32 ActiveRecipe::GetHoldTime() const
{
	return m_ActiveRecipe.HoldTime;
}

/**************************************************************************//**
* 
* \brief   - Returns the active recipe number which is used for weld.
*
* \param   - None.
*
* \return  - INT32 - RecipeNumber.
*
******************************************************************************/
INT32 ActiveRecipe::GetRecipeNumber() const
{
	return m_ActiveRecipe.RecipeNumber;
}

/**************************************************************************//**
* 
* \brief   - Returns the trigger force which will be reference to SC.
*
* \param   - None.
*
* \return  - INT32 - TriggerForce.
*
******************************************************************************/
INT32 ActiveRecipe::GetTriggerForce() const
{
	return m_ActiveRecipe.TriggerForce;
}

/**************************************************************************//**
* 
* \brief   - Returns amplitude base value.
*
* \param   - None.
*
* \return  - UINT16 - AmplitudeStep.
*
******************************************************************************/
UINT16 ActiveRecipe::InitialAmplitude() const
{
	return m_ActiveRecipe.AmplitudeStep[0];			/* Default amplitude used during before seek */
}

/**************************************************************************//**
* 
* \brief   - Returns number of amplitude step configured.
*
* \param   - None.
*
* \return  - UINT8 - NumAmplitudeSteps.
*
******************************************************************************/
UINT8 ActiveRecipe::GetAmplitudeSteps() const
{
	return m_ActiveRecipe.NumAmplitudeSteps;
}

/**************************************************************************//**
* 
* \brief   - Returns amplitude step at position.
*
* \param   - None.
*
* \return  - UINT8 - StepAt.
*
******************************************************************************/
UINT8 ActiveRecipe::GetAmplitudeStepAt() const
{
	return m_ActiveRecipe.AmplitudeStepAt;
}

/**************************************************************************//**
* 
* \brief   - Returns the trigger distance - reference to SC -WELD.
*
* \param   - None.
*
* \return  - INT32 - TriggerDistance.
*
******************************************************************************/
INT32 ActiveRecipe::GetTriggerDistance() const
{
	return m_ActiveRecipe.TriggerDistance;
}

/**************************************************************************//**
* \brief   - Returns the max weld timeout- reference to SC -WELD
*
* \param   - None
*
* \return  - UINT32 - MaxWeldTimeout
*
******************************************************************************/
UINT32 ActiveRecipe::GetMaxWeldTimeout() const
{
	return m_ActiveRecipe.MaxWeldTimeout;
}

/**************************************************************************//**
* 
* \brief   - Returns amplitude step value present at index.
*
* \param   - UINT8 - index.
*
* \return  - UINT32 - AmplitudeStepValue.
*
******************************************************************************/
UINT32 ActiveRecipe::GetAmplitudeStepAtValue(UINT8 index) const
{
	UINT16 stepValue = 0;
	if(index < m_ActiveRecipe.NumAmplitudeSteps)
	{
		stepValue = m_ActiveRecipe.AmplitudeStepValue[index];
	}
	
	return stepValue;
}

/**************************************************************************//**
* 
* \brief   - Returns amplitude step present at index.
*
* \param   - UINT8 - index.
*
* \return  - UINT16 - AmplitudeStep.
*
******************************************************************************/
UINT16 ActiveRecipe::GetAmplitudeStep(UINT8 index) const
{
	UINT16 step = 0;
	if(index < m_ActiveRecipe.NumAmplitudeSteps)
	{
		step = m_ActiveRecipe.AmplitudeStep[index];
	}

	return step;
}

/**************************************************************************//**
* 
* \brief   - Returns the number of force step.
*
* \param   - None.
*
* \return  - UINT16 - m_NumOfForceStep.
*
******************************************************************************/
UINT16 ActiveRecipe::GetNumberOfForceSteps() const
{
	return m_ActiveRecipe.NumForceSteps;
}

/**************************************************************************//**
* 
* \brief   - Returns the step at mode of force.
*
* \param   - None.
*
* \return  - UINT16 - m_ForceStepAt.
*
******************************************************************************/
UINT16 ActiveRecipe::GetForceStepAt() const
{
	return m_ActiveRecipe.ForceStepAt;
}

/**************************************************************************//**
* 
* \brief   - Returns the force step value.
*
* \param   - UINT8 index.
*
* \return  - UINT32 - stepValue.
*
******************************************************************************/
UINT32 ActiveRecipe::GetForceStepAtValue(UINT8 index) const
{
	UINT16 stepValue = 0;
	
	if(index < m_ActiveRecipe.NumForceSteps)
	{
		stepValue = m_ActiveRecipe.ForceStepAtValue[index]; 
	}
	return stepValue;
}

/**************************************************************************//**
* 
* \brief   - Returns the Scrub Amplitude value.
*
* \param   - None
*
* \return  - UINT32 - ScrubAmplitude.
*
******************************************************************************/
UINT32 ActiveRecipe::GetScrubAmplitude() const
{
	return m_ActiveRecipe.ScrubAmplitude;
}

/**************************************************************************//**
* 
* \brief   - Updates the enabled force step and step values.
*
* \param   - UINT16 noOfStep, UINT16 stepAt ,UINT16 &stepValue.
*
* \return  - None.
*
******************************************************************************/
void ActiveRecipe::UpdateForceStep(UINT16 noOfStep, UINT16 stepAt,UINT16 *stepValue)
{
	m_ActiveRecipe.NumForceSteps= noOfStep;
	m_ActiveRecipe.ForceStepAt	= stepAt;
	
	for(UINT8 idx = 0; idx < MAX_NO_OF_FORCE_STEP; idx++)
	{
		m_ActiveRecipe.ForceStepAtValue[idx] = stepValue[idx];
	}
}

/**************************************************************************//**
* 
* \brief   - Returns active user name of UW system.
*
* \param   - string& - userName.
*
* \return  - None.
*
******************************************************************************/
void ActiveRecipe::GetUserName(std::string& userName)
{
	strcpy((char *)userName.c_str(),m_ActiveRecipe.User);
}

/**************************************************************************//**
* 
* \brief   - Sets active user name of UW system.
*
* \param   - char* - userName.
*
* \return  - None.
*
******************************************************************************/
void ActiveRecipe::SetUserName(char* name)
{
	strncpy(m_ActiveRecipe.User, name, sizeof(m_ActiveRecipe.User));
}

/**************************************************************************//**
* 
* \brief   - Gets Active recipe number.
*
* \param   - None.
*
* \return  - UINT32.
*
******************************************************************************/
UINT32 ActiveRecipe::GetRecipeNumber()
{
	return m_ActiveRecipe.RecipeNumber;
}

/**************************************************************************//**
* 
* \brief   - Gets Active recipe version number.
*
* \param   - None.
*
* \return  - UINT32.
*
******************************************************************************/
UINT32 ActiveRecipe::GetRecipeRevNumber()
{
	return m_ActiveRecipe.RecipeRevNumber;
}

/**************************************************************************//**
* 
* \brief   - Returns stack serial number of UW system.
*
* \param   - None.
*
* \return  - UINT32 stackSerialNumber.
*
******************************************************************************/
UINT32 ActiveRecipe::GetStackSerialNumber() const
{
	return m_ActiveRecipe.StackSerialNo;
}

/**************************************************************************//**
* \brief   - Returns active weld recipe structure of SC.
*
* \param   - WeldRecipeSC& recipe (structure of object of weld recipe SC)
*
* \return  - None.
*
******************************************************************************/
void ActiveRecipe::GetActiveRecipe(WeldRecipeSC& recipe)
{
	memcpy(&recipe, &m_ActiveRecipe,sizeof(m_ActiveRecipe));
}

/**************************************************************************//**
* \brief   - Updates SC weld recipe parameters with latest data which is 
* 			 modified by user from UIC.
*
* \param   - WeldRecipeSC& recipe (structure of object of weld recipe SC)
*
* \return  - None
*
******************************************************************************/
void ActiveRecipe::SetActiveRecipe(WeldRecipeSC& recipe)
{
	memcpy(&m_ActiveRecipe,&recipe,sizeof(m_ActiveRecipe));
}

/**************************************************************************//**
* \brief   - Returns the cooling valve seek status as enabled or disabled
*
* \param   - None
*
* \return  - bool (true /false )
*
******************************************************************************/
bool ActiveRecipe::IsCoolingValveEnabled()
{
	return m_ActiveRecipe.bIsCoolingValveEnabled;
}

/**************************************************************************//**
* \brief   - Returns the Preweld seek status as enabled or disabled
*
* \param   - None
*
* \return  - bool (true /false )
*
******************************************************************************/
bool ActiveRecipe::IsPreWeldSeekEnabled()
{
	return m_ActiveRecipe.bIsPreWeldSeekEnabled;
}


/**************************************************************************//**
* \brief   - Returns the Postweld seek status as enabled or disabled
*
* \param   - None
*
* \return  - bool (true /false )
*
******************************************************************************/
bool ActiveRecipe::IsPostWeldSeekEnabled()
{
	return m_ActiveRecipe.bIsPostWeldSeekEnabled;
}

/**************************************************************************//**
* \brief   - Returns the Timed seek value if enabled
*
* \param   - None
*
* \return  - UINT32 
*
******************************************************************************/
UINT32 ActiveRecipe::GetTimedSeek()
{
	UINT32 TimedSeekValue=0;
	
	if(m_ActiveRecipe.bIsTimedSeekEnabled)
	{
		TimedSeekValue=m_ActiveRecipe.TimedSeekPeriod;
	}
	
	return TimedSeekValue;
}

/**************************************************************************//**
* \brief   - Returns the Timed seeked is enabled or not
*
* \param   - None
*
* \return  - bool(true / fail) 
*
******************************************************************************/
bool ActiveRecipe::IsTimedSeekEnabled()
{
	return m_ActiveRecipe.bIsTimedSeekEnabled;
}

/**************************************************************************//**
* \brief   - Validates the suspect and reject limits are global option 
* 			 enabled or not.
*
* \param   - UINT8 typeOfLimit (Suspect or Reject)
*
* \return  - bool - TRUE on enable
*
******************************************************************************/
bool ActiveRecipe::IsGlobalLimitCheckEnabled(UINT8 typeOfLimit)
{
	bool bIsEnabled = false;
	
	if(SUSPECT_LIMIT == typeOfLimit)
	{
		bIsEnabled = m_ActiveRecipe.bIsSuspectLimitEnabled;
	}
	else if(REJECT_LIMIT == typeOfLimit)
	{
		bIsEnabled = m_ActiveRecipe.bIsRejectLimitEnabled;
	}
	
	return bIsEnabled;
}

/**************************************************************************//**
* \brief   - Check the SR mode enable status
*
* \param   - UINT8 typeOfLimit(Suspect or Reject), UINT8 SRMode
*
* \return  - bool bIsEnabled (TRUE or FALSE)
*
******************************************************************************/
bool ActiveRecipe::IsSRCheckEnabled(UINT8 typeOfLimit, UINT8 SRMode)
{
	bool bIsEnabled = false;
	
	if(SUSPECT_LIMIT == typeOfLimit)
	{
		switch(SRMode)
		{
		case SR_TIME_EN:
			bIsEnabled = m_ActiveRecipe.SuspectLimit.Time.bIsEnabled;
			break;
		case SR_ENERGY_EN:
			bIsEnabled = m_ActiveRecipe.SuspectLimit.Energy.bIsEnabled;
			break;
		case SR_PEAKPOWER_EN:
			bIsEnabled = m_ActiveRecipe.SuspectLimit.PeakPower.bIsEnabled;
			break;
		case SR_COLLAPSE_EN:
			bIsEnabled = m_ActiveRecipe.SuspectLimit.CollapseDistance.bIsEnabled;
			break;
		case SR_ABSOLUTE_EN:
			bIsEnabled = m_ActiveRecipe.SuspectLimit.AbsoluteDistance.bIsEnabled;
			break;
		case SR_TRIGGER_EN:
			bIsEnabled = m_ActiveRecipe.SuspectLimit.TriggerDistance.bIsEnabled;
			break;
		case SR_WELDFORCE_EN:
			bIsEnabled = m_ActiveRecipe.SuspectLimit.WeldForce.bIsEnabled;
			break;
		case SR_FREQUENCY_EN:
			bIsEnabled = m_ActiveRecipe.SuspectLimit.Frequency.bIsEnabled;
			break;
		}
	}
	else if(REJECT_LIMIT == typeOfLimit)
	{
		switch(SRMode)
		{
		case SR_TIME_EN:
			bIsEnabled = m_ActiveRecipe.RejectLimit.Time.bIsEnabled;
			break;
		case SR_ENERGY_EN:
			bIsEnabled = m_ActiveRecipe.RejectLimit.Energy.bIsEnabled;
			break;
		case SR_PEAKPOWER_EN:
			bIsEnabled = m_ActiveRecipe.RejectLimit.PeakPower.bIsEnabled;
			break;
		case SR_COLLAPSE_EN:
			bIsEnabled = m_ActiveRecipe.RejectLimit.CollapseDistance.bIsEnabled;
			break;
		case SR_ABSOLUTE_EN:
			bIsEnabled = m_ActiveRecipe.RejectLimit.AbsoluteDistance.bIsEnabled;
			break;
		case SR_TRIGGER_EN:
			bIsEnabled = m_ActiveRecipe.RejectLimit.TriggerDistance.bIsEnabled;
			break;
		case SR_WELDFORCE_EN:
			bIsEnabled = m_ActiveRecipe.RejectLimit.WeldForce.bIsEnabled;
			break;
		case SR_FREQUENCY_EN:
			bIsEnabled = m_ActiveRecipe.RejectLimit.Frequency.bIsEnabled;
			break;
		}
	}
	
	return bIsEnabled;
}

/**************************************************************************//**
* \brief   - Checks , Is suspect and reject limits validation enabled
*      		 or not against all the modes
*
* \param   - UINT8 typeOfLimit(Suspect or Reject), UINT8 typeOfMode
*
* \return  - bool bIsEnabled (TRUE or FALSE)
*
******************************************************************************/
bool ActiveRecipe::IsLimitCheckEnabled(UINT8 typeOfLimit,UINT8 typeOfMode)
{
	bool bIsEnabled = false;

	if(SUSPECT_LIMIT == typeOfLimit)
	{
		switch(typeOfMode)
		{
			case TIME_LIMIT_ML:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.Time.bIsLowEnabled;
				break;
			case TIME_LIMIT_PL:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.Time.bIsHighEnabled;
				break;
			case ENERGY_LIMIT_ML:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.Energy.bIsLowEnabled;
				break;
			case ENERGY_LIMIT_PL:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.Energy.bIsHighEnabled;
				break;
			case PEAKPOWER_LIMIT_ML:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.PeakPower.bIsLowEnabled;
				break;
			case PEAKPOWER_LIMIT_PL:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.PeakPower.bIsHighEnabled;
				break;	
			case COLLAPSE_LIMIT_ML:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.CollapseDistance.bIsLowEnabled;
				break;
			case COLLAPSE_LIMIT_PL:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.CollapseDistance.bIsHighEnabled;
				break;	
			case ABSOLUTE_LIMIT_ML:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.AbsoluteDistance.bIsLowEnabled;
				break;
			case ABSOLUTE_LIMIT_PL:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.AbsoluteDistance.bIsHighEnabled;
				break;	
			case TRIGGER_LIMIT_ML:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.TriggerDistance.bIsLowEnabled;
				break;
			case TRIGGER_LIMIT_PL:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.TriggerDistance.bIsHighEnabled;
				break;	
			case WELDFORCE_LIMIT_ML:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.WeldForce.bIsLowEnabled;
				break;
			case WELDFORCE_LIMIT_PL:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.WeldForce.bIsHighEnabled;
				break;
			case FREQUENCY_LIMIT_ML:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.Frequency.bIsLowEnabled;
				break;	
			case FREQUENCY_LIMIT_PL:
				bIsEnabled = m_ActiveRecipe.SuspectLimit.Frequency.bIsHighEnabled;
				break;
			default:
				LOGERR("Invalid Limit validation : %d",typeOfMode,0,0);
				break;
		}
	}
	else if(REJECT_LIMIT == typeOfLimit)
	{
		switch(typeOfMode)
		{
		case TIME_LIMIT_ML:
			bIsEnabled = m_ActiveRecipe.RejectLimit.Time.bIsLowEnabled;
			break;
		case TIME_LIMIT_PL:
			bIsEnabled = m_ActiveRecipe.RejectLimit.Time.bIsHighEnabled;
			break;
		case ENERGY_LIMIT_ML:
			bIsEnabled = m_ActiveRecipe.RejectLimit.Energy.bIsLowEnabled;
			break;
		case ENERGY_LIMIT_PL:
			bIsEnabled = m_ActiveRecipe.RejectLimit.Energy.bIsHighEnabled;
			break;
		case PEAKPOWER_LIMIT_ML:
			bIsEnabled = m_ActiveRecipe.RejectLimit.PeakPower.bIsLowEnabled;
			break;
		case PEAKPOWER_LIMIT_PL:
			bIsEnabled = m_ActiveRecipe.RejectLimit.PeakPower.bIsHighEnabled;
			break;
		case COLLAPSE_LIMIT_ML:
			bIsEnabled = m_ActiveRecipe.RejectLimit.CollapseDistance.bIsLowEnabled;
			break;
		case COLLAPSE_LIMIT_PL:
			bIsEnabled = m_ActiveRecipe.RejectLimit.CollapseDistance.bIsHighEnabled;
			break;
		case ABSOLUTE_LIMIT_ML:
			bIsEnabled = m_ActiveRecipe.RejectLimit.AbsoluteDistance.bIsLowEnabled;
			break;
		case ABSOLUTE_LIMIT_PL:
			bIsEnabled = m_ActiveRecipe.RejectLimit.AbsoluteDistance.bIsHighEnabled;
			break;
		case TRIGGER_LIMIT_ML:
			bIsEnabled = m_ActiveRecipe.RejectLimit.TriggerDistance.bIsLowEnabled;
			break;
		case TRIGGER_LIMIT_PL:
			bIsEnabled = m_ActiveRecipe.RejectLimit.TriggerDistance.bIsHighEnabled;
			break;
		case WELDFORCE_LIMIT_ML:
			bIsEnabled = m_ActiveRecipe.RejectLimit.WeldForce.bIsLowEnabled;
			break;
		case WELDFORCE_LIMIT_PL:
			bIsEnabled = m_ActiveRecipe.RejectLimit.WeldForce.bIsHighEnabled;
			break;
		case FREQUENCY_LIMIT_ML:
			bIsEnabled = m_ActiveRecipe.RejectLimit.Frequency.bIsLowEnabled;
			break;
		case FREQUENCY_LIMIT_PL:
			bIsEnabled = m_ActiveRecipe.RejectLimit.Frequency.bIsHighEnabled;
			break;
		default:
			LOGERR("Invalid Limit validation : %d",typeOfMode,0,0);
			break;
		}
	}

	return bIsEnabled;
}

/**************************************************************************//**
* \brief   - Validates reject low and high limits based on input passed.
*
* \param   - UINT8 typeOfMode, UINT32 value - value to be check
*
* \return  - bool bIsValid (TRUE or FALSE)
*
******************************************************************************/
bool ActiveRecipe::ValidateRejectLimit(UINT8 typeOfMode,UINT32 value)
{
	bool bIsValid = false;

	switch(typeOfMode)
	{
	case TIME_LIMIT_ML:
		if(value < m_ActiveRecipe.RejectLimit.Time.LowLimit) 
		{
			bIsValid = true;
		}
		break;
	case TIME_LIMIT_PL:
		if(value > m_ActiveRecipe.RejectLimit.Time.HighLimit)
		{
			bIsValid = true;
		}
       break;
	case ENERGY_LIMIT_ML:
		if(value < m_ActiveRecipe.RejectLimit.Energy.LowLimit) 
		{
			bIsValid = true; 
		}
		break;
	case ENERGY_LIMIT_PL:
		if(value > m_ActiveRecipe.RejectLimit.Energy.HighLimit)
		{
			bIsValid = true;
		}
        break;
	case PEAKPOWER_LIMIT_ML:
		if(value < m_ActiveRecipe.RejectLimit.PeakPower.LowLimit)
		{
			bIsValid = true;
		}
		break;
	case PEAKPOWER_LIMIT_PL:
		if (value > m_ActiveRecipe.RejectLimit.PeakPower.HighLimit)
		{
		   bIsValid =  true;	
		}
		break;
	case COLLAPSE_LIMIT_ML:
		if(value < m_ActiveRecipe.RejectLimit.CollapseDistance.LowLimit) 
		{
			bIsValid = true;
		}
		break;
	case COLLAPSE_LIMIT_PL:
		if(value > m_ActiveRecipe.RejectLimit.CollapseDistance.HighLimit)
		{
			bIsValid = true;	
		}
		break;
	case ABSOLUTE_LIMIT_ML:
		if(value < m_ActiveRecipe.RejectLimit.AbsoluteDistance.LowLimit)
		{
			bIsValid = true;
		}
		break;
	case ABSOLUTE_LIMIT_PL:
		if(value > m_ActiveRecipe.RejectLimit.AbsoluteDistance.HighLimit)
		{
			bIsValid = true;
		}
		break;
	case TRIGGER_LIMIT_ML:
		if(value < m_ActiveRecipe.RejectLimit.TriggerDistance.LowLimit) 
		{
			bIsValid = true;
		}
		break;
	case TRIGGER_LIMIT_PL:
		if(value > m_ActiveRecipe.RejectLimit.TriggerDistance.HighLimit)
		{
			bIsValid = true;
		}
	   break;
	case WELDFORCE_LIMIT_ML:
		if(value < m_ActiveRecipe.RejectLimit.WeldForce.LowLimit) 
		{
			bIsValid = true;
		}
		break; 
	case WELDFORCE_LIMIT_PL:
		if(value>m_ActiveRecipe.RejectLimit.WeldForce.HighLimit)
		{
			bIsValid = true;
		}
		break;
	case FREQUENCY_LIMIT_ML:
		if(value < m_ActiveRecipe.RejectLimit.Frequency.LowLimit*10)
		{
			bIsValid = true;
		}
		break;
	case FREQUENCY_LIMIT_PL:
	    if(value >  m_ActiveRecipe.RejectLimit.Frequency.HighLimit*10)
	    {
	    	bIsValid = true;
	    }
	    break;
	default:
		LOGERR("Invalid Limit validation : %d",typeOfMode,0,0);
		break;
	}
	
	if(bIsValid)
	{
		bRejectStatus = true;
	}
	
	return bIsValid;
}

/**************************************************************************//**
* \brief   - Validates suspect low and high limits based on input passed.
*
* \param   - UINT8 typeOfMode, UINT32 value - value to be check
*
* \return  - bool bIsValid (TRUE or FALSE)
*
******************************************************************************/
bool ActiveRecipe::ValidateSuspectLimit(UINT8 typeOfMode,UINT32 value)
{
	bool bIsValid = false;
	
	switch(typeOfMode)
	{
		case TIME_LIMIT_ML:
			if(value < m_ActiveRecipe.SuspectLimit.Time.LowLimit) 
			{
				bIsValid = true;
			}
			break;
		case TIME_LIMIT_PL:
			if(value > m_ActiveRecipe.SuspectLimit.Time.HighLimit)
			{
				bIsValid = true;
			}
	       break;
		case ENERGY_LIMIT_ML:
			if(value < m_ActiveRecipe.SuspectLimit.Energy.LowLimit) 
			{
				bIsValid = true; 
			}
			break;
		case ENERGY_LIMIT_PL:
			if(value > m_ActiveRecipe.SuspectLimit.Energy.HighLimit)
			{
				bIsValid = true;
			}
	        break;
		case PEAKPOWER_LIMIT_ML:
			if(value < m_ActiveRecipe.SuspectLimit.PeakPower.LowLimit)
			{
				bIsValid = true;
			}
			break;
		case PEAKPOWER_LIMIT_PL:
			if (value > m_ActiveRecipe.SuspectLimit.PeakPower.HighLimit)
			{
			   bIsValid =  true;	
			}
			break;
		case COLLAPSE_LIMIT_ML:
			if(value < m_ActiveRecipe.SuspectLimit.CollapseDistance.LowLimit) 
			{
				bIsValid = true;
			}
			break;
		case COLLAPSE_LIMIT_PL:
			if(value > m_ActiveRecipe.SuspectLimit.CollapseDistance.HighLimit)
			{
				bIsValid = true;
			}
			break;
		case ABSOLUTE_LIMIT_ML:
			if(value < m_ActiveRecipe.SuspectLimit.AbsoluteDistance.LowLimit)
			{
				bIsValid = true;
			}
			break;
		case ABSOLUTE_LIMIT_PL:
			if(value > m_ActiveRecipe.SuspectLimit.AbsoluteDistance.HighLimit)
			{
				bIsValid = true;
			}
			break;
		case TRIGGER_LIMIT_ML:
			if(value < m_ActiveRecipe.SuspectLimit.TriggerDistance.LowLimit) 
			{
				bIsValid = true;
			}
			break;
		case TRIGGER_LIMIT_PL:
			if(value > m_ActiveRecipe.SuspectLimit.TriggerDistance.HighLimit)
			{
				bIsValid = true;
			}
		   break;
		case WELDFORCE_LIMIT_ML:
			if(value < m_ActiveRecipe.SuspectLimit.WeldForce.LowLimit) 
			{
				bIsValid = true;
			}
			break; 
		case WELDFORCE_LIMIT_PL:
			if(value>m_ActiveRecipe.SuspectLimit.WeldForce.HighLimit)
			{
				bIsValid = true;
			}
			break;
		case FREQUENCY_LIMIT_ML:
			if(value < m_ActiveRecipe.SuspectLimit.Frequency.LowLimit*10)
			{
				bIsValid = true;
			}
			break;
		case FREQUENCY_LIMIT_PL:
		    if(value >  m_ActiveRecipe.SuspectLimit.Frequency.HighLimit*10)
		    {
		    	bIsValid = true;
		    }
		    break;
		default:
			LOGERR("Invalid Limit validation : %d",typeOfMode,0,0);
			break;
	}
	
	if(bIsValid)
	{
		bSuspectStatus = true;
	}
		
	return bIsValid;
}


/**************************************************************************//**
* \brief   - return Afterburst parameters of Weldrecipe
*
* \param   - None
*
* \return  - AfterBurst
*
******************************************************************************/
Burst ActiveRecipe::GetAfterBurst() const
{
	return m_ActiveRecipe.AfterBurst;
}

/**************************************************************************//**
* \brief   - return PreTrigger parameters of Weldrecipe
*
* \param   - None
*
* \return  - PreTrigger
*
******************************************************************************/
Trigger ActiveRecipe::GetPreTrigger() const
{
	return m_ActiveRecipe.PreTrigger;
}

/**************************************************************************//**
* \brief   - return EnergyBrake parameters of Weldrecipe
*
* \param   - None
*
* \return  - EnergyBrake
*
******************************************************************************/
Brake ActiveRecipe::GetEnergyBrake() const
{
	return m_ActiveRecipe.EnergyBrake;
}

/**************************************************************************//**
* \brief   - return Dynamic parameters of Weldrecipe
*
* \param   - None
*
* \return  - Dynamic parameters
*
******************************************************************************/
Dynamic ActiveRecipe::GetDynamic() const
{
	return m_ActiveRecipe.DynamicModeParam;
}

/**************************************************************************//**
* \brief   - Display suspect limits, Only when flag is enabled.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ActiveRecipe::DisplaySuspectLimit()
{
	if(IsGlobalLimitCheckEnabled(SUSPECT_LIMIT))
	{
		if(IsLimitCheckEnabled(SUSPECT_LIMIT,TIME_LIMIT_ML))
		{
			LOGDBG("SuspectLimit.Time.IsEnabled				: %d",m_ActiveRecipe.SuspectLimit.Time.bIsLowEnabled,0,0);
			LOGDBG("SuspectLimit.Time.Low 					: %d",m_ActiveRecipe.SuspectLimit.Time.LowLimit,0,0);
		}

		if(IsLimitCheckEnabled(SUSPECT_LIMIT,TIME_LIMIT_PL))
		{
			LOGDBG("SuspectLimit.Time.IsEnabled				: %d",m_ActiveRecipe.SuspectLimit.Time.bIsHighEnabled,0,0);
			LOGDBG("SuspectLimit.Time.High 					: %d",m_ActiveRecipe.SuspectLimit.Time.HighLimit,0,0);
		}
		if(IsLimitCheckEnabled(SUSPECT_LIMIT,ENERGY_LIMIT_ML))
		{
			LOGDBG("SuspectLimit.Energy.IsEnabled 			: %d",m_ActiveRecipe.SuspectLimit.Energy.bIsLowEnabled,0,0);
			LOGDBG("SuspectLimit.Energy.Low 				: %d",m_ActiveRecipe.SuspectLimit.Energy.LowLimit,0,0);
		}
		if(IsLimitCheckEnabled(SUSPECT_LIMIT,ENERGY_LIMIT_PL))
		{
			LOGDBG("SuspectLimit.Energy.IsEnabled 			: %d",m_ActiveRecipe.SuspectLimit.Energy.bIsHighEnabled,0,0);
			LOGDBG("SuspectLimit.Energy.High 				: %d",m_ActiveRecipe.SuspectLimit.Energy.HighLimit,0,0);
		}

		if(IsLimitCheckEnabled(SUSPECT_LIMIT,PEAKPOWER_LIMIT_ML))
		{
			LOGDBG(" SuspectLimit.PeakPower.IsEnabled 		: %d",m_ActiveRecipe.SuspectLimit.PeakPower.bIsLowEnabled,0,0);
			LOGDBG("SuspectLimit.PeakPower.Low				: %d",m_ActiveRecipe.SuspectLimit.PeakPower.LowLimit,0,0);	  
		}
		
		if(IsLimitCheckEnabled(SUSPECT_LIMIT,PEAKPOWER_LIMIT_PL))
		{
			LOGDBG(" SuspectLimit.PeakPower.IsEnabled 		: %d",m_ActiveRecipe.SuspectLimit.PeakPower.bIsHighEnabled,0,0);	  
			LOGDBG("SuspectLimit.PeakPower.High 			: %d",m_ActiveRecipe.SuspectLimit.PeakPower.HighLimit,0,0);
		}

		if(IsLimitCheckEnabled(SUSPECT_LIMIT,COLLAPSE_LIMIT_ML))
		{
			LOGDBG("SuspectLimit.CollapseDistance.IsEnabled	: %d",m_ActiveRecipe.SuspectLimit.CollapseDistance.bIsLowEnabled,0,0);
			LOGDBG("SuspectLimit.CollapseDistance.Low		: %d",m_ActiveRecipe.SuspectLimit.CollapseDistance.LowLimit,0,0);
			LOGDBG("SuspectLimit.CollapseDistance.High		: %d",m_ActiveRecipe.SuspectLimit.CollapseDistance.HighLimit,0,0);
		}
		
		if(IsLimitCheckEnabled(SUSPECT_LIMIT,COLLAPSE_LIMIT_PL))
		{
			LOGDBG("SuspectLimit.CollapseDistance.IsEnabled	: %d",m_ActiveRecipe.SuspectLimit.CollapseDistance.bIsHighEnabled,0,0);
			LOGDBG("SuspectLimit.CollapseDistance.High		: %d",m_ActiveRecipe.SuspectLimit.CollapseDistance.HighLimit,0,0);
		}

		if(IsLimitCheckEnabled(SUSPECT_LIMIT,ABSOLUTE_LIMIT_ML))
		{
			LOGDBG("SuspectLimit.AbsoluteDistance.IsEnabled : %d",m_ActiveRecipe.SuspectLimit.AbsoluteDistance.bIsLowEnabled,0,0);
			LOGDBG("SuspectLimit.AbsoluteDistance.Low 		: %d",m_ActiveRecipe.SuspectLimit.AbsoluteDistance.LowLimit,0,0);
		}
		
		if(IsLimitCheckEnabled(SUSPECT_LIMIT,ABSOLUTE_LIMIT_PL))
		{
			LOGDBG("SuspectLimit.AbsoluteDistance.IsEnabled : %d",m_ActiveRecipe.SuspectLimit.AbsoluteDistance.bIsHighEnabled,0,0);
			LOGDBG("SuspectLimit.AbsoluteDistance.High 		: %d",m_ActiveRecipe.SuspectLimit.AbsoluteDistance.HighLimit,0,0);
		}

		if(IsLimitCheckEnabled(SUSPECT_LIMIT,TRIGGER_LIMIT_ML))
		{
			LOGDBG(" SuspectLimit.TriggerDistance.IsEnabled : %d",m_ActiveRecipe.SuspectLimit.TriggerDistance.bIsLowEnabled,0,0);
			LOGDBG(" SuspectLimit.TriggerDistance.Low 		: %d", m_ActiveRecipe.SuspectLimit.TriggerDistance.LowLimit,0,0);
		}
		
		if(IsLimitCheckEnabled(SUSPECT_LIMIT,TRIGGER_LIMIT_PL))
		{
			LOGDBG(" SuspectLimit.TriggerDistance.IsEnabled : %d",m_ActiveRecipe.SuspectLimit.TriggerDistance.bIsHighEnabled,0,0);
			LOGDBG(" SuspectLimit.TriggerDistance.High 		: %d", m_ActiveRecipe.SuspectLimit.TriggerDistance.HighLimit,0,0);
		}

		if(IsLimitCheckEnabled(SUSPECT_LIMIT,WELDFORCE_LIMIT_ML))
		{
			LOGDBG(" SuspectLimit.WeldForce.IsEnabled		: %d",m_ActiveRecipe.SuspectLimit.WeldForce.bIsLowEnabled,0,0);
			LOGDBG(" SuspectLimit.WeldForce.Low				: %d",m_ActiveRecipe.SuspectLimit.WeldForce.LowLimit,0,0);
		}

		if(IsLimitCheckEnabled(SUSPECT_LIMIT,WELDFORCE_LIMIT_PL))
		{
			LOGDBG(" SuspectLimit.WeldForce.IsEnabled		: %d",m_ActiveRecipe.SuspectLimit.WeldForce.bIsHighEnabled,0,0);
			LOGDBG(" SuspectLimit.WeldForce.High			: %d",m_ActiveRecipe.SuspectLimit.WeldForce.HighLimit,0,0);
		}
		
		if(IsLimitCheckEnabled(SUSPECT_LIMIT,FREQUENCY_LIMIT_ML))
		{
			LOGDBG(" SuspectLimit.Frequency.IsEnabled		: %d",m_ActiveRecipe.SuspectLimit.Frequency.bIsLowEnabled,0,0);
			LOGDBG(" SuspectLimit.Frequency.Low				: %d",m_ActiveRecipe.SuspectLimit.Frequency.LowLimit,0,0);
		}
		if(IsLimitCheckEnabled(SUSPECT_LIMIT,FREQUENCY_LIMIT_PL))
		{
			LOGDBG(" SuspectLimit.Frequency.IsEnabled		: %d",m_ActiveRecipe.SuspectLimit.Frequency.bIsHighEnabled,0,0);
			LOGDBG(" SuspectLimit.Frequency.High			: %d",m_ActiveRecipe.SuspectLimit.Frequency.HighLimit,0,0);
		}
	}
	else
	{
		LOGDBG("Suspect limit disabled",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Display reject limits, Only when flag is enabled.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ActiveRecipe::DisplayRejectLimit()
{
	if(IsGlobalLimitCheckEnabled(REJECT_LIMIT))
	{
		if(IsLimitCheckEnabled(REJECT_LIMIT,TIME_LIMIT_ML))
		{
			LOGDBG("RejectLimit.Time.IsEnabled 				: %d",m_ActiveRecipe.RejectLimit.Time.bIsLowEnabled,0,0);
			LOGDBG("RejectLimit.Time.Low 	 				: %d",m_ActiveRecipe.RejectLimit.Time.LowLimit,0,0);
		}
		
		if(IsLimitCheckEnabled(REJECT_LIMIT,TIME_LIMIT_PL))
		{
			LOGDBG("RejectLimit.Time.IsEnabled 				: %d",m_ActiveRecipe.RejectLimit.Time.bIsHighEnabled,0,0);
			LOGDBG("RejectLimit.Time.High 					: %d",m_ActiveRecipe.RejectLimit.Time.HighLimit,0,0);
		}

		if(IsLimitCheckEnabled(REJECT_LIMIT,ENERGY_LIMIT_ML))
		{
			LOGDBG("RejectLimit.Energy.IsEnabled			: %d",m_ActiveRecipe.RejectLimit.Energy.bIsLowEnabled,0,0);
			LOGDBG("RejectLimit.Energy.Low 					: %d",m_ActiveRecipe.RejectLimit.Energy.LowLimit,0,0);
		}
		
		if(IsLimitCheckEnabled(REJECT_LIMIT,ENERGY_LIMIT_PL))
		{
			LOGDBG("RejectLimit.Energy.IsEnabled			: %d",m_ActiveRecipe.RejectLimit.Energy.bIsHighEnabled,0,0);
			LOGDBG("RejectLimit.Energy.High 				: %d",m_ActiveRecipe.RejectLimit.Energy.HighLimit,0,0);
		}

		if(IsLimitCheckEnabled(REJECT_LIMIT,PEAKPOWER_LIMIT_ML))
		{
			LOGDBG(" RejectLimit.PeakPower.IsEnabled		: %d",m_ActiveRecipe.RejectLimit.PeakPower.bIsLowEnabled,0,0);
			LOGDBG("RejectLimit.PeakPower.Low				: %d",m_ActiveRecipe.RejectLimit.PeakPower.LowLimit,0,0);	  
		}
		
		if(IsLimitCheckEnabled(REJECT_LIMIT,PEAKPOWER_LIMIT_PL))
		{
			LOGDBG(" RejectLimit.PeakPower.IsEnabled		: %d",m_ActiveRecipe.RejectLimit.PeakPower.bIsHighEnabled,0,0);
			LOGDBG("RejectLimit.PeakPower.High 				: %d",m_ActiveRecipe.RejectLimit.PeakPower.HighLimit,0,0);
		}

		if(IsLimitCheckEnabled(REJECT_LIMIT,COLLAPSE_LIMIT_ML))
		{
			LOGDBG("RejectLimit.CollapseDistance.IsEnabled	: %d",m_ActiveRecipe.RejectLimit.CollapseDistance.bIsLowEnabled,0,0);
			LOGDBG("RejectLimit.CollapseDistance.Low		: %d",m_ActiveRecipe.RejectLimit.CollapseDistance.LowLimit,0,0);
		}
		
		if(IsLimitCheckEnabled(REJECT_LIMIT,COLLAPSE_LIMIT_PL))
		{
			LOGDBG("RejectLimit.CollapseDistance.IsEnabled	: %d",m_ActiveRecipe.RejectLimit.CollapseDistance.bIsHighEnabled,0,0);
			LOGDBG("RejectLimit.CollapseDistance.High		: %d",m_ActiveRecipe.RejectLimit.CollapseDistance.HighLimit,0,0);
		} 

		if(IsLimitCheckEnabled(REJECT_LIMIT,ABSOLUTE_LIMIT_ML))
		{
			LOGDBG("RejectLimit.AbsoluteDistance.IsEnabled 	: %d",m_ActiveRecipe.RejectLimit.AbsoluteDistance.bIsLowEnabled,0,0);
			LOGDBG("RejectLimit.AbsoluteDistance.Low 		: %d",m_ActiveRecipe.RejectLimit.AbsoluteDistance.LowLimit,0,0);
		}
		
		if(IsLimitCheckEnabled(REJECT_LIMIT,ABSOLUTE_LIMIT_PL))
		{
			LOGDBG("RejectLimit.AbsoluteDistance.IsEnabled 	: %d",m_ActiveRecipe.RejectLimit.AbsoluteDistance.bIsHighEnabled,0,0);
			LOGDBG("RejectLimit.AbsoluteDistance.High 		: %d",m_ActiveRecipe.RejectLimit.AbsoluteDistance.HighLimit,0,0);
		}

		if(IsLimitCheckEnabled(REJECT_LIMIT,TRIGGER_LIMIT_ML))
		{
			LOGDBG(" RejectLimit.TriggerDistance.IsEnabled 	: %d",m_ActiveRecipe.RejectLimit.TriggerDistance.bIsLowEnabled,0,0);
			LOGDBG(" RejectLimit.TriggerDistance.Low 		: %d", m_ActiveRecipe.RejectLimit.TriggerDistance.LowLimit,0,0);
		}

		if(IsLimitCheckEnabled(REJECT_LIMIT,TRIGGER_LIMIT_PL))
		{
			LOGDBG(" RejectLimit.TriggerDistance.IsEnabled 	: %d",m_ActiveRecipe.RejectLimit.TriggerDistance.bIsHighEnabled,0,0);
			LOGDBG(" RejectLimit.TriggerDistance.High 		: %d", m_ActiveRecipe.RejectLimit.TriggerDistance.HighLimit,0,0);
		}
		
		if(IsLimitCheckEnabled(REJECT_LIMIT,WELDFORCE_LIMIT_ML))
		{
			LOGDBG(" RejectLimit.WeldForce.IsEnabled		: %d",m_ActiveRecipe.RejectLimit.WeldForce.bIsLowEnabled,0,0);
			LOGDBG(" RejectLimit.WeldForce.Low				: %d",m_ActiveRecipe.RejectLimit.WeldForce.LowLimit,0,0);
		}
		
		if(IsLimitCheckEnabled(REJECT_LIMIT,WELDFORCE_LIMIT_PL))
		{
			LOGDBG(" RejectLimit.WeldForce.IsEnabled		: %d",m_ActiveRecipe.RejectLimit.WeldForce.bIsHighEnabled,0,0);
			LOGDBG(" RejectLimit.WeldForce.High				: %d",m_ActiveRecipe.RejectLimit.WeldForce.HighLimit,0,0);
		}

		if(IsLimitCheckEnabled(REJECT_LIMIT,FREQUENCY_LIMIT_ML))
		{
			LOGDBG(" RejectLimit.Frequency.IsEnabled		: %d",m_ActiveRecipe.RejectLimit.Frequency.bIsLowEnabled,0,0);
			LOGDBG(" RejectLimit.Frequency.Low				: %d",m_ActiveRecipe.RejectLimit.Frequency.LowLimit,0,0);
		}
		if(IsLimitCheckEnabled(REJECT_LIMIT,FREQUENCY_LIMIT_PL))
		{
			LOGDBG(" RejectLimit.Frequency.IsEnabled		: %d",m_ActiveRecipe.RejectLimit.Frequency.bIsHighEnabled,0,0);
			LOGDBG(" RejectLimit.Frequency.High				: %d",m_ActiveRecipe.RejectLimit.Frequency.HighLimit,0,0);
		}
	}
	else
	{
		LOGDBG("Reject limit disabled",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Display core weld parameters and user information of recipe.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ActiveRecipe::RecipeInformation()
{
	LOGDBG("------ActiveRecipe::RecipeInformation::Weld Recipe SC--------",0,0,0);
	LOGDBG("Weld mode 			: %d\n",m_ActiveRecipe.WeldMode,0,0);
	LOGDBG("Mode value 		: %d\n",m_ActiveRecipe.ModeValue,0,0);
	LOGDBG("Hold time 		: %d\n",m_ActiveRecipe.HoldTime,0,0);
	LOGDBG("Trigger distance	: %d\n",m_ActiveRecipe.TriggerDistance,0,0);
	LOGDBG("Trigger force 		: %d\n",m_ActiveRecipe.TriggerForce,0,0);
	LOGDBG("Weld time out   : %d\n",m_ActiveRecipe.MaxWeldTimeout,0,0);
	LOGDBG("bIsCoolingValveEnabled 	: %d\n",m_ActiveRecipe.bIsCoolingValveEnabled,0,0);
	LOGDBG("bIsTimedSeekEnabled 		: %d\n",m_ActiveRecipe.bIsTimedSeekEnabled,0,0);
	LOGDBG("TimedSeekPeriod 		: %d\n",m_ActiveRecipe.TimedSeekPeriod,0,0);
	LOGDBG("bIsPreWeldSeekEnabled 		:%d\n",m_ActiveRecipe.bIsPreWeldSeekEnabled,0,0);
	LOGDBG("bIsPostWeldSeekEnabled 		:%d\n",m_ActiveRecipe.bIsPostWeldSeekEnabled,0,0);
	LOGDBG("Recipe No 		: %d\n",m_ActiveRecipe.RecipeNumber,0,0);
	LOGDBG("Recipe Rev Num	: %d\n",m_ActiveRecipe.RecipeRevNumber,0,0);
	LOGDBG("User name 		: %s\n",(_Vx_usr_arg_t)m_ActiveRecipe.User,0,0);
	LOGDBG("Serial No 		: %s\n",(_Vx_usr_arg_t)m_ActiveRecipe.StackSerialNo,0,0);
}

/**************************************************************************//**
* \brief   - Display amplitude stepping related information.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ActiveRecipe::DisplayAmplitudeStep()
{
	LOGDBG("No.AmpStep 		: %d\n",m_ActiveRecipe.NumAmplitudeSteps,0,0);
	LOGDBG("AmplitudeStepAt	: %d\n",m_ActiveRecipe.AmplitudeStepAt,0,0);

	for(UINT8 idx = 0; idx < m_ActiveRecipe.NumAmplitudeSteps; idx++)
	{
		LOGDBG("Step%d	 	: %d\n",idx,m_ActiveRecipe.AmplitudeStep[idx],0);
	}

	for(UINT8 idx = 0; idx < m_ActiveRecipe.NumAmplitudeSteps; idx++)
	{
		LOGDBG("StepValue%d : %d\n",idx,m_ActiveRecipe.AmplitudeStepValue[idx],0);
	}
}

/**************************************************************************//**
* \brief   - Display force stepping related information.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ActiveRecipe::DisplayForceStep()
{
	LOGDBG("No.ForceStep 		: %d\n",m_ActiveRecipe.NumForceSteps,0,0);
	LOGDBG("ForceStepAt	 		: %d\n",m_ActiveRecipe.ForceStepAt,0,0);

	for(UINT8 idx = 0; idx < m_ActiveRecipe.NumForceSteps; idx++)
	{
		LOGDBG("StepAtValue%d 	: %d\n",idx,m_ActiveRecipe.ForceStepAtValue[idx],0);
	}
}

/**************************************************************************//**
* \brief   - Display pre-trigger parameters, Only when is it enabled.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ActiveRecipe::DisplayPreTrigger()
{
	if((m_ActiveRecipe.PreTrigger.bIsAutoEnabled) || (m_ActiveRecipe.PreTrigger.bIsEnabled)) 
	{
		LOGDBG("PreTrigger.bIsAutoEnabled    : %d\n",m_ActiveRecipe.PreTrigger.bIsAutoEnabled,0,0);
		LOGDBG("PreTrigger.bIsEnabled        : %d\n",m_ActiveRecipe.PreTrigger.bIsEnabled,0,0);
		LOGDBG("PreTrigger.TriggerAmplitude  : %d\n",m_ActiveRecipe.PreTrigger.TriggerAmplitude,0,0);
		LOGDBG("PreTrigger.bIsDistanceEnabled : %d\n",m_ActiveRecipe.PreTrigger.bIsDistanceEnabled,0,0);
		LOGDBG("PreTrigger.PreTriggerDistance : %d\n",m_ActiveRecipe.PreTrigger.PreTriggerDistance,0,0);
	}
}

/**************************************************************************//**
* \brief   - Display after burst parameters, Only when is it enabled.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ActiveRecipe::DisplayAfterBurst()
{
	if(m_ActiveRecipe.AfterBurst.bIsEnabled)
	{
		LOGDBG("AfterBurst.bIsEnabled    	: %d\n",m_ActiveRecipe.AfterBurst.bIsEnabled,0,0);
		LOGDBG("AfterBurst.BurstAmplitude	: %d\n",m_ActiveRecipe.AfterBurst.BurstAmplitude,0,0);
		LOGDBG("AfterBurst.BurstDelay    	: %d\n",m_ActiveRecipe.AfterBurst.BurstDelay,0,0);
		LOGDBG("AfterBurst.BurstTime     	: %d\n",m_ActiveRecipe.AfterBurst.BurstTime,0,0);
	}
}

/**************************************************************************//**
* \brief   - Display after EnergyBrake parameters
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ActiveRecipe::DisplayEnergyBrake()
{
	if(m_ActiveRecipe.EnergyBrake.bIsEnabled)
	{
		LOGDBG("EnergyBrake.bIsEnabled    			: %d\n",m_ActiveRecipe.EnergyBrake.bIsEnabled,0,0);
		LOGDBG("EnergyBrake.EnergyBrakeAmplitude	: %d\n",m_ActiveRecipe.EnergyBrake.EnergyBrakeAmplitude,0,0);
		LOGDBG("EnergyBrake.EnergyBrakeTime    	    : %d\n",m_ActiveRecipe.EnergyBrake.EnergyBrakeTime,0,0);
	}
}  

/**************************************************************************//**
* \brief   - Validates control limits based on the input
*
* \param   - UINT8 Type of value to check
*            UINT32 value - value to be check
*     
* \return  - bool bIsValid (TRUE or FALSE)
*
******************************************************************************/
bool ActiveRecipe::ValidateCutoffLimit(UINT8 typeOfMode, UINT32 value)
{
	bool bIsValid = false;
	
	switch(typeOfMode)
	{
	case PEAK_POWER_CUTOFF:
		if(value >= m_ActiveRecipe.CutoffLimit.PeakPowerCutoff.Value) 
		{
			bIsValid = true;
		}
		break;
	case COLLAPSE_DISTANCE_CUTOFF:
		//This cast is needed because sometimes collapse value throws negative values.
	    if((INT32)value >= (INT32)m_ActiveRecipe.CutoffLimit.CollapseDistanceCutoff.Value) 
	    {
	      	bIsValid = true;
	    }
	    break;	   
	case ABSOLUTE_DISTANCE_CUTOFF:
		if (value >= m_ActiveRecipe.CutoffLimit.AbsoluteDistanceCutoff.Value) 
		{
	      	bIsValid = true;
	    }
	    break;
	case FREQUENCY_LOW_CUTOFF:
		if (value <= m_ActiveRecipe.CutoffLimit.FrequencyLowCutoff.Value)
		{
			bIsValid = true;
		}
		break;
	case FREQUENCY_HIGH_CUTOFF:
		if (value >= m_ActiveRecipe.CutoffLimit.FrequencyHighCutoff.Value)
		{
			bIsValid = true;
		}
		break;
	case ENERGY_CUTOFF:
		if (value >= m_ActiveRecipe.CutoffLimit.EnergyCutoff.Value)
		{
			bIsValid = true;
		}
		break;
	case TIME_CUTOFF:
	   if (value >=m_ActiveRecipe.CutoffLimit.TimeCutoff.Value)
	   {
		    bIsValid = true;
	   }
	   break;
	default:
		LOGERR("Invalid Limit validation : %d",typeOfMode,0,0);
		break;
	}
	
	return bIsValid;	
}

/**************************************************************************//**
* \brief   - Display cutoff limits, Only when flag is enabled.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ActiveRecipe::DisplaycutoffLimit()
{
	if(IsGlobalCutOffLimitCheckEnabled())
	{
		if(IsCutoffLimitCheckEnabled(PEAK_POWER_CUTOFF))
		{
			LOGDBG("cutoffLimits.peakpower.IsEnabled 		: %d",m_ActiveRecipe.CutoffLimit.PeakPowerCutoff.bIsEnabled,0,0);
			LOGDBG("cutoffLimits.peakpower.value	 		: %d",m_ActiveRecipe.CutoffLimit.PeakPowerCutoff.Value,0,0);
		}

		if(IsCutoffLimitCheckEnabled(COLLAPSE_DISTANCE_CUTOFF))
		{
			LOGDBG("cutoffLimit.collapse.IsEnabled			: %d",m_ActiveRecipe.CutoffLimit.CollapseDistanceCutoff.bIsEnabled,0,0);
			LOGDBG("cutoffLimit.collapse.value 				: %d",m_ActiveRecipe.CutoffLimit.CollapseDistanceCutoff.Value,0,0);
		}

		if(IsCutoffLimitCheckEnabled(ABSOLUTE_DISTANCE_CUTOFF))
		{
			LOGDBG("cutoffLimit.Absolute.IsEnabled		    : %d",m_ActiveRecipe.CutoffLimit.AbsoluteDistanceCutoff.bIsEnabled,0,0);
			LOGDBG("cutoffLimit.Absolute.value				: %d",m_ActiveRecipe.CutoffLimit.AbsoluteDistanceCutoff.Value,0,0);	  
		}

		if(IsCutoffLimitCheckEnabled(FREQUENCY_LOW_CUTOFF))
		{
			LOGDBG("cutoffLimit.FrequencyLow.IsEnabled	    : %d",m_ActiveRecipe.CutoffLimit.FrequencyLowCutoff.bIsEnabled,0,0);
			LOGDBG("cutoffLimit.FrequencyLow.value  		: %d",m_ActiveRecipe.CutoffLimit.FrequencyLowCutoff.Value,0,0);
		} 

		if(IsCutoffLimitCheckEnabled(FREQUENCY_HIGH_CUTOFF))
		{
			LOGDBG("cutoffLimit.FrequencyHigh.IsEnabled 	: %d",m_ActiveRecipe.CutoffLimit.FrequencyHighCutoff.bIsEnabled,0,0);
			LOGDBG("cutoffLimit.FrequencyHigh.value 		: %d",m_ActiveRecipe.CutoffLimit.FrequencyHighCutoff.Value,0,0);
		}

		if(IsCutoffLimitCheckEnabled(ENERGY_CUTOFF))
		{
			LOGDBG(" cutoffLimit.Energy.IsEnabled       	: %d",m_ActiveRecipe.CutoffLimit.EnergyCutoff.bIsEnabled,0,0);
			LOGDBG(" cutoffLimit.Energye.value      		: %d", m_ActiveRecipe.CutoffLimit.EnergyCutoff.Value,0,0);
		}

		if(IsCutoffLimitCheckEnabled(TIME_CUTOFF))
		{
			LOGDBG(" cutoffLimit.Time.IsEnabled	        	: %d",m_ActiveRecipe.CutoffLimit.TimeCutoff.bIsEnabled,0,0);
			LOGDBG(" cutoffLimit.Time.value 				: %d ",m_ActiveRecipe.CutoffLimit.TimeCutoff.Value,0,0);
		}
	
	}
	else
	{
		LOGDBG("cutoff limit disabled",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Checks , Is suspect and reject limits validation enabled
*      		 or not against all the modes
*
* \param   - UINT8 typeOfLimit(Suspect or Reject), UINT8 typeOfMode
*
* \return  - bool bIsEnabled (TRUE or FALSE)
*
******************************************************************************/
bool ActiveRecipe::IsCutoffLimitCheckEnabled(UINT8 typeOfMode)
{
	bool bIsEnabled = false;

	switch(typeOfMode)
	{
		case PEAK_POWER_CUTOFF:
			bIsEnabled = m_ActiveRecipe.CutoffLimit.PeakPowerCutoff.bIsEnabled;
			break;
		case COLLAPSE_DISTANCE_CUTOFF:
			bIsEnabled = m_ActiveRecipe.CutoffLimit.CollapseDistanceCutoff.bIsEnabled;
			break;
		case ABSOLUTE_DISTANCE_CUTOFF:
			bIsEnabled = m_ActiveRecipe.CutoffLimit.AbsoluteDistanceCutoff.bIsEnabled;
			break;
		case TIME_CUTOFF:
			bIsEnabled = m_ActiveRecipe.CutoffLimit.TimeCutoff.bIsEnabled;
			break;
		case FREQUENCY_LOW_CUTOFF:
			bIsEnabled = m_ActiveRecipe.CutoffLimit.FrequencyLowCutoff.bIsEnabled;
			break;
		case FREQUENCY_HIGH_CUTOFF:
			bIsEnabled = m_ActiveRecipe.CutoffLimit.FrequencyHighCutoff.bIsEnabled;
			break;	
		case ENERGY_CUTOFF:
			bIsEnabled = m_ActiveRecipe.CutoffLimit.EnergyCutoff.bIsEnabled;
			break;
		case GROUND_DETECT_CUTOFF:
	     	bIsEnabled = m_ActiveRecipe.CutoffLimit.GroundDetectCutoff;
			break;
		default:
			LOGERR("Invalid Limit validation : %d",typeOfMode,0,0);
			break;
	}

	return bIsEnabled;
}

/**************************************************************************//**
* \brief   - Validates the cutoff limits are global option 
* 			 enabled or not.
*
* \param   - UINT8 typeOfLimit (Suspect or Reject)
*
* \return  - bool - TRUE on enable
*
******************************************************************************/
bool ActiveRecipe::IsGlobalCutOffLimitCheckEnabled()
{
	bool bIsEnabled = false;
	
	bIsEnabled = m_ActiveRecipe.bIsCutoffLimitEnabled;
	
	return bIsEnabled;
}

/**************************************************************************//**
* \brief   - Returns active recipe validation status.
*
* \param   - None.
*
* \return  - bool - true on recipe validated.
*
******************************************************************************/
bool ActiveRecipe::IsRecipeValidated() const
{
	return m_ActiveRecipe.bIsRecipeValidated;
}

/**************************************************************************//**
* \brief   - Object destruction and memory release of object.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
ActiveRecipe::~ActiveRecipe()
{	
}
