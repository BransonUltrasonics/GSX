#include "ACState.h"
#include "AC.h"

/**************************************************************************//**
* \brief  - Default constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
ACState::ACState() {
}

/**************************************************************************//**
* \brief  - Default destructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
ACState::~ACState() {
}

/**************************************************************************//**
* \brief  - Wrapper function used by derived classes to call the state machine ChangeState function
*
* \param  - newState - state to which to transition
*
*
* \return  - none
*
******************************************************************************/
void ACState::ChangeState(ACSTATES newState) {
	AC::stateMachine.ChangeState(newState);
}
