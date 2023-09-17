#include "Header/CustomChartView.h"

#include "Header/CustomXYSeries.h"

#include <QFontMetrics>
#include <QtConcurrent/QtConcurrent>

/**
 * @brief CustomChartView::CustomChartView : Constructor, Initialization of class members
 * @param parent
 */
CustomChartView::CustomChartView(QQuickItem *parent) : QQuickPaintedItem(parent)
  , m_plotArea(QRectF(0, 0, this->width(), this->height()))
  , m_iLeftMargin(10)
  , m_iTopMargin(90)
  , m_iRightMargin(10)
  , m_iBottomMargin(45)
  , m_iTitleRectWidth(0)
  , m_iAxisSpace(0)
  , m_iTitleSpace(0)
  , m_iTitleRowSpace(0)
  , m_thisSize(QSizeF(this->width(), this->height()))
  , m_scaleWidth(0)
  , m_textScaleSpace(0)
  , m_gridColor("#88cccccc")
{
    m_titleFont.setPixelSize(1);

    connect(this, SIGNAL(childrenChanged()), this, SLOT(slotChildItemsChanged()));
}

/**
 * @brief CustomXYSeries *CustomChartView : Reads the series name
 * @param QString &name
 */
CustomXYSeries *CustomChartView::series(const QString &name)
{
    QList<CustomXYSeries*>::Iterator seriesItr = m_listXYSeries.begin();
    while(seriesItr != m_listXYSeries.end())
    {
        if((*seriesItr)->name() == name)
        {
            return *seriesItr;
        }
        ++seriesItr;
    }
    return nullptr;
}

/**
 * @brief CustomChartView::plotArea : returns the plot area value
 */
QRectF CustomChartView::plotArea() const
{
    return m_plotArea;
}

/**
 * @brief CustomChartView::setplotArea : the plot area slot implementation
 * @param plot area
 */
void CustomChartView::setPlotArea(const QRectF &plotArea)
{
    if(m_plotArea != plotArea)
    {
        m_plotArea = plotArea;
        emit plotAreaChanged();
    }
}

/**
 * @brief CustomChartView::leftMargin : returns the left margin value
 */
int CustomChartView::leftMargin() const
{
    return m_iLeftMargin;
}

/**
 * @brief CustomChartView::setLeftMargin : the left margin slot implementation
 * @param left margin
 */
void CustomChartView::setLeftMargin(int leftMargin)
{
    if(m_iLeftMargin != leftMargin)
    {
        m_iLeftMargin = leftMargin;

        calculatePlotArea();
        calculateAxisInfo();
        calculateSerisData();
        update();

        emit leftMarginChanged();
    }
}

/**
 * @brief CustomChartView::rightMargin : returns the right margin value
 */
int CustomChartView::rightMargin() const
{
    return m_iRightMargin;
}

/**
 * @brief CustomChartView::setRightMargin : the right margin slot implementation
 * @param right margin
 */
void CustomChartView::setRightMargin(int rightMargin)
{
    if(m_iRightMargin != rightMargin)
    {
        m_iRightMargin = rightMargin;

        calculatePlotArea();
        calculateAxisInfo();
        calculateSerisData();
        update();

        emit rightMarginChanged();
    }
}

/**
 * @brief CustomChartView::TopMargin : the top margin slot implementation
 */
int CustomChartView::topMargin() const
{
    return m_iTopMargin;
}

/**
 * @brief CustomChartView::setTopMargin : the top margin slot implementation
 * @param top margin
 */
void CustomChartView::setTopMargin(int topMargin)
{
    if(m_iTopMargin != topMargin)
    {
        m_iTopMargin = topMargin;

        calculatePlotArea();
        calculateAxisInfo();
        calculateSerisData();
        update();

        emit topMarginChanged();
    }
}

/**
 * @brief CustomChartView::bottomMargin : returns the bottom margin value
 */
int CustomChartView::bottomMargin() const
{
    return m_iBottomMargin;
}

/**
 * @brief CustomChartView::setbottomMargin : the bottom margin slot implementation
 * @param bottom margin
 */
void CustomChartView::setBottomMargin(int bottomMargin)
{
    if(m_iBottomMargin != bottomMargin)
    {
        m_iBottomMargin = bottomMargin;

        calculatePlotArea();
        calculateAxisInfo();
        calculateSerisData();
        update();

        emit bottomMarginChanged();
    }
}

/**
 * @brief CustomChartView::bottomMargin : returns the title label size value
 */
int CustomChartView::titleRectWidth()
{
    return m_iTitleRectWidth;
}

/**
 * @brief CustomChartView::setTitleRectWidth : the title name label slot implementation
 * @param  titleRectWidth
 */
void CustomChartView::setTitleRectWidth(int titleRectWidth)
{
    if(m_iTitleRectWidth != titleRectWidth)
    {
        m_iTitleRectWidth = titleRectWidth;
        if(m_listSeriesTitle.count() > 0)
        {
            updateSerisTitleData();
            calculatePlotArea();
            updateYAxisInfo();
            updateYRightAxisInfo();
            calculateSerisData();
            update();
        }
        emit titleRectWidthChanged();
    }
}

/**
 * @brief CustomChartView::TitleFont : returns the title font
 */
QFont CustomChartView::titleFont()
{
    return m_titleFont;
}

/**
 * @brief CustomChartView::setTitleFont : the title font slot implementation
 * @param  titleFont
 */
void CustomChartView::setTitleFont(QFont titleFont)
{
    if(m_titleFont != titleFont)
    {
        m_titleFont = titleFont;

        updateSerisTitleData();
        if(m_listSeriesTitle.count() > 0)
        {
            updateSerisTitleData();
            calculatePlotArea();
            updateYAxisInfo();
            updateYRightAxisInfo();
            calculateSerisData();
            update();
        }

        emit titleFontChanged();
    }
}

/**
 * @brief CustomChartView::TitleSpace : return space value between each title
 */
int CustomChartView::titleSpace()
{
    return m_iTitleSpace;
}

/**
 * @brief CustomChartView::setTitleSpace : title space slot implementation
 * @param  titleSpace
 */
void CustomChartView::setTitleSpace(int titleSpace)
{
    if(m_iTitleSpace != titleSpace)
    {
        m_iTitleSpace = titleSpace;

        updateSerisTitleData();
        if(m_listSeriesTitle.count() != 0
                && m_listSeriesTitle.last()->row > 0)
        {
            updateSerisTitleData();
            calculatePlotArea();
            updateYAxisInfo();
            updateYRightAxisInfo();
            calculateSerisData();
        }
        if(m_listSeriesTitle.count() > 0)
        {
            update();
        }

        emit titleSpaceChanged();
    }
}

/**
 * @brief CustomChartView::TitleRowSpace : returns the title row space
 */
int CustomChartView::titleRowSpace()
{
    return m_iTitleRowSpace;
}

/**
 * @brief CustomChartView::setTitleRowSpace : title row space slot implementation
 * @param  titleRowSpace
 */
void CustomChartView::setTitleRowSpace(int titleRowSpace)
{
    if(m_iTitleRowSpace != titleRowSpace)
    {
        m_iTitleRowSpace = titleRowSpace;

        if(m_listSeriesTitle.count() != 0
                && m_listSeriesTitle.last()->row > 0)
        {
            updateSerisTitleData();
            calculatePlotArea();
            updateYAxisInfo();
            updateYRightAxisInfo();
            calculateSerisData();
            update();
        }

        emit titleRowSpaceChanged();
    }
}

