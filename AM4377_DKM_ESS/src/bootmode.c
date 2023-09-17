/**
\addtogroup ESM EtherCAT State Machine
@{
*/

/**
\file bootmode.c
\author EthercatSSC@beckhoff.com
\brief Implementation

\version 4.20

<br>Changes to version - :<br>
V4.20: File created
*/

/*--------------------------------------------------------------------------------------
------
------    Includes
------
--------------------------------------------------------------------------------------*/
#include <ecat_def.h>
#if BOOTSTRAPMODE_SUPPORTED

#define    _BOOTMODE_ 1
#include <bootmode.h>
#undef _BOOTMODE_

#include <vxbAm437xQspiLib.h>
#include "firmwareInfo.h"
#include <subsys/gpio/vxbGpioLib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ioLib.h>

#define EEPROM_FOE_FW_FLASH_INFO 0x40

#define FOE_APPL_BIN_OFFSET 0
/** Memory size 1 byte */
#define MEM_SIZE_1B                (1U)
/** Memory size 1 kilo-byte */
#define MEM_SIZE_KB                (1024U * MEM_SIZE_1B)
/** Memory size 1 MEGA-byte */
#define MEM_SIZE_MB                (1024U * MEM_SIZE_KB)
/** QSPI access block size */
#define BLOCK_SIZE					(64U * FOEMEM_SIZE_KB)

#define UBOOT_LEN				   (400 * MEM_SIZE_KB)
#define VXWORKS_LEN				   (10 * MEM_SIZE_MB )

#define E_FAIL -1 

typedef struct SlaveUpgrade
{
	char 	  SlaveName[13];
	uint32_t  SlaveOffset;
	ssize_t   SlaveSize;
}FoeSlaveBinInfo;

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    State        Current state

 \brief Dummy BL_Start function
*////////////////////////////////////////////////////////////////////////////////////////
void BL_Start( ESS_UINT8 State)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    password    download password

 \brief Dummy BL_StartDownload function
*////////////////////////////////////////////////////////////////////////////////////////
void BL_StartDownload(ESS_UINT32 password)
{
	#if PRUESC_HW
	#ifdef ENABLE_ONLINE_FIRMWARE_UPGRADE
		printf("Started Downloading The Firmware.\n");
	   // bsp_start_fw_download(password);
	#endif    
	#endif    
}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pData    Data pointer
 \param    Size    Data Length


 \return    FoE error code

 \brief Dummy BL_Data function
*////////////////////////////////////////////////////////////////////////////////////////
ESS_UINT16 BL_Data(ESS_UINT16 *pData,ESS_UINT16 Size)
{
	#if PRUESC_HW
	#ifdef ENABLE_ONLINE_FIRMWARE_UPGRADE
		//   bsp_store_fw_data(pData,Size);
		/*store the firmware data */
	    memcpy(FirmwareUpgradeFileBuffer+FirmwareUpgradeFileBufferLenght,pData,Size);
	    FirmwareUpgradeFileBufferLenght += Size;
	#endif    
	#endif    
		return 0;
}
#endif //BOOTSTRAPMODE_SUPPORTED

#if 0 /*TBD not used */
int FirmwareWriteLocation(char *fileName) 
{
	int ret;
	uint32_t offsetAddr;
	FoeSlaveBinInfo FoeSlaveBinInfo,FoeSlaveBinInfoR;
	unsigned int newOffset;
	int status;
	char msg=0;
	
	int fd = open("/eeprom/0",O_RDWR,0);
	
	memset(&FoeSlaveBinInfo,0,sizeof(FoeSlaveBinInfo));
	strcpy(FoeSlaveBinInfo.SlaveName,"/0");
	FoeSlaveBinInfo.SlaveOffset = 0;
	FoeSlaveBinInfo.SlaveSize = 0;
	close(fd);
	return msg;	
	
}
#endif



