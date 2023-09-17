#ifndef WELDRECIPEPARAMETERDATA_H
#define WELDRECIPEPARAMETERDATA_H

#include <QObject>
#include <QMap>
#include <Header/welduitobl.h>

//#define WELD_MODE                   "Weld Mode"
#define WELD_MODE_TIME              "Time"
#define WELD_MODE_PEAK_POWER        "Peak Power"
#define WELD_MODE_ABS_DIS           "Absolute Distance"
#define WELD_MODE_COL_DIS           "Collapse Distance"
#define WELD_MODE_GND_DET           "Ground Detect"
#define WELD_MODE_GND_DET_VAL       "Scrub Time"
#define WELD_MODE_ENERGY            "Energy"
#define WELD_MODE_DYNAMIC           "Dynamic"
#define WELD_MODE_DYNAMIC_VAL       "Density"
#define WELD_PARAM_REACTIVIITY		"Reactivity"
#define WELD_PARAM_FORCE_LEVEL		"Force Level"
#define WELD_PARAM_TRI_DIS          "Trigger Distance"
#define WELD_PARAM_END_WELD_FORCE   "End Weld Force"
#define WELD_PARAM_FREQ             "Frequency"
#define FIXED_PARAM_WELD_AMP        "WELD AMPLITUDE"
#define FIXED_PARAM_TRI_FORCE       "TRIGGER FORCE"
#define FIXED_PARAM_WELD_FORCE      "WELD FORCE"
#define FIXED_PARAM_HOLD_FORCE      "HOLD FORCE"
#define FIXED_PARAM_HOLD_TIME       "HOLD TIME"
#define GLOBAL_SUPSPECT             "Global Suspect"
#define GLOBAL_REJECT               "Global Reject"

#define USERLEVEL_OPETATOR               "Operator"

#define SRLimitCnt 10
#define SRLimitDefaultVal "0:0:0:0:0:0:0:0:0:0"
#define SetupLimitCnt 3
#define SetupLimitDefaultVal "0:0:0"
#define ZERO_STR "0"

#define WELD_SETUPMODE_HIGH QObject::tr("setup weld mode high")
#define WELD_SETUPMODE_LOW QObject::tr("setup weld mode low")
#define WELD_DISTANCE_AUTO QObject::tr("DistanceAuto")
#define WELD_MULTIMODE QObject::tr("Multi Mode")
#define STATUS QObject::tr(" status")
#define SUSPECT QObject::tr("suspect ")
#define REJECT   QObject::tr( "reject ")
#define LOW     QObject::tr(" low")
#define HIGH    QObject::tr(" high")
#define VALUE   QObject::tr(" value")
#define SETUP   QObject::tr("setup ")
#define CUTOFF_STATE QObject::tr(" cutoff state")

#define WELD_RECIPE_GENERAL_DATA_SIZE 128
#define WELD_RECIPE_SUSPECT_REJECT_DATA_SIZE 8
#define WELD_RECIPE_BRANSON_ONLY_DATA_SIZE 39
#define WELD_RECIPE_SETUP_DATA_SIZE 10
#define WELD_RECIPE_STACK_DATA_SIZE 5

enum LimitsCheckState
{
    OFF=0,
    ON_SUCCESS,
    ON_FAIL_LIMIT,
    ON_FAIL_PARAM
};

