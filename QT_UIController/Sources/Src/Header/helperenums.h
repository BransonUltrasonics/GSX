/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Helper Enums

***************************************************************************/

#ifndef HELPERENUMS_H
#define HELPERENUMS_H

#include <QObject>
#include <QQmlEngine>

#define HELPERENUMS_URI_NAME "com.branson.helperenums"
#define HELPERENUMS_QML_MAJOR_VERSION 1
#define HELPERENUMS_QML_MINIOR_VERSION 0
#define HELPERENUMS_QML_NAME "HelperEnums"

#define SCSTATEENUMS_URI_NAME "com.branson.scstate"
#define SCSTATEENUMS_QML_MAJOR_VERSION 1
#define SCSTATEENUMS_QML_MINIOR_VERSION 0
#define SCSTATEENUMS_QML_NAME "ScStateEnums"

#define OPENEDSCREEN_URI_NAME "com.branson.openedscreen"
#define OPENEDSCREEN_QML_MAJOR_VERSION 1
#define OPENEDSCREEN_QML_MINIOR_VERSION 0
#define OPENEDSCREEN_QML_NAME "OpenedScreenEnums"

#define FEATURES_URI_NAME "com.branson.menu"
#define FEATURES_QML_MAJOR_VERSION 1
#define FEATURES_QML_MINIOR_VERSION 0
#define FEATURES_QML_NAME "Features"

#define EVENTLOGGING_URI_NAME "com.branson.eventlogging"
#define EVENTLOGGING_QML_MAJOR_VERSION 1
#define EVENTLOGGING_QML_MINIOR_VERSION 0
#define EVENTLOGGING_QML_NAME "EventLogging"

#define RUNTIMEFEATURES_URI_NAME "com.branson.runtimefeatures"
#define RUNTIMEFEATURES_QML_MAJOR_VERSION 1
#define RUNTIMEFEATURES_QML_MINIOR_VERSION 0
#define RUNTIMEFEATURES_QML_NAME "RuntimeFeatures"

#define WS_RESTRICTIONSCREEN_URI_NAME "com.branson.WSrestrictionscreen"
#define WS_RESTRICTIONSCREEN_QML_MAJOR_VERSION 1
#define WS_RESTRICTIONSCREEN_QML_MINIOR_VERSION 0
#define WS_RESTRICTIONSCREEN_QML_NAME "WSRestrictionScreen"


#define DELETERECIPERESPONSE_URI_NAME "com.branson.deleteRecipeResponse"
#define DELETERECIPERESPONSE_QML_MAJOR_VERSION 1
#define DELETERECIPERESPONSE_QML_MINOR_VERSION 0
#define DELETERECIPERESPONSE_QML_NAME "DeleteRecipeResponse"



class HelperEnums : public QObject
{
    Q_OBJECT
    Q_ENUMS(UserLevel)

public:
    enum class UserLevel
    {
        NO_LEVEL = 0,
        EXECUTIVE,
        SUPERVISOR,
        TECHNICIAN,
        OPERATOR
    };

    static void registerQmlType()
    {
        qmlRegisterType<HelperEnums>(HELPERENUMS_URI_NAME, HELPERENUMS_QML_MAJOR_VERSION, HELPERENUMS_QML_MINIOR_VERSION, HELPERENUMS_QML_NAME);
    }
};

class SCStateEnums : public QObject
{
    Q_OBJECT
    Q_ENUMS(SCState)

public:
    enum class SCState
    {
        READY = 1,
        ALARM = 7
    };

    static void registerQmlType()
    {
        qmlRegisterType<SCStateEnums>(SCSTATEENUMS_URI_NAME, SCSTATEENUMS_QML_MAJOR_VERSION, SCSTATEENUMS_QML_MINIOR_VERSION, SCSTATEENUMS_QML_NAME);
    }
};

class OpenedScreenEnums : public QObject
{
    Q_OBJECT
    Q_ENUMS(OpenedScreen)

public:
    enum class OpenedScreen
    {
        NONE = 0,
        DASHBORAD,
        PRODUTCION,
        RECIPE,
        WELDGRAPH,
        ANALYTICS,
        SYSTEM,
        SPLASH_SCREEN,
        LOGIN_SCREEN
    };

