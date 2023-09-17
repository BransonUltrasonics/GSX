#ifndef FIRMWAREUPGRADE_H
#define FIRMWAREUPGRADE_H

#define NO_FIRMWARE_UPGRADE "None"
#define FILEDELIMITER       "||"
#define FILESEPERATOR       ","

#include <QObject>
#include <QQmlEngine>
#include <QDebug>
#include <QQmlContext>

class FirmwareUpgrade : public QObject
{
    Q_OBJECT

public:
    explicit FirmwareUpgrade(QObject *parent = nullptr);

    enum Firmwareupgradetype
    {
        ACTUATOR_CONTROLLER=1,
        POWER_CONTROLLER,
        SUPERVISORY_CONTROLLER
    };

    static void declareQML()
    {
        qmlRegisterType<FirmwareUpgrade>("com.branson.firmwareupgradedefs", 1, 0, "Firmwareupgradetypes");
    }

    void upgradeProgressStatusData(QString qstrBuffer);
    void upgradeResponseStatus(QString qstrResponseBuffer);
    Q_INVOKABLE int readUSB();
    Q_INVOKABLE QString upgradeFirmwares(int scIndex,int acIndex,int pcIndex,int upgradeType);
    Q_INVOKABLE QStringList getfileslist(int nIdentifier);
    Q_INVOKABLE void upgradePopup(QString upgradeFiles);
    Q_ENUMS(Firmwareupgradetype)
    Q_INVOKABLE void clearfilelist();
    Q_INVOKABLE bool getUpgradeStatus();

signals:
    void upgradeCompletionStatus(int statusVal);
    void upgradeProgressDetail(int nModule,int nProgress);

private:
    QStringList qstrACUpgradeFilenames;
    QStringList qstrPCUpgradeFilenames;
    QStringList qstrSCUpgradeFilenames;
};

#endif // FIRMWAREUPGRADE_H
