/****************************************************************************
** Meta object code from reading C++ file 'trendsgraph.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Header/trendsgraph.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trendsgraph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TrendsGraph_t {
    QByteArrayData data[115];
    char stringdata0[2764];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TrendsGraph_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TrendsGraph_t qt_meta_stringdata_TrendsGraph = {
    {
QT_MOC_LITERAL(0, 0, 11), // "TrendsGraph"
QT_MOC_LITERAL(1, 12, 13), // "valueXChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 17), // "tickCountYChanged"
QT_MOC_LITERAL(4, 45, 23), // "suspectLimitHighChanged"
QT_MOC_LITERAL(5, 69, 22), // "rejectLimitHighChanged"
QT_MOC_LITERAL(6, 92, 21), // "rejectLimitLowChanged"
QT_MOC_LITERAL(7, 114, 22), // "suspectLimitLowChanged"
QT_MOC_LITERAL(8, 137, 27), // "timeSuspectLimitHighChanged"
QT_MOC_LITERAL(9, 165, 26), // "timeRejectLimitHighChanged"
QT_MOC_LITERAL(10, 192, 25), // "timeRejectLimitLowChanged"
QT_MOC_LITERAL(11, 218, 26), // "timeSuspectLimitLowChanged"
QT_MOC_LITERAL(12, 245, 29), // "energySuspectLimitHighChanged"
QT_MOC_LITERAL(13, 275, 28), // "energyRejectLimitHighChanged"
QT_MOC_LITERAL(14, 304, 27), // "energyRejectLimitLowChanged"
QT_MOC_LITERAL(15, 332, 28), // "energySuspectLimitLowChanged"
QT_MOC_LITERAL(16, 361, 32), // "peakPowerSuspectLimitHighChanged"
QT_MOC_LITERAL(17, 394, 31), // "peakPowerRejectLimitHighChanged"
QT_MOC_LITERAL(18, 426, 30), // "peakPowerRejectLimitLowChanged"
QT_MOC_LITERAL(19, 457, 31), // "peakPowerSuspectLimitLowChanged"
QT_MOC_LITERAL(20, 489, 35), // "absoluteDistSuspectLimitHighC..."
QT_MOC_LITERAL(21, 525, 34), // "absoluteDistRejectLimitHighCh..."
QT_MOC_LITERAL(22, 560, 33), // "absoluteDistRejectLimitLowCha..."
QT_MOC_LITERAL(23, 594, 34), // "absoluteDistSuspectLimitLowCh..."
QT_MOC_LITERAL(24, 629, 32), // "frequencySuspectLimitHighChanged"
QT_MOC_LITERAL(25, 662, 31), // "frequencyRejectLimitHighChanged"
QT_MOC_LITERAL(26, 694, 30), // "frequencyRejectLimitLowChanged"
QT_MOC_LITERAL(27, 725, 31), // "frequencySuspectLimitLowChanged"
QT_MOC_LITERAL(28, 757, 35), // "maxWeldForceSuspectLimitHighC..."
QT_MOC_LITERAL(29, 793, 34), // "maxWeldForceRejectLimitHighCh..."
QT_MOC_LITERAL(30, 828, 33), // "maxWeldForceRejectLimitLowCha..."
QT_MOC_LITERAL(31, 862, 34), // "maxWeldForceSuspectLimitLowCh..."
QT_MOC_LITERAL(32, 897, 25), // "minWeldForceYValueChanged"
QT_MOC_LITERAL(33, 923, 25), // "maxWeldForceYValueChanged"
QT_MOC_LITERAL(34, 949, 25), // "minHoldForceYValueChanged"
QT_MOC_LITERAL(35, 975, 25), // "maxHoldForceYValueChanged"
QT_MOC_LITERAL(36, 1001, 29), // "minTotalAbsoluteYValueChanged"
QT_MOC_LITERAL(37, 1031, 29), // "maxTotalAbsoluteYValueChanged"
QT_MOC_LITERAL(38, 1061, 30), // "minPartContactPosYValueChanged"
QT_MOC_LITERAL(39, 1092, 30), // "maxPartContactPosYValueChanged"
QT_MOC_LITERAL(40, 1123, 24), // "minWeldTimeYValueChanged"
QT_MOC_LITERAL(41, 1148, 24), // "maxWeldTimeYValueChanged"
QT_MOC_LITERAL(42, 1173, 26), // "minWeldEnergyYValueChanged"
QT_MOC_LITERAL(43, 1200, 26), // "maxWeldEnergyYValueChanged"
QT_MOC_LITERAL(44, 1227, 29), // "minWeldPeakPowerYValueChanged"
QT_MOC_LITERAL(45, 1257, 29), // "maxWeldPeakPowerYValueChanged"
QT_MOC_LITERAL(46, 1287, 30), // "minStartFrequencyYValueChanged"
QT_MOC_LITERAL(47, 1318, 30), // "maxStartFrequencyYValueChanged"
QT_MOC_LITERAL(48, 1349, 31), // "minFrequencyChangeYValueChanged"
QT_MOC_LITERAL(49, 1381, 31), // "maxFrequencyChangeYValueChanged"
QT_MOC_LITERAL(50, 1413, 20), // "getGraphValueByIndex"
QT_MOC_LITERAL(51, 1434, 6), // "gIndex"
QT_MOC_LITERAL(52, 1441, 9), // "typeIndex"
QT_MOC_LITERAL(53, 1451, 15), // "trendsGraphData"
QT_MOC_LITERAL(54, 1467, 12), // "recipeNumber"
QT_MOC_LITERAL(55, 1480, 27), // "getTrendsGraphSignatureData"
QT_MOC_LITERAL(56, 1508, 11), // "a_historyId"
QT_MOC_LITERAL(57, 1520, 17), // "getYAxisMaxValues"
QT_MOC_LITERAL(58, 1538, 17), // "getYAxisMinValues"
QT_MOC_LITERAL(59, 1556, 22), // "getGraphParametersName"
QT_MOC_LITERAL(60, 1579, 20), // "getCycleConterValues"
QT_MOC_LITERAL(61, 1600, 27), // "getSuspectRejectParamLimits"
QT_MOC_LITERAL(62, 1628, 9), // "getXCount"
QT_MOC_LITERAL(63, 1638, 12), // "getAlarmFlag"
QT_MOC_LITERAL(64, 1651, 5), // "index"
QT_MOC_LITERAL(65, 1657, 16), // "getTopScaleModel"
QT_MOC_LITERAL(66, 1674, 20), // "getBrushColorForAxis"
QT_MOC_LITERAL(67, 1695, 11), // "countValueX"
QT_MOC_LITERAL(68, 1707, 10), // "tickCountY"
QT_MOC_LITERAL(69, 1718, 20), // "timeSuspectLimitHigh"
QT_MOC_LITERAL(70, 1739, 19), // "timeRejectLimitHigh"
QT_MOC_LITERAL(71, 1759, 18), // "timeRejectLimitLow"
QT_MOC_LITERAL(72, 1778, 19), // "timeSuspectLimitLow"
QT_MOC_LITERAL(73, 1798, 22), // "energySuspectLimitHigh"
QT_MOC_LITERAL(74, 1821, 21), // "energyRejectLimitHigh"
QT_MOC_LITERAL(75, 1843, 20), // "energyRejectLimitLow"
QT_MOC_LITERAL(76, 1864, 21), // "energySuspectLimitLow"
QT_MOC_LITERAL(77, 1886, 25), // "peakPowerSuspectLimitHigh"
QT_MOC_LITERAL(78, 1912, 24), // "peakPowerRejectLimitHigh"
QT_MOC_LITERAL(79, 1937, 23), // "peakPowerRejectLimitLow"
QT_MOC_LITERAL(80, 1961, 24), // "peakPowerSuspectLimitLow"
QT_MOC_LITERAL(81, 1986, 28), // "absoluteDistSuspectLimitHigh"
QT_MOC_LITERAL(82, 2015, 27), // "absoluteDistRejectLimitHigh"
QT_MOC_LITERAL(83, 2043, 26), // "absoluteDistRejectLimitLow"
QT_MOC_LITERAL(84, 2070, 27), // "absoluteDistSuspectLimitLow"
QT_MOC_LITERAL(85, 2098, 25), // "frequencySuspectLimitHigh"
QT_MOC_LITERAL(86, 2124, 24), // "frequencyRejectLimitHigh"
QT_MOC_LITERAL(87, 2149, 23), // "frequencyRejectLimitLow"
QT_MOC_LITERAL(88, 2173, 24), // "frequencySuspectLimitLow"
QT_MOC_LITERAL(89, 2198, 28), // "maxWeldForceSuspectLimitHigh"
QT_MOC_LITERAL(90, 2227, 27), // "maxWeldForceRejectLimitHigh"
QT_MOC_LITERAL(91, 2255, 26), // "maxWeldForceRejectLimitLow"
QT_MOC_LITERAL(92, 2282, 27), // "maxWeldForceSuspectLimitLow"
QT_MOC_LITERAL(93, 2310, 16), // "suspectLimitHigh"
QT_MOC_LITERAL(94, 2327, 15), // "rejectLimitHigh"
QT_MOC_LITERAL(95, 2343, 14), // "rejectLimitLow"
QT_MOC_LITERAL(96, 2358, 15), // "suspectLimitLow"
QT_MOC_LITERAL(97, 2374, 18), // "minWeldForceYValue"
QT_MOC_LITERAL(98, 2393, 18), // "maxWeldForceYValue"
QT_MOC_LITERAL(99, 2412, 18), // "minHoldForceYValue"
QT_MOC_LITERAL(100, 2431, 18), // "maxHoldForceYValue"
QT_MOC_LITERAL(101, 2450, 22), // "minTotalAbsoluteYValue"
QT_MOC_LITERAL(102, 2473, 22), // "maxTotalAbsoluteYValue"
QT_MOC_LITERAL(103, 2496, 23), // "minPartContactPosYValue"
QT_MOC_LITERAL(104, 2520, 23), // "maxPartContactPosYValue"
QT_MOC_LITERAL(105, 2544, 17), // "minWeldTimeYValue"
QT_MOC_LITERAL(106, 2562, 17), // "maxWeldTimeYValue"
QT_MOC_LITERAL(107, 2580, 19), // "minWeldEnergyYValue"
QT_MOC_LITERAL(108, 2600, 19), // "maxWeldEnergyYValue"
QT_MOC_LITERAL(109, 2620, 22), // "minWeldPeakPowerYValue"
QT_MOC_LITERAL(110, 2643, 22), // "maxWeldPeakPowerYValue"
QT_MOC_LITERAL(111, 2666, 23), // "minStartFrequencyYValue"
QT_MOC_LITERAL(112, 2690, 23), // "maxStartFrequencyYValue"
QT_MOC_LITERAL(113, 2714, 24), // "minFrequencyChangeYValue"
QT_MOC_LITERAL(114, 2739, 24) // "maxFrequencyChangeYValue"

    },
    "TrendsGraph\0valueXChanged\0\0tickCountYChanged\0"
    "suspectLimitHighChanged\0rejectLimitHighChanged\0"
    "rejectLimitLowChanged\0suspectLimitLowChanged\0"
    "timeSuspectLimitHighChanged\0"
    "timeRejectLimitHighChanged\0"
    "timeRejectLimitLowChanged\0"
    "timeSuspectLimitLowChanged\0"
    "energySuspectLimitHighChanged\0"
    "energyRejectLimitHighChanged\0"
    "energyRejectLimitLowChanged\0"
    "energySuspectLimitLowChanged\0"
    "peakPowerSuspectLimitHighChanged\0"
    "peakPowerRejectLimitHighChanged\0"
    "peakPowerRejectLimitLowChanged\0"
    "peakPowerSuspectLimitLowChanged\0"
    "absoluteDistSuspectLimitHighChanged\0"
    "absoluteDistRejectLimitHighChanged\0"
    "absoluteDistRejectLimitLowChanged\0"
    "absoluteDistSuspectLimitLowChanged\0"
    "frequencySuspectLimitHighChanged\0"
    "frequencyRejectLimitHighChanged\0"
    "frequencyRejectLimitLowChanged\0"
    "frequencySuspectLimitLowChanged\0"
    "maxWeldForceSuspectLimitHighChanged\0"
    "maxWeldForceRejectLimitHighChanged\0"
    "maxWeldForceRejectLimitLowChanged\0"
    "maxWeldForceSuspectLimitLowChanged\0"
    "minWeldForceYValueChanged\0"
    "maxWeldForceYValueChanged\0"
    "minHoldForceYValueChanged\0"
    "maxHoldForceYValueChanged\0"
    "minTotalAbsoluteYValueChanged\0"
    "maxTotalAbsoluteYValueChanged\0"
    "minPartContactPosYValueChanged\0"
    "maxPartContactPosYValueChanged\0"
    "minWeldTimeYValueChanged\0"
    "maxWeldTimeYValueChanged\0"
    "minWeldEnergyYValueChanged\0"
    "maxWeldEnergyYValueChanged\0"
    "minWeldPeakPowerYValueChanged\0"
    "maxWeldPeakPowerYValueChanged\0"
    "minStartFrequencyYValueChanged\0"
    "maxStartFrequencyYValueChanged\0"
    "minFrequencyChangeYValueChanged\0"
    "maxFrequencyChangeYValueChanged\0"
    "getGraphValueByIndex\0gIndex\0typeIndex\0"
    "trendsGraphData\0recipeNumber\0"
    "getTrendsGraphSignatureData\0a_historyId\0"
    "getYAxisMaxValues\0getYAxisMinValues\0"
    "getGraphParametersName\0getCycleConterValues\0"
    "getSuspectRejectParamLimits\0getXCount\0"
    "getAlarmFlag\0index\0getTopScaleModel\0"
    "getBrushColorForAxis\0countValueX\0"
    "tickCountY\0timeSuspectLimitHigh\0"
    "timeRejectLimitHigh\0timeRejectLimitLow\0"
    "timeSuspectLimitLow\0energySuspectLimitHigh\0"
    "energyRejectLimitHigh\0energyRejectLimitLow\0"
    "energySuspectLimitLow\0peakPowerSuspectLimitHigh\0"
    "peakPowerRejectLimitHigh\0"
    "peakPowerRejectLimitLow\0"
    "peakPowerSuspectLimitLow\0"
    "absoluteDistSuspectLimitHigh\0"
    "absoluteDistRejectLimitHigh\0"
    "absoluteDistRejectLimitLow\0"
    "absoluteDistSuspectLimitLow\0"
    "frequencySuspectLimitHigh\0"
    "frequencyRejectLimitHigh\0"
    "frequencyRejectLimitLow\0"
    "frequencySuspectLimitLow\0"
    "maxWeldForceSuspectLimitHigh\0"
    "maxWeldForceRejectLimitHigh\0"
    "maxWeldForceRejectLimitLow\0"
    "maxWeldForceSuspectLimitLow\0"
    "suspectLimitHigh\0rejectLimitHigh\0"
    "rejectLimitLow\0suspectLimitLow\0"
    "minWeldForceYValue\0maxWeldForceYValue\0"
    "minHoldForceYValue\0maxHoldForceYValue\0"
    "minTotalAbsoluteYValue\0maxTotalAbsoluteYValue\0"
    "minPartContactPosYValue\0maxPartContactPosYValue\0"
    "minWeldTimeYValue\0maxWeldTimeYValue\0"
    "minWeldEnergyYValue\0maxWeldEnergyYValue\0"
    "minWeldPeakPowerYValue\0maxWeldPeakPowerYValue\0"
    "minStartFrequencyYValue\0maxStartFrequencyYValue\0"
    "minFrequencyChangeYValue\0"
    "maxFrequencyChangeYValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TrendsGraph[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      60,   14, // methods
      48,  390, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      48,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  314,    2, 0x06 /* Public */,
       3,    0,  315,    2, 0x06 /* Public */,
       4,    0,  316,    2, 0x06 /* Public */,
       5,    0,  317,    2, 0x06 /* Public */,
       6,    0,  318,    2, 0x06 /* Public */,
       7,    0,  319,    2, 0x06 /* Public */,
       8,    0,  320,    2, 0x06 /* Public */,
       9,    0,  321,    2, 0x06 /* Public */,
      10,    0,  322,    2, 0x06 /* Public */,
      11,    0,  323,    2, 0x06 /* Public */,
      12,    0,  324,    2, 0x06 /* Public */,
      13,    0,  325,    2, 0x06 /* Public */,
      14,    0,  326,    2, 0x06 /* Public */,
      15,    0,  327,    2, 0x06 /* Public */,
      16,    0,  328,    2, 0x06 /* Public */,
      17,    0,  329,    2, 0x06 /* Public */,
      18,    0,  330,    2, 0x06 /* Public */,
      19,    0,  331,    2, 0x06 /* Public */,
      20,    0,  332,    2, 0x06 /* Public */,
      21,    0,  333,    2, 0x06 /* Public */,
      22,    0,  334,    2, 0x06 /* Public */,
      23,    0,  335,    2, 0x06 /* Public */,
      24,    0,  336,    2, 0x06 /* Public */,
      25,    0,  337,    2, 0x06 /* Public */,
      26,    0,  338,    2, 0x06 /* Public */,
      27,    0,  339,    2, 0x06 /* Public */,
      28,    0,  340,    2, 0x06 /* Public */,
      29,    0,  341,    2, 0x06 /* Public */,
      30,    0,  342,    2, 0x06 /* Public */,
      31,    0,  343,    2, 0x06 /* Public */,
      32,    0,  344,    2, 0x06 /* Public */,
      33,    0,  345,    2, 0x06 /* Public */,
      34,    0,  346,    2, 0x06 /* Public */,
      35,    0,  347,    2, 0x06 /* Public */,
      36,    0,  348,    2, 0x06 /* Public */,
      37,    0,  349,    2, 0x06 /* Public */,
      38,    0,  350,    2, 0x06 /* Public */,
      39,    0,  351,    2, 0x06 /* Public */,
      40,    0,  352,    2, 0x06 /* Public */,
      41,    0,  353,    2, 0x06 /* Public */,
      42,    0,  354,    2, 0x06 /* Public */,
      43,    0,  355,    2, 0x06 /* Public */,
      44,    0,  356,    2, 0x06 /* Public */,
      45,    0,  357,    2, 0x06 /* Public */,
      46,    0,  358,    2, 0x06 /* Public */,
      47,    0,  359,    2, 0x06 /* Public */,
      48,    0,  360,    2, 0x06 /* Public */,
      49,    0,  361,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      50,    2,  362,    2, 0x02 /* Public */,
      53,    1,  367,    2, 0x02 /* Public */,
      55,    1,  370,    2, 0x02 /* Public */,
      57,    1,  373,    2, 0x02 /* Public */,
      58,    1,  376,    2, 0x02 /* Public */,
      59,    0,  379,    2, 0x02 /* Public */,
      60,    0,  380,    2, 0x02 /* Public */,
      61,    1,  381,    2, 0x02 /* Public */,
      62,    0,  384,    2, 0x02 /* Public */,
      63,    1,  385,    2, 0x02 /* Public */,
      65,    0,  388,    2, 0x02 /* Public */,
      66,    0,  389,    2, 0x02 /* Public */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::QPointF, QMetaType::Int, QMetaType::Int,   51,   52,
    QMetaType::Void, QMetaType::QString,   54,
    QMetaType::Int, QMetaType::QString,   56,
    QMetaType::Int, QMetaType::Int,    2,
    QMetaType::Int, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int, QMetaType::QString,    2,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,   64,
    QMetaType::Void,
    QMetaType::QString,

 // properties: name, type, flags
      67, QMetaType::Int, 0x00495003,
      68, QMetaType::Int, 0x00495103,
      69, QMetaType::Int, 0x00495103,
      70, QMetaType::Int, 0x00495103,
      71, QMetaType::Int, 0x00495103,
      72, QMetaType::Int, 0x00495103,
      73, QMetaType::Int, 0x00495103,
      74, QMetaType::Int, 0x00495103,
      75, QMetaType::Int, 0x00495103,
      76, QMetaType::Int, 0x00495103,
      77, QMetaType::Int, 0x00495103,
      78, QMetaType::Int, 0x00495103,
      79, QMetaType::Int, 0x00495103,
      80, QMetaType::Int, 0x00495103,
      81, QMetaType::Int, 0x00495103,
      82, QMetaType::Int, 0x00495103,
      83, QMetaType::Int, 0x00495103,
      84, QMetaType::Int, 0x00495103,
      85, QMetaType::Int, 0x00495103,
      86, QMetaType::Int, 0x00495103,
      87, QMetaType::Int, 0x00495103,
      88, QMetaType::Int, 0x00495103,
      89, QMetaType::Int, 0x00495103,
      90, QMetaType::Int, 0x00495103,
      91, QMetaType::Int, 0x00495103,
      92, QMetaType::Int, 0x00495103,
      93, QMetaType::Int, 0x00495103,
      94, QMetaType::Int, 0x00495103,
      95, QMetaType::Int, 0x00495103,
      96, QMetaType::Int, 0x00495103,
      97, QMetaType::Int, 0x00495103,
      98, QMetaType::Int, 0x00495103,
      99, QMetaType::Int, 0x00495103,
     100, QMetaType::Int, 0x00495103,
     101, QMetaType::Int, 0x00495103,
     102, QMetaType::Int, 0x00495103,
     103, QMetaType::Long, 0x00495103,
     104, QMetaType::Long, 0x00495103,
     105, QMetaType::Int, 0x00495103,
     106, QMetaType::Int, 0x00495103,
     107, QMetaType::Int, 0x00495103,
     108, QMetaType::Int, 0x00495103,
     109, QMetaType::Int, 0x00495103,
     110, QMetaType::Int, 0x00495103,
     111, QMetaType::Int, 0x00495103,
     112, QMetaType::Int, 0x00495103,
     113, QMetaType::Int, 0x00495103,
     114, QMetaType::Int, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       6,
       7,
       8,
       9,
      10,
      11,
      12,
      13,
      14,
      15,
      16,
      17,
      18,
      19,
      20,
      21,
      22,
      23,
      24,
      25,
      26,
      27,
      28,
      29,
       2,
       3,
       4,
       5,
      30,
      31,
      32,
      33,
      34,
      35,
      36,
      37,
      38,
      39,
      40,
      41,
      42,
      43,
      44,
      45,
      46,
      47,

       0        // eod
};

