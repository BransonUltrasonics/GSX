/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Horn Down

***************************************************************************/

#ifndef HORNDOWN_H
#define HORNDOWN_H

#include<QObject>
#include<QList>
#include <QMap>
#include <QQmlContext>

typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;

struct ActuatorData
{
    UINT8   ResponseType;
    UINT8   ResponseStatus;
    UINT32  AbsolutePosition;
    UINT16  PrePartContactVelocity;
    UINT32  PrePartContact;
    UINT32  ExcpetedPartContact;
    UINT16  ActualForce;
    UINT32  ActualDistance;
};

struct ActuatorDataUI
{
    double  ResponseType;
    double  ResponseStatus;
    double  AbsolutePosition;
    double  PrePartContactVelocity;
    double  PrePartContact;
    double  ExcpetedPartContact;
    double  ActualForce;
    double  ActualDistance;
};

enum SUB_ID
{
    PART_CONTACT=3,
    JOG_UP,
    JOG_DOWN,
    JOG_READY_POSITION,
    HORN_DOWN_CLAMP_ON,
    HORN_DOWN_CLAMP_OFF,
    HORN_RETRACT
};
enum STARTSTOP_WELD
{
    NO = 0,
    WELD
};

class Scope
{
public:
    QString m_Min;
    QString m_Max;
    QString m_Default;
    Scope()
    {
        m_Min="";
        m_Max="";
        m_Default="";
    }

    Scope(QString a_Min,QString a_Max,QString a_Default)
    {
        m_Min=a_Min;
        m_Max=a_Max;
        m_Default=a_Default;
    }
};

class HornDown:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString readyposition READ getreadyposition WRITE setreadyposition NOTIFY readypositionChanged)
    Q_PROPERTY(QString ppcPosition READ getppcPosition WRITE setppcPosition NOTIFY ppcPositionChanged)
    Q_PROPERTY(QString epcPosition READ getepcPosition WRITE setepcPosition NOTIFY epcPositionChanged)
    Q_PROPERTY(QString mpPosition READ getmpPosition WRITE setmpPosition NOTIFY mpPositionChanged)
    Q_PROPERTY(QString wpPosition READ getwpPosition WRITE setwpPosition NOTIFY wpPositionChanged)
    Q_PROPERTY(QString hornPointer READ gethornPointer WRITE sethornPointer NOTIFY hornPointerChanged)
    Q_PROPERTY(QString strokeLengthInPixel READ getstrokeLengthInPixel WRITE setstrokeLengthInPixel NOTIFY strokeLengthInPixelChanged)
    Q_PROPERTY(QString absolutePosition READ getabsolutePosition WRITE setabsolutePosition NOTIFY absolutePositionChanged)
    Q_PROPERTY(QString partContactVelocity READ getpartContactVelocity WRITE setpartContactVelocity NOTIFY partContactVelocityChanged)
    Q_PROPERTY(QString actualForce READ getactualForce WRITE setactualForce NOTIFY actualForceChanged)
    Q_PROPERTY(QString actualPPC READ getactualPPC WRITE setactualPPC NOTIFY actualPPCChanged)

public:
    explicit HornDown(QObject *parent = nullptr);

    ActuatorData m_StActuatordata;
    QStringList m_actuatorRecipeBuffer;
    QString m_ReadyPosition;
    QString m_PpcPosition;
    QString m_EpcPosition;
    QString m_MpPosition;
    QString m_WpPosition;
    QString m_Hornpointer;
    QString m_StrokeLength;
    QString m_StrokeLengthInPixel;
    QString m_AbsolutePosition;
    QString m_PartContactVelocity;
    QString m_ActualForce;
    QString m_PrePartContact;
    ActuatorDataUI m_ActuatorData;

    Q_INVOKABLE void RefreshAllData();
    Q_INVOKABLE void resetValues();
    Q_INVOKABLE void messageStatusOfRequest(int dwType);
    Q_INVOKABLE void successMessageStatusOfRequest(int dwType);    
    Q_INVOKABLE void UpdateStructureFromBL(QString);
    Q_INVOKABLE void updateEPCDataFromBL();
    Q_INVOKABLE void findPartContactRequestToBL();
    Q_INVOKABLE void jogUpRequestToBL();
    Q_INVOKABLE void jogDownRequestToBL();
    Q_INVOKABLE void setMyReadyPositionRequestToBL(QString);
    Q_INVOKABLE void horndownRequestToBL(bool isHornClamp);
    Q_INVOKABLE void retractRequestToBL();
    Q_INVOKABLE void startOrStopWeld(int subID);
    Q_INVOKABLE QString getHornDownRememberModes();
    Q_INVOKABLE void setHornDownRememberModes(bool bRFPC, bool bRJOG, bool bRHORNDOWN, bool bFPCStatus, bool bJOGStatus, bool bHORNDOWNStatus, bool bUpFlag, bool bDownFlag);
    Q_INVOKABLE void updateActuatorList(int);
    Q_INVOKABLE void updateHornPositionValues();
    Q_INVOKABLE void fillActiveRecipeForActuator(int dwFlag);
    Q_INVOKABLE QString getRecipeNumberForActuator();
    Q_INVOKABLE QString getRecipeNameForActuator();    
    Q_INVOKABLE QString toPixel(QString a_valueInMetric);
    Q_INVOKABLE QString splitdata(QString,int);
    Q_INVOKABLE QString getStrokeLengthFormConfig();
    Q_INVOKABLE int getUnitsType();
    QString getActiveRecipeForActuator();
	Q_INVOKABLE int getResponseType();
    QString getreadyposition() const;
    QString getppcPosition() const;    
    QString getepcPosition() const;    
    QString getmpPosition() const;    
    QString getwpPosition() const;    
    QString gethornPointer() const;    
    QString getstrokeLengthInPixel() const;    
    QString getabsolutePosition() const;    
    QString getpartContactVelocity() const;    
    QString getactualForce() const;    
    QString getactualPPC() const;
    void setreadyposition(const QString name);
    void setppcPosition(const QString name);
    void setepcPosition(const QString name);
    void setmpPosition(const QString name);
    void setwpPosition(const QString name);
    void sethornPointer(const QString name);
    void setstrokeLengthInPixel(const QString name);
    void setabsolutePosition(const QString name);
    void setpartContactVelocity(const QString name);
    void setactualForce(const QString name);
    void setactualPPC(const QString name);

signals:
    void readypositionChanged();
    void ppcPositionChanged();
    void epcPositionChanged();
    void mpPositionChanged();
    void wpPositionChanged();
    void hornPointerChanged();
    void strokeLengthInPixelChanged();
    void absolutePositionChanged();
    void partContactVelocityChanged();
    void actualForceChanged();
    void actualPPCChanged();
    Q_INVOKABLE void updateHornDownStatusText(QString strHornDownStatusText);
    void updateHornPositions();
};

#endif // HORNDOWN_H
