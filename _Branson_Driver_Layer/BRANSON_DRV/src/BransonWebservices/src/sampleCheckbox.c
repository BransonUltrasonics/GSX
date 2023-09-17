/* sampleCheckbox.c - sample implementation for tutorial */

/* Copyright (c) 2012, 2014 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
18feb16,h_x  Fix compiler and static analysis warnings
26sep14,r_w  fix static CHECKED_RETURN issue.
09aug12,r_w  create file
*/

/*
DESCRIPTION
This file contains the checkboxHandler associated with
the windMark myCheckbox.
*/


/* includes */
#include "wmw_httpconf.h"
#include "wm.h"

#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "wmb.h"

/* Setting initial values of example WindMark */
sbyte   myCheckbox[32] = "ON";


/*********************/
/* checkboxHandler */
/*********************/

STATUS checkboxHandler
    (
    WMB_TID_T  tid,     /* wmb transaction pointer */
    WMB_OBJ_T  obj,     /* wmb object      pointer */
    COMMAND_T  command  /* transaction type (e.g. GET,SET) */
    )
    {
    STATUS status = OK;
    size_t       written;
    
    /* loop through all objects in the group linked list */
    while (wmbObjectGood(obj))
        {

        (void)wmbObjectValueGet (obj);

        switch (command)
            {
            case CMD_GET_K:
                /* if current value is on, fill in buffer with CHECKED string */
                if (0 == (STRCMP (myCheckbox,"ON")))
                {
                     status = wmbObjectImport(obj,"CHECKED", 100, DATATYPE_STRING_K);
                }
                else
                {
                    status = wmbObjectImport(obj,"UNCHECKED", 100, DATATYPE_STRING_K);
                
                }
                break;

            case CMD_GET_NEXT_K:
                break;

            case CMD_TEST_K:
                /* backup data */

             /* Attach undo information to transaction */
                (void) wmbObjectSavedSet(obj,NULL,4,DATATYPE_INTEGER_K);
                break;

            case CMD_COMMIT_K:
                /* if reached here, checkbox is checked in cgi-stream, set myCheckbox to "value" which is "ON" */
                status = wmbObjectExport(obj, myCheckbox, 32, DATATYPE_STRING_K, &written);
                break;

            case CMD_UNDO_K:
                status = wmbObjectSavedGet(obj, wmbObjectCookieGet(obj), 256, DATATYPE_STRING_K);
                break;

            case CMD_COMPLETE_K:
                wmbObjectSavedFree(obj);
                break;
            default:
#ifdef _DEBUG
                printf("unknown command: %d\n", command);
#endif
                return ERROR;
            }


        obj = wmbObjectNextGet(obj);
        }

    return status;
}

/* end checkboxHandler */

/* eof sampleCheckbox.c */
#endif
