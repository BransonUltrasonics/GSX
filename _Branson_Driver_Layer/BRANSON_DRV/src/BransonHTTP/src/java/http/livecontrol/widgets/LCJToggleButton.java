/* LCJToggleButton.java */

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


public class LCJToggleButton extends JToggleButton implements DataObjectChangeListener,DataObjectStatusListener {

    /**
     * Implementation of interface DataObjectChangeListener.
     * This method is called by the proxy of this widget to pass value updates to it.
     * @param data event containing the new value that should be represented by the widget from now on
     */
    public void dataObjectChange (DataObjectChangeEvent data) throws RuntimeException {
	if ( data.getNewValue() != null ) {
	    setSelected(((Boolean)data.getNewValue()).booleanValue());
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
    }

}
