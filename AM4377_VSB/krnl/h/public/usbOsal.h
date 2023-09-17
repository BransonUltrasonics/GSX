/* usbOsal.h - USB OS Abstraction Layer Definitions */

/*
 * Copyright (c) 2003-2004, 2007, 2009-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2003-2004, 2007, 2009-2015 Wind River Systems, Inc.

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according
   to the terms of their Wind River license agreement(s) applicable to
   this software.
*/

/*
Modification history
--------------------
06feb15,whu  updated to support 64-bit (US48787)
20aug14,bbz  cleanup warnings of no check of the return value (US44214)
16apr14,j_x  Add O/U/XHCI vxBus GEN2 support (US22049 US22048 US25159)
04nov13,s_z  make USB work both on new VxBus model and VxBus legacy model
03sep13,wyy  Add macro OS_BUSY_WAIT_US and OS_BUSY_WAIT_MS to busy wait
             when rebooting (WIND00432482)
03sep13,x_z  fixed hwif header file path.
19jul12,w_x  Add support for USB 3.0 host (WIND00188662)
10aug11,ljg  Increase the max number of HCI Controllers (WIND00286762)
01aug11,ghs  Fix VxWorks version check macro (WIND00287708)
07sep10,ghs  Add MHCD max count.
02sep10,ghs  Use OS_THREAD_FAILURE to check taskSpawn failure (WIND00229830)
22jun10,ghs  Change macro OS_CREATE_THREAD(WIND00218930)
18may10,w_x  USB transition to vxbDmaBufLib supporting 64 bit (WIND00193267)
20mar10,y_l  Changed for defect (WIND00196728)
15mar10,j_x  Changed for USB debug (WIND00184542)
13jan10,ghs  vxWorks 6.9 LP64 adapting
03dec09,m_y  Move the max synopsys hcd define into this file.
11sep09,ghs  Add global max hcd defines(WIND00152418)
29jun09,ghs  Add USB_VXB_VERSIONID for usb (WIND00172037)
18mar09,j_x  Fix timing bug in OS_DELAY_MS()(WIND00155914)
10jul07,jrp  Removing OS_ENTER_CRITICAL_SECTION
26oct04,ami  Debug Messages Changes
06oct04,ami  SPR #94684 Fix
02jun04,hch  remove duplicate definition of TRUE and FALSE macros
09may04,dat  add parens around TRUE and FALSE.
17mar03,ssh  written
*/

/*
DESCRIPTION

This file contains the definitions of the OS Abstraction Layer for the USB2.0
Host Stack core code
*/

/*
INTERNAL
 *******************************************************************************
 * Filename         : OSAL.h
 *
 * Copyright        :
 *
 * THE COPYRIGHT IN THE CONTENTS OF THIS SOFTWARE VEST WITH WIPRO
 * LIMITED A COMPANY INCORPORATED UNDER THE LAWS OF INDIA AND HAVING
 * ITS REGISTERED OFFICE AT DODDAKANNELLI SARJAPUR ROAD  BANGALORE
 * 560 035. DISTRIBUTION OR COPYING OF THIS SOFTWARE BY
 * ANY INDIVIDUAL OR ENTITY OTHER THAN THE ADDRESSEE IS STRICTLY
 * PROHIBITED AND MAY INCUR LEGAL LIABILITY. IF YOU ARE NOT THE
 * ADDRESSEE PLEASE NOTIFY US IMMEDIATELY BY PHONE OR BY RETURN EMAIL.
 * THE ADDRESSEE IS ADVISED TO MAINTAIN THE PROPRIETARY INTERESTS OF
 * THIS COPYRIGHT AS PER APPLICABLE LAWS.
 *
 *
 *
 * Description      : OS Abstraction layer (OSAL) - for VxWorks 5.4.
 *
 ******************************************************************************/

#ifndef __USBOSAL_H__
#define __USBOSAL_H__

#include <vxWorks.h>
#include <logLib.h>     /* For error logging */
#include <assert.h>
#include <semLib.h>     /* For semaphores */
#include <taskLib.h>    /* For task */
#include <string.h>     /* For memset(), memcpy() */
#include <intLib.h>     /* For intLock() */
#include <sysLib.h>     /* For sysClkRateGet(), etc */
#include <stdio.h>      /* For printf() and scanf() support */
#include <wdLib.h>      /* For watch dog timers */
#include <math.h>       /* For using floor() */
#include <vxAtomicLib.h>    /* Atomic increment/decrement */
#include "usbPlatform.h"
#include "ossLib.h"            /* os specific services */
#include "usbQueueLib.h"
#include "usbListLib.h"

