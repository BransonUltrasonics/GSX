/* DirectWriteProxy.java - extends AbstractProxy to provide an interface to update
 *                         server symbols.
 */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
01a,18Oct99,csr  Added comments; improved clarity; and fixed
                 SPR #29127 (DirectWriteProxy widgets
                 (DataObjectChangeSupport objects) cannot exist
                 without a DirectReadProxy (DataObjectChangeListener))
*/

package http.livecontrol.directevents;

/* Java imports */

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
 * The DirectWriteProxy is able to send value updates to the DirectRegistry.
 * It will receive these updates from any widget.
 * There is only unidirectional information flow, that means, DirecWriteProxy is not able to
 * send value updates to widgets.
 *
 * @see http.livecontrol.directevents.DirectReadProxy
 * @see http.livecontrol.common.AbstractProxy
 * @since Wind Web Server 2.0
 */
public class DirectWriteProxy extends AbstractProxy implements DataObjectChangeListener,DataObjectStatusSupport {
    // private vars

    /**
     * for debugging.
     */
    private Logging                     Log = new Logging (System.out, 5, "DirectWriteProxy: ");

    /**
     * the object that fires <code>PropertyChangeEvent</code>s for
     * <code>Registry</code> and state enabled properties
     */
    private Registry                    myRegistry;

    /**
     * interface to the web server; implements the socket level communication.
     */
    protected LiveControlCommunication  lcomm;

    /**
     * <code>Vector</code> of listeners that listen for <code>DataObjectStatusEvent</code>
     * firings.
     */
    protected Vector                    statuslisteners;

    /**
     * Create a new DirectWriteProxy for a distinct symbol. It sets this proxy
     * as unreadable. Therefore, widgets will not receive updates through this
     * proxy. If a widget needs to write and read a symbol value, it is recommended
     * use <code>DirectRWProxy</code>.  This class takes less resources and will
     * update symbol values without the need for other proxies.
     *
     * @param  symbolname  Name of the symbol represented by this instance.
     * @since Wind Web Server 2.0
     */
    public DirectWriteProxy (String symbolname)  throws NullPointerException
        {
        super(symbolname);
        setReadable(false);
        setWritable(true);
        init();
        }

    /**
     * Deregister any objects which registered with addDataObjectStatusListener() previously.
     * It reinitializes the <code>statuslisteners</code> Vector.
     * @since Wind Web Server 2.0
     */
    public synchronized void init ()
        {
        Log.log ( 25,"init() called." );
        statuslisteners = new Vector();
        }

    /**
     * Get the Live Control communication object used by this proxy.
     *
     * @since Wind Web Server 2.0
     */
    public LiveControlCommunication getCommunication ()
        {
        return lcomm;
        }

    /**
     * Assign a new value to the symbol.
     * This method is used by widgets that want to update the server side symbol.
     * @param  newvalue  new value with which to update the symbol.
     * @since Wind Web Server 2.0
     */
    public void setValue(Object newvalue)
        {
        super.setValue(newvalue);

        if ( newvalue != null )
            {
            try {
                    byte[] raw;

                    raw = getRawValue();
	            lcomm.setValue ( getName() , raw );
                    Log.log(10, "setValue() called with ("+getName()+","+new String(raw)+")");
                }
            catch (Exception e)
                {
	              Log.log ( 10,"setValue("+myValue+",name="+getName()+",) can not send data! (reason: "+e+")!" );
                }
            }
        }


    /**
     * Implementation of interface DataObjectChangeListener.
     * The <code>DirectRegistry</code> notifies the proxy about value updates using this method.
     * When a <code>DataObjectChangeEvent</code> gets fired, this object sets the
     * value of the symbol to the value contained within the <code>DataObjectChangeEvent</code>
     * object.
     *
     * @param data <code>DataObjectChangeEvent</code> containing the new value
     * @since Wind Web Server 2.0
     */
    public synchronized void dataObjectChange (DataObjectChangeEvent data)
        {
        Log.log (25, "dataObjectChange ("+data+") called." );
        setValue(data.getNewValue());
        }


    /**
     * Implementation of interface DataObjectStatusSupport.
     * Register status listeners. If the listener is already
     * added, this method returns without adding the listener.
     * Once a <i>new</i> listener is added, a <code>DataObjectStatusEvent</code>
     * is fired at that object.
     *
     * @param  l  The <code>Object</code> that will
     *            listen for <code>DataObjectStatusEvent</code> updates.
     * @since Wind Web Server 2.0
     */
    public synchronized void addDataObjectStatusListener ( DataObjectStatusListener l )
        {
        if ( (l == null) || statuslisteners.contains(l) )
            {
            String tmp = "addDataObjectStatusListener(): argument is null or ";
            tmp += "status listeners vector contains listener already.";
            Log.log (1, tmp);
            return;
            }

        statuslisteners.addElement ( l );
        DataObjectStatusEvent event =
              new DataObjectStatusEvent( this,getName(),null,new Boolean(isEnabled()) );
        fireDataObjectStatus ( l, event );
        }

