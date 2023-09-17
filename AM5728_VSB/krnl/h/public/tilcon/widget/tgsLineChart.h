/* tgsLineChart.h - line chart header */

/*
 * Copyright (c) 2014-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
29jul15,c_l  support circle,ellipse and arc (V7GFX-247)
01jan14,yya  created (US15550)
*/

/*
DESCRIPTION

This file defines line chart widget.
*/


#ifndef _INCLUDE_tgsLineChart_H_
#define _INCLUDE_tgsLineChart_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/widget/tgsScale.h"

#define TRT_DEFAULT_LINE_CHART_DATA_CNT     (100)


class TChartBody
    {
public:

    struct BodyInfo
        {
        BodyInfo() :
            majorXLinePen(TRT_COLOR_BLACK, 2),
            majorYLinePen(TRT_COLOR_BLACK, 2),
            minorXLinePen(TRT_COLOR_BLACK, 1),
            minorYLinePen(TRT_COLOR_BLACK, 1),
            showMajorXLine(TRT_TRUE),
            showMajorYLine(TRT_TRUE),
            showMinorXLine(TRT_TRUE),
            showMinorYLine(TRT_TRUE),
            showXScale(TRT_TRUE),
            showYScale(TRT_TRUE)
            {
            yScaleInfo.majorTickIntervals = 6;
            }
        TPen                majorXLinePen;
        TPen                majorYLinePen;
        TPen                minorXLinePen;
        TPen                minorYLinePen;
        TScale::ScaleInfo   xScaleInfo;
        TScale::ScaleInfo   yScaleInfo;
        TRT_BOOL            showMajorXLine;
        TRT_BOOL            showMajorYLine;
        TRT_BOOL            showMinorXLine;
        TRT_BOOL            showMinorYLine;
        TRT_BOOL            showXScale;
        TRT_BOOL            showYScale;
        };

public:
    TChartBody();
    virtual ~TChartBody();

    static void DrawXMajorLine(TPainter & painter, const TRect & rect, const BodyInfo & info);
    static void DrawXMinorLine(TPainter & painter, const TRect & rect, const BodyInfo & info);
    static void DrawYMajorLine(TPainter & painter, const TRect & rect, const BodyInfo & info);
    static void DrawYMinorLine(TPainter & painter, const TRect & rect, const BodyInfo & info);
    static void DrawXScale(TPainter & painter, const TRect & rect, const BodyInfo & info, const TArray<TWString> & labels);
    static void DrawYScale(TPainter & painter, const TRect & rect, const BodyInfo & info, const TArray<TWString> & labels);
    static void GenerateXLabels(const TLinearF & range, const BodyInfo & info, TArray<TWString> & labels);
    static void GenerateYLabels(const TLinearF & range, const BodyInfo & info, TArray<TWString> & labels);

    void SetXRange(const TLinearF & value)
        {
        GenerateXLabels(value, viewInfo, xLabels);
        }

    void SetYRange(const TLinearF & value)
        {
        GenerateYLabels(value, viewInfo, yLabels);
        }

    void SetCurveAreaMargin(const TMargin & value)
        {
        if (curveAreaMargin != value)
            {
            curveAreaMargin = value;
            }
        }

    void SetBodyInfo(const BodyInfo & value)
        {
        viewInfo = value;
        }

    TRect GetCurveAreaRect(const TRect & rect) const
        {
        TRect viewRect = rect;
        viewRect = viewRect.Adjust(curveAreaMargin);
        viewRect.left += viewInfo.xScaleInfo.tickStartOffset;
        viewRect.right -= viewInfo.xScaleInfo.tickEndOffset;
        viewRect.top += viewInfo.yScaleInfo.tickEndOffset;
        viewRect.bottom -= viewInfo.yScaleInfo.tickStartOffset;
        return viewRect;
        }

    void DrawBody(TPainter & painter, const TRect & rect);

    TMargin                 curveAreaMargin;
    BodyInfo                viewInfo;
    TArray<TWString>        xLabels;
    TArray<TWString>        yLabels;
    };

