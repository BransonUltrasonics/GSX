/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Contains seek recipe which is edit and set by user in UIC   
 
****************************************************************************/

#include "SeekRecipe.h"

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
SeekRecipe::SeekRecipe()
{	
}

/**************************************************************************//**
* 
* \brief   - Returns the seek time.
*
* \param   - None.
*
* \return  - UINT32 m_SeekTime.
*
******************************************************************************/
UINT32 SeekRecipe::GetSeekTime() const
{
	return m_Recipe.SeekTime;
}

/**************************************************************************//**
* \brief   - Updates seek recipe parameter with latest data which is 
* 			 modified by user from UIC or DB.
*
* \param   - SeekRecipe& recipe
*
* \return  - None
*
******************************************************************************/
void SeekRecipe::UpdateSeekRecipe(seekRecipe& recipe)
{
	m_Recipe = recipe;
}

/**************************************************************************//**
* 
* \brief   - Returns the latest seek recipe.
*
* \param   - None.
*
* \return  - seekRecipe& m_Recipe.
*
******************************************************************************/
seekRecipe& SeekRecipe::GetSeekRecipe()
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
SeekRecipe::~SeekRecipe()
{
}