#ifdef _WRS_CONFIG_VXBUS_LEGACY
#include <vxbTimerLib.h>
#else
#include <subsys/timer/vxbTimerLib.h>
#endif

#include "usbOsalDebug.h" /* For module level message logging support */

/* To define a structure with the packed attribute */
#define OS_STRUCT_PACKED    __attribute__((__packed__))

/* Define usb vxBus version number */

#ifndef VXB_VER_4_0_0
#define USB_VXB_VERSIONID   VXBUS_VERSION_3
#else
#define USB_VXB_VERSIONID   VXB_VER_5_0_0
#endif

#define USB_MAX_HOSTS_PER_HCD   20  /* max number of host Controllers per HCD */
#define USB_MAX_UHCI_COUNT      20  /* max number of UHCI Controllers */
#define USB_MAX_OHCI_COUNT      20  /* max number of OHCI Controllers */
#define USB_MAX_EHCI_COUNT      20  /* max number of EHCI Controllers */
#define USB_MAX_SYNOPSYSHCI_COUNT   5     /* max number of Synopsys HCI
                                           * Controllers
                                           */
#define USB_MAX_MHCI_COUNT      5  /* max number of MHCI Controllers */

/* Just in case anyone else has redefined BOOLEAN */
#ifdef BOOLEAN
#undef BOOLEAN
#endif

/* To hold a boolean variable */
typedef UINT8   BOOLEAN;

/* To hold a pointer to UINT8 */
typedef UINT8   *PUINT8;


/* To hold a pointer to UINT16 */
typedef UINT16  *PUINT16;

/* To hold a pointer to UINT32 */
typedef UINT32  *PUINT32;

/* To hold a pointer to an UCHAR */
typedef UCHAR   *PUCHAR;

/* To hold the type definition for void and pointer to void */
typedef VOID    *PVOID;

/************************* MACROS FOR PRINTF AND SCANF ************************/

/*******************************************************************************
 * MACRO NAME    : OS_PRINTF
 * DESCRIPTION   : Macro to print a text.
 *
 *                 NOTE: Avoid using this macro in the code. This should be
 *                       used only for diagnostics.
 *
 * PARAMETERS    : None
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_PRINTF(FORMAT, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)                                      \
        (void)printf(FORMAT, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6 )

/*******************************************************************************
 * MACRO NAME    : OS_SCANF
 * DESCRIPTION   : Macro to obtain inputs from the user.
 *
 *                 NOTE: Avoid using this macro in the code. This should be
 *                       used only for diagnostics.
 *
 * PARAMETERS    : None
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_SCANF(FORMAT, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)                                       \
        scanf(FORMAT, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)


/****************************** MACROS FOR ASSERT *****************************/

/*******************************************************************************
 * MACRO NAME    : OS_ASSERT
 * DESCRIPTION   : Macro for asserting the program execution. This is used to
 *                 identify invalid conditions.
 *
 *                 NOTE: This macro will be enabled only for debug mode.
 * PARAMETERS    : BOOLEAN_EXPRESSION   IN  Boolean expression.
 * RETURN TYPE   : None
 ******************************************************************************/
#ifdef DEBUG
#define OS_ASSERT(BOOLEAN_EXPRESSION)                                   \
        assert(BOOLEAN_EXPRESSION)
#else
#define OS_ASSERT(BOOLEAN_EXPRESSION)
#endif /* End of #ifdef DEBUG */


/*********************** MACROS FOR MEMORY OPERATION **************************/

/*******************************************************************************
 * MACRO NAME    : OS_MEMPOOL_INIT
 * DESCRIPTION   : Initialize memory pool information.
 * PARAMETERS    : NONE.
 * RETURN TYPE   : OK
 ******************************************************************************/
#define OS_MEMPOOL_INIT()                                               \
        ossInitialize()

/*******************************************************************************
 * MACRO NAME    : OS_MALLOC
 * DESCRIPTION   : Macro to allocate memory.
 * PARAMETERS    : MEMORY_SIZE  IN  Size of memory to be allocated.
 * RETURN TYPE   : On success, returns the pointer to the memory. Else returns
 *                 NULL.
 ******************************************************************************/
