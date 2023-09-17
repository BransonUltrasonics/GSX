/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Reads dashboard data from DB and updates in UI

***************************************************************************/

#include "Header/dashboard.h"
#include <QQuickView>
#include <QQmlContext>
#include "Header/configuration.h"
#include "Header/common.h"

#include "Header/utility.h"
extern QQmlContext* context;
extern CommunicationInterface CIObj;
extern Recipe *recipe;
extern int dwCurrentUnit;

/**
 * @brief GetActiveRecipe : This function gets the ActiveRecipe info from Communicationinterface Class
 * @param qstrSupportedModes: have to specify the supported modes as , delimeter
 * @return QString
 */
QString GetActiveRecipe(QString qstrSupportedModes)
{
    CIObj.sendMessage(REQ_GET_ACTIVE_RECIPE, RES_GET_ACTIVE_RECIPE, qstrSupportedModes);
    QString a_activeRecipeBuffer = "";
    CIObj.recivedMessage(RES_GET_ACTIVE_RECIPE,a_activeRecipeBuffer);
    return a_activeRecipeBuffer;
}

/**
 * @brief GetProductionWeldResult : This function gets the Production Weld Result from Communicationinterface Class
 * @param qstrRecipeName
 * @return QString
 */
QString GetProductionWeldResult(QString qstrRecipeName)
{
    CIObj.sendMessage(REQ_GET_PRODUCTION_WELD_RESULT,RES_GET_PRODUCTION_WELD_RESULT,qstrRecipeName);
    QString a_productionWeldResultBuffer = "";
    CIObj.recivedMessage(RES_GET_PRODUCTION_WELD_RESULT,a_productionWeldResultBuffer);
    return a_productionWeldResultBuffer;
}

/**
 * @brief GetLastWeldResult : This function gets the last Weld Result from Communicationinterface Class
 * @param qstrRecipeNumber
 * @return QString
 */
QString GetLastWeldResult(QString qstrRecipeNumber)
{
    CIObj.sendMessage(REQ_GET_LAST_WELD_RESULT,RES_GET_LAST_WELD_RESULT,qstrRecipeNumber);
    QString a_lastWeldResultBuffer = "";
    CIObj.recivedMessage(RES_GET_LAST_WELD_RESULT,a_lastWeldResultBuffer);
    return a_lastWeldResultBuffer;
}

/**
 * @brief GetAlarmLog : This function gets the Alarm Logs from Communicationinterface Class
 * @param qstrRecipeName
 * @return QString
 */
QString GetAlarmLog(QString qstrRecipeName)
{
    QString a_alarmLogBuffer = "";
    if(Configuration::getInstance()->getFeatureFlagStatus(ALARM_LOG) == 1)
    {
        CIObj.sendMessage(REQ_GET_ALARM_LOG,RES_GET_ALARM_LOG,qstrRecipeName);
        CIObj.recivedMessage(RES_GET_ALARM_LOG,a_alarmLogBuffer);
    }
    return a_alarmLogBuffer;
}

/**
 * @brief DashBoard::DashBoard : Constructor
 * @param parent
 */
DashBoard::DashBoard(QObject *parent) : QObject(parent)
{
    bDashBoardOpenStatus = false;
    FillLastWeldResult();
    FillProductionWeldResult();
    m_isActiveValue = false;

    /* Connecting a signal dashboardUpdate from CommunicationInterface.cpp to dashboardUpdateInd slot */
    QObject::connect(&CIObj , &CommunicationInterface::dashboardUpdate , this , &DashBoard :: dashboardUpdateInd);
}

/**
 * @brief DashBoard::dashboardUpdateInd : Updates UI when signal received from Communication interface
 */
void DashBoard::dashboardUpdateInd()
{
    QString Null_String = "";
    QString dashboardLastWeldResult = GetLastWeldResult(Null_String);
    updateLastWeldData(dashboardLastWeldResult);

    QString dashboardProductionWeldResult = GetProductionWeldResult(m_recipeNumber);
    updateProductionWeldResult(dashboardProductionWeldResult);
    alarmLogUpdateInd();
}

/**
 * @brief DashBoard::alarmLogUpdateInd : Updates alarm logs in UI when signal received from Communication interface
 */
