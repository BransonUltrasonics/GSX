/*  LiveControlException.java   - */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,02jan04,dlr  Port to WM Web
*/

package http.livecontrol.comm;

import java.lang.Exception;


public class LiveControlException extends Exception
{
  public LiveControlException (String message)
  {
    super("LiveControl Exception: " + message);
  }

  public LiveControlException()
  {
    super ("LiveControl Exception!");
  }
}