    static void registerQmlType()
    {
        qmlRegisterType<OpenedScreenEnums>(OPENEDSCREEN_URI_NAME, OPENEDSCREEN_QML_MAJOR_VERSION, OPENEDSCREEN_QML_MINIOR_VERSION, OPENEDSCREEN_QML_NAME);
    }
};

class Features : public QObject
{
    Q_OBJECT
    Q_ENUMS(FeaturesEnum)

public:
    enum FeaturesEnum
    {
        RUN_VALIDATED_WELD_RECIPE,
        RUN_UNVALIDATED_WELD_RECIPE,
        RESET_ALARAMS,
        SET_AS_ACTIVE,
        RESET_BATCH_COUNTER,
        QUICK_CALIBRATION,
        BARCODE_RECIPE_RECALL,
        WELD_SETUP,
        HORN_DOWN,
        DIAGONISTICS,
        MODIFY_WELD_PARAMS_WITH_MIN_MAX,
        SYSTEM_CONFIGURATION,
        VALIDATE_AND_LOCK_WELD_RECIPE,
        MODIFY_LOCKED_WELD_RECIPE,
        ADD_OR_MODIFY_USER_ID_SETUP,
        SET_AUTHORITY_CHECK_TO_YES,
        GENERAL,
        USER_LIST,
        USER_IO,
        USER_AUTHORITY,
        CHANGE_PASSWORD,
        ALARAM_MANAGEMENT,
        IP_CONFIGURATION,
        EXTERNAL_STATUS,
        COMPONENTS,
        BRANSON,
        CLEAR_MEMORY,
        SCAN_SEEK_TEST,
        SYSTEM_DATA,
        SET_MEMORY_FULL_ACTION_TO_YES,
        BATCH_SETUP,
        SYSTEM_CLOCK,
        NEW_SAVE_COPY_VALIDATED_UNVALIDATED_RECIPE,
        RESET_WELD_COUNTER,
        SECURITY
    };

    static void registerQmlType()
    {
        qmlRegisterType<Features>(FEATURES_URI_NAME, FEATURES_QML_MAJOR_VERSION, FEATURES_QML_MINIOR_VERSION, FEATURES_QML_NAME);
    }
};

class EventLogging : public QObject
{
    Q_OBJECT
    Q_ENUMS(EventLoggingEnum)

public:
    enum class EventLoggingEnum
    {
        UI_EVENT_USER_LOGIN = 1,
        UI_EVENT_USER_LOGOUT,
        UI_EVENT_HORN_SCAN_COMPLETED,
        UI_EVENT_HORN_SCAN_ABORTED,
        UI_EVENT_USER_CREATED,
        UI_EVENT_USER_MODIFIED,
        UI_EVENT_AUTHORITY_CHECK_CHANGED,
        UI_EVENT_RECIPE_CREATED,
        UI_EVENT_RECIPE_MODIFIED,
        UI_EVENT_SEEK_SCAN_STARTED,
        UI_EVENT_CYCLE_COUNT_RESETTED,
        UI_EVENT_BOARD_REPLACED,
        UI_EVENT_SYSTEM_CLEANED,
        UI_EVENT_SYSTEM_COMPONENT_FACTORY_RESET,
        UI_EVENT_RECIPE_VALIDATED,
        UI_EVENT_RECIPE_INVALIDATED,
        UI_EVENT_DATE_CHANGED,
        UI_EVENT_PASSWORD_CHANGED,
        UI_EVENT_RECIPE_COPIED,
        UI_EVENT_VALIDATED_CHANGE,
        UI_EVENT_SYSTEM_CALIBRATION,
        UI_EVENT_SYSTEM_HW_CHANGED,
        UI_EVENT_UIC_SOFTWARE_VERSION,
        UI_EVENT_RECIPE_SAVED,
        UI_EVENT_USER_LOCKED,
        UI_EVENT_DELETE_RECIPE
    };
    static void registerQmlType()
    {
        qmlRegisterType<EventLogging>(EVENTLOGGING_URI_NAME, EVENTLOGGING_QML_MAJOR_VERSION, EVENTLOGGING_QML_MINIOR_VERSION, EVENTLOGGING_QML_NAME);
    }
};

