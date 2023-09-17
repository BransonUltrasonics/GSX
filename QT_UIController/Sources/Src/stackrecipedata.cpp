 #ifndef HORNRECIPEDATA_H
#include "Header/stackrecipedata.h"
#include "Header/common.h"
#endif

#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif

#include<QDebug>
#include<QFile>
#include <QQmlContext>
#include <Header/sysconfigdata.h>

using namespace std ;

extern QQmlContext* context;
extern CommunicationInterface CIObj;

#define STACK_DIGITAL                QObject::tr("Digital Tune")
#define STACK_INTERNALOFF            QObject::tr("Internal Offset Flag")
#define STACK_INTERNALFREQ           QObject::tr("Internal Freq Offset")
#define STACK_WELDSTORE              QObject::tr("End of Weld Store")


/* In the future the extern implementation of sysconfig data to remove and read from global class */
extern unsigned int psFreqGlobalVar;
int StackRecipeData::RecipeNumber=0;

/**
 * @brief StackRecipeParameter::StackRecipeParameter : Initialization of class members and memory for object
 * @param a_parameterName
 * @param a_parameterVal
 * @param a_parameterUnit
 */
StackRecipeParameter ::StackRecipeParameter(const QString &a_parameterName , const quint32 &a_parameterVal, const QString &a_parameterUnit)
{
    m_parameterName = a_parameterName ;
    m_parameterValue = a_parameterVal;
    m_parameterUnit = a_parameterUnit;
}

/**
 * @brief StackRecipeParameter::~StackRecipeParameter : Destructor
 */
StackRecipeParameter :: ~StackRecipeParameter()
{
}

/**
 * @brief StackRecipeParameter::getParameterName : Return the Stack Recipe Name
 * @return
 */
QString StackRecipeParameter :: getParameterName()
{
    return m_parameterName;
}

/**
 * @brief StackRecipeParameter::getParameterUnit : Return the recipe unit
 * @return
 */
QString StackRecipeParameter :: getParameterUnit()
{
    return m_parameterUnit;
}

/**
 * @brief StackRecipeParameter::getParameterValue
 * @return
 */
quint32 StackRecipeParameter :: getParameterValue()
{
    return m_parameterValue ;
}

/**
 * @brief StackRecipeParameter::setParameterValue
 * @param a_val
 */
void StackRecipeParameter :: setParameterValue(quint32 a_val)
{
    m_parameterValue = a_val;
}

/**
 * @brief StackRecipeData::StackRecipeData : Adding stack recipe parameter to list
 */
StackRecipeData ::StackRecipeData()
{
    StackRecipeParameter* tmpStackRecipeParam;

    tmpStackRecipeParam = new StackRecipeParameter(STACK_DIGITAL, 19950, HERTZ);
    m_StackRecipeParamList.append(tmpStackRecipeParam);

    tmpStackRecipeParam = new StackRecipeParameter(STACK_INTERNALOFF, 0, "");
    m_StackRecipeParamList.append(tmpStackRecipeParam);

    tmpStackRecipeParam = new StackRecipeParameter(STACK_INTERNALFREQ, 0, HERTZ);
    m_StackRecipeParamList.append(tmpStackRecipeParam);

    tmpStackRecipeParam = new StackRecipeParameter(STACK_WELDSTORE, 0, "");
    m_StackRecipeParamList.append(tmpStackRecipeParam);

    m_StackRecipeLastSavedList.append(0);
    m_StackRecipeLastSavedList.append(0);
    m_StackRecipeLastSavedList.append(0);
    m_StackRecipeLastSavedList.append(0);
}

/**
 * @brief StackRecipeData::init : Refresh updated values
 * @return int success
 */
