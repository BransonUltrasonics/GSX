/**
 *  qspi_lib.c
 *
 *  This file adds qspi library API's for read, write, erase
 *         from QSPI flash. This also initialize the QSPI controller
 *         to start transfers.
 */

#include <types.h>
#include <hw_types.h>
#include <vxbTiAm437xQspi.h>
#include <qspi_flash.h>
#include <vxbAm437xQspiLib.h>

#include <stdio.h>
#include <errno.h>
#include <private/stdioP.h>
#include <ioLib.h>
#define  QSPI_DBG_ON 	/* turn on/off debug messages */
#ifdef  QSPI_DBG_ON

#define DEBUG_PRINT 1
/* turning local symbols into global symbols */

#define QSPI_DBG_OFF            0x00000000
#define QSPI_DBG_ISR            0x00000001
#define QSPI_DBG_RW             0x00000002
#define QSPI_DBG_ERR            0x00000004
#define QSPI_DBG_RTN            0x00000008
#define QSPI_DBG_INFO		   0x0000000C	
#define QSPI_DBG_ALL            0xffffffff

LOCAL UINT32 qspiDbgMask = QSPI_DBG_ALL;

#define SPI_DBG(mask, ...)                                              \
    do                                                                  \
        {                                                               \
        if ((qspiDbgMask & mask) || (mask == QSPI_DBG_ALL))               \
            {                                                           \
            if (_func_kprintf != NULL)                                  \
                {                                                       \
                (* _func_kprintf)(__VA_ARGS__);                         \
                }                                                       \
            }                                                           \
        }                                                               \
    while ((FALSE))
#else
#undef SPI_DBG
#define SPI_DBG(...)
#endif  /* QSPI_DBG_ON */


/** Memory size 1 byte */
#define MEM_SIZE_1B                (1U)
/** Memory size 1 kilo-byte */
#define MEM_SIZE_KB                (1024U * MEM_SIZE_1B)
/** Memory size 1 mega-byte */
#define MEM_SIZE_MB                (1024U * MEM_SIZE_KB)
/** Memory size 1 gega-byte */
#define MEM_SIZE_GB                (1024U * MEM_SIZE_MB)


/** Frequency macros */
/** Frequency in kilo-hertz */
#define FREQ_KHZ                   (1000U)
/** Frequency in mega-hertz */
#define FREQ_MHZ                   (1000U * FREQ_KHZ)
/** Frequency in giga-hertz */
#define FREQ_GHZ                   (1000U * FREQ_MHZ)

#define DEVICE_ID_INVALID   				0xFFFFFFFFU



