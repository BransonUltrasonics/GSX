/* tgsScrollBarNode.h - Tilcon scroll bar node */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
01jan14,yya created
*/

/*
DESCRIPTION

This file defines Tilcon's scroll bar node.

INCLUDE FILES: tilcon/widget/tgsScrollBarNode.h
*/

#ifndef _INCLUDE_tgsScrollBarNode_H_
#define _INCLUDE_tgsScrollBarNode_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/widget/tgsPushButton.h"

class TScrollThumb
    {
public:
    TScrollThumb(TNode & o, TEvent & e);
    virtual ~TScrollThumb();

public:

    void BeginDrag(TRT_S32);
    void Drag(TRT_S32);
    void EndDrag(TRT_S32);

    TRT_S32 GetThumbLen() const;
    TRT_S32 GetThumbPos() const;

    void SetTrackAndThumbLength(TRT_S32 trackLen, TRT_S32 thumbLen);
    void SetScrollPos(TRT_S32);
    void SetScrollData(TRT_S32 page, TRT_S32 total, TRT_S32 pos);
    TRT_S32 GetScrollPos() const
        {
        return scrollPos;
        }
    TRT_S32 GetScrollPageSize() const
        {
        return scrollPageSize;
        }
    TRT_S32 GetScrollTotalSize() const
        {
        return scrollTotalSize;
        }
    void ScrollTo(TRT_S32);
    void ScrollBy(TRT_S32);

private:
    void AdjustThumb();

    TNode &    owner;
    TEvent &   scrollEvent;

    TRT_S32         thumbLen;
    TRT_S32         thumbPos;
    TRT_S32         trackLen;

    TRT_S32         scrollPageSize;
    TRT_S32         scrollTotalSize;
    TRT_S32         scrollPos;
    TLinear         scrollRange;
    TLinear         thumbRange;

    TRT_S32         dragBeginThumbPos;
    TRT_S32         dragBeginMousePos;
    TRT_S32         dragBeginScrollPos;
    };

class TScrollBarNode : public TNodeArray
    {
public:
    class ScrollParam : public NodeEventParam
        {
    public:
        TRT_S32 pos;
        TRT_S32 delta;
        };

public:
    TScrollBarNode(TRT_S32 x, TRT_S32 y,
            TRT_S32 w, TRT_S32 h,
            OrientationType _direction,
            TRT_S32 _page, TRT_S32 _total, TRT_S32 _pos,
            TVisual &, TVisual &, TVisual &, TVisual &, TextProperty &);

    virtual ~TScrollBarNode();

    TEvent & ScrollEvent()
        {
        return scrollEvent;
        }

    void SetStepSize(TRT_S32 value);
    TRT_S32 GetStepSize() const
        {
        return scrollArrowStepSize;
        }
    void SetOrientation(OrientationType);
    OrientationType GetOrientation() const
        {
        return orientation;
        }
    void SetScrollPos(TRT_S32);
    void SetScrollData(TRT_S32 page, TRT_S32 total, TRT_S32 pos);
    TRT_S32 GetScrollPos() const
        {
        return drag.GetScrollPos();
        }
    TRT_S32 GetScrollPageSize() const
        {
        return drag.GetScrollPageSize();
        }
    TRT_S32 GetScrollTotalSize() const
        {
        return drag.GetScrollTotalSize();
        }
    void ScrollTo(TRT_S32);
    void ScrollBy(TRT_S32);

    void SetButtonWidth(TRT_S32 value)
        {
        if (buttonWidth != value)
            {
            buttonWidth = value;
            UpdateBoundRect();
            }
        }

    TRT_S32 GetButtonWidth() const
        {
        return buttonWidth;
        }

    void SetTrackWidth(TRT_S32 value)
        {
        if (trackWidth != value)
            {
            trackWidth = value;
            UpdateBoundRect();
            }
        }

    TRT_S32 GetTrackWidth() const
        {
        return trackWidth;
        }

private:
    void UpdateArrowAndTrackSize(const TSize & size);

    void NodeHandler(EventParam &);
    void NegativeClickHandler(EventParam &);
    void PositiveClickHandler(EventParam &);

    void ResizeHandler(EventParam &);
    void ThumbMouseHandler(EventParam &);

    void UpdateThumb();

    TEvent              scrollEvent;

    TNodeEventDelegate  eventDelegate;
    TNode               thumb;
    TPushButtonNode     negativeArrow;
    TPushButtonNode     positiveArrow;

    TRect               trackRect;
    TScrollThumb        drag;

    OrientationType     orientation;
    TRT_S32             trackWidth;
    TRT_S32             buttonWidth;
    TRT_S32             scrollArrowStepSize;

    TVisual &           trackVisual;
    TVisual &           thumbVisual;
    };

#endif /* _INCLUDE_tgsScrollBar_H_ */

