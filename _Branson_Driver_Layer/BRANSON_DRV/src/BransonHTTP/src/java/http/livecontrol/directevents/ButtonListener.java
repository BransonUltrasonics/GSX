/* ButtonListener.java - experimental listener to demonstrate the functionality of LiveControlConnection.java */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,04jan04,dlr    initial creation
*/

package http.livecontrol.directevents;

import java.awt.*;
import java.awt.event.*;
import java.applet.*;

import http.livecontrol.comm.*;
import http.livecontrol.directevents.*;

public class ButtonListener implements ActionListener
{
	// private vars
	private Logging Log = new Logging (java.lang.System.out,8,"ButtonListener");

	private TextWidget theWidget;

	public ButtonListener (TextWidget widget)
		{
		theWidget = widget;
  		}

	public void actionPerformed(ActionEvent e)
		{
		theWidget.fireDataObjectChange();
		}

}
