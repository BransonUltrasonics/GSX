/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Machine Details

***************************************************************************/
#include "Header/communicationinterface.h"
#include "Header/configuration.h"
#include "Header/machinedetails.h"

#include "Header/sysconfigdata.h"
extern QQmlContext* context;
extern CommunicationInterface CIObj;
extern int dwCurrentUnit;
char version_UI[10];
/**
 * @brief MachineDetails::MachineDetails
 */
MachineDetails::MachineDetails()
{
	QObject::connect(&CIObj , &CommunicationInterface::webserviceVersionRequest , this , &MachineDetails::sendVersionStructureValue);
}

/**
 * @brief MachineDetails::~MachineDetails
 */
MachineDetails::~MachineDetails()
{
}

/**
 * @brief MachineDetails::getVersionlistName : Return parameter values
 * @return
 */
QStringList MachineDetails::getVersionlistName()
{
    QStringList parameternames;
    parameternames.append(MACHINE_DETAILS_SC);
    parameternames.append(MACHINE_DETAILS_UI_CONT);
    parameternames.append(MACHINE_DETAILS_PC);
    parameternames.append(MACHINE_DETAILS_AC);

    if(SysConfigData::getInstance()->getBransonKeyGlobalFlag())
    {
        parameternames.append(MACHINE_DETAILS_FPGA);
    }
    return parameternames;
}

/**
 * @brief MachineDetails::getVersionStructureValue : Return machine version details
 * @param index
 * @return
 */
QString MachineDetails::getVersionStructureValue(int index)
{
    QString retValue = " ";
    switch (index)
    {
    case VERSION_SC:
        retValue.clear();
        retValue.append(machinedetailsdata.version_SC);
        if(machinedetailsdata.crc_SC != 0)
        {
            retValue.append(" (");
            retValue.append(QString::number(machinedetailsdata.crc_SC));
            retValue.append(")");
        }
        break;
    case VERSION_UIC:
    	sprintf(version_UI,"%d.%d.%d.%d",UIC_VERSION_NUM_MAJOR, UIC_VERSION_NUM_MINOR, UIC_VERSION_NUM_BUILD, UIC_VERSION_NUM_AUTOS);
    	retValue = QString::fromUtf8(version_UI);
        break;
    case VERSION_PC:
        retValue.clear();
        retValue.append(machinedetailsdata.version_PC);
        if(machinedetailsdata.crc_PC != 0)
        {
            retValue.append(" (");
            retValue.append(QString::number(machinedetailsdata.crc_PC));
            retValue.append(")");
        }
        break;
    case VERSION_AC:
        retValue.clear();
        retValue.append(machinedetailsdata.version_AC);
        if(machinedetailsdata.crc_AC != 0)
        {
            retValue.append(" (");
            retValue.append(QString::number(machinedetailsdata.crc_AC));
            retValue.append(")");
        }
        break;
    case VERSION_FPGA:
        if(SysConfigData::getInstance()->getBransonKeyGlobalFlag())
        {
            retValue.clear();
            retValue.append(machinedetailsdata.pc_FPGA_Version);
        }
        break;
    default:
        break;
    }
    return retValue;
}

/**
 * @brief MachineDetails::createVersionlist : Create version list
 */

void MachineDetails::createVersionlist()
{
    context->setContextProperty("modelversionlist", QVariant::fromValue(m_modellistdummy));
    m_versionlist.clear();    
    QStringList versonList = getVersionlistName();
    for(int dwIndex1=0;dwIndex1<versonList.size();dwIndex1++)
    {
        SystemInfoParameter* infoParameter = new SystemInfoParameter;
        infoParameter->m_parameterName = versonList[dwIndex1];
        infoParameter->m_parameterContent = getVersionStructureValue(dwIndex1);
        m_versionlist.append(infoParameter);
    }

    context->setContextProperty("modelversionlist", QVariant::fromValue(m_versionlist));
}

/**
 * @brief MachineDetails::getPowerSupplylistName : Return power supply list name
 * @return
 */
QString MachineDetails::getPowerSupplylistName()
{
    QString parameternames;
    parameternames = MACHINE_DETAILS_WELD+","+MACHINE_DETAILS_OVERLOADS+","+MACHINE_DETAILS_PS_TYPE+","+MACHINE_DETAILS_PS_FREQ+","+MACHINE_DETAILS_PS_WATT;
    return parameternames;
}

/**
 * @brief MachineDetails::getPowerSupplyStructureValue : Return power supply structure values
 * @param index
 * @return
 */
