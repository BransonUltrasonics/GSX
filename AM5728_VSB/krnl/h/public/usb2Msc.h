/* usb2Msc.h - USB Mass Storage Class Driver Public Header */

/*
 * Copyright (c) 2008-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01s,02jul13,wyy  Remove compiler warning (WIND00423315)
01r,23apr13,s_z  Remove IGNORE_CBI_INTERRUPT from USB2_MSC_DEV_FLAG_RELAXED
                 level (WIND00409416)
01q,17oct12,w_x  Fix compiler warnings (WIND00370525)
01p,31aug12,s_z  Describe usb2_msc_scsi_pass_through members (WIND00366758)
01o,25jul12,ljg  Fix coverity issue
                 match quirkyFlag and uUsb2MscQuirkyFlag (WIND00357082)
01n,19jul12,w_x  Add support for USB 3.0 host basic UAS (WIND00188662)
01m,06jul11,ghs  USB sticks coverage expansion (WIND00183500)
01l,21may10,pad  Fixed comment line.
01k,29apr10,pad  Moved extern C statement after include statements.
01j,18may10,w_x  Move some structures from usb2MscP.h to usb2Msc.h
01i,30mar10,w_x  Back port GEN2 class drivers for old VxWorks (WIND00192167)
                 Add USB2_MSC_DA_GET_SECTOR_SIZE support (WIND00192249)
01h,13jan10,ghs  vxWorks 6.9 LP64 adapting
01g,24sep09,w_x  Add READ FORMAT CAPACITIES Command support (WIND00183566)
01f,27aug08,w_x  Unify log mechanism and variable naming with some compiler
                 warning removal in USB GEN2 class drivers (WIND00178936)
01e,17jun09,w_x  correct USB2_MSC_SCSI_PASS_THROUGH timeout type (WIND00168950)
01d,14apr09,w_x  coding convention changes
01c,14apr09,w_x  remove warning for redefition for some adopted SCSI macros
01b,19jan09,w_x  clean up for first check in
01a,29oct08,w_x  written
*/

#ifndef __INCusb2Msch
#define __INCusb2Msch

#include <vxWorks.h>
#include <version.h>
#include <errnoLib.h>
#include <cacheLib.h>
#include <usb2Helper.h>
#include <blkIo.h> /* BLK_DEV */

