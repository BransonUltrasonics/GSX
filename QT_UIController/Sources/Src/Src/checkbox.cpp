/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Sets and gets graph right parameter values

***************************************************************************/
#include <QQmlContext>
#include <QSettings>
#include "Header/checkbox.h"
#include "Header/common.h"
#include "Header/configuration.h"

#define CHECK_BOX_NAME_AMPL            QObject::tr("Amplitude")
#define CHECK_BOX_NAME_POWER           QObject::tr("Power")
#define CHECK_BOX_NAME_FREQ            QObject::tr("Frequency")
#define CHECK_BOX_NAME_FORCE           QObject::tr("Force")
#define CHECK_BOX_NAME_COLLAPSE_DIST   QObject::tr("Collapse Distance")
#define CHECK_BOX_NAME_ABSOLUTE_DIST   QObject::tr("Absolute Distance")
#define CHECK_BOX_NAME_VELOCITY        QObject::tr("Velocity")
#define CHECK_BOX_NAME_CURRENT         QObject::tr("Rated Current")
#define CHECK_BOX_NAME_PHASE           QObject::tr("Phase")
#define CHECK_BOX_NAME_ENERGY          QObject::tr("Energy")
#define CHECK_BOX_NAME_MEMORY          QObject::tr("Memory")

extern int dwCurrentUnit;
extern QQmlContext* context;

/**
 * @brief checkbox::checkbox : Initialization of class members and memory for object
 * @param color
 * @param name
 * @param unit
 * @param val
 * @param paramVal
 * @param state
 * @param axis
 */
checkbox::checkbox(QString color,QString name,QString unit,QString val,QString paramVal,bool state,int axis)
{
    m_color = color;
    m_name = name;
    m_unit = unit;
    m_value = val;
    m_parameterVal = paramVal;
    m_isCheckBoxSelected = state;
    m_isLeftAxis = axis;
}

/**
 * @brief checkbox::checkbox : Constructor
 * @param parent
 */
checkbox::checkbox(QObject *parent) : QObject(parent)
{
}

/**
 * @brief checkbox::GetcheckBoxcolor : Returns check box color
 * @return
 */
QString checkbox::getcheckBoxcolor()
{
    return m_color;
}

/**
 * @brief checkbox::GetcheckBoxname : Returns check box name
 * @return
 */
QString checkbox::getcheckBoxname()
{
    return m_name;
}

/**
 * @brief checkbox::getcheckBoxunit : Returns check box unit
 * @return
 */
QString checkbox::getcheckBoxunit()
{
    return m_unit;
}

/**
 * @brief checkbox::getcheckBoxValue : Returns check box value
 * @return
 */
QString checkbox::getcheckBoxValue()
{
    return m_value;
}

/**
 * @brief checkbox::getparameterval : Returns check box parameter value
 * @return
 */
QString checkbox::getparameterval()
{
    return m_parameterVal;
}

/**
 * @brief checkbox::getisCheckBoxSelected : Returns whether check box is selected or not
 * @return
 */
bool checkbox::getisCheckBoxSelected()
{
    return m_isCheckBoxSelected;
}

/**
 * @brief checkbox::setParameterVal : Sets checkbox parameter value
 * @param a_parameterVal
 */
void checkbox :: setParameterVal(QString a_parameterVal)
{
    m_parameterVal = a_parameterVal;
    parametervalChanged();
}

/**
 * @brief checkbox::setParameterVal : Sets checkbox parameter value of int type
 * @param a_parameterVal
 */
void checkbox :: setParameterVal(unsigned int a_parameterVal)
{
    m_parameterVal = QString::number(a_parameterVal);
    parametervalChanged();
}

/**
 * @brief checkbox::setParameterVal : Sets checkbox parameter value of float type
 * @param a_parameterVal
 */
void checkbox :: setParameterVal(float a_parameterVal)
{
    m_parameterVal = QString::number( a_parameterVal);
    parametervalChanged();
}

/**
 * @brief checkbox::GetisLeftAxisSelected : Returns whether left axis selecetd or not
 * @return
 */
int checkbox::getisLeftAxisSelected()
{
    return m_isLeftAxis;
}