#define OS_MALLOC(MEMORY_SIZE)                                          \
        OSS_MALLOC(MEMORY_SIZE)

/*******************************************************************************
 * MACRO NAME    : OS_FREE
 * DESCRIPTION   : Macro to release the specified memory.
 * PARAMETERS    : MEMORY_POINTER   IN  Pointer to the memory to be released.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_FREE(MEMORY_POINTER)                                         \
        OSS_FREE(MEMORY_POINTER)

/*******************************************************************************
 * MACRO NAME    : OS_MEMSET
 * DESCRIPTION   : Macro to initialize the memory with the specified value.
 * PARAMETERS    : SRC_POINTER  IN  Pointer to the source memory location.
 *                 VALUE        IN  Value to be assigned to the memory location.
 *                 LENGTH       IN  Number of bytes to be copied.
 * RETURN TYPE   : Returns the number of bytes copied.
 ******************************************************************************/
#define OS_MEMSET(SRC_POINTER, VALUE, LENGTH)                           \
        memset(SRC_POINTER, VALUE, (size_t)(LENGTH))

/*******************************************************************************
 * MACRO NAME    : OS_MEMCPY
 * DESCRIPTION   : Macro to copy the memory from the source to the destination.
 * PARAMETERS    : DST_POINTER  IN  Pointer to the destination memory location.
 *                 SRC_POINTER  IN  Pointer to the source memory location.
 *                 LENGTH       IN  Number of bytes to be copied.
 * RETURN TYPE   : Returns the number of bytes copied.
 ******************************************************************************/
#define OS_MEMCPY(DST_POINTER, SRC_POINTER, LENGTH)                     \
        memcpy(DST_POINTER, SRC_POINTER, LENGTH)


/************************* MACROS FOR STRING OPERATION ************************/

/*******************************************************************************
 * MACRO NAME    : OS_STRLEN
 * DESCRIPTION   : Macro to obtain the length of a null terminated string.
 * PARAMETERS    : STRING   IN  Pointer to the string.
 * RETURN TYPE   : Returns the length of the string.
 ******************************************************************************/
/* To obtain the length of the string */
#define OS_STRLEN(STRING)                                               \
        strlen(STRING)

/*******************************************************************************
 * MACRO NAME    : OS_STRCPY
 * DESCRIPTION   : Macro to copy the source string into the destination string.
 * PARAMETERS    : DST  OUT Pointer to the destination string.
 *                 SRC  IN  Pointer to the source string.
 * RETURN TYPE   : Returns the length of the string.
 ******************************************************************************/
#define OS_STRCPY(DST, SRC)                                             \
        strcpy(DST, SRC)



/************************* MACROS FOR USING EVENTS ****************************/

/* To hold the Event ID */
#define OS_EVENT_ID             SEM_ID

/* To hold the invalid Event ID */
#define OS_INVALID_EVENT_ID     NULL

/* To create an event in signaled state */
#define OS_EVENT_SIGNALED       SEM_FULL

/* To create an event in non signaled state */
#define OS_EVENT_NON_SIGNALED   SEM_EMPTY

/* To wait infinitely for an event to become available */
#define OS_WAIT_INFINITE        WAIT_FOREVER

/* To come out immediately without waiting for an event to become available */
#define OS_NO_WAIT              NO_WAIT

/* Macro to indicate the OS_WAIT_FOR_EVENT returned with a timeout */
#define OS_WAIT_TIMEOUT         ERROR

/*******************************************************************************
 * MACRO NAME    : OS_CREATE_EVENT
 * DESCRIPTION   : Macro to create an event.
 *
 *                 The VxWorks Mutual Exclusion Semaphore is created. The
 *                 semBCreate() function allocates memory for the mutual
 *                 exclusion semaphore and initializes the semaphore.
 *
 * PARAMETERS    : INITIAL_EVENT_STATE  IN  Initial state of the event.
 *
 *                                          Use OS_EVENT_SIGNALED to create
 *                                          an in signaled state.
 *
 *                                          Use OS_EVENT_NON_SIGNALED to
 *                                          create an in non signaled state.
 *
 * RETURN TYPE   : On success, returns the OS_EVENT_ID corresponding to the
 *                 mutual exclustion semaphore. Else returns NULL.
 ******************************************************************************/
#define OS_CREATE_EVENT(INITIAL_EVENT_STATE)                            \
        semBCreate(SEM_Q_PRIORITY, INITIAL_EVENT_STATE)

