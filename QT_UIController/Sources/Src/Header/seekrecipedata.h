/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Seek Recipe Data

***************************************************************************/
#ifndef SEEKRECIPEDATA_H
#define SEEKRECIPEDATA_H

#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif

#ifndef QTCOMMON_H
#include "Header/qtcommon.h"
#endif

#ifndef RECIPE_H
#include "Header/recipe.h"
#endif

#include <QObject>
#include <QString>
#include <QFile>
#include<QDebug>
#include <Header/checkbox.h>
#include <Header/sysconfigdata.h>

//#define DEBUG

#define SEEK_RECIPE_SAVED_SUCCESS "1"
#define SEEK_RECIPE_SAVED_FAILED "0"

#define SEEK_RECIPE_OBJ_SIZE 20

#define PARAM_NOT_CHANGED 1

#define SEEK_RECIPE_PARAM_COUNT 10

/* SEEK Frequencies */
#define SEEK_FREQ_20KHZ 20000
#define SEEK_FREQ_30KHZ 30000
#define SEEK_FREQ_40KHZ 40000

/* Limits for 20 khz seek */
#define SEEK_START_FREQ_MIN_20KHZ 19450
#define SEEK_STOP_FREQ_MAX_20KHZ 20449
#define SEEK_FREQ_STEP_MAX_20KHZ 1000

/* Limits for 30khz seek */
#define SEEK_START_FREQ_MIN_30KHZ 29250
#define SEEK_STOP_FREQ_MAX_30KHZ 30749
#define SEEK_FREQ_STEP_MAX_30KHZ 1500

/* Limits for 40 khz seek */
#define SEEK_START_FREQ_MIN_40KHZ 39000
#define SEEK_STOP_FREQ_MAX_40KHZ 40999
#define SEEK_FREQ_STEP_MAX_40KHZ 2000

#define SEEK_FREQ_STEP_MIN 1

#define SEEK_RECP_TIME               QObject::tr("Seek Time")
#define SEEK_RECP_RAMP               QObject::tr("Seek Ramp Time")
#define SEEK_RECP_OFFSET             QObject::tr("Seek Frequency Offset")
#define SEEK_RECP_MEMORY             QObject::tr("Memory Clear Before Seek")
#define SEEK_RECP_AMPLOOPC1          QObject::tr("Seek Amplitude Loop C1")
#define SEEK_RECP_AMPLOOPC2          QObject::tr("Seek Amplitude Loop C2")
#define SEEK_RECP_FREQ_LIMIT_HIGH    QObject::tr("Seek Frequency Limit High")
#define SEEK_RECP_FREQ_LIMIT_LOW     QObject::tr("Seek Frequency Limit Low")
#define SEEK_RECP_PHASE_LOOP_I       QObject::tr("Seek Phase loop I")
#define SEEK_RECP_PHASE_LOOP_CF      QObject::tr("Seek Phase loop CF")
#define COMMA ","

#define SEEK_GRAPH_AMP              QObject::tr("Amplitude")
#define SEEK_GRAPH_FRQ              QObject::tr("Frequency")
#define SEEK_GRAPH_POW              QObject::tr("Power")
#define SEEK_GRAPH_RATEDCUR         QObject::tr("Rated Current")
#define SEEK_GRAPH_PHASE            QObject::tr("Phase")

using namespace std ;

//! A seekRecipeParameter Class
/*!
  This class derived from QObject, holds the seek recipe parameter data for each parameter
*/

struct seekRecipeStruct {
    INT32 SeekAmplitudeLoopC1 ;
    INT32 SeekAmplitudeLoopC2 ;
    INT32 SeekFreqLimitHigh ;
    INT32 SeekFreqLimitLow;
    INT32 SeekPhaseloopI ;
    INT32 SeekPhaseloopCF ;
    INT32 SeekRampTime;
    INT32 SeekFreqOffset ;
    INT8  SeekMemClearbeforeSeek;
    UINT32 SeekTime ;
} ;
enum StructureParamIndexSeek
{
    SEEK_TIME = 0,
    SEEK_RAMP_TIME,
    SEEK_FREQ_OFFSET,
    SEEK_MEMORY_CLEAR_BEFORE_SEEK,
    SEEK_AMP_LOOPC1,
    SEEK_AMP_LOOPC2,
    SEEK_FREQ_LIMIT_HIGH,
    SEEK_FREQ_LIMIT_LOW,
    SEEK_PHASE_LOOPI,
    SEEK_PHASE_LOOPCF
};
class SeekRecipeParameter : public QObject
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
    SeekRecipeParameter(QObject *parent=0);
    SeekRecipeParameter(const QString &a_parameterName, const QString &a_parameterLocaleName, const QString &a_parameterVal,
                        const QString &a_parameterUnit,const QString &a_parameterMinVal,
                        const QString &a_parameterMaxVal,bool isSwitch = false,
                        bool switchState = false);
    ~SeekRecipeParameter();

    QString m_parameterName;
    QString m_parameterLocaleName;
    QString m_parameterValue;
    QString m_parameterUnit;
    QString m_parameterMinValue;
    QString m_parameterMaxValue;
    bool m_parameterSwitch;
    bool m_parameterSwitchState;

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