static const qspiObj_t QSPIOBJ_DEFAULT =
{
    0U,  /* chipSelect */
    0U,  /* instNum */
    0U,  /* instAddr */
    DEVICE_ID_INVALID,  /* devId */
    0U,  /* length*/
    {
        {
        	0U, /* pDev */
            0U, /* chipSelect */
			QSPI_FLASH_MMAP_START, /* memMapBaseAddr */
			QSPI_CORE_START, /* cfgBaseAddr */
            QSPI_LIB_READ_TYPE_QUAD, /* qspiLibReadType */
			//QSPI_LIB_TX_MODE_CONFIG_MODE,
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

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */


uint32_t QspiLibFlashStatus(void);


void QspiLibWriteEnable(void);


void QspiLibEnable4ByteAddrMode(
                             uint32_t enable4ByteAddrMode);

void QspiLibMemMapWrite(
                        uint32_t dstOffsetAddr,
                        uint32_t srcAddr,
                        uint32_t length);

void QspiLibCfgModeWrite(
                         uint32_t dstOffsetAddr,
                         uint32_t srcAddr,
                         uint32_t length);

void QspiLibMemMapRead(
                       uint32_t srcOffsetAddr,
                       uint32_t dstAddr,
                       uint32_t length);

void QspiLibCfgModeRead(
                        uint32_t srcOffsetAddr,
                        uint32_t dstAddr,
                        uint32_t length);

LOCAL qspiLibInfo_t pQspiLibInfo;

/**
 * This API stores the QSPI Controller Driver instance &
 *        Instantiate Flash Library struct as well .
 *
 * pDrvCtrl       Pointer to QSPI Controller Driver
 *
 */
void QSPIRegister
    (
	TI_AM437X_SPI_CTRL *    pDrvCtrl
    )
    {
	qspiObj_t* gQSPIFlashObj;
	qspiLibFlashInfo_t qspiLibFlashInfo;
	
	gQSPIFlashObj = (qspiObj_t*)vxbMemAlloc(sizeof(qspiObj_t));
	qspiLibInfo_t qspiLibInfo;
	*gQSPIFlashObj = QSPIOBJ_DEFAULT;
	
	pQspiLibInfo = gQSPIFlashObj->qspiLibInfo;
	pQspiLibInfo.qspiLibCtrlInfo.pDev = pDrvCtrl->pDev;
	pQspiLibInfo.qspiLibCtrlInfo.memMapBaseAddr = pDrvCtrl->regFlashBase;
	}

/**
 * This API initializes the QSPI controller for
 *        read/ write and erase in desired mode.
 *
 * pQspiLibInfo       Pointer to structure containing QSPI controller and
 *                            Flash device Info.
 *
 */
void QSPILibInit()
{
    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
    uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    /** Setting IDLE mode for QSPI controller */
    QSPISetIdleMode(pDev, baseAddr, QSPI_SYSCONFIG_IDLE_MODE_NO);
    /** Set clock mode in mode 3 */
    QSPISetClkMode(pDev, baseAddr, chipSelect, QSPI_CLK_MODE_3);
    QSPISetCsPol(pDev, baseAddr, chipSelect, QSPI_CS_POL_ACTIVE_LOW);
    QSPISetDataDelay(pDev, baseAddr, chipSelect, QSPI_DATA_DELAY_0);
    /**
     *  Setting write command for memory mapped mode in init
     *  as there is only one write type possible i.e. single write
     */
    QSPISetMemMapWriteCmd(pDev, baseAddr, chipSelect, QSPI_LIB_CMD_PAGE_PRG);
    /* Default number of address bytes are 3 */
    QSPISetMemMapNumAddrBytes(pDev, baseAddr,
                      chipSelect,
                              QSPI_MEM_MAP_NUM_ADDR_BYTES_THREE);
    /* Enable clock and set divider value */
    QSPISetPreScaler(pDev, baseAddr, 0x0U);
    /* Enable memory mapped port */
    QSPISetMemAddrSpace(pDev, baseAddr,
                        chipSelect,
                        QSPI_MEM_MAP_PORT_SEL_MEM_MAP_PORT);
}

/**
 * This API is used to read data from the QSPI flash device.
 *         The number of bytes to be read is passed as function parameter.
 *
 * pQspiLibInfo       Pointer to structure containing QSPI controller and
 *                            Flash device Info.
 * srcOffsetAddr    Source Offset address
 * dstAddr          destination address
 * length           length of data to be read
 *
 * retval OK          Read successful
 * retval ERROR          Read failed
 *
 */
int32_t QSPILibRead(
                 uint32_t srcOffsetAddr,
                 uint32_t dstAddr,
                 uint32_t length)
{
    int32_t retval = OK;
    if (QSPI_LIB_TX_MODE_CONFIG_MODE ==
            pQspiLibInfo.qspiLibCtrlInfo.qspiLibTxMode)
    {
        QspiLibCfgModeRead(srcOffsetAddr, dstAddr, length);
    } else if (QSPI_LIB_TX_MODE_MEMORY_MAPPED ==
                   pQspiLibInfo.qspiLibCtrlInfo.qspiLibTxMode)
    {
        QspiLibMemMapRead(srcOffsetAddr, dstAddr, length);
    } else
    {
        retval = ERROR;
    }
    return retval;
}

/**
 * This API writes data to flash device using QSPI controller.
 *
 * pQspiLibInfo       Pointer to structure containing QSPI controller and
 *                            Flash device Info.
 * dstOffsetAddr       Destination offset address
 * srcAddr             Source address
 * length              Length of data to be written
 *
 * retval OK             Write Successful
 * retval ERROR          Write failed
 */
int32_t QSPILibWrite(
                  uint32_t dstOffsetAddr,
                  uint32_t srcAddr,
                  uint32_t length)
{
    int32_t retval = OK;
    
    if(QSPI_LIB_TX_MODE_CONFIG_MODE ==
          pQspiLibInfo.qspiLibCtrlInfo.qspiLibTxMode)
    {
    	
        /* Max number of bytes written at one time in config mode is 256
           This is taken care by the qspi lib. Application can pass any length
           to be written
         */
        while (length > 256U)
        {
        	
             QspiLibCfgModeWrite(dstOffsetAddr,srcAddr, 256U);
             length -=256U;
             dstOffsetAddr += 256U;
             srcAddr += 256U;
        }
        if (length > 0U)
        {
        	
             QspiLibCfgModeWrite( dstOffsetAddr,srcAddr, length);
        }
    } else if (QSPI_LIB_TX_MODE_MEMORY_MAPPED ==
                  pQspiLibInfo.qspiLibCtrlInfo.qspiLibTxMode)
    {
    	
        QspiLibMemMapWrite( dstOffsetAddr,srcAddr, length);
    } else
    {
    	
        retval = ERROR;
    }
    return retval;
}

/**
 * This API erases a block of QSPI flash device
 * by sending the command through QSPI controller.
 *
 * pQspiLibInfo       Pointer to structure containing QSPI controller and
 *                            Flash device Info.
 * blockNumber     The block number from where erase has to start.
 */
void QSPILibBlockErase(uint32_t blockNumber)
{

    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
	uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    uint32_t data;
    qspiTxCmd_t qspiTxCmd = {0};
    uint32_t eraseAddr       = blockNumber * 
							   pQspiLibInfo.qspiLibFlashInfo.blockSize;
    uint32_t wordLength = 24U;
    /* If source offset address is greater than 24 bits
     * Enter 32 bit address mode */
    if(eraseAddr > 0xFFFFFFU)
    {
        /* Enter 32 bit address mode */
        QspiLibEnable4ByteAddrMode(TRUE);
        wordLength = 32U;

    }
    QspiLibWriteEnable();
    data = QSPI_LIB_CMD_BLOCK_ERASE;
    QSPIWriteData(pDev, baseAddr, &data, 1U);

    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    qspiTxCmd.frameLength = 2U;
    qspiTxCmd.wordLength = 8U;
    qspiTxCmd.chipSelect = chipSelect;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));

    data = eraseAddr;
    kprintf("Erase Sector No  0x%08x\n",eraseAddr);
    
    QSPIWriteData(pDev, baseAddr,&data, 1U);
    qspiTxCmd.wordLength = wordLength;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));
    /* Wait while busy */
    
    while (1U == (QspiLibFlashStatus() & 0x1U));
    if(eraseAddr > 0xFFFFFFU)
    {
        /* Exit 32 bit address mode */
        QspiLibEnable4ByteAddrMode(FALSE);
    }
}

