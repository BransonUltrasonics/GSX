/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    It is used to get and display weld results in table view

***************************************************************************/

#include <QQmlContext>
#include <QSettings>
#include "Header/graphtableview.h"
#include "Header/utility.h"

extern CommunicationInterface CIObj;
extern QQmlContext* context;
extern int dwCurrentUnit;

#define GRAPH_RECIPE_NUMBER					QObject::tr("Recipe #")
#define GRAPH_VERSION_NUMBER				QObject::tr("Recipe Version #")
#define GRAPH_DATE_TIME						QObject::tr("Date Time")
#define GRAPH_STACKSERIALNO					QObject::tr("Stack Serial #")
#define GRAPH_CYCLE_COUNTER					QObject::tr("Cycle Counter")
#define GRAPH_WELD_MODE						QObject::tr("Weld Mode")
#define GRAPH_MAX_WELD_FORCE				QObject::tr("Max Weld Force")
#define GRAPH_END_HOLD_FORCE				QObject::tr("End Hold Force")
#define GRAPH_WELD_ABSOLUTE					QObject::tr("Weld Absolute")
#define GRAPH_TOTAL_ABSOLUTE				QObject::tr("Total Absolute")
#define GRAPH_WELD_COLLAPSE_DISTANCE		QObject::tr("Weld Collapse Dis")
#define GRAPH_HOLD_COLLAPSE_DISTANCE		QObject::tr("Hold Collapse Dis")
#define GRAPH_TOTAL_COLLAPSE_DISTANCE		QObject::tr("Total Collapse Dis")
#define GRAPH_TRIGGER_DISTANCE				QObject::tr("Trigger Distance")
#define GRAPH_VELOCITY						QObject::tr("Velocity")
#define GRAPH_WELD_TIME						QObject::tr("Weld Time")
#define GRAPH_WELD_ENERGY					QObject::tr("Weld Energy")
#define GRAPH_WELD_PEAK_POWER				QObject::tr("Peak Power")
#define GRAPH_START_FREQ					QObject::tr("Start Frequency")
#define GRAPH_FREQ_CHANGE					QObject::tr("Frequency Change")
#define GRAPH_TOTAL_CYCLE_TIME				QObject::tr("Total Cycle Time")
#define GRAPH_USERNAME						QObject::tr("UserName")
#define GRAPH_PARTID						QObject::tr("Part ID")
#define GRAPH_BATCHID						QObject::tr("Batch ID")
#define GRAPH_ALARM_FLAG					QObject::tr("Alarm Raised")
#define GRAPH_TRIGGER_POINT					QObject::tr("Trigger Point")
#define GRAPH_WELD_SONIC_POINT				QObject::tr("Weld Sonic Point")
#define GRAPH_HOLD_POINT					QObject::tr("Hold Point")
#define GRAPH_RECIPE_STATUS                 QObject::tr("Recipe Status")

#define WELD_MODE_TIME                      QObject::tr("Time")
#define WELD_MODE_PEAK_POWER                QObject::tr("Peak Power")
#define WELD_MODE_ABS_DIS                   QObject::tr("Absolute Distance")
#define WELD_MODE_COL_DIS                   QObject::tr("Collapse Distance")
#define WELD_MODE_GND_DET                   QObject::tr("Ground Detect")
#define WELD_MODE_GND_DET_VAL               QObject::tr("Scrub Time")
#define WELD_MODE_ENERGY                    QObject::tr("Energy")
#define WELD_MODE_DYNAMIC                   QObject::tr("Dynamic")
#define WELD_MODE_MULTI                     QObject::tr("MULTI Mode")

#define RECIPE_STATUS_SAVED                 QObject::tr("Saved")
#define RECIPE_STATUS_UNSAVED               QObject::tr("Unsaved")
#define RECIPE_STATUS_VALIDATED             QObject::tr("Validated")
#define RECIPE_STATUS_INVALIDATED           QObject::tr("Invalidated")
#define RECIPE_STATUS_NODEFINE              QObject::tr("No Define")

/**
 * @brief WeldModeTypeEnumToString : Returns mode type based on enum
 * @param weldModeValue
 * @return
 */
