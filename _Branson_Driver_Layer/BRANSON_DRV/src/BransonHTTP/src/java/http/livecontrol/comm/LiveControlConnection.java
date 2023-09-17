/* LiveControlConnection.java - manage the connections to the embedded server */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02b,11feb04,dlr  Support for GET, GETNEXT, Instance
02a,02jan04,dlr  Port to WM Web
01c,11may99,mb   bugfix: reconnect works after connection has been lost
01b,14jan97,ck   reworked for LiveConnect interface
01a,03dec96,ck   initial creation
*/


package http.livecontrol.comm;


import java.lang.*;
import java.net.*;
import java.io.*;
import java.util.*;
import java.beans.PropertyChangeSupport;
import java.beans.PropertyChangeListener;

import http.livecontrol.comm.*;


/**
  * LiveControlConnection holds the client side of the target communication socket and
  * provides the client side Live Control protocol suite.
  */
public class LiveControlConnection implements LiveControlCommunication {

   // instance vars
   private   LiveControlProtocol rConnection = null;
   private   LiveControlProtocol wConnection = null;
   private   SymbolTable       symTab = null;
   private   Logging           Log = new Logging (System.out, 5, "LiveControlConnection - writer");
   private   LiveConnectionReader readThread = null;
   private   PropertyChangeSupport listeners = new PropertyChangeSupport(this);
   private   URL               lcURL = null;
   private   URL               lcProxy = null;



   // packet separator byte
   protected final byte separators[] = { 1 };


   // Live Control packet identifiers
   final static byte HTTP_LC_PACKET_PING         = 1;
   final static byte HTTP_LC_PACKET_QUIT         = 2;
   final static byte HTTP_LC_PACKET_VERSION      = 3;
   final static byte HTTP_LC_PACKET_SUBSCRIBE    = 4;
   final static byte HTTP_LC_PACKET_UNSUBSCRIBE  = 5;
   final static byte HTTP_LC_PACKET_VALUE        = 6;
   final static byte HTTP_LC_PACKET_INTERVAL     = 7;
   final static byte HTTP_LC_PACKET_ERROR        = 8;
   final static byte HTTP_LC_PACKET_DESCRIPTION  = 9;
   final static byte HTTP_LC_PACKET_SYMBOLTABLE  = 10;
   public final static byte HTTP_LC_PACKET_SUBSCRIBE_NONE = 11;
   public final static byte HTTP_LC_PACKET_SUBSCRIBE_MED  = HTTP_LC_PACKET_SUBSCRIBE;
   public final static byte HTTP_LC_PACKET_SUBSCRIBE_HIGH = 13;
   public final static byte HTTP_LC_PACKET_SET     = HTTP_LC_PACKET_VALUE;
   public final static byte HTTP_LC_PACKET_GET     = 14;
   public final static byte HTTP_LC_PACKET_GETNEXT = 15;
   final static byte HTTP_LC_PACKET_FINISHED       = 127;

   /**
     * Initialize the Live Control communication client and connect to the
     * server specified by myURL using the WWW proxy myProxy.
     * @param myURL valid URL specifying the Live Control server to connect to
     * @param myProxy valid URL specifying the proxy that should be used
     */
   public synchronized void init ( URL myURL , URL myProxy ) throws IOException {
      lcProxy = myProxy;
      init(myURL);
   }

   /**
     * Initialize the Live Control communication client and connect to the
     * server specified by myURL.
     * @param myURL valid URL specifying the Live Control server to connect to
     */
   public synchronized void init (URL myURL) throws IOException {
      lcURL = myURL;
      lcProxy = myURL;
      init();
   }

