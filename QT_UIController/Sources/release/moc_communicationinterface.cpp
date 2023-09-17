/****************************************************************************
** Meta object code from reading C++ file 'communicationinterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/communicationinterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'communicationinterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HeartBeatUIBL_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HeartBeatUIBL_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HeartBeatUIBL_t qt_meta_stringdata_HeartBeatUIBL = {
    {
QT_MOC_LITERAL(0, 0, 13) // "HeartBeatUIBL"

    },
    "HeartBeatUIBL"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HeartBeatUIBL[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void HeartBeatUIBL::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject HeartBeatUIBL::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_HeartBeatUIBL.data,
      qt_meta_data_HeartBeatUIBL,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HeartBeatUIBL::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HeartBeatUIBL::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HeartBeatUIBL.stringdata0))
        return static_cast<void*>(const_cast< HeartBeatUIBL*>(this));
    return QThread::qt_metacast(_clname);
}

int HeartBeatUIBL::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_PartsPerMin_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PartsPerMin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PartsPerMin_t qt_meta_stringdata_PartsPerMin = {
    {
QT_MOC_LITERAL(0, 0, 11) // "PartsPerMin"

    },
    "PartsPerMin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PartsPerMin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void PartsPerMin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject PartsPerMin::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_PartsPerMin.data,
      qt_meta_data_PartsPerMin,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PartsPerMin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PartsPerMin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PartsPerMin.stringdata0))
        return static_cast<void*>(const_cast< PartsPerMin*>(this));
    return QThread::qt_metacast(_clname);
}

int PartsPerMin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_CommunicationInterface_t {
    QByteArrayData data[50];
    char stringdata0[851];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommunicationInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommunicationInterface_t qt_meta_stringdata_CommunicationInterface = {
    {
QT_MOC_LITERAL(0, 0, 22), // "CommunicationInterface"
QT_MOC_LITERAL(1, 23, 15), // "hornScanMsgData"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 5), // "reqId"
QT_MOC_LITERAL(4, 46, 7), // "msgData"
QT_MOC_LITERAL(5, 54, 15), // "seekScanMsgData"
QT_MOC_LITERAL(6, 70, 15), // "dashboardUpdate"
QT_MOC_LITERAL(7, 86, 16), // "productionUpdate"
QT_MOC_LITERAL(8, 103, 21), // "productionPowerUpdate"
QT_MOC_LITERAL(9, 125, 9), // "reqBuffer"
QT_MOC_LITERAL(10, 135, 17), // "updateWeldHistory"
QT_MOC_LITERAL(11, 153, 16), // "systemStatusData"
QT_MOC_LITERAL(12, 170, 18), // "dBStorageStatusRes"
QT_MOC_LITERAL(13, 189, 7), // "resData"
QT_MOC_LITERAL(14, 197, 17), // "weldDataDeleteInd"
QT_MOC_LITERAL(15, 215, 15), // "upgradeProgress"
QT_MOC_LITERAL(16, 231, 15), // "upgradeResponse"
QT_MOC_LITERAL(17, 247, 11), // "alarmRaised"
QT_MOC_LITERAL(18, 259, 19), // "clearNonResetAlarms"
QT_MOC_LITERAL(19, 279, 15), // "setupIndication"
QT_MOC_LITERAL(20, 295, 21), // "a_PartContactDistance"
QT_MOC_LITERAL(21, 317, 17), // "generateReportRes"
QT_MOC_LITERAL(22, 335, 13), // "a_generateRes"
QT_MOC_LITERAL(23, 349, 15), // "vacummReportRes"
QT_MOC_LITERAL(24, 365, 19), // "emergencyPopupStart"
QT_MOC_LITERAL(25, 385, 18), // "emergencyPopupStop"
QT_MOC_LITERAL(26, 404, 18), // "overLoadIndication"
QT_MOC_LITERAL(27, 423, 11), // "overLoadInd"
QT_MOC_LITERAL(28, 435, 15), // "setProgressData"
QT_MOC_LITERAL(29, 451, 14), // "a_ProgressData"
QT_MOC_LITERAL(30, 466, 19), // "barcodeRecipeRecall"
QT_MOC_LITERAL(31, 486, 14), // "sRecipeBarCode"
QT_MOC_LITERAL(32, 501, 28), // "barcodeRecipeRecallAuthCheck"
QT_MOC_LITERAL(33, 530, 17), // "sBarcodeAuthCheck"
QT_MOC_LITERAL(34, 548, 30), // "barcodescannerconnectionstatus"
QT_MOC_LITERAL(35, 579, 27), // "sBarcodescannerconnectcheck"
QT_MOC_LITERAL(36, 607, 12), // "partidstatus"
QT_MOC_LITERAL(37, 620, 13), // "sPartIdStatus"
QT_MOC_LITERAL(38, 634, 20), // "barcodeProcessStatus"
QT_MOC_LITERAL(39, 655, 17), // "qstrProcessStatus"
QT_MOC_LITERAL(40, 673, 14), // "changeStackInd"
QT_MOC_LITERAL(41, 688, 14), // "qstrSmartStack"
QT_MOC_LITERAL(42, 703, 13), // "memoryCleared"
QT_MOC_LITERAL(43, 717, 17), // "qstrMemoryCleared"
QT_MOC_LITERAL(44, 735, 9), // "copyDBRes"
QT_MOC_LITERAL(45, 745, 8), // "response"
QT_MOC_LITERAL(46, 754, 27), // "firmwareUpgradeOverEthernet"
QT_MOC_LITERAL(47, 782, 24), // "webserviceVersionRequest"
QT_MOC_LITERAL(48, 807, 24), // "webserviceUserLoggedInfo"
QT_MOC_LITERAL(49, 832, 18) // "ChangeFromWSSignal"

    },
    "CommunicationInterface\0hornScanMsgData\0"
    "\0reqId\0msgData\0seekScanMsgData\0"
    "dashboardUpdate\0productionUpdate\0"
    "productionPowerUpdate\0reqBuffer\0"
    "updateWeldHistory\0systemStatusData\0"
    "dBStorageStatusRes\0resData\0weldDataDeleteInd\0"
    "upgradeProgress\0upgradeResponse\0"
    "alarmRaised\0clearNonResetAlarms\0"
    "setupIndication\0a_PartContactDistance\0"
    "generateReportRes\0a_generateRes\0"
    "vacummReportRes\0emergencyPopupStart\0"
    "emergencyPopupStop\0overLoadIndication\0"
    "overLoadInd\0setProgressData\0a_ProgressData\0"
    "barcodeRecipeRecall\0sRecipeBarCode\0"
    "barcodeRecipeRecallAuthCheck\0"
    "sBarcodeAuthCheck\0barcodescannerconnectionstatus\0"
    "sBarcodescannerconnectcheck\0partidstatus\0"
    "sPartIdStatus\0barcodeProcessStatus\0"
    "qstrProcessStatus\0changeStackInd\0"
    "qstrSmartStack\0memoryCleared\0"
    "qstrMemoryCleared\0copyDBRes\0response\0"
    "firmwareUpgradeOverEthernet\0"
    "webserviceVersionRequest\0"
    "webserviceUserLoggedInfo\0ChangeFromWSSignal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommunicationInterface[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      32,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  174,    2, 0x06 /* Public */,
       5,    2,  179,    2, 0x06 /* Public */,
       6,    1,  184,    2, 0x06 /* Public */,
       7,    0,  187,    2, 0x06 /* Public */,
       8,    1,  188,    2, 0x06 /* Public */,
      10,    0,  191,    2, 0x06 /* Public */,
      11,    1,  192,    2, 0x06 /* Public */,
      12,    1,  195,    2, 0x06 /* Public */,
      14,    1,  198,    2, 0x06 /* Public */,
      15,    1,  201,    2, 0x06 /* Public */,
      16,    1,  204,    2, 0x06 /* Public */,
      17,    1,  207,    2, 0x06 /* Public */,
      18,    0,  210,    2, 0x06 /* Public */,
      19,    1,  211,    2, 0x06 /* Public */,
      21,    1,  214,    2, 0x06 /* Public */,
      23,    1,  217,    2, 0x06 /* Public */,
      24,    0,  220,    2, 0x06 /* Public */,
      25,    0,  221,    2, 0x06 /* Public */,
      26,    1,  222,    2, 0x06 /* Public */,
      28,    1,  225,    2, 0x06 /* Public */,
      30,    1,  228,    2, 0x06 /* Public */,
      32,    1,  231,    2, 0x06 /* Public */,
      34,    1,  234,    2, 0x06 /* Public */,
      36,    1,  237,    2, 0x06 /* Public */,
      38,    1,  240,    2, 0x06 /* Public */,
      40,    1,  243,    2, 0x06 /* Public */,
      42,    1,  246,    2, 0x06 /* Public */,
      44,    1,  249,    2, 0x06 /* Public */,
      46,    1,  252,    2, 0x06 /* Public */,
      47,    1,  255,    2, 0x06 /* Public */,
      48,    1,  258,    2, 0x06 /* Public */,
      49,    1,  261,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    3,    4,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void, QMetaType::QString,   29,
    QMetaType::Void, QMetaType::QString,   31,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::Void, QMetaType::QString,   35,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void, QMetaType::QString,   39,
    QMetaType::Void, QMetaType::QString,   41,
    QMetaType::Void, QMetaType::QString,   43,
    QMetaType::Void, QMetaType::QString,   45,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,

       0        // eod
};

void CommunicationInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CommunicationInterface *_t = static_cast<CommunicationInterface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->hornScanMsgData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 1: _t->seekScanMsgData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 2: _t->dashboardUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->productionUpdate(); break;
        case 4: _t->productionPowerUpdate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->updateWeldHistory(); break;
        case 6: _t->systemStatusData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->dBStorageStatusRes((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->weldDataDeleteInd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->upgradeProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->upgradeResponse((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->alarmRaised((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->clearNonResetAlarms(); break;
        case 13: _t->setupIndication((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->generateReportRes((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->vacummReportRes((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->emergencyPopupStart(); break;
        case 17: _t->emergencyPopupStop(); break;
        case 18: _t->overLoadIndication((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: _t->setProgressData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->barcodeRecipeRecall((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->barcodeRecipeRecallAuthCheck((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->barcodescannerconnectionstatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->partidstatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: _t->barcodeProcessStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: _t->changeStackInd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->memoryCleared((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 27: _t->copyDBRes((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->firmwareUpgradeOverEthernet((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 29: _t->webserviceVersionRequest((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 30: _t->webserviceUserLoggedInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 31: _t->ChangeFromWSSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CommunicationInterface::*_t)(int , QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::hornScanMsgData)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(int , QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::seekScanMsgData)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::dashboardUpdate)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::productionUpdate)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::productionPowerUpdate)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::updateWeldHistory)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::systemStatusData)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::dBStorageStatusRes)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::weldDataDeleteInd)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::upgradeProgress)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::upgradeResponse)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::alarmRaised)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::clearNonResetAlarms)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::setupIndication)) {
                *result = 13;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::generateReportRes)) {
                *result = 14;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::vacummReportRes)) {
                *result = 15;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::emergencyPopupStart)) {
                *result = 16;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::emergencyPopupStop)) {
                *result = 17;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::overLoadIndication)) {
                *result = 18;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::setProgressData)) {
                *result = 19;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::barcodeRecipeRecall)) {
                *result = 20;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::barcodeRecipeRecallAuthCheck)) {
                *result = 21;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::barcodescannerconnectionstatus)) {
                *result = 22;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::partidstatus)) {
                *result = 23;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::barcodeProcessStatus)) {
                *result = 24;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::changeStackInd)) {
                *result = 25;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::memoryCleared)) {
                *result = 26;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::copyDBRes)) {
                *result = 27;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::firmwareUpgradeOverEthernet)) {
                *result = 28;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::webserviceVersionRequest)) {
                *result = 29;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::webserviceUserLoggedInfo)) {
                *result = 30;
                return;
            }
        }
        {
            typedef void (CommunicationInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicationInterface::ChangeFromWSSignal)) {
                *result = 31;
                return;
            }
        }
    }
}