#ifdef __cplusplus
extern "C" {
#endif



/* IOCTL commands for direct access module */

#define USB2_MSC_DA_START_COMPLIANCE_TEST               0x1001
#define USB2_MSC_DA_STOP_COMPLIANCE_TEST                0x1002
#define USB2_MSC_DA_SET_WRITE_OFFSET                    0x1003
#define USB2_MSC_DA_SET_READ_OFFSET                     0x1004
#define USB2_MSC_DA_SET_MEDIUM_CHANGE_CALLBACK          0x1005
#define USB2_MSC_DA_SCSI_PASS_THROUGH                   0x1006
#define USB2_MSC_DA_GET_SECTOR_SIZE                     0x1007

/* bInterfaceClass code */

#define USB2_MSC_CLASS_MASS_STORAGE 0x08 /* USB Mass Storage Class */

/* bInterfaceSubclass code */

#define USB2_MSC_SUBCLASS_RBC       0x01 /* Reduced Block Commands (RBC) */
#define USB2_MSC_SUBCLASS_SFF8020I  0x02 /* SFF-8020i, MMC-2 (ATAPI)     */
                                         /* (CD/DVD drives)              */
#define USB2_MSC_SUBCLASS_QIC157    0x03 /* QIC-157 (tape drives)        */
#define USB2_MSC_SUBCLASS_UFI       0x04 /* USB Floppy Interface (UFI)   */
#define USB2_MSC_SUBCLASS_SFF8070I  0x05 /* SFF-8070i (ATAPI removable   */
                                         /* media devices)               */
#define USB2_MSC_SUBCLASS_SCSI      0x06 /* SCSI transparent command set */
                                         /* (Use the SCSI INQUIRY        */
                                         /* command to determine the     */
                                         /* peripheral device type for   */
                                         /* most devices.)               */
#define USB2_MSC_SUBCLASS_RESERVED  0xFF /* Reserved                     */

/* bInterfaceProtocol code */

#define USB2_MSC_PROTOCOL_CBI       0x00  /* CBI with command            */
                                          /* completion interrupt        */
#define USB2_MSC_PROTOCOL_CB        0x01  /* CBI without command         */
                                          /* completion  interrupt       */
#define USB2_MSC_PROTOCOL_BBB       0x50  /* Bulk Only (Recommended      */
                                          /* for most devices)           */
#define USB2_MSC_PROTOCOL_RESERVED   0xFF /* Reserved                    */

/* SCSI version numbers */

#define USB2_MSC_SCSI_UNKNOWN           0
#define USB2_MSC_SCSI_1                 1
#define USB2_MSC_SCSI_1_CCS             2
#define USB2_MSC_SCSI_2                 3
#define USB2_MSC_SCSI_3                 4
#define USB2_MSC_SCSI_SPC_2             5
#define USB2_MSC_SCSI_SPC_3             6

/* Default timeout values (milliseconds) */

#define USB2_MSC_SCSI_READ_SECTOR_TIMEOUT        10000
#define USB2_MSC_SCSI_WRITE_SECTOR_TIMEOUT       10000
#define USB2_MSC_SCSI_COMMAND_TIMEOUT            5000

#define USB2_MSC_SCSI_STD_INQUIRY_LEN            36   /* Length of std INQUIRY data   */
#define USB2_MSC_SCSI_REQ_SENSE_ALLOC_LEN        18   /* Length of Req Sense data     */
#define USB2_MSC_SCSI_READ_CAP_LEN               8    /* Length of RD_CAP response    */
#define USB2_MSC_SCSI_MODE_SENSE_ALLOC_LEN       192  /* Mode Sense allocation length */

#define USB2_MSC_SCSI_READ_FORMAT_CAP_LEN        252  /* Length of RD_FORMAT_CAP response */

/* Macros copied and adopted from target/h/scsi2Lib.h - START */

/*
 * SCSI direct-access device and general purpose command opcodes are enumerated.
 * NOTE: Some commands are optional and may not be supported by all devices.
 */

/* Mandatory SCSI-2 commands */

#define USB2_MSC_SCSI_OPCODE_TEST_UNIT_READY      0x00
#define USB2_MSC_SCSI_OPCODE_REQUEST_SENSE        0x03
#define USB2_MSC_SCSI_OPCODE_INQUIRY              0x12
#define USB2_MSC_SCSI_OPCODE_SEND_DIAGNOSTIC      0x1D

/* Mandatory device-specific SCSI-2 commands */

#define USB2_MSC_SCSI_OPCODE_READ10               0x28
#define USB2_MSC_SCSI_OPCODE_READ_CAPACITY10      0x25
#define USB2_MSC_SCSI_OPCODE_READ_FORMAT_CAPACITIES 0x23

/* Optional device-specific SCSI-2 commands */

#define USB2_MSC_SCSI_OPCODE_MODE_SELECT6         0x15
#define USB2_MSC_SCSI_OPCODE_MODE_SENSE6          0x1A
#define USB2_MSC_SCSI_OPCODE_START_STOP_UNIT      0x1B
#define USB2_MSC_SCSI_OPCODE_WRITE10              0x2A
#define USB2_MSC_SCSI_OPCODE_MODE_SELECT10        0x55
#define USB2_MSC_SCSI_OPCODE_MODE_SENSE10         0x5A
#define USB2_MSC_SCSI_OPCODE_FORMAT_UNIT          0x04

/* ATAPI (CD-ROM) commands */

#define USB2_MSC_SCSI_OPCODE_CD_READ_TOC          0x43
#define USB2_MSC_SCSI_OPCODE_CD_PLAY10            0x45
#define USB2_MSC_SCSI_OPCODE_CD_PLAY_MSF          0x47
#define USB2_MSC_SCSI_OPCODE_CD_PAUSE_RESUME      0x4B
#define USB2_MSC_SCSI_OPCODE_CD_STOP              0x4E

/* Some other commands */

#define USB2_MSC_SCSI_OPCODE_READ12               0xA8
#define USB2_MSC_SCSI_OPCODE_WRITE12              0xAA
#define USB2_MSC_SCSI_OPCODE_PREVENT_ALLOW_MEDIUM_REMOVAL 0x1E
#define USB2_MSC_SCSI_OPCODE_READ_TOC_PMA_ATIP    0x43
#define USB2_MSC_SCSI_OPCODE_VERIFY10             0x2F
#define USB2_MSC_SCSI_OPCODE_SEND_DIAGNOSTIC      0x1D

/* Mode pages */

#define USB2_MSC_SCSI_MODE_PAGE_FLEXIBLE_DISK     0x05
#define USB2_MSC_SCSI_MODE_PAGE_CDROM             0x0D
#define USB2_MSC_SCSI_MODE_PAGE_CDROM_AUDIO       0x0E
#define USB2_MSC_SCSI_MODE_PAGE_CDROM_CAPS        0x2A
#define USB2_MSC_SCSI_MODE_PAGE_ALL               0x3F

/*
 * SCSI general purpose sense keys are enumerated.
 * NOTE: Some sense keys are optional and may not be supported by all devices.
 */

#define USB2_MSC_SCSI_SENSE_KEY_NO_SENSE          0x00  /* no sense sense key */
#define USB2_MSC_SCSI_SENSE_KEY_RECOVERED_ERROR   0x01  /* recovered error sense key */
#define USB2_MSC_SCSI_SENSE_KEY_NOT_READY         0x02  /* not ready sense key */
#define USB2_MSC_SCSI_SENSE_KEY_MEDIUM_ERROR      0x03  /* medium error sense key */
#define USB2_MSC_SCSI_SENSE_KEY_HARDWARE_ERROR    0x04  /* hardware error sense key */
#define USB2_MSC_SCSI_SENSE_KEY_ILLEGAL_REQUEST   0x05  /* illegal request sense key */
#define USB2_MSC_SCSI_SENSE_KEY_UNIT_ATTENTION    0x06  /* unit attention sense key */
#define USB2_MSC_SCSI_SENSE_KEY_DATA_PROTECT      0x07  /* data protect sense key */
#define USB2_MSC_SCSI_SENSE_KEY_BLANK_CHECK       0x08  /* blank check sense key */
#define USB2_MSC_SCSI_SENSE_KEY_VENDOR_UNIQUE     0x09  /* vendor unique sense key */
#define USB2_MSC_SCSI_SENSE_KEY_COPY_ABORTED      0x0A  /* copy aborted sense key */
#define USB2_MSC_SCSI_SENSE_KEY_ABORTED_COMMAND   0x0B  /* aborted command sense key */
#define USB2_MSC_SCSI_SENSE_KEY_EQUAL             0x0C  /* key equal sense key */
#define USB2_MSC_SCSI_SENSE_KEY_VOLUME_OVERFLOW   0x0D  /* volume overflow sense key */
#define USB2_MSC_SCSI_SENSE_KEY_MISCOMPARE        0x0E  /* miscompare sense key */
#define USB2_MSC_SCSI_SENSE_KEY_RESERVED          0x0F  /* reserved sense key */

/* SCSI additional sense codes */

#define USB2_MSC_SCSI_ASC_LUN_NOT_READY           0x04
#define USB2_MSC_SCSI_ASC_INVALID_COMMAND_FIELD   0x24
#define USB2_MSC_SCSI_ASC_WRITE_PROTECTED         0x27
#define USB2_MSC_SCSI_ASC_MEDIA_CHANGED           0x28
#define USB2_MSC_SCSI_ASC_RESET                   0x29
#define USB2_MSC_SCSI_ASC_COMMANDS_CLEARED        0x2F
#define USB2_MSC_SCSI_ASC_MEDIUM_NOT_PRESENT      0x3A

/* SCSI MODE header parameters */

#define USB2_MSC_SCSI_MODE_DEV_SPECIFIC_PARAM    0x2   /* device specific parameter */
#define USB2_MSC_SCSI_DEV_SPECIFIC_WP_MASK       0x80  /* Write protect bit         */

/* Byte offsets and bitmasks within the data returned by an INQUIRY command */

#define USB2_MSC_SCSI_INQUIRY_DEV_TYPE           0     /* device type and qualifier   */
#define USB2_MSC_SCSI_INQUIRY_DEV_MODIFIER       1     /* type modifier, removable    */
#define USB2_MSC_SCSI_INQUIRY_VERSION            2     /* ISO, ECMA and ANSI versions */
#define USB2_MSC_SCSI_INQUIRY_FORMAT             3     /* reponse format, etc.        */
#define USB2_MSC_SCSI_INQUIRY_ADD_LENGTH         4     /* additional length byte      */
#define USB2_MSC_SCSI_INQUIRY_FLAGS              7     /* miscellaneous flags (below) */
#define USB2_MSC_SCSI_INQUIRY_VENDOR_ID          8     /* vendor ID                   */
#define USB2_MSC_SCSI_INQUIRY_PRODUCT_ID         16    /* product ID                  */
#define USB2_MSC_SCSI_INQUIRY_REV_LEVEL          32    /* product revision level      */

#define USB2_MSC_SCSI_INQUIRY_DEV_TYPE_MASK      0x1f  /* mask for device type (byte 0) */

#define USB2_MSC_SCSI_INQUIRY_REMOVABLE_MASK     0x80  /* removable media flag (byte 1) */

#define USB2_MSC_SCSI_INQUIRY_ANSI_VSN_MASK      0x07  /* ANSI version         (byte 2) */

#define USB2_MSC_SCSI_INQUIRY_FORMAT_MASK        0x0f  /* response data format (byte 3) */

#define USB2_MSC_SCSI_INQUIRY_VENDOR_ID_LENGTH   8
#define USB2_MSC_SCSI_INQUIRY_PRODUCT_ID_LENGTH  16
#define USB2_MSC_SCSI_INQUIRY_REV_LEVEL_LENGTH   4

#define USB2_MSC_SCSI_INQUIRY_VENDOR_ID_OFFSET   8
#define USB2_MSC_SCSI_INQUIRY_PRODUCT_ID_OFFSET  16
#define USB2_MSC_SCSI_INQUIRY_REV_LEVEL_OFFSET   32

/* Some useful defines and structures for CCS commands */

#define USB2_MSC_SCSI_FORMAT_DATA_BIT    ((UINT8) 0x10)
#define USB2_MSC_SCSI_COMPLETE_LIST_BIT  ((UINT8) 0x08)

#define DEFAULT_INQUIRY_DATA_LENGTH     36
#define REQ_SENSE_ADD_LENGTH_BYTE       7
#define INQUIRY_ADD_LENGTH_BYTE         4
#define MODE_SENSE_ADD_LENGTH_BYTE      0

#define NON_EXT_SENSE_DATA_LENGTH       4

#define USB2_MSC_SCSI_SENSE_DATA_CLASS           0x70
#define USB2_MSC_SCSI_SENSE_DATA_CODE            0x0f

#define USB2_MSC_SCSI_SENSE_KEY_MASK             0x0f

#define USB2_MSC_SCSI_EXT_SENSE_CLASS            0x70
#define USB2_MSC_SCSI_EXT_SENSE_CODE             0x00

#define SPACE_CODE_DATABLK              0x00        /* space data blocks  */
#define SPACE_CODE_FILEMARK             0x01        /* space file-marks   */

/* Macros copied and adopted from target/h/scsi2Lib.h - END */

/* Command lengths */

#define USB2_MSC_SCSI_GROUP_0_CMD_LENGTH         6
#define USB2_MSC_SCSI_GROUP_1_CMD_LENGTH         10
#define USB2_MSC_SCSI_GROUP_5_CMD_LENGTH         12
#define USB2_MSC_CMDSET_CDB_SIZE6       USB2_MSC_SCSI_GROUP_0_CMD_LENGTH
#define USB2_MSC_CMDSET_CDB_SIZE10      USB2_MSC_SCSI_GROUP_1_CMD_LENGTH
#define USB2_MSC_CMDSET_CDB_SIZE12      USB2_MSC_SCSI_GROUP_5_CMD_LENGTH
#define USB2_MSC_CMDSET_UFI_CDB_SIZE    USB2_MSC_SCSI_GROUP_5_CMD_LENGTH
#define USB2_MSC_CMDSET_ATAPI_CDB_SIZE  USB2_MSC_SCSI_GROUP_5_CMD_LENGTH
#define USB2_MSC_CMDSET_MAX_CDB_SIZE    USB2_MSC_SCSI_GROUP_5_CMD_LENGTH

/* Device types */

#define USB2_MSC_SCSI_DEV_TYPE_MASK              0x1F
#define USB2_MSC_SCSI_DEV_TYPE_UNKNOWN           0x1F
#define USB2_MSC_SCSI_DEV_TYPE_DIRECT_ACCESS     0x00
#define USB2_MSC_SCSI_DEV_TYPE_SEQUENTIAL_ACCESS 0x01
#define USB2_MSC_SCSI_DEV_TYPE_PRINTER           0x02
#define USB2_MSC_SCSI_DEV_TYPE_PROCESSOR         0x03
#define USB2_MSC_SCSI_DEV_TYPE_WRITE_ONCE        0x04
#define USB2_MSC_SCSI_DEV_TYPE_CDROM             0x05
#define USB2_MSC_SCSI_DEV_TYPE_SCANNER           0x06
#define USB2_MSC_SCSI_DEV_TYPE_OPTICAL           0x07
#define USB2_MSC_SCSI_DEV_TYPE_MEDIUM_CHANGER    0x08
#define USB2_MSC_SCSI_DEV_TYPE_COMM              0x09
#define USB2_MSC_SCSI_DEV_TYPE_RAID              0x0C
#define USB2_MSC_SCSI_DEV_TYPE_ENCLOSURE         0x0D
#define USB2_MSC_SCSI_DEV_TYPE_RBC               0x0E

/* Medium types */

#define USB2_MSC_SCSI_MEDIUM_TYPE_UNKNOWN        0xFF

/*
 * SCSI Response is in BIG ENDIAN format.
 * Needs swapping on LITTLE ENDIAN platforms.
 */

#if (_BYTE_ORDER == _BIG_ENDIAN)
#define USB2_MSC_SCSI_SWAP_32(x) (x)
#define USB2_MSC_SCSI_SWAP_16(x) (x)
#else
#define USB2_MSC_SCSI_SWAP_32(x)  LONGSWAP((UINT)x)
#define USB2_MSC_SCSI_SWAP_16(x)  ((LSB(x) << 8)|MSB(x))
#endif

/* Maximum number of LUNs device supported */

#define USB2_MSC_MAX_LUN                        16

#define USB2_MAX_ALTSETTING                     128

#define USB2_MSC_MAX_LUN_CMD_BUFFER_LENGTH      16
#define USB2_MSC_MAX_LUN_DATA_BUFFER_LENGTH     256
#define USB2_MSC_MAX_WORKSPACE_LENGTH           64

/* Filesystem interface definitions */

#define USB2_MSC_FS_INTERFACE_XBD       0
#define USB2_MSC_FS_INTERFACE_BLK_DEV   1
#define USB2_MSC_FS_INTERFACE_DEFAULT   USB2_MSC_FS_INTERFACE_XBD

#define USB2_MSC_DATA_DIR_IN            0x80
#define USB2_MSC_DATA_DIR_OUT           0x00
#define USB2_MSC_DATA_DIR_NONE          0x01

/* USB transport status codes */

typedef enum usb2_msc_transport_status
    {
    USB2_MSC_TRANSPORT_SUCCESS = 0, /* Command has passed without problem     */
    USB2_MSC_TRANSPORT_FAILED,      /* Command has failed, want auto sense    */
    USB2_MSC_TRANSPORT_ERROR        /* Command has error, want reset recovery */
    } USB2_MSC_TRANSPORT_STATUS;

typedef struct usb2_msc_scsi_command_info
    {
    UINT8   bDataIn;          /* Is data transfer IN or OUT   */
    UINT8   lun;              /* LUN we are sending cmd to    */
    int     timeout;          /* Time out for this cmd        */
    UINT32  uTAG;             /* Command TAG */ 
    UINT16  uCmdLen;          /* Length of the cmd buffer     */
    UINT8 * pCmdBuffer;       /* CDB buffer data              */
    UINT32  uSnsLen;
    UINT8 * pSnsBuffer;
    }USB2_MSC_SCSI_COMMAND_INFO;

typedef struct usb2_msc_data_transfer_info
    {
    UINT32  uRequestedLendth; /* Requested Length to be transfered    */
    UINT32  uTAG;             /* DATA TAG */ 
    UINT32  uActualLength;    /* Number of bytes actually transferred */
    UINT8 * pDataBuffer;      /* Pointer to the data buffer or NULL   */
    }USB2_MSC_DATA_TRANSFER_INFO;

/*
 * This usb2_msc_scsi_pass_through structure include all related 
 * information needed in all stages for a complete mass storage bulk 
 * transaction loop.
 * 
 * CBW : Command Block Wapper
 *
 *  - <uCdbLength>          : The CDB length of the data in CDB buffer
 *  - <CDB[16]>             : CDB buffer store the CDB command
 *
 * DATA IN/OUT: Data IN/OUT/NONE stage 
 *
 *  - <bDataDirection>      : The DATA IN/OUT direction
 *                            - USB2_MSC_DATA_DIR_IN : DATA IN
 *                            - USB2_MSC_DATA_DIR_OUT : DATA OUT
 *                            - USB2_MSC_DATA_DIR_NONE : NO DATA Stage
 *  - <uDataTransferLength> : The data length to be transfer,
 *  - <pDataBuffer>         : Buffer used to store data in DATA stage
 *
 * CSW : Command Status Wapper
 * 
 *  - <uSenseDataLength>    : the needed sense data langth
 *  - <pSensDataBuffer>     : Buffer used to store sence data
 * 
 * Misc: Other needed paramenter for the transaction
 *
 * <timeout>                : The timeout value
 */

typedef struct usb2_msc_scsi_pass_through
    {
    UINT8   uCdbLength;             /* CDB length (e.g. 6, 10 or 12)          */
    UINT8   CDB[16];                /* CDB buffer                             */
    UINT8   bDataDirection;         /* Data Direction                         */
    UINT32  uDataTransferLength;    /* Data transfer length                   */
    UINT8 * pDataBuffer;            /* Data buffer                            */
    UINT8   uSenseDataLength;       /* Sense data length                      */
    UINT8 * pSensDataBuffer;        /* Sense allocation area                  */
    UINT8   deviceStatus;           /* Device status                          */
    int     timeout;                /* Timeout value                          */
    }USB2_MSC_SCSI_PASS_THROUGH;

/* Command Block Wrapper */

typedef struct usb2MscCbw
   {
   UINT32    dCBWSignature;              /* CBW Signature                  */
   UINT32    dCBWTag;                    /* Tag field                      */
   UINT32    dCBWDataTransferLength;     /* Size of data (bytes)           */
   UINT8     bmCBWFlags;                 /* direction bit                  */
   UINT8     bCBWLUN;                    /* Logical Unit Number            */
   UINT8     bCBWCBLength;               /* Length of command block        */
   UINT8     CBWCB [16];                 /* buffer for command block       */
   } WRS_PACK_ALIGN(1) USB2_MSC_CBW;

/* Command Status Wrapper */

typedef struct usb2MscCsw
   {
   UINT32    dCSWSignature;              /* CBW Signature                  */
   UINT32    dCSWTag;                    /* Tag field                      */
   UINT32    dCSWDataResidue;            /* Size of residual data (bytes)  */
   UINT8     bCSWStatus;                 /* buffer for command block       */
   } WRS_PACK_ALIGN(1) USB2_MSC_CSW;

typedef struct usb2_msc_device
   {
   /* Class device specific information */

   USB2_CLASS_DEVICE *     pClassDevice;   /* GEN2 Class device information */
   USBD_DEVICE_INFO *      pDeviceInfo;    /* USBD device information       */
   UINT8                   uSpeed;         /* Device speed reported by USBD */
   UINT32                  uMaxStreams;    /* Max Streams usable for UAS    */
   BOOL                    ioReady;        /* Boolean for ready for I/O     */
   BOOL                    deviceRemoved;  /* Boolean for device removed    */
   int                     resetTimeout;   /* Device reset command timeout  */
   int                     commandTimeout; /* Device normal command timeout */
   int                     statusTimeout;  /* Device status timeout         */
   SEM_ID                  bulkOutSem;     /* Binary semaphpre for BULK OUT */
   SEM_ID                  bulkInSem;      /* Binary semaphpre for BULK IN  */
   SEM_ID                  stsInSem;       /* Binary semaphpre for STS IN   */
   SEM_ID                  cmdOutSem;      /* Binary semaphpre for CMD OUT  */
   USBHST_URB              bulkOutUrb;     /* URB for BULK OUT endpoint     */
   USBHST_URB              bulkInUrb;      /* URB for BULK IN endpoint      */
   USBHST_URB              stsInUrb;       /* URB for INT IN endpoint       */
   USBHST_URB              cmdOutUrb;      /* URB for CMD OUT endpoint      */
   UINT32                  uBulkOutBufferSize; /* Size of BULK OUT transfer */
   UINT32                  uBulkInBufferSize;  /* Size of BULK IN transfer  */
   UINT32                  uStsInBufferSize;   /* Size of INT IN transfer   */
   UINT32                  uCmdOutBufferSize;  /* Size of CMD OUT transfer  */
   UINT8                   bBulkOutEndPointAddr; /* BULK OUT EP address     */
   UINT8                   bBulkInEndPointAddr;  /* BULK IN EP address      */
   UINT8                   bStsInEndPointAddr;   /* Status IN EP address    */
   UINT8                   bCmdOutEndPointAddr;  /* Command OUT EP address  */

   struct usb2_msc_quirky_info * pQuirky;        /* Quirky device entry     */
   UINT32                  quirkyFlag;           /* Dynamical quirky flags  */
   UINT8                   subClassCode;         /* Device subclass code    */
   UINT8                   protocolCode;         /* Device protocol code    */
   UINT8                   interfaceNum;         /* Device interface number */
   UINT8                   curAltSetting;        /* Alternate setting       */
   UINT8                   maxLun;               /* Total number of LUNs    */
   UINT32                  curCBWTag;            /* CBW dCBWTag increased   */
                                                 /* for every command       */
   UINT32                  dCSWSignatureCompare; /* value to be compared    */
                                                 /* with CSW dCSWSignature  */
   UINT32                  dCSWDataResidueLast;  /* To save last residue    */
   UINT8 *                 pWorkspace;           /* Short transfer buffer   */
   UINT16                  endpointStatus;       /* Avoid allocate on stack */
   ULONG                   hUsrConfig;           /* Handle to user context  */
   ULONG                   hDirectAccess;        /* Handle to Direct Access */
   ULONG                   hCompliance;          /* Handle to compliance tst*/
   ULONG                   hLunDevice[USB2_MSC_MAX_LUN]; /* Handle for LUN  */
   FUNCPTR                 mediumChangeCallback; /* Medium Change callback  */
   void *                  pFurtherExt;          /* For further extention   */
   } USB2_MSC_DEVICE;

typedef struct usb2_msc_lun_device
   {
   USB2_MSC_DEVICE *   pMscDevice;         /* Parent represent the device   */
   UINT8               lun;                /* LUN that we represent         */
   SEM_ID              lunMutex;           /* Access lock                   */
   int                 readSectorTimeout;  /* Sector read timeout           */
   int                 writeSectorTimeout; /* Sector read timeout           */
   int                 scsiCommandTimeout; /* Sector read timeout           */
   sector_t            totalSectors;       /* Total number of sectors       */
   unsigned            bytesPerSector;     /* Size in bytes per sector      */
   UINT64              totalCapacityKB;    /* Total capacity in KB          */
   UINT8               scsiLevel;          /* SCSI level of the device      */
   UINT8               scsiPDT;            /* SCSI Peripheral Device Type   */
   UINT8               scsiPQ;             /* SCSI Peripheral Qualifier     */
   UINT8               mediumType;         /* SCSI medium type              */
   BOOL                mediumRemovable;    /* Removable media bit (RMB)     */
   BOOL                mediumPresent;      /* A medium is present           */
   BOOL                mediumMounted;      /* Is this mounted by filesystem */
   BOOL                writeProtected;     /* Medium/device write-protected */
   BOOL                removing;           /* Device marked for removal     */
   BOOL                busy;               /* Device marked as busy         */
   BOOL                mediumChanged;      /* Medium has changed            */
   BOOL                readyChanged;       /* Ready changed                 */
   UINT8 *             pCmdBuffer;         /* Command buffer                */
   UINT8 *             pDataBuffer;        /* Driver internal data buffer   */
   VOID *              pFileSystemHook;

   /* Device personality, add 1 byte to allow fixup */

   char                deviceVIDString [ USB2_MSC_SCSI_INQUIRY_VENDOR_ID_LENGTH + 1 ];
   char                devicePIDString [ USB2_MSC_SCSI_INQUIRY_PRODUCT_ID_LENGTH + 1 ];
   char                deviceREVString [ USB2_MSC_SCSI_INQUIRY_REV_LEVEL_LENGTH + 1 ];

   /* Sense data related */

   UINT8               bSenseKey;          /* Sense Key information         */
   UINT8               bASC;               /* Additional Sense Code         */
   UINT8               bASCQ;              /* ASC Qualifier                 */
   BOOL                bSenseValid;        /* Is the last sense data valid  */
   UINT8               bSenseDataLength;   /* Sense data length             */
   UINT8 *             pSenseBuffer;       /* Buffer of sense data          */
   }USB2_MSC_LUN_DEVICE;

   /*
    * The BOOL return value indicates if the driver
    * can continue handle medium hotplug events normally.
    */

typedef BOOL (*USB2_MSC_MEDIUM_CHANGE_FUNCPTR)
    (
    UINT32 hDevice,         /* Device handle for the device      */
    UINT8 lun,              /* LUN of the medium change          */
    BOOL  mediumInsert      /* Is this a medium insert or remove */
    );

typedef void (*USB2_MSC_SETUP_FUNCPTR)
    (
    UINT32 hDevice,         /* Device handle for the device */
    void *pQuirky           /* Pointer to quirky info below */
    );

typedef STATUS (*USB2_MSC_MOUNT_FUNCPTR)
    (
    struct usb2_msc_device * pMscDevice    /* device for mounting */
    );

typedef STATUS (*USB2_MSC_UNMOUNT_FUNCPTR)
    (
    struct usb2_msc_device * pMscDevice    /* device for unmounting */
    );

typedef struct usb2MscPrivateFuncs
    {
    USB2_MSC_SETUP_FUNCPTR setup;   /* setup function ptr */
    USB2_MSC_MOUNT_FUNCPTR mount;   /* mount function ptr */
    USB2_MSC_UNMOUNT_FUNCPTR unmount; /* unmount function ptr */
    } USB2_MSC_PRIVATE_FUNCS;

typedef struct usb2_msc_quirky_info
    {
    USBHST_DEVICE_ID devId;         /* Device ID info for match */
    const char * deviceName;        /* Quirky device name */
    UINT8 forceSubclass;            /* force sub class code */
    UINT8 forceProtocol;            /* force protocol code */
    UINT32 quirkyFlag;              /* quirky flags */
    USB2_MSC_PRIVATE_FUNCS funcs;   /* private function pointers */
    }USB2_MSC_QUIRKY_INFO;

#define USB2_MSC_QUIRKY(vendorId,productId,driverBCDLow,driverBCDHigh,  \
            deviceName, subclass,protocol,setup,mount,unmount,flags)    \
    {                                                                   \
        {                                                               \
        {NULL}, 0,                                                      \
        vendorId, productId, driverBCDLow, driverBCDHigh, 0,            \
        0, 0, 0,                                                        \
        0, 0, 0,                                                        \
        USBD_MATCH_ID_VENDORID | USBD_MATCH_ID_PRODUCTID,               \
        NULL                                                            \
        },                                                              \
    deviceName,                                                         \
    subclass,                                                           \
    protocol,                                                           \
    flags,                                                              \
    {setup, mount, unmount},                                            \
    }

