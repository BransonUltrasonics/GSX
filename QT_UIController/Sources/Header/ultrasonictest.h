/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Test Mode

***************************************************************************/

#ifndef ULTRASONICTEST_H
#define ULTRASONICTEST_H

#include <QObject>
#include <QList>
#include <QQmlContext>

#include "Header/checkbox.h"
#include "Header/testmodeprogressbar.h"
#include "Header/testrecipe.h"
#include "Header/communicationinterface.h"
#include "Header/testmodecheckboxdata.h"
#include "Header/configuration.h"
#include <Header/sysconfigdata.h>
#include "Header/recipe.h"
//
#define TEST_RECIPE_TEST_AMPLITUDE      QObject::tr("Test Amplitude")
#define TEST_RECIPE_AMPLITUDE_C1        QObject::tr("Amplitude Loop C1")
#define TEST_RECIPE_AMPLITUDE_C2        QObject::tr("Amplitude Loop C2")
#define TEST_RECIPE_PHASE_LOOP          QObject::tr("Phase Loop I")
#define TEST_RECIPE_PHASE_CF            QObject::tr("Phase Loop CF")
#define TEST_RECIPE_FREQUENCY_HIGH      QObject::tr("Frequency High")
#define TEST_RECIPE_FREQUENCY_LOW       QObject::tr("Frequency Low")
#define TEST_RECIPE_READY_STATUS        QObject::tr("Ready")
#define COMMA                           ","
enum StructureParamIndexTest
{
    TEST_AMPLITUDE_PER = 0,
    TEST_AMPLITUDE_LOOP_C1 ,
    TEST_AMPLITUDE_LOOP_C2,
    TEST_PHASE_LOOP1,
    TEST_PHASE_LOOP_CF,
    TEST_FREQ_HIGH,
    TEST_FREQ_LOW
};


class Range;
/**
 * @brief The UltrasonicTest class : handles test mode
 */
class UltrasonicTest : public QObject
{

    Q_PROPERTY(QString StatusText READ getStatusText WRITE setStatusText NOTIFY statusTextChanged)
    Q_OBJECT
public:
    UltrasonicTest(QObject *parent=0);
    QString getStatusText();
    int getPsFrequency();

    Q_INVOKABLE void setCheckBoxModelData();
    Q_INVOKABLE void startTest();
    Q_INVOKABLE void getTestModeAdvancedDataFromDB();
    Q_INVOKABLE bool sendTestRecipeDataToSC();
    Q_INVOKABLE bool saveTestRecipeData();
    Q_INVOKABLE void cancelTestRecipeData();
    Q_INVOKABLE void resetTestRecipeData();
    Q_INVOKABLE void getTestModeProgressbarDataFromSC(QString);
    Q_INVOKABLE bool sendTestResponseToSC();
    Q_INVOKABLE void setCheckboxStatus(int dwIndex, bool isChecked);
    Q_INVOKABLE void setAxisIndex(int dwIndex, int isLeftSelected);
    Q_INVOKABLE QString rangeCheck(QString,QString);
    Q_INVOKABLE void setTestDefaultValues();
    Q_INVOKABLE void getTestDefaultValues();

    void addParameterForBranson();
    void setStatusText(QString a_Status);

    Q_INVOKABLE void createTestRecipe();
    QString getTestRecipeParameterNames();
    QString getTestRecipeLocaleParameterNames();
private:
    QList<QObject*> m_testModeAdvancedDataTemp;
    QList<QObject*> m_listTestModeProgressbarTemp;
    QList<QObject*> m_listTestModeProgressbar;
    QList<QObject*> m_listTestModeCheckBoxDataTemp;
    QList<QObject*> m_listTestModeCheckBoxData;
    QList<QObject*> m_dummyListTestModeCheckBoxData;
    QList<QObject*> m_testModeAdvancedData;
    std::map<int,QList<Range> > m_RangeMap;
    QString m_StatusText;
    TestRecipeData m_TestRecipeData;
    char m_StartTestCommand;
signals:

    void statusTextChanged();

public:
    QMap<QString,stRecipeParmInfo> m_MapTestRecipeParams;
    SystemFreq m_CurrentSystemFreq;
    Units m_MetrixUnitSelected;
    TestRecipe m_stTestRecipe;
    TestRecipe m_st_tempTestRecipe;
    TestRecipe m_TestRecipeStrDefault;

    uint32_t m_psFreq = 0 ;


    void initializeTestRecipeData();
    void getDefaultFrequencySelectionForTest();
    QString getTestRecipeStructureVal(int dwParamIndex);
    Q_INVOKABLE void setDefaultSystemValuesForTest(int dwMetrixUnit, int psFreqGlobalVar);
    Q_INVOKABLE void setTestRecipeStructureValue(qint8 a_paramIndex, QString a_paramVal);
    void updateDefaultParams();
    Q_INVOKABLE void modifyParameter(QString a_paramValue, QString a_paramName);
    int reqToLoadRecipeData();
    Q_INVOKABLE int init();
};
/**
 * @brief The Range class : object of the class contains minimum and maximum values of a parameter
 */
class Range
{
public:
    int min;
    int max;
    Range(int a_min,int a_max)
    {
        min=a_min;
        max=a_max;
    }
};

#endif // ULTRASONICTEST_H
