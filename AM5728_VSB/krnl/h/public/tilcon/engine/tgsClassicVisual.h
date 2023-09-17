/* tgsClassicVisual.h - Tilcon classic visual engine */

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

This file defines the Tilcon's classic visual engine. The classic visual engine
is derived from Tilcon's default visual engine, and it overwrites some of the
default visual engine's drawing methods and replace it with something which
can draw fancier staff, such as button with gradient fill.

Users are strongly encouraged to write their own visual engine, based on the
default visual engine or the classic visual engine, to added personalized
appearance to the User Interface.

INCLUDE FILES: tilcon/engine/tgsClassicVisual.h
*/

#ifndef _INCLUDE_tgsClassicVisual_H_
#define _INCLUDE_tgsClassicVisual_H_

#include "./tgsDefaultVisual.h"

class TClassicVisualEngine : public TDefaultVisualEngine
    {
public:
    TClassicVisualEngine();
    virtual ~TClassicVisualEngine();

    virtual void DrawFrame(FrameType, TPainter &, const VisualState &, const TRect &);

protected:
    TPaint paintHorNormal;
    TPaint paintHorHighlighted;
    TPaint paintHorActive;
    TPaint paintHorFocused;
    TPaint paintHorFocusedHighlighted;
    TPaint paintHorFocusedActive;
    TPaint paintHorDimmed;

    TPaint paintVerNormal;
    TPaint paintVerHighlighted;
    TPaint paintVerActive;
    TPaint paintVerFocused;
    TPaint paintVerFocusedHighlighted;
    TPaint paintVerFocusedActive;
    TPaint paintVerDimmed;

    TPaint paintProgressBar;

    void DrawToggleButton(TPainter &, const VisualState &, const TRect &, TRT_BOOL);
    void DrawProgressBar(TPainter &, const VisualState &, const TRect &, OrientationType);

    void DrawHorRect(TPainter &, const VisualState &, const TRect &);
    void DrawVerRect(TPainter &, const VisualState &, const TRect &);

    void DrawRoundBorder(TPainter &, const VisualState &, const TRect &) const;
    };

#endif /* _INCLUDE_tgsClassicVisual_H_ */

