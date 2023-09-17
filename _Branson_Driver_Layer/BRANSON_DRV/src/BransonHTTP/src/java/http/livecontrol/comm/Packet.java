/*  Packet.java - Live Control packet implementation */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,02jan04,dlr  Port to WM Web
01a,07may98,mb     initial creation
*/


package http.livecontrol.comm;


import java.lang.*;
import java.net.*;
import java.io.*;
import java.util.*;


/**
  * Represents a Live Control packet as transmitted through a network channel
  */
class Packet extends Object {

  // private instance vars
  private byte      packet[];
  private Logging   Log = new Logging (System.out, 30, "Packet:");


  // constructors
  /**
    * Creates a Packet Object out of a byte array
    */
  public Packet (byte[] content) throws IOException {
    packet = content;
  }


  /**
    * creates a Packet Object out of a String.
    * @param content This string will be converted into a byte sequence.
    */
  public Packet (String content) throws IOException {
    packet = content.getBytes("ISO-8859-1");
  }


  /**
    * Creates a Packet by reading from a DataInputStream.
    * @param inStream Input stream to read bytes from.
    */
  public Packet(InputStream inStream) throws IOException {
    int size = 0;
    byte bsize[] = new byte[4];
    int test;
    int bytecount;
    int got;

    try {
      if ( ( test = inStream.read(bsize)) == 4 ) {
	//XXX calculate size
	size = byteToInt(bsize[0]);
	size = size * 256 + byteToInt(bsize[1]);
	size = size * 256 + byteToInt(bsize[2]);
	size = size * 256 + byteToInt(bsize[3]);
      } else {
	throw new IOException ("can not read packet header length" );
      }
      Log.log ( "Packet: Packet Header ("+test+" Bytes length): Packet size "+size+" Bytes ["+byteToHex(bsize[0])+","+byteToHex(bsize[1])+","+byteToHex(bsize[2])+","+byteToHex(bsize[3])+"]" );
      if ( ( size > ( 1<<11 )) || ( size < 1 ) ) {
	throw new IOException ( "packet has wrong size!("+Integer.toHexString(size)+")" );
      }
      packet = new byte[size];
      bytecount = 0;
      while (bytecount < size) {
	got = inStream.read (packet,bytecount,size-bytecount);
	if (got == -1) {
	  throw new IOException ("short read");
	}
	bytecount += got;
      }


      {
          int i;
          String output = "Packet(InputStream inStream)  packet[]=";

          for (i=0; i<size; i++) {
              output += "[" + Integer.toHexString( (int)packet[i] ) + "]";
          }
          Log.log ( 25,output);
      }


    } catch (IOException ex) {
      Log.log (5,"Communication error: " + ex);
      throw new IOException ("receive: "+ex.toString());
    }
  }


  /**
    * Add bytes at the end of the current packet.
    * @param additional Bytes to append at the end of the packet.
    */
  public final void add (byte additional[]) {
    int   i;
    byte  newpacket[] = new byte[packet.length+additional.length];

    for (i=0;i<packet.length;i++)
      newpacket[i] = packet[i];
    for (i=0;i<additional.length;i++)
      newpacket[i+packet.length] = additional[i];
    packet = newpacket;
  }


  /**
    * Add String at the end of the current packet.
    * @param additional The content of this String will be added to the end
    * of the packet as byte array.
    */
  public final void add (String additional) {
    this.add (additional.getBytes());
  }


  /**
    * Get the bytes contained within this packet.
    * @return Byte array containing all bytes of this packet
    */
  public final byte[] getContent () {
    return packet;
  }

  /**
    * Get the bytes contained within this packet including packet
    * header. This is the format used for transmitting packets.
    * @return Byte array containing all bytes of this packet.
    */
  public final byte[] getBytes() {
    byte tpacket[] = new byte[packet.length+4];
    int  i;

    tpacket[0] = (byte)(packet.length/16777216);
    tpacket[1] = (byte)((packet.length-tpacket[0]*16777216)/65536);
    tpacket[2] = (byte)((packet.length-tpacket[0]*16777216-tpacket[1]*65536)/256);
    tpacket[3] = (byte)(packet.length-tpacket[2]*256-tpacket[1]*65536-tpacket[0]*16777216);

    for ( i=0;i<packet.length;i++ ) {
      tpacket[i+4] = packet[i];
    }
    return tpacket;
  }

  /**
    * Get the Live Control command byte of this packet.
    * @return The Live Control command byte of this packet.
    */
  public final byte getCommand () {
    return packet[0];
  }


  /**
    * Transmit this packet.
    * @param outStream Output stream to use for transmission.
    */
  public void send(OutputStream outStream) throws IOException {

    try {
      outStream.write (getBytes());
      Log.log ( "send: "+packet.length+" Bytes written." );
    } catch (Exception ex)
      {
	throw new IOException ("outputStream.writeBytes() failed!");
      }
  }

  /**
    * Get the number of bytes needed to transmit this packet using
    * the send() method call. This is important to determine the
    * packet size in advance to its transmission.
    * @return The number of bytes needed to transmit this packet, or 0 if packet is empty
    */
  public final int getTransmissionLength() {
    if ( packet.length == 0 ) {
      return 0;
    } else {
      return packet.length+4;
    }
  }

  public static int byteToInt(byte b) {
    int i = ((int)b) & 255;

    return i;
  }

  public static final String byteToHex (byte b) {
    int i= byteToInt(b);
    return Integer.toHexString(i);
  }

  //----------------------------
  // private methods

}




