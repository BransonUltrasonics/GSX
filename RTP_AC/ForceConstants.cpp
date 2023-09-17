/*
 * ForceCalibration.cpp
 *
 *  Created on: Nov 21, 2017
 *      Author: slurie
 */

#include "ForceConstants.h"
#include "Eeprom.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>

using namespace std;

/**************************************************************************//**
* \brief  - constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
ForceConstants::ForceConstants() {
	
}


/**************************************************************************//**
* \brief  - destructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
ForceConstants::~ForceConstants() {
	
}


/**************************************************************************//**
* \brief  - Reads force constants from a text file
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ForceConstants::LoadValuesFromFile()
{
	ifstream Fp(CAL_FILE);
	string forceConstsStr;
	UINT16 index = 0;
	
	for(index = 0; index <= FORCE_CALIBRATION_POLYNOMIAL_ORDER; index++) 
	{
		forceConstants[index] = 0.0;
	}	
	
	if(Fp.is_open())     //Check if file is opened
	{
		
		for(index = 0; index <= FORCE_CALIBRATION_POLYNOMIAL_ORDER; index++) 
		{
			getline(Fp,forceConstsStr,'\n');
			forceConstants[index] = atof(forceConstsStr.c_str());
		}
		Fp.close();    //Closes the text file
	}
}


/**************************************************************************//**
* \brief  - Reads force constants from a Eeprom
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ForceConstants::LoadValuesFromEeprom()
{
	UINT16 index = 0;
	char eeprom_sig;
	Eeprom epclass;

	for(index = 0; index <= FORCE_CALIBRATION_POLYNOMIAL_ORDER; index++) 
	{
		forceConstants[index] = 0.0;
	}
	
	epclass.Read(&eeprom_sig,FORCECONST_IDENTIFIER_SIZE,FORCECONST_BASE);

	if (eeprom_sig != FORCECONST_SIG)            /*Checks Eeprom is empty*/
	{	
			index  = 0;
			eeprom_sig = FORCECONST_SIG;
			epclass.Write(&eeprom_sig,FORCECONST_IDENTIFIER_SIZE,FORCECONST_BASE);
			
				forceConstants[0] = FORCECONSTANT_ONE; 
				forceConstants[1] = FORCECONSTANT_TWO;  
				epclass.Write((char *)&forceConstants,sizeof(forceConstants),FORCECONST_ADDR);
	}
	else
	{
	memset(&forceConstants,0x0,sizeof(forceConstants));
	epclass.Read((char *)&forceConstants,sizeof(forceConstants),FORCECONST_ADDR);
	}

}


/**************************************************************************//**
* \brief  - get a force constant
*
* \param  - polynomial order of desired constant
*
*
* \return  - force constant of requested order
*
******************************************************************************/
float ForceConstants::GetForceConstant(UINT16 power) {
	return forceConstants[power];
}


/**************************************************************************//**
* \brief  - Calculate 2nd order polynomial force constants based on 2 given points
* \breif  - Stores results to the txt file to be used loaded on system bootup
*
* \param  - two measured pairs of spring compression (µm) vs force applied (N)
*
*
* \return  - none
*
******************************************************************************/
void ForceConstants::CalculateTwoPointCalibration(UINT32 distanceOne, UINT32 forceOne, UINT32 distanceTwo, UINT32 forceTwo) {
	Eeprom 	epclass;
#ifdef READ_CAL_DATA_FROM_FILE
	ofstream Fp(CAL_FILE);
#endif //READ_CAL_DATA_FROM_FILE
	
#ifdef PRINT
	cout << "Distance one: "<< distanceOne << endl;
	cout << "Force one: "<< forceOne << endl;
	cout << "Distance two: "<< distanceTwo << endl;
	cout << "Force two: "<< forceTwo << endl;
#endif
	if(forceTwo > forceOne && forceOne > 0)
	{
		//spring constant calculated as change in force over change in distance
		forceConstants[1] = (float) (forceTwo - forceOne) / (float) (distanceTwo - distanceOne); 
		//y=mx+b substitute in point one for x and y and solve for b
		forceConstants[0] = (float) forceOne - ( forceConstants[1] * distanceOne); 
#ifdef PRINT
		cout << "m = " << forceConstants[1] << endl;
		cout << "b = " << forceConstants[0] << endl;
#endif

#ifdef READ_CAL_DATA_FROM_FILE
		if(Fp.is_open()) 
		{
			Fp << forceConstants[0] << endl << forceConstants[1];
			Fp.close();
		}
#endif //READ_CAL_DATA_FROM_FILE	
		epclass.Write((char *)&forceConstants,sizeof(forceConstants),FORCECONST_ADDR);
	}
}