/*******************************************************************************
 * MACRO NAME    : OS_DESTROY_EVENT
 * DESCRIPTION   : Macro to destroy an event.
 *
 *                 NOTE: The semDelete() call terminates a semaphore and
 *                 deallocates any associated memory. When deleting event,
 *                 first obtain the event by calling OS_WAIT_FOR_EVENT() and
 *                 then delete the event. This ensure that the event is not
 *                 released when some other task is still using the event.
 * PARAMETERS    : EVENT_ID  IN Event ID corresponding to the event to be
 *                              destroyed.
 * RETURN TYPE   : Returns the OS_EVENT_ID corresponding to the mutual
 *                 exclustion semaphore.
 ******************************************************************************/
#define OS_DESTROY_EVENT(EVENT_ID)                                      \
        (void)semDelete(EVENT_ID)

/*******************************************************************************
 * MACRO NAME    : OS_WAIT_FOR_EVENT
 * DESCRIPTION   : Macro to wait for an event.
 * PARAMETERS    : EVENT_ID     IN  Event ID corresponding to the event for
 *                                  which to wait.
 *                 WAIT_VALUE   IN  Amount of time to wait.
 *
 *                                  To wait infinitely, use the macro
 *                                  OS_WAIT_INFINITE.
 *
 *                                  To come out immediately without waiting,
 *                                  use the macro OS_NO_WAIT.
 *
 *                                  To wait for a specified number of milli
 *                                  seconds, use macro
 *                                  OS_CONVERT_MILLISECONDS_TO_WAIT_VALUE()
 *                                  with the parameter specifing the required
 *                                  milli second.
 *
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_WAIT_FOR_EVENT(EVENT_ID, WAIT_VALUE)                         \
            semTake(EVENT_ID, WAIT_VALUE)

/*******************************************************************************
 * MACRO NAME    : OS_RELEASE_EVENT
 * DESCRIPTION   : Macro to release an event.
 * PARAMETERS    : EVENT_ID IN  Event ID corresponding to the event to be
 *                              released.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_RELEASE_EVENT(EVENT_ID)                                      \
            semGive(EVENT_ID)

/************************* MACROS FOR USING DELAYS ****************************/

/*******************************************************************************
 * MACRO NAME    : OS_CONVERT_MILLISECONDS_TO_WAIT_VALUE
 * DESCRIPTION   : Macro to convert the specified duration parameter in milli
 *                 seconds into the number of ticks.
 *
 *                 In order to compute the number of ticks corresponding to the
 *                 number of milli seconds specified, the following logic is
 *                 used:
 *
 *                 a) Call the sysClkRateGet() function to obtain the number
 *                    number of ticks per second.
 *                 b) Compute the number of ticks for the specified number of
 *                    milli seconds.
 *                 c) If the required millisecond cannot be supported, 1 tick
 *                    of delay will be introduced.
 *
 * PARAMETERS    : DURATION     IN  Duration in milli seconds.
 * RETURN TYPE   : Returns the number of ticks.
 ******************************************************************************/
#define OS_CONVERT_MILLISECONDS_TO_WAIT_VALUE(DURATION)                 \
        ((DURATION == WAIT_FOREVER) ? WAIT_FOREVER :                    \
        (0 == ((sysClkRateGet() * DURATION) / 1000) ? 2 :               \
        (((sysClkRateGet() * DURATION) / 1000)+1)))

/*******************************************************************************
 * MACRO NAME    : OS_DELAY_MS
 * DESCRIPTION   : Macro to introduce a delay in milli seconds.
 *
 *                 The taskDelay() function is used to introduce the delay.
 *                 The taskDelay() function takes the delay in number of ticks.
 * PARAMETERS    : DELAY_INTERVAL   IN  Specifies the delay interval in milli
 *                                      seconds.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_DELAY_MS(DELAY_INTERVAL)                                     \
        (void)taskDelay(OS_CONVERT_MILLISECONDS_TO_WAIT_VALUE(DELAY_INTERVAL))

/*******************************************************************************
 * MACRO NAME    : OS_BUSY_WAIT_MS
 * DESCRIPTION   : Macro to introduce a busy wait in milli seconds.
 *                 The vxbMsDelay() function is used to introduce the delay.
 *
 * PARAMETERS    : DELAY_INTERVAL   IN  Specifies the delay interval in milli
 *                                      seconds.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_BUSY_WAIT_MS(DELAY_INTERVAL)                                     \
        vxbMsDelay(DELAY_INTERVAL)

/*******************************************************************************
 * MACRO NAME    : OS_BUSY_WAIT_US
 * DESCRIPTION   : Macro to introduce a busy wait in micro seconds.
 *                 The vxbUsDelay() function is used to introduce the delay.
 *
 * PARAMETERS    : DELAY_INTERVAL   IN  Specifies the delay interval in micro
 *                                      seconds.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_BUSY_WAIT_US(DELAY_INTERVAL)                                     \
        vxbUsDelay(DELAY_INTERVAL)


/************************ MACROS FOR USING THREADS ****************************/

