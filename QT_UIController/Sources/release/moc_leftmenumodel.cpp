/****************************************************************************
** Meta object code from reading C++ file 'leftmenumodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/leftmenumodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'leftmenumodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LeftMenuData_t {
    QByteArrayData data[5];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LeftMenuData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LeftMenuData_t qt_meta_stringdata_LeftMenuData = {
    {
QT_MOC_LITERAL(0, 0, 12), // "LeftMenuData"
QT_MOC_LITERAL(1, 13, 13), // "resetLeftMenu"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 11), // "resetParent"
QT_MOC_LITERAL(4, 40, 10) // "parentName"

    },
    "LeftMenuData\0resetLeftMenu\0\0resetParent\0"
    "parentName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LeftMenuData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x02 /* Public */,
       3,    1,   25,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,

       0        // eod
};

void LeftMenuData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LeftMenuData *_t = static_cast<LeftMenuData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->resetLeftMenu(); break;
        case 1: _t->resetParent((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject LeftMenuData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LeftMenuData.data,
      qt_meta_data_LeftMenuData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LeftMenuData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LeftMenuData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LeftMenuData.stringdata0))
        return static_cast<void*>(const_cast< LeftMenuData*>(this));
    return QObject::qt_metacast(_clname);
}

int LeftMenuData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_LeftMenuModel_t {
    QByteArrayData data[12];
    char stringdata0[162];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LeftMenuModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LeftMenuModel_t qt_meta_stringdata_LeftMenuModel = {
    {
QT_MOC_LITERAL(0, 0, 13), // "LeftMenuModel"
QT_MOC_LITERAL(1, 14, 19), // "OnMenuOptionChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 17), // "OnMenuIconChanged"
QT_MOC_LITERAL(4, 53, 19), // "OnMenuActionChanged"
QT_MOC_LITERAL(5, 73, 18), // "OnMenuColorChanged"
QT_MOC_LITERAL(6, 92, 18), // "OnMenuIndexChanged"
QT_MOC_LITERAL(7, 111, 10), // "MenuOption"
QT_MOC_LITERAL(8, 122, 8), // "MenuIcon"
QT_MOC_LITERAL(9, 131, 10), // "MenuAction"
QT_MOC_LITERAL(10, 142, 9), // "MenuColor"
QT_MOC_LITERAL(11, 152, 9) // "MenuIndex"

    },
    "LeftMenuModel\0OnMenuOptionChanged\0\0"
    "OnMenuIconChanged\0OnMenuActionChanged\0"
    "OnMenuColorChanged\0OnMenuIndexChanged\0"
    "MenuOption\0MenuIcon\0MenuAction\0MenuColor\0"
    "MenuIndex"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LeftMenuModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       5,   44, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    0,   41,    2, 0x06 /* Public */,
       5,    0,   42,    2, 0x06 /* Public */,
       6,    0,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       7, QMetaType::QString, 0x00495003,
       8, QMetaType::QString, 0x00495003,
       9, QMetaType::QString, 0x00495003,
      10, QMetaType::QString, 0x00495003,
      11, QMetaType::Int, 0x00495003,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,

       0        // eod
};

void LeftMenuModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LeftMenuModel *_t = static_cast<LeftMenuModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnMenuOptionChanged(); break;
        case 1: _t->OnMenuIconChanged(); break;
        case 2: _t->OnMenuActionChanged(); break;
        case 3: _t->OnMenuColorChanged(); break;
        case 4: _t->OnMenuIndexChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LeftMenuModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LeftMenuModel::OnMenuOptionChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (LeftMenuModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LeftMenuModel::OnMenuIconChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (LeftMenuModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LeftMenuModel::OnMenuActionChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (LeftMenuModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LeftMenuModel::OnMenuColorChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (LeftMenuModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LeftMenuModel::OnMenuIndexChanged)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        LeftMenuModel *_t = static_cast<LeftMenuModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->GetMenuOption(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->GetMenuIcon(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->GetMenuAction(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->GetMenuColor(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->GetMenuIndex(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        LeftMenuModel *_t = static_cast<LeftMenuModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->SetMenuOption(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->SetMenuIcon(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->SetMenuAction(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->SetMenuColor(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->SetMenuIndex(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject LeftMenuModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LeftMenuModel.data,
      qt_meta_data_LeftMenuModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LeftMenuModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LeftMenuModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LeftMenuModel.stringdata0))
        return static_cast<void*>(const_cast< LeftMenuModel*>(this));
    return QObject::qt_metacast(_clname);
}

int LeftMenuModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void LeftMenuModel::OnMenuOptionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void LeftMenuModel::OnMenuIconChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void LeftMenuModel::OnMenuActionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void LeftMenuModel::OnMenuColorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void LeftMenuModel::OnMenuIndexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