QString WeldModeTypeEnumToString(WeldModeType weldModeValue)
{
    QString enumString = "";

    switch (weldModeValue) {
    case WeldModeType::TIME:
        enumString = WELD_MODE_TIME;
        break;
    case WeldModeType::ENERGY:
        enumString = WELD_MODE_ENERGY;
        break;
    case WeldModeType::PEAK_POWER:
        enumString = WELD_MODE_PEAK_POWER;
        break;
    case WeldModeType::GND_DET_VAL:
        enumString = WELD_MODE_GND_DET_VAL;
        break;
    case WeldModeType::ABS_DIS:
        enumString = WELD_MODE_ABS_DIS;
        break;
    case WeldModeType::COL_DIS:
        enumString = WELD_MODE_COL_DIS;
        break;
    case WeldModeType::DYNAMIC:
        enumString = WELD_MODE_DYNAMIC;
        break;
    default:
        enumString = WELD_MODE_MULTI;
        break;
    }

    return enumString;
}

/**
 * @brief RecipeStatusTypeEnumToString
 * @param recipeStatusValue
 * @return
 */
QString RecipeStatusTypeEnumToString(RecipeStatusType recipeStatusValue)
{
    QString enumString = "";

    switch (recipeStatusValue) {
    case RecipeStatusType::SAVED:
        enumString = RECIPE_STATUS_SAVED;
        break;
    case RecipeStatusType::UNSAVED:
        enumString = RECIPE_STATUS_UNSAVED;
        break;
    case RecipeStatusType::VALIDATED:
        enumString = RECIPE_STATUS_VALIDATED;
        break;
    case RecipeStatusType::INVALIDATED:
        enumString = RECIPE_STATUS_INVALIDATED;
        break;
    default:
        enumString = RECIPE_STATUS_NODEFINE;
        break;
    }

    return enumString;
}

/**
 * @brief GraphtableEnumToString : Returns weld result parameter based on enum
 * @param enumValue
 * @return
 */
QString GraphtableEnumToString(GraphtableEnum enumValue)
{
    QString enumString = "";

    switch (enumValue)
    {
    case GraphtableEnum::RECIPE_NUMBER:
        enumString = GRAPH_RECIPE_NUMBER;
        break;
    case GraphtableEnum::VERSION_NUMBER:
        enumString = GRAPH_VERSION_NUMBER;
        break;
    case GraphtableEnum::DATE_TIME:
        enumString = GRAPH_DATE_TIME;
        break;
    case GraphtableEnum::CYCLE_COUNTER:
        enumString = GRAPH_CYCLE_COUNTER;
        break;
    case GraphtableEnum::WELD_MODE:
        enumString = GRAPH_WELD_MODE;
        break;
    case GraphtableEnum::MAX_WELD_FORCE:
        enumString = GRAPH_MAX_WELD_FORCE;
        break;
    case GraphtableEnum::END_HOLD_FORCE:
        enumString = GRAPH_END_HOLD_FORCE;
        break;
    case GraphtableEnum::WELD_ABSOLUTE:
        enumString = GRAPH_WELD_ABSOLUTE;
        break;
    case GraphtableEnum::TOTAL_ABSOLUTE:
        enumString = GRAPH_TOTAL_ABSOLUTE;
        break;
    case GraphtableEnum::WELD_COLLAPSE_DISTANCE:
        enumString = GRAPH_WELD_COLLAPSE_DISTANCE;
        break;
    case GraphtableEnum::HOLD_COLLAPSE_DISTANCE:
        enumString = GRAPH_HOLD_COLLAPSE_DISTANCE;
        break;
    case GraphtableEnum::TOTAL_COLLAPSE_DISTANCE:
        enumString = GRAPH_TOTAL_COLLAPSE_DISTANCE;
        break;
    case GraphtableEnum::TRIGGER_DISTANCE:
        enumString = GRAPH_TRIGGER_DISTANCE;
        break;
    case GraphtableEnum::WELD_TIME:
        enumString = GRAPH_WELD_TIME;
        break;
    case GraphtableEnum::WELD_ENERGY:
        enumString = GRAPH_WELD_ENERGY;
        break;
    case GraphtableEnum::WELD_PEAK_POWER:
        enumString = GRAPH_WELD_PEAK_POWER;
        break;
    case GraphtableEnum::START_FREQ:
        enumString = GRAPH_START_FREQ;
        break;
    case GraphtableEnum::FREQ_CHANGE:
        enumString = GRAPH_FREQ_CHANGE;
        break;
    case GraphtableEnum::CYCLE_TIME:
        enumString = GRAPH_TOTAL_CYCLE_TIME;
        break;
    case GraphtableEnum::USERNAME:
        enumString = GRAPH_USERNAME;
        break;
    case GraphtableEnum::PARTID:
        enumString = GRAPH_PARTID;
        break;
    case GraphtableEnum::BATCHID:
        enumString = GRAPH_BATCHID;
        break;
    case GraphtableEnum::TRIGGER_POINT:
        enumString = GRAPH_TRIGGER_POINT;
        break;
    case GraphtableEnum::WELD_SONIC_POINT:
        enumString = GRAPH_WELD_SONIC_POINT;
        break;
    case GraphtableEnum::HOLD_POINT:
        enumString = GRAPH_HOLD_POINT;
        break;
    case GraphtableEnum::ALARM_FLAG:
        enumString = GRAPH_ALARM_FLAG;
        break;
    case GraphtableEnum::RECIPE_STATUS:
        enumString = GRAPH_RECIPE_STATUS;
        break;
    default:
        enumString = "NA";
        break;
    }

    return enumString;
}

