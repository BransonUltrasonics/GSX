/* tgsPushButtonNode.h - Tilcon push button node */

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
23oct15,c_l IA64 warning reduction
01jan14,yya created
*/

/*
DESCRIPTION

This file defines Tilcon's push button node.

INCLUDE FILES: tilcon/widget/tgsPushButtonNode.h
*/

#ifndef _INCLUDE_tgsPushButtonNode_H_
#define _INCLUDE_tgsPushButtonNode_H_

#include "tilcon/engine/tgsControl.h"

/* push button class */
class TPushButtonNode : public TNode
    {
public:

    /* parameter of click event */
    class ClickParam : public NodeEventParam
        {
    public:
        TRT_BOOL isRepeatedClick;       /* TRT_TRUE for repeated click */
        TRT_S32  repeatedTimes;         /* repeated times */
        TRT_S32  elapseSinceMouseDown;  /* elapsed time since mouse down */
        };

public:
    TPushButtonNode(TRT_S32 x, TRT_S32 y, TRT_S32 w, TRT_S32 h, TVisual &, TextProperty &);
    virtual ~TPushButtonNode();

    TEvent & ClickEvent()
        {
        return clickEvent;
        }

    void Click();

    void SetText(const TWString & value)
        {
        if (text != value)
            {
            text = value;
            UpdateBoundRect();
            }
        }

    const TWString & GetText() const
        {
        return text;
        }

    void SetShowText(TRT_BOOL show)
        {
        if (showText != ((show) ? 1: 0))
            {
            showText = (show & 0x1U) ? !!1 : !!0;
            Update();
            }
        }

    TRT_BOOL GetShowText() const
        {
        return showText;
        }

    void SetTextMargin(const TMargin & value)
        {
        if (textMargin != value)
            {
            textMargin = value;
            UpdateBoundRect();
            }
        }

    const TMargin & GetTextMargin() const
        {
        return textMargin;
        }

    void SetClickWhenMouseUp(TRT_BOOL value)
        {
        clickWhenMouseUp = (value & 0x1U) ? !!1 : !!0;
        }

    TRT_BOOL GetClickWhenMouseUp() const
        {
        return clickWhenMouseUp;
        }

    void SetClickRepeat(TRT_BOOL value)
        {
        clickRepeat = (value & 0x1U) ? !!1 : !!0;
        }

    TRT_BOOL GetClickRepeat() const
        {
        return clickRepeat;
        }

    void SetClickRepeatInitDelay(TRT_S32 value)
        {
        repeatTimer.SetInitDelay(value);
        }

    TRT_S32 GetClickRepeatInitDelay() const
        {
        return repeatTimer.GetInitDelay();
        }

    void SetClickRepeatReloadDelay(TRT_S32 value)
        {
        repeatTimer.SetRepeatDelay(value);
        }

    TRT_S32 GetClickRepeatReloadDelay() const
        {
        return repeatTimer.GetRepeatDelay();
        }

    void SetClickRepeatTimes(TRT_S32 value)
        {
        repeatTimer.SetRepeatTimes(value);
        }

    TRT_S32 GetClickRepeatTimes() const
        {
        return repeatTimer.GetRepeatTimes();
        }

protected:
    void NodeHandler(EventParam &);
    void RepeatTimerHandler(EventParam &);

    void OnPaint(PaintEventParam &);
    void OnMouse(MouseEventParam &);
    void OnDetach(EventParam &);

    TEvent              clickEvent;
    TNodeEventDelegate  eventDelegate;
    TTimer              repeatTimer;

    /* button property */
    TWString        text;
    TMargin         textMargin;

    TextProperty &  textProperty;
    TVisual &       buttonVisual;

    TRT_U8          clickRepeat         : 1;
    TRT_U8          clickWhenMouseUp    : 1;
    TRT_U8          showText            : 1;
    };

#endif /* _INCLUDE_tgsPushButtonNode_H_ */

