/*
 * firmwareInfo.h
 *
 *  Created on: Mar 19, 2018
 *      Author: rpsharma
 */

#ifndef H_FIRMWAREINFO_H_
#define H_FIRMWAREINFO_H_
#include <vxworks.h>

char 	*FirmwareUpgradeFileBuffer;
UINT32 	FirmwareUpgradeFileBufferLenght;


typedef struct {
	int bootMode;
	int fwUpgrade;
	int activePart;
	int passivePart;
} FwInfo;

typedef struct 
{
	volatile    UINT32	boot_status;
	volatile    UINT32	fw_upgrade;
	volatile    UINT32	partition_a_offset;
	volatile    UINT32	partition_b_offset;	 
	volatile    UINT32	partition_a_size;
	volatile    UINT32	partition_b_size;
	volatile    UINT16	partition_a_crc;
	volatile    UINT16	partition_b_crc;
	volatile    UINT16	emmc_copy;
} FwInfoCRC;

typedef struct 
{	
	UINT16 	FW_CRC_Checksum;
	UINT32 	FW_Length;
	UINT32  FW_FileStartAddress;

}
FirmWareUpgradeInfo;
/** Memory size 1 byte */
#define FOEMEM_SIZE_1B					(1U)
/** Memory size 1 kilo-byte */
#define FOEMEM_SIZE_KB					(1024U * FOEMEM_SIZE_1B)
/** Memory size 1 kilo-byte */
#define FOEMEM_SIZE_MB					(1024U * FOEMEM_SIZE_KB)

/* Max size of file downloaded through FOE */
#define FOE_DOWNLOAD_MAX_SIZE			(10U * FOEMEM_SIZE_MB )



#define BRANSON_ACTIVE_BOOTMODE 	0X00
#define BRANSON_PASSIVE_BOOTMODE  	0X01

#define BRANSON_NORMAL_BOOT		 	0X00
#define BRANSON_FWUPGRADE		 	0X01


/*Allocated size of u-boot 5MB and qspi blocks 0-79*/
#define UBOOT_FIRMWARE_START			0x00000000 
#define UBOOT_FIRMWARE_END				0x00060000

/* Allocated size of active VxWorks bin  10MB and qspi blocks 160-319*/
#define ACTIVE_SLAVE_FIRMWARE_START		0x00100000 
#define ACTIVE_SLAVE_FIRMWARE_END		0x01FFFFFC

/* Allocated size of passive VxWorks bin  10MB  and qspi blocks 320-479*/
#define PASSIVE_SLAVE_FIRMWARE_START	0x02000000 
#define PASSIVE_SLAVEFIRMWARE_END		0x03FFFFFC

/* Max FOE packet size 1012 Bytes */
#define FOE_PACKET_MAX_SIZE				0x000003F4

#define FW_PARTITION_INFO_EEPROM_OFFSET	0x00000020
#define FW_PARTITION_INFO_QSPI_OFFSET	0x000D0000

#define UBOOT_VERSION_QSPI_OFFSET		0x000C0000
/*QSPI Memory Selection GPIO */
#define GPIO_BANK1_PIN0 0x20
/*	uboot version+timestamp	*/
#define UBOOT_VERSION					0x133C648 //20170312
/* Reading firmware boot info */
ESS_BOOL ReadBootInformation(void);

#endif /* H_FIRMWAREINFO_H_ */