class RuntimeFeatures : public QObject
{
    Q_OBJECT
    Q_ENUMS(RuntimeFeaturesEnum)

public:
    enum class RuntimeFeaturesEnum
    {
        UI_MULTI_MODE = 1,
        UI_MODE_TIME,
        UI_MODE_ENERGY,
        UI_MODE_POWER,
        UI_MODE_COLLAPSE_DISTANCE,
        UI_MODE_ABSOLUTE_DISTANCE,
        UI_MODE_GROUND_DETECT,
        UI_MODE_CONTINOUS,
        UI_PRE_TRIGGER,
        UI_AFTER_BURST,
        UI_ENERGY_BRAKE,
        UI_AMPLITUDE_STEP,
        UI_FORCE_STEP,
        UI_ALARM_LOG,
        UI_EVENT_LOG,
        UI_WELD_LOG,
        UI_REPORTING,
        UI_CRACKED_HORN,
        UI_USB,
        UI_BINARY,
        UI_WEB_SERVICES,
        UI_OPC_UA,
        UI_USER_IO,
        UI_ACTUATOR_IO,
        UI_CYCLE_COUNTER,
        UI_BATCH_COUNTER,
        UI_POWER_MATCH_CURVE,
        UI_SMART_CONVERTER,
        UI_CONTROL_SPEED,
        UI_SEQUENCING,
        UI_AUTO_CONVERTER_COOLING,
        UI_FIELD_BUS,
        UI_DASHBOARD_SCREEN,
        UI_PASSWORD_SCREEN,
        UI_MOBILE,
        UI_OVERLAY,
        UI_TRENDS,
        UI_OPTITUNE,
        UI_LIMITS,
        UI_HD_MODE,
        UI_TEACH_MODE,
        UI_ADV_COMM,
        UI_EXTRA_DATA_STORAGE,
        UI_TOUBLE_SHOOT,
        UI_CFR,
        UI_HOUR_COUNTER,
        UI_MODEL_ID_1,
        UI_MODEL_ID_2,
        UI_MODEL_ID_3,
        UI_MODEL_ID_4
    };

    static void registerQmlType()
    {
        qmlRegisterType<RuntimeFeatures>(RUNTIMEFEATURES_URI_NAME, RUNTIMEFEATURES_QML_MAJOR_VERSION, RUNTIMEFEATURES_QML_MINIOR_VERSION, RUNTIMEFEATURES_QML_NAME);
    }
};

class WSRestrictionScreen : public QObject
{
    Q_OBJECT
    Q_ENUMS(WSRestrictionScreenEnum)

public:
    enum class WSRestrictionScreenEnum
    {
        NO_RESTRICTED = 0,
        GENERAL_RESTRICTED,
        USER_MANAGEMENT,
        USER_OPERATOR_AUTHORITY,
        CHANGE_PASSWORD
    };

    static void registerQmlType()
    {
        qmlRegisterType<WSRestrictionScreen>(WS_RESTRICTIONSCREEN_URI_NAME, WS_RESTRICTIONSCREEN_QML_MAJOR_VERSION, WS_RESTRICTIONSCREEN_QML_MINIOR_VERSION, WS_RESTRICTIONSCREEN_QML_NAME);
    }
};


class DeleteRecipeResponse: public QObject{
    Q_OBJECT
    Q_ENUMS(DeleteRecipeResponseEnum)
public:
    enum DeleteRecipeResponseEnum{
        NOT_RECEIVE_DATA = -1,                 /* Cannot recieve the data from SC */
        DELETE_RECIPE_SUCCESS = 0,             /* Recipe is Deleted Succesfully */
        INVALID_RECIPE_NUMBER,                 /* Recipe number is invalid */
        DELETE_RECIPE_FAILED_ACTIVE_RECIPE,    /* Delete recipe failed because its a ACTIVE RECIPE */
        DELETE_RECIPE_FAILED,                  /* Failed to delete recipe from Database */
        RECIPE_NOT_PRESENT,                    /* Recipe is not present in Database */
        END_FIAL_DELETE                        /* Fail to delete recipe */
    };
    static void registerQmlType(){
        qmlRegisterType<DeleteRecipeResponse>(DELETERECIPERESPONSE_URI_NAME, DELETERECIPERESPONSE_QML_MAJOR_VERSION, DELETERECIPERESPONSE_QML_MINOR_VERSION
                                              ,DELETERECIPERESPONSE_QML_NAME);
    }
};


#endif // HELPERENUMS_H
