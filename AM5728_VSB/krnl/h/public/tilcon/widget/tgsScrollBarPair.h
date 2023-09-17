/* tgsScrollBarPair.cpp - scroll bar pair implementation */

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

This file defines the scroll bar pair widget.

INCLUDE FILES:
*/


#ifndef _INCLUDE_tgsScrollBarPair_H_
#define _INCLUDE_tgsScrollBarPair_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/widget/tgsScrollBar.h"


class TScrollBarPair : public TNodeArray
    {
public:
    enum ShowPolicy
        {
        SP_never = 0,
        SP_auto,
        SP_always
        };

public:
    struct ClientResizeEventParam : public EventParam
        {
        TSize client;
        TSize page;
        TSize total;
        };

    TEvent & ClientResizeEvent()
        {
        return clientResizeEvent;
        }

    TScrollBarPair(TRT_S32, TRT_S32 y, TRT_S32 w, TRT_S32 h,
        TMargin &, TVisual &, TextProperty &);
    virtual ~TScrollBarPair();

    TEvent & HorBarScrollEvent()
        {
        return horBar.ScrollEvent();
        }
    TEvent & VerBarScrollEvent()
        {
        return verBar.ScrollEvent();
        }

    void AssociateClient(TNode * c)
        {
        if (TRT_NULL == client)
            {
            AddChild(c, 2);
            client = c;
            }
        }

    void SetHorBarPolicy(ShowPolicy policy)
        {
        if (horBarPolicy != policy)
            {
            horBarPolicy = policy;
            UpdateBoundRect();
            }
        }

    ShowPolicy GetHorBarPolicy() const
        {
        return horBarPolicy;
        }

    void SetVerBarPolicy(ShowPolicy policy)
        {
        if (verBarPolicy != policy)
            {
            verBarPolicy = policy;
            UpdateBoundRect();
            }
        }

    ShowPolicy GetVerBarPolicy() const
        {
        return verBarPolicy;
        }

    void SetScrollBarPairData(const TSize & _page, const TSize & _total, const TPoint & _pos)
        {
        SetHorBarScrollData(_page.w, _total.w, _pos.x);
        SetVerBarScrollData(_page.h, _total.h, _pos.y);
        }

    void SetHorBarScrollData(TRT_S32 page, TRT_S32 total, TRT_S32 pos)
        {
        horBar.SetScrollData(page, total, pos);
        }

    void SetVerBarScrollData(TRT_S32 page, TRT_S32 total, TRT_S32 pos)
        {
        verBar.SetScrollData(page, total, pos);
        }

    void SetScrollPos(const TPoint & pt)
        {
        horBar.SetScrollPos(pt.x);
        verBar.SetScrollPos(pt.y);
        }

    TPoint GetScrollPos() const
        {
        return TPoint(horBar.GetScrollPos(), verBar.GetScrollPos());
        }

    void SetHorBarStepSize(TRT_S32 arrowStepSize)
        {
        horBar.SetStepSize(arrowStepSize);
        }

    void SetVerBarStepSize(TRT_S32 arrowStepSize)
        {
        verBar.SetStepSize(arrowStepSize);;
        }

    void SetScrollBarButtonWidth(TRT_S32 value)
        {
        horBar.SetButtonWidth(value);
        verBar.SetButtonWidth(value);
        }

    TRT_S32 GetScrollBarButtonWidth() const
        {
        return horBar.GetButtonWidth();
        }

    void SetScrollBarTrackWidth(TRT_S32 value)
        {
        trackWidth = value;
        UpdateBoundRect();
        horBar.SetTrackWidth(value);
        verBar.SetTrackWidth(value);
        }

    TRT_S32 GetScrollBarTrackWidth() const
        {
        return trackWidth;
        }

    void UpdateLayout();

    TRT_BOOL IsHorBarPresent() const
        {
        return horBarPresent;
        }

    TRT_BOOL IsVerBarPresent() const
        {
        return verBarPresent;
        }

    void SetLeftArrowVisual(const TCustomVisual & value)
        {
        horBarLeftButtonVisual.SetCustomVisual(value);
        Update();
        }

    const TCustomVisual & GetLeftArrowVisual() const
        {
        return horBarLeftButtonVisual.GetCustomVisual();
        }

    void SetRightArrowVisual(const TCustomVisual & value)
        {
        horBarRightButtonVisual.SetCustomVisual(value);
        Update();
        }

    const TCustomVisual & GetRightArrowVisual() const
        {
        return horBarRightButtonVisual.GetCustomVisual();
        }

    void SetHorBarTrackVisual(const TCustomVisual & value)
        {
        horBarTrackVisual.SetCustomVisual(value);
        Update();
        }

    const TCustomVisual & GetHorBarTrackVisual() const
        {
        return horBarTrackVisual.GetCustomVisual();
        }

    void SetHorBarThumbVisual(const TCustomVisual & value)
        {
        horBarThumbVisual.SetCustomVisual(value);
        Update();
        }

    const TCustomVisual & GetHorBarThumbVisual() const
        {
        return horBarThumbVisual.GetCustomVisual();
        }

    void SetUpArrowVisual(const TCustomVisual & value)
        {
        verBarUpButtonVisual.SetCustomVisual(value);
        Update();
        }

    const TCustomVisual & GetUpArrowVisual() const
        {
        return verBarUpButtonVisual.GetCustomVisual();
        }

    void SetDownArrowVisual(const TCustomVisual & value)
        {
        verBarDownButtonVisual.SetCustomVisual(value);
        Update();
        }

    const TCustomVisual & GetDownArrowVisual() const
        {
        return verBarDownButtonVisual.GetCustomVisual();
        }

    void SetVerBarTrackVisual(const TCustomVisual & value)
        {
        verBarTrackVisual.SetCustomVisual(value);
        Update();
        }

    const TCustomVisual & GetVerBarTrackVisual() const
        {
        return verBarTrackVisual.GetCustomVisual();
        }

    void SetVerBarThumbVisual(const TCustomVisual & value)
        {
        verBarThumbVisual.SetCustomVisual(value);
        Update();
        }

    const TCustomVisual & GetVerBarThumbVisual() const
        {
        return verBarThumbVisual.GetCustomVisual();
        }

private:
    void NodeHandler(EventParam &);

    void ResizeHandler(EventParam &);

    void UpdateScrollBarShowPolicy();
    void UpdateLayout(const TSize & size);

    TEvent      clientResizeEvent;

    ShowPolicy          horBarPolicy;
    ShowPolicy          verBarPolicy;
    TSize               scrollTotalSize;
    TSize               scrollPageSize;

    TRT_S32             trackWidth;

    TMargin &   margin;
    TVisual &   frameVisual;
    TVisual     horBarTrackVisual;
    TVisual     horBarThumbVisual;
    TVisual     horBarLeftButtonVisual;
    TVisual     horBarRightButtonVisual;
    TVisual     verBarTrackVisual;
    TVisual     verBarThumbVisual;
    TVisual     verBarUpButtonVisual;
    TVisual     verBarDownButtonVisual;

    TNodeEventDelegate  eventDelegate;
    TScrollBarNode      horBar;
    TScrollBarNode      verBar;
    TNode *             client;

    TRT_BOOL    horBarPresent;
    TRT_BOOL    verBarPresent;
    };

#endif /* _INCLUDE_tgsScrollBarPair_H_ */

