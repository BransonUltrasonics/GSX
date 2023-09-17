/* tgsNode.h - Tilcon node class */

/*
 * Copyright (c) 2014-2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
06jan17,jnl  code clean
14mar16,jnl  support window surface (F6570)
23oct15,c_l  IA64 warning reduction
29jul15,c_l  support circle,ellipse and arc (V7GFX-247)
01jan14,yya  created (US15550)
*/

/*
DESCRIPTION

This file defines several node types. There nodes together forms the Tilcon's
hierarchy, which is used to organize all UI elements in parent-child
relationship, and to route messages among them.

INCLUDE FILES: tilcon/basic/tgsNode.h
*/

#ifndef _INCLUDE_tgsNode_H_
#define _INCLUDE_tgsNode_H_

#include "tilcon/tilcon.h"
#include "tilcon/engine/tgsEventHandler.h"


class TNode;
class TControl;
class TPainter;
class TWindowHost;
class TPaintDevice;
class TChildrenDelegate;
class TEventDelegate;

/* message used by nodes */

typedef void (*EventCallback)(TNode &, TRT_PVOID);

/* node event type */
enum NodeEventType
    {
    ET_general = 0,
    ET_mouse,
    ET_mouse_enter,
    ET_mouse_leave,
    ET_mouse_activate,
    ET_keyboard,
    ET_resize,
    ET_size_hint,
    ET_paint,
    ET_gain_focus,
    ET_lose_focus,
    ET_attach,
    ET_detach,
    ET_bound_change,
    ET_child_bound_change,
    };

/* node event parameter */
struct NodeEventParam : public EventParam
    {
    NodeEventType   type;
    TNode *         sender;
    };