/* enable or disable quirky device support for compile */
#define USB2_MSC_QUIRKY_DEV_SUPPORT

#ifdef USB2_MSC_QUIRKY_DEV_SUPPORT
/* quirky device flags */
#define USB2_MSC_DEV_FLAG_ODD_SECTORS                   (1UL << 0)
#define USB2_MSC_DEV_FLAG_IGNORE_DATA_RESIDUE           (1UL << 1)
#define USB2_MSC_DEV_FLAG_MAX_64_BLOCKS_TRANSFER        (1UL << 2)
#define USB2_MSC_DEV_FLAG_NO_MS_WP_CHECK                (1UL << 3)
#define USB2_MSC_DEV_FLAG_SINGLE_LUN                    (1UL << 4)
#define USB2_MSC_DEV_FLAG_IGNORE_CBI_INTERRUPT          (1UL << 5)
#define USB2_MSC_DEV_FLAG_UNABLE_PREVENT_REMOVAL        (1UL << 6)
#define USB2_MSC_DEV_FLAG_UNMACTHED_CSW_TAG             (1UL << 7)
#define USB2_MSC_DEV_FLAG_DELAY_AFTER_CBW               (1UL << 8)
#define USB2_MSC_DEV_FLAG_BBB_USE_32_BYTES_CBW          (1UL << 9)
#define USB2_MSC_DEV_FLAG_DUMMY_INQUIRY                 (1UL << 10)
#define USB2_MSC_DEV_FLAG_IGNORE_DEVICE                 (1UL << 11)

