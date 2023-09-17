/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Production

***************************************************************************/
//!  A Production class.
/*!
  This class derived from QObject, handles Production BackEnd functionality.
*/
#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <QObject>
#include <QList>
#include <Header/recipeparameter.h>
#include <QQmlContext>
#include "communicationinterface.h"
#include "productionsetupinfo.h"

#define PRODUCTION_PARTID_INFO QObject::tr("Part ID : ")
#define PRODUCTION_PARTID_STATUS QObject::tr("Waiting for Part ID")
#define CUST_NAME_TIME  QObject::tr("Time")
#define CUST_NAME_ENERGY  QObject::tr("Energy")
#define CUST_NAME_PEAK  QObject::tr("Peak power")
#define CUST_NAME_SCRUB  QObject::tr("Scrub Time")
#define CUST_NAME_ABSOLUTE  QObject::tr("Absolute Distance")
#define CUST_NAME_COLLAPSE  QObject::tr("Collapse Distance")
#define CUST_NAME_DYNAMIC  QObject::tr("Dynamic")
#define CUST_NAME_MULTI_MODE  QObject::tr("Multi Mode")
#define CUST_NAME_LOAD_PART QObject::tr("LOAD PART")
#define CUST_NAME_FREQ_20KHZ 20000
#define CUST_NAME_FREQ_30KHZ 30000
#define CUST_NAME_FREQ_40KHZ 40000
#define CUST_NAME_MAXPOWER_4000 4000
#define CUST_NAME_MAXPOWER_1500 1500
#define CUST_NAME_MAXPOWER_800 800
#define CUST_NAME_PERCENT 100

#define CUST_NAME_UINTSEC "s"
#define CUST_NAME_UINTMilliMeter "mm"
#define CUST_NAME_UINTINCHES "in"
class Production: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString m_RecipeNumber READ GetRecipeNumber NOTIFY OnRecipeNumberChanged)
    Q_PROPERTY(QString m_RecipeName READ GetRecipeName NOTIFY OnRecipeNameChanged)
    Q_PROPERTY(QString m_WeldMode READ GetWeldMode NOTIFY OnWeldModeChanged)
    Q_PROPERTY(QString m_WeldStatus READ GetWeldStatus NOTIFY OnWeldStatusChanged)
    Q_PROPERTY(QString m_ControlValue READ GetControlValue NOTIFY OnControlValueChanged)
    Q_PROPERTY(QString m_ControlValueUnit READ GetControlValueUnit NOTIFY OnControlValueUnitChanged)

    Q_PROPERTY(int m_goodNum READ GetGoodNum WRITE SetGoodNum NOTIFY OnGoodNumChanged)
    Q_PROPERTY(int m_partsPerMin READ GetPartPerMinChanged WRITE SetPartPerMin NOTIFY OnPartPerMinChanged)
    Q_PROPERTY(int m_suspectNum READ GetSuspectNum WRITE SetSuspectNum NOTIFY OnSuspectNumChanged)
    Q_PROPERTY(int m_rejectNum READ GetRejectNum WRITE SetRejectNum NOTIFY OnRejectNumChanged)
    Q_PROPERTY(int m_batchCount READ GetBatchCountNum WRITE SetBatchCounttNum NOTIFY OnBatchCountNumChanged)
    Q_PROPERTY(bool m_batchSetupEnable READ GetBatchSetupEnable WRITE SetBatchSetupEnable NOTIFY OnBatchSetupEnableChanged)
    Q_PROPERTY(int m_BatchCounter READ GetBatchCounterNum WRITE SetBatchCounterNum NOTIFY OnBatchCounterNumChanged)
    Q_PROPERTY(int m_cycleCount READ GetCycleCountNum WRITE SetCycleCountNum NOTIFY OnCycleCountNumChanged)
    Q_PROPERTY(int m_weldProgressPerc READ GetWeldProgressPerc WRITE SetWeldProgressPerc NOTIFY OnWeldProgressPercChanged)
    Q_PROPERTY(int m_goodByGoalPerc READ GetGoodByGoalPerc WRITE SetGoodByGoalPerc NOTIFY OnGoodByGoalPercChanged)
    Q_PROPERTY(int m_peakPower READ GetPeakPowerNum WRITE SetPeakPowerNum NOTIFY OnPeakPowerNumChanged)

