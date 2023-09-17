#ifndef SYSTEMCONFIGGENERAL_H
#define SYSTEMCONFIGGENERAL_H

#include <QObject>

#define SYS_CONFIG_EXTRA_COOLING            QObject::tr("Extra Cooling")
#define SYS_CONFIG_EXTERNAL_PRESETS         QObject::tr("External Presets")
#define SYS_CONFIG_PS_TYPE                  QObject::tr("PS Type")
#define SYS_CONFIG_ACTUATOR_TYPE            QObject::tr("Actuator Type")
#define SYS_CONFIG_MEMORY_FULL_ACTION       QObject::tr("Memory Full Action")
#define SYS_CONFIG_AUTHORITY_CHECK          QObject::tr("Authority Check")
#define SYS_CONFIG_PS_WATT                  QObject::tr("PS Watt")
#define SYS_CONFIG_HORN_CLAMP               QObject::tr("Horn Clamp")

#define SYS_CONFIG_LANGUAGE                 QObject::tr("Language")
#define SYS_CONFIG_PS_FREQUENCY             QObject::tr("PS Frequency (Hz)")
#define SYS_CONFIG_UNITS                    QObject::tr("Units")
#define SYS_CONFIG_START_SCREEN             QObject::tr("Start Screen")
#define SYS_CONFIG_PS_POWER_ON_OPTION       QObject::tr("PS Power On Option")
#define SYS_CONFIG_MACHINE_NAME             QObject::tr("Machine Name")
#define SYS_CONFIG_BRANSON_KEY              QObject::tr("Key")
#define SYS_CONFIG_BARCODE_RECIPE_PREFIX    QObject::tr("Barcode recall recipe prefix")
#define SYS_CONFIG_PARTID_STATUS            QObject::tr("Part ID Scan")
#define SYS_CONFIG_RESET_MEMORY             QObject::tr("Reset Memory")
#define SYS_CONFIG_POWERUP_MEMORY           QObject::tr("Powerup Memory")
#define SYS_CONFIG_STROKE_LENGTH            QObject::tr("Stroke Length (mm)")
#define SYS_CONFIG_SYSTEM_TYPE              QObject::tr("System Type")
#define SYS_SC_SERIAL_NUMBER                QObject::tr("SC Serial Number")
#define SYS_PC_SERIAL_NUMBER                QObject::tr("PC Serial Number")
#define SYS_AC_SERIAL_NUMBER                QObject::tr("AC Serial Number")
#define SYS_AUX_BOX_SERIAL_NUMBER           QObject::tr("Aux Box Serial Number")
#define SYS_CONFIG_SSD_SATA                 QObject::tr("SSD SATA")
#define SYS_CONFIG_PS_POWER_LEVEL           QObject::tr("PS Power Level (W)")

#define SYS_SERVICE_FACTORY_RESET           QObject::tr("Factory Reset")
#define SYS_SERVICE_DELETE_DB               QObject::tr("Delete DB")
#define SYS_SERVICE_COPY_DB                 QObject::tr("Copy DB to USB")
#define SYS_SERVICE_LIFE_TIME_ALARMS        QObject::tr("Life Time Alarms")
#define SYS_SERVICE_LIFE_TIME_WELDS         QObject::tr("Life Time Welds")
#define SYS_SERVICE_EMMC_WEAR_LEVEL         QObject::tr("EMMC Memory Usage")
#define SYS_SERVICE_BOARD_REPLACED          QObject::tr("Board Replaced")
#define SYS_SERVICE_SYSTEM_CLEANED          QObject::tr("System Cleaned")
#define SYS_SERVICE_SERVICE_NOTES           QObject::tr("Service Notes")


class SystemConfigGeneral : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ParameterName READ getParameterName WRITE setParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(bool ParameterSwitch READ getParameterSwitch WRITE setParameterSwitch NOTIFY ParameterSwitchChanged)
    Q_PROPERTY(bool ParameterComboBox READ getParameterComboBox WRITE setParameterComboBox NOTIFY ParameterComboBoxChanged)
    Q_PROPERTY(bool ParameterTextEdit READ getParameterTextEdit WRITE setParameterTextEdit NOTIFY ParameterTextEditChanged)
    Q_PROPERTY(bool ParameterSwitchState READ getParameterSwitchState WRITE setParameterSwitchState NOTIFY ParameterSwitchStateChanged)
    Q_PROPERTY(QString ParamTextEditName READ getParamTextEditName WRITE setParamTextEditName NOTIFY paramTextEditNameChanged)
    Q_PROPERTY(int ComboBoxModelName READ getComboBoxModelName  WRITE setComboBoxModelName  NOTIFY ComboBoxModelNameChanged)
    Q_PROPERTY(bool PrimaryButton READ getPrimaryButton WRITE setPrimaryButton NOTIFY primaryButtonStatusChanged)
    Q_PROPERTY(bool ParameterText READ getText WRITE setText NOTIFY textStatusChanged)
    Q_PROPERTY(bool ssdToggleSwitch READ getSsdToggleSwitch WRITE setSsdToggleSwitch  NOTIFY ssdToggleSwitchStateChanged)
    Q_PROPERTY(QString ParamButtonText READ getParamButtonText WRITE setParamButtonText NOTIFY parameterBuutonTextChanged)

public:
    SystemConfigGeneral(QObject *parent=0);
    SystemConfigGeneral(const QString name,bool isSwitch=false, bool switchState=false, QObject *parent=0);

    QString m_ParameterName;
    bool m_ParameterSwitch;
    bool m_ParameterComboBox;
    bool m_ParameterTextEdit;
    bool m_ParameterSwitchState;
    bool m_PrimaryButton;
    bool m_text;
    bool m_ssdToggleSwitch;
    QString m_ParamTextEditName;
    int m_ComboBoxModelName;
    QString m_ParamButtonText;

    QString getParameterName() const;
    void setParameterName(const QString name);

    bool getParameterSwitch();
    void setParameterSwitch(bool isSwitch);

    bool getParameterComboBox();
    void setParameterComboBox(bool isSwitch);

    int getComboBoxModelName();
    void setComboBoxModelName(int ind);

    bool getParameterTextEdit();
    void setParameterTextEdit(bool isSwitch);

    bool getParameterSwitchState();
    void setParameterSwitchState(bool switchState);

    QString getParamTextEditName() const;
    void setParamTextEditName(const QString name);

    bool getPrimaryButton() const;
    void setPrimaryButton(bool PrimaryButton);

    bool getText() const;
    void setText(bool text);

    bool getSsdToggleSwitch() const;
    void setSsdToggleSwitch(bool ssdToggleSwitch);

    QString getParamButtonText() const;
    void setParamButtonText(const QString ParamButtonText);

signals:
    void parameterNameChanged();
    void ParameterSwitchChanged();
    void ParameterComboBoxChanged();
    void ParameterTextEditChanged();
    void ParameterSwitchStateChanged();
    void paramTextEditNameChanged();
    void ComboBoxModelNameChanged();
    void primaryButtonStatusChanged();
    void textStatusChanged();
    void ssdToggleSwitchStateChanged();
    void parameterBuutonTextChanged();

};

#endif // SYSTEMCONFIGGENERAL_H
