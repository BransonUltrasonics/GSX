/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Test Recipe

***************************************************************************/
#ifndef TESTRECIPE_H
#define TESTRECIPE_H
#include <map>
#include <QObject>
#include "Header/weldrecipeparameterdata.h"
#include "Header/SysConfig.h"

/**
  * @brief struct TestRecipe :- structure for Test Recipe
*/
struct TestRecipe
{
    qint32  AmplitudeLoopC1;
    qint32  AmplitudeLoopC2;
    qint32  PhaseLoopI;
    qint32  PhaseLoopCF;
    qint32  FrequencyHigh;
    qint32  FrequencyLow;
    qint32  TestAmplitude;
};

/**
  * @brief class TestRecipeData :- use to handle data between TestRecipe and database
*/

class TestRecipeData:public QObject{
    Q_OBJECT

public:
    TestRecipeData(QObject *parent=0);
    TestRecipeData &operator =(TestRecipeData TestRecipeData);


public:
    TestRecipe m_stTestRecipe;
    TestRecipe m_st_tempTestRecipe;
    std::map<int,TestRecipe >m_AdvancedDataDefaultList;



public:

//    Q_INVOKABLE void setAmplitudeLoopC1(qint32);
//    Q_INVOKABLE void setAmplitudeLoopC2(qint32);
//    Q_INVOKABLE void setPhaseLoopI(qint32);
//    Q_INVOKABLE void setPhaseLoopCF(qint32);
//    Q_INVOKABLE void setFrequencyHigh(qint32);
//    Q_INVOKABLE void setFrequencyLow(qint32);
//    Q_INVOKABLE void setTestAmplitude(qint32);
//    Q_INVOKABLE qint32 getAmplitudeLoopC1();
//    Q_INVOKABLE qint32 getAmplitudeLoopC2();
//    Q_INVOKABLE qint32 getPhaseLoopI();
//    Q_INVOKABLE qint32 getPhaseLoopCF();
//    Q_INVOKABLE qint32 getFrequencyHigh();
//    Q_INVOKABLE qint32 getFrequencyLow();
//    Q_INVOKABLE qint32 getTestAmplitude();
};
/**
  * @brief class TestRecipeModel :- List of Objects of class TestRecipeModel is the model for Test Recipe
*/

class TestRecipeModel:public QObject{

    Q_OBJECT
    Q_PROPERTY(QString parameterName READ getParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(QString parameterLocaleName READ getParameterLocaleName NOTIFY parameterLocaleNameChanged)
    Q_PROPERTY(qint32 parameterValue READ getParameterValue NOTIFY parameterValueChanged)
    Q_PROPERTY(QString parameterUnit READ getParameterUnit NOTIFY parameterUnitChanged)
    Q_PROPERTY(QString parameterMinValue READ getParameterMinValue NOTIFY parameterMinValueChanged)
    Q_PROPERTY(QString parameterMaxValue READ getParameterMaxValue NOTIFY parameterMaxValueChanged)

public :
    TestRecipeModel(const QString &a_parameterName, const QString &a_parameterLocaleName, const qint32 &a_parameterVal, const QString &a_parameterUnit,const QString &a_parameterMinVal,
                    const QString &a_parameterMaxVal,QObject *parent=0);
    explicit TestRecipeModel(QObject *parent = nullptr);
    ~TestRecipeModel();
    QString m_parameterName;
    QString m_parameterLocaleName;
    qint32 m_parameterValue;
    QString m_parameterUnit;
    QString m_parameterMinValue;
    QString m_parameterMaxValue;

    QString getParameterName();
    QString getParameterLocaleName();
    qint32 getParameterValue();
    QString getParameterUnit();
    QString getParameterMinValue();
    QString getParameterMaxValue();

    void setParameterValue(qint32 a_val);

signals:
    void parameterNameChanged();
    void parameterLocaleNameChanged();
    void parameterValueChanged();
    void parameterUnitChanged();
    void parameterMinValueChanged();
    void parameterMaxValueChanged();
};

#endif // TESTRECIPE_H
