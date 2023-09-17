#ifndef USERIOSTRUCTS_H
#define USERIOSTRUCTS_H

/* USER I/O  Structures*/
#include <QObject>
#define COMMA ","
#define INVERTED_COMMA "'"
#define OPENING_PARENTHESES "("
#define CLOSING_PARENTHESES ")"
#define  PS_PIN1   "J17-01"
#define  PS_PIN2   "J17-02"
#define  PS_PIN3   "J17-03"
#define  PS_PIN4   "J17-04"
#define  PS_PIN7   "J17-07"
#define  PS_PIN8   "J17-08"
#define  PS_PIN9   "J17-09"
#define  PS_PIN10  "J17-10"
#define  PS_PIN11  "J17-11"
#define  PS_PIN12  "J17-12"
#define  PS_PIN13  "J17-13"
#define  PS_PIN16  "J17-16"
#define  PS_PIN17  "J17-17"
#define  PS_PIN18  "J17-18"
#define  PS_PIN19  "J17-19"
#define  PS_PIN20  "J17-20"
#define  PS_PIN21  "J17-21"
#define  PS_PIN22  "J17-22"
#define  PS_PIN23  "J17-23"
#define  PS_PIN24  "J17-24"
#define  PS_PIN25  "J17-25"
#define  AC_PIN1   "J58-01"
#define  AC_PIN2   "J58-02"
#define  AC_PIN3   "J58-03"
#define  AC_PIN4   "J58-04"
#define  AC_PIN5   "J58-05"
#define  AC_PIN6   "J58-06"
#define  AC_PIN7   "J58-07"
#define  AC_PIN8   "J58-08"
#define  AC_PIN9   "J58-09"
#define  AC_PIN10  "J58-10"
#define  AC_PIN11  "J58-11"
#define  AC_PIN12  "J58-12"
#define  AC_PIN13  "J58-13"
#define  AC_PIN14  "J58-14"
#define  EMPTY_STRING       ""
#define IS_CHECKBOX_CHECKED true
#define IS_PIN_ENABLED      true
#define ACTIVE_STATUS       false
#define DEFAULT_ACTIVE_STATUS !ACTIVE_STATUS
#define NOT_USED 0
#define ACTIVE_STATUS_NOT_APPLICABLE         0
#define PIN_FUNCTION_NAME_NOT_USED           QObject::tr("Not Used")

#define PIN_FUNCTION_NAME_AMPLITUDE_SETTING  QObject::tr("Amplitude Setting")
#define PIN_FUNCTION_NAME_FREQUENCY_OFFSET   QObject::tr("Frequency Offset")

#define PIN_FUNCTION_NAME_POWER_OUT          QObject::tr("Power Output")
#define PIN_FUNCTION_NAME_FREQUENCY_OUT      QObject::tr("Frequency Out")

#define PIN_FUNCTION_NAME_TOOLING_IN         QObject::tr("Tooling Input")
#define PIN_FUNCTION_NAME_PART_PRESENT       QObject::tr("Part Present")
#define PIN_FUNCTION_NAME_HOME_POSITION_IN   QObject::tr("Home Position")
#define PIN_FUNCTION_NAME_READY_POSITION_IN  QObject::tr("Ready Position")

#define PIN_FUNCTION_NAME_CYCLEABORT         QObject::tr("Cycle Abort")
#define PIN_FUNCTION_NAME_US_DISABLE         QObject::tr("U/S Disable")
#define PIN_FUNCTION_NAME_RESET              QObject::tr("Reset")
#define PIN_FUNCTION_NAME_CONFIRM_REJECT     QObject::tr("Confirm Reject")
#define PIN_FUNCTION_NAME_HOLD_DELAY         QObject::tr("Hold Delay")
#define PIN_FUNCTION_NAME_HORN_SEEK_IN       QObject::tr("Horn Seek")

#define PIN_FUNCTION_NAME_HOMEPOSITION_ULS   QObject::tr("Home Position")
#define PIN_FUNCTION_NAME_READY_POSITION     QObject::tr("Ready Position")
#define PIN_FUNCTION_NAME_PB_RELEASE         QObject::tr("PB Release")
#define PIN_FUNCTION_NAME_TOOLING_OUT        QObject::tr("Tooling Output")
#define PIN_FUNCTION_NAME_SOLENOID_VALVE     QObject::tr("Solenoid Valve")
#define PIN_FUNCTION_NAME_HOLD_ACTIVE        QObject::tr("Hold Active")