/**
 * @brief GraphTableStringToEnum : Returns weld result parameter enum based on name
 * @param enumValue
 * @return
 */
GraphtableEnum GraphTableStringToEnum(QString qstrName)
{
    GraphtableEnum enumString = GraphtableEnum::NO_ELEMENT;

    if(!qstrName.compare(GRAPH_RECIPE_NUMBER,Qt::CaseInsensitive))
        enumString = GraphtableEnum::RECIPE_NUMBER;
    else if(!qstrName.compare(GRAPH_VERSION_NUMBER,Qt::CaseInsensitive))
        enumString = GraphtableEnum::VERSION_NUMBER;
    else if(!qstrName.compare(GRAPH_DATE_TIME,Qt::CaseInsensitive))
        enumString = GraphtableEnum::DATE_TIME;
    else if(!qstrName.compare(GRAPH_CYCLE_COUNTER,Qt::CaseInsensitive))
        enumString = GraphtableEnum::CYCLE_COUNTER;
    else if(!qstrName.compare(GRAPH_WELD_MODE,Qt::CaseInsensitive))
        enumString = GraphtableEnum::WELD_MODE;
    else if(!qstrName.compare(GRAPH_MAX_WELD_FORCE,Qt::CaseInsensitive))
        enumString = GraphtableEnum::MAX_WELD_FORCE;
    else if(!qstrName.compare(GRAPH_WELD_ABSOLUTE,Qt::CaseInsensitive))
        enumString = GraphtableEnum::WELD_ABSOLUTE;
    else if(!qstrName.compare(GRAPH_END_HOLD_FORCE,Qt::CaseInsensitive))
        enumString = GraphtableEnum::END_HOLD_FORCE;
    else if(!qstrName.compare(GRAPH_TOTAL_ABSOLUTE,Qt::CaseInsensitive))
        enumString = GraphtableEnum::TOTAL_ABSOLUTE;
    else if(!qstrName.compare(GRAPH_WELD_COLLAPSE_DISTANCE,Qt::CaseInsensitive))
        enumString = GraphtableEnum::WELD_COLLAPSE_DISTANCE;
    else if(!qstrName.compare(GRAPH_HOLD_COLLAPSE_DISTANCE,Qt::CaseInsensitive))
        enumString = GraphtableEnum::HOLD_COLLAPSE_DISTANCE;
    else if(!qstrName.compare(GRAPH_TOTAL_COLLAPSE_DISTANCE,Qt::CaseInsensitive))
        enumString = GraphtableEnum::TOTAL_COLLAPSE_DISTANCE;
    else if(!qstrName.compare(GRAPH_TRIGGER_DISTANCE,Qt::CaseInsensitive))
        enumString = GraphtableEnum::TRIGGER_DISTANCE;
    else if(!qstrName.compare(GRAPH_WELD_TIME,Qt::CaseInsensitive))
        enumString = GraphtableEnum::WELD_TIME;
    else if(!qstrName.compare(GRAPH_WELD_ENERGY,Qt::CaseInsensitive))
        enumString = GraphtableEnum::WELD_ENERGY;
    else if(!qstrName.compare(GRAPH_WELD_PEAK_POWER,Qt::CaseInsensitive))
        enumString = GraphtableEnum::WELD_PEAK_POWER;
    else if(!qstrName.compare(GRAPH_START_FREQ,Qt::CaseInsensitive))
        enumString = GraphtableEnum::START_FREQ;
    else if(!qstrName.compare(GRAPH_FREQ_CHANGE,Qt::CaseInsensitive))
        enumString = GraphtableEnum::FREQ_CHANGE;
    else if(!qstrName.compare(GRAPH_TOTAL_CYCLE_TIME,Qt::CaseInsensitive))
        enumString = GraphtableEnum::CYCLE_TIME;
    else if(!qstrName.compare(GRAPH_USERNAME,Qt::CaseInsensitive))
        enumString = GraphtableEnum::USERNAME;
    else if(!qstrName.compare(GRAPH_PARTID,Qt::CaseInsensitive))
        enumString = GraphtableEnum::PARTID;
    else if(!qstrName.compare(GRAPH_BATCHID,Qt::CaseInsensitive))
        enumString = GraphtableEnum::BATCHID;
    else if(!qstrName.compare(GRAPH_TRIGGER_POINT,Qt::CaseInsensitive))
        enumString = GraphtableEnum::TRIGGER_POINT;
    else if(!qstrName.compare(GRAPH_WELD_SONIC_POINT,Qt::CaseInsensitive))
        enumString = GraphtableEnum::WELD_SONIC_POINT;
    else if(!qstrName.compare(GRAPH_HOLD_POINT,Qt::CaseInsensitive))
        enumString = GraphtableEnum::HOLD_POINT;
    else if(!qstrName.compare(GRAPH_ALARM_FLAG,Qt::CaseInsensitive))
        enumString = GraphtableEnum::ALARM_FLAG;
    else if(!qstrName.compare(GRAPH_RECIPE_STATUS,Qt::CaseInsensitive))
            enumString = GraphtableEnum::RECIPE_STATUS;
    else
        enumString = GraphtableEnum::NO_ELEMENT;

    return enumString;
}