/**
 * This API sends chip erase command using QSPI
 * to erase whole QSPI flash memory.
 *
 * pQspiLibInfo       Pointer to structure containing QSPI controller and
 *                            Flash device Info.
 */
void QSPILibChipErase()
{
    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
	uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    uint32_t data;
    qspiTxCmd_t qspiTxCmd = {0};

    /* Write enable flash device by sending write enable command */
    QspiLibWriteEnable();

    /* This is for sending full erase command for QSPI flash device */
    data = QSPI_LIB_CMD_FULL_ERASE;
    QSPIWriteData(pDev, baseAddr, &data, 1U);
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    qspiTxCmd.frameLength = 1U;
    qspiTxCmd.wordLength = 8U;
    qspiTxCmd.chipSelect = chipSelect;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    /* Wait while flash is getting erased */
    while (1U == (QspiLibFlashStatus() & 0x1U));
}
/**
 * This API reads the device ID and manufacturer ID
 * from QSPI flash device and display the result on the console.
 *
 * pQspiLibInfo       Pointer to structure containing QSPI controller and
 *                            Flash device Info.
 */
void QSPILibPrintId()
{
    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
    uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    qspiTxCmd_t qspiTxCmd = {0};
    uint32_t deviceId, mfgId;
    uint32_t writeVal;

    /* Write Command */
    writeVal = QSPI_LIB_CMD_READ_MFG_ID;
    QSPIWriteData(pDev, baseAddr, &writeVal, 1U);
    qspiTxCmd.frameLength = 4U;
    qspiTxCmd.wordLength = 8U;
    qspiTxCmd.chipSelect = chipSelect;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));

    /* Write Address Bytes */
    writeVal = 0x0U;
    QSPIWriteData(pDev, baseAddr, &writeVal, 1U);
    qspiTxCmd.wordLength = 24U;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));

    /* Read Manufacturer ID
     * The manufacturer ID is of 1 byte(8 bits)
     */
    qspiTxCmd.wordLength = 8U;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_READ_SINGLE;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));
    QSPIReadData(pDev, baseAddr, &mfgId, 1U);
    SPI_DBG(QSPI_DBG_INFO,"Flash Manufacturer ID = %x\n",mfgId);

    /* Read Device ID */
    qspiTxCmd.wordLength = 8U;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_READ_SINGLE;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));

    QSPIReadData(pDev, baseAddr, &deviceId, 1U);
    SPI_DBG(QSPI_DBG_INFO,"Device ID = %x\n",deviceId);
    /* Note - This ID is the device ID of the flash device.
     * This ID is read from the flash
     */
}
/**
 * This API enables the quad enable bit in the flash device
 *
 * pQspiLibInfo       Pointer to structure containing QSPI controller and
 *                            Flash device Info.
 * qspiFlashDeviceData      qspi flash specific data required to enable the quad
 *                                 quad enable bit.
 */
