/****************************************************************************
** Meta object code from reading C++ file 'seekscandata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/seekscandata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'seekscandata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SeekScanData_t {
    QByteArrayData data[20];
    char stringdata0[272];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SeekScanData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SeekScanData_t qt_meta_stringdata_SeekScanData = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SeekScanData"
QT_MOC_LITERAL(1, 13, 17), // "replaceSeekSample"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 16), // "QAbstractSeries*"
QT_MOC_LITERAL(4, 49, 8), // "a_series"
QT_MOC_LITERAL(5, 58, 6), // "a_type"
QT_MOC_LITERAL(6, 65, 16), // "calculateLargest"
QT_MOC_LITERAL(7, 82, 6), // "qreal&"
QT_MOC_LITERAL(8, 89, 9), // "a_axisVal"
QT_MOC_LITERAL(9, 99, 5), // "a_val"
QT_MOC_LITERAL(10, 105, 17), // "calculateSmallest"
QT_MOC_LITERAL(11, 123, 20), // "setAxisMinParameters"
QT_MOC_LITERAL(12, 144, 13), // "QList<qreal>&"
QT_MOC_LITERAL(13, 158, 20), // "setAxisMaxParameters"
QT_MOC_LITERAL(14, 179, 20), // "getAxisMinParameters"
QT_MOC_LITERAL(15, 200, 12), // "QList<qreal>"
QT_MOC_LITERAL(16, 213, 20), // "getAxisMaxParameters"
QT_MOC_LITERAL(17, 234, 20), // "getSeekSignatureData"
QT_MOC_LITERAL(18, 255, 11), // "a_historyId"
QT_MOC_LITERAL(19, 267, 4) // "init"

    },
    "SeekScanData\0replaceSeekSample\0\0"
    "QAbstractSeries*\0a_series\0a_type\0"
    "calculateLargest\0qreal&\0a_axisVal\0"
    "a_val\0calculateSmallest\0setAxisMinParameters\0"
    "QList<qreal>&\0setAxisMaxParameters\0"
    "getAxisMinParameters\0QList<qreal>\0"
    "getAxisMaxParameters\0getSeekSignatureData\0"
    "a_historyId\0init"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SeekScanData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x0a /* Public */,
       6,    2,   64,    2, 0x0a /* Public */,
      10,    2,   69,    2, 0x0a /* Public */,
      11,    1,   74,    2, 0x0a /* Public */,
      13,    1,   77,    2, 0x0a /* Public */,
      14,    0,   80,    2, 0x0a /* Public */,
      16,    0,   81,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      17,    1,   82,    2, 0x02 /* Public */,
      19,    0,   85,    2, 0x02 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::UChar,    4,    5,
    QMetaType::Void, 0x80000000 | 7, QMetaType::QReal,    8,    9,
    QMetaType::Void, 0x80000000 | 7, QMetaType::QReal,    8,    9,
    QMetaType::Void, 0x80000000 | 12,    8,
    QMetaType::Void, 0x80000000 | 12,    8,
    0x80000000 | 15,
    0x80000000 | 15,

 // methods: parameters
    QMetaType::Int, QMetaType::QString,   18,
    QMetaType::Void,

       0        // eod
};

void SeekScanData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SeekScanData *_t = static_cast<SeekScanData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->replaceSeekSample((*reinterpret_cast< QAbstractSeries*(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 1: _t->calculateLargest((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 2: _t->calculateSmallest((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 3: _t->setAxisMinParameters((*reinterpret_cast< QList<qreal>(*)>(_a[1]))); break;
        case 4: _t->setAxisMaxParameters((*reinterpret_cast< QList<qreal>(*)>(_a[1]))); break;
        case 5: { QList<qreal> _r = _t->getAxisMinParameters();
            if (_a[0]) *reinterpret_cast< QList<qreal>*>(_a[0]) = std::move(_r); }  break;
        case 6: { QList<qreal> _r = _t->getAxisMaxParameters();
            if (_a[0]) *reinterpret_cast< QList<qreal>*>(_a[0]) = std::move(_r); }  break;
        case 7: { int _r = _t->getSeekSignatureData((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->init(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSeries* >(); break;
            }
            break;
        }
    }
}

const QMetaObject SeekScanData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SeekScanData.data,
      qt_meta_data_SeekScanData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SeekScanData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SeekScanData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SeekScanData.stringdata0))
        return static_cast<void*>(const_cast< SeekScanData*>(this));
    return QObject::qt_metacast(_clname);
}

int SeekScanData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
