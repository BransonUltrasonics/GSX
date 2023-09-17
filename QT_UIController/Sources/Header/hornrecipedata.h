/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Horn Recipe Data

***************************************************************************/
#ifndef HORNRECIPEDATA_H
#define HORNRECIPEDATA_H

#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif

#ifndef QTCOMMON_H
#include "Header/qtcommon.h"
#endif

#ifndef RECIPE_H
#include "Header/recipe.h"
#endif

#include <Header/sysconfigdata.h>

#include <QObject>
#include <QString>
#include <QFile>
#include<QDebug>

//#define DEBUG

/* Indices for the horn recipe parameters */

#define HORN_RECIPE_SAVED_SUCCESS "1"
#define HORN_RECIPE_SAVED_FAILED "0"

#define HORN_RECIPE_OBJ_SIZE 20

#define PARAM_NOT_CHANGED 1

#define HORN_RECIPE_PARAM_COUNT 7

/* Horn Frequencies */
#define HORN_FREQ_20KHZ 20000
#define HORN_FREQ_30KHZ 30000
#define HORN_FREQ_40KHZ 40000

/* Limits for 20 khz horn */
#define HORN_START_FREQ_MIN_20KHZ 19450
#define HORN_STOP_FREQ_MAX_20KHZ 20450

/* Limits for 30khz horn */
#define HORN_START_FREQ_MIN_30KHZ 29250
#define HORN_STOP_FREQ_MAX_30KHZ 30750

/* Limits for 40 khz horn */
#define HORN_START_FREQ_MIN_40KHZ 38900
#define HORN_STOP_FREQ_MAX_40KHZ 40900

#define HORN_FREQ_STEP_MIN 1
#define HORN_FREQ_STEP_MAX 100

#define HORN_RECIPE_FREQ_STEP       QObject::tr("Frequency Step")
#define HORN_RECIPE_MAX_AMP         QObject::tr("Max Amplitude")
#define HORN_RECIPE_MAX_CUR         QObject::tr("Max Current")
#define HORN_RECIPE_SETDIGITAL      QObject::tr("Set Digital Tune with Horn Scan")
#define HORN_RECIPE_START_FREQ      QObject::tr("Start Frequency")
#define HORN_RECIPE_STOP_FREQ       QObject::tr("Stop Frequency")
#define HORN_RECIPE_TIME_DELAY      QObject::tr("Time Delay")
#define COMMA                       ","

#define HORN_GRAPH_AMP              QObject::tr("Amplitude")
#define HORN_GRAPH_RATEDCUR         QObject::tr("Rated Current")
#define HORN_GRAPH_PHASE            QObject::tr("Phase")
#define HORN_GRAPH_IMP              QObject::tr("Impedance")

using namespace std ;

//! A HornRecipeParameter Class
/*!
  This class derived from QObject, holds the horn recipe parameter data for each parameter
*/

struct t_hornRecipeStruct
{
    UINT32 startFreq ;
    UINT32 stopFreq ;
    UINT16 freqStep ;
    UINT16 timeDelay ;
    UINT16 maxAmp ;
    UINT16 maxCurrent ;
    UINT16 SetDigTuneWithHornScan;
};

enum StructureParamIndex
{
    HORN_FREQUENCY_STEP = 0,
    HORN_MAX_AMP,
    HORN_MAX_CURRENT,
    HORN_DIGITAL_TUNE,
    HORN_START_FREQUENCY,
    HORN_STOP_FREQUENCY,
    HORN_TIME_DELAY
};

class HornRecipeParameter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString parameterName READ getParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(QString parameterLocaleName READ getParameterLocaleName NOTIFY parameterLocaleNameChanged)
    Q_PROPERTY(QString parameterValue READ getParameterValue NOTIFY parameterValueChanged)
    Q_PROPERTY(QString parameterUnit READ getParameterUnit NOTIFY parameterUnitChanged)
    Q_PROPERTY(QString parameterMinValue READ getParameterMinValue NOTIFY parameterMinValueChanged)
    Q_PROPERTY(QString parameterMaxValue READ getParameterMaxValue NOTIFY parameterMaxValueChanged)
    Q_PROPERTY(bool parameterSwitch READ getParameterSwitch NOTIFY ParameterSwitchChanged)
    Q_PROPERTY(bool parameterSwitchState READ getParameterSwitchState NOTIFY ParameterSwitchStateChanged)

public :
    QString m_parameterName;
    QString m_parameterLocaleName;
    QString m_parameterValue;
    QString m_parameterUnit;
    QString m_parameterMinValue;
    QString m_parameterMaxValue;
    bool m_parameterSwitch;
    bool m_parameterSwitchState;

    HornRecipeParameter(QObject *parent=0);
    HornRecipeParameter(const QString &a_parameterName, const QString &a_parameterLocaleName, const QString &a_parameterVal,
                        const QString &a_parameterUnit,const QString &a_parameterMinVal,
                        const QString &a_parameterMaxVal,bool isSwitch = false,
                        bool switchState = false);
    ~HornRecipeParameter();

    QString getParameterName();
    QString getParameterLocaleName();
    QString getParameterValue();
    QString getParameterUnit();
    QString getParameterMinValue();
    QString getParameterMaxValue();
    bool getParameterSwitch();
    bool getParameterSwitchState();
    void setParameterValue(QString a_val);

