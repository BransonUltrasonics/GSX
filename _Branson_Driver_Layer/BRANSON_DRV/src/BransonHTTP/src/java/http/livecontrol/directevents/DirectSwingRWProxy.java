/* DirectSwingRWProxy.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.directevents;

import java.util.Vector;
import java.util.Enumeration;
import java.lang.*;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.*;

import http.livecontrol.common.*;
import http.livecontrol.comm.*;
import http.livecontrol.converter.Convertible;


/**
 * This proxy implementation provides a direct interface to several swing components like
 * JToggleButton, JScrollbar and others, which communicate using the supported event-listeners.
 */
public class DirectSwingRWProxy extends DirectRWProxy implements ChangeListener,ActionListener,AdjustmentListener {

  public DirectSwingRWProxy(String symbolname) {
    super(symbolname);
  }

  public void stateChanged(ChangeEvent e) {
    AbstractButton b = (AbstractButton)e.getSource();
    ButtonModel    model = b.getModel();
    setValue(new Boolean(model.isPressed()));
  }

  public void adjustmentValueChanged(AdjustmentEvent e) {
    Log.log ( 25,"adjustmentValueChanged("+e+") called." );

    if ( e.getSource() instanceof JScrollBar ) {
      JScrollBar s = (JScrollBar)e.getSource();
      if (s.getValueIsAdjusting())
	return;
    }

    try {
      setValue(new Integer(e.getValue()));
    } catch ( Exception x ) {
      Log.log ( 0, "Exception occured in adjustmentValueChanged! "+x);
    }
  }

  public void actionPerformed(ActionEvent e) {
    Log.log ( 25,"actionPerformed ("+e+") called." );
    try {
      setValue(new Boolean(((AbstractButton)e.getSource()).getModel().isSelected()));
    } catch ( Exception x ) {
      Log.log ( 0, "Exception occured in actionPerformed! "+x);
    }
  }

  public void fireDataObjectChange(DataObjectChangeListener l,DataObjectChangeEvent d) {
    super.fireDataObjectChange(l,d);
    if ( l instanceof Component ) {
      ((Component)l).setEnabled(isEnabled());
    }
  }
}
