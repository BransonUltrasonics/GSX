/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*********************  Header files *********************************/

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#if !defined (SOC_K2G)
#include <ti/csl/soc/am572x/src/cslr_control_core_pad_io.h>
#endif
#include <stdlib.h>

#if !defined(SOC_K2G)
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/soc/QSPI_v1.h>
#include <ti/drv/spi/soc/SPI_v1.h>
#endif

//#define TEST_DATA_LEN 	111830

/* Flash header file */
#if !defined(SOC_K2G)
#include "qspi_flash.h"
#endif

#include "board.h"
#include "board_cfg.h"

#include "MLO.h"
#include "u-boot.h"
#include "vxWorks.h"
#include "crc16.h"

/************************** Macros ************************************/

/* Board specific definitions */
#define QSPI_PER_CNT            (1U)
#define QSPI_INSTANCE           (1U)

#if defined (SOC_AM335x) || defined (SOC_AM437x)
#define QSPI_OFFSET             (5U)
#endif
#if defined (SOC_AM572x) || defined (SOC_AM571x) || defined (SOC_AM574x)
#define QSPI_OFFSET             (4U)
#endif

#define CSL_MCSPI_PER_CNT   (4U)
#define CSL_QSPI_PER_CNT    1U
#define PDK_RAW_BOOT 1

/* Flash-SF25FL specifications */

/** Memory size 1 byte */
#define MEM_SIZE_1B                (1U)
/** Memory size 1 kilo-byte */
#define MEM_SIZE_KB                (1024U * MEM_SIZE_1B)
/** Memory size 1 gega-byte */
#define MEM_SIZE_MB                (1024U * MEM_SIZE_KB)

/** QSPI device specific definitions */
#define QSPI_INSTANCE                       (1U)
#define QSPI_OFFSET                         (4U)
#define QSPI_DEVICE_SIZE                    (64 * MEM_SIZE_MB)
#define QSPI_DEVICE_BLOCK_SIZE              (64 * MEM_SIZE_KB)
 
/************************** Internal functions ************************/

int qspiFlashWrite(S25FL_Handle flashHandle,uint32_t offset, uint32_t length,uint8_t *src );

int qspiFlashRead(S25FL_Handle flashHandle,uint32_t offset, uint32_t length,uint8_t *dest);

void qspiFlashErase(S25FL_Handle flashHandle,uint32_t offset, uint32_t length);

unsigned short crc16_ccitt(const void *buf, int len);

int qspi_JtagFlash(void);


/************************** Global Variables **************************/


/* Flash offset address to which data write/read has to be performed */
//uint32_t addrValue = 0x000000U;

extern const SPI_Config SPI_config[];

/* transfer length */
uint32_t transferLength = 0;

/* Image CRC calculation buffer*/
//static char crc_check[ksize_vxWorks];

/************************* Function definitions ************************/

/*
 *  Qspi write abstract function
 */
int qspiFlashWrite(S25FL_Handle flashHandle,uint32_t offset, uint32_t length,uint8_t *src )
{
    int retval=0;
    S25FL_Transaction flashTransaction;

   /* Update transaction parameters */
    flashTransaction.data       = src;
    flashTransaction.address    = offset;
    flashTransaction.dataSize   = length;  /* In bytes */

    /* Write buffer to flash */
    retval = SF25FL_bufferWrite(flashHandle, &flashTransaction);

    return retval;
}


/*
 *  Qspi read abstract function
 */
int qspiFlashRead(S25FL_Handle flashHandle,uint32_t offset, uint32_t length,uint8_t *dest)
{
    int retval=0;
    S25FL_Transaction flashTransaction;

    /* Update transaction parameters */
    flashTransaction.data       = dest;
    flashTransaction.address    = offset;
    flashTransaction.dataSize   = length;  /* In bytes */

    /* Write buffer to flash */
    retval = SF25FL_bufferRead(flashHandle, &flashTransaction);

    return retval;
}


/*
 *  Qspi erase function
 */
void qspiFlashErase(S25FL_Handle flashHandle,uint32_t offset, uint32_t length)
{
  
   uint32_t i;
   uint32_t startBlockNumber, endBlockNumber;

   startBlockNumber = (offset / QSPI_DEVICE_BLOCK_SIZE);
   endBlockNumber = ( (offset + length) / QSPI_DEVICE_BLOCK_SIZE);

    for (i = startBlockNumber; i <= endBlockNumber; i++)
    {
        S25FLFlash_BlockErase(flashHandle, i);
    }
   
   return;
}


/*
 *  Standard 16-bit CRC calculation function
 */
unsigned short crc16_ccitt(const void *buf, int len)
{
   int counter;
   unsigned short crc = 0;
   for( counter = 0; counter < len; counter++)
   {
       crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(char *)buf)&0x00FF]; 
       buf = ((char *)buf) +1;
   }
     return crc;
}


/*
 *  Qspi jtag fash function
 */
