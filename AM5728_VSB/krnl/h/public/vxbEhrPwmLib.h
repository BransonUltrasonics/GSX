/* vxbEhrPwmLib.h - VxBus EhrPwm interfaces header file */

/*
 * Copyright - To be Done
 */

/*
DESCRIPTION

This library provides the EhrPwm specific interfaces

*/

#ifndef __INCvxbEhrPwmLibh
#define __INCvxbEhrPwmLibh 

#include <hwif/vxBus.h>
#include <hwif/util/vxbIsrDeferLib.h>
#ifdef __cplusplus
extern "C" {
#endif



#define TBCNT_EQ_ZERO    			0
#define TBCNT_EQ_TBPRD   			1
#define TBCNT_EQ_INC_CMPA 			2
#define TBCNT_EQ_DEC_CMPA 			3
#define TBCNT_EQ_INC_CMPB 			4	 
#define TBCNT_EQ_DEC_CMPB 			5

#define ETPS_INTPRD_DISABLE 		0
#define ETPS_INTPRD_GEN_FIRST_EVT 	1
#define ETPS_INTPRD_GEN_SECOND_EVT 	2
#define ETPS_INTPRD_GEN_THIRD_EVT 	3

#define  EHRPWM  	0x01
#define  EQEP   	0x02
#define  ECAP   	0x03

/* structure to be used and updated RTC time and Alarm */
typedef void (*EHRPWMDRV_IRQ_HANDLER)(void);
typedef void (*EHRPWMDRV_CALLBACK)(void*);
typedef struct  vxbEhrPwmIrqStruct
{
	VXB_DEV_ID inst;
	EHRPWMDRV_IRQ_HANDLER func;
}EHRPWMDRV_IRQ_STRUCT;

typedef struct vxbEhrPwmFunctionality
    {
    STATUS (*ehrpwmGet)(VXB_DEV_ID pInst, struct tm * rtc_time);
    STATUS (*ehrpwmSet)(VXB_DEV_ID pInst, struct tm * rtc_time);
    STATUS (*ehrpwmGetTbClk)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbClk)(VXB_DEV_ID pInst,uint32_t tb);
    STATUS (*ehrpwmGetTbPWMFreq)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbPWMFreq)(VXB_DEV_ID pInst,uint32_t tb,uint32_t dir);
    STATUS (*ehrpwmGetTbPWMDir)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbPWMDir)(VXB_DEV_ID pInst,uint32_t tb);
    STATUS (*ehrpwmGetTbPWMCfg)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbPWMCfg)(VXB_DEV_ID pInst,uint32_t tb,uint32_t dir);
    STATUS (*ehrpwmGetTbEmulationMode)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbEmulationMode)(VXB_DEV_ID pInst,uint32_t tb);
    STATUS (*ehrpwmGetTbCOMPAConf)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbCOMPAConf)(VXB_DEV_ID pInst,uint32_t tb);
    STATUS (*ehrpwmGetTbCOMPBConf)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbCOMPBConf)(VXB_DEV_ID pInst,uint32_t tb);
    STATUS (*ehrpwmGetTbAQConfA)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbAQConfA)(VXB_DEV_ID pInst,uint32_t tb);
    STATUS (*ehrpwmGetTbAQConfB)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbAQConfB)(VXB_DEV_ID pInst,uint32_t tb);
    STATUS (*ehrpwmGetCounter)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetCounter)(VXB_DEV_ID pInst,uint16_t counter);
    STATUS (*ehrpwmGetPRD)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmGetTbStatus)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmPWMRegisterISR)(VXB_DEV_ID pInst,EHRPWMDRV_IRQ_HANDLER irqHandler,void* ehrpwmHandlerArgs,int period,int mode);
    STATUS (*ehrpwmGetIntStatus)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmEnableInt)(VXB_DEV_ID pInst,int period, int mode);
    STATUS (*ehrpwmDisableInt)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetEmulation)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetAQSFRC)(VXB_DEV_ID pInst,UINT32 val);
    STATUS (*ehrpwmGetAQSFRC)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTBCTL)(VXB_DEV_ID pInst,UINT32 val);
    STATUS (*ehrpwmGetTBCTL)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetIntEvtTBCTL)(VXB_DEV_ID pInst,UINT32 mode);
	STATUS (*ehrpwmSetIntPrdTBCTL)(VXB_DEV_ID pInst,UINT32 mode);
    STATUS (*ehrpwmGetIntEvtTBCTL)(VXB_DEV_ID pInst);
	STATUS (*ehrpwmGetIntPrdTBCTL)(VXB_DEV_ID pInst);
	} VXB_EHRPWM_FUNC;

typedef struct vxbPwmssFunctionality
	{
	STATUS (*ehrpwmClkConfigEnable)(VXB_DEV_ID pInst,int module);
	STATUS (*ehrpwmClkConfigDisable)(VXB_DEV_ID pInst,int module);
	} VXB_PWMSS_FUNC;
	
struct vxbEhrPwmHandle
    {
    struct vxbEhrPwmFunctionality *pEhrPwmFunc;
    VXB_DEV_ID                  pEhrPwmInst;
    };

struct vxbPwmssHandle
    {
    struct vxbPwmssFunctionality *pPwmssFunc;
    VXB_DEV_ID                  pPwmssInst;
    };
/* IMPORT */

IMPORT STATUS vxbPwmssDisable (int pwmss,int module);
IMPORT STATUS vxbPwmssEnable (int pwmss,int module);
IMPORT STATUS vxbEhrPwmGetTbClk (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetTbClk (uint32_t tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetTbPWMFreq (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetTbPWMFreq (uint32_t tb,uint32_t dir, UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetTbPWMDir (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetTbPWMDir (uint32_t tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetTbPWMCfg (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetTbPWMCfg (uint32_t freq,uint32_t dir, UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetTbEmulationMode (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetTbEmulationMode (uint32_t tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetCMPA (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetCMPA (uint32_t tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetCMPB (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetCMPB (uint32_t tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetTbAQConfA (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetTbAQConfA (uint32_t tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetTbAQConfB (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetTbAQConfB (uint32_t tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetTbPRD (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetTbCounter (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetTbStatus (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmRegisterISR (EHRPWMDRV_IRQ_HANDLER irqHandler,void* ehrpwmHandlerArgs, UINT8	ehrpwm_inst,int period,int mode);
IMPORT STATUS vxbEhrPwmEnableInt (UINT8	ehrpwm_inst,int period, int mode);
IMPORT STATUS vxbEhrPwmDisableInt (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmIncInt (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetEmulation (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetAQSFRC (UINT32 val,UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetAQSFRC (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetTBCTL (UINT32 val,UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetTBCTL (UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetIntEvt(UINT32 mode,UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetIntPrd(UINT32 mode,UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetIntEvt(UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmGetIntPrd(UINT8	ehrpwm_inst);
IMPORT STATUS vxbEhrPwmSetTbCounter(UINT16  counter,UINT8	ehrpwm_inst);

IMPORT void ehrpwmRegister
    (
    VXB_DEV_ID pDev,
	VXB_EHRPWM_FUNC * ehrpwm,
	char    name[32]
    );
IMPORT void pwmssRegister
    (
    VXB_DEV_ID pDev,
    VXB_PWMSS_FUNC * pwmss,
    char    name[32]
    )  ; 
#ifdef __cplusplus
}
#endif

#endif /* __INCvxbEhrPwmLibh */

