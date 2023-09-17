#ifndef STACKRECIPEDATA_H
#define STACKRECIPEDATA_H

#ifndef COMMUNICATIONINTERFACE_H
    #include "Header/communicationinterface.h"
#endif

#ifndef QTCOMMON_H
#include "Header/qtcommon.h"
#endif

#include <QObject>
#include <QString>
#include <QFile>
#include<QDebug>

//#define DEBUG

/* Indices for the STACK recipe parameters */
enum {STACK_RECIPE_INDEX_1=0 , STACK_RECIPE_INDEX_2 , STACK_RECIPE_INDEX_3, STACK_RECIPE_INDEX_4 };

#define STACK_RECIPE_SAVED_SUCCESS "1"
#define STACK_RECIPE_SAVED_FAILED  "0"

#define STACK_RECIPE_OBJ_SIZE 20

#define PARAM_NOT_CHANGED 1

#define STACK_RECIPE_PARAM_COUNT 4

/* STACK Frequencies */
#define STACK_FREQ_20KHZ 20000
#define STACK_FREQ_30KHZ 30000
#define STACK_FREQ_40KHZ 40000

#define DEFAULT_STACK_FREQ_20KHZ 19950
#define DEFAULT_STACK_FREQ_30KHZ 30000
#define DEFAULT_STACK_FREQ_40KHZ 39900

/* Limits for 20 khz STACK */
#define STACK_FREQ_MIN_20KHZ 19450
#define STACK_FREQ_MAX_20KHZ 20450
#define STACK_FREQ_INTERNAL_OFFSET_MIN_20KHZ -500
#define STACK_FREQ_INTERNAL_OFFSET_MAX_20KHZ  500

/* Limits for 30khz STACK */
#define STACK_FREQ_MIN_30KHZ 29250
#define STACK_FREQ_MAX_30KHZ 30750
#define STACK_FREQ_INTERNAL_OFFSET_MIN_30KHZ -750
#define STACK_FREQ_INTERNAL_OFFSET_MAX_30KHZ  750

/* Limits for 40 khz STACK */
#define STACK_FREQ_MIN_40KHZ 38900
#define STACK_FREQ_MAX_40KHZ 40900
#define STACK_FREQ_INTERNAL_OFFSET_MIN_40KHZ -1000
#define STACK_FREQ_INTERNAL_OFFSET_MAX_40KHZ  1000

#define STACK_FREQ_STEP_MIN 1

using namespace std ;

//! A STACKRecipeParameter Class
/*!
  This class derived from QObject, holds the STACK recipe parameter data for each parameter
*/

struct StackRecipeStruct {
    int  DigitalTune ;
    int  InternalOffsetFlag;
    int  InternalFreqOffset ;
    int  EndofWeldStore;
};

class StackRecipeParameter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString parameterName READ getParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(quint32 parameterValue READ getParameterValue NOTIFY parameterValueChanged)
    Q_PROPERTY(QString parameterUnit READ getParameterUnit NOTIFY parameterUnitChanged)

public :
    StackRecipeParameter(const QString &a_parameterName , const quint32 &a_parameterVal, const QString &a_parameterUnit);
    ~StackRecipeParameter();

    QString getParameterName();
    quint32 getParameterValue();
    QString getParameterUnit();

    void setParameterValue(quint32 a_val);

signals:
    void parameterNameChanged();
    void parameterValueChanged();
    void parameterUnitChanged();

public:
    QString m_parameterName;
    quint32 m_parameterValue;
    QString m_parameterUnit;
};

//!  A StackRecipeData class.
/*!
  This class derived from QObject, Holds the horn recipe data and is the Back End interface
    between horn recipe parameter UI and DB
*/
/* Purpose: Holds the horn recipe data. Back End interface between horn recipe parameter UI and DB
*/
class StackRecipeData : public QObject
{
    Q_OBJECT

public:
    // constructor.
    /*
      This Constructor initializes and creates StackRecipeData object.
       * */
    explicit StackRecipeData();

    /* Horn Recipe UI model */
    QList<QObject*>m_StackRecipeParamList ;

    // Destructor.
    /*
      This Destructor destroys StackRecipeData object.
       * */
    ~StackRecipeData();

    Q_INVOKABLE int sendStackRecipeDataToSc();
    Q_INVOKABLE void modifyParameter(QString a_paramValue , qint8 a_paramIndex);
    Q_INVOKABLE void cancelUserData();
    Q_INVOKABLE int resetStackRecipeData();
    Q_INVOKABLE bool getPowerupLoadFlag();
    Q_INVOKABLE bool getParameterChangedFlag();
   // Q_INVOKABLE unsigned int getStackPsFreq() ;
    Q_INVOKABLE int init();
    Q_INVOKABLE int rangeCheck( QString a_paramValue , qint8 a_paramIndex) ;
    Q_INVOKABLE int saveUserData();

    void setStackRecipeParameter(QString a_val , qint8 a_paramIndex );
    int reqToLoadRecipeData();
    int reqToSaveRecipeData();
    void updateDefaultParams();

signals:
    /* Signal to be defined */
    void updateStackRecipeParams();

private:
    bool m_dataLoadedOnPowerup = false;

    /* Object parameter holding the power supply freq */
    uint32_t m_psFreq = 0 ;
    /* Set this flag initially true because */
    bool m_parameterChanged = true;

    /* Parameter to check step freq max */
    uint16_t m_stepFreqMax = 0 ;

    QList<QObject*>m_dummyRecipeParamList ;

    /* Local copy of the horn Recipe DB data. This is updated at the power up of UI */
    QList<quint32>m_StackRecipeLastSavedList ;

    /* Member structure to hold the default parameters based on the PS Freq */
    StackRecipeStruct StackRecipeDefault ;

 public:
    static int RecipeNumber;
};

#endif // STACKRECIPEDATA_H