enum WeldRecipeIndex
{
    //SC
    SC_RECIPE_NUMBER =1,
    SC_RECIPE_VER_NUMBER,
    SC_WELD_MODE,
    SC_MODE_VALUE,
    SC_HOLD_TIME,
    SC_TRIGGER_FORCE,
    SC_TRIGGER_DISTANCE,
    SC_MAX_WELD_TIMEOUT,
    SC_NUM_AMPLITUDE_STEPS,
    SC_AMPLITUDE_STEP_AT,
    SC_NUM_FORCE_STEPS,
    SC_FORCE_STEP_AT,
    SC_FORCE_STEP_VALUE1,
    SC_FORCE_STEP_VALUE2,
    SC_FORCE_STEP_VALUE3,
    SC_FORCE_STEP_VALUE4,
    SC_FORCE_STEP_VALUE5,
    SC_FORCE_STEP_VALUE6,
    SC_FORCE_STEP_VALUE7,
    SC_FORCE_STEP_VALUE8,
    SC_FORCE_STEP_VALUE9,
    SC_FORCE_STEP_VALUE10,
    SC_AMPLITUDE_STEP_VALUE1,
    SC_AMPLITUDE_STEP_VALUE2,
    SC_AMPLITUDE_STEP_VALUE3,
    SC_AMPLITUDE_STEP_VALUE4,
    SC_AMPLITUDE_STEP_VALUE5,
    SC_AMPLITUDE_STEP_VALUE6,
    SC_AMPLITUDE_STEP_VALUE7,
    SC_AMPLITUDE_STEP_VALUE8,
    SC_AMPLITUDE_STEP_VALUE9,
    SC_AMPLITUDE_STEP_VALUE10,
    SC_AMPLITUDE_STEP1,
    SC_AMPLITUDE_STEP2,
    SC_AMPLITUDE_STEP3,
    SC_AMPLITUDE_STEP4,
    SC_AMPLITUDE_STEP5,
    SC_AMPLITUDE_STEP6,
    SC_AMPLITUDE_STEP7,
    SC_AMPLITUDE_STEP8,
    SC_AMPLITUDE_STEP9,
    SC_AMPLITUDE_STEP10,
    SC_COOLING_VALVE,
    SC_GLOBAL_SUSPECT,
    SC_GLOBAL_REJECT,
    SC_CONTROL_LIMIT,
    SC_CL_PEAKPOWER_CUTOFF,
    SC_CL_PEAKPOWER_CUTOFF_VAL,
    SC_CL_ABSOLUTE_CUTOFF,
    SC_CL_ABSOLUTE_CUTOFF_VAL,
    SC_CL_COLLAPSE_CUTOFF,
    SC_CL_COLLAPSE_CUTOFF_VAL,
    SC_CL_TIME_CUTOFF,
    SC_CL_TIME_CUTOFF_VAL,
    SC_CL_FREQ_LOW,
    SC_CL_FREQ_LOW_VAL,
    SC_CL_FREQ_HIGH,
    SC_CL_FREQ_HIGH_VAL,
    SC_CL_ENERGY_CUTOFF,
    SC_CL_ENERGY_CUTOFF_VAL,
    SC_CL_GROUND_DETECT_CUTOFF,
    SC_PRETRIGGER,
    SC_AUTO_PRETRIGGER,
    SC_DISTANCE_PRETRIGGER,
    SC_PRETRIGGER_AMPLITUDE,
    SC_PRETRIGGER_DISTANCE,
    SC_AFTER_BURST,
    SC_AB_AMPLITUDE,
    SC_AB_DELAY,
    SC_AB_TIME,
    SC_ENERGY_BRAKE,
    SC_ENERGY_BRAKE_AMPLITUDE,
    SC_ENERGY_BRAKE_TIME,
    SC_SUSPECT_TIME_ENABLED,
    SC_SUSPECT_TIME_LOW_ENABLED,
    SC_SUSPECT_TIME_HIGH_ENABLED,
    SC_SUSPECT_TIME_LOW,
    SC_SUSPECT_TIME_HIGH,
    SC_SUSPECT_ENERGY_ENABLED,
    SC_SUSPECT_ENERGY_LOW_ENABLED,
    SC_SUSPECT_ENERGY_HIGH_ENABLED,
    SC_SUSPECT_ENERGY_LOW,
    SC_SUSPECT_ENERGY_HIGH,
    SC_SUSPECT_POWER_ENABLED,
    SC_SUSPECT_POWER_LOW_ENABLED,
    SC_SUSPECT_POWER_HIGH_ENABLED,
    SC_SUSPECT_POWER_LOW,
    SC_SUSPECT_POWER_HIGH,
    SC_SUSPECT_CD_ENABLED,
    SC_SUSPECT_CD_LOW_ENABLED,
    SC_SUSPECT_CD_HIGH_ENABLED,
    SC_SUSPECT_CD_LOW,
    SC_SUSPECT_CD_HIGH,
    SC_SUSPECT_AD_ENABLED,
    SC_SUSPECT_AD_LOW_ENABLED,
    SC_SUSPECT_AD_HIGH_ENABLED,
    SC_SUSPECT_AD_LOW,
    SC_SUSPECT_AD_HIGH,
    SC_SUSPECT_TD_ENABLED,
    SC_SUSPECT_TD_LOW_ENABLED,
    SC_SUSPECT_TD_HIGH_ENABLED,
    SC_SUSPECT_TD_LOW,
    SC_SUSPECT_TD_HIGH,
    SC_SUSPECT_END_FORCE_ENABLED,
    SC_SUSPECT_END_FORCE_LOW_ENABLED,
    SC_SUSPECT_END_FORCE_HIGH_ENABLED,
    SC_SUSPECT_END_FORCE_LOW,
    SC_SUSPECT_END_FORCE_HIGH,
    SC_SUSPECT_FREQ_ENABLED,
    SC_SUSPECT_FREQ_LOW_ENABLED,
    SC_SUSPECT_FREQ_HIGH_ENABLED,
    SC_SUSPECT_FREQ_LOW,
    SC_SUSPECT_FREQ_HIGH,
    SC_REJECT_TIME_ENABLED,
    SC_REJECT_TIME_LOW_ENABLED,
    SC_REJECT_TIME_HIGH_ENABLED,
    SC_REJECT_TIME_LOW,
    SC_REJECT_TIME_HIGH,
    SC_REJECT_ENERGY_ENABLED,
    SC_REJECT_ENERGY_LOW_ENABLED,
    SC_REJECT_ENERGY_HIGH_ENABLED,
    SC_REJECT_ENERGY_LOW,
    SC_REJECT_ENERGY_HIGH,
    SC_REJECT_POWER_ENABLED,
    SC_REJECT_POWER_LOW_ENABLED,
    SC_REJECT_POWER_HIGH_ENABLED,
    SC_REJECT_POWER_LOW,
    SC_REJECT_POWER_HIGH,
    SC_REJECT_CD_ENABLED,
    SC_REJECT_CD_LOW_ENABLED,
    SC_REJECT_CD_HIGH_ENABLED,
    SC_REJECT_CD_LOW,
    SC_REJECT_CD_HIGH,
    SC_REJECT_AD_ENABLED,
    SC_REJECT_AD_LOW_ENABLED,
    SC_REJECT_AD_HIGH_ENABLED,
    SC_REJECT_AD_LOW,
    SC_REJECT_AD_HIGH,
    SC_REJECT_TD_ENABLED,
    SC_REJECT_TD_LOW_ENABLED,
    SC_REJECT_TD_HIGH_ENABLED,
    SC_REJECT_TD_LOW,
    SC_REJECT_TD_HIGH,
    SC_REJECT_END_FORCE_ENABLED,
    SC_REJECT_END_FORCE_LOW_ENABLED,
    SC_REJECT_END_FORCE_HIGH_ENABLED,
    SC_REJECT_END_FORCE_LOW,
    SC_REJECT_END_FORCE_HIGH,
    SC_REJECT_FREQ_ENABLED,
    SC_REJECT_FREQ_LOW_ENABLED,
    SC_REJECT_FREQ_HIGH_ENABLED,
    SC_REJECT_FREQ_LOW,
    SC_REJECT_FREQ_HIGH,
    SC_TIMED_SEEK_PERIOD,
    SC_TIMED_SEEK,
    SC_PRE_WELD_SEEK,
    SC_POST_WELD_SEEK,
    SC_USER,
    SC_STACK_SERIAL_NO,

