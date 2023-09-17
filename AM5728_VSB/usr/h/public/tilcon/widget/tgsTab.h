/* tgsTab.h - Tilcon tab widget */

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
15may14,yya fix V7GFX-68
01jan14,yya created (US15550)
*/

/*
DESCRIPTION

This file defines Tilcon's tab widget.

INCLUDE FILES: tilcon/widget/tgsTab.h
*/

#ifndef _INCLUDE_tab_H_
#define _INCLUDE_tab_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/engine/tgsWindow.h"
#include "tilcon/engine/tgsGridLayout.h"


class TTab : public TControl
    {
public:
    /* tab change event parameter */
    struct TabChangeEventParam : public ControlEventParam
        {
        TRT_S32 previousTab;   /* previous selection; */
        TRT_S32 currentTab;    /* current selection; */
        };

    enum HeaderLayoutMethod
        {
        HLM_auto = 0,
        HLM_fixed
        };

    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        HeaderLayoutMethod      headerLayoutMethod;
        TMargin                 headerTextMargin;
        TMargin                 tabMargin;
        TCustomVisual           frameVisual;
        TextProperty            headerTextProperty;
        };

public:
/*******************************************************************************
*
* TTab - construct a tab widget
*
* SYNOPSIS
* \ss
* TTab
*     (
*     TRT_S32 x = 0,            /@ x position @/
*     TRT_S32 y = 0,            /@ y position @/
*     TRT_S32 w = 0,            /@ width @/
*     TRT_S32 h = 0,            /@ height @/
*     TRT_S32 n = 0             /@ number of tabs @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a tab widget.
* This function may throws std::bad_alloc.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* the position and size of the tab widget;
* \i <n>
* number of tabs; If specified value is less than 1, default value of 1 is used;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TTab(TRT_S32 x =0, TRT_S32 y = 0, TRT_S32 w = 0, TRT_S32 h = 0,
        TRT_S32 number = 1);

/*******************************************************************************
*
* ~TTab - destructs the tab widget
*
* This function destructs the tab widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TTab();

/*******************************************************************************
*
* TabChangeEvent - get the Tab Change Event of the tab widget
*
* SYNOPSIS
* \ss
* TEvent & CheckEvent()
* \se
*
* DESCRIPTION
* This function gets the reference to the Tab Change Event. Tab Change Event is
* emitted when the tab page is changed. The event parameter is of type
* TabChangeEventParam, which contains the index of the previous active tab page
* and the current active tab page:
*
* \cs
* struct TabChangeEventParam : public ControlEventParam
*     {
*     TRT_S32 previousTab;   /@ previous selection; @/
*     TRT_S32 currentTab;    /@ current selection; @/
*     };
* \ce
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the Check Event of check box.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & TabChangeEvent()
        {
        return tabChangeEvent;
        }

/*******************************************************************************
*
* SetProperty - change the property of tab widget
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
* This function changes the property of tab widget. This function is used to
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
* GetProperty - get the property of tab widget
*
* SYNOPSIS
* \ss
* Property GetProperty() const
* \se
*
* DESCRIPTION
* This function gets the property of tab widget.
*
* RETURNS: the Property structure representing the tab widget's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* AddControl - add child control to the specified tab page
*
* SYNOPSIS
* \ss
* TRT_STATUS AddControl
*     (
*     TRT_S32       page,   /@ tab page to add child @/
*     TControl *    child,  /@ child to be added @/
*     TRT_S32       index   /@ z-order index @/
*     )
* \se
*
* DESCRIPTION
* This function adds child control to the specified page, at the specified
* Z-order.
* This function may throws std::bad_alloc.
*
* Parameters:
* \is
* \i <page>
* tab page to add child. tab page shall be a valid number, or TException of
* ET_index type is thrown.
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
* SEE ALSO: RemoveControl()
*/
    void AddControl(TRT_S32 tab, TControl *, TRT_S32 index);

/*******************************************************************************
*
* RemoveControl - remove a specific control from the specified tab page
*
* SYNOPSIS
* \ss
* void RemoveControl
*     (
*     TRT_S32       page,   /@ tab page to remove child @/
*     TControl *    child   /@ child to be removed @/
*     )
* \se
*
* DESCRIPTION
* This function removes a specific control from the specified tab page. if the
* specified control is not a child of the tab page, this function does nothing.
*
* Parameters:
* \is
* \i <page>
* tab page to add child. tab page shall be a valid number, or TException of
* ET_index type is thrown.
* \i <child>
* control to be removed.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: AddControl()
*/
    void RemoveControl(TRT_S32 tab, TControl *);