/**
 * @brief CustomChartView::axisSpace : returns the Axis space value
 */
int CustomChartView::axisSpace()
{
    return m_iAxisSpace;
}

/**
 * @brief CustomChartView::setAxisSpace : the Axis space slot implementation
 * @param  AxisSpace
 */
void CustomChartView::setAxisSpace(int axisSpace)
{
    if(m_iAxisSpace != axisSpace)
    {
        m_iAxisSpace = axisSpace;

        updateAxisInfo();
        calculatePlotArea();
        calculateAxisInfo();
        calculateSerisData();
        update();

        emit axisSpaceChanged();
    }
}

/**
 * @brief CustomChartView::ScaleWidth : returns the width scale
 */
double CustomChartView::scaleWidth()
{
    return m_scaleWidth;
}

/**
 * @brief CustomChartView::setScaleWidth : the width scale slot implementation
 * @param  scaleWidth
 */
void CustomChartView::setScaleWidth(double scaleWidth)
{
    if(m_scaleWidth < scaleWidth || m_scaleWidth > scaleWidth)
    {
        m_scaleWidth = scaleWidth;

        updateAxisInfo();
        calculatePlotArea();
        calculateAxisInfo();
        calculateSerisData();
        update();

        emit scaleWidthChanged();
    }
}

/**
 * @brief CustomChartView::TextScaleSpace : returns Text scale space
 */
int CustomChartView::textScaleSpace()
{
    return m_textScaleSpace;
}

/**
 * @brief CustomChartView::setTextScaleSpace : the Text scale space slot implementation
 * @param  textScaleSpace
 */
void CustomChartView::setTextScaleSpace(int textScaleSpace)
{
    if(m_textScaleSpace != textScaleSpace)
    {
        m_textScaleSpace = textScaleSpace;

        updateAxisInfo();
        calculatePlotArea();
        calculateAxisInfo();
        calculateSerisData();
        update();

        emit textScaleSpaceChanged();
    }
}

/**
 * @brief CustomChartView::GridColor : returns the grid color
 */
QColor CustomChartView::gridColor()
{
    return m_gridColor;
}

/**
 * @brief CustomChartView::setGridColor : the grid color slot implementation
 * @param  gridColor
 */
void CustomChartView::setGridColor(QColor gridColor)
{
    if(m_gridColor != gridColor)
    {
        m_gridColor = gridColor;
        update();

        emit gridColorChanged();
    }
}

/**
 * @brief CustomChartView::paint : paints the graph
 * @param  *painter
 */
