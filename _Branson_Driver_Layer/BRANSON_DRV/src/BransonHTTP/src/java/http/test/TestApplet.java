/* TestApplet.java - a test of the LiveControl Java API */

/* Copyright 1999-2007 Wind River Systems, Inc. */

/*
modification history
--------------------
02b,22mar07,ps  build base URL using getCodeBase rather than getDocumentBase.
                Defect WIND00091284
02a,20oct03,dlr  Port to WM Web
*/

/* Java imports */

import java.awt.*;
import java.awt.event.KeyListener;
import java.awt.event.FocusListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.net.*;
import java.util.Hashtable;
import java.applet.*;
import java.beans.*;

/* http imports */

import http.livecontrol.directevents.*;
import http.livecontrol.common.*;
import http.livecontrol.comm.*;

/**
 * This class extends Applet.  It connects to the Wind Web Server using
 * http://<code>getDocumentBase().getHost()</code>/lc as the <code>URL</code>.
 * This URL must have been added to the configuration as the URL to which
 * connections would be attached to the <code>LiveControl RPM</code>.
 *
 * @since Wind Web Server 2.0
 */
public class TestApplet extends java.applet.Applet
                        implements KeyListener,
                                   FocusListener
    {

    // private data objects

    /**
     * the web server URL which to connect. For LiveControl connections,
     * this URL is "http://<targetIP>/<RPM configured LiveControl path>/".
     * In this example, the <RPM configured LiveControl path> is 'lc'.
     */
    private URL              myServerURL;
    private URL              myServerBaseURL;

    /**
     * The registry that is used to proxy components through a LiveControl
     * connection. <code>DirectRegistry</code> notifies symbol changes by
     * firing property changes.
     */
    private DirectRegistry   myRegistry;

    /**
     * This object is used to await property change events from myRegistry.
     * It will fire <code>DataObjectChangeEvent</code>s as symbol values
     * change.  Any component that implements <code>DataObjectChangeListener</code>
     * and is added to <code>DirectReadProxy</code>'s list of change listener
     * will receive <code>DataObjectChangeEvent</code>s
     */
    private DirectReadProxy  myReadProxy;

    /**
     * This object is used to send updates to the <code>DirectRegistry</code>.
     * <code>DirectWriteProxy</code> is notified of updates via
     * <code>DataObjectChangeEvent</code>s.
     */
    private DirectWriteProxy myWriteProxy;

    /**
     * This object is an awt (textarea for strings) component that implements
     * <code>DataObjectChangeListener</code>. This variable will allow input.
     */
    private TextWidget       inputTW;

    /**
     * This object is an awt (textarea for strings) component that implements
     * <code>DataObjectChangeListener</code>.  This variable will not allow
     * input.
     */
    private TextWidget       outputTW;

    /**
     * This object is an awt (textarea for numbers) component that implements
     * <code>DataObjectChangeListener</code>. This variable will allow imput.
     */
    private NumberWidget     inputNW;

    /**
     * This object is an awt (textarea for numbers) component that implements
     * <code>DataObjectChangeListener</code>.  This variable will not allow
     * input.
     */
    private NumberWidget     outputNW;

    /**
     * A <code>Panel</code> that is used by the <code>KeyListener</code>
     * and <code>FocusListener</code> implementation.
     */
    private Panel            inputFieldKeyListenerPanel = null;

    private Hashtable        keyListenerPanels = new Hashtable();

    /**
     * color of all input widgets (pale blue)
     */
    private static final Color INPUTW_COLOR = new Color (135, 206, 250);

    /**
     * font for all labels
     */
    private static final Font LABEL_FONT = new Font( "serif", Font.BOLD, 12);

    /**
     * create some useful constants
     */
    private final String CLIENT_DESCRIPTION = "Client Browser";
    private final String SERVER_DESCRIPTION = "Server SymbolTable";
    private final int NUMBER_WIDGET = 0x1;
    private final int TEXT_WIDGET = 0x2;
    private final short DEFAULT_WIDGET_WIDTH = 15;

    private short widgetWidth = DEFAULT_WIDGET_WIDTH;

    /**
     * for debugging.
     */
    public static final Logging LOG = new Logging (System.out, 0, "TestApplet");

    /**
     * initializes the applet. For each symbol, it sets up either a
     * <code>TextWidget</code> or <code>NumberWidget</code> for reading
     * the server side value (via <code>DirectReadProxy</code>) and
     * a <code>TextWidget</code> or <code>NumberWidget</code> for writing
     * to the server side symbol table (http symbol table) (via
     * <code>DirectReadProxy</code>).
     *
     * @see DirectReadProxy
     * @see DirectWriteProxy
     * @see NumberWidget
     * @see TextWidget
     * @since Wind Web Server 2.0
     */
    public void init()
        {
        Label l;
        Panel p;
        Panel pSymbolTmp;
        Button b;

        super.init();
        // initialize Live Control communication with DirectEvent architecture
        try
           {
           // myServerURL = new URL ( "http://"+getDocumentBase().getHost()+"/lc/" );
           myServerBaseURL = getCodeBase();

           myServerURL = new URL ( "http://" +
                                   myServerBaseURL.getHost() +
                                   ":" + myServerBaseURL.getPort() +
                                   "/lc/" );

           LOG.log (0, "server LiveControl URL should be " + 
                       "http://" + myServerBaseURL.getHost() + ":" +
                       myServerBaseURL.getPort() + "/lc/");


           myRegistry = new DirectRegistry (myServerURL);


           /* Overall container, this applet, contains lots of other panels */
           setLayout  (new GridLayout (0, 2));
           setBackground(Color.white);

           /****************************
           Panel for symbol type - Short
           ****************************/
           /***********************************************
           GUI appearance for panel for symbol type - Short
           ***********************************************/

           pSymbolTmp = symbolPanelBorderCreate
                        (
                        Color.yellow,
                        "Symbol \"ShortTest\" is of type Short",
                        NUMBER_WIDGET
                        );
           if (pSymbolTmp != null)
               add (pSymbolTmp);


           try
               {
               widgetSetup ("ShortTest", inputNW, outputNW);
               }
           catch (Exception any)
               {
               System.out.println ("TestApplet: ShortTest not initialized!");
               }


           /****************************
           Panel for symbol type - Char
           ****************************/
           /***********************************************
           GUI appearance for panel for symbol type - Char
           ***********************************************/

           pSymbolTmp = symbolPanelBorderCreate
                        (
                        Color.white,
                        "Symbol \"CharTest\" is of type Char",
                        NUMBER_WIDGET
                        );
           if (pSymbolTmp != null)
               add (pSymbolTmp);

           try
               {
               widgetSetup ("CharTest", inputNW, outputNW);
               }
           catch (Exception any)
               {
               System.out.println ("TestApplet: CharTest not initialized!");
               }

           /****************************
           Panel for symbol type - Bool
           ****************************/
           /***********************************************
           GUI appearance for panel for symbol type - Bool
           ***********************************************/

           pSymbolTmp = symbolPanelBorderCreate
                        (
                        Color.white,
                        "Symbol \"BoolTest\" is of type Bool",
                        NUMBER_WIDGET
                        );
           if (pSymbolTmp != null)
               add (pSymbolTmp);

           try
               {
               widgetSetup ("BoolTest", inputNW, outputNW);
               }
           catch (Exception any)
               {
               System.out.println ("TestApplet: BoolTest not initialized!");
               }

           /****************************
           Panel for symbol type - Float
           ****************************/
           /***********************************************
           GUI appearance for panel for symbol type - Float
           ***********************************************/

           pSymbolTmp = symbolPanelBorderCreate
                        (
                        Color.yellow,
                        "Symbol \"FloatTest\" is of type Float (w/ No Polling)",
                        TEXT_WIDGET
                        );
           if (pSymbolTmp != null)
               add (pSymbolTmp);
           try
               {
               widgetSetup ("FloatTest", inputTW, outputTW, DirectReadProxy.PRIORITY_NONE);
               }
           catch (Exception any)
               {
               System.out.println ("TestApplet: FloatTest not initialized!");
               }

           /****************************
           Panel for symbol type - Long
           ****************************/
           /***********************************************
           GUI appearance for panel for symbol type - Long
           ***********************************************/

           pSymbolTmp  = symbolPanelBorderCreate
                         (
                         Color.yellow,
                         "Symbol \"LongTest\" is of type Long",
                         NUMBER_WIDGET
                         );
           if (pSymbolTmp != null)
               add (pSymbolTmp);
           try
               {
               widgetSetup("LongTest", inputNW, outputNW);
               }
           catch (Exception any)
               {
               System.out.println ("TestApplet: LongTest not initialized!");
               }

           /*****************************
           Panel for symbol type - string
           *****************************/
           /***********************************************
           GUI appearance for panel for symbol type - String
           ***********************************************/

           //
           // grid layout needed for this panel, so
           // we cannot use symbolPanelBorderCreate()
           //

           pSymbolTmp = new Panel();
           pSymbolTmp.setLayout (new GridLayout(3, 1));
           pSymbolTmp.setBackground(Color.white);
           pSymbolTmp.setForeground(Color.black);

           // Label "Symbol xxx is of Type xxx" - North)
           p = new Panel();
           l = new Label("Symbol \"szString\" is of type String (w/ High Polling)");
           l.setFont(new Font( "serif", Font.BOLD, 12));
           p.add(l);
           pSymbolTmp.add (p);

           // Client INPUT text widget layout stuff
           p = new Panel();
           p.setLayout (new GridLayout(2,1));
           p.add(new Label("Client Browser"));
           inputTW = new TextWidget(40);
           inputTW.setBackground(INPUTW_COLOR);
           inputTW.setForeground(Color.black);
           p.add(inputTW);
           pSymbolTmp.add(p);

           // Server details
           p = new Panel();
           p.setLayout (new GridLayout(2,1));
           p.add(new Label("Server Symbol Table"));
           outputTW = new TextWidget(40);
           outputTW.setBackground(Color.white);
           outputTW.setForeground(Color.black);
           outputTW.setEditable(false);
           p.add(outputTW);
           pSymbolTmp.add(p);

           add(pSymbolTmp);

           try
                {
                widgetSetup("szString", inputTW, outputTW, DirectReadProxy.PRIORITY_HIGH);
                }
           catch (Exception any)
               {
               System.out.println ("TestApplet: szString not initialized!");
               }


           /*****************************
           Panel for GET symbol type - string
           *****************************/
           /***********************************************
           GUI appearance for panel for symbol type - String
           ***********************************************/

           //
           // grid layout needed for this panel, so
           // we cannot use symbolPanelBorderCreate()
           //

           pSymbolTmp = new Panel();
           pSymbolTmp.setLayout (new GridLayout(4, 1));
           pSymbolTmp.setBackground(Color.white);
           pSymbolTmp.setForeground(Color.black);

           // Label "Symbol xxx is of Type xxx" - North)
           p = new Panel();
           l = new Label("WindMark Value using 'Get'");
           l.setFont(new Font( "serif", Font.BOLD, 12));
           p.add(l);
           pSymbolTmp.add (p);

           // Client INPUT text widget layout stuff
           p = new Panel();
           p.setLayout (new BorderLayout() );
           p.add("West", new Label("WindMark:"));
           TextWidget nameGetTW = new TextWidget(20);
           nameGetTW.setUpdateByKeyPress(false);	/* wait for button push */
           nameGetTW.setBackground(INPUTW_COLOR);
           nameGetTW.setForeground(Color.black);
           p.add("East", nameGetTW);
           pSymbolTmp.add(p);

           // Server details
           p = new Panel();
           p.setLayout (new BorderLayout() );
           p.add("West", new Label("Value:"));
           outputTW = new TextWidget(20);
           outputTW.setBackground(Color.white);
           outputTW.setForeground(Color.black);
           outputTW.setEditable(false);
           p.add("East", outputTW);
           pSymbolTmp.add(p);

           // Get Button
           b = new Button("Get Value");
           pSymbolTmp.add(b);
           ButtonListener bl_g = new ButtonListener(nameGetTW);
           b.addActionListener(bl_g);

           add(pSymbolTmp);

           try
                {
                widgetWindMarkSetup("sRadioIndex",DirectWindMarkProxy.GET_REQUEST, null, outputTW, nameGetTW);
                }
           catch (Exception any)
               {
               System.out.println ("TestApplet: szString not initialized!");
               }

           /*****************************
           Panel for GetNext type - string
           *****************************/
           /***********************************************
           GUI appearance for panel for symbol type - String
           ***********************************************/

           //
           // grid layout needed for this panel, so
           // we cannot use symbolPanelBorderCreate()
           //

           pSymbolTmp = new Panel();
           pSymbolTmp.setLayout (new GridLayout(4, 1));
           pSymbolTmp.setBackground(Color.yellow);
           pSymbolTmp.setForeground(Color.black);

           // Label "Symbol xxx is of Type xxx" - North)
           p = new Panel();
           l = new Label("WindMark Value using 'GetNext'");
           l.setFont(new Font( "serif", Font.BOLD, 12));
           p.add(l);
           pSymbolTmp.add (p);

          // Client INPUT text widget layout stuff
           p = new Panel();
           p.setLayout (new BorderLayout() );
           p.add("West", new Label("WindMark:"));
           nameGetTW = new TextWidget(20);
           nameGetTW.setUpdateByKeyPress(false);	/* wait for button push */
           nameGetTW.setBackground(INPUTW_COLOR);
           nameGetTW.setForeground(Color.black);
           p.add("East", nameGetTW);
           pSymbolTmp.add(p);

          // Server details
           p = new Panel();
           p.setLayout (new BorderLayout() );
           p.add("West", new Label("Value:"));
           outputTW = new TextWidget(20);
           outputTW.setBackground(Color.white);
           outputTW.setForeground(Color.black);
           outputTW.setEditable(false);
           p.add("East", outputTW);
           pSymbolTmp.add(p);

             // Get Button
           b = new Button("Get Next Value");
           pSymbolTmp.add(b);
           ButtonListener bl_gn = new ButtonListener(nameGetTW);
           b.addActionListener(bl_gn);

           add(pSymbolTmp);

           try
                {
                widgetWindMarkSetup("number",DirectWindMarkProxy.GETNEXT_REQUEST, null, outputTW, nameGetTW);
                }
           catch (Exception any)
               {
               System.out.println ("TestApplet: szString not initialized!");
               }


           /*****************************
           End of TestApplet Init
           *****************************/

            }
        catch (Exception e)
            {
            e.printStackTrace();
            }

        } // end of init()



    public void destroy()
        {
		myRegistry.stop();
  	    }


    /**
     * creates a symbol panel. It uses <code>BorderLayout</code> to layout the
     * components.  There are 3 panels and an image:
     *
     * <pre>
     *                          [Label panel]
     *
     *                          [image panel]
     *
     *     [client input widget panel]    [server output widget panel]
     * </pre>
     *
     * The client input widget fires <code>DataObjectChangeEvents</code>
     * and the server output widget receives <code>DataObjectChangeEvents</code>.
     * The output widget monitors the value of the symbol on the server
     * while the input widget changes the value. <br>
     * The widgets are either a <code>NumberWidget</code> or <code>TextWidget</code>
     * with input and output being the same type, but input widgets are editable
     * and output widgets are not.
     *
     * @param  bgColor  The background of the <code>Panel</code> that is created.
     * @param  title    The <code>String</code> to use for the heading of the panel.
     * @param  inputOutputType  Either <code>NUMBER_WIDGET</code> or <code>TEXT_WIDGET</code>,
     *                          it determines what kind of widgets will be created.
     * @returns <code>Panel</code> objects.
     * @since Wind Web Server 2.0
     * @see DataObjectChangeEvent
     */
    protected Panel symbolPanelBorderCreate
        (
        Color bgColor,
        String title,
        int inputOutputType
        )
        {
        Panel pSymbol;
        Panel p, pImage;
        Label l;

        // argument sanity check

        if ( (bgColor == null) || (title == null) || (title.length() == 0) ||
             !((inputOutputType == NUMBER_WIDGET) ||
              (inputOutputType == TEXT_WIDGET)) )
            {
            LOG.log("INVALID arguments to symbolPanelCreate");
            return null;
            }

        // create a new panel with BorderLayout

        pSymbol = new Panel();
        pSymbol.setLayout (new BorderLayout ());
        pSymbol.setBackground(bgColor);  // background is set from arguments
        pSymbol.setForeground(Color.black);

        // Label "Symbol xxx is of type xxx" - North)
        p = new Panel();
        l = new Label(title);
        l.setFont(LABEL_FONT);
        p.add(l);

        pSymbol.add (p, "North");

        // Client INPUT text widget layout stuff

        p = new Panel();
        p.setLayout (new BorderLayout ());
        p.add(new Label(CLIENT_DESCRIPTION), "North");

        // set up arrows image

        pImage = setupImage();


        // are created NumberWidgets or TextWidgets?

        if (inputOutputType == NUMBER_WIDGET)
            {

            // you can set this before calling this method

            inputNW = new NumberWidget(widgetWidth);
            inputNW.setEditable(true);
            inputNW.setBackground(INPUTW_COLOR);
            inputNW.setForeground(Color.black);
            inputNW.addKeyListener(this);
            inputNW.addFocusListener(this);
            keyListenerPanels.put(inputNW, pImage);
            p.add(inputNW, "South");

            }
        else
            {
            inputTW = new TextWidget(widgetWidth);
            inputTW.setEditable(true);
            inputTW.setBackground(INPUTW_COLOR);
            inputTW.setForeground(Color.black);
            inputTW.addKeyListener(this);
            inputTW.addFocusListener(this);
            keyListenerPanels.put(inputTW, pImage);

            p.add(inputTW, "South");

            }

        pSymbol.add (p, "West");

        // Server layout stuff
        p = new Panel(new BorderLayout());
        p.add(new Label(SERVER_DESCRIPTION), "North");

        if (inputOutputType == NUMBER_WIDGET)
            {
            outputNW = new NumberWidget(widgetWidth);
            outputNW.setBackground(Color.white);
            outputNW.setForeground(Color.black);
            outputNW.setEditable(false);

            p.add(outputNW, "South");
            }
        else
            {
            outputTW = new TextWidget(widgetWidth);
            outputTW.setBackground(Color.white);
            outputTW.setForeground(Color.black);
            outputTW.setEditable(false);
            p.add(outputTW, "South");
            }

        pSymbol.add (p, "East");


        pSymbol.add (pImage, "Center");
        pSymbol.add (new Label(" "), "South");
        return pSymbol;
        }

    /**
     * This routine sets up <code>DataObject[changeType]Listener</code> objects
     * to listen for events from DirectReadProxy and DirectWriteProxy.
     *
     * @param  name      <code> String </code> containin the name of the symbol.
     * @param  inWidget  must implement <code>DataObjectChangeListener</code>
     *                 and <code>DataObjectStatusListener</code>.
     * @param  outWidget  must implement <code>DataObjectChangeListener</code>
     *                 and <code>DataObjectStatusListener</code>.
     */
    protected void widgetSetup (String name, Component inWidget, Component outWidget, String pollingPriority)
                   throws Exception
        {

        // sanity check

        if ((name == null || name.length() == 0)             ||
            (inWidget == null) || (outWidget == null)        ||
            !(inWidget instanceof DataObjectChangeListener)  ||
            !(inWidget instanceof DataObjectStatusListener)  ||
            !(inWidget instanceof DataObjectChangeSupport)   ||
            !(outWidget instanceof DataObjectChangeListener) ||
            !(outWidget instanceof DataObjectStatusListener))
            return;

        // DirectWriteProxy setup

        myWriteProxy = new DirectWriteProxy(name);
        myWriteProxy.addDataObjectStatusListener ((DataObjectStatusListener)inWidget);
        myRegistry.addPropertyChangeListener ((PropertyChangeListener)myWriteProxy);
        ((DataObjectChangeSupport)inWidget).addDataObjectChangeListener ((DataObjectChangeListener)myWriteProxy);

        // DirectReadProxy setup

        myReadProxy = new DirectReadProxy (name, pollingPriority);
        myReadProxy.addDataObjectChangeListener ((DataObjectChangeListener)outWidget);
        myReadProxy.addDataObjectStatusListener ((DataObjectStatusListener)outWidget);
        myRegistry.addPropertyChangeListener ((PropertyChangeListener)myReadProxy);
        }

    protected void widgetSetup (String name, Component inWidget, Component outWidget)
                   throws Exception
        {
		widgetSetup (name, inWidget, outWidget, DirectReadProxy.PRIORITY_MED);
		}


    /**
     * This routine sets up <code>DataObject[changeType]Listener</code> objects
     * to listen for events from DirectReadProxy and DirectWriteProxy.
     *
     * @param  cmnd is
     * @param  inWidget  must implement <code>DataObjectChangeListener</code>
     *                 and <code>DataObjectStatusListener</code>.
     * @param  outWidget  must implement <code>DataObjectChangeListener</code>
     *                 and <code>DataObjectStatusListener</code>.
     * @param  nameWidget  must implement <code>DataObjectChangeListener</code>
     *                 and <code>DataObjectStatusListener</code>.
     */
    protected void widgetWindMarkSetup (String name, int cmnd, Component inWidget, Component outWidget, Component nameWidget)
                   throws Exception
        {

        // sanity check

        if ((  inWidget   != null                               ) &&
            (!(inWidget   instanceof DataObjectChangeListener) ||
             !(inWidget   instanceof DataObjectStatusListener)  ) )
            return;

        if ((  outWidget  != null                               ) &&
            (!(outWidget  instanceof DataObjectChangeListener) ||
             !(outWidget  instanceof DataObjectStatusListener)  ) )
            return;

        if ((  nameWidget != null                               ) &&
            (!(nameWidget instanceof DataObjectChangeListener) ||
             !(nameWidget instanceof DataObjectStatusListener)  ) )
            return;


		DirectWindMarkProxy myWindMarkProxy = new DirectWindMarkProxy (name, cmnd);

        // Setup Value Input Widget
		if (null != inWidget)
			{
			// fire changes to the value intput Widget
			myWindMarkProxy.addDataObjectStatusListener ((DataObjectStatusListener)inWidget);

			/* Add the Write Proxy */
			myRegistry.addPropertyChangeListener ((PropertyChangeListener)myWindMarkProxy);

			/* Add lister to new values from inWidget */
			((DataObjectChangeSupport)inWidget).addDataObjectChangeListener ((DataObjectChangeListener)myWindMarkProxy);

			}

        // Setup Value Output Widget
		if (null != outWidget)
			{
			// fire changes to the value output Widget
			myWindMarkProxy.addDataObjectChangeListener ((DataObjectChangeListener)outWidget);
			myWindMarkProxy.addDataObjectStatusListener ((DataObjectStatusListener)outWidget);
			}

        // Setup Name Input Widget
		if (null != nameWidget)
			{
			// fire changes to the value name   Widget
//			myWindMarkProxy.addDataObjectChangeListener ((DataObjectChangeListener)nameWidget);
			myWindMarkProxy.addDataObjectStatusListener ((DataObjectStatusListener)nameWidget);

			/* provide WM names not values */
			((TextWidget) nameWidget).setPropertyName(".Name");

			/* Waiting is only for User */
			nameWidget.setEnabled(true);
			/* Preset provided name for WindMark [! BEFORE ADDING LISTENER !] */
			((TextWidget) nameWidget).setText(name);

			/* Add lister to new names from name Widget */
			((DataObjectChangeSupport)nameWidget).addDataObjectChangeListener ((DataObjectChangeListener)myWindMarkProxy);
			}

		/* Add the WindMark Proxy */
		myRegistry.addPropertyChangeListener ((PropertyChangeListener)myWindMarkProxy);

        }

    // Implementation of KeyListener

    /**
     * Called when a key is released.  This event is ignored.
     */
    public void keyReleased(java.awt.event.KeyEvent e)
        {
        // pass it on
        }

    /**
     * called when a key is pressed. This event is ignored.
     */
    public void keyPressed(java.awt.event.KeyEvent e)
        {
        // pass it on
        }

    /**
     * called when a key is typed. It updates the image to the
     * red arrows image.
     *
     * @param  e   <code>KeyEvent</code> object that led to the
     *             invocation of this method.
     * @see KeyEvent
     * @see KeyListener
     * @since Wind Web Server 2.0
     */
    public void keyTyped(java.awt.event.KeyEvent e)
        {
        // replace existing image with animated one
        Image image;

        inputFieldKeyListenerPanel = (Panel)keyListenerPanels.get(e.getComponent());
        image = getImage(getDocumentBase(), "images/RedArrows.gif");
        Component mvgImg = createMovingImage (image);
        inputFieldKeyListenerPanel.removeAll();
        inputFieldKeyListenerPanel.add(mvgImg);
        inputFieldKeyListenerPanel.invalidate();
        inputFieldKeyListenerPanel.validate();
        }

    // Implementation of FocusListener

    /**
     * called when the focus of the component is regained. This
     * event is ignored.
     */
    public void focusGained (java.awt.event.FocusEvent e)
        {
        // pass it on
        }

    /**
     * Called when the focus of the component is lost.  It replaces
     * the image with the original black arrows image.
     *
     * @param  e  <code>FocusEvent</code> that spawned the invocation
     *            of this method.
     * @since Wind Web Server 2.0
     * @see  FocusEvent
     */
    public void focusLost (java.awt.event.FocusEvent e)
         {
         // replace existing image with original one
         Image image;

         inputFieldKeyListenerPanel = (Panel)keyListenerPanels.get(e.getComponent());
         image = getImage(getDocumentBase(), "images/BlackArrows.gif");
         inputFieldKeyListenerPanel.removeAll();
         Component mvgImg = createMovingImage (image);
         inputFieldKeyListenerPanel.add(mvgImg);
         inputFieldKeyListenerPanel.invalidate();
         inputFieldKeyListenerPanel.validate();
         }

    /**
     * This routine loads the black arrows image and adds it to a <code>Panel</code>
     * which is returned.
     *
     * @returns <code>Panel</code> containing the black arrows image.
     */
    public Panel setupImage ()
        {
        Image image = null;
        Panel p = new Panel();

        try
            {
            image = getImage(getDocumentBase(), "images/BlackArrows.gif");
            }
        catch (Exception e)
            {
            LOG.log("getImage returns exception: " + e);
            }

        if (image != null)
            {
            Component mvgImg = createMovingImage(image);
            p.add(mvgImg);
            }
        else
            LOG.log("image is null");
        return (p);
        }

    /**
     * takes an <code>Image</code> object
     * and creates a component holder for it.
     *
     * @param image <code>Image</code> object to house.
     */
    public Component createMovingImage (Image image)
        {
        return new MovingImage(image);
        }

    }
