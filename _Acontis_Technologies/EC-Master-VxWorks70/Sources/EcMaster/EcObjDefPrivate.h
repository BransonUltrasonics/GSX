/*-----------------------------------------------------------------------------
 * EcObjDefPrivate.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              Master Object Dictionary
 *---------------------------------------------------------------------------*/

#ifndef INC_ECOBJDEFPRIVATE
#define INC_ECOBJDEFPRIVATE

 /******************************************************************************************
 **
 ** ETG.1000.6 "Object Dictionary Structure"
 **
 ** 0x0000-0x0FFF: Data Type Area
 ** 0x1000-0x1FFF: CoE Commnucation Area
 ** 0x2000-0x5FFF: Manufacturer Specific Area
 ** 0x6000-0xFFFF: Profile Area
 */

/*-CONSTANTS-----------------------------------------------------------------*/
const EC_T_BYTE C_abySubindexDesc[] = "Subindex 000";

const EC_T_BYTE C_abyAddrListSubidxDesc1[] = "Conf Slave AddrList Object"; /* Max 64 Byte! */
const EC_T_BYTE C_abyAddrListSubidxDesc2[] = "Conn Slave AddrList Object"; /* Max 64 Byte! */

const EC_T_SDOINFOENTRYDESC S_oSubindex0EntryDesc = {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ | OBJACCESS_NOPDOMAPPING }; /* Subindex 000 */

const EC_T_WORD C_awBitMask[16] = 
{
    0x0000, 0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F,
    0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF
};

/******************************************************************************************
**
** Object 0x0800  ENUM
*/
EC_T_CHAR szEnum0800_Value00[] = "\000\000\000\000Signed presentation";   /* Value = 0x00, Text = Signed presentation */
EC_T_CHAR szEnum0800_Value01[] = "\001\000\000\000Unsigned presentation"; /* Value = 0x01, Text = Unsigned presentation */
EC_T_CHAR *apszEnum0800[2] = { szEnum0800_Value00, szEnum0800_Value01};

EC_T_SDOINFOENTRYDESC   S_aoEntryDesc0x0800[] =
{
    {DEFTYPE_UNSIGNED8, 8, ACCESS_READ | OBJACCESS_NOPDOMAPPING},                               /* Subindex 000 */
    {DEFTYPE_OCTETSTRING, 8*sizeof(szEnum0800_Value00), ACCESS_READ | OBJACCESS_NOPDOMAPPING},  /* Subindex 001 */
    {DEFTYPE_OCTETSTRING, 8*sizeof(szEnum0800_Value01), ACCESS_READ | OBJACCESS_NOPDOMAPPING},  /* Subindex 002 */
};

/******************************************************************************************
**
** Object 0x1000  Device Type
*/
EC_T_DWORD S_dwDevicetype = DEVICETYPE_ETHERCAT_MASTER; /* DeviceType Master */
EC_T_SDOINFOENTRYDESC   S_oEntryDesc0x1000 = 
{
    DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ | OBJACCESS_NOPDOMAPPING                              /* Subindex 000 */
};
EC_T_BYTE S_abyName0x1000[] = "Device type";

/******************************************************************************************
**
** Object 0x1008  Manufacturer Device Name
*/
EC_T_BYTE S_abyName0x1008[] = "Device name";
                                    
/******************************************************************************************
**
** Object 0x1009  Manufacturer Hardware Version
*/
EC_T_BYTE S_abyName0x1009[] = "Hardware version";

/******************************************************************************************
**
** Object 0x100A  Manufacturer Software Version
*/
EC_T_BYTE S_abyName0x100A[] = "Software version";

/******************************************************************************************
**
** Object 0x1018  Identity Object
*/
EC_T_OBJ1018 S_oIdentity = {4, EC_MASTER_VENDOR_ID, EC_MASTER_PRODUCT_CODE, ATECAT_VERSION, 0x00};