#if !defined (SOC_K2G)
int qspi_JtagFlash()
{
    #define MLO_FLASH_OFFSET          0x000000
    #define UBOOT_FLASH_OFFSET        0x040000
    #define VXWORKS_FLASH_OFFSET      0x1E0000
    #define FIRMWARE_PARTITION_INFO   0x1C0000
   // #define CRC_REFERENCE_INFO        0x1D0000
    #define FLASH_PARTITION_B_OFFSET  0x2000000
    #define CTRL_CORE_PAD_RESET       0x4AE07D00


    SPI_Params spiParams;                /* SPI params structure  */
    S25FL_Handle flashHandle;            /* Flash handle */
    SPI_Handle handle;                   /* SPI handle */
    QSPI_HwAttrs *hwAttrs;               /* QSPI hardwar e attributes  */
    int i;uint32_t ret;                  /* return value  */
    uint32_t addressValue;               /* address parameter */
    //unsigned short actualCrc[4]={0,};    /* Actual CRC */
    //unsigned short imageCrc[4]={0,};     /* Calculated image CRC */
    uint32_t addrValue = 0x0;
    FwFlashInfo fwflashinfo_t;
    FwFlashInfo *ptr_fwflashinfo = &fwflashinfo_t;


    for (i=0; i<CSL_MCSPI_PER_CNT; i++)
    {
        ((SPI_v1_HWAttrs *)SPI_config[i].hwAttrs)->enableIntr = false;
    }

    for (i=CSL_MCSPI_PER_CNT; i<CSL_MCSPI_PER_CNT+CSL_QSPI_PER_CNT; i++)
    {
        ((QSPI_HwAttrs *)SPI_config[i].hwAttrs)->intrEnable = false;
    }

    
    /* Init SPI driver */
    SPI_init();
	
    /* Default SPI configuration parameters */
    SPI_Params_init(&spiParams);
    /* Open QSPI driver */
    flashHandle = SF25FL_open(((QSPI_INSTANCE - 1)+(QSPI_OFFSET)), &spiParams);

    /* Extract hardware attributes */
    handle = flashHandle->spiHandle;
    hwAttrs = (QSPI_HwAttrs *)handle->hwAttrs;

    /* Print flash Id */
    FlashPrintId(flashHandle);

    /* Reference CRC calculation */
    //actualCrc[0]=crc16_ccitt(MLO, ksize_MLO);
    //actualCrc[1]=crc16_ccitt(u_boot, ksize_u_boot);
    //actualCrc[2]=crc16_ccitt(vxWorks, ksize_vxWorks);
    //actualCrc[3]=crc16_ccitt(uEnv, ksize_uEnv);
#if 1
    /* Update the structure used for reference by uboot to verify vxworks checksum */
    //fwflashinfo_t.partition_a_crc    =  actualCrc[2];
    //fwflashinfo_t.partition_a_size   =  ksize_vxWorks;
    //fwflashinfo_t.partition_a_offset =  VXWORKS_FLASH_OFFSET;

    fwflashinfo_t.bootMode        	 =  0xFFFFFFFF;
    fwflashinfo_t.fw_upgrade         =  0;
	fwflashinfo_t.partition_a_offset =  VXWORKS_FLASH_OFFSET;
    fwflashinfo_t.partition_b_offset =  FLASH_PARTITION_B_OFFSET;
	
    //fwflashinfo_t.fwflashinfo_crc = crc16_ccitt(ptr_fwflashinfo,sizeof(FwFlashInfo));
    //UART_printf("\n Ref info struct CRC=0x%x\n",fwflashinfo_t.fwflashinfo_crc);
#endif

    /* Update target address based on the modes. For config mode pass the
       address of the variable containing the flash offset value.
       For memory mapped mode directly pass the flash offset value. */

    addressValue = (QSPI_OPER_MODE_CFG == hwAttrs->operMode) ?
        (uint32_t)&addrValue: addrValue;

#if 1
    UART_printf("\n Entire Chip erase in progress..\n");
    /* Writing MLO to flash */

    transferLength = 0x4000000;
    addressValue = MLO_FLASH_OFFSET;

    qspiFlashErase(flashHandle,addressValue,transferLength);
    UART_printf("\n flash erase succesfully! \n");
#endif

#if 1

    /* Writing MLO to flash */

    transferLength = sizeof(MLO);
    addressValue = MLO_FLASH_OFFSET;
 
    qspiFlashErase(flashHandle,addressValue,transferLength);
 
    ret = qspiFlashWrite(flashHandle,addressValue,transferLength,(uint8_t *)&MLO);
    if(ret > 0)
    {
       UART_printf("\n MLO flashed succesfully! \n");
    }
#endif
    /* Writing u-boot to flash */

    transferLength = sizeof(u_boot);
    addressValue = UBOOT_FLASH_OFFSET;  

    qspiFlashErase(flashHandle,addressValue,transferLength);
   
    qspiFlashWrite(flashHandle,addressValue,transferLength,(uint8_t *)&u_boot);
    UART_printf("\n U-boot flashed successfully! \n");

    /* Writing vxworks to flash */
#if 1
    transferLength = sizeof(vxWorks);
    addressValue = VXWORKS_FLASH_OFFSET;
    qspiFlashErase(flashHandle,addressValue,transferLength);
    qspiFlashWrite(flashHandle,addressValue,transferLength,(uint8_t *)&vxWorks);
   
    if(ret > 0)
    {
      UART_printf("\n Vxworks flashed successfully! \n");
    }  
#endif
#if 0
    /* Writing uEnv to flash */ 
  
    transferLength = ksize_uEnv;
    addressValue = UENV_FLASH_OFFSET;

    qspiFlashErase(flashHandle,addressValue,transferLength);    

    qspiFlashWrite(flashHandle,addressValue,transferLength,(uint8_t *)&uEnv);
    if(ret > 0)
    {
       UART_printf("\n uEnv script flashed successfully! \n");
    }
#endif
    /* Writing refernce info to flash */
#if 1
    transferLength = sizeof(fwflashinfo_t);
    addressValue = FIRMWARE_PARTITION_INFO;

    qspiFlashErase(flashHandle,addressValue,transferLength);

    qspiFlashWrite(flashHandle,addressValue,transferLength,(uint8_t *)&fwflashinfo_t);
    if(ret > 0)
    {
       UART_printf("\n Master board firmware upgrade successfully completed! \n");
    }

#endif
    /* Configuring qspi into quad mode for read */

    if(QSPI_RX_LINES_QUAD == hwAttrs->rxLines)
    {
      S25FLFlash_QuadModeEnable(flashHandle);
    }
#if 0
    UART_printf("\n***********Verfication of checksum!**********\n");

    /* MLO checksum verification */

    transferLength = ksize_MLO;
    addressValue = MLO_FLASH_OFFSET; 

    ret = qspiFlashRead(flashHandle,addressValue,transferLength,(uint8_t *)&crc_check);
    
    imageCrc[0] = crc16_ccitt(crc_check, ksize_MLO);

    if(imageCrc[0] == actualCrc[0])
    {
       UART_printf("\n MLO CRC matched!\n");
    }
    else
    {
       UART_printf("MLO CRC Mismatched!\n");
    }
 
    /* u-boot checksum verification */

    transferLength = ksize_u_boot;
    addressValue = UBOOT_FLASH_OFFSET;
  
    ret = qspiFlashRead(flashHandle,addressValue,transferLength,(uint8_t *)&crc_check);
    if(ret < 0 )
    {
       UART_printf("u-boot read failed");
    }

    imageCrc[1] = crc16_ccitt(crc_check, ksize_u_boot);
    
    if(imageCrc[1] == actualCrc[1])
    {
       UART_printf("\n u-boot CRC matched!\n");
    }
    else
    {
       UART_printf("u-boot CRC Mismatched!\n");
    }


    /*  vxWorks checksum verification  */

    transferLength = ksize_vxWorks;
    addressValue = VXWORKS_FLASH_OFFSET;
  
    ret = qspiFlashRead(flashHandle,addressValue,transferLength,(uint8_t *)&crc_check);
    if(ret < 0 )
    {
       UART_printf("vxWorks read failed");
    }
 
    imageCrc[2] = crc16_ccitt(crc_check, ksize_vxWorks);

    if(imageCrc[2] == actualCrc[2])
    {
       UART_printf("\n Vxworks CRC matched!\n");
    }
    else
    {
       UART_printf("Vxworks CRC Mismatched!\n");
    }


    /*  uEnv script checksum verification  */

    transferLength = ksize_uEnv;
    addressValue = UENV_FLASH_OFFSET;
   
    ret = qspiFlashRead(flashHandle,addressValue,transferLength,(uint8_t *)&crc_check);
    if(ret < 0 )
    {
       UART_printf("uEnv read failed");
    }

    imageCrc[3] = crc16_ccitt(crc_check, ksize_uEnv);

    if(imageCrc[3] == actualCrc[3])
    {
       UART_printf("\n uEnv CRC matched!\n");
    }
    else
    {
       UART_printf("uEnv CRC Mismatched!\n");
    }
#endif
   SF25FL_close(flashHandle);

   /* Warm Reset */
  HW_WR_REG32(CTRL_CORE_PAD_RESET,0x02);

   return 0;

}
#endif


int qspi_start()
{
   UART_printf("\n*********************************************\n");
   UART_printf  ("*             QSPI JTAG FLASHING            *\n");
   UART_printf  ("*********************************************\n");
   
   UART_printf("\n*************Flashing started!***************\n");

   return qspi_JtagFlash();
}

/*
 * Main function
 */
int main(void)
{
    Board_initCfg boardCfg;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
        BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO;
#endif
    Board_init(boardCfg);

	return qspi_start();
}
