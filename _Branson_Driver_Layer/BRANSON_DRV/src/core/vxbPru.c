/* vxbPru.c - PRU device library*/

/*
 * Copyright (c) 2013-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
25mar16,zly  missing semGive() (V7STO-607)
22dec15,y_y  fix unsupported open flags. (V7RAD-2611)
12jun15,p_x  fix file pointer issue. (V7RAD-120)
22jan15,d_l  fix gnu warnings.
24nov14,xms  fix ioctl FIOSEEK issue. (V7PRO-1333)
18aug14,l_z  fix static analysis issue. (V7PRO-1181)
12may14,l_z  remove unused variable. (V7PRO-909)
22aug13,y_y  created
*/

/*
DESCRIPTION
The logical layers of a typical system using a Pru appear:

/cs
         +--------------------+
         | Application module |
         +--------------------+ <-- read(), write(), ioctl(), close()
                   |
         +--------------------+
         | VxWorks I/O System |
         +--------------------+ <-- IOS layer iosRead, iosWrite, ioctl...(vxbPru.c)
                   |
+-----------------------------------------+
| Diff interface Controller driver module |
+-----------------------------------------+ <-- eg: vxbFdtTiPrussEnd.c...

/cs

This driver allows the I/O system to access pru directly as an I/O device.

USER-CALLABLE ROUTINES
Most of the routines in this driver are accessible only through the
I/O system. However, the following two routines can be called directly:
pruDrv() to initialize the driver, and pruDevRemove("name") to delete
devices.

Before using the driver, it must be initialized by calling pruDrv().
This routine should be called only once, before any reads, writes, or
pruDrvLoad() calls.

The open() function establishes the connection between a pru and a file descriptor.
The file descriptor is used by other I/O functions to refer to that pru. The file 
access modes of the open file description will be set according to the value of flag.

Values for flag are constructed by a bitwise-inclusive-OR of flags from the following list,
defined in <fcntlcom.h>. 

Applications must specify exactly one of the first three values (file access modes) 
below in the value of flag:

O_RDONLY
Open for reading only.
O_WRONLY
Open for writing only.
O_RDWR
Open for reading and writing. 

Any combination of the following could not be supported when access pru:
O_APPEND
O_CREAT
O_DSYNC
O_EXCL
O_NOCTTY
O_NONBLOCK
O_RSYNC
O_TRUNC

CONFIGURATION
To use the pru device driver, configure VxWorks with the
INCLUDE_PRUDRV component.

This will result in the creation of the "/pru/0" I/O device for pru,
as shown by the output of the 'devs' target shell command:
\cs
-> devs
drv name
  0 /null
  1 /tyCo/0
  1 /tyCo/1
  1 /tyCo/2
  1 /tyCo/3
  4 /tffs0
  2 /eeprom/0
  3 /pru/0
  7 host:
value = 0 = 0x0
\ce

SEE ALSO:
the VxWorks programmer guides.

*/

/* includes */

#include <vxWorks.h>
#include <semLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <logLib.h>
#include <lstLib.h>
#include <iosLib.h>
#include <hwif/vxBus.h>
#include <vxbPru.h>

/* typedefs */

typedef struct
    {
    DEV_HDR    devHdr;        /* device header */
    VXB_DEV_ID devID;         /* device id */
    char       name[MAX_DRV_NAME_LEN];

    STATUS     (*pruRead) (VXB_DEV_ID, UINT32, char*, UINT32, UINT32*);
    STATUS     (*pruWrite)(VXB_DEV_ID, UINT32, char*, UINT32, UINT32*);
    } PRU_DEV;

/* pru device file descriptor */

typedef struct      /* PRU_FILE_DESC */
    {
    NODE         fdNode;       /* linked list node info */
    SEM_ID       fdSemId;      /* semaphore for this file descriptor */
    int          fdStatus;     /* (OK | NOT_IN_USE) */
    int          fdMode;       /* mode: O_RDONLY, O_WRONLY, O_RDWR */
    int          fdCurtPtr;    /* file byte ptr for new read/writes */
    PRU_DEV * pDev;
    } PRU_FILE_DESC;

/* defines */
    
/* File descriptor status values */

#define FD_AVAILABLE      (-1)    /* file descriptor available */
#define FD_IN_USE         0       /* file descriptor in-use */

/* max files open at once */

#define PRU_DEF_MAX_FILES  2

/* unsupported flag*/

#define PRU_UNSUPPORT_FLAG  O_APPEND | O_NONBLOCK | O_TRUNC | O_CREAT | \
                               O_EXCL | O_DSYNC | O_RSYNC | O_NOCTTY