/* overwrite function flag should be the highest bit */
#define USB2_MSC_DEV_FLAG_DRIVER_OVERWRITE              (1UL << 31)

/* 
 * three modes defined for using global flags:
 * Strict Mode: will disable all quirky support except defined in quirky list,
 *              may have compatible issues, not recommended.
 * Relaxed Mode: Enable generic quirky support compatible with normal devices,
 *                  recommended use this mode.
 * All Flags Mode: Enable all quirky support(except ignore device flag and 
 *                 overwrite flag), may cause losing performance, not recommended
 *                 unless to resolve compatible issue.
 */

#define USB2_MSC_DEV_FLAG_STRICT        0
#define USB2_MSC_DEV_FLAG_RELAXED       USB2_MSC_DEV_FLAG_ODD_SECTORS \
                                        | USB2_MSC_DEV_FLAG_IGNORE_DATA_RESIDUE \
                                        | USB2_MSC_DEV_FLAG_MAX_64_BLOCKS_TRANSFER \
                                        | USB2_MSC_DEV_FLAG_NO_MS_WP_CHECK \
                                        | USB2_MSC_DEV_FLAG_SINGLE_LUN \
                                        | USB2_MSC_DEV_FLAG_UNABLE_PREVENT_REMOVAL

#define USB2_MSC_DEV_FLAG_ALL           USB2_MSC_DEV_FLAG_ODD_SECTORS \
                                        | USB2_MSC_DEV_FLAG_IGNORE_DATA_RESIDUE \
                                        | USB2_MSC_DEV_FLAG_MAX_64_BLOCKS_TRANSFER \
                                        | USB2_MSC_DEV_FLAG_NO_MS_WP_CHECK \
                                        | USB2_MSC_DEV_FLAG_SINGLE_LUN \
                                        | USB2_MSC_DEV_FLAG_IGNORE_CBI_INTERRUPT \
                                        | USB2_MSC_DEV_FLAG_UNABLE_PREVENT_REMOVAL \
                                        | USB2_MSC_DEV_FLAG_UNMACTHED_CSW_TAG \
                                        | USB2_MSC_DEV_FLAG_DELAY_AFTER_CBW \
                                        | USB2_MSC_DEV_FLAG_BBB_USE_32_BYTES_CBW \
                                        | USB2_MSC_DEV_FLAG_DUMMY_INQUIRY \