    //PC
    PC_AMPLITUDE_LOOPC1,
    PC_AMPLITUDE_LOOPC2,
    PC_PHASE_LOOP,
    PC_PHASE_LOOPCF,
    PC_FREQUENCY_LOW,
    PC_FREQUENCY_HIGH,
    PC_PHASE_LIMIT_TIME,
    PC_PHASE_LIMIT ,
    PC_CONTROL_MODE,
    PC_BLIND_TIME_SEEK,
    PC_BLIND_TIME_WELD,
    PC_DDS_SWITCH_TIME,
    PC_PARAMETER7 ,
    PC_PARAMETER8,
    PC_PARAMETER9,
    PC_PARAMETER10,
    PC_PARAMETER11,
    PC_PARAMETER12,
    PC_PARAMETER13,
    PC_PARAMETER14,
    PC_PARAMETER15,
    PC_F_LIMIT_TIME,
    PC_AMP_PROPORTIONAL_GAIN,
    PC_AMP_INTEGRAL_GAIN,
    PC_PHASE_INTEGRAL_GAIN,
    PC_PHASE_PROPORTIONAL_GAIN,
    PC_FREQUENCY_WINDOW_SIZE,
    PC_PF_OF_PHASE_CONTROL_LOOP,
    PC_PI_FREQUENCY_LOW,
    PC_PI_FREQUENCY_HIGH,
    PC_PI_PHASE_LIMIT_TIME,
    PC_PI_PHASE_LIMIT,
    PC_WELD_RAMP_TIME,
    PC_START_FREQUENCY,
    PC_MEMORY_OFFSET,
    PC_DIGITAL_TUNE,
    PC_FREQUENCY_OFFSET,

