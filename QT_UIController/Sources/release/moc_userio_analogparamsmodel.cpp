/****************************************************************************
** Meta object code from reading C++ file 'userio_analogparamsmodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/userio_analogparamsmodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userio_analogparamsmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UserIOAnalogParamsModel_t {
    QByteArrayData data[21];
    char stringdata0[316];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UserIOAnalogParamsModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UserIOAnalogParamsModel_t qt_meta_stringdata_UserIOAnalogParamsModel = {
    {
QT_MOC_LITERAL(0, 0, 23), // "UserIOAnalogParamsModel"
QT_MOC_LITERAL(1, 24, 12), // "titleChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 16), // "isCheckedChanged"
QT_MOC_LITERAL(4, 55, 19), // "comboBoxListChanged"
QT_MOC_LITERAL(5, 75, 27), // "CurrentIndexComboboxChanged"
QT_MOC_LITERAL(6, 103, 16), // "IsEnabledChanged"
QT_MOC_LITERAL(7, 120, 8), // "setTitle"
QT_MOC_LITERAL(8, 129, 7), // "a_Title"
QT_MOC_LITERAL(9, 137, 12), // "setIsChecked"
QT_MOC_LITERAL(10, 150, 11), // "a_IsChecked"
QT_MOC_LITERAL(11, 162, 15), // "getComboBoxList"
QT_MOC_LITERAL(12, 178, 15), // "setComboBoxList"
QT_MOC_LITERAL(13, 194, 14), // "a_ComboBoxList"
QT_MOC_LITERAL(14, 209, 23), // "setCurrentIndexCombobox"
QT_MOC_LITERAL(15, 233, 22), // "a_CurrentIndexCombobox"
QT_MOC_LITERAL(16, 256, 5), // "Title"
QT_MOC_LITERAL(17, 262, 9), // "IsChecked"
QT_MOC_LITERAL(18, 272, 12), // "ComboBoxList"
QT_MOC_LITERAL(19, 285, 20), // "CurrentIndexCombobox"
QT_MOC_LITERAL(20, 306, 9) // "IsEnabled"

    },
    "UserIOAnalogParamsModel\0titleChanged\0"
    "\0isCheckedChanged\0comboBoxListChanged\0"
    "CurrentIndexComboboxChanged\0"
    "IsEnabledChanged\0setTitle\0a_Title\0"
    "setIsChecked\0a_IsChecked\0getComboBoxList\0"
    "setComboBoxList\0a_ComboBoxList\0"
    "setCurrentIndexCombobox\0a_CurrentIndexCombobox\0"
    "Title\0IsChecked\0ComboBoxList\0"
    "CurrentIndexCombobox\0IsEnabled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserIOAnalogParamsModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       5,   82, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    0,   66,    2, 0x06 /* Public */,
       5,    0,   67,    2, 0x06 /* Public */,
       6,    0,   68,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       7,    1,   69,    2, 0x02 /* Public */,
       9,    1,   72,    2, 0x02 /* Public */,
      11,    0,   75,    2, 0x02 /* Public */,
      12,    1,   76,    2, 0x02 /* Public */,
      14,    1,   79,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::QVariant,
    QMetaType::Void, QMetaType::QVariant,   13,
    QMetaType::Void, QMetaType::Int,   15,

 // properties: name, type, flags
      16, QMetaType::QString, 0x00495103,
      17, QMetaType::Bool, 0x00495103,
      18, QMetaType::QVariant, 0x00495103,
      19, QMetaType::Int, 0x00495103,
      20, QMetaType::Int, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,

       0        // eod
};

void UserIOAnalogParamsModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UserIOAnalogParamsModel *_t = static_cast<UserIOAnalogParamsModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->titleChanged(); break;
        case 1: _t->isCheckedChanged(); break;
        case 2: _t->comboBoxListChanged(); break;
        case 3: _t->CurrentIndexComboboxChanged(); break;
        case 4: _t->IsEnabledChanged(); break;
        case 5: _t->setTitle((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->setIsChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: { QVariant _r = _t->getComboBoxList();
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->setComboBoxList((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 9: _t->setCurrentIndexCombobox((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (UserIOAnalogParamsModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UserIOAnalogParamsModel::titleChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (UserIOAnalogParamsModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UserIOAnalogParamsModel::isCheckedChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (UserIOAnalogParamsModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UserIOAnalogParamsModel::comboBoxListChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (UserIOAnalogParamsModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UserIOAnalogParamsModel::CurrentIndexComboboxChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (UserIOAnalogParamsModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UserIOAnalogParamsModel::IsEnabledChanged)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        UserIOAnalogParamsModel *_t = static_cast<UserIOAnalogParamsModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getTitle(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->getIsChecked(); break;
        case 2: *reinterpret_cast< QVariant*>(_v) = _t->getComboBoxList(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->getCurrentIndexCombobox(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->getIsEnabled(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        UserIOAnalogParamsModel *_t = static_cast<UserIOAnalogParamsModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTitle(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setIsChecked(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setComboBoxList(*reinterpret_cast< QVariant*>(_v)); break;
        case 3: _t->setCurrentIndexCombobox(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setIsEnabled(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject UserIOAnalogParamsModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UserIOAnalogParamsModel.data,
      qt_meta_data_UserIOAnalogParamsModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *UserIOAnalogParamsModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserIOAnalogParamsModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UserIOAnalogParamsModel.stringdata0))
        return static_cast<void*>(const_cast< UserIOAnalogParamsModel*>(this));
    return QObject::qt_metacast(_clname);
}

int UserIOAnalogParamsModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
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
void UserIOAnalogParamsModel::titleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void UserIOAnalogParamsModel::isCheckedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void UserIOAnalogParamsModel::comboBoxListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void UserIOAnalogParamsModel::CurrentIndexComboboxChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void UserIOAnalogParamsModel::IsEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