void CustomChartView::paint(QPainter *painter)
{
    if(m_thisSize != QSizeF(this->width(), this->height()))
    {
        m_thisSize = QSizeF(this->width(), this->height());
        updatePaintData();
        return;
    }

    QPen pen = painter->pen();
    double plotAreaEndX = m_plotArea.x() + m_plotArea.width();
    double plotAreaEndY = m_plotArea.y() + m_plotArea.height();
    bool   drawFirstXAxis = false;
    QColor firstXAxisColor;

    //draw title
    painter->save();
    painter->setClipRect(QRectF(m_plotArea.x(), m_iTopMargin, m_plotArea.width(), m_plotArea.y() - m_iTopMargin - m_iTitleRowSpace));
    painter->setFont(m_titleFont);
    QList<TagSeriesTitle*>::Iterator seriesTitleItr = m_listSeriesTitle.begin();
    if(m_iTitleRow == 0)
    {
        double drawX = m_plotArea.x() + (m_plotArea.width() - m_dTitleWidth)/2;
        while(seriesTitleItr != m_listSeriesTitle.end())
        {
            pen.setColor((*seriesTitleItr)->color);
            painter->setPen(pen);
            painter->setBrush(QBrush((*seriesTitleItr)->color));
            painter->drawRect(QRectF(drawX, m_iTopMargin + ((*seriesTitleItr)->height - m_iTitleRectWidth)/2,
                                     m_iTitleRectWidth, m_iTitleRectWidth));
            painter->drawText(QRectF(drawX, m_iTopMargin, (*seriesTitleItr)->width, (*seriesTitleItr)->height),
                              (*seriesTitleItr)->text, QTextOption(Qt::AlignRight|Qt::AlignVCenter));
            drawX += (*seriesTitleItr)->width + m_iTitleSpace;
            ++seriesTitleItr;
        }
    }
    else
    {
        double drawX = m_plotArea.x();
        double drawY = m_iTopMargin;
        int currentRow = 0;
        double preViousHeight = 0;
        while(seriesTitleItr != m_listSeriesTitle.end())
        {
            pen.setColor((*seriesTitleItr)->color);
            painter->setPen(pen);
            painter->setBrush(QBrush((*seriesTitleItr)->color));
            if((*seriesTitleItr)->row != currentRow)
            {
                currentRow = (*seriesTitleItr)->row;
                drawX = m_plotArea.x();
                drawY += preViousHeight + m_iTitleRowSpace;
            }
            preViousHeight = (*seriesTitleItr)->height;
            painter->drawRect(QRectF(drawX, drawY + ((*seriesTitleItr)->height - m_iTitleRectWidth)/2, m_iTitleRectWidth, m_iTitleRectWidth));
            painter->drawText(QRectF(drawX, drawY, (*seriesTitleItr)->width, (*seriesTitleItr)->height),
                              (*seriesTitleItr)->text, QTextOption(Qt::AlignRight|Qt::AlignVCenter));
            drawX += (*seriesTitleItr)->width + m_iTitleSpace;
            ++seriesTitleItr;
        }
    }
    painter->restore();

    //draw X axis
    double xAxisY = plotAreaEndY;
    QList<TagAxisInfo*>::Iterator xAxisItr = m_listXAxisInfo.begin();
    while(xAxisItr != m_listXAxisInfo.end())
    {
        if((*xAxisItr)->gridVisible)
        {
            pen.setColor(m_gridColor);
            painter->setPen(pen);
            QList<double>::Iterator scaleItr = (*xAxisItr)->listScalePos.begin();
            while(scaleItr != (*xAxisItr)->listScalePos.end())
            {
                painter->drawLine(QPointF(*scaleItr, m_plotArea.y()), QPointF(*scaleItr, plotAreaEndY));
                ++scaleItr;
            }
        }
        if((*xAxisItr)->visible)
        {
            double scaleEndY = xAxisY + m_scaleWidth;
            pen.setColor((*xAxisItr)->color);
            painter->setPen(pen);
            painter->drawLine(QPointF(m_plotArea.x(), xAxisY), QPointF(plotAreaEndX, xAxisY));
            QList<double>::Iterator scaleItr = (*xAxisItr)->listScalePos.begin();
            while(scaleItr != (*xAxisItr)->listScalePos.end())
            {
                painter->drawLine(QPointF(*scaleItr, xAxisY), QPointF(*scaleItr, scaleEndY));
                ++scaleItr;
            }
            if(xAxisItr == m_listXAxisInfo.begin())
            {
                drawFirstXAxis = true;
                firstXAxisColor = (*xAxisItr)->color;
            }
        }
        double textY = xAxisY + m_scaleWidth + m_textScaleSpace;
        if((*xAxisItr)->labelsVisible)
        {
            painter->setFont((*xAxisItr)->font);
            pen.setColor((*xAxisItr)->labelColor);
            painter->setPen(pen);
            int index = 0;
            QList<QString>::Iterator textItr = (*xAxisItr)->listScaleValue.begin();
            while(textItr != (*xAxisItr)->listScaleValue.end())
            {
                painter->drawText(QRectF((*xAxisItr)->listScalePos[index] - (*xAxisItr)->textMaxWidth/2,
                                         textY, (*xAxisItr)->textMaxWidth, (*xAxisItr)->textHeight),
                                  *textItr, QTextOption(Qt::AlignHCenter|Qt::AlignTop));
                ++index;
                ++textItr;
            }
        }
        if((*xAxisItr)->titleVisible)
        {
            painter->drawText(QRectF(m_plotArea.x(), textY + (*xAxisItr)->textHeight + m_textScaleSpace,
                                     m_plotArea.width(), (*xAxisItr)->textHeight),
                              (*xAxisItr)->titleText, QTextOption(Qt::AlignHCenter|Qt::AlignVCenter));
        }

        xAxisY += (*xAxisItr)->widthOrHeight + m_iAxisSpace;
        ++xAxisItr;
    }

    //draw Y axis
    double yAxisX = m_plotArea.x();
    QList<TagAxisInfo*>::Iterator yAxisItr = m_listYAxisInfo.begin();
    while(yAxisItr != m_listYAxisInfo.end())
    {
        if((*yAxisItr)->gridVisible)
        {
            pen.setColor(m_gridColor);
            painter->setPen(pen);
            QList<double>::Iterator scaleItr = (*yAxisItr)->listScalePos.begin();
            while(scaleItr != (*yAxisItr)->listScalePos.end())
            {
                painter->drawLine(QPointF(m_plotArea.x(), *scaleItr), QPointF(plotAreaEndX, *scaleItr));
                ++scaleItr;
            }
        }
        if((*yAxisItr)->visible)
        {
            double scaleEndX = yAxisX - m_scaleWidth;
            pen.setColor((*yAxisItr)->color);
            painter->setPen(pen);
            painter->drawLine(QPointF(yAxisX, m_plotArea.y()), QPointF(yAxisX, plotAreaEndY));
            QList<double>::Iterator scaleItr = (*yAxisItr)->listScalePos.begin();
            while(scaleItr != (*yAxisItr)->listScalePos.end())
            {
                painter->drawLine(QPointF(yAxisX, *scaleItr), QPointF(scaleEndX, *scaleItr));
                ++scaleItr;
            }
        }
        double textX = yAxisX - m_scaleWidth - m_textScaleSpace;
        if((*yAxisItr)->labelsVisible)
        {
            painter->setFont((*yAxisItr)->font);
            pen.setColor((*yAxisItr)->labelColor);
            painter->setPen(pen);
            int index = 0;
            QList<QString>::Iterator textItr = (*yAxisItr)->listScaleValue.begin();
            while(textItr != (*yAxisItr)->listScaleValue.end())
            {
                painter->drawText(QRectF(textX - (*yAxisItr)->textMaxWidth, (*yAxisItr)->listScalePos[index] - (*yAxisItr)->textHeight/2,
                                         (*yAxisItr)->textMaxWidth, (*yAxisItr)->textHeight),
                                  *textItr, QTextOption(Qt::AlignRight|Qt::AlignVCenter));
                ++index;
                ++textItr;
            }
        }
        if((*yAxisItr)->titleVisible)
        {
            painter->save();
            double titleTextWidth = painter->fontMetrics().width((*yAxisItr)->titleText);
            double titleX = textX - (*yAxisItr)->textMaxWidth - m_textScaleSpace;
            double tiyleY = m_plotArea.y() + (m_plotArea.height() - titleTextWidth)/2;
            painter->translate(titleX, tiyleY);
            painter->rotate(270);
            painter->drawText(QPointF(-titleTextWidth, 0), (*yAxisItr)->titleText);
            painter->restore();
        }

        yAxisX -= (*yAxisItr)->widthOrHeight + m_iAxisSpace;
        ++yAxisItr;
    }

    //draw right y axis
    double yRightAxisX = plotAreaEndX;
    QList<TagAxisInfo*>::Iterator yRightAxisItr = m_listYRightAxisInfo.begin();
    while(yRightAxisItr != m_listYRightAxisInfo.end())
    {
        if((*yRightAxisItr)->gridVisible)
        {
            pen.setColor(m_gridColor);
            painter->setPen(pen);
            QList<double>::Iterator scaleItr = (*yRightAxisItr)->listScalePos.begin();
            while(scaleItr != (*yRightAxisItr)->listScalePos.end())
            {
                painter->drawLine(QPointF(m_plotArea.x(), *scaleItr), QPointF(plotAreaEndX, *scaleItr));
                ++scaleItr;
            }
        }
        if((*yRightAxisItr)->visible)
        {
            double scaleEndX = yRightAxisX + m_scaleWidth;
            pen.setColor((*yRightAxisItr)->color);
            painter->setPen(pen);
            painter->drawLine(QPointF(yRightAxisX, m_plotArea.y()), QPointF(yRightAxisX, plotAreaEndY));
            QList<double>::Iterator scaleItr = (*yRightAxisItr)->listScalePos.begin();
            while(scaleItr != (*yRightAxisItr)->listScalePos.end())
            {
                painter->drawLine(QPointF(yRightAxisX, *scaleItr), QPointF(scaleEndX, *scaleItr));
                ++scaleItr;
            }
        }
        double textX = yRightAxisX + m_scaleWidth + m_textScaleSpace;
        if((*yRightAxisItr)->labelsVisible)
        {
            painter->setFont((*yRightAxisItr)->font);
            pen.setColor((*yRightAxisItr)->labelColor);
            painter->setPen(pen);
            int index = 0;
            QList<QString>::Iterator textItr = (*yRightAxisItr)->listScaleValue.begin();
            while(textItr != (*yRightAxisItr)->listScaleValue.end())
            {
                painter->drawText(QRectF(textX, (*yRightAxisItr)->listScalePos[index] - (*yRightAxisItr)->textHeight/2,
                                         (*yRightAxisItr)->textMaxWidth, (*yRightAxisItr)->textHeight),
                                  *textItr, QTextOption(Qt::AlignRight|Qt::AlignVCenter));
                ++index;
                ++textItr;
            }
        }
        if((*yRightAxisItr)->titleVisible)
        {
            painter->save();
            double titleTextWidth = painter->fontMetrics().width((*yRightAxisItr)->titleText);
            double titleX = textX + (*yRightAxisItr)->textMaxWidth + m_textScaleSpace;
            double tiyleY = m_plotArea.y() + (m_plotArea.height() - titleTextWidth)/2;
            painter->translate(titleX, tiyleY);
            painter->rotate(90);
            painter->drawText(QPointF(0, 0), (*yRightAxisItr)->titleText);
            painter->restore();
        }

        yRightAxisX += (*yRightAxisItr)->widthOrHeight + m_iAxisSpace;
        ++yRightAxisItr;
    }

    //draw first x axis
    if(drawFirstXAxis)
    {
        pen.setColor(firstXAxisColor);
        painter->setPen(pen);
        painter->drawLine(QPointF(m_plotArea.x(), plotAreaEndY), QPointF(plotAreaEndX, plotAreaEndY));
    }

    //draw data
    painter->save();
    painter->setClipRect(m_plotArea);
    QList<TagSeriesPoint*>::Iterator seriesPointItr = m_listSeriesData.begin();
    while(seriesPointItr != m_listSeriesData.end())
    {
        if((*seriesPointItr)->visible)
        {
            pen.setColor((*seriesPointItr)->color);
            pen.setWidthF((*seriesPointItr)->width);
            painter->setPen(pen);
            painter->setRenderHint(QPainter::Antialiasing, true);
            QList<QPointF>::Iterator preViousPointItr = (*seriesPointItr)->listPoint.begin();
            QList<QPointF>::Iterator pointItr = (*seriesPointItr)->listPoint.begin();
            if(pointItr != (*seriesPointItr)->listPoint.end())
            {
                ++pointItr;
                while(pointItr != (*seriesPointItr)->listPoint.end())
                {
                    painter->drawLine(*preViousPointItr, *pointItr);

                    ++preViousPointItr;
                    ++pointItr;
                }
            }
        }

        ++seriesPointItr;
    }
    painter->restore();
}

