/* sampleDynamicTable.c - sample implementation for tutorial */

/* Copyright (c) 2012-2014 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
06nov14,r_w  fix defect V7MAN-130
26sep14,r_w  fix static CHECKED_RETURN issue.
05may14,r_w  fix static analysis issues(US35919)
28aug12,r_w  create file
*/

/*
DESCRIPTION
This file contains sample user code for manipulating the table instance fields
*/

#include <string.h>
#include "wmw_httpconf.h"
#include "wm.h"

#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "wmb.h"
#include "sampleDynamicTable.h"

unsigned char sample_a = 40;
unsigned char sample_b = 30;
unsigned char sample_c = 20;
unsigned char sample_d = 10;

/* FIRST INSTANCE WINDMARKS */
char sFirstInt[11] = "";
char sFirstPtr[19] = "";
char sFirstPtrInt[19] = "";
char sFirstStr[stringTableValMax*2] = "";
char sFirstString[stringTableValMax*2] = "";

ssize_t myIpAddress=0x04030201;

char     myIpMapping[4][myIPSubNameLenth]={"","","",""};

struct stringEntry *stringTblHead = NULL;

struct numberEntry numberTable[numberTableRowMax] =
{
	{	0, "zero",  &numberTable[1] },
	{	1, "one",   &numberTable[2] },
	{	2, "two",   &numberTable[3] },
	{	3, "three", &numberTable[4] },
	{	4, "four",  &numberTable[5] },
	{	5, "five",  NULL            }
};

struct numberEntry *numberTblHead = &numberTable[0];

extern STATUS errnoSet (int error);

/* Parse name */
STATUS StringTableParseName(char *sName, char *firstName,char *lastName)
{	
	char *sPtr;

	/* Skip white space */
	sPtr=sName;
	while ((' ' == *sPtr) || ('\t' == *sPtr))
		sPtr++;

	/* Find white space separator */
	while ((' ' != *sPtr) && ('\t' != *sPtr) && ('\0' != *sPtr))
	{
		*firstName++ = *sPtr;
		sPtr++;
	}
	*firstName = '\0';

	/* Skip white space */
	while ((' ' == *sPtr) || ('\t' == *sPtr))
		sPtr++;

	/* Find end of string (Allow Spaces in name) */
	while ('\0' != *sPtr)
	{
		*lastName++ = *sPtr;
		sPtr++;
	}
	*lastName = '\0';

	/* Trim end of last name */
	while (' ' == lastName[strlen(lastName)-1])
		lastName[strlen(lastName)-1] = '\0';

	return OK;

}

/* create key */
static STATUS StringTableNameToKey(char *sKey, char *firstName,char *lastName)
{	char *sPtr, *dPtr;

	sprintf(sKey,"%s,%s",lastName,firstName);

	/* Remove Spaces, make lower case */
	for (sPtr=sKey,dPtr=sKey; *sPtr ;sPtr++)
	{
		if (' ' != *sPtr)
			*dPtr++ = tolower(*sPtr);
	}
	*dPtr = '\0';

	return OK;
}

/* Add a name to the string table */
STATUS StringTableAddName(char *firstName,char *lastName)
{
    struct stringEntry *p_lstNewElement, *pThisPtr, *pPrevPtr;
    int    nKeyLength;

	nKeyLength = (int)(strlen(lastName) + 1 + strlen(firstName) + 1);
	nKeyLength = min(nKeyLength,stringTableValMax*2);

	if (2 == nKeyLength)
		return OK;

	/* create new record */
    p_lstNewElement = (struct stringEntry *) wmMalloc (sizeof(stringEntry));
    if (NULL == p_lstNewElement)
        return ERROR;
	(void) StringTableNameToKey(p_lstNewElement->key,firstName,lastName);
	strcpy( p_lstNewElement->firstName  ,firstName);
	strcpy( p_lstNewElement->lastName   ,lastName );
    p_lstNewElement->nextPtr    = NULL;

	/* Is this the first record ? */
	if (NULL == stringTblHead)
	{
		stringTblHead = p_lstNewElement;
		return OK;
	}

	/* Is this the new head of the list? */
	if (0 > strcmp(p_lstNewElement->key,stringTblHead->key))
	{
		p_lstNewElement->nextPtr = stringTblHead;
		stringTblHead            = p_lstNewElement;
		return OK;
	}

	/* Search the list for a insert location, or the end */
	pPrevPtr = stringTblHead;
	pThisPtr = stringTblHead->nextPtr;
	while ((NULL != pThisPtr) && (0 < strcmp(p_lstNewElement->key,pThisPtr->key)))
        {
		pPrevPtr = pThisPtr;
		pThisPtr = pThisPtr->nextPtr;
        }

	/* Insert into list at this location */
	p_lstNewElement->nextPtr = pThisPtr;
	pPrevPtr->nextPtr        = p_lstNewElement;

    return OK;
}