/* LOCALS */

LOCAL int pruDrvNum = ERROR;   /* driver number assigned to this driver */

LOCAL LIST pruFdActiveList;    /* linked list of in-use Fd's */
LOCAL LIST pruFdFreeList;      /* linked list of avail. Fd's */
LOCAL SEM_ID pruFdListSemId;   /* file descr list semaphore  */

/* forward declarations */

LOCAL PRU_FILE_DESC * pruDrvOpen (PRU_DEV * pPruDev, char * name, int flags);
LOCAL ssize_t pruDrvRead (PRU_FILE_DESC * pFd, char * buffer, size_t bytes);
LOCAL ssize_t pruDrvWrite (PRU_FILE_DESC * pFd, char * buffer, size_t bytes);
LOCAL STATUS pruDrvClose (PRU_FILE_DESC * pFd);
LOCAL PRU_FILE_DESC * pruFdGet (void);
LOCAL void pruFdFree (PRU_FILE_DESC * pFd);

struct pru_info
    {
    char   name[MAX_DRV_NAME_LEN];
    };


/*******************************************************************************
*
* pruDrv - install a pru driver
*
* This routine initializes the pru driver. It is called automatically
* when VxWorks is configured with the INCLUDE_PRUDRV component.
*
* It must be called exactly once, before any other routine in the library.
* The maximum number of file descriptors that may be open at once are MAXFILES.
* This routine allocates and sets up the necessary memory structures and
* initializes semaphores.
*
* This routine also installs pru library routines in the VxWorks I/O
* system driver table.  The driver number assigned to pru device is placed in
* the global variable "pruDrvNum".  This number will later be associated
* with system file descriptors opened to pru devices.
*
* RETURNS: OK, or ERROR if the I/O system cannot install the driver.
*
* ERRNO: N/A.
*/

STATUS pruDrv(void)
    {
    PRU_FILE_DESC * pPruFd; /* pointer to created file descriptor */
    int ix;                       /* index var */

    /* Check if driver already installed */

    if (pruDrvNum > 0)
        {
        return (OK);
        }

    pruDrvNum = iosDrvInstall ((DRV_CREATE_PTR) pruDrvOpen,
                                  (DRV_REMOVE_PTR) NULL,
                                  (DRV_OPEN_PTR)   pruDrvOpen,
                                  (DRV_CLOSE_PTR)  pruDrvClose,
                                  (DRV_READ_PTR)   pruDrvRead,
                                  (DRV_WRITE_PTR)  pruDrvWrite,
								  (DRV_IOCTL_PTR)  NULL);

    if (pruDrvNum <= 0)
        {
        return (ERROR);
        }

    /* Create semaphore for locking access to file descriptor list */

    pruFdListSemId = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE);

    /* Can't create semaphore */

    if (pruFdListSemId == NULL)
        {
        return (ERROR);
        }

    /* Take control of fd list */

    if (semTake (pruFdListSemId, WAIT_FOREVER) != OK)
        return ERROR;

    /* Allocate memory for required number of file descriptors */

    pPruFd = (PRU_FILE_DESC *)
                malloc (PRU_DEF_MAX_FILES * sizeof (PRU_FILE_DESC));

    if (NULL == pPruFd)
        {
        (void) semGive (pruFdListSemId);
        return (ERROR);
        }

    bzero ((char *) pPruFd, (PRU_DEF_MAX_FILES * sizeof (PRU_FILE_DESC)));

    for (ix = 0; ix < PRU_DEF_MAX_FILES; ix++)
        {
        pPruFd->fdStatus = FD_AVAILABLE;

        /* Create semaphore for this fd (initially available) */

        pPruFd->fdSemId = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE);

        if (pPruFd->fdSemId == NULL)
            {
            free (pPruFd);
            (void) semGive (pruFdListSemId);
            return (ERROR);     /* Could not create semaphore */
            }

        /* Add file descriptor to free list */

        lstAdd (&pruFdFreeList, &pPruFd->fdNode);

        /* Next Fd */

        pPruFd++;
        }

    /* Release Fd lists */

    (void) semGive (pruFdListSemId);

    return OK;
    }

/*******************************************************************************
*
* pruDrvNextUnit - assign the unused unit number
*
* RETURN - OK or ERROR
*
* ERRNO - NA
*/

UINT32 pruDrvNextUnit()
    {
    static int pruDrvNextNumber = 0;

    return(pruDrvNextNumber++);
    }

/*******************************************************************************
*
* pruDrvLoad - retrieve the pru info and create device
*
* RETURN - OK or ERROR
*
* ERRNO - NA
*/