/**
 * @brief CustomChartView::lanuchPaint : updates the graph
 */
void CustomChartView::lanuchPaint()
{
    update();
}

/**
 * @brief CustomChartView::slotChildItemsChanged : The slot implementation for XYSeries
 */
void CustomChartView::slotChildItemsChanged()
{
    QList<CustomXYSeries*> tmpListSeries;
    bool hasNewSeries = false;
    QList<QQuickItem *> listChild = childItems();
    while(listChild.count() > 0)
    {
       CustomXYSeries* pCustomXYSeries = static_cast<CustomXYSeries*>(listChild.takeFirst());
        //CustomXYSeries* pCustomXYSeries ;
        if(pCustomXYSeries && pCustomXYSeries->property("custom").toString() == "CustomXYSeries")
        {
            tmpListSeries.append(pCustomXYSeries);
            if(!m_listXYSeries.contains(pCustomXYSeries))
            {
                connectBySeries(pCustomXYSeries);
                hasNewSeries = true;
            }
        }
    }

    if(hasNewSeries)
    {
        m_listXYSeries.clear();
        m_listXYSeries = tmpListSeries;
        updatePaintData();
    }
}

/**
 * @brief CustomChartView::slotSeriesDataChanged : The slot implementation for Series data
 */
void CustomChartView::slotSeriesDataChanged()
{
    CustomXYSeries *pXYSeries = static_cast<CustomXYSeries*>(sender());
    TagSeriesPoint *pSeriesPoint = pXYSeries->getDrawPointInfo();
    updateSerisData(*pXYSeries, *pSeriesPoint);
    if(pXYSeries->visible() && pSeriesPoint->visible)
    {
      update();

      // QtConcurrent::run(this, &CustomChartView::lanuchPaint);

    }
}

/**
 * @brief CustomChartView::slotSeriesNameChanged : The slot implementation for Series name
 */
void CustomChartView::slotSeriesNameChanged()
{
    CustomXYSeries *pXYSeries = static_cast<CustomXYSeries*>(sender());
    if(pXYSeries->visible())
    {
        updateSerisTitleData();

        double plotAreaTopMargin;
        updatePlotAreaTop(plotAreaTopMargin, m_plotArea.width());
        if(plotAreaTopMargin < m_plotArea.y() || plotAreaTopMargin > m_plotArea.y())
        {
            calculatePlotArea();

            updateYAxisInfo();
            updateYRightAxisInfo();

            calculateSerisData();
        }
        update();
    }
    else
    {
        serisToSerisTitle(*pXYSeries, *(pXYSeries)->getTitleInfo());
    }
}

/**
 * @brief CustomChartView::slotSeriesWidthChanged : The slot implementation for Series width
 */
void CustomChartView::slotSeriesWidthChanged()
{
    CustomXYSeries *pXYSeries = static_cast<CustomXYSeries*>(sender());
    TagSeriesPoint *periesPoint = pXYSeries->getDrawPointInfo();
    periesPoint->width = pXYSeries->width();
    if(periesPoint->visible)
    {
        update();
    }
}

/**
 * @brief CustomChartView::slotSeriesColorChanged : The slot implementation for Series color
 */
void CustomChartView::slotSeriesColorChanged()
{
    CustomXYSeries *pXYSeries = static_cast<CustomXYSeries*>(sender());
    TagSeriesPoint *periesPoint = pXYSeries->getDrawPointInfo();
    pXYSeries->getTitleInfo()->color = pXYSeries->color();
    periesPoint->color = pXYSeries->color();
    if(pXYSeries->visible())
    {
        update();
    }
}

/**
 * @brief CustomChartView::slotSeriesAxisXChanged : The slot implementation for Series X axis
 */
void CustomChartView::slotSeriesAxisXChanged()
{
    CustomXYSeries *pXYSeries = static_cast<CustomXYSeries*>(sender());
    if(pXYSeries->visible())
    {
        updatePaintData();
    }
}

/**
 * @brief CustomChartView::slotSeriesAxisYChanged : The slot implementation for Series Y axis
 */
void CustomChartView::slotSeriesAxisYChanged()
{
    CustomXYSeries *pXYSeries = static_cast<CustomXYSeries*>(sender());
    if(pXYSeries->visible())
    {
        updatePaintData();
    }
}

/**
 * @brief CustomChartView::slotSeriesAxisYChanged : The slot implementation for Series Y axis at
 * right side
 */
void CustomChartView::slotSeriesAxisYRightChanged()
{
    CustomXYSeries *pXYSeries = static_cast<CustomXYSeries*>(sender());
    if(pXYSeries->visible())
    {
        updatePaintData();
    }
}

void CustomChartView::slotSeriesVisibleChanged()
{
    updatePaintData();
}

/**
 * @brief CustomChartView::slotSeriesAxisYChanged : The slot implementation for Series Y axis at
 * right side
 */
void CustomChartView::slotSeriesPointsVisibleChanged()
{
    CustomXYSeries *pXYSeries = static_cast<CustomXYSeries*>(sender());
    TagSeriesPoint *pSeriesPoint = pXYSeries->getDrawPointInfo();
    if(pXYSeries->visible())
    {
        pSeriesPoint->visible = pXYSeries->pointsVisible();
        update();
    }
}

/**
 * @brief CustomChartView::slotAxisColorChanged : The slot implementation for the Axis color
 */
void CustomChartView::slotAxisColorChanged()
{
    CustomValueAxis *pAxis = static_cast<CustomValueAxis*>(sender());
    TagAxisInfo *pAxisInfo = pAxis->getAxisInfo();
    pAxisInfo->color = pAxis->color();
    if(pAxisInfo->visible)
    {
        update();
    }
}

/**
 * @brief CustomChartView::slotAxisGridVisibleChanged : The slot implementation for the grid
 * visible
 */
void CustomChartView::slotAxisGridVisibleChanged()
{
    CustomValueAxis *pAxis = static_cast<CustomValueAxis*>(sender());
    TagAxisInfo *pAxisInfo = pAxis->getAxisInfo();
    pAxisInfo->gridVisible = pAxis->gridVisible();
    update();
}

/**
 * @brief CustomChartView:slotAxisLabelsVisibleChanged : The slot implementation for the Axis
 * label visible
 */
void CustomChartView::slotAxisLabelsVisibleChanged()
{
    CustomValueAxis *pAxis = static_cast<CustomValueAxis*>(sender());
    TagAxisInfo *pAxisInfo = pAxis->getAxisInfo();
    pAxisInfo->labelsVisible = pAxis->labelsVisible();
    update();
}

/**
 * @brief CustomChartView:slotAxisLabelsFontChanged : The slot implementation for the Axis
 * label font
 */
