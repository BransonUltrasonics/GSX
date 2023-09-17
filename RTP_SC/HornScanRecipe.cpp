/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Contains scan recipe which is edit and set by user in UIC   
 
****************************************************************************/

#include "HornScanRecipe.h"

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
HornScanRecipe::HornScanRecipe()
{	
}

/**************************************************************************//**
* \brief   - Returns the digital tune flag status.
*
* \param   - None.
*
* \return  - UINT8 - SetDigTuneWithHornScan
*
******************************************************************************/
UINT8 HornScanRecipe::GetDigitalTuneStatus()
{
	return m_Recipe.SetDigTuneWithHornScan;
}

/**************************************************************************//**
* \brief   - Updates scan recipe parameter with latest data which is 
* 			 modified by user from UIC or DB.
*
* \param   - ScanRecipe& recipe
*
* \return  - None
*
******************************************************************************/
void HornScanRecipe::UpdateScanRecipe(ScanRecipe& recipe)
{
	m_Recipe = recipe;
}

/**************************************************************************//**
* 
* \brief   - Returns the latest scan recipe.
*
* \param   - None.
*
* \return  - ScanRecipe& m_Recipe.
*
******************************************************************************/
ScanRecipe& HornScanRecipe::GetScanRecipe()
{
	return m_Recipe;
}

/**************************************************************************//**
* \brief   - Object destruction and memory release of object.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
HornScanRecipe::~HornScanRecipe()
{
}