    /**
     * Implementation of interface DataObjectStatusSupport.
     * Deregisters a status listeners.  If the specified object
     * is not in the status listener <code>Vector</code>, the
     * removal does nothing.
     *
     * @param l  <code>DataObjectStatusListener</code> object to remove
     *           from the <code>Vector</code>.
     * @since Wind Web Server 2.0
     */
    public synchronized void removeDataObjectStatusListener ( DataObjectStatusListener l )
        {
        statuslisteners.removeElement ( l );
        }

    /**
     * Implementation of interface DataObjectStatusSupport.
     * Send all registered status listeners the current proxy status.
     * This method will be called automatically by propertyChange()
     * if the connection status changes.
     *
     * @since Wind Web Server 2.0
     */
    public synchronized void fireDataObjectStatus()
        {
        DataObjectStatusEvent d = new DataObjectStatusEvent(this,getName(),null,new Boolean(isEnabled()));
        Enumeration c = statuslisteners.elements();

        while ( c.hasMoreElements() )
            {
            fireDataObjectStatus((DataObjectStatusListener)c.nextElement(),d);
            }
        }


    /**
     * Implementation of interface DataObjectStatusSupport.
     * Send a message to a distinct status listener.
     *
     * @param l  <code>DataObjectStatusListener</code> object that will
     *           handle status updates through a <code>DataObjectStatusEvent</code>.
     * @param d  <code>DataObjectStatusEvent</code> that contains the objects
     *           new status.
     * @see DataObjectStatusListener
     * @see DataObjectStatusEvent
     * @since Wind Web Server 2.0
     */
    public void fireDataObjectStatus(DataObjectStatusListener l,DataObjectStatusEvent d)
        {
        try
            {
            l.dataObjectStatus ( d );
            }
        catch (RuntimeException e)
            {
            }
        }


    /**
     * Implementation of interface PropertyChangeListener.
     * This method is called by the DirectRegistry to notify a proxy about
     * connection status or when it should send subscribe commands to the server.
     *
     * @param e  The new property, such as ".Registry".
     * @since  Wind Web Server 2.0
     * @see PropertyChangeListener
     * @see PropertyChangeEvent
     */
    public void propertyChange(PropertyChangeEvent e)
        {
        Log.log ( 10,"DirectWriteProxy:got event "+e.getPropertyName()+" with value "+e.getNewValue() );
        if (e.getPropertyName().equals(".Registry") == true )
            {
            myRegistry = (Registry)e.getNewValue();
            lcomm = myRegistry.getCommunication();

            try
                {

                //
                // since there is no way to know if the symbol is
                // already subscribed, we need to do an unsubscribe
                // and then a subscribe
                //

                lcomm.unsubscribe (getName());
                lcomm.subscribe (getName());
                }
            catch (Exception exception)
                {
                Log.log (0, "cannot subscribe: " + getName() + "\n" + exception);
                }

            setConverter ( getConverterObject() );

            // this calls fireObjectStatusChange()

            setEnabled(true);
            }
        else
            {
            if ( e.getPropertyName().equals(".enable") )
                {
                setEnabled(((Boolean)e.getNewValue()).booleanValue());
                }
            else
                {
                Log.log ( 0,"unknown property "+e.getPropertyName()+" received!" );
                }
            }
        }

    /**
     * Set the proxy status and then notify status listeners.
     * @param x new status.
     * @since Wind Web Server 2.0
     */
    public void setEnabled(boolean x)
        {
        super.setEnabled(x);
        fireDataObjectStatus();
        }

    /**
     * Get the converter object used for this proxy. The <code>Registry</code>
     * obtains the appropriate <code>Convertible</code> based on the type of
     * the symbol.
     *
     * @return converter object or null if none exists or symbol type not available.
     * @since Wind Web Server 2.0
     */
    protected Convertible getConverterObject ()
        {
        SymbolTable symTab=null;
        Symbol      mySymbol=null;

        try
            {
            symTab = lcomm.getSymbolTable();
            Log.log ( 20, "Symboltable status: "+symTab );
            mySymbol = symTab.get ( getName() );
            Log.log ( 20, "Symbol: "+mySymbol );
            if ( mySymbol.isInitialized() == true )
                {
                String myType = mySymbol.getType();
                Log.log (20,"Type of symbol "+getName()+": "+myType );
                return ( myRegistry.getConverterFactory().getConverter ( myType ) );
                }
            Log.log ( 5,"Symbol "+getName()+" not yet initialized!" );
            }
        catch (Exception e)
            {
            Log.log ( 0,"unable to retrieve type of Proxy "+getName()+" because of "+e+".Symbol="+mySymbol+" symTab="+symTab );
            }
        return null;
        }

    }