/* Remove a name from the string table */
STATUS StringTableDelName(char *firstName,char *lastName)
{
	struct stringEntry *thisPtr, *prevPtr;
    int    nKeyLength;
	char * delKey;

	nKeyLength = (int)(strlen(lastName) + 1 + strlen(firstName) + 1);
	nKeyLength = min(nKeyLength,stringTableValMax);

	if (2 == nKeyLength)
		return OK;

	if (NULL == stringTblHead)
		return ERROR;

	/* generate delete key */
    delKey = (char *) wmMalloc (nKeyLength);
    if (NULL == delKey)
        return ERROR;
	(void) StringTableNameToKey(delKey,firstName,lastName);

	/* Is it the first record? */
	if (0 == strcmp(delKey,stringTblHead->key))
	{
		thisPtr       = stringTblHead;
		stringTblHead = stringTblHead->nextPtr;
		wmFree(thisPtr);

		wmFree(delKey);
		return OK;
	}

	/* Find a matching record */
	prevPtr = stringTblHead;
	thisPtr = stringTblHead->nextPtr;
	while (NULL != thisPtr)
        {
		if (0 == strcmp(delKey,thisPtr->key))
			{
			prevPtr->nextPtr = thisPtr->nextPtr;
			wmFree(thisPtr);

			wmFree(delKey);
			return OK;
			}

		prevPtr = thisPtr;
		thisPtr = thisPtr->nextPtr;
        }

	wmFree(delKey);
	return ERROR;
}

/* Find a key in the string table */
STATUS StringTableFindbyKey
	(
    struct stringEntry   ** pPtrInstance,	/* the Pointer Index instance */
	char   				 *	sKey,
	BOOL 					bExact
	)
{
	struct stringEntry *thisPtr;

	/* Find a matching key */
	thisPtr = stringTblHead;
	while (NULL != thisPtr)
        {
		if (0 >= strcmp(sKey,thisPtr->key))
			{

			if (bExact && (0 != strcmp(sKey,thisPtr->key)))
				{
				*pPtrInstance = NULL;
				return ERROR;
				}

			*pPtrInstance = thisPtr;
			return OK;
			}

		thisPtr = thisPtr->nextPtr;
        }

	*pPtrInstance = NULL;
	return ERROR;
}

/* Initiate table  */
void StringTableInit(void)
{
	(void) StringTableAddName("Tony","Bennett");
	(void) StringTableAddName("Ella","Fitzgerald");
	(void) StringTableAddName("Frank","Sinatra");
	(void) StringTableAddName("Barbra","Streisan");
	(void) StringTableAddName("Nat","King Cole");
	(void) StringTableAddName("Ray","Charles");
}

/* remove table  */
void StringTableDelete(void)
{
	struct stringEntry *thisPtr, *nextPtr;

	thisPtr = stringTblHead;

	while (NULL != thisPtr)
		{
		nextPtr = thisPtr->nextPtr;
		free(thisPtr);
		thisPtr = nextPtr;
		}
}

/**************************************************************************
*
* usrObjectInstanceInt : Table indexed by integer offset
*
*
*/

