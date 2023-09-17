/* tgsWindow.h - Tilcon window class */

/*
 * Copyright (c) 2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
14mar16,jnl  support window surface (F6570)
29jul15,c_l  support circle,ellipse and arc (V7GFX-247)
01jan14,yya  created (US15550)
*/

/*
DESCRIPTION

This file defines the Tilcon's window class. Tilcon's window class is the top
level container to host Tilcon widget.

INCLUDE FILES: tilcon/engine/tgsWindow.h
*/

#ifndef _INCLUDE_tgsWindow_H_
#define _INCLUDE_tgsWindow_H_

#include "tilcon/tilcon.h"
#include "tilcon/engine/tgsEventHandler.h"
#include "tilcon/engine/tgsControl.h"
#include "tilcon/engine/tgsContainer.h"


class TSurface;
class TNode;
class TWSWindow;
class TWindowHost
    {
    friend class TWindow;
    friend class TWSHost;
    friend class TApplication;
public:
    struct HostEventParam : public EventParam
        {
        TWindowHost *   host;
        };

    struct InvalidateEventParam : public HostEventParam
        {
        TRect   rect;
        };

    struct HostToScreenEventParam : public HostEventParam
        {
        TPoint  point;
        };

    struct GrabMouseEventParam : public HostEventParam
        {
        TRT_BOOL    grab;
        };

    TEvent  invalidateEvent;
    TEvent  mouseGrabEvent;
    TEvent  toScreenEvent;

public:
    TWindowHost(int w, int h);
    virtual ~TWindowHost();

    void InvalidateRect(const TRect & rect);
    TPoint HostToScreen(const TPoint & pt);

    TNode * GetTopNode()
        {
        return static_cast<TNode *>(topNode);
        }

    void SetMouseHookNode(TNode * node)
        {
        mouseHookNode = node;
        }

    TNode * GetActiveChild()
        {
        return activeChild;
        }
    void ActivateChild(TNode * child);
    void DeactivateChild(TNode * child);
    void UpdateMouseTrack();
    void ResetMouseGrab()
        {
        mouseGrabNode = TRT_NULL;
        }

private:
    void RouteMouseEvent(TNode * target,TNode::MouseEventParam &);
    void RouteMouseActivateEvent(TNode * target);
    void ProcessMouseTrack(TNode * object);
    void ClearMouseTrack();
    TNode * FindMouseResponsiveNodeAt(const TPoint & pt);

    void OnPaint(TPainter & painter, const TRect & rect);

    void WindowEventHandler(EventParam &);

    void OnMouse(EventParam &);
    void OnKeyboard(EventParam &);
    void OnResize(EventParam &);
    void OnMouseLeave(EventParam &);
    void OnActivate(EventParam &);
    void OnDeactivate(EventParam &);
    void OnShow(EventParam &);
    void OnHide(EventParam &);
    void Attach();
    void Detach();

    TRT_BOOL        isHosted;

    TNode *         topNode;
    TNode *         activePopup;
    TNode *         activeChild;

    TArray<TNode *> mouseHoverList;
    TNode *         mouseGrabNode;
    TNode *         mouseHookNode;

    TRT_BOOL        processMouseTrack;
    TPoint          mousePt;
    };

