#include "Header/CustomXYSeries.h"

#include <QVector>

#include <QTime>

/**
 * @brief CustomXYSeries::CustomXYSeries : Constructor, Initialization of class members
 * @param parent
 */
CustomXYSeries::CustomXYSeries(QQuickItem *parent) : QQuickItem(parent)
  , m_width(1)
  , m_color("#ffffff")
  , m_axisX(nullptr)
  , m_axisY(nullptr)
  , m_axisYRight(nullptr)
  , m_visible(true)
  , m_pointsVisible(true)
  , m_iTime(0)
{
    this->setProperty("custom", "CustomXYSeries");
}

/**
 * @brief CustomXYSeries::replace : replace the graphic point
 * @param points
 */
void CustomXYSeries::replace(QList<QPointF> points)
{
    this->setListData(points);
}

/**
 * @brief CustomXYSeries::replace : overload the replace method
 * @param points
 */
void CustomXYSeries::replace(QVector<QPointF> points)
{
    this->setListData(points.toList());
}

/**
 * @brief CustomXYSeries::createData: creates the graph data
 */
void CustomXYSeries::createData()
{
    if(m_iTime != 0)
    {
        this->killTimer(m_iTime);
        m_iTime = 0;
    }
    else
    {
        m_iTime = this->startTimer(100);
    }
}

/**
 * @brief CustomXYSeries::clear: clears the graph data
 */
void CustomXYSeries::clear()
{
  m_listData.clear();
  m_drawPointInfo.listPoint.clear();
}

/**
 * @brief CustomXYSeries::constListData: returns the list of data for the graph
 */
const QList<QPointF> &CustomXYSeries::constListData() const
{
    return m_listData;
}

/**
 * @brief CustomXYSeries::at : get the x and y data at specified location
 * @param index
 */
QPointF CustomXYSeries::at(int index)
{
    if (m_listData.size()>0)
    {
        return m_listData.at(index);
    }
    else
    {
        return QPointF(0,0);
    }
}

/**
 * @brief CustomXYSeries::getTitleInfo: returns the title properties
 */
TagSeriesTitle* CustomXYSeries::getTitleInfo()
{
    return &m_titleInfo;
}

/**
 * @brief CustomXYSeries::getDrawPointInfo: returns the data point properties
 */
TagSeriesPoint* CustomXYSeries::getDrawPointInfo()
{
    return &m_drawPointInfo;
}

/**
 * @brief CustomXYSeries::listData: returns the list of the data
 */
QList<QPointF> CustomXYSeries::listData() const
{
    return m_listData;
}

/**
 * @brief CustomXYSeries::setListData: The slot implementation for the list of the data
 * @param listData
 */
void CustomXYSeries::setListData(const QList<QPointF> &listData)
{
    if(m_listData != listData)
    {
        m_listData = listData;
        emit listDataChanged();
    }
}

/**
 * @brief CustomXYSeries::name: returns the name
 */
QString CustomXYSeries::name() const
{
    return m_name;
}

/**
 * @brief CustomXYSeries::setName: The slot implementation for the name
 * @param &name
 */
void CustomXYSeries::setName(const QString &name)
{
    if(m_name != name)
    {
        m_name = name;
        emit nameChanged();
    }
}

/**
 * @brief CustomXYSeries::name: returns the width
 */
double CustomXYSeries::width() const
{
    return m_width;
}

/**
 * @brief CustomXYSeries::setName: The slot implementation for the width
 * @param &name
 */
void CustomXYSeries::setWidth(double width)
{
    if(m_width < width || m_width > width)
    {
        m_width = width;
        emit mwidthChanged();
    }
}

/**
 * @brief CustomXYSeries::color: returns the color
 */
QColor CustomXYSeries::color() const
{
    return m_color;
}

/**
 * @brief CustomXYSeries::setColor: The slot implementation for the color
 * @param &color
 */
void CustomXYSeries::setColor(const QColor &color)
{
    if(m_color != color)
    {
        m_color = color;
        emit colorChanged();
    }
}

/**
 * @brief CustomValueAxis *CustomXYSeries : return the X axises
 */
CustomValueAxis *CustomXYSeries::axisX() const
{
    return m_axisX;
}

/**
 * @brief CustomXYSeries::setAxisX: The slot implementation for the X axis
 * @param *axisX
 */