signals:
    void parameterNameChanged();
    void parameterLocaleNameChanged();
    void parameterValueChanged();
    void parameterUnitChanged();
    void parameterMinValueChanged();
    void parameterMaxValueChanged();
    void ParameterSwitchChanged();
    void ParameterSwitchStateChanged();
};

//!  A hornRecipeData class.
/*!
  This class derived from QObject, Holds the horn recipe data and is the Back End interface
    between horn recipe parameter UI and DB
*/
/* Purpose: Holds the horn recipe data. Back End interface between horn recipe parameter UI and DB
*/
class HornRecipeData : public QObject
{
    Q_OBJECT

public:
    bool isHornAutoGraphRefreshState;
    bool isHornManualRefreshState;
    SystemFreq m_CurrentSystemFreq;
    Units m_MetrixUnitSelected;

    // constructor.
    /*
      This Constructor initializes and creates HornRecipeData object.
       * */
    explicit HornRecipeData();

    // Destructor.
    /*
      This Destructor destroys HornRecipeData object.
       * */
    ~HornRecipeData();

    Q_PROPERTY(bool autoHornGraphRefreshState READ getHornAutoGraphRefreshState WRITE setHornAutoGraphRefreshState NOTIFY HornAutoGraphRefreshStateChanged)
    Q_PROPERTY(bool isHornManualRefresh READ getIsHornManualRefreshState WRITE setIsHornManualRefreshState NOTIFY IsHornManualRefreshStateChanged)

    Q_INVOKABLE void setHornDefaultValues();
    Q_INVOKABLE void getHornDefaultValues();
    Q_INVOKABLE void modifyParameter(QString a_paramValue , QString a_paramIndex);
    Q_INVOKABLE void cancelUserData();
    Q_INVOKABLE void resetHornRecipeData();
    Q_INVOKABLE void setAxisIndex(int dwIndex, int isLeftSelected);
    Q_INVOKABLE void setCheckboxStatus(int dwIndex, bool isChecked);
    Q_INVOKABLE void updateParameterVal(QString amp, QString curr, QString phase, QString impedence);
    Q_INVOKABLE bool getPowerupLoadFlag();
    Q_INVOKABLE bool getParameterChangedFlag();
    Q_INVOKABLE bool getComboIndex(int dwIndex);
    Q_INVOKABLE bool getHornAutoGraphRefreshState();
    Q_INVOKABLE bool getIsHornManualRefreshState();
    Q_INVOKABLE unsigned int getHornPsFreq() ;
    Q_INVOKABLE int init();
    Q_INVOKABLE int saveUserData();
    Q_INVOKABLE int sendHornRecipeDataToSc();
    Q_INVOKABLE QString rangeCheck( QString a_paramValue , QString a_paramIndex);
    int reqToLoadRecipeData();
    int reqToSaveRecipeData();
    void updateDefaultParams();
    void setHornRecipeStructureValue(qint8 a_paramName, QString a_paramVal);
    void createHornRecipe();
    void initializeHornRecipeData();
    void setDefaultSystemValuesForScan(int dwMetrixUnit, int psFreqlVal);
    void getDefaultFrequencySelectionForScan();
    void createRightParameters();
    QString getHornRecipeStructureVal(int a_paramVal);
    QString getHornRecipeParameterNames();
    QString getHornRecipeLocaleParameterNames();
signals:
    /* Signal to be defined */
    void updateHornRecipeParams();
    void HornAutoGraphRefreshStateChanged();
    void IsHornManualRefreshStateChanged();

public slots:
    Q_INVOKABLE void setHornAutoGraphRefreshState(bool);
    Q_INVOKABLE void setIsHornManualRefreshState(bool);

private:
    bool m_dataLoadedOnPowerup = false;

    /* Object parameter holding the power supply freq */
    uint32_t m_psFreq = 0 ;
    /* Set this flag initially true because */
    bool m_parameterChanged = true;

    /* Horn Recipe UI model */
    QList<QObject*>m_hornRecipeParamList ;
    QList<QObject*>m_dummyHornRecipe ;

    /* Local copy of the horn Recipe DB data. This is updated at the power up of UI */
    QList<QString>m_hornRecipeLastSavedList ;

    /* Member structure to hold the default parameters based on the PS Freq */
    t_hornRecipeStruct hornRecipeDefault ;
    t_hornRecipeStruct hornRecipe;

    /* Check box parameters for Horn scan window */
    QList<QObject*> m_rightParameters ;
    QList<QObject*> m_rightParametersTemp ;
    QStringList qstrlstParamValues;
    //SystemFreq m_CurrentSystemFreq;
    QMap<QString,stRecipeParmInfo> m_MapHornRecipeParams;
    void clearList(const QString& modelName, QList<QObject*> list);
};

#endif // HORNRECIPEDATA_H