void DashBoard::alarmLogUpdateInd()
{
    /* Refreshing UI alarm logs */
    context->setContextProperty("alarmList1Model", QVariant::fromValue(alarmList));

    QString dashboardAlarmResult = GetAlarmLog(m_recipeNumber);
    updateAlarmsList(dashboardAlarmResult);

    if(alarmList1.size())
    {
        /* Refreshing UI alarm logs */
        context->setContextProperty("alarmList1Model", QVariant::fromValue(alarmList1));
    }
}

/**
 * @brief DashBoard::updateLastWeldData : Updates last weld results
 * @param dashboardLastWeldResult
 */
void DashBoard::updateLastWeldData(QString dashboardLastWeldResult)
{
    if (dashboardLastWeldResult.isEmpty())
    {
        dashboardLastWeldResult="0,0,0,0,0,0";
    }
    QStringList lastWeldResultBuffer = dashboardLastWeldResult.split(",");
    runtimeFillLastWeldResult(lastWeldResultBuffer);
}

/**
 * @brief DashBoard::updateProductionWeldResult : Updates production weld results on dashboard
 * @param dashboardProductionWeldResult
 */
void DashBoard::updateProductionWeldResult(QString dashboardProductionWeldResult)
{
    if (!dashboardProductionWeldResult.isEmpty())
    {
		QStringList productionResultBuffer = dashboardProductionWeldResult.split(",");
        if((productionResultBuffer.size()%8) == 0)
        {
            runtimeFillProductionWeldResult(productionResultBuffer);
        }
    }
}

/**
 * @brief DashBoard::updateAlarmsList : Updates alarm log on dashboard
 * @param dashboardAlarmLog
 */
void DashBoard::updateAlarmsList(QString dashboardAlarmLog)
{
    QStringList alarmBuffer = dashboardAlarmLog.split(",");
    if(!dashboardAlarmLog.isEmpty() && (alarmBuffer.size()%CUST_NAME_EXPECTED_ALARM_RESULTS)==0)
    {
        DashBoard::FillAlarmLogs(alarmBuffer);
        dashboardAlarmLog.clear();
    }
    else
    {
        clearAlarmList();
    }
}

/**
 * @brief DashBoard::runtimeFillProductionWeldResult : Updates production weld results on dashboard runtime
 * @param a_productionResultBuffer
 */
void DashBoard::runtimeFillProductionWeldResult( QStringList a_productionResultBuffer)
{
    ProductionResults* productionResults;
    int size;
    if(bDashBoardOpenStatus)
        context->setContextProperty("modelProdctionParams", QVariant::fromValue(m_productionListDummy));
    for(int dwIndex=0;dwIndex < a_productionResultBuffer.size()-4;dwIndex++)
    {
        productionResults = (ProductionResults*)m_productionList.at(dwIndex);
        if(dwIndex == 1)
            if( CIObj.queueWelds.size() >= 1)
                productionResults->m_ParameterValue = QString::number(CIObj.queueWelds.last() - CIObj.queueWelds.first());
            else
                productionResults->m_ParameterValue="0";
        else
            productionResults->m_ParameterValue=a_productionResultBuffer[dwIndex];

        size = productionResults->m_ParameterValue.size();
        if( size == VALUE_SIZE_1 || size == VALUE_SIZE_2 || size == VALUE_SIZE_3 )
        {
            productionResults->m_FontSize = FONT_SIZE_72;
        }
        else if( size == VALUE_SIZE_4 || size == VALUE_SIZE_5 )
        {
            productionResults->m_FontSize = FONT_SIZE_48;
        }
        else if( size == VALUE_SIZE_6 )
        {
            productionResults->m_FontSize = FONT_SIZE_42;
        }
        else if( size == VALUE_SIZE_7 || size == VALUE_SIZE_8 )
        {
            productionResults->m_FontSize = FONT_SIZE_34;
        }
    }
    /* If dahboard is open then only model refreshes*/
    if(bDashBoardOpenStatus)
        context->setContextProperty("modelProdctionParams", QVariant::fromValue(m_productionList));
}

/**
 * @brief DashBoard::refreshPartsPerMin : Refreshes parts per minute in UI
 */
void DashBoard::refreshPartsPerMin()
{
    if(bDashBoardOpenStatus)
        context->setContextProperty("modelProdctionParams", QVariant::fromValue(m_productionListDummy));
    if(m_productionList.size() == 4)
    {
        ProductionResults *productionResultsTemp = dynamic_cast<ProductionResults*>(m_productionList.at(1));
        if(CIObj.queueWelds.size() >= 1)
            productionResultsTemp->m_ParameterValue = QString::number(CIObj.queueWelds.last() - CIObj.queueWelds.first());
        else
            productionResultsTemp->m_ParameterValue = "0";
    }
    if(bDashBoardOpenStatus)
        context->setContextProperty("modelProdctionParams", QVariant::fromValue(m_productionList));
}

