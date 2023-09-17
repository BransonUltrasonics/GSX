/* DirectRegistry.java - maintins the connection to the server and proxies
 *                       components via DirectReadProxy and DirectWriteProxy.
 */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02c,22mar07,ps  rename variable "enum". Defect WIND00091285
02b,11feb04,dlr  Add GET, GETNEXT, Instance support
02a,20oct03,dlr  Port to WM Web
01a,11aug99,csr created.
*/

package http.livecontrol.directevents;

/* Java imports */

import java.io.IOException;
import java.net.URL;
import java.beans.PropertyChangeSupport;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;
import java.util.Hashtable;
import java.util.Enumeration;

/* LiveControl imports */

import http.livecontrol.comm.LiveControlCommunication;
import http.livecontrol.comm.LiveControlConnection;
import http.livecontrol.common.Proxy;
import http.livecontrol.common.AbstractProxy;
import http.livecontrol.converter.ConverterFactory;
import http.livecontrol.comm.Registry;
import http.livecontrol.comm.Logging;
import http.livecontrol.comm.Symbol;

/**
 * This class is the partner API for Live Control proxy components.
 * It is used to tie proxy components to the <code>LiveControlCommunication</code>
 * object. This object manages <code>DirectReadProxy</code> and <code>DirectWriteProxy</code>
 * objects.  It contains a <code>ConverterFactory</code> for converting a <code>byte</code>
 * array into a meaningful data object and visa versa. <code>PropertyChangeSupport</code>
 * is used to update the <code>LiveCommunication</code> object referrenced by
 * the Proxy types and to set thier enabled status.
 *
 * @see DirectReadProxy
 * @see DirectWriteProxy
 * @since Wind Web Server 2.0
 */
public class DirectRegistry implements PropertyChangeListener,Registry {

    // private vars

    /**
     * the <code>URL</code> of the web server
     */
    private URL                      myServerURL;

    /**
     * The object that handles socket layer communication with the
     * web server.
     */
    private LiveControlCommunication myComm = null;

    /**
     * the set of convertibles that convert data to raw byte arrays and
     * back again.
     */
    private ConverterFactory         myConverters;

    /**
     * for notifying <code>DirectReadProxy</code> and <code>DirectWriteProxy</code>
     * objects that either the communication has changed or the enabled status
     * has changed.
     */
    private PropertyChangeSupport    myProxies = new PropertyChangeSupport(this);

    /**
     * the listeners that must here updates to let thier attached widgets (components)
     * know that data object values or statuses have changed.
     */
    private Hashtable                myReadProxies = new Hashtable();

    /**
     * for debugging.
     */
    private Logging Log = new Logging (java.lang.System.out,5,"DirectRegistry: ");

    // constructors

    /**
     * constructor for <code>DirectRegistry</code> objects.  Given the server's url in a
     * URL object, the constructor initializes a <code>DirectRegistry</code> object.
     * It creates the <code>ConverterFactory</code> object and initializes the
     * <code>LiveCommunication</code> object.
     *
     * @param serverURL   The object that contains the url to the web server; the
     *                    <code>URL</code> should point to a location that has been
     *                    configured as the <code>LiveControl RPM</code> path on the
     *                    web server.
     * @since Wind Web Server 2.0
     */
    public DirectRegistry ( URL serverURL ) throws IOException
        {
        myConverters = new ConverterFactory ();
        this.init ( serverURL );
        }

    /**
     * initialize the registry and the communication object for use
     * with a Live Control server located at serverURL.
     * This method is automatically called by the constructor!
     *
     * @params serverURL        <code>URL</code> object that contains the link to the
     *                          server.
     * @since Wind Web Server 2.0
     */
    public synchronized void init ( URL serverURL ) throws IOException
        {
        myServerURL = serverURL;
        stop();
        this.init();
        }

    /**
     * stop the Live Control communication.  There will be no more server
     * socket communication until <code><i>init()</i></code> is called to
     * resume communication.
     *
     * @since Wind Web Server 2.0
     */
    public synchronized void stop()
        {
        if ( myComm != null )
            {
            myComm.stop();
            }
        }

    /**
     * initialize the registry and the communication object. For
     * each proxy, a <code>PropertyChangeEvent</code> is called with
     * the disabled status.  <code>LiveControlCommunication</code> is
     * started with the server. If an exception ocurrs during an attempt
     * to connect to the server (via <code>LiveControlCommunication.init()</code>),
     * it is assumed to be because of a timeout and the routine will wait
     * 6000 ms and try again.
     * This method is automatically called by the constructor.
     */
    public synchronized void init ( ) throws IOException
        {
        Log.log ( 25,"init() called." );

        myProxies.firePropertyChange ( ".enable" , null , new Boolean(false) );

        if ( myComm == null )
            {
            myComm = new LiveControlConnection ();
            myComm.addListener ( this );
            }

        while ( myComm.isConnected() == false )
            {
            try
                {
                myComm.init ( myServerURL );
                }
            catch (IOException x)
                {
                Log.log ( 25,"init(): waiting (Exception "+x+" occured)...." );
                try
                    {
                    java.lang.Thread.sleep(6000);
                    }
                catch (Exception xxx)
                    {
                    }
                }
            }
        }