void CustomChartView::slotAxisLabelsFontChanged()
{
    CustomValueAxis *pAxis = static_cast<CustomValueAxis*>(sender());
    TagAxisInfo *pAxisInfo = pAxis->getAxisInfo();

    pAxisInfo->font = pAxis->labelsFont();
    QFontMetrics fontMetrics(pAxisInfo->font);
    int startValueWidth = fontMetrics.width(QString::number(pAxisInfo->startValue, 'f', pAxisInfo->decimals));
    int endValueWidth = fontMetrics.width(QString::number(pAxisInfo->endValue, 'f', pAxisInfo->decimals));
    pAxisInfo->textMaxWidth = startValueWidth > endValueWidth ? startValueWidth : endValueWidth;
    pAxisInfo->textHeight = fontMetrics.height();
    if(pAxisInfo->axisType == TagAxisInfo::EAT_X_AXIS)
    {
        pAxisInfo->widthOrHeight = pAxisInfo->textHeight*2 + m_scaleWidth + m_textScaleSpace*2;

        calculatePlotArea();

        updateYAxisInfo();
        updateYRightAxisInfo();

        calculateSerisData();
    }
    else
    {
        pAxisInfo->widthOrHeight = pAxisInfo->textHeight + m_textScaleSpace*2 + pAxisInfo->textMaxWidth + m_scaleWidth;
        calculatePlotArea();
        updateXAxisInfo();
        if(pAxisInfo->axisType == TagAxisInfo::EAT_Y_LEFT_AXIS)
        {
            updateYAxisInfo();
        }
        else
        {
            updateYRightAxisInfo();
        }
        calculateSerisData();
    }
    update();
}

/**
 * @brief CustomChartView:slotAxisLabelsColorChanged : The slot implementation for the Axis
 * label color
 */
void CustomChartView::slotAxisLabelsColorChanged()
{
    CustomValueAxis *pAxis = static_cast<CustomValueAxis*>(sender());
    TagAxisInfo *pAxisInfo = pAxis->getAxisInfo();
    pAxisInfo->labelColor = pAxis->labelsColor();
    update();
}

/**
 * @brief CustomChartView:slotAxisLineVisibleChanged : The slot implementation for setting the Axis
 * label visible
 */
void CustomChartView::slotAxisLineVisibleChanged()
{
    CustomValueAxis *pAxis = static_cast<CustomValueAxis*>(sender());
    TagAxisInfo *pAxisInfo = pAxis->getAxisInfo();
    pAxisInfo->visible = pAxis->lineVisible();
    update();
}

/**
 * @brief CustomChartView:slotAxisTitleTextChanged : The slot implementation for the Axis
 * title text
 */
void CustomChartView::slotAxisTitleTextChanged()
{
    CustomValueAxis *pAxis = static_cast<CustomValueAxis*>(sender());
    TagAxisInfo *pAxisInfo = pAxis->getAxisInfo();
    pAxisInfo->titleText = pAxis->titleText();
    update();
}

/**
 * @brief CustomChartView:slotAxisTitleVisibleChanged : The slot implementation for setting Axis
 * title text visible
 */
void CustomChartView::slotAxisTitleVisibleChanged()
{
    CustomValueAxis *pAxis = static_cast<CustomValueAxis*>(sender());
    TagAxisInfo *pAxisInfo = pAxis->getAxisInfo();
    pAxisInfo->titleVisible = pAxis->titleVisible();
    update();
}

/**
 * @brief CustomChartView:slotAxisDecimalsChanged : The slot implementation for setting Axis
 * decimals
 */
void CustomChartView::slotAxisDecimalsChanged()
{
    CustomValueAxis *pAxis = static_cast<CustomValueAxis*>(sender());
    TagAxisInfo *pAxisInfo = pAxis->getAxisInfo();

    axisToAxisInfo(*pAxis, *pAxisInfo, pAxisInfo->axisType);

    if(pAxisInfo->axisType != TagAxisInfo::EAT_X_AXIS)
    {
        calculatePlotArea();
        updateXAxisInfo();
        calculateSerisData();
    }
    update();
}

/**
 * @brief CustomChartView:slotAxisMinChanged : The slot implementation for Axis Min
 *
 */
void CustomChartView::slotAxisMinChanged()
{
    slotAxisMaxChanged();
}

/**
 * @brief CustomChartView:slotAxisMaxChanged : The slot implementation for Axis Max
 *
 */
void CustomChartView::slotAxisMaxChanged()
{
    CustomValueAxis *pAxis = static_cast<CustomValueAxis*>(sender());
    TagAxisInfo *pAxisInfo = pAxis->getAxisInfo();

    if(pAxis->max() > pAxis->min())
    {
        pAxisInfo->startValue = pAxis->min();
        pAxisInfo->endValue = pAxis->max();
    }
    else
    {
        pAxisInfo->startValue = 0;
        pAxisInfo->endValue = 1;
    }

    pAxisInfo->listScaleValue.clear();
    pAxisInfo->scaleValue = (pAxisInfo->endValue - pAxisInfo->startValue)/(pAxisInfo->tickCount-1);
    double tmpValue = pAxisInfo->startValue;
    for(int i = 0; i < pAxisInfo->tickCount - 1; ++i)
    {
        pAxisInfo->listScaleValue.append(QString::number(tmpValue, 'f', pAxisInfo->decimals));
        tmpValue += pAxisInfo->scaleValue;
    }
    pAxisInfo->listScaleValue.append(QString::number(pAxisInfo->endValue, 'f', pAxisInfo->decimals));

    calculateAxisInfo();
//    calculateSerisData();
//    update();
}

/**
 * @brief CustomChartView:slotAxisTickCountChanged : The slot implementation for tick count
 *
 */
void CustomChartView::slotAxisTickCountChanged()
{
    CustomValueAxis *pAxis = static_cast<CustomValueAxis*>(sender());
    TagAxisInfo *pAxisInfo = pAxis->getAxisInfo();
    pAxisInfo->tickCount = pAxis->tickCount();
    pAxisInfo->listScalePos.clear();
    pAxisInfo->listScaleValue.clear();

    if(pAxisInfo->axisType == TagAxisInfo::EAT_X_AXIS)
    {
        double scaleInterval = m_plotArea.width()/(pAxisInfo->tickCount-1);
        double scaleX = m_plotArea.x();
        for(int i = 0; i < pAxisInfo->tickCount - 1; ++i)
        {
            pAxisInfo->listScalePos.append(scaleX);
            scaleX += scaleInterval;
        }
        pAxisInfo->listScalePos.append(m_plotArea.x() + m_plotArea.width());
    }
    else
    {
        double scaleInterval = m_plotArea.height()/(pAxisInfo->tickCount - 1);
        double scaleY = m_plotArea.y() + m_plotArea.height();
        for(int i = 0; i < pAxisInfo->tickCount - 1; ++i)
        {
            pAxisInfo->listScalePos.append(scaleY);
            scaleY -= scaleInterval;
        }
        pAxisInfo->listScalePos.append(m_plotArea.y());
    }

    pAxisInfo->scaleValue = (pAxisInfo->endValue - pAxisInfo->startValue)/(pAxisInfo->tickCount-1);
    double tmpValue = pAxisInfo->startValue;
    for(int i = 0; i < pAxisInfo->tickCount - 1; ++i)
    {
        pAxisInfo->listScaleValue.append(QString::number(tmpValue, 'f', pAxisInfo->decimals));
        tmpValue += pAxisInfo->scaleValue;
    }
    pAxisInfo->listScaleValue.append(QString::number(pAxisInfo->endValue, 'f', pAxisInfo->decimals));

    update();
}

