#ifndef UTILITY_H
#define UTILITY_H

#include <QString>

namespace Utility
{
// generic converison functions
double convertSecToMsec(double);
double convertMsecToSec(double);
double convertMicrometreToMillimetre(double);
double convertMillimetreToMicrometre(double);
double convertinchestomicrometre(double);
double convertmicrometretoinches(double);
double convertMilliJoulesToJoules(double);
double convertJoulesToMilliJoules(double);
double convertNtolbs(double);
double convertlbstoN(double);

// convertion utility function for SAE units
QString convertMsecToSec(const QString& milliSeconds);
QString convertNtolbs(const QString& newton);
QString convertmicrometretoinches(const QString& microMeter);

// convertion utility functions for SI units
QString convertMicrometreToMillimetre(const QString& microMeter);
QString convertMilliJoulesToJoules(const QString& milliJoules);
QString convertSecToMsec(const QString& time);
}

#endif // UTILITY_H
