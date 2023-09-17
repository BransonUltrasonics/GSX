#ifndef CUSTOMEVENTFILTER_H
#define CUSTOMEVENTFILTER_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <QFile>

class CustomEventFilter : public QObject
{
    Q_OBJECT
public:
    int m_TimeOut;
    QTimer m_Timer;
    bool m_bIsSleeping = false;

    explicit CustomEventFilter(QObject *parent = 0);
    bool eventFilter(QObject *obj, QEvent *ev);

    Q_INVOKABLE void setTimeOut(int milliseconds);

public slots:
    void appSlepp();

signals:
    void logOut();
};
#endif // CUSTOMEVENTFILTER_H
