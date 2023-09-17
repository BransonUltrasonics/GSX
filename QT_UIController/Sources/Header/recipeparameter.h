/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Recipe Parameter

***************************************************************************/

#ifndef RECIPEPARAMETER_H
#define RECIPEPARAMETER_H

#include <QObject>

class RecipeParameter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ParameterName READ getParameterName WRITE setParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(QString ParameterLocaleName READ getParameterLocaleName WRITE setParameterLocaleName NOTIFY parameterLocaleNameChanged)
    Q_PROPERTY(int ParameterIndex READ getParameterIndex WRITE setParameterIndex NOTIFY parameterIndexChanged)
    Q_PROPERTY(QString ParameterValue READ getParameterValue WRITE setParameterValue NOTIFY parameterValueChanged)
    Q_PROPERTY(QString ParameterUnit READ getParameterUnit WRITE setParameterUnit NOTIFY parameterUnitChanged)
    Q_PROPERTY(bool ParameterSwitch READ getParameterSwitch WRITE setParameterSwitch NOTIFY ParameterSwitchChanged)
    Q_PROPERTY(bool ParameterDoubleSwitch READ getParameterDoubleSwitch WRITE setParameterDoubleSwitch NOTIFY ParameterDoubleSwitchChanged)
    Q_PROPERTY(bool ParameterSwitchState READ getParameterSwitchState WRITE setParameterSwitchState NOTIFY ParameterSwitchStateChanged)
    Q_PROPERTY(QString ParameterMinValue READ getParameterMinValue WRITE setParameterMinValue NOTIFY parameterMinValueChanged)
    Q_PROPERTY(QString ParameterMaxValue READ getParameterMaxValue WRITE setParameterMaxValue NOTIFY parameterMaxValueChanged)
    Q_PROPERTY(QString ParameterStripColor READ getParameterStripColor WRITE setParameterStripColor NOTIFY parameterStripColorChanged)
    Q_PROPERTY(bool ParameterTripleSwitch READ getParameterTripleSwitch WRITE setParameterTripleSwitch NOTIFY ParameterTripleSwitchChanged)
    Q_PROPERTY(bool ParameterVisible READ getParameterVisible WRITE setParameterVisible NOTIFY ParameterVisibleChanged)
    Q_PROPERTY(QString DoubleSwitchLeftText READ getDoubleSwitchLeftText WRITE setDoubleSwitchLeftText NOTIFY DoubleSwitchLeftTextChanged)
    Q_PROPERTY(QString DoubleSwitchRightText READ getDoubleSwitchRightText WRITE setDoubleSwitchRightText NOTIFY DoubleSwitchRightTextChanged)

public:
    RecipeParameter(QObject *parent=0);
    RecipeParameter(const QString name, int index, const QString value,const QString unit,
                    bool isSwitch=false, bool doubleSwitch = false,bool switchState=false,
                    bool tripleSwitch = false, QObject *parent=0, bool parameterVisible = false);

    QString m_ParameterName;
    QString m_ParameterLocaleName;
    QString m_ParameterValue;
    QString m_ParameterEndValue;
    QString m_ParameterUnit;
    QString m_ParameterMinValue;
    QString m_ParameterMaxValue;
    QString m_ParameterStripColor;
    QString m_DoubleSwitchLeftText;
    QString m_DoubleSwitchRightText;

    bool m_ParameterVisible;
    bool m_ParameterSwitch;
    bool m_ParameterDoubleSwitch;
    bool m_ParameterTripleSwitch;
    bool m_ParameterSwitchState;
    int m_ParameterIndex;

    QString getParameterName() const;
    void setParameterName(const QString name);

    QString getParameterLocaleName() const;
    void setParameterLocaleName(const QString name);

    int getParameterIndex();
    void setParameterIndex(int index);

    QString getParameterValue() const;
    void setParameterValue(const QString value);

    QString getParameterUnit() const;
    void setParameterUnit(const QString unit);

    bool getParameterSwitch();
    void setParameterSwitch(bool isSwitch);

    bool getParameterDoubleSwitch();
    void setParameterDoubleSwitch(bool isSwitch);

    bool getParameterSwitchState();
    void setParameterSwitchState(bool switchState);

    QString getParameterMinValue() const;
    void setParameterMinValue(const QString minValue);

    QString getParameterMaxValue() const;
    void setParameterMaxValue(const QString maxValue);

    QString getParameterStripColor() const;
    void setParameterStripColor(const QString stripColor);

    bool getParameterTripleSwitch();
    void setParameterTripleSwitch(bool isSwitch);

    bool getParameterVisible();
    void setParameterVisible(bool ReacVisible);

    QString getDoubleSwitchLeftText() const;
    void setDoubleSwitchLeftText(const QString leftText);

    QString getDoubleSwitchRightText() const;
    void setDoubleSwitchRightText(const QString rightText);

signals:
    void parameterNameChanged();
    void parameterLocaleNameChanged();
    void parameterIndexChanged();
    void parameterValueChanged();
    void parameterUnitChanged();
    void ParameterSwitchChanged();
    void ParameterDoubleSwitchChanged();
    void ParameterSwitchStateChanged();
    void parameterMinValueChanged();
    void parameterMaxValueChanged();
    void parameterStripColorChanged();
    void ParameterTripleSwitchChanged();
    void ParameterVisibleChanged();
    void DoubleSwitchLeftTextChanged();
    void DoubleSwitchRightTextChanged();

};

#endif // RECIPEPARAMETER_H

