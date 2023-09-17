/****************************************************************************
** Meta object code from reading C++ file 'alarmnotification.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/alarmnotification.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'alarmnotification.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AlarmNotification_t {
    QByteArrayData data[33];
    char stringdata0[534];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AlarmNotification_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AlarmNotification_t qt_meta_stringdata_AlarmNotification = {
    {
QT_MOC_LITERAL(0, 0, 17), // "AlarmNotification"
QT_MOC_LITERAL(1, 18, 26), // "clearEstopResetPopUpSignal"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 24), // "OnAlarmNameFromDBChanged"
QT_MOC_LITERAL(4, 71, 24), // "OnAlarmTimeFromDBChanged"
QT_MOC_LITERAL(5, 96, 24), // "OnAlarmDateFromDBChanged"
QT_MOC_LITERAL(6, 121, 14), // "OnResetChanged"
QT_MOC_LITERAL(7, 136, 18), // "SetAlarmNameFromDB"
QT_MOC_LITERAL(8, 155, 11), // "a_AlarmName"
QT_MOC_LITERAL(9, 167, 18), // "SetAlarmTimeFromDB"
QT_MOC_LITERAL(10, 186, 11), // "a_AlarmTime"
QT_MOC_LITERAL(11, 198, 18), // "SetAlarmDateFromDB"
QT_MOC_LITERAL(12, 217, 11), // "a_AlarmDate"
QT_MOC_LITERAL(13, 229, 8), // "setReset"
QT_MOC_LITERAL(14, 238, 7), // "a_reset"
QT_MOC_LITERAL(15, 246, 29), // "clearNonResetNotificationlist"
QT_MOC_LITERAL(16, 276, 9), // "alarmInit"
QT_MOC_LITERAL(17, 286, 15), // "updateAlarmList"
QT_MOC_LITERAL(18, 302, 7), // "a_Index"
QT_MOC_LITERAL(19, 310, 33), // "updateAlarmListFromAnalyticsA..."
QT_MOC_LITERAL(20, 344, 4), // "a_id"
QT_MOC_LITERAL(21, 349, 17), // "getAlarmIdAtIndex"
QT_MOC_LITERAL(22, 367, 7), // "a_index"
QT_MOC_LITERAL(23, 375, 20), // "fillNotificationList"
QT_MOC_LITERAL(24, 396, 9), // "a_AlarmID"
QT_MOC_LITERAL(25, 406, 26), // "clearAlarmNotificationList"
QT_MOC_LITERAL(26, 433, 8), // "getReset"
QT_MOC_LITERAL(27, 442, 15), // "getAlarmDetails"
QT_MOC_LITERAL(28, 458, 13), // "getNoOfAlarms"
QT_MOC_LITERAL(29, 472, 7), // "m_Reset"
QT_MOC_LITERAL(30, 480, 17), // "m_AlarmNameFromDB"
QT_MOC_LITERAL(31, 498, 17), // "m_AlarmTimeFromDB"
QT_MOC_LITERAL(32, 516, 17) // "m_AlarmDateFromDB"

    },
    "AlarmNotification\0clearEstopResetPopUpSignal\0"
    "\0OnAlarmNameFromDBChanged\0"
    "OnAlarmTimeFromDBChanged\0"
    "OnAlarmDateFromDBChanged\0OnResetChanged\0"
    "SetAlarmNameFromDB\0a_AlarmName\0"
    "SetAlarmTimeFromDB\0a_AlarmTime\0"
    "SetAlarmDateFromDB\0a_AlarmDate\0setReset\0"
    "a_reset\0clearNonResetNotificationlist\0"
    "alarmInit\0updateAlarmList\0a_Index\0"
    "updateAlarmListFromAnalyticsAlarm\0"
    "a_id\0getAlarmIdAtIndex\0a_index\0"
    "fillNotificationList\0a_AlarmID\0"
    "clearAlarmNotificationList\0getReset\0"
    "getAlarmDetails\0getNoOfAlarms\0m_Reset\0"
    "m_AlarmNameFromDB\0m_AlarmTimeFromDB\0"
    "m_AlarmDateFromDB"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AlarmNotification[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       4,  150, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x06 /* Public */,
       3,    0,  110,    2, 0x06 /* Public */,
       4,    0,  111,    2, 0x06 /* Public */,
       5,    0,  112,    2, 0x06 /* Public */,
       6,    0,  113,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,  114,    2, 0x0a /* Public */,
       9,    1,  117,    2, 0x0a /* Public */,
      11,    1,  120,    2, 0x0a /* Public */,
      13,    1,  123,    2, 0x0a /* Public */,
      15,    0,  126,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      16,    0,  127,    2, 0x02 /* Public */,
      17,    1,  128,    2, 0x02 /* Public */,
      19,    1,  131,    2, 0x02 /* Public */,
      21,    1,  134,    2, 0x02 /* Public */,
      23,    4,  137,    2, 0x02 /* Public */,
      25,    0,  146,    2, 0x02 /* Public */,
      26,    0,  147,    2, 0x02 /* Public */,
      27,    0,  148,    2, 0x02 /* Public */,
      28,    0,  149,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::QString, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,   24,   10,   12,   14,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,

 // properties: name, type, flags
      29, QMetaType::Int, 0x00495003,
      30, QMetaType::QString, 0x00495003,
      31, QMetaType::QString, 0x00495003,
      32, QMetaType::QString, 0x00495003,

 // properties: notify_signal_id
       4,
       1,
       2,
       3,

       0        // eod
};

