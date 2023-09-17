/****************************************************************************
** Meta object code from reading C++ file 'testrecipe.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/testrecipe.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'testrecipe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TestRecipeData_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TestRecipeData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TestRecipeData_t qt_meta_stringdata_TestRecipeData = {
    {
QT_MOC_LITERAL(0, 0, 14) // "TestRecipeData"

    },
    "TestRecipeData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TestRecipeData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void TestRecipeData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject TestRecipeData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TestRecipeData.data,
      qt_meta_data_TestRecipeData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TestRecipeData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestRecipeData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TestRecipeData.stringdata0))
        return static_cast<void*>(const_cast< TestRecipeData*>(this));
    return QObject::qt_metacast(_clname);
}

int TestRecipeData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_TestRecipeModel_t {
    QByteArrayData data[14];
    char stringdata0[257];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TestRecipeModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TestRecipeModel_t qt_meta_stringdata_TestRecipeModel = {
    {
QT_MOC_LITERAL(0, 0, 15), // "TestRecipeModel"
QT_MOC_LITERAL(1, 16, 20), // "parameterNameChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 26), // "parameterLocaleNameChanged"
QT_MOC_LITERAL(4, 65, 21), // "parameterValueChanged"
QT_MOC_LITERAL(5, 87, 20), // "parameterUnitChanged"
QT_MOC_LITERAL(6, 108, 24), // "parameterMinValueChanged"
QT_MOC_LITERAL(7, 133, 24), // "parameterMaxValueChanged"
QT_MOC_LITERAL(8, 158, 13), // "parameterName"
QT_MOC_LITERAL(9, 172, 19), // "parameterLocaleName"
QT_MOC_LITERAL(10, 192, 14), // "parameterValue"
QT_MOC_LITERAL(11, 207, 13), // "parameterUnit"
QT_MOC_LITERAL(12, 221, 17), // "parameterMinValue"
QT_MOC_LITERAL(13, 239, 17) // "parameterMaxValue"

    },
    "TestRecipeModel\0parameterNameChanged\0"
    "\0parameterLocaleNameChanged\0"
    "parameterValueChanged\0parameterUnitChanged\0"
    "parameterMinValueChanged\0"
    "parameterMaxValueChanged\0parameterName\0"
    "parameterLocaleName\0parameterValue\0"
    "parameterUnit\0parameterMinValue\0"
    "parameterMaxValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TestRecipeModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       6,   50, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    0,   46,    2, 0x06 /* Public */,
       5,    0,   47,    2, 0x06 /* Public */,
       6,    0,   48,    2, 0x06 /* Public */,
       7,    0,   49,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       8, QMetaType::QString, 0x00495001,
       9, QMetaType::QString, 0x00495001,
      10, QMetaType::Int, 0x00495001,
      11, QMetaType::QString, 0x00495001,
      12, QMetaType::QString, 0x00495001,
      13, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,

       0        // eod
};

void TestRecipeModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TestRecipeModel *_t = static_cast<TestRecipeModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->parameterNameChanged(); break;
        case 1: _t->parameterLocaleNameChanged(); break;
        case 2: _t->parameterValueChanged(); break;
        case 3: _t->parameterUnitChanged(); break;
        case 4: _t->parameterMinValueChanged(); break;
        case 5: _t->parameterMaxValueChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TestRecipeModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TestRecipeModel::parameterNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TestRecipeModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TestRecipeModel::parameterLocaleNameChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TestRecipeModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TestRecipeModel::parameterValueChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (TestRecipeModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TestRecipeModel::parameterUnitChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (TestRecipeModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TestRecipeModel::parameterMinValueChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (TestRecipeModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TestRecipeModel::parameterMaxValueChanged)) {
                *result = 5;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TestRecipeModel *_t = static_cast<TestRecipeModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getParameterName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getParameterLocaleName(); break;
        case 2: *reinterpret_cast< qint32*>(_v) = _t->getParameterValue(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getParameterUnit(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getParameterMinValue(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->getParameterMaxValue(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject TestRecipeModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TestRecipeModel.data,
      qt_meta_data_TestRecipeModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TestRecipeModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestRecipeModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TestRecipeModel.stringdata0))
        return static_cast<void*>(const_cast< TestRecipeModel*>(this));
    return QObject::qt_metacast(_clname);
}

int TestRecipeModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TestRecipeModel::parameterNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TestRecipeModel::parameterLocaleNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TestRecipeModel::parameterValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TestRecipeModel::parameterUnitChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TestRecipeModel::parameterMinValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TestRecipeModel::parameterMaxValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