#define PIN_FUNCTION_NAME_READY              QObject::tr("Ready")
#define PIN_FUNCTION_NAME_SONICS_ACTIVE      QObject::tr("Sonics Active")
#define PIN_FUNCTION_NAME_GENERAL_ALARM      QObject::tr("General Alarm")
#define PIN_FUNCTION_NAME_CYCLE_RUNNING      QObject::tr("Cycle Running")
#define PIN_FUNCTION_NAME_EXTERNAL_BEEPER    QObject::tr("External Beeper")
#define PIN_FUNCTION_NAME_CYCLE_OK           QObject::tr("Cycle OK")
#define PIN_FUNCTION_NAME_CUSTOM_ALARM       QObject::tr("Custom Alarm")
#define PIN_FUNCTION_NAME_REJECTPART_ALARM   QObject::tr("Reject Alarm")
#define PIN_FUNCTION_NAME_SUSPECTPART_ALARM  QObject::tr("Suspect Alarm")
#define PIN_FUNCTION_NAME_HORN_SEEK_OUT      QObject::tr("Horn Seek")
#define PIN_FUNCTION_NAME_WELD_ACTIVE        QObject::tr("Weld Active")

#define PS_ANALOG_INPUT_PARAMS_CONTEXT_PROPERTY    "analogInputParamsPS"
#define PS_ANALOG_OUTPUTS_PARAMS_CONTEXT_PROPERTY  "analogOutputParamsPS"
#define PS_DIGITAL_INPUTS_PARAMS_CONTEXT_PROPERTY  "digitalInputParamsPS"
#define PS_DIGITAL_OUTPUTS_PARAMS_CONTEXT_PROPERTY "digitalOutputParamsPS"
#define AC_DIGITAL_INPUTS_PARAMS_CONTEXT_PROPERTY  "digitalInputParamsAC"
#define AC_DIGITAL_OUTPUTS_PARAMS_CONTEXT_PROPERTY "digitalOutputParamsAC"

#define PS_DIGITAL_INPUT_CONTEXT_PROPERTY        "userIOPSDIObj"
#define PS_DIGITAL_OUTPUT_CONTEXT_PROPERTY       "userIOPSDOObj"
#define PS_ANALOG_INPUT_CONTEXT_PROPERTY         "userIOPSAIObj"
#define PS_ANALOG_OUTPUT_CONTEXT_PROPERTY        "userIOPSAOObj"

#define AC_DIGITAL_INPUT_CONTEXT_PROPERTY        "userIOACDIObj"
#define AC_DIGITAL_OUTPUT_CONTEXT_PROPERTY        "userIOACDOObj"

#define PS_CONTEXT_PROPERTY        "userIOPSObj"
#define AC_CONTEXT_PROPERTY        "userIOACObj"

#define USERIO_CONTEXT_PROPERTY "userioObj"

#define RANGE_PS_ANALOG_INPUTS_ID_IN_DB "18,19"
#define RANGE_PS_ANALOG_OUTPUTS_ID_IN_DB "20,21"
#define RANGE_PS_DIGITAL_INPUTS_ID_IN_DB "1,9"
#define RANGE_PS_DIGITAL_OUTPUTS_ID_IN_DB "10,17"
#define RANGE_AC_DIGITAL_INPUTS_ID_IN_DB "22,26"
#define RANGE_AC_DIGITAL_OUTPUTS_ID_IN_DB "27,31"

