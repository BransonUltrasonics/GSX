/* tgsControl.h - Tilcon control class */

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

This file defines the Tilcon's TControl class. the TControl class is the basic
class to derive all other types of Tilcon UI controls.

The TControl class has a visible frame (includes the border and the are area).
The Tilcon Framework has defined various types of frame types for typical UI
elements, such as panel, button, check indicator etc, and the Framework will
draw these predefined frame type in a unified manner through the <TVisualEngine>
interface. If more personalized control appearance is required, TControl's
SetFrameVisual() method can be used to attach a <TCustomVisual> object that
will be used to customize the control frame's appearance.

The TControl class contains a <TextProperty> member, which will be used by the
derived controls to show their textual elements, such as the label of a button,
the items in a list, etc. The attributes in <TextProperty> can be individually
changed by the SetFont(), GetFont(), SetTextColor(), GetTextColor(),
GetTextBkColor(), GetTextBkColor(), SetTextHorizontalAlignment(),
GetTextHorizontalAlignment(), SetTextVerticalAlignment() and
GetTextVerticalAlignment() methods. The complete <TextProperty> member can be
changed by the SetTextProperty() and GetTextProperty() methods.

INCLUDE FILES: tilcon/engine/tgsControl.h
*/

#ifndef _INCLUDE_tgsControl_H_
#define _INCLUDE_tgsControl_H_

#include "tilcon/tilcon.h"
#include "tilcon/engine/tgsNode.h"
#include "tilcon/engine/tgsPainter.h"
#include "tilcon/engine/tgsTimer.h"
#include "tilcon/engine/tgsVisual.h"

#define DEFAULT_FRAME_MARGIN                (2)

/* property for text used in Tilcon control */
struct TextProperty
    {
    TextProperty() :
        color(TRT_COLOR_BLACK),
        horAlign(THA_left),
        verAlign(TVA_center)
        {}

    TRT_BOOL operator == (const TextProperty & p) const
        {
        return (font        == p.font &&
                color       == p.color &&
                horAlign    == p.horAlign &&
                verAlign    == p.verAlign);
        }

    TRT_BOOL operator != (const TextProperty & p) const
        {
        return (font        != p.font ||
                color       != p.color ||
                horAlign    != p.horAlign ||
                verAlign    != p.verAlign);
        }

    TFont                       font;
    TColor                      color;
    TextHorizontalAlignment     horAlign;
    TextVerticalAlignment       verAlign;
    };

class TControl;
struct ControlEventParam : public EventParam
    {
    TControl * sender;
    };

