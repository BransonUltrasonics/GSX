/****************************************************************************
** Meta object code from reading C++ file 'RecipeInformation.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/RecipeInformation.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RecipeInformation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RecipeInformation_t {
    QByteArrayData data[12];
    char stringdata0[195];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RecipeInformation_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RecipeInformation_t qt_meta_stringdata_RecipeInformation = {
    {
QT_MOC_LITERAL(0, 0, 17), // "RecipeInformation"
QT_MOC_LITERAL(1, 18, 14), // "recipeInfoInit"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 16), // "qStrRecipeNumber"
QT_MOC_LITERAL(4, 51, 14), // "qStrRecipeName"
QT_MOC_LITERAL(5, 66, 17), // "getInfoRecipeName"
QT_MOC_LITERAL(6, 84, 20), // "getRecipeDescription"
QT_MOC_LITERAL(7, 105, 24), // "getProductionInstruction"
QT_MOC_LITERAL(8, 130, 10), // "getCompany"
QT_MOC_LITERAL(9, 141, 14), // "getControlmode"
QT_MOC_LITERAL(10, 156, 19), // "getControlmodeValue"
QT_MOC_LITERAL(11, 176, 18) // "getControlModeUnit"

    },
    "RecipeInformation\0recipeInfoInit\0\0"
    "qStrRecipeNumber\0qStrRecipeName\0"
    "getInfoRecipeName\0getRecipeDescription\0"
    "getProductionInstruction\0getCompany\0"
    "getControlmode\0getControlmodeValue\0"
    "getControlModeUnit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RecipeInformation[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x02 /* Public */,
       5,    0,   59,    2, 0x02 /* Public */,
       6,    0,   60,    2, 0x02 /* Public */,
       7,    0,   61,    2, 0x02 /* Public */,
       8,    0,   62,    2, 0x02 /* Public */,
       9,    0,   63,    2, 0x02 /* Public */,
      10,    0,   64,    2, 0x02 /* Public */,
      11,    0,   65,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,

       0        // eod
};

void RecipeInformation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RecipeInformation *_t = static_cast<RecipeInformation *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->recipeInfoInit((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: { QString _r = _t->getInfoRecipeName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 2: { QString _r = _t->getRecipeDescription();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->getProductionInstruction();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->getCompany();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->getControlmode();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 6: { QString _r = _t->getControlmodeValue();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 7: { QString _r = _t->getControlModeUnit();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject RecipeInformation::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RecipeInformation.data,
      qt_meta_data_RecipeInformation,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RecipeInformation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RecipeInformation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RecipeInformation.stringdata0))
        return static_cast<void*>(const_cast< RecipeInformation*>(this));
    return QObject::qt_metacast(_clname);
}

int RecipeInformation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
