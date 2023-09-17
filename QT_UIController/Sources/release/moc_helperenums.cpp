/****************************************************************************
** Meta object code from reading C++ file 'helperenums.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/helperenums.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'helperenums.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HelperEnums_t {
    QByteArrayData data[7];
    char stringdata0[72];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HelperEnums_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HelperEnums_t qt_meta_stringdata_HelperEnums = {
    {
QT_MOC_LITERAL(0, 0, 11), // "HelperEnums"
QT_MOC_LITERAL(1, 12, 9), // "UserLevel"
QT_MOC_LITERAL(2, 22, 8), // "NO_LEVEL"
QT_MOC_LITERAL(3, 31, 9), // "EXECUTIVE"
QT_MOC_LITERAL(4, 41, 10), // "SUPERVISOR"
QT_MOC_LITERAL(5, 52, 10), // "TECHNICIAN"
QT_MOC_LITERAL(6, 63, 8) // "OPERATOR"

    },
    "HelperEnums\0UserLevel\0NO_LEVEL\0EXECUTIVE\0"
    "SUPERVISOR\0TECHNICIAN\0OPERATOR"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HelperEnums[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x2,    5,   18,

 // enum data: key, value
       2, uint(HelperEnums::UserLevel::NO_LEVEL),
       3, uint(HelperEnums::UserLevel::EXECUTIVE),
       4, uint(HelperEnums::UserLevel::SUPERVISOR),
       5, uint(HelperEnums::UserLevel::TECHNICIAN),
       6, uint(HelperEnums::UserLevel::OPERATOR),

       0        // eod
};

void HelperEnums::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject HelperEnums::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HelperEnums.data,
      qt_meta_data_HelperEnums,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HelperEnums::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HelperEnums::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HelperEnums.stringdata0))
        return static_cast<void*>(const_cast< HelperEnums*>(this));
    return QObject::qt_metacast(_clname);
}

int HelperEnums::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_SCStateEnums_t {
    QByteArrayData data[4];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SCStateEnums_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SCStateEnums_t qt_meta_stringdata_SCStateEnums = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SCStateEnums"
QT_MOC_LITERAL(1, 13, 7), // "SCState"
QT_MOC_LITERAL(2, 21, 5), // "READY"
QT_MOC_LITERAL(3, 27, 5) // "ALARM"

    },
    "SCStateEnums\0SCState\0READY\0ALARM"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SCStateEnums[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x2,    2,   18,

 // enum data: key, value
       2, uint(SCStateEnums::SCState::READY),
       3, uint(SCStateEnums::SCState::ALARM),

       0        // eod
};

void SCStateEnums::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject SCStateEnums::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SCStateEnums.data,
      qt_meta_data_SCStateEnums,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SCStateEnums::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SCStateEnums::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SCStateEnums.stringdata0))
        return static_cast<void*>(const_cast< SCStateEnums*>(this));
    return QObject::qt_metacast(_clname);
}

int SCStateEnums::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_OpenedScreenEnums_t {
    QByteArrayData data[11];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OpenedScreenEnums_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OpenedScreenEnums_t qt_meta_stringdata_OpenedScreenEnums = {
    {
QT_MOC_LITERAL(0, 0, 17), // "OpenedScreenEnums"
QT_MOC_LITERAL(1, 18, 12), // "OpenedScreen"
QT_MOC_LITERAL(2, 31, 4), // "NONE"
QT_MOC_LITERAL(3, 36, 9), // "DASHBORAD"
QT_MOC_LITERAL(4, 46, 10), // "PRODUTCION"
QT_MOC_LITERAL(5, 57, 6), // "RECIPE"
QT_MOC_LITERAL(6, 64, 9), // "WELDGRAPH"
QT_MOC_LITERAL(7, 74, 9), // "ANALYTICS"
QT_MOC_LITERAL(8, 84, 6), // "SYSTEM"
QT_MOC_LITERAL(9, 91, 13), // "SPLASH_SCREEN"
QT_MOC_LITERAL(10, 105, 12) // "LOGIN_SCREEN"

    },
    "OpenedScreenEnums\0OpenedScreen\0NONE\0"
    "DASHBORAD\0PRODUTCION\0RECIPE\0WELDGRAPH\0"
    "ANALYTICS\0SYSTEM\0SPLASH_SCREEN\0"
    "LOGIN_SCREEN"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OpenedScreenEnums[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x2,    9,   18,

 // enum data: key, value
       2, uint(OpenedScreenEnums::OpenedScreen::NONE),
       3, uint(OpenedScreenEnums::OpenedScreen::DASHBORAD),
       4, uint(OpenedScreenEnums::OpenedScreen::PRODUTCION),
       5, uint(OpenedScreenEnums::OpenedScreen::RECIPE),
       6, uint(OpenedScreenEnums::OpenedScreen::WELDGRAPH),
       7, uint(OpenedScreenEnums::OpenedScreen::ANALYTICS),
       8, uint(OpenedScreenEnums::OpenedScreen::SYSTEM),
       9, uint(OpenedScreenEnums::OpenedScreen::SPLASH_SCREEN),
      10, uint(OpenedScreenEnums::OpenedScreen::LOGIN_SCREEN),

       0        // eod
};

void OpenedScreenEnums::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject OpenedScreenEnums::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OpenedScreenEnums.data,
      qt_meta_data_OpenedScreenEnums,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *OpenedScreenEnums::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenedScreenEnums::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OpenedScreenEnums.stringdata0))
        return static_cast<void*>(const_cast< OpenedScreenEnums*>(this));
    return QObject::qt_metacast(_clname);
}

int OpenedScreenEnums::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_Features_t {
    QByteArrayData data[37];
    char stringdata0[655];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Features_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Features_t qt_meta_stringdata_Features = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Features"
QT_MOC_LITERAL(1, 9, 12), // "FeaturesEnum"
QT_MOC_LITERAL(2, 22, 25), // "RUN_VALIDATED_WELD_RECIPE"
QT_MOC_LITERAL(3, 48, 27), // "RUN_UNVALIDATED_WELD_RECIPE"
QT_MOC_LITERAL(4, 76, 13), // "RESET_ALARAMS"
QT_MOC_LITERAL(5, 90, 13), // "SET_AS_ACTIVE"
QT_MOC_LITERAL(6, 104, 19), // "RESET_BATCH_COUNTER"
QT_MOC_LITERAL(7, 124, 17), // "QUICK_CALIBRATION"
QT_MOC_LITERAL(8, 142, 21), // "BARCODE_RECIPE_RECALL"
QT_MOC_LITERAL(9, 164, 10), // "WELD_SETUP"
QT_MOC_LITERAL(10, 175, 9), // "HORN_DOWN"
QT_MOC_LITERAL(11, 185, 12), // "DIAGONISTICS"
QT_MOC_LITERAL(12, 198, 31), // "MODIFY_WELD_PARAMS_WITH_MIN_MAX"
QT_MOC_LITERAL(13, 230, 20), // "SYSTEM_CONFIGURATION"
QT_MOC_LITERAL(14, 251, 29), // "VALIDATE_AND_LOCK_WELD_RECIPE"
QT_MOC_LITERAL(15, 281, 25), // "MODIFY_LOCKED_WELD_RECIPE"
QT_MOC_LITERAL(16, 307, 27), // "ADD_OR_MODIFY_USER_ID_SETUP"
QT_MOC_LITERAL(17, 335, 26), // "SET_AUTHORITY_CHECK_TO_YES"
QT_MOC_LITERAL(18, 362, 7), // "GENERAL"
QT_MOC_LITERAL(19, 370, 9), // "USER_LIST"
QT_MOC_LITERAL(20, 380, 7), // "USER_IO"
QT_MOC_LITERAL(21, 388, 14), // "USER_AUTHORITY"
QT_MOC_LITERAL(22, 403, 15), // "CHANGE_PASSWORD"
QT_MOC_LITERAL(23, 419, 17), // "ALARAM_MANAGEMENT"
QT_MOC_LITERAL(24, 437, 16), // "IP_CONFIGURATION"
QT_MOC_LITERAL(25, 454, 15), // "EXTERNAL_STATUS"
QT_MOC_LITERAL(26, 470, 10), // "COMPONENTS"
QT_MOC_LITERAL(27, 481, 7), // "BRANSON"
QT_MOC_LITERAL(28, 489, 12), // "CLEAR_MEMORY"
QT_MOC_LITERAL(29, 502, 14), // "SCAN_SEEK_TEST"
QT_MOC_LITERAL(30, 517, 11), // "SYSTEM_DATA"
QT_MOC_LITERAL(31, 529, 29), // "SET_MEMORY_FULL_ACTION_TO_YES"
QT_MOC_LITERAL(32, 559, 11), // "BATCH_SETUP"
QT_MOC_LITERAL(33, 571, 12), // "SYSTEM_CLOCK"
QT_MOC_LITERAL(34, 584, 42), // "NEW_SAVE_COPY_VALIDATED_UNVAL..."
QT_MOC_LITERAL(35, 627, 18), // "RESET_WELD_COUNTER"
QT_MOC_LITERAL(36, 646, 8) // "SECURITY"

    },
    "Features\0FeaturesEnum\0RUN_VALIDATED_WELD_RECIPE\0"
    "RUN_UNVALIDATED_WELD_RECIPE\0RESET_ALARAMS\0"
    "SET_AS_ACTIVE\0RESET_BATCH_COUNTER\0"
    "QUICK_CALIBRATION\0BARCODE_RECIPE_RECALL\0"
    "WELD_SETUP\0HORN_DOWN\0DIAGONISTICS\0"
    "MODIFY_WELD_PARAMS_WITH_MIN_MAX\0"
    "SYSTEM_CONFIGURATION\0VALIDATE_AND_LOCK_WELD_RECIPE\0"
    "MODIFY_LOCKED_WELD_RECIPE\0"
    "ADD_OR_MODIFY_USER_ID_SETUP\0"
    "SET_AUTHORITY_CHECK_TO_YES\0GENERAL\0"
    "USER_LIST\0USER_IO\0USER_AUTHORITY\0"
    "CHANGE_PASSWORD\0ALARAM_MANAGEMENT\0"
    "IP_CONFIGURATION\0EXTERNAL_STATUS\0"
    "COMPONENTS\0BRANSON\0CLEAR_MEMORY\0"
    "SCAN_SEEK_TEST\0SYSTEM_DATA\0"
    "SET_MEMORY_FULL_ACTION_TO_YES\0BATCH_SETUP\0"
    "SYSTEM_CLOCK\0NEW_SAVE_COPY_VALIDATED_UNVALIDATED_RECIPE\0"
    "RESET_WELD_COUNTER\0SECURITY"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Features[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x0,   35,   18,

 // enum data: key, value
       2, uint(Features::RUN_VALIDATED_WELD_RECIPE),
       3, uint(Features::RUN_UNVALIDATED_WELD_RECIPE),
       4, uint(Features::RESET_ALARAMS),
       5, uint(Features::SET_AS_ACTIVE),
       6, uint(Features::RESET_BATCH_COUNTER),
       7, uint(Features::QUICK_CALIBRATION),
       8, uint(Features::BARCODE_RECIPE_RECALL),
       9, uint(Features::WELD_SETUP),
      10, uint(Features::HORN_DOWN),
      11, uint(Features::DIAGONISTICS),
      12, uint(Features::MODIFY_WELD_PARAMS_WITH_MIN_MAX),
      13, uint(Features::SYSTEM_CONFIGURATION),
      14, uint(Features::VALIDATE_AND_LOCK_WELD_RECIPE),
      15, uint(Features::MODIFY_LOCKED_WELD_RECIPE),
      16, uint(Features::ADD_OR_MODIFY_USER_ID_SETUP),
      17, uint(Features::SET_AUTHORITY_CHECK_TO_YES),
      18, uint(Features::GENERAL),
      19, uint(Features::USER_LIST),
      20, uint(Features::USER_IO),
      21, uint(Features::USER_AUTHORITY),
      22, uint(Features::CHANGE_PASSWORD),
      23, uint(Features::ALARAM_MANAGEMENT),
      24, uint(Features::IP_CONFIGURATION),
      25, uint(Features::EXTERNAL_STATUS),
      26, uint(Features::COMPONENTS),
      27, uint(Features::BRANSON),
      28, uint(Features::CLEAR_MEMORY),
      29, uint(Features::SCAN_SEEK_TEST),
      30, uint(Features::SYSTEM_DATA),
      31, uint(Features::SET_MEMORY_FULL_ACTION_TO_YES),
      32, uint(Features::BATCH_SETUP),
      33, uint(Features::SYSTEM_CLOCK),
      34, uint(Features::NEW_SAVE_COPY_VALIDATED_UNVALIDATED_RECIPE),
      35, uint(Features::RESET_WELD_COUNTER),
      36, uint(Features::SECURITY),

       0        // eod
};

void Features::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Features::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Features.data,
      qt_meta_data_Features,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Features::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Features::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Features.stringdata0))
        return static_cast<void*>(const_cast< Features*>(this));
    return QObject::qt_metacast(_clname);
}

int Features::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_EventLogging_t {
    QByteArrayData data[28];
    char stringdata0[701];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EventLogging_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EventLogging_t qt_meta_stringdata_EventLogging = {
    {
QT_MOC_LITERAL(0, 0, 12), // "EventLogging"
QT_MOC_LITERAL(1, 13, 16), // "EventLoggingEnum"
QT_MOC_LITERAL(2, 30, 19), // "UI_EVENT_USER_LOGIN"
QT_MOC_LITERAL(3, 50, 20), // "UI_EVENT_USER_LOGOUT"
QT_MOC_LITERAL(4, 71, 28), // "UI_EVENT_HORN_SCAN_COMPLETED"
QT_MOC_LITERAL(5, 100, 26), // "UI_EVENT_HORN_SCAN_ABORTED"
QT_MOC_LITERAL(6, 127, 21), // "UI_EVENT_USER_CREATED"
QT_MOC_LITERAL(7, 149, 22), // "UI_EVENT_USER_MODIFIED"
QT_MOC_LITERAL(8, 172, 32), // "UI_EVENT_AUTHORITY_CHECK_CHANGED"
QT_MOC_LITERAL(9, 205, 23), // "UI_EVENT_RECIPE_CREATED"
QT_MOC_LITERAL(10, 229, 24), // "UI_EVENT_RECIPE_MODIFIED"
QT_MOC_LITERAL(11, 254, 26), // "UI_EVENT_SEEK_SCAN_STARTED"
QT_MOC_LITERAL(12, 281, 29), // "UI_EVENT_CYCLE_COUNT_RESETTED"
QT_MOC_LITERAL(13, 311, 23), // "UI_EVENT_BOARD_REPLACED"
QT_MOC_LITERAL(14, 335, 23), // "UI_EVENT_SYSTEM_CLEANED"
QT_MOC_LITERAL(15, 359, 39), // "UI_EVENT_SYSTEM_COMPONENT_FAC..."
QT_MOC_LITERAL(16, 399, 25), // "UI_EVENT_RECIPE_VALIDATED"
QT_MOC_LITERAL(17, 425, 27), // "UI_EVENT_RECIPE_INVALIDATED"
QT_MOC_LITERAL(18, 453, 21), // "UI_EVENT_DATE_CHANGED"
QT_MOC_LITERAL(19, 475, 25), // "UI_EVENT_PASSWORD_CHANGED"
QT_MOC_LITERAL(20, 501, 22), // "UI_EVENT_RECIPE_COPIED"
QT_MOC_LITERAL(21, 524, 25), // "UI_EVENT_VALIDATED_CHANGE"
QT_MOC_LITERAL(22, 550, 27), // "UI_EVENT_SYSTEM_CALIBRATION"
QT_MOC_LITERAL(23, 578, 26), // "UI_EVENT_SYSTEM_HW_CHANGED"
QT_MOC_LITERAL(24, 605, 29), // "UI_EVENT_UIC_SOFTWARE_VERSION"
QT_MOC_LITERAL(25, 635, 21), // "UI_EVENT_RECIPE_SAVED"
QT_MOC_LITERAL(26, 657, 20), // "UI_EVENT_USER_LOCKED"
QT_MOC_LITERAL(27, 678, 22) // "UI_EVENT_DELETE_RECIPE"

    },
    "EventLogging\0EventLoggingEnum\0"
    "UI_EVENT_USER_LOGIN\0UI_EVENT_USER_LOGOUT\0"
    "UI_EVENT_HORN_SCAN_COMPLETED\0"
    "UI_EVENT_HORN_SCAN_ABORTED\0"
    "UI_EVENT_USER_CREATED\0UI_EVENT_USER_MODIFIED\0"
    "UI_EVENT_AUTHORITY_CHECK_CHANGED\0"
    "UI_EVENT_RECIPE_CREATED\0"
    "UI_EVENT_RECIPE_MODIFIED\0"
    "UI_EVENT_SEEK_SCAN_STARTED\0"
    "UI_EVENT_CYCLE_COUNT_RESETTED\0"
    "UI_EVENT_BOARD_REPLACED\0UI_EVENT_SYSTEM_CLEANED\0"
    "UI_EVENT_SYSTEM_COMPONENT_FACTORY_RESET\0"
    "UI_EVENT_RECIPE_VALIDATED\0"
    "UI_EVENT_RECIPE_INVALIDATED\0"
    "UI_EVENT_DATE_CHANGED\0UI_EVENT_PASSWORD_CHANGED\0"
    "UI_EVENT_RECIPE_COPIED\0UI_EVENT_VALIDATED_CHANGE\0"
    "UI_EVENT_SYSTEM_CALIBRATION\0"
    "UI_EVENT_SYSTEM_HW_CHANGED\0"
    "UI_EVENT_UIC_SOFTWARE_VERSION\0"
    "UI_EVENT_RECIPE_SAVED\0UI_EVENT_USER_LOCKED\0"
    "UI_EVENT_DELETE_RECIPE"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EventLogging[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x2,   26,   18,

 // enum data: key, value
       2, uint(EventLogging::EventLoggingEnum::UI_EVENT_USER_LOGIN),
       3, uint(EventLogging::EventLoggingEnum::UI_EVENT_USER_LOGOUT),
       4, uint(EventLogging::EventLoggingEnum::UI_EVENT_HORN_SCAN_COMPLETED),
       5, uint(EventLogging::EventLoggingEnum::UI_EVENT_HORN_SCAN_ABORTED),
       6, uint(EventLogging::EventLoggingEnum::UI_EVENT_USER_CREATED),
       7, uint(EventLogging::EventLoggingEnum::UI_EVENT_USER_MODIFIED),
       8, uint(EventLogging::EventLoggingEnum::UI_EVENT_AUTHORITY_CHECK_CHANGED),
       9, uint(EventLogging::EventLoggingEnum::UI_EVENT_RECIPE_CREATED),
      10, uint(EventLogging::EventLoggingEnum::UI_EVENT_RECIPE_MODIFIED),
      11, uint(EventLogging::EventLoggingEnum::UI_EVENT_SEEK_SCAN_STARTED),
      12, uint(EventLogging::EventLoggingEnum::UI_EVENT_CYCLE_COUNT_RESETTED),
      13, uint(EventLogging::EventLoggingEnum::UI_EVENT_BOARD_REPLACED),
      14, uint(EventLogging::EventLoggingEnum::UI_EVENT_SYSTEM_CLEANED),
      15, uint(EventLogging::EventLoggingEnum::UI_EVENT_SYSTEM_COMPONENT_FACTORY_RESET),
      16, uint(EventLogging::EventLoggingEnum::UI_EVENT_RECIPE_VALIDATED),
      17, uint(EventLogging::EventLoggingEnum::UI_EVENT_RECIPE_INVALIDATED),
      18, uint(EventLogging::EventLoggingEnum::UI_EVENT_DATE_CHANGED),
      19, uint(EventLogging::EventLoggingEnum::UI_EVENT_PASSWORD_CHANGED),
      20, uint(EventLogging::EventLoggingEnum::UI_EVENT_RECIPE_COPIED),
      21, uint(EventLogging::EventLoggingEnum::UI_EVENT_VALIDATED_CHANGE),
      22, uint(EventLogging::EventLoggingEnum::UI_EVENT_SYSTEM_CALIBRATION),
      23, uint(EventLogging::EventLoggingEnum::UI_EVENT_SYSTEM_HW_CHANGED),
      24, uint(EventLogging::EventLoggingEnum::UI_EVENT_UIC_SOFTWARE_VERSION),
      25, uint(EventLogging::EventLoggingEnum::UI_EVENT_RECIPE_SAVED),
      26, uint(EventLogging::EventLoggingEnum::UI_EVENT_USER_LOCKED),
      27, uint(EventLogging::EventLoggingEnum::UI_EVENT_DELETE_RECIPE),

       0        // eod
};

void EventLogging::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject EventLogging::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_EventLogging.data,
      qt_meta_data_EventLogging,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *EventLogging::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EventLogging::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EventLogging.stringdata0))
        return static_cast<void*>(const_cast< EventLogging*>(this));
    return QObject::qt_metacast(_clname);
}

int EventLogging::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_RuntimeFeatures_t {
    QByteArrayData data[52];
    char stringdata0[788];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RuntimeFeatures_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RuntimeFeatures_t qt_meta_stringdata_RuntimeFeatures = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RuntimeFeatures"
QT_MOC_LITERAL(1, 16, 19), // "RuntimeFeaturesEnum"
QT_MOC_LITERAL(2, 36, 13), // "UI_MULTI_MODE"
QT_MOC_LITERAL(3, 50, 12), // "UI_MODE_TIME"
QT_MOC_LITERAL(4, 63, 14), // "UI_MODE_ENERGY"
QT_MOC_LITERAL(5, 78, 13), // "UI_MODE_POWER"
QT_MOC_LITERAL(6, 92, 25), // "UI_MODE_COLLAPSE_DISTANCE"
QT_MOC_LITERAL(7, 118, 25), // "UI_MODE_ABSOLUTE_DISTANCE"
QT_MOC_LITERAL(8, 144, 21), // "UI_MODE_GROUND_DETECT"
QT_MOC_LITERAL(9, 166, 17), // "UI_MODE_CONTINOUS"
QT_MOC_LITERAL(10, 184, 14), // "UI_PRE_TRIGGER"
QT_MOC_LITERAL(11, 199, 14), // "UI_AFTER_BURST"
QT_MOC_LITERAL(12, 214, 15), // "UI_ENERGY_BRAKE"
QT_MOC_LITERAL(13, 230, 17), // "UI_AMPLITUDE_STEP"
QT_MOC_LITERAL(14, 248, 13), // "UI_FORCE_STEP"
QT_MOC_LITERAL(15, 262, 12), // "UI_ALARM_LOG"
QT_MOC_LITERAL(16, 275, 12), // "UI_EVENT_LOG"
QT_MOC_LITERAL(17, 288, 11), // "UI_WELD_LOG"
QT_MOC_LITERAL(18, 300, 12), // "UI_REPORTING"
QT_MOC_LITERAL(19, 313, 15), // "UI_CRACKED_HORN"
QT_MOC_LITERAL(20, 329, 6), // "UI_USB"
QT_MOC_LITERAL(21, 336, 9), // "UI_BINARY"
QT_MOC_LITERAL(22, 346, 15), // "UI_WEB_SERVICES"
QT_MOC_LITERAL(23, 362, 9), // "UI_OPC_UA"
QT_MOC_LITERAL(24, 372, 10), // "UI_USER_IO"
QT_MOC_LITERAL(25, 383, 14), // "UI_ACTUATOR_IO"
QT_MOC_LITERAL(26, 398, 16), // "UI_CYCLE_COUNTER"
QT_MOC_LITERAL(27, 415, 16), // "UI_BATCH_COUNTER"
QT_MOC_LITERAL(28, 432, 20), // "UI_POWER_MATCH_CURVE"
QT_MOC_LITERAL(29, 453, 18), // "UI_SMART_CONVERTER"
QT_MOC_LITERAL(30, 472, 16), // "UI_CONTROL_SPEED"
QT_MOC_LITERAL(31, 489, 13), // "UI_SEQUENCING"
QT_MOC_LITERAL(32, 503, 25), // "UI_AUTO_CONVERTER_COOLING"
QT_MOC_LITERAL(33, 529, 12), // "UI_FIELD_BUS"
QT_MOC_LITERAL(34, 542, 19), // "UI_DASHBOARD_SCREEN"
QT_MOC_LITERAL(35, 562, 18), // "UI_PASSWORD_SCREEN"
QT_MOC_LITERAL(36, 581, 9), // "UI_MOBILE"
QT_MOC_LITERAL(37, 591, 10), // "UI_OVERLAY"
QT_MOC_LITERAL(38, 602, 9), // "UI_TRENDS"
QT_MOC_LITERAL(39, 612, 11), // "UI_OPTITUNE"
QT_MOC_LITERAL(40, 624, 9), // "UI_LIMITS"
QT_MOC_LITERAL(41, 634, 10), // "UI_HD_MODE"
QT_MOC_LITERAL(42, 645, 13), // "UI_TEACH_MODE"
QT_MOC_LITERAL(43, 659, 11), // "UI_ADV_COMM"
QT_MOC_LITERAL(44, 671, 21), // "UI_EXTRA_DATA_STORAGE"
QT_MOC_LITERAL(45, 693, 15), // "UI_TOUBLE_SHOOT"
QT_MOC_LITERAL(46, 709, 6), // "UI_CFR"
QT_MOC_LITERAL(47, 716, 15), // "UI_HOUR_COUNTER"
QT_MOC_LITERAL(48, 732, 13), // "UI_MODEL_ID_1"
QT_MOC_LITERAL(49, 746, 13), // "UI_MODEL_ID_2"
QT_MOC_LITERAL(50, 760, 13), // "UI_MODEL_ID_3"
QT_MOC_LITERAL(51, 774, 13) // "UI_MODEL_ID_4"

    },
    "RuntimeFeatures\0RuntimeFeaturesEnum\0"
    "UI_MULTI_MODE\0UI_MODE_TIME\0UI_MODE_ENERGY\0"
    "UI_MODE_POWER\0UI_MODE_COLLAPSE_DISTANCE\0"
    "UI_MODE_ABSOLUTE_DISTANCE\0"
    "UI_MODE_GROUND_DETECT\0UI_MODE_CONTINOUS\0"
    "UI_PRE_TRIGGER\0UI_AFTER_BURST\0"
    "UI_ENERGY_BRAKE\0UI_AMPLITUDE_STEP\0"
    "UI_FORCE_STEP\0UI_ALARM_LOG\0UI_EVENT_LOG\0"
    "UI_WELD_LOG\0UI_REPORTING\0UI_CRACKED_HORN\0"
    "UI_USB\0UI_BINARY\0UI_WEB_SERVICES\0"
    "UI_OPC_UA\0UI_USER_IO\0UI_ACTUATOR_IO\0"
    "UI_CYCLE_COUNTER\0UI_BATCH_COUNTER\0"
    "UI_POWER_MATCH_CURVE\0UI_SMART_CONVERTER\0"
    "UI_CONTROL_SPEED\0UI_SEQUENCING\0"
    "UI_AUTO_CONVERTER_COOLING\0UI_FIELD_BUS\0"
    "UI_DASHBOARD_SCREEN\0UI_PASSWORD_SCREEN\0"
    "UI_MOBILE\0UI_OVERLAY\0UI_TRENDS\0"
    "UI_OPTITUNE\0UI_LIMITS\0UI_HD_MODE\0"
    "UI_TEACH_MODE\0UI_ADV_COMM\0"
    "UI_EXTRA_DATA_STORAGE\0UI_TOUBLE_SHOOT\0"
    "UI_CFR\0UI_HOUR_COUNTER\0UI_MODEL_ID_1\0"
    "UI_MODEL_ID_2\0UI_MODEL_ID_3\0UI_MODEL_ID_4"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RuntimeFeatures[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x2,   50,   18,

 // enum data: key, value
       2, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MULTI_MODE),
       3, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MODE_TIME),
       4, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MODE_ENERGY),
       5, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MODE_POWER),
       6, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MODE_COLLAPSE_DISTANCE),
       7, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MODE_ABSOLUTE_DISTANCE),
       8, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MODE_GROUND_DETECT),
       9, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MODE_CONTINOUS),
      10, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_PRE_TRIGGER),
      11, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_AFTER_BURST),
      12, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_ENERGY_BRAKE),
      13, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_AMPLITUDE_STEP),
      14, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_FORCE_STEP),
      15, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_ALARM_LOG),
      16, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_EVENT_LOG),
      17, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_WELD_LOG),
      18, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_REPORTING),
      19, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_CRACKED_HORN),
      20, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_USB),
      21, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_BINARY),
      22, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_WEB_SERVICES),
      23, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_OPC_UA),
      24, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_USER_IO),
      25, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_ACTUATOR_IO),
      26, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_CYCLE_COUNTER),
      27, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_BATCH_COUNTER),
      28, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_POWER_MATCH_CURVE),
      29, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_SMART_CONVERTER),
      30, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_CONTROL_SPEED),
      31, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_SEQUENCING),
      32, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_AUTO_CONVERTER_COOLING),
      33, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_FIELD_BUS),
      34, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_DASHBOARD_SCREEN),
      35, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_PASSWORD_SCREEN),
      36, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MOBILE),
      37, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_OVERLAY),
      38, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_TRENDS),
      39, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_OPTITUNE),
      40, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_LIMITS),
      41, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_HD_MODE),
      42, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_TEACH_MODE),
      43, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_ADV_COMM),
      44, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_EXTRA_DATA_STORAGE),
      45, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_TOUBLE_SHOOT),
      46, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_CFR),
      47, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_HOUR_COUNTER),
      48, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MODEL_ID_1),
      49, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MODEL_ID_2),
      50, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MODEL_ID_3),
      51, uint(RuntimeFeatures::RuntimeFeaturesEnum::UI_MODEL_ID_4),

       0        // eod
};

void RuntimeFeatures::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject RuntimeFeatures::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RuntimeFeatures.data,
      qt_meta_data_RuntimeFeatures,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RuntimeFeatures::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RuntimeFeatures::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RuntimeFeatures.stringdata0))
        return static_cast<void*>(const_cast< RuntimeFeatures*>(this));
    return QObject::qt_metacast(_clname);
}

int RuntimeFeatures::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_WSRestrictionScreen_t {
    QByteArrayData data[7];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WSRestrictionScreen_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WSRestrictionScreen_t qt_meta_stringdata_WSRestrictionScreen = {
    {
QT_MOC_LITERAL(0, 0, 19), // "WSRestrictionScreen"
QT_MOC_LITERAL(1, 20, 23), // "WSRestrictionScreenEnum"
QT_MOC_LITERAL(2, 44, 13), // "NO_RESTRICTED"
QT_MOC_LITERAL(3, 58, 18), // "GENERAL_RESTRICTED"
QT_MOC_LITERAL(4, 77, 15), // "USER_MANAGEMENT"
QT_MOC_LITERAL(5, 93, 23), // "USER_OPERATOR_AUTHORITY"
QT_MOC_LITERAL(6, 117, 15) // "CHANGE_PASSWORD"

    },
    "WSRestrictionScreen\0WSRestrictionScreenEnum\0"
    "NO_RESTRICTED\0GENERAL_RESTRICTED\0"
    "USER_MANAGEMENT\0USER_OPERATOR_AUTHORITY\0"
    "CHANGE_PASSWORD"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WSRestrictionScreen[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x2,    5,   18,

 // enum data: key, value
       2, uint(WSRestrictionScreen::WSRestrictionScreenEnum::NO_RESTRICTED),
       3, uint(WSRestrictionScreen::WSRestrictionScreenEnum::GENERAL_RESTRICTED),
       4, uint(WSRestrictionScreen::WSRestrictionScreenEnum::USER_MANAGEMENT),
       5, uint(WSRestrictionScreen::WSRestrictionScreenEnum::USER_OPERATOR_AUTHORITY),
       6, uint(WSRestrictionScreen::WSRestrictionScreenEnum::CHANGE_PASSWORD),

       0        // eod
};

void WSRestrictionScreen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject WSRestrictionScreen::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WSRestrictionScreen.data,
      qt_meta_data_WSRestrictionScreen,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *WSRestrictionScreen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WSRestrictionScreen::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WSRestrictionScreen.stringdata0))
        return static_cast<void*>(const_cast< WSRestrictionScreen*>(this));
    return QObject::qt_metacast(_clname);
}

int WSRestrictionScreen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_DeleteRecipeResponse_t {
    QByteArrayData data[9];
    char stringdata0[198];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DeleteRecipeResponse_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DeleteRecipeResponse_t qt_meta_stringdata_DeleteRecipeResponse = {
    {
QT_MOC_LITERAL(0, 0, 20), // "DeleteRecipeResponse"
QT_MOC_LITERAL(1, 21, 24), // "DeleteRecipeResponseEnum"
QT_MOC_LITERAL(2, 46, 16), // "NOT_RECEIVE_DATA"
QT_MOC_LITERAL(3, 63, 21), // "DELETE_RECIPE_SUCCESS"
QT_MOC_LITERAL(4, 85, 21), // "INVALID_RECIPE_NUMBER"
QT_MOC_LITERAL(5, 107, 34), // "DELETE_RECIPE_FAILED_ACTIVE_R..."
QT_MOC_LITERAL(6, 142, 20), // "DELETE_RECIPE_FAILED"
QT_MOC_LITERAL(7, 163, 18), // "RECIPE_NOT_PRESENT"
QT_MOC_LITERAL(8, 182, 15) // "END_FIAL_DELETE"

    },
    "DeleteRecipeResponse\0DeleteRecipeResponseEnum\0"
    "NOT_RECEIVE_DATA\0DELETE_RECIPE_SUCCESS\0"
    "INVALID_RECIPE_NUMBER\0"
    "DELETE_RECIPE_FAILED_ACTIVE_RECIPE\0"
    "DELETE_RECIPE_FAILED\0RECIPE_NOT_PRESENT\0"
    "END_FIAL_DELETE"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DeleteRecipeResponse[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x0,    7,   18,

 // enum data: key, value
       2, uint(DeleteRecipeResponse::NOT_RECEIVE_DATA),
       3, uint(DeleteRecipeResponse::DELETE_RECIPE_SUCCESS),
       4, uint(DeleteRecipeResponse::INVALID_RECIPE_NUMBER),
       5, uint(DeleteRecipeResponse::DELETE_RECIPE_FAILED_ACTIVE_RECIPE),
       6, uint(DeleteRecipeResponse::DELETE_RECIPE_FAILED),
       7, uint(DeleteRecipeResponse::RECIPE_NOT_PRESENT),
       8, uint(DeleteRecipeResponse::END_FIAL_DELETE),

       0        // eod
};

void DeleteRecipeResponse::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject DeleteRecipeResponse::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DeleteRecipeResponse.data,
      qt_meta_data_DeleteRecipeResponse,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DeleteRecipeResponse::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DeleteRecipeResponse::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DeleteRecipeResponse.stringdata0))
        return static_cast<void*>(const_cast< DeleteRecipeResponse*>(this));
    return QObject::qt_metacast(_clname);
}

int DeleteRecipeResponse::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
