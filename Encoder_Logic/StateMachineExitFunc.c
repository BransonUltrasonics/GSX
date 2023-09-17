/*
 * StateMachineExitFunc.c
 *
 *  Created on: Mar 2, 2018
 *      Author: eguajardo
 */


#include "StateMachineExitFunc.h"
#include "Global.h"

extern tSINT32 QEIPosition;
extern tSINT32 QEIPositionOld;

/******************************************************************************
* \brief  - Exit
* 			This is Exit code for PowerUp State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void PowerUpExit()
{
#ifndef PROTOTYPEBOARD
	printf("PowerUpExit\n");
#endif
}

/******************************************************************************
* \brief  - Exit
* 			This is Exit code for PreReady State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void PreReadyExit()
{
#ifndef PROTOTYPEBOARD
	printf("PreReadyExit\n");
#endif
}

/************************************************************************//******
* \brief  - Exit
* 			This is Exit code for Ready State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ReadyExit()
{
#ifndef PROTOTYPEBOARD
	printf("ReadyExit\n");
#endif
	QEI_ResetPosition();
	QEI_GetPosition();
	QEIPositionOld = QEIPosition;
}

/************************************************************************//******
* \brief  - Exit
* 			This is Exit code for Fault State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void FaultExit()
{
#ifndef PROTOTYPEBOARD
	printf("FaultExit\n");
#endif
}

/************************************************************************//******
* \brief  - Exit
* 			This is Exit code for DriveOn State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void DriveOnExit()
{
#ifndef PROTOTYPEBOARD
	printf("DriveOnExit\n");
#endif
}

/************************************************************************//******
* \brief  - Exit
* 			This is Exit code for PCDetected State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void PCDetectedExit()
{
#ifndef PROTOTYPEBOARD
	printf("PCDetectedExit\n");
#endif
}

/*************************************************************************//*****
* \brief  - Exit
* 			This is Exit code for FaultReset State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void FaultResetExit()
{
#ifndef PROTOTYPEBOARD
	printf("FaultResetExit\n");
#endif
}
