/* DataObjectStatusSupport.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.common;

import java.lang.*;

import http.livecontrol.common.DataObjectStatusListener;


/**
 * Any object that wants to send Live Control value updates must implement this interface.
 */
public interface DataObjectStatusSupport {
  public void    addDataObjectStatusListener ( DataObjectStatusListener consumer );
  public void    removeDataObjectStatusListener ( DataObjectStatusListener consumer );
  public void    fireDataObjectStatus();
  public void    fireDataObjectStatus(DataObjectStatusListener l,DataObjectStatusEvent d);
}