int StackRecipeData :: init()
{
    int ind=0;
    int retValLoadRecipe = OPERATION_SUCCESS ;
    StackRecipeParameter* tmpRecipe ;

    context->setContextProperty("stackRecipeParameterModel",QVariant::fromValue(m_StackRecipeParamList));

    if(1)
    {
           /* Update the PS Freq Object parameter */
           m_psFreq = psFreqGlobalVar ;

           /* Updating the default parameter list */
           updateDefaultParams();

           retValLoadRecipe = reqToLoadRecipeData();

           if(OPERATION_SUCCESS != retValLoadRecipe)
           {
              resetStackRecipeData();
              /* Updating the last saved values with the default values at powerup when no recipe is found from DB */
              for(ind = 0 ; ind < STACK_RECIPE_PARAM_COUNT ; ind++)
              {
                  tmpRecipe = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(ind));
                  m_StackRecipeLastSavedList[ind] =  tmpRecipe->getParameterValue();
              }
            }
            else
            {
                   /* Update the Last saved values in the DB with the current values */
                   /* This is done only once at the first time the horn scan screen is loaded */
               for(ind = 0 ; ind < STACK_RECIPE_PARAM_COUNT ; ind++)
               {
                  tmpRecipe = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(ind));
                  m_StackRecipeLastSavedList[ind] =  tmpRecipe->getParameterValue();
               }
             }
             m_dataLoadedOnPowerup = true ;
             m_parameterChanged = false ;
      }
      else
      {
            /* Cancel any modifications done to the recipe and not saved by the user */
            /* We do this forcefully each time to ensure there is no unsaved data */
         cancelUserData();
      }

    return retValLoadRecipe ;
}

/**
 * @brief StackRecipeData::reqToLoadRecipeData : Send Recipe data to BL
 * @return
 */
int StackRecipeData :: reqToLoadRecipeData()
{
    QString data;
    char paramInd = 0 ;
    QStringList StackRecipeFromDb;
    QString StackRecipeMsg;
    QString stringRead ;
    int retValue = OPERATION_SUCCESS ;

    INT32 RecipeNumber;

    RecipeNumber=StackRecipeData::RecipeNumber;

    /* Send message to get default horn recipe */
    CIObj.sendMessage(REQ_GET_STACK_RECIPE_DATA , RES_GET_STACK_RECIPE_DATA , QString::number(RecipeNumber));
    CIObj.recivedMessage(RES_GET_STACK_RECIPE_DATA ,StackRecipeMsg);

    /* Filter the string with the delimiter */
    StackRecipeFromDb =StackRecipeMsg.split(',');

    /* Check for the length of the msg received */
    if(1 >=StackRecipeFromDb.length())
    {
        retValue = NO_PARAM ;
    }
    else
    {
        /* Check the range and update the parameters with the current values */
        for(paramInd = 0 ; paramInd <StackRecipeFromDb.length() && paramInd < STACK_RECIPE_PARAM_COUNT ; paramInd++)
        {
            stringRead =StackRecipeFromDb.at(paramInd);

            retValue = rangeCheck( stringRead , paramInd);
            if(PARAM_IN_RANGE != retValue)
            {
                break;
            }
            else
            {
                setStackRecipeParameter(stringRead , paramInd );
            }
        }
    }

    return retValue ;
}

/**
 * @brief StackRecipeData::reqToSaveRecipeData : Save the Recipe data
 * @return int success
 */
int StackRecipeData :: reqToSaveRecipeData()
{
   StackRecipeParameter* tmpRecipeParam ;

    QString reqSaveRecipeDbMsg = "";
    QString resSaveRecipeDbMsg = "";

    /* Get the stack recipe parameters */
    INT32 a_DigitalTune;
    INT32 a_InternalOffsetFlag;
    INT32 a_InternalFreqOffset ;
    INT32 a_EndofWeldStore;
    INT32 RecipeNumber;

    RecipeNumber = StackRecipeData::RecipeNumber;


    int retValue = OPERATION_SUCCESS ;

    tmpRecipeParam = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(0));
    a_DigitalTune = (INT32)tmpRecipeParam->getParameterValue();

    tmpRecipeParam = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(1));
     a_InternalOffsetFlag = (INT32)tmpRecipeParam->getParameterValue();

    tmpRecipeParam = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(2));
    a_InternalFreqOffset = (INT32)tmpRecipeParam->getParameterValue();

    tmpRecipeParam = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(3));
    a_EndofWeldStore = (INT32)tmpRecipeParam->getParameterValue();


    /* Generate the STACK recipe msg to DB */
    reqSaveRecipeDbMsg = QString::number(RecipeNumber)+","
            + QString :: number(a_DigitalTune) + ","
            + QString :: number(a_InternalOffsetFlag) + ","
            + QString :: number(a_InternalFreqOffset) +","
            + QString :: number(a_EndofWeldStore);


    /* Sending STACK recipe message */
    CIObj.sendMessage(REQ_SET_STACK_RECIPE_DATA, RES_SET_STACK_RECIPE_DATA, reqSaveRecipeDbMsg);

    /* Receive the STACK recipe response message */
    CIObj.recivedMessage(RES_SET_STACK_RECIPE_DATA, resSaveRecipeDbMsg);

    if("" == resSaveRecipeDbMsg)
    {
        retValue = NO_PARAM ;
    }
    else
    {
        if(STACK_RECIPE_SAVED_SUCCESS != resSaveRecipeDbMsg.toStdString())
        {
            retValue = OPERATION_FAILURE ;
        }
    }

    return retValue ;
}

