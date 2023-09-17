/****************************************************************************
** Meta object code from reading C++ file 'hornrecipedata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/hornrecipedata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hornrecipedata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HornRecipeParameter_t {
    QByteArrayData data[18];
    char stringdata0[349];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HornRecipeParameter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HornRecipeParameter_t qt_meta_stringdata_HornRecipeParameter = {
    {
QT_MOC_LITERAL(0, 0, 19), // "HornRecipeParameter"
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
    "HornRecipeParameter\0parameterNameChanged\0"
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

static const uint qt_meta_data_HornRecipeParameter[] = {

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

void HornRecipeParameter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HornRecipeParameter *_t = static_cast<HornRecipeParameter *>(_o);
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
            typedef void (HornRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornRecipeParameter::parameterNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (HornRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornRecipeParameter::parameterLocaleNameChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (HornRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornRecipeParameter::parameterValueChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (HornRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornRecipeParameter::parameterUnitChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (HornRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornRecipeParameter::parameterMinValueChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (HornRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornRecipeParameter::parameterMaxValueChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (HornRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornRecipeParameter::ParameterSwitchChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (HornRecipeParameter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornRecipeParameter::ParameterSwitchStateChanged)) {
                *result = 7;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        HornRecipeParameter *_t = static_cast<HornRecipeParameter *>(_o);
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

const QMetaObject HornRecipeParameter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HornRecipeParameter.data,
      qt_meta_data_HornRecipeParameter,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HornRecipeParameter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HornRecipeParameter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HornRecipeParameter.stringdata0))
        return static_cast<void*>(const_cast< HornRecipeParameter*>(this));
    return QObject::qt_metacast(_clname);
}

int HornRecipeParameter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void HornRecipeParameter::parameterNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HornRecipeParameter::parameterLocaleNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HornRecipeParameter::parameterValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void HornRecipeParameter::parameterUnitChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void HornRecipeParameter::parameterMinValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void HornRecipeParameter::parameterMaxValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void HornRecipeParameter::ParameterSwitchChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void HornRecipeParameter::ParameterSwitchStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
struct qt_meta_stringdata_HornRecipeData_t {
    QByteArrayData data[36];
    char stringdata0[614];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HornRecipeData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HornRecipeData_t qt_meta_stringdata_HornRecipeData = {
    {
QT_MOC_LITERAL(0, 0, 14), // "HornRecipeData"
QT_MOC_LITERAL(1, 15, 22), // "updateHornRecipeParams"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 32), // "HornAutoGraphRefreshStateChanged"
QT_MOC_LITERAL(4, 72, 31), // "IsHornManualRefreshStateChanged"
QT_MOC_LITERAL(5, 104, 28), // "setHornAutoGraphRefreshState"
QT_MOC_LITERAL(6, 133, 27), // "setIsHornManualRefreshState"
QT_MOC_LITERAL(7, 161, 20), // "setHornDefaultValues"
QT_MOC_LITERAL(8, 182, 20), // "getHornDefaultValues"
QT_MOC_LITERAL(9, 203, 15), // "modifyParameter"
QT_MOC_LITERAL(10, 219, 12), // "a_paramValue"
QT_MOC_LITERAL(11, 232, 12), // "a_paramIndex"
QT_MOC_LITERAL(12, 245, 14), // "cancelUserData"
QT_MOC_LITERAL(13, 260, 19), // "resetHornRecipeData"
QT_MOC_LITERAL(14, 280, 12), // "setAxisIndex"
QT_MOC_LITERAL(15, 293, 7), // "dwIndex"
QT_MOC_LITERAL(16, 301, 14), // "isLeftSelected"
QT_MOC_LITERAL(17, 316, 17), // "setCheckboxStatus"
QT_MOC_LITERAL(18, 334, 9), // "isChecked"
QT_MOC_LITERAL(19, 344, 18), // "updateParameterVal"
QT_MOC_LITERAL(20, 363, 3), // "amp"
QT_MOC_LITERAL(21, 367, 4), // "curr"
QT_MOC_LITERAL(22, 372, 5), // "phase"
QT_MOC_LITERAL(23, 378, 9), // "impedence"
QT_MOC_LITERAL(24, 388, 18), // "getPowerupLoadFlag"
QT_MOC_LITERAL(25, 407, 23), // "getParameterChangedFlag"
QT_MOC_LITERAL(26, 431, 13), // "getComboIndex"
QT_MOC_LITERAL(27, 445, 28), // "getHornAutoGraphRefreshState"
QT_MOC_LITERAL(28, 474, 27), // "getIsHornManualRefreshState"
QT_MOC_LITERAL(29, 502, 13), // "getHornPsFreq"
QT_MOC_LITERAL(30, 516, 4), // "init"
QT_MOC_LITERAL(31, 521, 12), // "saveUserData"
QT_MOC_LITERAL(32, 534, 22), // "sendHornRecipeDataToSc"
QT_MOC_LITERAL(33, 557, 10), // "rangeCheck"
QT_MOC_LITERAL(34, 568, 25), // "autoHornGraphRefreshState"
QT_MOC_LITERAL(35, 594, 19) // "isHornManualRefresh"

    },
    "HornRecipeData\0updateHornRecipeParams\0"
    "\0HornAutoGraphRefreshStateChanged\0"
    "IsHornManualRefreshStateChanged\0"
    "setHornAutoGraphRefreshState\0"
    "setIsHornManualRefreshState\0"
    "setHornDefaultValues\0getHornDefaultValues\0"
    "modifyParameter\0a_paramValue\0a_paramIndex\0"
    "cancelUserData\0resetHornRecipeData\0"
    "setAxisIndex\0dwIndex\0isLeftSelected\0"
    "setCheckboxStatus\0isChecked\0"
    "updateParameterVal\0amp\0curr\0phase\0"
    "impedence\0getPowerupLoadFlag\0"
    "getParameterChangedFlag\0getComboIndex\0"
    "getHornAutoGraphRefreshState\0"
    "getIsHornManualRefreshState\0getHornPsFreq\0"
    "init\0saveUserData\0sendHornRecipeDataToSc\0"
    "rangeCheck\0autoHornGraphRefreshState\0"
    "isHornManualRefresh"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HornRecipeData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       2,  182, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  129,    2, 0x06 /* Public */,
       3,    0,  130,    2, 0x06 /* Public */,
       4,    0,  131,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,  132,    2, 0x0a /* Public */,
       6,    1,  135,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       7,    0,  138,    2, 0x02 /* Public */,
       8,    0,  139,    2, 0x02 /* Public */,
       9,    2,  140,    2, 0x02 /* Public */,
      12,    0,  145,    2, 0x02 /* Public */,
      13,    0,  146,    2, 0x02 /* Public */,
      14,    2,  147,    2, 0x02 /* Public */,
      17,    2,  152,    2, 0x02 /* Public */,
      19,    4,  157,    2, 0x02 /* Public */,
      24,    0,  166,    2, 0x02 /* Public */,
      25,    0,  167,    2, 0x02 /* Public */,
      26,    1,  168,    2, 0x02 /* Public */,
      27,    0,  171,    2, 0x02 /* Public */,
      28,    0,  172,    2, 0x02 /* Public */,
      29,    0,  173,    2, 0x02 /* Public */,
      30,    0,  174,    2, 0x02 /* Public */,
      31,    0,  175,    2, 0x02 /* Public */,
      32,    0,  176,    2, 0x02 /* Public */,
      33,    2,  177,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   15,   16,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   15,   18,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   20,   21,   22,   23,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::Int,   15,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::UInt,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,   10,   11,

 // properties: name, type, flags
      34, QMetaType::Bool, 0x00495003,
      35, QMetaType::Bool, 0x00495003,

 // properties: notify_signal_id
       1,
       2,

       0        // eod
};

