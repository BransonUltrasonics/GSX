/*
 * PRU-subsystem Core 0/1 driver for AM437x IDK
 *
 *
 * Copyright information - To be Done.
 *
 */


#ifndef _PRUSS_H_
#define _PRUSS_H_

/* defines */
#define TIESC_EEPROM_SIZE               (0x800)
#define PRU_CORE_CTRL_SOFT_RST_N_MASK   (0x00000001U)
#define PRU_CORE_CTRL_SOFT_RST_N_SHIFT  (0U)
/* PRUSS_SYSCFG register bits */
#define PRUSS_SYSCFG_SUB_MWAIT_READY    BIT(5)
#define PRUSS_SYSCFG_STANDBY_INIT       BIT(4)
    
#define PRUSS_SYSCFG_STANDBY_MODE_FORCE (0 << 2)
#define PRUSS_SYSCFG_STANDBY_MODE_NO    (1 << 2)
#define PRUSS_SYSCFG_STANDBY_MODE_SMART (2 << 2)
#define PRUSS_SYSCFG_STANDBY_MODE_MASK  (3 << 2)
    
#define PRUSS_SYSCFG_IDLE_MODE_FORCE    0
#define PRUSS_SYSCFG_IDLE_MODE_NO       1
#define PRUSS_SYSCFG_IDLE_MODE_SMART    2
#define PRUSS_SYSCFG_IDLE_MODE_MASK     3

#define PRUSS_CONTROL_STATUS            0x140

/* debug macro */

#define PRUSS_CORE_DEBUG
#ifdef PRUSS_CORE_DEBUG
#include <private/kwriteLibP.h>         /* _func_kprintf */

#define PRUICSS_CORE_DBG_OFF             0x00000000
#define PRUICSS_CORE_DBG_WARN            0x00000001
#define PRUICSS_CORE_DBG_ERR             0x00000002
#define PRUICSS_CORE_DBG_INFO            0x00000004
#define PRUICSS_CORE_DBG_ALL             0xffffffff
LOCAL UINT32 dbgMask      = PRUICSS_CORE_DBG_ALL;

#undef PRUICSS_CORE_DBG_MSG

#define PRUICSS_CORE_DBG_MSG(mask,...)                                      \
do                                                             \
{                                                              \
    if ((dbgMask & (mask)) || ((mask) == PRUICSS_CORE_DBG_ALL))            \
        {                                                      \
        if (_func_kprintf != NULL)                             \
            {                                                  \
            (* _func_kprintf)(__VA_ARGS__);                    \
            }                                                  \
        }                                                      \
}while (0)
#else
#define PRUICSS_CORE_DBG_MSG(...)
#endif  /* PRUSS_CORE_DEBUG */

/* Structure for PRU Core Device */
typedef struct pruicss_core
    {
    VXB_DEV_ID          pDev;               /* device information               */
    void *              regHandle;          /* Register handle                  */
    void *              regBase;            /* Register base address            */
    void *              pPsdrmRegBase;      /* Shared RAM Register base address */
    void *              pPsdrmRegHandle;    /* Shared RAM Register handle       */
    void *              pIramregBase;       /* Shared RAM Register base address */
    void *              pIramregHandle;     /* IRAM Register handle             */
    void *              pCtrlregBase;       /* Control Register base address    */
    void *              pCtrlregHandle;     /* Control Register handle          */
    void *              pDbgregBase;        /* Debug Register base address      */
    void *              pDbgregHandle;      /* Debug Register handle            */
    SEM_ID              semSync;            /* Semaphore ID                     */
    VXB_RESOURCE *      intRes;             /* vxbResource                      */
    struct vxbFdtDev    vxbFdtDev;          /* FDT Device Structure             */
    VXB_RESOURCE_LIST   vxbResList;         /* Resource list                    */
    } PRUICSS_CORE_CTRL;

/* For future use, PRUICSS Object */
/*
 * LOCAL PRUICSS_V1_Object           object;
 */

/* Pointer to PRU Core Device */
LOCAL PRUICSS_CORE_CTRL *    pDrvCtrl;

/* 4 Byte READ/WRITE MACROS */
#define PRU_CORE_BAR(p)        (((PRUICSS_CORE_CTRL *) vxbDevSoftcGet(p))->regBase)
#define PRU_CORE_HANDLE(p)     (((PRUICSS_CORE_CTRL *) vxbDevSoftcGet(p))->regHandle)
#undef CSR_READ_4
#define CSR_READ_4(pDev, addr)                                                  \
                    vxbRead32  (PRU_CORE_HANDLE(pDev),                          \
                               (uint32_t *)((char *)PRU_CORE_BAR(pDev) + addr))

#undef CSR_WRITE_4
#define CSR_WRITE_4(pDev, addr, data)                                           \
                    vxbWrite32 (PRU_CORE_HANDLE(pDev),                          \
                               (uint32_t *)((char *)PRU_CORE_BAR(pDev) + addr)  \
                               ,data)



LOCAL STATUS    pruCoreProbe        (VXB_DEV_ID     pDev);
LOCAL STATUS    pruCoreAttach       (VXB_DEV_ID     pDev);
LOCAL STATUS    pruCoreEnable       (VXB_DEV_ID     pDev
                                            ,int            pruNum);
LOCAL STATUS    pruCoreDisable      (VXB_DEV_ID     pDev
                                            ,int            pruNum);
LOCAL STATUS    pruCoreReset            (VXB_DEV_ID     pDev
                                            ,int            pruNum);
LOCAL STATUS    pruCoreBootFirmware (VXB_DEV_ID     pDev
                                            ,int            pruNum);
LOCAL STATUS    pruCoreWriteMemory  (VXB_DEV_ID     pDev
                                            ,uint32_t       pruMem
                                            ,uint32_t       wordoffset
                                            ,uint32_t     * source_mem
                                            ,uint32_t       bytelength
                                            ,int            pruNum
                                            );
LOCAL STATUS    pruCoreStringLookup (int            offset
                                            ,char         * propName
                                            ,char         * string
                                            );


STATUS   pruSharedMemTransfer_core(VXB_DEV_ID pDev, int num, uint32_t write_val);

STATUS  ReadByte_SHdram(VXB_DEV_ID pDev,uint32_t offset);

STATUS  ReadWord_SHdram(VXB_DEV_ID pDev,uint32_t offset);

STATUS  ReadDword_SHdram(VXB_DEV_ID pDev,uint32_t offset);

STATUS  WriteByte_SHdram(VXB_DEV_ID pDev,uint32_t offset, uint8_t data);

STATUS  WriteWord_SHdram(VXB_DEV_ID pDev,uint32_t offset, uint16_t data);

STATUS  WriteDword_SHdram(VXB_DEV_ID pDev,uint32_t offset, uint32_t data);

STATUS ReadBlock_SHdram(VXB_DEV_ID pDev,uint32_t offset, uint8_t *pData, uint32_t len);

STATUS ReadRange_SHdram(     VXB_DEV_ID pDev,	 uint32_t offset, uint32_t len    );

STATUS WriteBlock_SHdram(VXB_DEV_ID pDev,uint32_t offset, uint8_t *pData, uint32_t len);



#endif	/* _PRUSS_H_ */
