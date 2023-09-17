/****************************************************************************
** Meta object code from reading C++ file 'errorhandling.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/errorhandling.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'errorhandling.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ErrorHandling_t {
    QByteArrayData data[6];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ErrorHandling_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ErrorHandling_t qt_meta_stringdata_ErrorHandling = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ErrorHandling"
QT_MOC_LITERAL(1, 14, 10), // "ErrorCodes"
QT_MOC_LITERAL(2, 25, 13), // "ERROR_UNKNOWN"
QT_MOC_LITERAL(3, 39, 6), // "SUCESS"
QT_MOC_LITERAL(4, 46, 19), // "ERROR_LOGIN_FAILURE"
QT_MOC_LITERAL(5, 66, 17) // "ERROR_USER_LOCKED"

    },
    "ErrorHandling\0ErrorCodes\0ERROR_UNKNOWN\0"
    "SUCESS\0ERROR_LOGIN_FAILURE\0ERROR_USER_LOCKED"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ErrorHandling[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x0,    4,   18,

 // enum data: key, value
       2, uint(ErrorHandling::ERROR_UNKNOWN),
       3, uint(ErrorHandling::SUCESS),
       4, uint(ErrorHandling::ERROR_LOGIN_FAILURE),
       5, uint(ErrorHandling::ERROR_USER_LOCKED),

       0        // eod
};

void ErrorHandling::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ErrorHandling::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ErrorHandling.data,
      qt_meta_data_ErrorHandling,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ErrorHandling::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ErrorHandling::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ErrorHandling.stringdata0))
        return static_cast<void*>(const_cast< ErrorHandling*>(this));
    return QObject::qt_metacast(_clname);
}

int ErrorHandling::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