/**
 * @brief addUnitToParam : Adds units to respective weld result parameters
 * @param enumValue
 * @param unit
 * @return
 */
QString addUnitToParam(GraphtableEnum enumValue, const QString& unit)
{

    QString convertdValue = "";

    switch (enumValue)
    {
    case GraphtableEnum::MAX_WELD_FORCE:
    case GraphtableEnum::END_HOLD_FORCE:
        if (unit == "SI")
            convertdValue = GraphtableEnumToString(enumValue) + "(N)";
        else
            convertdValue = GraphtableEnumToString(enumValue) + "(lbs)";
        break;
    case GraphtableEnum::WELD_ABSOLUTE:
    case GraphtableEnum::TOTAL_ABSOLUTE:
    case GraphtableEnum::WELD_COLLAPSE_DISTANCE:
    case GraphtableEnum::HOLD_COLLAPSE_DISTANCE:
    case GraphtableEnum::TOTAL_COLLAPSE_DISTANCE:
    case GraphtableEnum::TRIGGER_DISTANCE:
        if (unit == "SI")
            convertdValue = GraphtableEnumToString(enumValue) + "(mm)";
        else
            convertdValue = GraphtableEnumToString(enumValue) + "(in)";
        break;
    case GraphtableEnum::WELD_TIME:
    case GraphtableEnum::CYCLE_TIME:
    case GraphtableEnum::TRIGGER_POINT:
    case GraphtableEnum::WELD_SONIC_POINT:
    case GraphtableEnum::HOLD_POINT:
        convertdValue = GraphtableEnumToString(enumValue) + "(s)";
        break;
    case GraphtableEnum::WELD_ENERGY:
        convertdValue = GraphtableEnumToString(enumValue) + "(J)";
        break;
    case GraphtableEnum::WELD_PEAK_POWER:
        convertdValue = GraphtableEnumToString(enumValue) + "(W)";
        break;
    case GraphtableEnum::START_FREQ:
        convertdValue = GraphtableEnumToString(enumValue) + "(Hz)";
        break;
    default:
        convertdValue = GraphtableEnumToString(enumValue);
        break;
    } // switch end

    // return conveted value
    return convertdValue;

}

