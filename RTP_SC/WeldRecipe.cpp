/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Updating Weld Recipe to slaves( PC and AC )  
 
**********************************************************************************************************/


#include "WeldRecipe.h"

using namespace std;

// static initialization
RECIPE_STATUS WeldRecipeStatus::recipeStatus = UNSAVED;

/**************************************************************************//**
* 
* \brief   - Initialization of class members and memory for object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
PcWeldRecipe::PcWeldRecipe()
{
}

/**************************************************************************//**
* 
* \brief   - Copy constructor - To avoid shallow copy.
*
* \param   - const PcWeldRecipe& srcObj.
*
* \return  - None.
*
******************************************************************************/
PcWeldRecipe::PcWeldRecipe(const PcWeldRecipe& srcObj)
{
	m_Recipe = srcObj.m_Recipe;
}

/**************************************************************************//**
* 
* \brief   - Assignment operator overload - To avoid shallow copy.
*
* \param   - const PcWeldRecipe& srcObj.
*
* \return  - PcWeldRecipe& - Instance.
*
******************************************************************************/
PcWeldRecipe& PcWeldRecipe::operator=(const PcWeldRecipe& srcObj)
{
	m_Recipe = srcObj.m_Recipe;
	
	return *this;
}

/**************************************************************************//**
* 
* \brief   - return the StartFrequency as resonant freq.
*
* \param   - None.
*
* \return  - UINT32 StartFrequency in 10's of Hz.
*
******************************************************************************/
UINT32 PcWeldRecipe::GetResonantFreq()
{
	return m_Recipe.StartFrequency * 10;		/* convert Hz -> 10s of Hz */
}

/**************************************************************************//**
* 
* \brief   - return the DigitalTune.
*
* \param   - None.
*
* \return  - UINT32 DigitalTune in 10's of Hz.
*
******************************************************************************/
INT32 PcWeldRecipe::GetDigitalTune()
{
	return m_Recipe.DigitalTune;
}

/**************************************************************************//**
* 
* \brief   - return the MemoryOffset.
*
* \param   - None.
*
* \return  - UINT32 MemoryOffset in 10's of Hz.
*
******************************************************************************/
INT32 PcWeldRecipe::GetMemoryOffset()
{
	return m_Recipe.MemoryOffset;
}

/**************************************************************************//**
* 
* \brief   - return the FrequencyOffset.
*
* \param   - None.
*
* \return  - UINT32 FrequencyOffset in 10's of Hz.
*
******************************************************************************/
INT32 PcWeldRecipe::GetFrequencyOffset()
{
	return m_Recipe.FrequencyOffset;
}

/**************************************************************************//**
* 
* \brief   - update the StartFreq after seek or scan operation.
*
* \param   - UINT32 frequency
*
* \return  - None.
*
******************************************************************************/
void PcWeldRecipe::UpdateResonantFreq(UINT32 freqeuncy)
{
	//make sure the frequency has the correct order of magnitude, as it is in tenths of Hz in some places and Hz in others
	if(freqeuncy > 100000)
		m_Recipe.StartFrequency = freqeuncy / 10;
	else if (freqeuncy < 10000)
		m_Recipe.StartFrequency = freqeuncy * 10;
	else
		m_Recipe.StartFrequency = freqeuncy;
}

void PcWeldRecipe::UpdateMemoryOffset(INT32 MemoryOffset)
{
	m_Recipe.MemoryOffset = MemoryOffset;
	
}

void PcWeldRecipe::UpdateDigitalTune(INT32 DigitalTune)
{
	m_Recipe.DigitalTune = DigitalTune;
}

void PcWeldRecipe::UpdateFrequencyOffset(INT32 FreqOffset)
{
	m_Recipe.FrequencyOffset = FreqOffset;
}
/**************************************************************************//**
* 
* \brief   - Get the member data PCWeldRecipe.
*
* \param   - WeldRecipePC& recipe.
*
* \return  - None.
*
******************************************************************************/
void PcWeldRecipe::GetWeldRecipe(WeldRecipePC& recipe)
{
	recipe = m_Recipe;
	recipe.StartFrequency = recipe.StartFrequency * 10;		/* convert Hz -> 10s of Hz */
}

