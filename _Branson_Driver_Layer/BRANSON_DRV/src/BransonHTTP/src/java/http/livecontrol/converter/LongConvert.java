/* LongConvert.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
01a,10feb00,csr    written.
*/

package http.livecontrol.converter;

/* Java imports */

import java.lang.String;
import java.lang.Object;
import java.lang.Long;
import java.lang.Number;
import java.lang.RuntimeException;

/* http imports */

import http.livecontrol.comm.Logging;
import http.livecontrol.converter.Convertible;


/**
 * Converts objects and primitives of type Long (or long) to
 * a byte and visa versa.  This object only supports 32-bit longs
 * as the communication service, the LiveControl RPM, was created
 * for VxWorks.  To use 64-bit longs, create a class called
 * LongLongConvert as per the 64-bit long long primitive type
 * for VxWorks.
 *
 * @since Wind Web Server 2.0
 * @see Convertible.
 */
public class LongConvert implements Convertible {

  // local vars

    /**
     * list of types supported by this object.
     */
  private final static String[] myTypeNames = { "long" };

  /**
   * for debugging.
   */
  private final Logging         Log = new Logging (System.out, 0, "LongConvert: ");

    /**
     * default constructor.
     * Does nothing.
     */
  public LongConvert ( )
      {
      }


    /**
     * Implementation of Raw2Data for Long objects. Converts a 4 element
     * byte array into a 32-bit Long object.
     *
     * @param   raw  raw value to convert
     * @return     <code>Long</code> on success
     * @exception   RuntimeException  thrown if the byte array is not 4 bytes
     *                                long.
     */
  public final Object Raw2Data ( byte[] raw ) throws RuntimeException {
    Long data = null;
    int  tmpval;

    if ( raw.length == 4 ) {
      data = new Long ( (((long)raw[0]) << 24) + (b2l(raw[1]) << 16) + (b2l(raw[2]) << 8) + b2l(raw[3]) );
      Log.log ( 25,"converted raw values ("+raw[0]+","+raw[1]+","+raw[2]+","+raw[3]+") to Long("+data+")" );
    } else {
      throw new RuntimeException ( "Raw2Data: invalid number of input bytes ("+raw.length+")!" );
    }
    return data;
  }

    /**
     * Convert a <code>Long</code> object to a <code>byte</code> array.
     *
     * @param   data  to convert to raw byte array; it must be an integer.
     * @return  a byte array on success.
     * @exception   RuntimeException  thrown on falure.
     */
  public final byte[] Data2Raw ( Object data ) throws RuntimeException {
    byte raw[] = { 0,0,0,0 };
    long myLong;

    try {
      myLong = ((Number)data).longValue();
      raw[3] = (byte)(myLong & 0xff);
      raw[2] = (byte)(( myLong >> 8 ) & 0xff);
      raw[1] = (byte)(( myLong >> 16 ) & 0xff);
      raw[0] = (byte)( myLong >> 24 );

      raw[3] = raw[3] < 0 ? (byte)(256+raw[3]) : raw[3];
      raw[2] = raw[2] < 0 ? (byte)(256+raw[2]) : raw[2];
      raw[1] = raw[1] < 0 ? (byte)(256+raw[1]) : raw[1];

      Log.log ( 25,"raw[] created with values ("+raw[0]+","+raw[1]+","+raw[2]+","+raw[3]+")" );
    } catch ( Exception e ) {
      throw new RuntimeException ( "Raw2Data: invalid input type!" );
    }
    return raw;
  }

    /**
     * returns the list of primitive types that can be converted by
     * this object.  If this list does not contain the desired primitive,
     * there is no guarantee that the data will be converted.
     *
     * @return <code>String</code> array containing the valid types
     *         which can be converted by this class.
     */
  public final String[] getValidTypeNames ( ) {
    return myTypeNames;
  }

    /**
     * converts a byte to a long.
     *
     * @param   b  the byte to be converted.
     * @return    long representation of the byte
     */
  private final long b2l ( byte b ) {
    long l =  b < 0 ? (long)(256+b) : (long)b;
    return l;
  }

}
