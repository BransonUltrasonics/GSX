/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Check Box Model

***************************************************************************/
#ifndef CHECKBOX_H
#define CHECKBOX_H
#include <QObject>

#define CHECK_BOX_COLOR1                    "#f03e3e"
#define CHECK_BOX_COLOR2                    "#a82ba8"
#define CHECK_BOX_COLOR3                    "#4285f4"
#define CHECK_BOX_COLOR4                    "#bf6000"
#define CHECK_BOX_COLOR5                    "#ff80c0"
#define CHECK_BOX_COLOR6                    "#8BC24A"
#define CHECK_BOX_COLOR7                    "#524b03"
#define CHECK_BOX_COLOR8                    "#009587"
#define CHECK_BOX_COLOR9                    "#000001"
#define CHECK_BOX_COLOR10                   "#f4b400"

class checkbox:public QObject
{
    Q_OBJECT
public:
    QString m_name;
    QString m_value;
    QString m_unit;
    QString m_color;
    QList<QObject*> checkboxlist;
    QList<QObject*> dummycheckboxlist;
    QStringList qstrlstParamValues;
    QString m_parameterVal;
    bool m_isCheckBoxSelected;
    int m_isLeftAxis;

    checkbox(QString,QString,QString,QString,QString,bool,int);
    explicit checkbox(QObject *parent = nullptr);
    ~checkbox() ;

    Q_PROPERTY(QString checkBoxname READ getcheckBoxname  NOTIFY checkBoxnameChanged)
    Q_PROPERTY(QString checkBoxValue READ getcheckBoxValue  NOTIFY checkBoxValueChanged)
    Q_PROPERTY(QString checkBoxunit READ getcheckBoxunit  NOTIFY checkBoxunitChanged)
    Q_PROPERTY(QString checkBoxcolor READ getcheckBoxcolor  NOTIFY checkBoxcolorChanged)
    Q_PROPERTY(QString parameterval READ getparameterval WRITE setParameterVal  NOTIFY parametervalChanged)
    Q_PROPERTY(bool isCheckBoxSelected READ getisCheckBoxSelected WRITE setIsCheckBoxSelected  NOTIFY checkisCheckBoxSelected)
    Q_PROPERTY(int isLeftAxis READ getisLeftAxisSelected WRITE setIsLeftAxisSelected  NOTIFY checkisLeftAxisSelected)

    QString getcheckBoxname();
    QString getcheckBoxValue();
    QString getcheckBoxunit();
    QString getcheckBoxcolor();
    QString getparameterval();
    bool getisCheckBoxSelected();
    int getisLeftAxisSelected();
    void setIsLeftAxisSelected(int a_val);
    void setIsCheckBoxSelected(bool a_checkBoxSelected);
    void setParameterVal(QString a_parameterVal);
    void setParameterVal(unsigned int a_parameterVal);
    void setParameterVal(float a_parameterVal) ;
    void addcheckboxValue();
    Q_INVOKABLE void setAxisIndex(int,int);
    Q_INVOKABLE void setCheckboxStatus(int dwIndex, bool isChecked);
    Q_INVOKABLE void updateParameterVal(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    Q_INVOKABLE bool getComboIndex(int dwIndex);
    Q_INVOKABLE bool getCheckBoxState(int dwIndex);
    Q_INVOKABLE void setDefaultValues();
    Q_INVOKABLE void getDefaultValues();

signals:
    void checkBoxnameChanged();
    void checkBoxValueChanged();
    void checkBoxunitChanged();
    void checkBoxcolorChanged();
    void parametervalChanged();
    void checkisCheckBoxSelected();
    void checkisLeftAxisSelected();
 };
#endif // CHECKBOX_H