void TrendsGraph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TrendsGraph *_t = static_cast<TrendsGraph *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueXChanged(); break;
        case 1: _t->tickCountYChanged(); break;
        case 2: _t->suspectLimitHighChanged(); break;
        case 3: _t->rejectLimitHighChanged(); break;
        case 4: _t->rejectLimitLowChanged(); break;
        case 5: _t->suspectLimitLowChanged(); break;
        case 6: _t->timeSuspectLimitHighChanged(); break;
        case 7: _t->timeRejectLimitHighChanged(); break;
        case 8: _t->timeRejectLimitLowChanged(); break;
        case 9: _t->timeSuspectLimitLowChanged(); break;
        case 10: _t->energySuspectLimitHighChanged(); break;
        case 11: _t->energyRejectLimitHighChanged(); break;
        case 12: _t->energyRejectLimitLowChanged(); break;
        case 13: _t->energySuspectLimitLowChanged(); break;
        case 14: _t->peakPowerSuspectLimitHighChanged(); break;
        case 15: _t->peakPowerRejectLimitHighChanged(); break;
        case 16: _t->peakPowerRejectLimitLowChanged(); break;
        case 17: _t->peakPowerSuspectLimitLowChanged(); break;
        case 18: _t->absoluteDistSuspectLimitHighChanged(); break;
        case 19: _t->absoluteDistRejectLimitHighChanged(); break;
        case 20: _t->absoluteDistRejectLimitLowChanged(); break;
        case 21: _t->absoluteDistSuspectLimitLowChanged(); break;
        case 22: _t->frequencySuspectLimitHighChanged(); break;
        case 23: _t->frequencyRejectLimitHighChanged(); break;
        case 24: _t->frequencyRejectLimitLowChanged(); break;
        case 25: _t->frequencySuspectLimitLowChanged(); break;
        case 26: _t->maxWeldForceSuspectLimitHighChanged(); break;
        case 27: _t->maxWeldForceRejectLimitHighChanged(); break;
        case 28: _t->maxWeldForceRejectLimitLowChanged(); break;
        case 29: _t->maxWeldForceSuspectLimitLowChanged(); break;
        case 30: _t->minWeldForceYValueChanged(); break;
        case 31: _t->maxWeldForceYValueChanged(); break;
        case 32: _t->minHoldForceYValueChanged(); break;
        case 33: _t->maxHoldForceYValueChanged(); break;
        case 34: _t->minTotalAbsoluteYValueChanged(); break;
        case 35: _t->maxTotalAbsoluteYValueChanged(); break;
        case 36: _t->minPartContactPosYValueChanged(); break;
        case 37: _t->maxPartContactPosYValueChanged(); break;
        case 38: _t->minWeldTimeYValueChanged(); break;
        case 39: _t->maxWeldTimeYValueChanged(); break;
        case 40: _t->minWeldEnergyYValueChanged(); break;
        case 41: _t->maxWeldEnergyYValueChanged(); break;
        case 42: _t->minWeldPeakPowerYValueChanged(); break;
        case 43: _t->maxWeldPeakPowerYValueChanged(); break;
        case 44: _t->minStartFrequencyYValueChanged(); break;
        case 45: _t->maxStartFrequencyYValueChanged(); break;
        case 46: _t->minFrequencyChangeYValueChanged(); break;
        case 47: _t->maxFrequencyChangeYValueChanged(); break;
        case 48: { QPointF _r = _t->getGraphValueByIndex((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QPointF*>(_a[0]) = std::move(_r); }  break;
        case 49: _t->trendsGraphData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 50: { int _r = _t->getTrendsGraphSignatureData((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 51: { int _r = _t->getYAxisMaxValues((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 52: { int _r = _t->getYAxisMinValues((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 53: _t->getGraphParametersName(); break;
        case 54: _t->getCycleConterValues(); break;
        case 55: { int _r = _t->getSuspectRejectParamLimits((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 56: { int _r = _t->getXCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 57: { int _r = _t->getAlarmFlag((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 58: _t->getTopScaleModel(); break;
        case 59: { QString _r = _t->getBrushColorForAxis();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::valueXChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::tickCountYChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::suspectLimitHighChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::rejectLimitHighChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::rejectLimitLowChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::suspectLimitLowChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::timeSuspectLimitHighChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::timeRejectLimitHighChanged)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::timeRejectLimitLowChanged)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::timeSuspectLimitLowChanged)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::energySuspectLimitHighChanged)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::energyRejectLimitHighChanged)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::energyRejectLimitLowChanged)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::energySuspectLimitLowChanged)) {
                *result = 13;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::peakPowerSuspectLimitHighChanged)) {
                *result = 14;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::peakPowerRejectLimitHighChanged)) {
                *result = 15;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::peakPowerRejectLimitLowChanged)) {
                *result = 16;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::peakPowerSuspectLimitLowChanged)) {
                *result = 17;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::absoluteDistSuspectLimitHighChanged)) {
                *result = 18;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::absoluteDistRejectLimitHighChanged)) {
                *result = 19;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::absoluteDistRejectLimitLowChanged)) {
                *result = 20;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::absoluteDistSuspectLimitLowChanged)) {
                *result = 21;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::frequencySuspectLimitHighChanged)) {
                *result = 22;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::frequencyRejectLimitHighChanged)) {
                *result = 23;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::frequencyRejectLimitLowChanged)) {
                *result = 24;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::frequencySuspectLimitLowChanged)) {
                *result = 25;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxWeldForceSuspectLimitHighChanged)) {
                *result = 26;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxWeldForceRejectLimitHighChanged)) {
                *result = 27;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxWeldForceRejectLimitLowChanged)) {
                *result = 28;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxWeldForceSuspectLimitLowChanged)) {
                *result = 29;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::minWeldForceYValueChanged)) {
                *result = 30;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxWeldForceYValueChanged)) {
                *result = 31;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::minHoldForceYValueChanged)) {
                *result = 32;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxHoldForceYValueChanged)) {
                *result = 33;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::minTotalAbsoluteYValueChanged)) {
                *result = 34;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxTotalAbsoluteYValueChanged)) {
                *result = 35;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::minPartContactPosYValueChanged)) {
                *result = 36;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxPartContactPosYValueChanged)) {
                *result = 37;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::minWeldTimeYValueChanged)) {
                *result = 38;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxWeldTimeYValueChanged)) {
                *result = 39;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::minWeldEnergyYValueChanged)) {
                *result = 40;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxWeldEnergyYValueChanged)) {
                *result = 41;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::minWeldPeakPowerYValueChanged)) {
                *result = 42;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxWeldPeakPowerYValueChanged)) {
                *result = 43;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::minStartFrequencyYValueChanged)) {
                *result = 44;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxStartFrequencyYValueChanged)) {
                *result = 45;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::minFrequencyChangeYValueChanged)) {
                *result = 46;
                return;
            }
        }
        {
            typedef void (TrendsGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrendsGraph::maxFrequencyChangeYValueChanged)) {
                *result = 47;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TrendsGraph *_t = static_cast<TrendsGraph *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->getXCount(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->getTickCountY(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->getTimeSuspectLimitHigh(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->getTimeRejectLimitHigh(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->getTimeRejectLimitLow(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->getTimeSuspectLimitLow(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->getEnergySuspectLimitHigh(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->getEnergyRejectLimitHigh(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->getEnergyRejectLimitLow(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->getEnergySuspectLimitLow(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->getPeakPowerSuspectLimitHigh(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->getPeakPowerRejectLimitHigh(); break;
        case 12: *reinterpret_cast< int*>(_v) = _t->getPeakPowerRejectLimitLow(); break;
        case 13: *reinterpret_cast< int*>(_v) = _t->getPeakPowerSuspectLimitLow(); break;
        case 14: *reinterpret_cast< int*>(_v) = _t->getAbsoluteDistSuspectLimitHigh(); break;
        case 15: *reinterpret_cast< int*>(_v) = _t->getAbsoluteDistRejectLimitHigh(); break;
        case 16: *reinterpret_cast< int*>(_v) = _t->getAbsoluteDistRejectLimitLow(); break;
        case 17: *reinterpret_cast< int*>(_v) = _t->getAbsoluteDistSuspectLimitLow(); break;
        case 18: *reinterpret_cast< int*>(_v) = _t->getFrequencySuspectLimitHigh(); break;
        case 19: *reinterpret_cast< int*>(_v) = _t->getFrequencyRejectLimitHigh(); break;
        case 20: *reinterpret_cast< int*>(_v) = _t->getFrequencyRejectLimitLow(); break;
        case 21: *reinterpret_cast< int*>(_v) = _t->getFrequencySuspectLimitLow(); break;
        case 22: *reinterpret_cast< int*>(_v) = _t->getMaxWeldForceSuspectLimitHigh(); break;
        case 23: *reinterpret_cast< int*>(_v) = _t->getMaxWeldForceRejectLimitHigh(); break;
        case 24: *reinterpret_cast< int*>(_v) = _t->getMaxWeldForceRejectLimitLow(); break;
        case 25: *reinterpret_cast< int*>(_v) = _t->getMaxWeldForceSuspectLimitLow(); break;
        case 26: *reinterpret_cast< int*>(_v) = _t->getSuspectLimitHigh(); break;
        case 27: *reinterpret_cast< int*>(_v) = _t->getRejectLimitHigh(); break;
        case 28: *reinterpret_cast< int*>(_v) = _t->getRejectLimitLow(); break;
        case 29: *reinterpret_cast< int*>(_v) = _t->getSuspectLimitLow(); break;
        case 30: *reinterpret_cast< int*>(_v) = _t->getMinWeldForceYValue(); break;
        case 31: *reinterpret_cast< int*>(_v) = _t->getMaxWeldForceYValue(); break;
        case 32: *reinterpret_cast< int*>(_v) = _t->getMinHoldForceYValue(); break;
        case 33: *reinterpret_cast< int*>(_v) = _t->getMaxHoldForceYValue(); break;
        case 34: *reinterpret_cast< int*>(_v) = _t->getMinTotalAbsoluteYValue(); break;
        case 35: *reinterpret_cast< int*>(_v) = _t->getMaxTotalAbsoluteYValue(); break;
        case 36: *reinterpret_cast< long*>(_v) = _t->getMinPartContactPosYValue(); break;
        case 37: *reinterpret_cast< long*>(_v) = _t->getMaxPartContactPosYValue(); break;
        case 38: *reinterpret_cast< int*>(_v) = _t->getMinWeldTimeYValue(); break;
        case 39: *reinterpret_cast< int*>(_v) = _t->getMaxWeldTimeYValue(); break;
        case 40: *reinterpret_cast< int*>(_v) = _t->getMinWeldEnergyYValue(); break;
        case 41: *reinterpret_cast< int*>(_v) = _t->getMaxWeldEnergyYValue(); break;
        case 42: *reinterpret_cast< int*>(_v) = _t->getMinWeldPeakPowerYValue(); break;
        case 43: *reinterpret_cast< int*>(_v) = _t->getMaxWeldPeakPowerYValue(); break;
        case 44: *reinterpret_cast< int*>(_v) = _t->getMinStartFrequencyYValue(); break;
        case 45: *reinterpret_cast< int*>(_v) = _t->getMaxStartFrequencyYValue(); break;
        case 46: *reinterpret_cast< int*>(_v) = _t->getMinFrequencyChangeYValue(); break;
        case 47: *reinterpret_cast< int*>(_v) = _t->getMaxFrequencyChangeYValue(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        TrendsGraph *_t = static_cast<TrendsGraph *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setXCount(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setTickCountY(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setTimeSuspectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setTimeRejectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setTimeRejectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setTimeSuspectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setEnergySuspectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 7: _t->setEnergyRejectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 8: _t->setEnergyRejectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 9: _t->setEnergySuspectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 10: _t->setPeakPowerSuspectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 11: _t->setPeakPowerRejectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 12: _t->setPeakPowerRejectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 13: _t->setPeakPowerSuspectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 14: _t->setAbsoluteDistSuspectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 15: _t->setAbsoluteDistRejectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 16: _t->setAbsoluteDistRejectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 17: _t->setAbsoluteDistSuspectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 18: _t->setFrequencySuspectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 19: _t->setFrequencyRejectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 20: _t->setFrequencyRejectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 21: _t->setFrequencySuspectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 22: _t->setMaxWeldForceSuspectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 23: _t->setMaxWeldForceRejectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 24: _t->setMaxWeldForceRejectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 25: _t->setMaxWeldForceSuspectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 26: _t->setSuspectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 27: _t->setRejectLimitHigh(*reinterpret_cast< int*>(_v)); break;
        case 28: _t->setRejectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 29: _t->setSuspectLimitLow(*reinterpret_cast< int*>(_v)); break;
        case 30: _t->setMinWeldForceYValue(*reinterpret_cast< int*>(_v)); break;
        case 31: _t->setMaxWeldForceYValue(*reinterpret_cast< int*>(_v)); break;
        case 32: _t->setMinHoldForceYValue(*reinterpret_cast< int*>(_v)); break;
        case 33: _t->setMaxHoldForceYValue(*reinterpret_cast< int*>(_v)); break;
        case 34: _t->setMinTotalAbsoluteYValue(*reinterpret_cast< int*>(_v)); break;
        case 35: _t->setMaxTotalAbsoluteYValue(*reinterpret_cast< int*>(_v)); break;
        case 36: _t->setMinPartContactPosYValue(*reinterpret_cast< long*>(_v)); break;
        case 37: _t->setMaxPartContactPosYValue(*reinterpret_cast< long*>(_v)); break;
        case 38: _t->setMinWeldTimeYValue(*reinterpret_cast< int*>(_v)); break;
        case 39: _t->setMaxWeldTimeYValue(*reinterpret_cast< int*>(_v)); break;
        case 40: _t->setMinWeldEnergyYValue(*reinterpret_cast< int*>(_v)); break;
        case 41: _t->setMaxWeldEnergyYValue(*reinterpret_cast< int*>(_v)); break;
        case 42: _t->setMinWeldPeakPowerYValue(*reinterpret_cast< int*>(_v)); break;
        case 43: _t->setMaxWeldPeakPowerYValue(*reinterpret_cast< int*>(_v)); break;
        case 44: _t->setMinStartFrequencyYValue(*reinterpret_cast< int*>(_v)); break;
        case 45: _t->setMaxStartFrequencyYValue(*reinterpret_cast< int*>(_v)); break;
        case 46: _t->setMinFrequencyChangeYValue(*reinterpret_cast< int*>(_v)); break;
        case 47: _t->setMaxFrequencyChangeYValue(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject TrendsGraph::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TrendsGraph.data,
      qt_meta_data_TrendsGraph,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TrendsGraph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrendsGraph::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TrendsGraph.stringdata0))
        return static_cast<void*>(const_cast< TrendsGraph*>(this));
    return QObject::qt_metacast(_clname);
}

int TrendsGraph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 60)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 60;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 60)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 60;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 48;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 48;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 48;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 48;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 48;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 48;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TrendsGraph::valueXChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TrendsGraph::tickCountYChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TrendsGraph::suspectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TrendsGraph::rejectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TrendsGraph::rejectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TrendsGraph::suspectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TrendsGraph::timeSuspectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TrendsGraph::timeRejectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TrendsGraph::timeRejectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TrendsGraph::timeSuspectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TrendsGraph::energySuspectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void TrendsGraph::energyRejectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void TrendsGraph::energyRejectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void TrendsGraph::energySuspectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void TrendsGraph::peakPowerSuspectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void TrendsGraph::peakPowerRejectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void TrendsGraph::peakPowerRejectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void TrendsGraph::peakPowerSuspectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void TrendsGraph::absoluteDistSuspectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void TrendsGraph::absoluteDistRejectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void TrendsGraph::absoluteDistRejectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void TrendsGraph::absoluteDistSuspectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void TrendsGraph::frequencySuspectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void TrendsGraph::frequencyRejectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}

// SIGNAL 24
void TrendsGraph::frequencyRejectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void TrendsGraph::frequencySuspectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 25, nullptr);
}

// SIGNAL 26
void TrendsGraph::maxWeldForceSuspectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 26, nullptr);
}

// SIGNAL 27
void TrendsGraph::maxWeldForceRejectLimitHighChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}

// SIGNAL 28
void TrendsGraph::maxWeldForceRejectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 28, nullptr);
}

// SIGNAL 29
void TrendsGraph::maxWeldForceSuspectLimitLowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void TrendsGraph::minWeldForceYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 30, nullptr);
}

// SIGNAL 31
void TrendsGraph::maxWeldForceYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 31, nullptr);
}