void CustomXYSeries::setAxisX(CustomValueAxis *axisX)
{
    if(m_axisX != axisX)
    {
        m_axisX = axisX;
        emit axisXChanged();
    }
}

/**
 * @brief CustomValueAxis *CustomXYSeries : return the Y axises
 */
CustomValueAxis *CustomXYSeries::axisY() const
{
    return m_axisY;
}

/**
 * @brief CustomXYSeries::setAxisY: The slot implementation for the Y axis
 * @param *axisY
 */
void CustomXYSeries::setAxisY(CustomValueAxis *axisY)
{
    if(m_axisY != axisY || !m_axisYIsLeft)
    {
        m_axisY = axisY;
        this->setAxisYIsLeft(true);
        emit axisYChanged();
    }
}

/**
 * @brief CustomValueAxis *CustomXYSeries::axisYRight: returns the axis on the right
 */
CustomValueAxis *CustomXYSeries::axisYRight() const
{
    return m_axisYRight;
}

/**
 * @brief CustomXYSeries::setAxisYRight: The slot implementation for the Y axis
 * @param *axisY
 */
void CustomXYSeries::setAxisYRight(CustomValueAxis *axisYRight)
{
    if(m_axisYRight != axisYRight || m_axisYIsLeft)
    {
        m_axisYRight = axisYRight;
        this->setAxisYIsLeft(false);
        emit axisYRightChanged();
    }
}

/**
 * @brief CustomValueAxis visible : return the visibility
 */
bool CustomXYSeries::visible() const
{
    return m_visible;
}

/**
 * @brief CustomXYSeries::setVisible: The slot implementation for the visiblity
 * @param visible
 */
void CustomXYSeries::setVisible(bool visible)
{
    if(m_visible != visible)
    {
        m_visible = visible;
        emit mvisibleChanged();
    }
}

/**
 * @brief CustomValueAxis pointsVisible : return the point visibility
 */
bool CustomXYSeries::pointsVisible() const
{
    return m_pointsVisible;
}

/**
 * @brief CustomXYSeries::setPointsVisible: The slot implementation for the point visiblity
 * @param pointvisible
 */
void CustomXYSeries::setPointsVisible(bool pointsVisible)
{
    if(m_pointsVisible != pointsVisible)
    {
        m_pointsVisible = pointsVisible;
        emit pointsVisibleChanged();
    }
}

/**
 * @brief CustomValueAxis::axisYIsLeft : return the Y axis on the left
 */
bool CustomXYSeries::axisYIsLeft() const
{
    return m_axisYIsLeft;
}

/**
 * @brief CustomXYSeries::setAxisYIsLeft: The slot implementation for Y axis on the left
 * @param axisYIsLeft
 */
void CustomXYSeries::setAxisYIsLeft(bool axisYIsLeft)
{
    if(m_axisYIsLeft != axisYIsLeft)
    {
        m_axisYIsLeft = axisYIsLeft;
        emit axisYIsLeftChanged();
    }
}


/**
 * @brief CustomXYSeries::timerEvent: The time event when drewing the graph
 * @param *event
 */
void CustomXYSeries::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_iTime)
    {
        QList<QPointF> listData;
        if(m_axisYIsLeft)
        {
            if(m_axisX && m_axisY)
            {
                qsrand(uint(QTime( 0,  0, 0).secsTo(QTime::currentTime())));
                double min = m_axisY->min();
                double tmpValue = m_axisY->max() - min;
                if(tmpValue > 1)
                {
                    for(int i = int(m_axisX->min()); i < m_axisX->max(); ++i)
                    {
                        listData.append(QPointF(i, min + qrand()%(int(tmpValue))));
                    }
                }
            }
        }
        else
        {
            if(m_axisX && m_axisYRight)
            {
                qsrand(uint(QTime( 0,  0, 0).secsTo(QTime::currentTime())));
                double min = m_axisYRight->min();
                double tmpValue = m_axisYRight->max() - min;
                if(tmpValue > 1)
                {
                    for(int i = int(m_axisX->min()); i < m_axisX->max(); ++i)
                    {
                        listData.append(QPointF(i, min + qrand()%(int(tmpValue))));
                    }
                }
            }
        }
        this->setListData(listData);
    }
}


