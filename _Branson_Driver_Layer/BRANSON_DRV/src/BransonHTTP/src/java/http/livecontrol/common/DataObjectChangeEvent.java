/* DataObjectChangeEvent.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.common;

import java.lang.*;
import java.beans.PropertyChangeEvent;

/**
 * This event is used to transport all Live Control value updates.
 */
public class DataObjectChangeEvent extends PropertyChangeEvent {
    /**
     * Construct a new Live Control value update event.
     * @param source the reference of the sending object.
     * @param propertyname the name of the Live Control symbol.
     * @param oldvalue the old value of the symbol - in most cases set to null
     * @param newvalue the new value of the symbol.
     */
  public DataObjectChangeEvent (Object source, String propertyName, Object oldValue, Object newValue) {
    super(source,propertyName,oldValue,newValue);
  }
}
