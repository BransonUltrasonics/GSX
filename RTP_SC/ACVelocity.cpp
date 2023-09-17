#include "ACVelocity.h"
#include "MainTask.h"

UINT8 ACVelocity::i = 0;
INT32 ACVelocity::instVelocities[VELOCITY_BUFFER_SIZE];
INT32 ACVelocity::lastPos=0;
INT32 ACVelocity::movingAverage=0;

/**************************************************************************//**
* 
* \brief   - Default constructor. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
ACVelocity::ACVelocity() {

}


/**************************************************************************//**
* 
* \brief   - Default destructor. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
ACVelocity::~ACVelocity() {
}


/**************************************************************************//**
* 
* \brief   - clear the velocity buffer and reset the moving average to 0 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACVelocity::ClearVelocityBuffer()
{
	for(i=0;i<VELOCITY_BUFFER_SIZE;i++)
	{
		instVelocities[i]=0;
	}
	i=0;
	movingAverage=0;
	lastPos = AC_TX->ActualDistance;
}


/**************************************************************************//**
* 
* \brief   - Get filtered velocity  
*
* \param   - None.
*
* \return  - moving average of velocity readings in µm/s
*
******************************************************************************/
INT32 ACVelocity::GetVelocity()
{
	return movingAverage;
}


/**************************************************************************//**
* 
* \brief   - Calculates the instantaneous velocity in µm/s and updates the moving average
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ACVelocity::CalculateVelocity()
{
	movingAverage -= instVelocities[i]/VELOCITY_BUFFER_SIZE;
	instVelocities[i] = (AC_TX->ActualDistance - lastPos) * (1000000 / ECAT_CYCLE_TIME_IN_US);
	movingAverage += instVelocities[i]/VELOCITY_BUFFER_SIZE;
	lastPos = AC_TX->ActualDistance;
	i = (i + 1) % VELOCITY_BUFFER_SIZE;
}
