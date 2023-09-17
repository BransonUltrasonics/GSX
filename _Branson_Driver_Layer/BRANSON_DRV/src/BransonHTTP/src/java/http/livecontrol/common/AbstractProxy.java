/* AbstractProxy.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
01a,27oct99,csr   written. Removed commented methods.
*/

package http.livecontrol.common;

/* Java imports */

import java.lang.*;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;

/* http imports */

import http.livecontrol.converter.Convertible;
import http.livecontrol.common.Proxy;
import http.livecontrol.comm.LiveControlCommunication;
import http.livecontrol.comm.Logging;

/**
 * AbstractProxy is a object skeleton that can be easily extended for use with different
 * different client side communication or event architectures, e.g. the Live Control,
 * InfoBus or custom object-to-object communication.
 * The main goal tracked by AbstractProxy is to get a common superclass for all proxy
 * implementations, and to encapsulate the Live Control data coding modules at this
 * one point.
 *
 * @since Wind Web Server 2.0
 * @see Proxy, PropertyChangeListener
 */
public abstract class AbstractProxy implements Proxy,PropertyChangeListener
    {

    // local variables
    private String        myName;
    private Convertible   myConverter = null;
    private String        myType = null;
    private final Logging Log = new Logging (System.out, 5, "AbstractProxy: ");

    private boolean       isreadable = false;
    private boolean       iswritable = false;
    private boolean       enabled = false;
    private boolean       isvariableinstance = false;

    // package vars
    protected Object      myValue = null;

    // constructors

    /**
     * Creates an AbstractProxy - intended must be called by subclasses.
     * @param symbolname symbol name represented by this proxy
     * @since Wind Web Server 2.0
     */
    public AbstractProxy ( String symbolname ) throws NullPointerException
        {
        if (symbolname != null)
            {
            myName = symbolname;
            Log.log (10, "Name: "+myName );
            }
        else
            {
            NullPointerException e = new NullPointerException ( "AbstractProxy needs symbolname!" );
            e.printStackTrace();
            throw e;
            }
        }

    /**
     * Get the converter object neccessary to decode Live Control packets for this symbol.
     * @return Converter object for the matching symbol type. The symbol type is determinded
     * automatically using the server symbol table information.
     */
    protected abstract Convertible getConverterObject ();

    // interface PropertyChangeListener

    /**
     * Proxies are PropertyChangeListeners and may be influenced dependend on the
     * client side object-to-object communication.
     * @since Wind Web Server 2.0
     */
    public abstract void propertyChange(PropertyChangeEvent e);


    /**
     * Set the converter module of this proxy.
     *
     * As soon as the symbol type from the server symbol table is known,
     * the proxy is able to select the matching conversion object for
     * this very symbol type. The conversion object transforms Live Control
     * data packets to Java objects and vice versa.
     *
     * @param converter The conversion object to be used in the future by this proxy.
     * @see http.livecontrol.common.AbstractProxy#getConverter
     * @since Wind Web Server 2.0
     */
    protected void setConverter (Convertible converter)
        {
        myConverter = converter;
        }

    /**
     * Get the converter module currently used by this proxy object
     * @see http.livecontrol.common.AbstractProxy#setConverter
     */
    protected Convertible getConverter () {
      if ( myConverter == null ) {
        myConverter = getConverterObject();
      }
      return myConverter;
    }

    /* implementation of interface Proxy */

    /**
     * Get the name of this proxy.
     *
     * @return String containing the proxy`s name.
     * @since Wind Web Server 2.0
     */
    public String getName ()
        {
        return myName;
        }

    /**
     * Set the updated name of this proxy.
     *
     * Note: this should ONLY be used by DirectGetNextProxy, which supports
     * changes in the symbol name.
     *
     * @param String containing the proxy`s new symbol name.
     * @since Wind Manage Web 4.3
     */
    public void setNewName ( String newName )
        {
		if (true || isvariableinstance)
			{
	        myName = newName;
			}
        }

    /**
     * Provide the value of the symbol represented by the proxy in Live Control
     * Protocol encoding. Please have a look at the symbol type handler within
     * the embedded system (httpSymValToNetPrint()) to get information about
     * the encoding of a distinct symbol type.
     * To be able to perform a conversion, the symbol type must be available prior to
     * calling this function.
     *
     * @param raw Encoded symbol value used to update the proxy object.
     * @since Wind Web Server 2.0
     */
    public void setRawValue ( byte[] raw )
        {
        try
            {
            if ( myConverter == null )
                myConverter = getConverter();

            byte myRaw[] = new byte[raw.length-1];

            for ( int i=1;i<raw.length;i++)
                myRaw[i-1] = raw[i];

            /* get the object representation of the byte value */

            myValue = myConverter.Raw2Data ( myRaw );
            Log.log ( 10 , "setRawValue: set value "+myValue.toString() );
            }
        catch ( Exception e )
            {
            Log.log ( "setRawValue("+myName+") failed because of "+e );
            }
        }

    /**
      * Get the current value of the proxy encoded according to the Live Control
      * protocol.
      * This byte array is the same one as received by the appropriate symbol
      * type handler (using httpSymValSetByNet()) in the target system.
      * @return Byte array containing the proxy`s value encoded for Live Control
      * network transmission.
      */
    public byte[] getRawValue () {
      byte[] result = null;

      if ( myConverter == null ) {
        myConverter = getConverter();
      }
      try {
        result = myConverter.Data2Raw ( myValue );
      } catch ( Exception e ) {
        Log.log ( "getRawValue("+myName+") failed!" );
      }
      return result;
    }

    /**
      * Set the current value of the proxy.
      * @param newvalue new value.
      */
    public void setValue(Object newvalue) {
      Log.log ( 25,"setValue ("+newvalue+") for symbol "+getName()+" called." );
      myValue = newvalue;
    }

    /**
      * Get the current value of the proxy.
      * @return current value or null if none available.
      */
    public Object getValue() {
      return myValue;
    }

    /**
      * Determine if this Proxy is useable for read access by widgets
      * @return true if this proxy may be read or false if not.
      */
    public boolean isReadProxy() {
      return isreadable;
    }

    /**
      * Determine if this Proxy is useable for write access by widgets
      * @return true if this proxy accepts new values by widgets or false if not.
      */
    public boolean isWriteProxy() {
      return iswritable;
    }

      /**
       * Set the proxy status and then notify status listeners.
       * @param x new status.
       */
    public void setEnabled(boolean x) {
      enabled = x;
    }

      /**
       * Get the proxy status
       * @return true if proxy is active or false if proxy does not have an
       * accurate symbol value.
       */
    public boolean isEnabled() {
      return enabled;
    }

      /**
       * Set the flag to allow the instance part of a name to vary (e.g. GetNext)
       * @param x new status.
       */
    public void setVariableInstance(boolean x) {
      isvariableinstance = x;
    }

      /**
       * Get the variable instance flag
       * @return true if instance part of a name can vary (e.g. GetNext)
       */
    public boolean isVariableInstance() {
      return isvariableinstance;
    }

    /**
      * Mark this Proxy as a proxy with read abilities
      * @param readable true if this proxy can receive updates from the registry
      */
    protected void setReadable(boolean readable) {
      isreadable = readable;
    }

    /**
     * Mark this Proxy as a proxy with write abilities
     * @param readable true if this proxy can send updates to the registry
     */
    protected void setWritable(boolean writable)
        {
        iswritable = writable;
        }
    }