/* To hold the thread ID */
#define OS_THREAD_ID            TASK_ID

/* To hold the thread options */
#define OS_THREAD_OPTIONS       0

/* To hold the stack size of the thread */
#define OS_THREAD_STACK_SIZE    4096 << 2

/* To hold the value returned when a thread operation fails */
#if  (_WRS_VXWORKS_MAJOR > 6) || \
     ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 9))
#define OS_THREAD_FAILURE       TASK_ID_ERROR
#else
#define OS_THREAD_FAILURE       ERROR
#endif

/* To hold the value returned when a thread operation is successful */
#define OS_THREAD_SUCCESS       OK


/*******************************************************************************
 * MACRO NAME    : OS_CREATE_THREAD
 * DESCRIPTION   : Macro to create a thread.
 * PARAMETERS    : NAME         IN  Name of the task.
 *                 PRIORITY     IN  Priority of the task. Tasks may have
 *                                  priorities ranging from 0 (the highest
 *                                  priority), to 255 (the lowest priority).
 *                 ENTRY_POINT  IN  Entry point for the task.
 *                 ARGS         IN  Arguments to be passed to the task.
 *                                  Only one argument can be passed to the task.
 *
 *                 NOTE: Even though VxWorks allows upto 10 parameters to be
 *                       passed to the task, this macro takes only one argument.
 *                       If there are more than one argument, pass them in a
 *                       structure.
 *
 * RETURN TYPE   : On success, returns the OS_THREAD_ID corresponding to the
 *                 thread. Else return OS_THREAD_FAILURE.
 ******************************************************************************/
#define OS_CREATE_THREAD(NAME, PRIORITY, ENTRY_POINT, ARGUMENT)         \
        taskSpawn(NAME,                                                 \
                  PRIORITY,                                             \
                  OS_THREAD_OPTIONS,                                    \
                  OS_THREAD_STACK_SIZE,                                 \
                  (FUNCPTR)ENTRY_POINT,                                 \
                  (_Vx_usr_arg_t)ARGUMENT,                              \
                  0,                                                    \
                  0,                                                    \
                  0,                                                    \
                  0,                                                    \
                  0,                                                    \
                  0,                                                    \
                  0,                                                    \
                  0,                                                    \
                  0)

/*******************************************************************************
 * MACRO NAME    : OS_DESTROY_THREAD
 * DESCRIPTION   : Macro to destroy a thread.
 * PARAMETERS    : THREAD_ID    IN  Thread ID corresponding to the thread to be
 *                                  deleted.
 * RETURN TYPE   : On success, returns OS_THREAD_SUCCESS. Else returns
 *                 OS_THREAD_FAILURE.
 ******************************************************************************/
#define OS_DESTROY_THREAD(THREAD_ID)                                    \
        (void)taskDelete((TASK_ID)(THREAD_ID))

/*******************************************************************************
 * MACRO NAME    : OS_RESCHEDULE_THREAD
 * DESCRIPTION   : Macro to manually reschedule the thread. This macro allows
 *                 the other threads in the schedule queue to execute.
 * PARAMETERS    : None
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_RESCHEDULE_THREAD()                                          \
        (void)taskDelay(1)

/*******************************************************************************
 * MACRO NAME    : OS_GET_THREAD_PRIORITY
 * DESCRIPTION   : Macro to obtain the priority of the thread..
 * PARAMETERS    : THREAD_PRIORITY  OUT Variable to hold the priority of the
 *                                      thread. Tasks may have priorities
 *                                      ranging from 0 (the highest priority),
 *                                      to 255 (the lowest priority).
 *                 ENTRY_POINT  IN  Entry point for the task.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_GET_THREAD_PRIORITY(THREAD_PRIORITY)                         \
        taskPriorityGet(taskIdSelf(), (THREAD_PRIORITY))


/****************** MACROS FOR USING DEFERRED PROCEDURE CALLS *****************/

