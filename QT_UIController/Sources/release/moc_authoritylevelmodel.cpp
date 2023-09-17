/****************************************************************************
** Meta object code from reading C++ file 'authoritylevelmodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/authoritylevelmodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'authoritylevelmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AuthorityLevelModel_t {
    QByteArrayData data[11];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AuthorityLevelModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AuthorityLevelModel_t qt_meta_stringdata_AuthorityLevelModel = {
    {
QT_MOC_LITERAL(0, 0, 19), // "AuthorityLevelModel"
QT_MOC_LITERAL(1, 20, 15), // "updateAuthority"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 12), // "permissionId"
QT_MOC_LITERAL(4, 50, 6), // "module"
QT_MOC_LITERAL(5, 57, 5), // "title"
QT_MOC_LITERAL(6, 63, 5), // "grant"
QT_MOC_LITERAL(7, 69, 23), // "sendAuthorityStatusToDB"
QT_MOC_LITERAL(8, 93, 12), // "addAuthority"
QT_MOC_LITERAL(9, 106, 3), // "get"
QT_MOC_LITERAL(10, 110, 3) // "row"

    },
    "AuthorityLevelModel\0updateAuthority\0"
    "\0permissionId\0module\0title\0grant\0"
    "sendAuthorityStatusToDB\0addAuthority\0"
    "get\0row"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AuthorityLevelModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    4,   34,    2, 0x02 /* Public */,
       7,    0,   43,    2, 0x02 /* Public */,
       8,    4,   44,    2, 0x02 /* Public */,
       9,    1,   53,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::Int,    3,    4,    5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::Int,    3,    4,    5,    6,
    QMetaType::QVariantMap, QMetaType::Int,   10,

       0        // eod
};

void AuthorityLevelModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AuthorityLevelModel *_t = static_cast<AuthorityLevelModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateAuthority((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: _t->sendAuthorityStatusToDB(); break;
        case 2: _t->addAuthority((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: { QVariantMap _r = _t->get((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject AuthorityLevelModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_AuthorityLevelModel.data,
      qt_meta_data_AuthorityLevelModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AuthorityLevelModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AuthorityLevelModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AuthorityLevelModel.stringdata0))
        return static_cast<void*>(const_cast< AuthorityLevelModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int AuthorityLevelModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
