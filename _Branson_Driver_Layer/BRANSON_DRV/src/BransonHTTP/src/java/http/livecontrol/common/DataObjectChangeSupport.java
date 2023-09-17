/* DataObjectChangeSupport.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.common;

import java.lang.Object;

import http.livecontrol.common.DataObjectChangeListener;


public interface DataObjectChangeSupport {
  public void    addDataObjectChangeListener ( DataObjectChangeListener consumer );
  public void    removeDataObjectChangeListener ( DataObjectChangeListener consumer );
  public void    fireDataObjectChange();
  public void    fireDataObjectChange(DataObjectChangeListener l,DataObjectChangeEvent d);
  public Object  getDataObject ();
}
