/****************************************************************************
** Meta object code from reading C++ file 'globalsettings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/globalsettings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'globalsettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GlobalSettings_t {
    QByteArrayData data[9];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GlobalSettings_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GlobalSettings_t qt_meta_stringdata_GlobalSettings = {
    {
QT_MOC_LITERAL(0, 0, 14), // "GlobalSettings"
QT_MOC_LITERAL(1, 15, 27), // "passwordExpiryInDaysChanged"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 22), // "idealLogoffTimeChanged"
QT_MOC_LITERAL(4, 67, 20), // "handleGlobalSettings"
QT_MOC_LITERAL(5, 88, 20), // "updateGlobalSettings"
QT_MOC_LITERAL(6, 109, 14), // "passwordExpiry"
QT_MOC_LITERAL(7, 124, 15), // "idealLogoffTime"
QT_MOC_LITERAL(8, 140, 20) // "passwordExpiryInDays"

    },
    "GlobalSettings\0passwordExpiryInDaysChanged\0"
    "\0idealLogoffTimeChanged\0handleGlobalSettings\0"
    "updateGlobalSettings\0passwordExpiry\0"
    "idealLogoffTime\0passwordExpiryInDays"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GlobalSettings[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       2,   42, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       4,    0,   36,    2, 0x02 /* Public */,
       5,    2,   37,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,

 // properties: name, type, flags
       8, QMetaType::QString, 0x00495103,
       7, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void GlobalSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GlobalSettings *_t = static_cast<GlobalSettings *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->passwordExpiryInDaysChanged(); break;
        case 1: _t->idealLogoffTimeChanged(); break;
        case 2: _t->handleGlobalSettings(); break;
        case 3: _t->updateGlobalSettings((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GlobalSettings::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GlobalSettings::passwordExpiryInDaysChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (GlobalSettings::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GlobalSettings::idealLogoffTimeChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        GlobalSettings *_t = static_cast<GlobalSettings *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->passwordExpiryInDays(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->idealLogoffTime(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        GlobalSettings *_t = static_cast<GlobalSettings *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPasswordExpiryInDays(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setIdealLogoffTime(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject GlobalSettings::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GlobalSettings.data,
      qt_meta_data_GlobalSettings,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *GlobalSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GlobalSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GlobalSettings.stringdata0))
        return static_cast<void*>(const_cast< GlobalSettings*>(this));
    return QObject::qt_metacast(_clname);
}

int GlobalSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
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
void GlobalSettings::passwordExpiryInDaysChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GlobalSettings::idealLogoffTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
