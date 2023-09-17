/* sampleDynamicTable.h - sample implementation for tutorial */

/* Copyright (c) 2012,2014 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
06nov14,r_w  fix defect V7MAN-130
28aug12,r_w  create file
*/

/*
DESCRIPTION
This file contains sample user code for manipulating the table instance fields
*/

#ifndef __SAMPLE_DYNAMIC_TABLE_HEADER__
#define __SAMPLE_DYNAMIC_TABLE_HEADER__

#ifdef __cplusplus
extern "C" {
#endif

/* STATIC NUMBER SAMPLE TABLE */
#define numberTableRowMax 6
#define numberTableValMax 20

/* DYNAMIC STRING SAMPLE TABLE */
#define stringTableRowMax 6
#define stringTableValMax 20

#define myIPSubNameLenth 20

/* Local Predefines */ 

 extern unsigned char sample_a;
 extern unsigned char sample_b;
 extern unsigned char sample_c;
 extern unsigned char sample_d;

/* FIRST INSTANCE WINDMARKS */
 extern char sFirstInt[11];
 extern char sFirstPtr[19];
 extern char sFirstPtrInt[19];
 extern char sFirstStr[stringTableValMax*2];
 extern char sFirstString[stringTableValMax*2];

 extern ssize_t myIpAddress;

 extern char  myIpMapping[4][myIPSubNameLenth];


struct stringEntry
{
	char	key[      stringTableValMax*2];	/* Value for entry */
	char	firstName[stringTableValMax+1];	/* Value for entry */
	char	lastName[ stringTableValMax+1];	/* Value for entry */
	struct 	stringEntry *nextPtr;			/* Sort order linked list pointer */
} stringEntry;

struct numberEntry
{
	int		offset;						/* absolute offset of entry into table */
	char	value[numberTableValMax];	/* Value for entry */
	struct 	numberEntry *nextPtr;		/* Sort order linked list pointer */
} numberEntry;

extern struct stringEntry *stringTblHead;
extern struct numberEntry numberTable[numberTableRowMax];
extern struct numberEntry *numberTblHead;

STATUS usrObjectStrInstanceInt
    (
    WMB_TID_T   tid,     		/* wmb transaction pointer */
    WMB_OBJ_T   obj,    		/* the object */
    COMMAND_T   cmd,        	/* handler command */
    unsigned int *	pIntInstance,	/* the integer instance */
    size_t	 	minIntInstance,	/* the integer instance minimum */
    size_t	 	maxIntInstance	/* the integer instance maximum */
    );

STATUS usrObjectStrInstancePtr
    (
    WMB_TID_T   tid,     		/* wmb transaction pointer */
    WMB_OBJ_T   obj,    		/* the object */
    COMMAND_T   cmd,        	/* handler command */
    void	**	pPtrInstance,	/* the Pointer instance */
    void	*	headInstance,	/* the integer instance minimum */
    size_t		offsetToPtr 	/* the byte offset into struct for the next_pointer */
    );

STATUS usrObjectStrInstancePtrInt
    (
    WMB_TID_T   tid,     		/* wmb transaction pointer */
    WMB_OBJ_T   obj,    		/* the object */
    COMMAND_T   cmd,        	/* handler command */
    void   ** 	pPtrInstance,	/* the Pointer Index instance */
    void	*	headInstance,	/* the integer instance minimum */
    size_t		offsetToPtr 	/* the byte offset into struct for the next_pointer */
    );

STATUS usrObjectStrInstanceStrKey
    (
    WMB_TID_T   tid,     		/* wmb transaction pointer */
    WMB_OBJ_T   obj,    		/* the object */
    COMMAND_T   cmd,        	/* handler command */
    void   ** 	pPtrInstance,	/* the Pointer Index instance */
    void	*	headInstance,	/* the integer instance minimum */
    size_t		offsetToKey,	/* the byte offset into struct for the string key   */
    size_t		offsetToPtr 	/* the byte offset into struct for the next_pointer */
    );


extern void StringTableInit(void);
extern STATUS StringTableFindbyKey(struct stringEntry   ** pPtrInstance, char  *sKey,	BOOL bExact);
extern STATUS StringTableParseName(char *sName, char *firstName,char *lastName);
extern STATUS StringTableAddName(char *firstName,char *lastName);
extern STATUS StringTableDelName(char *firstName,char *lastName);
extern void StringTableDelete(void);

#ifdef __cplusplus
}
#endif

#endif /* __SAMPLE_DYNAMIC_TABLE_HEADER__*/