   /**
     * Initialize the Live Control communication client and connect to the
     * default server.
     * Any registry will be notified when connection is establishment was successful.
     * @exception IOException thrown if no connection to the server could be established.
     */
   public synchronized void init () throws IOException {
      if ( symTab == null ) {
         symTab = new SymbolTable();
      }

      if ( java.lang.Thread.currentThread() != readThread ) {
         stop();
      } else {
         close();
      }

      Log.log(5,"trying to connect to "+lcURL);
      try {
         if ( rConnection == null ) {
            rConnection = new LiveControlProtocol(lcURL,lcProxy.getHost(),lcProxy.getPort());
         }
         if ( wConnection == null ) {
            wConnection = new LiveControlProtocol(lcURL,lcProxy.getHost(),lcProxy.getPort());
         }

         // initialize persistent receive link
         rConnection.sendSynchronizedRequest(null);
         wConnection.setCookie(rConnection.getCookie());

         // tell registry about new connection
         listeners.firePropertyChange ( ".reconnected" , null , this );
         Log.log (5,"connected.");
      } catch (Exception e) {
         Log.log (0, "Not connected because of: "+e );
         close();
         e.printStackTrace();
         throw new IOException("init(): failed because of "+e);
      }

      if ( readThread != null )
         readThread.shutdown();
      readThread = new LiveConnectionReader(this);
      readThread.start();

      Log.log ( 25,"init finished." );
      try {
         java.lang.Thread.sleep(1000);
      } catch (Exception xxx) {
      }
   }

   /**
     * Retrieve the input stream of the current connection
     * @return InputStream of persistent read connection
     * @exception IOException thrown when no connection is available.
     */
   protected synchronized InputStream getInputStream() throws IOException {
      return rConnection.getInputStream();
   }

   /**
     * Shut down the reader-thread and close all network connections.
     */
   public synchronized void stop () {
      LiveConnectionReader t = readThread;

      close();
      readThread = null;
      if ( t!= null ) {
         t.shutdown();
         //t.waitFinished();
      }
      close();
   }


   /**
     * Close down any network connections and notify any registry.
     * The reader thread will not be stopped, but remains inoperable!
     */
   public synchronized void close () {
      byte cmd[] = { HTTP_LC_PACKET_QUIT };

      listeners.firePropertyChange ( ".reconnected" , this , null );

      if ( rConnection != null ) {
         rConnection.destroy();
         // rConnection.disconnect();
         rConnection=null;
      }
      try {
         if ( wConnection != null) {
            wConnection.destroy();
            // wConnection.disconnect();
            wConnection=null;
            // transmitRequest ( new Packet ( cmd ) );
         }
      } catch (Exception e) {
      }
   }

   /**
    * Determine if there is a working connection available to the server.
    * @return boolean value reflecting the availability of connections.
    */
   public synchronized boolean isConnected() {
      try {
         if ( rConnection != null )
            return rConnection.isConnected();
      } catch (Exception x) {
      }
      return false;
   }

   /**
     * Shut down open network connection if available and reap read-thread.
     * Usually this method is called by the Java garbage collector and should
     * not be called directly.
     * Use close() instead of.
     * @exception Throwable
     * @see http.livecontrol.comm.LiveControlConnection#close
     */
   public void finalize() throws Throwable {
      Log.log ( "finalize: cleaning up..." );
      stop();
      super.finalize ();
   }

   /**
     * Registration of server side symbols.
     * In consequence the server will provide an initial value and
     * subsequent updates any time the symbol value is changed at the server.
     *
     * Additionally the LiveControlConnection object keeps track of all subscribed
     * symbols and their properties.
     *
     * @param name name of the symbol to subscribe
     * @param pollingPriority Type of value polling update requested.
     *
     * @return always true as the Live Control protocol currently supports only
     * asynchronous communication.
     * @exception IOException if subscribe can not be performed due to network connection problems.
     */
   public synchronized boolean subscribe (String name, byte pollingPriority)  throws IOException {
      Symbol  symbol;
      boolean isSubscribed = false;
      byte    cmd[] = { HTTP_LC_PACKET_SUBSCRIBE };

      cmd[0] = pollingPriority;

      Log.log ( 30,"trying to subscribe <"+name+">,priority"+pollingPriority );
      symbol = symTab.get ( name );
      if ( symbol != null ) {
         if ( ( symbol.getStatus() == Symbol.SUBSCRIBED ) || ( symbol.getStatus() == Symbol.WAITING ) )
            isSubscribed = true;
      } else {
         // write dummy symbol to symbol table (put will provide unique entries)
         symTab.put ( new Symbol (name , Symbol.WAITING ) );
         // to be thread-safe, read symbol from symbol table
         symbol = symTab.get ( name );
         Log.log ( 35, "added dummy symbol <"+name+"> to symbol table." );
      }

      if ( isSubscribed == false ) {
         // asynchronous subscribe
         symbol.setStatus ( Symbol.WAITING );
         transmitRequest ( new Packet ((new String (cmd))+name ));
         Log.log (35, "subscribed "+name );
      } else {
         Log.log("Symbol <"+name+"> already subscribed");
      }
      // no synchronous subscribe possible now
      return true;
   }