void AlarmNotification::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AlarmNotification *_t = static_cast<AlarmNotification *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clearEstopResetPopUpSignal(); break;
        case 1: _t->OnAlarmNameFromDBChanged(); break;
        case 2: _t->OnAlarmTimeFromDBChanged(); break;
        case 3: _t->OnAlarmDateFromDBChanged(); break;
        case 4: _t->OnResetChanged(); break;
        case 5: _t->SetAlarmNameFromDB((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->SetAlarmTimeFromDB((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->SetAlarmDateFromDB((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->setReset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->clearNonResetNotificationlist(); break;
        case 10: _t->alarmInit(); break;
        case 11: _t->updateAlarmList((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->updateAlarmListFromAnalyticsAlarm((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: { QString _r = _t->getAlarmIdAtIndex((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->fillNotificationList((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 15: _t->clearAlarmNotificationList(); break;
        case 16: { int _r = _t->getReset();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 17: { int _r = _t->getAlarmDetails();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 18: { int _r = _t->getNoOfAlarms();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AlarmNotification::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlarmNotification::clearEstopResetPopUpSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AlarmNotification::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlarmNotification::OnAlarmNameFromDBChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AlarmNotification::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlarmNotification::OnAlarmTimeFromDBChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AlarmNotification::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlarmNotification::OnAlarmDateFromDBChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AlarmNotification::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlarmNotification::OnResetChanged)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        AlarmNotification *_t = static_cast<AlarmNotification *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->getReset(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->GetAlarmNameFromDB(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->GetAlarmTimeFromDB(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->GetAlarmDateFromDB(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        AlarmNotification *_t = static_cast<AlarmNotification *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setReset(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->SetAlarmNameFromDB(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->SetAlarmTimeFromDB(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->SetAlarmDateFromDB(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject AlarmNotification::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AlarmNotification.data,
      qt_meta_data_AlarmNotification,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AlarmNotification::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AlarmNotification::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AlarmNotification.stringdata0))
        return static_cast<void*>(const_cast< AlarmNotification*>(this));
    return QObject::qt_metacast(_clname);
}

int AlarmNotification::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void AlarmNotification::clearEstopResetPopUpSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AlarmNotification::OnAlarmNameFromDBChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AlarmNotification::OnAlarmTimeFromDBChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AlarmNotification::OnAlarmDateFromDBChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void AlarmNotification::OnResetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