/**
 * @brief DashBoard::runtimeFillLastWeldResult : Updates last weld results on dashboard runtime
 * @param m_lastWeldResultBuffer
 */
void DashBoard::runtimeFillLastWeldResult ( QStringList m_lastWeldResultBuffer )
{
    double dTime;
    LastWeldResults* lastweldparameterresults;
    /* If dahboard is open then only model refreshes*/
    if(bDashBoardOpenStatus)
        context->setContextProperty("modelLastWeldParams", QVariant::fromValue(m_lastWeldListDummy));

    for(int dwIndex=0;dwIndex<m_lastWeldResultBuffer.size();dwIndex++)
    {
        lastweldparameterresults = (LastWeldResults*)m_lastWeldList.at(dwIndex);
        if(lastweldparameterresults->m_ParameterUnit == CUST_NAME_UINTSEC)
        {
            dTime = Configuration::getInstance()->convertMsecToSec(m_lastWeldResultBuffer[dwIndex].toDouble());
            lastweldparameterresults->m_ParameterValue.setNum(dTime,'f',3);
        }
        else if (lastweldparameterresults->m_ParameterUnit == CUST_NAME_UINTMilliMeter)
        {
            dTime = Configuration::getInstance()->convertMicrometreToMillimetre(m_lastWeldResultBuffer[dwIndex].toDouble());
            /*For default value precision for Collpse Distance and Absolute Distance on dashboard screen*/
            if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                lastweldparameterresults->m_ParameterValue.setNum(dTime,'f',2);
            }
            else
            {
                lastweldparameterresults->m_ParameterValue.setNum(dTime,'f',3);
            }
        }
        else if (lastweldparameterresults->m_ParameterUnit == CUST_NAME_UINTINCHES)
        {
            dTime = Configuration::getInstance()->convertmicrometretoinches(m_lastWeldResultBuffer[dwIndex].toDouble());
            lastweldparameterresults->m_ParameterValue.setNum(dTime,'f',4);
        }
        else if (lastweldparameterresults->m_ParameterUnit == CUST_NAME_UINTJOULES)
        {
            dTime = Utility::convertMilliJoulesToJoules(m_lastWeldResultBuffer[dwIndex].toDouble());
            lastweldparameterresults->m_ParameterValue.setNum(dTime,'f',1);
        }
        /*To display Force value to one decimal for imperial Unit on dashboard screen*/
        else if (lastweldparameterresults->m_ParameterUnit == CUST_NAME_UINTLBS)
        {
            double newtonValue = (m_lastWeldResultBuffer[dwIndex].toDouble())/10.0;
            dTime = Utility::convertNtolbs(newtonValue);
            lastweldparameterresults->m_ParameterValue.setNum(dTime,'f',1);
        }
        /*To display integer Force value for Metric Unit on dashboard screen*/
        else if (lastweldparameterresults->m_ParameterUnit == CUST_NAME_UNIT_NEWTON)
        {
            dTime = (m_lastWeldResultBuffer[dwIndex].toDouble())/10.0;
            lastweldparameterresults->m_ParameterValue = QString::number(round(dTime));
        }
        else
        {
            lastweldparameterresults->m_ParameterValue=m_lastWeldResultBuffer[dwIndex];
        }
    }
    /* If dahboard is open then only model refreshes*/
    if(bDashBoardOpenStatus)
        context->setContextProperty("modelLastWeldParams", QVariant::fromValue(m_lastWeldList));

}

/**
 * @brief DashBoard::GetSupportedModes : Returns runtime feature modes
 * @return QString
 */
