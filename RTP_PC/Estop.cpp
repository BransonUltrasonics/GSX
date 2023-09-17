/*
 * Estop.cpp
 *
 *  Created on: May 17, 2021
 *      Author: BMX
 */

#include "PCStateMachine.h"
#include "PCObjDictionary.h"
#include <stdio.h>
#include <commons.h>
#include "FPGA.h"

/******************************************************************************
* \brief  - Estop
* 			constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
Estop::Estop() 
{
	//Constructor
}



/******************************************************************************
* \brief  - ~Estop
* 			destructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
Estop::~Estop()
{
	//Destructor
}

/******************************************************************************
* \brief  - Enter
* 			This is Entry code for Estop State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Estop::Enter()
{
	//Enter code
	FPGAStat = 0;
}

/******************************************************************************
* \brief  - Loop
* 			This is Loop code for Estop State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Estop::Loop()
{
	if((PC_RxPdo.MasterEvents & BIT_MASK(CTRL_ESTOP)) != BIT_MASK(CTRL_ESTOP))
	{
		FPGAStat = FPGA::GetStatus();
			
		if(((FPGAStat & FPGA_STAT_INIT_DONE) != 0) && ((FPGAStat & FPGA_STAT_RUN) != 0))
		{
			ChangeState(PC_READY);
		}
	}
	else
	{
		//Do nothing while E-stop is pressed
	}
}


/******************************************************************************
* \brief  - Exit
* 			This is Exit code for Estop State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Estop::Exit()
{
	//Exit code
	
	//Read recipes
	SeekRecipobj.ReadSeekRecipeFromOD();
	WeldRecipeObj.ReadWeldRecipeFromOD();
	
	FPGA::SetFrequency(WeldRecipeObj.PCWeldRecipe.StartFrequency);
	
}
