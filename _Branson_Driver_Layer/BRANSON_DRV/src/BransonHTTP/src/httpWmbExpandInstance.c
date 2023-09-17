/* httpWmbExpandInstance.c - expand Wind Manage Instances */

/* Copyright 2001-2004,2010-2011 Wind River Systems, Inc. */

/*
modification history
-------------------- 
02c,03mar11,r_w  fix defect WIND00255073 and WIND00255074
02b,28jul10,m_z   eliminate compile warning
02a,02feb10,h_y   support for LP64 
01f,03jun04,gsr  fixed memory leak
01e,10may04,gsr  fixed garbage instance value for recursive braces
01d,05may04,dkg  Modified httpParseInlineExpression() and httpInsertMarkup() for
                 Complex Test Cases of inline instancing
01c,03may04,dkg  Modified data type of input argument in httpParseInlineExpression()
                 and httpProcessMarkup() to fix SPR 96709
01b,27apr04,adb  replaced all buffer size constants with kMagicMarkupBufferSize
01a,16jan04,gsr  Created for SPR#91008
*/

/*
DESCRIPTION
This module implements the expansion of wind manage wind instances

INCLUDE FILES: wm.h wmb.h
*/


/* includes */
#include "httpWmbExpandInstance.h"
#include "httpCfg.h"

BOOL ISALPHANUMERIC
    (
    sbyte4 c
    )
    {
    return (    (('a' <= c) && (c <= 'z')) ||
                (('A' <= c) && (c <= 'Z')) ||
                (('0' <= c) && (c <= '9')) ||
                ('_' == c)                      ) ? TRUE : FALSE;

    } /*from rc_rlstdlib.c*/

#ifndef WEBCLI_STANDALONE_WEBSERVER
/*******************************************************************************
*
* httpExpandInstance - the main expansion routine
*
* This routine is called from httpWmbParser in markParser()
*
* This module validates for the presence of Inline delimeter invokes routines to
* expand the instances and returns the instance string in the supplied buffer
*
* RETURNS: NONE
*/


void httpExpandInstance
     (
     WMB_SESSION_T *   pSession,
     char *pInstanceString,
     char *pRetInstance
     )
     {
     *pRetInstance = '\0';

    /* build up an expanded instance in a iterative process, allows greatest amount of flexibility */
    while ('\0' != *pInstanceString)
    {
        if (ISDIGIT(*pInstanceString))
          {
            while (ISDIGIT(*pInstanceString))
            {
                *pRetInstance = *pInstanceString;

                pRetInstance++;
                pInstanceString++;
            }

            *pRetInstance = '\0';
        }
        else if (kInlineStartDelim == *pInstanceString)   /* process inline instance */
        {
            sbyte*  pNextComponent = STRCHR(pInstanceString + 1, kInlineEndDelim);
            sbyte   OutputBuf[kMagicMarkupBufferSize + 1];

            if (NULL == pNextComponent)
                return;                   /* bad instance string */


           if (NULL !=  STRCHR (pInstanceString, kInlineExpStartDelim))
              httpParseInlineExpression(pSession,pInstanceString + 1,OutputBuf, kMagicMarkupBufferSize,
                                        kInlineExpStartDelim, kInlineExpEndDelim);

           else if (NULL != STRCHR (pInstanceString, kInlineExpStartBrace))
	          httpParseInlineExpression(pSession,pInstanceString + 1,OutputBuf, kMagicMarkupBufferSize,
                                        kInlineExpStartBrace, kInlineExpEndBrace);
          else
               return;


            if ('\0' == OutputBuf[0])
                return;                   /* bad instance string */

            STRCPY(pRetInstance, OutputBuf);
            pRetInstance += STRLEN(OutputBuf);

            pInstanceString = pNextComponent + 1;   /* go beyond closing hash mark */

        }
        else
        {

	     return;

        }
    }       /* while not at the end of string */

} /* expandInstance */



/*******************************************************************************
*
* httpParseInlineExpression - the parse routine for inline instance
*
* This module is the parser for the inline instances. This routine does a sanity
* check and extracts the windmarks using the delimeters. This routine is responsible
* to completely resolve the inline instance and return the value for the upper layer
*
* RETURNS: NONE
*/

