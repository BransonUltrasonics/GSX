#include "Header/CustomValueAxis.h"

/**
 * @brief CustomValueAxis::CustomValueAxis : Constructor, Initialization of class members
 * @param parent
 */
CustomValueAxis::CustomValueAxis(QQuickItem *parent) : QQuickItem(parent)
{
    this->setProperty("custom", "CustomValueAxis");

    m_color = "#ffffff";
    m_gridVisible = true;
    m_labelsVisible = true;
    m_labelsColor = "#ffffff";
    m_lineVisible = true;
    m_min = 0;
    m_max = 1;
    m_tickCount = 2;
    m_labelsFont.setPixelSize(10);
}

/**
 * @brief CustomValueAxis::operator : copy assignment operator
 * @param &other
 */
bool CustomValueAxis::operator!=(const CustomValueAxis &other)
{
    if(m_color == other.color() && m_gridVisible == other.gridVisible() && m_labelsVisible == other.labelsVisible()
            && m_labelsFont == other.labelsFont() && m_labelsColor == other.labelsColor() && m_lineVisible == other.lineVisible()
            && m_titleText == other.titleText() && m_titleVisible == other.titleVisible()
            && m_decimals == other.decimals()
            && !(m_min < other.min() || m_min > other.min())
            && !(m_max < other.max() || m_max > other.max())
            && m_tickCount == other.tickCount())
    {
        return true;
    }
    return false;
}

/**
 * @brief CustomValueAxis::operator : overloading copy assignment operator
 * @param &other
 */
void CustomValueAxis::operator=(const CustomValueAxis &other)
{
    this->setColor(other.color());
    this->setGridVisible(other.gridVisible());
    this->setLabelsVisible(other.labelsVisible());
    this->setLabelsFont(other.labelsFont());
    this->setLabelsColor(other.labelsColor());
    this->setLineVisible(other.lineVisible());
    this->setTitleText(other.titleText());
    this->setTitleVisible(other.titleVisible());
    this->setDecimals(other.decimals());
    this->setMin(other.min());
    this->setMax(other.max());
    this->setTickCount(other.tickCount());
}

/**
 * @briefCustomValueAxis::getAxisInfo() : returns Axis info
 */
TagAxisInfo* CustomValueAxis::getAxisInfo()
{
    return &m_axisInfo;
}

/**
 * @briefCustomValueAxis::color : returns color
 */
QColor CustomValueAxis::color() const
{
    return m_color;
}

/**
 * @briefCustomValueAxis::setColor : the slot implementation for color
 * @param &color
 */
void CustomValueAxis::setColor(const QColor &color)
{
    if(m_color != color)
    {
        m_color = color;
        emit colorChanged();
    }
}

/**
 * @briefCustomValueAxis::gridVisible : returns the grid visible property
 */
bool CustomValueAxis::gridVisible() const
{
    return m_gridVisible;
}

/**
 * @briefCustomValueAxis::setGridVisible : the slot implementation for setting the grid visible
 * @param gridVisible
 */
void CustomValueAxis::setGridVisible(bool gridVisible)
{
    if(m_gridVisible != gridVisible)
    {
        m_gridVisible = gridVisible;
        emit gridVisibleChanged();
    }
}

/**
 * @briefCustomValueAxis::labelsVisible : returns the label visible property
 */
bool CustomValueAxis::labelsVisible() const
{
    return m_labelsVisible;
}

/**
 * @briefCustomValueAxis::setGridVisible : the slot implementation for setting the label visible
 * @param labelsVisible
 */
void CustomValueAxis::setLabelsVisible(bool labelsVisible)
{
    if(m_labelsVisible != labelsVisible)
    {
        m_labelsVisible = labelsVisible;
        emit labelsVisibleChanged();
    }
}

/**
 * @briefCustomValueAxis::labelsFont : returns the label font
 */
QFont CustomValueAxis::labelsFont() const
{
    return m_labelsFont;
}

/**
 * @briefCustomValueAxis::setLabelsFont : the slot implementation for the label font
 * @param labelsFont
 */