/* To hold deferred procedure call (DPC) ID */
#define OS_DPC_ID       WDOG_ID

/* To hold the status of a DPC operation */
#define OS_DPC_STATUS   STATUS

/* To hold the value returned when a DPC operation fails */
#define OS_DPC_FAILURE  ERROR

/* To hold the value returned when a DPC operation is successful */
#define OS_DPC_SUCCESS  OK


/*******************************************************************************
 * MACRO NAME    : OS_CREATE_DPC
 * DESCRIPTION   : Macro to create a deferred procedure call.
 * PARAMETERS    : None.
 * RETURN TYPE   : On success returns OS_DPC_ID. Else returns NULL
 ******************************************************************************/
#define OS_CREATE_DPC()                                                 \
        wdCreate()

/*******************************************************************************
 * MACRO NAME    : OS_DESTROY_DPC
 * DESCRIPTION   : Macro to destroy a deferred procedure call.
 * PARAMETERS    : DPC_ID   IN  Specifies the DPC ID.
 * RETURN TYPE   : On success returns OS_DPC_SUCCESS. Else returns
 *                 OS_DPC_FAILURE.
 ******************************************************************************/
#define OS_DESTROY_DPC(DPC_ID)                                          \
        (void)wdDelete(DPC_ID)

/*******************************************************************************
 * MACRO NAME    : OS_START_DPC
 * DESCRIPTION   : Macro to start a deferred procedure call.
 * PARAMETERS    : DPC_ID           IN  Specifies the DPC ID.
 *                 DELAY            IN  Delay count, in ticks.
 *                 DPC_FUNCTION     IN  Routine to call on time out.
 *                 DPC_PARAMETER    IN  Parameter to be passed to the DPC
 *                                      routine.
 * RETURN TYPE   : On success returns OS_DPC_SUCCESS. Else returns
 *                 OS_DPC_FAILURE.
 ******************************************************************************/
#define OS_START_DPC(DPC_ID, DELAY, DPC_FUNCTION, DPC_PARAMETER)        \
        wdStart(DPC_ID, DELAY, (FUNCPTR)DPC_FUNCTION, DPC_PARAMETER)

/*******************************************************************************
 * MACRO NAME    : OS_CANCEL_DPC
 * DESCRIPTION   : Macro to cancel a deferred procedure call.
 * PARAMETERS    : DPC_ID           IN  Specifies the DPC ID.
 * RETURN TYPE   : On success returns OS_DPC_SUCCESS. Else returns
 *                 OS_DPC_FAILURE.
 ******************************************************************************/
#define OS_CANCEL_DPC(DPC_ID)                                           \
        (void)wdCancel(DPC_ID)


/****************** MACROS FOR LOCKED INCREMENT OR DECREMENT ******************/

/*******************************************************************************
 * MACRO NAME    : OS_LOCKED_DECREMENT
 * DESCRIPTION   : Macro to decrement a variable safely.
 *
 *                 NOTE: This macro should be used only when the race condition
 *                       with the ISR.
 *
 * PARAMETERS    : VARIABLE IN  Variable to be decremented.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_LOCKED_DECREMENT(VARIABLE) vxAtomicDec (&VARIABLE);

/*******************************************************************************
 * MACRO NAME    : OS_LOCKED_INCREMENT
 * DESCRIPTION   : Macro to increment a variable safely.
 *
 *                 NOTE: This macro should be used only when the race condition
 *                       with the ISR.
 *
 * PARAMETERS    : VARIABLE IN  Variable to be incremented.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_LOCKED_INCREMENT(VARIABLE) vxAtomicInc (&VARIABLE);


/********************** MACROS FOR HANDLING ENDIANESS *************************/

#if  (_BYTE_ORDER == _BIG_ENDIAN)

/*******************************************************************************
 * MACRO NAME    : OS_UINT32_CPU_TO_LE
 * DESCRIPTION   : Macro to convert an 32 bit integer from CPU format to the
 *                 little endian format.
 * PARAMETERS    : DATA IN  Data to be converted.
 * RETURN TYPE   : None
 ******************************************************************************/

