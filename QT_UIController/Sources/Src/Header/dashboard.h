/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Dashboard

***************************************************************************/

#ifndef DASHBOARD_H
#define DASHBOARD_H
#include <QObject>
#include <QDate>
#include <QTime>
#include "communicationinterface.h"
#include "productionsetupinfo.h"
#include "lastweldresults.h"
#include "productionresults.h"
#include "Header/recipe.h"

#define CUST_NAME_TIME                    QObject::tr("Time")
#define CUST_NAME_ENERGY                  QObject::tr("Energy")
#define CUST_NAME_PEAK                    QObject::tr("Peak power")
#define CUST_NAME_SCRUB                   QObject::tr("Scrub Time")
#define CUST_NAME_ABSOLUTE                QObject::tr("Absolute Distance")
#define CUST_NAME_COLLAPSE                QObject::tr("Collapse Distance")
#define CUST_NAME_DYNAMIC                 QObject::tr("Dynamic")
#define CUST_NAME_MULTI_MODE              QObject::tr("Multi Mode")

#define CUST_NAME_ABSOLUTE_DIS     QObject::tr("Absolute Distance")
#define CUST_NAME_COLLAPSE_DIS     QObject::tr("Collapse Distance")
#define CUST_NAME_WELD_TIME        QObject::tr("WeldTime")
#define CUST_NAME_WELD_FORCE       QObject::tr("WeldForce")
#define CUST_NAME_WELD_ENERGY      QObject::tr("WeldEnergy")
#define CUST_NAME_HOLD_FORCE       QObject::tr("HoldForce")

#define DASH_CYCLESOK               QObject::tr("Cycles OK")
#define DASH_PARTSMIN               QObject::tr("Parts/Min")
#define DASH_REJECT                 QObject::tr("Reject")
#define DASH_SUSPECT                QObject::tr("Suspect")

#define VALUE_SIZE_1                      1
#define VALUE_SIZE_2                      2
#define VALUE_SIZE_3                      3
#define VALUE_SIZE_4                      4
#define VALUE_SIZE_5                      5
#define VALUE_SIZE_6                      6
#define VALUE_SIZE_7                      7
#define VALUE_SIZE_8                      8
#define VALUE_SIZE_14                     14
#define FONT_SIZE_34                      34
#define FONT_SIZE_42                      42
#define FONT_SIZE_48                      48
#define FONT_SIZE_72                      72

#define CUST_NAME_UINTSEC                 "s"
#define CUST_NAME_UINTMilliMeter          "mm"
#define CUST_NAME_UINTINCHES              "in"
#define CUST_NAME_UINTLBS                 "lbs"
#define CUST_NAME_UINTJOULES              "J"
#define CUST_NAME_UINTWATTS               "W"
#define CUST_NAME_UINTPERCENTAGE          "%"
//#define CUST_NAME_EXPECTED_RESULTS         9
#define CUST_NAME_EXPECTED_ALARM_RESULTS   3
#define CUST_NAME_NA                       QObject::tr("NA")
#define CUST_NAME_NOACTIVE_RECIPE          QObject::tr("No Active Recipe Available")
#define CUST_NAME_MACHINE_IMG              "qrc:/images/image/machine.svg"
#define CUST_NAME_PART_ICON_IMG            "qrc:/images/image/part_icon_u135.png"

//!  A DashBoard class.
/*!
  This class derived from QObject, handles DashBoard BackEnd functionality.
*/
class DashBoard: public QObject
{
    Q_OBJECT
public:
    QStringList m_listAlarmId;
    QStringList m_listAlarmType;
    QList<QObject*> alarmList;
    QList<QObject*> alarmList1;
    QList<QObject*> m_lastWeldList;
    QList<QObject*> m_lastWeldListDummy;
    QList<QObject*> m_productionList;
    QList<QObject*> m_productionListDummy;

    DashBoard(const QString &AlarmId,const QString &AlarmType, const QTime &AlarmTime, const QDate &AlarmDate, QObject *parent=0);
    explicit DashBoard(QObject *parent = 0);

