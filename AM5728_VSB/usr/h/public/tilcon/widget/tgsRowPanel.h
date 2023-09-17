/* tgsRowPanel.h - Tilcon row panel widget */

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

This file defines Tilcon's row panel widget. Row panel is a layout container
that arranges child controls in rows.

INCLUDE FILES: tilcon/widget/tgsRowPanel.h
*/

#ifndef _INCLUDE_tgsRowPanel_H_
#define _INCLUDE_tgsRowPanel_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/engine/tgsContainer.h"

class TRowLayout
    {
public:
    void Reset();
    void SetWidth(TRT_S32);
    TRect GetRectForRow(TRT_S32);

private:
    TRT_S32 width;
    TRT_S32 height;
    };

class TRowPanel : public TControl
    {
public:
/*******************************************************************************
*
* TRowPanel - construct a row panel
*
* SYNOPSIS
* \ss
* TRowPanel
*     (
*     TRT_S32 x = 0,                                /@ x position @/
*     TRT_S32 y = 0,                                /@ y position @/
*     TRT_S32 w = 0,                                /@ width @/
*     TRT_S32 h = 0                                 /@ height @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a row panel widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* the position and size of the row panel;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRowPanel(TRT_S32 x = 0, TRT_S32 y = 0,
        TRT_S32 w = 0, TRT_S32 h = 0);

/*******************************************************************************
*
* ~TRowPanel - destructs the row panel
*
* This function destructs the row panel widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TRowPanel();

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
    TRT_STATUS AddControl(TControl * child, TRT_S32 index);

/*******************************************************************************
*
* AddControlAtTop - add child control at top most Z-order
*
* SYNOPSIS
* \ss
* TRT_STATUS AddControlAtTop
*     (
*     TControl *          child   /@ child to be added @/
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
    TRT_STATUS AddControlAtTop(TControl * child);

/*******************************************************************************
*
* AddControlAtBottom - add child control at bottom most Z-order
*
* SYNOPSIS
* \ss
* TRT_STATUS AddControlAtBottom
*     (
*     TControl *          child  /@ child to be added @/
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
    TRT_STATUS AddControlAtBottom(TControl * child);

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
    void RemoveControl(TControl * child);

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
    void RemoveAllControls();

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
    TRT_S32 Count() const;

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
    TControl * operator [] (TRT_S32 index) const;

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
    TRT_S32 IndexOf(const TControl * child) const;

private:
    class RowChildrenDelegate : public TChildrenDelegate
        {
        friend class TRowPanel;
    public:
        RowChildrenDelegate(TRowPanel & p) :
            panel(p)
            {}
        virtual TRT_S32 GetCount() const;
        virtual TNode & GetNode(TRT_S32);

    private:
        TRowPanel & panel;
        };

    void NodeHandler(EventParam &);
    void OnResize();
    void OnChildBoundChange(TControl *);
    void OnSizeHint(TNode::SizeHintEventParam &);
    void UpdateRowLayout(const TRect &);

    TNodeEventDelegate  eventDelegate;
    TNode               boundNode;
    RowChildrenDelegate childrenDelegate;
    TControlArray       container;
    };

#endif /* _INCLUDE_tgsRowPanel_H_ */

