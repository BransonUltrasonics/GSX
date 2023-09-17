#ifndef SETUPPARAMETER_H
#define SETUPPARAMETER_H

#include <QObject>

class SetupParameter : public QObject
{
    Q_PROPERTY(QString ParamName READ getParamName WRITE setParamName NOTIFY paramNameChanged)
    Q_PROPERTY(QString ParamValue READ getParamValue WRITE setParamValue NOTIFY paramValueChanged)
    Q_PROPERTY(QString ParamUnit READ getParamUnit WRITE setParamUnit NOTIFY paramUnitChanged)
    Q_OBJECT

public:
    SetupParameter(QObject *parent=0);
    SetupParameter(QString a_paramName,
    QString a_paramValue,
    QString a_paramUnit,QObject *parent = nullptr);

    QString m_paramName;
    QString m_paramValue;
    QString m_paramUnit;

    QString getParamName();
    QString getParamValue();
    QString getParamUnit();

    void setParamName(QString a_ParamName);
    void setParamValue(QString a_ParamValue);
    void setParamUnit(QString a_ParamUnit);

signals:
    void paramNameChanged();
    void paramValueChanged();
    void paramUnitChanged();
};
#endif // SETUPPARAMETER_H
