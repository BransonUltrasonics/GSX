/* StringConvert.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.converter;

import java.lang.String;
import java.lang.Object;

import http.livecontrol.comm.Logging;
import http.livecontrol.converter.Convertible;


public class StringConvert implements Convertible {
  // local vars
  private final static String[] myTypeNames = { "string" };
  private final Logging         Log = new Logging (System.out, 0, "StringConvert: ");

  public StringConvert ( ) {
    Log.log ( 25,"constructor called." );
  }

  public final Object Raw2Data ( byte[] raw ) {
    String data;

    Log.log ( 25,"Raw2Data() called" );
    data = new String(raw);
    Log.log ( 25,"converted raw values "+raw+" to >"+data+"<" );
    return data;
  }

  public final byte[] Data2Raw ( Object data ) {
    byte raw[];

    Log.log ( 25,"Data2Raw() called" );
    raw = data.toString().getBytes();
    return raw;
  }

  public final String[] getValidTypeNames ( ) {
    return myTypeNames;
  }

}
