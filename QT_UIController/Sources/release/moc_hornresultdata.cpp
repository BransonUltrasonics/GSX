/****************************************************************************
** Meta object code from reading C++ file 'hornresultdata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/hornresultdata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hornresultdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HornResonantPoints_t {
    QByteArrayData data[8];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HornResonantPoints_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HornResonantPoints_t qt_meta_stringdata_HornResonantPoints = {
    {
QT_MOC_LITERAL(0, 0, 18), // "HornResonantPoints"
QT_MOC_LITERAL(1, 19, 16), // "labelNameChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 15), // "xAxisPosChanged"
QT_MOC_LITERAL(4, 53, 16), // "custColorChanged"
QT_MOC_LITERAL(5, 70, 9), // "labelName"
QT_MOC_LITERAL(6, 80, 8), // "xAxisPos"
QT_MOC_LITERAL(7, 89, 9) // "custColor"

    },
    "HornResonantPoints\0labelNameChanged\0"
    "\0xAxisPosChanged\0custColorChanged\0"
    "labelName\0xAxisPos\0custColor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HornResonantPoints[] = {

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
       6, QMetaType::Int, 0x00495001,
       7, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

void HornResonantPoints::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HornResonantPoints *_t = static_cast<HornResonantPoints *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->labelNameChanged(); break;
        case 1: _t->xAxisPosChanged(); break;
        case 2: _t->custColorChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HornResonantPoints::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornResonantPoints::labelNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (HornResonantPoints::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornResonantPoints::xAxisPosChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (HornResonantPoints::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornResonantPoints::custColorChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        HornResonantPoints *_t = static_cast<HornResonantPoints *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getLabelName(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->getXAxisPos(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getCustColor(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject HornResonantPoints::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HornResonantPoints.data,
      qt_meta_data_HornResonantPoints,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HornResonantPoints::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HornResonantPoints::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HornResonantPoints.stringdata0))
        return static_cast<void*>(const_cast< HornResonantPoints*>(this));
    return QObject::qt_metacast(_clname);
}

int HornResonantPoints::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void HornResonantPoints::labelNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HornResonantPoints::xAxisPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HornResonantPoints::custColorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_HornResultData_t {
    QByteArrayData data[4];
    char stringdata0[47];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HornResultData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HornResultData_t qt_meta_stringdata_HornResultData = {
    {
QT_MOC_LITERAL(0, 0, 14), // "HornResultData"
QT_MOC_LITERAL(1, 15, 18), // "getHornResultDbRes"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 11) // "a_historyId"

    },
    "HornResultData\0getHornResultDbRes\0\0"
    "a_historyId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HornResultData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Int, QMetaType::QString,    3,

       0        // eod
};

void HornResultData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HornResultData *_t = static_cast<HornResultData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->getHornResultDbRes((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject HornResultData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HornResultData.data,
      qt_meta_data_HornResultData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HornResultData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HornResultData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HornResultData.stringdata0))
        return static_cast<void*>(const_cast< HornResultData*>(this));
    return QObject::qt_metacast(_clname);
}

int HornResultData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
