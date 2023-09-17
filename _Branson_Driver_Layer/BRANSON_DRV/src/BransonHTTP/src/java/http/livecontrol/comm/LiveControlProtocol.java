/* LiveControlProtocol.java - manage one connection to the embedded server */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,02jan04,dlr  Port to WM Web
01b,23jan00,csr   fixing issue with socket that will block even though
                  a link is down.
01a,27oct99,csr   created. Removed one debug statement. Added comments.
*/

package http.livecontrol.comm;

/* java imports */

import java.lang.*;
import java.net.*;
import java.io.*;
import java.util.*;

/* http imports */

import http.livecontrol.comm.*;

/**
 * The Live Control protocol works with several persistent connections simultaneous.
 * One distinct connection is represented by an instance of this class.
 * Note that this class is intended for internal use by class LiveControlConnection and therefore
 * does not implement each method inherited by HttpURLConnection propperly.
 * Also it must be said that the proxy-related methods should not be used.
 * <code>LiveConnect</code> does not currently support SSL.
 *
 * @see http.livecontrol.comm.LiveControlConnection
 * @since Wind Web Server 2.0
 */
public class LiveControlProtocol extends HttpURLConnection
    {

    /* local variables */

    /**
     * for debuggin'
     */
    private Logging Log = new Logging (System.out, 5, "LiveControlProtocol: ");

    /**
     * name of the connection
     */
    private String connectname;

    /**
     * port to connect to.
     */
    private int    connectport;

    /**
     * sockect connection
     */
    protected Socket socket;

    /**
     * the io stream through which packets are sent. Obtained from
     * the socket's OutputStream.
     */
    protected OutputStream outstream;

    /**
     * the input stream from which packets are received.
     */
    protected InputStream instream;

    private String response;
    private String cookie;
    private String sendcookie;

    /**
     * status of this connection. If destroyed, no more connections
     * can ocurr.
     */
    private boolean destroyed = false;

    /**
     * Create a new connection
     *
     * @param requesturl destination of the connection.
     * @since Wind Web Server 2.0
     */
    public LiveControlProtocol (URL requesturl)
        {
        super(requesturl);
        init();
        setProxy(requesturl.getHost(),requesturl.getPort());
        }

    /**
     * create a new connection giving the proxyname, the request URL and
     * the port number.
     *
     * @param requesturl  <code>URL</code> to connect to.
     * @param proxyname   <code>String</code> string containing the name of
     *                    this proxy.
     * @param proxyport   port number through which to connect.
     */
    public LiveControlProtocol (URL requesturl,String proxyname,int proxyport)
        {
        super(requesturl);
        init();
        setProxy(proxyname,proxyport);
        }

    /**
     * disconnects this connection.
     */
    public void finalize()  throws Throwable
        {
        disconnect();
        }

    /**
     * assigns <code>method</code> to "POST"
     */
    private void init()
        {
        method = "POST";
        }

    /**
     * sets the proxy name and port.
     *
     * @param  name new name for the proxy.
     * @port   port the new port to connect to.
     */
    public synchronized void setProxy (String name,int port)
        {
        connectname = name;
        connectport = ( port <= 0 ) ? 80 : port;
        }

    /**
     * returns the current state of whether this client
     * is using a proxy.  (always returns false)
     *
     * @since Wind Web Server 2.0
     */
    public boolean usingProxy()
        {
        return false;
        }

    /**
     * Destroy the connection.
     * Destroyed connections can not reconnect again.
     *
     * @see http.livecontrol.comm.LiveControlProtocol#connect
     * @since Wind Web Server 2.0
     */
    public synchronized void destroy()
        {
        destroyed = true;
        disconnect();
        }

    /**
     * Establish connection to the server. Creates a new socket, assigns
     * the output and input I/O streams. Upon success, it sets the connected
     * status to true.
     *
     * @exception IOException if attempt to connect failed.
     * @since Wind Web Server 2.0
     */
    public synchronized void connect() throws IOException
        {
        if ( destroyed )
            throw new IOException ( "THIS CONNECTION HAS BEEN USED AFTER BEING DESTROYED!" );
        try
            {
            socket = new Socket(connectname, connectport);
            outstream = socket.getOutputStream();
            instream  = socket.getInputStream();
            socket.setSoTimeout (0);
            doInput = true;
            doOutput = true;
            }
        catch (Exception e)
            {
            disconnect();
            throw new IOException(e.toString());
            }
        connected = true;
        }

    /**
     * Close down connection.
     * The connection may be opened again if neccessary.
     *
     * @see http.livecontrol.comm.LiveControlProtocol#connect
     * @see http.livecontrol.comm.LiveControlProtocol#destroy
     * @since Wind Web Server 2.0
     */
    public synchronized void disconnect()
        {
        connected = false;
        outstream = null;
        instream = null;
        try
            {
            socket.close();
            }
        catch (Exception xx)
            {
            }
        socket = null;
        }

    /**
     * returns I/O input stream used for socket reads.
     */
    public synchronized InputStream getInputStream() throws IOException
        {
        return instream;
        }

    /**
     * returns I/O output stream used for socket writes.
     */
    public synchronized OutputStream getOutputStream() throws IOException
        {
        return outstream;
        }

      /**
       * Get current connection status.
       * @return true if connection is open, false otherwise.
       */
    public synchronized boolean isConnected() {
      return connected;
    }

      /**
       * Send the specified HTTP request and read the HTTP header of the answer.
       * @param content message body of the request.
       * @exception IOException if network connection problems occured during operation.
       */
    public synchronized void sendSynchronizedRequest(byte[] content) throws IOException {
      try {
        sendRequest(content);
        readHeader();
        Log.log ( 25,"sendSynchronizedRequest: responseCode="+responseCode);

        // test if request succeeded
        if ( responseCode > 299 )
    throw new IOException("Server denied service (server error)!");
      } catch (Exception e) {
        Log.log ( 5,"sendSynchronizedRequest: failed: "+e);
        disconnect();
        throw new IOException(e.toString());
      }
    }

      /**
       * Send the specified HTTP request but do not read the HTTP header of the answer.
       * @param content message body of the request.
       * @exception IOException if network connection problems occured during operation.
       */
    public synchronized void sendRequest(byte[] content) throws IOException {
      int    contentlength;
      String req;

      if ( content == null ) {
        contentlength = 0;
      } else {
        contentlength = content.length;
      }

      if (!connected) {
        connect();
      }


      req = method+" "+url.toString()+" HTTP/1.1\r\nhost: "+url.getHost()+"\r\nconnection: keepalive\r\npersist:\r\nkeep-alive:\r\ncontent-length: "+contentlength+"\r\nCookie: "+sendcookie+"\r\n\r\n";
      Log.log ( 20,"sendRequest()  "+req );
      outstream.write ( req.getBytes() );

      {
          int i;
          String output = "sendRequest: ***content[]=";

          for (i=0; i<contentlength; i++) {
              output += "[" + Integer.toHexString( (int)content[i] ) + "]";
          }
          Log.log ( 25,output);
      }


      if ( contentlength > 0 ) {
        outstream.write( content );
      }
      outstream.flush();
    }

      /**
       * Set the HTTP-cookie that should be used.
       * @param newcookie cookie string, see RFC2068 for details.
       */
    public void setCookie(String newcookie) {
      sendcookie = newcookie;
    }


    /**
     * Get the currently used HTTP-cookie.
     * @return the currently used cookie string, see RFC2068 for details.
     */
    public String getCookie()
        {
        return cookie;
        }

      /**
       * Read the HTTP response and any MIME header up to message body.
       * @exception IOException if network connection problems occured during operation.
       */
    public synchronized void readHeader() throws IOException {
      String line;
      int    pos;

      try {
        response = readLine();
        responseCode = Integer.parseInt(response.substring(9,12));
        responseMessage = response.substring(12);

        do {
    line = readLine();
    pos = line.indexOf(": ");
    if ( pos > 0 ) {
      // setRequestProperty ( line.substring(0,pos).toLowerCase() , line.substring(pos+1));

      if ( line.startsWith ("Set-Cookie: ") )
        {
          cookie = line.substring(12,line.indexOf(";"));
          Log.log ( 25,"readHeader: Cookie found: "+cookie );
        }
    }
        } while ((line != null) && (line.length() > 0 ));

      } catch (IOException e) {
        Log.log(5,"readHeader: Got IOException "+e);
        disconnect();
        throw e;
      } catch (IndexOutOfBoundsException e) {
        Log.log ( 0,"readHeader: Malfunction! response="+response+", Code="+responseCode+", Message="+responseMessage+" - Exception: "+e );
        disconnect();
        throw new IOException("invalid HTTP header!");
      } catch (Exception e) {
        disconnect();
        throw new IOException("readHeader() FAILURE! "+e);
      }
    }

    private String readLine() throws IOException {
      byte b[] = new byte[1];
      StringBuffer s = new StringBuffer();

      try {
        while ( instream.read(b) > 0 ) {
    // System.out.print ( "-"+b[0]);
    if ( b[0] == '\r' ) continue;
    if ( b[0] == '\n' ) {
      Log.log (25, "readLine: Got "+s.toString() );
      return s.toString();
    }
    s.append(new String(b));
        }
      } catch (Exception e) {
        Log.log ( 0,"readLine: ("+s+")failed: "+e);
        throw new IOException(e.toString());
      }
      throw new IOException("readline: InputStream disconnected");
    }

    }