/**
 * @brief GraphTableView::GraphTableView : Constructor
 * @param parent
 */
GraphTableView::GraphTableView(QObject *parent) : QObject(parent)
{
    initAnalytics();
    //initEditRecipeHistory();
    context->setContextProperty("graphtableview",this);
}

/**
 * @brief GraphTableView::addColumns : Adds columns to the list for table view model
 * @param a_paramIndex
 */
void GraphTableView::addColumns(int a_paramIndex)
{
    if(isIndexInList(a_paramIndex))
        return;
    m_TableParamIndexList.append(a_paramIndex);
    std::sort(m_TableParamIndexList.begin(),m_TableParamIndexList.end());
}

/**
 * @brief GraphTableView::removeColumn : Removes columns to the list for table view model
 * @param a_paramIndex
 */
void GraphTableView::removeColumn(int a_paramIndex)
{
    m_TableParamIndexList.removeAt(m_TableParamIndexList.indexOf(a_paramIndex));
    //m_TableParamIndexList.erase(m_TableParamIndexList.find(a_paramIndex));
}

/**
 * @brief GraphTableView::getTableIndex : Returns index of the table view
 * @param a_paramIndex
 * @return
 */
int GraphTableView::getTableIndex(int a_paramIndex)
{
    return m_TableParamIndexList.at(a_paramIndex);
    //return *std::next(m_TableParamIndexList.begin(),a_paramIndex);
}

/**
 * @brief GraphTableView::getListSize : Returns the size of table list
 * @return
 */
int GraphTableView::getListSize()
{
    return m_TableParamIndexList.count();
}

/**
 * @brief GraphTableView::isIndexInList : Returns a bool value, if index is present in list or not
 * @param a_paramIndex
 * @return
 */
bool GraphTableView::isIndexInList(int a_paramIndex)
{
    if(m_TableParamIndexList.indexOf(a_paramIndex) == -1)
        return false;
    return true;

    //return m_TableParamIndexList.find(a_paramIndex)!=m_TableParamIndexList.end();
}

/**
 * @brief GraphTableView::getTextFromModelAtIndex : Returns parameter name in the list at particular index
 * @param a_paramIndex
 * @return
 */
QString GraphTableView::getTextFromModelAtIndex(int a_paramIndex)
{
    return (dynamic_cast<TableParamName*>(m_CheckBoxParamList.at(a_paramIndex)))->m_ParamName;
    /*(dynamic_cast <GraphTableView*>(checkBoxParamList.at(a_paramIndex)))->;*/
}

/**
 * @brief TableParamName::getName : Returns parameter name
 * @return
 */
QString TableParamName::getName()
{
    return m_ParamName;
}

/**
 * @brief TableParamName::TableParamName : Constructor
 * @param parent
 */
TableParamName::TableParamName(QObject *parent): QObject(parent)
{

}

/**
 * @brief TableParamName::TableParamName : Parameterized constructor
 * @param a_Name
 * @param parent
 */
TableParamName::TableParamName(QString a_Name,QObject *parent): QObject(parent)
{
    m_ParamName= a_Name;
}

/**
 * @brief TableRowData::TableRowData  : Constructor
 * @param a_param
 * @param ptr
 */
TableRowData::TableRowData(QString a_param,QObject* ptr):QObject(ptr)
{
    m_ParamValue=a_param;
}

/**
 * @brief TableRowData::getParamValue : Returns parameter value
 * @return
 */
QString TableRowData::getParamValue()
{
    return m_ParamValue;
}

/**
 * @brief TableRowData::setTableRowData : Sets table row data
 * @param a_row
 */
void TableRowData::setTableRowData(QString a_row)
{
    QStringList query=a_row.split(',');
}
/**
 * @brief GraphTableView::getLastWeldResultData
 */
