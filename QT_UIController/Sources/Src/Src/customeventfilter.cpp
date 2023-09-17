/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Used for ideal logout form UI

***************************************************************************/

#include <QEvent>
#include <QFile>
#include <QDebug>
#include <QQmlContext>
#include "Header/customeventfilter.h"

extern QQmlContext* context;

/**
 * @brief CustomEventFilter::CustomEventFilter : Constructor
 * @param parent
 */
CustomEventFilter::CustomEventFilter(QObject *parent) :
    QObject(parent)
{
    m_TimeOut = 1800000;
    m_Timer.setInterval(m_TimeOut);
    connect(&m_Timer,SIGNAL(timeout()),this,SLOT(appSlepp()));
    m_Timer.start(m_TimeOut);
}

/**
 * @brief CustomEventFilter::eventFilter : Checks if any event is raised or not
 * @param obj
 * @param ev
 * @return
 */
bool CustomEventFilter::eventFilter(QObject *obj, QEvent *ev)
{
    static const int BACK_GROUND_THREAD_RUNNING = 1112;

    if(ev->type() == QEvent::KeyPress || ev->type() == QEvent::MouseMove || ev->type() == (QEvent::Type)BACK_GROUND_THREAD_RUNNING)
    {
        m_bIsSleeping = false;
        m_Timer.stop();
        m_Timer.start(m_TimeOut);
    }
    return QObject::eventFilter(obj, ev);
}

/**
 * @brief CustomEventFilter::appSlepp : Called when UI is in sleep and emits logout signal
 */
void CustomEventFilter::appSlepp()
{
    m_bIsSleeping = true;
    emit logOut();
}

/**
 * @brief CustomEventFilter::setTimeOut : Sets time out variable
 * @param milliseconds
 */
void CustomEventFilter::setTimeOut(int milliseconds)
{
    m_TimeOut = milliseconds;
    m_Timer.setInterval(m_TimeOut);
}
