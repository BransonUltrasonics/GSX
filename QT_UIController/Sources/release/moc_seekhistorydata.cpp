/****************************************************************************
** Meta object code from reading C++ file 'seekhistorydata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/seekhistorydata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'seekhistorydata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SeekHistoryItem_t {
    QByteArrayData data[6];
    char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SeekHistoryItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SeekHistoryItem_t qt_meta_stringdata_SeekHistoryItem = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SeekHistoryItem"
QT_MOC_LITERAL(1, 16, 17), // "seekscanIdChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 19), // "seekdateTimeChanged"
QT_MOC_LITERAL(4, 55, 6), // "scanId"
QT_MOC_LITERAL(5, 62, 8) // "dateTime"

    },
    "SeekHistoryItem\0seekscanIdChanged\0\0"
    "seekdateTimeChanged\0scanId\0dateTime"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SeekHistoryItem[] = {

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
       5, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void SeekHistoryItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SeekHistoryItem *_t = static_cast<SeekHistoryItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->seekscanIdChanged(); break;
        case 1: _t->seekdateTimeChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SeekHistoryItem::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekHistoryItem::seekscanIdChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SeekHistoryItem::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekHistoryItem::seekdateTimeChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        SeekHistoryItem *_t = static_cast<SeekHistoryItem *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getSeekScanId(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getSeekDateTime(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject SeekHistoryItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SeekHistoryItem.data,
      qt_meta_data_SeekHistoryItem,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SeekHistoryItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SeekHistoryItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SeekHistoryItem.stringdata0))
        return static_cast<void*>(const_cast< SeekHistoryItem*>(this));
    return QObject::qt_metacast(_clname);
}

int SeekHistoryItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void SeekHistoryItem::seekscanIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SeekHistoryItem::seekdateTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_SeekHistoryData_t {
    QByteArrayData data[39];
    char stringdata0[626];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SeekHistoryData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SeekHistoryData_t qt_meta_stringdata_SeekHistoryData = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SeekHistoryData"
QT_MOC_LITERAL(1, 16, 18), // "changeSeekProgress"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 9), // "statusVal"
QT_MOC_LITERAL(4, 46, 17), // "seekScanCompleted"
QT_MOC_LITERAL(5, 64, 22), // "disableSeekConnections"
QT_MOC_LITERAL(6, 87, 19), // "seekOpStatusChanged"
QT_MOC_LITERAL(7, 107, 22), // "seekIndSelectedChanged"
QT_MOC_LITERAL(8, 130, 20), // "clearSeekHistoryData"
QT_MOC_LITERAL(9, 151, 21), // "receiveScUiseekReqMsg"
QT_MOC_LITERAL(10, 173, 5), // "reqId"
QT_MOC_LITERAL(11, 179, 7), // "ScUiMsg"
QT_MOC_LITERAL(12, 187, 23), // "updateSeekHistoryDataId"
QT_MOC_LITERAL(13, 211, 13), // "a_positionInd"
QT_MOC_LITERAL(14, 225, 9), // "updatedId"
QT_MOC_LITERAL(15, 235, 18), // "refreshSeekHistory"
QT_MOC_LITERAL(16, 254, 21), // "retrieveSeekSigPoints"
QT_MOC_LITERAL(17, 276, 16), // "QAbstractSeries*"
QT_MOC_LITERAL(18, 293, 8), // "a_series"
QT_MOC_LITERAL(19, 302, 6), // "a_type"
QT_MOC_LITERAL(20, 309, 22), // "getSeekGraphDataFromDb"
QT_MOC_LITERAL(21, 332, 11), // "a_historyId"
QT_MOC_LITERAL(22, 344, 24), // "getSeekHistoryDataFromDb"
QT_MOC_LITERAL(23, 369, 10), // "a_dateTime"
QT_MOC_LITERAL(24, 380, 10), // "a_countReq"
QT_MOC_LITERAL(25, 391, 15), // "performSeekScan"
QT_MOC_LITERAL(26, 407, 22), // "getSeekOperationStatus"
QT_MOC_LITERAL(27, 430, 18), // "getSeekIndSelected"
QT_MOC_LITERAL(28, 449, 18), // "setSeekIndSelected"
QT_MOC_LITERAL(29, 468, 11), // "a_hornIndex"
QT_MOC_LITERAL(30, 480, 23), // "receiveStartSeekScanRes"
QT_MOC_LITERAL(31, 504, 6), // "status"
QT_MOC_LITERAL(32, 511, 21), // "receiveSeekScanStatus"
QT_MOC_LITERAL(33, 533, 14), // "samplesCounted"
QT_MOC_LITERAL(34, 548, 17), // "seekAxisMaxValues"
QT_MOC_LITERAL(35, 566, 12), // "QList<qreal>"
QT_MOC_LITERAL(36, 579, 17), // "seekAxisMinValues"
QT_MOC_LITERAL(37, 597, 12), // "seekOpStatus"
QT_MOC_LITERAL(38, 610, 15) // "seekIndSelected"

    },
    "SeekHistoryData\0changeSeekProgress\0\0"
    "statusVal\0seekScanCompleted\0"
    "disableSeekConnections\0seekOpStatusChanged\0"
    "seekIndSelectedChanged\0clearSeekHistoryData\0"
    "receiveScUiseekReqMsg\0reqId\0ScUiMsg\0"
    "updateSeekHistoryDataId\0a_positionInd\0"
    "updatedId\0refreshSeekHistory\0"
    "retrieveSeekSigPoints\0QAbstractSeries*\0"
    "a_series\0a_type\0getSeekGraphDataFromDb\0"
    "a_historyId\0getSeekHistoryDataFromDb\0"
    "a_dateTime\0a_countReq\0performSeekScan\0"
    "getSeekOperationStatus\0getSeekIndSelected\0"
    "setSeekIndSelected\0a_hornIndex\0"
    "receiveStartSeekScanRes\0status\0"
    "receiveSeekScanStatus\0samplesCounted\0"
    "seekAxisMaxValues\0QList<qreal>\0"
    "seekAxisMinValues\0seekOpStatus\0"
    "seekIndSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SeekHistoryData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       2,  162, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  114,    2, 0x06 /* Public */,
       4,    1,  117,    2, 0x06 /* Public */,
       5,    0,  120,    2, 0x06 /* Public */,
       6,    0,  121,    2, 0x06 /* Public */,
       7,    0,  122,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  123,    2, 0x0a /* Public */,
       9,    2,  124,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      12,    2,  129,    2, 0x02 /* Public */,
      15,    0,  134,    2, 0x02 /* Public */,
      16,    2,  135,    2, 0x02 /* Public */,
      20,    1,  140,    2, 0x02 /* Public */,
      22,    2,  143,    2, 0x02 /* Public */,
      25,    0,  148,    2, 0x02 /* Public */,
      26,    0,  149,    2, 0x02 /* Public */,
      27,    0,  150,    2, 0x02 /* Public */,
      28,    1,  151,    2, 0x02 /* Public */,
      30,    1,  154,    2, 0x02 /* Public */,
      32,    1,  157,    2, 0x02 /* Public */,
      34,    0,  160,    2, 0x02 /* Public */,
      36,    0,  161,    2, 0x02 /* Public */,

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
    QMetaType::Bool,
    QMetaType::UInt,
    QMetaType::Void, QMetaType::UInt,   29,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void, QMetaType::Int,   33,
    0x80000000 | 35,
    0x80000000 | 35,

 // properties: name, type, flags
      37, QMetaType::Bool, 0x00495001,
      38, QMetaType::UInt, 0x00495103,

 // properties: notify_signal_id
       3,
       4,

       0        // eod
};