void GraphTableView::getLastWeldResultData(const QString& recipeNumber)
{
    m_Context->setContextProperty("tableGridViewmodel",QVariant::fromValue(m_dummyTableRow));

    CIObj.sendMessage(REQ_GET_LAST_ALL_WELD_RESULT_DATA, RES_GET_LAST_ALL_WELD_RESULT_DATA, recipeNumber);
    QString a_recipeBuffer;
    CIObj.recivedMessage(RES_GET_LAST_ALL_WELD_RESULT_DATA, a_recipeBuffer);
    QStringList query=a_recipeBuffer.split(',');
    int no_Of_Recipe=query.size()/GRAPH_TABLE_EXPECTED_RESULT;;
    int startIndex=0;
    int dwTableIndex=0;
    for(int index=0;index<no_Of_Recipe;index++)
    {
        m_TableRow.insert(dwTableIndex,new TableRowData(joinQStringList(query,startIndex,17)));
        startIndex+=GRAPH_TABLE_EXPECTED_RESULT;
        dwTableIndex++;
    }

    m_Context->setContextProperty("tableGridViewmodel",QVariant::fromValue(m_TableRow));
}

/**
 * @brief GraphTableView::getWeldResultData : Reads weld results data from DB and updates in the UI
 */
void GraphTableView::getWeldResultData()
{
    m_Context->setContextProperty("deltaParametersModel",QVariant::fromValue(m_dummyCheckBoxParamList));
    //m_Context->setContextProperty("tableGridViewmodel",QVariant::fromValue(m_dummyTableRow));

    m_Context->setContextProperty("deltaParametersModel",QVariant::fromValue(m_CheckBoxParamList));
    //m_Context->setContextProperty("tableGridViewmodel",QVariant::fromValue(m_TableRow));

}

/**
 * @brief GraphTableView::initAnalytics : Called when clicks on table view in UI, and sets checked
 *                                        status of weld result parameters in triangel popup
 */
void GraphTableView::initAnalytics()
{
    getGraphViewTableOrder();

    QString unit = "";
    if (dwCurrentUnit == 0)
        unit = GRAPH_TABLE_UNIT_SI;

    m_CheckBoxParamList.clear();
    /* Below indicates, omission of two parameters to display from DB
       i.e. Stack Serial No. and Velocity */
    for (auto count = 0; count < (GRAPH_TABLE_MAX_FIELDS-2); ++count)
    {
        auto param = addUnitToParam(static_cast<GraphtableEnum>(count), unit);
        m_CheckBoxParamList.append(new TableParamName(param));
    }
}

/**
 * @brief GraphTableView::getTableElementValue : Returns table element value
 * @param a_str
 * @param a_column
 * @return
 */
QString GraphTableView::getTableElementValue(QString a_str,int a_column)
{
    QStringList query=a_str.split(',');
    QString unit = "";
    if (dwCurrentUnit == 0)
        unit = GRAPH_TABLE_UNIT_SI;

    return convertTableValues(query.at(a_column), a_column, unit);
}

/**
 * @brief GraphTableView::addEditRecipeHistoryColumns : Returns edit recipe history columns
 * @param a_paramIndex
 */
void GraphTableView::addEditRecipeHistoryColumns(int a_paramIndex)
{
    m_EditRecipeHistoryTableParamIndexList.insert(a_paramIndex);
}

/**
 * @brief GraphTableView::removeEditRecipeHistoryColumn : Removes edit recipe history column
 * @param a_paramIndex
 */
void GraphTableView::removeEditRecipeHistoryColumn(int a_paramIndex)
{
    m_EditRecipeHistoryTableParamIndexList.erase(m_EditRecipeHistoryTableParamIndexList.find(a_paramIndex));
}

/**
 * @brief GraphTableView::getEditRecipeHistoryTableIndex : Returns edit recipe history list index
 * @param a_paramIndex
 * @return
 */
int GraphTableView::getEditRecipeHistoryTableIndex(int a_paramIndex)
{
    return *std::next(m_EditRecipeHistoryTableParamIndexList.begin(),a_paramIndex);
}

/**
 * @brief GraphTableView::getEditRecipeHistoryListSize : Returns edit recipe history list size
 * @return
 */
int GraphTableView::getEditRecipeHistoryListSize()
{
    return m_EditRecipeHistoryTableParamIndexList.size();
}

/**
 * @brief GraphTableView::isIndexInEditRecipeHistoryList : Checks whether clicked recipe index is available or not
 * @param a_paramIndex
 * @return
 */
