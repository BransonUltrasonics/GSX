#ifndef CUSTOMCALENDAR_H
#define CUSTOMCALENDAR_H

#include <QObject>
#include <QQmlContext>

#define CUSTOM_CALENDER_START_YEAR          2000
#define CUSTOM_CALENDER_NUM_OF_YEARS        31

class CustomCalendar : public QObject
{
     Q_OBJECT
public:
    CustomCalendar(QObject *parent = nullptr);
    QString getYearsList();
    Q_INVOKABLE void initializeMonthYearModel();
};
#endif // CUSTOMCALENDAR_H