/**
 * @brief checkbox::setIsLeftAxisSelected : Sets left axis status
 * @param a_val
 */
void checkbox::setIsLeftAxisSelected(int a_val)
{
    if(a_val!=m_isLeftAxis)
    {
        m_isLeftAxis = a_val ;
        checkisLeftAxisSelected();
    }
}

/**
 * @brief checkbox::setIsCheckBoxSelected : Sets checkbox selected status
 * @param a_checkBoxSelected
 */
void checkbox::setIsCheckBoxSelected(bool a_checkBoxSelected)
{
    m_isCheckBoxSelected = a_checkBoxSelected ;
}

/**
 * @brief checkbox::addcheckboxValue : Adds checkbox properties to a model and updates in UI
 */
void checkbox::addcheckboxValue()
{
    checkboxlist.clear();
    context->setContextProperty("checkboxlistModel", QVariant::fromValue(dummycheckboxlist));
    int dwUnit =  dwCurrentUnit;

    checkboxlist.append(new checkbox(CHECK_BOX_COLOR1,CHECK_BOX_NAME_AMPL,PERCENTAGE,"true","0",true,0));
    checkboxlist.append(new checkbox(CHECK_BOX_COLOR2,CHECK_BOX_NAME_POWER,WATT,"true","0",true,0));
    checkboxlist.append(new checkbox(CHECK_BOX_COLOR3,CHECK_BOX_NAME_FREQ,HERTZ,"true","0",true,0));

    if(dwUnit)
    {
        checkboxlist.append(new checkbox(CHECK_BOX_COLOR4,CHECK_BOX_NAME_FORCE,LECIBALS,"true","0",true,0));
        checkboxlist.append(new checkbox(CHECK_BOX_COLOR5,CHECK_BOX_NAME_COLLAPSE_DIST,INCHES,"true","0",true,0));
        checkboxlist.append(new checkbox(CHECK_BOX_COLOR6,CHECK_BOX_NAME_ABSOLUTE_DIST,INCHES,"true","0",true,1));
        checkboxlist.append(new checkbox(CHECK_BOX_COLOR7,CHECK_BOX_NAME_VELOCITY,INCHPERSECOND,"true","0",true,1));
    }
    else
    {
        checkboxlist.append(new checkbox(CHECK_BOX_COLOR4,CHECK_BOX_NAME_FORCE,NEWTONS,"true","0",true,0));
        checkboxlist.append(new checkbox(CHECK_BOX_COLOR5,CHECK_BOX_NAME_COLLAPSE_DIST,MICROMETRE,"true","0",true,0));
        checkboxlist.append(new checkbox(CHECK_BOX_COLOR6,CHECK_BOX_NAME_ABSOLUTE_DIST,MICROMETRE,"true","0",true,1));
        checkboxlist.append(new checkbox(CHECK_BOX_COLOR7,CHECK_BOX_NAME_VELOCITY,CUST_NAME_MMPERSECOND,"true","0",true,1));
    }

    checkboxlist.append(new checkbox(CHECK_BOX_COLOR8,CHECK_BOX_NAME_CURRENT,PERCENTAGE,"true","0",true,1));
    checkboxlist.append(new checkbox(CHECK_BOX_COLOR9,CHECK_BOX_NAME_PHASE,DEGREE,"true","0",true,1));
    checkboxlist.append(new checkbox(CHECK_BOX_COLOR10,CHECK_BOX_NAME_ENERGY,JOULE,"true","0",true,1));

    qstrlstParamValues.append("0");
    qstrlstParamValues.append("0");
    qstrlstParamValues.append("0");
    qstrlstParamValues.append("0");
    qstrlstParamValues.append("0");
    qstrlstParamValues.append("0");
    qstrlstParamValues.append("0");
    qstrlstParamValues.append("0");
    qstrlstParamValues.append("0");
    qstrlstParamValues.append("0");
    context->setContextProperty("checkboxlistModel", QVariant::fromValue(checkboxlist));
}

