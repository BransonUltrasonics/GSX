/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
/*
  * Utils.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: DShilonie
 *      
 *  Implementations for various utilities should be here.
 *  
 */
 

/* Header files */
#include "Utils.h"

/**************************************************************************//**
 * \brief convertNtolbs : Convert Newtons to lbs
 * \param dbNewtons
 * \return
******************************************************************************/
float Utils::convertNtolbs(float dbNewtons)
{
    float dblbs;
    dblbs = dbNewtons /4.44822;
    return dblbs;
}

/**************************************************************************//**
 * \brief convertmillimetretoinches : Convert millimeter to inches
 * \param dbMillimeter
 * \return
 ******************************************************************************/
float Utils::convertmillimetretoinches(float dbMillimeter)
{
    float dbInches;
    dbInches = dbMillimeter / 25.4;
    return dbInches;
}

/**************************************************************************//**
 * \brief convertMicrometreToMillimetre : Convert micro meter to milli meter
 * \param dbMicrometer
 * \return
 ******************************************************************************/
float Utils::convertMicrometerToMillimeter(float dbMicrometer)
{
    float dbMillimeter;
    dbMillimeter = dbMicrometer/(float)1000;
    return dbMillimeter;
}

/**************************************************************************//**
 * \brief convertmicrometretoinches : Convert micro meter to inches
 * \param dbmicrometer
 * \return
  ******************************************************************************/
float Utils::convertMicrometerToInches(float dbMicrometer)
{
    float dbInches;
    dbInches = dbMicrometer / (float)25400;
    return dbInches;
}

/**************************************************************************//**
 * \brief convertMsecToSec : Convert milliseconds to  seconds
 * \param time
 * \return
 ******************************************************************************/
float Utils::convertMsecToSec(float time)
{
    float tmp;
    tmp = time/(float)1000;
    return tmp;
}

/**************************************************************************//**
 * \brief convertMsecToSec : Convert millimeter per seconds to inches per seconds
 * \param time
 * \return
 ******************************************************************************/
float Utils::convertMMPerSecToInchPerSec(float velocity)
{
    float tmp;
    tmp = velocity/25.4;
    return tmp;
}

/**************************************************************************//**
 * \brief convertlbstoN : Convert lbs to newton
 * \param dblbs
 * \return
 *******************************************************************************/
float Utils::convertlbstoN(float dblbs)
{
	float dbNewtons;
    dbNewtons = dblbs * 4.44822;
    return dbNewtons;
}

/**************************************************************************//**
 * \brief convertInchesToMicrometre : Convert inches to micro meter
 * \param dbInches
 * \return
 ******************************************************************************/
float Utils::convertInchesToMicrometre(float dbInches)
{
	float dbMicrometre;
    dbMicrometre = dbInches * (float)25400;
    return dbMicrometre;
}

/**************************************************************************//**
 * \brief WeldRecipeParameter::convertInchesToMillimetre : Convert inches to millimetre
 * \param dbInches
 * \return
 */
float Utils::convertInchesToMillimetre(float dbInches)
{
	float dbMillimetre;
    dbMillimetre = dbInches *25.4;
    return dbMillimetre;
}


