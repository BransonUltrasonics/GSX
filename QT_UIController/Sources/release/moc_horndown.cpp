/****************************************************************************
** Meta object code from reading C++ file 'horndown.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/horndown.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'horndown.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HornDown_t {
    QByteArrayData data[65];
    char stringdata0[1100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HornDown_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HornDown_t qt_meta_stringdata_HornDown = {
    {
QT_MOC_LITERAL(0, 0, 8), // "HornDown"
QT_MOC_LITERAL(1, 9, 20), // "readypositionChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 18), // "ppcPositionChanged"
QT_MOC_LITERAL(4, 50, 18), // "epcPositionChanged"
QT_MOC_LITERAL(5, 69, 17), // "mpPositionChanged"
QT_MOC_LITERAL(6, 87, 17), // "wpPositionChanged"
QT_MOC_LITERAL(7, 105, 18), // "hornPointerChanged"
QT_MOC_LITERAL(8, 124, 26), // "strokeLengthInPixelChanged"
QT_MOC_LITERAL(9, 151, 23), // "absolutePositionChanged"
QT_MOC_LITERAL(10, 175, 26), // "partContactVelocityChanged"
QT_MOC_LITERAL(11, 202, 18), // "actualForceChanged"
QT_MOC_LITERAL(12, 221, 16), // "actualPPCChanged"
QT_MOC_LITERAL(13, 238, 24), // "updateHornDownStatusText"
QT_MOC_LITERAL(14, 263, 21), // "strHornDownStatusText"
QT_MOC_LITERAL(15, 285, 19), // "updateHornPositions"
QT_MOC_LITERAL(16, 305, 14), // "RefreshAllData"
QT_MOC_LITERAL(17, 320, 11), // "resetValues"
QT_MOC_LITERAL(18, 332, 22), // "messageStatusOfRequest"
QT_MOC_LITERAL(19, 355, 6), // "dwType"
QT_MOC_LITERAL(20, 362, 29), // "successMessageStatusOfRequest"
QT_MOC_LITERAL(21, 392, 21), // "UpdateStructureFromBL"
QT_MOC_LITERAL(22, 414, 19), // "updateEPCDataFromBL"
QT_MOC_LITERAL(23, 434, 26), // "findPartContactRequestToBL"
QT_MOC_LITERAL(24, 461, 16), // "jogUpRequestToBL"
QT_MOC_LITERAL(25, 478, 18), // "jogDownRequestToBL"
QT_MOC_LITERAL(26, 497, 29), // "setMyReadyPositionRequestToBL"
QT_MOC_LITERAL(27, 527, 19), // "horndownRequestToBL"
QT_MOC_LITERAL(28, 547, 11), // "isHornClamp"
QT_MOC_LITERAL(29, 559, 18), // "retractRequestToBL"
QT_MOC_LITERAL(30, 578, 15), // "startOrStopWeld"
QT_MOC_LITERAL(31, 594, 5), // "subID"
QT_MOC_LITERAL(32, 600, 24), // "getHornDownRememberModes"
QT_MOC_LITERAL(33, 625, 24), // "setHornDownRememberModes"
QT_MOC_LITERAL(34, 650, 5), // "bRFPC"
QT_MOC_LITERAL(35, 656, 5), // "bRJOG"
QT_MOC_LITERAL(36, 662, 10), // "bRHORNDOWN"
QT_MOC_LITERAL(37, 673, 10), // "bFPCStatus"
QT_MOC_LITERAL(38, 684, 10), // "bJOGStatus"
QT_MOC_LITERAL(39, 695, 15), // "bHORNDOWNStatus"
QT_MOC_LITERAL(40, 711, 7), // "bUpFlag"
QT_MOC_LITERAL(41, 719, 9), // "bDownFlag"
QT_MOC_LITERAL(42, 729, 18), // "updateActuatorList"
QT_MOC_LITERAL(43, 748, 24), // "updateHornPositionValues"
QT_MOC_LITERAL(44, 773, 27), // "fillActiveRecipeForActuator"
QT_MOC_LITERAL(45, 801, 6), // "dwFlag"
QT_MOC_LITERAL(46, 808, 26), // "getRecipeNumberForActuator"
QT_MOC_LITERAL(47, 835, 24), // "getRecipeNameForActuator"
QT_MOC_LITERAL(48, 860, 7), // "toPixel"
QT_MOC_LITERAL(49, 868, 15), // "a_valueInMetric"
QT_MOC_LITERAL(50, 884, 9), // "splitdata"
QT_MOC_LITERAL(51, 894, 25), // "getStrokeLengthFormConfig"
QT_MOC_LITERAL(52, 920, 12), // "getUnitsType"
QT_MOC_LITERAL(53, 933, 15), // "getResponseType"
QT_MOC_LITERAL(54, 949, 13), // "readyposition"
QT_MOC_LITERAL(55, 963, 11), // "ppcPosition"
QT_MOC_LITERAL(56, 975, 11), // "epcPosition"
QT_MOC_LITERAL(57, 987, 10), // "mpPosition"
QT_MOC_LITERAL(58, 998, 10), // "wpPosition"
QT_MOC_LITERAL(59, 1009, 11), // "hornPointer"
QT_MOC_LITERAL(60, 1021, 19), // "strokeLengthInPixel"
QT_MOC_LITERAL(61, 1041, 16), // "absolutePosition"
QT_MOC_LITERAL(62, 1058, 19), // "partContactVelocity"
QT_MOC_LITERAL(63, 1078, 11), // "actualForce"
QT_MOC_LITERAL(64, 1090, 9) // "actualPPC"

    },
    "HornDown\0readypositionChanged\0\0"
    "ppcPositionChanged\0epcPositionChanged\0"
    "mpPositionChanged\0wpPositionChanged\0"
    "hornPointerChanged\0strokeLengthInPixelChanged\0"
    "absolutePositionChanged\0"
    "partContactVelocityChanged\0"
    "actualForceChanged\0actualPPCChanged\0"
    "updateHornDownStatusText\0strHornDownStatusText\0"
    "updateHornPositions\0RefreshAllData\0"
    "resetValues\0messageStatusOfRequest\0"
    "dwType\0successMessageStatusOfRequest\0"
    "UpdateStructureFromBL\0updateEPCDataFromBL\0"
    "findPartContactRequestToBL\0jogUpRequestToBL\0"
    "jogDownRequestToBL\0setMyReadyPositionRequestToBL\0"
    "horndownRequestToBL\0isHornClamp\0"
    "retractRequestToBL\0startOrStopWeld\0"
    "subID\0getHornDownRememberModes\0"
    "setHornDownRememberModes\0bRFPC\0bRJOG\0"
    "bRHORNDOWN\0bFPCStatus\0bJOGStatus\0"
    "bHORNDOWNStatus\0bUpFlag\0bDownFlag\0"
    "updateActuatorList\0updateHornPositionValues\0"
    "fillActiveRecipeForActuator\0dwFlag\0"
    "getRecipeNumberForActuator\0"
    "getRecipeNameForActuator\0toPixel\0"
    "a_valueInMetric\0splitdata\0"
    "getStrokeLengthFormConfig\0getUnitsType\0"
    "getResponseType\0readyposition\0ppcPosition\0"
    "epcPosition\0mpPosition\0wpPosition\0"
    "hornPointer\0strokeLengthInPixel\0"
    "absolutePosition\0partContactVelocity\0"
    "actualForce\0actualPPC"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HornDown[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   14, // methods
      11,  282, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  204,    2, 0x06 /* Public */,
       3,    0,  205,    2, 0x06 /* Public */,
       4,    0,  206,    2, 0x06 /* Public */,
       5,    0,  207,    2, 0x06 /* Public */,
       6,    0,  208,    2, 0x06 /* Public */,
       7,    0,  209,    2, 0x06 /* Public */,
       8,    0,  210,    2, 0x06 /* Public */,
       9,    0,  211,    2, 0x06 /* Public */,
      10,    0,  212,    2, 0x06 /* Public */,
      11,    0,  213,    2, 0x06 /* Public */,
      12,    0,  214,    2, 0x06 /* Public */,
      13,    1,  215,    2, 0x06 /* Public */,
      15,    0,  218,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      16,    0,  219,    2, 0x02 /* Public */,
      17,    0,  220,    2, 0x02 /* Public */,
      18,    1,  221,    2, 0x02 /* Public */,
      20,    1,  224,    2, 0x02 /* Public */,
      21,    1,  227,    2, 0x02 /* Public */,
      22,    0,  230,    2, 0x02 /* Public */,
      23,    0,  231,    2, 0x02 /* Public */,
      24,    0,  232,    2, 0x02 /* Public */,
      25,    0,  233,    2, 0x02 /* Public */,
      26,    1,  234,    2, 0x02 /* Public */,
      27,    1,  237,    2, 0x02 /* Public */,
      29,    0,  240,    2, 0x02 /* Public */,
      30,    1,  241,    2, 0x02 /* Public */,
      32,    0,  244,    2, 0x02 /* Public */,
      33,    8,  245,    2, 0x02 /* Public */,
      42,    1,  262,    2, 0x02 /* Public */,
      43,    0,  265,    2, 0x02 /* Public */,
      44,    1,  266,    2, 0x02 /* Public */,
      46,    0,  269,    2, 0x02 /* Public */,
      47,    0,  270,    2, 0x02 /* Public */,
      48,    1,  271,    2, 0x02 /* Public */,
      50,    2,  274,    2, 0x02 /* Public */,
      51,    0,  279,    2, 0x02 /* Public */,
      52,    0,  280,    2, 0x02 /* Public */,
      53,    0,  281,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Bool,   28,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::QString,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,   34,   35,   36,   37,   38,   39,   40,   41,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   45,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString, QMetaType::QString,   49,
    QMetaType::QString, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::QString,
    QMetaType::Int,
    QMetaType::Int,

 // properties: name, type, flags
      54, QMetaType::QString, 0x00495003,
      55, QMetaType::QString, 0x00495003,
      56, QMetaType::QString, 0x00495003,
      57, QMetaType::QString, 0x00495003,
      58, QMetaType::QString, 0x00495003,
      59, QMetaType::QString, 0x00495003,
      60, QMetaType::QString, 0x00495003,
      61, QMetaType::QString, 0x00495003,
      62, QMetaType::QString, 0x00495003,
      63, QMetaType::QString, 0x00495003,
      64, QMetaType::QString, 0x00495003,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,
       6,
       7,
       8,
       9,
      10,

       0        // eod
};

