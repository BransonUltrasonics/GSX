/****************************************************************************
** Meta object code from reading C++ file 'testmodecheckboxdata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/testmodecheckboxdata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'testmodecheckboxdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TestModeCheckBoxData_t {
    QByteArrayData data[17];
    char stringdata0[207];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TestModeCheckBoxData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TestModeCheckBoxData_t qt_meta_stringdata_TestModeCheckBoxData = {
    {
QT_MOC_LITERAL(0, 0, 20), // "TestModeCheckBoxData"
QT_MOC_LITERAL(1, 21, 16), // "setCheckBoxColor"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 8), // "boxColor"
QT_MOC_LITERAL(4, 48, 17), // "setCheckBoxStatus"
QT_MOC_LITERAL(5, 66, 9), // "boxStatus"
QT_MOC_LITERAL(6, 76, 12), // "setParamName"
QT_MOC_LITERAL(7, 89, 9), // "paramName"
QT_MOC_LITERAL(8, 99, 13), // "setParamValue"
QT_MOC_LITERAL(9, 113, 10), // "paramValue"
QT_MOC_LITERAL(10, 124, 12), // "setParamUnit"
QT_MOC_LITERAL(11, 137, 9), // "paramUnit"
QT_MOC_LITERAL(12, 147, 13), // "CheckBoxColor"
QT_MOC_LITERAL(13, 161, 14), // "CheckBoxStatus"
QT_MOC_LITERAL(14, 176, 9), // "ParamName"
QT_MOC_LITERAL(15, 186, 10), // "ParamValue"
QT_MOC_LITERAL(16, 197, 9) // "ParamUnit"

    },
    "TestModeCheckBoxData\0setCheckBoxColor\0"
    "\0boxColor\0setCheckBoxStatus\0boxStatus\0"
    "setParamName\0paramName\0setParamValue\0"
    "paramValue\0setParamUnit\0paramUnit\0"
    "CheckBoxColor\0CheckBoxStatus\0ParamName\0"
    "ParamValue\0ParamUnit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TestModeCheckBoxData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       5,   54, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       4,    1,   42,    2, 0x0a /* Public */,
       6,    1,   45,    2, 0x0a /* Public */,
       8,    1,   48,    2, 0x0a /* Public */,
      10,    1,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   11,

 // properties: name, type, flags
      12, QMetaType::QString, 0x00095103,
      13, QMetaType::Bool, 0x00095103,
      14, QMetaType::QString, 0x00095103,
      15, QMetaType::QString, 0x00095103,
      16, QMetaType::QString, 0x00095103,

       0        // eod
};

void TestModeCheckBoxData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TestModeCheckBoxData *_t = static_cast<TestModeCheckBoxData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setCheckBoxColor((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->setCheckBoxStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setParamName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->setParamValue((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->setParamUnit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TestModeCheckBoxData *_t = static_cast<TestModeCheckBoxData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getCheckBoxColor(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->getCheckBoxStatus(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getParamName(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getParamValue(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getParamUnit(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        TestModeCheckBoxData *_t = static_cast<TestModeCheckBoxData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setCheckBoxColor(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setCheckBoxStatus(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setParamName(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setParamValue(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setParamUnit(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject TestModeCheckBoxData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TestModeCheckBoxData.data,
      qt_meta_data_TestModeCheckBoxData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TestModeCheckBoxData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestModeCheckBoxData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TestModeCheckBoxData.stringdata0))
        return static_cast<void*>(const_cast< TestModeCheckBoxData*>(this));
    return QObject::qt_metacast(_clname);
}

int TestModeCheckBoxData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
