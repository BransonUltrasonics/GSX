/* tgsLabel.h - Tilcon label class */

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

This file defines the Tilcon's label class.

INCLUDE FILES: tilcon/engine/tgsLabel.h
*/

#ifndef _INCLUDE_tgsLabel_H_
#define _INCLUDE_tgsLabel_H_

#include "tilcon/engine/tgsControl.h"

class TLabel : public TControl
    {
public:
    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        TMargin         margin;
        TCustomVisual   frameVisual;
        TextProperty    textProperty;
        };

public:
/*******************************************************************************
*
* TLabel - construct a label
*
* SYNOPSIS
* \ss
* TLabel
*     (
*     TRT_S32 x = 0,                /@ x position @/
*     TRT_S32 y = 0,                /@ y position @/
*     TRT_S32 w = 0,                /@ width @/
*     TRT_S32 h = 0,                /@ height @/
*     const TWString & label = L""  /@ label content @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a label widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* The position and size of the label;
* \i <label>
* label content;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TLabel(TRT_S32 x = 0, TRT_S32 y = 0,
        TRT_S32 w = 0, TRT_S32 h = 0, const TWString & str = L"");

/*******************************************************************************
*
* ~TLabel - destructs the label
*
* DESCRIPTION
* This function destructs the label widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TLabel();

/*******************************************************************************
*
* SetProperty - set the property of the label widget
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
* This function changes the property of the label. This function is used to
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
* GetProperty - get the property of the label widget
*
* SYNOPSIS
* \ss
* Property GetProperty() const;
* \se
*
* DESCRIPTION
* This function gets the property of the label.
*
* RETURNS: the Property structure representing the label's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* SetText - set the text content of the label widget
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
* This function sets the text content of the label widget.
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
        text = value;
        UpdateBoundRect();
        }

/*******************************************************************************
*
* GetText - get the text content of the label widget
*
* SYNOPSIS
* \ss
* const TWString & GetText() const
* \se
*
* DESCRIPTION
* This function gets the text content of the label widget
*
* RETURNS: TWString as the label content
*
* ERRNO: N/A
*
* SEE ALSO: SetText()
*/
    const TWString & GetText() const
        {
        return text;
        }

private:
    void NodeHandler(EventParam &);
    void OnPaint(TNode::PaintEventParam &);

    TNodeEventDelegate  eventDelegate;
    TNode               boundNode;

    TWString            text;
    };

#endif /* _INCLUDE_tgsLabel_H_ */

