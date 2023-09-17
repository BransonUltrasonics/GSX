/****************************************************************************
** Meta object code from reading C++ file 'trendsgraphparametername.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/trendsgraphparametername.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trendsgraphparametername.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Trendsgraphparametername_t {
    QByteArrayData data[6];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Trendsgraphparametername_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Trendsgraphparametername_t qt_meta_stringdata_Trendsgraphparametername = {
    {
QT_MOC_LITERAL(0, 0, 24), // "Trendsgraphparametername"
QT_MOC_LITERAL(1, 25, 20), // "parameterNameChanged"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 22), // "cycleCounterValChanged"
QT_MOC_LITERAL(4, 70, 13), // "parameterName"
QT_MOC_LITERAL(5, 84, 15) // "cycleCounterVal"

    },
    "Trendsgraphparametername\0parameterNameChanged\0"
    "\0cycleCounterValChanged\0parameterName\0"
    "cycleCounterVal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Trendsgraphparametername[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       2,   26, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       4, QMetaType::QString, 0x00495103,
       5, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void Trendsgraphparametername::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Trendsgraphparametername *_t = static_cast<Trendsgraphparametername *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->parameterNameChanged(); break;
        case 1: _t->cycleCounterValChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Trendsgraphparametername::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Trendsgraphparametername::parameterNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Trendsgraphparametername::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Trendsgraphparametername::cycleCounterValChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Trendsgraphparametername *_t = static_cast<Trendsgraphparametername *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getParameterName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getCycleCounterVal(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Trendsgraphparametername *_t = static_cast<Trendsgraphparametername *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setParameterName(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setCycleCounterVal(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject Trendsgraphparametername::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Trendsgraphparametername.data,
      qt_meta_data_Trendsgraphparametername,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Trendsgraphparametername::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Trendsgraphparametername::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Trendsgraphparametername.stringdata0))
        return static_cast<void*>(const_cast< Trendsgraphparametername*>(this));
    return QObject::qt_metacast(_clname);
}

int Trendsgraphparametername::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Trendsgraphparametername::parameterNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Trendsgraphparametername::cycleCounterValChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_TrendsGraphCycleCounterVal_t {
    QByteArrayData data[6];
    char stringdata0[92];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TrendsGraphCycleCounterVal_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TrendsGraphCycleCounterVal_t qt_meta_stringdata_TrendsGraphCycleCounterVal = {
    {
QT_MOC_LITERAL(0, 0, 26), // "TrendsGraphCycleCounterVal"
QT_MOC_LITERAL(1, 27, 21), // "cycleCountNameChanged"
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 16), // "alarmFlagChanged"
QT_MOC_LITERAL(4, 67, 14), // "cycleCountName"
QT_MOC_LITERAL(5, 82, 9) // "alarmFlag"

    },
    "TrendsGraphCycleCounterVal\0"
    "cycleCountNameChanged\0\0alarmFlagChanged\0"
    "cycleCountName\0alarmFlag"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TrendsGraphCycleCounterVal[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       2,   26, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       4, QMetaType::QString, 0x00495001,
       5, QMetaType::Int, 0x00495001,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void TrendsGraphCycleCounterVal::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TrendsGraphCycleCounterVal *_t = static_cast<TrendsGraphCycleCounterVal *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->cycleCountNameChanged(); break;
        case 1: _t->alarmFlagChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TrendsGraphCycleCounterVal::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraphCycleCounterVal::cycleCountNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TrendsGraphCycleCounterVal::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraphCycleCounterVal::alarmFlagChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TrendsGraphCycleCounterVal *_t = static_cast<TrendsGraphCycleCounterVal *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getCycleCountName(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->getAlarmFlag(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject TrendsGraphCycleCounterVal::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TrendsGraphCycleCounterVal.data,
      qt_meta_data_TrendsGraphCycleCounterVal,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TrendsGraphCycleCounterVal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrendsGraphCycleCounterVal::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TrendsGraphCycleCounterVal.stringdata0))
        return static_cast<void*>(const_cast< TrendsGraphCycleCounterVal*>(this));
    return QObject::qt_metacast(_clname);
}

int TrendsGraphCycleCounterVal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TrendsGraphCycleCounterVal::cycleCountNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TrendsGraphCycleCounterVal::alarmFlagChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