/**
 * @brief CustomChartView:updatePaintData : updates the graphic data
 *
 */
void CustomChartView::updatePaintData()
{
    updateAxis();

    updateSerisTitleData();

    updateAxisInfo();

    calculatePlotArea();

    calculateAxisInfo();

    calculateSerisData();

    update();
}

/**
 * @brief CustomChartView:updateAxis :  updates Axis
 *
 */
void CustomChartView::updateAxis()
{
    m_listXAxis.clear();
    m_listYAxis.clear();
    m_listYRightAxis.clear();
    this->disconnectByAxis();

    QList<CustomXYSeries*>::Iterator itr = m_listXYSeries.begin();
    while(itr != m_listXYSeries.end())
    {
        CustomValueAxis* xAxis = (*itr)->axisX();
        if(xAxis && !m_listXAxis.contains(xAxis) && (*itr)->visible())
        {
            m_listXAxis.append(xAxis);
            connectByAxis(xAxis);
        }
        CustomValueAxis* yAxis = (*itr)->axisY();
        CustomValueAxis* yRightAxis = (*itr)->axisYRight();
        CustomValueAxis* seriesYAxis = nullptr;
        if((*itr)->axisYIsLeft() && yAxis && !m_listYAxis.contains(yAxis) && (*itr)->visible())
        {
            m_listYAxis.append(yAxis);
            seriesYAxis = yAxis;
        }
        else if(!((*itr)->axisYIsLeft()) && yRightAxis && !m_listYRightAxis.contains(yRightAxis) && (*itr)->visible())
        {
            m_listYRightAxis.append(yRightAxis);
            seriesYAxis = yRightAxis;
        }
        if(seriesYAxis != nullptr)
        {
            connectByAxis(seriesYAxis);
        }

        ++itr;
    }
}

/**
 * @brief CustomChartView:updateAxisInfo :  updates Axis Info
 *
 */
void CustomChartView::updateAxisInfo()
{
    m_listXAxisInfo.clear();
    m_listYAxisInfo.clear();
    m_listYRightAxisInfo.clear();

    QList<CustomValueAxis*>::Iterator xAxisItr = m_listXAxis.begin();
    while(xAxisItr != m_listXAxis.end())
    {
        TagAxisInfo *pAxisInfo = (*xAxisItr)->getAxisInfo();
        axisToAxisInfo(*(*xAxisItr), *pAxisInfo, TagAxisInfo::EAT_X_AXIS);

        m_listXAxisInfo.append(pAxisInfo);
        ++xAxisItr;
    }

    QList<CustomValueAxis*>::Iterator yAxisItr = m_listYAxis.begin();
    while(yAxisItr != m_listYAxis.end())
    {
        TagAxisInfo *pAxisInfo = (*yAxisItr)->getAxisInfo();
        axisToAxisInfo(*(*yAxisItr), *pAxisInfo, TagAxisInfo::EAT_Y_LEFT_AXIS);

        m_listYAxisInfo.append(pAxisInfo);
        ++yAxisItr;
    }

    QList<CustomValueAxis*>::Iterator yRightAxisItr = m_listYRightAxis.begin();
    while(yRightAxisItr != m_listYRightAxis.end())
    {
        TagAxisInfo *pAxisInfo = (*yRightAxisItr)->getAxisInfo();
        axisToAxisInfo(*(*yRightAxisItr), *pAxisInfo, TagAxisInfo::EAT_Y_RIGHT_AXIS);

        m_listYRightAxisInfo.append(pAxisInfo);
        ++yRightAxisItr;
    }
}

/**
 * @brief CustomChartView:axisToAxisInfo :  updates Axis properties
 *
 */
void CustomChartView::axisToAxisInfo(const CustomValueAxis &valueAxis, TagAxisInfo &axisInfo, int axisType)
{
    axisInfo.axisType = static_cast<TagAxisInfo::E_AXIS_TYPE>(axisType);
    axisInfo.color = valueAxis.color();
    axisInfo.labelColor = valueAxis.labelsColor();
    axisInfo.visible = valueAxis.lineVisible();
    axisInfo.labelsVisible = valueAxis.labelsVisible();
    axisInfo.titleText = valueAxis.titleText();
    axisInfo.titleVisible = valueAxis.titleVisible();
    axisInfo.gridVisible = valueAxis.gridVisible();
    axisInfo.startValue = valueAxis.min();
    axisInfo.endValue = valueAxis.max();
    if(axisInfo.startValue >= axisInfo.endValue)
    {
        axisInfo.startValue = 0;
        axisInfo.endValue = 1;
    }
    axisInfo.tickCount = valueAxis.tickCount();
    if(axisInfo.tickCount < 2)
    {
        axisInfo.tickCount = 2;
    }
    axisInfo.decimals = valueAxis.decimals();

    axisInfo.scaleValue = (axisInfo.endValue - axisInfo.startValue)/(axisInfo.tickCount-1);
    axisInfo.listScaleValue.clear();
    double tmpValue = axisInfo.startValue;
    for(int i = 0; i < axisInfo.tickCount - 1; ++i)
    {
        axisInfo.listScaleValue.append(QString::number(tmpValue, 'f', axisInfo.decimals));
        tmpValue += axisInfo.scaleValue;
    }
    axisInfo.listScaleValue.append(QString::number(axisInfo.endValue, 'f', axisInfo.decimals));

    axisInfo.font = valueAxis.labelsFont();
    QFontMetrics fontMetrics(axisInfo.font);
    int startValueWidth = fontMetrics.width(QString::number(axisInfo.startValue, 'f', axisInfo.decimals));
    int endValueWidth = fontMetrics.width(QString::number(axisInfo.endValue, 'f', axisInfo.decimals));
    axisInfo.textMaxWidth = startValueWidth > endValueWidth ? startValueWidth : endValueWidth;
    axisInfo.textHeight = fontMetrics.height();
    if(axisType == TagAxisInfo::EAT_X_AXIS)
    {
        axisInfo.widthOrHeight = axisInfo.textHeight*2 + m_scaleWidth + m_textScaleSpace*2;
    }
    else
    {
        axisInfo.widthOrHeight = axisInfo.textHeight + m_textScaleSpace*2 + axisInfo.textMaxWidth + m_scaleWidth;
    }
}

/**
 * @brief CustomChartView:calculatePlotArea :  calculates the plot area
 *
 */
void CustomChartView::calculatePlotArea()
{
    QRectF rectf;

    //left
    double plotAreaLeftMargin;
    updatePlotAreaLeft(plotAreaLeftMargin);
    rectf.setX(plotAreaLeftMargin);
    //right
    double plotAreaRightMargin;
    updatePlotAreaRight(plotAreaRightMargin);
    rectf.setWidth(this->width() - plotAreaLeftMargin - plotAreaRightMargin);
    //top
    double plotAreaTopMargin;
    updatePlotAreaTop(plotAreaTopMargin, rectf.width());
    rectf.setY(plotAreaTopMargin);
    //bottom
    double plotAreaBottomMargin;
    updatePlotAreaBottom(plotAreaBottomMargin);
    rectf.setHeight(this->height() - plotAreaTopMargin - plotAreaBottomMargin);

    this->setPlotArea(rectf);
}

/**
 * @brief CustomChartView:calculateAxisInfo :  calculates the Axis infor
 *
 */
