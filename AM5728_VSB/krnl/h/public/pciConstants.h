/* pciConstants.h - Defines constants related to the PCI bus. */

/*
 * Copyright (c) 1999-2001, 2005, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01f,29apr10,pad  Moved extern C statement after include statements.
01e,13jan10,ghs  vxWorks 6.9 LP64 adapting
01d,28mar05,pdg  non-PCI changes
01c,18sep01,wef  merge from wrs.tor2_0.usb1_1-f for veloce
01b,05dec00,wef  removed some macros that were defined in pciConfigLib.h
01a,29may99,rcb  First.
*/

#ifndef __INCpciConstantsh
#define __INCpciConstantsh

/* includes */

#include <drv/pci/pciConfigLib.h>

#ifdef  __cplusplus
extern "C" {
#endif

/* Constants */

#ifndef PCI_MAX_BUS
#define PCI_MAX_BUS     255    /* Max number of PCI buses in system */
#endif  /* PCI_MAX_BUS */

#ifndef PCI_MAX_DEV
#define PCI_MAX_DEV     32     /* Max number of PCI devices in system */
#endif  /* PCI_MAX_DEV */

/* Constants related to PCI configuration */

#define PCI_CFG_NUM_BASE_REG    6   /* Number of base address registers */
                                    /* in PCI config. space for a device */

/* The following PCI_CFG_ADDRESS_xxxx constants qualify the address in each
of the PCI configuration base address registers. */

#define PCI_CFG_BASE_IO     0x00000001  /* Mask indicates adrs is I/O adrs */
#define PCI_CFG_BASE_MEM    0x00000000  /* Mask indicates adrs is mem adrs */

#define PCI_CFG_IOBASE_MASK     ~(0x3)
#define PCI_CFG_MEMBASE_MASK    ~(0xf)

#define OHCI_CLASS              0x0c        /* BASEC value for serial bus */
#define OHCI_SUBCLASS           0x03        /* SCC value for USB */
#define OHCI_PGMIF              0x10        /* OpenHCI */

#define UHCI_CLASS              0x0c        /* BASEC value for serial bus */
#define UHCI_SUBCLASS           0x03        /* SCC value for USB */
#define UHCI_PGMIF              0x00        /* no specific pgm i/f defined */
#define PCI_UHCI_LEGREG_OFFSET  0xC0        /* UHCI Legacy register offset */
#define PCI_UHCI_LEGREG_VALUE   0x2000      /* UHCI Legacy register value */

#define EHCI_CLASS              0x0c        /* BASEC value for serial bus */
#define EHCI_SUBCLASS           0x03        /* SCC value for USB */
#define EHCI_PGMIF              0x20        /* EnhancedHCI */

#define EHCI_HCCPARAMS          0x08        /* HCCPARAMS register offset */
#define EHCI_HCCPARAMS_EECP_MASK    0x0000FF00     /*
                                                    * extended capabilities
                                                    * field position mask value
                                                    */
#define EHCI_HCCPARAMS_EECP_MIN     0x40           /*
                                                    * extended capabilities
                                                    * minimum value
                                                    */
#define EHCI_USBLEGSUP_OS_OFFSET    3              /*
                                                    * Offset into the USBLEGSUP
                                                    * register to access the OS
                                                    * controllable space
                                                    */

#define USB_IF_NON_PCI              0x80        /* Mask value for non-PCI bus */

/* typedefs */

/*
 * PCI_CFG_HEADER
 *
 * PCI_CFG_HEADER defines the invariant portion of a PCI devices configuration
 * space.
 *
 * NOTE: If 0-byte packing is in effect, this structure will match the PCI
 * configuration header byte-for-byte.
 */

typedef struct pci_cfg_header
    {
    UINT16 vendorId;                       /* PCI-assigned vendor ID */
    UINT16 deviceId;                       /* Vendor-assigned device ID */
    UINT16 command;                        /* Config. cmd register */
    UINT16 status;                         /* Cfg status register */
    UINT8 revisionId;                      /* Revision ID */
    UINT8 pgmIf;                           /* Programming interface */
    UINT8 subClass;                        /* PCI Sub-class */
    UINT8 pciClass;                        /* PCI Class code */
    UINT8 cacheLineSize;                   /* Cache line size */
    UINT8 latencyTimer;                    /* Latency timer */
    UINT8 headerType;                      /* Header type */
    UINT8 bist;                            /* BIST */
    UINT32 baseReg [PCI_CFG_NUM_BASE_REG]; /* Base adrs registers */
    UINT32 reserved [2];
    UINT32 romBase;                        /* Expansion ROM base address */
    UINT32 reserved1 [2];
    UINT8 intLine;                         /* Interrupt line chosen by POST */
    UINT8 intPin;                          /* HW int assignment(INTA = 1, etc)*/
    UINT8 minGrant;                        /* Minimum grant */
    UINT8 maxLatency;                      /* Maximum latency */
    } PCI_CFG_HEADER, *pPCI_CFG_HEADER;

/*
 * USB_HCD_BUS_INFO
 *
 * USB_HCD_BUS_INFO defines the function pointers which can be used for
 * performing host bus specific operations.
 */
typedef struct usbHcdBusInfo
    {
    FUNCPTR     pFuncDataSwap;      /* Function to swap data */
    FUNCPTR     pFuncCpuToBus;      /* Function to convert CPU to Bus memory */
    FUNCPTR     pFuncBusToCpu;      /* Function to convert Bus to CPU memory */
    FUNCPTR     pFuncIntConnect;    /* Function to hook an ISR */
    FUNCPTR     pFuncIntDisconnect; /* Function to unhook an ISR */
    pVOID       pBaseAddress;       /* Base Address of registers */
    UINT32      irq;                /* Irq value of the controller */
    FUNCPTR     pFuncBufferSwap;    /* Function to swap buffer contents */
    pVOID       pDeviceSpecificData;/*
                                     * Pointer which can be used for
                                     * storing device specific data
                                     */
    }USB_HCD_BUS_INFO, *pUSB_HCD_BUS_INFO;

#ifdef  __cplusplus
}
#endif

#endif  /* __INCpciConstantsh */

/* End of file. */