void httpParseInlineExpression
    (
    WMB_SESSION_T *   pSession,
    char *pinlineExpString,
    char *pOutputString,
    sbyte4 OutputLen,
    sbyte InlineExpStartDelim,
    sbyte InlineExpEndDelim
    )
    {
    sbyte4 openBrackets = 0, closedBrackets = 0;
    sbyte  *pStart = NULL, *pEnd = NULL, *pChar = NULL;
    sbyte  *pOBracket = NULL, *pCBracket = NULL;
    sbyte  *pExpressionResult = NULL, *pCopiedBuffer = NULL;
    STATUS status = ERROR_GENERAL;
    char   windmark[WMB_NAME_LEN + 1];

    pExpressionResult = wmMalloc (kMaxExpResultLen + 1);
    if (NULL == pExpressionResult)
    {
    	*pOutputString = '\0';
      return;
    }
    
    pCopiedBuffer = wmMalloc (kMaxExpResultLen + 1);
    if (NULL == pCopiedBuffer)
    {
    	wmFree(pExpressionResult);
    	*pOutputString = '\0';
      return;    	
    }

    STRCPY(pCopiedBuffer, pinlineExpString);
    pStart = pCopiedBuffer;

        /* find the end of the embedded markup */
        if ((NULL == (pEnd = STRCHR(pStart, kInlineEndDelim)))     ||
            (NULL ==         STRCHR(pStart, InlineExpStartDelim))  ||
            (NULL ==         STRCHR(pStart, InlineExpEndDelim)))
        {
            goto parse_error;
        }

        *pEnd = '\0';

        /* determine how many levels of nesting there are */
        pChar = pStart;
        while (NULL != (pChar = STRCHR(pChar, InlineExpStartDelim)))
        {
            pOBracket = pChar;
            openBrackets++;
            pChar++;
        }

        pChar = pStart;
        while (NULL != (pChar = STRCHR(pChar, InlineExpEndDelim)))
        {
            if (NULL == pCBracket)
                pCBracket = pChar;
            closedBrackets++;
            pChar++;
        }

        /* simple consistency test */
        if (openBrackets != closedBrackets)
            goto parse_error;

        /* Parsing starts now!! */
        /**** parse embedded markups from the inner most ****/
        /**** to the outer most ****/

        do
        {
           pChar = pStart;
           pOBracket = NULL;

           while (NULL != (pChar = STRCHR(pChar, InlineExpStartDelim)))
		       {
           pOBracket = pChar;
	         pChar++;
	         }

           if (NULL == pOBracket)
              break;

          pCBracket = STRCHR(pOBracket, InlineExpEndDelim);
          if (NULL == pCBracket)
             break;

           /* below is to get the instances */
           windmark[0] = '\0';
           STRNCPY(windmark, pOBracket + 1, (size_t)(pCBracket - pOBracket));
           windmark[(pCBracket - pOBracket) - 1] = '\0';

          httpProcessMarkup (pSession,windmark, pOutputString,OutputLen);

          status = httpInsertMarkup (&pStart,pOBracket,pCBracket,pOutputString,(sbyte)STRLEN(pOutputString));
          if (status != OK)
          	break;
        }while (1);

#ifdef __DEBUG_OCSNMP__
    printf ("Parse Completed\n");
#endif

    if (NULL != pStart)
    {
	    STRCPY (pOutputString,pStart);
      wmFree(pStart);
		}
	  else
		{
		*pOutputString = '\0';
		}

    if (NULL != pExpressionResult)
        wmFree(pExpressionResult);

    return;

parse_error:

    if (NULL != pExpressionResult)
        wmFree(pExpressionResult);

    if (NULL != pStart)
    	  wmFree(pStart);
		
    *pOutputString = '\0';

return;
} /* httpParseInlineExpression */


/*******************************************************************************
*
* httpProcessMarkup -  routine to extract the windmark value
*
* This routine separates windmark and its instance and resolves the windmark value
* for using wmbDataStringGet.
*
* RETURNS: OK if success
*/

STATUS httpProcessMarkup
    (
    WMB_SESSION_T *   pSession,
    sbyte *pReadObjectString,
    sbyte *pOutputBuf,
    sbyte4 OutputLen
    )
    {
    sbyte*   pStartOfInstance = NULL;
    sbyte*   pInstance = NULL;
    sbyte    instanceLen;
    STATUS status = ERROR_GENERAL;
    sbyte*   pBaseComponent = wmMalloc (kMaxMibLength+1);
    sbyte*   pInstanceComponents = wmMalloc (kMaxMibLength+1);

    if ((NULL == pBaseComponent) || (NULL == pInstanceComponents))
        goto process_error;

    pStartOfInstance = pReadObjectString;

    while (ISALPHANUMERIC(*pStartOfInstance))
          pStartOfInstance++;

    STRNCPY(pBaseComponent, pReadObjectString, (size_t)((long)pStartOfInstance - (long)pReadObjectString));
    pBaseComponent[(long)pStartOfInstance - (long)pReadObjectString] = '\0';

    STRCPY(pInstanceComponents, pStartOfInstance);

    if (*pInstanceComponents == 0)
        pInstance = NULL;
    else
        pInstance = pInstanceComponents + 1;

    instanceLen = (sbyte)((pInstance != NULL)? STRLEN(pInstanceComponents):0);

    status = wmbDataStringGet (pSession, pBaseComponent, pInstance, instanceLen,
                                   pOutputBuf,OutputLen, NULL);

process_error:

    if (NULL != pBaseComponent)
        wmFree(pBaseComponent);

    if (NULL != pInstanceComponents)
        wmFree(pInstanceComponents);

    return status;

} /* httpProcessMarkup */
#endif

/*******************************************************************************
*
* httpInsertMarkup -  routine to insert the windmark value in the inline instance
*
* This routine inserts the windmark value at the appropriate location in the
* supplied inline instance string.
*
* This routine changes the supplied pStart pointer to allocate a new pointer
* with the windmark value inserted
*
* RETURNS: NONE
*/

STATUS httpInsertMarkup
    (
    sbyte **pStart,
    sbyte *pOBracket,
    sbyte *pCBracket,
    sbyte *outputBuf,
    sbyte outputLen
    )
    {
    sbyte *pnewStart = NULL;
    pnewStart = (sbyte *)wmMalloc (100); /*(STRLEN(pStart) + outputLen - (pCBracket - pOBracket)) + 1); fix*/
    if (NULL == pnewStart)
    	return ERROR_GENERAL;
    bzero(pnewStart,0);

    STRNCPY(pnewStart,*pStart,(size_t)(pOBracket - *pStart));
    pnewStart[(pOBracket - *pStart)] = '\0';
    STRNCAT(pnewStart,outputBuf,(size_t)outputLen);
    STRNCAT(pnewStart,pCBracket+1,STRLEN(pCBracket)-1);

    wmFree(*pStart);
    *pStart = pnewStart;
    return OK;
   } /* httpInsertMarkup */
