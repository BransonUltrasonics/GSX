/****************************************************************************
** Meta object code from reading C++ file 'newpassword.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/newpassword.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'newpassword.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NewPassword_t {
    QByteArrayData data[14];
    char stringdata0[193];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NewPassword_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NewPassword_t qt_meta_stringdata_NewPassword = {
    {
QT_MOC_LITERAL(0, 0, 11), // "NewPassword"
QT_MOC_LITERAL(1, 12, 9), // "setUserID"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 8), // "a_UserID"
QT_MOC_LITERAL(4, 32, 14), // "setNewPassword"
QT_MOC_LITERAL(5, 47, 13), // "a_NewPassword"
QT_MOC_LITERAL(6, 61, 9), // "getUserID"
QT_MOC_LITERAL(7, 71, 18), // "getCurrentPassword"
QT_MOC_LITERAL(8, 90, 17), // "getPasswordFromDB"
QT_MOC_LITERAL(9, 108, 15), // "isUserDataValid"
QT_MOC_LITERAL(10, 124, 17), // "a_CurrentPassword"
QT_MOC_LITERAL(11, 142, 21), // "isConfirmPasswordSame"
QT_MOC_LITERAL(12, 164, 17), // "a_ConfirmPassword"
QT_MOC_LITERAL(13, 182, 10) // "setContext"

    },
    "NewPassword\0setUserID\0\0a_UserID\0"
    "setNewPassword\0a_NewPassword\0getUserID\0"
    "getCurrentPassword\0getPasswordFromDB\0"
    "isUserDataValid\0a_CurrentPassword\0"
    "isConfirmPasswordSame\0a_ConfirmPassword\0"
    "setContext"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NewPassword[] = {

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
       1,    1,   54,    2, 0x02 /* Public */,
       4,    1,   57,    2, 0x02 /* Public */,
       6,    0,   60,    2, 0x02 /* Public */,
       7,    0,   61,    2, 0x02 /* Public */,
       8,    0,   62,    2, 0x02 /* Public */,
       9,    2,   63,    2, 0x02 /* Public */,
      11,    2,   68,    2, 0x02 /* Public */,
      13,    0,   73,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,    3,   10,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,    5,   12,
    QMetaType::Void,

       0        // eod
};

void NewPassword::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NewPassword *_t = static_cast<NewPassword *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setUserID((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setNewPassword((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: { QString _r = _t->getUserID();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->getCurrentPassword();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->getPasswordFromDB(); break;
        case 5: { bool _r = _t->isUserDataValid((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->isConfirmPasswordSame((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->setContext(); break;
        default: ;
        }
    }
}

const QMetaObject NewPassword::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NewPassword.data,
      qt_meta_data_NewPassword,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *NewPassword::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NewPassword::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NewPassword.stringdata0))
        return static_cast<void*>(const_cast< NewPassword*>(this));
    return QObject::qt_metacast(_clname);
}

int NewPassword::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
