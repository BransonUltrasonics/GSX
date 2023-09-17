/****************************************************************************
** Meta object code from reading C++ file 'sysconfigdata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/sysconfigdata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sysconfigdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DateAndTime_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DateAndTime_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DateAndTime_t qt_meta_stringdata_DateAndTime = {
    {
QT_MOC_LITERAL(0, 0, 11) // "DateAndTime"

    },
    "DateAndTime"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DateAndTime[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void DateAndTime::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject DateAndTime::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_DateAndTime.data,
      qt_meta_data_DateAndTime,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DateAndTime::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DateAndTime::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DateAndTime.stringdata0))
        return static_cast<void*>(const_cast< DateAndTime*>(this));
    return QThread::qt_metacast(_clname);
}

int DateAndTime::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_SysConfigData_t {
    QByteArrayData data[107];
    char stringdata0[1851];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SysConfigData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SysConfigData_t qt_meta_stringdata_SysConfigData = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SysConfigData"
QT_MOC_LITERAL(1, 14, 15), // "LanguageChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 18), // "DateAndTimeChanged"
QT_MOC_LITERAL(4, 50, 17), // "responseResChaned"
QT_MOC_LITERAL(5, 68, 15), // "vacuumResChaned"
QT_MOC_LITERAL(6, 84, 15), // "dbStatusChanged"
QT_MOC_LITERAL(7, 100, 27), // "bransonKeyGlobalFlagChanged"
QT_MOC_LITERAL(8, 128, 29), // "bransonSecondLevelFlagChanged"
QT_MOC_LITERAL(9, 158, 22), // "globalCheckFlagChanged"
QT_MOC_LITERAL(10, 181, 16), // "emmcUsageChanged"
QT_MOC_LITERAL(11, 198, 14), // "setMachineName"
QT_MOC_LITERAL(12, 213, 13), // "a_MachineName"
QT_MOC_LITERAL(13, 227, 11), // "setLanguage"
QT_MOC_LITERAL(14, 239, 10), // "a_language"
QT_MOC_LITERAL(15, 250, 16), // "setSysConfigData"
QT_MOC_LITERAL(16, 267, 22), // "setPartContactDistance"
QT_MOC_LITERAL(17, 290, 17), // "a_PartContactDist"
QT_MOC_LITERAL(18, 308, 17), // "saveSysConfigData"
QT_MOC_LITERAL(19, 326, 12), // "sendSetupReq"
QT_MOC_LITERAL(20, 339, 12), // "recvSetupRes"
QT_MOC_LITERAL(21, 352, 16), // "setWaitingStatus"
QT_MOC_LITERAL(22, 369, 13), // "setSetupModel"
QT_MOC_LITERAL(23, 383, 29), // "setPresetBarcode_RecallRecipe"
QT_MOC_LITERAL(24, 413, 26), // "cPresetBarcodeRecallRecipe"
QT_MOC_LITERAL(25, 440, 29), // "getPresetBarcode_RecallRecipe"
QT_MOC_LITERAL(26, 470, 21), // "setPartIdScanRequired"
QT_MOC_LITERAL(27, 492, 19), // "bPartIdScanRequired"
QT_MOC_LITERAL(28, 512, 21), // "getPartIdScanRequired"
QT_MOC_LITERAL(29, 534, 20), // "getTimeAndDateFromSC"
QT_MOC_LITERAL(30, 555, 14), // "setDateAndTime"
QT_MOC_LITERAL(31, 570, 13), // "a_dateAndTime"
QT_MOC_LITERAL(32, 584, 14), // "getDateAndTime"
QT_MOC_LITERAL(33, 599, 26), // "convertDateAndTimeToString"
QT_MOC_LITERAL(34, 626, 19), // "sendDateAndTimeToSC"
QT_MOC_LITERAL(35, 646, 8), // "dateTime"
QT_MOC_LITERAL(36, 655, 22), // "generateReportResPopup"
QT_MOC_LITERAL(37, 678, 3), // "res"
QT_MOC_LITERAL(38, 682, 20), // "vacuumReportResPopup"
QT_MOC_LITERAL(39, 703, 19), // "dBMemoryStatusPopup"
QT_MOC_LITERAL(40, 723, 14), // "copyDBToUSBRes"
QT_MOC_LITERAL(41, 738, 14), // "generateReport"
QT_MOC_LITERAL(42, 753, 5), // "index"
QT_MOC_LITERAL(43, 759, 8), // "subIndex"
QT_MOC_LITERAL(44, 768, 12), // "outputformat"
QT_MOC_LITERAL(45, 781, 8), // "deleteID"
QT_MOC_LITERAL(46, 790, 16), // "sendReportAction"
QT_MOC_LITERAL(47, 807, 6), // "status"
QT_MOC_LITERAL(48, 814, 11), // "startVacuum"
QT_MOC_LITERAL(49, 826, 27), // "getSystemConfigGeneralParam"
QT_MOC_LITERAL(50, 854, 34), // "createSystemConfigGeneralPara..."
QT_MOC_LITERAL(51, 889, 29), // "createBransonSystemConfigList"
QT_MOC_LITERAL(52, 919, 17), // "getStatusOfSwitch"
QT_MOC_LITERAL(53, 937, 17), // "getNameOfTextEdit"
QT_MOC_LITERAL(54, 955, 24), // "getCurrentNameOfComboBox"
QT_MOC_LITERAL(55, 980, 26), // "initializeComboxModelsName"
QT_MOC_LITERAL(56, 1007, 23), // "updateToggleSwitchState"
QT_MOC_LITERAL(57, 1031, 9), // "paramText"
QT_MOC_LITERAL(58, 1041, 11), // "toogleState"
QT_MOC_LITERAL(59, 1053, 19), // "updateComboBoxState"
QT_MOC_LITERAL(60, 1073, 12), // "currentState"
QT_MOC_LITERAL(61, 1086, 19), // "updateLineEditState"
QT_MOC_LITERAL(62, 1106, 11), // "currentText"
QT_MOC_LITERAL(63, 1118, 17), // "compareBransonKey"
QT_MOC_LITERAL(64, 1136, 8), // "keyValue"
QT_MOC_LITERAL(65, 1145, 28), // "compareBransonSecondLevelKey"
QT_MOC_LITERAL(66, 1174, 17), // "ReadEMMCWareUsage"
QT_MOC_LITERAL(67, 1192, 12), // "getEMMCUsage"
QT_MOC_LITERAL(68, 1205, 11), // "getLanguage"
QT_MOC_LITERAL(69, 1217, 15), // "getLanguageCode"
QT_MOC_LITERAL(70, 1233, 14), // "getMachineName"
QT_MOC_LITERAL(71, 1248, 17), // "getAuthorityCheck"
QT_MOC_LITERAL(72, 1266, 17), // "setAuthorityCheck"
QT_MOC_LITERAL(73, 1284, 18), // "incrementTimeBySec"
QT_MOC_LITERAL(74, 1303, 8), // "deleteDB"
QT_MOC_LITERAL(75, 1312, 6), // "copyDB"
QT_MOC_LITERAL(76, 1319, 4), // "Init"
QT_MOC_LITERAL(77, 1324, 12), // "getReportRes"
QT_MOC_LITERAL(78, 1337, 12), // "getVacuumRes"
QT_MOC_LITERAL(79, 1350, 23), // "getBransonKeyGlobalFlag"
QT_MOC_LITERAL(80, 1374, 18), // "getGlobalCheckFlag"
QT_MOC_LITERAL(81, 1393, 17), // "getDbmemoryStatus"
QT_MOC_LITERAL(82, 1411, 15), // "getQTLanguageId"
QT_MOC_LITERAL(83, 1427, 12), // "currentIndex"
QT_MOC_LITERAL(84, 1440, 17), // "getQTLanguageName"
QT_MOC_LITERAL(85, 1458, 15), // "lRealLanguageId"
QT_MOC_LITERAL(86, 1474, 23), // "getQTNativeLanguageName"
QT_MOC_LITERAL(87, 1498, 16), // "sendfactoryQuery"
QT_MOC_LITERAL(88, 1515, 22), // "createServiceNotesList"
QT_MOC_LITERAL(89, 1538, 15), // "servicenoteText"
QT_MOC_LITERAL(90, 1554, 26), // "updateServiceNotesFromFile"
QT_MOC_LITERAL(91, 1581, 23), // "createServiceRightModel"
QT_MOC_LITERAL(92, 1605, 22), // "setServiceSwitchStatus"
QT_MOC_LITERAL(93, 1628, 22), // "getServiceSwitchStatus"
QT_MOC_LITERAL(94, 1651, 9), // "paramName"
QT_MOC_LITERAL(95, 1661, 24), // "resetServiceSwitchStatus"
QT_MOC_LITERAL(96, 1686, 15), // "saveServiceData"
QT_MOC_LITERAL(97, 1702, 15), // "stopTimerThread"
QT_MOC_LITERAL(98, 1718, 16), // "startTimerThread"
QT_MOC_LITERAL(99, 1735, 12), // "getCopyDBRes"
QT_MOC_LITERAL(100, 1748, 12), // "TitleBarDate"
QT_MOC_LITERAL(101, 1761, 9), // "reportRes"
QT_MOC_LITERAL(102, 1771, 20), // "bransonKeyGlobalFlag"
QT_MOC_LITERAL(103, 1792, 22), // "bransonSecondLevelFlag"
QT_MOC_LITERAL(104, 1815, 15), // "globalCheckFlag"
QT_MOC_LITERAL(105, 1831, 9), // "emmcUsage"
QT_MOC_LITERAL(106, 1841, 9) // "vacuumRes"

    },
    "SysConfigData\0LanguageChanged\0\0"
    "DateAndTimeChanged\0responseResChaned\0"
    "vacuumResChaned\0dbStatusChanged\0"
    "bransonKeyGlobalFlagChanged\0"
    "bransonSecondLevelFlagChanged\0"
    "globalCheckFlagChanged\0emmcUsageChanged\0"
    "setMachineName\0a_MachineName\0setLanguage\0"
    "a_language\0setSysConfigData\0"
    "setPartContactDistance\0a_PartContactDist\0"
    "saveSysConfigData\0sendSetupReq\0"
    "recvSetupRes\0setWaitingStatus\0"
    "setSetupModel\0setPresetBarcode_RecallRecipe\0"
    "cPresetBarcodeRecallRecipe\0"
    "getPresetBarcode_RecallRecipe\0"
    "setPartIdScanRequired\0bPartIdScanRequired\0"
    "getPartIdScanRequired\0getTimeAndDateFromSC\0"
    "setDateAndTime\0a_dateAndTime\0"
    "getDateAndTime\0convertDateAndTimeToString\0"
    "sendDateAndTimeToSC\0dateTime\0"
    "generateReportResPopup\0res\0"
    "vacuumReportResPopup\0dBMemoryStatusPopup\0"
    "copyDBToUSBRes\0generateReport\0index\0"
    "subIndex\0outputformat\0deleteID\0"
    "sendReportAction\0status\0startVacuum\0"
    "getSystemConfigGeneralParam\0"
    "createSystemConfigGeneralParamList\0"
    "createBransonSystemConfigList\0"
    "getStatusOfSwitch\0getNameOfTextEdit\0"
    "getCurrentNameOfComboBox\0"
    "initializeComboxModelsName\0"
    "updateToggleSwitchState\0paramText\0"
    "toogleState\0updateComboBoxState\0"
    "currentState\0updateLineEditState\0"
    "currentText\0compareBransonKey\0keyValue\0"
    "compareBransonSecondLevelKey\0"
    "ReadEMMCWareUsage\0getEMMCUsage\0"
    "getLanguage\0getLanguageCode\0getMachineName\0"
    "getAuthorityCheck\0setAuthorityCheck\0"
    "incrementTimeBySec\0deleteDB\0copyDB\0"
    "Init\0getReportRes\0getVacuumRes\0"
    "getBransonKeyGlobalFlag\0getGlobalCheckFlag\0"
    "getDbmemoryStatus\0getQTLanguageId\0"
    "currentIndex\0getQTLanguageName\0"
    "lRealLanguageId\0getQTNativeLanguageName\0"
    "sendfactoryQuery\0createServiceNotesList\0"
    "servicenoteText\0updateServiceNotesFromFile\0"
    "createServiceRightModel\0setServiceSwitchStatus\0"
    "getServiceSwitchStatus\0paramName\0"
    "resetServiceSwitchStatus\0saveServiceData\0"
    "stopTimerThread\0startTimerThread\0"
    "getCopyDBRes\0TitleBarDate\0reportRes\0"
    "bransonKeyGlobalFlag\0bransonSecondLevelFlag\0"
    "globalCheckFlag\0emmcUsage\0vacuumRes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SysConfigData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      76,   14, // methods
       7,  540, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  394,    2, 0x06 /* Public */,
       3,    0,  395,    2, 0x06 /* Public */,
       4,    0,  396,    2, 0x06 /* Public */,
       5,    0,  397,    2, 0x06 /* Public */,
       6,    0,  398,    2, 0x06 /* Public */,
       7,    0,  399,    2, 0x06 /* Public */,
       8,    0,  400,    2, 0x06 /* Public */,
       9,    0,  401,    2, 0x06 /* Public */,
      10,    0,  402,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,  403,    2, 0x0a /* Public */,
      13,    1,  406,    2, 0x0a /* Public */,
      15,    0,  409,    2, 0x0a /* Public */,
      16,    1,  410,    2, 0x0a /* Public */,
      18,    0,  413,    2, 0x0a /* Public */,
      19,    0,  414,    2, 0x0a /* Public */,
      20,    0,  415,    2, 0x0a /* Public */,
      21,    0,  416,    2, 0x0a /* Public */,
      22,    0,  417,    2, 0x0a /* Public */,
      23,    1,  418,    2, 0x0a /* Public */,
      25,    0,  421,    2, 0x0a /* Public */,
      26,    1,  422,    2, 0x0a /* Public */,
      28,    0,  425,    2, 0x0a /* Public */,
      29,    0,  426,    2, 0x0a /* Public */,
      30,    1,  427,    2, 0x0a /* Public */,
      32,    0,  430,    2, 0x0a /* Public */,
      33,    0,  431,    2, 0x0a /* Public */,
      34,    1,  432,    2, 0x0a /* Public */,
      36,    1,  435,    2, 0x0a /* Public */,
      38,    1,  438,    2, 0x0a /* Public */,
      39,    1,  441,    2, 0x0a /* Public */,
      40,    1,  444,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      41,    4,  447,    2, 0x02 /* Public */,
      46,    1,  456,    2, 0x02 /* Public */,
      48,    0,  459,    2, 0x02 /* Public */,
      49,    0,  460,    2, 0x02 /* Public */,
      50,    0,  461,    2, 0x02 /* Public */,
      51,    0,  462,    2, 0x02 /* Public */,
      52,    1,  463,    2, 0x02 /* Public */,
      53,    1,  466,    2, 0x02 /* Public */,
      54,    1,  469,    2, 0x02 /* Public */,
      55,    0,  472,    2, 0x02 /* Public */,
      56,    2,  473,    2, 0x02 /* Public */,
      59,    2,  478,    2, 0x02 /* Public */,
      61,    2,  483,    2, 0x02 /* Public */,
      63,    1,  488,    2, 0x02 /* Public */,
      65,    1,  491,    2, 0x02 /* Public */,
      66,    0,  494,    2, 0x02 /* Public */,
      67,    0,  495,    2, 0x02 /* Public */,
      68,    0,  496,    2, 0x02 /* Public */,
      69,    0,  497,    2, 0x02 /* Public */,
      70,    0,  498,    2, 0x02 /* Public */,
      71,    0,  499,    2, 0x02 /* Public */,
      72,    0,  500,    2, 0x02 /* Public */,
      73,    0,  501,    2, 0x02 /* Public */,
      74,    0,  502,    2, 0x02 /* Public */,
      75,    0,  503,    2, 0x02 /* Public */,
      76,    0,  504,    2, 0x02 /* Public */,
      77,    0,  505,    2, 0x02 /* Public */,
      78,    0,  506,    2, 0x02 /* Public */,
      79,    0,  507,    2, 0x02 /* Public */,
      80,    0,  508,    2, 0x02 /* Public */,
      81,    0,  509,    2, 0x02 /* Public */,
      82,    1,  510,    2, 0x02 /* Public */,
      84,    1,  513,    2, 0x02 /* Public */,
      86,    1,  516,    2, 0x02 /* Public */,
      87,    0,  519,    2, 0x02 /* Public */,
      88,    1,  520,    2, 0x02 /* Public */,
      90,    0,  523,    2, 0x02 /* Public */,
      91,    0,  524,    2, 0x02 /* Public */,
      92,    2,  525,    2, 0x02 /* Public */,
      93,    1,  530,    2, 0x02 /* Public */,
      95,    0,  533,    2, 0x02 /* Public */,
      96,    1,  534,    2, 0x02 /* Public */,
      97,    0,  537,    2, 0x02 /* Public */,
      98,    0,  538,    2, 0x02 /* Public */,
      99,    0,  539,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::QString,
    QMetaType::Void, QMetaType::Bool,   27,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDateTime,   31,
    QMetaType::QDateTime,
    QMetaType::QString,
    QMetaType::Bool, QMetaType::QString,   35,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void, QMetaType::QString,   37,

 // methods: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   42,   43,   44,   45,
    QMetaType::Void, QMetaType::QString,   47,
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,    2,
    QMetaType::QString, QMetaType::QString,    2,
    QMetaType::Int, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   57,   58,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   57,   60,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   57,   62,
    QMetaType::Int, QMetaType::QString,   64,
    QMetaType::Int, QMetaType::QString,   64,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Int,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::QString,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,   83,
    QMetaType::QString, QMetaType::Int,   85,
    QMetaType::QString, QMetaType::Int,   85,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::QString,   89,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   42,   47,
    QMetaType::Bool, QMetaType::QString,   94,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,

 // properties: name, type, flags
     100, QMetaType::QDateTime, 0x00495003,
     101, QMetaType::Int, 0x00495103,
     102, QMetaType::Bool, 0x00495103,
     103, QMetaType::Bool, 0x00495003,
     104, QMetaType::Bool, 0x00495103,
     105, QMetaType::QString, 0x00495003,
     106, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       1,
       2,
       5,
       6,
       7,
       8,
       3,

       0        // eod
};