class TWindow
    {
    friend class TPainter;
public:
    /* window type */
    enum WindowType
        {
        WT_normal = 0,  /* standard frame window to host User Interface */
        WT_tool         /* none-active window used in tool-box, menu, etc.*/
        };

    /* window sizing */
    enum WindowSizing
        {
        WS_normal = 0,      /* fixed size */
        WS_maximized,       /* maximize to screen in both dimensions */
        };

    /* window event type */
    enum WindowEventType
        {
        WE_general = 0,
        WE_resize,
        WE_paint,
        WE_mouse,
        WE_mouse_enter,
        WE_mouse_leave,
        WE_keyboard,
        WE_activate,
        WE_deactivate,
        WE_create,
        WE_destroy,
        };

    /* window event parameter */
    struct WindowEventParam : public EventParam
        {
        WindowEventParam(WindowEventType t, TWindow * s) :
            type(t), sender(s), processed(0)
            {
            }
        WindowEventType type;
        TWindow *       sender;
        TRT_BOOL        processed;
        };

    /* mouse event parameter */
    struct MouseEventParam : public WindowEventParam
        {
        MouseEventParam(TWindow * s, TRT_MOUSE_MSG & _mouse,
            TRT_DRAG_MSG & _ldrag, TRT_DRAG_MSG & _rdrag) :
            WindowEventParam(WE_mouse, s),
            mouse(_mouse), ldrag(_ldrag), rdrag(_rdrag)
            {
            }
        TRT_MOUSE_MSG & mouse;
        TRT_DRAG_MSG & ldrag;
        TRT_DRAG_MSG & rdrag;
        };

    /* keyboard event parameter */
    struct KeyboardEventParam : public WindowEventParam
        {
        KeyboardEventParam(TWindow * s, TRT_KEYBOARD_MSG  & _keyboard) :
            WindowEventParam(WE_keyboard, s),
            keyboard(_keyboard)
            {
            }
        TRT_KEYBOARD_MSG  & keyboard;
        };

    /* paint event parameter */
    struct PaintEventParam : public WindowEventParam
        {
        PaintEventParam(TWindow * s, const TRect & r) :
            WindowEventParam(WE_paint, s),
            target(s), dirtyRect(r)
            {}
        TWindow *           target;
        TRect               dirtyRect;
        };

    /* resize event parameter */
    struct ResizeEventParam : public WindowEventParam
        {
        ResizeEventParam(TWindow * s) :
            WindowEventParam(WE_resize, s)
            {}
        TSize   size;
        };

public:

/*******************************************************************************
*
* TWindow - construct a window
*
* SYNOPSIS
* \ss
* TWindow
*     (
*     TRT_S32 x,                        /@ x position @/
*     TRT_S32 y,                        /@ y position @/
*     TRT_S32 w,                        /@ width @/
*     TRT_S32 h,                        /@ height @/
*     TWindow * parent = TRT_NULL,      /@ parent @/
*     WindowSizing sizing = WS_normal,  /@ sizing method @/
*     WindowType type = WT_normal       /@ window type @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a Tilcon window.
*
* Parameters:
* \is
* \i <x>, <y>
* the position of the window;
* \i <w>, <h>
* the size of the group;
* \i <sizing>
* the window's sizing method;
* \i <type>
* the type of the window;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TWindow
        (
        TRT_S32 x = 0,
        TRT_S32 y = 0,
        TRT_S32 w = 0,
        TRT_S32 h = 0,
        TWindow * parent = TRT_NULL,
        WindowSizing _sizing = WS_normal,
        WindowType _type = WT_normal
        );

/*******************************************************************************
*
* ~TWindow - destruct the window
*
* This function destructs the window.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TWindow();

/*******************************************************************************
*
* Create - create the window
*
* SYNOPSIS
* \ss
* TRT_STATUS Create()
* \se
*
* DESCRIPTION
* This function creates the window. Unlike the TWindow's constructor, which only
* allocates data and sets them to proper value, this function actually requests
* the underlying Window System to create the native window.
* Explicit calling of Create() can be omitted; instead, the Show() method will
* call Create() implicitly if Create() method is not called yet.
*
* RETURNS: TRT_STATUS_OK if the window creation is successful,
* otherwise TRT_STATUS_ERROR.
*
* ERRNO: N/A
*
* SEE ALSO: Destroy(), Show(), Hide()
*/
    TRT_STATUS Create();

/*******************************************************************************
*
* Destroy - destroy the window
*
* SYNOPSIS
* \ss
* void Destroy()
* \se
*
* DESCRIPTION
* This function requests the underlying native Window System to destroy the
* native window associated with this TWindow instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: Create(), Show(), Hide()
*/
    void Destroy();

/*******************************************************************************
*
* Show - show the window
*
* SYNOPSIS
* \ss
* TRT_STATUS Show
*     (
*     TRT_BOOL  show    /@ show or hide the window @/
*     )
* \se
*
* DESCRIPTION
* This function requests the underlying native Window System to show or hide
* the native window associated with this TWindow instance. If the Create() is
* not called prior to this call, then it will implicitly call Create() to first
* create the underlying window.
*
* if <show> is TRT_TRUE, the function reveals the window.
* if <show> is TRT_FALSE, the function hides the window.
*
* RETURNS: TRT_STATUS_OK if operation completes successfully.
* TRT_STATUS_ERROR if window is not created yet and the attempt to create
* the window is failed.
*
* ERRNO: N/A
*
* SEE ALSO: Create(), Destroy(), Hide()
*/
    TRT_STATUS Show
        (
        TRT_BOOL show = TRT_TRUE
        );

/*******************************************************************************
*
* Hide - hide the window
*
* SYNOPSIS
* \ss
* TRT_STATUS Hide()
* \se
*
* DESCRIPTION
* This function requests the underlying native Window System to hide
* the native window associated with this TWindow instance. If the Create() is
* not called prior to this call, then it will implicitly call Create() to first
* create the underlying window.
*
* RETURNS: TRT_STATUS_OK if operation completes successfully.
* TRT_STATUS_ERROR if window is not created yet and the attempt to create
* the window is failed.
*
* ERRNO: N/A
*
* SEE ALSO: Create(), Destroy(), Show()
*/
    TRT_STATUS Hide();

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
    TRT_STATUS AddControl
        (
        TControl * child,
        TRT_S32 index
        )
        {
        return window.AddControl(child, index);
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
    TRT_STATUS AddControlAtTop
        (
        TControl * child
        )
        {
        return window.AddControlAtTop(child);
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
    TRT_STATUS AddControlAtBottom
        (
        TControl * child
        )
        {
        return window.AddControlAtBottom(child);
        }

/*******************************************************************************
*
* RemoveControl - remove a specific control from the window
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
* This function removes a specific control from the window. if the specified
* control is not a child of the window, this function does nothing.
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
    void RemoveControl
        (
        TControl * child
        )
        {
        window.RemoveControl(child);
        }

/*******************************************************************************
*
* RemoveAllControls - remove all child controls from the window
*
* SYNOPSIS
* \ss
* void RemoveAllControls()
* \se
*
* DESCRIPTION
* This function removes all child controls from the window.
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
        window.RemoveAllControls();
        }

/*******************************************************************************
*
* GetType - get the type of the window
*
* SYNOPSIS
* \ss
* WindowType GetType() const
* \se
*
* DESCRIPTION
* This function gets the type of the window.
*
* RETURNS: window type.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    WindowType GetType() const
        {
        return type;
        }

/*******************************************************************************
*
* SetWindowRect - set the bounding rectangle of the window
*
* SYNOPSIS
* \ss
* void SetWindowRect
*     (
*     const TRect & rect    /@ new bounding rectangle to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the bounding rectangle of the window, in screen's
* coordinate system.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetWindowRect()
*/
    void SetWindowRect(const TRect & rect);

/*******************************************************************************
*
* GetWindowRect - get the bounding rectangle of the window
*
* SYNOPSIS
* \ss
* TRect GetWindowRect() const
* \se
*
* DESCRIPTION
* This function returns the bounding rectangle of the window.
*
* RETURNS: the bounding rectangle, in screen's coordinate system.
*
* ERRNO: N/A
*
* SEE ALSO: SetWindowRect()
*/
    TRect GetWindowRect() const;

/*******************************************************************************
*
* SetFrameType - set the frame type of the window
*
* SYNOPSIS
* \ss
* void SetFrameType
*     (
*     FrameType type    /@ new frame type to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the frame type of this window. The default value of the
* window's frame type is FT_panel_without_border, which renders the window
* area as a GUI panel without border.
* The frame type of the window only affects the visual appearance of the window;
* other than using frame type to change the appearance, user can use the
* SetFrameVisual() method to apply customized visual style to the window.
* If the frame type or the applied custom visual appearance contains the border
* part, to avoid the child widget to appear over the border, user can call the
* SetMargin() method to set the border margin of the window, so that child
* widget's placement will exclude the margin area.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetFrameType(), SetFrameVisual(), SetMargin()
*/
    void SetFrameType
        (
        FrameType type
        )
        {
        window.SetFrameType(type);
        }

/*******************************************************************************
*
* GetFrameType - get the frame type of the window
*
* SYNOPSIS
* \ss
* FrameType GetFrameType() const
* \se
*
* DESCRIPTION
* This function gets the frame type of this window.
*
* RETURNS: frame type of the window.
*
* ERRNO: N/A
*
* SEE ALSO: SetFrameType()
*/
    FrameType GetFrameType() const
        {
        return window.GetFrameType();
        }

/*******************************************************************************
*
* SetMargin - set the margin of the window
*
* SYNOPSIS
* \ss
* void SetMargin
*     (
*     const TMargin & value    /@ new margin to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the margin of the window. The default value for window's
* margin is (0, 0, 0, 0), which mean child control's coordinate begins at the
* very upper left corner of the window.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetMargin()
*/
    void SetMargin
        (
        const TMargin & value
        )
        {
        window.SetMargin(value);
        }

/*******************************************************************************
*
* GetMargin - get the margin of the window
*
* SYNOPSIS
* \ss
* const TMargin & GetMargin() const
* \se
*
* DESCRIPTION
* This function gets the margin of the window.
*
* RETURNS: margin of the window.
*
* ERRNO: N/A
*
* SEE ALSO: SetMargin()
*/
    const TMargin & GetMargin() const
        {
        return window.GetMargin();
        }

/*******************************************************************************
*
* SetFrameVisual - set the custom visual of the window frame
*
* SYNOPSIS
* \ss
* void SetFrameVisual
*     (
*     const TCustomVisual & value    /@ custom visual to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual of the window frame.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetFrameVisual
        (
        const TCustomVisual & value
        )
        {
        window.SetFrameVisual(value);
        }

/*******************************************************************************
*
* SetTrackMouse - set whether to track mouse move within this window
*
* SYNOPSIS
* \ss
* void SetTrackMouse
*     (
*     TRT_BOOL value    /@ whether to track mouse move in this window @/
*     )
* \se
*
* DESCRIPTION
* This function sets whether to track mouse move within this window. Enable
* mouse track so that the window will route Mouse Enter & Mouse Leave messages
* to child control in response to mouse movement. If mouse track is disabled,
* the window no longer tracks the mouse and will not route Mouse Enter & Mouse
* Leave messages.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetTrackMouse
        (
        TRT_BOOL value
        )
        {
        windowHost.processMouseTrack = value;
        }

/*******************************************************************************
*
* InvalidateRect - mark an area dirty in the window
*
* This function marks an rectangle area to be dirty, so as to let the framework
* to redraw this area in next refresh interval.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    void InvalidateRect
        (
        const TRect & rect
        );

    TEvent & GetWindowEvent()
        {
        return windowEvent;
        }

    void SwapWindow();
    TRT_BYTE * LockWindow();
    void UnlockWindow();

private:
    friend class TApplication;

    void WindowEventHandler(EventParam &);
    void InvalidateHandler(EventParam &);
    void ToScreenHandler(EventParam &);

    TEvent              windowEvent;

    TWindowHost         windowHost;
    TPanel              window;
    WindowSizing        sizing;
    WindowType          type;
    TRect               fixedRect;
    TRect               dirtyRect;
    TRT_BOOL            dirty;

    TWSWindow *         twsWindow;
    TRT_HANDLE          surface;
    TRT_HANDLE          buffer;

    static TWindow *    root;
    };


#endif /* _INCLUDE_tgsWindow_H_ */

