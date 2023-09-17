/*  Logging.java   -  Debug information logging */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,02jan04,dlr  Port to WM Web
01a,12dec97,ck     initial creation
*/

package http.livecontrol.comm;

import java.io.*;
import java.lang.*;

public class Logging extends Object
{
  private int         iDebugLevel = 1;
  private String      sModuleName = null;
  private PrintStream Output = null;
  private static int  verbosity = 5;

  /*
   * Constructors...
   */

  public Logging (PrintStream Out, int iDefaultDebugLevel, String sModule) {
    iDebugLevel = iDefaultDebugLevel;
    sModuleName = sModule;
    Output      = Out;
  }

  public final static synchronized void setVerbosityLevel (int newVerbosity)
  {
    verbosity = newVerbosity;
  }

  /*
   * Access methods
   */

  public final void log (String sMessage) {
    log ( iDebugLevel , sMessage );
  }

  public final void log (int iLevel, String sMessage)
  {
    if (iLevel <= verbosity)
      Output.println (iLevel + ": " + sModuleName + ":> " + sMessage);
  }

  public final void stackTrace (int iLevel, Throwable tThrow)
  {
    if (iLevel <= verbosity)
      tThrow.printStackTrace(Output);
  }

  public final void stackTrace (int iLevel)
  {
    stackTrace (iLevel, new Exception());
  }
}