const QMetaObject CommunicationInterface::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CommunicationInterface.data,
      qt_meta_data_CommunicationInterface,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CommunicationInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommunicationInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CommunicationInterface.stringdata0))
        return static_cast<void*>(const_cast< CommunicationInterface*>(this));
    return QThread::qt_metacast(_clname);
}

int CommunicationInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 32;
    }
    return _id;
}

// SIGNAL 0
void CommunicationInterface::hornScanMsgData(int _t1, QByteArray _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CommunicationInterface::seekScanMsgData(int _t1, QByteArray _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CommunicationInterface::dashboardUpdate(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CommunicationInterface::productionUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CommunicationInterface::productionPowerUpdate(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CommunicationInterface::updateWeldHistory()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CommunicationInterface::systemStatusData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CommunicationInterface::dBStorageStatusRes(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CommunicationInterface::weldDataDeleteInd(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CommunicationInterface::upgradeProgress(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CommunicationInterface::upgradeResponse(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CommunicationInterface::alarmRaised(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CommunicationInterface::clearNonResetAlarms()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void CommunicationInterface::setupIndication(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CommunicationInterface::generateReportRes(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CommunicationInterface::vacummReportRes(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CommunicationInterface::emergencyPopupStart()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void CommunicationInterface::emergencyPopupStop()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void CommunicationInterface::overLoadIndication(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void CommunicationInterface::setProgressData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void CommunicationInterface::barcodeRecipeRecall(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void CommunicationInterface::barcodeRecipeRecallAuthCheck(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void CommunicationInterface::barcodescannerconnectionstatus(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void CommunicationInterface::partidstatus(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void CommunicationInterface::barcodeProcessStatus(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void CommunicationInterface::changeStackInd(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void CommunicationInterface::memoryCleared(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void CommunicationInterface::copyDBRes(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void CommunicationInterface::firmwareUpgradeOverEthernet(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void CommunicationInterface::webserviceVersionRequest(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}

// SIGNAL 30
void CommunicationInterface::webserviceUserLoggedInfo(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void CommunicationInterface::ChangeFromWSSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