    //AC
    AC_WELD_FORCE,
    AC_FORCE_RAMP_TIME,
    AC_HOLD_MODE,
    AC_TOTAL_COLLAPSE_TARGET,
    AC_HOLD_FORCE,
    AC_HOLD_FORCE_RAMP_TIME,
    AC_EXPECTED_PART_CONTACT_POSITION,
    AC_READY_POSITION,
    AC_DOWN_ACCELERATION,
    AC_DOWN_MAX_VELOCITY,
    AC_DOWN_DECELERATION,
    AC_RETURN_ACCELERATION,
    AC_RETURN_MAX_VELOCITY,
    AC_RETURN_DECELERATION,
    AC_PART_CONTANCT_OFFSET,
    AC_PART_CONTANCT_WINDOW_MINUS,
    AC_PART_CONTANCT_WINDOW_PLUS,
    AC_FORCE_STEP_RAMP_VALUE1,
    AC_FORCE_STEP_RAMP_VALUE2,
    AC_FORCE_STEP_RAMP_VALUE3,
    AC_FORCE_STEP_RAMP_VALUE4,
    AC_FORCE_STEP_RAMP_VALUE5,
    AC_FORCE_STEP_RAMP_VALUE6,
    AC_FORCE_STEP_RAMP_VALUE7,
    AC_FORCE_STEP_RAMP_VALUE8,
    AC_FORCE_STEP_RAMP_VALUE9,
    AC_FORCE_STEP_RAMP_VALUE10,
    AC_FORCE_STEP1,
    AC_FORCE_STEP2,
    AC_FORCE_STEP3,
    AC_FORCE_STEP4,
    AC_FORCE_STEP5,
    AC_FORCE_STEP6,
    AC_FORCE_STEP7,
    AC_FORCE_STEP8,
    AC_FORCE_STEP9,
    AC_FORCE_STEP10,
    AC_READY_POSITION_TOGGLE,
    AC_WELD_FORCE_CONTROL,

    SC_MODE_VALUE_TIME,
    SC_MODE_VALUE_ENERGY,
    SC_MODE_VALUE_POWER,
    SC_MODE_VALUE_AD,
    SC_MODE_VALUE_CD,
    SC_MODE_VALUE_ST,
    SC_MODE_VALUE_GD,

    UI_RECIPE_NAME,
    UI_ISACTIVE,
    UI_ISLOCK,
    UI_COMPANYNAME,
    UI_ISVALID,
    UI_ISNEW_RECIPE,
    UI_IS_MODIFIED,
    UI_RECIPE_VERSION_NUMBER,
    UI_GLOBAL_SETUP,
    UI_SETUP_WELD_MODE_STATUS,
    UI_SETUP_WELD_MODE_LOW,
    UI_SETUP_WELD_MODE_HIGH,
    UI_WELD_AMP_STATUS,
    UI_WELD_AMP_LOW,
    UI_WELD_AMP_HIGH,
    UI_WELD_FORCE_STATUS,
    UI_WELD_FORCE_LOW,
    UI_WELD_FORCE_HIGH,
    UI_TRIGGER_FORCE_STATUS,
    UI_TRIGGER_FORCE_LOW,
    UI_TRIGGER_FORCE_HIGH,
    UI_HOLD_FORCE_STATUS,
    UI_HOLD_FORCE_LOW,
    UI_HOLD_FORCE_HIGH,
    UI_HOLD_TIME_STATUS,
    UI_HOLD_TIME_LOW,
    UI_HOLD_TIME_HIGH,
    UI_SCRUB_AMP_STATUS,
    UI_SCRUB_AMP_LOW,
    UI_SCRUB_AMP_HIGH,
    UI_SCRUB_TIME_STATUS,
    UI_SCRUB_TIME_LOW,
    UI_SCRUB_TIME_HIGH,
    SC_MODE_VALUE_DY,
    SC_MODE_VALUE_DENSITY,
    SC_REACTIVITY,
    SC_FORCE_LEVEL_ENABLED,
    SC_FORCE_LEVEL,
    SC_FORCE_LEVEL_TIME,
    UI_REACTIVITY_STATUS,
    UI_REACTIVITY_LOW,
    UI_REACTIVITY_HIGH,
    UI_FORCE_LEVEL_STATUS,
    UI_FORCE_LEVEL_LOW,
    UI_FORCE_LEVEL_HIGH,
    SC_SCRUB_AMPLITUDE,
    LAST_PARAM
};

