/****************************************************************************
** Meta object code from reading C++ file 'graphtableview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/graphtableview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphtableview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TableRowData_t {
    QByteArrayData data[4];
    char stringdata0[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TableRowData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TableRowData_t qt_meta_stringdata_TableRowData = {
    {
QT_MOC_LITERAL(0, 0, 12), // "TableRowData"
QT_MOC_LITERAL(1, 13, 19), // "OnParamValueChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5) // "Value"

    },
    "TableRowData\0OnParamValueChanged\0\0"
    "Value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TableRowData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       1,   20, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // properties: name, type, flags
       3, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,

       0        // eod
};

void TableRowData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TableRowData *_t = static_cast<TableRowData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnParamValueChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TableRowData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TableRowData::OnParamValueChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TableRowData *_t = static_cast<TableRowData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getParamValue(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject TableRowData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TableRowData.data,
      qt_meta_data_TableRowData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TableRowData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TableRowData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TableRowData.stringdata0))
        return static_cast<void*>(const_cast< TableRowData*>(this));
    return QObject::qt_metacast(_clname);
}

int TableRowData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TableRowData::OnParamValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_TableParamName_t {
    QByteArrayData data[4];
    char stringdata0[35];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TableParamName_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TableParamName_t qt_meta_stringdata_TableParamName = {
    {
QT_MOC_LITERAL(0, 0, 14), // "TableParamName"
QT_MOC_LITERAL(1, 15, 13), // "OnNameChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 4) // "name"

    },
    "TableParamName\0OnNameChanged\0\0name"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TableParamName[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       1,   20, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // properties: name, type, flags
       3, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,

       0        // eod
};

void TableParamName::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TableParamName *_t = static_cast<TableParamName *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnNameChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TableParamName::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TableParamName::OnNameChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TableParamName *_t = static_cast<TableParamName *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getName(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject TableParamName::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TableParamName.data,
      qt_meta_data_TableParamName,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TableParamName::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TableParamName::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TableParamName.stringdata0))
        return static_cast<void*>(const_cast< TableParamName*>(this));
    return QObject::qt_metacast(_clname);
}

int TableParamName::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TableParamName::OnNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_GraphTableView_t {
    QByteArrayData data[28];
    char stringdata0[524];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GraphTableView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GraphTableView_t qt_meta_stringdata_GraphTableView = {
    {
QT_MOC_LITERAL(0, 0, 14), // "GraphTableView"
QT_MOC_LITERAL(1, 15, 10), // "addColumns"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 12), // "a_paramIndex"
QT_MOC_LITERAL(4, 40, 12), // "removeColumn"
QT_MOC_LITERAL(5, 53, 21), // "initEditRecipeHistory"
QT_MOC_LITERAL(6, 75, 13), // "initAnalytics"
QT_MOC_LITERAL(7, 89, 17), // "getWeldResultData"
QT_MOC_LITERAL(8, 107, 21), // "getLastWeldResultData"
QT_MOC_LITERAL(9, 129, 12), // "recipeNumber"
QT_MOC_LITERAL(10, 142, 27), // "addEditRecipeHistoryColumns"
QT_MOC_LITERAL(11, 170, 29), // "removeEditRecipeHistoryColumn"
QT_MOC_LITERAL(12, 200, 22), // "setGraphViewTableOrder"
QT_MOC_LITERAL(13, 223, 9), // "qstrOrder"
QT_MOC_LITERAL(14, 233, 22), // "getGraphViewTableOrder"
QT_MOC_LITERAL(15, 256, 16), // "updateOrderIndex"
QT_MOC_LITERAL(16, 273, 17), // "qstrOrderedString"
QT_MOC_LITERAL(17, 291, 13), // "getTableIndex"
QT_MOC_LITERAL(18, 305, 11), // "getListSize"
QT_MOC_LITERAL(19, 317, 30), // "getEditRecipeHistoryTableIndex"
QT_MOC_LITERAL(20, 348, 28), // "getEditRecipeHistoryListSize"
QT_MOC_LITERAL(21, 377, 30), // "isIndexInEditRecipeHistoryList"
QT_MOC_LITERAL(22, 408, 13), // "isIndexInList"
QT_MOC_LITERAL(23, 422, 23), // "getTextFromModelAtIndex"
QT_MOC_LITERAL(24, 446, 20), // "getTableElementValue"
QT_MOC_LITERAL(25, 467, 5), // "a_str"
QT_MOC_LITERAL(26, 473, 3), // "row"
QT_MOC_LITERAL(27, 477, 46) // "getTextFromModelAtIndexInEdit..."

    },
    "GraphTableView\0addColumns\0\0a_paramIndex\0"
    "removeColumn\0initEditRecipeHistory\0"
    "initAnalytics\0getWeldResultData\0"
    "getLastWeldResultData\0recipeNumber\0"
    "addEditRecipeHistoryColumns\0"
    "removeEditRecipeHistoryColumn\0"
    "setGraphViewTableOrder\0qstrOrder\0"
    "getGraphViewTableOrder\0updateOrderIndex\0"
    "qstrOrderedString\0getTableIndex\0"
    "getListSize\0getEditRecipeHistoryTableIndex\0"
    "getEditRecipeHistoryListSize\0"
    "isIndexInEditRecipeHistoryList\0"
    "isIndexInList\0getTextFromModelAtIndex\0"
    "getTableElementValue\0a_str\0row\0"
    "getTextFromModelAtIndexInEditRecipeHistoryList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GraphTableView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,  114,    2, 0x02 /* Public */,
       4,    1,  117,    2, 0x02 /* Public */,
       5,    0,  120,    2, 0x02 /* Public */,
       6,    0,  121,    2, 0x02 /* Public */,
       7,    0,  122,    2, 0x02 /* Public */,
       8,    1,  123,    2, 0x02 /* Public */,
      10,    1,  126,    2, 0x02 /* Public */,
      11,    1,  129,    2, 0x02 /* Public */,
      12,    1,  132,    2, 0x02 /* Public */,
      14,    0,  135,    2, 0x02 /* Public */,
      15,    1,  136,    2, 0x02 /* Public */,
      17,    1,  139,    2, 0x02 /* Public */,
      18,    0,  142,    2, 0x02 /* Public */,
      19,    1,  143,    2, 0x02 /* Public */,
      20,    0,  146,    2, 0x02 /* Public */,
      21,    1,  147,    2, 0x02 /* Public */,
      22,    1,  150,    2, 0x02 /* Public */,
      23,    1,  153,    2, 0x02 /* Public */,
      24,    2,  156,    2, 0x02 /* Public */,
      27,    1,  161,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Int, QMetaType::Int,    3,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,    3,
    QMetaType::Int,
    QMetaType::Bool, QMetaType::Int,    3,
    QMetaType::Bool, QMetaType::Int,    3,
    QMetaType::QString, QMetaType::Int,    3,
    QMetaType::QString, QMetaType::QString, QMetaType::Int,   25,   26,
    QMetaType::QString, QMetaType::Int,    3,

       0        // eod
};

