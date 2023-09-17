/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Suspect Reject Limits

***************************************************************************/

#ifndef SUSPECTREJECTPARAMETER_H
#define SUSPECTREJECTPARAMETER_H

#include <QObject>

class SuspectRejectParameter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ParameterName READ getParameterName WRITE setParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(QString ParameterLocaleName READ getParameterLocaleName WRITE setParameterLocaleName NOTIFY parameterLocaleNameChanged)
    Q_PROPERTY(bool SuspectStatus READ getSuspectStatus WRITE setSuspectStatus NOTIFY SuspectStatusValueChanged)
    Q_PROPERTY(bool SuspectLowStatus READ getSuspectLowStatus WRITE setSuspectLowStatus NOTIFY SuspectLowStatusValueChanged)
    Q_PROPERTY(bool SuspectHighStatus READ getSuspectHighStatus WRITE setSuspectHighStatus NOTIFY SuspectHighStatusValueChanged)
    Q_PROPERTY(QString SuspectLow READ getSuspectLow WRITE setSuspectLow NOTIFY SuspectLowValueChanged)
    Q_PROPERTY(QString SuspectHigh READ getSuspectHigh WRITE setSuspectHigh NOTIFY SuspectHighValueChanged)
    Q_PROPERTY(bool RejectStatus READ getRejectStatus WRITE setRejectStatus NOTIFY RejectStatusValueChanged)
    Q_PROPERTY(bool RejectLowStatus READ getRejectLowStatus WRITE setRejectLowStatus NOTIFY RejectLowStatusValueChanged)
    Q_PROPERTY(bool RejectHighStatus READ getRejectHighStatus WRITE setRejectHighStatus NOTIFY RejectHighStatusValueChanged)
    Q_PROPERTY(QString RejectLow READ getRejectLow WRITE setRejectLow NOTIFY RejectLowValueChanged)
    Q_PROPERTY(QString RejectHigh READ getRejectHigh WRITE setRejectHigh NOTIFY RejectHighValueChanged)
    Q_PROPERTY(bool ParameterSwitch READ getParameterSwitch WRITE setParameterSwitch NOTIFY ParameterSwitchChanged)
    Q_PROPERTY(bool ParameterSwitchState READ getParameterSwitchState WRITE setParameterSwitchState NOTIFY ParameterSwitchStateChanged)
    Q_PROPERTY(QString ParameterUnit READ getParameterUnit WRITE setParameterUnit NOTIFY parameterUnitChanged)

public:
    SuspectRejectParameter(QObject *parent=0);
    SuspectRejectParameter(QString name,
                           QString localeName,
                           const QString unit,
                           bool suspectStatus,
                           bool suspectLowStatus,
                           bool suspectHighStatus,
                           QString suspectLow,
                           QString suspectHigh,
                           bool rejectStatus,
                           bool rejectLowStatus,
                           bool rejectHighStatus,
                           QString rejectLow,
                           QString rejectHigh,
                           bool isSwitch=false,
                           bool switchState=false,
                           QObject *parent=0);
    QString m_parameterName;
    QString m_parameterLocaleName;
    QString m_suspectLow;
    QString m_suspectHigh;
    QString m_rejectLow;
    QString m_rejectHigh;
    QString m_ParameterUnit;
    bool m_suspectStatus;
    bool m_suspectLowStatus;
    bool m_suspectHighStatus;
    bool m_rejectStatus;
    bool m_rejectLowStatus;
    bool m_rejectHighStatus;
    bool m_ParameterSwitch;
    bool m_ParameterSwitchState;

    QString getParameterUnit() const;
    void setParameterUnit(const QString unit);

    QString getParameterName() const;
    void setParameterName(const QString name);

    QString getParameterLocaleName() const;
    void setParameterLocaleName(const QString name);

    QString getSuspectLow() const;
    void setSuspectLow(const QString value);

    QString getSuspectHigh() const;
    void setSuspectHigh(const QString value);

    QString getRejectLow() const;
    void setRejectLow(const QString value);

    QString getRejectHigh() const;
    void setRejectHigh(const QString value);

    bool getRejectStatus() const;
    void setRejectStatus(const bool value);

    bool getRejectLowStatus() const;
    void setRejectLowStatus(const bool value);

    bool getRejectHighStatus() const;
    void setRejectHighStatus(const bool value);

    bool getSuspectStatus() const;
    void setSuspectStatus(const bool value);

    bool getSuspectLowStatus() const;
    void setSuspectLowStatus(const bool value);

    bool getSuspectHighStatus() const;
    void setSuspectHighStatus(const bool value);

    bool getParameterSwitch();
    void setParameterSwitch(bool isSwitch);

    bool getParameterSwitchState();
    void setParameterSwitchState(bool switchState);
signals:
    void parameterNameChanged();
    void parameterLocaleNameChanged();
    void SuspectLowValueChanged();
    void SuspectHighValueChanged();
    void RejectLowValueChanged();
    void RejectHighValueChanged();
    void RejectStatusValueChanged();
    void RejectLowStatusValueChanged();
    void RejectHighStatusValueChanged();
    void SuspectStatusValueChanged();
    void SuspectLowStatusValueChanged();
    void SuspectHighStatusValueChanged();
    void ParameterSwitchChanged();
    void ParameterSwitchStateChanged();
    void parameterUnitChanged();
};

#endif // SUSPECTREJECTPARAMETER_H

