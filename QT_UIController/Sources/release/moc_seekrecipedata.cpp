/****************************************************************************
** Meta object code from reading C++ file 'seekrecipedata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/seekrecipedata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'seekrecipedata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SeekRecipeParameter_t {
    QByteArrayData data[18];
    char stringdata0[349];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SeekRecipeParameter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SeekRecipeParameter_t qt_meta_stringdata_SeekRecipeParameter = {
    {
QT_MOC_LITERAL(0, 0, 19), // "SeekRecipeParameter"
QT_MOC_LITERAL(1, 20, 20), // "parameterNameChanged"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 26), // "parameterLocaleNameChanged"
QT_MOC_LITERAL(4, 69, 21), // "parameterValueChanged"
QT_MOC_LITERAL(5, 91, 20), // "parameterUnitChanged"
QT_MOC_LITERAL(6, 112, 24), // "parameterMinValueChanged"
QT_MOC_LITERAL(7, 137, 24), // "parameterMaxValueChanged"
QT_MOC_LITERAL(8, 162, 22), // "ParameterSwitchChanged"
QT_MOC_LITERAL(9, 185, 27), // "ParameterSwitchStateChanged"
QT_MOC_LITERAL(10, 213, 13), // "parameterName"
QT_MOC_LITERAL(11, 227, 19), // "parameterLocaleName"
QT_MOC_LITERAL(12, 247, 14), // "parameterValue"
QT_MOC_LITERAL(13, 262, 13), // "parameterUnit"
QT_MOC_LITERAL(14, 276, 17), // "parameterMinValue"
QT_MOC_LITERAL(15, 294, 17), // "parameterMaxValue"
QT_MOC_LITERAL(16, 312, 15), // "parameterSwitch"
QT_MOC_LITERAL(17, 328, 20) // "parameterSwitchState"

    },
    "SeekRecipeParameter\0parameterNameChanged\0"
    "\0parameterLocaleNameChanged\0"
    "parameterValueChanged\0parameterUnitChanged\0"
    "parameterMinValueChanged\0"
    "parameterMaxValueChanged\0"
    "ParameterSwitchChanged\0"
    "ParameterSwitchStateChanged\0parameterName\0"
    "parameterLocaleName\0parameterValue\0"
    "parameterUnit\0parameterMinValue\0"
    "parameterMaxValue\0parameterSwitch\0"
    "parameterSwitchState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SeekRecipeParameter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       8,   62, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    0,   56,    2, 0x06 /* Public */,
       5,    0,   57,    2, 0x06 /* Public */,
       6,    0,   58,    2, 0x06 /* Public */,
       7,    0,   59,    2, 0x06 /* Public */,
       8,    0,   60,    2, 0x06 /* Public */,
       9,    0,   61,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      10, QMetaType::QString, 0x00495001,
      11, QMetaType::QString, 0x00495001,
      12, QMetaType::QString, 0x00495001,
      13, QMetaType::QString, 0x00495001,
      14, QMetaType::QString, 0x00495001,
      15, QMetaType::QString, 0x00495001,
      16, QMetaType::Bool, 0x00495001,
      17, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,
       6,
       7,

       0        // eod
};