void SysConfigData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SysConfigData *_t = static_cast<SysConfigData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->LanguageChanged(); break;
        case 1: _t->DateAndTimeChanged(); break;
        case 2: _t->responseResChaned(); break;
        case 3: _t->vacuumResChaned(); break;
        case 4: _t->dbStatusChanged(); break;
        case 5: _t->bransonKeyGlobalFlagChanged(); break;
        case 6: _t->bransonSecondLevelFlagChanged(); break;
        case 7: _t->globalCheckFlagChanged(); break;
        case 8: _t->emmcUsageChanged(); break;
        case 9: _t->setMachineName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->setSysConfigData(); break;
        case 12: _t->setPartContactDistance((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: { int _r = _t->saveSysConfigData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->sendSetupReq(); break;
        case 15: { QString _r = _t->recvSetupRes();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 16: { QString _r = _t->setWaitingStatus();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 17: _t->setSetupModel(); break;
        case 18: _t->setPresetBarcode_RecallRecipe((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: { QString _r = _t->getPresetBarcode_RecallRecipe();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->setPartIdScanRequired((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: { bool _r = _t->getPartIdScanRequired();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 22: _t->getTimeAndDateFromSC(); break;
        case 23: _t->setDateAndTime((*reinterpret_cast< QDateTime(*)>(_a[1]))); break;
        case 24: { QDateTime _r = _t->getDateAndTime();
            if (_a[0]) *reinterpret_cast< QDateTime*>(_a[0]) = std::move(_r); }  break;
        case 25: { QString _r = _t->convertDateAndTimeToString();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 26: { bool _r = _t->sendDateAndTimeToSC((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 27: _t->generateReportResPopup((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->vacuumReportResPopup((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 29: _t->dBMemoryStatusPopup((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 30: _t->copyDBToUSBRes((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 31: _t->generateReport((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 32: _t->sendReportAction((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 33: _t->startVacuum(); break;
        case 34: { QString _r = _t->getSystemConfigGeneralParam();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 35: _t->createSystemConfigGeneralParamList(); break;
        case 36: _t->createBransonSystemConfigList(); break;
        case 37: { bool _r = _t->getStatusOfSwitch((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 38: { QString _r = _t->getNameOfTextEdit((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 39: { int _r = _t->getCurrentNameOfComboBox((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 40: _t->initializeComboxModelsName(); break;
        case 41: _t->updateToggleSwitchState((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 42: _t->updateComboBoxState((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 43: _t->updateLineEditState((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 44: { int _r = _t->compareBransonKey((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 45: { int _r = _t->compareBransonSecondLevelKey((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 46: { QString _r = _t->ReadEMMCWareUsage();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 47: { QString _r = _t->getEMMCUsage();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 48: { int _r = _t->getLanguage();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 49: { QString _r = _t->getLanguageCode();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 50: { QString _r = _t->getMachineName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 51: { int _r = _t->getAuthorityCheck();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 52: _t->setAuthorityCheck(); break;
        case 53: _t->incrementTimeBySec(); break;
        case 54: { bool _r = _t->deleteDB();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 55: _t->copyDB(); break;
        case 56: _t->Init(); break;
        case 57: { int _r = _t->getReportRes();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 58: { QString _r = _t->getVacuumRes();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 59: { bool _r = _t->getBransonKeyGlobalFlag();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 60: { bool _r = _t->getGlobalCheckFlag();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 61: { int _r = _t->getDbmemoryStatus();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 62: { int _r = _t->getQTLanguageId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 63: { QString _r = _t->getQTLanguageName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 64: { QString _r = _t->getQTNativeLanguageName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 65: { bool _r = _t->sendfactoryQuery();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 66: _t->createServiceNotesList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 67: _t->updateServiceNotesFromFile(); break;
        case 68: _t->createServiceRightModel(); break;
        case 69: _t->setServiceSwitchStatus((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 70: { bool _r = _t->getServiceSwitchStatus((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 71: _t->resetServiceSwitchStatus(); break;
        case 72: _t->saveServiceData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 73: _t->stopTimerThread(); break;
        case 74: _t->startTimerThread(); break;
        case 75: { int _r = _t->getCopyDBRes();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SysConfigData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SysConfigData::LanguageChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SysConfigData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SysConfigData::DateAndTimeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SysConfigData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SysConfigData::responseResChaned)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SysConfigData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SysConfigData::vacuumResChaned)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SysConfigData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SysConfigData::dbStatusChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (SysConfigData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SysConfigData::bransonKeyGlobalFlagChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (SysConfigData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SysConfigData::bransonSecondLevelFlagChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (SysConfigData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SysConfigData::globalCheckFlagChanged)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (SysConfigData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SysConfigData::emmcUsageChanged)) {
                *result = 8;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        SysConfigData *_t = static_cast<SysConfigData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QDateTime*>(_v) = _t->getDateAndTime(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->getReportRes(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->getBransonKeyGlobalFlag(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->getBransonSecondLevelKeyFlag(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->getGlobalCheckFlag(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->getEMMCUsage(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->getVacuumRes(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        SysConfigData *_t = static_cast<SysConfigData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setDateAndTime(*reinterpret_cast< QDateTime*>(_v)); break;
        case 1: _t->setReportRes(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setBransonKeyGlobalFlag(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setBransonSecondLevelKeyFlag(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setGlobalCheckFlag(*reinterpret_cast< bool*>(_v)); break;
        case 5: _t->updateEMMCUsage(*reinterpret_cast< QString*>(_v)); break;
        case 6: _t->setVacuumRes(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject SysConfigData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SysConfigData.data,
      qt_meta_data_SysConfigData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SysConfigData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SysConfigData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SysConfigData.stringdata0))
        return static_cast<void*>(const_cast< SysConfigData*>(this));
    return QObject::qt_metacast(_clname);
}

int SysConfigData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 76)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 76;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 76)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 76;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SysConfigData::LanguageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SysConfigData::DateAndTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SysConfigData::responseResChaned()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SysConfigData::vacuumResChaned()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SysConfigData::dbStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void SysConfigData::bransonKeyGlobalFlagChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void SysConfigData::bransonSecondLevelFlagChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void SysConfigData::globalCheckFlagChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void SysConfigData::emmcUsageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