void CustomValueAxis::setLabelsFont(const QFont &labelsFont)
{
    if(m_labelsFont != labelsFont)
    {
        m_labelsFont = labelsFont;
        emit labelsFontChanged();
    }
}

/**
 * @briefCustomValueAxis::labelsColor : returns the label Color
 */
QColor CustomValueAxis::labelsColor() const
{
    return m_labelsColor;
}

/**
 * @briefCustomValueAxis::setLabelsColor : the slot implementation for the label color
 * @param labelsColor
 */
void CustomValueAxis::setLabelsColor(const QColor &labelsColor)
{
    if(m_labelsColor != labelsColor)
    {
        m_labelsColor = labelsColor;
        emit labelsColorChanged();
    }
}

/**
 * @briefCustomValueAxis::labelsColor : returns the label visibility
 */
bool CustomValueAxis::lineVisible() const
{
    return m_lineVisible;
}

/**
 * @briefCustomValueAxis::setLineVisible : the slot implementation for the setting the visibility of the
 * line
 * @param lineVisible
 */
void CustomValueAxis::setLineVisible(bool lineVisible)
{
    if(m_lineVisible != lineVisible)
    {
        m_lineVisible = lineVisible;
        emit lineVisibleChanged();
    }
}

/**
 * @brief CustomValueAxis::titleText : returns the title text
 */
QString CustomValueAxis::titleText() const
{
    return m_titleText;
}

/**
 * @briefCustomValueAxis::setTitleText : the slot implementation for the title Text
 * @param  &titleText
 */
void CustomValueAxis::setTitleText(const QString &titleText)
{
    if(m_titleText != titleText)
    {
        m_titleText = titleText;
        emit titleTextChanged();
    }
}

/**
 * @brief CustomValueAxis::titleVisible : returns the visibility of the title
 */
bool CustomValueAxis::titleVisible() const
{
    return m_titleVisible;
}

/**
 * @briefCustomValueAxis::setTitleVisible : the slot implementation for the visibility of the title Text
 * @param  &titleVisible
 */
void CustomValueAxis::setTitleVisible(bool titleVisible)
{
    if(m_titleVisible != titleVisible)
    {
        m_titleVisible = titleVisible;
        emit titleVisibleChanged();
    }
}

/**
 * @brief CustomValueAxis::decimals : returns the decimals
 */
int CustomValueAxis::decimals() const
{
    return m_decimals;
}

/**
 * @briefCustomValueAxis::setDecimals : the slot implementation for the decimals
 * @param  &decimals
 */
void CustomValueAxis::setDecimals(int decimals)
{
    if(m_decimals != decimals)
    {
        m_decimals = decimals;
        emit decimalsChanged();
    }
}

/**
 * @brief CustomValueAxis::min : returns the min value
 */
double CustomValueAxis::min() const
{
    return m_min;
}

/**
 * @brief CustomValueAxis::setMin : the slot implementation for the setting Min value
 * @param max
 */
void CustomValueAxis::setMin(double min)
{
    if(m_min < min || m_min > min)
    {
        m_min = min;
        emit minChanged();
    }
}

/**
 * @brief CustomValueAxis::max : returns the Max value
 */
double CustomValueAxis::max() const
{
    return m_max;
}

/**
 * @brief CustomValueAxis::setMax : the slot implementation for the setting Max value
 * @param max
 */
void CustomValueAxis::setMax(double max)
{
    if(m_max < max || m_max > max)
    {
        m_max = max;
        emit maxChanged();
    }
}

/**
 * @brief CustomValueAxis::tickCount : returns the tick count
 */
int CustomValueAxis::tickCount() const
{
    return m_tickCount;
}

/**
 * @brief CustomValueAxis::setTickCount : the slot implementation for the setting tick count
 * @param tickCount
 */
void CustomValueAxis::setTickCount(int tickCount)
{
    if(tickCount < 2)
    {
        tickCount = 2;
    }
    if(m_tickCount != tickCount)
    {
        m_tickCount = tickCount;
        emit tickCountChanged();
    }
}