/**************************************************************************//**
* 
* \brief   - Assign recipe values to member data. 
*
* \param   - WeldRecipePC& recipe.
*
* \return  - None.
*
******************************************************************************/
void PcWeldRecipe::SetWeldRecipe(WeldRecipePC& recipe)
{
	recipe.FrequencyHigh = recipe.FrequencyHigh * 10;  /* convert Hz -> 10s of Hz */
	recipe.FrequencyLow = recipe.FrequencyLow * 10;
	recipe.PhaseLimitTime = PHASELIMITTIME_DEFAULT;
	recipe.PIPhaseLimitTime = PIRPHASELIMITTIME_DEFAULT;
	m_Recipe = recipe;
	m_Recipe.StartFrequency = m_Recipe.StartFrequency / 10;		/* convert 10s of Hz -> Hz */
}

/**************************************************************************//**
* 
* \brief   - Memory release of object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
PcWeldRecipe::~PcWeldRecipe()
{
}


/**************************************************************************//**
* 
* \brief   - Initialization of class members and memory for object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
AcWeldRecipe::AcWeldRecipe()
{
}

/**************************************************************************//**
* 
* \brief   - Copy constructor - To avoid shallow copy.
*
* \param   - const AcWeldRecipe& srcObj.
*
* \return  - None.
*
******************************************************************************/
AcWeldRecipe::AcWeldRecipe(const AcWeldRecipe& srcObj)
{
	*this = srcObj;
}

/**************************************************************************//**
* 
* \brief   - Assignment operator overload - To avoid shallow copy.
*
* \param   - const AcWeldRecipe& srcObj.
*
* \return  - AcWeldRecipe& - instance.
*
******************************************************************************/
AcWeldRecipe& AcWeldRecipe::operator=(const AcWeldRecipe& srcObj)
{
	//m_Recipe = srcObj.m_Recipe;
	m_Recipe.WeldForce						= srcObj.m_Recipe.WeldForce;
	m_Recipe.ForceRampTime					= srcObj.m_Recipe.ForceRampTime;	
	m_Recipe.HoldMode						= srcObj.m_Recipe.HoldMode;
	m_Recipe.TotalCollapseTarget			= srcObj.m_Recipe.TotalCollapseTarget;
	m_Recipe.HoldForce						= srcObj.m_Recipe.HoldForce;
	m_Recipe.HoldForceRampTime				= srcObj.m_Recipe.HoldForceRampTime;
	m_Recipe.ExpectedPartContactPosition	= srcObj.m_Recipe.ExpectedPartContactPosition;
	m_Recipe.ReadyPosition					= srcObj.m_Recipe.ReadyPosition;
	m_Recipe.DownAcceleration				= srcObj.m_Recipe.DownAcceleration;
	m_Recipe.DownMaxVelocity				= srcObj.m_Recipe.DownMaxVelocity;
	m_Recipe.DownDeceleration				= srcObj.m_Recipe.DownDeceleration;
	m_Recipe.ReturnAcceleration				= srcObj.m_Recipe.ReturnAcceleration;
	m_Recipe.ReturnMaxVelocity				= srcObj.m_Recipe.ReturnMaxVelocity;
	m_Recipe.ReturnDeceleration				= srcObj.m_Recipe.ReturnDeceleration;
	m_Recipe.ExpectedPartContactOffset		= srcObj.m_Recipe.ExpectedPartContactOffset;
	m_Recipe.PartContactWindowMinus			= srcObj.m_Recipe.PartContactWindowMinus;
	m_Recipe.PartContactWindowPlus			= srcObj.m_Recipe.PartContactWindowPlus;
	m_Recipe.NumForceSteps					= srcObj.m_Recipe.NumForceSteps;
	m_Recipe.ReadyPositionToggle			= srcObj.m_Recipe.ReadyPositionToggle;
	m_Recipe.WeldForceControl				= srcObj.m_Recipe.WeldForceControl;
	
    copy(srcObj.m_Recipe.ForceStepRampTime, srcObj.m_Recipe.ForceStepRampTime + (sizeof(srcObj.m_Recipe.ForceStepRampTime)/sizeof(srcObj.m_Recipe.ForceStepRampTime[0])), m_Recipe.ForceStepRampTime);
    copy(srcObj.m_Recipe.ForceStepForce, srcObj.m_Recipe.ForceStepForce + (sizeof(srcObj.m_Recipe.ForceStepForce)/sizeof(srcObj.m_Recipe.ForceStepForce[0])), m_Recipe.ForceStepForce);

	return *this;
}

/**************************************************************************//**
* 
* \brief   - returns the updated part contact position .
*
* \param   - None.
*
* \return  - UINT32 - ExpectedPartContactPosition.
*
******************************************************************************/
UINT32 AcWeldRecipe::GetPartContactPos() const
{
	return m_Recipe.ExpectedPartContactPosition;
}

