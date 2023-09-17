/*  Mime.java   - */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,02jan04,dlr  Port to WM Web
*/

package http.livecontrol.comm;


import java.lang.*;
import java.net.*;


public class Mime {
  private String myName;
  private String myValue;

  public Mime ( String name, String value ) throws ProtocolException {
    myName = name;
    myValue = value;
  }

  public Mime ( String headerline ) throws ProtocolException {
    int valstart;

    valstart = headerline.indexOf(": ");
    if ( valstart == -1 ) {
      throw new ProtocolException();
    }
    myName = headerline.substring (0,valstart-1);
    myValue = headerline.substring (valstart+1);
  }

  public String getName () {
    return myName;
  }

  public String getValue () {
    return myValue;
  }
}
