#include <QQmlContext>
#include <algorithm>

#include "Header/SystemConfigGeneral.h"

extern QQmlContext*context;

/**
 * @brief SystemConfigGeneral::SystemConfigGeneral : Constructor
 * @param parent
 */
SystemConfigGeneral::SystemConfigGeneral(QObject *parent)
    : QObject(parent)
{
    m_ParameterSwitch = false;
    m_ParameterSwitchState = false;
}

/**
 * @brief SystemConfigGeneral::SystemConfigGeneral  : Assign values to Global variable
 * @param name
 * @param value
 * @param unit
 * @param isSwitch
 * @param switchState
 * @param parent
 */
SystemConfigGeneral::SystemConfigGeneral(const QString name, bool isSwitch, bool switchState, QObject *parent)
    : QObject(parent)
{
    m_ParameterName = name;
    m_ParameterSwitch = isSwitch;
    m_ParameterSwitchState = switchState;
}

/**
 * @brief SystemConfigGeneral::getParamButtonText
 * @return
 */
QString SystemConfigGeneral::getParamButtonText() const
{
    return m_ParamButtonText;
}

/**
 * @brief SystemConfigGeneral::setParamButtonText
 * @param ParamButtonText
 */
void SystemConfigGeneral::setParamButtonText(const QString ParamButtonText)
{
    if(ParamButtonText != m_ParamButtonText)
    {
        m_ParamButtonText = ParamButtonText;
        emit parameterBuutonTextChanged();
    }
}


/**
 * @brief SystemConfigGeneral::getSsdToggleSwitch
 * @return
 */
bool SystemConfigGeneral::getSsdToggleSwitch() const
{
    return m_ssdToggleSwitch;
}

/**
 * @brief SystemConfigGeneral::setSsdToggleSwitch
 * @param ssdToggleSwitch
 */
void SystemConfigGeneral::setSsdToggleSwitch(bool ssdToggleSwitch)
{
    if (ssdToggleSwitch != m_ssdToggleSwitch)
    {
        m_ssdToggleSwitch = ssdToggleSwitch;
        emit ssdToggleSwitchStateChanged();
    }
}

/**
 * @brief SystemConfigGeneral::getParameterName
 * @return
 */
QString SystemConfigGeneral::getParameterName() const
{
    return m_ParameterName;
}

/**
 * @brief SystemConfigGeneral::setParameterName
 * @param name
 */
void SystemConfigGeneral::setParameterName(const QString name)
{
    if (name != m_ParameterName)
    {
        m_ParameterName = name;
        emit parameterNameChanged();
    }
}

/**
 * @brief SystemConfigGeneral::getParamTextEditName
 * @return
 */
QString SystemConfigGeneral::getParamTextEditName() const
{
    return m_ParamTextEditName;
}

/**
 * @brief SystemConfigGeneral::setParamTextEditName
 * @param name
 */
void SystemConfigGeneral::setParamTextEditName(const QString name)
{
    if (name != m_ParamTextEditName)
    {
        m_ParamTextEditName = name;
        emit paramTextEditNameChanged();
    }
}

/**
 * @brief SystemConfigGeneral::getPrimaryButton
 * @return
 */
bool SystemConfigGeneral::getPrimaryButton() const
{
    return m_PrimaryButton;
}

/**
 * @brief SystemConfigGeneral::setPrimaryButton
 * @param PrimaryButton
 */
void SystemConfigGeneral::setPrimaryButton(bool PrimaryButton)
{
    if (PrimaryButton != m_PrimaryButton)
    {
        m_PrimaryButton = PrimaryButton;
        emit primaryButtonStatusChanged();
    }
}

/**
 * @brief SystemConfigGeneral::getText
 * @return
 */
bool SystemConfigGeneral::getText() const
{
    return m_text;
}

/**
 * @brief SystemConfigGeneral::setText
 * @param text
 */
void SystemConfigGeneral::setText(bool text)
{
    if (text != m_text)
    {
        m_text = text;
        emit textStatusChanged();
    }
}

/**
 * @brief SystemConfigGeneral::getParameterSwitch
 * @return
 */
bool SystemConfigGeneral::getParameterSwitch()
{
    return m_ParameterSwitch;
}

/**
 * @brief SystemConfigGeneral::setParameterSwitch
 * @param isSwitch
 */
void SystemConfigGeneral::setParameterSwitch(bool isSwitch)
{
    if (isSwitch != m_ParameterSwitch)
    {
        m_ParameterSwitch = isSwitch;
        emit ParameterSwitchChanged();
    }
}

/**
 * @brief SystemConfigGeneral::getParameterComboBox
 * @return
 */
bool SystemConfigGeneral::getParameterComboBox()
{
    return m_ParameterComboBox;
}

/**
 * @brief SystemConfigGeneral::setParameterComboBox
 * @param isSwitch
 */
void SystemConfigGeneral::setParameterComboBox(bool isSwitch)
{
    if (isSwitch != m_ParameterComboBox)
    {
        m_ParameterComboBox = isSwitch;
        emit ParameterComboBoxChanged();
    }
}

/**
 * @brief SystemConfigGeneral::getComboBoxModelName
 * @return
 */
int SystemConfigGeneral::getComboBoxModelName()
{
    return m_ComboBoxModelName;
}

/**
 * @brief SystemConfigGeneral::setComboBoxModelName
 * @param isSwitch
 */
void SystemConfigGeneral::setComboBoxModelName(int ind)
{
    if (ind != m_ComboBoxModelName)
    {
        m_ComboBoxModelName = ind;
        emit ComboBoxModelNameChanged();
    }
}

/**
 * @brief SystemConfigGeneral::getParameterTextEdit
 * @return
 */
bool SystemConfigGeneral::getParameterTextEdit()
{
    return m_ParameterTextEdit;
}

/**
 * @brief SystemConfigGeneral::setParameterTextEdit
 * @param isSwitch
 */
void SystemConfigGeneral::setParameterTextEdit(bool isSwitch)
{
    if (isSwitch != m_ParameterTextEdit)
    {
        m_ParameterTextEdit = isSwitch;
        emit ParameterTextEditChanged();
    }
}

/**
  * @brief SystemConfigGeneral::getParameterSwitchState
  * @return
  */
bool SystemConfigGeneral::getParameterSwitchState()
{
    return m_ParameterSwitchState;
}

/**
 * @brief SystemConfigGeneral::setParameterSwitchState
 * @param switchState
 */
void SystemConfigGeneral::setParameterSwitchState(bool switchState)
{
    if (switchState != m_ParameterSwitchState)
    {
        m_ParameterSwitchState = switchState;
        emit ParameterSwitchStateChanged();
    }
}