signals:
    void updatePartStatus(QString strPartStatusText); //This will display text in UI Status bar
public:

    explicit Production(QObject *parent = 0);
    QString m_recipeNumber;
    QString m_recipeName;
    QString m_weldMode;
    QString m_weldStatus;
    QString m_controlValueUnit;
    QString m_controlValue;

    int m_goodNum;
    int m_partsPerMin;
    int m_rejectNum;
    int m_suspectNum;
    int m_batchCount;
    int m_BatchCounter;
    int m_cycleCount;
    int m_weldProgressPerc;
    int m_goodByGoalPerc;
	int m_peakPower;
    bool m_batchSetupEnable;
    bool productionPageStatus = false;

    QString GetRecipeNumber();
    QString GetRecipeName();
    QString GetActiveRecipeForProduction(QString qstrSupportedModes);
    QString GetSupportedModes();
    QString GetWeldMode();
    QString GetWeldStatus();
    QString GetControlValue();
    QString GetControlValueUnit();
    QString GetProductionWeldResultForProduction();
    QString GetLastWeldPeakPower();
    Q_INVOKABLE int GetGoodNum();
    int GetWeldProgressPerc();
    int GetSuspectNum();
    int GetRejectNum();
    Q_INVOKABLE int GetBatchCountNum();
    Q_INVOKABLE int GetBatchCounterNum();
    Q_INVOKABLE int GetCycleCountNum();
    Q_INVOKABLE bool GetBatchSetupEnable();
    Q_INVOKABLE int GetPartPerMinChanged();
    Q_INVOKABLE int GetGoodByGoalPerc();
    Q_INVOKABLE int GetPeakPowerNum();
    Q_INVOKABLE void updateProductionScreenResults();
    Q_INVOKABLE void updateProductionResults();
    Q_INVOKABLE void updateProductionPower(QString a_powerStatus);
    Q_INVOKABLE void productionStatus(bool a_status);
    Q_INVOKABLE void partidstatusInfo(QString sPartIdStatusInfo);
    Q_INVOKABLE void productionInit();
    Q_INVOKABLE void refreshPartsPerMin();
    void FillActiveRecipe ( QStringList );
    void FillProductionWeldResult( QStringList );

public slots:
    void SetRecipeName(QString RecipeName);
    void SetWeldStatus(QString WeldStatus);

    void SetGoodNum(int Good);
    void SetSuspectNum(int Suspect);
    void SetRejectNum(int Reject);
    void SetPartPerMin(int PartPerMin);
    void SetGoodByGoalPerc(int GoodByGoal);
    void SetWeldProgressPerc(int weldProPerc);
    void SetBatchCounttNum(int CycleCount);
    void SetBatchCounterNum(int BatchCounter);
    void SetCycleCountNum(int CycleCount);
	void SetBatchSetupEnable(bool BatchSetupEnable);
    void SetPeakPowerNum(int PeakPower);
signals:
    void OnRecipeNumberChanged();
    void OnRecipeNameChanged();
    void OnWeldModeChanged();
    void OnWeldStatusChanged();
    void OnControlValueChanged();
    void OnControlValueUnitChanged();

    void OnGoodNumChanged();
    void OnPartPerMinChanged();
    void OnRejectNumChanged();
    void OnSuspectNumChanged();
    void OnGoodByGoalPercChanged();
    void OnWeldProgressPercChanged();
    void OnBatchCountNumChanged();
    void OnBatchCounterNumChanged();
    void OnCycleCountNumChanged();
	void OnBatchSetupEnableChanged();
    void OnPeakPowerNumChanged();
};
#endif // PRODUCTION_H
