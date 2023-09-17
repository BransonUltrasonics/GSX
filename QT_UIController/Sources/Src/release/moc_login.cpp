/****************************************************************************
** Meta object code from reading C++ file 'login.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/login.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Login_t {
    QByteArrayData data[32];
    char stringdata0[493];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Login_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Login_t qt_meta_stringdata_Login = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Login"
QT_MOC_LITERAL(1, 6, 27), // "setWebserviceUserLoggedInfo"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 25), // "WebServicesLoggedUserInfo"
QT_MOC_LITERAL(4, 61, 11), // "GetUserType"
QT_MOC_LITERAL(5, 73, 23), // "setAuthorityCheckStatus"
QT_MOC_LITERAL(6, 97, 6), // "status"
QT_MOC_LITERAL(7, 104, 23), // "getAuthorityCheckStatus"
QT_MOC_LITERAL(8, 128, 23), // "getWSUserAndLevelLogged"
QT_MOC_LITERAL(9, 152, 22), // "setWSRestrictedScreens"
QT_MOC_LITERAL(10, 175, 16), // "restrictedScreen"
QT_MOC_LITERAL(11, 192, 13), // "loginValidate"
QT_MOC_LITERAL(12, 206, 10), // "a_userName"
QT_MOC_LITERAL(13, 217, 10), // "a_password"
QT_MOC_LITERAL(14, 228, 14), // "passwordEnable"
QT_MOC_LITERAL(15, 243, 15), // "a_userNameEmpty"
QT_MOC_LITERAL(16, 259, 6), // "logout"
QT_MOC_LITERAL(17, 266, 14), // "setUserDetails"
QT_MOC_LITERAL(18, 281, 8), // "userName"
QT_MOC_LITERAL(19, 290, 15), // "updateUserLogin"
QT_MOC_LITERAL(20, 306, 22), // "validatePasswordExpiry"
QT_MOC_LITERAL(21, 329, 12), // "isFirstLogin"
QT_MOC_LITERAL(22, 342, 12), // "getUserLevel"
QT_MOC_LITERAL(23, 355, 10), // "m_username"
QT_MOC_LITERAL(24, 366, 10), // "m_password"
QT_MOC_LITERAL(25, 377, 10), // "m_userType"
QT_MOC_LITERAL(26, 388, 10), // "userStatus"
QT_MOC_LITERAL(27, 399, 19), // "lastChangedPassword"
QT_MOC_LITERAL(28, 419, 19), // "IsWebServicesLogged"
QT_MOC_LITERAL(29, 439, 16), // "WSLoggedUserName"
QT_MOC_LITERAL(30, 456, 17), // "WSUserLevelStatus"
QT_MOC_LITERAL(31, 474, 18) // "UserOnModifyScreen"

    },
    "Login\0setWebserviceUserLoggedInfo\0\0"
    "WebServicesLoggedUserInfo\0GetUserType\0"
    "setAuthorityCheckStatus\0status\0"
    "getAuthorityCheckStatus\0getWSUserAndLevelLogged\0"
    "setWSRestrictedScreens\0restrictedScreen\0"
    "loginValidate\0a_userName\0a_password\0"
    "passwordEnable\0a_userNameEmpty\0logout\0"
    "setUserDetails\0userName\0updateUserLogin\0"
    "validatePasswordExpiry\0isFirstLogin\0"
    "getUserLevel\0m_username\0m_password\0"
    "m_userType\0userStatus\0lastChangedPassword\0"
    "IsWebServicesLogged\0WSLoggedUserName\0"
    "WSUserLevelStatus\0UserOnModifyScreen"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Login[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       9,  112, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x02 /* Public */,
       4,    0,   87,    2, 0x02 /* Public */,
       5,    1,   88,    2, 0x02 /* Public */,
       7,    0,   91,    2, 0x02 /* Public */,
       8,    0,   92,    2, 0x02 /* Public */,
       9,    1,   93,    2, 0x02 /* Public */,
      11,    2,   96,    2, 0x02 /* Public */,
      14,    1,  101,    2, 0x02 /* Public */,
      16,    0,  104,    2, 0x02 /* Public */,
      17,    1,  105,    2, 0x02 /* Public */,
      19,    0,  108,    2, 0x02 /* Public */,
      20,    0,  109,    2, 0x02 /* Public */,
      21,    0,  110,    2, 0x02 /* Public */,
      22,    0,  111,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::QString,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Int, QMetaType::QString, QMetaType::QString,   12,   13,
    QMetaType::Bool, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Bool,
    QMetaType::QString,

 // properties: name, type, flags
      23, QMetaType::QString, 0x00095001,
      24, QMetaType::QString, 0x00095001,
      25, QMetaType::QString, 0x00095001,
      26, QMetaType::QString, 0x00095001,
      27, QMetaType::QString, 0x00095001,
      28, QMetaType::Bool, 0x00095001,
      29, QMetaType::QString, 0x00095001,
      30, QMetaType::Int, 0x00095001,
      31, QMetaType::QString, 0x00095103,

       0        // eod
};

void Login::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Login *_t = static_cast<Login *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setWebserviceUserLoggedInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: { QString _r = _t->GetUserType();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->setAuthorityCheckStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: { int _r = _t->getAuthorityCheckStatus();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->getWSUserAndLevelLogged(); break;
        case 5: _t->setWSRestrictedScreens((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: { int _r = _t->loginValidate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->passwordEnable((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->logout(); break;
        case 9: _t->setUserDetails((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->updateUserLogin(); break;
        case 11: { int _r = _t->validatePasswordExpiry();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: { bool _r = _t->isFirstLogin();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 13: { QString _r = _t->getUserLevel();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Login *_t = static_cast<Login *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->GetLoginUserName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->GetPassword(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->GetUserType(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->GetUserStatus(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getLastChangedPassword(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->getIsWebServicesLogged(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->getWSLoggedUserName(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->getWSUserLevelStatus(); break;
        case 8: *reinterpret_cast< QString*>(_v) = _t->getUserOnModifyScreen(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Login *_t = static_cast<Login *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 8: _t->setUserOnModifyScreen(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Login::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Login.data,
      qt_meta_data_Login,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Login::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Login::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Login.stringdata0))
        return static_cast<void*>(const_cast< Login*>(this));
    return QObject::qt_metacast(_clname);
}

int Login::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
