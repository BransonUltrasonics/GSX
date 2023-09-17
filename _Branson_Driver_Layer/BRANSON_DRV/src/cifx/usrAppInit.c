/* usrAppInit.c - stub application initialization routine */

/* Copyright 1984-1998 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,02jun98,ms   written
*/

/*
DESCRIPTION
Initialize user application code.
*/

#include <vxworks.h>

/* Forward declarations */
extern STATUS registerCIFXDrv (BOOL                        fSingleDir,         /*!< !=0 will use a single firmware directory */
                               unsigned long               ulTraceLevel,       /*!< Trace level at start-up */
                               unsigned long               ulPollInterval,     /*!< Interval to poll for COS Flag changes */
                               char*                       szDriverBaseDir);   /*!< Base device directory to search for settings/configuration/firmware files */

/******************************************************************************
*
* usrAppInit - initialize the users application
*/

int main(int argc, char* argv[])
{
    /* add application specific code here */

    registerCIFXDrv ( FALSE,      /* fSingleDir,*/           /* !=0 will use a single firmware directory */
                      0x9,        /* ulTraceLevel,*/         /* Trace level at start-up */
                      500,        /* ulPollInterval,*/       /* Interval to poll for COS Flag changes */
                      "/bd0/cifX" /*szDriverBaseDir*/);      /* Base device directory to search for settings/configuration/firmware files */

  return 0;
}