//!  A seekRecipeData class.
/*!
  This class derived from QObject, Holds the horn recipe data and is the Back End interface
    between horn recipe parameter UI and DB
*/
/* Purpose: Holds the horn recipe data. Back End interface between horn recipe parameter UI and DB
*/
class SeekRecipeData : public QObject
{
    Q_OBJECT

public:
    // constructor.
    /*
      This Constructor initializes and creates SeekRecipeData object.
       * */
    explicit SeekRecipeData();

    // Destructor.
    /*
      This Destructor destroys SeekRecipeData object.
       * */
    ~SeekRecipeData();

    Q_INVOKABLE int sendSeekRecipeDataToSc();
    Q_INVOKABLE void modifyParameter(QString a_paramValue , QString a_paramIndex);
    Q_INVOKABLE void cancelUserData();
    Q_INVOKABLE void resetSeekRecipeData();
    Q_INVOKABLE bool getPowerupLoadFlag();
    Q_INVOKABLE bool getParameterChangedFlag();
    Q_INVOKABLE unsigned int getSeekPsFreq() ;
    Q_INVOKABLE int init();
    Q_INVOKABLE QString rangeCheck( QString a_paramValue , QString a_paramIndex) ;
    Q_INVOKABLE void setParamValues(int dwAmp, int dwFreq, int dwPower, int dwCurr, int dwPhase);
    Q_INVOKABLE int getComboIndex(int dwIndex);
    Q_INVOKABLE int saveUserData();
    int reqToLoadRecipeData();
    int reqToSaveRecipeData();
    void updateDefaultParams();
    Q_INVOKABLE void setCheckboxStatus(int dwIndex, bool isChecked);
    Q_INVOKABLE void setAxisIndex(int dwIndex, int isLeftSelected);
    Q_INVOKABLE void setSeekCheckBoxModelData();
    Q_INVOKABLE void setSeekDefaultValues();
    Q_INVOKABLE void getSeekDefaultValues();
    void initializeSeekRecipeData();
    void getDefaultFrequencySelectionForSeek();
    void setDefaultSystemValuesForSeek(int dwMetrixUnit, int psFreqGlobalVar);
    QString getSeekRecipeParameterNames();
    QString getLocaleSeekRecipeParameterNames();
    void setSeekRecipeStructureValue(qint8 a_paramIndex, QString a_paramVal);
    QString getSeekRecipeStructureVal(int dwParamIndex);
    void createSeekRecipe();
signals:
    /* Signal to be defined */
    void updateSeekRecipeParams();

public slots:

private:
    bool m_dataLoadedOnPowerup = false;

    /* Object parameter holding the power supply freq */
    uint32_t m_psFreq = 0 ;
    /* Set this flag initially true because */
    bool m_parameterChanged = true;

    /* Parameter to check step freq max */
    uint16_t m_stepFreqMax = 0 ;

    /* Horn Recipe UI model */
    QList<QObject*>m_seekRecipeParamList ;

    QList<QObject*>m_dummyRecipeParamList ;

    /* Local copy of the horn Recipe DB data. This is updated at the power up of UI */
    QList<double>m_seekRecipeLastSavedList ;
    /* Member structure to hold the default parameters based on the PS Freq */
    seekRecipeStruct seekRecipeDefault ;
    seekRecipeStruct seekRecipe;

    QList<QObject*> m_seekCheckBoxData;
    QList<QObject*> m_seekCheckBoxDataTemp;
    QMap<QString,stRecipeParmInfo> m_MapSeekRecipeParams;

    SystemFreq m_CurrentSystemFreq;
    Units m_MetrixUnitSelected;
};


#endif // SEEKRECIPEDATA_H