void CustomChartView::calculateAxisInfo()
{
    updateXAxisInfo();
    updateYAxisInfo();
    updateYRightAxisInfo();
}

/**
 * @brief CustomChartView:calculateSerisData :  calculates the series data
 *
 */
void CustomChartView::calculateSerisData()
{
    m_listSeriesData.clear();

    QList<CustomXYSeries*>::Iterator seriesItr = m_listXYSeries.begin();
    while(seriesItr != m_listXYSeries.end())
    {
        TagSeriesPoint *pSeriesPoint = (*seriesItr)->getDrawPointInfo();
        updateSerisData(*(*seriesItr), *pSeriesPoint);
        m_listSeriesData.append(pSeriesPoint);
        ++seriesItr;
    }
}

/**
 * @brief CustomChartView:updatePlotAreaLeft :  Updates the plot area on the left
 *
 */
void CustomChartView::updatePlotAreaLeft(double &plotAreaLeftMargin)
{
    plotAreaLeftMargin = m_iLeftMargin;
    QList<TagAxisInfo*>::Iterator yLeftItr = m_listYAxisInfo.begin();
    while(yLeftItr != m_listYAxisInfo.end())
    {
        if((*yLeftItr)->visible)
        {
            plotAreaLeftMargin += m_iAxisSpace + (*yLeftItr)->widthOrHeight;
        }
        ++yLeftItr;
    }
    if(plotAreaLeftMargin > m_iLeftMargin)
    {
        plotAreaLeftMargin -= m_iAxisSpace;
    }
}

/**
 * @brief CustomChartView:updatePlotAreaRight :  Updates the plot area on the Right
 *
 */
void CustomChartView::updatePlotAreaRight(double &plotAreaRightMargin)
{
    plotAreaRightMargin = m_iRightMargin;
    QList<TagAxisInfo*>::Iterator yRightItr = m_listYRightAxisInfo.begin();
    while(yRightItr != m_listYRightAxisInfo.end())
    {
        if((*yRightItr)->visible)
        {
            plotAreaRightMargin += m_iAxisSpace + (*yRightItr)->widthOrHeight;
        }
        ++yRightItr;
    }
    if(plotAreaRightMargin > m_iRightMargin)
    {
        plotAreaRightMargin -= m_iAxisSpace;
    }
}

/**
 * @brief CustomChartView:updatePlotAreaTop :  Updates the plot area on the Top
 *
 */
void CustomChartView::updatePlotAreaTop(double &plotAreaTopMargin, double plotAreaWidth)
{
    plotAreaTopMargin = m_iTopMargin;
    QList<TagSeriesTitle*>::Iterator seriesTitleItr = m_listSeriesTitle.begin();
    double tmpTitleWidth = 0;

    m_iTitleRow = 0;
    m_dTitleWidth = 0;

    if(seriesTitleItr != m_listSeriesTitle.end())
    {
        (*seriesTitleItr)->row = m_iTitleRow;
        plotAreaTopMargin += (*seriesTitleItr)->height + m_iTitleRowSpace;
        tmpTitleWidth = (*seriesTitleItr)->width;
        m_dTitleWidth += (*seriesTitleItr)->width;
        ++seriesTitleItr;
    }
    while(seriesTitleItr != m_listSeriesTitle.end())
    {
        tmpTitleWidth += m_iTitleSpace + (*seriesTitleItr)->width;
        m_dTitleWidth += m_iTitleSpace + (*seriesTitleItr)->width;
        if(tmpTitleWidth >= plotAreaWidth)
        {
            ++m_iTitleRow;
            tmpTitleWidth = (*seriesTitleItr)->width;
            plotAreaTopMargin += m_iTitleRowSpace + (*seriesTitleItr)->height;
        }
        (*seriesTitleItr)->row = m_iTitleRow;
        ++seriesTitleItr;
    }
}

/**
 * @brief CustomChartView:updatePlotAreaBottom :  Updates the plot area on the bottom
 *
 */
void CustomChartView::updatePlotAreaBottom(double &plotAreaBottomMargin)
{
    plotAreaBottomMargin = m_iBottomMargin;
    QList<TagAxisInfo*>::Iterator xItr = m_listXAxisInfo.begin();
    while(xItr != m_listXAxisInfo.end())
    {
        plotAreaBottomMargin += m_iAxisSpace + (*xItr)->widthOrHeight;
        ++xItr;
    }
    if(plotAreaBottomMargin > m_iBottomMargin)
    {
        plotAreaBottomMargin -= m_iAxisSpace;
    }
}

/**
 * @brief CustomChartView:updateXAxisInfo :  Updates the X Axis info
 *
 */
void CustomChartView::updateXAxisInfo()
{
    QList<TagAxisInfo*>::Iterator xItr = m_listXAxisInfo.begin();
    while(xItr != m_listXAxisInfo.end())
    {
        (*xItr)->listScalePos.clear();
        (*xItr)->pixelValue = ((*xItr)->endValue - (*xItr)->startValue)/m_plotArea.width();
        double scaleInterval = m_plotArea.width()/((*xItr)->tickCount-1);
        double scaleX = m_plotArea.x();
        for(int i = 0; i < (*xItr)->tickCount - 1; ++i)
        {
            (*xItr)->listScalePos.append(scaleX);
            scaleX += scaleInterval;
        }
        (*xItr)->listScalePos.append(m_plotArea.x() + m_plotArea.width());
        ++xItr;
    }
}

/**
 * @brief CustomChartView:updateYAxisInfo :  Updates the Y Axis info
 *
 */
void CustomChartView::updateYAxisInfo()
{
    QList<TagAxisInfo*>::Iterator yLeftItr = m_listYAxisInfo.begin();
    while(yLeftItr != m_listYAxisInfo.end())
    {
        (*yLeftItr)->listScalePos.clear();
        (*yLeftItr)->pixelValue = ((*yLeftItr)->endValue - (*yLeftItr)->startValue)/m_plotArea.height();
        double scaleInterval = m_plotArea.height()/((*yLeftItr)->tickCount - 1);
        double scaleY = m_plotArea.y() + m_plotArea.height();
        for(int i = 0; i < (*yLeftItr)->tickCount - 1; ++i)
        {
            (*yLeftItr)->listScalePos.append(scaleY);
            scaleY -= scaleInterval;
        }
        (*yLeftItr)->listScalePos.append(m_plotArea.y());
        ++yLeftItr;
    }
}

/**
 * @brief CustomChartView:updateYRightAxisInfo :  Updates the X Axis info
 *
 */
void CustomChartView::updateYRightAxisInfo()
{
    QList<TagAxisInfo*>::Iterator yRightItr = m_listYRightAxisInfo.begin();
    while(yRightItr != m_listYRightAxisInfo.end())
    {
        (*yRightItr)->listScalePos.clear();
        (*yRightItr)->pixelValue = ((*yRightItr)->endValue - (*yRightItr)->startValue)/m_plotArea.height();
        double scaleInterval = m_plotArea.height()/((*yRightItr)->tickCount - 1);
        double scaleY = m_plotArea.y() + m_plotArea.height();
        for(int i = 0; i < (*yRightItr)->tickCount - 1; ++i)
        {
            (*yRightItr)->listScalePos.append(scaleY);
            scaleY -= scaleInterval;
        }
        (*yRightItr)->listScalePos.append(m_plotArea.y());
        ++yRightItr;
    }
}

/**
 * @brief CustomChartView:updateSerisTitleData :  Updates the series title Data
 *
 */