QString MachineDetails::getPowerSupplyStructureValue(int index)
{
    QString qstrRetValue="";
    int data;
    switch (index) {
    case PS_LIFE_COUNTER:
        qstrRetValue = QString::number(machinedetailsdata.psLifeCounter);
        break;
    case OVERLOAD_ALARM_COUNTER:
        qstrRetValue = QString::number(machinedetailsdata.overloadAlarmCounter);
        break;
    case PS_TYPE:
        data = machinedetailsdata.psType;
        if(data == 0)
        {
            qstrRetValue = "STANDARD";
        }        
        else {
            qstrRetValue = " ";
        }
        break;
    case PS_FREQUENCY:
        data = machinedetailsdata.psFrequency;
        if(data == 0)
        {
            qstrRetValue = "20 KHz";
        }
        else if(data == 1)
        {
            qstrRetValue = "30 KHz";
        }
        else if(data == 2)
        {
            qstrRetValue = "40 KHz";
        }
        else
        {
            qstrRetValue = " ";
        }        break;
    case PS_WATT:
        qstrRetValue =  QString::number(machinedetailsdata.psWatt);
        qstrRetValue.append(" W");
        break;
    default:
        break;
    }
    return qstrRetValue;
}

/**
 * @brief MachineDetails::createPowerSupplylist : Create powersupply list and update model
 */
void MachineDetails::createPowerSupplylist()
{
    context->setContextProperty("modelpowersupplylist", QVariant::fromValue(m_modellistdummy));
    m_powersupplylist.clear();
    QString psName = getPowerSupplylistName();
    QStringList powerSupplyList = psName.split(",");

    for(int dwIndex1=0;dwIndex1<powerSupplyList.size();dwIndex1++)
    {
        SystemInfoParameter* infoParameter = new SystemInfoParameter;
        infoParameter->m_parameterName = powerSupplyList[dwIndex1];
        infoParameter->m_parameterContent = getPowerSupplyStructureValue(dwIndex1);
        m_powersupplylist.append(infoParameter);
    }
    context->setContextProperty("modelpowersupplylist", QVariant::fromValue(m_powersupplylist));
}

/**
 * @brief MachineDetails::getAssemblieslistName : Return assembly list name
 * @return
 */
QString MachineDetails::getAssemblieslistName()
{
    QString parameternames;
    parameternames = MACHINE_DETAILS_PS_ASSEMBLY+","+MACHINE_DETAILS_STACK_ASSEMBLY+","+MACHINE_DETAILS_ACTUATOR_ASSEMBLY;
    return parameternames;
}

/**
 * @brief MachineDetails::getAssembliesStructureValue : Return assembly structure value
 * @param index
 * @return
 */
QString MachineDetails::getAssembliesStructureValue(int index)
{
    QString qstrRetValue="";
    switch (index)
    {
    case PS_ASSEMBLY_NUMBER:
        qstrRetValue = machinedetailsdata.psAssemblyNumber;
        break;
    case STACK_ASSEMBLY:
        qstrRetValue = machinedetailsdata.stackAssembly;
        break;
    case ACTUATOR_ASSEMBLY:
        qstrRetValue = machinedetailsdata.actuatorAssembly;
        break;
    default:
        break;
    }
    return qstrRetValue;
}

/**
 * @brief MachineDetails::createAssemblieslist : Create assembly list and update model
 */
void MachineDetails::createAssemblieslist()
{
    context->setContextProperty("modelassemblylist", QVariant::fromValue(m_modellistdummy));
    m_assemblislist.clear();
    QString assemblyName = getAssemblieslistName();
    QStringList assemblyList = assemblyName.split(",");

    for(int dwIndex1=0;dwIndex1<assemblyList.size();dwIndex1++)
    {
        SystemInfoParameter* infoParameter = new SystemInfoParameter;
        infoParameter->m_parameterName = assemblyList[dwIndex1];
        infoParameter->m_parameterContent = getAssembliesStructureValue(dwIndex1);
        m_assemblislist.append(infoParameter);
    }

    context->setContextProperty("modelassemblylist", QVariant::fromValue(m_assemblislist));
}

/**
 * @brief MachineDetails::getActuatorlistName : Return actuator list name
 * @return
 */
QString MachineDetails::getActuatorlistName()
{
    QString parameternames;
    parameternames = MACHINE_DETAILS_LIFE+","+MACHINE_DETAILS_TYPE+","+MACHINE_DETAILS_CALIBRATE_DATE+","+MACHINE_DETAILS_OVERLOADS+","+MACHINE_DETAILS_STROKE;
    return parameternames;
}

/**
 * @brief MachineDetails::getActuatorStructureValue : Return actuator structure value
 * @param index
 * @return
 */
QString MachineDetails::getActuatorStructureValue(int index)
{
    QString qstrRetValue="";
    int data;
    double dwValue;
    switch (index)
    {
    case ACTUATOR_LIFE_COUNTER:
        qstrRetValue = QString::number(machinedetailsdata.actuatorlifecounter);
        break;
    case ACTUATOR_TYPE:
        data = machinedetailsdata.actuatorType;
        if(data == 0)
        {
            qstrRetValue = "SERVO";
        }
        else
        {
            qstrRetValue = " ";
        }
        break;
    case DATE_AND_TIME:
        qstrRetValue = machinedetailsdata.dateTime;
        break;
    case ACTUATOR_OVERLOADS:
        qstrRetValue = QString::number(machinedetailsdata.actuatorOverloads);
        break;
    case STROKE_LENGTH:        
        dwValue = machinedetailsdata.actuatorStrokeLength;
        if(dwCurrentUnit)
        {
            dwValue = dwValue/(double)25.4;
            qstrRetValue.setNum(dwValue,'f',4);
            qstrRetValue.append(" in");
        }
        else
        {
            qstrRetValue = dwValue;
            qstrRetValue.setNum(dwValue,'f',3);
            qstrRetValue.append(" mm");
        }
        break;
    default:
        break;
    }
    return qstrRetValue;
}

