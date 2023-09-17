/* usbUas.h - USB Attached SCSI (UAS) Public Header */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,11jun11,w_x  written
*/

#ifndef __INCusbUash
#define __INCusbUash

#include <vxWorks.h>
#include <version.h>
#include <usb.h>

#ifdef __cplusplus
extern "C" {
#endif

#define USB_MSC_UAS_VERSION1            1
#define USB_MSC_UAS_VERSION2            2
#define USB_MSC_UAS_VERSION3            3
#define USB_MSC_UAS_VERSION4            4
#define USB_MSC_UAS_VERSION_SUPPORTED   4

/* 
 * USB Interface Descriptor bInterfaceProtocol filed 
 * for USB Attached SCSI (UAS) Transport Protocol.
 */

#define USB_MSC_PROTOCOL_UAS       0x62  

/* 
 * 5.3.3.5 Pipe Usage class specific descriptor
 *
 * A Pipe Usage class specific descriptor shall be the first descriptor 
 * following each endpoint descriptor referenced by the Interface 
 * descriptor (see 5.2.3.3). 
 */

typedef struct usbUasPipeUsageDescr
    {
    UINT8   bLength;        /* Size of this descriptor in bytes (0x04) */
    UINT8   bDescriptorType;/* Pipe Usage class specific descriptor (0x24) */
    UINT8   bPipeId;        /* Identifies pipe associated with the EP Descr */
    UINT8   bRzv;           /* Reserved */
    }USB_UAS_PIPE_USAGE_DESCR, *pUSB_UAS_PIPE_USAGE_DESCR;

/* 
 * Table 9 - Pipe ID
 *
 * The bPipeId field identifies the pipe associated with the 
 * endpoint descriptor. values 0, 05h-DFh, F0h-FFh are Reserved,
 * and E0h-EFh Vendor specific. The UAS spec defines the following
 * Pipe IDs.
 */
 
#define USB_UAS_PIPE_ID_CMD         1 /* Command pipe */
#define USB_UAS_PIPE_ID_STS         2 /* Status pipe */
#define USB_UAS_PIPE_ID_DATA_IN     3 /* Data-in pipe */
#define USB_UAS_PIPE_ID_DATA_OUT    4 /* Data-out pipe */

/*
 * Table 10 - IU ID field summary
 *
 * Summary of the Information Units (IUs) and the assoicated IU ID field.
 *
 * If a UAS target port processes an IU with an IU ID field conaining a 
 * reserved value, then the UAS target port shall return a RESPONSE IU 
 * with the RESPONSE CODE field set to INVALID INFORMATION UNIT.
 *
 * Values 0, 2, and 08h..FFh are Reserved. The UAS spec defines the 
 * following IU IDs.
 */
 
#define USB_UAS_IU_ID_CMD           1 /* COMMAND IU */
#define USB_UAS_IU_ID_SENSE         3 /* SENSE IU */
#define USB_UAS_IU_ID_RESPONSE      4 /* RESPONSE IU */
#define USB_UAS_IU_ID_TASK_MGMT     5 /* TASK MANAGEMENT IU */
#define USB_UAS_IU_ID_READ_READY    6 /* READ READY IU */
#define USB_UAS_IU_ID_WRITE_READY   7 /* WRITE READY IU */

/* 
 * Table 11 - IU Header
 *
 * All IUs include the header defined in table 11 as the first bytes 
 * of the IU.
 */ 

typedef struct usbUasIUHdr
    {
    UINT8   uID;    /* IU ID, identifies the type of IU (see table 10) */
    UINT8   uRzv0;  /* Reserved */
    UINT16  uTAG;   /* The TAG field is in BIG ENDIAN */
    }USB_UAS_IU_HDR, *pUSB_UAS_IU_HDR;

/*
 * 6.2.2 COMMAND IU
 *
 * The COMMAND IU shall be contained in a single USB packet and shall 
 * not share a USB packet with any other IU. Table 12 defines the 
 * COMMAND IU.
 */

typedef struct usbUasCmdIU
    {
    UINT8   uID;        /* IU ID, identifies the type of IU (see table 10) */
    UINT8   uRzv0;      /* Reserved */
    UINT16  uTAG;       /* The TAG field is in BIG ENDIAN */
#if (USB_MSC_UAS_VERSION_SUPPORTED < 4)  
    UINT16  uLength;    /* Number of bytes that follow in the COMMAND IU */
#endif /* (USB_MSC_UAS_VERSION_SUPPORTED < 4) */
    UINT8   uPrioAttr;  /* COMMAND PRIORITY and TASK ATTRIBUTE */
    UINT8   uRzv1;      /* Reserved */
#if (USB_MSC_UAS_VERSION_SUPPORTED >= 4)  
    UINT8   uAddCDBLen; /* ADDITIONAL CDB LENGTH (n dwords) */
    UINT8   uRzv2;      /* Reserved */
#endif /* (USB_MSC_UAS_VERSION_SUPPORTED >= 4) */    
    UINT64  uLUN;       /* LOGICAL UNIT NUMBER in BIG ENDIAN */
    UINT8   pCDB[16];   /* CDB (may be followed by ADDITIONAL CDB BYTES) */
    }USB_UAS_CMD_IU, *pUSB_UAS_CMD_IU;

/* 
 * Access macros for COMMAND PRIORITY field in <uPrioAttr>.
 *
 * The COMMAND PRIORITY field specifies the relative scheduling of 
 * this command as defined in SAM-4.
 */

#define USB_UAS_CMD_IU_PRIO_OFFSET 3
#define USB_UAS_CMD_IU_PRIO_MASK ((0xF << 3))
#define USB_UAS_CMD_IU_PRIO_GET(uPrioAttr) \
            (((uPrioAttr) >> 3) & 0xF)
#define USB_UAS_CMD_IU_PRIO(uPrio) \
            (((uPrio) & 0xF) << 3)

/* Access macros for TASK ATTRIBUTE field in <uPrioAttr> */

#define USB_UAS_CMD_IU_ATTR_OFFSET 0
#define USB_UAS_CMD_IU_ATTR_MASK (0x07)
#define USB_UAS_CMD_IU_ATTR_GET(uPrioAttr) \
            ((uPrioAttr) & 0x07)
#define USB_UAS_CMD_IU_ATTR(uAttr) \
            ((uAttr) & 0x07)

/*
 * Table 13 - TASK ATTRIBUTE field
 *
 * Specifies that the command be managed according to the rules for a
 * simple, head of queue, ordered, and automatic contingent allegiance
 * task attribute (see SAM-4). 
 *
 * Values 3, 5, 6, 7 are Reserved.
 */
 
#define USB_UAS_TASK_ATTR_SIMPLE    0 /* simple task attribute */
#define USB_UAS_TASK_ATTR_Q_HEAD    1 /* head of queue task attribute */
#define USB_UAS_TASK_ATTR_ORDERED   2 /* ordered task attribute */
#define USB_UAS_TASK_ATTR_ACA       4 /* automatic contingent allegiance 
                                           * task attribute */

/*
 * 6.2.3 READ READY IU
 *
 * The READ READY IU is sent by a UAS target port to inform the UAS 
 * initiator port that the UAS target port is ready to send data for 
 * a data-in command (see 6.3.5) or a bi-directional command (see 6.3.7). 
 * UAS target devices that connect via SuperSpeed (see USB-3) shall 
 * return an ERDY transaction packet (see USB-3) on the Data-in pipe 
 * using the tag as the stream ID instead of the READ READY IU on the 
 * Status pipe. The READ READY IU shall be contained in a single USB 
 * packet and shall not share a USB packet with any other Information 
 * Unit. Table 14 describes the READ READY IU.
 */

typedef struct usbUasReadReadyIU
    {
    UINT8   uID;    /* IU ID, identifies the type of IU (see table 10) */
    UINT8   uRzv0;  /* Reserved */
    UINT16  uTAG;   /* The TAG field is in BIG ENDIAN */
    }USB_UAS_READ_READY_IU, *pUSB_UAS_READ_READY_IU;

/*
 * 6.2.4 WRITE READY IU
 *
 * The WRITE READY IU is sent by a UAS target port to request write 
 * data from the UAS initiator port during a data-out command (see 6.3.4)
 * or a bi-directional command (see 6.3.7). UAS target devices that 
 * connect via SuperSpeed (see USB-3) shall return an ERDY transaction 
 * packet (see USB-3) on the Data-out pipe using the tag as the stream 
 * ID instead of the WRITE READY IU on the Status pipe. The WRITE READY 
 * IU shall be contained in a single USB packet and shall not share a 
 * USB packet with any other Information Unit. Table 15 defines the
 * WRITE READY IU.
 */

typedef struct usbUasWriteReadyIU
    {
    UINT8   uID;    /* IU ID, identifies the type of IU (see table 10) */
    UINT8   uRzv0;  /* Reserved */
    UINT16  uTAG;   /* The TAG field is in BIG ENDIAN */
    }USB_UAS_WRITE_READY_IU, *pUSB_UAS_WRITE_READY_IU;

/*
 * 6.2.5 SENSE IU
 *
 * The SENSE IU is sent by the UAS target port to deliver SCSI status.
 * The SENSE IU shall be contained in a single USB packet and shall
 * not share a USB packet with any other Information Unit. Table 16 
 * defines the SENSE IU.
 */
 
#define USB_SCSI_MAX_SENSE_LENGTH 96

typedef struct usbUasSenseIU
    {
    UINT8   uID;        /* IU ID, identifies the type of IU (see table 10) */
    UINT8   uRzv0;      /* Reserved */
    UINT16  uTAG;       /* The TAG field is in BIG ENDIAN */
#if (USB_MSC_UAS_VERSION_SUPPORTED >= 4)  
    UINT16  uQulifier;  /* STATUS QUALIFIER */  
#endif /* (USB_MSC_UAS_VERSION_SUPPORTED >= 4) */
    UINT8   uStatus;    /* STATUS */
#if (USB_MSC_UAS_VERSION_SUPPORTED >= 4)  
    UINT8   uRzv1[7];   /* Reserved */
#else /* (USB_MSC_UAS_VERSION_SUPPORTED >= 4) */
    UINT8   uRzv1[1];   /* Reserved */
#endif /* (USB_MSC_UAS_VERSION_SUPPORTED >= 4) */
    UINT16  uLength;    /* LENGTH field contains the number of bytes that
                         * follow in the SENSE IU. If no sense data is 
                         * available, then the LENGTH field shall be set
                         * to 0000h.*/
    UINT8  pSenseData[USB_SCSI_MAX_SENSE_LENGTH]; 
                        /* The SENSE DATA field shall be set to the sense 
                         * data, if any, for the command associated with 
                         * the tag (see SAM-4).*/
    }USB_UAS_SENSE_IU, *pUSB_UAS_SENSE_IU;

/*
 * 6.2.6 RESPONSE IU
 *
 * The RESPONSE IU is used to pass task management status information
 * from the UAS target port to the UAS initiator port. The RESPONSE IU 
 * may be returned in response to a COMMAND IU as a means to report an 
 * error condition detected by the transport. Each RESPONSE IU shall be 
 * sent in a single USB packet. The RESPONSE IU shall be contained in 
 * a single USB packet and shall not share a USB packet with any other
 * Information Unit. Table 17 defines the RESPONSE IU.
 */

typedef struct usbUasResponseIU
    {
    UINT8   uID;        /* IU ID, identifies the type of IU (see table 10) */
    UINT8   uRzv0;      /* Reserved */
    UINT16  uTAG;       /* The TAG field is in BIG ENDIAN */
    UINT16  uAddInfo;   /* ADDITIONAL RESPONSE INFORMATION for certain task
                         * management functions (e.g., QUERY ASYNCHRONOUS 
                         * EVENT). Shall be set to zero if the task management 
                         * function does not define, or the logical unit does
                         * not support response information. */
    UINT8   uResponse;  /* RESPONSE CODE */
    }USB_UAS_RESPONSE_IU, *pUSB_UAS_RESPONSE_IU;

/*
 * Table 18 - RESPONSE CODE field 
 * 
 * The RESPONSE CODE field indicates the status of a task management function.
 *
 * Values 01h, 03h, 06h..07h, 0Bh-FFh are Reserved
 */

#define USB_UAS_RESP_TASK_MGMT_COMPLETE     0x00 /* TASK MANAGEMENT FUNCTION COMPLETE */
#define USB_UAS_RESP_INVALID_INFO_UNIT      0x02 /* INVALID INFORMATION UNIT */
#define USB_UAS_RESP_TASK_MGMT_UNSUPPORTED  0x04 /* TASK MANAGEMENT FUNCTION NOT SUPPORTED */
#define USB_UAS_RESP_TASK_MGMT_FAILED       0x05 /* TASK MANAGEMENT FUNCTION FAILED */
#define USB_UAS_RESP_TASK_MGMT_SUCCEEDED    0x08 /* TASK MANAGEMENT FUNCTION SUCCEEDED */
#define USB_UAS_RESP_INCORRECT_LUN          0x09 /* INCORRECT LOGICAL UNIT NUMBER */
#define USB_UAS_RESP_OVERLAPPED_TAG         0x0A /* OVERLAPPED TAG ATTEMPTED */

/*
 * 6.2.7 TASK MANAGEMENT IU
 *
 * The TASK MANAGEMENT IU is sent by a UAS initiator port to request that
 * a task management function be processed by the task manager in a logical
 * unit. The TASK MANAGEMENT IU shall be contained in a single USB packet 
 * and shall not share a USB packet with any other Information Unit. Table 
 * 19 defines the TASK MANAGEMENT IU format.
 *
 * If the TASK MANAGEMENT FUNCTION field is set to 01h (i.e., ABORT TASK) 
 * or 80h (i.e., QUERY TASK) , then the TAG OF TASK TO BE MANAGED field 
 * specifies the tag from the COMMAND IU that contained the command to be
 * aborted or queried. For all other task management functions, the TAG 
 * OF TASK TO BE MANAGED field is reserved.
 *
 * If the TASK MANAGEMENT FUNCTION field contains a reserved or unsupported
 * value, then the task manager shall return a RESPONSE IU with the RESPONSE 
 * CODE field set to TASK MANAGEMENT FUNCTION NOT SUPPORTED.
 *
 * The LOGICAL UNIT NUMBER field contains the address of the logical unit. 
 * The structure of the LOGICAL UNIT NUMBER field shall be as defined in 
 * SAM-4. If the addressed logical unit does not exist, the task manager
 * shall return a RESPONSE IU with the RESPONSE CODE field set to INCORRECT 
 * LOGICAL UNIT NUMBER.
 */

typedef struct usbUasTaskMgmtIU
    {
    UINT8   uID;        /* IU ID, identifies the type of IU (see table 10) */
    UINT8   uRzv0;      /* Reserved */
    UINT16  uTAG;       /* The TAG field is in BIG ENDIAN */
    UINT8   uTaskMgmt;  /* TASK MANAGEMENT FUNCTION */
    UINT8   uRzv1;      /* Reserved */
    UINT16  uMgmtTAG;   /* TAG OF TASK TO BE MANAGED */
    UINT16  uLUN;       /* LOGICAL UNIT NUMBER */
    }USB_UAS_TASK_MGMT_IU, *pUSB_UAS_TASK_MGMT_IU;

#ifdef __cplusplus
}
#endif
#endif  /* __INCusbUash */

