#ifndef SMARTSTACK_H
#define SMARTSTACK_H

#include <QString>
#include <QObject>
#include "custompropertyhelper.h"
#include "Header/communicationinterface.h"
#include "Header/recipe.h"
#define SMART_STACK_DATA_SIZE        20
#define MEMORY_CLEARED               QObject::tr("Memory cleared")
#define MEMORY_NOTCLEARED            QObject::tr("Memory is not cleared")
#define TURNOFF_TO_INSTALL_STACK     QObject::tr("Barcode scan successful. Please turn OFF power to the power supply and install the new stack. Once stack is installed, reapply power")

struct SmartStackSt
{
    UINT32  StackSerialNum;
    UINT16  Amplitude;
    UINT16  Voltage;
    char    BirthDate[SMART_STACK_DATA_SIZE];
    char    EDP[SMART_STACK_DATA_SIZE];
    char    Model[SMART_STACK_DATA_SIZE];
};


class SmartStack : public QObject
{
    Q_OBJECT

public:
    explicit SmartStack(QObject *parent = nullptr): QObject(parent)
    {
        bIsChangeStackClicked = false;
        qstrNewSmartStack = " , , , , , , ";
        qstrCurrentSmartStack = " , , , , , , ";
    }
    SmartStack(QString ParameterName,bool bIsTextField) :
        m_ParameterName(ParameterName),
        m_bIsTextField(bIsTextField){}
    CUSTOM_PROPERTY(QString, ParameterName)
    CUSTOM_PROPERTY(bool, bIsTextField)

    Q_INVOKABLE void smartStackInit();
    void createCurrentStackModel();
    void createFixedStackModel();
    void createNewStackModel();
    Q_INVOKABLE bool getIsChangeStackClickedStatus();
    Q_INVOKABLE void sendClearMemoryReqToSC();
    QString getFixedStackModel();
    Q_INVOKABLE void sendChangeStackReqToSC();
    Q_INVOKABLE void sendCancelStackReqToSC();
    Q_INVOKABLE void changeStackInd(QString strSmartStack);
    Q_INVOKABLE void clearNewStackInfo();
    Q_INVOKABLE void copyNewStackToCurrentStack();
    void memoryClearedInd(QString strMemClearStatus);
    QList<QObject*> m_FixedModelList;
    QList<QObject*> m_CurrentModelList;
    QList<QObject*> m_NewModelList;
    QList<QObject*> m_DummyList;
    bool bIsChangeStackClicked;
    QString qstrNewSmartStack;
    QString qstrCurrentSmartStack;
    SmartStackSt stSmartStack;

};
#endif // SMARTSTACK_H
