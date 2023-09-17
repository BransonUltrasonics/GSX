/* httpShow.c - HTTP Lists Show Routines */

/* Copyright 2003,2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,26jan10,h_y  support for LP64
01b,07nov03,adb  WRS coding conventions modifications
01a,29oct03,adb  created
*/

/*
DESCRIPTION

The implementation file of a utility show routine used to depict the contents 
of any HTTP_GEN_LIST especially designed to ease the illustration of the HTTP 
configuration lists when called within the body of the various http<*>Show () 
routines that are implemented within the appropriate source files.

INCLUDE FILES: httpShow.h
*/

/* includes */

#include "httpShow.h"

#ifdef HTTP_SHOW
/***************************************************************************
*
* httpListShow - shows HTTP Generic List contents
*
* This is a utility routine called by various HTTP List Show routines.
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpListShow 
    (
    HTTP_GEN_LIST_ID listId,         /* pointer to list */
    char *           pListName,      /* pointer to display string */
    BOOL             idIsaPointer,   /* show entry id as pointer or string */
    BOOL             dataIsaPointer, /* show entry data as pointer or string */
    char *           pLeftMargin     /* pointer to left margin display string */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID  entryId = NULL;
    char                    aux =     '\0'; /* the empty string */

    /* replace NULL input display string pointers with empty string pointer */

    if  (NULL == pListName)
        {
        pListName = &aux;
        }

    if  (NULL == pLeftMargin)
        {
        pLeftMargin = &aux;
        }

    /* we will always print header even if the input is invalid */

    printf ("%s", pLeftMargin);
    printf ("httpListShow: p = %p, name = %s\n", listId, pListName);

    printf ("%s", pLeftMargin);
    printf ("%4s------\n", "");

    /* test whether the input list pointer is valid */

    if  (NULL == listId)
        {
        printf ("%s", pLeftMargin);
        printf ("%4slist is empty\n", "");

        printf ("%s", pLeftMargin);
        printf ("%4s------\n", "");

        return;
        }

    /* main display loop */

    while (NULL != httpGenListIterator (listId, &entryId))
        {
        printf ("%s", pLeftMargin);
        printf ("%4spEntry = %p\n", "", entryId);

        printf ("%s", pLeftMargin);
        if  (idIsaPointer)
            {
            printf ("%8sid = %p\n", "", httpGenListIdGet (entryId));
            }
        else
            {
            printf ("%8sid = %s\n", "", httpGenListIdGet (entryId));
            }

        printf ("%s", pLeftMargin);
        if  (dataIsaPointer)
            {
            printf ("%6sdata = %p\n", "", httpGenListDataGet (entryId));
            }
        else
            {
            printf ("%6sdata = %s\n", "", (char *)(httpGenListDataGet (entryId)));
            }

        printf ("%s", pLeftMargin);
        printf ("%4s------\n", "");
        }

    return;
    
    } /* httpListShow */
#endif /* HTTP_SHOW */