/* Tilcon's base class for control */
class TControl
    {
    friend class TNode;
    friend class TWindow;
public:
    /* parameter of the canvas's Paint Event */
    struct PaintEventParam : public ControlEventParam
        {
        PaintEventParam(TPainter & p) : painter(p)
            {}
        TPainter &  painter;
        };

    /* parameter for mouse event */
    struct MouseEventParam : public ControlEventParam
        {
        TRT_MOUSE_MSG   data;
        };

    /* parameter for keyboard event */
    struct KeyboardEventParam : public ControlEventParam
        {
        TRT_KEYBOARD_MSG    data;
        };

    /* parameter for resize event */
    struct ResizeEventParam : public ControlEventParam
        {
        TSize   size;
        };

    /* parameter for attach event */
    struct AttachEventParam : public ControlEventParam
        {
        };

    /* parameter for detach event */
    struct DetachEventParam : public ControlEventParam
        {
        };
public:
/*******************************************************************************
*
* TControl - constructs a control
*
* SYNOPSIS
* \ss
* TControl
*     (
*     TRT_S32 x,            /@ x position @/
*     TRT_S32 y,            /@ y position @/
*     TRT_S32 w,            /@ width @/
*     TRT_S32 h,            /@ height @/
*     TNode & node          /@ the associated node object @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a control with specified identifier, geometry and
* associated TNode object.
*
* Parameters:
* \is
* \i <x>, <y>
* the position of the control;
* \i <w>, <h>
* the size of the control;
* \i <node>
* the associated node object for the control;
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
    TControl
        (
        TRT_S32 x,
        TRT_S32 y,
        TRT_S32 w,
        TRT_S32 h,
        TNode & node
        );

/*******************************************************************************
*
* ~TControl - destructs the control
*
* This function destructs the control.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TControl();

/*******************************************************************************
*
* GetParent - get the parent control
*
* SYNOPSIS
* \ss
* TControl * GetParent () const
* \se
*
* DESCRIPTION
* This function gets the parent control.
*
* RETURNS: parent control
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TControl * GetParent() const
        {
        return parent;
        }

/*******************************************************************************
*
* SetPosition - set the position of the control
*
* SYNOPSIS
* \ss
* void SetPosition
*     (
*     const TPoint & pos    /@ new position to be set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the position of the control while maintaining its size.
* The position only takes effect when the fixed alignment policy is set,
* otherwise, the position of the control is managed by the framework, for
* example, the control is set to left, central or right aligned to its parent.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: SetSize(), SetRect(), GetPosition(), GetSize(), GetRect(),
* SetAlignmentPolicy(), GetAlignmentPolicy()
*/
    void SetPosition(const TPoint & pos);

/*******************************************************************************
*
* SetSize - set the size of the control
*
* SYNOPSIS
* \ss
* void SetSize
*     (
*     const TSize & size    /@ new size to be set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the size of the control while maintaining the position.
* The size only takes effect when the control's alignment policy is set to
* values other than to fill the parent area, otherwise, the control's size is
* decided by the dimension of its parent's content area.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: SetPosition(), SetRect(), GetPosition(), GetSize(), GetRect(),
* SetAlignmentPolicy(), GetAlignmentPolicy()
*/
    void SetSize(const TSize & size);

/*******************************************************************************
*
* SetRect - set the rectangle of the control
*
* SYNOPSIS
* \ss
* void SetRect
*     (
*     const TRect & rect    /@ new rect to be set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the rectangle of the node. It equals calling SetPosition()
* and SetSize() together.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: SetPosition(), SetSize(), GetPosition(), GetSize(), GetRect(),
* SetAlignmentPolicy(), GetAlignmentPolicy()
*/
    void SetRect(const TRect & rect);

/*******************************************************************************
*
* GetPosition - get the position of the control
*
* SYNOPSIS
* \ss
* TPoint GetPosition() const
* \se
*
* DESCRIPTION
* This function gets the position specified by SetPosition() or SetRect().
* The position may not equal the actually position of the control, depending on
* what type of alignment police is used, and whether layout manager is present.
* To get the actual position of the control, use the GetBoundRect() method.
*
* RETURNS: the position of the node.
*
* ERRNO: N/A
*
* SEE ALSO: SetPosition(), SetSize(), SetRect(), GetSize(), GetRect(),
* GetBoundRect()
*/
    TPoint GetPosition() const
        {
        return nodePosition;
        }

/*******************************************************************************
*
* GetSize - get the size of the control
*
* SYNOPSIS
* \ss
* TSize GetSize() const
* \se
*
* DESCRIPTION
* This function gets the size specified by SetSize() or SetRect().
* The returned size may not equal the actually size of the control, depending on
* what type of alignment police is used, and whether layout manager is present.
* To get the actual size of the node, use the GetBoundRect() method.
*
* RETURNS: the size of the node.
*
* ERRNO: N/A
*
* SEE ALSO: SetPosition(), SetSize(), SetRect(), GetPosition(), GetRect(),
* GetBoundRect()
*/
    TSize GetSize() const
        {
        return nodeSize;
        }

/*******************************************************************************
*
* GetRect - get the rectangle of the control
*
* SYNOPSIS
* \ss
* TRect GetRect() const
* \se
*
* DESCRIPTION
* This function gets the rectangle of the node specified by SetSize(),
* SetPosition() and SetRect();
* The returned rectangle may not equal the actually bounding rectangle of the
* control, depending on what type of alignment police is used, and whether the
* layout manager is present.
* to get the actual rectangle of the node, use the GetBoundRect() method.
*
* RETURNS: the rectangle of the node.
*
* ERRNO: N/A
*
* SEE ALSO: SetPosition(), SetSize(), SetRect(), GetPosition(), GetSize(),
* GetBoundRect()
*/
    TRect GetRect() const
        {
        return TRect(nodePosition, nodeSize);
        }

/*******************************************************************************
*
* SetAlignmentPolicy - set the alignment policy of the control
*
* SYNOPSIS
* \ss
* void SetAlignmentPolicy
*     (
*     HorizontalAlignment ha,   /@ horizontal alignment @/
*     VerticalAlignment va      /@ vertical alignment @/
*     )
* \se
*
* DESCRIPTION
* This function sets the alignment policy of the control, which is defined as:
*
* \cs
* enum HorizontalAlignment
*     {
*     HA_fixed = 0, /@ control is not aligned @/
*     HA_fill,      /@ control fills all space of the parent @/
*     HA_left,      /@ control is left aligned @/
*     HA_center,    /@ control is central aligned @/
*     HA_right,     /@ control is right aligned @/
*     };
*
* enum VerticalAlignment
*     {
*     VA_fixed = 0, /@ control is not aligned @/
*     VA_fill,      /@ control fills all space of the parent @/
*     VA_top,       /@ control is top aligned @/
*     VA_center,    /@ control is central aligned @/
*     VA_bottom,    /@ control is bottom aligned @/
*     };
* \ce
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetAlignmentPolicy()
*/
    void SetAlignmentPolicy
        (
        HorizontalAlignment ha,
        VerticalAlignment va
        );

/*******************************************************************************
*
* GetAlignmentPolicy - get the alignment policy of the control
*
* SYNOPSIS
* \ss
* const AlignmentPolicy & GetAlignmentPolicy() const
* \se
*
* DESCRIPTION
* This function gets the alignment policy of the control, which is defined as:
*
* RETURNS: alignment policy of the control
*
* ERRNO: N/A
*
* SEE ALSO: SetAlignmentPolicy()
*/
    const AlignmentPolicy & GetAlignmentPolicy() const
        {
        return alignmentPolicy;
        }

/*******************************************************************************
*
* SetSizeSource - set the method to size the control
*
* SYNOPSIS
* \ss
* void SetSizeSource
*     (
*     SizeSource value  /@ new size source to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the method to size the control.
* \is
* \i <SS_fixed>
* size is based on the fixed data specified using SetSize() and SetRect();
* \i <SS_auto>
* size is auto calculated by the size hint method of the control;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO: GetSizeSource(), GetHintedSize(), SetSize(), SetRect()
*/
    void SetSizeSource(SizeSource value);

/*******************************************************************************
*
* GetSizeSource - get the method to size the control
*
* SYNOPSIS
* \ss
* SizeSource GetSizeSource() const
* \se
*
* DESCRIPTION
* This function gets the method to size the control.
*
* RETURNS: the source to sizing the node.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO: SetSizeSource(), GetHintedSize()
*/
    SizeSource GetSizeSource() const
        {
        return sizeSource;
        }

/*******************************************************************************
*
* GetHintedSize - get the hinted size of the control
*
* SYNOPSIS
* \ss
* TSize GetHintedSize()
* \se
*
* DESCRIPTION
* This function gets the control's hinted size. This function invokes the
* control's size hint method to get the size;
*
* RETURNS: the hinted size of the node.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO: SetSizeSource()
*/
    TSize GetHintedSize();

/*******************************************************************************
*
* SetMargin - set the content margin of the control
*
* SYNOPSIS
* \ss
* void SetMargin
*     (
*     const TMargin & margin    /@ margin to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the content margin of the control. How the margin is used
* is up to the derived widget itself, however, generally margin area is used to
* show decorative frames or borders; all other contents of the control, such as
* visual elements, children, etc., shall be placed inside the content area.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetContentRect(), GetMargin()
*/
    void SetMargin(const TMargin & value)
        {
        if (innerMargin != value)
            {
            innerMargin = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetMargin - get the content margin of the control
*
* SYNOPSIS
* \ss
* const TMargin & GetMargin() const
* \se
*
* DESCRIPTION
* This function gets the content margin of the control.
*
* RETURNS: the content's rectangle.
*
* ERRNO: N/A
*
* SEE ALSO: GetContentRect(), SetMargin()
*/
    const TMargin & GetMargin() const
        {
        return innerMargin;
        }

/*******************************************************************************
*
* GetContentRect - get the content rectangle of the control
*
* SYNOPSIS
* \ss
* TRect GetContentRect() const
* \se
*
* DESCRIPTION
* This function gets the content's rectangle of the control. The content area is
* calculated by shrink the margin from the control's bounding rectangle.
* The content rectangle returned by this function is in the local coordinate
* system of the control itself.
*
* RETURNS: the content rectangle of the control.
*
* ERRNO: N/A
*
* SEE ALSO: GetBoundRect(), SetMargin(), GetMargin()
*/
    TRect GetContentRect() const
        {
        return boundNode.GetNormalizedBoundRect().Adjust(innerMargin);
        }

/*******************************************************************************
*
* GetBoundRect - get the actual bounding rectangle of the control
*
* SYNOPSIS
* \ss
* const TRect & GetBoundRect() const
* \se
*
* DESCRIPTION
* This function gets the actual bounding rectangle of the control. The actual
* bounding rectangle of the control is decided by the framework, based on
* factors such as the position and size of the control, its alignment, and
* whether the control is contained in a layout manager, in which case the
* control's bounding rectangle is given by the layout manager.
*
* RETURNS: the actual bounding rectangle.
*
* ERRNO: N/A
*
* SEE ALSO: SetPosition(), SetSize(), SetRect(), GetPosition(), GetSize(),
* GetRect()
*/
    const TRect & GetBoundRect() const
        {
        return boundNode.GetBoundRect();
        }

/*******************************************************************************
*
* SetEnabled - set the enable state of the control
*
* SYNOPSIS
* \ss
* void SetEnabled
*     (
*     TRT_BOOL value    /@ new enable state to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the control to be enabled or disabled. The actual enable
* state is determined by the enable state of the control itself and the enable
* state of all its parents. Disabled node is shown in dimmed state, and
* cannot receive user input message, thus cannot be operated. The default value
* for control's enable state is enabled.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetEnabled()
*/
    void SetEnabled
        (
        TRT_BOOL value
        )
        {
        boundNode.SetEnabled(value);
        }

/*******************************************************************************
*
* GetEnabled - get the enabled state of the control
*
* SYNOPSIS
* \ss
* TRT_BOOL GetEnabled() const
* \se
*
* DESCRIPTION
* This function gets the enabled state of the control.
*
* RETURNS: TRT_TURE if enabled; TRT_FALSE if disabled.
*
* ERRNO: N/A
*
* SEE ALSO: SetEnabled()
*/
    TRT_BOOL GetEnabled() const
        {
        return boundNode.GetEnabled();
        }

/*******************************************************************************
*
* SetVisible - set the visibility of the control
*
* SYNOPSIS
* \ss
* void SetVisible
*     (
*     TRT_BOOL value    /@ new visibility to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the visibility of the control. The actual visibility is
* determined by the visibility of the control itself and the visibility of all
* its parent. Invisible control is not rendered on the UI, and is skipped during
* input message dispatching. The default value for control's visibility is
* visible.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetEnabled()
*/
    void SetVisible(TRT_BOOL value)
        {
        boundNode.SetVisible(value);
        }

/*******************************************************************************
*
* GetEnabled - get the visibility of the node
*
* SYNOPSIS
* \ss
* TRT_BOOL GetVisible() const
* \se
*
* DESCRIPTION
* This function gets the visibility of the node.
*
* RETURNS: TRT_TURE if visible; TRT_FALSE if invisible.
*
* ERRNO: N/A
*
* SEE ALSO: SetEnabled()
*/
    TRT_BOOL GetVisible() const
        {
        return boundNode.GetVisible();
        }

/*******************************************************************************
*
* SetAllowMouseActivate - set whether to activate the control by mouse click
*
* SYNOPSIS
* \ss
* void SetAllowMouseActivate
*     (
*     TRT_BOOL value    /@ whether to activate the control by mouse click @/
*     )
* \se
*
* DESCRIPTION
* This function sets whether to activate the control by mouse click
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetAllowMouseActivate()
*/
    void SetAllowMouseActivate(TRT_BOOL value)
        {
        boundNode.SetAllowMouseActivate(value);
        }

/*******************************************************************************
*
* GetAllowMouseActivate - get whether to activate the control by mouse click
*
* SYNOPSIS
* \ss
* TRT_BOOL SetAllowMouseActivate() const
* \se
*
* DESCRIPTION
* This function gets whether to activate the control by mouse click
*
* RETURNS: whether to activate the control by mouse click
*
* ERRNO: N/A
*
* SEE ALSO: SetAllowMouseActivate()
*/
    TRT_BOOL GetAllowMouseActivate() const
        {
        return boundNode.GetAllowMouseActivate();
        }

/*******************************************************************************
*
* SetFont - set the text font of the control
*
* SYNOPSIS
* \ss
* void SetFont
*     (
*     const TFont & font /@ new font to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the text font of the control.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetFont()
*/
    void SetFont(const TFont & value)
        {
        if (textProperty.font != value)
            {
            textProperty.font = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetFont - get the text font of the control
*
* SYNOPSIS
* \ss
* const TFont & GetFont() const
* \se
*
* DESCRIPTION
* This function gets the text font of the control.
*
* RETURNS: text font of the control
*
* ERRNO: N/A
*
* SEE ALSO: SetFont()
*/
    const TFont & GetFont() const
        {
        return textProperty.font;
        }

/*******************************************************************************
*
* SetTextHorizontalAlignment - set the text horizontal alignment of the control
*
* SYNOPSIS
* \ss
* void SetTextHorizontalAlignment
*     (
*     TextHorizontalAlignment alignment /@ new alignment to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the text horizontal alignment of the control.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTextHorizontalAlignment()
*/
    void SetTextHorizontalAlignment(TextHorizontalAlignment value)
        {
        if (textProperty.horAlign != value)
            {
            textProperty.horAlign = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetTextHorizontalAlignment - get the text horizontal alignment of the control
*
* SYNOPSIS
* \ss
* TextHorizontalAlignment GetTextHorizontalAlignment() const
* \se
*
* DESCRIPTION
* This function gets the text horizontal alignment of the control.
*
* RETURNS: text horizontal alignment of the control
*
* ERRNO: N/A
*
* SEE ALSO: SetTextHorizontalAlignment()
*/
    TextHorizontalAlignment GetTextHorizontalAlignment() const
        {
        return textProperty.horAlign;
        }

/*******************************************************************************
*
* SetTextVerticalAlignment - set the text vertical alignment of the control
*
* SYNOPSIS
* \ss
* void SetTextVerticalAlignment
*     (
*     TextVerticalAlignment alignment /@ new alignment to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the text vertical alignment of the control.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTextVerticalAlignment()
*/
    void SetTextVerticalAlignment(TextVerticalAlignment value)
        {
        if (textProperty.verAlign != value)
            {
            textProperty.verAlign = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetTextVerticalAlignment - get the text vertical alignment of the control
*
* SYNOPSIS
* \ss
* TextVerticalAlignment GetTextVerticalAlignment() const
* \se
*
* DESCRIPTION
* This function gets the text vertical alignment of the control.
*
* RETURNS: text vertical alignment of the control
*
* ERRNO: N/A
*
* SEE ALSO: SetTextVerticalAlignment()
*/
    TextVerticalAlignment GetTextVerticalAlignment() const
        {
        return textProperty.verAlign;
        }

/*******************************************************************************
*
* SetTextColor - set the text color of the control
*
* SYNOPSIS
* \ss
* void SetTextColor
*     (
*     const TColor &  color   /@ new color to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the text color of the control.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTextColor()
*/
    void SetTextColor(const TColor & color)
        {
        if (textProperty.color != color)
            {
            textProperty.color = color;
            Update();
            }
        }

/*******************************************************************************
*
* GetTextColor - get the text color of the control
*
* SYNOPSIS
* \ss
* const TColor & GetTextColor() const
* \se
*
* DESCRIPTION
* This function gets the text color of the control.
*
* RETURNS: text color of the control
*
* ERRNO: N/A
*
* SEE ALSO: SetTextColor()
*/
    const TColor & GetTextColor() const
        {
        return textProperty.color;
        }

/*******************************************************************************
*
* SetTextProperty - set the text property of the control
*
* SYNOPSIS
* \ss
* void SetTextProperty
*     (
*     const TextProperty & value /@ new text property @/
*     )
* \se
*
* DESCRIPTION
* This function sets the text property of the control.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTextProperty()
*/
    void SetTextProperty(const TextProperty & value)
        {
        if (textProperty != value)
            {
            textProperty = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetTextProperty - get the text property of the control
*
* SYNOPSIS
* \ss
* const TextProperty &  GetTextProperty() const
* \se
*
* DESCRIPTION
* This function gets the text property of the control.
*
* RETURNS: the text property of the control
*
* ERRNO: N/A
*
* SEE ALSO: SetTextProperty()
*/
    const TextProperty &  GetTextProperty() const
        {
        return textProperty;
        }

/*******************************************************************************
*
* SetFrameVisual - set custom visual object of the control
*
* SYNOPSIS
* \ss
* void SetFrameVisual
*     (
*     const TCustomVisual & value /@ custom visual to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets custom visual object of the control
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetFrameVisual()
*/
    void SetFrameVisual
        (
        const TCustomVisual & value
        )
        {
        frameVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetFrameVisual - get custom visual object of the control
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetFrameVisual() const
* \se
*
* DESCRIPTION
* This function gets custom visual object of the control.
*
* RETURNS: the custom visual object of the control.
*
* ERRNO: N/A
*
* SEE ALSO: SetFrameVisual()
*/
    const TCustomVisual & GetFrameVisual() const
        {
        return frameVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetFrameType - set the frame type of the control
*
* SYNOPSIS
* \ss
* void SetFrameType
*       (
*       FrameType value     /@ new frame type to apply @/
*       )
* \se
*
* DESCRIPTION
* This function sets the frame type of the control.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetFrameType()
*/
    void SetFrameType(FrameType value)
        {
        frameVisual.SetFrameType(value);
        Update();
        }

/*******************************************************************************
*
* GetFrameType - get the frame type of the control
*
* SYNOPSIS
* \ss
* FrameType GetFrameType() const
* \se
*
* DESCRIPTION
* This function gets the frame type of the control.
*
* RETURNS: the frame type of the control
*
* ERRNO: N/A
*
* SEE ALSO: SetFrameType()
*/
    FrameType GetFrameType() const
        {
        return frameVisual.GetFrameType();
        }

/*******************************************************************************
*
* Update - update the appearance of the control
*
* SYNOPSIS
* \ss
* void Update()
* \se
*
* DESCRIPTION
* This function marks the entire area of this control to be dirty, so that the
* framework will issue an paint event to redrawn its content.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void Update()
        {
        boundNode.Update();
        }

/*******************************************************************************
*
* UpdateBoundRect - update the bounding rectangle of the control
*
* SYNOPSIS
* \ss
* void UpdateBoundRect()
* \se
*
* DESCRIPTION
* This function asks the framework to recalculate the bounding rectangle of the
* control. If the bounding rectangle is changed, a subsequent resize event will
* be issued to let the control to update the internal layout of itself.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void UpdateBoundRect()
        {
        boundNode.DetermineBoundRect(this);
        boundNode.UpdateBoundRect();
        }

protected:

    static TNode & ToNode(TControl & c)
        {
        return c.boundNode;
        }

    void OnControlAdded(TControl & c)
        {
        c.boundNode.AddToParent(&boundNode);
        c.parent = this;
        }

    void OnControlRemoved(TControl & c)
        {
        c.boundNode.RemoveFromParent();
        c.parent = TRT_NULL;
        }

    TMargin             innerMargin;
    TPoint              nodePosition;
    TSize               nodeSize;
    SizeSource          sizeSource;
    AlignmentPolicy     alignmentPolicy;

    TControl *          parent;
    TNode &             boundNode;

    TextProperty        textProperty;
    TVisual             frameVisual;
    };

#endif /* _INCLUDE_tgsControl_H_ */

