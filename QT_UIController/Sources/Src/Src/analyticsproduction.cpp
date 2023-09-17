/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Reads production details from DB and updates in the UI as graph

***************************************************************************/

#include <Header/analyticsproduction.h>
#include "Header/communicationinterface.h"
#include "Header/configuration.h"
#include <QDateTime>
#include <QQmlContext>

extern QQmlContext* context;
extern CommunicationInterface CIObj;

/**
 * @brief AnalyticsProdcution::AnalyticsProdcution : Initialization of class members and memory for object
 * @param : QString productionName
 * @param : QString sunVal
 * @param : QString monVal
 * @param : QString tueVal
 * @param : QString wedVal
 * @param : QString thrVal
 * @param : QString friVal
 * @param : QString satVal
 * @param : QString productionUnit
 * @param : QString productionBarColor
 * @param : QString avgVal
 */
AnalyticsProdcution::AnalyticsProdcution(QString productionName, QString sunVal, QString monVal, QString tueVal, QString wedVal, QString thrVal, QString friVal, QString satVal, QString productionUnit, QString productionBarColor,QString avgVal)
{
    m_productionName = productionName;
    m_sunVal = sunVal;
    m_monVal = monVal;
    m_tueVal = tueVal;
    m_wedVal = wedVal;
    m_thrVal = thrVal;
    m_friVal = friVal;
    m_satVal = satVal;
    m_productionUnit = productionUnit;
    m_productionBarColor = productionBarColor;
    m_avgValue = avgVal;
}

/**
 * @brief AnalyticsProdcution::AnalyticsProdcution : Constructor
 * @param parent
 */
AnalyticsProdcution::AnalyticsProdcution(QObject *parent): QObject(parent)
{
}

/**
 * @brief AnalyticsProdcution::getproductionName : Returns type of production
 * @return : QString
 */
QString AnalyticsProdcution::getproductionName()
{
    return m_productionName;
}

/**
 * @brief AnalyticsProdcution::getsunVal : Returns value on sunday
 * @return : QString
 */
QString AnalyticsProdcution::getsunVal()
{
    return m_sunVal;
}

/**
 * @brief AnalyticsProdcution::getmonVal : Returns value on monday
 * @return : QString
 */
QString AnalyticsProdcution::getmonVal()
{
    return m_monVal;
}

/**
 * @brief AnalyticsProdcution::gettueVal : Returns value on tuesday
 * @return : QString
 */
QString AnalyticsProdcution::gettueVal()
{
    return m_tueVal;
}

/**
 * @brief AnalyticsProdcution::getwedVal : Returns value on Wednesday
 * @return : QString
 */
QString AnalyticsProdcution::getwedVal()
{
    return m_wedVal;
}

/**
 * @brief AnalyticsProdcution::getthrVal : Returns value on thursday
 * @return : QString
 */
QString AnalyticsProdcution::getthrVal()
{
    return m_thrVal;
}

/**
 * @brief AnalyticsProdcution::getfriVal : Returns value on friday
 * @return : QString
 */
QString AnalyticsProdcution::getfriVal()
{
    return m_friVal;
}

/**
 * @brief AnalyticsProdcution::getsatVal : Returns value on saturday
 * @return : QString
 */
QString AnalyticsProdcution::getsatVal()
{
    return m_satVal;
}

/**
 * @brief AnalyticsProdcution::getproductionUnit : Returns production unit
 * @return : QString
 */
QString AnalyticsProdcution::getproductionUnit()
{
    return m_productionUnit;
}

/**
 * @brief AnalyticsProdcution::getproductionBarColor : Returns production bar color
 * @return : QString
 */
QString AnalyticsProdcution::getproductionBarColor()
{
    return m_productionBarColor;
}

/**
 * @brief AnalyticsProdcution::getavgVal : Returns average value
 * @return : QString
 */
QString AnalyticsProdcution::getavgVal()
{
    return m_avgValue;
}

/**
 * @brief AnalyticsProdcution::setAnalyticsDateProductionList : Updates production details based date selected
 * @param : QString fromDateStr
 * @param : QStringtoDateStr
 */
void AnalyticsProdcution::setAnalyticsDateProductionList(QString fromDateStr,QString toDateStr)
{
    QString sendstr =fromDateStr+","+ toDateStr;
    QString productionResult = sendDateParams(sendstr);

    if(!productionResult.isEmpty())
    {
        updateAnalyticsProductiondetails(productionResult);
    }
    else
    {
        clearList();
        defaultAnalyticsProductiondetails();
    }
}

/**
 * @brief AnalyticsProdcution::startDateofCurrentWeek : Returns start date of the current week
 * @return : QString
 */