void QSPILibQuadEnable(qspiFlashDeviceData_t *qspiFlashDeviceData)
{
    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
    uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    uint32_t data;
    uint32_t norStatus;
    qspiTxCmd_t qspiTxCmd = {0};

    /* Read Status register */
    data = qspiFlashDeviceData->quadReadCmd;
    QSPIWriteData(pDev, baseAddr, &data, 1U);
    qspiTxCmd.frameLength = 2U;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    qspiTxCmd.wordLength  = 8U;
    qspiTxCmd.chipSelect = chipSelect;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_READ_SINGLE;
    qspiTxCmd.wordLength  = 8U;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));
    QSPIReadData(pDev, baseAddr, &norStatus, 1U);

    /* Send Write Enable before writing status register Quad enable bit */
    QspiLibWriteEnable();

    /* Send write status register command */
    data = qspiFlashDeviceData->quadWriteCmd;
    QSPIWriteData(pDev, baseAddr, &data, 1U);
    qspiTxCmd.frameLength = 2U;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    qspiTxCmd.wordLength  = 8U;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));

    /* Set status register 6th bit to 1 for Quad enable
     * Write this value to the status register.
     */
    norStatus &= ~(1U << qspiFlashDeviceData->quadEnableBit);
    norStatus |= (qspiFlashDeviceData->quadValue << qspiFlashDeviceData->quadEnableBit);
    QSPIWriteData(pDev, baseAddr, &norStatus, 1U);
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    qspiTxCmd.wordLength  = 8U;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    /* wait till the command is being send */
    while (TRUE == QSPIIsBusy(pDev, baseAddr));
    /* Wait till the status register is being written */
    while (1U == (QspiLibFlashStatus() & 0x1U));
}

/**
 * This API writes data to flash device
 *         using QSPI controller in memory mapped mode.
 *
 * pQspiLibInfo    Pointer to structure containing QSPI controller
 *                         and flash device Info.
 * dstOffsetAddr    destination offset address
 * srcAddr          Source address
 * length           length of data to be written.
 *
 */

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress (float percentage)
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;

    kprintf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}

