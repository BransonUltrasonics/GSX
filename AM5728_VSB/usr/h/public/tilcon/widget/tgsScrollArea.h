/* tgsScrollArea.h - Tilcon scroll area widget */

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

This file defines Tilcon's scroll area widget. The scroll area widget is a
container which holds child widget inside a view that can be scrolled.

INCLUDE FILES: tilcon/widget/tgsScrollArea.h
*/

#ifndef _INCLUDE_tgsScrollArea_H_
#define _INCLUDE_tgsScrollArea_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/engine/tgsContainer.h"
#include "tilcon/widget/tgsScrollBarPair.h"

class TScrollArea : public TControl
    {
public:
    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        TRT_S32                     trackWidth;
        TRT_S32                     buttonWidth;
        TScrollBarPair::ShowPolicy  horBarPolicy;
        TScrollBarPair::ShowPolicy  verBarPolicy;
        TCustomVisual               viewVisual;

        TCustomVisual               horBarTrackVisual;
        TCustomVisual               horBarThumbVisual;
        TCustomVisual               horBarLeftButtonVisual;
        TCustomVisual               horBarRightButtonVisual;
        TCustomVisual               verBarTrackVisual;
        TCustomVisual               verBarThumbVisual;
        TCustomVisual               verBarUpButtonVisual;
        TCustomVisual               verBarDownButtonVisual;
        };
public:

/*******************************************************************************
*
* TScrollArea - construct a scroll area
*
* SYNOPSIS
* \ss
* TScrollArea
*     (
*     TRT_S32 x = 0,            /@ x position @/
*     TRT_S32 y = 0,            /@ y position @/
*     TRT_S32 w = 0,            /@ width @/
*     TRT_S32 h = 0,            /@ height @/
*     TRT_S32 vw = 0,           /@ view width @/
*     TRT_S32 vh = 0            /@ view height @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a scroll bar widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* the position and size of the scroll area;
* \i <vw>, <vh>
* the width and height of the scroll view;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TScrollArea(TRT_S32 x =0, TRT_S32 y = 0, TRT_S32 w = 0, TRT_S32 h = 0,
        TRT_S32 vw = 0, TRT_S32 vh = 0);

/*******************************************************************************
*
* ~TScrollArea - destructs the scroll area
*
* This function destructs the scroll area widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TScrollArea();

/*******************************************************************************
*
* SetProperty - change the property of scroll area
*
* SYNOPSIS
* \ss
* void SetProperty
*     (
*     const Property & property /@ new property to apply @/
*     )
* \se
*
* DESCRIPTION
* This function changes the property of scroll area. This function is used to
* change multiple property items in one call. Frequently used property can be
* individually changed by dedicated APIs, e.g., text content can be changed by
* SetText(), and font can be changed by SetFont().
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetProperty(const Property &);

/*******************************************************************************
*
* GetProperty - get the property of scroll area
*
* SYNOPSIS
* \ss
* Property GetProperty() const
* \se
*
* DESCRIPTION
* This function gets the property of scroll area.
*
* RETURNS: the Property structure representing the scroll area's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* AddControl - add child control at specified Z-order position
*
* SYNOPSIS
* \ss
* TRT_STATUS AddControl
*     (
*     TControl *          child,  /@ child to be added @/
*     TRT_S32             index   /@ z-order of the added child @/
*     )
* \se
*
* DESCRIPTION
* This function adds child control at specified Z-order position.
*
* Parameters:
* \is
* \i <child>
* child to be added;
* \i <index>
* the insertion position. Possible value for <index> is between 0 and the number
* of children in the container. insertion position of 0 means the child will be
* added at bottom most Z-order; insertion position of the children count means
* the child will be added at the top most Z-order (it is OK to pass -1 as index,
* the function treats -1 as the top most Z-order); insertion value in between
* means to add the child at other Z-order between the bottom most and top most.
* \ie
*
* RETURNS: TRT_STATUS_OK if successful; otherwise TRT_STATUS_ERROR.
*
* ERRNO: N/A
*
* SEE ALSO: AddControlAtTop(), AddControlAtBottom(), RemoveControl(),
* RemoveAllControls()
*/
    TRT_STATUS AddControl(TControl * child, TRT_S32 index)
        {
        return basePanel.AddControl(child, index);
        }

/*******************************************************************************
*
* AddControlAtTop - add child control at top most Z-order
*
* SYNOPSIS
* \ss
* TRT_STATUS AddControlAtTop
*     (
*     TControl *    child   /@ child to be added @/
*     )
* \se
*
* DESCRIPTION
* This function adds child control at top most Z-order.
*
* Parameters:
* \is
* \i <child>
* child to be added;
* \ie
*
* RETURNS: TRT_STATUS_OK if successful; otherwise TRT_STATUS_ERROR.
*
* ERRNO: N/A
*
* SEE ALSO: AddControl(), AddControlAtBottom(), RemoveControl(),
* RemoveAllControls()
*/
    TRT_STATUS AddControlAtTop(TControl * child)
        {
        return basePanel.AddControlAtTop(child);
        }