    // -------------------------------------------
    // implementation of interface PropertyChangeListener

    /**
     * This method is called by <code>PropertyChangeSupport</code> and
     * handles <code>PropertyChangeEvent</code>s of the type ".reconnected",
     * ".enable", and ".Registry".  If the property is ".reconnected" and the
     * value is <code>null</code>, this fires a new <code>PropertyChangeEvent</code>
     * at each of the Proxy objects with a ".enable" property and a value of false.
     * If it is not <code>null</code>, it fires a new <code>PropertyChangeEvent</code>
     * for the ".Registry" property at each of the proxies.  All other
     * <code>PropertyChangeEvent</code> cause each of the proxies to set the raw value
     * of their <code>Symbol</code> object to the value of the event.
     *
     * @param e  The event that led the invocation of this method.
     * @since Wind Web Server 2.0
     */
    public void propertyChange(PropertyChangeEvent e)
        {
        AbstractProxy targetProxy;
        boolean hasInstanceChanged = false;

        Log.log ( 10,"got propertyChange("+e.getPropertyName()+")" );
        if ( e.getPropertyName().equals ( ".reconnected" ) )
            {
            if ( e.getNewValue() == null )
                {
                myProxies.firePropertyChange ( ".enable" , null , new Boolean(false) );
                }
            else
                {
                myProxies.firePropertyChange ( ".Registry" , null,this );
                }
            }
		else if (e.getPropertyName().equals(".Name") )
			{
			/* Derived Proxys will override this property change */
			}
        else
            {

            targetProxy = (AbstractProxy) getProxy( (String) e.getPropertyName() ,true);

			if ( null != targetProxy )
                {
				/* set the new value */
                targetProxy.setRawValue ( (byte[])e.getNewValue() );

                /* if new instance, fire off the new name change */
                if ( !targetProxy.getName().equals((String) e.getPropertyName()) )
                	{
					//	targetProxy.setNewName ( e.getPropertyName() );
					targetProxy.propertyChange(
						new PropertyChangeEvent(this,".Name" , null , new String((String)e.getPropertyName()) )
						/*".Name" , null , (String) e.getPropertyName()*/
						);

					}
                }
            }
        }

  // ===========================================

  // -------------------------------------------
  // implementation of interface Registry


    /**
     * register a new Proxy object.
     * The proxy object has to identify itself as a read or write or read-write-proxy.
     * The identification is performed asking the proxy object's
     * isReadProxy()/isWriteProxy() method calls. If the object is not
     * an instance of any of these classes, <code>IllegalArgumentException</code>
     * is thrown.
     *
     * @param  newProxy  The new <code>PropertyChangeListener</code> to add.
     * @since Wind Web Server 2.0
     */
    public void addPropertyChangeListener ( PropertyChangeListener newProxy)
        {
        Log.log (25,"DirectRegistry:addPropertyChangeListener() called." );

        if ( newProxy instanceof AbstractProxy )
            {
            AbstractProxy proxy = (AbstractProxy)newProxy;
            if ( proxy.isReadProxy() )
                {
                addReadProxy (proxy);
                }
            if ( proxy.isWriteProxy() )
                {
                addWriteProxy (proxy);
                }
            if ( ( !proxy.isReadProxy() ) && ( !proxy.isWriteProxy() ) )
                {
                throw new java.lang.IllegalArgumentException ( "argument pretends to be neigther readable nor writable proxy!" );
                }
            }
        else
            {
            throw new java.lang.IllegalArgumentException ( "argument is not instance of AbstractProxy!" );
            }

        newProxy.propertyChange (new PropertyChangeEvent(this,".enable" , null , new Boolean(false) ));
        Log.log(25, "DirectRegistry:addPropertyChangeListener - calling propertyChange() for .Registry");
        newProxy.propertyChange (new PropertyChangeEvent(this,".Registry",null,this));
        }

    /**
     * deregister a Proxy object. The argument must be an instance of
     * <code>AbstractProxy</code>.  The object will no longer receive
     * updates of <code>PropertyChangeEvent</code>s.
     *
     * @param oldProxy  The <code>PropertyChangeListener</code> to remove.
     */
    public void removePropertyChangeListener ( PropertyChangeListener oldProxy)
        {
        Log.log ( 25,"removePropertyChangeListener() called." );
        if ( oldProxy instanceof AbstractProxy )
            {
            AbstractProxy proxy = (AbstractProxy)oldProxy;
            removeReadProxy (proxy);
            removeWriteProxy (proxy);
            }
        else
            {
            throw new java.lang.IllegalArgumentException ( "argument is not instance of AbstractProxy!" );
            }
        }