   public synchronized boolean subscribe (String name)  throws IOException {
	   return ( subscribe (name, HTTP_LC_PACKET_SUBSCRIBE_MED) );
   }


    /**
     * Read the Symbol from server. Instance or argument syntax is allowed
     * In consequence the server will provide an initial value and
     * subsequent updates any time the symbol value is changed at the server.
     *
     * Additionally the LiveControlConnection object keeps track of all subscribed
     * symbols and their properties.
     *
     * @param name name of the symbol to subscribe
     *
     * @return always true as the Live Control protocol currently supports only
     * asynchronous communication.
     * @exception IOException if subscribe can not be performed due to network connection problems.
     *
     * since Wind Manage Web 4.3
     */
   public synchronized boolean getValue (String name)  throws IOException {
      Symbol  symbol;
      boolean isWaiting = false;
      byte    cmd[] = { HTTP_LC_PACKET_GET };

      Log.log ( 30,"getValue trying to get <"+name+">" );
      symbol = symTab.get ( name );
      if ( symbol != null ) {
         if ( symbol.getStatus() == Symbol.WAITING )
            isWaiting = true;
      } else {
         // write dummy symbol to symbol table (put will provide unique entries)
         symTab.put ( new Symbol (name , Symbol.WAITING, false ) );
         // to be thread-safe, read symbol from symbol table
         symbol = symTab.get ( name );
         Log.log ( 35, "added dummy symbol "+name+" to symbol table." );
      }

      if ( isWaiting == false ) {
         // asynchronous subscribe
         symbol.setStatus ( Symbol.WAITING );
         transmitRequest ( new Packet ((new String (cmd))+name ));
         Log.log (35, "subscribed <"+name+">" );
      } else {
         Log.log("Symbol "+name+" already in wait state.");
      }
      // no synchronous subscribe possible now
      return true;
   }

    /**
     * Read the Symbol with the next instance from server. Instance field of returned name will change.
     * In consequence the server will provide an initial value and
     * subsequent updates any time the symbol value is changed at the server.
     *
     * Additionally the LiveControlConnection object keeps track of all subscribed
     * symbols and their properties.
     *
     * @param name name of the symbol to subscribe
     *
     * @return always true as the Live Control protocol currently supports only
     * asynchronous communication.
     * @exception IOException if subscribe can not be performed due to network connection problems.
     *
     * since Wind Manage Web 4.3
     */
   public synchronized boolean getNextValue (String name)  throws IOException {
      Symbol  symbol;
      boolean isWaiting = false;
      byte    cmd[] = { HTTP_LC_PACKET_GETNEXT };

      Log.log ( 30,"trying to getNext <"+name+">" );
      symbol = symTab.get ( name );
      if ( symbol != null ) {
         if ( symbol.getStatus() == Symbol.WAITING )
            isWaiting = true;
      } else {
         // write dummy symbol to symbol table (put will provide unique entries)
         symTab.put ( new Symbol (name , Symbol.WAITING, true ) );
         // to be thread-safe, read symbol from symbol table
         symbol = symTab.get ( name );
         Log.log ( 35, "added dummy symbol "+name+" to symbol table." );
      }

      if ( isWaiting == false ) {
         // asynchronous subscribe
         symbol.setStatus ( Symbol.WAITING );
         transmitRequest ( new Packet ((new String (cmd))+name ));
         Log.log (35, "subscribed "+name );
      } else {
         Log.log("Symbol "+name+" already in wait state");
      }
      // no synchronous subscribe possible now
      return true;
   }

  /**
     * Deregistration of server side symbols.
     * In consequence the server will not provide further
     * updates if the symbol value is changed at the server.
     *
     * @param name name of the symbol to unsubscribe
     * @exception IOException if unsubscribe can not be performed due to network connection problems.
     */
   public synchronized void unsubscribe (String name)  throws IOException {
      Symbol symbol;
      boolean isunsubscribed = false;
      byte cmd[] = { HTTP_LC_PACKET_UNSUBSCRIBE };

      Log.log ( 30,"Trying to unsubscribe <"+name+">" );
      symbol = symTab.get(name);

      if ( symbol != null ) {
         if ( ( symbol.getStatus() == Symbol.SUBSCRIBED ) || ( symbol.getStatus() == Symbol.WAITING ) ) {
            symbol.setStatus (Symbol.UNUSED);
            transmitRequest ( new Packet (( new String(cmd))+name ) );
            Log.log ( 35,"unsubscribed "+name );
            isunsubscribed = true;
         }
      }
      // if ( isunsubscribed ) {
      // throw new IllegalAccessException("Symbol "+name+" not subscribed");
      // }
   }


