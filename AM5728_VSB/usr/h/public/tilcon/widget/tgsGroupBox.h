/* tgsGroupBox.h - Tilcon group box class */

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

This file defines the Tilcon's group box class.

TGroupBox is a container class which holds children in a visible box, with an
optional label. TGroupBox provides the AddControl(), AddControlAtTop(),
AddControlAtBottom(), RemoveControl() and RemoveAllControls() methods to manage
the add/remove of the child. TGroupBox contains two visible parts: the grouping
box and the text label, which can be customized by SetBoxVisual() and
SetLabelVisual() methods.

INCLUDE FILES: tilcon/engine/tgsGroupBox.h
*/

#ifndef _INCLUDE_tgsGroupBox_H_
#define _INCLUDE_tgsGroupBox_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/engine/tgsContainer.h"

class TGroupBox : public TControl
    {
public:
    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        TMargin             frameMargin;
        TCustomVisual       frameVisual;

        TRT_S32             headerHeight;
        TMargin             headerMargin;
        TMargin             boxMargin;
        TMargin             groupMargin;

        TCustomVisual       headerVisual;
        TCustomVisual       boxVisual;

        TextProperty        textProperty;
        };
public:
/*******************************************************************************
*
* TGroupBox - construct a group box
*
* SYNOPSIS
* \ss
* TGroupBox
*     (
*     TRT_S32 x = 0,                /@ x position @/
*     TRT_S32 y = 0,                /@ y position @/
*     TRT_S32 w = 0,                /@ width @/
*     TRT_S32 h = 0,                /@ height @/
*     const TWString & label = L""  /@ group box label @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a group box widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* The position and size of the group box;
* \i <label>
* label of the group box;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TGroupBox(TRT_S32 x = 0, TRT_S32 y = 0,
        TRT_S32 w = 0, TRT_S32 h = 0, const TWString & _label = L"");

/*******************************************************************************
*
* ~TGroupBox - destructs the group box
*
* DESCRIPTION
* This function destructs the group box widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TGroupBox();

/*******************************************************************************
*
* SetProperty - change the property of group box
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
* This function changes the property of group box. This function is used to
* change multiple property items in one call. Frequently used property can be
* individually changed by dedicated APIs, e.g., label content can be changed by
* SetText(), and label font can be changed by SetFont().
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetProperty()
*/
    void SetProperty(const Property &);

/*******************************************************************************
*
* GetProperty - get the property of group box
*
* SYNOPSIS
* \ss
* Property GetProperty() const
* \se
*
* DESCRIPTION
* This function gets the property of group box.
*
* RETURNS: the Property structure representing the widget's property
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
        return group.AddControl(child, index);
        }

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
        return group.AddControlAtTop(child);
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
        return group.AddControlAtBottom(child);
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
        group.RemoveControl(child);
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
        group.RemoveAllControls();
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
        return group.Count();
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
        return group[index];
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
        return group.IndexOf(child);
        }

/*******************************************************************************
*
* SetText - set label content of group box
*
* SYNOPSIS
* \ss
* void SetText
*     (
*     const TWString & string /@ new text to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the label content of group box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetText()
*/
    void SetText
        (
        const TWString & value
        )
        {
        label = value;
        UpdateBoundRect();
        }

/*******************************************************************************
*
* GetText - get label content of group box
*
* SYNOPSIS
* \ss
* const TWString & GetText() const
* \se
*
* DESCRIPTION
* This function gets the label content of group box.
*
* RETURNS: TWString as the label content
*
* ERRNO: N/A
*
* SEE ALSO: SetText()
*/
    const TWString & GetText() const
        {
        return label;
        }

/*******************************************************************************
*
* SetHeaderVisual - set the custom visual of the group box header
*
* SYNOPSIS
* \ss
* void SetHeaderVisual
*     (
*     const TCustomVisual & value   /@ custom visual to be applied @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual of the group box header.
*
* RETURNS: N/A;
*
* ERRNO: N/A
*
* SEE ALSO: GetHeaderVisual()
*/
    void SetHeaderVisual(const TCustomVisual & value)
        {
        headerVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetHeaderVisual - get the custom visual of the group box header
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetHeaderVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual of the group box header.
*
* RETURNS: N/A;
*
* ERRNO: N/A
*
* SEE ALSO: SetHeaderVisual()
*/
    const TCustomVisual & GetHeaderVisual() const
        {
        return headerVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetBoxVisual - set the custom visual of the grouping box
*
* SYNOPSIS
* \ss
* void SetBoxVisual
*     (
*     const TCustomVisual & value   /@ custom visual to be applied @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual of the grouping box.
*
* RETURNS: N/A;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetBoxVisual(const TCustomVisual & value)
        {
        boxVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetBoxVisual - get the custom visual of the grouping box
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetBoxVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual of the grouping box.
*
* RETURNS: N/A;
*
* ERRNO: N/A
*
* SEE ALSO: SetBoxVisual()
*/
    const TCustomVisual & GetBoxVisual() const
        {
        return boxVisual.GetCustomVisual();
        }

private:
    void NodeHandler(EventParam &);
    void ResizeHandler(EventParam &);

    TWString            label;
    TRT_S32             headerHeight;
    TMargin             headerMargin;
    TMargin             boxMargin;
    TMargin             groupMargin;

    TVisual             headerVisual;
    TVisual             boxVisual;

    TNodeEventDelegate  eventDelegate;
    TNodeArray          boundNode;
    TGroup              group;
    };

#endif /* _INCLUDE_tgsGroupBox_H_ */