void HornDown::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HornDown *_t = static_cast<HornDown *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->readypositionChanged(); break;
        case 1: _t->ppcPositionChanged(); break;
        case 2: _t->epcPositionChanged(); break;
        case 3: _t->mpPositionChanged(); break;
        case 4: _t->wpPositionChanged(); break;
        case 5: _t->hornPointerChanged(); break;
        case 6: _t->strokeLengthInPixelChanged(); break;
        case 7: _t->absolutePositionChanged(); break;
        case 8: _t->partContactVelocityChanged(); break;
        case 9: _t->actualForceChanged(); break;
        case 10: _t->actualPPCChanged(); break;
        case 11: _t->updateHornDownStatusText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->updateHornPositions(); break;
        case 13: _t->RefreshAllData(); break;
        case 14: _t->resetValues(); break;
        case 15: _t->messageStatusOfRequest((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->successMessageStatusOfRequest((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->UpdateStructureFromBL((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->updateEPCDataFromBL(); break;
        case 19: _t->findPartContactRequestToBL(); break;
        case 20: _t->jogUpRequestToBL(); break;
        case 21: _t->jogDownRequestToBL(); break;
        case 22: _t->setMyReadyPositionRequestToBL((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->horndownRequestToBL((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: _t->retractRequestToBL(); break;
        case 25: _t->startOrStopWeld((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: { QString _r = _t->getHornDownRememberModes();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 27: _t->setHornDownRememberModes((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7])),(*reinterpret_cast< bool(*)>(_a[8]))); break;
        case 28: _t->updateActuatorList((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->updateHornPositionValues(); break;
        case 30: _t->fillActiveRecipeForActuator((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: { QString _r = _t->getRecipeNumberForActuator();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 32: { QString _r = _t->getRecipeNameForActuator();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 33: { QString _r = _t->toPixel((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 34: { QString _r = _t->splitdata((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 35: { QString _r = _t->getStrokeLengthFormConfig();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 36: { int _r = _t->getUnitsType();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 37: { int _r = _t->getResponseType();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::readypositionChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::ppcPositionChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::epcPositionChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::mpPositionChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::wpPositionChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::hornPointerChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::strokeLengthInPixelChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::absolutePositionChanged)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::partContactVelocityChanged)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::actualForceChanged)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::actualPPCChanged)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::updateHornDownStatusText)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (HornDown::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HornDown::updateHornPositions)) {
                *result = 12;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        HornDown *_t = static_cast<HornDown *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getreadyposition(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getppcPosition(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getepcPosition(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getmpPosition(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getwpPosition(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->gethornPointer(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->getstrokeLengthInPixel(); break;
        case 7: *reinterpret_cast< QString*>(_v) = _t->getabsolutePosition(); break;
        case 8: *reinterpret_cast< QString*>(_v) = _t->getpartContactVelocity(); break;
        case 9: *reinterpret_cast< QString*>(_v) = _t->getactualForce(); break;
        case 10: *reinterpret_cast< QString*>(_v) = _t->getactualPPC(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        HornDown *_t = static_cast<HornDown *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setreadyposition(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setppcPosition(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setepcPosition(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setmpPosition(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setwpPosition(*reinterpret_cast< QString*>(_v)); break;
        case 5: _t->sethornPointer(*reinterpret_cast< QString*>(_v)); break;
        case 6: _t->setstrokeLengthInPixel(*reinterpret_cast< QString*>(_v)); break;
        case 7: _t->setabsolutePosition(*reinterpret_cast< QString*>(_v)); break;
        case 8: _t->setpartContactVelocity(*reinterpret_cast< QString*>(_v)); break;
        case 9: _t->setactualForce(*reinterpret_cast< QString*>(_v)); break;
        case 10: _t->setactualPPC(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject HornDown::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HornDown.data,
      qt_meta_data_HornDown,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HornDown::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HornDown::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HornDown.stringdata0))
        return static_cast<void*>(const_cast< HornDown*>(this));
    return QObject::qt_metacast(_clname);
}

int HornDown::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 38)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 38;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 11;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void HornDown::readypositionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HornDown::ppcPositionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HornDown::epcPositionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void HornDown::mpPositionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void HornDown::wpPositionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void HornDown::hornPointerChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void HornDown::strokeLengthInPixelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void HornDown::absolutePositionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void HornDown::partContactVelocityChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void HornDown::actualForceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void HornDown::actualPPCChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void HornDown::updateHornDownStatusText(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void HornDown::updateHornPositions()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
