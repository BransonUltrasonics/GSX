/* LCJScrollBar.java - Scrollbar widget (Swing) */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.widgets;

import java.lang.*;
import java.awt.*;
import javax.swing.*;

import http.livecontrol.common.*;

/**
 * Example swing Scrollbar widget.
 * This class is unsupported!
 */
public class LCJScrollBar extends JScrollBar implements DataObjectChangeListener,DataObjectStatusListener {

    /**
     * Implementation of interface DataObjectChangeListener.
     * This method is called by the proxy of this widget to pass value updates to it.
     * @param data event containing the new value that should be represented by the widget from now on
     */
    public void dataObjectChange (DataObjectChangeEvent data) {
	BoundedRangeModel model;
	int               value;

	if ( data.getNewValue() != null ) {
	    // myScrollBar.setValue(((Integer)data.getNewValue()).intValue());
	    value = ((Integer)data.getNewValue()).intValue();

	    // model = getModel();
	    // model.setValue(value);
	    // setModel(model);
	    setValue(value);
	    revalidate();
	    repaint(400,getX(),getY(),getHeight(),getWidth());
	}
    }

    /**
     * Implementation of interface DataObjectStatusListener that provides
     * knowledge about the accuracy of the current value displayed.
     * Called by the proxy of this widget after connection establishment/shutdown.
     * @param event containing the new status of this widget as Boolean value.
     */
    public void    dataObjectStatus(DataObjectStatusEvent x) {
	setEnabled(((Boolean)x.getNewValue()).booleanValue());
	repaint(400,getX(),getY(),getHeight(),getWidth());
    }

}