#define OS_UINT32_CPU_TO_LE(DATA)                                       \
        (((DATA & 0xFF) << 24) |                                        \
                (((DATA >> 8) & 0xFF) << 16) |                          \
                (((DATA >> 16) & 0xFF) << 8) |                          \
                ((DATA >> 24) & 0xFF))

/*******************************************************************************
 * MACRO NAME    : OS_UINT32_LE_TO_CPU
 * DESCRIPTION   : Macro to convert an 32 bit integer from little endian format
 *                 to the CPU format.
 * PARAMETERS    : DATA IN  Data to be converted.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_UINT32_LE_TO_CPU(DATA)                                       \
        (((DATA & 0xFF) << 24) |                                        \
                (((DATA >> 8) & 0xFF) << 16) |                          \
                (((DATA >> 16) & 0xFF) << 8) |                          \
                ((DATA >> 24) & 0xFF))

/*******************************************************************************
 * MACRO NAME    : OS_UINT16_LE_TO_CPU
 * DESCRIPTION   : Macro to convert an 16 bit integer from little endian format
 *                 to the CPU format.
 * PARAMETERS    : DATA IN  Data to be converted.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_UINT16_LE_TO_CPU(DATA)                                       \
        ((((DATA >> 8) & 0xFF) | ((DATA & 0xFF) << 8)))

/*******************************************************************************
 * MACRO NAME    : OS_UINT16_CPU_TO_LE
 * DESCRIPTION   : Macro to convert an 16 bit integer from CPU format to the
 *                 little endian format.
 * PARAMETERS    : DATA IN  Data to be converted.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_UINT16_CPU_TO_LE(DATA)                                       \
        ((((DATA >> 8) & 0xFF) | ((DATA & 0xFF) << 8)))

#define OS_BUFFER_CPU_TO_LE(BUFFER, LENGTH)

#define OS_BUFFER_LE_TO_CPU(BUFFER, LENGTH)

#else

/*
 * Macros for handling the endianess on little endian
 * systems
 */

/*******************************************************************************
 * MACRO NAME    : OS_BUFFER_LE_TO_CPU
 * DESCRIPTION   : Macro to convert a buffer from little endian format to the
 *                 CPU format.
 * PARAMETERS    : BUFFER   IN  Pointer to the buffer location.
 *                 LENGTH   IN  Number of bytes to be converted.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_BUFFER_LE_TO_CPU(BUFFER, LENGTH)

/*******************************************************************************
 * MACRO NAME    : OS_BUFFER_CPU_TO_LE
 * DESCRIPTION   : Macro to convert a buffer from CPU format to the little
 *                 endian format .
 * PARAMETERS    : BUFFER   IN  Pointer to the buffer location.
 *                 LENGTH   IN  Number of bytes to be converted.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_BUFFER_CPU_TO_LE(BUFFER, LENGTH)

/*******************************************************************************
 * MACRO NAME    : OS_UINT16_LE_TO_CPU
 * DESCRIPTION   : Macro to convert an 16 bit integer from little endian format
 *                 to the CPU format.
 * PARAMETERS    : DATA IN  Data to be converted.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_UINT16_LE_TO_CPU(DATA)                                       \
        DATA

/*******************************************************************************
 * MACRO NAME    : OS_UINT16_CPU_TO_LE
 * DESCRIPTION   : Macro to convert an 16 bit integer from CPU format to the
 *                 little endian format.
 * PARAMETERS    : DATA IN  Data to be converted.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_UINT16_CPU_TO_LE(DATA)                                       \
        DATA

/*******************************************************************************
 * MACRO NAME    : OS_UINT32_LE_TO_CPU
 * DESCRIPTION   : Macro to convert an 32 bit integer from little endian format
 *                 to the CPU format.
 * PARAMETERS    : DATA IN  Data to be converted.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_UINT32_LE_TO_CPU(DATA)                                       \
        DATA

/*******************************************************************************
 * MACRO NAME    : OS_UINT32_CPU_TO_LE
 * DESCRIPTION   : Macro to convert an 32 bit integer from CPU format to the
 *                 little endian format.
 * PARAMETERS    : DATA IN  Data to be converted.
 * RETURN TYPE   : None
 ******************************************************************************/
#define OS_UINT32_CPU_TO_LE(DATA)                                       \
        DATA


#endif /* End of #if  (_BYTE_ORDER != _BIG_ENDIAN) */

#endif /* End of __OSAL_H__ */

/****************************** End of File OSAL.h ****************************/