bool GraphTableView::isIndexInEditRecipeHistoryList(int a_paramIndex)
{
    return m_EditRecipeHistoryTableParamIndexList.find(a_paramIndex)!=m_EditRecipeHistoryTableParamIndexList.end();
}

/**
 * @brief GraphTableView::getTextFromModelAtIndexInEditRecipeHistoryList : Returns parameter name
 *                                                                         based on index
 * @param a_paramIndex
 * @return
 */
QString GraphTableView::getTextFromModelAtIndexInEditRecipeHistoryList(int a_paramIndex)
{
    return (dynamic_cast<TableParamName*>(m_EditRecipeHistoryCheckBoxParamList.at(a_paramIndex)))->m_ParamName;
    /*(dynamic_cast <GraphTableView*>(checkBoxParamList.at(a_paramIndex)))->;*/
}

/**
 * @brief GraphTableView::joinQStringList : Appends string list values to string
 * @param a_QStringList
 * @param a_StartIndex
 * @param a_Length
 * @return
 */
QString GraphTableView::joinQStringList(QStringList a_QStringList,int a_StartIndex,int a_Length)
{
    QString str=a_QStringList.at(a_StartIndex);
    for(int index=a_StartIndex+1;index<(a_StartIndex+a_Length);index++)
    {
        str= str+","+a_QStringList.at(index);
    }
    return str;
}

/**
 * @brief GraphTableView::convertTableValues : Converts table values according to units
 * @param value
 * @param columnIndex
 * @param unit
 * @return
 */
QString GraphTableView::convertTableValues(const QString &value, int columnIndex, const QString &unit)
{
    QString convertdValue = "";

    GraphtableEnum index = static_cast<GraphtableEnum>(columnIndex);

    switch (index)
    {
    case GraphtableEnum::MAX_WELD_FORCE:
    case GraphtableEnum::END_HOLD_FORCE:
        if (unit == "SI")
            convertdValue = value;
        else
            convertdValue = Utility::convertNtolbs(value);
        break;
        /*To segregate mm parameters apart from time parameters*/
    case GraphtableEnum::WELD_ABSOLUTE:
    case GraphtableEnum::TOTAL_ABSOLUTE:
    case GraphtableEnum::WELD_COLLAPSE_DISTANCE:
    case GraphtableEnum::HOLD_COLLAPSE_DISTANCE:
    case GraphtableEnum::TOTAL_COLLAPSE_DISTANCE:
    case GraphtableEnum::TRIGGER_DISTANCE:
        convertdValue = Utility::convertMicrometreToMillimetre(value);
        break;
    case GraphtableEnum::TRIGGER_POINT:
    case GraphtableEnum::WELD_SONIC_POINT:
    case GraphtableEnum::HOLD_POINT:
    case GraphtableEnum::WELD_TIME:
    case GraphtableEnum::CYCLE_TIME:
        convertdValue = Utility::convertMsecToSec(value);
        break;
    case GraphtableEnum::WELD_MODE:
        convertdValue = WeldModeTypeEnumToString(static_cast<WeldModeType>(value.toInt()));
        break;
    case GraphtableEnum::RECIPE_STATUS:
        convertdValue = RecipeStatusTypeEnumToString(static_cast<RecipeStatusType>(value.toInt()));
        break;
    case GraphtableEnum::WELD_ENERGY:
        convertdValue = Utility::convertMilliJoulesToJoules(value);
        break;
    default:
        convertdValue = value;
        break;
    } // switch end

    // return conveted value
    return convertdValue;
}

/**
 * @brief GraphTableView::initEditRecipeHistory : Initializes recipe history into list
 */
