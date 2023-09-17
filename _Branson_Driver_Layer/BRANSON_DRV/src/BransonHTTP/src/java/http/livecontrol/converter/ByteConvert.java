/* ShortConvert.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.converter;

import java.lang.String;
import java.lang.Object;
import java.lang.Byte;
import java.lang.RuntimeException;

import http.livecontrol.comm.Logging;
import http.livecontrol.converter.Convertible;


public class ByteConvert implements Convertible {
  // local vars
  private final static String[] myTypeNames = { "byte","char" };
  private final Logging         Log = new Logging (System.out, 25, "ByteConvert: ");

  public ByteConvert ( ) {
  }

  public final Object Raw2Data ( byte[] raw ) throws RuntimeException {
    Byte data = null;

    if ( raw.length == 1 ) {
      data = new Byte ( raw[0] );
      Log.log ( 25,"converted raw value "+raw[0]+" to Byte("+data+")" );
    } else {
      throw new RuntimeException ( "Raw2Data: invalid number of input bytes ("+raw.length+")!" );
    }
    return data;
  }

  public final byte[] Data2Raw ( Object data ) throws RuntimeException {
    byte raw[] = { 0 };

    try {
      raw[0] = ((Number)data).byteValue();
    } catch ( Exception e ) {
      throw new RuntimeException ( "Data2Raw: invalid input data ("+data+")" );
    }
    return raw;
  }

  public final String[] getValidTypeNames ( ) {
    return myTypeNames;
  }

}
