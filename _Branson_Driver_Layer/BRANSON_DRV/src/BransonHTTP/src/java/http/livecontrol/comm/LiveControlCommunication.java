/* LiveControlCommunication.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,02jan04,dlr  Port to WM Web
01a,27apr98,mb   initial creation
*/


package http.livecontrol.comm;

import java.io.IOException;
import java.beans.PropertyChangeListener;
import java.lang.String;
import java.net.URL;

import http.livecontrol.comm.SymbolTable;


/**
  * This interface offers the public functionality of the Live Control communication
  * system.
  *
  * @see http.livecontrol.comm.LiveControlConnection
  *
  */

public interface LiveControlCommunication {

  public void init (URL myURL) throws IOException;


    /**
      * Registration of server side symbols.
      * In consequence the server will provide an initial value and
      * subsequent updates any time the symbol value is changed at the server.
      *
      * The form with "pollingPriority" allows the user to select the
      * polling frequency. Without this, the default priority is "Medium".
      */
  public boolean subscribe (String name) throws IOException;
  public boolean subscribe (String name, byte pollingPriority) throws IOException;

    /**
      * Allows deregistration of server side symbols and
      * therefore disables subsequent symbol value updates sent by the server.
      */
  public void unsubscribe (String name) throws IOException;

    /**
      * Sends a symbol value to the server. If the server
      * accepts to write this value, any client subscribed to this symbol will
      * get an update, too.
      */
  public void setValue (String symbol, byte[] content) throws IOException;

     /**
      * Gets a single symbol value from the server
      * The symbol does not get registered for automatic updates by the server
      * The user leaves this symbol registered so that later requests do not
      * incure an description packet overhead
      */
  public boolean getValue (String name) throws IOException;

    /**
      * Gets a single symbol value from the server, expects a new instance value
      * The symbol does not get registered for automatic updates by the server
      * The user leaves this symbol registered so that later requests do not
      * incure an description packet overhead
      */
  public boolean getNextValue (String name) throws IOException;

   /**
      * Disconnects the connection to the server and stops any communication.
      */
  public void close ();

  public void stop ();

  public boolean isConnected();

  public void addListener ( PropertyChangeListener newListener );

  public void removeListener ( PropertyChangeListener oldListener );

  public SymbolTable getSymbolTable ();
};