STATUS usrObjectStrInstanceInt
    (
    WMB_TID_T   tid,     		/* wmb transaction pointer */
    WMB_OBJ_T   obj,    		/* the object */
    COMMAND_T   cmd,        	/* handler command */
    unsigned int *	pIntInstance,	/* the integer instance */
    size_t	 	minIntInstance,	/* the integer instance minimum */
    size_t	 	maxIntInstance	/* the integer instance maximum */
    )
    {
    WM_BUFFER_T *		pInstance;		/* current Instance structure */
    size_t        		Written;		/* bytes copied from instance */
    OS_MEM_SHARE_T *    pMem;			/* memory pool for allowcation */
    char				sInstance[21];
    BOOL				isNewInstance = FALSE;
    STATUS          	status = OK;

    pMem       	   = wmbTransactionSharedGet(tid);
    pInstance      = wmbObjectInstanceGet(obj);

    /* if there is no current instance, create one */
	if (( NULL == pInstance                                                                      ) ||
		( 0 == wmBufferUsedGet(pInstance)                                                        ) ||
		((1 == wmBufferUsedGet(pInstance)) && (DATATYPE_STRING_K == wmBufferLocalGet(pInstance)) ) )
		{
		/* Create a new instance, and copy in the initial value (0) */
		sprintf(sInstance,"%d",minIntInstance);
		pInstance   = wmBufferCreate(pMem, 2, sInstance, DATATYPE_STRING_K, TRUE);
		wmbObjectInstanceSet (obj, pInstance);
		isNewInstance = TRUE;
		}

	/* Get the instance integer */
	status = wmBufferExport  (pInstance, sInstance, 20, DATATYPE_STRING_K, &Written);
	(void)sscanf(sInstance,"%d",pIntInstance);

    switch (cmd)
    	{
		case CMD_GET_NEXT_K:

			/* Increment the instance */
			if (!isNewInstance)
				{
				(*pIntInstance)++;
				sprintf(sInstance,"%d",*pIntInstance);
				}

			/* Update the instance */
			status = wmBufferUpdate  (pInstance, sInstance, strlen(sInstance)+1, DATATYPE_STRING_K, TRUE);
			break;
        }

    if (*pIntInstance < minIntInstance)
    	{
		/* Fix the instance */
		sprintf(sInstance,"%d",minIntInstance);
		status = wmBufferUpdate  (pInstance, sInstance, strlen(sInstance)+1, DATATYPE_STRING_K, TRUE);
		(void)errnoSet(ERROR_GENERAL_NO_SUCH_INSTANCE);
		status = ERROR;
		}
    if (*pIntInstance > maxIntInstance)
    	{
		/* Fix the instance */
		sprintf(sInstance,"%d",maxIntInstance);
		status = wmBufferUpdate  (pInstance, sInstance, strlen(sInstance)+1, DATATYPE_STRING_K, TRUE);
		(void)errnoSet((CMD_GET_NEXT_K == cmd) ? ERROR_TABLE_END_OF_TABLE:ERROR_GENERAL_NO_SUCH_INSTANCE);
		status = ERROR;
		}

    return status;
    }


/**************************************************************************
*
* usrObjectInstancePtr : Sample user code for manipulating the table instance fields
*
*
*/

STATUS usrObjectStrInstancePtr
    (
    WMB_TID_T   tid,     		/* wmb transaction pointer */
    WMB_OBJ_T   obj,    		/* the object */
    COMMAND_T   cmd,        	/* handler command */
    void	**	pPtrInstance,	/* the Pointer instance */
    void	*	headInstance,	/* the integer instance minimum */
    size_t		offsetToPtr 	/* the byte offset into struct for the next_pointer */
    )
    {
    WM_BUFFER_T *		pInstance;		/* current Instance structure */
    size_t        		Written;		/* bytes copied from instance */
    OS_MEM_SHARE_T *    pMem;			/* memory pool for allowcation */
    char				sInstance[20];
    BOOL				isNewInstance = FALSE;
    BOOL				isFound;
    STATUS          	status = OK;
    void *				pRecPointer = NULL;
    void *				pTstPointer;

    /* Is it an empty list? */
    if (NULL == headInstance)
    	{
		(void)errnoSet(ERROR_GENERAL_NO_SUCH_INSTANCE);
    	return ERROR;
		}

    pMem       	   = wmbTransactionSharedGet(tid);
    pInstance      = wmbObjectInstanceGet(obj);

    /* if there is no current instance, create one */
	if (( NULL == pInstance                                                                      ) ||
		( 0 == wmBufferUsedGet(pInstance)                                                        ) ||
		((1 == wmBufferUsedGet(pInstance)) && (DATATYPE_STRING_K == wmBufferLocalGet(pInstance)) ) )
		{
		/* Create a new instance, and copy in the initial value (0) */
		sprintf(sInstance,"0x%lx",(unsigned long)headInstance);
		pInstance   = wmBufferCreate(pMem, 19, sInstance, DATATYPE_STRING_K, TRUE);
		wmbObjectInstanceSet (obj, pInstance);
		isNewInstance = TRUE;
		}

	/* Get the struct pointer from instance */
	status = wmBufferExport  (pInstance, sInstance, 20, DATATYPE_STRING_K, &Written);
	(void)sscanf(sInstance,"%lx",(unsigned long *)&pRecPointer);

	/* Confirm that this is valid pointer (don't let users give a bad instance and crash the target */
	for (pTstPointer = headInstance, isFound = FALSE; (NULL != pTstPointer); )
		{
		/* have we found the pointer? */
		if (pTstPointer == pRecPointer)
			{
			isFound = TRUE;
			break;
			}
		pTstPointer = ((struct numberEntry *)pTstPointer)->nextPtr;
		}

	if (!isFound)
		{
		*pPtrInstance = NULL;
		(void)errnoSet(ERROR_GENERAL_NO_SUCH_INSTANCE);
		return ERROR;
		}

    switch (cmd)
    	{
		case CMD_GET_K:
		case CMD_NVMSET_K:
		case CMD_TEST_K:
		case CMD_COMMIT_K:
		default:
			break;
		case CMD_GET_NEXT_K:

			/* Getch the next structure, using pointer arithmetic */
			if (!isNewInstance)
				{
				pRecPointer = ((struct numberEntry *)pRecPointer)->nextPtr;
				sprintf(sInstance,"0x%lx",(NULL == pRecPointer) ? 0x00:(unsigned long)pRecPointer);
				}

			/* Update the instance */
			status = wmBufferUpdate  (pInstance, sInstance, strlen(sInstance)+1, DATATYPE_STRING_K, TRUE);

			/* Have we gone past the end? */
			if (NULL == pRecPointer)
				{
				*pPtrInstance = NULL;
				(void)errnoSet(ERROR_TABLE_END_OF_TABLE);
				status = ERROR;
				}

			break;
        }

	*pPtrInstance = pRecPointer;

    return status;
    }