#define  PS_PIN_NUMBER1   "01"
#define  PS_PIN_NUMBER2   "02"
#define  PS_PIN_NUMBER3   "03"
#define  PS_PIN_NUMBER4   "04"
#define  PS_PIN_NUMBER7   "07"
#define  PS_PIN_NUMBER8   "08"
#define  PS_PIN_NUMBER9   "09"
#define  PS_PIN_NUMBER10  "10"
#define  PS_PIN_NUMBER11  "11"
#define  PS_PIN_NUMBER12  "12"
#define  PS_PIN_NUMBER13  "13"
#define  PS_PIN_NUMBER16  "16"
#define  PS_PIN_NUMBER17  "17"
#define  PS_PIN_NUMBER18  "18"
#define  PS_PIN_NUMBER19  "19"
#define  PS_PIN_NUMBER20  "20"
#define  PS_PIN_NUMBER21  "21"
#define  PS_PIN_NUMBER22  "22"
#define  PS_PIN_NUMBER23  "23"
#define  PS_PIN_NUMBER24  "24"
#define  PS_PIN_NUMBER25  "25"
#define  AC_PIN_NUMBER1   "01"
#define  AC_PIN_NUMBER2   "02"
#define  AC_PIN_NUMBER3   "03"
#define  AC_PIN_NUMBER4   "04"
#define  AC_PIN_NUMBER5   "05"
#define  AC_PIN_NUMBER6   "06"
#define  AC_PIN_NUMBER7   "07"
#define  AC_PIN_NUMBER8   "08"
#define  AC_PIN_NUMBER9   "09"
#define  AC_PIN_NUMBER10  "10"

#define IO_DIAGNOSTIC_DIGITAL_INPUT_PS  "digitalInputParamsIODiagnosticPS"
#define IO_DIAGNOSTIC_DIGITAL_OUTPUT_PS "digitalOutputParamsIODiagnosticPS"
#define IO_DIAGNOSTIC_ANALOG_INPUT_PS   "analogInputParamsIODiagnosticPS"
#define IO_DIAGNOSTIC_ANALOG_OUTPUT_PS  "analogOutputParamsIODiagnosticPS"
#define IO_DIAGNOSTIC_DIGITAL_INPUT_AC  "digitalInputParamsIODiagnosticAC"
#define IO_DIAGNOSTIC_DIGITAL_OUTPUT_AC "digitalOutputParamsIODiagnosticAC"

enum ActuatorInput
{
    ACTUATORINPUT_NOTUSED,
    PARTPRESENT,
    TOOLING_IN,
    HOME_POSITION_IN,
    READY_POSITION_IN
};

enum PowerSupplyInput
{
    PSINPUT_NOTUSED=0,
    CYCLEABORT,
    US_DISABLE,
    RESET,
    HOLD_DELAY,
    CONFIRM_REJECT,
    HORN_SEEK_IN
};

enum ActuatorOutput
{
    ACTUATOROUTPUT_NOTUSED,
    HOMEPOSITION_OUT,
    READY_POSITION_OUT,
    PB_RELEASE,
    TOOLING_OUT,
    SOLENOID_VALVE,
    HOLD_ACTIVE
};

enum PowerSupplyOutput
{
    PSOUTPUT_NOTUSED,
    READY,
    SONICS_ACTIVE,
    GENERAL_ALARM,
    CYCLE_RUNNING,
    CYCLE_OK,
    EXTERNAL_BEEPER,
    CUSTOM_ALARM,
    REJECTPART_ALARM,
    SUSPECTPART_ALARM,
    HORN_SEEK_OUT,
    WELD_ACTIVE,
    PSHOLD_ACTIVE
};

enum AnalogInput
{
    ANALOGINPUT_NOTUSED,
    AMPLITUDE_SETTING,
    FREQUENCY_OFFSET
};

enum AnalogOutput
{
    ANALOGOUTPUT_NOTUSED,
    POWER_OUT,
    FREQUENCY_OUT
};

struct PS_INPUT
{
    PowerSupplyInput     PS_EVENT;
    bool                 Status;
};

struct PS_OUTPUT
{
    PowerSupplyOutput    PS_EVENT;
    bool                 Status;
};

struct AC_INPUT
{
    ActuatorInput        AC_EVENT;
    bool                 Status;
};

struct AC_OUTPUT
{
    ActuatorOutput       AC_EVENT;
    bool                 Status;
};

struct PS_DIGITALINPUT
{
    PS_INPUT             J116_01;
    PS_INPUT             J116_02;
    PS_INPUT             J116_03;
    PS_INPUT             J116_04;
    PS_INPUT             J116_11;
    PS_INPUT             J116_12;
    PS_INPUT             J116_13;
    PS_INPUT             J116_16;
    PS_INPUT             J116_23;
};