STATUS pruDevCreate
    (
    char *            name,   /* name to use for this device */
    VXB_DEV_ID        pDev,
    VXB_PRU_FUNC * pCandidate
    )
    {
    PRU_DEV * pPruDev = NULL;

    /* Check for the validity of the function parameters */

    pPruDev = malloc (sizeof (PRU_DEV));

    VXB_ASSERT (pPruDev != NULL, ERROR);

    bzero ((char *) pPruDev, sizeof (PRU_DEV));

    /* Retrieve the pru device name */

    strncpy (pPruDev->name, "/pru/0", MAX_DRV_NAME_LEN - 1);

    /* Retrieve the pru device read/write functions */

    pPruDev->pruWrite = (void *) pCandidate->pruWrite;
    pPruDev->pruRead = (void *) pCandidate->pruRead;

    pPruDev->devID = pDev;

    /* Add the device to the I/O systems device list */

    if (iosDevAdd ((DEV_HDR *) & (pPruDev->devHdr), name, pruDrvNum) != OK)
        {
    	kprintf("iosdevadd error\n");
        free (pPruDev);
        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* pruDrvOpen - open pru device
*
* This routine is called to open a pru.  It returns a pointer to the
* device.  This routine is normally reached only via the I/O system.
*
* RETURNS: The opaque PRU_FILE_DESC ptr. For this driver it is really a
* pointer to a PRU_FILE_DESC structure.
*
* ERRNO: N/A
*/

LOCAL PRU_FILE_DESC * pruDrvOpen
    (
    PRU_DEV * pPruDev,
    char *       name,
    int          flags
    )
    {
    PRU_FILE_DESC * pFd;     /* file descriptor pointer */

    /* Make sure it's a valid pru id */

    if (pPruDev == NULL)
        {
        return((PRU_FILE_DESC *) ERROR);
        }

    /* Non-null filename */

    if (name[0] != EOS)
        {
        return ((PRU_FILE_DESC *) ERROR);
        }

    /* Check for invalid mode */

    flags &= ~(PRU_UNSUPPORT_FLAG); 

    if ((flags != O_RDONLY) && (flags != O_WRONLY) && (flags != O_RDWR))
        {
        return ((PRU_FILE_DESC *) ERROR);
        }

    /* Get a free file descriptor */

    if ((pFd = pruFdGet ()) == NULL)
        {
        return ((PRU_FILE_DESC *) ERROR);
        }

    /* Take control of fd */

    if (semTake (pFd->fdSemId, WAIT_FOREVER) != OK)
        {
        return ((PRU_FILE_DESC *) ERROR);
        }

    /* Save the flags */

    pFd->fdMode = flags;
    pFd->pDev = (PRU_DEV *) pPruDev;

    /* Set the location to 0 */

    pFd->fdCurtPtr = 0;

    /* Release fd */

    (void) semGive (pFd->fdSemId);

    return (pFd);
    }



/*******************************************************************************
*
* pruDrvRead - read bytes from pru device
*
* Perform a read operation on an open pru file.
*
* RETURNS: The Number of bytes actually read
*
* ERRNO - NA
*/

LOCAL ssize_t pruDrvRead
    (
    PRU_FILE_DESC * pFd,
    char *             buffer,     /* buffer to read */
    size_t             bytes       /* maximum length of read */
    )
    {
    PRU_DEV * pVd;
    UINT32 readByte = 0;

    /* Make sure it's a valid id */

    if (pFd == NULL || (pVd = pFd->pDev) == NULL)
        {
        return ERROR;
        }

    /* Make sure the file isn't opened in write-only mode */

    if ((pFd->fdMode & O_ACCMODE) == O_WRONLY)
        {
        return (ERROR);
        }

    /* Take control of fd */

    if (semTake (pFd->fdSemId, WAIT_FOREVER) != OK)
        {
        return (ERROR);
        }

	if (bytes > 1)
		{
		bytes = 1;
		}
		
    if (pVd->pruRead (pVd->devID,
                         pFd->fdCurtPtr,
                         buffer,
                         (UINT32) bytes,
                         &readByte) != OK)
        {
    	
        (void) semGive (pFd->fdSemId);
        return ERROR;
        }

    pFd->fdCurtPtr += readByte;

    (void) semGive (pFd->fdSemId);

    return (ssize_t) readByte;
    }

/*******************************************************************************
*
* pruDrvWrite - write pru device
*
* Perform a write operation to an open pru file.
*
* RETURNS: The number of bytes written
*
* ERRNO - NA
*/

LOCAL ssize_t pruDrvWrite
    (
    PRU_FILE_DESC * pFd,
    char *             buffer,     /* buffer to write */
    size_t             bytes       /* maximum length of read */
    )
    {
    PRU_DEV * pVd;
    UINT32 writeByte = 0;

    /* Make sure it's a valid pru id */

    if (pFd == NULL || (pVd = pFd->pDev) == NULL)
        {
        return ERROR;
        }

    /* Make sure the file isn't opened in read-only mode */

    if ((pFd->fdMode & O_ACCMODE) == O_RDONLY)
        {
        return (ERROR);
        }

    /* Take control of fd */

    if (semTake (pFd->fdSemId, WAIT_FOREVER) != OK)
        {
        return (ERROR);
        }


    if (pVd->pruWrite (pVd->devID,
                          pFd->fdCurtPtr,
                          buffer,
                          (UINT32) bytes,
                          &writeByte) != OK)
        {
        (void) semGive (pFd->fdSemId);
        return ERROR;
        }

    pFd->fdCurtPtr += writeByte;

    (void) semGive (pFd->fdSemId);
    
    return (ssize_t) writeByte;
    }

/*******************************************************************************
*
* pruDrvClose - close a pru device
*
* This routine is called to close a pru.  This routine is normally reached
* only via the I/O system.
*
* RETURNS:  OK or ERROR if NULL pru device pointer.
*
* ERRNO: N/A.
*/

LOCAL STATUS pruDrvClose
    (
    PRU_FILE_DESC * pPruFd
    )
    {
    /* Take control of file descriptor */

    if (semTake (pPruFd->fdSemId, WAIT_FOREVER) != OK)
        {
        return (ERROR);
        }

    /* Put fd on free list */

    pruFdFree (pPruFd);

    /* Release fd */

    (void) semGive (pPruFd->fdSemId);

    return (OK);
    }

/*******************************************************************************
*
* pruDevRemove - remove a pru device
*
* This routine deletes a pru device. The device is deleted with it own name.
*
* For example, to delete the device, the proper call would be:
* \cs
*   pruDevRemove ("/pru/0");
* \ce
*
* RETURNS: OK, or ERROR if the device doesn't exist.
*
* ERRNO: N/A.
*/

STATUS pruDevRemove
    (
    char * name           /* device name */
    )
    {
    DEV_HDR * pDevHdr;

    /* Get the device pointer corresponding to the device name */

    if ((pDevHdr = iosDevFind (name, NULL)) == NULL)
        {
        return (ERROR);
        }

    /* Delete the device from the I/O system */

    (void) iosDevDelete (pDevHdr);

    /* Free the device pointer */

    free ((PRU_DEV *) pDevHdr);

    return (OK);
    }

/*******************************************************************************
*
* pruFdGet - get an available file descriptor
*
* This routine obtains a free pru file descriptor.
*
* RETURNS: Pointer to file descriptor, or NULL if none available.
*
* ERRNO - NA
*/

LOCAL PRU_FILE_DESC * pruFdGet
    (
    void
    )
    {
    PRU_FILE_DESC * pFd;

    /* Take control of Fd lists */

    if (semTake (pruFdListSemId, WAIT_FOREVER) != OK)
        return NULL;

    /* Get a free Fd */

    pFd = (PRU_FILE_DESC *) lstGet (&pruFdFreeList);

    if (pFd != NULL)
        {
        /* Mark Fd as in-use */
        pFd->fdStatus = FD_IN_USE;

        /* Add to active list */
        lstAdd (&pruFdActiveList, (NODE *) pFd);
        }

    /* Release Fd lists */

    (void) semGive (pruFdListSemId);

    return (pFd);
    }

/*******************************************************************************
*
* pruFdFree - free a file descriptor
*
* This routine removes a pru device file descriptor from the active
* Fd list and places it on the free Fd list.
*
* RETURN - NA
*
* ERRNO - NA
*/

LOCAL void pruFdFree
    (
    PRU_FILE_DESC * pPruFd      /* pointer to file descriptor to free */
    )
    {
    /* Take control of Fd lists */

    if (semTake (pruFdListSemId, WAIT_FOREVER) != OK)
        return;

    pPruFd->fdStatus = FD_AVAILABLE;

    /* Remove Fd from active list */

    lstDelete (&pruFdActiveList, &pPruFd->fdNode);

    /* Add Fd to free list */

    lstAdd (&pruFdFreeList, &pPruFd->fdNode);

    /* Release Fd lists */

    (void) semGive (pruFdListSemId);
    }