/*******************************************************************************
*
* SetTabText - set the tab header text
*
* SYNOPSIS
* \ss
* void SetTabText
*     (
*     TRT_S32           page,   /@ tab page to change @/
*     const TWString &  text    /@ text to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the tab header text.
* If the specified page index is out of range, a TException with type ET_index
* is thrown.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetTabText(TRT_S32 page, const TWString & text);

/*******************************************************************************
*
* SetTabWidth - set the tab header width
*
* SYNOPSIS
* \ss
* void SetTabWidth
*     (
*     TRT_S32   page,   /@ tab page to change @/
*     TRT_S32   width   /@ new width @/
*     )
* \se
*
* DESCRIPTION
* This function sets the tab header width.
* If the specified page index is out of range, a TException with type ET_index
* is thrown.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetTabWidth(TRT_S32 page, TRT_S32 width);

/*******************************************************************************
*
* AddTab - add a new tab page to the tab widget
*
* SYNOPSIS
* \ss
* TRT_S32 AddTab
*     (
*     const TWString & text,    /@ header text for the new tab @/
*     TRT_S32 width             /@ header width of the new tab @/
*     )
* \se
*
* DESCRIPTION
* This function adds a new tab page to the widget, with the specified header
* text and header width.
* This function may throws std::bad_alloc.
*
* RETURNS: the index of the newly added tab; or -1 when tab is not added due
* to maximum tab page number is reached.
*
* ERRNO: N/A
*
* SEE ALSO: RemoveTab()
*/
    TRT_S32 AddTab(const TWString & label, TRT_S32 width);

/*******************************************************************************
*
* RemoveTab - remove the specified tab page from the tab widget
*
* SYNOPSIS
* \ss
* void RemoveTab
*     (
*     TRT_S32 page             /@ page to remove @/
*     )
* \se
*
* DESCRIPTION
* This function removes the specified tab page to the widget. If the specified
* page index is out of range, a TException with type ET_index is thrown.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: AddTab()
*/
    void RemoveTab(TRT_S32);

/*******************************************************************************
*
* SwitchTo - switch to the specified tab page
*
* SYNOPSIS
* \ss
* void SwitchTo
*     (
*     TRT_S32 page             /@ page to switch to @/
*     )
* \se
*
* DESCRIPTION
* This function switches the specified tab page to the widget. If the specified
* page index is out of range, a TException with type ET_index is thrown.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SwitchTo(TRT_S32);

/*******************************************************************************
*
* GetActiveTab - get the index of active tab page
*
* SYNOPSIS
* \ss
* TRT_S32 GetActiveTab() const
* \se
*
* DESCRIPTION
* This function gets the index of active tab page.
*
* RETURNS: index of the active tab page
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 GetActiveTab() const
        {
        return activeTab;
        }

/*******************************************************************************
*
* SetHeaderLayoutMethod - set the method to layout the tab headers
*
* SYNOPSIS
* \ss
* void SetHeaderLayoutMethod
*     (
*     HeaderLayoutMethod value  /@ new method to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the method to layout the tab headers. Header layout methods
* are defined as:
* \is
* \i <HLM_auto>
* all header widths are equally divided.
* \i <HLM_fixed>
* header width is specified by user, via AddTab() or SetTabWidth() methods.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetHeaderLayoutMethod()
*/
    void SetHeaderLayoutMethod(HeaderLayoutMethod value)
        {
        headerLayoutMethod = value;
        }

/*******************************************************************************
*
* GetHeaderLayoutMethod - get the method to layout the tab headers
*
* SYNOPSIS
* \ss
* HeaderLayoutMethod GetHeaderLayoutMethod() const
* \se
*
* DESCRIPTION
* This function gets the method to layout the tab headers.
*
* RETURNS: the method to layout the tab headers
*
* ERRNO: N/A
*
* SEE ALSO: SetHeaderLayoutMethod()
*/
    HeaderLayoutMethod GetHeaderLayoutMethod() const
        {
        return headerLayoutMethod;
        }

private:

    struct TabInfo
        {
        TRT_S32         width;
        TWString        label;
        TRT_S32         start;
        TRT_S32         end;
        TGroup *        panel;
        };

    void NodeHandler(EventParam &);
    void ResizeHandler(EventParam &);
    void HeaderPaintHandler(EventParam &);
    void HeaderMouseHandler(EventParam &);

    void SetTabCount(TRT_S32 value);

    TEvent                  tabChangeEvent;
    TRT_S32                 activeTab;

    HeaderLayoutMethod      headerLayoutMethod;
    TMargin                 headerTextMargin;
    TVisual                 tabVisual;
    TMargin                 tabMargin;

    TNodeEventDelegate      eventDelegate;
    TGridLayout             layout;
    TNodeArray              boundNode;
    TNodeContainer          header;
    TNodeContainer          body;
    TArray<TabInfo>         tabArray;
    };

#endif /* _INCLUDE_tab_H_ */