struct PS_DIGITALOUTPUT
{
    PS_OUTPUT            J116_07;
    PS_OUTPUT            J116_08;
    PS_OUTPUT            J116_09;
    PS_OUTPUT            J116_10;
    PS_OUTPUT            J116_19;
    PS_OUTPUT            J116_20;
    PS_OUTPUT            J116_21;
    PS_OUTPUT            J116_22;
};

struct PS_ANALOGINPUT
{
    AnalogInput          J116_17;
    AnalogInput          J116_18;

};

struct PS_ANALOGOUTPUT
{
    AnalogOutput         J116_24;
    AnalogOutput         J116_25;

};

struct AC_DIGITALINPUT
{
    AC_INPUT             J710_01;
    AC_INPUT             J710_02;
    AC_INPUT             J710_03;
    AC_INPUT             J710_09;
    AC_INPUT             J710_10;

};

struct AC_DIGITALOUTPUT
{
    AC_OUTPUT            J710_05;
    AC_OUTPUT            J710_06;
    AC_OUTPUT            J710_07;
    AC_OUTPUT            J710_13;
    AC_OUTPUT            J710_14;
};

struct POWERSUPPLY_USERIO
{
    PS_DIGITALINPUT      PsDigitalInput;
    PS_DIGITALOUTPUT     PsDigitalOutput;

    PS_ANALOGINPUT       PsAnalogInput;
    PS_ANALOGOUTPUT      PsAnalogOutput;
};

struct ACTUATOR_USERIO
{
    AC_DIGITALINPUT      ACDigitalInput;
    AC_DIGITALOUTPUT     ACDigitalOutput;
};

struct USERIO
{
    POWERSUPPLY_USERIO   PS_USERIO;
    ACTUATOR_USERIO      AC_USERIO;
};

struct PS_DIGITALINPUT_CHECKSTATUS
{
   bool J116_01;
   bool J116_02;
   bool J116_03;
   bool J116_04;
   bool J116_11;
   bool J116_12;
   bool J116_13;
   bool J116_16;
   bool J116_23;
};

struct PS_DIGITALOUTPUT_CHECKSTATUS
{
    bool J116_07;
    bool J116_08;
    bool J116_09;
    bool J116_10;
    bool J116_19;
    bool J116_20;
    bool J116_21;
    bool J116_22;
};

struct PS_ANALOGINPUT_CHECKSTATUS
{
    bool J116_17;
    bool J116_18;
};

struct PS_ANALOGOUTPUT_CHECKSTATUS
{
    bool J116_24;
    bool J116_25;
};

struct AC_DIGITALINPUT_CHECHSTATUS
{
   bool J710_01;
   bool J710_02;
   bool J710_03;
   bool J710_09;
   bool J710_10;
};

struct AC_DIGITALOUTPUT_CHECKSTATUS
{
   bool J710_05;
   bool J710_06;
   bool J710_07;
   bool J710_13;
   bool J710_14;
};

struct POWERSUPPLY_USERIO_CHECKSTATUS
{
    PS_DIGITALINPUT_CHECKSTATUS PsDigitalInputCheckStatus;
    PS_DIGITALOUTPUT_CHECKSTATUS PsDigitalOutputCheckStatus;
    PS_ANALOGINPUT_CHECKSTATUS PsAnalogInputCheckStatus;
    PS_ANALOGOUTPUT_CHECKSTATUS  PsAnalogOutputCheckStatus;
};

struct ACTUATOR_USERIO_CHECKSTATUS
{
    AC_DIGITALINPUT_CHECHSTATUS ACDigitalInputCheckStatus;
    AC_DIGITALOUTPUT_CHECKSTATUS ACDigitalOutputCheckStatus;
};

struct USERIO_CHECKSTATUS
{
    POWERSUPPLY_USERIO_CHECKSTATUS PS_USERIO_CHECKSTATUS;
    ACTUATOR_USERIO_CHECKSTATUS AC_USERIO_CHECKSTATUS;
};

#endif // USERIOSTRUCTS_H
