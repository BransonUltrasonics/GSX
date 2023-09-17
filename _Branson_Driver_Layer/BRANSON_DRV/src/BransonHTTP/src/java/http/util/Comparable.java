/*  Comparable.java */

/* Copyright 1999-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
02a,20oct03,dlr  Port to WM Web
01a,05feb97,ck     initial creation
*/

package http.util;

public interface Comparable
{
  public int compareTo (Comparable what) throws ComparableException;
}