EC_T_SDOINFOENTRYDESC   S_aoEntryDesc0x1018[5] = 
{
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING },                     /* Subindex 000 */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING | OBJACCESS_FORCE },   /* Subindex 001: Vendor ID */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING | OBJACCESS_FORCE },   /* Subindex 002: Product code */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING | OBJACCESS_FORCE },   /* Subindex 003: Revision */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING | OBJACCESS_FORCE },   /* Subindex 004: Serial number */
};
EC_T_BYTE S_abyName0x1018[] = "Identity\000Vendor ID\000Product code\000Revision\000Serial number\000\377";

/******************************************************************************************
**
** Object 0x10F3  History Object
*/
#define OBJ10F3_MAX_SUBINDEX (5 + MAX_DIAG_MSG)
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x10F3[OBJ10F3_MAX_SUBINDEX + 1 /* Subindex 0 */] =
{
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},                      /* Subindex 000 */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},                      /* Subindex 001: MaxDiagMessages */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},                      /* Subindex 002: SubIndexMessageNew */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING},                 /* Subindex 003: SubIndexMessageAck */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},                      /* Subindex 004: NewDiagMessages */
    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING},                 /* Subindex 005: Flags */
};
EC_T_BYTE S_abyName0x10F3[]   = "History\000Maximum Messages\000Newest Message\000Newest Ack Message\000New Messages Available\000Flags\000Diagnosis Message\000\377";

/******************************************************************************************
**
** Object 0x2000  Master State change Command
*/
EC_T_SDOINFOENTRYDESC   S_oEntryDesc0x2000 = {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING};
EC_T_BYTE S_abyName0x2000[] = "Master State Change Command";

/******************************************************************************************
**
** Object 0x2001  Master State Summary
*/
EC_T_SDOINFOENTRYDESC   S_oEntryDesc0x2001 = {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ | OBJACCESS_NOPDOMAPPING};
EC_T_BYTE S_abyName0x2001[] = "Master State Summary";

/******************************************************************************************
**
** Object 0x2002  Bus Diagnosis Object
*/
#define OBJ2002_MAX_SUBINDEX 14
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x2002[] =
{
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: Reserved */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 002: Configuration Checksum */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 003: Number of Slaves found */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 004: Number of DC Slaves found */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 005: Number of Slaves in configuration */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 006: Number of Mailbox Slaves in configuration */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 007: TX Frames */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 008: RX Frames */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 009: Lost Frames */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 010: Cyclic Frames */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 011: Cyclic Datagrams */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 012: Acyclic Frames */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 013: Acyclic Datagrams */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING}, /* Subindex 014: Clear Counters */
};
EC_T_BYTE S_abyName0x2002[] = "Bus Diagnosis Object\000Reserved\000Configuration Checksum\000Slaves Found\000DC Slaves found\000Slaves in Config\000Mailbox Slaves in Config\000TX Frames\000RX Frames\000Lost Frames\000Cyclic Frames\000Cyclic Datagrams\000Acyclic Frames\000Acyclic Datagrams\000Counter Clear\000\377";

