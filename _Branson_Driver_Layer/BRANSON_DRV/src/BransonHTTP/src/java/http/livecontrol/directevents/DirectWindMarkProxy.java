/* DirectWindMarkProxy.java - a WindMark symbol-superset Get/GetNext/Set proxy interface for LC */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,11feb04,dlr  Add GET and GETNEXT support
01a,04jan04,dlr  Create for WM Web 4.3, based on DirectRWProxy
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
import java.io.*;

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
public class DirectWindMarkProxy extends DirectReadProxy implements DataObjectChangeListener
    {
	// public constants
    public final static int GET_REQUEST 		= 0x1;
    public final static int GETNEXT_REQUEST 	= 0x2;
    public final static int SET_REQUEST 		= 0x3;

    // private vars
    private boolean     consumersinitialized = false;
    private boolean     enforceevents = false;
    private int         requesttype   = GET_REQUEST;

    /**
     * constructor. Given a symbolname as a <code>String</code>, it
     * initializes the super class, <code>DirectReadProxy</code>, and
     * sets the isWritable() status to <b><code>true</code></b>.
     *
     * @param  symbolname  The name of the symbol that this proxy is
     *                     attached to.
     * @since Wind Web Server 2.0
     */
    public DirectWindMarkProxy (String symbolname, int RequestType)  throws NullPointerException
        {
        super(symbolname);
        setWritable(true);
        setRequestType(RequestType);

        Log.setVerbosityLevel(5);
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
	* overrides DirectReadProxy.subscribe (to avoid registering WindMark names that
	* are not ready) which will normally register the symbol with the <code>LiveControlCommunication</code>
	* object (and the web server). The <code>LiveControlCommunication</code> object will communicate with
	* the web server's <code>Live Control</code> RPM to subscribe this symbol's name.
	*
	* @returns <code>true</code> if successful or <code>false</code> otherwise.
	* @since Wind Manage Web 4.3
	*/
	protected boolean subscribe ( )
     	{
		if (0 != getName().length())
			return ( super.subscribe() );

		return true;
	 	}

	protected void unsubscribe ( )
     	{
		if (0 != getName().length())
			super.unsubscribe();
		}

   /**
     * Set a new value.
     * This method is used by widgets that want to update the server side WindMark.
     * @param newvalue new value.
     * @since Wind Manage Web 4.3 - functionaly identical the setValue
     */
    public void setWindMark(Object newvalue)
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
      * Gets a single WindMark value from the server
      * The symbol does not get registered for automatic updates by the server
      * The user leaves this symbol registered so that later requests do not
      * incure an description packet overhead
      *
      * @see LiveControlCommunication
      * @since Wind Manage Web 4.3
      */
     protected void getWindMark( )
         {
         try
             {
             lcomm.getValue( getName() );
             }
         catch ( Exception e )
             {
             Log.log ( 5,"getValue(name="+getName()+",) can not send data! (reason: "+e+")!" );
             }
         }

    /**
      * Gets a single WindMark value from the server: expects an instance value
      * The reply should be the value with the WindMark name advanced with the next instance
      *  e.g. myTableIndex.112 -> myTableIndex.113  WM=myTableIndex, Instance=122 then 113
      *
      * The symbol does not get registered for automatic updates by the server
      * The user leaves this symbol registered so that later requests do not
      * incure an description packet overhead
      *
      * @see LiveControlCommunication
      * @since Wind Manage Web 4.3
      */
    protected void getNextWindMark( )
         {
         try
             {
             lcomm.getNextValue ( getName() );
             }
         catch ( Exception e )
             {
             Log.log ( 5,"getNextValue(name="+getName()+",) can not send data! (reason: "+e+")!" );
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
        	{
            consumersinitialized = false;

            myRegistry = (Registry)e.getNewValue();
            lcomm = myRegistry.getCommunication();
            setConverter ( getConverterObject() );
            unsubscribe();

            /* We do not automatically subscribe, as DirectReadProxy would do.
             * We wait for next explicit request !
             */
			}
		else if (e.getPropertyName().equals(".Name") )
			{
			/* A new name coming this way is from a changing instance */

			/* if WindMark name change, update symbol table */
			if (!getName().equals((String) e.getNewValue()))
				{
				lcomm.getSymbolTable().replaceInstance (getName(), (String) e.getNewValue());
				setNewName( (String) e.getNewValue() );
				}

			/* if there is a widget hold a GetNext name, it is listening for a change */
			 Enumeration c = statuslisteners.elements();
			 DataObjectStatusEvent d = new DataObjectStatusEvent(this,".Name",null,new String( getName() ) );
			 while ( c.hasMoreElements() )
				 {
				 fireDataObjectStatus((DataObjectStatusListener)c.nextElement(),d);
				 }

            /* NOTE:do not pass this event to the super, since only this DirectProxy accepts this event */
			}
        else
        	super.propertyChange(e);
        }

    /**
     * Implementation of interface DataObjectChangeListener.
     * The DirectRegistry notifies the proxy about name updates using this method.
     * @param data new name
     * @since Wind Manage Web 4.3
     */
    public synchronized void dataObjectChange (DataObjectChangeEvent data)
        {
        Log.log ( 25,"dataObjectChange ("+data.getPropertyName()+") called." );

		if (data.getPropertyName().equals(".Name") )
			{
			/* This widget is supplying the WindMark name (+ instance) */

			/* Is this WindMark name already in use ? */
	        AbstractProxy testProxy;
			testProxy = (AbstractProxy) ((DirectRegistry) myRegistry).getProxy( (String) data.getNewValue() ,true);
			if ((null != testProxy) && (this != testProxy))
				{
				Enumeration c = statuslisteners.elements();
				StringBuffer retName = new StringBuffer((String) data.getNewValue());
				retName.insert(0,"'");
				retName.append("' is in use");
				DataObjectStatusEvent d = new DataObjectStatusEvent(this,".Name",null, retName.toString() );

				while ( c.hasMoreElements() )
					{
					fireDataObjectStatus((DataObjectStatusListener)c.nextElement(),d);
				 	}

				return;
				}


			/* if WindMark name change, unsubscribe previous */
			if (!getName().equals((String) data.getNewValue()) )
				{
				consumersinitialized = false;

				/* Take the old name off the LC server */
				unsubscribe();

				/* Apply new name */
				setNewName( (String) data.getNewValue() );

				/* Assume that the data type is going to change */
				setConverter(null);
				}

			/* Send Request */
			if (0 != getName().length())
				{
				if (GET_REQUEST == requesttype)
					getWindMark();
				else if (GETNEXT_REQUEST == requesttype)
					getNextWindMark();
				}
			}
		else
			{
			/* This widget is supplying the WindMark value */
			setWindMark(data.getNewValue());
			}
        }


    /**
      * Return request type
      * @return GET_REQUEST or GETNEXT_REQUEST
      */
    public int getRequestType() {
      return requesttype;
    }

	/**
      * Set request type
	 * @param is GET_REQUEST or GETNEXT_REQUEST
	 */
	protected void setRequestType(int requestType)
		{
		if (GET_REQUEST == requestType)
			{
			setVariableInstance(false);
			requesttype = requestType;
			}
		else if (GETNEXT_REQUEST == requestType)
			{
			setVariableInstance(true);
			requesttype = requestType;
			}
		else if (SET_REQUEST == requestType)
			{
			setVariableInstance(false);
			requesttype = requestType;
			}
		}
    }
