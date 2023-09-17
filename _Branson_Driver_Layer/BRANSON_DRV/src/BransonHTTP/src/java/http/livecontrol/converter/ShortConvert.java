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
import java.lang.Short;
import java.lang.RuntimeException;

import http.livecontrol.comm.Logging;
import http.livecontrol.converter.Convertible;


public class ShortConvert implements Convertible {
  // local vars
  private final static String[] myTypeNames = { "short" };
  private final Logging         Log = new Logging (System.out, 25, "ShortConvert: ");

  public ShortConvert ( ) {
  }

  public final Object Raw2Data ( byte[] raw ) throws RuntimeException {
    Short data = null;

    if ( raw.length == 2 ) {
      data = new Short ( (short)((((short)raw[0])<<8) + b2s(raw[1]) ) );
      Log.log (25,"Raw2Data: Converted raw values 0x"+Integer.toHexString((int)raw[0])+",0x"+Integer.toHexString((int)raw[1])+" to Short("+data+")" );
    } else {
      throw new RuntimeException ("Raw2Data: invalid number of input bytes ("+raw.length+")!" );
    }
    return data;
  }

  public final byte[] Data2Raw ( Object data ) throws RuntimeException {
    byte raw[] = { 0,0 };
    short  tmpval;



    try {
      tmpval = ((Number)data).shortValue();

      raw[0] = (byte)( tmpval>>8 );
      raw[1] = (byte)( tmpval & 0x00ff );

      Log.log (25, "Data2Raw: Converted short "+tmpval+" to 0x"+Integer.toHexString((int)raw[0])+",0x"+Integer.toHexString((int)raw[1])+"." );
    } catch ( Exception e ) {
      throw new RuntimeException ( "Data2Raw: invalid input data ("+data+")" );
    }
    return raw;
  }

  public final String[] getValidTypeNames ( ) {
    return myTypeNames;
  }

  private final short b2s ( byte b ) {
    short s =  b < 0 ? (short)(256+b) : (short)b;
    return s;
  }

}
