/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Production Setup Info

***************************************************************************/

#ifndef RECIPESETUPINFO_H
#define RECIPESETUPINFO_H
#include <QString>
#include <QObject>

#define CUST_NAME_NA QObject::tr("NA")
#define CUST_NAME_BATCHCOUNT_DEFAULT "100000"
#define CUST_NAME_EXPECTED_RESULTS 8

/**
 * @brief The stProductionSetUpInfo struct
 */
struct stProductionSetUpInfo
{
    QString m_RecipeNumber;
    QString m_CycleCount;
    QString m_ProductionInstruction;
    QString m_RecipeDescription;
    int m_BatchSetupEnable;
    int m_CountWithAlarmEnable;
    QString m_BatchCount;
    QString m_BatchId;
};

#define OVERWRITE_BATCHCOUNT 1
#define POPUP_BATCHCOUNT 2

//! Class ProductionSetUpInfo.
/*!
  This class derived from QObject, handles communication with QML.
*/
//
class ProductionSetUpInfo:public QObject
{
    Q_OBJECT
public:
    ProductionSetUpInfo(QObject *parent=0);
    ProductionSetUpInfo (const ProductionSetUpInfo &a_productionSetUpInfo,QObject *parent=0);
    ProductionSetUpInfo &operator =(ProductionSetUpInfo a_productionSetUpInfo);

    stProductionSetUpInfo m_stProductionSetUpInfo;
    Q_INVOKABLE void setBatchCount(QString);
    Q_INVOKABLE void setCycleCount(QString);
    Q_INVOKABLE void setProductionInstruction(QString);
    Q_INVOKABLE void setRecipeDescription(QString);

    Q_INVOKABLE void setBatchSetupEnable(int);
    Q_INVOKABLE void setCountWithAlarmEnable(int);
    Q_INVOKABLE void setBatchId(QString);

    Q_INVOKABLE int getBatchSetupEnable();
    Q_INVOKABLE int getCountWithAlarmEnable();
    Q_INVOKABLE QString getRecipeNumber();
    Q_INVOKABLE QString getRecipeName();
    Q_INVOKABLE QString getBatchCount();
    Q_INVOKABLE QString getCycleCount();
    Q_INVOKABLE QString getProductionInstruction();
    Q_INVOKABLE QString getRecipeDescription();
    Q_INVOKABLE QString getBatchId();

    Q_INVOKABLE void recipeSetupInfoInit(QString,QString);
    Q_INVOKABLE void savetoDB();
    Q_INVOKABLE int checkBatchCount();
    Q_INVOKABLE QString getActiveRecipeNumberForProduction();
    Q_INVOKABLE int sendResetBatchCounterReqToSC();
    Q_INVOKABLE void replaceBatchCountWithOldVal();
    void FillRecipeInfo(QStringList m_recipeInfoBuffer);

private:
    QString m_qstrRecipeName;
    QString m_qstrRecipeNumber;
    QString m_batchCountDBVal;

};


#endif // RECIPESETUPINFO_H
