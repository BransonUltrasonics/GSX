/* tgsScale.h - scale header */

/*
 * Copyright (c) 2014-2015, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
06jan17,jnl code clean
29jul15,c_l support circle,ellipse and arc (V7GFX-247)
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the axis scale used in chart.
*/


#ifndef _INCLUDE_tgsScale_H_
#define _INCLUDE_tgsScale_H_

#include "tilcon/engine/tgsControl.h"

class TScale : public TControl
    {
public:
    enum PositioningMethod
        {
        PM_auto = 0,
        PM_fixed
        };
    enum TickPosition
        {
        TP_outside_baseline = 0,
        TP_through_baseline,
        TP_inside_baseline,
        };
    enum LabelPosition
        {
        LP_outside_baseline = 0,
        LP_inside_baseline,
        };

    struct ScaleInfo
        {
        ScaleInfo();

        TickPosition    tickPosition;
        LabelPosition   labelPosition;

        TPen            baselinePen;
        TPen            majorTickPen;
        TPen            minorTickPen;
        TextProperty    textProperty;

        TRT_S32         tickStartOffset;
        TRT_S32         tickEndOffset;
        TRT_S32         majorTickLength;
        TRT_S32         majorTickIntervals;
        TRT_S32         minorTickLength;
        TRT_S32         minorTickIntervals;
        TRT_S32         labelOffset;

        TRT_U8          showLabel       : 1;
        TRT_U8          showBaseLine    : 1;
        TRT_U8          showMajorTick   : 1;
        TRT_U8          showMinorTick   : 1;
        TRT_U8          showAlarm       : 1;
        };
public:
    TScale(TRT_S32 x = 0, TRT_S32 y = 0,
                TRT_S32 w = 0, TRT_S32 h = 0, OrientationType d = OT_horizontal);
    virtual ~TScale();
    void SetScaleInfo(const ScaleInfo &);
    const ScaleInfo & GetScaleInfo() const
        {
        return scaleInfo;
        }

    void SetRange(const TLinear & value);
    const TLinear & GetRange() const
        {
        return range;
        }
    static void GenerateLabels(const TLinear & range, const ScaleInfo & info,
        TArray<TWString> & labels);
    static void GenerateLabels(const TLinearF & range, const ScaleInfo & info,
        TArray<TWString> & labels);
    static void DrawXScale(TPainter & painter, TRT_S32 x1, TRT_S32 x2, TRT_S32 y,
        const ScaleInfo & info, const TArray<TWString> & labels);
    static void DrawYScale(TPainter & painter, TRT_S32 x, TRT_S32 y1, TRT_S32 y2,
        const ScaleInfo & info, const TArray<TWString> & labels);

private:
    void NodeHandler(EventParam &);
    void OnResize(TNode::ResizeEventParam &);
    void OnPaint(TNode::PaintEventParam &);

    TNodeEventDelegate  eventDelegate;
    TNode               boundNode;

    TRT_S32             baselineStartOffset;
    TRT_S32             baselineEndOffset;
    TPoint              baselineStart;
    TPoint              baselineEnd;

    OrientationType     dimensionType;
    PositioningMethod   positioningMethod;
    ScaleInfo           scaleInfo;
    TLinear             range;
    TArray<TWString>    labels;
    };

#endif /* _INCLUDE_tgsScale_H_ */

