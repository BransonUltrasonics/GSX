/* Registry.java - This interface is the partner API for Live Control proxy components.
 * It is used to tie proxy components to the LiveControlCommunication object.
 */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,02jan04,dlr  Port to WM Web
*/

package http.livecontrol.comm;

import java.lang.String;
import java.io.IOException;
import java.beans.PropertyChangeListener;

import http.livecontrol.comm.LiveControlCommunication;
import http.livecontrol.common.Proxy;
import http.livecontrol.common.AbstractProxy;
import http.livecontrol.converter.ConverterFactory;

/**
  */


public interface Registry {

  /**
    * initialize registry
    *
    */
  public void init () throws IOException;

  /**
    * register a new Proxy object
    *
    *
    */
  public void addPropertyChangeListener ( PropertyChangeListener listener );

  /**
    * deregister a Proxy object
    *
    */
  public void removePropertyChangeListener ( PropertyChangeListener listener );

  /**
    * get Proxy with name "name"
    *
    * throws nullpointer exception if no such Proxy is available
    * @return if specified Proxy is available return it
    */
  public Proxy getProxy ( String name );


  /**
    * provide the communication object to proxies
    * @return reference to Live Control communication object
    */
  public LiveControlCommunication getCommunication ();

  public ConverterFactory getConverterFactory ();
};
