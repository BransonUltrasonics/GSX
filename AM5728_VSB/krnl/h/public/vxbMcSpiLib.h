/* vxbMcSpiLib.h - VxBus Mc Spi interfaces header file */


/*
DESCRIPTION

This library provides the McSPI specific interfaces

*/

#ifndef __INCvxbMcSpiLibh
#define __INCvxbMcSpiLibh 

#ifdef __cplusplus
extern "C" {
#endif

#include <hwif/vxBus.h>
#include <hwif/util/vxbIsrDeferLib.h>
#include <hwif/buslib/vxbSpiLib.h>

/* structure to be used and updated RTC time and Alarm */

typedef struct vxbMcSpiFunctionality
    {
    STATUS (*mcspiTrans)(VXB_DEV_ID pInst, SPI_HARDWARE *  devInfo,SPI_TRANSFER *  pPkg);   
    } VXB_MC_SPI_FUNC;

struct vxbMcSpiHandle
    {
    struct vxbMcSpiFunctionality *pMcSpiFunc;
    VXB_DEV_ID                  pInst0;
    VXB_DEV_ID                  pInst1;
    VXB_DEV_ID                  pInst2;
    VXB_DEV_ID                  pInst3;
    VXB_DEV_ID                  pInst4;
    };

IMPORT STATUS vxbMcSpiTrans ( SPI_HARDWARE *  devInfo,SPI_TRANSFER *  pPkg, int channel);

IMPORT void mcspiRegister
    (
    VXB_DEV_ID pDev,
	VXB_MC_SPI_FUNC * mcspi,
	char    name[32]
    );
    
#ifdef __cplusplus
}
#endif

#endif /* __INCvxbMcSpiLibh */

