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
#include <ti/csl/soc/am572x/src/cslr_control_core_pad_io.h>
#include <stdlib.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/soc/QSPI_v1.h>
#include <ti/drv/spi/soc/SPI_v1.h>

//#define TEST_DATA_LEN 	111830

/* Flash header file */
#include "qspi_flash.h"

#include "board.h"
#include "board_cfg.h"

#include "MLO.h"
#include "u-boot.h"
#include "vxWorks.h"
#include "uEnv.h"
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

//#define HW_WR_REG32(addr, data)   *(unsigned int*)(addr)=(unsigned int)(data)

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

int qspi_JtagFlash(void);


/************************** Global Variables **************************/


/* Flash offset address to which data write/read has to be performed */
//uint32_t addrValue = 0x000000U;

extern const SPI_Config SPI_config[];

/* transfer length */
uint32_t transferLength = 0;


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
   //uint32_t step=0, per=0,last_per=0;
   startBlockNumber = (offset / QSPI_DEVICE_BLOCK_SIZE);
   endBlockNumber = ( (offset + length) / QSPI_DEVICE_BLOCK_SIZE);
   //step = (endBlockNumber - startBlockNumber)/100;
    UART_printf("startBlockNumber:0x%x endBlockNumber:0x%x\n", startBlockNumber, endBlockNumber);
    for (i = startBlockNumber; i <= endBlockNumber; i++)
    {
	UART_printf("block erase start\n");
   	//per  = i/step;
        S25FLFlash_BlockErase(flashHandle, i);
//	if((i%step) &&(last_per != per))
//	{
//		UART_printf("#");
//		last_per = per;
//	}
    }
   //UART_printf("\n");
   return;
}


/*
 *  Qspi jtag fash function
 */
