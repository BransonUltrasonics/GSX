/* DataObjectChangeListener.java */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.common;

import java.lang.*;
import http.livecontrol.common.DataObjectChangeEvent;


public interface DataObjectChangeListener {
  public void dataObjectChange (DataObjectChangeEvent data) throws RuntimeException;
}
