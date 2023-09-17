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
#define SYS_BOOT_BASE_ADDR                  (0x44E10040)
#define SYS_BOOT_MASK                       0xFFFFFF0F
/** \brief Maximum buffer size. */
#define MMCSD_DATA_BUF_SIZE                 (512U)

#define  UBOOT_QSPI_OFFSET          0x00000000
#define  PARTITION_A_OFFSET         0x00100000
#define  PARTITION_B_OFFSET         0x02000000
#define  UBOOT_VERSION_OFFSET       0x000C0000
#define  REF_STRUCT_QSPI_OFFSET     0x000D0000


/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

typedef struct fwflashinfo{

    int             boot_status;
    int             fw_upgrade;
    unsigned int    partition_a_offset;
    unsigned int    partition_b_offset;
    unsigned int    partition_a_size;
    unsigned int    partition_b_size;
    unsigned short  partition_a_crc;
    unsigned short  partition_b_crc;
    unsigned short  emmc_copy;
}FwFlashInfo;


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
    unsigned short actualRead_crc[3];
    unsigned short image_crc[3];

    static char crc_check[0x4000000];
    static const unsigned char uboot_version[]={0x48,0xC6,0x33,0x01};
    unsigned short crcFail = FALSE;
    uint32_t k =0;
    /* First block which has to be erased */
    uint32_t startBlockNumber = 0U;
    /* The last block which has to be erased */
    uint32_t endBlockNumber = 0U;
    /* Temporary variable for loops */
    uint32_t i;
   // BinInfo bInfoObj;
    FwFlashInfo fwflashinfo_t;
    FwFlashInfo *fwflashinfo = &fwflashinfo_t;

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