QString AnalyticsProdcution::startDateofCurrentWeek()
{
    QDate cd = QDate::currentDate();
    int wd = cd.dayOfWeek();
    auto startDayOfWeek = cd.addDays(1 - wd);
    return startDayOfWeek.toString(Qt::ISODate);
}

/**
 * @brief AnalyticsProdcution::endDateofCurrentWeek : Returns end date of the current week
 * @return : QString
 */
QString AnalyticsProdcution::endDateofCurrentWeek()
{
    QDate cd = QDate::currentDate();
    int wd = cd.dayOfWeek();
    // To get the start date of teh week
    auto startDayOfWeek = cd.addDays(1 - wd);
    auto endDayOfWeek = startDayOfWeek.addDays(6);
    return endDayOfWeek.toString(Qt::ISODate);
}

/**
 * @brief AnalyticsProdcution::startDateofWeek : Returns start date of particular week
 * @param : QString startdate
 * @return : QString
 */
QString AnalyticsProdcution::startDateofWeek(QString startdate)
{
    QDate cd =  QDate::fromString(startdate,Qt::ISODate);
    int wd = cd.dayOfWeek();
    // To get the start date of teh week
    auto startDayOfWeek = cd.addDays(1 - wd);
    return startDayOfWeek.toString(Qt::ISODate);
}

/**
 * @brief AnalyticsProdcution::endDateofWeek : Returns end date of particular week
 * @param : QString enddate
 * @return : QString
 */
QString AnalyticsProdcution::endDateofWeek(QString enddate)
{
    QDate cd =  QDate::fromString(enddate,Qt::ISODate);
    int wd = cd.dayOfWeek();
    // To get the start date of teh week
    auto startDayOfWeek = cd.addDays(1 - wd);
    auto endDayOfWeek = startDayOfWeek.addDays(6);
    return endDayOfWeek.toString(Qt::ISODate);
}

/**
 * @brief AnalyticsProdcution::sendDateParams :  Returns production details based on selected dates
 * @param  : QString buffer
 * @return : QString
 */
QString AnalyticsProdcution::sendDateParams(QString buffer)
{
    QString a_dateBuffer="";

    /* Send message to DB */
    CIObj.sendMessage(REQ_SET_PRODUCT_DATE_PARAM,RES_SET_PRODUCT_DATE_PARAM,buffer);
    CIObj.recivedMessage(RES_SET_PRODUCT_DATE_PARAM,a_dateBuffer);

    return a_dateBuffer;
}

/**
 * @brief AnalyticsProdcution::updateAnalyticsProductiondetails : Updates production details read from DB to UI
 * @param : QString results
 */
void AnalyticsProdcution::updateAnalyticsProductiondetails(QString results)
{
    clearList();
    QStringList productionBuffer = results.split(PRODUCTION_OVR_SPLITTER);
    QString productionName,productionUnit,productionBarColor,avgVal;
    QString sunVal=0,monVal=0,tueVal=0,wedVal=0,thrVal=0,friVal=0,satVal=0;

    if(!productionBuffer.isEmpty())
    {
        int dwIndex,size=0;
        QString productionAvgCount;
        QList<QString>m_productionCountList;
        size = productionBuffer.size();
        float average =0;
        float sum =0;
        int totalSize = size/2;

        for (dwIndex=0;dwIndex<size;dwIndex++)
        {
            productionAvgCount = productionBuffer[dwIndex+1];
            m_productionCountList.append(productionAvgCount);
            sum = productionAvgCount.toFloat();
            average = average + sum;
            dwIndex++;
        }

        average = average/totalSize;
        avgVal = QVariant(average).toString();

        int currentSize=m_productionCountList.size();

        switch (currentSize)
        {
        case 0:
            break;
        case 1:
            sunVal = m_productionCountList.at(0);
            break;
        case 2:
            sunVal = m_productionCountList.at(0);
            monVal = m_productionCountList.at(1);
            break;
        case 3:
            sunVal = m_productionCountList.at(0);
            monVal = m_productionCountList.at(1);
            tueVal = m_productionCountList.at(2);
            break;
        case 4:
            sunVal = m_productionCountList.at(0);
            monVal = m_productionCountList.at(1);
            tueVal = m_productionCountList.at(2);
            wedVal = m_productionCountList.at(3);
            break;
        case 5:
            sunVal = m_productionCountList.at(0);
            monVal = m_productionCountList.at(1);
            tueVal = m_productionCountList.at(2);
            wedVal = m_productionCountList.at(3);
            thrVal = m_productionCountList.at(4);
            break;
        case 6:
            sunVal = m_productionCountList.at(0);
            monVal = m_productionCountList.at(1);
            tueVal = m_productionCountList.at(2);
            wedVal = m_productionCountList.at(3);
            thrVal = m_productionCountList.at(4);
            friVal = m_productionCountList.at(5);
            break;
        case 7:
            sunVal = m_productionCountList.at(0);
            monVal = m_productionCountList.at(1);
            tueVal = m_productionCountList.at(2);
            wedVal = m_productionCountList.at(3);
            thrVal = m_productionCountList.at(4);
            friVal = m_productionCountList.at(5);
            satVal = m_productionCountList.at(6);
            break;
        default:
            break;
        }

        for (int dwIndex = 0;dwIndex <= 2;dwIndex++)
        {
            if(dwIndex==0)
            {
                productionName = PRODUCTION_OVR_NAME_WELD;
                productionUnit = PRODUCTION_OVR_UNIT_RLIMITS;
                productionBarColor = PRODUCTION_OVR_BAR_COLOR_WELD;
            }
            else if(dwIndex==1)
            {
                productionName = PRODUCTION_OVR_NAME_ALARMS;
                productionUnit = PRODUCTION_OVR_UNIT_ERRORS;
                productionBarColor = PRODUCTION_OVR_BAR_COLOR_ALARMS;
            }
            else if(dwIndex==2)
            {
                productionName = PRODUCTION_OVR_NAME_RATE;
                productionUnit = PRODUCTION_OVR_UNIT_PARTPERMIN;
                productionBarColor = PRODUCTION_OVR_BAR_COLOR_RATE;
            }

            m_productionList.append(new AnalyticsProdcution(productionName,sunVal,monVal,tueVal,wedVal,thrVal,friVal,satVal,productionUnit,productionBarColor,avgVal));
        }

        if(m_productionList.size())
        {
            /* Updating UI production overview model*/
            context->setContextProperty("productionModel", QVariant::fromValue(m_productionList));
        }
    }
}