void SeekRecipeParameter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SeekRecipeParameter *_t = static_cast<SeekRecipeParameter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->parameterNameChanged(); break;
        case 1: _t->parameterLocaleNameChanged(); break;
        case 2: _t->parameterValueChanged(); break;
        case 3: _t->parameterUnitChanged(); break;
        case 4: _t->parameterMinValueChanged(); break;
        case 5: _t->parameterMaxValueChanged(); break;
        case 6: _t->ParameterSwitchChanged(); break;
        case 7: _t->ParameterSwitchStateChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SeekRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekRecipeParameter::parameterNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SeekRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekRecipeParameter::parameterLocaleNameChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SeekRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekRecipeParameter::parameterValueChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SeekRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekRecipeParameter::parameterUnitChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SeekRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekRecipeParameter::parameterMinValueChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (SeekRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekRecipeParameter::parameterMaxValueChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (SeekRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekRecipeParameter::ParameterSwitchChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (SeekRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekRecipeParameter::ParameterSwitchStateChanged)) {
                *result = 7;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        SeekRecipeParameter *_t = static_cast<SeekRecipeParameter *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getParameterName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getParameterLocaleName(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getParameterValue(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getParameterUnit(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getParameterMinValue(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->getParameterMaxValue(); break;
        case 6: *reinterpret_cast< bool*>(_v) = _t->getParameterSwitch(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->getParameterSwitchState(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject SeekRecipeParameter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SeekRecipeParameter.data,
      qt_meta_data_SeekRecipeParameter,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SeekRecipeParameter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SeekRecipeParameter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SeekRecipeParameter.stringdata0))
        return static_cast<void*>(const_cast< SeekRecipeParameter*>(this));
    return QObject::qt_metacast(_clname);
}

int SeekRecipeParameter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SeekRecipeParameter::parameterNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SeekRecipeParameter::parameterLocaleNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SeekRecipeParameter::parameterValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SeekRecipeParameter::parameterUnitChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SeekRecipeParameter::parameterMinValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void SeekRecipeParameter::parameterMaxValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void SeekRecipeParameter::ParameterSwitchChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void SeekRecipeParameter::ParameterSwitchStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
struct qt_meta_stringdata_SeekRecipeData_t {
    QByteArrayData data[30];
    char stringdata0[421];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SeekRecipeData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SeekRecipeData_t qt_meta_stringdata_SeekRecipeData = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SeekRecipeData"
QT_MOC_LITERAL(1, 15, 22), // "updateSeekRecipeParams"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 22), // "sendSeekRecipeDataToSc"
QT_MOC_LITERAL(4, 62, 15), // "modifyParameter"
QT_MOC_LITERAL(5, 78, 12), // "a_paramValue"
QT_MOC_LITERAL(6, 91, 12), // "a_paramIndex"
QT_MOC_LITERAL(7, 104, 14), // "cancelUserData"
QT_MOC_LITERAL(8, 119, 19), // "resetSeekRecipeData"
QT_MOC_LITERAL(9, 139, 18), // "getPowerupLoadFlag"
QT_MOC_LITERAL(10, 158, 23), // "getParameterChangedFlag"
QT_MOC_LITERAL(11, 182, 13), // "getSeekPsFreq"
QT_MOC_LITERAL(12, 196, 4), // "init"
QT_MOC_LITERAL(13, 201, 10), // "rangeCheck"
QT_MOC_LITERAL(14, 212, 14), // "setParamValues"
QT_MOC_LITERAL(15, 227, 5), // "dwAmp"
QT_MOC_LITERAL(16, 233, 6), // "dwFreq"
QT_MOC_LITERAL(17, 240, 7), // "dwPower"
QT_MOC_LITERAL(18, 248, 6), // "dwCurr"
QT_MOC_LITERAL(19, 255, 7), // "dwPhase"
QT_MOC_LITERAL(20, 263, 13), // "getComboIndex"
QT_MOC_LITERAL(21, 277, 7), // "dwIndex"
QT_MOC_LITERAL(22, 285, 12), // "saveUserData"
QT_MOC_LITERAL(23, 298, 17), // "setCheckboxStatus"
QT_MOC_LITERAL(24, 316, 9), // "isChecked"
QT_MOC_LITERAL(25, 326, 12), // "setAxisIndex"
QT_MOC_LITERAL(26, 339, 14), // "isLeftSelected"
QT_MOC_LITERAL(27, 354, 24), // "setSeekCheckBoxModelData"
QT_MOC_LITERAL(28, 379, 20), // "setSeekDefaultValues"
QT_MOC_LITERAL(29, 400, 20) // "getSeekDefaultValues"

    },
    "SeekRecipeData\0updateSeekRecipeParams\0"
    "\0sendSeekRecipeDataToSc\0modifyParameter\0"
    "a_paramValue\0a_paramIndex\0cancelUserData\0"
    "resetSeekRecipeData\0getPowerupLoadFlag\0"
    "getParameterChangedFlag\0getSeekPsFreq\0"
    "init\0rangeCheck\0setParamValues\0dwAmp\0"
    "dwFreq\0dwPower\0dwCurr\0dwPhase\0"
    "getComboIndex\0dwIndex\0saveUserData\0"
    "setCheckboxStatus\0isChecked\0setAxisIndex\0"
    "isLeftSelected\0setSeekCheckBoxModelData\0"
    "setSeekDefaultValues\0getSeekDefaultValues"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SeekRecipeData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       3,    0,  105,    2, 0x02 /* Public */,
       4,    2,  106,    2, 0x02 /* Public */,
       7,    0,  111,    2, 0x02 /* Public */,
       8,    0,  112,    2, 0x02 /* Public */,
       9,    0,  113,    2, 0x02 /* Public */,
      10,    0,  114,    2, 0x02 /* Public */,
      11,    0,  115,    2, 0x02 /* Public */,
      12,    0,  116,    2, 0x02 /* Public */,
      13,    2,  117,    2, 0x02 /* Public */,
      14,    5,  122,    2, 0x02 /* Public */,
      20,    1,  133,    2, 0x02 /* Public */,
      22,    0,  136,    2, 0x02 /* Public */,
      23,    2,  137,    2, 0x02 /* Public */,
      25,    2,  142,    2, 0x02 /* Public */,
      27,    0,  147,    2, 0x02 /* Public */,
      28,    0,  148,    2, 0x02 /* Public */,
      29,    0,  149,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // methods: parameters
    QMetaType::Int,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::UInt,
    QMetaType::Int,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   15,   16,   17,   18,   19,
    QMetaType::Int, QMetaType::Int,   21,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   21,   24,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   21,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SeekRecipeData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SeekRecipeData *_t = static_cast<SeekRecipeData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateSeekRecipeParams(); break;
        case 1: { int _r = _t->sendSeekRecipeDataToSc();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->modifyParameter((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->cancelUserData(); break;
        case 4: _t->resetSeekRecipeData(); break;
        case 5: { bool _r = _t->getPowerupLoadFlag();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->getParameterChangedFlag();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { uint _r = _t->getSeekPsFreq();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = std::move(_r); }  break;
        case 8: { int _r = _t->init();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { QString _r = _t->rangeCheck((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->setParamValues((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 11: { int _r = _t->getComboIndex((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->saveUserData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 13: _t->setCheckboxStatus((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 14: _t->setAxisIndex((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->setSeekCheckBoxModelData(); break;
        case 16: _t->setSeekDefaultValues(); break;
        case 17: _t->getSeekDefaultValues(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SeekRecipeData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekRecipeData::updateSeekRecipeParams)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject SeekRecipeData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SeekRecipeData.data,
      qt_meta_data_SeekRecipeData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SeekRecipeData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SeekRecipeData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SeekRecipeData.stringdata0))
        return static_cast<void*>(const_cast< SeekRecipeData*>(this));
    return QObject::qt_metacast(_clname);
}

int SeekRecipeData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void SeekRecipeData::updateSeekRecipeParams()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