/**
 * @brief StackRecipeData::sendStackRecipeDataToSc : Send stack recipe data to BL
 * @return
 */
int StackRecipeData :: sendStackRecipeDataToSc()
{
    int status = 0;

    //char StackRecipeResponseMsg[1];
    
    StackRecipeStruct send;
    
    /*TBD stack recipe will be saved and send as it is common to all weld recipes*/ 
    saveUserData();
    
    send.DigitalTune               = m_StackRecipeLastSavedList.at(STACK_RECIPE_INDEX_1);
    send.InternalOffsetFlag        = m_StackRecipeLastSavedList.at(STACK_RECIPE_INDEX_2);
    send.InternalFreqOffset        = m_StackRecipeLastSavedList.at(STACK_RECIPE_INDEX_3);
    send.EndofWeldStore            = m_StackRecipeLastSavedList.at(STACK_RECIPE_INDEX_4);


    char *buff=reinterpret_cast<char *>(&send);

    /* Sending horn recipe message */
    CIObj.sendMessage(SCBL_STACK_RECIPE_REQ, UIC_STACK_RECIPE_RES, buff, sizeof(send));

    /* Receive the horn recipe response message */
    //CIObj.recivedMessage(UIC_STACK_RECIPE_RES,StackRecipeResponseMsg , 1);

    //status = (int)StackRecipeResponseMsg[0];

    return status ;

}

/**
 * @brief StackRecipeData::saveUserData : Save user data and update model
 * @return scuess response
 */
int StackRecipeData :: saveUserData()
{
   StackRecipeParameter* tmpRecipe;
    int ind = 0;
    int retVal = OPERATION_SUCCESS;

    context->setContextProperty("stackRecipeParameterModel" , QVariant::fromValue(m_dummyRecipeParamList));
    /* Request to save the current STACK recipe data to DB */
    retVal = reqToSaveRecipeData();

    /* Failure : Retrieve the values from the DB and update the current values. Show pop up indicating save failed */
    /* Success : Update the Last saved values with the current parameter values */
    if(OPERATION_SUCCESS != retVal)
    {
        for(ind = 0 ; ind < STACK_RECIPE_PARAM_COUNT ; ind++)
        {
            tmpRecipe = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(ind));

            tmpRecipe->setParameterValue(m_StackRecipeLastSavedList.at(ind));
        }
    }
    else
    {
        for(ind = 0 ; ind < STACK_RECIPE_PARAM_COUNT ; ind++)
        {
            tmpRecipe = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(ind));

            m_StackRecipeLastSavedList[ind] =  tmpRecipe->getParameterValue();
        }
    }

    m_parameterChanged = false ;
    /* Update the STACK recipe UI */
    context->setContextProperty("stackRecipeParameterModel",QVariant::fromValue(m_StackRecipeParamList));

    return retVal ;
}

/**
 * @brief StackRecipeData::cancelUserData : Cancel user data and update the model
 */
void StackRecipeData :: cancelUserData()
{
    context->setContextProperty("stackRecipeParameterModel" , QVariant::fromValue(m_dummyRecipeParamList));
   StackRecipeParameter* tmpRecipe;
    int ind = 0;

    for(ind = 0 ; ind < STACK_RECIPE_PARAM_COUNT ; ind++)
    {
        tmpRecipe = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(ind));

        tmpRecipe->setParameterValue(m_StackRecipeLastSavedList.at(ind));
    }

    m_parameterChanged = false ;
    /* Update the STACK recipe UI */
    context->setContextProperty("stackRecipeParameterModel",QVariant::fromValue(m_StackRecipeParamList));
}

/**
 * @brief StackRecipeData::resetStackRecipeData : Reset the stack recipe data to default
 * @return success
 */
