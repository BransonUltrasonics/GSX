#include "Header/setupparameter.h"

/**
 * @brief SetupParameter::SetupParameter : Initialization of class members and memory for object
 * @param a_paramName
 * @param a_paramValue
 * @param a_paramUnit
 * @param parent
 */
SetupParameter::SetupParameter(QString a_paramName, QString a_paramValue, QString a_paramUnit, QObject *parent)
    :QObject(parent){
    m_paramName=a_paramName ;
    m_paramValue=a_paramValue ;
    m_paramUnit=a_paramUnit ;
}

/**
 * @brief SetupParameter::SetupParameter
 * @param parent
 */
SetupParameter::SetupParameter(QObject *parent)
    : QObject(parent)
{

}

/**
 * @brief SetupParameter::getParamName : Return stack name
 * @return
 */
QString SetupParameter::getParamName()
{
    return m_paramName;
}

/**
 * @brief SetupParameter::getParamValue : Return stack value
 * @return
 */
QString SetupParameter::getParamValue()
{
    return m_paramValue;
}

/**
 * @brief SetupParameter::getParamUnit : Return stack Unit
 * @return
 */
QString SetupParameter::getParamUnit()
{
    return m_paramUnit;
}

/**
 * @brief SetupParameter::setParamName : Set name and emit the signal
 * @param a_ParamName
 */
void SetupParameter::setParamName(QString a_ParamName)
{
    if(m_paramName!=a_ParamName){

        m_paramName=a_ParamName ;
        emit paramNameChanged();
    }

}

/**
 * @brief SetupParameter::setParamValue : set value and emit signal
 * @param a_ParamValue
 */
void SetupParameter::setParamValue(QString a_ParamValue)
{
    if(m_paramValue!=a_ParamValue){

        m_paramValue=a_ParamValue ;
        emit paramValueChanged();
    }
}

/**
 * @brief SetupParameter::setParamUnit : set unit and emit signal
 * @param a_ParamUnit
 */
void SetupParameter::setParamUnit(QString a_ParamUnit)
{
    if(m_paramUnit!=a_ParamUnit){

        m_paramUnit=a_ParamUnit ;
        emit paramUnitChanged();
    }
}