class TLineChart : public TControl
    {
public:
    enum CurveShape
        {
        CS_line = 0,
        CS_bar,
        CS_fill,
        };
    enum CurveMode
        {
        CM_scroll = 0,
        CM_sweep,
        };

    TLineChart
        (
        TRT_S32 x = 0,
        TRT_S32 y = 0,
        TRT_S32 w = 0,
        TRT_S32 h = 0,
        TRT_U32 channel_num = 1,
        TRT_U32 channel_size = TRT_DEFAULT_LINE_CHART_DATA_CNT
        );
    virtual ~TLineChart();

    void SetSamplesPerScreen(TRT_U32 value)
        {
        if (samplesPerScreen != value)
            {
            samplesPerScreen = value;
            }
        }
    void SetCurveAreaMargin(const TMargin & value)
        {
        body.SetCurveAreaMargin(value);
        Update();
        }

    void SetBodyInfo(const TChartBody::BodyInfo & value)
        {
        body.SetBodyInfo(value);
        }

    void SetXRange(const TLinearF & value)
        {
        if (xRange != value)
            {
            xRange = value;
            body.SetXRange(xRange);
            }
        }

    void SetYRange(const TLinearF & value)
        {
        if (yRange != value)
            {
            yRange = value;
            body.SetYRange(yRange);
            }
        }

    void SetCurveShape(CurveShape value)
        {
        if (curveShape != value)
            {
            curveShape = value;
            Update();
            }
        }

    void SetCurveMode(CurveMode value)
        {
        if (curveMode != value)
            {
            curveMode = value;
            Update();
            }
        }

    void SetChannelVisible(TRT_S32, TRT_BOOL);
    void SetChannelAmplifier(TRT_S32, TRT_FLOAT);
    void SetChannelOffset(TRT_S32, TRT_FLOAT);
    void SetChannelPen(TRT_S32, const TPen &);
    void SetChannelBrush(TRT_S32, const TBrush &);

    void SetDataSize(TRT_U32 size);

    void ResetData(TRT_S32 channel)
        {
        channels[channel].sweepData.Reset();
        channels[channel].scrollData.Reset();
        Update();
        }

    void InsertData(TRT_S32 channel, TRT_FLOAT value)
        {
        channels[channel].sweepData.InsertData(value);
        channels[channel].scrollData.InsertData(value);
        Update();
        }

private:
    class ScrollData
            {
        public:
            ScrollData() :
                buffer(TRT_NULL),
                count(0),
                head(0),
                index(0)
                {
                }

            ~ScrollData()
                {
                if (buffer)
                    {
                    delete [] buffer;
                    }
                }

            TRT_U32 GetCount() const
                {
                return count;
                }

            TRT_U32 GetIndex() const
                {
                return index;
                }

            TRT_FLOAT GetData(TRT_U32 index) const
                {
                if (index < count)
                    {
                    return buffer[(index + head) % count];
                    }
                return 0.0f;
                }

            void Reset()
                {
                head = 0;
                index = 0;
                for (TRT_U32 i = 0; i < count; i++)
                    {
                    buffer[i] = 0.0f;
                    }
                }

            void Resize(TRT_U32 new_size)
                {
                if (buffer)
                    {
                    delete [] buffer;
                    }
                buffer = new_size == 0 ? TRT_NULL : new TRT_FLOAT[new_size];
                count = new_size;
                head = new_size == 0 ? 0 : new_size - 1;
                }

            void InsertData(TRT_FLOAT value)
                {
                if (count == 0 || buffer == 0)
                    {
                    return;
                    }
                head = (head + count - 1) % count;
                buffer[head] = value;
                index++;
                }

        private:
            TRT_FLOAT * buffer;
            TRT_U32     count;
            TRT_U32     head;
            TRT_U32     index;
            };

    class SweepData
        {
    public:
        SweepData() :
            buffer(TRT_NULL),
            count(0),
            head(0)
            {
            }

        ~SweepData()
            {
            if (buffer)
                {
                delete [] buffer;
                }
            }

        TRT_U32 GetCount() const
            {
            return count;
            }

        TRT_U32 GetHead() const
            {
            return head;
            }

        TRT_FLOAT GetData(TRT_U32 index) const
            {
            if (index < count)
                {
                return buffer[index];
                }
            return 0.0f;
            }

        void Reset()
            {
            head = 0;
            for (TRT_U32 i = 0; i < count; i++)
                {
                buffer[i] = 0.0f;
                }
            }

        void Resize(TRT_U32 new_size)
            {
            if (buffer)
                {
                delete [] buffer;
                }
            buffer = new_size == 0 ? TRT_NULL : new TRT_FLOAT[new_size];
            count = new_size;
            head = 0;
            }

        void InsertData(TRT_FLOAT value)
            {
            if (count == 0 || buffer == 0)
                {
                return;
                }
            head = (head + count + 1) % count;
            buffer[head] = value;
            }

    private:
        TRT_FLOAT * buffer;
        TRT_U32     count;
        TRT_S32     head;
        };

    struct ChannelInfo
        {
        ChannelInfo() : show(TRT_TRUE),
            amplifier(1.0f), offset(0.0f)
            {}
        SweepData               sweepData;
        ScrollData              scrollData;
        TPen                    pen;
        TBrush                  brush;
        TRT_BOOL                show;
        TRT_FLOAT               amplifier;
        TRT_FLOAT               offset;
        };

private:
    void NodeHandler(EventParam &);
    void OnPaint(TNode::PaintEventParam &);

    void DrawScrollLineChart(TPainter & painter, const TRect & rect,
        const TLinearF & range, const ChannelInfo & chanInfo);
    void DrawSweepLineChart(TPainter & painter, const TRect & rect,
        const TLinearF & range, const ChannelInfo & chanInfo);

    TNodeEventDelegate      eventDelegate;
    TNode                   boundNode;

    TChartBody              body;
    TLinearF                xRange;
    TLinearF                yRange;
    TArray<ChannelInfo>     channels;
    TRT_U32                 samplesPerScreen;
    CurveShape              curveShape;
    CurveMode               curveMode;
    };

#endif /* _INCLUDE_tgsLineChart_H_ */