    Q_PROPERTY(QString m_RecipeNumber READ GetRecipeNumber NOTIFY OnRecipeNumberChanged)
    Q_PROPERTY(QString m_RecipeName READ GetRecipeName NOTIFY OnRecipeNameChanged)
    Q_PROPERTY(QString m_Company READ GetCompany  NOTIFY OnCompanyChanged)
    Q_PROPERTY(QString m_WeldMode READ GetWeldMode NOTIFY OnWeldModeChanged)
    Q_PROPERTY(QString m_ControlValue READ GetControlValue NOTIFY OnControlValueChanged)
    Q_PROPERTY(bool m_IsActiveValue READ GetIsActiveValue NOTIFY OnIsActiveValueChanged)
    Q_PROPERTY(bool m_IsLockedValue READ GetIsLockedValue NOTIFY OnIsLockedValueChanged)
    Q_PROPERTY(bool m_IsValidated READ GetIsValidated NOTIFY OnIsValidatedChanged)
    Q_PROPERTY(QString m_ControlValueUnit READ GetControlValueUnit NOTIFY OnControlValueUnitChanged)
    Q_PROPERTY(QString m_GoodNo READ GetGoodNo  NOTIFY OnGoodNoChanged)
    Q_PROPERTY(QString m_ThroughPutNo READ GetThroughPutNo  NOTIFY OnThroughPutNoChanged)
    Q_PROPERTY(QString m_SuspectNo READ GetSuspectNo NOTIFY OnSuspectNoChanged)
    Q_PROPERTY(QString m_RejectNo READ GetRejectNo NOTIFY OnRejectNoChanged)

    Q_PROPERTY(QString AlarmId READ GetAlarmId NOTIFY OnAlarmIdChanged )
    Q_PROPERTY(QString AlarmType READ GetAlarmType NOTIFY OnAlarmTypeChanged)
    Q_PROPERTY(QString AlarmTime READ GetAlarmTime NOTIFY OnAlarmTimeChanged)
    Q_PROPERTY(QString AlarmDate READ GetAlarmDate NOTIFY OnAlarmDateChanged)

    void FillActiveRecipe(QStringList );
    void FillProductionWeldResult();
    Q_INVOKABLE void refreshPartsPerMin();
    void runtimeFillLastWeldResult ( QStringList m_lastWeldResultBuffer );
    void runtimeFillProductionWeldResult ( QStringList m_lastWeldResultBuffer );
    void FillAlarmLogs(QStringList);
    void clearAlarmList();
    void clearLastWeldList();
    void clearProductionList();
    void updateLastWeldData(QString dashboardLastWeldResult);
    void updateProductionWeldResult(QString dashboardProductionWeldResult);
    void updateAlarmsList(QString dashboardAlarmLog);
    Q_INVOKABLE void FillLastWeldResult();

signals:
public:
    Q_INVOKABLE void init();
    Q_INVOKABLE QString GetRecipeNumber();
    Q_INVOKABLE QString GetRecipeName();
    Q_INVOKABLE QString GetCompany();
    Q_INVOKABLE bool GetIsActiveValue();
    QString GetWeldMode();
    QString GetControlValue();
    QString GetControlValueUnit();
    bool GetIsLockedValue();
    bool GetIsValidated();

    QString GetGoodNo();
    QString GetThroughPutNo();
    QString GetSuspectNo();
    QString GetRejectNo();
    QString GetAlarmId() const;
    QString GetAlarmType() const;
    QString GetAlarmTime();
    QString GetAlarmDate();
    QString getLastWeldUnits();
    QString getLastWeldImperialUnits();
    QString getProductionParamsColor();
    Q_INVOKABLE void setDashboardOpenStatus(bool bStatus);
    QString GetSupportedModes();
    void alarmLogUpdateInd();

public slots:
   Q_INVOKABLE void dashboardUpdateInd();

signals:
    void OnRecipeNumberChanged();
    void OnRecipeNameChanged();
    void OnCompanyChanged();
    void OnWeldModeChanged();
    void OnControlValueChanged();
    void OnIsActiveValueChanged();
    void OnIsLockedValueChanged();
    void OnControlValueUnitChanged();
    void OnGoodNoChanged();
    void OnThroughPutNoChanged();
    void OnRejectNoChanged();
    void OnSuspectNoChanged();
    void OnIsValidatedChanged();
    void OnAlarmIdChanged();
    void OnAlarmTimeChanged();
    void OnAlarmDateChanged();
    void OnAlarmTypeChanged();

private:
    QString m_recipeNumber;
    QString m_recipeName;
    QString m_company;
    QString m_weldMode;
    QString m_controlValue;
    QString m_controlValueUnit;
    bool m_isActiveValue;
    bool m_isLockedValue;
    bool m_isValidated;
    bool bDashBoardOpenStatus;
    QString m_goodNo;
    QString m_throughPutNo;
    QString m_rejectNo;
    QString m_suspectNo;

    QString m_alarmId;
    QString m_alarmType;
    QTime m_alarmTime;
    QDate m_alarmDate;
};

#endif // DASHBOARD_H
