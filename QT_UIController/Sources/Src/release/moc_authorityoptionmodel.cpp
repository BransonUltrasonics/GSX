/****************************************************************************
** Meta object code from reading C++ file 'authorityoptionmodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/authorityoptionmodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'authorityoptionmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConfigurationData_t {
    QByteArrayData data[9];
    char stringdata0[80];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConfigurationData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConfigurationData_t qt_meta_stringdata_ConfigurationData = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ConfigurationData"
QT_MOC_LITERAL(1, 18, 12), // "indexChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 5), // "value"
QT_MOC_LITERAL(4, 38, 11), // "nameChanged"
QT_MOC_LITERAL(5, 50, 12), // "stateChanged"
QT_MOC_LITERAL(6, 63, 5), // "index"
QT_MOC_LITERAL(7, 69, 4), // "name"
QT_MOC_LITERAL(8, 74, 5) // "state"

    },
    "ConfigurationData\0indexChanged\0\0value\0"
    "nameChanged\0stateChanged\0index\0name\0"
    "state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConfigurationData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       3,   38, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,
       5,    1,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    3,

 // properties: name, type, flags
       6, QMetaType::Int, 0x00495003,
       7, QMetaType::QString, 0x00495003,
       8, QMetaType::Bool, 0x00495003,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

void ConfigurationData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConfigurationData *_t = static_cast<ConfigurationData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->indexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->nameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->stateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConfigurationData::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConfigurationData::indexChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ConfigurationData::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConfigurationData::nameChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ConfigurationData::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConfigurationData::stateChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        ConfigurationData *_t = static_cast<ConfigurationData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->index(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->name(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->state(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        ConfigurationData *_t = static_cast<ConfigurationData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->index(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->name(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->state(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject ConfigurationData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ConfigurationData.data,
      qt_meta_data_ConfigurationData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ConfigurationData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConfigurationData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConfigurationData.stringdata0))
        return static_cast<void*>(const_cast< ConfigurationData*>(this));
    return QObject::qt_metacast(_clname);
}

int ConfigurationData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ConfigurationData::indexChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConfigurationData::nameChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ConfigurationData::stateChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_AuthorityOptionModel_t {
    QByteArrayData data[6];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AuthorityOptionModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AuthorityOptionModel_t qt_meta_stringdata_AuthorityOptionModel = {
    {
QT_MOC_LITERAL(0, 0, 20), // "AuthorityOptionModel"
QT_MOC_LITERAL(1, 21, 10), // "updateList"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 9), // "userLevel"
QT_MOC_LITERAL(4, 43, 24), // "authorityOptionModelInit"
QT_MOC_LITERAL(5, 68, 15) // "getDefaultIndex"

    },
    "AuthorityOptionModel\0updateList\0\0"
    "userLevel\0authorityOptionModelInit\0"
    "getDefaultIndex"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AuthorityOptionModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x02 /* Public */,
       4,    0,   32,    2, 0x02 /* Public */,
       5,    0,   33,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Int,

       0        // eod
};

void AuthorityOptionModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AuthorityOptionModel *_t = static_cast<AuthorityOptionModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateList((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->authorityOptionModelInit(); break;
        case 2: { int _r = _t->getDefaultIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject AuthorityOptionModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AuthorityOptionModel.data,
      qt_meta_data_AuthorityOptionModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AuthorityOptionModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AuthorityOptionModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AuthorityOptionModel.stringdata0))
        return static_cast<void*>(const_cast< AuthorityOptionModel*>(this));
    return QObject::qt_metacast(_clname);
}

int AuthorityOptionModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