void QspiLibMemMapWrite(
                     uint32_t dstOffsetAddr,
                     uint32_t srcAddr,
                     uint32_t length)
{
    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
    uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    uint32_t dstAddr = pQspiLibInfo.qspiLibCtrlInfo.memMapBaseAddr +
                       dstOffsetAddr;
    uint32_t *pDestAddr = (uint32_t *) (dstAddr);
    uint32_t *pSrcAddr = (uint32_t *) (srcAddr);
    uint32_t i;
    int pct;
    
   /* If destination offset address is greater than 24 bits
    * Enter 32 bit address mode */
    if(dstOffsetAddr > 0xFFFFFFU)
    {
        /* Enter 32 bit address mode */
        QspiLibEnable4ByteAddrMode( TRUE);
        QSPISetMemMapNumAddrBytes(pDev, baseAddr,
                                  chipSelect,
                                  QSPI_MEM_MAP_NUM_ADDR_BYTES_FOUR);
    }

    for (i = 0U; i < (length/4U); i++)
    {
        QspiLibWriteEnable();
        *pDestAddr = *pSrcAddr;
        pDestAddr++;
        pSrcAddr++;
        while ((QspiLibFlashStatus() & 0x1U));
        //printProgress(((float) i/((float) length/4U)));
        //printf("\rIn progress %d", (i/(length/4U) * 100));
        
    }
    if (0U != (length % 4U))
    {
        /* In case number of bytes is not a multiple of 4
         * One extra word is written to ensure complete data is written
         * and no data is lost.
         */
        QspiLibWriteEnable();
        *pDestAddr = *pSrcAddr;
        pDestAddr++;
        pSrcAddr++;
        while ((QspiLibFlashStatus() & 0x1U));
        printProgress(100);
    }
    if(dstOffsetAddr > 0xFFFFFF)
    {
        /* Exit 32 bit address mode */
        QspiLibEnable4ByteAddrMode( FALSE);
        QSPISetMemMapNumAddrBytes(pDev, baseAddr,
                                 chipSelect,
                                 QSPI_MEM_MAP_NUM_ADDR_BYTES_THREE);
    }
}



/**
 * This API writes data to flash device
 * using QSPI controller in config mode.
 *
 * pQspiLibInfo       Pointer to structure containing QSPI controller and
 *                         Flash device Info.
 * dstOffsetAddr    destination offset address
 * srcAddr          Source address
 * length           length of data to be written.
 *
 */
void QspiLibCfgModeWrite(
                      uint32_t dstOffsetAddr,
                      uint32_t srcAddr,
                      uint32_t length)
{
	
    /* Config mode write */
    qspiTxCmd_t qspiTxCmd = {0};
    uint32_t writeVal;
    uint32_t i;
    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
    uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    uint32_t addrWordLength = 24U;

    
    /* If destination offset address is greater than 24 bits
     * Enter 32 bit address mode */
    if(dstOffsetAddr > 0xFFFFFFU)
    {
        QspiLibEnable4ByteAddrMode( TRUE);
        addrWordLength = 32U;
    }

    
    QspiLibWriteEnable();
    /* Write Command */
    writeVal = QSPI_LIB_CMD_PAGE_PRG;
    
    QSPIWriteData(pDev, baseAddr, &writeVal, 1U);
    qspiTxCmd.frameLength = (length / 4U) + 2U;
    qspiTxCmd.wordLength = 8U;
    qspiTxCmd.chipSelect = chipSelect;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));

    /* Write Address Bytes */
    writeVal = dstOffsetAddr;
    
    QSPIWriteData(pDev, baseAddr, &writeVal, 1U);
    qspiTxCmd.wordLength = addrWordLength;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));

    qspiTxCmd.wordLength = 32U;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    
    for(i = 0U; i < (length/4U); i++)
    {
        QSPIWriteData(pDev, baseAddr, (uint32_t *)srcAddr, 1U);
        srcAddr += 4U;
        QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
        while (TRUE == QSPIIsBusy(pDev, baseAddr));
    }
    
    if(0U != (length % 4U))
    {
        /* In case number of bytes is not a multiple of 4
         * Remaining bytes are written to the flash device
         */
        qspiTxCmd.wordLength = (length % 32U);
        qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
        
        QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
        QSPIWriteData(pDev, baseAddr, (uint32_t *)srcAddr, 1U);
        while (TRUE == QSPIIsBusy(pDev, baseAddr));
    }
    
    while ((QspiLibFlashStatus() & 0x1U));
    if(dstOffsetAddr > 0xFFFFFFU)
    {
        /* Exit 32 bit address mode */
    	
        QspiLibEnable4ByteAddrMode( FALSE);
    }
    
}
/**
 * This APIs reads length bytes of data from flash device
 * using QSPI controller in memory mapped mode.
 *
 * pQspiLibInfo    Pointer to structure containing QSPI controller
 *                         and flash device Info.
 * srcOffsetAddr    Source Offset address
 * dstAddr          destination address
 * length           length of data to be read.
 *
 */