   /**
     * Set the interval between symbol value checks in milli seconds.
     * This function is only supported in conjunction with the observer package.
     * Currently unavailable!
     */
   private synchronized void setInterval (String name, int interval) throws IOException {
      Log.log (0, "setInterval() is useful only with observer package (not available)" );
   }


   /**
     * Set a symbol to a new value at the server.
     * The server may or may not accept this attempt.
     * If the attempt is successful, all Live Control clients will be updated, too.
     * @exception IOException if network connection problems occured during operation.
     */
   public synchronized void setValue (String name, byte[] content) throws IOException {
      byte cmd[] = { HTTP_LC_PACKET_VALUE};
      String encode;


      encode = new String(cmd,"ISO-8859-1");
      encode += name;
      encode += new String(separators,"ISO-8859-1");
      encode += new String(content,"ISO-8859-1");


      {
         int i;
         byte decode[];
         String output = "***setValue() encode[]=";
         decode = encode.getBytes("ISO-8859-1");

         for (i=0; i<decode.length; i++) {
            output += "[" + Integer.toHexString( (int)decode[i] ) + "]";
         }
         Log.log ( 25,output);
      }

      Packet txReq = new Packet( encode );

      transmitRequest ( txReq );
   }

   /**
     * Register Java components to receive update events.
     * Any symbol value updates received from the server are published to
     * other Java components as properties.
     * If updates occur, the reader thread will distribute PropertyChangeEvents
     * containing the changed symbol´s name and its new value.
     * The new value is represented as byte array.
     *
     * @param newListener listening component that should receive PropertyChangeEvents in the future
     *
     * @see java.beans.PropertyChangeEvent
     * @see java.beans.PropertyChangeListener
     */
   public void addListener ( PropertyChangeListener newListener ) {
      listeners.addPropertyChangeListener (newListener);
   }

   /**
     * Deregister Java components receiving update events.
     * The deregistered component will not receive any further PropertyChangeEvents
     * containing symbol value updates.
     *
     * @param newListener listening component that should not receive PropertyChangeEvents any longer
     *
     * @see java.beans.PropertyChangeListener
     */
   public void removeListener ( PropertyChangeListener oldListener ) {
      listeners.removePropertyChangeListener (oldListener);
   }

   /**
     * Get the symbol table with all entries currently available.
     * The LiveControlConnection class collects info about all symbols
     * available from the embedded system connected to and stores these
     * data within the client side symbol table.
     * Attention: Depending on the communication progress symbol entries may
     * not be available or incomplete!
     *
     * @return reference to the client side symbol table.
     */
   public synchronized SymbolTable getSymbolTable () {
      return symTab;
   }

   /**
     * Transmit a complete Live Contro request containing packet p
     */
   private synchronized void transmitRequest ( Packet p ) throws IOException {
      int retries = 3;
      Log.log (25,">>>>>>>>>>>>>TRANSMIT START<<<<<<<<<<<<<<<<<" );
      while (true) {
         try {
            transmit ( p );
            Log.log (25,">>>>>>>>>>>>>TRANSMIT END<<<<<<<<<<<<<<<<<" );
            return;
         } catch (IOException x) {
            if ( retries-- > 0 ) {
               Log.log ( 5,"transmit got IOException (retry="+retries+"). "+x);
            } else {
               Log.log ( 0,"transmit got IOException, failed). "+x);
               close();
               listeners.firePropertyChange ( ".reconnected" , this , null );
               throw x;
            }
         }
      }
   }

   private synchronized void transmit (Packet p) throws IOException {
      try {
         wConnection.sendSynchronizedRequest ( p.getBytes() );
      } catch (NullPointerException n) {
         throw new IOException ("transmit: not connected!");
      } catch (Exception ex) {
         Log.log (5,"Communication error: " + ex);
         throw new IOException ("transmit: "+ex.toString());
      }
   }

