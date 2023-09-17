/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Production Results

***************************************************************************/

#ifndef PRODUCTIONRESULTS_H
#define PRODUCTIONRESULTS_H
#include <QObject>

class ProductionResults : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ParameterName READ getParameterName WRITE setParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(QString ParameterValue READ getParameterValue WRITE setParameterValue NOTIFY parameterValueChanged)
    Q_PROPERTY(QString ParameterRecColor READ getParameterRecColor WRITE setParameterRecColor NOTIFY parameterRecColorChanged)
    Q_PROPERTY(int FontSize READ getFontSize WRITE setFontSize NOTIFY fontSizeChanged)


public:
    ProductionResults(QObject *parent=0);
    ProductionResults(const QString name,const QString value, const QString parameterColor, const int font, QObject *parent=0);

    QString m_ParameterName;
    QString m_ParameterValue;
    QString m_ParameterColor;
    int m_FontSize;

    QString getParameterName() const;
    void setParameterName(const QString name);

    QString getParameterValue() const;
    void setParameterValue(const QString value);

    QString getParameterRecColor() const;
    void setParameterRecColor(const QString value);

    int getFontSize() const;
    void setFontSize(const int value);

signals:
    void parameterNameChanged();
    void parameterValueChanged();
    void parameterRecColorChanged();
    void fontSizeChanged();

};
#endif // PRODUCTIONRESULTS_H
