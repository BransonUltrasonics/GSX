/*
 * customSystemCallHdlr.c
 *
 *  Created on: Oct 16, 2017
 *      Author: EE208214
 */

#ifndef __INCcustomSystemusrCallh
#define __INCcustomSystemusrCallh

/* Header files */
#include <vxWorks.h>
typedef void (*EHRPWMDRV_IRQ_HANDLER)(void);


/* Global Variables */

/*
 * Timer Structure for set and get time custom system call
 */
typedef struct
{
	int tm_sec;         /* seconds after the minute     - [0, 59] */
	int tm_min;         /* minutes after the hour       - [0, 59] */
	int tm_hour;        /* hours after midnight         - [0, 23] */
	int tm_mday;        /* day of the month             - [1, 31] */
	int tm_mon;         /* months since January         - [0, 11] */
	int tm_year;        /* years since 1900     */
	int tm_wday;        /* days since Sunday            - [0, 6] */
	int tm_yday;        /* days since January 1         - [0, 365] */
	int tm_isdst;       /* Daylight Saving Time flag */
}RTCtimerStr;


/*
 * The vxbSpiDevInfo struct used to store the SPI device feature, such as
 * chipSelect, spi_device mode,etc.
 */

typedef struct vxbSpiDevInfo
    {
    /* SPI device name, used for match the corresponding driver */

    char *  deviceName;

   /* connect on which CS */

    UINT8   chipSelect;

    /* the bit width per words */

    UINT8   bitWidth;

    /* the maximum work frequency of the SPI device */

    UINT32  devFreq;

    /* device mode - see SPI device mode defines */

    UINT32  mode;

    /* the number of SPI devices in a group */

    UINT32  chipNum;

    /*
     * used data lines
     * For SPI devices, the value must be 1. For QSPI devices, the value
     * can be 1, 2 and 4.
     */

    UINT32  dataLines;
    }SPI_HARDWARE;

 
/*
 * The spiTransfer struct used to store the SPI transfer data info,
 * the spi device driver will fill this struct.
 */

typedef struct spiTransfer 
    {
    UINT8 * txBuf;    /* data to be written, or NULL */
    UINT8 * rxBuf;    /* data to be read, or NULL */
    UINT32  txLen;    /* size of tx buffers in bytes */
    UINT32  rxLen;    /* size of rx buffers in bytes */
    UINT32  usDelay;
    UINT32  base;     /* flash base address of transfer */
    }SPI_TRANSFER;


// Exposing custom system calls to RTP     
extern int AuxClkConnect(int task_id);
extern void AuxClkEnable(void);
extern void AuxClkDisable(void);
extern int AuxClkRateGet(void);
extern int AuxClkRateSet(int ticksPerSecond);
extern void SetRTC(RTCtimerStr time);
extern void GetRTC(RTCtimerStr * pTime);
extern int GpioAlloc(unsigned int);
extern unsigned int GpioGetValue(unsigned int);
extern int GpioSetValue(unsigned int, unsigned int);
extern int GpioSetDir(unsigned int, unsigned int);
extern int EhrPwmRegisterISR(EHRPWMDRV_IRQ_HANDLER irqHandler, void* ehrpwmHandlerArgs, UINT8 ehrpwm_inst, int period, int mode);
extern STATUS EhrPwmSetAQSFRC(UINT32 val, UINT8	ehrpwm_inst);
extern STATUS EhrPwmSetTbClk(uint32_t tb, UINT8	ehrpwm_inst);
extern STATUS EhrPwmSetTbAQConfA(uint32_t tb, UINT8	ehrpwm_inst);
extern STATUS EhrPwmSetTbAQConfB(uint32_t tb, UINT8	ehrpwm_inst);
extern STATUS EhrPwmSetTbPWMCfg(uint32_t freq, uint32_t dir, UINT8 ehrpwm_inst);
extern STATUS EqepSetMaxCount(int eqep, int count);
extern STATUS EqepGetPosCount(int eqep);
extern STATUS McSpiTrans(SPI_HARDWARE *  devInfo, SPI_TRANSFER *  pPkg, int	channel);
extern void MsDelay(int delayTime);
extern unsigned int GpioGetDir(unsigned int);

#endif /* __INCcustomSystemusrCallh */