/* macro to check a quirky flag */

#define USB2_MSC_QUIRKY_CHK(pMscDevice,FLAG) \
        (((pMscDevice)->pQuirky != NULL) ? \
            (((pMscDevice)->quirkyFlag & (USB2_MSC_DEV_FLAG_##FLAG)) ? TRUE : FALSE) : \
            ((uUsb2MscQuirkyFlag & (USB2_MSC_DEV_FLAG_##FLAG)) ? TRUE : FALSE))  \

#endif
/*
 * Exported functions
 */

IMPORT STATUS usb2MscDrvInit
    (
    char *  pName
    );

IMPORT STATUS usb2MscDrvUnInit
    (
    void
    );

#ifdef USB2_MSC_QUIRKY_DEV_SUPPORT
/* function for match/add/delete quirky device to quirky list */
IMPORT USB2_MSC_QUIRKY_INFO * usb2MscQuirkyMatch
    (
    UINT16 uVendorID,
    UINT16 uProductID,
    UINT16 uBCDDevice
    );

IMPORT STATUS usb2MscQuirkyAdd
    (
    USB2_MSC_QUIRKY_INFO *  pQuirky,
    UINT32                  members
    );

IMPORT STATUS usb2MscQuirkyDel
    (
    USB2_MSC_QUIRKY_INFO *  pQuirky
    );
#endif

IMPORT UINT8 usrUsb2MscFilesystemInterfaceGet
    (
    void
    );

IMPORT BOOL usrUsb2MscDirectAccessEnabled
    (
    void
    );

IMPORT BOOL usrUsb2MscComplianceEnabled
    (
    void
    );

IMPORT BOOL usrUsb2MscMediumHotplugCheckEabled
    (
    void
    );

IMPORT int usrUsb2MscMediumHotplugCheckIntervalGet
    (
    void
    );

IMPORT int usrUsb2MscResetTimeoutGet
    (
    void
    );

IMPORT int usrUsb2MscCommandTimeoutGet
    (
    void
    );

IMPORT int usrUsb2MscStatusTimeoutGet
    (
    void
    );

IMPORT BOOL usrUsb2MscStatusCheckEnabled
    (
    void
    );

IMPORT BOOL usrUsb2MscWriteProtectionCheckEnabled
    (
    void
    );

IMPORT char * usrUsb2MscDrvNameGet
    (
    ULONG   hUsrContext,
    UINT8   lunIndex
    );

IMPORT char * usrUsb2MscServiceTaskNameGet
    (
    ULONG   hUsrContext,
    UINT8   lun
    );

IMPORT STATUS usrUsb2MscCbioDevMount
    (
    UINT32          hUsrContext,
    UINT8           lun,
    BLK_DEV *       pBlkDev,
    const char *    pName
    );

IMPORT STATUS usrUsb2MscCbioDevUnmount
    (
    ULONG           hUsrContext,
    UINT8           lun,
    const char *    pName
    );

IMPORT BOOL usrUsb2MscComplianceEnabled
    (
    void
    );

IMPORT FILE * usrUsb2MscComplianceTestLogFileOpen
    (
    UINT32 hDevice
    );

IMPORT void usrUsb2MscComplianceTestLogFileClose
    (
    FILE * file
    );

IMPORT STATUS usrUsb2MscReserveDevice
    (
    UINT32      hDevice,
    ULONG *    pUsrContext
    );

IMPORT STATUS usrUsb2MscUnreserveDevice
    (
    UINT32 hDevice
    );

IMPORT USB2_MSC_QUIRKY_INFO * usrUsb2MscLookupQuirkyInfo
    (
    UINT16  uVendorID,
    UINT16  uDeviceID,
    UINT16  uBCDDevice
    );

IMPORT BOOL usrUsb2MscDefaultMediumChangeCallback
    (
    UINT32  hDevice,
    UINT8   lun,
    BOOL    mediumInsert
    );

IMPORT int usrUsb2MscMaxBlocksPerTransferGet
    (
    void
    );

IMPORT int usrUsb2MscServiceTaskPriorityGet
    (
    void
    );

IMPORT int usrUsb2MscServiceTaskOptionsGet
    (
    void
    );

IMPORT int usrUsb2MscServiceTaskStackSizeGet
    (
    void
    );

IMPORT BOOL usrUsb2MscCBItransportEnabled
    (
    void
    );

IMPORT STATUS usrUsb2MscDeviceMount
    (
    USB2_MSC_LUN_DEVICE *           pMscLunDevice
    );

IMPORT STATUS usrUsb2MscDeviceUnmount
    (
    USB2_MSC_LUN_DEVICE *           pMscLunDevice
    );

IMPORT STATUS usrUsb2MscDirectAccessInit
    (
    void
    );

IMPORT STATUS usrUsb2MscDirectAccessUnInit
    (
    void
    );

IMPORT STATUS usrUsb2MscDirectAccessDeviceAdd
    (
    USB2_MSC_DEVICE *               pMscDevice
    );

IMPORT STATUS usrUsb2MscDirectAccessDeviceRemove
    (
    USB2_MSC_DEVICE *               pMscDevice
    );


IMPORT STATUS usrUsb2MscComplianceTestDeviceAdd
    (
    USB2_MSC_DEVICE *               pMscDevice,
    UINT8                           lun
    );

IMPORT STATUS usrUsb2MscComplianceTestDeviceRemove
    (
    USB2_MSC_DEVICE *               pMscDevice,
    UINT8                           lun
    );

IMPORT USB2_MSC_TRANSPORT_STATUS usrUsb2MscCBITransport
    (
    USB2_MSC_DEVICE *               pMscDevice,
    USB2_MSC_SCSI_COMMAND_INFO *    pCmdInfo,
    USB2_MSC_DATA_TRANSFER_INFO *   pDataInfo
    );

IMPORT USB2_MSC_TRANSPORT_STATUS usb2MscUASTransportUSB2
    (
    USB2_MSC_DEVICE *               pMscDevice, 
    USB2_MSC_SCSI_COMMAND_INFO *    pCmdInfo,   
    USB2_MSC_DATA_TRANSFER_INFO *   pDataInfo   
    );

IMPORT USB2_MSC_TRANSPORT_STATUS usb2MscUASTransportUSB3
    (
    USB2_MSC_DEVICE *               pMscDevice,
    USB2_MSC_SCSI_COMMAND_INFO *    pCmdInfo,
    USB2_MSC_DATA_TRANSFER_INFO *   pDataInfo
    );

/******* usb2MscXBD.c exported routines - START *****/

IMPORT STATUS usb2MscXbdMount
    (
    USB2_MSC_LUN_DEVICE *           pMscLunDevice
    )
;

IMPORT STATUS usb2MscXbdUnmount
    (
    USB2_MSC_LUN_DEVICE *           pMscLunDevice
    );

/******* usb2MscXBD.c exported routines - END *****/

/******* usb2MscBLK.c exported routines - START *****/

IMPORT STATUS usb2MscBlkDevMount
    (
    USB2_MSC_LUN_DEVICE *           pMscLunDevice
    );

IMPORT STATUS usb2MscBlkDevUnmount
    (
    USB2_MSC_LUN_DEVICE *           pMscLunDevice
    );
/******* usb2MscBLK.c exported routines - END *****/

/******* usb2MscDirectAccess.c exported routines - START *****/

STATUS usb2MscDirectAccessInit
    (
    void
    );

STATUS usb2MscDirectAccessUnInit
    (
    void
    );

STATUS usb2MscDirectAccessDeviceAdd
    (
    USB2_MSC_DEVICE *               pMscDevice
    );

STATUS usb2MscDirectAccessDeviceRemove
    (
    USB2_MSC_DEVICE *               pMscDevice
    );

/******* usb2MscDirectAccess.c exported routines - END *****/

/******* usb2MscCompliance.c exported routines - START *****/

STATUS usb2MscComplianceTestDeviceAdd
    (
    USB2_MSC_DEVICE *               pMscDevice,
    UINT8                           lun
    );

STATUS usb2MscComplianceTestDeviceRemove
    (
    USB2_MSC_DEVICE *               pMscDevice,
    UINT8                           lun
    );

/******* usb2MscCompliance.c exported routines - END *****/

/******* usb2MscCBI.c exported routines - START *****/

USB2_MSC_TRANSPORT_STATUS usb2MscCBITransport
    (
    USB2_MSC_DEVICE *               pMscDevice,
    USB2_MSC_SCSI_COMMAND_INFO *    pCmdInfo,
    USB2_MSC_DATA_TRANSFER_INFO *   pDataInfo
    );

/******* usb2MscCBI.c exported routines - END *****/

#ifdef __cplusplus
}
#endif
#endif  /* __INCusb2Msch */

