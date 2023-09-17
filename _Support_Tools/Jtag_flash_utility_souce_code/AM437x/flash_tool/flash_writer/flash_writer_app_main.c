/**
 *  \file  flash_writer_app_main.c
 *
 *  \brief This application reads the images from mmcsd and
 *         copies them into qspi flash.
 *         It expects a file name config in the sd card which
 *         contains the name of the image to be flashed and the offset
 *         where it need to be flashed.
 *         Sample config file can be as :
 *         boot 0
 *         app 80000
 *         So this application will copy file named boot to offset 0
 *         of QSPI flash and app at offset 80000.
 *         It erases contents of flash before writing. The flash is erased
 *         in chunks of 64KB blocks.
 */

/**
 * \copyright  Copyright (C) 2013 Texas Instruments Incorporated -
 *             http://www.ti.com/
 */

/**
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "types.h"
#include "error.h"
#include "board.h"
#include "chipdb.h"
#include "soc.h"
#include "device.h"
#include "console_utils.h"
#include "misc.h"
#include "example_utils_mmu.h"
#include "cache.h"
#include "mmu.h"
#include "stdlib.h"

/* MMCSD and FS related header files */
#include "mmcsd_lib.h"
#include "ff.h"
#include "diskio.h"
#include "fatfs_port.h"
#include "fatfs_port_mmcsd.h"
#include "hsmmcsd_app_pol.h"

/* QSPI related header files */
#include "qspi_flash.h"
#include "qspi_lib.h"
#include "qspi.h"

/* Application header files */
#include "qspi_app.h"
#include "u-boot.h"
#include "uEnv.h"
#include "vxWorks.h"
#include "crc16.h"




/* ========================================================================== */
/*                          Macros and Typedefs                               */
/* ========================================================================== */

/** \brief Base address of QSPI ctrl module */
#define QSPI_CONFIG_BASE_ADDR              (0x47900000U)
#define SYS_CONFIG_RESET_BASE_ADDR         (0x44DF4000U)
#define SYS_BOOT_BASE_ADDR					(0x44E10040)
#define SYS_BOOT_MASK						0xFFFFFF0F
/** \brief Maximum buffer size. */
#define MMCSD_DATA_BUF_SIZE                 (512U)

#define  UBOOT_QSPI_OFFSET          0x00000000
#define  PARTITION_A_OFFSET         0x00100000
#define  PARTITION_B_OFFSET         0x02000000
#define  REF_STRUCT_QSPI_OFFSET     0x000D0000


/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */


typedef int32_t Board_STATUS;

typedef struct fwInfo {
	int bootMode;
	int fwUpgrade;
	int activePart;
	int passivePart;

} FW_INFO;


/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 * \brief This function initializes the MMCSD controller and mounts the device.
 */
static void QspiMmcsdInit(void);

/**
 * \brief This API copies the flash image from MMCSD to DDR
 *
 * \param fileName Name of the file to be copied.
 * \param destAddr Address where the file is to be copied
 * \param fileSize Total size of the file read
 *
 * \retval status S_PASS Image copy passed
 * \retval status E_FAIL Image copy failed
 */
static int32_t MmcsdImageCopy(char * fileName,
                               uint32_t destAddr,
                               uint32_t * fileSize);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/** \brief  Global structure for QSPI instance related data */
static qspiAppObj_t gQspiAppFlashWriterObj;

/** \brief Default configuration for QSPI */
static const qspiAppObj_t QSPIAPPOBJ_DEFAULT =
{
    0U,  /* chipSelect */
    0U,  /* instNum */
    0U,  /* instAddr */
    DEVICE_ID_INVALID,  /* devId */
    0U,  /* length*/
    {
        {
            0U, /* chipSelect */
            0U, /* memMapBaseAddr */
            0U, /* cfgBaseAddr */
            QSPI_LIB_READ_TYPE_QUAD, /* qspiLibReadType */
            QSPI_LIB_TX_MODE_MEMORY_MAPPED, /* qspiLibTxMode */
        },
        {
            (64U * MEM_SIZE_MB),/* deviceSize */
            0x19U,/* flashDevId */
            0xC2,/* flashMfgId */
            (4U * MEM_SIZE_KB),/* sectorSize */
            (64U * MEM_SIZE_KB),/* blockSize */
        }
    }
};

/**
 * \brief MMCSD card info object.
 */
ALIGN_VAR(sdCard, 32U)
mmcsdLibCardInfo_t sdCard;

/**
 * \brief MMCSD controller info object.
 */
