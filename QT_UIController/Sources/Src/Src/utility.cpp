/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Converting All Units

***************************************************************************/


#include "Header/utility.h"
#include "Header/configuration.h"


namespace Utility
{

/**
 * @brief convertSecToMsec : Convert seconds to milliseconds
 * @param time
 * @return
 */
double convertSecToMsec(double time)
{
    double tmp;
    tmp=time*(double)1000;
    return tmp;
}

/**
 * @brief convertMsecToSec : Convert milliseconds to  seconds
 * @param time
 * @return
 */
double convertMsecToSec(double time)
{
    double tmp;
    tmp=time/(double)1000;
    return tmp;
}

/**
 * @brief convertMicrometreToMillimetre : Convert micro meter to milli meter
 * @param length
 * @return
 */
double convertMicrometreToMillimetre(double length)
{
    double tmp;
    tmp=length/(double)1000;
    return tmp;
}

/**
 * @brief convertMillimetreToMicrometre : Convert millimeter to micro meter
 * @param length
 * @return
 */
double convertMillimetreToMicrometre(double length)
{
    double tmp;
    tmp=length*(double)1000;
    return tmp;
}

/**
 * @brief convertinchestomicrometre : Convert inches to micro meter
 * @param dbInches
 * @return
 */
double convertinchestomicrometre(double dbInches)
{
    double dbMicrometre;
    dbMicrometre = dbInches * (double)25400;
    return dbMicrometre;
}

/**
 * @brief convertmicrometretoinches : Convert micro meter to inches
 * @param dbmicrometres
 * @return
 */
double convertmicrometretoinches(double dbmicrometres)
{
    double dbInches;
    dbInches = dbmicrometres / (double)25400;
    return dbInches;
}


/**
 * @brief Configuration::convertMilliJoulesToJoules : Converts MilliJoules to Joules
 * @param length
 * @return
 */
double convertMilliJoulesToJoules(double length)
{
    double tmp;
    tmp=length/(double)1000;
    return tmp;
}

/**
 * @brief Configuration::convertJoulesToMilliJoules : Converts Joules to MilliJoules
 * @param length
 * @return
 */
double convertJoulesToMilliJoules(double length)
{
    double tmp;
    tmp=length*(double)1000;
    return tmp;
}


/**
 * @brief convertNtolbs : Convert Newtons to lbs
 * @param dbNewtons
 * @return
 */
double convertNtolbs(double dbNewtons)
{
    double dblbs;
    dblbs = dbNewtons /(double)4.44822;
    return dblbs;
}

/**
 * @brief convertlbstoN : Convert lbs to newton
 * @param dblbs
 * @return
 */
double convertlbstoN(double dblbs)
{
    double dbNewtons;
    dbNewtons = dblbs *(double)4.44822;
    return dbNewtons;
}

/**
 * @brief convertSecToMsec : Convert seconds to milli seconds
 * @param time
 * @return
 */
QString convertSecToMsec(const QString& time)
{
    return QString::number(convertSecToMsec(time.toDouble()));
}

/**
 * @brief convertNtolbs : Convert Newtons to lbs
 * @param newton
 * @return
 */
QString convertNtolbs(const QString& newton)
{
    return QString::number(convertNtolbs(newton.toDouble()));
}

/**
 * @brief convertmicrometretoinches :  Convert micro meter to inches
 * @param microMeter
 * @return
 */
QString convertmicrometretoinches(const QString& microMeter)
{
    QString convetedValue = "";
    convetedValue.setNum(convertmicrometretoinches(microMeter.toDouble()),'f', 4);

    return convetedValue;
}

/**
 * @brief convertMsecToSec : Convert milliseconds to sec onds
 * @param milliSeconds
 * @return
 */
QString convertMsecToSec(const QString &milliSeconds)
{
    QString convetedValue = "";
    convetedValue.setNum(convertMsecToSec(milliSeconds.toDouble()), 'f', 3);

    return convetedValue;
}

/**
 * @brief convertMicrometreToMillimetre : Convert micro meter to millimeter
 * @param microMeter
 * @return
 */
QString convertMicrometreToMillimetre(const QString &microMeter)
{
    QString convetedValue = "";
    /*To handle precision for mm unit parameters in Analytics History screen based on System Type*/
    if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
    {
        convetedValue.setNum(convertMicrometreToMillimetre(microMeter.toDouble()),'f', 2);
    }

    else
    {
        convetedValue.setNum(convertMicrometreToMillimetre(microMeter.toDouble()),'f', 3);
    }

    return convetedValue;
}

/**
 * @brief convertMilliJoulesToJoules : Convert convert MilliJoules To Joules
 * @param MilliJoules
 * @return Joules
 */
QString convertMilliJoulesToJoules(const QString &milliJoules)
{
    QString convetedValue = "";
    convetedValue.setNum(convertMilliJoulesToJoules(milliJoules.toDouble()),'f', 1);

    return convetedValue;
}
}
