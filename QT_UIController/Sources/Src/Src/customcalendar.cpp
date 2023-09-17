/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Setting custom calender months and years

***************************************************************************/

#include <QQmlContext>
#include<QDebug>
#include <QDate>
#include <QTime>
#include "Header/customcalendar.h"

#define CUSTOM_CALENDER_JAN             QObject::tr("Jan")
#define CUSTOM_CALENDER_FEB             QObject::tr("Feb")
#define CUSTOM_CALENDER_MAR             QObject::tr("Mar")
#define CUSTOM_CALENDER_APR             QObject::tr("Apr")
#define CUSTOM_CALENDER_MAY             QObject::tr("May")
#define CUSTOM_CALENDER_JUN             QObject::tr("Jun")
#define CUSTOM_CALENDER_JUL             QObject::tr("Jul")
#define CUSTOM_CALENDER_AUG             QObject::tr("Aug")
#define CUSTOM_CALENDER_SEP             QObject::tr("Sep")
#define CUSTOM_CALENDER_OCT             QObject::tr("Oct")
#define CUSTOM_CALENDER_NOV             QObject::tr("Nov")
#define CUSTOM_CALENDER_DEC             QObject::tr("Dec")

extern QQmlContext* context;

/**
 * @brief CustomCalendar::CustomCalendar : Constructor
 * @param parent
 */
CustomCalendar::CustomCalendar(QObject *parent) : QObject(parent)
{
}

/**
 * @brief CustomCalendar::getYearsList : Returns list of past 31 years from current year
 * @return
 */
QString CustomCalendar::getYearsList()
{
    QString yearsStr="";
    int startYr = CUSTOM_CALENDER_START_YEAR;
    QDate currentDate= QDate::currentDate();

    int endYr = ((int)currentDate.year()) - startYr + CUSTOM_CALENDER_NUM_OF_YEARS;

    for(int dwIndex = 1 ; dwIndex < endYr ;dwIndex++)
    {
        yearsStr.append(QString::number(startYr+dwIndex));
        if(dwIndex<endYr-1)
        {
            yearsStr.append(",");
        }
    }
    return yearsStr;
}

/**
 * @brief CustomCalendar::initializeMonthYearModel : Creates months and years model and updates in UI
 * @return
 */
void CustomCalendar::initializeMonthYearModel()
{
    QString parameterMsgYr="" ;
    QStringList a_parameterMsgListYr;
    QStringList a_parameterMsgListMth;

    context->setContextProperty("monthModel", QVariant::fromValue(a_parameterMsgListMth));
    a_parameterMsgListMth.clear();
    a_parameterMsgListMth.append(CUSTOM_CALENDER_JAN);
    a_parameterMsgListMth.append(CUSTOM_CALENDER_FEB);
    a_parameterMsgListMth.append(CUSTOM_CALENDER_MAR);
    a_parameterMsgListMth.append(CUSTOM_CALENDER_APR);
    a_parameterMsgListMth.append(CUSTOM_CALENDER_MAY);
    a_parameterMsgListMth.append(CUSTOM_CALENDER_JUN);
    a_parameterMsgListMth.append(CUSTOM_CALENDER_JUL);
    a_parameterMsgListMth.append(CUSTOM_CALENDER_AUG);
    a_parameterMsgListMth.append(CUSTOM_CALENDER_SEP);
    a_parameterMsgListMth.append(CUSTOM_CALENDER_OCT);
    a_parameterMsgListMth.append(CUSTOM_CALENDER_NOV);
    a_parameterMsgListMth.append(CUSTOM_CALENDER_DEC);

    context->setContextProperty("monthModel", QVariant::fromValue(a_parameterMsgListMth));

    context->setContextProperty("yearModel", QVariant::fromValue(a_parameterMsgListYr));
    a_parameterMsgListYr.clear();
    parameterMsgYr = getYearsList();
    a_parameterMsgListYr = parameterMsgYr.split(',');
    context->setContextProperty("yearModel", QVariant::fromValue(a_parameterMsgListYr));

}