/**************************************************************************
*
* usrObjectInstancePtrInt : Sample user code for manipulating the table instance fields
*
*
*/

STATUS usrObjectStrInstancePtrInt
    (
    WMB_TID_T   tid,     		/* wmb transaction pointer */
    WMB_OBJ_T   obj,    		/* the object */
    COMMAND_T   cmd,        	/* handler command */
    void   ** 	pPtrInstance,	/* the Pointer Index instance */
    void	*	headInstance,	/* the integer instance minimum */
    size_t		offsetToPtr 	/* the byte offset into struct for the next_pointer */
    )
    {
    WM_BUFFER_T *		pInstance;		/* current Instance structure */
    size_t        		Written;		/* bytes copied from instance */
    OS_MEM_SHARE_T *    pMem;			/* memory pool for allowcation */
    char				sInstance[21];
    BOOL				isNewInstance = FALSE;
    STATUS          	status = OK;
    void *				pRecPointer = NULL;
    size_t				nIntInstance;
    size_t				i;

    /* Is it an empty list? */
    if (NULL == headInstance)
    	{
		(void)errnoSet(ERROR_GENERAL_NO_SUCH_INSTANCE);
    	return ERROR;
		}

    pMem       	   = wmbTransactionSharedGet(tid);
    pInstance      = wmbObjectInstanceGet(obj);

    /* if there is no current instance, create one */
	if (( NULL == pInstance                                                                      ) ||
		( 0 == wmBufferUsedGet(pInstance)                                                        ) ||
		((1 == wmBufferUsedGet(pInstance)) && (DATATYPE_STRING_K == wmBufferLocalGet(pInstance)) ) )
		{
		/* Create a new instance, and copy in the initial value (0) */
		sprintf(sInstance,"%ld",(long)0);
		pInstance   = wmBufferCreate(pMem, 19, sInstance, DATATYPE_STRING_K, TRUE);
		wmbObjectInstanceSet (obj, pInstance);
		isNewInstance = TRUE;
		}

	/* Get the pointer Index from instance */
	status = wmBufferExport  (pInstance, sInstance, 20, DATATYPE_STRING_K, &Written);
	(void)sscanf(sInstance,"%ld",(long *)&nIntInstance);

    /*  Find the record in question */
	pRecPointer = headInstance;
	for (i=0; (NULL != pRecPointer) && (i < nIntInstance); i++)
		{
		pRecPointer = ((struct numberEntry *)pRecPointer)->nextPtr;
		}

	/* Have we not found this pointer instance */
	if (NULL == pRecPointer)
		{
		*pPtrInstance = NULL;
		(void)errnoSet(ERROR_GENERAL_NO_SUCH_INSTANCE);
		return ERROR;
		}

    switch (cmd)
    	{
		case CMD_GET_K:
		case CMD_NVMSET_K:
		case CMD_TEST_K:
		case CMD_COMMIT_K:
		default:
			break;
		case CMD_GET_NEXT_K:

			/* Getch the next structure, using pointer arithmetic */
			if (!isNewInstance)
				{
				pRecPointer = ((struct numberEntry *)pRecPointer)->nextPtr;
				nIntInstance++;
				sprintf(sInstance,"%d",nIntInstance);
				}

			/* Update the instance */
			status = wmBufferUpdate  (pInstance, sInstance, strlen(sInstance)+1, DATATYPE_STRING_K, TRUE);

			/* Have we gone past the end? */
			if (NULL == pRecPointer)
				{
				*pPtrInstance = NULL;
				(void)errnoSet(ERROR_TABLE_END_OF_TABLE);
				status = ERROR;
				}

			break;
        }

	*pPtrInstance = pRecPointer;

    return status;
    }