int qspi_JtagFlash()
{
    #define MLO_FLASH_OFFSET          0x0000000
    #define UBOOT_VERSION_OFFSET      0x020000
    #define UBOOT_FLASH_OFFSET        0x040000
    #define VXWORKS_FLASH_OFFSET      0x1E0000
    #define FIRMWARE_PARTITION_INFO   0x1D0000
    //#define CRC_REFERENCE_INFO        0x1D0000
    #define FLASH_PARTITION_B_OFFSET  0x1000000
    #define CTRL_CORE_PAD_RESET       0x4AE07D00


    SPI_Params spiParams;                /* SPI params structure  */
    S25FL_Handle flashHandle;            /* Flash handle */
    SPI_Handle handle;                   /* SPI handle */
    QSPI_HwAttrs *hwAttrs;               /* QSPI hardwar e attributes  */
    int j;uint32_t ret;                  /* return value  */
    uint32_t addressValue;               /* address parameter */
    unsigned short actualCrc[4];    /* Actual CRC */
    unsigned short imageCrc[4];     /* Calculated image CRC */
    uint32_t addrValue = 0x0;
    int i,k;
    /* Image CRC calculation buffer*/
    static char crc_check_vxworks[kSize_vxWorks];
    char *ptr = NULL;
    static char vxWorks_test;
    unsigned short crcFail = FALSE;
    static const unsigned char uboot_version[] = {0x48,0xC6,0x33,0x01}; 
    //static const unsigned char test[4]={0xAA,0x55,0xAA,0x55};
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
    
    /* Inkt SPI driver */
    SPI_init();
	
    /* Default SPI configuration parameters */
    SPI_Params_init(&spiParams);
    //UART_printf("bitrate%u\n", spiParams.bitRate);
    /* Open QSPI driver */
    flashHandle = SF25FL_open(((QSPI_INSTANCE - 1)+(QSPI_OFFSET)), &spiParams);

    /* Extract hardware attributes */
    handle = flashHandle->spiHandle;
    hwAttrs = (QSPI_HwAttrs *)handle->hwAttrs;

    /* Print flash Id */
    FlashPrintId(flashHandle);

    /* Reference CRC calculation */
    actualCrc[0]=ByteCRC(MLO, sizeof(MLO));
    actualCrc[1]=ByteCRC(u_boot, sizeof(u_boot));
    actualCrc[2]=ByteCRC(vxWorks, sizeof(vxWorks));
    //actualCrc[3]=crc16_ccitt(uEnv, ksize_uEnv);
    UART_printf("crc values are MLO:0x%x, uboot:0x%x,vxworks:0x%x\n", actualCrc[0],actualCrc[1],actualCrc[2]);
#if 1
    /* Update the structure used for reference by uboot to verify vxworks checksum */
    fwflashinfo_t.partition_a_crc    =  actualCrc[2];
    fwflashinfo_t.partition_a_size   =  kSize_vxWorks;
    fwflashinfo_t.partition_a_offset =  VXWORKS_FLASH_OFFSET;

    fwflashinfo_t.boot_status        =  0xFFFFFFFF;
    fwflashinfo_t.fw_upgrade         =  0;
    fwflashinfo_t.partition_b_size   =  0;
    fwflashinfo_t.partition_b_offset =  FLASH_PARTITION_B_OFFSET;
    fwflashinfo_t.emmc_copy          =  0;
    fwflashinfo_t.partition_b_crc    =  0xFFFF;
#endif

    /* Update target address based on the modes. For config mode pass the
       address of the variable containing the flash offset value.
       For memory mapped mode directly pass the flash offset value. */
    UART_printf("\n QSPI mode %d..\n", hwAttrs->operMode);
    addressValue = (QSPI_OPER_MODE_CFG == hwAttrs->operMode) ?
        (uint32_t)&addrValue: addrValue;

#if 1
    UART_printf("\n Chip erase in progress..\n");
    S25FLFlash_ChipErase(flashHandle);
    UART_printf("\n chip erase succesfully! \n");
#endif
#if 1

    UART_printf("\n MLO flash in progress.. \n");
    /* Writing MLO to flash */

    transferLength = sizeof(MLO);
    addressValue = MLO_FLASH_OFFSET;
   
    ret = qspiFlashWrite(flashHandle,addressValue,transferLength,(uint8_t *)&MLO);
    if(ret > 0)
    {
       UART_printf("\n MLO flashed succesfully! \n");
    }
#endif
#if 1
    UART_printf("\n uboot Version flash in progress.. \n");
   transferLength = 0x4;
   addressValue = UBOOT_VERSION_OFFSET;
   ret = qspiFlashWrite(flashHandle,addressValue,transferLength,(uint8_t *)&uboot_version);
    if(ret > 0)
    {
       UART_printf("\n uboot Version flashed succesfully! \n");
    }
#endif
#if 1
    /* Writing u-boot to flash */
    UART_printf("\n Uboot flash in progress.. \n");
    transferLength = sizeof(u_boot);
    addressValue = UBOOT_FLASH_OFFSET;

    ret = qspiFlashWrite(flashHandle,addressValue,transferLength,(uint8_t *)&u_boot);
    if(ret > 0)
    {
    UART_printf("\n U-boot flashed successfully! \n");
    }

    /* Writing vxworks to flash */
#endif
#if 1
    UART_printf("\n vxWorks flash in progress.. \n");
    transferLength = sizeof(vxWorks);
    addressValue = VXWORKS_FLASH_OFFSET;
    ret = qspiFlashWrite(flashHandle,addressValue,transferLength,(uint8_t *)&vxWorks);
   
    if(ret > 0)
    {
      UART_printf("\n vxWorks flashed successfully! \n");
    }
#endif
    /* Writing refernce info to flash */
#if 1
    UART_printf("\n Firmware structure flash in progress.. \n");
    transferLength = sizeof(fwflashinfo_t);
    addressValue = FIRMWARE_PARTITION_INFO;
    qspiFlashWrite(flashHandle,addressValue,transferLength,(uint8_t *)&fwflashinfo_t);
    if(ret > 0)
    {
       UART_printf("\n Reference info for firmware upgrade and checksum validation written successfully! \n");
    }

#endif

#if 1
    /* Configuring qspi into quad mode for read */

    if(QSPI_RX_LINES_QUAD == hwAttrs->rxLines)
    {
      UART_printf("QSPI_RX_LINES_QUAD used\n");
      S25FLFlash_QuadModeEnable(flashHandle);
    }
#endif
    
    UART_printf("\n***********Verfication of checksum!**********\n");
#if 1
    /* MLO checksum verification */

    transferLength = sizeof(MLO);
    addressValue = MLO_FLASH_OFFSET; 

    ret = qspiFlashRead(flashHandle,addressValue,transferLength,(uint8_t *)&crc_check_vxworks);

    
    imageCrc[0] = ByteCRC(&crc_check_vxworks, sizeof(MLO));

    if(imageCrc[0] == actualCrc[0])
    {
       UART_printf("\n MLO CRC matched!\n");
    }
    else
    {
       UART_printf("MLO CRC Mismatched!0x%x\n",imageCrc[0]);
       UART_printf("Please reflash the binaries !\n");

       //crcFail = TRUE;
       //while(crcFail);
    }
    /* u-boot checksum verification */

    transferLength = sizeof(u_boot);
    addressValue = UBOOT_FLASH_OFFSET;
  
    ret = qspiFlashRead(flashHandle,addressValue,transferLength,(uint8_t *)&crc_check_vxworks);
    if(ret < 0 )
    {
       UART_printf("u-boot read failed");
    }

    imageCrc[1] = ByteCRC(&crc_check_vxworks, sizeof(u_boot));
    
    if(imageCrc[1] == actualCrc[1])
    {
       UART_printf("\n u-boot CRC matched!\n");
    }
    else
    {
	UART_printf("Uboot CRC Mismatched!0x%x\n",imageCrc[1]);
       UART_printf("Please reflash the binaries !\n");   
   
       //crcFail = TRUE;
       //while(crcFail);
    }

#endif
    /*  vxWorks checksum verification  */
#if 1
    transferLength = sizeof(vxWorks);
    addressValue = VXWORKS_FLASH_OFFSET;
  
    ret = qspiFlashRead(flashHandle,addressValue,transferLength,(uint8_t *)&crc_check_vxworks);
    if(ret < 0 )
    {
       UART_printf("vxWorks read failed");
    }
#if 0
   for(k=0xE20000;k<=0xE30000;k++)
  {
        UART_printf("0x%02x", *(crc_check_vxworks+k));
        UART_printf(",");
        if(k%10 == 0)
        {
                UART_printf("\n");
                UART_printf("%x: ",k);
        }
  }
#endif

   imageCrc[2] = ByteCRC(crc_check_vxworks, sizeof(vxWorks));
    if(imageCrc[2] == actualCrc[2])
    {
       UART_printf("\n Vxworks CRC matched 0x%x\n",imageCrc[2]);
    }
    else
    {
	UART_printf("\nvxworks CRC Mismatched 0x%x\n",imageCrc[2]);
       UART_printf("Please reflash the binaries !\n");

       //crcFail = TRUE;
       //while(crcFail);
    }
 #endif
   SF25FL_close(flashHandle);

   /* Warm Reset */
   HW_WR_REG32(CTRL_CORE_PAD_RESET,0x02);

   return 0;

}


int qspi_start()
{
   UART_printf("\n*********************************************\n");
   UART_printf  ("*             Branson QSPI JTAG FLASHING            *\n");
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
