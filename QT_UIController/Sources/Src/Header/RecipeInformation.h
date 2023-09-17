/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Recipe Information

***************************************************************************/

#ifndef RECIPEINFORMATION_H
#define RECIPEINFORMATION_H
#include <QObject>
#include "Header/recipe.h"

#define CUST_NAME_TIME  QObject::tr("Time")
#define CUST_NAME_ENERGY  QObject::tr("Energy")
#define CUST_NAME_PEAK  QObject::tr("Peak power")
#define CUST_NAME_SCRUB  QObject::tr("Scrub Time")
#define CUST_NAME_ABSOLUTE  QObject::tr("Absolute Distance")
#define CUST_NAME_COLLAPSE  QObject::tr("Collapse Distance")
#define CUST_NAME_MULTI_MODE  QObject::tr("Multi Mode")
#define CUST_NAME_NA QObject::tr("NA")

#define CUST_NAME_UINTSEC "s"
#define CUST_NAME_UINTMilliMeter "mm"
#define CUST_NAME_UINTINCHES "in"
#define CUST_NAME_UINTLBS "lbs"
#define CUST_NAME_UINTJOULES "J"
#define CUST_NAME_UINTWATTS "W"
#define CUST_NAME_EXPECTED_RESULTS 8
#define CUST_NAME_EXPECTED_ALARM_RESULTS 3
#define CUST_NAME_NOACTIVE_RECIPE QObject::tr("No Active Recipe Available")
#define CUST_NAME_PARTIMAGES_FOLDER "/PartImages/"

class RecipeInformation: public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE void recipeInfoInit(QString qStrRecipeNumber,QString qStrRecipeName);
    Q_INVOKABLE QString getInfoRecipeName();
    Q_INVOKABLE QString getRecipeDescription();
    Q_INVOKABLE QString getProductionInstruction();
    Q_INVOKABLE QString getCompany();
    Q_INVOKABLE QString getControlmode();
    Q_INVOKABLE QString getControlmodeValue();
    Q_INVOKABLE QString getControlModeUnit();
    explicit RecipeInformation(QObject *parent = 0);
    void FillRecipeInfo(QStringList );
    void InitializationOfParams();

private:
    QString m_infoRecipeName;
    QString m_recipeDescription;
    QString m_productionInstruction;
    QString m_Company;
    QString m_weldMode;
    QString m_controlValue;
    QString m_controlValueUnit;
    QString m_infoRecipeNumber;
    QString m_PartImageNameTodisplay;
    QString m_PowerSupplyAssembly;
    QString m_ActuatorAssembly;
    QString m_StackAssembly;
    bool m_isValidate;

};

#endif // RECIPEINFORMATION_H