/**************************************************************************
*
* usrObjectStrInstanceStrKey : Sample user code for manipulating the table instance fields
*
*
*/

STATUS usrObjectStrInstanceStrKey
    (
    WMB_TID_T   tid,     		/* wmb transaction pointer */
    WMB_OBJ_T   obj,    		/* the object */
    COMMAND_T   cmd,        	/* handler command */
    void   ** 	pPtrInstance,	/* the Pointer Index instance */
    void	*	headInstance,	/* the integer instance minimum */
    size_t		offsetToKey,	/* the byte offset into struct for the string key   */
    size_t		offsetToPtr 	/* the byte offset into struct for the next_pointer */
    )
    {
    WM_BUFFER_T *		pInstance;		/* current Instance structure */
    OS_MEM_SHARE_T *    pMem;			/* memory pool for allowcation */
    char *				sInstance;
    char *				sKey;
    BOOL				isNewInstance = FALSE;
    STATUS          	status = OK;
    void *				pRecPointer;
    size_t				i;

    /* Is it an empty list? */
    if (NULL == headInstance)
    	{
		(void)errnoSet(ERROR_GENERAL_NO_SUCH_INSTANCE);
    	return ERROR;
		}

    pMem      = wmbTransactionSharedGet(tid);
    pInstance = wmbObjectInstanceGet(obj);

    /* if there is no current instance, create one */
	if (wmbObjectInstanceIsEmpty(obj))
		{
		/* Create a new instance, and copy in the first key value */
		isNewInstance = TRUE;
		sInstance     = (((char *) headInstance) + offsetToKey);
		pInstance     = wmBufferCreate(pMem, strlen(sInstance)+1, sInstance, DATATYPE_STRING_K, TRUE);
		wmbObjectInstanceSet (obj, pInstance);
		}

	/* Get the pointer Index from instance */
	sInstance = wmBufferDataGet(pInstance);

	/*  Find EXACT (or NEXT Best, since this table is dynamic) */
	pRecPointer = headInstance;
	sKey        = (((char *) pRecPointer) + offsetToKey);
	for (i=0; (NULL != pRecPointer) && (NULL != sInstance) && (0 < strcmp(sInstance,sKey)); i++)
		{
		pRecPointer = ((struct stringEntry *)pRecPointer)->nextPtr;
		/* Get it key */
		sKey        = (((char *) pRecPointer) + offsetToKey);
		}

	/* Have we not found this pointer instance */
	if (NULL == pRecPointer)
		{
		*pPtrInstance = NULL;
		(void)errnoSet(ERROR_GENERAL_NO_SUCH_INSTANCE);
		return ERROR;
		}

	/* if GET and it was not an EXACT match, then ERROR */
	if ((CMD_GET_K == cmd           ) &&
	    (0 != strcmp(sInstance,sKey)) )
		{
		*pPtrInstance = NULL;
		(void)errnoSet(ERROR_GENERAL_NO_SUCH_INSTANCE);
		return ERROR;
		}

	/* if GET_NEXT and it was EXACT match, get the next */
	if ((CMD_GET_NEXT_K == cmd      ) &&
	    (0 == strcmp(sInstance,sKey)) &&
	    (!isNewInstance             ) )
		{
		pRecPointer = ((struct stringEntry *)pRecPointer)->nextPtr;
		if (NULL == pRecPointer)
			{
			status = wmBufferUpdate(pInstance, sKey, strlen(sKey)+1, DATATYPE_STRING_K, TRUE);
			*pPtrInstance = NULL;
			(void)errnoSet(ERROR_GENERAL_NO_SUCH_INSTANCE);
			return ERROR;
			}
		}

	sKey   = (((char *) pRecPointer) + offsetToKey);
	status = wmBufferUpdate(pInstance, sKey, strlen(sKey)+1, DATATYPE_STRING_K, TRUE);

	*pPtrInstance = pRecPointer;

    return status;
    }
#endif