// SIGNAL 32
void TrendsGraph::minHoldForceYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 32, nullptr);
}

// SIGNAL 33
void TrendsGraph::maxHoldForceYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 33, nullptr);
}

// SIGNAL 34
void TrendsGraph::minTotalAbsoluteYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 34, nullptr);
}

// SIGNAL 35
void TrendsGraph::maxTotalAbsoluteYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 35, nullptr);
}

// SIGNAL 36
void TrendsGraph::minPartContactPosYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 36, nullptr);
}

// SIGNAL 37
void TrendsGraph::maxPartContactPosYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 37, nullptr);
}

// SIGNAL 38
void TrendsGraph::minWeldTimeYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 38, nullptr);
}

// SIGNAL 39
void TrendsGraph::maxWeldTimeYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 39, nullptr);
}

// SIGNAL 40
void TrendsGraph::minWeldEnergyYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 40, nullptr);
}

// SIGNAL 41
void TrendsGraph::maxWeldEnergyYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 41, nullptr);
}

// SIGNAL 42
void TrendsGraph::minWeldPeakPowerYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 42, nullptr);
}

// SIGNAL 43
void TrendsGraph::maxWeldPeakPowerYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 43, nullptr);
}

// SIGNAL 44
void TrendsGraph::minStartFrequencyYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 44, nullptr);
}

// SIGNAL 45
void TrendsGraph::maxStartFrequencyYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 45, nullptr);
}

// SIGNAL 46
void TrendsGraph::minFrequencyChangeYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 46, nullptr);
}

// SIGNAL 47
void TrendsGraph::maxFrequencyChangeYValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 47, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
