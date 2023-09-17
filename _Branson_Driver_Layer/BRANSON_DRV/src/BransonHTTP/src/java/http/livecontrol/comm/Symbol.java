/*  Symbol.java - Mirror the contents of a target symbol table entry */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,02jan04,dlr  Port to WM Web
01b,07may98,mb     Live Control rework
01a,14jan97,ck     initial creation
*/


package http.livecontrol.comm;

import http.util.Comparable;
import http.util.ComparableException;


/**
  * Live Control communication operates with symbolic names as well as the
  * eHTTPd symbol table concept.
  */
public class Symbol implements Comparable {

  // possible states a symbol may have
  public final static byte UNDEF      = 0;
  public final static byte UNUSED     = 1;
  public final static byte WAITING    = 2;
  public final static byte SUBSCRIBED = 3;

  private String   name;
  private String   description;
  private String   type;
  private int      items = 0;
  private byte     status = UNUSED;
  private boolean  valid;
  private boolean  enableNewInstance = false;

  //  private Proxy    proxy = null;
  private Logging  Info = new Logging (System.out, 20, "Symbol");


  /**
    * Create a completely initialized Symbol.
    */
  public Symbol (String myName, String myDescription, String myType, int myItems, byte myStatus, boolean myEnableNewInstance) {
    name        = myName;
    description = myDescription;
    type        = myType.toLowerCase();
    items       = myItems;
    status      = myStatus;
    valid       = true;
    enableNewInstance = myEnableNewInstance;
    Info.log ( "Symbol("+this.toString()+") created." );
  }

  /**
    * Create a completely initialized Symbol.
    */
  public Symbol (String myName, String myDescription, String myType, int myItems, byte myStatus) {
    name        = myName;
    description = myDescription;
    type        = myType.toLowerCase();
    items       = myItems;
    status      = myStatus;
    valid       = true;
    enableNewInstance = false;
    Info.log ( "Symbol("+this.toString()+") created." );
  }

  /**
    * Create a dummy Symbol without any information from the target system.
    */
  public Symbol (String myName, byte myStatus) {
    name        = myName;
    description = null;
    type        = null;
    items       = 0;
    status      = myStatus;
    valid       = false;
    enableNewInstance = false;
  }

  /**
    * Create a dummy Symbol without any information from the target system.
    * Tag this if it will accept a new instance
    */
  public Symbol (String myName, byte myStatus, boolean bEnableNewInstance) {
    name        = myName;
    description = null;
    type        = null;
    items       = 0;
    status      = myStatus;
    valid       = false;
    enableNewInstance = bEnableNewInstance;
  }

  /**
    * Get the kind of this Symbol.
    * @return true if Symbol was completely initialized.
    * @return false if symbol is a dummy symbol.
    */
  public boolean isInitialized() {
    return valid;
  }


  /**
    * Get the name of this symbol.
    * @return Name of the symbol.
    */
  public String getName () {
    return (name);
  }

  /**
    * Get the length of the root name of this symbol.
    * @return length of the root name.
    */
  public static int getWindMarkNameLength (String WindMark)
	{
	int i = getWindMarkInstanceOffset (WindMark);

	if (i>0)
		return(i);

	return(WindMark.length());
	}

  /**
    * Get the length of the Instance of this symbol, if any.
    * @return length of the Instance, or 0 if no instance.
    */
  public static int getWindMarkInstanceLength (String WindMark)
  	{
	int i = getWindMarkInstanceOffset (WindMark);

	if (i>0)
		return(WindMark.length() - i);

	return(0);
  	}

  /**
    * Get the offset of the instance part of the name, if any.
    * @return offset of the instance, or -1 if no instance.
    */
  public static int getWindMarkInstanceOffset (String WindMark) {
	int i;

    if (0 < (i = WindMark.indexOf ('.')) )
    	return(i);
    if (0 < (i = WindMark.indexOf ('(')) )
    	return(i);

   	return(-1);
  }

  /**
    * Get the description of this symbol.
    * @return Description of the symbol.
    */
  public String getDescription () {
    return (description);
  }


  /**
    * Get the type of this symbol.
    * @return Type string of the symbol.
    */
  public String getType () {
    return (type);
  }


  /**
    * Get the number of items this symbol contains.
    * @return number of items.
    */
  public int getItems () {
    return (items);
  }

  /**
    * Get the number of items this symbol contains.
    * @return number of items.
    */
  public boolean getEnableNewInstance() {
    return (enableNewInstance);
  }


  /**
    * Get the status of this symbol.
    * @return UNDEF if symbol is undefined
    * @return UNUSED if symbol is currently not in use
    * @return WAITING if symbol has being subscribed, but has not yet received the subscribe acknowledge
    * @return SUBSCRIBED symbol has been successfully subscribed
    * @see http.livecontrol.comm.Symbol#setStatus
    */
  public byte getStatus () {
    return (status);
  }


  /**
    * Set the status of this symbol.
    * @param newStatus UNDEF, UNUSED, WAITING, SUBSCRIBED
    * @see http.livecontrol.comm.Symbol#getStatus
    */
  public void setStatus (byte newStatus) {
    status = newStatus;

  }


  /**
    * Convert the symbol information to string representation.
    * @return descriptive string containing all symbol information
    */
  public String toString () {
    return ("Symbol: Name=" + name +
			", Descr=" + description +
			", Type=" + type +
			", Items=" + items +
			", status=" +status);
  }

  // implementation of interface Comparable
  /**
    * Compares two symbols.
    * @param what Another symbol to compare this symbol with
    */
  public int compareTo(Comparable what) throws ComparableException {
    int res = 0;

	/* if enableNewInstance, compare only the name part of the WindMark/symbol */
	if (enableNewInstance)
		{

		if (what instanceof Symbol)
		  {
		  String  thisName = new String( name );
		  String  testName = new String(((Symbol)what).getName() );
		  res = (thisName.compareTo(testName));
	      }
		else
		  {
			if (super.equals(what))
			  return (0);
			else
			  throw new ComparableException ("Symbol not comparable to " + what.getClass().getName());
		  }
	  }
	else
		{
		if (what instanceof Symbol)
		  res = (name.compareTo(((Symbol)what).getName()));
		else
		  {
			if (super.equals(what))
			  return (0);
			else
			  throw new ComparableException ("Symbol not comparable to " + what.getClass().getName());
		  }
	  }

    Info.log (100, "Compare " + this + " to " + what + " gives " + res);
    return (res);
  }

}














