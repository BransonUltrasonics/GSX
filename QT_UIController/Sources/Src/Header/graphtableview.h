/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Graph Table View

***************************************************************************/
#ifndef GRAPHTABLEVIEW_H
#define GRAPHTABLEVIEW_H

#include <QObject>
#include <set>
#include <QList>
#include <QQmlContext>
#include <Header/communicationinterface.h>

#define GRAPH_TABLE_UNIT_SI                "SI"
#define GRAPH_TABLE_UNIT_NEWTON            "(N)"
#define GRAPH_TABLE_UNIT_LBS               "(lbs)"
#define GRAPH_TABLE_UNIT_MILLIMETRE        "(mm)"
#define GRAPH_TABLE_UNIT_INCHES            "(in)"
#define GRAPH_TABLE_UNIT_SECONDS           "(s)"
#define GRAPH_TABLE_UNIT_JOULES            "(J)"
#define GRAPH_TABLE_UNIT_WATTS             "(W)"
#define GRAPH_TABLE_UNIT_HERTZ             "(Hz)"
#define GRAPH_TABLE_EXPECTED_RESULT        17

#define GRAPH_TABLE_MAX_FIELDS             29
#define GRAPH_TABLE_SKIP_STACK_SN          3
#define GRAPH_TABLE_SKIP_VELOCITY          14

enum class GraphtableEnum
{
    NO_ELEMENT = -1,
    RECIPE_NUMBER = 0,
    VERSION_NUMBER,
    DATE_TIME,
    CYCLE_COUNTER,
    WELD_MODE,
    MAX_WELD_FORCE,
    END_HOLD_FORCE,
    WELD_ABSOLUTE,
    TOTAL_ABSOLUTE,
    WELD_COLLAPSE_DISTANCE,
    HOLD_COLLAPSE_DISTANCE,
    TOTAL_COLLAPSE_DISTANCE,
    TRIGGER_DISTANCE,
    WELD_TIME,
    WELD_ENERGY,
    WELD_PEAK_POWER,
    START_FREQ,
    FREQ_CHANGE,
    CYCLE_TIME,
    USERNAME,
    PARTID,
    BATCHID,
    TRIGGER_POINT,
    WELD_SONIC_POINT,
    HOLD_POINT,
    ALARM_FLAG,
    RECIPE_STATUS
};

/**
 * @brief The WeldModeType enum
 */
enum class WeldModeType
{
    NONE = 0,
    TIME,
    ENERGY,
    PEAK_POWER,
    GND_DET_VAL,
    ABS_DIS,
    COL_DIS,
    DYNAMIC
};

/**
 * @brief The RecipeStatusType enum
 */
enum class RecipeStatusType {
    SAVED = 0,
    VALIDATED,
    UNSAVED,
    INVALIDATED,
};

class TableRowData:public QObject
{
    Q_OBJECT
public:
    QString m_ParamValue;

    explicit TableRowData(QObject *parent = nullptr);
    TableRowData(QString param,QObject* ptr=nullptr);

    Q_PROPERTY(QString Value READ getParamValue NOTIFY OnParamValueChanged)

    QString getParamValue();
    void setTableRowData(QString a_row);

signals:
    void OnParamValueChanged();
};

class TableParamName:public QObject
{
    Q_OBJECT
public:
    QString m_ParamName;

    explicit TableParamName(QObject *parent = nullptr);
    TableParamName(QString a_Name,QObject *parent = nullptr);

    Q_PROPERTY(QString name READ getName NOTIFY OnNameChanged)

    QString getName();

signals:
    void OnNameChanged();
};

class GraphTableView : public QObject
{
    Q_OBJECT
public:
    QList<QObject*> m_EditRecipeHistoryTableRow;
    QList<QObject*> m_dummyEditRecipeHistoryTableRow;
    QList<QObject*> m_EditRecipeHistoryCheckBoxParamList;
    QList<QObject*> m_dummyEditRecipeHistoryCheckBoxParamList;
    QQmlContext *m_Context;

    explicit GraphTableView(QObject *parent = nullptr);

    Q_INVOKABLE void addColumns(int a_paramIndex);
    Q_INVOKABLE void removeColumn(int a_paramIndex);
    Q_INVOKABLE void initEditRecipeHistory();
    Q_INVOKABLE void initAnalytics();
    Q_INVOKABLE void getWeldResultData();
    Q_INVOKABLE void getLastWeldResultData(const QString& recipeNumber);
    Q_INVOKABLE void addEditRecipeHistoryColumns(int a_paramIndex);
    Q_INVOKABLE void removeEditRecipeHistoryColumn(int a_paramIndex);
    //Q_INVOKABLE void getEditRecipeHistoryDataFromDB(QString a_recipeName);
    Q_INVOKABLE void setGraphViewTableOrder(QString qstrOrder);
    Q_INVOKABLE void getGraphViewTableOrder();
    Q_INVOKABLE void updateOrderIndex(QString qstrOrderedString);
    Q_INVOKABLE int getTableIndex(int a_paramIndex);
    Q_INVOKABLE int getListSize();
    Q_INVOKABLE int getEditRecipeHistoryTableIndex(int a_paramIndex);
    Q_INVOKABLE int getEditRecipeHistoryListSize();
    Q_INVOKABLE bool isIndexInEditRecipeHistoryList(int a_paramIndex);
    Q_INVOKABLE bool isIndexInList(int a_paramIndex);
    Q_INVOKABLE QString getTextFromModelAtIndex(int a_paramIndex);
    Q_INVOKABLE QString getTableElementValue(QString a_str,int row);
    Q_INVOKABLE QString getTextFromModelAtIndexInEditRecipeHistoryList(int a_paramIndex);
    static QString joinQStringList(QStringList a_QStringList,int a_StartIndex,int a_Length);
    QString convertTableValues(const QString &value,  int columnIndex, const QString& unit);

public:
    int m_SelectedIndex;
    QQmlContext * m_context;
    QList<QObject*> m_TableRow;
    QList<QObject*> m_dummyTableRow;
    QList<int> m_TableParamIndexList;
    QList<QObject*> m_CheckBoxParamList;
    QList<QObject*> m_dummyCheckBoxParamList;
    std::set<int> m_EditRecipeHistoryTableParamIndexList;
};

#endif // GRAPHTABLEVIEW_H
