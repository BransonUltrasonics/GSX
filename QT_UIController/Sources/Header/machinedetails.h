/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Machine Details

***************************************************************************/
#ifndef MACHINEDETAILS_H
#define MACHINEDETAILS_H

#include <QObject>
#include <QList>
#include <QQmlContext>
#include "Header/systeminfoparameters.h"

#define SYSINFO_SIZE 16
#define MACHINE_DETAILS_WELD                QObject::tr("Life Time Welds")
#define MACHINE_DETAILS_OVERLOADS           QObject::tr("Overloads")
#define MACHINE_DETAILS_PS_TYPE             QObject::tr("PS Type")
#define MACHINE_DETAILS_PS_FREQ             QObject::tr("PS Frequency")
#define MACHINE_DETAILS_PS_WATT             QObject::tr("PS Watt")
#define MACHINE_DETAILS_LIFE                QObject::tr("Life time cycles")
#define MACHINE_DETAILS_CALIBRATE_STATUS    QObject::tr("Calibration Status")
#define MACHINE_DETAILS_TYPE                QObject::tr("Type")
#define MACHINE_DETAILS_CALIBRATE_DATE      QObject::tr("Calibration Date")
#define MACHINE_DETAILS_STROKE              QObject::tr("Stroke Length")
#define MACHINE_DETAILS_PS_ASSEMBLY         QObject::tr("P/S Assembly")
#define MACHINE_DETAILS_STACK_ASSEMBLY      QObject::tr("Stack Assembly")
#define MACHINE_DETAILS_ACTUATOR_ASSEMBLY   QObject::tr("Actuator Assembly")
#define MACHINE_DETAILS_UIC_VER             QObject::tr("0.12.0")
#define MACHINE_DETAILS_UI_CONT             QObject::tr("UI Controller")
#define MACHINE_DETAILS_SC                  QObject::tr("Supervisory Controller")
#define MACHINE_DETAILS_AC                  QObject::tr("Actuator Controller")
#define MACHINE_DETAILS_PC                  QObject::tr("Power Controller")
#define MACHINE_DETAILS_FPGA                QObject::tr("FPGA")
#define MACHINE_DETAILS_MODEL               QObject::tr("Model")
#define MACHINE_DETAILS_GEN_ALRM            QObject::tr("General Alarm")
#define MACHINE_DETAILS_MAC_ID              QObject::tr("MAC ID")

//UIC_VERSION_NUM to be shown on HMI under System Information
#define UIC_VERSION_NUM_MAJOR 2
#define UIC_VERSION_NUM_MINOR 5
#define UIC_VERSION_NUM_BUILD 0
#define UIC_VERSION_NUM_AUTOS 26

struct SystemInformation
{
    qint32        psLifeCounter;
    qint32        actuatorlifecounter;
    qint32        generalAlarmCounter;
    qint32        overloadAlarmCounter;
    qint32        actuatorOverloads;
    qint32        actuatorStrokeLength;
    quint16       psFrequency;
    qint32        psWatt;
    quint8        calibrationStatus;
    quint8        psType;
    quint8        actuatorType;    
    char          modelName[SYSINFO_SIZE];
    char          version_SC[SYSINFO_SIZE];
    char          version_PC[SYSINFO_SIZE];
    char          version_AC[SYSINFO_SIZE];
    char          pc_FPGA_Version[SYSINFO_SIZE];
    char          psAssemblyNumber[SYSINFO_SIZE];
    char          actuatorAssembly[SYSINFO_SIZE];
    char          stackAssembly[SYSINFO_SIZE];
    char          psMacID[SYSINFO_SIZE+2];
    char          psIP[SYSINFO_SIZE];
    char          dateTime[2*SYSINFO_SIZE];
    quint16       crc_SC;
    quint16       crc_PC;
    quint16       crc_AC;
};
enum
{
    VERSION_SC = 0,
    VERSION_UIC,
    VERSION_PC,
    VERSION_AC,
    VERSION_FPGA,

    VERSION_MAX
};
enum
{
    PS_LIFE_COUNTER = 0,
    OVERLOAD_ALARM_COUNTER,
    PS_TYPE,
    PS_FREQUENCY,
    PS_WATT
};
enum
{
    PS_ASSEMBLY_NUMBER = 0,
    STACK_ASSEMBLY ,
    ACTUATOR_ASSEMBLY

};
enum
{
    ACTUATOR_LIFE_COUNTER = 0,
    ACTUATOR_TYPE,
    DATE_AND_TIME,
    ACTUATOR_OVERLOADS,
    STROKE_LENGTH

};
enum
{
    MODEL_NAME = 0,
    GENERAL_ALARM_COUNTER
};
enum
{
    PS_MAC_ID = 0,
    PS_IP
};

class MachineDetails : public QObject
{
    Q_OBJECT
public:
    QList<QObject*> m_versionlist;
    QList<QObject*> m_powersupplylist;
    QList<QObject*> m_assemblislist;
    QList<QObject*> m_actuatorlist;
    QList<QObject*> m_connectivitylist;
    QList<QObject*> m_modellist;
    QList<QObject*> m_modellistdummy;

    MachineDetails();
    ~MachineDetails();

    QStringList getVersionlistName();
    QString getPowerSupplylistName();
    QString getAssemblieslistName();
    QString getActuatorlistName();
    void createVersionlist();
    void createPowerSupplylist();
    void createAssemblieslist();
    void createActuatorlist();
    void createConnectivityList();
    void createmodelList();
    QString getVersionStructureValue(int);
    void sendVersionStructureValue();
    QString getPowerSupplyStructureValue(int);
    QString getAssembliesStructureValue(int);
    QString getActuatorStructureValue(int);
    QString getConnectivityStructureValue(int);
    QString getmodelStructureValue(int);
    Q_INVOKABLE void getmachineDetails();

private:
    SystemInformation machinedetailsdata;
    SystemInformation machinedetailsdatadummy;
};

#endif // MACHINEDETAILS_H
