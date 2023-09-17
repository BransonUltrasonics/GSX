/****************************************************************************
** Meta object code from reading C++ file 'graph.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/graph.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Graph_t {
    QByteArrayData data[29];
    char stringdata0[331];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Graph_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Graph_t qt_meta_stringdata_Graph = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Graph"
QT_MOC_LITERAL(1, 6, 17), // "XAxisValueChanged"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 16), // "GraphTimeChanged"
QT_MOC_LITERAL(4, 42, 13), // "replaceSample"
QT_MOC_LITERAL(5, 56, 5), // "index"
QT_MOC_LITERAL(6, 62, 4), // "copy"
QT_MOC_LITERAL(7, 67, 16), // "QAbstractSeries*"
QT_MOC_LITERAL(8, 84, 14), // "QSplineSeries*"
QT_MOC_LITERAL(9, 99, 7), // "addAxes"
QT_MOC_LITERAL(10, 107, 8), // "addGraph"
QT_MOC_LITERAL(11, 116, 11), // "colorAtAxis"
QT_MOC_LITERAL(12, 128, 10), // "nameAtAxis"
QT_MOC_LITERAL(13, 139, 12), // "colorAtGraph"
QT_MOC_LITERAL(14, 152, 11), // "nameAtGraph"
QT_MOC_LITERAL(15, 164, 10), // "noOfGraphs"
QT_MOC_LITERAL(16, 175, 16), // "getSignatureData"
QT_MOC_LITERAL(17, 192, 6), // "getMin"
QT_MOC_LITERAL(18, 199, 6), // "getMax"
QT_MOC_LITERAL(19, 206, 13), // "setVisibility"
QT_MOC_LITERAL(20, 220, 14), // "QAbstractAxis*"
QT_MOC_LITERAL(21, 235, 10), // "getAtIndex"
QT_MOC_LITERAL(22, 246, 11), // "getMinPoint"
QT_MOC_LITERAL(23, 258, 11), // "getMaxPoint"
QT_MOC_LITERAL(24, 270, 10), // "setMin_Max"
QT_MOC_LITERAL(25, 281, 10), // "clearLists"
QT_MOC_LITERAL(26, 292, 17), // "addAxesAndSplines"
QT_MOC_LITERAL(27, 310, 10), // "XAxisValue"
QT_MOC_LITERAL(28, 321, 9) // "GraphTime"

    },
    "Graph\0XAxisValueChanged\0\0GraphTimeChanged\0"
    "replaceSample\0index\0copy\0QAbstractSeries*\0"
    "QSplineSeries*\0addAxes\0addGraph\0"
    "colorAtAxis\0nameAtAxis\0colorAtGraph\0"
    "nameAtGraph\0noOfGraphs\0getSignatureData\0"
    "getMin\0getMax\0setVisibility\0QAbstractAxis*\0"
    "getAtIndex\0getMinPoint\0getMaxPoint\0"
    "setMin_Max\0clearLists\0addAxesAndSplines\0"
    "XAxisValue\0GraphTime"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Graph[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       2,  176, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  119,    2, 0x06 /* Public */,
       3,    0,  120,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,  121,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       6,    1,  124,    2, 0x02 /* Public */,
       9,    0,  127,    2, 0x02 /* Public */,
      10,    0,  128,    2, 0x02 /* Public */,
      11,    1,  129,    2, 0x02 /* Public */,
      12,    1,  132,    2, 0x02 /* Public */,
      13,    1,  135,    2, 0x02 /* Public */,
      14,    1,  138,    2, 0x02 /* Public */,
      15,    0,  141,    2, 0x02 /* Public */,
      16,    0,  142,    2, 0x02 /* Public */,
      17,    1,  143,    2, 0x02 /* Public */,
      18,    1,  146,    2, 0x02 /* Public */,
      19,    3,  149,    2, 0x02 /* Public */,
      21,    2,  156,    2, 0x02 /* Public */,
      22,    1,  161,    2, 0x02 /* Public */,
      23,    1,  164,    2, 0x02 /* Public */,
      24,    3,  167,    2, 0x02 /* Public */,
      25,    0,  174,    2, 0x02 /* Public */,
      26,    0,  175,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,

 // methods: parameters
    0x80000000 | 7, 0x80000000 | 8,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int,    2,
    QMetaType::QString, QMetaType::Int,    2,
    QMetaType::QString, QMetaType::Int,    2,
    QMetaType::QString, QMetaType::Int,    2,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::QPointF, 0x80000000 | 8,    2,
    QMetaType::QPointF, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 20, QMetaType::Bool,    2,    2,    2,
    QMetaType::QPointF, 0x80000000 | 8, QMetaType::Int,    2,    2,
    QMetaType::QPointF, QMetaType::Int,    2,
    QMetaType::QPointF, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 20, QMetaType::QReal, QMetaType::QReal,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      27, QMetaType::Double, 0x00495103,
      28, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void Graph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Graph *_t = static_cast<Graph *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->XAxisValueChanged(); break;
        case 1: _t->GraphTimeChanged(); break;
        case 2: _t->replaceSample((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: { QAbstractSeries* _r = _t->copy((*reinterpret_cast< QSplineSeries*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QAbstractSeries**>(_a[0]) = std::move(_r); }  break;
        case 4: _t->addAxes(); break;
        case 5: _t->addGraph(); break;
        case 6: { QString _r = _t->colorAtAxis((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 7: { QString _r = _t->nameAtAxis((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 8: { QString _r = _t->colorAtGraph((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 9: { QString _r = _t->nameAtGraph((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->noOfGraphs();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 11: { int _r = _t->getSignatureData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: { QPointF _r = _t->getMin((*reinterpret_cast< QSplineSeries*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QPointF*>(_a[0]) = std::move(_r); }  break;
        case 13: { QPointF _r = _t->getMax((*reinterpret_cast< QSplineSeries*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QPointF*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->setVisibility((*reinterpret_cast< QSplineSeries*(*)>(_a[1])),(*reinterpret_cast< QAbstractAxis*(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 15: { QPointF _r = _t->getAtIndex((*reinterpret_cast< QSplineSeries*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QPointF*>(_a[0]) = std::move(_r); }  break;
        case 16: { QPointF _r = _t->getMinPoint((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QPointF*>(_a[0]) = std::move(_r); }  break;
        case 17: { QPointF _r = _t->getMaxPoint((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QPointF*>(_a[0]) = std::move(_r); }  break;
        case 18: _t->setMin_Max((*reinterpret_cast< QAbstractAxis*(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3]))); break;
        case 19: _t->clearLists(); break;
        case 20: _t->addAxesAndSplines(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSplineSeries* >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSplineSeries* >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSplineSeries* >(); break;
            }
            break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractAxis* >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSplineSeries* >(); break;
            }
            break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSplineSeries* >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractAxis* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Graph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Graph::XAxisValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Graph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Graph::GraphTimeChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Graph *_t = static_cast<Graph *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->getXAxisValue(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getGraphTime(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Graph *_t = static_cast<Graph *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setXAxisValue(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setGraphTime(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Graph::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Graph.data,
      qt_meta_data_Graph,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Graph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Graph::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Graph.stringdata0))
        return static_cast<void*>(const_cast< Graph*>(this));
    return QObject::qt_metacast(_clname);
}

int Graph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Graph::XAxisValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Graph::GraphTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_GraphProperties_t {
    QByteArrayData data[3];
    char stringdata0[27];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GraphProperties_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GraphProperties_t qt_meta_stringdata_GraphProperties = {
    {
QT_MOC_LITERAL(0, 0, 15), // "GraphProperties"
QT_MOC_LITERAL(1, 16, 4), // "name"
QT_MOC_LITERAL(2, 21, 5) // "color"

    },
    "GraphProperties\0name\0color"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GraphProperties[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QString, 0x00095001,
       2, QMetaType::QString, 0x00095001,

       0        // eod
};

void GraphProperties::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        GraphProperties *_t = static_cast<GraphProperties *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getColor(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject GraphProperties::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GraphProperties.data,
      qt_meta_data_GraphProperties,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *GraphProperties::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphProperties::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GraphProperties.stringdata0))
        return static_cast<void*>(const_cast< GraphProperties*>(this));
    return QObject::qt_metacast(_clname);
}

int GraphProperties::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
