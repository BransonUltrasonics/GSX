/* DirectReadProxy.java - read proxy implementation */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
01a,18Oct99,csr    Added comments and improved clarity.
*/

package http.livecontrol.directevents;

/* Java Imports */
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
 * The DirectReadProxy is able to receive value updates from the DirectRegistry.
 * It will distribute these updates to any widget registered at this DirectReadProxy instance.
 * There is only unidirectional information flow, that means, DirecReadProxy is not able to
 * receive value updates from widgets.
 *
 * @see http.livecontrol.directevents.DirectWriteProxy
 * @since Wind Web Server 2.0
 */
public class DirectReadProxy extends AbstractProxy
														 implements DataObjectChangeSupport,DataObjectStatusSupport
    {

    // private variables

    /**
     * the <code>Registry</code> object which manages symbols and their values.
     */
    protected Registry                   myRegistry;

    // extendable data objects

    /**
     * for debugging.
     */
    protected Logging                  Log = new Logging (System.out, 1 , "DirectReadProxy: ");

    /**
     * The interface between this object and the server. It handles <code>socket</code>
     * read/write operations.
     */
    protected LiveControlCommunication lcomm;

    /**
     * The <code>Vector</code> of <code>DataObjectChangeListener</code> objects
     * which will receive <code>DataObjectChangeEvent</code> updates.
     */
    protected Vector                   consumers;

    /**
     * The <code>Vector</code> of <code>DataObjectStatusListener</code> objects
     * which will receive <code>DataObjectStatusEvent</code> updates.
     */
    protected Vector                   statuslisteners;

    /**
     * The property name expected by <code>propertyChange()</code> for when
     * the internal <code>Registry</code> object changes.
     */
    protected final String REGISTRY_PROPERTY = ".Registry";

    /**
     * The property name expected by <code>propertyChange()</code> for when
     * the symbol's enabled status changes.
     */
    protected final String ENABLE_PROPERTY = ".enable";

    /**
     * The property name expected by <code>propertyChange()</code> for when
     * the symbol's LiveControl Polling priority.
     *
     * PRIORITY_NONE = New values are push solely by the respective handler
	 * PRIORITY_MED  = New values are polled for at a medium rate (~1   second) (DEFAULT)
	 * PRIORITY_HIGH = New values are polled for at a high   rate (~1/3 second)
     */
    public static final String PRIORITY_NONE = ".priorityNone";
    public static final String PRIORITY_MED  = ".priorityMedium";
    public static final String PRIORITY_HIGH = ".priorityHigh";

    protected       String pollingPriority;

    // class methods

    /**
     * Create a new DirectReadProxy for a distinct symbol.
     *
     * @param symbolname Name of the symbol represented by this instance.
     * @throws NullPointerException If <code>symbolname</code> is not found
     * @since Wind Web Server 2.0
     */
    public DirectReadProxy (String symbolname)  throws NullPointerException
        {
		super(symbolname);
		setReadable(true);
		setWritable(false);
        setPollingPriority(PRIORITY_MED);
		init();
        }

    /**
     * Create a new DirectReadProxy for a distinct symbol, with polling priority
     *
     * @param symbolname Name of the symbol represented by this instance.
     * @param pollingPriority Type of value polling update requested.
     * @throws NullPointerException If <code>symbolname</code> is not found
     * @since Wind Manage Web 4.3
     */
    public DirectReadProxy (String symbolname,String pollingPriority)  throws NullPointerException
        {
        super(symbolname);
        setReadable(true);
        setWritable(false);
        setPollingPriority(pollingPriority);
        init();
        }

     /**
      * Executed as this object is garbage collected.  It unsubscribes the symbol
      * attached to this <code>DirectReadProxy</code> and then sets registry and
      * communication data objects to <code>null</code>.
      *
      * @see DirectReadProxy#unsubscribe
      * @see Registry
      */
     public void finalize() throws Throwable
         {
         try
             {

             // unsubscribe this proxy

             this.unsubscribe();
             }
         catch (Exception e)
             {
             }

         myRegistry = null;
         lcomm = null;
         }

     /**
      * Given a representation of an object as a <code>byte</code> array,
      * This method will set the internal value of the data object. The
      * data converter is obtained from this objects <code>Registry</code>.
      *
      * @since Wind Web Server 2.0
      */
     public void setRawValue ( byte[] raw )
         {
         super.setRawValue ( raw );
         setEnabled(true);
         fireDataObjectChange();
         }

     /**
      * initializes the data elements of this object.  It does <b>NOT</b>
      * call <code>subscribe()</code> for the symbol.
      *
      * @since Wind Web Server 2.0
      */
     public void init ()
         {
         Log.log ( 25,"init() called." );
         consumers = new Vector();
         statuslisteners = new Vector();
         }

     /**
      * subscribes the name of the symbol with the <code>LiveControlCommunication</code>
      * object (and the web server). The <code>LiveControlCommunication</code> object will communicate with
      * the web server's <code>Live Control</code> RPM to subscribe this symbol's name.
      *
      * @returns <code>true</code> if successful or <code>false</code> otherwise.
      * @since Wind Manage Web 4.3
      */
     protected boolean subscribe ( )
         {
         try
			{
			/* No need to subscribe the empty name */
			if (getName().equals(""))
				{
				return true;
				}

			 if      (pollingPriority == PRIORITY_NONE)
			 	lcomm.subscribe ( getName(), LiveControlConnection.HTTP_LC_PACKET_SUBSCRIBE_NONE );
			 else if (pollingPriority == PRIORITY_HIGH)
			 	lcomm.subscribe ( getName(), LiveControlConnection.HTTP_LC_PACKET_SUBSCRIBE_HIGH );
			 else
			 	lcomm.subscribe ( getName() );
             }
         catch ( Exception e )
             {
             return false;
             }
         return true;
         }

     /**
      * unsubscribes the name of the symbol from the <code>LiveControlCommunication</code>
      * object. Subsequent accesses to this symbol will result in a null. There is no
      * way to determine if this action succeeds.  All exceptions are caught.
      *
      * @see LiveControlCommunication
      * @since Wind Web Server 2.0
      */
     protected void unsubscribe ( )
         {
         try
             {
             lcomm.unsubscribe ( getName() );
             }
         catch ( Exception e )
             {
             }
         }


     /**
      * returns the <code>LiveControlCommunication</code> object that handles server
      * interraction.
      *
      * @returns <code>LiveControlCommunication</code> data object.
      *
      * @see LiveControlCommunication
      * @since Wind Web Server 2.0
      */
     public LiveControlCommunication getCommunication ()
         {
         return lcomm;
         }

     // implementation of interface DataObjectStatusSupport

     /**
      * Adds a status listener to this <code>DirectReadProxy</code>. The status listener
      * responds to a <code>DataObjectStatusEvent</code>.  After the listener is added,
      * it fires a <code>DataObjectStatusEvent</code> using <code>fireDataObjectStatus</code>.
      * The old value of the event is set to <code>null</code> and the new value is set
      * to the <code>Boolean</code> representation of the <code>DirectReadProxy</code>'s
      * enabled status.
      *
      * @param l <code>DataObjectStatusListener</code> that will handle events fired from
      *          <code>fireDataObjectStatus()</code>.
      *
      * @see DataObjectStatusListener
      * @since Wind Web Server 2.0
      */
     public synchronized void addDataObjectStatusListener ( DataObjectStatusListener l )
         {
         if (!statuslisteners.contains(l))
             {
             statuslisteners.addElement ( l );
             fireDataObjectStatus ( l,new DataObjectStatusEvent(this,getName(),null,new Boolean(isEnabled()) ) );
             }
         }

     /**
      * removes a <code>DataObjectStatusListener</code> if it exists.
      *
      * @param l <code>DataObjectStatusListener</code> to be removed from the
      *          <code>Vector</code> of status listeners.
      *
      * @see DataObjectStatusListener
      * @since Wind Web Server 2.0
      */
     public synchronized void removeDataObjectStatusListener ( DataObjectStatusListener l )
         {
         statuslisteners.removeElement ( l );
         }

     /**
      * fires a <code>DataObjectStatusEvent</code>. It creates a <code>DataObjectStatusEvent</code>
      * with the name of this symbol proxied by this object, a previous value of
      * <code>null</code> and a new value containing <code>Boolean</code> representation
      * of the enabled status of the symbol.
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
      * This routine calls the <code>dataObjectStatus()</code> method for the
      * status listener specified.
      *
      * @param  l  <code>DataObjectStatusListener</code> which will be notified of
      *            the <code>DataObjectStatusEvent</code>; the object will either
      *            be enabled or disabled.
      *
      * @param  d  <code>DataObjectStatusEvent</code> that contains the new enabled
      *            status of the symbol that has changed; the event identifies a
      *            state toggle from enabled to disabled or visa versa.
      *
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

     // implementation of interface DataObjectChangeSupport

     /**
      * This routine adds a <code>DataObjectChangeListener</code> to the <code>Vector</code>
      * of change listeners, if the listener is not already in the <code>Vector</code>.
      *
      * @param  consumer  the new <code>DataObjectChangeListener</code> to accept
      *                   and handle events of type <code>DataObjectChangeEvent</code>.
      *
      * @see DataObjectChangeListener
      * @see DataObjectChangeEvent
      * @since Wind Web Server 2.0
      */
     public synchronized void addDataObjectChangeListener ( DataObjectChangeListener consumer )
         {
         if (!consumers.contains(consumer))
             {
             consumers.addElement ( consumer );
             fireDataObjectChange ( consumer,new DataObjectChangeEvent(this,getName(),null,getValue() ) );
             }
         }

     /**
      * This method removes a particular <code>DataObjectChangeListener</code> from the
      * <code>Vector</code> of change listeners (if it exists).  Subsequent
      * <code>DataObjectChangeEvent</code>s will no longer be sent to that listener.
      *
      * @param  consumer  the <code>DataObjectChangeListener</code> to remove from the
      *                   <code>Vector</code> of change listeners.
      *
      * @see DataObjectChangeListener
      * @since Wind Web Server 2.0
      */
     public synchronized void removeDataObjectChangeListener ( DataObjectChangeListener consumer )
         {
         consumers.removeElement ( consumer );
         }

     /**
      * To each change listener object, this method sends a <code>DataObjectChangeEvent</code>.
      * The event consists of the name of the symbol and its new value.  NOTE: this class
      * does not save the old value of the symbol, thus the event object <i>allways</i>
      * identifies it as <code>null</code>.
      *
      * @see  DataObjectChangeEvent
      * @see  DataObjectChangeListener
      * @since  Wind Web Server 2.0
      */
     public synchronized void fireDataObjectChange()
         {
         DataObjectChangeEvent d = new DataObjectChangeEvent(this,getName(),null,getValue());
         Enumeration c = consumers.elements();

         while ( c.hasMoreElements() )
             {

             // notify the change listener of the event

             fireDataObjectChange((DataObjectChangeListener)c.nextElement(),d);
             }
         }

     /**
      * The specified <code>DataObjectChangeListener</code>'s <code>dataObjectChange()</code>
      * method is called with the specified <code>DataObjectChangeEvent</code>.
      *
      * @param  l  <code>DataObjectChangeListener</code> which to notify that the
      *            <code>DataObjectChangeEvent</code> has occurred.
      * @param  d  <code>DataObjectChangeEvent</code> that has occurred.
      * @see  DataObjectChangeListener
      * @see  DataObjectChangeEvent
      * @since Wind Web Server 2.0
      */
     public void fireDataObjectChange(DataObjectChangeListener l, DataObjectChangeEvent d)
         {
         try
            {
            l.dataObjectChange ( d );
            }
         catch (RuntimeException e)
            {
            }
         }

     // -------------------------------------------
     // implementation of interface PropertyChangeListener

    /**
     * This is required by <code>PropertyChangeListener</code>. Class
     * <code>AbstractProxy</code> extends <code>PropertyChangeListener</code>, and
     * <code>propertyChange()</code> is implemented here. Given the
     * <code>PropertyChangeEvent</code>, e, this routine checks to see whether a
     * change in the <code>REGISTRY_PROPERTY</code> or the <code>ENABLE_PROPERTY</code>
     * has fired this <code>PropertyChangeEvent</code>.
     * <br><br>
     * A property change in the <code>Registry</code> object (as per the
     * REGISTRY_PROPERTY name) causes this object to refresh its internal
     * <code>Registry</code> object and to update the internal
     * <code>LiveCommunication</code> object with that associated with the
     * <code>Registry</code>.
     * <br><br>
     * A property change of type <code>ENABLE_PROPERTY</code> causes this routine to
     * update the object's enabled status.
     *
     * @param  e  The <code>PropertyChangeEvent</code> resulting in this method's
     *            invocation.
     * @see  PropertyChangeListener
     * @see  PropertyChangeEvent
     * @since  Wind Web Server 2.0
     */
    public void propertyChange(PropertyChangeEvent e)
        {
        Log.log ( 10,getName()+" got event "+e.getPropertyName()+" with value "+e.getNewValue() );
        if (e.getPropertyName().equals(REGISTRY_PROPERTY) == true )
            {
            myRegistry = (Registry)e.getNewValue();
            lcomm = myRegistry.getCommunication();
            setConverter ( getConverterObject() );
            unsubscribe();
            subscribe();
            }
        else
            {
            if ( e.getPropertyName().equals(ENABLE_PROPERTY)  == true )
                {
	              if ( ((Boolean)e.getNewValue()).booleanValue() == false )
	                  setEnabled(false);
                else if ( ((Boolean)e.getNewValue()).booleanValue() == true )
                    setEnabled(true);
                }
            else
                {
	              Log.log ( 0, "unknown property "+e.getPropertyName()+" received!" );
                }
            }
        }

    /**
     * This routine is used to update the polling status of this object.
     *
     * @param  x  the polling priority to which to set the WindMark (aka Symbol) at subscription.
     * @see DirectReadProxy
     * @since Wind Manage Web 4.3
     */
    public void setPollingPriority(String s)
        {

        // don't perform massive event firing if we don't need to

        if ((s == PRIORITY_NONE) ||
            (s == PRIORITY_MED ) ||
            (s == PRIORITY_HIGH) )
            {
			pollingPriority = s;
            }
        else
            {
			pollingPriority = PRIORITY_MED;
            }

        }

    /**
     * This routine is used to update the enabled status of this object.
     * It calles <code>fireDataObjectStatus()</code> after the update completes.
     *
     * @param  x  the boolean value to which to set the enabled status.
     * @see AbstractProxy
     * @since Wind Web Server 2.0
     */
    public void setEnabled(boolean x)
        {

        // don't perform massive event firing if we don't need to

        if (isEnabled() != x)
            {
            super.setEnabled(x);
            fireDataObjectStatus();
            }
        }

    /**
     * This routine returns the value of the symbol attached to this read
     * proxy. It calls <code>AbstractProxy.getValue()</code>.
     *
     * @return <code>Object</code> containing the symbol's value.
     * @see AbstractProxy
     * @since Wind Web Server 2.0
     */
    public Object getDataObject ()
        {
        String tmp = (getValue() == null) ? "null" : getValue().toString();
        Log.log (0, "in getDataObject(): getting value: " + tmp);
        return getValue();
        }

    /**
     * From the <code>LiveCommunication</code> object, lcomm, this routine
     * obtains the <code>SymbolTable</code> which should contain this symbol.
     * If the <code>SymbolTable</code> object is non-<code>null</code> and
     * the <code>Symbol</code> retrieved from it is non-<code>null</code>,
     * this routine will obtain the <code>Convertible</code> object via
     * <code>getConverterFactory().getConverter()</code> in <code>Registry</code>.
     * The method, <code>getConverter()</code>, is passed in the <code>Symbol</code>'s
     * type using <code>getType()</code> in <code>Symbol</code>.
     *
     * @return <code>Convertible</code> returned by the <code>Registry</code> object.
     * @see Convertible
     * @see Registry
     * @see SymbolTable
     * @see Symbol
     * @since Wind Web Server 2.0
     */
    protected Convertible getConverterObject ()
        {
        SymbolTable symTab=null;
        Symbol      mySymbol=null;

        try
            {
            symTab = lcomm.getSymbolTable();
            Log.log ( 30, "Symboltable status: "+symTab );

            if (symTab != null)
                mySymbol = symTab.get ( getName() );
            else
                Log.log ( 0, "getConverterObject(): symTab is null");

            Log.log ( 20, "Symbol: <"+mySymbol+">" );

            if ( (mySymbol != null) && (mySymbol.isInitialized() == true) )
                {
	              String myType = mySymbol.getType();
	              Log.log (20,"getConverterObject:Type of symbol "+getName()+": "+myType +"Factory="+myRegistry.getConverterFactory()+"...");
	              return ( myRegistry.getConverterFactory().getConverter ( myType ) );
                }

            Log.log ( 5,"Symbol <"+getName()+"> not yet initialized!" );
            }
        catch (Exception e)
            {
            Log.log ( 0,"unable to retrieve type of Proxy "+getName()+" because of "+e+".Symbol="+mySymbol+" symTab="+symTab );
            }
        return null;
        }

    }
