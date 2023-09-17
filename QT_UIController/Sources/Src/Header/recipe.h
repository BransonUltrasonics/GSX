/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Recipe

***************************************************************************/


#ifndef RECIPE_H
#define RECIPE_H

#include <QObject>
#include <QList>
#include <Header/recipeparameter.h>
#include <Header/suspectrejectparameter.h>
#include <Header/pressureparameter.h>
#include <Header/setuplimitis.h>
#include <Header/menulimits.h>
#include <Header/modelweldmodes.h>
#include <QQmlContext>
#include <Header/clientsocket.h>
#include <Header/communicationinterface.h>
#include <Header/welduitobl.h>
#include <QMap>
#include <QPair>
#include <Header/weldrecipeparameterdata.h>

#define RECIPE_BARCODE_FOUND                                    QObject::tr(" found and made Active")
#define RECIPE_BARCODE_NOTFOUND                                 QObject::tr(" Barcode Recipe not found, Recalling Weld Recipe Failed ")
#define RECIPE_BARCODE_PRESENT                                  QObject::tr("Barcode scanner connected")
#define RECIPE_BARCODE_NOTPRESENT                               QObject::tr("Barcode Scanner not present")
#define BARCODE_RECIPE                                          QObject::tr("Barcode recipe : ")
#define BARCODE_FAILURE_INVALID_RECIPE_BARCODE_LENGTH           QObject::tr("Invalid Barcode, Barcode limit crossed")
#define BARCODE_FAILURE_INVALID_RECIPE_BARCODE_NOT_FOUND        QObject::tr("Invalid Recipe Barcode, Recipe not found")
#define BARCODE_FAILURE_INVALID_RECIPE_BARCODE                  QObject::tr("Invalid Recipe Barcode, Recipe number beyond range")
#define BARCODE_FAILURE_INVALID_SMART_STACK                     QObject::tr("Invalid Smart Stack")
#define BARCODE_FAILURE_INVALID_PARTID                          QObject::tr("Invalid Part ID")
#define BARCODE_FAILURE_INVALID_BARCODE                         QObject::tr("Invalid Barcode")
#define RECIPE_SAVED_INVALIDATED                                QObject::tr("Weld recipe saved successfully and invalidated")
#define RECIPE_SAVED                                            QObject::tr("Weld recipe saved successfully")
#define RECIPE_VALIDATED                                        QObject::tr("Recipe validated")
#define RECIPE_INVALIDATED                                      QObject::tr("Recipe Invalidated")

#define WEB_SERVICES_CHANGED                                    QObject::tr("A Web Services change was made: ")
#define SWITCH_SCREENS_MESSAGE                                  QObject::tr(" Please switch screens to see the updates")
#define ACTIVE_RECIPE_CHANGED                                   QObject::tr("ACTIVE RECIPE CHANGED")
#define ACTIVE_RECIPE_SAVED                                     QObject::tr("ACTIVE RECIPE SAVED")
#define RECIPE_PARAMETER_CHANGED                                QObject::tr("RECIPE PARAMETER CHANGED")
#define RECIPE_DELETED                                          QObject::tr("RECIPE DELETED")

#define RECIPE_BARCODE_ACTIVE_RECIPE_FOUND          			1
#define RECIPE_BARCODE_ACTIVE_RECIPE_NOT_FOUND      			0
#define FrequencySelected20KHZ                  20000
#define FrequencySelected30KHZ                  30000
#define FrequencySelected40KHZ                  40000

#define propertycount                           6
#define RECIPE_LIST_PARAM_COUNT                 9
#define CFR_ENABLED                             1
struct stModifiedRecipeInfo
{
    QString qstrRecipeName;
    QString qstrWeldMode;
    QString qstrModeValue;
    QString qstrUnit;
    QString qstrModeNameScreen;
};

enum WeldHistoryDataLoad
{
    ENUM_LOAD_ALL = 0,
    ENUM_LOAD_RECENT,
    ENUM_LOAD_NEXT_PAGE
};