void QspiLibMemMapRead(
                    uint32_t srcOffsetAddr,
                    uint32_t dstAddr,
                    uint32_t length)
{
    /* Read from QSPI in memory mapped mode */
    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
    uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    uint32_t srcAddr = pQspiLibInfo.qspiLibCtrlInfo.memMapBaseAddr +
                       srcOffsetAddr;
    uint32_t *pDestAddr = (uint32_t *) (dstAddr);
    uint32_t *pSrcAddr = (uint32_t *) (srcAddr);
    uint32_t i;

    switch(pQspiLibInfo.qspiLibCtrlInfo.qspiLibReadType)
    {
        case QSPI_LIB_READ_TYPE_SINGLE:
            /**< Single read mode */
            QSPISetMemMapReadCmd(pDev, baseAddr,
                                 chipSelect,
                                 QSPI_LIB_CMD_READ_SINGLE);
            QSPISetMemMapReadType(pDev, baseAddr,
                                  chipSelect,
                                  QSPI_MEM_MAP_READ_TYPE_NORMAL);
            QSPISetMemMapNumDummyBits(pDev, baseAddr, chipSelect, 0x0U);
            break;

        case QSPI_LIB_READ_TYPE_DUAL:
        /**< Dual read mode */
            QSPISetMemMapReadCmd(pDev, baseAddr, chipSelect, QSPI_LIB_CMD_READ_DUAL);
            QSPISetMemMapReadType(pDev, baseAddr,
                                  chipSelect,
                                  QSPI_MEM_MAP_READ_TYPE_DUAL);
            QSPISetMemMapNumDummyBits(pDev, baseAddr, chipSelect, 0x8U);
            break;

        case QSPI_LIB_READ_TYPE_QUAD:
            /**< Quad read mode */
            QSPISetMemMapReadCmd(pDev, baseAddr, chipSelect, QSPI_LIB_CMD_READ_QUAD);
            QSPISetMemMapReadType(pDev, baseAddr,
                                  chipSelect,
                                  QSPI_MEM_MAP_READ_TYPE_QUAD);
            QSPISetMemMapNumDummyBits(pDev, baseAddr, chipSelect, 0x8U);
            break;
    }

   /* If source offset address is greater than 24 bits
    * Enter 32 bit address mode */
    if(srcOffsetAddr > 0xFFFFFF)
    {
        /* Enter 32 bit address mode */
        QspiLibEnable4ByteAddrMode( TRUE);
        QSPISetMemMapNumAddrBytes(pDev, baseAddr,
                                  chipSelect,
                                  QSPI_MEM_MAP_NUM_ADDR_BYTES_FOUR);
    }
    for (i = 0U; i < (length/4U); i++)
    {
        *pDestAddr = *pSrcAddr;
        pDestAddr++;
        pSrcAddr++;
    }
    if ((length % 4U) != 0U)
    {
        /* In case number of bytes is not a multiple of 4
         * One extra word is read from the flash to ensure that no
         * data is lost.
         */
        *pDestAddr = *pSrcAddr;
        pDestAddr++;
        pSrcAddr++;
    }
    if(srcOffsetAddr > 0xFFFFFFU)
    {
        /* Exit 32 bit address mode */
        QspiLibEnable4ByteAddrMode( FALSE);
        QSPISetMemMapNumAddrBytes(pDev, baseAddr,
                                  chipSelect,
                                  QSPI_MEM_MAP_NUM_ADDR_BYTES_THREE);
    }

}
/**
 * This APIs reads length byte of data from flash device
 *         using QSPI controller in config mode.
 *
 * pQspiLibInfo    Pointer to structure containing QSPI controller
 *                         and flash device Info.
 * srcOffsetAddr    Source Offset address
 * dstAddr          destination address
 * length           length of data to be read.
 *
 */
