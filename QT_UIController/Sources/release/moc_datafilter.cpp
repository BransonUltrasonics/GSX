/****************************************************************************
** Meta object code from reading C++ file 'datafilter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/datafilter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datafilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataFilter_t {
    QByteArrayData data[22];
    char stringdata0[415];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataFilter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataFilter_t qt_meta_stringdata_DataFilter = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DataFilter"
QT_MOC_LITERAL(1, 11, 25), // "selectedFilterNameChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 25), // "selectedFilterListChanged"
QT_MOC_LITERAL(4, 64, 26), // "selectedFilterIndexChanged"
QT_MOC_LITERAL(5, 91, 19), // "subTitleNameChanged"
QT_MOC_LITERAL(6, 111, 23), // "exportFilterNameChanged"
QT_MOC_LITERAL(7, 135, 23), // "deleteFilterNameChanged"
QT_MOC_LITERAL(8, 159, 21), // "getSelectedFilterName"
QT_MOC_LITERAL(9, 181, 21), // "getSelectedFilterList"
QT_MOC_LITERAL(10, 203, 16), // "getSelectedIndex"
QT_MOC_LITERAL(11, 220, 21), // "setSelectedFilterList"
QT_MOC_LITERAL(12, 242, 21), // "SelectedFrequencyType"
QT_MOC_LITERAL(13, 264, 21), // "setSelectedFilterName"
QT_MOC_LITERAL(14, 286, 18), // "SelectedReportType"
QT_MOC_LITERAL(15, 305, 16), // "setSelectedIndex"
QT_MOC_LITERAL(16, 322, 13), // "strFilterName"
QT_MOC_LITERAL(17, 336, 13), // "strFilterList"
QT_MOC_LITERAL(18, 350, 14), // "selected_index"
QT_MOC_LITERAL(19, 365, 15), // "strSubTitleName"
QT_MOC_LITERAL(20, 381, 16), // "exportFilterName"
QT_MOC_LITERAL(21, 398, 16) // "deleteFilterName"

    },
    "DataFilter\0selectedFilterNameChanged\0"
    "\0selectedFilterListChanged\0"
    "selectedFilterIndexChanged\0"
    "subTitleNameChanged\0exportFilterNameChanged\0"
    "deleteFilterNameChanged\0getSelectedFilterName\0"
    "getSelectedFilterList\0getSelectedIndex\0"
    "setSelectedFilterList\0SelectedFrequencyType\0"
    "setSelectedFilterName\0SelectedReportType\0"
    "setSelectedIndex\0strFilterName\0"
    "strFilterList\0selected_index\0"
    "strSubTitleName\0exportFilterName\0"
    "deleteFilterName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataFilter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       6,   92, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    0,   76,    2, 0x06 /* Public */,
       5,    0,   77,    2, 0x06 /* Public */,
       6,    0,   78,    2, 0x06 /* Public */,
       7,    0,   79,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       8,    0,   80,    2, 0x02 /* Public */,
       9,    0,   81,    2, 0x02 /* Public */,
      10,    0,   82,    2, 0x02 /* Public */,
      11,    1,   83,    2, 0x02 /* Public */,
      13,    1,   86,    2, 0x02 /* Public */,
      15,    1,   89,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::QString,
    QMetaType::QStringList,
    QMetaType::Int,
    QMetaType::Void, QMetaType::QStringList,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::Int,    2,

 // properties: name, type, flags
      16, QMetaType::QString, 0x00495003,
      17, QMetaType::QStringList, 0x00495003,
      18, QMetaType::Int, 0x00495003,
      19, QMetaType::QString, 0x00495003,
      20, QMetaType::Int, 0x00495103,
      21, QMetaType::Int, 0x00495003,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,

       0        // eod
};

void DataFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DataFilter *_t = static_cast<DataFilter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selectedFilterNameChanged(); break;
        case 1: _t->selectedFilterListChanged(); break;
        case 2: _t->selectedFilterIndexChanged(); break;
        case 3: _t->subTitleNameChanged(); break;
        case 4: _t->exportFilterNameChanged(); break;
        case 5: _t->deleteFilterNameChanged(); break;
        case 6: { QString _r = _t->getSelectedFilterName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 7: { QStringList _r = _t->getSelectedFilterList();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 8: { int _r = _t->getSelectedIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: _t->setSelectedFilterList((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 10: _t->setSelectedFilterName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->setSelectedIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DataFilter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataFilter::selectedFilterNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DataFilter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataFilter::selectedFilterListChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (DataFilter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataFilter::selectedFilterIndexChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (DataFilter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataFilter::subTitleNameChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (DataFilter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataFilter::exportFilterNameChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (DataFilter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataFilter::deleteFilterNameChanged)) {
                *result = 5;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        DataFilter *_t = static_cast<DataFilter *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getSelectedFilterName(); break;
        case 1: *reinterpret_cast< QStringList*>(_v) = _t->getSelectedFilterList(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->getSelectedIndex(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getSubTitleName(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->getexportFilterName(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->getdeleteFilterName(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        DataFilter *_t = static_cast<DataFilter *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSelectedFilterName(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setSelectedFilterList(*reinterpret_cast< QStringList*>(_v)); break;
        case 2: _t->setSelectedIndex(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setSubTitleName(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setExportFilterName(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setdeleteFilterName(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject DataFilter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataFilter.data,
      qt_meta_data_DataFilter,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DataFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataFilter.stringdata0))
        return static_cast<void*>(const_cast< DataFilter*>(this));
    return QObject::qt_metacast(_clname);
}

int DataFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
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
void DataFilter::selectedFilterNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DataFilter::selectedFilterListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DataFilter::selectedFilterIndexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void DataFilter::subTitleNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void DataFilter::exportFilterNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void DataFilter::deleteFilterNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