enum RecipeModelIndex
{
    ENUM_CurrentRecipeFixedSixParameter=1,
    ENUM_DummyCurrentRecipeFixedSixParameter,
    ENUM_NewRecipeFixedParameter,
    ENUM_DummyNewRecipeFixedParameter,
    ENUM_ParamAtoZ,
    ENUM_DummyParamAtoZ,
    ENUM_WeldResults,
    ENUM_DummyWeldResults,
    ENUM_MenuList,
    ENUM_WeldMode,
    ENUM_WeldMode_DUMMY,
    ENUM_SteppingModes,
    ENUM_DummySteppingModes,
    ENUM_SuspectRejectParamList,
    ENUM_DummySuspectRecjectParamList,
    ENUM_DummyActuatorSetUpdata,
    ENUM_DummyActuatorSetUpAdvanceddata,
    ENUM_ActuatorSetUpdata,
    ENUM_ActuatorSetUpAdvanceddata,
    ENUM_PressureList,
    ENUM_DummyPressureList,
    ENUM_ControlList,
    ENUM_DummyControlList,
    ENUM_SetUplist,
    ENUM_DummySetUpList,
    ENUM_RecipeModelList,
    ENUM_WeldProcessModelList,
    ENUM_DummyWeldProcessModelList,
    ENUM_WeldProcessAfterBurstModelList,
    ENUM_DummyweldProcessAfterBurstModelList,
    ENUM_WeldProcessPreTriggerModelList,
    ENUM_DummyweldProcessPreTriggerModelList,
    ENUM_DummyRecipeModelList,
    ENUM_DummyweldProcessEnergyBrakeModelList,
    ENUM_WeldProcessEnergyBrakeModelList,
    ENUM_BransonOnlyList,
    ENUM_DUMMYBransonOnlyList
};

enum RecipeInfoListDBIndex
{
    RECIPE_INFO_RECIPE_NUMBER = 0,
    RECIPE_INFO_RECIPE_NAME,
    RECIPE_INFO_COMPANY_NAME,
    RECIPE_INFO_WELD_MODE,
    RECIPE_INFO_WELD_MODE_VALUE,
    RECIPE_INFO_ACTIVE_STATE,
    RECIPE_INFO_LOCKED_STATE,
    RECIPE_INFO_VALIDATE_STATE,
    RECIPE_INFO_RECIPE_VERSION_NUMBER
};

enum ParameterLimitChecks
{
    SUCCESS = 0,
    INVALID_ENTRY,
    PARAM_OUTOF_RANGE,
    SR_OUTOF_RANGE,
    PARAM_MID_POINT_SR,
    PARAM_INVALID,
    PARAM_MID_POINT
};

enum WSTypeOfChange
{
    SET_ACTIVE_RECIPE = 1,
    SAVE_CURRENT_RECIPE,
    SET_RECIPE_VALUE,
    DELETE_RECIPE
};

class RecipeInfo:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString RecipeNumber READ getModelRecipeNumber NOTIFY OnModelRecipeNumberChanged)
    Q_PROPERTY(QString RecipeName READ getModelRecipeName NOTIFY OnModelRecipeNameChanged)
    Q_PROPERTY(QString CompanyName READ getModelCompanyName NOTIFY OnModelCompanyNameChanged )

    Q_PROPERTY(QString WeldMode READ getModelWeldMode NOTIFY OnModelWeldModeChanged )
    Q_PROPERTY(QString WeldModeUnit READ getModelWeldModeUnit NOTIFY OnModelWeldModeUnitChanged)
    Q_PROPERTY(QString WeldModeValue READ getModelWeldModeValue NOTIFY OnModelWeldModeValueChanged)
    Q_PROPERTY(QString WeldModeNameScreen READ getModelWeldModeNameScreen NOTIFY OnModelWeldModeNameScreenChanged)

    Q_PROPERTY(bool isActive READ getModelisActive NOTIFY OnModelisActiveChanged)
    Q_PROPERTY(QString isLockedValue READ getModelisLockedValue  NOTIFY OnModelisLockedValueChanged)
    Q_PROPERTY(bool isValidate READ getModelisValidate NOTIFY OnModelisValidateChanged )
    Q_PROPERTY(bool isModified READ getModelisModified NOTIFY OnModelisModifiedChanged )