void SeekHistoryData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SeekHistoryData *_t = static_cast<SeekHistoryData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeSeekProgress((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: _t->seekScanCompleted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->disableSeekConnections(); break;
        case 3: _t->seekOpStatusChanged(); break;
        case 4: _t->seekIndSelectedChanged(); break;
        case 5: _t->clearSeekHistoryData(); break;
        case 6: _t->receiveScUiseekReqMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 7: { int _r = _t->updateSeekHistoryDataId((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 8: { int _r = _t->refreshSeekHistory();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->retrieveSeekSigPoints((*reinterpret_cast< QAbstractSeries*(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->getSeekGraphDataFromDb((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 11: { int _r = _t->getSeekHistoryDataFromDb((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->performSeekScan();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 13: { bool _r = _t->getSeekOperationStatus();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 14: { uint _r = _t->getSeekIndSelected();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->setSeekIndSelected((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 16: _t->receiveStartSeekScanRes((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->receiveSeekScanStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: { QList<qreal> _r = _t->seekAxisMaxValues();
            if (_a[0]) *reinterpret_cast< QList<qreal>*>(_a[0]) = std::move(_r); }  break;
        case 19: { QList<qreal> _r = _t->seekAxisMinValues();
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
            typedef void (SeekHistoryData::*_t)(qreal );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekHistoryData::changeSeekProgress)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SeekHistoryData::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekHistoryData::seekScanCompleted)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SeekHistoryData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekHistoryData::disableSeekConnections)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SeekHistoryData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekHistoryData::seekOpStatusChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SeekHistoryData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeekHistoryData::seekIndSelectedChanged)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        SeekHistoryData *_t = static_cast<SeekHistoryData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->getSeekOperationStatus(); break;
        case 1: *reinterpret_cast< uint*>(_v) = _t->getSeekIndSelected(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        SeekHistoryData *_t = static_cast<SeekHistoryData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setSeekIndSelected(*reinterpret_cast< uint*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject SeekHistoryData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SeekHistoryData.data,
      qt_meta_data_SeekHistoryData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SeekHistoryData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SeekHistoryData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SeekHistoryData.stringdata0))
        return static_cast<void*>(const_cast< SeekHistoryData*>(this));
    return QObject::qt_metacast(_clname);
}

int SeekHistoryData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
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
void SeekHistoryData::changeSeekProgress(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SeekHistoryData::seekScanCompleted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SeekHistoryData::disableSeekConnections()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SeekHistoryData::seekOpStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SeekHistoryData::seekIndSelectedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