int StackRecipeData :: resetStackRecipeData()
{
   StackRecipeParameter* tmpRecipe;
    QList<quint32>StackRecipeDefaultValues ;
    int ind = 0 ;

    context->setContextProperty("stackRecipeParameterModel" , QVariant::fromValue(m_dummyRecipeParamList));
   StackRecipeDefaultValues.reserve(STACK_RECIPE_PARAM_COUNT);

   StackRecipeDefaultValues.insert(STACK_RECIPE_INDEX_1 ,StackRecipeDefault.DigitalTune);
   StackRecipeDefaultValues.insert(STACK_RECIPE_INDEX_2 ,StackRecipeDefault.InternalOffsetFlag);
   StackRecipeDefaultValues.insert(STACK_RECIPE_INDEX_3 ,StackRecipeDefault.InternalFreqOffset);
   StackRecipeDefaultValues.insert(STACK_RECIPE_INDEX_4 ,StackRecipeDefault.EndofWeldStore);


    /* Update the current values with the reset values */
    for(ind = 0 ; ind < STACK_RECIPE_PARAM_COUNT ; ind++)
    {
        tmpRecipe = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(ind));

        tmpRecipe->setParameterValue(StackRecipeDefaultValues.at(ind));

        if(tmpRecipe->getParameterValue() != m_StackRecipeLastSavedList.at(ind))
        {
            m_parameterChanged = true;
        }
    }

    /* Update the Horn recipe UI */
    context->setContextProperty("stackRecipeParameterModel",QVariant::fromValue(m_StackRecipeParamList));

    return OPERATION_SUCCESS ;
}

/**
 * @brief StackRecipeData::rangeCheck : Checking the validation limits
 * @param a_tmpValue
 * @param a_paramIndex
 * @return range limits
 */
int StackRecipeData :: rangeCheck( QString a_tmpValue , qint8 a_paramIndex)
{
    int returnValue = PARAM_IN_RANGE ;

    string paramValue = a_tmpValue.toStdString();

    /* Parameter to hold the start freq and stop freq value */
    unsigned char length = 0;


    /* Check Type - All parameters */
    length = strlen(paramValue.c_str());

    if( (length == 0) || (length > 10) || ('.' == paramValue[0]))
    {
       if('.' == paramValue[0])
       {
         returnValue = INCORRECT_PARAM ;
       }
       else
       {
         returnValue = PARAM_OUT_OF_RANGE;
       }
    }
    else
    {
       int value = a_tmpValue.toInt();
       switch(a_paramIndex)
       {
         case STACK_RECIPE_INDEX_1:
                if (m_psFreq==STACK_FREQ_20KHZ)
                {
                    if(value < STACK_FREQ_MIN_20KHZ|| value > STACK_FREQ_MAX_20KHZ)
                    {
                      returnValue = PARAM_OUT_OF_RANGE ;
                    }
                }
                else if (m_psFreq==STACK_FREQ_30KHZ)
                {
                    if(value < STACK_FREQ_MIN_30KHZ|| value > STACK_FREQ_MAX_30KHZ)
                    {
                     returnValue = PARAM_OUT_OF_RANGE ;
                    }
                }
                else if (m_psFreq==STACK_FREQ_40KHZ)
                {
                    if(value < STACK_FREQ_MIN_40KHZ|| value > STACK_FREQ_MAX_40KHZ)
                    {
                     returnValue = PARAM_OUT_OF_RANGE ;
                    }
                }

                break;
           case STACK_RECIPE_INDEX_2:
             if(value < 0 || value > 1)
             {
              returnValue = PARAM_OUT_OF_RANGE ;
             }
            break;

            case STACK_RECIPE_INDEX_3:
                 //switch(m_psFreq)
                // {
                   if (m_psFreq==STACK_FREQ_20KHZ)
                   {
                      if(value < STACK_FREQ_INTERNAL_OFFSET_MIN_20KHZ || value > STACK_FREQ_INTERNAL_OFFSET_MAX_20KHZ )
                       {
                          returnValue = PARAM_OUT_OF_RANGE ;
                       }
                   }
                  else if (m_psFreq==STACK_FREQ_30KHZ)
                   {
                       if(value < STACK_FREQ_INTERNAL_OFFSET_MIN_30KHZ || value > STACK_FREQ_INTERNAL_OFFSET_MAX_30KHZ )
                       {
                           returnValue = PARAM_OUT_OF_RANGE ;
                       }
                    }
                   else if (m_psFreq==STACK_FREQ_40KHZ)
                    {
                       if(value < STACK_FREQ_INTERNAL_OFFSET_MIN_40KHZ || value > STACK_FREQ_INTERNAL_OFFSET_MAX_40KHZ )
                        {
                          returnValue = PARAM_OUT_OF_RANGE ;
                        }
                    }
                  break ;
              case STACK_RECIPE_INDEX_4:
              if(value < 0 || value > 1)
              {
                returnValue = PARAM_OUT_OF_RANGE ;
              }
              break;

                default:
                    returnValue = INCORRECT_PARAM ;
                    break;
                }
            }
    return returnValue ;
}