//    QSPILibChipErase(&qspiLibInfo);
//    CONSOLEUtilsPrintf("flash erase completed\n");

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
        image_crc[0]=ByteCRC(u_boot, sizeof(u_boot));
       // image_crc[1]=ByteCRC(uEnv, sizeof(uEnv));
        image_crc[1]=ByteCRC(vxWorks, sizeof(vxWorks));
        CONSOLEUtilsPrintf("crc values are uboot:0x%x,vxworks:0x%x\n", image_crc[0],image_crc[1]);

        if (S_PASS == status)
        {
#if 1
        	CONSOLEUtilsPrintf("\n\nErasing the entire QSPI Flash! ");
			CONSOLEUtilsPrintf("This takes a while!\n\n");
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

            CONSOLEUtilsPrintf("Chip erase completed\n");
#endif
#if 1
            CONSOLEUtilsPrintf("uboot flashing in progress...\n");
            gQspiAppFlashWriterObj.length = sizeof(u_boot);
            /* Computing the block numbers to be erased  */
            offsetAddr = 0x0;
            startBlockNumber = (offsetAddr / qspiLibInfo.qspiLibFlashInfo.blockSize);
            endBlockNumber = (offsetAddr + gQspiAppFlashWriterObj.length) /
                            qspiLibInfo.qspiLibFlashInfo.blockSize;

            for (i = startBlockNumber; i <= endBlockNumber; i++)
            {
                QSPILibBlockErase(&qspiLibInfo, i);
            }
            /* Write image to QSPI flash */
            QSPILibWrite(&qspiLibInfo,
                        offsetAddr,
                        (uint32_t)u_boot,
                        gQspiAppFlashWriterObj.length);

            CONSOLEUtilsPrintf("u-boot Flashing completed %x\n", offsetAddr);
#endif
#if 1
            CONSOLEUtilsPrintf("uboot version flashing in progress...\n");
            gQspiAppFlashWriterObj.length = sizeof(uboot_version);
            /* Computing the block numbers to be erased  */
            offsetAddr = UBOOT_VERSION_OFFSET;
            startBlockNumber = (offsetAddr / qspiLibInfo.qspiLibFlashInfo.blockSize);
            endBlockNumber = (offsetAddr + gQspiAppFlashWriterObj.length) /
                            qspiLibInfo.qspiLibFlashInfo.blockSize;

            for (i = startBlockNumber; i <= endBlockNumber; i++)
            {
                QSPILibBlockErase(&qspiLibInfo, i);
            }
            /* Write image to QSPI flash */
            QSPILibWrite(&qspiLibInfo,
                        offsetAddr,
                        (uint32_t)uboot_version,
                        gQspiAppFlashWriterObj.length);

            CONSOLEUtilsPrintf("u-boot version Flashing completed %x\n", offsetAddr);


#endif

#if 1
            /* Update the structure used for reference by uboot to verify vxworks checksum */
           fwflashinfo_t.partition_a_crc    =  image_crc[1];
           fwflashinfo_t.partition_a_size   =  sizeof(vxWorks);
           fwflashinfo_t.partition_a_offset =  PARTITION_A_OFFSET;

           fwflashinfo_t.boot_status        =  0xFFFFFFFF;
           fwflashinfo_t.fw_upgrade         =  0x0;
           fwflashinfo_t.partition_b_size   =  0x0;
           fwflashinfo_t.partition_b_offset =  PARTITION_B_OFFSET;
           fwflashinfo_t.emmc_copy          =  0x0;
           fwflashinfo_t.partition_b_crc    =  0xFFFF;

            /* Flashing BinInfo Structure */
            unsigned int    partition_b_size;
            unsigned short  partition_a_crc;

            CONSOLEUtilsPrintf("Fwflashinfo structure flashing in progress...\n");
            offsetAddr = REF_STRUCT_QSPI_OFFSET;
            CONSOLEUtilsPrintf("offset addr : %x\n",offsetAddr);

            gQspiAppFlashWriterObj.length = sizeof(fwflashinfo_t);

            startBlockNumber = (offsetAddr / qspiLibInfo.qspiLibFlashInfo.blockSize);
            endBlockNumber = (offsetAddr + gQspiAppFlashWriterObj.length) /
                                qspiLibInfo.qspiLibFlashInfo.blockSize;

            CONSOLEUtilsPrintf("startBlockNumber : %x, endBlockNumber : %x\n",startBlockNumber,endBlockNumber);
            for (i = startBlockNumber; i <= endBlockNumber; i++)
            {
                CONSOLEUtilsPrintf("QSPILibBlockErase block : %x\n",i);
                QSPILibBlockErase(&qspiLibInfo, i);
            }

            QSPILibWrite(&qspiLibInfo,
                         offsetAddr,
                         (uint32_t)(fwflashinfo),
                         gQspiAppFlashWriterObj.length);

//            QSPILibRead(&qspiLibInfo,offsetAddr,
//                                    (uint32_t)(fwflashinfo), gQspiAppFlashWriterObj.length);
//
//            CONSOLEUtilsPrintf(" >> fwflashinfo_t.boot_status =  %x \n",fwflashinfo_t.boot_status);
//            CONSOLEUtilsPrintf(" >> fwflashinfo_t.fw_upgrade =  %x \n",fwflashinfo_t.fw_upgrade);
//            CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_a_offset =  %x \n",fwflashinfo_t.partition_a_offset);
//            CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_b_offset =  %x \n",fwflashinfo_t.partition_b_offset);
//            CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_a_size = %x \n",fwflashinfo_t.partition_a_size );
//            CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_b_size =  %x \n",fwflashinfo_t.partition_b_size);
//            CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_a_crc = %x \n",fwflashinfo_t.partition_a_crc);
//            CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_b_crc = %x \n",fwflashinfo_t.partition_b_crc);
            CONSOLEUtilsPrintf("write fwflashinfo_t values to flash at 0xD0000\n");
#endif
#if 1
            CONSOLEUtilsPrintf("vxworks flashing in progress...\n");
            offsetAddr = PARTITION_A_OFFSET;
            gQspiAppFlashWriterObj.length = sizeof(vxWorks);
            startBlockNumber = (offsetAddr / qspiLibInfo.qspiLibFlashInfo.blockSize);
            endBlockNumber = (offsetAddr + gQspiAppFlashWriterObj.length) /
                              qspiLibInfo.qspiLibFlashInfo.blockSize;
            for (i = startBlockNumber; i <= endBlockNumber; i++)
            {
                QSPILibBlockErase(&qspiLibInfo, i);
            }
            //CONSOLEUtilsPrintf("vxworks Flashing started %x length = %d\n offsetaddr = %x\n", offsetAddr,gQspiAppFlashWriterObj.length,offsetAddr);
            /* Write image to QSPI flash */
            QSPILibWrite(&qspiLibInfo,
                        offsetAddr,
                        (uint32_t)vxWorks,
                        gQspiAppFlashWriterObj.length);

            CONSOLEUtilsPrintf("vxworks Flashing completed \n");

            CONSOLEUtilsPrintf(" \n\n>> READING THE FWINFO STRUCTURE AFTER IT IS WRITTEN TO FLASH : BEFORE THE QUAD MODE IS ENABLED\n");
#endif
            CONSOLEUtilsPrintf("Changing read to quad mode \n");

            flashDeviceData = QSPIFlashGetDeviceData(gQspiAppFlashWriterObj.devId);
            if(QSPI_LIB_READ_TYPE_QUAD == qspiLibInfo.qspiLibCtrlInfo.qspiLibReadType)
            {
                /* Enter quad mode if Quad read mode is selected */
                QSPILibQuadEnable(&qspiLibInfo, flashDeviceData);
            }
            CONSOLEUtilsPrintf("\nRead mode has been changed to Quad mode \n");
            CONSOLEUtilsPrintf("SUCCESS!!!\n\n");


            //READING THE FWINFO STRUCTURE AFTER IT IS WRITTEN TO FLASH
#if 1
            CONSOLEUtilsPrintf(" >> READING THE FWINFO STRUCTURE AFTER IT IS WRITTEN TO FLASH\n");
            fwflashinfo_t.boot_status = 0;
            fwflashinfo_t.fw_upgrade = 0;
            fwflashinfo_t.partition_a_offset = 0;
            fwflashinfo_t.partition_b_offset = 0;
            fwflashinfo_t.partition_a_size = 0;
            fwflashinfo_t.partition_b_size = 0;
            fwflashinfo_t.partition_a_crc = 0;
            fwflashinfo_t.partition_b_crc = 0;

            gQspiAppFlashWriterObj.length = sizeof(FwFlashInfo);
            offsetAddr = REF_STRUCT_QSPI_OFFSET;
            QSPILibRead(&qspiLibInfo,offsetAddr,
                         (uint32_t)(fwflashinfo), gQspiAppFlashWriterObj.length);

            CONSOLEUtilsPrintf(" >> fwflashinfo_t.boot_status =  %x \n",fwflashinfo_t.boot_status);
              CONSOLEUtilsPrintf(" >> fwflashinfo_t.fw_upgrade =  %x \n",fwflashinfo_t.fw_upgrade);
              CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_a_offset =  %x \n",fwflashinfo_t.partition_a_offset);
              CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_b_offset =  %x \n",fwflashinfo_t.partition_b_offset);
              CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_a_size = %x \n",fwflashinfo_t.partition_a_size );
              CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_b_size =  %x \n",fwflashinfo_t.partition_b_size);
              CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_a_crc = %x \n",fwflashinfo_t.partition_a_crc);
              CONSOLEUtilsPrintf(" >> fwflashinfo_t.partition_b_crc = %x \n",fwflashinfo_t.partition_b_crc);



            gQspiAppFlashWriterObj.length = sizeof(u_boot);
            offsetAddr = UBOOT_QSPI_OFFSET;
            QSPILibRead(&qspiLibInfo,offsetAddr,
                         (uint32_t)crc_check, gQspiAppFlashWriterObj.length);

            actualRead_crc[0] = ByteCRC(crc_check, sizeof(u_boot));
            if(image_crc[0] == actualRead_crc[0])
            {
                CONSOLEUtilsPrintf("U-boot CRC matched\n");
            }
            else
            {
                CONSOLEUtilsPrintf("U-boot CRC failed\n");
                CONSOLEUtilsPrintf("Re-flash the binaries using JTAG\n");

                crcFail = TRUE;
                while(crcFail);
            }

            offsetAddr = PARTITION_A_OFFSET;
            gQspiAppFlashWriterObj.length = sizeof(vxWorks);
            QSPILibRead(&qspiLibInfo,offsetAddr,
                         (uint32_t)crc_check, gQspiAppFlashWriterObj.length);

            actualRead_crc[1] = ByteCRC(crc_check, sizeof(vxWorks));
            if(image_crc[1] == actualRead_crc[1])
            {
                CONSOLEUtilsPrintf(" Vxworks CRC matched\n");
            }
            else
           {
               CONSOLEUtilsPrintf("Vxworks CRC failed\n");
               CONSOLEUtilsPrintf("Re-flash the binaries using JTAG\n");

               crcFail = TRUE;
               while(crcFail);
           }
#endif
            CONSOLEUtilsPrintf("rebooting ..... \n");
            /* reset PRCM control register */
            HW_WR_REG32(SYS_CONFIG_RESET_BASE_ADDR, 0x02);

        }

    }
    return (S_PASS);
}
