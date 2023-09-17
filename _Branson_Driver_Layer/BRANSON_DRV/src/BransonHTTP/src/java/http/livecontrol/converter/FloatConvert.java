/* FloatConvert.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.converter;

import java.lang.String;
import java.lang.Float;
import java.lang.Object;

import http.livecontrol.comm.Logging;
import http.livecontrol.converter.Convertible;


public class FloatConvert implements Convertible {
  // local vars
  private final static String[] myTypeNames = { "float","double" };
  private final Logging         Log = new Logging (System.out, 25, "FloatConvert: ");

  public FloatConvert ( ) {
  }

  public final Object Raw2Data ( byte[] raw ) {
    Float data;

    data = new Float(new String(raw));
    Log.log ( 25,"converted raw values "+raw+" to >"+data+"<" );
    return data;
  }

  public final byte[] Data2Raw ( Object data ) {
    byte raw[];

    raw = data.toString().getBytes();
    return raw;
  }

  public final String[] getValidTypeNames ( ) {
    return myTypeNames;
  }

}
