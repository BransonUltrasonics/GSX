/* DataObjectStatusListener.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.common;

import java.lang.*;

/**
 * Any object that should be able to receive LiveControl value updates must implement this interface.
 */
public interface DataObjectStatusListener {
    /**
     * Called to notify implementor about a value update.
     */
  public void    dataObjectStatus(DataObjectStatusEvent x);
}
