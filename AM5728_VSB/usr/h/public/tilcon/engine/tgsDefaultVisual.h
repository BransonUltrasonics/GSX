/* tgsDefaultVisual.h - Tilcon classic visual engine */

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
29jul15,c_l support circle,ellipse and arc (V7GFX-247)
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the Tilcon's default visual engine. The default visual engine
renders the User Interface with mainly lines and rectangles, and the drawing
speed is fast.

Users are strongly encouraged to write their own visual engine, based on the
default visual engine, to added personalized appearance to the User Interface.

INCLUDE FILES: tilcon/engine/tgsDefaultVisual.h
*/

#ifndef _INCLUDE_tgsDefaultVisual_H_
#define _INCLUDE_tgsDefaultVisual_H_

#include "tilcon/engine/tgsApplication.h"

class TControl;

class TDefaultVisualEngine : public TVisualEngine
    {
public:
    TDefaultVisualEngine();
    virtual ~TDefaultVisualEngine();

    virtual void DrawFrame(FrameType, TPainter &, const VisualState &, const TRect &);

protected:
    static TRT_U32 bufferCheck_d[13*13];
    static TRT_U32 bufferCheck_h[13*13];
    static TRT_U32 bufferCheck_p[13*13];
    static TRT_U32 bufferCheck_cn[13*13];
    static TRT_U32 bufferCheck_cd[13*13];
    static TRT_U32 bufferCheck_ch[13*13];
    static TRT_U32 bufferCheck_cp[13*13];

    static TRT_U32 bufferRadio_n[15*15];
    static TRT_U32 bufferRadio_d[15*15];
    static TRT_U32 bufferRadio_h[15*15];
    static TRT_U32 bufferRadio_p[15*15];
    static TRT_U32 bufferRadio_cn[15*15];
    static TRT_U32 bufferRadio_cd[15*15];
    static TRT_U32 bufferRadio_ch[15*15];
    static TRT_U32 bufferRadio_cp[15*15];

    TPixelBuffer  check_d;
    TPixelBuffer  check_h;
    TPixelBuffer  check_p;
    TPixelBuffer  check_cn;
    TPixelBuffer  check_cd;
    TPixelBuffer  check_ch;
    TPixelBuffer  check_cp;

    TPixelBuffer  radio_n;
    TPixelBuffer  radio_d;
    TPixelBuffer  radio_h;
    TPixelBuffer  radio_p;
    TPixelBuffer  radio_cn;
    TPixelBuffer  radio_cd;
    TPixelBuffer  radio_ch;
    TPixelBuffer  radio_cp;

    TColor colorFlatBorderNormal;
    TColor colorFlatBorderHighlighted;
    TColor colorFlatBorderActive;
    TColor color3DBorder1ToneBright;
    TColor color3DBorder2ToneBright;
    TColor color3DBorder1ToneDull;
    TColor color3DBorder2ToneDull;

    TColor colorPanelNormal;
    TColor colorPanelHighlighted;
    TColor colorPanelActive;
    TColor colorPanelFocused;
    TColor colorPanelDimmed;

    TColor colorFieldNormal;
    TColor colorFieldHighlighted;
    TColor colorFieldActive;
    TColor colorFieldFocused;
    TColor colorFieldDimmed;

    TColor colorScrollButtonNormal;
    TColor colorScrollButtonHighlighted;
    TColor colorScrollButtonActive;
    TColor colorScrollButtonFocused;
    TColor colorScrollButtonDimmed;

    TColor colorScrollTrack;
    TColor colorProgressBar;

    void DrawHeaderPanel(TPainter &, const VisualState &, const TRect &);
    void DrawPushButton(TPainter &, const VisualState &, const TRect &);
    void DrawToggleButton(TPainter &, const VisualState &, const TRect &, TRT_BOOL);
    void DrawCheckIndicator(TPainter &, const VisualState &, const TRect &, TRT_BOOL);
    void DrawRadioIndicator(TPainter &, const VisualState &, const TRect &, TRT_BOOL);
    void DrawComboboxButton(TPainter &, const VisualState &, const TRect &);
    void DrawScrollTrack(TPainter &, const VisualState &, const TRect &, OrientationType);
    void DrawScrollThumb(TPainter &, const VisualState &, const TRect &, OrientationType);
    void DrawSliderTrack(TPainter &, const VisualState &, const TRect &, OrientationType);
    void DrawSliderThumb(TPainter &, const VisualState &, const TRect &, OrientationType);

    void DrawEtchBorder(TPainter & painter, const TRect & rect, TRT_S32 width, TRT_BOOL sunken) const;
    void DrawFlatBorder(TPainter & painter, const TRect & rect, TRT_S32 width, const TColor & color) const;
    void Draw3DBorder(TPainter & painter, const TRect & rect, TRT_S32 width, TRT_BOOL sunken) const;
    void Draw3DCircle(TPainter & painter, const TRect & rect, TRT_S32 width, TRT_BOOL sunken) const;
    void DrawSolidRect(TPainter & painter, const TRect & rect, const TColor & color) const;
    void DrawSolidCircle(TPainter & painter, const TRect & rect, const TColor & color) const;
    void DrawLeftArrow(TPainter & painter, const TRect & rect, const TColor & color) const;
    void DrawRightArrow(TPainter & painter, const TRect & rect, const TColor & color) const;
    void DrawUpArrow(TPainter & painter, const TRect & rect, const TColor & color) const;
    void DrawDownArrow(TPainter & painter, const TRect & rect, const TColor & color) const;
    };

#endif /* _INCLUDE_tgsDefaultVisual_H_ */

