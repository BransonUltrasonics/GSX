/*  SymbolTable.java   - */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02c,22mar07,ps  rename variable "enum". Defect WIND00091285
02b,11feb04,dlr	Support for Instances
02a,02jan04,dlr	Port to WM Web
01a,24apr98,mb	initial creation
*/

package http.livecontrol.comm;

import java.util.Hashtable;
import java.util.Enumeration;
import java.util.Vector;
import java.util.StringTokenizer;
import java.io.InputStream;
import java.io.DataInputStream;

import http.util.Sort;
import http.util.Comparable;
import http.util.ComparableException;




/**
  * The Live Control communication system offers a client side representation of
  * the symbol table inside the embedded system. Class symbol table collects all
  * symbols known to the client for further processing.
  */
public class SymbolTable extends Hashtable
	{
	private   Logging   Log    = new Logging (System.out, 10, "SymbolTable");

	public SymbolTable () {
	super();
	Log.log ( "new symbol table created." );
	}

  /**
    * Add new symbols, deny duplicates.
    * If several symbols with the same name are tried to be added to the symbol
    * table, only the first symbol will be stored and all successing ones are
    * ignored.
    */
public synchronized void put(Symbol symbol)
	{
    if ( this.containsKey ( symbol.getName() ) == true )
      return;
    super.put ( symbol.getName(),symbol );
  	}


  /**
    * Get a symbol.
    * Get a certain symbol with the specified name.
    * @param name name of the symbol
    * @return symbol with specified name or null if there is no such symbol.
    */
public synchronized Symbol get (String name)
	{
    return ((Symbol)super.get(name));
  	}


  /**
    * Replace symbol with the same name.
    * Remove an existing symbol with the same name as the specified
    * symbol before storing the new one.
    * @param sym symbol that should be stored inside the symbol table instead of any other symbol with same name.
    */
public synchronized void replace (Symbol sym)
	{
    this.remove ( sym.getName() );
    this.put ( sym.getName() , sym );
	}


  /**
    * Replace symbol with the changed name (e.g. new instance part of name).
    * Remove an existing symbol with the same name as the specified
    * symbol before storing the new one.
    * @param sym symbol that should be stored inside the symbol table instead of any other symbol with same name.
    */
public synchronized void replace (String oldName, Symbol sym)
	{
    this.remove ( oldName );
    this.put ( sym.getName() , sym );
	}


  /**
    * Replace symbol with the changed name (e.g. new instance part of name).
    * Remove an existing symbol with the same name as the specified
    * symbol before storing the new one.
    * @param sym symbol that should be stored inside the symbol table instead of any other symbol with same name.
    */
public synchronized void replaceInstance (String oldName, String newName)
	{
	Log.log (20,"SymbolTable: replace<"+oldName+">with<"+newName+">");

    // do find over all keys in hashtable...
    Enumeration enumval  = this.elements();
	int keyNameLength = Symbol.getWindMarkNameLength( oldName );
	String keyName    = oldName.substring(0,keyNameLength);

    while (enumval.hasMoreElements())
		{
		Symbol tstSymbol  = (Symbol) enumval.nextElement();
		String tstName    = tstSymbol.getName();
		int tstNameLength = Symbol.getWindMarkNameLength( tstName );

		/* same root name? */
		if ((keyNameLength == tstNameLength) && tstName.startsWith(keyName))
			{
			Symbol newSymbol = new Symbol(newName,
				tstSymbol.getDescription (),
				tstSymbol.getType   (),
				tstSymbol.getItems  (),
				tstSymbol.getStatus (),
				tstSymbol.getEnableNewInstance()
				);

			this.remove ( tstSymbol.getName() );
			this.put    ( newSymbol.getName() , newSymbol );
			}
		}
	}


  /**
    * Get all symbols matching a specified name pattern.
    * This function allows the selection of a set of symbols
    * depending on their name.
    * @return array of symbols with matching name.
    * @param namePattern wildcard expression which may contain '*'
    */
public Symbol[] getByName (String namePattern)
	{
    Vector      result = new Vector ();
    Enumeration enumval = null;

    if (namePattern.indexOf ('*') < 0)
    	{
		Symbol[] res = new Symbol[1];
		res[0] = (Symbol) this.get (namePattern);
		return (res);
		}

    // do wildcard find over all keys in hashtable...
    enumval = this.keys();
    while (enumval.hasMoreElements())
		{
		String current = (String) enumval.nextElement();

		if (StringMatches (current, namePattern))
			result.addElement (this.get (current));
		}
    return ((Symbol[]) SymbolVectorToArray (result));
	}


  /**
    * Get all symbols matching a specified type pattern.
    * This function allows the selection of a set of symbols
    * depending on their type.
    * @return array of symbols with matching type.
    * @param typePattern wildcard expression which may contain '*'
    */
public Symbol[] getByType (String typePattern)
  	{
    Vector      result = new Vector ();
    Enumeration enumval = this.elements();

    // do wildcard find over all types in hashtable...
    while (enumval.hasMoreElements())
		{
		Symbol current = (Symbol) enumval.nextElement();

		if (StringMatches (current.getType(), typePattern))
			result.addElement (this.get (current.getName()));
		}
    return ((Symbol[]) SymbolVectorToArray (result));
	}


  /**
    * Get all symbols matching a specified description pattern.
    * This function allows the selection of a set of symbols
    * depending on their description.
    * @return array of symbols with matching description.
    * @param descriptionPattern wildcard expression which may contain '*'
    */
public Symbol[] getByDescription (String descriptionPattern)
  	{
    Vector      result = new Vector ();
    Enumeration enumval = this.elements();
    // do wildcard find over all descriptions in hashtable...

    while (enumval.hasMoreElements())
		{
		Symbol current = (Symbol) enumval.nextElement();

		if (StringMatches (current.getDescription(), descriptionPattern))
			result.addElement (this.get (current.getName()));
		}
    return ((Symbol[]) SymbolVectorToArray (result));
  }


  // -----------------------------------------------------
  // private methods
  // -----------------------------------------------------
private String[] StringVectorToArray (Vector vect)
	{
    String[] tmp = new String [vect.size()];

    for (int i=0; i < vect.size(); i++)
      tmp[i] = (String) vect.elementAt (i);

    return (tmp);
	}

private Symbol[] SymbolVectorToArray (Vector vect)
  	{
    Symbol[] tmp = new Symbol [vect.size()];

    for (int i=0; i < vect.size(); i++)
      tmp[i] = (Symbol) vect.elementAt (i);

    try {
		Sort.Sort (tmp);
    	}
    catch (ComparableException ex)
    	{
  		Log.log (5,"Uncomparable elements in array... this never should happen");
		}

    return (tmp);
	}


  //
  // This is not quite correct (TODO) ab*ab does not match ababcdab.
  // This is case sensitive!
  //
private static boolean StringMatches(String what, String pattern)
	{
    StringTokenizer st = new StringTokenizer(pattern,"*");
    boolean pmustbe0 = !pattern.startsWith("*");

    while (st.hasMoreTokens())
		{
		String s = st.nextToken();
		int p = what.indexOf(s);
		if (pmustbe0)
			{
			pmustbe0 = false;// only once in the beginning
			if (p != 0)
				return false;
			}
		if (p>=0)
			{
			what = what.substring(p+s.length(),what.length());
		  	}
		else
			{
			return false;
		  	}
		}
	return ((what.length() == 0) || pattern.endsWith("*"));
	}


  /**
    * Get all symbols and display in the log. Used for debugging.
    */
public synchronized void dbgDisplay ()
	{
    Enumeration enumval = null;
    int			i=0;

    // do wildcard find over all keys in hashtable...
    enumval = this.keys();
    while (enumval.hasMoreElements())
		{
		String current = (String) enumval.nextElement();
		Log.log (0 ,"SymbolTable ["+ i +"]:<"+current+">");
		i++;
		}
	}

}

