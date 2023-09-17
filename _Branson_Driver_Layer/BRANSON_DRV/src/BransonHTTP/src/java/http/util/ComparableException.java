/*  ComparableException.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
*/

package http.util;

import java.lang.Exception;


public class ComparableException extends Exception
{
  public ComparableException (String message)
  {
    super("ComparableException: " + message);
  }

  public ComparableException()
  {
    super ("ComparableException");
  }
}