void QspiLibCfgModeRead(
                     uint32_t srcOffsetAddr,
                     uint32_t dstAddr,
                     uint32_t length)
{
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
    uint32_t addrWordLength = 24U;
    /** Config mode read */
    qspiTxCmd_t qspiTxCmd = {0};
    uint32_t readCmd = 0U, txCmd = 0U;
    uint32_t numDummyBits = 0U, frameLength = 0U;
    uint32_t writeVal;
    uint32_t i;

    /* If source offset address is greater than 24 bits
     * Enter 32 bit address mode */
    if(srcOffsetAddr > 0xFFFFFFU)
    {
        /* Enter 32 bit address mode */
        QspiLibEnable4ByteAddrMode( TRUE);
        addrWordLength = 32U;
    }
    /* The readCmd, txCmd, numDummyBits and the frameLength are
     * set according the the qspi read type
     */
    switch(pQspiLibInfo.qspiLibCtrlInfo.qspiLibReadType)
    {
        case QSPI_LIB_READ_TYPE_SINGLE:
            readCmd = QSPI_LIB_CMD_READ_SINGLE;
            txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_READ_SINGLE;
            numDummyBits = 0U;
            frameLength = (length / 4U) + 2U;
            break;
        case QSPI_LIB_READ_TYPE_DUAL:
            readCmd = QSPI_LIB_CMD_READ_DUAL;
            txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_READ_DUAL;
            numDummyBits = 8U;
            frameLength = (length / 4U) + 3U;
            break;
        case QSPI_LIB_READ_TYPE_QUAD:
            readCmd = QSPI_LIB_CMD_READ_QUAD;
            txCmd = QSPI_CFG_MODE_TX_CMD_SIX_PIN_READ_QUAD;
            numDummyBits = 8U;
            frameLength = (length / 4U) + 3U;
            break;
    }
    /* Write Command */
    writeVal = readCmd;
    QSPIWriteData(pDev, baseAddr, &writeVal, 1U);
    qspiTxCmd.frameLength = frameLength;
    qspiTxCmd.wordLength = 8U;
    qspiTxCmd.chipSelect = chipSelect;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));
    if(0U != numDummyBits)
    {
        /**< Write dummy bytes for fast read if required */
        writeVal = 0U;
        QSPIWriteData(pDev, baseAddr, &writeVal, 1U);
        qspiTxCmd.wordLength = numDummyBits;
        qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
        QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
        while (TRUE == QSPIIsBusy(pDev, baseAddr));
    }
    /* Write Address Bytes */
    writeVal = srcOffsetAddr;
    QSPIWriteData(pDev, baseAddr, &writeVal, 1U);
    qspiTxCmd.wordLength = addrWordLength;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev,  baseAddr));
    qspiTxCmd.wordLength = 32U;
    qspiTxCmd.txCmd = txCmd;
    for(i = 0U; i < (length/4U); i++)
    {
        QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
        while (TRUE == QSPIIsBusy(pDev, baseAddr));
        QSPIReadData(pDev, baseAddr, (uint32_t *)dstAddr, 1U);
        dstAddr += 4U;
    }
    if (0U != (length % 4U))
    {
        /* In case number of bytes is not a multiple of 4
         * Read remaining bytes from the flash device */
        qspiTxCmd.wordLength = (length % 32U);
        qspiTxCmd.txCmd = txCmd;
        QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
        while (TRUE == QSPIIsBusy(pDev, baseAddr));
        QSPIReadData(pDev, baseAddr, (uint32_t *)dstAddr, 1U);
    }
    if(srcOffsetAddr > 0xFFFFFFU)
    {
        /* Exit 32 bit address mode */
        QspiLibEnable4ByteAddrMode(FALSE);
    }
}

