/* tgsContainer.h - Tilcon control container class */

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
01jan14,yya  created
*/

/*
DESCRIPTION

This file defines the Tilcon's control container class.

TControlArray class defines methods to manage parent/children relationship among
Tilcon controls. TPanel and TGroup and two type of controls that directly derive
from TControlArray, thus have the ability to contain child controls.

TGroup is a container without visual element, it just groups children together.
TPanel also groups children, and it has a visible frame which will be rendered
on screen. If TPanel's visual frame type is set to FT_none, TPanel becomes
a functional equivalence to TGroup.

For all those containers, use AddControl(), AddControlAtTop(),
AddControlAtBottom() to add child; Use RemoveControl() to remove specific child;
Use RemoveAllControl() to remove all children; Count() and operator[] methods
are provided get children count and access individual child by index.

INCLUDE FILES: tilcon/engine/tgsContainer.h
*/

#ifndef _INCLUDE_tgsContainer_H_
#define _INCLUDE_tgsContainer_H_

#include "tilcon/engine/tgsControl.h"

class TControlArray
    {
public:
    TRT_STATUS AddControl(TControl * children, TRT_S32 index);
    TRT_STATUS AddControlAtTop(TControl * child);
    TRT_STATUS AddControlAtBottom(TControl * child);
    void RemoveControl(TControl * child);
    void RemoveAllControls();

    TRT_S32 Count() const
        {
        return children.Count();
        }
    TControl * operator [] (TRT_S32 index) const;
    TRT_S32 IndexOf(const TControl * child) const;

private:
    TArray<TControl *>  children;
    };

/* group class */
class TGroup : public TControl
    {
public:
/*******************************************************************************
*
* TGroup - construct a group widget
*
* SYNOPSIS
* \ss
* TGroup
*     (
*     TRT_S32 x,            /@ x position @/
*     TRT_S32 y,            /@ y position @/
*     TRT_S32 w,            /@ width @/
*     TRT_S32 h             /@ height @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TGroup with specified identifier, position and
* size.
*
* Parameters:
* \is
* \i <x>, <y>
* the position of the group;
* \i <w>, <h>
* the size of the group;
* \ie
*
* This constructor throws TException of type ET_no_application_instance if
* no TApplication instance is created yet. This constructor also throws
* std::bad_alloc on memory allocation failure.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TGroup
        (
        TRT_S32 x = 0,
        TRT_S32 y = 0,
        TRT_S32 w = 0,
        TRT_S32 h = 0
        );

/*******************************************************************************
*
* ~TGroup - destructs the group
*
* This function destructs the group.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TGroup();

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
    TRT_STATUS AddControlAtTop(TControl * child)
        {
        return AddControl(child, -1);
        }

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
    TRT_STATUS AddControlAtBottom(TControl * child)
        {
        return AddControl(child, 0);
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
    TRT_S32 Count() const
        {
        return container.Count();
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
        return container[index];
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
        return container.IndexOf(child);
        }

protected:
    static TRect CalculateBoundRect(const TRect &, const TRect &, const AlignmentPolicy &);

private:
    class GroupChildrenDelegate : public TChildrenDelegate
        {
        friend class TGroup;
    public:
        GroupChildrenDelegate(TGroup & p) :
            group(p)
            {}
        virtual TRT_S32 GetCount() const;
        virtual TNode & GetNode(TRT_S32);

    private:
        TGroup & group;
        };

    void NodeHandler(EventParam &);
    void OnResize();
    void OnChildBoundChange(TControl *);

    TNodeEventDelegate      eventDelegate;
    GroupChildrenDelegate   childrenDelegate;
    TNode                   boundNode;
    TControlArray           container;
    };

/* panel class */
class TPanel : public TGroup
    {
public:
/*******************************************************************************
*
* TPanel - class constructor
*
* SYNOPSIS
* \ss
* TPanel
*     (
*     TRT_S32 x,            /@ x position @/
*     TRT_S32 y,            /@ y position @/
*     TRT_S32 w,            /@ width @/
*     TRT_S32 h             /@ height @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TPanel with specified identifier, position and
* size.
*
* Parameters:
* \is
* \i <x>, <y>
* the position of the panel;
* \i <w>, <h>
* the size of the panel;
* \ie
*
* This constructor throws TException of type ET_no_application_instance if
* no TApplication instance is created yet. This constructor also throws
* std::bad_alloc on memory allocation failure.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPanel
        (
        TRT_S32 x = 0,
        TRT_S32 y = 0,
        TRT_S32 w = 0,
        TRT_S32 h = 0
        );

/*******************************************************************************
*
* ~TPanel - destructs the panel
*
* This function destructs the panel.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TPanel();

private:
    };

#endif /* _INCLUDE_tgsContainer_H_ */

