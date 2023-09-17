/****************************************************************************
** Meta object code from reading C++ file 'WeldGraph.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/WeldGraph.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WeldGraph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WeldResonantPoints_t {
    QByteArrayData data[6];
    char stringdata0[72];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WeldResonantPoints_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WeldResonantPoints_t qt_meta_stringdata_WeldResonantPoints = {
    {
QT_MOC_LITERAL(0, 0, 18), // "WeldResonantPoints"
QT_MOC_LITERAL(1, 19, 16), // "labelNameChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 15), // "xAxisPosChanged"
QT_MOC_LITERAL(4, 53, 9), // "labelName"
QT_MOC_LITERAL(5, 63, 8) // "xAxisPos"

    },
    "WeldResonantPoints\0labelNameChanged\0"
    "\0xAxisPosChanged\0labelName\0xAxisPos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WeldResonantPoints[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       2,   26, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       4, QMetaType::QString, 0x00495001,
       5, QMetaType::Double, 0x00495001,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void WeldResonantPoints::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WeldResonantPoints *_t = static_cast<WeldResonantPoints *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->labelNameChanged(); break;
        case 1: _t->xAxisPosChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (WeldResonantPoints::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WeldResonantPoints::labelNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (WeldResonantPoints::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WeldResonantPoints::xAxisPosChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        WeldResonantPoints *_t = static_cast<WeldResonantPoints *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getLabelName(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->getXAxisPos(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject WeldResonantPoints::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WeldResonantPoints.data,
      qt_meta_data_WeldResonantPoints,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *WeldResonantPoints::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WeldResonantPoints::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WeldResonantPoints.stringdata0))
        return static_cast<void*>(const_cast< WeldResonantPoints*>(this));
    return QObject::qt_metacast(_clname);
}

int WeldResonantPoints::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void WeldResonantPoints::labelNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void WeldResonantPoints::xAxisPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_WeldGraphData_t {
    QByteArrayData data[21];
    char stringdata0[303];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WeldGraphData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WeldGraphData_t qt_meta_stringdata_WeldGraphData = {
    {
QT_MOC_LITERAL(0, 0, 13), // "WeldGraphData"
QT_MOC_LITERAL(1, 14, 13), // "appendSamples"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 15), // "CustomXYSeries*"
QT_MOC_LITERAL(4, 45, 8), // "a_series"
QT_MOC_LITERAL(5, 54, 6), // "a_type"
QT_MOC_LITERAL(6, 61, 20), // "receiveWeldGraphData"
QT_MOC_LITERAL(7, 82, 14), // "a_weldResultId"
QT_MOC_LITERAL(8, 97, 17), // "receiveWeldPoints"
QT_MOC_LITERAL(9, 115, 20), // "setAxisMinParameters"
QT_MOC_LITERAL(10, 136, 13), // "QList<qreal>&"
QT_MOC_LITERAL(11, 150, 9), // "a_axisVal"
QT_MOC_LITERAL(12, 160, 20), // "setAxisMaxParameters"
QT_MOC_LITERAL(13, 181, 20), // "getAxisMinParameters"
QT_MOC_LITERAL(14, 202, 12), // "QList<qreal>"
QT_MOC_LITERAL(15, 215, 20), // "getAxisMaxParameters"
QT_MOC_LITERAL(16, 236, 8), // "getCount"
QT_MOC_LITERAL(17, 245, 10), // "clearGraph"
QT_MOC_LITERAL(18, 256, 15), // "GetClosestIndex"
QT_MOC_LITERAL(19, 272, 14), // "qstrXAxisValue"
QT_MOC_LITERAL(20, 287, 15) // "getTriggerPoint"

    },
    "WeldGraphData\0appendSamples\0\0"
    "CustomXYSeries*\0a_series\0a_type\0"
    "receiveWeldGraphData\0a_weldResultId\0"
    "receiveWeldPoints\0setAxisMinParameters\0"
    "QList<qreal>&\0a_axisVal\0setAxisMaxParameters\0"
    "getAxisMinParameters\0QList<qreal>\0"
    "getAxisMaxParameters\0getCount\0clearGraph\0"
    "GetClosestIndex\0qstrXAxisValue\0"
    "getTriggerPoint"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WeldGraphData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x02 /* Public */,
       6,    1,   74,    2, 0x02 /* Public */,
       8,    1,   77,    2, 0x02 /* Public */,
       9,    1,   80,    2, 0x02 /* Public */,
      12,    1,   83,    2, 0x02 /* Public */,
      13,    0,   86,    2, 0x02 /* Public */,
      15,    0,   87,    2, 0x02 /* Public */,
      16,    0,   88,    2, 0x02 /* Public */,
      17,    0,   89,    2, 0x02 /* Public */,
      18,    1,   90,    2, 0x02 /* Public */,
      20,    0,   93,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Int, 0x80000000 | 3, QMetaType::UChar,    4,    5,
    QMetaType::Int, QMetaType::QString,    7,
    QMetaType::Int, QMetaType::QString,    7,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 10,   11,
    0x80000000 | 14,
    0x80000000 | 14,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Int, QMetaType::QString,   19,
    QMetaType::Double,

       0        // eod
};

void WeldGraphData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WeldGraphData *_t = static_cast<WeldGraphData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->appendSamples((*reinterpret_cast< CustomXYSeries*(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: { int _r = _t->receiveWeldGraphData((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 2: { int _r = _t->receiveWeldPoints((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->setAxisMinParameters((*reinterpret_cast< QList<qreal>(*)>(_a[1]))); break;
        case 4: _t->setAxisMaxParameters((*reinterpret_cast< QList<qreal>(*)>(_a[1]))); break;
        case 5: { QList<qreal> _r = _t->getAxisMinParameters();
            if (_a[0]) *reinterpret_cast< QList<qreal>*>(_a[0]) = std::move(_r); }  break;
        case 6: { QList<qreal> _r = _t->getAxisMaxParameters();
            if (_a[0]) *reinterpret_cast< QList<qreal>*>(_a[0]) = std::move(_r); }  break;
        case 7: { int _r = _t->getCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->clearGraph(); break;
        case 9: { int _r = _t->GetClosestIndex((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: { double _r = _t->getTriggerPoint();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CustomXYSeries* >(); break;
            }
            break;
        }
    }
}

const QMetaObject WeldGraphData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WeldGraphData.data,
      qt_meta_data_WeldGraphData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *WeldGraphData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WeldGraphData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WeldGraphData.stringdata0))
        return static_cast<void*>(const_cast< WeldGraphData*>(this));
    return QObject::qt_metacast(_clname);
}

int WeldGraphData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
