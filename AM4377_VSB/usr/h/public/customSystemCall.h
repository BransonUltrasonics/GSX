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
	INT32 tm_sec;         /* seconds after the minute     - [0, 59] */
	INT32 tm_min;         /* minutes after the hour       - [0, 59] */
	INT32 tm_hour;        /* hours after midnight         - [0, 23] */
	INT32 tm_mday;        /* day of the month             - [1, 31] */
	INT32 tm_mon;         /* months since January         - [0, 11] */
	INT32 tm_year;        /* years since 1900     */
	INT32 tm_wday;        /* days since Sunday            - [0, 6] */
	INT32 tm_yday;        /* days since January 1         - [0, 365] */
	INT32 tm_isdst;       /* Daylight Saving Time flag */
}RTCtimerStr;


/*
 * The vxbSpiDevInfo struct used to store the SPI device feature, such as
 * chipSelect, spi_device mode,etc.
 */

typedef struct vxbSpiDevInfo
    {
    /* SPI device name, used for match the corresponding driver */

    UINT8 *  deviceName;

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

/* SIO_CHAN STRUCTURE */
	
typedef struct sio_drv_funcs SIO_DRV_FUNCS;

typedef struct sio_chan				
	{
		SIO_DRV_FUNCS * pDrvFuncs;
	
	} SIO_CHAN;

 struct sio_drv_funcs				
 {
		int		(*ioctl)
		(
				SIO_CHAN *	pSioChan,
				int		cmd,
				void *		arg
		);

		int		(*txStartup)
		(
				SIO_CHAN *	pSioChan
		);

		int		(*callbackInstall)
		(
				SIO_CHAN *	pSioChan,
				int		callbackType,
				STATUS		(*callback)(void *, ...),
				void *		callbackArg
		);

		int		(*pollInput)
		(
				SIO_CHAN *	pSioChan,
				char *		inChar
		);

		int		(*pollOutput)
		(
				SIO_CHAN *	pSioChan,
				char 		outChar
		);
 };
 
 typedef enum intr_trigger {
     INTR_TRIGGER_CONFORM = 0,
     INTR_TRIGGER_EDGE = 1,
     INTR_TRIGGER_LEVEL = 2
 }INTR_TRIGGER;

 #define INTR_TRIGER INTR_TRIGGER

 typedef enum intr_polarity {
     INTR_POLARITY_CONFORM = 0,
     INTR_POLARITY_HIGH = 1,
     INTR_POLARITY_LOW = 2,
     INTR_POLARITY_BOTH   /* valid for edge only, e.g. double edge trigger */
 }INTR_POLARITY;
 
// EST_LIFE est_life;
 typedef struct 
 	{
 	 UINT32 LifeTimeEstA;
      UINT32 LifeTimeEstB;
      UINT32 PreEOLInfo;
    } EST_LIFE;
 
// Exposing custom system calls to RTP     
extern INT32 AuxClkConnect(INT32 task_id);
extern void AuxClkEnable(void);
extern void AuxClkDisable(void);
extern INT32 AuxClkRateGet(void);
extern INT32 AuxClkRateSet(INT32 ticksPerSecond);
extern STATUS SetRTC(RTCtimerStr time);
extern STATUS GetRTC(RTCtimerStr * pTime);
extern INT32 GetRTClowPwr(RTCtimerStr * pTime);
extern INT32 GpioAlloc(UINT32 id);
extern UINT32 GpioGetValue(UINT32 id);
extern INT32 GpioSetValue(UINT32 id, UINT32 value);
extern INT32 GpioSetDir(UINT32 id, UINT32 value);
extern INT32 GpioGetDir(UINT32 id);
extern INT32 GpioFree(UINT32 id);
extern INT32 EhrPwmRegisterISR(EHRPWMDRV_IRQ_HANDLER irqHandler, void* ehrpwmHandlerArgs, UINT8 ehrpwm_inst, INT32 period, INT32 mode);
extern STATUS EhrPwmSetAQSFRC(UINT32 val, UINT8	ehrpwm_inst);
extern STATUS EhrPwmSetTbClk(UINT32 tb, UINT8	ehrpwm_inst);
extern STATUS EhrPwmSetTbAQConfA(UINT32 tb, UINT8	ehrpwm_inst);
extern STATUS EhrPwmSetTbAQConfB(UINT32 tb, UINT8	ehrpwm_inst);
extern STATUS EhrPwmSetTbPWMCfg(UINT32 freq, UINT32 dir, UINT8 ehrpwm_inst);
extern STATUS EqepSetMaxCount(INT32 eqep, INT32 count);
extern STATUS EqepGetPosCount(INT32 eqep);
extern STATUS McSpiTrans(SPI_HARDWARE *  devInfo, SPI_TRANSFER *  pPkg, INT32	channel);
extern void MsDelay(INT32 delayTime);

extern STATUS logCRIT(char *logData, uint16_t lineNo, const char *fileName, uint16_t logLevel, uint16_t logToFile, _Vx_usr_arg_t arg1, _Vx_usr_arg_t arg2, _Vx_usr_arg_t arg3); 
extern STATUS logERR(char *logData,  uint16_t lineNo, const char *fileName, uint16_t logLevel, uint16_t logToFile, _Vx_usr_arg_t arg1, _Vx_usr_arg_t arg2, _Vx_usr_arg_t arg3);
extern STATUS logWARN(char *logData, uint16_t lineNo, const char *fileName, uint16_t logLevel, uint16_t logToFile, _Vx_usr_arg_t arg1, _Vx_usr_arg_t arg2, _Vx_usr_arg_t arg3);
extern STATUS logDBG(char *logData,  uint16_t lineNo, const char *fileName, uint16_t logLevel, uint16_t logToFile, _Vx_usr_arg_t arg1, _Vx_usr_arg_t arg2, _Vx_usr_arg_t arg3);

extern INT32 Am5728EhrPwmRegisterISR(EHRPWMDRV_IRQ_HANDLER irqHandler, void* ehrpwmHandlerArgs, UINT8 ehrpwm_inst, INT32 period, INT32 mode);
extern STATUS Am5728EhrPwmSetAQSFRC(UINT32 val, UINT8	ehrpwm_inst);
extern STATUS Am5728EhrPwmSetTbClk(UINT32 tb, UINT8	ehrpwm_inst);
extern STATUS Am5728EhrPwmSetTbAQConfA(UINT32 tb, UINT8	ehrpwm_inst);
extern STATUS Am5728EhrPwmSetTbAQConfB(UINT32 tb, UINT8	ehrpwm_inst);
extern STATUS Am5728EhrPwmSetTbPWMCfg(UINT32 freq, UINT32 dir, UINT8 ehrpwm_inst);

extern STATUS RunTimeFeature(void);
extern STATUS IPConfiguration(char *ipAddr, char *subnetAddr, char *gatewayAddr, int subnetMask);

/* QSPI API's for AM5728 */
extern void QSPIFlashErase(uint32_t offset, uint32_t length);
extern STATUS QSPIFlashWrite(uint32_t offset, uint32_t length, unsigned int src);
extern STATUS QSPIFlashRead(uint32_t offset, uint32_t length, unsigned int dest) ;

/* QSPI API's for AM437x */
extern void QspiLibInit();
extern void QspiLibPrintId();
extern void QspiLibBlockErase(unsigned int block);
extern int QspiLibWrite (UINT32 dstOffsetAddr, UINT32 srcAddr, UINT32 length);
extern int QspiLibRead (UINT32 srcOffsetAddr, UINT32 dstAddr, UINT32 length);

extern INT32 SioPollInput(SIO_CHAN *pChan,  char *pChar);
extern INT32 SioPollOutput(SIO_CHAN *pChan, char outChar);
extern INT32 GpioIntConnect(UINT32 id, VOIDFUNCPTR pIsr, void *pArg );
extern INT32 GpioIntEnable(UINT32 id, VOIDFUNCPTR pIsr, void *pArg );        
extern INT32 GpioIntConfig(UINT32 id, INTR_TRIGER trig, INTR_POLARITY pol );
extern float TimeStamp();
extern STATUS Gpio24VLowShutdown(void);

extern STATUS PcGpio15VLow(void);
extern STATUS PcGpio5VLow(void);

extern STATUS EmmcExtCsdDecode(EST_LIFE * emmc_est_life);
#endif /* __INCcustomSystemusrCallh */