   private void requestSymbolTable() {
      byte cmd[] = { HTTP_LC_PACKET_SYMBOLTABLE };

      try {
         transmitRequest ( new Packet (cmd) );
      } catch (IOException ex) {
      }
   }


   // ==========================================================
   // ----------------------------------------------------------
   // inner classes
   // ----------------------------------------------------------
   class PacketParser {

      // private vars
      private byte    tail[];
      private byte    cmd;
      private int     items;
      private String  token[];

      private Logging Log = new Logging (System.out, 0, "PacketParser");


      // --------------------------------------------------------------
      // constructors
      public PacketParser ( byte content[] , int itemcount) throws NoSuchElementException {
         items = itemcount;
         cmd = content[0];
         setTokens (content);
      }

      // --------------------------------------------------------------
      // methods
      public final byte[] getTail () {
         return tail;
      }

      public final String getToken (int i) {
         return token[i];
      }

      public final byte getCommand () {
         return cmd;
      }

      // -------------------------------------------------------------
      // private methods
      private final void setTokens ( byte bytes[] )  {
         int             length = 0;
         String          fullValue;
         StringTokenizer tokenizer;

         try {
            fullValue = new String (bytes,"ISO-8859-1");
         } catch (Exception e) {
            Log.log ( 0,"setTokens: Exception " + e);
            return;
         }

         tokenizer = new StringTokenizer( fullValue.substring(1) , new String(separators) , false );
         token = new String[items];

         for ( int i=0 ; i<items ; i++ ) {
            try {
               token[i] = tokenizer.nextToken();
            } catch (NoSuchElementException ex) {
               token[i] = new String("--->unknown<---");
            }
            length += token[i].length();
         }

         length += items;

         try {
            tail = fullValue.substring(length).getBytes("ISO-8859-1");

            {
               int i;
               String output = "setTokens: tail[]=";

               for (i=0; i<tail.length; i++) {
                  output += "[" + Integer.toHexString( (int)tail[i] ) + "]";
               }
               Log.log ( 25,output);
            }
         } catch (StringIndexOutOfBoundsException ex) {
            tail = null;
            Log.log ( 25,"setTokens: packet "+fullValue+" has no tail");
         } catch (Exception e) {
            Log.log ( 0,"setTokens: Exception " + e);
            return;
         }
      }
   }


   // ===================================================================
   // LiveConnectionReader implements Live Connect read functionality
   // -------------------------------------------------------------------
   class LiveConnectionReader extends Thread {
      // private vars
      private Logging     Log = new Logging(System.out, 8, "LiveControlConnection - reader");
      private boolean     running;
      private boolean     finished=true;
      private LiveControlConnection lc;

      public LiveConnectionReader () {
         running = false;
      }

      public LiveConnectionReader (LiveControlConnection lcconnection) {
         this.init(lcconnection);
      }

      public final void init(LiveControlConnection lcconnection)
      	{
		lc = lcconnection;
		running = false;
      	}

      public final void run ()
		{
		 InputStream in;

		 Log.log (10,"Running in Listening Thread...");
		 running = true;
		 finished = false;
		 try
		 	{
			this.sleep(1000);
		 	}
		 catch (Exception x)
		 	{
		 	}

		 while (running)
		 	{
			try
				{
				while (lc.isConnected() && running)
					{
				  	try
				  		{
						 in = lc.getInputStream();
						 if ( in != null )
							getRequestBody(in);
				  		}
				  	catch ( IOException e )
				  		{
						Log.log ( 0,"communication failure: "+e );
						lc.close();
					  	}
			   		}
				// try to reconnect
				if ( running )
					lc.init();
				}
			catch ( IOException e )
				{
			   	try
			   		{
				  	this.sleep(5000);
			   		}
			   	catch (Exception x)
			   		{
			   		}
				}
			catch ( Exception e )
				{
				// if an unexpected exception occurs, shut down connection and reader thread
				Log.log ( 0,"NON COMMUNICATION ERROR - stopping myself." );
				destroy();
				}
			 }
		destroy();
		}

      public final void destroy()
      	{
         running = false;
         finished = true;
         lc = null;
         // super.destroy();
      	}

      public final void shutdown()
      	{
         running = false;
         lc=null;
      	}