void HornRecipeData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HornRecipeData *_t = static_cast<HornRecipeData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateHornRecipeParams(); break;
        case 1: _t->HornAutoGraphRefreshStateChanged(); break;
        case 2: _t->IsHornManualRefreshStateChanged(); break;
        case 3: _t->setHornAutoGraphRefreshState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setIsHornManualRefreshState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setHornDefaultValues(); break;
        case 6: _t->getHornDefaultValues(); break;
        case 7: _t->modifyParameter((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 8: _t->cancelUserData(); break;
        case 9: _t->resetHornRecipeData(); break;
        case 10: _t->setAxisIndex((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->setCheckboxStatus((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 12: _t->updateParameterVal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 13: { bool _r = _t->getPowerupLoadFlag();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 14: { bool _r = _t->getParameterChangedFlag();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 15: { bool _r = _t->getComboIndex((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 16: { bool _r = _t->getHornAutoGraphRefreshState();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 17: { bool _r = _t->getIsHornManualRefreshState();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 18: { uint _r = _t->getHornPsFreq();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = std::move(_r); }  break;
        case 19: { int _r = _t->init();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 20: { int _r = _t->saveUserData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 21: { int _r = _t->sendHornRecipeDataToSc();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 22: { QString _r = _t->rangeCheck((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HornRecipeData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornRecipeData::updateHornRecipeParams)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (HornRecipeData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornRecipeData::HornAutoGraphRefreshStateChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (HornRecipeData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornRecipeData::IsHornManualRefreshStateChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        HornRecipeData *_t = static_cast<HornRecipeData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->getHornAutoGraphRefreshState(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->getIsHornManualRefreshState(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        HornRecipeData *_t = static_cast<HornRecipeData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setHornAutoGraphRefreshState(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setIsHornManualRefreshState(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject HornRecipeData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HornRecipeData.data,
      qt_meta_data_HornRecipeData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HornRecipeData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HornRecipeData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HornRecipeData.stringdata0))
        return static_cast<void*>(const_cast< HornRecipeData*>(this));
    return QObject::qt_metacast(_clname);
}

int HornRecipeData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 23;
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
void HornRecipeData::updateHornRecipeParams()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HornRecipeData::HornAutoGraphRefreshStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HornRecipeData::IsHornManualRefreshStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