/**
 * @brief StackRecipeData::setStackRecipeParameter : Set recipe parameter name based on index
 * @param a_tmpVal
 * @param a_paramIndex
 */
void StackRecipeData :: setStackRecipeParameter(QString a_tmpVal , qint8 a_paramIndex)
{
   StackRecipeParameter* tmpRecipe ;

    context->setContextProperty("stackRecipeParameterModel" , QVariant::fromValue(m_dummyRecipeParamList));
    tmpRecipe = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(a_paramIndex));

    tmpRecipe->setParameterValue(stoul(a_tmpVal.toStdString()));

    if(tmpRecipe->getParameterValue() != m_StackRecipeLastSavedList.at(a_paramIndex))
    {
        m_parameterChanged = true;
    }

    context->setContextProperty("stackRecipeParameterModel",QVariant::fromValue(m_StackRecipeParamList));
}

/**
 * @brief StackRecipeData::modifyParameter : Modified values based on index
 * @param a_paramValue
 * @param a_paramIndex
 */
void StackRecipeData :: modifyParameter(QString a_paramValue , qint8 a_paramIndex)
{
    int retVal = 0 ;

    retVal = rangeCheck(a_paramValue , a_paramIndex);

    if(0 == retVal)
    {
        setStackRecipeParameter(a_paramValue , a_paramIndex);
        sendStackRecipeDataToSc();
    }
}

/**
 * @brief StackRecipeData::getPowerupLoadFlag
 * @return
 */
bool StackRecipeData :: getPowerupLoadFlag()
{
    return m_dataLoadedOnPowerup;
}

/**
 * @brief StackRecipeData::getParameterChangedFlag
 * @return
 */
bool StackRecipeData :: getParameterChangedFlag()
{
    return m_parameterChanged ;
}

/**
 * @brief StackRecipeData::updateDefaultParams : Default parameter values to load in recipe
 */
void StackRecipeData :: updateDefaultParams()
{
    switch(m_psFreq)
    {
    // to be debug
    case STACK_FREQ_20KHZ :
       StackRecipeDefault.DigitalTune = DEFAULT_STACK_FREQ_20KHZ;
       StackRecipeDefault.InternalOffsetFlag=0;
       StackRecipeDefault.InternalFreqOffset = 0;
       StackRecipeDefault.EndofWeldStore = 0;
        break ;
    case STACK_FREQ_30KHZ:
       StackRecipeDefault.DigitalTune = DEFAULT_STACK_FREQ_30KHZ;
        StackRecipeDefault.InternalOffsetFlag=0;
       StackRecipeDefault.InternalFreqOffset = 0;
       StackRecipeDefault.EndofWeldStore = 0;
        break ;
    case STACK_FREQ_40KHZ:
       StackRecipeDefault.DigitalTune = DEFAULT_STACK_FREQ_40KHZ;
        StackRecipeDefault.InternalOffsetFlag=0;
       StackRecipeDefault.InternalFreqOffset = 0;
       StackRecipeDefault.EndofWeldStore = 0;
        break ;
    default :
       StackRecipeDefault.DigitalTune = STACK_FREQ_20KHZ;
        StackRecipeDefault.InternalOffsetFlag=0;
       StackRecipeDefault.InternalFreqOffset = 0;
       StackRecipeDefault.EndofWeldStore = 0;
        break ;
    }
}

/**
 * @brief StackRecipeData::~StackRecipeData
 */
StackRecipeData :: ~StackRecipeData()
{
   StackRecipeParameter* tmpRecipe;

    int ind = 0 ;
    for(ind = 0 ; ind < m_StackRecipeParamList.length() ; ind++)
    {
        tmpRecipe = dynamic_cast<StackRecipeParameter*>(m_StackRecipeParamList.at(ind));
        delete tmpRecipe ;
    }

    m_StackRecipeParamList.clear();
    m_StackRecipeLastSavedList.clear();
}
