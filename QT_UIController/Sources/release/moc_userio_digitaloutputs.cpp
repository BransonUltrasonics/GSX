/****************************************************************************
** Meta object code from reading C++ file 'userio_digitaloutputs.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/userio_digitaloutputs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userio_digitaloutputs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DigitalOutputs_t {
    QByteArrayData data[10];
    char stringdata0[171];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DigitalOutputs_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DigitalOutputs_t qt_meta_stringdata_DigitalOutputs = {
    {
QT_MOC_LITERAL(0, 0, 14), // "DigitalOutputs"
QT_MOC_LITERAL(1, 15, 38), // "setDefaultValuesForDigitalOut..."
QT_MOC_LITERAL(2, 54, 0), // ""
QT_MOC_LITERAL(3, 55, 29), // "updateDigitalOutputsModelList"
QT_MOC_LITERAL(4, 85, 15), // "a_ListViewIndex"
QT_MOC_LITERAL(5, 101, 15), // "a_ComboboxIndex"
QT_MOC_LITERAL(6, 117, 15), // "a_CheckBoxState"
QT_MOC_LITERAL(7, 133, 14), // "a_ActiveStatus"
QT_MOC_LITERAL(8, 148, 11), // "onUnchecked"
QT_MOC_LITERAL(9, 160, 10) // "onChecking"

    },
    "DigitalOutputs\0setDefaultValuesForDigitalOutputParams\0"
    "\0updateDigitalOutputsModelList\0"
    "a_ListViewIndex\0a_ComboboxIndex\0"
    "a_CheckBoxState\0a_ActiveStatus\0"
    "onUnchecked\0onChecking"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DigitalOutputs[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x02 /* Public */,
       3,    4,   35,    2, 0x02 /* Public */,
       8,    4,   44,    2, 0x02 /* Public */,
       9,    4,   53,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool, QMetaType::Bool,    4,    5,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool, QMetaType::Bool,    4,    5,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool, QMetaType::Bool,    4,    5,    6,    7,

       0        // eod
};

void DigitalOutputs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DigitalOutputs *_t = static_cast<DigitalOutputs *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setDefaultValuesForDigitalOutputParams(); break;
        case 1: _t->updateDigitalOutputsModelList((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 2: _t->onUnchecked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 3: _t->onChecking((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObject DigitalOutputs::staticMetaObject = {
    { &DigitalIO::staticMetaObject, qt_meta_stringdata_DigitalOutputs.data,
      qt_meta_data_DigitalOutputs,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DigitalOutputs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DigitalOutputs::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DigitalOutputs.stringdata0))
        return static_cast<void*>(const_cast< DigitalOutputs*>(this));
    return DigitalIO::qt_metacast(_clname);
}

int DigitalOutputs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DigitalIO::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
