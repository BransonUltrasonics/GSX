/****************************************************************************
** Meta object code from reading C++ file 'setupparameter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/setupparameter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setupparameter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetupParameter_t {
    QByteArrayData data[8];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetupParameter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetupParameter_t qt_meta_stringdata_SetupParameter = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SetupParameter"
QT_MOC_LITERAL(1, 15, 16), // "paramNameChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 17), // "paramValueChanged"
QT_MOC_LITERAL(4, 51, 16), // "paramUnitChanged"
QT_MOC_LITERAL(5, 68, 9), // "ParamName"
QT_MOC_LITERAL(6, 78, 10), // "ParamValue"
QT_MOC_LITERAL(7, 89, 9) // "ParamUnit"

    },
    "SetupParameter\0paramNameChanged\0\0"
    "paramValueChanged\0paramUnitChanged\0"
    "ParamName\0ParamValue\0ParamUnit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetupParameter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       3,   32, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    0,   30,    2, 0x06 /* Public */,
       4,    0,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       5, QMetaType::QString, 0x00495103,
       6, QMetaType::QString, 0x00495103,
       7, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

void SetupParameter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SetupParameter *_t = static_cast<SetupParameter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->paramNameChanged(); break;
        case 1: _t->paramValueChanged(); break;
        case 2: _t->paramUnitChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SetupParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SetupParameter::paramNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SetupParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SetupParameter::paramValueChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SetupParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SetupParameter::paramUnitChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        SetupParameter *_t = static_cast<SetupParameter *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getParamName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getParamValue(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getParamUnit(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        SetupParameter *_t = static_cast<SetupParameter *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setParamName(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setParamValue(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setParamUnit(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject SetupParameter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SetupParameter.data,
      qt_meta_data_SetupParameter,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SetupParameter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetupParameter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetupParameter.stringdata0))
        return static_cast<void*>(const_cast< SetupParameter*>(this));
    return QObject::qt_metacast(_clname);
}

int SetupParameter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SetupParameter::paramNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SetupParameter::paramValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SetupParameter::paramUnitChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