ALIGN_VAR(ctrlInfo, 32U)
mmcsdLibCtrlInfo_t ctrlInfo;

/**
 * \brief Temporary data buffer to be used while copying data
 *        from SD card to DDR.
 */
ALIGN_VAR(gTmpBuf, 32U)
static char gTmpBuf[MMCSD_DATA_BUF_SIZE];

/**
 * \brief FAT fs object.
 */
static FATFS gFatFs;

/**
 * \brief Device.
 */
fatFsPortDev_t gDevice;

/**
 * \brief Drive.
 */
fatFsPortDrv_t gDrive;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
unsigned short crc16(const unsigned char* data_p, unsigned char length){
    unsigned char x;
    unsigned short crc = 0xFFFF;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
    }
    return crc;
}

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int main(void)
{

    qspiFlashDeviceData_t *flashDeviceData;
    /* Object containing the QSPI flash device and controller info */
    qspiLibInfo_t qspiLibInfo;

    /* File name which need to be copied from mmcsd to QSPI */
    char fileName[9U];
    /* Offset address where the flash image is to be written */
    int offsetAddr = 0U;
    unsigned short actual_crc[2];
    unsigned short image_crc[2];
    unsigned short crc_uboot=0xFFFF;
    unsigned short crc_uenv=0xFFFF;
    unsigned short crc_vxworks=0xFFFF;
    static const char crc_check[ksize_vxWorks];
    /* First block which has to be erased */
    uint32_t startBlockNumber = 0U;
    /* The last block which has to be erased */
    uint32_t endBlockNumber = 0U;
    /* Temporary variable for loops */
    uint32_t i;

    int32_t status = E_FAIL;

    /* Enable cache memory and MMU */
    MMUConfigAndEnable();
    CACHEEnable(CACHE_IDCACHE, CACHE_INNER_OUTER);

    boardInitParams_t boardInfo = {BOARD_IDKEVM, BOARD_REV_ICEV1_A3, 0};

    status = BOARDInit(&boardInfo);

    /* Initialize the UART console */
    CONSOLEUtilsInit();
    /* Select the console type based on compile time check */
    CONSOLEUtilsSetType(CONSOLE_UTILS_TYPE_UART);

    CONSOLEUtilsPrintf("\nBranson QSPI Flash Writer!!\n");

    gQspiAppFlashWriterObj = QSPIAPPOBJ_DEFAULT;
    qspiLibInfo = gQspiAppFlashWriterObj.qspiLibInfo;
    CONSOLEUtilsPrintf("BOARDInit status [0x%x]\n", status);

    /* Print SoC & Board Information. */
    SOCPrintInfo();
    BOARDPrintInfo();

    //QspiMmcsdInit();

    if (S_PASS == status)
    {
        /* Get Board Info for QSPI*/
        status = QSPIAppBoardInfoGet(&gQspiAppFlashWriterObj);
    }
    else
    {
        CONSOLEUtilsPrintf("Board Init Failed \n");
    }

    if(S_PASS == status)
    {
        /* Get SOC info for QSPI*/
        status = QSPIAppSocInfoGet(&gQspiAppFlashWriterObj);
    }
    else
    {
        CONSOLEUtilsPrintf("This is not supported on this board\n");
    }

    if (S_PASS == status)
    {
        /* Clock and pinmux configurations for QSPI  */
        status = QSPIAppInit(&gQspiAppFlashWriterObj);
    }
    else
    {
        CONSOLEUtilsPrintf("This is not supported on this SOC\n ");
    }

    if (S_PASS == status)
    {
        /* TODO Set the QSPI config base address
           Hard coded for now.
         */
        qspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr = QSPI_CONFIG_BASE_ADDR;
        /* Set the QSPI memory mapped base Address from chibdb*/
        qspiLibInfo.qspiLibCtrlInfo.memMapBaseAddr = gQspiAppFlashWriterObj.instAddr;
        qspiLibInfo.qspiLibCtrlInfo.chipSelect = gQspiAppFlashWriterObj.chipSelect;

        /* Initialise the QPSI controller */
        QSPILibInit(&qspiLibInfo);
        /* Display manufacturer and device ID of QSPI flash */
        QSPILibPrintId(&qspiLibInfo);
        image_crc[0]=crc16_ccitt(u_boot, sizeof(u_boot));
        image_crc[1]=crc16_ccitt(uEnv, sizeof(uEnv));
        image_crc[2]=crc16_ccitt(vxWorks, sizeof(vxWorks));
        //CONSOLEUtilsPrintf("crc values are uboot:0x%x, uenv:0x%x,vxworks:0x%x\n", image_crc[0],image_crc[1],image_crc[2]);

        if (S_PASS == status)
        {
        	CONSOLEUtilsPrintf("\n\nErasing the entire QSPI Flash! ");
        	printf("\n\nErasing the entire QSPI Flash! ");
        	CONSOLEUtilsPrintf("This takes a while!\n\n");
        	printf("This takes a while!\n\n");
        	//QSPILibChipErase(&qspiLibInfo);

			CONSOLEUtilsPrintf("Flashing images to QSPI flash..\n");
			CONSOLEUtilsPrintf("Size of uBoot: 0x%x\n", sizeof(u_boot));
			gQspiAppFlashWriterObj.length = 0x4000000;//sizeof(vxWorks);//0x70000 - 0x4/*sizeof(u_boot)*/;
			/* Computing the block numbers to be erased  */
			offsetAddr = 0x0;
			startBlockNumber = (offsetAddr / qspiLibInfo.qspiLibFlashInfo.blockSize);
			endBlockNumber = (offsetAddr + gQspiAppFlashWriterObj.length) /
							qspiLibInfo.qspiLibFlashInfo.blockSize;

			for (i = startBlockNumber; i <= endBlockNumber; i++)
			{
				QSPILibBlockErase(&qspiLibInfo, i);
			}

			/* Computing the block numbers to be erased  */
			offsetAddr = 0x0;
			gQspiAppFlashWriterObj.length = sizeof(u_boot);
			startBlockNumber = (offsetAddr / qspiLibInfo.qspiLibFlashInfo.blockSize);
			endBlockNumber = (offsetAddr + gQspiAppFlashWriterObj.length) /
							qspiLibInfo.qspiLibFlashInfo.blockSize;
            for (i = startBlockNumber; i <= endBlockNumber; i++)
            {
                QSPILibBlockErase(&qspiLibInfo, i);
            }
			/* Write image to QSPI flash */
			CONSOLEUtilsPrintf("Bootloader flashing start! %x\n", offsetAddr);
			printf("Bootloader flashing start! %x\n", offsetAddr);
			QSPILibWrite(&qspiLibInfo,
						offsetAddr,
						(uint32_t)u_boot,
						gQspiAppFlashWriterObj.length);

			CONSOLEUtilsPrintf("Bootloader flashing completed! %x\n", offsetAddr);
			printf("Bootloader flashing completed! %x\n", offsetAddr);


			offsetAddr = 0x100000;//offsetAddr + sizeof(uEnv_txt);
			CONSOLEUtilsPrintf("Size of vxWorks: 0x%x\n", sizeof(vxWorks));
			gQspiAppFlashWriterObj.length = sizeof(vxWorks);
			startBlockNumber = (offsetAddr / qspiLibInfo.qspiLibFlashInfo.blockSize);
			endBlockNumber = (offsetAddr + gQspiAppFlashWriterObj.length) /
							  qspiLibInfo.qspiLibFlashInfo.blockSize;
			for (i = startBlockNumber; i <= endBlockNumber; i++)
			{
				QSPILibBlockErase(&qspiLibInfo, i);
			}
			CONSOLEUtilsPrintf("Active partition vxworks Flashing started %x\n", offsetAddr);
			printf("Vxworks Flashing started %x\n", offsetAddr);
			/* Write image to QSPI flash */
			QSPILibWrite(&qspiLibInfo,
						offsetAddr,
						(uint32_t)vxWorks,
						gQspiAppFlashWriterObj.length);

			CONSOLEUtilsPrintf("vxworks Flashing completed \n");
			printf("vxworks Flashing completed \n");
#if 1
			offsetAddr = 0xD0000;//offsetAddr + sizeof(uEnv_txt);
			gQspiAppFlashWriterObj.length = sizeof(FW_INFO);
			startBlockNumber = (offsetAddr / qspiLibInfo.qspiLibFlashInfo.blockSize);
			endBlockNumber = (offsetAddr + gQspiAppFlashWriterObj.length) /
							  qspiLibInfo.qspiLibFlashInfo.blockSize;
			for (i = startBlockNumber; i <= endBlockNumber; i++)
			{
				QSPILibBlockErase(&qspiLibInfo, i);
			}
#endif


//			/* set sysboot control register */
////			unsigned int regVal = HW_RD_REG32(SYS_BOOT_BASE_ADDR);
////			regVal &=SYS_BOOT_MASK;
////			HW_WR_REG32(SYS_BOOT_BASE_ADDR,regVal);
//			CONSOLEUtilsPrintf("qspi boot priority changed successfully \n");

			printf("SOFTWARE FLASHING COMPLETE. REBOOT / POWER CYCLE THE DEVICE!\n");


		}

		/* The quad mode has to be enabled while flashing image as it cannot be
		 * done while booting and the ROM code expects the Flash to be
		 * in quad mode.
		 * So this change to quad mode is required here.
		 */
#if 1

        CONSOLEUtilsPrintf("Changing read to quad mode ");

        flashDeviceData = QSPIFlashGetDeviceData(gQspiAppFlashWriterObj.devId);
        if(QSPI_LIB_READ_TYPE_QUAD == qspiLibInfo.qspiLibCtrlInfo.qspiLibReadType)
        {
        	/* Enter quad mode if Quad read mode is selected */
        	QSPILibQuadEnable(&qspiLibInfo, flashDeviceData);
        }
        CONSOLEUtilsPrintf("\nRead mode has been changed to Quad mode \n");
        CONSOLEUtilsPrintf("SUCCESS!!!\n");
#endif

    }
    CONSOLEUtilsPrintf("rebooting ..... \n");
    HW_WR_REG32(SYS_CONFIG_RESET_BASE_ADDR, 0x02);
    return (S_PASS);
}

