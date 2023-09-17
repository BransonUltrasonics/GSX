/* LiveControlTest.java - Test of Live Control functionality */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,02jan04,dlr  Port to WM Web
01a,26apr98,mb     initial creation
*/

package http.livecontrol.comm;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.net.URL;
import java.net.MalformedURLException;
import java.beans.PropertyChangeSupport;
import java.beans.PropertyChangeListener;

import http.livecontrol.comm.*;
import http.test.*;


public class LiveControlTest {

  // local vars
  static URL                   ServerURL;
  static LiveControlConnection LiveServer;
  private static String myString = new String ( "Live!" );
  private static String myNumber = "0004";


  // public methods
  public static void init() {
    try
      {
	ServerURL  = new URL ("http://target03/lc/"); // change the name of the server according to your environment
	LiveServer = new LiveControlConnection ();
	LiveServer.init (ServerURL);
	LiveServer.addListener ( new TestListener() );
	LiveServer.subscribe ( "sRadioIndex" );
	LiveServer.setValue ( "szString" , myString.getBytes() );
	LiveServer.setValue ( "sRadioIndex" , myNumber.getBytes() );
      } catch (Exception ex)
	{
	  ex.printStackTrace();
	  System.out.println("Error: "+ex);
	  return;
	}
  }

  public static void main (String argv[]) {
    init();
  }

}
