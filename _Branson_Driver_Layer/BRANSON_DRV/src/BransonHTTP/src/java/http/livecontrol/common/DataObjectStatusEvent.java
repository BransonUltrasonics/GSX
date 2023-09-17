/* DataObjectStatusEvent.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.common;

import java.lang.*;
import java.beans.PropertyChangeEvent;


public class DataObjectStatusEvent extends PropertyChangeEvent {
  public DataObjectStatusEvent (Object source, String propertyName, Object oldValue, Object newValue) {
    super(source,propertyName,oldValue,newValue);
  }
}