/**
 * @brief AnalyticsProdcution::defaultAnalyticsProductiondetails : Updating production model with default values
 */
void AnalyticsProdcution::defaultAnalyticsProductiondetails()
{
    QString productionName,productionUnit,productionBarColor,avgVal;
    QString sunVal=0,monVal=0,tueVal=0,wedVal=0,thrVal=0,friVal=0,satVal=0;
    avgVal ="0";

    for (int i=0;i<=2;i++)
    {
        if(i==0)
        {
            productionName = PRODUCTION_OVR_NAME_WELD;
            productionUnit = PRODUCTION_OVR_UNIT_RLIMITS;
            productionBarColor = PRODUCTION_OVR_BAR_COLOR_WELD;
        }
        else if(i==1)
        {
            productionName = PRODUCTION_OVR_NAME_ALARMS;
            productionUnit = PRODUCTION_OVR_UNIT_ERRORS;
            productionBarColor = PRODUCTION_OVR_BAR_COLOR_ALARMS;
        }
        else if(i==2)
        {
            productionName = PRODUCTION_OVR_NAME_RATE;
            productionUnit = PRODUCTION_OVR_UNIT_PARTPERMIN;
            productionBarColor = PRODUCTION_OVR_BAR_COLOR_RATE;
        }
        //adding all the weekd days to teh model to show in UI
        m_productionList.append(new AnalyticsProdcution(productionName,sunVal,monVal,tueVal,wedVal,thrVal,friVal,satVal,productionUnit,productionBarColor,avgVal));
    }

    /* Updating UI production model */
    context->setContextProperty("productionModel", QVariant::fromValue(m_productionList));
}

/**
 * @brief AnalyticsProdcution::clickNextWeek : Returns next week when clicks on '>' in UI
 * @param : QString nextday
 * @return : QString
 */
QString AnalyticsProdcution::clickNextWeek(QString nextday)
{
    QDate date = QDate::fromString(nextday,Qt::ISODate);
    auto nextDayOfWeek = date.addDays(7);
    return nextDayOfWeek.toString(Qt::ISODate);
}

/**
 * @brief AnalyticsProdcution::clickPreviousWeek : Returns next week when clicks on '<' in UI
 * @param : QString previousday
 * @return : QString
 */
QString AnalyticsProdcution::clickPreviousWeek(QString previousday)
{
    QDate date = QDate::fromString(previousday,Qt::ISODate);
    //Getting the previous week dates for the data
    auto previousDayOfWeek = date.addDays(-7);
    return previousDayOfWeek.toString(Qt::ISODate);
}

/**
 * @brief AnalyticsProdcution::clearList : Clears production list model
 */
void AnalyticsProdcution::clearList()
{
    if (!m_productionList.empty())
    {
        auto tempList = m_productionList;
        m_productionList.clear();
        context->setContextProperty("productionModel", QVariant::fromValue(m_productionList));
        qDeleteAll(tempList);
        tempList.clear();
    }
}