/**************************************************************************//**
* 
* \brief   - returns the updated part contact position .
*
* \param   - None.
*
* \return  - UINT16 - WeldForce.
*
******************************************************************************/
UINT16 AcWeldRecipe::GetWeldForce() const
{
	return m_Recipe.WeldForce;
}

/**************************************************************************//**
 * 
 * \brief   - returns the Force of the selected StepForce.
 *
 * \param   - index.
 *
 * \return  - UINT16 - ForceStepForce.
 *
 ******************************************************************************/
 UINT16 AcWeldRecipe::GetForceStepForce(UINT8 index) const
 {
 	return m_Recipe.ForceStepForce[index];
 }

/**************************************************************************//**
* 
* \brief   - updates the part contact position.
*
* \param   - UINT32 - position.
*
* \return  - None.
*
******************************************************************************/
void AcWeldRecipe::UpdatePartContactPos(UINT32 position)
{	
	m_Recipe.ExpectedPartContactPosition = position;
}

/**************************************************************************//**
* 
* \brief   - Get the member data of ACWeldRecipe.
*
* \param   - WeldRecipeAC& recipe.
*
* \return  - None.
*
******************************************************************************/
void AcWeldRecipe::GetWeldRecipe(WeldRecipeAC& recipe)
{
	recipe = m_Recipe;
}

/**************************************************************************//**
* 
* \brief   - Assign recipe values to member data. 
*
* \param   - WeldRecipeAC& recipe.
*
* \return  - None.
*
******************************************************************************/
void AcWeldRecipe::SetWeldRecipe(WeldRecipeAC& recipe)
{
	m_Recipe = recipe;
}

/**************************************************************************//**
* 
* \brief   - Memory release of object. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
AcWeldRecipe::~AcWeldRecipe()
{
}

WeldRecipeAC::WeldRecipeAC()		/* Constructor of WeldRecipeAC structure which is defined in WeldRecipe.h */
{
		/* default constructor initializes each parameter with the min value */
		WeldForce 				= AC_MIN_WeldForce;
		ForceRampTime 			= AC_MIN_ForceRampTime;
		HoldMode 				= AC_MIN_HoldMode;
		TotalCollapseTarget 	= AC_MIN_TotalCollapseTarget;
		HoldForce 				= AC_MIN_HoldForce ;
		HoldForceRampTime 		= AC_MIN_HoldForceRampTime;
		ExpectedPartContactPosition = AC_MIN_ExpectedPartContactPosition;
		ReadyPosition  			= AC_MIN_ReadyPosition;
		DownAcceleration 		= AC_MIN_DownAcceleration;
		DownMaxVelocity 		= AC_MIN_DownMaxVelocity;
		DownDeceleration 		= AC_MIN_DownDeceleration;
		ReturnAcceleration 		= AC_MIN_ReturnAcceleration;
		ReturnMaxVelocity 		= AC_MIN_ReturnMaxVelocity;
		ReturnDeceleration 		= AC_MIN_ReturnDeceleration;
		NumForceSteps 			= AC_MIN_NumForceSteps;
		ForceStepForce[0] 		= AC_MIN_ForceStepForce;
		ForceStepRampTime[0] 	= AC_MIN_ForceStepRampTime;
		ForceStepForce[1] 		= AC_MIN_ForceStepForce;
		ForceStepRampTime[1] 	= AC_MIN_ForceStepRampTime;
		ForceStepForce[2] 		= AC_MIN_ForceStepForce;
		ForceStepRampTime[2] 	= AC_MIN_ForceStepRampTime;
		ForceStepForce[3] 		= AC_MIN_ForceStepForce;
		ForceStepRampTime[3] 	= AC_MIN_ForceStepRampTime;
		ForceStepForce[4] 		= AC_MIN_ForceStepForce;
		ForceStepRampTime[4] 	= AC_MIN_ForceStepRampTime;
		ForceStepForce[5] 		= AC_MIN_ForceStepForce;
		ForceStepRampTime[5] 	= AC_MIN_ForceStepRampTime;
		ForceStepForce[6] 		= AC_MIN_ForceStepForce;
		ForceStepRampTime[6] 	= AC_MIN_ForceStepRampTime;
		ForceStepForce[7] 		= AC_MIN_ForceStepForce;
		ForceStepRampTime[7] 	= AC_MIN_ForceStepRampTime;
		ForceStepForce[8] 		= AC_MIN_ForceStepForce;
		ForceStepRampTime[8] 	= AC_MIN_ForceStepRampTime;
		ForceStepForce[9] 		= AC_MIN_ForceStepForce;
		ForceStepRampTime[9] 	= AC_MIN_ForceStepRampTime;
		
		PartContactWindowMinus	= AC_MIN_PartContactWindowMinus; 
		PartContactWindowPlus	= AC_MIN_PartContactWindowPlus;
		ExpectedPartContactOffset = AC_MIN_ExcepectedPartContactOffset;
		WeldForceControl		= (UINT16)CONTROL_HIGH;
}

