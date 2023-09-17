/*
 * ShortConvert.java
 *
 *  1998 by M. Bathelt - Copyright 3SOFT GmbH
 *
 */

package http.livecontrol.converter;

import java.lang.String;
import java.lang.Object;
import java.lang.Boolean;
import java.lang.RuntimeException;

import http.livecontrol.comm.Logging;
import http.livecontrol.converter.Convertible;


public class BoolConvert implements Convertible {
  // local vars
  private final static String[] myTypeNames = { "bool","boolean" };
  private final Logging         Log = new Logging (System.out, 0, "BoolConvert: ");

  public BoolConvert ( ) {
  }

  public final Object Raw2Data ( byte[] raw ) throws RuntimeException {
    Boolean data = null;

    if ( raw.length == 1 ) {
      data = new Boolean ( raw[0] != 0 );
      Log.log ( 25,"converted raw value "+raw[0]+" to Boolean("+data+")" );
    }
    return data;
  }

  public final byte[] Data2Raw ( Object data ) throws RuntimeException {
    byte raw[] = { 0 };

    if ( data instanceof Number ) {
      raw[0] = ((Number)data).byteValue() == 0 ? (byte)0 : (byte)1;
    } else {
      if ( data instanceof Boolean ) {
	raw[0] = ((Boolean)data).booleanValue() ? (byte)1 : (byte)0;
      }
    }
    Log.log (25, "converted boolean "+data+" to "+raw[0]);
    return raw;
  }

  public final String[] getValidTypeNames ( ) {
    return myTypeNames;
  }

}