struct stSetupLimits
{
    bool isGlobalSetup;
    int dwWeldModeStatus;
    int dwWeldModeLow;
    int dwWeldModeHigh;
    int dwWeldAmpStatus;
    int dwWeldAmpLow;
    int dwWeldAmpHigh;
    int dwWeldForceStatus;
    int dwWeldForceLow;
    int dwWeldForceHigh;
    int dwTriggerForceStatus;
    int dwTriggerForceLow;
    int dwTriggerForceHigh;
    int dwHoldForceStatus;
    int dwHoldForceLow;
    int dwHoldForceHigh;
    int dwHoldTimeStatus;
    int dwHoldTimeLow;
    int dwHoldTimeHigh;
    int dwScrubAmpStatus;
    int dwScrubAmpLow;
    int dwScrubAmpHigh;
    int dwScrubTimeStatus;
    int dwScrubTimeLow;
    int dwScrubTimeHigh;
    int dwReactivityStatus;
    int dwReactivityLow;
    int dwReactivityHigh;
    int dwForceLevelStatus;
    int dwForceLevelLow;
    int dwForceLevelHigh;
};

enum StepAtValues
{
	STEP_AT_TIME = 1,
	STEP_AT_ENERGY,
	STEP_AT_POWER,
	STEP_AT_ABSOLUTE_DISTANCE,
	STEP_AT_COLLAPSE_DISTANCE,
	STEP_AT_EXTERNAL_SIGNAL,
};
struct WeldRecipe_UI
{
    char RecipeName[16];
    bool isActive;
    bool isValid;
    bool isLock;
    bool isNewRecipe;
    bool isModified;
    char CompanyName[32];
    stSetupLimits SetupLimitsst;
    int dwRecipeVerNumber;
};

struct WeldRecipe
{
    WeldRecipe_SC m_weldRecipe_SC;
    WeldRecipe_PC_AC m_weldRecipe_PC_AC;
    WeldRecipe_UI m_weldRecipe_UI;
};

struct stRecipeParmInfo
{
    int ParamIndexInStructure;
    QString ParamName;
    QString ParamDefaultValue[3];
    QString ParamMinValue[3];
    QString ParamMaxValue[3];
    QString ParamUnit[2];
};

struct stSuspectRejectParamInfo
{
    int ParameterIndexInStructure;
    QString RejectMin;
    QString SuspectMin;
    QString SuspectMax;
    QString RejectMax;
    bool suspectStatus = false;
    bool rejectStatus = false;
};

struct ModeInfo
{
    QString ModeName;
    QString ModeLocaleName;
    QString ModeValue;
    QString ModeUnit;
    QString ModeIndex;
    QString ModeNameScreen;
};

enum SystemFreq
{
    SYSTEM_TYPE_20KHZ =0,
    SYSTEM_TYPE_30KHZ,
    SYSTEM_TYPE_40KHZ
};

enum ParamUnits
{
    UNIT_SI = 0,
    UNIT_SAE
};

enum RecipeIndex
{
    ACTUAL_RECIPE = 0,
    ACTUATOR_FORCE_RECIPE
};

enum RecipeParamEditedFlags
{
   SEND_ALL_PARAM = 0,
   SEND_WELD_PARAM,
   SEND_SUSPECT_REJECT_LIMITS_PARAM,
   SEND_SETUP_LIMITS_PARAM,
   SEND_BRANSON_PARAM
};

enum RECIPE_PARSE_ERROR_CODE
{
    RANGE_ERROR     = 0x01,
    STRING_ERROR    = 0x02, //messy code detection, such as Company Name
    DATA_ERROR      = 0x04,
    SIZE_ERROR      = 0x08,
};

