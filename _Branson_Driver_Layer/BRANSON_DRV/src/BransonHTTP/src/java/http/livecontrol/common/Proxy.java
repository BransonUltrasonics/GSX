/* Proxy.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.common;

import java.lang.String;

import http.livecontrol.comm.LiveControlCommunication;

public interface Proxy {
  public String getName ();
  public void setRawValue ( byte[] raw);
  public byte[] getRawValue ();
  public boolean isReadProxy();
  public boolean isWriteProxy();
  public boolean isEnabled();
  public void setEnabled(boolean x);
  public boolean isVariableInstance();
}
