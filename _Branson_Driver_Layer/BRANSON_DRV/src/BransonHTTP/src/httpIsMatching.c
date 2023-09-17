/* httpIsMatching.c - String pattern matching routines for the http server */

/* Copyright 1999 - 2004,2010 Wind River Systems, Inc. */

/* Project: Wind Web Server, Version 2.0
 *
 * Copyright (c) 1996/97, 3Soft GmbH
 *
 * 3Soft GmbH, Wetterkreuz 19a
 * D-91058 Erlangen, Germany
 * infohttp@dreisoft.de, http://www.dreisoft.de
 */

/*
modification history
--------------------
02h,2feb10,h_y   support for LP64
02g,12jan04,jws  make independent of httpLib.h
02f,18aug03,jws  remove HTTP_BOOL
02e,01may03,jws  some reformating and reource leak fix.
02d,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02c,11jun97,ck   bug fix "abc*" no longer matches "xxabcyy".
02b,14may97,mb   osal introduced.
02a,30sep96,ck   splitt off from httputil.
01d,02sep96,mb   minor bugs fixed.
01c,25jul96,ck   significantChars added.
01b,17jul96,pk   documented, beautified.
01a,25jun96,pk   written.
*/

/*
DESCRIPTION
String pattern-matching utilities for the HTTP server. For example, to match
URLs to patterns.

INCLUDE FILES:
	httpType.h
	httpLib.h

*/


/* includes */

#include "ctype.h"
#include <string.h>
#include <stdio.h>
#include "stdlib.h"

#include "wm.h"

/* this prototype is temporary pending re-org of headers */

char * httpStrdup(MEM_POOL_ID,const char * const);
BOOL httpIsMatching
    (
    MEM_POOL_ID   memPartID,      /* where to allocate temporarily buffer */
    const char * const szPattern, /* pattern to search */
    const char * const szExamenee /* string to be matched to pattern */
    );
long httpSignificantChars
    (
    const char * const szPattern
    );
/* #include "httpType.h" */
/* #include "httpLib.h"  */


/*******************************************************************************
*
* httpIsMatching - Compares string <szExamenee> to pattern <szPattern>.
*
* This routine performs simple pattern matching.  The only meta-character
* allowed is the asterisk wildcard, which matches any characters.
*
* EXAMPLES:
* The following returns TRUE:
* \cs
*     httpIsMatching("abc","abc")
* \ce
* The following returns FALSE:
* \cs
*     httpIsMatching("abc","ab")
* \ce
* The following returns TRUE:
* \cs
*     httpIsMatching("ab*","abxyz")
* \ce
* The following returns TRUE:
* \cs
*     httpIsMatching("ab*xyz","abcxyz")
* \ce
* The following returns TRUE:
* \cs
*     httpIsMatching("ab*x","abcxdx")
* \ce
*
* RETURNS: TRUE if pattern is matched, FALSE if not.
*/

BOOL httpIsMatching
    (
    MEM_POOL_ID   memPartID,      /* where to allocate temporarily buffer */
    const char * const szPattern, /* pattern to search */
    const char * const szExamenee /* string to be matched to pattern */
    )
    {
    char *    pchAsterisk;       /* pointer to asterisks in szTmpPattern */
    size_t    sizeExamenee;      /* length of remainder of szTmpExamenee  */
    size_t    sizePattern;       /* length of remainder of szTmpPattern   */
    char *    szTmpPattern;
    char *    szTmpExamenee;
    char *    szOrgPattern;
    char *    szOrgExamenee;
    BOOL      fAfterAsterisk = FALSE;
    BOOL      fReturnVal = FALSE;

    if (szPattern == NULL)  return FALSE;

    /* A null string is matched only by "*". */

    if ( ( szExamenee == NULL ) || strlen (szExamenee) == 0 )
        return  ((strcmp(szPattern,"*") == 0 ) ? TRUE : FALSE );



    if ((szOrgPattern  = httpStrdup (memPartID, szPattern)) == NULL)
        return FALSE;

	if ((szOrgExamenee = httpStrdup (memPartID, szExamenee)) == NULL)
	    {
	    (void)memPoolFree (memPartID, szOrgPattern);
        return FALSE;
        }

    szTmpPattern  = szOrgPattern;
    szTmpExamenee = szOrgExamenee;

    pchAsterisk = strchr(szTmpPattern, '*');

    if ( pchAsterisk != NULL )  /* There is at least one asterisk */
        {

        do       /* loop for each asterisk */
	        {

	        *pchAsterisk = (char)0;

	        if (!fAfterAsterisk)
	            {

		        if (strncmp(szTmpExamenee, szTmpPattern, strlen(szTmpPattern))
		            == 0)
		            {
		            fAfterAsterisk = TRUE;
		            }
		        else
		            {
		            *pchAsterisk = '*';          /* restore asterisk            */
		            goto clean_exit;
		            }
		        }
	        else     /* otherwise, only a substringmatch must be performed */
		        {
		        /* Look for substring in rest of examinee. */

		        szTmpExamenee = strstr( szTmpExamenee, szTmpPattern );

		        *pchAsterisk = '*';                 /* Restore asterisk      */
		        if (szTmpExamenee == NULL)          /* Substring found?      */
		            goto clean_exit;                /* If not -> no match.   */
		        szTmpExamenee += pchAsterisk - szTmpPattern; /* Advance examenee ptr. */
		        szTmpPattern = pchAsterisk + 1;           /* Go on behind asterisk */
		        if ( *szTmpPattern == (char)0 )           /* Closing asterisk      */
		            {
		            fReturnVal = TRUE;                  /* matches everything.   */
		            goto clean_exit;
		            }
		        pchAsterisk = strchr(szTmpPattern, '*');  /* Search next asterisk  */
		        }
            } while (pchAsterisk != NULL);

        /* No more asterisks */

        sizeExamenee = strlen( szTmpExamenee );
        sizePattern  = strlen( szTmpPattern );

        if ( sizePattern > sizeExamenee )  goto clean_exit;

        szTmpExamenee += sizeExamenee - sizePattern;
        }

    /* The rest of the string must be an exact match. */

    fReturnVal = (strcmp( szTmpPattern, szTmpExamenee ) == 0)
                     ?  TRUE : FALSE;

clean_exit:

    (void)memPoolFree (memPartID, szOrgPattern);
    (void)memPoolFree (memPartID, szOrgExamenee);
    return (fReturnVal);
    }


/****************************************************************************
*
* httpSignificantChars - Number of non-meta chars in a pattern string.
*
* Returns the number of characters significant for pattern matching
* in the given string (ie. the number of non-wildcard characters).
* This routine is useful in conjunction with httpIsMatching() to
* identify the "longest matching string."
*
* RETURNS: The number of non-meta-characters in a pattern string.
*/

long httpSignificantChars
    (
    const char * const szPattern
    )
    {
    long i;
    long lLength;
    long lSignificant;

    lLength = (long) strlen (szPattern);
    lSignificant = lLength;
    for (i=0; i<lLength; i++)
        if (szPattern[i] == '*')
            lSignificant --;

    return lSignificant;
    }