/******************************************************************************************
**
** Object 0x2003  Redundancy Diagnosis Object
*/
#define OBJ2003_MAX_SUBINDEX 4
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x2003[] =
{
  {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
  {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: Red Enabled */
  {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 002: Main Line Slave Count */
  {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 003: Red Line Slave Count */
  {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 004: Line Break Detected */
};
EC_T_BYTE S_abyName0x2003[] = "Redundancy Diagnosis Object\000Red Enabled\000Main Line Slave Count\000Red Line Slave Count\000Line Break Detected\000\377";

/******************************************************************************************
**
** Object 0x2004  Notification Counter Object
*/
#define OBJ2004_MAX_SUBINDEX (1 + NOTIFICATION_MEMBER_COUNT+MAX_NOTIFICATIONS)
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x2004[OBJ2004_MAX_SUBINDEX + 1 /* subindex 0 */] =
{
  {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
  {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: Maximum Messages */
  {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 002: Message Count */
  {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING}, /* Subindex 003: Flags */
};
EC_T_BYTE S_abyName0x2004[] = "Notification Counter Object\000Maximum Notifications\000Notification Count\000Flags\000\377";

/******************************************************************************************
**
** Object 0x2005  MAC Address Object
*/
#define OBJ2005_MAX_SUBINDEX 4
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x2005[5] =
{
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
    {DEFTYPE_UNSIGNED48,    0x30,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: Hardware MAC */
    {DEFTYPE_UNSIGNED48,    0x30,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 002: Redundant Hardware MAC */
    {DEFTYPE_UNSIGNED48,    0x30,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 003: Config Source MAC */
    {DEFTYPE_UNSIGNED48,    0x30,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 004: Config Destination MAC */
};
EC_T_BYTE S_abyName0x2005[] = "MAC Address Object\000Hardware MAC\000Redundant Hardware MAC\000Config Source MAC\000Config Destination MAC\000\377";

#if (defined INCLUDE_MAILBOX_STATISTICS)
/******************************************************************************************
**
** Object 0x2006  Mailbox Statistics Object
*/
#define OBJ2006_MAX_SUBINDEX 65
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x2006[] =
{
    { DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 000 */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 001: AoE Total Read Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 002: AoE Read Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 003: AoE Total Bytes Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 004: AoE Bytes Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 005: AoE Total Write Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 006: AoE Write Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 007: AoE Total Write Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 008: AoE Write Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 009: CoE Total Read Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 010: CoE Read Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 011: CoE Total Bytes Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 012: CoE Bytes Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 013: CoE Total Write Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 014: CoE Write Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 015: CoE Total Write Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 016: CoE Write Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 017: EoE Total Read Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 018: EoE Read Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 019: EoE Total Bytes Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 020: EoE Bytes Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 021: EoE Total Write Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 022: EoE Write Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 023: EoE Total Write Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 024: EoE Write Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 025: FoE Total Read Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 026: FoE Read Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 027: FoE Total Bytes Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 028: FoE Bytes Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 029: FoE Total Write Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 030: FoE Write Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 031: FoE Total Write Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 032: FoE Write Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 033: SoE Total Read Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 034: SoE Read Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 035: SoE Total Bytes Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 036: SoE Bytes Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 037: SoE Total Write Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 038: SoE Write Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 039: SoE Total Write Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 040: SoE Write Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 041: VoE Total Read Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 042: VoE Read Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 043: VoE Total Bytes Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 044: VoE Bytes Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 045: VoE Total Write Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 046: VoE Write Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 047: VoE Total Write Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 048: VoE Write Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 049: RawMbx Total Read Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 050: RawMbx Read Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 051: RawMbx Total Bytes Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 052: RawMbx Bytes Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 053: RawMbx Total Write Transfer Count */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 054: RawMbx Write Transfer Count Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 055: RawMbx Total Write Read */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 056: RawMbx Write Read Last Second */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 057: Reserved */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 058: Reserved */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 059: Reserved */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 060: Reserved */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 061: Reserved */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 062: Reserved */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 063: Reserved */
    { DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING },      /* Subindex 064: Reserved */
    { DEFTYPE_UNSIGNED64,    0x40,   ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING }, /* Subindex 065: Clear Counters */
};
EC_T_BYTE S_abyName0x2006[] = "Mailbox Statistics Object\000" \
    "AoE Total Read Transfer Count\000" \
    "AoE Read Transfer Count Last Second\000" \
    "AoE Total Read Transfer Count\000" \
    "AoE Read Transfer Count Last Second\000" \
    "AoE Total Write Transfer Count\000" \
    "AoE Write Transfer Count Last Second\000" \
    "AoE Total Write Read\000" \
    "AoE Write Read Last Second\000" \
    "CoE Total Read Transfer Count\000" \
    "CoE Read Transfer Count Last Second\000" \
    "CoE Total Bytes Read\000" \
    "CoE Bytes Read Last Second\000" \
    "CoE Total Write Transfer Count\000" \
    "CoE Write Transfer Count Last Second\000" \
    "CoE Total Write Read\000" \
    "CoE Write Read Last Second\000" \
    "EoE Total Read Transfer Count\000" \
    "EoE Read Transfer Count Last Second\000" \
    "EoE Total Bytes Read\000" \
    "EoE Bytes Read Last Second\000" \
    "EoE Total Write Transfer Count\000" \
    "EoE Write Transfer Count Last Second\000" \
    "EoE Total Write Read\000" \
    "EoE Write Read Last Second\000" \
    "FoE Total Read Transfer Count\000" \
    "FoE Read Transfer Count Last Second\000" \
    "FoE Total Bytes Read\000" \
    "FoE Bytes Read Last Second\000" \
    "FoE Total Write Transfer Count\000" \
    "FoE Write Transfer Count Last Second\000" \
    "FoE Total Write Read\000" \
    "FoE Write Read Last Second\000" \
    "SoE Total Read Transfer Count\000" \
    "SoE Read Transfer Count Last Second\000" \
    "SoE Total Bytes Read\000" \
    "SoE Bytes Read Last Second\000" \
    "SoE Total Write Transfer Count\000" \
    "SoE Write Transfer Count Last Second\000" \
    "SoE Total Write Read\000" \
    "SoE Write Read Last Second\000" \
    "VoE Total Read Transfer Count\000" \
    "VoE Read Transfer Count Last Second\000" \
    "VoE Total Bytes Read\000" \
    "VoE Bytes Read Last Second\000" \
    "VoE Total Write Transfer Count\000" \
    "VoE Write Transfer Count Last Second\000" \
    "VoE Total Write Read\000" \
    "VoE Write Read Last Second\000" \
    "RawMbx Total Read Transfer Count\000" \
    "RawMbx Read Transfer Count Last Second\000" \
    "RawMbx Total Bytes Read\000" \
    "RawMbx Bytes Read Last Second\000" \
    "RawMbx Total Write Transfer Count\000" \
    "RawMbx Write Transfer Count Last Second\000" \
    "RawMbx Total Write Read\000" \
    "RawMbx Write Read Last Second\000" \
    "Reserved\000" \
    "Reserved\000" \
    "Reserved\000" \
    "Reserved\000" \
    "Reserved\000" \
    "Reserved\000" \
    "Reserved\000" \
    "Reserved\000" \
    "Clear Counters\000\377";
#endif /* INCLUDE_MAILBOX_STATISTICS */

/******************************************************************************************
**
** Object 0x2010  Debug Register
*/
EC_T_SDOINFOENTRYDESC   S_oEntryDesc0x2010 = {DEFTYPE_UNSIGNED64, 0x40, ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING};
EC_T_BYTE S_abyName0x2010[] = "Debug Register";


/******************************************************************************************
**
** Object 0x2020  Master Initialization Parameters
*/
#define OBJ2020_MAX_SUBINDEX 16
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x2020[] =
{
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: Application Version */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 002: Master Version */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 003: Max Slaves Processed Per Cycle */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 004: EcatCmdTimeout */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 005: EcatCmdMaxRetries */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 006: Bus Cycle Time usec */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 007: EoE Timeout */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 008: FoE Busy Timeout */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 009: Max Queued ETH Frames */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 010: Max Slave CMD per frame */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 011: Max Queued CoE Slaves */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 012: Max Queued CoE CMDs */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 013: State change debug */
    {DEFTYPE_VISIBLESTRING, (MAX_DRIVER_IDENT_LEN*8),   
                                    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 014: LinkLayer Driver Ident */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 015: Polling Mode Active */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 016: Alloc Send Frame Active */
};
EC_T_BYTE S_abyName0x2020[] = "Master Initialization Parameters\000Application Version\000Master Version\000Max Slaves Processed Per Cycle\000" \
                                "CMD Timeout\000CMD Max Retries\000Bus Cycle Time usec\000EoE Timeout\000FoE Busy Timeout\000" \
                                "Max Queued ETH Frames\000Max Slave CMD per Frame\000Max Queued CoE Slaves\000Max Queue CoE CMDs\000" \
                                "State Change Debug\000Linklayer driver Ident\000Linklayer polling\000Linklayer use SendFrameAlloc\000\377";

/******************************************************************************************
**
** Object 0x2100  DC Deviation limit
*/
EC_T_SDOINFOENTRYDESC   S_oEntryDesc0x2100 = {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ | OBJACCESS_NOPDOMAPPING};
EC_T_BYTE S_abyName0x2100[] = "DC Deviation Limit";

/******************************************************************************************
**
** Object 0x2101  DC Current Deviation
*/
EC_T_SDOINFOENTRYDESC   S_oEntryDesc0x2101 = {DEFTYPE_INTEGER32, 0x20, ACCESS_READ | OBJACCESS_NOPDOMAPPING};
EC_T_BYTE S_abyName0x2101[] = "DC Current Deviation";

/******************************************************************************************
**
** Object 0x2102  DCM Bus Shift Object
*/
#define OBJ2102_MAX_SUBINDEX 7
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x2102[] =
{
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000             */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: dwErrorCode */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 002: bDcInSync   */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 003: bDcmInSync  */
    {DEFTYPE_INTEGER32,     0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 004: nCtlSetVal  */
    {DEFTYPE_INTEGER32,     0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 005: nCtlErrorFilt */
    {DEFTYPE_INTEGER32,     0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 006: nCtlErrorAvg */
    {DEFTYPE_INTEGER32,     0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 007: nCtlErrorMax */
};
EC_T_BYTE S_abyName0x2102[] = "DCM Bus Shift Object\000Error Code\000DC synchronized\000DCM controller synchronized\000" \
                                "Controller Set Value [nsec]\000Controller Error Filtered [nsec]\000Controller Error Average [nsec]\000" \
                                "Controller Error Maximum [nsec]\000\377";

/******************************************************************************************
**
** Object 0x2200  Bus Load Base Object
*/
#define OBJ2200_MAX_SUBINDEX 6
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x2200[15] =
{
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: TX bytes/second actual value */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 002: TX bytes/second min. value   */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 003: TX bytes/second max. value   */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 004: TX bytes/cycle actual value  */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 005: TX bytes/cycle min. value    */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 006: TX bytes/cycle max. value    */
};
EC_T_BYTE S_abyName0x2200[] = "Bus Load Base Object\000TX Bytes/sec act.\000TX Bytes/sec min.\000TX Bytes/sec max.\000" \
                                "TX Bytes/cycle act.\000TX Bytes/cycle min.\000TX Bytes/cycle max.\000\377";

/******************************************************************************************
**
** Object 0x3000 - 0x3fff Slave Info Objects
*/
#define OBJ3XXX_MAX_SUBINDEX 51
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x3xxx[] =
{
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: Entry Valid */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 002: Vendor ID */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 003: Product Code */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 004: Revision No */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 005: Serial No */
    {DEFTYPE_VISIBLESTRING, (MAX_SLAVE_DEVICENAME*8),   
                                    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 006: Device Name */
    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 007: Auto Inc Addr */
    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 008: Phys Addr */
    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 009: Config Phys Addr */
    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 010: Alias Addr */
    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 011: Port State */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 012: DC Support */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 013: DC64 Support */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 014: Mailbox Support */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING}, /* Subindex 015: Requested Slave State */

    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 016: Current Slave State */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 017: Error Flag set */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING}, /* Subindex 018: Link Messages Enable */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 019: Error Code */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 020: Sync Pulse Active */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 021: DC Sync 0 Period */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 022: DC Sync 1 Period */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 023: SB Error code */

    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 024: RX Error Counter Port 0 */
    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 025: RX Error Counter Port 1 */
    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 026: RX Error Counter Port 2 */
    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 027: RX Error Counter Port 3 */
    
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 028: Fwd Rx Error Counter Port 0 */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 029: Fwd Rx Error Counter Port 1 */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 030: Fwd Rx Error Counter Port 2 */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 031: Fwd Rx Error Counter Port 3 */
    
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 032: Ecat Processing Unit Error Counter */
    
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 033: PDI Error Counter */
    
    {DEFTYPE_UNSIGNED16,    0x10,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 034: Supported Mailbox Protocols */
    
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 035: Lost Link Counter Port 0 */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 036: Lost Link Counter Port 1 */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 037: Lost Link Counter Port 2 */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 038: Lost Link Counter Port 3 */

    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 039: Number of Supported FMMU entities */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 040: Number of Supported SyncManager entities */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 041: Size of RAM in kBytes */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 042: Port Descriptor */
    {DEFTYPE_UNSIGNED8,     0x8,    ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 043: ESC Type */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 044: Slave is Optional */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 045: Slave is present */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 046: Hot Connect Group Id */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 047: System Time Difference */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 048: Process data offset of input data (in Bits) */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 049: Process data size of input data (in Bits) */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 050: Process data offset of output data (in Bits) */
    {DEFTYPE_UNSIGNED32,    0x20,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 051: Process data size of output data (in Bits) */
};
EC_T_BYTE S_abyName0x3xxx[] = "Slave Info Object\000Entry Valid\000Vendor ID\000Product Code\000Revision No\000Serial No\000Device Name\000" \
                                "AutoInc Address\000Phys Address\000Config Phys Address\000Alias Address\000Port State\000DC Support\000" \
                                "DC 64Bit Support\000Mailbox Support\000Requested Slave State\000Current Slave State\000Error Flag\000Link Msg Enabled\000" \
                                "Error Code\000Sync Pulse Active\000DC Sync 0 Period\000DC Sync 1 Period\000Bus Topology Error\000" \
                                "Slave RX Error Counter P0\000Slave RX Error Counter P1\000Slave RX Error Counter P2\000Slave RX Error Counter P3\000" \
                                "Slave FWD RX Error Counter P0\000Slave FWD RX Error Counter P1\000Slave FWD RX Error Counter P2\000Slave FWD RX Error Counter P3\000" \
                                "Processing Unit Error Counter\000PDI Error Counter\000Supported Mailbox Protocols\000Lost Link Counter P0\000Lost Link Counter P1\000" \
                                "Lost Link Counter P2\000Lost Link Counter P3\000Supported FMMU entities\000Supported SyncManager entities\000" \
                                "Size of RAM in kByte\000Port Descriptor\000ESC Type\000Slave is Optional\000Slave is Present\000HotConnect Group Id\000System Time Difference\000" \
                                "Process Data Input Offset [Bit]\000Process Data Input Size [Bit]\000Process Data Output Offset [Bit]\000Process Data Output Size [Bit]\000\377";

/******************************************************************************************
**
** Modular Device Profiles: EtherCAT Master
** Object 0x8000 - 0x8FFF Expected configuration of the EtherCAT slaves
*/
#define OBJ8XXX_MAX_SUBINDEX 34
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x8xxx[] =
{
    {DEFTYPE_UNSIGNED8,        0x8,      ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
    {DEFTYPE_UNSIGNED16,       0x10,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: Fixed Station Address */
    {DEFTYPE_VISIBLESTRING,    64 * 8,   ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING}, /* Subindex 002: Type */
    {DEFTYPE_VISIBLESTRING,    64 * 8,   ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 003: Name */
    {DEFTYPE_UNSIGNED32,       0x20,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 004: Device Type */
    {DEFTYPE_UNSIGNED32,       0x20,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 005: Vendor ID */
    {DEFTYPE_UNSIGNED32,       0x20,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 006: Product Code */
    {DEFTYPE_UNSIGNED32,       0x20,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 007: Revision */
    {DEFTYPE_UNSIGNED32,       0x20,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 008: Serial */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 009: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 010: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 011: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 012: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 013: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 014: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 015: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 016: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 017: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 018: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 019: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 020: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 021: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 022: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 023: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 024: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 025: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 026: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 027: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 028: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 029: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 030: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 031: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 032: Fill */
    {DEFTYPE_UNSIGNED16,       0x10,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 033: Mailbox Out Size */
    {DEFTYPE_UNSIGNED16,       0x10,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 034: Mailbox In Size */
};
EC_T_BYTE S_abyName0x8xxx[] = 
"Slave Config Object\000"    /* Subindex 000 */
"Fixed Station Address\000"  /* Subindex 001 */
"Type\000"                   /* Subindex 002 */
"Name\000"                   /* Subindex 003 */
"DeviceType\000"             /* Subindex 004 */
"Vendor ID\000"              /* Subindex 005 */
"Product Code\000"           /* Subindex 006 */
"Revision\000"               /* Subindex 007 */
"Serial\000"                 /* Subindex 008 */
"\000"                       /* Subindex 009 */
"\000"                       /* Subindex 010 */
"\000"                       /* Subindex 011 */
"\000"                       /* Subindex 012 */
"\000"                       /* Subindex 013 */
"\000"                       /* Subindex 014 */
"\000"                       /* Subindex 015 */
"\000"                       /* Subindex 016 */
"\000"                       /* Subindex 017 */
"\000"                       /* Subindex 018 */
"\000"                       /* Subindex 019 */
"\000"                       /* Subindex 020 */
"\000"                       /* Subindex 021 */
"\000"                       /* Subindex 022 */
"\000"                       /* Subindex 023 */
"\000"                       /* Subindex 024 */
"\000"                       /* Subindex 025 */
"\000"                       /* Subindex 026 */
"\000"                       /* Subindex 027 */
"\000"                       /* Subindex 028 */
"\000"                       /* Subindex 029 */
"\000"                       /* Subindex 030 */
"\000"                       /* Subindex 031 */
"\000"                       /* Subindex 032 */
"Mailbox Out Size\000"       /* Subindex 033 */
"Mailbox In Size\000"        /* Subindex 034 */
;

/******************************************************************************************
** 
** Modular Device Profiles: EtherCAT Master
** Object 0x9000 - 0x9FFF Detected configuration of the EtherCAT slaves
*/
#define OBJ9XXX_MAX_SUBINDEX 32
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0x9xxx[] =
{
    {DEFTYPE_UNSIGNED8,        0x8,      ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
    {DEFTYPE_UNSIGNED16,       0x10,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: Fixed Station Address */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 002: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 003: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 004: Fill */
    {DEFTYPE_UNSIGNED32,       0x20,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 005: Vendor ID */
    {DEFTYPE_UNSIGNED32,       0x20,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 006: Product Code */
    {DEFTYPE_UNSIGNED32,       0x20,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 007: Revision */
    {DEFTYPE_UNSIGNED32,       0x20,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 008: Serial */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 009: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 010: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 011: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 012: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 013: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 014: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 015: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 016: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 017: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 018: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 019: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 020: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 021: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 022: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 023: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 024: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 025: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 026: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 027: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 028: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 029: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 030: Fill */
    {DEFTYPE_NULL,             0,        ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 031: Fill */
    {DEFTYPE_UNSIGNED16,       0x10,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 032: DL Status */
};
EC_T_BYTE S_abyName0x9xxx[] = 
"Slave Info Object\000"      /* Subindex 000 */
"Fixed Station Address\000"  /* Subindex 001 */
"\000"                       /* Subindex 002 */
"\000"                       /* Subindex 003 */
"\000"                       /* Subindex 004 */
"Vendor ID\000"              /* Subindex 005 */
"Product Code\000"           /* Subindex 006 */
"Revision\000"               /* Subindex 007 */
"Serial\000"                 /* Subindex 008 */
"\000"                       /* Subindex 009 */
"\000"                       /* Subindex 010 */
"\000"                       /* Subindex 011 */
"\000"                       /* Subindex 012 */
"\000"                       /* Subindex 013 */
"\000"                       /* Subindex 014 */
"\000"                       /* Subindex 015 */
"\000"                       /* Subindex 016 */
"\000"                       /* Subindex 017 */
"\000"                       /* Subindex 018 */
"\000"                       /* Subindex 019 */
"\000"                       /* Subindex 020 */
"\000"                       /* Subindex 021 */
"\000"                       /* Subindex 022 */
"\000"                       /* Subindex 023 */
"\000"                       /* Subindex 024 */
"\000"                       /* Subindex 025 */
"\000"                       /* Subindex 026 */
"\000"                       /* Subindex 027 */
"\000"                       /* Subindex 028 */
"\000"                       /* Subindex 029 */
"\000"                       /* Subindex 030 */
"\000"                       /* Subindex 031 */
"DL Status\000"              /* Subindex 032 */
;

/******************************************************************************************
** 
** Modular Device Profiles: EtherCAT Master
** Object 0xA000 - 0xAFFF Slave diagnosis
*/
#define OBJAXXX_MAX_SUBINDEX 2
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0xAxxx[] =
{
    {DEFTYPE_UNSIGNED8,        0x8,      ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
    {DEFTYPE_UNSIGNED16,       0x10,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: AL Status */
    {DEFTYPE_UNSIGNED16,       0x10,     ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING}, /* Subindex 002: AL Control */
};
EC_T_BYTE S_abyName0xAxxx[] = 
"Slave Diag Object\000"      /* Subindex 000 */
"AL Status\000"              /* Subindex 001 */
"AL Control\000"             /* Subindex 002 */
;

/******************************************************************************************
** 
** Modular Device Profiles: EtherCAT Master
** Object 0xF000  Modular device profile object
*/
#define OBJF000_MAX_SUBINDEX 4
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0xF000[] =
{
    {DEFTYPE_UNSIGNED8,        0x8,      ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
    {DEFTYPE_UNSIGNED16,       0x10,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 001: Index distance */
    {DEFTYPE_UNSIGNED16,       0x10,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 002: Maximum number of modules */
    {DEFTYPE_UNSIGNED32,       0x20,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 003: General configuration */
    {DEFTYPE_UNSIGNED32,       0x20,     ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 004: General information */
};
EC_T_BYTE S_abyName0xF000[] =
"Device Profile Object\000"  /* Subindex 000 */
"Index distance\000"         /* Subindex 001 */
"Maximum no of modules\000"  /* Subindex 002 */
"General configuration\000"  /* Subindex 003 */
"General information\000"    /* Subindex 004 */
;

/******************************************************************************************
** 
** Modular Device Profiles: EtherCAT Master
** Object 0xF002  detect modules command object
*/
#define OBJF002_MAX_SUBINDEX 3
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0xF002[] =
{
    {DEFTYPE_UNSIGNED8,        0x8,      ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 000 */
    {DEFTYPE_OCTETSTRING,      0x8 * 2,  ACCESS_READWRITE | OBJACCESS_NOPDOMAPPING}, /* Subindex 001: Command Request */
    {DEFTYPE_UNSIGNED8,        0x8,      ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 002: Command Status */
    {DEFTYPE_OCTETSTRING,      0x8 * 6,  ACCESS_READ | OBJACCESS_NOPDOMAPPING},      /* Subindex 003: Command Response */
};
EC_T_BYTE S_abyName0xF002[] =
"Detect Modules Cmd Object\000"  /* Subindex 000 */
"Command Request\000"            /* Subindex 001 */
"Command Status\000"             /* Subindex 002 */
"Command Response\000"           /* Subindex 003 */
;

/******************************************************************************************
** 
** Modular Device Profiles: EtherCAT Master
** Object 0xF02x  Configured address list
*/
EC_T_SDOINFOENTRYDESC S_aoEntryDesc0xF02x[256];
EC_T_BYTE S_abyName0xF02x[sizeof(C_abyAddrListSubidxDesc1) + 256];

/******************************************************************************************
** 
** Modular Device Profiles: EtherCAT Master
** Object 0xF04x  Detected address list
*/
#define S_aoEntryDesc0xF04x S_aoEntryDesc0xF02x // Safe 1500 Byte RAM
EC_T_BYTE S_abyName0xF04x[sizeof(C_abyAddrListSubidxDesc2) + 256];

#endif /* INC_ECOBJDEFPRIVATE */ 

/*-END OF SOURCE FILE--------------------------------------------------------*/