/**
 * @brief MachineDetails::createActuatorlist : Create actuator list and update model
 */
void MachineDetails::createActuatorlist()
{
    context->setContextProperty("modelactuatorlist", QVariant::fromValue(m_modellistdummy));
    m_actuatorlist.clear();
    QString actuatorName = getActuatorlistName();
    QStringList actuatorList = actuatorName.split(",");

    for(int dwIndex1=0;dwIndex1<actuatorList.size();dwIndex1++)
    {
        SystemInfoParameter* infoParameter = new SystemInfoParameter;
        infoParameter->m_parameterName = actuatorList[dwIndex1];
        infoParameter->m_parameterContent = getActuatorStructureValue(dwIndex1);
        m_actuatorlist.append(infoParameter);
    }

    context->setContextProperty("modelactuatorlist", QVariant::fromValue(m_actuatorlist));
}

/**
 * @brief MachineDetails::getConnectivityStructureValue : Return connectivity structure value
 * @param index
 * @return
 */
QString MachineDetails::getConnectivityStructureValue(int index)
{
    QString retValue="";
    switch (index)
    {
    case PS_MAC_ID:
        retValue = machinedetailsdata.psMacID;
        break;
    case PS_IP:
        retValue = machinedetailsdata.psIP;
        break;
    default:
        break;
    }
    return retValue;
}

/**
 * @brief MachineDetails::createConnectivityList : Create connectivity list and update model
 */
void MachineDetails::createConnectivityList()
{
    context->setContextProperty("modelconnectivitylist", QVariant::fromValue(m_modellistdummy));
    m_connectivitylist.clear();
    QStringList connectivityList;
    connectivityList.append(MACHINE_DETAILS_MAC_ID);
    for(int dwIndex1=0;dwIndex1<connectivityList.size();dwIndex1++)
    {
        SystemInfoParameter* infoParameter = new SystemInfoParameter;
        infoParameter->m_parameterName = connectivityList[dwIndex1];
        infoParameter->m_parameterContent = getConnectivityStructureValue(dwIndex1);
        m_connectivitylist.append(infoParameter);
    }

    context->setContextProperty("modelconnectivitylist", QVariant::fromValue(m_connectivitylist));
}

/**
 * @brief MachineDetails::getmodelStructureValue : Return model and alarm structure value
 * @param index
 * @return
 */
QString MachineDetails::getmodelStructureValue(int index)
{
    QString retValue="";
    switch (index)
    {
    case MODEL_NAME:
        /*To update Model Name in System Information Machine Details screen*/
        if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            retValue = "ASX";
        }

        else
        {
            retValue = "GSX-E1";
        }
        break;
    case GENERAL_ALARM_COUNTER:
        retValue = QString::number(machinedetailsdata.generalAlarmCounter);
        break;
    default:
        break;
    }
    return retValue;
}

void MachineDetails::sendVersionStructureValue()
{
    sprintf(version_UI,"%d.%d.%d.%d",UIC_VERSION_NUM_MAJOR, UIC_VERSION_NUM_MINOR, UIC_VERSION_NUM_BUILD, UIC_VERSION_NUM_AUTOS);
	CIObj.sendMessage(SCBL_WEBSERVICE_UI_VERSION_RES,0,version_UI,sizeof(version_UI));
}
/**
 * @brief MachineDetails::createmodelList : Create model and general alarm list
 */
void MachineDetails::createmodelList()
{
    context->setContextProperty("modelmodellist", QVariant::fromValue(m_modellistdummy));
    m_modellist.clear();
    QStringList modelList;
    modelList.append(MACHINE_DETAILS_MODEL);
    modelList.append(MACHINE_DETAILS_GEN_ALRM);
    for(int dwIndex1=0;dwIndex1<modelList.size();dwIndex1++)
    {
        SystemInfoParameter* infoParameter = new SystemInfoParameter;
        infoParameter->m_parameterName = modelList[dwIndex1];
        infoParameter->m_parameterContent = getmodelStructureValue(dwIndex1);
        m_modellist.append(infoParameter);
    }

    context->setContextProperty("modelmodellist", QVariant::fromValue(m_modellist));
}

/**
 * @brief MachineDetails::getmachineDetails : Return machine version from BL
 */
void MachineDetails::getmachineDetails()
{
    char data[1] = {' '};
    CIObj.sendMessage(SCBL_FW_VERSION_REQ, UIC_FW_VERSION_RES, data, sizeof(data));
    char *m_data = reinterpret_cast<char*>(&machinedetailsdata);
    CIObj.recivedMessage(UIC_FW_VERSION_RES, m_data ,sizeof(machinedetailsdata));
    createmodelList();
    createVersionlist();
    createPowerSupplylist();
    createAssemblieslist();
    createActuatorlist();
    createConnectivityList();
}
