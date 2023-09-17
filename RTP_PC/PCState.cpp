/*
 * PCState.cpp
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */


#include "PCState.h"
#include "PCStateMachine.h"
#include "PC.h"



/**************************************************************************//**
* \brief  - Constructor.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/
PCState::PCState()
{
	//Constructor
	
}

/**************************************************************************//**
* \brief  - Destructor.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/
PCState::~PCState()
{
	//Destructor
}

/**************************************************************************//**
* \brief  - State of entry code.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/
void PCState::Enter()
{
	//Enter code
}

/**************************************************************************//**
* \brief  - State of exit code.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/
void PCState::Exit()
{
	//Exit code
}

/**************************************************************************//**
* \brief  - State of loop code.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/
void PCState::Loop()
{
	//Loop code
}

/**************************************************************************//**
* \brief  - Jump to the next state.
*
* \param  - NextState.
*			
* \return  - none.
*
******************************************************************************/
void PCState::ChangeState(PCSTATES NextState)
{	
	PC::stateMachine.ChangeState(NextState);
}


