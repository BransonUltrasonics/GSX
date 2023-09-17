/* ConverterFactory.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.livecontrol.converter;

import java.lang.Object;
import java.lang.Class;
import java.lang.String;
import java.lang.Exception;
import java.lang.RuntimeException;

import http.livecontrol.converter.*;


public class ConverterFactory {

  // public vars
  public final static String types[] =
  {
    "http.livecontrol.converter.FloatConvert",
    "http.livecontrol.converter.LongConvert",
    "http.livecontrol.converter.ShortConvert",
    "http.livecontrol.converter.StringConvert",
    "http.livecontrol.converter.BoolConvert",
    "http.livecontrol.converter.ByteConvert"
  };

  // local vars
  // private static Convertible converters[];
  private static Convertible converters[];


  public ConverterFactory () throws RuntimeException
  	{
	int   i;
	Object newConverter;

	converters = new Convertible[types.length];
	for ( i=0 ; i<types.length ; i++ )
		{
		try {
			converters[i] = (Convertible)Class.forName(types[i]).newInstance();
		  	}
		catch ( Exception e )
			{
			e.printStackTrace();
			throw new RuntimeException ( "ConverterFactory can not create instance of "+types[i] );
		 	}
		}
  	}

  public Convertible getConverter ( String type )
  	{
    int i,k;
    String supportedTypes[];

    for ( i=0 ; i<converters.length ; i++ )
    	{
		supportedTypes = converters[i].getValidTypeNames();
		for ( k=0 ; k<supportedTypes.length ; k++ )
			{
			if ( supportedTypes[k].compareTo(type) == 0 )
				{
		  		return converters[i];
				}
		  	}
		}
    return null;
  }

}