/**
 * This function will return the value of the QSPI flash status register.
 *
 * pQspiLibInfo    Pointer to structure containing QSPI
 *                         controller and flash device Info.
 *
 * flashStatus Value of flash status register.
 */
uint32_t QspiLibFlashStatus()
{
	 
    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
    uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    qspiTxCmd_t qspiTxCmd = {0};
    uint32_t data, writeVal;
    /* Write Address Bytes */
    
    writeVal = QSPI_LIB_CMD_READ_STATUS_REG;
    QSPIWriteData(pDev, baseAddr, &writeVal, 1U);
    qspiTxCmd.frameLength = 2U;
    qspiTxCmd.chipSelect = chipSelect;
    qspiTxCmd.wordLength = 8U;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    
    while (TRUE == QSPIIsBusy(pDev, baseAddr));
    
    /* Read data status register to check write in progress bit */
    qspiTxCmd.wordLength = 8U;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_READ_SINGLE;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    
    while (TRUE == QSPIIsBusy(pDev, baseAddr));
    
    QSPIReadData(pDev, baseAddr, &data, 1U);
    return (data & 0xFFU);
}
/**
 * This function sends write enable command to the flash device.
 *
 * pQspiLibInfo    Pointer to structure containing QSPI
 *                         controller and flash device Info.
 */
void QspiLibWriteEnable()
{
    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
    uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    qspiTxCmd_t qspiTxCmd = {0};
    uint32_t data = QSPI_LIB_CMD_WRITE_ENABLE;
    /* Sets the write enable bit of the Serial flash device used */

    QSPIWriteData(pDev, baseAddr, &data, 1U);

    qspiTxCmd.frameLength = 1U;
    qspiTxCmd.chipSelect = chipSelect;
    qspiTxCmd.wordLength = 8U;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev,  baseAddr));

}
/**
 * This API allows flash to enter/exit 32 bit addressing mode
 *
 * pQspiLibInfo    Pointer to structure containing QSPI controller
 *                         and flash device Info.
 * enable4ByteAddrMode  TRUE - Enter 4 byte addressing mode
 *                             FALSE  Exit four byte addressing mode
 */
void QspiLibEnable4ByteAddrMode(uint32_t flag)
{
    uint32_t chipSelect = pQspiLibInfo.qspiLibCtrlInfo.chipSelect;
    uint32_t baseAddr = pQspiLibInfo.qspiLibCtrlInfo.cfgBaseAddr;
    VXB_DEV_ID pDev = pQspiLibInfo.qspiLibCtrlInfo.pDev;
    qspiTxCmd_t qspiTxCmd = {0};
    uint32_t data;
    if(TRUE == flag)
    {
        data = QSPI_LIB_CMD_ENTER_4_BYTE_ADDR;
    }
    else if(FALSE == flag)
    {
        data = QSPI_LIB_CMD_EXIT_4_BYTE_ADDR;
    }

    QSPIWriteData(pDev, baseAddr, &data, 1U);

    qspiTxCmd.frameLength = 1U;
    qspiTxCmd.chipSelect = chipSelect;
    qspiTxCmd.wordLength = 8U;
    qspiTxCmd.txCmd = QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE;
    QSPISetCfgModeTxCmd(pDev, baseAddr, qspiTxCmd);
    while (TRUE == QSPIIsBusy(pDev, baseAddr));
}
