/* TestListener.java - experimental listener to demonstrate the functionality of LiveControlConnection.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
01a,29apr98,mb     initial creation
*/

package http.test;

import java.beans.PropertyChangeSupport;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;
import java.io.PrintStream;

import http.livecontrol.comm.*;


public class TestListener extends Object implements PropertyChangeListener {
  // private vars
  private Logging Log = new Logging (java.lang.System.out,8,"TestListener");

  public TestListener () {
  }

  public void propertyChange(PropertyChangeEvent e) {
    Log.log ( "got event "+e.getPropertyName()+" with value "+e.getNewValue() );
  }

}
