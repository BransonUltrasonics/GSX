/* DirectRWProxy.java - a read/write proxy interface for LC */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.directevents;

/* java imports */

import java.util.Vector;
import java.util.Enumeration;
import java.lang.String;
import java.lang.NullPointerException;
import java.lang.RuntimeException;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;

/* http imports */

import http.livecontrol.common.*;
import http.livecontrol.comm.*;
import http.livecontrol.converter.Convertible;


/**
 * Proxy implementation for simultaneous read and write access by widgets.
 * This extends DirectReadProxy to provide the write access to the symbol's
 * value from a <code>DataObjectChangeSupport</code> object using this class
 * as a listener.
 *
 * @since Wind Web Server 2.0
 */
public class DirectRWProxy extends DirectReadProxy implements DataObjectChangeListener
    {
    // private vars
    private boolean     consumersinitialized = false;
    private boolean     enforceevents = false;

    /**
     * constructor. Given a symbolname as a <code>String</code>, it
     * initializes the super class, <code>DirectReadProxy</code>, and
     * sets the isWritable() status to <b><code>true</code></b>.
     *
     * @param  symbolname  The name of the symbol that this proxy is
     *                     attached to.
     * @since Wind Web Server 2.0
     */
    public DirectRWProxy (String symbolname)  throws NullPointerException
        {
        super(symbolname);
        setWritable(true);
        }

    /**
     * Set a new value using the transmission encoding.
     * The DirectRegistry calls this method with the raw transmission data
     * originating from the symbol type handler of the represented symbol.
     * @param newvalue new value.
     * @since Wind Web Server 2.0
     */
    public void setRawValue ( byte[] raw )
        {
        super.setRawValue ( raw );
        if (!consumersinitialized)
            {
            fireDataObjectChange();
            consumersinitialized = true;
            }
        }

    /**
     * Set a new value.
     * This method is used by widgets that want to update the server side symbol.
     * @param newvalue new value.
     * @since Wind Web Server 2.0
     */
    public void setValue(Object newvalue)
        {
        super.setValue(newvalue);
        if ( newvalue != null ) {
            try {
                lcomm.setValue ( getName() , getRawValue() );
                }
            catch (Exception e)
                {
                Log.log ( 5,"setValue("+myValue+",name="+getName()+",) can not send data! (reason: "+e+")!" );
                }
            }
        }

    /**
     * Implementation of interface PropertyChangeListener.
     * This method is called by the DirectRegistry to notify a proxy about
     * connection status or when it should send subscribe commands to the server.
     * @param e new property
     * @since Wind Web Server 2.0
     */
    public synchronized void propertyChange(PropertyChangeEvent e)
        {
        if ( e.getPropertyName().equals(".Registry") )
            consumersinitialized = false;
        super.propertyChange(e);
        }

    /**
     * Implementation of interface DataObjectChangeListener.
     * The DirectRegistry notifies the proxy about value updates using this method.
     * @param data new value
     * @since Wind Web Server 2.0
     */
    public synchronized void dataObjectChange (DataObjectChangeEvent data)
        {
        Log.log ( 25,"dataObjectChange ("+data+") called." );
        setValue(data.getNewValue());
        }
    }