    /**
     * Add a new <code>DirectReadProxy</code> to the list of <code>PropertyChangeListener</code>
     * objects. <code>DirectReadProxy</code> objects are also added to a
     * hashtable for quick access. This is called by addPropertyChangeListener().
     *
     * @param newProxy  The new AbstractProxy to add.
     * @since Wind Web Server 2.0
     */
    protected void addReadProxy ( AbstractProxy newProxy )
        {
        Log.log ( 25,"addReadProxy() called." );
        myProxies.addPropertyChangeListener ( newProxy );
        myReadProxies.put ( newProxy.getName() , newProxy );
        }

    /**
     * add a new <code>DirectWriteProxy</code> or a <code>DirectRWProxy</code>
     * to the <code>PropertyChangeListener</code> list. This is called by
     * addPropertyChangeListener().
     *
     * @param newProxy  The new AbstractProxy to add.
     * @since Wind Web Server 2.0
     */
    protected void addWriteProxy ( AbstractProxy newProxy )
        {
        Log.log ( 25,"addWriteProxy() called." );
        myProxies.addPropertyChangeListener ( newProxy );
        }

    /**
     * deregister a readable Proxy component. d
     *
     * @param oldProxy  The proxy to remove from the list of proxies.
     * @since Wind Web Server 2.0
     */
    protected void removeReadProxy ( AbstractProxy oldProxy )
        {
        Log.log ( 25,"removeReadProxy() called." );
        myReadProxies.remove ( oldProxy.getName());
        myProxies.removePropertyChangeListener ( oldProxy );
        }

    /**
     * removes a write proxy from the list.
     *
     * @param oldProxy  The proxy to remove from the list of proxies
     *                  (<code>PropertyChangeListeners</code>)
     * @since Wind Web Server 2.0
     */
    protected void removeWriteProxy ( AbstractProxy oldProxy )
        {
        Log.log ( 25,"removeWriteProxy() called." );
        myProxies.removePropertyChangeListener ( oldProxy );
        }

    /**
     * get Proxy with name "name".
     *
     * throws nullpointer exception if no such Proxy is available
     *
     * @param name  The name of a symbol which is attached to a
     *              DirectReadProxy.
     * @returns a DirectReadProxy if one is found, or null.
     * @since Wind Web Server 2.0
     */
    public Proxy getProxy ( String name )
        {
        return (Proxy)myReadProxies.get(name);
        }

    /**
     * get Proxy with name "name". Flag allows for ignoring a difference in instance
     *
     * throws nullpointer exception if no such Proxy is available
     *
     * @param name  The name of a symbol which is attached to a
     *              DirectReadProxy.
     * @returns a DirectReadProxy if one is found, or null.
     * @since Wind Manage Web 4.3
     */
    public Proxy getProxy ( String name, boolean ignoreInstance)
        {
        AbstractProxy targetProxy;

	    targetProxy = (AbstractProxy) myReadProxies.get(name);

		if ((null == targetProxy) && ignoreInstance)
	        {
            /* if fully qualified name is not found (e.i. name[.instance]|[(arg)]) then
             * try the root name for a match */

			Enumeration enumval  = myReadProxies.elements();
			int keyNameLength = Symbol.getWindMarkNameLength( name );
			String keyName    = name.substring(0,keyNameLength);

			while (enumval.hasMoreElements())
				{
				AbstractProxy tstProxy = (AbstractProxy) enumval.nextElement();

				String        tstName  = tstProxy.getName();

				int      tstNameLength = Symbol.getWindMarkNameLength( tstName );

				/* same root name? */
				if ((tstProxy.isVariableInstance() ) &&
					(keyNameLength == tstNameLength) &&
					(tstName.startsWith(keyName)   ) )
					{
					targetProxy        = tstProxy;
					break;
					}

				/* same exact name? */
				if ((name.equals(tstName)) )
					{
					targetProxy        = tstProxy;
					break;
					}
				}
			}

		return ( (Proxy) targetProxy );
        }


    /**
     * provide the communication object to proxies
     * @return reference to Live Control communication object
     * @since Wind Web Server 2.0
     */
    public LiveControlCommunication getCommunication ()
        {
        return myComm;
        }

    /**
     * get the <code>ConverterFactor</code> data object.
     *
     * @returns a referrence to the private <code>ConverterFactory</code>
     *          object.
     * @since Wind Web Server 2.0
     */
    public ConverterFactory getConverterFactory ()
        {
        return myConverters;
        }

};