public:
    QString m_RecipeName;
    QString m_WeldMode;
    QString m_CompanyName;
    QString m_WeldModeValue;
    QString m_WeldModeUnit;
    QString m_isLockedValue;
    QString m_weldRecipeNumber;
    QString m_RecipeNumber;
    QString m_WeldModeNameScreen;
    bool m_isActive;
    bool m_isValidate;
    bool m_isModified;

    QString getModelRecipeNumber();
    QString getModelRecipeName();
    QString getModelCompanyName();
    QString getModelWeldMode();
    QString getModelWeldModeNameScreen();
    QString getModelWeldModeUnit();
    QString getModelWeldModeValue();
    QString getModelisLockedValue();
    bool getModelisActive();
    bool getModelisValidate();
    bool getModelisModified();

signals:
    void OnModelRecipeNumberChanged();
    void OnModelRecipeNameChanged();
    void OnModelCompanyNameChanged();
    void OnModelWeldModeChanged();
    void OnModelWeldModeNameScreenChanged();
    void OnModelWeldModeUnitChanged();
    void OnModelWeldModeValueChanged();
    void OnModelisActiveChanged();
    void OnModelisLockedValueChanged();
    void OnModelisValidateChanged();
    void OnModelisModifiedChanged();
};

class Recipe : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isRecipeModified READ getRecipeModifiedState WRITE setRecipeModifiedState NOTIFY RecipeModifiedStateChanged)
    Q_PROPERTY(bool autoGraphRefreshState READ getAutoGraphRefreshState WRITE setAutoGraphRefreshState NOTIFY AutoGraphRefreshStateChanged)
    Q_PROPERTY(bool isManualRefresh READ getIsManualRefreshState WRITE setIsManualRefreshState NOTIFY IsManualRefreshStateChanged)

private:
    QList<QObject*> m_FixedParamModelList;
    QList<QObject*> m_PreTriggerModelList;
    QList<QObject*> m_EnergyBrakeModelList;
    QList<QObject*> m_AfterBurstModelList;
    QList<QObject*> m_AtoZParamModelList;
    QList<QObject*> m_ControlLimitsModelList;
    QList<QObject*> m_BransonParamsModelList;
    QList<QObject*> m_ActuatorSetupParamsModelList;
    QList<QObject*> m_ActuatorSetupAdvancedParamsModelList;
    QList<QObject*> m_TableGraphRow;
    QList<QObject*> m_DummyModelList;

    WeldRecipeParameter m_WeldRecipeParam;
    ParamUnits m_Units;
    bool m_EditRecipeClicked;


    int currentIndex;
    int m_MachineFrequency;
    int m_reciveRecipeNumber;
    int m_isValidatevalue;
    int m_CopiedRecipeNumber;
    QString persistentRecipeNum;
    QString oldRecipeNumber;
    QStringList recipeNumberBufferList;
    QStringList paramAtoZList;
    QStringList paramAtoZLocaleList;

    QStringList getSteppingComboBoxList();
    QString getWeldModesList();
    QString getLocaleWeldModesList();
    QString getWeldModesListIndexes();
    QString getLimitsMenu();
    QString getLocaleLimitsMenu();
    QString getWeldProcessMenu();
    QString getLocaleWeldProcessMenu();
    QString getFixedParamsList();
    QString getLocaleFixedParamsList();
    QString getPreTriggerParamsList();
    QString getLocalePreTriggerParamsList();
    QString getEnergyBrakeParamsList();
    QString getLocaleEnergyBrakeParamsList();
    QString getAfterBurstParamsList();
    QString getLocaleAfterBurstParamsList();
    QString getAtoZParamsList();
    QString getLocaleAtoZParamsList();
    QString getSetupLimitsParamsList();
    QString getLocaleSetupLimitsParamsList();
    QString getControlLimitsParamsList();
    QString getLocaleControlLimitsParamsList();
    QString getSuspectRejectLimitsParamsList();
    QString getLocaleSuspectRejectLimitsParamsList();
    QString getBransonParamsList();
    QString getLocaleBransonParamsList();
    QString getActuatorSetupParamsList();
    QString getLocaleActuatorSetupParamsList();
    QString getActuatorSetupAdvancedParamsList();
    QString getLocaleActuatorSetupAdvancedParamsList();

    void createParamModel(QList<QObject*> *ptrModelList,QString qstrParams,QString qstrParamsLocale);
    void clearList(QList<QObject*> *tempList);
    void clearWeldResultList(QList<QObject*> *tempList);
    void clearWeldResultTableList(QList<QObject*> *tempList);

    void createSteppingComboBoxModel();
    void createWeldModesModel();
    void createLimitsMenu();
    void createWeldProcessMenu();
    void createFixedParamModel();
    void createPreTriggerModel();
    void createEnergyBrakeModel();
    void createAfterBurstModel();
    void createAtoZParamModel();
    void createSetupLimitsModel();
    void createControlLimitsModel();
    void createSuspectRejectLimitsModel();
    void createBransonParamsModel();
    void createActuatorSetupParamsModel();
    void createActuatorSetupAdvancedParamsModel();

    void loadAllModels();
    void LoadParamVal(QList<QObject*> *ptrModelList);
    void loadFixedParamModel();
    void loadPreTriggerModel();
    void loadEnergyBrakeModel();
    void loadAfterBurstModel();
    void loadAtoZParamModel();
    void loadSetupLimitsModel();
    void loadControlLimitsModel();
    void loadSuspectRejectLimitsModel();
    void loadBransonParamsModel();

    QString getMaxRecipeNumberFromDb();
    QString getRecipeList();