/* Tilcon's base node class */
class TNode
    {
    friend class TControl;
    friend class TWSHost;
public:

    /* mouse event parameter */
    struct MouseEventParam : public NodeEventParam
        {
        MouseEventParam()
            {
            data.event.value = 0;
            data.state.value = 0;
            data.x = 0;
            data.y = 0;
            ldrag.ox = 0;
            ldrag.oy = 0;
            ldrag.state = DS_none;
            rdrag.ox = 0;
            rdrag.oy = 0;
            rdrag.state = DS_none;
            }
        TRT_MOUSE_MSG   data;
        TRT_DRAG_MSG    ldrag;
        TRT_DRAG_MSG    rdrag;
        };

    /* paint event parameter */
    struct PaintEventParam : public NodeEventParam
        {
        PaintEventParam(TPainter & p, const TPoint & o, const TRect & b) :
            painter(p), origin(o), bound(b)
            {}
        TPainter &          painter;
        TPoint              origin;
        TRect               bound;
        };

    /* resize event parameter */
    struct ResizeEventParam : public NodeEventParam
        {
        TSize   size;
        };

    /* keyboard event parameter */
    struct KeyboardEventParam : public NodeEventParam
        {
        TRT_KEYBOARD_MSG    data;
        };

    /* size hint event parameter */
    struct SizeHintEventParam : public NodeEventParam
        {
        SizeHintEventParam() : hinted(TRT_FALSE)
            {}
        TSize       size;
        TRT_BOOL    hinted;
        };

    /* attach event parameter */
    struct AttachEventParam : public NodeEventParam
        {
        };

    /* detach event parameter */
    struct DetachEventParam : public NodeEventParam
        {
        };

    /* hit test event parameter */
    struct HitTestEventParam : public NodeEventParam
        {
        HitTestEventParam(const TPoint & _pos) :
            pos(_pos),
            hit(TRT_TRUE)
            {}
        TPoint pos;
        TRT_BOOL hit;
        };

    /* child bound change event parameter */
    struct ChildBoundChangeEventParam : public NodeEventParam
        {
        TControl * control;
        };

public:
    TNode(TRT_S32 x, TRT_S32 y, TRT_S32 w, TRT_S32 h,
        TEventDelegate* e = TRT_NULL, TChildrenDelegate * b = TRT_NULL);
    virtual ~TNode();

public:
/*******************************************************************************
*
* GetParent - get the parent node
*
* This function gets the parent node of this node in the node hierarchy.
*
* RETURNS: pointer to parent node, or TRT_NULL if node is not added to any
* parent yet or the node is the top-most in the hierarchy;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TNode * GetParent() const
        {
        return parent;
        }

/*******************************************************************************
*
* OffsetToWindow - translate node coordinate into window coordinates system
*
* This function translates a node-based coordinate to the coordinate under
* the window-based coordinates system. For example, if the node is placed at
* (100, 200) in the window, then a node-based (50, 50) is (150, 250) in the
* window-based coordinates system.
*
* <coords> is the node-based coordinates to be translated;
*
* RETURNS: the coordinate in window-based coordinates system. If the node is
* not attached to any window, the input point is returned unchanged.
*
* ERRNO: N/A
*
* SEE ALSO: OffsetToScreen
*/
    TPoint OffsetToWindow
        (
        const TPoint & coords
        ) const;

/*******************************************************************************
*
* OffsetToScreen - translate node coordinate into screen coordinates system
*
* This function translates a node-based coordinate to the coordinate under
* the screen-based coordinates system. For example, if the node is placed at
* (100, 200) to the window, and the window is in turn placed at (100, 100) in
* the screen, then a node-based (50, 50) is (250, 350) in the screen-based
* coordinates system.
*
* <coords> is the node-based coordinates to be translated;
*
* RETURNS: the coordinate in screen-based coordinates system. If the node is
* not attached to any window, the input point is returned unchanged.
*
* ERRNO: N/A
*
* SEE ALSO: OffsetToWindow
*/
    TPoint OffsetToScreen
        (
        const TPoint & coords
        ) const;

/*******************************************************************************
*
* UpdateBoundRect - updates the node's geometry
*
* This function notifies the framework to issues the Resize Event so that the
* node can plan its layout under the new bounding rectangle.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void UpdateBoundRect()
        {
        if (attachedHost)
            {
            OnResize();
            Update();
            }
        }

/*******************************************************************************
*
* Update - request an update for the node
*
* This function marks the entire area of this node to be dirty. This function
* dose not directly triggers the paint event, instead, the framework will record
* this dirty area and later issues the PaintEventParam to the node to redraw it.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void Update();

/*******************************************************************************
*
* Update - request an updates for a specified area of the node
*
* This function marks a rectangle area of this node to be dirty. This function
* dose not directly triggers the paint event, instead, the framework will record
* this dirty area and later issues the PaintEventParam to the node to redraw it.
*
* <rect> is the dirty rectangle area to be updated;
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void Update
        (
        const TRect & rect
        );

/*******************************************************************************
*
* GetBoundRect - get the effective bounding rectangle of the node
*
* This function gets the effective bounding rectangle of the node. The effective
* bounding rectangle of node is decided by multiple factors, including the
* geometry data of the node itself, the alignment, and whether a layout manager
* is presented.
*
* RETURNS: the effective bounding rectangle.
*
* ERRNO: N/A
*
* SEE ALSO: SetPosition, SetSize, SetRect, GetPosition, GetSize
*/
    const TRect & GetBoundRect() const
        {
        return boundRect;
        }

/*******************************************************************************
*
* GetNormalizedBoundRect - get the normalized effective bounding rectangle
*
* This function gets the normalized effective bounding rectangle of the node.
*
* RETURNS: the normalized effective bounding rectangle.
*
* ERRNO: N/A
*
* SEE ALSO: GetBoundRect
*/
    TRect GetNormalizedBoundRect() const
        {
        return boundRect - boundRect.LeftTop();
        }

/*******************************************************************************
*
* SetEnabled - set the node to be enabled or disabled
*
* This function sets the node to be enabled or disabled. The real (effective)
* enable state is determined by the enable state of the node itself and the
* enable state of its parent. Disabled node is shown in dimmed state, and
* cannot receive user input message, thus cannot be operated. The default value
* for node's enable state is enabled.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetEnabled
*/
    void SetEnabled
        (
        TRT_BOOL value
        )
        {
        if (enabled != ((value) ? 1 : 0))
            {
            enabled = (value & 0x1U) ? !!1 : !!0;
            RouteDimStateChangeEvent();
            }
        }

/*******************************************************************************
*
* GetEnabled - get the enabled state of the node
*
* This function gets the enabled state of the node.
*
* RETURNS: TRT_TURE if enabled; TRT_FALSE if disabled.
*
* ERRNO: N/A
*
* SEE ALSO: SetEnabled, GetParentEnabled
*/
    TRT_BOOL GetEnabled() const
        {
        return enabled;
        }

/*******************************************************************************
*
* SetVisible - set the visibility of the node
*
* This function sets the visibility of the node. The real (effective)
* visibility is determined by the visibility of the node itself and the
* visibility of its parent. Invisible node is not rendered on the UI, and is
* skipped during input message dispatching. The default value for node's
* visibility is visible.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetEnabled
*/
    void SetVisible
        (
        TRT_BOOL value
        )
        {
        if (visible != ((value) ? 1: 0))
            {
            visible = (value & 0x1U) ? !!1 : !!0;
            Update();
            }
        }

/*******************************************************************************
*
* GetEnabled - get the visibility of the node
*
* This function gets the visibility of the node.
*
* RETURNS: TRT_TURE if visible; TRT_FALSE if invisible.
*
* ERRNO: N/A
*
* SEE ALSO: SetEnabled, GetParentVisible
*/
    TRT_BOOL GetVisible() const
        {
        return visible;
        }

/*******************************************************************************
*
* GetParentEnabled - get the enable state of all parent nodes
*
* This function gets the enable state of all parent nodes. This function
* is usually ANDed with GetEnabled to get the effective enable state of the
* node (i.e., node is effectively enabled only both node itself and its
* parent are enabled).
*
* RETURNS: TRT_TURE if all parent nodes along the parent line are enable;
* otherwise TRT_FALSE.
*
* ERRNO: N/A
*
* SEE ALSO: GetEnabled
*/
    TRT_BOOL GetParentEnabled() const;

/*******************************************************************************
*
* GetParentVisible - get the visibility of all parent nodes
*
* This function gets the visibility of all parent nodes. This function
* is usually ANDed with GetVisible to determine the effective visibility of the
* node (i.e., node is effectively visible only both node itself and its
* parent are visible).
*
* RETURNS: TRT_TURE if all parent nodes along the parent line are visible;
* otherwise TRT_FALSE.
*
* ERRNO: N/A
*
* SEE ALSO: GetEnabled
*/
    TRT_BOOL GetParentVisible() const;

/*******************************************************************************
*
* Activate - make this node the focused node of the window
*
* This function makes the node the focused node of the window. The focused
* node is the node which the keyboard message will be routed to; within one
* window, there is only one focused node at any time, so this function will
* effectively remove the focus from previous focused node.
* This function give the focus to the given child disregard of the
* allowTabActivate and allowMouseActive attributes.
* This function may trigger gainFocusEvent and LoseFocusEvent of the node.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void Activate();

/*******************************************************************************
*
* Deactivate - remove the input focus from this node
*
* This function removes input focus of the node. If the node is not the
* focused node, this function does nothing.
* This function may trigger LoseFocusEvent of the node.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: Activate
*/
    void Deactivate();

/*******************************************************************************
*
* SetAllowTabActivate - set allowing node activation by TAB-key navigation
*
* This function sets whether to allow activating the node by TAB navigation.
* If TAB activate is disabled, this node is skipped when pressing TAB to
* navigate through node.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetAllowTabActivate
*/
    void SetAllowTabActivate
        (
        TRT_BOOL value
        )
        {
        allowTabActivate = (value & 0x1U) ? !!1 : !!0;
        }

/*******************************************************************************
*
* GetAllowTabActivate - get allowing node activation by TAB-key navigation
*
* This function gets whether to allow activating the node by TAB navigation.
*
* RETURNS: whether TAB navigation activation is enabled;
*
* ERRNO: N/A
*
* SEE ALSO: SetAllowTabActivate
*/
    TRT_BOOL GetAllowTabActivate() const
        {
        return allowTabActivate;
        }

/*******************************************************************************
*
* SetAllowMouseActivate - set allowing node activation by mouse click
*
* This function sets whether to allow activating the node by mouse click.
* If mouse activation is disabled, mouse click will not put input focus to this
* node.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetAllowMouseActivate
*/
    void SetAllowMouseActivate
        (
        TRT_BOOL value
        )
        {
        allowMouseActivate = (value & 0x1U) ? !!1 : !!0;
        }

/*******************************************************************************
*
* GetAllowMouseActivate - get allowing node activation by mouse click
*
* This function gets whether to allow activating the node by mouse click.
*
* RETURNS: whether mouse click activation is enabled;
*
* ERRNO: N/A
*
* SEE ALSO: SetAllowMouseActivate
*/
    TRT_BOOL GetAllowMouseActivate() const
        {
        return allowMouseActivate;
        }

/*******************************************************************************
*
* SetAllowMouseTrack - set mouse enter & leave tracking for this node.
*
* This function set whether to allow framework to track mouse enter & leave for
* this node. If the mouse track is enabled, the framework will triggers mouse
* enter and mouse leave events when the corresponding mouse actions occur.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetAllowMouseTrack
*/
    void SetAllowMouseTrack(TRT_BOOL value)
        {
        allowMouseTrack = (value & 0x1U) ? !!1: !!0;
        }

/*******************************************************************************
*
* GetAllowMouseTrack - get mouse enter & leave tracking for this node.
*
* This function gets mouse enter & leave tracking for this node.
*
* RETURNS: whether mouse tracking is enabled;
*
* ERRNO: N/A
*
* SEE ALSO: SetAllowMouseTrack
*/
    TRT_BOOL GetAllowMouseTrack() const
        {
        return allowMouseTrack;
        }

/*******************************************************************************
*
* GetVisualState - get the visual state of the node
*
* This function get the current visual state of the node.
*
* RETURNS: the current visual state of the node;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    const VisualState & GetVisualState() const
        {
        return visualState;
        }

    void SetVisualStateMask
        (
        TRT_BOOL f_mask,
        TRT_BOOL d_mask,
        TRT_BOOL h_mask,
        TRT_BOOL a_mask
        );

    void SetFocused
        (
        TRT_BOOL value
        );

    void SetHighlighted
        (
        TRT_BOOL value
        );

    void SetDimmed
        (
        TRT_BOOL value
        );

    void SetActive
        (
        TRT_BOOL value
        );

    TRT_BOOL GetHighlighted() const
        {
        return visualState.highlighted;
        }

    TRT_BOOL GetFocused() const
        {
        return visualState.focused;
        }

    TRT_BOOL GetDimmed() const
        {
        return visualState.dimmed;
        }

    TRT_BOOL GetActive() const
        {
        return visualState.active;
        }
    void SetBoundPosition(const TPoint &);
    void SetBoundSize(const TSize &);
    void SetBoundRect(const TRect &);
    void AddToParent(TNode * p);
    void RemoveFromParent();
    void AddToMouseHook();
    void RemoveFromMouseHook();

    TRect UpliftToNode(TNode * node, const TRect & rect) const;
    TPoint OffsetToNode(TNode *, const TPoint &) const;

    void RouteGenericEvent(EventCallback callback, TRT_PVOID param);
    void RouteAttachEvent(TWindowHost *);
    void RouteDetachEvent();
    void RoutePaintEvent(PaintEventParam &);
    void RouteDimStateChangeEvent();

protected:
    friend class TWindowHost;
    friend class TChildrenDelegate;


    static TNode & ToNode(TControl & c);

    void DetermineBoundRect(TControl * c)
        {
        if (attachedHost && parent)
            {
            parent->OnChildBoundChange(c);
            }
        }

    TNode * FindMouseResponsiveNodeAt(const TPoint &);

    void OnGainFocus();
    void OnLoseFocus();
    void OnPaint(PaintEventParam &);
    void OnKeyboard(KeyboardEventParam &);
    void OnMouse(MouseEventParam &);
    void OnMouseEnter();
    void OnMouseLeave();
    void OnAttachToHost();
    void OnDetachFromHost();
    void OnResize();
    void OnChildBoundChange(TControl * n);
    void OnDimStateChange();

    TRect                   boundRect;
    TNode *                 parent;
    TWindowHost *           attachedHost;
    TEventDelegate *        event;
    TChildrenDelegate *     children;
    VisualState             visualState;

    TRT_U8 mouseResponsive      : 1;
    TRT_U8 enabled              : 1;
    TRT_U8 visible              : 1;
    TRT_U8 allowMouseActivate   : 1;
    TRT_U8 allowTabActivate     : 1;
    TRT_U8 allowMouseTrack      : 1;
    TRT_U8 geometryDirty        : 1;
    };

/* event delegate */
class TEventDelegate
    {
    friend class TNode;
private:
    virtual void OnEvent(EventParam &) = 0;
    };

/* children delegate */
class TChildrenDelegate
    {
    friend class TNode;
private:
    virtual TRT_S32 GetCount() const = 0;
    virtual TNode & GetNode(TRT_S32) = 0;
    };

class TNodeContainer : public TNode, public TChildrenDelegate
    {
public:
    TNodeContainer(TRT_S32 x, TRT_S32 y, TRT_S32 w, TRT_S32 h, TEventDelegate * e) :
        TNode(x, y, w, h, e, static_cast<TChildrenDelegate *>(this))
        {
        }
    virtual ~TNodeContainer()
        {}

    void AddChild(TNode * child, TRT_S32 zpos);
    void RemoveChild(TNode * child);
    void SetChildZPos(TNode * child, TRT_S32 zpos);

    TRT_S32 Count() const
        {
        return nodeList.Count();
        }
    TNode * operator [] (TRT_S32 index) const;
    TRT_S32 IndexOf(const TNode * child) const;

private:
    virtual TRT_S32 GetCount() const;
    virtual TNode & GetNode(TRT_S32);

    TArray<TNode *> nodeList;
    };

class TNodeArray : public TNode, public TChildrenDelegate
    {
public:
    TNodeArray(TRT_S32 x, TRT_S32 y, TRT_S32 w, TRT_S32 h, TEventDelegate * e, TRT_S32 n) :
        TNode(x, y, w, h, e, static_cast<TChildrenDelegate *>(this)),
        nodeArray(),
        num(n)
        {
        nodeArray = new TNode*[num];
        memset(nodeArray, 0, sizeof(TNode *) * num);
        }
    virtual ~TNodeArray()
        {
        delete nodeArray;
        }

    void AddChild(TNode * c, TRT_S32 i)
        {
        if (nodeArray[i] == TRT_NULL)
            {
            nodeArray[i] = c;
            c->AddToParent(this);
            }
        }

private:
    virtual TRT_S32 GetCount() const;
    virtual TNode & GetNode(TRT_S32);

    TNode ** nodeArray;
    TRT_S32 num;
    };

class TNodeEventDelegate : public TEventDelegate
    {
public:
    TEvent  event;

private:
    virtual void OnEvent(EventParam &);
    };

#endif /* _INCLUDE_tgsNode_H_ */


