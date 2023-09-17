/*
 * PRU-subsystem driver for AM437x IDK
 *
 *
 * Copyright information - To be Done.
 *
 */



#ifndef __INCvxbFdtTiPrussEndh
#define __INCvxbFdtTiPrussEndh

#ifdef __cplusplus
extern "C" {
#endif

/* number of PRUs within a PRUSS */
#ifndef PRUSS_NUM_PRUS
#define PRUSS_NUM_PRUS		2
#endif


/* defines */
#define PRUICSS_IEP_DIGIO_CTRL                                 (0x300) 
#define PRUICSS_IEP_DIGIO_EXP                                  (0x318)

#define DRAM0 1
#define DRAM1 2
#define IEP 3

#define ESC_AL_EVENT_OFFSET                     0x0220
#define ESC_AL_CONTROL_OFFSET                   0x0120
#define ESC_AL_STATUS_OFFSET                    0x0130
#define ESC_AL_STATUS_CODE_OFFSET				0x0132
#define AL_CONTROL_EVENT					((unsigned short) 0x01)

#define MAX_SYNC_MAN 8

/* debug macro */

#undef PRUICSS_END_DEBUG
#ifdef PRUICSS_END_DEBUG
#include <private/kwriteLibP.h> /* _func_kprintf */

#define PRUICSS_END_DBG_OFF             0x00000000
#define PRUICSS_END_DBG_WARN            0x00000001
#define PRUICSS_END_DBG_ERR             0x00000002
#define PRUICSS_END_DBG_INFO            0x00000004
#define PRUICSS_END_DBG_ALL             0xffffffff
LOCAL UINT32 dbgMask      = PRUICSS_END_DBG_ALL;

#undef PRUICSS_END_DBG_MSG

#define PRUICSS_END_DBG_MSG(mask,...)                                      \
do                                                             \
{                                                              \
    if ((dbgMask & (mask)) || ((mask) == PRUICSS_END_DBG_ALL))             \
        {                                                      \
        if (_func_kprintf != NULL)                             \
            {                                                  \
            (* _func_kprintf)("%s,%d, ",__FUNCTION__,__LINE__);\
            (* _func_kprintf)(__VA_ARGS__);                    \
            }                                                  \
        }                                                      \
}while (0)

#define PRUICSS_END_DBG_MSG(fmt, args...)                       \
            do       {                                        \
                        if (_func_kprintf != NULL)            \
                            _func_kprintf (fmt, ##args);      \
                        } while (FALSE)
                        

#else
#define PRUICSS_END_DBG_MSG(...)
#endif  /* PRUICSS_END_DEBUG */

/* Structure for PRUICSS child Devices: PRU INTC, Cores 0/1, MDIO */
typedef struct PruicssDevInfo
    {
    struct vxbFdtDev  vxbFdtDev;
    VXB_RESOURCE_LIST vxbResList;
    } VXB_PRUICSS_INFO;

/* Structure for PRUICSS Device */
typedef struct pruicss
    {
    VXB_DEV_ID      pDev;           /* device information                   */
    VXB_FDT_DEV *   pFdtDev;        /* FDT information                      */
    void *          pRegbase;       /* Register base address                */
    void *          pHandle;        /* Register Handle                      */
    void *          pDram0Regbase;  /* DRAM0 Register base address          */
    void *          pDram0Handle;   /* DRAM0 Register handle                */
    void *          pDram1Regbase;  /* DRAM1 Register base address          */
    void *          pDram1Handle;   /* DRAM1 Register handle                */
    void *          pSDram1Regbase; /* Shared RAM1 Register base address    */
    void *          pSDram1Handle;  /* Shared RAM1 Register handle          */
    void *          pCfgRegbase;    /* Configuration Register base address  */
    void *          pCfgHandle;     /* Configuration Register handle        */
    void *          pIepRegbase;    /* IEP Register base address            */
    void *          pIepHandle;     /* IEP Register handle                  */
    void *          pMiiRtRegbase;  /* MII RT Register base address         */
    void *          pMiiRtHandle;   /* MII RT Register handle               */
    void *          pPsdrmRegHandle;/* Shared RAM Register handle           */
    void *          pPsdrmRegBase;  /* Shared RAM Register base address     */
    SEM_ID          pruSem;         /* PRU Semapore                         */         
    const char *    fw_name;        /* PRU Firmware name                    */
    }PRUICSS_CTRL;

    /* Struct for register permission array used in pruicss Attach procedure */
    typedef struct {
    	uint8_t reserved[1024];
    	uint8_t reg_properties[4096];
    } t_register_properties;


    typedef struct
    {
    	uint16_t physical_start_addr;
    	uint16_t length;
    } t_sm_properties;




#define PRUICSS_BAR(p)          (((PRUICSS_CTRL *) vxbDevSoftcGet(p))->pRegbase)
#define PRUICSS_HANDLE(p)       (((PRUICSS_CTRL *) vxbDevSoftcGet(p))->pHandle )


/* 4 Byte READ/WRITE MACROS */
#undef CSR_READ_4
#define CSR_READ_4(pDev, addr)                                                      \
                        vxbRead32 (PRUICSS_HANDLE(pDev),                            \
                                  (uint32_t *)((char *)PRUICSS_BAR(pDev) + addr))

#undef CSR_WRITE_4
#define CSR_WRITE_4(pDev, addr, data)               \
                        vxbWrite32 (PRUICSS_HANDLE(pDev),                           \
                                   (uint32_t *)((char *)PRUICSS_BAR(pDev) + addr),  \
                                   data)

				   
				   
#define	ESC_ADDR_TI_PORT0_TX_START_DELAY	0xE10
#define	ESC_ADDR_TI_PORT1_TX_START_DELAY	0xE12
#define TIESC_PORT0_TX_DELAY    			0x48
#define TIESC_PORT1_TX_DELAY    			TIESC_PORT0_TX_DELAY
#define	ESC_ADDR_MI_ECAT_ACCESS				0x516
#define	ESC_ADDR_MI_PDI_ACCESS				0x517
#define TIESC_PERM_RW       				0x0
#define TIESC_PERM_WRITE_ONLY 				0x1
#define TIESC_PERM_READ_ONLY 				0x2
#define ESC_ADDR_TI_PORT0_PHYADDR			0xE08
#define ESC_ADDR_TI_PORT1_PHYADDR			0xE09
#define	ESC_ADDR_TI_PDI_ISR_PINSEL			0xE0A
#define	ESC_ADDR_TI_PHY_LINK_POLARITY		0XE0C
#define PDI_ISR_EDIO_NUM    				7 //GPMC_CSN(2) -> pr1_edio_data_out7
											// for ICEv2.J4.Pin21
#define TIESC_LINK_POL_ACTIVE_LOW      	 	1
#define TIESC_LINK_POL_ACTIVE_HIGH      	0
#define TIESC_PERM_WRITE 					TIESC_PERM_WRITE_ONLY
#define TIESC_PERM_READ 					TIESC_PERM_READ_ONLY
#define PRU0_DATA_RAM						0x2000 // 8 kB
#define ICSS_SHARED_RAM						0x3000 // 12 kB
#define PRU1_DATA_RAM_RESERVED_REGION		0x400// 1 kB
#define PRU1_DATA_RAM_REG_PROP_REGION		0x1000// 4 kB

#define GPIO_BANK2_PIN23 0x57
#define GPIO_BANK2_PIN24 0x58  //Red
#define GPIO_BANK2_PIN25 0x59
#define GPIO_BANK2_PIN22 0x56
#define GPIO_BANK4_PIN13 0x8D
#define GPIO_BANK4_PIN16 0x90

#define CASE_ESC_TYPE 			1
#define CASE_ESC_REVISION 		2
#define CASE_ESC_BUILD_VERSION 	3
#define CASE_ESC_AL_CONTROL 	4
#define AL_STATUS				0x140
#define INVALID_CASE			-1
    

LOCAL STATUS            pruicssProbe        (VXB_DEV_ID         pDev);
LOCAL STATUS            pruicssAttach       (VXB_DEV_ID         pDev);
LOCAL STATUS            pruicssChildCreate  (VXB_DEV_ID         pDev,
                                                     VXB_FDT_DEV       * pFdtDev);
LOCAL VXB_FDT_DEV *     pruicssFdtDevGet    (VXB_DEV_ID         pDev,
                                                     VXB_DEV_ID          pChild);
LOCAL STATUS            pruicssResFree      (VXB_DEV_ID         pDev,
                                                     VXB_DEV_ID          pChild,
                                                     VXB_RESOURCE *      pRes);
LOCAL VXB_RESOURCE *    pruicssResAlloc     (VXB_DEV_ID         pDev,
                                                     VXB_DEV_ID          pChild,
                                                     UINT32              id);
LOCAL STATUS            pruicssRegbaseSetup (PRUICSS_CTRL *     pPruicssCtrl,
                                                     VXB_DEV_ID          pDev);
LOCAL STATUS            pruicssEnWatchDog   (PRUICSS_CTRL *     pPruicssCtrl,
                                                     VXB_DEV_ID          pDev);
LOCAL STATUS            pruicssEnDGIO       (PRUICSS_CTRL *     pPruicssCtrl,
                                                     VXB_DEV_ID          pDev);
LOCAL STATUS            pruicssStringLookup (int                offset,
                                                     char *              propName,
                                                     char *              string);
LOCAL STATUS            pruicssCmdIntfWrite (uint16_t           val,
                                                     volatile uint16_t * ptr);
LOCAL STATUS            pruicssRegPermInit  (VXB_DEV_ID         pDev);

STATUS pruSharedMemTransfer (VXB_DEV_ID pDev, int num);

STATUS  ReadByte_Dram_Iep(VXB_DEV_ID pDev,uint32_t offset, uint8_t op);

STATUS  ReadWord_Dram_Iep(VXB_DEV_ID pDev,uint32_t offset, uint8_t op);

STATUS  ReadDword_Dram_Iep(VXB_DEV_ID pDev,uint32_t offset, uint8_t op);

STATUS  WriteByte_Dram_Iep(VXB_DEV_ID pDev,uint32_t offset, uint8_t data, uint8_t op);

STATUS  WriteWord_Dram_Iep(VXB_DEV_ID pDev,uint32_t offset, uint16_t data, uint8_t op);

STATUS  WriteDword_Dram_Iep(VXB_DEV_ID pDev,uint32_t offset, uint32_t data, uint8_t op);

STATUS ReadBlock_Dram_Iep(VXB_DEV_ID pDev,uint32_t offset, uint8_t *pData, uint32_t len, uint8_t op);

STATUS WriteBlock_Dram_Iep(VXB_DEV_ID pDev,uint32_t offset, uint8_t *pData, uint32_t len, uint8_t op);





#ifdef __cplusplus
}
#endif

#endif /* __INCvxbFdtTiPrussEndh */

