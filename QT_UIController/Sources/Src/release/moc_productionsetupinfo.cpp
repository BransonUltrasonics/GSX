/****************************************************************************
** Meta object code from reading C++ file 'productionsetupinfo.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/productionsetupinfo.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'productionsetupinfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ProductionSetUpInfo_t {
    QByteArrayData data[24];
    char stringdata0[446];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProductionSetUpInfo_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProductionSetUpInfo_t qt_meta_stringdata_ProductionSetUpInfo = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ProductionSetUpInfo"
QT_MOC_LITERAL(1, 20, 13), // "setBatchCount"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 13), // "setCycleCount"
QT_MOC_LITERAL(4, 49, 24), // "setProductionInstruction"
QT_MOC_LITERAL(5, 74, 20), // "setRecipeDescription"
QT_MOC_LITERAL(6, 95, 19), // "setBatchSetupEnable"
QT_MOC_LITERAL(7, 115, 23), // "setCountWithAlarmEnable"
QT_MOC_LITERAL(8, 139, 10), // "setBatchId"
QT_MOC_LITERAL(9, 150, 19), // "getBatchSetupEnable"
QT_MOC_LITERAL(10, 170, 23), // "getCountWithAlarmEnable"
QT_MOC_LITERAL(11, 194, 15), // "getRecipeNumber"
QT_MOC_LITERAL(12, 210, 13), // "getRecipeName"
QT_MOC_LITERAL(13, 224, 13), // "getBatchCount"
QT_MOC_LITERAL(14, 238, 13), // "getCycleCount"
QT_MOC_LITERAL(15, 252, 24), // "getProductionInstruction"
QT_MOC_LITERAL(16, 277, 20), // "getRecipeDescription"
QT_MOC_LITERAL(17, 298, 10), // "getBatchId"
QT_MOC_LITERAL(18, 309, 19), // "recipeSetupInfoInit"
QT_MOC_LITERAL(19, 329, 8), // "savetoDB"
QT_MOC_LITERAL(20, 338, 15), // "checkBatchCount"
QT_MOC_LITERAL(21, 354, 34), // "getActiveRecipeNumberForProdu..."
QT_MOC_LITERAL(22, 389, 28), // "sendResetBatchCounterReqToSC"
QT_MOC_LITERAL(23, 418, 27) // "replaceBatchCountWithOldVal"

    },
    "ProductionSetUpInfo\0setBatchCount\0\0"
    "setCycleCount\0setProductionInstruction\0"
    "setRecipeDescription\0setBatchSetupEnable\0"
    "setCountWithAlarmEnable\0setBatchId\0"
    "getBatchSetupEnable\0getCountWithAlarmEnable\0"
    "getRecipeNumber\0getRecipeName\0"
    "getBatchCount\0getCycleCount\0"
    "getProductionInstruction\0getRecipeDescription\0"
    "getBatchId\0recipeSetupInfoInit\0savetoDB\0"
    "checkBatchCount\0getActiveRecipeNumberForProduction\0"
    "sendResetBatchCounterReqToSC\0"
    "replaceBatchCountWithOldVal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProductionSetUpInfo[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,  124,    2, 0x02 /* Public */,
       3,    1,  127,    2, 0x02 /* Public */,
       4,    1,  130,    2, 0x02 /* Public */,
       5,    1,  133,    2, 0x02 /* Public */,
       6,    1,  136,    2, 0x02 /* Public */,
       7,    1,  139,    2, 0x02 /* Public */,
       8,    1,  142,    2, 0x02 /* Public */,
       9,    0,  145,    2, 0x02 /* Public */,
      10,    0,  146,    2, 0x02 /* Public */,
      11,    0,  147,    2, 0x02 /* Public */,
      12,    0,  148,    2, 0x02 /* Public */,
      13,    0,  149,    2, 0x02 /* Public */,
      14,    0,  150,    2, 0x02 /* Public */,
      15,    0,  151,    2, 0x02 /* Public */,
      16,    0,  152,    2, 0x02 /* Public */,
      17,    0,  153,    2, 0x02 /* Public */,
      18,    2,  154,    2, 0x02 /* Public */,
      19,    0,  159,    2, 0x02 /* Public */,
      20,    0,  160,    2, 0x02 /* Public */,
      21,    0,  161,    2, 0x02 /* Public */,
      22,    0,  162,    2, 0x02 /* Public */,
      23,    0,  163,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::QString,
    QMetaType::Int,
    QMetaType::Void,

       0        // eod
};

void ProductionSetUpInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ProductionSetUpInfo *_t = static_cast<ProductionSetUpInfo *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setBatchCount((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setCycleCount((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setProductionInstruction((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setRecipeDescription((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setBatchSetupEnable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setCountWithAlarmEnable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setBatchId((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: { int _r = _t->getBatchSetupEnable();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 8: { int _r = _t->getCountWithAlarmEnable();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { QString _r = _t->getRecipeNumber();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 10: { QString _r = _t->getRecipeName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 11: { QString _r = _t->getBatchCount();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 12: { QString _r = _t->getCycleCount();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 13: { QString _r = _t->getProductionInstruction();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 14: { QString _r = _t->getRecipeDescription();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 15: { QString _r = _t->getBatchId();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->recipeSetupInfoInit((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 17: _t->savetoDB(); break;
        case 18: { int _r = _t->checkBatchCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 19: { QString _r = _t->getActiveRecipeNumberForProduction();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 20: { int _r = _t->sendResetBatchCounterReqToSC();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 21: _t->replaceBatchCountWithOldVal(); break;
        default: ;
        }
    }
}

const QMetaObject ProductionSetUpInfo::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ProductionSetUpInfo.data,
      qt_meta_data_ProductionSetUpInfo,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ProductionSetUpInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProductionSetUpInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ProductionSetUpInfo.stringdata0))
        return static_cast<void*>(const_cast< ProductionSetUpInfo*>(this));
    return QObject::qt_metacast(_clname);
}

int ProductionSetUpInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
