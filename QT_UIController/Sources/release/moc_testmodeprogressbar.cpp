/****************************************************************************
** Meta object code from reading C++ file 'testmodeprogressbar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/testmodeprogressbar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'testmodeprogressbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TestModeProgressBar_t {
    QByteArrayData data[14];
    char stringdata0[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TestModeProgressBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TestModeProgressBar_t qt_meta_stringdata_TestModeProgressBar = {
    {
QT_MOC_LITERAL(0, 0, 19), // "TestModeProgressBar"
QT_MOC_LITERAL(1, 20, 16), // "paramNameChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 17), // "paramValueChanged"
QT_MOC_LITERAL(4, 56, 16), // "paramUnitChanged"
QT_MOC_LITERAL(5, 73, 12), // "setParamName"
QT_MOC_LITERAL(6, 86, 9), // "paramName"
QT_MOC_LITERAL(7, 96, 13), // "setParamValue"
QT_MOC_LITERAL(8, 110, 10), // "paramValue"
QT_MOC_LITERAL(9, 121, 12), // "setParamUnit"
QT_MOC_LITERAL(10, 134, 9), // "paramUnit"
QT_MOC_LITERAL(11, 144, 9), // "ParamName"
QT_MOC_LITERAL(12, 154, 10), // "ParamValue"
QT_MOC_LITERAL(13, 165, 9) // "ParamUnit"

    },
    "TestModeProgressBar\0paramNameChanged\0"
    "\0paramValueChanged\0paramUnitChanged\0"
    "setParamName\0paramName\0setParamValue\0"
    "paramValue\0setParamUnit\0paramUnit\0"
    "ParamName\0ParamValue\0ParamUnit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TestModeProgressBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       3,   56, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    0,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   47,    2, 0x0a /* Public */,
       7,    1,   50,    2, 0x0a /* Public */,
       9,    1,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::QString,   10,

 // properties: name, type, flags
      11, QMetaType::QString, 0x00495103,
      12, QMetaType::Double, 0x00495103,
      13, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

void TestModeProgressBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TestModeProgressBar *_t = static_cast<TestModeProgressBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->paramNameChanged(); break;
        case 1: _t->paramValueChanged(); break;
        case 2: _t->paramUnitChanged(); break;
        case 3: _t->setParamName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->setParamValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->setParamUnit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TestModeProgressBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TestModeProgressBar::paramNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TestModeProgressBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TestModeProgressBar::paramValueChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TestModeProgressBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TestModeProgressBar::paramUnitChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TestModeProgressBar *_t = static_cast<TestModeProgressBar *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getParamName(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->getParamValue(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getParamUnit(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        TestModeProgressBar *_t = static_cast<TestModeProgressBar *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setParamName(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setParamValue(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setParamUnit(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject TestModeProgressBar::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TestModeProgressBar.data,
      qt_meta_data_TestModeProgressBar,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TestModeProgressBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestModeProgressBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TestModeProgressBar.stringdata0))
        return static_cast<void*>(const_cast< TestModeProgressBar*>(this));
    return QObject::qt_metacast(_clname);
}

int TestModeProgressBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void TestModeProgressBar::paramNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TestModeProgressBar::paramValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TestModeProgressBar::paramUnitChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