WeldRecipePC::WeldRecipePC()			/* Constructor of WeldRecipePC structure which is defined in WeldRecipe.h */
{	
		/*Initialize the all parameter with the min value	*/
		switch(uiFrequencyInput)
		{
			case FREQ_20KHZ:
				AmplitudeLoopC1 	= AMPLOOPC1_20KDEFAULT;
				AmplitudeLoopC2 	= AMPLOOPC2_20KDEFAULT;
				PhaseLoop 			= PLOOP_20KDEFAULT;
				PhaseLoopCF 		= PLOOPCF_20KDEFAULT;
				FrequencyLow 		= FREQENCYLOW_20KDEFAULT;
				FrequencyHigh 		= FREQENCYHIGH_20KDEFAULT;
				StartFrequency		= START_FREQUENCY20KDEFAULT;
				DigitalTune	    	= START_FREQUENCY20KDEFAULT;
				break;
			case FREQ_30KHZ:
				AmplitudeLoopC1 	= AMPLOOPC1_30KDEFAULT;
				AmplitudeLoopC2 	= AMPLOOPC2_30KDEFAULT;
				PhaseLoop 			= PLOOP_30KDEFAULT;
				PhaseLoopCF 		= PLOOPCF_30KDEFAULT;
				FrequencyLow 		= FREQENCYLOW_30KDEFAULT;
				FrequencyHigh 		= FREQENCYHIGH_30KDEFAULT;
				StartFrequency		= START_FREQUENCY30KDEFAULT;
				DigitalTune	    	= START_FREQUENCY30KDEFAULT;
				break;
			case FREQ_40KHZ:
				AmplitudeLoopC1 	= AMPLOOPC1_40KDEFAULT;
				AmplitudeLoopC2 	= AMPLOOPC2_40KDEFAULT;
				PhaseLoop 			= PLOOP_40KDEFAULT;
				PhaseLoopCF 		= PLOOPCF_40KDEFAULT;
				FrequencyLow 		= FREQENCYLOW_40KDEFAULT;
				FrequencyHigh 		= FREQENCYHIGH_40KDEFAULT;
				StartFrequency		= START_FREQUENCY40KDEFAULT;
				DigitalTune	    	= START_FREQUENCY40KDEFAULT;
				break;
		}

		PhaseLimitTime 			= PC_MIN_PhaseLimitTime;
		PhaseLimit 				= PC_MIN_PhaseLimit;
		ControlMode 			= PC_MIN_ControlMode;
		BlindtimeSeek 			= PC_MIN_BlindtimeSeek;
		Blindtimeweld 			= PC_MIN_Blindtimeweld;
		DDSSwitchtime 			= PC_MIN_DDSSwitchtime;
		Parameter7 				= PC_MIN_Parameter7;
		Parameter8 				= PC_MIN_Parameter8;
		Parameter9 				= PC_MIN_Parameter9;
		Parameter10 			= PC_MIN_Parameter10;
		Parameter11 			= PC_MIN_Parameter11;
		Parameter12 			= PC_MIN_Parameter12;
		Parameter13 			= PC_MIN_Parameter13;
		Parameter14 			= PC_MIN_Parameter14;
		Parameter15				= PC_MIN_Parameter15;
		FLimitTime 				= PC_MIN_FLimitTime;
		AmpProportionalGain 	= PC_MIN_AmpProportionalGain;
		AmpIntegralGain 		= PC_MIN_AmpIntegralGain;
		PhaseProportionalGain 	= PC_MIN_PhaseProportionalGain;
		FrequencyWindowSize 	= PC_MIN_FrequencyWindowSize;
		PFofPhasecontrolloop 	= PC_MIN_PFofPhasecontrolloop;
		PIFrequencyLow 			= PC_MIN_PIFrequencyLow;
		PIFrequencyHigh 		= PC_MIN_PIFrequencyHigh;
		PIPhaseLimitTime 		= PC_MIN_PIPhaseLimitTime;
		PIPhaseLimit 			= PC_MIN_PIPhaseLimit;
		WeldRampTime			= PC_MIN_WeldRampTime;
		PhaseIntegralGain		= 0;
		MemoryOffset            = 0;
		FrequencyOffset         = 0;
	}
