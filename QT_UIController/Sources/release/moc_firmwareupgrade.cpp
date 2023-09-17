/****************************************************************************
** Meta object code from reading C++ file 'firmwareupgrade.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/firmwareupgrade.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'firmwareupgrade.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FirmwareUpgrade_t {
    QByteArrayData data[23];
    char stringdata0[314];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FirmwareUpgrade_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FirmwareUpgrade_t qt_meta_stringdata_FirmwareUpgrade = {
    {
QT_MOC_LITERAL(0, 0, 15), // "FirmwareUpgrade"
QT_MOC_LITERAL(1, 16, 23), // "upgradeCompletionStatus"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 9), // "statusVal"
QT_MOC_LITERAL(4, 51, 21), // "upgradeProgressDetail"
QT_MOC_LITERAL(5, 73, 7), // "nModule"
QT_MOC_LITERAL(6, 81, 9), // "nProgress"
QT_MOC_LITERAL(7, 91, 7), // "readUSB"
QT_MOC_LITERAL(8, 99, 16), // "upgradeFirmwares"
QT_MOC_LITERAL(9, 116, 7), // "scIndex"
QT_MOC_LITERAL(10, 124, 7), // "acIndex"
QT_MOC_LITERAL(11, 132, 7), // "pcIndex"
QT_MOC_LITERAL(12, 140, 11), // "upgradeType"
QT_MOC_LITERAL(13, 152, 12), // "getfileslist"
QT_MOC_LITERAL(14, 165, 11), // "nIdentifier"
QT_MOC_LITERAL(15, 177, 12), // "upgradePopup"
QT_MOC_LITERAL(16, 190, 12), // "upgradeFiles"
QT_MOC_LITERAL(17, 203, 13), // "clearfilelist"
QT_MOC_LITERAL(18, 217, 16), // "getUpgradeStatus"
QT_MOC_LITERAL(19, 234, 19), // "Firmwareupgradetype"
QT_MOC_LITERAL(20, 254, 19), // "ACTUATOR_CONTROLLER"
QT_MOC_LITERAL(21, 274, 16), // "POWER_CONTROLLER"
QT_MOC_LITERAL(22, 291, 22) // "SUPERVISORY_CONTROLLER"

    },
    "FirmwareUpgrade\0upgradeCompletionStatus\0"
    "\0statusVal\0upgradeProgressDetail\0"
    "nModule\0nProgress\0readUSB\0upgradeFirmwares\0"
    "scIndex\0acIndex\0pcIndex\0upgradeType\0"
    "getfileslist\0nIdentifier\0upgradePopup\0"
    "upgradeFiles\0clearfilelist\0getUpgradeStatus\0"
    "Firmwareupgradetype\0ACTUATOR_CONTROLLER\0"
    "POWER_CONTROLLER\0SUPERVISORY_CONTROLLER"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FirmwareUpgrade[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       1,   80, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    2,   57,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       7,    0,   62,    2, 0x02 /* Public */,
       8,    4,   63,    2, 0x02 /* Public */,
      13,    1,   72,    2, 0x02 /* Public */,
      15,    1,   75,    2, 0x02 /* Public */,
      17,    0,   78,    2, 0x02 /* Public */,
      18,    0,   79,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,

 // methods: parameters
    QMetaType::Int,
    QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    9,   10,   11,   12,
    QMetaType::QStringList, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void,
    QMetaType::Bool,

 // enums: name, flags, count, data
      19, 0x0,    3,   84,

 // enum data: key, value
      20, uint(FirmwareUpgrade::ACTUATOR_CONTROLLER),
      21, uint(FirmwareUpgrade::POWER_CONTROLLER),
      22, uint(FirmwareUpgrade::SUPERVISORY_CONTROLLER),

       0        // eod
};

void FirmwareUpgrade::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FirmwareUpgrade *_t = static_cast<FirmwareUpgrade *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->upgradeCompletionStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->upgradeProgressDetail((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: { int _r = _t->readUSB();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->upgradeFirmwares((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QStringList _r = _t->getfileslist((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->upgradePopup((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->clearfilelist(); break;
        case 7: { bool _r = _t->getUpgradeStatus();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FirmwareUpgrade::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FirmwareUpgrade::upgradeCompletionStatus)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FirmwareUpgrade::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FirmwareUpgrade::upgradeProgressDetail)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject FirmwareUpgrade::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FirmwareUpgrade.data,
      qt_meta_data_FirmwareUpgrade,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FirmwareUpgrade::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FirmwareUpgrade::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FirmwareUpgrade.stringdata0))
        return static_cast<void*>(const_cast< FirmwareUpgrade*>(this));
    return QObject::qt_metacast(_clname);
}

int FirmwareUpgrade::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void FirmwareUpgrade::upgradeCompletionStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FirmwareUpgrade::upgradeProgressDetail(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
