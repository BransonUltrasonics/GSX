/****************************************************************************
** Meta object code from reading C++ file 'stackrecipedata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/stackrecipedata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stackrecipedata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_StackRecipeParameter_t {
    QByteArrayData data[8];
    char stringdata0[129];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StackRecipeParameter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StackRecipeParameter_t qt_meta_stringdata_StackRecipeParameter = {
    {
QT_MOC_LITERAL(0, 0, 20), // "StackRecipeParameter"
QT_MOC_LITERAL(1, 21, 20), // "parameterNameChanged"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 21), // "parameterValueChanged"
QT_MOC_LITERAL(4, 65, 20), // "parameterUnitChanged"
QT_MOC_LITERAL(5, 86, 13), // "parameterName"
QT_MOC_LITERAL(6, 100, 14), // "parameterValue"
QT_MOC_LITERAL(7, 115, 13) // "parameterUnit"

    },
    "StackRecipeParameter\0parameterNameChanged\0"
    "\0parameterValueChanged\0parameterUnitChanged\0"
    "parameterName\0parameterValue\0parameterUnit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StackRecipeParameter[] = {

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
       5, QMetaType::QString, 0x00495001,
       6, QMetaType::UInt, 0x00495001,
       7, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

void StackRecipeParameter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        StackRecipeParameter *_t = static_cast<StackRecipeParameter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->parameterNameChanged(); break;
        case 1: _t->parameterValueChanged(); break;
        case 2: _t->parameterUnitChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (StackRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&StackRecipeParameter::parameterNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (StackRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&StackRecipeParameter::parameterValueChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (StackRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&StackRecipeParameter::parameterUnitChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        StackRecipeParameter *_t = static_cast<StackRecipeParameter *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getParameterName(); break;
        case 1: *reinterpret_cast< quint32*>(_v) = _t->getParameterValue(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getParameterUnit(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject StackRecipeParameter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_StackRecipeParameter.data,
      qt_meta_data_StackRecipeParameter,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *StackRecipeParameter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StackRecipeParameter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_StackRecipeParameter.stringdata0))
        return static_cast<void*>(const_cast< StackRecipeParameter*>(this));
    return QObject::qt_metacast(_clname);
}

int StackRecipeParameter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void StackRecipeParameter::parameterNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void StackRecipeParameter::parameterValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void StackRecipeParameter::parameterUnitChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_StackRecipeData_t {
    QByteArrayData data[14];
    char stringdata0[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StackRecipeData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StackRecipeData_t qt_meta_stringdata_StackRecipeData = {
    {
QT_MOC_LITERAL(0, 0, 15), // "StackRecipeData"
QT_MOC_LITERAL(1, 16, 23), // "updateStackRecipeParams"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 23), // "sendStackRecipeDataToSc"
QT_MOC_LITERAL(4, 65, 15), // "modifyParameter"
QT_MOC_LITERAL(5, 81, 12), // "a_paramValue"
QT_MOC_LITERAL(6, 94, 12), // "a_paramIndex"
QT_MOC_LITERAL(7, 107, 14), // "cancelUserData"
QT_MOC_LITERAL(8, 122, 20), // "resetStackRecipeData"
QT_MOC_LITERAL(9, 143, 18), // "getPowerupLoadFlag"
QT_MOC_LITERAL(10, 162, 23), // "getParameterChangedFlag"
QT_MOC_LITERAL(11, 186, 4), // "init"
QT_MOC_LITERAL(12, 191, 10), // "rangeCheck"
QT_MOC_LITERAL(13, 202, 12) // "saveUserData"

    },
    "StackRecipeData\0updateStackRecipeParams\0"
    "\0sendStackRecipeDataToSc\0modifyParameter\0"
    "a_paramValue\0a_paramIndex\0cancelUserData\0"
    "resetStackRecipeData\0getPowerupLoadFlag\0"
    "getParameterChangedFlag\0init\0rangeCheck\0"
    "saveUserData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StackRecipeData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       3,    0,   65,    2, 0x02 /* Public */,
       4,    2,   66,    2, 0x02 /* Public */,
       7,    0,   71,    2, 0x02 /* Public */,
       8,    0,   72,    2, 0x02 /* Public */,
       9,    0,   73,    2, 0x02 /* Public */,
      10,    0,   74,    2, 0x02 /* Public */,
      11,    0,   75,    2, 0x02 /* Public */,
      12,    2,   76,    2, 0x02 /* Public */,
      13,    0,   81,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // methods: parameters
    QMetaType::Int,
    QMetaType::Void, QMetaType::QString, QMetaType::SChar,    5,    6,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Int,
    QMetaType::Int, QMetaType::QString, QMetaType::SChar,    5,    6,
    QMetaType::Int,

       0        // eod
};

void StackRecipeData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        StackRecipeData *_t = static_cast<StackRecipeData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateStackRecipeParams(); break;
        case 1: { int _r = _t->sendStackRecipeDataToSc();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->modifyParameter((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< qint8(*)>(_a[2]))); break;
        case 3: _t->cancelUserData(); break;
        case 4: { int _r = _t->resetStackRecipeData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->getPowerupLoadFlag();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->getParameterChangedFlag();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { int _r = _t->init();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 8: { int _r = _t->rangeCheck((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< qint8(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->saveUserData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (StackRecipeData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&StackRecipeData::updateStackRecipeParams)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject StackRecipeData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_StackRecipeData.data,
      qt_meta_data_StackRecipeData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *StackRecipeData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StackRecipeData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_StackRecipeData.stringdata0))
        return static_cast<void*>(const_cast< StackRecipeData*>(this));
    return QObject::qt_metacast(_clname);
}

int StackRecipeData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void StackRecipeData::updateStackRecipeParams()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
