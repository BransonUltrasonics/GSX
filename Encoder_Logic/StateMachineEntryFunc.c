/*
 * StateMachineEntryFunc.c
 *
 *  Created on: Mar 2, 2018
 *      Author: eguajardo
 */

#include "StateMachineEntryFunc.h"
#include "Global.h"
#include "DeviceDiagnostics.h"

extern tUINT32 DriveONCtr;

/***********************************************************************//*****
* \brief  - Enter
* 			This is Entry code for PowerUp State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void PowerUpEntry()
{
#ifndef PROTOTYPEBOARD
	printf("PowerUpEntry\n");
#endif
}

/***********************************************************************//*****
* \brief  - Enter
* 			This is Entry code for PreReady State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void PreReadyEntry()
{
#ifndef PROTOTYPEBOARD
	printf("PreReadyEntry\n");
#endif
}

/************************************************************************//*****
* \brief  - Enter
* 			This is Entry code for Ready State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ReadyEntry()
{
#ifndef PROTOTYPEBOARD
	printf("ReadyEntry\n");
#endif
}

/*************************************************************************//*****
* \brief  - Enter
* 			This is Entry code for Fault State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void FaultEntry()
{
#ifndef PROTOTYPEBOARD
	printf("FaultEntry\n");
	if(PrintfEnabled == TRUE)
	{
		if((FaultFlag & PBFaultFlag) == PBFaultFlag)
		{
			printf("PBFault\n");
		}
		 if((FaultFlag & UFAIL24FaultFlag) == UFAIL24FaultFlag)
		{
			printf("UFail24V\n");
		}
		 if((FaultFlag & EStopFaultFlag) == EStopFaultFlag)
		{
			printf("EStopFault\n");
		}
		 if((FaultFlag & LEFaultFlag) == LEFaultFlag)
		{
			printf("LEFault\n");
		}
		 if((FaultFlag & SBeamFaultFlag) == SBeamFaultFlag)
		{
			printf("SBeamFault\n");
		}
		 if((FaultFlag & TRSFaultFlag) == TRSFaultFlag)
		{
			printf("TRSFault\n");
		}
		 if((FaultFlag & DriveFaultFlag) == DriveFaultFlag)
		{
			printf("DriveFault\n");
		}
		 if((FaultFlag & CMFaultFlag) == CMFaultFlag)
		{
			printf("CMFault\n");
		}
		 if((FaultFlag & LUFaultFlag) == LUFaultFlag)
		{
			printf("LUFault\n");
		}
		 if((FaultFlag & SonicsEnFaultFlag) == SonicsEnFaultFlag)
		 {
			 printf("SonicsEnFault\n");
		 }
	}
#endif
}

/*************************************************************************//*****
* \brief  - Enter
* 			This is Entry code for DriveOn State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void DriveOnEntry()
{
#ifndef PROTOTYPEBOARD
	printf("DriveOnEntry\n");
#endif
	DriveONCtr = 0;
}

/*************************************************************************//*****
* \brief  - Enter
* 			This is Entry code for PCDetected State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void PCDetectedEntry()
{
#ifndef PROTOTYPEBOARD
	printf("PCDetectedEntry\n");
#endif
}

/*************************************************************************//*****
* \brief  - Enter
* 			This is Entry code for FaultReset State.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void FaultResetEntry()
{
#ifndef PROTOTYPEBOARD
	printf("FaultResetEntry\n");
#endif
}
