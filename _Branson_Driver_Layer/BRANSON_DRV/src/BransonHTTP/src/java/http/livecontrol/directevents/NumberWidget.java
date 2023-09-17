/* NumberWidget.java - Number widget */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.directevents;

import java.lang.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

import http.livecontrol.common.*;
import http.livecontrol.comm.*;

/**
 * Example awt widget that can require/display number values.
 * This class is unsupported!
 *
 * The class implements DataObjectChangeListener and DataObjectChangeSupport.
 * In doing so, it can receive LiveControl updates and send them. By extending
 * TextField, one can enable or disable write access to the widget thus providing
 * either a widget that can either update symbol values or just read them.
 */
public class NumberWidget
  extends TextField
  implements TextListener,DataObjectChangeListener,DataObjectChangeSupport,DataObjectStatusListener,KeyListener
    {

    // local variables

    /**
     * for debugging.
     */
    public static final Logging Log = new Logging (System.out, 0, "NumberWidget: ");

    /**
     * The list of objects that will listen for DataObjectChangeEvent firings.
     */
    private Vector        myConsumers = new Vector();

    /**
     * the character to listen to inorder to fire a DataObjectChangeEvent
     */
    private char keyCharFire;

    /**
     * default constructor.  The <code>TextField</code> will contain no data,
     * and DataObjectChangeEvents will be fired on every key
     */
    public NumberWidget()
        {
        super( );
        keyCharFire = '\0';
        init();
        }

    /**
     * constructor.  This sets the text of the <code>TextField</code>
     * Otherwise, it is the same as the default constructor.
     *
     * @param text  The initial value of the <code>TextField</code>.
     */
    public NumberWidget ( String text )
        {
        super ( text );
        keyCharFire = '\0';
        init();
        }

    /**
     * constructor.  Initializes the <code>TextField</code> with a size.
     * Otherwise, it is the same as the default constructor.
     *
     * @param size  The initial size of the <code>TextField</code>.
     */
    public NumberWidget ( int size )
        {
        super ( size );
        keyCharFire = '\0';
        init();
        }

    /**
     * constructor. Initializes the <code>TextField</code> with data and
     * a size. Otherwise, it is the same as the default constructor.
     * @param text  The initial value of the TextField.
     * @param size  The initial size of the TextField.
     */
    public NumberWidget ( String text , int size )
        {
        super ( text ,size );
        keyCharFire = '\0';
        init();
        }

    /**
     * Initialize NumberWidget with a character to listen for.
     * When this key is hit, <code>fireDataObjectChange()</code>
     * is called.
     *
     * @param text  The initial value of the TextField.
     * @param size  The initial size of the TextField.
     * @param keyChar  When the user hits this key value, the
     *                 DataObjectChangeEvent is fired.
     *
     */
    public NumberWidget ( String text, int size, char keyChar)
        {
        this (text, size);
        keyCharFire = keyChar;
        }


    /**
     * called by the constructors.  It adds text and key listeners
     * to this object.
     */
    public void init ()
        {
        addTextListener ( this );
        addKeyListener (this);
        }

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
        setEnabled(((Boolean)x.getNewValue()).booleanValue());
        }

    // -------------------------------------------
    // implementation of interface DataObjectChangeListener

    /**
     * This routine updates the text value of the <code>TextArea</code>
     * based on the data passed in by the <code>DataObjectChangeEvent</code>
     *
     * @param data  the event invoking this method.
     */
    public void dataObjectChange ( DataObjectChangeEvent data ) throws RuntimeException
        {
        try
          {
          this.setText ( data.getNewValue().toString() );
          Log.log (10, "dataObjectChange ("+data.getNewValue().toString()+") called." );
          }
        catch ( Exception e )
          {
	        throw new RuntimeException("Not a number!");
          }
        doLayout();
        }

    // -------------------------------------------
    // implementation of interface DataObjectChangeSupport

    /**
     * adds a listener to the list of consumers.
     *
     * @param consumer  A <code>DataObjectChangeListener</code> that will
     *                  updates its internal data objects when <code>NumberWidget</code>
     *                  fires a <code>DataObjectChangeEvent</code>.
     */
    public void addDataObjectChangeListener ( DataObjectChangeListener consumer )
        {
        Log.log (10, "addDataObjectChangeListener() called." );
        myConsumers.addElement ( consumer );
        }

    /**
     * removes a listener from the list of consumers.
     *
     * @param consumer  listener to remove.
     * @see DataObjectChangeListener
     * @see NumberWidget#addDataObjectChangeListener
     */
    public void removeDataObjectChangeListener ( DataObjectChangeListener consumer )
        {
        Log.log ( 10,"removeDataObjectChangeListener() called." );
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
        Enumeration consumers = myConsumers.elements();
        DataObjectChangeEvent d = new DataObjectChangeEvent(this,"textwidget",null,getDataObject());

        Log.log(10, "About to fire data object changes with "+myConsumers.size()+" elements");
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
        Log.log(10, "firing data object change with: " + getDataObject());
        l.dataObjectChange(d);
        }

    /**
     * returns the value of the <code>TextField</code> as a <code>Double</code>.
     *
     * @returns   Double
     */
    public Object getDataObject ()
        {
        Double i;
        try
            {
	          i = new Double(getText());
            }
        catch (NumberFormatException e)
            {
	          return new Double(0);
            }
        return i;
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
        Log.log (10, "textValueChanged ("+t+") called." );
        if (keyCharFire == '\0')
            fireDataObjectChange();
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

    //--------------------------------------------
    // implementation of interface KeyListener

    /**
     * pass it on.  This does nothing.
     */
    public void keyPressed (KeyEvent ke)
        {
        }

    /**
     * pass it on. This also does nothing
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
            fireDataObjectChange();
            }
        }

    }