public:

    bool isAutoGraphRefreshState;
    bool isManualRefreshState;
    bool m_newRecipeCreated;

    int dwLastSelectedResultIndex;
    int m_clickedRecipe_Index;
    int m_RecipeModifyStatusFlag;

    double m_resonantFrequency;

    QStringList m_recipeButtonFullListOtherUsers;
    QStringList m_recipeButtonFullList;
    QStringList m_recipeButtonFullListUnvalidated;
    QStringList m_recipeButtonListByUserlevel;

    QString m_sentToDbList;
    QString m_suspectRejectBufferFromDB;
    QString m_BransonOnlyListFromDB;
    QString m_strHeaderOrder;

    QList<QObject*> weldResults;
    QList<QObject*> m_menuList;
    QList<QObject*> m_weldmode;
    QList<QObject*> m_steppingmodes;
    QList<QObject*> m_suspectRejectParamList;
    QList<QObject*> m_setupList;
    QList<QObject*> m_RecipeModelList;
    QList<QObject*> m_weldProcessModelList;

    QMap<QString,stRecipeParmInfo> m_MapRecipeParams;
    SystemFreq m_CurrentSystemFreq;

    explicit Recipe(QObject *parent = nullptr);

    void loadActatorSetupParamsModel();
    void loadActatorSetupAdvancedParamsModel();
    void createAllModels();
    void editFixedRecipeParam(QString qstrParamName, QString qstrParamValue);
    void setSystemConfigData(int dwMetrix, int dwPsFreq);
    void recallRecipeByBarcode(QString sRecipeNumber);
    void barcodeConnectionStatus(QString sbarcodeConnectionStatus);
    void barcodeProcessInfo(QString sbarcodeProcessInfo);
    void SetDefaultProductionDetail();
    void weldDataDeleteIndication(QString qstrBuff);
    void refreshModel(int);

    stModifiedRecipeInfo getCurrentRecipeInfo();

    ModeInfo GetModeInfoByIndex(int dwModeIndex,QString qstrModeValue);

    Q_INVOKABLE int validateSuspectRejectValues(bool isSuspectEnabled, QString qstrSuspectLow, QString qstrSuspectHigh,
                                                bool isRejectEnabled, QString qstrRejectLow, QString qstrRejectHigh,
                                                QString qstrParamName);
    Q_INVOKABLE int checkSuspectRejectValues(bool isGlobalSuspectEnabled,bool isGlobalRejectEnabled,bool isSuspectEnabled, QString qstrSuspectLow, QString qstrSuspectHigh,
                                                bool isRejectEnabled, QString qstrRejectLow, QString qstrRejectHigh,
                                                QString qstrParamName);
    Q_INVOKABLE void createNewRecipe();
    Q_INVOKABLE void editExistingRecipe(QString qstrRecipeNumber);
    Q_INVOKABLE void loadDataForModel(int dwModelIndex);
    Q_INVOKABLE void editWeldRecipePramValue(QString qstrParamName, QString qstrParamValue);
    Q_INVOKABLE void loadRecipeCards();
    Q_INVOKABLE void updateRecipeMenuList(int userLevel);
    Q_INVOKABLE void setStructureValue(QString qstrParamName,QString qstrValue,bool bSendToBL=true);
    Q_INVOKABLE void copySelectedRecipe(QString qstrSourceRecipeNumber,QString qstrNewRecipeNumber,bool isNewRecipe);
    Q_INVOKABLE void exisistingRecipeEdited(QString qstrSourceRecipeNumber,QString qstrNewRecipeNumber);
    Q_INVOKABLE void changeWeldMode(QString weldmode);
    Q_INVOKABLE void setSelectedRecipeToActive(QString qStrRecipeNum);
    Q_INVOKABLE void readResonantFrequencyFromDB();
    Q_INVOKABLE void setLastSelectedResult(QString qstrIndex);
    Q_INVOKABLE int readRecipeFromDB(QString);
    Q_INVOKABLE QString storeRecipeToDB();
    Q_INVOKABLE void updateWeldHistoryList(int dwInsertIndex=0);
    Q_INVOKABLE void updateTableGraphModel();
    Q_INVOKABLE void updateTableGridViewModel();
    Q_INVOKABLE void setHistoryToNull();
    Q_INVOKABLE void hornDownRestoreDefaultAdvParameters();
    Q_INVOKABLE void updateWeldHistory();
    Q_INVOKABLE void isActiveRequired(bool bIsActiveReq);
    Q_INVOKABLE void setActuatorRecipeValue(QString qstrValue, bool independentRecipeSendToBL);
    Q_INVOKABLE void updateAcutatorForceSetting();
    QString getActuatorRecipeForceStructureValue();
    Q_INVOKABLE int getNewRecipeNumber();
    Q_INVOKABLE int validateRecipeNumber(QString);
    Q_INVOKABLE int getStructureValue(int dwIndex);
    Q_INVOKABLE int setSelectedRecipeToValidate(QString qStrRecipeNum);
    Q_INVOKABLE int getPastSelectIndex(QString qstrPrevResultSelected);
    Q_INVOKABLE int getSelectedIndex();
    Q_INVOKABLE int midPointValidationForAllParams(bool,bool,bool);

    Q_INVOKABLE bool controlLimitValidation(QString numpadTitle,QString keyInputText);
    Q_INVOKABLE int checkSetupLimits(double,double,bool,QString);
    Q_INVOKABLE bool midPointValidation(QString a_paramName,double a_low, double a_high);
    Q_INVOKABLE bool getModifiedStatus(QString qstrRecipeNumber);
    Q_INVOKABLE bool getRecipeRecallAuthCheck(QString sRecvBuff);
    Q_INVOKABLE bool getRecipeModifiedState();
    Q_INVOKABLE bool getAutoGraphRefreshState();
    Q_INVOKABLE bool getIsManualRefreshState();
    Q_INVOKABLE bool checkControlLimits(bool bState,QString qstrParamName,QString qstrParamValue);
    bool validateSRWithSetupLimits(QString qstrParamName,double dSRLow, double dSRHigh);
    bool validateSRWithControlLimits(QString qstrParamName,double dSRLow, double dSRHigh);

    Q_INVOKABLE QString getCurrentRecipeName();
    Q_INVOKABLE QString getStructureValueByName(QString paramEditText);
    Q_INVOKABLE QString getMaxValue(QString paramEditText);
    Q_INVOKABLE QString getRecipeName();
    Q_INVOKABLE QString getRecipeNumber();
    Q_INVOKABLE QString getLastSelectedResult();
    Q_INVOKABLE QString keypadlimitation(QString,QString);
    Q_INVOKABLE QString setupLimitValidation(QString keypadTextname,QString keypadValue);
    Q_INVOKABLE QString getDefaultValue(QString qstrParamName);
    Q_INVOKABLE QString paramRangeValidation(QString keypadTextname,QString keypadValue);

    Q_INVOKABLE QList<RecipeInfo *>RecipeInfoList;
    Q_INVOKABLE int existingRecipeCount();

    Q_INVOKABLE void setParamaterChangedFlag(int);
    Q_INVOKABLE void getAtoZParamsMap();

    Q_INVOKABLE bool isRuntimeFeatureEnabledForCFR();
	Q_INVOKABLE void displayandStoreGraphData();


    Q_INVOKABLE int deleteArecipeCard(int recipeNumber);