void GraphTableView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GraphTableView *_t = static_cast<GraphTableView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addColumns((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->removeColumn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->initEditRecipeHistory(); break;
        case 3: _t->initAnalytics(); break;
        case 4: _t->getWeldResultData(); break;
        case 5: _t->getLastWeldResultData((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->addEditRecipeHistoryColumns((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->removeEditRecipeHistoryColumn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->setGraphViewTableOrder((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->getGraphViewTableOrder(); break;
        case 10: _t->updateOrderIndex((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: { int _r = _t->getTableIndex((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->getListSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 13: { int _r = _t->getEditRecipeHistoryTableIndex((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 14: { int _r = _t->getEditRecipeHistoryListSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 15: { bool _r = _t->isIndexInEditRecipeHistoryList((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 16: { bool _r = _t->isIndexInList((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 17: { QString _r = _t->getTextFromModelAtIndex((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 18: { QString _r = _t->getTableElementValue((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 19: { QString _r = _t->getTextFromModelAtIndexInEditRecipeHistoryList((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject GraphTableView::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GraphTableView.data,
      qt_meta_data_GraphTableView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *GraphTableView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphTableView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GraphTableView.stringdata0))
        return static_cast<void*>(const_cast< GraphTableView*>(this));
    return QObject::qt_metacast(_clname);
}

int GraphTableView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
