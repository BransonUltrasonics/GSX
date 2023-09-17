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
    STATUS (*Am5728ehrpwmGet)(VXB_DEV_ID pInst, struct tm * rtc_time);
    STATUS (*Am5728ehrpwmSet)(VXB_DEV_ID pInst, struct tm * rtc_time);
    STATUS (*Am5728ehrpwmGetTbClk)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetTbClk)(VXB_DEV_ID pInst,UINT32 tb);
    STATUS (*Am5728ehrpwmGetTbPWMFreq)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetTbPWMFreq)(VXB_DEV_ID pInst,UINT32 tb,UINT32 dir);
    STATUS (*Am5728ehrpwmGetTbPWMDir)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetTbPWMDir)(VXB_DEV_ID pInst,UINT32 tb);
    STATUS (*Am5728ehrpwmGetTbPWMCfg)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetTbPWMCfg)(VXB_DEV_ID pInst,UINT32 tb,UINT32 dir);
    STATUS (*Am5728ehrpwmGetTbEmulationMode)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetTbEmulationMode)(VXB_DEV_ID pInst,UINT32 tb);
    STATUS (*Am5728ehrpwmGetTbCOMPAConf)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetTbCOMPAConf)(VXB_DEV_ID pInst,UINT32 tb);
    STATUS (*Am5728ehrpwmGetTbCOMPBConf)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetTbCOMPBConf)(VXB_DEV_ID pInst,UINT32 tb);
    STATUS (*Am5728ehrpwmGetTbAQConfA)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetTbAQConfA)(VXB_DEV_ID pInst,UINT32 tb);
    STATUS (*Am5728ehrpwmGetTbAQConfB)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetTbAQConfB)(VXB_DEV_ID pInst,UINT32 tb);
    STATUS (*Am5728ehrpwmGetCounter)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetCounter)(VXB_DEV_ID pInst,UINT16 counter);
    STATUS (*Am5728ehrpwmGetPRD)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmGetTbStatus)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmPWMRegisterISR)(VXB_DEV_ID pInst,EHRPWMDRV_IRQ_HANDLER irqHandler,void* ehrpwmHandlerArgs,int period,int mode);
    STATUS (*Am5728ehrpwmGetIntStatus)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmEnableInt)(VXB_DEV_ID pInst,int period, int mode);
    STATUS (*Am5728ehrpwmDisableInt)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetEmulation)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetAQSFRC)(VXB_DEV_ID pInst,UINT32 val);
    STATUS (*Am5728ehrpwmGetAQSFRC)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetTBCTL)(VXB_DEV_ID pInst,UINT32 val);
    STATUS (*Am5728ehrpwmGetTBCTL)(VXB_DEV_ID pInst);
    STATUS (*Am5728ehrpwmSetIntEvtTBCTL)(VXB_DEV_ID pInst,UINT32 mode);
	STATUS (*Am5728ehrpwmSetIntPrdTBCTL)(VXB_DEV_ID pInst,UINT32 mode);
    STATUS (*Am5728ehrpwmGetIntEvtTBCTL)(VXB_DEV_ID pInst);
	STATUS (*Am5728ehrpwmGetIntPrdTBCTL)(VXB_DEV_ID pInst);
	} VXB_EHRPWM_FUNC;

typedef struct vxbPwmssFunctionality
	{
	STATUS (*Am5728ehrpwmClkConfigEnable)(VXB_DEV_ID pInst,int module);
	STATUS (*Am5728ehrpwmClkConfigDisable)(VXB_DEV_ID pInst,int module);
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

IMPORT STATUS vxbAm5728PwmssDisable (int pwmss,int module);
IMPORT STATUS vxbAm5728PwmssEnable (int pwmss,int module);
IMPORT STATUS vxbAm5728EhrPwmGetTbClk (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetTbClk (UINT32 tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetTbPWMFreq (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetTbPWMFreq (UINT32 tb,UINT32 dir, UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetTbPWMDir (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetTbPWMDir (UINT32 tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetTbPWMCfg (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetTbPWMCfg (UINT32 freq,UINT32 dir, UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetTbEmulationMode (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetTbEmulationMode (UINT32 tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetCMPA (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetCMPA (UINT32 tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetCMPB (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetCMPB (UINT32 tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetTbAQConfA (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetTbAQConfA (UINT32 tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetTbAQConfB (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetTbAQConfB (UINT32 tb, UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetTbPRD (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetTbCounter (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetTbStatus (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmRegisterISR (EHRPWMDRV_IRQ_HANDLER irqHandler,void* ehrpwmHandlerArgs, UINT8	ehrpwm_inst,int period,int mode);
IMPORT STATUS vxbAm5728EhrPwmEnableInt (UINT8	ehrpwm_inst,int period, int mode);
IMPORT STATUS vxbAm5728EhrPwmDisableInt (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmIncInt (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetEmulation (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetAQSFRC (UINT32 val,UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetAQSFRC (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetTBCTL (UINT32 val,UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetTBCTL (UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetIntEvt(UINT32 mode,UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetIntPrd(UINT32 mode,UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetIntEvt(UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmGetIntPrd(UINT8	ehrpwm_inst);
IMPORT STATUS vxbAm5728EhrPwmSetTbCounter(UINT16  counter,UINT8	ehrpwm_inst);

IMPORT void Am5728ehrpwmRegister
    (
    VXB_DEV_ID pDev,
	VXB_EHRPWM_FUNC * ehrpwm,
	char    name[32]
    );
IMPORT void Am5728pwmssRegister
    (
    VXB_DEV_ID pDev,
    VXB_PWMSS_FUNC * pwmss,
    char    name[32]
    )  ; 
#ifdef __cplusplus
}
#endif

#endif /* __INCvxbEhrPwmLibh */

