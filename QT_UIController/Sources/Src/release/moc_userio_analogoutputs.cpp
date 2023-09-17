/****************************************************************************
** Meta object code from reading C++ file 'userio_analogoutputs.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/userio_analogoutputs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userio_analogoutputs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AnalogOutputs_t {
    QByteArrayData data[7];
    char stringdata0[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AnalogOutputs_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AnalogOutputs_t qt_meta_stringdata_AnalogOutputs = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AnalogOutputs"
QT_MOC_LITERAL(1, 14, 28), // "updateAnalogOutputsModelList"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 15), // "a_ListViewIndex"
QT_MOC_LITERAL(4, 60, 15), // "a_ComboboxIndex"
QT_MOC_LITERAL(5, 76, 15), // "a_CheckBoxState"
QT_MOC_LITERAL(6, 92, 37) // "setDefaultValuesForAnalogOutp..."

    },
    "AnalogOutputs\0updateAnalogOutputsModelList\0"
    "\0a_ListViewIndex\0a_ComboboxIndex\0"
    "a_CheckBoxState\0setDefaultValuesForAnalogOutputParams"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AnalogOutputs[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x02 /* Public */,
       6,    0,   31,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void,

       0        // eod
};

void AnalogOutputs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AnalogOutputs *_t = static_cast<AnalogOutputs *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateAnalogOutputsModelList((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->setDefaultValuesForAnalogOutputParams(); break;
        default: ;
        }
    }
}

const QMetaObject AnalogOutputs::staticMetaObject = {
    { &AnalogIO::staticMetaObject, qt_meta_stringdata_AnalogOutputs.data,
      qt_meta_data_AnalogOutputs,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AnalogOutputs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnalogOutputs::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AnalogOutputs.stringdata0))
        return static_cast<void*>(const_cast< AnalogOutputs*>(this));
    return AnalogIO::qt_metacast(_clname);
}

int AnalogOutputs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AnalogIO::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
