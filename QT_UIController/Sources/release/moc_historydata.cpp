/****************************************************************************
** Meta object code from reading C++ file 'historydata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/historydata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'historydata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HistoryItem_t {
    QByteArrayData data[10];
    char stringdata0[109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HistoryItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HistoryItem_t qt_meta_stringdata_HistoryItem = {
    {
QT_MOC_LITERAL(0, 0, 11), // "HistoryItem"
QT_MOC_LITERAL(1, 12, 15), // "scanNameChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 13), // "scanIdChanged"
QT_MOC_LITERAL(4, 43, 15), // "dateTimeChanged"
QT_MOC_LITERAL(5, 59, 15), // "recipeIdChanged"
QT_MOC_LITERAL(6, 75, 8), // "scanName"
QT_MOC_LITERAL(7, 84, 6), // "scanId"
QT_MOC_LITERAL(8, 91, 8), // "dateTime"
QT_MOC_LITERAL(9, 100, 8) // "recipeId"

    },
    "HistoryItem\0scanNameChanged\0\0scanIdChanged\0"
    "dateTimeChanged\0recipeIdChanged\0"
    "scanName\0scanId\0dateTime\0recipeId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HistoryItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       4,   38, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    0,   36,    2, 0x06 /* Public */,
       5,    0,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       6, QMetaType::QString, 0x00495001,
       7, QMetaType::QString, 0x00495001,
       8, QMetaType::QString, 0x00495001,
       9, QMetaType::UShort, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,

       0        // eod
};

void HistoryItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HistoryItem *_t = static_cast<HistoryItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->scanNameChanged(); break;
        case 1: _t->scanIdChanged(); break;
        case 2: _t->dateTimeChanged(); break;
        case 3: _t->recipeIdChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HistoryItem::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistoryItem::scanNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (HistoryItem::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistoryItem::scanIdChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (HistoryItem::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistoryItem::dateTimeChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (HistoryItem::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistoryItem::recipeIdChanged)) {
                *result = 3;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        HistoryItem *_t = static_cast<HistoryItem *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getScanName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getScanId(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getDateTime(); break;
        case 3: *reinterpret_cast< quint16*>(_v) = _t->getRecipeId(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject HistoryItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HistoryItem.data,
      qt_meta_data_HistoryItem,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HistoryItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HistoryItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryItem.stringdata0))
        return static_cast<void*>(const_cast< HistoryItem*>(this));
    return QObject::qt_metacast(_clname);
}

int HistoryItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void HistoryItem::scanNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HistoryItem::scanIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HistoryItem::dateTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void HistoryItem::recipeIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
struct qt_meta_stringdata_HistoryData_t {
    QByteArrayData data[40];
    char stringdata0[608];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HistoryData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HistoryData_t qt_meta_stringdata_HistoryData = {
    {
QT_MOC_LITERAL(0, 0, 11), // "HistoryData"
QT_MOC_LITERAL(1, 12, 14), // "changeProgress"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 9), // "statusVal"
QT_MOC_LITERAL(4, 38, 17), // "hornScanCompleted"
QT_MOC_LITERAL(5, 56, 18), // "disableConnections"
QT_MOC_LITERAL(6, 75, 19), // "hornOpStatusChanged"
QT_MOC_LITERAL(7, 95, 22), // "hornIndSelectedChanged"
QT_MOC_LITERAL(8, 118, 16), // "clearHistoryData"
QT_MOC_LITERAL(9, 135, 17), // "receiveScUiReqMsg"
QT_MOC_LITERAL(10, 153, 5), // "reqId"
QT_MOC_LITERAL(11, 159, 7), // "ScUiMsg"
QT_MOC_LITERAL(12, 167, 19), // "updateHistoryDataId"
QT_MOC_LITERAL(13, 187, 13), // "a_positionInd"
QT_MOC_LITERAL(14, 201, 9), // "updatedId"
QT_MOC_LITERAL(15, 211, 14), // "refreshHistory"
QT_MOC_LITERAL(16, 226, 21), // "retrieveHornSigPoints"
QT_MOC_LITERAL(17, 248, 16), // "QAbstractSeries*"
QT_MOC_LITERAL(18, 265, 8), // "a_series"
QT_MOC_LITERAL(19, 274, 6), // "a_type"
QT_MOC_LITERAL(20, 281, 22), // "getHornGraphDataFromDb"
QT_MOC_LITERAL(21, 304, 11), // "a_historyId"
QT_MOC_LITERAL(22, 316, 20), // "getHistoryDataFromDb"
QT_MOC_LITERAL(23, 337, 10), // "a_dateTime"
QT_MOC_LITERAL(24, 348, 10), // "a_countReq"
QT_MOC_LITERAL(25, 359, 13), // "abortHornScan"
QT_MOC_LITERAL(26, 373, 15), // "performHornScan"
QT_MOC_LITERAL(27, 389, 22), // "getHornOperationStatus"
QT_MOC_LITERAL(28, 412, 18), // "getHornIndSelected"
QT_MOC_LITERAL(29, 431, 18), // "setHornIndSelected"
QT_MOC_LITERAL(30, 450, 11), // "a_hornIndex"
QT_MOC_LITERAL(31, 462, 23), // "receiveStartHornScanRes"
QT_MOC_LITERAL(32, 486, 6), // "status"
QT_MOC_LITERAL(33, 493, 21), // "receiveHornScanStatus"
QT_MOC_LITERAL(34, 515, 14), // "samplesCounted"
QT_MOC_LITERAL(35, 530, 17), // "hornAxisMaxValues"
QT_MOC_LITERAL(36, 548, 12), // "QList<qreal>"
QT_MOC_LITERAL(37, 561, 17), // "hornAxisMinValues"
QT_MOC_LITERAL(38, 579, 12), // "hornOpStatus"
QT_MOC_LITERAL(39, 592, 15) // "hornIndSelected"

    },
    "HistoryData\0changeProgress\0\0statusVal\0"
    "hornScanCompleted\0disableConnections\0"
    "hornOpStatusChanged\0hornIndSelectedChanged\0"
    "clearHistoryData\0receiveScUiReqMsg\0"
    "reqId\0ScUiMsg\0updateHistoryDataId\0"
    "a_positionInd\0updatedId\0refreshHistory\0"
    "retrieveHornSigPoints\0QAbstractSeries*\0"
    "a_series\0a_type\0getHornGraphDataFromDb\0"
    "a_historyId\0getHistoryDataFromDb\0"
    "a_dateTime\0a_countReq\0abortHornScan\0"
    "performHornScan\0getHornOperationStatus\0"
    "getHornIndSelected\0setHornIndSelected\0"
    "a_hornIndex\0receiveStartHornScanRes\0"
    "status\0receiveHornScanStatus\0"
    "samplesCounted\0hornAxisMaxValues\0"
    "QList<qreal>\0hornAxisMinValues\0"
    "hornOpStatus\0hornIndSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HistoryData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       2,  168, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  119,    2, 0x06 /* Public */,
       4,    1,  122,    2, 0x06 /* Public */,
       5,    0,  125,    2, 0x06 /* Public */,
       6,    0,  126,    2, 0x06 /* Public */,
       7,    0,  127,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  128,    2, 0x0a /* Public */,
       9,    2,  129,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      12,    2,  134,    2, 0x02 /* Public */,
      15,    0,  139,    2, 0x02 /* Public */,
      16,    2,  140,    2, 0x02 /* Public */,
      20,    1,  145,    2, 0x02 /* Public */,
      22,    2,  148,    2, 0x02 /* Public */,
      25,    0,  153,    2, 0x02 /* Public */,
      26,    0,  154,    2, 0x02 /* Public */,
      27,    0,  155,    2, 0x02 /* Public */,
      28,    0,  156,    2, 0x02 /* Public */,
      29,    1,  157,    2, 0x02 /* Public */,
      31,    1,  160,    2, 0x02 /* Public */,
      33,    1,  163,    2, 0x02 /* Public */,
      35,    0,  166,    2, 0x02 /* Public */,
      37,    0,  167,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QReal,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,   10,   11,

 // methods: parameters
    QMetaType::Int, QMetaType::UInt, QMetaType::QString,   13,   14,
    QMetaType::Int,
    QMetaType::Int, 0x80000000 | 17, QMetaType::UChar,   18,   19,
    QMetaType::Int, QMetaType::QString,   21,
    QMetaType::Int, QMetaType::QString, QMetaType::QString,   23,   24,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Bool,
    QMetaType::UInt,
    QMetaType::Void, QMetaType::UInt,   30,
    QMetaType::Void, QMetaType::Int,   32,
    QMetaType::Void, QMetaType::Int,   34,
    0x80000000 | 36,
    0x80000000 | 36,

 // properties: name, type, flags
      38, QMetaType::Bool, 0x00495001,
      39, QMetaType::UInt, 0x00495103,

 // properties: notify_signal_id
       3,
       4,

       0        // eod
};

void HistoryData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HistoryData *_t = static_cast<HistoryData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeProgress((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: _t->hornScanCompleted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->disableConnections(); break;
        case 3: _t->hornOpStatusChanged(); break;
        case 4: _t->hornIndSelectedChanged(); break;
        case 5: _t->clearHistoryData(); break;
        case 6: _t->receiveScUiReqMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 7: { int _r = _t->updateHistoryDataId((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 8: { int _r = _t->refreshHistory();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->retrieveHornSigPoints((*reinterpret_cast< QAbstractSeries*(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->getHornGraphDataFromDb((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 11: { int _r = _t->getHistoryDataFromDb((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->abortHornScan();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 13: { int _r = _t->performHornScan();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 14: { bool _r = _t->getHornOperationStatus();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 15: { uint _r = _t->getHornIndSelected();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->setHornIndSelected((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 17: _t->receiveStartHornScanRes((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->receiveHornScanStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: { QList<qreal> _r = _t->hornAxisMaxValues();
            if (_a[0]) *reinterpret_cast< QList<qreal>*>(_a[0]) = std::move(_r); }  break;
        case 20: { QList<qreal> _r = _t->hornAxisMinValues();
            if (_a[0]) *reinterpret_cast< QList<qreal>*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSeries* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HistoryData::*_t)(qreal );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistoryData::changeProgress)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (HistoryData::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistoryData::hornScanCompleted)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (HistoryData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistoryData::disableConnections)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (HistoryData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistoryData::hornOpStatusChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (HistoryData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistoryData::hornIndSelectedChanged)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        HistoryData *_t = static_cast<HistoryData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->getHornOperationStatus(); break;
        case 1: *reinterpret_cast< uint*>(_v) = _t->getHornIndSelected(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        HistoryData *_t = static_cast<HistoryData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setHornIndSelected(*reinterpret_cast< uint*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject HistoryData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HistoryData.data,
      qt_meta_data_HistoryData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HistoryData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HistoryData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryData.stringdata0))
        return static_cast<void*>(const_cast< HistoryData*>(this));
    return QObject::qt_metacast(_clname);
}

int HistoryData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
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
void HistoryData::changeProgress(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HistoryData::hornScanCompleted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HistoryData::disableConnections()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void HistoryData::hornOpStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void HistoryData::hornIndSelectedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
