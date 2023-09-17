/* MovingImage.java - a class that contains an image that can be updated */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

import java.awt.*;

/**
 * This sub class is a component holder for images.
 * It allows for images to be housed in a component
 * for redrawing and to receive events.  Called
 * <code>MovingImage</code> because the image color changes
 * from red to black and visa versa.
 *
 * @since Wind Web Server 2.0
 */
public class MovingImage extends Component
    {
    private Image image;
    /**
     * constructor.  It takes an <code>Image</code> object
     * and sets the size.
     *
     * @param image <code>Image</code> object to house.
     */
    MovingImage (Image image)
        {
        this.image = image;
        setSize (60, 40);
        }
    /**
     * paint the image.  Overrides <code>paint(Graphics)</code>
     * in <code>Component</code>.
     *
     * @param g  <code>Graphics</code> object to use for painting.
     */
    public void paint (Graphics g)
        {
        g.setColor(Color.white);
        int width = getSize().width, height = getSize().height;

        g.drawImage (image, 0, 0, 60, 40, this);
        }
    /**
     * returns the size as a <code>Dimension</code>
     *
     * @returns <code>getSize()</code> as a <code>Dimension </code>.
     *
     * @see Component#getSize()
     */
    public Dimension getPreferredSize()
        {
        return getSize();
        }
    }
