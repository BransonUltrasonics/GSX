/****************************************************************************
** Meta object code from reading C++ file 'smartstack.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/smartstack.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'smartstack.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SmartStack_t {
    QByteArrayData data[16];
    char stringdata0[274];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SmartStack_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SmartStack_t qt_meta_stringdata_SmartStack = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SmartStack"
QT_MOC_LITERAL(1, 11, 20), // "ParameterNameChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 5), // "value"
QT_MOC_LITERAL(4, 39, 19), // "bIsTextFieldChanged"
QT_MOC_LITERAL(5, 59, 14), // "smartStackInit"
QT_MOC_LITERAL(6, 74, 29), // "getIsChangeStackClickedStatus"
QT_MOC_LITERAL(7, 104, 22), // "sendClearMemoryReqToSC"
QT_MOC_LITERAL(8, 127, 22), // "sendChangeStackReqToSC"
QT_MOC_LITERAL(9, 150, 22), // "sendCancelStackReqToSC"
QT_MOC_LITERAL(10, 173, 14), // "changeStackInd"
QT_MOC_LITERAL(11, 188, 13), // "strSmartStack"
QT_MOC_LITERAL(12, 202, 17), // "clearNewStackInfo"
QT_MOC_LITERAL(13, 220, 26), // "copyNewStackToCurrentStack"
QT_MOC_LITERAL(14, 247, 13), // "ParameterName"
QT_MOC_LITERAL(15, 261, 12) // "bIsTextField"

    },
    "SmartStack\0ParameterNameChanged\0\0value\0"
    "bIsTextFieldChanged\0smartStackInit\0"
    "getIsChangeStackClickedStatus\0"
    "sendClearMemoryReqToSC\0sendChangeStackReqToSC\0"
    "sendCancelStackReqToSC\0changeStackInd\0"
    "strSmartStack\0clearNewStackInfo\0"
    "copyNewStackToCurrentStack\0ParameterName\0"
    "bIsTextField"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SmartStack[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       2,   80, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       5,    0,   70,    2, 0x02 /* Public */,
       6,    0,   71,    2, 0x02 /* Public */,
       7,    0,   72,    2, 0x02 /* Public */,
       8,    0,   73,    2, 0x02 /* Public */,
       9,    0,   74,    2, 0x02 /* Public */,
      10,    1,   75,    2, 0x02 /* Public */,
      12,    0,   78,    2, 0x02 /* Public */,
      13,    0,   79,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    3,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      14, QMetaType::QString, 0x00495003,
      15, QMetaType::Bool, 0x00495003,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void SmartStack::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SmartStack *_t = static_cast<SmartStack *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ParameterNameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->bIsTextFieldChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->smartStackInit(); break;
        case 3: { bool _r = _t->getIsChangeStackClickedStatus();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->sendClearMemoryReqToSC(); break;
        case 5: _t->sendChangeStackReqToSC(); break;
        case 6: _t->sendCancelStackReqToSC(); break;
        case 7: _t->changeStackInd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->clearNewStackInfo(); break;
        case 9: _t->copyNewStackToCurrentStack(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SmartStack::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SmartStack::ParameterNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SmartStack::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SmartStack::bIsTextFieldChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        SmartStack *_t = static_cast<SmartStack *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->ParameterName(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->bIsTextField(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        SmartStack *_t = static_cast<SmartStack *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->ParameterName(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->bIsTextField(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject SmartStack::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SmartStack.data,
      qt_meta_data_SmartStack,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SmartStack::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SmartStack::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SmartStack.stringdata0))
        return static_cast<void*>(const_cast< SmartStack*>(this));
    return QObject::qt_metacast(_clname);
}

int SmartStack::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void SmartStack::ParameterNameChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SmartStack::bIsTextFieldChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