class WeldRecipeParameter
{
private:
    ParamUnits m_UnitSelected;
    SystemFreq m_CurrentSystemFreq;
    WeldRecipe m_weldRecipe;
    WeldRecipe m_weldActuatorForceRecipe;
    bool m_IsActiveRequired;
    QMap<QString,stRecipeParmInfo> m_MapRecipeParams;
    QMap<QString,stSuspectRejectParamInfo> m_MapSuspectRejectParams;

    void addNewParmToMap(QString qstrMetricUnit, QString qstrImperialUnit,
                         QString qstr20KDefaultValue, QString qstr20KMinValue, QString qstr20KMaxValue,
                         QString qstr30KDefaultValue, QString qstr30KMinValue, QString qstr30KMaxValue,
                         QString qstr40KDefaultValue, QString qstr40KMinValue, QString qstr40KMaxValue,
                         QString qstrParamName, int dwIndex);
    void initializeSuspectRejectLimits();
    void getDefaultFrequencySelection(int tmpRecipeNum);

    QString prepareWeldRecipeStringToDB();
    QString prepareSuspectRejectStringToDB();
    QString prepareBransonOnlyParamsStringToDB();
    QString prepareSetupLimitsStringToDB();
    QString prepareStackRecipeStringToDB();

    void readWeldRecipeParameterFromDB(QString a_RecivedBuffer);

    int parseWeldRecipeParameterFromDB(QString strRecivedBuffer);
    int parseWeldRecipeGeneralDataFromDB(QStringList query);
    void parseWeldRecipeSuspectRejectDataFromDB(QStringList query);
    int parseWeldRecipeBransonOnlyDataFromDB(QStringList query);
    void parseWeldRecipeSetupDataFromDB(QStringList query);
    int parseWeldRecipeStackDataFromDB(QStringList query);
    int parseWeldRecipeCompanyName();
    int parseWeldRecipeSuspectRejectUpperLimit();

    int getDefaultVariableValue(QString paramEditText, QString qstrIndexValue);

    double convertSecToMsec(double);
    double convertMsecToSec(double);
    double convertMillimetreToMicrometre(double);

    double convertinchestomicrometre(double);
    double convertinchestomillimetre(double);
    double convertmicrometretoinches(double);
    double convertmillimetretoinches(double);
    QString getFirstRuntimeIndex();

public:
    WeldRecipeParameter();
    int m_oldRecipeNumber;
    int m_ParamaterChangedFlag;
    bool bIsWeldForceChanged;
    double convertMicrometreToMillimetre(double);

    void setlimitsFlagsForBL(WeldRecipe_SC &stSC_Temp);
    void initializeRecipeParamaterStructure(int tmpRecipeNum);
    void createRecipeParamaterStructure();
    int CheckSetupRange(QString qstrParamName,double qstrParamValue);
    int CheckSuspectRejectRange(QString qstrParamName,double qstrParamValue);
    int CheckControlLimits(QString qstrParamName,double qstrParamValue);
    stRecipeParmInfo getStructureByName(QString paramEditText);
    QString getStructureValueByName(QString paramEditText);
    QString getStructureValueByIndex(int);
    void setStructureVariableByName(QString ,QString );
    void setStructureVariableByIndex(int,QString);
    void setStructureVariableByIndex(int,int);

    ModeInfo getModeInfoByIndex(int dwModeIndex,QString qstrModeValue);
    ModeInfo getModeInfoByName(QString qstrModeName);
    void setModeInfoByName(QString qstrModeName);

    QString rangeValidation(QString,QString);
    QString rangeSetupLimitValidation(QString keypadTextName,QString keypadValue);
    QString getDefaultValue(QString);
    QString getMinValue(QString qstrParamName);
    QString getMaxValue(QString qstrParamName);
    void setMaxValue(QString qstrParamName,int qstrParamVal);
    void sendRecipeToSC();
    void sendStructureToSC(int dwStructureIndex);
    void forceSettingIndependent(int qstrParamValue);
    void copyRecipeStructToForceStruct();
    QString getActuatorRecipeStructureValue();
    void storeRecipeToDB(bool bIsNewRecipe = false );
    Q_INVOKABLE int readRecipeFromDB(QString qstrRecipeNumber);
    void setMetrixUnit(int dwMetrixUnit);
    void setPsFreqValue(int dwPsFreq);
    void setIsActiveRequired(bool bIsActiveReq);
    bool getIsActiveRequired();

    Q_INVOKABLE void setParamaterChangedFlag(int ParamaterChangedFlag);
};

#endif // WELDRECIPEPARAMETERDATA_H
