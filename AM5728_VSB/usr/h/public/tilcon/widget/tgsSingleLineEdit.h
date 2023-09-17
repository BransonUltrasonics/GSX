/* tgsSingleLineEdit.h - Tilcon single line edit widget */

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

This file defines the Tilcon's single line edit widget.

INCLUDE FILES: tilcon/engine/tgsSingleLineEdit.h
*/

#ifndef _INCLUDE_tgsSingleLineEdit_H_
#define _INCLUDE_tgsSingleLineEdit_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/engine/tgsTimer.h"
#include "tilcon/widget/tgsSingleLineEditNode.h"

class TSingleLineEdit : public TControl
    {
public:
    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        TRT_S32         maxTextLen;
        TMargin         margin;
        TCustomVisual   frameVisual;
        TextProperty    textProperty;
        };

public:
/*******************************************************************************
*
* TSingleLineEdit - construct a single line edit
*
* SYNOPSIS
* \ss
* TSingleLineEdit
*     (
*     TRT_S32 x = 0,                /@ x position @/
*     TRT_S32 y = 0,                /@ y position @/
*     TRT_S32 w = 0,                /@ width @/
*     TRT_S32 h = 0,                /@ height @/
*     TRT_S32 text = L"",           /@ text content @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a single line edit widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* The position and size of the widget;
* \i <text>
* default text to show in the single line edit;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TSingleLineEdit(TRT_S32 x = 0, TRT_S32 y = 0, TRT_S32 w = 0, TRT_S32 h = 0,
        const TWString & wstr = L"");

/*******************************************************************************
*
* ~TSingleLineEdit - destructs the single line edit
*
* This function destructs the single line edit.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TSingleLineEdit();

/*******************************************************************************
*
* SetProperty - set the property of the single line edit
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
* This function changes the property of the single line edit. This function is
* used to change multiple property items in one call. Frequently used property
* can be individually changed by dedicated APIs, e.g., the content can be
* changed by SetText(), and label font can be changed by SetFont().
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
* GetProperty - get the property of the single line edit
*
* SYNOPSIS
* \ss
* Property GetProperty() const;
* \se
*
* DESCRIPTION
* This function gets the property of the single line edit.
*
* RETURNS: the property of the single line edit
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* ClearText - clear the text content of the single line edit
*
* SYNOPSIS
* \ss
* void ClearText()
* \se
*
* DESCRIPTION
* This function clears the text content of the single line edit.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetText()
*/
    void ClearText()
        {
        editNode.ClearText();
        }

/*******************************************************************************
*
* SetText - set the text content of the single line edit
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
* This function sets the text content of the single line edit.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ClearText(), GetText()
*/
    void SetText(const TWString & value)
        {
        editNode.SetText(value);
        }

/*******************************************************************************
*
* GetText - get the text content of the single line edit
*
* SYNOPSIS
* \ss
* const TWString & GetText() const
* \se
*
* DESCRIPTION
* This function gets the text content of the single line edit
*
* RETURNS: content of the single line edit
*
* ERRNO: N/A
*
* SEE ALSO: SetText()
*/
    const TWString & GetText() const
        {
        return editNode.GetText();
        }

/*******************************************************************************
*
* SetTextFromFloat - set the text content from a fractional number
*
* SYNOPSIS
* \ss
* void SetTextFromFloat
*     (
*     TRT_FLOAT value /@ float value to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the text content from a fractional number.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetTextFromFloat(TRT_FLOAT);

/*******************************************************************************
*
* SetMaxTextLength - set the maximum text length
*
* SYNOPSIS
* \ss
* void SetMaxTextLength
*     (
*     TRT_S32 value /@ new maximum text length @/
*     )
* \se
*
* DESCRIPTION
* This function sets the maximum text length of the edit widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetMaxTextLength()
*/
    void SetMaxTextLength(TRT_S32 value)
        {
        editNode.SetMaxTextLength(value);
        }

/*******************************************************************************
*
* GetMaxTextLength - get the maximum text length
*
* SYNOPSIS
* \ss
* TRT_S32 GetMaxTextLength() const
* \se
*
* DESCRIPTION
* This function gets the maximum text length of the edit widget
*
* RETURNS: the mximum text length
*
* ERRNO: N/A
*
* SEE ALSO: SetMaxTextLength()
*/
    TRT_S32 GetMaxTextLength() const
        {
        return editNode.GetMaxTextLength();
        }

private:
    TEditNode       editNode;
    };

#endif /* _INCLUDE_tgsSingleLineEdit_H_ */