QString DashBoard::GetSupportedModes()
{
    QString supportedModes="";

    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_TIME) == 1)
    {
        supportedModes.append("1,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ENERGY) == 1)
    {
        supportedModes.append("2,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_POWER) == 1)
    {
        supportedModes.append("3,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_GROUND_DETECT) == 1)
    {
        supportedModes.append("4,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ABSOLUTE_DISTANCE) == 1)
    {
        supportedModes.append("5,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_COLLAPSE_DISTANCE) == 1)
    {
        supportedModes.append("6,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_DYNAMIC) == 1)
    {
        supportedModes.append("7,");
    }

    supportedModes.remove(supportedModes.length()-1,1);
    return supportedModes;
}

/**
 * @brief DashBoard::init : Called when clickes on dashboard and reads data from DB
 */
void DashBoard::init()
{
    context->setContextProperty("alarmList1Model", QVariant::fromValue(alarmList));
    bool isActiveRecipeAvailable=false;
    QString supportedModes = GetSupportedModes();
    QString m_dashboardActive = GetActiveRecipe(supportedModes);
    QStringList m_activeBuffer = m_dashboardActive.split(",");
    if ( !m_dashboardActive.isEmpty() && (m_activeBuffer.size()%CUST_NAME_EXPECTED_RESULTS)==0)
    {
        isActiveRecipeAvailable = true;
        DashBoard::FillActiveRecipe(m_activeBuffer);
        m_dashboardActive.clear();
    }
    else
    {
        m_recipeNumber = CUST_NAME_NA;
        m_recipeName = CUST_NAME_NOACTIVE_RECIPE;
        m_company = CUST_NAME_NA;
        m_weldMode = CUST_NAME_NA;
        m_controlValueUnit = CUST_NAME_NA;
        m_controlValue = "0";
        m_isValidated = false;
        m_isLockedValue = false;
        m_isActiveValue = false;
    }

    QString dashboardProductionWeldResult="",dashboardLastWeldResult="",dashboardAlarmLog="";

    if(isActiveRecipeAvailable)
    {
        dashboardProductionWeldResult = GetProductionWeldResult(m_recipeNumber);
        dashboardLastWeldResult = GetLastWeldResult(m_recipeNumber);
        dashboardAlarmLog = GetAlarmLog(m_recipeNumber);
    }

    updateProductionWeldResult(dashboardProductionWeldResult);
    updateLastWeldData(dashboardLastWeldResult);
    updateAlarmsList(dashboardAlarmLog);

    if(alarmList1.size())
        context->setContextProperty("alarmList1Model", QVariant::fromValue(alarmList1));
}

/**
 * @brief DashBoard::FillActiveRecipe :This function fills the Active Recipe parameters, which needs to be displayed in qml
 * @param m_activeBuffer
 */
void DashBoard::FillActiveRecipe ( QStringList m_activeBuffer )
{
    double dTime;
    m_recipeNumber = m_activeBuffer[0];
    m_recipeName = m_activeBuffer[1];
    m_company = m_activeBuffer[2];
    int dwUnit = dwCurrentUnit;
    QString distanceUnit = "mm";
    if(dwUnit)
        distanceUnit = "in";
    if(recipe->getModifiedStatus(m_recipeNumber))
    {
        stModifiedRecipeInfo stTemp = recipe->getCurrentRecipeInfo();
        m_recipeName = stTemp.qstrRecipeName;
        m_weldMode = stTemp.qstrModeNameScreen;
        m_controlValueUnit = stTemp.qstrUnit;
        m_controlValue = stTemp.qstrModeValue;
    }
    else
    {
        m_weldMode = m_activeBuffer[3];
        if (m_weldMode == WELDMODE_TIME)
        {
            m_controlValueUnit = CUST_NAME_UINTSEC;
            m_weldMode = CUST_NAME_TIME;
        }
        else if (m_weldMode == WELDMODE_ENERGY)
        {
            m_weldMode = CUST_NAME_ENERGY;
            m_controlValueUnit = CUST_NAME_UINTJOULES;
        }
        else if (m_weldMode == WELDMODE_PEAK_POWER)
		{
            m_weldMode = CUST_NAME_PEAK;
            m_controlValueUnit = CUST_NAME_UINTWATTS;
        }
        else if (m_weldMode == WELDMODE_GROUND_DETECT)
		{
            m_weldMode=CUST_NAME_SCRUB;
            m_controlValueUnit = CUST_NAME_UINTSEC;
        }
        else if (m_weldMode == WELDMODE_ABS_DISTANCE)
		{
            m_weldMode=CUST_NAME_ABSOLUTE;
            m_controlValueUnit = distanceUnit;
        }
        else if (m_weldMode == WELDMODE_COL_DISTANCE)
		{
            m_weldMode=CUST_NAME_COLLAPSE;
            m_controlValueUnit = distanceUnit;
        }
        else if (m_weldMode == WELDMODE_DYNAMIC)
       {
           m_weldMode=CUST_NAME_DYNAMIC;
           m_controlValueUnit = CUST_NAME_UINTPERCENTAGE;
       }
        if (m_controlValueUnit == CUST_NAME_UINTSEC)
        {
            dTime = Configuration::getInstance()->convertMsecToSec(m_activeBuffer[4].toDouble());
            m_controlValue.setNum(dTime,'f',3);
        }
        else if( m_controlValueUnit == CUST_NAME_UINTMilliMeter)
        {
            dTime = Configuration::getInstance()->convertMicrometreToMillimetre(m_activeBuffer[4].toDouble());
            /*For default value precision for Collpse Distance and Absolute Distance on dashboard screen*/
            if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                m_controlValue.setNum(dTime,'f',2);
            }

            else
            {
                m_controlValue.setNum(dTime,'f',3);
            }
        }
        else if( m_controlValueUnit == CUST_NAME_UINTINCHES)
        {
            dTime = Configuration::getInstance()->convertmicrometretoinches(m_activeBuffer[4].toDouble());
            m_controlValue.setNum(dTime,'f',4);
        }
        else if( m_controlValueUnit == CUST_NAME_UINTJOULES)
        {
            dTime = Utility::convertMilliJoulesToJoules(m_activeBuffer[4].toDouble());
            m_controlValue.setNum(dTime,'f',1);
        }
        else
        {
            m_controlValue = m_activeBuffer[4];
        }
    }

    m_isActiveValue = m_activeBuffer[5].toInt();
    m_isLockedValue = m_activeBuffer[6].toInt();
    m_isValidated = m_activeBuffer[7].toInt();
}

/**
 * @brief DashBoard::FillProductionWeldResult :This function fills the production results parameters, which needs to be displayed in qml
 */
void DashBoard::FillProductionWeldResult ()
{
    clearProductionList();
    if(bDashBoardOpenStatus)
        context->setContextProperty("modelProdctionParams", QVariant::fromValue(m_productionListDummy));
    QStringList productionParamNameList;
    productionParamNameList.append(DASH_CYCLESOK);
    productionParamNameList.append(DASH_PARTSMIN);
    productionParamNameList.append(DASH_REJECT);
    productionParamNameList.append(DASH_SUSPECT);

    QString productionParamColor = getProductionParamsColor();
    QStringList productionParamColorList = productionParamColor.split(",");
    for(int dwIndex = 0; dwIndex < productionParamNameList.size(); dwIndex++)
    {
        ProductionResults* productionParamNameResult = new ProductionResults;
        productionParamNameResult->m_ParameterName = productionParamNameList[dwIndex];
        productionParamNameResult->m_ParameterColor = productionParamColorList[dwIndex];
        productionParamNameResult->m_ParameterValue = CUST_NAME_NA;
        productionParamNameResult->m_FontSize = FONT_SIZE_72;
        m_productionList.append(productionParamNameResult);
    }
    if(bDashBoardOpenStatus)
        context->setContextProperty("modelProdctionParams", QVariant::fromValue(m_productionList));
}

/**
 * @brief DashBoard::FillLastWeldResult : This function fills the last weld results parameters, which needs to be displayed in qml
 */
void DashBoard::FillLastWeldResult ()
{
    context->setContextProperty("modelLastWeldParams", QVariant::fromValue(m_lastWeldListDummy));
    clearLastWeldList();
    QStringList LastWeldParamlist;
    LastWeldParamlist.append(CUST_NAME_WELD_TIME);
    LastWeldParamlist.append(CUST_NAME_WELD_FORCE);
    LastWeldParamlist.append(CUST_NAME_WELD_ENERGY);
    LastWeldParamlist.append(CUST_NAME_HOLD_FORCE);
    LastWeldParamlist.append(CUST_NAME_COLLAPSE_DIS);
    LastWeldParamlist.append(CUST_NAME_ABSOLUTE_DIS);
    QString lastWeldUnits;
    int dwUnit = dwCurrentUnit;

    if(dwUnit)
        lastWeldUnits = getLastWeldImperialUnits();
    else
        lastWeldUnits = getLastWeldUnits();
    QStringList LastWeldUnitlist = lastWeldUnits.split(",");
    for(int dwIndex = 0; dwIndex < LastWeldParamlist.size(); dwIndex++)
    {
        LastWeldResults* lastweldparameterresults = new LastWeldResults;
        lastweldparameterresults->m_ParameterName = LastWeldParamlist[dwIndex];
        lastweldparameterresults->m_ParameterUnit = LastWeldUnitlist[dwIndex];
        lastweldparameterresults->m_ParameterValue = "";
        m_lastWeldList.append(lastweldparameterresults);
    }

    if(bDashBoardOpenStatus)
        context->setContextProperty("modelLastWeldParams", QVariant::fromValue(m_lastWeldList));
}

/**
 * @brief DashBoard::FillAlarmLogs  : This function fills the alarm logs parameters, which needs to be displayed in qml
 * @param m_alarmBuffer
 */
void DashBoard::FillAlarmLogs ( QStringList m_alarmBuffer )
{
    clearAlarmList();
    QDate m_alarmbufDate;
    QTime m_alarmbufTime;
    QRegExp separator(" ");
    int dwIndex,listIndex,size = 0;
    size = m_alarmBuffer.size();
    for (dwIndex = 0,listIndex = 0;dwIndex < size; listIndex++)
    {
        m_listAlarmId << m_alarmBuffer[dwIndex];
        m_listAlarmType << m_alarmBuffer[dwIndex+1];
        QStringList firstList = m_alarmBuffer[dwIndex+2].split(separator);
        m_alarmbufTime = QTime::fromString(firstList.takeLast(),"HH:mm:ss");
        m_alarmbufDate = QDate::fromString(firstList.takeFirst(),"yyyy/MM/dd");
        if (m_listAlarmType[listIndex] == "1"){
            m_listAlarmType[listIndex] = CUST_NAME_PART_ICON_IMG;
        }
        else{
            m_listAlarmType[listIndex] = CUST_NAME_MACHINE_IMG;
        }
        alarmList1.append(new DashBoard(m_listAlarmId[listIndex],m_listAlarmType[listIndex], m_alarmbufTime,m_alarmbufDate ));
        dwIndex = dwIndex+3;
    }
}

/**
 * @brief DashBoard::getLastWeldUnits : Returns last weld results CGS units
 * @return QString
 */
QString DashBoard::getLastWeldUnits()
{
    QString lastWeldUnits;
    lastWeldUnits = "s,N,J,N,mm,mm";
    return lastWeldUnits;
}

/**
 * @brief DashBoard::getLastWeldImperialUnits : Returns last weld results Imperial units
 * @return QString
 */
QString DashBoard::getLastWeldImperialUnits()
{
    QString lastWeldUnits;
    lastWeldUnits = "s,lbs,J,lbs,in,in";
    return lastWeldUnits;
}

/**
 * @brief DashBoard::getProductionParamsColor : Returns production weld rectangle label color
 * @return QString
 */
QString DashBoard::getProductionParamsColor()
{
    QString productionParamsColor;
    productionParamsColor = "#8BC24A,#898D96,#F03E3E,#F59F00";
    return productionParamsColor;
}

/**
 * @brief DashBoard::clearAlarmList : Clears all the alarms in the list
 */
void DashBoard::clearAlarmList()
{
    for(int element=0; element <alarmList1.size() ; element++)
    {
        DashBoard *dashboardTemp = dynamic_cast<DashBoard*>(alarmList1.at(element));
        delete dashboardTemp;
        dashboardTemp = NULL;
    }
    alarmList1.clear();
    m_listAlarmId.clear();
    m_listAlarmType.clear();
}

/**
 * @brief DashBoard::clearLastWeldList : Clears all the last weld results in the list
 */
void DashBoard::clearLastWeldList()
{
    for(int element=0; element <m_lastWeldList.size() ; element++)
    {
        LastWeldResults *weldresultsTemp = dynamic_cast<LastWeldResults*>(m_lastWeldList.at(element));
        delete weldresultsTemp;
        weldresultsTemp = NULL;
    }
    m_lastWeldList.clear();
}

/**
 * @brief DashBoard::clearProductionList : Clears all the production results in the list
 */
void DashBoard::clearProductionList()
{
    for(int element=0; element <m_productionList.size() ; element++)
    {
        ProductionResults *productionresTemp = dynamic_cast<ProductionResults*>(m_productionList.at(element));
        delete productionresTemp;
        productionresTemp = NULL;
    }
    m_productionList.clear();
}

/**
 * @brief DashBoard::DashBoard : Initialization of class members and memory for object
 * @param AlarmId
 * @param AlarmType
 * @param AlarmTime
 * @param AlarmDate
 * @param parent
 */
DashBoard::DashBoard(const QString &AlarmId,const QString &AlarmType,const QTime &AlarmTime, const QDate &AlarmDate,QObject *parent)
    : QObject(parent), m_alarmId(AlarmId),m_alarmType(AlarmType), m_alarmTime(AlarmTime),m_alarmDate(AlarmDate)
{

}

/**
 * @brief DashBoard::GetRecipeNumber : Returns active recipe number
 * @return QString
 */
QString DashBoard::GetRecipeNumber()
{
    return m_recipeNumber;
}

/**
 * @brief DashBoard::GetRecipeName : Returns active recipe name
 * @return QString
 */
QString DashBoard::GetRecipeName()
{
    return m_recipeName;
}

/**
 * @brief DashBoard::GetCompany : Returns company name
 * @return QString
 */
QString DashBoard::GetCompany()
{
    return m_company;
}

/**
 * @brief DashBoard::GetWeldMode : Returns type of weld mode
 * @return QString
 */
QString DashBoard::GetWeldMode()
{
    return m_weldMode;
}

/**
 * @brief DashBoard::GetControlValue : Returns mode value
 * @return QString
 */
QString DashBoard::GetControlValue()
{
    return m_controlValue;
}

/**
 * @brief DashBoard::GetControlValueUnit : Returns mode unit
 * @return QString
 */
QString DashBoard::GetControlValueUnit()
{
    return m_controlValueUnit;
}

/**
 * @brief DashBoard::GetIsActiveValue : Returns mode value
 * @return QString
 */
bool  DashBoard::GetIsActiveValue()
{
    return m_isActiveValue;
}

/**
 * @brief DashBoard::GetIsLockedValue : Returns whether recipe is locked or not
 * @return QString
 */
bool  DashBoard::GetIsLockedValue()
{
    return m_isLockedValue;
}

/**
 * @brief DashBoard::GetIsValidated : Returns whether recipe is validated or not
 * @return QString
 */
bool  DashBoard::GetIsValidated()
{
    return m_isValidated;
}

/**
 * @brief DashBoard::GetGoodNo : Returns good number of welds
 * @return QString
 */
QString DashBoard::GetGoodNo()
{
    return m_goodNo;
}

/**
 * @brief DashBoard::GetThroughPutNo : Returns throughput
 * @return QString
 */
QString DashBoard::GetThroughPutNo()
{
    return m_throughPutNo;
}

/**
 * @brief DashBoard::GetRejectNo : Returns rejected number of welds
 * @return QString
 */
QString DashBoard::GetRejectNo()    {
    return m_rejectNo;
}

/**
 * @brief DashBoard::GetSuspectNo : Returns suspected number of welds
 * @return QString
 */
QString DashBoard::GetSuspectNo()
{
    return m_suspectNo;
}

/**
 * @brief DashBoard::GetAlarmId  : Returns alarm ID
 * @return QString
 */
QString DashBoard::GetAlarmId() const
{
    return m_alarmId;
}

/**
 * @brief DashBoard::GetAlarmType : Returns alarm type
 * @return QString
 */
QString DashBoard::GetAlarmType() const
{
    return m_alarmType;
}

/**
 * @brief DashBoard::GetAlarmTime : Returns alarm time
 * @return QString
 */
QString DashBoard::GetAlarmTime()
{
    return m_alarmTime.toString("hh:mm");
}

/**
 * @brief DashBoard::GetAlarmDate : Returns alarm date
 * @return QString
 */
QString DashBoard::GetAlarmDate()
{
    return m_alarmDate.toString("dd/MM/yyyy");
}

/**
 * @brief DashBoard::setDashboardOpenStatus : Sets the status dashboard, whether it is opened or not
 * @param bStatus
 */
void DashBoard::setDashboardOpenStatus(bool bStatus)
{
    bDashBoardOpenStatus = bStatus;
    if(bDashBoardOpenStatus)
    {
        context->setContextProperty("modelLastWeldParams", QVariant::fromValue(m_lastWeldListDummy));
        context->setContextProperty("modelLastWeldParams", QVariant::fromValue(m_lastWeldList));
        context->setContextProperty("modelProdctionParams", QVariant::fromValue(m_productionListDummy));
        context->setContextProperty("modelProdctionParams", QVariant::fromValue(m_productionList));
    }
}