/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */

//static void QspiMmcsdInit(void)
//{
//    HSMMCSDAppPolInit(&ctrlInfo, &sdCard, 0U);
//
//    gDevice.devType = FATFS_PORT_DEV_TYPE_MMCSD;
//    gDevice.pName = "CARD";
//    gDevice.pIntrf = &sdCard;
//    gDevice.initStatus = STA_NOINIT;
//
//    gDrive.pName = "MMCSD0";
//    gDrive.pDev = &gDevice;
//    gDrive.pFatFs = &gFatFs;
//
//    FATFSPortDevConfig(&gDevice, TRUE);
//    FATFSPortDrvConfig(&gDrive, TRUE);
//}
//
//static int32_t MmcsdImageCopy(char * fileName,
//                              uint32_t destAddr,
//                              uint32_t * fileSize)
//{
//    FRESULT fResult;
//    static FIL fileObject;
//    uint32_t bytesRead = 0U;
//    int32_t status = E_FAIL;
//    /* Open the file for reading */
//    fResult = f_open(&fileObject, fileName, FA_READ);
//
//    if(FR_OK == fResult)
//    {
//        status = S_PASS;
//    }
//    /* If there was some problem opening the file, then return an error. */
//    else
//    {
//        CONSOLEUtilsPrintf("\r\n Unable to open file %s \r\n", fileName);
//    }
//
//    if(S_PASS == status)
//    {
//        /*
//         * Enter a loop to repeatedly read data from the file and display it, until
//         * the end of the file is reached.
//         */
//        CONSOLEUtilsPrintf("Copying %s to QSPI Flash \n", fileName);
//        do
//        {
//            /*
//             * Read a block of data from the file.  Read as much as can fit in the
//             * temporary buffer, including a space for the trailing null.
//             */
//            fResult = f_read(&fileObject, gTmpBuf, sizeof(gTmpBuf) - 1,
//                             (unsigned int *) &bytesRead);
//
//            /*
//             * If there was an error reading, then print a newline and return the
//             * error to the user.
//             */
//            if(fResult != FR_OK)
//            {
//                CONSOLEUtilsPuts("\r\n Error reading application file\r\n");
//                status = E_FAIL;
//            }
//
//            if(bytesRead >= sizeof(gTmpBuf))
//            {
//                status = E_FAIL;
//            }
//
//            /* Read the last chunk of the file that was received. */
//            memcpy((uint32_t *)destAddr, gTmpBuf, (sizeof(gTmpBuf) - 1));
//            destAddr += (sizeof(gTmpBuf) - 1);
//            /*
//             * Continue reading until less than the full number of bytes are read.
//             * That means the end of the buffer was reached.
//             */
//            *(fileSize) += bytesRead;
//        }
//        while((bytesRead == sizeof(gTmpBuf) - 1) && (S_PASS == status));
//        /* Close the file. */
//        fResult = f_close(&fileObject);
//        if (E_FAIL == status)
//        {
//            CONSOLEUtilsPrintf(" Read failed for %s\n",fileName);
//        }
//    }
//    return status;
//}