      public final boolean isRunning() {
         return running;
      }

      public final boolean isFinished() {
         return finished;
      }

      public final void waitFinished() {
         try {
            while ( !finished )
               this.sleep(500);
         } catch (Exception x) {
         }
      }

      public final synchronized void getRequestBody (InputStream instream) throws IOException {
         Packet          newPacket;
         PacketParser    parse;
         String          symbol = "unknown";
         byte            value[] = {};

         while (running) {
            try {
               newPacket = new Packet(instream);

               switch ( newPacket.getCommand() ) {
               case HTTP_LC_PACKET_VALUE:
                  parse  = new PacketParser ( newPacket.getContent() , 1 );
                  symbol = parse.getToken ( 0 );
                  value  = parse.getTail ();
                  Log.log (35 ,"getRequestBody: Got notify for symbol <"+symbol+">, value length "+value.length+": " );
                  listeners.firePropertyChange ( symbol , null , value );
                  break;
               case -HTTP_LC_PACKET_SUBSCRIBE:
                  parse  = new PacketParser ( newPacket.getContent() , 1 );
                  symbol = parse.getToken ( 0 );

                  // tell subscribe method about acknowledge...
                  parse  = new PacketParser ( newPacket.getContent() , 1 );
                  symbol = parse.getToken ( 0 );
                  Log.log ( 35,"getRequestBody: Subscribe acknowledge for symbol <"+symbol+">" );
                  try {
                     symTab.get ( symbol ).setStatus(Symbol.SUBSCRIBED);
                  } catch (Exception ex) {
                     Log.log (25, "getRequestBody: Subscribe-acknowledge for non-existent symbol <"+symbol+"> !" );
                  }
                  break;
               case -HTTP_LC_PACKET_GET:
                  // tell subscribe method about acknowledge...
                  parse  = new PacketParser ( newPacket.getContent() , 1 );
                  symbol = parse.getToken ( 0 );
                  Log.log (35,"getRequestBody: Get acknowledge for symbol "+symbol );
                  try {
                     symTab.get ( symbol ).setStatus(Symbol.SUBSCRIBED);
                  } catch (Exception ex) {
                     Log.log (25, "getRequestBody: Get-acknowledge for non-existent symbol <"+symbol+"> !" );
                  }
                  break;
               case -HTTP_LC_PACKET_GETNEXT:
                  // tell subscribe method about acknowledge...
                  parse  = new PacketParser ( newPacket.getContent() , 1 );
                  symbol = parse.getToken ( 0 );
                  Log.log ( 35,"getRequestBody: Get acknowledge for symbol <"+symbol+">" );
                  try {
					 /* Name will update instance fields for GetNext names */
                     symTab.get ( symbol ).setStatus(Symbol.SUBSCRIBED);
                  } catch (Exception ex) {
                     Log.log (25, "getRequestBody: GetNext-acknowledge for non-existent symbol <"+symbol+"> !" );
                  }
                  break;
               case HTTP_LC_PACKET_DESCRIPTION:
                  parse       = new PacketParser ( newPacket.getContent() , 4 );
                  symbol      = parse.getToken (0);
                  byte status = Symbol.UNUSED;
                  try {
                     Symbol mySym = symTab.get(symbol);
                     Log.log (10, "getRequestBody: Description for symbol <"+symbol+">: "+mySym.getType());
                     status = mySym.getStatus();
                  } catch (Exception e) {
                  }
                  symTab.replace (new Symbol ( symbol,parse.getToken (2),parse.getToken (1),1,status)); // XXX numitems missing
                  break;
               case HTTP_LC_PACKET_ERROR:
                  parse = new PacketParser ( newPacket.getContent() , 1 );
                  Log.log ( 20,"getRequestBody: Got error message: "+parse.getToken ( 0 ) );
                  break;
               default:
                  Log.log ( 5,"getRequestBody: Unrecognized packet!" );
                  break;
               }
            } catch (IOException e) {
               Log.log (0, "getRequestBody: Communication error (->shutdown):" + e);
               throw e;
            } catch (NoSuchElementException e) {
               Log.log ( 5,"getRequestBody: Tokenizing failed: "+e );
               e.printStackTrace();
               throw new IOException("getRequestBody: Tokenizing failed: "+e);
            }
         }
      }
   }

}