void CustomChartView::updateSerisTitleData()
{
    m_listSeriesTitle.clear();

    QList<CustomXYSeries*>::Iterator seriesItr = m_listXYSeries.begin();
    while(seriesItr != m_listXYSeries.end())
    {
        serisToSerisTitle(*(*seriesItr), *((*seriesItr)->getTitleInfo()));
        if((*seriesItr)->visible())
        {
            m_listSeriesTitle.append((*seriesItr)->getTitleInfo());
        }
        ++seriesItr;
    }
}

/**
 * @brief CustomChartView:serisToSerisTitle :  Updates the series title properties
 *
 */
void CustomChartView::serisToSerisTitle(const CustomXYSeries &series, TagSeriesTitle &seriesTitle)
{
    seriesTitle.text = series.name();
    seriesTitle.color = series.color();
    QFontMetrics fontMetrics(m_titleFont);
    seriesTitle.width = m_iTitleRectWidth + seriesTitle.space + fontMetrics.width(seriesTitle.text);
    seriesTitle.height = fontMetrics.height() > m_iTitleRectWidth ? fontMetrics.height() : m_iTitleRectWidth;
}

/**
 * @brief CustomChartView:updateSerisData :  Updates the series data
 *
 */
bool CustomChartView::updateSerisData(const CustomXYSeries &series, TagSeriesPoint &seriesPoint)
{
    seriesPoint.listPoint.clear();
    CustomValueAxis *axisX = series.axisX();
    CustomValueAxis *axisY = series.axisYIsLeft() ? series.axisY() : series.axisYRight();
    if(axisX && axisY)
    {
        TagAxisInfo *pXAxisInfo = axisX->getAxisInfo();
        TagAxisInfo *pYAxisInfo = axisY->getAxisInfo();
        QList<QPointF>::const_iterator constSeriesDataItr = series.constListData().begin();
        while(constSeriesDataItr != series.constListData().end())
        {
            double x = m_plotArea.x() + ((*constSeriesDataItr).x() - pXAxisInfo->startValue)/pXAxisInfo->pixelValue;
            double y = m_plotArea.y() + (pYAxisInfo->endValue - (*constSeriesDataItr).y())/pYAxisInfo->pixelValue;
            seriesPoint.listPoint.append(QPointF(x, y));
            ++constSeriesDataItr;
        }
    }
    seriesPoint.color = series.color();
    seriesPoint.width = series.width();
    seriesPoint.visible = series.visible() && series.pointsVisible();
    return seriesPoint.listPoint.count() != 0;
}

/**
 * @brief CustomChartView:connectBySeries :  connect the signals and slots
 *
 */
void CustomChartView::connectBySeries(CustomXYSeries *pXYSeries)
{

    connect(pXYSeries, SIGNAL(listDataChanged()), this, SLOT(slotSeriesDataChanged()));
    connect(pXYSeries, SIGNAL(nameChanged()), this, SLOT(slotSeriesNameChanged()));
    connect(pXYSeries, SIGNAL(mwidthChanged()), this, SLOT(slotSeriesWidthChanged()));
    connect(pXYSeries, SIGNAL(colorChanged()), this, SLOT(slotSeriesColorChanged()));
    connect(pXYSeries, SIGNAL(axisXChanged()), this, SLOT(slotSeriesAxisXChanged()));
    connect(pXYSeries, SIGNAL(axisYChanged()), this, SLOT(slotSeriesAxisYChanged()));
    connect(pXYSeries, SIGNAL(axisYRightChanged()), this, SLOT(slotSeriesAxisYRightChanged()));
    connect(pXYSeries, SIGNAL(mvisibleChanged()), this, SLOT(slotSeriesVisibleChanged()));
    connect(pXYSeries, SIGNAL(pointsVisibleChanged()), this, SLOT(slotSeriesPointsVisibleChanged()));

/*
    connect(this, SIGNAL(listDataChanged()), this, SLOT(slotSeriesDataChanged()));
    connect(this, SIGNAL(nameChanged()), this, SLOT(slotSeriesNameChanged()));
    connect(this, SIGNAL(widthChanged()), this, SLOT(slotSeriesWidthChanged()));
    connect(this, SIGNAL(colorChanged()), this, SLOT(slotSeriesColorChanged()));
    connect(this, SIGNAL(axisXChanged()), this, SLOT(slotSeriesAxisXChanged()));
    connect(this, SIGNAL(axisYChanged()), this, SLOT(slotSeriesAxisYChanged()));
    connect(this, SIGNAL(axisYRightChanged()), this, SLOT(slotSeriesAxisYRightChanged()));
    connect(this, SIGNAL(visibleChanged()), this, SLOT(slotSeriesVisibleChanged()));
    connect(this, SIGNAL(pointsVisibleChanged()), this, SLOT(slotSeriesPointsVisibleChanged()));
*/
}
//pXYSeries
/**
 * @brief CustomChartView:connectByAxis :  connect the Axis signals and slots
 *
 */
void CustomChartView::connectByAxis(CustomValueAxis *pAxis)
{
    connect(pAxis, SIGNAL(colorChanged()), this, SLOT(slotAxisColorChanged()));
    connect(pAxis, SIGNAL(gridVisibleChanged()), this, SLOT(slotAxisGridVisibleChanged()));
    connect(pAxis, SIGNAL(labelsVisibleChanged()), this, SLOT(slotAxisLabelsVisibleChanged()));
    connect(pAxis, SIGNAL(labelsFontChanged()), this, SLOT(slotAxisLabelsFontChanged()));
    connect(pAxis, SIGNAL(labelsColorChanged()), this, SLOT(slotAxisLabelsColorChanged()));
    connect(pAxis, SIGNAL(lineVisibleChanged()), this, SLOT(slotAxisLineVisibleChanged()));
    connect(pAxis, SIGNAL(titleTextChanged()), this, SLOT(slotAxisTitleTextChanged()));
    connect(pAxis, SIGNAL(titleVisibleChanged()), this, SLOT(slotAxisTitleVisibleChanged()));
    connect(pAxis, SIGNAL(decimalsChanged()), this, SLOT(slotAxisDecimalsChanged()));
    connect(pAxis, SIGNAL(minChanged()), this, SLOT(slotAxisMinChanged()));
    connect(pAxis, SIGNAL(maxChanged()), this, SLOT(slotAxisMaxChanged()));
    connect(pAxis, SIGNAL(tickCountChanged()), this, SLOT(slotAxisTickCountChanged()));
}

/**
 * @brief CustomChartView:disconnectByAxis :  disconnect the signals and slots
 *
 */
void CustomChartView::disconnectByAxis()
{
    disconnect(this, SLOT(slotAxisColorChanged()));
    disconnect(this, SLOT(slotAxisGridVisibleChanged()));
    disconnect(this, SLOT(slotAxisLabelsVisibleChanged()));
    disconnect(this, SLOT(slotAxisLabelsFontChanged()));
    disconnect(this, SLOT(slotAxisLabelsColorChanged()));
    disconnect(this, SLOT(slotAxisLineVisibleChanged()));
    disconnect(this, SLOT(slotAxisTitleTextChanged()));
    disconnect(this, SLOT(slotAxisTitleVisibleChanged()));
    disconnect(this, SLOT(slotAxisDecimalsChanged()));
    disconnect(this, SLOT(slotAxisMinChanged()));
    disconnect(this, SLOT(slotAxisMaxChanged()));
    disconnect(this, SLOT(slotAxisTickCountChanged()));
}