signals:
    void RecipeModifiedStateChanged();
    void AutoGraphRefreshStateChanged();
    void IsManualRefreshStateChanged();
    Q_INVOKABLE void updateStatusText(QString strStatusText); //This will display text in UI Status bar
    Q_INVOKABLE void refreshPage(int dwIndex); //This will display text in UI Status bar
    Q_INVOKABLE void refreshScreenPopUpSignal(QString popUpString, int typeOfChangeIndex);
public slots:
    void setRecipeModifiedState(bool);
    Q_INVOKABLE void setAutoGraphRefreshState(bool);
    Q_INVOKABLE void setIsManualRefreshState(bool);
    Q_INVOKABLE void refreshScreenPopUp(QString typeOfChange);
};

class WeldResultData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString WeldResultID READ GetWeldResultID NOTIFY OnWeldResultIDChanged )
    Q_PROPERTY(QString WeldResultMode READ GetWeldResultMode NOTIFY OnWeldResultModeChanged )
    Q_PROPERTY(QString WeldResultNumber READ GetWeldResultNumber NOTIFY OnWeldResultNumberChanged )
    Q_PROPERTY(QString WeldResultDate READ GetWeldResultDate NOTIFY OnWeldResultDateChanged)
    Q_PROPERTY(QString WeldResultTime READ GetWeldResultTime NOTIFY OnWeldResultDateChanged)
    Q_PROPERTY(QString AlaramRaised READ GetAlaramRaised NOTIFY OnAlaramRaisedChanged)
    Q_PROPERTY(QString WeldResultRowId READ GetWeldResultRowId NOTIFY OnWeldResultRowIdChanged)

    QString m_WeldResultID;
    QString m_WeldResultMode;
    QString m_WeldResultNumber;
    QString m_WeldResultDate;
    QString m_WeldResultTime;
    QString m_AlaramRaised;
    QString m_WeldResultRowId;

public:
    QString GetWeldResultID();
    QString GetWeldResultMode();
    QString GetWeldResultNumber();
    QString GetWeldResultDate();
    QString GetWeldResultTime();
    QString GetAlaramRaised();
    QString GetWeldResultRowId();
    WeldResultData(QString WeldResultID,QString WeldResultMode,QString WeldResultNumber,QString WeldResultDate,
                   QString WeldResultTime, QString AlaramRaised, QString RowId);

signals:
    void OnWeldResultIDChanged();
    void OnWeldResultModeChanged();
    void OnWeldResultNumberChanged();
    void OnWeldResultDateChanged();
    void OnAlaramRaisedChanged();
    void OnWeldResultRowIdChanged();
};

#endif // RECIPE_H