/*******************************************************************************
*
* AddControlAtBottom - add child control at bottom most Z-order
*
* SYNOPSIS
* \ss
* TRT_STATUS AddControlAtBottom
*     (
*     TControl *    child  /@ child to be added @/
*     )
* \se
*
* DESCRIPTION
* This function adds child control at bottom most Z-order.
*
* Parameters:
* \is
* \i <child>
* child to be added;
* \ie
*
* RETURNS: TRT_STATUS_OK if successful; otherwise TRT_STATUS_ERROR.
*
* ERRNO: N/A
*
* SEE ALSO: AddControl(), AddControlAtTop(), RemoveControl(),
* RemoveAllControls()
*/
    TRT_STATUS AddControlAtBottom(TControl * child)
        {
        return basePanel.AddControlAtBottom(child);
        }

/*******************************************************************************
*
* RemoveControl - remove a specific control from the container
*
* SYNOPSIS
* \ss
* void RemoveControl
*     (
*     TControl * child    /@ child to be removed @/
*     )
* \se
*
* DESCRIPTION
* This function removes a specific control from the container. if the specified
* control is not a child of the container, this function does nothing.
*
* Parameters:
* \is
* \i <child>
* control to be removed.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: AddControl(), AddControlAtTop(), AddControlAtBottom(),
* RemoveAllControls()
*/
    void RemoveControl(TControl * child)
        {
        basePanel.RemoveControl(child);
        }

/*******************************************************************************
*
* RemoveAllControls - remove all child controls from the container
*
* SYNOPSIS
* \ss
* void RemoveAllControls()
* \se
*
* DESCRIPTION
* This function removes all child controls from the container.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: AddControl(), AddControlAtTop(), AddControlAtBottom(),
* RemoveControl()
*/
    void RemoveAllControls()
        {
        basePanel.RemoveAllControls();
        }

/*******************************************************************************
*
* Count - get the number of child controls in the container
*
* SYNOPSIS
* \ss
* TRT_S32 Count() const
* \se
*
* DESCRIPTION
* This function gets the number of child controls in the container.
*
* RETURNS: the number of child controls in the container
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 Count() const
        {
        return basePanel.Count();
        }

/*******************************************************************************
*
* operator - get the pointer to child control at specific Z-order index
*
* SYNOPSIS
* \ss
* TControl * operator []
*     (
*     TRT_S32 index   /@ Z-order index @/
*     ) const
* \se
*
* DESCRIPTION
* This function gets the pointer to child control at specific Z-order index.
* Z-order increases from screen towards outside, i.e., Z-order 0 means
* bottom-most, and Z-order of last child index means top-most.
*
* Parameters:
* \is
* \i <index>
* the Z-order index, valid value is between 0 and last child index;
* \ie
*
* RETURNS: pointer to child control; throws exception if index is out of range.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TControl * operator [] (TRT_S32 index) const
        {
        return basePanel[index];
        }

/*******************************************************************************
*
* IndexOf - get the Z-order index of specified child
*
* SYNOPSIS
* \ss
* TRT_S32 IndexOf
*     (
*     const TControl * child    /@ of which to get the index @/
*     ) const
* \se
*
* DESCRIPTION
* This function gets the Z-order index of specified child.
*
* Parameters:
* \is
* \i <child>
* the child whose Z-order index is to be retrieved;
* \ie
*
* RETURNS: the index of the child; if the child does not belong to this
* container, the return value is -1;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 IndexOf(const TControl * child) const
        {
        return basePanel.IndexOf(child);
        }

/*******************************************************************************
*
* SetScrollAreaSize - set the view size of the scroll area
*
* SYNOPSIS
* \ss
* TPoint SetScrollAreaSize
*     (
*     const TSize & size /@ new size @/
*     )
* \se
*
* DESCRIPTION
* This function sets the view size of the scroll area.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetScrollAreaSize()
*/
    void SetScrollAreaSize(const TSize & size);

/*******************************************************************************
*
* GetScrollAreaSize - get the view size of the scroll area
*
* SYNOPSIS
* \ss
* TSize GetScrollAreaSize() const
* \se
*
* DESCRIPTION
* This function gets the view size of the scroll area.
*
* RETURNS: the view size of the scroll area
*
* ERRNO: N/A
*
* SEE ALSO: SetScrollAreaSize()
*/
    TSize GetScrollAreaSize() const;

/*******************************************************************************
*
* SetScrollAreaPos - set the view position of the scroll area
*
* SYNOPSIS
* \ss
* TPoint SetScrollAreaPos
*     (
*     const TPoint & pt /@ new position @/
*     )
* \se
*
* DESCRIPTION
* This function sets the view position of the scroll area.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetScrollAreaPos()
*/
    void SetScrollAreaPos(const TPoint & pt);

/*******************************************************************************
*
* GetScrollAreaPos - get the view position of the scroll area
*
* SYNOPSIS
* \ss
* TPoint GetScrollAreaPos() const
* \se
*
* DESCRIPTION
* This function gets the view position of the scroll area.
*
* RETURNS: the view position of the scroll area
*
* ERRNO: N/A
*
* SEE ALSO: SetScrollAreaPos()
*/
    TPoint GetScrollAreaPos() const;

private:

    void UpdateAfterSizeChange();

    void BasePaintHandler(EventParam &);
    void HScrollHandler(EventParam &);
    void VScrollHandler(EventParam &);
    void TScrollViewportResizeHandler(EventParam &);

    TVisual             contentVisual;
    TVisual             viewVisual;

    TNodeEventDelegate  eventDelegate;
    TPanel              basePanel;
    TScrollBarPair      scrollBarPair;
    TNodeArray          scrollViewport;
    TNode &             scrollBase;
    };

#endif /* _INCLUDE_tgsScrollArea_H_ */

