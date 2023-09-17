#ifndef AUTHORITYOPTIONMODEL_H
#define AUTHORITYOPTIONMODEL_H

#include <QObject>
#include <QList>
#include <QString>
#include "custompropertyhelper.h"

#define AUTHORITYOPTIONMODEL_GENERAL            QObject::tr("General")
#define AUTHORITYOPTIONMODEL_USER_MANAGEMENT    QObject::tr("User Management")
#define AUTHORITYOPTIONMODEL_USER_AUTHORIYT     QObject::tr("User Authority")
#define AUTHORITYOPTIONMODEL_CHANGE_PASSWORD    QObject::tr("Change Password")
#define AUTHORITYOPTIONMODEL_ALARM_MANAGEMENT   QObject::tr("Alarm Management")
#define AUTHORITYOPTIONMODEL_IP_CONFIG          QObject::tr("IP Configuration")
#define AUTHORITYOPTIONMODEL_EXTERNAL_STATUS    QObject::tr("External Status")
#define AUTHORITYOPTIONMODEL_COMPONENTS         QObject::tr("Components")
#define AUTHORITYOPTIONMODEL_BRANSON            QObject::tr("Branson")
#define AUTHORITYOPTIONMODEL_USERIO             QObject::tr("User I/O")
#define AUTHORITYOPTIONMODEL_NA                 "N/A"
class ConfigurationData : public QObject
{
     Q_OBJECT
public:
    explicit ConfigurationData(QObject *parent = nullptr) : QObject(parent) {}
    ConfigurationData(int index, QString name, bool state) : m_index(index), m_name(name), m_state(state) {}
    CUSTOM_PROPERTY(int, index)
    CUSTOM_PROPERTY(QString, name)
    CUSTOM_PROPERTY(bool, state)
};

class AuthorityOptionModel : public QObject
{
    Q_OBJECT
public:
    explicit AuthorityOptionModel(QObject *parent = nullptr);
    ~AuthorityOptionModel();

    Q_INVOKABLE void updateList(int userLevel);
    Q_INVOKABLE void authorityOptionModelInit();
    Q_INVOKABLE int getDefaultIndex();
    void setUserLevel(int userLevel);
    void clearList(const QString& modelName, QList<QObject*>& usedList);

private:
    int m_userlevel;
    int m_defaultIndex;
    QList<QObject*> m_systemLevelConfigList;   
    QMap<int, QString> m_accesslist;
};

#endif // AUTHORITYOPTIONMODEL_H
