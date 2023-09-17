/* Convertible.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.converter;

import java.lang.String;
import java.lang.Object;
import java.lang.RuntimeException;

public interface Convertible {
  public Object Raw2Data ( byte[] raw ) throws RuntimeException;
  public byte[] Data2Raw ( Object data ) throws RuntimeException;
  public String[] getValidTypeNames ( );
  // public boolean isConvertible ( byte[] raw );
  // public boolean isConvertible ( Object data );
}