/**
 * @brief checkbox::updateParameterVal : Updates the parameter value and displays in UI
 * @param amp
 * @param curr
 * @param power
 * @param freq
 * @param energy
 * @param phase
 * @param force
 * @param velocity
 * @param absolutedis
 * @param collapsedis
 */
void checkbox::updateParameterVal(QString amp, QString curr, QString power, QString freq, QString energy, QString phase, QString force, QString velocity, QString absolutedis, QString collapsedis)
{
    QObject *ptr;
    checkbox *m_ptr;

    qstrlstParamValues.clear();
    amp.setNum(amp.toDouble(),'f',0);
    qstrlstParamValues.append(amp);

    power.setNum(power.toDouble(),'f',0);
    qstrlstParamValues.append(power);

    freq.setNum(freq.toDouble(),'f',0);
    qstrlstParamValues.append(freq);
    /*To display one decimal value for imperial Unit and round value for Metric Unit*/
    if(dwCurrentUnit)
    {
        force.setNum(force.toDouble(),'f',1);
    }
    else
    {
        force.setNum(force.toDouble(),'f',0);
    }
    qstrlstParamValues.append(force);

    if(dwCurrentUnit)
    {
        collapsedis.setNum(collapsedis.toDouble(),'f',4);
    }
    else
    {
        collapsedis.setNum(collapsedis.toDouble(),'f',3);
    }

    qstrlstParamValues.append(collapsedis);

    if(dwCurrentUnit)
    {
        absolutedis.setNum(absolutedis.toDouble(),'f',4);
    }
    else
    {
        absolutedis.setNum(absolutedis.toDouble(),'f',3);
    }
    qstrlstParamValues.append(absolutedis);

    if(dwCurrentUnit)
    {
        velocity.setNum(velocity.toDouble(),'f',4);
    }
    else
    {
        velocity.setNum(velocity.toDouble(),'f',2);
    }
    qstrlstParamValues.append(velocity);

    curr.setNum(curr.toDouble(),'f',0);
    qstrlstParamValues.append(curr);

    phase.setNum(phase.toDouble(),'f',0);
    qstrlstParamValues.append(phase);

    energy.setNum(energy.toDouble(),'f',1);
    qstrlstParamValues.append(energy);

    /* Refreshing UI checkbox list */
    context->setContextProperty("checkboxlistModel", QVariant::fromValue(dummycheckboxlist));
    for(int i=0;i<10;i++)
    {
        ptr = checkboxlist.at(i);
        m_ptr = reinterpret_cast<checkbox*>(ptr);
        m_ptr->m_parameterVal = qstrlstParamValues.at(i);
    }

    /* Refreshing UI checkbox list */
    context->setContextProperty("checkboxlistModel", QVariant::fromValue(checkboxlist));
}
/**
 * @brief checkbox::setCheckboxStatus : Sets check box state
 * @param dwIndex
 * @param isChecked
 */
void checkbox::setCheckboxStatus(int dwIndex, bool isChecked)
{
    QObject *ptr;
    checkbox *m_ptr;
    ptr = checkboxlist.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);
    m_ptr->m_isCheckBoxSelected = isChecked;
    if(isChecked)
        m_ptr->m_parameterVal = qstrlstParamValues.at(dwIndex);
    else
        m_ptr->m_parameterVal = "0";
}
/**
 * @brief checkbox::setAxisIndex : Sets axis index whether it is left or right
 * @param dwIndex
 * @param isLeftSelected
 */
void checkbox::setAxisIndex(int dwIndex, int isLeftSelected)
{
    QObject *ptr;
    checkbox *m_ptr;
    if(dwIndex < 0)
        dwIndex=0;
    ptr = checkboxlist.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);

    if(isLeftSelected == 0)
        m_ptr->m_isLeftAxis = 0;
    else
        m_ptr->m_isLeftAxis = 1;
}
/**
 * @brief checkbox::~checkbox : Destructor
 */