void GraphTableView::initEditRecipeHistory()
{

    m_EditRecipeHistoryTableParamIndexList.insert(0);
    m_EditRecipeHistoryTableParamIndexList.insert(1);
    m_EditRecipeHistoryTableParamIndexList.insert(2);
    m_EditRecipeHistoryTableParamIndexList.insert(3);
    m_EditRecipeHistoryTableParamIndexList.insert(4);
    m_EditRecipeHistoryTableParamIndexList.insert(5);

    QString  weldRecipeTable = "WeldRecipeVerNumber,WeldMode,ModeValue,HoldTime,AmplitudeStepsCnt,AmplitudeStepAt,AmplitudeStepValue1,AmplitudeStepValue2,AmplitudeStepValue3,AmplitudeStepValue4,AmplitudeStepValue5,AmplitudeStepValue6,AmplitudeStepValue7,AmplitudeStepValue8,AmplitudeStepValue9,AmplitudeStepValue10,AmplitudeStep1,AmplitudeStep2,AmplitudeStep3,AmplitudeStep4,AmplitudeStep5,AmplitudeStep6,AmplitudeStep7,AmplitudeStep8,AmplitudeStep9,AmplitudeStep10,WeldForce,ForceRampTime,TotalCollapseTarget,HoldForce,HoldForceRampTime,ExpectedPartContactPosition,ReadyPosition,DownAcceleration,DownMaxVelocity,DownDeceleration,ReturnAcceleration,ReturnMaxVelocity,ReturnDeceleration,NumForceSteps,ForceStepAt,ForceStepValue1,ForceStepValue2,ForceStepValue3,ForceStepValue4,ForceStepValue5,ForceStepValue6,ForceStepValue7,ForceStepValue8,ForceStepValue9,ForceStepValue10,ForceStepRampValue1,ForceStepRampValue2,ForceStepRampValue3,ForceStepRampValue4,ForceStepRampValue5,ForceStepRampValue6,ForceStepRampValue7,ForceStepRampValue8,ForceStepRampValue9,ForceStepRampValue10,ForceStep1,ForceStep2,ForceStep3,ForceStep4,ForceStep5,ForceStep6,ForceStep7,ForceStep8,ForceStep9,ForceStep10,IsActive,IsLocked,CompanyName";

    QStringList query=weldRecipeTable.split(',');
    for(int paramIndex=0;paramIndex<query.size();paramIndex++)
    {
        m_EditRecipeHistoryCheckBoxParamList.append(new TableParamName(query.at(paramIndex)));
    }
}

/**
 * @brief Configuration::setGraphViewTableOrder : Sets event table order into config.ini
 * @param qstrOrder
 */
void GraphTableView::setGraphViewTableOrder(QString qstrOrder)
{
    for (auto index : m_TableParamIndexList)
        qstrOrder += QString::number(index) + ",";

    qstrOrder.remove(qstrOrder.length()-1, 1);

    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("graphviewtableorder");
    settings.setValue("ORDER", qstrOrder);
    settings.endGroup();
}

/**
 * @brief Configuration::getGraphViewTableOrder : Returns event table order read from config.ini
 * @return
 */
void GraphTableView::getGraphViewTableOrder()
{
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("graphviewtableorder");
    QString value = settings.value("ORDER").toString();

    if (!value.isEmpty())
    {
        auto indexList = value.split(",");

        m_TableParamIndexList.clear();

        for (auto list : indexList)
        {
            m_TableParamIndexList.append(list.toInt());
        }
    }
    else
    {
        m_TableParamIndexList.append(0);
        m_TableParamIndexList.append(1);
        m_TableParamIndexList.append(2);
        m_TableParamIndexList.append(3);
        m_TableParamIndexList.append(4);
        m_TableParamIndexList.append(5);
        setGraphViewTableOrder("");
    }
}

void GraphTableView::updateOrderIndex(QString qstrOrderedString)
{
    if(qstrOrderedString.at(qstrOrderedString.size()-1) == ",")
        qstrOrderedString.remove(qstrOrderedString.size()-1,1);

    m_TableParamIndexList.clear();
    QStringList qstrLstTemp = qstrOrderedString.split(",");
    for(int dwIndex = 0; dwIndex < qstrLstTemp.count();dwIndex++)
    {
        QString qstrIndName = qstrLstTemp.at(dwIndex);
        int dwUnitIndex = qstrIndName.indexOf("(");
        if(dwUnitIndex >= 0)
        {
            qstrIndName.remove(dwUnitIndex,qstrIndName.size()-dwUnitIndex);
        }

        int dwInt = (int)GraphTableStringToEnum(qstrIndName);
        if(dwInt != -1)
        {
            m_TableParamIndexList.append(dwInt);
        }
    }
    setGraphViewTableOrder("");
}
