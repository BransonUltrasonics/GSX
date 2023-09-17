/* TextWidget.java - Text widget */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02b,11feb04,dlr  Text Widget can now signal WindMark names
02a,20oct03,dlr  Port to WM Web
*/


package http.livecontrol.directevents;

/* Java imports */

import java.lang.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;

/* http imports */

import http.livecontrol.common.*;
import http.livecontrol.comm.*;

/**
 * Example awt textfield widget. The class does the same work as
 * NumberWidget, except that it does not restrict values to
 * numbers.
 *
 * <b>Note</b>: <i>This class is unsupported.</i>
 */
public class TextWidget
  extends TextField
  implements TextListener,KeyListener,DataObjectChangeListener,DataObjectChangeSupport,DataObjectStatusListener
    {

    // local variables

    /**
     * for debugging.
     */
    protected final Logging Log = new Logging (System.out, 30, "TextWidget: ");

    /**
     * list of objects whom will listen for <code>DataObjectChangeEvent</code>s
     */
    protected Vector        myConsumers = new Vector();

    /**
     * the character to listen to inorder to fire a DataObjectChangeEvent
     */
    protected char keyCharFire;

    /**
     * Flag used when some other mechanism, like a button, is to trigger DataObjectChange
     */
    protected boolean updateByKeyPress;

    /**
     * Name of the property the this widget is exporting, = "textwidget" | ".Name"
     */
    protected String propertyName;

    /**
     * default constructor.  The <code>TextField</code> will contain no data,
     * and DataObjectChangeEvents will be fired on every key
     */
    public TextWidget()
        {
        super( );
        keyCharFire      = '\0';
 		updateByKeyPress = true;
 		propertyName     = "textwidget";
        init();
        }

    /**
     * constructor.  This sets the text of the <code>TextField</code>
     * Otherwise, it is the same as the default constructor.
     *
     * @param text  The initial value of the <code>TextField</code>.
     */
    public TextWidget ( String text )
        {
        super ( text );
        keyCharFire      = '\0';
 		updateByKeyPress = true;
 		propertyName     = "textwidget";
 		init();
        }

    /**
     * constructor.  Initializes the <code>TextField</code> with a size.
     * Otherwise, it is the same as the default constructor.
     *
     * @param size  The initial size of the <code>TextField</code>.
     */
    public TextWidget ( int size )
        {
        super ( size );
        keyCharFire      = '\0';
 		updateByKeyPress = true;
 		propertyName     = "textwidget";
        init();
        }

    /**
     * constructor. Initializes the <code>TextField</code> with data and
     * a size. Otherwise, it is the same as the default constructor.
     * @param text  The initial value of the TextField.
     * @param size  The initial size of the TextField.
     */
    public TextWidget ( String text , int size )
        {
        super ( text ,size );
        keyCharFire      = '\0';
 		updateByKeyPress = true;
 		propertyName     = "textwidget";
        init();
        }

    /**
     * Initialize TextWidget with a character to listen for.
     * When this key is hit, <code>fireDataObjectChange()</code>
     * is called. An uninitialized value ('\0') causes the
     * fireDataObjectChange() method to be called on every
     * TextEvent.
     *
     * @param text  The initial value of the TextField.
     * @param size  The initial size of the TextField.
     * @param keyChar  When the user hits this key value, the
     *                 DataObjectChangeEvent is fired.
     *
     */
    public TextWidget ( String text, int size, char keyChar)
        {
        this (text, size);
        keyCharFire      = keyChar;
 		updateByKeyPress = true;
 		propertyName     = "textwidget";
        }

    /**
     * adds a text and key listener to this class.
     */
    public void init ()
        {
        addTextListener ( this );
        addKeyListener ( this );
        }

    // implementation of interface DataObjectStatusListener

    /**
     * callback for DataObjectStatusListener.  It sets the enabled
     * status of the <code>TextField</code> to the <code>Boolean</code>
     * value of x.getNewValue().
     *
     * @param x  <code>DataObjectStatusEvent</code> spawning this method's
     *           invocation.
     */
    public void    dataObjectStatus(DataObjectStatusEvent x)
        {
        if ( x.getPropertyName().equals(".Name") )
        	{
			/* This name change is propagating from DirectWindMarkProxy */
			if (propertyName.equals(".Name"))
				{
				/* This widget holds a WindMark name that is now changing */
	        	setText ( (String) x.getNewValue() );
				}
			}
		else
			{
	        setEnabled(((Boolean)x.getNewValue()).booleanValue());
			}
        }

    // implementation of interface Producer

    /**
     * adds a listener to the list of consumers.
     *
     * @param consumer  A <code>DataObjectChangeListener</code> that will
     *                  updates its internal data objects when <code>NumberWidget</code>
     *                  fires a <code>DataObjectChangeEvent</code>.
     */
    public void addDataObjectChangeListener ( DataObjectChangeListener consumer )
        {
        Log.log ( 25,"addDataObjectChangeListener called." );

        myConsumers.addElement ( consumer );
        }

    /**
     * removes a listener from the list of consumers.
     *
     * @param consumer  listener to remove.
     * @see DataObjectChangeListener
     * @see NumberWidget#addDataObjectChangeListener
     */
    public void removeDataObjectChangeListener( DataObjectChangeListener consumer )
        {
        Log.log ( 25,"removeDataObjectChangeListener() called." );
        myConsumers.removeElement ( consumer );
        }

    /**
     * This method is called when the TextListener or KeyListener determines that
     * an appropriate character has been typed. This character is sent through
     * a <code>DataObjectChangeEvent</code> object to each consumer.
     *
     * @see DataObjectChangeEvent
     */
    public void fireDataObjectChange()
    {

	/* No need to propagate the NOP empty string if a WindMark name */
	if ((getText().equals("")) && (!propertyName.equals("textwidget")))
		{
		return;
		}

	Enumeration consumers = myConsumers.elements();
	DataObjectChangeEvent d = new DataObjectChangeEvent(this,propertyName,null,getDataObject());

	while ( consumers.hasMoreElements() )
          {
          try
              {
              fireDataObjectChange( ((DataObjectChangeListener)consumers.nextElement()),d);
              }
          catch (RuntimeException e)
              {
              e.printStackTrace();
              }
          }
      }

    /**
     * call the <code>dataObjectChange()</code> method of a specified listener
     *
     * @param l  listener to recieve the update
     * @param d  event that is to be recieved by the listener
     */
    public void fireDataObjectChange(DataObjectChangeListener l,DataObjectChangeEvent d)
        {
        l.dataObjectChange(d);
        }

    /**
     * Get the current value of the <code>TextField</code>. This method returns
     * the current text of the <code>TextField</code> using <code>getText()</code>.
     *
     * @returns <code>String</code>
     */
    public Object getDataObject ()
        {
        return getText();
        }

    /**
     * This routine updates the text value of the <code>TextArea</code>
     * based on the data passed in by the <code>DataObjectChangeEvent</code>
     *
     * @param data  the event invoking this method.
     */
    public void dataObjectChange (DataObjectChangeEvent data) throws RuntimeException
        {
        setText ( data.getNewValue().toString() );
        }

    // -------------------------------------------
    // implementation of interface TextListener

    /**
     * invoked by <code>TextEvent</code>s, this method
     * will call fireDataObjectChange() as long as the
     * <code>getKeyCharFire()</code> returns '\0'.
     *
     * @param t  <code>TextEvent</code> invoking this method.
     */
    public void textValueChanged ( TextEvent t )
        {
        Log.log ( "textValueChanged ("+t+") called." );

        if (keyCharFire == '\0')
        	{
			if (updateByKeyPress)
            	fireDataObjectChange();
			}
        }

    /**
     * returns the key that was passed into the constructor
     * as the character to listen for inorder to fire a
     * DataObjectChangeEvent.
     *
     * @returns  char
     */
    public char getKeyCharFire()
        {
        return keyCharFire;
        }

    /**
     * returns the Property Name, the event property to fire with new values
     * DataObjectChangeEvent.
     *
     * @returns  String propertyName
     */
    public String getPropertyName()
        {
        return propertyName;
        }

    /**
     * sets the Property Name, the event property to fire with new values
     * DataObjectChangeEvent.
     *
     * @param  propertyname
     */
    public void setPropertyName(String name)
        {
        propertyName = name;
        }

    /**
     * pass it on.  This does nothing.
     */
    public void keyPressed (KeyEvent ke)
        {
        }

    /**
     * pass it on.  This does nothing.
     */
    public void keyReleased (KeyEvent ke)
        {
        }

    /**
     * on a KeyEvent, analyze the return of getKeyCharFire. If the
     * key typed matches <i>that</i> char, fire a DataObjectChangeEvent.
     *
     * @param ke  KeyEvent invoking this method.
     */
    public void keyTyped (KeyEvent ke)
        {
        if (ke.getKeyChar() == getKeyCharFire())
            {
			if (updateByKeyPress)
	            fireDataObjectChange();
            }
        }


    /**
     * setUpdateByKeyPress: change updateByKeyPress value.
     * used when some other mechanism, like a button, is to trigger DataObjectChange
     *
     * @param value  new updateByKeyPress value.
     */
    public void setUpdateByKeyPress (boolean value)
        {
		updateByKeyPress = value;
        }


    }