checkbox :: ~checkbox()
{
    checkbox* p_checkBox ;

    unsigned int ind = 0 ;
    unsigned int length = checkboxlist.count() ;

    for(ind = 0 ; ind < length ; ind++)
    {
        p_checkBox = reinterpret_cast<checkbox*>(checkboxlist.at(ind));

        delete p_checkBox ;
        p_checkBox = NULL;
    }

    checkboxlist.clear();
    qstrlstParamValues.clear();
}
/**
 * @brief checkbox::GetComboIndex : Returns combobox index to tell whether left axis is selected or right axis
 * @param dwIndex
 * @return
 */
bool checkbox::getComboIndex(int dwIndex)
{
    QObject *ptr;
    checkbox *m_ptr;
    if(dwIndex < 0)
        dwIndex = 0;
    ptr = checkboxlist.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);
    if(m_ptr->m_isLeftAxis)
        return true;
    return false;
}

/**
 * @brief checkbox::GetCheckBoxState : Returns checkbox state
 * @param dwIndex
 * @return
 */
bool checkbox::getCheckBoxState(int dwIndex)
{
    QObject *ptr;
    checkbox *m_ptr;
    if(dwIndex < 0)
        dwIndex = 0;
    ptr = checkboxlist.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);
    if(m_ptr->m_isCheckBoxSelected)
        return true;
    return false;
}

/**
 * @brief checkbox::setDefaultValues : Sets default status of right graph parameters and reads from config.ini
 */
void checkbox::setDefaultValues()
{
    context->setContextProperty("checkboxlistModel", QVariant::fromValue(dummycheckboxlist));
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("WeldGraphCheckBoxStatus");
    QString checkValue = settings.value("WeldCheckStatus").toString();
    settings.endGroup();
    settings.beginGroup("WeldGraphComboBoxStatus");
    QString comboValue = settings.value("WeldComboStatus").toString();
    settings.endGroup();
    if(checkValue.size()>1)
    {
        checkValue.remove(checkValue.length()-1,1);
        checkValue.remove(0,1);
    }
    else
    {
        checkValue="1,1,1,1,1,1,1,1,1,1";
    }
    if(comboValue.size()>1)
    {
        comboValue.remove(comboValue.length()-1,1);
        comboValue.remove(0,1);
    }
    else
    {
        comboValue="0,0,0,0,0,1,1,1,1,1";
    }

    QStringList temp1 = checkValue.split(",");
    QStringList temp2 = comboValue.split(",");
    QObject *ptr;
    checkbox *m_ptr;
    for(int i=0;i<checkboxlist.size();i++)
    {
        ptr = checkboxlist.at(i);
        m_ptr = reinterpret_cast<checkbox*>(ptr);
        m_ptr->m_isCheckBoxSelected = temp1[i].toInt();
        m_ptr->m_isLeftAxis = temp2[i].toInt();
    }
    context->setContextProperty("checkboxlistModel", QVariant::fromValue(checkboxlist));
}

/**
 * @brief checkbox::getDefaultValues : Returns default values and writes into config.ini
 */
void checkbox::getDefaultValues()
{
    QString qstrCheckOrder="",qstrComboOrder="";
    QObject *ptr;
    checkbox *m_ptr;
    for(int i=0;i<checkboxlist.size();i++)
    {
        ptr = checkboxlist.at(i);
        m_ptr = reinterpret_cast<checkbox*>(ptr);
        qstrComboOrder.append(QString::number(m_ptr->m_isLeftAxis));
        qstrComboOrder.append(",");
        qstrCheckOrder.append(QString::number(m_ptr->m_isCheckBoxSelected));
        qstrCheckOrder.append(",");
    }
    qstrComboOrder.remove(qstrComboOrder.length()-1,1);
    qstrComboOrder.insert(0,"\"");
    qstrComboOrder.insert(qstrComboOrder.length(),"\"");

    qstrCheckOrder.remove(qstrCheckOrder.length()-1,1);
    qstrCheckOrder.insert(0,"\"");
    qstrCheckOrder.insert(qstrCheckOrder.length(),"\"");

    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("WeldGraphCheckBoxStatus");
    settings.setValue("WeldCheckStatus",qstrCheckOrder);
    settings.endGroup();
    settings.beginGroup("WeldGraphComboBoxStatus");
    settings.setValue("WeldComboStatus",qstrComboOrder);
    settings.endGroup();
}
