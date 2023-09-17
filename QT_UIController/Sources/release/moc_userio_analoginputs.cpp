/****************************************************************************
** Meta object code from reading C++ file 'userio_analoginputs.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/userio_analoginputs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userio_analoginputs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AnalogInputs_t {
    QByteArrayData data[9];
    char stringdata0[150];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AnalogInputs_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AnalogInputs_t qt_meta_stringdata_AnalogInputs = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AnalogInputs"
QT_MOC_LITERAL(1, 13, 27), // "updateAnalogInputsModelList"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 15), // "a_ListViewIndex"
QT_MOC_LITERAL(4, 58, 15), // "a_ComboboxIndex"
QT_MOC_LITERAL(5, 74, 15), // "a_CheckBoxState"
QT_MOC_LITERAL(6, 90, 36), // "setDefaultValuesForAnalogInpu..."
QT_MOC_LITERAL(7, 127, 11), // "onUnchecked"
QT_MOC_LITERAL(8, 139, 10) // "onChecking"

    },
    "AnalogInputs\0updateAnalogInputsModelList\0"
    "\0a_ListViewIndex\0a_ComboboxIndex\0"
    "a_CheckBoxState\0setDefaultValuesForAnalogInputParams\0"
    "onUnchecked\0onChecking"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AnalogInputs[] = {

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
       1,    3,   34,    2, 0x02 /* Public */,
       6,    0,   41,    2, 0x02 /* Public */,
       7,    3,   42,    2, 0x02 /* Public */,
       8,    3,   49,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    3,    4,    5,

       0        // eod
};

void AnalogInputs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AnalogInputs *_t = static_cast<AnalogInputs *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateAnalogInputsModelList((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->setDefaultValuesForAnalogInputParams(); break;
        case 2: _t->onUnchecked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 3: _t->onChecking((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject AnalogInputs::staticMetaObject = {
    { &AnalogIO::staticMetaObject, qt_meta_stringdata_AnalogInputs.data,
      qt_meta_data_AnalogInputs,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AnalogInputs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnalogInputs::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AnalogInputs.stringdata0))
        return static_cast<void*>(const_cast< AnalogInputs*>(this));
    return AnalogIO::qt_metacast(_clname);
}

int AnalogInputs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AnalogIO::qt_metacall(_c, _id, _a);
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
