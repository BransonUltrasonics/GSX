/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Contains test recipe which is edit and set by user in UIC   
 
****************************************************************************/

#include "TestRecipe.h"

using namespace std;

/**************************************************************************//**
* 
* \brief   - Initialization of class members and memory for object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
TestRecipe::TestRecipe()
{
}

/**************************************************************************//**
* \brief   - Returns the AmplitudeLoopC1 of test recipe.
*
* \param   - None.
*
* \return  - INT32 - AmplitudeLoopC1
*
******************************************************************************/
INT32 TestRecipe::GetAmplitudeLoopC1() const
{
	return m_recipe.AmplitudeLoopC1;
}

/**************************************************************************//**
* \brief   - Returns the AmplitudeLoopC2 of test recipe.
*
* \param   - None.
*
* \return  - INT32 - AmplitudeLoopC2
*
******************************************************************************/
INT32 TestRecipe::GetAmplitudeLoopC2() const
{
	return m_recipe.AmplitudeLoopC2;
}

/**************************************************************************//**
* \brief   - Returns the PhaseLoopI of test recipe.
*
* \param   - None.
*
* \return  - INT32 - PhaseLoopI
*
******************************************************************************/
INT32 TestRecipe::GetPhaseLoopI() const
{
	return m_recipe.PhaseLoopI;
}

/**************************************************************************//**
* \brief   - Returns the PhaseLoopCF of test recipe.
*
* \param   - None.
*
* \return  - INT32 - PhaseLoopCF
*
******************************************************************************/
INT32 TestRecipe::GetPhaseLoopCF() const
{
	return m_recipe.PhaseLoopCF;
}

/**************************************************************************//**
* \brief   - Returns the FrequencyHigh of test recipe.
*
* \param   - None.
*
* \return  - INT32 - FrequencyHigh
*
******************************************************************************/
INT32 TestRecipe::GetFrequencyHigh() const
{
	return m_recipe.FrequencyHigh * 10;
}

/**************************************************************************//**
* \brief   - Returns the FrequencyLow of test recipe.
*
* \param   - None.
*
* \return  - INT32 - FrequencyLow
*
******************************************************************************/
INT32 TestRecipe::GetFrequencyLow() const
{
	return m_recipe.FrequencyLow * 10;
}

/**************************************************************************//**
* \brief   - Returns the TestAmplitude of test recipe.
*
* \param   - None.
*
* \return  - UINT16 - TestAmplitude
*
******************************************************************************/
UINT16 TestRecipe::GetTestAmplitude() const
{
	return m_recipe.TestAmplitude;
}

/**************************************************************************//**
* \brief   - Updates latest test recipe which is received from UIC
*
* \param   - testRecipe& recipe
*
* \return  - None.
*
******************************************************************************/
void TestRecipe::UpdateTestRecipe(testRecipe& recipe)
{
	m_recipe = recipe;
}

/**************************************************************************//**
* \brief   - Object destruction and memory release of object.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
TestRecipe::~TestRecipe()
{
}
